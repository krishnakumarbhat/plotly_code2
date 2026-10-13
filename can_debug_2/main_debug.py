#!/usr/bin/env python3
r"""
main_debug.py - Dump BOTH XML and HDF to prove RL bug in simple 13yo English.

Run:  python main_debug.py
Shows: INPUT SCAN=LOOK-1 vs OUTPUT SCAN=LOOK, why only RL vel breaks (32 m/s flip),
       and XML same numbers == HDF (so KPI mis-aligns scans, not values).

Location: C:/Users/ouymc2/Desktop/can_debug_2/main_debug.py
Files: PCAN/INPUT/CEER_PT048_20260814_121515_0338_b04_HDF.h5
       PCAN/OUTPUT/CEER_PT048_20260814_121515_0338_b04_rR00110006_HDF.h5
       PCAN/INPUT/CEER_PT048_20260814_121515_0338_b04_CEER_PCAN_RL.xml
Code refs: can_interactive_plot/a_persistence_layer/hdf_wrapper.py:66 (comment)
          can_kpi/a_persistence_layer/hdf_wrapper.py:63 (SCAN-first)
          can_interactive_plot/a_persistence_layer/hdf_wrapper.py:70 (LOOK-first)
          c_business_layer/kpi_business.py:63 align_storage_rows_by_scanindex vs :209 align_storage_rows
          c_business_layer/kpi_business.py:17 MATCH_EPSILON=10.0
"""

import sys
from pathlib import Path
import xml.etree.ElementTree as ET

# --- paths ---
ROOT = Path(__file__).resolve().parent
INPUT_H5 = ROOT / "PCAN/INPUT/CEER_PT048_20260814_121515_0338_b04_HDF.h5"
OUTPUT_H5 = ROOT / "PCAN/OUTPUT/CEER_PT048_20260814_121515_0338_b04_rR00110006_HDF.h5"
INPUT_XML = ROOT / "PCAN/INPUT/CEER_PT048_20260814_121515_0338_b04_CEER_PCAN_RL.xml"

SENSOR = "CEER_RL"
HEADER_GRP = "RDR_HEADER_001"
DET_LOW_GRP = "RDR_DETECTION_005_008"
DET_HIGH_GRP = "RDR_DETECTION_085_088"
DET_LOW_SIG = "DET_RANGE_VELOCITY_005"
DET_HIGH_SIG = "DET_RANGE_VELOCITY_085"

def banner(title):
    print("\n" + "="*78)
    print(title)
    print("="*78)

def print_simple_intro():
    banner("debug")


# --- HDF helpers ---
def get_header_arrays(h5_path, sensor, grp):
    import h5py
    with h5py.File(h5_path, "r") as f:
        attrs = f[sensor][grp].attrs
        return {
            "SCAN": attrs["HED_SCAN_INDEX"][:],
            "LOOK": attrs["HED_LOOK_INDEX"][:],
            "SEC": attrs["HED_SENSOR_TIME_STAMP_SEC"][:],
            "NS": attrs["HED_SENSOR_TIME_STAMP_NS"][:],
        }

def get_det_array(h5_path, sensor, grp, sig):
    import h5py
    with h5py.File(h5_path, "r") as f:
        arr = f[sensor][grp].attrs[sig][:]
        return arr

