"""Radar XCP-over-UDP diagnostic tool for Gen8 iND13400."""

# ─────────────────────────────────────────────────────────────────────────────
#  IMPORTS
# ─────────────────────────────────────────────────────────────────────────────
# Each import brings in a Python library (a collection of ready-made functions).
# Think of them as toolboxes -- we open only the ones we need.

import socket  # For creating UDP network connections (send/receive data over the network)
import struct  # For packing/unpacking binary data (e.g. converting integers to raw bytes)
import logging  # For writing detailed debug information to a log file on disk
import re  # Regular expressions -- used to validate that user input contains only hex characters
import os  # Operating system utilities -- used here to detect Windows vs Linux
import sys  # System-level info -- used to check if we're running in a real terminal
import time  # High-resolution timer -- used for precise latency measurements
import datetime  # Date and time utilities -- used for timestamps in logs and filenames
from collections import (
    OrderedDict,
)  # A dictionary that remembers insertion order (for error grouping)


# ═════════════════════════════════════════════════════════════════════════════
#  ANSI COLOR SUPPORT (auto-detect, graceful fallback)
# ═════════════════════════════════════════════════════════════════════════════
# ANSI escape codes are special character sequences (e.g. "\033[91m") that tell
# the terminal to display text in color. Not every terminal supports them, so
# we first check if colors will work. If they won't, we set all color variables
# to empty strings ("") so the rest of the code doesn't need any changes.


def _colors_supported():
    """Check if the terminal likely supports ANSI escape codes.

    How it works:
      1. If stdout is piped to a file (not a live terminal), colors are skipped.
      2. On Windows, we call the Windows API to enable color processing.
      3. On Linux/macOS, colors are almost always supported natively.

    Returns:
        True  -- safe to use color codes
        False -- fall back to plain text (no colors)
    """
    # Step 1: Check if stdout is a real terminal ("tty"), not a file or pipe.
    #         sys.stdout.isatty() returns True only for live terminal sessions.
    if not hasattr(sys.stdout, "isatty") or not sys.stdout.isatty():
        return False

    # Step 2: On Windows, ANSI colors aren't enabled by default.
    #         We need to tell Windows to turn on "Virtual Terminal Processing".
    if os.name == "nt":  # 'nt' means Windows (New Technology)
        try:
            import ctypes  # ctypes lets Python talk directly to Windows system DLLs

            kernel32 = ctypes.windll.kernel32
            # Get a handle to the console's standard output stream
            # STD_OUTPUT_HANDLE = -11 is a Windows constant meaning "stdout"
            handle = kernel32.GetStdHandle(-11)
            # Read the current console mode (a set of flags controlling behavior)
            mode = ctypes.c_ulong()
            kernel32.GetConsoleMode(handle, ctypes.byref(mode))
            # Turn on the "ENABLE_VIRTUAL_TERMINAL_PROCESSING" flag (bit 0x0004)
            # This tells Windows to interpret ANSI escape sequences as colors
            kernel32.SetConsoleMode(handle, mode.value | 0x0004)
            return True
        except Exception:
            # If the ctypes approach fails (e.g. older Windows), try a fallback
            try:
                os.system("")  # This sometimes activates ANSI support as a side effect
                return True
            except Exception:
                return False

    # Step 3: On Linux/macOS, ANSI is almost always supported in a real terminal
    return True


# Apply the detection: if colors are supported, define color code strings.
# If not, set them all to "" (empty) -- print() will just ignore them.
if _colors_supported():
    RST = "\033[0m"  # Reset -- turn off all formatting, back to default
    BOLD = "\033[1m"  # Bold -- makes text thicker/brighter
    DIM = "\033[2m"  # Dim -- makes text faded/gray (for secondary info)
    RED = "\033[91m"  # Bright red -- used for errors, timeouts, failures
    GRN = "\033[92m"  # Bright green -- used for OK / success responses
    YLW = "\033[93m"  # Bright yellow -- used for warnings and error reasons
    CYN = "\033[96m"  # Bright cyan -- used for headers, titles, separators
    WHT = "\033[97m"  # Bright white -- used for normal emphasis
    MAG = "\033[95m"  # Bright magenta -- used for latency/timing info
else:
    # No color support -- all codes become empty strings so print() output stays plain text.
    # This means: print(f"{RED}Error{RST}") just prints "Error" without any color codes.
    RST = BOLD = DIM = RED = GRN = YLW = CYN = WHT = MAG = ""

# ───── Pre-built horizontal line strings ─────
# These are used throughout the tool for visual separators in the terminal.
# We pre-build them here because Python versions below 3.12 don't allow
# backslash characters (like '\u2500') inside f-string curly braces {}.
# \u2500 is the Unicode "box drawing light horizontal" character: ─
LINE_72 = "\u2500" * 72  # 72-character horizontal line: ────────────...
LINE_68 = "\u2500" * 68  # 68-character horizontal line (slightly shorter)
LINE_3 = "\u2500" * 3  # 3-character line prefix: ───

# ═════════════════════════════════════════════════════════════════════════════
#  RADAR POSITION CONFIGURATION
# ═════════════════════════════════════════════════════════════════════════════
# Each radar sensor is mounted at a specific position on the vehicle (e.g. rear-left,
# front-center). Each position has its own IP address on the local network.
#
# How networking works:
#   SRC_IP / SRC_PORT = This PC's IP address and port (where we SEND from)
#   DST_IP / DST_PORT = The radar sensor's IP address and port (where we SEND to)
#
# To communicate with a different radar, just change the active position at
# runtime by typing: pos <N>  (e.g. "pos 3" for FRONT RIGHT)
#
# IMPORTANT: Edit the IPs below to match YOUR bench setup before running!

RADAR_POSITIONS = {
    # Position 1: REAR LEFT radar sensor
    1: {"SRC_IP": "192.168.1.40", "SRC_PORT": 65000, "DST_IP": "192.168.1.71", "DST_PORT": 5557},
    # Position 2: REAR RIGHT radar sensor
    2: {"SRC_IP": "192.168.1.40", "SRC_PORT": 65000, "DST_IP": "192.168.1.72", "DST_PORT": 5557},
    # Position 3: FRONT RIGHT radar sensor
    3: {"SRC_IP": "192.168.1.40", "SRC_PORT": 65000, "DST_IP": "192.168.1.73", "DST_PORT": 5557},
    # Position 4: FRONT LEFT radar sensor
    4: {"SRC_IP": "192.168.1.40", "SRC_PORT": 65000, "DST_IP": "192.168.1.74", "DST_PORT": 5557},
    # Position 5: FRONT CENTER radar sensor
    5: {"SRC_IP": "192.168.1.40", "SRC_PORT": 65000, "DST_IP": "192.168.1.75", "DST_PORT": 5557},
    # Position 6: REAR CENTER radar sensor
    6: {"SRC_IP": "192.168.1.40", "SRC_PORT": 65000, "DST_IP": "192.168.1.76", "DST_PORT": 5557},
    # Position 7: RIGHT CENTER radar sensor
    7: {"SRC_IP": "192.168.1.40", "SRC_PORT": 65000, "DST_IP": "192.168.1.77", "DST_PORT": 5557},
    # Position 8: LEFT CENTER radar sensor
    8: {"SRC_IP": "192.168.1.40", "SRC_PORT": 65000, "DST_IP": "192.168.1.78", "DST_PORT": 5557},
}

# Human-readable names for each position (used in prompts, logs, and output)
RADAR_POSITION_NAMES = {
    1: "REAR LEFT",
    2: "REAR RIGHT",
    3: "FRONT RIGHT",
    4: "FRONT LEFT",
    5: "FRONT CENTER",
    6: "REAR CENTER",
    7: "RIGHT CENTER",
    8: "LEFT CENTER",
}


# ═════════════════════════════════════════════════════════════════════════════
#  NETWORK AND PROTOCOL SETTINGS
# ═════════════════════════════════════════════════════════════════════════════

# Maximum number of bytes we expect to receive in a single UDP response.
# 1480 is safe for standard Ethernet frames (MTU 1500 minus IP/UDP headers).
BUFFER_SIZE = 1480

# How many seconds to wait for a radar's response before declaring a TIMEOUT.
# Increase this if the radar is slow to respond or if the network has high latency.
TIMEOUT_SEC = 20

# CTO = "Command Transfer Object" -- the XCP term for a command packet.
# MIN_CTO sets the minimum payload size in bytes. If your actual command is shorter
# than this, it gets right-padded with 0x00 bytes to reach this size.
# Example: If MIN_CTO=8 and you send FF (1 byte), it becomes FF 00 00 00 00 00 00 00.
# Set to 0 to disable padding and send the raw payload as-is.
MIN_CTO = 8

