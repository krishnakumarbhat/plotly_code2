# MCAL Test Automation

Complete automation for MCAL testing on Gen8 iND13400 radar system with automatic Trace32 launch, software flashing, test execution, and comprehensive reporting.

---

## Table of Contents

- [Features](#features)
- [Prerequisites](#prerequisites)
- [Quick Start](#quick-start)
- [Configuration File](#configuration-file)
- [Usage Options](#usage-options)
- [MCAL Tests](#mcal-tests)
- [Output Reports](#output-reports)
- [How Auto-Flash Works](#how-auto-flash-works)
- [Troubleshooting](#troubleshooting)
- [Advanced Topics](#advanced-topics)

---

## Features

✅ **Auto-Flash Mode** - Launches Trace32 and flashes software automatically (bypasses manual OK dialog)
✅ **Remote API Control** - Full debugger control via Trace32 Remote API
✅ **Smart Path Resolution** - Handles Bazel symlinks correctly
✅ **10 MCAL Tests** - MCU, DIO, I2C, Flash, UART, SPI, PWM, GPT, ADC, DMA
✅ **ADC Validation** - Reads and validates 10 ADC buffer values
✅ **SPI/Radar Init Validation** - Verifies Radar Control initialization
✅ **Dual Report Formats** - HTML (browser) and JUnit XML (CI/CD)
✅ **Batch File Support** - One-click execution via Windows batch file
✅ **Configuration File** - Simple INI file for easy customization

---

## Prerequisites

### Required
1. **Lauterbach Trace32** with Remote API support
2. **Python 3.7+** with `lauterbach-trace32-rcl`:
   ```powershell
   pip install lauterbach-trace32-rcl
   ```
3. **Build with MCAL tests enabled**:
   ```powershell
   bazelisk build //:gen8 --config=flr8 --veh_com=can --enable_mcal_test
   ```
4. **Hardware connected** and powered on

### Configuration
Enable Remote API in `tools/lauterbach/config.t32`:
```
RCL=NETASSIST PORT=20000 PACKLEN=1024
```

---

## Quick Start

### Method 1: Batch File (Easiest) ⭐

```powershell
# Step 1: Build software
bazelisk build //:gen8 --config=flr8 --veh_com=can --enable_mcal_test

# Step 2: Edit config file
# Set r52_elf path in tools/python/mcal_test_config.ini

# Step 3: Run batch file
.\tools\python\run_mcal_tests.bat
```

**Done!** Reports generated in `mcal_test_reports/` and HTML opens automatically.

**Total time: ~45 seconds**

---

### Method 2: Configuration File

```powershell
python tools/python/mcal_test_automation.py --config tools/python/mcal_test_config.ini
```

---

### Method 3: Command Line

```powershell
python tools/python/mcal_test_automation.py `
    bazel-bin/outputs/flr8_satellite_can/r52App.elf `
    --auto-flash `
    --output mcal_test_reports
```

---

## Configuration File

**File:** `tools/python/mcal_test_config.ini`

```ini
[Paths]
# Path to R52 ELF file (REQUIRED)
# Use forward slashes (/) or double backslashes (\\)
r52_elf = bazel-bin/outputs/flr8_satellite_can/r52App.elf

# Flash session file (OPTIONAL - auto-detected if in same folder as ELF)
flash_session = bazel-bin/outputs/flr8_satellite_can/flash_session.ini

# Output directory for reports (OPTIONAL - default: mcal_test_reports)
output_dir = mcal_test_reports

[Trace32]
# Trace32 executable path (OPTIONAL - auto-detected)
# t32_executable = C:\\T32\\bin\\windows64\\t32marm.exe

# Remote API settings (OPTIONAL - defaults shown)
port = 20000
packlen = 1024

[Options]
# Auto-flash: true = automatic flash, false = manual flash required
auto_flash = true

# Open HTML report in browser after completion
open_report = true
```

### Different Build Variants

**FLR8 Satellite CAN:**
```ini
r52_elf = bazel-bin/outputs/flr8_satellite_can/r52App.elf
```

**SRR8P Satellite CAN:**
```ini
r52_elf = bazel-bin/outputs/srr8p_satellite_can/r52App.elf
```

**FLR8 SomeIP (Ethernet):**
```ini
r52_elf = bazel-bin/outputs/flr8_someip/r52App.elf
```

---

## Usage Options

### Command-Line Arguments

| Argument | Description | Default | Example |
|----------|-------------|---------|---------|
| `elf_file` | Path to R52 ELF file | - | `r52App.elf` |
| `--config` | Path to config INI file | - | `mcal_test_config.ini` |
| `--auto-flash` | Enable auto-flash mode | `false` | - |
| `--output` | Output directory | `mcal_test_reports` | `my_reports` |
| `--port` | Trace32 Remote API port | `20000` | `20001` |
| `--packlen` | Trace32 packet length | `1024` | `2048` |

### Examples

```powershell
# Using config file (recommended)
python mcal_test_automation.py --config mcal_test_config.ini

# Command line with auto-flash
python mcal_test_automation.py r52App.elf --auto-flash

# Custom output directory
python mcal_test_automation.py r52App.elf --output test_results

# Custom Trace32 port
python mcal_test_automation.py r52App.elf --port 20001

# Show help
python mcal_test_automation.py --help
```

---

## MCAL Tests

### Test List

| # | Module | Description | Pass Criteria | Validation |
|---|--------|-------------|---------------|------------|
| 1 | **MCU** | MCU initialization | `status == 1` | C variable |
| 2 | **DIO** | Digital I/O read/write | `status == 1` | C variable |
| 3 | **I2C** | I2C communication | `status == 1` | C variable |
| 4 | **Flash** | Flash memory access | `status == 1` | C variable |
| 5 | **UART** | UART communication | `status == 1` | C variable |
| 6 | **SPI** | SPI communication | `Radar_Ctl_Data.init_status == 3` | Python validation |
| 7 | **PWM** | PWM generation | `status == 1` | C variable |
| 8 | **GPT** | General Purpose Timer | `status == 1` | C variable |
| 9 | **ADC** | ADC conversion | All 10 buffer values non-zero | Python validation |
| 10 | **DMA** | DMA transfer | `status == 1` | C variable |

### C Test Variables

These variables are declared in `software/r52/drivers/mcal/src/test/mcal_test.c`:

```c
bool Mcal_Mcu_Test_status = TEST_FAIL;   // 0 = FAIL, 1 = PASS
bool Mcal_Dio_Test_status = TEST_FAIL;
bool Mcal_I2c_Test_status = TEST_FAIL;
bool Mcal_Fls_Test_status = TEST_FAIL;
bool Mcal_Uart_Test_status = TEST_FAIL;
bool Mcal_Pwm_Test_status = TEST_FAIL;
bool Mcal_Gpt_Test_status = TEST_FAIL;
bool Mcal_Dma_Test_status = TEST_FAIL;
```

### Python-Based Validation

**ADC Test:**
- Reads `Adc_Results_Buff[10]` array
- Validates all 10 values are non-zero
- Sets `Mcal_Adc_Test_status = PASS` if all valid

**SPI/Radar Control Test:**
- Reads `Radar_Ctl_Data.init_status`
- Validates value is `3` (RADAR_CTL_INIT_SUCCESS)
- Sets `Mcal_Spi_Test_status = PASS` if valid

---

## Output Reports

### Generated Files

Reports are saved to `mcal_test_reports/` (or custom directory):

```
mcal_test_reports/
├── mcal_test_report_20260106_143022.html    ← Open in browser
└── mcal_test_report_20260106_143022.xml     ← JUnit format for CI/CD
```

### HTML Report Contents

- **Test Summary**
  - Total: 10 tests
  - Passed/Failed/Not Run counts
  - Radar Control Init Status badge (green = success, red = fail)
  - Test duration

- **Detailed Results Table**
  - Test name
  - Actual value (from target)
  - Expected value
  - Status (PASS/FAIL/NOT_RUN/ERROR)
  - Color-coded rows

- **ADC Buffer Values**
  - All 10 ADC samples displayed
  - Validation result

- **Execution Details**
  - ELF file path
  - Timestamp
  - Breakpoints hit

### XML Report Format

JUnit-compatible XML for CI/CD integration:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<testsuites name="MCAL Tests" tests="10" failures="0" time="45.2">
    <testsuite name="MCAL_Test_Suite" tests="10" failures="0">
        <testcase name="Mcal_Mcu_Test_status" classname="MCAL"/>
        <testcase name="Mcal_Dio_Test_status" classname="MCAL"/>
        <!-- ... more tests ... -->
    </testsuite>
</testsuites>
```

---

## How Auto-Flash Works

### Technical Flow

1. **Script generates temporary config** at `tools/lauterbach/mcal_test_user_config.cmm`:
   ```cmm
   &auto_flash_session="C:/path/to/flash_session.ini"
   ```

2. **Launches Trace32** using existing `_start_powerview_r52.bat`:
   ```
   C:\T32\bin\windows64\t32marm.exe -c config.t32 -s start_powerview.cmm USER_CONFIG=mcal_test_user_config.cmm
   ```

3. **Flash Utility auto-loads** and flashes without manual OK dialog

4. **Waits for completion** (40 seconds: 20s flash + 5s stabilize + 15s buffer)

5. **Connects via Remote API** on port 20000

6. **Executes test sequence**:
   - Load ELF symbols
   - Set breakpoint at `Mcal_Test()`
   - Reset target (SYStem.RESetOut)
   - Wait for breakpoint (up to 30 seconds)
   - Continue execution
   - Wait 5 seconds for tests to complete
   - Read all test variables
   - Validate ADC buffer and SPI status
---

## Troubleshooting

### ❌ "flash_session.ini not found"
**Cause:** ELF path is wrong or build incomplete
**Fix:**
```powershell
Test-Path "bazel-bin/outputs/flr8_satellite_can/r52App.elf"
Test-Path "bazel-bin/outputs/flr8_satellite_can/flash_session.ini"
```

### ❌ "ERROR connecting to Trace32"
**Cause:** Trace32 not running or Remote API not enabled
**Fix:**
1. Check Trace32 is running
2. Verify `tools/lauterbach/config.t32` has: `RCL=NETASSIST PORT=20000`
3. Restart Trace32 to enable Remote API
4. Check firewall isn't blocking port 20000

### ❌ "Could not find Trace32 Python API"
**Cause:** Python package not installed
**Fix:**
```powershell
pip install lauterbach-trace32-rcl
```

### ❌ "Timeout waiting for Mcal_Test breakpoint"
**Cause:** Software not reaching `Mcal_Test()` function
**Fix:**
1. Verify build: `bazelisk build //:gen8 --config=flr8 --enable_mcal_test`
2. Check `Mcal_Test()` is called in `main.c` or `mcal_init.c`
3. Ensure software is flashed correctly
4. Check target is running (not crashed)

### ❌ Flash loads wrong binaries (shows C:\Users\ulb4jq\...)
**Cause:** Old issue - should be fixed in latest version
**Status:** ✅ Fixed with symlink resolution in script
**Verify:** Check console output shows real path, not `bazel-bin` symlink

### ❌ "ERROR reading Adc_Results_Buff"
**Cause:** Variable not in symbol table or optimized out
**Fix:**
1. In Trace32 command line: `Var.View Adc_Results_Buff`
2. If not found, rebuild with `--enable_mcal_test`
3. Check variable isn't optimized out

### ❌ "ERROR reading Radar_Ctl_Data.init_status"
**Cause:** Variable not in symbol table
**Fix:**
1. In Trace32: `Var.View Radar_Ctl_Data.init_status`
2. Check SPI initialization completed

### ❌ Manual OK dialog appears during automation
**Cause:** `auto_flash = false` in config
**Fix:** Set `auto_flash = true` in `mcal_test_config.ini`

### ❌ Tests pass but ADC/SPI fail
**Cause:** Python validation failed
**Fix:**
- ADC: Check all 10 values in `Adc_Results_Buff` are non-zero
- SPI: Check `Radar_Ctl_Data.init_status == 3`

---

## Advanced Topics

### Extending with New MCAL Tests

**Step 1:** Add test in `mcal_test.c`:
```c
bool Mcal_NewModule_Test_status = TEST_FAIL;

void Mcal_NewModule_Test(void) {
    // Your test code
    Mcal_NewModule_Test_status = TEST_PASS;
}

void Mcal_Test(void) {
    // ...
    #if (NEWMODULE_TEST_ENABLE == TEST_ENABLED)
    Mcal_NewModule_Test();
    #endif
}
```

**Step 2:** Update `mcal_test_automation.py`:

Add to `test_results` dictionary in `__init__()`:
```python
"Mcal_NewModule_Test_status": {
    "actual": None,
    "expected": 1,
    "status": "NOT_RUN"
}
```

Add to `test_variables` list in `execute_test()`:
```python
test_variables = [
    # ... existing ...
    "Mcal_NewModule_Test_status"
]
```

The script will automatically read, validate, and report it!

### CI/CD Integration

**Jenkinsfile example:**
```groovy
stage('MCAL Test') {
    steps {
        bat """
            python tools/python/mcal_test_automation.py ^
                --config tools/python/mcal_test_config.ini
        """
    }
    post {
        always {
            junit 'mcal_test_reports/*.xml'
            publishHTML([
                reportDir: 'mcal_test_reports',
                reportFiles: '*.html',
                reportName: 'MCAL Test Report'
            ])
        }
    }
}
```

### Trace32 State Machine

Target states from `t32.get_state()`:
- `0` (DOWN) - Target powered off or disconnected
- `1` (STOPPED) - Target halted
- `2` (BREAKPOINT) - Target stopped at breakpoint
- `3` (RUNNING) - Target executing code

**Note:** API returns `bytearray`, convert using `state[0]`

### Reset Sequence

The script uses this proven sequence:
1. `SYStem.RESetOut` - Hardware reset
2. `wait(1)` - Stabilization
3. `Go` - Start execution
4. `wait(5)` - Allow breakpoint to be hit

This matches Core Radar automation framework patterns.

---

## Important Notes

⚠️ **Build requirement:** Must include `--enable_mcal_test` flag
⚠️ **Flash session uses relative paths** - Script resolves symlinks automatically
⚠️ **Trace32 stays running** after script (for debugging)
⚠️ **Hardware must be connected** and powered on

✅ **No manual intervention** when using auto-flash mode
✅ **Batch file is simplest** - edit config and double-click
✅ **Works with all variants** - FLR8, SRR8P, SomeIP, Standalone

---

## Related Files

- **Script:** `tools/python/mcal_test_automation.py`
- **Config:** `tools/python/mcal_test_config.ini`
- **Batch File:** `tools/python/run_mcal_tests.bat`
- **Source Code:** `software/r52/drivers/mcal/src/test/mcal_test.c`
- **Main Entry:** `software/r52/main.c`
- **Build Flag:** `.bazelrc` (`--flag_alias=enable_mcal_test`)
- **Trace32 Config:** `tools/lauterbach/config.t32`
- **Flash Infrastructure:** `tools/lauterbach/` directory

---

## Quick Reference

### Complete Workflow
```powershell
# 1. Build
bazelisk build //:gen8 --config=flr8 --veh_com=can --enable_mcal_test

# 2. Edit config (one line)
# r52_elf = bazel-bin/outputs/flr8_satellite_can/r52App.elf

# 3. Run
.\tools\python\run_mcal_tests.bat

# 4. Check reports
# Open: mcal_test_reports/*.html
```

### What Script Does
1. ✅ Launches Trace32 automatically
2. ✅ Flashes software (no manual OK)
3. ✅ Loads ELF symbols
4. ✅ Sets breakpoint at `Mcal_Test()`
5. ✅ Resets and starts target
6. ✅ Waits for breakpoint
7. ✅ Runs all 10 MCAL tests
8. ✅ Reads test results
9. ✅ Validates ADC buffer
10. ✅ Validates SPI/Radar init
11. ✅ Generates HTML + XML reports
12. ✅ Opens HTML in browser

**Total time: ~45 seconds** ⚡

---

## Getting Help

For issues:
1. Check console output for detailed error messages
2. Review configuration file: `tools/python/mcal_test_config.ini`
3. Check Trace32 is running and connected to hardware
4. Verify build includes MCAL tests: `--enable_mcal_test`
5. Check this README troubleshooting section
