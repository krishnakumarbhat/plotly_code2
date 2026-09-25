#!/usr/bin/env python3
"""
validate_lrcf_fmu.py  —  LogicModel2 FMU Validator
===================================================
Tests:
  [1] FMI 2.0 XML schema validation            (fmpy)
  [2] Model description inspection             (variable counts, spot-checks)
  [3] Co-simulation: instantiate→init→DoStep   (fmpy simulate_fmu)
  [4] FMI API round-trip via ctypes            (SetInteger→DoStep→GetInteger)

Usage:
  python3 validate_lrcf_fmu.py [path/to/LogicModel2.fmu]
"""

import sys, os, zipfile, ctypes, tempfile, shutil

FMU_FILE = sys.argv[1] if len(sys.argv) > 1 else 'LogicModel2.fmu'

PASS = "PASS"; WARN = "WARN"; FAIL = "FAIL"
results = {}

def hdr(n, title):
    print(f"\n[{n}] {title}")
    print("-" * 56)

def check(label, ok, detail=""):
    icon = "✓" if ok else "✗"
    print(f"  {icon}  {label}" + (f"  →  {detail}" if detail else ""))
    return ok

# ─────────────────────────────────────────────────────────────────────────────
# [1] XML Schema Validation
# ─────────────────────────────────────────────────────────────────────────────
def test_schema():
    hdr("1/4", "FMI 2.0 XML Schema Validation")
    try:
        from fmpy.validation import validate_fmu
        issues = validate_fmu(FMU_FILE)
        errors  = [i for i in issues if 'error'   in str(i).lower()]
        warnings= [i for i in issues if 'warning' in str(i).lower()]
        for w in warnings: print(f"  ⚠  {w}")
        for e in errors:   print(f"  ✗  {e}")
        ok = len(errors) == 0
        check("Schema conformance", ok,
              "no errors" if ok else f"{len(errors)} error(s)")
        return PASS if ok else FAIL
    except Exception as e:
        print(f"  ERROR: {e}")
        return FAIL

# ─────────────────────────────────────────────────────────────────────────────
# [2] Model Description Inspection
# ─────────────────────────────────────────────────────────────────────────────
def test_model_desc():
    hdr("2/4", "Model Description Inspection")
    try:
        from fmpy.model_description import read_model_description
        md = read_model_description(FMU_FILE)

        inputs  = [v for v in md.modelVariables if v.causality == 'input']
        outputs = [v for v in md.modelVariables if v.causality == 'output']

        can_in   = [v for v in inputs  if v.name.startswith('VCAN_RX.')]
        can_out  = [v for v in outputs if v.name.startswith('VCAN_TX.')]
        params   = [v for v in md.modelVariables if v.causality == 'parameter']

        print(f"  Model name  : {md.modelName}")
        print(f"  FMI version : {md.fmiVersion}")
        print(f"  GUID        : {md.guid}")
        print(f"  CoSim type  : {md.coSimulation.modelIdentifier}")
        print()
        print(f"  Total variables  : {len(md.modelVariables):>6}")
        print(f"  ├─ Parameters    : {len(params):>6}  (VR 100-115)")
        print(f"  ├─ Inputs        : {len(inputs):>6}")
        print(f"  │  └─ CAN RX     : {len(can_in):>6}  (VCAN_RX.<Msg>.<Signal>)")
        print(f"  └─ Outputs       : {len(outputs):>6}")
        print(f"     └─ CAN TX     : {len(can_out):>6}  (VCAN_TX.<Msg>.<Signal>)")
        print(f"  Ethernet: lo/hi pointer-triplet  RX VR 5000-5002  TX VR 5003-5005  (1 frame/step)")

        # Spot-check: Config params and first CAN RX/TX signals
        vr_map = {v.valueReference: v.name for v in md.modelVariables}
        spot = [
            (100,  "Config_ETH_RX_IP"),
            (101,  "Config_ETH_RX_Port"),
            (102,  "Config_ETH_TX_IP"),
            (103,  "Config_ETH_TX_Port"),
            (1000, "VCAN_RX.FD15_ZCU_F_DATA_9.E2E_CRC_ZCU_F_DATA_9"),
            (1003, "VCAN_RX.FD15_ZCU_F_DATA_9.ACCEL_PEDAL_PERCENT_PWT"),
            (1205, "VCAN_TX.FD15_LRCF_DATA_7.VHL_1_CL_SUB_TYPE"),
            (1215, "VCAN_TX.FD15_LRCF_DATA_7.VHL_1_LEFT_ANGLE"),
            # Ethernet lo/hi pointer-triplet
            (5000, "EthRxIn.lo"),
            (5001, "EthRxIn.hi"),
            (5002, "EthRxIn.size"),
            (5003, "EthTxOut.lo"),
            (5004, "EthTxOut.hi"),
            (5005, "EthTxOut.size"),
        ]
        print()
        all_ok = True
        for vr, expected in spot:
            got = vr_map.get(vr, "<not found>")
            ok  = (got == expected)
            all_ok &= ok
            check(f"VR {vr:>6}  =  {expected}", ok, got if not ok else "")

        # 205 CAN RX + 3 ETH RX (lo/hi/size) = 208 inputs
        # 330 CAN TX + 3 ETH TX (lo/hi/size) = 333 outputs
        ok_counts = (len(inputs) == 208 and len(outputs) == 333)
        check("Variable counts (208 in / 333 out)", ok_counts)

        ok_naming = (md.variableNamingConvention == 'structured')
        check("variableNamingConvention = structured", ok_naming)

        return PASS if (all_ok and ok_counts and ok_naming) else WARN
    except Exception as e:
        print(f"  ERROR: {e}")
        import traceback; traceback.print_exc()
        return FAIL

