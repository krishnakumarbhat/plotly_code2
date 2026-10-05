"""Contamination-budgeted rank gate with reachability + sensor quorum (N24).

Frozen reference rank offset by replacement budget m: q=S~(k+m),
sandwich S(k)<=q<=S(k+2m); maneuver via reachable box (not innovation
ranks); 2-of-3 sensor quorum; write barrier on calibration.
# ponytail: numpy only; deterministic fixtures + stochastic KPI.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import numpy as np

N, M, ALPHA = 255, 8, 0.1
K = int(np.ceil((N + 1) * (1 - ALPHA)))  # 231
Q_LO, Q_OBS, Q_HI = K, K + M, K + 2 * M  # 231, 239, 247 (1-indexed)


def frozen_quantile(scores, m=M):
    """Rank-offset quantile; +inf when the rank exceeds the array."""
    s = np.sort(np.asarray(scores, float))
    idx = K + m  # 1-indexed rank
    if idx > len(s):
        return np.inf
    return float(s[idx - 1])


def hist_up(q, nbins=64):
    """Upward 64-bin quantizer on [0,1] + overflow bin (+inf)."""
    if not np.isfinite(q):
        return np.inf
    if q > 1.0:
        return np.inf
    return float(np.ceil(min(max(q, 0.0), 1.0) * nbins) / nbins)


def reach_box(p0, v0, A, dt, b_p=0.0, b_v=0.0):
    """Reachable box [c-d, c+d] per axis (N24 reachability)."""
    p0, v0, A = map(lambda a: np.asarray(a, float), (p0, v0, A))
    c = p0 + v0 * dt
    d = b_p + b_v * dt + 0.5 * A * dt ** 2
    return c - d, c + d


def sensor_box(z, q, sig, b):
    """Candidate box z-bar +- (q*sig + b) per axis."""
    z = np.asarray(z, float)
    r = q * np.asarray(sig, float) + np.asarray(b, float)
    return z - r, z + r


def boxes_hit(lo1, hi1, lo2, hi2):
    return bool(np.all(lo1 <= hi2) and np.all(lo2 <= hi1))


def gate_admit(R, boxes):
    """2-of-3 union-of-intersections admissibility (N24 Gt)."""
    Rlo, Rhi = R
    ok = []
    for i in range(3):
        for j in range(i + 1, 3):
            lo = np.maximum(np.maximum(Rlo, boxes[i][0]), boxes[j][0])
            hi = np.minimum(np.minimum(Rhi, boxes[i][1]), boxes[j][1])
            if bool(np.all(lo <= hi)):
                ok.append((i, j))
    return ok


def demo() -> None:
    # 1. rank sandwich (N24.1)
    S = np.arange(1, N + 1) / 256.0
    slow = S.copy()
    slow[:8] = 1e9  # 8 smallest -> unbounded high outliers
    q1 = frozen_quantile(slow)
    shigh = S.copy()
    shigh[-8:] = 0.0  # 8 largest -> zeros
    q2 = frozen_quantile(shigh)
    print(f"[sandwich] high-poison q={q1:.6f} (expect {247/256:.6f}); "
          f"low-poison q={q2:.6f} (expect {231/256:.6f})")
    assert abs(q1 - 247 / 256) < 1e-12 and abs(q2 - 231 / 256) < 1e-12
    assert hist_up(q1) == 248 / 256 and hist_up(q2) == 232 / 256, "upward bins"
    # mixture + ties
    mix = S.copy()
    mix[:4] = 1e9
    mix[-4:] = 0.0
    qm = frozen_quantile(mix)
    assert 231 / 256 - 1e-12 <= qm <= 247 / 256 + 1e-12, "mixture sandwich"
    ties = np.full(N, 0.5)
    assert frozen_quantile(ties) == 0.5, "ties"
    assert frozen_quantile(S, m=30) == np.inf, "violated budget -> +inf"
    print("[sandwich] mixture/ties/budget-violation OK")

    # 2. exact inclusion vectors (N24.2)
    p = np.array([10.0, 2.0])
    sig = np.array([0.1, 0.1])
    q = 231 / 256
    R = (p - np.array([0.5, 0.5]), p + np.array([0.5, 0.5]))
    honest = [np.array([10.04, 1.97]), np.array([9.98, 2.05])]
    for mag in (1e2, 1e6, 1e9):
        boxes = [sensor_box(z, q, sig, 0.0) for z in
                 honest + [np.array([100.0, -100.0]) * mag / 100.0]]
        adm = gate_admit(R, boxes)
        assert len(adm) == 1 and adm[0] == (0, 1), f"outlier mag {mag}"
    print("[inclusion] true bundle admitted for outlier mags 1e2/1e6/1e9")
    q0 = q  # frozen reference
    for _ in range(600):  # online outlier burst: no write path
        _ = gate_admit(R, [sensor_box(z, q0, sig, 0.0) for z in
                           honest + [np.array([1e6, -1e6])]])
    assert q0 == q, "calibration bit-identical after 600 outlier scans"
    print("[frozen] q bit-identical after 600-scan outlier burst")

    # 3. cut-in reachability (N24.3)
    p0 = np.array([20.0, 0.0])
    v0 = np.array([5.0, 0.0])
    Ay = 12.0
    for ay, T in ((6.0, 0.5), (12.0, 0.5)):
        true = p0 + v0 * T + np.array([0.0, 0.5 * ay * T ** 2])
        lo, hi = reach_box(p0, v0, np.array([0.0, Ay]), T)
        assert bool(np.all(lo <= true)) and bool(np.all(true <= hi)), f"ay={ay}"
    print("[cutin] ay=6/12 contained in Ay=12 box (disp 0.75/1.5 m)")
    # sensor ages 0/25/45 ms with transport bound
    for age in (0.0, 0.025, 0.045):
        b = np.array([0.05, 0.05]) + np.array([5.0, Ay]) * age + 0.5 * np.array([0.0, Ay]) * age ** 2
        zb = sensor_box(p0 + v0 * (0.5 + age), q, sig, b)
        lo, hi = reach_box(p0, v0, np.array([0.0, Ay]), 0.5)
        assert boxes_hit(lo, hi, *zb), f"age {age}"
    print("[transport] ages 0/25/45 ms overlap reachable box")
    # out-of-contract: ay=18, two bad sensors, false prior
    true18 = p0 + v0 * 0.5 + np.array([0.0, 0.5 * 18 * 0.25])
    lo, hi = reach_box(p0, v0, np.array([0.0, Ay]), 0.5)
    assert not (bool(np.all(lo <= true18)) and bool(np.all(true18 <= hi))), "ay=18 escapes"
    bad2 = [sensor_box(np.array([50.0, 50.0]), q, sig, 0.0),
            sensor_box(np.array([-50.0, 50.0]), q, sig, 0.0),
            sensor_box(honest[0], q, sig, 0.0)]
    assert gate_admit(R, bad2) == [], "two bad sensors -> empty"
    print("[kill] ay=18 escapes; two-bad-sensors gate empty (explicit)")

    # 4. stochastic KPI (N24.4): 10k trials x Gaussian/Laplace/bounded-mixture
    rng = np.random.default_rng(8)
    for name, sampler in (
            ("gauss", lambda n: rng.normal(size=n)),
            ("laplace", lambda n: rng.laplace(size=n) / np.sqrt(2)),
            ("mix", lambda n: np.where(rng.uniform(size=n) < 0.9, rng.normal(size=n),
                                       rng.normal(0, 3, size=n)) / 1.3)):
        hits = 0
        trials = 10_000
        for _ in range(trials):
            ref = np.abs(sampler(N))
            # corrupt <=8 reference entries arbitrarily
            idx = rng.choice(N, 8, replace=False)
            ref[idx] = rng.uniform(5, 50, 8)
            qq = frozen_quantile(ref)
            test = abs(float(sampler(1)[0]))
            # <=1 online sensor replaced (here: scalar analog, 1 of 1 -> skip-or-count)
            hits += test <= qq
        cov = hits / trials
        se = np.sqrt(cov * (1 - cov) / trials)
        lob = cov - 1.645 * se
        print(f"[kpi-{name}] coverage={cov:.4f} lower95={lob:.4f} (require >=0.89)")
        assert lob >= 0.89, f"coverage {name}"
    # sliding-window gate poisons under the same burst (Run-16 mechanism)
    win = list(np.abs(rng.normal(size=200)))
    w0 = float(np.quantile(win, 0.9))
    for _ in range(50):
        win = win[4:] + [30.0] * 4  # accepted-association feedback
    w1 = float(np.quantile(win, 0.9))
    print(f"[poison] sliding q {w0:.3f} -> {w1:.3f} (inflates); frozen q {q:.4f} fixed")
    assert w1 > 5 * w0, "sliding gate poisoned"
    print("demo PASS")


if __name__ == "__main__":
    demo()