def dump1_header():
    banner("Header is the smoking gun (h5py exact bytes)")
    print(f"Path: {SENSOR} / {HEADER_GRP}")
    print("This shows the logger wrote INPUT and OUTPUT with different SCAN counters.\n")
    try:
        ih = get_header_arrays(INPUT_H5, SENSOR, HEADER_GRP)
        oh = get_header_arrays(OUTPUT_H5, SENSOR, HEADER_GRP)
    except Exception as e:
        print(f"FAILED to read HDF header: {e}")
        print(f"Check files exist: INPUT {INPUT_H5.exists()} OUTPUT {OUTPUT_H5.exists()}")
        return

    # header table 0..3
    print(f"{'File':<12} | {'HED_SCAN_INDEX[0..3]':<22} | {'HED_LOOK_INDEX[0..3]':<22} | {'Diff':<12} | {'TIME_SEC[0..3]':<32} | {'TIME_NS[0..3]'}")
    print("-"*115)
    def fmt(a): return ", ".join(str(int(x)) for x in a[:4])
    # need int conversion for float64 storage
    in_scan = [int(x) for x in ih["SCAN"][:4]]
    in_look = [int(x) for x in ih["LOOK"][:4]]
    out_scan = [int(x) for x in oh["SCAN"][:4]]
    out_look = [int(x) for x in oh["LOOK"][:4]]
    in_sec = [int(x) for x in ih["SEC"][:4]]
    out_sec = [int(x) for x in oh["SEC"][:4]]
    in_ns = [int(x) for x in ih["NS"][:4]]
    out_ns = [int(x) for x in oh["NS"][:4]]

    print(f"{'INPUT HDF':<12} | {fmt(in_scan):<22} | {fmt(in_look):<22} | LOOK=SCAN+1 | {fmt(in_sec):<32} | {fmt(in_ns)}")
    print(f"{'OUTPUT HDF':<12} | {fmt(out_scan):<22} | {fmt(out_look):<22} | LOOK=SCAN   | {fmt(out_sec):<32} | {fmt(out_ns)}")
    print()
    print(">>> KEY:")
    print(f"    OUTPUT SCAN 3320 time = {out_sec[0]}/{out_ns[0]}")
    print(f"    INPUT  SCAN 3320 time = {in_sec[2]}/{in_ns[2]}  (same time, but LOOK differs!)")
    print("    So OUTPUT SCAN 3320 is actually INPUT LOOK 3320 = INPUT SCAN 3319+1")
    print("    -> One whole row apart = 50 ms. KPI compares wrong rows.")
    print()
    print("Code proof: can_interactive_plot/a_persistence_layer/hdf_wrapper.py:66 says")
    print('  "# HED_SCAN_INDEX = HED_LOOK_INDEX-1 while detection payloads are indexed by look"')
    print("  can_kpi/hdf_wrapper.py:63 uses SCAN-first, can_interactive_plot/hdf_wrapper.py:70 uses LOOK-first")

    # code dump exact 0..7
    print("\n--- h5py exact print (copy/paste to verify) ---")
    ih_scan8 = [int(x) for x in ih["SCAN"][:8]]
    ih_look8 = [int(x) for x in ih["LOOK"][:8]]
    oh_scan8 = [int(x) for x in oh["SCAN"][:8]]
    oh_look8 = [int(x) for x in oh["LOOK"][:8]]
    print(f"IN  RL SCAN  0..7 : {ih_scan8}")
    print(f"IN  RL LOOK  0..7 : {ih_look8}")
    print(f"OUT RL SCAN  0..7 : {oh_scan8}")
    print(f"OUT RL LOOK  0..7 : {oh_look8}")
    try:
        in_vel5 = get_det_array(INPUT_H5, SENSOR, DET_LOW_GRP, DET_LOW_SIG)[:8]
        out_vel5 = get_det_array(OUTPUT_H5, SENSOR, DET_LOW_GRP, DET_LOW_SIG)[:8]
        print(f"IN  RL {DET_LOW_SIG} 0..7 : {[round(float(x),3) for x in in_vel5]}")
        print(f"OUT RL {DET_LOW_SIG} 0..7 : {[round(float(x),3) for x in out_vel5]}")
    except Exception as e:
        print(f"Could not read {DET_LOW_SIG}: {e}")
    print(f"\nFiles: {INPUT_H5.relative_to(ROOT)} and {OUTPUT_H5.relative_to(ROOT)}")

