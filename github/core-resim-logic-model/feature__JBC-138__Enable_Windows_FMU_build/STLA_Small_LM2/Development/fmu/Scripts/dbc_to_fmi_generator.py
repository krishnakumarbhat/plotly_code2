#!/usr/bin/env python3
"""
DBC to FMI Generator  (v5.3 - CAN + Ethernet lo/hi pointer-triplet)
====================================================================
v5.3:
  - ETH variable names flattened to consistent 2-level depth:
      EthRxIn.lo / EthRxIn.hi / EthRxIn.size   (was EthRxIn.base.lo/hi + EthRxIn.size)
      EthTxOut.lo / EthTxOut.hi / EthTxOut.size (was EthTxOut.base.lo/hi + EthTxOut.size)
    Mixed depth (2-level .size with 3-level .base.lo) caused ConfigurationDesk to split
    each ETH direction into two separate ModelPorts. All 6 ETH vars are now 2-level,
    rendering as one EthRxIn and one EthTxOut ModelPort each.
    VR numbers unchanged.
v5.2:
  - CAN variables grouped under VCAN_RX.<MsgName>.<SigName> and VCAN_TX.<MsgName>.<SigName>
    Collapses 26 flat message nodes into 2 top-level groups in ConfigurationDesk,
    mirroring the EthRxIn / EthTxOut pattern already used for Ethernet.
    VR numbers are unchanged — only display names change.
v5.1 fixes:
  - DIAGNOSTIC_ROE messages excluded (64-bit N_PDU overflows fmi2Integer)
  - VIN_INFORMATION excluded (136-bit ASCII — no FMI scalar type fits; not used by perception)
  - signal_is_real() returns True for signals >32 bits (e.g. 48-bit MAC) — avoids int32 truncation
  - CAN variable naming: <MsgName>.<SigName> (2-level structured, matches DBC hierarchy directly)
    Previously CAN.<MsgName>.<SigName>; the CAN. prefix was ambiguous and did not match any
    real SCALEXIO channel name, preventing ConfigurationDesk auto-wiring.

Generates from a CAN DBC file:
  1. FMI 2.0 modelDescription.xml        -> modelDescription.xml
  2. C++ data structures                  -> generated_structs.h
  3. C++ signal mapping (get/set)         -> generated_signal_map.cpp

CAN signals:
  Sent BY the FMU node  -> FMI causality = output
  Sent TO  the FMU node -> FMI causality = input
  Diagnostic / NM / DEV messages excluded by default.
  Variable naming: CAN.<MsgName>.<SignalName>  (variableNamingConvention=structured)

Ethernet — lo/hi 64-bit pointer-triplet (same pattern as Lime_v3/SM2 FMU):
  ONE frame per fmi2DoStep.  No batching, no internal socket.
  RX (input  from master):  EthRxIn.lo, EthRxIn.hi, EthRxIn.size
  TX (output to   master):  EthTxOut.lo, EthTxOut.hi, EthTxOut.size

  How it works:
    Master (dSPACE SIL UDP Receive block) allocates a buffer, fills it with the
    received datagram, then sets lo/hi to the 64-bit buffer address and size to
    the byte count before each fmi2DoStep.  The FMU reconstructs the pointer and
    reads directly from the master's buffer (zero-copy on RX side).
    For TX the FMU fills a static internal buffer, sets lo/hi/size so the master
    can locate and DMA/copy it after fmi2DoStep returns.

  Total ETH FMI vars: 6  (3 RX Integer inputs + 3 TX Integer outputs)
  This reduces variable count from ~30,631 to ~555, eliminating the AXIOM 3-4 h
  load-time problem while keeping Ethernet wiring visible in ConfigurationDesk.

Usage:
    python3 dbc_to_fmi_generator.py <dbc_file> [fmu_node] [--include-diag]

Examples:
    python3 dbc_to_fmi_generator.py JOB3_LRCF_FD_CAN15.dbc
    python3 dbc_to_fmi_generator.py JOB3_LRCF_FD_CAN15.dbc LRCF --include-diag
"""

import sys, os, cantools, uuid, datetime
from xml.etree import ElementTree as ET
from xml.dom import minidom

# ─── Ethernet constants ───────────────────────────────────────────────────────
ETH_PAYLOAD    = 1500  # max bytes per frame (standard Ethernet MTU)
ETH_RX_VR_BASE = 5000  # lo=5000, hi=5001, size=5002  (Integer inputs)
ETH_TX_VR_BASE = 5003  # lo=5003, hi=5004, size=5005  (Integer outputs)

# ─── CAN exclusion patterns ───────────────────────────────────────────────────
EXCLUDE_PATTERNS = [
    'DIAG_', 'NM_', 'SUPERVISION', 'PDC_INFO', '_DEV_',
    'CFG_DATA_CODE', 'FOTA', 'VERS_', 'ACK_EVT',
    'DIAGNOSTIC_ROE',          # ROE msgs carry 64-bit N_PDU — overflows fmi2Integer (int32)
]

