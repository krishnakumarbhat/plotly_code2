/** \file
   This file contains UT for functions defined in f360_update_detection_property.cpp
*/
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

#include "f360_clear_detections_props.h"
#include "f360_vcs_long_sorted_dets_support_functions.h"
#include "f360_update_detection_property.h"
#include "rspp_detection_list.h"
#include "f360_constants.h"
#include "f360_internal_preprocessing.h"
#include "rspp_look_type.h"
#include "rspp_range_type.h"


/** \defgroup  f360_update_detection_property
 *  @{
 */
using namespace f360_variant_A;
/** \brief
*  Check if dedtections with bistatic are correclty handled. \n
*  Test f360_update_detection_property file by setting up Sensor property and check the detection property.
**/

void Set_Default_Flags_Raw_Detections(
      rspp_variant_A::RSPP_Detection_List_T &raw_detect_list)
   {
      for (uint32_t det_idx = 0U; det_idx < raw_detect_list.number_of_valid_detections; det_idx++)
      {
         rspp_variant_A::RSPP_Detection_T& det = raw_detect_list.detections[det_idx];

         det.processed.f_ok_to_use = true;
         det.processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_INVALID;
      }
   }

TEST_GROUP(f360_update_detection_property)
{
   F360_Calibrations_T calibrations = {};
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
   float32_t elapsed_time_s = 0.0F;
   F360_Host_T host = {};
   F360_Host_Props_T host_props = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Radar_Sensor_Props_T sensor_props[MAX_NUMBER_OF_SENSORS] = {};
   F360_Tracker_Info_T tracker_info = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   Static_Env_Poly_T static_env_polys[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   int sensor_index=0;
   float32_t threshold = 0.001F;
   F360_Trailer_Estimator_Output_T trailer_output = {};

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);

      sensors[sensor_index].constant.mounting_position.vcs_position.lateral = 0.118685171;
      sensors[sensor_index].constant.mounting_position.vcs_position.longitudinal = -0.0599999987;
      sensors[sensor_index].variable.look_id = F360_DET_LOOK_ID_2;
      sensors[sensor_index].constant.v_wrapping[sensors[sensor_index].variable.look_id] = 35.5742188;
      sensors[sensor_index].refined.time_since_measurement_s = 0.140803993;
      sensors[sensor_index].constant.mounting_position.vcs_boresight_azimuth_angle=0.5;

      raw_detection_list.number_of_valid_detections = MAX_NUMBER_OF_DETECTIONS;
      Set_Default_Flags_Raw_Detections(raw_detection_list);
      raw_detection_list.number_of_valid_detections = 5;

      raw_detection_list.detections[0].raw.range = 5.78000021;
      raw_detection_list.detections[0].raw.range_rate = 0.734375;
      raw_detection_list.detections[0].raw.azimuth = -0.515454471;


      for (int i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
      {
         raw_detection_list.detections[i].processed.f_ok_to_use = true;
         raw_detection_list.detections[i].raw.sensor_id = sensor_index + 1;
      }
   }
};

/**
*\purpose Test Update_Detection_Property function with 0 detection
*\req NA
*/
TEST(f360_update_detection_property, Test_Updated_detection_no_detection)
{
   /** \precond
    * Test Update_Detection_Property with valid data and number_of_valid_detections is 0 all detections should be cleared
    **/
   raw_detection_list.number_of_valid_detections=0;

   /** \action
    * Call Update_Detection_Property
    **/
   Update_Detection_Property(sensors, raw_detection_list, host, calibrations, static_env_polys, tracker_info, sensor_props, det_props, timing_info);
   /** \result
    * Check the expected value for global_id, cos_vcs_az, vcs_az, sin_vcs_az, range_rate_dealiased, rngrate_interval_width, time_since_measurement, look_type and range_type
    **/

   for (auto det : raw_detection_list.detections)
   {
      DOUBLES_EQUAL(det.raw.det_id,0,threshold)
      DOUBLES_EQUAL(det.processed.cos_vcs_az,0,threshold)
      DOUBLES_EQUAL(det.processed.vcs_az,0,threshold)
      DOUBLES_EQUAL(det.processed.sin_vcs_az,0,threshold)
      DOUBLES_EQUAL(det.processed.vcs_position_y,0.0,threshold)
      DOUBLES_EQUAL(det.processed.vcs_position_x,0.0,threshold)
   }
}

