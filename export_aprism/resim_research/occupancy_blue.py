"""Occupancy-BLUE CFAR noise estimation (N22g).

Theorem 1: beta_i = (m_i/v_i)/sum(m_j^2/v_j), m_i = 1+pi_i s_i,
v_i = 2[1-pi+pi(1+s)^2] - (1+pi s)^2. Theorem 2: exact Pfa for
prior-only weights: Pfa = prod 1/(1+alpha beta_i).
# ponytail: exponential cells, Newton/bisection threshold, vector MC.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import numpy as np


def moments(pi, s):
    """Mean/variance ratios m, v for occupancy pi and SNR s."""
    m = 1.0 + pi * s
    v = 2.0 * (1.0 - pi + pi * (1.0 + s) ** 2) - m ** 2
    return m, max(v, 1e-12)


def blue_weights(pis, ss):
    """Theorem 1 BLUE weights."""
    m = np.array([moments(p, s)[0] for p, s in zip(pis, ss)])
    v = np.array([moments(p, s)[1] for p, s in zip(pis, ss)])
    num = m / v
    return num / float(np.sum(m * num)), m, v


def threshold_alpha(beta, pfa_star):
    """Solve sum ln(1+alpha beta) + ln Pfa* = 0 (monotone concave)."""
    beta = np.asarray(beta, float)
    lo, hi = 0.0, 1.0
    def g(a):
        return float(np.sum(np.log1p(a * beta)) + np.log(pfa_star))
    assert g(0.0) < 0, "Pfa*<1 required"
    while g(hi) < 0:
        hi *= 2.0
        assert hi < 1e12, "no root"
    for _ in range(200):
        mid = 0.5 * (lo + hi)
        if g(mid) < 0:
            lo = mid
        else:
            hi = mid
    return 0.5 * (lo + hi)


def demo() -> None:
    # 1. worked example
    pis = np.array([0.0] * 14 + [0.3] * 2)
    ss = np.array([0.0] * 14 + [10.0] * 2)
    beta, m, v = blue_weights(pis, ss)
    print(f"[weights] beta_clean={beta[0]:.5f} beta_cont={beta[14]:.6f}")
    assert abs(beta[0] - 0.06872) < 2e-4, "clean weight"
    assert abs(beta[14] - 0.004740) < 2e-5, "contaminated weight"
    assert abs(float(np.sum(beta * m)) - 1.0) < 1e-12, "unbiasedness"
    relvar = 1.0 / float(np.sum(m ** 2 / v))
    print(f"[weights] relvar={relvar:.4f} (CA14=0.0714)")
    assert abs(relvar - 0.0687) < 2e-3, "variance"
    assert relvar < 1.0 / 14.0, "beats hard censor"

    alpha = threshold_alpha(beta, 1e-4)
    print(f"[threshold] alpha={alpha:.4f}")
    assert 13.2 <= alpha <= 13.4, "alpha range"

    # 2. exact Pfa under noise-only (vectorized MC)
    rng = np.random.default_rng(1)
    n = 1_000_000
    X = rng.exponential(1.0, n)
    Y = rng.exponential(1.0, (n, 16))
    stat = X - alpha * (Y * beta).sum(axis=1)
    emp = float(np.mean(stat > 0))
    print(f"[pfa] empirical={emp:.2e} target=1e-4")
    assert 7.0e-5 <= emp <= 1.3e-4, "Pfa interval"

    # random-beta universality (smaller MC)
    rb = rng.dirichlet(np.ones(8))
    a2 = threshold_alpha(rb, 1e-3)
    n2 = 400_000
    X2 = rng.exponential(1.0, n2)
    Y2 = rng.exponential(1.0, (n2, 8))
    emp2 = float(np.mean(X2 - a2 * (Y2 * rb).sum(axis=1) > 0))
    print(f"[pfa-rand] empirical={emp2:.2e} target=1e-3")
    assert 7.0e-4 <= emp2 <= 1.3e-3, "random-beta Pfa"

    # 3. bias/variance under contamination
    occ = (rng.uniform(size=(200_000, 16)) < pis).astype(float)
    P = np.where(occ > 0, rng.exponential(1.0 + ss, (200_000, 16)),
                 rng.exponential(1.0, (200_000, 16)))
    mu_blue = P @ beta
    print(f"[contam] BLUE mean={float(mu_blue.mean()):.4f} var={float(mu_blue.var()):.4f} "
          f"(theory 1.0, {relvar:.4f})")
    assert abs(float(mu_blue.mean()) - 1.0) < 3e-3, "unbiased"
    assert abs(float(mu_blue.var()) - relvar) / relvar < 0.05, "variance match"
    mu_ca16 = P.mean(axis=1)
    print(f"[contam] CA16 mean={float(mu_ca16.mean()):.4f} (expect ~1.375)")
    assert abs(float(mu_ca16.mean()) - 1.375) < 0.02, "CA bias"
    print("demo PASS")


if __name__ == "__main__":
    demo()
