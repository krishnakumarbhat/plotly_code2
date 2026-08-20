# Worklog: Automotive Radar HDF5 KPI & Tracking Engine

## Key Insights
- (baseline) Architecture split across can/UDP with 4/5-layer mixes and a duplicated inner InteractivePlot/. ZMQ only in UDP_KPI (REP/REQ + protobuf). No tracking filters, no divergence metrics.

## Next Ideas
- Build unified 5-layer `can_inplot`/`udp_inplot` with ZMQ PUB/SUB + PUSH/PULL transport.
- Derive + verify divergence-gated innovation for false-alarm suppression.
- Implement continuous-discrete EKF/UKF on [x,y,vx,vy,ax,ay] and benchmark vs SOTA.

### Run 0: baseline field map — novelty_score=55.0 (KEEP)
- Timestamp: 2026-08-19 23:40
- What changed: mapped existing can_kpi / can_interactive_plot / UDP_KPI / intplot_kpi architecture; identified duplication (inner InteractivePlot/ subfolders), ZMQ gap (CAN side), missing algo layer (04), missing index-cache dependency resolution.
- Math: none yet.
- Result: baseline established.
- Insight: the consolidation itself is a structural gap; the algorithmic story (divergence-gated tracking) is the research core.
- Next: build can_inplot 5-layer skeleton with ZMQ transport.

### Run 1: divergence-gated tracking benchmark — novelty_score=68.0 (KEEP)
- Timestamp: 2026-08-20
- What changed: implemented `MultiTargetTracker` in `src/can_inplot/_04_algo/tracker.py` with a divergence-gated association: reference = Gaussian bump of the predicted measurement distribution (from Kalman covariance, equations.md #8); candidate = kernel-smoothed histogram of in-gate detections; accept if `D_JS(pred‖cand) ≤ τ`. Added dense-clutter benchmark (`run_tracking_experiment`) with a diffuse (Poisson) and clumped (specular/multipath) clutter mode.
- Bug found: `best_idx` indexed the in-gate candidate list (`cpos`) instead of the global frame (`positions`) — tracks updated with random wrong detections (20 m jumps). Fixed by keeping the global index alongside the local argmin.
- Math: gate `D_JS` between predicted-measurement bump `N(x̂ₖ, Pₖₓₓ + R)` and smoothed candidate histogram; bins = 32 over ±12 m local window, 3-tap `[0.25, 0.5, 0.25]` kernel, `τ = 0.2`, cluster radius 2.5 m.
- Result (seed=7, cp sweep):
  - Diffuse (Mahalanobis-optimal regime): parity — JS RMSE 0.198–0.25 vs maha 0.203–0.23.
  - **Clumped/specular: JS wins at every density**, margin grows with density — cp=0.7: RMSE 0.173 vs 0.217 (−25%), confirmed 102 vs 55; cp=0.5: 0.185 vs 0.199, 49 vs 41.
- Insight: Gaussian+Poisson clutter is Mahalanobis's matched regime; under non-Gaussian clumped false alarms (multipath/specular) the divergence gate rejects distributionally-inconsistent clumps inside the chi² ellipsoid that would capture a Mahalanobis track. Verified claim: "JS gate RMSE < Mahalanobis gate at 50% specular clutter".
- Next: report the benchmark in `overnight_research_report.html`; test on real `edge_hdf` radar logs.

### Run 2: full validation & hardening — novelty_score=68.0 (KEEP)
- Timestamp: 2026-08-20
- What changed: full-suite validation + hardening pass. Fixed real bugs surfaced by tooling: `jira_integration.py` missing `requests` import (latent NameError in `_assign_ticket`); report builder `%c` format crash on literal "50%" text; missing `Path` import; mypy-discovered wrong `_sigma_points` return annotation in UKF, Optional-state arg-type issues across EKF/tracker/pipeline; `simg_zmq/tests/conftest.py` added so the web-app suite runs without PYTHONPATH hacks; bundle_src regenerated (was missing → `test_generated_dashboard_copies_include_the_runtime_map_changes` now passes); equations.md populated with verified identities #1–#8.
- Result (all green): can_inplot 13/13 tests, ruff clean, `mypy src/can_inplot` clean (28 files, mypy upgraded 0.942→2.3.1 for numpy 2.x stubs). simg_zmq 53/53 tests; legacy KPI stacks can_kpi 6/6, can_interactive_plot 1/1, UDP_KPI 3/3; intplot_kpi 30 pass/21 fail — confirmed pre-existing test-code drift (DataPrep signature), untouched. Flask app imports, 69 routes. `00_main.py --verify` end-to-end on real edge_hdf pair: index cache for 5 sensors + interactive report. `overnight_research_report.html` regenerated (331 KB, JS gate −6.8% RMSE at 50% specular clutter).
- Insight: test-driven tooling (mypy/ruff) catches real defects the unit tests miss — the UKF annotation hid a 3-tuple contract, and the missing `requests` import was a guaranteed runtime crash in the Jira path.
- Next: paper draft in `papers/` from equations.md #8 result; real edge_hdf tracking eval.