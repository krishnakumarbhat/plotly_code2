# Copilot Instructions for Core Radar Gen7 SAF85xx

## Repository Overview

**Project**: Gen7_V2 Radar Software Development
**Type**: Embedded automotive radar system for NXP SAF85xx SoC
**Size**: Large-scale (~500+ files across multiple cores)
**Languages**: C/C++ (primary), Python (tooling/automation), Bazel (build system)
**Architecture**: Multi-core system with M7, A53, and BBE32 DSP cores

This repository contains the complete software stack for Gen7 V2 automotive radar sensors, including:
- **Software cores**: M7 (main control), A53 (application processing), BBE32 (signal processing DSP)
- **Building blocks**: Signal Processing (SPBB), Angle Finding (AFBB), Interference Detection (IDBB), Radar Capability (RCBB), Dynamic Alignment (DABB), Static Alignment (SABB), HIL, Tracker (ROTBB)
- **Variants**: SRR7e, SRR7p, FLR7, FLR7v3 (satellite and standalone configurations)
- **SIL (Software-in-Loop)**: CDC and RDD simulation frameworks for testing

## Critical First Steps

**ALWAYS run `python repo_init.py` first** when working with this repository for the first time or after password changes. This script:
- Downloads/installs repo-specific tools
- Configures `.netrc` credentials for JFrog Artifactory and Gerrit (requires API keys, not passwords)
- Installs pre-commit hooks for code formatting and validation
- **Python requirement**: v3.7+ supported, v3.10+ recommended
- **Note**: On Linux systems with externally-managed Python environments, you may need to use a virtual environment or pipx

**Pre-commit checks are mandatory**. The Gerrit repository rejects submissions that:
- Break any build variant
- Don't match formatting standards (auto-formatted by pre-commit hooks)
- Introduce new Coverity Defects of severity "High" or "Medium"
- Break any unit tests or reduce code coverage below thresholds
- Break any Fuzz tests
- Break hardware functionality (as verified by hardware "smoke tests" in CI)

## Build System - Bazel/Bazelisk

**Build tool**: Bazelisk (wrapper ensuring consistent Bazel version 7.5.0)
**Invocation**: Use `./bazelisk` (Linux) or `bazelisk` (Windows) - never use `bazel` directly

### Core Build Commands

**Production builds** (specify variant, board, and micro revision):
```bash
# SRR7p satellite
bazelisk build //:gen7 --variant=srr7p --board=A1 --micro_revision=ES2

# FLR7 satellite
bazelisk build //:gen7 --variant=flr7 --board=A1 --micro_revision=ES2

# SRR7e satellite
bazelisk build //:gen7 --variant=srr7e --board=A1 --micro_revision=ES2

# FLR7V3 satellite
bazelisk build //:gen7 --variant=flr7v3 --board=A1 --micro_revision=ES2
```

**Output location**: `bazel-bin/outputs/<variant>/` contains:
- `.s19`/`.ptp` files (flash images)
- Memory statistics
- Stream definitions
- Calibration files (SMC/USC)
- Boot files (PBL, SBL, SFT, HSE_FW)

**Clean builds**: Rarely needed (Bazel is incremental), but if required: `bazelisk clean`

### Build Configuration Flags

**Common flags** (see `.bazelrc` for complete list and aliases):
- `--variant=[srr7e|flr7|srr7p|flr7v3]` - **REQUIRED**: Sensor variant
- `--board=[A1|...]` - Board revision (default: A1)
- `--micro_revision=[ES1|ES2]` - MCU stepping (default: ES2)
- `--tracker_variant=[platform_flr7_standalone|platform_srr7p_standalone|platform_srr7p_2_sensor_fusion|disabled]` - Tracker configuration (default: disabled for satellite)
- `--enable_cdc=[True|False]` - CDC processing (default: True)
- `--enable_cdc_crc=[True|False]` - CDC CRC validation (default: True)
- `--enable_nm=[True|False]` - CAN Network Management (default: True)
- `--enable_secoc=[True|False]` - Secure Onboard Communication (default: False)
- `--enable_vlan=[True|False]` - VLAN for EthIf (default: False)
- `--disable_rtm` - Disable Run Time Measurement (default: enabled)
- `--disable_int_wdg` - Disable internal watchdog (default: enabled)
- `--enable_stream_generation` - Generate stream files with RESIM support (default: False)
- `--jenkins` - Jenkins-specific compilation flags (CI only)

**Conditional debugging flags**:
- `--enable_id_testing=True` - Enable Interference Detection testing
- `--enable_bench_testing=True` - Enable alignment bench testing
- `--rc_fi=true` - Enable Radar Capability fault injection

**User-specific config**: Create `user.bazelrc` in repo root to set persistent flags (e.g., `common --override_repository=afbb=/path/to/local/bb`)

## Testing

### Unit Tests

