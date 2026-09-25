"""
vcan_reader.py
==============
Read SRRL / SRRR CAN messages from MDF4 (.mf4) files and decode them using a
DBC definition file.

Public API
----------
load_dbc(dbc_path, prefixes)
    Parse a DBC file and return a cantools Database limited to the messages
    whose names start with any of *prefixes*.

read_vcan_from_mf4(mf4_path, dbc_path, prefixes)
    Open an MF4 file, extract every CAN frame whose arbitration ID matches a
    message in the filtered DBC, decode the signals, and return a dict
    mapping  message_name -> pd.DataFrame  (one row per received frame,
    timestamp in seconds as the index).

save_to_json(frames, output_path)
    Serialize the decoded frames dict to a JSON file.  Each top-level key is
    a message name; its value is an ordered list of frame records (one dict
    per frame, containing the timestamp and all decoded signal values).
"""

from __future__ import annotations

import json
import math
import struct
import warnings
from pathlib import Path
from typing import Dict, List

import cantools
import cantools.database
import numpy as np
import pandas as pd
from asammdf import MDF

# ---------------------------------------------------------------------------
# Constants
# ---------------------------------------------------------------------------
_DEFAULT_PREFIXES: tuple[str, ...] = ("SRRL_", "SRRR_")

# Channels we need from every CAN_DataFrame channel group in the MF4.
_REQUIRED_CHANNELS = {
    "CAN_DataFrame.ID",
    "CAN_DataFrame.DataBytes",
    "CAN_DataFrame.DataLength",
    "CAN_DataFrame.IDE",
}

# ---------------------------------------------------------------------------
# CANoe VLSD format constants and helpers
# ---------------------------------------------------------------------------
# Each record in a CANoe VLSD_CHANNEL_GROUP is exactly 23 bytes:
#   bytes  0-7  : uint64  timestamp_ns (relative to file start)
#   byte   8    : uint8   Flags
#   byte   9    : uint8   DLC
#   byte   10   : uint8   DataLength (actual payload bytes, 0-64)
#   bytes 11-14 : uint32  CAN-ID (LE, bit 31 set for extended frames)
#   bytes 15-22 : uint64  VLSD byte-offset (into the SD payload area)
_CANOE_RECORD_SIZE  = 23
_CANOE_TS_OFFSET    = 0
_CANOE_DLEN_OFFSET  = 10
_CANOE_ID_OFFSET    = 11
_CANOE_VLSD_OFFSET  = 15
_CANOE_TS_NS_PER_S  = 1_000_000_000

# Generic MDF4 block header: id(4), reserved(4), length(8), link_count(8)
_MDF4_BLOCK_HDR = struct.Struct("<4sI QQ")


def _canoe_iter_dl_addresses(fp, dl_address: int) -> List[int]:
    """Walk a ##DL (Data List) chain and return all linked block addresses."""
    addresses: List[int] = []
    current = dl_address
    while current:
        fp.seek(current)
        raw = fp.read(_MDF4_BLOCK_HDR.size)
        if len(raw) < _MDF4_BLOCK_HDR.size:
            break
        block_id, _, _, link_count = _MDF4_BLOCK_HDR.unpack(raw)
        if block_id != b"##DL":
            break
        links_raw = fp.read(link_count * 8)
        links = struct.unpack(f"<{link_count}Q", links_raw)
        next_dl    = links[0]
        data_addrs = links[1:]
        # flags(1) + reserved(3) + count(4)
        meta = fp.read(8)
        count = struct.unpack("<I", meta[4:8])[0]
        for addr in data_addrs[:count]:
            if addr:
                addresses.append(addr)
        current = next_dl
    return addresses


def _canoe_read_sd_block(fp, sd_address: int) -> bytes:
    """Read the raw data payload from a ##SD (Signal Data) block."""
    fp.seek(sd_address)
    raw = fp.read(_MDF4_BLOCK_HDR.size)
    block_id, _, length, link_count = _MDF4_BLOCK_HDR.unpack(raw)
    if block_id != b"##SD":
        raise ValueError(f"Expected ##SD at 0x{sd_address:X}, got {block_id!r}")
    fp.seek(sd_address + _MDF4_BLOCK_HDR.size + link_count * 8)
    data_size = length - _MDF4_BLOCK_HDR.size - link_count * 8
    return fp.read(data_size)


