#!/usr/bin/env python3
"""
run_fmu_udp.py  —  Drive LogicModel2 FMU with UDP frames
=========================================================

Loads LogicModel2.fmu (FMI 2.0 Co-Simulation, Stellantis LRCF).
On every UDP frame received (or dummy frame generated), the script:
  1. Writes the frame payload into a pinned ctypes buffer
  2. Sets EthRxIn.lo / .hi / .size  (VR 5000-5002) on the FMU
  3. Calls fmi2DoStep (1 ms step @ 1 kHz)
  4. Reads EthTxOut.lo / .hi / .size (VR 5003-5005) and logs TX output

Usage:
  python run_fmu_udp.py [OPTIONS]

  --fmu PATH              Path to LogicModel2.fmu  (default: ../Build/LogicModel2.fmu)
  --port PORT             UDP listen port          (default: 7400)
  --host HOST             UDP bind address         (default: 0.0.0.0)
  --dummy                 Generate synthetic frames instead of listening on a socket
  --rate HZ               Dummy frame rate in Hz   (default: 20)
  --steps N               Stop after N steps       (default: run until Ctrl-C)
  --verbose               Print a line for every step (default: first 5 + every 100th)
  --verbose-basename PATH Base file path for SIL debug output files
                          (.dvl / .dvsu / .srr3 / .csv).
                          Default: SIL_Output/<timestamp>/virtual.mf4  next to this script.
                          Pass an empty string ("") to disable verbose output.
"""

import argparse
import ctypes
import datetime
from logging import info
import os
import platform
import queue
import shutil
import socket
import struct
import sys
import tempfile
import threading
import time
import xml.etree.ElementTree as ET
import zipfile

# ─────────────────────────────────────────────────────────────────────────────
#  FMI 2.0 constants
# ─────────────────────────────────────────────────────────────────────────────

FMI2_OK      = 0
FMI2_WARNING = 1
FMI2_DISCARD = 2
FMI2_ERROR   = 3
FMI2_FATAL   = 4
FMI2_PENDING = 5

# ─────────────────────────────────────────────────────────────────────────────
#  Well-known value references  (from modelDescription.xml)
# ─────────────────────────────────────────────────────────────────────────────

# Ethernet pointer-triplet — RX (input)
VR_ETH_RX_LO   = 5000
VR_ETH_RX_HI   = 5001
VR_ETH_RX_SIZE = 5002

# Ethernet pointer-triplet — TX (output)
VR_ETH_TX_LO   = 5003
VR_ETH_TX_HI   = 5004
VR_ETH_TX_SIZE = 5005

# Config parameters (set during initialisation mode)
VR_CFG_ETH_RX_IP        = 100   # String
VR_CFG_ETH_RX_PORT      = 101   # Integer
VR_CFG_ETH_TX_IP        = 102   # String
VR_CFG_ETH_TX_PORT      = 103   # Integer
VR_CFG_SENSOR_POS_X     = 110   # Real  [m]
VR_CFG_SENSOR_POS_Y     = 111   # Real  [m]
VR_CFG_SENSOR_POS_Z     = 112   # Real  [m]
VR_CFG_SENSOR_ORI_ROLL  = 113   # Real  [deg]
VR_CFG_SENSOR_ORI_PITCH = 114   # Real  [deg]
VR_CFG_SENSOR_ORI_YAW   = 115   # Real  [deg]
VR_CFG_VERBOSE_BASENAME  = 116   # String — base path for SIL debug output files

# Sample CAN TX output — used for per-step health logging
VR_SAMPLE_CAN_TX_REAL = 1215   # VCAN_TX.FD15_LRCF_DATA_7.VHL_1_LEFT_ANGLE
VR_SAMPLE_CAN_TX_INT  = 1205   # VCAN_TX.FD15_LRCF_DATA_7.VHL_1_CL_SUB_TYPE

# Sample CAN RX inputs for optional per-step dummy stimulation
VR_CAN_RX_YAW_RATE           = 1039   # VCAN_RX.FD15_ORC_DATA_1.YAW_RATE
VR_CAN_RX_LAT_ACCELERATION    = 1040   # VCAN_RX.FD15_ORC_DATA_1.LAT_ACCELERATION
VR_CAN_RX_LONG_ACCELERATION   = 1041   # VCAN_RX.FD15_ORC_DATA_1.LONG_ACCELERATION
VR_CAN_RX_LONG_ACCELERATION_FAULT_STS = 1042   # VCAN_RX.FD15_ORC_DATA_1.LONG_ACCELERATION_FAULT_STS
VR_CAN_RX_LAT_ACCELERATION_FAULT_STS  = 1043   # VCAN_RX.FD15_ORC_DATA_1.LAT_ACCELERATION_FAULT_STS
VR_CAN_RX_YAW_RATE_FAULT_STS  = 1044   # VCAN_RX.FD15_ORC_DATA_1.YAW_RATE_FAULT_STS

# FMU step size
STEP_SIZE_S   = 0.001   # 1 ms  (1 kHz)
ETH_PAYLOAD   = 1500    # max Ethernet frame bytes
ETH_HEADER    = 42      # Ethernet(14) + IPv4(20) + UDP(8)
IPV4_HEADER   = 20    # (assuming no IP options)
UDP_HEADER    = 8   # (assuming no UDP options)
# ─────────────────────────────────────────────────────────────────────────────
#  Model-description parser
# ─────────────────────────────────────────────────────────────────────────────

class VarMeta:
    """Lightweight descriptor for a single ScalarVariable."""
    __slots__ = ('name', 'vr', 'causality', 'variability', 'type_')

    def __init__(self, name, vr, causality, variability, type_):
        self.name        = name
        self.vr          = vr
        self.causality   = causality
        self.variability = variability
        self.type_       = type_   # 'Integer' | 'Real' | 'Boolean' | 'String'


def parse_model_description(xml_path):
    """Return (inputs, outputs, params) as lists of VarMeta."""
    tree = ET.parse(xml_path)
    root = tree.getroot()
    inputs, outputs, params = [], [], []

    for sv in root.iter('ScalarVariable'):
        name        = sv.get('name', '')
        vr          = int(sv.get('valueReference', '-1'))
        causality   = sv.get('causality', '')
        variability = sv.get('variability', '')
        type_       = next(
            (child.tag for child in sv
             if child.tag in ('Integer', 'Real', 'Boolean', 'String')),
            'Unknown'
        )
        vm = VarMeta(name, vr, causality, variability, type_)
        if causality == 'input':
            inputs.append(vm)
        elif causality == 'output':
            outputs.append(vm)
        elif causality == 'parameter':
            params.append(vm)

    return inputs, outputs, params


def read_cosim_attrs(xml_path):
    """Return (model_identifier, guid) from the modelDescription root."""
    tree = ET.parse(xml_path)
    root = tree.getroot()
    guid  = root.get('guid', '')
    cosim = root.find('CoSimulation')
    model_id = cosim.get('modelIdentifier', 'LogicModel2') if cosim else 'LogicModel2'
    return model_id, guid


# ─────────────────────────────────────────────────────────────────────────────
#  FMI 2.0 ctypes wrappers
# ─────────────────────────────────────────────────────────────────────────────

def _ok(st, label):
    """Raise if status is worse than FMI2_WARNING."""
    if st not in (FMI2_OK, FMI2_WARNING):
        raise RuntimeError(f"{label} failed with FMI status {st}")


def fmi_set_integers(lib, comp, vr_list, val_list):
    n    = len(vr_list)
    vrs  = (ctypes.c_uint * n)(*vr_list)
    vals = (ctypes.c_int  * n)(*val_list)
    return lib.fmi2SetInteger(comp, vrs, n, vals)