# ─────────────────────────────────────────────────────────────────────────────
# [3] Co-Simulation via fmpy
# ─────────────────────────────────────────────────────────────────────────────
def test_cosim():
    hdr("3/4", "Co-Simulation (fmpy.simulate_fmu, 5 steps × 10 ms)")
    try:
        from fmpy import simulate_fmu
        # Only record a handful of outputs to keep it fast
        watched = [
            'VCAN_TX.FD15_LRCF_DATA_7.VHL_1_LEFT_ANGLE',
            'VCAN_TX.FD15_LRCF_DATA_7.VHL_2_LEFT_ANGLE',
            'VCAN_TX.FD15_LRCF_DATA_7.VHL_3_LEFT_ANGLE',
        ]
        result = simulate_fmu(
            FMU_FILE,
            start_time=0.0,
            stop_time=0.05,
            output_interval=0.01,
            output=watched,
        )
        check("Simulation completed", True, f"{len(result)} rows recorded")
        for col in watched:
            val = result[col][-1]
            check(f"{col}", True, f"final = {val}")
        return PASS
    except Exception as e:
        print(f"  ERROR: {e}")
        import traceback; traceback.print_exc()
        return FAIL

# ─────────────────────────────────────────────────────────────────────────────
# [4] FMI API Round-Trip (ctypes direct)
# ─────────────────────────────────────────────────────────────────────────────
def test_api_roundtrip():
    hdr("4/4", "FMI API Round-Trip (ctypes)")
    tmp = tempfile.mkdtemp(prefix="fmu_test_")
    try:
        # Extract .so
        with zipfile.ZipFile(FMU_FILE, 'r') as zf:
            zf.extractall(tmp)
        so = os.path.join(tmp, 'binaries', 'linux64', 'LogicModel2.so')
        check("Binary present in FMU", os.path.exists(so), so)

        lib = ctypes.cdll.LoadLibrary(so)
        check("dlopen .so", True)

        # --- fmi2Instantiate ---
        lib.fmi2Instantiate.restype = ctypes.c_void_p
        comp = lib.fmi2Instantiate(
            b"LogicModel2_Test", ctypes.c_int(1),
            b"test-guid", b"",
            ctypes.c_void_p(0), ctypes.c_int(0), ctypes.c_int(0))
        check("fmi2Instantiate", bool(comp))

        # --- fmi2SetupExperiment ---
        lib.fmi2SetupExperiment.restype  = ctypes.c_int
        lib.fmi2SetupExperiment.argtypes = [
            ctypes.c_void_p, ctypes.c_int, ctypes.c_double,
            ctypes.c_double, ctypes.c_int, ctypes.c_double,
        ]
        st = lib.fmi2SetupExperiment(comp, 0, 0.0, 0.0, 1, 1.0)
        check("fmi2SetupExperiment", st == 0, f"status={st}")

        # --- fmi2EnterInitializationMode ---
        lib.fmi2EnterInitializationMode.restype = ctypes.c_int
        st = lib.fmi2EnterInitializationMode(comp)
        check("fmi2EnterInitializationMode", st == 0, f"status={st}")

        # --- fmi2SetInteger: CAN Integer input VR=1000 ---
        lib.fmi2SetInteger.restype = ctypes.c_int
        vr  = (ctypes.c_uint * 1)(1000)
        val = (ctypes.c_int  * 1)(42)
        st  = lib.fmi2SetInteger(comp, vr, 1, val)
        check("fmi2SetInteger  CAN RX Int[0] = 42  (VR 1000)", st == 0, f"status={st}")

        # --- fmi2SetReal: CAN Real input VR=1003 ---
        lib.fmi2SetReal.restype = ctypes.c_int
        vr  = (ctypes.c_uint   * 1)(1003)
        val = (ctypes.c_double * 1)(1.234)
        st  = lib.fmi2SetReal(comp, vr, 1, val)
        check("fmi2SetReal     CAN RX Real[0] = 1.234  (VR 1003)", st == 0, f"status={st}")

        # --- fmi2ExitInitializationMode ---
        lib.fmi2ExitInitializationMode.restype = ctypes.c_int
        st = lib.fmi2ExitInitializationMode(comp)
        check("fmi2ExitInitializationMode", st == 0, f"status={st}")

        # --- fmi2DoStep ---
        lib.fmi2DoStep.restype  = ctypes.c_int
        lib.fmi2DoStep.argtypes = [ctypes.c_void_p, ctypes.c_double, ctypes.c_double, ctypes.c_int]
        st = lib.fmi2DoStep(comp, 0.0, 0.01, 1)
        check("fmi2DoStep", st == 0, f"status={st}")

        # --- fmi2GetInteger: CAN Integer output VR=1205 (VHL_1_CL_SUB_TYPE) ---
        lib.fmi2GetInteger.restype = ctypes.c_int
        vr  = (ctypes.c_uint * 1)(1205)
        out = (ctypes.c_int  * 1)(0)
        st  = lib.fmi2GetInteger(comp, vr, 1, out)
        check("fmi2GetInteger  CAN TX Int  (VR 1205)", st == 0,
              f"value={out[0]}  status={st}")

        # --- fmi2GetReal: CAN Real output VR=1215 (VHL_1_LEFT_ANGLE) ---
        lib.fmi2GetReal.restype = ctypes.c_int
        vr  = (ctypes.c_uint   * 1)(1215)
        out = (ctypes.c_double * 1)(0.0)
        st  = lib.fmi2GetReal(comp, vr, 1, out)
        check("fmi2GetReal     CAN TX Real (VR 1215)", st == 0,
              f"value={out[0]:.4f}  status={st}")

        # --- fmi2FreeInstance ---
        lib.fmi2FreeInstance.restype = None
        lib.fmi2FreeInstance(comp)
        check("fmi2FreeInstance", True)

        return PASS
    except Exception as e:
        print(f"  ERROR: {e}")
        import traceback; traceback.print_exc()
        return FAIL
    finally:
        shutil.rmtree(tmp, ignore_errors=True)

