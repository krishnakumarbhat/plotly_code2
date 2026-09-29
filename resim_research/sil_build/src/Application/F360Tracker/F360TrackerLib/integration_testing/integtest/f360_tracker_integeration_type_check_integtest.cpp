/** \file
   This file contains test cases for interface data types
*/
#include <float.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cfloat>
#include "f360_tracker.h"
#include "f360_update_relative_timestamps.h"
#include "f360_time_update_tracks.h"
#include "f360_pre_association_track_management.h"
#include "f360_detection_to_track_association.h"
#include "f360_clustering.h"
#include "f360_cluster_grouping.h"
#include "f360_track_grouping.h"
#include "f360_internal_preprocessing.h"
#include "f360_inputs_preprocessing.h"
#include "f360_initialize_tracks.h"
#include "f360_measurement_update_tracks.h"
#include "f360_post_update_track_adjustments.h"
#include "f360_track_classification.h"
#include "f360_track_validity.h"
#include "f360_track_downselection.h"
#include "f360_e2e_protection.h"
#include "f360_static_environment_class.h"
#include "f360_trailer_detector_core.h"
#include "f360_sanity_check.h"
#include "f360_sensor_postprocessing.h"
#include "f360_get_wall_time.h"
#include "f360_occlusion.h"
#include "init_tracker_info_T.h"
#include "init_object_tracks_T.h"
#include "init_clusters_T.h"
#include "init_core_T.h"
#include "init_det_hist_T.h"

#include "init_det_props_T.h"
#include "init_globals_T.h"
#include "init_host_props_T.h"
#include "init_host_T.h"
#include "init_raw_detect_T.h"
#include "init_sensor_calib_T.h"
#include "init_sensor_props_T.h"
#include "init_sensors_T.h"
#include "init_timing_info_T.h"
#include "init_static_env_class_T.h"
#include "init_trailer_detector_core.h"
#include "init_occupancy_grid_T.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include "f360_calibrations.h"
#include "f360_ut_type_check.h"

/*F360_Input_Init header file contains input_init() macro  which call input test vector initialization */
#include "F360_Input_Init.h"
using namespace f360_variant_A;

/** \defgroup  f360_tracker_integration_types_test
 *  @{
 */

/** \brief
*  this test suits aim to test the data type of interface signals
**/
TEST_GROUP(f360_tracker_integration_types_test)
{   
   #include "F360_Interface_Declaration.h"
   #include "F360_Tracker_Module_Call.h"

   /** \setup
   * init the test input vector
   **/
   TEST_SETUP()
   {
     //input_init
      input_init();
   }

   /** \teardown
   * Nothing to teardown in this test group
   **/
};
    

/**
**********************************************************************************************************
*                                            F360_Detection_Props_T                                      *
**********************************************************************************************************
*/

/** \purpose  
 * Purpose of this test is to verify Detections_props  interfaces data types according to requirements.
 * \req
 * CPR-4526
 */

TEST(f360_tracker_integration_types_test, PreProcessor_check_verification_of_detection_props_data_types)
{
   /** \precond
    **/
    

   /** \action
    * Call function
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
    **/
   CHECK_TEXT(type_name<decltype(detections[0].position_cov_nees)>() == "float [2][2]", "Unexpected data type for detections[0].position_cov_nees");
   CHECK_TEXT(type_name<decltype(detections[0].vcs_position)>() == "f360_variant_A::Point", "Unexpected data type for detections[0].vcs_position");
   CHECK_TEXT(type_name<decltype(detections[0].cluster_id)>() == "short", "Unexpected data type for detections[0].cluster_id");
   CHECK_TEXT(type_name<decltype(detections[0].object_track_id)>() == "int", "Unexpected data type for detections[0].object_track_id");
   CHECK_TEXT(type_name<decltype(detections[0].wheel_spin_type)>() == "f360_variant_A::F360_Detection_Wheelspin_Type_Tag", "Unexpected data type for detections[0].wheel_spin_type");
   CHECK_TEXT(type_name<decltype(detections[0].range_rate_dealiased)>() == "float", "Unexpected data type for detections[0].range_rate_dealiased");
   CHECK_TEXT(type_name<decltype(detections[0].range_rate_compensated)>() == "float", "Unexpected data type for detections[0].range_rate_compensated");
   CHECK_TEXT(type_name<decltype(detections[0].range_rate_predicted)>() == "float", "Unexpected data type for detections[0].range_rate_predicted");
   CHECK_TEXT(type_name<decltype(detections[0].probability_of_detection)>() == "float", "Unexpected data type for detections[0].probability_of_detection");
   CHECK_TEXT(type_name<decltype(detections[0].f_dealiased)>() == "bool", "Unexpected data type for detections[0].f_dealiased");
   CHECK_TEXT(type_name<decltype(detections[0].f_double_bounce)>() == "bool", "Unexpected data type for detections[0].f_double_bounce");
   CHECK_TEXT(type_name<decltype(detections[0].f_FOV_edge)>() == "bool", "Unexpected data type for detections[0].f_FOV_edge");
   CHECK_TEXT(type_name<decltype(detections[0].f_rr_inlier)>() == "bool", "Unexpected data type for detections[0].f_rr_inlier");
   CHECK_TEXT(type_name<decltype(detections[0].f_used_in_rr_msmt_update)>() == "bool", "Unexpected data type for detections[0].f_used_in_rr_msmt_update");
   CHECK_TEXT(type_name<decltype(detections[0].f_close_target)>() == "bool", "Unexpected data type for detections[0].f_close_target");
   CHECK_TEXT(type_name<decltype(detections[0].f_inside_gate)>() == "bool", "Unexpected data type for detections[0].f_inside_gate");
   CHECK_TEXT(type_name<decltype(detections[0].f_ok_to_use)>() == "bool", "Unexpected data type for detections[0].f_ok_to_use");
   CHECK_TEXT(type_name<decltype(detections[0].f_det_pair)>() == "bool", "Unexpected data type for detections[0].f_det_pair");
   CHECK_TEXT(type_name<decltype(detections[0].dist_to_closest_assoc_det_sq)>() == "float", "Unexpected data type for detections[0].dist_to_closest_assoc_det_sq");
   CHECK_TEXT(type_name<decltype(detections[0].any_other_assoc_det_close)>() == "f360_variant_A::F360_Any_Other_Assoc_Det_Close", "Unexpected data type for detections[0].any_other_assoc_det_close");
   CHECK_TEXT(type_name<decltype(detections[0].f_potential_angle_jump)>() == "bool", "Unexpected data type for detections[0].f_potential_angle_jump");
   CHECK_TEXT(type_name<decltype(detections[0].f_object_based_angle_jump)>() == "bool", "Unexpected data type for detections[0].f_object_based_angle_jump");
   CHECK_TEXT(type_name<decltype(detections[0].f_water_spray)>() == "bool", "Unexpected data type for detections[0].f_water_spray");
   CHECK_TEXT(type_name<decltype(detections[0].f_valid_for_liberal_tracking)>() == "bool", "Unexpected data type for detections[0].f_valid_for_liberal_tracking");
   CHECK_TEXT(type_name<decltype(detections[0].f_stationary_bounce)>() == "bool", "Unexpected data type for detections[0].f_stationary_bounce");
   CHECK_TEXT(type_name<decltype(detections[0].f_azimuth_rdot_outlier)>() == "bool", "Unexpected data type for detections[0].f_azimuth_rdot_outlier");
   CHECK_TEXT(type_name<decltype(detections[0].behind_sep_id)>() == "unsigned char", "Unexpected data type for detections[0].behind_sep_id");
   CHECK_TEXT(type_name<decltype(detections[0].on_sep_id)>() == "unsigned char", "Unexpected data type for detections[0].on_sep_id");
}
/**
**********************************************************************************************************
*                                            F360_Host_T                                           *
**********************************************************************************************************
*/

/**
 * \purpose
 * Purpose of this test is to verify host interfaces data types according to requirements.
 * \req
 * CPR-4266
 */
TEST(f360_tracker_integration_types_test, PreProcessor_check_verification_of_host_data_types)
{
   /** \precond
    **/
    

   /** \action
    * Call function
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
    
    **/
   CHECK_TEXT(type_name<decltype(host.vehicle_index)>() == "unsigned int", "Unexpected data type for host.vehicle_index");
   CHECK_TEXT(type_name<decltype(host.speed)>() == "float", "Unexpected data type for host.speed");
   CHECK_TEXT(type_name<decltype(host.vcs_speed)>() == "float", "Unexpected data type for host.vcs_speed");
   CHECK_TEXT(type_name<decltype(host.acceleration)>() == "float", "Unexpected data type for host.acceleration");
   CHECK_TEXT(type_name<decltype(host.vcs_lat_acceleration)>() == "float", "Unexpected data type for host.vcs_lat_acceleration"); // Note: in polarion there is vcs_acceleration
   CHECK_TEXT(type_name<decltype(host.vcs_long_acceleration)>() == "float", "Unexpected data type for host.vcs_long_acceleration");// Note: in polarion there is vcs_acceleration
   CHECK_TEXT(type_name<decltype(host.yaw_rate_rad)>() == "float", "Unexpected data type for host.yaw_rate_rad");
   CHECK_TEXT(type_name<decltype(host.vcs_sideslip)>() == "float", "Unexpected data type for host.vcs_sideslip");
   CHECK_TEXT(type_name<decltype(host.curvature_rear)>() == "float", "Unexpected data type for host.curvature_rear");
   CHECK_TEXT(type_name<decltype(host.dist_rear_axle_to_vcs_m)>() == "float", "Unexpected data type for host.dist_rear_axle_to_vcs_m");
   CHECK_TEXT(type_name<decltype(host.rear_cornering_compliance)>() == "float", "Unexpected data type for host.rear_cornering_compliance");
   CHECK_TEXT(type_name<decltype(host.speed_correction_factor)>() == "float", "Unexpected data type for host.speed_correction_factor");
}

/**
**********************************************************************************************************
*                                            Static_Env_T                                           *
**********************************************************************************************************
*/

/**
 * \purpose
 * Purpose of this test is to verify Static_Env_T interfaces data types according to requirements.
 * \req
 * CPR-4538
 */
TEST(f360_tracker_integration_types_test, PreProcessor_check_verification_of_static_env_class_data_types)
{
   /** \precond
    **/
   const Static_Env_Poly_T (&static_env_polys)[F360_NUM_OF_STATIC_ENV_POLYS] = static_env_class.Get_Polynomials();
   
   /** \action
    * Call function
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
    **/
   CHECK_TEXT(type_name<decltype( static_env_polys->status)>() == "f360_variant_A::Static_Env_Poly_Status_T", "Unexpected data type for static_env_polys->status");
   CHECK_TEXT(type_name<decltype( static_env_polys->poly_type)>() == "f360_variant_A::Static_Env_Poly_Type_T", "Unexpected data type for static_env_polys->type");
   CHECK_TEXT(type_name<decltype( static_env_polys->age)>() == "unsigned char", "Unexpected data type for static_env_polys->age");
   CHECK_TEXT(type_name<decltype( static_env_polys->confidence)>() == "float", "Unexpected data type for static_env_polys->confidence");
   CHECK_TEXT(type_name<decltype( static_env_polys->lower_limit)>() == "float", "Unexpected data type for static_env_polys->lower_limit");
   CHECK_TEXT(type_name<decltype( static_env_polys->upper_limit)>() == "float", "Unexpected data type for static_env_polys->upper_limit");
   CHECK_TEXT(type_name<decltype( static_env_polys->p0)>() == "float", "Unexpected data type for static_env_polys->p0");
   CHECK_TEXT(type_name<decltype( static_env_polys->p1)>() == "float", "Unexpected data type for static_env_polys->p1");
   CHECK_TEXT(type_name<decltype( static_env_polys->p2)>() == "float", "Unexpected data type for static_env_polys->p2");
}

