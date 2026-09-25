"""
S19 File CRC32 Calculator.

This script calculates the CRC32 checksum for S19 (Motorola S-record) files
using the zlib crc32b algorithm (polynomial 0xEDB88320).

Usage:
    python calculate_s19_crc32.py <s19_file_path>

Example:
    python calculate_s19_crc32.py bazel-bin/outputs/srr7p/m7App.s19
"""

import sys
import os
import argparse


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


def calculate_s19_crc32(file_path, start_addr=None, end_addr=None, verbose=False):
    """
    Calculate CRC32 for data in an S19 file within specified address range.

    Args:
        file_path: Path to the S19 file
        start_addr: Start address (inclusive) for CRC calculation. If None, starts from first record.
        end_addr: End address (exclusive) for CRC calculation. If None, ends at last record.
        verbose: If True, print detailed information about each record

    Returns:
        uint32 CRC32 checksum value
    """
    if not os.path.isfile(file_path):
        raise FileNotFoundError(f"File not found: {file_path}")

    all_data = bytearray()
    record_count = 0
    total_bytes = 0
    min_address = None
    max_address = None
    skipped_records = 0

    with open(file_path, "r") as f:
        for line_num, line in enumerate(f, 1):
            line = line.strip()
            if not line:
                continue

            result = parse_s19_record(line)
            if result:
                record_type, address, data = result
                record_end = address + len(data)

                # Track actual address range in file
                if min_address is None:
                    min_address = address
                else:
                    min_address = min(min_address, address)

                if max_address is None:
                    max_address = record_end
                else:
                    max_address = max(max_address, record_end)

                # Filter by address range if specified
                # start_addr is inclusive, end_addr is inclusive
                if start_addr is not None and record_end <= start_addr:
                    skipped_records += 1
                    if verbose:
                        print(
                            f"Line {line_num}: S{record_type} record at 0x{address:08X} "
                            f"SKIPPED (before start address)"
                        )
                    continue

                # For inclusive end_addr, skip if address > end_addr
                if end_addr is not None and address > end_addr:
                    skipped_records += 1
                    if verbose:
                        print(
                            f"Line {line_num}: S{record_type} record at 0x{address:08X} "
                            f"SKIPPED (after end address)"
                        )
                    continue

                # Handle partial overlap with start_addr
                data_offset = 0
                if start_addr is not None and address < start_addr < record_end:
                    data_offset = start_addr - address
                    if verbose:
                        print(
                            f"Line {line_num}: S{record_type} record at 0x{address:08X}, "
                            f"using {len(data) - data_offset} bytes (partial overlap with start)"
                        )

                # Handle partial overlap with end_addr (inclusive)
                data_end = len(data)
                if end_addr is not None and address <= end_addr < record_end:
                    # Include up to and including byte at end_addr
                    data_end = (end_addr - address) + 1
                    if verbose:
                        print(
                            f"Line {line_num}: S{record_type} record at 0x{address:08X}, "
                            f"using {data_end - data_offset} bytes (partial overlap with end)"
                        )

                # Add data within range
                data_slice = data[data_offset:data_end]
                all_data.extend(data_slice)
                record_count += 1
                total_bytes += len(data_slice)

                if verbose and data_offset == 0 and data_end == len(data):
                    print(
                        f"Line {line_num}: S{record_type} record at 0x{address:08X}, "
                        f"{len(data)} bytes"
                    )

    # Calculate CRC32 over all concatenated data (default initial value is 0)
    crc32_value = crc32_zlib(all_data, 0x00000000)

    print(f"\nS19 File: {file_path}")
    print(
        f"File address range: 0x{min_address:08X} - 0x{max_address:08X}"
        if min_address
        else "No data records found"
    )
    if start_addr is not None or end_addr is not None:
        filter_start = f"0x{start_addr:08X}" if start_addr is not None else "start"
        filter_end = f"0x{end_addr:08X}" if end_addr is not None else "end"
        print(f"CRC address range: {filter_start} - {filter_end}")
    print(f"Records processed: {record_count} (skipped: {skipped_records})")
    print(f"Total data bytes: {total_bytes}")
    print(f"CRC32 (zlib): 0x{crc32_value:08X} ({crc32_value})")

    return crc32_value


def main():
    """Parse command line arguments and calculate CRC32 for S19 file."""
    parser = argparse.ArgumentParser(
        description="Calculate CRC32 checksum for S19 files using zlib crc32b algorithm",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s m7App.s19
  %(prog)s bazel-bin/outputs/srr7p/m7App.s19 --verbose
  %(prog)s output.s19 --start 0x00400000 --end 0x00500000
  %(prog)s output.s19 -s 0x400000 -e 0x500000 -v
        """,
    )

    parser.add_argument("s19_file", help="Path to the S19 file")
    parser.add_argument(
        "-s",
        "--start",
        type=lambda x: int(x, 0),
        help="Start address (inclusive) for CRC calculation (hex: 0x... or decimal)",
    )
    parser.add_argument(
        "-e",
        "--end",
        type=lambda x: int(x, 0),
        help="End address (inclusive) for CRC calculation (hex: 0x... or decimal)",
    )
    parser.add_argument(
        "--end-exclusive",
        action="store_true",
        help="Treat end address as exclusive instead of inclusive",
    )
    parser.add_argument(
        "-v",
        "--verbose",
        action="store_true",
        help="Print detailed information about each record",
    )

    args = parser.parse_args()

    try:
        # End address is inclusive by default, so we DON'T add 1
        # For 0xAFFFF, we calculate up to and including byte at 0xAFFFF
        # which means exclusive end is 0xB0000, but then we process
        # the data extraction logic handles this
        end_addr = args.end
        if end_addr is not None and not args.end_exclusive:
            # Keep end_addr as-is for inclusive interpretation
            # The calculate function will handle it properly
            pass

        calculate_s19_crc32(
            args.s19_file,
            start_addr=args.start,
            end_addr=end_addr,
            verbose=args.verbose,
        )
        return 0

    except FileNotFoundError as e:
        print(f"Error: {e}", file=sys.stderr)
        return 1

    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        import traceback

        traceback.print_exc()
        return 1


if __name__ == "__main__":
    sys.exit(main())
