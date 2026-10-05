# Ideas Backlog (swarm-sourced, 2026-09-27 — PhD-level literature)

Scored/filtered by loop rules before execution. None executed yet.

- **N13 — Track-conditioned residual Doppler estimator** (arXiv:2609.30176): tracker predicts phase → remove before FFT → 4-complex-sum residual beat/Doppler (16× reduction, 93.8% info). Gap: cold-start loop + <50KB quantized residual head. TinyML-native. *Next: prototype residual estimator + quantize.*
- **N14 — Conformal async fusion** (arXiv:2609.27506 conformal KF): distribution-free credible regions for OOS/async multi-sensor fusion without Gaussian assumption; extend to cross-sensor extrinsic drift. *Next: conformal wrapper on T3 compensator.*
- **N15 — IMM accel-covariance gate** (Sensors 2022 s22030875 hybrid IMM + gate): r_gate = √(χ² + k·σₐ·|a_IMM|), expands only on accel spike. Extends kept N5 with model-probability grounding. *EXECUTED 2026-09-30 (Run 29, E26): IMM-pure 0.880 vs N5 0.999 — REFUTED standalone (onset chicken-and-egg); hybrid == N5. Novelty 55 → DISCARD.*
- **N16 — Ghost-conditioned generative RD augmentation** (arXiv:2609.29912 flow matching + N9): synthesize multipath-augmented Range-Doppler conditioned on predicted ghost geometry; train learned CFAR. Attacks cold-start + data scarcity. *Next: condition RadarSplat MVP on N9 pairs.*
- **N17 — Sector-local tracker-masked CoFAR prior** (IEEE TAES 2024.3445319 CoFAR + 2022 transfun knowledge-aided map): replace global prior with track-masked sector prior; merges T1 slow-loop with Bayesian clutter estimation. *Next: VB sector update vs T1 EMA floor.*
- **N18 — Combined-ideas detector** (user directive): fuse kept N9 (ghost aperture) + N5 (adaptive gate) + N7 (generative CDC) + T3/T5 into ONE evaluation chain on real HDF pairs via the harness; old-vs-new depth comparison + presentation. *This is the breakthrough vehicle: each idea is proven solo; the combination is untested.*

## N23 — Three-Product, Cubic-Exact Sub-Chirp Residual Tracker

### Algorithmic Mechanics & Exact Closed-Form Equations

**2026-09-29 proposal; EXECUTED 2026-09-30 (`n23_residual_track.py` PASS, storage Run 26, equations E23). Novelty 57 → DISCARD (TCRE direct art).** Context audited: `storage.md` Runs 14–17, `equations.md` E9–E12, `strategies.md`, and `Research/SYSTEM_ARCHITECTURE_DEEP_DIVE.md` §§1.3/A.4/A.5. Its eleven-repository map comprises Gen7 SAF85xx, Gen8 iND13400, UDP Decoder, Bordnet, DC Emb Library, HIL Engine, KPI, HPCC, USS Sensor Model, Gen7 AWR294x, and Logic Model; HPCC/USS were placeholders in that snapshot. The separate `github/` mirror contains fourteen directories. No graph artifact was available; no CLI/build/simulation was invoked.

Replace full-window mixing and correlation with **three individual lag products**, followed by scalar phase-increment subtraction. For one already acquired, dominant complex beat, sample at four ADC indices separated by integer lag \(L\), centered at \(t_j=(j-3/2)h\), \(h=LT_s\), \(j=0,1,2,3\). Let \(\Phi\) denote phase in **cycles** and \(\Phi_p\) the predicted phase:

\[
\Phi(t)-\Phi_p(t)=a+ft+\tfrac12gt^2+\tfrac16\ell t^3,
\quad C_j=x_{j+1}\overline{x_j},\quad j=0,1,2,
\]
\[
\eta_{j-1}=\operatorname{wrap}_{[-1/2,1/2)}
\left[\frac{\operatorname{atan2}(\Im C_j,\Re C_j)}{2\pi}
-\{\Phi_p(t_{j+1})-\Phi_p(t_j)\}\right].
\]

Unwrapping is valid only when each true residual increment plus its phase-error bound lies strictly inside \((-1/2,1/2)\). Prediction selects the alias; the four samples cannot independently certify that the prior selected the correct alias. The exact interval equations and inverse are

\[
\eta_-=fh-gh^2+\tfrac{13}{24}\ell h^3,\quad
\eta_0=fh+\tfrac1{24}\ell h^3,\quad
\eta_+=fh+gh^2+\tfrac{13}{24}\ell h^3,
\]
\[
\boxed{\hat f=\frac{26\eta_0-\eta_+-\eta_-}{24h}},\qquad
\hat g=\frac{\eta_+-\eta_-}{2h^2},\qquad
\hat\ell=\frac{\eta_+-2\eta_0+\eta_-}{h^3}.
\]

Thus the center-frequency estimate cancels quadratic and cubic phase contributions without multiplying complex residual samples. The four phase weights are \((1,-27,27,-1)/(24h)\). For bounded sample-phase error \(\epsilon_\phi\) radians and independent equal-variance phase noise:

\[
|\delta f|\le\frac{7\epsilon_\phi}{6\pi h},\qquad
\operatorname{var}(\hat f)=\frac{1460}{576(2\pi h)^2}\sigma_\phi^2.
\]

With \(\lambda=c/(77\,\mathrm{GHz})\), \(K_R=2S/c\), update the scalar beat observation \(f_b=f_{b,p}+\hat f\), where
\[
f_b\simeq K_RR+2v_r/\lambda,\qquad
\hat R=(f_b-2v_{r,p}/\lambda)/K_R.
\]
The range error additionally includes \(2|v_r-v_{r,p}|/(\lambda K_R)\). A single chirp does **not** separately identify range and Doppler; \(g,\ell\) are phase-curvature diagnostics, not accurate single-sub-chirp acceleration estimates. Curvature cancellation handles the local polynomial phase of a maneuver; long-horizon state updates retain the existing tracker and its uncertainty. Multiple unresolved targets, unknown phase jumps, fades, and cold start are outside the one-tone guarantee. A four-point cubic interpolates any four phases: zero fitting residual is not a target-validity test.

### Hardware Feasibility Audit

- **Exactly 3 complex multiplies/update**, implemented by \(\Re C=I'I+Q'Q\), \(\Im C=Q'I-I'Q\): 12 real integer multiplies + 6 adds. No per-sample complex mixer, FFT, CFAR, coherent-sum preprocessing, or learned head is hidden in this count. Scalar prediction/scaling budget: at most 24 additional integer multiplies and 80 adds/shifts; three fixed 20-step CORDICs budget at most 600 add/shift/compare operations. Floating-point operations: **0**; algebraic floating implementation would use fewer than 120 scalar arithmetic operations plus three atan2 calls.
- INT8 complex ADC with a **common I/Q scale**; INT32 products, INT64 phase combinations, fixed-point turn accumulator. For \(|I|,|Q|\le127\), each product component is bounded by 32,258. Do not independently rescale I and Q as in the old fake-quantization path. Learned parameters **0**, INT8 weights **0 KB**; four complex samples 8 bytes, fixed state/scratch budget 256 bytes, 20-entry INT32 CORDIC table 80 bytes: **<0.5 KB** total reserved data.
- Zero-heap construction: fixed four-slot sample storage, fixed scalar state, fixed-iteration CORDIC; no input-sized buffer, recursion, or allocator. \(T_s=100\) ns, \(L=128\) gives a **38.4 µs acquisition aperture**. Target compute budget \(<11\) µs (6,600 BBE32 cycles at 600 MHz) permits acquisition-plus-compute below 50 µs; this is an **unmeasured acceptance budget**, not a WCET result. R52 requires its actual clock and compiler timing. Per-track slice; full-scan capacity scales with track count.

### Patentability Hook

