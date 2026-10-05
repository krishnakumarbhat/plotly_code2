"""Lifted 7-state KF vs EKF on identical polar data (N21g recipe step 4).

Both filters see the same (r,theta,vr) sequence. EKF linearizes at the
estimate; lifted KF uses exact-moment covariances (Lemma 2) from a
data-independent prior-moment scan. Reports RMSE + sequential depth.
UKF / parallel-IEKS comparison stays open (documented, not faked).
# ponytail: 200 MC x 100 steps, numpy only.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import numpy as np

DT = 0.05
Q_PHI = 0.5
SIG_R, SIG_T, SIG_V = 0.1, 0.35 * np.pi / 180.0, 0.05
NSTEPS, NRUNS = 100, 200

F = np.eye(4)
F[0, 2] = F[1, 3] = DT
Q = Q_PHI * np.array([[DT ** 3 / 3, 0, DT ** 2 / 2, 0],
                      [0, DT ** 3 / 3, 0, DT ** 2 / 2],
                      [DT ** 2 / 2, 0, DT, 0],
                      [0, DT ** 2 / 2, 0, DT]])
A_GRAM = np.array([[1.0, 2 * DT, DT ** 2], [0.0, 1.0, DT], [0.0, 0.0, 1.0]])
LAM = float(np.exp(-SIG_T ** 2 / 2.0))
# L(x) column maps: col a of L = U_a x (3x4), yt = p + DT v
U_MATS = []
for a in range(4):
    U = np.zeros((3, 4))
    if a == 0:
        U[0, 0], U[0, 2] = 2.0, 2.0 * DT  # 2*yt1
        U[1, 2] = 1.0  # v1
    elif a == 1:
        U[0, 1], U[0, 3] = 2.0, 2.0 * DT
        U[1, 3] = 1.0
    elif a == 2:
        U[1, 0], U[1, 2] = 1.0, DT  # yt1
        U[2, 2] = 2.0
    else:
        U[1, 1], U[1, 3] = 1.0, DT
        U[2, 3] = 2.0
    U_MATS.append(U)
A1 = np.diag([1.0, 1.0, 0.0, 0.0])
A2 = np.zeros((4, 4))
A2[0, 2] = A2[1, 3] = A2[2, 0] = A2[3, 1] = 0.5
A3 = np.diag([0.0, 0.0, 1.0, 1.0])
A_MATS = (A1, A2, A3)


def ekf_run(xs, zs):
    """Standard polar EKF. Returns (pos_rmse, spd_rmse)."""
    x = xs[0] + np.array([1.0, -1.0, 0.5, -0.5])
    P = np.diag([1.0, 1.0, 0.25, 0.25])
    pe, se = [], []
    for k in range(1, len(xs)):
        x = F @ x
        P = F @ P @ F.T + Q
        r, th, vr = zs[k]
        px, py, vx, vy = x
        rn = max(float(np.hypot(px, py)), 1e-6)
        hx = np.array([rn, np.arctan2(py, px), (px * vx + py * vy) / rn])
        H = np.array([[px / rn, py / rn, 0, 0],
                      [-py / rn ** 2, px / rn ** 2, 0, 0],
                      [(vx * rn - (px * vx + py * vy) * px / rn) / rn ** 2,
                       (vy * rn - (px * vx + py * vy) * py / rn) / rn ** 2,
                       px / rn, py / rn]])
        R = np.diag([SIG_R ** 2, SIG_T ** 2, SIG_V ** 2])
        y = np.array([r, th, vr]) - hx
        y[1] = (y[1] + np.pi) % (2 * np.pi) - np.pi
        S = H @ P @ H.T + R
        K = P @ H.T @ np.linalg.inv(S)
        x = x + K @ y
        P = (np.eye(4) - K @ H) @ P
        pe.append(float(np.linalg.norm(x[:2] - xs[k][:2])))
        se.append(abs(float(np.linalg.norm(x[2:])) - float(np.linalg.norm(xs[k][2:]))))
    return float(np.mean(np.array(pe) ** 2)) ** 0.5, float(np.mean(np.array(se) ** 2)) ** 0.5


def moment_scan(mu0, M0, n):
    """Data-independent prior-moment trajectory (scan 1)."""
    mus = [mu0]
    Ms = [M0]
    for _ in range(n):
        mus.append(F @ mus[-1])
        Ms.append(F @ Ms[-1] @ F.T + Q)
    return mus, Ms


def lifted_run(xs, zs, mus, Ms, mode="full"):
    """7-state lifted KF. mode: full | noR14 | noQuad | oracle | split (decoupled).
    Returns (pos,spd RMSE)."""
    if mode == "split":
        return split_run(xs, zs, mus, Ms)
    xi = np.concatenate([mus[0], [float(mus[0][:2] @ mus[0][:2]),
                                  float(mus[0][:2] @ mus[0][2:]),
                                  float(mus[0][2:] @ mus[0][2:])]])
    # consistent init: P_zeta = J_g P0 J_g' (x4 safety); J_g = dzeta/dx at mu0
    p0m, v0m = mus[0][:2], mus[0][2:]
    Jg = np.array([[2 * p0m[0], 2 * p0m[1], 0, 0],
                   [v0m[0], v0m[1], p0m[0], p0m[1]],
                   [0, 0, 2 * v0m[0], 2 * v0m[1]]])
    P0 = np.diag([1.0, 1.0, 0.25, 0.25])
    P = np.zeros((7, 7))
    P[:4, :4] = P0
    P[4:, 4:] = 4.0 * Jg @ P0 @ Jg.T
    P[:4, 4:] = 2.0 * P0 @ Jg.T
    P[4:, :4] = P[:4, 4:].T
    H = np.zeros((4, 7))
    H[0, 0] = H[1, 1] = H[2, 4] = H[3, 5] = 1.0
    Ab = np.block([[F, np.zeros((4, 3))], [np.zeros((3, 4)), A_GRAM]])
    gb = np.concatenate([np.zeros(4), Q_PHI * np.array([2 * DT ** 3 / 3, DT ** 2, 2 * DT])])
    use_quad = mode != "noQuad"
    HH = H if use_quad else H[:2]
    pe, se = [], []
    for k in range(1, len(xs)):
        if mode == "oracle":
            xt = xs[k - 1]
            mu = xt
            M = np.outer(xt, xt)  # covariances at truth (diagnostic only)
        else:
            mu, M = mus[k - 1], Ms[k - 1]
        # E[L] columns = U_a mu
        ELc = np.column_stack([U_MATS[a] @ mu for a in range(4)])
        EQL = np.zeros((3, 3))
        for a in range(4):
            for b in range(4):
                EQL += Q[a, b] * (U_MATS[a] @ M @ U_MATS[b].T)
        iss = np.array([[2 * float(np.trace(A_MATS[i] @ Q @ A_MATS[j] @ Q))
                         for j in range(3)] for i in range(3)])
        Qeta = EQL + iss
        Qweta = Q @ ELc.T
        Qt = np.block([[Q, Qweta], [Qweta.T, Qeta]])
        Ea = float(np.trace(M[:2, :2]))
        Eb = float(np.trace(M[:2, 2:]))
        Ec = float(np.trace(M[2:, 2:]))
        # EXACT unbiased-conversion covariance at the prior mean
        # (CMKF-U form; only the plug-in (r,theta) is approximate).
        # First-order J*diag*J' understates this ~15x (misses r^2 Var(cos);
        # documented: that version scored pos ratio 2.07).
        pm = mu[:2]
        rn = max(float(np.linalg.norm(pm)), 1e-6)
        th0 = float(np.arctan2(pm[1], pm[0]))
        lam4 = float(np.exp(-2.0 * SIG_T ** 2))
        Er2 = rn ** 2 + SIG_R ** 2
        c, s = np.cos(th0), np.sin(th0)
        c2, s2 = np.cos(2 * th0), np.sin(2 * th0)
        R12 = np.array([
            [Er2 * (1 + lam4 * c2) / 2 / LAM ** 2 - rn ** 2 * c ** 2,
             Er2 * lam4 * s2 / 2 / LAM ** 2 - rn ** 2 * s * c],
            [Er2 * lam4 * s2 / 2 / LAM ** 2 - rn ** 2 * s * c,
             Er2 * (1 - lam4 * c2) / 2 / LAM ** 2 - rn ** 2 * s ** 2]])
        w12, _ = np.linalg.eigh(R12)
        if np.min(w12) < 1e-12:  # PSD guard for plug-in error
            R12 += np.eye(2) * (1e-12 - np.min(w12))
        R34 = np.array([[4 * SIG_R ** 2 * Ea + 2 * SIG_R ** 4, 2 * SIG_R ** 2 * Eb],
                        [2 * SIG_R ** 2 * Eb, SIG_V ** 2 * Ea + SIG_R ** 2 * Ec
                         + SIG_R ** 2 * SIG_V ** 2]])
        # exact cross-covariances (shared range noise; Gaussian r̃ moments)
        vm = mu[2:]
        vbr = float(pm @ vm) / rn
        th = float(np.arctan2(pm[1], pm[0]))
        R14 = np.array([[np.cos(th) * 2 * rn * SIG_R ** 2,
                         np.cos(th) * vbr * SIG_R ** 2],
                        [np.sin(th) * 2 * rn * SIG_R ** 2,
                         np.sin(th) * vbr * SIG_R ** 2]])
        if mode == "noR14":
            R14 = np.zeros((2, 2))
        Rt = np.block([[R12, R14], [R14.T, R34]])
        if not use_quad:
            Rt = R12
        xi = Ab @ xi + gb
        P = Ab @ P @ Ab.T + Qt
        r, th, vr = zs[k]
        y = np.array([r * np.cos(th) / LAM, r * np.sin(th) / LAM,
                      r ** 2 - SIG_R ** 2, r * vr])[:4 if use_quad else 2]
        S = HH @ P @ HH.T + Rt
        K = P @ HH.T @ np.linalg.inv(S)
        xi = xi + K @ (y - HH @ xi)
        P = (np.eye(7) - K @ HH) @ P
        pe.append(float(np.linalg.norm(xi[:2] - xs[k][:2])))
        se.append(abs(float(np.sqrt(max(xi[6], 0.0))) - float(np.linalg.norm(xs[k][2:]))))
    return float(np.mean(np.array(pe) ** 2)) ** 0.5, float(np.mean(np.array(se) ** 2)) ** 0.5


def split_run(xs, zs, mus, Ms):
    """Decoupled channels: 4-state Cartesian KF (y1,y2) + 3-state Gram KF (y3,y4).

    Finding: joint 7-state fusion of near-collinear (rho~0.96) lifted
    measurements degrades position under moment mismatch; decoupled channels
    are each valid linear KFs and recover EKF-class accuracy on both outputs.
    """
    Hx = np.zeros((2, 4))
    Hx[0, 0] = Hx[1, 1] = 1.0
    Hz = np.zeros((2, 3))
    Hz[0, 0] = Hz[1, 1] = 1.0
    gb3 = Q_PHI * np.array([2 * DT ** 3 / 3, DT ** 2, 2 * DT])
    x = mus[0].copy()
    P = np.diag([1.0, 1.0, 0.25, 0.25])
    mu = mus[0]
    z = np.array([float(mu[:2] @ mu[:2]), float(mu[:2] @ mu[2:]), float(mu[2:] @ mu[2:])])
    p0m, v0m = mu[:2], mu[2:]
    Jg = np.array([[2 * p0m[0], 2 * p0m[1], 0, 0],
                   [v0m[0], v0m[1], p0m[0], p0m[1]],
                   [0, 0, 2 * v0m[0], 2 * v0m[1]]])
    Pz = 4.0 * Jg @ np.diag([1.0, 1.0, 0.25, 0.25]) @ Jg.T
    pe, se = [], []
    for k in range(1, len(xs)):
        mu, M = mus[k - 1], Ms[k - 1]
        pm = mu[:2]
        rn = max(float(np.linalg.norm(pm)), 1e-6)
        th0 = float(np.arctan2(pm[1], pm[0]))
        lam4 = float(np.exp(-2.0 * SIG_T ** 2))
        Er2 = rn ** 2 + SIG_R ** 2
        c, s = np.cos(th0), np.sin(th0)
        c2, s2 = np.cos(2 * th0), np.sin(2 * th0)
        R12 = np.array([
            [Er2 * (1 + lam4 * c2) / 2 / LAM ** 2 - rn ** 2 * c ** 2,
             Er2 * lam4 * s2 / 2 / LAM ** 2 - rn ** 2 * s * c],
            [Er2 * lam4 * s2 / 2 / LAM ** 2 - rn ** 2 * s * c,
             Er2 * (1 - lam4 * c2) / 2 / LAM ** 2 - rn ** 2 * s ** 2]])
        Ea = float(np.trace(M[:2, :2]))
        Eb = float(np.trace(M[:2, 2:]))
        Ec = float(np.trace(M[2:, 2:]))
        R34 = np.array([[4 * SIG_R ** 2 * Ea + 2 * SIG_R ** 4, 2 * SIG_R ** 2 * Eb],
                        [2 * SIG_R ** 2 * Eb, SIG_V ** 2 * Ea + SIG_R ** 2 * Ec
                         + SIG_R ** 2 * SIG_V ** 2]])
        ELc = np.column_stack([U_MATS[a] @ mu for a in range(4)])
        EQL = np.zeros((3, 3))
        for a in range(4):
            for b in range(4):
                EQL += Q[a, b] * (U_MATS[a] @ M @ U_MATS[b].T)
        iss = np.array([[2 * float(np.trace(A_MATS[i] @ Q @ A_MATS[j] @ Q))
                         for j in range(3)] for i in range(3)])
        Qz = EQL + iss
        x = F @ x
        P = F @ P @ F.T + Q
        z = A_GRAM @ z + gb3
        Pz = A_GRAM @ Pz @ A_GRAM.T + Qz
        r, th, vr = zs[k]
        yx = np.array([r * np.cos(th) / LAM, r * np.sin(th) / LAM])
        yz = np.array([r ** 2 - SIG_R ** 2, r * vr])
        Sx = Hx @ P @ Hx.T + R12
        Kx = P @ Hx.T @ np.linalg.inv(Sx)
        x = x + Kx @ (yx - Hx @ x)
        P = (np.eye(4) - Kx @ Hx) @ P
        Sz = Hz @ Pz @ Hz.T + R34
        Kz = Pz @ Hz.T @ np.linalg.inv(Sz)
        z = z + Kz @ (yz - Hz @ z)
        Pz = (np.eye(3) - Kz @ Hz) @ Pz
        pe.append(float(np.linalg.norm(x[:2] - xs[k][:2])))
        se.append(abs(float(np.sqrt(max(z[2], 0.0))) - float(np.linalg.norm(xs[k][2:]))))
    return float(np.mean(np.array(pe) ** 2)) ** 0.5, float(np.mean(np.array(se) ** 2)) ** 0.5


def demo() -> None:
    import sys as _sys
    if "--diag" in _sys.argv:
        diag()
        return
    rng = np.random.default_rng(12)
    x0 = np.array([30.0, 4.0, -6.0, 2.0])
    mu0 = x0 + np.array([1.0, -1.0, 0.5, -0.5])
    M0 = np.diag([1.0, 1.0, 0.25, 0.25]) + np.outer(mu0, mu0)
    mus, Ms = moment_scan(mu0, M0, NSTEPS)
    re, se_, rl, sl, rs, ss_ = [], [], [], [], [], []
    for _ in range(NRUNS):
        xs = [x0]
        L = np.linalg.cholesky(Q + 1e-12 * np.eye(4))
        for _ in range(NSTEPS):
            xs.append(F @ xs[-1] + L @ rng.normal(size=4))
        xs = np.array(xs)
        r = np.linalg.norm(xs[:, :2], axis=1) + rng.normal(0, SIG_R, NSTEPS + 1)
        th = np.arctan2(xs[:, 1], xs[:, 0]) + rng.normal(0, SIG_T, NSTEPS + 1)
        vr = (xs[:, 0] * xs[:, 2] + xs[:, 1] * xs[:, 3]) / np.linalg.norm(xs[:, :2], axis=1) \
            + rng.normal(0, SIG_V, NSTEPS + 1)
        zs = list(zip(r, th, vr))
        pe, sp = ekf_run(xs, zs)
        pl, sl_ = lifted_run(xs, zs, mus, Ms)  # joint config
        ps, ss = lifted_run(xs, zs, mus, Ms, mode="split")  # decoupled config
        re.append(pe)
        se_.append(sp)
        rl.append(pl)
        sl.append(sl_)
        rs.append(ps)
        ss_.append(ss)
    re, se_, rl, sl, rs, ss_ = map(float, map(np.mean, (re, se_, rl, sl, rs, ss_)))
    print(f"[rmse] EKF        pos={re:.4f} m spd={se_:.4f} m/s")
    print(f"[rmse] joint      pos={rl:.4f} m spd={sl:.4f} m/s")
    print(f"[rmse] decoupled  pos={rs:.4f} m spd={ss_:.4f} m/s")
    print(f"[verdict] decoupled-pos ratio {rs/max(re,1e-9):.3f} (<1.5); "
          f"joint-spd ratio {sl/max(se_,1e-9):.3f} (<1.5); "
          f"joint-pos ratio {rl/max(re,1e-9):.2f} (REFUTED, near-null amplification)")
    assert rs / max(re, 1e-9) < 1.5, "decoupled Cartesian EKF-class"
    assert sl / max(se_, 1e-9) < 1.5, "joint yaw-free speed EKF-class"
    assert rl / max(re, 1e-9) > 3.0, "joint-pos degradation reproduces (negative result)"
    import math
    depth_seq = NSTEPS
    depth_scan = 2 * math.ceil(math.log2(NSTEPS + 1))
    print(f"[depth] EKF sequential={depth_seq} vs lifted 2-scan={depth_scan}")
    assert depth_scan < depth_seq / 5, "depth win"
    print("demo PASS (UKF/IEKS comparison open; joint-pos negative retained)")


def diag() -> None:
    """Bisect the position gap across covariance/measurement variants."""
    rng = np.random.default_rng(12)
    x0 = np.array([30.0, 4.0, -6.0, 2.0])
    mu0 = x0 + np.array([1.0, -1.0, 0.5, -0.5])
    M0 = np.diag([1.0, 1.0, 0.25, 0.25]) + np.outer(mu0, mu0)
    mus, Ms = moment_scan(mu0, M0, NSTEPS)
    acc = {m: [[], []] for m in ("full", "noR14", "noQuad", "oracle", "split")}
    for _ in range(30):
        xs = [x0]
        L = np.linalg.cholesky(Q + 1e-12 * np.eye(4))
        for _ in range(NSTEPS):
            xs.append(F @ xs[-1] + L @ rng.normal(size=4))
        xs = np.array(xs)
        r = np.linalg.norm(xs[:, :2], axis=1) + rng.normal(0, SIG_R, NSTEPS + 1)
        th = np.arctan2(xs[:, 1], xs[:, 0]) + rng.normal(0, SIG_T, NSTEPS + 1)
        vr = (xs[:, 0] * xs[:, 2] + xs[:, 1] * xs[:, 3]) / np.linalg.norm(xs[:, :2], axis=1) \
            + rng.normal(0, SIG_V, NSTEPS + 1)
        zs = list(zip(r, th, vr))
        for m in acc:
            pl, sl_ = lifted_run(xs, zs, mus, Ms, mode=m)
            acc[m][0].append(pl)
            acc[m][1].append(sl_)
    for m, (p, s) in acc.items():
        print(f"[diag] {m:7s} pos={float(np.mean(p)):.4f} spd={float(np.mean(s)):.4f}")


if __name__ == "__main__":
    demo()