def fmi_get_integers(lib, comp, vr_list):
    n    = len(vr_list)
    vrs  = (ctypes.c_uint * n)(*vr_list)
    out  = (ctypes.c_int  * n)(*([0] * n))
    st   = lib.fmi2GetInteger(comp, vrs, n, out)
    return st, list(out)


def fmi_set_reals(lib, comp, vr_list, val_list):
    n    = len(vr_list)
    vrs  = (ctypes.c_uint  * n)(*vr_list)
    vals = (ctypes.c_double * n)(*val_list)
    return lib.fmi2SetReal(comp, vrs, n, vals)


def fmi_get_reals(lib, comp, vr_list):
    n    = len(vr_list)
    vrs  = (ctypes.c_uint  * n)(*vr_list)
    out  = (ctypes.c_double * n)(*([0.0] * n))
    st   = lib.fmi2GetReal(comp, vrs, n, out)
    return st, list(out)


def fmi_get_booleans(lib, comp, vr_list):
    n    = len(vr_list)
    vrs  = (ctypes.c_uint * n)(*vr_list)
    out  = (ctypes.c_int * n)(*([0] * n))
    st   = lib.fmi2GetBoolean(comp, vrs, n, out)
    return st, [bool(v) for v in out]


def fmi_set_string(lib, comp, vr, value: str):
    vrs  = (ctypes.c_uint * 1)(vr)
    enc  = value.encode()
    vals = (ctypes.c_char_p * 1)(enc)
    return lib.fmi2SetString(comp, vrs, 1, vals)


# ─────────────────────────────────────────────────────────────────────────────
#  Pointer-triplet helpers (64-bit address ↔ two signed int32)
# ─────────────────────────────────────────────────────────────────────────────

def ptr_to_lo_hi(addr: int):
    """Split a 64-bit pointer into signed int32 (lo, hi) for FMI SetInteger."""
    addr &= 0xFFFFFFFFFFFFFFFF          # ensure unsigned 64-bit
    lo    =  addr        & 0xFFFFFFFF
    hi    = (addr >> 32) & 0xFFFFFFFF
    if lo >= 0x80000000:
        lo -= 0x100000000               # reinterpret as signed
    if hi >= 0x80000000:
        hi -= 0x100000000
    return lo, hi


def lo_hi_to_ptr(lo: int, hi: int) -> int:
    """Reconstruct 64-bit address from signed int32 lo/hi."""
    return ((hi & 0xFFFFFFFF) << 32) | (lo & 0xFFFFFFFF)


# ─────────────────────────────────────────────────────────────────────────────
#  FMU loader — extract .fmu ZIP, load shared library, manage lifecycle
# ─────────────────────────────────────────────────────────────────────────────

class FMULoader:
    def __init__(self, fmu_path: str):
        self.fmu_path     = os.path.abspath(fmu_path)
        self._tmp_dir     = tempfile.mkdtemp(prefix='lm2_fmu_')
        self._binary_path = None
        self._xml_path    = None
        self.lib          = None
        self.comp         = None

    # ── Extraction ───────────────────────────────────────────────────────────

    def extract(self) -> str:
        """Unzip FMU; return path to extracted modelDescription.xml."""
        print(f"[FMU] Extracting  {self.fmu_path}")
        with zipfile.ZipFile(self.fmu_path, 'r') as zf:
            zf.extractall(self._tmp_dir)

        self._xml_path = os.path.join(self._tmp_dir, 'modelDescription.xml')
        if not os.path.isfile(self._xml_path):
            raise FileNotFoundError("modelDescription.xml not found inside FMU archive")

        sys_plat = platform.system()
        if sys_plat == 'Windows':
            subdir, ext = 'win64',   '.dll'
        elif sys_plat == 'Darwin':
            subdir, ext = 'darwin64', '.dylib'
        else:
            subdir, ext = 'linux64', '.so'

        bin_dir    = os.path.join(self._tmp_dir, 'binaries', subdir)
        candidates = [f for f in os.listdir(bin_dir)
                      if f.endswith(ext)] if os.path.isdir(bin_dir) else []
        if not candidates:
            raise FileNotFoundError(
                f"No {ext} binary found in binaries/{subdir}/  "
                f"(platform={sys_plat})")

        # The FMU spec mandates: shared library filename == modelIdentifier + ext.
        # os.listdir() returns filesystem order (not alphabetical), so
        # candidates[0] may be a helper lib (e.g. libSRR3_MUDP_Log.so) rather
        # than the FMU entry-point, causing AttributeError on fmi2Instantiate.
        _root  = ET.parse(self._xml_path).getroot()
        _cosim = _root.find('CoSimulation')
        _mid   = _cosim.get('modelIdentifier', '') if _cosim is not None else ''
        _want  = _mid + ext            # e.g. "LogicModel2.so"
        if _want in candidates:
            chosen = _want
        else:
            # Fallback: prefer names that don't begin with 'lib' (helper convention)
            main_cands = [f for f in candidates if not f.startswith('lib')]
            chosen = (main_cands or candidates)[0]
            print(f"[FMU][WARN] Expected '{_want}' but not found; using '{chosen}'")

        self._binary_path = os.path.join(bin_dir, chosen)
        print(f"[FMU] Binary      {self._binary_path}")
        return self._xml_path

    # ── Library loading ──────────────────────────────────────────────────────

    def load_library(self):
        """dlopen / LoadLibrary the shared object and configure argtypes."""
        # RTLD_LAZY (0x1): defer symbol resolution until first call, which
        # avoids circular-dependency failures at load time.
        # RTLD_GLOBAL: put LogicModel2.so exports in the global symbol table so
        # sibling libraries (libSRR3_MUDP_Log.so etc.) can back-reference them.
        _RTLD_LAZY = 0x1
        self.lib = ctypes.CDLL(self._binary_path, mode=_RTLD_LAZY | ctypes.RTLD_GLOBAL)
        lib = self.lib

        lib.fmi2Instantiate.restype             = ctypes.c_void_p
        lib.fmi2SetupExperiment.restype         = ctypes.c_int
        lib.fmi2SetupExperiment.argtypes        = [
            ctypes.c_void_p,   # component
            ctypes.c_int,      # toleranceDefined
            ctypes.c_double,   # tolerance
            ctypes.c_double,   # startTime
            ctypes.c_int,      # stopTimeDefined
            ctypes.c_double,   # stopTime
        ]
        lib.fmi2EnterInitializationMode.restype = ctypes.c_int
        lib.fmi2EnterInitializationMode.argtypes = [ctypes.c_void_p]
        lib.fmi2ExitInitializationMode.restype  = ctypes.c_int
        lib.fmi2ExitInitializationMode.argtypes = [ctypes.c_void_p]
        lib.fmi2Terminate.restype               = ctypes.c_int
        lib.fmi2Terminate.argtypes              = [ctypes.c_void_p]
        lib.fmi2FreeInstance.restype            = None
        lib.fmi2FreeInstance.argtypes           = [ctypes.c_void_p]
        lib.fmi2DoStep.restype                  = ctypes.c_int
        lib.fmi2DoStep.argtypes                 = [
            ctypes.c_void_p,
            ctypes.c_double,   # currentCommunicationPoint
            ctypes.c_double,   # communicationStepSize
            ctypes.c_int,      # noSetFMUStatePriorToCurrentPoint
        ]
        lib.fmi2SetInteger.restype  = ctypes.c_int
        lib.fmi2GetInteger.restype  = ctypes.c_int
        lib.fmi2SetReal.restype     = ctypes.c_int
        lib.fmi2GetReal.restype     = ctypes.c_int
        lib.fmi2SetString.restype   = ctypes.c_int
        lib.fmi2GetBoolean.restype  = ctypes.c_int

        print("[FMU] Library loaded")

    # ── Lifecycle ────────────────────────────────────────────────────────────

    def instantiate(self, model_identifier: str, guid: str):
        lib = self.lib
        comp = lib.fmi2Instantiate(
            model_identifier.encode(),
            ctypes.c_int(1),        # fmi2CoSimulation
            guid.encode(),
            b"",                    # fmuResourceLocation
            ctypes.c_void_p(0),     # callbacks (NULL → default)
            ctypes.c_int(0),        # visible
            ctypes.c_int(0),        # loggingOn
        )
        if not comp:
            raise RuntimeError("fmi2Instantiate returned NULL — check GUID or binary")
        print(f"[FMU] Instantiated  comp=0x{comp:016x}")
        comp = ctypes.c_void_p(comp)        # wrap as c_void_p so 64-bit ptr is passed correctly
        self.comp = comp

        _ok(lib.fmi2SetupExperiment(comp, 0, 0.0, 0.0, 0, 0.0),
            "fmi2SetupExperiment")
        _ok(lib.fmi2EnterInitializationMode(comp),
            "fmi2EnterInitializationMode")

    def exit_init(self):
        _ok(self.lib.fmi2ExitInitializationMode(self.comp),
            "fmi2ExitInitializationMode")

    def terminate(self):
        if self.lib and self.comp:
            try:
                self.lib.fmi2Terminate(self.comp)
                self.lib.fmi2FreeInstance(self.comp)
            except Exception:
                pass
            self.comp = None

    def cleanup(self):
        self.terminate()
        if self._tmp_dir and os.path.isdir(self._tmp_dir):
            shutil.rmtree(self._tmp_dir, ignore_errors=True)
            self._tmp_dir = None


