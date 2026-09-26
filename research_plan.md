# `research_plan.md` — Radar Perception & TinyML Sprint → Publication (detailed)

**Date:** 2026-09-26 · **Venue:** NeurIPS (methods) + IEEE RadarConf/TAES (domain) · **Compute:** local `.venv` (numpy), HPCC burst on overflow
**Sandbox:** `resim_research/` (45 copied files + `SOURCES.md`, copy-only) · **Corpus:** `mf4_data/` (115 logs, 46 GB) + `Research/Resim_MF4_Sample_20260925/`
**Budgets:** BBE32 600 MHz · 50 µs SiL · <25k params · <50 KB int8/fp16 · χ² gates · R⁻⁴ physics

## 1. Idea catalog (21 budgeted ideas, 3 per track)

### T1 — Dual-loop CFAR (covers F1)
- **I1.1 `clutter-floor-lambda`**: per-range slow floor `C(r,t+1)=(1−λ)C+λ·nf_est` (λ≈0.02, ex-track cells only) + slope-mapped `R0∈{Rmin,Rmid,Rmax}` hysteresis; `T=a(R0)·C∗B(d)`. BBE: +2 MUL/ADD per rbin, persist in D2M. Exp: rain/spray logs, Tier-2 +pp.
- **I1.2 `lut-linearize`**: replace 128-entry u4p12 `LUT[idx]` (`cfar.c:792-804`) with 2-seg piecewise-linear `α=a·ratio+b`; saves gather+clip (~30–40 cyc). Exp: threshold-parity sweep vs LUT.
- **I1.3 `micro-cfar-net`**: 1-D CNN (65-tap window →16ch→8ch→offset), ~8k int8 params ≈8 KB, <5 µs; supervised by lidar-projected labels (RaDelft protocol). Exp: Pd/Pfa vs CA/OS-CFAR + MECO-style latency.

### T2 — Ghost disambiguation (covers F2, F9-adjacent)
- **I2.1 `mirror-cone-test`**: wall pose `(α,a)` RANSAC + GT1..3 closed forms + Doppler-cone bound; score `G`, down-weight `1+κG`. Pure C, no ML. Exp: ghost-labeled pairs precision/recall.
- **I2.2 `tigre-vs-mirror`**: reproduce TIGRE angle-grid l0 vs I2.1 on same pairs; low-SNR split analysis. Exp: AUC bake-off.
- **I2.3 `persist-gate`**: look-to-look phase/persistence `φpers` (θ-jitter N=5) fused `G=s(w1φpers+w2φmirror+b)` micro-logreg (m2cgen → C, <1 KB). Exp: FPs on guardrail frames.

### T3 — Async projection (covers F3)
- **I3.1 `ego-extrapolate`**: common-epoch projection `p+v·dt+½a·dt²`, `Rz(ωdt)` at `sil_wrapper Execute` entry. Exp: yaw-sweep disagreement −30–60%.
- **I3.2 `dt-adaptive`**: scale projection by per-head timestamp confidence; fallback hold. Exp: split-track rate.
- **I3.3 `sat-sync-metric`**: new KPI (pairwise echo divergence) into detection matcher. Exp: HLR-1436 story validation.

### T4 — Adaptive gating (covers F4)
- **I4.1 `acc-gate`**: `γ=g0+k·a²/3` in tracker + KPI matcher sweep harness. Exp: cut-in recall +3–8pp.
- **I4.2 `innov-scheduler`**: maneuver probability from innovation sequence switches gate family. Exp: coast/re-init counts.
- **I4.3 `gate-oracle`**: Hungarian deterministic re-association vs greedy `matched_sim_indices` (order-dependence audit). Exp: TP variance at fixed thresholds.

### T5 — Covariance scaling (covers F1-wet)
- **I5.1 `entropy-R`**: `R(t)=R0(1+βH/Hmax)` over Doppler power; variance shortcut if CNN over budget. Exp: spray-log track stability.
- **I5.2 `rest-gate-couple`**: REST on/off driven by entropy regime (links `k_dopp_sll_rest/no_rest`). Exp: false-moving rate.
- **I5.3 `dual-loop-close`**: T1 floor + T5 scale joint tuning via coordinate ascent on KPI. Exp: joint +pp.

### T6 — Parallel scan KF (HPCC/SiL replay)
- **I6.1 `scan-replay`**: Särkkä `(A,b,C,η,J)` elements + Blelloch over log batches; python ref → C port. Verified associativity 2.2e-16. Exp: wall-time vs sequential on 64-core.
- **I6.2 `odd-even-compare`**: replicate QR odd-even smoother on same logs; NC-variant for LM-style loops. Exp: beat/confirm 47× claim on our HPCC.
- **I6.3 `two-filter-dual`**: forward/backward scans on 2 workers (PTFS pattern). Exp: 2-GPU/2-node scaling.

### Frontier — Doppler-RadarSplat (synthetic data engine)
- **IF.1 `doppler-splat-mvp`**: 4DGS + `dj=⟨w,v⟩` projection + `P=σ·α/R⁴`; one mf4 log + OSI poses. Exp: PSNR vs RadarSplat baseline.
- **IF.2 `laplace-head`**: Laplace vs Gaussian NLL for detection confidence (NeuRadar finding). Exp: calibration curves.
- **IF.3 `cdc-synth`**: synthesize CDC-saturation + rain variants for F5/edge training. Exp: KPI lift on saturated scenes.

## 2. Experiment backlog (ordered)
E1 citedByCount logging (Semantic Scholar API) → E2 micro-CFAR offline train → E3 wall-pose bake-off → E4 SiL A/B (yaw, cut-in) → E5 scan-replay port → E6 splat MVP → E7 saturation synth → E8 USS/LM2 conformance → E9 paper pipeline.

## 3. Synthetic-data plan
VV OSI traces = pose GT; RSP-SIL/fast-resim = paired sim/target; `mf4_data/` = edge corpus (rain/Traton/CDC-saturated); RadarSplat-family = novel-view augmentation; guardrail frames = T2 labels via matched-filter backprojection.

## 4. Venue route
NeurIPS: T6 scan-KF, frontier splat, micro-CFAR (methods + theorems + verified equations). RadarConf/TAES: T2 invariants, T1 loops, T3/T4 gating (domain + real-log evidence). Shared `equations.md` ledger; arXiv-first; open code+data.

## 5. Driver ops (multi-hour loop)
Launch `autoresearch-loop.sh autoresearch_research` post-commit. Per iteration: 1 idea (frontier-first graph nav) → math verify → log JSONL + worklog + `equations.md` → paper at novelty ≥70. Budgets enforced per idea; secrets never committed (scan gate); no source-tree writes outside sandbox/loop files.

## 6. Guardrails
No fabricated citations/counts/results · every equation numerically verified · citation bar (≥100 hot / ≥30 young) · TinyML budgets blocking · `Research/`+`github/` read-only sources · 56 quarantined secret files never pushed.
