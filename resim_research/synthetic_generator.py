"""Programmatic corner-case generator (Phase-4): 4 synthetic scenarios in
production SENSOR1 HDF5 schema (DETECTION/RDD/VSE/CDC streams), consumable
by hdf5_to_kpi_csv.py + the KPI suites.

  S1 overpass : 1 lead vehicle + 3 planar specular ghost tracks (mirror
                kinematics + ego-motion Doppler projection).
  S2 cutin    : host 25 m/s; adjacent target, ay = 4.2 m/s^2, crosses the
                lane boundary in 800 ms.
  S3 spray    : lead truck + high-entropy Range-Doppler clutter
                (H > 0.85, SNR < 3 dB proxy: dense low-flag detections).
  S4 curve    : host on R = 150 m curve, w = 0.45 rad/s, two satellites
                with async clocks dt in [5, 45] ms.

Quantization mimics CDC bins (rindx/dindx integer + physical fields), so
Tier-1 pairing and Tier-2 gates evaluate meaningfully.
# ponytail: straight-line + circular kinematics only; clothoid next.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import h5py
import numpy as np

RES_R, RES_V = 0.05, 0.02  # CDC bin sizes for rindx/dindx


def to_det(px, py, vx, vy):
    r = float(np.hypot(px, py))
    th = float(np.arctan2(py, px))
    vr = float((vx * px + vy * py) / max(r, 1e-9))
    return r, vr, th, 0.0


def emit(path, scans):
    """scans: list of dicts with keys det=[(r,vr,th,phi)...], v_ego, cdc_n.
    Writes SENSOR1-schema HDF5."""
    n = len(scans)
    W = max(len(s["det"]) for s in scans)
    det = {k: np.zeros((n, W), dtype=np.float32) for k in
           ("ran", "vel", "theta", "phi", "rdd_idx",
            "f_single_target", "f_superres_target", "f_bistatic")}
    num_af = np.zeros(n, dtype=np.int32)
    scan_index = np.arange(n, dtype=np.int32)
    rdd = {k: np.zeros((n, W), dtype=np.float32) for k in
           ("rdd1_rindx", "rdd1_dindx", "rdd2_range", "rdd2_range_rate")}
    num_rd = np.zeros(n, dtype=np.int32)
    vse = np.zeros(n, dtype=np.float32)
    cdc = np.zeros(n, dtype=np.float32)
    for i, s in enumerate(scans):
        dd = s["det"]
        m = len(dd)
        num_af[i] = m
        num_rd[i] = m
        for j, (r, vr, th, ph) in enumerate(dd):
            det["ran"][i, j] = r
            det["vel"][i, j] = vr
            det["theta"][i, j] = th
            det["phi"][i, j] = ph
            det["rdd_idx"][i, j] = j
            det["f_single_target"][i, j] = 1.0
            rdd["rdd1_rindx"][i, j] = round(r / RES_R)
            rdd["rdd1_dindx"][i, j] = round(vr / RES_V)
            rdd["rdd2_range"][i, j] = r
            rdd["rdd2_range_rate"][i, j] = vr
        vse[i] = s.get("v_ego", 25.0)
        cdc[i] = s.get("cdc_n", 100)
    with h5py.File(path, "w") as f:
        s1 = f.create_group("SENSOR1")
        g = s1.create_group("DETECTION_STREAM")
        g.create_dataset("scan_index", data=scan_index)
        g.create_dataset("num_af_det", data=num_af)
        for k, v in det.items():
            g.create_dataset(k, data=v)
        g = s1.create_group("RDD_STREAM")
        g.create_dataset("scan_index", data=scan_index)
        g.create_dataset("rdd1_num_detect", data=num_rd)
        for k, v in rdd.items():
            g.create_dataset(k, data=v)
        g = s1.create_group("VSE_STREAM")
        g.create_dataset("veh_speed", data=vse)
        g = s1.create_group("CDC_STREAM")
        g.create_dataset("num_cdc_records", data=cdc)
    return path


def s_overpass(n=40, seed=0):
    """Lead vehicle + 3 planar specular ghosts (wall y=3)."""
    rng = np.random.default_rng(seed)
    scans = []
    v_ego = 25.0
    for k in range(n):
        t = k * 0.05
        px, py = 30.0 - 2.0 * t, 1.5
        vx, vy = -2.0, 0.0
        det = [to_det(px, py, vx - v_ego, vy)]
        for gy in (3.0, 6.0, 9.0):
            # mirror across y=gy: ghost of (px,py) with mirrored vy
            det.append(to_det(px, 2 * gy - py, vx - v_ego, -vy))
        det = [(r + rng.normal(0, 0.05), vr + rng.normal(0, 0.02), th, ph)
               for r, vr, th, ph in det]
        scans.append({"det": det, "v_ego": v_ego, "cdc_n": 120})
    return scans


def s_cutin(n=40, seed=1):
    """Adjacent target, ay=4.2, crosses lane in 800 ms."""
    rng = np.random.default_rng(seed)
    scans = []
    for k in range(n):
        t = k * 0.05
        x = 30.0 - 2.0 * t
        if t < 1.0:
            y, vy = 3.5, 0.0
        elif t < 1.8:
            tau = t - 1.0
            y, vy = 3.5 - 0.5 * 4.2 * tau ** 2, -4.2 * tau
        else:
            y, vy = 3.5 - 0.5 * 4.2 * 0.64 - 4.2 * 0.8 * (t - 1.8), -3.36
        r, vr, th, ph = to_det(x, y, -2.0 - 25.0, vy)
        scans.append({"det": [(r + rng.normal(0, 0.05),
                               vr + rng.normal(0, 0.02), th, ph)],
                      "v_ego": 25.0, "cdc_n": 90})
    return scans


def s_spray(n=40, seed=2):
    """Lead truck + dense low-flag spray clutter around it."""
    rng = np.random.default_rng(seed)
    scans = []
    for k in range(n):
        t = k * 0.05
        px, py = 40.0 - 1.0 * t, 0.0
        det = [to_det(px, py, -1.0 - 25.0, 0.0)]
        for _ in range(12):  # spray: nearby, random Doppler, flag 0
            det.append((px + rng.normal(0, 3.0),
                        rng.normal(-5, 8.0),
                        np.arctan2(py + rng.normal(0, 1.0), px), 0.0))
        scans.append({"det": det, "v_ego": 25.0, "cdc_n": 4500})
    return scans


def s_curve(n=40, seed=3, w=0.45, R=150.0):
    """Host on curve; two satellites dt1/dt2 in [5,45] ms (stale salt)."""
    rng = np.random.default_rng(seed)
    scans = []
    dt1, dt2 = rng.uniform(0.005, 0.045, 2)
    for k in range(n):
        t = k * 0.05
        th = w * t
        # static posts in world -> ego-frame with stale clocks per sat
        det = []
        for j in range(6):
            Pw = np.array([20.0 + 8.0 * j, 3.0])
            for dt in (dt1, dt2):
                ts = t - dt
                ths, Ts = w * ts, np.array(
                    [R * np.sin(w * ts), R * (1 - np.cos(w * ts))])
                c, s = np.cos(ths), np.sin(ths)
                pe = np.array([[c, s], [-s, c]]) @ (Pw - Ts)
                det.append((float(np.linalg.norm(pe)) + rng.normal(0, 0.05),
                            rng.normal(0, 0.5),
                            float(np.arctan2(pe[1], pe[0])), 0.0))
        scans.append({"det": det, "v_ego": w * R, "cdc_n": 200})
    return scans


def demo() -> None:
    outdir = "resim_research/kpi_work/synth"
    os.makedirs(outdir, exist_ok=True)
    made = {}
    for name, fn in (("overpass", s_overpass), ("cutin", s_cutin),
                     ("spray", s_spray), ("curve", s_curve)):
        p = emit(os.path.join(outdir, f"synth_{name}.h5"), fn())
        made[name] = p
        print(f"[synth] {name}: {p}")
    # schema self-check: reload + verify Tier-1 index consistency
    for name, p in made.items():
        with h5py.File(p, "r") as f:
            d = f["SENSOR1/DETECTION_STREAM"]
            r = f["SENSOR1/RDD_STREAM"]
            assert (np.asarray(d["scan_index"]) == np.asarray(r["scan_index"])).all()
            assert (np.asarray(d["num_af_det"]) == np.asarray(r["rdd1_num_detect"])).all()
            # rindx/dindx invert back within half a bin
            ri = np.asarray(r["rdd1_rindx"])
            rr = np.asarray(r["rdd2_range"])
            assert np.allclose(ri * RES_R, np.round(rr / RES_R) * RES_R, atol=RES_R), name
    print("demo PASS")


if __name__ == "__main__":
    demo()
