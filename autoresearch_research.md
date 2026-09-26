# Autoresearch Research: ADAS Radar Perception, TinyML Micro-Kernels, HPCC Parallelization & Generative Resimulation

## Objective
35-hour autonomous loop. Senior Principal ADAS Radar Systems Architect. Invent, derive, implement, and empirically validate (on local `.mf4`/`.h5` resim data + synthetic stress scenarios) novel algorithms across 11 radar perception repos: (T1) range-adaptive dual-loop CFAR, (T2) pre-association specular mirror ghost disambiguation, (T3) async satellite motion extrapolation, (T4) kinematics-aware adaptive Mahalanobis gating, (T5) local spectral covariance scaling, (F) Doppler-RadarSplat 4D generative resimulation, (H) HPCC parallelization incl. alignment fast-lock + O(log N) associative prefix-scan Kalman filter. Publishable win: ≥1 kept idea with novelty_score ≥70, verified math, executed metrics, LaTeX paper + IDFs.

## Publish Venue
Primary: IEEE RadarConf (radar systems) / CVPRW (NeuRadar-adjacent generative track). Secondary: MLSys (TinyML kernels), NeurIPS theory track (prefix-scan KF). Style: NeurIPS template default (`papers/templates/neurips.tex`).

## Metrics
- **Primary**: novelty_score (0-100, higher is better)
- **Secondary**: sota_gap_pct, contrast_score, proof_strength, prior_art_clear

## Research Resources (nearest venues for this topic)
- Radar: IEEE RadarConf, arXiv eess.SP/physics.app-ph; baseline: in-house `variable_cfar_initial_changes` CFAR, classical CA-CFAR/OS-CFAR.
- Vision/generative: CVPR/ICCV + arXiv cs.CV — NeuRadar (CVPRW 2025), RadarSplat. Gap: no Doppler velocity, no RCS/SNR, no CDC synthesis.
- Filtering: Särkkä associative parallel KF formulation; EKF/UKF tracking baselines (F360/XTRK).
- Systems: MLSys — Xtensa ConnX BBE32 / Cortex-R52 TinyML constraints.
- Always: arXiv + Semantic Scholar (citation-sorted) + Papers with Code + OpenReview + HF Papers.

## High-Index Targets
- [ ] CFAR classics (cite counts TBD via Semantic Scholar search) — crack: static R0 window vs guardrail clutter (F1 miss / F5 CDC saturation >5016).
- [ ] NeuRadar CVPRW 2025 — crack: omits radial Doppler, RCS/SNR, CDC synthesis.
- [ ] Särkkä parallel KF associative operator — verify associativity <1e-14, Blelloch scan O(N)→O(log N).
- [ ] EKF/Joseph-form covariance literature — crack: static R0 over-trusts tire-spray backscatter.
- Log citedByCount in `equations.md` before building on any paper.

## Files in Scope
`resim_research/**` (prototypes: dual_loop_cfar.py, specular_ghost_suppression.py, async_motion_compensation.py, adaptive_gating.py, spectral_covariance_scaling.py, doppler_radarsplat_mvp.py, synthetic_generator.py, fast_da.py, parallel_kf.py, hpcc_local_orchestrator.py, parallel_orchestrator/), `storage.md`, `research.md`, `experiments/**`, `equations.md`, `strategies.md`, `papers/**`, `autoresearch_research*.jsonl/md`, `run_hpcc_burst.sh`, Slurm configs.

## Off Limits
- `Research/**`, `mf4_data/**`, `edge_hdf/**`: READ-ONLY (never edit, never commit data; local-only corpus).
- Production repos must stay clean/functional; all experiments inside `resim_research/`.
- No secrets/API tokens/passwords in any commit. No `malloc`/`new` in real-time loop code paths.

## Constraints
- TinyML hard caps: <25k params, <50KB INT8/FP16, <50µs/scan, Xtensa BBE32 @600MHz / Cortex-R52.
- KPI contracts: UDP Tier-1 integer pairing ≥99.0%; Tier-2 |Δr|≤0.010m, |Δv|≤0.015m/s, |Δθ|,|Δφ|≤0.00873rad; CAN 50ms continuity, MATCH_EPSILON=10.0 4D hash, latency ≤40ms.
- Zero hallucinated metrics: every number in storage.md/research.md from an executed script. Joseph-form + eigenvalue clipping everywhere.
- Novelty check mandatory (arXiv + Semantic Scholar + Scholar + PwC + OpenReview + HF + local graph-position check).

## What's Been Tried
- Run 1 (baseline): env audit + field map. Python 3.13.4, numpy/scipy/h5py/pandas OK; gcc/clang MISSING (C++ prototypes compile-check deferred); edge_hdf HDF5 verified (CEER_ FL/FR/RL/RR/FLR groups); mf4_MANIFEST.csv 124 rows; resim_research sandbox skeleton inventoried (SOURCES.md). No ideas yet.
- Run 2 (N2/T1 dual-loop CFAR, 62, DISCARD): prototype works (edge-skirt recall 0.50→1.00, FA 1→0) but the mechanism traces to VI-CFAR/OFPI-CFAR/ACCA-CMLD lineage — engineering, not a paradigm shift. Artifact retained per directive; idea discarded.
- **Run 3 (N3→N9, T2 specular ghosts, 74, KEEP) — the session's first real idea.** New core result: the *rank-1 specular displacement theorem* — for a planar reflector the unfolded direct and ghost positions satisfy `p − p_s = 2d·n̂` **exactly**, independent of target range/velocity/bearing; verified to 1.066e-14 over 2000 samples. Three consequences drive the paper: (1) the field's "ghost = target mirror image" model is **range-exact but bearing-wrong** (15.189°, 8.000 m at 30.3 m) — a half-correct failure invisible to range-only checks; (2) the ghost is a deterministic function of parent state, hence a **virtual antenna**: assimilating it cuts position RMSE **3.242 → 0.739 m (−77.2%, 4.39×)** because it supplies the only second measurement direction a single corner radar has; (3) because the reflector parameters ξ=(d,n̂) are shared, the correct noise covariance is `R_eff = diag(σ²) + J_ξ Σ_ξ J_ξᵀ` with the gauge-respecting `Σ_n = σ_n²(I − n̂n̂ᵀ)`, worth +13.2% over naive `diag(R)` at 2°/0.15 m geometry error. Also measured, and blocking: **curved guardrails destroy the invariant** (2D(φ) sweeps 132 m = 1320× range noise; recall 0.993→0.450) and the purely geometric test is **provably blind** to a same-range pair at exactly 2d (precision 0.638). 1-DOF DOF-reduction sub-claim refuted by its own matched 3-DOF baseline. Novelty clear per 2 independent searches; S2 citation counts unverified (429) — see worklog Run 3.
- Iteration-cost lesson: 5 of 9 prototype bugs were estimator/tolerance bugs, not physics. The derivation was right first try; budget debugging for the estimator.

## Immediate Next Actions (priority order)
1. **N10** — implement the E2e bend-conditioned test to restore recall on curved guardrails (blocking for the target environment).
2. **N11** — add the E2b Doppler test (40σ) to `classify_pairs` to close the decoy blind spot.
3. **N12** — 1-DOF prefilter + local 2-D direction refinement to recover 3-DOF accuracy cheaply.
4. LaTeX paper for N9 (novelty 74 ≥ 70) once N10/N11 land, so the limitation ships answered.
5. Resume untouched frontier: N4 (T3 async epochs), N5 (T4 adaptive gating), N6 (T5 spectral covariance), N7 (F Doppler-RadarSplat), N8 (H prefix-scan KF).
