/** \file
* This file contains unit tests for content of f360_detect_multipath.cpp
*/

#include "f360_detect_multipath.h"
#include "f360_position.h"
#include "f360_radar_sensor.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>
#include "f360_sorted_tracks_mgmt.h"
#include "f360_set_variant.h"
#include "f360_internal_preprocessing.h"
using namespace f360_variant_A;

/** \defgroup  f360_detect_multipath__Check_If_Object_Is_MP_For_Any_Sensor
 *  @{
 */

/** \brief
*  Test group of Check_If_Object_Is_MP_For_Any_Sensor from f360_detect_multipath module
**/

void Update_Sensor_FOV(F360_Radar_Sensor_T &sensor)
{
   // normal vectors for edges of interior FOV.
   const float32_t min_fov_az_angle_lr = std::min(sensor.constant.fov_min_az_rad[F360_DET_LOOK_ID_0], sensor.constant.fov_min_az_rad[F360_DET_LOOK_ID_1]);
   const float32_t min_fov_az_interior_angle_lr = min_fov_az_angle_lr;
   const float32_t max_fov_az_angle_lr = std::max(sensor.constant.fov_max_az_rad[F360_DET_LOOK_ID_0], sensor.constant.fov_max_az_rad[F360_DET_LOOK_ID_1]);
   const float32_t max_fov_az_interior_angle_lr = max_fov_az_angle_lr;
   const float32_t min_fov_az_angle_mr = std::min(sensor.constant.fov_min_az_rad[F360_DET_LOOK_ID_2], sensor.constant.fov_min_az_rad[F360_DET_LOOK_ID_3]);
   const float32_t min_fov_az_interior_angle_mr = min_fov_az_angle_mr;
   const float32_t max_fov_az_angle_mr = std::max(sensor.constant.fov_max_az_rad[F360_DET_LOOK_ID_2], sensor.constant.fov_max_az_rad[F360_DET_LOOK_ID_3]);
   const float32_t max_fov_az_interior_angle_mr = max_fov_az_angle_mr;
   sensor.refined.interior_fov[F360_DET_LOOK_ID_0] = min_fov_az_interior_angle_lr;
   sensor.refined.interior_fov[F360_DET_LOOK_ID_1] = max_fov_az_interior_angle_lr;
   sensor.refined.interior_fov[F360_DET_LOOK_ID_2] = min_fov_az_interior_angle_mr;
   sensor.refined.interior_fov[F360_DET_LOOK_ID_3] = max_fov_az_interior_angle_mr;
   const float32_t min_fov_vcs_az_angle_lr = sensor.constant.mounting_position.vcs_boresight_azimuth_angle + min_fov_az_interior_angle_lr;
   const float32_t max_fov_vcs_az_angle_lr = sensor.constant.mounting_position.vcs_boresight_azimuth_angle + max_fov_az_interior_angle_lr;
   const float32_t min_fov_vcs_az_angle_mr = sensor.constant.mounting_position.vcs_boresight_azimuth_angle + min_fov_az_interior_angle_mr;
   const float32_t max_fov_vcs_az_angle_mr = sensor.constant.mounting_position.vcs_boresight_azimuth_angle + max_fov_az_interior_angle_mr;
   sensor.refined.left_fov_normal[F360_DET_LOOK_ID_0] = -F360_Sinf(min_fov_vcs_az_angle_lr);
   sensor.refined.left_fov_normal[F360_DET_LOOK_ID_1] = F360_Cosf(min_fov_vcs_az_angle_lr);
   sensor.refined.right_fov_normal[F360_DET_LOOK_ID_0] = F360_Sinf(max_fov_vcs_az_angle_lr);
   sensor.refined.right_fov_normal[F360_DET_LOOK_ID_1] = -F360_Cosf(max_fov_vcs_az_angle_lr);
   sensor.refined.left_fov_normal[F360_DET_LOOK_ID_2] = -F360_Sinf(min_fov_vcs_az_angle_mr);
   sensor.refined.left_fov_normal[F360_DET_LOOK_ID_3] = F360_Cosf(min_fov_vcs_az_angle_mr);
   sensor.refined.right_fov_normal[F360_DET_LOOK_ID_2] = F360_Sinf(max_fov_vcs_az_angle_mr);
   sensor.refined.right_fov_normal[F360_DET_LOOK_ID_3] = -F360_Cosf(max_fov_vcs_az_angle_mr);
}

