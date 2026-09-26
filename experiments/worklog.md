# Worklog: ADAS Radar Perception / TinyML / HPCC / Generative Resimulation

**Session started:** 2026-09-26 | **Venue target:** IEEE RadarConf / CVPRW / MLSys / NeurIPS-theory | **Branch:** research/adas-radar-20260926

## Key Insights
- (Run 1) Local corpus is real and readable: corner-radar HDF5 (CEER groups) + 124-row MF4 manifest + KPI scripts in resim_research/kpi. No cluster access — HPCC work stays a local multi-worker replay simulator.
- (Run 1) gcc/clang absent on this host: C++ prototypes are draft-only until a toolchain appears; Python/NumPy is the verification path.
- (Run 2) Three CFAR faults found numerically, all fixed in prototype: median/mean CA-scaling mismatch (×1/ln2), CUT-relative censor legitimizing walls (→ clean-side min(lead,lag) censor), fixed-N alpha with censored refs (→ alpha(N_eff)). Edge recovery required two-regime wall/skirt censoring + M-of-2 transition confirmation. Lesson: range-only adaptation cannot beat a wide wall without a clutter-map regime switch.
- (Run 2) Novelty verdict: adaptive window/censoring traces to VI-CFAR, OFPI-CFAR (adaptive reference window), ACCA-CMLD/GCMLD (automatic censoring) — combination is solid engineering, not a paradigm shift. Score 62 → DISCARD as research idea; prototype + metrics retained per 35h directive (deviation from skill revert rule, documented here).

## Next Ideas
- T2 ghost score G on overpass synthetic (N3, spatial × missing-paradigm); T3 yaw-sweep residual (N4); T4 cut-in recall (N5); T5 Joseph-form definiteness (N6); F Doppler-RadarSplat MVP (N7); H prefix-scan associativity + speedup (N8).

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