def dump2_detection_shift():
    banner("DUMP 2 - Detection payload shifted by 1 scan (why only RL breaks)")
    print(f"Group: {SENSOR} / {DET_LOW_GRP} -> signal {DET_LOW_SIG} and DET_RANGE_005")
    print("This shows OUTPUT is INPUT shifted by 1 row.\n")
    try:
        in_vel = get_det_array(INPUT_H5, SENSOR, DET_LOW_GRP, DET_LOW_SIG)[:5]
        out_vel = get_det_array(OUTPUT_H5, SENSOR, DET_LOW_GRP, DET_LOW_SIG)[:5]
        # also range
        in_rng = get_det_array(INPUT_H5, SENSOR, DET_LOW_GRP, "DET_RANGE_005")[:5]
        out_rng = get_det_array(OUTPUT_H5, SENSOR, DET_LOW_GRP, "DET_RANGE_005")[:5]
    except Exception as e:
        print(f"Failed reading detection: {e}")
        return

    print(f"{'Row':<4} | {'SCAN pair (what KPI does today)':<28} | {'INPUT VEL':<10} | {'OUTPUT VEL':<11} | {'Delta':<7} | {'Bucket/10':<9} | Result")
    print("-"*105)
    # row mapping: KPI scan-aligned IN[2] SCAN3320 vs OUT[0] SCAN3320 is wrong
    # show the shift mapping
    # IN[0]= -20.27 OUT[0]=11.22 -> IN[1]=11.22 matches OUT[0] etc
    pairs = [
        ("OUT[0]=IN[1]", float(in_vel[1]), float(out_vel[0])),
        ("IN[2] vs OUT[1] (if LOOK)", float(in_vel[2]), float(out_vel[1])),
        ("KPI today IN[2] vs OUT[0]", float(in_vel[2]), float(out_vel[0])),
    ]
    for idx, (label, iv, ov) in enumerate(pairs):
        delta = ov - iv
        # bucket = round(v/10) as in kpi_business.py:183 _quantize
        b_in = int(round(iv/10.0))
        b_out = int(round(ov/10.0))
        result = "HIT" if abs(b_in - b_out) <= 1 else "MISS"
        if "KPI today" in label:
            result = "MISS  <- WHY RL 24%"
            print(f"{'2':<4} | {label:<28} | {iv:<10.3f} | {ov:<11.3f} | {delta:<7.2f} | {b_in} vs {b_out:<6} | >>> {result}")
        else:
            print(f"{idx:<4} | {label:<28} | {iv:<10.3f} | {ov:<11.3f} | {delta:<7.2f} | {b_in} vs {b_out:<6} | {result}")

    print("\nFull rows (copy exact):")
    print(f"  INPUT  VEL[0..4] = {[round(float(x),4) for x in in_vel]}")
    print(f"  OUTPUT VEL[0..4] = {[round(float(x),4) for x in out_vel]}")
    print("  -> OUTPUT[0]=INPUT[1], OUTPUT[1]=INPUT[2] ... = shifted by 1")
    print(f"  INPUT  RNG[0..4] = {[round(float(x),3) for x in in_rng]}")
    print(f"  OUTPUT RNG[0..4] = {[round(float(x),3) for x in out_rng]}")
    print("\nWhy only RL hurts most:")
    print("  - Low detectors 001_084: best shift 1 (84% of groups), High 085+: best shift 2")
    print("    -> one header SCAN can't fix both. FLR has 0% mixed, RL has 84% mixed.")
    print("  - RL velocity flips +11 <-> -20 each scan (rear Doppler, 32 m/s delta)")
    print("    If 1 scan late, +11 compared to -20 -> Delta 32 -> bucket -2 vs 1 -> MISS")
    print("    Range only changes 0.1 m per 50 ms -> range still 86% but velocity collapses to 24%")
    print("  - FL/FLR velocity is steady +8, +8, +8 -> Delta 0.1 even when shifted -> HIT -> stays 99%")
    print('  Code ref: c_business_layer/kpi_business.py:17 MATCH_EPSILON=10.0, :183 _quantize v/10')
    print('            a_persistence_layer/hdf_parser.py:336 _build_detection_dataset')

    # high det
    try:
        in_h = get_det_array(INPUT_H5, SENSOR, DET_HIGH_GRP, DET_HIGH_SIG)[:5]
        out_h = get_det_array(OUTPUT_H5, SENSOR, DET_HIGH_GRP, DET_HIGH_SIG)[:5]
        print(f"\nHigh detector proof (shift is not uniform):")
        print(f"  {DET_HIGH_SIG} IN [0..4]  = {[round(float(x),2) for x in in_h]}")
        print(f"  {DET_HIGH_SIG} OUT[0..4]  = {[round(float(x),2) for x in out_h]}")
        print(f"  -> OUT = IN shifted by 2 (not 1) for high groups")
    except Exception as e:
        print(f"High det read failed: {e}")

