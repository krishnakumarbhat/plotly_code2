"""Candidate HDF5 -> output MF4 wrapper (Phase-4 harness).

The production SiL (APT_SRR_RESIM.exe) is cluster-only (binaries via jfrog,
no local exe), so this wraps OUR Python-candidate detection streams into a
testable output .mf4 with asammdf: one channel group per SENSOR stream,
scan time (scan_index * 50 ms) as master, active detections as channels.

Usage:
    python resim_research/candidate_mf4.py <candidate.h5> <out.mf4>
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import h5py
import numpy as np
from asammdf import MDF, Signal

CYCLE = 0.05


def main(h5_path, out_path):
    f = h5py.File(h5_path, "r")
    s = f["SENSOR1"]
    det = s["DETECTION_STREAM"]
    rdd = s["RDD_STREAM"]
    scan = np.asarray(det["scan_index"]).astype(int)
    n = len(scan)
    t = (scan - scan[0]) * CYCLE
    n_af = np.asarray(det["num_af_det"]).astype(int)
    width = np.asarray(det["ran"]).shape[1]
    sigs = [
        Signal(np.asarray(det["scan_index"]), t, name="scan_index"),
        Signal(n_af, t, name="num_af_det"),
        Signal(np.asarray(rdd["rdd1_num_detect"]).astype(int), t,
               name="rdd1_num_detect"),
    ]
    for k in ("ran", "vel", "theta", "phi", "rdd_idx",
              "f_single_target", "f_superres_target", "f_bistatic",
              "rdd1_rindx", "rdd1_dindx", "rdd2_range", "rdd2_range_rate"):
        src = det if k in det else rdd
        arr = np.asarray(src[k])
        for i in range(min(width, arr.shape[1])):
            sigs.append(Signal(np.asarray(arr[:, i]), t, name=f"{k}_{i}"))
    try:
        vse = np.asarray(s["VSE_STREAM"]["veh_speed"])
        sigs.append(Signal(vse[:n], t, name="veh_speed"))
    except KeyError:
        pass
    mdf = MDF()
    mdf.append(sigs, comment="resim candidate: T-harness output")
    mdf.save(out_path, overwrite=True)
    f.close()
    nch = sum(len(g.channels) for g in MDF(out_path).groups)
    print(f"wrote {out_path}: {n} scans, {nch} channels")
    print("verify: reopen + compare ran_0 against HDF5")


if __name__ == "__main__":
    main(sys.argv[1], sys.argv[2])
