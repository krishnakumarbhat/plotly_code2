"""Specular-pair rank-1 consensus + dual-path assimilation (T2 / N3).

Paradigm: a first-order specular echo is a DETERMINISTIC function of the parent
target's state, not a spurious target. Unfold the path by reflecting the sensor
in the mirror plane: s* = 2 d n. Then

    r_spec = |p - 2 d n|,   u_spec = (p - 2 d n)/|p - 2 d n|,
    p_s    := r_spec u_spec = p - 2 d n      (exact, any p, d, n)

RANK-1 DISPLACEMENT: p - p_s = 2 d n, a vector of constant magnitude 2d along a
common direction n, INDEPENDENT of target range / velocity / bearing. So one
1-DOF histogram of |p_i - p_j| over all detection pairs identifies the reflector
per scan (no track history, no velocity, no Hungarian assignment), and every
pair at that 2d with that n is a (parent, ghost) edge. Multiple reflectors =
multiple peaks in the SAME histogram.

ASSIMILATION: the ghost is a virtual antenna. Direct gradient u_d and ghost
gradient u_s are non-parallel, so the stacked position Jacobian has rank 2
instead of 1. But the ghost is NOT an independent measurement: the reflector
parameters xi=(d,n) are shared, so the correct noise covariance is
    R_eff = diag(sigma^2 I) + J_xi Sigma_xi J_xi^T     (rank-2 cross-coupled)
Naive diag(R) over-trusts the ghost. See equations.md E2a..E2e.

# ponytail: single-scan geometric test only; temporal track confirmation is
# deliberately omitted (add if per-scan recall saturates and FA dominates).
"""
import numpy as np

SIG_R, SIG_V = 0.10, 0.05          # sensor noise: range m, radial velocity m/s
EPS = 1e-12


# ---------------------------------------------------------------- geometry
def direct_obs(p):
    """Direct (non-reflected) observation geometry: range, unit bearing."""
    r = float(np.linalg.norm(p))
    return r, p / r


def plane_specular(p, d, n):
    """First-order specular path via a plane n.x = d (n unit, d > 0).

    Returns (r_spec, u_spec, p_unfolded). p_unfolded is the position a
    Cartesian tracker infers from range+bearing.
    """
    q = p - 2.0 * d * n
    r = float(np.linalg.norm(q))
    return r, q / r, q


def naive_ghost(p, d, n):
    """The field's default model: ghost sits at the TARGET's mirror image."""
    return p - 2.0 * (float(n @ p) - d) * n


def spec_rdot(p, v, d, n):
    """Exact specular radial velocity dr_spec/dt = u_spec . v."""
    _, u_s, _ = plane_specular(p, d, n)
    return float(u_s @ v)


def bearing_az(p):
    """Azimuth in degrees (atan2(y, x))."""
    return np.degrees(np.arctan2(p[1], p[0]))


# ------------------------------------------------- E2a fault quantification
def fault_naive_model():
    """E2a: the mirror-IMAGE bearing is wrong; the sensor-IMAGE bearing is right.

    The naive model is RANGE-exact (|p*| == r_spec) but BEARING-wrong, i.e. a
    half-correct failure that a range-only sanity check cannot catch.
    """
    d, n = 3.0, np.array([0.0, 1.0, 0.0])
    p = np.array([30.0, 2.0, 0.0])
    r_spec, u_spec, p_s = plane_specular(p, d, n)
    p_star = naive_ghost(p, d, n)
    r_star = float(np.linalg.norm(p_star))
    az_spec, az_star = bearing_az(u_spec), bearing_az(p_star)
    # C3: range exact?
    range_err = abs(r_star - r_spec)
    # bearing error and the position error it induces at the TRUE range
    dz = abs(az_spec - az_star)
    pos_err = 2.0 * r_spec * np.sin(np.radians(dz) / 2.0)
    return {
        "d": d, "p": p, "r_spec": r_spec, "az_spec_deg": az_spec,
        "az_naive_deg": az_star, "range_err_m": range_err,
        "bearing_err_deg": dz, "induced_pos_err_m": pos_err,
        "rank1_holds": float(np.linalg.norm(p - p_s) - 2.0 * d),
    }


