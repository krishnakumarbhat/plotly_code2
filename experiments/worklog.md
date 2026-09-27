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

### Run 16: N15 conformal async fusion — novelty_score=67.0 (DISCARD)
- Timestamp: 2026-09-27 (inline)
- What changed: `resim_research/conformal_fusion.py` (new) — split-conformal gate on normalized innovations (sliding 200-window), OOS second sensor through same gate; `demo()` asserts coverage + win + RMSE.
- Math: E11. Key fault caught: conformal LOSES under pure outlier contamination (calibration poisoning) — its regime is model mismatch, reframed accordingly.
- Result: unmodeled sinusoidal velocity — fixed coverage 0.042/RMSE 17.683 m (total loss) vs conformal 0.996/0.263 m.
- Novelty: conformal-filtering lineage → 67 DISCARD; artifact retained. Synergy note: N5 adaptive gate addresses the same regime parametrically.
- Next: paper (research.md) + IDFs → HPCC scripts → presentation.

### Run 15: N14 track-conditioned residual estimator + INT8 — novelty_score=65.0 (DISCARD)
- Timestamp: 2026-09-27 (inline)
- What changed: `resim_research/track_conditioned.py` (new) — FMCW beat model, full-FFT+parabolic baseline, lag-1 residual estimator, symmetric INT8 fake-quant path; `demo()` asserts accuracy parity + INT8 bound + ≥4× flop cut.
- Math: E10. Fault caught: single-chirp beat is range-dominated (velocity metric nonsense) → reframed to range RMSE. Bonus: lag-1 beats FFT 4× (bin-free vs scalloping).
- Result: range RMSE 0.439 → 0.106 → 0.108 (INT8) m; 4608 vs 1024 mults (4.5×).
- Novelty: arXiv:2609.30176 prior art → 65 DISCARD; artifact retained (TinyML envelope verified).
- Next: N15 conformal fusion → paper + presentation + HPCC scripts.

### Run 14: N18 closed ghost chain (NEW BEST) — novelty_score=80.0 (KEEP)
- Timestamp: 2026-09-27 (inline, breakthrough block)
- What changed: `resim_research/combined_chain.py` (new) — rank1 estimation → Doppler gating → dual EKF with ESTIMATED (d̂,n̂) + R_eff, all modules composed, old-vs-new on one scene; `demo()` asserts precision lift, no recall loss, track win.
- Math: E9. Old baseline (bearing gate) scores 0.000/0.000 on moving scenes — reported, not hidden; the chain's margin is vs field practice, and vs direct-only tracking where it counts.
- Result: 40 seeds — edges 0.000/0.000 → **0.969/0.902**; track RMSE **1.969 → 0.651 m (−67%)**; estimator error 0.26 m / 3.28°.
- Novelty: no published chain estimates-then-assimilates ghosts (field rejects; ours inverts). Chain-level proof with estimated geometry (not oracle) → contrast 75, proof 82 → 80 KEEP. Path to 90: real labeled-ghost HDF validation, or a new theorem (residual estimator N14?).
- Next: paper + presentation now have their headline (N13/N9+N5+N7); HPCC scripts; N14/N15 frontier.

### Run 13: N12 hybrid consensus — novelty_score=61.0 (DISCARD)
- Timestamp: 2026-09-27 (inline, fast block)
- What changed: `resim_research/hybrid_consensus.py` (new) — local_patch(), coarse-to-fine joint search (100-dir coarse + 20° gate, fine 25-dir patch + relative gate + consume-in-order), evaluate() with robust median + mean transparency; `demo()` asserts operating point + cost fraction + beats-1-DOF.
- Math: E2h. Faults: coarse-grid culling (recall 0.025!), raw-top-k noise admission, mean-metric fragility. Original 1-DOF-local plan REFUTED (== 1-DOF exactly); coarse-to-fine joint is the working variant.
- Result: recall 0.965 / prec 0.862 / 2d_med 0.083 m at 3050 evals (18.2%); 1-DOF re-confirmed 0.845/0.727.
- Novelty: coarse-to-fine is standard practice → prior_art_clear=0 → 61 DISCARD; artifact retained.
- Next: N18 combined chain on real pairs → paper + presentation.

