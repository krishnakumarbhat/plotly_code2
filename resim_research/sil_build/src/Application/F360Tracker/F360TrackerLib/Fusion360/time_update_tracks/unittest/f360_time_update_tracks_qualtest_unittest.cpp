/** \file
 * This file contains qualification tests for content of f360_time_update_tracks.cpp file
 */

#include "f360_kill_coasted_tracks.h"
#include "f360_set_variant.h"
#include "f360_radar_sensor_props.h"
#include "f360_internal_preprocessing.h"
#include <CppUTest/TestHarness.h>


// Unit testing guidelines: http://confluenceprod1.delphiauto.net:8090/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_kill_coasted_tracks_qualtest
 *  @{
 */

/** \brief
 * This group provides tests ensuring specified behavior for termination of coasting objects.
 */
TEST_GROUP(f360_kill_coasted_tracks_qualtest)
{
   // Declare common variables used within all tests in this test group.
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_TRKR_TIMING_INFO_T timing_info;
   F360_Calibrations_T calibrations = {};
   F360_Globals_T globals;
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS];

   /** \setup
    * Define calibration thresholds needed for function-under-test
    * - set threshold for mirror probability for termination of coasting objects to 0.3
    * - set threshold for stage age of confirmed coasting tracks for termination to 0.55 s
    * - set threshold for stage age of mirrored coasting tracks for termination to 0.25 s
    */
   TEST_SETUP()
   {
      // Initialize calibrations
      Initialize_Tracker_Calibrations(calibrations);
      Set_Tracker_Variant(tracker_info.variant);

      // Set sensor mounting position and FOV
      const float32_t azim_th = F360_PI / 4;
      sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;
      sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = 0.0F;
      sensors[0].constant.mounting_position.vcs_position.longitudinal = -5.5F;
      sensors[0].constant.mounting_position.vcs_position.lateral = 0.0F;

      for (int i = 0; i < 4; i++)
      {
         sensors[0].constant.fov_min_az_rad[i] = -azim_th;
         sensors[0].constant.fov_max_az_rad[i] = azim_th;
      }

      const float min_fov_az_angle_lr = std::min(sensors[0].constant.fov_min_az_rad[F360_DET_LOOK_ID_0], sensors[0].constant.fov_min_az_rad[F360_DET_LOOK_ID_1]);
      const float min_fov_az_interior_angle_lr = min_fov_az_angle_lr;
      const float max_fov_az_angle_lr = std::max(sensors[0].constant.fov_max_az_rad[F360_DET_LOOK_ID_0], sensors[0].constant.fov_max_az_rad[F360_DET_LOOK_ID_1]);
      const float max_fov_az_interior_angle_lr = max_fov_az_angle_lr;
      const float min_fov_az_angle_mr = std::min(sensors[0].constant.fov_min_az_rad[F360_DET_LOOK_ID_2], sensors[0].constant.fov_min_az_rad[F360_DET_LOOK_ID_3]);
      const float min_fov_az_interior_angle_mr = min_fov_az_angle_mr;
      const float max_fov_az_angle_mr = std::max(sensors[0].constant.fov_max_az_rad[F360_DET_LOOK_ID_2], sensors[0].constant.fov_max_az_rad[F360_DET_LOOK_ID_3]);
      const float max_fov_az_interior_angle_mr = max_fov_az_angle_mr;

      sensors[0].refined.interior_fov[F360_DET_LOOK_ID_0] = min_fov_az_interior_angle_lr;
      sensors[0].refined.interior_fov[F360_DET_LOOK_ID_1] = max_fov_az_interior_angle_lr;
      sensors[0].refined.interior_fov[F360_DET_LOOK_ID_2] = min_fov_az_interior_angle_mr;
      sensors[0].refined.interior_fov[F360_DET_LOOK_ID_3] = max_fov_az_interior_angle_mr;

      const float min_fov_vcs_az_angle_lr = sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle + min_fov_az_interior_angle_lr;
      const float max_fov_vcs_az_angle_lr = sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle + max_fov_az_interior_angle_lr;
      const float min_fov_vcs_az_angle_mr = sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle + min_fov_az_interior_angle_mr;
      const float max_fov_vcs_az_angle_mr = sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle + max_fov_az_interior_angle_mr;

      sensors[0].refined.left_fov_normal[F360_DET_LOOK_ID_0] = -F360_Sinf(min_fov_vcs_az_angle_lr);
      sensors[0].refined.left_fov_normal[F360_DET_LOOK_ID_1] = F360_Cosf(min_fov_vcs_az_angle_lr);
      sensors[0].refined.right_fov_normal[F360_DET_LOOK_ID_0] = F360_Sinf(max_fov_vcs_az_angle_lr);
      sensors[0].refined.right_fov_normal[F360_DET_LOOK_ID_1] = -F360_Cosf(max_fov_vcs_az_angle_lr);
      sensors[0].refined.left_fov_normal[F360_DET_LOOK_ID_2] = -F360_Sinf(min_fov_vcs_az_angle_mr);
      sensors[0].refined.left_fov_normal[F360_DET_LOOK_ID_3] = F360_Cosf(min_fov_vcs_az_angle_mr);
      sensors[0].refined.right_fov_normal[F360_DET_LOOK_ID_2] = F360_Sinf(max_fov_vcs_az_angle_mr);
      sensors[0].refined.right_fov_normal[F360_DET_LOOK_ID_3] = -F360_Cosf(max_fov_vcs_az_angle_mr);
   }
};