# These are the only valid CTO padding sizes the user is allowed to choose.
# They follow powers of 2 (plus 0 for "no padding").
VALID_CTO_SIZES = [0, 2, 4, 8, 16, 32, 64, 128, 256]


def get_position_config(radar_pos: int = 1) -> dict:
    """Look up the network config (IPs and ports) for a given radar position number.

    Args:
        radar_pos: Position number (1-8). If not found, defaults to position 1.

    Returns:
        A dictionary with keys: SRC_IP, SRC_PORT, DST_IP, DST_PORT
    """
    return RADAR_POSITIONS.get(radar_pos, RADAR_POSITIONS[1])


# ═════════════════════════════════════════════════════════════════════════════
#  GLOBAL STATE (these variables are shared across the entire session)
# ═════════════════════════════════════════════════════════════════════════════

# Which radar position is currently active (1-8). Changed via "pos <N>" command.
radar_pos = 1
# The full config dict for the currently active position
pos_config = get_position_config(radar_pos)

# ───── Logging Setup ─────
# Every session creates a new log file with a unique timestamp in its name.
# All XCP commands, responses, errors, and the session summary are written here.
# The log is UTF-8 encoded so special characters (like -- or Unicode) display correctly.
timestamp = datetime.datetime.now().strftime("%Y%m%d_%H%M%S")  # e.g. "20260324_225738"
log_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), "logs")
os.makedirs(log_dir, exist_ok=True)
log_filename = os.path.join(log_dir, f"xcp_comm_debug_log_{timestamp}.log")
logging.basicConfig(
    filename=log_filename,  # Write to this file (created in the current directory)
    level=logging.INFO,  # Log everything at INFO level and above (INFO, WARNING, ERROR)
    format="%(asctime)s - %(levelname)s - %(message)s",  # Timestamp - Level - Message
    encoding="utf-8",  # Use UTF-8 encoding so all characters render correctly
)
logger = logging.getLogger(__name__)  # Create a logger instance for this module

# XCP message counter -- a 16-bit number (0 to 65535) that increments with each
# command sent. It is placed in the XCP transport header so the radar can track
# which request corresponds to which response. Wraps from 65535 back to 0.
xcp_msg_counter = 0

# Transaction log -- a list that accumulates one dictionary per command sent.
# At the end of the session, this is written to the log file as a summary table.
# Each entry stores: command name, counter, position, raw TX/RX bytes, result,
# latency, and timestamp.
transaction_log = []

# ═════════════════════════════════════════════════════════════════════════════
#  XCP PROTOCOL LOOKUP TABLES
# ═════════════════════════════════════════════════════════════════════════════
# XCP (Universal Measurement and Calibration Protocol) uses numbered command codes.
# The FIRST byte of every XCP command payload identifies which command it is.
# For example: 0xFF = CONNECT, 0xF1 = USER_CMD, 0xF0 = DOWNLOAD.
#
# These lookup tables let us convert between raw hex bytes and human-readable names.
# Reference: ASAM XCP specification + CMP_SRS_CoreRadar_Radar_Calibration_Validation

# XCP_COMMANDS: Maps the first byte of a SENT command to its name.
# Example: payload starts with 0xFF -> "CONNECT"
# Ref: CMP_SRS_CoreRadar_Radar_Calibration_Validation Table 3
XCP_COMMANDS = {
    0xFF: "CONNECT",
    0xFE: "DISCONNECT",
    0xFD: "GET_STATUS",
    0xFC: "SYNCH",
    0xFB: "GET_COMM_MODE_INFO",
    0xFA: "GET_ID",
    0xF9: "SET_REQUEST",
    0xF8: "GET_SEED",
    0xF7: "UNLOCK",
    0xF6: "SET_MTA",
    0xF5: "UPLOAD",
    0xF4: "SHORT_UPLOAD",
    0xF3: "BUILD_CHECKSUM",
    0xF2: "TRANSPORT_LAYER_CMD",
    0xF1: "USER_CMD",
    0xF0: "DOWNLOAD",
    0xEF: "DOWNLOAD_NEXT",
    0xEE: "DOWNLOAD_MAX",
    0xED: "SHORT_DOWNLOAD",
    0xEC: "MODIFY_BITS",
    0xEB: "SET_CAL_PAGE",
    0xEA: "GET_CAL_PAGE",
    0xE9: "GET_PAG_PROCESSOR_INFO",
    0xE8: "GET_SEGMENT_INFO",
    0xE7: "GET_PAGE_INFO",
    0xE6: "SET_SEGMENT_MODE",
    0xE5: "GET_SEGMENT_MODE",
    0xE4: "COPY_CAL_PAGE",
    0xE3: "CLEAR_DAQ_LIST",
    0xE2: "SET_DAQ_PTR",
    0xE1: "WRITE_DAQ",
    0xE0: "SET_DAQ_LIST_MODE",
    0xDF: "GET_DAQ_LIST_MODE",
    0xDE: "START_STOP_DAQ_LIST",
    0xDD: "START_STOP_SYNCH",
    0xDC: "GET_DAQ_CLOCK",
    0xDB: "READ_DAQ",
    0xDA: "GET_DAQ_PROCESSOR_INFO",
    0xD9: "GET_DAQ_RESOLUTION_INFO",
    0xD8: "GET_DAQ_LIST_INFO",
    0xD7: "GET_DAQ_EVENT_INFO",
    0xD6: "FREE_DAQ",
    0xD5: "ALLOC_DAQ",
    0xD4: "ALLOC_ODT",
    0xD3: "ALLOC_ODT_ENTRY",
    0xD2: "PROGRAM_START",
    0xD1: "PROGRAM_CLEAR",
    0xD0: "PROGRAM",
    0xCF: "PROGRAM_RESET",
    0xCE: "GET_PGM_PROCESSOR_INFO",
    0xCD: "GET_SECTOR_INFO",
    0xCC: "PROGRAM_PREPARE",
    0xCB: "PROGRAM_FORMAT",
    0xCA: "PROGRAM_NEXT",
    0xC9: "PROGRAM_MAX",
    0xC8: "PROGRAM_VERIFY",
}

# XCP_RESPONSES: Maps the first byte of a RECEIVED response to its meaning.
# Every XCP response starts with one of these four PID (Packet ID) values.
#   0xFF = OK    -- command succeeded (positive response)
#   0xFE = ERR   -- command failed (negative response; byte 2 has the error code)
#   0xFD = EV    -- asynchronous event notification from the radar
#   0xFC = SERV  -- service request from the radar
XCP_RESPONSES = {
    0xFF: "OK",
    0xFE: "ERR",
    0xFD: "EV",
    0xFC: "SERV",
}

# XCP_ERRORS: When a response starts with 0xFE (ERR), the SECOND byte tells us
# what went wrong. This table maps that error code to a short name.
# Example: response is [FE 22] -> error code 0x22 -> "OUT_OF_RANGE"
# Ref: CMP_SRS_CoreRadar_Radar_Calibration_Validation Table 6
XCP_ERRORS = {
    0x00: "CMD_SYNCH_CPV",
    0x10: "CMD_BUSY",
    0x11: "DAQ_ACTIVE",
    0x12: "PGM_ACTIVE",
    0x20: "CMD_UNKNOWN",
    0x21: "CMD_SYNTAX",
    0x22: "OUT_OF_RANGE",
    0x23: "WRITE_PROTECTED",
    0x24: "ACCESS_DENIED",
    0x25: "ACCESS_LOCKED",
    0x26: "PAGE_NOT_VALID_CPV",
    0x27: "MODE_NOT_VALID_CPV",
    0x28: "SEGMENT_NOT_VALID_CPV",
    0x29: "SEQUENCE",
    0x2A: "DAQ_CONFIG",
    0x30: "MEMORY_OVERFLOW",
    0x31: "GENERIC",
    0x32: "VERIFY",
}

# XCP_ERROR_DESC: Human-readable explanations for each error code.
# These descriptions are shown to the user on the terminal and written to the log.
# Example: 0x22 -> "Command parameter(s) out of range."
XCP_ERROR_DESC = {
    0x00: "Command processor synchronization.",
    0x10: "Command was not executed.",
    0x11: "Command rejected because DAQ is running.",
    0x12: "Command rejected because PGM is running.",
    0x20: "Unknown command or not implemented optional command.",
    0x21: "Command syntax invalid.",
    0x22: "Command parameter(s) out of range.",
    0x23: "The memory location is write protected.",
    0x24: "The memory location is not accessible.",
    0x25: "Access denied, Seed & Key is required.",
    0x26: "Selected page not available.",
    0x27: "Selected page mode not available.",
    0x28: "Selected segment not valid.",
    0x29: "Sequence error.",
    0x2A: "DAQ configuration not valid.",
    0x30: "Memory overflow error.",
    0x31: "Generic error.",
    0x32: "Slave internal program verify routine detects an error.",
}

