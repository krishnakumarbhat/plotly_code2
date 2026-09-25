# S19 CRC32 Calculator

Calculate CRC32 checksums for S19 files using zlib crc32b algorithm. Matches `crc32_sw()` in `software/common/crc_calc/crc_calc.c`.

## Requirements

Python 3.7+ (standard library only)

## Usage

```powershell or command prompt

# Address range (both inclusive)
python tools\python\calculate_s19_crc32.py <file.s19> -s <start> -e <end>

# Verbose mode
python tools\python\calculate_s19_crc32.py <file.s19> -s 0xA0000 -e 0xAFFFE -v
```

## Memory Regions

Gen7 V2 radar memory partitions for CRC calculation:

| Region | Start Address | CRC End Address | Size | C Macro End |
|--------|---------------|-----------------|------|-------------|
| App Partition A | 0x00110000 | 0x0037FFF7 | 2,555,897 bytes | 0x0037FFF8 |
| App Partition B | 0x00420000 | 0x0068FFF7 | 2,555,897 bytes | 0x0068FFF8 |
| Bootloader (BTLD) | 0x00000400 | 0x0003FFEE | 261,616 bytes | 0x0003FFEF |
| RFE | 0x00040000 | 0x0006FFFE | 196,608 bytes | 0x0006FFFF |
| RFE Backup | 0x00070000 | 0x0009FFFE | 196,608 bytes | 0x0009FFFF |
| HSE | 0x003A0000 | 0x003FFFFE | 393,216 bytes | 0x003FFFFF |
| HSE Backup | 0x00690000 | 0x006EFFFE | 393,216 bytes | 0x006EFFFF |
| USC (User Cal) | 0x000A0000 | 0x000AFFFE | 65,536 bytes | 0x000AFFFF |
| SMC (System Cal) | 0x000F0000 | 0x0010FFFE | 131,072 bytes | 0x0010FFFF |

**Note**: CRC End Address = C Macro End - 1 (script uses inclusive addressing)

## Examples

Run from repository root directory:

```powershell or command prompt
# App Partition A
python tools\python\calculate_s19_crc32.py App.s19 -s 0x110000 -e 0x37FFF8

# App Partition B
python tools\python\calculate_s19_crc32.py App.s19 -s 0x420000 -e 0x68FFF8

# Bootloader (BTLD)
python tools\python\calculate_s19_crc32.py App.s19 -s 0x400 -e 0x3FFEF

# RFE
python tools\python\calculate_s19_crc32.py App.s19 -s 0x40000 -e 0x6FFFF

# RFE Backup
python tools\python\calculate_s19_crc32.py App.s19 -s 0x70000 -e 0x9FFFF

# HSE
python tools\python\calculate_s19_crc32.py App.s19 -s 0x3A0000 -e 0x3FFFFF

# HSE Backup
python tools\python\calculate_s19_crc32.py App.s19 -s 0x690000 -e 0x6EFFFF

# USC (User Calibration)
python tools\python\calculate_s19_crc32.py App.ptp -s 0xA0000 -e 0xAFFFF

# SMC (System Calibration)
python tools\python\calculate_s19_crc32.py App.s19 -s 0xF0000 -e 0x10FFFF
```
