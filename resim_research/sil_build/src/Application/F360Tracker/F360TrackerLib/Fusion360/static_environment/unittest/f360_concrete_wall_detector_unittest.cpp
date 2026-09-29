/** \file
 * This file contains unit tests for content of f360_concrete_wall_detector.cpp file
 */

#include "f360_concrete_wall_detector.h"
#include <CppUTest/TestHarness.h>
#include "f360_vcs_long_sorted_dets_support_functions.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_concrete_wall_detector
 *  @{
 */

/** \brief
 * The concrete wall detector shall report an estimated distance to a potential concrete wall.
 * It shall be able to support cases when there are one or more sensors mounted on each side of
 * the host vehicle. It shall be able to determine if measurements are too noisy to create a
 * reliable estimate.
 */
TEST_GROUP(f360_concrete_wall_detector)
{
   // Declare common variables used within all tests in this test group.
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Host_T host = {};
   F360_Tracker_Info_T tracker_info = {};
   CWD_Data_T cwd_data = {};
   Static_Env_Poly_T static_env_polys[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_TRKR_TIMING_INFO_T timing_info;

   TEST_SETUP()
   {
      host.speed = 2.0F;

      sensors[0].variable.is_valid = true;
      sensors[1].variable.is_valid = true;
      sensors[2].variable.is_valid = true;
      sensors[3].variable.is_valid = true;

      sensors[0].variable.number_of_valid_detections = 1;
      sensors[1].variable.number_of_valid_detections = 1;
      sensors[2].variable.number_of_valid_detections = 1;
      sensors[3].variable.number_of_valid_detections = 1;

      sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = F360_DEG2RAD(62.0F);
      sensors[1].constant.mounting_position.vcs_boresight_azimuth_angle = F360_DEG2RAD(-62.0F);
      sensors[2].constant.mounting_position.vcs_boresight_azimuth_angle = F360_DEG2RAD(120.0F);
      sensors[3].constant.mounting_position.vcs_boresight_azimuth_angle = F360_DEG2RAD(-120.0F);

      sensors[0].constant.mounting_position.vcs_position.longitudinal = 0.0F;
      sensors[1].constant.mounting_position.vcs_position.longitudinal = 0.0F;
      sensors[2].constant.mounting_position.vcs_position.longitudinal = -4.0F;
      sensors[3].constant.mounting_position.vcs_position.longitudinal = -4.0F;

      raw_detect_list.number_of_valid_detections = 4;

      raw_detect_list.detections[0].raw.rcs = -10.0F;
      raw_detect_list.detections[1].raw.rcs = -10.0F;
      raw_detect_list.detections[2].raw.rcs = -10.0F;
      raw_detect_list.detections[3].raw.rcs = -10.0F;
      raw_detect_list.detections[0].raw.sensor_id = 1;
      raw_detect_list.detections[1].raw.sensor_id = 2;
      raw_detect_list.detections[2].raw.sensor_id = 3;
      raw_detect_list.detections[3].raw.sensor_id = 4;
      det_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
      det_props[1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
      det_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
      det_props[3].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;

      det_props[0].f_ok_to_use = true;
      det_props[1].f_ok_to_use = true;
      det_props[2].f_ok_to_use = true;
      det_props[3].f_ok_to_use = true;
      det_props[0].vcs_position.y = 5.0F;
      raw_detect_list.detections[0].processed.vcs_position_y = 5.0F;
      det_props[1].vcs_position.y = -6.0F;
      raw_detect_list.detections[1].processed.vcs_position_y = -6.0F;
      det_props[2].vcs_position.y = 8.0F;
      raw_detect_list.detections[2].processed.vcs_position_y = 8.0F;
      det_props[3].vcs_position.y = -10.0F;
      raw_detect_list.detections[3].processed.vcs_position_y = -10.0F;
      det_props[0].vcs_position.x = 0.0F;
      raw_detect_list.detections[0].processed.vcs_position_x = 0.0F;
      det_props[1].vcs_position.x = 0.0F;
      raw_detect_list.detections[1].processed.vcs_position_x = 0.0F;
      det_props[2].vcs_position.x = -4.0F;
      raw_detect_list.detections[2].processed.vcs_position_x = -4.0F;
      det_props[3].vcs_position.x = -4.0F;
      raw_detect_list.detections[3].processed.vcs_position_x = -4.0F;
      det_props[0].object_track_id = 0;
      det_props[1].object_track_id = 0;
      det_props[2].object_track_id = 0;
      det_props[3].object_track_id = 0;
   }
};

/** \purpose
 * Verify that the function reports invalid status when there are no detections and no host speed.
 * \req NA
 */
TEST(f360_concrete_wall_detector, Run_with_no_detections)
{
   /** \precond
    * Inputs are zero.
    */
   raw_detect_list.number_of_valid_detections = 0;
   host.speed = 0.0F;
   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Run_CWD()
    */
   Run_CWD(det_props, raw_detect_list, sensors, object_tracks, host, tracker_info, cwd_data, static_env_polys, timing_info);

   /** \result
    * Expect the status to be set invalid.
    */
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_INVALID, static_env_polys[6].status);
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_INVALID, static_env_polys[7].status);
}

/** \purpose
 * Verify that the function reports invalid status when all detections are behind the host vehicle.
 * \req NA
 */
TEST(f360_concrete_wall_detector, Run_with_detections_behind_host_vehicle)
{
   /** \precond
    * All detection x positions are less than -4m.
    */
   det_props[0].vcs_position.x = -4.0F;
   raw_detect_list.detections[0].processed.vcs_position_x = -4.0F;
   det_props[1].vcs_position.x = -16.0F;
   raw_detect_list.detections[1].processed.vcs_position_x = -16.0F;
   det_props[2].vcs_position.x = -27.0F;
   raw_detect_list.detections[2].processed.vcs_position_x = -27.0F;
   det_props[3].vcs_position.x = -38.0F;
   raw_detect_list.detections[3].processed.vcs_position_x = -38.0F;
   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Run_CWD()
    */
   Run_CWD(det_props, raw_detect_list, sensors, object_tracks, host, tracker_info, cwd_data, static_env_polys, timing_info);

   /** \result
    * Expect the status to be set invalid.
    */
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_INVALID, static_env_polys[6].status);
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_INVALID, static_env_polys[7].status);
}