/**
**********************************************************************************************************
*                                            Trailer_Detector_Core                                           *
**********************************************************************************************************
*/

/**
 * \purpose
 * Purpose of this test is to verify Trailer_Detector_Core interfaces data types according to requirements.
 * \req
 * CPR-4538
 */
TEST(f360_tracker_integration_types_test, PreProcessor_check_verification_of_trailer_detector_data_types)
{
   /** \precond
    **/
   const Trailer_Detector_Flt_Fus_Output trailer_detector_output = trailer_detector_core.Get_Trailer_Detector_Output();
   
   /** \action
    * Call function
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
    **/
   CHECK_TEXT(type_name<decltype(trailer_detector_output.trailer_length)>() == "float", "Unexpected data type for trailer_detector_output.trailer_length");
   CHECK_TEXT(type_name<decltype(trailer_detector_output.trailer_width)>() == "float", "Unexpected data type for trailer_detector_output.trailer_width");
   CHECK_TEXT(type_name<decltype(trailer_detector_output.trailer_angle)>() == "float", "Unexpected data type for trailer_detector_output.trailer_angle");
   CHECK_TEXT(type_name<decltype(trailer_detector_output.trailer_angle_rate)>() == "float", "Unexpected data type for trailer_detector_output.trailer_angle_rate");
   CHECK_TEXT(type_name<decltype(trailer_detector_output.radar_detection_timer)>() == "unsigned int", "Unexpected data type for trailer_detector_output.radar_detection_timer");
   CHECK_TEXT(type_name<decltype(trailer_detector_output.stationary_timer)>() == "unsigned int", "Unexpected data type for trailer_detector_output.stationary_timer");
   CHECK_TEXT(type_name<decltype(trailer_detector_output.trailer_detection_status)>() == "f360_variant_A::Trailer_Detector_Status", "Unexpected data type for trailer_detector_output.trailer_detection_status");
   CHECK_TEXT(type_name<decltype(trailer_detector_output.trailer_presence)>() == "f360_variant_A::Trailer_Presence_State", "Unexpected data type for trailer_detector_output.trailer_presence");
   CHECK_TEXT(type_name<decltype(trailer_detector_output.trailer_presence_conf)>() == "f360_variant_A::Trailer_Detector_Conf", "Unexpected data type for trailer_detector_output.trailer_presence_conf");
   CHECK_TEXT(type_name<decltype(trailer_detector_output.trailer_width_conf)>() == "f360_variant_A::Trailer_Detector_Conf", "Unexpected data type for trailer_detector_output.trailer_width_conf");
}

/**
 *
**********************************************************************************************************
*                                            F360_Cluster_T                                           *
**********************************************************************************************************
*/

/**
 *\purpose
 * Purpose of this test is to verify clusters interfaces data types according to requirements.
 * \req
 * CPR-4532
 */
TEST(f360_tracker_integration_types_test, PreProcessor_check_verification_of_clusters_data_types)
{
   /** \precond
    **/
    

   /** \action
    * Call function
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
    
    **/
   CHECK_TEXT(type_name<decltype(clusters[0].vcs_position)>() == "f360_variant_A::Point", "Unexpected data type for clusters[0].vcs_position");
   CHECK_TEXT(type_name<decltype(clusters[0].vcs_position.x)>() == "float", "Unexpected data type for clusters[0]..longitudinal");
   CHECK_TEXT(type_name<decltype(clusters[0].vcs_position.y)>() == "float", "Unexpected data type for clusters[0].vcs_position.y");
   CHECK_TEXT(type_name<decltype(clusters[0].rep_vcs_az)>() == "float", "Unexpected data type for clusters[0].rep_vcs_az");
   CHECK_TEXT(type_name<decltype(clusters[0].cos_vcs_az)>() == "float", "Unexpected data type for clusters[0].cos_vcs_az");
   CHECK_TEXT(type_name<decltype(clusters[0].sin_vcs_az)>() == "float", "Unexpected data type for clusters[0].sin_vcs_az");
   CHECK_TEXT(type_name<decltype(clusters[0].rep_rdotcomp)>() == "float", "Unexpected data type for clusters[0].rep_rdotcomp");
   CHECK_TEXT(type_name<decltype(clusters[0].exist_prob)>() == "float", "Unexpected data type for clusters[0].exist_prob");
   CHECK_TEXT(type_name<decltype(clusters[0].id)>() == "short", "Unexpected data type for clusters[0].id");
   CHECK_TEXT(type_name<decltype(clusters[0].ndets)>() == "short", "Unexpected data type for clusters[0].ndets");
   CHECK_TEXT(type_name<decltype(clusters[0].detids)>() == "short [80]", "Unexpected data type for clusters[0].detids");
   CHECK_TEXT(type_name<decltype(clusters[0].num_old_dets)>() == "short", "Unexpected data type for clusters[0].num_old_dets");
   CHECK_TEXT(type_name<decltype(clusters[0].old_det_idx)>() == "short [80]", "Unexpected data type for clusters[0].old_det_idx");
   CHECK_TEXT(type_name<decltype(clusters[0].num_types_of_dets)>() == "short [2]", "Unexpected data type for clusters[0].num_types_of_dets");
   CHECK_TEXT(type_name<decltype(clusters[0].num_of_cluster_merges)>() == "unsigned short", "Unexpected data type for clusters[0].num_of_cluster_merges");
   CHECK_TEXT(type_name<decltype(clusters[0].f_dealiased)>() == "bool", "Unexpected data type for clusters[0].f_dealiased");
   CHECK_TEXT(type_name<decltype(clusters[0].f_to_be_killed)>() == "bool", "Unexpected data type for clusters[0].f_to_be_killed");
   CHECK_TEXT(type_name<decltype(clusters[0].motion_status)>() == "f360_variant_A::F360_Cluster_Motion_Status_Tag", "Unexpected data type for clusters[0].motion_status");
   CHECK_TEXT(type_name<decltype(clusters[0].time_since_created)>() == "float", "Unexpected data type for clusters[0].time_since_created");
   CHECK_TEXT(type_name<decltype(clusters[0].time_since_cluster_updated)>() == "float", "Unexpected data type for clusters[0].time_since_cluster_updated");
   CHECK_TEXT(type_name<decltype(clusters[0].time_since_measurement)>() == "float", "Unexpected data type for clusters[0].time_since_measurement");
   CHECK_TEXT(type_name<decltype(clusters[0].priority)>() == "float", "Unexpected data type for clusters[0].priority");
   CHECK_TEXT(type_name<decltype(clusters[0].low_rcs_dets_cnt)>() == "unsigned char", "Unexpected data type for clusters[0].low_rcs_dets_cnt");
}

 /**
**********************************************************************************************************
*                                            F360_Core_Info_T                                          *
**********************************************************************************************************
*/

/** \purpose
* Purpose of this test is to verify F360_Core_Info_T interfaces data types according to requirements.
* \req
* CPR-4434
*/
TEST(f360_tracker_integration_types_test, PreProcessor_check_verification_of_core_info_data_types)
{
   /** \precond
    **/
    

   /** \action
    * Call function
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
    
    **/
   CHECK_TEXT(type_name<decltype(core_info.cnt_loops)>() == "unsigned int", "Unexpected data type for core_info.cnt_loops");
   CHECK_TEXT(type_name<decltype(core_info.time_us)>() == "unsigned long", "Unexpected data type for core_info.time_us");
   CHECK_TEXT(type_name<decltype(core_info.prev_time_us)>() == "unsigned long", "Unexpected data type for core_info.prev_time_us");
   CHECK_TEXT(type_name<decltype(core_info.elapsed_time_s)>() == "float", "Unexpected data type for core_info.elapsed_time_s");  
}
 /**
**********************************************************************************************************
*                                            rspp_variant_A::RSPP_Detection_List_T                                          *
**********************************************************************************************************
*/

/**
 *\purpose
 * Purpose of this test is to verify rspp_variant_A::RSPP_Detection_List_T interfaces data types according to requirements.
 *\req CPR-4326, CPR-4329
 */
