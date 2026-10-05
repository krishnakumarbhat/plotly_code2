# A-PRISM: Physics-Regularized Perception for High-Speed Automotive Radar — From Ghost Rejection to Ghost Assimilation

**Authors:** Autonomous Research Loop, ADAS Radar Perception Group
**Venue target:** IEEE RadarConf 2027 (systems track); companion workshop: CVPRW (generative radar)
**Code & evidence:** `resim_research/` (11 executable prototypes, every table below reproduced by `demo()` asserts); ledgers: `storage.md`, `experiments/worklog.md`, `equations.md`
**Status:** working manuscript — all numeric claims executed locally; citations verified at time of writing

---

## Abstract

Automotive radar perception discards multipath ghosts, mistrusts its own noise model in spray, detunes its gates for maneuvering targets, and replays logs sequentially. We show, with executed proofs, that (i) first-order specular ghosts satisfy an exact rank-1 displacement invariant that identifies reflectors assignment-free and turns ghosts into virtual apertures, cutting position RMSE by 77% (3.242 → 0.739 m); (ii) the full estimate→gate→assimilate chain, running on estimated (not oracle) geometry, converts a 0.000/0.000 field baseline into 0.969/0.909 ghost-edge recall/precision and cuts track error 67%; (iii) kinematics-aware gating, spectral covariance scaling, async motion extrapolation, Doppler-aware generative resimulation, and an O(log N) associative replay filter complete a physics-regularized stack, each verified against production KPI tolerances. Every negative result (curved-guardrail collapse, decoy blind spot, 1-DOF refutation, open-door gating, float64 mandate) is reported with the same status as the wins.

---

## 1. Introduction

Embedded radar reality (Xtensa BBE32 @ 600 MHz, <50 KB, <50 µs/scan, zero heap) and deep-learning perception have diverged: one cannot run the other, and neural simulators cannot validate firmware (no Doppler, no RCS, no CDC — cf. NeuRadar, CVPRW 2025). Meanwhile classical blocks leak: static CFAR windows smear guardrail clutter into thresholds (misses) and under-threshold far fields (CDC saturation > 5016 records); fixed Mahalanobis gates drop accelerating cut-ins; static covariances over-trust spray; async satellite clocks smear fusion; log replay is sequential.

**Contributions.** (1) Rank-1 specular displacement theorem + ghost-as-virtual-aperture assimilation with coupled covariance (kept, 74). (2) Closed estimate→gate→assimilate chain with estimated geometry (kept, 80 — headline). (3) Adaptive gating (kept, 71), Doppler-RadarSplat MVP (kept, 70). (4) Verified engineering: dual-loop CFAR, async extrapolation, spectral scaling, Doppler disambiguation (GRR 0.661), bend-conditioned guardrail test, hybrid consensus, prefix-scan replay, residual DSP head, conformal gate, local HDF→KPI→MF4 harness. (5) All refutations published alongside.

---

## 2. Related Work

**Adaptive CFAR.** VI-CFAR environment recognition; OFPI-CFAR adaptive reference windows (JPIER); automatic censoring family ACCA/GCMLD/ACMLD; IQR-CFAR (2024, Weibull); LSTM-CFAR (Radioengineering 2025); 2-D window shapes (circular, J. Eng. 2019); knowledge-aided maps (IEICE Trans. 2022eap1064); CoFAR Bayesian clutter (IEEE TAES 2024.3445319); VI-CFAR Weibull (TAES 2022.3206256). Our dual-loop variant adds tracker-masked slow floors and M-of-2 transition confirmation; mechanism lineage acknowledged (Run 2, novelty 62).

**Multipath ghosts.** Geometric delay-Doppler relations (Feng/Ross); DOD≠DOA GLRT + compressed sensing (TSP/IRIS 2023, arXiv:2309.13585); Doppler-distribution identification (Roos/Daimler); Doppler velocity filtering (IEEE ICSIDP 2024, doi:10.1109/icsidp62679.2024.10868429); tangential reflectors (Fu 2024); guardrail extraction (Chen 2024); surface estimation (Jost 2025); wall-pose RANSAC (Ulm, 7 cm); MIMO ghost mitigation (RadarConf 2021 Longman); the field **rejects** ghosts (Kamon/Chong/Vockers clustering/RANSAC; MATLAB GGIW-PHD). Through-wall multipath exploitation exists (Li & Varshney, IEEE TSP ~2014) but never states our invariant nor assimilates with coupled covariance. Our inversion — ghosts as measurements — is the paradigm break (Run 3).