/** \purpose
 * Test that the function reports an expected output for ideal inputs.
 * \req NA
 */
TEST(f360_concrete_wall_detector, Run_with_consistent_detections)
{
   /** \precond
    * Inputs are valid and unchanged from one iteration to the next
    */
   raw_detect_list.number_of_valid_detections = 2;
   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Run_CWD() 5 times
    */
   for (int32_t i = 0; i < 5; i++)
   {
      Run_CWD(det_props, raw_detect_list, sensors, object_tracks, host, tracker_info, cwd_data, static_env_polys, timing_info);
   }

   /** \result
    * Check that status is updated and the p0 value corresponds to the detection coordinates.
    */
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_UPDATED, static_env_polys[6].status);
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_UPDATED, static_env_polys[7].status);
   DOUBLES_EQUAL(-6.0F, static_env_polys[6].p0, 0.01F)
   DOUBLES_EQUAL(5.0F, static_env_polys[7].p0, 0.01F)
}

/** \purpose
 * Test that the function reports an expected output for ideal inputs.
 * \req NA
 */
TEST(f360_concrete_wall_detector, Run_with_consistent_detections_on_highway_CV)
{
   /** \precond
    * Inputs are valid and unchanged from one iteration to the next
    */
   raw_detect_list.number_of_valid_detections = 2;
   Sort_Detections_Vcs_Long(raw_detect_list);

   tracker_info.f_highway_suspected = true;
   host.speed = 20.0F;

   /** \action
    * Call Run_CWD() 5 times
    */
   for (int32_t i = 0; i < 5; i++)
   {
      Run_CWD(det_props, raw_detect_list, sensors, object_tracks, host, tracker_info, cwd_data, static_env_polys, timing_info);
   }

   /** \result
    * Check that status is updated and the p0 value corresponds to the detection coordinates.
    */
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_UPDATED, static_env_polys[6].status);
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_UPDATED, static_env_polys[7].status);
   DOUBLES_EQUAL(-6.0F, static_env_polys[6].p0, 0.01F)
   DOUBLES_EQUAL(5.0F, static_env_polys[7].p0, 0.01F)
   CHECK_TRUE(static_env_polys[6].upper_limit >= 40.0F)
}

