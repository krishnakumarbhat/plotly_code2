
PSP_parser_script - Gen8 iND13400 SIL Testing Framework
=========================================================

OVERVIEW
--------
This folder contains the MATLAB-based PSP (Pre-Signal Processing) parser script
for the Gen8 iND13400 radar SIL (Software-in-Loop) testing framework. It reads
PCAP data files, parses them, and generates binary test data for the SIL
executable.

QUICK START - RUNNING THE SIL PIPELINE
======================================

Prerequisites:
1. MATLAB R2021a or later installed
2. SIL repository built: Run "bazelisk build //:gen8" before proceeding
3. PCAP test data file (*.pcap format)

Steps to Run:
1. Open MATLAB and navigate to this folder (PSP_parser_script/)
2. Execute main script:
   >> main()

3. Follow the interactive prompts:
   - Select workspace path: (defaults to current repo if not set)
   - Select PCAP file: Choose your test data file
   - Select frame number: Enter frame index to process

4. Output binary files are generated in the working directory

CORE FILES (DO NOT REMOVE)
==========================
These files are ESSENTIAL for the SIL pipeline:

Entry Point:
  - main.m                                Main orchestration script

RDD Data Writers:
  - write_rdd_data.m                      Writes raw radar detection data
  - write_stream_hdr_data.m               Writes stream header metadata
  - write_look_data.m                     Writes look direction vectors
  - write_vse_data.m                      Writes vehicle state/parameters
  - write_rfft_data.m                     Writes FFT processed data
  - write_psp_data.m                      Writes pre-signal processing data

Embedded Output Writers:
  - write_alignment_emb_out_parser.m      Alignment embedded output
  - write_rc_emb_out_parser.m             Rate control embedded output
  - write_id_emb_out_parser.m             Identifier embedded output
  - write_opmode_data_qualifier_data.m    Operation mode & data qualifiers

Data Parsing:
  - read_pcap_data.m                      Reads PCAP files
  - parse_pcap_data.m                     Core PCAP parsing logic
  - get_user_frame_index.m                User frame selection helper

COMMON CONFIGURATION
====================

Update paths in main.m if needed:

1. Workspace Path:
   wkspace = "C:\Workspace\Core_Radar_Gen8_iND13400"

2. SIL Executable Path:
   sil_exe_path = "bazel-bin\sil\rsp_sil\main\rsp_wrapper_interface\rdd_sil_interface\test\rdd_sil_Test.exe"

3. Output Directory:
   Default: Current working directory where main.m is located
   Custom: Modify output_path variable in main.m

TROUBLESHOOTING
===============

Problem: "PCAP file not found"
  - Ensure PCAP file is in working directory or full path is provided

Problem: "MATLAB cannot find function"
  - Add PSP_parser_script folder to MATLAB path:
    >> addpath(genpath(pwd))

Problem: "SIL executable not found"
  - Build SIL first: bazelisk build //:gen8
  - Verify sil_exe_path in main.m matches actual location

Problem: "Memory issues with large PCAP files"
  - Process one frame at a time
  - Use Mat file caching if available (srr8p.mat)

Last Updated: March 2026
Repository: Core_Radar_Gen8_iND13400