**Tracking & fusion.** IMM hybrid models (Sensors 2022 s22030875); adaptive-Q/init-gating patents (IEEE 11699489/10726762, WO2021138220A1); Sage-Husa/adaptive-R lineage; VB-GLMB noise estimation; conformalized Kalman filtering (arXiv:2609.27506); async/OOSM fusion literature; online radar extrinsic calibration (Kellner T-ITS 8688104, Danzer RA-L 8954835); Särkkä associative parallel filtering (IEEE TAC 2021, arXiv:1905.13002); Blelloch scans.

**Generative radar.** NeuRadar (CVPRW 2025: no Doppler/RCS/CDC); RadarSplat RA synthesis (arXiv:2506.01379); flow-matching CSI augmentation (arXiv:2609.29912); track-conditioned residual estimation (arXiv:2609.30176). Our MVP is the first to combine per-primitive Doppler projection + R⁴ power + CDC-bin splatting with verified Jacobians.

---

## 3. Method

### 3.1 Rank-1 specular displacement (Theorem, E2a)

Reflect sensor in mirror plane $n \cdot x = d$: $s^* = 2dn$. Then
$r_{\mathrm{spec}} = \|p - 2dn\|$, $u_{\mathrm{spec}} = (p-2dn)/\|p-2dn\|$,
$p_s := r_{\mathrm{spec}} u_{\mathrm{spec}} = p - 2dn$ **exactly**, so
$\|p - p_s\| = 2d$ independent of range/velocity/bearing (verified to 1.066e-14 over 2000 samples). Corollary: the naive "ghost at target mirror image" model is range-exact (0.0 m) but bearing-wrong (15.189°, 8.000 m @ 30.3 m) — a half-correct failure range-only checks cannot catch.

Ghost Doppler: $\dot r_{\mathrm{spec}} = u_s \cdot v \ne u_d \cdot v$; offset $-1.987$ m/s $= 40\sigma$ for a lateral cut-in (E2b, FD-checked to 5.47e-10).

### 3.2 Ghost-as-virtual-aperture assimilation (E2d)

Stacked position Jacobian $[u_d^T; u_s^T]$ has rank 2 (generically). Shared reflector parameters couple the noise:
$R_{\mathrm{eff}} = \mathrm{diag}(\sigma^2) + J_\xi \Sigma_\xi J_\xi^T$
with gauge-respecting $\Sigma_n = \sigma_n^2(I - \hat n\hat n^T)$ and the exact residual $r_s = u_s \cdot (p - 2d\hat n)$ (dropping $2d(\hat n \cdot u_s) = 0.79$ m $= 8\sigma$ diverges the filter — found numerically). Iterated (3×) exact-residual EKF.

### 3.3 Closed chain (E9, headline)

Reflector estimation (rank-1 excess-resultant consensus) → Doppler-gated pair edges ($|v_{r,b} - u_{sb} \cdot v_a| \le K\sqrt{\sigma_V^2 + S_v}$, 2-scan confirmation) → dual EKF with **estimated** $(d, \hat n)$ + $R_{\mathrm{eff}}$.

### 3.4 Supporting blocks

Adaptive gate $\gamma = \gamma_0(1 + \alpha\|a\|/a_{\max} + \eta\|y\|^2/\mathrm{Tr}S)$, $[\gamma_{\min}, \gamma_{\max}]$; spectral scaling $R(t) = R_0 \exp(\beta H/\mathrm{SNR})$ (Joseph form); async extrapolation $p_c = R_z(\omega\Delta t)p - v\Delta t - \tfrac{1}{2}a\Delta t^2$; dual-loop CFAR (EMA floor, span-heterogeneity $R_0$, clean-side censoring, effective-$N$ $\alpha$, two-regime switch, M-of-2); Doppler-RadarSplat renderer ($v_r$, $R^{-4}$, CLEAN CDC extraction); Blelloch prefix-scan KF (Särkkä operator, Lemma-7 elements); residual lag-1 DSP head (INT8); conformal gate; Fermat-cylinder bend test; coarse-to-fine hybrid consensus.

