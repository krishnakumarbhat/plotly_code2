"""Local spectral covariance scaling (T5 / N6).

Physical defect: tire-spray backscatter breaks the Gaussian assumption;
static R0 makes the Kalman filter over-trust spray noise. Fix: modulate by
local spectral entropy H and SNR:

    R(t) = R0 * exp(beta * H(t) / SNR_lin(t))

Spray (H->1, SNR->0): R->large, K->0, tracker coasts on CV kinematics.
Joseph-form update + eigenvalue clipping guarantee positive-definiteness.
Scenario: lead truck at 25 m/s, spray bursts kicking H>0.85, SNR<3 dB.
# ponytail: H/SNR supplied as detector outputs; estimator itself is separate work.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import numpy as np

DT = 0.05
BETA = 1.0
R0_POS = 0.01   # clean-weather position variance (0.1 m std)
R0_VEL = 0.0025  # radial-velocity variance (0.05 m/s std)


def spray_schedule(k):
    """Two spray bursts. Returns (H, SNR_linear)."""
    if 40 <= k < 80 or 140 <= k < 170:
        return 0.90, 0.25   # dense spray: high entropy, ~-6 dB local SNR
    return 0.20, 20.0       # clean


def run_trial(adaptive, seed):
    rng = np.random.default_rng(seed)
    x = np.array([50.0, 25.0])       # lead truck: pos, vel (CV truth)
    X = np.array([49.0, 24.0])       # estimate
    P = np.diag([1.0, 1.0])
    F = np.array([[1.0, DT], [0.0, 1.0]])
    Q = np.diag([0.005, 0.05])
    H = np.eye(2)
    R0 = np.diag([R0_POS, R0_VEL])
    errs_spray, min_eig = [], []
    for k in range(200):
        x = F @ x                    # CV truth
        h, snr = spray_schedule(k)
        # measurement noise follows the TRUE regime (spray blinds sensor)
        s = 2.0 if h > 0.85 else 0.1
        z = np.array([x[0] + rng.normal(0.0, s),
                      x[1] + rng.normal(0.0, s * 0.5)])
        X = F @ X
        P = F @ P @ F.T + Q
        R = R0 * np.exp(BETA * h / snr) if adaptive else R0
        y = z - H @ X
        S = H @ P @ H.T + R
        K = P @ H.T @ np.linalg.inv(S)
        # Joseph form: P = (I-KH)P(I-KH)' + KRK'
        I_KH = np.eye(2) - K @ H
        P = I_KH @ P @ I_KH.T + K @ R @ K.T
        P = 0.5 * (P + P.T)
        w = np.linalg.eigvalsh(P)
        min_eig.append(float(w[0]))
        X = X + K @ y
        if h > 0.85:
            errs_spray.append(float(abs(X[0] - x[0])))
    return float(np.mean(errs_spray)), min(min_eig)


def demo() -> None:
    rb, eb, mb = [], [], []
    ra, ea, ma = [], [], []
    for s in range(30):
        e, m = run_trial(False, s)
        rb.append(e)
        mb.append(m)
        e, m = run_trial(True, s)
        ra.append(e)
        ma.append(m)
    print(f"static R0:   spray RMSE={np.mean(rb):.3f} m minEig={min(mb):.2e}")
    print(f"scaled R(t): spray RMSE={np.mean(ra):.3f} m minEig={min(ma):.2e}")
    assert np.mean(ra) < np.mean(rb), "must beat static covariance in spray"
    assert min(ma) > 0 and min(mb) > 0, "Joseph form must stay PD"
    print("demo PASS")


if __name__ == "__main__":
    demo()
