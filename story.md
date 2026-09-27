# Company-Network SiL, KPI and Prototype Verification Story

**Date:** 2026-09-27 (workspace clock)
**Branch:** `research/adas-radar-20260926`
**Starting research HEAD:** `53c5495`
**Report purpose:** detail every command-path, build, replay, conversion, KPI, regression, failure and remaining blocker from the request to validate APT_SRR_RESIM and the accumulated radar research.
**Data rule:** raw MF4/HDF and vendor source remain read-only; replay/build/probe artifacts are in ignored `resim_research/sil_runtime/`, `resim_research/sil_build/`, and `resim_research/kpi_work/`.

## Executive summary

The company-network session did not need to download APT_SRR_RESIM: the repository’s ignored/local vendor corpus already contains Gen7 and Gen8 engine bundles. I identified the matching Gen7 engine, copied it into a sandbox, built its missing SRR DC DLL from source using installed Visual Studio 2022/MSVC, fixed a CMake working-directory defect **only in the sandbox copy**, and successfully replayed four real vehicle MF4 segments continuously. The engine emitted four real ORCAS MF4 outputs.

The output MF4s are raw `MF4Frame`/Ethernet-SOMEIP captures (one group, 22 channels each), not decoded radar detection streams. The bundled MDF decoder was exercised on both no-op and conversion branches: the first exits 0 without artifacts; the CANoe conversion branch prints `MDF library load failed` and still exits 0, leaving a zero-byte CSV. Therefore this run has **successful SiL replay and output-MF4 generation, but no decoded UDP-HDF KPI result for those new ORCAS outputs**.

Independent production KPI tools were run on existing HDF pairs in the correct vehicle/reference-input → resim-output order. Two real CAN-HDF pairs scored **95.56%** and **97.13%** overall average. UDP HDF: one dummy pair scored **100.00% (50/50)**; two real UDP fixtures parse as empty with this HDF KPI schema. One CAN IFV7XX pair is also an empty report due to HDF flavor/sensor-name mismatch. All **18/18** algorithm prototype demos passed in the final regression script.

## 1. Starting state and safe boundaries

Before the run, branch HEAD was `53c5495`, tracking `origin/research/adas-radar-20260926`. The working tree already contained unrelated pre-existing user modifications/deletions (README/research plan and `simg_zmq/*`); they were preserved and not staged.

The local environment had:

| Component | Observed |
|---|---|
| Python | 3.13.4 |
| numpy / scipy / h5py / pandas / asammdf | imports available; asammdf 8.7.2 |
| Visual Studio | VS 2022 Community 17.14.7, MSVC 19.44.35211, x64 compiler/linker available after `vcvarsall.bat x64` |
| CMake | available; VS 2022 generator used |
| JFrog | unauthenticated `curl -I` returned **401 Unauthorized**; no `JFROG_ACCESS_TOKEN`, `JFROG_USER`, or `JFROG_PASSWORD` in environment |
| Local engine package | Gen7 and Gen8 `APT_SRR_RESIM.exe` and extensive DLL bundles already existed under ignored `Research/Core_RESIM_DC_Emb_Library/DC_SIL/sil_executables/` |

Artifactory credential bypass was not attempted. Since the engine binaries were already available locally, the tests used those copies.

### Original request items checked

The original autoresearch objective included: real MF4/HDF ingestion; SiL replay and output MF4 generation; exact UDP Tier-1/Tier-2 and CAN KPI matching; five perception tracks; Doppler-RadarSplat; four synthetic scenarios; dynamic alignment state injection/fast-lock; parallel prefix KF; HPCC halo orchestration; full manuscript/IDFs; execution log; tests and push. The pre-run ledger already documented 21 runs, the N18 chain score 80, prototypes, a draft paper/IDFs, presentation, HPCC templates and local HDF KPI bridges. This story adds the previously missing **actual APT_SRR_RESIM execution** and the precise decode/KPI boundary discovered here.

## 2. Finding and identifying the actual APT_SRR_RESIM

