# AUTOSAR Code Generation Automation

Automated AUTOSAR BSW/MCAL code generation using DaVinci Configurator Command Line (DVCfgCmd.exe).

## Overview

`generate_autosar.py` is a Python automation script that eliminates manual DaVinci Configurator GUI operations by orchestrating the complete AUTOSAR code generation workflow:

1. **SIP Generator Setup** - Auto-downloads DaVinci generators if missing
2. **MCAL Package Sync** - Copies latest MCAL packages from repository
3. **Configuration Validation** - Validates AUTOSAR configuration (`.dpa` project)
4. **Code Generation** - Generates BSW/MCAL source files for specified modules

## Prerequisites

- **Python 3.7+** (3.10 recommended)
- **DaVinci Configurator CLI** (`DVCfgCmd.exe`) - Auto-installed if missing
- **AUTOSAR SIP** (Software Integration Package) - Auto-downloaded if needed
- **AUTOSAR Project File** - `Gen8_iND13400.dpa` must exist in config directory

## Quick Start

### Generate All Modules

```bash
python generate_autosar.py
```

### Validate Configuration Only

```bash
python generate_autosar.py --validate
```

### Generate Specific Modules

```bash
python generate_autosar.py --modules Dio Port Gpt Mcu Spi
```

### Set Hardware Variant and Generate

```bash
python generate_autosar.py --modules Dio Port --hardware-variant B0
```

### Enable Verbose Output

```bash
python generate_autosar.py --modules Can Eth --verbose
```

## Command-Line Options

| Option | Short | Description |
|--------|-------|-------------|
| `--validate` | `-val` | Validate configuration only (no code generation) |
| `--generate` | `-gen` | Force code generation (default behavior) |
| `--modules` | `-m` | Specify modules to generate (space-separated list) |
| `--module-paths` | `-mp` | Specify module paths directly (alternative to --modules) |
| `--hardware-variant` | `-hw` | Set hardware variant (requires --modules) |
| `--verbose` | `-v` | Enable verbose debug output |
| `--repo-root` | | Override repository root path (auto-detected by default) |

## Supported Modules

- **Dio** - Digital I/O driver
- **Port** - Port driver
- **Gpt** - General Purpose Timer driver
- **Mcu** - Microcontroller driver
- **Spi** - SPI communication driver
- **I2c** - I2C communication driver
- **Adc** - Analog-to-Digital Converter driver
- **Pwm** - Pulse Width Modulation driver
- **Can** - CAN communication driver
- **Eth** - Ethernet communication driver
- **Fls** - Flash driver
- **Uart** - Uart driver
- **DMA** - Dma Driver

## Automation Workflow

### Step 1: SIP Generator Check/Download

- Searches for `DVCfgCmd.exe` in SIP folder
- If not found → automatically runs `init_sip_and_download_generators.bat`
- **Skipped if DVCfgCmd.exe already exists**
- Real-time download progress displayed

### Step 2: MCAL Package Copy

- Executes `repo_copy.py` from `tools/python/copy_mcal_package/`
- Copies latest MCAL packages into config directory
- **Always runs** to ensure up-to-date packages
- Real-time output streaming

### Step 3: AUTOSAR Configuration Validation

- Validates `.dpa` project file using DVCfgCmd.exe
- Checks AUTOSAR parameter definitions
- Reports validation errors with module-specific details

### Step 4: Code Generation

- Generates C source files for specified modules
- Updates configuration headers
- Creates output in module-specific directories
- Displays failed modules summary if errors occur

## Output

### Generation Logs

Logs are stored in: `software/r52/autosar/config/`

- `AUTOSAR_Validation_<timestamp>.log` - Validation results
- `AUTOSAR_Generation_<timestamp>.log` - Generation results

### Generated Files

Generated code is placed in module-specific directories:
```
software/r52/autosar/config/
├── Dio_bswmd.arxml
├── Port_bswmd.arxml
├── Config/
│   └── ECUC/
│       ├── Dio_ecuc.arxml
│       └── Port_ecuc.arxml
└── Appl/
    └── GenData/
        ├── Dio_Cfg.c
        ├── Port_Cfg.c
        └── ...
```

## Error Handling

### Validation Errors

The script parses DaVinci log files and displays:
- Total error count
- Module-specific errors
- List of failed modules for quick identification

**Example Output:**
```
VALIDATION RESULTS
==================
Total Errors: 15
Total Warnings: 42

FAILED MODULES:
  ✗ Can (5 errors)
  ✗ Eth (8 errors)
  ✗ Port (2 errors)
```

### Common Errors

| Error Type | Cause | Solution |
|------------|-------|----------|
| `AR-ECUC02008` | Invalid parameter multiplicity | Fix in DaVinci GUI |
| `AR-ECUC03019` | Incorrect definition reference | Update parameter paths in `.dpa` |
| `DVCfgCmd.exe not found` | Missing SIP generators | Run `init_sip_and_download_generators.bat` |
| `repo_copy.py failed` | MCAL packages missing | Check source repository path |

The script automatically handles all prerequisite steps (SIP download, package copy).

**Last Updated:** January 2026
**Script Version:** 2.0 (Auto-download + repo_copy integration)