TEST(f360_tracker_integration_types_test, PreProcessor_check_verification_of_detections_data_types)
{
   /** \precond
    **/
    

   /** \action
    * Call function
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
    
    **/
   CHECK_TEXT(type_name<decltype( raw_detection_list.detections[0].raw.sensor_id)>() == "int", "Unexpected data type for raw_detection_list.detections[0].sensor_id");
   CHECK_TEXT(type_name<decltype( raw_detection_list.detections[0].raw.det_id)>() == "int", "Unexpected data type for raw_detection_list.detections[0].det_id");
   CHECK_TEXT(type_name<decltype( raw_detection_list.detections[0].raw.range)>() == "float", "Unexpected data type for raw_detection_list.detections[0].range");
   CHECK_TEXT(type_name<decltype( raw_detection_list.detections[0].raw.range_rate)>() == "float", "Unexpected data type for raw_detection_list.detections[0].range_rate");
   // CHECK_TEXT(type_name<decltype( raw_detection_list.detections[0].std_range_rate)>() == "float", "Unexpected data type for raw_detection_list.detections[0].std_range_rate"); //Note: doesn't exist in polarion
   CHECK_TEXT(type_name<decltype( raw_detection_list.detections[0].raw.azimuth_raw)>() == "float", "Unexpected data type for raw_detection_list.detections[0].azimuth_raw");
   CHECK_TEXT(type_name<decltype( raw_detection_list.detections[0].raw.elevation_raw)>() == "float", "Unexpected data type for raw_detection_list.detections[0].elevation_raw");
   CHECK_TEXT(type_name<decltype( raw_detection_list.detections[0].raw.azimuth)>() == "float", "Unexpected data type for raw_detection_list.detections[0].azimuth");
   // CHECK_TEXT(type_name<decltype( raw_detection_list.detections[0].std_azimuth)>() == "float", "Unexpected data type for raw_detection_list.detections[0].std_azimuth"); // Note:doesn't exist in polarion
   CHECK_TEXT(type_name<decltype( raw_detection_list.detections[0].raw.elevation)>() == "float", "Unexpected data type for raw_detection_list.detections[0].elevation");
   // CHECK_TEXT(type_name<decltype( raw_detection_list.detections[0].std_elevation)>() == "float", "Unexpected data type for raw_detection_list.detections[0].std_elevation");// Note: doesn't exist in polarion
   CHECK_TEXT(type_name<decltype( raw_detection_list.detections[0].raw.snr)>() == "float", "Unexpected data type for raw_detection_list.detections[0].snr");
   CHECK_TEXT(type_name<decltype( raw_detection_list.detections[0].raw.confid_azimuth)>() == "signed char", "Unexpected data type for raw_detection_list.detections[0].confid_azimuth");
   CHECK_TEXT(type_name<decltype( raw_detection_list.detections[0].raw.confid_elevation)>() == "signed char", "Unexpected data type for raw_detection_list.detections[0].confid_elevation");
   CHECK_TEXT(type_name<decltype( raw_detection_list.detections[0].raw.rcs)>() == "float", "Unexpected data type for raw_detection_list.detections[0].rcs");
   CHECK_TEXT(type_name<decltype( raw_detection_list.detections[0].raw.f_super_res)>() == "bool", "Unexpected data type for raw_detection_list.detections[0].f_super_res");
   CHECK_TEXT(type_name<decltype( raw_detection_list.detections[0].raw.f_host_veh_clutter)>() == "bool", "Unexpected data type for raw_detection_list.detections[0].f_host_veh_clutter");
   CHECK_TEXT(type_name<decltype( raw_detection_list.detections[0].raw.f_nd_target)>() == "bool", "Unexpected data type for raw_detection_list.detections[0].f_nd_target");
   // CHECK_TEXT(type_name<decltype( raw_detection_list.detections[0].f_bistatic)>() == "bool", "Unexpected data type for raw_detection_list.detections[0].f_bistatic");//Note: doesn't exist in polarion
   CHECK_TEXT(type_name<decltype( raw_detection_list.number_of_valid_detections)>() == "unsigned int", "Unexpected data type for raw_detection_list.number_of_valid_detections");
   CHECK_TEXT(type_name<decltype(raw_detection_list.vcslong_det_idx_min)>() == "short", "Unexpected data type for raw_detection_list.vcslong_det_idx_min");
   CHECK_TEXT(type_name<decltype(raw_detection_list.vcslong_det_idx_max)>() == "short", "Unexpected data type for raw_detection_list.vcslong_det_idx_max");
   CHECK_TEXT(type_name<decltype(raw_detection_list.vcslong_sorted_ref_det_idx)>() == "short [33]", "Unexpected data type for raw_detection_list.vcslong_sorted_ref_det_idx");
   CHECK_TEXT(type_name<decltype(raw_detection_list.detections[0].processed.next_sorted_idx)>() == "short", "Unexpected data type forraw_detection_list. detections[0].next_sorted_idx");
   CHECK_TEXT(type_name<decltype(raw_detection_list.detections[0].processed.prev_sorted_idx)>() == "short", "Unexpected data type for raw_detection_list.detections[0].prev_sorted_idx");
   CHECK_TEXT(type_name<decltype(raw_detection_list.detections[0].processed.vcs_az)>() == "float", "Unexpected data type for raw_detection_list.detections[0].vcs_az");
   CHECK_TEXT(type_name<decltype(raw_detection_list.detections[0].processed.std_vcs_az_scm)>() == "float", "Unexpected data type for raw_detection_list.detections[0].std_vcs_az_scm");
   CHECK_TEXT(type_name<decltype(raw_detection_list.detections[0].processed.cos_vcs_az)>() == "float", "Unexpected data type for raw_detection_list.detections[0].cos_vcs_az");
   CHECK_TEXT(type_name<decltype(raw_detection_list.detections[0].processed.sin_vcs_az)>() == "float", "Unexpected data type for raw_detection_list.detections[0].sin_vcs_az");
   CHECK_TEXT(type_name<decltype(raw_detection_list.detections[0].processed.rngrate_interval_width)>() == "float", "Unexpected data type for raw_detection_list.detections[0].rngrate_interval_width");
   CHECK_TEXT(type_name<decltype(raw_detection_list.detections[0].processed.vcs_cross_covariances_scm)>() == "float [5]", "Unexpected data type for raw_detection_list.detections[0].prev_sorted_idx");
   CHECK_TEXT(type_name<decltype(raw_detection_list.detections[0].processed.f_is_range_in_all_looks)>() == "bool", "Unexpected data type for raw_detection_list.detections[0].f_is_range_in_all_looks");
   CHECK_TEXT(type_name<decltype(raw_detection_list.detections[0].processed.f_esr_mr_in_lr_fov)>() == "bool", "Unexpected data type for raw_detection_list.detections[0].f_esr_mr_in_lr_fov");
   CHECK_TEXT(type_name<decltype(raw_detection_list.detections[0].processed.look_type)>() == "F360_Det_Look_Type_Tag", "Unexpected data type for raw_detection_list.detections[0].look_type");
   CHECK_TEXT(type_name<decltype(raw_detection_list.detections[0].processed.range_type)>() == "F360_Det_Range_Type_Tag", "Unexpected data type for raw_detection_list.detections[0].range_type");
   CHECK_TEXT(type_name<decltype(raw_detection_list.detections[0].processed.f_elevation_unreliable)>() == "bool", "Unexpected data type for raw_detection_list.detections[0].f_elevation_unreliable");
   CHECK_TEXT(type_name<decltype(raw_detection_list.detections[0].processed.f_azimuth_error_stat_mov)>() == "bool", "Unexpected data type for raw_detection_list.detections[0].f_azimuth_error_stat_mov");
   CHECK_TEXT(type_name<decltype(raw_detection_list.detections[0].processed.vcs_position_cov_scm)>() == "float [2][2]", "Unexpected data type for raw_detection_list.detections[0].vcs_position_cov_scm");
   CHECK_TEXT(type_name<decltype(raw_detection_list.detections[0].processed.global_id)>() == "int", "Unexpected data type for raw_detection_list.detections[0].global_id");
   CHECK_TEXT(type_name<decltype(raw_detection_list.detections[0].processed.motion_status)>() == "f360_variant_A::RSPP_Detection_Motion_Status_T", "Unexpected data type for deteraw_detection_list.detectionsctions[0].motion_status");
   CHECK_TEXT(type_name<decltype(raw_detection_list.detections[0].processed.time_since_measurement)>() == "float", "Unexpected data type for raw_detection_list.detections[0].time_since_measurement");
}
 /**
**********************************************************************************************************
*                                            F360_Detection_Hist_T                                          *
**********************************************************************************************************
*/

/**
 *\purpose
 * Purpose of this test is to verify F360_Detection_Hist_T  interfaces data types according to requirements.
 *\req
 *CPR-4527 ,CPR-4528
 */
TEST(f360_tracker_integration_types_test, PreProcessor_check_verification_of_detections_hist_data_types)
{
   /** \precond
    **/
    

   /** \action
    * Call function
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
    
    **/
   CHECK_TEXT(type_name<decltype(det_hist.det_data[0].vcs_position)>() == "f360_variant_A::Point", "Unexpected data type for det_hist.det_data[0].vcs_position");
   CHECK_TEXT(type_name<decltype(det_hist.det_data[0].vcs_position.x)>() == "float", "Unexpected data type for det_hist.det_data[0].vcs_position.x");
   CHECK_TEXT(type_name<decltype(det_hist.det_data[0].vcs_position.y)>() == "float", "Unexpected data type for det_hist.det_data[0].vcs_position.y");
   CHECK_TEXT(type_name<decltype(det_hist.det_data[0].position_cov_nees)>() == "float [2][2]", "Unexpected data type for det_hist.det_data[0].position_cov_nees");
   CHECK_TEXT(type_name<decltype(det_hist.det_data[0].rdot)>() == "float", "Unexpected data type for det_hist.det_data[0].rdot");
   CHECK_TEXT(type_name<decltype(det_hist.det_data[0].rdot_comp)>() == "float", "Unexpected data type for det_hist.det_data[0].rdot_comp");
   CHECK_TEXT(type_name<decltype(det_hist.det_data[0].vcs_az)>() == "float", "Unexpected data type for det_hist.det_data[0].vcs_az");
   CHECK_TEXT(type_name<decltype(det_hist.det_data[0].time_since_meas)>() == "float", "Unexpected data type for det_hist.det_data[0].time_since_meas");
   CHECK_TEXT(type_name<decltype(det_hist.det_data[0].f_dealiased)>() == "bool", "Unexpected data type for det_hist.det_data[0].f_dealiased");
   CHECK_TEXT(type_name<decltype(det_hist.det_data[0].f_FOV_edge)>() == "bool", "Unexpected data type for det_hist.det_data[0].f_FOV_edge");
   CHECK_TEXT(type_name<decltype(det_hist.det_data[0].f_selected)>() == "bool", "Unexpected data type for det_hist.det_data[0].f_selected");
   CHECK_TEXT(type_name<decltype(det_hist.det_data[0].f_esr_mr_in_lr_fov)>() == "bool", "Unexpected data type for det_hist.det_data[0].f_esr_mr_in_lr_fov");
   CHECK_TEXT(type_name<decltype(det_hist.det_data[0].f_azimuth_error_stat_mov)>() == "bool", "Unexpected data type for det_hist.det_data[0].f_azimuth_error_stat_mov");
   CHECK_TEXT(type_name<decltype(det_hist.det_data[0].f_is_range_in_all_looks)>() == "bool", "Unexpected data type for det_hist.det_data[0].f_is_range_in_all_looks");
   CHECK_TEXT(type_name<decltype(det_hist.det_data[0].f_potential_angle_jump)>() == "bool", "Unexpected data type for det_hist.det_data[0].f_potential_angle_jump");
   CHECK_TEXT(type_name<decltype(det_hist.det_data[0].look_type)>() == "F360_Det_Look_Type_Tag", "Unexpected data type for det_hist.det_data[0].look_type");
   CHECK_TEXT(type_name<decltype(det_hist.det_data[0].range_type)>() == "F360_Det_Range_Type_Tag", "Unexpected data type for det_hist.det_data[0].range_type");
   CHECK_TEXT(type_name<decltype(det_hist.det_data[0].wheel_spin_type)>() == "f360_variant_A::F360_Detection_Wheelspin_Type_Tag", "Unexpected data type for det_hist.det_data[0].wheel_spin_type");
   CHECK_TEXT(type_name<decltype(det_hist.det_data[0].motion_status)>() == "f360_variant_A::RSPP_Detection_Motion_Status_T", "Unexpected data type for det_hist.det_data[0].motion_status");
   CHECK_TEXT(type_name<decltype(det_hist.det_data[0].rngrate_interval_width)>() == "float", "Unexpected data type for det_hist.det_data[0].rngrate_interval_width");
   CHECK_TEXT(type_name<decltype(det_hist.det_data[0].cluster_idx)>() == "short", "Unexpected data type for det_hist.det_data[0].cluster_idx");
}

 /**
**********************************************************************************************************
*                                            F360_Globals_T                                         *
**********************************************************************************************************
*/

/**
 *\purpose
 * Purpose of this test is to verify F360_Globals_T interfaces data types according to requirements.
 *\req
 * CPR-4517
 */
TEST(f360_tracker_integration_types_test, PreProcessor_check_verification_of_globals_data_types)
{
   /** \precond
    **/
    

   /** \action
    * Call function
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
    
    **/
   CHECK_TEXT(type_name<decltype(globals.obj_mov_stat_spd_thresh)>() == "float", "Unexpected data type for globals.obj_mov_stat_spd_thresh"); 
   CHECK_TEXT(type_name<decltype(globals.rear_cornering_compliance)>() == "float", "Unexpected data type for globals.rear_cornering_compliance"); 
   CHECK_TEXT(type_name<decltype(globals.oncoming_speed_thresh)>() == "float", "Unexpected data type for globals.oncoming_speed_thresh"); 
   CHECK_TEXT(type_name<decltype(globals.obj_vehicular_spd_thresh)>() == "float", "Unexpected data type for globals.obj_vehicular_spd_thresh"); 
   CHECK_TEXT(type_name<decltype(globals.default_half_length)>() == "float", "Unexpected data type for globals.default_half_length"); 
   CHECK_TEXT(type_name<decltype(globals.default_half_width)>() == "float", "Unexpected data type for globals.default_half_width"); 
   CHECK_TEXT(type_name<decltype(globals.default_length)>() == "float", "Unexpected data type for globals.default_length"); 
   CHECK_TEXT(type_name<decltype(globals.default_width)>() == "float", "Unexpected data type for globals.default_width"); 
   CHECK_TEXT(type_name<decltype(globals.default_height)>() == "float", "Unexpected data type for globals.default_height"); 
   CHECK_TEXT(type_name<decltype(globals.max_otg_speed)>() == "float", "Unexpected data type for globals.max_otg_speed"); 
   CHECK_TEXT(type_name<decltype(globals.rotated_left_fov_normal)>() == "float [10][2]", "Unexpected data type for globals.rotated_left_fov_normal"); 
   CHECK_TEXT(type_name<decltype(globals.rotated_right_fov_normal)>() == "float [10][2]", "Unexpected data type for globals.rotated_right_fov_normal"); 
   CHECK_TEXT(type_name<decltype(globals.f_single_front_center_radar_only)>() == "bool", "Unexpected data type for globals.f_single_front_center_radar_only"); 
}

 /**
**********************************************************************************************************
*                                            F360_Host_Props_T                                         *
**********************************************************************************************************
*/
/**
 *\purpose
 * Purpose of this test is to verify F360_Host_Props_T interfaces data types according to requirements.
 *\req
 * CPR-4521
 */
