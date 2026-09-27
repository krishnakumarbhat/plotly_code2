# Autoresearch Research Dashboard: ADAS Radar Perception, TinyML, HPCC, Generative Resimulation

**Runs:** 21 | **Kept:** 9 | **Discarded:** 12 | **Crashed:** 0
**Baseline:** novelty_score: 55.0pts (#1)
**Best:** novelty_score: 80.0pts (#14, +45.5%)

| # | commit | novelty_score | status | description |
|---|--------|---------------|--------|-------------|
| 1 | e2673d8 | 55.0pts | keep | baseline: env audit + field map |
| 2 | f741d8b | 62.0pts (+12.7%) | discard | T1 dual-loop CFAR: recall 0.50→1.00, FA 1→0, prior-art lineage |
| 3 | cdc4890 | **74.0pts (+34.5%)** | **keep** | T2 rank-1 specular displacement `p−p_s=2d·n̂` (exact, 1.066e-14) + ghost-as-virtual-aperture: RMSE 3.242→0.739 m (−77.2%); naive mirror-image model range-exact but bearing-wrong (8.000 m); R_eff coupling +13.2% |
| 4 | 5256da9 | 63.0pts (+14.5%) | discard | N11 Doppler gate: decoy prec 0.638→0.877, GRR 0.661, recall cost ≤0.009; mechanism published (ICSIDP 2024) |
| 5 | f13469e | 68.0pts (+23.6%) | discard | T3 async extrapolation: residual cut 32–86% over yaw sweep; sign challenge survived; lidar-deskewing paradigm |
| 6 | fa2baaf | 71.0pts (+29.1%) | keep | T4 adaptive gating: cut-in recall 0.863→0.996, RMSE 0.283→0.212 m; open-door fault answered; swarm-CLEAR |
| 7 | 71817be | 67.0pts (+21.8%) | discard | T5 spectral covariance: spray RMSE 0.911→0.315 m, Joseph PD holds; adaptive-R lineage |
| 8 | 71112c5 | 70.0pts (+27.3%) | keep | F RadarSplat MVP: R⁴ exact, grad 1e-9, CDC 6/6 recovery; CLEAN fixes; NeuRadar gap confirmed |
| 9 | e4c233a | 60.0pts (+9.1%) | keep | Phase-4 harness: KPI identity 100.0 / bias 0.0, MF4 round-trip exact (infrastructure) |
| 10 | 7bb174a | 60.0pts (+9.1%) | keep | HDF-KPI 100% direct-call; CAN schema documented; SiL unbuildable locally; backlog N13–N18 |
| 11 | 7306464 | 64.0pts (+16.4%) | discard | Prefix-scan KF: rel 6e-15, span 16 vs 50k, Lemma-7 fix from source; operator prior art |
| 12 | fcb56f6 | 63.0pts (+14.5%) | discard | Fermat cylinder RANSAC: curved rec 0.198→0.753; midpoint invariant refuted; wall-RANSAC lineage |
| 13 | 3207679 | 61.0pts (+10.9%) | discard | Coarse-to-fine joint: 0.965/0.862 at 18% evals; 1-DOF-local refuted; standard practice |
| 14 | 8114bb0 | **80.0pts (+45.5%)** | **keep** | N18 closed chain: edges 0→0.969/0.902, track −67%, estimated reflector (breakthrough) |
| 15 | 6a022d1 | 65.0pts (+18.2%) | discard | Residual estimator + INT8: 0.439→0.106→0.108 m, 4.5× fewer mults; 2609.30176 lineage |
| 16 | 0d12b38 | 67.0pts (+21.8%) | discard | Conformal gate: coverage 0.042→0.996, RMSE 17.7→0.263 m; outlier-regime fault noted |
| 17 | b99328e | 70.0pts (+27.3%) | keep | Fast DA A+B+B+: real az −1.145; lock 46 vs 400; ghost seed 1.146° (floor proven) |
| 18 | b99328e | 60.0pts (+9.1%) | keep | Synth 4-scenario generator + halo replay: drops 360k→914→0 (infrastructure) |
| 19 | 78d4ba9 | 67.0pts (+21.8%) | discard | Driving alignment: 13→0.96°; 5 fusions fail documented; 80 stands |
| 20 | 8d301eb | 64.0pts (+16.4%) | discard | Ghost augmentation: FA 0.425→0.033, recall 1.000; normalized stat |
| 21 | 8d301eb | 58.0pts (+5.5%) | discard | Sector pooling: sparse 56→30 (1.86×); fixed-sector win refuted |

## Run 3 — executed evidence (`experiments/run-3.log`)

| Claim | Measurement |
|-------|-------------|
| Rank-1 displacement `p−p_s=2d·n̂` | max abs err **1.066e-14** over 2000 random (p,n,d) |
| Naive "ghost = target mirror image" | range err **0.0 m**, bearing err **15.189°** → **8.000 m** @ 30.3 m |
| Ghost ≠ parent Doppler | offset **−1.987 m/s** = **40σ**; FD check err 5.47e-10 |
| Ghost-pair extraction, 3-DOF | recall **0.993**, precision **0.909**, 2d err **0.077 m** |
| Ghost-pair extraction, 1-DOF | recall 0.845, precision 0.727 — **REFUTED by own baseline** |
| Curved guardrail (E2e) | 2D(φ) band **132 m** = **1320×** range noise; recall → **0.450** |
| Decoy blind spot | precision → **0.638** (provable: same-range pair at exactly 2d) |
| Assimilation (exact ξ) | pos RMSE **3.242 → 0.739 m** (−77.2%, 4.39×) |
| R_eff vs naive diag(R) | +0.0% (ξ=0) → +2.9% → **+13.2%** (0.15 m / 2°) |

## Data integrity
`autoresearch_research.jsonl` = 3 runs, `experiments/worklog.md` = 3 `### Run` entries — **consistent**.

## Next
N10 bend-conditioned test (curved guardrails, blocking) → N11 Doppler disambiguation → N12 hybrid consensus → LaTeX paper for the kept idea.
