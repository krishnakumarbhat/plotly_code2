# Ideas Backlog (swarm-sourced, 2026-09-27 — PhD-level literature)

Scored/filtered by loop rules before execution. None executed yet.

- **N13 — Track-conditioned residual Doppler estimator** (arXiv:2609.30176): tracker predicts phase → remove before FFT → 4-complex-sum residual beat/Doppler (16× reduction, 93.8% info). Gap: cold-start loop + <50KB quantized residual head. TinyML-native. *Next: prototype residual estimator + quantize.*
- **N14 — Conformal async fusion** (arXiv:2609.27506 conformal KF): distribution-free credible regions for OOS/async multi-sensor fusion without Gaussian assumption; extend to cross-sensor extrinsic drift. *Next: conformal wrapper on T3 compensator.*
- **N15 — IMM accel-covariance gate** (Sensors 2022 s22030875 hybrid IMM + gate): r_gate = √(χ² + k·σₐ·|a_IMM|), expands only on accel spike. Extends kept N5 with model-probability grounding. *Next: IMM pair + compare vs N5 energy gate.*
- **N16 — Ghost-conditioned generative RD augmentation** (arXiv:2609.29912 flow matching + N9): synthesize multipath-augmented Range-Doppler conditioned on predicted ghost geometry; train learned CFAR. Attacks cold-start + data scarcity. *Next: condition RadarSplat MVP on N9 pairs.*
- **N17 — Sector-local tracker-masked CoFAR prior** (IEEE TAES 2024.3445319 CoFAR + 2022 transfun knowledge-aided map): replace global prior with track-masked sector prior; merges T1 slow-loop with Bayesian clutter estimation. *Next: VB sector update vs T1 EMA floor.*
- **N18 — Combined-ideas detector** (user directive): fuse kept N9 (ghost aperture) + N5 (adaptive gate) + N7 (generative CDC) + T3/T5 into ONE evaluation chain on real HDF pairs via the harness; old-vs-new depth comparison + presentation. *This is the breakthrough vehicle: each idea is proven solo; the combination is untested.*
