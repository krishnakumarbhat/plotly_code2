"""HDF5 (SENSOR1 streams) -> UDP KPI CSVs + candidate generation (Phase-4 harness).

Fills the locally-missing decoder step (C++ radar_stream_lib has no binary
here): DETECTION/RDD/VSE/CDC streams -> *_UDP_GEN7_{DET,RDD,VSE,CDC}_CORE.csv
with the exact wide-column layout detection_matching_kpi_script.py consumes.

Candidates (the "resim" step): identity golden + range-bias sensitivity probes.
Naming follows the script's version-pair pattern:
    vehicle: <STEM>_UDP_GEN7_DET_CORE.csv
    sim:     <STEM>_r00080104_UDP_GEN7_DET_CORE.csv
(sensor position from filename: '_FC_UDP_' -> front/768 else corner/680.)

Usage:
    python resim_research/hdf5_to_kpi_csv.py <input.h5> <outdir> [bias_m]
Writes vehicle CSVs + identity candidate + biased candidate, log_path.txt,
meta_data.json. Then run:
    python resim_research/kpi/gen7v2_scripts/detection_matching_kpi_script.py \\
        <outdir>/log_path.txt <outdir>/meta_data.json <outdir>
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import h5py
import numpy as np
import pandas as pd

N_RDD = 512
N_FRONT, N_CORNER = 768, 680


def load(h5_path):
    f = h5py.File(h5_path, "r")
    s = f["SENSOR1"]
    return f, s


def det_df(det, n_af):
    n_scan = det["scan_index"].shape[0]
    data = {
        "scan_index": np.asarray(det["scan_index"]).astype(int),
        "num_af_det": np.asarray(det["num_af_det"]).astype(int),
    }
    src = {k: np.asarray(det[k]) for k in
           ("rdd_idx", "ran", "vel", "theta", "phi",
            "f_single_target", "f_superres_target", "f_bistatic")}
    width = src["ran"].shape[1]
    for i in range(n_af):
        for k, v in src.items():
            col = v[:, i] if i < width else np.zeros(n_scan, dtype=v.dtype)
            if k == "rdd_idx":
                col = np.asarray(col, dtype=int)
            data[f"{k}_{i}"] = np.asarray(col)
    return pd.DataFrame(data)


def rdd_df(rdd):
    n_scan = rdd["scan_index"].shape[0]
    data = {
        "scan_index": np.asarray(rdd["scan_index"]).astype(int),
        "rdd1_num_detect": np.asarray(rdd["rdd1_num_detect"]).astype(int),
    }
    src = {k: np.asarray(rdd[k]) for k in
           ("rdd1_rindx", "rdd1_dindx", "rdd2_range", "rdd2_range_rate")}
    width = src["rdd1_rindx"].shape[1]
    for i in range(N_RDD):
        for k, v in src.items():
            col = v[:, i] if i < width else np.zeros(n_scan, dtype=v.dtype)
            if k in ("rdd1_rindx", "rdd1_dindx"):
                col = np.asarray(col, dtype=int)
            data[f"{k}_{i}"] = np.asarray(col)
    return pd.DataFrame(data)


def vse_df(vse):
    return pd.DataFrame({
        "scan_index": np.arange(1, len(vse["veh_speed"]) + 1),
        "veh_speed": np.asarray(vse["veh_speed"]),
    })


def cdc_df(cdc, scans):
    """One row per CDC record carrying its scan index."""
    rows = []
    counts = np.asarray(cdc["num_cdc_records"]).astype(int)
    for si, c in zip(scans, counts):
        rows.extend([int(si)] * max(int(c), 0))
    return pd.DataFrame({"Scan_Index": rows})


def write_set(s, outdir, stem, tag, n_af, bias=0.0):
    """Write one DET/RDD/VSE/CDC csv set. tag='' vehicle, else version tag."""
    det = det_df(s["DETECTION_STREAM"], n_af)
    if bias:
        for c in det.columns:
            if c.startswith("ran_"):
                det[c] = det[c] + bias
    rdd = rdd_df(s["RDD_STREAM"])
    vse = vse_df(s["VSE_STREAM"])
    scans = np.asarray(s["DETECTION_STREAM"]["scan_index"]).astype(int)
    cdc = cdc_df(s["CDC_STREAM"], scans)
    names = {}
    for kind, df in (("DET_CORE", det), ("RDD_CORE", rdd),
                     ("VSE_CORE", vse), ("CDC", cdc)):
        if kind == "CDC":
            fn = f"{stem}{tag}_UDP_CDC.csv" if tag else f"{stem}_UDP_CDC.csv"
        else:
            fn = f"{stem}{tag}_UDP_GEN7_{kind}.csv" if tag else f"{stem}_UDP_GEN7_{kind}.csv"
        p = os.path.join(outdir, fn)
        df.to_csv(p, index=False)
        names[kind] = p
    return names


def main(h5_path, outdir, bias=0.02):
    os.makedirs(outdir, exist_ok=True)
    f, s = load(h5_path)
    raw = os.path.splitext(os.path.basename(h5_path))[0]
    stem = raw + "_FL"  # corner position tag drives 680-wide layout in KPI
    n_af = N_FRONT if "_FC_UDP_" in stem else N_CORNER
    v = write_set(s, outdir, stem, "", n_af)
    c0 = write_set(s, outdir, stem, "_r00080104", n_af)          # identity golden
    c1 = write_set(s, outdir, stem, "_r00080105", n_af, bias=bias)  # bias probe
    with open(os.path.join(outdir, "log_path.txt"), "w") as fh:
        fh.write(outdir + "\n")
    import json
    with open(os.path.join(outdir, "meta_data.json"), "w") as fh:
        json.dump({"Mode": "CDC", "Max_CDC_Records": 5016,
                   "Range_Saturation_Thresh_Front_Radar": 135.0,
                   "Range_Saturation_Thresh_Corner_Radar": 135.0,
                   "SiL_Engine": "local-harness", "SW": "research",
                   "RSP_SiL": "python", "Tracker": "none"}, fh, indent=1)
    f.close()
    print("vehicle:", list(v.values()))
    print("identity:", list(c0.values()))
    print(f"bias(+{bias}m):", list(c1.values()))
    print("OK - run detection_matching_kpi_script.py on", outdir)


if __name__ == "__main__":
    main(sys.argv[1], sys.argv[2],
         float(sys.argv[3]) if len(sys.argv) > 3 else 0.02)
