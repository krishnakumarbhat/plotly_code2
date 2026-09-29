/** \file
 * This file contains unit tests for testing the requirements connected to f360_pre_association_track_management.cpp file
 */
#include "f360_object_based_water_spray_detector.h"
#include "f360_pre_association_track_management.h"
#include "f360_object_based_radar_phenomena.h"
#include "f360_static_env_polys_support_functions.h"
#include "f360_vcs_long_sorted_dets_support_functions.h"
#include "f360_math.h"
#include "f360_set_variant.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: http://confluenceprod1.delphiauto.net:8090/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_object_based_water_spray_detector_qualtest
*  @{
*/

/** \brief
* Test group contains variables and test cases related to flagging of detections as water spray from objects.
* Three objects and five detections are defined in the test group, please see DFD-390 for a detailed description of the setup.
*/
TEST_GROUP(f360_object_based_water_spray_detector_qualtest)
{
   F360_Tracker_Info_T tracker_info = {};
   F360_Calibrations_T calib = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_det_list = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   

   /** \setup
   * Setup an object so that it is a valid object to check for water spray detections
   */
   TEST_SETUP()
   {
      // Initialize calibrations
      Initialize_Tracker_Calibrations(calib);

      // Objects
      object_tracks[0].id = 1;
      object_tracks[0].speed = calib.k_ws_min_speed + 1.0F;
      object_tracks[0].vcs_position.x = calib.k_ows_min_long_pos + 10.0F;
      object_tracks[0].vcs_position.y = -(calib.k_ows_max_lat_pos + 5.0F);
      object_tracks[0].Set_Bbox_Orientation(Angle{ 0.0F });
      object_tracks[0].reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = object_tracks[0].vcs_position;
      object_tracks[0].bbox.Set_Center(center);
      object_tracks[0].bbox.Set_Length(3.0F);
      object_tracks[0].bbox.Set_Width(2.0F);

      object_tracks[1].id = 2;
      object_tracks[1].speed = calib.k_ws_min_speed + 1.0F;
      object_tracks[1].vcs_position.x = 1.0F;
      object_tracks[1].vcs_position.y = 2.0F;
      object_tracks[1].Set_Bbox_Orientation(Angle{ 0.0F });
      object_tracks[1].vcs_velocity.longitudinal = object_tracks[1].speed;
      object_tracks[1].vcs_velocity.lateral = 0.0F;
      object_tracks[1].reference_point = F360_REFERENCE_POINT_CENTER;
      center = object_tracks[1].vcs_position;
      object_tracks[1].bbox.Set_Center(center);
      object_tracks[1].bbox.Set_Length(3.0F);
      object_tracks[1].bbox.Set_Width(2.0F);

      object_tracks[2].id = 3;
      object_tracks[2].speed = calib.k_ws_min_speed - 1.0F;
      object_tracks[2].vcs_position.x = calib.k_ows_min_long_pos + 1.0F;
      object_tracks[2].vcs_position.y = -calib.k_ows_max_lat_pos + 5.0F;
      object_tracks[2].Set_Bbox_Orientation(Angle{ 0.0F });
      object_tracks[2].reference_point = F360_REFERENCE_POINT_CENTER;
      center = object_tracks[2].vcs_position;
      object_tracks[2].bbox.Set_Center(center);
      object_tracks[2].bbox.Set_Length(3.0F);
      object_tracks[2].bbox.Set_Width(2.0F);

      object_tracks[3].id = 4;
      object_tracks[3].speed = calib.k_ws_min_speed + 1.0F;
      object_tracks[3].vcs_position.x = 4.0F;
      object_tracks[3].vcs_position.y = 0.0F;
      object_tracks[3].Set_Bbox_Orientation(Angle{ F360_PI / 4.0F });
      object_tracks[3].reference_point = F360_REFERENCE_POINT_CENTER;
      center = object_tracks[3].vcs_position;
      object_tracks[3].bbox.Set_Center(center);
      object_tracks[3].bbox.Set_Length(0.0F);
      object_tracks[3].bbox.Set_Width(2.0F);

      // Detections
      raw_det_list.number_of_valid_detections = 6U;

      raw_det_list.detections[0].raw.rcs = 0.0F;
      raw_det_list.detections[0].raw.confid_azimuth = 1;
      det_props[0].f_ok_to_use = true;
      det_props[0].vcs_position.x = object_tracks[0].vcs_position.x - 0.5F * object_tracks[0].bbox.Get_Length() - 1.0F;
      det_props[0].vcs_position.y = object_tracks[0].vcs_position.y;

      raw_det_list.detections[1].raw.rcs = calib.k_ws_max_det_rcs - 1.0F;
      raw_det_list.detections[1].raw.confid_azimuth = 1;
      det_props[1].f_ok_to_use = true;
      det_props[1].vcs_position.x = object_tracks[1].vcs_position.x - 10.0F;
      det_props[1].vcs_position.y = object_tracks[1].vcs_position.y;

      raw_det_list.detections[2].raw.rcs = calib.k_ws_max_det_rcs + 1.0F;
      raw_det_list.detections[2].raw.confid_azimuth = 1;
      det_props[2].f_ok_to_use = true;
      det_props[2].vcs_position.x = object_tracks[1].vcs_position.x - 0.5F * object_tracks[0].bbox.Get_Length() - 0.5F;
      det_props[2].vcs_position.y = object_tracks[1].vcs_position.y;

      raw_det_list.detections[3].raw.rcs = calib.k_ws_max_det_rcs - 1.0F;
      raw_det_list.detections[3].raw.confid_azimuth = 1;
      det_props[3].f_ok_to_use = true;
      det_props[3].vcs_position.x = object_tracks[1].vcs_position.x - 0.5F * object_tracks[0].bbox.Get_Length() - 0.1F;
      det_props[3].vcs_position.y = object_tracks[1].vcs_position.y;
      raw_det_list.detections[3].processed.vcs_az = atan2(det_props[3].vcs_position.y, det_props[3].vcs_position.x);
      raw_det_list.detections[3].processed.cos_vcs_az = F360_Cosf(raw_det_list.detections[3].processed.vcs_az);
      raw_det_list.detections[3].processed.sin_vcs_az = F360_Sinf(raw_det_list.detections[3].processed.vcs_az);
      det_props[3].range_rate_compensated = -0.173F;

      raw_det_list.detections[4].raw.rcs = calib.k_ws_max_det_rcs - 1.0F;
      raw_det_list.detections[4].raw.confid_azimuth = 1;
      det_props[4].f_ok_to_use = true;
      det_props[4].vcs_position.x = object_tracks[2].vcs_position.x - 0.5F * object_tracks[0].bbox.Get_Length() - 0.1F;
      det_props[4].vcs_position.y = object_tracks[2].vcs_position.y;

      raw_det_list.detections[5].raw.rcs = calib.k_ws_max_det_rcs - 1.0F;
      raw_det_list.detections[5].raw.confid_azimuth = 1;
      det_props[5].f_ok_to_use = true;
      det_props[5].vcs_position.x = 4.1F;
      det_props[5].vcs_position.y = 0.1F;

      // Initialize detection sorted info (Done in Clear_Detection() function call in production code)
      for (uint32_t det_idx = 0U; det_idx < MAX_NUMBER_OF_DETECTIONS; det_idx++)
      {
         raw_det_list.detections[det_idx].processed.prev_sorted_idx = F360_INVALID_ID;
         raw_det_list.detections[det_idx].processed.next_sorted_idx = F360_INVALID_ID;
      }

      // Tracker info
      tracker_info.num_active_objs = 4;
      tracker_info.active_obj_ids[0] = 1;
      tracker_info.active_obj_ids[1] = 2;
      tracker_info.active_obj_ids[2] = 3;
      tracker_info.active_obj_ids[3] = 4;
      Set_Tracker_Variant(tracker_info.variant);

      Sort_Detections_Vcs_Long(raw_det_list);

   }
};

/** \purpose
* Verify that detections have been flagged as object water spray correctly in scenario defined in test group
* \req CPR-3758
* NA
*/
TEST(f360_object_based_water_spray_detector_qualtest, Detect_Water_Spray_From_Objects)
{
   /** \action
   * Call function
   */
   Detect_Water_Spray_From_Objects(tracker_info, calib, object_tracks, raw_det_list, sensors, det_props);

   /** \result
   * Compare against expected data
   */
   CHECK_FALSE_TEXT(det_props[0].f_water_spray, "Detection inaccurately flagged as object water spray");
   CHECK_TRUE_TEXT(det_props[0].f_ok_to_use, "Detection inaccurately flagged as not ok to use")

   CHECK_FALSE_TEXT(det_props[1].f_water_spray, "Detection inaccurately flagged as object water spray");
   CHECK_TRUE_TEXT(det_props[1].f_ok_to_use, "Detection inaccurately flagged as not ok to use")

   CHECK_FALSE_TEXT(det_props[2].f_water_spray, "Detection inaccurately flagged as object water spray");
   CHECK_TRUE_TEXT(det_props[2].f_ok_to_use, "Detection inaccurately flagged as not ok to use")

   CHECK_TRUE_TEXT(det_props[3].f_water_spray, "Detection not flagged as object water spray as expected");
   CHECK_TRUE_TEXT(det_props[3].f_ok_to_use, "Detection inaccurately flagged as ok to use")

   CHECK_FALSE_TEXT(det_props[4].f_water_spray, "Detection inaccurately flagged as object water spray");
   CHECK_TRUE_TEXT(det_props[4].f_ok_to_use, "Detection inaccurately flagged as not ok to use")

   CHECK_FALSE_TEXT(det_props[5].f_water_spray, "Detection inaccurately flagged as object water spray");
   CHECK_TRUE_TEXT(det_props[5].f_ok_to_use, "Detection inaccurately flagged as not ok to use")
}

/** @}*/

/** \defgroup  f360_object_based_angle_jump_detector_Detect_Angle_Jumps_From_Objects_qualtest
 *  @{
 */

/** \brief
 * Test group for testing Detect_Angle_Jumps_From_Objects()
 */
TEST_GROUP(f360_object_based_angle_jump_detector_Detect_Angle_Jumps_From_Objects_qualtest)
{
   const float32_t tolerance = 0.0001F;
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Calibrations_T calibs = {};
   F360_Tracker_Info_T tracker_info = {};

   /** \setup
   *
   **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      calibs.rp_max_object_lateral_distance = 7.0F;
      calibs.rp_max_abs_pointing_disagreement = F360_DEG2RAD(10.0F);
      calibs.rp_object_max_longitudinal_margin = 2.0F;

      for (int idx = 0; idx < MAX_NUMBER_OF_SENSORS; idx++)
      {
         sensors[idx].variable.is_valid = false;
         sensors[idx].constant.mounting_location = F360_MOUNTING_LOCATION_UNKNOWN;
      }

      // Front right
      sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
      sensors[0].constant.mounting_position.vcs_position.longitudinal = -0.25F;
      sensors[0].constant.mounting_position.vcs_position.lateral = 0.9F;
      sensors[0].variable.is_valid = true;

      // Front left
      sensors[1].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
      sensors[1].constant.mounting_position.vcs_position.longitudinal = -0.25F;
      sensors[1].constant.mounting_position.vcs_position.lateral = -0.9F;
      sensors[1].variable.is_valid = true;

      // Rear right
      sensors[2].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;
      sensors[2].constant.mounting_position.vcs_position.longitudinal = -3.9F;
      sensors[2].constant.mounting_position.vcs_position.lateral = 0.9F;
      sensors[2].variable.is_valid = true;

      // Rear left
      sensors[3].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;
      sensors[3].constant.mounting_position.vcs_position.longitudinal = -3.9F;
      sensors[3].constant.mounting_position.vcs_position.lateral = -0.9F;
      sensors[3].variable.is_valid = true;

      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 1;
      Set_Tracker_Variant(tracker_info.variant);
   }
};

/** \purpose
 * Detect_Angle_Jumps_From_Objects should set correctly f_ok_to_use
 * \req CPR-3759
 * NA
 */

TEST(f360_object_based_angle_jump_detector_Detect_Angle_Jumps_From_Objects_qualtest, flagging_correctness_of_f_ok_to_use)
{
   /** \precond
    * Set input in order to set only one detection
    */
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};

   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].f_moving = true;
   object_tracks[0].vcs_heading = Angle{ 0.0F };
   object_tracks[0].vcs_position.y = 4.0F;
   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].Update_Bbox_Size(6.0F, 3.0F);

   object_tracks[0].vcs_velocity.longitudinal = 10.0F;
   object_tracks[0].vcs_velocity.lateral = 0.0F;
   object_tracks[0].confidenceLevel = calibs.rp_min_confidence_level + tolerance;

   sensors[0].variable.vcs_velocity.longitudinal = 10.0F;
   sensors[0].variable.vcs_velocity.lateral = 0.0F;
   sensors[0].constant.sensor_type = F360_SENSOR_TYPE_SRR5_RADAR;
   sensors[0].variable.vacs_boresight_az_estimated = F360_DEG2RAD(0.0F);
   sensors[0].constant.polarity = 1;

   raw_detection_list.number_of_valid_detections = 2;

   raw_detection_list.detections[0].raw.sensor_id = 1;
   raw_detection_list.detections[0].raw.confid_azimuth = 1;
   raw_detection_list.detections[0].raw.range = 4.0F;
   raw_detection_list.detections[0].raw.range_rate = 14.5F;
   raw_detection_list.detections[0].raw.azimuth = F360_DEG2RAD(60.0F);
   detection_props[0].vcs_position.y = raw_detection_list.detections[0].raw.range * F360_Sinf(raw_detection_list.detections[0].processed.vcs_az);
   detection_props[0].vcs_position.x = raw_detection_list.detections[0].raw.range * F360_Cosf(raw_detection_list.detections[0].processed.vcs_az);
   detection_props[0].f_ok_to_use = true;

   raw_detection_list.detections[1].raw.sensor_id = 1;
   raw_detection_list.detections[1].raw.confid_azimuth = 1;
   raw_detection_list.detections[1].raw.range = 4.0F;
   raw_detection_list.detections[1].raw.range_rate = -0.1F;
   raw_detection_list.detections[1].raw.azimuth = F360_DEG2RAD(10.0F);
   detection_props[1].vcs_position.y = raw_detection_list.detections[1].raw.range * F360_Sinf(raw_detection_list.detections[1].processed.vcs_az);
   detection_props[1].vcs_position.x = raw_detection_list.detections[1].raw.range * F360_Cosf(raw_detection_list.detections[1].processed.vcs_az);
   detection_props[1].f_ok_to_use = true;

   /** \action
    * Call Detect_Angle_Jumps_From_Objects()
    */
   Check_Dets_Against_Radar_Phenomena(tracker_info, object_tracks, sensors, calibs, raw_detection_list, detection_props);

   /** \result
    * Det[1] is not ok to use
    */
   CHECK_TRUE(detection_props[0].f_ok_to_use);
   CHECK_FALSE(detection_props[1].f_ok_to_use);
}
/** @}*/

