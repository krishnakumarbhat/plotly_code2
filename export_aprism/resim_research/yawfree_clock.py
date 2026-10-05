"""Yaw-free timestamp + Doppler-bias elimination (N27).

N27.1: r w - d^T v = |v|^2 tau (yaw-free scalar invariant).
N27.2: two-target [a_k, r_k] system separates (tau, b).
N27.3: circular yaw after time correction. N27.5: accel cubic.
# ponytail: numpy only, deterministic fixtures + noise ladder.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import numpy as np


def tau_single(d, v, r, w):
    """N27.1 single-target timestamp estimate."""
    d = np.asarray(d, float)
    v = np.asarray(v, float)
    den = float(v @ v)
    assert den > 0, "v=0 unobservable"
    return float(r * w - d @ v) / den


def tau_bias_2(a1, r1, y1, a2, r2, y2):
    """N27.2 joint (tau, b) from two targets."""
    D = a1 * r2 - a2 * r1
    assert abs(D) > 0, "singular speed^2/range ratios"
    return (y1 * r2 - y2 * r1) / D, (a1 * y2 - a2 * y1) / D


def yaw_estimate(ds, vs, tau, thetas, wts=None):
    """N27.3 circular yaw from time-corrected geometry."""
    n = len(ds)
    wts = np.ones(n) if wts is None else np.asarray(wts, float)
    acc = 0j
    for d, v, th, wt in zip(ds, vs, thetas, wts):
        x = np.asarray(d, float) + np.asarray(v, float) * tau
        acc += wt * complex(x[0], x[1]) / max(float(np.linalg.norm(x)), 1e-12) \
            * np.exp(-1j * th)
    assert abs(acc) > 0, "zero resultant"
    return float(np.angle(acc))


def demo() -> None:
    # 1. exact clock fixture
    d = np.array([10.0, 4.0])
    v = np.array([5.0, -2.0])
    tau = 0.02
    x = d + v * tau
    r = float(np.linalg.norm(x))
    w = float(x @ v) / r
    assert abs(r * w - float(d @ v) - float(v @ v) * tau) < 1e-12, "invariant"
    th = tau_single(d, v, r, w)
    print(f"[clock] tau={th:.12f} (expect 0.02)")
    assert abs(th - 0.02) < 1e-12, "exact recovery"

    # yaw invariance: timing unchanged, yaw recovered mod 2pi
    for deg in (-170.0, -5.0, 0.0, 5.0, 170.0):
        beta = np.deg2rad(deg)
        th_meas = float(np.arctan2(x[1], x[0]) - beta)
        assert abs(tau_single(d, v, r, w) - 0.02) < 1e-12, "yaw-free"
        be = yaw_estimate([d], [v], tau, [th_meas])
        err = (be - beta + np.pi) % (2 * np.pi) - np.pi
        assert abs(err) < 1e-12, f"yaw {deg}"
    print("[yaw] 5 orientations recovered")

    # 2. clock-plus-bias fixture
    x1 = np.array([10.0, 0.0])
    v1 = np.array([5.0, 0.0])
    x2 = np.array([0.0, 20.0])
    v2 = np.array([0.0, 10.0])
    b = 0.1
    d1, d2 = x1 - v1 * tau, x2 - v2 * tau
    r1, r2 = float(np.linalg.norm(x1)), float(np.linalg.norm(x2))
    w1 = float(x1 @ v1) / r1 + b
    w2 = float(x2 @ v2) / r2 + b
    a1, a2 = float(v1 @ v1), float(v2 @ v2)
    y1, y2 = r1 * w1 - float(d1 @ v1), r2 * w2 - float(d2 @ v2)
    D = a1 * r2 - a2 * r1
    print(f"[bias] D={D:.1f} (expect -500)")
    assert abs(D + 500.0) < 1e-9, "determinant"
    te, be = tau_bias_2(a1, r1, y1, a2, r2, y2)
    print(f"[bias] tau={te:.12f} b={be:.12f}")
    assert abs(te - 0.02) < 1e-12 and abs(be - 0.1) < 1e-12, "joint recovery"

    # 3. accel cubic (N27.5) numeric check
    a = np.array([0.0, 3.0])
    T = 0.045
    x = d + v * T + 0.5 * a * T ** 2
    vv = v + a * T
    r = float(np.linalg.norm(x))
    w = float(x @ vv) / r
    lhs = r * w - float(d @ v)
    c1 = float(v @ v) + float(d @ a)
    rhs = c1 * T + 1.5 * float(v @ a) * T ** 2 + 0.5 * float(a @ a) * T ** 3
    print(f"[accel] lhs={lhs:.9f} rhs={rhs:.9f}")
    assert abs(lhs - rhs) < 1e-9, "cubic identity"
    rem = (1.5 * abs(float(v @ a)) * T ** 2 + 0.5 * float(a @ a) * T ** 3) / abs(c1)
    print(f"[accel] linear-truncation bound={rem * 1e3:.4f} ms")
    assert rem < 5e-3, "remainder budget"

    # 4. noise ladder (tangential case included)
    rng = np.random.default_rng(2)
    for cfg, dd, vv in (("radial", np.array([20.0, 0.0]), np.array([6.0, 0.5])),
                        ("tangential", np.array([20.0, 0.0]), np.array([0.0, 8.0]))):
        errs = []
        for _ in range(2000):
            dn = dd + rng.normal(0, 0.01, 2)
            vn = vv + rng.normal(0, 0.01, 2)
            xx = dd + vv * tau
            rr = float(np.linalg.norm(xx)) + rng.normal(0, 0.01)
            ww = float(xx @ vv) / float(np.linalg.norm(xx)) + rng.normal(0, 0.005)
            errs.append(tau_single(dn, vn, rr, ww) - tau)
        errs = np.array(errs)
        print(f"[noise-{cfg}] rmse={float(np.sqrt((errs**2).mean()))*1e3:.3f} ms "
              f"bias={float(errs.mean())*1e3:.3f} ms")
        assert float(np.sqrt((errs ** 2).mean())) < 0.020, f"rmse {cfg}"
    print("demo PASS")


if __name__ == "__main__":
    demo()
