# Worklog: ADAS Radar Perception / TinyML / HPCC / Generative Resimulation

**Session started:** 2026-09-26 | **Venue target:** IEEE RadarConf / CVPRW / MLSys / NeurIPS-theory | **Branch:** research/adas-radar-20260926

## Key Insights
- (Run 1) Local corpus is real and readable: corner-radar HDF5 (CEER groups) + 124-row MF4 manifest + KPI scripts in resim_research/kpi. No cluster access — HPCC work stays a local multi-worker replay simulator.
- (Run 1) gcc/clang absent on this host: C++ prototypes are draft-only until a toolchain appears; Python/NumPy is the verification path.

## Next Ideas
- T1 dual-loop CFAR vs static LUT on spray/clutter frames; T2 ghost score G on overpass synthetic; T3 yaw-sweep residual; T4 cut-in recall; T5 Joseph-form definiteness; F Doppler-RadarSplat MVP; H prefix-scan associativity + speedup curve.

### Run 1: baseline env audit + field map — novelty_score=55.0 (KEEP)
- Timestamp: 2026-09-26
- What changed: setup committed (state files, strategy graph N1..N7, equations ledger E1..E6, storage.md skeleton); verified Python 3.13.4 + numpy/scipy/h5py/pandas import OK; confirmed edge_hdf HDF5 groups [CEER_ FL, FR, RL, RR, FLR]; mf4_MANIFEST.csv (124,3); sandbox copies inventoried via SOURCES.md.
- Math: none yet — equations E1..E6 seeded as frontier (unverified).
- Result: novelty_score=55.0 (baseline), prior_art_clear=1.
- Insight: venue routing fixed (RadarConf/CVPRW/MLSys/NeurIPS-theory); high-index citation counts still to collect — mandatory before any keep that builds on a paper.
- Next: Run 2 = T1 dual-loop CFAR numeric prototype + equation verification (critical thinking: attack static-R0 assumption).