/** \purpose
 * Test that the function reports an invalid status when the detection inputs have too much noise.
 * \req NA
 */
TEST(f360_concrete_wall_detector, Run_with_noisy_detections)
{
   /** \precond
    * Limit the number of detections to 2. Start the sorted detection list on index 1. Set the magnitude of the lateral shift.
    */
   raw_detect_list.number_of_valid_detections = 2;
   Sort_Detections_Vcs_Long(raw_detect_list);
   float32_t det_shift = 2.0F;

   /** \action
    * Call Run_CWD() 5 times, shifting the detections laterally between each execution
    */
   for (int32_t i = 0; i < 5; i++)
   {
      Run_CWD(det_props, raw_detect_list, sensors, object_tracks, host, tracker_info, cwd_data, static_env_polys, timing_info);
      det_props[0].vcs_position.y += det_shift;
      det_props[1].vcs_position.y += det_shift;
      det_shift *= -1.0F;
   }

   /** \result
    * Expect the status to be set invalid
    */
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_INVALID, static_env_polys[6].status);
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_INVALID, static_env_polys[7].status);
}

/** \purpose
 * Test that the function reports the closer estimate when two sensors on the same side reports significantly different results.
 * \req NA
 */
TEST(f360_concrete_wall_detector, Run_with_inconsistent_sensors)
{
   /** \precond
    * Set configuration according to test setup, 4 sensors with detections at varying lateral offset.
    */
   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Run_CWD() 5 times
    */
   for (int32_t i = 0; i < 5; i++)
   {
      Run_CWD(det_props, raw_detect_list, sensors, object_tracks, host, tracker_info, cwd_data, static_env_polys, timing_info);
   }

   /** \result
    * Expect the closest estimate to be chosen
    */
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_UPDATED, static_env_polys[6].status);
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_UPDATED, static_env_polys[7].status);
   DOUBLES_EQUAL(-6.0F, static_env_polys[6].p0, 0.01F)
   DOUBLES_EQUAL(5.0F, static_env_polys[7].p0, 0.01F)
}

/** \purpose
 * Test that the function reports invalid status when detections are too noisy and sensors report different results.
 * \req NA
 */
TEST(f360_concrete_wall_detector, Run_with_inconsistent_noisy_sensors)
{
   /** \precond
    * Set configuration according to test setup, 4 sensors with detections at varying lateral offset. Set the magnitude of the lateral shift.
    */
   float32_t det_shift = 0.6F;
   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Run_CWD() 5 times, shifting the detections laterally between each execution
    */
   for (int32_t i = 0; i < 5; i++)
   {
      Run_CWD(det_props, raw_detect_list, sensors, object_tracks, host, tracker_info, cwd_data, static_env_polys, timing_info);
      det_props[0].vcs_position.y += det_shift;
      det_props[1].vcs_position.y += det_shift;
      det_shift *= -1.0F;
   }

   /** \result
    * Expect the status to be set invalid
    */
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_INVALID, static_env_polys[6].status);
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_INVALID, static_env_polys[7].status);
}

/** \purpose
 * Test that the function correctly rejects detections outside the zone of interest or with certain properties.
 * \req NA
 */