# ---------------------------------------------------------------------------
# Public functions
# ---------------------------------------------------------------------------

def load_dbc(
    dbc_path: str | Path,
    prefixes: tuple[str, ...] = _DEFAULT_PREFIXES,
) -> cantools.database.Database:
    """
    Load *dbc_path* and return a :class:`cantools.database.Database` that
    contains **only** the messages whose names start with one of *prefixes*.

    Parameters
    ----------
    dbc_path:
        Path to the ``.dbc`` file.
    prefixes:
        Tuple of name prefixes used to filter messages.
        Defaults to ``("SRRL_", "SRRR_")``.

    Returns
    -------
    cantools.database.Database
        Filtered database.  Use it like any cantools database.
    """
    dbc_path = Path(dbc_path)
    full_db = cantools.database.load_file(str(dbc_path))
    filtered = [m for m in full_db.messages if m.name.startswith(prefixes)]
    if not filtered:
        warnings.warn(
            f"No messages starting with {prefixes} found in {dbc_path.name}.",
            stacklevel=2,
        )
    return cantools.database.Database(messages=filtered)


# ---------------------------------------------------------------------------
# File-type detection
# ---------------------------------------------------------------------------

def detect_mf4_type(mf4_path: str | Path) -> str:
    """
    Detect which logger produced the MF4 file.

    Returns
    -------
    ``"man_autera"``
        Autera logger – CAN data stored in ``CAN_DataFrame`` channel groups.
    ``"aptiv_orcas"``
        APTIV Orcas logger – single ``MF4Frame`` group, CAN frames have
        ``MF4Frame.ProtocolType == 4``, all VCAN on bus channel 3.
    ``"scania_orcas"``
        SCANIA Orcas logger – same ``MF4Frame`` structure as APTIV, but
        SRRR on bus channel 3 and SRRL on bus channel 4.
    ``"resim_canoe"``
        CANoe resim output – ``CAN_DataFrame`` group has ``samples_byte_nr == 23``
        (8 B ts + 1 B flags + 1 B DLC + 1 B DataLength + 4 B CAN-ID + 8 B VLSD offset).
    ``"unknown"``
        Could not identify.
    """
    mdf = MDF(str(mf4_path))
    has_mf4frame = False
    for grp in mdf.groups:
        names = {ch.name for ch in grp.channels}
        if "CAN_DataFrame" in names:
            # CANoe resim files use a fixed 23-byte record layout:
            #   8 B timestamp | 1 B flags | 1 B DLC | 1 B DataLength
            #   | 4 B CAN-ID | 8 B VLSD offset
            # MAN-Autera files use 22-byte records (no VLSD offset field).
            # Both have CG flags 0x06 and DataBytes channel_type == 1,
            # so samples_byte_nr is the only reliable discriminator.
            is_canoe = grp.channel_group.samples_byte_nr == 23
            mdf.close()
            return "resim_canoe" if is_canoe else "man_autera"
        if "MF4Frame.ProtocolType" in names:
            has_mf4frame = True
    if has_mf4frame:
        # Distinguish APTIV (bus 3 only) from SCANIA (buses 3 + 4).
        try:
            proto = mdf.get("MF4Frame.ProtocolType").samples
            bus   = mdf.get("MF4Frame.BusChannel").samples
            can_buses = set(bus[proto == 4].tolist())
            mdf.close()
            if 4 in can_buses:
                return "scania_orcas"
            return "aptiv_orcas"
        except Exception:
            mdf.close()
            return "aptiv_orcas"
    mdf.close()
    return "unknown"


# ---------------------------------------------------------------------------
# Public dispatcher
# ---------------------------------------------------------------------------

