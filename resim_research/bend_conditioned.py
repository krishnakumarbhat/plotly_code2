"""Bend-conditioned specular test for curved guardrails (T2-N10).

E2e proved the constant-2d spike smears over a 132 m band on a cylinder
(recall 0.993 -> 0.450). Fix: replace the scalar 2d hypothesis with a
bearing-conditioned one. For a vertical cylinder (radius Rc, wall line
y = yg, center C = (0, yg+Rc)), the tangent-plane offset at pair bearing
phi is D(phi) = (yg+Rc)*cos(phi) - Rc (signed), so

    |Delta_i| = 2*|D(phi_i)|,   phi_i = bearing of pair midpoint

Fit (Rc, yg) on a coarse grid maximizing direction-coherent inliers, then
classify pairs against the conditioned prediction. Truth data: faceted
cylinder (36 tangent planes, nearest-valid-facet first-order specular).
# ponytail: single cylinder + single scan; multi-bend scenes need sequential modes.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import numpy as np

from resim_research.specular_ghost import (
    plane_specular, rank1_consensus, classify_pairs, observe,
    SIG_R, EPS,
)

N_FACET = 36


def cylinder_facets(rc, yg):
    """Tangent planes of vertical cylinder touching wall line y=yg."""
    C = np.array([0.0, yg + rc, 0.0])
    out = []
    for j in range(N_FACET):
        a = 2.0 * np.pi * j / N_FACET
        n = np.array([np.sin(a), -np.cos(a), 0.0])  # outward, road side
        F = C + rc * np.array([np.sin(a), -np.cos(a), 0.0])
        out.append((n, float(n @ F), F))
    return out


def seg_clear(a, b, C, rc):
    """True if segment ab stays outside the cylinder (occlusion test)."""
    ab = b - a
    t = float(-(a - C) @ ab) / max(float(ab @ ab), 1e-12)
    t = min(max(t, 0.0), 1.0)
    return float(np.linalg.norm(a + t * ab - C)) >= rc - 1e-9


def facet_ghost(p, facets, width, C=None, rc=0.0):
    """Nearest valid facet first-order specular ghost (unfolded position).

    Correct construction: mirror the SENSOR (S' = 2d·n), foot = crossing of
    segment S'->P with the facet plane. Valid: sensor and target share the
    outer side + foot on patch + occlusion-free both legs."""
    S = np.zeros(3)
    best = None
    for n, d, F in facets:
        if (-d) * (float(n @ p) - d) <= 0:
            continue
        Sp = 2.0 * d * n                       # mirrored sensor
        denom = float(n @ (np.asarray(p) - Sp))
        if abs(denom) < 1e-12:
            continue
        t = float(d - n @ Sp) / denom
        if not (0.0 <= t <= 1.0):
            continue
        foot = Sp + t * (np.asarray(p) - Sp)
        tang = np.array([-n[1], n[0], 0.0])
        if abs(float((foot - F) @ tang)) > 0.75 * width:
            continue
        if C is not None and not (seg_clear(S, foot, C, rc)
                                  and seg_clear(foot, p, C, rc)):
            continue
        q = np.asarray(p) - 2.0 * (float(n @ p) - d) * n
        r = float(np.linalg.norm(q))
        if best is None or r < best[0]:
            best = (r, q)
    return best[1] if best is not None else None


def make_cylinder_scene(seed, n_tgt=8, rc=30.0, yg=3.0):
    rng = np.random.default_rng(seed)
    facets = cylinder_facets(rc, yg)
    width = 2.0 * np.pi * rc / N_FACET
    C = np.array([0.0, yg + rc, 0.0])
    P, V, edges = [], [], set()
    for _ in range(n_tgt):
        # traffic lanes BETWEEN sensor and wall (same side, y < yg):
        # the physical guardrail-multipath regime
        p = np.array([rng.uniform(10.0, 55.0), rng.uniform(0.5, yg - 0.2),
                      rng.uniform(-0.3, 0.3)])
        P.append(p)
        V.append(np.zeros(3))
    n = len(P)
    X = list(P)
    for i, p in enumerate(P):
        g = facet_ghost(p, facets, width, C, rc)
        if g is not None:
            X.append(g)
            edges.add((i, n + len(X) - n - 1))
    return np.array(X), edges, (rc, yg), rng


def surf_residual(m, rc, yg):
    """Midpoint-on-surface invariant — REFUTED (kept for the record).

    The midpoint ALWAYS lies on the facet plane, but generally far from the
    foot along the plane (|m-C|^2 = Rc^2 + t^2 with t up to 17 m here), so
    |m-C| ~= Rc holds only for parents hugging the wall. Do not use."""
    C = np.array([0.0, yg + rc, 0.0])
    return abs(float(np.linalg.norm(m - C)) - rc), (m - C) / max(float(np.linalg.norm(m - C)), EPS)


A_GRID = np.linspace(0.0, 2.0 * np.pi, 720, endpoint=False)


def predict_ghost(p, rc, yg):
    """Forward model: Fermat reflection point on the cylinder + mirror.
    Returns (q_hat, u_hat) or (None, None) if no outer-side path."""
    C = np.array([0.0, yg + rc, 0.0])
    Fa = C[None, :] + rc * np.stack(
        [np.sin(A_GRID), -np.cos(A_GRID), np.zeros_like(A_GRID)], axis=1)
    L = np.linalg.norm(Fa, axis=1) + np.linalg.norm(Fa - p[None, :], axis=1)
    j = int(np.argmin(L))
    F = Fa[j]
    n = (F - C) / max(float(np.linalg.norm(F - C)), EPS)
    d = float(n @ F)
    if (-d) * (float(n @ p) - d) <= 0:
        return None, None
    q = np.asarray(p) - 2.0 * (float(n @ p) - d) * n
    u = (np.asarray(p) - q)
    u /= max(float(np.linalg.norm(u)), EPS)
    return q, u


def bend_fit(X, tol=1.0, coh_deg=15.0):
    """RANSAC over cylinder hypotheses: for each (Rc, yg) predict each
    pair's ghost from its parent endpoint via the Fermat forward model;
    inlier = position + direction agreement. Orientation: the endpoint
    FARTHER from the fitted cylinder is the parent (ghosts sit behind
    the wall). Returns (rc, yg, pairs)."""
    N = X.shape[0]
    iu, ju = np.triu_indices(N, k=1)
    D = X[iu] - X[ju]
    L = np.linalg.norm(D, axis=1)
    U = D / np.maximum(L, EPS)[:, None]
    cos_coh = np.cos(np.radians(coh_deg))
    best = (-1.0, None, None, None)
    for rc in (10.0, 20.0, 30.0, 45.0, 60.0):
        for yg in (1.5, 3.0, 5.0, 8.0):
            C = np.array([0.0, yg + rc, 0.0])
            hits = np.zeros(L.size, dtype=bool)
            par = np.zeros(L.size, dtype=int)  # 0: iu=parent, 1: ju=parent
            for k in range(L.size):
                a, b = int(iu[k]), int(ju[k])
                da = float(np.linalg.norm(X[a] - C)) - rc
                db = float(np.linalg.norm(X[b] - C)) - rc
                # parent = farther from cylinder surface
                if da >= db:
                    pa, qa = X[a], X[b]
                    s = 0
                else:
                    pa, qa = X[b], X[a]
                    s = 1
                qh, uh = predict_ghost(pa, rc, yg)
                if qh is None:
                    continue
                if (float(np.linalg.norm(qa - qh)) <= tol
                        and abs(float(U[k] @ uh)) >= cos_coh):
                    hits[k] = True
                    par[k] = s
            cnt = int(hits.sum())
            if cnt > best[0]:
                best = (float(cnt), rc, yg, (hits, par))
    _, rc, yg, hb = best
    if rc is None or hb is None:
        return None
    hits, par = hb
    pairs = set()
    for k in np.nonzero(hits)[0]:
        a, b = int(iu[k]), int(ju[k])
        pairs.add((a, b) if par[k] == 0 else (b, a))
    if len(pairs) < 3:
        return None
    return rc, yg, pairs
    return rc, yg, pairs


def run_scan(seed):
    Xt, truth, _, rng = make_cylinder_scene(seed)
    X = observe(Xt, rng)
    # baseline: planar rank-1 consensus
    modes = rank1_consensus(X)
    base = set()
    for refl in modes:
        base.update(classify_pairs(X, refl))
    fit = bend_fit(X)
    cond = fit[2] if fit else set()
    n_truth = max(len(truth), 1)

    def pr(sel):
        inter = len(truth & sel)
        return inter / n_truth, inter / max(len(sel), 1)
    return pr(base) + pr(cond)


def demo() -> None:
    rb, pb, rg, pg = [], [], [], []
    for s in range(20, 60):
        a, b, c, d = run_scan(s)
        rb.append(a)
        pb.append(b)
        rg.append(c)
        pg.append(d)
    print(f"planar-consensus: recall={np.mean(rb):.3f} prec={np.mean(pb):.3f}")
    print(f"bend-conditioned: recall={np.mean(rg):.3f} prec={np.mean(pg):.3f}")
    assert np.mean(rg) - np.mean(rb) >= 0.25, "must recover curved recall"
    assert np.mean(rg) >= 0.75, "curved operating point"
    assert np.mean(pg) >= 0.60, "must not drown in false pairs"
    print("demo PASS")


if __name__ == "__main__":
    demo()
