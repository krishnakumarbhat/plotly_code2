"""This script takes in s19 files, merges them, and creates a flattened image with a copy table."""

import bincopy
import argparse
import zlib

parser = argparse.ArgumentParser()

parser.add_argument(
    "input_files", type=str, nargs="+", help="The files to include in the flash image"
)
parser.add_argument(
    "-o", "--output", type=str, required=True, help="The the output flash image file name"
)
parser.add_argument(
    "-p", "--pattern", type=str, required=True, help="Pattern to be used for presence pattern"
)
parser.add_argument("-a", "--alignment", type=int, default=4)
parser.add_argument("-e", "--entry-address", type=int, default=None)
parser.add_argument("-s", "--start-address", type=int, default=None)
parser.add_argument("-le", "--little-endian", action="store_true")
parser.add_argument("-be", "--big-endian", dest="little-endian", action="store_false")

args = parser.parse_args()

if args.little_endian:
    byteorder = "little"
else:
    byteorder = "big"

merged_image = bincopy.BinFile()
flash_image = bincopy.BinFile()

for file in args.input_files:
    merged_image.add_file(file)
    if args.entry_address is None:
        args.entry_address = merged_image.execution_start_address

pattern_bytes = args.pattern.encode("ascii").ljust(4)[:4]
address_bytes = args.entry_address.to_bytes(4, byteorder)
num_seg_bytes = len(merged_image.segments).to_bytes(4, byteorder)

flash_image.add_binary(pattern_bytes, 0x0)
flash_image.add_binary(pattern_bytes, 0x4)
flash_image.add_binary(address_bytes, 0x8)
flash_image.add_binary(num_seg_bytes, 0xC)

table_address = 0x10
data_address = 0x10 + 0x10 * len(merged_image.segments)

for segment in merged_image.segments:
    data_length = len(segment.data)

    if data_length % args.alignment == 0:
        padding = 0
    else:
        padding = args.alignment - (data_length % args.alignment)

    for _x in range(0, padding):
        segment.data.append(0xFF)
    data_length = data_length + padding

    source_offset_bytes = data_address.to_bytes(4, byteorder)
    dest_address_bytes = segment.address.to_bytes(4, byteorder)
    size_bytes = data_length.to_bytes(4, byteorder)
    crc_bytes = zlib.crc32(segment.data).to_bytes(4, byteorder)

    flash_image.add_binary(source_offset_bytes, table_address + 0x0)
    flash_image.add_binary(dest_address_bytes, table_address + 0x4)
    flash_image.add_binary(size_bytes, table_address + 0x8)
    flash_image.add_binary(crc_bytes, table_address + 0xC)
    flash_image.add_binary(segment.data, data_address)

    table_address = table_address + 0x10
    data_address = data_address + data_length

if len(flash_image.segments) > 1:
    print("Warning: Flash image has more than one segment")

if args.start_address is not None:
    data = flash_image.as_binary()
    flash_image = bincopy.BinFile()
    flash_image.add_binary(data, args.start_address)

with open(args.output, "w") as f:
    f.write(flash_image.as_srec())
