# ADVRADAR AWR294X Gen 7 Radar Project - Copilot Instructions

## Project Overview
This is Aptiv's Gen 7 Advanced Radar project for AWR294X chipset. It's a large-scale embedded automotive radar system (~4M SLOC) with multi-core architecture (ARM R5F MSS + C66x DSS) built using Bazel 7.1.0. The project targets three radar variants: SRR7P (Short Range), SRR7HD (High Definition), and FLR7 (Front Long Range).

**Key Languages/Frameworks**: C (embedded), C++ (tests), Python 3.10+ (tooling), Bazel (build), AUTOSAR (architecture)
**Target Platform**: Texas Instruments AWR294x (dual-core radar SoC)
**Repository Size**: Large (~4M lines), with extensive external dependencies via JFrog Artifactory

---

## Critical First Steps - ALWAYS DO THESE

### 1. Environment Setup (MANDATORY)
**Always run `repo_init.py` first** - this is required before any build/test operations:
```bash
python repo_init.py
```
This script:
- Verifies Python 3.7+ (recommend 3.10+)
- Installs pre-commit hooks (formatter, linter, buildifier)
- Configures .netrc credentials for JFrog Artifactory access
- Must be run once per machine, and again if credentials change

**If repo_init.py fails**, builds will fail with dependency download errors from JFrog.

### 2. Bazel Version (CRITICAL)
- Bazel **7.1.0** is required (specified in `.bazeliskrc`)
- Use `bazelisk` command (NOT `bazel` directly) - it auto-downloads correct version
- If bazelisk not installed: https://github.com/bazelbuild/bazelisk/releases

---

## Build System

### Primary Build Commands
**All commands run from repository root** (NOT from `software/build/`):

```bash
# Build specific variant (REQUIRED --variant flag)
bazelisk build //:gen7 --variant=srr7p     # Short Range Radar
bazelisk build //:gen7 --variant=srr7hd    # High Definition
bazelisk build //:gen7 --variant=flr7      # Front Long Range

# Convenience scripts (optional)
cd software/build && ./build.sh srr7p      # Linux
cd software/build && build.bat srr7p       # Windows
```

**Build outputs location**: `outputs/<variant>/` (e.g., `outputs/srr7p/`)

### Incremental Builds
Bazel is excellent at incremental builds - **do NOT run `bazelisk clean` unless absolutely necessary**. Clean builds take significantly longer and are rarely needed. Only clean if experiencing unexplained build failures.

### Conditional Build Flags (Important)
Common flags that modify build behavior:

```bash
--board=[A1|A2|B3|EDU|A1_Gbps|A2_Gbps|B3_Gbps|EDU_Gbps]  # Default: A1
--micro_revision=[ES1|ES2]           # Chip revision, default: ES2
--enable_cdc / --disable_cdc         # Compressed Data Cube
--enable_tdc / --disable_tdc         # Tailored Data Cube
--enable_rtm                          # Runtime Measurement
--multicast                           # Multicast logging
--veh_com=[can|someip]               # Vehicle communication, default: someip
--rot=[standalone|master|slave|none|sdv]  # Tracker configuration
--jenkins                             # Jenkins CI mode (don't use locally)
```

See `.bazelrc` for full flag list and aliases (e.g., `--variant` is alias for `--@appl_inclusion_dep//:variant`)

---

## Testing

### Unit Tests
```bash
# MSS (Microcontroller Subsystem) tests
bazelisk test //:all_unit_tests_mss --test_output=all

# DSS (Digital Signal Processing) tests - REQUIRES --config=dss_ut
bazelisk test //:all_unit_tests_dss --config=dss_ut --test_output=all

# Single test target
bazelisk test //software/app/common/test:version_unit_test --test_output=all
```

**Important**: DSS tests require the `--config=dss_ut` flag to configure C6X simulator correctly.

### Coverage Reports
```bash
bazelisk run //:coverage_mss                    # MSS coverage
bazelisk run //:coverage_dss --config=dss_ut    # DSS coverage
```
Combined coverage reports are only generated in Jenkins CI.

### Test Framework
- **Google Test** for unit tests
- **FFF (Fake Function Framework)** for mocking (header: `fff.h`)
- Tests located in `test/` subdirectories next to source

---

## Pre-Commit Validation Pipeline

### Automated Checks (Run on Every Commit)
When you commit, pre-commit hooks automatically run:
1. **clang-format** (v13.0.1) - C/C++ code formatting
2. **buildifier** - Bazel BUILD file formatting
3. **black** - Python formatting
4. **flake8** - Python linting
5. **File checks** - EOF, trailing whitespace, merge conflicts

**If pre-commit fails**:
```bash
# Run hooks manually to see failures
python -m pre_commit run --from-ref HEAD~1 --to-ref HEAD

# Auto-fix most issues
python -m pre_commit run --all-files

# Then commit fixed files
```

### Jenkins CI Pipeline (Gerrit Integration)
Code must pass Jenkins "Verified" job before merge:
- **Formatter Check**: Ensures pre-commit hooks were run
- **Build Check**: All 3 variants (srr7p, srr7hd, flr7) must build
- **Unit Tests**: Both MSS and DSS test suites must pass
- **Coverity**: Static analysis (separate job)

**Key Jenkins Jobs**: verifiedJob, buildJob, unitTestJob, formatterJob, coverityJob

If Jenkins build fails with formatter errors, you forgot to run `repo_init.py` or pre-commit hooks.

---

## Project Structure

