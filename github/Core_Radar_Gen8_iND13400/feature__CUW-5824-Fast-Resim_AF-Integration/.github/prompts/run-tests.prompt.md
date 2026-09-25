---
mode: agent
description: "Run unit tests for a specific module or all tests"
---

# Run Unit Tests

## Input
- Module name or path (e.g., "ECUSync", "IPC", "bbe32", or full path)
- OR "all" to run everything

## Quick Commands

### Run ALL tests
```bash
bazelisk test //:all_unit_tests --test_output=all
```

### Run BBE32 tests only
```bash
bazelisk test //:tests_bbe --test_output=all
```

### Run single module test
```bash
bazelisk test //software/r52/autosar/swc/PLT_SWC/SWC_PLT_CDD_ECUSync/test:unit_tests --test_output=all
```

## Find the Right Test Target

### By SWC name:
`//software/r52/autosar/swc/PLT_SWC/SWC_PLT_<name>/test:unit_tests`

### By BBE32 module:
`//software/bbe32/<module>/test:unit_tests`

### By common module:
`//software/common/<module>/test:unit_tests`

### External dependency tests:
`@Calibration_Handler//calib_handler/test:unit_tests`
`@spbb//:spbb_tests`

## Test Output Options

| Flag | Effect |
|------|--------|
| `--test_output=all` | Show all test output (pass + fail) |
| `--test_output=errors` | Show only failures |
| `--test_output=summary` | Just pass/fail counts |
| `--test_filter=TestSuite.TestName` | Run specific test case |
| `--runs_per_test=5` | Run N times (flakiness check) |
| `--cache_test_results=no` | Force re-run (bypass cache) |

## Fix Common Test Failures

| Symptom | Fix |
|---------|-----|
| `TIMEOUT` | Add `size = "medium"` or `timeout = "moderate"` to BUILD |
| Linker error | Missing dep in test BUILD target |
| Header not found | Add header's library to `deps` |
| Mock mismatch | Update mock to match new function signature |
| Segfault | Check NULL pointer in test setup/teardown |

## Test with Coverage

```bash
# Full coverage report
bazelisk test //coverage:report

# Module-specific coverage
bazelisk test //coverage:ecusync
bazelisk test //coverage:dsp_bbe
```

## Workflow
1. Identify module → find test target
2. Run with `--test_output=all`
3. If fail → read error, fix source or test
4. Re-run until green
5. Run `//:all_unit_tests` before commit
