# SWC_PLT_Appl_Communication Unit Tests

This directory contains comprehensive unit tests for the SWC_PLT_Appl_Communication component, which handles vehicle communication via CAN or SOME/IP protocols.

## Overview

The SWC_PLT_Appl_Communication component is responsible for:
- Initializing communication mode (CAN or SOME/IP)
- Processing received messages (25ms cycle)
- Transmitting status and diagnostic messages (50ms cycle)
- Event-triggered CAN message transmission

## Test Structure

### Test Files

- **SWC_PLT_Appl_Communication_unit_test.cc** - Main test suite with 15 test cases
- **SWC_PLT_Appl_Communication_fake.h** - Mock function declarations (FFF framework)
- **SWC_PLT_Appl_Communication_fake.cc** - Mock function implementations
- **BUILD** - Bazel build configuration with separate CAN and SOME/IP test targets

### Stub Headers

Nine stub header files prevent inclusion of real AUTOSAR BSW headers during unit testing:
- `CanSM_ComM.h`, `ComM_Cfg.h`, `Rte_ComM_Type.h`
- `Rte_SWC_PLT_Appl_Communication.h`, `SchM_Com.h`
- `can_rx_msgs.h`, `can_tx_msgs.h`, `someip_services.h`
- `SWC_PLT_Appl_Communication_MemMap.h`

## Test Variants

Two test executables cover different conditional compilation paths:

### 1. CAN Variant (`swc_plt_appl_communication_unit_test_can`)
- **Define:** `VEH_CAN`
- **Tests:** 10 test cases
- **Coverage:** Init, Rx, Tx event trigger for CAN protocol

### 2. SOME/IP Variant (`swc_plt_appl_communication_unit_test_someip`)
- **Define:** `VEH_SOMEIP`
- **Tests:** 5 test cases
- **Coverage:** Init, Rx for SOME/IP protocol

## Running Tests

### Run All Tests
```bash
bazelisk test //software/r52/autosar/swc/SWC_PLT_Appl_Communication/test:unit_tests
```

### Run CAN Tests Only
```bash
bazelisk test //software/r52/autosar/swc/SWC_PLT_Appl_Communication/test:swc_plt_appl_communication_unit_test_can
```

### Run SOME/IP Tests Only
```bash
bazelisk test //software/r52/autosar/swc/SWC_PLT_Appl_Communication/test:swc_plt_appl_communication_unit_test_someip
```

### Run with Detailed Output
```bash
bazelisk test //software/r52/autosar/swc/SWC_PLT_Appl_Communication/test:unit_tests --test_output=all
```

## Test Cases

### CAN Mode Tests

| Test Case | Purpose |
|-----------|---------|
| `RE_Communication_Init_CAN_RequestsCommMode` | Verify CanSM_RequestComMode called with correct parameters |
| `RE_Communication_Init_CAN_Success` | Test successful initialization (E_OK) |
| `RE_Communication_Init_CAN_Failure` | Test failed initialization (E_NOT_OK) |
| `RE_Communication_Rx_CAN_ProcessesMessages` | Verify CAN message processing function called |
| `RE_Communication_Rx_CAN_MultipleCalls` | Test multiple cyclic calls (simulating 25ms timing) |
| `CAN_Tx_Event_Trigger_SendsAllMessages` | Verify all 10 CAN message types are sent |
| `CAN_Tx_Event_Trigger_CallsComMainFunctionTx` | Verify COM main TX function is called |
| `CAN_Tx_Event_Trigger_MessageOrder` | Validate message transmission order |
| `CAN_Tx_Event_Trigger_MultipleInvocations` | Test multiple event triggers |
| `SanityCheck` | Basic framework sanity test |

### SOME/IP Mode Tests

| Test Case | Purpose |
|-----------|---------|
| `RE_Communication_Init_SOMEIP_InitializesServices` | Verify provided and consumed services initialized |
| `RE_Communication_Init_SOMEIP_CallOrder` | Validate init order (provided before consumed) |
| `RE_Communication_Rx_SOMEIP_ProcessesServices` | Verify SOME/IP service processing |
| `RE_Communication_Rx_SOMEIP_MultipleCalls` | Test multiple cyclic calls (25ms timing) |
| `SanityCheck` | Basic framework sanity test |

## Mocked Functions

