# Radar Perception & TinyML Research Sprint — `research.md`

**Date:** 2026-09-26 · **Window:** 2 h sprint (recon → lit → synthesis) · **Venue route:** NeurIPS (methods) + IEEE RadarConf/TAES (domain)
**Budgets (hard):** Xtensa BBE32 @600 MHz · 50 µs SiL slices · params <25k · weights <50 KB int8/fp16 · no transformer/LLM backbones

## 1. Executive summary & scope
Bridge between Aptiv's embedded chain (Gen7 SAF85xx / Gen8 iND13400 FW, DC-SiL resim, VV co-sim, KPI matchers) and 2024–2026 SOTA, under TinyML compilability. Sprint verified: (a) exact estimator math in EmLib (`cfar.c` scrub-threshold chain, `rdd_fp_detection` gates, RDU S1/S2/S3 unfolding), (b) the resim→KPI data path with its brittle exact-key matchers, (c) 11 failure modes with NO covering IDF (F5–F15) + 3 IDFs lacking touchpoints. Output: A-PRISM 6+1 framework, 21 budgeted ideas, numeric spot-checks (4 pass, 1 guard-finding), `resim_research/` sandbox (45 files, copy-only), publication route.

## 2. Unified architecture — A-PRISM / 6+1
- **T1 Range-adaptive dual-loop CFAR** `R0(r,t)`: slow clutter floor `C(r,t)` (λ≈0.02 ex-track cells) + fast local spectral-entropy scale; replaces static `cfar_thold=nf*LUT[idx]` (`cfar.c:804`). Covers F1.
- **T2 Pre-association specular-mirror disambiguation** `G∈[0,1]`: wall-pose-indexed mirror test + Doppler-cone bound `|vr_meas+ve·cosθ|≤tol` + look-persistence; down-weight `1+κG`, never delete. Covers F2.
- **T3 Async satellite motion projection**: extrapolate all heads to common fusion epoch `p(t+dt)=p+v·dt+½a·dt²`, `Rz(ωdt)`; kills yaw skew (F3).
- **T4 Innovation-conditioned adaptive gating** `γ=d²≤g0+k·a²/3`: acceleration-expanded Mahalanobis; cut-in retention (F4).
- **T5 Local spectral covariance scaling** `R(t)`: entropy-driven `R` inflation for rain/spray backscatter (F1-wet).
- **T6 O(log N) associative prefix-scan KF**: Särkkä elements `a=(A,b,C,η,J)` + Blelloch scan over HPCC replay logs; verified associativity err 2.2e-16.
- **Frontier Doppler-RadarSplat**: 4DGS + differentiable Doppler projection `dj=⟨w,v⟩` + `P=σ·α/R⁴` rendering; fixes NeuRadar gaps (no vr/RCS/spectra).

## 3. Math formulations & compact architectures
- **T6 element combine** (verified): `Aij=Aj(I+CiJj)⁻¹Ai`, `bij=AjM(bi+Ciηj)+bj`, `Cij=AjMCiAjᵀ+Cj`, `ηij=AiᵀMᵀ(ηj−Jjbi)+ηi`, `Jij=AiᵀMᵀJjAi+Ji`. Per-element: 2×2 inverse + ~40 MACs; int8-quantizable; Blelloch span O(log T), work O(T).
- **T1 CFAR rewrite**: `thold=nf·(a·ratio+b)`, `ratio=var/R`, 2-seg piecewise-linear replaces 128-entry u4p12 LUT+gather (~30–40 cyc/rbin-angle saved, BBE32 `RECIP` already paid at `cfar.c:771,774`).
- **T2 mirror test**: wall `(α,a)` pose (Ulm RANSAC, 7 cm/0.2°); `GT1..3` closed forms per Table-1; `vr_cone=−ve·cosθ` (spot-check: 20 m/s ego → {−20,−17.3,−10,0} m/s at {0,30,60,90}°).
- **T4 gate**: `g0=9.21` (χ² 2dof 99%) → `{9.21,9.88,11.88,17.38,25.88}` at a={0,1,2,3.5,5} m/s².
- **T5 entropy scale**: `R(t)=R0·(1+β·H(spec)/Hmax)`, H over Doppler-bin power; 1-D CNN (3×1×8ch, ~2k params) or variance shortcut.
- **Micro-CFAR net** (T1/T5 actuator): 1-D CNN over range-profile window (65 taps → 16ch → 8ch → threshold offset), ~8k params int8 ≈ 8 KB, <5 µs/slice est.
- **Frontier rendering**: `Y(r,d)=Σ α(x)·σ(x)·G_az·G_el·leak(r)/R⁴`, Doppler ring integral `dj=⟨w,v⟩`; RCS head per Gaussian + Laplace NLL (NeuRadar probabilistic finding).
- **Guard finding (honest)**: frac-bin `δ=(curr−prev)/((curr−next)+(curr−prev))` yields 0.5714 on test triple — outside ±0.5 without the sign-flip/eps guard (`rdd_fp_frac_binest.c:196-214`); guard is load-bearing, any rewrite must keep it.

