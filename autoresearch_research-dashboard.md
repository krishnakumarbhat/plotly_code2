# Autoresearch Research Dashboard: ADAS Radar Perception, TinyML, HPCC, Generative Resimulation

**Runs:** 4 | **Kept:** 2 | **Discarded:** 2 | **Crashed:** 0
**Baseline:** novelty_score: 55.0pts (#1)
**Best:** novelty_score: 74.0pts (#3, +34.5%)

| # | commit | novelty_score | status | description |
|---|--------|---------------|--------|-------------|
| 1 | e2673d8 | 55.0pts | keep | baseline: env audit + field map |
| 2 | f741d8b | 62.0pts (+12.7%) | discard | T1 dual-loop CFAR: recall 0.50→1.00, FA 1→0, prior-art lineage |
| 3 | cdc4890 | **74.0pts (+34.5%)** | **keep** | T2 rank-1 specular displacement `p−p_s=2d·n̂` (exact, 1.066e-14) + ghost-as-virtual-aperture: RMSE 3.242→0.739 m (−77.2%); naive mirror-image model range-exact but bearing-wrong (8.000 m); R_eff coupling +13.2% |
| 4 | TBD | 63.0pts (+14.5%) | discard | N11 Doppler gate: decoy prec 0.638→0.877, GRR 0.661, recall cost ≤0.009; mechanism published (ICSIDP 2024) |

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
