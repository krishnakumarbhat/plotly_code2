# Storage Ledger: ADAS Radar 35h Autonomous Run (started 2026-09-26)

Chronological engineering ledger. Every metric below is computed by an executed script — no invented numbers.

## Experiment Ledger [2026-09-26 00:00 UTC] — Env audit & corpus verification
### Hypothesis & Theoretical Basis
Baseline: local corpus suffices to open all five A-PRISM tracks + frontier + HPCC without cluster access.
### Target Codebase Touchpoints
`resim_research/` sandbox copies (SOURCES.md), `resim_research/kpi/` KPI scripts, `edge_hdf/*.h5`, `mf4_data/mf4_MANIFEST.csv`, `Research/Resim_MF4_Sample_20260925/`.
### Code Changes & Prototypes Created
Setup only: autoresearch state files + strategy graph (N1 kept, N2..N8 frontier) + equations ledger (E1..E7). No algorithm code yet.
### Raw Metric Outputs & KPI Comparison Tables
| Check | Result |
|---|---|
| Python | 3.13.4 OK |
| numpy/scipy/h5py/pandas | import OK |
| gcc/clang | MISSING — C++ compile-check deferred |
| edge_hdf HDF5 groups | [CEER_ FL, FR, RL, RR, FLR] verified via h5py |
| mf4_MANIFEST.csv | (124, 3) rows |
| Tier-1 / Tier-2 / CAN KPI | baselines not yet run — pending |
### Failure Analysis & Anomalies Encountered
- `edge_hdf/udp.json`/`can.json` are JSON sidecars, not HDF5 (h5py open fails — expected).
- `Resim_MF4_Sample_20260925/` holds manifests/inventory, bulk `.mf4` lives under `mf4_data/` per manifest paths.
- Deleted-file dirty state (README.md/research.md/research_plan.md) predates this branch; left untouched.
### Verification Verdict (PASS / FAIL / REGRESSION)
PASS (audit) — loop baseline novelty_score=55.0 logged as Run 1.

## Experiment Ledger [2026-09-26 inline] — Run 2: T1 dual-loop CFAR (N2)
### Hypothesis & Theoretical Basis
Static R0 smears guardrail-wall energy into edge-bin thresholds (F1 miss). Dual time-scale floor + span-heterogeneity window + censoring recovers edge targets without FA flood. Equations E1.
### Target Codebase Touchpoints
`resim_research/dual_loop_cfar.py` (new); production `cfar.c` static LUTs untouched (read-only).
### Code Changes & Prototypes Created
DualLoopCFAR (preallocated buffers, no in-loop alloc): EMA floor λ=0.02 over track-free medians; mr(r) span ratio → R0 ∈ {8,16,32}; clean-side min(lead,lag) censor; median×1/ln2; alpha(N_eff); two-regime wall/skirt switch at 12× global median; M-of-2 transition confirmation (1-bit history). `demo()` asserts recall gain + no FA regression.
### Raw Metric Outputs & KPI Comparison Tables
Synthetic 128×32 Range-Doppler, 25× wall (bins 30–54) + 6× skirt (55–57), targets (56,16) edge-skirt + (100,20) clean; 4 seeds (7 warm, 100–102 scored, fresh speckle/scan):

| Detector | Recall | False alarms | CDC load |
|---|---|---|---|
| Static CA-CFAR (R0=16, fixed α) | 0.50 | 1 | 2 |
| Dual-loop candidate | 1.00 | 0 | 2 |

### Failure Analysis & Anomalies Encountered
- Initial candidate tied recall but added FAs (median/mean scaling fault), then missed edge CUT (CUT-relative censor fault), then FA-flooded (fixed-N alpha fault) — all caught by demo asserts, fixed numerically. Full trace in worklog Run 2.
- Driver iters 1–2 produced zero output before watchdog (backend probe healthy) — root cause under investigation; this run executed inline.
### Verification Verdict (PASS / FAIL / REGRESSION)
PASS as engineering benchmark (dual-loop recall 0.50→1.00, FA 1→0). Idea novelty 62 → DISCARD per loop bar (prior-art lineage documented); prototype retained per directive.
