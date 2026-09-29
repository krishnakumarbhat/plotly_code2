import os
from typing import List, Dict, Any, Union, BinaryIO

import numpy as np


# Number of bytes allocated to each signal name
SIGNAL_NAME_NUM_BYTES = 256

# Maximum number of nested arrays
MAX_LEVELS = 6

# Private variables that need to be shared between functions
__repeat_count = None
__repeat_count_all = None
__repeat_skip = None
__repeat_first = None
__repeat_level = None
__valid_signals = None


def _data_type(f_ext: str) -> "np.dtype":
    """Gets the data type of the data in the bin file

    Args:
        f_ext (str): The file extension.

    Returns:
        np.dtype: the data type of the data in the bin file
    """
    # Data in .bin files is float64; for .bin32 files it's float32
    dtype = np.float64
    if f_ext == ".bin32":
        dtype = np.float32

    return dtype


def _read_header(f: BinaryIO, dtype: "np.dtype") -> List[str]:
    """Reads the header information from the bin file.

    Args:
        f (BinaryIO): A file handle pointing to the bin file.
        dtype (np.dtype): The data type of data.

    Returns:
        List[str]: The list of all signals in the bin file.
    """
    # Read number of signals in the file
    num_signals = int(np.fromfile(f, dtype=dtype, count=1))

    # Prepare to read header
    global __repeat_count, __repeat_count_all, __repeat_skip, __repeat_first, __repeat_level, __valid_signals
    __repeat_count = np.zeros((num_signals,), dtype=int)
    __repeat_count_all = np.zeros((num_signals, MAX_LEVELS), dtype=int)
    __repeat_skip = np.zeros((num_signals, MAX_LEVELS), dtype=int)
    __repeat_first = np.zeros((num_signals,), dtype=int)
    __repeat_level = np.zeros((num_signals,), dtype=int)
    __valid_signals = np.zeros((num_signals,), dtype=bool)
    curr_repeat_start = np.zeros((MAX_LEVELS,), dtype=int)
    curr_repeat_counter = np.zeros((MAX_LEVELS,), dtype=int)
    curr_repeat_count = np.zeros((MAX_LEVELS,), dtype=int)
    curr_repeat_skip = np.zeros((MAX_LEVELS,), dtype=int)
    level = 0
    start = 0

    # Read header
    signal_names = []
    for i in range(num_signals):
        # 256 bytes are allocated for the signal name. Unused bytes are
        # masked out
        signal_bytes = np.fromfile(f, dtype=np.byte, count=SIGNAL_NAME_NUM_BYTES)
        signal_bytes = signal_bytes[signal_bytes != 0]
        signal_name = signal_bytes.tobytes().decode()

        # Add signal name to the list of signals
        signal_names.append(signal_name)

        if signal_name[:6] == "REPEAT":
            # Start of signal(s) that are array(s). All signals between REPEAT and ENDREPEAT are
            # arrays
            curr_repeat_start[level] = i
            curr_repeat_counter[level] = 0
            curr_repeat_skip[level] = 0
            curr_repeat_count[level] = int(signal_name[6:])
            level += 1
        elif signal_name[:10] == "END_REPEAT":
            # Array(s) end
            level -= 1
            __repeat_skip[curr_repeat_start[level] + 1 : i, level] = curr_repeat_skip[level]
            start += curr_repeat_skip[level] * (curr_repeat_count[level] - 1)
            if level > 0:
                curr_repeat_skip[level] += curr_repeat_count[level] * curr_repeat_counter[level]
                curr_repeat_counter[level] = curr_repeat_count[level] * curr_repeat_counter[level]
        else:
            # Valid signal
            if level == 0:
                __repeat_count[i] = 1
            else:
                __repeat_count[i] = np.prod(curr_repeat_count[:level])
                __repeat_count_all[i, :level] = curr_repeat_count[:level]
                curr_repeat_counter[level - 1] += 1
                curr_repeat_skip[level - 1] += 1

            __valid_signals[i] = True
            __repeat_first[i] = start
            __repeat_level[i] = level
            start = start + 1

    return signal_names


