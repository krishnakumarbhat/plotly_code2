/** \file
   Give a detailed description of what  this unit-test file contain.
*/

#include "f360_pre_association_track_management.h"
#include "f360_static_env_polys_support_functions.h"
#include "f360_set_variant.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

/** \defgroup  f360_pre_association_track_management
 *  @{
 */
using namespace f360_variant_A;
/** \brief
*  Add brief description of test group
**/
TEST_GROUP(f360_pre_association_track_management)
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
      relevant_object.movable_prob = 1.0F;
      relevant_object.on_sep_id = F360_INVALID_UNSIGNED_ID;
      relevant_object.vcs_position = { -2.0F, 15.0F };
      relevant_object.bbox.Set_Length(5.0F);
      relevant_object.bbox.Set_Width(2.0F);
      relevant_object.Set_Bbox_Orientation(Angle{ 0.0F });
      relevant_object.behind_sep_id = F360_INVALID_UNSIGNED_ID;
      relevant_object.reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = relevant_object.vcs_position;
      relevant_object.bbox.Set_Center(center);
      relevant_object.bbox.Set_Length(5.0F);
      relevant_object.bbox.Set_Width(2.0F);

      relevant_sep.status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      relevant_sep.p0 = relevant_object.vcs_position.y;
      relevant_sep.upper_limit = 30.0F;
      relevant_sep.lower_limit = -30.0F;
   }
};

/**
*\purpose   Check if sep_intersection_point is correctly calculated
*\req       NA
*/
TEST(f360_pre_association_track_management, correct_position_of_intersection_point)
{
   /** \precond
   * Same as setup with below changes:
   * move the SEP laterally in front of the SEP (fully)
   */
   // TODO: DFT-1734 -> to be fixed together with bug fix
   relevant_sep.p0 = 10.0F;

   /** \action
   * Call Pre_Association_Track_Management().
   */
   Pre_Association_Track_Management(tracker_info, object_tracks, raw_detection_list, sensors, host, calibs, globals, SEP, detection_props, timing_info);

   /** \result
   * Intersection point x = -2.0, y = 10.0F
   */
   DOUBLES_EQUAL(-1.3333F, relevant_object.sep_intersection_point.x, tolerance);
   DOUBLES_EQUAL(10.0F, relevant_object.sep_intersection_point.y, tolerance);
}

/**
*\purpose   Check if object is NOT marked as f_behind_sep_ambiguous
*\req       NA
*/
TEST(f360_pre_association_track_management, object_NOT_marked_as_behind_sep_ambiguously)
{
   /** \precond
   * Same as setup
   */

   /** \action
   * Call Pre_Association_Track_Management().
   */
   Pre_Association_Track_Management(tracker_info, object_tracks, raw_detection_list, sensors, host, calibs, globals, SEP, detection_props, timing_info);

   /** \result
   * Object NOT marked as behind sep ambiguously
   */
   CHECK_FALSE(relevant_object.f_behind_sep_ambiguous);
}

/**
*\purpose   Check if object is correctly marked as f_behind_sep_ambiguous
*\req       NA
*/
TEST(f360_pre_association_track_management, object_marked_as_behind_sep_ambiguously)
{
   /** \precond
   * Same as setup with below changes
   * move sep upper limit to object centroid
   */
   relevant_sep.upper_limit = relevant_object.vcs_position.x;

   /** \action
   * Call Pre_Association_Track_Management().
   */
   Pre_Association_Track_Management(tracker_info, object_tracks, raw_detection_list, sensors, host, calibs, globals, SEP, detection_props, timing_info);

   /** \result
   * Object marked as behind sep ambiguously
   */
   CHECK_TRUE(relevant_object.f_behind_sep_ambiguous);
}
/** @}*/


/** \defgroup  f360_flag_single_object_behind_sep
 *  @{
 *
 */
 /** \brief
  * Test group for testing that flagging of objects behind SEPs work as expected.
  */
