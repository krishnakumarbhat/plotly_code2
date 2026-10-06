# Research Landing Map — where every idea lives and where it must go

Status 2026-09-30. All prototypes, ledgers, monograph, and export package are
committed in THIS repo (`plotly_code2`, branch `research/adas-radar-20260926`,
commit `bd3283e`). Nothing is uncommitted. This file maps each result to its
**production landing repo** — DONE = fully landable here-or-noted, with the
exact target named for everything that cannot live in this sandbox.

## A. DONE in this repo (research artifacts — no further action)

| # | Item | Artifact here | Verdict |
|---|---|---|---|
| 1 | Runs 2–30, all 30 prototypes | `resim_research/*.py`, `run_regression.py` | Executed, PASS |
| 2 | Ledgers | `storage.md`, `equations.md` (E1–E28), `strategies.md`, JSONL Runs 1–30 | Complete |
| 3 | Manuscript + disclosures | `research.md` (§§1–7, IDF-1–8) | Complete |
| 4 | Monograph | `papers/radar_monograph.tex` + `.pdf` (70 pp, 0 errors) | Complete |
| 5 | Export package | `export_aprism/` + bench (18/18 PASS) | Complete |

## B. Lands in `Research/Core_Radar_Gen8_iND13400` (satellite DSP + alignment)

| # | Idea | Score | Exact target block | Needs from elsewhere |
|---|---|---|---|---|
| 6 | T1 dual-loop CFAR (Run 2) | 62 | `software/bbe32/rdd_proc/imp/src/appl_cfar_ifc.c` + `api/` + `test/` | — (self-contained, flag default-off) |
| 7 | N22g Occupancy-BLUE (Runs 23+30) | 62 | same CFAR block; new (πᵢ,sᵢ) API field, zero-init = plain CA | Occupancy predictor feed (object-tracker follow-up) |
| 8 | N14 lag-1 + N23 3-product residual (Runs 15+26) | 65/57 | `software/bbe32/src/range_process_ifc.c`, `doppler_process_ifc.c` | Tracker (f_pred, v_r,p) prediction struct |
| 9 | F5 CDC priority packing | infra | `software/bbe32/src/cdc_packing.c` (35-rec/1472B layout kept) | — |
| 10 | N11 Doppler disambiguation (Run 4) | 63 | `software/bbe32/rdu/src/stationary_moving_classifier.c`, `moving_special_cases.c` | — |
| 11 | Fast-lock DA (Run 17) | 70 | `software/bbe32/dyn_alignment/dyn_align_wrapper.c` + `DA.md` + preflight injection | — |
| 12 | N25/N26/N20g ghost calibration (Runs 25+28/22) | 58/60/68 | `dyn_alignment/` new submodule (yaw + INT64 cert) fed by rdd/AF ghost observables | — (all inputs in-repo) |
| 13 | N27 clock/bias (Run 24) | 60 | `dyn_alignment/` timestamp path + R52 time base (`get_r52_time_from_bbe32.c`) | Independent tracks (object-tracker) |
| 14 | N19 driving alignment (Run 19) | 67 | `dyn_alignment/` (~1° fusion) | — |
| 15 | Satellite-side enablers (all tracking wins) | — | `software/bbe32/emb_tracker/` + stream structs: ghost flags, Doppler scores, occupancy masks, SNR fields | Consumed by central tracker (Phase B) |

Mirror every §B edit into `sil/emb_lib` + `sil/rsp_sil` (per `Design_Doc_RDD_SIL.md`).

## C. CANNOT land here — must go to other repos

| # | Idea | Score | Target repo | Exact target |
|---|---|---|---|---|
| 16 | Run-14 ghost assimilation chain | **80** | `github/core-radar-object-tracker` | `mb_tracker/` EKF: R_eff coupling, dual-path update |
| 17 | N9 rank-1 consensus (3-DOF) | 74 | `github/core-radar-object-tracker` | `mb_tracker/` pair association |
| 18 | N5 adaptive gate | 71 | `github/core-radar-object-tracker` | `mb_tracker/` Mahalanobis gating |
| 19 | N24 rank gate (Run 27) | 63 | `github/core-radar-object-tracker` | `mb_tracker/` association + frozen calibration store |
| 20 | T5 spectral scaling (Run 7) | 67 | `github/core-radar-object-tracker` | `mb_tracker/` covariance update (Joseph form) |
| 21 | Conformal fusion (Run 16) | 67 | `github/core-radar-object-tracker` | fusion layer credible regions |
| 22 | Gram-lift filter (Runs 25+30) | 60 | `github/core-radar-object-tracker` | decoupled Cartesian + yaw-free channels |
| 23 | T3 async deskew (Run 5) | 68 | `github/Core_RESIM_DC_Emb_Library` + object-tracker | fusion epoch alignment |
| 24 | N28 replay certs + prefix-scan | 60/64 | `github/core-resim-hpcc`, `github/core-resim-engine` | shard scheduler, replay operators |
| 25 | KPI threshold updates | infra | `Research/Core_RESIM_KPI` | Tier-1/2 + CAN contracts |
| 26 | N7 RadarSplat + N16 augmentation | 70/64 | `github/core-resim-sensor-model`, `github/core-resim-vv-engine` | generative sim + training data |
| 27 | Gen7 backports (CFAR/gating) | — | `Research/Core_Radar_Gen7_SAF85xx`, `core-radar-gen7-awr294x` | same kernels, Gen7 DSP port |
| 28 | S32R47 variant port | — | `github/core-radar-gen8-s32r47-signal-processing` | duplicate DSP changes |

## D. No action anywhere (refuted / below bar, artifacts retained)

N15 IMM gate (55, REFUTED) · N21g-accuracy (REFUTED, 6.77×) · N17 sector pooling (58, refuted win) · N10/N12 (63/61, superseded by N20g) · N2/N4/N6/N8/N11/N13/N14/N23 (57–68, prior art) · N1 baseline · N18 vehicle (Run 14 executed it).

## Recommended order

1. §B rows 6–7 (CFAR: smallest, self-contained) → 2. §B rows 8–9 → 3. §B rows 11–12 (unblocks HPCC) → 4. §B rows 10,13,14 → 5. §B row 15 enablers → 6. §C Phase B (object-tracker: rows 16–22, highest novelty first) → 7. §C rows 23–26 proof chain.