### Key Directories
```
ADVRADAR_AWR294X/
├── BUILD                    # Root build file - defines //:gen7 target
├── WORKSPACE               # External dependencies (JFrog, TI SDK, building blocks)
├── .bazelrc                # Build configurations and flag aliases
├── .bazeliskrc             # Bazel version (7.1.0)
├── .pre-commit-config.yaml # Pre-commit hook configuration
├── repo_init.py            # MANDATORY setup script
├── bb.bzl                  # Building blocks loading
├── software/
│   ├── app/
│   │   ├── mss/            # Microcontroller Subsystem (ARM R5F)
│   │   │   ├── autosar/    # AUTOSAR stack (EB tresos)
│   │   │   ├── mcal/       # Microcontroller Abstraction Layer
│   │   │   ├── mmic/       # MMIC (radar frontend) driver
│   │   │   └── src/        # MSS application (aptivMss binary)
│   │   ├── dss/            # Digital Signal Processing (TI C66x)
│   │   │   └── src/        # DSS application (aptivDss binary)
│   │   ├── rss/            # Radar Subsystem (metarprc binary)
│   │   ├── common/         # Shared code (IPC, calibrations, versions)
│   │   └── building_block/ # Building block configurations
│   ├── boot/               # Bootloader (SBL)
│   └── FBL/                # Flashable Bootloader (DOIP/UDS)
├── tools/
│   ├── Jenkins/            # CI/CD pipeline definitions
│   ├── python/             # Build/test/analysis scripts
│   ├── bazel/              # Custom Bazel rules
│   ├── preCommit/          # Pre-commit configs
│   └── quickflash/         # Flashing utilities
├── instrumentation/
│   ├── Lauterbach/         # Trace32 debug scripts
│   └── CANape/             # Calibration tool setup
├── toolchains/             # TI compiler toolchains
└── documentation/          # Architecture, user guides, test procedures
```

### Build Outputs (After Build)
```
outputs/
├── srr7p/                  # SRR7P variant outputs
│   ├── aptivApp.hex        # Main application image
│   ├── aptivMss.out        # MSS ELF
│   ├── aptivDss.out        # DSS ELF
│   ├── smc_cal.bin         # System Master Calibration
│   ├── usc_cal.bin         # Unit Specific Calibration
│   ├── uniflash/           # Flashing tools
│   └── quickflash/         # Quick flash configs
├── srr7hd/                 # SRR7HD outputs
└── flr7/                   # FLR7 outputs
outputs/boot/               # Bootloader outputs (variant-independent)
    ├── aptivBootloader.hex
    ├── FBLImage.hex
    └── hsm.hex
```

---

## Common Workflows

### Making Code Changes
1. **Before starting**: `python repo_init.py` (if first time or credentials changed)
2. Make changes to source files
3. Build affected variant: `bazelisk build //:gen7 --variant=<variant>`
4. Run relevant tests: `bazelisk test <test_target> --test_output=all`
5. Commit (pre-commit hooks run automatically)
6. If pre-commit fails, fix and re-commit
7. Push to Gerrit - Jenkins runs full validation

### Adding New Source Files
- Create file in appropriate directory
- Add to BUILD file's `srcs` or `hdrs` list
- No need to update WORKSPACE unless adding external dependency
- Bazel automatically tracks dependencies

### Build Troubleshooting
**Clean Build** (last resort):
```bash
bazelisk clean
bazelisk build //:gen7 --variant=<variant>
```

**Dependency Issues**: Check JFrog access - re-run `repo_init.py`

**Toolchain Issues**: Verify `.bazeliskrc` has `USE_BAZEL_VERSION=7.1.0`

**Test Failures**: For DSS tests, always use `--config=dss_ut`

---

## Important Constraints and Gotchas

1. **AUTOSAR Files**: Don't manually edit files in `software/app/mss/autosar/config/Appl/GenData/` or `software/app/mss/mcal/Workspace/` - these are auto-generated by EB tresos tool.

2. **Calibration Files**: `software/app/common/calibrations/smc/` and `usc/` contain auto-generated calibrations from external tools - don't manually modify.

3. **TI SDK Files**: Anything under `ti/`, `drivers/soc/`, or marked as TI source in pre-commit config is vendor code - avoid modifications.

4. **Building Block Dependencies**: The project uses external building blocks (signal processing, angle finding, radar capability) loaded from JFrog in `WORKSPACE`. To develop building blocks locally, comment out `http_archive` and use `local_repository` instead.

5. **Variant-Specific Code**: Use Bazel `select()` statements to conditionally include code per variant. See BUILD file examples with `@appl_inclusion_dep//:srr7p` conditions.

6. **Versioning**: Software version info auto-generated in `software/app/common/versions/versions.c` via Git stamping during build.

7. **Memory Constraints**: DSS has strict memory limits. Check `flash_memory_stats.txt` after build. MSS/DSS linker scripts in `software/app/<mss|dss>/sys_config/`.

---

## Quick Reference

**Build**: `bazelisk build //:gen7 --variant=<srr7p|srr7hd|flr7>`
**Test MSS**: `bazelisk test //:all_unit_tests_mss --test_output=all`
**Test DSS**: `bazelisk test //:all_unit_tests_dss --config=dss_ut --test_output=all`
**Coverage**: `bazelisk run //:coverage_<mss|dss> [--config=dss_ut]`
**Format Check**: `python -m pre_commit run --all-files`
**Clean**: `bazelisk clean` (rarely needed)
**Setup**: `python repo_init.py` (must run first!)

**Documentation**: See `documentation/bazel/*.rst` for detailed Bazel guide, `README.md` for project overview.

---

## Trust These Instructions
This document was created through comprehensive repository analysis. **Trust this information and only search/explore if details are missing or found to be incorrect.** This will minimize exploration time and reduce build failures.
