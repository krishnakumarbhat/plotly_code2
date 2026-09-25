# Gen8 iND13400 ITF
### Overview
This folder contains Integration tetsing framework which is used to validate the signal processing building blocks.

### What it does?
This framework simulates the ADC data from the corresponding testcases using Radar simulator and feeds into Executable spec, Matlab Executable spec and Embedded software and collects the data across all the Spec and compares.

There will be HTML report generated for the corresponding testcase.

### Requirements
Ensure that the Executable Specs and Mex Spec are cloned before running ITF
- For the time being, Ensure the PBL that is being flashed on the board is relevant to the board.
- Power cycle after flashing the board is required at the moment to start the ECU, There is a timeout window to perform a powercycle. Please comply.
- Ensure the build outputs are present for the time being.

### Triggering the Framework
Lite_ITF_data_collection.m is the main file that needs to be run through Matlab to perform Integration testing.
- Ensure the testcases are present in the file folder corresponding to the selected sensor type.

### Flags and their usage/importance
The table below summarizes the main control flags used by `RunTest_Integration.m` and why they matter.

| Flag | Default | Usage | Importance |
| --- | --- | --- | --- |
| `debug_mode` | `0` | Skips flashing/online ECU interaction when set to `1`. | High: enables offline debugging and protects hardware from unnecessary flashing. |
| `offline_mode_inject` | `2` | Selects the injection path: `0` = MARS ADC capture, `1` = radar datacube injection, `2` = ADC injection. | High: controls how input data reaches the embedded flow. |
| `offline_mode_flag` | derived | Maps `offline_mode_inject` to the ECU offline-mode command (`0x33333333`, `0x22222222`, `0x11111111`). | High: must stay in sync with `ipc_command.h`. |
| `save_ADC_data` | `1` | When `1`, generates ADC data from the radar simulator and saves it for reuse; when `0`, loads previously saved data. | High: decides whether the testcase is generated or replayed. |
| `mex_embed_comparison_mode` | `0` | Enables Mex-vs-Embed only validation and skips Exec Spec processing. | Medium/High: useful for focused comparison and faster triage. |
| `download_data_set` | `0` | In Mex-Embed mode, downloads the reference dataset instead of running full MEX processing. | Medium: supports dataset-based validation workflows. |
| `af_comparison` | `0` | Enables Angle Finding comparison path. | Medium: currently disabled by default until AF issues are resolved. |
| `rdd_comparison` | `1` | Enables Range/Doppler/CFAR/BW/First Pass/Second Pass comparison flow. | High: this is the main comparison path used in the script. |
| `run_single_look` | `1` | Runs only the selected look instead of all looks in the testcase. | Medium: reduces runtime during targeted debugging. |
| `selected_look` | `1` | Selects the look index to run when `run_single_look` is enabled. | Medium: targets the exact look under investigation. |
| `sitp_run` | `0` | Enables SITP-specific result collection and reporting. | Medium: used when the output must be formatted for SITP. |
| `integTesting` | `1` | Passed into `addpaths()` to configure the MEX executable-spec environment. | High: required for the MEX execution path. |
| `InjectRoadData` | `0` | Switches between live simulation data and road-data injection mode. | High: changes the source of input data completely. |
| `af_Testing` | `0` | Reserved flag to run only Angle Finding validation. | Low/Medium: present for specialized AF-only runs. |
| `BB_flag` | `0` | Enables building-block-specific testcase handling in report finalization. | Medium: affects report aggregation and testcase grouping. |
| `build_flag` | `0` | Forces a build check/build for the selected variant. | High: used when build outputs are missing or need regeneration. |
| `clean_build` | `0` | Requests a clean build when `build_flag` is enabled. | Medium: useful when build artifacts are stale or inconsistent. |

### Recommended usage
- Use `debug_mode = 1` for data collection or troubleshooting without flashing hardware.
- Keep `rdd_comparison = 1` for normal validation runs.
- Use `mex_embed_comparison_mode = 1` only when comparing Mex and Embed outputs directly.
- Set `run_single_look = 1` with `selected_look` to isolate a failing look quickly.
- Use `save_ADC_data = 0` only when valid saved testcase data already exists.
- Change `offline_mode_inject` only when the testcase requires a different input injection path.

### Reports
Reports will be generated for the corresponding testcase under test in the root folder.