# ─────────────────────────────────────────────────────────────────────────────
#  CAN input initialiser — batch-set all CAN RX signals to default (0)
# ─────────────────────────────────────────────────────────────────────────────

def build_cantx_message_map(outputs: list) -> dict:
    """
    Group VCAN_TX.FD15_LRCF_DATA*.<SigName> FMI output variables by CAN message.

    Only FD15_LRCF_DATA* messages are considered: these are the only CAN-TX
    messages decoded from the SIL DVL payload with a companion VALID flag
    (see Decode_CanTx_From_SIL / cantx_valid in LogicModel2.cpp). Other
    VCAN_TX.* messages (e.g. FD15_FLT_EVT_LRCF) are not populated by that
    decode path and are excluded.
      - VCAN_TX.FD15_LRCF_DATA_X.VALID       (Boolean output) — set true the
        step a fresh copy of that message was decoded from the SIL output.
      - VCAN_TX.FD15_LRCF_DATA_X.<SigName>   (Real/Integer outputs) — decoded
        signal values, valid only on steps where VALID is true.

    Returns:
        { msg_name: {'valid_vr': int, 'signals': [(vr, sig_name, type_), ...]} }
        Only FD15_LRCF_DATA* messages with a VALID flag variable are included.
    """
    messages: dict = {}
    for v in outputs:
        if not v.name.startswith('VCAN_TX.FD15_LRCF_DATA_'):
            continue
        parts = v.name.split('.', 2)
        if len(parts) != 3:
            continue
        _, msg_name, sig_name = parts
        entry = messages.setdefault(msg_name, {'valid_vr': None, 'signals': []})
        if sig_name == 'VALID':
            entry['valid_vr'] = v.vr
        else:
            entry['signals'].append((v.vr, sig_name, v.type_))
    return {name: info for name, info in messages.items() if info['valid_vr'] is not None}


def init_can_inputs(lib, comp, inputs: list):
    """
    Set every CAN RX input variable to its default start value (0 / 0.0).
    ETH pointer-triplet variables are excluded; they are managed per-step.
    """
    ETH_VRS = {VR_ETH_RX_LO, VR_ETH_RX_HI, VR_ETH_RX_SIZE}

    int_vrs,  int_vals  = [], []
    real_vrs, real_vals = [], []

    for v in inputs:
        if v.vr in ETH_VRS:
            continue
        if v.type_ == 'Integer':
            int_vrs.append(v.vr);  int_vals.append(0)
        elif v.type_ in ('Real', 'Boolean'):
            real_vrs.append(v.vr); real_vals.append(0.0)

    if int_vrs:
        _ok(fmi_set_integers(lib, comp, int_vrs, int_vals),
            f"fmi2SetInteger ({len(int_vrs)} CAN RX Integer signals)")
    if real_vrs:
        _ok(fmi_set_reals(lib, comp, real_vrs, real_vals),
            f"fmi2SetReal    ({len(real_vrs)} CAN RX Real signals)")

    print(f"[FMU] CAN RX defaults applied  "
          f"({len(int_vrs)} Integer, {len(real_vrs)} Real)")


# ─────────────────────────────────────────────────────────────────────────────
#  Dummy radar UDP frame generator
# ─────────────────────────────────────────────────────────────────────────────
#
#  Minimal synthetic payload format (little-endian):
#
#    Header (12 bytes):
#      uint32  magic       = 0xDEADBEEF
#      uint32  frame_id    (incrementing counter)
#      float32 timestamp_s (seconds since start)
#
#    Per-target record (20 bytes each):
#      float32 range_m
#      float32 azimuth_deg
#      float32 elevation_deg
#      float32 velocity_mps
#      float32 rcs_dbsm
#
#  Total: 12 + N × 20 bytes

DUMMY_MAGIC         = 0xDEADBEEF
DUMMY_NUM_TARGETS   = 3

_HDR_FMT    = '<IIf'                   # magic, frame_id, timestamp
_TARGET_FMT = '<fffff'                 # range, azimuth, elevation, vel, rcs


def make_dummy_frame(frame_id: int, timestamp_s: float) -> bytes:
    """Return a synthetic radar detection UDP payload."""
    header = struct.pack(_HDR_FMT, DUMMY_MAGIC, frame_id, timestamp_s)
    targets = b''
    for i in range(DUMMY_NUM_TARGETS):
        targets += struct.pack(_TARGET_FMT,
            15.0 + i * 10.0,    # range_m
            -10.0 + i * 10.0,   # azimuth_deg
            0.0,                 # elevation_deg
            -(5.0 + i * 2.0),   # velocity_mps  (approaching)
            10.0 + i * 3.0,     # rcs_dbsm
        )
    return header + targets

def iter_udp_frames_from_mf4(mf4_path, channel="ETH_Frame", length_channel=None):
    """
    Yield one UDP frame payload (bytes) per MF4 sample.

    mf4_path      : path to an .mf4 file or a directory of .mf4 files
    channel       : channel name that holds raw Ethernet/UDP bytes
    length_channel: optional companion channel that stores the actual byte
                    count per frame.  Required when the MF4 uses a fixed-size
                    byte-array channel (otherwise trailing zero-padding is kept).
    """
    from pathlib import Path as _Path
    from asammdf import MDF as _MDF

    path = _Path(mf4_path)

    mf4_files = (
        sorted(path.glob("*.mf4")) + sorted(path.glob("*.MF4"))
        if path.is_dir()
        else [path]
    )

    for mf4_file in mf4_files:
        mdf = _MDF(str(mf4_file))

        if channel not in mdf.channels_db:
            print(f"[MF4] Channel '{channel}' not found in {mf4_file.name} — skipping")
            mdf.close()
            continue

        sig = mdf.get(channel)
        timestamps = sig.timestamps
        # Optional companion length channel (strips zero-padding on fixed-size channels)
        lengths = None
        if length_channel and length_channel in mdf.channels_db:
            lengths = mdf.get(length_channel).samples

        mdf.close()  # data is now in memory; safe to close

        for idx, raw in enumerate(sig.samples):
            payload = _sample_to_bytes(raw)
            if payload is None:
                continue

            # Strip trailing zero-padding when a length channel is available
            if lengths is not None and idx < len(lengths):
                actual_len = int(lengths[idx])
                payload = payload[:actual_len]

            if len(payload) == 0:
                continue

            udp_payload = _extract_udp_payload(payload)
            if not udp_payload:
                continue

            yield udp_payload, timestamps[idx] if idx < len(timestamps) else None


