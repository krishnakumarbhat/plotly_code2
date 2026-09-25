# RESEARCH INVENTIONS & ROADMAP — Radar Re-Simulation Platform

**Workspace:** `C:\Users\ouymc2\Desktop\Research` · **Date:** 2026-09-20
**Companions:** `SYSTEM_ARCHITECTURE_DEEP_DIVE.md` (architecture ground truth) · `radar_resim_learning_portal.html` (interactive learning)
**11 repos in scope:** Core_Radar_Gen7_SAF85xx, Core_Radar_Gen8_iND13400, core-radar-gen7-awr294x (NEW — Gen7v1 AWR294x satellite FW), core-resim-logic-model (NEW — LM2 logic model / FMU), Core_RESIM_UDP_Decoder_Library, Core_RESIM_Bordnet_Tool, Core_RESIM_DC_Emb_Library, Core_RESIM_HIL_Engine, Core_RESIM_KPI (Core_RESIM_HPCC, Core_RESIM_USS_Sensor_Model remain empty placeholders — USS work is live on core-resim-logic-model/feature/USS_Aggregator). **50 branch snapshots** under _branch_snapshots/<repo>/<branch>/ with full _BRANCH_CATALOG.txt per repo (see Appendix B).

---

## 0. Executive summary

Twelve failure modes (F1–F12, catalogued in the Deep Dive §9) bound the platform's regression ceiling. Four are **architectural** (fixed-R0 clutter reference, single-look ghost confirmation, per-sensor timestamps without cross-satellite extrapolation, static association gates) and each is independently convertible into a patentable perception improvement whose KPI effect is directly measurable in the existing harness — no new loggers, no new silicon. This document converts the failure catalogue into (a) a severity × likelihood bottleneck matrix, (b) a sequenced investigation backlog, (c) four Invention Disclosure Forms with LaTeX-grade math, integration points, and validation experiments, (d) executable HDF5 mining tooling that runs against today's `h5py` datasets, and (e) a quarterly roadmap with staffing shape and risk register.

