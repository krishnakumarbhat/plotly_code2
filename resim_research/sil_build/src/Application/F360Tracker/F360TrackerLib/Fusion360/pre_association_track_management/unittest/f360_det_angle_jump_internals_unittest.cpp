/** \file
 * This file contains unit tests for content of f360_det_angle_jump_internals.cpp file
 */

#include "f360_det_angle_jump_internals.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_det_angle_jump_internals__Get_SEP_Info_Per_Sensor
 *  @{
 */

  /** \brief
   *  Test using this test group will test internal functions to detection angle jump algorithm. 
   */

TEST_GROUP(f360_det_angle_jump_internals__Get_SEP_Info_Per_Sensor)
{
   F360_Radar_Sensor_T       sensors[MAX_NUMBER_OF_SENSORS] = {};
   Static_Env_Poly_T         static_env_polys[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_Calibrations_T       calib = {};
   const float tolerance = 1e-3F;

   /** \setup
   * Set up required variables
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

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

      for (uint16_t sep_idx = 0U; sep_idx < F360_NUM_OF_STATIC_ENV_POLYS; sep_idx++)
      {
         static_env_polys[sep_idx].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
         static_env_polys[sep_idx].lower_limit = -20.0F;
         static_env_polys[sep_idx].upper_limit = 20.0F;
         static_env_polys[sep_idx].p2 = 0.0F;
         static_env_polys[sep_idx].p1 = 0.0F;

         if (0U == sep_idx)
         {
            static_env_polys[sep_idx].p0 = -4.0F;
         }
         else if (1U == sep_idx)
         {
            static_env_polys[sep_idx].p0 = 5.0F;
         }
         else
         {
            static_env_polys[sep_idx].p0 = 2.0F * static_env_polys[sep_idx - 2].p0;
         }
      }
   }
};

/**
*\purpose  Check if seps are valid
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Get_SEP_Info_Per_Sensor, Get_SEP_Info_Per_Sensor__seps_valid)
{
   /** \precond
   * Same as setup
   */
   Close_SEPs close_seps[MAX_NUMBER_OF_SENSORS] = {};

   /** \action
    * Call Get_SEP_Info_Per_Sensor()
    */
   Get_SEP_Info_Per_Sensor(sensors, static_env_polys, calib, close_seps);

   /** \result
    * All seps are valid for 4 sensors setup
    */
   for (uint16_t idx = 0; idx < 4; idx++)
   {
      CHECK_TRUE(close_seps[idx].first_closest.f_valid);
      CHECK_TRUE(close_seps[idx].second_closest.f_valid);
   }

   for (uint16_t idx = 4; idx < MAX_NUMBER_OF_SENSORS; idx++)
   {
      CHECK_FALSE(close_seps[idx].first_closest.f_valid);
      CHECK_FALSE(close_seps[idx].second_closest.f_valid);
   }
}

/**
*\purpose  Check if seps are valid (rest of sensors mounting locations)
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Get_SEP_Info_Per_Sensor, Get_SEP_Info_Per_Sensor__seps_valid_for_all_locations)
{

   /** \precond
   * Same as setup (plus below changes)
   * Set all possible mounting locations (differentt from group setup)
   */
   Close_SEPs close_seps[MAX_NUMBER_OF_SENSORS] = {};
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_SIDE1;
   sensors[1].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_SIDE2;
   sensors[2].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_SIDE1;
   sensors[3].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_SIDE2;   
   
   /** \action
    * Call Get_SEP_Info_Per_Sensor()
    */
   Get_SEP_Info_Per_Sensor(sensors, static_env_polys, calib, close_seps);

   /** \result
    * All seps are valid for 4 sensors setup
    */
   for (uint16_t idx = 0; idx < 4; idx++)
   {
      CHECK_TRUE(close_seps[idx].first_closest.f_valid);
      CHECK_TRUE(close_seps[idx].second_closest.f_valid);
   }

   for (uint16_t idx = 4; idx < MAX_NUMBER_OF_SENSORS; idx++)
   {
      CHECK_FALSE(close_seps[idx].first_closest.f_valid);
      CHECK_FALSE(close_seps[idx].second_closest.f_valid);
   }
}

/**
*\purpose  Check if only closest seps are valid
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Get_SEP_Info_Per_Sensor, Get_SEP_Info_Per_Sensor__only_closest_seps_valid)
{
   /** \precond
   * Same as setup (plus below changes)
   * Set further seps as invalid
   */
   Close_SEPs close_seps[MAX_NUMBER_OF_SENSORS] = {};
   static_env_polys[2].status = F360_STATIC_ENV_POLY_STATUS_INVALID;
   static_env_polys[3].status = F360_STATIC_ENV_POLY_STATUS_INVALID;
   static_env_polys[4].status = F360_STATIC_ENV_POLY_STATUS_INVALID;
   static_env_polys[5].status = F360_STATIC_ENV_POLY_STATUS_INVALID;

   /** \action
    * Call Get_SEP_Info_Per_Sensor()
    */
   Get_SEP_Info_Per_Sensor(sensors, static_env_polys, calib, close_seps);

   /** \result
    * Only closest seps are valid for 4 sensors setup
    */
   for (uint16_t idx = 0; idx < 4; idx++)
   {
      CHECK_TRUE(close_seps[idx].first_closest.f_valid);
   }

   for (uint16_t idx = 4; idx < MAX_NUMBER_OF_SENSORS; idx++)
   {
      CHECK_FALSE(close_seps[idx].first_closest.f_valid);
      CHECK_FALSE(close_seps[idx].second_closest.f_valid);
   }
}


