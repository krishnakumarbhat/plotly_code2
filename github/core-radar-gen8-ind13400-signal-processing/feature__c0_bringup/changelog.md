# CHANGELOG

## Version 0.0
### Revision 0
- Initial version of SPBB using the versioning scheme.
- Helper modules for timing, profiling, sysmem, and RA are ready for use

## Version 0.1
### Revision 0
- Range process module using FITO mode
- MIPI and RA APIs added for enabling the IRQ for the BBE32.

### Revision 1
- Range process module changes for Integration with Application

## Version 0.2
### Revision 0
- Range Process module changes for supporting the ADC chirp logging

### Revision 1
- Changes in Range proc SPBB for ADC logging to work with integration
 with Application state machine.

## Version 0.3
### Revision 0
- Doppler Process module Integration with Application

## Version 0.4
### Revision 0
- Sync with latest SPBB updates & resolved dependencies with Application repo build

## Version 0.5
### Revision 0
- Update Doppler processing to support REST and non-REST processing
- Updates in FP module for functional testing

## Version 0.6
### Revision 0
- Added Second Pass Implementation

## Version 0.7
### Revision 0
- Bug fix in Second Pass source files for application

## Version 0.8
### Revision 0
- Sysmem Config Changes to support Application integration

## Version 0.9
### Revision 0
- Range process done should not do full reset of the RA, this clears the min shift that is needed for doppler processing.

## Version 0.10
### Revision 0
- Update the RA helpers CQMEM operations to not use memcpy, but instead use the volatile pointer and write directly.

## Version 0.11
### Revision 0
- Added the functions to extract bvs
- Updated the interface to output the rdd_bvs to the correct datatype
- Changed the range process ra configuration to remove a large stack allocation
## Version 0.12
### Revision 0
- Added helper function to read min chirp scale value from RA

## Version 0.13
### Revision 0
- Added extract compressed Bv and RDD FP XCP changes

## Version 0.14
### Revision 0
- Minor changes for integration of IDM and DC Comp

## Version 0.15
### Revision 0
- Data type changes for integration of SMC in doppler and secondpass

## Version 0.16
### Revision 0
- Fix unaligned vector copy for non multiple of 32 bytes

## Version 0.17
### Revision 0
- Changed test data due to BPM changes, Comp level changes for range proc, Round and Saturation mode changes

## Version 0.18
### Revision 0
- Updated range processing commands to be directly from Matlab models.
- Updated MIPI transpose bypass to always be active in the ADC logging mode.
- Only reset the min chirp scaling in the initial chirp, not in every chirp command.

## Version 0.19
### Revision 0
- Updated range processing XCP command to check for NULL pointers.
- RDD first pass XCP updates
   - Output rdop_avg for selected rbin based on XCP range selection
   - Output rdop_avg for selected dbin based on XCP doppler selection
   - Add selection of CI/NCI output for RDD XCP

## Version 0.20
### Revision 0
- RDD first pass bugfix for updating number of detections.

## Version 0.21
### Revision 0
- RDD first pass bugfix for outputting the rbinest and dbinest.

## Version 0.22
### Revision 0
- RDD Secondpass fix to match spec.

## Version 0.23
### Revision 0
- Fix xcp beam vector extraction mapping to p_xcp_raw_bv

## Version 0.24
### Revision 0
- RDD first pass update for RBIN0 and Last Range bin

## Version 0.25
### Revision 0
- Add the range processing lightly compressed FFT output for a selected range bin over XCP
- Update the circular shift NCI and CI operations to allow +/- DDMA offsets

## Version 0.26
### Revision 0
- Sweep_BW: Revert k_sensitivity_sf_usc to u6p10_T and update the tests

## Version 0.27
### Revision 0
- Fix bug in range processing related to the output of NCI bexp
- Fix bug with copy of data to the output buffer for lightly compressed data

## Version 0.28
### Revision 0
- Doppler: Added Doppler_Ra_Reset_Min_Shift_Set for offline injection and UT

## Version 0.29
### Revision 0
- Timing and profiling helpers updated to use integers instead of floats

## Version 0.30
### Revision 0
- Doppler: Removed Ra_Reset from Doppler_Ra_Reset_Min_Shift_Set for offline injection and UT
- Doppler: Moved Apis called by application from doppler_process_prv.h to doppler_process.h

## Version 0.31
### Revision 0
Modification to exec spec for the min shift value not applying correctly.
- Range: No longer incrementing the A counter, so value will be 0 at the end of range processing
- Doppler: Moved Apis called by application from doppler_process_prv.h to doppler_process.h

## Version 0.32
### Revision 0
- First pass: Bug fix to use correct bexp value for fractional bin est

## Version 0.33
### Revision 0
- Range: Update compressor rounding and saturation modes
## Version 0.34
### Revision 0
- First pass: Integrate ctl_samp_mode from SMC
## Version 0.35
### Revision 0
- First pass: Allocate memory for CFAR output from Application and dbin estimation bug fix

