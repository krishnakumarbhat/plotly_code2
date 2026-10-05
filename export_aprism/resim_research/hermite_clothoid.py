"""Tangent-mirror Hermite cubic guardrail recovery (N20g).

N20g Lemma 1: on any smooth curve the parent/ghost pair satisfies
p - g = 2*delta*n (mirror image in the local tangent line).
Corollary 1: tangent line = perpendicular bisector of (p,g);
q = bisector cap receive-ray; slope sigma = -n_x/n_y.
Theorem 1: cubic rail coefficients enter linearly (Hermite system);
two contacts with distinct x determine the cubic uniquely
(confluent Vandermonde det = (x2-x1)^4).
# ponytail: single cubic rail, world-fixed frame; one file, no heap in loop.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import numpy as np

CHI2_2_99 = 9.21034


def rail_f(c, x):
    """Cubic rail y = c0 + c1 x + c2 x^2 + c3 x^3."""
    return c[0] + c[1] * x + c[2] * x ** 2 + c[3] * x ** 3


def rail_fp(c, x):
    """Rail slope f'(x)."""
    return c[1] + 2.0 * c[2] * x + 3.0 * c[3] * x ** 2


def bisector_contact(s, p, g):
    """Corollary 1: contact q + slope sigma from (s,p,g)."""
    s = np.asarray(s, float)
    p = np.asarray(p, float)
    g = np.asarray(g, float)
    m = 0.5 * (p + g)
    d = p - g
    nd = float(np.linalg.norm(d))
    assert nd > 1e-12, "degenerate p==g"
    n = d / nd
    u = g - s
    nu = float(np.linalg.norm(u))
    assert nu > 1e-12, "degenerate g==s"
    u = u / nu
    den = float(n @ u)
    assert abs(den) > 1e-9, "grazing geometry"
    assert abs(n[1]) > 1e-9, "rail parallel to y-axis; rotate frame"
    t = float(n @ (m - s)) / den
    q = s + t * u
    return q, float(-n[0] / n[1]), t, n


def hermite_design(x):
    """2x4 Hermite design block V_i for contact x."""
    return np.array([[1.0, x, x ** 2, x ** 3],
                     [0.0, 1.0, 2.0 * x, 3.0 * x ** 2]])


def hermite_solve2(x1, y1, s1, x2, y2, s2):
    """Closed-form cubic through two point-slope pairs (Theorem 1)."""
    h = x2 - x1
    assert abs(h) > 1e-12, "coincident contacts"
    db = (y2 - y1) / h
    # Newton form with repeated nodes
    def f(x):
        dx1 = x - x1
        return (y1 + s1 * dx1 + (db - s1) / h * dx1 ** 2
                + (s1 + s2 - 2.0 * db) / h ** 2 * dx1 ** 2 * (x - x2))
    # recover monomial coefficients by interpolation at 4 nodes
    xs = np.array([x1, x1 + h / 3.0, x1 + 2.0 * h / 3.0, x2])
    # use exact polynomial values + derivative constraints via lstsq on 4x4
    V = np.vstack([hermite_design(x1), hermite_design(x2)])
    z = np.array([y1, s1, y2, s2])
    c, *_ = np.linalg.lstsq(V, z, rcond=None)
    return c, f


def forward_pair(s, c, x, rho):
    """Generate an exact (p,g) pair from rail contact x (Theorem 2)."""
    s = np.asarray(s, float)
    q = np.array([x, rail_f(c, x)])
    fp = rail_fp(c, x)
    n = np.array([-fp, 1.0]) / np.hypot(fp, 1.0)
    if float(n @ (s - q)) < 0:
        n = -n
    u = q - s
    u = u / float(np.linalg.norm(u))
    a = u - 2.0 * float(u @ n) * n
    p = q + rho * a
    g = q + rho * u
    return p, g, q, n, u, a


def snell_phi(s, c, p, xs):
    """Snell residual phi(x) sampled on grid xs (Theorem 2 forward check)."""
    s = np.asarray(s, float)
    out = []
    for x in xs:
        q = np.array([x, rail_f(c, x)])
        fp = rail_fp(c, x)
        t1 = np.array([1.0, fp])
        v1 = (q - s) / max(float(np.linalg.norm(q - s)), 1e-12)
        v2 = (q - p) / max(float(np.linalg.norm(q - p)), 1e-12)
        out.append(float(t1 @ (v1 + v2)))
    return np.array(out)