/**
*\purpose Test Update_Detection_Property function and check on the position of object
*\req NA
*/
TEST(f360_update_detection_property, Test_Updated_detection_position_with_data)
{
   /** \precond
    * Test Update_Detection_Property with valid data
    **/
   raw_detection_list.number_of_valid_detections=1;
   Clear_Detections_Props(det_props);
   /** \action
    * Call Update_Detection_Property
    **/
   for (unsigned int i = 0; i < raw_detection_list.number_of_valid_detections; i++)
   {
      const int32_t sensor_idx = raw_detection_list.detections[i].raw.sensor_id - 1;
      rspp_variant_A::RSPP_Detection_T& detection = raw_detection_list.detections[i];
      const F360_Radar_Sensor_T& sensor = sensors[sensor_idx];
      detection.processed.vcs_az = sensor.constant.mounting_position.vcs_boresight_azimuth_angle + detection.raw.azimuth;
      detection.processed.cos_vcs_az = F360_Cosf(detection.processed.vcs_az);
      detection.processed.sin_vcs_az = F360_Sinf(detection.processed.vcs_az);
      detection.processed.vcs_position_x = sensor.constant.mounting_position.vcs_position.longitudinal + (detection.raw.range * detection.processed.cos_vcs_az);
      detection.processed.vcs_position_y = sensor.constant.mounting_position.vcs_position.lateral + (detection.raw.range * detection.processed.sin_vcs_az);
   }
   Copy_Detections_Info(raw_detection_list, sensors, det_props);
   Update_Detection_Property(sensors, raw_detection_list, host, calibrations, static_env_polys, tracker_info, sensor_props, det_props, timing_info);
   /** \result
    * Check the expected value for cos_vcs_az, vcs_az, sin_vcs_az, vcs_position.y, vcs_position.x and position_cov_nees.
    **/
   DOUBLES_EQUAL(det_props[0].vcs_position.y,0.0293619,threshold)
   DOUBLES_EQUAL(det_props[0].vcs_position.x,5.71931,threshold)
   DOUBLES_EQUAL(raw_detection_list.detections[0].processed.cos_vcs_az ,0.999881 ,threshold)
   DOUBLES_EQUAL(raw_detection_list.detections[0].processed.sin_vcs_az ,-0.0154539 ,threshold)
   DOUBLES_EQUAL(raw_detection_list.detections[0].processed.vcs_az     ,-0.0154545 ,threshold)
}

/**
*\purpose  Check if tracker mark det as not ok to use if it is bistatic
*\req    NA
*/
TEST(f360_update_detection_property, Not_use_det_if_bistatic)
{
   /** \precond
   Set det with idx = 4 as bistatic
   **/
   for (unsigned int i = 0; i < raw_detection_list.number_of_valid_detections; i++)
   {
      raw_detection_list.detections[i].raw.sensor_id = 1;
      raw_detection_list.detections[i].raw.azimuth = 1;
      raw_detection_list.detections[i].raw.range = 6;
      const int32_t sensor_idx = raw_detection_list.detections[i].raw.sensor_id - 1;
      rspp_variant_A::RSPP_Detection_T& detection = raw_detection_list.detections[i];
      const F360_Radar_Sensor_T& sensor = sensors[sensor_idx];
      detection.processed.vcs_az = sensor.constant.mounting_position.vcs_boresight_azimuth_angle + detection.raw.azimuth;
      detection.processed.cos_vcs_az = F360_Cosf(detection.processed.vcs_az);
      detection.processed.sin_vcs_az = F360_Sinf(detection.processed.vcs_az);
      detection.processed.vcs_position_x = sensor.constant.mounting_position.vcs_position.longitudinal + (detection.raw.range * detection.processed.cos_vcs_az);
      detection.processed.vcs_position_y = sensor.constant.mounting_position.vcs_position.lateral + (detection.raw.range * detection.processed.sin_vcs_az);
   }

   raw_detection_list.detections[4].raw.f_bistatic = true;
   Clear_Detections_Props(det_props);
   Copy_Detections_Info(raw_detection_list, sensors, det_props);

   /** \action
   * Call Update_Detection_Property
   **/
   Update_Detection_Property(sensors, raw_detection_list, host, calibrations, static_env_polys, tracker_info, sensor_props, det_props, timing_info);

   /** \result
   * Only det with idx = 4 should be not ok to use
   **/
   for (int i = 0; i < 4; i++)
   {
      CHECK_TRUE(det_props[i].f_ok_to_use);
   }

   CHECK_FALSE(det_props[4].f_ok_to_use);

   for (unsigned int i = 5; i < raw_detection_list.number_of_valid_detections; i++)
   {
      CHECK_TRUE(det_props[i].f_ok_to_use);
   }
}

