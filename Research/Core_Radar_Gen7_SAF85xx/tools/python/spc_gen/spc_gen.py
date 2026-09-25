"""This script generates a sensor position calibration (SPC) based on the configuration specified."""

import bincopy
import argparse
import json
import sys

# Section data structures and default values
h1 = {
    "offset": 0x0,
    "size": {"offset": 0, "size": 4, "value": 0},
    "checksum": {"offset": 4, "size": 4, "value": 0},
}

h2 = {
    "offset": 0x8,
    "customer": {"offset": 0, "size": 1, "value": 1},
    "cal_type": {"offset": 1, "size": 1, "value": 1},
    "platform": {"offset": 2, "size": 1, "value": 0},
    "num_sections": {"offset": 3, "size": 1, "value": 1},
    "compatibility": {"offset": 4, "size": 1, "value": 3},
    "version": {"offset": 6, "size": 2, "value": 1},
    "size": {"offset": 8, "size": 4, "value": 0},
}

h3 = {
    "offset": 0x1C,
    "section_num": {"offset": 0, "size": 1, "value": 1},
    "compatibility": {"offset": 4, "size": 2, "value": 1},
    "version": {"offset": 6, "size": 2, "value": 1},
    "size": {"offset": 8, "size": 4, "value": 0},
}

spc_data = {
    "offset": 0x28,
    "fixed_position": {"offset": 0, "size": 1, "value": 0},
    "position_id_map": {
        "offset": 1,
        "size": 16,
        "value": [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16],
    },
}


def write_value(key, section, output, config, signed=False):
    """Write a value to the SPC file based on the section contents."""
    value = config.get(key, section[key]["value"])
    address = section["offset"] + section[key]["offset"]
    size = section[key]["size"]
    if size <= 4:
        output.add_binary(value.to_bytes(size, "little", signed=signed), address)
    else:
        output.add_binary(bytes(value), address)


def int_or_hex(value):
    """Returns value if an integer is provided or converts a string from hex to integer."""
    if isinstance(value, int):
        return value
    if isinstance(value, str):
        return int(value, 16)


def write_spc(config):
    """Writes the SPC based on the configuration specified."""
    output = bincopy.BinFile()

    # Add H2 data
    write_value("customer", h2, output, config)
    write_value("cal_type", h2, output, config)
    write_value("platform", h2, output, config)
    write_value("num_sections", h2, output, config)
    write_value("compatibility", h2, output, config)
    write_value("version", h2, output, config)

    # Add H3 data
    write_value("section_num", h3, output, config)
    write_value("compatibility", h3, output, config)
    write_value("version", h3, output, config)

    # Add SPC data
    write_value("fixed_position", spc_data, output, config)
    write_value("position_id_map", spc_data, output, config)

    # Align to 4 bytes
    if output.maximum_address % 4 != 0:
        output.add_binary(bytes(4 - (output.maximum_address % 4)), output.maximum_address)

    # Add section sizes
    h2["size"]["value"] = output.maximum_address - h2["offset"]
    write_value("size", h2, output, config)
    h3["size"]["value"] = output.maximum_address - h3["offset"]
    write_value("size", h3, output, config)

    # Fill any unused bytes with 0
    output.fill(value=b"\x00")

    # Calculate checksum
    file_checksum = 0
    for byte in output.as_binary():
        file_checksum += byte
    file_checksum = -file_checksum

    # Add file size and checksum
    h1["size"]["value"] = output.maximum_address - h2["offset"]
    write_value("size", h1, output, config)
    h1["checksum"]["value"] = file_checksum
    write_value("checksum", h1, output, config, signed=True)

    # If fill parameter is true, set the last byte then use bincopy to fill with 0xff
    if "fill" in config and config["fill"]:
        fill_align = 0x10000 - 1
        if "fill_align" in config:
            fill_align = int_or_hex(config["fill_align"])
        output.add_binary(b"\xff", fill_align)
        output.fill()

    # Offset to the defined start address
    offset_output = bincopy.BinFile()
    start_address = 0
    if "start_address" in config:
        start_address = int_or_hex(config["start_address"])
    offset_output.add_binary(output.as_binary(), start_address)
    offset_output.header = "SPC"
    with open(config["filename"], "w") as f:
        f.write(offset_output.as_srec())

    return config


parser = argparse.ArgumentParser()

parser.add_argument(
    "config_files",
    type=str,
    nargs="*",
    help="Configuration file(s) for sensor position calibration",
)
parser.add_argument(
    "-f", "--filename", type=str, default="spc.s19", help="Output file base name (no extension)"
)
parser.add_argument("-p", "--position", type=int, default=1, help="Sensor position ID")
parser.add_argument("-s", "--start_address", type=str, default="0", help="Start address in hex")
parser.add_argument("--fill", default=False, action="store_true", help="Fill the output file")
parser.add_argument("--fill_align", default=0, type=int, help="Fill s19 to alignment")

args = parser.parse_args()

if len(sys.argv) <= 1:
    config = {}
    config["filename"] = input("Output file base name (no extension): ")
    config["fixed_position"] = int(input("Sensor position ID, or 0 for pin position map: "))
    config["start_address"] = input("Start address in hex: ")
    config["fill"] = input("Fill? (y/n): ").lower() in ["y", "yes"]

    config = write_spc(config)
    with open(config["filename"] + ".json", "w") as file:
        json.dump(config, file, indent=4)
elif len(args.config_files) == 0:
    config = {}
    config["filename"] = args.filename
    config["fixed_position"] = args.position
    config["start_address"] = int(args.start_address, 0)
    config["fill"] = args.fill
    config["fill_align"] = args.fill_align

    config = write_spc(config)
    with open(config["filename"] + ".json", "w") as file:
        json.dump(config, file, indent=4)
else:
    for config_file in args.config_files:
        with open(config_file, "r") as file:
            config = json.load(file)
        write_spc(config)
