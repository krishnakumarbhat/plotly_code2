"""
Stream Bandwidth Calculation Script.

The script takes the first line from streamdef files (no_of_bytes) as the input and
calculates the Bandwidth in Mbps

The python script generates a output filw with .csv extention

output file has the information like
Stream_File_Name,Stream_Size(Bytes),Bandwidth(Mpbs)
"""

import os
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("--output_file_path", type=str, required=True)
parser.add_argument("--input_files", type=argparse.FileType("r"), required=True)
args = parser.parse_args()

size = {}
total_stream_size = 0
total_stream_chunks = 0
total_stream_bandwidth = 0
REC_PAYLOAD = 1444  # Bytes

"""CDC max chunks/frame calculation"""
SPBB_MAX_CDC_DBIN_COUNT_PER_FRAME = 420  # Enabled CDC_DBins
TOTAL_CDC_RECORDS_PER_FRAME = 20  # CDC records per one frame
PARTIAL_FRAME = 0  # Partial Data frame (Gen8 don't have so this change done for Gen8 only)
LAST_INDICATION_FRAME = 1  # Last frame indication as per AptivHeader_6

MAX_CDC_CHUNKS = (int)(
    (SPBB_MAX_CDC_DBIN_COUNT_PER_FRAME / TOTAL_CDC_RECORDS_PER_FRAME)
    + PARTIAL_FRAME
    + LAST_INDICATION_FRAME
)

"""Dynamic Streams Worst-Case calculation"""
MAX_TARGET_REPORTS = 768  # used for stream 21 calculation
AF_MAX_NUM_DET = 2048  # used for stream 1 and 15 calculation


def calculate_bandwidth(total_chunks):
    """Stream bandwith calculation in Mbps."""
    ETH_FRAME_LENGTH = 1514  # Bytes
    BYTE_TO_BITS = 8  # 1 Byte = 8 bits
    RADAR_CYCLE_MS_TEMPARARY = 50  # milliseconds
    SECOND_TO_MS = 1000
    MBPS_TO_BPS = 1000000

    total_bits = total_chunks * ETH_FRAME_LENGTH * BYTE_TO_BITS
    bandwidth_bps = (total_bits / RADAR_CYCLE_MS_TEMPARARY) * SECOND_TO_MS
    bandwidth_mbps = bandwidth_bps / MBPS_TO_BPS

    return bandwidth_mbps


def stream_total_chunks(stream_size):
    """Calculate the stream total chunks."""
    if stream_size % REC_PAYLOAD == 0:
        total_chunks = stream_size // REC_PAYLOAD
    else:
        total_chunks = (stream_size // REC_PAYLOAD) + 1

    return total_chunks


"""Read input file and make ready the required data"""
for file in args.input_files.readlines():
    file = file.strip()
    filename = os.path.basename(file)
    #  print("\nfile_path: ",file, "\nfile_name=",filename)
    if ("str014" in filename) or ("str046" in filename):
        size[filename] = [int(REC_PAYLOAD), int(1), calculate_bandwidth(int(1)), "STATIC"]
    elif "str006" in filename:
        size[filename] = [
            int(MAX_CDC_CHUNKS * REC_PAYLOAD),
            int(MAX_CDC_CHUNKS),
            calculate_bandwidth(int(MAX_CDC_CHUNKS)),
            "DYNAMIC",
        ]
    else:
        with open(file, "r") as file_1:
            for line in file_1:
                stream_type = "REGULAR"
                dynstart = line.find("+")
                if -1 == dynstart:
                    stream_size = int(line)
                else:
                    stream_type = "REGULAR+DYNAMIC"
                    static_size = int(line[:dynstart])
                    dynamic_size = int(line[dynstart + 1 :])
                    if "str001" in filename:
                        stream_size = static_size + dynamic_size * AF_MAX_NUM_DET
                    elif "str015" in filename:
                        stream_size = static_size + dynamic_size * AF_MAX_NUM_DET
                    elif "str021" in filename:
                        stream_size = static_size + dynamic_size * MAX_TARGET_REPORTS
                    else:
                        raise Exception("Unexpected dynamic stream " + filename)

                stream_chunks = int(stream_total_chunks(stream_size))
                size[filename] = [
                    int(stream_size),
                    int(stream_chunks),
                    calculate_bandwidth(int(stream_chunks)),
                    stream_type,
                ]
                break
#  print("size value=",size[filename])

"""Open output file and write the Data"""
with open(args.output_file_path, "w") as f:
    f.write("STREAM_FILE_NAME,STREAM_SIZE(Bytes),TOTAL_CHUNKS,BANDWIDTH(Mbps),STREAM_TYPE\n")
    for path, values in size.items():
        total_stream_size += values[0]
        total_stream_chunks += values[1]
        total_stream_bandwidth += values[2]
        f.write(f"{path},{values[0]},{values[1]},{values[2]},{values[3]}\n")
    f.write("TOTAL STREAM DATA ON ETHERNET,")
    total_stream_size = "{:.6f}".format(total_stream_size)
    total_stream_chunks = "{:.6f}".format(total_stream_chunks)
    total_stream_bandwidth = "{:.6f}".format(total_stream_bandwidth)
    f.write(f"{total_stream_size},{total_stream_chunks},{total_stream_bandwidth}\n")