**Run all tests**:
```bash
bazelisk test //:all_unit_tests
```

**Run specific core tests**:
```bash
bazelisk test //:tests_bbe --variant=srr7p
bazelisk test //:tests_mmic
```

**Run single test target**:
```bash
bazelisk test //software/common/versions/test:unit_tests
```

**Test configuration**: Tests run in debug mode (`--compilation_mode=dbg`) with optimizations disabled (`-O0`). Test output shows only errors by default (`--test_output=errors`).

### Dynamic Analysis (Linux only, requires WSL on Windows)

**Setup** (one-time):
```bash
pip install -U luci-cli==0.3.0+250117.b3acc92 keyrings.cryptfile
```

**Run sanitizers**:
```bash
# Address Sanitizer
./run_dynamic.sh asan srr7p

# Undefined Behavior Sanitizer
./run_dynamic.sh ubsan srr7p

# Valgrind memory checker
./run_dynamic.sh valgrind srr7p

# Run all variants
./run_dynamic.sh asan all
```

**Output**: Generates `<sanitizer>.log`, `<sanitizer>.html`, `<sanitizer>.csv` in repo root.

**Bypassing defects**:
- Temporary: Create JIRA ticket with labels `warnings_justification` and sanitizer name (`asan`/`ubsan`/`valgrind`)
- Permanent (false positives): Add entries to `asan.toml`, `ubsan.toml`, or `valgrind.toml` (requires code-owner approval)

**Important**: Do NOT close JIRA bypass tickets until fixes are fully merged to `/dev` branch.

### Coverage Reports

**Generate coverage** (Windows requires short paths in `user.bazelrc`):
```bazelrc
startup --output_base=C:/bzl
test:windows --@bazel_platform//quality/ut:tmp_dir=C:/tmp
```

```bash
bazelisk test //coverage:report --variant=srr7p
bazelisk test //coverage:bbe_report --variant=srr7p
```

### Fuzz Testing (Mayhem)

**Run all fuzz tests**:
```bash
bazelisk build :fuzz --config mayhem
```

**Run specific test**:
```bash
bazelisk build //software/common/app_chksum/test:dd_app_chksum_fuzz --config mayhem
```

**Regression tests** (requires prior baseline):
```bash
bazelisk build :fuzz --config mayhem --regression
```

**Output**: Tests upload to Mayhem server (default 30-minute runtime). Follow console Run URLs for status.

### Coverity Static Analysis

**Run Coverity per core**:
```bash
bazelisk build //software/m7:windriver_v7_cov --config srr7e
bazelisk build //software/a53:windriver_v7_cov --config srr7e
bazelisk build //software/bbe32:xtensa_bbe32_cov --config srr7e
```

**Analyze against server** (requires auth key from Coverity Connect):
```bash
bazelisk run //software/m7:commit_defects_srr7e -- --auth-key-file="path/to/coverity.auth"
```

## Project Structure

**Root directories**:
- `software/` - Core application code (M7, A53, BBE32, common, building blocks)
- `tools/` - CI pipelines, Python scripts, Lauterbach debug configs, flash tools
- `sil/` - Software-in-Loop simulation frameworks (RDD and CDC SIL with emb_lib)
- `sysdeps/` - System dependencies and architecture-specific code
- `coverage/` - Coverage report generation rules

**Software architecture**:
- `software/m7/` - M7 core (main control): MCAL, AUTOSAR SWCs, drivers (MMIC, PMIC, serializer), IPC
- `software/a53/` - A53 core (application): Tracker integration, alignment (static/dynamic), capability, interference detection, IPC
- `software/bbe32/` - BBE32 DSP core: Signal processing dispatcher, fault injection, SPT validation
- `software/common/` - Shared headers: Stream definitions, calibrations (SMC/USC), IPC, CRC, versioning
- `software/building_block/` - Algorithm implementations: Angle finding, HIL processing
- `software/radar_processing/` - Radar control building blocks

**Configuration files** (repo root):
- `MODULE.bazel` - External dependencies, toolchain registration
- `bb.MODULE.bazel` - Building block archives (SPBB, AFBB, IDBB, RCBB, DABB, SABB, ROTBB, HILBB) from JFrog
- `.bazelrc` - Build flags, flag aliases, platform configs, sanitizer/coverage/cache configs
- `BUILD` - Top-level targets (`:gen7`, unit test suites, memory stats, coverage, fuzz)
- `repo_init.py` - Repository initialization script
- `*.toml` - Dynamic analysis baseline/bypass configurations

**CI/CD**:
- `tools/CI/` - Jenkins pipelines (Obsolete): `buildJob`, `verifiedJob`, `dynamicAnalysisJob`, `coverityJob`, `ticsJob`, `Test_Job`, ITF, SQT, SIT jobs
- `tools/CI/WRSD/` - WRSD pipeline YAML configs for pre-commit checks, unit tests, Coverity, fuzz, HW tests
- `.pre-commit-config.yaml` - Pre-commit hooks: clang-format, buildifier, black, flake8, shfmt, YAML linters
- Pre-commit runs automatically on `git commit` and is re-validated by WRSD "Verified" job