/** \purpose
 * This test ensures that updated objects are not terminated, even if their remaining attributes fulfill conditions for being terminated.
 * \req
 * CPR-3754
 */
TEST(f360_kill_coasted_tracks_qualtest, Do_not_terminate_updated_tracks)
{
   /** \precond
    * Create an object
    * - set status to UPDATED
    * - set time since stage start to a value exceeding the threshold for termination of coasting objects
    * - set mirror probability to a value exceeding the threshold for termination of coasting objects
    * Set number number of active objects and values for active object id array in tracker information structure accordingly
    */
   tracker_info.num_active_objs = 1;
   tracker_info.active_obj_ids[0] = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].time_since_stage_start = calibrations.k_max_conf_objtrk_coast_time + 0.1F;
   object_tracks[0].mirror_prob = calibrations.k_mirror_prob_threshold + 0.2F;

   /** \action
    * Call function-under-test
    */
   Kill_Coasted_Tracks(object_tracks, tracker_info, timing_info, calibrations, globals, sensors);

   /** \result
    * Object is not terminated
    */
   CHECK_EQUAL_TEXT(F360_OBJECT_STATUS_UPDATED, object_tracks[0].status, "Object status is expected not to change to invalid");
   CHECK_EQUAL_TEXT(1, tracker_info.num_active_objs, "Number of active objects did not change");
   CHECK_EQUAL_TEXT(1, tracker_info.active_obj_ids[0], "Active object id is set to 1");
}

/** \purpose
* This test ensures that confirmed coasting objects are terminated, if their time since stage start exceeds a given threshold.
* \req
* CPR-3754
*/
TEST(f360_kill_coasted_tracks_qualtest, Termination_of_coasting_confirmed_objects)
{
   /** \precond
    * Create three objects
    * - set status of all objects to COASTED
    * - 1. object: set time since stage start to a value below the threshold for termination of coasting objects
    * - 2. object: set time since stage start to a value that equals the threshold for termination of coasting objects
    * - 3. object: set time since stage start to a value above the threshold for termination of coasting objects
    * Set number number of active objects and values for active object id array in tracker information structure accordingly
    */
   tracker_info.num_active_objs = 3;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;
   tracker_info.active_obj_ids[2] = 3;

   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].time_since_stage_start = calibrations.k_max_conf_objtrk_coast_time - 0.1F;

   object_tracks[1].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[1].time_since_stage_start = calibrations.k_max_conf_objtrk_coast_time;

   object_tracks[2].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[2].time_since_stage_start = calibrations.k_max_conf_objtrk_coast_time + 0.1F;

   /** \action
    * Call function-under-test
    */
   Kill_Coasted_Tracks(object_tracks, tracker_info, timing_info, calibrations, globals, sensors);

   /** \result
    * Coasting object with time since stage start at a value above the threshold is expected to be terminated, the other two
    * objects keep their status
    */
   CHECK_EQUAL_TEXT(F360_OBJECT_STATUS_COASTED, object_tracks[0].status, "Object status is expected not to change");
   CHECK_EQUAL_TEXT(F360_OBJECT_STATUS_COASTED, object_tracks[1].status, "Object status is expected not to change");
   CHECK_EQUAL_TEXT(F360_OBJECT_STATUS_INVALID, object_tracks[2].status, "Object status is expected to change to invalid");
   CHECK_EQUAL_TEXT(2, tracker_info.num_active_objs, "Number of active objects is expected to be decreased by 1");
   CHECK_EQUAL_TEXT(1, tracker_info.active_obj_ids[0], "Active object id is set to 1");
   CHECK_EQUAL_TEXT(2, tracker_info.active_obj_ids[1], "Active object id is set to 2");
   CHECK_EQUAL_TEXT(0, tracker_info.active_obj_ids[2], "Active object id contains default value");
}