TEST(f360_concrete_wall_detector, Run_with_less_than_perfect_detections)
{
   /** \precond
    * Set configuration according to test setup. Add additional detections.
    */
   raw_detect_list.number_of_valid_detections = 8;
   raw_detect_list.detections[4].raw.rcs = -10.0F;
   raw_detect_list.detections[5].raw.rcs = 10.0F;
   raw_detect_list.detections[6].raw.rcs = -10.0F;
   raw_detect_list.detections[7].raw.rcs = -10.0F;
   raw_detect_list.detections[4].raw.sensor_id = 1;
   raw_detect_list.detections[5].raw.sensor_id = 1;
   raw_detect_list.detections[6].raw.sensor_id = 1;
   raw_detect_list.detections[7].raw.sensor_id = 1;
   raw_detect_list.detections[4].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   raw_detect_list.detections[5].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   raw_detect_list.detections[6].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   raw_detect_list.detections[7].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;

   raw_detect_list.detections[0].processed.next_sorted_idx = 4;
   raw_detect_list.detections[4].processed.next_sorted_idx = 5;
   raw_detect_list.detections[5].processed.next_sorted_idx = 6;
   raw_detect_list.detections[6].processed.next_sorted_idx = 7;
   raw_detect_list.detections[7].processed.next_sorted_idx = -1;

   det_props[3].f_double_bounce = true;
   det_props[4].f_ok_to_use = true;
   det_props[5].f_ok_to_use = true;
   det_props[6].f_ok_to_use = true;
   det_props[7].f_ok_to_use = true;
   det_props[4].vcs_position.y = 5.0F;
   det_props[5].vcs_position.y = 5.0F;
   det_props[6].vcs_position.y = 5.0F;
   det_props[7].vcs_position.y = 5.0F;
   det_props[4].vcs_position.x = 0.01F;
   det_props[5].vcs_position.x = 0.05F;
   det_props[6].vcs_position.x = 5.0F;
   det_props[7].vcs_position.x = 10.0F;

   det_props[3].object_track_id = 1;
   object_tracks[0].movable_prob = 0.0F;
   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
    * Call Run_CWD() 5 times
    */
   for (int32_t i = 0; i < 5; i++)
   {
      Run_CWD(det_props, raw_detect_list, sensors, object_tracks, host, tracker_info, cwd_data, static_env_polys, timing_info);
   }

   /** \result
    * Expect the closest estimate to be chosen
    */
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_UPDATED, static_env_polys[6].status);
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_UPDATED, static_env_polys[7].status);
   DOUBLES_EQUAL(-6.0F, static_env_polys[6].p0, 0.01F)
   DOUBLES_EQUAL(5.0F, static_env_polys[7].p0, 0.01F)
}

/**
 *\purpose
 * Test that wall creation is blocked if a moving detection is within the blocking region for both left and right walls.
 *\req NA
 */
TEST(f360_concrete_wall_detector, Run_with_moving_detection_blocks_wall_creation)
{
   /** \precond
    * Valid ambiguous detections for both walls, moving detections within block region for both.
    */
   raw_detect_list.number_of_valid_detections = 4;

   // Right wall (index 0, y=5.0)
   det_props[0].f_ok_to_use = true;
   det_props[0].vcs_position.y = 5.0F;
   det_props[0].vcs_position.x = 0.0F;
   det_props[0].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   raw_detect_list.detections[0].raw.sensor_id = 1;
   raw_detect_list.detections[0].raw.rcs = -10.0F;

   // Left wall (index 1, y=-6.0)
   det_props[1].f_ok_to_use = true;
   det_props[1].vcs_position.y = -6.0F;
   det_props[1].vcs_position.x = 0.0F;
   det_props[1].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   raw_detect_list.detections[1].raw.sensor_id = 2;
   raw_detect_list.detections[1].raw.rcs = -10.0F;

   // Moving detection near right wall (should block right wall)
   det_props[2].f_ok_to_use = true;
   det_props[2].vcs_position.y = 5.3F;  // within 0.5m of 5.0
   det_props[2].vcs_position.x = -2.0F; // within 0 to -5m
   det_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   raw_detect_list.detections[2].raw.sensor_id = 1;
   raw_detect_list.detections[2].raw.rcs = -10.0F;

   // Moving detection near left wall (should block left wall)
   det_props[3].f_ok_to_use = true;
   det_props[3].vcs_position.y = -6.2F; // within 0.5m of -6.0
   det_props[3].vcs_position.x = -3.0F; // within 0 to -5m
   det_props[3].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   raw_detect_list.detections[3].raw.sensor_id = 2;
   raw_detect_list.detections[3].raw.rcs = -10.0F;

   Sort_Detections_Vcs_Long(raw_detect_list);
   host.speed = 10.0F;

   /** \action
    * Call Run_CWD() 5 times to fill buffer
    */
   for (int32_t i = 0; i < 5; i++)
      Run_CWD(det_props, raw_detect_list, sensors, object_tracks, host, tracker_info, cwd_data, static_env_polys, timing_info);

   /** \result
    * Both left and right wall outputs should be blocked (status INVALID).
    */
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_INVALID, static_env_polys[6].status); // left wall
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_INVALID, static_env_polys[7].status); // right wall
}

