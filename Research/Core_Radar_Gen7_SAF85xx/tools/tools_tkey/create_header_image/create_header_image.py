"""This script takes in s19 files, merges them, and creates a flattened image with a copy table."""

import glob
import bincopy
import argparse
import struct
import sys


def extract_data_from_m7App(symbol_name, target_address):
    """
    Extract structure data from the M7 application binary and write to flash.

    Args:
        symbol_name: The name of the symbol to extract from the map file (e.g., 'Radar_info_from_ptp')
        target_address: The destination flash address where the data should be written

    This function:
    1. Locates the m7App.elf.map file from the build output
    2. Parses the map file to find the symbol's RAM address and size
    3. Extracts the structure data from the m7App.s19 file using the RAM address
    4. Writes the extracted data to the specified target flash address in the header image
    """
    # Search for the map file in the Bazel output directory
    map_file_paths = glob.glob("bazel-out/**/m7App.elf.map", recursive=True)
    if not map_file_paths:
        print("ERROR: Could not find m7App.elf.map file")
        sys.exit(1)

    # Use the first matching map file found
    map_file_path = map_file_paths[0]
    radar_info_address = None  # RAM address from map file (source)
    radar_info_size = None

    # Parse the map file to find the symbol's RAM address and size
    # Map file format: <whitespace><symbol_name> <address> <size>
    # Example:     Radar_info_from_ptp 341b0015 000000b7
    with open(map_file_path, "r") as map_file:
        for line in map_file:
            # Search for the specified symbol in the map file
            # Line must start with whitespace and symbol must be the first token
            if symbol_name in line:
                parts = line.split()
                # Verify this is the correct format: first part is symbol name, followed by address and size
                if len(parts) >= 3 and parts[0] == symbol_name:
                    # Verify that parts[1] is a valid hexadecimal number
                    try:
                        radar_info_address = int(parts[1], 16)  # Source RAM address
                        radar_info_size = int(parts[2], 16)
                        break
                    except ValueError:
                        # Skip this line if parts[1] or parts[2] is not a valid hex number
                        continue

    # Extract the structure data from the S19 file at the RAM address
    radar_info_data = None
    for segment in s19_data.segments:
        segment_start = segment.address
        segment_end = segment.address + len(segment.data)

        # Check if the radar_info_address (RAM) falls within this segment's range
        if segment_start <= radar_info_address < segment_end:
            # Calculate the offset within the segment
            offset = radar_info_address - segment_start
            # Extract the data using the size from the map file
            radar_info_data = segment.data[offset : offset + radar_info_size]
            break

    # Write the extracted data to the target flash address in the header image
    header_image.add_binary(radar_info_data, target_address)


parser = argparse.ArgumentParser(description="Generate header image for Bazel rule")
parser.add_argument("--output", required=True, help="Output S19 file path")
parser.add_argument("--bmhdr_address", type=int, required=True, help="BmHeader address")
parser.add_argument("--magic_flag", required=True, help="Magic flag (4 bytes)")
parser.add_argument("--ram_entry_address", type=int, required=True, help="RAM entry address")
parser.add_argument("--start_address", type=int, required=True, help="App start address in flash")
parser.add_argument("--length", type=int, required=True, help="App length")
parser.add_argument(
    "--auth_hdr_addr", type=int, required=True, help="Authentication header address"
)

args = parser.parse_args()

header_image = bincopy.BinFile()

byteorder = "little"

ram_addr = args.ram_entry_address

# Find the S19 file
s19_file_paths = glob.glob("bazel-out/**/m7App.s19", recursive=True)
if not s19_file_paths:
    print("ERROR: Could not find m7App.s19 file")
    sys.exit(1)

s19_file_path = s19_file_paths[0]  # Take the first match
s19_data = bincopy.BinFile(s19_file_path)
# This is RAM address + 4, which typically points to the start address
target_address = ram_addr + 4

found = False
# Iterate through all segments in the S19 file
for segment in s19_data.segments:
    segment_start = segment.address
    segment_end = segment.address + len(segment.data)

    if segment_start <= target_address < segment_end and target_address + 3 < segment_end:
        # Found the segment containing our target address
        offset = target_address - segment_start
        data = segment.data[offset : offset + 4]

        # Convert the 4 bytes to a 32-bit integer using little-endian byte order
        # This value will be used as the start_address
        start_address = struct.unpack("<I", data)[0]
        found = True
        break

if not found:
    print(f"Error: Address 0x{target_address:X} not found in any segment")

bmhdr_addr = args.bmhdr_address

# Convert the magic flag from hex string to integer
mag_flag = int(args.magic_flag, 16)
mag_flag_bytes = mag_flag.to_bytes(4, byteorder)

entry_addr = start_address
entry_addr_bytes = entry_addr.to_bytes(4, byteorder)

# Define the target_handle as FBLBMHDR_TARGET_APPL
target_handle = 0x00000002
target_handle_bytes = target_handle.to_bytes(4, byteorder)

start_addr = args.start_address
block_start_addr_bytes = start_addr.to_bytes(4, byteorder)

block_length = args.length
block_length_bytes = block_length.to_bytes(4, byteorder)

auth_hdr_addr = args.auth_hdr_addr
auth_hdr_addr_bytes = auth_hdr_addr.to_bytes(4, byteorder)

presence_pattern = bytes.fromhex("736A293EFFFFFFFFFFFFFFFFFFFFFFFF")
pattern_address = args.bmhdr_address + 0x20


header_image.add_binary(mag_flag_bytes, bmhdr_addr)
header_image.add_binary(entry_addr_bytes, bmhdr_addr + 0x4)
header_image.add_binary(target_handle_bytes, bmhdr_addr + 0x8)
header_image.add_binary(block_start_addr_bytes, bmhdr_addr + 0xC)
header_image.add_binary(block_length_bytes, bmhdr_addr + 0x10)
header_image.add_binary(auth_hdr_addr_bytes, bmhdr_addr + 0x14)

header_image.add_binary(presence_pattern, pattern_address)

target_flash_address = 0x37FE04
extract_data_from_m7App("Radar_info_from_ptp", target_flash_address)

with open(args.output, "w") as f:
    f.write(header_image.as_srec())
