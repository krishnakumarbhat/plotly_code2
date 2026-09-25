"""
S19 File CRC32 Patcher for M7 Code Section.

This script:
1. Reads the m7App.elf.map file to extract __M7_code_start_c0 and __M7_code_end_c0 addresses
2. Calculates the CRC32 checksum for the specified address range in the S19 file
3. Patches the CRC32 value at the requested address in the output S19 file

This is integrated into the Bazel build system to automatically calculate and embed
the M7 code section CRC in every build.

Usage:
    python patch_s19_crc.py --map m7App.elf.map --input app_no_rfe.s19 --output app_no_rfe_crc.s19
"""

import sys
import os
import argparse
import re
import struct


# Import the crc32_zlib function from the existing calculate_s19_crc32 module
# We'll use the same algorithm to ensure consistency
def crc32_zlib(data, initial_value=0x00000000):
    """
    Calculate CRC32 using zlib crc32b algorithm.

    This implementation matches the crc32_sw function in crc_calc.c:
    - Polynomial: 0xEDB88320
    - Initial value: 0 for new calculation (inverted to 0xFFFFFFFF internally)
    - Final: return ~crc (single inversion, NOT double XOR like standard zlib)

    Args:
        data: bytes or bytearray to calculate CRC over
        initial_value: initial CRC value (default: 0 for new calculation,
                       or previous CRC result for continued calculation)

    Returns:
        uint32 CRC32 checksum value
    """
    crc = ~initial_value & 0xFFFFFFFF

    for byte in data:
        crc = crc ^ byte
        for _ in range(8):
            mask = -(crc & 1)
            crc = ((crc >> 1) ^ (0xEDB88320 & mask)) & 0xFFFFFFFF

    # C code returns ~crc (not ~crc ^ 0xFFFFFFFF)
    return ~crc & 0xFFFFFFFF


def parse_map_file(map_file_path):
    """
    Parse the m7App.elf.map file to extract symbol addresses.

    Supports multiple map file formats:
      Wind River Diab:  __M7_code_start_c0 340c5000
      GCC/LD style:     __M7_code_start_c0 = 0x00110000

    Args:
        map_file_path: Path to the .map file

    Returns:
        dict: Dictionary with 'start' and 'end' addresses (as integers)
    """
    if not os.path.isfile(map_file_path):
        raise FileNotFoundError(f"Map file not found: {map_file_path}")

    # Regex patterns to match symbol definitions in the map file
    # Wind River Diab format:  __M7_code_start_c0 340c5000
    # GCC/LD format:           __M7_code_start_c0 = 0x00110000
    start_pattern = re.compile(r"^\s*__M7_code_start_c0\s+(?:=\s*)?(?:0x)?([0-9a-fA-F]+)\s*$")
    end_pattern = re.compile(r"^\s*__M7_code_end_c0\s+(?:=\s*)?(?:0x)?([0-9a-fA-F]+)\s*$")

    addresses = {}

    with open(map_file_path, "r", encoding="utf-8", errors="ignore") as f:
        for line in f:
            # Check for start address
            start_match = start_pattern.search(line)
            if start_match:
                addresses["start"] = int(start_match.group(1), 16)

            # Check for end address
            end_match = end_pattern.search(line)
            if end_match:
                addresses["end"] = int(end_match.group(1), 16)

            # Early exit if both found
            if "start" in addresses and "end" in addresses:
                break

    if "start" not in addresses:
        raise ValueError(f"Symbol __M7_code_start_c0 not found in {map_file_path}")

    if "end" not in addresses:
        raise ValueError(f"Symbol __M7_code_end_c0 not found in {map_file_path}")

    return addresses


def parse_s19_record(line):
    """
    Parse a single S19 record line.

    S19 format:
    - S0: Header record (optional, ignored for CRC)
    - S1: Data record with 16-bit address
    - S2: Data record with 24-bit address
    - S3: Data record with 32-bit address
    - S5: Count record (optional, ignored for CRC)
    - S7/S8/S9: Start address record (ignored for CRC)

    Returns:
        tuple: (record_type, address, data_bytes) or None if not a data record
    """
    if not line.startswith("S"):
        return None

    record_type = line[1]

    # Only process data records (S1, S2, S3)
    if record_type not in ["1", "2", "3"]:
        return None

    try:
        # Byte count (includes address, data, and checksum)
        byte_count = int(line[2:4], 16)

        # Determine address size based on record type
        addr_size = {"1": 4, "2": 6, "3": 8}[record_type]  # Characters (2 per byte)

        # Extract address
        address = int(line[4 : 4 + addr_size], 16)

        # Extract data (exclude address and checksum)
        data_start = 4 + addr_size
        data_end = 4 + (byte_count * 2) - 2  # -2 for checksum
        data_hex = line[data_start:data_end]

        # Convert hex string to bytes
        data_bytes = bytes.fromhex(data_hex)

        return (record_type, address, data_bytes)

    except (ValueError, IndexError) as e:
        print(f"Warning: Failed to parse line: {line.strip()} - {e}", file=sys.stderr)
        return None


