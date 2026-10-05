# A-PRISM Export Package (2026-09-30)

Drop-in mirror of `resim_research/` prototypes for external evaluation.
Production trees are untouched — copy any file back to the matching
`resim_research/<name>` path to diff or integrate.

## Contents

`resim_research/` — 12 new kernels (Runs 22–30) + 6 legacy anchors + `run_regression.py`
(`doppler_disambiguation.py` ships as a `combined_chain.py` dependency):

| File | Idea | Claim (executed) |
|---|---|---|
| `hermite_clothoid.py` | N20g tangent-mirror Hermite cubic | coeff rel-err 3.4e-14 exact; 5.2e-3 @ 0.02 m noise |
| `occupancy_blue.py` | N22g Occupancy-BLUE CFAR | Pfa 1.02e-4 vs 1e-4; BLUE var 0.0686 vs CA16 bias +37.5% |
| `yawfree_clock.py` | N27 yaw-free clock/bias | τ/b exact 1e-12; noise RMSE 6.6/3.8 ms |
| `specular_contact.py` | N26 contact inverse + curvature cert | t=5 exact; contact RMSE 0.018 m @ low rung |
| `guarded_replay.py` | N28 guarded replay algebra | 300/300 assoc; margin 1/3; fixed-pt enclosure-only |
| `gram_lift.py` | N21g Gram-invariant lift | closure 5.9e-13; scan equiv <1e-9; speed exact |
| `n23_residual_track.py` | N23 3-product residual | stencil 1e-12; 13.13 mm = theory; 3 mults |
| `n24_rank_gate.py` | N24 rank gate | sandwich exact; coverage ≥0.89; sliding poisons 19× |
| `n25_ghost_yaw.py` | N25 ghost yaw | 1e-10 exact; cert 0.071°; all failures refused |
| `imm_accel_gate.py` | N15 IMM gate (REFUTED) | 0.880 vs N5 0.999; hybrid == N5 |
| `blue_closed_loop.py` | N22g closed loop | recall +0.074; loop gain 0.099 |
| `gram_filter_compare.py` | N21g vs EKF | decoupled 1.24×; joint-spd 1.25×; joint-pos 6.77× NEG |
| `specular_ghost.py` | Run 3 rank-1 ghost | RMSE −77.2% |
| `combined_chain.py` | Run 14 closed chain | edges 0.969/0.902, track −67% |
| `adaptive_gating.py` | Run 6 gate | recall 0.863→0.996 |
| `dual_loop_cfar.py` | Run 2 CFAR | recall 0.50→1.00, FA 1→0 |
| `parallel_kf.py` | Run 11 prefix-scan | rel 6.35e-15 |

`bench_aprism.py` — benchmark runner. `bench_aprism_report.txt` — generated report.

## Quick start

```bash
python bench_aprism.py --quick   # 6 new kernels (~1-2 min)
python bench_aprism.py           # + 5 legacy anchors
```

Python 3.13, `numpy`/`scipy` only. Every prototype runs `demo()` asserts;
non-zero exit = FAIL. Full history: `storage.md` Runs 22–25,
`equations.md` E17–E22, `research.md` §§7–8 + IDF-5–8 (in main repo).

## Notes for integrators

- New kernels are world-fixed-frame, single-rail/single-bundle prototypes;
  association, ego-motion compensation, and fixed-point ports are separate budgets.
- N22g closed-loop tracker integration and N21g full 7-dim filter comparison
  vs IEKS are open (see recipes in `autoresearch_research.ideas.md`).
- No novelty/patent clearance asserted — claim charts in ideas file list
  nearest inspected art per node.