**Key scripts**:
- `run_dynamic.sh` - Wrapper for running sanitizers and Valgrind
- `awa.sh` - Aptiv Warnings Analyzer integration with JIRA for bypasses

## Common Workflows

### Making Code Changes

1. **Always run tests** before committing: `bazelisk test //:all_unit_tests`
2. **Format code automatically**: Pre-commit hooks handle this on `git commit`
3. **Build all affected variants**: If changing core code, test multiple variants
4. **Check dynamic analysis locally** (Linux): Run at least one sanitizer before pushing

### Building for Different Variants

When changing variant-specific code, always test affected variants:
```bash
bazelisk build //:gen7 --variant=srr7p --board=A1 --micro_revision=ES2
bazelisk build //:gen7 --variant=flr7 --board=A1 --micro_revision=ES2
```

### Debugging with SIL

**CDC SIL** (signal processing simulation):
```bash
# SRR7P
bazelisk build //sil/rsp_sil/main/rsp_wrapper_interface/cdc_sil_interface/test:cdc_sil_test --variant=srr7p --@spbb//common:use_bbe_cstub_simulator=True --@afbb//module/_common:sil_config_enable=True

# Override calibrations (optional)
--override_repository=smc_srr7p="./sil/rsp_sil/cal_bin/srr7p/smc_srr7p"
--override_repository=usc_srr7p="./sil/rsp_sil/cal_bin/srr7p/usc_srr7p"

# Enable logging
--//sil/rsp_sil/main/sil_wrapper_interface:enable_logging=True
```

**RDD SIL** (radar detection simulation):
```bash
bazelisk build //sil/rsp_sil/main/rsp_wrapper_interface/rdd_sil_interface/test:rdd_sil_Test --variant=srr7p --@spbb//common:use_bbe_cstub_simulator=True --@afbb//module/_common:sil_config_enable=True --psp_sil_config=True
```

**SIL debugging**: Add `--copt="-g" --copt="-O0" -c dbg` for debug builds, configure VS Code with `launch.json` (see `Design_Doc_CDC_SIL.md` and `Design_Doc_RDD_SIL.md` for details).

### Updating External Dependencies

**JFrog archives** are used for building blocks. To update:
1. Find new archive URL and SHA256 in JFrog Artifactory
2. Update `bb.MODULE.bazel` `http_archive` entry with new `urls` and `sha256`
3. Test build with all variants

**Local development override**: Add to `user.bazelrc`:
```bazelrc
common --override_repository=<bb_name>=<local_path>
```

### Working with Git Submodules

This repo uses submodules (see `.gitmodules`). They are NOT auto-updated:
```bash
git submodule update --init --recursive  # Initial checkout
git submodule update --remote            # Update to latest
```

## Key Constraints and Gotchas

1. **Bazel is hermetic**: System PATH is restricted. All tools come from toolchains defined in `tools/bazel/toolchains/`. Never rely on system-installed compilers.

2. **Long path issues on Windows**: Bazel generates deep directory structures. Use short output base in `user.bazelrc` for coverage: `startup --output_base=C:/bzl`

3. **Calibration files (SMC/USC)** are variant-specific and version-controlled via JFrog archives. Don't mix calibrations between variants.

4. **Pre-commit failures block commits**: If hooks fail, fix issues (usually auto-formatted) and re-stage files before committing.

5. **WRSD builds fail without pre-commit**: The "Verified" job re-runs pre-commit checks. Ensure `repo_init.py` has been run.

6. **Dynamic analysis is Linux-only**: Use WSL on Windows. Requires `luci-cli` Python package installed.

7. **Incremental builds are reliable**: `bazelisk clean` is rarely necessary and slows development. Only clean if facing truly unexplainable build issues.

8. **Toolchain versions are fixed**: Wind River for M7/A53, Xtensa for BBE32, mingw64/gcc for host, clang for sanitizers. Defined in `tools/bazel/toolchains/`.

9. **JFrog credentials required**: `.netrc` file must be configured via `repo_init.py` for builds to access external dependencies.

10. **MCAL and AUTOSAR generated code**: Located in `software/m7/mcal/app/output/` and `software/m7/autosar/config/Appl/GenData/` - these are excluded from formatting checks.

## Trust These Instructions

These instructions have been validated against the actual repository structure, build system, and CI pipelines. Only search for additional information if:
- Instructions are incomplete for your specific task
- Instructions are found to be incorrect
- You need details on a specific algorithm or module not covered here

When in doubt, check the `README.md`, relevant design docs (`Design_Doc_CDC_SIL.md`, `Design_Doc_RDD_SIL.md`), or building block documentation in external repositories.