# Individual signal names excluded regardless of their message.
# These signals cannot be correctly represented by any FMI 2.0 scalar type,
# or are not consumed by the perception algorithm.
EXCLUDE_SIGNAL_NAMES = frozenset([
    'VIN_INFORMATION',  # 136-bit ASCII VIN — no FMI scalar type fits; not used by LRCF algo
])


def should_exclude(msg_name, include_diag):
    if include_diag:
        return False
    return any(p in msg_name.upper() for p in EXCLUDE_PATTERNS)


def signal_is_real(sig):
    """True when the signal needs an FMI Real (physical / floating-point value)."""
    if sig.is_float:
        return True
    scale  = float(sig.scale)  if sig.scale  is not None else 1.0
    offset = float(sig.offset) if sig.offset is not None else 0.0
    if scale != 1.0 or offset != 0.0:
        return True
    if sig.unit and sig.unit not in ('-', ''):
        return True
    if sig.length > 32:
        return True  # fmi2Integer = int32_t (32-bit); wider raw values use Real (double has 52-bit mantissa, lossless up to 48 bits)
    return False


def prettify_xml(elem):
    return minidom.parseString(ET.tostring(elem, 'utf-8')).toprettyxml(indent="  ")


# ─── Build CAN variable list ──────────────────────────────────────────────────
def clean_comment(comment):
    """Extract the first meaningful sentence from a DBC signal comment."""
    if not comment:
        return ''
    for line in comment.strip().split('\n'):
        line = line.strip().lstrip('/ ').strip()
        if (len(line) > 8
                and not line.startswith('Description')
                and not line.startswith('DATA_ID')
                and not line.startswith('Commentaires')):
            return line[:120]
    return ''


def build_can_variables(dbc_file, fmu_node, include_diag):
    db = cantools.database.load_file(dbc_file)

    rx_msgs, tx_msgs = [], []
    for msg in sorted(db.messages, key=lambda m: m.frame_id):
        if should_exclude(msg.name, include_diag):
            continue
        sender = msg.senders[0] if msg.senders else ''
        if sender == fmu_node:
            tx_msgs.append(msg)
        else:
            all_receivers = []
            for s in msg.signals:
                all_receivers.extend(s.receivers or [])
            if fmu_node in all_receivers:
                rx_msgs.append(msg)

    variables = []
    vr = 1000

    print(f"\n  FMU INPUT messages  ({len(rx_msgs)} messages):")
    for msg in rx_msgs:
        print(f"    {msg.name:<45} ID=0x{msg.frame_id:08X}  {len(msg.signals):3d} signals")
        for sig in msg.signals:
            if sig.name in EXCLUDE_SIGNAL_NAMES:
                continue
            is_real = signal_is_real(sig)
            unit    = (sig.unit or '').strip()
            unit    = '' if unit == '-' else unit
            note    = clean_comment(sig.comment)
            desc    = f"[CAN-RX] {msg.name} :: {sig.name}"
            if note:
                desc += f" — {note}"
            variables.append({
                'name':           f"VCAN_RX.{msg.name}.{sig.name}",
                'valueReference': vr,
                'causality':      'input',
                'variability':    'continuous' if is_real else 'discrete',
                'description':    desc,
                'type':           'Real' if is_real else 'Integer',
                'start':          0.0 if is_real else 0,
                'unit':           unit,
                'min':            sig.minimum,
                'max':            sig.maximum,
                '_msg':           msg.name,
                '_sig':           sig.name,
            })
            vr += 1

    print(f"\n  FMU OUTPUT messages ({len(tx_msgs)} messages):")
    for msg in tx_msgs:
        print(f"    {msg.name:<45} ID=0x{msg.frame_id:08X}  {len(msg.signals):3d} signals")
        for sig in msg.signals:
            if sig.name in EXCLUDE_SIGNAL_NAMES:
                continue
            is_real = signal_is_real(sig)
            unit    = (sig.unit or '').strip()
            unit    = '' if unit == '-' else unit
            note    = clean_comment(sig.comment)
            desc    = f"[CAN-TX] {msg.name} :: {sig.name}"
            if note:
                desc += f" — {note}"
            variables.append({
                'name':           f"VCAN_TX.{msg.name}.{sig.name}",
                'valueReference': vr,
                'causality':      'output',
                'variability':    'continuous' if is_real else 'discrete',
                'description':    desc,
                'type':           'Real' if is_real else 'Integer',
                'start':          0.0 if is_real else 0,
                'unit':           unit,
                'min':            sig.minimum,
                'max':            sig.maximum,
                '_msg':           msg.name,
                '_sig':           sig.name,
            })
            vr += 1

    return variables, rx_msgs, tx_msgs


