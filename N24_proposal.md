=== N24 PROPOSAL: ROBUST DISTRIBUTION-FREE GATE (COMPACT) ===

CONTEXT: Run16 outlier quantile poisoning widened gate width 2.6 vs .49, RMSE ~3x.
SOLUTION: frozen independent clean calibration + bounded-acceleration reachable-tube gate + 2-of-3 union-of-intersections. Zero online quantile update.

--- ASSUMPTIONS (EXPLICIT, NOT IMPOSSIBLE) ---
A1. Calibration C = {(x_j, {z_j,s}_{s=1..3})}_{j=1..n}, n=255, exchangeable, drawn independently of online stream.
A2. Geometry h_s(x;xi) certified on independent reference block; xi_frozen = xi_cal.
A3. Calibration: all 3 sensors honest; arbitrary adaptive replacements bounded by m=8 (high outliers only, poison top ranks).
A4. Temporal drift allowed; UNCONDITIONAL coverage impossible online. Coverage holds CONDITIONALLY: x_true(t) in R_t(x,v,a_max) and <=1 Byzantine sensor online.

--- CALIBRATION SCORE (MAX NORMALIZED ERROR, FROZEN) ---
s_j = max_{s=1..3} ||z_j,s - h_s(x_j; xi_frozen)||_{Sigma_s^{-1}} / sqrt(d_s)
where d_s = dim(z_s); Sigma_s frozen from certified geometry. Max selects worst honest error; normalization removes dimension bias.
Quantile index: k = ceil((n+1)(1-alpha)) = ceil(256*0.9) = 231.
q_frozen = s_(k) ascending, deterministic tie-break by index j (no interpolation, preserves integer rank under quantization).

--- REPLACEMENT-ROBUST RANK SANDWICH (THEOREM) ---
Theorem: Let original clean scores ordered s_(1)..s_(255). Replace <=m=8 entries by arbitrary high outliers A (unbounded). Observed ordered s'_obs.
Define q_obs = s'_obs(k+m) = s'_obs(239).
Then s_(231) <= q_obs <= s_(247), and k+2m = 247 <= 255 => finite width preserved.
Proof (high-outlier poisoning, top-rank contamination):
  - At most m clean entries are displaced downward by adversarial insertions at top.
  - Clean entries remaining = 255 - 8 = 247; their highest rank is s_(247).
  - To reach index 239 observed, at most m=8 of those slots can be adversarial => at least 231 observed entries are clean <= s_(247). Thus q_obs <= s_(247).
  - Conversely, adversarial insertions occupy top slots; the 239th slot must sit at or below the 231st original clean rank => q_obs >= s_(231).
  Hence frozen q_frozen = s_(231) <= q_obs <= s_(247). Gate does NOT shrink below calibration; upper expansion bounded by clean tail, not unbounded.
Quantization: histogram bins 256 (i/256 deterministic vector) or 64 (i/64). Angle wrapped: theta_w = ((theta+pi) % 2pi) - pi. 2D/4D rectilinear tube: [xmin,xmax]x[ymin,ymax] or (x,y,vx,vy).

--- REACHABLE TUBE (MANEUVER ADAPTATION, NO INNOVATION RANK) ---
R_t(x_cand, v_cand, a_max) = { x | ||(x - x_cand) - v_cand*dt - 0.5*a*dt^2||_W <= eps_tube, ||a|| <= a_max }
with a_max in {6, 12} m/s^2 (online cut-in), W = diag(w_x,w_y,w_v) calibrated from geometry scale, dt fixed to sensor epoch (e.g. 50 ms). Signed angle wrapped properly; tube expanded deterministically. No stochastic innovation rank used => outliers cannot poison tube geometry.

--- ONLINE GATE (2-OF-3 UNION-OF-INTERSECTIONS) ---
Per-sensor acceptance region:
  A_s(t) = { z_s | min_{x in R_t} ||z_s - h_s(x;xi_frozen)||_{Sigma_s^{-1}} / sqrt(d_s) <= q_frozen }
Gate G(t):
  G(t) = (sum_{s=1..3} 1_{A_s(t)} >= 2)  AND  (intersection_{s in H} A_s(t) != empty)
where H = {honest sensors}, |H| >= 2 (since <=1 Byzantine). If exactly 1 outlier: both honest A_s non-empty => overlap exists (tube bounded); outlier score > q_frozen with probability bounded by 1 - p_marg, cannot collapse intersection.
Inclusion Theorem (not association correctness):
  P( x_true(t) in cap_{s in H} A_s(t) ) >= p_pair = (231/256)^2 ≈ 0.814 (joint of 2 independent honest sensors).
Marginal measurement coverage (independent of online replacement):
  p_marg = 231/256 = 0.90234375 = k/256 (deterministic binning). With 2 honest sensors, both scores <= q => coverage bounded; replaced sensor's score irrelevant to gate.

--- OPERATIONAL FIXED SMALL OPERATIONS ---
Per scan: compute 3 normalized errors (3 sqrt + 3 mult), compare to scalar q_frozen (3 compares), count >=2 (add + compare), compute tube (constant-time rectilinear expand, fixed a_max). Total ~30 FLOP, 1 branch, no iteration, no quantile recompute.

--- COVERAGE / WIDTH SUMMARY ---
n=255, alpha=.1 => k=231, q_obs sandwich [231, 247] (m=8, 2m headroom). Width finite; huge high outliers affect only top 8 ranks, never pull q_obs above 247.
Marginal: 231/256 = 0.90234375. Pair joint (2 honest): ≈0.8142. Outlier replaced online => honest scores unchanged => 2-of-3 holds iff both honest <= q, independent of outlier.
Quantile recompute ONLY on externally labeled independent reference blocks; NEVER on accepted associations. Online truth bounded by reachable tube assumption; corruption affects only replaced measurement, not predictor or quantile.

--- SKIPPED / WHEN TO ADD ---
skipped: adaptive innovation-rank gate (contaminated by outlier); unconditional temporal-drift coverage (impossible); custom cache; full non-rectilinear curved-guardrail tube. Add when: reference block label rate exceeds calibration refresh; curved reflector requires polar tube; Byzantine count online exceeds 1 (need m>8 or 3-of-5 majority).

=== COMPACT EQUATIONS ===
q_frozen = s_(231),  s_j = max_s ||delta_z_s||_Sigma_s / sqrt(d_s)
R_t(x,v,a_max; eps,W,dt) = {x | ||(x - x - v*dt - 0.5*a*dt^2)||_W <= eps, ||a|| <= a_max}
G = [sum 1_A_s >= 2] && [intersect_A_H != empty]
p_marg = 231/256; p_pair = (231/256)^2; q_obs in [s_(231), s_(247)]
Angle wrapped: theta_w = ((theta + pi) % 2pi) - pi; bins v_i = i/256.
Fixed ops: ~30 FLOP/scan; a_max = 6 or 12 m/s^2.

=== END N24 ===
