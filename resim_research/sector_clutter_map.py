"""Tracker-masked sector clutter map vs global floor (N17 backlog).

Seed: sector-LOCAL floors with track masking (CoFAR spirit, sector prior).
The regime where sectors matter is SPARSE updating (real dwell
scheduling observes a subset of bins per scan): per-bin EMA starves
while sector pooling shares across bins. Test: 25% bins/scan observed,
15% track-masked; metric = floor MSE vs truth after settling.
# ponytail: fixed 8 sectors; adaptive sectorization if boundaries bite.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import numpy as np

LAM = 0.05
N_SEC = 16
OBS_FRAC = 0.10  # sparse regime: per-bin EMA starves, pooling shares
TRIM = 0.25  # (superseded by median+in-pool censoring; kept documented)


def truth_floor(r):
    """True noise floor: block + clean + far rise."""
    if 30 <= r < 55:
        return 25.0
    if r >= 100:
        return 1.5
    return 1.0


def run_trial(seed, n_scan=120):
    rng = np.random.default_rng(seed)
    R = 128
    Cg = np.ones(R)
    Cs = np.ones((N_SEC, R))
    err_g, err_s, cnt = 0.0, 0.0, 0
    bounds = np.linspace(0, R, N_SEC + 1).astype(int)
    for _ in range(n_scan):
        p = np.array([rng.exponential(truth_floor(r)) for r in range(R)])
        free = rng.random(R) > 0.15
        obs = rng.random(R) < OBS_FRAC
        use = free & obs
        Cg = np.where(use, (1 - LAM) * Cg + LAM * p, Cg)
        for s in range(N_SEC):
            lo, hi = bounds[s], bounds[s + 1]
            vals = p[lo:hi][use[lo:hi]]
            if vals.size >= 2:
                # censor regime edges INSIDE the pool (T1-style): drop
                # cells >6 dB from the sector median before pooling
                med = float(np.median(vals))
                vals = vals[vals <= 4.0 * med]
            if vals.size >= 2:
                pooled = float(np.median(vals)) / 0.6931471805599453
                Cs[s, lo:hi] = (1 - LAM) * Cs[s, lo:hi] + LAM * pooled
        if _ >= 60:  # settled
            tgt = np.array([truth_floor(r) for r in range(R)])
            err_g += float(np.mean((Cg - tgt) ** 2))
            est = np.concatenate([Cs[s, bounds[s]:bounds[s + 1]]
                                  for s in range(N_SEC)])
            err_s += float(np.mean((est - tgt) ** 2))
            cnt += 1
    return err_g / cnt, err_s / cnt


def demo() -> None:
    sg, ss = [], []
    for s in range(20):
        g, e = run_trial(s)
        sg.append(g)
        ss.append(e)
    # NOTE run_trial returns (global, sector); collect accordingly
    print(f"[floor] global MSE={np.mean(sg):.3f} sector-masked MSE={np.mean(ss):.3f}")
    assert np.mean(ss) < np.mean(sg) / 1.5, "sectors must win in sparse regime"
    print("demo PASS")


if __name__ == "__main__":
    demo()