TEST_GROUP(f360_flag_single_object_behind_sep)
{
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_Calibrations_T calibs = {};
   F360_Object_Track_T obj = {};
   F360_Globals_T globals = {};

   /** \setup
   * Initialize tracker calibrations
   * Create a straight SEP 5m laterally from VCS origin between longitudinal position x = (-10, 15)
   * Set object movable_prob to 1.0
   * Set object on SEP id to F360_INVALID_UNSIGNED_ID
   * Set object vcs position (x,y) = (5, 6)
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      sep[0].p2 = 0.0F;
      sep[0].p1 = 0.0F;
      sep[0].p0 = 5.0F;
      sep[0].lower_limit = -10.0F;
      sep[0].upper_limit = 15.0F;

      obj.movable_prob = 1.0F;
      obj.f_behind_sep_ambiguous = false;
      obj.on_sep_id = F360_INVALID_UNSIGNED_ID;
      obj.vcs_position.x = 5.0F;
      obj.vcs_position.y = 6.0F;
      obj.reference_point = F360_REFERENCE_POINT_CENTER;
      obj.Set_Bbox_Orientation(Angle{ 0.0F });
      obj.bbox.Set_Length(4.0F);
      obj.Update_Bbox_Center();
   }
};

/** \purpose
 * Verify that function correctly marks a moveable object as behind SEP when there is only one valid SEP and object is well behind the SEP.
 *\req    NA
 * NA.
 */
TEST(f360_flag_single_object_behind_sep, Flag_Single_Object_Behind_SEP_Moveable_Obj_Is_Well_Behind_SEP)
{
   /** \precond
    * The following has been set up in test group:
    * - Tracker calibrations have been initialized.
    * - A straight SEP has been created between x = (-10, 15), y = 5
    * - A moveable object has been created and its vcs position has been set to (5, 6) such that it is behind SEP
    * - Object on SEP id has been set to F360_INVALID_UNSIGNED_ID
    */

    /** \action
     * Call Flag_Single_Object_Behind_SEP
     */
   Flag_Single_Object_Behind_SEP(sep, calibs, globals, obj);

   /** \result
    * Check that obj was marked as behind SEP with id 1
    * Check that object behind SEP ambiguous flag is false
    */
   CHECK_EQUAL_TEXT(1U, obj.behind_sep_id, "Object was not correctly marked as behind SEP with ID 1.");
   CHECK_FALSE_TEXT(obj.f_behind_sep_ambiguous, "Object was marked as f_behind_sep_ambiguous when there is no ambiguity.");
}

/** \purpose
 * Verify that function does not mark a moveable object as behind SEP when the object is clearly in front of SEP.
 *\req    NA
 * NA.
 */
TEST(f360_flag_single_object_behind_sep, Flag_Single_Object_Behind_SEP_Moveable_Obj_Is_Not_Behind_SEP)
{
   /** \precond
    * The following has been set up in test group:
    * - Tracker calibrations have been initialized.
    * - A straight SEP has been created between x = (-10, 15), y = 5
    * - A moveable object has been created
    * - Object on SEP id has been set to F360_INVALID_UNSIGNED_ID
    * Set object vcs position to (40, 6)
    */
   obj.vcs_position.x = 40.0F;
   obj.vcs_position.y = 6.0F;
   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = obj.vcs_position;
   obj.bbox.Set_Center(center);

   /** \action
    * Call Flag_Single_Object_Behind_SEP
    */
   Flag_Single_Object_Behind_SEP(sep, calibs, globals, obj);

   /** \result
    * Check that obj behind SEP id is F360_INVALID_UNSIGNED_ID
    * Check that object behind SEP ambiguous flag is false
    */
   CHECK_EQUAL_TEXT(F360_INVALID_UNSIGNED_ID, obj.behind_sep_id, "Object was marked as behind an SEP when it should not have been.");
   CHECK_FALSE_TEXT(obj.f_behind_sep_ambiguous, "Object was marked as f_behind_sep_ambiguous when there is no ambiguity.");
}

/** \purpose
 * Verify that function does not mark a nonmoveable object as behind SEP even though it is well behind the SEP.
 *\req    NA
 * NA.
 */
TEST(f360_flag_single_object_behind_sep, Flag_Single_Object_Behind_SEP_Nonmoveable_Obj_Is_Well_Behind_SEP)
{
   /** \precond
    * The following has been set up in test group:
    * - Tracker calibrations have been initialized.
    * - A straight SEP has been created between x = (-10, 15), y = 5
    * - A object has been created and its vcs position has been set to (5, 6) such that it is behind SEP
    * - Object on SEP id has been set to F360_INVALID_UNSIGNED_ID
    * Set object movable_prob to 0.0, such that it should not be considered for marking as behind SEP
    */
   obj.movable_prob = 0.0F;

   /** \action
    * Call Flag_Single_Object_Behind_SEP
    */
   Flag_Single_Object_Behind_SEP(sep, calibs, globals, obj);

   /** \result
    * Check that obj behind SEP id is F360_INVALID_UNSIGNED_ID
    * Check that object behind SEP ambiguous flag is false
    */
   CHECK_EQUAL_TEXT(F360_INVALID_UNSIGNED_ID, obj.behind_sep_id, "Nonmoveable object was marked as behind an SEP when the object should not have been.");
   CHECK_FALSE_TEXT(obj.f_behind_sep_ambiguous, "Object was marked as f_behind_sep_ambiguous when there is no ambiguity.");
}
/** @}*/


/** \defgroup  object_dead_zone_tests
 *  @{
 */

 /** \brief
  *  Test requirements w.r.t. setting object dead zone status
  */
TEST_GROUP(object_dead_zone_tests)
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

   /** \setup
   * Set up required variables
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      sensors[0].variable.is_valid = true;
      sensors[0].constant.mounting_position.vcs_position.longitudinal = 0.5F;

      sensors[1].variable.is_valid = true;
      sensors[1].constant.mounting_position.vcs_position.longitudinal = -5.0F;

      globals.average_sensor_position_x = -2.25F;

      relevant_object.vcs_position.y = 2.0F;
      relevant_object.vcs_position.x = -2.5F;
      
      relevant_object.status = F360_OBJECT_STATUS_UPDATED;

      host.speed = calibs.k_dead_zone_min_host_speed + 1.0F;


      tracker_info.active_obj_ids[0] = 1;
      tracker_info.num_active_objs = 1;
      Set_Tracker_Variant(tracker_info.variant);
   }
};

/**
*\purpose   Check if object dead zone status is correctly set if using dead zone is turned on
*\req       NA
*/
TEST(object_dead_zone_tests, object_dead_zone_set_to_inside_when_turned_on)
{
   /** \precond
   * Same as setup
   */

   /** \action
   * Call Pre_Association_Track_Management().
   */
   Pre_Association_Track_Management(tracker_info, object_tracks, raw_detection_list, sensors, host, calibs, globals, SEP, detection_props, timing_info);

   /** \result
   * Object dead zone set to inside
   */
   CHECK_EQUAL(F360_Dead_Zone_Status_T::INSIDE, relevant_object.dead_zone_status);
}

/**
*\purpose   Check if object dead zone status is correctly set to undefined if using dead zone is turned off
*\req       NA
*/
TEST(object_dead_zone_tests, object_dead_zone_set_to_undefined_when_turned_off)
{
   /** \precond
   * Same as setup with below changes
   * Turn off using dead zone
   */
   calibs.k_use_dead_zone_in_stationkeeping_scenarions = false;

   /** \action
   * Call Pre_Association_Track_Management().
   */
   Pre_Association_Track_Management(tracker_info, object_tracks, raw_detection_list, sensors, host, calibs, globals, SEP, detection_props, timing_info);

   /** \result
   * Object dead zone set to inside
   */
   CHECK_EQUAL(F360_Dead_Zone_Status_T::UNDEFINED, relevant_object.dead_zone_status);
}
/** @}*/

/** \defgroup  detecting_angle_jumps_from_SEP
 *  @{
 */

 /** \brief
  *  Test requirements w.r.t. angle jump detections (from SEP)
  */
TEST_GROUP(detecting_angle_jumps_from_SEP)
{
   const float tolerance = 1e-3F;
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS]{};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   F360_Calibrations_T calibs{};
   F360_Globals_T globals{};
   F360_Detection_Props_T detection_props[MAX_NUMBER_OF_DETECTIONS]{};
   F360_Tracker_Info_T tracker_info{};
   Static_Env_Poly_T SEP[F360_NUM_OF_STATIC_ENV_POLYS];
   F360_TRKR_TIMING_INFO_T timing_info{};
   F360_Host_T host{};

   /** \setup
   * Set up required variables
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      Set_Tracker_Variant(tracker_info.variant);

      for (uint16_t idx = 0; idx < 4; idx++)
      {
         sensors[idx].variable.is_valid = true;
      }

      sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;
      sensors[0].constant.mounting_position.vcs_position.lateral = -0.4F;
      sensors[0].constant.mounting_position.vcs_position.longitudinal = -0.4F;

      sensors[1].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;
      sensors[1].constant.mounting_position.vcs_position.lateral = -0.4F;
      sensors[1].constant.mounting_position.vcs_position.longitudinal = -3.0F;

      sensors[2].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;
      sensors[2].constant.mounting_position.vcs_position.lateral = 0.4F;
      sensors[2].constant.mounting_position.vcs_position.longitudinal = -0.4F;

      sensors[3].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;
      sensors[3].constant.mounting_position.vcs_position.lateral = 0.4F;
      sensors[3].constant.mounting_position.vcs_position.longitudinal = -3.0F;

      globals.average_sensor_position_x = -1.7F;

      for (uint16_t sep_idx = 0U; sep_idx < F360_NUM_OF_STATIC_ENV_POLYS; sep_idx++)
      {
         SEP[sep_idx].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
         SEP[sep_idx].lower_limit = -20.0F;
         SEP[sep_idx].upper_limit = 20.0F;
         SEP[sep_idx].p2 = 0.0F;
         SEP[sep_idx].p1 = 0.0F;

         if (0U == sep_idx)
         {
            SEP[sep_idx].p0 = -4.0F;
         }
         else if (1U == sep_idx)
         {
            SEP[sep_idx].p0 = 5.0F;
         }
         else
         {
            SEP[sep_idx].p0 = 2.0F * SEP[sep_idx - 2].p0;
         }
      }

      raw_detection_list.number_of_valid_detections = 1;
      raw_detection_list.detections[0].raw.sensor_id = 1;
      raw_detection_list.detections[0].raw.range = 2.6F + tolerance;
      raw_detection_list.detections[0].raw.range_rate = 0.0F;
      raw_detection_list.detections[0].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_MIDHIGH;

      raw_detection_list.detections[0].processed.vcs_az = calibs.k_angle_jump_min_abs_azimuth_vcs - tolerance;
      detection_props[0].on_sep_id = F360_INVALID_UNSIGNED_ID;
      detection_props[0].behind_sep_id = F360_INVALID_UNSIGNED_ID;
   }
};

/**
*\purpose   Check if detection is correctly marked as angle jump
*\req       NA
*/
TEST(detecting_angle_jumps_from_SEP, Detect_Angle_Jumps__Marked_detection)
{
   /** \precond
   * Same as setup
   */

   /** \action
   * Call Pre_Association_Track_Management().
   */
   Pre_Association_Track_Management(tracker_info, object_tracks, raw_detection_list, sensors, host, calibs, globals, SEP, detection_props, timing_info);

   /** \result
   * Detection marked as angle jump
   */
   CHECK_TRUE(detection_props[0].f_potential_angle_jump);
}

/**
*\purpose   Check if detection is NOT marked as angle jump due to range
*\req       NA
*/
TEST(detecting_angle_jumps_from_SEP, Detect_Angle_Jumps__NOT_marked_detection_due_to_range)
{
   /** \precond
   * Same as setup (plus changes below)
   * detection range set to high value
   */
   raw_detection_list.detections[0].raw.range = std::abs(SEP[0].p0 - sensors[1].constant.mounting_position.vcs_position.lateral) + calibs.k_angle_jump_range_tolerance + tolerance;

   /** \action
   * Call Pre_Association_Track_Management().
   */
   Pre_Association_Track_Management(tracker_info, object_tracks, raw_detection_list, sensors, host, calibs, globals, SEP, detection_props, timing_info);

   /** \result
   * Detection NOT marked as angle jump
   */
   CHECK_FALSE(detection_props[0].f_potential_angle_jump);
}
/** @}*/
