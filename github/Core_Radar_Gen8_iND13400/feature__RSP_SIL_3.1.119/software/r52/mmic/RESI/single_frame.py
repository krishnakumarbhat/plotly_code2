"""
Python script to do injection using the RESI board.

This script takes a directory of mat files if provided or the default location defined in this script
    Then converts this mat files into bin files that are eventually loaded one frame at a time into RESI board for
    resimulation.
"""
import os
import h5py
import shutil
import socket
import time
import requests
import numpy as np
import sys
from pathlib import Path

# config
IS_ADC_CHIRP_VISUALIZATION_NEEDED = True

# Constants
SCRIPT_DIR = Path(__file__).resolve().parent
DEFAULT_MAT_DIR = SCRIPT_DIR / "mat_files"
OUTPUT_DIR = SCRIPT_DIR / "output_bins"
MAP_FILE_PATH = SCRIPT_DIR / "../../../../bazel-bin/outputs/flr8/r52App.elf.map"
TARGET_IP = "192.168.1.71"
TARGET_IP_RESI = "192.168.1.99"
TARGET_PORT = 5557
SOURCE_PORT = 65000
WAIT_VALUE = 0x0000AAAA
NEW_VALUE_AFTER_SUCCESS = 0x0000BBBB
RESI_STATE_SYMBOL = "RESI_State"
MAX_FRAMES_ALLOWED_SYMBOL = "max_number_of_resi_frames"
CHANNELS = 4
TIMEOUT = 500
TRANSPOSE_MODE = "rx_samples_chirps"
CLEAN_ON_INTERRUPT = True  # Toggle this if you don't want to delete output_bins

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("", SOURCE_PORT))
sock.settimeout(2)

all_bin_paths = []


def extract_address_from_map(map_file_path, symbol_name):
    """
    Extract the address of a string if available in the mapfile.
    """
    symbol_name = symbol_name.lower()
    with open(map_file_path, "r") as file:
        for line in file:
            if symbol_name not in line.lower():
                continue
            if any(section in line.lower() for section in [".bss", ".data", ".text"]):
                continue
            parts = line.strip().split()
            if len(parts) >= 3 and parts[0].lower() == symbol_name:
                addr = parts[1]
                if addr and len(addr) == 8 and all(c in "0123456789abcdefABCDEF" for c in addr):
                    return int(addr, 16)
    return None


def send_and_receive(payload):
    """
    Send and recieve packets over xcp.
    """
    sock.sendto(payload, (TARGET_IP, TARGET_PORT))
    try:
        data, addr = sock.recvfrom(1024)
        return data
    except socket.timeout:
        return None


def xcp_connect():
    """
    Connect the XCP.
    """
    resp = send_and_receive(bytes.fromhex("02 00 00 00 ff 00"))
    return resp and b"\xff" in resp


def xcp_disconnect():
    """
    Disconnect the XCP.
    """
    send_and_receive(bytes.fromhex("02 00 00 00 fe 00"))


def set_mta(address):
    """
    Abstracted so that the SET MTA can work outside of the exact byte information.
    """
    return bytes([0x02, 0x00, 0x00, 0x00, 0xF6, 0x00, 0x00, 0x00]) + address.to_bytes(4, "little")


def upload():
    """
    Abstracted to skip the byte information.
    """
    return bytes([0x02, 0x00, 0x00, 0x00, 0xF5, 0x04])


def download(value):
    """
    Downloading a value: abstracted to skip the byte information.
    """
    return bytes([0x02, 0x00, 0x00, 0x00, 0xF0, 0x04]) + value.to_bytes(4, "little")


def read_memory(address):
    """
    Read any memory addresses and return the value.
    """
    send_and_receive(set_mta(address))
    resp = send_and_receive(upload())
    return (
        int.from_bytes(resp[-4:], "little")
        if resp and b"\xff" in resp and len(resp) >= 8
        else None
    )


def write_memory(address, value):
    """
    Write a memory addresses with a value.
    """
    send_and_receive(set_mta(address))
    send_and_receive(download(value))


def trigger_resi(entry):
    """
    Trigger the RESI.

    Input: entry: chirp meta data.
    Output: none.
    """
    file_path = Path(entry["bin_file"])
    samples = entry["samples"]
    chirps = entry["chirps"]
    # samples = 1024
    # chirps = 1020
    # file_path = "bin1.bin"
    data = np.fromfile(file_path, dtype="uint16").reshape((chirps, CHANNELS, samples))
    data = data.transpose(0, 2, 1)
    mipidata = data.tobytes()

    # metadata = {
    #     'channels': CHANNELS,
    #     'delay': 4,
    #     'frames': 1,
    #     'frameinfo': str([{'samples': samples, 'chirps': chirps}])
    # }

    metadata = {
        "samples": samples,
        "chirps": chirps,
        "channels": CHANNELS,
        "delay": 4,
        "frames": 1,
    }

    url = f"http://{TARGET_IP_RESI}:8008/chandramipi/multiframe/start"
    files = {"mipifile": mipidata}
    try:
        print("Sending metadata:", metadata)
        requests.post(url, files=files, data=metadata)
        # response.raise_for_status()
        print("✔ MIPI frame sent successfully.")
    except requests.RequestException as e:
        print("✘ Failed to send MIPI frame:", e)


