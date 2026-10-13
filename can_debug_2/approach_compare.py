#!/usr/bin/env python3
r"""Compare 4 alignment approaches on PT048 RL + CCA regression.
Run: python approach_compare.py
"""
import h5py
import numpy as np
from pathlib import Path

ROOT = Path(__file__).resolve().parent
IN_H5 = ROOT / "PCAN/INPUT/CEER_PT048_20260814_121515_0338_b04_HDF.h5"
OUT_H5 = ROOT / "PCAN/OUTPUT/CEER_PT048_20260814_121515_0338_b04_rR00110006_HDF.h5"
CCA_IN = Path(r"C:\Users\ouymc2\Desktop\plotly_code2\edge_hdf\can\input\CCA_9010_3100099_BUS_20260119_120641_0000_rR00080104_HDF.h5")
CCA_OUT = Path(r"C:\Users\ouymc2\Desktop\plotly_code2\edge_hdf\can\output\CCA_9010_3100099_BUS_20260119_120641_0000_HDF.h5")

SENSOR = "CEER_RL"
HDR = "RDR_HEADER_001"
DET_GRP = "RDR_DETECTION_005_008"
VEL = "DET_RANGE_VELOCITY_005"
RNG = "DET_RANGE_005"
EPS = 10.0
HDR_TOL_NS = 2_000_000
CAN_TOL_S = 0.002


def q(v):
    return int(round(float(v) / EPS))


def hit(a, b):
    return abs(q(a) - q(b)) <= 1


def load_side(h5_path, sensor, hdr, det_grp, vel, rng):
    with h5py.File(h5_path, "r") as f:
        ha = f[sensor][hdr].attrs
        da = f[sensor][det_grp].attrs
        scan = np.asarray(ha["HED_SCAN_INDEX"]).astype(int)
        look = np.asarray(ha["HED_LOOK_INDEX"]).astype(int)
        sec = np.asarray(ha["HED_SENSOR_TIME_STAMP_SEC"], dtype=np.float64)
        ns = np.asarray(ha["HED_SENSOR_TIME_STAMP_NS"], dtype=np.float64)
        t_ns = np.rint(sec).astype(np.int64) * 1_000_000_000 + np.clip(np.rint(ns).astype(np.int64), 0, None)
        hdr_can = np.asarray(ha["timestamp_" + hdr], dtype=np.float64)
        det_can = np.asarray(da["timestamp_" + det_grp], dtype=np.float64)
        v = np.asarray(da[vel], dtype=np.float64)
        r = np.asarray(da[rng], dtype=np.float64)
        nvalid = np.asarray(ha["HED_NUM_OF_VALID_DETECTIONS"]).astype(int) if "HED_NUM_OF_VALID_DETECTIONS" in ha else np.full(len(scan), -1)
    return dict(scan=scan, look=look, t_ns=t_ns, hdr_can=hdr_can,
                det_can=det_can, vel=v, rng=r, nvalid=nvalid)


def pair_scan_only(a, b):
    out_pos = {}
    for j, s in enumerate(b["scan"]):
        out_pos.setdefault(int(s), []).append(j)
    pairs = []
    for i, s in enumerate(a["scan"]):
        lst = out_pos.get(int(s))
        if lst:
            pairs.append((i, lst.pop(0)))
    return pairs


def score_vel(pairs, a, b):
    h = sum(1 for i, j in pairs if hit(a["vel"][i], b["vel"][j]))
    return h, len(pairs), (100.0 * h / len(pairs) if pairs else 0.0)