def _extract_udp_payload(frame_bytes: bytes) -> bytes:
    """
    Best-effort extraction of the LRCF UDP payload from MF4 ETH samples.

    The MF4 input may contain:
      - full Ethernet frame (ETH + IPv4 + UDP + payload), or
      - already-trimmed UDP payload bytes.

    We first try known offsets used in the native tooling (28/32/36), then
    fallback to parsed Ethernet/IPv4/UDP headers.
    """
    if not frame_bytes:
        return b''

    n = len(frame_bytes)

    # Known payload markers used by SRR streams (byte-swapped variants included).
    markers = {
        (0xA2, 0x18), (0x18, 0xA2),
        (0xA3, 0x18), (0x18, 0xA3),
    }

    # Known offsets observed in existing MF4 processing code.
    for off in (28, 32, 36, 42):
        if n >= off + 2 and (frame_bytes[off], frame_bytes[off + 1]) in markers:
            return frame_bytes[off:]

    # Fallback: parse Ethernet + IPv4 + UDP header sizes.
    if n >= ETH_HEADER + IPV4_HEADER + UDP_HEADER and frame_bytes[12:14] == b'\x08\x00':
        ip_off = ETH_HEADER
        ihl = (frame_bytes[ip_off] & 0x0F) * 4
        if ihl >= IPV4_HEADER and n >= ip_off + ihl + UDP_HEADER:
            # IPv4 protocol field must be UDP (17)
            if frame_bytes[ip_off + 9] == 17:
                return frame_bytes[ip_off + ihl + UDP_HEADER:]

    # Already looks like a payload stream (starts with known SRR markers).
    if n >= 2 and (frame_bytes[0], frame_bytes[1]) in markers:
        return frame_bytes

    # Unknown layout: pass through unchanged rather than dropping samples.
    return frame_bytes


def _sample_to_bytes(raw):
    """
    Convert one asammdf signal sample to bytes.  Returns None if not convertible.

    asammdf may return:
      - bytes / bytearray  → VLSD variable-length channel (most ETH bus logs)
      - numpy.ndarray      → fixed-size byte-array channel (iterate gives a row)
      - numpy.void         → structured/void channel
    """
    if isinstance(raw, (bytes, bytearray)):
        return bytes(raw)
    try:
        import numpy as _np
        if isinstance(raw, _np.ndarray):
            return raw.tobytes()   # row of uint8 array → flat bytes
        if isinstance(raw, _np.void):
            # Structured ETH frame record: pull only the DataBytes field.
            if getattr(raw.dtype, 'names', None):
                for fld in raw.dtype.names:
                    if fld == 'DataBytes' or fld.endswith('.DataBytes'):
                        v = raw[fld]
                        return v.tobytes() if hasattr(v, 'tobytes') else bytes(v)
            return raw.tobytes()
    except ImportError:
        pass
    if hasattr(raw, 'tobytes'):
        return raw.tobytes()
    return None

# ─────────────────────────────────────────────────────────────────────────────
#  Frame sources (real UDP socket  OR  dummy generator)
# ─────────────────────────────────────────────────────────────────────────────

class UDPReceiver(threading.Thread):
    """Listens on a UDP socket; pushes received payloads into a queue."""

    def __init__(self, host: str, port: int, frame_queue: 'queue.Queue[bytes]'):
        super().__init__(daemon=True, name='UDPReceiver')
        self.host        = host
        self.port        = port
        self.frame_queue = frame_queue
        self._stop_evt   = threading.Event()

    def stop(self):
        self._stop_evt.set()

    def run(self):
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        try:
            sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            sock.settimeout(0.5)
            sock.bind((self.host, self.port))
            print(f"[UDP] Listening on {self.host}:{self.port}")
            while not self._stop_evt.is_set():
                try:
                    data, _addr = sock.recvfrom(ETH_PAYLOAD)
                    if not self.frame_queue.full():
                        self.frame_queue.put_nowait(data)
                except socket.timeout:
                    pass
                except OSError as exc:
                    print(f"[UDP] Socket error: {exc}")
                    break
        finally:
            sock.close()


class DummyFrameSource(threading.Thread):
    """Generates synthetic frames at a configurable rate and posts to queue."""

    def __init__(self, frame_queue: 'queue.Queue[bytes]', rate_hz: float = 20.0, mf4_path: str = None):
        super().__init__(daemon=True, name='DummyFrameSource')
        self.frame_queue = frame_queue
        self.interval    = 1.0 / max(rate_hz, 0.1)
        self._stop_evt   = threading.Event()
        self.mf4_path    = mf4_path

    def stop(self):
        self._stop_evt.set()

    def run(self):
        frame_id = 0
        t_start  = time.monotonic()
        print(f"[DUMMY] Generating synthetic frames at {1/self.interval:.0f} Hz")
        while not self._stop_evt.is_set():
            ts      = time.monotonic() - t_start
            payload = make_dummy_frame(frame_id, ts)
            if not self.frame_queue.full():
                self.frame_queue.put_nowait(payload)
            frame_id += 1
            self._stop_evt.wait(timeout=self.interval)


class MF4FrameSource(threading.Thread):
    """Reads ETH frames from an MF4 file/directory and posts them to a queue."""

    def __init__(self, mf4_path: str, frame_queue: 'queue.Queue[bytes]',
                 rate_hz: float = 10000.0, channel: str = 'ETH_Frame'):
        super().__init__(daemon=True, name='MF4FrameSource')
        self.mf4_path    = mf4_path
        self.frame_queue = frame_queue
        self.interval    = 1.0 / max(rate_hz, 0.1)
        self.channel     = channel
        self._stop_evt   = threading.Event()

    def stop(self):
        self._stop_evt.set()

    def run(self):
        print(f"[MF4] Reading frames from {self.mf4_path}  "
              f"channel='{self.channel}'  rate={1/self.interval:.0f} Hz")
        frame_count = 0
        previous_ts = None
        for payload, timestamp in iter_udp_frames_from_mf4(self.mf4_path, self.channel):
            if self._stop_evt.is_set():
                break
            # Back-pressure: wait if the consumer (simulation loop) is falling behind
            while self.frame_queue.full() and not self._stop_evt.is_set():
                self._stop_evt.wait(timeout=0.005)
            if self._stop_evt.is_set():
                break
            self.frame_queue.put(payload)
            frame_count += 1
            
            # Throttle to replay timing:
            #   - If timestamps are available, sleep the inter-frame gap from the MF4.
            #   - Otherwise fall back to the fixed interval (rate_hz).
            if timestamp is not None and previous_ts is not None and payload[0]==0x18 and payload[1]==0xA3:
                inter_frame = float(timestamp - previous_ts)
                self._stop_evt.wait(timeout=max(0.0, inter_frame))
            else:
                self._stop_evt.wait(timeout=self.interval)
                

            previous_ts = timestamp
        print(f"[MF4] All {frame_count} frames enqueued — source finished")
        self.frame_queue.put(None)  # EOF sentinel: tells the sim loop to stop