def calculate_s19_crc32(file_path, start_addr, end_addr):
    """
    Calculate CRC32 for data in an S19 file within specified address range.

    Args:
        file_path: Path to the S19 file
        start_addr: Start address (inclusive) for CRC calculation
        end_addr: End address (inclusive) for CRC calculation

    Returns:
        uint32 CRC32 checksum value
    """
    if not os.path.isfile(file_path):
        raise FileNotFoundError(f"File not found: {file_path}")

    all_data = bytearray()

    with open(file_path, "r") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue

            result = parse_s19_record(line)
            if result:
                record_type, address, data = result
                record_end = address + len(data)

                # Filter by address range
                if record_end <= start_addr:
                    continue
                if address > end_addr:
                    continue

                # Handle partial overlap with start_addr
                data_offset = 0
                if address < start_addr < record_end:
                    data_offset = start_addr - address

                # Handle partial overlap with end_addr (inclusive)
                data_end_idx = len(data)
                if address <= end_addr < record_end:
                    # Include up to and including byte at end_addr
                    data_end_idx = (end_addr - address) + 1

                # Add data within range
                data_slice = data[data_offset:data_end_idx]
                all_data.extend(data_slice)

    # Calculate CRC32 over all concatenated data
    crc32_value = crc32_zlib(all_data, 0x00000000)
    return crc32_value


def create_s19_record(record_type, address, data):
    """
    Create an S19 record with proper formatting and checksum.

    Args:
        record_type: '1', '2', or '3'
        address: Address for this record
        data: bytes to include in the record

    Returns:
        str: Complete S19 record line (without newline)
    """
    # Determine address size
    addr_sizes = {"1": 2, "2": 3, "3": 4}  # Bytes
    addr_size = addr_sizes[record_type]

    # Byte count includes: address bytes + data bytes + checksum byte
    byte_count = addr_size + len(data) + 1

    # Format address with appropriate width
    addr_format = {
        "1": f"{address:04X}",
        "2": f"{address:06X}",
        "3": f"{address:08X}",
    }[record_type]

    # Build the record without checksum
    record_no_checksum = f"S{record_type}{byte_count:02X}{addr_format}"

    # Add data
    for byte in data:
        record_no_checksum += f"{byte:02X}"

    # Calculate checksum (sum of all bytes after 'S' record type, then one's complement)
    checksum = 0
    # Start from position 2 (skip 'S' and record type)
    for i in range(0, len(record_no_checksum) - 2, 2):
        checksum += int(record_no_checksum[2 + i : 2 + i + 2], 16)

    checksum = (~checksum) & 0xFF

    return f"{record_no_checksum}{checksum:02X}"


def patch_s19_with_crc(input_s19, output_s19, crc_address, crc_value):
    """
    Patch the S19 file to insert CRC value at specified address.

    This function:
    1. Reads the input S19 file
    2. Finds or creates a record that contains the CRC address
    3. Writes the CRC value (4 bytes, big-endian) at that address
    4. Writes the modified S19 file to output

    Args:
        input_s19: Path to input S19 file
        output_s19: Path to output S19 file
        crc_address: Address where CRC should be written (e.g., 0x0037FED0)
        crc_value: 32-bit CRC value to write
    """
    if not os.path.isfile(input_s19):
        raise FileNotFoundError(f"Input file not found: {input_s19}")

    # Read all records
    records = []
    with open(input_s19, "r") as f:
        for line in f:
            line = line.strip()
            if line:
                records.append(line)

    # CRC value as little-endian bytes (4 bytes) to match ARM Cortex-M7 byte order
    crc_bytes = struct.pack("<I", crc_value)

    # Track which CRC bytes are already patched. This allows patching when the
    # 4-byte CRC range spans multiple S-records.
    patched_mask = [False, False, False, False]
    modified_records = []

    for record in records:
        result = parse_s19_record(record)

        if result:
            record_type, address, data = result
            record_end = address + len(data)

            # Check if this record overlaps with CRC address range [crc_address, crc_address + 3]
            crc_end_exclusive = crc_address + 4
            if address < crc_end_exclusive and crc_address < record_end:
                data_list = bytearray(data)

                # Patch each CRC byte that falls into this record.
                for i in range(4):
                    byte_addr = crc_address + i
                    if address <= byte_addr < record_end:
                        data_list[byte_addr - address] = crc_bytes[i]
                        patched_mask[i] = True

                # Recreate the record
                new_record = create_s19_record(record_type, address, bytes(data_list))
                modified_records.append(new_record)
            else:
                # Keep original record
                modified_records.append(record)
        else:
            # Non-data record (S0, S5, S7, S8, S9) - keep as-is
            modified_records.append(record)

    # If one or more CRC bytes were not present in existing records, create a new
    # S3 record containing all 4 CRC bytes at the designated address.
    if not all(patched_mask):
        # Create a new S3 record (32-bit address) for the CRC
        new_record = create_s19_record("3", crc_address, crc_bytes)

        # Insert before the termination record (S7/S8/S9)
        insert_index = len(modified_records)
        for i in range(len(modified_records) - 1, -1, -1):
            if modified_records[i].startswith(("S7", "S8", "S9")):
                insert_index = i
            else:
                break

        modified_records.insert(insert_index, new_record)

    # Write modified records to output
    with open(output_s19, "w") as f:
        for record in modified_records:
            f.write(record + "\n")


