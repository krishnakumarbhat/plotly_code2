# Gen8 iND13400 Radar Software - Copilot Instructions

## Repository Overview

This repository contains embedded radar software for the Gen8 iND13400 microcontroller, targeting automotive radar systems (FLR8 and SRR8P variants). The codebase is a large-scale (~100k+ lines), safety-critical embedded C/C++ project using **Bazel/Bazelisk** as the build system.

**Key Technologies:**
- **Languages:** C (embedded), C++ (tests), Python (tooling), Bazel/Starlark (build)
- **Build System:** Bazel 7.5.0 via Bazelisk wrapper (bzlmod enabled)
- **Toolchains:** WindRiver R52 compiler, Xtensa BBE32 compiler, MinGW (simulation), GCC (unit tests)
- **Testing:** GoogleTest framework for unit tests, Coverity for static analysis
- **Processors:** Dual-core architecture - R52 (ARM) and BBE32 (Xtensa DSP)

## Critical Setup - Run FIRST Every Time

**ALWAYS run `repo_init.py` after cloning or when your password changes:**
```bash
python repo_init.py
```
This script (Python 3.7+, 3.10 recommended):
1. Installs pre-commit hooks (REQUIRED for CI)
2. Creates/updates `.netrc` with build credentials
3. Installs Python dependencies

**Failure to run this will cause local build failures.**

## Build System

### Build Commands - Standard Workflow

**Default build (CAN variant):**
```bash
# FLR8 variant
bazelisk build //:gen8 --config=flr8

# SRR8P variant
bazelisk build //:gen8 --config=srr8p
```

**Other communication variants:**
```bash
# SomeIP (Ethernet, high detection count)
bazelisk build //:gen8 --config=flr8 --veh_com=someip

# Standalone (tracker enabled, no CAN/SomeIP)
bazelisk build //:gen8 --config=flr8 --veh_com=none --rot=standalone
```

**Clean build (when needed):**
```bash
bazelisk clean  # Use when switching variants or troubleshooting
```

### Build Flags & Conditional Builds

Common flags documented in `.bazelrc` (see README.md for full list):
- `--bbe_asic_fpga=[asic|fpga]` - FPGA vs ASIC configuration
- `--enable_adc_logging=True` - Enable ADC chirp logging for debugging
- `--enable_chirp_profiling=True` - Enable timing profiling
- `--enable_cdc=True` - Enable CDC logging
- `--enable_uart=True` - Enable MCAL UART module
- `--disable_stack_monitor_r52` - Disable R52 stack monitoring

**Build outputs:** Located in `bazel-bin/outputs/[variant]_[config]/`

### Known Build Issues & Workarounds

1. **Long path issues (Windows):** Create `user.bazelrc` in repo root:
   ```
   startup --output_base=C:/bzl
   test:windows --@bazel_platform//quality/ut:tmp_dir=C:/tmp
   ```

2. **Corrupt remote cache:** Add flag to bypass cache:
   ```bash
   bazelisk build //:gen8 --config=flr8 --noremote_accept_cached
   ```

3. **Bazel needs GCC on Linux:** PATH is hardcoded in `.bazelrc` to ensure hermetic builds:
   ```
   /bin:/usr/bin
   ```

## Testing & Validation

### Unit Tests - ALWAYS Run Before Commit

```bash
# Run ALL unit tests (recommended)
bazelisk test //:all_unit_tests --test_output=all

# Run BBE-specific tests
bazelisk test //:tests_bbe

# Run single test target
bazelisk test //software/r52/autosar/swc/SWC_PLT_CDD_ECUSync/test:unit_tests --test_output=all
```

### Coverage Reports

```bash
# Generate full coverage report
bazelisk test //coverage:report

# Individual module coverage
bazelisk test //coverage:versions
bazelisk test //coverage:ecusync
```
Coverage thresholds vary by module (see `coverage/BUILD`). Minimum: 20-25% (line/branch), up to 90%+ for critical modules.  Build fails if thresholds not met.

### Coverity Static Analysis - Must Pass for CI

**Build with Coverity (choose one):**
```bash
# R52 analysis
bazelisk build //software/r52:windriver_r52_cov --config=flr8

# BBE32 analysis
bazelisk build //software/bbe32:xtensa_bbe32_cov --config=flr8
```