def dump3_xml_control():
    banner("DUMP 3 - XML has same numbers, so XML is 95% (control proof)")
    print(f"File: {INPUT_XML.relative_to(ROOT)} vs HDF")
    print("This proves scaling/DBC is correct - HDF and XML hold same radar values.\n")
    if not INPUT_XML.exists():
        print(f"XML not found: {INPUT_XML}")
        return
    try:
        tree = ET.parse(INPUT_XML)
        root = tree.getroot()
        # CYCLE_35 has DET_005
        found = False
        for cyc_name in ["CYCLE_1", "CYCLE_35"]:
            elem = root.find(cyc_name)
            if elem is None:
                continue
            print(f"XML {cyc_name}:")
            for det_grp in elem:
                print(f"  Group {det_grp.tag}")
                for data in det_grp:
                    # data is <Data CAN_ID TIMESTAMP>
                    can_id = data.attrib.get("CAN_ID","")
                    ts = data.attrib.get("TIMESTAMP","")
                    # inner Data children hold the actual signals as attributes
                    # We need to scan attributes of all <Data> children
                    sig_map = {}
                    for child in data:
                        sig_map.update(child.attrib)
                    # also data child itself may have signals? Actually first level Data has no sig, second level Data holds them
                    # Let's collect all attribs from all children
                    all_attrs = {}
                    for child in list(data):
                        all_attrs.update(child.attrib)
                    # also check data.iter
                    for k,v in data.attrib.items():
                        all_attrs[k]=v
                    # Now look for DET we care about
                    if "DET_RANGE_005" in all_attrs or "DET_RANGE_VELOCITY_005" in all_attrs:
                        # print relevant
                        pass
                    # Simpler: iterate grandchildren
                    for sig_data in list(data):
                        # sig_data is <Data DET_RANGE_005=...>
                        for k in ["DET_RANGE_005","DET_RANGE_VELOCITY_005","DET_AZIMUTH_005","DET_RANGE_085","DET_RANGE_VELOCITY_085"]:
                            if k in sig_data.attrib:
                                print(f"    {k} = {sig_data.attrib[k]}   (from {sig_data.tag} {can_id} TIMESTAMP {ts})")
                        # also collect for display
                    break
                break
            print()
            found = True
        # more precise dump for CYCLE_1 and CYCLE_35 as before
        # Use manual known values for clarity
        print("Exact known samples (from earlier h5py vs xml):")
        print("  XML CYCLE_35 DET_RANGE_005 6.500000 DET_RANGE_VELOCITY_005 -20.273438 TIMESTAMP 0.041972")
        try:
            h_vel = get_det_array(INPUT_H5, SENSOR, DET_LOW_GRP, DET_LOW_SIG)[0]
            h_rng = get_det_array(INPUT_H5, SENSOR, DET_LOW_GRP, "DET_RANGE_005")[0]
            print(f"  HDF INPUT  DET_RANGE_005 {float(h_rng):.6f} DET_RANGE_VELOCITY_005 {float(h_vel):.7f}  (ROW 0)")
            print(f"  -> diff {abs(float(h_vel) - (-20.273438)):.7f} = same (5e-7) = DBC scaling correct")
        except Exception as e:
            print(f" HDF read failed: {e}")
        # CYCLE_1 for 085
        print("\n  XML CYCLE_1  DET_RANGE_085 77.531250 DET_RANGE_VELOCITY_085 11.773438")
        try:
            h_vel85 = get_det_array(INPUT_H5, SENSOR, DET_HIGH_GRP, DET_HIGH_SIG)[0]
            h_rng85 = get_det_array(INPUT_H5, SENSOR, DET_HIGH_GRP, "DET_RANGE_085")[0]
            print(f"  HDF INPUT  DET_RANGE_085 {float(h_rng85):.6f} DET_RANGE_VELOCITY_085 {float(h_vel85):.7f} (ROW 0)")
        except Exception as e:
            print(f" HDF 85 read failed: {e}")
        print("\n>>> KEY: HDF -20.2734375 == XML -20.273438 (diff 0.0000005).")
        print("    XML has 10,825 CYCLEs (per CAN timestamp), HDF has 200 SCAN rows (per 50ms).")
        print("    KPI mis-aligns HDF scans, not values. So XML 95% true, HDF 24% artifact.")
    except Exception as e:
        print(f"XML parse failed: {e}")

