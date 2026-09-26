# Worklog: ADAS Radar Perception / TinyML / HPCC / Generative Resimulation

**Session started:** 2026-09-26 | **Venue target:** IEEE RadarConf / CVPRW / MLSys / NeurIPS-theory | **Branch:** research/adas-radar-20260926

## Key Insights
- (Run 1) Local corpus is real and readable: corner-radar HDF5 (CEER groups) + 124-row MF4 manifest + KPI scripts in resim_research/kpi. No cluster access — HPCC work stays a local multi-worker replay simulator.
- (Run 1) gcc/clang absent on this host: C++ prototypes are draft-only until a toolchain appears; Python/NumPy is the verification path.
- (Run 2) Three CFAR faults found numerically, all fixed in prototype: median/mean CA-scaling mismatch (×1/ln2), CUT-relative censor legitimizing walls (→ clean-side min(lead,lag) censor), fixed-N alpha with censored refs (→ alpha(N_eff)). Edge recovery required two-regime wall/skirt censoring + M-of-2 transition confirmation. Lesson: range-only adaptation cannot beat a wide wall without a clutter-map regime switch.
- (Run 2) Novelty verdict: adaptive window/censoring traces to VI-CFAR, OFPI-CFAR (adaptive reference window), ACCA-CMLD/GCMLD (automatic censoring) — combination is solid engineering, not a paradigm shift. Score 62 → DISCARD as research idea; prototype + metrics retained per 35h directive.
- **(Run 3) A "half-correct" model is worse than a wrong one.** The field's ghost model (target's mirror image) reproduces range to 0.0 m but mislocates bearing by 15.189° → 8.000 m at 30.3 m. A range-only sanity check cannot detect it. Lesson: audit *which* output of a closed-form model is exact, not whether it looks plausible.
- **(Run 3) The specular echo is a deterministic function of the parent state, so it is a virtual antenna, not a spurious target.** Assimilating it cuts position RMSE 3.242 → 0.739 m (−77.2%, 4.39×) — the ghost is the *only* second direction available, so it is what makes 3-D velocity observable at all from one corner radar.
- **(Run 3) Ghost ≠ parent Doppler, at 40σ.** Δv_r = v·(u_s − u_d) = −1.987 m/s in the nominal cut-in. The "same Doppler bin" assumption is false, and this is the *only* signal that can separate a ghost from a decoy pair — the geometric test is provably blind to that case.
- **(Run 3) An exact invariant does not automatically make a cheap estimator.** I predicted the 1-DOF (magnitude-only) consensus would match a 3-DOF directional search; my own baseline refuted it (0.845/0.727 vs 0.993/0.909). Direction must be *searched*, not estimated first, because the estimate is contaminated by the random pairs inside the magnitude window. Negative result asserted in the demo, not hidden.
- **(Run 3) Curved guardrails break the invariant, and that is the real ADAS environment.** 2D(φ) sweeps a 132 m band (1320× the range noise); recall collapses 0.993 → 0.450. The planar theorem does not transfer. This is the top-priority follow-up (N10).
- (Run 3) Iteration cost note: 5 of 9 prototype bugs were *statistics/tolerance* bugs, not physics — the physics was right on the first try (1e-14). Budget debugging time for the estimator, not the derivation.

## Next Ideas
- **N10 (top priority)**: implement the E2e bend-conditioned test — bearing-conditioned |p−p_s| = 2R_c + 2(y_g+R_c)cosφ(u_d), a 1-DOF fit in the bend parameter — to recover the 0.45 recall on curved guardrails. Blocking for the target environment.
- **N11**: add the E2b Doppler test to `classify_pairs` to kill the decoy blind spot (precision 0.638 → target >0.95).
- **N12**: 1-DOF excess-resultant prefilter + local 2-D direction refinement to recover 3-DOF accuracy at 1-DOF global cost.
- Then resume N4 (T3 yaw-sweep residual), N5 (T4 cut-in recall), N6 (T5 Joseph-form definiteness), N7 (F Doppler-RadarSplat MVP), N8 (H prefix-scan associativity + speedup).
- Deferred per `autoresearch_directive.md` item 4: LaTeX paper for the kept N9 idea (novelty 74 ≥ 70) — write after N10/N11 close the curved-guardrail gap, so the paper ships with its limitation answered.

