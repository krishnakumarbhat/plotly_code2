# Equations Log: Automotive Radar HDF5 KPI & Tracking Engine

**Session started:** 2026-08-19

| # | Source (paper/arXiv id) | Original equation | Fault found | Variation derived | Verification (numeric run) | Status |
|---|------------------------|-------------------|-------------|-------------------|---------------------------|--------|
| 1 | Cover & Thomas, *Elements of Information Theory* | KL divergence `D_KL(P‖Q)=Σp·log(p/q)` | asymmetric, unbounded; symmetric gates need a metric | — | `D_KL(P‖P)=0.0`; `D_KL(δ‖U)=ln2≈0.6931`; `D_KL(δ‖U)=0.6931 ≠ D_KL(U‖δ)=13.122` | verified |
| 2 | Cover & Thomas (ibid.) | JS divergence `D_JS=½D_KL(P‖M)+½D_KL(Q‖M)`, `M=½(P+Q)` | bounded, symmetric, `√D_JS` is a metric | — | `D_JS(P‖P)=0.0`; `D_JS(δ‖U)=0.2158`; disjoint bound `D_JS=ln2≈0.6931` | verified |
| 3 | Villani, *Optimal Transport* | Wasserstein-1 `W_1(P,Q)=∫‖F_P−F_Q‖` | metric on matched supports; insensitive to high-`k` tails | — | `W_1(δ_0,δ_1)=1.0`; `W_1` shift-0.5 dist = 0.5429 > `D_JS` shift-0.5 = 0.0395 | verified |
| 4 | Bar-Shalom & Li, *Tracking and Data Association* | Discrete-time KF `x_k=F x_{k-1}+w`, `P_k=F P Fᵀ+Q` | continuous-discrete mismatch: discrete `Q` underestimates noise vs physical PSD | continuous-discrete `Q_c` from `F_c·P·Pᵀ·dt` via `expm`/matrix fraction decomposition | `Q_d[4,4]=q_psd·dt=0.1` closed-form match | verified |
| 5 | Julier & Uhlmann, *UKF* | EKF linearization `y≈h(x̂)+H(x−x̂)` | Jacobian truncation loses moments for polar `h(r,θ)` | — | EKF polar RMSE = 0.3331 m, final pos error 0.2363 m | verified |
| 6 | Julier & Uhlmann (ibid.) | Unscented transform `y_i=h(x_i)`, `ȳ=ΣW_i y_i` | — | — | UKF polar RMSE = 0.3171 m; UT moments `E[y]=2.0`, `Var[y]=6.0` match | verified |
| 7 | — | — | — | Linear KF on `[x,y,vx,vy,ax,ay]` CV model | RMSE = 0.3651 m, tracks line | verified |
| 8 | Bar-Shalom (ibid.) | Mahalanobis gate `d²=yᵀS⁻¹y ≤ χ²₂` | Gaussian-innovation assumption violated under non-Gaussian (specular/multipath) clutter: clumps inside the chi² ellipsoid capture tracks | **Divergence-gated association**: reference = predicted-measurement bump `N(x̂ₖ, Pₓₓ+R)`; candidate = kernel-smoothed histogram of in-gate detections (cluster radius 2.5 m, 32 bins over ±12 m); accept iff `D_JS(pred‖cand) ≤ τ=0.2` | 50% specular clutter, seed=7: JS RMSE 0.185 m vs Mahalanobis 0.199 m (−6.8%); 70%: 0.173 vs 0.217 (−25%), confirmed 102 vs 55. Diffuse Gaussian clutter: parity (JS RMSE 0.198–0.25 vs maha 0.203–0.23) | verified |

**Gate design notes (row #8):**
- Reference bump width rule: `σ = √(P_xx + P_yy)/2 + r` so the bump matches the innovation scale of a well-tuned filter (≈1 histogram bin).
- Candidate smoothing kernel: 3-tap `[0.25, 0.5, 0.25]`; smoothing makes the delta-vs-delta comparison across adjacent bins tolerant (JS target ≈ 0.02–0.08, clutter ≥ 0.30).
- Nearest-in-cluster detection is used for the filter update; the gate decision is cluster-level.
- Critical implementation detail: candidate indices must map back to global frame indices (`positions`), not local cluster indices — a local-index bug caused 20 m track jumps at birth.