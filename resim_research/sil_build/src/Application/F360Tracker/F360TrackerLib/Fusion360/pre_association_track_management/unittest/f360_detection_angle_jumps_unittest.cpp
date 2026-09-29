/** \file
    Tests of angle jump detector (sep based) sub-module.
*/

#include "f360_detection_angle_jumps.h"
#include <CppUTest/TestHarness.h>


using namespace f360_variant_A;

/** \defgroup  f360_detection_angle_jumps
 *  @{
 */

/** \brief
 *  Overall test of Detect_Angle_Jumps(). Detailed UTs are done in f360_det_angle_jump_internals_unittest.cpp
 */
TEST_GROUP(f360_detection_angle_jumps)
{
   const float tolerance = 1e-3F;
   F360_Radar_Sensor_T       sensors[MAX_NUMBER_OF_SENSORS] = {};
   Static_Env_Poly_T         static_env_polys[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_Calibrations_T       calib = {};
   rspp_variant_A::RSPP_Detection_List_T     detections_raw = {};
   F360_Detection_Props_T    detections_props[MAX_NUMBER_OF_DETECTIONS] = {};

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

      detections_raw.number_of_valid_detections = 1;
      detections_raw.detections[0].raw.sensor_id = 1;
      detections_raw.detections[0].raw.range = 2.6F + tolerance;
      detections_raw.detections[0].raw.range_rate = 0.0F;
      detections_raw.detections[0].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_MIDHIGH;

      detections_raw.detections[0].processed.vcs_az = calib.k_angle_jump_min_abs_azimuth_vcs - tolerance;
      detections_props[0].on_sep_id = F360_INVALID_UNSIGNED_ID;
      detections_props[0].behind_sep_id = F360_INVALID_UNSIGNED_ID;
   }
};

/**
*\purpose  Check if detection is correctly marked as angle jump
*\req    NA
*/
TEST(f360_detection_angle_jumps, Detect_Angle_Jumps__Marked_detection)
{
   /** \precond
   * Same as setup
   */

   /** \action
   * Call Detect_Angle_Jumps()
   */
   Detect_Angle_Jumps(sensors, static_env_polys, detections_raw, calib, detections_props);

   /** \result
   * Detection marked as angle jump
   */
   CHECK_TRUE(detections_props[0].f_potential_angle_jump);
}

/**
*\purpose  Check if detection is NOT marked as angle jump due to range
*\req    NA
*/
TEST(f360_detection_angle_jumps, Detect_Angle_Jumps__NOT_marked_detection_due_to_range)
{
   /** \precond
   * Same as setup (plus changes below)
   * detection range set to high value
   */
   detections_raw.detections[0].raw.range = 10.0F;

   /** \action
   * Call Detect_Angle_Jumps()
   */
   Detect_Angle_Jumps(sensors, static_env_polys, detections_raw, calib, detections_props);

   /** \result
   * Detection NOT marked as angle jump
   */
   CHECK_FALSE(detections_props[0].f_potential_angle_jump);
}


/**
*\purpose  Check if detection is NOT marked as angle jump
*\req    NA
*/
TEST(f360_detection_angle_jumps, Detect_Angle_Jumps__NOT_marked_detection_due_to_invalid_sensor)
{
   /** \precond
   * Same as setup (plus changes below)
   * set sensor to invalid 
   */
   sensors[0].variable.is_valid = false;

   /** \action
   * Call Detect_Angle_Jumps()
   */
   Detect_Angle_Jumps(sensors, static_env_polys, detections_raw, calib, detections_props);

   /** \result
   * Detection NOT marked as angle jump
   */
   CHECK_FALSE(detections_props[0].f_potential_angle_jump);
}


/**
*\purpose  Check if detection is NOT marked as angle jump due to invalid seps
*\req    NA
*/
TEST(f360_detection_angle_jumps, Detect_Angle_Jumps__NOT_marked_detection_due_to_invalid_seps)
{
   /** \precond
   * Same as setup (plus changes below)
   * set first sep as invalid
   */
   static_env_polys[0].status = F360_STATIC_ENV_POLY_STATUS_INVALID;

   /** \action
   * Call Detect_Angle_Jumps()
   */
   Detect_Angle_Jumps(sensors, static_env_polys, detections_raw, calib, detections_props);

   /** \result
   * Detection NOT marked as angle jump
   */
   CHECK_FALSE(detections_props[0].f_potential_angle_jump);
}

/**
*\purpose  Check calibrations setup
*\req    NA
*/
TEST(f360_detection_angle_jumps, Detect_Angle_Jumps__calibrations)
{
   /** \precond
   * Same as setup (plus changes below)
   */

   /** \action
   * NA 
   */

   /** \result
   * Correct calibs
   */
   DOUBLES_EQUAL(1.0F, calib.k_angle_jump_range_tolerance, tolerance);
   DOUBLES_EQUAL(2.0F, calib.k_angle_jump_max_abs_range_rate, tolerance);
   DOUBLES_EQUAL(F360_DEG2RAD(40.0F), calib.k_angle_jump_min_abs_azimuth_vcs, tolerance);
   DOUBLES_EQUAL(F360_DEG2RAD(140.0F), calib.k_angle_jump_max_abs_azimuth_vcs, tolerance);
   DOUBLES_EQUAL(15.0F, calib.k_angle_jump_max_range, tolerance);
   DOUBLES_EQUAL(5.0F, calib.k_angle_jump_long_search_margin, tolerance);
}
/** @}*/
