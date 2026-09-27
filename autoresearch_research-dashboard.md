# Autoresearch Research Dashboard: ADAS Radar Perception, TinyML, HPCC, Generative Resimulation

**Runs:** 12 | **Kept:** 6 | **Discarded:** 6 | **Crashed:** 0
**Baseline:** novelty_score: 55.0pts (#1)
**Best:** novelty_score: 74.0pts (#3, +34.5%)

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
| 12 | TBD12 | 63.0pts (+14.5%) | discard | Fermat cylinder RANSAC: curved rec 0.198→0.753; midpoint invariant refuted; wall-RANSAC lineage |

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