def process_mat_file(mat_path: Path, output_dir: Path):
    """
    Process the mat files one at a time, create a bin file for every mat file.
    """
    mat_name = mat_path.stem
    with h5py.File(mat_path, "r") as f:
        for group_name in f.keys():
            group = f[group_name]
            if "Data" in group:
                data = np.array(group["Data"])
                if data.ndim != 3:
                    continue
                rx, chirps, samples = data.shape

                if TRANSPOSE_MODE == "samples_rx_chirps":
                    transposed_data = data.transpose(2, 0, 1)
                elif TRANSPOSE_MODE == "chirps_samples_rx":
                    transposed_data = data.transpose(1, 2, 0)
                elif TRANSPOSE_MODE == "rx_chirps_samples":
                    transposed_data = data  # no transpose
                elif TRANSPOSE_MODE == "rx_samples_chirps":
                    transposed_data = data.transpose(0, 1, 2)  # most likely transpose
                    # transposed_data = data.transpose(0,2,1)
                    # transposed_data = data.transpose(1,0,2)
                    # transposed_data = data.transpose(2, 1, 0)
                    # transposed_data = data.transpose(2, 0, 1)
                    # transposed_data = data.transpose(1, 2, 0) # another candidate
                    # transposed_data = data  # no transpose
                else:
                    raise ValueError(f"Unsupported TRANSPOSE_MODE: {TRANSPOSE_MODE}")

                bin_path = output_dir / f"{mat_name}_{group_name}_Data.bin"
                transposed_data.flatten(order="C").tofile(bin_path)

                all_bin_paths.append(
                    {"bin_file": str(bin_path), "samples": samples, "chirps": chirps}
                )


def is_running():
    """
    Check if the RESI is running.
    """
    url = f"http://{TARGET_IP_RESI}:8008/chandramipi/multiframe/running"
    r = requests.get(url)
    return r.text.strip() == "True"


def main():
    """
    Process Mat files, create bin files and trigger resi.
    """
    if len(sys.argv) > 1:
        mat_dir = Path(sys.argv[1])
        output_dir = mat_dir / "output_bins"
    else:
        mat_dir = DEFAULT_MAT_DIR
        output_dir = OUTPUT_DIR
    output_dir.mkdir(parents=True, exist_ok=True)
    # mat_dir = Path(sys.argv[1]) if len(sys.argv) > 1 else DEFAULT_MAT_DIR
    if not os.path.isdir(mat_dir):
        print(f"Error: '{mat_dir}' is not a valid directory.")
        sys.exit(1)
    mat_files = list(mat_dir.glob("*.mat"))
    if not mat_files:
        print(f"No .mat files found in '{mat_dir}'. Exiting.")
        sys.exit(0)
    print(f"Found {len(mat_files)} .mat file(s) in '{mat_dir}':")
    for f in mat_files:
        print(f"Processing: {f.name}")
        process_mat_file(f, output_dir)

    if not all_bin_paths:
        print("No bin files created.")
        return

    if not xcp_connect():
        print("XCP CONNECT failed.")
        return

    resi_state_address = extract_address_from_map(MAP_FILE_PATH, RESI_STATE_SYMBOL)
    if not resi_state_address:
        print("RESI state symbol not found.")
        return

    if IS_ADC_CHIRP_VISUALIZATION_NEEDED:
        chirp_num_address = extract_address_from_map(MAP_FILE_PATH, "XCP_ADC_Log_Chirp_Num")
        if chirp_num_address:
            write_memory(chirp_num_address, 0)

    try:
        frame_index = 0
        while frame_index < len(all_bin_paths):
            value = read_memory(resi_state_address)
            if value == WAIT_VALUE:
                print(f"Trigger received. Sending frame {frame_index + 1}/{len(all_bin_paths)}...")
                trigger_resi(all_bin_paths[frame_index])
                time.sleep(1.5)
                # while is_running():
                #     time.sleep(0.5)
                # running = is_running()
                # assert running
                write_memory(resi_state_address, NEW_VALUE_AFTER_SUCCESS)
                frame_index += 1
                time.sleep(0.2)
            else:
                time.sleep(1)
        print("✅ All frames have been sent.")
    except KeyboardInterrupt:
        print("Interrupted by user.")
    finally:
        xcp_disconnect()
        if CLEAN_ON_INTERRUPT:
            print("Cleaning up output_bins...")
            shutil.rmtree(output_dir)


if __name__ == "__main__":
    main()