**Appendices:** [B — Multi-branch update & new-repo surface](#appendix-b--multi-branch-investigation-update--new-repo-research-surface-audited-2026-09-23) · [C — Protocols, prior-art & validation matrix](#appendix-c--experiment-protocols-prior-art-register--branch-validation-matrix-2026-09-24)

**Expected cumulative KPI effect (conservative, per invention §3–§6):** UDP Tier-2 all-params +0.5…2.0 pp each for IDF-1/IDF-2; CAN association recall +3…8 pp for IDF-4; cross-satellite fusion disagreement −30…60% for IDF-3. Combined target: UDP sub-parameter accuracies ≥ 99.5% on clean logs excluding saturation, CAN F1 ≥ 0.97 on 50 ms continuity-clean runs.

---

## 1. Bottleneck matrix

| ID | Symptom (observable in KPI) | Pipeline stage / owner file | KPI metric that moves | Severity × Likelihood | Research question |
|----|------------------------------|-----------------------------|----------------------|----------------------|-------------------|
| F1 | Clutter-ridge divergence misses, range-adjacent baseline-only detections | Range/CFAR `software/bbe32/src/range_proc.c`, `appl_cfar_ifc` (`Appl_Cfar_Execute`), thresholds `bwdep_*_thold[MAX_RANGE_BINS]` | UDP Tier-1 pair rate; `Δr` tail | **S4 × L5** | Can the clutter reference track local range statistics instead of a fixed cell count? → **IDF-1** |
| F2 | Multipath ghosts pass UDP Tier-1 (paired) then poison tracks | AF + tracker `anglefinding_project_interface.c`, `emb_tracker_wrapper.cpp::Run_Emb_Tracker` | Tier-1 pair rate inflated; CAN precision down | **S5 × L4** | Can temporal persistence + geometric mirror tests separate ghosts pre-association? → **IDF-2** |
| F3 | Inter-satellite primary-echo disagreement (same target, different (r,θ)) | Harness time base `sil_wrapper.cpp`, Gen7/8 `M2D_Look_Info_T`, `SIL_Engine_Config.xml` L_CURVE/EgoMotion | Cross-sensor fusion residual; CAN ID-switch rate | **S4 × L4** | Can per-detection sensor-time extrapolation to a common fusion epoch remove the skew? → **IDF-3** |
| F4 | Maneuvering-target handoff drops (track coasts, candidate re-inits) | Tracker gate + CAN match `emb_tracker`, `can_kpi_hdf/c_business_layer/kpi_business.py` | CAN recall, ID continuity, `trk_cov` growth | **S4 × L4** | Can the association gate adapt to innovation and maneuver probability? → **IDF-4** |
| F5 | CDC saturation truncation (urban dense scenes) | `cdc_packing.c`, `detection_matching_kpi_script.py:146` (`MAX_CDC_RECORDS=5016`) | `avg_cdc` accuracy, excluded-accuracy delta | S3 × L4 | Priority-ranked CDC packing (RCS/SNR/near-field first) — §7.3 |
| F6 | Range saturation (>135 m) | `rdd_stream.h` fixed-point widths; `hdf_parser.py` saturation flag | `avg_rng` accuracy | S2 × L3 | Range-conditional tolerance + saturation-aware scoring — §7.3 |
| F7 | Fascia/bumper bias after part change | `Execute_Fascia_Compensation`, `calib_cfg` SMC blob | `avg_azm/avg_elv` bias (not scatter) | S3 × L3 | Self-cal trigger from bias monitor — §7.2 |
| F8 | Dynamic-alignment lock loss (long curves, construction) | `dynamic_alignment`, `da_sil` wrappers, `_DYNAMIC_ALIGNMENT_STREAM` | CAN F1 on curve segments | S3 × L3 | Alignment-quality-gated fusion weights — §7.2 |
| F9 | Jammer/interference leak into RDD | ID stage `IntfDet`, jammer flags before CFAR | `missandjamm` accuracy; Tier-2 tails | S3 × L2 | Interference-aware CFAR blanking — backlog §2 |
| F10 | Scan drops / sync loss (SiL + HiL) | `SyncManager::DoWait`, `HiLExecutive::RequestNextFrame`, `can_xml_kpi_scripts.py` continuity | `scan_index_continuity` PASSED/FAILED | S4 × L2 | Deterministic replay barrier + gap-aware scorer — backlog §2 |
| F11 | Latency regression (candidate slower than baseline) | `module_time_ms`/`sourceTxTime`, `timing_plotter.py` ΔT | ΔT histogram shift | S2 × L3 | Latency budget CI gate — backlog §2 |
| F12 | Version shear (DBC/stream vs decoder) | `Decoder_DLL_Release_Notes.xml`, autogen `*_DBC_signal_macros.h` | Parser exceptions, hash mismatch | S3 × L2 | Version-pinned decoder CI — backlog §2 |

**Reading guide.** F1–F4 are invention-grade (novel algorithm, measurable, shippable). F5–F6 are scoring/packing fixes (weeks). F7–F8 are calibration-loop closures (months, needs fleet data). F9–F12 are infrastructure hardening (continuous).

---

## 2. Prioritized investigation backlog (sequenced)

| Order | Work item | Entry point (exact file) | Output artifact | Effort |
|-------|-----------|--------------------------|-----------------|--------|
| 1 | Mine clutter-floor vs range on 3+ highway logs; fit R0(R) curve | `radar_hdf_datacollect.py` + mining script §8 | `clutter_floor_by_range.png` + R0(R) table | 1–2 wk |
| 2 | Ghost persistence statistics: pair ghost survival over N looks | `detection_matching_kpi_script.py` ghost outputs + §8 extension | survival histogram, mirror-test precision/recall | 2 wk |
| 3 | Cross-satellite timestamp skew: measure Δt distribution per log | `SRR_DC_Lib_Control.xml` timestamps, `sourceTxTime` | skew CDF, fusion-residual baseline | 1 wk |
| 4 | Maneuver gating audit: innovation magnitude at drop events | `emb_tracker` XTRK logs, `kpi_business.py` track gates | innovation-at-drop distribution | 2 wk |
| 5 | CDC saturation ranking prototype (RCS/SNR sort before truncation) | `cdc_packing.c::CDC_OutputDataCube` | saturation-recall delta on urban logs | 2–3 wk |
| 6 | Range-conditional tolerance + saturation-aware UDP scorer | `config.py`, `detection_matching_kpi_script.py` | scorer PR, no-threshold-gaming proof | 1 wk |
| 7 | Fascia-bias monitor → recal trigger spec | `Execute_Fascia_Compensation`, SMC blob loader | bias-drift detector + trigger thresholds | 3 wk |
| 8 | Deterministic replay barrier for HiL + gap-aware CAN scorer | `SyncManager`, `can_xml_kpi_scripts.py:772-778` | zero-false-FAILED proof on gapped logs | 2 wk |

Items 1–4 are the experiments that validate IDFs 1–4 (§3–§6 "Validation" subsections). Do them in order; each produces the baseline its IDF must beat.

---

## 3. IDF-1 — Range-Adaptive Clutter Reference CFAR, R(t) → R0(R, t)

### 3.1 Problem statement
First- and second-pass CFAR (`Appl_Cfar_Execute`, Gen8 `rdd_proc`) compare each cell against a noise/clutter estimate averaged over a **fixed** reference window R0. On real roads the clutter floor is range-dependent (guardrail at 20–60 m, road-surface backscatter near-field, rain shelf far-field). A fixed R0 overestimates the floor near strong clutter (→ valid-target suppression, F1 misses) and underestimates it in clean far field (→ false alarms that later saturate CDC, F5). The threshold tables `bwdep_cfar_*_thold[MAX_RANGE_BINS]` already encode *some* range dependence, but as static per-build constants — they cannot track rain, road-wetness, or bumper-aging drift within a drive.

### 3.2 Prior-art gap
Classical CA-CFAR / OS-CFAR / TM-CFAR adapt across cells but assume a **homogeneous** reference window within one CUT evaluation; automotive variants add Doppler guard bands. None in the surveyed stack (Gen7 `doppler_proc.c`, Gen8 `appl_cfar_ifc`) makes the *reference window geometry itself* a function of measured local clutter non-homogeneity. Range-dependent threshold tables exist but are frozen at build time.

### 3.3 Invention
A **dual-loop CFAR**: a slow loop estimates the clutter floor profile $C(r, t)$ per range bin (exponential averager over confirmed-clutter cells, excluding associated-track cells so targets never poison the floor); a fast loop sets per-CUT reference geometry $R_0(r,t)$ — window length and guard width — from the local slope $|dC/dr|$: steep-slope regions (guardrail edges) get **wider** reference windows (more averaging, less edge-triggering) plus local guard extension; flat regions keep today's narrow, sensitive windows. The detection threshold becomes

$$T(r, d, t) = \alpha\big(R_0(r,t)\big)\cdot \hat{C}(r,t)\cdot B(d)$$

where $\alpha(\cdot)$ is the standard CA-CFAR scaling for the *actual* window length in use (look-up, no runtime division), $\hat{C}$ is the slow-loop floor, and $B(d)$ keeps the existing Doppler-dependent term untouched. Minimum/maximum clamps on $R_0$ guarantee worst-case MIPS.

**Algorithm (per scan, per look):**
1. Run Range/Doppler FFTs as today.
2. Update slow floor only from cells **not** claimed by any confirmed track and below a provisional ceiling: $\hat{C}_{k+1}(r) = (1-\lambda)\hat{C}_k(r) + \lambda\cdot \mathrm{median}_{d \in \mathcal{D}_{free}} P(r,d)$, $\lambda \approx 0.02$ (≈50-scan time constant at 20 Hz).
3. Compute discrete slope $s(r) = |\hat{C}(r+1) - \hat{C}(r-1)| / 2$; map $s \mapsto R_0 \in \{R_{min}, R_{mid}, R_{max}\}$ via two slope thresholds (hysteresis ±10% to prevent chatter); extend guard by one cell where $s$ exceeds the upper threshold.
4. Evaluate CFAR with $T(r,d,t)$ above; feed surviving cells into the unchanged Tier-2 / tracker path.

### 3.4 Mathematics
For a CA-CFAR with $N$ reference cells and design $P_{fa}$, $\alpha = N\big(P_{fa}^{-1/N} - 1\big)$. Because $N = R_0(r,t)$ now varies, precompute $\alpha$ for the three admissible $\{R_{min}, R_{mid}, R_{max}\}$ — zero extra runtime cost. Floor-estimator variance: $\mathrm{Var}[\hat{C}] \approx \frac{\lambda}{2-\lambda}\sigma_P^2$ per bin; with $\lambda=0.02$, variance drops ~50× vs single-scan estimate while tracking rain-onset (minutes) two orders of magnitude faster than the drift it must follow. Guard extension cost: at most one extra excluded cell per CUT in steep regions — bounded, provable.

### 3.5 Integration points (exact)
- Gen7: `software/bbe32/src/doppler_proc.c` (threshold application), new `clutter_floor.[ch]` beside `range_proc.c`; floor state in `D2M` persistent block (survives `RECU_SiL_Execute` iterations).
- Gen8: extend `rdd_proc` (`appl_cfar_ifc.c`) with identical interface; floor profile ships in the existing calib/CB path so HiL needs no new channel.
- SiL validation: zero harness changes — improvement appears as Tier-1 pair-rate gain in `detection_matching_kpi_script.py` unmodified.

### 3.6 Expected gains / validation
- **Metric:** UDP `avg_ran`/`avg_vel` Tier-2 accuracy + ghost-floor `unmatched_baseline` count; target +0.5…1.5 pp on guardrail-heavy logs, zero regression on clean highway.
- **Experiment:** A/B `R0-fixed` vs `R0(R,t)` on ≥5 logs (dry/wet/guardrail/urban); report per-range-bin miss-rate curves (backlog item 1 output) — win = miss-rate flattening across 15–70 m with far-field false-alarm count non-increasing.
- **Claims sketch:** (1) per-range-bin slow-loop clutter floor excluding track-claimed cells; (2) slope-mapped reference-window geometry with hysteresis; (3) precomputed α-LUT over admissible window set bounding compute.

---

## 4. IDF-2 — Multi-Look Ghost Suppression via Persistence + Mirror-Geometry Test

### 4.1 Problem statement
Multipath ghosts (guardrail bounce, underbody double-bounce, bridge girder) survive single-scan CFAR and AF because in any one look they are energetically and geometrically plausible (F2). They pair in UDP Tier-1 (same (R,D) bin as *something* energetic), inflate match statistics, then either corrupt tracks or force the tracker to spend gates on phantoms. Today's pipeline has no memory of *which* detections behaved like ghosts.

### 4.2 Prior-art gap
Multi-scan track-before-detect and M/N logic exist in the tracker — but they operate **post-association** (on plots already admitted). Specular-multipath mirror tests exist in literature as single-frame geometric checks. No stage in this stack performs a **pre-association, multi-look** ghost score combining (i) look-to-look persistence statistics with (ii) a road-geometry mirror hypothesis test, emitting a ghost-likelihood that down-weights (not hard-drops) the candidate.

### 4.3 Invention
A ghost-likelihood score $G \in [0,1]$ per RDD detection, computed over a sliding window of $N=5$ looks, fusing two independent evidences:

$$G = \sigma\big(w_1 \cdot \phi_{pers} + w_2 \cdot \phi_{mirror} + b\big)$$

- **Persistence feature** $\phi_{pers}$: real targets persist with smooth $(r, v, \theta)$ evolution; ghosts flicker (appear/vanish with ego-motion phase) and jitter in $\theta$. $\phi_{pers} = 1 - \frac{1}{N-1}\sum_{k} \mathbb{1}[\mathrm{matched}_k]\cdot e^{-|\Delta\theta_k|/\theta_0}$, i.e. penalize detections whose angular history is discontinuous.
- **Mirror feature** $\phi_{mirror}$: for each detection, hypothesize the specular path (guardrail plane from dynamic-alignment road model, or nearest strong stationary return as mirror proxy); predict the mirror ghost position $(r', \theta')$; $\phi_{mirror}$ is high when the detection coincides with a predicted mirror of a stronger parent *and* its Doppler is consistent with $v_{ghost} \approx v_{parent} + 2v_{ego}\cos\psi$ (bounce adds ego-motion projection).
- Detections with $G > \tau_{ghost}$ are **down-weighted** (CFAR second-pass threshold locally raised by $1+ \kappa G$; tracker gate $\gamma$ tightened) rather than deleted — so a maneuvering real target mirrored near a rail is attenuated, never blinded. Weights $(w_1, w_2, b)$, $\tau_{ghost}$, $\kappa$ are calibrated once per radar position on the mining output of backlog item 2.

### 4.4 Mathematics
Persistence match across looks uses the existing Tier-1 integer key plus a Tier-2-light gate ($|\Delta r| \le 3\epsilon_r$, $|\Delta \theta| \le 5\epsilon_\theta$ — intentionally looser than KPI to catch jittery ghosts). Mirror prediction for a planar reflector with unit normal $\mathbf{n}$ through point $\mathbf{p}_0$: $\mathbf{x}_{ghost} = \mathbf{x}_{parent}
...[truncated 9117 chars]
---

# APPENDIX B — MULTI-BRANCH INVESTIGATION UPDATE & NEW-REPO RESEARCH SURFACE (audited 2026-09-23)

> Workspace grew from 9 → **11 git repositories**. Full per-branch source snapshots live under
> `_branch_snapshots/<repo>/<branch>/` (50 copies, ~61 GB); complete branch manifests (ref @@ sha @@ date @@ subject)
> in `_branch_snapshots/<repo>/_BRANCH_CATALOG.txt`. Cross-branch architecture narrative: Appendix A of
> `SYSTEM_ARCHITECTURE_DEEP_DIVE.md`. This appendix (B) folds the new evidence into the research program:
> revised bottleneck inputs, new backlog items, branch-aware validation assets, and two IDF addenda.

## B.0 New repos & the research questions they unlock

| New repo | Research-relevant fact | Feeds |
|---|---|---|
| `core-radar-gen7-awr294x` (~240 branches) | `feature/variable_cfar_initial_changes` [GHW-94] *"Update the CFAR BB to implement variable CFAR LUT"* (2023-02-28) — **in-house variable-CFAR prior art**; `feature/emb-lib-linux` (gcc/glibc toolchain) + `feature/emb-lib-hdf5` [HLR-212] — historical origin of off-target EmLib+HDF5 capture; `feature/HIL_development_branch` *"HIL stream bypass"*; `feature-review/CUX-3104_interference_detection` (F9 prior art) | **IDF-1 prior-art differentiation (B.1)**; F9 backlog; provenance narrative for SiL+KPI loop |
| `core-resim-logic-model` (11 branches, 10 snapshotted) | LM2 FMU packs customer logs → Resim interfaces; `feature/USS_Aggregator` implements the USS aggregation the empty `Core_RESIM_USS_Sensor_Model` placeholder never got; `Interface_Output_Control.xml` RUN_MODE matrix incl. `E_READ_VEHICLE_LOG_N_TRANSMIT` / `E_READ_DSPACE_LOG_N_TRANSMIT`; JBC-191 heap-overflow + ASan; JBC-186 OSMP MUDP TX | **B.2 LM2 research surface**; F12 (customer/version shear is now enumerable via `customer_ID_config.h`); GT-ingestion backlog item 9 |

## B.1 IDF-1 addendum — prior-art differentiation vs `variable_cfar_initial_changes`

AWR294x branch `feature/variable_cfar_initial_changes` (single squashed head `853c0150`, GHW-94, 2023-02-28)
already teaches **precomputed variable-CFAR LUTs**: a build-time table indexed by (range bin, look) selects
threshold/LUT entries. That is *static* adaptivity — the table cannot change within a drive or with weather/road state.

**Claim distinction to carry into the IDF-1 §3.2 prior-art gap (rewrite that paragraph as follows):**

> Prior-art gap (revised 2026-09-23). Three prior layers exist: (i) classical CA/OS/TM-CFAR adapt across
> cells of one CUT evaluation assuming within-window homogeneity; (ii) this program's own Gen7v1 work
> `core-radar-gen7-awr294x@feature/variable_cfar_initial_changes` [GHW-94] generalizes thresholds into a
> **static LUT** indexed by range/look — still build-time constant, blind to rain, wet-road, bumper aging;
> (iii) Gen7v2/Gen8 `bwdep_*_thold[MAX_RANGE_BINS]` tables are the same idea frozen in `radar_sw_config.h`.
> IDF-1's novelty is the **runtime two-loop estimator**: a slow clutter-floor process $\hat C(r,t)$ updated
> *in-drive* from track-free cells, driving a slope-mapped *reference-window geometry* $R_0(r,t)$ (window
> length + guard width), with precomputed $\alpha(N)$ only over the three admissible window sizes so compute
> stays bounded. LUT selects *parameters*; IDF-1 selects *geometry* from measured non-homogeneity.

**Validation asset now available:** archive `core-radar-gen7-awr294x/_branch_snapshots/feature__variable_cfar_initial_changes/`
side-by-side with `feature__Gen7_Downselection` and `dev` — the CFAR building-block files can be diffed to
extract the exact GHW-94 LUT layout as the baseline implementation for the A/B in backlog item 1.

## B.2 LM2 (core-resim-logic-model) research surface

1. **GT ingestion fidelity:** every `Code/Customer/*_Log_Convertor` is a customer-specific decode of logged
   ego + object data feeding `E_READ_VEHICLE_LOG_N_TRANSMIT`. A convertor bug (BMW High/Low/MID, Scania,
   TML SRR5, Motional SRR3, STLA Small IFV600, DSPACE common) silently corrupts the *baseline* half of every
   KPI comparison. **New backlog item 9:** convertor conformance suite — replay one golden log per customer
   through LM2 twice (candidate LM2 vs last-released FMU in `LM2_Packaging/Deliverables/`) and diff MDF4
   outputs (hash-level); failures are F12-class (version/shear) but upstream of the decoders already covered.
2. **RUN_MODE matrix as experiment taxonomy:** `Interface_Output_Control.xml` enumerates the legitimate
   resim entry points — `E_LIME_SIL_FRAMEWORK` (full stub-driven SiL), `E_LIME_HIL_FRAMEWORK` (HiL),
   `E_READ_VEHICLE_LOG_N_TRANSMIT` (bus replay), `E_READ_DSPACE_LOG_N_TRANSMIT` (dSPACE log replay).
   KPI runs should tag reports with RUN_MODE + `Customer_Name` + `XML_Version` so cross-mode comparisons are
   never accidental (**extends F12 with LM2 dimensions**).
3. **USS Aggregator → empty-repo risk retired:** `feature/USS_Aggregator` (456 files, +21.5k lines) ships
   `LM2_USS_AGG_FMU_CEER.fmu` / `USS_AGG_FMU.dll`. If USS work resumes, it starts from this branch —
   not from the empty `Core_RESIM_USS_Sensor_Model`. Roadmap staffing note updated (B.5).
4. **Robustness:** `feature/JBC-191/Heap_overflow_fix` proves a heap overflow was reachable via the XML_Reader
   path (now fixed + ASan). Any new XML schema surface (e.g. adding RUN_MODE tags) must keep ASan CI from this
   branch's build flags.
5. **Windows FMU lineage:** JBC-138 (LoadLibraryExA, MSVC detection) → merged into `adcam_main_dev` →
   `adcam_releases` "v1p2". Packaging bugs here never appear in radar-firmware KPIs but break customer
   deliverables — treat `Deliverables/*.fmu` checksums as a release gate.

## B.3 Branch-aware additions to the bottleneck matrix (F13–F15)

| ID | Symptom | Evidence (branch) | KPI metric | Sev × Lik | Research action |
|----|---------|-------------------|------------|-----------|-----------------|
| F13 | **Scan-index semantics drift** across Gen8 builds — tracker scan index increment bug + header-stream dependency | Fixed on `story/HLR-1436-fusion` (HLR-1606); present on older release cuts | CAN continuity FAILED spike *only* on pre-fix builds | S4 × L4 | Gate KPI comparisons on commit ≥ HLR-1606; add `story/DNP-7666-Align-timestamp-for-tracker` (timestamp alignment, 2026-09-17) to the same gate → **backlog item 10** |
| F14 | **Dynamic-alignment silently off** in RSP_SIL builds → alignment KPI = vacuous pass | `feature/RSP_SIL` EUR-2031 comments out DA in RDD SIL mode | `alignment_matching_kpi_script` N/A or trivially-pass | S3 × L5 | Detect DA-disabled builds (config fingerprint) and mark alignment KPIs skipped-not-passed → **backlog item 11** |
| F15 | **LM2 customer convertor/version shear** corrupts baseline before radar decoders see it | `core-resim-logic-model` `Code/Customer/*`, `customer_ID_config.h`, `Interface_Output_Control.xml` XML_Version/Customer_Name | All KPIs shift together; HTML `meta_data` lacks LM2 identity | S3 × L3 | Log LM2 FMU/DLL version + RUN_MODE into `meta_data.json`; golden-log conformance (B.2 item 1) → **backlog items 9 & 12** |

## B.4 Backlog extension (items 9–12, appended to §2)

| Order | Work item | Entry point | Output | Effort |
|------:|-----------|-------------|--------|--------|
| 9 | LM2 convertor conformance: golden log per customer, dual-FMU MDF4 diff | `core-resim-logic-model/Code/Customer/*_Log_Convertor.cpp`, `LM2_Packaging/Deliverables/*.fmu` | per-customer pass/fail + first-divergent-byte report | 2 wk |
| 10 | KPI build-gate: require tracker-scan-index fix (HLR-1606) + timestamp alignment (DNP-7666) in SHA-256 whitelist | `can_kpi_hdf/c_business_layer/kpi_business.py` entry | gate script + CI job | 0.5 wk |
| 11 | DA-disabled build detector → mark alignment KPI skipped | Gen8 `sil/rsp_sil` config fingerprint (EUR-2031 macro), `alignment_matching_kpi_script.py` | corrected HTML verdict semantics | 0.5 wk |
| 12 | LM2 provenance fields (FMU version, RUN_MODE, Customer_Name, XML_Version) injected into KPI `meta_data.json` | LM2 `Version.cpp`, `Interface_Output_Control.xml`, KPI `meta_data.py` | enriched reports, F15 traceability | 1 wk |

**Revised sequencing:** items 1–4 (IDF-1…4 baselines) unchanged and still first; items 9–12 are cheap
instrumentation — run them in the same sprint as item 3 (timestamp skew) so the F3 measurements already
carry LM2 provenance.

## B.5 Roadmap deltas

- **Q4-2026 (in flight):** add *A1* — extract GHW-94 CFAR LUT layout from AWR294x snapshot as IDF-1 baseline
  comparator (1 wk, piggybacks backlog item 1). Add *A2* — backlog items 9–12 instrumentation (3 wk total).
- **Q1-2027:** fast-resim validation (was implicit) becomes explicit — run IDF A/B on both full-pipeline
  (`sil/rsp_sil`) and fast-resim (`feature/CUW-5824…` `sil/fast_resim/`) paths; disagreement between paths is
  itself a KPI-trust metric.
- **HPCC placeholder:** `Core_RESIM_HPCC` still empty — if mining (backlog 1–2) hits corpus limits locally,
  HPCC is the designated burst target; keep staffing contingency (risk R-5 unchanged).
- **USS:** staffing no longer needs a greenfield sensor-model team — the aggregator exists on LM2
  `feature/USS_Aggregator`; a 0.5 FTE can maintain it if USS returns to plan.
- **Risk additions:** **R-6** — snapshot staleness: `_branch_snapshots/` is a point-in-time export (2026-09-23);
  refresh script (re-run `git archive` per catalog) before using it as a merge base. **R-7** — LFS objects
  unreachable (`jfrog.asux.aptiv.com`); releases containing `SystemModel2.tdb`-class files need a VPN/LFS
  rehydrate step before any build from snapshot.

## B.6 How to navigate the snapshots (operating manual)

1. Full branch lists: `_branch_snapshots/<repo>/_BRANCH_CATALOG.txt` (`ref @@ sha @@ date @@ subject`).
2. Browse a branch without touching git: open `_branch_snapshots/<repo>/<branch_folder>/` — it is a clean
   export of that ref's tree (`/` in ref → `__`; e.g. `origin/feature/JBC-138/Enable_Windows_FMU_build` →
   `feature__JBC-138__Enable_Windows_FMU_build`).