TEST(f360_tracker_integration_types_test, PreProcessor_check_verification_of_host_props_data_types)
{
   /** \precond
    **/
    

   /** \action
    * Call function
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
    
    **/
   CHECK_TEXT(type_name<decltype( host_props.position)>() == "f360_variant_A::Point", "Unexpected data type for host_props.position");
   CHECK_TEXT(type_name<decltype( host_props.vel_cov_scm)>() == "float [2][2]", "Unexpected data type for host_props.vel_cov_scm"); 
   CHECK_TEXT(type_name<decltype( host_props.vel_cov)>() == "float [2][2]", "Unexpected data type for host_props.vel_cov"); 
   CHECK_TEXT(type_name<decltype( host_props.position_inc_cov_scm)>() == "float [2][2]", "Unexpected data type for host_props.position_inc_cov_scm"); 
   CHECK_TEXT(type_name<decltype( host_props.position_inc_cov)>() == "float [2][2]", "Unexpected data type for host_props.position_inc_cov"); 
   CHECK_TEXT(type_name<decltype( host_props.std_speed_scm)>() == "float", "Unexpected data type for host_props.std_speed_scm"); 
   CHECK_TEXT(type_name<decltype( host_props.std_yaw_rate_scm)>() == "float", "Unexpected data type for host_props.std_yaw_rate_scm"); 
   CHECK_TEXT(type_name<decltype( host_props.heading_angle)>() == "float", "Unexpected data type for host_props.heading_angle"); 
   CHECK_TEXT(type_name<decltype( host_props.cos_heading)>() == "float", "Unexpected data type for host_props.cos_heading"); 
   CHECK_TEXT(type_name<decltype( host_props.sin_heading)>() == "float", "Unexpected data type for host_props.sin_heading"); 
   CHECK_TEXT(type_name<decltype( host_props.delta_pointing)>() == "float", "Unexpected data type for host_props.delta_pointing");                       // Note: doesn't exist in polarion
   CHECK_TEXT(type_name<decltype( host_props.cos_delta_pointing)>() == "float", "Unexpected data type for host_props.cos_delta_pointing");               // Note: doesn't exist in polarion
   CHECK_TEXT(type_name<decltype( host_props.sin_delta_pointing)>() == "float", "Unexpected data type for host_props.sin_delta_pointing");               // Note: doesn't exist in polarion
   CHECK_TEXT(type_name<decltype( host_props.delta_position)>() == "f360_variant_A::Point", "Unexpected data type for host_props.delta_position");                 // Note: doesn't exist in polarion 
}
/**
**********************************************************************************************************
*                                            F360_Radar_Sensor_T                                         *
**********************************************************************************************************
*/
/**
 *\purpose
 * Purpose of this test is to verify F360_Radar_Sensor_T interfaces data types according to requirements.
 *\req
 * CPR-4308,CPR-4319
 */
TEST(f360_tracker_integration_types_test, PreProcessor_check_verification_of_radar_sensor_data_types)
{
   /** \precond
    **/
    

   /** \action
    * Call function
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
    
    **/
   CHECK_TEXT(type_name<decltype( sensors[0].variable.timestamp_us)>() == "unsigned long", "Unexpected data type for sensors[0].variable.timestamp_us");
   CHECK_TEXT(type_name<decltype( sensors[0].variable.number_of_valid_detections)>() == "unsigned int", "Unexpected data type for sensors[0].variable.number_of_valid_detections");
   CHECK_TEXT(type_name<decltype( sensors[0].variable.look_index)>() == "unsigned short", "Unexpected data type for sensors[0].look_index");
   CHECK_TEXT(type_name<decltype( sensors[0].variable.look_id)>() == "F360_Det_Look_ID_Tag", "Unexpected data type for sensors[0].variable.look_id");
   CHECK_TEXT(type_name<decltype( sensors[0].variable.vcs_velocity)>() == "F360_VCS_Velocity_Tag", "Unexpected data type for sensors[0].variable.vcs_velocity");
   CHECK_TEXT(type_name<decltype( sensors[0].variable.yaw_rate_calc_dps)>() == "float", "Unexpected data type for sensors[0].yaw_rate_calc_dps");
   CHECK_TEXT(type_name<decltype( sensors[0].variable.vehicle_speed_calc_mps)>() == "float", "Unexpected data type for sensors[0].vehicle_speed_calc_mps");
}
/**
**********************************************************************************************************
*                                            F360_Radar_Sensor_T                                         *
**********************************************************************************************************
*/
/**
 *\purpose
 * Purpose of this test is to verify F360_Radar_Sensor_T interfaces data types according to requirements.
 *\req
 * CPR-4304
 */
TEST(f360_tracker_integration_types_test, PreProcessor_check_verification_of_radar_sensor_calib_data_types)
{
   /** \precond
    **/
    

   /** \action
    * Call function
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
    
    **/
   CHECK_TEXT(type_name<decltype( sensors[0].constant.id)>() == "unsigned int", "Unexpected data type for  sensors[0].constant.id");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.sensor_type)>() == "F360_Sensor_Type_Tag", "Unexpected data type for  sensors[0].constant.sensor_type");
   CHECK_TEXT(type_name<decltype( sensors[0].variable.is_valid)>() == "bool", "Unexpected data type for  sensors[0].variable.is_valid");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.mounting_location)>() == "F360_Mounting_Location_Tag", "Unexpected data type for  sensors[0].constant.mounting_location");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.mounting_position)>() == "F360_Sensor_Mounting_Position_Tag", "Unexpected data type for  sensors[0].constant.mounting_position");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.polarity)>() == "int", "Unexpected data type for  sensors[0].constant.polarity");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.range_limits)>() == "float [4]", "Unexpected data type for  sensors[0].constant.range_limits"); //Note: polarion doesn't indicate array lenth
   CHECK_TEXT(type_name<decltype( sensors[0].constant.fov_min_az_rad)>() == "float [4]", "Unexpected data type for  sensors[0].constant.fov_min_az_rad"); //Note: polarion doesn't indicate array lenth
   CHECK_TEXT(type_name<decltype( sensors[0].constant.fov_max_az_rad)>() == "float [4]", "Unexpected data type for  sensors[0].constant.fov_max_az_rad"); //Note: polarion doesn't indicate array lenth
   CHECK_TEXT(type_name<decltype( sensors[0].constant.fov_min_el_rad)>() == "float [4]", "Unexpected data type for  sensors[0].constant.fov_min_el_rad"); //Note: polarion doesn't indicate array lenth
   CHECK_TEXT(type_name<decltype( sensors[0].constant.fov_max_el_rad)>() == "float [4]", "Unexpected data type for  sensors[0].constant.fov_max_el_rad"); //Note: polarion doesn't indicate array lenth
   CHECK_TEXT(type_name<decltype( sensors[0].constant.min_aliaised_range_rate)>() == "float [4]", "Unexpected data type for  sensors[0].constant.min_aliaised_range_rate"); //Note: polarion doesn't indicate array lenth
   CHECK_TEXT(type_name<decltype( sensors[0].constant.v_wrapping)>() == "float [4]", "Unexpected data type for  sensors[0].constant.v_wrapping"); //Note: polarion doesn't indicate array lenth
   CHECK_TEXT(type_name<decltype( sensors[0].variable.f_ant_sens_available)>() == "bool", "Unexpected data type for  sensors[0].variable.f_ant_sens_available");
   CHECK_TEXT(type_name<decltype( sensors[0].variable.f_ant_sens_degraded)>() == "bool", "Unexpected data type for  sensors[0].variable.f_ant_sens_degraded");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.ant_sens_SCS_azim)>() == "float [18]", "Unexpected data type for  sensors[0].constant.ant_sens_SCS_azim");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.ant_sens_SCS_sq_rng_90)>() == "float [18]", "Unexpected data type for  sensors[0].constant.ant_sens_SCS_sq_rng_90");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.ant_sens_SCS_sq_rng_50)>() == "float [18]", "Unexpected data type for  sensors[0].constant.ant_sens_SCS_sq_rng_50");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.f_read_cdc_data)>() == "bool", "Unexpected data type for  sensors[0].constant.f_read_cdc_data");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.sensor_sw_version)>() == "unsigned int", "Unexpected data type for  sensors[0].constant.sensor_sw_version");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.internal_reflections.min_host_vel)>()=="float","Unexpected data type for  sensors[0].constant.internal_reflections.min_host_vel");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.internal_reflections.age_threshold)>()=="unsigned short","Unexpected data type for  sensors[0].constant.internal_reflections.age_threshold");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.internal_reflections.occurrence_lowerlimit)>()=="float","Unexpected data type for  sensors[0].constant.internal_reflections.occurrence_lowerlimit");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.internal_reflections.occurrence_threshold)>()=="float","Unexpected data type for  sensors[0].constant.internal_reflections.occurrence_threshold");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.internal_reflections.rcs_tolerance)>()=="float","Unexpected data type for  sensors[0].constant.internal_reflections.rcs_tolerance");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.internal_reflections.azimuth_tolerance)>()=="float","Unexpected data type for  sensors[0].constant.internal_reflections.azimuth_tolerance");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.internal_reflections.range_tolerance)>()=="float","Unexpected data type for  sensors[0].constant.internal_reflections.range_tolerance");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.internal_reflections.max_abs_range_rate)>()=="float","Unexpected data type for  sensors[0].constant.internal_reflections.max_abs_range_rate");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.internal_reflections.rcs_max)>()=="float","Unexpected data type for  sensors[0].constant.internal_reflections.rcs_max");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.internal_reflections.range_max)>()=="float","Unexpected data type for  sensors[0].constant.internal_reflections.range_max");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.interior_fov)>() == "float [4]", "Unexpected data type for sensors[0].constant.interior_fov");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.left_fov_normal)>() == "float [4]", "Unexpected data type for sensors[0].constant.left_fov_normal");
   CHECK_TEXT(type_name<decltype( sensors[0].constant.right_fov_normal)>() == "float [4]", "Unexpected data type for sensors[0].constant.right_fov_normal");
   CHECK_TEXT(type_name<decltype( sensors[0].variable.time_since_measurement_s)>() == "float", "Unexpected data type for sensors[0].variable.time_since_measurement_s");
   CHECK_TEXT(type_name<decltype( sensors[0].variable.first_detection_list_idx)>() == "int", "Unexpected data type for sensors[0].variable.first_detection_list_idx");
}
/**
**********************************************************************************************************
*                                            F360_Radar_Sensor_Props_T                                   *
**********************************************************************************************************
*/
/**
 *\purpose
 * Purpose of this test is to verify F360_Radar_Sensor_Props_T interfaces data types according to requirements.
 *\req
 * CPR-4523
 */
