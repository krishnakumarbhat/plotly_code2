"""This script takes in s19 files and reports flash memory usage statistics."""

import bincopy
import argparse
import os
import re
import json

parser = argparse.ArgumentParser()

parser.add_argument(
    "input_files", type=str, nargs="+", help="The files to include in the flash memory statistics"
)
parser.add_argument("-o", "--output", type=str, required=True, help="The path to the output file")
parser.add_argument("-s", "--flash_size", type=int, required=True, help="Flash size (MB)")
parser.add_argument("-g", "--ignored_gap", type=int, default=1024, required=False)
parser.add_argument(
    "-l",
    "--linker_script",
    type=str,
    default=None,
    required=False,
    help="Optional path to preprocessed common.ld for static region annotation",
)
parser.add_argument(
    "-n",
    "--region_names",
    type=str,
    default=None,
    required=False,
    help="Optional path to a JSON file mapping linker region keys to display names",
)

args = parser.parse_args()

merged_image = bincopy.BinFile()
file_at_address = {}
file_info = {}
total_size = 0
display_width = 0


def parse_static_regions_from_linker(linker_path: str, names: dict):
    """Parsing staic regions."""
    global total_size
    regions = []
    if not linker_path or not os.path.exists(linker_path):
        return regions

    start_re = re.compile(r"__([A-Z_]+)_FLASH_START_ADDRESS\s*=\s*0x([0-9A-Fa-f]+)")
    size_re = re.compile(r"__([A-Z_]+)_FLASH_(?:BLOCK_)?SIZE\s*=\s*0x([0-9A-Fa-f]+)")

    starts = {}
    sizes = {}
    try:
        with open(linker_path, "r") as lf:
            text = lf.read()
        for m in start_re.finditer(text):
            key = m.group(1)
            addr = int(m.group(2), 16)
            starts[key] = addr
        for m in size_re.finditer(text):
            key = m.group(1)
            sz = int(m.group(2), 16)
            sizes[key] = sz
    except Exception:
        return regions

    for key, disp in names.items():
        if key in starts and key in sizes:
            start = starts[key]
            end = start + sizes[key]
            regions.append({"name": disp, "start": start, "end": end})
    # Minimal: fill entire static regions with dummy data and update dictionaries
    for r in regions:
        length = max(0, (r["end"] - r["start"]) - 1)
        if length > 0:
            total_size += length
            merged_image.add_binary(b"\xFF" * length, r["start"])  # leave 1-byte gap at end
        file_at_address[r["start"]] = r["name"]
        file_info[r["name"]] = {
            "minimum_address": r["start"],
            "maximum_address": r["end"],
            "size": r["end"] - r["start"],  # 100% utilization in summary
        }
    return []


for file in args.input_files:
    filename = os.path.basename(file)

    binfile = bincopy.BinFile(file)
    binfile.fill(max_words=args.ignored_gap)
    total_file_size = 0
    for segment in binfile.segments:
        file_at_address[segment.address] = filename
        merged_image.segments.add(segment, overwrite=True)
        size = len(segment.data)
        total_file_size += size
        total_size += size
        display_width = max(display_width, len(f" {filename} - {size / 1024:0.2f} KB "))
    file_info[filename] = {
        "minimum_address": binfile.minimum_address,
        "maximum_address": binfile.maximum_address,
        "size": total_file_size,
    }


if args.region_names:
    with open(args.region_names, "r") as rn:
        loaded = json.load(rn)
        if isinstance(loaded, dict):
            names = loaded

static_regions = parse_static_regions_from_linker(args.linker_script, names)
for region in static_regions:
    file_info[region["name"]] = {
        "minimum_address": region["start"],
        "maximum_address": region["end"],
        "size": 0,
    }

    file_at_address[region["start"]] = region["name"]

size_mb = total_size / 1024 / 1024
size_pct = size_mb * 100 / args.flash_size
span_mb = (merged_image.maximum_address - merged_image.minimum_address) / 1024 / 1024
span_pct = span_mb * 100 / args.flash_size

with open(args.output, "w") as f:
    f.write(
        f"   Flash utilization: {size_pct:0.2f}% - {size_mb:0.2f} MB of {args.flash_size:0.2f} MB\n"
    )
    f.write(
        f"    Flash usage span: {span_pct:0.2f}% - {span_mb:0.2f} MB of {args.flash_size:0.2f} MB - 0x{merged_image.minimum_address:0>8X} to 0x{merged_image.maximum_address:0>8X}\n\n"
    )

    def format_file_info(name, size, min_addr, max_addr):
        """Format the file information for output."""
        span = max_addr - min_addr
        return f"{name:>20}: {size / 1024:7.2f} KB over {span / 1024:7.2f} KB span ({100 * size / span:6.2f}%) - 0x{min_addr:0>8X} to 0x{max_addr:0>8X}\n"

    f.write(
        format_file_info(
            "Total", total_size, merged_image.minimum_address, merged_image.maximum_address
        )
    )
    for file in sorted(file_info, key=lambda x: file_info[x]["maximum_address"]):
        size = file_info[file]["size"]
        min_addr = file_info[file]["minimum_address"]
        max_addr = file_info[file]["maximum_address"]
        f.write(format_file_info(file, size, min_addr, max_addr))
    f.write("\n")

    divider = "-" * display_width
    last_segment_end = 0
    for segment in merged_image.segments:
        if last_segment_end != segment.address:
            temp_str = "-" * display_width
            f.write(f"0x{last_segment_end:0>8X} +{divider}+\n")
            temp_str = f"Gap - {(segment.address - last_segment_end) / 1024:>0.2f} KB"
            f.write(f"{' ':>11}|{temp_str.center(display_width)}|\n")

        f.write(f"0x{segment.address:0>8X} +{divider}+\n")
        temp_str = f"{file_at_address[segment.address]} - {len(segment.data) / 1024:>0.2f} KB"
        f.write(f"{' ':>11}|{temp_str.center(display_width)}|\n")

        last_segment_end = segment.maximum_address

    f.write(f"0x{last_segment_end:0>8X} +{divider}+\n\n")

    f.write(
        f"Note: Gaps smaller than {args.ignored_gap} bytes in a single file are considered used flash memory"
    )