def read_vcan_from_mf4(
    mf4_path: str | Path,
    dbc_path: str | Path,
    prefixes: tuple[str, ...] = _DEFAULT_PREFIXES,
    fmt: str | None = None,
    progress_cb=None,
    base_pct: int = 5,
    span_pct: int = 80,
) -> Dict[str, pd.DataFrame]:
    """
    Decode SRRL / SRRR (and optionally SRR2_) CAN frames from an MDF4 file.

    Automatically detects the source format (MAN-Autera, APTIV/SCANIA-Orcas, or
    RESIM-Canoe) and dispatches to the appropriate private decoder.

    Parameters
    ----------
    mf4_path:
        Path to the ``.mf4`` recording.
    dbc_path:
        Path to the ``.dbc`` file.
    prefixes:
        Name prefixes used to select messages from the DBC.
    fmt:
        Pre-detected format string (output of :func:`detect_mf4_type`).  When
        provided the format detection step is skipped, saving one MDF open.

    Returns
    -------
    dict[str, pd.DataFrame]
        One entry per message name that had at least one received frame.
        Each DataFrame has:
        * Index  ``timestamp_s``  – reception time in seconds (float64).
        * One column per decoded signal (physical / engineering values).
    """
    mf4_path = Path(mf4_path)
    dbc_path = Path(dbc_path)
    if fmt is None:
        fmt = detect_mf4_type(mf4_path)
    if fmt in ("aptiv_orcas", "scania_orcas"):
        return _read_vcan_aptiv_orcas(mf4_path, dbc_path, prefixes,
                                      progress_cb=progress_cb,
                                      base_pct=base_pct, span_pct=span_pct)
    if fmt == "resim_canoe":
        return _read_vcan_resim_canoe(mf4_path, dbc_path, prefixes,
                                      progress_cb=progress_cb,
                                      base_pct=base_pct, span_pct=span_pct)
    return _read_vcan_man_autera(mf4_path, dbc_path, prefixes,
                                 progress_cb=progress_cb,
                                 base_pct=base_pct, span_pct=span_pct)


def _read_vcan_man_autera(
    mf4_path: Path,
    dbc_path: Path,
    prefixes: tuple[str, ...],
    progress_cb=None,
    base_pct: int = 5,
    span_pct: int = 80,
) -> Dict[str, pd.DataFrame]:
    """Decode CAN from MAN-Autera (CAN_DataFrame groups) MF4 files."""

    db = load_dbc(dbc_path, prefixes)

    # Build fast lookup: MF4-stored ID -> cantools Message object.
    # The Aptiv MF4 logger stores CAN IDs WITH bit 31 set for extended frames
    # (i.e. the raw DBC integer, e.g. 0x8EFF3B8D).
    # cantools strips bit 31 internally, so we restore it here.
    id_to_msg: Dict[int, cantools.database.can.Message] = {
        (msg.frame_id | 0x80000000) if msg.is_extended_frame else msg.frame_id: msg
        for msg in db.messages
    }

    # Accumulate decoded rows per message name before building DataFrames.
    rows: Dict[str, List[dict]] = {msg.name: [] for msg in db.messages}

    mdf = MDF(str(mf4_path))
    total_groups = max(len(mdf.groups), 1)

    for group_idx, grp in enumerate(mdf.groups):
        if progress_cb is not None:
            pct = base_pct + int(span_pct * group_idx / total_groups)
            progress_cb(pct, "Reading CAN frames…")

        ch_names = {ch.name for ch in grp.channels}

        # Skip groups that don't carry standard CAN data frames.
        if not _REQUIRED_CHANNELS.issubset(ch_names):
            continue

        # Read the four channels we need from this group in one pass.
        sig_id = mdf.get("CAN_DataFrame.ID", group=group_idx, raw=True)
        sig_dl = mdf.get("CAN_DataFrame.DataLength", group=group_idx, raw=True)
        sig_db = mdf.get("CAN_DataFrame.DataBytes", group=group_idx, raw=True)
        sig_ide = mdf.get("CAN_DataFrame.IDE", group=group_idx, raw=True)

        timestamps: np.ndarray = sig_id.timestamps   # shape (n,)
        ids: np.ndarray = sig_id.samples              # shape (n,)    uint32
        dlengths: np.ndarray = sig_dl.samples         # shape (n,)    uint8/16
        databytes: np.ndarray = sig_db.samples        # shape (n, max_len) uint8
        ide: np.ndarray = sig_ide.samples             # shape (n,)    uint8

        # Pre-filter: keep only extended frames whose IDs we care about.
        ext_mask = ide.astype(bool)
        ids_ext = ids[ext_mask]
        target_mask = np.isin(ids_ext, np.array(list(id_to_msg.keys()), dtype=ids.dtype))

        # Indices into the original arrays that are both extended AND matched.
        matched_indices = np.where(ext_mask)[0][target_mask]

        for idx in matched_indices:
            arb_id_int = int(ids[idx])
            msg = id_to_msg[arb_id_int]
            actual_len = int(dlengths[idx])

            # Convert numpy row to bytes, sliced to the actual data length.
            raw_row: np.ndarray = databytes[idx]
            data: bytes = raw_row.tobytes()[:actual_len]

            try:
                decoded: dict = msg.decode(data, decode_choices=False)
            except Exception:
                # Skip corrupted or truncated frames silently.
                continue

            decoded["timestamp"] = float(timestamps[idx])
            rows[msg.name].append(decoded)

    mdf.close()

    # Build one DataFrame per message.
    result: Dict[str, pd.DataFrame] = {}
    for msg in db.messages:
        frame_list = rows[msg.name]
        if not frame_list:
            continue
        df = pd.DataFrame(frame_list)
        df = df.set_index("timestamp")
        df.index.name = "timestamp_s"
        result[msg.name] = df

    return result