Candidate independent claim: a radar track-update circuit choosing four prior-conditioned ADC timestamps, forming exactly three raw lag products, subtracting predicted phase **after** argument extraction, and applying a cubic-annihilating central-frequency stencil with explicit alias/quantization bounds. Commercial value: constant-work refresh of acquired ADAS tracks between normal detector dwells. Claim the coordinated sampling/phase-budget mechanism, not autocorrelation or polynomial interpolation alone. [TCRE, arXiv:2609.30176](https://arxiv.org/abs/2609.30176) already discloses tracker-conditioned residual estimation and four coherent summaries; generic residual tracking is therefore prior art. Its summaries are not three individual products. Polynomial-phase/PLL patent searches remain necessary; originality is a hypothesis, not an established finding.

### Lower-Tier Model Verification Recipe

1. **Exact stencil:** use \(u=t/h\), residual phase \(0.1u+0.01u^2+0.001u^3\) cycles, arbitrary constant phase, and any known predicted carrier. Expected increments \((0.08325,0.10025,0.12325)\); assert \(\hat fh=0.1\), \(\hat gh^2=0.02\), \(\hat\ell h^3=0.006\) to \(10^{-12}\) in a downstream floating reference. Pure cubic must return \(\hat f=\hat g=0\). Sweep phase offsets and carrier wraps.
2. **Fixed-point bounds:** unit tone scaled to amplitude 80 counts, round both components on the same INT8 grid. Sample angle error is bounded by \(\arcsin(\sqrt{1/2}/80)\); add the implemented CORDIC error. Assert the frequency bound above, no overflow, and no saturation. For \(S=5\times10^{12}\) Hz/s and \(h=12.8\) µs, its quantization-only range bound is approximately **8 mm**, conditional on correct velocity/alias. Test zero amplitude and missing prediction: return invalid, never a fabricated range.
3. **Statistical target, not measured:** complex AWGN at 30 dB per-sample SNR, 10,000 independent trials, isolated target, correct velocity, \(|fh|\le0.2\). High-SNR theory gives \(\sigma_R\approx0.0133\) m using \(\sigma_\phi^2\approx1/(2\mathrm{SNR})\). Acceptance target: actual RMSE \(\le0.025\) m and report alias-loss probability separately; add 0.5 m/s velocity-prior error to expose the approximately 7.7 mm coupling term. Stress a second tone at −20/−6/0 dB and deliberate wrong aliases; report failure rather than extrapolating the one-tone guarantee.
4. Instrument **all** operations from raw ADC samples through the returned beat observation; assert three complex products and the timing/data budgets above. Compare against Run 15 only after correcting `track_conditioned.py`'s baseline audit: its printed “RMSE” is mean absolute error, its parabolic denominator clips the normally negative curvature, and 4,608 is its \(N\log_2N\) accounting convention, not a measured optimized FFT multiply count. Its range conversion also uses oracle velocity. Do not inherit those numbers as a fair speed/accuracy proof.

## N24 — Contamination-Budgeted Rank Gate with Reachability and Sensor Quorum

### Algorithmic Mechanics & Exact Closed-Form Equations

**Analytical proposal; EXECUTED 2026-09-30 (`n24_rank_gate.py` PASS, storage Run 27, equations E24). Novelty 63 → DISCARD (contaminated-conformal art).** Separate maneuver accommodation from noise calibration: expand a physical reachable set, not a quantile of self-selected innovations. Use three sensors, fixed Cartesian scales \(\sigma_{sr}>0\), and independently truth-labeled reference bundles. For reference bundle \(j\), define the **joint** clean noise score
\[
S_j=\max_{s\in\{1,2,3\},\,r\in\{x,y\}}
\frac{|z_{jsr}^{\rm clean}-p_{jr}|}{\sigma_{sr}}.
\]
Coordinates here are at the measurement epoch in a common reference frame; ego rotation/translation are deskewed into that fixed frame, with bounded deskew error included below. Externally fitted transforms/scales are fixed before calibration. Assume \((S_1,\ldots,S_n,S_*)\) exchangeable for the clean measurement population; sensors within each bundle need not be independent. Allow up to \(m\) **arbitrary replacements** among the \(n\) reference scores. Let \(\widetilde S\) be the contaminated array, \(\alpha=0.1\), \(n=255\), \(m=8\):
\[
k=\lceil(n+1)(1-\alpha)\rceil=231,\qquad
q=\widetilde S_{(k+m)}=\widetilde S_{(239)}.
\]
\[
\boxed{S_{(k)}\le q\le S_{(k+2m)}=S_{(247)}}
\quad\Longrightarrow\quad
\Pr(S_*\le q)\ge k/(n+1)=0.90234375.
\]
Proof: replacing at most \(m\) entries shifts any order-statistic rank by at most \(m\); the clean test rank is uniform under exchangeability, with ties conservative. The upper sandwich bounds poisoning-induced width independently of outlier magnitude when \(k+2m\le n\). It does not bound the clean distribution's own tail. If the rank exceeds \(n\), use \(+\infty\) and flag loss of a useful finite gate; silently clipping would invalidate coverage.

For a certified prior position/velocity box at epoch \(t_0\), componentwise acceleration bound \(A_r\), and \(\Delta=t-t_0\ge0\):
\[
\mathcal R_t=\prod_r[c_r-d_r,c_r+d_r],\quad
c_r=\hat p_{0r}+\hat v_{0r}\Delta,\quad
d_r=b_{pr}+b_{vr}\Delta+\tfrac12A_r\Delta^2.
\]
This holds for any measurable acceleration satisfying \(|a_r(t)|\le A_r\), including abrupt cut-ins. Map each candidate sensor measurement to the common epoch using \(\bar z_s=z_s+\hat v_s\tau_s\), \(\tau_s=t-t_s\ge0\); let \(b_{sr}\) cover the bounded velocity-transport, acceleration, extrinsic, and clock errors, e.g. \(b_{vr,s}\tau_s+A_r\tau_s^2/2+b_{\mathrm{geom},sr}\). Define
\[
\mathcal B_s=\prod_r[\bar z_{sr}-q\sigma_{sr}-b_{sr},\,
\bar z_{sr}+q\sigma_{sr}+b_{sr}],
\]
\[
\boxed{\mathcal G_t=\mathcal R_t\cap
[(\mathcal B_1\cap\mathcal B_2)\cup(\mathcal B_1\cap\mathcal B_3)
\cup(\mathcal B_2\cap\mathcal B_3)]}.
\]
An association bundle is admissible iff \(\mathcal G_t\ne\varnothing\). If at most one sensor observation is arbitrarily corrupted and the physical bounds hold, \(S_*\le q\) implies the true state belongs to two honest boxes and \(\mathcal R_t\), hence to \(\mathcal G_t\). Coverage remains **joint marginal inclusion**, not squared per-sensor coverage and not correct identity assignment. Multiple nonempty bundles remain ambiguous; retain competing hypotheses/coast rather than declaring a unique match. A probabilistic prior certificate failing with probability \(\delta\) reduces the bound by at most \(\delta\). Temporal distribution shift, correlated reference episodes that are not exchangeable, or two faulty sensors void this theorem.

Freeze \(q\) through the drive episode; refresh only from a new externally labeled calibration block, never from accepted associations or selectively retained low residuals. Thus Run 16's online quantile-feedback poisoning has no write path. A 64-bin upward quantizer on \([0,1]\), plus an overflow bin representing \(+\infty\), gives \(q_\uparrow\ge q\); finite-bin rounding adds at most \(1/64\).

### Hardware Feasibility Audit

- Learned parameters **0**, weights **0 KB**. Fixed histogram: 65 UINT16 counts = 130 bytes; 255 UINT8 bin IDs if refresh storage is retained = 255 bytes; three 2-D boxes, reachability bounds, and scratch reserve 512 bytes: **<1 KB**. INT32 coordinates with outward-rounded bounds and INT64 intermediates; no INT8 coordinate quantization masquerading as INT8 weights.
- Per pre-associated three-sensor bundle: budget **≤64 real integer multiplies, ≤192 adds/subtracts, ≤96 comparisons** including time transport, tube construction, and three box intersections. Floating-point operations **0**; equivalent scalar arithmetic ≤256. Histogram query ≤65 count additions/comparisons, performed only on calibration activation; ingestion is one bounded counter update per reference score. Fixed arrays/scalars and three explicit pair intersections prove zero heap and bounded work. Candidate enumeration/track assignment is a separate budget: do not hide an unbounded all-to-all search.
- Target **<30,000 cycles / 50 µs** per bounded bundle on BBE32 at 600 MHz; profile R52 independently. Runtime is unmeasured. Keep calibration version, \(m\), scales, and prior certificate in per-worker replay state; never aggregate independently overlapping replay windows as extra calibration samples.

### Patentability Hook

Candidate independent claim: a multi-radar gate whose reference rank is offset by a declared replacement budget, whose width has a clean-rank upper certificate, and whose maneuver expansion occurs exclusively through reachable-set geometry with a two-of-three sensor quorum and a calibration write barrier. Commercial value: cut-in handoff without allowing an outlier burst to enlarge subsequent noise gates. Rank robustness, conformal calibration, reachable sets, and quorum fusion are individually established mechanisms; the coordinated embedded contract is the proposed claim boundary. [Bashari–Sesia–Romano, arXiv:2502.04807](https://arxiv.org/abs/2502.04807) already treats contaminated conformal reference data. No claim of a new general conformal theorem or completed patent clearance.

### Lower-Tier Model Verification Recipe

1. Reference scores \(S_i=i/256\), \(i=1,\ldots,255\). Replace any eight smallest scores by \(10^9\): expect \(q=247/256\); replace eight largest by zero: expect \(q=231/256\). Upward histogram results: respectively \(248/256=0.96875\), \(232/256=0.90625\). Assert the sandwich for mixtures of high/low replacements and overflow-bin handling; test ties and violated contamination budgets explicitly.
2. Exact inclusion vectors: \(p=(10,2)\) m, \(\sigma_{sr}=0.1\) m, honest errors \((0.04,-0.03)\), \((-0.02,0.05)\) m, third sensor replaced by \((100,-100)\) m. With the lower finite quantile above and a reachable box containing \(p\), assert \(p\in\mathcal G_t\) regardless of outlier magnitude. Assert the calibration histogram and \(q\) remain bit-identical during 600 online outlier scans. One nonempty false bundle is **not** a correct-association success.
3. Cut-in: \(p_0=(20,0)\) m, \(v_0=(5,0)\) m/s, \(a_y\in\{6,12\}\) m/s² for 0.5 s; set \(A_y=12\) m/s². Lateral displacement is 0.75/1.5 m. Use sensor ages 0/25/45 ms with the stated transport bound. Assert exact reachable-set containment and retain true bundles whenever their clean score is below \(q\). Add \(a_y=18\), two bad sensors, and false prior bounds as explicit out-of-contract tests.
4. Downstream stochastic KPI: 10,000 independent calibration-plus-test trials per Gaussian, Laplace, and bounded-mixture population; in each trial draw 255 reference bundles and one held-out bundle, corrupt ≤8 reference entries and ≤1 online sensor. Require the one-sided 95% binomial lower coverage bound ≥0.89 for the nominal 0.9023 target; report coverage, finite width, ambiguous bundles, false associations, RMSE, and cut-in recall separately. Independent recalibration tests the **marginal** theorem; a long drive under one frozen calibration is a separate conditional-coverage diagnostic. **99.6% recall is a stretch comparison to Run 6, not implied by 90% conformal coverage.** Compare Run 16's fixed/sliding gates under identical clutter and truth labels; demonstrate quantile immunity, not assumed RMSE superiority.

## N25 — Ghost-Difference Relative Yaw with Surveyed-Baseline Absolute Closure

### Algorithmic Mechanics & Exact Closed-Form Equations

**Analytical proposal; EXECUTED 2026-09-30 (`n25_ghost_yaw.py` PASS, storage Run 28, equations E25). Novelty 58 → DISCARD (ghost-ID literature); sub-0.1°/1s UNVALIDATED.** Azimuth boresight only. Use moving targets simultaneously seen directly and through a shared specular plane. Specify the propagation path before using the ledger's virtual-aperture invariant: for a reciprocal two-leg reflected path \(s\to q\to p\to q\to s\), a plane \(n^Tx=d\), \(\|n\|=1\), gives the physical virtual target
\[
p^*=p-2(n^Tp-d)n,\quad
r_g=\|p^*-s\|,\quad u_g=(p^*-s)/r_g.
\]
These are raw receive-bearing coordinates. E2a's \((p-s^*)/\|p-s^*\|\) is an **unfolded-frame** direction; it cannot replace raw receive AoA without the reflection transform. Mixed direct/reflected paths have a half-sum equivalent range and are excluded. Reproducing synthetic unfolded geometry does not certify physical path conventions.

Represent 2-D vectors as complex numbers; sensor centers \(s_i\) are surveyed in the vehicle frame, with unknown yaw \(\beta_i\). For matched target-time records \(k\), physical local Cartesian measurements satisfy
\[
z^d_{ik}=e^{-\mathrm i\beta_i}(p_k-s_i),\quad
z^g_{ik}=e^{-\mathrm i\beta_i}(p_k^*-s_i),\quad
D_{ik}=z^d_{ik}-z^g_{ik},\quad C_{ik}=\tfrac12(z^d_{ik}+z^g_{ik}).
\]
The **target/reflector nuisance cancels across satellites**:
\[
D_{ik}=e^{-\mathrm i\beta_i}(p_k-p_k^*),\quad
H_{ij}=\sum_{k=1}^{M}D_{ik}\overline{D_{jk}},\quad
\boxed{\hat\delta_{ij}=\arg H_{ij}=\beta_j-\beta_i}.
\]
The sum is over shared records for one fixed sensor pair, not over all ordered sensor pairs. A surveyed nonzero baseline removes the remaining common-yaw gauge:
\[
B_{ij}=s_j-s_i,\quad
\bar b_{ij}=\bar C_i-e^{\mathrm i\hat\delta_{ij}}\bar C_j,
\quad\boxed{\hat\beta_i=\arg(B_{ij}\overline{\bar b_{ij}}),\qquad
\hat\beta_j=\hat\beta_i+\hat\delta_{ij}}.
\]
In noiseless data \(\bar b_{ij}=e^{-\mathrm i\beta_i}B_{ij}\), independently of target motion or wall orientation. No surface-normal estimate, stationary-ground Doppler, iterative optimizer, or startup gain race is required. A connected star handles up to four satellites; each edge needs overlapping observations. Without surveyed baselines, only relative yaw is observable. Distinct curved-surface specular feet generally yield different \(p^*\) per sensor, breaking cancellation; accept a shared planar facet or carry the discrepancy as a bounded input error. The midpoint is not asserted to lie on a curved surface.

**Deterministic precision certificate.** Suppose every direct/ghost Cartesian observation, including correspondence/path, quantization and synchronization error, has norm error ≤\(\varepsilon\); let \(L_k=\|p_k-p_k^*\|\ge L_{\min}>0\), \(\|\bar C_j^{\rm true}\|\le C_{\max}\). Then
\[
|\Delta H|\le\sum_k(4\varepsilon L_k+4\varepsilon^2),\qquad
\rho=\frac{4\varepsilon}{L_{\min}}+\frac{4\varepsilon^2}{L_{\min}^2},\qquad
e_\delta\le\arcsin\rho\quad(\rho<1),
\]
\[
E_b\le2\varepsilon+2C_{\max}\sin(e_\delta/2),\qquad
e_i\le\arcsin(E_b/|B_{ij}|),\qquad e_j\le e_i+e_\delta.
\]
The arcsine bound follows from the tangent to a disk of radius \(|\Delta H|\) around the true complex resultant; no equal-modulus assumption. Add surveyed-baseline angle uncertainty and arithmetic error to the final bounds. A measurable conservative lever bound is \(L_{\min}\ge\min_k|\widehat D_{ik}|-2\varepsilon\); similarly use \(C_{\max}=|\widehat{\bar C}_j|+\varepsilon\). Release alignment only when the complete error certificate is below 0.1°, never merely after a fixed number of frames.

For \(|B|=4\) m, \(L_{\min}=7\) m, \(C_{\max}=10\) m, \(\varepsilon=0.5\) mm: \(e_\delta<0.0164^\circ\), \(e_i<0.0553^\circ\), \(e_j<0.0717^\circ\). Reserving 0.02° for survey/arithmetic gives **<0.092°**. This is a sufficient-input theorem, **not evidence that production radar supplies 0.5 mm Cartesian observations**. Existing 0.35° angle noise at 30 m is approximately 0.18 m cross-range; it does not meet this certificate. Averaging reduces independent random noise, not coherent glint, calibration bias, clock error, or common geometry error. Under the existing noise, sub-0.1° in one second remains an unvalidated target.

### Hardware Feasibility Audit

- Per matched record and sensor edge, the estimator core uses four 2-D sum/difference constructions, one complex product into \(H\), and two complex midpoint sums: **4 real multiplies + ≤24 adds/shifts** from already Cartesian inputs. Finalize the core with two CORDIC vectoring operations, one CORDIC rotation, and ≤12 real multiplies/24 adds. Computing the precision certificate additionally budgets one fixed CORDIC magnitude per record, two at finalization, and ≤32 scalar multiplies/divisions/comparisons; scalar divisions use bounded fixed-point routines. Bound checks can use fixed-point sine thresholds rather than inverse sine. Floating-point operations **0**. Polar-to-Cartesian input conversion, if needed, adds four fixed CORDIC rotations and eight radius multiplies per two-sensor direct/ghost record; include all of these in measured WCET.
- Store only \((H,\sum C_i,\sum C_j,M,L_{\min},C_{\max})\), surveyed centers, and error budgets. Three edges ×128-byte accumulator slots + 512-byte scratch + 80-byte CORDIC table: **<1 KB**, learned parameters **0**, weights **0 KB**. Fixed maximum four sensors and 256 records/epoch; INT64 sums with range-scaling proof, no record history or heap. Four records/edge/slice is a fixed scheduling cap; accumulation and finalization each target <30,000 cycles at 600 MHz, unmeasured.
- These sufficient statistics are additive across distributed replay partitions; merge each unique target-time record once, preserve calibration/coordinate versions, and exclude duplicated halo samples. Fixed-point addition is order-independent only with identical scaling and no saturation/overflow. Publish a snapshot only after the common epoch's required records are complete. Sub-second **observation availability** is separate from sub-50 µs kernel execution.

### Patentability Hook

Candidate independent claim: derive relative radar yaw from cross-satellite direct-minus-ghost vectors, close absolute vehicle-frame yaw through midpoint/baseline cancellation, and condition calibration release on a propagated common-mode error certificate using mergeable constant-size statistics. Commercial value: moving traffic and guardrail multipath become self-calibration observations during short SiL startup windows and stationary-clutter droughts. Distinguish this from Run 17's 46-frame gain scheduling and Run 19's approximately 0.955° normal fusion. Reflection geometry, rigid registration, and multipath calibration have substantial prior art; this exact cancellation-and-release architecture requires a dedicated patent claim search. No novelty score, granted-patent prediction, or measured sub-second result is asserted; available cross-index searches did not establish clearance.

### Lower-Tier Model Verification Recipe

1. **Exact moving scene:** \(s_1=(-2,0)\), \(s_2=(2,0)\) m, \(\beta_1=+1^\circ\), \(\beta_2=-1^\circ\); plane \(y=6\) m; \(p(t)=(8+2t,2+0.5t)\), \(p^*(t)=(8+2t,10-0.5t)\), \(t=0,0.05,\ldots,0.95\) s. Generate **physical** ranges/bearings from the equations above. Here \(L\ge7.05\) m and \(|C_2|<10\) m. Assert \(\hat\delta=-2^\circ\), both absolute yaws recovered within \(10^{-10}\) radians in a downstream floating reference. Repeat with changing target velocities and different shared planar normals; no ground-clutter returns.
2. **Bounded adversary:** perturb each of four Cartesian vectors/record by arbitrary vectors of norm ≤0.5 mm, including coherent same-sign perturbations; enforce exact centers for the base test. Assert all actual yaw errors remain below the derived bounds, and the fixed-point implementation contributes ≤0.005°. Allocate ≤0.015° to a separate surveyed-baseline uncertainty test. Completion deadline: last required record at 0.95 s plus scheduled finalization before 1.00 s; measure maximum slice duration, not mean latency.
3. **Failure vectors:** zero baseline; target on reflector (\(L=0\)); missing common ghost; swapped parent/ghost at one sensor; mixed-path range; distinct glints; curved rails of radius 30/100 m; clock offsets 5/25/45 ms; and 0.35° angle noise at 30 m. Require singular cases to refuse certification and bound violations to be exposed by the independent input/path certificate. Algebraic consistency alone cannot detect every coherent wrong correspondence. Never silently report “<0.1°” from a relative-yaw estimate or from averaging beyond the one-second deadline.
4. **Replay/resource assertions:** partition identical records into 1/2/20 disjoint worker groups, merge fixed-point statistics, and require bit-identical output to single-worker accumulation without overflow. Include a duplicated-halo negative control. Report calibration availability fraction, absolute and relative angle errors, path-rejection rate, worst-case cycles, and maximum static memory; retain Run 14/17/19 as separately labeled historical synthetic baselines, not performance evidence for N25.

## N26 — Inverse Specular Contact Geometry with a Curvature-to-Calibration Error Certificate

### Algorithmic Mechanics & Exact Closed-Form Equations

**Research handoff, 2026-09-29; EXECUTED 2026-09-30 (`specular_contact.py` PASS, storage Run 24, equations E20).** New direction relative to N25: recover the actual reflecting contact and quantify when curved-rail ghosts cease to share a virtual target. Do not fit a global cylinder before interpreting the echo. The inverse problem can be simpler than the forward Fermat problem.

Inputs: known sensor position \(s\), independently estimated simultaneous parent position \(p\), unit physical receive ray \(u\), and ghost equivalent one-way path length \(\ell\). Restrict the path to \(s\to q\to p\to q\to s\), with one smooth static reflecting surface and one target scattering center. This is two surface encounters over the round trip, not a mixed direct/reflected path. Set \(d=p-s\), \(q=s+tu\), \(\rho=\ell-t\). Since \(\ell=t+\|d-tu\|\), squaring cancels \(t^2\):

\[
\boxed{t=\frac{\ell^2-\|d\|^2}{2(\ell-u^Td)}},\qquad
q=s+tu,\qquad a=\frac{p-q}{\rho},\qquad
n=\frac{a-u}{\|a-u\|}.
\tag{N26.1}
\]

Here \(a\) is the target-side propagation direction and \(n\) the specular normal toward the sensor side. “Specular contact” means a stationary point of path length constrained to the surface, not a perpendicular projection. Require \(\|u\|=1\), \(\ell>\|d\|\), \(0<t<\ell\), \(\rho>0\), and \(\|a-u\|>0\), all with uncertainty margins. At \(\ell=\|d\|\), a ray on the direct segment may admit a continuum of contacts; never choose \(t=0\) to hide the degeneracy. A nonunit quantized \(u\) destroys the cancellation unless renormalized or enclosed by interval arithmetic.

**Sensitivity structure.** Holding \(s\) fixed and writing \(B=\ell-u^Td\), \(P_n=I-nn^T\):
\[
\partial_\ell t=\frac{\ell-t}{B},\quad
\nabla_p t=\frac{q-p}{B},\quad
\nabla_u t=\frac{td}{B},\quad dq=u\,dt+t\,du,
\]
\[
\boxed{dn=\frac{P_n\,[dp-\ell\,du-a\,d\ell]}{\rho\|a-u\|}}.
\tag{N26.2}
\]
The \(dt\) term disappears from the normal differential because it is parallel to \(n\). This exposes two distinct noise amplifiers: \(B^{-1}\) for contact location and \((\rho\|a-u\|)^{-1}\) for orientation. Range/parent errors move the contact along the receive ray; angular error also moves it across the ray with gain \(t\). These are local Jacobians, not global finite-error certificates. Use a full joint covariance or outward interval enclosure; the parent, ghost angle and ghost range are generally correlated.

**Own extension: an exact curvature budget for virtual-target disagreement.** In a 2-D cross-section, let \(q(\sigma)\) be arc-length parametrized, \(T=q'\), \(n'=\kappa T\), and hold the parent \(p\) fixed. Reflect it across each local tangent:
\[
g(\sigma)=p-2[(p-q)\cdot n]n.
\]
At a specular contact this equals the apparent raw ghost location \(s+\ell u\). Differentiation gives
\[
g'=-2\kappa\{[(p-q)\cdot T]n+[(p-q)\cdot n]T\},\qquad
\boxed{\|g'\|=2|\kappa|\|p-q\|},
\]
\[
\boxed{\|g(\sigma_2)-g(\sigma_1)\|
\le 2\kappa_{\max}\rho_{\max}|\sigma_2-\sigma_1|}.
\tag{N26.3}
\]
This is an integral bound, not a small-arc approximation with an omitted remainder. It converts curvature into an explicit virtual-aperture calibration error. For a circle it is \(2\rho_{\max}|\Delta\theta|\); a chord is **not** an upper bound on arc length. If two radar views violate the N25 common-image budget, do not fuse their ghost differences as one calibration observation. For \(R=30\) m, \(\rho_{\max}=5\) m and a 0.5 mm permitted image discrepancy, the sufficient arc separation is only 1.5 mm: arbitrary curved-rail ghosts do not rescue N25's precision requirement.

For signed radius estimation from contacts/normals with a consistent orientation:
\[
q_j=c+Rn_j\ \Longrightarrow\
\hat R=\frac{\sum_j\Delta q_j\cdot\Delta n_j}{\sum_j\|\Delta n_j\|^2},\quad
\hat c=\overline{q-\hat Rn}.
\tag{N26.4}
\]
Equation N26.4 is exact for noiseless circles; it is an errors-in-variables estimator under noise, and fitting two normals does not certify a curvature upper bound between contacts. A finite-sample upper-curvature certificate needs an independently justified smoothness bound or a richer surface model. Otherwise return “curvature unbounded between samples.” Individual normal flips leave N26.3's image unchanged but corrupt N26.4 unless signs are aligned.

**Curvature-free first-order velocity.** Stationarity removes tangential contact motion from the derivative of the path value:
\[
\dot\ell_g=a^Tv_p-u^Tv_s,\qquad
u_d^Tv_p=\dot r_d+u_d^Tv_s,\quad u_d=d/\|d\|,
\]
\[
\begin{bmatrix}u_d^T\\a^T\end{bmatrix}v_p
=\begin{bmatrix}\dot r_d+u_d^Tv_s\\\dot\ell_g+u^Tv_s\end{bmatrix}.
\tag{N26.5}
\]
The 2-D velocity solve is available when \(u_d\times a\ne0\); its condition number is \(\sqrt{(1+|u_d^Ta|)/(1-|u_d^Ta|)}\). Curvature disappears from this derivative but still affects \(a\) and its uncertainty. Moving reflectors add a normal-motion term; rough/diffuse scattering, path switching and glint jumps invalidate the differentiable stationary-path model. Estimating \(p\) from the same ghost and then treating N26.5 as independent information double-counts evidence.

### Hardware Feasibility Audit

Fixed-point implementation target: **0 learned parameters, 0 KB weights**, INT32 geometry and INT64 intermediates. Reserve 16 contact/normal records ×32 bytes plus 1 KB scratch/covariance/certificates: **<2 KB**. Per paired 2-D echo, allocate ≤200 scalar multiply/add operations, ≤8 divisions and ≤4 square-root/normalization operations including inverse geometry, sensitivities and the 2×2 velocity solve; a 16-record curvature reduction receives its own slice. These are design budgets, not a measured instruction count. Fixed-count reciprocal/sqrt iterations or platform primitives, fixed arrays and bounded records imply zero heap. Acceptance: every kernel slice <30,000 BBE32 cycles at 600 MHz / <50 µs; measure R52 at its actual clock. Association, upstream AoA estimation and proving interval bounds have separate costs that must be included in an end-to-end claim.

### Patentability Hook & Internet Prior-Art Check

Candidate claim elements: **(A)** parent-conditioned contact/normal inversion from physical path range and AoA; **(B)** propagation of contact/normal uncertainty; **(C)** curvature-to-virtual-target discrepancy certificate; **(D)** selective switching between common-image yaw calibration and curvature-aware velocity assimilation. The possible invention is that coordinated decision mechanism, not ray–ellipse intersection, reflection, circle fitting or the envelope theorem.

- **Directly inspected:** [Xin et al., “A Theory of Fermat Paths for Non-Line-Of-Sight Shape Reconstruction,” CVPR 2019, pp. 6800–6809](https://openaccess.thecvf.com/content_CVPR_2019/html/Xin_A_Theory_of_Fermat_Paths_for_Non-Line-Of-Sight_Shape_Reconstruction_CVPR_2019_paper.html). Its abstract explicitly derives surface normals from spatial derivatives of Fermat path lengths. This defeats a broad “recover shape from specular path derivatives” novelty claim. Compare its full theory and supplement against N26.2–N26.3 before asserting a new theorem.
- **Patent text and independent claim examined:** [EP4177638A1, Aptiv, “Detection and localization of non-line-of-sight objects using multipath radar reflections and map data,” published 2023-05-10](https://data.epo.org/publication-server/rest/v1.0/publication-dates/20230510/patents/EP4177638NWA1/document.html). Claim 1 uses map data to determine whether a direct-path interpretation lies within a roadway, then tests multipath viability and indicates an NLOS object. Description §§0038/0044 derives reflection geometry from map/sensor reflector information. N26 starts with a known parent and estimates a previously unknown contact, then budgets cross-view calibration error. This is a claim-feature distinction, not a freedom-to-operate conclusion.
- **Adjacent literature identified by MiMo:** [Zheng et al., arXiv:2309.13585](https://arxiv.org/abs/2309.13585), multipath detection/angular estimation; [Radar Ghost Dataset, arXiv:2404.01437](https://arxiv.org/abs/2404.01437), real ghost data. Neither abstract-level comparison is sufficient to exclude related equations in full text. Classical bistatic localization/AOA–TOA reflector reconstruction and sonar Fermat-flow work remain mandatory search families.
- Searches actually attempted: arXiv `radar specular surface curvature` (no result for that exact conjunctive query); `radar multipath reflector localization` (two results, including [N²LoS, arXiv:2505.08240](https://arxiv.org/abs/2505.08240)); Google Patents `radar multipath curvature` returned broad unrelated/adjacent results. **No exact combination was established in the inspected sources; worldwide novelty remains unresolved.** An empty query is weak evidence.

### Lower-Tier Model Verification Recipe & Research Decisions

1. **Analytic fixture:** \(s=(0,0),p=(6,0),u=(3/5,4/5),\ell=10\). Expect \(t=5,q=(3,4),\rho=5,a=(3/5,-4/5),n=(0,-1)\). Expect \(\partial_\ell t=0.78125\), \(\nabla_pt=(-0.46875,0.625)\). With \(v_s=(1,0),v_p=(0.3,-0.2)\), expected direct/ghost range rates −0.7/−0.26 m/s and recovered velocity exactly \((0.3,-0.2)\). Assert downstream reference errors <\(10^{-10}\), then derivative relative error <\(10^{-5}\) away from singularities.
2. **Circle fixture:** \(q_1=(3,4),n_1=(0,-1),q_2=(-9,8),n_2=(-3/5,-4/5)\). Expect \(R=20,c=(3,24)\). Generate physical sensor/target paths satisfying the reflection law; do not create “ghosts” by assuming the invariant being tested. Compare interval-enclosed N26.3 with actual apparent-image differences over radii 20/30/100 m and independently supplied arc bounds.
3. **Noise ladder:** \(\sigma_\ell=0.01/0.05/0.10\) m, \(\sigma_\theta=0.05/0.15/0.35^\circ\), parent position standard deviation 0.02/0.10/0.30 m; incidence angles 0/30/60/80/89°. Report contact error, normal error, velocity RMSE, curvature coverage and rejection fraction. At the fixture, first-order propagation for \(\sigma_\ell=0.01\) m, \(\sigma_\theta=0.05^\circ\), independent isotropic parent noise 0.02 m predicts contact RMS around 0.02 m; use **0.05 m RMS as a provisional acceptance target**, not a result. Stress correlated errors separately.
4. **Kill conditions:** one accepted interval certificate that misses the true discrepancy; treating an unobserved high-curvature inter-contact segment as bounded; singular \(B,\rho\) or grazing geometry returning a confident finite answer; a gain disappearing when the same ghost is excluded from the parent estimate. Reject claimed novelty if the four claim elements are already disclosed together, or if the mechanism reduces to published inverse reflector mapping plus an ordinary threshold without additional demonstrable benefit.
5. **Research burden for the next model:** establish a global interval form of N26.2; quantify errors-in-variables curvature bias; derive a valid surface-regularity assumption for N26.3; compare with local `bend_conditioned.py`, `combined_chain.py`, and `ego_ghost_alignment.py`. Promotion target: curved-rail tracking improves over a physically matched planar baseline while calibration false acceptance remains bounded. Do not promise a numerical SOTA margin before testing.

## N27 — Yaw-Free Timestamp and Doppler-Bias Elimination from Moving-Target Range Products

### Algorithmic Mechanics & Exact Closed-Form Equations

**Analytical research handoff; EXECUTED 2026-09-30 (`yawfree_clock.py` PASS, storage Run 24, equations E19).** Replace a coupled yaw/time optimizer with a scalar rotational invariant, then use scene diversity to separate timestamp error from Doppler DC bias. This estimates **measurement-epoch offset**, not picosecond RF phase synchronization or sampling-clock skew.

Use a leave-one-sensor-out track giving relative position \(d_k\) at nominal epoch and constant relative velocity \(v_k\), both in a known nonrotating reference frame. Incorporate the sensor's lever-arm motion in relative velocity; the formula does not permit unmodeled vehicle rotation/acceleration. An actual measurement occurs at nominal time plus \(\tau\):
\[
x_k=d_k+v_k\tau,\qquad r_k=\|x_k\|,\qquad
w_k=\frac{x_k^Tv_k}{r_k}.
\]
Multiplying the measured range and range rate eliminates the line-of-sight normalization and all sensor yaw:
\[
\boxed{r_kw_k-d_k^Tv_k=\|v_k\|^2\tau},\qquad
\boxed{\hat\tau_k=\frac{r_kw_k-d_k^Tv_k}{\|v_k\|^2}}.
\tag{N27.1}
\]
With nonzero relative speed, known translational track geometry supplies time information even under constant velocity; an ego-velocity-only calibration loses that information under constant motion. This is a different information input, not a contradiction of existing observability results. Tangential motion \(d^Tv=0\) is **not** singular when \(\|v\|>0\); timing appears through \(rw=\|v\|^2\tau\).

**Own extension: separate clock offset and common Doppler DC bias before yaw.** Let \(w_k^{obs}=w_k+b\), with a common \(b\) and \(\tau\) for the selected simultaneous bundle, accurate range scale, and unaliased Doppler. Define \(y_k=r_kw_k^{obs}-d_k^Tv_k\), \(a_k=\|v_k\|^2\). Then
\[
y_k=a_k\tau+r_kb,\quad D=a_1r_2-a_2r_1,
\]
\[
\boxed{\hat\tau=\frac{y_1r_2-y_2r_1}{D}},\qquad
\boxed{\hat b=\frac{a_1y_2-a_2y_1}{D}}.
\tag{N27.2}
\]
Rank fails exactly when speed-squared/range ratios coincide. Equal ranges alone do not cause singularity if speeds differ. Normalize the columns by fixed engineering scales before applying a conditioning threshold. With \(K\le8\) targets, use the explicit inverse of the 2×2 Gram matrix \(G=\sum_k\omega_k[a_k,r_k]^T[a_k,r_k]\), \(h=\sum_k\omega_k[a_k,r_k]^Ty_k\); \((\hat\tau,\hat b)^T=G^{-1}h\). These are initial estimates under noisy regressors; ordinary LS is not unbiased errors-in-variables inference.

Having corrected time, estimate azimuth yaw on the circle:
\[
\hat\beta=\arg\sum_k\omega_k
\frac{x_{kx}+\mathrm i x_{ky}}{\|x_k\|}\,e^{-\mathrm i\theta_k},\qquad
x_k=d_k+v_k\hat\tau.
\tag{N27.3}
\]
Require nonzero resultant and known elevation/roll/pitch treatment. N27.3 returns sensor orientation in the chosen reference frame; subtract independently known vehicle heading at the corrected epoch to obtain mounting yaw. Bundle records spanning vehicle rotation must be individually heading-corrected before circular averaging. A practical branch chooses records by normalized determinant and predicted uncertainty, not simply by largest Doppler magnitude. Reuse N24's bounded candidate handling; do not let the sensor being calibrated determine its own supposedly independent \(d,v\).

**Uncertainty and maneuver remainder.** For N27.1 with separately observed \((r,w)\) and track estimates \((d,v)\), its partial Jacobian is
\[
J_\tau=\left[\frac w{\|v\|^2},\frac r{\|v\|^2},
-\frac{v^T}{\|v\|^2},-\frac{(d+2\tau v)^T}{\|v\|^2}\right],
\quad\sigma_\tau^2\approx J_\tau\Sigma_{r,w,d,v}J_\tau^T.
\tag{N27.4}
\]
No independence assumption is necessary if the full covariance is known. At long range, velocity bias contributes approximately \(-d^T\delta v/\|v\|^2\): tiny algebra does not imply precise calibration.

For known constant relative acceleration \(a\), \(x=d+v\tau+a\tau^2/2\) gives the **exact cubic**
\[
rw-d^Tv=(\|v\|^2+d^Ta)\tau+\frac{3}{2}(v^Ta)\tau^2+\frac{1}{2}\|a\|^2\tau^3.
\tag{N27.5}
\]
With \(|\tau|\le T\) and \(c_1=\|v\|^2+d^Ta\ne0\), a linear estimate using \(c_1\) has truncation bound
\[
|\hat\tau_{lin}-\tau|\le
\frac{1.5|v^Ta|T^2+0.5\|a\|^2T^3}{|c_1|}.
\]
The cubic is uniquely invertible on the chosen interval if its derivative \(\|v+a\tau\|^2+x(\tau)^Ta\) has a certified constant nonzero sign. Unknown acceleration, range bias, Doppler scale error or clock drift introduces additional nuisance variables: two targets no longer suffice. For the minimal micro-kernel, reject intervals whose acceleration remainder exceeds the timing budget; do not silently run unconstrained cubic root selection.

### Hardware Feasibility Audit

Zero learned parameters/weights; a fixed eight-record store, per-record uncertainty and a 2×2 accumulator reserve **<4 KB**. Target ≤512 scalar multiply/add operations, ≤16 bounded divisions, ≤8 fixed CORDIC angle/rotation evaluations per eight-target bundle, with covariance processing split into a second slice if required. INT32 physical quantities and INT64 products with a declared maximum range/speed; no heap or variable-size optimizer. Floating-point operations may be zero in a fixed-point implementation; the operation budget is not a demonstrated cycle bound. Require each slice <50 µs on BBE32/R52, and report transport, matching, leave-one-sensor-out track production and calibration-observation latency separately.

### Patentability Hook & Internet Prior-Art Check

Candidate claim: (A) independently timestamped moving-target reference tracks; (B) yaw-free \(rw-d^Tv\) residuals; (C) determinant-conditioned joint time-offset/Doppler-bias elimination; (D) subsequent circular yaw estimation and acceleration/correlation-aware refusal. Commercial objective: avoid allowing a timing or Doppler bias to masquerade as boresight drift during fast dynamic alignment.

- [Kim et al., “EKF-Based Radar-Inertial Odometry with Online Temporal Calibration,” RA-L 2025 / arXiv:2502.00661](https://arxiv.org/html/2502.00661v2), **full method inspected**: §IV-D Eq. 12 estimates ego-velocity from Doppler observations; Eqs. 13–16 place time offset in a radar/IMU velocity residual and its Jacobian. §IV-E explicitly discusses weak temporal observability when ego-velocity changes little. N27 adds independently known relative target position and multiplies range by Doppler; it is not a claim to invent online temporal calibration.
- [Štironja et al., “Radar-Inertial Odometry with Online Spatio-Temporal Calibration via Continuous-Time IMU Modeling,” arXiv:2603.19958](https://arxiv.org/html/2603.19958v1), **full method inspected**: §§III-C–F use cubic B-splines, compensated ego-velocity factors, a factor graph and one-second optimization window. The paper already combines spatial and temporal calibration. Its §IV-D acknowledges excitation-dependent extrinsic behavior. Compare N27 against this capability with matched information inputs; an eight-target independent tracker is not free.
- [Wise et al., “A Continuous-Time Approach for 3D Radar-to-Camera Extrinsic Calibration,” ICRA 2021 / arXiv:2103.07505](https://arxiv.org/abs/2103.07505), abstract inspected: radar-velocity-based calibration without specialized retroreflectors and explicit observability analysis. Full derivation remains a claim-search obligation.
- [US20160025844A1, Honeywell, “Frequency-modulated-continuous-wave (FMCW) radar with timing synchronization”](https://patents.google.com/patent/US20160025844A1/en), independent claim 1 examined by MiMo: obtains timing differences from FMCW signals, receives a second radar's timing offset, and synchronizes clocks using their difference. N27 consumes monostatic target range/rate and independent tracks, not an exchanged pair of radar timing offsets. This patent is relevant prior art, but the inspected claim does not disclose N27.2's two-target elimination. Related families/continuations remain unchecked.
- Google Scholar query `"radar" "time offset" "Doppler"` returned both calibration and RF synchronization work. Google Patents returned thousands of broad matches, including the Honeywell document above. **The searched methods establish adjacent prior art; they do not establish that N27 is patentable or absent from unpublished applications.** Exact range–range-rate invariants, bias-separated sensor registration and multistatic registration textbooks need further searching.

### Lower-Tier Model Verification Recipe & Research Decisions

1. **Exact clock fixture:** \(d=(10,4)\) m, \(v=(5,-2)\) m/s, \(\tau=0.02\) s. Then \(x=(10.1,3.96)\), \(rw=x^Tv=42.58\), \(d^Tv=42\), \(\|v\|^2=29\). Assert recovered time 0.02 s to \(10^{-12}\). Set yaw to −170/−5/0/5/170° and synthesize only the measured angle from it; timing must be unchanged, yaw recovered modulo \(2\pi\).
2. **Clock-plus-bias fixture:** actual-time positions \(x_1=(10,0),x_2=(0,20)\); velocities \(v_1=(5,0),v_2=(0,10)\); \(\tau=0.02\), \(b=0.1\) m/s. Thus \(d_1=(9.9,0),d_2=(0,19.8)\), \((r_1,r_2)=(10,20)\), \((w_1^{obs},w_2^{obs})=(5.1,10.1)\), \((y_1,y_2)=(1.5,4)\), \(D=-500\). Expect exact recovery \((0.02,0.1)\). Add a third target to test held-out residual consistency rather than fitting two equations perfectly and claiming validation.
3. **Counterexamples:** \(v=0\); equal \(\|v_k\|^2/r_k\); tangential nonzero velocity (must still work); opposite Doppler sign convention; aliased Doppler; per-target measurement epochs; changing shared bias; 0.01/0.1/0.5 m/s reference-velocity bias; 0.01/0.1 m range bias. Assert invalid on rank failure, report uncertainty growth near failure. Sweep acceleration 0/3/6/12 m/s² and \(|\tau|=5/20/45/100\) ms; assert N27.5 and its remainder bound before using the cheap approximation.
4. **Feasibility ladder:** 10,000 trials, ranges 5–30 m, relative speeds 5–20 m/s, independent position noise 0.01 m, velocity noise 0.01 m/s, range noise 0.01 m, Doppler noise 0.005 m/s, eight diverse tracks. Provisional goal: accepted bundles have timing RMSE ≤5 ms, bias RMSE ≤0.03 m/s; report acceptance fraction and uncertainty coverage, then repeat with tenfold worse track noise. These are test targets, not promised results. The one-target fixture alone has a nontrivial velocity-noise timing floor, which the model must calculate rather than erase by averaging correlated tracks.
5. **Promotion/kill:** prove a bias/variance or bounded-error result for noisy N27.2, including its random denominator and shared track errors; establish an information-matched benefit over joint EKF/FGO. Kill the claimed advantage if an oracle reference is necessary, circular fusion gives overconfidence, existing bias-registration art discloses A–D, or timing is less reliable than the current timestamp path after realistic velocity bias. Touchpoints: async motion compensation, fast DA, DC SiL timestamp adapters; do not modify production firmware before these questions close.

## N28 — Guarded Error-Transfer Algebra for Certified Radar Replay Boundaries

### Algorithmic Mechanics & Exact Closed-Form Equations

**Analytical handoff; EXECUTED 2026-09-30 (`guarded_replay.py` PASS, storage Run 25, equations E21).** Target the unproved step behind “46 frames warm-up is enough”: a small continuous initialization error may still switch an association, birth/death decision or calibration mode. Certify the discrete decisions and exported KPI tolerance before accepting a reused shard boundary. This is a branch-equivalence certificate, not a new Kalman scan operator.

Let a recorded reference trace use inputs \(z_k\), full state \(x_k^*\) and winner \(j_*\). Over a specified norm ball, assume verified uniform bounds
\[
\|F_k(x,z_k)-F_k(x_k^*,z_k)\|\le L_k\|x-x_k^*\|+\eta_k,
\quad |c_j(x)-c_j(x_k^*)|\le C_{kj}\|x-x_k^*\|.
\]
For every alternative \(j\), let \(m_{kj}=c_j(x_k^*)-c_{j_*}(x_k^*)>0\). A sufficient decision radius is
\[
h_k=\min_{j\ne j_*}\frac{m_{kj}}{C_{kj}+C_{kj_*}},\qquad
e_{k+1}\le L_ke_k+\eta_k,\qquad 0\le e_k<h_k.
\tag{N28.1}
\]
Include every discrete predicate, gate threshold, timeout and supported birth/death branch in the minimum, not merely the nearest two costs. Also intersect \(h_k\) with the radius of the neighborhood on which all stated bounds were proved. If a denominator is zero and its positive margin is truly constant, that constraint is \(+\infty\). Ties are uncertified unless exact discrete-state equality provides a separate proof. A point Jacobian is not a Lipschitz bound over a finite neighborhood. For global assignment, \(c_j\) must represent competing complete assignments or an independently sound assignment-stability certificate; per-detection winners do not prove Hungarian/global assignment invariance.

**Own compression direction:** summarize a segment by a guarded scalar affine upper map \(S=(a,b,h)\), with semantics \(e\mapsto ae+b\), valid on \([0,h)\), \(a,b\ge0\). Later segment \(S_2\) after \(S_1\) has
\[
\boxed{a_{21}=a_2a_1,\qquad b_{21}=a_2b_1+b_2},
\]
\[
\boxed{h_{21}=\max\{0,\min[h_1,(h_2-b_1)/a_1]\}}\quad(a_1>0).
\tag{N28.2}
\]
When \(a_1=0\), the second guard receives constant radius \(b_1\): retain \(h_1\) if \(b_1<h_2\), otherwise return an explicit empty certificate. Empty certificates absorb composition. The identity is \((1,0,+\infty)\).

**Proof obligation mostly closed analytically:** the composed domain is precisely \(e<h_1\land a_1e+b_1<h_2\). Monotonicity for \(a_1\ge0\) makes this a single anchored interval; function composition plus domain preimage is associative over exact reals. Therefore a constant-size certificate closes over arbitrarily many scalar steps. Its domain is exact for the chosen upper-map/guard abstraction, generally conservative for the true radar dynamics. Arbitrary vector polyhedral certificates need not admit this three-number compression; no universal impossibility claim about all vector abstractions is needed.

To incorporate a continuous exported quantity \(K\) with Lipschitz constant \(L_K\) and tolerance \(\epsilon_K\), append the guard \(e<\epsilon_K/L_K\). UDP bin indices, CAN track IDs and thresholded KPI percentages instead require their own discrete stability margins. Same association sequence does not imply bit-identical state, and a 1 cm continuous error bound does not guarantee identical quantized bins at a boundary.

**Critical fixed-point correction:** rounded composition is generally **not associative**. Round \(a,b\) upward and guard radii downward; use a canonical merge tree and report extra conservatism. Only an exact bounded arithmetic representation with a proven overflow bound can support an arbitrary-tree bit-identity claim. Do not inherit N25's additive-sum bit-identity statement for products/divisions here. Two independently quantized paths can differ by up to a full quantization step from rounding alone, not necessarily half a step.

**Replay contract:** bind the certificate to input sequence, build, parameter version, scaling, complete branch-relevant state, and the proven neighborhoods. Reuse is meaningful only when the incoming error \(e_0\) is independently bounded. A changed build requires a verified cross-build map/decision perturbation bound in \(\eta\) and the margins; a matching filename or copied warm-up counter is insufficient. If a segment fails, replay from a valid upstream checkpoint, then propagate/recompute downstream bounds. Do not claim isolated failure leaves all downstream states valid. A certificate constructed from a full reference pass has a construction cost and only helps repeated/perturbed replay when that cost is amortized.

### Hardware Feasibility Audit

Learned parameters **0**, weights **0 KB**. A triple stored as three 64-bit fixed-point quantities is 24 bytes; flags/version references need additional space. A fixed 32-entry merge workspace plus 1 KB scratch/metadata is **<2 KB**, excluding the stored reference trace/checkpoints. One nondegenerate merge needs two scalar multiplies, one addition, one subtraction, one division and a few comparisons; bounded integer division and overflow checking are required. Allocate at most 32 merges per slice and demand <30,000 cycles at 600 MHz / <50 µs on target hardware. Floating-point operations can be zero. The verifier is TinyML-sized; **generating uniform bounds, global-assignment margins and reference snapshots is not included in this tiny online cost**. Fixed arrays, no recursion, bounded tree traversal and caller-owned checkpoints give zero heap in the verifier loop.

### Patentability Hook & Internet Prior-Art Check

Candidate claim: reference radar association/decision margins produce a guarded boundary-error transfer summary; summaries compose into a certified reuse radius tied to build/input provenance; a scheduler accepts approximate initialization only when both branch identity and explicit output tolerances are certified, otherwise recomputes from a valid checkpoint. Commercial value: avoid blindly discarding fixed warm-up prefixes across thousands of SiL shards while preserving traceable regression tolerances.

- [US8079022B2, “Simulation of software,” Carbon/ARM, priority 2007-06-04](https://patents.google.com/patent/US8079022B2/en), text inspected: saves state and I/O, replays until divergence is detected, then restarts before divergence. This already covers broad replay acceleration/checkpoint reuse. The proposed distinction is a prospective quantitative initialization-radius/decision certificate, not saving or replaying state.
- [US11719749B1, “Method and system for saving and restoring of initialization actions on DUT and corresponding test environment,” Cadence, priority 2020-10-22](https://patents.google.com/patent/US11719749B1/en), text inspected: restores initialized DUT/testbench state and loads new test code. “Skip initialization using snapshots” is established prior art. N28 requires a quantified perturbation contract, but inspection of one patent does not clear broader verification/replay claims.
- MiMo identified the adjacent theory: [Blelloch, “Prefix Sums and Their Applications”](https://www.cs.cmu.edu/~blelloch/papers/Ble93.pdf), associative scans; [Goubault–Putot, CAV 2009 affine-arithmetic work](https://www.lix.polytechnique.fr/~putot/Publications/cav09_taylor.pdf), numerical abstractions; [validated containment/shadowing](https://www.cs.toronto.edu/~wayne/research/papers/containment.pdf). These are **prior-art leads requiring equation/claim comparison**, not evidence of N28 novelty. Affine composition, weakest preconditions and norm error tubes are all established concepts.
- Closest unresolved question: does existing incremental verification or speculative hybrid-system simulation already combine decision margins, monotone interval preimages and trace reuse in this form? Search `simulation replay robustness radius branch stability`, `guarded affine transfer weakest precondition`, `incremental verification checkpoint Lipschitz`, and `radar association sensitivity certified warm start`. Discard a broad monoid claim; the possible claim scope is the radar-specific certificate/scheduler contract and a demonstrated cost advantage.

### Lower-Tier Model Verification Recipe & Research Decisions

1. **Exact composition fixture:** \(S_1=(0.5,0.01,0.2)\), \(S_2=(2,0.05,0.1)\). Expect \(S_2\circ S_1=(1,0.07,0.18)\), valid only for \(e<0.18\). Reversing order gives \((1,0.035,0.075)\). At \(e=0.18\), the first output is 0.1 and the strict second guard fails. With \(a_1=0,b_1=h_2\), require empty; equality is not acceptance.
2. **Margin fixture:** scalar state \(x\), costs \(c_1=(x-1)^2,c_2=(x+1)^2\), reference \(x^*=0.5\). Gap is 2. On \(|x-x^*|\le0.5\), valid uniform constants \(C_1=2,C_2=4\) certify \(e<1/3\); the exact winner changes at \(x=0\), so the certificate is sound but conservative. A tighter direct Lipschitz bound on \(c_2-c_1=4x\) certifies \(e<0.5\). Add a third candidate with a larger nominal gap but much larger slope to prove runner-up-only testing unsound.
3. **Associativity test:** exact rational reference over \(a\in\{0,1/2,1,2\}\), nonnegative rational \(b,h\); assert identical domains and affine maps under all parenthesizations. For fixed-point arithmetic assert **sound enclosure**, not arbitrary-tree bit equality. Include overflow, underflow, zero gain, infinite guard, empty domain and one-ULP boundary cases. Downward-rounded \(h\) may reject valid points; it must never accept an invalid one.
4. **Hybrid replay fixture:** CV tracking plus nearest-neighbor association, a birth counter, deletion timeout, adaptive gate state, DA lock state and quantized exports. Compare a sequential candidate reference with initialized shards perturbed inside/outside their certified radii. Require zero certified branch mismatches and continuous output discrepancies within bounds; explicitly include ID counters and all history buffers. Start with fixed input detections, then separately account for signal-processing changes that alter them.
5. **KPI target:** on 300 logs ×20 shards, report certificate construction time, reference storage, replayed frames, admitted radius distribution, refusal causes, mismatches and end-to-end wall time. Goal for promotion: ≥50% fewer warm-up frames than the matched 46-frame schedule **after construction cost is amortized**, with zero observed certificate violations. This is a test criterion, not a prediction; one valid counterexample kills soundness. If most radii collapse to zero or exact checkpoints already solve the workload more cheaply, retain the negative result and discard the optimization claim.
6. **Next-model theorem tasks:** derive uniform bounds for the actual state/covariance/DA update, prove all relevant decisions are covered, and quantify precision loss under canonical fixed-point merging. Investigate whether a useful cross-build certificate can be generated without running the full candidate sequentially. Integration targets: `hpcc_local_orchestrator.py`, `fast_da.py`, DC SiL wrapper boundaries and UDP/CAN KPI contracts. This unresolved construction cost is the central research problem, not an implementation footnote.

### N26–N28 — Next-Iteration Handoff Index and Evidence Status

**Model provenance:** MiMo v2.6 Flash Free performed geometry/replay research and claim checks; one geometry call failed and was retried, one clock call returned no content. Inkling Free was used for the failed clock task and mathematical critique. Muse Spark 1.3 was not available as an exposed configured route; it was not used or impersonated. No other subagent model was dispatched for this extension. No code or simulation was generated/executed; no proposals were recorded as empirical keeps or given fabricated novelty scores.

**Cross-index audit limitations:** arXiv search and relevant calibration full texts were readable; EPO and Google Patents supplied patent text. Google Scholar returned relevant results despite a page warning. Semantic Scholar's HTML gave no usable content and the citation-count API returned HTTP 429, so citation thresholds remain unverified. OpenReview search returned adjacent radar-fusion papers/reviews, not a complete mechanism search: [EchoFusion](https://openreview.net/forum?id=LZzsn51DPr) reviewers questioned temporal alignment, corroborating the problem but not N27 novelty. Papers with Code redirected to an unrelated Hugging Face trending page; that search is **not completed**. Unpublished patent applications, non-English filings, continuation claims and proprietary implementations are unobservable here. “Nobody has done it” is not established.

| Start here | Central result to prove or refute | First non-oracle experiment | Minimum handoff artifact |
|---|---|---|---|
| N26 | Global uncertainty enclosure plus N26.3 under justified surface regularity | Independent parent + physical curved specular paths, including glint mismatch | Derivation, source claim chart, contact/velocity/curvature error and refusal table |
| N27 | Stable clock/bias identification with noisy correlated tracks and acceleration | Leave-one-sensor-out reference, independent epochs and Doppler-bias injection | Identifiability proof, full covariance/interval treatment, timing-vs-bias-vs-yaw ablation |
| N28 | Sound complete branch certificate with affordable construction | Full branch-state replay, perturbed shard initializations and quantized exports | Exact-arithmetic proof, fixed-point counterexamples/enclosure proof, amortized-cost table |

**Instruction to the next model:** Read the relevant node and cited source equations/claims before implementation. Start by trying to invalidate its assumptions and numerical examples. Preserve N23–N25 and the historical ledgers. Separate algebraic identities, statistical approximations, conjectures, acceptance targets and measured results. For every claimed novelty element, record the nearest paper/patent passage and why that passage does or does not disclose it. Do not convert “not found in this search” into “patentable.” If implementation is subsequently authorized, use a bounded prototype plus one meaningful regression check per mechanism; first prove timing/memory on the actual target before adding a learned component. Return a decision—advance, narrow or discard—with evidence and the next unresolved question.

---

# N20–N22 — State-Space / Geometric Frontier Derivations (2026-09-30; EXECUTED Runs 22–25)

Status 2026-09-30: all three implemented + verified (`hermite_clothoid.py`, `occupancy_blue.py`, `gram_lift.py` PASS; storage.md Runs 22–25, equations.md E17–E22, research.md §6 + IDF-5/6/8). No novelty score assigned; prior-art status per node. Node IDs follow the user request (N20–N22); distinct from executed strategy-graph nodes with the same numbers in older logs — prefix as **N20g, N21g, N22g** in the JSONL graph.

Convention: physical reciprocal double-bounce path \(s\to q\to p\to q\to s\); raw apparent ghost \(g=s+\ell u\) (one-way equivalent length \(\ell\), receive unit ray \(u\)), as in N25/N26. Run 3's E2 invariant used an unfolded sensor-image convention; reconcile before comparing numbers.

## N20g — Tangent-Mirror Hermite Theorem: Exact Linear Recovery of Cubic/Clothoid Guardrails from Direct–Ghost Pairs

### Core Theorem

**Lemma 1 (local rank-1 invariant on any smooth curve).** Let the rail be a \(C^1\) planar curve, \(q\) a specular contact for \((s,p)\), \(n\) the unit normal at \(q\) oriented toward the sensor side, \(\rho=\|p-q\|\). Then
\[
\boxed{p-g=2\,\delta\,n,\qquad \delta=(p-q)^Tn>0},
\]
i.e. \(g\) is the mirror image of \(p\) in the **tangent line** at \(q\).

*Proof.* Stationarity of \(L(q)=\|q-s\|+\|q-p\|\) along the curve gives \(T^T(u_{in}+a')=0\) with \(u_{in}=(q-s)/\|q-s\|=u\), \(a'=(q-p)/\rho\); hence the outgoing direction \(a=(p-q)/\rho\) is the reflection \(a=u-2(u^Tn)n\) (law of reflection w.r.t. the tangent line). Continuing the receive ray beyond \(q\) by \(\rho\): \(g=s+(\|q-s\|+\rho)u=q+\rho u\). Reflection \(M=I-2nn^T\) is an involution fixing the tangent line through \(q\): \(q+M(p-q)=q+\rho Ma=q+\rho u=g\). Therefore \(p-g=(I-M)(p-q)=2nn^T(p-q)=2\delta n\). \(\square\)

The planar rank-1 law is the special case \(n,\ \delta=d-n^Tp\) constant. On curves the direction and magnitude vary **with the contact**, which is exactly why the global \(2d\) histogram smeared over 132 m (E2e): the invariant never broke, it became local.

**Corollary 1 (bisector–ray contact, closed form).** With \(m=\tfrac12(p+g)\), \(\hat n=(p-g)/\|p-g\|\):
\[
\boxed{t=\frac{\hat n^T(m-s)}{\hat n^Tu},\qquad q=s+tu,\qquad \text{slope } \sigma\equiv f'(x_q)=-\frac{\hat n_x}{\hat n_y}}
\]
(tangent line = perpendicular bisector of \(p,g\); \(q\) = its intersection with the receive ray). Requires \(\hat n^Tu\neq0\) (non-grazing) and \(\hat n_y\neq0\) (rail not parallel to the \(y\)-axis of the chosen frame; otherwise rotate frame). Equivalent to N26.1 but needs no \(\ell\) beyond \(g\).

**Theorem 1 (Hermite linearity for polynomial rails).** Let the rail be \(y=f(x)=c_0+c_1x+c_2x^2+c_3x^3\) in a world-fixed frame (ego-motion compensated). Each valid pair \(i\) yields Hermite data \((x_i,y_i,\sigma_i)\) from Cor. 1, and
\[
\underbrace{\begin{bmatrix}1&x_i&x_i^2&x_i^3\\0&1&2x_i&3x_i^2\end{bmatrix}}_{V_i}c=\begin{bmatrix}y_i\\\sigma_i\end{bmatrix}.
\]
The coefficients enter **linearly**; no Fermat root solve, no \((R_c,y_g)\) grid. For two contacts \(x_1\neq x_2\), the confluent Vandermonde determinant is \(\det\begin{bmatrix}V_1\\V_2\end{bmatrix}=(x_2-x_1)^4\neq0\), so the cubic is unique. With \(h=x_2-x_1\), \(\bar\delta=(y_2-y_1)/h\):
\[
\boxed{f(x)=y_1+\sigma_1(x-x_1)+\frac{\bar\delta-\sigma_1}{h}(x-x_1)^2+\frac{\sigma_1+\sigma_2-2\bar\delta}{h^2}(x-x_1)^2(x-x_2)}
\]
(Newton form with repeated nodes: \(f[x_1,x_1]=\sigma_1\), \(f[x_1,x_1,x_2]=(\bar\delta-\sigma_1)/h\), \(f[x_1,x_1,x_2,x_2]=(\sigma_1+\sigma_2-2\bar\delta)/h^2\)).

*Proof.* Hermite interpolation with two double nodes has a unique degree-≤3 solution iff nodes are distinct; the determinant formula is the confluent Vandermonde \(\prod_{i<j}(x_j-x_i)^{m_im_j}\) with \(m_1=m_2=2\). \(\square\)

**Clothoid link.** A clothoid \(\kappa(s)=\kappa_0+\kappa_1 s\) with small heading gives \(y\approx y_0+\psi_0x+\tfrac{\kappa_0}{2}x^2+\tfrac{\kappa_1}{6}x^3\); exact curvature of the cubic is \(\kappa(x)=f''/(1+f'^2)^{3/2}\), \(f''=2c_2+6c_3x\). The cubic is the standard road-geometry model; Theorem 1 is exact for the cubic, approximate for the clothoid with error \(O(\psi^2\kappa x^2)\).

**Theorem 2 (exact Snell manifold / forward model for gating).** For fixed \(s\) and rail \(c\), the set of all consistent (parent, ghost) pairs is the 2-parameter manifold
\[
\mathcal M_c=\{(p,g):\ p=q(x)+\rho\,a(x),\ g=q(x)+\rho\,u(x),\ \rho>0\},
\]
\(q(x)=(x,f(x))\), \(u=(q-s)/\|q-s\|\), \(n=(-f',1)/\sqrt{1+f'^2}\) (oriented toward \(s\)), \(a=u-2(u^Tn)n\). For a **given** parent \(p\), the admissible contacts are the roots of the scalar Snell function
\[
\phi(x)=\begin{bmatrix}1\\f'(x)\end{bmatrix}^T\!\left(\frac{q-s}{\|q-s\|}+\frac{q-p}{\|q-p\|}\right)=0,
\]
a 1-DOF manifold (the requested "reflection manifold"); ghosts are \(g=q+\|q-p\|u\) at each root. Multiple roots (convex/concave inflection with \(c_3\neq0\)) are physical multi-ghosts, not ambiguities to be resolved by RANSAC.

**Consensus without RANSAC ambiguity.** Weighted LS over \(K\) pairs: \(\hat c=(\sum V_i^TW_iV_i)^{-1}\sum V_i^TW_i[y_i,\sigma_i]^T\); per-pair residual \(r_i=[y_i,\sigma_i]^T-V_i\hat c\), test \(r_i^T(S_i)^{-1}r_i\le\chi^2_{2,0.99}\). Minimal hypotheses: all \(\binom K2\) pair-of-pairs Hermite solves (\(K\le16\Rightarrow120\), deterministic, exhaustive). Accumulating contacts over time in the world frame supplies \(x_1\ne x_2\) even with one target. Slope error: with \(\Delta=p-g\), \(\partial\sigma/\partial\Delta=[-1/\Delta_y,\ \Delta_x/\Delta_y^2]\); \(\mathrm{var}(\sigma)\approx\|\partial\sigma/\partial\Delta\|^2\,\mathrm{tr}(\Sigma_p+\Sigma_g)/2\) for isotropic errors. Conditioning of \(c\): \(\kappa(V)\) grows as \(h^{-3}\) for close contacts—report it, do not hide it.

### Patent Claim Hook

A method in which, for a moving target observed both directly and via a curved roadside reflector, the perpendicular bisector of the parent/ghost positions is intersected with the ghost receive ray to obtain a specular contact point **and** tangent slope, and a polynomial/clothoid barrier model is solved as a **linear Hermite system** from such point–slope pairs, then used to predict multi-root ghosts for gating. Distinct from map-based NLOS detection (EP4177638A1, map required in claim 1), stationary-detection guardrail curve fitting (fits points only, no slope from moving-target multipath), and cylinder/Fermat RANSAC (Run 12 lineage). Prior-art search still required: "guardrail estimation multipath moving target tangent", road-boundary clothoid from radar ghosts.

### Numerical Verification Recipe

1. **Planar fixture (exact):** \(s=(0,0)\), rail \(c=(-4,0,0,0)\), \(p=(20,-1)\), \(g=(20,-7)\). Expect \(\hat n=(0,1)\), \(\sigma=0\), \(t=\frac{-4}{-7/\sqrt{449}}=4\sqrt{449}/7\approx12.109\), \(q=(80/7,-4)\approx(11.4286,-4)\). Assert \(|q_y+4|<10^{-12}\), \(|q_x-80/7|<10^{-12}\).
2. **Cubic generation (no root solving, avoids circularity):** \(c=(-4,\,0.02,\,1.5\times10^{-3},\,-8\times10^{-6})\), \(s=(0,0)\). For \(x\in\{8,15,25,40,60\}\), \(\rho\in\{3,6\}\): compute \(q,n,u,a\) from Theorem 2, set \(p=q+\rho a\), \(g=q+\rho u\); keep only \(\delta=(p-q)^Tn>0\). Invert using Cor. 1 from \((s,p,g)\) only. Assert contact error \(<10^{-9}\) m and slope error \(<10^{-12}\); Hermite solve from pairs \(x=8,40\) recovers \(c\) with relative error \(<10^{-8}\); every other pair residual \(<10^{-9}\).
3. **Snell forward check:** for each generated \(p\), bracket-solve \(\phi(x)=0\) on \([0,120]\) (bisection, 60 iters) and assert the generating \(x\) is among the roots; count roots and report multi-ghost cases.
4. **Noise ladder:** Gaussian noise on \(p,g\): \(0.02/0.05/0.10\) m; 2000 trials of 8 pairs + 8 decoys (random points at matching ranges). Report contact RMSE, \(\hat c\) RMSE, curvature error at \(x=30\), pair precision/recall under the \(\chi^2\) test. Comparison baseline: `bend_conditioned.py` (Run 12: rec 0.753, prec 0.834) on the same scenes. Provisional pass target (not a result): recall \(\ge0.90\), precision \(\ge0.90\) at 0.05 m, and \(\hat c\) unique (no multi-mode fit as in Run 12's (45,3)/(30,3) ambiguity).
5. **Kill:** if slope noise at realistic AoA (0.35°) makes \(\kappa(V)\) unusable for \(h<30\) m, or if bisector–ray contacts fail on real ghost data (mixed paths, glint), report and narrow to point-only fitting.

## N21g — Gram-Invariant Lift: Linearization-Free, Exactly Associative Parallel Filtering for Polar Range/Doppler

### Core Theorem

State \(x=[p;v]\in\mathbb R^4\) (relative position/velocity, 2-D), CV model \(x'=Fx+w\), \(F=\begin{bmatrix}I&\Delta I\\0&I\end{bmatrix}\), \(w\sim(0,Q)\) Gaussian, \(Q=q\begin{bmatrix}\tfrac{\Delta^3}{3}I&\tfrac{\Delta^2}{2}I\\\tfrac{\Delta^2}{2}I&\Delta I\end{bmatrix}\). Radar measures \(\tilde r=r+\epsilon_r\), \(\tilde\theta=\theta+\epsilon_\theta\), \(\tilde v_r=v_r+\epsilon_v\), independent zero-mean Gaussian, \(v_r=p^Tv/r\).

**Rotation-invariant Gram coordinates.** \(\zeta=[a,b,c]^T=[\,p^Tp,\ p^Tv,\ v^Tv\,]^T\). Note \(r^2=a\) and \(r v_r=b\): range and range-rate are **linear** in \(\zeta\), and \(\zeta\) is invariant to sensor yaw (connects to N27).

**Lemma 1 (exact linear Gram dynamics).** With \(w=[w_p;w_v]\), \(\tilde y=p+\Delta v\):
\[
\zeta'=\Phi\zeta+\gamma+\eta,\qquad
\Phi=\begin{bmatrix}1&2\Delta&\Delta^2\\0&1&\Delta\\0&0&1\end{bmatrix}=e^{\Delta N},\ N=\begin{bmatrix}0&2&0\\0&0&1\\0&0&0\end{bmatrix},
\]
\[
\gamma=\big[\mathrm{tr}Q_{pp},\ \mathrm{tr}Q_{pv},\ \mathrm{tr}Q_{vv}\big]^T=q\big[\tfrac{2\Delta^3}{3},\ \Delta^2,\ 2\Delta\big]^T,
\]
\[
\eta=\begin{bmatrix}2\tilde y^Tw_p+\|w_p\|^2-\gamma_1\\ \tilde y^Tw_v+v^Tw_p+w_p^Tw_v-\gamma_2\\ 2v^Tw_v+\|w_v\|^2-\gamma_3\end{bmatrix},\qquad \mathbb E[\eta\mid\mathcal F_k]=0.
\]
*Proof.* Expand \(\|p+\Delta v+w_p\|^2\), \((p+\Delta v+w_p)^T(v+w_v)\), \(\|v+w_v\|^2\); collect terms; zero-mean because \(\mathbb E w=0\), \(\mathbb E[w_i w_j^T]=Q_{ij}\). \(\Phi\) is the order-2 Pascal (binomial) matrix: \(\Phi(\Delta_1)\Phi(\Delta_2)=\Phi(\Delta_1+\Delta_2)\). \(\square\)

**Lifted linear system.** \(\xi=[x;\zeta]\in\mathbb R^7\): \(\xi'=\mathcal A\xi+[0;\gamma]+[w;\eta]\), \(\mathcal A=\mathrm{blkdiag}(F,\Phi)\). Measurements, all with **conditional mean linear in \(\xi\)**:
\[
y=\begin{bmatrix}\lambda^{-1}\tilde r\cos\tilde\theta\\\lambda^{-1}\tilde r\sin\tilde\theta\\\tilde r^2-\sigma_r^2\\\tilde r\,\tilde v_r\end{bmatrix}=\underbrace{\begin{bmatrix}I_2&0&0\\0&0&e_1^T\\0&0&e_2^T\end{bmatrix}}_{\mathcal H}\xi+e,\qquad \lambda=e^{-\sigma_\theta^2/2},
\]
since \(\mathbb E[\cos(\theta+\epsilon_\theta)]=\lambda\cos\theta\) (unbiased converted measurement), \(\mathbb E[\tilde r^2]=r^2+\sigma_r^2\), \(\mathbb E[\tilde r\tilde v_r]=rv_r=b\).

**Lemma 2 (second-order statistics are prior-moment computable).** Let \(M_k=\mathbb E[x_kx_k^T]\), \(\mu_k=\mathbb E x_k\); these obey the deterministic affine recursions \(\mu'=F\mu\), \(M'=FMF^T+Q\). By Isserlis (\(\mathbb E[(w^TAw-\mathrm{tr}AQ)(w^TBw-\mathrm{tr}BQ)]=2\mathrm{tr}(AQBQ)\), odd moments vanish):
\[
\mathrm{Cov}(\eta)=\mathbb E[L(x)QL(x)^T]+\Big[2\,\mathrm{tr}(A_iQA_jQ)\Big]_{ij},\qquad \mathrm{Cov}(w,\eta)=Q\,\mathbb E[L(x)]^T,
\]
where \(\eta=L(x)w+[w^TA_iw-\mathrm{tr}A_iQ]_i\), \(L(x)=\begin{bmatrix}2\tilde y^T&0\\v^T&\tilde y^T\\0&2v^T\end{bmatrix}\), \(A_1=\begin{bmatrix}I&0\\0&0\end{bmatrix}\), \(A_2=\tfrac12\begin{bmatrix}0&I\\I&0\end{bmatrix}\), \(A_3=\begin{bmatrix}0&0\\0&I\end{bmatrix}\); \(\mathbb E[L QL^T]\) is linear in \(M_k\) (entries \(\mathrm{tr}(Q_{\cdot\cdot}\,\cdot\,M_k)\)) and \(\mathbb E L\) linear in \(\mu_k\). Measurement noise: exact
\[
\mathrm{Var}(e_3)=4\sigma_r^2\,\mathbb E a+2\sigma_r^4,\quad \mathrm{Cov}(e_3,e_4)=2\sigma_r^2\,\mathbb E b,\quad
\mathrm{Var}(e_4)=\sigma_v^2\mathbb Ea+\sigma_r^2\mathbb E[v_r^2]+\sigma_r^2\sigma_v^2\le\sigma_v^2\mathbb Ea+\sigma_r^2\mathbb Ec+\sigma_r^2\sigma_v^2,
\]
\(\mathbb Ea=\mathrm{tr}M_{pp}\), \(\mathbb Eb=\mathrm{tr}M_{pv}\), \(\mathbb Ec=\mathrm{tr}M_{vv}\). Converted-Cartesian covariance \(e_{1:2}\): use the standard unbiased-conversion formula evaluated in expectation; it contains \(\cos2\theta\) terms not polynomial in \(x\) → use the conservative bound \(\mathbb E[\cdot]\preceq(\lambda^{-2}(\mathbb Ea+\sigma_r^2)-\mathbb Ea\,\lambda^{2}\cdot0)\,I\) or a prior-Gaussian quadrature (declared approximation).

**Theorem (LMMSE exactness + exact associativity, no iteration).** If the noise second-order statistics of Lemma 2 are used exactly, the Kalman filter/RTS smoother on the lifted system returns the **linear minimum-MSE estimator of \(\xi_k\) given \(y_{1:N}\)** (among estimators affine in the stacked \(y\)), and its Särkkä–García-Fernández associative elements (Lemma 7, with offset \([0;\gamma]\) inside \(b_k\)) compose **exactly** under the associative operator (Lemma 8)—for any tree order.

*Proof.* (i) The Kalman recursion is the LMMSE (BLUE) estimator for any linear system whose process/measurement noises are zero-mean, mutually white and uncorrelated with the initial state, with known (possibly time-varying, cross-correlated at equal time) covariances—Gaussianity is not required (Anderson & Moore, *Optimal Filtering*, Ch. 5). (ii) \(\eta_k,e_k\) are martingale differences w.r.t. the natural filtration (Lemma 1 and conditional unbiasedness of \(y\)), hence uncorrelated with all past states and noises; the equal-time correlation \(\mathrm{Cov}(w_k,\eta_k)\) is included in the stacked process covariance. (iii) Unconditional covariances depend only on \((\mu_k,M_k)\), which are deterministic and computable by an affine prefix scan (\(\mathrm{vec}M'=(F\otimes F)\mathrm{vec}M+\mathrm{vec}Q\)), so all elements are **data-independent** and can be built in parallel. (iv) Associativity of the operator holds for any linear-Gaussian-form elements (Särkkä & García-Fernández 2021, Lemma 8); no linearization point enters, so no outer Gauss–Newton loop is needed. \(\square\)

**Corollary (yaw-free speed observability).** The \(\zeta\)-subsystem with \(H_\zeta=[e_1,e_2]^T\) has observability matrix rows \((1,0,0),(0,1,0),(1,2\Delta,\Delta^2)\): rank 3, so \(\|v\|^2\) is linearly observable from range and range·range-rate alone in two scans, with no angle.

**Cost/benefit versus IEKS scan.** Parallel IEKS/SLR smoothers (Yaghoobi, Corenflos, Hassan, Särkkä, ICASSP 2021) already make nonlinear smoothing associative **per iteration**; they need \(L\) sequential iterations (span \(O(L\log N)\)). N21g needs 2 scans (moments + filter) and no iteration, at the price of: (a) relaxing the constraint \(a=\|p\|^2\) (information loss; optional post-hoc projection), (b) LMMSE rather than posterior mode. Novelty risk: Kronecker/polynomial lifted Kalman filters exist (Carravetta, Germani, Raimondi, polynomial filtering, 1996–2005). The claimed new elements are the **rotation-invariant 3-dim Gram lift with range² and range·Doppler as exact linear measurements**, the prior-moment covariance closure, and its exactly associative parallel implementation.

### Patent Claim Hook

A radar tracker/replay engine that augments the Cartesian state with the rotation-invariant quadratic invariants \((\|p\|^2,p^Tv,\|v\|^2)\), propagates them with the Pascal transition \(\Phi\) and additive offset \(\gamma\), ingests \(\tilde r^2-\sigma_r^2\) and \(\tilde r\tilde v_r\) as linear observations with noise covariances computed from a separately prefix-scanned prior second-moment trajectory, and executes filtering/smoothing by a single associative parallel scan without iterative relinearization.

### Numerical Verification Recipe

1. **Lemma 1 exactness:** random \(x\), \(w\) (1e4 samples); assert \(\zeta'-\Phi\zeta-\gamma-\eta=0\) to \(10^{-12}\) (float64) and sample mean of \(\eta\) within \(4\sigma/\sqrt{n}\) of 0; \(\Phi(0.05)\Phi(0.03)=\Phi(0.08)\) to \(10^{-15}\).
2. **Covariance closure:** \(\Delta=0.05\), \(q=1\), \(x_0\sim\mathcal N([20,5,-8,1],\mathrm{diag}(1,1,0.25,0.25))\), 200 steps; Monte-Carlo (1e5) \(\mathrm{Cov}(\eta_k)\), \(\mathrm{Cov}(e_k)\) vs Lemma 2; assert relative Frobenius error <2% (MC error), except the declared bounds which must dominate (PSD difference ≥ −1e-3).
3. **Associativity/parallel equivalence:** build 7-dim elements for \(N=4096\); Blelloch scan vs sequential lifted KF: max relative difference <\(10^{-9}\); random triple \((e_1\otimes e_2)\otimes e_3\) vs \(e_1\otimes(e_2\otimes e_3)\) <\(10^{-12}\) relative (use Lemma-7 elements; naive elements must fail as in E6).
4. **Statistical validity:** 2000 MC runs, \(\sigma_r=0.1\) m, \(\sigma_\theta=0.35^\circ\), \(\sigma_v=0.05\) m/s. Assert time-averaged MSE of \(\hat\xi\) ≤ trace of filter covariance ×1.1 (LMMSE covariance is exact for the MSE, not a Gaussian NEES claim). Report position/speed RMSE vs EKF, UKF and 3-iteration parallel IEKS on identical data; report span (sequential depth) per method. Provisional promotion target: RMSE within 10% of IEKS with ≥3× lower depth; if worse than EKF, record as negative result.
5. **Speed corollary:** zero-angle-information run (drop \(e_{1:2}\)); assert \(\hat c\to\|v\|^2\) with MSE matching filter covariance.

## N22g — Occupancy-BLUE CFAR: Closed-Form Tracker/Ghost-Informed Noise Estimation with Exact \(P_{fa}\) and SNR-Scaled \(R(t)\)

### Core Theorem

Square-law detector, homogeneous noise power \(\mu\); reference cell powers \(P_i\), \(i=1..N\), independent. From the **predicted** (pre-update) track density \(x_{k|k-1}\sim\mathcal N(\hat x,P)\) and ghost hypotheses (N20g/N26 geometry), each reference cell has occupancy probability and expected SNR:
\[
\pi_i=1-(1-\pi_i^{d})(1-\pi_i^{g}),\quad
\pi_i^{d}=P_D\!\int_{\mathcal C_i}\!\mathcal N(z;H\hat x,\,HPH^T+R)\,dz,\quad
\pi_i^{g}=P_D^{g}\!\int_{\mathcal C_i}\!\mathcal N(z;h_g(\hat x),\,G_gPG_g^T+R_g)\,dz,
\]
and cell model: w.p. \(1-\pi_i\): \(P_i\sim\mathrm{Exp}(\mu)\); w.p. \(\pi_i\): \(P_i\sim\mathrm{Exp}(\mu(1+s_i))\) (Swerling-1 target of SNR \(s_i\)).

**Lemma 1 (moments).** \(m_i\equiv\mathbb E P_i/\mu=1+\pi_is_i\); \(v_i\equiv\mathrm{Var}P_i/\mu^2=2[1-\pi_i+\pi_i(1+s_i)^2]-(1+\pi_is_i)^2\).

**Theorem 1 (BLUE noise estimator).** Among \(\hat\mu=\sum_i\beta_iP_i\) with \(\mathbb E\hat\mu=\mu\), the variance \(\mu^2\sum\beta_i^2v_i\) is minimized by
\[
\boxed{\beta_i=\frac{m_i/v_i}{\sum_j m_j^2/v_j}},\qquad \mathrm{Var}(\hat\mu)/\mu^2=\Big(\sum_j m_j^2/v_j\Big)^{-1}.
\]
*Proof.* Lagrangian \(\sum\beta_i^2v_i-2\lambda(\sum\beta_im_i-1)\) ⇒ \(\beta_i=\lambda m_i/v_i\); normalize. \(\square\) Clean cells (\(\pi=0\)) reduce to \(\beta=1/N\) (CA-CFAR). Hard censoring is the special case \(\beta_i=0\); Theorem 1 dominates it because a contaminated cell still carries information \(m_i^2/v_i>0\).

**Theorem 2 (exact \(P_{fa}\) with prior-only weights).** Because \(\beta\) depends only on the predicted track/ghost state (past scans) and not on current \(P_i\), conditionally on the past and under noise-only CUT and reference cells:
\[
\boxed{P_{fa}(\alpha)=\Pr\!\Big(X>\alpha\sum_i\beta_iY_i\Big)=\prod_{i=1}^N\frac{1}{1+\alpha\beta_i}},\qquad X,Y_i\overset{iid}{\sim}\mathrm{Exp}(1).
\]
*Proof.* \(\Pr(X>t)=e^{-t}\); \(\mathbb E\,e^{-\alpha\beta_iY_i}=(1+\alpha\beta_i)^{-1}\); independence. \(\square\) \(\alpha\) solves \(g(\alpha)=\sum\ln(1+\alpha\beta_i)+\ln P_{fa}^*=0\): \(g\) strictly increasing and concave ⇒ unique root, Newton from \(\alpha_0=N(P_{fa}^{*-1/N}-1)\) converges monotonically. The data-dependent EM variant \(\beta_i\propto w_i+(1-w_i)/(1+s_i)\) (posterior responsibilities \(w_i\)) has a closed-form M-step \(\hat\mu=\tfrac1N\sum P_i[w_i+(1-w_i)/(1+s_i)]\) but **loses** Theorem 2's exact \(P_{fa}\); keep it only as a diagnostic.

**Theorem 3 (information bridge to the tracker).** Detection measurement information scales with SNR (CRB for range/angle of a single tone: \(\sigma^2\propto1/\mathrm{SNR}\)). Use the **predicted** SNR to avoid data-dependent gain bias:
\[
R_k^{-1}=\frac{\hat s_k}{\kappa}\,I,\quad \hat s_k=\frac{\mathbb E_{k|k-1}[P_{\mathrm{CUT}}]}{\hat\mu}-1,\qquad
Y_{k|k}=Y_{k|k-1}+H^TR_k^{-1}H.
\]
Closed loop \(P\to\pi\to\beta\to\hat\mu\to R\to P\). For a scalar random-walk channel, steady-state prior variance \(P=\tfrac12(q+\sqrt{q^2+4qR})\), so \(\partial P/\partial R=q/\sqrt{q^2+4qR}<1\). Sufficient condition for a unique stable fixed point (Banach):
\[
\Big|\frac{\partial P}{\partial R}\Big|\cdot\Big|\frac{\partial R}{\partial\hat\mu}\Big|\cdot\Big|\frac{\partial\hat\mu}{\partial P}\Big|<1,\quad
\frac{\partial R}{\partial\hat\mu}=\frac{\kappa\,\mathbb E P_{\mathrm{CUT}}}{(\mathbb EP_{\mathrm{CUT}}-\hat\mu)^2},\quad
\frac{\partial\hat\mu}{\partial P}=\sum_i P_i\frac{\partial\beta_i}{\partial\pi_i}\frac{\partial\pi_i}{\partial P}.
\]
Ghost coupling: a confirmed N20g/N26 ghost hypothesis raises \(\pi_i^g\) in its predicted cell, preventing the ghost's energy from inflating \(\hat\mu\) (masking weak true targets nearby) while the ghost itself is routed to virtual-aperture assimilation rather than rejected.

**Worked example (hand-computed).** \(N=16\); 14 clean cells; 2 cells with \(\pi=0.3,\ s=10\) (10 dB). \(m=4,\ v=58\). \(\sum m^2/v=14+2\cdot16/58=14.5517\). \(\beta_{\text{clean}}=0.06872\), \(\beta_{\text{cont}}=0.004740\); unbiasedness \(14(0.06872)+2(4)(0.00474)=1.000\). Relative variance \(1/14.5517=0.0687\) vs hard-censor CA-14: \(0.0714\); plain CA-16 is biased by \((14+8)/16-1=+37.5\%\). \(P_{fa}^*=10^{-4}\): \(\alpha\approx13.3\) (hand iteration; CA-16 would use \(\alpha=16(10^{4/16}-1)\approx12.45\)).

### Patent Claim Hook

A radar detector whose reference-cell weights are the closed-form best-linear-unbiased weights \(\beta_i\propto m_i/v_i\) computed from **tracker-predicted and multipath-ghost-predicted** cell occupancy and SNR, whose threshold multiplier is set from the exact product formula \(\prod(1+\alpha\beta_i)^{-1}=P_{fa}\), and whose resulting noise estimate sets the tracker's measurement information via predicted SNR, with a contraction condition governing the detector–tracker loop. Distinct from knowledge-aided/clutter-map CFAR (static priors), hard track-masked censoring (Run 2/T1 lineage, \(\beta_i\in\{0,1/N_{\text{eff}}\}\)), and tracker-aided detection that adjusts thresholds without an unbiased-variance-optimal estimator and exact \(P_{fa}\). Prior-art search still required: "weighted CA-CFAR track prior", Willett/Blanding tracker-aided detection, OS/TM-CFAR weighted variants.

### Numerical Verification Recipe

1. **Weights/threshold:** inputs of the worked example; assert \(\sum\beta_im_i=1\pm10^{-12}\), root \(g(\alpha)=0\) to \(10^{-12}\), \(\alpha\in[13.2,13.4]\).
2. **Exact \(P_{fa}\):** \(10^7\) noise-only trials (all 17 cells \(\mathrm{Exp}(1)\)); empirical \(P_{fa}\) within the 99% binomial interval of \(10^{-4}\). Repeat for random \(\beta\) (Dirichlet) — formula must hold for any fixed \(\beta\).
3. **Bias/variance under contamination:** draw occupancy per \(\pi_i\) with \(s_i=10\); \(10^6\) trials; assert \(|\mathbb E\hat\mu/\mu-1|<3\times10^{-3}\) and \(\mathrm{Var}\) within 2% of \(1/\sum m^2/v\); report CA-16 bias (+37.5% expected) and CA-14 variance.
4. **Closed loop:** Run 2 synthetic RD map (128×32, 25× wall + 6× skirt, targets at (56,16),(100,20)) plus one moving target and its N20g ghost; tracker CV EKF with \(R\) from Theorem 3. Compare static CA, T1 dual-loop (Run 2: recall 1.00/FA 0), hard censor, Occupancy-BLUE: recall, FA, tracker RMSE, loop-gain estimate. Assert: loop-gain product <1 at every step where it is claimed stable; FA not higher than T1; weak target adjacent to ghost detected (recall gain vs CA reported, no target assumed).
5. **Kill:** if prior miscalibration (\(\pi\) overconfident, e.g. \(P\) under-reported by 4×) raises \(P_{fa}\)-under-contamination or misses beyond hard censoring, record the failure and require \(\pi\) from inflated covariance; if gains vanish when \(\pi\) comes from the tracker rather than oracle truth, discard.

**Priority for next model:** N20g (closes the curved-rail blocker of the 80-point chain with exact linear algebra) → N22g (cheap, exact, directly testable on Run 2 scenes) → N21g (HPCC replay; verify novelty against polynomial/Carleman filtering and parallel IEKS before investing).
