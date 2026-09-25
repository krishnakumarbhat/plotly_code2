"""This script takes in s19 files and reports flash memory usage statistics."""

import bincopy
import argparse
import os
import re

from tools.python.platformHealthMetrics import platformHealthStats

parser = argparse.ArgumentParser()

parser.add_argument(
    "input_files", type=str, nargs="+", help="The files to include in the flash memory statistics"
)
parser.add_argument("-o", "--output", type=str, required=True, help="The path to the output file")
parser.add_argument("-s", "--flash_size", type=int, required=True, help="Flash size (MB)")
parser.add_argument("-g", "--ignored_gap", type=int, default=1024, required=False)
parser.add_argument(
    "-p",
    "--platformHealth",
    dest="platformHealthMetrics",
    action="store_true",
    default=False,
    help="A flag to trigger sending metrics to Platform Health",
)
parser.add_argument(
    "-v",
    "--variant",
    dest="variant",
    type=str,
    action="store",
    default=None,
    help="A variant to tag the Platform Health metrics with",
)
parser.add_argument(
    "--buildVariant",
    dest="buildVariant",
    type=str,
    action="store",
    default=None,
    help="The build variant (satellite, standalone) to tag the Platform Health metrics with",
)
parser.add_argument(
    "-b",
    "--branch",
    dest="branch",
    type=str,
    action="store",
    default=None,
    help="A branch to tag the Platform Health metrics with",
)
args = parser.parse_args()

merged_image = bincopy.BinFile()
file_at_address = {}
file_info = {}
total_size = 0
display_width = 0

commitid = "test"  # need to fill this out

if args.platformHealthMetrics:
    platformHealthMetrics = platformHealthStats(
        commitid=commitid, project="Core_Radar_Gen7_AWR294x", branch=args.branch
    )

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

# If Platform Health option is enabled
if args.platformHealthMetrics:

    # Parse flash file
    flash_memory = []
    with open(args.output, "r") as file:
        for line in file:
            # Parse utilization lines
            match = re.match(
                r"^\s*(\S.*?):\s+([\d.]+)%\s+-\s+([\d.]+)\s+MB\s+of\s+([\d.]+)\s+MB", line
            )
            if match:
                name, percentage, used_mb, total_mb = match.groups()
                flash_memory.append(
                    {
                        "name": name.strip(),
                        "percentage": float(percentage),
                        "used_mb": float(used_mb),
                        "total_mb": float(total_mb),
                    }
                )
                continue

            # Parse total lines and addressed sections
            match = re.match(
                r"^\s*(\S.*?):\s+([\d.]+)\s+KB\s+over\s+([\d.]+)\s+KB\s+span\s*\(\s*([\d.]+)%\s*\)\s+-\s+0x([\da-fA-F]+)\s+to\s+0x([\da-fA-F]+)",
                line,
            )
            if match:
                name, used_kb, span_kb, percentage, start_address, end_address = match.groups()
                flash_memory.append(
                    {
                        "name": name.strip(),
                        "used_kb": float(used_kb),
                        "span_kb": float(span_kb),
                        "percentage": float(percentage),
                        "start_address": f"0x{start_address}",
                        "end_address": f"0x{end_address}",
                    }
                )
                continue

            # Parse individual flash sections
            match = re.match(
                r"^\s*(\S.*?):\s+([\d.]+)\s+KB\s+over\s+([\d.]+)\s+KB\s+span\s+\(([\d.]+)%\)", line
            )
            if match:
                name, used_kb, span_kb, percentage = match.groups()
                flash_memory.append(
                    {
                        "name": name.strip(),
                        "used_kb": float(used_kb),
                        "span_kb": float(span_kb),
                        "percentage": float(percentage),
                    }
                )

                continue

    # Format Flash Memory data for Platform Health
    for flash in flash_memory:
        metric_name = "core_radar.gen7.memoryStats.flash"
        metric_tags = {
            "name": flash["name"],
            "variant": args.variant,
            "buildVariant": args.buildVariant,
        }

        if "used_mb" in flash:
            platformHealthMetrics.sendMetric(
                metric_name,
                float(flash["total_mb"]),
                {**metric_tags, "type": "total_mb"},
            )
            platformHealthMetrics.sendMetric(
                metric_name,
                float(flash["used_mb"]),
                {**metric_tags, "type": "used_mb"},
            )

        elif "used_kb" in flash:
            platformHealthMetrics.sendMetric(
                metric_name,
                float(flash["span_kb"]),
                {**metric_tags, "type": "span_kb"},
            )
            platformHealthMetrics.sendMetric(
                metric_name,
                float(flash["used_kb"]),
                {**metric_tags, "type": "used_kb"},
            )