/**
*\purpose  Check if tracker mark det as ok to use if there is no bistatic det
*\req    NA
*/
TEST(f360_update_detection_property, All_det_are_ok_to_use_if_there_is_no_bistatic)
{

   Clear_Detections_Props(det_props);
   /** \action
   * Call Update_Detection_Property
   **/

   Update_Detection_Property(sensors, raw_detection_list, host, calibrations, static_env_polys, tracker_info, sensor_props, det_props, timing_info);

   /** \result
   * All det are ok to use
   **/
   for (int i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      CHECK_TRUE(det_props[i].f_ok_to_use);
   }
}

/**
*\purpose  Check if tracker mark det as not ok to use if all are bistatic &  are not in bistatic FP area
*\req    NA
*/
TEST(f360_update_detection_property, All_det_are_not_ok_to_use_if_all_are_bistatic)
{
   /** \precond
   Set all det as bistatic
   **/


   for (unsigned int i = 0; i < raw_detection_list.number_of_valid_detections; i++)
   {
      raw_detection_list.detections[i].raw.sensor_id = 1;
      raw_detection_list.detections[i].raw.f_bistatic = true;
      raw_detection_list.detections[i].raw.azimuth = 1;
      raw_detection_list.detections[i].raw.range = 6;
   }

   /** \action
   * Call Update_Detection_Property
   **/


   Update_Detection_Property(sensors, raw_detection_list, host, calibrations, static_env_polys, tracker_info, sensor_props, det_props, timing_info);

   /** \result
   * All dets are not ok to use
   **/
   for (unsigned int i = 0; i < raw_detection_list.number_of_valid_detections; i++)
   {
      CHECK_FALSE(det_props[i].f_ok_to_use);
   }
}

/** @}*/

/** \defgroup  f360_update_detection_property_low_quality_detection_filter
 *  @{
 */

