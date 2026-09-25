import os
from typing import Any, Union

import numpy as np

from .read import _read_header, _read_data, _data_type


def get_num_samples(file_path: Union[str, "os.PathLike[Any]"]) -> int:
    """Returns the number of samples in the bin file.

    Args:
        file_path (Union[str, "os.PathLike[Any]"): The path to the bin file.

    Returns:
        int: The number of samples.
    """
    # Split the filepath into components for later usage
    f_parts = os.path.normpath(file_path).split(os.path.sep)
    f_name, f_ext = os.path.splitext(f_parts[-1])

    dtype = _data_type(f_name)

    with open(file_path, "rb") as f:
        _ = _read_header(f, dtype)
        raw_data = _read_data(f, dtype)

    return raw_data.shape[1]


def crop(
    file_path: Union[str, "os.PathLike[Any]"],
    new_num_samples: int,
    new_file_path: Union[str, "os.PathLike[Any]"],
):
    """Crop the number of samples of a bin file.

    Keeps a specified number of samples from the original bin file and saves the cropped bin file in
    a specified path.

    Args:
        file_path (Union[str, "os.PathLike[Any]"): The path to the bin file to be cropped.
        new_num_samples (int): The number of samples to keep, starting from the beginning.
        new_file_path (Union[str,"os.PathLike[Any]"): The file path to save the cropped bin file.

    Raises:
        ValueError: If `new_num_samples` is less than 1.
        ValueError: If `new_num_samples` is greater or equal to the nubmer of samples in the bin
            file.
    """
    if new_num_samples < 1:
        raise ValueError(f"'new_num_samples' ({new_num_samples}) must be greater than 0")

    # Split the filepath into components for later usage
    f_parts = os.path.normpath(file_path).split(os.path.sep)
    f_name, f_ext = os.path.splitext(f_parts[-1])

    dtype = _data_type(f_name)

    with open(file_path, "rb") as f:
        _ = _read_header(f, dtype)
        header_len = f.tell()
        raw_data = _read_data(f, dtype)

        num_samples = raw_data.shape[1]
        if new_num_samples >= num_samples:
            raise ValueError(
                f"'new_num_samples' ({new_num_samples}) must be less than the existing "
                "number of samples in the bin file ({num_samples})"
            )

        new_raw_data = raw_data[:, :new_num_samples]
        data_len = np.dtype(dtype).itemsize * new_raw_data.size
        total_len = header_len + data_len

        f.seek(0)
        new_data = np.fromfile(f, dtype=np.byte, count=total_len)

    new_data.tofile(new_file_path)