# ---------------------------------------------------------------------------
# APTIV-Orcas decoder
# ---------------------------------------------------------------------------

def _read_vcan_aptiv_orcas(
    mf4_path: Path,
    dbc_path: Path,
    prefixes: tuple[str, ...],
    progress_cb=None,
    base_pct: int = 5,
    span_pct: int = 80,
) -> Dict[str, pd.DataFrame]:
    """
    Decode CAN from APTIV-Orcas MF4 files.

    The Orcas logger stores all bus traffic in a single ``MF4Frame`` channel
    group.  CAN frames are identified by ``MF4Frame.ProtocolType == 4``.
    CAN IDs are stored with bit-31 set (same convention as MAN-Autera).

    The ``MF4Frame.DataBytes`` channel is a VLSD channel backed by a ##HL
    (Header List) block.  asammdf's ``get()`` does not handle that combination,
    so we read the VLSD blob via the internal ``_load_signal_data`` API and
    reconstruct per-frame byte offsets from the ``MF4Frame.DataLength`` channel.
    """
    db = load_dbc(dbc_path, prefixes)

    # Build lookup: (frame_id | 0x80000000 for extended) → message
    id_to_msg: Dict[int, cantools.database.can.Message] = {
        int(np.uint32((msg.frame_id | 0x80000000) if msg.is_extended_frame
                      else msg.frame_id)): msg
        for msg in db.messages
    }

    mdf = MDF(str(mf4_path))
    inner = mdf._mdf   # type: ignore[attr-defined]

    # Find the group that contains MF4Frame channels.
    frame_grp = None
    for grp in inner.groups:
        if any(ch.name == "MF4Frame.ProtocolType" for ch in grp.channels):
            frame_grp = grp
            break
    if frame_grp is None:
        mdf.close()
        return {}

    # Find the signal_data index for the DataBytes VLSD channel.
    # In asammdf's internal layout the signal_data list has one extra entry
    # (for the master / timestamp channel), so channel[i] → signal_data[i+1].
    vlsd_sig_idx: int | None = None
    for ch_idx, ch in enumerate(frame_grp.channels):
        if ch.name == "MF4Frame.DataBytes":
            # Try ch_idx+1 first (the common case), then ch_idx as fallback.
            for candidate in (ch_idx + 1, ch_idx):
                if (candidate < len(frame_grp.signal_data)
                        and frame_grp.signal_data[candidate] is not None):
                    vlsd_sig_idx = candidate
                    break
            break

    if vlsd_sig_idx is None:
        mdf.close()
        return {}

    # Load the full VLSD blob (compressed blocks are decompressed automatically).
    vlsd_blob: bytes = inner._load_signal_data(group=frame_grp, index=vlsd_sig_idx)  # type: ignore[arg-type]

    # Read scalar channels – safe because they are not VLSD.
    proto_sig   = mdf.get("MF4Frame.ProtocolType")
    ids_sig     = mdf.get("MF4Frame.ID")
    dlength_sig = mdf.get("MF4Frame.DataLength")

    proto:   np.ndarray = proto_sig.samples
    ids_raw: np.ndarray = ids_sig.samples.astype(np.int64)
    dlength: np.ndarray = dlength_sig.samples.astype(np.int64)
    ts_all:  np.ndarray = ids_sig.timestamps

    # Reconstruct per-frame byte offset into the VLSD blob.
    # Each entry is stored as: [4-byte LE length][payload bytes].
    padded  = 4 + dlength
    offsets = np.concatenate([[0], np.cumsum(padded[:-1])])

    # CAN frame mask.
    can_mask = proto == 4

    # Decode all matched messages.
    rows: Dict[str, List[dict]] = {msg.name: [] for msg in db.messages}
    total_msgs = max(len(db.messages), 1)

    for msg_i, msg in enumerate(db.messages):
        if progress_cb is not None:
            pct = base_pct + int(span_pct * msg_i / total_msgs)
            progress_cb(pct, f"Decoding {msg.name}…")

        key = int(np.uint32((msg.frame_id | 0x80000000) if msg.is_extended_frame
                            else msg.frame_id))
        mask = can_mask & (ids_raw == key)
        indices = np.where(mask)[0]
        if len(indices) == 0:
            continue

        for idx in indices:
            off  = int(offsets[idx])
            elen = int.from_bytes(vlsd_blob[off: off + 4], "little")
            data = bytes(vlsd_blob[off + 4: off + 4 + min(elen, msg.length)])
            if len(data) < msg.length:
                continue
            try:
                decoded: dict = msg.decode(data, decode_choices=False)
            except Exception:
                continue
            decoded["timestamp"] = float(ts_all[idx])
            rows[msg.name].append(decoded)

    mdf.close()

    result: Dict[str, pd.DataFrame] = {}
    for msg in db.messages:
        frame_list = rows[msg.name]
        if not frame_list:
            continue
        df = pd.DataFrame(frame_list)
        df = df.set_index("timestamp")
        df.index.name = "timestamp_s"
        result[msg.name] = df

    return result