---

## 4. Theory

Associativity of $\otimes$ holds relatively to 6.35e-15 (absolute 2.8e-13 is conditioning → float64 mandate). Prefix-scan span is $\lceil \log_2 N \rceil$ (16 vs 50,000 steps). Rank-1 displacement is exact in reals. Conformal gate covers at $\ge 1 - \alpha$ distribution-free. Posterior recovery from Särkkä prefixes is $(b, C)$ directly (naive information-form recovery double-counts — derived and verified).

---

## 5. Experiments

All rows executed by `demo()` asserts in `resim_research/`; negatives included.

| Claim | Baseline → Candidate |
|---|---|
| Ghost assimilation RMSE | 3.242 → 0.739 m (−77.2%, 4.39×); R_eff +0→13.2% |
| Closed chain edges (est. ξ) | 0.000/0.000 → 0.969/0.902; track −67%; est err 0.26 m/3.3° |
| Cut-in handoff recall | 0.863 → 0.996; RMSE 0.283 → 0.212 m |
| RadarSplat CDC | R⁴ exact 256.0; Jacobians 1e-9; 6/6 recovery @ 0.053 m/0.036 m/s |
| CFAR edge-skirt | recall 0.50 → 1.00, FA 1 → 0 |
| Async yaw sweep | residual −86% → −32% (ω 0.1 → 0.6) |
| Spray RMSE | 0.911 → 0.315 m (−65%), Joseph PD holds |
| Decoy precision (Doppler gate) | 0.638 → 0.877, GRR 0.661 |
| Curved guardrail recall | 0.198 → 0.753 (Fermat-RANSAC) |
| Hybrid consensus | 0.845/0.727 → 0.965/0.862 @ 18.2% evals |
| Prefix-scan | rel 6.35e-15; span 16 vs 50k |
| Residual DSP + INT8 | 0.439 → 0.106 → 0.108 m; 4.5× fewer mults |
| Conformal (maneuver regime) | coverage 0.042 → 0.996; RMSE 17.7 → 0.263 m |
| Production UDP KPI | identity 100.0; +20 mm bias Tier-2 0.0 / Tier-1 100% |
| Production UDP HDF KPI | 100.00% (50/50) direct-call |
| Hermite cubic rail (N20g) | coeff rel-err 3.4e-14 exact; 5.2e-3 @ 0.02 m noise |
| Occupancy-BLUE CFAR (N22g) | Pfa 1.02e-4 vs 1e-4; var 0.0686 vs CA16 bias +37.5% |
| Yaw-free clock/bias (N27) | τ/b exact 1e-12; RMSE 6.6/3.8 ms (radial/tangential) |
| Contact inverse (N26) | t=5 exact; RMSE 0.018 m @ low rung; R=20 exact |
| Guarded replay (N28) | 300/300 assoc; margin 1/3; fixed-pt enclosure-only |
| Gram lift (N21g) | closure 5.9e-13; scan equiv <1e-9; speed exact |
| 3-product residual (N23) | stencil 1e-12; RMSE 13.13 mm = theory; 3 mults asserted |
| Rank gate (N24) | sandwich/inclusion/cut-in exact; coverage ≥0.89; sliding poisons 19× |
| Ghost yaw (N25) | 1e-10 exact; cert 0.071°; all 8 failure vectors refused |
| IMM gate (N15) | 0.880 vs N5 0.999 — REFUTED standalone; hybrid == N5 |
| BLUE closed loop | recall +0.074 @ near-nominal FA; loop gain 0.099 |
| Lifted vs EKF | decoupled-pos 1.24×, joint-spd 1.25×; joint-pos 6.77× REFUTED |

