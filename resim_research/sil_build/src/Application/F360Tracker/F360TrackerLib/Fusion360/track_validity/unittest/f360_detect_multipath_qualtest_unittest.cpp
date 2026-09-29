/** \file
* This file contains qualification tests for content of f360_detect_multipath.cpp
*/

#include "f360_detect_multipath.h"
#include "f360_position.h"
#include "f360_radar_sensor.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include "f360_sorted_tracks_mgmt.h"
#include "f360_set_variant.h"
#include "f360_internal_preprocessing.h"

using namespace f360_variant_A;

/** \defgroup  f360_detect_multipath__Detect_And_Mark_Multipath_Objects_Qual
 *  @{
 */

 /** \brief
 *  Test group of Detect_And_Mark_Multipath_Objects from f360_detect_multipath module
 **/
TEST_GROUP(f360_detect_multipath__Detect_And_Mark_Multipath_Object_Qual)
{
   const float32_t tolerance = 0.00001F;

   F360_Tracker_Info_T tracker_info = {};
   F360_Calibrations_T calibrations = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Radar_Sensor_Props_T sensor_props[MAX_NUMBER_OF_SENSORS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   Static_Env_Poly_T polys[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_Host_T host = {};
   Multipath_Detector mp_detector{polys, object_tracks, tracker_info, calibrations};

   TEST_SETUP()
   {
      /** \setup
      Two rear sensors are set up.
      Three objects inside radar FOV are created: source, reflector and ghost.
      Properties of those objects are set according multipath theory.
      Tracker info is also fill properly for those three active objects.
      **/
      Initialize_Tracker_Calibrations(calibrations);
      Set_Tracker_Variant(tracker_info.variant);
      sensors[0].variable.is_valid = true;
      sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;
      sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = F360_DEG2RAD(135.0F);
      sensors[0].constant.mounting_position.vcs_position = { -3.6F, 0.6F, 0.0F };

      sensors[1].variable.is_valid = true;
      sensors[1].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;
      sensors[1].constant.mounting_position.vcs_boresight_azimuth_angle = F360_DEG2RAD(-135.0F);
      sensors[1].constant.mounting_position.vcs_position = { -3.6F, -0.6F, 0.0F };

      for (int k = 0; k < F360_NUM_LOOK_ID; k++)
      {
         sensors[0].constant.range_limits[k] = 80.0F;
         sensors[0].constant.fov_min_az_rad[k] = F360_DEG2RAD(-75.0F);
         sensors[0].constant.fov_max_az_rad[k] = F360_DEG2RAD(75.0F);
         sensors[1].constant.range_limits[k] = 80.0F;
         sensors[1].constant.fov_min_az_rad[k] = F360_DEG2RAD(-75.0F);
         sensors[1].constant.fov_max_az_rad[k] = F360_DEG2RAD(75.0F);
      }

       for (uint8_t sensor_index = 0U; sensor_index < MAX_NUMBER_OF_SENSORS; sensor_index++)
      {
         F360_Radar_Sensor_T &current_sensor = sensors[sensor_index];
         const float min_fov_az_angle_lr = std::min(current_sensor.constant.fov_min_az_rad[F360_DET_LOOK_ID_0], current_sensor.constant.fov_min_az_rad[F360_DET_LOOK_ID_1]);
         const float min_fov_az_interior_angle_lr = min_fov_az_angle_lr;
         const float max_fov_az_angle_lr = std::max(current_sensor.constant.fov_max_az_rad[F360_DET_LOOK_ID_0], current_sensor.constant.fov_max_az_rad[F360_DET_LOOK_ID_1]);
         const float max_fov_az_interior_angle_lr = max_fov_az_angle_lr;
         const float min_fov_az_angle_mr = std::min(current_sensor.constant.fov_min_az_rad[F360_DET_LOOK_ID_2], current_sensor.constant.fov_min_az_rad[F360_DET_LOOK_ID_3]);
         const float min_fov_az_interior_angle_mr = min_fov_az_angle_mr;
         const float max_fov_az_angle_mr = std::max(current_sensor.constant.fov_max_az_rad[F360_DET_LOOK_ID_2], current_sensor.constant.fov_max_az_rad[F360_DET_LOOK_ID_3]);
         const float max_fov_az_interior_angle_mr = max_fov_az_angle_mr;

         current_sensor.refined.interior_fov[F360_DET_LOOK_ID_0] = min_fov_az_interior_angle_lr;
         current_sensor.refined.interior_fov[F360_DET_LOOK_ID_1] = max_fov_az_interior_angle_lr;
         current_sensor.refined.interior_fov[F360_DET_LOOK_ID_2] = min_fov_az_interior_angle_mr;
         current_sensor.refined.interior_fov[F360_DET_LOOK_ID_3] = max_fov_az_interior_angle_mr;

         const float min_fov_vcs_az_angle_lr = current_sensor.constant.mounting_position.vcs_boresight_azimuth_angle + min_fov_az_interior_angle_lr;
         const float max_fov_vcs_az_angle_lr = current_sensor.constant.mounting_position.vcs_boresight_azimuth_angle + max_fov_az_interior_angle_lr;
         const float min_fov_vcs_az_angle_mr = current_sensor.constant.mounting_position.vcs_boresight_azimuth_angle + min_fov_az_interior_angle_mr;
         const float max_fov_vcs_az_angle_mr = current_sensor.constant.mounting_position.vcs_boresight_azimuth_angle + max_fov_az_interior_angle_mr;

         current_sensor.refined.left_fov_normal[F360_DET_LOOK_ID_0] = -F360_Sinf(min_fov_vcs_az_angle_lr);
         current_sensor.refined.left_fov_normal[F360_DET_LOOK_ID_1] = F360_Cosf(min_fov_vcs_az_angle_lr);
         current_sensor.refined.right_fov_normal[F360_DET_LOOK_ID_0] = F360_Sinf(max_fov_vcs_az_angle_lr);
         current_sensor.refined.right_fov_normal[F360_DET_LOOK_ID_1] = -F360_Cosf(max_fov_vcs_az_angle_lr);
         current_sensor.refined.left_fov_normal[F360_DET_LOOK_ID_2] = -F360_Sinf(min_fov_vcs_az_angle_mr);
         current_sensor.refined.left_fov_normal[F360_DET_LOOK_ID_3] = F360_Cosf(min_fov_vcs_az_angle_mr);
         current_sensor.refined.right_fov_normal[F360_DET_LOOK_ID_2] = F360_Sinf(max_fov_vcs_az_angle_mr);
         current_sensor.refined.right_fov_normal[F360_DET_LOOK_ID_3] = -F360_Cosf(max_fov_vcs_az_angle_mr);

      }

      F360_Object_Track_T &ghost_object = object_tracks[0];
      ghost_object.reference_point = F360_REFERENCE_POINT_CENTER;
      ghost_object.vcs_position = { -15.0F, 2.0F};
      ghost_object.bbox.Set_Center(ghost_object.vcs_position);
      ghost_object.vcs_velocity = { 1.5F, 2.0F };
      ghost_object.vcs_heading = Angle(F360_Atan2f(ghost_object.vcs_velocity.lateral, ghost_object.vcs_velocity.longitudinal));
      ghost_object.hdg_ptng_disagmt = 0.0F;
      ghost_object.bbox.Set_Orientation(ghost_object.vcs_heading + ghost_object.hdg_ptng_disagmt);
      ghost_object.bbox.Set_Length(5.0F);
      ghost_object.bbox.Set_Width(2.0F);
      ghost_object.f_moving = true;
      ghost_object.id = 1;
      ghost_object.status = F360_OBJECT_STATUS_UPDATED;
      ghost_object.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

      F360_Object_Track_T &source_object = object_tracks[1];
      source_object.reference_point = F360_REFERENCE_POINT_CENTER;
      source_object.vcs_position = { -12.0F, -4.0F};
      source_object.bbox.Set_Center(source_object.vcs_position);
      source_object.vcs_velocity = { 0.0F, 1.5F };
      source_object.vcs_heading = Angle(F360_Atan2f(source_object.vcs_velocity.lateral, source_object.vcs_velocity.longitudinal));
      source_object.hdg_ptng_disagmt = 0.0F;
      source_object.bbox.Set_Orientation(source_object.vcs_heading + source_object.hdg_ptng_disagmt);
      source_object.bbox.Set_Length(4.0F);
      source_object.bbox.Set_Width(3.0F);
      source_object.f_moving = true;
      source_object.id = 2;
      source_object.status = F360_OBJECT_STATUS_UPDATED;

      F360_Object_Track_T &reflector_object = object_tracks[2];
      reflector_object.reference_point = F360_REFERENCE_POINT_CENTER;
      reflector_object.vcs_position = { -8.5F, 1.0F };
      reflector_object.bbox.Set_Center(reflector_object.vcs_position);
      reflector_object.vcs_velocity = { 0.0F, 0.0F };
      reflector_object.vcs_heading = Angle(0.0F);
      reflector_object.hdg_ptng_disagmt = 0.0F;
      reflector_object.bbox.Set_Orientation(reflector_object.vcs_heading + reflector_object.hdg_ptng_disagmt);
      reflector_object.bbox.Set_Length(1.0F);
      reflector_object.bbox.Set_Width(1.0F);
      reflector_object.f_moving = false;
      reflector_object.id = 3;
      reflector_object.average_rcs = 1.0F;
      reflector_object.status = F360_OBJECT_STATUS_UPDATED;


      tracker_info.active_obj_ids[0] = 1;
      tracker_info.active_obj_ids[1] = 2;
      tracker_info.active_obj_ids[2] = 3;

      // Reset sorted vcslong list
      tracker_info.vcslong_sorted_start = NULL;
      for(uint32_t i = 0U; i < NUMBER_OF_OBJECT_TRACKS; i++)
      {
         tracker_info.vcslong_sorted_next_track[i] = NULL;
         tracker_info.vcslong_sorted_prev_track[i] = NULL;
      }
      // Add objects to vcslong sorted list
      tracker_info.num_active_objs = 1;
      Sorted_Tracks_Insert(tracker_info, &(ghost_object));
      tracker_info.num_active_objs = 2;
      Sorted_Tracks_Insert(tracker_info, &(source_object));
      tracker_info.num_active_objs = 3;
      Sorted_Tracks_Insert(tracker_info, &(reflector_object));
   }
};

/**
*\purpose Check if Detect_And_Mark_Multipath_Objects() is not activated properly if host speed is above threshold.
*\req    CPR-3768
*/
TEST(f360_detect_multipath__Detect_And_Mark_Multipath_Object_Qual, default_scenario_where_host_speed_is_too_high)
{
   /** \precond
   * Host speed is minimum host speed for disabling multipath detection.
   **/
   host.speed = calibrations.k_mp_max_allowed_host_speed_to_use_MP;
   /** \action
   * Call Detect_And_Mark_Multipath_Objects()
   **/
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[0], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[1], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[2], mp_detector);

   /** \result
   * All objects mirror probability should be set to 0.0F
   **/
   DOUBLES_EQUAL(0.0F, object_tracks[0].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[1].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[2].mirror_prob, tolerance);
}

/**
*\purpose Check if Detect_And_Mark_Multipath_Objects() change ghost object mirror probability to
*         default mirror probability when host speed is below threshold and all other
*         conditions have been met.
*\req     CPR-3768 CPR-3769 CPR-3771 CPR-3770
*/
TEST(f360_detect_multipath__Detect_And_Mark_Multipath_Object_Qual, default_scenario_where_host_speed_is_below_threshold)
{
   /** \precond
   * Host speed is lower than max host speed for enabling multipath detection.
   **/
   host.speed = calibrations.k_mp_max_allowed_host_speed_to_use_MP - 0.1F;

   /** \action
   * Call tested function
   **/
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[0], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[1], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[2], mp_detector);

   /** \result
   * Ghost object mirror probability should be set to default mirror probability. The rest objects should have 0.0F
   **/
   DOUBLES_EQUAL(0.0F, object_tracks[0].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[1].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[2].mirror_prob, tolerance);
}

