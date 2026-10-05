"""Track-velocity-aided Doppler disambiguation of specular pairs (T2-N11).

N9 left a provable blind spot: two INDEPENDENT targets 2d apart at the same
range satisfy the geometric pair test (|Delta|=2d, Delta||n) — decoy precision
0.638 with geometry alone. E2b showed the ghost Doppler is a deterministic
function of the PARENT velocity (offset decodable at 40 sigma for a cut-in).

Idea: the tracker already estimates parent velocity vectors (F360/XTRK).
Predict the ghost's radial velocity under the pair hypothesis and gate:

    vr_b_pred = u_sb . v_a_est,   u_sb = p_b_unfolded / |p_b_unfolded|
    |vr_b_meas - vr_b_pred| <= K * sqrt(SIG_V^2 + u_sb^T Sv u_sb)

A true ghost MUST satisfy this (same moving parent); a decoy has independent
velocity and fails unless coincidentally consistent. Tracker->detector
feedback, same architectural theme as T1's track-masked floor.
# ponytail: single-scan gate on track priors; M-of-N across scans if solo gate saturates.
"""
import numpy as np
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from resim_research.specular_ghost import (
    plane_specular, rank1_consensus, classify_pairs, observe,
    SIG_V, SIG_R, EPS,
)

SIGV_TRACK = 0.5   # track velocity-vector uncertainty per axis (m/s)
K_GATE = 3.0       # sigma gate


def make_doppler_scene(seed, n_tgt=6, d=3.0, decoy=False):
    """Positions + true velocity vectors. Ghosts inherit parent velocity;
    decoys get independent velocities (the adversarial case)."""
    rng = np.random.default_rng(seed)
    n = np.array([0.0, 1.0, 0.0])
    P, V, is_ghost, edges = [], [], [], set()
    for _ in range(n_tgt):
        r = rng.uniform(12.0, 60.0)
        az = np.radians(rng.uniform(6.0, 55.0))
        p = np.array([r * np.cos(az), r * np.sin(az), rng.uniform(-0.3, 0.3)])
        v = np.array([rng.uniform(5, 15), rng.uniform(-6, 6), 0.0])
        P.append(p)
        V.append(v)
        is_ghost.append(False)
    if decoy:
        for sgn in (+1.0, -1.0):
            p = np.array([35.0, sgn * d, 0.0])
            v = np.array([rng.uniform(-8, 8), rng.uniform(-8, 8), 0.0])
            P.append(p)
            V.append(v)
            is_ghost.append(False)
    n_real = len(P)
    for i in range(n_real):
        _, _, q = plane_specular(P[i], d, n)
        P.append(q)              # ghost: unfolded position the tracker sees
        V.append(V[i].copy())    # ghost inherits parent motion
        is_ghost.append(True)
        edges.add((i, n_real + i))
    return (np.array(P), np.array(V), np.array(is_ghost), edges,
            (d, n), rng)


def observe_doppler(P, V, rng):
    """Noisy positions (shared observe) + noisy radial velocities."""
    X = observe(P, rng)
    vr = np.array([(X[i] / max(np.linalg.norm(X[i]), EPS)) @ V[i]
                   + rng.normal(0.0, SIG_V) for i in range(X.shape[0])])
    return X, vr


def doppler_gate(X, vr, Vtracking, edges, Sv=SIGV_TRACK ** 2):
    """Keep candidate edges whose child radial velocity matches the
    parent-velocity prediction. Vtracking: (M,3) track velocity priors
    (NaN rows = no track -> edge abstains, kept)."""
    keep = []
    for a, b in edges:
        va = Vtracking[a]
        if not np.all(np.isfinite(va)):
            keep.append((a, b))
            continue
        u_sb = X[b] / max(np.linalg.norm(X[b]), EPS)
        pred = float(u_sb @ va)
        gate = K_GATE * np.sqrt(SIG_V ** 2 + Sv)
        if abs(vr[b] - pred) <= gate:
            keep.append((a, b))
    return keep


def run_scan(seed, decoy=False, track_noise=SIGV_TRACK, confirm=True):
    """Full pipeline: geometry proposes, Doppler disposes. With confirm,
    the gate must pass on TWO independent noise realizations (fresh speckle
    + fresh track noise); coincidences rarely repeat, true pairs persist."""
    P, V, _, truth, _, rng = make_doppler_scene(seed, decoy=decoy)
    X, vr = observe_doppler(P, V, rng)
    modes = rank1_consensus(X)
    if not modes:
        return None
    cand = set()
    for refl in modes:
        cand.update(classify_pairs(X, refl))
    # track priors = truth velocity + noise (decoys: own independent truth)
    Vt = V + rng.normal(0.0, track_noise, V.shape)
    gated = set(doppler_gate(X, vr, Vt, cand))
    if confirm:
        rng2 = np.random.default_rng(seed + 1000)
        X2, vr2 = observe_doppler(P, V, rng2)
        Vt2 = V + rng2.normal(0.0, track_noise, V.shape)
        gated &= set(doppler_gate(X2, vr2, Vt2, cand))

    def pr(sel):
        inter = len(truth & sel)
        return (inter / max(len(truth), 1), inter / max(len(sel), 1))
    rb, pb = pr(cand)
    rg, pg = pr(gated)
    return {"rec_before": rb, "prec_before": pb, "rec_after": rg,
            "prec_after": pg, "n_cand": len(cand), "n_gated": len(gated)}


def demo() -> None:
    seeds = range(20, 60)
    dec = [run_scan(s, decoy=True) for s in seeds]
    dec = [r for r in dec if r]
    pla = [r for r in (run_scan(s) for s in seeds) if r]
    for label, rs in (("decoy", dec), ("planar", pla)):
        rb = float(np.mean([r["rec_before"] for r in rs]))
        pb = float(np.mean([r["prec_before"] for r in rs]))
        rg = float(np.mean([r["rec_after"] for r in rs]))
        pg = float(np.mean([r["prec_after"] for r in rs]))
        grr = 1.0 - (1.0 - pg) / max(1.0 - pb, 1e-9)  # false-edge removal rate
        print(f"[{label:6s}] n={len(rs)} geom: rec={rb:.3f} prec={pb:.3f} "
              f"-> doppler-gated: rec={rg:.3f} prec={pg:.3f} GRR={grr:.3f}")
    d_pb = float(np.mean([r["prec_before"] for r in dec]))
    d_pg = float(np.mean([r["prec_after"] for r in dec]))
    d_rb = float(np.mean([r["rec_before"] for r in dec]))
    d_rg = float(np.mean([r["rec_after"] for r in dec]))
    p_rb = float(np.mean([r["rec_before"] for r in pla]))
    p_rg = float(np.mean([r["rec_after"] for r in pla]))
    grr = 1.0 - (1.0 - d_pg) / max(1.0 - d_pb, 1e-9)
    assert d_pg - d_pb >= 0.20, "Doppler gate must lift decoy precision"
    assert grr >= 0.60, "ghost reduction rate operating point"
    assert d_rb - d_rg <= 0.03 and p_rb - p_rg <= 0.03, "recall cost bounded"
    print("demo PASS")


if __name__ == "__main__":
    demo()