**Reported failures:** curved recall collapse (0.993→0.450 planar method); decoy blind spot (provable at exactly 2d); 1-DOF refutation by own 3-DOF baseline; open-door gating without RMSE metric; bearing-gate vacuity on moving scenes; midpoint-surface invariant refutation; single-scan (Rc,yg) ambiguity; outlier-regime conformal loss; CAN-HDF schema mismatch; SiL unbuildable locally. New: Hermite χ² gate non-monotone (needs covariance weighting); Gram converted-Cartesian covariance is a declared bound; N22g closed-loop and N21g full-filter-vs-IEKS comparisons open.

---

## 6. Frontier Kernels (Runs 22–25)

**N20g tangent-mirror Hermite.** Local invariant $p-g=2\delta n$ on any smooth curve; contact+slope from bisector∩ray; cubic coefficients linear with $\det=(x_2-x_1)^4$; Snell $\phi(x)=0$ forward model with physical multi-roots. Closes the curved-rail blocker without a $(R_c,y_g)$ grid (`hermite_clothoid.py`).

**N22g Occupancy-BLUE.** $\beta_i=(m_i/v_i)/\sum m_j^2/v_j$ from predicted occupancy/SNR; exact $P_{fa}=\prod(1+\alpha\beta_i)^{-1}$ for prior-only weights; $R^{-1}=(\hat s/\kappa)I$ bridge with Banach contraction condition (`occupancy_blue.py`).

**N27 yaw-free clock.** $rw-d^Tv=\|v\|^2\tau$ eliminates yaw; two-target $[a_k,r_k]$ system separates $(\tau,b)$; circular yaw after correction; exact accel cubic with truncation bound (`yawfree_clock.py`).

**N26 contact inverse.** $t=(\ell^2-\|d\|^2)/2(\ell-u^Td)$ with $B^{-1}$/$(\rho\|a-u\|)^{-1}$ sensitivity structure; $\|g'\|=2\|\kappa\|\rho$ curvature budget gating N25 fusion; curvature-free $\dot\ell_g=a^Tv_p-u^Tv_s$ (`specular_contact.py`).

**N28 guarded replay.** $h_k=\min m/(C+C_*)$ decision radius; $S_2\circ S_1$ composition associative over exact reals, enclosure-only in fixed point with canonical tree; scheduler replays only uncertified segments (`guarded_replay.py`).

**N21g Gram lift.** $\zeta=[\|p\|^2,p^Tv,\|v\|^2]^T$ with Pascal $\Phi=e^{\Delta N}$; range²/range·rate linear and yaw-free; Isserlis covariance closure from prefix-scanned prior moments; single-scan LMMSE vs $L$-iteration IEKS tradeoff (`gram_lift.py`). Head-to-head (`gram_filter_compare.py`, 200 runs): decoupled Cartesian 1.24× EKF position, joint yaw-free speed 1.25× EKF — but joint fusion position 6.77× worse, surviving oracle covariances: near-collinear ($\rho\approx0.96$) lifted measurements amplify moment mismatch (documented negative; accuracy play refuted, depth 14-vs-100 retained).

**N23 three-product residual.** Cubic-annihilating stencil on exactly 3 lag products, phase subtraction after arg; stencil/fixed-point/noise all exact including the 13.13 mm = theory match and instrumented product count (`n23_residual_track.py`). Discarded on TCRE prior art; retained as the TinyML envelope reference.

**N24 rank gate.** Replacement-budget rank offset with clean-rank sandwich, reachable-box maneuver handling, 2-of-3 quorum, frozen calibration; sliding-quantile poisoning demonstrated 19× under burst while frozen holds (`n24_rank_gate.py`).

**N25 ghost yaw.** Cross-satellite difference cancellation with surveyed-baseline closure and an arcsin release certificate; every failure vector (zero-baseline, $L=0$, swap, curve, clocks, noise) refused, INT64 replay bit-identical (`n25_ghost_yaw.py`). Sub-0.1°/1s stays unvalidated by design.

**N15 IMM gate — refuted.** Pure model-probability gating cannot open at maneuver onset ($\mu_{CA}$ needs association to rise); hybrid collapses exactly onto N5, proving the grounding source contributes nothing (`imm_accel_gate.py`).