def fault_doppler():
    """E2b: ghost Doppler != parent Doppler. Offset = v.(u_s - u_d)."""
    d, n = 3.0, np.array([0.0, 1.0, 0.0])
    p = np.array([30.0, 2.0, 0.0])
    v = np.array([0.0, 10.0, 0.0])            # pure lateral cut-in
    _, u_d = direct_obs(p)
    _, u_s, _ = plane_specular(p, d, n)
    vr_d, vr_s = float(u_d @ v), spec_rdot(p, v, d, n)
    # finite-difference cross-check of the closed form
    h = 1e-6
    fd = (plane_specular(p + v * h, d, n)[0] - plane_specular(p - v * h, d, n)[0]) / (2 * h)
    return {
        "vr_direct": vr_d, "vr_spec": vr_s, "offset": vr_s - vr_d,
        "offset_closed_form": float(v @ (u_s - u_d)),
        "fd_check": fd, "fd_err": abs(fd - vr_s),
        "same_bin": abs(vr_s - vr_d) < 0.5,
        "separation_sigma": abs(vr_s - vr_d) / SIG_V,
    }


def fault_curved_guardrail():
    """E2e: the rank-1 constant-2d consensus dies on a curved guardrail.

    Tangent-plane offset for a vertical cylinder of radius Rc at lateral offset
    yg:  D(phi) = (yg + Rc) cos(phi) + Rc,  so |p - p_s| = 2 D(phi) sweeps a
    band instead of a spike, and the displacement direction rotates too.
    """
    yg, rc = 3.0, 30.0
    phi = np.linspace(0.0, np.pi, 2001)
    d_phi = (yg + rc) * np.cos(phi) + rc
    two_d = 2.0 * d_phi
    return {
        "two_d_min_m": float(two_d.min()), "two_d_max_m": float(two_d.max()),
        "band_width_m": float(two_d.max() - two_d.min()),
        "ratio_to_range_noise": float((two_d.max() - two_d.min()) / SIG_R),
        "plane_fixed_2d_m": 2.0 * (yg + rc),
    }


