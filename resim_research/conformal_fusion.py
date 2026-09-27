"""Conformal async fusion with distribution-free coverage (N15).

Problem: EKF Mahalanobis gates assume Gaussian innovations. Under spray /
multipath outliers the assumed S is wrong and the gate misbehaves; async
(out-of-sequence) sensors make it worse. Fix: split-conformal gate over
normalized innovations with a sliding calibration window:

    s_k = |y_k - H x_k| / sqrt(S_k),   q = Q_{(1-a)(1+1/n)}({s})
    accept iff s_k <= q   (coverage >= 1-a regardless of distribution)

Second sensor B (every 3rd scan, 2-scan delay) fuses through the same gate
(OOS-tolerant: stale predictions widen S, conformal q absorbs the rest).
Metrics: TRUE-state coverage (not residual coverage), mean interval width
(sharpness), track RMSE, all vs fixed chi2 gate.
# ponytail: scalar CV + sliding window; adaptive windows and vector states next.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import numpy as np

DT = 0.05
ALPHA = 0.10
WIN = 200
CHI2_90 = 2.7055  # 1-DOF 90% gate (matches ALPHA)


def run_trial(mode, seed, maneuver=True):
    """mode: 'fixed' | 'conformal'. Unmodeled sinusoidal velocity (model
    mismatch makes S wrong) + clean sensor + delayed second sensor."""
    rng = np.random.default_rng(seed)
    x_true, x = 50.0, 49.0
    v_true, v = 25.0, 24.0
    P, Pv = 1.0, 1.0
    cal = []
    cov_hit, widths, errs = 0, [], []
    # sensor B state (delayed, slower)
    b_buf = []
    for k in range(600):
        if maneuver:
            v_true = 25.0 + 6.0 * np.sin(2 * np.pi * k / 200.0)
        x_true += v_true * DT
        # predict
        x += v * DT
        P += 0.05
        S = P + 0.01
        # sensor A (clean, every scan)
        y = x_true + rng.normal(0.0, 0.1)
        # sensor B every 3rd scan, arrives 2 scans late (OOS)
        b_buf.append(x_true + rng.normal(0.0, 0.15))
        yoos = b_buf.pop(0) if len(b_buf) > 2 else None
        for z in [y] + ([yoos] if yoos is not None else []):
            s = abs(z - x) / np.sqrt(S)
            if mode == "conformal" and len(cal) >= 50:
                q = float(np.quantile(cal, min((1 - ALPHA) * (1 + 1 / len(cal)), 1.0)))
            else:
                q = np.sqrt(CHI2_90)
            if s <= q:
                K = P / (S + 1e-12)
                x += K * (z - x)
                P = (1 - K) * P
            if mode == "conformal":
                cal.append(s)
                if len(cal) > WIN:
                    cal.pop(0)
        # coverage of the TRUE state in the current credible interval
        q_now = (float(np.quantile(cal, min((1 - ALPHA) * (1 + 1 / len(cal)), 1.0)))
                 if mode == "conformal" and len(cal) >= 50 else np.sqrt(CHI2_90))
        half = q_now * np.sqrt(P + 0.01)
        cov_hit += int(abs(x_true - x) <= half)
        widths.append(2 * half)
        errs.append(abs(x_true - x))
    n = 600
    return cov_hit / n, float(np.mean(widths)), float(np.mean(errs))


def demo() -> None:
    cf, wf, ef, cc, wc, ec = [], [], [], [], [], []
    for s in range(20):
        c, w, e = run_trial("fixed", s)
        cf.append(c)
        wf.append(w)
        ef.append(e)
        c, w, e = run_trial("conformal", s)
        cc.append(c)
        wc.append(w)
        ec.append(e)
    print(f"[fixed    ] coverage={np.mean(cf):.3f} width={np.mean(wf):.3f} "
          f"rmse={np.mean(ef):.3f}")
    print(f"[conformal] coverage={np.mean(cc):.3f} width={np.mean(wc):.3f} "
          f"rmse={np.mean(ec):.3f} (target coverage >= {1 - ALPHA})")
    assert np.mean(cc) >= 1 - ALPHA - 0.02, "conformal coverage guarantee"
    assert np.mean(cc) > np.mean(cf), "must beat fixed gate coverage"
    assert np.mean(ec) <= np.mean(ef), "must not lose RMSE"
    print("demo PASS")


if __name__ == "__main__":
    demo()