TEST_GROUP(f360_detect_multipath__Check_If_Object_Is_MP_For_Any_Sensor)
{
   const float32_t tolerance = 0.00001F;

   F360_Tracker_Info_T tracker_info = {};
   F360_Calibrations_T calibrations = {};

   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Radar_Sensor_Props_T sensor_props[MAX_NUMBER_OF_SENSORS] = {};
   Static_Env_Polys_Array polys = {};

   TEST_SETUP()
   {
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

      for (int sensor_idx = 0; sensor_idx < MAX_NUMBER_OF_SENSORS; sensor_idx++)
      {
         Update_Sensor_FOV(sensors[sensor_idx]);
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
      ghost_object.number_of_events_of_multipath_with_forgetting_factor = 5.0F;
      ghost_object.time_since_initialization_with_forgetting_factor = 0.3F;
      ghost_object.f_moving = true;
      ghost_object.id = 1;
      ghost_object.status = F360_OBJECT_STATUS_UPDATED;

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
      source_object.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(90.0F) });
      source_object.status = F360_OBJECT_STATUS_UPDATED;

      F360_Object_Track_T &reflector_object = object_tracks[2];
      reflector_object.reference_point = F360_REFERENCE_POINT_CENTER;
      reflector_object.vcs_position = { -8.5F, 1.0F};
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

      tracker_info.num_active_objs = 3;
      Sorted_Tracks_Insert(tracker_info, &(ghost_object));
      Sorted_Tracks_Insert(tracker_info, &(source_object));
      Sorted_Tracks_Insert(tracker_info, &(reflector_object));
   }
};

/**
*\purpose Check if Check_If_Object_Is_MP_For_Any_Sensor() returns true for default setup where ghost should be found.
*\req    NA
*/
TEST(f360_detect_multipath__Check_If_Object_Is_MP_For_Any_Sensor, default_scenario_where_ghost_is_found)
{
   /** \precond
   * Same as test group setup
   **/
   Multipath_Detector mp_detector{polys, object_tracks, tracker_info, calibrations};

   /** \action
   * Call tested function
   **/
   const bool test_result = Check_If_Object_Is_MP_For_Any_Sensor(sensors, tracker_info, object_tracks[0], mp_detector);

   /** \result
   * Returned true
   **/
   CHECK_TRUE(test_result);
}

/**
*\purpose Check if Check_If_Object_Is_MP_For_Any_Sensor() returns false for false positive object.
*\req    NA
*/
TEST(f360_detect_multipath__Check_If_Object_Is_MP_For_Any_Sensor, scenario_where_real_object_was_once_marked_as_multipath)
{
   /** \precond
   * Same as test group setup
   * Only set number_of_events_of_multipath_with_forgetting_facto to being marked only once as multipath
   **/
   Multipath_Detector mp_detector{polys, object_tracks, tracker_info, calibrations};
   object_tracks[0].number_of_events_of_multipath_with_forgetting_factor = 0.0F;

   /** \action
   * Call tested function
   **/
   const bool test_result = Check_If_Object_Is_MP_For_Any_Sensor(sensors, tracker_info, object_tracks[0], mp_detector);

   /** \result
   * Returned false
   **/
   CHECK_FALSE(test_result);
}

/**
*\purpose Check if Check_If_Object_Is_MP_For_Any_Sensor() returns false when ghost candidate range rate is positive.
*\req    NA
*/
TEST(f360_detect_multipath__Check_If_Object_Is_MP_For_Any_Sensor, no_ghost_detected_due_to_positive_range_rate)
{
   /** \precond
   * Same as test group setup
   * Change ghost velocity in order to tet positive range rate
   **/
   F360_Object_Track_T &ghost_object = object_tracks[0];
   ghost_object.vcs_velocity = { -1.5F, 2.0F };
   Multipath_Detector mp_detector{ polys, object_tracks, tracker_info, calibrations };
   object_tracks[0].number_of_events_of_multipath_with_forgetting_factor = 1.0F;

   /** \action
   * Call tested function
   **/
   const bool test_result = Check_If_Object_Is_MP_For_Any_Sensor(sensors, tracker_info, object_tracks[0], mp_detector);

   /** \result
   * Returned false
   **/
   CHECK_FALSE(test_result);
}

/**
*\purpose Check if Check_If_Object_Is_MP_For_Any_Sensor() returns false when MP detector algorith returns false
*\req    NA
*/
TEST(f360_detect_multipath__Check_If_Object_Is_MP_For_Any_Sensor, no_ghost_detected_due_to_fail_in_MP_Detector)
{
   /** \precond
   * Same as test group setup
   * Change source f_moving to false
   **/
   F360_Object_Track_T &source_object = object_tracks[1];
   source_object.f_moving = false;
   Multipath_Detector mp_detector{ polys, object_tracks, tracker_info, calibrations };
   object_tracks[0].number_of_events_of_multipath_with_forgetting_factor = 1.0F;

   /** \action
   * Call tested function
   **/
   const bool test_result = Check_If_Object_Is_MP_For_Any_Sensor(sensors, tracker_info, object_tracks[0], mp_detector);

   /** \result
   * Returned false
   **/
   CHECK_FALSE(test_result);
}