# ─── Build Ethernet variable list ────────────────────────────────────────────
def build_ethernet_variables():
    """
    Generates 6 FMI Integer variables for the Ethernet lo/hi pointer-triplet
    interface (same pattern as Lime_v3/SM2 FMU).
    One frame per fmi2DoStep:
      RX: EthRxIn.lo  (VR 5000) + EthRxIn.hi  (VR 5001) + EthRxIn.size  (VR 5002)
      TX: EthTxOut.lo (VR 5003) + EthTxOut.hi (VR 5004) + EthTxOut.size (VR 5005)
    All 6 are 2-level structured names -> ConfigurationDesk renders ONE EthRxIn and
    ONE EthTxOut ModelPort, each with 3 scalar signals.
    """
    eth_vars = [
        # ── RX: master allocates buffer, fills it, passes pointer to FMU ───────────
        {
            'name': 'EthRxIn.lo',
            'valueReference': ETH_RX_VR_BASE + 0,
            'causality': 'input', 'variability': 'discrete',
            'description': '[ETH-RX] Lower 32 bits of 64-bit pointer to received frame buffer',
            'type': 'Integer', 'start': 0, '_eth': 'rx_lo',
        },
        {
            'name': 'EthRxIn.hi',
            'valueReference': ETH_RX_VR_BASE + 1,
            'causality': 'input', 'variability': 'discrete',
            'description': '[ETH-RX] Upper 32 bits of 64-bit pointer to received frame buffer',
            'type': 'Integer', 'start': 0, '_eth': 'rx_hi',
        },
        {
            'name': 'EthRxIn.size',
            'valueReference': ETH_RX_VR_BASE + 2,
            'causality': 'input', 'variability': 'discrete',
            'description': f'[ETH-RX] Valid byte count (0..{ETH_PAYLOAD}; 0 = no frame this step)',
            'type': 'Integer', 'start': 0, '_eth': 'rx_size',
        },
        # ── TX: FMU fills static buffer, sets lo/hi/size; master reads after step ───
        {
            'name': 'EthTxOut.lo',
            'valueReference': ETH_TX_VR_BASE + 0,
            'causality': 'output', 'variability': 'discrete',
            'description': '[ETH-TX] Lower 32 bits of 64-bit pointer to FMU TX frame buffer',
            'type': 'Integer', 'start': 0, '_eth': 'tx_lo',
        },
        {
            'name': 'EthTxOut.hi',
            'valueReference': ETH_TX_VR_BASE + 1,
            'causality': 'output', 'variability': 'discrete',
            'description': '[ETH-TX] Upper 32 bits of 64-bit pointer to FMU TX frame buffer',
            'type': 'Integer', 'start': 0, '_eth': 'tx_hi',
        },
        {
            'name': 'EthTxOut.size',
            'valueReference': ETH_TX_VR_BASE + 2,
            'causality': 'output', 'variability': 'discrete',
            'description': f'[ETH-TX] Byte count to send (0..{ETH_PAYLOAD}; 0 = nothing this step)',
            'type': 'Integer', 'start': 0, '_eth': 'tx_size',
        },
    ]
    print(f"  Ethernet: lo/hi pointer-triplet  RX VR {ETH_RX_VR_BASE}-{ETH_RX_VR_BASE+2}  TX VR {ETH_TX_VR_BASE}-{ETH_TX_VR_BASE+2}  (1 frame/step)")
    return eth_vars