/** \brief
*  Tests using this test group will verify that the filter that sets low quality detections that are on or behind guardrail as not ok to use works as expected.
**/
TEST_GROUP(f360_update_detection_property_low_quality_detection_filter)
{
   F360_Calibrations_T calibrations = {};
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
   float32_t elapsed_time_s = 0.0F;
   F360_Host_T host = {};
   F360_Host_Props_T host_props = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Radar_Sensor_Props_T sensor_props[MAX_NUMBER_OF_SENSORS] = {};
   F360_Tracker_Info_T tracker_info = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   Static_Env_Poly_T static_env_polys[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   F360_Trailer_Estimator_Output_T trailer_output = {};

   /** \setup
    * Initialize tracker calibrations
    * Create a guardrail with lateral position 5 that spans longitudinally from -10 to 10
    * Set number of valid detections to 2
    * Set sensor id for the two detections to 1
    * Set range of the two detections to 20m
    * Set azimuth for both detections to 90 degrees (sensor boresight is 0 degress, detection position will be vcs x,y = 0, 20)
    * Set azimuth confidence of first detection to 2
    * Set rcs of second detection to -9
    * Set azimuth confidence of second detection to 3
    * Set rcs of second detection to -11
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);

      static_env_polys[0U].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      static_env_polys[0U].p0 = 5.0F;
      static_env_polys[0U].lower_limit = -10.0F;
      static_env_polys[0U].upper_limit = 10.0F;

      raw_detection_list.number_of_valid_detections = 2U;
      raw_detection_list.detections[0U].raw.sensor_id = 1;
      raw_detection_list.detections[0U].processed.f_ok_to_use = true;
      raw_detection_list.detections[1U].raw.sensor_id = 1;
      raw_detection_list.detections[1U].processed.f_ok_to_use = true;

      raw_detection_list.detections[0U].raw.range = 20.0F;
      raw_detection_list.detections[1U].raw.range = 20.0F;
      raw_detection_list.detections[0U].raw.azimuth = F360_DEG2RAD(90.0F);
      raw_detection_list.detections[1U].raw.azimuth = F360_DEG2RAD(90.0F);

      raw_detection_list.detections[0U].raw.confid_azimuth = 2;
      raw_detection_list.detections[0U].raw.rcs = -9.0F;

      raw_detection_list.detections[1U].raw.confid_azimuth = 3;
      raw_detection_list.detections[1U].raw.rcs = -11.0F;

   }
};

/**
*\purpose Verify that a detection behind guardrail is correctly marked as not ok to use when it has poor azimuth confidence and low rcs.
*\req    NA
*/
TEST(f360_update_detection_property_low_quality_detection_filter, Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail)
{
   /** \precond
    * In test group setup the following has been set up:
    * - Tracker calibrations have been initialized
    * - A guardrail with lateral position 5 that spans longitudinally from -10 to 10 has been created
    * - Number of valid detections has been set to 2
    * - Range of the two detections has been set to 20m
    * - Azimuth for both detections has been set to 90 degrees (sensor boresight is 0 degress, detection position will be vcs x,y = 0,20)
    * - Azimuth confidence of first detection has been set to 2
    * - rcs of first detection has been set to -9
    * - Azimuth confidence of second detection has been set to 3
    * - rcs of second detection has been set to -11
    **/
   Clear_Detections_Props(det_props);
   Sort_Detections_Vcs_Long(raw_detection_list);
   Copy_Detections_Info(raw_detection_list, sensors, det_props);
   det_props[0].vcs_position.x = 0;
   det_props[0].vcs_position.y = 20;
   det_props[1].vcs_position.x = 0;
   det_props[1].vcs_position.y = 20;
   /** \action
    * Call Update_Detection_Property
    **/

   Update_Detection_Property(sensors, raw_detection_list, host, calibrations, static_env_polys, tracker_info, sensor_props, det_props, timing_info);

   /** \result
    * Check that the first detection is still ok to use
    * Check that second detection was marked as not ok to use
    **/
   CHECK_TRUE_TEXT(det_props[0U].f_ok_to_use, "The first detection was incorrectly marked as not ok to use.");
   CHECK_FALSE_TEXT(det_props[1U].f_ok_to_use, "The second detection is still marked as ok to use when it should not have been.");
}
/** @}*/
/** \defgroup  f360_update_detection_property_discard_CI_and_IDM_detections
 *  @{
 */

/** \brief
*  Tests using this test group will verify that the CI and IDM detections are handled properly
**/
TEST_GROUP(f360_update_detection_property_discard_CI_and_IDM_detections)
{
   F360_Calibrations_T calibrations = {};
   F360_Cluster_T clusters[NUMBER_OF_CLUSTERS] = {};
   float32_t elapsed_time_s = 0.0F;
   F360_Host_T host = {};
   F360_Host_Props_T host_props = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Radar_Sensor_Props_T sensor_props[MAX_NUMBER_OF_SENSORS] = {};
   F360_Tracker_Info_T tracker_info = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   Static_Env_Poly_T static_env_polys[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   F360_Trailer_Estimator_Output_T trailer_output = {};

   /** \setup
    * Initialize tracker calibrations
    * Set number of valid detections to 4
    * All detections are set such that no property other CI and IDM flag will change their status to NOK to use
    * The first detection is set as f_ci_det = true, f_idm_det = false
    * The second detection is set as f_ci_det = false, f_idm_det = true
    * The thrid detection is set as f_ci_det = true, f_idm_det = true
    * The fourth detection is set as f_ci_det = false, f_idm_det = false
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);

      raw_detection_list.number_of_valid_detections = 4U;

      for (uint32_t i = 0U; i < raw_detection_list.number_of_valid_detections; i++)
      {
         raw_detection_list.detections[i].raw.sensor_id = 1;
         raw_detection_list.detections[i].raw.range = 20.0F;
         raw_detection_list.detections[i].raw.azimuth = F360_DEG2RAD(90.0F);
         raw_detection_list.detections[i].raw.confid_azimuth = 2;
         raw_detection_list.detections[i].raw.rcs = -9.0F;
         raw_detection_list.detections[i].processed.f_ok_to_use = true;
      }

      raw_detection_list.detections[0U].raw.f_ci_det = true;
      raw_detection_list.detections[0U].raw.f_idm_det = false;

      raw_detection_list.detections[1U].raw.f_ci_det = false;
      raw_detection_list.detections[1U].raw.f_idm_det = true;

      raw_detection_list.detections[2U].raw.f_ci_det = true;
      raw_detection_list.detections[2U].raw.f_idm_det = true;

      raw_detection_list.detections[3U].raw.f_ci_det = false;
      raw_detection_list.detections[3U].raw.f_idm_det = false;
   }
};

/**
*\purpose Verify that a detection flagged as ci or idm is set as NOK to use, otherwise the status shoudl remain as OK to use
*\req    NA
*/
TEST(f360_update_detection_property_discard_CI_and_IDM_detections, check_if_ci_and_idm_dets_are_set_as_NOK_to_use)
{
   /** \precond
    * This test uses the setup from the test group
    **/
   Clear_Detections_Props(det_props);
   Sort_Detections_Vcs_Long(raw_detection_list);
   Copy_Detections_Info(raw_detection_list, sensors, det_props);
   /** \action
    * Call Update_Detection_Property
    **/

   Update_Detection_Property(sensors, raw_detection_list, host, calibrations, static_env_polys, tracker_info, sensor_props, det_props, timing_info);

   /** \result
    * Check that the 1st, 2nd, & 3rd detection is set to NOK to use
    * Check that the 4th detection is still ok to use
    **/
   CHECK_FALSE_TEXT(det_props[0U].f_ok_to_use, "The detection is still marked as ok to use when it should not have been.");
   CHECK_FALSE_TEXT(det_props[1U].f_ok_to_use, "The detection is still marked as ok to use when it should not have been.");
   CHECK_FALSE_TEXT(det_props[2U].f_ok_to_use, "The detection is still marked as ok to use when it should not have been.");
   CHECK_TRUE_TEXT(det_props[3U].f_ok_to_use, "The detection was incorrectly marked as not ok to use.");
}
/**
*\purpose Verify that a detection flagged as idm is set as OK to use, if tracker variant is D
*\req    NA
*/
TEST(f360_update_detection_property_discard_CI_and_IDM_detections, check_if_idm_det_is_set_as_OK_to_use_variant_D)
{
   /** \precond
    * This test uses the setup from the test group.
    * Tracker variant is set to D
    **/
   Clear_Detections_Props(det_props);
   Sort_Detections_Vcs_Long(raw_detection_list);
   Copy_Detections_Info(raw_detection_list, sensors, det_props);
   tracker_info.variant.type = F360_VARIANT_TYPE_D;

   /** \action
    * Call Update_Detection_Property
    **/

   Update_Detection_Property(sensors, raw_detection_list, host, calibrations, static_env_polys, tracker_info, sensor_props, det_props, timing_info);

   /** \result
    * CI flagged detections (1st and 3rd) should be set to NOK to use, IDM flagged detection (2nd) should be set to OK to use
    **/
   CHECK_FALSE_TEXT(det_props[0U].f_ok_to_use, "The detection is still marked as ok to use when it should not have been.");
   CHECK_TRUE_TEXT(det_props[1U].f_ok_to_use, "The detection is incorrectly marked as not ok to use - IDM but tracker variant D.");
   CHECK_FALSE_TEXT(det_props[2U].f_ok_to_use, "The detection is still marked as ok to use when it should not have been.");
   CHECK_TRUE_TEXT(det_props[3U].f_ok_to_use, "The detection was incorrectly marked as not ok to use.");
}
/** @}*/

/** \defgroup  f360_update_detection_property_Handle_ND_Target_Detections
 *  @{
 */

/** \brief
*  Tests using this test group will verify that nd_target detections are handled properly
**/
TEST_GROUP(f360_update_detection_property_Handle_ND_Target_Detections)
{
   F360_Calibrations_T calibrations = {};
   F360_Host_T host = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Radar_Sensor_Props_T sensor_props[MAX_NUMBER_OF_SENSORS] = {};
   F360_Tracker_Info_T tracker_info = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   Static_Env_Poly_T static_env_polys[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   /** \setup
    * Initialize tracker calibrations
    * Set number of valid detections to 1
    * Detection is set such that no property other than mtd flag will change it's status to NOK to use
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);

      tracker_info.variant.type = F360_VARIANT_TYPE_K;

      raw_detection_list.number_of_valid_detections = 1U;

      raw_detection_list.detections[0].raw.sensor_id = 1;
      raw_detection_list.detections[0].raw.azimuth = F360_DEG2RAD(90.0F);
      raw_detection_list.detections[0].raw.confid_azimuth = 2;
      raw_detection_list.detections[0].raw.rcs = -9.0F;
      raw_detection_list.detections[0].processed.f_ok_to_use = true;

      raw_detection_list.detections[0].raw.f_nd_target = true;
      raw_detection_list.detections[0].raw.range = 1.0F;
      host.speed = 0.0F;
   }
};

/**
*\purpose Verify that a detection flagged as nd_target is set as OK to use when it has good properties
*\req    NA
*/
TEST(f360_update_detection_property_Handle_ND_Target_Detections, check_if_nd_target_is_set_as_OK_to_use)
{
   /** \precond
    * This test uses the setup from the test group
    **/
   Copy_Detections_Info(raw_detection_list, sensors, det_props);
   /** \action
    * Call Update_Detection_Property
    **/

   Update_Detection_Property(sensors, raw_detection_list, host, calibrations, static_env_polys, tracker_info, sensor_props, det_props, timing_info);

   /** \result
    * Check that the nd_target detection is still ok to use and it has the nd_target flag mapped correctly to detection props structure
    **/
   CHECK_TRUE_TEXT(det_props[0U].f_ok_to_use, "The detection was incorrectly marked as not ok to use.");
   CHECK_TRUE_TEXT(det_props[0U].f_nd_target, "The nd_target signal was correctly mapped to internal structure.");
}

/**
*\purpose Verify that a detection flagged as nd_target is set as OK to use when it used different variants than K
*\req    NA
*/
TEST(f360_update_detection_property_Handle_ND_Target_Detections, check_if_nd_target_is_set_as_OK_to_use_variant_NOT_K)
{
   /** \precond
    * This test uses the setup from the test group
    * Change tracker variant to A
    **/
   Copy_Detections_Info(raw_detection_list, sensors, det_props);
   tracker_info.variant.type = F360_VARIANT_TYPE_A;
   /** \action
    * Call Update_Detection_Property
    **/

   Update_Detection_Property(sensors, raw_detection_list, host, calibrations, static_env_polys, tracker_info, sensor_props, det_props, timing_info);

   /** \result
    * Check that the nd_target detection is still ok to use and it has the nd_target flag NOT mapped to detection props structure
    **/
   CHECK_TRUE_TEXT(det_props[0U].f_ok_to_use, "The detection was incorrectly marked as not ok to use.");
   CHECK_FALSE_TEXT(det_props[0U].f_nd_target, "The nd_target signal was incorrectly mapped to internal structure due to wrong variant.");
}

/**
*\purpose Verify that a detection flagged as nd_target is set as NOT ok to use when it's range is too high
*\req    NA
*/
TEST(f360_update_detection_property_Handle_ND_Target_Detections, check_if_nd_target_is_set_as_NOT_OK_to_use_high_range)
{
   /** \precond
    * This test uses the setup from the test group
    * Change detection range to higher than 2m
    **/
   raw_detection_list.detections[0].raw.range = 2.1F;
   Copy_Detections_Info(raw_detection_list, sensors, det_props);
   /** \action
    * Call Update_Detection_Property
    **/

   Update_Detection_Property(sensors, raw_detection_list, host, calibrations, static_env_polys, tracker_info, sensor_props, det_props, timing_info);

   /** \result
    * Check that the detection is set to NOK to use and it has the nd_target flag mapped correctly to detection props structure
    **/
   CHECK_FALSE_TEXT(det_props[0U].f_ok_to_use, "The detection is still marked as ok to use when it should not have been.");
   CHECK_TRUE_TEXT(det_props[0U].f_nd_target, "The nd_target signal was correctly mapped to internal structure.");
}

/**
*\purpose Verify that a detection flagged as nd_target is set as NOT ok to use when host speed is too high
*\req    NA
*/
TEST(f360_update_detection_property_Handle_ND_Target_Detections, check_if_nd_target_is_set_as_NOT_OK_to_use_high_host_speed)
{
   /** \precond
    * This test uses the setup from the test group
    * Change host speed to higher than 9m/s
    **/
   host.speed = 9.1F;
   Copy_Detections_Info(raw_detection_list, sensors, det_props);
   /** \action
    * Call Update_Detection_Property
    **/

   Update_Detection_Property(sensors, raw_detection_list, host, calibrations, static_env_polys, tracker_info, sensor_props, det_props, timing_info);

   /** \result
    * Check that the detection is set to NOK to use and it has the nd_target flag mapped correctly to detection props structure
    **/
   CHECK_FALSE_TEXT(det_props[0U].f_ok_to_use, "The detection is still marked as ok to use when it should not have been.");
   CHECK_TRUE_TEXT(det_props[0U].f_nd_target, "The nd_target signal was correctly mapped to internal structure.");
}
/** @}*/