3. Diff two branches without checkout: `git -C <repo> diff --stat <refA> <refB>` (or `diff -ru` two snapshot
   folders for pure content diffs excluding `.git`).
4. To materialize an additional branch: `git -c filter.lfs.smudge= -c filter.lfs.required=false -C <repo>
   archive --format=zip -o out.zip <ref>` then `Expand-Archive`.
5. Never copy snapshots back into the repos (sibling folder by design); they are read-only research artifacts.

---

*Appendix B generated 2026-09-23 from git metadata, diffstats, and tree inspection across 11 repositories /
~1,100 remote refs. Companion: Appendix A of SYSTEM_ARCHITECTURE_DEEP_DIVE.md.*

---

# APPENDIX C — EXPERIMENT PROTOCOLS, PRIOR-ART REGISTER & BRANCH-VALIDATION MATRIX (2026-09-24)

> Executable companion to Appendices A (Deep Dive) and B. Every protocol names the exact snapshot folders,
> config knobs, scripts, and pass criteria — a researcher with this file and `_branch_snapshots/` can run
> each study without asking anyone anything. Prior-art register pins the in-house baselines each IDF must
> beat and distinguish.

## C.1 Prior-art register (in-house baselines — cite in every IDF filing)

| # | Artifact (exact ref) | What it teaches | IDF it bounds | Distinguishing fact |
|---|---|---|---|---|
| PA-1 | `core-radar-gen7-awr294x@feature/variable_cfar_initial_changes` (`853c0150`, GHW-94, 2023-02-28) — CFAR BB variable LUT | Static range/look-indexed threshold LUT in the CFAR building block | IDF-1 | LUT is build-time constant; IDF-1 estimates floor **at runtime** and adapts **window geometry**, not just threshold value |
| PA-2 | `Core_Radar_Gen7_SAF85xx@feature/gen7v2_rdd_sil_dev` — stream-def codegen (`DNP-5148`), RFE trigger move (EUR-735) | How RDD SIL fixtures are generated per release | IDF-2 (test harness) | Fixture generator, not a detector — reuse it to regenerate fixtures for ghost-injection studies |
| PA-3 | `Core_Radar_Gen8_iND13400@feature/RSP_SIL` EUR-2031 (DA commented out in RDD SIL) + `rdu_sil_interface/` | Which pipeline stages are simplifiable in SiL without breaking KPI | IDF-3 | Documents accepted SiL simplifications; IDF-3 must show its extrapolation survives them |
| PA-4 | `Core_Radar_Gen8_iND13400@story/HLR-1436-fusion` (HLR-1606 scan-index fix) | Scan-index increment + header-stream dependency removal | IDF-3/IDF-4 (validity gate) | Any CAN-continuity measurement on pre-HLR-1606 builds is invalid — whitelist SHAs (backlog 10) |
| PA-5 | `core-radar-gen7-awr294x@feature-review/CUX-3104_interference_detection` | Interference/jammer detection feature-review line | IDF-2 (ghost vs jammer disambiguation) | Jammer flags are an input feature to ghost scoring, not a substitute |
| PA-6 | `Core_Radar_Gen8_iND13400@feature/CUW-5824-Fast-Resim_AF-Integration` (`sil/fast_resim/`, NLS_2T_GNK, 2D gridsearch + fast variant) | Streamlined AF (S1/S5 init, elevation FFT, S4 downselect, bistatic detection, error-mag) as a fast path | ALL IDFs (experiment accelerator) | Fast-resim is the inner loop; full `sil/rsp_sil` is the acceptance gate — report both numbers |
| PA-7 | `core-resim-logic-model@feature/JBC-191/Heap_overflow_fix` (ASan flags) + JBC-138 (Windows FMU) | Hardened XML_Reader + Windows FMU build | Backlog 9/12 (LM2 conformance) | Keep ASan flags when adding provenance fields to XML paths |
| PA-8 | `Core_RESIM_DC_Emb_Library@feature/DC_FF_Testing` HLR-930 (DC input data-quality check) | Input gating before DC tracker | IDF-4 (gate inputs) | DataQualityChk verdicts should be logged as covariates in maneuver-gating audits |

