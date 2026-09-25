---
description: "Unit test coverage for Gen8 radar — run reports, fix thresholds, add tests"
---

# Coverage Skill — Gen8 Radar (Repo-Verified)

## Quick Commands

| Task | Command |
|------|---------|
| Full report | `bazelisk test //coverage:report` |
| ECUSync only | `bazelisk test //coverage:ecusync` |
| Versions only | `bazelisk test //coverage:versions` |
| SPBB only | `bazelisk test //coverage:spbb` |
| AFBB only | `bazelisk test //coverage:afbb` |
| DSP BBE | `bazelisk test //coverage:dsp_bbe` |

## Coverage Tool

- Tool: **gcovr** (always enabled: `build --@bazel_platform//quality/ut:coverage_mode=gcovr`)
- Windows temp: `test:windows --@bazel_platform//quality/ut:tmp_dir=C:/tmp`
- Rule: `coverage_report` from `@bazel_platform//quality/ut:coverage_report.bzl`

## Main Report Thresholds (`:report`)

| Metric | Minimum |
|--------|---------|
| Line | 90.0% |
| Branch | 85.0% |
| Decision | 90.0% |
| Function | 90.0% |

**Build fails if ANY threshold is not met.**

## Subreport Targets (from coverage/BUILD)

60+ subreports. Key ones:
- `:versions`, `:ecusync`, `:MCU_Safety`, `:appl_PLT_Diag`
- `:appl_PLT_FaultMgr`, `:Diagnostic_Library`, `:SWC_Appl_Modemanager`
- `:r52_dd_mmic`, `:r52_appl_xcp`, `:r52_Stream_Handler`
- `:afbb`, `:dabb`, `:rcbb`, `:sabb`, `:spbb`
- `:dsp_bbe`, `:ipc_dsp`, `:mpu`, `:rdd_proc_ifc`
- `:swc_plt_appl_communication`, `:radar_ctl`

## What's Excluded

### Compile-level excludes (never instrumented):
- GoogleTest, MinGW, GCC-glibc, BBE toolchain

### Source-level excludes (in report but not penalizing):
- `external/.*iND13400_autosar_sip/.*`
- `external/.*iND13400_sdk/.*`
- `.*/test/.*`, `.*/_test/.*`, `.*/mocks/.*`
- `software/r52/autosar/config/Appl/GenData/.*`
- `software/bbe32/f360_tracker/.*`
- `software/r52/startup/.*`
- `software/r52/main.c`
- All `*_Test.c` test wrapper files

## Adding Coverage for New Module

### 1. Create `coverage_report` target in `coverage/BUILD`:
```python
coverage_report(
    name = "my_module",
    compile_coverage_exclude = COMPILE_UT_EXCLUDES,
    exclude = UT_EXCLUDES + [
        # Additional excludes specific to this module
    ],
    min_line = "80",
    min_branch = "75",
    min_decision = "80",
    min_function = "85",
    tests = [
        "//software/r52/autosar/swc/MY_MODULE/test:unit_tests",
    ],
)
```

### 2. Add to `:report` subreports list:
```python
subreports = [
    # ... existing
    ":my_module",
],
```

### 3. Add test target to `:all_unit_tests`:
```python
test_suite(
    name = "all_unit_tests",
    tests = [
        # ... existing
        "//software/r52/autosar/swc/MY_MODULE/test:unit_tests",
    ],
)
```

## Fixing Coverage Threshold Failures

### Symptom: `Line coverage is 82.5%, minimum is 90.0%`

**Steps:**
1. Run the specific subreport to see uncovered lines:
   ```bash
   bazelisk test //coverage:my_module --test_output=all
   ```
2. Look at the HTML report in `bazel-testlogs/coverage/my_module/`
3. Add tests for uncovered branches/lines
4. If code is untestable (HW register writes), add to excludes with comment

### Common untestable patterns (legitimate excludes):
- Hardware register writes (MCAL init)
- AUTOSAR OS entry points (main, TASK macros)
- Startup/boot code
- Generated AUTOSAR config

## Test Structure Pattern (GoogleTest)

```
software/r52/autosar/swc/MY_MODULE/
├── Source/
│   └── my_module.c
├── Include/
│   └── my_module.h
└── test/
    ├── BUILD
    ├── test_my_module.cc
    └── mocks/
        └── mock_dependencies.h
```

### Test BUILD pattern:
```python
cc_test(
    name = "unit_tests",
    srcs = ["test_my_module.cc"],
    deps = [
        "//software/r52/autosar/swc/MY_MODULE:my_module_lib",
        "@com_google_googletest//:gtest_main",
    ],
)
```