The Gen7 Windows runtime was found at:

`Research/Core_RESIM_DC_Emb_Library/DC_SIL/sil_executables/fw_dlls/APT_SRR_RESIM.exe`

Observed binary facts:

| Fact | Value |
|---|---|
| Size | 796,160 bytes |
| PE architecture | x64 |
| Engine banner | `APT_SRR_RESIM_26_24_108_32` |
| SHA256 | `312ae84ed260938a3f25d31c516b5f38135f9dc366c3def50819722bee1743ff` |
| Adjacent Gen7 runtime bundle | 55 files, 580.7 MiB |
| Gen8 engine | Also present (`fw_dlls_gen8/APT_SRR_RESIM.exe`, 742,912 bytes); not selected for this validation |

Run from its directory with `--help` (and with bundle directory prepended to `PATH`) to verify the supported CLI:

```powershell
APT_SRR_RESIM.exe -c <SIL_Engine_Config.xml> -p <SIL_Input.txt> -o <output-dir>
```

It also supports positional `config.xml input-list.txt output-dir`. The run used the explicit `-c/-p/-o` form.

## 3. Isolated source build of the missing Gen7 SRR DC DLL

The engine initially failed with `Failed to load lib: SRR7_SiL_RL.dll`. Those four sensor DLLs were found in the sibling `sil_executables/radar_dlls/` directory and copied to the sandbox. The next attempt correctly advanced and reported `Unable to load Radar ECUPlugin: SRR_DC_SIL_LIB.dll`; that library was absent from the runtime bundle but source and dependencies were checked in.

To preserve the vendor source, `DC_SIL/` and `Application/` were copied to ignored `resim_research/sil_build/src/`. The vendor `update.py` mutates F360 tracker files and assumes its working directory is `build`; the initial CMake configure ran it from the source folder and failed with `FileNotFoundError`. I patched **only the sandbox copy** of `CMakeLists.txt` to pass `WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}` to `execute_process`.

Exact configure/build:

```powershell
cmake -S resim_research/sil_build/src/DC_SIL `
  -B resim_research/sil_build/src/DC_SIL/build `
  -G "Visual Studio 17 2022" -A x64 -DBUILD_MODE=srr_dc

cmake --build resim_research/sil_build/src/DC_SIL/build `
  --config Release --target SRR_DC_SIL_LIB --parallel 8