## C.2 Experiment protocols (each: setup → run → pass criterion)

**XP-1 · Clutter-floor vs range (feeds IDF-1 baseline, backlog 1).**
Setup: 3+ highway logs with guardrail segments; veh-side `_UDP_GEN7_RDD_CORE.csv` (suffix from `config.RDD_FILE_SUFFIX`).
Run: §8 mining script extended — group `cfar_nf_est`/`cfar_thold` per range bin; fit $\hat C(r)$; overlay GHW-94 LUT values extracted from `awr294x/feature__variable_cfar_initial_changes` CFAR BB sources.
Pass: miss-rate curve 15–70 m flattens (variance across bins −30%+) with far-field false alarms non-increasing; R0(R) table checked in as `clutter_floor_by_range.{png,csv}`.

**XP-2 · Ghost survival over N looks (feeds IDF-2, backlog 2).**
Setup: overpass/guardrail logs; UDP matcher ghost outputs (`sim_pairs` leftovers per `process_one_log()`).
Run: link ghosts across looks with Tier-2-light gate ($|\Delta r|\le3\epsilon_r$, $|\Delta\theta|\le5\epsilon_\theta$); histogram survival; evaluate mirror predictor ($v_{ghost}\approx v_{parent}+2v_{ego}\cos\psi$) precision/recall; jammer-flagged cells (PA-5 lineage) as exclusion covariate.
Pass: persistence+mirror score separates ghost/real with AUC ≥ 0.90 before touching the tracker.