# ═════════════════════════════════════════════════════════════════════════════
#  FORMATTING HELPER FUNCTIONS
# ═════════════════════════════════════════════════════════════════════════════
# These small functions convert raw bytes into human-readable strings.
# They are used throughout the code for both terminal output and log files.


def _format_hex(data) -> str:
    r"""Format bytes as [ FF C1 00 A5 ] (Hex) for log files.

    Example: b'\xff\xc1' -> '[ FF C1 ] (Hex)'
    Returns '(empty)' if data is empty or None.
    """
    if not data:
        return "(empty)"
    # Join each byte as a 2-digit uppercase hex string, separated by spaces
    return "[ " + " ".join(f"{b:02X}" for b in data) + " ] (Hex)"


def _clean_hex(data) -> str:
    r"""Format bytes as 'FF C1 00 A5' for clean terminal display.

    Similar to _format_hex() but without the brackets and '(Hex)' suffix.
    This is used for the colored terminal output where we want a compact look.

    Example: b'\xff\xc1' -> 'FF C1'
    Returns '(empty)' if data is empty or None.
    """
    if not data:
        return "(empty)"
    return " ".join(f"{b:02X}" for b in data)


def _format_dec(data) -> str:
    r"""Format bytes as [ 255, 193, 0, 165 ] (Dec) for log files.

    Same data as _format_hex(), but shown as decimal numbers instead of hex.
    Useful for engineers who prefer to read decimal values.

    Example: b'\xff\xc1' -> '[ 255, 193 ] (Dec)'
    Returns '(empty)' if data is empty or None.
    """
    if not data:
        return "(empty)"
    return "[ " + ", ".join(f"{b}" for b in data) + " ] (Dec)"


def _format_time(dt: datetime.datetime) -> str:
    """Format a datetime object as HH:MM:SS.ffffff (microsecond precision).

    Example: datetime(2026, 3, 24, 22, 57, 48, 133480) -> '22:57:48.133480'
    This format is used in logs and the transaction ledger.
    """
    return dt.strftime("%H:%M:%S.%f")


# ═════════════════════════════════════════════════════════════════════════════
#  XCP COMMAND / RESPONSE NAME LOOKUP FUNCTIONS
# ═════════════════════════════════════════════════════════════════════════════
# These functions translate raw byte values into human-readable command/response names.


def xcp_cmd_name(payload: bytes) -> str:
    r"""Return the human-readable XCP command name from a command payload.

    Looks at the FIRST byte of the payload and finds its name in XCP_COMMANDS.

    Example: b'\xff' -> 'CONNECT' (because 0xFF = CONNECT)
    Example: b'\xf1\x03\x03\x01' -> 'USER_CMD' (because 0xF1 = USER_CMD)
    Returns '?' if the payload is empty, or 'UNKNOWN(0xNN)' if not in the table.
    """
    if not payload:
        return "?"
    return XCP_COMMANDS.get(payload[0], f"UNKNOWN(0x{payload[0]:02X})")


def xcp_resp_name(resp_bytes: bytes) -> str:
    """Return a full human-readable description of an XCP response.

    This is the DETAILED version used in terminal output and log files.
    - For OK (0xFF): returns 'OK'
    - For ERR (0xFE): returns 'ERR -- OUT_OF_RANGE: Command parameter(s) out of range.'
    - For events/service requests: returns 'EV' or 'SERV'

    Args:
        resp_bytes: The XCP response payload (after stripping the 4-byte transport header).

    Returns:
        A descriptive string like 'OK' or 'ERR -- OUT_OF_RANGE: Command parameter(s) out of range.'
    """
    if not resp_bytes:
        return "?"
    pid = resp_bytes[0]  # PID = Packet Identifier (first byte of the response)
    tag = XCP_RESPONSES.get(pid, f"UNKNOWN(0x{pid:02X})")
    # If it's an error response and there's at least a second byte (error code):
    if pid == 0xFE and len(resp_bytes) > 1:
        err_code = resp_bytes[1]  # The specific error code (e.g. 0x22 = OUT_OF_RANGE)
        err = XCP_ERRORS.get(err_code, f"0x{err_code:02X}")
        desc = XCP_ERROR_DESC.get(err_code, "")
        if desc:
            return f"{tag} -- {err}: {desc}"  # e.g. "ERR -- OUT_OF_RANGE: Command parameter(s) out of range."
        return f"{tag} -- {err}"  # e.g. "ERR -- OUT_OF_RANGE" (no description available)
    return tag  # e.g. "OK", "EV", "SERV"


def xcp_resp_short(resp_bytes: bytes) -> str:
    r"""Return a SHORT result tag for table/ledger display.

    This is the COMPACT version used in the session-end summary table where
    space is limited. Unlike xcp_resp_name(), it does NOT include the long
    error description.

    Examples:
        b'\xff...' -> 'OK'
        b'\xfe\x22' -> 'ERR:OUT_OF_RANGE'
        b'\xfe\x20' -> 'ERR:CMD_UNKNOWN'

    Args:
        resp_bytes: The XCP response payload (after stripping the 4-byte transport header).

    Returns:
        A short string like 'OK', 'ERR:OUT_OF_RANGE', or 'TIMEOUT'.
    """
    if not resp_bytes:
        return "?"
    pid = resp_bytes[0]
    tag = XCP_RESPONSES.get(pid, f"0x{pid:02X}")
    if pid == 0xFE and len(resp_bytes) > 1:
        err_code = resp_bytes[1]
        err = XCP_ERRORS.get(err_code, f"0x{err_code:02X}")
        return f"ERR:{err}"  # e.g. "ERR:OUT_OF_RANGE" -- compact, fits in a table column
    return tag


# ═════════════════════════════════════════════════════════════════════════════
#  CORE UDP SEND/RECEIVE FUNCTION
# ═════════════════════════════════════════════════════════════════════════════
# This is the heart of the tool. Every time the user sends a command to a radar,
# this function handles:
#   1. Building the XCP-over-Ethernet UDP message (header + payload)
#   2. Sending it over the network to the radar
#   3. Waiting for and receiving the radar's response
#   4. Displaying results on the terminal (with colors)
#   5. Logging all details to the log file
#   6. Recording the transaction for the session-end summary