/** \defgroup  f360_find_tracks_on_and_behind_guardrail_qualtest
 *  @{
 */

 /** \brief
  *  Test group of Find_Tracks_On_And_Behind_Guardrail
  */

TEST_GROUP(f360_find_tracks_on_and_behind_guardrail_qualtest)
{
   F360_Calibrations_T calibrations = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS] = {};

   // Initialize tracker calibrations
   // Create two SEPs spanning from -20 to 20m longitudinally, one on each side of host
   // Create an object located on the SEP on the left side of host
   // Set up tracker info
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);

      sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      sep[0].p0 = -2.0F;
      sep[0].lower_limit = -20.0F;
      sep[0].upper_limit = 20.0F;
      sep[1].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      sep[1].p0 = 4.0F;
      sep[1].lower_limit = -20.0F;
      sep[1].upper_limit = 20.0F;

      object_tracks[0].vcs_position.x = 0.0F;
      object_tracks[0].vcs_position.y = -1.8F;
      object_tracks[0].reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = object_tracks[0].vcs_position;
      object_tracks[0].bbox.Set_Center(center);


      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 1;
      Set_Tracker_Variant(tracker_info.variant);
   }
};

/**
*\purpose  Test that object is correctly marked as on guardrail when object is on SEP on left side
*\req    CPR-3760
*/
TEST(f360_find_tracks_on_and_behind_guardrail_qualtest, Flag_Objects_On_SEP_ObjectOnGuardrailLeftSide)
{
    /** \precond
     * In test group setup:
     * - A SEP has been set up on the left side of host.
     * - An object has been placed on the SEP.
     * - The object has been placed inside tracker info struct.
     **/

   /** \action
   *Call Flag_Objects_On_SEP
   **/
   Flag_Objects_On_SEP(tracker_info, sep, calibrations, object_tracks);

   /** \result
   * Make sure that object is flagged as on SEP with id 1
   **/
   CHECK_EQUAL(1U, object_tracks[0].on_sep_id);
}