# ─── FMI Configuration Parameters ────────────────────────────────────────────
def build_parameter_variables():
    """
    FMI 2.0 parameters (causality=parameter, variability=fixed).
    Required by SD.00454 §5.2.4, §5.3.3, §5.3.9:
      - Connection settings (IP, port) without recompilation
      - Sensor mounting position and orientation
    VR range: 100-199 (below CAN RX base of 1000)
    """
    params = [
        # ── Ethernet connection settings ──────────────────────────────────
        {
            'name': 'Config_ETH_RX_IP',
            'valueReference': 100,
            'causality': 'parameter', 'variability': 'fixed',
            'description': '[CONFIG] Ethernet RX source IP address filter (empty = accept all)',
            'type': 'String', 'start': '',
        },
        {
            'name': 'Config_ETH_RX_Port',
            'valueReference': 101,
            'causality': 'parameter', 'variability': 'fixed',
            'description': '[CONFIG] Ethernet RX UDP listen port (0 = disabled)',
            'type': 'Integer', 'start': 0,
        },
        {
            'name': 'Config_ETH_TX_IP',
            'valueReference': 102,
            'causality': 'parameter', 'variability': 'fixed',
            'description': '[CONFIG] Ethernet TX destination IP address',
            'type': 'String', 'start': '',
        },
        {
            'name': 'Config_ETH_TX_Port',
            'valueReference': 103,
            'causality': 'parameter', 'variability': 'fixed',
            'description': '[CONFIG] Ethernet TX destination UDP port (0 = disabled)',
            'type': 'Integer', 'start': 0,
        },
        # ── Sensor mounting position (metres, vehicle coordinate frame) ───
        {
            'name': 'Config_Sensor_Pos_X',
            'valueReference': 110,
            'causality': 'parameter', 'variability': 'fixed',
            'description': '[CONFIG] Sensor mounting position X (longitudinal, m)',
            'type': 'Real', 'start': 0.0, 'unit': 'm',
        },
        {
            'name': 'Config_Sensor_Pos_Y',
            'valueReference': 111,
            'causality': 'parameter', 'variability': 'fixed',
            'description': '[CONFIG] Sensor mounting position Y (lateral, m)',
            'type': 'Real', 'start': 0.0, 'unit': 'm',
        },
        {
            'name': 'Config_Sensor_Pos_Z',
            'valueReference': 112,
            'causality': 'parameter', 'variability': 'fixed',
            'description': '[CONFIG] Sensor mounting position Z (vertical, m)',
            'type': 'Real', 'start': 0.0, 'unit': 'm',
        },
        # ── Sensor mounting orientation (degrees, vehicle coordinate frame) ─
        {
            'name': 'Config_Sensor_Ori_Roll',
            'valueReference': 113,
            'causality': 'parameter', 'variability': 'fixed',
            'description': '[CONFIG] Sensor orientation Roll angle (deg)',
            'type': 'Real', 'start': 0.0, 'unit': 'Degree',
        },
        {
            'name': 'Config_Sensor_Ori_Pitch',
            'valueReference': 114,
            'causality': 'parameter', 'variability': 'fixed',
            'description': '[CONFIG] Sensor orientation Pitch angle (deg)',
            'type': 'Real', 'start': 0.0, 'unit': 'Degree',
        },
        {
            'name': 'Config_Sensor_Ori_Yaw',
            'valueReference': 115,
            'causality': 'parameter', 'variability': 'fixed',
            'description': '[CONFIG] Sensor orientation Yaw angle (deg, 0=forward)',
            'type': 'Real', 'start': 0.0, 'unit': 'Degree',
        },
    ]
    print(f"  Configuration parameters: {len(params)}  (VR 100-115)")
    return params


# ─── modelDescription.xml ────────────────────────────────────────────────────
def write_model_description(variables, fmu_name, out_file):
    root = ET.Element('fmiModelDescription')
    root.set('fmiVersion', '2.0')
    root.set('modelName', fmu_name)
    root.set('guid', '{' + str(uuid.uuid4()) + '}')
    root.set('description',
             'Signal-based FMU - Stellantis LRCF FD-CAN15 (JOB3_Entry_LRCF_FD_CAN15_ver7). '
             'CAN: inputs VCAN_RX.<Msg>.<Signal> from ZCU_F/ZCU_CL; outputs VCAN_TX.<Msg>.<Signal> from LRCF perception. '
             f'Ethernet: lo/hi pointer-triplet, 1 frame/step (EthRxIn VR {ETH_RX_VR_BASE}-{ETH_RX_VR_BASE+2}, EthTxOut VR {ETH_TX_VR_BASE}-{ETH_TX_VR_BASE+2}). '
             'Required step size: 1 ms (1000 Hz). '
             'Config parameters VR 100-115: ETH IP/port hint + sensor position/orientation.')
    root.set('generationTool', 'dbc_to_fmi_generator.py v5.3')
    root.set('generationDateAndTime',
             datetime.datetime.now().strftime('%Y-%m-%dT%H:%M:%SZ'))
    root.set('variableNamingConvention', 'structured')
    root.set('numberOfEventIndicators', '0')

    cosim = ET.SubElement(root, 'CoSimulation')
    cosim.set('modelIdentifier', fmu_name)
    cosim.set('canHandleVariableCommunicationStepSize', 'true')
    cosim.set('canInterpolateInputs', 'false')
    cosim.set('maxOutputDerivativeOrder', '0')
    cosim.set('canGetAndSetFMUstate', 'false')
    cosim.set('canSerializeFMUstate', 'false')
    cosim.set('providesDirectionalDerivative', 'false')
    # NOTE: stepSize is FMI 3.0 only; SD.00454 §5.3.2 required 1 ms step is
    # documented in the description attribute and the ICD.

    # FMI 2.0 requires every unit referenced in a ScalarVariable to be declared here
    all_units = sorted(set(
        v['unit'] for v in variables
        if v.get('unit') and v['unit'] not in ('', '-')
    ))
    if all_units:
        ud = ET.SubElement(root, 'UnitDefinitions')
        for unit_name in all_units:
            ET.SubElement(ud, 'Unit').set('name', unit_name)

    mv = ET.SubElement(root, 'ModelVariables')
    for v in variables:
        sv = ET.SubElement(mv, 'ScalarVariable')
        sv.set('name', v['name'])
        sv.set('valueReference', str(v['valueReference']))
        sv.set('causality', v['causality'])
        sv.set('variability', v['variability'])
        sv.set('description', v['description'])
        if v['causality'] == 'output':
            sv.set('initial', 'exact')
        if v['type'] == 'Real':
            te = ET.SubElement(sv, 'Real')
            te.set('start', str(v['start']))
            if v.get('unit'):
                te.set('unit', v['unit'])
            # SD.00454 §5.2.5: write min/max when available from DBC
            if v.get('min') is not None:
                te.set('min', str(v['min']))
            if v.get('max') is not None:
                te.set('max', str(v['max']))
        elif v['type'] == 'String':
            te = ET.SubElement(sv, 'String')
            te.set('start', str(v['start']))
        else:
            te = ET.SubElement(sv, 'Integer')
            te.set('start', str(int(v['start'])))

    ms = ET.SubElement(root, 'ModelStructure')
    outs = ET.SubElement(ms, 'Outputs')
    for idx, v in enumerate(variables, 1):
        if v['causality'] == 'output':
            ET.SubElement(outs, 'Unknown').set('index', str(idx))

    with open(out_file, 'w', encoding='utf-8') as f:
        f.write(prettify_xml(root))
    print(f"  OK {out_file}")