def udp(
    payload: bytes, pad_override: int = None, pos_override: int = None, ctr_override: int = None
):
    r"""Send an XCP command over UDP and display/log the request, response, and timing.

    This function does everything needed for one XCP command:
      - Resolves which radar to talk to (position override or global default)
      - Pads the payload to CTO size (pad override or global default)
      - Builds the 4-byte XCP transport header (payload length + message counter)
      - Opens a UDP socket, sends the message, waits for the response
      - Prints a colored summary to the terminal
      - Writes detailed hex/dec/header breakdown to the log file
      - Records the transaction in the session ledger

    Args:
        payload:      The raw XCP command bytes to send.
                      Example: b'\xFF' for CONNECT, b'\xF1\x03\x03\x01' for USER_CMD.
        pad_override: One-shot CTO padding size from inline '| pad N'.
                      If None, uses the global MIN_CTO setting.
                      Example: pad_override=32 pads the payload to 32 bytes.
        pos_override: One-shot radar position from inline '| pos N'.
                      If None, uses the global radar_pos setting.
                      This does NOT change the persistent position.
        ctr_override: One-shot message counter from inline '| ctr N'.
                      If None, uses the global xcp_msg_counter (and increments it).
                      When set, the global counter is NOT incremented -- this is for
                      replaying a specific counter value without disturbing the sequence.
    """
    # --- STEP 1: Resolve which radar position to use ---
    # 'global' tells Python we want to READ AND WRITE the module-level variable,
    # not create a new local one with the same name.
    global xcp_msg_counter

    # If the user typed something like "FF | pos 3", pos_override will be 3.
    # We use that temporarily without changing the global radar_pos setting.
    if pos_override is not None:
        cfg = get_position_config(pos_override)  # Look up the config for the overridden position
        active_pos = pos_override
    else:
        cfg = pos_config  # Use the current global position config
        active_pos = radar_pos  # Use the current global position number

    # --- STEP 2: Extract network addresses from the resolved config ---
    src_ip = cfg["SRC_IP"]  # This PC's IP address (where we send FROM)
    src_port = int(cfg["SRC_PORT"])  # This PC's port number
    dst_ip = cfg["DST_IP"]  # The radar's IP address (where we send TO)
    dst_port = int(cfg["DST_PORT"])  # The radar's port number

    # --- STEP 3: Apply CTO padding ---
    # CTO padding ensures the payload meets the minimum size expected by the radar.
    # If pad_size > 0, we right-pad with 0x00 bytes. Example:
    #   payload = FF (1 byte), pad_size = 8 -> FF 00 00 00 00 00 00 00 (8 bytes)
    # If pad_size = 0, we send the raw payload exactly as the user typed it.
    pad_size = pad_override if pad_override is not None else MIN_CTO
    padded_payload = payload.ljust(pad_size, b"\x00") if pad_size > 0 else payload

    # --- STEP 4: Build the XCP-over-Ethernet transport header ---
    # Per ASAM XCP Part 3 (Transport Layer: Ethernet), every XCP message starts
    # with a 4-byte header:
    #   Bytes 0-1: Payload length (uint16, little-endian)
    #              Tells the receiver how many payload bytes follow the header.
    #   Bytes 2-3: Message counter (uint16, little-endian)
    #              A sequence number that increments per command. Wraps at 65535->0.
    #
    # struct.pack('<HH', ...) packs two unsigned 16-bit integers in little-endian format.
    # '<' = little-endian byte order, 'H' = unsigned 16-bit integer (2 bytes each).
    #
    # If the user gave an inline counter override (| ctr N), we use that value
    # instead of the auto-incrementing global counter.
    current_ctr = ctr_override if ctr_override is not None else xcp_msg_counter
    header = struct.pack("<HH", len(padded_payload), current_ctr)

    # The complete UDP message = 4-byte header + padded payload
    message = header + padded_payload

    # Get the human-readable name of this command (e.g. "CONNECT", "USER_CMD")
    cmd_name = xcp_cmd_name(payload)

    # --- STEP 5: Print the command summary to the terminal (colored output) ---
    # Build a decorative title bar: ─── CONNECT (ctr=0) ─────────...
    pos_name = RADAR_POSITION_NAMES.get(active_pos, "")
    title = f" {cmd_name} (ctr={current_ctr}) "
    pad_len = max(1, 69 - len(title))  # Calculate remaining space for right-side dashes
    rule = LINE_3 + title + "─" * pad_len  # e.g. "─── CONNECT (ctr=0) ──..."
    print(f"\n  {CYN}{rule}{RST}")  # Cyan colored header line
    # Show which radar and network path we're using (dim/gray)
    print(
        f"  {DIM}Pos {active_pos} ({pos_name}) | {src_ip}:{src_port} -> {dst_ip}:{dst_port}{RST}"
    )
    # Show what we're sending (TX = Transmit)
    print(f"    {WHT}TX :{RST} {_clean_hex(padded_payload)}")
    # Show the raw bytes on the wire: [header] [payload] (total size)
    print(
        f"    {DIM}RAW: [{_clean_hex(header)}] [{_clean_hex(padded_payload)}]  ({len(message)} bytes on wire){RST}"
    )
    # --- STEP 6: Log the TX details to the log file ---
    logger.info("=" * 80)
    logger.info(f"XCP COMMAND: {cmd_name}  (ctr={current_ctr}) | Radar Position: {active_pos}")
    logger.info(f"[CONFIG]   {src_ip}:{src_port} -> {dst_ip}:{dst_port}")
    logger.info(f"[REQUEST]  {_format_hex(payload)}")  # Original payload in hex
    logger.info(f"[REQUEST]  {_format_dec(payload)}")  # Original payload in decimal
    logger.info(f"[RAW TX]   {_format_hex(message)}")  # Full message (header + padded payload)
    # Break down the 4-byte transport header for debugging:
    # struct.unpack('<H', ...) extracts a uint16 from 2 bytes in little-endian order.
    tx_hdr_len = struct.unpack("<H", header[:2])[0]  # Bytes 0-1 = payload length
    tx_hdr_ctr = struct.unpack("<H", header[2:4])[0]  # Bytes 2-3 = message counter
    logger.info(
        f"[TX HDR]   Bytes 0-1: 0x{header[0]:02X} 0x{header[1]:02X} = Payload Length {tx_hdr_len} bytes (little-endian)"
    )
    logger.info(
        f"[TX HDR]   Bytes 2-3: 0x{header[2]:02X} 0x{header[3]:02X} = Message Counter {tx_hdr_ctr} (little-endian)"
    )
    logger.info(
        f"[TX HDR]   Total UDP datagram: {len(message)} bytes (4-byte header + {len(padded_payload)}-byte payload)"
    )

    # --- STEP 7: Create a UDP socket, send the command, wait for the response ---
    # UDP (User Datagram Protocol) is a lightweight network protocol.
    # AF_INET = IPv4 addressing, SOCK_DGRAM = UDP (as opposed to TCP).
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        # Bind to our source IP and port so the radar knows where to send the reply
        sock.bind((src_ip, src_port))
        # Set a timeout -- if no response arrives within TIMEOUT_SEC seconds, give up
        sock.settimeout(TIMEOUT_SEC)

        # Record the exact time we're about to send (for latency calculation)
        # datetime.now()  = wall-clock time (for logs)
        # perf_counter()  = high-resolution monotonic timer (for accurate latency)
        request_dt = datetime.datetime.now()
        perf_start = time.perf_counter()

        # Send the XCP message to the radar
        sock.sendto(message, (dst_ip, dst_port))

        # Increment the 16-bit message counter for the NEXT command.
        # The "& 0xFFFF" ensures the value wraps from 65535 back to 0 (staying in uint16 range).
        # BUT: if the user supplied a manual counter override (| ctr N), we DON'T increment.
        # The override is a one-shot "replay" value and should not disturb the natural sequence.
        if ctr_override is None:
            xcp_msg_counter = (xcp_msg_counter + 1) & 0xFFFF

        # Block here and wait for the radar to send back a response.
        # recvfrom() returns a tuple: (data_bytes, sender_address)
        # If no response arrives before the timeout, a socket.timeout exception is raised.
        data, address = sock.recvfrom(BUFFER_SIZE)
        perf_end = time.perf_counter()  # Record when we received the response
        response_dt = datetime.datetime.now()

        # Calculate the round-trip latency (time between send and receive) in milliseconds
        latency_ms = (perf_end - perf_start) * 1000  # Convert seconds to milliseconds

        # --- STEP 8: Parse the response ---
        # The response also has a 4-byte XCP transport header, then the actual payload.
        # We strip the header (data[:4]) and keep only the XCP payload (data[4:]).
        resp_payload = data[4:]  # Everything after the 4-byte header
        resp_name = xcp_resp_name(resp_payload)  # Full description (for display/logs)
        resp_short = xcp_resp_short(resp_payload)  # Short tag (for summary table)

        # --- STEP 9: Display the response on the terminal ---
        # RX = Receive (what came back from the radar)
        print(f"    {WHT}RX :{RST} {_clean_hex(resp_payload)}  ({len(resp_payload)} bytes)")
        print(
            f"    {DIM}RAW: [{_clean_hex(data[:4])}] [{_clean_hex(resp_payload)}]  ({len(data)} bytes on wire){RST}"
        )
        print(f"    {DIM}{LINE_68}{RST}")

        # Color the result line based on success or failure:
        #   Green  = OK (0xFF)   -- command succeeded
        #   Red    = ERR (0xFE)  -- command failed
        #   Yellow = anything else (unexpected response type)
        if resp_payload and resp_payload[0] == 0xFF:
            print(f"    {GRN}{BOLD}Result  : {resp_name}{RST}")
        elif resp_payload and resp_payload[0] == 0xFE:
            print(f"    {RED}{BOLD}Result  : {resp_name}{RST}")
        else:
            print(f"    {YLW}Result  : {resp_name}{RST}")

        # For error responses, show the specific error reason in yellow
        if resp_payload and resp_payload[0] == 0xFE and len(resp_payload) > 1:
            err_code = resp_payload[1]  # Second byte = error subcode
            reason = XCP_ERROR_DESC.get(err_code, "No description available.")
            print(f"    {YLW}Reason  : 0x{err_code:02X} -- {reason}{RST}")
            logger.warning(f"[REASON]   ERR 0x{err_code:02X} for {cmd_name}: {reason}")

        # Show round-trip latency in magenta
        print(f"    {MAG}Latency : {latency_ms:.3f} ms{RST}")

        # --- STEP 10: Log detailed RX info to the log file ---
        time_msg = f"[TIME]     Sent: {_format_time(request_dt)} | Received: {_format_time(response_dt)} | Latency: {latency_ms:.3f} ms"
        logger.info(time_msg)
        logger.info(f"[RAW RX]   {_format_hex(data)}")
        # Break down the 4-byte response header (same format as TX header)
        rx_hdr = data[:4]
        rx_hdr_len = struct.unpack("<H", rx_hdr[:2])[0]  # Payload length from response
        rx_hdr_ctr = struct.unpack("<H", rx_hdr[2:4])[0]  # Counter from response
        logger.info(
            f"[RX HDR]   Bytes 0-1: 0x{rx_hdr[0]:02X} 0x{rx_hdr[1]:02X} = Payload Length {rx_hdr_len} bytes (little-endian)"
        )
        logger.info(
            f"[RX HDR]   Bytes 2-3: 0x{rx_hdr[2]:02X} 0x{rx_hdr[3]:02X} = Message Counter {rx_hdr_ctr} (little-endian)"
        )
        logger.info(
            f"[RX HDR]   Total UDP datagram: {len(data)} bytes (4-byte header + {len(resp_payload)}-byte payload)"
        )
        logger.info(f"[RESPONSE] {_format_hex(resp_payload)} ({len(resp_payload)} bytes)")
        logger.info(f"[RESPONSE] {_format_dec(resp_payload)} ({len(resp_payload)} bytes)")
        logger.info(f"[RESULT]   {resp_name}")

        # Log which type of response this was (OK, ERR, EV, or SERV)
        # and for errors, log the specific error code and its meaning
        if resp_payload and resp_payload[0] == 0xFE:
            logger.info(f"[RX INFO]  PID byte: 0x{resp_payload[0]:02X} = ERR (Negative response)")
            if len(resp_payload) > 1:
                err_code = resp_payload[1]  # The specific error subcode
                err_name = XCP_ERRORS.get(err_code, f"UNKNOWN(0x{err_code:02X})")
                err_desc = XCP_ERROR_DESC.get(err_code, "No description available.")
                logger.info(f"[RX INFO]  Error code byte: 0x{err_code:02X} = {err_name}")
                logger.info(f"[RX INFO]  Meaning: {err_desc}")
        elif resp_payload and resp_payload[0] == 0xFF:
            logger.info(f"[RX INFO]  PID byte: 0x{resp_payload[0]:02X} = OK (Positive response)")
        elif resp_payload and resp_payload[0] == 0xFD:
            logger.info(f"[RX INFO]  PID byte: 0x{resp_payload[0]:02X} = EV (Event)")
        elif resp_payload and resp_payload[0] == 0xFC:
            logger.info(f"[RX INFO]  PID byte: 0x{resp_payload[0]:02X} = SERV (Service request)")

        # --- STEP 11: Save this transaction to the session ledger ---
        # This dictionary is appended to transaction_log[] and gets printed
        # as a summary table in the log file when the user exits.
        transaction_log.append(
            {
                "cmd": cmd_name,
                "ctr": current_ctr,
                "pos": active_pos,
                "tx_raw": _format_hex(message),
                "rx_raw": _format_hex(data),
                "tx_hex": _clean_hex(padded_payload),
                "rx_hex": _clean_hex(resp_payload),
                "result": resp_short,
                "result_full": resp_name,
                "latency_ms": latency_ms,
                "timestamp": _format_time(request_dt),
            }
        )

    except socket.timeout:
        # TIMEOUT: The radar did not respond within TIMEOUT_SEC seconds.
        # This could mean the radar is off, unreachable, or the IP/port is wrong.
        print(f"    {RED}{BOLD}** TIMEOUT -- No response received.{RST}")
        logger.warning(f"[TIMEOUT]  No response for {cmd_name} (ctr={current_ctr})")
        transaction_log.append(
            {
                "cmd": cmd_name,
                "ctr": current_ctr,
                "pos": active_pos,
                "tx_raw": _format_hex(message),
                "rx_raw": "(no response)",
                "tx_hex": _clean_hex(padded_payload),
                "rx_hex": "(timeout)",
                "result": "TIMEOUT",
                "result_full": "TIMEOUT -- No response received",
                "latency_ms": None,
                "timestamp": _format_time(datetime.datetime.now()),
            }
        )
    except socket.error as e:
        # SOCKET ERROR: A network-level error occurred (e.g. port already in use,
        # network unreachable, permission denied).
        print(f"    {RED}{BOLD}** ERROR -- Socket error: {e}{RST}")
        logger.error(f"[ERROR]    Socket error for {cmd_name} (ctr={current_ctr}): {e}")
        transaction_log.append(
            {
                "cmd": cmd_name,
                "ctr": current_ctr,
                "pos": active_pos,
                "tx_raw": _format_hex(message),
                "rx_raw": f"(error: {e})",
                "tx_hex": _clean_hex(padded_payload),
                "rx_hex": "(error)",
                "result": "ERROR",
                "result_full": f"Socket error: {e}",
                "latency_ms": None,
                "timestamp": _format_time(datetime.datetime.now()),
            }
        )
    except Exception as e:
        # UNEXPECTED ERROR: Something we didn't anticipate went wrong.
        # This is a catch-all to prevent the tool from crashing.
        print(f"    {RED}{BOLD}** ERROR -- {e}{RST}")
        logger.error(f"[ERROR]    Unexpected error for {cmd_name} (ctr={current_ctr}): {e}")
        transaction_log.append(
            {
                "cmd": cmd_name,
                "ctr": current_ctr,
                "pos": active_pos,
                "tx_raw": _format_hex(message),
                "rx_raw": f"(error: {e})",
                "tx_hex": _clean_hex(padded_payload),
                "rx_hex": "(error)",
                "result": "ERROR",
                "result_full": f"Unexpected error: {e}",
                "latency_ms": None,
                "timestamp": _format_time(datetime.datetime.now()),
            }
        )
    finally:
        # ALWAYS close the socket when done, whether the command succeeded or failed.
        # This releases the port so it can be reused for the next command.
        sock.close()
    # Print a closing separator line for this command's output block
    print(f"  {DIM}{LINE_72}{RST}")


