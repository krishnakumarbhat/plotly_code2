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