/**
 *\purpose
 * Test that wall creation is not blocked if a moving detection is outside the lateral blocking region.
 *\req NA
 */
TEST(f360_concrete_wall_detector, Run_with_moving_detection_outside_lateral_does_not_block)
{
   /** \precond
    * Valid ambiguous detection for right wall, moving detection outside lateral block region.
    */
   raw_detect_list.number_of_valid_detections = 2;

   det_props[0].f_ok_to_use = true;
   det_props[0].vcs_position.y = 5.0F;
   det_props[0].vcs_position.x = 0.0F;
   raw_detect_list.detections[0].raw.sensor_id = 1;
   raw_detect_list.detections[0].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   raw_detect_list.detections[0].raw.rcs = -10.0F;

   // Moving detection, but lateral distance > 0.5m from wall
   det_props[1].f_ok_to_use = true;
   det_props[1].vcs_position.y = 6.0F;  // 1.0m away from 5.0
   det_props[1].vcs_position.x = -2.0F; // within 0 to -5m
   raw_detect_list.detections[1].raw.sensor_id = 1;
   raw_detect_list.detections[1].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   raw_detect_list.detections[1].raw.rcs = -10.0F;

   Sort_Detections_Vcs_Long(raw_detect_list);
   host.speed = 10.0F;

   /** \action
    * Call Run_CWD() 5 times to fill buffer
    */
   for (int32_t i = 0; i < 5; i++)
      Run_CWD(det_props, raw_detect_list, sensors, object_tracks, host, tracker_info, cwd_data, static_env_polys, timing_info);

   /** \result
    * Wall output should be created (status UPDATED).
    */
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_UPDATED, static_env_polys[7].status); // right wall
}

/**
 *\purpose
 * Test that wall creation is not blocked if a moving detection is outside the longitudinal blocking region.
 *\req NA
 */
TEST(f360_concrete_wall_detector, Run_with_moving_detection_outside_longitudinal_does_not_block)
{
   /** \precond
    * Valid ambiguous detection for right wall, moving detection outside longitudinal block region.
    */
   raw_detect_list.number_of_valid_detections = 2;

   det_props[0].f_ok_to_use = true;
   det_props[0].vcs_position.y = 5.0F;
   det_props[0].vcs_position.x = 0.0F;
   raw_detect_list.detections[0].raw.sensor_id = 1;
   raw_detect_list.detections[0].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   raw_detect_list.detections[0].raw.rcs = -10.0F;

   // Moving detection, but longitudinal position < -5m (outside block region)
   det_props[1].f_ok_to_use = true;
   det_props[1].vcs_position.y = 5.2F;  // within 0.5m
   det_props[1].vcs_position.x = -6.0F; // < -5m
   raw_detect_list.detections[1].raw.sensor_id = 1;
   raw_detect_list.detections[1].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   raw_detect_list.detections[1].raw.rcs = -10.0F;

   Sort_Detections_Vcs_Long(raw_detect_list);
   host.speed = 10.0F;

   /** \action
    * Call Run_CWD() 5 times to fill buffer
    */
   for (int32_t i = 0; i < 5; i++)
      Run_CWD(det_props, raw_detect_list, sensors, object_tracks, host, tracker_info, cwd_data, static_env_polys, timing_info);

   /** \result
    * Wall output should be created (status UPDATED).
    */
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_UPDATED, static_env_polys[7].status); // right wall
}