/**
*\purpose Check if Check_If_Object_Is_MP_For_Any_Sensor() returns false when ghost object is out of sensors FOV
*\req    NA
*/
TEST(f360_detect_multipath__Check_If_Object_Is_MP_For_Any_Sensor, no_ghost_detected_due_to_no_in_FOV)
{
   /** \precond
   * Same as test group setup
   * Change ghost lateral position to 150.F
   **/
   F360_Object_Track_T &ghost_object = object_tracks[0];
   ghost_object.vcs_position.y = 150.0F;
   ghost_object.bbox.Set_Center(ghost_object.vcs_position); // Note: object reference_point was cet to CENTER in TEST_SETUP()
   Multipath_Detector mp_detector{ polys, object_tracks, tracker_info, calibrations };
   object_tracks[0].number_of_events_of_multipath_with_forgetting_factor = 1.0F;

   /** \action
   * Call tested function
   **/
   const bool test_result = Check_If_Object_Is_MP_For_Any_Sensor(sensors, tracker_info, object_tracks[0], mp_detector);

   /** \result
   * Returned false
   **/
   CHECK_FALSE(test_result);
}
/** @}*/


/** \defgroup  f360_detect_multipath__Detect_And_Mark_Multipath_Objects
 *  @{
 */

 /** \brief
 *  Test group of Detect_And_Mark_Multipath_Objects from f360_detect_multipath module
 **/
TEST_GROUP(f360_detect_multipath__Detect_And_Mark_Multipath_Object)
{
   const float32_t tolerance = 0.00001F;

   F360_Tracker_Info_T tracker_info = {};
   F360_Calibrations_T calibrations = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Radar_Sensor_Props_T sensor_props[MAX_NUMBER_OF_SENSORS] = {};
   Static_Env_Polys_Array polys = {};
   F360_Host_T host = {};

   TEST_SETUP()
   {
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

      for (int sensor_idx = 0; sensor_idx < MAX_NUMBER_OF_SENSORS; sensor_idx++)
      {
         Update_Sensor_FOV(sensors[sensor_idx]);
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
      ghost_object.number_of_events_of_multipath_with_forgetting_factor = 5.0F;
      ghost_object.time_since_initialization_with_forgetting_factor = 0.3F;
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

      tracker_info.num_active_objs = 3;
      tracker_info.active_obj_ids[0] = 1;
      tracker_info.active_obj_ids[1] = 2;
      tracker_info.active_obj_ids[2] = 3;
      Sorted_Tracks_Insert(tracker_info, &(ghost_object));
      Sorted_Tracks_Insert(tracker_info, &(source_object));
      Sorted_Tracks_Insert(tracker_info, &(reflector_object));
   }
};

/**
*\purpose Check if Detect_And_Mark_Multipath_Objects() change ghost object mirror probability to 1.0F for default setup
* (where ghost object is of CTCA type).
*\req    NA
*/
TEST(f360_detect_multipath__Detect_And_Mark_Multipath_Object, default_scenario_where_ghost_is_found__ghost_object_is_CTCA)
{
   /** \precond
   * Same as test group setup
   * Add Higher value to speed to pass into the function
   **/
   Multipath_Detector mp_detector{polys, object_tracks, tracker_info, calibrations};
   object_tracks[0].speed = calibrations.fast_moving_thresh + 1e-3F;
   /** \action
   * Call tested function
   **/
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[0], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[1], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[2], mp_detector);

   /** \result
   * Ghost object mirror probability should be set to 1.0F. The rest objects should have 0.0F
   **/
   DOUBLES_EQUAL(1.0F, object_tracks[0].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[1].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[2].mirror_prob, tolerance);
}

/**
*\purpose Check if Detect_And_Mark_Multipath_Objects() change ghost object mirror probability to 1.0F for default setup but where
* object type if changed to fast moving CCA
*\req    NA
*/
TEST(f360_detect_multipath__Detect_And_Mark_Multipath_Object, default_scenario_where_ghost_is_found__ghost_object_is_fast_moving_CCA)
{
   /** \precond
   * Same as test group setup except for:
   *    - Change ghost object filter type to CCA
   *    - Change ghost object speed to just above calib.fast_moving_thres
   **/
   Multipath_Detector mp_detector{polys, object_tracks, tracker_info, calibrations};
   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[0].speed = calibrations.fast_moving_thresh + 1e-3F;


   /** \action
   * Call tested function
   **/
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[0], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[1], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[2], mp_detector);

   /** \result
   * Ghost object mirror probability should be set to 1.0F. The rest objects should have 0.0F
   **/
   DOUBLES_EQUAL(1.0F, object_tracks[0].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[1].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[2].mirror_prob, tolerance);
}