/**
*\purpose  Check if max range is correct
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Get_SEP_Info_Per_Sensor, Get_SEP_Info_Per_Sensor__sensors_max_ranges)
{
   /** \precond
   * Same as setup
   */
   Close_SEPs close_seps[MAX_NUMBER_OF_SENSORS] = {};

   /** \action
    * Call Get_SEP_Info_Per_Sensor()
    */
   Get_SEP_Info_Per_Sensor(sensors, static_env_polys, calib, close_seps);

   /** \result
    * Correct max range computed as (dist_to_sep + range_tolerance)
    */
   DOUBLES_EQUAL(close_seps[0].first_closest.max_range, 4.6F, tolerance);
   DOUBLES_EQUAL(close_seps[0].second_closest.max_range, 8.6F, tolerance);
   DOUBLES_EQUAL(close_seps[1].first_closest.max_range, 4.6F, tolerance);
   DOUBLES_EQUAL(close_seps[1].second_closest.max_range, 8.6F, tolerance);
   DOUBLES_EQUAL(close_seps[2].first_closest.max_range, 5.6F, tolerance);
   DOUBLES_EQUAL(close_seps[2].second_closest.max_range, 10.6F, tolerance);
   DOUBLES_EQUAL(close_seps[3].first_closest.max_range, 5.6F, tolerance);
   DOUBLES_EQUAL(close_seps[3].second_closest.max_range, 10.6F, tolerance);
}


/**
*\purpose  Check if min_range is correct
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Get_SEP_Info_Per_Sensor, Get_SEP_Info_Per_Sensor__sensors_min_ranges)
{
   /** \precond
   * Same as setup
   */
   Close_SEPs close_seps[MAX_NUMBER_OF_SENSORS] = {};

   /** \action
    * Call Get_SEP_Info_Per_Sensor()
    */
   Get_SEP_Info_Per_Sensor(sensors, static_env_polys, calib, close_seps);

   /** \result
    * Correct min range computed as (dist_to_sep - range_tolerance)
    */
   DOUBLES_EQUAL(close_seps[0].first_closest.min_range, 2.6F, tolerance);
   DOUBLES_EQUAL(close_seps[0].second_closest.min_range, 6.6F, tolerance);
   DOUBLES_EQUAL(close_seps[1].first_closest.min_range, 2.6F, tolerance);
   DOUBLES_EQUAL(close_seps[1].second_closest.min_range, 6.6F, tolerance);
   DOUBLES_EQUAL(close_seps[2].first_closest.min_range, 3.6F, tolerance);
   DOUBLES_EQUAL(close_seps[2].second_closest.min_range, 8.6F, tolerance);
   DOUBLES_EQUAL(close_seps[3].first_closest.min_range, 3.6F, tolerance);
   DOUBLES_EQUAL(close_seps[3].second_closest.min_range, 8.6F, tolerance);
}


