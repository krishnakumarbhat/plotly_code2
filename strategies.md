# Strategy Diversity Log

| Node | Thinking mode × gap type × routing | Why new | Outcome | Status |
|------|-----------------------------------|---------|---------|--------|
| N1 | env-audit × field-map × local-corpus | session seed | baseline 55.0 | kept |
| N2 | critical × assumption-violation (static R0) × RadarConf/CFAR | attacks SOTA's founding average | executed: edge-skirt recall 0.50→1.00, FA 1→0 — but mechanism traces to VI-CFAR/OFPI/ACCA lineage; combination (dual time-scale + track mask + M-of-2) is engineering, not paradigm shift | explored (62) |
| N3 | spatial × missing-paradigm (specular geometry) × RadarConf/multipath | geometry the field thresholds away | **KEPT as N9 (74)**: rank-1 displacement 2dn̂ exact to 1e-14; naive mirror-image model is range-exact but bearing-wrong (15.189° / 8.000 m); ghost-as-virtual-aperture −77.2% RMSE; R_eff coupling +13.2%. 1-DOF sub-claim REFUTED by own 3-DOF baseline | kept (74) |
| N4 | first-principles × domain-fault (async epochs) × filtering/EKF | derives correction from clock axioms | executed: 32–86% residual cut; sign challenge survived; swarm CLEAR but lidar-deskewing paradigm caps contrast | explored (68) |
| N5 | probabilistic × generalization-gap (kinematic gating) × tracking/Mahalanobis | gate as function of maneuver state | KEPT (71): cut-in recall 0.863→0.996 + RMSE −25% under clutter; open-door fault caught via RMSE metric; swarm-CLEAR exact formula | kept (71) |
| N6 | information-theoretic × assumption-violation (Gaussian R0) × filtering/covariance | entropy-modulated trust | executed: spray RMSE −65%, PD holds; swarm-CLEAR exact form but Sage-Husa lineage caps contrast | explored (67) |
| N7 | lateral (graphics→radar) × missing-paradigm (Doppler+CDC synthesis) × CVPR/generative | steals Gaussian splatting for 4D radar | KEPT (70): R⁴ exact, gradients 1e-9, 6/6 CDC recovery; near-far answered by CLEAN; NeuRadar gap confirmed | kept (70) |
| N8 | algebraic × complexity-blowup (O(N)→O(log N)) × NeurIPS-theory/parallel-KF | associative operator + Blelloch scan | executed: Lemma-7 fix + order fix; rel 6e-15; span 16 vs 50k; operator is Särkkä prior art, replay application novel | explored (64) |

Branched from N9 (kept, 74):

| Node | Thinking mode × gap type × routing | Why new | Outcome | Status |
|------|-----------------------------------|---------|---------|--------|
| N10 | first-principles × domain-fault (curved guardrail) × RadarConf | E2e proved the constant-2d spike smears over a 132 m band = 1320× range noise, and guardrails ARE curved — this is the blocking gap for the target environment | executed: Fermat-RANSAC rec 0.198→0.753/prec 0.834; midpoint invariant refuted; fit ambiguous single-scan; wall-RANSAC lineage | explored (63) |
| N11 | probabilistic × missing-paradigm (temporal disambiguation) × tracking | the geometric test is provably blind to a same-range pair at exactly 2d (precision 0.638); only the E2b Doppler invariant (40σ) can separate ghost from decoy | executed: track-velocity gate + 2-scan confirmation → decoy prec 0.638→0.877, GRR 0.661, recall cost ≤0.009 — but Doppler velocity filtering of ghosts published (ICSIDP 2024, Roos/Daimler) | explored (63) |
| N12 | algebraic × complexity-blowup (hybrid consensus) × RadarConf | 1-DOF excess-resultant prefilter + LOCAL 2-D direction refinement should recover 3-DOF accuracy at 1-DOF global cost; directly attacks the E2c refutation | executed: 1-DOF-local FAILED (==1-DOF); coarse-to-fine joint succeeds 0.965/0.862 at 18% cost; E2c re-confirmed | explored (61) |

Hard-ban a thinking-mode category after 3 uses. Same-category repeats are void.

Branched from N13 (kept, 80 — new best, breakthrough chain):

| Node | Thinking mode × gap type × routing | Why new | Outcome | Status |
|------|-----------------------------------|---------|---------|--------|
| N14 | TinyML × assumption-violation (FFT-first DSP) × MLSys | track-conditioned residual Doppler (4 complex sums, 16× reduction) — detector AFTER tracker, not before | executed: range RMSE 0.439→0.106→0.108 INT8, 4.5× fewer mults; beat-is-range fault reframed; lineage 2609.30176 | explored (65) |
| N15 | probabilistic × missing-paradigm (distribution-free fusion) × filtering | conformal credible regions for async/OOS fusion without Gaussian assumption | — | frontier |