/** \purpose
* This test ensures that mirrored coasting objects are terminated, if their time since stage start exceeds a given threshold.
* \req
* CPR-3754
*/
TEST(f360_kill_coasted_tracks_qualtest, Termination_of_coasting_mirror_objects_variation_of_stage_time)
{
   /** \precond
   * Create three objects
   * - set status of all objects to COASTED
   * - set mirror probability for all objects to a value above threshold for termination of coasting mirror objects
   * - 1. object: set time since stage start to a value below the threshold for termination of coasting mirror objects
   * - 2. object: set time since stage start to a value that equals the threshold for termination of coasting mirror objects
   * - 3. object: set time since stage start to a value above the threshold for termination of coasting mirror objects
   * Set number number of active objects and values for active object id array in tracker information structure accordingly
   */
   tracker_info.num_active_objs = 3;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;
   tracker_info.active_obj_ids[2] = 3;

   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].time_since_stage_start = calibrations.k_max_coast_time_mirror - 0.1F;
   object_tracks[0].mirror_prob = calibrations.k_mirror_prob_threshold + 0.2F;

   object_tracks[1].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[1].time_since_stage_start = calibrations.k_max_coast_time_mirror;
   object_tracks[1].mirror_prob = calibrations.k_mirror_prob_threshold + 0.2F;

   object_tracks[2].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[2].time_since_stage_start = calibrations.k_max_coast_time_mirror + 0.1F;
   object_tracks[2].mirror_prob = calibrations.k_mirror_prob_threshold + 0.2F;

   /** \action
    * Call function-under-test
    */
   Kill_Coasted_Tracks(object_tracks, tracker_info, timing_info, calibrations, globals, sensors);

   /** \result
    * Coasting object with time since stage start at a value above the threshold is expected to be terminated, the other two
    * objects keep their status
    */
   CHECK_EQUAL_TEXT(F360_OBJECT_STATUS_COASTED, object_tracks[0].status, "Object status is expected not to change");
   CHECK_EQUAL_TEXT(F360_OBJECT_STATUS_COASTED, object_tracks[1].status, "Object status is expected not to change");
   CHECK_EQUAL_TEXT(F360_OBJECT_STATUS_INVALID, object_tracks[2].status, "Object status is expected to change to invalid");
   CHECK_EQUAL_TEXT(2, tracker_info.num_active_objs, "Number of active objects is expected to be decreased by 1");
   CHECK_EQUAL_TEXT(1, tracker_info.active_obj_ids[0], "Active object id is set to 1");
   CHECK_EQUAL_TEXT(2, tracker_info.active_obj_ids[1], "Active object id is set to 2");
   CHECK_EQUAL_TEXT(0, tracker_info.active_obj_ids[2], "Active object id is unset");
}