def main():
    """Main entry point for the CRC patching tool."""
    parser = argparse.ArgumentParser(
        description="Calculate M7 code CRC and patch it into S19 file",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s --map m7App.elf.map --input app.s19 --output app_with_crc.s19
    %(prog)s --map m7App.elf.map --input app_no_rfe.s19 --output app_no_rfe_with_crc.s19
  %(prog)s --map bazel-out/.../m7App.elf.map --input app.s19 --output app_crc.s19 --crc-address 0x0037FED0
        """,
    )

    parser.add_argument("--map", required=True, help="Path to m7App.elf.map file")
    parser.add_argument(
        "--m7-s19",
        required=True,
        help="Path to m7App.s19 file (contains data at RAM addresses for CRC calculation)",
    )
    parser.add_argument(
        "--input",
        required=True,
        help="Path to input S19 file (e.g., app.s19) to patch CRC into",
    )
    parser.add_argument("--output", required=True, help="Path to output S19 file with CRC patched")
    parser.add_argument(
        "--crc-address",
        type=lambda x: int(x, 0),
        default=0x0037FED0,
        help="Address where CRC should be stored in the output S19 (default: 0x0037FED0)",
    )
    parser.add_argument("-v", "--verbose", action="store_true", help="Print detailed information")

    args = parser.parse_args()

    try:
        # Step 1: Parse the map file to get code section addresses
        # print(f"Parsing map file: {args.map}")
        addresses = parse_map_file(args.map)
        start_addr = addresses["start"]
        end_addr = addresses["end"]

        # print(f"  __M7_code_start_c0 = 0x{start_addr:08X}")
        # print(f"  __M7_code_end_c0   = 0x{end_addr:08X}")

        # Step 2: Calculate CRC from m7App.s19 (which has data at RAM addresses)
        # The map file symbols are RAM addresses, and m7App.s19 contains data at
        # those same RAM addresses. The flash S19 (--input) has different addresses
        # due to the create_flash_image copy-table remapping.
        # NOTE: __M7_code_end_c0 is a one-past-the-end address (like C++ .end()),
        # so we subtract 1 to get the last inclusive byte for CRC calculation.
        crc_end_addr = end_addr - 1
        # print(f"\nCalculating CRC32 from {args.m7_s19}")
        # print(f"  Address range: 0x{start_addr:08X} - 0x{crc_end_addr:08X} (inclusive)")
        # print( f"  Code size: {crc_end_addr - start_addr + 1} bytes (0x{crc_end_addr - start_addr + 1:X})")
        crc_value = calculate_s19_crc32(args.m7_s19, start_addr, crc_end_addr)
        # print(f"  CRC32 = 0x{crc_value:08X} ({crc_value})")

        # Step 3: Patch the CRC into the S19 file
        # print(f"\nPatching CRC at address 0x{args.crc_address:08X}...")
        patch_s19_with_crc(args.input, args.output, args.crc_address, crc_value)

        print(f"  Output written to: {args.output}")
        # print("\nCRC patching completed successfully!")

        return 0

    except FileNotFoundError as e:
        print(f"Error: {e}", file=sys.stderr)
        return 1

    except ValueError as e:
        print(f"Error: {e}", file=sys.stderr)
        return 1

    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        import traceback

        traceback.print_exc()
        return 1


if __name__ == "__main__":
    sys.exit(main())