# ═════════════════════════════════════════════════════════════════════════════
#  USER INPUT PARSING
# ═════════════════════════════════════════════════════════════════════════════


def parse_hex_input(hex_string):
    r"""Convert a user-typed hex string into raw bytes.

    The user can type hex in many flexible formats, and this function handles
    all of them:
      'FF'              -> b'\xff'           (single byte)
      'F4 01 00 00 00'  -> b'\xf4\x01...'   (spaces between bytes)
      '0xFF'            -> b'\xff'           ('0x' prefix is stripped)
      'f103'            -> b'\xf1\x03'       (no spaces, lowercase)
      'F10'             -> b'\xf1\x00'       (odd length: last nibble padded with 0)

    How it works:
      1. Strip whitespace, remove '0x' prefixes, convert to lowercase
      2. Validate that only hex characters (0-9, a-f) remain
      3. If odd number of characters, append '0' (so bytes.fromhex can parse it)
      4. Convert the hex string to raw bytes

    Raises:
        ValueError: If the input contains non-hex characters or is empty.
    """
    # Clean up: strip leading/trailing spaces, lowercase everything,
    # remove '0x' prefixes, remove internal spaces
    hex_string = hex_string.strip().lower().replace("0x", "").replace(" ", "")

    # Validate: re.fullmatch() checks if the ENTIRE string matches the pattern.
    # [0-9a-f]* means "zero or more hex characters". If anything else is in there, reject it.
    if not re.fullmatch(r"[0-9a-f]*", hex_string):
        raise ValueError("Input contains non-hexadecimal characters.")
    if len(hex_string) == 0:
        raise ValueError("Empty input.")

    # bytes.fromhex() requires an even number of characters (each byte = 2 hex digits).
    # If the user typed an odd number (like 'F10'), pad with '0' to make it even.
    if len(hex_string) % 2 != 0:
        hex_string = hex_string + "0"

    # Convert the cleaned hex string into actual bytes.
    # Example: 'f103' -> b'\xf1\x03'
    return bytes.fromhex(hex_string)


# ═════════════════════════════════════════════════════════════════════════════
#  DISPLAY FUNCTIONS (help screen, command history)
# ═════════════════════════════════════════════════════════════════════════════