# ---------------------------------------------------------------------------
# CANoe resim decoder
# ---------------------------------------------------------------------------

def _read_vcan_resim_canoe(
    mf4_path: Path,
    dbc_path: Path,
    prefixes: tuple[str, ...],
    progress_cb=None,
    base_pct: int = 5,
    span_pct: int = 80,
) -> Dict[str, pd.DataFrame]:
    """
    Decode CAN from CANoe resim MF4 files (VLSD_CHANNEL_GROUP format).

    The CANoe resim logger stores all CAN bus traffic in a single
    VLSD_CHANNEL_GROUP (channel-group flags = 0x06).  Each record is a fixed
    23-byte entry whose layout is defined by ``_CANOE_RECORD_SIZE`` and the
    associated offset constants.  Variable-length CAN payload bytes are stored
    externally in ##SD blocks chained via a ##DL block, referenced by
    ``CAN_DataFrame.DataBytes.data_block_addr``.
    """
    db = load_dbc(dbc_path, prefixes)

    # Build fast lookup: raw MF4 CAN-ID (bit 31 set for extended) → Message.
    id_to_msg: Dict[int, cantools.database.can.Message] = {
        int(np.uint32((msg.frame_id | 0x80000000) if msg.is_extended_frame
                      else msg.frame_id)): msg
        for msg in db.messages
    }

    mdf = MDF(str(mf4_path))
    inner = mdf._mdf  # type: ignore[attr-defined]
    grp = inner.groups[0]

    # --- Load raw 23-byte records ------------------------------------------
    if progress_cb is not None:
        progress_cb(base_pct, "Loading raw CAN records…")

    chunks: List[bytes] = []
    for fragment in inner._load_data(grp):
        data = fragment.data if hasattr(fragment, "data") else fragment[0]
        chunks.append(bytes(data))
    records = b"".join(chunks)

    # --- Load VLSD payload bytes -------------------------------------------
    db_ch = next(
        (ch for ch in grp.channels if ch.name == "CAN_DataFrame.DataBytes"),
        None,
    )
    vlsd_bytes = b""
    if db_ch is not None and db_ch.data_block_addr:
        with open(mf4_path, "rb") as fp:
            fp.seek(db_ch.data_block_addr)
            hdr = fp.read(4)
            if hdr == b"##DL":
                addrs = _canoe_iter_dl_addresses(fp, db_ch.data_block_addr)
                vlsd_bytes = b"".join(_canoe_read_sd_block(fp, a) for a in addrs)
            elif hdr == b"##SD":
                vlsd_bytes = _canoe_read_sd_block(fp, db_ch.data_block_addr)

    mdf.close()

    # --- Iterate records and decode ----------------------------------------
    n = len(records) // _CANOE_RECORD_SIZE
    rows: Dict[str, List[dict]] = {msg.name: [] for msg in db.messages}
    vlsd_len = len(vlsd_bytes)

    for i in range(n):
        if progress_cb is not None and i % 5000 == 0:
            pct = base_pct + int(span_pct * i / max(n, 1))
            progress_cb(pct, "Decoding CAN frames…")

        off = i * _CANOE_RECORD_SIZE
        can_id = struct.unpack_from("<I", records, off + _CANOE_ID_OFFSET)[0]
        msg = id_to_msg.get(can_id)
        if msg is None:
            continue

        ts_ns   = struct.unpack_from("<Q", records, off + _CANOE_TS_OFFSET)[0]
        vlsd_off = int(struct.unpack_from("<Q", records, off + _CANOE_VLSD_OFFSET)[0])

        if vlsd_off + 4 > vlsd_len:
            continue
        pay_len = struct.unpack_from("<I", vlsd_bytes, vlsd_off)[0]
        data = bytes(vlsd_bytes[vlsd_off + 4: vlsd_off + 4 + min(pay_len, msg.length)])
        if len(data) < msg.length:
            continue

        try:
            decoded: dict = msg.decode(data, decode_choices=False)
        except Exception:
            continue

        decoded["timestamp"] = float(ts_ns) / _CANOE_TS_NS_PER_S
        rows[msg.name].append(decoded)

    result: Dict[str, pd.DataFrame] = {}
    for msg in db.messages:
        frame_list = rows[msg.name]
        if not frame_list:
            continue
        df = pd.DataFrame(frame_list)
        df = df.set_index("timestamp")
        df.index.name = "timestamp_s"
        result[msg.name] = df

    return result