/**
*\purpose Check if Detect_And_Mark_Multipath_Objects() change ghost object mirror probability to 1.0F for default setup but where
* object type if changed to fast moving CCA
*\req    NA
*/
TEST(f360_detect_multipath__Detect_And_Mark_Multipath_Object, default_scenario_where_real_object_marked_as_ghost)
{
   /** \precond
   * Same as test group setup except for:
   *    - Change ghost object filter type to CCA
   *    - Change ghost object speed to just above calib.fast_moving_thres
   *    - Change number of multipath events to 1
   **/
   Multipath_Detector mp_detector{polys, object_tracks, tracker_info, calibrations};
   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[0].speed = calibrations.fast_moving_thresh + 1e-3F;
   object_tracks[0].number_of_events_of_multipath_with_forgetting_factor = 1.0F;


   /** \action
   * Call tested function
   **/
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[0], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[1], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[2], mp_detector);

   /** \result
   * Ghost object mirror probability should be set to 0.0F. The rest objects should have 0.0F
   **/
   DOUBLES_EQUAL(0.0F, object_tracks[0].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[1].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[2].mirror_prob, tolerance);
}


/**
*\purpose Check such that Detect_And_Mark_Multipath_Objects() does not change ghost object mirror probability  when it is of the type slow moving CCA
*\req    NA
*/
TEST(f360_detect_multipath__Detect_And_Mark_Multipath_Object, ghost_not_found_due_to_ghost_object_is_slow_moving_CCA)
{
   /** \precond
   * Same as test group setup except for:
   *    - Change ghost object filter type to CCA
   *    - Change ghost object speed to just below calib.fast_moving_thres
   **/
   Multipath_Detector mp_detector{polys, object_tracks, tracker_info, calibrations};
   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[0].speed = calibrations.fast_moving_thresh - 1e-3F;


   /** \action
   * Call tested function
   **/
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[0], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[1], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[2], mp_detector);

   /** \result
   * All the objects should have 0.0F
   **/
   DOUBLES_EQUAL(0.0F, object_tracks[0].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[1].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[2].mirror_prob, tolerance);
}

/**
*\purpose Check if Detect_And_Mark_Multipath_Objects() does NOT change ghost object mirror probability due to fail in finding candidate
*\req    NA
*/
TEST(f360_detect_multipath__Detect_And_Mark_Multipath_Object, ghost_not_found_due_to_fail_in_finding_candidate)
{
   /** \precond
   * Same as test group setup
   * Change host speed above threshold
   **/
   Multipath_Detector mp_detector{polys, object_tracks, tracker_info, calibrations};
   host.speed = calibrations.k_mp_max_allowed_host_speed_to_use_MP + tolerance;

   /** \action
   * Call tested function
   **/
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[0], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[1], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[2], mp_detector);

   /** \result
   * All objects should have 0.0F mirror probability
   **/
   DOUBLES_EQUAL(0.0F, object_tracks[0].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[1].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[2].mirror_prob, tolerance);
}

/**
*\purpose  Check if Detect_And_Mark_Multipath_Objects() does NOT change ghost object mirror probability due to fail in MP detector
*\req    NA
*/
TEST(f360_detect_multipath__Detect_And_Mark_Multipath_Object, ghost_not_found_due_to_fail_in_MP_Detector)
{
   /** \precond
   * Same as test group setup
   * Change reflector lateral position in order to get false from MP detector
   **/
   Multipath_Detector mp_detector{polys, object_tracks, tracker_info, calibrations};
   F360_Object_Track_T &reflector_object = object_tracks[2];
   reflector_object.vcs_position.y = 200.0F;
   reflector_object.bbox.Set_Center(reflector_object.vcs_position); // Note: Object reference_point was set to CENTER in TEST_SETUP()

   /** \action
   * Call tested function
   **/
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[0], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[1], mp_detector);
   Detect_And_Mark_Multipath_Object(host, tracker_info, calibrations, sensors, object_tracks[2], mp_detector);

   /** \result
   * All objects should have 0.0F mirror probability
   **/
   DOUBLES_EQUAL(0.0F, object_tracks[0].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[1].mirror_prob, tolerance);
   DOUBLES_EQUAL(0.0F, object_tracks[2].mirror_prob, tolerance);
}
/** @}*/