```

| Build check | Result |
|---|---|
| CMake configure | PASS after sandbox-only CWD fix |
| SRR DC DLL | `SRR_DC_SIL_LIB.dll`, 20,925,440 bytes |
| DLL SHA256 | `f1dc6cf92c6507b2caee5ae6aea2dd61cc71404f917634741c22a90dc1c2d0d9` |
| Incremental rebuild | exit 0 |
| Errors | 0 |
| Warnings | float64→float32 conversion warnings in tracker wrapper, `DGPS_TARGET_COUNT` redefinition, MSVC inline/deprecation warnings |

The built library, DGPS runtime DLLs, and version file were copied into the **ignored Gen7 runtime sandbox**, not back into `Research/`.

## 4. Runtime config, first failures, and debugging

The original Gen7 config was unsuitable as-is:

- `RECU_CONFIG_00/01` pointed to stale absolute `C:\Git\DGPS\...` paths.
- DC `DGPS_SM_Config` pointed to a Linux developer home directory.
- Video generation was enabled despite video being irrelevant to this test.
- Vendor DLLs were not all colocated with the engine in the checked-in source tree.

`resim_research/sil_runner.py prepare <b05.mf4>` creates a private runtime under ignored `resim_research/sil_runtime/gen7/`, copies the runtime/DGPS config, rewrites paths in copied XML only, disables video, copies input into the sandbox, and prepares `SIL_Input.txt`. `run [timeout_seconds]` invokes the engine and captures the full console log.

Initialization failures were debugged in sequence:

1. `SRR7_SiL_RL.dll` load fail → located `radar_dlls/SRR7_SiL_{FL,FR,RL,RR}.dll`; copied to runtime.
2. `SRR_DC_SIL_LIB.dll` load fail → built Release DLL offline from sandbox C++ sources and copied runtime dependencies.
3. With the complete runtime, engine initialized RL/RR/FR/FL and SRR ECU and executed data.

## 5. Real vehicle MF4 SiL replay

The four inputs staged in the isolated runtime were:

| Input | Size | MDF frames reported by asammdf |
|---|---:|---:|
| CCA DEBUG `165612_0000.MF4` | 294.3 MiB | 406,644 |
| CCA DEBUG `165627_0001.MF4` | 303.7 MiB | 419,584 |
| CCA DEBUG `165642_0002.MF4` | 306.8 MiB | 423,844 |
| CEER S12 `151836_0003_b05.MF4` | 101.6 MiB | 140,224 |

The CEER b05 is a real vehicle input from the available sample set; the CCA logs are local HIL MF4s. The three CCA segments were ordered continuously and run alongside the CEER input in the same engine invocation.

Command:

```powershell
python resim_research/sil_runner.py run 1800
```

Run log: `resim_research/sil_runtime/gen7/run_20260927_182400.log`.

| Engine measure | Result |
|---|---:|
| Process return code | 0 |
| Engine version | 26.24.108.32 |
| Logs started/completed | 4 / 4 |
| Engine runtime | 143 seconds |
| `Resimulation Operation Completed...` | 1 |
| `[ERROR]` / `Completed with errors` | 0 |
| `HeaderChecksumMismatch` text markers | 6 (FL/RR events; warning retained) |
| Bad scan indexes | 0 on RL/RR/FR/FL for each log |

Latched scan counts from engine log:

| Input | RL | RR | FR | FL | FC |
|---|---:|---:|---:|---:|---:|
| CCA `165612_0000` | 290 | 289 | 289 | 289 | 0 |
| CCA `165627_0001` | 300 | 300 | 300 | 300 | 0 |
| CCA `165642_0002` | 300 | 300 | 300 | 300 | 0 |
| CEER `151836_0003_b05` | 79 | 80 | 79 | 80 | 0 |

The 6 header-checksum markers require log-owner/decoder investigation. They did not create bad scan-index counts in this run; that is not evidence that checksum warnings are harmless in general.

Per-log latched scans: CCA `165612_0000` RL/RR/FR/FL = 290/289/289/289; CCA `165627_0001` = 300/300/300/300; CCA `165642_0002` = 300/300/300/300; CEER = 79/80/79/80. Each has zero bad scan indexes for the four active radars. Wall time is 143 seconds across these four segments; this is not presented as a production throughput benchmark.

Output MF4s in `resim_research/sil_runtime/gen7/output/ORCAS/`:

| Output | Size | asammdf structure |
|---|---:|---|
| CCA `165612_0000_r00100010.mf4` | 165.1 MiB | 1 group `MF4Frame`, 22 channels, 180,185 frames |
| CCA `165627_0001_r00100010.mf4` | 172.9 MiB | 1 group, 22 channels, 188,399 frames |
| CCA `165642_0002_r00100010.mf4` | 172.9 MiB | 1 group, 22 channels, 188,399 frames |
| CEER `151836_0003_b05_r00100010.mf4` | 45.0 MiB | 1 group, 22 channels, 48,983 frames |

Channels are ORCAS `MF4Frame.*` Ethernet/SOMEIP fields and `TimeStamp`. **These are genuine SiL candidate MF4 outputs, but they are not decoded radar-detection HDF files.**

## 6. Attempt to decode the real SiL output

The bundled Windows decoder is `Research/Core_RESIM_HIL_Engine/ApplicationProjects/MDF4_Decoder/output/Release/mdf_udpData_Proc.exe`, version 21.29.03.03. It was copied to ignored `sil_runtime/decoder/`; it accepts XML master + JSON `{reprocessingInputFileStreams: ...}`.

Source review (`mdf_udp_Data_Proc.cpp`) found the router only recognizes exact keys `BN_CALIFR`, `BN_FASETH`/`BN_IUKETH`, `SRR_DEBUG`, and `SRR_REFERENCE`. Findings:

| Probe | Config/route | Process exit | Output | Interpretation |
|---|---|---:|---|---|
| Initial probe | `DEBG`, no conversions | 0 | none | Wrong route key/no-op; exit 0 alone is not a success signal |
| Supported key | `SRR_DEBUG`, no conversions | 0 | none | Router accepted; conversion flags disabled |
| Conversion branch | `SRR_DEBUG`, CANoe output enabled | 0 | 0-byte `.mf4.csv` sidecar, no decoded MF4/HDF | Log: `[ERROR]: MDF library load failed...` and `[ERR]:Conversion failed...` — decoder conversion failed despite exit 0 |

Reproduce probes with `python resim_research/decoder_probe.py`; configs, per-probe logs, and `summary.json` are under ignored `resim_research/sil_runtime/decoder/probes/`. The executable’s source config requires a proprietary MDF CCA conversion library; available DLLs do not satisfy that runtime load, and the source decoder has no HDF-Report extraction branch. There is **no bundled `MUDP_DATA_Extracter` executable** in checked-out files. I did not alter vendor files or attempt to evade auth.

## 7. Production HDF KPI validation (correct order)

Used `resim_research/validate_hdf_pairs.py`, which calls UDP HDF `parse_for_kpi` and CAN `can_kpi_hdf/kpi_main.py`. CAN pair order is vehicle/reference input → resim output. This corrects the earlier reversed CAN invocation.

### UDP HDF

| Pair | Result |
|---|---|
| `kpi_dummy_pair1_input` → `kpi_dummy_pair1_output` | **50/50 matched, 100.00%** |
| Real CCA HDF fixture 1 | streams `OD data not found` / no detection stream; no meaningful accuracy |
| Real CCA HDF fixture 2 | same empty-stream limitation; no meaningful accuracy |

### CAN HDF

| Pair | Aligned scans by radar | Overall sensor average | Sensor accuracy FL / FLR / FR / RL / RR |
|---|---|---:|---|
| CCA `120641_0000` | 288 / 282 / 288 / 288 / 288 | **95.56%** | 93.43 / 100 / 86.09 / 87.77 / 91.83 |
| CCA `120711_0002` | 298 / 292 / 299 / 299 / 299 | **97.13%** | 92.20 / 100 / 92.85 / 93.43 / 94.95 |
| IFV7XX `083635_0000` | no compatible matched sensor rows | **empty/0% report** | Bordnet/SiL HDF flavor and `CEER_ FL` vs `CEER_FL` naming mismatch |

Detailed production HTML/log artifacts: `resim_research/kpi_work/final_hdf_validation/`. These are existing paired HDF fixtures, not outputs from the newly replayed ORCAS MF4s.

## 8. Prototype regression

```powershell
python resim_research/run_regression.py
```

Result: **18/18 pass**, exit 0. Complete stdout/stderr: `resim_research/kpi_work/final_prototype_regression.log`. Reproduced claims include:

| Prototype | Current regression result |
|---|---|
| Adaptive gating | recall 0.863→0.996; RMSE 0.283→0.212 m |
| Async compensation | residual cut 86.2%→31.7% for yaw 0.1→0.6 rad/s |
| Bend-conditioned ghost fit | recall/precision 0.198/0.220→0.753/0.834 |
| Closed ghost chain | edge rec/prec 0→0.969/0.902; track RMSE 1.969→0.651 m; estimated geometry |
| Conformal maneuver gate | coverage 0.042→0.996; RMSE 17.683→0.263 m |
| Doppler decoy gate | precision 0.638→0.877; GRR 0.661 |
| Doppler-RadarSplat | R⁴ ratio 256; Jacobian relative error <1e-8; 6/6 recovery with clutter |
| Dual-loop CFAR | recall 0.50→1.00; FA 1→0 |
| Driving ghost alignment | error 13.0°→1.73°→0.955° at scans 5/30/60 |
| Fast DA | actual HDF az −1.145°, el −0.018°; gain lock 46 vs 400-frame cap |
| Ghost augmentation | clean-tuned FA 0.425→aug-tuned 0.033; recall 1.000 |
| Hybrid consensus | 0.965/0.862 at 18.2% of 3-DOF candidate evaluations |
| Parallel KF | relative associativity 6.35e-15; max mean error 1.51e-10 at 50k |
| Sector clutter | sparse-regime MSE 56.393→30.382 |
| Spectral covariance | spray RMSE 0.911→0.315 m; covariance min eigenvalue positive |
| Specular theorem | rank-1 error 1.066e-14; R_eff track RMSE 3.242→0.739 m |
| Synthetic generator | overpass/cutin/spray/curve HDF5 SENSOR1 schema assertions pass |
| Track-conditioned residual | range RMSE 0.439→0.106→0.108 INT8 m; 4.5× nominal multiply reduction |

## 9. Full research ledger and deliverables

The carried research history is in `autoresearch_research.jsonl`, `experiments/worklog.md`, `equations.md`, `storage.md`, `strategies.md`, and `autoresearch_research-dashboard.md`. State integrity check: 21 unique run records, sequential runs 1–21. Best research score is **80.0** (Run 14, closed ghost chain); Run 19 (driving alignment) measured 0.955° at scan 60 but was scored 67 and discarded as a modest extension. Full manuscript + four invention disclosures: `research.md`; presentation: `PRESENTATION_APRISM.html`; HPCC templates: `run_hpcc_burst.sh`, `slurm_resim.sbatch`.

This operational verification adds actual Gen7 SiL runs and correct-direction real CAN-HDF KPI scores. It does **not** change the best score or claim decoded HDF validation for raw ORCAS frames.

## 10. Files/commands/artifact locations

| Evidence | Location / command |
|---|---|
| SiL preparation | `python resim_research/sil_runner.py prepare <b05.mf4>` |
| SiL execution | `python resim_research/sil_runner.py run 1800` |
| Four-log SiL log | `resim_research/sil_runtime/gen7/run_20260927_182400.log` |
| Four raw ORCAS outputs | `resim_research/sil_runtime/gen7/output/ORCAS/` |
| Sandbox C++ source/build | `resim_research/sil_build/` (not vendor tree) |
| Decoder probes | `resim_research/sil_runtime/decoder/probes/` |
| Probe runner | `python resim_research/decoder_probe.py` |
| Correct-order KPI runner | `python resim_research/validate_hdf_pairs.py` |
| HDF KPI reports | `resim_research/kpi_work/final_hdf_validation/` |
| Full regression | `python resim_research/run_regression.py` |
| Regression log | `resim_research/kpi_work/final_prototype_regression.log` |

Generated MF4/HDF reports and build products are local ignored artifacts; only code, configs needed to reproduce, and this story are to be committed. No API token, credential, raw MF4, HDF, or vendor binary is added to Git.

## 11. Remaining work

1. Obtain the approved decoder/MDF conversion dependency or `MUDP_DATA_Extracter` release package; decode SiL ORCAS outputs into production UDP HDF and compute their Tier-1/Tier-2 KPIs.
2. Investigate the six `HeaderChecksumMismatch` events with the stream owner despite zero bad scan-index reports.
3. Validate CAN KPI on the correct SiL CAN-output HDF schema; the two existing CAN pair scores above are fixture baselines, not this run’s SiL output.
4. Run TinyML cycle/size/heap measurements on actual BBE32/R52 hardware; Python FLOP proxies do not prove the <50 µs/<50 KB constraints.
5. Obtain cluster allocation to run `run_hpcc_burst.sh` / Slurm templates; current local replay is a discrete-event simulator only.

These boundaries are recorded as blockers, not marked complete.