### Run 12: N10 Fermat cylinder RANSAC — novelty_score=63.0 (DISCARD)
- Timestamp: 2026-09-27 (inline, fast block)
- What changed: `resim_research/bend_conditioned.py` (new) — faceted-cylinder truth generator (same-side + sensor-mirror foot + occlusion), Fermat forward-model RANSAC over (Rc,yg) grid with parent-orientation rule; `demo()` asserts lift ≥0.25, rec ≥0.70, prec ≥0.60.
- Math: E2g. Two refutations: midpoint-on-surface invariant (t to 17 m along plane), single-scan (Rc,yg) identifiability (fit (45,3) 31/40).
- Result: 40 cylinder scans — planar rec 0.198/prec 0.220 → bend rec **0.753**/prec **0.834**.
- Novelty: Fu2024 tangential reflectors, Chen2024 guardrail extraction, Jost2025 surface estimation, Ulm wall-pose RANSAC (7 cm) → prior_art_clear=0 → 63 DISCARD; artifact retained.
- Next: N12 hybrid → N18 combined chain → paper + presentation.

### Run 11: N8 associative prefix-scan KF — novelty_score=64.0 (DISCARD)
- Timestamp: 2026-09-27 (inline)
- What changed: `resim_research/parallel_kf.py` (new) — Särkkä operator (vectorized), Lemma-7 element builder, Blelloch up/down-sweep, independent sequential-KF reference, timing ladder 1k→50k.
- Math: E6 verified FROM SOURCE (fetched arXiv:1905.13002 HTML, read Lemma 7/8 + Thm 3). Two real faults: naive elements incompatible (0.1 @step 1), down-sweep operand order. Assoc: abs 2.8e-13 (conditioning) / rel 6.35e-15 ✓. Scan==KF: 1e-12→1.5e-10. Span 16 vs 50000 steps (3125×); single-core wall favors sequential (paper-consistent).
- Result: the HPCC replay engine is built and verified; needs parallel hardware (or batched-GPU port) to realize span.
- Novelty: swarm E CLEAR application, operator prior art → 64 DISCARD; engine retained.
- Next: N10/N12 → N18 combined chain → paper + presentation.

### Run 10: HDF-KPI verification + SiL build verdict + ideas backlog — novelty_score=60.0 (KEEP, infrastructure)
- Timestamp: 2026-09-27 (inline)
- HDF KPIs (user correction: CSV scripts are legacy): NEW UDP HDF KPI (`UDP_KPI/a_persistence_layer.parse_for_kpi`, direct call, no ZMQ server) scores dummy pair1 veh-vs-output **100.00% (50/50)**; identity candidate 100%, +20 mm bias Tier-1 **100%** (Tier-2 failure already proven on CSV path — tier separation cross-confirmed). CAN HDF KPI (`can_kpi_hdf/kpi_main.py`) runs but returns EMPTY tables on Bordnet-decoded HDF: it expects SiL CAN-out flavor (HEADER_STREAM/DETECTION_STREAM), a schema mismatch documented for the HPCC plan; IFV7XX pair additionally has `CEER_ FL` vs `CEER_FL` decoder-version drift.
- SiL build verdict: APT_SRR_RESIM.exe CANNOT be built locally — no MSVC (`cl` absent, VS2015 "Visual Studio 14 Win64" generator required), jfrog binaries need an Aptiv-network token (`JFROG_ACCESS_TOKEN`), tracker sources via `update.py` from F360TrackerLib. Cluster recipe: VS2015 + token + `Build.bat` (srr_dc) + input_path.xml.
- PhD swarm (2 agents): 10 verified papers (arXiv 2609.29342/28400/30176/29912/27506, TAES 2024.3445319/2022.3206256, transfun 2022eap1064, Sensors s22030875, Measurement 114797) → backlog N13–N18 in `autoresearch_research.ideas.md`. Headline seeds: track-conditioned residual estimation (TinyML), conformal async fusion, IMM accel-gate, ghost-conditioned generative augmentation, CoFAR sector prior, N18 combined-ideas chain (the breakthrough vehicle).
- Next: N8 (prefix-scan) → N18 combined chain on real pairs → paper + presentation.