/** \purpose
* This test ensures that mirrored coasting objects are terminated, if their time since stage start and their mirror probabilities exceed given thresholds.
* \req
* CPR-3754
*/
TEST(f360_kill_coasted_tracks_qualtest, Termination_of_coasting_mirror_objects_variation_of_mirror_probability)
{
   /** \precond
    * Create three objects
    * - set status of all objects to COASTED
    * - set time since stage start for all objects to a value above the threshold for termination of coasting mirror objects
    * - 1. object: set mirror probability to a value below the threshold for termination of coasting mirror objects
    * - 2. object: set mirror probability to a value that equals the threshold for termination of coasting mirror objects
    * - 3. object: set mirror probability to a value above the threshold for termination of coasting mirror objects
    * Set number number of active objects and values for active object id array in tracker information structure accordingly
    */
   tracker_info.num_active_objs = 3;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;
   tracker_info.active_obj_ids[2] = 3;

   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].time_since_stage_start = calibrations.k_max_coast_time_mirror + 0.1F;
   object_tracks[0].mirror_prob = calibrations.k_mirror_prob_threshold - 0.2F;

   object_tracks[1].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[1].time_since_stage_start = calibrations.k_max_coast_time_mirror + 0.1F;
   object_tracks[1].mirror_prob = calibrations.k_mirror_prob_threshold;

   object_tracks[2].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[2].time_since_stage_start = calibrations.k_max_coast_time_mirror + 0.1F;
   object_tracks[2].mirror_prob = calibrations.k_mirror_prob_threshold + 0.2F;

   /** \action
    * Call function-under-test
    */
   Kill_Coasted_Tracks(object_tracks, tracker_info, timing_info, calibrations, globals, sensors);

   /** \result
   * Coasting object with mirror probability at a value above the threshold is expected to be terminated, the other two
   * objects keep their status
   */
   CHECK_EQUAL_TEXT(F360_OBJECT_STATUS_COASTED, object_tracks[0].status, "Object status is expected not to change");
   CHECK_EQUAL_TEXT(F360_OBJECT_STATUS_COASTED, object_tracks[1].status, "Object status is expected not to change");
   CHECK_EQUAL_TEXT(F360_OBJECT_STATUS_INVALID, object_tracks[2].status, "Object status is expected to change to invalid");
   CHECK_EQUAL_TEXT(2, tracker_info.num_active_objs, "Number of active objects is expected to be decreased by 1");
   CHECK_EQUAL_TEXT(1, tracker_info.active_obj_ids[0], "Active object id is set to 1");
   CHECK_EQUAL_TEXT(2, tracker_info.active_obj_ids[1], "Active object id is set to 2");
   CHECK_EQUAL_TEXT(0, tracker_info.active_obj_ids[2], "Active object id is unset");
}

/** \purpose
* This test ensures that non-moveable coasting objects, outside FOV given front radar only config, are terminated, if their time since stage start exceeds a given threshold.
* \req
* CPR-4890
*/
TEST(f360_kill_coasted_tracks_qualtest, Non_Movable_Coasting_Obj_Outside_Front_Only_FOV_Coasting_Time)
{
   /** \precond
    * Create three objects
    * - set status of all objects to COASTED
    * - set mirror probability to 0.1 to prevent mirror probability kill branch from being triggered
    * - set movable_prob to be 0.0
    * - set object position outside sensor FOV
    * - 1. object: set time since stage start to a value below the threshold for termination of coasting objects
    * - 2. object: set time since stage start to a value that equals the threshold for termination of coasting objects
    * - 3. object: set time since stage start to a value above the threshold for termination of coasting objects
    * With the object alive time near the threshold of k_max_coast_time_outside_fov, the normal coasting kill branch is not triggered.
    * Set number number of active objects and values for active object id array in tracker information structure accordingly
    * Set f_single_front_center_radar_only to TRUE
    */
   tracker_info.num_active_objs = 3;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;
   tracker_info.active_obj_ids[2] = 3;
   globals.f_single_front_center_radar_only = true;

   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].vcs_position = Point(0.0F, 10.0F);
   object_tracks[0].mirror_prob = 0.1F;
   object_tracks[0].movable_prob = 0.0F;
   object_tracks[0].time_since_stage_start = calibrations.k_max_coast_time_outside_fov - 0.1F;

   object_tracks[1].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[1].vcs_position = Point(0.0F, 10.0F);
   object_tracks[1].mirror_prob = 0.1F;
   object_tracks[1].movable_prob = 0.0F;
   object_tracks[1].time_since_stage_start = calibrations.k_max_coast_time_outside_fov;

   object_tracks[2].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[2].vcs_position = Point(0.0F, 10.0F);
   object_tracks[2].mirror_prob = 0.1F;
   object_tracks[2].movable_prob = 0.0F;
   object_tracks[2].time_since_stage_start = calibrations.k_max_coast_time_outside_fov + 0.1F;

   /** \action
    * Call function-under-test
    */
   Kill_Coasted_Tracks(object_tracks, tracker_info, timing_info, calibrations, globals, sensors);

   /** \result
    * Coasting object with time since stage start at a value above the threshold is expected to be terminated, the other two
    * objects keep their status
    */
   CHECK_EQUAL_TEXT(F360_OBJECT_STATUS_COASTED, object_tracks[0].status, "Object status is expected not to change");
   CHECK_EQUAL_TEXT(F360_OBJECT_STATUS_COASTED, object_tracks[1].status, "Object status is expected not to change");
   CHECK_EQUAL_TEXT(F360_OBJECT_STATUS_INVALID, object_tracks[2].status, "Object status is expected to change to invalid");
   CHECK_EQUAL_TEXT(2, tracker_info.num_active_objs, "Number of active objects is expected to be decreased by 1");
   CHECK_EQUAL_TEXT(1, tracker_info.active_obj_ids[0], "Active object id is set to 1");
   CHECK_EQUAL_TEXT(2, tracker_info.active_obj_ids[1], "Active object id is set to 2");
   CHECK_EQUAL_TEXT(0, tracker_info.active_obj_ids[2], "Active object id contains default value");
}

