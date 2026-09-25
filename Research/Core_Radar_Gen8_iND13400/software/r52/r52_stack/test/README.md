# Stack Monitor R52 Unit Tests

## Overview
This directory contains comprehensive unit tests for the `stack_monitor_r52.c` module. The tests are designed to achieve **95%+ code coverage** as per the project requirements.

## Test Structure

### Files
- `stack_monitor_r52_unit_test.cc` - Main unit test file with all test cases
- `stack_monitor_r52_fake.h` - Mock/fake function declarations
- `stack_monitor_r52_fake.cc` - Mock/fake function definitions
- `BUILD` - Bazel build configuration for tests

### Test Framework
- **Google Test (gtest)**: Primary testing framework
- **FFF (Fake Function Framework)**: For mocking AUTOSAR OS functions

## Functions Under Test

### 1. `Stack_Monitor_R52()`
Main monitoring function that:
- Increments monitoring counter
- Clears previous stack data
- Monitors kernel, error hook, and shutdown hook stacks
- Monitors ISR stacks for all configured ISRs
- Monitors task stacks for all configured tasks
- Calculates percentage utilization

**Test Cases:**
- Normal operation with valid data
- ISR monitoring verification
- Task monitoring verification
- Shared vs non-shared task stacks
- Zero stack usage scenarios
- Maximum stack usage scenarios
- Counter increment verification
- Percentage calculation verification
- Data clearing between calls

### 2. `CalculateStackUsageInPercentage()`
Calculates percentage of stack usage given size and usage.

**Test Cases:**
- Normal values (50% usage)
- Zero stack size (division by zero protection)
- Zero usage
- 100% usage
- Maximum values
- Edge cases

### 3. `CalculateStackUsageInPercentage_task()`
Calculates task stack usage percentage using low/high address pointers.

**Test Cases:**
- Valid pointers with normal usage
- NULL low pointer
- NULL high pointer
- Both NULL pointers
- Zero usage
- 100% usage
- Address calculation verification

### 4. Getter Functions
- `Get_stack_isr_r52()`
- `Get_stack_task_r52()`
- `Get_stack_core_r52()`

**Test Cases:**
- Returns valid non-NULL pointer
- Returns correct structure address

## Running Tests

### Build and Run All Tests
```bash
bazelisk test //software/r52/r52_stack/test:unit_tests --test_output=all
```

### Run Specific Test
```bash
bazelisk test //software/r52/r52_stack/test:stack_monitor_r52_unit_test --test_output=all
```

### Generate Coverage Report (Individual)
```bash
bazelisk coverage //software/r52/r52_stack/test:unit_tests
```

### Generate Full Coverage Report (With Stack Monitor)
```bash
bazelisk coverage //coverage:r52_stack_monitor
```

### View Coverage Results
After running coverage:
```bash
# View summary
genhtml bazel-out/_coverage/_coverage_report.dat --output-directory coverage_html

# Open in browser
start coverage_html/index.html
```

## Coverage Goals

| Metric | Target | Description |
|--------|--------|-------------|
| Line Coverage | 95%+ | Percentage of executable lines tested |
| Function Coverage | 95%+ | Percentage of functions called in tests |
| Decision Coverage | 95%+ | Percentage of decision points tested |
| Branch Coverage | 95%+ | Percentage of branches executed |

## Mocked AUTOSAR OS Functions

The following OS functions are mocked using FFF:
- `Os_GetKernelStackUsage()`
- `Os_GetErrorHookStackUsage()`
- `Os_GetShutdownHookStackUsage()`
- `Os_GetProtectionHookStackUsage()`
- `Os_GetStartupHookStackUsage()`
- `Os_GetISRStackUsage()`
- `Os_GetTaskStackUsage()`
- `Os_TaskId2Task()`
- `Os_TaskGetThread()`
- `Os_ThreadGetStack()`

## Test Methodology

### Setup (Before Each Test)
1. Reset FFF framework history
2. Clear global counters
3. Initialize mock structures
4. Setup mock stack addresses
5. Configure default task properties

### Teardown (After Each Test)
- Cleanup test artifacts if needed

### Assertions
- Verify function call counts
- Verify return values
- Verify data structure contents
- Verify percentage calculations
- Verify proper initialization/cleanup

## Adding New Tests

To add new test cases:

1. **Add test function in `stack_monitor_r52_unit_test.cc`:**
```cpp
TEST_F(StackMonitorR52_UnitTestSuite, YourTestName)
{
   // Setup mocks
   Os_GetKernelStackUsage_fake.return_val = expected_value;

   // Execute function under test
   Stack_Monitor_R52();

   // Verify results
   ASSERT_EQ(actual_value, expected_value);
}
```

2. **Update BUILD file if adding new dependencies**

3. **Run tests to verify**

4. **Check coverage to ensure new code paths are covered**

## Troubleshooting

### Test Failures
1. Check mock setup - ensure all required mocks return valid values
2. Verify test expectations match actual implementation
3. Check for uninitialized data in mock structures

### Low Coverage
1. Review coverage report to identify uncovered lines
2. Add test cases for missing branches
3. Ensure all error paths are tested
4. Test boundary conditions

### Build Errors
1. Verify all dependencies are properly declared in BUILD file
2. Check include paths
3. Ensure FFF and gtest are available

## Integration with CI/CD

These tests are automatically run as part of:
- Pre-commit hooks
- Pull request validation
- Nightly builds
- Coverage reporting pipeline

## References
- [Google Test Documentation](https://google.github.io/googletest/)
- [FFF Framework](https://github.com/meekrosoft/fff)
- AUTOSAR OS Specification
- Stack Monitor R52 Design Document

## Maintenance

### Review Frequency
- Tests should be reviewed quarterly
- Update tests when source code changes
- Maintain 95%+ coverage threshold

### Contact
For questions or issues with these tests, contact the Platform Software Team.

---
Last Updated: November 1, 2025
Coverage Target: 95%+
Test Framework: Google Test + FFF
