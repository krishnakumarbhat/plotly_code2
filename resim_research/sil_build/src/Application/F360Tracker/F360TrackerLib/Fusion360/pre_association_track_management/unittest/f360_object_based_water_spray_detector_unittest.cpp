/** \file
 * This file contains unit tests for content of f360_object_based_water_spray_detector.cpp file
 */

#include "f360_object_based_water_spray_detector.h"
#include <CppUTest/TestHarness.h>

#include "f360_vcs_long_sorted_dets_support_functions.h"
#include "f360_math.h"

using namespace f360_variant_A;


/** \defgroup  f360_pre_association_track_management
 *  @{
 */
   using namespace f360_variant_A;

/** \brief
*  Add brief description of test group
**/
TEST_GROUP(f360_object_based_water_spray_detector)
{
   float tolerance = 0.000001F;
   F360_Calibrations_T calibs;
   F360_Tracker_Info_T tracker_info = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_det_list = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   const float32_t range_rate_otg = -1.2126812F;
   F360_Object_Track_T &ref_obj = object_tracks[0];
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   /** \setup
   * Initialize calibs
   **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 1;
     
      ref_obj.id = 1;
      ref_obj.speed = calibs.k_ws_min_speed + tolerance;
      ref_obj.vcs_position.x = 1.0F;
      ref_obj.vcs_position.y = 2.0F;
      ref_obj.Set_Bbox_Orientation(Angle{ 0.0F });
      ref_obj.bbox.Set_Length(3.0F);
      ref_obj.bbox.Set_Width(2.0F);
      ref_obj.vcs_velocity.longitudinal = ref_obj.speed;
      ref_obj.vcs_velocity.lateral = 0.0F;

      raw_det_list.number_of_valid_detections = 1;
      raw_det_list.detections[0].raw.det_id = 1;
      raw_det_list.detections[0].raw.sensor_id = 1;
      raw_det_list.detections[0].raw.rcs = calibs.k_ws_max_det_rcs - tolerance;
      raw_det_list.detections[0].raw.confid_azimuth = 1;
      det_props[0].f_ok_to_use = true;
      det_props[0].vcs_position.x = ref_obj.vcs_position.x - 0.5F * ref_obj.bbox.Get_Length() - tolerance;
      det_props[0].vcs_position.y = ref_obj.vcs_position.y - tolerance;
      raw_det_list.detections[0].processed.vcs_az = atan2(det_props[0].vcs_position.y, det_props[0].vcs_position.x);
      raw_det_list.detections[0].processed.cos_vcs_az = F360_Cosf(raw_det_list.detections[0].processed.vcs_az);
      raw_det_list.detections[0].processed.sin_vcs_az = F360_Sinf(raw_det_list.detections[0].processed.vcs_az);

      det_props[0].range_rate_compensated = range_rate_otg * calibs.k_ows_range_rate_max_factor + tolerance;
   }
};

/**
*\purpose
* Check if detection is set as water spray when all conditions are met
*\req    NA
*/
TEST(f360_object_based_water_spray_detector, det_marked_as_water_spray)
{
   /** \precond
    * Same as setup
   **/

   /** \action
   * Call function
   */
   Detect_Water_Spray_From_Objects(tracker_info, calibs, object_tracks, raw_det_list, sensors, det_props);

   /** \result
   * Detection marked as water spray
   */
   CHECK_TRUE(det_props[0].f_water_spray);
}

/**
*\purpose
* Check if detection is NOT set as water spray due to wrong range rate
*\req    NA
*/
TEST(f360_object_based_water_spray_detector, not_det_marked_as_water_spray_due_to_incorrect_rr)
{
   /** \precond
    * Same as setup and detection range rate set to incorrect value
   **/
   det_props[0].range_rate_compensated = range_rate_otg * calibs.k_ows_range_rate_max_factor - tolerance;

   /** \action
   * Call function
   */
   Detect_Water_Spray_From_Objects(tracker_info, calibs, object_tracks, raw_det_list, sensors, det_props);

   /** \result
   * Detection not marked as water spray
   */
   CHECK_FALSE(det_props[0].f_water_spray);
}

/**
*\purpose
* Check if detection is NOT set as water sprawy due to being out of water spray bounding box (zone)
*\req    NA
*/
TEST(f360_object_based_water_spray_detector, not_det_marked_as_water_spray_due_to_not_being_inside_ws_bbox)
{
   /** \precond
    * Same as setup and set object bbox to 45 deg (detection will be out of ws bbox)
   **/
   ref_obj.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(45.0F) });

   /** \action
   * Call function
   */
   Detect_Water_Spray_From_Objects(tracker_info, calibs, object_tracks, raw_det_list, sensors, det_props);

   /** \result
   * Detection not marked as water spray
   */
   CHECK_FALSE(det_props[0].f_water_spray);
}

/**
*\purpose 
* Test calibrations setup
*\req    NA
*/
TEST(f360_object_based_water_spray_detector, calibs_verification)
{
   /** \result
   * Calibs set as expected
   **/
   DOUBLES_EQUAL(-25.0F, calibs.k_ows_min_long_pos, tolerance);
   DOUBLES_EQUAL(50.0F, calibs.k_ows_max_long_pos, tolerance);
   DOUBLES_EQUAL(25.0F, calibs.k_ows_max_lat_pos, tolerance);
   DOUBLES_EQUAL(0.57F, calibs.k_ows_para_box_obj_speed_factor, tolerance);
   DOUBLES_EQUAL(0.1F, calibs.k_ows_ortho_box_obj_speed_factor, tolerance);
   DOUBLES_EQUAL(0.1F, calibs.k_ows_range_rate_min_factor, tolerance);
   DOUBLES_EQUAL(0.7F, calibs.k_ows_range_rate_max_factor, tolerance);
}
/** @}*/