/**
*\purpose  Test that object is correctly marked as on guardrail when object is on SEP on right side
*\req    CPR-3760
*/
TEST(f360_find_tracks_on_and_behind_guardrail_qualtest, Flag_Objects_On_SEP_ObjectOnGuardrailRightSide)
{

    /** \precond
     * - A SEP has been set up on the right side of host.
     * - An object has been placed inside tracker info struct.
     * Set object position to be on the right SEP.
     **/

   object_tracks[0].vcs_position.x = 0.0F;
   object_tracks[0].vcs_position.y = 3.5F;
   Point center = object_tracks[0].vcs_position;
   object_tracks[0].bbox.Set_Center(center);

   /** \action
   *Call Flag_Objects_On_SEP
   **/
   Flag_Objects_On_SEP(tracker_info, sep, calibrations, object_tracks);

   /** \result
   * Make sure that object is flagged as on SEP with id 2
   **/
   CHECK_EQUAL(2U, object_tracks[0].on_sep_id);
}
/** @}*/



/** \defgroup  detecting_multibounces_from_object
 *  @{
 */

 /** \brief
  * Test requirements w.r.t. multibounce detections
  */
TEST_GROUP(detecting_multibounces_from_object)
{
   const float tolerance = 0.0001F;

   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS]{};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   bool relevant_sensors[MAX_NUMBER_OF_SENSORS]{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   F360_Calibrations_T calibs{};
   F360_Globals_T globals{};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS]{};
   F360_Tracker_Info_T tracker_info{};
   Static_Env_Poly_T SEP[F360_NUM_OF_STATIC_ENV_POLYS];
   F360_TRKR_TIMING_INFO_T timing_info{};
   F360_Host_T host{};

   F360_Radar_Sensor_T &front_right_sensor = sensors[0];
   rspp_variant_A::RSPP_Detection_T &fr_mb_det_raw = raw_detection_list.detections[0];
   F360_Detection_Props_T &fr_mb_det_prop = detection_props[0];
   rspp_variant_A::RSPP_Detection_T &rr_mb_det_raw = raw_detection_list.detections[1];
   F360_Detection_Props_T &rr_mb_det_prop = detection_props[1];
   F360_Object_Track_T &relevant_object = object_tracks[0];

   /** \setup
    * Setup all needed data like radars, detections and object
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      //Setup radars
      front_right_sensor.constant.mounting_position.vcs_position = { -0.4F, 0.9F, 0.0F };
      front_right_sensor.variable.is_valid = true;
      front_right_sensor.variable.vcs_velocity = { 20.0F, 0.0F };
      front_right_sensor.constant.sensor_type = F360_SENSOR_TYPE_SRR5_RADAR;
      globals.average_sensor_position_x = -0.4F;

      relevant_sensors[0] = true;
      relevant_sensors[1] = true;
      relevant_sensors[2] = true;
      relevant_sensors[3] = true;

      //Setup object
      relevant_object.vcs_position = { -2.0F, 5.0F };
      relevant_object.vcs_velocity = { 22.0F, 0.0F };    // moves faster than host/sensors
      relevant_object.id = 1;
      relevant_object.f_moving = true;
      relevant_object.status = F360_OBJECT_STATUS_UPDATED;
      relevant_object.confidenceLevel = calibs.rp_min_confidence_level + tolerance;
      relevant_object.reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = relevant_object.vcs_position;
      relevant_object.bbox.Set_Center(center);
      relevant_object.bbox.Set_Length(10.0F);
      relevant_object.bbox.Set_Width(4.0F);

      tracker_info.active_obj_ids[0] = 1;
      tracker_info.num_active_objs = 1;
      Set_Tracker_Variant(tracker_info.variant);

      //Setup multibounce detection
      raw_detection_list.number_of_valid_detections = 1U;

      fr_mb_det_raw.raw.sensor_id = 1;  // belongs to front right sensor
      fr_mb_det_raw.raw.range = 7.87F;
      fr_mb_det_raw.raw.range_rate = 0.863F * 3.0F;
      fr_mb_det_raw.processed.sin_vcs_az = std::sin(F360_DEG2RAD(64.411F));
      fr_mb_det_raw.processed.cos_vcs_az = std::cos(F360_DEG2RAD(64.411F));
      fr_mb_det_prop.vcs_position = { 3.0F, 8.0F };
      fr_mb_det_prop.f_ok_to_use = true;

      host.speed = F360_KPH2MPS(11.0F);
   }
};

/** \purpose
 * Check if detection is marked as multibounces if all conditions are met.
 * \req CPR-3761
 */