TEST(f360_tracker_integration_types_test, PreProcessor_check_verification_of_radar_sensor_props_data_types)
{
   /** \precond
    **/
    

   /** \action
    * Call function
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
    
    **/
   CHECK_TEXT(type_name<decltype(  sensor_props[0].time_since_measurement_s)>() == "float", "Unexpected data type for   sensor_props[0].time_since_measurement_s");
   CHECK_TEXT(type_name<decltype(  sensor_props[0].first_detection_list_idx)>() == "int", "Unexpected data type for   sensor_props[0].first_detection_list_idx "); //Note: doesn't exist in polarion
   CHECK_TEXT(type_name<decltype(  sensor_props[0].f_object_track_next_to_sensor)>() == "bool", "Unexpected data type for   sensor_props[0].f_object_track_next_to_sensor ");
   CHECK_TEXT(type_name<decltype(  sensor_props[0].next_to_sensor_object_track_id)>() == "int", "Unexpected data type for   sensor_props[0].next_to_sensor_object_track_id  ");
   CHECK_TEXT(type_name<decltype(  sensor_props[0].next_to_sensor_object_track_min_long_pos)>() == "float", "Unexpected data type for sensor_props[0].next_to_sensor_object_track_min_long_pos");
   CHECK_TEXT(type_name<decltype(  sensor_props[0].next_to_sensor_object_track_max_long_pos)>() == "float", "Unexpected data type for sensor_props[0].next_to_sensor_object_track_max_long_pos");
   CHECK_TEXT(type_name<decltype(  sensor_props[0].next_to_sensor_object_track_min_lat_pos)>() == "float", "Unexpected data type for sensor_props[0].next_to_sensor_object_track_min_lat_pos");
   CHECK_TEXT(type_name<decltype(  sensor_props[0].next_to_sensor_object_track_max_lat_pos)>() == "float", "Unexpected data type for sensor_props[0].next_to_sensor_object_track_max_lat_pos");
   CHECK_TEXT(type_name<decltype(  sensor_props[0].interior_fov)>() == "float [4]", "Unexpected data type for sensor_props[0].interior_fov");
   CHECK_TEXT(type_name<decltype(  sensor_props[0].left_fov_normal)>() == "float [4]", "Unexpected data type for sensor_props[0].left_fov_normal");
   CHECK_TEXT(type_name<decltype(  sensor_props[0].right_fov_normal)>() == "float [4]", "Unexpected data type for sensor_props[0].right_fov_normal");
   CHECK_TEXT(type_name<decltype(  sensor_props[0].internal_reflections_buffer[0].range)>()=="float","Unexpected data type for sensor_props[0].internal_reflections_buffer[0].range");
   CHECK_TEXT(type_name<decltype(  sensor_props[0].internal_reflections_buffer[0].azimuth)>()=="float","Unexpected data type for sensor_props[0].internal_reflections_buffer[0].azimuth");
   CHECK_TEXT(type_name<decltype(  sensor_props[0].internal_reflections_buffer[0].rcs)>()=="float","Unexpected data type for sensor_props[0].internal_reflections_buffer[0].rcs");
   CHECK_TEXT(type_name<decltype(  sensor_props[0].internal_reflections_buffer[0].occurrence_count)>()=="unsigned int","Unexpected data type for sensor_props[0].internal_reflections_buffer[0].occurrence_count");
   CHECK_TEXT(type_name<decltype(  sensor_props[0].internal_reflections_buffer[0].age)>()=="unsigned int","Unexpected data type for sensor_props[0].internal_reflections_buffer[0].age");
   CHECK_TEXT(type_name<decltype(  sensor_props[0].internal_reflections_buffer[0].f_updated_this_cycle)>()=="bool","Unexpected data type for sensor_props[0].internal_reflections_buffer[0].f_updated_this_cycle");
   CHECK_TEXT(type_name<decltype(  sensor_props[0].internal_reflections_buffer[0].f_classified_as_internal_reflection)>()=="bool","Unexpected data type for sensor_props[0].internal_reflections_buffer[0].f_classified_as_internal_reflection");
   CHECK_TEXT(type_name<decltype(  sensor_props[0].internal_reflections_buffer[0].padding)>()=="unsigned char [2]","Unexpected data type for sensor_props[0].internal_reflections_buffer[0].padding");
}
/**
**********************************************************************************************************
*                                            F360_TRKR_TIMING_INFO_T                                        *
**********************************************************************************************************
*/
/**
 *\purpose
 * Purpose of this test is to verify F360_TRKR_TIMING_INFO_T interfaces data types according to requirements.
 *\req
 * CPR-4534 , CPR-4479
 */
TEST(f360_tracker_integration_types_test, PreProcessor_check_verification_of_traker_timing_info_data_types)
{
   /** \precond
    **/
    

   /** \action
    * Call function
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
    
    **/
    CHECK_TEXT(type_name<decltype(timing_info.core_tracker)>() == "float", "Unexpected data type for timing_info.core_tracker");
    CHECK_TEXT(type_name<decltype(timing_info.sensor_preprocessing)>() == "float", "Unexpected data type for timing_info.sensor_preprocessing");
    CHECK_TEXT(type_name<decltype(timing_info.time_update_tracks)>() == "float", "Unexpected data type for timing_info.time_update_tracks");
    CHECK_TEXT(type_name<decltype(timing_info.pre_association_track_management)>() == "float", "Unexpected data type for timing_info.pre_association_track_management");
    CHECK_TEXT(type_name<decltype(timing_info.detection_to_track_association)>() == "float", "Unexpected data type for timing_info.detection_to_track_association");
    CHECK_TEXT(type_name<decltype(timing_info.clustering)>() == "float", "Unexpected data type for timing_info.clustering");
    CHECK_TEXT(type_name<decltype(timing_info.cluster_grouping)>() == "float", "Unexpected data type for timing_info.cluster_grouping");
    CHECK_TEXT(type_name<decltype(timing_info.track_grouping)>() == "float", "Unexpected data type for timing_info.track_grouping");
    CHECK_TEXT(type_name<decltype(timing_info.measurement_update_tracks)>() == "float", "Unexpected data type for timing_info.measurement_update_tracks");
    CHECK_TEXT(type_name<decltype(timing_info.initialize_tracks)>() == "float", "Unexpected data type for timing_info.initialize_tracks");
    CHECK_TEXT(type_name<decltype(timing_info.post_update_track_adjustments)>() == "float", "Unexpected data type for timing_info.post_update_track_adjustments");
    CHECK_TEXT(type_name<decltype(timing_info.track_classification)>() == "float", "Unexpected data type for timing_info.track_classification");
    CHECK_TEXT(type_name<decltype(timing_info.track_validity)>() == "float", "Unexpected data type for timing_info.track_validity");
    CHECK_TEXT(type_name<decltype(timing_info.track_downselection)>() == "float", "Unexpected data type for timing_info.track_downselection");
    CHECK_TEXT(type_name<decltype(timing_info.e2e_protection)>() == "float", "Unexpected data type for timing_info.e2e_protection");
    CHECK_TEXT(type_name<decltype(timing_info.lsc_module)>() == "float", "Unexpected data type for timing_info.lsc_module");
    CHECK_TEXT(type_name<decltype(timing_info.concrete_wall_detector)>() == "float", "Unexpected data type for timing_info.concrete_wall_detector");
    CHECK_TEXT(type_name<decltype(timing_info.host_state_update)>() == "float", "Unexpected data type for timing_info.host_state_update");
    CHECK_TEXT(type_name<decltype(timing_info.calc_obj_mov_stat_thresh)>() == "float", "Unexpected data type for timing_info.calc_obj_mov_stat_thresh");
    CHECK_TEXT(type_name<decltype(timing_info.update_sensor_valid_info)>() == "float", "Unexpected data type for timing_info.update_sensor_valid_info");
    CHECK_TEXT(type_name<decltype(timing_info.configure_rdot_interval_compability)>() == "float", "Unexpected data type for timing_info.configure_rdot_interval_compability");
    CHECK_TEXT(type_name<decltype(timing_info.sensor_motion)>() == "float", "Unexpected data type for timing_info.sensor_motion");
    CHECK_TEXT(type_name<decltype(timing_info.update_det_hist)>() == "float", "Unexpected data type for timing_info.update_det_hist");
    CHECK_TEXT(type_name<decltype(timing_info.ego_motion_compensate_dets)>() == "float", "Unexpected data type for timing_info.ego_motion_compensate_dets");
    CHECK_TEXT(type_name<decltype(timing_info.detect_wheelspin)>() == "float", "Unexpected data type for timing_info.detect_wheelspin");
    CHECK_TEXT(type_name<decltype(timing_info.check_az_el_conf)>() == "float", "Unexpected data type for timing_info.check_az_el_conf");
    CHECK_TEXT(type_name<decltype(timing_info.mark_out_det_pairs)>() == "float", "Unexpected data type for timing_info.mark_out_det_pairs");
    CHECK_TEXT(type_name<decltype(timing_info.sensor_capability_module)>() == "float", "Unexpected data type for timing_info.sensor_capability_module");
    CHECK_TEXT(type_name<decltype(timing_info.double_bounce_detection_countermeasure)>() == "float", "Unexpected data type for timing_info.double_bounce_detection_countermeasure");
    CHECK_TEXT(type_name<decltype(timing_info.time_update_obj_trks_cca)>() == "float", "Unexpected data type for timing_info.time_update_obj_trks_cca");
    CHECK_TEXT(type_name<decltype(timing_info.time_update_obj_trks_ctca)>() == "float", "Unexpected data type for timing_info.time_update_obj_trks_ctca");
    CHECK_TEXT(type_name<decltype(timing_info.split_tracks)>() == "float", "Unexpected data type for timing_info.split_tracks");
    CHECK_TEXT(type_name<decltype(timing_info.kill_coasted_tracks)>() == "float", "Unexpected data type for timing_info.kill_coasted_tracks");
    CHECK_TEXT(type_name<decltype(timing_info.assoc_dets_with_obj_trks)>() == "float", "Unexpected data type for timing_info.assoc_dets_with_obj_trks");
    CHECK_TEXT(type_name<decltype(timing_info.assoc_countermeasure)>() == "float", "Unexpected data type for timing_info.assoc_countermeasure");
    CHECK_TEXT(type_name<decltype(timing_info.store_det_data)>() == "float", "Unexpected data type for timing_info.store_det_data");
    CHECK_TEXT(type_name<decltype(timing_info.det_downselect)>() == "float", "Unexpected data type for timing_info.det_downselect");
    CHECK_TEXT(type_name<decltype(timing_info.cluster_moving_detections)>() == "float", "Unexpected data type for timing_info.cluster_moving_detections");
    CHECK_TEXT(type_name<decltype(timing_info.cluster_leftover_detections)>() == "float", "Unexpected data type for timing_info.cluster_leftover_detections");
    CHECK_TEXT(type_name<decltype(timing_info.m_initialize_clusters)>() == "float", "Unexpected data type for timing_info.initialize_clusters");
    CHECK_TEXT(type_name<decltype(timing_info.assoc_unconf_obj_trks)>() == "float", "Unexpected data type for timing_info.assoc_unconf_obj_trks");
    CHECK_TEXT(type_name<decltype(timing_info.predict_existence_probability)>() == "float", "Unexpected data type for timing_info.predict_existence_probability");
    CHECK_TEXT(type_name<decltype(timing_info.update_existence_probability)>() == "float", "Unexpected data type for timing_info.update_existence_probability");
    CHECK_TEXT(type_name<decltype(timing_info.pseudo_estimations)>() == "float", "Unexpected data type for timing_info.pseudo_estimations");
    CHECK_TEXT(type_name<decltype(timing_info.obj_trk_status_bookkeeping)>() == "float", "Unexpected data type for timing_info.obj_trk_status_bookkeeping");
    CHECK_TEXT(type_name<decltype(timing_info.adjust_fltr_type_dependent_params)>() == "float", "Unexpected data type for timing_info.adjust_fltr_type_dependent_params");
    CHECK_TEXT(type_name<decltype(timing_info.obj_trk_properties)>() == "float", "Unexpected data type for timing_info.obj_trk_properties");
    CHECK_TEXT(type_name<decltype(timing_info.cancel_new_updated_trk_overlapping_confirmed_trks)>() == "float", "Unexpected data type for timing_info.cancel_new_updated_trk_overlapping_confirmed_trks");
    CHECK_TEXT(type_name<decltype(timing_info.adjust_overlapping_confirmed_trks)>() == "float", "Unexpected data type for timing_info.adjust_overlapping_confirmed_trks");
    CHECK_TEXT(type_name<decltype(timing_info.det_mean_var)>() == "float", "Unexpected data type for timing_info.det_mean_var");
    CHECK_TEXT(type_name<decltype(timing_info.assign_underdrivability_status_to_tracks_ocg)>() == "float", "Unexpected data type for timing_info.assign_underdrivability_status_to_tracks_ocg");
    CHECK_TEXT(type_name<decltype(timing_info.mark_object_tracks_next_to_sensors)>() == "float", "Unexpected data type for timing_info.mark_object_tracks_next_to_sensors");
    CHECK_TEXT(type_name<decltype(timing_info.sanity_check)>() == "float", "Unexpected data type for timing_info.sanity_check");
    CHECK_TEXT(type_name<decltype(timing_info.msmt_update_obj_trks_cca_non_moveable)>() == "float", "Unexpected data type for timing_info.msmt_update_obj_trks_cca_non_moveable");
    CHECK_TEXT(type_name<decltype(timing_info.msmt_update_obj_trks_cca_moveable)>() == "float", "Unexpected data type for timing_info.msmt_update_obj_trks_cca_moveable");    
    CHECK_TEXT(type_name<decltype(timing_info.msmt_update_obj_trks_cca)>() == "float", "Unexpected data type for timing_info.msmt_update_obj_trks_cca");
    CHECK_TEXT(type_name<decltype(timing_info.occlusion)>() == "float", "Unexpected data type for timing_info.occlusion");
    CHECK_TEXT(type_name<decltype(timing_info.msmt_update_obj_trks_ctca)>() == "float", "Unexpected data type for timing_info.msmt_update_obj_trks_ctca");
    CHECK_TEXT(type_name<decltype(timing_info.assoc_unconf_obj_trks_sub_time_1)>() == "float", "Unexpected data type for timing_info.assoc_unconf_obj_trks_sub_time_1");
    CHECK_TEXT(type_name<decltype(timing_info.assoc_unconf_obj_trks_sub_time_2)>() == "float", "Unexpected data type for timing_info.assoc_unconf_obj_trks_sub_time_2");
    CHECK_TEXT(type_name<decltype(timing_info.assoc_unconf_obj_trks_sub_time_3)>() == "float", "Unexpected data type for timing_info.assoc_unconf_obj_trks_sub_time_3");
    CHECK_TEXT(type_name<decltype(timing_info.assoc_unconf_obj_trks_sub_time_4)>() == "float", "Unexpected data type for timing_info.assoc_unconf_obj_trks_sub_time_4");
    CHECK_TEXT(type_name<decltype(timing_info.assoc_unconf_obj_trks_sub_time_5)>() == "float", "Unexpected data type for timing_info.assoc_unconf_obj_trks_sub_time_5");
    CHECK_TEXT(type_name<decltype(timing_info.assoc_unconf_obj_trks_sub_time_6)>() == "float", "Unexpected data type for timing_info.assoc_unconf_obj_trks_sub_time_6");

}
/**
**********************************************************************************************************
*                                            F360_Tracker_Info_T                                        *
**********************************************************************************************************
*/
/**
 *\purpose
 * Purpose of this test is to verify F360_Tracker_Info_T interfaces data types according to requirements.
 *\req
 * CPR-4533
 */
