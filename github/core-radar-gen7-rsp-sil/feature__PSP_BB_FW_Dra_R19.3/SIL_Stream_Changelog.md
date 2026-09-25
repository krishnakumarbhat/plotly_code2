# Changelog for required modifications to the RDD stream data
-------------------------
## Overview
When directly utilizing the SIL APIs at the second pass or anglefinding inputs, there is the possibility that the RDD stream has been modified to support updates to the RDD logic. The changes documented in this document will allow tracking those changes across versions and list out the required changes to support use of the SIL.

## Uses
To utilize these updates, the version of the RDD stream in the application code where the data logs are collected *must* be used to successfully parse the logs into the updated SIL_RDD_Data_T log structure. The RDD_Data_T structure is not backward compatible, as it is auto-generated and backward compatibility is not a feature of the generator code. The integrator of the API will need to use the data structure of the appropriate version that aligns with the application (as a local typedef, for instance) to parse the RDD stream data in the log, and then it can be copied into the SIL_RDD_Data_T typedef structure required by the the SIL API.

## R7.0 Update
In the R7.0 SIL update, the RDD stream was updated from v24 to v25. The required changes in the SPBB are shown here:

![image info](./pictures/rdd_stream_v25/v25_rdd_data_added_variables.png)

### Change requirement
Each of the new members of the SIL_RDD_Data_T structure are outputs of second pass processing, therefore, the variables can be initialized with zeros, as the second pass API call will overwrite the data. These are the members that are added to the SIL_RDD_Data_T structure:

   1. uint8_t rdd2_below_2nd_pass_thold[MAX_DETS_FIRST_PASS];
   1. uint8_t rdd2_below_zdb_thold[MAX_DETS_FIRST_PASS];
   1. uint8_t rdd2_below_hvc_thold[MAX_DETS_FIRST_PASS];

## R8.0 Update
In the R8.0 release of AWR, the RDD stream is updated to version 27. The R8.0 release is minimally impacted by this change, because the mapping of the signals is abstracted through the SIL_RDD_Data_T type definition

![image info](./pictures/rdd_stream_v27/v27_rdd_data_added_variables.png)

### Change requirements
These changes can simply be ignored. There is no need to populate these XCP variables, as they will be unused in the SIL.

   1. uint32_t rdd1_xcp_compressed_bv[XCP_NO_OF_TARGET_SELECTION][BV_COMP_WORD_COUNT];
   1. Radar_Data32_T rdd1_xcp_decompressed_bv[XCP_NO_OF_TARGET_SELECTION][NUM_RX_CHANNELS * NUM_TX_CHANNELS];

This variable is added, but unused in the R8.0 release.

   1. u16p16_T rdd1_rbin_res_in;

These members will be updated in a call to first pass, but are only logged to the RDD data stream. These will not be updated in RDD only processing use cases for the SIL. These can be ignored in the mapping from RDD logging to SIL inputs.

   1. float32_t rdd1_max_detectable_range;
   1. uint16_t rdd1_max_rbin_processed;
   1. bool rdd1_saturation_occured_flag;
      a. This is utilized in data quality in the R8.0 release, but not in the RSP SIL.

## R8.1 Update
In the R8.1 release of AWR, the RDD stream is updated to version 30.

![image info](./pictures/rdd_stream_v30/v27_to_v30.png)

### v27 to v30 Change Notes and requirements
These removals can be ignored. These are unused in the SIL.

   1. uint32_t rdd1_xcp_compressed_bv[XCP_NO_OF_TARGET_SELECTION][BV_COMP_WORD_COUNT];
   1. Radar_Data32_T rdd1_xcp_decompressed_bv[XCP_NO_OF_TARGET_SELECTION][NUM_RX_CHANNELS * NUM_TX_CHANNELS];
   1. uint32_t rdd1_xcp_rdop_amp;
   1. s10p21_T rdd1_xcp_rbin_est;
   1. s10p21_T rdd1_xcp_dbin_est;
   1. uint32_t rdd1_xcp_rdop_avg_dbin[MAX_RANGE_BINS];
   1. uint32_t rdd1_xcp_rdop_avg_rbin[MAX_DOPPLER_FFT_SIZE];
   1. float32_t rdd2_xcp_snr;
   1. s10p21_T rdd2_xcp_range_rate;
   1. s10p21_T rdd2_xcp_range;
   1. bool rdd1_xcp_ci_flag;

#### These variables are added to the RDD stream information.

##### Input data (should be copied to SIL_RDD_Data_T inputs)

   1. float32_t cfar_corr_coeff[MAX_RANGE_BINS]

###### Added to SIL_RDD_Data_T, but already part of v27 stream.
In the SIL_RDD_Data_T, this variable is added. This is already available in stream 27, but is now added and can be mapped to the SIL_RDD_Data_T.
   1. uint8_t rest_bin_proc_flag[MAX_RANGE_BINS]

##### Output data (will be overwritten into SIL_RDD_Data_T output)
   1. uint32_t rdd2_thold_idm_artifact_mask[MAX_RANGE_BINS];
   1. uint8_t rdd2_below_iam_thold[MAX_DETS_FIRST_PASS];

