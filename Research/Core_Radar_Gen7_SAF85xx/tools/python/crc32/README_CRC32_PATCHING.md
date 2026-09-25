# M7 Code Section CRC32 Patching

This directory contains tools for calculating and patching CRC32 checksums into S19 files for the M7 code section.

## Overview

The build system automatically calculates a CRC32 checksum for the M7 code section (defined by symbols `__M7_code_start_c0` and `__M7_code_end_c0` in the linker map file) and embeds it at address `0x0037FED0` in the final `app.s19` file.

This happens automatically in every build when you run:
```bash
bazelisk build //software:app.s19 --variant=srr7p --board=A1 --micro_revision=ES2
```

## How It Works

1. **Build Flow Integration**: The `//software:app.s19` target now includes an automatic CRC patching step:
   - First, `app_no_crc.s19` is created (merged application, RFE firmware, and header)
   - Then, the CRC patching tool reads `m7App.elf.map` to find code section addresses
   - The CRC is calculated for the code section in `app_no_crc.s19`
   - The CRC value is patched at address `0x0037FED0` to produce the final `app.s19`

2. **CRC Calculation**: Uses the zlib crc32b algorithm (polynomial `0xEDB88320`), matching the implementation in `software/common/crc_calc/crc_calc.c`

3. **Address Symbols**:
   - `__M7_code_start_c0`: Start address of M7 code section (typically `0x00110000`)
   - `__M7_code_end_c0`: End address of M7 code section (typically `0x0037FFFC`)
   - These symbols are defined in the linker script (`software/common/linker/m7.ld`)

## Files

- **`patch_s19_crc.py`**: Python script that performs the CRC calculation and patching
  - Parses the map file to extract address symbols
  - Calculates CRC32 for the code section
  - Patches the CRC value into the S19 file

- **`patch_s19_crc.bzl`**: Bazel rule definition for integrating the tool into the build system

- **`calculate_s19_crc32.py`**: Standalone tool for calculating CRC32 checksums on S19 files (can be used for verification)

- **`BUILD`**: Bazel build file defining the Python binaries and exports

## Manual Usage

While the CRC patching happens automatically during builds, you can use the tools manually for verification:

### Calculate CRC for a specific address range:
```bash
python tools/python/crc32/calculate_s19_crc32.py \
    bazel-bin/outputs/srr7p/m7App.s19 \
    --start 0x00110000 \
    --end 0x0037FFFC \
    --verbose
```

### Patch CRC into an S19 file:
```bash
python tools/python/crc32/patch_s19_crc.py \
    --map bazel-out/.../m7App.elf.map \
    --input app_no_crc.s19 \
    --output app.s19 \
    --crc-address 0x0037FED0
```

## CRC Storage Address

The CRC is stored at address **`0x0037FED0`** (4 bytes, big-endian format). This address is configurable in the Bazel rule if needed.

**Note**: This is different from the existing `app_crc_filled.s19` target which stores CRC at `0x0037FFFC`. The new implementation uses `0x0037FED0` as specified by the user requirement.

## Verification

After building, you can verify the CRC was correctly embedded:

1. Build the application:
   ```bash
   bazelisk build //software:app.s19 --variant=srr7p
   ```

2. Check the CRC value at address `0x0037FED0` in the output:
   ```bash
   # The build will print the calculated CRC value during the patching step
   # Look for output like: "CRC32 = 0x12345678 (305419896)"
   ```

3. Verify by calculating manually:
   ```bash
   python tools/python/crc32/calculate_s19_crc32.py \
       bazel-bin/outputs/srr7p/app.s19 \
       --start 0x00110000 \
       --end 0x0037FFFC
   ```

## Technical Details

### CRC Algorithm
- **Polynomial**: 0xEDB88320 (reversed)
- **Initial Value**: 0x00000000 (inverted to 0xFFFFFFFF internally)
- **Final XOR**: Single inversion (~crc)
- **Endianness**: Big-endian storage in S19 file

This matches the software CRC implementation in `software/common/crc_calc/crc_calc.c` function `crc32_sw()`.

### Build Dependencies
- Input: `app_no_crc.s19` (merged application without CRC)
- Map file: `//software/m7:m7App` (provides m7App.elf.map)
- Output: `app.s19` (final application with embedded CRC)

### Integration Points
- **Primary integration**: `software/BUILD` - defines the `app.s19` target with automatic CRC patching
- **Tooling**: `tools/python/crc32/` - contains the Python scripts and Bazel rules
- **Downstream targets**: `app.ptp` and other targets automatically use the CRC-patched `app.s19`

## Troubleshooting

**Error: "Symbol __M7_code_start_c0 not found"**
- Ensure the linker script defines these symbols
- Check that `m7App.elf.map` is being generated correctly

**Error: "No .map file found"**
- Ensure `//software/m7:m7App` target produces a `.map` file
- Check that the build completed successfully before CRC patching

**CRC value mismatch**
- Verify the address range is correct
- Check that the S19 file hasn't been modified after CRC calculation
- Ensure the same CRC algorithm is used for verification

## Future Enhancements

Potential improvements:
- Support for multiple code sections
- Configurable CRC algorithm parameters
- Runtime CRC verification in the application
- CRC validation tests in CI/CD pipeline
