# Mayhem Fuzz Tests for versions.c Library

This directory contains a comprehensive fuzz test harness for the versions.c library using the Mayhem fuzzing platform with LibFuzzer integration.

## Overview

The fuzz test exercises all major version retrieval functions in the versions.c library to discover potential security vulnerabilities, memory safety issues, and unexpected behavior under random inputs.

## Fuzz Test Target

### Comprehensive Versions Fuzz Test
- **Target**: `//software/common/versions/test:versions_comprehensive_fuzz`
- **Harness**: `versions_comprehensive_harness.cc`
- **Functions Under Test**:
  - `Get_Application_Version()`
  - `Get_Bootloader_Version()`
  - `Get_Calibration_Version()`
- **Focus**:
  - Application version structure integrity and configuration validation
  - Boot shared data access and volatile memory handling
  - Calibration version retrieval with mocked underlying functions
  - Cross-function consistency and interaction testing## Building and Running

### Build Fuzz Test
```bash
# Build the comprehensive fuzz test
bazel build --config mayhem //software/common/versions/test:versions_comprehensive_fuzz
```

### Run Harness Locally (LibFuzzer)
```bash
# Run the harness locally with LibFuzzer
bazel run --config mayhem //software/common/versions/test:versions_comprehensive_harness -- -runs=1000

# Run with specific test case
bazel run --config mayhem //software/common/versions/test:versions_comprehensive_harness -- /path/to/test_input
```

### Upload to Mayhem Platform
The `fuzz_test()` target automatically packages and uploads to the Mayhem platform when built with `--config mayhem`. Simply use `bazel build` (not `bazel run`) for the fuzz_test target.

## Coverage Reporting (LibFuzzer)

You can generate a coverage report for the fuzz harness using the custom rule `libfuzzer_coverage`.

### 1. Add the rule to your BUILD file

```starlark
load("//tools/mayhem:libfuzzer_coverage.bzl", "libfuzzer_coverage")

libfuzzer_coverage(
    name = "versions_comprehensive_coverage",
    fuzz_target = ":versions_comprehensive_harness_bin",  # Use _bin suffix for actual binary
    runs = 1000,  # optional, default is 100
    testonly = True,
)
```

### 2. Build the coverage target

```bash
bazel build --config mayhem //software/common/versions/test:versions_comprehensive_coverage
```

### 3. Outputs

After the build you will find:

```
bazel-bin/software/common/versions/test/versions_comprehensive_coverage.profraw
bazel-bin/software/common/versions/test/versions_comprehensive_coverage.profdata
bazel-bin/software/common/versions/test/versions_comprehensive_coverage_report.txt
bazel-bin/software/common/versions/test/versions_comprehensive_coverage_html/ (HTML files)
```

Open the HTML index file in a browser to visually inspect line-level coverage.

### Notes

* The `cc_fuzz_test` already includes `-fprofile-instr-generate` and `-fcoverage-mapping` in both `copts` and `linkopts`.
* Use `--config mayhem` to ensure the Clang/LibFuzzer toolchain is used for instrumentation.
* Provide a testsuite via the `testsuite` attribute if you want broader coverage with Mayhem-generated test cases.

## Test Strategy

The comprehensive harness tests all version retrieval functions using fuzzer-controlled inputs to maximize code coverage and discover edge cases.

### Application Version Testing
- Validates generation field (should be 7 for Gen7 SAF85xx)
- Tests version field ranges (0-255 for uint8_t fields)
- Exercises different configuration flags
- Verifies pointer consistency across calls

### Bootloader Version Testing
- Tests shared data interface (`Boot_Shared_Data`)
- Validates version field ranges
- Tests pointer consistency
- Verifies structure immutability across calls

### Calibration Version Testing
- Mocks the underlying calibration accessor functions:
  - `Get_SMC_Cal_Major()`, `Get_SMC_Cal_Minor()`, `Get_SMC_Cal_Patch()`, `Get_SMC_Cal_Platform()`
  - `Get_USC_Cal_Major()`, `Get_USC_Cal_Minor()`, `Get_USC_Cal_Patch()`, `Get_USC_Cal_Platform()`
- Exercises the actual `Get_Calibration_Version()` function with fuzz-controlled mock return values
- Validates correct data flow from mocked functions to version structure
- Verifies all calibration functions are called exactly once
- Tests pointer consistency across multiple calls

### Orchestrated Testing
- Tests all functions with varying input patterns
- Exercises different call combinations based on fuzz input
- Tests edge cases with extreme values (0x00, 0xFF)
- Validates cross-function consistency

## Expected Behavior

### Success Criteria
- All version functions should return valid non-NULL pointers
- Version structures should remain consistent across calls
- Functions should handle NULL input pointers gracefully
- No memory corruption or crashes under normal operation

### Potential Issues to Discover
- Buffer overflows in version string handling
- NULL pointer dereferences
- Memory corruption in shared data structures
- Race conditions in multi-threaded environments
- Integer overflow in version calculations
- Uninitialized memory access

## Integration with CI/CD

The fuzz test is designed to integrate with your existing CI/CD pipeline:

```bash
# Add to CI build verification
bazel test --config mayhem //software/common/versions/test:versions_comprehensive_harness --test_timeout=300

# Add to nightly fuzzing runs
bazel run --config mayhem //software/common/versions/test:versions_comprehensive_harness -- -max_total_time=3600
```

## Debugging

### Reproducing Issues
If the fuzz test discovers an issue, you can reproduce it:

```bash
# Save failing input to a file
echo "failing_input_hex" | xxd -r -p > failing_input.bin

# Reproduce the crash
bazel run --config mayhem //software/common/versions/test:versions_comprehensive_harness -- failing_input.bin
```

### Analyzing Coverage
To analyze code coverage, use the libfuzzer_coverage rule as described above, or:

```bash
bazel coverage --config mayhem //software/common/versions/test:versions_comprehensive_harness --instrument_test_targets
```

## Dependencies

- `@rules_fuzzing//fuzzing:cc_defs.bzl` - LibFuzzer integration
- `//tools/mayhem:mayhem_aptiv.bzl` - Mayhem platform integration
- `@fff` - Fake Function Framework for mocking calibration functions
- Clang compiler with fuzzing support (enabled via `--config mayhem`)

## Mocking Strategy

The harness uses FFF (Fake Function Framework) to mock the calibration accessor functions that are normally macros accessing global calibration data structures. This approach:

- Allows testing `Get_Calibration_Version()` with controlled inputs
- Exercises the actual version retrieval logic (not just API surface)
- Verifies correct data flow from underlying accessors to the version structure
- Enables fuzzer to explore different calibration version combinations
- Validates that all expected functions are called exactly once

The macros are undefined and replaced with mock functions before including `versions.c`, ensuring the real implementation calls our mocks.

## Notes

- The harness is designed for LibFuzzer/Clang, not GTest framework
- Tests require `--config mayhem` flag to enable proper fuzzing toolchain
- The harness includes proper mock setups for external dependencies (MMIC, calibration pointers)
- Tests are designed to run on x86_64 platforms for compatibility with fuzzing infrastructure
- The harness includes extensive validation and error reporting
- Tests exercise both normal and edge case scenarios
- Mocked functions allow complete control over calibration version values during fuzzing