/**
*\purpose  Check if expected_range_rate is correct (sensors moving left)
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Get_SEP_Info_Per_Sensor, Get_SEP_Info_Per_Sensor__moving_left_correct_expected_range_rate)
{
   /** \precond
   * Same as setup (plus below changes)
   * Set sensor[0] velocity to -1.0 
   * Set sensor[2] velocity to -4.0
   */
   sensors[0].variable.vcs_velocity.lateral = -1.0F;
   sensors[2].variable.vcs_velocity.lateral = -4.0F;
   Close_SEPs close_seps[MAX_NUMBER_OF_SENSORS] = {};

   /** \action
    * Call Get_SEP_Info_Per_Sensor()
    */
   Get_SEP_Info_Per_Sensor(sensors, static_env_polys, calib, close_seps);

   /** \result
    * Expected range rate set to -1 and 4
    */
   DOUBLES_EQUAL(close_seps[0].first_closest.expected_range_rate, -1.0F, tolerance);
   DOUBLES_EQUAL(close_seps[0].second_closest.expected_range_rate, -1.0F, tolerance);
   DOUBLES_EQUAL(close_seps[2].first_closest.expected_range_rate, 4.0F, tolerance);
   DOUBLES_EQUAL(close_seps[2].second_closest.expected_range_rate, 4.0F, tolerance);
}


/**
*\purpose  Check if expected_range_rate is correct (sensors moving right)
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Get_SEP_Info_Per_Sensor, Get_SEP_Info_Per_Sensor__moving_right_correct_expected_range_rate)
{
   /** \precond
   * Same as setup (plus below changes)
   * Set sensor[0] velocity to 2.0
   * Set sensor[2] velocity to 5.0
   */
   sensors[0].variable.vcs_velocity.lateral = 2.0F;
   sensors[2].variable.vcs_velocity.lateral = 5.0F;
   Close_SEPs close_seps[MAX_NUMBER_OF_SENSORS] = {};

   /** \action
    * Call Get_SEP_Info_Per_Sensor()
    */
   Get_SEP_Info_Per_Sensor(sensors, static_env_polys, calib, close_seps);

   /** \result
    * Expected range rate set to 2 and -5
    */
   DOUBLES_EQUAL(close_seps[0].first_closest.expected_range_rate, 2.0F, tolerance);
   DOUBLES_EQUAL(close_seps[0].second_closest.expected_range_rate, 2.0F, tolerance);
   DOUBLES_EQUAL(close_seps[2].first_closest.expected_range_rate, -5.0F, tolerance);
   DOUBLES_EQUAL(close_seps[2].second_closest.expected_range_rate, -5.0F, tolerance);
}

/**
*\purpose  Check if chosen seps are NOT valid due to invalid seps
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Get_SEP_Info_Per_Sensor, Get_SEP_Info_Per_Sensor__Not_valid_seps)
{
   /** \precond
   * Same as setup (plus below changes)
   * Set all seps invalid
   */
   Close_SEPs close_seps[MAX_NUMBER_OF_SENSORS] = {};
   static_env_polys[0].status = F360_STATIC_ENV_POLY_STATUS_INVALID;
   static_env_polys[1].status = F360_STATIC_ENV_POLY_STATUS_INVALID;
   static_env_polys[2].status = F360_STATIC_ENV_POLY_STATUS_INVALID;
   static_env_polys[3].status = F360_STATIC_ENV_POLY_STATUS_INVALID;
   static_env_polys[4].status = F360_STATIC_ENV_POLY_STATUS_INVALID;
   static_env_polys[5].status = F360_STATIC_ENV_POLY_STATUS_INVALID;

   /** \action
    * Call Get_SEP_Info_Per_Sensor()
    */
   Get_SEP_Info_Per_Sensor(sensors, static_env_polys, calib, close_seps);

   /** \result
    * All invalid seps info
    */
   for (uint16_t idx = 0; idx < MAX_NUMBER_OF_SENSORS; idx++)
   {
      CHECK_FALSE(close_seps[idx].second_closest.f_valid);
   }
}