def _read_data(f: BinaryIO, dtype: "np.dtype") -> "np.ndarray":
    """Read the raw data contained in a bin file.

    Args:
        f (BinaryIO): A file handle pointing to the bin file.
        dtype (np.dtype): The data type of data.

    Raises:
        Exception: If the raw data cannot be reshaped into the expected shape. This is likely the
            result of a corrupted bin file or a resim that did not finish cleanly.

    Returns:
        np.ndarray: The raw data
    """
    # Read the whole data block
    data = np.fromfile(f, dtype=dtype)

    # Reshape the data
    global __repeat_count
    n_rows = np.sum(__repeat_count)
    try:
        data = data.reshape((n_rows, -1), order="F")
    except ValueError as e:
        f_name = os.path.basename(f.name)
        raise Exception(
            f"Corrupted data in file '{f_name}'! Has this file been fully writtenpy?"
        ).with_traceback(e.__traceback__)

    return data


def _parse_data(signal_names: List[str], raw_data: "np.ndarray", sample_indexing: bool) -> Dict:
    """Pases raw data into a Python dictionary.

    Args:
        signal_names (List[str]): The list of all signal names in the bin file.
        raw_data (np.ndarray): Data contained in the bin file.
        sample_indexing (bool): Whether to return the dictionary indexed by sample or by
            signal.

    Returns:
        Dict: The data contained in the bin file indexed by signal or sample.
    """
    global __repeat_count_all, __repeat_skip, __repeat_first, __repeat_level, __valid_signals

    ret_data = {}

    # Create a dictionary for each sample if sample_indexing is set to True
    num_samples = raw_data.shape[1]
    if sample_indexing:
        for sample_idx in range(num_samples):
            ret_data[sample_idx] = {}

    # Iterate through all signals and parse each into the dictionary to be returned
    for i, signal in enumerate(signal_names):
        if __valid_signals[i]:
            idx = __repeat_first[i]
            level = __repeat_level[i]

            # Compute all indices of all elements of an array
            for j in reversed(range(level)):
                idx += __repeat_skip[i, j] * np.arange(__repeat_count_all[i, j])

            # Get the data that belongs to this signal
            tmp_data = raw_data[idx, :].T

            # For an array, we need to reshape it to the expected dimensions
            if level > 0:
                shape = (-1,) + tuple(__repeat_count_all[i, np.arange(level)])
                tmp_data = tmp_data.reshape(shape, order="F")

            # Store the data in the dictionary to be returned. The data is either indexed by signal
            # or sample (and then signal)
            if sample_indexing:
                for sample_idx in range(num_samples):
                    ret_data[sample_idx][signal] = tmp_data[sample_idx]
            else:
                ret_data[signal] = tmp_data

    return ret_data


def read(file_path: Union[str, "os.PathLike[Any]"], sample_indexing: bool = False) -> Dict:
    """Read a bin file.

    This function is based on the MatLab script `10027594_08_BIN_WRITER/Matlab/read_log_data.m`.
    Setting `sample_indexing = False` returns a dictionary with data indexed by signal name;
    accessing the data is as follows:

    ```py
    import pybin.read as pbr

    bin_path = "path/to/example.bin"
    pbr_data = pbr.read(bin_path, sample_indexing=True)

    # Get a list of all scan_index
    scan_index_hist = pbr_data["scan_index"]

    # Get the scan_index of the 3rd sample
    scan_index = scan_index_hist[2]
    ```

    Setting `sample_indexing = True` returns a dictionary with data indexed by sample; accessing the
    data is as follows:

    ```py
    import pybin.read as pbr

    bin_path = "path/to/example.bin"
    pbr_data = pbr.read(bin_path, sample_indexing=True)

    # Get all signals of the 3rd sample
    all_signals = pbr_data[2]

    # Get the scan_index of the 3rd sample
    scan_index = all_signals["scan_index"]
    ```

    Args:
        file_path (Union[str, "os.PathLike[Any]"): The path to the bin file to read.
        sample_indexing (bool, optional): Whether to return the dictionary indexed by sample or by
            signal. Defaults to False, indexes by signal.

    Returns:
        Dict: The data contained in the bin file indexed by signal or sample.
    """
    # Split the filepath into components for later usage
    f_parts = os.path.normpath(file_path).split(os.path.sep)
    f_name, f_ext = os.path.splitext(f_parts[-1])

    dtype = _data_type(f_name)

    with open(file_path, "rb") as f:
        raw_signal_names = _read_header(f, dtype)
        raw_data = _read_data(f, dtype)

    data = _parse_data(raw_signal_names, raw_data, sample_indexing)

    return data