/**
 *\purpose
 * Test that wall creation is not blocked if there is no moving detection.
 *\req NA
 */
TEST(f360_concrete_wall_detector, Run_with_no_moving_detection_wall_created)
{
   /** \precond
    * Valid ambiguous detection for right wall, no moving detection.
    */
   raw_detect_list.number_of_valid_detections = 1;

   det_props[0].f_ok_to_use = true;
   det_props[0].vcs_position.y = 5.0F;
   det_props[0].vcs_position.x = 0.0F;
   raw_detect_list.detections[0].raw.sensor_id = 1;
   raw_detect_list.detections[0].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   raw_detect_list.detections[0].raw.rcs = -10.0F;

   Sort_Detections_Vcs_Long(raw_detect_list);
   host.speed = 10.0F;

   /** \action
    * Call Run_CWD() 5 times to fill buffer
    */
   for (int32_t i = 0; i < 5; i++)
      Run_CWD(det_props, raw_detect_list, sensors, object_tracks, host, tracker_info, cwd_data, static_env_polys, timing_info);

   /** \result
    * Wall output should be created (status UPDATED).
    */
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_UPDATED, static_env_polys[7].status); // right wall
}

/**
 *\purpose
 * Test that wall creation is not blocked if a moving detection is present but on the opposite wall side.
 *\req NA
 */
TEST(f360_concrete_wall_detector, Run_with_moving_detection_on_opposite_side_does_not_block)
{
   /** \precond
    * Valid ambiguous detection for right wall, moving detection on left wall side.
    */
   raw_detect_list.number_of_valid_detections = 2;

   det_props[0].f_ok_to_use = true;
   det_props[0].vcs_position.y = 5.0F;
   det_props[0].vcs_position.x = 0.0F;
   raw_detect_list.detections[0].raw.sensor_id = 1;
   raw_detect_list.detections[0].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   raw_detect_list.detections[0].raw.rcs = -10.0F;

   // Moving detection on left wall side (should not block right wall)
   det_props[1].f_ok_to_use = true;
   det_props[1].vcs_position.y = -6.0F;
   det_props[1].vcs_position.x = -2.0F;
   raw_detect_list.detections[1].raw.sensor_id = 2;
   raw_detect_list.detections[1].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   raw_detect_list.detections[1].raw.rcs = -10.0F;

   Sort_Detections_Vcs_Long(raw_detect_list);
   host.speed = 10.0F;

   /** \action
    * Call Run_CWD() 5 times to fill buffer
    */
   for (int32_t i = 0; i < 5; i++)
      Run_CWD(det_props, raw_detect_list, sensors, object_tracks, host, tracker_info, cwd_data, static_env_polys, timing_info);

   /** \result
    * Wall output should be created (status UPDATED).
    */
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_UPDATED, static_env_polys[7].status); // right wall
}

/**
 *\purpose
 * Test that wall creation is blocked if a reduced track (object track) is within the blocking region for both left and right walls.
 *\req NA
 */