/**
*\purpose  Check if chosen SEP is NOT valid due to invalid sensors
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Get_SEP_Info_Per_Sensor, Get_SEP_Info_Per_Sensor__Not_valid_sensor)
{
   /** \precond
   * Same as setup (plus below changes)
   * Set all sensors invalid
   */
   Close_SEPs close_seps[MAX_NUMBER_OF_SENSORS] = {};
   for (uint16_t idx = 0; idx < MAX_NUMBER_OF_SENSORS; idx++)
   {
      sensors[idx].variable.is_valid = false;
   }

   /** \action
    * Call Get_SEP_Info_Per_Sensor()
    */
   Get_SEP_Info_Per_Sensor(sensors, static_env_polys, calib, close_seps);

   /** \result
    * All invalid seps info
    */
   for (uint16_t idx = 0; idx < MAX_NUMBER_OF_SENSORS; idx++)
   {
      CHECK_FALSE(close_seps[idx].first_closest.f_valid);
      CHECK_FALSE(close_seps[idx].second_closest.f_valid);
   }
}


/**
*\purpose  Check if chosen seps are NOT valid due to invalid locations
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Get_SEP_Info_Per_Sensor, Get_SEP_Info_Per_Sensor__Sensor_Invalid_locations)
{
   /** \precond
   * Same as setup (plus below changes)
   * Set all sensors' mouting location to invalid
   */
   Close_SEPs close_seps[MAX_NUMBER_OF_SENSORS] = {};
   sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER2_FORWARD;
   sensors[1].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER2_REAR;
   sensors[2].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;
   sensors[3].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_REAR;
   sensors[4].constant.mounting_location = F360_MOUNTING_LOCATION_UNKNOWN;

   /** \action
    * Call Get_SEP_Info_Per_Sensor()
    */
   Get_SEP_Info_Per_Sensor(sensors, static_env_polys, calib, close_seps);

   /** \result
    * All invalid seps info
    */
   for (uint16_t idx = 0; idx < MAX_NUMBER_OF_SENSORS; idx++)
   {
      CHECK_FALSE(close_seps[idx].first_closest.f_valid);
      CHECK_FALSE(close_seps[idx].second_closest.f_valid);
   }
}
/** @}*/


/** \defgroup  f360_det_angle_jump_internals__Detect_Angle_Jump_From_SEP
 *  @{
 */

 /** \brief
  *  Test using this test group will test internal functions to detection angle jump algorithm.
  */

TEST_GROUP(f360_det_angle_jump_internals__Detect_Angle_Jump_From_SEP)
{
   F360_Calibrations_T       calib = {};
   rspp_variant_A::RSPP_Detection_List_T     detections_raw = {};
   F360_Detection_Props_T    detections_props[MAX_NUMBER_OF_DETECTIONS] = {};
   Close_SEPs sep_info_per_sensor[MAX_NUMBER_OF_SENSORS] = {};
   float tolerance = 0.0001F;

   /** \setup
   * Set up required variables
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      sep_info_per_sensor[0].first_closest.f_valid = true;
      sep_info_per_sensor[0].first_closest.min_range = 1.0F;
      sep_info_per_sensor[0].first_closest.max_range = 2.0F;
      sep_info_per_sensor[0].first_closest.expected_range_rate = 0.4F;

      sep_info_per_sensor[0].second_closest.f_valid = true;
      sep_info_per_sensor[0].second_closest.min_range = 3.0F;
      sep_info_per_sensor[0].second_closest.max_range = 4.0F;
      sep_info_per_sensor[0].second_closest.expected_range_rate = 0.4F;

      detections_raw.number_of_valid_detections = 1;
      detections_raw.detections[0].raw.sensor_id = 1;
      detections_raw.detections[0].raw.range = sep_info_per_sensor[0].first_closest.min_range + tolerance;
      detections_raw.detections[0].raw.range_rate = sep_info_per_sensor[0].first_closest.expected_range_rate;
      detections_raw.detections[0].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_MIDHIGH;

      detections_raw.detections[0].processed.vcs_az = calib.k_angle_jump_min_abs_azimuth_vcs - tolerance;
      detections_props[0].on_sep_id = F360_INVALID_UNSIGNED_ID;
      detections_props[0].behind_sep_id = F360_INVALID_UNSIGNED_ID;
   }
};

/**
*\purpose  Check if detection is marked as angle jump (based on first sep)
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Detect_Angle_Jump_From_SEP, Detect_Angle_Jump_From_SEP__Marked_detection__first_sep)
{
   /** \precond
   * Same as group setup
   */

   /** \action
   * Call Detect_Angle_Jump_From_SEP()
   */
   Detect_Angle_Jump_From_SEP(sep_info_per_sensor, detections_raw, calib, detections_props);

   /** \result
    * Detection marked as angle jump
    */
   CHECK_TRUE(detections_props[0].f_potential_angle_jump);
}