TEST(f360_tracker_integration_types_test, PreProcessor_check_verification_of_traker_info_data_types)
{
   /** \precond
    **/
    

   /** \action
    * Call function
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
    
    **/
   CHECK_TEXT(type_name<decltype(  tracker_info.active_cluster_ids)>() == "short [2000]", "Unexpected data type for   tracker_info.active_cluster_ids");//Note: doesn't exist in polarion
   CHECK_TEXT(type_name<decltype(  tracker_info.inactive_cluster_ids)>() == "short [2000]", "Unexpected data type for   tracker_info.inactive_cluster_ids");//Note: doesn't exist in polarion
   CHECK_TEXT(type_name<decltype(  tracker_info.num_active_clusters)>() == "short", "Unexpected data type for   tracker_info.num_active_clusters");
   CHECK_TEXT(type_name<decltype(  tracker_info.active_obj_ids)>() == "int [1000]", "Unexpected data type for   tracker_info.active_obj_ids");
   CHECK_TEXT(type_name<decltype(  tracker_info.inactive_obj_ids)>() == "int [1000]", "Unexpected data type for   tracker_info.inactive_obj_ids");
   CHECK_TEXT(type_name<decltype(  tracker_info.object_list_timestamp)>() == "unsigned long", "Unexpected data type for   tracker_info.object_list_timestamp");//Note: doesn't exist in polarion
   CHECK_TEXT(type_name<decltype(  tracker_info.num_active_objs)>() == "int", "Unexpected data type for   tracker_info.num_active_objs");
   CHECK_TEXT(type_name<decltype(  tracker_info.reduced_active_obj_ids)>() == "int [500]", "Unexpected data type for   tracker_info.reduced_active_obj_ids");  //Note: No Array Information in polarion
   CHECK_TEXT(type_name<decltype(  tracker_info.reduced_inactive_obj_ids)>() == "int [500]", "Unexpected data type for   tracker_info.num_active_objs");       //Note: No Array Information in polarion
   CHECK_TEXT(type_name<decltype(  tracker_info.reduced_obj_ids)>() == "int [500]", "Unexpected data type for   tracker_info.num_active_objs");                //Note: No Array Information in polarion
   CHECK_TEXT(type_name<decltype(  tracker_info.reduced_num_active_objs)>() == "int", "Unexpected data type for   tracker_info.reduced_num_active_objs");
   CHECK_TEXT(type_name<decltype(  tracker_info.cnt_loops)>() == "unsigned int", "Unexpected data type for   tracker_info.cnt_loops");
   CHECK_TEXT(type_name<decltype(  tracker_info.elapsed_time_s)>() == "float", "Unexpected data type for   tracker_info.elapsed_time_s");
   CHECK_TEXT(type_name<decltype(  tracker_info.unique_rdot_interval_widths)>() == "float [40]", "Unexpected data type for   tracker_info.unique_rdot_interval_widths");                //Note: No Array Information in polarion
   CHECK_TEXT(type_name<decltype(  tracker_info.num_unique_rdot_interval_widths)>() == "int", "Unexpected data type for   tracker_info.num_unique_rdot_interval_widths");
   CHECK_TEXT(type_name<decltype(  tracker_info.rdot_interval_compatibility)>() == "bool [40][40]", "Unexpected data type for   tracker_info.rdot_interval_compatibility");                //Note: No Array Information in polarion
   CHECK_TEXT(type_name<decltype(  tracker_info.f_esr_sensor_valid)>() == "bool", "Unexpected data type for   tracker_info.f_esr_sensor_valid");
   CHECK_TEXT(type_name<decltype(  tracker_info.f_srr2_sensor_valid)>() == "bool", "Unexpected data type for   tracker_info.f_srr2_sensor_valid");
   CHECK_TEXT(type_name<decltype(  tracker_info.vcslong_sorted_trackid)>() == "int [1000]", "Unexpected data type for   tracker_info.vcslong_sorted_trackid");                //Note: No Array Information in polarion
   CHECK_TEXT(type_name<decltype(  tracker_info.vcslong_sorted_start)>() == "f360_variant_A::F360_Object_Track_T*", "Unexpected data type for   tracker_info.vcslong_sorted_start");
   CHECK_TEXT(type_name<decltype(  tracker_info.vcslong_sorted_next_track)>() == "f360_variant_A::F360_Object_Track_T* [1000]", "Unexpected data type for tracker_info.vcslong_sorted_next_track");                //Note: No Array Information in polarion
   CHECK_TEXT(type_name<decltype(  tracker_info.vcslong_sorted_prev_track)>() == "f360_variant_A::F360_Object_Track_T* [1000]", "Unexpected data type for tracker_info.vcslong_sorted_prev_track");                //Note: No Array Information in polarion
   CHECK_TEXT(type_name<decltype(  tracker_info.vcslong_sorted_cluster_list)>() == "short [2000]", "Unexpected data type for tracker_info.vcslong_sorted_cluster_list");                //Note: No Array Information in polarion
   CHECK_TEXT(type_name<decltype(  tracker_info.vcslong_sorted_cluster_start)>() == "f360_variant_A::F360_Cluster_Tag*", "Unexpected data type for tracker_info.vcslong_sorted_cluster_start");
   CHECK_TEXT(type_name<decltype(  tracker_info.vcslong_sorted_cluster_next)>() == "f360_variant_A::F360_Cluster_Tag* [2000]", "Unexpected data type for tracker_info.vcslong_sorted_cluster_next");                //Note: No Array Information in polarion
   CHECK_TEXT(type_name<decltype(  tracker_info.vcslong_sorted_cluster_prev)>() == "f360_variant_A::F360_Cluster_Tag* [2000]", "Unexpected data type for tracker_info.vcslong_sorted_cluster_prev");                //Note: No Array Information in polarion
   CHECK_TEXT(type_name<decltype(  tracker_info.p_highest_priority_track)>() == "f360_variant_A::F360_Object_Track_T*", "Unexpected data type for tracker_info.p_highest_priority_track");
   CHECK_TEXT(type_name<decltype(  tracker_info.p_lowest_priority_track)>() == "f360_variant_A::F360_Object_Track_T*", "Unexpected data type for tracker_info.p_lowest_priority_track");
}

/**
**********************************************************************************************************
*                                            F360_Object_Track_T                                        *
**********************************************************************************************************
*/
/**
 *\purpose
 * Purpose of this test is to verify F360_Object_Track_T interfaces data types according to requirements.
 *\req
 *  CPR-4536
 */