# --------------------------------------------------- 1-DOF reflector ID
def rank1_consensus(X, dmax=90.0, bin=0.05, tol=0.8, n_peak=2, min_r=0.35,
                    coh_deg=15.0, min_coh=3, min_excess=1.0):
    """Identify reflectors from a detection list. 1-DOF search over 2d ONLY.

    X: (N,3) unfolded detection positions. Returns a list of reflectors
    [(two_d, n_hat, n_coherent, resultant_len, excess), ...].

    THE KEY DESIGN POINT: the invariant is the VECTOR 2d*n, not the scalar 2d.
    A plain 1-D histogram of |p_i - p_j| is NOT selective - with ~10 targets a
    random pair falls in the tolerance window as often as a true one, and argmax
    on raw counts or on m*R^2 picks noise (m*R^2 punishes a legitimately wide
    window; the counts have no directional selectivity at all). So score the
    ONE search dimension by the CONCENTRATION of the candidate displacement
    directions, as an excess resultant over the isotropic expectation:
        exc(2d) = |sum_k U_k| - 0.7*sqrt(m),   U_k = Delta_k/|Delta_k|
    A true reflector gives m near-parallel U_k (|sum| ~ m); m isotropic random
    pairs give |sum| ~ sqrt(m/2) and zero excess. Only 2d is searched -> 1 DOF;
    n_hat is ESTIMATED as the resultant, never searched. Then the accepted mode
    is refined on the coherent sub-set (2d = median|Delta|, n = mean U), which
    is what makes 2d accurate. Multiple reflectors = multiple modes of the SAME
    1-D score curve.
    """
    N = X.shape[0]
    if N < 2:
        return []
    iu, ju = np.triu_indices(N, k=1)
    D = X[iu] - X[ju]
    L = np.linalg.norm(D, axis=1)
    keep = (L > min_r) & (L < dmax)
    iu, ju, D, L = iu[keep], ju[keep], D[keep], L[keep]
    if L.size < min_coh:
        return []
    U = D / L[:, None]
    cos_coh = np.cos(np.radians(coh_deg))
    grid = np.arange(bin, dmax, bin)
    exc = np.full(grid.size, -np.inf)
    cache = {}
    for gi, g in enumerate(grid):
        sel = np.abs(L - g) <= tol
        m = int(sel.sum())
        if m < min_coh:
            continue
        S = U[sel].sum(axis=0)
        exc[gi] = float(np.linalg.norm(S)) - 0.7 * np.sqrt(m)
        cache[gi] = (S, sel)
    work = exc.copy()
    exc_max = float(np.nanmax(work[np.isfinite(work)])) if np.isfinite(work).any() else 0.0
    # SEQUENTIAL EXTRACTION with a relative gate: a second mode must be nearly
    # as strong as the first AND explain a DISJOINT (non-consumed) pair set.
    # A true second reflector has comparable support and passes; a noise mode
    # riding on the first reflector's tolerance window does not.
    consumed = np.zeros(L.size, dtype=bool)   # NON-REDUNDANCY
    refl = []
    for pi in range(n_peak):
        gate = max(min_excess, (0.7 if pi else 0.4) * exc_max)
        k = int(np.argmax(work))
        if not np.isfinite(work[k]) or work[k] < gate:
            break
        two_d0 = float(grid[k])
        # FIXED-POINT refinement of n_hat: the direction is never SEARCHED
        # (that would be 2 extra DOF), only re-estimated on its own coherent
        # sub-set. Keep the LARGEST coherent set seen: a naive iterate-and-
        # replace can walk onto a random cluster and collapse, which would
        # silently promote a spurious lower mode to primary.
        n_hat = U[np.abs(L - two_d0) <= tol].sum(axis=0)
        n_hat /= max(np.linalg.norm(n_hat), EPS)
        best_coh, best_n = None, n_hat
        for _ in range(3):
            c_try = (np.abs(L - two_d0) <= tol) & (U @ n_hat >= cos_coh) & (~consumed)
            if int(c_try.sum()) > 0 and (best_coh is None
                                         or int(c_try.sum()) > int(best_coh.sum())):
                best_coh = c_try
                n_try = U[c_try].mean(axis=0)
                best_n = n_try / max(np.linalg.norm(n_try), EPS)
            if int(c_try.sum()) < min_coh:
                break
            n_hat = best_n
        coh, n_hat = best_coh, best_n
        if coh is None or int(coh.sum()) < min_coh:
            work[np.abs(grid - two_d0) <= 1.5 * tol] = -np.inf
            continue
        m_coh = int(coh.sum())
        R_len = float(np.abs(U[coh] @ n_hat).mean())
        two_d = float(np.median(L[coh]))
        refl.append((two_d, n_hat, m_coh, R_len, float(work[k])))
        consumed |= coh
        work[np.abs(grid - two_d0) <= 1.5 * tol] = -np.inf
    return refl


