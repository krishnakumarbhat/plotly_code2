# Changelog for required modifications to the SIL CDC stream data
-------------------------
## Overview
This file is to maintatin a a track of log buffers that need to be mapped onto the SIL input from the log when running in CDC to detection configuration.
The buffers are copied using memcpy in cdc_sil_interface.cpp
The Rdd_Log_Input_T structure contains the values from the log. The buffers needs to copied to SIL_RDD_Data_T structure for further processing SIL in CDC mode.


### R9.1 These are the log variables need to be copied from Rdd_Log_Input_T structure to SIL_RDD_Data_T structure
   1. uint32_t nf_est_log[MAX_RANGE_BINS];
   2. uint32_t cfar_thold_log[MAX_RANGE_BINS];
   3. uint32_t mprb_log[MAX_RANGE_BINS];
   4. uint32_t cfar_nf_est_ci_log[MAX_RANGE_BINS];
   5. uint32_t cfar_thold_ci_log[MAX_RANGE_BINS];
   6. uint32_t rdd1_max_per_range_bin_ci_log[MAX_RANGE_BINS];
   7. uint8_t min_chirp_scaling_log;
   8. uint8_t rest_bin_proc_flag_log[MAX_RANGE_BINS];

### The following variables from the log need not be copied since they are recalculated in CDC mode.
   1. s10p21_T rdd1_rbinest_log[MAX_DETS_FIRST_PASS];
   2. s10p21_T rdd1_dbinest_log[MAX_DETS_FIRST_PASS];
   3. uint16_t rdd1_rindx_log[MAX_DETS_FIRST_PASS];
   4. uint16_t rdd1_dindx_log[MAX_DETS_FIRST_PASS];
   5. uint32_t rdd1_rdop_amp_log[MAX_DETS_FIRST_PASS];
   6. bool ci_detections_flag_log[MAX_DETS_FIRST_PASS];
   7. Mixing_T mixing_strength_log[MAX_DETS_FIRST_PASS];
   8. uint32_t rdd1_bv_log[MAX_DETS_FIRST_PASS][BV_COMP_WORD_COUNT];
   9. uint16_t rdd1_detections_log;
