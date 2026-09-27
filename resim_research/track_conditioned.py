"""Track-conditioned residual Doppler estimator, INT8 TinyML head (N14).

Idea: the tracker already predicts (r, v). Remove the predicted beat
before spectral estimation; the residual is a near-DC tone carrying ONLY
the prediction error, estimable with a 1-lag autocorrelation phase
(2N complex mults) instead of a full N-FFT (N log2 N). Then quantize the
residual path to INT8 (TinyML envelope: <50 KB, no FPU needed).

    s[n] = A e^{j(2 pi f_b n Ts + phi)} + w[n],  f_b = 2Sr/c + 2v/lam
    z[n] = s[n] e^{-j 2 pi f_pred n Ts}  -> residual tone f_b - f_pred
    f_res = angle( sum_n z[n] conj(z[n-1]) ) / (2 pi Ts)   (lag-1)

Demo: accuracy parity (residual vs full FFT) over tracker-error sweep +
INT8 degradation + flop ratio. Cold-start loop (no track yet) is a
documented limitation, not modeled here.
# ponytail: single chirp, single target; multi-target needs CLEAN first.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import numpy as np

C = 3e8
FC = 77e9
LAM = C / FC
SLOPE = 5e12            # 5 MHz/us
N = 512
TS = 100e-9             # fs = 10 MHz; beat <= 2 MHz, no aliasing
K_R = 2.0 * SLOPE / C   # beat Hz per meter of range


def beat(r, v, rng, snr_db=10.0):
    """Beat samples for a point target + complex noise. Single chirp
    resolves range (Doppler term kept for physical completeness)."""
    fb = K_R * r + 2.0 * v / LAM
    n = np.arange(N)
    s = np.exp(1j * (2 * np.pi * fb * n * TS + rng.uniform(0, 2 * np.pi)))
    sig, noise = 1.0, 10.0 ** (-snr_db / 20.0)
    s = sig * s + noise * (rng.normal(size=N) + 1j * rng.normal(size=N)) / np.sqrt(2)
    return s, fb


def full_fft(s):
    """Baseline: full N-FFT + parabolic peak (N log2 N complex mults)."""
    spec = np.abs(np.fft.fft(s))
    k = int(np.argmax(spec))
    a, b, c = spec[k - 1], spec[k], spec[(k + 1) % N]
    d = 0.5 * (a - c) / max(a - 2 * b + c, 1e-12)
    d = min(max(d, -0.5), 0.5)
    return (k + d) / (N * TS), N * np.log2(N)


def residual_lag1(s, f_pred):
    """Proposed: de-rotate by tracker prediction, lag-1 phase (2N mults)."""
    n = np.arange(N)
    z = s * np.exp(-1j * 2 * np.pi * f_pred * n * TS)
    f_res = np.angle(np.sum(z[1:] * np.conj(z[:-1]))) / (2 * np.pi * TS)
    return f_pred + f_res, 2.0 * N


def quantize8(x):
    """Symmetric INT8 fake-quantization, real/imag separately."""
    xr = np.asarray(np.real(x), dtype=float)
    xi = np.asarray(np.imag(x), dtype=float)
    mr = max(float(np.max(np.abs(xr))), 1e-12)
    mi = max(float(np.max(np.abs(xi))), 1e-12)
    qr = np.round(xr / mr * 127.0) / 127.0 * mr
    qi = np.round(xi / mi * 127.0) / 127.0 * mi
    return qr + 1j * qi


def residual_lag1_int8(s, f_pred):
    """INT8 residual path: quantized mix + quantized accumulation."""
    n = np.arange(N)
    mix = quantize8(np.exp(-1j * 2 * np.pi * f_pred * n * TS))
    z = quantize8(s) * mix
    zq = quantize8(z)
    acc = np.sum(zq[1:] * np.conj(zq[:-1]))
    return f_pred + float(np.angle(acc)) / (2 * np.pi * TS)


def f_to_r(f, v):
    return (f - 2.0 * v / LAM) / K_R


def demo() -> None:
    rng = np.random.default_rng(0)
    errs_full, errs_res, errs_q = [], [], []
    for t in range(60):
        r = rng.uniform(10.0, 60.0)
        v = rng.uniform(-15.0, 15.0)
        s, fb = beat(r, v, rng)
        # tracker prediction with realistic error (0.3 m, 0.5 m/s)
        rp = r + rng.normal(0.0, 0.3)
        vp = v + rng.normal(0.0, 0.5)
        f_pred = K_R * rp + 2.0 * vp / LAM
        f1, c1 = full_fft(s)
        f2, c2 = residual_lag1(s, f_pred)
        f3 = residual_lag1_int8(s, f_pred)
        errs_full.append(abs(f_to_r(f1, v) - r))
        errs_res.append(abs(f_to_r(f2, v) - r))
        errs_q.append(abs(f_to_r(f3, v) - r))
    m1, m2, m3 = (float(np.mean(errs_full)), float(np.mean(errs_res)),
                  float(np.mean(errs_q)))
    print(f"[acc] full-FFT range RMSE={m1:.4f} m")
    print(f"[acc] residual  range RMSE={m2:.4f} m (ratio {m2/max(m1,1e-12):.2f}x)")
    print(f"[acc] INT8      range RMSE={m3:.4f} m (ratio {m3/max(m1,1e-12):.2f}x)")
    print(f"[cost] full={c1:.0f} vs residual={c2:.0f} cplx mults "
          f"({c1/c2:.1f}x fewer)")
    assert m2 <= 1.2 * m1, "residual must match full-FFT accuracy"
    assert m3 <= 1.5 * m1, "INT8 must stay within 50% of float"
    assert c1 / c2 >= 4.0, "must cut DSP cost >= 4x"
    print("demo PASS")


if __name__ == "__main__":
    demo()