def ransac3dof(X, dmax=90.0, tol=0.8, min_r=0.35, coh_deg=15.0, min_coh=3,
               n_dir=300):
    """BASELINE: 3-DOF consensus on the displacement-vector set.

    Identical statistic to rank1_consensus (excess resultant, coherent
    sub-set, median magnitude) but n_hat is SEARCHED over a Fibonacci sphere
    instead of estimated. This isolates the cost of the extra 2 DOF: same data,
    same tolerance, same inlier logic, ~n_dir x the search.
    """
    N = X.shape[0]
    if N < 2:
        return []
    iu, ju = np.triu_indices(N, k=1)
    D = X[iu] - X[ju]
    L = np.linalg.norm(D, axis=1)
    keep = (L > min_r) & (L < dmax)
    D, L = D[keep], L[keep]
    if L.size < min_coh:
        return []
    U = D / L[:, None]
    cos_coh = np.cos(np.radians(coh_deg))
    grid = np.arange(tol, dmax, 2.0 * tol)
    ga = np.pi * (3.0 - np.sqrt(5.0))
    i = np.arange(n_dir) + 0.5
    th = np.arccos(1 - 2 * i / n_dir)
    ph = ga * i
    dirs = np.stack([np.sin(th) * np.cos(ph), np.sin(th) * np.sin(ph), np.cos(th)], axis=1)
    best = (-np.inf, 0.0, None, 0, 0.0)
    for n_hat in dirs:
        al = U @ n_hat
        for two_d in grid:
            sel = (np.abs(L - two_d) <= tol) & (al >= cos_coh)
            m = int(sel.sum())
            if m < min_coh:
                continue
            S = U[sel].sum(axis=0)
            e = float(np.linalg.norm(S)) - 0.7 * np.sqrt(m)
            if e > best[0]:
                best = (e, float(np.median(L[sel])), n_hat, m,
                        float(np.abs(U[sel] @ n_hat).mean()))
    e, two_d, n_hat, m, rl = best
    if m < min_coh or e < 1.0:
        return []
    return [(two_d, n_hat, m, rl, e)]


def classify_pairs(X, reflector, tol=0.8, coh_deg=15.0):
    """(parent, child) index pairs implied by one reflector. Assignment-free.

    Requires BOTH the magnitude invariant (|Delta| = 2d) and the direction
    invariant (Delta || n_hat). Both are needed: magnitude alone is ambiguous
    among the O(N^2) pairs, direction alone is trivially satisfied by a lane
    mate. The conjunction is what makes the test assignment-free.
    """
    two_d, n_hat = reflector[0], reflector[1]
    N = X.shape[0]
    iu, ju = np.triu_indices(N, k=1)
    D = X[iu] - X[ju]
    L = np.linalg.norm(D, axis=1)
    sel = (np.abs(L - two_d) <= tol) & ((D / np.maximum(L, EPS)[:, None]) @ n_hat
                                        >= np.cos(np.radians(coh_deg)))
    out = []
    for k in np.nonzero(sel)[0]:
        a, b = int(iu[k]), int(ju[k])
        # orient: displacement p_a - p_b along +n_hat
        if float((X[a] - X[b]) @ n_hat) >= 0.0:
            out.append((a, b))
        else:
            out.append((b, a))
    return out


def bearing_gate(X, half_deg=2.0, dmax=90.0):
    """Baseline: the field's generic ghost gate - a bearing-window coincidence.

    Flags any two detections whose bearings agree within half_deg. No geometry.
    """
    az = np.degrees(np.arctan2(X[:, 1], X[:, 0]))
    N = X.shape[0]
    out = []
    for a in range(N):
        for b in range(a + 1, N):
            if abs(az[a] - az[b]) <= half_deg:
                out.append((a, b))
    return out


# ------------------------------------------------------------- scene + sim
def make_scene(seed, n_tgt=6, d_primary=3.0, curved=False, decoy_pair=False):
    """Urban-canyon-ish scene: left guardrail plane (y = d) + front targets.

    Detection layout is BLOCKED so the truth index map is trivial:
        [0 .. n-1]                  real targets (parents)
        [n + k*n + i]               ghost of target i off reflector k
    """
    rng = np.random.default_rng(seed)
    reflectors = [(d_primary, np.array([0.0, 1.0, 0.0]))]
    if curved:
        reflectors.append((6.0, np.array([0.6, 0.8, 0.0])))   # bend / second wall
    truths, real = [], []
    for _ in range(n_tgt):
        r = rng.uniform(12.0, 60.0)
        az = np.radians(rng.uniform(6.0, 55.0))                # front-left
        p = np.array([r * np.cos(az), r * np.sin(az), rng.uniform(-0.3, 0.3)])
        v = np.array([rng.uniform(-5, 15), rng.uniform(-6, 6), 0.0])
        truths.append((p.copy(), v))
        real.append(p.copy())
    n = len(real)
    if decoy_pair:
        # two INDEPENDENT targets, 2*d apart laterally, same range: the
        # adversarial case where a real pair mimics a specular pair.
        for sgn in (+1.0, -1.0):
            p = np.array([35.0, sgn * d_primary, 0.0])
            truths.append((p.copy(), np.zeros(3)))
            real.append(p.copy())
        n = len(real)
    X = list(real)
    for d, nv in reflectors:
        for p in real:
            X.append(plane_specular(p, d, nv)[2])
    return truths, reflectors, np.array(X), rng