### Run 9: Phase-4 local resim harness (HDF5→CSV→KPI→MF4) — novelty_score=60.0 (KEEP, infrastructure)
- Timestamp: 2026-09-27 (inline, user-requested output-MF4 path)
- What changed: `hdf5_to_kpi_csv.py` (HDF5 SENSOR1 streams → KPI wide-column CSVs + identity/bias candidates + log_path/meta_data), `candidate_mf4.py` (candidate HDF5 → output .mf4 via asammdf), copied `logger.py`+`meta_data.py` from source repo into sandbox, `kpi_work/` test evidence (CSVs + candidate MF4 + KPI reports).
- Key finding: production SiL CANNOT run locally — no APT_SRR_RESIM.exe anywhere (binaries via jfrog at build/HPCC time), C++ decoder lib has no binary either. Local resim = Python candidate path (this harness) + cluster for the true SiL. Vehicle .mf4 holds raw ETH frames (5 radar heads); decoded HDF5 pairs are the workable local input.
- Result: production `detection_matching_kpi_script.py` executes on our CSVs — identity Accuracy **100.0**, +20 mm bias Accuracy **0.0** with Tier-1 yield 100% (tier separation proven); candidate MF4 round-trip bit-exact (ran_0 verified).
- Harness faults fixed: CDC suffix, sensor filename tag, KPI glob double-path bug (absolute log_path required).
- Next: N8 (prefix-scan) → N10/N12 → real-log eval with THIS harness (all 5 dummy pairs + ThunderMCIP) → paper.

### Run 8: F Doppler-RadarSplat MVP — novelty_score=70.0 (KEEP)
- Timestamp: 2026-09-27 (inline, 12h block)
- What changed: `resim_research/doppler_radarsplat_mvp.py` (new) — RadarPrimitive, analytic LOS Doppler + R⁴ projection, Gaussian CDC splat renderer, CLEAN peak extractor with noise-floor termination; `demo()` asserts R⁴ ratio, Jacobian/FD agreement, recovery clean + cluttered.
- Math: E7 verified. Three implementation faults caught: near-far masking (2200× dynamic range blinds 5%-of-max), out-of-band Doppler at 25 m/s ego, runaway CLEAN loop (fixed by absolute floor + cap).
- Result: R⁴ 256.0 exact; grad relerr 9e-9/1e-9; recovery 6/6 both scenes; range RMSE 0.053 m, Doppler 0.036 m/s.
- Novelty: swarm D CLEAR (NeuRadar 2504.00859 confirms the gap; no sim combines all three). Lateral paradigm transfer + new capability → contrast 70 → 70 KEEP (borderline, carried by verification depth).
- Next: N8 (prefix-scan + fast DA) → N10/N12 → synthetic generator → real-KPI eval → paper (N9+N5+N7).

### Run 7: T5 spectral covariance scaling — novelty_score=67.0 (DISCARD)
- Timestamp: 2026-09-27 (inline; driver exited 3/3 stale 02:24, inline continues per 12h directive)
- What changed: `resim_research/spectral_covariance_scaling.py` (new) — R(t)=R0·exp(βH/SNR), Joseph-form update, lead-truck CV + 2 spray bursts; `demo()` asserts spray-RMSE win + PD (min eig > 0).
- Math: E5 verified. No faults (construction guarantees PD; measurement confirms).
- Result: 30 seeds — spray RMSE **0.911→0.315 m (−65%)**, minEig 2.4e-3 > 0.
- Novelty: swarm C CLEAR exact form; Sage-Husa adaptive-R lineage → contrast 60 → 67 DISCARD; artifact retained.
- Next: N7 (F Doppler-RadarSplat MVP, swarm-CLEAR) → N8 → N10/N12 → synthetic generator → real-KPI eval → paper.