/**
*\purpose Check if Detect_And_Mark_Multipath_Objects() doesn't change ghost object mirror probability to
*         default value when source object meet all conditions except motion status.
*\req     CPR-3769
*/
TEST(f360_detect_multipath__Detect_And_Mark_Multipath_Object_Qual, scenario_where__source_objects_is_stationary)
{
   /** \precond
   * Set source object moving flag to false
   **/
   object_tracks[0].f_moving = false;

   /** \action
   * Call tested function
   **/
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[0], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[1], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[2], mp_detector);

   /** \result
   * Ghost object mirror probability should be set to 0.0F. Other objects also should have 0.0F
   **/
   DOUBLES_EQUAL(0.0F, object_tracks[0].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[1].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[2].mirror_prob, tolerance);
}

/**
*\purpose Check if Detect_And_Mark_Multipath_Objects() doesn't change ghost object mirror probability to
*         default value when source object meet all conditions except object is inactive.
*\req     CPR-3769
*/
TEST(f360_detect_multipath__Detect_And_Mark_Multipath_Object_Qual, scenario_where__source_objects_is_inactive)
{
   /** \precond
   * Deactivate source object
   **/

   sorted_tracks_remove(tracker_info, object_tracks[0]);
   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 2;
   tracker_info.active_obj_ids[1] = 3;
   tracker_info.active_obj_ids[2] = 0;

   /** \action
   * Call tested function
   **/
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[0], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[1], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[2], mp_detector);

   /** \result
   * Ghost object mirror probability should be set to 0.0F. Other objects also should have 0.0F
   **/
   DOUBLES_EQUAL(0.0F, object_tracks[0].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[1].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[2].mirror_prob, tolerance);
}

