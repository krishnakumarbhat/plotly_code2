# Doppler Module EVB Testing

## Steps

1. **Create binary file**
   Run the MATLAB script to generate input data:
   ```
   modules\doppler\test\evb_diag_tool_profile_spt\matlab\create_rfft_dc_bin.m
   ```

2. **Build the project**
   ```bash
   bazelisk build -c dbg //modules/doppler/test/evb_diag_tool_profile_spt:doppler_profile_spt
   ```
   > Use `-c dbg` for debugging

3. **Flash using Trace32**
   Flash the binary using T32. It will stop at the `execute_test()` function breakpoint.

4. **Load binary file in T32**
   ```
   Data.LOAD.Binary "..\matlab\DATA.bin" 0x60000000 /NoClear
   ```

5. **Start execution**
   Continue execution in T32.

6. **Save output data**
   Once execution completes, save the output to a binary file:
   ```
   For single rbin
   DATA.SAVE.Binary "..\matlab\OUTPUT.bin" 0x615C0000--0x61637FFF
   For 232 rbins
   DATA.SAVE.Binary "..\matlab\OUTPUT.bin" 0x615C0000--0x6827FFFF
   ```

7. **Verify output**
   Run the verification script:
   ```
   modules\doppler\test\evb_diag_tool_profile_spt\matlab\output_verification.m
   ```
   - It will prompt for the output binary file — point it to the saved `OUTPUT.bin`
   - Converts binary data to decompressed DFFT outputs and AFFT magnitude outputs
   - Compares results with spec data using a **1 dB threshold**