**XP-3 · Timestamp-skew CDF (feeds IDF-3, backlog 3).**
Setup: multi-head logs; `sil_wrapper` timestamps + `sourceTxTime` + `TimingInfo` (LM2) + HIL `ptp_to_xml_generator` artifacts.
Run: per-log $\Delta t$ distribution per satellite; fusion-residual baseline; repeat on HLR-1606-whitelisted builds only (PA-4 gate) and tag RUN_MODE/Customer_Name (backlog 12 fields).
Pass: skew CDF + residual baseline published; IDF-3 must cut disagreement 30–60% on the same logs.

**XP-4 · Innovation-at-drop audit (feeds IDF-4, backlog 4).**
Setup: maneuver logs (lane-change, cut-in); XTRK tracker internals (`LOAD_TRACKER_INTERNALS=ENABLE_TRACKER_MODE_INIT` in `Emb_Lib_Config.xml` v7.5) + `kpi_business.py` gates + HLR-930 DataQualityChk verdicts (PA-8) as covariates.
Run: distribution of Mahalanobis $d^2$ at drop events vs held events; fit maneuver-conditioned gate curve $\gamma(m)$.
Pass: adaptive gate recovers 3–8 pp CAN recall on maneuver segments with <0.5 pp precision loss elsewhere.

