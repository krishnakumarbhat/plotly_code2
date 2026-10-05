"""Three-product cubic-exact sub-chirp residual tracker (N23).

Four ADC samples at t_j=(j-3/2)h -> exactly 3 lag products C_j;
predicted phase subtracted AFTER arg; cubic-annihilating stencil
f=(26e0-e+-e-)/24h, g=(e+-e-)/2h^2, l=(e+-2e0+e-)/h^3.
# ponytail: one-tone prototype; instrumented product counter.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import numpy as np

C = 3e8
FC = 77e9
LAM = C / FC
SLOPE = 5e12
K_R = 2.0 * SLOPE / C  # 33333.33 Hz/m


class Counter:
    """Instrumented complex multiplier (N23 recipe step 4)."""

    def __init__(self):
        self.n = 0

    def prod(self, a, b):
        self.n += 1
        return a * np.conj(b)

    def reset(self):
        self.n = 0


def estimate(x, pred_incr, counter=None):
    """Residual frequency from 4 samples. Returns (f_hat_cyc_per_h, invalid).

    x: 4 complex samples; pred_incr: 3 predicted increments (cycles).
    """
    x = np.asarray(x, dtype=complex)
    if np.max(np.abs(x)) == 0 or pred_incr is None:
        return None, True
    counter = counter or Counter()
    eta = np.empty(3)
    for j in range(3):
        Cj = counter.prod(x[j + 1], x[j])
        meas = np.angle(Cj) / (2.0 * np.pi)  # cycles in [-0.5, 0.5)
        d = meas - pred_incr[j]
        eta[j] = d - np.round(d)  # wrap to [-0.5, 0.5)
    em, e0, ep = eta
    fh = (26.0 * e0 - ep - em) / 24.0
    gh2 = (ep - em) / 2.0
    lh3 = ep - 2.0 * e0 + em
    return (fh, gh2, lh3), False


def tone4(A, phi0, fh, gh2=0.0, lh3=0.0):
    """4 noise-free samples, residual phase r(u)=f u+g u^2/2+l u^3/6 cycles."""
    u = np.array([-1.5, -0.5, 0.5, 1.5])
    r = fh * u + 0.5 * gh2 * u ** 2 + (lh3 / 6.0) * u ** 3
    return A * np.exp(2j * np.pi * (phi0 + r))


def demo() -> None:
    ctr = Counter()
    # 1. exact stencil (N23.1)
    x = tone4(1.0, 0.3, 0.1, 0.02, 0.006)
    (fh, gh2, lh3), bad = estimate(x, np.zeros(3), ctr)
    print(f"[stencil] fh={fh:.12f} gh2={gh2:.12f} lh3={lh3:.12f}")
    assert not bad and ctr.n == 3, "3 products"
    assert abs(fh - 0.1) < 1e-12 and abs(gh2 - 0.02) < 1e-12 and abs(lh3 - 0.006) < 1e-12
    e = tone4(1.0, 0.0, 0.0, 0.0, 0.006)  # pure cubic -> f=g=0
    (fh0, gh20, _), _ = estimate(e, np.zeros(3), ctr)
    assert abs(fh0) < 1e-12 and abs(gh20) < 1e-12, "pure cubic"
    for phi0 in (0.0, 1.7, -2.9):  # phase-offset sweep
        (f_, _, _), _ = estimate(tone4(2.5, phi0, -0.07, 0.01, -0.003), np.zeros(3), ctr)
        assert abs(f_ + 0.07) < 1e-12, "offset sweep"
    print("[stencil] increments (0.08325,0.10025,0.12325) recovered; offsets swept")
    assert ctr.n == 3 * 5, f"product count {ctr.n}"

    # 2. fixed-point bounds (N23.2): common I/Q scale, amplitude 80
    h = 12.8e-6
    rng = np.random.default_rng(7)
    worst = 0.0
    for _ in range(2000):
        th = rng.uniform(-np.pi, np.pi)
        z = 80.0 * np.exp(1j * th)
        zq = np.round([z.real, z.imag])  # INT8 grid, common scale 1
        err = abs(np.angle(complex(zq[0], zq[1])) - th)
        err = min(err, 2 * np.pi - err)
        worst = max(worst, err)
    bound = float(np.arcsin(np.sqrt(0.5) / 80.0))
    print(f"[fixedpt] worst angle err={worst:.6f} rad bound={bound:.6f}")
    assert worst <= bound + 1e-12, "quant angle bound"
    df = 7.0 * bound / (6.0 * np.pi * h)
    dR = df / K_R
    print(f"[fixedpt] quant-only range bound={dR*1e3:.2f} mm (target ~8)")
    assert dR < 0.010, "8mm class bound"
    _, bad0 = estimate(np.zeros(4), np.zeros(3), ctr)
    assert bad0, "zero amplitude -> invalid"
    _, bad1 = estimate(tone4(1.0, 0.0, 0.05), None, ctr)
    assert bad1, "missing prediction -> invalid"

    # 3. statistical target (N23.3): 30 dB, 10k trials, |fh|<=0.2
    snr_lin, n = 1000.0, 10_000
    sig = 1.0 / np.sqrt(snr_lin)  # complex-noise std per sample
    f_true = rng.uniform(-0.2, 0.2, n)
    ph = rng.uniform(0, 2 * np.pi, n)
    u = np.array([-1.5, -0.5, 0.5, 1.5])
    s = np.exp(2j * np.pi * (ph[:, None] + f_true[:, None] * u[None, :]))
    s = s + sig * (rng.normal(size=(n, 4)) + 1j * rng.normal(size=(n, 4))) / np.sqrt(2)
    errs, alias = [], 0
    for k in range(n):
        C0 = s[k, 1:] * np.conj(s[k, :-1])
        eta = np.angle(C0) / (2 * np.pi)
        fh_k = (26 * eta[1] - eta[2] - eta[0]) / 24.0
        if abs(fh_k - f_true[k]) > 0.25:
            alias += 1
        errs.append(fh_k - f_true[k])
    errs = np.array(errs)
    rmse_R = float(np.sqrt((errs ** 2).mean())) / h / K_R
    print(f"[noise] range RMSE={rmse_R*1e3:.2f} mm (theory ~13-19, accept <=25), alias={alias}/{n}")
    assert rmse_R <= 0.025, "statistical target"
    # velocity-prior coupling: 0.5 m/s -> ~7.7 mm
    coup = 2.0 * 0.5 / LAM / K_R
    print(f"[couple] 0.5 m/s prior error -> {coup*1e3:.2f} mm (expect ~7.7)")
    assert abs(coup - 0.0077) < 5e-4, "coupling"
    # second-tone stress (report failure, do not extrapolate)
    for sir_db in (-20.0, -6.0, 0.0):
        amp2 = 10.0 ** (sir_db / 20.0)
        f2 = rng.uniform(-0.4, 0.4, 2000)
        s1 = np.exp(2j * np.pi * (rng.uniform(0, 2 * np.pi, 2000)[:, None]
                                  + np.zeros(2000)[:, None] * u[None, :]))
        s2 = amp2 * np.exp(2j * np.pi * f2[:, None] * u[None, :])
        tot = s1 + s2
        C0 = tot[:, 1:] * np.conj(tot[:, :-1])
        eta = np.angle(C0) / (2 * np.pi)
        fh2 = (26 * eta[:, 1] - eta[:, 2] - eta[:, 0]) / 24.0
        print(f"[interf] SIR={sir_db:+.0f}dB rmse_fh={float(np.sqrt((fh2**2).mean())):.3f} "
              f"(one-tone guarantee void)")
    print("demo PASS")


if __name__ == "__main__":
    demo()