/** \purpose
* This test ensures that non-moveable long coasting objects given front radar only config, are terminated, if their position is outside sensor FOV.
* \req
* CPR-4890
*/
TEST(f360_kill_coasted_tracks_qualtest, Non_Movable_Coasting_Obj_Outside_Front_Only_FOV_Pos)
{
   /** \precond
    * Create two objects
    * - set status of all objects to COASTED
    * - set mirror probability to 0.1 to prevent mirror probability kill branch from being triggered
    * - set movable_prob to be 0.0
    * - set time since stage start to a value above the threshold for termination of coasting objects
    * - 1. object: set object position within FOV
    * - 2. object: set object position outside FOV
    * With the object alive time near the threshold of k_max_coast_time_outside_fov, the normal coasting kill branch is not triggered.
    * Set number number of active objects and values for active object id array in tracker information structure accordingly
    * Set f_single_front_center_radar_only to TRUE
    */
   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;
   globals.f_single_front_center_radar_only = true;

   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].vcs_position = Point(10.0F, 0.0F);
   object_tracks[0].mirror_prob = 0.1F;
   object_tracks[0].movable_prob = 0.0F;
   object_tracks[0].time_since_stage_start = calibrations.k_max_coast_time_outside_fov + 0.1F;

   object_tracks[1].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[1].vcs_position = Point(0.0F, 10.0F);
   object_tracks[1].mirror_prob = 0.1F;
   object_tracks[1].movable_prob = 0.0F;
   object_tracks[1].time_since_stage_start = calibrations.k_max_coast_time_outside_fov + 0.1F;


   /** \action
    * Call function-under-test
    */
   Kill_Coasted_Tracks(object_tracks, tracker_info, timing_info, calibrations, globals, sensors);

   /** \result
    * Coasting object with position outside FOV is expected to be terminated, the other
    * object keeps its status
    */
   CHECK_EQUAL_TEXT(F360_OBJECT_STATUS_COASTED, object_tracks[0].status, "Object status is expected not to change");
   CHECK_EQUAL_TEXT(F360_OBJECT_STATUS_INVALID, object_tracks[1].status, "Object status is expected to change to invalid");
   CHECK_EQUAL_TEXT(1, tracker_info.num_active_objs, "Number of active objects is expected to be decreased by 1");
   CHECK_EQUAL_TEXT(1, tracker_info.active_obj_ids[0], "Active object id is set to 1");
   CHECK_EQUAL_TEXT(0, tracker_info.active_obj_ids[1], "Active object id contains default value");
}