def truth_edge_set(n_real, n_refl):
    """(parent, child) index pairs the physics guarantees."""
    return {(i, n_real + k * n_real + i) for k in range(n_refl) for i in range(n_real)}


def observe(X_true, rng):
    """Add range noise on |p| and direction noise on the bearing."""
    X = X_true.copy()
    for i in range(X.shape[0]):
        u = X[i] / max(np.linalg.norm(X[i]), EPS)
        r = np.linalg.norm(X[i])
        # direction noise: small random rotation
        u = u + rng.normal(0.0, np.radians(0.35), 3)
        u /= max(np.linalg.norm(u), EPS)
        X[i] = (r + rng.normal(0.0, SIG_R)) * u
    return X


def e2e_consensus(seed, curved=False, decoy_pair=False, n_tgt=10):
    """Per-scan reflector ID + ghost-pair extraction accuracy, ours vs 3-DOF."""
    truths, reflectors, X_true, rng = make_scene(
        seed, n_tgt=n_tgt, curved=curved, decoy_pair=decoy_pair)
    n_real = len(truths)
    X_obs = observe(X_true, rng)
    truth_edges = truth_edge_set(n_real, len(reflectors))

    def evaluate(finder):
        found, d_errs, rl = set(), [], []
        modes = list(finder(X_obs))
        for refl in modes:
            two_d, n_hat = refl[0], refl[1]
            rl.append(refl[3])
            found.update(classify_pairs(X_obs, refl))
        # identifiability metric: error of the BEST-matching mode only
        if modes:
            d_errs = [min(abs(m[0] - 2.0 * d) for d, _ in reflectors) for m in modes]
            d_errs = [min(d_errs)]
        inter = len(truth_edges & found)
        de = [v for v in d_errs if np.isfinite(v)]
        return {
            "recall": inter / max(len(truth_edges), 1),
            "precision": inter / max(len(found), 1),
            "n_refl_found": len(modes), "n_refl_true": len(reflectors),
            "detect_rate": 1.0 if modes else 0.0,
            "two_d_err_m": float(np.mean(de)) if de else float("nan"),
            "resultant_len": float(np.mean(rl)) if rl else 0.0,
            "n_found": len(found),
        }

    ours = evaluate(rank1_consensus)
    base = evaluate(ransac3dof)
    bg = set(bearing_gate(X_obs))
    bg_inter = len(truth_edges & bg)
    return {
        "n_det": X_obs.shape[0], "n_truth_edges": len(truth_edges),
        "ours": ours, "ransac3dof": base,
        "bg_pairs": len(bg), "bg_true_hits": bg_inter,
        "bg_precision": bg_inter / max(len(bg), 1),
        "bg_recall": bg_inter / max(len(truth_edges), 1),
    }


