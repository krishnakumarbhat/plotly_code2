# pybin

A Python package to read bin files. Writing bin files will (hopefully) be supported in the future.

This package is based on the MatLab script found in `10027594_08_BIN_WRITER\Matlab\read_log_data.m`.

## Installation

Procedure:

- Install Python 3.7.1 or higher
- Install pip

## Usage

1. (Optional, but recommended) Make a virtual environment. Run the following commands in a command-line:

    ```bat
    python -m venv venv
    call venv\Scripts\activate
    python -m easy_install -U pip
    ```

2. Install pybin: `pip install -e <path-to-this-package>`

### Read

Code snippet for reading a file:

```py
import pybin.read as pbr

bin_path = "path/to/example.bin"
pbr_data = pbr.read(bin_path, sample_indexing=True)
```

`sample_indexing` is equivalent to `aos_flag` in the MatLab script that this package is based on. Setting `sample_indexing = False` returns a dictionary with data indexed by signal name; accessing the data is as follows:

```py
# Get a list of all scan_index
scan_index_hist = pbr_data["scan_index"]

# Get the scan_index of the 3rd sample
scan_index = scan_index_hist[2]
```

Setting `sample_indexing = True` returns a dictionary with data indexed by sample; accessing the data is as follows:

```py
# Get all signals of the 3rd sample
all_signals = pbr_data[2]

# Get the scan_index of the 3rd sample
scan_index = all_signals["scan_index"]
```

### Crop

Code snippet from cropping a bin file:

```py
import pybin.crop as pbc

bin_path = "path/to/example.bin"
num_samples = pbc.get_num_samples(bin_path)
pbc.crop(bin_path, num_samples - 10, bin_path)
```

## Development

- Create a virtual environment by running: `make_venv.bat`
- Preferably use VS Code to make changes or use another IDE but respect the code style
- To run the tests use the command: `python -m unittest discover tests`
- Some tests validate the implementation against the MatLab script. The validation has two stages:
    1. In MatLab: a bin file is read with the MatLab script and its output is saved to a `.mat` file. This is done by running the script `tests/bin2mat_ground_truth.m`.
    2. In pybin: the same bin file is read with pybin; the `.mat` file, saved in the previous stage, is read using a third-party package; the two are compared against each other and much match exactly. `tests/test_read.py` is responsible for this.