# ─── C++ structs header ───────────────────────────────────────────────────────
def write_cpp_structs(all_variables, rx_msgs, tx_msgs, out_file):
    L = [
        '// AUTO-GENERATED by dbc_to_fmi_generator.py v5.0 -- do not edit',
        '#pragma once',
        '#include <cstdint>',
        '',
        '// ============================================================',
        '// ETHERNET POINTER-TRIPLET STRUCTURE',
        '// One frame per fmi2DoStep.  RX: master sets lo/hi/size before step.',
        '// TX: FMU sets lo/hi pointing to its static buffer after step.',
        '// ============================================================',
        '',
        'struct FMU_ETH_PTR_t {',
        '    int32_t lo;    // lower 32 bits of 64-bit buffer address (uintptr_t cast)',
        '    int32_t hi;    // upper 32 bits of 64-bit buffer address',
        '    int32_t size;  // valid byte count (0 = no data this step)',
        '};',
        '',
        '// ============================================================',
        '// CAN DATA STRUCTURES',
        '// ============================================================',
        '',
    ]

    def emit_can_struct(msg, direction):
        L.append(f'// {"INPUT " if direction=="rx" else "OUTPUT"} -- {msg.name}  '
                 f'(0x{msg.frame_id:X}, {len(msg.signals)} signals)')
        L.append(f'struct {msg.name}_t {{')
        for sig in msg.signals:
            if sig.name in EXCLUDE_SIGNAL_NAMES:
                continue
            is_real = signal_is_real(sig)
            ctype   = 'double' if is_real else 'int32_t'
            unit_c  = f'  // [{sig.unit}]' if (sig.unit and sig.unit not in ('-', '')) else ''
            L.append(f'    {ctype:<12} {sig.name};{unit_c}')
        L.append('};')
        L.append('')

    L.append('// ─── FMU CAN INPUT structs ───────────────────────────────────')
    for msg in rx_msgs:
        emit_can_struct(msg, 'rx')

    L.append('// ─── FMU CAN OUTPUT structs ──────────────────────────────────')
    for msg in tx_msgs:
        emit_can_struct(msg, 'tx')

    L += [
        '// ─── Aggregate CAN containers ────────────────────────────────',
        'struct FMU_CAN_RX_t {',
    ]
    for msg in rx_msgs:
        L.append(f'    {msg.name}_t {msg.name};')
    L += ['};', '', 'struct FMU_CAN_TX_t {']
    for msg in tx_msgs:
        L.append(f'    {msg.name}_t {msg.name};')
    L += ['};', '',
          '// ─── Top-level FMU instance state (included by both generated_signal_map.cpp and LogicModel2.cpp) ───',
          'struct FMU_Instance {',
          '    FMU_CAN_RX_t  rx;       // CAN inputs',
          '    FMU_CAN_TX_t  tx;       // CAN outputs',
          '    FMU_ETH_PTR_t eth_rx;   // Ethernet RX pointer-triplet (set by master before DoStep)',
          '    FMU_ETH_PTR_t eth_tx;   // Ethernet TX pointer-triplet (set by FMU during DoStep)',
          '};', '']

    with open(out_file, 'w', encoding='utf-8') as f:
        f.write('\n'.join(L))
    print(f"  OK {out_file}")