def show_help():
    """Display the help screen with all available commands and XCP quick reference."""
    print(f"\n  {CYN}{LINE_3} Help {LINE_68[:66]}{RST}")
    print(f"  {WHT}Commands:{RST}")
    print(f"    {CYN}<hex>{RST}         Send hex payload  (e.g. FF, F4 01 00 00 00)")
    print(f"    {CYN}<hex> | pad <N> | pos <N> | ctr <N>{RST}")
    print("                  Inline overrides, any order  (e.g. pad 4 | FF | pos 5 | ctr 0)")
    print(f"    {CYN}last{RST}          Resend the last payload")
    print(f"    {CYN}history{RST}       Show command history")
    print(f"    {CYN}pos [N]{RST}       Show or switch radar position")
    print(f"    {CYN}pad [N]{RST}       Show or set CTO padding size")
    print(f"    {CYN}ctr [N]{RST}       Show or set message counter")
    print(f"    {CYN}config{RST}        Show active network configuration")
    print(f"    {CYN}help{RST}          Show this help")
    print(f"    {CYN}exit{RST}          Quit")
    print()
    print(f"  {WHT}XCP Quick Reference:{RST}")
    print(
        f"    {CYN}FF{RST}  CONNECT         {CYN}FE{RST}  DISCONNECT       {CYN}FD{RST}  GET_STATUS"
    )
    print(
        f"    {CYN}FB{RST}  COMM_MODE_INFO  {CYN}FA{RST}  GET_ID           {CYN}F9{RST}  SET_REQUEST"
    )
    print(
        f"    {CYN}F8{RST}  GET_SEED        {CYN}F7{RST}  UNLOCK           {CYN}F6{RST}  SET_MTA"
    )
    print(
        f"    {CYN}F5{RST}  UPLOAD          {CYN}F4{RST}  SHORT_UPLOAD     {CYN}F3{RST}  BUILD_CHECKSUM"
    )
    print(
        f"    {CYN}F2{RST}  TRANSPORT_CMD   {CYN}F1{RST}  {BOLD}USER_CMD{RST}         {CYN}F0{RST}  DOWNLOAD"
    )
    print(
        f"    {CYN}EF{RST}  DOWNLOAD_NEXT   {CYN}ED{RST}  SHORT_DOWNLOAD   {CYN}EC{RST}  MODIFY_BITS"
    )
    print(f"  {DIM}{LINE_72}{RST}")


def show_history(history):
    """Display a numbered table of all XCP commands sent during this session.

    Args:
        history: A list of bytes objects, each being a previously sent payload.
    """
    if not history:
        print(f"  {DIM}No commands in history.{RST}")
        return
    print(f"\n  {CYN}{LINE_3} History {LINE_68[:63]}{RST}")
    print(f"    {DIM}{'#':>3s}   {'Payload':<40s}  Command{RST}")
    for i, entry in enumerate(history, 1):
        name = xcp_cmd_name(entry)
        print(f"    {WHT}{i:3d}.{RST}  {_clean_hex(entry):<40s}  {CYN}{name}{RST}")
    print(f"  {DIM}{LINE_72}{RST}")


# ═════════════════════════════════════════════════════════════════════════════
#  MAIN ENTRY POINT
# ═════════════════════════════════════════════════════════════════════════════
# The code below only runs when this script is executed directly (not imported).
# It sets up the interactive command loop where the user types XCP commands.

