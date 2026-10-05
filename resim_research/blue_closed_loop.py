"""Occupancy-BLUE closed detector-tracker loop (N22g recipe step 4).

Scalar channel: ghost energy leaks into a reference cell; BLUE (with
ghost-predicted occupancy) holds the threshold down so an adjacent weak
CUT target is still detected, while plain CA-16 is blinded. Tracker loop
P->pi->beta->muhat->R->P shown contractive (loop gain < 1) with a stable
fixed point from two initializations.
# ponytail: scalar prototypes; Pfa*=1e-2 for MC speed (formula is exact).
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import numpy as np

from resim_research.occupancy_blue import blue_weights, threshold_alpha

PFA = 1e-2
KAPPA = 1.0  # R = kappa/s_hat scale


def demo() -> None:
    rng = np.random.default_rng(11)
    # scene: 16 ref cells; cell 3 carries deterministic ghost leakage s_g=10,
    # predicted occupancy pi_g=0.9; CUT holds a weak target s_tgt=2 (3 dB)
    pis = np.zeros(16)
    ss = np.zeros(16)
    pis[3], ss[3] = 0.9, 10.0
    beta, m, v = blue_weights(pis, ss)
    alpha_b = threshold_alpha(beta, PFA)
    beta_ca = np.full(16, 1 / 16)
    alpha_ca = threshold_alpha(beta_ca, PFA)
    print(f"[thresh] BLUE alpha={alpha_b:.3f} (ghost cell w={beta[3]:.4f}) vs "
          f"CA16 alpha={alpha_ca:.3f}")

    n = 200_000
    ghost = rng.exponential(1 + 10.0, n)  # ghost always present in cell 3
    refs = rng.exponential(1.0, (n, 16))
    refs[:, 3] = ghost
    cut_t = rng.exponential(1 + 2.0, n)  # weak target present
    cut_0 = rng.exponential(1.0, n)  # noise-only CUT
    det_b = cut_t > alpha_b * (refs * beta).sum(axis=1)
    det_c = cut_t > alpha_ca * (refs * beta_ca).sum(axis=1)
    fa_b = cut_0 > alpha_b * (refs * beta).sum(axis=1)
    fa_c = cut_0 > alpha_ca * (refs * beta_ca).sum(axis=1)
    print(f"[detect] recall BLUE={det_b.mean():.4f} CA16={det_c.mean():.4f}; "
          f"FA BLUE={fa_b.mean():.2e} CA16={fa_c.mean():.2e}")
    assert det_b.mean() > det_c.mean() + 0.05, "ghost-adjacent recall gain"
    # CA16's lower FA is ghost-blinding (recall collapses); BLUE holds near-nominal FA
    assert fa_b.mean() <= PFA * 1.25, "FA near nominal under ghost contamination"

    # tracker loop (Theorem 3 chain, genuine coupling):
    # P -> pi(P) [ghost-cell mass grows with predicted variance] ->
    # Var(muhat)=1/w(P) -> R(P) via Jensen correction on s_hat=Ecut/muhat-1
    # -> P_next. BLUE is unbiased so E[muhat]=1; coupling rides on variance.
    q_proc, s_tgt = 0.5, 3.0
    c0 = 0.35
    Ecut = 1 + s_tgt

    def w_of_P(P):
        pi = min(1 - np.exp(-c0 * P), 0.99)
        mm = 1 + pi * 10.0
        vv = 2 * (1 - pi + pi * 121.0) - mm ** 2
        return 15 * 1.0 + mm ** 2 / vv  # 15 clean cells + ghost cell

    def R_of_P(P):
        var = 1.0 / w_of_P(P)
        return KAPPA / s_tgt * (1 + var / s_tgt ** 2)

    def P_next(P):
        Ppred = P + q_proc
        R = R_of_P(P)
        return Ppred * R / (Ppred + R)

    P_star = 1.0
    for _ in range(1000):
        P_star = P_next(P_star)
    h = 1e-6
    gain = (P_next(P_star + h) - P_next(P_star - h)) / (2 * h)
    print(f"[loop] P*={P_star:.4f} full-chain gain={gain:.4f} (<1 required); "
          f"R(P*)={R_of_P(P_star):.4f} Var(muhat)={1/w_of_P(P_star):.4f}")
    assert gain < 1.0, "contraction"
    for P_init in (0.1, 10.0):
        P = P_init
        for _ in range(200):
            P = P_next(P)
        assert abs(P - P_star) < 1e-6, f"fixed point from {P_init}"
    print("[loop] fixed point stable from P=0.1 and P=10.0")
    print("demo PASS")


if __name__ == "__main__":
    demo()