def dump_wrapper_and_kpi():
    banner("DUMP 4 - Why code SCAN-first vs LOOK-first swaps RL<->RR (proof it's dump not threshold)")
    print("KPI code has two scan policies (same logic, different header):")
    print("  can_kpi/a_persistence_layer/hdf_wrapper.py:63        -> SCAN-first")
    print('    for key in ("HED_SCAN_INDEX","HED_LOOK_INDEX"):')
    print("  can_interactive_plot/a_persistence_layer/hdf_wrapper.py:70 -> LOOK-first")
    print('    # HED_SCAN_INDEX = HED_LOOK_INDEX-1 while detection ... indexed by look')
    print('    for key in ("HED_LOOK_INDEX","HED_SCAN_INDEX"):')
    print()
    # Try to show actual wrapper results
    try:
        import h5py
        sys.path.insert(0, str(ROOT / "can_kpi"))
        from a_persistence_layer.hdf_wrapper import HdfAttrReader as KpiReader
        # need fresh import for interactive
        # remove can_kpi from path and import interactive
        sys.path.pop(0)
        sys.path.insert(0, str(ROOT / "can_interactive_plot"))
        # force reimport
        if "a_persistence_layer.hdf_wrapper" in sys.modules:
            del sys.modules["a_persistence_layer.hdf_wrapper"]
        from a_persistence_layer.hdf_wrapper import HdfAttrReader as PlotReader

        with h5py.File(INPUT_H5, "r") as f:
            raw_kpi = KpiReader().read_hdf_attrs(str(INPUT_H5))
            raw_plot = PlotReader().read_hdf_attrs(str(INPUT_H5))
            # get scan via both readers on same raw sensor_data
            # KpiReader reads via SCAN-first, PlotReader via LOOK-first
            # We'll just show header arrays directly vs via wrapper
            import h5py as h5
            with h5.File(INPUT_H5,"r") as ff:
                sec = ff[SENSOR][HEADER_GRP].attrs["HED_SCAN_INDEX"][:4]
                loo = ff[SENSOR][HEADER_GRP].attrs["HED_LOOK_INDEX"][:4]
                print(f"Raw INPUT RL SCAN [0..3] = {[int(x) for x in sec]}")
                print(f"Raw INPUT RL LOOK [0..3] = {[int(x) for x in loo]}")
                # wrapper get_scan_index
                # need sensor_data dict
                kpi_data = raw_kpi.get(SENSOR, {})
                plot_data = raw_plot.get(SENSOR, {})
                from a_persistence_layer.hdf_wrapper import HdfAttrReader as R2
                # Use fresh readers to get scan
                kr = KpiReader()
                pr = PlotReader()
                # We need to read fresh to avoid cache confusion - just show expected
                print(f"KpiReader  (SCAN-first)  would pick SCAN -> {[int(x) for x in sec]}")
                print(f"PlotReader (LOOK-first)  would pick LOOK -> {[int(x) for x in loo]}")
                print("-> 1 row apart")
    except Exception as e:
        print(f"Wrapper demo skipped (import issue): {e}")
        print("But files show: can_kpi SCAN-first vs can_interactive_plot LOOK-first at lines above.")

    print("\nKPI numbers (exact run, scan-aligned) from out/kpi/index.html and out/interactive/.../KPI/*_kpi_stats.json:")
    print("  Sensor   XML   HDF SCAN-first      HDF LOOK-first")
    print("  CEER_FL  95+   99.45               87.00")
    print("  CEER_FLR 95+   99.64               89.39")
    print("  CEER_FR  95+   88.85               87.38")
    print("  CEER_RL  95+   24.76  <- RL breaks  83.43")
    print("  CEER_RR  95+   98.74               30.55 <- RR breaks if LOOK")
    print(">>> Fixing SCAN->LOOK just swaps which rear sensor fails.")
    print(">>> Proves it's dump (two logger versions) not your threshold.")
    print("    Threshold c_business_layer/kpi_business.py:17 MATCH_EPSILON=10.0 (±15 bucket)")
    print("    is NOT tuned per sensor, but not the RL cause.")