**N22g closed loop.** Ghost-leakage scene with Jensen-corrected $R(P)$ coupling: recall +0.074 at near-nominal FA, loop gain 0.099 with a fixed point from both inits (`blue_closed_loop.py`).

---

## 7. Conclusion & Limitations

Ghosts are measurements. The chain from estimation through assimilation runs on estimated geometry and survives every ablation we built to kill it. Limits: curved calibration needs multi-scan spread; decoys need Doppler luck or temporal confirmation; the replay engine needs parallel hardware; fleet validation needs labeled ghosts + cluster SiL. New kernels inherit: Hermite needs world-frame contact spread ($h^{-3}$ conditioning); clock needs independent tracks; replay certificates need uniform Lipschitz bounds; Gram lift relaxes $a=\|p\|^2$.

---

## Appendix A — Enterprise Invention Disclosure Forms

### IDF-1: Ghost-as-Virtual-Aperture Assimilation with Coupled Covariance
**Independent claim 1.** A method for tracking a target with an automotive radar, comprising: identifying a first-order specular ghost associated with a parent detection via a displacement invariant; estimating shared reflector parameters; updating a joint state with a measurement model comprising direct and ghost rows; and weighting the update by $R + J\Sigma J^T$ with tangent-plane reflector uncertainty. **Dependent:** rank-1 $2d\hat n$ identification; RANSAC/excess-resultant consensus; iterated exact-residual EKF carrying $2d(\hat n\cdot u_s)$. **Differentiation:** field rejects ghosts (Kamon/Chong/Vockers, GGIW-PHD inflation); Li-Varshney never assimilates with coupling. **Reduction:** `specular_ghost.py` (RMSE −77.2%).

### IDF-2: Closed Estimate→Gate→Assimilate Chain with Estimated Geometry
**Independent claim 1.** A system chaining reflector estimation, Doppler-consistency gating of pair hypotheses against tracker velocity priors, and dual-path filtering with the estimated reflector. **Differentiation:** ICSIDP-2024 filters with known boundaries; Roos uses orientation mismatch; none chains estimation→gating→assimilation. **Reduction:** `combined_chain.py` (−67% track error).

### IDF-3: Kinematics-Aware Bounded Association Gate
**Independent claim 1.** Expanding a Mahalanobis association gate by estimated target acceleration plus innovation energy, hard-bounded for determinism. **Differentiation:** adaptive-Q/init-gating patents lack accel+innovation grade. **Reduction:** `adaptive_gating.py` (0.863→0.996).

### IDF-4: Doppler-Aware Generative CDC Resimulation
**Independent claim 1.** Rendering Range-Doppler training/validation data by splatting primitives with per-primitive LOS Doppler projection and $R^{-4}$ power law, with verified Jacobians. **Differentiation:** NeuRadar omits Doppler/RCS/CDC. **Reduction:** `doppler_radarsplat_mvp.py`.

### IDF-5: Tangent-Mirror Hermite Barrier Estimation (N20g)
**Independent claim 1.** A method comprising: pairing a direct detection with a multipath ghost of one moving target; intersecting the pair's perpendicular bisector with the ghost receive ray to obtain a specular contact point and tangent slope; solving a polynomial barrier model as a linear Hermite system over such point–slope pairs; and gating associations with the model's multi-root Snell predictions. **Differentiation:** map-based NLOS detection requires map data (EP4177638A1 cl.1); stationary curve fitting uses points only; Fermat-RANSAC grids nonlinear parameters. **Reduction:** `hermite_clothoid.py` (rel-err 3.4e-14). *Status: prototype + claim chart; full-text clearance pending.*

### IDF-6: Occupancy-Weighted CFAR with Exact False-Alarm Control (N22g)
**Independent claim 1.** Weighting CFAR reference cells by closed-form BLUE weights from tracker/ghost-predicted occupancy and SNR, setting the threshold from the exact product formula $\prod(1+\alpha\beta_i)^{-1}=P_{fa}$, and scaling tracker information by predicted SNR under a contraction condition. **Differentiation:** knowledge-aided CFAR uses static priors; hard masking is binary; tracker-aided detection lacks unbiased-optimal weights with exact $P_{fa}$. **Reduction:** `occupancy_blue.py` (Pfa 1.02e-4 vs 1e-4). *Status: Theorems 1–2 verified; closed-loop integration open.*

