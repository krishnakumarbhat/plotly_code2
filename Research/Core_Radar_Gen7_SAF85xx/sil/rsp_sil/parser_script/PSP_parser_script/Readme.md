
RSP SIL Automation Script
=========================

## Overview

This automation script executes the SIL flow end to end. Bin file generation and SIL build do not need to be done separately. With the logs and `.exe` file in place, the automation script can be run to generate output in the respective folder.

SIL Test Execution Steps
=======================

This workflow is for running the MATLAB parser script that prepares the input bin files, executes the SIL test binary, and validates the generated SIL/EMB output.

Prerequisites
-------------
1. Update the bin directory paths in:
   `sil/rsp_sil/main/rsp_wrapper_interface/rdd_sil_interface/test/main.cpp`
   with your actual local path.

2. Open the repository root and build the required SIL target first.
   Example:
   bazelisk build //sil/rsp_sil/main/rsp_wrapper_interface/rdd_sil_interface/test:rdd_sil_Test --variant=srr7p --@spbb//common:use_bbe_cstub_simulator=True --@afbb//module/_common:sil_config_enable=True --//sil/rsp_sil/main/sil_wrapper_interface:enable_logging=True --psp_sil_config=True --copt="-g" --copt="-O0" -c dbg

3. Confirm the built binary exists under:
   bazel-bin/sil/rsp_sil/main/rsp_wrapper_interface/rdd_sil_interface/test/

4. Make sure the input data folder for the selected variant exists under:
   sil/rsp_sil/data_bin/<variant>/

Step-by-step execution
----------------------
1. Open the file in MATLAB:
   sil/rsp_sil/parser_script/PSP_parser_script/main.m

2. Update the repository path in main.m:
   - wkspace = "C:\Core_Radar_Gen7_SAF85xx";
   - Use the actual local path of your repo.

3. Verify the executable path in main.m:
   - sil_exe_path should point to the built SIL binary, for example:
     "C:\Core_Radar_Gen7_SAF85xx\bazel-bin\sil\rsp_sil\main\rsp_wrapper_interface\rdd_sil_interface\test\rdd_sil_Test.exe"

4. If required for the selected variant, update the binary input paths in main.cpp (or the relevant generated input configuration) so they match the variant being tested.

5. Run main.m in MATLAB.
   - A variant selection dialog will appear.
   - Select the target variant, for example: srr7p, flr7, or srr7e.

6. When prompted, select the PCAP file to parse.
   - Use the PCAP captured for the same variant and configuration being tested.

7. When prompted, select the corresponding Stream Definition folder.
   - This folder contains the stream definition files required to parse the PCAP.

8. Enter the required frame number when prompted.
   - The script reads the selected frame and generates the SIL input bin files for that frame.
   - Can cross verify in bazel-bin/sil/rsp_sil/main/rsp_wrapper_interface/rdd_sil_interface/test/
   - sil/rsp_sil/data_bin/<variant>/
      - Generated bin files under `sil/rsp_sil/data_bin/<variant>/`:
         - `rdd_data/rdd_input.bin`
         - `rdd_data/look_data_input.bin`
         - `rdd_data/stream_hdr.bin`
         - `rfft_data/rfft_input.bin`
         - `vse_data/vse_stream_input.bin`
         - `psp_data/psp_input.bin`
         - `psp_data/m2d_onetime_mounting_input.bin`

9. Wait for the script to complete.
   - It builds the relevant wrapper, runs the SIL executable, and generates output files.

11. Verify the EMB and SIL output files created during execution.
   - New output files may be generated in the repo and the bazel output directory.
   - EMB output files are generated in: bazel-bin/sil/rsp_sil/main/rsp_wrapper_interface/rdd_sil_interface/test/
   - Expected EMB files: da_out_emb.txt, sa_out_emb.txt, rc_out_emb.txt, id_out_emb.txt

Notes
-----
- Always use the PCAP and stream definitions that match the same variant being tested.
- If the script fails to find files, check:
  - wkspace path
  - sil_exe_path
  - selected variant
  - streamdef folder path
  - the corresponding data_bin path for that variant
- The script expects the built SIL executable to exist before running MATLAB.