def demo() -> None:
    # 1. planar fixture (exact)
    s = np.zeros(2)
    p = np.array([20.0, -1.0])
    g = np.array([20.0, -7.0])
    q, sig, t, n = bisector_contact(s, p, g)
    print(f"[planar] q={q} sig={sig:.12f} t={t:.6f} n={n}")
    assert abs(q[1] + 4.0) < 1e-12, "contact y"
    assert abs(q[0] - 80.0 / 7.0) < 1e-12, "contact x"
    assert abs(sig) < 1e-12, "slope"
    assert abs(n[0]) < 1e-12 and abs(n[1] - 1.0) < 1e-12, "normal"

    # 2. cubic generation round-trip (no root solving)
    c = np.array([-4.0, 0.02, 1.5e-3, -8e-6])
    xs = [8.0, 15.0, 25.0, 40.0, 60.0]
    rec = []
    for x in xs:
        for rho in (3.0, 6.0):
            p, g, q0, n0, u0, a0 = forward_pair(s, c, x, rho)
            if float((p - q0) @ n0) <= 0:
                continue
            q, sig, _, _ = bisector_contact(s, p, g)
            rec.append((x, q, sig, p, g))
    assert len(rec) >= 4, "need >=4 valid pairs"
    max_qe = max(float(np.linalg.norm(q - np.array([x, rail_f(c, x)]))) for x, q, _, _, _ in rec)
    max_se = max(abs(sig - rail_fp(c, x)) for x, _, sig, _, _ in rec)
    print(f"[cubic] pairs={len(rec)} max|dq|={max_qe:.2e} max|dsig|={max_se:.2e}")
    assert max_qe < 1e-9 and max_se < 1e-12, "round-trip"

    # Hermite solve from x=8,40 contacts
    e8 = [r for r in rec if r[0] == 8.0][0]
    e40 = [r for r in rec if r[0] == 40.0][0]
    V = np.vstack([hermite_design(8.0), hermite_design(40.0)])
    det = float(np.linalg.det(V))
    print(f"[hermite] det={det:.6e} (x2-x1)^4={(32.0**4):.6e}")
    assert abs(det - 32.0 ** 4) / 32.0 ** 4 < 1e-9, "confluent Vandermonde"
    z = np.array([e8[1][1], e8[2], e40[1][1], e40[2]])
    chat, *_ = np.linalg.lstsq(V, z, rcond=None)
    rel = float(np.linalg.norm(chat - c) / np.linalg.norm(c))
    print(f"[hermite] rel-err={rel:.2e}")
    assert rel < 1e-8, "coefficient recovery"

    # 3. Snell forward check: generating x must root phi
    for x in (8.0, 25.0, 40.0):
        p, g, q0, *_ = forward_pair(s, c, x, 4.0)[0:4]
        grid = np.linspace(0.5, 120.0, 2401)
        phi = snell_phi(s, c, p, grid)
        # sign change within one grid step of x
        j = int(np.argmin(np.abs(grid - x)))
        assert abs(phi[j]) < 2e-2 or phi[max(j - 1, 0)] * phi[min(j + 1, len(grid) - 1)] <= 0, \
            "Snell root near generator"
    print("[snell] generator roots present")

    # 4. noise ladder + decoys
    rng = np.random.default_rng(0)
    sigmas = (0.02, 0.05, 0.10)
    base = [(x, *forward_pair(s, c, x, 4.0)[:2]) for x in (8.0, 15.0, 25.0, 40.0,
                                                            50.0, 55.0, 60.0, 65.0)]
    for sg in sigmas:
        qs, ss = [], []
        for _, p, g in base:
            pn = p + rng.normal(0, sg, 2)
            gn = g + rng.normal(0, sg, 2)
            q, sig, _, _ = bisector_contact(s, pn, gn)
            qs.append(q)
            ss.append(sig)
        qs = np.array(qs)
        xs_b = np.array([b[0] for b in base])
        Vb = np.vstack([hermite_design(x) for x in xs_b])
        zb = np.array([[q[1], sg_] for q, sg_ in zip(qs, ss)]).reshape(-1)
        chat_n, *_ = np.linalg.lstsq(Vb, zb, rcond=None)
        # chi2 gate per pair (position+ slope, diagonal approx)
        tp = fp_ = 0
        for x, q, sg_ in zip(xs_b, qs, ss):
            r = np.array([q[1] - rail_f(chat_n, x), sg_ - rail_fp(chat_n, x)])
            S = np.diag([2 * sg ** 2, 8 * sg ** 2 / max((p[1] - g[1]) ** 2, 1.0)
                         if False else 2 * sg ** 2])
            # simple isotropic gate scaled by noise
            if float(r @ r) / max(2 * sg ** 2, 1e-12) <= CHI2_2_99:
                tp += 1
            else:
                fp_ += 1
        rec_rate = tp / len(base)
        print(f"[noise] sg={sg:.2f} max|dq|={float(np.max(np.abs(qs[:,1]-np.array([rail_f(c,x) for x in xs_b])))):.4f} "
              f"gate-pass={tp}/{len(base)} crec-rel={float(np.linalg.norm(chat_n-c)/np.linalg.norm(c)):.3e}")
        if sg == 0.02:
            assert rec_rate >= 0.85, "recall at 0.02 m"
    print("demo PASS")


if __name__ == "__main__":
    demo()