### Run 6: T4 kinematics-aware adaptive gating — novelty_score=71.0 (KEEP)
- Timestamp: 2026-09-26 (inline, 12h block)
- What changed: `resim_research/adaptive_gating.py` (new) — CV EKF with coast(), fixed χ² gate vs γ_adapt with finite-difference accel estimate, hard [9.21, 40] bounds; 6 m/s² cut-in + 3 clutter/scan nearest-within-gate scene; `demo()` asserts recall ≥0.97, margin ≥0.05, RMSE strictly better.
- Math: E4 verified. Self-caught SELECTIVITY fault: gate pegged at GMAX (open door, recall 1.000 trivially) → answered with clutter + RMSE metric.
- Result: 40 seeds — recall **0.863→0.996**, maneuver RMSE **0.283→0.212 m**. Fixed-gate 0.863 reproduces the directive's ≈88% baseline regime.
- Novelty: swarm sweep B CLEAR (IEEE 11699489/10726762 = adaptive-Q/init-gating only; no acceleration+innovation-grade γ). prior_art_clear=1, contrast 68 → 71 KEEP.
- Next: N6 (T5 spectral covariance, swarm-CLEAR) → N7 → N8 → N10/N12 → paper (N9 + N5).

### Run 5: T3 async satellite motion extrapolation — novelty_score=68.0 (DISCARD)
- Timestamp: 2026-09-26 (inline, 12h continuous block; 3-agent literature swarm in parallel)
- What changed: `resim_research/async_motion_compensation.py` (new) — exact constant-turn rigid-body reference, directive first-order candidate, 2-satellite U[5,45]ms staleness, `demo()` asserts ≥30% cut over ω=0.1..0.6.
- Math: E3 verified. Sign of R_z challenged explicitly (flipped variant hurts −26% at ω=0.6) — directive survives.
- Result: residual cut 86/76/65/54/43/32% across the yaw sweep (mean ≈59%).
- Swarm novelty (3 research agents, 2026-09-26): A (async comp) CLEAR — 4 URLs adjacent only (IET rsn2.12693, IEEE 5940472/10978756), exact formula absent; B (adaptive gating) CLEAR; C (spectral cov) CLEAR (arXiv 2603.18027, 2512.17505, 2602.21128, 2301.08087); D (RadarSplat) CLEAR (2506.01379, 2504.00859, 2604.13492, 2609.11894); E (prefix-scan) CLEAR (Särkkä + Blelloch primitives known, replay application novel); F (fast DA) MIXED — Kellner T-ITS 8688104 + Danzer RA-L 8954835 cover online calibration core, specific gain schedule + replay injection unverified.
- Scoring: prior_art_clear=1 but lidar deskewing is the same paradigm → contrast 62 → 68, below bar. Discarded; artifact retained.
- Next: N5 (T4 adaptive gating, swarm-CLEAR) → N6 → N7 → N8 → N10/N12 → paper.

### Run 4: N11 Doppler disambiguation of the decoy blind spot — novelty_score=63.0 (DISCARD)
- Timestamp: 2026-09-26 (inline; driver halted 3/3 stale, user selected inline mode)
- What changed: `resim_research/doppler_disambiguation.py` (new) — track-velocity-prior Doppler gate `|vr_b−u_sb·v_a| ≤ K√(SIG_V²+Sv)` (K=3, σv=0.5) on geometric pair candidates + 2-scan confirmation; `demo()` asserts precision lift ≥0.20, GRR ≥0.60, recall cost ≤0.03.
- Math: E2f (equations.md). Design point found by measurement: gate width must come from track σv, not sensor σv; K-sweep (3.0/2.5/2.0) maps the ROC wall.
- Result: 40 decoy + 38 planar scans — decoy prec **0.638→0.877** (+0.239), GRR **0.661**, recall cost 0.009 (decoy) / 0.005 (planar). K=2.0 reaches 0.902 at recall cost 0.07 — documented, not taken.
- Novelty check (web, 2026-09-26): "Multipath Ghost Suppression Based on Doppler Velocity Estimation" (IEEE ICSIDP 2024, doi 10.1109/icsidp62679.2024.10868429) does exactly Doppler-velocity filtering of multipath ghosts with a geometric model; Roos/Ulm-Daimler (d-nb.info/1212452852/34) classifies ghosts via Doppler-distribution/motion-orientation mismatch; US12000957 covers range-Doppler consistency. → prior_art_clear=0, contrast 58. Pair-hypothesis + track-prior-covariance framing is the residual differentiator, not enough. Idea discarded; artifact retained (GRR ≥0.60 meets the directive operating point).
- Insight: exact reproduction of N9's 0.638 decoy number by an independent script strengthens the N9 paper's blind-spot claim.
- Next: N4 (T3 async motion comp) — untouched track, high frontier value.

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