# ------------------------------------------------------------- E2d EKF
def ekf_filter(truths, reflectors, xi_err=(0.0, 0.0), mode="dual", n_steps=25,
               dt=0.05, seed=0):
    """Constant-velocity EKF on (p, v). mode: direct | dual_naive | dual.

    xi_err = (sigma_d_m, sigma_n_rad) uncertainty of the identified reflector.
    """
    rng = np.random.default_rng(1000 + seed)
    n_t = len(truths)
    X = np.zeros((6, 1))
    x0 = truths[0][0] + rng.normal(0.0, 0.5, 3)
    X[:3, 0] = x0
    X[3:, 0] = np.array([12.0, 0.0, 0.0])
    P = np.diag([1.0, 1.0, 1.0, 25.0, 25.0, 25.0])
    Q = np.diag([0.01, 0.01, 0.01, 0.5, 0.5, 0.5])
    p0, v0 = truths[0]
    pos_err = []
    for k in range(n_steps):
        # ---- propagate truth
        t = k * dt
        p_true = p0 + v0 * t
        v_true = v0
        if k > 0:
            F = np.eye(6)
            F[:3, 3:] = np.eye(3) * dt
            X = F @ X
            P = F @ P @ F.T + Q
        # ---- measurements
        # ---- measurement model, re-evaluated at the current estimate
        v_true = v0
        v_est = X[3:, 0].copy()
        r_d, u_d = direct_obs(p_true)
        vr_d = float(u_d @ v_true)
        z_d = [r_d + rng.normal(0.0, SIG_R), vr_d + rng.normal(0.0, SIG_V)]
        ghost = None
        n_hat = None
        d_hat = 0.0
        Jxi = None
        Sig_xi = None
        if mode != "direct":
            d_hat = reflectors[0][0] + rng.normal(0.0, xi_err[0])
            n_hat = reflectors[0][1].copy()
            th = rng.normal(0.0, xi_err[1])
            c, s = np.cos(th), np.sin(th)
            n_hat = np.array([n_hat[0] * c - n_hat[2] * s, n_hat[1], n_hat[0] * s + n_hat[2] * c])
            r_s, u_s, _ = plane_specular(p_true, d_hat, n_hat)
            vr_s = float(u_s @ v_true)
            z_s = [r_s + rng.normal(0.0, SIG_R), vr_s + rng.normal(0.0, SIG_V)]
            ghost = (d_hat, n_hat, z_s)
        R_sig = SIG_R ** 2 if mode == "direct" else np.diag(
            [SIG_R ** 2, SIG_V ** 2] * 2)
        Jxi = None
        if mode == "dual":
            Jxi = np.zeros((4, 4))
            Sig_xi = np.zeros((4, 4))
            Sig_xi[0, 0] = xi_err[0] ** 2
            Sig_xi[1:, 1:] = xi_err[1] ** 2 * (np.eye(3) - np.outer(n_hat, n_hat))

        # ITERATED EKF update with EXACT residuals. Two traps here, both found
        # numerically: (1) r_s = u_s.(p - 2d n) is NOT u_s.p - the constant
        # 2d(n.u_s) (= 0.79 m for the nominal scene, 8 sigma of SIG_R) must be
        # carried, or the filter chases a constant bias; (2) dv_r/dp is
        # ~0.4/m, so with a poor velocity prior one Gauss-Newton step turns a
        # 1 m position error into a ~4 sigma spurious Doppler innovation.
        # Exact residuals + iteration remove both.
        for _ in range(3):
            p_e, v_e = X[:3, 0].copy(), X[3:, 0].copy()
            r_dm, u_dm = direct_obs(p_e)
            H = np.zeros((2, 6))
            H[0, :3] = u_dm
            H[1, :3] = (v_e - float(u_dm @ v_e) * u_dm) / max(r_dm, EPS)
            H[1, 3:] = u_dm
            resid = [z_d[0] - r_dm, z_d[1] - float(u_dm @ v_e)]
            R = np.diag([SIG_R ** 2, SIG_V ** 2])
            if ghost is not None:
                d_hat, n_hat2, z_s = ghost
                q = p_e - 2.0 * d_hat * n_hat2
                r_sm = float(np.linalg.norm(q))
                u_sm = q / max(r_sm, EPS)
                vr_sm = float(u_sm @ v_e)
                H = np.vstack([H, np.zeros((2, 6))])
                H[2, :3] = u_sm
                H[3, :3] = (v_e - vr_sm * u_sm) / max(r_sm, EPS)
                H[3, 3:] = u_sm
                resid = resid + [z_s[0] - r_sm, z_s[1] - vr_sm]
                R = np.diag([SIG_R ** 2, SIG_V ** 2] * 2)
                if mode == "dual":
                    Jx = Jxi.copy()
                    Jx[2, 0] = -2.0 * float(n_hat2 @ u_sm)
                    Jx[2, 1:] = -2.0 * d_hat * u_sm
                    Jx[3, 0] = 2.0 * (vr_sm * float(n_hat2 @ u_sm)
                                      - float(v_e @ n_hat2)) / max(r_sm, EPS)
                    Jx[3, 1:] = 2.0 * d_hat * (vr_sm * u_sm - v_e) / max(r_sm, EPS)
                    R = R + Jx @ Sig_xi @ Jx.T
            innov = np.array(resid).reshape(-1, 1)
            S = H @ P @ H.T + R
            K = P @ H.T @ np.linalg.inv(S)
            X = X + K @ innov
            P = P - K @ S @ K.T
            P = 0.5 * (P + P.T)                  # Joseph-grade symmetrisation
        pos_err.append(float(np.linalg.norm(X[:3, 0] - p_true)))
    return float(np.mean(pos_err[-5:])), float(np.mean(pos_err))