# ─────────────────────────────────────────────────────────────────────────────
#  TX frame builder — wrap FMU output payload in a complete Ethernet/IP/UDP frame
# ─────────────────────────────────────────────────────────────────────────────

_TX_ETH_HDR_LEN = 14                           # Ethernet II header length
_TX_DST_MAC     = b'\xff\xff\xff\xff\xff\xff'   # broadcast (dummy)
_TX_SRC_MAC     = b'\x02\x00\x00\x00\x00\x01'  # locally-administered unicast (dummy)
_TX_ETHERTYPE   = b'\x08\x00'                  # IPv4
_TX_SRC_IP      = b'\xc0\xa8\x01\x01'          # 192.168.1.1 (dummy)
_TX_DST_IP      = b'\xc0\xa8\x01\x02'          # 192.168.1.2 (dummy)
_TX_SRC_PORT    = 7401
_TX_DST_PORT    = 7400


def _ip_checksum(header: bytes) -> int:
    """RFC 791 one's-complement Internet checksum over *header* bytes."""
    if len(header) % 2:
        header += b'\x00'
    s = sum(struct.unpack(f'>{len(header)//2}H', header))
    while s >> 16:
        s = (s & 0xFFFF) + (s >> 16)
    return ~s & 0xFFFF


def _build_eth_frame(payload: bytes, frame_id: int = 0) -> bytes:
    """
    Wrap *payload* in a complete Ethernet II / IPv4 / UDP frame.
    Dummy MAC and IP addresses are used; UDP checksum is set to 0
    (disabled, permitted by RFC 768 for IPv4).
    28 zero bytes are inserted between the UDP header and the payload.
    """
    _PRE_PAYLOAD_PAD = 28
    udp_len = UDP_HEADER + _PRE_PAYLOAD_PAD + len(payload)
    ip_len  = IPV4_HEADER + udp_len

    # UDP header — checksum disabled (0)
    udp_hdr = struct.pack('>HHHH',
        _TX_SRC_PORT,
        _TX_DST_PORT,
        udp_len,
        0,           # checksum (disabled)
    )

    # IPv4 header — checksum field filled in after calculation
    ip_hdr_raw = struct.pack('>BBHHHBBH4s4s',
        0x45,                  # Version=4, IHL=5 (20 bytes)
        0x00,                  # DSCP/ECN
        ip_len,                # total length
        frame_id & 0xFFFF,     # identification
        0x4000,                # flags=DF, fragment offset=0
        64,                    # TTL
        17,                    # protocol = UDP
        0,                     # checksum placeholder
        _TX_SRC_IP,
        _TX_DST_IP,
    )
    ip_cksum = _ip_checksum(ip_hdr_raw)
    ip_hdr   = ip_hdr_raw[:10] + struct.pack('>H', ip_cksum) + ip_hdr_raw[12:]

    # Ethernet II header
    eth_hdr = _TX_DST_MAC + _TX_SRC_MAC + _TX_ETHERTYPE

    return eth_hdr + ip_hdr + udp_hdr + b'\x00' * _PRE_PAYLOAD_PAD + payload


# ─────────────────────────────────────────────────────────────────────────────
#  Main simulation loop
# ─────────────────────────────────────────────────────────────────────────────

def run_simulation(loader: FMULoader,
                   frame_queue: 'queue.Queue[bytes]',
                   max_steps: int,
                   verbose: bool,
                   output_mf4_path: str = '',
                   dummy_canrx: bool = True,
                   cantx_mf4_path: str = '',
                   cantx_map: dict = None) -> int:
    """
    Step the FMU once per UDP frame.  Returns the total step count.

    ETH RX pointer-triplet protocol:
      - Allocate a persistent ctypes buffer (never moves → stable address).
      - For each frame: copy payload, compute lo/hi, call fmi2SetInteger,
        then fmi2DoStep.
      - After DoStep: read EthTxOut lo/hi/size to discover TX data written
        by the FMU into its own internal static buffer.

    CAN-TX decoded-signal capture (VCAN_TX.<MsgName>.* outputs):
      - After each DoStep, fmi2GetBoolean reads every message's VALID flag
        (VCAN_TX.<MsgName>.VALID) in one batched call.
      - For each message whose VALID flag is true this step, its decoded
        signals are fetched (fmi2GetReal / fmi2GetInteger) and appended,
        tagged with the current sim_time, to cantx_data[msg_name].
      - On completion, cantx_data is written to cantx_mf4_path (one Signal
        per <MsgName>.<SigName>, sampled only on the steps that were valid).
    """
    lib  = loader.lib
    comp = loader.comp

    # Persistent pinned buffer — must NOT be garbage-collected between Set and DoStep
    rx_buf = (ctypes.c_ubyte * ETH_PAYLOAD)()

    sim_time   = 0.0
    step_count = 0
    t_wall_start = time.monotonic()

    # Accumulators for TX MF4 output (relative PC timestamps in seconds)
    _mf4_timestamps: list = []
    _mf4_samples:    list = []

    # Accumulators for decoded CAN-TX MF4 output — one entry per LRCF message
    cantx_map = cantx_map or {}
    cantx_data = {
        msg_name: {
            'timestamps': [],
            'samples': {sig_name: [] for _vr, sig_name, _t in info['signals']},
        }
        for msg_name, info in cantx_map.items()
    }

    # Debug text log — one line per DoStep listing which FD15_LRCF_DATA*
    # messages had VALID=true (i.e. were freshly decoded/written) this frame.
    cantx_debug_fh = None
    if cantx_map:
        cca_debug_dir = os.path.join(
            os.path.dirname(os.path.abspath(__file__)),
            "SIL_Output", "mf4", "CCA_DEBUG"
        )
        os.makedirs(cca_debug_dir, exist_ok=True)
        cantx_debug_path = os.path.join(cca_debug_dir, "lm2cantx_debug.txt")
        cantx_debug_fh = open(cantx_debug_path, "w")
        cantx_debug_fh.write(
            "# CAN-TX decoded-signal log — one line per (step, valid message)\n"
            "# step=<step count>  t=<sim time s>  VCAN_TX.<MsgName>: sig1=val1, sig2=val2, ...\n"
            "# These are the raw values returned by fmi2GetReal/fmi2GetInteger right after\n"
            "# fmi2DoStep — compare against the C++ debugger (inst->tx / d_ptr_LRCF) to check\n"
            "# whether the FMI Get path or the MF4 write path is dropping values.\n"
        )
        print(f"[SIM] CAN-TX debug log: {cantx_debug_path}")

    limit_str = str(max_steps) if max_steps > 0 else '∞'
    print(f"\n[SIM] Entering step loop  (max_steps={limit_str})")
    print("-" * 60)

    try:
        while max_steps <= 0 or step_count < max_steps:

            # ── 1. Wait for next frame ────────────────────────────────────────
            try:
                frame_bytes = frame_queue.get(timeout=100.0)
            except queue.Empty:
                print("[SIM] No frame for 100 s — stopping")
                break

            # None is the EOF sentinel pushed by MF4FrameSource when the file
            # is exhausted.  For dummy mode the loop is governed by max_steps.
            if frame_bytes is None:
                print("[SIM] Frame source exhausted (MF4 EOF) — stopping")
                break

            payload_len = min(len(frame_bytes), ETH_PAYLOAD)

            # ── 2. Copy frame payload into the pinned RX buffer ───────────────
            ctypes.memmove(rx_buf, frame_bytes[:payload_len], payload_len)

            # ── 3. Set EthRxIn pointer-triplet (VR 5000 / 5001 / 5002) ────────
            buf_addr = ctypes.addressof(rx_buf)
            lo, hi   = ptr_to_lo_hi(buf_addr)
            _ok(
                fmi_set_integers(lib, comp,
                                 [VR_ETH_RX_LO, VR_ETH_RX_HI, VR_ETH_RX_SIZE],
                                 [lo, hi, payload_len]),
                "fmi2SetInteger (EthRxIn)"
            )

             # Optional CAN RX dummy stimulation per-step (inst->rx via FMI setters)
            if dummy_canrx:
                apply_dummy_can_rx_inputs(lib, comp, step_count, sim_time)

            # ── 4. Advance simulation by one step ─────────────────────────────
            _ok(
                lib.fmi2DoStep(comp, sim_time, STEP_SIZE_S, 1),
                "fmi2DoStep"
            )
            sim_time  += STEP_SIZE_S
            step_count += 1

            # ── 5. Read EthTxOut pointer-triplet (VR 5003 / 5004 / 5005) ──────
            st, tx_vals = fmi_get_integers(
                lib, comp, [VR_ETH_TX_LO, VR_ETH_TX_HI, VR_ETH_TX_SIZE])
            _ok(st, "fmi2GetInteger (EthTxOut)")
            tx_lo, tx_hi, tx_size = tx_vals

            tx_ptr = lo_hi_to_ptr(tx_lo, tx_hi)  # sanity-check: should reconstruct to an address

            # ── 5b. Capture TX frame bytes for MF4 output ─────────────────────
            if output_mf4_path and tx_size > 0 and tx_ptr != 0:
                _tx_ctype    = ctypes.c_ubyte * tx_size
                _tx_payload  = bytes(_tx_ctype.from_address(tx_ptr))
                _tx_eth_frame = _build_eth_frame(_tx_payload, step_count)
                _mf4_samples.append(_tx_eth_frame)
                _mf4_timestamps.append(time.monotonic() - t_wall_start)

            # ── 5c. Capture decoded CAN-TX signals for messages that were  ───
            #        (re)decoded this step — VCAN_TX.<MsgName>.VALID == true.
            if cantx_map:
                written = _capture_cantx_step(lib, comp, sim_time, cantx_map, cantx_data)
                if cantx_debug_fh is not None:
                    if written:
                        for msg_name, values_by_name in written.items():
                            sig_str = ", ".join(
                                f"{sig}={val}" for sig, val in values_by_name.items()
                            )
                            cantx_debug_fh.write(
                                f"step={step_count:>6}  t={sim_time:.4f}s  {msg_name}: {sig_str}\n"
                            )
                    else:
                        cantx_debug_fh.write(
                            f"step={step_count:>6}  t={sim_time:.4f}s  messages=[(none)]\n"
                        )

            # ── 6. Logging ────────────────────────────────────────────────────
            should_log = verbose
            if should_log:
                _log_step(step_count, sim_time, payload_len,
                          tx_lo, tx_hi, tx_size, lib, comp)

    except KeyboardInterrupt:
        print("\n[SIM] Interrupted by user (Ctrl-C)")

    finally:
        if cantx_debug_fh is not None:
            cantx_debug_fh.close()
        if output_mf4_path and _mf4_samples:
            _write_tx_mf4(output_mf4_path, _mf4_timestamps, _mf4_samples)
        if cantx_mf4_path and cantx_data:
            output_dir = os.path.join(
                os.path.dirname(os.path.abspath(__file__)),
                    "SIL_Output",
                    "mf4",
                    "CCA_DEBUG"
                )
            
            os.makedirs(output_dir, exist_ok=True)
        
            cantx_mf4_path = os.path.join(
                output_dir,
                "lm2cantx_output.mf4"
            )
            _write_cantx_mf4(cantx_mf4_path, cantx_data)

    elapsed = time.monotonic() - t_wall_start
    print(f"\n[SIM] Done — {step_count} steps  "
          f"sim_time={sim_time:.3f} s  "
          f"wall={elapsed:.2f} s")
    return step_count