##### Placeholder data, can be ignored.
   1. uint8_t K_Unused8_1;

#### The data needed to calculate the IDM information in the second pass comes from the SIL_Rfft_Data_T structure (rfft_data) of the RDD stream. This signal is added into the SIL_RDD_Data_T and needs to be copied. If this is not populated correctly, the related IDM artifact masking outputs will be impacted.
This additional signal is needed as an input for the RDD second pass processing.
   1. uint16_t interfered_samples_per_chirp[K_MAX];

##### Updated af_sil_interface_pvt.cpp file for mapping the detection stream variables
   1. f_ci_det
   2. std_ran
   3. std_vel
   4. std_theta
   5. std_phi

## R9.0 Update

##### Updated files for PSP (interference detection to static alignment BB)
   1. ID, RC, DA and SA SIL API is moved to a single wrapper Psp_Id_To_Sa_wrapper
   2. Vse stream(sil_psp_in_stream.h) is added for vehicle parameters input
   3. Psp input stream(sil_psp_in_stream.h) is added to handle the parameters which is not part of any SIL stream
   4. input interface correction is done for ID and RC. here ID denotes Interference_detection and RC denotes Radar capability.
   5. workspace is updated w.r.t latest ID and RC jfrog link

## R9.1 Update

##### Updated files for PSP (interference detection to static alignment BB)

   1. ID, RC and SA SIL stream is updated to access same stream file in both embedded(awr) and RSP SIL repo.
   2. new stream file stream_header.h is added inside sil_wrapper_interface.
   3. sil_wrapper.h is updated : da_sil_in and sa_sil_in is been removed and respective input prameters are added in
      sil_psp_in_stream.h.
   4. rcbb_include.h is addded.
   5. sil_psp_in_stream.h is updated to keep all the required PSP input at one place.
   6. PSP_Input_Sil_T structure parameters required to be filled or updated as per below to run the PSP wrapper/algo.
        6.1. SIL_MMIC_RFHealth_Status_L_T to updated from mmic stream data
        6.2. SIL_Static_Alignment_Data_T to be updated as per below value mapping
            plate_distance      = k_Config_AlignmentStatic_TargetDistance_SMC
            plate_angle_deg_scs = K_Static_Align_Plate_Angle_Deg_scs
            yaw                 = k_Config_AlignmentStatic_TargetAzimuth_SMC
            pitch               = k_Config_AlignmentStatic_TargetElevation_SMC
            polarity            = K_Static_Align_Polarity
            plate_lat_position  = K_Static_Align_Plate_Lat_Position
            horz_angle_min      = k_Config_AlignmentStatic_AzimuthLower_Threshold_SMC
            horz_angle_max      = k_Config_AlignmentStatic_AzimuthUpper_Threshold_SMC
            vert_angle_min      = k_Config_AlignmentStatic_ElivationLower_Threshold_SMC
            vert_angle_max      = k_Config_AlignmentStatic_ElevationUpper_Threshold_SMC
            mount_loc           = k_sensor_mount_location_SMC
            f_data_valid        = 1U
            align_cmd           = sa_align_cmd_sil_in(from static alignment stream)
        6.3. SIL_Dynamic_Alignment_Data_T to be updated as per below value mapping. here sensor_posn = Get_Radar_Position() - 1.
        	    if (sensor_posn < MAX_RADAR_SENSOR_POS)
             {
	         mount_loc                          = k_sensor_mount_location_SMC
            iso_sensor_x_posn                  = k_sensor_mount_pos_x_list_SMC[sensor_posn]
            iso_sensor_y_posn                  = k_sensor_mount_pos_y_list_SMC[sensor_posn]
            iso_sensor_z_posn                  = k_sensor_mount_pos_z_list_SMC[sensor_posn]
            iso_sensor_yaw                     = k_sensor_mount_ornt_yaw_list_SMC[sensor_posn]
            iso_sensor_pitch                   = k_sensor_mount_ornt_pitch_list_SMC[sensor_posn]
            iso_sensor_roll                    = k_sensor_mount_ornt_roll_list_SMC[sensor_posn]
            distance_rear_axle_to_front_bumper = k_distance_front_bumper_to_rear_axle_SMC
            max_abs_misalignment_az            = k_max_abs_misalignment_az_SMC
            max_abs_misalignment_el_sky        = k_max_abs_misalignment_el_SMC
            max_abs_misalignment_el_ground     = k_max_abs_misalignment_el_SMC

            short_track_target_height     = 0.8F
            short_track_target_range_rate = -11.0F
            az_scf_kf_covariance[0]       = 0.0F
            az_scf_kf_covariance[1]       = 0.0F
            az_scf_kf_covariance[2]       = 0.0F
            az_scf_kf_covariance[3]       = 0.0F
            el_kf_covariance              = 0.0F
            short_track_exit_condition    = DRA_SHORT_TRACK_EXIT_AUTO;
            initial_vacs_boresight_az_estimated = iso_sensor_yaw;
            initial_vacs_boresight_el_estimated = iso_sensor_pitch;

	         initial_speed_compensation_factor = 1
            initial_num_updates_az            = n_updates_azimuth;
            initial_num_updates_el            = n_updates_elevation;

            f_data_valid = 1U;
            align_cmd    = da_align_cmd_sil_in(from static alignment stream)

	        }
        6.4. SIL_GLOB_TS_STRUCT_TYPE to be updated from Radar capability stream(RC_Ts_Output_Stream_T)
        6.5. avg_mmic_onchip_temp to be updated from Radar capability stream(avg_mmic_onchip_temp)
        6.6. scan_index and  range_coverage to be updated from RDD stream.

