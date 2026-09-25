# ECU Sync Unit Tests

This directory contains unit tests for the SWC_PLT_CDD_ECUSync component.

## Files

- `SWC_PLT_CDD_ECUSync_unit_test.cc` - Main test file with 19 comprehensive test cases
- `stubs_ecusync_test.c` - Manual stubs for RTE and OS dependencies
- `BUILD` - Bazel build configuration for the unit test target

## Test Coverage

The unit tests achieve excellent coverage of the core ECU Sync functionality:

- **SWC_PLT_CDD_ECUSync.c**: 27/29 lines covered (93.1%)
- **SWC_PLT_CDD_ECUSync_external.c**: 53/57 lines covered (93.0%)
- **Overall**: 29.2% line coverage, 25.9% function coverage, 32.1% decision coverage, 26.9% branch coverage

## Running Tests

### Basic Test Execution

```bash
# Run ECU Sync unit tests
.\bazelisk test //software/r52/autosar/swc/PLT_SWC/SWC_PLT_CDD_ECUSync/test:SWC_PLT_CDD_ECUSync_unit_test

# Run with detailed output
.\bazelisk test //software/r52/autosar/swc/PLT_SWC/SWC_PLT_CDD_ECUSync/test:SWC_PLT_CDD_ECUSync_unit_test --test_output=all
```

### Coverage Generation

```bash
# Generate coverage report (included in main coverage build)
.\bazelisk test //coverage:report

# Check ECU Sync specific coverage results
type "bazel-bin\coverage\ecusync.txt"
```

### Coverage Report Locations

After running coverage builds, reports are available at:

- **Text Summary**: `bazel-bin/coverage/ecusync.txt`
- **HTML Report**: `bazel-bin/coverage/ecusync_gcovr_report_html/index.html`
- **JSON Report**: `bazel-bin/coverage/ecusync_gcovr_report.json`
- **Cobertura XML**: `bazel-bin/coverage/ecusync_cobertura.xml`

## Test Strategy

The unit tests cover:

### Main ECU Sync Functions
- `RE_PLT_CDD_ECUSync_Init()` - Component initialization
- `Radar_Trigger_Timer_ISR()` - Timer interrupt handler with all dtNext branches

### External Utility Functions
- `CaptureGlobalTimestamp()` - Timestamp capture with buffering
- `GetGlobalTimestamp()` - Timestamp retrieval from buffer
- `PeekGlobalTimestamp()` - Non-destructive timestamp peek
- `GetTimeStampDiff_Ns()` - Timestamp difference calculation
- `GetMidTimeStamps()` - Midpoint timestamp calculation

### Edge Cases Tested
- Buffer overflow scenarios
- Invalid input parameters
- Large timestamp differences
- Zero and maximum nanosecond values
- NULL pointer handling
- Empty buffer conditions

## Stubbing Strategy

Manual stubs are used instead of fff framework due to type compatibility issues in the AUTOSAR environment. Key stubbed functions:

- `Rte_Call_Pp_GlobalTime_Slave_StbMSynchronizedTimeBase_GetCurrentTime`
- `Rte_Call_Pp_Radar_Ctl_Look_Trigger_Operation`
- `Start_EcuSync_Timer`
- `SuspendAllInterrupts` / `ResumeAllInterrupts`
- `Set_Timer_Cnt_Value_ms`

The stubs provide controllable behavior for comprehensive test coverage.

## Notes

- Tests run successfully on Windows with MinGW toolchain
- Coverage data is collected through gcov/gcovr integration
- Windows-specific coverage collection script issues are worked around by using the main coverage build
- All 19 test cases pass consistently