/**
*\purpose  Check if detection is marked as angle jump (based on second sep)
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Detect_Angle_Jump_From_SEP, Detect_Angle_Jump_From_SEP__Marked_detection__second_sep)
{
   /** \precond
   * Same as setup (plus changes below)
   * detection range witihn second sep
   */
   detections_raw.detections[0].raw.range = sep_info_per_sensor[0].second_closest.max_range - tolerance;

   /** \action
   * Detect_Angle_Jump_From_SEP()
   */
   Detect_Angle_Jump_From_SEP(sep_info_per_sensor, detections_raw, calib, detections_props);

   /** \result
    * Detection marked as angle jump
    */
   CHECK_TRUE(detections_props[0].f_potential_angle_jump);
}


/**
*\purpose  Check if detection is marked as angle jump (detection negative azimuth)
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Detect_Angle_Jump_From_SEP, Detect_Angle_Jump_From_SEP__Marked_detection_negative_azimuthes)
{
   /** \precond
   * Same as setup (plus changes below)
   * negate detection azimuth (make it negative)
   */
   detections_raw.detections[0].raw.azimuth = F360_DEG2RAD(-30.0F);

   /** \action
   * Detect_Angle_Jump_From_SEP()
   */
   Detect_Angle_Jump_From_SEP(sep_info_per_sensor, detections_raw, calib, detections_props);

   /** \result
   * Detection marked as angle jump
   */
   CHECK_TRUE(detections_props[0].f_potential_angle_jump);
}


/**
*\purpose  Check if detection is NOT marked as angle jump due to too low range
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Detect_Angle_Jump_From_SEP, Detect_Angle_Jump_From_SEP__Not_marked_detection_due_to_range_low)
{
   /** \precond
   * Same as setup (plus changes below)
   * detection range just below first sep range
   */
   detections_raw.detections[0].raw.range = sep_info_per_sensor[0].first_closest.min_range - tolerance;
   
   /** \action
   * Detect_Angle_Jump_From_SEP()
   */
   Detect_Angle_Jump_From_SEP(sep_info_per_sensor, detections_raw, calib, detections_props);

   /** \result
   * Detection NOT marked as angle jump
   */
   CHECK_FALSE(detections_props[0].f_potential_angle_jump);
}


/**
*\purpose  Check if detection is NOT marked as angle jump due to too high range
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Detect_Angle_Jump_From_SEP, Detect_Angle_Jump_From_SEP__Not_marked_detection_due_to_range_high)
{
   /** \precond
   * Same as setup (plus changes below)
   * detection range just above second sep range
   */
   detections_raw.detections[0].raw.range = sep_info_per_sensor[0].second_closest.max_range + tolerance;

   /** \action
   * Detect_Angle_Jump_From_SEP()
   */
   Detect_Angle_Jump_From_SEP(sep_info_per_sensor, detections_raw, calib, detections_props);

   /** \result
   * Detection NOT marked as angle jump
   */
   CHECK_FALSE(detections_props[0].f_potential_angle_jump);
}