def _log_step(step, sim_t, rx_len, tx_lo, tx_hi, tx_size, lib, comp):
    """Print a one-line diagnostics summary for this step."""
    tx_info = f"tx_size={tx_size:4d}B"
    if tx_size > 0:
        tx_addr = lo_hi_to_ptr(tx_lo, tx_hi)
        tx_info += f"  tx_ptr=0x{tx_addr:016x}"

    angle_info = ""
    st, vals = fmi_get_reals(lib, comp, [VR_SAMPLE_CAN_TX_REAL])
    if st == FMI2_OK:
        angle_info = f"  VHL1_LEFT_ANGLE={vals[0]:+.3f}°"

    st2, ivals = fmi_get_integers(lib, comp, [VR_SAMPLE_CAN_TX_INT])
    subtype_info = ""
    if st2 == FMI2_OK:
        subtype_info = f"  VHL1_CL_SUB_TYPE={ivals[0]}"

    print(f"  step={step:>6}  t={sim_t:.4f}s  "
          f"rx={rx_len:4d}B  {tx_info}{angle_info}{subtype_info}")


def _write_tx_mf4(path: str, timestamps: list, samples: list):
    """
    Write accumulated TX Ethernet frame data to an MF4 file.
    Timestamps are relative PC wall-clock seconds from simulation start.

    Frames are stored as:
      ETH_TX_Frame        — uint8 array channel, zero-padded to max frame size
      ETH_TX_Frame_Length — uint16 channel holding the actual byte count per frame
    """
    try:
        import numpy as np
        from asammdf import MDF, Signal
    except ImportError as exc:
        print(f"[MF4][WARN] asammdf/numpy not available — TX MF4 not written: {exc}")
        return

    print(f"[MF4] Writing {len(samples)} TX frames → {path}")
    try:
        ts_arr  = np.array(timestamps, dtype=np.float64)
        lengths = np.array([len(s) for s in samples], dtype=np.uint16)
        max_len = int(lengths.max()) if len(lengths) else 0

        # Pad all frames to max_len → typed 2D uint8 array (N × max_len)
        # asammdf handles 2D uint8 as an array channel; no dtype=object needed.
        data = np.zeros((len(samples), max_len), dtype=np.uint8)
        for i, s in enumerate(samples):
            n = len(s)
            data[i, :n] = np.frombuffer(s, dtype=np.uint8)

        sig_frame = Signal(
            samples=data,
            timestamps=ts_arr,
            name='ETH_Frame',
            unit='',
        )
        sig_len = Signal(
            samples=lengths,
            timestamps=ts_arr,
            name='ETH_Frame_Length',
            unit='bytes',
        )

        out_dir = os.path.dirname(os.path.abspath(path))
        os.makedirs(out_dir, exist_ok=True)

        mdf = MDF()
        try:
            mdf.append([sig_frame, sig_len])
            mdf.save(path, overwrite=True)
            print(f"[MF4] TX output saved  → {path}")
        finally:
            mdf.close()

    except Exception as exc:
        print(f"[MF4][WARN] Failed to write TX MF4: {exc}")