# ─── C++ signal mapping ───────────────────────────────────────────────────────
def write_cpp_signal_map(all_variables, fmu_name, out_file):
    # CAN variables only (identified by presence of _msg/_sig keys)
    can_vars = [v for v in all_variables if '_msg' in v]
    ri = [v for v in can_vars if v['causality'] == 'input'  and v['type'] == 'Real']
    ro = [v for v in can_vars if v['causality'] == 'output' and v['type'] == 'Real']
    ii = [v for v in can_vars if v['causality'] == 'input'  and v['type'] == 'Integer']
    io = [v for v in can_vars if v['causality'] == 'output' and v['type'] == 'Integer']

    L = [
        '// AUTO-GENERATED by dbc_to_fmi_generator.py v5.0 -- do not edit',
        '#include "fmi2Functions.h"',
        '#include "generated_structs.h"',
        '#include "LogicModel2.h"',
        '#include <cstring>',
        '#include <cstdlib>',
        '',
        '#ifndef FMI2_Export',
        '#define FMI2_Export __attribute__((visibility("default")))',
        '#endif',
        '',
        '// ─── FMI 2.0 exports — must have C linkage ──────────────────',
        'extern "C" {',
        '',
        '// ─── Lifecycle ────────────────────────────────────────────────',
        'FMI2_Export const char* fmi2GetTypesPlatform() { return fmi2TypesPlatform; }',
        'FMI2_Export const char* fmi2GetVersion()       { return fmi2Version; }',
        'FMI2_Export fmi2Status  fmi2SetDebugLogging(fmi2Component,fmi2Boolean,size_t,const fmi2String[]) { return fmi2OK; }',
        '',
        'FMI2_Export fmi2Component fmi2Instantiate(',
        '    fmi2String, fmi2Type, fmi2String, fmi2String,',
        '    const fmi2CallbackFunctions*, fmi2Boolean, fmi2Boolean)',
        '{',
        '    auto* inst = (FMU_Instance*)calloc(1, sizeof(FMU_Instance));',
        '    return inst;',
        '}',
        'FMI2_Export void       fmi2FreeInstance(fmi2Component c)                                                          { free(c); }',
        'FMI2_Export fmi2Status fmi2SetupExperiment(fmi2Component,fmi2Boolean,fmi2Real,fmi2Real,fmi2Boolean,fmi2Real)       { return fmi2OK; }',
        'FMI2_Export fmi2Status fmi2EnterInitializationMode(fmi2Component)                                                  { return fmi2OK; }',
        'FMI2_Export fmi2Status fmi2ExitInitializationMode(fmi2Component)                                                   { return fmi2OK; }',
        'FMI2_Export fmi2Status fmi2Terminate(fmi2Component)                                                                { return fmi2OK; }',
        'FMI2_Export fmi2Status fmi2Reset(fmi2Component)                                                                    { return fmi2OK; }',
        'FMI2_Export fmi2Status fmi2DoStep(fmi2Component c, fmi2Real t, fmi2Real h, fmi2Boolean)',
        '{',
        '    FMU_Instance* inst = (FMU_Instance*)c;',
        '    return LogicModel2_step(inst, t, h);',
        '}',
        '',
        '// ─── fmi2SetReal  (CAN Real inputs) ──────────────────────────',
        'FMI2_Export fmi2Status fmi2SetReal(',
        '    fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Real value[])',
        '{',
        '    FMU_Instance* inst = (FMU_Instance*)c;',
        '    for (size_t i = 0; i < nvr; ++i) {',
        '        switch (vr[i]) {',
    ]
    for v in ri:
        L.append(f'            case {v["valueReference"]}: inst->rx.{v["_msg"]}.{v["_sig"]} = value[i]; break;')
    L += [
        '            default: break;',
        '        }',
        '    }',
        '    return fmi2OK;',
        '}',
        '',
        '// ─── fmi2GetReal  (CAN Real outputs) ─────────────────────────',
        'FMI2_Export fmi2Status fmi2GetReal(',
        '    fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Real value[])',
        '{',
        '    FMU_Instance* inst = (FMU_Instance*)c;',
        '    for (size_t i = 0; i < nvr; ++i) {',
        '        switch (vr[i]) {',
    ]
    for v in ro:
        L.append(f'            case {v["valueReference"]}: value[i] = inst->tx.{v["_msg"]}.{v["_sig"]}; break;')
    L += [
        '            default: value[i] = 0.0; break;',
        '        }',
        '    }',
        '    return fmi2OK;',
        '}',
        '',
        '// ─── fmi2SetInteger  (CAN Integer inputs + ETH RX pointer-triplet) ────────',
        'FMI2_Export fmi2Status fmi2SetInteger(',
        '    fmi2Component c, const fmi2ValueReference vr[], size_t nvr, const fmi2Integer value[])',
        '{',
        '    FMU_Instance* inst = (FMU_Instance*)c;',
        '    for (size_t i = 0; i < nvr; ++i) {',
        '',
        '        // ── CAN Integer inputs + ETH RX lo/hi/size ─────────────────',
        '        switch (vr[i]) {',
    ]
    for v in ii:
        L.append(f'            case {v["valueReference"]}: inst->rx.{v["_msg"]}.{v["_sig"]} = (int32_t)value[i]; break;')
    _eth_rx = {'rx_lo': 'eth_rx.lo', 'rx_hi': 'eth_rx.hi', 'rx_size': 'eth_rx.size'}
    for v in [x for x in all_variables if '_eth' in x and x['causality'] == 'input']:
        L.append(f'            case {v["valueReference"]}: inst->{_eth_rx[v["_eth"]]} = (int32_t)value[i]; break;')
    L += [
        '            default: break;',
        '        }',
        '    }',
        '    return fmi2OK;',
        '}',
        '',
        '// ─── fmi2GetInteger  (CAN Integer outputs + ETH TX pointer-triplet) ────────',
        'FMI2_Export fmi2Status fmi2GetInteger(',
        '    fmi2Component c, const fmi2ValueReference vr[], size_t nvr, fmi2Integer value[])',
        '{',
        '    FMU_Instance* inst = (FMU_Instance*)c;',
        '    for (size_t i = 0; i < nvr; ++i) {',
        '',
        '        // ── CAN Integer outputs + ETH TX lo/hi/size ────────────────',
        '        switch (vr[i]) {',
    ]
    for v in io:
        L.append(f'            case {v["valueReference"]}: value[i] = (fmi2Integer)inst->tx.{v["_msg"]}.{v["_sig"]}; break;')
    _eth_tx = {'tx_lo': 'eth_tx.lo', 'tx_hi': 'eth_tx.hi', 'tx_size': 'eth_tx.size'}
    for v in [x for x in all_variables if '_eth' in x and x['causality'] == 'output']:
        L.append(f'            case {v["valueReference"]}: value[i] = inst->{_eth_tx[v["_eth"]]}; break;')
    L += [
        '            default: value[i] = 0; break;',
        '        }',
        '    }',
        '    return fmi2OK;',
        '}',
        '',
        'FMI2_Export fmi2Status fmi2GetBoolean(fmi2Component,const fmi2ValueReference[],size_t,fmi2Boolean[])      { return fmi2OK; }',
        'FMI2_Export fmi2Status fmi2SetBoolean(fmi2Component,const fmi2ValueReference[],size_t,const fmi2Boolean[]){ return fmi2OK; }',
        'FMI2_Export fmi2Status fmi2GetString (fmi2Component,const fmi2ValueReference[],size_t,fmi2String[])       { return fmi2OK; }',
        'FMI2_Export fmi2Status fmi2SetString (fmi2Component,const fmi2ValueReference[],size_t,const fmi2String[]) { return fmi2OK; }',
        '',
        '// ─── Optional FMU state functions (stubs – not supported) ───',
        'FMI2_Export fmi2Status fmi2GetFMUstate                (fmi2Component,fmi2FMUstate*)                          { return fmi2Error; }',
        'FMI2_Export fmi2Status fmi2SetFMUstate                (fmi2Component,fmi2FMUstate)                           { return fmi2Error; }',
        'FMI2_Export fmi2Status fmi2FreeFMUstate               (fmi2Component,fmi2FMUstate*)                          { return fmi2Error; }',
        'FMI2_Export fmi2Status fmi2SerializedFMUstateSize     (fmi2Component,fmi2FMUstate,size_t*)                   { return fmi2Error; }',
        'FMI2_Export fmi2Status fmi2SerializeFMUstate          (fmi2Component,fmi2FMUstate,fmi2Byte[],size_t)         { return fmi2Error; }',
        'FMI2_Export fmi2Status fmi2DeSerializeFMUstate        (fmi2Component,const fmi2Byte[],size_t,fmi2FMUstate*)  { return fmi2Error; }',
        'FMI2_Export fmi2Status fmi2GetDirectionalDerivative   (fmi2Component,const fmi2ValueReference[],size_t,',
        '                                                        const fmi2ValueReference[],size_t,',
        '                                                        const fmi2Real[],fmi2Real[])                          { return fmi2Error; }',
        '',
        '// ─── Optional status functions for async stepping (stubs) ───',
        'FMI2_Export fmi2Status fmi2CancelStep       (fmi2Component)                                { return fmi2Error; }',
        'FMI2_Export fmi2Status fmi2GetStatus        (fmi2Component,const fmi2StatusKind,fmi2Status*)        { return fmi2Error; }',
        'FMI2_Export fmi2Status fmi2GetRealStatus    (fmi2Component,const fmi2StatusKind,fmi2Real*)          { return fmi2Error; }',
        'FMI2_Export fmi2Status fmi2GetIntegerStatus (fmi2Component,const fmi2StatusKind,fmi2Integer*)       { return fmi2Error; }',
        'FMI2_Export fmi2Status fmi2GetBooleanStatus (fmi2Component,const fmi2StatusKind,fmi2Boolean*)       { return fmi2Error; }',
        'FMI2_Export fmi2Status fmi2GetStringStatus  (fmi2Component,const fmi2StatusKind,fmi2String*)        { return fmi2Error; }',
        '',
        '// ─── Co-Simulation interpolation stubs ───',
        'FMI2_Export fmi2Status fmi2SetRealInputDerivatives  (fmi2Component,const fmi2ValueReference[],size_t,const fmi2Integer[],const fmi2Real[])  { return fmi2Error; }',
        'FMI2_Export fmi2Status fmi2GetRealOutputDerivatives (fmi2Component,const fmi2ValueReference[],size_t,const fmi2Integer[],fmi2Real[])        { return fmi2Error; }',
        '',
        '// ─── Model Exchange stubs (not used in CoSim mode, but fmpy loads them) ───',
        'FMI2_Export fmi2Status fmi2SetTime                      (fmi2Component,fmi2Real)                        { return fmi2Error; }',
        'FMI2_Export fmi2Status fmi2SetContinuousStates          (fmi2Component,const fmi2Real[],size_t)         { return fmi2Error; }',
        'FMI2_Export fmi2Status fmi2EnterEventMode               (fmi2Component)                                { return fmi2Error; }',
        'FMI2_Export fmi2Status fmi2NewDiscreteStates            (fmi2Component,fmi2EventInfo*)                  { return fmi2Error; }',
        'FMI2_Export fmi2Status fmi2EnterContinuousTimeMode      (fmi2Component)                                { return fmi2Error; }',
        'FMI2_Export fmi2Status fmi2CompletedIntegratorStep      (fmi2Component,fmi2Boolean,fmi2Boolean*,fmi2Boolean*)  { return fmi2Error; }',
        'FMI2_Export fmi2Status fmi2GetDerivatives               (fmi2Component,fmi2Real[],size_t)               { return fmi2Error; }',
        'FMI2_Export fmi2Status fmi2GetEventIndicators           (fmi2Component,fmi2Real[],size_t)               { return fmi2Error; }',
        'FMI2_Export fmi2Status fmi2GetContinuousStates          (fmi2Component,fmi2Real[],size_t)               { return fmi2Error; }',
        'FMI2_Export fmi2Status fmi2GetNominalsOfContinuousStates(fmi2Component,fmi2Real[],size_t)               { return fmi2Error; }',
        '',
        '} // extern "C"',
    ]

    with open(out_file, 'w', encoding='utf-8') as f:
        f.write('\n'.join(L))
    print(f"  OK {out_file}")


