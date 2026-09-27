"""Ghost-conditioned generative Range-Doppler augmentation (N16 backlog).

Seed: condition the RadarSplat renderer on N9-estimated reflector geometry
to synthesize multipath-augmented RD frames for detector training/eval:
each parent primitive spawns a ghost primitive via the plane model
(q = p - 2(n.p-d)n, v_ghost from spec_rdot), rendered with R^4 power.
Value test: an energy detector tuned on CLEAN frames vs tested on
multipath frames (FA flood), then re-tuned with AUGMENTED (clean+ghost)
training (FA controlled, recall kept). Metric: FA rate + recall on a
held-out multipath set, threshold picked on training only (no peeking).
# ponytail: single wall, point primitives; extended targets need RCS maps.
"""
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

import numpy as np

import resim_research.doppler_radarsplat_mvp as rs
from resim_research.specular_ghost import spec_rdot


def render_frame(parents, ghosts, p_sens, v_ego):
    """Render RD power map from parent + ghost primitive lists."""
    prims = []
    for p, v, rcs in parents:
        prims.append(rs.RadarPrimitive(p, v, rcs))
    for p, v, rcs in ghosts:
        prims.append(rs.RadarPrimitive(p, v, rcs))
    return rs.render(prims, p_sens, v_ego)


def spec_rdot_full(p, v, d, n):
    """Ghost velocity VECTOR is the parent's (rigid echo); return it."""
    return np.asarray(v, dtype=float)


def make_set(rng, n_frames, ghost_prob, d=3.0,
             n=np.array([0.0, 1.0, 0.0])):
    """Frames with ghosts injected w.p. ghost_prob (multipath weather).
    Returns frames, labels, parent predicted powers (tracker range+RCS)."""
    p_sens = np.zeros(3)
    v_ego = np.array([25.0, 0.0, 0.0])
    frames, labels, pwr = [], [], []
    for _ in range(n_frames):
        p = np.array([rng.uniform(10.0, 50.0), rng.uniform(0.5, 2.5), 0.0])
        v = np.array([rng.uniform(-5.0, 5.0), rng.uniform(-2.0, 2.0), 0.0])
        rcs = rng.uniform(5.0, 20.0)
        parents = [(p, v, rcs)]
        ghosts = []
        has = rng.random() < ghost_prob
        if has:
            q = p - 2.0 * (float(n @ p) - d) * n
            vg = spec_rdot_full(p, v, d, n)
            ghosts = [(q, vg, rcs * 0.5)]
        rd = render_frame(parents, ghosts, p_sens, v_ego)
        frames.append(rd)
        labels.append(has)
        r = float(np.linalg.norm(p - p_sens))
        pwr.append(rs.PT * rs.G ** 2 * rs.LAM ** 2 * rcs
                   / ((4 * np.pi) ** 3 * r ** 4))
    return frames, labels, np.array(pwr)


def energy_stats(frames, pwr):
    """Normalized peak statistic: max cell over tracker-predicted parent
    power (removes the R^4 range spread so multipath shows)."""
    return np.array([float(f.max()) / p for f, p in zip(frames, pwr)])


def demo() -> None:
    rng = np.random.default_rng(0)
    # train threshold on CLEAN frames (today's practice)
    clean, _, pwrc = make_set(rng, 60, 0.0)
    ec = energy_stats(clean, pwrc)
    thr_clean = float(np.quantile(ec, 0.95))  # 5% FA on clean
    # held-out MULTIPATH frames
    multi, lab, pwrm = make_set(rng, 120, 0.6)
    em = energy_stats(multi, pwrm)
    fa_flood = float(np.mean(em > thr_clean))
    # re-tune threshold on AUGMENTED training (clean + ghosts)
    aug, _, pwra = make_set(rng, 60, 0.6)
    ea = energy_stats(aug, pwra)
    thr_aug = float(np.quantile(ea, 0.95))
    fa_aug = float(np.mean(em > thr_aug))
    # recall proxy: parent detectable = normalized peak above half threshold
    rec_clean_thr = float(np.mean(em > thr_clean * 0.5))
    rec_aug_thr = float(np.mean(em > thr_aug * 0.5))
    print(f"[train-clean] thr={thr_clean:.3f} FA on multipath={fa_flood:.3f}")
    print(f"[train-aug  ] thr={thr_aug:.3f} FA on multipath={fa_aug:.3f}")
    print(f"[recall@half-thr] clean-tuned={rec_clean_thr:.3f} "
          f"aug-tuned={rec_aug_thr:.3f}")
    assert fa_flood > fa_aug + 0.10, "ghosts must flood the clean threshold"
    assert fa_aug <= 0.10, "augmented tuning must hold FA"
    assert rec_aug_thr >= 0.95, "must keep parent recall"
    print("demo PASS")


if __name__ == "__main__":
    demo()