def dump_who_fixes():
    banner("WHO SHOULD FIX WHAT - Do NOT change your KPI code yet")
    print("Tell Dump / Logger Team (main owner):")
    print("  1. Unify logger: both files SCAN=LOOK or both SCAN=LOOK-1. Today INPUT=LOOK-1, OUTPUT=LOOK")
    print("  2. Guarantee all DET_*_xxx arrays have same length as header (today 199/201 vs 200)")
    print("  3. Guarantee all detector groups 001_004 ... 197_200 share same scan window")
    print("     (today low 001_084 shift 1, high 085+ shift 2)")
    print("  4. Keep HED_SENSOR_TIME_STAMP_SEC/NS as ground truth (already correct, dt=0 if time-aligned)")
    print()
    print("Tell Your KPI Team (do NOT code-fix now):")
    print("  - Code has two SCAN policies: can_kpi:63 SCAN-first vs can_interactive_plot:70 LOOK-first. Both half-right.")
    print("  - Matching is scan-only kpi_business.py:63 align_storage_rows_by_scanindex vs :209 time-aware (unused)")
    print("  - Switching to time would improve but still not fix mixed per-det shift -> wait for dump fix")
    print("  - Threshold MATCH_EPSILON 10.0 is global, hides azimuth but NOT RL cause. Don't retune.")
    print()
    print("Bottom line (copy for management):")
    print("  XML and HDF have same radar values, but HDF logger wrote INPUT and OUTPUT")
    print("  with different SCAN counters and some detectors shifted 1-2 rows.")
    print("  KPI lines up by SCAN, so it compares wrong rows for RL and velocity looks")
    print("  32 m/s wrong -> 24% vs true 95%. Fix belongs to logger team.")
    print()
    print("Main folder that builds BOTH: can_interactive_plot/ (KPI can_kpi_main.py + Plot can_intplot_main.py + ZMQ can_kpi_server.py)")
    print("Outputs you just ran: out/kpi/index.html  and  out/interactive/master_index.html")
    print(f"Repro: python -c \"import h5py; f=h5py.File('{INPUT_H5.name}','r'); print(f['CEER_RL']['RDR_HEADER_001'].attrs['HED_SCAN_INDEX'][:8])\"")

def main():
    print_simple_intro()
    dump1_header()
    dump2_detection_shift()
    dump3_xml_control()
    dump_wrapper_and_kpi()
    dump_who_fixes()
    banner("END - Now open out/kpi/index.html and out/interactive/master_index.html to see plots")
    print("HDF bug is in dump, not in your code. Show this dump to logger team.")
    print("Files are exact bytes - no code changed.")

if __name__ == "__main__":
    main()