if __name__ == "__main__":
    # history: A list that stores every payload the user has sent (as raw bytes).
    # The user can view previous commands with 'history' or resend with 'last'.
    history = []

    # --- Print the startup banner ---
    # Shows the tool name, current configuration, and available commands.
    banner_title = LINE_3 + " XCP over Ethernet \u2014 UDP Command Tool " + "\u2500" * 33
    print(f"\n  {CYN}{BOLD}{banner_title}{RST}")
    print(f"  {WHT}Position :{RST} {radar_pos} ({RADAR_POSITION_NAMES.get(radar_pos, '')})")
    print(f"  {WHT}Source   :{RST} {pos_config['SRC_IP']}:{pos_config['SRC_PORT']}")
    print(f"  {WHT}Target   :{RST} {pos_config['DST_IP']}:{pos_config['DST_PORT']}")
    print(
        f"  {WHT}Settings :{RST} Timeout {TIMEOUT_SEC}s | Buffer {BUFFER_SIZE} | CTO Pad {MIN_CTO}"
    )
    print(f"  {WHT}Log      :{RST} {DIM}{log_filename}{RST}")
    print(f"  {DIM}{LINE_72}{RST}")
    print(
        f"  {WHT}Commands :{RST} <hex> | last | history | pos <N> | pad <N> | ctr <N> | config | help | exit"
    )
    print(
        f"  {WHT}Examples :{RST} {CYN}FF{RST}  |  {CYN}F4 01 00 00 00{RST}  |  {CYN}FF | pad 8 | pos 2 | ctr 0{RST}  |  {CYN}pad 4 | FF | pos 5{RST}"
    )
    print(f"  {WHT}Pad sizes:{RST} {VALID_CTO_SIZES}")
    print(f"  {DIM}{LINE_72}{RST}\n")

    # --- Log the session start ---
    logger.info("=" * 80)
    logger.info("XCP over Ethernet -- UDP Command Tool started")
    logger.info(
        f"Radar Position: {radar_pos} | {pos_config['SRC_IP']}:{pos_config['SRC_PORT']} -> {pos_config['DST_IP']}:{pos_config['DST_PORT']}"
    )
    logger.info("=" * 80)

    # ===== MAIN INTERACTIVE LOOP =====
    # This is where the tool waits for the user to type commands.
    #
    # The prompt looks like:  XCP #0 | REAR LEFT (pos 1) >
    #   - #0 = current message counter
    #   - REAR LEFT = current radar position name
    #   - pos 1 = current position number
    #
    # INPUT PARSING RULES:
    #   1. If the input contains '|' (pipe character), it is split into segments.
    #      Each segment is classified independently (ORDER DOES NOT MATTER):
    #        - "pad <N>"  -> one-shot CTO padding override for this command only
    #        - "pos <N>"  -> one-shot radar position override for this command only
    #        - "ctr <N>"  -> one-shot message counter override (global counter NOT incremented)
    #        - anything else -> treated as the hex payload (or a built-in command like 'last')
    #      Exactly ONE hex/command segment is required; duplicates cause an error.
    #      Examples:
    #        "FF | pad 8 | pos 2 | ctr 0"  -> send CONNECT with pad=8, to pos 2, counter=0
    #        "pad 4 | f1 03 03 01 | pos 5" -> send USER_CMD with pad=4, to pos 5
    #        "pos 3 | FF"                   -> send CONNECT to pos 3 (uses default pad/ctr)
    #
    #   2. If no pipe is present, the entire input is treated as either:
    #      - A standalone command: exit, help, history, config, pad, pos, ctr, last
    #      - Or raw hex bytes to send as an XCP command
    while True:
        try:
            # Build and show the interactive prompt.
            # It looks like:  XCP #0 | REAR LEFT (pos 1) >
            # The user types their command after the '>' character.
            pos_name = RADAR_POSITION_NAMES.get(radar_pos, "")
            prompt = (
                f"{CYN}XCP{RST} #{xcp_msg_counter} | {GRN}{pos_name}{RST} (pos {radar_pos}) > "
            )
            user_input = input(prompt).strip()  # .strip() removes leading/trailing whitespace
        except (EOFError, KeyboardInterrupt):
            # EOFError: happens when stdin is closed (e.g. piped input ends)
            # KeyboardInterrupt: happens when user presses Ctrl+C
            print("\nExiting.")
            break
        if not user_input:
            continue  # If user just pressed Enter without typing anything, show prompt again

        # --- PIPE-SEPARATED INLINE OVERRIDE PARSING ---
        # If the user typed pipes (|), we split the input into segments.
        # Each segment is independently classified as 'pad', 'pos', 'ctr', or hex payload.
        # They can appear in ANY order. Example: "pad 4 | FF | pos 5 | ctr 0"
        pad_override = None  # Will hold one-shot pad size (None = use global MIN_CTO)
        pos_override = None  # Will hold one-shot position (None = use global radar_pos)
        ctr_override = None  # Will hold one-shot counter (None = use global xcp_msg_counter)
        if "|" in user_input:
            # Split on the pipe character. Example: "FF | pad 8 | pos 2" -> ['FF', 'pad 8', 'pos 2']
            parts_pipe = [p.strip() for p in user_input.split("|")]
            parse_error = False  # Flag to track if we hit a validation error
            hex_segment = None  # Will hold the single hex payload segment once found

            for segment in parts_pipe:
                seg = segment.lower()  # Compare in lowercase for case-insensitivity
                if not seg:
                    # Skip empty segments (e.g. trailing pipe "FF | " or double pipe "FF | | pad 8")
                    continue
                if seg.startswith("pad"):
                    # --- INLINE PAD OVERRIDE ---
                    # "pad 8" means: for THIS command only, pad the payload to 8 bytes.
                    # This does NOT change the global MIN_CTO setting.
                    if pad_override is not None:
                        print(
                            "  [ERROR]  Duplicate 'pad' option. Only one 'pad' is allowed per command."
                        )
                        parse_error = True
                        break
                    pad_parts = seg.split()
                    if len(pad_parts) == 2:
                        try:
                            pval = int(pad_parts[1])
                            if pval < 0 or pval not in VALID_CTO_SIZES:
                                print(
                                    f"  [ERROR]  Invalid inline pad size. Valid: {VALID_CTO_SIZES}"
                                )
                                parse_error = True
                                break
                            pad_override = pval
                        except ValueError:
                            print(f"  [ERROR]  Invalid inline pad value: {pad_parts[1]}")
                            parse_error = True
                            break
                    else:
                        print("  [ERROR]  Inline pad usage: pad <N>  (e.g. pad 32)")
                        parse_error = True
                        break
                elif seg.startswith("pos"):
                    # --- INLINE POS OVERRIDE ---
                    # "pos 3" means: for THIS command only, send to radar position 3.
                    # This does NOT change the global radar_pos setting.
                    if pos_override is not None:
                        print(
                            "  [ERROR]  Duplicate 'pos' option. Only one 'pos' is allowed per command."
                        )
                        parse_error = True
                        break
                    pos_parts = seg.split()
                    if len(pos_parts) == 2:
                        try:
                            pval = int(pos_parts[1])
                            if pval not in RADAR_POSITIONS:
                                print(
                                    f"  [ERROR]  Position {pval} not configured. Available: {sorted(RADAR_POSITIONS.keys())}"
                                )
                                parse_error = True
                                break
                            pos_override = pval
                        except ValueError:
                            print(f"  [ERROR]  Invalid inline pos value: {pos_parts[1]}")
                            parse_error = True
                            break
                    else:
                        print("  [ERROR]  Inline pos usage: pos <N>  (e.g. pos 2)")
                        parse_error = True
                        break
                elif seg.startswith("ctr"):
                    # --- INLINE CTR OVERRIDE ---
                    # "ctr 0" means: for THIS command only, use counter value 0 in the header.
                    # The global counter is NOT incremented. This is useful for replaying
                    # a specific counter value or resetting for debugging.
                    if ctr_override is not None:
                        print(
                            "  [ERROR]  Duplicate 'ctr' option. Only one 'ctr' is allowed per command."
                        )
                        parse_error = True
                        break
                    ctr_parts = seg.split()
                    if len(ctr_parts) == 2:
                        try:
                            cval = int(ctr_parts[1])
                            if cval < 0 or cval > 0xFFFF:
                                print(f"  [ERROR]  Counter must be 0–65535 (uint16). Got: {cval}")
                                parse_error = True
                                break
                            ctr_override = cval
                        except ValueError:
                            print(f"  [ERROR]  Invalid inline ctr value: {ctr_parts[1]}")
                            parse_error = True
                            break
                    else:
                        print("  [ERROR]  Inline ctr usage: ctr <N>  (e.g. ctr 0)")
                        parse_error = True
                        break
                else:
                    # --- HEX PAYLOAD SEGMENT ---
                    # Not 'pad', 'pos', or 'ctr' -- this must be the hex payload
                    # (or a command like 'last'). Only ONE such segment is allowed.
                    if hex_segment is not None:
                        print(
                            f"  [ERROR]  Multiple payload segments found: '{hex_segment}' and '{segment}'"
                        )
                        parse_error = True
                        break
                    hex_segment = segment
            if parse_error:
                continue  # Go back to the prompt and let the user try again
            if hex_segment is None:
                print(
                    "  [ERROR]  No hex payload found. Usage: <hex> | pad <N> | pos <N> | ctr <N>"
                )
                continue
            # After parsing all pipe segments, user_input now holds just the payload/command part
            user_input = hex_segment

        # --- HANDLE STANDALONE COMMANDS ---
        # At this point, user_input is either:
        #   a) The original input (if no pipes were used), or
        #   b) Just the hex/command segment extracted from the pipe parsing above.
        # We check for built-in commands first, then treat anything else as hex.
        cmd = user_input.lower()  # Compare in lowercase for case-insensitivity

        if cmd == "exit":
            # Exit the tool and write the session summary to the log
            break
        elif cmd == "help":
            # Show the help screen with all commands and the XCP quick reference table
            show_help()
            continue
        elif cmd == "history":
            # Show a numbered list of all commands sent during this session
            show_history(history)
            continue
        elif cmd == "config":
            # Show the current network configuration (IP, port, timeout, etc.)
            print(f"\n  {CYN}{LINE_3} Configuration {LINE_68[:57]}{RST}")
            print(
                f"  {WHT}Position :{RST} {radar_pos} ({RADAR_POSITION_NAMES.get(radar_pos, '')})"
            )
            print(f"  {WHT}Source   :{RST} {pos_config['SRC_IP']}:{pos_config['SRC_PORT']}")
            print(f"  {WHT}Target   :{RST} {pos_config['DST_IP']}:{pos_config['DST_PORT']}")
            print(f"  {WHT}Timeout  :{RST} {TIMEOUT_SEC}s")
            print(f"  {WHT}Buffer   :{RST} {BUFFER_SIZE} bytes")
            print(f"  {WHT}CTO Pad  :{RST} {MIN_CTO} bytes")
            print(f"  {DIM}{LINE_72}{RST}")
            continue
        elif cmd.startswith("pad"):
            # STANDALONE "pad" COMMAND
            # "pad"   -> show current CTO padding size
            # "pad 32" -> permanently change the global CTO padding to 32 bytes
            # NOTE: This is different from inline "| pad N" which is one-shot (only for that command).
            parts = cmd.split()
            if len(parts) == 1:
                print(f"  {WHT}CTO Padding:{RST} {MIN_CTO} bytes")
                print(f"  {WHT}Valid sizes:{RST} {VALID_CTO_SIZES}")
                print(f"  {DIM}Use 'pad 0' to disable padding (send raw payload as-is){RST}")
            else:
                try:
                    new_pad = int(parts[1])
                    if new_pad < 0:
                        print("  [ERROR]  Padding cannot be negative.")
                        continue
                    if new_pad not in VALID_CTO_SIZES:
                        print(f"  [ERROR]  Invalid size. Valid: {VALID_CTO_SIZES}")
                        continue
                    MIN_CTO = new_pad
                    if MIN_CTO == 0:
                        print(f"  {GRN}CTO padding disabled (raw payload sent as-is){RST}")
                    else:
                        print(f"  {GRN}CTO padding set to {MIN_CTO} bytes{RST}")
                    logger.info(f"CTO padding changed to {MIN_CTO} bytes")
                except ValueError:
                    print(f"  [ERROR]  Invalid number: {parts[1]}")
            continue
        elif cmd.startswith("pos"):
            # STANDALONE "pos" COMMAND
            # "pos"   -> list all radar positions and show which one is active
            # "pos 3" -> permanently switch to position 3 (FRONT RIGHT)
            # NOTE: This is different from inline "| pos N" which is one-shot (only for that command).
            parts = cmd.split()
            if len(parts) == 1:
                print(
                    f"  {WHT}Active: Position {radar_pos} ({RADAR_POSITION_NAMES.get(radar_pos, '')}){RST}"
                )
                for p in sorted(RADAR_POSITIONS.keys()):
                    if p == radar_pos:
                        print(
                            f"    {GRN}{p}: {RADAR_POSITION_NAMES.get(p, ''):<15s} -> {RADAR_POSITIONS[p]['DST_IP']} <-- active{RST}"
                        )
                    else:
                        print(
                            f"    {p}: {RADAR_POSITION_NAMES.get(p, ''):<15s} -> {RADAR_POSITIONS[p]['DST_IP']}"
                        )
            else:
                try:
                    new_pos = int(parts[1])
                    if new_pos not in RADAR_POSITIONS:
                        print(
                            f"  [ERROR]  Position {new_pos} not configured. Available: {sorted(RADAR_POSITIONS.keys())}"
                        )
                        continue
                    pos_config = get_position_config(new_pos)
                    radar_pos = new_pos
                    pos_name = RADAR_POSITION_NAMES.get(radar_pos, "")
                    print(f"  {GRN}Switched to Position {radar_pos} ({pos_name}){RST}")
                    print(
                        f"  {pos_config['SRC_IP']}:{pos_config['SRC_PORT']} -> {pos_config['DST_IP']}:{pos_config['DST_PORT']}"
                    )
                    logger.info(
                        f"Switched to Position {radar_pos} ({pos_name}): {pos_config['SRC_IP']}:{pos_config['SRC_PORT']} -> {pos_config['DST_IP']}:{pos_config['DST_PORT']}"
                    )
                except ValueError:
                    print(f"  [ERROR]  Invalid position: {parts[1]}")
            continue
        elif cmd.startswith("ctr"):
            # STANDALONE "ctr" COMMAND
            # "ctr"    -> show the current global message counter value
            # "ctr 42" -> permanently set the global counter to 42
            # NOTE: This is different from inline "| ctr N" which is one-shot (doesn't change global).
            parts = cmd.split()
            if len(parts) == 1:
                print(f"  {WHT}Message counter:{RST} {xcp_msg_counter}")
                print(f"  {DIM}Range: 0–65535 (uint16, wraps automatically){RST}")
            else:
                try:
                    new_ctr = int(parts[1])
                    if new_ctr < 0 or new_ctr > 0xFFFF:
                        print(f"  [ERROR]  Counter must be 0–65535 (uint16). Got: {new_ctr}")
                        continue
                    xcp_msg_counter = new_ctr
                    print(f"  {GRN}Message counter set to {xcp_msg_counter}{RST}")
                    logger.info(f"Message counter manually set to {xcp_msg_counter}")
                except ValueError:
                    print(f"  [ERROR]  Invalid number: {parts[1]}")
            continue
        elif cmd == "last":
            # RESEND THE LAST COMMAND
            # Retrieves the most recent payload from history and sends it again.
            # Any inline overrides (pad/pos/ctr) from the current line still apply.
            if history:
                payload = history[-1]
                print(f"  {DIM}Resending: {_clean_hex(payload)}  ({xcp_cmd_name(payload)}){RST}")
            else:
                print("  No previous command found.")
                continue
        else:
            # NOT A BUILT-IN COMMAND -> treat as raw hex bytes.
            # Parse the hex string into bytes and add it to history.
            try:
                payload = parse_hex_input(user_input)
                history.append(payload)
            except ValueError as e:
                print(f"  [ERROR]  Invalid input: {e}")
                continue

        # Send the XCP command via UDP, applying any one-shot pad/pos/ctr overrides from inline pipes
        udp(payload, pad_override, pos_override, ctr_override)

    # ═════════════════════════════════════════════════════════════════════════
    #  SESSION END -- Write summary to log file
    # ═════════════════════════════════════════════════════════════════════════
    # When the user exits (types 'exit' or Ctrl+C), we write a complete session
    # summary to the log file. This has 4 sections:
    #   Section 1: Transaction Summary Table -- one row per command, compact
    #   Section 2: Error/Failure Detail     -- unique errors grouped (not repeated)
    #   Section 3: RAW TX/RX Pairs          -- full hex dumps of every message
    #   Section 4: Session Statistics        -- counts and latency stats
    if transaction_log:
        sep = "=" * 110  # Thick separator line for major sections
        dash = "-" * 110  # Thin separator line for subsections

        # ---- SECTION 1: Transaction Summary Table ----
        # A compact, aligned table showing every command sent during the session.
        # Columns: #, Time, Counter, Position, Command Name, Short Result, Latency
        # The 'result' field uses the SHORT form (e.g. 'ERR:OUT_OF_RANGE') so the
        # table stays aligned and readable, unlike the old format that overflowed.
        logger.info("")
        logger.info(sep)
        logger.info("SESSION TX/RX LEDGER -- All transactions in sequence")
        logger.info(sep)
        logger.info(
            f"  {'#':>4s}  {'Time':<16s}  {'Ctr':>5s}  {'Pos':>3s}  {'Command':<20s}  {'Result':<22s}  {'Latency':>12s}"
        )
        logger.info(dash)
        for i, txn in enumerate(transaction_log, 1):
            # Format latency: right-aligned with 3 decimal places, or 'N/A' for timeouts
            lat = (
                f"{txn['latency_ms']:>8.3f} ms" if txn["latency_ms"] is not None else "       N/A"
            )
            logger.info(
                f"  {i:4d}  {txn['timestamp']:<16s}  {txn['ctr']:5d}  {txn['pos']:3d}  {txn['cmd']:<20s}  {txn['result']:<22s}  {lat:>12s}"
            )
        logger.info(dash)

        # ---- SECTION 2: Error/Failure Detail ----
        # Instead of repeating the same long error description on every row (which made
        # the old log messy), we group unique errors and show each one ONCE.
        # For each unique error code, we list:
        #   - The short code (e.g. ERR:OUT_OF_RANGE)
        #   - The full description (e.g. "Command parameter(s) out of range.")
        #   - Which transaction numbers hit this error (e.g. #3, #4, #5, ...)
        # Collect unique non-OK results and list which transaction #s hit each one
        error_groups = OrderedDict()  # Preserves the order errors were first seen
        for i, txn in enumerate(transaction_log, 1):
            if txn["result"] != "OK":  # Only track non-OK results
                key = txn["result"]  # e.g. 'ERR:OUT_OF_RANGE'
                if key not in error_groups:
                    # First time seeing this error -- create a new group
                    error_groups[key] = {"full": txn.get("result_full", txn["result"]), "txns": []}
                # Add this transaction number to the group
                error_groups[key]["txns"].append(i)
        if error_groups:
            logger.info("")
            logger.info("ERROR/FAILURE DETAIL:")
            logger.info(dash)
            for code, info in error_groups.items():
                txn_nums = ", ".join(f"#{n}" for n in info["txns"])
                logger.info(f"  [{code}]")
                logger.info(f"    Description : {info['full']}")
                logger.info(f"    Occurrences : {len(info['txns'])}x -- {txn_nums}")
                logger.info("")
            logger.info(dash)

        # ---- SECTION 3: RAW TX/RX Pairs ----
        # Shows the complete raw bytes (header + payload) for every TX and RX.
        # Two lines per transaction: TX on line 1, RX on line 2.
        # This is useful for deep debugging with a protocol analyzer.
        logger.info("")
        logger.info("RAW TX/RX PAIRS:")
        logger.info(dash)
        logger.info(f"  {'#':>4s}  {'Ctr':>5s}  {'Command':<20s}  {'Status':<10s}  {'TX RAW'}")
        logger.info(f"  {'':>4s}  {'':>5s}  {'':>20s}  {'':>10s}  {'RX RAW'}")
        logger.info(dash)
        for i, txn in enumerate(transaction_log, 1):
            # Mark each pair as OK or FAIL for quick visual scanning
            status = "OK" if txn["result"] == "OK" else "FAIL"
            logger.info(
                f"  {i:4d}  {txn['ctr']:5d}  {txn['cmd']:<20s}  {status:<10s}  TX: {txn['tx_raw']}"
            )
            logger.info(f"  {'':>4s}  {'':>5s}  {'':>20s}  {'':>10s}  RX: {txn['rx_raw']}")
        logger.info(dash)

        # ---- SECTION 4: Session Statistics ----
        # Quick summary: how many commands were sent, how many succeeded/failed.
        logger.info("")
        total = len(transaction_log)
        ok_count = sum(1 for t in transaction_log if t["result"] == "OK")  # Positive responses
        err_count = sum(
            1 for t in transaction_log if t["result"].startswith("ERR")
        )  # Error responses
        timeout_count = sum(1 for t in transaction_log if t["result"] == "TIMEOUT")  # No response
        other_count = total - ok_count - err_count - timeout_count  # Everything else
        logger.info(
            f"SESSION SUMMARY: {total} commands | {ok_count} OK | {err_count} ERR | {timeout_count} TIMEOUT | {other_count} OTHER"
        )
        if transaction_log:
            # Calculate latency statistics (min/max/avg) across all successful responses
            latencies = [t["latency_ms"] for t in transaction_log if t["latency_ms"] is not None]
            if latencies:
                logger.info(
                    f"LATENCY STATS : min={min(latencies):.3f} ms | max={max(latencies):.3f} ms | avg={sum(latencies)/len(latencies):.3f} ms"
                )
        logger.info(sep)
    else:
        logger.info("Session ended with no commands sent.")
    logger.info("XCP over Ethernet -- UDP Command Tool session ended.")