TEST(f360_tracker_integration_types_test, PreProcessor_check_verification_of_object_tracks_data_types)
{
   /** \precond
    **/
    

   /** \action
    * Call function
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
    **/
   CHECK_TEXT(type_name<decltype(object_tracks[0].pseudo_vcs_position)>() == "f360_variant_A::Point", "Unexpected data type for object_tracks[0].pseudo_vcs_position");
   CHECK_TEXT(type_name<decltype(object_tracks[0].speed)>() == "float", "Unexpected data type for object_tracks[0].speed");
   CHECK_TEXT(type_name<decltype(object_tracks[0].predicted_speed)>() == "float", "Unexpected data type for object_tracks[0].predicted_speed");
   CHECK_TEXT(type_name<decltype(object_tracks[0].hdg_ptng_disagmt)>() == "float", "Unexpected data type for object_tracks[0].hdg_ptng_disagmt");
   CHECK_TEXT(type_name<decltype(object_tracks[0].curvature)>() == "float", "Unexpected data type for object_tracks[0].curvature");
   CHECK_TEXT(type_name<decltype(object_tracks[0].tang_accel)>() == "float", "Unexpected data type for object_tracks[0].tang_accel");
   CHECK_TEXT(type_name<decltype(object_tracks[0].predicted_tang_accel)>() == "float", "Unexpected data type for object_tracks[0].predicted_tang_accel");
   CHECK_TEXT(type_name<decltype(object_tracks[0].vcs_position)>() == "f360_variant_A::Point", "Unexpected data type for object_tracks[0].vcs_position");
   CHECK_TEXT(type_name<decltype(object_tracks[0].predicted_vcs_position)>() == "f360_variant_A::Point", "Unexpected data type for object_tracks[0].predicted_vcs_position");
   CHECK_TEXT(type_name<decltype(object_tracks[0].vcs_velocity)>() == "F360_VCS_Velocity_Tag", "Unexpected data type for object_tracks[0].vcs_velocity");
   CHECK_TEXT(type_name<decltype(object_tracks[0].predicted_vcs_velocity)>() == "F360_VCS_Velocity_Tag", "Unexpected data type for object_tracks[0].predicted_vcs_velocity");
   CHECK_TEXT(type_name<decltype(object_tracks[0].vcs_accel)>() == "f360_variant_A::F360_VCS_Accel_Tag", "Unexpected data type for object_tracks[0].vcs_accel");
   CHECK_TEXT(type_name<decltype(object_tracks[0].vcs_heading.Value())>() == "float", "Unexpected data type for object_tracks[0].vcs_heading");
   CHECK_TEXT(type_name<decltype(object_tracks[0].predicted_vcs_heading)>() == "float", "Unexpected data type for object_tracks[0].vcs_heading");
   CHECK_TEXT(type_name<decltype(object_tracks[0].bbox.Get_Orientation().Value())>() == "float", "Unexpected data type for object_tracks[0].Get_Orientation().Value()");
   CHECK_TEXT(type_name<decltype(object_tracks[0].predicted_vcs_pointing)>() == "float", "Unexpected data type for object_tracks[0].predicted_vcs_pointing");
   CHECK_TEXT(type_name<decltype(object_tracks[0].bbox.Get_Length())>() == "float", "Unexpected data type for object_tracks[0].bbox.Get_Length()");
   CHECK_TEXT(type_name<decltype(object_tracks[0].bbox.Get_Width())>() == "float", "Unexpected data type for object_tracks[0].bbox.Get_Width()");
   CHECK_TEXT(type_name<decltype(object_tracks[0].status)>() == "f360_variant_A::F360_Object_Status_Tag", "Unexpected data type for object_tracks[0].status");
   CHECK_TEXT(type_name<decltype(object_tracks[0].occlusion_status.at_vcs_position)>() == "f360_variant_A::Occlusion_Status_T", "Unexpected data type for object_tracks[0].occlusion_status.at_vcs_position");
   CHECK_TEXT(type_name<decltype(object_tracks[0].time_since_cluster_created)>() == "float", "Unexpected data type for object_tracks[0].time_since_cluster_created");
   CHECK_TEXT(type_name<decltype(object_tracks[0].time_since_track_updated)>() == "float", "Unexpected data type for object_tracks[0].time_since_track_updated");
   CHECK_TEXT(type_name<decltype(object_tracks[0].errcov)>() == "float [6][6]", "Unexpected data type for object_tracks[0].errcov");
   CHECK_TEXT(type_name<decltype(object_tracks[0].init_scheme)>() == "f360_variant_A::F360_Track_Init_T", "Unexpected data type for object_tracks[0].init_scheme");
   CHECK_TEXT(type_name<decltype(object_tracks[0].init_vel_source)>() == "f360_variant_A::F360_NEES_CFMI_Vel_Hyp_Source_T", "Unexpected data type for object_tracks[0].init_vel_source");
   CHECK_TEXT(type_name<decltype(object_tracks[0].ndets)>() == "int", "Unexpected data type for object_tracks[0].ndets");
   CHECK_TEXT(type_name<decltype(object_tracks[0].detids)>() == "unsigned int [80]", "Unexpected data type for object_tracks[0].detids");
   CHECK_TEXT(type_name<decltype(object_tracks[0].num_rr_inlier_dets)>() == "int", "Unexpected data type for object_tracks[0].num_rr_inlier_dets");
   CHECK_TEXT(type_name<decltype(object_tracks[0].num_dets_used_in_rr_msmt_update)>() == "unsigned int", "Unexpected data type for object_tracks[0].num_dets_used_in_rr_msmt_update");
   CHECK_TEXT(type_name<decltype(object_tracks[0].f_crossing)>() == "bool", "Unexpected data type for object_tracks[0].f_crossing");
   CHECK_TEXT(type_name<decltype(object_tracks[0].f_moving)>() == "bool", "Unexpected data type for object_tracks[0].f_moving");
   CHECK_TEXT(type_name<decltype(object_tracks[0].movable_prob)>() == "float", "Unexpected data type for object_tracks[0].movable_prob");
   CHECK_TEXT(type_name<decltype(object_tracks[0].f_oncoming)>() == "bool", "Unexpected data type for object_tracks[0].f_oncoming");
   CHECK_TEXT(type_name<decltype(object_tracks[0].f_low_confidence_level)>() == "bool", "Unexpected data type for object_tracks[0].f_low_confidence_level");
   CHECK_TEXT(type_name<decltype(object_tracks[0].f_vehicular_trk)>() == "bool", "Unexpected data type for object_tracks[0].f_vehicular_trk");
   CHECK_TEXT(type_name<decltype(object_tracks[0].f_veh_trk_near_stat_host)>() == "bool", "Unexpected data type for object_tracks[0].f_veh_trk_near_stat_host");
   CHECK_TEXT(type_name<decltype(object_tracks[0].f_valid_for_liberal_tracking)>() == "bool", "Unexpected data type for object_tracks[0].f_valid_for_liberal_tracking");
   CHECK_TEXT(type_name<decltype(object_tracks[0].mirror_prob)>() == "float", "Unexpected data type for object_tracks[0].mirror_prob");
   CHECK_TEXT(type_name<decltype(object_tracks[0].id)>() == "int", "Unexpected data type for object_tracks[0].id");
   CHECK_TEXT(type_name<decltype(object_tracks[0].reduced_id)>() == "int", "Unexpected data type for object_tracks[0].reduced_id");
   CHECK_TEXT(type_name<decltype(object_tracks[0].reduced_status)>() == "f360_variant_A::F360_Object_Status_Tag", "Unexpected data type for object_tracks[0].reduced_status");
   CHECK_TEXT(type_name<decltype(object_tracks[0].cntConsecutiveAmbiguous)>() == "int", "Unexpected data type for object_tracks[0].cntConsecutiveAmbiguous");
   CHECK_TEXT(type_name<decltype(object_tracks[0].cntConsecutiveMoving)>() == "int", "Unexpected data type for object_tracks[0].cntConsecutiveMoving");
   CHECK_TEXT(type_name<decltype(object_tracks[0].raw_confidence_level)>() == "float", "Unexpected data type for object_tracks[0].raw_confidence_level");
   CHECK_TEXT(type_name<decltype(object_tracks[0].confidenceLevel)>() == "float", "Unexpected data type for object_tracks[0].confidenceLevel");
   CHECK_TEXT(type_name<decltype(object_tracks[0].prev_avrg_conf_level)>() == "float", "Unexpected data type for object_tracks[0].prev_avrg_conf_level");
   CHECK_TEXT(type_name<decltype(object_tracks[0].time_since_stage_start)>() == "float", "Unexpected data type for object_tracks[0].time_since_stage_start");
   CHECK_TEXT(type_name<decltype(object_tracks[0].num_types_of_dets)>() == "int [2]", "Unexpected data type for object_tracks[0].num_types_of_dets");
   CHECK_TEXT(type_name<decltype(object_tracks[0].meascov)>() == "float [2][2]", "Unexpected data type for object_tracks[0].meascov");
   CHECK_TEXT(type_name<decltype(object_tracks[0].cnt_error_in_predicted_speed)>() == "int", "Unexpected data type for object_tracks[0].cnt_error_in_predicted_speed");
   CHECK_TEXT(type_name<decltype(object_tracks[0].long_buffer_zone_len1)>() == "float", "Unexpected data type for object_tracks[0].long_buffer_zone_len1");
   CHECK_TEXT(type_name<decltype(object_tracks[0].long_buffer_zone_len2)>() == "float", "Unexpected data type for object_tracks[0].long_buffer_zone_len2");
   CHECK_TEXT(type_name<decltype(object_tracks[0].lat_buffer_zone_wid1)>() == "float", "Unexpected data type for object_tracks[0].lat_buffer_zone_wid1");
   CHECK_TEXT(type_name<decltype(object_tracks[0].lat_buffer_zone_wid2)>() == "float", "Unexpected data type for object_tracks[0].lat_buffer_zone_wid2");
   CHECK_TEXT(type_name<decltype(object_tracks[0].f_fast_moving)>() == "bool", "Unexpected data type for object_tracks[0].f_fast_moving");
   CHECK_TEXT(type_name<decltype(object_tracks[0].time_since_initialization)>() == "float", "Unexpected data type for object_tracks[0].time_since_initialization");
   CHECK_TEXT(type_name<decltype(object_tracks[0].trk_fltr_type)>() == "f360_variant_A::F360_Trk_Fltr_Type_T", "Unexpected data type for object_tracks[0].trk_fltr_type");
   CHECK_TEXT(type_name<decltype(object_tracks[0].reference_point)>() == "f360_variant_A::F360_Reference_Point_T", "Unexpected data type for object_tracks[0].reference_point");
   CHECK_TEXT(type_name<decltype(object_tracks[0].min_projection_reference_point)>() == "f360_variant_A::F360_Reference_Point_T", "Unexpected data type for object_tracks[0].min_projection_reference_point");
   CHECK_TEXT(type_name<decltype(object_tracks[0].time_since_vehicle_init)>() == "float", "Unexpected data type for object_tracks[0].time_since_vehicle_init");
   CHECK_TEXT(type_name<decltype(object_tracks[0].total_reduced_dets)>() == "int", "Unexpected data type for object_tracks[0].total_reduced_dets");
   CHECK_TEXT(type_name<decltype(object_tracks[0].filtered_dets)>() == "float", "Unexpected data type for object_tracks[0].filtered_dets");
   CHECK_TEXT(type_name<decltype(object_tracks[0].f_need_to_hide_trk)>() == "bool", "Unexpected data type for object_tracks[0].f_need_to_hide_trk");
   CHECK_TEXT(type_name<decltype(object_tracks[0].f_ghost_NU_2_C)>() == "bool", "Unexpected data type for object_tracks[0].f_ghost_NU_2_C");
   CHECK_TEXT(type_name<decltype(object_tracks[0].f_overlapping_with_object)>() == "bool", "Unexpected data type for object_tracks[0].f_overlapping_with_object");
   CHECK_TEXT(type_name<decltype(object_tracks[0].time_since_measurement)>() == "float", "Unexpected data type for object_tracks[0].time_since_measurement");
   CHECK_TEXT(type_name<decltype(object_tracks[0].innovation_length)>() == "float", "Unexpected data type for object_tracks[0].innovation_length");
   CHECK_TEXT(type_name<decltype(object_tracks[0].innovation_width)>() == "float", "Unexpected data type for object_tracks[0].innovation_width");
   CHECK_TEXT(type_name<decltype(object_tracks[0].accuracy_length)>() == "float", "Unexpected data type for object_tracks[0].accuracy_length");
   CHECK_TEXT(type_name<decltype(object_tracks[0].accuracy_width)>() == "float", "Unexpected data type for object_tracks[0].accuracy_width");
   CHECK_TEXT(type_name<decltype(object_tracks[0].priority)>() == "float", "Unexpected data type for object_tracks[0].priority");
   CHECK_TEXT(type_name<decltype(object_tracks[0].p_higher_priority_track)>() == "f360_variant_A::F360_Object_Track_T*", "Unexpected data type for object_tracks[0].p_higher_priority_track");
   CHECK_TEXT(type_name<decltype(object_tracks[0].p_lower_priority_track)>() == "f360_variant_A::F360_Object_Track_T*", "Unexpected data type for object_tracks[0].p_lower_priority_track");
   CHECK_TEXT(type_name<decltype(object_tracks[0].linear_moving)>() == "f360_variant_A::F360_Object_Linear_Moving_Type", "Unexpected data type for object_tracks[0].linear_moving");
   CHECK_TEXT(type_name<decltype(object_tracks[0].gain_msmt_cov)>() == "float", "Unexpected data type for object_tracks[0].gain_msmt_cov");
   CHECK_TEXT(type_name<decltype(object_tracks[0].object_class)>() == "f360_variant_A::F360_Object_Class_Tag", "Unexpected data type for object_tracks[0].object_class");
   CHECK_TEXT(type_name<decltype(object_tracks[0].f_used_by_occlusion)>() == "bool", "Unexpected data type for object_tracks[0].f_used_by_occlusion");
   CHECK_TEXT(type_name<decltype(object_tracks[0].vcs_heading.Cos())>() == "float", "Unexpected data type for object_tracks[0].vcs_heading.Cos()");
   CHECK_TEXT(type_name<decltype(object_tracks[0].vcs_heading.Sin())>() == "float", "Unexpected data type for object_tracks[0].vcs_heading.Sin()");
   CHECK_TEXT(type_name<decltype(object_tracks[0].bbox.Get_Orientation().Cos())>() == "float", "Unexpected data type for object_tracks[0].Get_Orientation().Cos()");
   CHECK_TEXT(type_name<decltype(object_tracks[0].bbox.Get_Orientation().Sin())>() == "float", "Unexpected data type for object_tracks[0].Get_Orientation().Sin()");
   CHECK_TEXT(type_name<decltype(object_tracks[0].dead_zone_status)>() == "f360_variant_A::F360_Dead_Zone_Status_T", "Unexpected data type for object_tracks[0].dead_zone_status");
   CHECK_TEXT(type_name<decltype(object_tracks[0].exist_prob)>() == "float", "Unexpected data type for object_tracks[0].exist_prob");
   CHECK_TEXT(type_name<decltype(object_tracks[0].p_track_state)>() == "float", "Unexpected data type for object_tracks[0].p_track_state");
   CHECK_TEXT(type_name<decltype(object_tracks[0].p_det_sensor)>() == "float", "Unexpected data type for object_tracks[0].p_det_sensor");
   CHECK_TEXT(type_name<decltype(object_tracks[0].p_measurement)>() == "float", "Unexpected data type for object_tracks[0].p_measurement");
   CHECK_TEXT(type_name<decltype(object_tracks[0].p_birth)>() == "float", "Unexpected data type for object_tracks[0].p_birth");
   CHECK_TEXT(type_name<decltype(object_tracks[0].p_persist)>() == "float", "Unexpected data type for object_tracks[0].p_persist");
   CHECK_TEXT(type_name<decltype(object_tracks[0].f_track_born)>() == "bool", "Unexpected data type for object_tracks[0].f_track_born");
   CHECK_TEXT(type_name<decltype(object_tracks[0].probability_pedestrian)>() == "float", "Unexpected data type for object_tracks[0].probability_pedestrian");
   CHECK_TEXT(type_name<decltype(object_tracks[0].probability_car)>() == "float", "Unexpected data type for object_tracks[0].probability_car");
   CHECK_TEXT(type_name<decltype(object_tracks[0].probability_bicycle)>() == "float", "Unexpected data type for object_tracks[0].probability_bicycle");
   CHECK_TEXT(type_name<decltype(object_tracks[0].probability_motorcycle)>() == "float", "Unexpected data type for object_tracks[0].probability_motorcycle");
   CHECK_TEXT(type_name<decltype(object_tracks[0].probability_truck)>() == "float", "Unexpected data type for object_tracks[0].probability_truck");
   CHECK_TEXT(type_name<decltype(object_tracks[0].probability_undet)>() == "float", "Unexpected data type for object_tracks[0].probability_undet");
   CHECK_TEXT(type_name<decltype(object_tracks[0].underdrivable_status)>() == "ocg::OCG_Underdrivable_Status_T", "Unexpected data type for object_tracks[0].underdrivable_status");
   CHECK_TEXT(type_name<decltype(object_tracks[0].lsc_prev_in_cluster)>() == "f360_variant_A::F360_Object_Track_T*",   "Unexpected data type for object_tracks[0].lsc_prev_in_cluster");
   CHECK_TEXT(type_name<decltype(object_tracks[0].lsc_next_in_cluster)>() == "f360_variant_A::F360_Object_Track_T*",   "Unexpected data type for object_tracks[0].lsc_next_in_cluster");
   CHECK_TEXT(type_name<decltype(object_tracks[0].behind_sep_id)>() == "unsigned char",   "Unexpected data type for object_tracks[0].behind_sep_id");
   CHECK_TEXT(type_name<decltype(object_tracks[0].on_sep_id)>() == "unsigned char",   "Unexpected data type for object_tracks[0].on_sep_id");
   CHECK_TEXT(type_name<decltype(object_tracks[0].f_behind_sep_ambiguous)>() == "bool", "Unexpected data type for object_tracks[0].f_behind_sep_ambiguous");
   CHECK_TEXT(type_name<decltype(object_tracks[0].sep_intersection_point.x)>() == "float",   "Unexpected data type for object_tracks[0].sep_intersection_point.x");
   CHECK_TEXT(type_name<decltype(object_tracks[0].sep_intersection_point.y)>() == "float",   "Unexpected data type for object_tracks[0].sep_intersection_point.y");
   CHECK_TEXT(type_name<decltype(object_tracks[0].reserved_value_1)>() == "float", "Unexpected data type for object_tracks[0].reserved_value_1");
   CHECK_TEXT(type_name<decltype(object_tracks[0].reserved_value_2)>() == "float", "Unexpected data type for object_tracks[0].reserved_value_2");
   CHECK_TEXT(type_name<decltype(object_tracks[0].reserved_value_3)>() == "float", "Unexpected data type for object_tracks[0].reserved_value_3");
   CHECK_TEXT(type_name<decltype(object_tracks[0].reserved_value_4)>() == "float", "Unexpected data type for object_tracks[0].reserved_value_4");
   CHECK_TEXT(type_name<decltype(object_tracks[0].reserved_value_5)>() == "float", "Unexpected data type for object_tracks[0].reserved_value_5");
   CHECK_TEXT(type_name<decltype(object_tracks[0].conf_longitudinal_position)>() == "f360_variant_A::CONF9_Tag",   "Unexpected data type for object_tracks[0].conf_longitudinal_position");
   CHECK_TEXT(type_name<decltype(object_tracks[0].conf_lateral_position)>() == "f360_variant_A::CONF9_Tag",   "Unexpected data type for object_tracks[0].conf_lateral_position");
   CHECK_TEXT(type_name<decltype(object_tracks[0].conf_longitudinal_velocity)>() == "f360_variant_A::CONF9_Tag",   "Unexpected data type for object_tracks[0].conf_longitudinal_velocity");
   CHECK_TEXT(type_name<decltype(object_tracks[0].conf_lateral_velocity)>() == "f360_variant_A::CONF9_Tag",   "Unexpected data type for object_tracks[0].conf_lateral_velocity");
   CHECK_TEXT(type_name<decltype(object_tracks[0].conf_speed)>() == "f360_variant_A::CONF9_Tag",   "Unexpected data type for object_tracks[0].conf_speed");
   CHECK_TEXT(type_name<decltype(object_tracks[0].conf_overall)>() == "f360_variant_A::CONF3_Tag",   "Unexpected data type for object_tracks[0].conf_overall");
   CHECK_TEXT(type_name<decltype(object_tracks[0].orth_delta_filtered)>() == "float", "Unexpected data type for object_tracks[0].orth_delta_filtered");
   CHECK_TEXT(type_name<decltype(object_tracks[0].orth_gap_filtered)>() == "float", "Unexpected data type for object_tracks[0].orth_gap_filtered");
   CHECK_TEXT(type_name<decltype(object_tracks[0].prev_vcs_center_pos.x)>() == "float",   "Unexpected data type for object_tracks[0].prev_vcs_center_pos.x");
   CHECK_TEXT(type_name<decltype(object_tracks[0].prev_vcs_center_pos.y)>() == "float",   "Unexpected data type for object_tracks[0].prev_vcs_center_pos.y");
   CHECK_TEXT(type_name<decltype(object_tracks[0].filtered_pos_diff_heading)>() == "float",   "Unexpected data type for object_tracks[0].filtered_pos_diff_heading");
   CHECK_TEXT(type_name<decltype(object_tracks[0].average_rcs)>() == "float",   "Unexpected data type for object_tracks[0].average_rcs");
   CHECK_TEXT(type_name<decltype(object_tracks[0].low_rcs_dets_cnt)>() == "unsigned char", "Unexpected data type for object_tracks[0].low_rcs_dets_cnt");
}