/**
*\purpose  Check if detection is NOT marked as angle jump due to not fitting range rate (positive case)
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Detect_Angle_Jump_From_SEP, Detect_Angle_Jump_From_SEP__Not_marked_detection_due_to_range_rate_positive)
{
   /** \precond
   * Same as setup (plus changes below)
   * detection range rate above first sep expected range rate
   */
   detections_raw.detections[0].raw.range_rate = sep_info_per_sensor[0].first_closest.expected_range_rate + calib.k_angle_jump_max_abs_range_rate + tolerance;

   /** \action
   * Detect_Angle_Jump_From_SEP()
   */
   Detect_Angle_Jump_From_SEP(sep_info_per_sensor, detections_raw, calib, detections_props);

   /** \result
   * Detection NOT marked as angle jump
   */
   CHECK_FALSE(detections_props[0].f_potential_angle_jump);
}


/**
*\purpose  Check if detection is NOT marked as angle jump due to not fitting range rate (negative case)
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Detect_Angle_Jump_From_SEP, Detect_Angle_Jump_From_SEP__Not_marked_detection_due_to_range_rate_negative)
{
   /** \precond
   * Same as setup (plus changes below)
   * detection range rate below first sep expected range rate and negate it
   */
   detections_raw.detections[0].raw.range_rate = -(sep_info_per_sensor[0].first_closest.expected_range_rate + calib.k_angle_jump_max_abs_range_rate + tolerance);

   /** \action
   * Detect_Angle_Jump_From_SEP()
   */
   Detect_Angle_Jump_From_SEP(sep_info_per_sensor, detections_raw, calib, detections_props);

   /** \result
   * Detection NOT marked as angle jump
   */
   CHECK_FALSE(detections_props[0].f_potential_angle_jump);
}


/**
*\purpose  Check if detection is NOT marked as angle jump due to wrong azimuth (being on lower bound)
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Detect_Angle_Jump_From_SEP, Detect_Angle_Jump_From_SEP__Not_marked_detection_due_to_azimuth__lower_bound)
{
   /** \precond
   * Same as setup (plus changes below)
   * detection vcs azimuth set to lower bound
   */
   detections_raw.detections[0].processed.vcs_az = calib.k_angle_jump_min_abs_azimuth_vcs;

   /** \action
   * Detect_Angle_Jump_From_SEP()
   */
   Detect_Angle_Jump_From_SEP(sep_info_per_sensor, detections_raw, calib, detections_props);

   /** \result
   * Detection NOT marked as angle jump
   */
   CHECK_FALSE(detections_props[0].f_potential_angle_jump);
}

/**
*\purpose  Check if detection is NOT marked as angle jump due to wrong azimuth (being on upper bound)
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Detect_Angle_Jump_From_SEP, Detect_Angle_Jump_From_SEP__Not_marked_detection_due_to_azimuth__upper_bound)
{
   /** \precond
   * Same as setup (plus changes below)
   * detection vcs azimuth set to upper bound
   */
   detections_raw.detections[0].processed.vcs_az = calib.k_angle_jump_max_abs_azimuth_vcs;

   /** \action
   * Detect_Angle_Jump_From_SEP()
   */
   Detect_Angle_Jump_From_SEP(sep_info_per_sensor, detections_raw, calib, detections_props);

   /** \result
   * Detection NOT marked as angle jump
   */
   CHECK_FALSE(detections_props[0].f_potential_angle_jump);
}

/**
*\purpose  Check if detection is marked as angle jump (vcs azimuth within bound)
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Detect_Angle_Jump_From_SEP, Detect_Angle_Jump_From_SEP__marked_detection___azimuth_within_bound)
{
   /** \precond
   * Same as setup (plus changes below)
   * detection vcs azimuth set to upper bound + tolerance
   */
   detections_raw.detections[0].processed.vcs_az = calib.k_angle_jump_max_abs_azimuth_vcs + tolerance;

   /** \action
   * Detect_Angle_Jump_From_SEP()
   */
   Detect_Angle_Jump_From_SEP(sep_info_per_sensor, detections_raw, calib, detections_props);

   /** \result
   * Detection marked as angle jump
   */
   CHECK_TRUE(detections_props[0].f_potential_angle_jump);
}