## Version 0.36
### Revision 0
- Second pass: Update secondpass thresholding to consider hvc and zdb thresholds
## Version 0.37
### Revision 0
- First pass: Removed If checks for RBI0 AA helper and created RDD Execute API for RBIN0
## Version 0.38
### Revision 0
- First pass: Use AA for reading data out of AA

## Version 0.39
### Revision 0
- Doppler Circular Shift - Add support for non-multiple of 16 ddm shift

## Version 0.40
### Revision 0
- Changes to range processing for supporting multiple looks

## Version 0.41
### Revision 0
- Changes to Doppler processing for supporting multiple looks.

## Version 1.00
### Revision 0
- ADC logging support for M_FRAME values not equal to 1024. Only multiples of 8 are supported
- Update the inclusion of the Xtensa libraries. Use the idma-xtos version instead of idma-debug.

## Version 1.01
### Revision 0
- SNR value stuck and was not updating in XCP path. Other values updated in XCP structure as well.

## Version 1.02
### Revision 0
- RDD FP Input Output structure  modification for NCI and CI Parallel use case.

## Version 1.03
### Revision 0
- Disable CI detection processing
- Add max calculation to NCI circular shift and min operation

## Version 1.04
### Revision 0
- Implemented and Verified Max Xput Scenario - static & varying target use case

## Version 1.05
### Revision 0
- Implemented Secondpass CI changes
- Optimized sweepbw with vectorized processing

## Version 1.06
### Revision 0
- Bug fix in sweepbw BUILD dependecy

## Version 1.07
### Revision 0
- ADC logging update to clear unused samples
- Update NCI/CI processing to output CFAR debug thresholds to different buffers.

## Version 1.08
### Revision 0
- Update the Indie SDK reference update to match the application
- Move MIPI debug mode writes outside macro check, so it can be used for ADC injection
- ADC injection enablement - requires Application to enable MIPI debug mode at MIPI configuration time

## Version 2.00
### Revision 0
- Updates to support the building block integration with other building blocks
- Update the building block to use appl_inclusion_dep repository instead of spbb_include
- Update the building block to use bb_cfg repository instead of spbb_cfg

## Version 2.01
### Revision 0
- Update the NCI ghost rejection in RDD FP

## Version 2.02
### Revision 0
- Update to AF sensitivity thold logging, rcs_lut scale changed to u2p30 from u20p12
- Define attributes using SPBB_DOPPLER_ATTRIBUTE_SRAM_FUNCTION for Doppler_Proc_Convert_CI
- Define attributes using SPBB_SECOND_PASS_ATTRIBUTE_SRAM_FUNCTION for Determine_Rwin_Thold and Estimate_Range_and_Range_Rate

## Version 3.0
### Revision 0
- RDD FP CI Integration

## Version 4.00
### Revision 0
- Update to bv variance
- Update MIPI statistics read function to read all the statistics to arrays of statistics per channel, instead of reading a single chirp for 4 channels.
- Update to allow moving functions to different memory sections using macro
- Define attributes using SPBB_FIRST_PASS_ATTRIBUTE_SRAM_FUNCTION for Calculate_BV_Variance
- Add MIPI helpers method to clear the MIPI statistics
- Add spbb_sort.c/h with merge sort and partial sort (ascending sort methods)
- Vectorize the MIPI helper read and store of statistics
- Avoid usage of SRAM memory section, used mempool for bv variance function
- Update for SIL usages in AF testing
- Add MIPI helper function to update the ABS and ABS_DIFF thresholds, the selection of "live" vs SW thresholds for the 4 receive paths, and the log selection

## Version 4.01
### Revision 0
- Implemented 3plane XCP based calibration

## Version 4.02
### Revision 0
- Changes to enable scaling CFAR LUT for xcp xput vary

## Version 4.03
### Revision 0
- CDC packing ut changes

## Version 4.04
### Revision 0
- CDC Selection logic Implementation in SPBB
- Add unit tests for the CDC Selection logic
- Add an optimized selection logic function

## Version 4.05
### Revision 0
- TiCS warning fix for 3-plane calibration in XCP

## Version 4.06
### Revision 0
- CDC integration

## Version 4.07
### Revision 0
- CDC integration

## Version 4.08
### Revision 0
- PCRESIM build for RDD SIL

## Version 4.09
### Revision 0
- Added cpp filetype to be zipped while tag creation

## Version 4.10
### Revision 0
- Included the @appl_inclusion_dep//:spbb_include_h as a dependency for timing_helpers_h

## Version 4.11
### Revision 0
- Add CI macro to enable/disable CI

## Version 4.12
### Revision 0
- ZDB bug Fix and UT Improvements

## Version 4.13
### Revision 0
- Profiling helpers UT implementation