**Analyze defects locally:**
```bash
bazelisk run //software/r52:commit_defects_flr8 -- --auth-key-file="$HOME/coverity.auth"
```
Generate auth key from [Coverity Connect](https://coverity.asux.aptiv.com/).

### Generate compile_commands.json (for IDE/TiCS)

```bash
bazel run //:compiledb
```

## CI/CD Pipeline - Gerrit Verification

**Pre-commit checks (automatic):**
Pre-commit hooks run on `git commit` and enforce:
- `clang-format` (C/C++)
- `buildifier` (Bazel)
- `black` (Python)
- `flake8` (Python linting)
- `mh_style` (MATLAB)
- No binary files, merge conflicts, trailing whitespace

**CI checks on Gerrit review (must pass to submit):**

1. **Verified** - Pre-commit formatting compliance
2. **Build** - Builds all variants (FLR8/SRR8P × CAN/SomeIP/Standalone)
3. **Coverity** - Static analysis (High/Medium + MISRA Mandatory/Required)
4. **Unit-Test** - All unit tests must pass and coverage thresholds met
5. **Smoke-Test** - Hardware integration tests

**Pipeline locations:**
- Main trigger: `tools/CI/WRSD/core-radar-gen8-ind13400-trigger-verification.yaml`
- WRSD pipelines: `tools/CI/WRSD/core-radar-gen8-ind13400-*.yaml`
- Pre-commit config: `.pre-commit-config.yaml`
- Jenkinsfiles have been deprecated in favor of WRSD.

**If CI fails:** WRSD writes comments to Gerrit with logs. Fix issues and upload new patchset.

## Project Architecture

### Directory Structure

```
├── software/               # Main source code
│   ├── r52/               # R52 (ARM) core sources
│   │   ├── autosar/       # AUTOSAR SWCs (main application logic)
│   │   │   ├── swc/       # Software components (Communication, Diag, FaultMgr, etc.)
│   │   │   └── config/    # AUTOSAR config (VECTOR generated, DO NOT format)
│   │   ├── drivers/       # MCAL drivers (ADC, GPT, I2C, etc.)
│   │   ├── bb_radar_ctl/  # Radar control building blocks
│   │   └── main.c         # R52 entry point
│   ├── bbe32/             # BBE32 (Xtensa DSP) core sources
│   │   ├── src/           # Main BBE DSP processing
│   │   ├── rdd_proc/      # Radar data detection processing
│   │   ├── static_alignment/  # Static alignment module
│   │   ├── dyn_alignment/     # Dynamic alignment module
│   │   ├── capability/    # Radar capability configuration
│   │   ├── bb_cfg/        # Building block configuration
│   │   └── main.c         # BBE32 entry point
│   └── common/            # Shared headers/streams between cores
├── sil/                   # Software-in-Loop (SIL) simulation framework
│   └── rsp_sil/          # RDD SIL wrapper and tests
├── tools/
│   ├── bazel/            # Bazel toolchain configs, scripts
│   ├── CI/               # Jenkins/WRSD pipeline definitions
│   ├── preCommit/        # Pre-commit hook configs (pyproject.toml, flake8)
│   ├── lauterbach/       # Trace32 debug scripts
│   ├── python/           # Python tooling (memory stats, stream gen, testing)
│   └── coverity/         # Coverity config and scripts
├── coverage/             # Coverage report targets
├── MODULE.bazel          # Bazel module dependencies (bzlmod)
├── BUILD                 # Root build targets
├── .bazelrc             # Bazel configuration flags
├── repo_init.py         # REQUIRED setup script
└── miss_hit.cfg         # MATLAB linter config
```

### Key Files & Configurations

- **MODULE.bazel** - External dependencies (googletest, rules_cc, toolchains, coverity)
- **BUILD** - Main build targets (`:gen8`, `:all_unit_tests`, `:compiledb`)
- **.bazelrc** - Build flags, toolchain setup, remote cache config
- **.pre-commit-config.yaml** - Pre-commit hooks (clang-format, buildifier, black, flake8)
- **software/module_filename_map.json** - Module-to-filename mapping for memory stats
- **miss_hit.cfg** - MATLAB style checker config

### External Dependencies (Managed via Bazel)

#### Major Modules
- SPBB (Signal Processing Building Blocks) - `@spbb`
- AFBB (Angle Finding Building Blocks) - `@afbb`
- SMC/USC calibration files - `@smc_flr8`, `@usc_flr8`, etc.
- Calibration Handler - `@Calibration_Handler`
- AUTOSAR SIP
  - `@iND13400_autosar_sip`
  - Git submodule at `software/r52/autosar/sip` when testing changes locally (legacy behavior, prefer --overrides in Bazel)

#### Other Dependencies
The majority of dependencies are managed via Bazel modules defined in `bb.MODULE.bazel`.

## Common Pitfalls & Trust These Instructions

1. **ALWAYS run `repo_init.py` first** - Missing pre-commit hooks = CI failure
2. **Clean between major changes** - Use `bazelisk clean` when switching variants, use `bazelisk clean --expunge` when modifying MODULE.bazel file or if issues persist
3. **Don't format AUTOSAR/vendor files** - Pre-commit excludes `software/r52/autosar/config/`, `drivers/mcal/modules/`, etc.
4. **Use correct variant flags** - `--config=flr8` or `--config=srr8p` (NOT both)
5. **Long paths on Windows** - Create `user.bazelrc` with shorter `output_base` and tmp dirs
6. **Unit tests must pass** - Run `//:all_unit_tests` before submitting to Gerrit
7. **Remote cache issues** - Use `--noremote_accept_cached` if builds fail mysteriously (last resort)

**Trust these instructions.** Only search/explore if:
- Instructions are incomplete for your specific task
- Commands fail with errors not documented here
- You need details about a specific module's internal implementation