TEST(f360_concrete_wall_detector, Run_with_reduced_track_blocks_wall_creation)
{
   /** \precond
    * Valid ambiguous detections for both walls, reduced tracks within block region for both.
    */
   raw_detect_list.number_of_valid_detections = 2;

   // Right wall (index 0, y=5.0)
   det_props[0].f_ok_to_use = true;
   det_props[0].vcs_position.y = 5.0F;
   det_props[0].vcs_position.x = 0.0F;
   raw_detect_list.detections[0].raw.sensor_id = 1;
   raw_detect_list.detections[0].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   raw_detect_list.detections[0].raw.rcs = -10.0F;

   // Left wall (index 1, y=-6.0)
   det_props[1].f_ok_to_use = true;
   det_props[1].vcs_position.y = -6.0F;
   det_props[1].vcs_position.x = 0.0F;
   raw_detect_list.detections[1].raw.sensor_id = 2;
   raw_detect_list.detections[1].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   raw_detect_list.detections[1].raw.rcs = -10.0F;
   
   Sort_Detections_Vcs_Long(raw_detect_list);
   host.speed = 10.0F;

   // Add reduced tracks within block region for both walls
   tracker_info.num_active_objs = 2;
   // Right wall block: y ~ 5.0, x in [0, -5], lat_diff <= 0.25, movable_prob > 0.5, reduced_status > INVALID
   object_tracks[0].vcs_position.y = 5.1F;
   object_tracks[0].vcs_position.x = -2.0F;
   object_tracks[0].movable_prob = 0.8F;
   object_tracks[0].reduced_status = F360_OBJECT_STATUS_UPDATED;
   // Left wall block: y ~ -6.0, x in [0, -5], lat_diff <= 0.25, movable_prob > 0.5, reduced_status > INVALID
   object_tracks[1].vcs_position.y = -5.9F;
   object_tracks[1].vcs_position.x = -3.0F;
   object_tracks[1].movable_prob = 1.0F;
   object_tracks[1].reduced_status = F360_OBJECT_STATUS_NEW_UPDATED;

   /** \action
    * Call Run_CWD() 5 times to fill buffer
    */
   for (int32_t i = 0; i < 5; i++)
      Run_CWD(det_props, raw_detect_list, sensors, object_tracks, host, tracker_info, cwd_data, static_env_polys, timing_info);

   /** \result
    * Both left and right wall outputs should be blocked (status INVALID).
    */
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_INVALID, static_env_polys[6].status); // left wall
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_INVALID, static_env_polys[7].status); // right wall
}

/**
 *\purpose
 * Test that wall creation is not blocked if a reduced track is outside the lateral blocking region.
 *\req NA
 */
TEST(f360_concrete_wall_detector, Run_with_reduced_track_outside_lateral_does_not_block)
{
   /** \precond
    * Valid ambiguous detection for right wall, reduced track outside lateral block region.
    */
   raw_detect_list.number_of_valid_detections = 1;

   det_props[0].f_ok_to_use = true;
   det_props[0].vcs_position.y = 5.0F;
   det_props[0].vcs_position.x = 0.0F;
   raw_detect_list.detections[0].raw.sensor_id = 1;
   raw_detect_list.detections[0].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   raw_detect_list.detections[0].raw.rcs = -10.0F;

   Sort_Detections_Vcs_Long(raw_detect_list);
   host.speed = 10.0F;

   tracker_info.num_active_objs = 1;
   // Lateral difference > 0.25m, so should not block
   object_tracks[0].vcs_position.y = 5.5F;
   object_tracks[0].vcs_position.x = -2.0F;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].reduced_status = F360_OBJECT_STATUS_UPDATED;

   /** \action
    * Call Run_CWD() 5 times to fill buffer
    */
   for (int32_t i = 0; i < 5; i++)
      Run_CWD(det_props, raw_detect_list, sensors, object_tracks, host, tracker_info, cwd_data, static_env_polys, timing_info);

   /** \result
    * Wall output should be created (status UPDATED).
    */
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_UPDATED, static_env_polys[7].status); // right wall
}

/**
 *\purpose
 * Test that wall creation is not blocked if a reduced track is outside the longitudinal blocking region.
 *\req NA
 */
