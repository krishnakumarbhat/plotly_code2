"""Closed-form specular contact inverse + curvature certificate (N26).

N26.1: t = (l^2-|d|^2)/2(l-u^T d); N26.2 sensitivities;
N26.3 curvature image bound; N26.5 curvature-free velocity solve.
# ponytail: one file, analytic fixtures + FD checks + noise ladder.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import numpy as np


def inverse_contact(s, p, u, ell):
    """N26.1 inverse foot, normal, target-side direction."""
    s = np.asarray(s, float)
    p = np.asarray(p, float)
    u = np.asarray(u, float) / float(np.linalg.norm(u))
    d = p - s
    B = ell - float(u @ d)
    assert ell > float(np.linalg.norm(d)) + 1e-12, "ell<=|d| degenerate"
    assert B > 1e-12, "denominator degenerate"
    t = (ell ** 2 - float(d @ d)) / (2.0 * B)
    assert 0.0 < t < ell, "foot outside segment"
    q = s + t * u
    rho = ell - t
    assert rho > 0, "rho<=0"
    a = (p - q) / rho
    z = a - u
    nz = float(np.linalg.norm(z))
    assert nz > 1e-12, "collinear a==u"
    return q, z / nz, a, t, rho, B


def demo() -> None:
    # 1. analytic fixture
    s = np.zeros(2)
    p = np.array([6.0, 0.0])
    u = np.array([0.6, 0.8])
    ell = 10.0
    q, n, a, t, rho, B = inverse_contact(s, p, u, ell)
    print(f"[fixture] t={t:.12f} q={q} n={n} a={a}")
    assert abs(t - 5.0) < 1e-12, "t"
    assert float(np.linalg.norm(q - np.array([3.0, 4.0]))) < 1e-12, "q"
    assert float(np.linalg.norm(n - np.array([0.0, -1.0]))) < 1e-12, "normal"
    assert float(np.linalg.norm(a - np.array([0.6, -0.8]))) < 1e-12, "a-ray"

    # 2. sensitivity check vs finite differences
    dt_dl = (ell - t) / B
    print(f"[sens] dt/dl={dt_dl:.6f} (expect 0.78125)")
    assert abs(dt_dl - 0.78125) < 1e-12, "dt/dl"
    grad_p = (q - p) / B
    assert float(np.linalg.norm(grad_p - np.array([-0.46875, 0.625]))) < 1e-12, "dt/dp"
    e = 1e-6
    fd = (inverse_contact(s, p, u, ell + e)[3] - t) / e
    assert abs(fd - dt_dl) / dt_dl < 1e-5, "FD dt/dl"

    # 3. curvature-free velocity (N26.5)
    vs = np.array([1.0, 0.0])
    vp = np.array([0.3, -0.2])
    d = p - s
    ud = d / float(np.linalg.norm(d))
    rd_dot = float(ud @ (vp - vs))
    lg_dot = float(a @ vp - u @ vs)
    print(f"[doppler] direct={rd_dot:.6f} (expect -0.7) ghost={lg_dot:.6f} (expect -0.26)")
    assert abs(rd_dot + 0.7) < 1e-12 and abs(lg_dot + 0.26) < 1e-12, "rates"
    M = np.vstack([ud, a])
    y = np.array([rd_dot + float(ud @ vs), lg_dot + float(u @ vs)])
    rec = np.linalg.solve(M, y)
    assert float(np.linalg.norm(rec - vp)) < 1e-12, "velocity solve"
    det = abs(ud[0] * a[1] - ud[1] * a[0])
    print(f"[doppler] |ud x a|={det:.4f} cond={np.linalg.cond(M):.3f}")

    # 4. circle radius fixture (N26.4)
    q1 = np.array([3.0, 4.0])
    n1 = np.array([0.0, -1.0])
    q2 = np.array([-9.0, 8.0])
    n2 = np.array([-0.6, -0.8])
    dq, dn = q2 - q1, n2 - n1
    R = float(dq @ dn) / float(dn @ dn)
    c0 = q1 - R * n1
    print(f"[circle] R={R:.12f} c={c0}")
    assert abs(R - 20.0) < 1e-12 and float(np.linalg.norm(c0 - np.array([3.0, 24.0]))) < 1e-9, "R"

    # 5. noise ladder
    rng = np.random.default_rng(3)
    for sl, st in ((0.01, 0.05), (0.05, 0.15), (0.10, 0.35)):
        st_r = np.deg2rad(st)
        errs = []
        for _ in range(3000):
            th = np.arctan2(u[1], u[0]) + rng.normal(0, st_r)
            un = np.array([np.cos(th), np.sin(th)])
            elln = ell + rng.normal(0, sl)
            pn = p + rng.normal(0, 0.02, 2)
            try:
                qn, *_ = inverse_contact(s, pn, un, elln)
                errs.append(float(np.linalg.norm(qn - q)))
            except AssertionError:
                errs.append(np.nan)
        errs = np.array(errs)
        ok = errs[~np.isnan(errs)]
        print(f"[noise] sl={sl:.2f} st={st:.2f}deg rmse={float(np.sqrt((ok**2).mean())):.4f} "
              f"reject={float(np.mean(np.isnan(errs))):.4f}")
        if sl == 0.01:
            assert float(np.sqrt((ok ** 2).mean())) < 0.05, "contact RMSE"
    print("demo PASS")


if __name__ == "__main__":
    demo()