def _capture_cantx_step(lib, comp, sim_time: float, cantx_map: dict, cantx_data: dict) -> dict:
    """
    Called once per DoStep.  Reads every message's VCAN_TX.<MsgName>.VALID
    Boolean flag in a single batched fmi2GetBoolean call.  For each message
    whose flag is true this step (i.e. Decode_CanTx_From_SIL just decoded a
    fresh copy of it from the SIL dvl_payload_out), fetches its decoded
    signal values and appends them — tagged with sim_time — to cantx_data.

    Returns { "VCAN_TX.<MsgName>": {sig_name: value, ...}, ... } for every
    message that was valid (i.e. written/decoded) this step — used for the
    per-frame debug log so the raw fmi2GetReal/fmi2GetInteger values can be
    inspected directly (independent of what ends up in the MF4 file).
    """
    msg_names = list(cantx_map.keys())
    valid_vrs = [cantx_map[m]['valid_vr'] for m in msg_names]

    st, valid_vals = fmi_get_booleans(lib, comp, valid_vrs)
    _ok(st, "fmi2GetBoolean (VCAN_TX VALID flags)")

    written_msgs: dict = {}
    for msg_name, is_valid in zip(msg_names, valid_vals):
        if not is_valid:
            continue

        info = cantx_map[msg_name]
        real_sigs = [(vr, sig) for vr, sig, t in info['signals'] if t == 'Real']
        int_sigs  = [(vr, sig) for vr, sig, t in info['signals'] if t in ('Integer', 'Boolean')]

        values_by_name = {}
        if real_sigs:
            st, rv = fmi_get_reals(lib, comp, [vr for vr, _sig in real_sigs])
            _ok(st, f"fmi2GetReal (VCAN_TX.{msg_name})")
            values_by_name.update((sig, val) for (_vr, sig), val in zip(real_sigs, rv))
        if int_sigs:
            st, iv = fmi_get_integers(lib, comp, [vr for vr, _sig in int_sigs])
            _ok(st, f"fmi2GetInteger (VCAN_TX.{msg_name})")
            values_by_name.update((sig, val) for (_vr, sig), val in zip(int_sigs, iv))

        rec = cantx_data[msg_name]
        rec['timestamps'].append(sim_time)
        for sig_name, value in values_by_name.items():
            rec['samples'][sig_name].append(value)

        written_msgs[f"VCAN_TX.{msg_name}"] = values_by_name

    return written_msgs


def _write_cantx_mf4(path: str, cantx_data: dict):
    """
    Write decoded CAN-TX signals (captured only on steps where each
    message's VALID flag was true) to an MF4 file.

    Each signal is stored under its full modelDescription.xml variable name,
    e.g. "VCAN_TX.FD15_LRCF_DATA_4.HIGH_BEAM_ASSIST_FAULT", sampled at the
    sim_time values recorded for that message (messages are decoded/valid at
    different, independent steps, so each gets its own timestamp axis).
    """
    try:
        import numpy as np
        from asammdf import MDF, Signal
    except ImportError as exc:
        print(f"[MF4][WARN] asammdf/numpy not available — CAN TX MF4 not written: {exc}")
        return

    total_samples = sum(len(rec['timestamps']) for rec in cantx_data.values())
    if total_samples == 0:
        print("[MF4] No valid CAN-TX samples captured — lm2cantx_output.mf4 not written")
        return

    print(f"[MF4] Writing decoded CAN-TX signals ({total_samples} valid message samples) → {path}")
    try:
        out_dir = os.path.dirname(os.path.abspath(path))
        os.makedirs(out_dir, exist_ok=True)

        mdf = MDF()
        try:
            for msg_name, rec in cantx_data.items():
                if not rec['timestamps']:
                    continue
                ts_arr = np.array(rec['timestamps'], dtype=np.float64)
                signals = [
                    Signal(
                        samples=np.array(values, dtype=np.float64),
                        timestamps=ts_arr,
                        name=f"VCAN_TX.{msg_name}.{sig_name}",
                    )
                    for sig_name, values in rec['samples'].items()
                ]
                if signals:
                    mdf.append(signals)

            mdf.save(path, overwrite=True)
            print(f"[MF4] CAN-TX output saved → {path}")
        finally:
            mdf.close()

    except Exception as exc:
        print(f"[MF4][WARN] Failed to write CAN TX MF4: {exc}")


def apply_dummy_can_rx_inputs(lib, comp, step_count: int, sim_time: float):
    """
    Populate a small set of CAN RX inputs with deterministic dummy patterns.
    Values are written directly before each DoStep when enabled.
    """

    yaw_rate   = 1.5
    lat_accel = 1.5
    long_accel = 1.5
    long_accel_flt_sys = 1
    lat_accel_flt_sys = 1
    yaw_rate_flt_sys = 1

    _ok(fmi_set_reals(
        lib,
        comp,
        [VR_CAN_RX_LONG_ACCELERATION, VR_CAN_RX_LAT_ACCELERATION, VR_CAN_RX_YAW_RATE],
        [long_accel, lat_accel, yaw_rate],
    ), "fmi2SetReal (dummy CAN RX)")

    _ok(fmi_set_integers(
        lib,
        comp,
        [VR_CAN_RX_LONG_ACCELERATION_FAULT_STS, VR_CAN_RX_LAT_ACCELERATION_FAULT_STS, VR_CAN_RX_YAW_RATE_FAULT_STS],
        [long_accel_flt_sys, lat_accel_flt_sys, yaw_rate_flt_sys],
    ), "fmi2SetInteger (dummy CAN RX)")

# ─────────────────────────────────────────────────────────────────────────────
#  FMU initialisation sequence
# ─────────────────────────────────────────────────────────────────────────────

def initialise_fmu(fmu_path: str, verbose_basename: str = '') -> tuple:
    """
    Extract, load, and fully initialise the FMU.
    Returns (loader, inputs, outputs, params).
    """
    loader = FMULoader(fmu_path)
    xml_path = loader.extract()

    inputs, outputs, params = parse_model_description(xml_path)
    model_id, guid = read_cosim_attrs(xml_path)

    print(f"[MD]  modelIdentifier = {model_id}  GUID = {guid}")
    print(f"[MD]  Variables: {len(inputs)} inputs  "
          f"{len(outputs)} outputs  {len(params)} params")

    loader.load_library()
    loader.instantiate(model_id, guid)

    lib  = loader.lib
    comp = loader.comp

    # ── Config parameters (set during initialisation mode) ───────────────────
    #
    #   Port = 0 → FMU does NOT open any socket (master drives the data).
    #   Sensor position: front-centre of vehicle, 0.5 m height, level.

    fmi_set_string(lib, comp, VR_CFG_ETH_RX_IP, '')
    fmi_set_string(lib, comp, VR_CFG_ETH_TX_IP, '')
    _ok(fmi_set_integers(lib, comp,
                         [VR_CFG_ETH_RX_PORT, VR_CFG_ETH_TX_PORT],
                         [0, 0]),
        "fmi2SetInteger (ETH ports)")
    _ok(fmi_set_reals(lib, comp,
                      [VR_CFG_SENSOR_POS_X,    VR_CFG_SENSOR_POS_Y,
                       VR_CFG_SENSOR_POS_Z,
                       VR_CFG_SENSOR_ORI_ROLL,  VR_CFG_SENSOR_ORI_PITCH,
                       VR_CFG_SENSOR_ORI_YAW],
                      [3.5, 0.0, 0.5, 0.0, 0.0, 0.0]),
        "fmi2SetReal (sensor mounting)")
    print("[FMU] Config parameters set  "
          "(pos=[3.5, 0.0, 0.5] m, ori=[0°, 0°, 0°])")

    # ── CAN RX defaults ───────────────────────────────────────────────────────
    init_can_inputs(lib, comp, inputs)

    # ── ETH RX triplet — clear (no frame pending at init) ────────────────────
    _ok(fmi_set_integers(lib, comp,
                         [VR_ETH_RX_LO, VR_ETH_RX_HI, VR_ETH_RX_SIZE],
                         [0, 0, 0]),
        "fmi2SetInteger (EthRxIn clear)")

    # ── Verbose basename — passed to dph_sil_set_verbosebasename on 1st step ─
    fmi_set_string(lib, comp, VR_CFG_VERBOSE_BASENAME, verbose_basename)
    if verbose_basename:
        print(f"[FMU] Verbose basename : {verbose_basename}")
    else:
        print("[FMU] Verbose basename : (disabled)")

    loader.exit_init()
    print("[FMU] Initialisation complete — FMU ready to step\n")

    return loader, inputs, outputs, params