# ─── Entry point ──────────────────────────────────────────────────────────────
def main():
    if len(sys.argv) < 2:
        print(__doc__)
        sys.exit(1)

    dbc_file     = sys.argv[1]
    args         = sys.argv[2:]
    fmu_node     = next((a for a in args if not a.startswith('--')), 'LRCF')
    include_diag = '--include-diag' in args

    if not os.path.isfile(dbc_file):
        print(f"ERROR: not found: {dbc_file}"); sys.exit(1)

    base     = os.path.splitext(os.path.basename(dbc_file))[0]
    fmu_name = "LogicModel2"
    out_dir  = os.path.dirname(os.path.abspath(dbc_file))

    print(f"\n{'='*62}")
    print(f"  DBC -> FMI Generator  v5.3  (CAN + Ethernet lo/hi pointer-triplet)")
    print(f"  DBC      : {os.path.basename(dbc_file)}")
    print(f"  FMU node : {fmu_node}")
    print(f"  Diag msgs: {'included' if include_diag else 'excluded'}")
    print(f"{'='*62}")

    can_variables, rx_msgs, tx_msgs = build_can_variables(dbc_file, fmu_node, include_diag)
    eth_variables                   = build_ethernet_variables()
    param_variables                 = build_parameter_variables()
    all_variables                   = param_variables + can_variables + eth_variables

    n_can_in  = sum(1 for v in can_variables if v['causality'] == 'input')
    n_can_out = sum(1 for v in can_variables if v['causality'] == 'output')
    n_eth_in  = sum(1 for v in eth_variables if v['causality'] == 'input')
    n_eth_out = sum(1 for v in eth_variables if v['causality'] == 'output')
    n_params  = len(param_variables)

    print(f"\n{'─'*62}")
    print(f"  Config parameters:             {n_params:6d}  (VR 100-115)")
    print(f"  CAN inputs  (RX into {fmu_node}):  {n_can_in:6d}")
    print(f"  CAN outputs (TX from {fmu_node}):  {n_can_out:6d}")
    print(f"  Ethernet RX lo/hi/size:        {n_eth_in:6d}  (VR {ETH_RX_VR_BASE}-{ETH_RX_VR_BASE+2})")
    print(f"  Ethernet TX lo/hi/size:        {n_eth_out:6d}  (VR {ETH_TX_VR_BASE}-{ETH_TX_VR_BASE+2})")
    print(f"  ─────────────────────────────────────────────────────")
    print(f"  TOTAL FMI variables:           {len(all_variables):6d}")
    print(f"{'─'*62}")

    print("\nWriting output files:")
    write_model_description(all_variables, fmu_name,
                            os.path.join(out_dir, 'modelDescription.xml'))
    write_cpp_structs(all_variables, rx_msgs, tx_msgs,
                      os.path.join(out_dir, 'generated_structs.h'))
    write_cpp_signal_map(all_variables, fmu_name,
                         os.path.join(out_dir, 'generated_signal_map.cpp'))

    print(f"\nDone: {len(all_variables)} FMI variables "
          f"({len(can_variables)} CAN + {n_params} params + {len(eth_variables)} ETH lo/hi)")


if __name__ == '__main__':
    main()