## Version 5.00
### Revision 0
- Cherry picked changes from B0 no-ridm no-map branch into dev for B0 HW switch from A0
- GHW-1698 Setup the BB Repo for Building Block development on B0 FPGA image
- GHW-1699 Changes for running existing range processing on B0 fpga image
- GHW-1754 Mods in Range proc commands for B0 changes
- GHW-1040 Get the doppler building block tests running on B0 FPGA
- GHW-1789 Update RA command config to support doppler on B0 hardware
- GHW-1805 Evaluate the first pass test on the B0 hardware
- GHW-1837 AA Bug Fix for Block Exponent Overflow with B0 FPGA
- GHW-1806 AA bug fix with Local Maxima on B0 FPGA
- GHW-1977 Merge B0 onto A0 and prep for PCE B0 Image support
- GHW-1900 Fix build issue with AA and FP test case on b0plus_bringup baseline
- GHW-2141 Integrate SDK CHA_037D_19 into SPBB repo
- GHW-2567 Updated the SPBB with B0 ASIC luna BBE config, removed A0 switches and builds,updated relevant bazel builds

## Version 5.01
### Revision 0
- Changes for b0 build

## Version 5.02
### Revision 0
- Chandra B0 take latest SDK_v2.40.44

## Version 5.03
### Revision 0
- Bug fix for comparing against ZDB and HVC threshold - Use the rdop_amp value that is scaled with the block exponent
- Bug fix for invalid XCP ADC output data. Due to cache enabled the vector clear operation is causing invalid ADC output.
  Remove the clear operation, since the DPMEM is cleared as part of ADC logging instructions.

## Version 5.04
### Revision 0
- Adj_BV logic merge to dev branch

## Version 5.05
### Revision 0
- Update to bazel mod
- fix failing profile test
- update simulator run to allow running in bazel 7.5 - Profiling files are still created, but not formally managed by bazel

## Version 5.06
### Revision 0
- GHW-2677 Improve the UT coverage from bbe side and spbb

## Version 5.07
### Revision 0
- Add spbb_memmap.h to allow definition of the memory mapping for the read only RA command arrays.

## Version 5.08
### Revision 0
- Updates to Range for RIDM Integration with SMC
- Updates to Doppler for MAP-REST Integration with SMC
- Remove FPGA build support
- REST Threshold bug fix and Bug fixes for the B0plus merge into Dev

## Version 6.00
### Revision 0
- Updates to CFAR, FP, SP for 3D MAP

## Version 6.01
### Revision 0
- XCP Updates

## Version 6.02
### Revision 0
- Allow moving RA command arrays to system memory through use of spbb_memmap.h
- Update port check in system memory helper to check for contiguous memory usage.

## Version 6.03
### Revision 0
- Added XCP decompression updates
- Updated the doppler commandes for estimator right shift value.

## Version 6.04
### Revision 0
- Bug fix in range processing for matching RIDM outputs for all 4 looks.

## Version 6.05
### Revision 0
- Change CFAR threshold output to 32 bit
- Update first pass to saturate CFAR threshold to 16 bit value for thresholding current bin

## Version 6.06
### Revision 0
- CFAR bugfix for full matching
- Second pass UT update
- First pass update to support saturation for CFAR and FP threshold.

## Version 6.07
### Revision 0
- GHW-3050 Merge the updated UT files to application

## Version 6.08
### Revision 0
- Removed the unused pointers in second_pass.c

## Version 6.09
### Revision 0
- Changes in FP BB for xcp snr issues.

## Version 6.10
### Revision 0
- Fixig failed UT for FP.

## Version 6.11
### Revision 0
- Update mipi_helpers and rdd_fp_detection to fix coverity warnings.

## Version 6.12
### Revision 0
- Update second pass to record all range/angle second pass thresholds.

## Version 6.13
### Revision 0
-  Range processing building block update to Include commands for the ADC,Restored ADC and lightly comp logging.
-  Coverity warning Fix.

## Version 6.14
### Revision 0
- UT fix

## Version 6.14
### Revision 0
- Updated Calculate_BV_Variance function

## Version 7.00
### Revision 0
-  QDDMA Baseline updates.
-  Common Ra Cmd Config for qddma and non qddma with adc varying bpm support.

## Version 7.01
### Revision 0
-  UT fix qddma and bv variance build dependency

## Version 7.02
### Revision 0
-  ADC logging timing improvement and clear dpmem removed.

## Version 7.03
### Revision 0
- Update QDDMA use case to accomodate zeroing out all samples to disable REST
- Update second pass use of the sensitivity threshold

## Version 7.04
### Revision 0
- Fixing unit tests for doppler

## Version 7.05
### Revision 0
- Vary flag update to work for the CFAR threshold

## Version 7.06
### Revision 0
- Updates to improve TICS
- Addition of the first pass sensitivity threhold logc

## Version 7.07
### Revision 0
- Second Pass updates to remove hvc thold filtering
- Second Pass updates to log below hvc and zdb flags

## Version 7.08
### Revision 0
- Update XCP_Variance_T to hold all decompressed BVs for the selected data.

## Version 7.09
### Revision 0
- SFW Boundary condition bug fix

## Version 7.10
### Revision 0
- CFAR UT FIX

## Version 7.11
### Revision 0
- Updated 3 plane calibration for 3DFFT use case

## Version 7.12
### Revision 0
- BUILD fix for 52D MCAL package integration

## Version 7.13
### Revision 0
- Bring in 48b compression for range cube