# ─────────────────────────────────────────────────────────────────────────────
# Main
# ─────────────────────────────────────────────────────────────────────────────
def main():
    print("=" * 56)
    print("  LogicModel2  —  FMU Validator")
    print("  CAN FD (535 signals v5.3) + Ethernet via lo/hi pointer-triplet (6 FMI Integer vars)")
    print("=" * 56)

    if not os.path.exists(FMU_FILE):
        print(f"\n  ERROR: {FMU_FILE} not found")
        sys.exit(1)
    size_kb = os.path.getsize(FMU_FILE) / 1024
    print(f"\n  File : {FMU_FILE}  ({size_kb:.0f} KB)")

    for name, fn in [
        ("XML Schema",      test_schema),
        ("Model Desc",      test_model_desc),
        ("Co-Sim DoStep",   test_cosim),
        ("FMI API ctypes",  test_api_roundtrip),
    ]:
        try:
            results[name] = fn()
        except Exception as e:
            results[name] = FAIL
            print(f"  UNHANDLED: {e}")

    print("\n" + "=" * 56)
    print("  RESULTS")
    print("=" * 56)
    icons = {PASS: "✓", WARN: "⚠", FAIL: "✗"}
    for name, status in results.items():
        print(f"  {icons[status]}  {name:<26}  {status}")
    print("=" * 56)

    any_fail = any(v == FAIL for v in results.values())
    print("  ✅ ALL TESTS PASSED" if not any_fail else "  ❌ SOME TESTS FAILED")
    sys.exit(1 if any_fail else 0)

if __name__ == '__main__':
    main()