/** \purpose
* This test ensures that non-moveable long coasting objects given outside sensor FOV, are not terminated unless the config is front radar only
* \req
* CPR-4890
*/
TEST(f360_kill_coasted_tracks_qualtest, Non_Movable_Coasting_Obj_Outside_Front_Only_FOV_Config)
{
   /** \precond
    * Create one objects
    * - set status to COASTED
    * - set mirror probability to 0.1 to prevent mirror probability kill branch from being triggered
    * - set movable_prob to be 0.0
    * - set time since stage start to a value above the threshold for termination of coasting objects
    * - set object position outside FOV
    * With the object alive time near the threshold of k_max_coast_time_outside_fov, the normal coasting kill branch is not triggered.
    * Set number number of active objects and values for active object id array in tracker information structure accordingly
    * Set f_single_front_center_radar_only to FALSE
    */
   tracker_info.num_active_objs = 1;
   tracker_info.active_obj_ids[0] = 1;
   globals.f_single_front_center_radar_only = false;

   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].vcs_position = Point(0.0F, 10.0F);
   object_tracks[0].mirror_prob = 0.1F;
   object_tracks[0].movable_prob = 0.0F;
   object_tracks[0].time_since_stage_start = calibrations.k_max_coast_time_outside_fov + 0.1F;


   /** \action
    * Call function-under-test
    */
   Kill_Coasted_Tracks(object_tracks, tracker_info, timing_info, calibrations, globals, sensors);

   /** \result
    * Coasting object with position outside FOV is NOT expected to be terminated, if NOT in a front sensor only config
    */
   CHECK_EQUAL_TEXT(F360_OBJECT_STATUS_COASTED, object_tracks[0].status, "Object status is expected not to change");
   CHECK_EQUAL_TEXT(1, tracker_info.active_obj_ids[0], "Active object id is set to 1");

}

/** \purpose
* This test ensures that coasting objects given outside sensor FOV and front radar only config, are not terminated, unless the object is non-moveable
* \req
* CPR-4890
*/
TEST(f360_kill_coasted_tracks_qualtest, Non_Movable_Coasting_Obj_Outside_Front_Only_FOV_Nonmoveable)
{
   /** \precond
    * Create two objects
    * - set status to COASTED
    * - set mirror probability to 0.1 to prevent mirror probability kill branch from being triggered
    * - set time since stage start to a value above the threshold for termination of coasting objects
    * - set object position outside FOV
    * - 1. object: set movable_prob to be 1.0
    * - 2. object: set movable_prob to be 0.0
    * With the object alive time near the threshold of k_max_coast_time_outside_fov, the normal coasting kill branch is not triggered.
    * Set number number of active objects and values for active object id array in tracker information structure accordingly
    * Set f_single_front_center_radar_only to TRUE
    */
   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;
   globals.f_single_front_center_radar_only = true;

   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].vcs_position = Point(0.0F, 10.0F);
   object_tracks[0].mirror_prob = 0.1F;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].time_since_stage_start = calibrations.k_max_coast_time_outside_fov + 0.1F;

   object_tracks[1].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[1].vcs_position = Point(0.0F, 10.0F);
   object_tracks[1].mirror_prob = 0.1F;
   object_tracks[1].movable_prob = 0.0F;
   object_tracks[1].time_since_stage_start = calibrations.k_max_coast_time_outside_fov + 0.1F;

   /** \action
    * Call function-under-test
    */
   Kill_Coasted_Tracks(object_tracks, tracker_info, timing_info, calibrations, globals, sensors);

   /** \result
    * Coasting objects with position outside FOV and in radar only config are NOT expected to be terminated, unless it is non-moveable
    */
   CHECK_EQUAL_TEXT(F360_OBJECT_STATUS_COASTED, object_tracks[0].status, "Object status is expected not to change");
   CHECK_EQUAL_TEXT(F360_OBJECT_STATUS_INVALID, object_tracks[1].status, "Object status is expected to change to invalid");
   CHECK_EQUAL_TEXT(1, tracker_info.num_active_objs, "Number of active objects is expected to be decreased by 1");
   CHECK_EQUAL_TEXT(1, tracker_info.active_obj_ids[0], "Active object id is set to 1");
   CHECK_EQUAL_TEXT(0, tracker_info.active_obj_ids[1], "Active object id contains default value");

}
/** @}*/