def save_to_json(
    frames: Dict[str, pd.DataFrame],
    output_path: str | Path,
) -> None:
    """
    Write decoded CAN frames to a JSON file.

    Structure of the output file::

        {
          "SRRL_Header_SRR2": [
            {"timestamp_s": 0.001234, "Scan_Index_L": 0, ...},
            {"timestamp_s": 0.011234, "Scan_Index_L": 1, ...},
            ...
          ],
          "SRRR_Header_SRR2": [...],
          ...
        }

    Parameters
    ----------
    frames:
        Dict returned by :func:`read_vcan_from_mf4`.
    output_path:
        Destination path for the JSON file.  Parent directories are created
        automatically.
    """
    output_path = Path(output_path)
    output_path.parent.mkdir(parents=True, exist_ok=True)

    out: Dict[str, list] = {}
    total_frames = 0

    for msg_name, df in frames.items():
        records = df.reset_index().to_dict(orient="records")
        clean: List[dict] = []
        for rec in records:
            clean_rec: dict = {}
            for k, v in rec.items():
                # Replace non-JSON-serialisable floats with None.
                if isinstance(v, float) and (math.isnan(v) or math.isinf(v)):
                    clean_rec[k] = None
                elif isinstance(v, (np.integer,)):
                    clean_rec[k] = int(v)
                elif isinstance(v, (np.floating,)):
                    clean_rec[k] = float(v)
                else:
                    clean_rec[k] = v
            clean.append(clean_rec)
        out[msg_name] = clean
        total_frames += len(clean)

    with output_path.open("w", encoding="utf-8") as fh:
        json.dump(out, fh, indent=2)

    print(
        f"[save_to_json] Saved {total_frames} frames across "
        f"{len(out)} message types -> {output_path}"
    )