TEST(detecting_multibounces_from_object, det_marked_as_multibounce)
{
   /** \precond
    * Same as setup
    */

   /** \action
   * Call Pre_Association_Track_Management().
   */
   Pre_Association_Track_Management(tracker_info, object_tracks, raw_detection_list, sensors, host, calibs, globals, SEP, detection_props, timing_info);

   /** \result
    * Detection is marked as multi bounce and should be not ok to use
    */
   CHECK_TRUE(fr_mb_det_prop.f_double_bounce);
   CHECK_FALSE(fr_mb_det_prop.f_ok_to_use);
}

/** \purpose
 * Check if detection is NOT marked as multibounces if one condition is not met.
 * \req CPR-3761
 */
TEST(detecting_multibounces_from_object, det_NOT_marked_as_multibounce)
{
   /** \precond
    * Same as setup with below changes.
    * Set detection range to high value
    */
   fr_mb_det_raw.raw.range = 50.0F;

   /** \action
   * Call Pre_Association_Track_Management().
   */
   Pre_Association_Track_Management(tracker_info, object_tracks, raw_detection_list, sensors, host, calibs, globals, SEP, detection_props, timing_info);

   /** \result
    * Detection is not marked as multi bounce and should ok to use
    */
   CHECK_FALSE(fr_mb_det_prop.f_double_bounce);
   CHECK_TRUE(fr_mb_det_prop.f_ok_to_use);
}
/** @}*/


