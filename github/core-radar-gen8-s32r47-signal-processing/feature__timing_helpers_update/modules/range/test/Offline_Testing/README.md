# RFFT Offline Testing

This folder runs RFFT processing on the target using ADC data loaded into DDR through Lauterbach Trace32, then compares the embedded output with the MATLAB reference.

## Prerequisites

- MATLAB
- Lauterbach Trace32 installed at `C:\T32`
- Connected and powered target board
- Target binaries built and available to the scripts under `../evb_diag_tool_profile_spt/scripts`
- Input file `../test_data/ADC_Data.mat` containing `data_cube_raw`

## Required Setup

Before building the target, enable offline ADC injection in `../../imp/src/nxp/range_spt_0.pspt`:

```text
.set OFFLINE_INJECTION_MODE, 1
```

Rebuild the target binaries after changing this macro.

## Run

Start MATLAB, change to this directory, and run:

```matlab
cd modules/range/test/Offline_Testing
Offline_testing_rfft_main
```

The workflow:

1. `generate_adc_bin` converts `data_cube_raw` to Q15, Tile-16, little-endian `int16` data and writes `adc_data.bin`.
2. Trace32 flashes and starts the target.
3. `adc_data.bin` is loaded at DDR address `0x60000000`.
4. Embedded compressed RFFT output and chirp scaling are read back.
5. `Compare_Rfft_results` compares the embedded results with the MATLAB reference.

## Manual Dump Comparison

Existing Trace32 `.txt` dumps can be compared without flashing the target:

1. Place the RFFT and chirp-scaling dump files in this directory.
2. Set `auto_flash = false` in `Offline_testing_rfft_main.m`.
3. Set `filename_rfft` and `filename_chirpscaling` to the dump filenames.
4. Run `Offline_testing_rfft_main` from this directory.

The script reads the supplied dumps and runs `Compare_Rfft_results` without connecting to or flashing the target.

## Configuration

The main settings are in `Offline_testing_rfft_main.m`:

- `max_range_bins`: number of range bins to compare
- `numChirps`: number of chirps to read and compare

Run the script with this folder as MATLAB's current directory because the Trace32 utilities and generated binary are resolved from `pwd`.

## Suggestion incase of Credential popups

- Run generate_t32_prototype.m script once. Enter credentials for running this script alone.

- After this, you wont be getting the popup issue, even when we log in after sign off or pc restart
