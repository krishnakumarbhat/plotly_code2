"""O(log N) associative prefix-scan Kalman filter (H / N8).

Bottleneck: replaying 300 logs x 20 workers is sequential O(N) per worker
because the Kalman filter is recursive. Sarkka's associative operator makes
filtering elements combinable in ANY order, so a Blelloch parallel prefix
scan recovers all filtering posteriors in O(log N) span:

    (Ai,bi,Ci,ni,Ji) x (Aj,bj,Cj,nj,Jj) = (Aij,bij,Cij,nij,Jij)
    Aij = Aj (I + Ci Jj)^-1 Ai
    bij = Aj (I + Ci Jj)^-1 (bi + Ci nj) + bj
    Cij = Aj (I + Ci Jj)^-1 Ci Aj' + Cj
    nij = Ai' (I + Jj Ci)^-1 (nj - Jj bi) + ni
    Jij = Ai' (I + Jj Ci)^-1 Jj Ai + Ji

Element construction (linear Gaussian, Särkkä TSP 2021 §III):
    dynamics x_k = F x_{k-1} + w, w ~ N(0,Q)  ->  A=F, b=0, C=Q
    measurement y_k, H, R                      ->  n=H'R^-1 y, J=H'R^-1 H
    prior N(m0,P0)                             ->  A=0, b=m0, C=P0, n=0, J=0
Posterior from a prefix element: P=(C^-1+J)^-1, m=P(C^-1 b + n).
Cross-validated against an independent sequential KF (not against itself).
# ponytail: time-invariant demo; time-varying F/H is the same operator.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import time

import numpy as np

I4 = np.eye(4)


def combine(e1, e2):
    """Associative binary operator. e = (A, b, C, n, J)."""
    A1, b1, C1, n1, J1 = e1
    A2, b2, C2, n2, J2 = e2
    n = A1.shape[0]
    M = np.linalg.inv(np.eye(n) + C1 @ J2)
    N = np.linalg.inv(np.eye(n) + J2 @ C1)
    A = A2 @ M @ A1
    b = A2 @ M @ (b1 + C1 @ n2) + b2
    C = A2 @ M @ C1 @ A2.T + C2
    n = A1.T @ N @ (n2 - J2 @ b1) + n1
    J = A1.T @ N @ J2 @ A1 + J1
    return (A, b, C, n, J)


IDENTITY = (np.eye(4), np.zeros(4), np.zeros((4, 4)), np.zeros(4),
            np.zeros((4, 4)))


def enorm(e):
    return sum(float(np.linalg.norm(x)) for x in e)


def posterior(e):
    """Filtering posterior (m, P) from a prefix element: the f-component
    of a prefix IS p(x_k|y_{1:k}) (Särkkä Thm 3); A=0 propagated from the
    k=1 element, so (b, C) are posterior mean/cov directly."""
    _, b, C, _, _ = e
    return b.copy(), C.copy()


def build_elements(Y, F, H, Q, R, m0, P0):
    """One filtering element per measurement (Lemma 7, Särkkä TSP 2021).

    f_k(x_k|x_{k-1}) = p(x_k|y_k,x_{k-1}): the element's conditional already
    assimilates y_k via a Q-only gain (NOT raw dynamics + raw information).
    g_k(x_{k-1}) = p(y_k|x_{k-1}) lives in x_{k-1} space (note the F^T)."""
    n = F.shape[0]
    els = []
    # k = 1: predict from prior, then update
    mbar = F @ m0
    Pbar = F @ P0 @ F.T + Q
    S = H @ Pbar @ H.T + R
    K = Pbar @ H.T @ np.linalg.inv(S)
    A = np.zeros((n, n))
    b = mbar + K @ (Y[0] - H @ mbar)
    C = Pbar - K @ S @ K.T
    Si = np.linalg.inv(S)
    HSi = H.T @ Si
    els.append((A, b, C, F.T @ HSi @ Y[0], F.T @ HSi @ H @ F))
    # k > 1: Q-only gain elements
    S = H @ Q @ H.T + R
    K = Q @ H.T @ np.linalg.inv(S)
    Si = np.linalg.inv(S)
    HSi = H.T @ Si
    IKHF = np.eye(n) - K @ H
    A = IKHF @ F
    C = IKHF @ Q
    FHSi = F.T @ HSi
    J = FHSi @ H @ F
    for y in Y[1:]:
        els.append((A.copy(), K @ y, C.copy(), FHSi @ y, J.copy()))
    return els


def sequential_kf(Y, F, H, Q, R, m0, P0):
    """Independent reference: textbook predict/update loop."""
    x, P = m0.copy(), P0.copy()
    out = []
    for y in Y:
        x = F @ x
        P = F @ P @ F.T + Q
        S = H @ P @ H.T + R
        K = P @ H.T @ np.linalg.inv(S)
        x = x + K @ (y - H @ x)
        P = P - K @ S @ K.T
        out.append((x.copy(), P.copy()))
    return out


def blelloch_scan(els):
    """Correct Blelloch scan (up-sweep + down-sweep), textbook indexing."""
    n = len(els)
    L = 1
    while L < n:
        L *= 2
    v = list(els) + [IDENTITY] * (L - n)
    # up-sweep: v[k+2d-1] = combine(v[k+d-1], v[k+2d-1])
    d = 1
    while d < L:
        for k in range(0, L, 2 * d):
            v[k + 2 * d - 1] = combine(v[k + d - 1], v[k + 2 * d - 1])
        d *= 2
    # down-sweep (order matters: a[k] <- a[k] x t, non-commutative)
    v[L - 1] = IDENTITY
    d = L // 2
    while d >= 1:
        for k in range(0, L, 2 * d):
            t = v[k + d - 1]
            v[k + d - 1] = v[k + 2 * d - 1]
            v[k + 2 * d - 1] = combine(v[k + 2 * d - 1], t)
        d //= 2
    # v now holds EXCLUSIVE prefixes; inclusive = combine(excl, els[i])
    return [combine(v[i], els[i]) for i in range(n)]


def benchmark(N, seed=0):
    """Wall-time: sequential KF loop vs Blelloch scan (batched NumPy).
    Returns (t_seq, t_scan, max_mean_err, levels)."""
    rng = np.random.default_rng(seed)
    dt = 0.05
    F = np.eye(4)
    F[0, 2] = F[1, 3] = dt
    H = np.zeros((2, 4))
    H[0, 0] = H[1, 1] = 1.0
    Q = np.diag([0.005, 0.005, 0.05, 0.05])
    R = np.diag([0.09, 0.0025])
    m0 = np.array([50.0, 3.5, -2.0, 0.0])
    P0 = np.diag([1.0, 1.0, 1.0, 1.0])
    x = np.array([50.0, 3.5, -2.0, 0.0])
    Y = []
    for _ in range(N):
        x = F @ x + rng.multivariate_normal(np.zeros(4), Q)
        Y.append(H @ x + rng.multivariate_normal(np.zeros(2), R))
    Y = np.array(Y)
    t0 = time.perf_counter()
    ref = sequential_kf(Y, F, H, Q, R, m0, P0)
    t_seq = time.perf_counter() - t0
    els = build_elements(Y, F, H, Q, R, m0, P0)
    t0 = time.perf_counter()
    # sequential REDUCTION baseline (same operator, list order) vs scan
    red = els[0]
    for e in els[1:]:
        red = combine(red, e)
    t_red = time.perf_counter() - t0
    t0 = time.perf_counter()
    pref = blelloch_scan(els)
    t_scan = time.perf_counter() - t0
    errs = [float(np.linalg.norm(posterior(p)[0] - r[0]))
            for p, r in zip(pref, ref)]
    # final-element cross-check vs sequential reduction
    mf, Pf = posterior(pref[-1])
    mr, Pr = posterior(red)
    assert np.linalg.norm(mf - mr) < 1e-9, "scan != reduction"
    levels = int(np.ceil(np.log2(len(els))))
    return t_seq, t_red, t_scan, max(errs), levels


def demo() -> None:
    rng = np.random.default_rng(0)
    # --- (1) associativity: RELATIVE error (absolute scales with element
    # conditioning, not algebra; float32 would break this -> HPCC must use
    # float64 for the scan even if the filter runs float32)
    worst_abs, worst_rel = 0.0, 0.0
    for _ in range(200):
        els = []
        for _ in range(3):
            M = rng.normal(size=(4, 4))
            els.append((M, rng.normal(size=4), M @ M.T + np.eye(4),
                        rng.normal(size=4),
                        (lambda A: A @ A.T)(rng.normal(size=(4, 4)))))
        l = combine(combine(els[0], els[1]), els[2])
        r = combine(els[0], combine(els[1], els[2]))
        num = enorm((l[0] - r[0], l[1] - r[1], l[2] - r[2], l[3] - r[3],
                     l[4] - r[4]))
        worst_abs = max(worst_abs, num)
        worst_rel = max(worst_rel, num / max(enorm(r), 1e-300))
    print(f"[assoc] max abs={worst_abs:.3e} rel={worst_rel:.3e} (bar 1e-14 rel)")
    assert worst_rel < 1e-14, "operator not associative"
    # --- (2) scan == sequential KF posteriors; (3) timing ladder
    print(f"{'N':>7} {'seqKF(s)':>9} {'seqRed(s)':>10} {'scan(s)':>9} "
          f"{'maxErr':>9} {'levels':>6}")
    for N in (1000, 5000, 10000, 50000):
        ts, tr, tc, err, lv = benchmark(N)
        print(f"{N:7d} {ts:9.3f} {tr:10.3f} {tc:9.3f} {err:9.2e} {lv:6d}")
        assert err < 1e-6, "scan posteriors diverge from sequential KF"
    print("demo PASS")


if __name__ == "__main__":
    demo()