## R10.0 Update

#### Updated files for PSP (Interference Detection to Static Alignment)

   1. **sil_dra_internal_stream.h** is added to Repository, which is new input parameter for SIL API. It will have Internal data from Structure of Dynamic Alignment and used to initialize DA in DA Wrapper of SIL.
   2. **Sil_Input_T structure is updated to include DRA_Internals_Sil_In_T**.
   3. A Binary file with data of stream 46 from Logs of R10 release is added in path - gen7_sil_wrapper\data_bin\srr7p\cdc_data with file name dra_internals.bin
   4. DRA_Internals_Sil_In_T is added as input parameter to API - Cdc_To_Detection_Configuration()
   5. DRA_Internals_Sil_In_T is added as input parameter to API - Psp_Id_To_Sa_wrapper()
   6. DRA_Internals_Sil_In_T is added as input parameter to API - DA_Configure_And_Execute()
   7. DRA_Internals_Sil_In_T parameter from Cdc_To_Detection_Configuration() is passed as input parameter to API - Psp_Id_To_Sa_wrapper()
   8. DRA_Internals_Sil_In_T parameter from Psp_Id_To_Sa_wrapper() is passed as input parameter to API - DA_Configure_And_Execute()
   9. Radar Capability Algo is updated to V 3.16.0
   10. Dynamic Alignment Algo Version Integrated is V 18.5.0
   11. Static Alignment Algo Version Integrated is V 8.5.0

#### SPBB updates: Added following variables to the SIL_RDD_Data_T. These are variables related to rainman algo which are part of RDD stream v32

   uint16_t rdd2_rain_level;
   uint16_t rdd2_rain_count;
   uint8_t rdd2_below_rain_thold[MAX_DETS_FIRST_PASS];

## R10.1 Update
#### Updated radar capability files

   1. Added functions in **sil_rc_wrapper.cpp**, i.e.,
      -> rc_sil_input_init()
      -> rc_sil_mapping_det()
      -> rc_sil_mapping_rdd()
      -> rc_sil_mapping_id()
      -> rc_sil_mapping_dyn()
      -> rc_sil_mapping_veh()
      -> rc_sil_wrapper_map_input_data()
      -> rc_sil_mapping_rf_health()
      -> rc_sil_mapping_rc_input()
   2. Added **Dynamic_alignment_stream.h**, **idbb_include.h** & **radar_capability_stream.h** dependency for input arguments for *RC_Configure_And_Execute()*
   3. Added *RFHealth_Status_T*, *Radar_Data_Status_Payload_T* & *Mmic_Radar_Data_Status_T* in **sil_psp_in_stream.h** as inputs.
   4. Removed
      -> *rc_sil_out.sil_rc_output.se.filt_max_range_m*
      -> *rc_sil_out.sil_rc_output.se.filt_num_dets*
      -> *rc_sil_out.sil_rc_output.se.num_dets*
      -> *rc_sil_out.sil_rc_output.se.f_few_st_dets*
      -> *rc_sil_out.sil_rc_output.se.f_high_MNR_close*
      -> *rc_sil_out.sil_rc_output.se.f_bad_data* from **main.cpp**
   5. Added *RC_Input_T* & **rc_types.h** in **rcbb_include.h**.
   6. Removed *RC_RDD_Data_T*, *RC_AF_Det_List_Property_T* & *RC_Look_Data_T* in **rcbb_include.h**
   7. Removed *AF_Detection_Stream_Sil_T *, *SIL_MMIC_RFHealth_Status_L_T * & *SIL_GLOB_TS_STRUCT_TYPE * in **rcbb_include.h**
   8. Removed **radar_math.h** from **idbb_include.h**.
   9. Radar Capability Algo is updated to V 3.19.6
   10. Dynamic Alignment Algo Version is V 18.5.0
   11. Static Alignment Algo Version is V 8.5.0

## R11.0 Update
#### Updated dynamic alignment files

   1. Added operation_mode_status, toi_data_qualifier and toi_ds_data_qualifier in **PSP_Input_Sil_T**, each one should be updated from header stream(header_stream->mss_stream_data.OperationMode_System), toi stream(toi_stream->toi_op_data.data_qualifier) and toi downselection stream(toi_ds_stream->toi_op_data.data_qualifier, This should be mapped only if the stream is available) respectively.