/**
*\purpose  Check if detection is NOT marked as angle jump due to being on sep
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Detect_Angle_Jump_From_SEP, Detect_Angle_Jump_From_SEP__Not_marked_detection_due_to_on_sep)
{
   /** \precond
   * Same as setup (plus changes below)
   * detection on sep
   */
   detections_props[0].on_sep_id = 1;

   /** \action
   * Detect_Angle_Jump_From_SEP()
   */
   Detect_Angle_Jump_From_SEP(sep_info_per_sensor, detections_raw, calib, detections_props);

   /** \result
   * Detection NOT marked as angle jump
   */
   CHECK_FALSE(detections_props[0].f_potential_angle_jump);
}

/**
*\purpose  Check if detection is NOT marked as angle jump due to too HIGH azimuth confidence
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Detect_Angle_Jump_From_SEP, Detect_Angle_Jump_From_SEP__Not_marked_detection_due_to_high_azimuth_confidence)
{
   /** \precond
   * Same as setup (plus changes below)
   * detection's azimuth confidence set to high
   */
   detections_raw.detections[0].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_HIGH;

   /** \action
   * Detect_Angle_Jump_From_SEP()
   */
   Detect_Angle_Jump_From_SEP(sep_info_per_sensor, detections_raw, calib, detections_props);

   /** \result
   * Detection NOT marked as angle jump
   */
   CHECK_FALSE(detections_props[0].f_potential_angle_jump);
}

/**
*\purpose  Check if detection is NOT marked as angle jump due to being behind sep
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Detect_Angle_Jump_From_SEP, Detect_Angle_Jump_From_SEP__Not_marked_detection_due_to_behind_sep)
{
   /** \precond
   * Same as setup (plus changes below)
   * detection behind sep
   */
   detections_props[0].behind_sep_id = 1;

   /** \action
   * Detect_Angle_Jump_From_SEP()
   */
   Detect_Angle_Jump_From_SEP(sep_info_per_sensor, detections_raw, calib, detections_props);

   /** \result
   * Detection NOT marked as angle jump
   */
   CHECK_FALSE(detections_props[0].f_potential_angle_jump);
}


/**
*\purpose  Check if detection is NOT marked as angle jump due to sep is invalid
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Detect_Angle_Jump_From_SEP, Detect_Angle_Jump_From_SEP__Not_marked_detection_due_to_invalid_sep)
{
   /** \precond
   * Same as setup (plus changes below)
   * set first sep as invalid
   */
   sep_info_per_sensor[0].first_closest.f_valid = false;

   /** \action
   * Detect_Angle_Jump_From_SEP()
   */
   Detect_Angle_Jump_From_SEP(sep_info_per_sensor, detections_raw, calib, detections_props);

   /** \result
   * Detection NOT marked as angle jump
   */
   CHECK_FALSE(detections_props[0].f_potential_angle_jump);
}

/**
*\purpose  Check if detection is NOT marked as angle jump due to being too far away (same as max threshold)
*\req    NA
*/
TEST(f360_det_angle_jump_internals__Detect_Angle_Jump_From_SEP, Detect_Angle_Jump_From_SEP__Not_marked_detection_due_to_being_on_max_range)
{
   /** \precond
   * Same as setup (plus changes below)
   * set detection to max threshold
   * set max range within first sep
   */
   calib.k_angle_jump_max_range = (sep_info_per_sensor[0].first_closest.min_range + sep_info_per_sensor[0].first_closest.max_range) * 0.5F;
   detections_raw.detections[0].raw.range = calib.k_angle_jump_max_range;

   /** \action
   * Detect_Angle_Jump_From_SEP()
   */
   Detect_Angle_Jump_From_SEP(sep_info_per_sensor, detections_raw, calib, detections_props);

   /** \result
   * Detection NOT marked as angle jump
   */
   CHECK_FALSE(detections_props[0].f_potential_angle_jump);
}
/** @}*/