/** \defgroup  object_wrt_to_sep_tests
 *  @{
 */

 /** \brief
  *  Test requirements w.r.t. object relation to a SEP
  */
TEST_GROUP(object_wrt_to_sep_tests)
{
   const float tolerance = 1e-3F;
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS]{};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   bool relevant_sensors[MAX_NUMBER_OF_SENSORS]{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   F360_Calibrations_T calibs{};
   F360_Globals_T globals{};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS]{};
   F360_Tracker_Info_T tracker_info{};
   Static_Env_Poly_T SEP[F360_NUM_OF_STATIC_ENV_POLYS];
   F360_TRKR_TIMING_INFO_T timing_info{};
   F360_Host_T host{};

   F360_Object_Track_T &relevant_object = object_tracks[0];
   Static_Env_Poly_T &relevant_sep = SEP[0];

   /** \setup
   * Set up required variables. 
   * Scenario: One object that lies on a SEP
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 1;
      Set_Tracker_Variant(tracker_info.variant);

      relevant_object.id = 1;
      relevant_object.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      relevant_object.on_sep_id = F360_INVALID_UNSIGNED_ID;
      relevant_object.vcs_position = { -2.0F, 15.0F };
      relevant_object.reference_point = F360_REFERENCE_POINT_CENTER;
      relevant_object.Set_Bbox_Orientation(Angle{ 0.0F });
      relevant_object.Update_Bbox_Size(5.0F, 2.0F);

      relevant_object.behind_sep_id = F360_INVALID_UNSIGNED_ID;

      relevant_sep.status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      relevant_sep.p0 = relevant_object.vcs_position.y;
      relevant_sep.upper_limit = 30.0F;
      relevant_sep.lower_limit = -30.0F;
   }
};

/**
*\purpose   Check if object is correctly marked on a SEP
*\req       CPR-3762
*/
TEST(object_wrt_to_sep_tests, obejct_marked_as_on_a_sep)
{
   /** \precond
   * Same as setup
   */

   /** \action
   * Call Pre_Association_Track_Management().
   */
   Pre_Association_Track_Management(tracker_info, object_tracks, raw_detection_list, sensors, host, calibs, globals, SEP, detection_props, timing_info);

   /** \result
   * Object is on SEP (sep id = 1)
   */
   CHECK_EQUAL(relevant_object.on_sep_id, 1);
}

/**
*\purpose   Check if object is correctly NOT marked on a SEP
*\req       CPR-3762
*/
TEST(object_wrt_to_sep_tests, obejct_NOT_marked_as_on_sep)
{
   /** \precond
   * Same as setup with below changes
   * move sep behind the object laterally
   */
   relevant_sep.p0 = 20.0F;

   /** \action
   * Call Pre_Association_Track_Management().
   */
   Pre_Association_Track_Management(tracker_info, object_tracks, raw_detection_list, sensors, host, calibs, globals, SEP, detection_props, timing_info);

   /** \result
   * Object is on SEP (sep id = F360_INVALID_UNSIGNED_ID)
   */
   CHECK_EQUAL(relevant_object.on_sep_id, F360_INVALID_UNSIGNED_ID);
}
/** @}*/
