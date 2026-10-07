# Research Paper Library — every source behind Runs 1–30

42 PDFs in `papers/`, all downloaded and magic-byte verified 2026-09-30.
Titles marked [verified] were confirmed from the publisher page/API this
session; titles marked [role] are described by their documented function in
our ledgers — confirm the title page inside the PDF before citing.

Companion "what WE claim" documents: `../papers/radar_monograph.pdf`
(70 pp), `../research.md` (IDF-1–8), `../RESEARCH_LANDING_MAP.md`.

## How to read this library

For each idea, read in this order: (1) the **gap paper** (what's missing),
(2) the **closest prior art** (what blocks novelty), (3) our prototype +
ledger row. The verdict column is the loop's honest scoring, not a claim.

## A. Tracking, gating, filtering foundations

| File | Paper [status] | Governs | Verdict / what to read |
|---|---|---|---|
| `1905.13002.pdf` | Särkkä & García-Fernández, Temporal Parallelization of Bayesian Filters [verified] | N8/Runs 11,25; N21g associativity | Operator is prior art (Lemma 7/8); our replay application + Gram lift are the attempted deltas. Read §III–IV. |
| `2103.07505.pdf` | Wise et al., Continuous-Time 3D Radar-to-Camera Extrinsic Calibration [verified] | N27 observability analysis | Velocity-based calibration without retroreflectors; compare §IV–V vs N27.1–N27.3. |
| `sensors-22-00875-IMM.pdf` | Hybrid IMM Filtering for Radar-Based FCW [verified] | N15 (REFUTED, 55) | IMM is textbook here; our experiment shows pure IMM gating fails onset. Read IMM structure §2–3. |
| `2301.08087.pdf`, `2512.17505.pdf`, `2602.21128.pdf`, `2603.18027.pdf` | Adaptive-R / entropy-adjacent sources [role, per equations E5] | T5/Run 7 (67) | Exact exp-entropy form is CLEAR of these; Sage-Husa lineage caps contrast anyway. |
| `2410.14422.pdf` | Adaptive gating-adjacent [role, per equations E4] | N5/Run 6 (71) | IEEE 11699489/10726762 cover adaptive-Q/init-gating only — our accel+innovation gate is CLEAR. |

## B. Multipath, ghosts, reflectors

| File | Paper [status] | Governs | Verdict / what to read |
|---|---|---|---|
| `2309.13585.pdf` | Zheng et al., Detection of Ghost Targets … (GLRT + CS) [verified] | N9 ghost detection; N26 inputs | Binary detection + angle estimation — no path inversion or reflector recovery. Read §II–III. |
| `2404.01437.pdf` | Kraus et al., The Radar Ghost Dataset [verified] | Ghost ID evaluation | Guardrails/curbs as real specular sources; dataset + identification only. |
| `roos2017-ghost-doppler-thesis.pdf` | Roos et al. 2017, ghost ID by Doppler distribution [verified] | N11/Doppler disambiguation (63) | Doppler already used for discrimination — our envelope relation (N26.5) must be compared § by §. |
| `xin2019-fermat-paths.pdf` | Xin et al., Theory of Fermat Paths, CVPR 2019 [verified] | N26 normals/curvature | Closest theory overlap: normals from path-length derivatives. Our claim must stay on the single-shot radar inverse + certificate. Read §3–4 + supplement. |
| `2505.08240.pdf` | N²LoS mmWave backscatter NLOS localization [verified] | NLOS adjacent | Different mechanism (tags, MUSIC); background only. |
| `2506.01379.pdf` | RadarSplat-adjacent [role, per swarm D] | N7 RadarSplat MVP (70) | Compare per-primitive Doppler + R⁴ + CDC splatting — NeuRadar gap is in `2504.00859.pdf`. |
| `2604.13492.pdf`, `2609.11894.pdf` | Generative-radar adjacent [role, per swarm D] | N7/N16 | Confirm titles in-PDF; background for augmentation claims. |
| `2602.11441.pdf`, `2602.11473.pdf`, `2603.01947.pdf`, `2604.14413.pdf`, `2606.16657.pdf`, `2609.02560.pdf` | Multipath-geometry / DOA-DOD / TIGRE-line sources [role, per prior audits] | N25/N26 claim charts | Each was cited as adjacent art in N25–N28 audits; verify title pages before citing. |
| `2406.00604.pdf`, `2509.14711.pdf`, `2602.05344.pdf`, `2511.14019.pdf` | Multipath-exploitation / reflector-localization line [role] | N26 inverse | Input/target roles are inverted vs ours (they localize targets, we solve the scene) — verify in-PDF. |
| `EP4177638A1-nlos-multipath.pdf` | Aptiv EP4177638A1, NLOS detection with map data [verified] | N26 map-free distinction | Claim 1 **requires** map + roadway — our map-free inverse falls outside literal scope. Read claims 1–10. |

## C. Conformal / robust statistics

| File | Paper [status] | Governs | Verdict / what to read |
|---|---|---|---|
| `2502.04807.pdf` | Bashari–Sesia–Romano, Robust Conformal Outlier Detection (ICML'25) [verified] | N24 (63), Run 16 | Contaminated-reference conservativeness + active cleaning. Our rank-offset + reachability + quorum is adjacent, not overlapping. Read §2–3. |
| `2505.04986.pdf` | Peng et al., Conformal Prediction with Cellwise Outliers [verified] | N24 contamination handling | Detect-then-impute PDI-CP/JDI-CP with 1−2α coverage — different mechanism. |
| `2603.08413.pdf` | GCOS conformally-inspired outlier synthesis [role] | N15-adjacent / synthesis | Shell method; background. |
| `2609.27506.pdf` | Conformalized Kalman filtering [role, per ideas N14] | N15/Run 16 lineage | Distribution-free fusion prior art — caps contrast. |

## D. Residual / track-conditioned DSP

| File | Paper [status] | Governs | Verdict / what to read |
|---|---|---|---|
| `2609.30176.pdf` | Trinh & Shaker, Track-conditioned Residual Frequency Estimation (TCRE) [verified] | N23 (57, direct hit) | Four coherent summaries per 64-sample chirp — our 3-product variant is a narrow implementation delta. Read §2–3. |
| `2609.29912.pdf` | Flow-matching CSI/generative augmentation [role] | N16 augmentation (64) | Generative lineage for ghost-conditioned augmentation. |
| `2609.29342.pdf`, `2609.28400.pdf` | Swarm-A backlog sources [role, per worklog Run 10] | N13–N18 seeds | Verify titles in-PDF before citing. |

## E. Temporal / spatial calibration

| File | Paper [status] | Governs | Verdict / what to read |
|---|---|---|---|
| `2502.00661.pdf` | Kim et al., EKF-RIO with Online Temporal Calibration [verified] | N27 (58) | Time offset in radar/IMU velocity residual + Jacobian (§IV-D/E). Our yaw-free invariant + bias separation is a different information input. |
| `2603.19958.pdf` | Štironja et al., RIO with Online Spatio-Temporal Calibration (B-splines) [verified] | N27 | Joint spatial+temporal in factor graph; already combines both — compare with matched information inputs. Read §III–IV. |
| `2503.02509.pdf` | Štironja et al., Impact of Temporal Delay on RIO [verified] | N27 motivation | Quantifies what misalignment costs — supports the problem, not our solution. |

## F. Generative radar baseline

| File | Paper [status] | Governs | Verdict / what to read |
|---|---|---|---|
| `2504.00859.pdf` | NeuRadar, CVPRW 2025 [verified] | N7 gap (no Doppler/RCS/CDC) | The documented gap our MVP closes. Read §3–4. |

## NOT downloadable (paywalled / landing-page only) — cite, don't claim to have read fully

- ICSIDP 2024 Doppler ghost filtering — doi:10.1109/icsidp62679.2024.10868429 (N11 lineage, blocks novelty).
- Li & Varshney, IEEE TSP ~2014, multipath exploitation (closest to virtual aperture, never states invariant).
- Kellner T-ITS 8688104; Danzer RA-L 8954835 (online calibration core).
- IEEE TAES 2024.3445319 (CoFAR); TAES 2022.3206256 (VI-CFAR Weibull); TAES 2023.3348769 (H∞ MIMO); doi:10.1587/transfun.2022eap1064; Sensors s22030875 ✅ (downloaded); Measurement 10.1016/j.measurement.2024.114797.
- Fu 2024 tangential reflectors; Chen 2024 guardrail extraction; Jost 2025 surface estimation (N10 lineage).
- Patents: US8079022B2 (replay/checkpoint), US11719749B1 (snapshot restore), US20160025844A1 (FMCW timing), US12000957 (range-Doppler consistency), WO2021138220A1 — full text via Google Patents/EPO; claim charts for the first three are already in the ideas file (N28/N27 audits).
- Scholar-only: Roos/Daimler thesis ✅ (downloaded as `roos2017-ghost-doppler-thesis.pdf`).

## Coverage check

Every arXiv ID cited in `autoresearch_research.ideas.md`, `research.md`,
`papers/radar_monograph.tex`, `experiments/worklog.md`, `storage.md` and
`equations.md` is present above except two DOI fragments (`2022.32062`,
`2024.34453` — these are IEEE DOI suffixes, not arXiv IDs) and one garbage
token (`2679.2024`). No cited source was skipped.