## 4. Concrete codebase touchpoints
| Track | File : function/struct |
|---|---|
| T1 | `gen7/.../doppler_proc.c:289,317 Cfar_Init/Appl_Cfar_Execute`; `gen8/.../rdd_proc/imp/src/appl_cfar_ifc.c:71,104`; `cfar.c:185,320,690,785,792,804`; new `clutter_floor.[ch]` |
| T2 | `anglefinding_project_interface.c:Appl_Angle_Finding_Process`; `rdu/src/moving_special_cases.c:541,551,1766`; `detection_matching_kpi_script.py:452-509` (ghost-labeled pairs) |
| T3 | `DC_SIL/sil_wrappers/sil_wrapper.cpp:51 Execute`; `M2D_Look_Info_T`; `SIL_Engine_Config.xml` ego-motion |
| T4 | `f360_tracker.cpp`/`StateManager` gate; `kpi_business.py:_compute_match_pct`; `tracker_matching_kpi_script.py:92-93` |
| T5 | `doppler_proc.c:566` REST cfg; `rdd_fp_detection.c:616,623` thresholds |
| T6 | `resim_research/` replay logs + HPCC burst scripts; python ref impl in sandbox |
| Frontier | `mf4_data/` + `Resim_MF4_Sample_20260925/` corpus; VV OSI traces as pose GT |

## 5. Literature reference matrix
| # | Authors · Year · Venue | Mechanism adapted | Track |
|---|---|---|---|
| 1 | Särkkä & García-Fernández 2021, IEEE TAC | associative scan KF/smoother, O(log n) span | T6 |
| 2 | Corenflos et al. 2021, arXiv:2102.00514 | parallel IEKS/IPLS (nonlinear ext.) | T6 |
| 3 | Odd-even QR smoother 2025, arXiv:2502.11686 | 47×/64-core, SelInv covariances, NC variant | T6 |
| 4 | Särkkä & García-Fernández 2025, arXiv:2511.10363 | GPU scan shootout, two-filter smoother | T6 |
| 5 | SRTM parallel 2025, IEEE SPL | integrated-measurement scan form | T6 |
| 6 | Liu et al. 2024, MECO (CA-CFAR is Convolution) | CFAR≡conv, 35–47× latency cut | T1 |
| 7 | mRadNet 2025, arXiv:2509.16223 | compact MetaFormer detector | T1 |
| 8 | Banerjee et al. 2025, IEEE Sensors (RCTD) | recursive CNN + FPGA DPU | T1 |
| 9 | RadarTCN 2024, Sensors (0.55M params) | causal TCN online classification | T1/T5 |
| 10 | Lee & Salim 2026, MST (TinyML triage C0/C1/C2) | sub-10 ms mitigation routing | T1/F9 |
| 11 | RaDelft 2024, arXiv:2406.04723 | lidar-supervised detector, Chamfer −75% | T1 eval |
| 12 | KI-ASIC/SpiNNaker2 2025, GeMiC | CFAR as SNN/CNN on neuromorphic | T1 hw |
| 13 | Zheng et al. 2024, IEEE TSP | ghost-target detection framework | T2 |
| 14 | Kweon & Monga 2025 (TIGRE) | angle-grid l0 regularizer, low-SNR | T2 |
| 15 | Takahashi & Wang 2025, ICASSP (GREST/ESTAR) | DOD≠DOA ghost test, sparse MIMO | T2 |
| 16 | Jost et al. 2025, IRS | multipath detection + surface estimation | T2 |
| 17 | Chen et al. 2024, ICASSPW | guardrail extraction from clutter | T2 |
| 18 | Ma et al. 2024, ICSIDP | Doppler-velocity ghost filtering | T2 |
| 19 | Shishanov et al. 2025, IRS | Tx/Rx/virtual angle-consistency feature | T2 |
| 20 | Ulm OGM thesis (matched-filter) | wall pose 7 cm/0.2°, backprojection of occluded targets | T2 |
| 21 | NeuRadar, CVPRW 2025 | radar point-cloud NeRF baseline (gaps: vr/RCS/spectra) | Frontier |
| 22 | DART, CVPR 2024 | Doppler tomography, RD rendering | Frontier |
| 23 | RadarSplat 2025, arXiv:2506.01379 | GS + noise/multipath, +3.4 PSNR | Frontier |
| 24 | RF4D 2025, arXiv:2505.20967 | occupancy+RCS + R⁻⁴ power rendering, dynamic | Frontier |
| 25 | 4DRotorGS, SIGGRAPH 2024 | 4D Gaussians, 277 FPS/3090 | Frontier |
*CitedByCounts to be logged via Semantic Scholar API in driver phase (not fabricated here).*

## 6. Next steps (driver backlog)
1. Log citedByCounts; drop sub-bar foundations.
2. Micro-CFAR net offline train (RaDelft-style lidar supervision on mf4 corpus) → int8 → BBE cycle estimate.
3. Wall-pose RANSAC on guardrail frames; TIGRE-vs-mirror-test bake-off on ghost-labeled pairs.
4. T3/T4 SiL A/B via VV configs (yaw-sweep, cut-in scenarios); T6 replay-scan prototype on HPCC logs.
5. Frontier: Doppler-RadarSplat MVP on one mf4 log + OSI poses; Laplace vs Gaussian head ablation.
6. Paper pipeline at novelty ≥70: NeurIPS methods + RadarConf domain twin.