**XP-5 · CDC saturation ranking (backlog 5).**
Setup: dense urban logs where `num_cdc_records==MAX_CDC_RECORDS` (5016).
Run: prototype RCS/SNR/near-field-priority pack in `cdc_packing.c` path (fast-resim AF branch for iteration speed, full pipeline for acceptance); compare saturation-recall delta + excluded-accuracy delta.
Pass: recall on saturated scans +5 pp with no regression on clean scans.

**XP-6 · Fast-resim parity (new, underpins all IDFs).**
Setup: same log set through `sil/rsp_sil` (full) vs `sil/fast_resim/` (CUW-5824 branch) AF paths.
Run: UDP Tier-1/Tier-2 agreement between paths; record disagreement as KPI-trust metric.
Pass: Tier-1 agreement ≥ 99.5% (else fast-resim is not a valid inner loop for that scenario class — quarantine those logs).

**XP-7 · LM2 conformance (backlog 9).**
Setup: one golden log per customer convertor (`BMW_{High,MID,Low}`, Scania, TML SRR5, Motional SRR3, STLA IFV600, DSPACE common).
Run: candidate LM2 FMU vs last-released `Deliverables/*.fmu`; MDF4 byte-diff; checksum gate on `Deliverables/`.
Pass: zero unexplained divergences; every divergence maps to a changelog entry + `meta_data.json` provenance bump.