# ─────────────────────────────────────────────────────────────────────────────
#  Argument parsing & entry point
# ─────────────────────────────────────────────────────────────────────────────

def parse_args():
    p = argparse.ArgumentParser(
        description="Drive LogicModel2 FMU with UDP frames",
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
    )
    p.add_argument('--fmu',     default=None,
                   help="Path to LogicModel2.fmu")
    p.add_argument('--port',    type=int, default=7400,
                   help="UDP listen port")
    p.add_argument('--host',    default='0.0.0.0',
                   help="UDP bind address")
    p.add_argument('--dummy',   action='store_true',
                   help="Use synthetic dummy frames (no socket required)")
    p.add_argument('--rate',    type=float, default=20.0,
                   help="Dummy frame rate [Hz]")
    p.add_argument('--steps',   type=int, default=0,
                   help="Stop after N steps (0 = run forever)")
    p.add_argument('--verbose', default=False, dest='verbose',
                   help="Log every step (default: first 5, then every 100th)")
    p.add_argument('--mf4file', default=None,
                   help="Use MF4 file as frame source instead of UDP")
    p.add_argument('--verbose-basename', default=None, dest='verbose_basename',
                   help="Base path for SIL debug output files "
                        "(default: SIL_Output/<timestamp>/virtual.mf4 next to this script; "
                        "pass empty string to disable)")
    p.add_argument('--output-mf4', default=None, dest='output_mf4',
                   help="Path for the TX output MF4 file "
                        "(default: SIL_Output/<timestamp>/tx_output.mf4 next to this script)")
    p.add_argument('--cantx-mf4', default=None, dest='cantx_mf4',
                   help="Path for the decoded CAN-TX (VCAN_TX.<Msg>.<Sig>) output MF4 file "
                        "(default: SIL_Output/mf4/CCA_DEBUG/lm2cantx_output.mf4 next to this script)")
    p.add_argument('--dummy-canrx', default=False, dest='dummy_canrx',
                   help="Populate a small set of CAN RX inputs with dummy patterns "
                        "(for testing FMU response without a real CAN bus)")
    return p.parse_args()


def main():
    args = parse_args()

    # ── Resolve FMU path ─────────────────────────────────────────────────────
    script_dir  = os.path.dirname(os.path.abspath(__file__))
    default_fmu = os.path.normpath(
        os.path.join(script_dir, '..', 'Build', 'LogicModel2.fmu'))
    fmu_path = args.fmu or default_fmu

    if not os.path.isfile(fmu_path):
        print(f"[ERROR] FMU not found: {fmu_path}")
        print("        Build it first:  Scripts/build.bat  (Windows)")
        print("                         Scripts/build.sh   (Linux)")
        sys.exit(1)

    # ── Resolve verbose basename ──────────────────────────────────────────────
    #  If the user passed --verbose-basename "", verbose output is disabled.
    #  If the flag was omitted entirely, auto-create a timestamped subfolder
    #  called SIL_Output/<YYYYMMDD_HHMMSS>/ next to this script and use
    #  "virtual" as the file basename (producing virtual.dvl, virtual.dvsu …).
    if args.verbose_basename is None:
        ts = datetime.datetime.now().strftime('%Y%m%d_%H%M%S')
        out_dir = os.path.join(script_dir, 'SIL_Output', ts)
        os.makedirs(out_dir, exist_ok=True)
        verbose_basename = os.path.join(out_dir, 'virtual.mf4')
    else:
        verbose_basename = args.verbose_basename
        if verbose_basename:          # non-empty: ensure parent directory exists
            os.makedirs(os.path.dirname(os.path.abspath(verbose_basename)),
                        exist_ok=True)

    # ── Resolve TX output MF4 path ───────────────────────────────────────────
    if args.output_mf4:
        output_mf4_path = args.output_mf4
    elif verbose_basename:
        _out_dir        = os.path.dirname(os.path.abspath(verbose_basename))
        output_mf4_path = os.path.join(_out_dir, 'tx_output.mf4')
    else:
        _ts_now         = datetime.datetime.now().strftime('%Y%m%d_%H%M%S')
        _out_dir        = os.path.join(script_dir, 'SIL_Output', _ts_now)
        output_mf4_path = os.path.join(_out_dir, 'tx_output.mf4')

    # ── Resolve decoded CAN-TX output MF4 path ───────────────────────────────
    if args.cantx_mf4:
        cantx_mf4_path = args.cantx_mf4
    elif verbose_basename:
        _out_dir       = os.path.dirname(os.path.abspath(verbose_basename))
        cantx_mf4_path = os.path.join(_out_dir, 'lm2cantx_output.mf4')
    else:
        _out_dir       = os.path.dirname(os.path.abspath(output_mf4_path))
        cantx_mf4_path = os.path.join(_out_dir, 'lm2cantx_output.mf4')

    print("=" * 60)
    print("  LogicModel2  —  UDP-driven FMU runner")
    print("=" * 60)
    print(f"  FMU   : {fmu_path}")
    source_desc = (f"dummy @ {args.rate:.0f} Hz"
                   if args.dummy
                   else f"MF4 file {args.mf4file}" if args.mf4file else f"UDP {args.host}:{args.port}")
    print(f"  Source: {source_desc}")
    print(f"  Steps : {'∞' if args.steps == 0 else args.steps}")
    print(f"  Output: {verbose_basename if verbose_basename else '(verbose disabled)'}")
    print(f"  TX MF4: {output_mf4_path}")
    print(f"  CANTX MF4: {cantx_mf4_path}")
    print()

    loader = None
    source = None
    try:
        # ── 1. Load & initialise FMU ─────────────────────────────────────────
        loader, inputs, outputs, params = initialise_fmu(fmu_path,
                                                         verbose_basename)

        # ── 1b. Build VCAN_TX.<MsgName>.* → VR map for decoded CAN-TX capture ──
        cantx_map = build_cantx_message_map(outputs)
        print(f"[FMU] CAN-TX decode capture: {len(cantx_map)} messages "
              f"({', '.join(cantx_map.keys()) if cantx_map else 'none'})")

        # ── 2. Start frame source ────────────────────────────────────────────
        frame_queue: 'queue.Queue[bytes]' = queue.Queue(maxsize=32)
        if args.dummy:
            source = DummyFrameSource(frame_queue, rate_hz=args.rate)
        elif args.mf4file:
            source = MF4FrameSource(args.mf4file, frame_queue)
        else:
            source = UDPReceiver(args.host, args.port, frame_queue)
        source.start()

        # ── 3. Simulation loop — one doStep per frame ────────────────────────
        run_simulation(loader, frame_queue, args.steps, args.verbose,
                       output_mf4_path,
                       dummy_canrx=args.dummy_canrx,
                       cantx_mf4_path=cantx_mf4_path,
                       cantx_map=cantx_map)

    finally:
        if source:
            source.stop()
        if loader:
            loader.cleanup()
        print("[FMU] Resources released")


if __name__ == '__main__':
    main()
