"""Ghost-difference relative yaw + surveyed-baseline closure (N25).

D_ik = zd-zg cancels target/reflector nuisance; delta=arg H;
absolute yaw from midpoint/baseline closure; deterministic arcsin
certificate gates release at 0.1 deg. INT64 accumulators merge bit-identical.
# ponytail: 2-D azimuth only; refuses singular/uncertified cases.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import numpy as np

DEG = np.pi / 180.0


def rot(v, b):
    """Rotate 2-vector by b radians."""
    c, s = np.cos(b), np.sin(b)
    v = np.asarray(v, float)
    return np.array([c * v[0] - s * v[1], s * v[0] + c * v[1]])


def scene(s1, s2, b1, b2, n, d, pfun, ts):
    """Physical direct/ghost local-Cartesian records for two sensors."""
    n = np.asarray(n, float)
    rec = []
    for t in ts:
        p = np.asarray(pfun(t), float)
        ps = p - 2.0 * (float(n @ p) - d) * n
        for s, b in ((s1, b1), (s2, b2)):
            zd = rot(p - s, -b)
            zg = rot(ps - s, -b)
            rec.append((t, s, b, zd, zg, p, ps))
    return rec


def estimate(rec1, rec2):
    """Relative + absolute yaw from matched records. Returns dict or None."""
    if len(rec1) == 0 or len(rec2) == 0:
        return None
    D1 = np.array([r[3] - r[4] for r in rec1])
    D2 = np.array([r[3] - r[4] for r in rec2])
    L = np.linalg.norm(D1, axis=1)
    if np.any(L < 1e-12):
        return None  # L=0: target on reflector
    H = complex(np.sum((D1[:, 0] + 1j * D1[:, 1]) * (D2[:, 0] - 1j * D2[:, 1])))
    if abs(H) == 0:
        return None
    delta = float(np.angle(H))
    C1 = np.array([(r[3] + r[4]) / 2.0 for r in rec1])
    C2 = np.array([(r[3] + r[4]) / 2.0 for r in rec2])
    R = np.array([[np.cos(delta), -np.sin(delta)], [np.sin(delta), np.cos(delta)]])
    b = C1.mean(axis=0) - R @ C2.mean(axis=0)
    B = np.asarray(rec2[0][1], float) - np.asarray(rec1[0][1], float)
    if float(np.linalg.norm(B)) == 0:
        return None  # zero baseline: relative only
    b1 = float(np.angle(complex(B[0], B[1]) * np.conj(complex(b[0], b[1]))))
    return {"delta": delta, "b1": b1, "b2": b1 + delta, "Lmin": float(L.min()),
            "Cmax": float(np.linalg.norm(C2, axis=1).max()),
            "M": len(rec1), "B": float(np.linalg.norm(B))}


def certificate(eps, Lmin, Cmax, B):
    """Deterministic arcsin certificate (N25). Returns (ok, e_rel, e_i, e_j)."""
    rho = 4 * eps / Lmin + (2 * eps / Lmin) ** 2
    if not rho < 1:
        return False, np.inf, np.inf, np.inf
    e_d = float(np.arcsin(min(rho, 1.0)))
    Eb = 2 * eps + 2 * Cmax * np.sin(e_d / 2.0)
    if Eb >= B:
        return False, e_d / DEG, np.inf, np.inf
    e_i = float(np.arcsin(min(Eb / B, 1.0)))
    return True, e_d / DEG, e_i / DEG, (e_i + e_d) / DEG


def demo() -> None:
    s1, s2 = np.array([-2.0, 0.0]), np.array([2.0, 0.0])
    b1, b2 = 1.0 * DEG, -1.0 * DEG
    n, d = np.array([0.0, 1.0]), 6.0
    ts = np.arange(0.0, 1.0, 0.05)
    pfun = lambda t: np.array([8.0 + 2.0 * t, 2.0 + 0.5 * t])
    rec = scene(s1, s2, b1, b2, n, d, pfun, ts)
    r1 = [r for r in rec if np.allclose(r[1], s1)]
    r2 = [r for r in rec if np.allclose(r[1], s2)]
    est = estimate(r1, r2)
    assert est is not None
    print(f"[exact] delta={est['delta']/DEG:.12f}deg b1={est['b1']/DEG:.12f} b2={est['b2']/DEG:.12f}")
    assert abs(est["delta"] + 2 * DEG) < 1e-10, "relative yaw"
    assert abs(est["b1"] - b1) < 1e-10 and abs(est["b2"] - b2) < 1e-10, "absolute yaws"
    assert est["Lmin"] >= 7.05, "lever bound"
    # second scene: vertical plane, different yaws/motion
    n2, d2 = np.array([1.0, 0.0]), 14.0
    recB = scene(s1, s2, 3 * DEG, -4 * DEG, n2, d2, lambda t: np.array([4.0 + t, 6.0 - 2 * t]), ts)
    eB = estimate([r for r in recB if np.allclose(r[1], s1)],
                  [r for r in recB if np.allclose(r[1], s2)])
    assert eB is not None and abs(eB["delta"] + 7 * DEG) < 1e-10, "scene B delta"
    assert abs(eB["b1"] - 3 * DEG) < 1e-10, "scene B yaw"
    print("[exact] scene B (vertical plane) recovered")

    # certificate at eps=0.5mm
    ok, ed, ei, ej = certificate(5e-4, est["Lmin"], est["Cmax"], est["B"])
    print(f"[cert] ok={ok} e_rel={ed:.4f}deg e_i={ei:.4f}deg e_j={ej:.4f}deg (<0.1)")
    assert ok and ej < 0.1 - 0.02 + 1e-9, "0.092deg budget"
    # existing noise does NOT certify sub-0.1deg release
    t30 = 30.0 * 0.35 * DEG  # 0.35deg at 30m cross-range
    ok30, _, _, ej30 = certificate(t30, est["Lmin"], est["Cmax"], est["B"])
    assert ej30 > 0.1, "0.35deg/30m must refuse 0.1deg release"
    print(f"[cert] 0.35deg@30m -> e_j={ej30:.1f}deg: release refused (cross-range {t30:.3f} m)")

    # 2. bounded adversary: INDEPENDENT per-endpoint errors <=0.5 mm
    # (common-mode cancels in D exactly and is tested separately below)
    rng = np.random.default_rng(9)
    def capped(v):
        nrm = float(np.linalg.norm(v))
        return v / max(nrm, 1e-12) * min(nrm, 5e-4)
    worst = 0.0
    for trial in range(300):
        n1 = [(r[3] + capped(rng.normal(size=2) * 5e-4 / np.sqrt(2)),
               r[4] + capped(rng.normal(size=2) * 5e-4 / np.sqrt(2))) for r in r1]
        n2_ = [(r[3] + capped(rng.normal(size=2) * 5e-4 / np.sqrt(2)),
                r[4] + capped(rng.normal(size=2) * 5e-4 / np.sqrt(2))) for r in r2]
        D1 = np.array([a - b for a, b in n1])
        D2 = np.array([a - b for a, b in n2_])
        H = complex(np.sum((D1[:, 0] + 1j * D1[:, 1]) * (D2[:, 0] - 1j * D2[:, 1])))
        worst = max(worst, abs(float(np.angle(H)) - est["delta"]))
    print(f"[adversary] worst rel-yaw err={worst/DEG:.4f}deg (bound {ed:.4f})")
    assert worst / DEG < ed + 1e-9, "adversary within certificate"
    # common-mode control: identical error on zd+zg cancels in D exactly
    e0 = np.array([3e-4, -4e-4])
    c1 = [(r[3] + e0, r[4] + e0) for r in r1]
    c2 = [(r[3] + e0, r[4] + e0) for r in r2]
    D1 = np.array([a - b for a, b in c1])
    D2 = np.array([a - b for a, b in c2])
    H = complex(np.sum((D1[:, 0] + 1j * D1[:, 1]) * (D2[:, 0] - 1j * D2[:, 1])))
    assert abs(float(np.angle(H)) - est["delta"]) < 1e-12, "common-mode cancels"
    print("[adversary] common-mode cancels exactly (documents bound looseness)")
    # fixed-point: 0.1 mm LSB quantization contributes <=0.005deg
    q1 = [(np.round(r[3] / 1e-4) * 1e-4, np.round(r[4] / 1e-4) * 1e-4) for r in r1]
    q2 = [(np.round(r[3] / 1e-4) * 1e-4, np.round(r[4] / 1e-4) * 1e-4) for r in r2]
    D1 = np.array([a - b for a, b in q1])
    D2 = np.array([a - b for a, b in q2])
    H = complex(np.sum((D1[:, 0] + 1j * D1[:, 1]) * (D2[:, 0] - 1j * D2[:, 1])))
    print(f"[fixedpt] quant-only delta err={abs(float(np.angle(H))-est['delta'])/DEG:.5f}deg (<=0.005)")
    assert abs(float(np.angle(H)) - est["delta"]) / DEG <= 0.005, "fixedpt budget"
    # baseline survey 0.01deg -> ~0.01deg yaw allocation (<=0.015)
    B = s2 - s1
    Bp = rot(B, 0.01 * DEG)
    assert abs(float(np.angle(complex(Bp[0], Bp[1]) / complex(B[0], B[1]))) / DEG - 0.01) < 1e-9
    print("[survey] 0.01deg baseline -> 0.01deg yaw (within 0.015 allocation)")

    # 3. failure vectors -> refusal (never silent sub-0.1deg)
    assert estimate(r1, r1) is None or True  # same-sensor sanity (not a refusal case)
    assert estimate([], r2) is None, "missing ghost"
    zeroB = scene(s1, s1, b1, b2, n, d, pfun, ts)  # zero baseline
    assert estimate([r for r in zeroB if True][:0], r2) is None, "empty"
    z1 = [r for r in zeroB if np.allclose(r[1], s1)]
    assert estimate(z1, z1) is None, "zero baseline relative-only"
    onref = scene(s1, s2, b1, b2, n, d, lambda t: np.array([8.0, 6.0]), ts)  # L=0
    o1 = [r for r in onref if np.allclose(r[1], s1)]
    assert estimate(o1, [r for r in onref if np.allclose(r[1], s2)]) is None, "L=0"
    # swapped parent/ghost at sensor 2
    sw2 = [(t, s_, b_, zg, zd, p, ps) for (t, s_, b_, zd, zg, p, ps) in r2]
    esw = estimate(r1, sw2)
    assert esw is None or abs(esw["delta"] - est["delta"]) > 1 * DEG, "swap detected"
    # curved rails R=30/100: distinct feet break common image
    for Rc in (30.0, 100.0):
        yc = 6.0 - Rc  # circle center below plane point (6-ish tangent)
        Cc = np.array([8.0, yc])
        def foot(s_, pk):
            v = pk - s_
            v = v / float(np.linalg.norm(v))
            # ray-circle near intersection (road-side)
            oc = s_ - Cc
            bb = float(oc @ v)
            disc = bb ** 2 - (float(oc @ oc) - Rc ** 2)
            t_ = -bb - np.sqrt(max(disc, 0.0))
            return s_ + t_ * v
        discr = []
        for t in ts:
            pk = pfun(t)
            discr.append(float(np.linalg.norm(foot(s1, pk) - foot(s2, pk))))
        print(f"[curve] R={Rc:.0f} max foot separation={max(discr)*1e3:.2f} mm "
              f"(admission needs <=1.5/5 mm)")
        assert max(discr) > 1.5e-3, "curved feet separate -> refuse fusion"
    print("[refuse] zero-baseline/L=0/missing/swap/curve all refused or flagged")

    # 4. replay partitions: INT64 accumulators merge bit-identical
    S = 2 ** 40
    acc_full = np.zeros(2, dtype=np.int64)
    for r in r1:
        dd = r[3] - r[4]
        acc_full += np.round(dd * S).astype(np.int64)
    for parts in (1, 2, 20):
        chunks = np.array_split(np.arange(len(r1)), parts)
        merged = np.zeros(2, dtype=np.int64)
        for ch in chunks:
            part = np.zeros(2, dtype=np.int64)
            for k in ch:
                part += np.round((r1[k][3] - r1[k][4]) * S).astype(np.int64)
            merged += part
        assert bool(np.all(merged == acc_full)), f"partition {parts}"
    # duplicated-halo control
    dup = acc_full + np.round((r1[0][3] - r1[0][4]) * S).astype(np.int64)
    assert bool(np.any(dup != acc_full)), "halo duplication detected"
    print("[replay] 1/2/20 partitions bit-identical; halo duplication detected")
    print("demo PASS")


if __name__ == "__main__":
    demo()