## C.3 Branch-validation matrix (which build to run each study on)

| Study | Primary build (snapshot) | Control build | Why this pair |
|---|---|---|---|
| XP-1, IDF-1 A/B | G7 `dev` + prototype patch | AWR `feature__variable_cfar_initial_changes` (GHW-94 LUT) | In-house static baseline |
| XP-2, IDF-2 A/B | G8 `dev` | G8 `dev` + ghost down-weight | Fast-resim inner loop (PA-6), full-pipeline acceptance |
| XP-3, IDF-3 A/B | G8 `story__HLR-1436-fusion` (post scan-index fix) | Same + extrapolation | PA-4 validity gate |
| XP-4, IDF-4 A/B | G7 `feature__EmbLibv11.0.x` or G8 `feature__EmbLibv7.0.x` | Same + adaptive gate, `LOAD_TRACKER_INTERNALS` on | Live EmLib lines, internals enabled |
| XP-5 | G7 `feature__CDC_SIL` lineage (`cdc_packing.c`) | Same + priority pack | CDC integration branch owns the pack path |
| XP-6 | G8 `feature__CUW-5824-Fast-Resim_AF-Integration` | G8 `dev` full pipeline | Defines fast-resim trust boundary |
| XP-7 | LM2 `feature__adcam_main_dev` | LM2 `main` deliverable FMUs | Dev vs released packager |
| F13 gate check | Any G8 build | Whitelist incl. HLR-1606 + DNP-7666 SHAs | Backlog 10 |
| F14 check | G8 `feature__RSP_SIL` (DA off) | G8 `dev` (DA on) | EUR-2031 fingerprint |
| F15 check | Any LM2 branch FMU | `main` FMU | Provenance fields (backlog 12) |

## C.4 Corpus & compute plan (feeds HPCC decision)

Mining (§8 script + XP-1…XP-4) runs locally on the checked-in `h5py` datasets; corpus target before HPCC burst:
≥ 50 logs × 5 heads × 10 min ≈ 2.5k head-hours. If local iteration exceeds 48 h per XP, burst to `Core_RESIM_HPCC`
(still an empty placeholder — provisioning it is backlog item 13, new): single-image Apptainer job
(`ResimHTMLReport.def` pattern), read-only `_branch_snapshots/` mount, results back to `profile_timing_plotter/`
convention (`timeDiff.csv` + PNG). USS aggregator evaluation (LM2 `feature__USS_Aggregator` FMUs) reuses the
same harness with `Customer_Name=USS` once that convertor path is wired (0.5 FTE, B.5).

---

*Appendix C generated 2026-09-24. Protocols reference pinned snapshot folders; re-validate SHAs after any `git fetch` (risk R-6).*
