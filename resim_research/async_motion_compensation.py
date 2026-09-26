"""Asynchronous satellite motion extrapolation (T3 / N4).

Physical defect: independent satellite clocks stamp detections at
t_sat != t_DC. During yaw maneuvers the stale ego frame smears the fused
point cloud (translation v*dt plus rotation-borne lever arm), splitting
cross-sensor tracks. Fix (first-order in dt, exact for constant yaw):

    p_corr = R_z(w*dt) . p_meas - v_ego*dt - 0.5*a_ego*dt^2

with v_ego, a_ego in the satellite-epoch ego frame. Validated against an
exact constant-turn rigid-body reference, not against itself.
# ponytail: constant-turn reference only; clothoid transients need jerk term.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import numpy as np

SIG_POS = 0.05  # sensor position noise per axis (m)


def Rz(phi):
    c, s = np.cos(phi), np.sin(phi)
    return np.array([[c, -s], [s, c]])


def ego_pose(t, v, w):
    """Exact constant-turn ego pose: heading th, position T (world)."""
    th = w * t
    if abs(w) < 1e-9:
        T = np.array([v * t, 0.0])
    else:
        T = np.array([v * np.sin(th) / w, v * (1.0 - np.cos(th)) / w])
    return th, T


def sense(P_world, t_sat, v, w, rng):
    """Perfect-clock... no: stale-clock measurement. Sensor stamps at t_sat
    but fusion treats it at t_DC. Returns ego-frame detection at t_sat."""
    th, T = ego_pose(t_sat, v, w)
    p = Rz(-th) @ (P_world - T) + rng.normal(0.0, SIG_POS, 2)
    return p


def exact_reference(p_meas_nonoise, dt, v, w):
    """Ground-truth ego-frame position at t_DC for a static world point,
    from exact rigid-body kinematics (constant turn)."""
    phi = w * dt
    if abs(w) < 1e-9:
        d_sat = np.array([v * dt, 0.0])
    else:
        d_sat = np.array([v * np.sin(phi) / w, v * (1.0 - np.cos(phi)) / w])
    return Rz(-phi) @ (p_meas_nonoise - d_sat)


def correct_directive(p_meas, dt, v, w, a_long=0.0):
    """Candidate: the directive's first-order formula. v_ego=(v,0) in the
    sat frame; lateral accel is centripetal v*w."""
    phi = w * dt
    v_ego = np.array([v, 0.0])
    a_ego = np.array([a_long, v * w])
    return Rz(phi) @ p_meas - v_ego * dt - 0.5 * a_ego * dt ** 2


def run_case(w, v=25.0, n_posts=8, scans=40, seed=0):
    """Two satellites, independent U[5,45]ms staleness per scan. Static
    guardrail-post world. Metric: RMS error of the fused (mean) detection
    vs exact t_DC ego-frame truth, naive vs corrected."""
    rng = np.random.default_rng(seed)
    errs_naive, errs_corr = [], []
    posts = np.array([[12.0 + 4.0 * i, 3.0 + 0.3 * (i % 2)] for i in range(n_posts)])
    t = 0.0
    for _ in range(scans):
        t += 0.05
        for P in posts:
            th_dc, T_dc = ego_pose(t, v, w)
            truth = Rz(-th_dc) @ (P - T_dc)
            fused_n, fused_c = [], []
            for _ in range(2):  # two satellites, independent clocks
                dt = rng.uniform(0.005, 0.045)
                p = sense(P, t - dt, v, w, rng)
                fused_n.append(p)
                fused_c.append(correct_directive(p, dt, v, w))
            errs_naive.append(float(np.linalg.norm(np.mean(fused_n, axis=0) - truth)))
            errs_corr.append(float(np.linalg.norm(np.mean(fused_c, axis=0) - truth)))
    return float(np.mean(errs_naive)), float(np.mean(errs_corr))


def demo() -> None:
    print(f"{'w(rad/s)':>9} {'naive(m)':>9} {'corr(m)':>9} {'reduction':>9}")
    reds = []
    for w in (0.1, 0.2, 0.3, 0.4, 0.5, 0.6):
        n, c = run_case(w)
        red = 1.0 - c / max(n, 1e-12)
        reds.append(red)
        print(f"{w:9.1f} {n:9.3f} {c:9.3f} {red:8.1%}")
    assert min(reds) >= 0.30, "must cut residuals >=30% over the yaw sweep"
    print("demo PASS")


if __name__ == "__main__":
    demo()
