/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef TRACKER_ERRORS_T_H
#define TRACKER_ERRORS_T_H

#include "reuse.h"

/**
 * This structure holds tracker error flags.
 * \ingroup tracker_error_handling
 */
typedef struct
{
   unsigned int configuration_is_nullpointer                           : 1; /**< pointer does not refer to a valid object */
   unsigned int tracker_output_is_nullpointer                          : 1; /**< pointer does not refer to a valid object */
   unsigned int radar_parameters_is_nullpointer                        : 1; /**< pointer does not refer to a valid object */
   unsigned int vehicle_data_is_nullpointer                            : 1; /**< pointer does not refer to a valid object */
   unsigned int detections_is_nullpointer                              : 1; /**< pointer does not refer to a valid object */
   unsigned int tracks_is_nullpointer                                  : 1; /**< pointer does not refer to a valid object */
   unsigned int objects_is_nullpointer                                 : 1; /**< pointer does not refer to a valid object */
   unsigned int calibration_is_nullpointer                             : 1; /**< pointer does not refer to a valid object */
   unsigned int radar_mounting_not_available                           : 1; /**< radar mounting information was not set */
   unsigned int tracker_internals_is_nullpointer                       : 1; /**< pointer does not refer to a valid object */
   unsigned int guardrail_is_nullpointer                               : 1; /**< pointer does not refer to a valid object */
   unsigned int incorrect_calibration_curvature_distance_constant      : 1; /**< k_vp_host_curvature_distance_constant_slow or k_vp_host_curvature_distance_constant_fast is zero -> causes division by zero*/
   unsigned int incorrect_object_class_estimation                      : 1; /**< The sum of an objects' class probabilities is not 1. This is an subsequent error happening if objects are looking strange. */
   unsigned int incorrect_calibration_object_classification            : 1; /**< the calibration for the object classification module is not set correctly */
   unsigned int incorrect_std_heading_estimation                       : 1; /**< the counter variable for standard heading estimation is corrupted */
   unsigned int v_un_is_zero                                           : 1; /**< the unambiguous range rate cannot be zero */
   unsigned int incorrect_object_heading_estimation                    : 1; /**< heading innovation covariance matrix is corrupted */
   unsigned int incorrect_calibration_cone_of_silence                  : 1; /**< k_pd_cone_of_silence_min_cos_az < 0, then check in classify motion status doesn't work */
   unsigned int init_wrong_mount_loc                                   : 1; /**< Initialization temp_cals->k_in_sensor_mount_loc != p_configuration->mount_loc */
   unsigned int init_wrong_trigger_mount_loc                           : 1; /**< Initialization temp_cals->c_gp_trigger_mount_loc != p_configuration->mount_loc */
   unsigned int det_iterator_init_out_of_bounds                        : 1; /**< the given sorted detections range for initializing the iterator is corrupted */
   unsigned int radar_parameters_az_pol_faulty                         : 1; /**< The azimuth polarity of a radar can ONLY be -1 or +1 */
   unsigned int vehicle_data_rear_axle_position_not_set                : 1; /**< the rear axle position needs to be set */
   unsigned int incorrect_calibration_in_vehicle_processing            : 1; /**< the calibration for the vehicle processing module is not set correctly */
   unsigned int scan_index_non_consecutive                             : 1; /**< The scan index is not the last one +1 for any sensor*/
   unsigned int radar_parameters_dwell_type_out_of_range               : 1; /**< The dwell type (look type) can only be 0 or 1 */
   unsigned int radar_parameters_alignment_not_set                     : 1; /**< The alignment value cannot equal its initialization value of INFINITY */
   unsigned int radar_parameters_v_un_not_set                          : 1; /**< The v_un value cannot equal its initialization value of -INFINITY */
   unsigned int incorrect_calibration_guardrail                        : 1; /**< guardrail detector is not calibrated correctly */
   unsigned int incorrect_calibration_in_rsds_tracker                  : 1; /**< the calibration for the RSDS Tracker module is not set correctly */
   unsigned int v_un_inconsistant_with_dets_rr                         : 1; /**< The difference of the max and the min range rate of all detections is bigger than the received v_un */
   unsigned int detections_azimuth_confidence_out_of_range             : 1; /**< The DETECTION_FLT_T::azimuth_confidence is bigger or equal to \ref DET_AZ_CONFIDENCE_NUMBER_OF_LEVELS */
   unsigned int max_range_current_look_inconsistant_with_dets_r        : 1; /**< The maximum range of all detections is bigger than the received max_range_current_look */
   unsigned int tracker_cal_type_not_set                               : 1; /**< c_tracker_cal_type is still set to TRACKER_CAL_TYPE_INVALID*/
   unsigned int trigonometric_function_called_with_non_triangle        : 1; /**< A function in ac_trigonometry has been called with data not suiting a triangle */
   unsigned int calculated_time_stamp_delta_out_of_range               : 1; /**< the calculated delta in time stamps is not within the expected region */
   unsigned int runtime_parameter_out_of_range                         : 1; /**< a runtime parameter was set with a value which is outside of the allowed range */
   unsigned int runtime_parameter_mandatory_was_not_set                : 1; /**< a mandatory runtime parameter was not set */
   unsigned int runtime_parameter_prohibited_was_set                   : 1; /**< a runtime parameter which is prohibited to set, was set */
   unsigned int incorrect_calibration_in_process_Detections            : 1; /**< the calibration for the process detections module is not set correctly */
   unsigned int tracker_can_not_run                                    : 1; /**< Error flags are set that prevent the tracker from running*/
   unsigned int matching_internal_detection_does_not_exist             : 1; /**< some module tried to get tracker internal detection data for a detection from the history buffer*/
   unsigned int tracker_configuration_not_set                          : 1; /* The structure CONFIGURATION_T which is passed to initialization() has faulty entries */
   unsigned int rolling_count_transform_detections_failed              : 1; /**< rolling_count_transformDetections in RADAR_PARAMS_FLT_T has not been increased. This indicates that transformDetections() has
                                                                         * not been run. */
   unsigned int rolling_count_process_detections_failed                : 1; /**< rolling_count_processDetections in RADAR_PARAMS_FLT_T has not been increased. This indicates that processDetections() has not
                                                                         * been run. */
   unsigned int rolling_count_tracklet_tracker_failed                  : 1; /**< rolling_count_trackletTracker in RADAR_PARAMS_FLT_T has not been increased. This indicates that trackletTracker() has not been
                                                                         * run. */
   unsigned int rolling_count_vehicle_processings_failed               : 1; /**< rolling_count_vehicle_data in VEHICLE_DATA_FLT_T has not been increased. This indicates that vehicleProcessing() has not been
                                                                         * run. */
   unsigned int vehicle_data_host_vehicle_length_not_set               : 1; /**< the host vehicle length needs to be set */
   unsigned int vehicle_data_host_vehicle_width_not_set                : 1; /**< the host vehicle width needs to be set */
   unsigned int vehicle_data_lane_width_external_not_set               : 1; /**< lane_width_external in vehicle data needs to be set */
   unsigned int vehicle_data_max_value_mastertime_not_set              : 1; /**< max_value_mastertime in vehicle data needs to be set */
   unsigned int detection_with_range_below_min_present                 : 1; /**< Found a detections with a range below a minimum */
   unsigned int inconsistent_rear_axle_position_vs_host_vehicle_length : 1; /**< absolute value of rear axle position exceeds host vehicle length */
   unsigned int v_un_inconsistent_with_min_max_rr_values               : 1; /**< the v_un value needs to be the difference of the maximum and minimum unambiguous range rate of the current look*/
   unsigned int inconsistent_min_max_range_rate_current_look           : 1; /**< the maximum and minimum unambiguous range rate of the current look are inconsistent, min>max */
   unsigned int incorrect_calibration_in_micro_doppler                 : 1; /**< the calibration for the \ref micro_doppler module is not set correctly */
} TRACKER_ERRORS_T;

#endif

