"""
Convert a ptp file into a binary file that can be flashed via UniFlash.
"""

from os import system, path, strerror
import sys
import errno


def convertPtp2Bin(ptp_path):
    """
    Convert a ptp file into a binary file that can be flashed via UniFlash.

    The binary file will be placed in the same folder as the ptp file,
    with the same name, but the .bin extension insted of .ptp

    Args:
        ptp_path: Path to the ptp file to convert to binary.
    """
    if not path.isfile(ptp_path):
        raise FileNotFoundError(errno.ENOENT, strerror(errno.ENOENT), ptp_path)

    fileBase = path.splitext(path.abspath(ptp_path))[0]
    outputBinPath = fileBase + ".bin"

    cmdString = [
        "ptp.exe",
        "convert",
        "-inputType=srec",  # Input is s record
        "-toType=bin8",  # Output is bin file
        "-replace",
        "-output=" + outputBinPath,
        ptp_path,
    ]
    system(" ".join(cmdString))


if "help" in sys.argv:
    print(
        "\nThis script converts a ptp or s-record type file into a binary that can be flahsed using UniFlash."
    )
    print("The path to the original file is the only input argument.")
    print("   Usage: python convertPtp2Bin path/to/srec")
    exit()

if len(sys.argv) == 2:
    convertPtp2Bin(sys.argv[1])
else:
    print(
        "Error: Wrong number of input arguments. The only input argument expected is the path to a s19/ptp file."
    )