### Run 3: T2 rank-1 specular displacement + ghost-as-virtual-aperture — novelty_score=74.0 (KEEP)
- Timestamp: 2026-09-26 18:22
- What changed: `resim_research/specular_ghost.py` (new, 620 lines) — specular unfolding geometry, `fault_naive_model`, `fault_doppler`, `fault_curved_guardrail`, `rank1_consensus` (1-DOF excess-resultant, largest-coherent-set fixed point, sequential non-redundant extraction), `ransac3dof` (matched-statistic 3-DOF baseline), `classify_pairs` (assignment-free, conjunction of magnitude + direction invariants), `bearing_gate` baseline, blocked-index scene generator, iterated exact-residual EKF with the R_eff coupling, `demo()` assert self-check. Artifact: `experiments/run-3.log`.
- Math: E2a/E2b/E2c/E2d/E2e (equations.md) — all executed, none trusted on sight. Symbolic re-derivation of C1–C8 delegated to an independent math sub-agent; its C2 REFUTATION independently predicted the 15.189°/8.000 m fault I then measured.
- Result: rank-1 |p−p_s|=2d exact to **1.066e-14** (2000 samples). Naive model range-exact (0.0 m) / bearing-wrong (15.189°, 8.000 m @ 30.3 m). Doppler offset −1.987 m/s = 40σ. Ghost-pair extraction (40 scans, 20 dets, σ_r 0.10 m, σ_az 0.35°): 3-DOF recall **0.993** / precision **0.909** / 2d err **0.077 m**; 1-DOF 0.845/0.727/0.618 (refuted). Curved: 0.450/0.717. Decoy: 0.983/0.799. Assimilation: pos RMSE **3.242 → 0.739 m, −77.2% (4.39×)**; R_eff coupling gain +0.0% → +2.9% → **+13.2%** as ξ grows. sota_gap_pct = 77.2% vs direct-only single-path.
- Novelty check (2 independent parallel sub-agents, 2026-09-26): rank-1 2d·n̂ invariant **not found** in any fetched source; 1-DOF reflector ID not found; ghost-as-virtual-aperture assimilation not found (closest: Li & Varshney, "Multipath exploitation for target localization", IEEE TSP ~2014, >250 cites, paywalled — exploits multipath for localization but never states the invariant or the R_eff coupling); the automotive ghost-target line (Kamon/Chong/Vockers ~2015, RadarConf/MDPI) **rejects** ghosts via clustering/RANSAC/track gating and never assimilates. Real URLs logged in the JSONL description. prior_art_clear=1. **Caveat logged: Semantic Scholar returned 429 on both agents, so citedByCount is unverified — the mandatory citation bar is NOT fully satisfied; the contribution rests on first-principles unfolding (classical optics), not on any cited paper's mechanism, so no foundation is being built on an uncounted source.**
- Insight: the citable core is (i) an exact theorem the field has not stated, (ii) a paradigm inversion with a measured 4.4× gain, and (iii) a correct covariance coupling nobody publishes. The two things that block a strong paper are both *measured*, not suspected: curved guardrails and the decoy blind spot.
- Next: N10 (bend-conditioned test) → N11 (Doppler disambiguation) → N12 (hybrid consensus) → LaTeX paper for the kept idea.

### Run 2: T1 dual-loop CFAR prototype + verification — novelty_score=62.0 (DISCARD)
- Timestamp: 2026-09-26 (inline intervention: driver iters 1–2 stalled on watchdog with zero output; backend probe OK; driver stopped, manual iteration, driver to be restarted)
- What changed: `resim_research/dual_loop_cfar.py` — slow EMA floor (λ=0.02, track-masked median), span-heterogeneity R0 ∈ {8,16,32}, clean-side censoring, effective-N alpha, two-regime wall/skirt switch, M-of-2 transition confirmation, `demo()` assert self-check.
- Math: E1 verified (see equations.md). Faults found and fixed numerically, not assumed.
- Result: synth 128×32 (25× wall + 6× skirt, 4 seeds): baseline static recall=0.50 FA=1 vs dual-loop recall=1.00 FA=0, CDC load 2. sota_gap_pct: recall +100% rel (edge-skirt recovery), FA −100%.
- Novelty check (web, 2026-09-26): adaptive reference windows (OFPI-CFAR, JPIER), VI-CFAR env recognition, auto-censoring CFAR family (ACCA/GCMLD/ACMLD, IQR-CFAR 2024), 2D window shapes, LSTM-CFAR 2025 — mechanism lineage exists → prior_art_clear=0, contrast 55. Idea discarded; artifact retained.
- Insight: the loop-stall root cause is worker-side (prompt too heavy for one watchdog window), not backend. Mitigation for restart: stall-warning prompt already injected by driver for iter 3+.
- Next: N3 (T2 specular ghost) — pick overpass synthetic; keep worker scope tight (one file + one run per iteration).

### Run 1: baseline env audit + field map — novelty_score=55.0 (KEEP)
- Timestamp: 2026-09-26
- What changed: setup committed (state files, strategy graph N1..N7, equations ledger E1..E6, storage.md skeleton); verified Python 3.13.4 + numpy/scipy/h5py/pandas import OK; confirmed edge_hdf HDF5 groups [CEER_ FL, FR, RL, RR, FLR]; mf4_MANIFEST.csv (124,3); sandbox copies inventoried via SOURCES.md.
- Math: none yet — equations E1..E6 seeded as frontier (unverified).
- Result: novelty_score=55.0 (baseline), prior_art_clear=1.
- Insight: venue routing fixed (RadarConf/CVPRW/MLSys/NeurIPS-theory); high-index citation counts still to collect — mandatory before any keep that builds on a paper.
- Next: Run 2 = T1 dual-loop CFAR numeric prototype + equation verification (critical thinking: attack static-R0 assumption).
