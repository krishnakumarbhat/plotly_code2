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
