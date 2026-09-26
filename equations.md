# Equations Log: ADAS Radar Perception / TinyML / HPCC / Generative Resimulation

**Session started:** 2026-09-26

| # | Source (paper/arXiv id) | Original equation | Fault found | Variation derived | Verification (numeric run) | Status |
|---|------------------------|-------------------|-------------|-------------------|---------------------------|--------|
| E1 | invented (T1 dual-loop CFAR) | Ĉ_{k+1}(r)=(1−λ)Ĉ_k(r)+λ·median_{d∈D_free} P(r,d), λ≈0.02; s(r)=\|Ĉ(r+1)−Ĉ(r−1)\|/2 ↦ R0(r,t)∈{Rmin,Rmid,Rmax} ±10% hysteresis; T(r,d,t)=α(R0)·Ĉ(r,t)·B(d) | pending: median cost on BBE32; hysteresis chatter at boundaries | pending | not run | frontier |
| E2 | invented (T2 ghost score) | G=σ(w1·φ_pers+w2·φ_mirror+b); φ_pers=1−1/(N−1)·Σ 1[matched_k]·e^{−\|Δθ_k\|/θ0}; p_ghost=p_parent−2((p_parent−p0)·n)n; \|v_ghost−(v_parent+2v_ego·cosψ)\|≤ε_v; second-pass threshold ×(1+κG) | pending: planar-reflector assumption fails on curved guardrails | pending | not run | frontier |
| E3 | invented (T3 async comp) | p_corr=R_z(ωΔt)·p_meas−v_ego·Δt−½a_ego·Δt², Δt=t_DC−t_sat | pending: constant-ω over Δt∈[5,45]ms during transients | pending | not run | frontier |
| E4 | invented (T4 adaptive gate) | γ_adapt=γ0·(1+α·‖a_target‖/a_max+η·‖y_k‖²/Tr(S_k)), clipped [γmin,γmax] | pending: innovation-feedback term risks gate inflation on clutter | pending | not run | frontier |
| E5 | invented (T5 spectral scaling) | R(t)=R0·exp(β·H_spectral(t)/SNR_local(t)); spray ⇒ R→∞, K→0 (coast on CV) | pending: H/SNR estimator itself under spray; exp overflow at SNR→0 | pending | not run | frontier |
| E6 | Särkkä assoc. KF (H prefix-scan) | (Ai,bi,Ci,ηi,Ji)⊗(Aj,bj,Cj,ηj,Jj): Aij=Aj(I+CiJj)^{−1}Ai; bij=Aj(I+CiJj)^{−1}(bi+Ciηj)+bj; Cij=Aj(I+CiJj)^{−1}CiAjᵀ+Cj; ηij=Aiᵀ(I+JjCi)^{−1}(ηj−Jjbi)+ηi; Jij=Aiᵀ(I+JjCi)^{−1}JjAi+Ji | pending: verify associativity ‖(a⊗b)⊗c−a⊗(b⊗c)‖<1e-14 in float64 | pending | not run | frontier |
| E7 | invented (F RadarSplat) | v_{r,i}=(v_i−v_ego)·(μ_i−p_sens)/‖μ_i−p_sens‖; P_{r,i}=Pt·G²λ²σ/(4π)³‖μ_i−p_sens‖⁴; splat into (r,d) bins w/ Laplace heads | pending: R⁴ singularity at close range; Doppler aliasing beyond PRF | pending | not run | frontier |