/**
*\purpose Check if Detect_And_Mark_Multipath_Objects() doesn't change ghost object mirror probability to
*         default value when source object meet all conditions except object is not in sensor FOV.
*\req     CPR-3769
*/
TEST(f360_detect_multipath__Detect_And_Mark_Multipath_Object_Qual, scenario_where__source_objects_is_beyond_sensor_fov)
{
   /** \precond
   * Reduce sensors FOV.
   **/
   for (int k = 0; k < F360_NUM_LOOK_ID; k++)
   {
      sensors[0].constant.range_limits[k] = 0.0F;
      sensors[1].constant.range_limits[k] = 0.0F;

   }

   /** \action
   * Call tested function
   **/
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[0], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[1], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[2], mp_detector);

   /** \result
   * Ghost object mirror probability should be set to 0.0F. Other objects also should have 0.0F
   **/
   DOUBLES_EQUAL(0.0F, object_tracks[0].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[1].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[2].mirror_prob, tolerance);
}

/**
*\purpose Check if Detect_And_Mark_Multipath_Objects() doesn't change ghost object mirror probability to default value
*         when source object meet all conditions except object doesn't use CTCA motion model
*\req     CPR-3769
*/
TEST(f360_detect_multipath__Detect_And_Mark_Multipath_Object_Qual, scenario_where__source_objects_use_CCV_filter)
{
   /** \precond
   *   Set source object filter type for CCV.
   **/
   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CCV;

   /** \action
   * Call tested function
   **/
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[0], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[1], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[2], mp_detector);

   /** \result
   * Ghost object mirror probability should be set to 0.0F. Other objects also should have 0.0F
   **/
   DOUBLES_EQUAL(0.0F, object_tracks[0].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[1].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[2].mirror_prob, tolerance);
}