def run_pt048():
    a = load_side(IN_H5, SENSOR, HDR, DET_GRP, VEL, RNG)
    b = load_side(OUT_H5, SENSOR, HDR, DET_GRP, VEL, RNG)
    print("=" * 78)
    print(f"PT048 {SENSOR} {DET_GRP}/{VEL}: N_in={len(a['scan'])} N_out={len(b['scan'])}")
    print(f"IN SCAN[:4]={a['scan'][:4].tolist()} LOOK[:4]={a['look'][:4].tolist()}")
    print(f"OUT SCAN[:4]={b['scan'][:4].tolist()} LOOK[:4]={b['look'][:4].tolist()}")
    print(f"hdr_can IN[:4]={np.round(a['hdr_can'][:4],4).tolist()} OUT[:4]={np.round(b['hdr_can'][:4],4).tolist()}")
    print(f"det_can IN[:4]={np.round(a['det_can'][:4],4).tolist()} OUT[:4]={np.round(b['det_can'][:4],4).tolist()}")
    print(f"nvalid zeros: IN={(a['nvalid']==0).sum()} OUT={(b['nvalid']==0).sum()} (header-only rows)")

    # A: scan-only baseline
    pa = pair_scan_only(a, b)
    ha, na, ra = score_vel(pa, a, b)
    print(f"\nA SCAN-only: pairs={na} hits={ha} acc={ra:.2f}%  (current code)")

    # B: SCAN + header-time gate (HED ns + hdr CAN), skip header-only
    pb = []
    for i, j in pa:
        if a["nvalid"][i] == 0 or b["nvalid"][j] == 0:
            continue
        if abs(int(a["t_ns"][i]) - int(b["t_ns"][j])) > HDR_TOL_NS:
            continue
        if abs(float(a["hdr_can"][i]) - float(b["hdr_can"][j])) > CAN_TOL_S:
            continue
        pb.append((i, j))
    hb, nb, rb = score_vel(pb, a, b)
    print(f"B SCAN+hdrTIME gate: pairs={nb} (dropped {na-nb}) hits={hb} acc={rb:.2f}%")

    # C: B + fixed global DET shift n in [-5..5]
    print("\nC B + fixed global DET-row shift n (OUT[j] vs IN[i+n]):")
    best = None
    for n in range(-5, 6):
        pairs = []
        for i, j in pb:
            ii = i + n
            if 0 <= ii < len(a["vel"]):
                pairs.append((ii, j))
        h, nn, r = score_vel(pairs, a, b)
        flag = ""
        if best is None or r > best[2]:
            best = (n, nn, r, h)
            flag = "  <-- best"
        print(f"  n={n:+d}: pairs={nn} hits={h} acc={r:.2f}%{flag}")
    print(f"C best fixed n={best[0]:+d} acc={best[2]:.2f}%")

    # D: per-side DET-timestamp O(1) hash: OUT[j] -> IN row with closest det_can within tol + scan window +-5
    det_q_in = np.round(a["det_can"] * 1000).astype(np.int64)
    det_map = {}
    for i, qq in enumerate(det_q_in):
        det_map.setdefault(int(qq), []).append(i)
    pd = []
    det_tol_q = int(CAN_TOL_S * 1000)
    for i, j in pb:
        qq = int(round(float(b["det_can"][j]) * 1000))
        cand = []
        for dq in range(-det_tol_q, det_tol_q + 1):
            cand.extend(det_map.get(qq + dq, []))
        # restrict to scan window +-5 around i
        cand = [c for c in cand if abs(c - i) <= 5]
        if cand:
            # pick closest det_can, tie -> closest to i
            cc = min(cand, key=lambda c: (abs(float(a["det_can"][c]) - float(b["det_can"][j])), abs(c - i)))
            pd.append((cc, j))
        else:
            pd.append((i, j))
    hd, nd, rd = score_vel(pd, a, b)
    shifts = [i - i0 for (i, j), (i0, j0) in zip(pd, pb)]
    import collections
    print(f"\nD DET-time O(1) hash + scan-window5: pairs={nd} hits={hd} acc={rd:.2f}%")
    print(f"  applied shift distribution (chosen_i - scan_i): {dict(sorted(collections.Counter(shifts).items()))}")
    return a, b


def run_cca():
    print("\n" + "=" * 78)
    print("CCA regression (old logger, LOOK=0, SCAN must stay working)")
    with h5py.File(CCA_IN, "r") as f:
        ha = f["CEER_FL"]["SRR_FL_HEADER_001"].attrs
        print(f"CCA IN SCAN[:4]={[int(x) for x in ha['HED_SCAN_INDEX'][:4]]} LOOK[:4]={[int(x) for x in ha['HED_LOOK_INDEX'][:4]]}")
    with h5py.File(CCA_OUT, "r") as f:
        ha = f["CEER_FL"]["SRR_FL_HEADER_001"].attrs
        print(f"CCA OUT SCAN[:4]={[int(x) for x in ha['HED_SCAN_INDEX'][:4]]} LOOK[:4]={[int(x) for x in ha['HED_LOOK_INDEX'][:4]]}")
    print("Fallback chain required: SCAN-only must still produce pairs when LOOK=0 / hdr_can missing.")


if __name__ == "__main__":
    run_pt048()
    run_cca()