TEST(f360_concrete_wall_detector, Run_with_reduced_track_outside_longitudinal_does_not_block)
{
   /** \precond
    * Valid ambiguous detection for right wall, reduced track outside longitudinal block region.
    */
   raw_detect_list.number_of_valid_detections = 1;

   det_props[0].f_ok_to_use = true;
   det_props[0].vcs_position.y = 5.0F;
   det_props[0].vcs_position.x = 0.0F;
   raw_detect_list.detections[0].raw.sensor_id = 1;
   raw_detect_list.detections[0].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   raw_detect_list.detections[0].raw.rcs = -10.0F;

   Sort_Detections_Vcs_Long(raw_detect_list);
   host.speed = 10.0F;

   tracker_info.num_active_objs = 1;
   // Longitudinal position < -5m, so should not block
   object_tracks[0].vcs_position.y = 5.0F;
   object_tracks[0].vcs_position.x = -6.0F;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].reduced_status = F360_OBJECT_STATUS_UPDATED;

   /** \action
    * Call Run_CWD() 5 times to fill buffer
    */
   for (int32_t i = 0; i < 5; i++)
      Run_CWD(det_props, raw_detect_list, sensors, object_tracks, host, tracker_info, cwd_data, static_env_polys, timing_info);

   /** \result
    * Wall output should be created (status UPDATED).
    */
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_UPDATED, static_env_polys[7].status); // right wall
}

/**
 *\purpose
 * Test that wall creation is not blocked if a reduced track is present but is not "movable" (movable_prob <= 0.5).
 *\req NA
 */
TEST(f360_concrete_wall_detector, Run_with_non_movable_reduced_track_does_not_block)
{
   /** \precond
    * Valid ambiguous detection for right wall, reduced track in block region but not movable.
    */
   raw_detect_list.number_of_valid_detections = 1;

   det_props[0].f_ok_to_use = true;
   det_props[0].vcs_position.y = 5.0F;
   det_props[0].vcs_position.x = 0.0F;
   raw_detect_list.detections[0].raw.sensor_id = 1;
   raw_detect_list.detections[0].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   raw_detect_list.detections[0].raw.rcs = -10.0F;

   Sort_Detections_Vcs_Long(raw_detect_list);
   host.speed = 10.0F;

   tracker_info.num_active_objs = 1;
   // In block region, but movable_prob <= 0.5, so should not block
   object_tracks[0].vcs_position.y = 5.0F;
   object_tracks[0].vcs_position.x = -2.0F;
   object_tracks[0].movable_prob = 0.5F;
   object_tracks[0].reduced_status = F360_OBJECT_STATUS_UPDATED;

   /** \action
    * Call Run_CWD() 5 times to fill buffer
    */
   for (int32_t i = 0; i < 5; i++)
      Run_CWD(det_props, raw_detect_list, sensors, object_tracks, host, tracker_info, cwd_data, static_env_polys, timing_info);

   /** \result
    * Wall output should be created (status UPDATED).
    */
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_UPDATED, static_env_polys[7].status); // right wall
}

/**
 *\purpose
 * Test that wall creation is not blocked if a reduced track is present but has reduced_status == INVALID.
 *\req NA
 */
TEST(f360_concrete_wall_detector, Run_with_invalid_reduced_status_does_not_block)
{
   /** \precond
    * Valid ambiguous detection for right wall, reduced track in block region but reduced_status == INVALID.
    */
   raw_detect_list.number_of_valid_detections = 1;

   det_props[0].f_ok_to_use = true;
   det_props[0].vcs_position.y = 5.0F;
   det_props[0].vcs_position.x = 0.0F;
   raw_detect_list.detections[0].raw.sensor_id = 1;
   raw_detect_list.detections[0].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
   raw_detect_list.detections[0].raw.rcs = -10.0F;

   Sort_Detections_Vcs_Long(raw_detect_list);
   host.speed = 10.0F;

   tracker_info.num_active_objs = 1;
   // In block region, but reduced_status == INVALID, so should not block
   object_tracks[0].vcs_position.y = 5.0F;
   object_tracks[0].vcs_position.x = -2.0F;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].reduced_status = F360_OBJECT_STATUS_INVALID;

   /** \action
    * Call Run_CWD() 5 times to fill buffer
    */
   for (int32_t i = 0; i < 5; i++)
      Run_CWD(det_props, raw_detect_list, sensors, object_tracks, host, tracker_info, cwd_data, static_env_polys, timing_info);

   /** \result
    * Wall output should be created (status UPDATED).
    */
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_UPDATED, static_env_polys[7].status); // right wall
}
/** @}*/