/**
*\purpose Check if Detect_And_Mark_Multipath_Objects() doesn't change ghost object mirror probability to
*         default value when reflector does not exists.
*\req     CPR-3770
*/
TEST(f360_detect_multipath__Detect_And_Mark_Multipath_Object_Qual, scenario_where__reflector_objects_does_not_exist)
{
   /** \precond
   *   Remove reflector object.
   **/
   sorted_tracks_remove(tracker_info, object_tracks[2]);
   object_tracks[2] = {};
   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 2;

   /** \action
   * Call tested function
   **/
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[0], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[1], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[2], mp_detector);

   /** \result
   * Ghost object mirror probability should be set to 0.0F. Other object also should have 0.0F.
   **/
   DOUBLES_EQUAL(0.0F, object_tracks[0].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[1].mirror_prob, tolerance);
}

/**
*\purpose Check if Detect_And_Mark_Multipath_Objects() doesn't change ghost object mirror probability to
*         default value when source does not exists.
*\req     CPR-3770
*/
TEST(f360_detect_multipath__Detect_And_Mark_Multipath_Object_Qual, scenario_where__source_objects_does_not_exist)
{
   /** \precond
   *   Remove source object.
   **/
   sorted_tracks_remove(tracker_info, object_tracks[1]);
   object_tracks[1] = {};
   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[0] = 1;
   tracker_info.active_obj_ids[1] = 3;

   /** \action
   * Call tested function
   **/
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[0], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[1], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[2], mp_detector);

   /** \result
   * Ghost object mirror probability should be set to 0.0F. Other object also should have 0.0F.
   **/
   DOUBLES_EQUAL(0.0F, object_tracks[0].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[1].mirror_prob, tolerance);
}

/** @}*/
