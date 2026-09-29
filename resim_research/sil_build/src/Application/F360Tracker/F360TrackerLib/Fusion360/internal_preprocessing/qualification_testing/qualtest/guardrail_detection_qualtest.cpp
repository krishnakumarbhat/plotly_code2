/** \file
This file contains basic unit tests for guard rail detection.
*/
#include "f360_math_func.h"


#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include "f360_internal_preprocessing.h"
#include "f360_inputs_preprocessing.h"

using namespace f360_variant_A;

/** \defgroup  guardrail_detection
 *  @{
 **/

 /** \brief
 *
 **/
TEST_GROUP(guardrail_detection)
{
   int sensor_index = 0;

   F360_Globals_T globals = {};
   F360_Calibrations_T calibrations = {};
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
   F360_Detection_Hist_T det_hist = {};
   F360_Host_T host = {};
   F360_Host_Props_T host_props = {};
   F360_Radar_Sensor_Calib_T sensor_calibs[MAX_NUMBER_OF_SENSORS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Radar_Sensor_Props_T sensor_props[MAX_NUMBER_OF_SENSORS] = {};
   F360_Tracker_Info_T tracker_info = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   F360_Detection_Posn_Hist_T detection_posn_hist[MAX_POSN_HIST_SLOTS] = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Core_Info_T core_info = {};

   /** \setup
    * Initialize calibrations
    * Set up a sensor with id 1 with mounting location forward right
    * Set up a static environment polynomial (sep) that represents the guardrail, spanning from -20m to 20m longitudinally with a lateral position of 10m.
   **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);
      
      //Sensor calibrations
      sensor_calibs[sensor_index].f_ant_sens_available = false;
      sensor_calibs[sensor_index].id = 1;
      sensor_calibs[sensor_index].is_valid = true;
      sensor_calibs[sensor_index].v_wrapping[0] = 70.0f;
      sensor_calibs[sensor_index].v_wrapping[1] = 60.0f;
      sensor_calibs[sensor_index].v_wrapping[2] = 70.0f;
      sensor_calibs[sensor_index].v_wrapping[3] = 60.0f;
      sensor_calibs[sensor_index].polarity = 1;
      sensor_calibs[sensor_index].mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
      sensor_calibs[sensor_index].mounting_position.vcs_boresight_azimuth_angle = 1; //[rad]
      sensor_calibs[sensor_index].mounting_position.vcs_position.lateral = 1.;
      sensor_calibs[sensor_index].mounting_position.vcs_position.longitudinal = 2.;
      
      // Static environment polynomial
      sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      sep[0].lower_limit = -20.0F;
      sep[0].upper_limit = 20.0F;
      sep[0].p0 = 10.0F;
   }

   struct vcs_detection {
      float32_t sensor_id;
      float32_t pos_y;
      float32_t pos_x;
      float32_t vel_y;
      float32_t vel_x;
   };


   void create_raw_detection_from_vcs(rspp_variant_A::RSPP_Detection_List_T & raw_detection_list, vcs_detection a_vcs_detection,
      F360_Radar_Sensor_Calib_T sensor_calibs[MAX_NUMBER_OF_SENSORS], int det_index)
   {
      //Convert positions and velocities of detections defined in vcs to sensor coordinates -> azimuth, range, range rate.
      //local coordinates are vcs translated to sensor position without rotating.
      const int sensor_index = a_vcs_detection.sensor_id - 1;
      const float local_sensor_x = a_vcs_detection.pos_x - sensor_calibs[sensor_index].mounting_position.vcs_position.longitudinal;
      const float local_sensor_y = a_vcs_detection.pos_y - sensor_calibs[sensor_index].mounting_position.vcs_position.lateral;
      const float local_azimuth = F360_Atan2f(local_sensor_y, local_sensor_x);

      const float vel = F360_Get_Hypotenuse(a_vcs_detection.vel_y, a_vcs_detection.vel_x);
      const float detection_heading_vcs = F360_Atan2f(a_vcs_detection.vel_y, a_vcs_detection.vel_x);
      const float vel_to_azimuth_angle = detection_heading_vcs - local_azimuth;

      raw_detection_list.detections[det_index].range = F360_Get_Hypotenuse(local_sensor_y, local_sensor_x);
      raw_detection_list.detections[det_index].range_rate = vel * F360_Cosf(vel_to_azimuth_angle);
      raw_detection_list.detections[det_index].sensor_id = a_vcs_detection.sensor_id;
      raw_detection_list.detections[det_index].azimuth = local_azimuth - sensor_calibs[sensor_index].mounting_position.vcs_boresight_azimuth_angle; //Compensation for installation angle;;
   }
};

/**
*\purpose  Verify that the tracker can find and flag detections that are likely to originate from a guardrail.
*\req    CPR-3857
*/
TEST(guardrail_detection, capability)
{
   /** \precond
    * In test group setup: 
    * - calibrations have been initialized.
    * - A sensor has been set up with id 1 with mounting location forward right
    * - A Static Environment Polynomial (SEP) that represents the guardrail has been set up. The sep spans from -20m to 20m longitudinally with a lateral position of 10m.
    * Create a detection that is located on the SEP.
    **/

   //Default guardrail detection
   vcs_detection guardrail_detection;
   guardrail_detection.sensor_id = 1;
   guardrail_detection.pos_y = 10;
   guardrail_detection.pos_x = 10;
   guardrail_detection.vel_y = 0;
   guardrail_detection.vel_x = 0;

   int detection_index = 0;
   raw_detection_list.number_of_valid_detections = 1;
   sensors[sensor_index].number_of_valid_detections = 1;
   create_raw_detection_from_vcs(raw_detection_list, guardrail_detection, sensor_calibs, detection_index);

   Inputs_Preprocessing(core_info, host, sensor_calibs, sensors, calibrations, globals.max_otg_speed, host_props, det_props, sensor_props, raw_detection_list);
   /** \action
   * Call Internal_Preprocessing()
   **/
   Internal_Preprocessing(host, raw_detection_list, sensor_calibs, sensors, calibrations, sep, host_props, globals, det_hist,
      detection_posn_hist, object_tracks, det_props, sensor_props, clusters, tracker_info, timing_info);

   /** \result
   * Detection is likely to originate from guardrail, check that it is flagged as on SEP.
   **/
   CHECK_TRUE(det_props[0].on_sep_id != F360_INVALID_UNSIGNED_ID);

}


/**
*\purpose  Verify that not all detections are marked as on guardrail
*\req    CPR-3857
*/
TEST(guardrail_detection, no_guardrail_sanity_check)
{
   // Create a detection that is not from guardrail
   vcs_detection non_guardrail_detection;
   non_guardrail_detection.sensor_id = 1;
   non_guardrail_detection.pos_y = 70;
   non_guardrail_detection.pos_x = 5;
   non_guardrail_detection.vel_y = 0;
   non_guardrail_detection.vel_x = 0;

   int detection_index = 0;
   raw_detection_list.number_of_valid_detections = 1;
   sensors[sensor_index].number_of_valid_detections = raw_detection_list.number_of_valid_detections;

   create_raw_detection_from_vcs(raw_detection_list, non_guardrail_detection, sensor_calibs, detection_index);

   Inputs_Preprocessing(core_info, host, sensor_calibs, sensors, calibrations, globals.max_otg_speed, host_props, det_props, sensor_props, raw_detection_list);
   /** \action
   * Call Internal_Preprocessing()
   **/
   
   Internal_Preprocessing(host, raw_detection_list, sensor_calibs, sensors, calibrations, sep, host_props, globals, det_hist,
      detection_posn_hist, object_tracks, det_props, sensor_props, clusters, tracker_info, timing_info);

   /** \result
   * Detection should not be marked as guardrail.
   **/
   CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, det_props[0].on_sep_id);
}
/** @}*/