def e2d_assimilation(seeds=range(20), xi_err=(0.15, np.radians(2.0))):
    """E2d: dual-path assimilation gain, and the price of naive diag(R)."""
    out = {}
    for mode in ("direct", "dual_naive", "dual"):
        errs = []
        for s in seeds:
            truths, reflectors, _, _ = make_scene(s, n_tgt=1, d_primary=3.0)
            errs.append(ekf_filter(truths[:1], reflectors, xi_err, mode, seed=s)[1])
        out[mode] = float(np.mean(errs))
    return out, xi_err


# ------------------------------------------------------------------ demo
def demo():
    # --- E2a: rank-1 exactness + naive-model fault
    rng = np.random.default_rng(0)
    errs = []
    for _ in range(2000):
        p = rng.normal(0.0, 30.0, 3)
        if np.linalg.norm(p) < 5.0:
            continue
        n = rng.normal(size=3)
        n /= np.linalg.norm(n)
        d = rng.uniform(0.5, 12.0)
        _, _, p_s = plane_specular(p, d, n)
        errs.append(abs(np.linalg.norm(p - p_s) - 2.0 * d))
    rank1_max_err = max(errs)
    f = fault_naive_model()
    print(f"[E2a] rank-1 |p-p_s|=2d : max abs err over {len(errs)} samples = "
          f"{rank1_max_err:.3e}")
    print(f"[E2a] naive mirror-image bearing {f['az_naive_deg']:.3f}deg vs true "
          f"{f['az_spec_deg']:.3f}deg  -> {f['bearing_err_deg']:.3f}deg error, "
          f"{f['induced_pos_err_m']:.3f} m at {f['r_spec']:.1f} m")
    print(f"[E2a] naive model range error = {f['range_err_m']:.3e} m "
          f"(range-exact, bearing-wrong => half-correct failure)")
    assert rank1_max_err < 1e-9, "rank-1 displacement theorem violated"
    assert f["range_err_m"] < 1e-9 and f["bearing_err_deg"] > 1.0, \
        "expected range-exact but bearing-wrong naive model"

    # --- E2b: Doppler fault
    g = fault_doppler()
    print(f"[E2b] ghost Doppler {g['vr_spec']:.3f} vs parent {g['vr_direct']:.3f} "
          f"m/s (offset {g['offset']:.3f}); FD check err {g['fd_err']:.2e}; "
          f"same_bin={g['same_bin']}; decodable at "
          f"{g['separation_sigma']:.0f} sigma")
    assert g["fd_err"] < 1e-6
    assert abs(g["offset"] - g["offset_closed_form"]) < 1e-9
    # the Doppler invariant is the disambiguator that the purely geometric
    # test provably lacks (see the decoy case in E2c)
    assert g["separation_sigma"] > 10.0

    # --- E2c: 1-DOF reflector ID vs 3-DOF consensus, planar/curved/decoy
    for label, kw in (("planar", {}), ("curved", {"curved": True}),
                      ("decoy", {"decoy_pair": True})):
        rs = [e2e_consensus(s, **kw) for s in range(7, 47)]
        keys = ("recall", "precision", "n_refl_found", "two_d_err_m",
                "resultant_len", "detect_rate")
        o = {k: float(np.nanmean([r["ours"][k] for r in rs])) for k in keys}
        b = {k: float(np.nanmean([r["ransac3dof"][k] for r in rs])) for k in keys}
        print(f"[E2c/{label:6s}] ndet={rs[0]['n_det']:2d}  1-DOF  "
              f"recall={o['recall']:.3f} prec={o['precision']:.3f} "
              f"det={o['detect_rate']:.2f} refl={o['n_refl_found']:.2f} "
              f"2d_err={o['two_d_err_m']:.3f}m R={o['resultant_len']:.3f}")
        print(f"[E2c/{label:6s}]             3-DOF  "
              f"recall={b['recall']:.3f} prec={b['precision']:.3f} "
              f"det={b['detect_rate']:.2f} refl={b['n_refl_found']:.2f} "
              f"2d_err={b['two_d_err_m']:.3f}m")
    planar = [e2e_consensus(s) for s in range(7, 47)]
    p_rec = float(np.nanmean([r["ours"]["recall"] for r in planar]))
    p_pre = float(np.nanmean([r["ours"]["precision"] for r in planar]))
    b_rec = float(np.nanmean([r["ransac3dof"]["recall"] for r in planar]))
    b_pre = float(np.nanmean([r["ransac3dof"]["precision"] for r in planar]))
    p_2d = float(np.nanmean([r["ours"]["two_d_err_m"] for r in planar]))
    # NEGATIVE RESULT, deliberately asserted: the DOF-reduction claim is
    # REFUTED by our own baseline. Committing to a direction ESTIMATE before
    # testing direction leaves the coherent set contaminated by the random
    # pairs inside the magnitude window; searching the direction is what makes
    # it clean. Reported, not hidden.
    print(f"[E2c] VERDICT 1-DOF vs 3-DOF on planar: recall {p_rec:.3f} vs "
          f"{b_rec:.3f}, precision {p_pre:.3f} vs {b_pre:.3f} "
          f"-> DOF reduction costs accuracy; 3-DOF search WINS")
    assert p_rec > 0.80 and p_pre > 0.70
    assert p_2d < 0.70, "dominant reflector must still be recovered (0.3 m in d)"
    assert b_rec > p_rec, "baseline should dominate (recorded negative result)"

    # --- E2e: curved guardrail kills the constant-2d spike
    c = fault_curved_guardrail()
    print(f"[E2e] cylinder: 2D sweeps [{c['two_d_min_m']:.2f},{c['two_d_max_m']:.2f}] m, "
          f"band {c['band_width_m']:.2f} m = {c['ratio_to_range_noise']:.1f}x range noise")
    assert c["band_width_m"] > 10 * SIG_R, "expected the 1-D spike to smear"

    # --- E2d: assimilation + the xi-sensitivity that decides it
    for xi in ((0.0, 0.0), (0.05, np.radians(0.5)), (0.15, np.radians(2.0))):
        res, _ = e2d_assimilation(seeds=range(12), xi_err=xi)
        print(f"[E2d] xi_err=(d {xi[0]:.2f} m, n {np.degrees(xi[1]):.1f}deg)  "
              f"pos RMSE: direct={res['direct']:.3f} m  dual_naive="
              f"{res['dual_naive']:.3f} m  dual(R_eff)={res['dual']:.3f} m  "
              f"coupling gain={100.0 * (res['dual_naive'] - res['dual']) / res['dual_naive']:+.1f}%")
        if xi[0] == 0.0:
            # exact geometry: the ghost is a genuine virtual aperture and MUST help
            assert res["dual"] < res["direct"], \
                "with exact reflector geometry the ghost must add information"
            assert res["dual"] <= res["dual_naive"] + 1e-9, "R_eff == diag at exact xi"
        else:
            # uncertain geometry: R_eff must strictly dominate naive diag(R),
            # and the ghost must be de-weighted toward direct-only rather than
            # trusted. Both orderings are the theorem's content.
            assert res["dual"] < res["dual_naive"], \
                "R_eff must dominate naive diag(R) under geometry uncertainty"

    print("demo PASS")


if __name__ == "__main__":
    demo()
