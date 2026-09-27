"""N18 combined ghost chain: estimate -> gate -> assimilate (system test).

The keeps (N9 rank-1 + N11 Doppler gate + R_eff assimilation) were proven
SOLO. N18 wires them into one chain on a single scene and compares against
the OLD field practice end-to-end:

  OLD: bearing-window ghost flagging + direct-only EKF (ghost = waste).
  NEW: rank1_consensus reflector estimation -> Doppler-gated pair edges ->
       dual-path EKF with estimated reflector + R_eff coupling
       (ghost = virtual aperture).

Metrics (40 seeds, 6 moving targets + ghosts + decoys + clutter):
  (a) ghost-edge precision old vs new,
  (b) parent track RMSE old (direct) vs new (dual, ESTIMATED xi),
  (c) reflector estimation error |d-hat - d|, angle(n-hat, n).
# ponytail: single wall, constant velocities; maneuvering parents are next.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import numpy as np

from resim_research.specular_ghost import (
    rank1_consensus, classify_pairs, bearing_gate, ekf_filter,
    plane_specular, SIG_V, EPS,
)
from resim_research.doppler_disambiguation import (
    make_doppler_scene, observe_doppler, doppler_gate, SIGV_TRACK,
)


def run_seed(seed):
    rng = np.random.default_rng(seed)
    P, V, _, truth, (d_true, n_true), _ = make_doppler_scene(
        seed, n_tgt=6, decoy=True)
    X, vr = observe_doppler(P, V, rng)
    # ---- OLD: bearing gate
    old_edges = set(bearing_gate(X))
    # ---- NEW: estimate reflector, gate pairs by Doppler
    modes = rank1_consensus(X)
    new_edges = set()
    d_hat, n_hat = None, None
    if modes:
        # strongest mode -> reflector estimate (median-2d/2 = d)
        m = max(modes, key=lambda r: r[4])
        d_hat, n_hat = m[0] / 2.0, m[1] / max(np.linalg.norm(m[1]), EPS)
        cand = set()
        for refl in modes:
            cand.update(classify_pairs(X, refl))
        Vt = V + rng.normal(0.0, SIGV_TRACK, V.shape)
        new_edges = set(doppler_gate(X, vr, Vt, cand))
    n_truth = max(len(truth), 1)

    def pr(sel):
        inter = len(truth & sel)
        return inter / n_truth, inter / max(len(sel), 1)
    ro, po = pr(old_edges)
    rn, pn = pr(new_edges)
    # ---- tracking leg: EKF on target 0, direct vs dual-with-estimate
    truths = [(P[0], V[0])]
    refl = [(d_hat if d_hat is not None else d_true,
             n_hat if n_hat is not None else n_true)]
    err_direct = ekf_filter(truths, [(d_true, n_true)], (0.0, 0.0),
                            "direct", seed=seed)[1]
    if d_hat is not None:
        err_dual = ekf_filter(truths, refl, (0.0, 0.0), "dual_naive",
                              seed=seed)[1]
    else:
        err_dual = err_direct
    dang = (float(np.degrees(np.arccos(
        min(1.0, abs(float(n_hat @ n_true)))))) if n_hat is not None else 90.0)
    derr = abs(d_hat - d_true) if d_hat is not None else 99.0
    return (ro, po, rn, pn, err_direct, err_dual, derr, dang)


def demo() -> None:
    R = [run_seed(s) for s in range(20, 60)]
    ro = float(np.mean([r[0] for r in R]))
    po = float(np.mean([r[1] for r in R]))
    rn = float(np.mean([r[2] for r in R]))
    pn = float(np.mean([r[3] for r in R]))
    eo = float(np.mean([r[4] for r in R]))
    en = float(np.mean([r[5] for r in R]))
    dd = float(np.mean([r[6] for r in R if r[6] < 90]))
    da = float(np.mean([r[7] for r in R if r[7] < 90]))
    print(f"[edges] old(bearing): rec={ro:.3f} prec={po:.3f}")
    print(f"[edges] new(chain)  : rec={rn:.3f} prec={pn:.3f}")
    print(f"[track] old(direct) RMSE={eo:.3f} m  new(dual-est) RMSE={en:.3f} m")
    print(f"[estim] |d-dhat|={dd:.3f} m  angle(n,nhat)={da:.2f} deg")
    assert pn > po + 0.15, "chain must lift edge precision"
    assert rn >= ro - 0.05, "must not lose true edges"
    assert en < eo, "estimated-aperture assimilation must help tracking"
    print("demo PASS")


if __name__ == "__main__":
    demo()