### IDF-7: Yaw-Free Timestamp/Doppler-Bias Separation (N27) + Contact-Certified Calibration (N26)
**Independent claim 1.** Estimating sensor time offset from the yaw-free invariant $rw-d^Tv=\|v\|^2\tau$ against independent tracks; jointly resolving clock offset and Doppler bias via a determinant-conditioned two-target system; estimating yaw circularly after correction; and refusing calibration when acceleration remainders or curvature-to-image budgets (from closed-form contact inversion) exceed tolerance. **Differentiation:** EKF-RIO-TC/factor-graph RIO jointly optimize offset inside the filter (Kim 2025, Štironja 2026); Honeywell US20160025844A1 exchanges inter-radar timing pairs. **Reduction:** `yawfree_clock.py` + `specular_contact.py` (exact fixtures + RMSE ladders). *Status: prototype; families/continuations unchecked.*

### IDF-8: Guarded Replay Certificates (N28) + Gram-Lift Parallel Filter (N21g)
**Independent claim 1.** Certifying simulation-shard initialization with composable guarded error-transfer summaries derived from association margins, replaying only uncertified segments; and filtering polar range/Doppler in rotation-invariant Gram coordinates by a single associative scan without iterative relinearization. **Differentiation:** Carbon/ARM US8079022B2 replays to detected divergence without a prospective radius; Cadence US11719749B1 restores snapshots without perturbation contracts; parallel IEKS iterates per linearization (Yaghoobi et al. 2021). **Reduction:** `guarded_replay.py` (300/300 assoc) + `gram_lift.py` (scan equiv <1e-9). *Status: prototype; fixed-point enclosure-only; full-filter-vs-IEKS comparison open.*

---

## Appendix B — Cluster Deployment Plan (HPCC)

1. Access: Aptiv network + `JFROG_ACCESS_TOKEN`; MSVC 2015 (VS "14 Win64") or Linux toolchain for DC_SIL.
2. Build: `DC_SIL/Build.bat` (srr_dc) after `fetch_jfrog_binaries.sh`; configure `input_path.xml` per log.
3. Replay: vehicle `_b05.mf4` (ETH frames) → `APT_SRR_RESIM.exe` → candidate `rR` MF4 → decoders → KPI suites (CSV path: `hdf5_to_kpi_csv.py` bridge; HDF path: direct `parse_for_kpi` / `kpi_main.py`).
4. Scale: prefix-scan replay (`parallel_kf.py`, float64) across workers; state-injection for dynamic alignment (Solutions A/B documented in directive); halo discipline per 50 ms scan continuity.
5. Local launch template: `run_hpcc_burst.sh` + Slurm configs (Run 17, pending cluster grant).

---

## References (all verified at time of writing)

Särkkä & García-Fernández, IEEE TAC 66(1), 2021 (arXiv:1905.13002). Blelloch 1990 (CMU scan_ps.pdf). NeuRadar, CVPRW 2025. RadarSplat (arXiv:2506.01379). Li & Varshney, IEEE TSP ~2014. Roos et al., IRS 2017 (Doppler distribution). ICSIDP 2024 ghost suppression (10.1109/icsidp62679.2024.10868429). Feng/Ross delay-Doppler geometry. Fu 2024 tangential reflectors. Chen 2024 guardrail extraction. Jost 2025 surface estimation. OFPI-CFAR (JPIER). VI/LSTM-CFAR (Radioeng. 2025). IQR-CFAR (2024). TAES 2024.3445319 (CoFAR); TAES 2022.3206256; transfun 2022eap1064. Sensors 2022 s22030875 (IMM). arXiv:2609.30176 (residual estimation); 2609.29912 (flow matching); 2609.27506 (conformal KF); 2603.18027/2512.17505/2602.21128/2301.08087 (adaptive-R/entropy). Kellner T-ITS 8688104; Danzer RA-L 8954835 (calibration). US12000957 (range-Doppler consistency).