### CAN Functions (3)
1. `CanSM_RequestComMode` - Request CAN communication mode
2. `Com_MainFunctionTx_ComMainFunctionTx_Event` - COM main transmission function
3. `Process_Received_CAN_Msgs` - Process received CAN messages

### SOME/IP Functions (4)
1. `Init_Provided_Services` - Initialize provided SOME/IP services
2. `Init_Consumed_Services` - Initialize consumed SOME/IP services
3. `Process_Provided_Services` - Process provided SOME/IP services
4. `Process_Consumed_Services` - Process consumed SOME/IP services

### CAN Message Transmission Functions (10)
1. `Send_Radar_Status_001_Messages`
2. `Send_Radar_Status_002_Messages`
3. `Send_Alignment_Status_001_Messages`
4. `Send_Alignment_Status_002_Messages`
5. `Send_Radar_Capability_Messages`
6. `Send_Tracker_Status_Messages`
7. `Send_Radar_Header_Messages`
8. `Send_Radar_Detection_Messages`
9. `Send_Tracker_Header_Messages`
10. `Send_Tracker_Objects_On_CAN`

## Functions Under Test

| Function | Lines | Purpose | Coverage |
|----------|-------|---------|----------|
| `RE_Communication_Init()` | ~899-950 | Initialize communication mode | 100% |
| `RE_Communication_Rx()` | ~952-998 | Process received messages (25ms) | 100% |
| `RE_Communication_Tx_50ms()` | ~1845-1890 | Transmit messages (50ms) | Partial |
| `CAN_Tx_Event_Trigger()` | ~1893-1947 | Event-triggered CAN transmission | 100% |

## Test Framework

- **Unit Test Framework:** Google Test (gtest)
- **Mocking Framework:** FFF (Fake Function Framework)
- **Build System:** Bazel 7.x
- **Target Platform:** x86_64 (unit tests) / ARM R52 (production)

## Coverage

### Coverage Report

Run coverage analysis:
```bash
bazelisk coverage //coverage:swc_plt_appl_communication
```

**Note:** Coverage data generation may fail on Windows due to path length limitations (>260 characters). Run on Linux for accurate coverage reports.

### Expected Coverage

- **Line Coverage:** ~85-90%
- **Function Coverage:** 100%
- **Branch Coverage:** 100%
- **Decision Coverage:** 100%

## CI/CD Integration

These tests are integrated into the CI/CD pipeline:

- **Test Suite:** `//BUILD:all_unit_tests`
- **Coverage Report:** `//coverage:swc_plt_appl_communication`
- **Build Targets:** Included in main build verification

## Development

### Adding New Tests

1. Add test case to `SWC_PLT_Appl_Communication_unit_test.cc`
2. Add any new mock functions to `SWC_PLT_Appl_Communication_fake.h/.cc`
3. Update `SetUp()` to reset new fakes
4. Run tests to verify

### Test Naming Convention

- Prefix: `RE_Communication_<Function>_<Variant>_<Scenario>`
- Example: `RE_Communication_Init_CAN_RequestsCommMode`

### Mock Function Pattern

```cpp
// In fake.h
DECLARE_FAKE_VOID_FUNC(Function_Name);

// In fake.cc
DEFINE_FAKE_VOID_FUNC(Function_Name);

// In unit_test.cc SetUp()
RESET_FAKE(Function_Name);
```

## Troubleshooting

### Tests Not Building

**Issue:** Compilation errors about missing headers
**Solution:** Ensure stub headers are in place and BUILD file includes them in hdrs

### Tests Failing

**Issue:** Unexpected call counts
**Solution:** Check that fakes are reset in `SetUp()` and test expectations are correct

### Coverage Not Working

**Issue:** `.gcda` files cannot be created
**Solution:**
- Run on Linux (no path length limitation)
- Use shorter workspace path (e.g., `C:\Gen8\`)
- Disable coverage collection: Remove `--collect_code_coverage` flag

## References

- **Source Code:** `software/r52/autosar/swc/SWC_PLT_Appl_Communication/Source/SWC_PLT_Appl_Communication.c`
- **FFF Framework:** https://github.com/meekrosoft/fff
- **Google Test:** https://github.com/google/googletest
- **Success Report:** See `TEST_SUCCESS_REPORT.md` in this directory

## Maintainers

For questions or issues with these tests, contact the Core Radar Gen8 team.

---

**Last Updated:** November 1, 2025
**Test Status:** ✅ All tests passing (15/15)
**Framework Version:** Google Test 1.11.0, FFF Latest