/**
**********************************************************************************************************
*                                            Occupancy_Grid_T                                            *
**********************************************************************************************************
*/
/**
 *\purpose
 * Purpose of this test is to verify Occupancy_Grid_T interfaces data types according to requirements.
 *\req
 *  CPR-4430, CPR-4431
 */
TEST(f360_tracker_integration_types_test, PreProcessor_check_verification_of_occupancy_grid_data_types)
{
   /** \precond
    **/
    
   /** \action
    * Call function
    **/
   F360_Tracker_Modules_Call(Up_To_Internal_Preprocessing_Module_call);

   /** \result
    
    **/
    CHECK_TEXT(type_name<decltype(occupancy_grid.timestamp)>() == "double", "Unexpected data type for occupancy_grid.timestamp");
    CHECK_TEXT(type_name<decltype(occupancy_grid.iteration_index)>() == "unsigned int", "Unexpected data type for occupancy_grid.iteration_index");
    CHECK_TEXT(type_name<decltype(occupancy_grid.grid_definition)>() == "ocg::OCG_Definition_T", "Unexpected data type for occupancy_grid.grid_definition");
    CHECK_TEXT(type_name<decltype(occupancy_grid.grid_definition.num_cells_x_far)>() == "unsigned short", "Unexpected data type for occupancy_grid.grid_definition.num_cells_x_far");
    CHECK_TEXT(type_name<decltype(occupancy_grid.grid_definition.num_cells_x_mid)>() == "unsigned short", "Unexpected data type for occupancy_grid.grid_definition.num_cells_x_mid");
    CHECK_TEXT(type_name<decltype(occupancy_grid.grid_definition.num_cells_x_close)>() == "unsigned short", "Unexpected data type for occupancy_grid.grid_definition.num_cells_x_close");
    CHECK_TEXT(type_name<decltype(occupancy_grid.grid_definition.num_cells_y)>() == "unsigned short", "Unexpected data type for occupancy_grid.grid_definition.num_cells_y");
    CHECK_TEXT(type_name<decltype(occupancy_grid.grid_definition.cell_length)>() == "float", "Unexpected data type for occupancy_grid.grid_definition.cell_length");
    CHECK_TEXT(type_name<decltype(occupancy_grid.grid_definition.cell_width)>() == "float", "Unexpected data type for occupancy_grid.grid_definition.cell_width");
    CHECK_TEXT(type_name<decltype(occupancy_grid.grid_definition.cell_width_extension_factor)>() == "float", "Unexpected data type for occupancy_grid.grid_definition.cell_width_extension_factor");
    CHECK_TEXT(type_name<decltype(occupancy_grid.underdrivability)>() == "ocg::OCG_Underdrivability_T", "Unexpected data type for occupancy_grid.underdrivaility");
    CHECK_TEXT(type_name<decltype(occupancy_grid.underdrivability.ogcs_host_rear_axle_position)>() == "ocg::OCG_Position_T", "Unexpected data type for occupancy_grid.underdrivaility.ogcs_host_rear_axle_position");
    CHECK_TEXT(type_name<decltype(occupancy_grid.underdrivability.ogcs_host_rear_axle_position.x)>() == "float", "Unexpected data type for occupancy_grid.underdrivaility.ogcs_host_rear_axle_position.x");
    CHECK_TEXT(type_name<decltype(occupancy_grid.underdrivability.ogcs_host_rear_axle_position.y)>() == "float", "Unexpected data type for occupancy_grid.underdrivaility.ogcs_host_rear_axle_position.y");
    CHECK_TEXT(type_name<decltype(occupancy_grid.underdrivability.ogcs_host_rear_axle_position.z)>() == "float", "Unexpected data type for occupancy_grid.underdrivaility.ogcs_host_rear_axle_position.z");
    CHECK_TEXT(type_name<decltype(occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw)>() == "float", "Unexpected data type for occupancy_grid.underdrivaility.ogcs_host_rear_axle_position.yaw");
    CHECK_TEXT(type_name<decltype(occupancy_grid.underdrivability.underdrivability_status)>() == "ocg::OCG_Underdrivable_Status_T [50][1]", "Unexpected data type for occupancy_grid.underdrivaility.underdrivability_status");
    CHECK_TEXT(type_name<decltype(occupancy_grid.underdrivability.grid_curvature)>() == "float", "Unexpected data type for occupancy_grid.underdrivaility.grid_curvature"); 
}


/** @}*/
