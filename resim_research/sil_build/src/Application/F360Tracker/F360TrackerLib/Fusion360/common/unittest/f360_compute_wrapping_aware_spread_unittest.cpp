/** \file
 * This file contains unit tests for content of f360_compute_wrapping_aware_spread.cpp file
 */

#include "f360_compute_wrapping_aware_spread.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_compute_wrapping_aware_spread
 *  @{
 */

/** \brief
 * Test group for Compute_Wrapping_Aware_Spread function which computes the Doppler spread
 * between two raw range rates, accounting for Doppler wrapping when both detections
 * originate from the same sensor.
 */
TEST_GROUP(f360_compute_wrapping_aware_spread)
{
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   rspp_variant_A::RSPP_Detection_T det1 = {};
   rspp_variant_A::RSPP_Detection_T det2 = {};
   const float32_t tolerance = 1e-6F;
   float32_t rng_rate_1;
   float32_t rng_rate_2;

   /** \setup
    * Initialize sensors and detections to default zero-initialized state.
    * Sensor 1 is configured with realistic v_wrapping = 25.0 m/s at look_id 0.
    */
   TEST_SETUP()
   {
      sensors[0].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[0].constant.v_wrapping[0] = 25.0F;
      sensors[1].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[1].constant.v_wrapping[0] = 25.0F;
      det1.raw.sensor_id = 1;
      det2.raw.sensor_id = 1;

      rng_rate_1 = 1.0F;
      rng_rate_2 = 24.0F;
   }
};

/**
 * \purpose  Verify that when both detections are from different sensors, the raw spread is returned
 *           without wrapping correction even when v_wrapping is set.
 * \req    NA
 */
TEST(f360_compute_wrapping_aware_spread, Compute_Wrapping_Aware_Spread__Different_Sensors_Returns_Raw_Spread)
{
   /** \precond
    * Detection 1 from sensor 1, detection 2 from sensor 2 (different sensors).
    * Both sensors have v_wrapping = 25.0. Range rates: 1.0 and 24.0, raw spread = 23.0.
    */
   det2.raw.sensor_id = 2;

   /** \action
    * Call Compute_Wrapping_Aware_Spread with range rates 1.0 and 24.0.
    */
   const float32_t spread = Compute_Wrapping_Aware_Spread(rng_rate_1, rng_rate_2, det1, det2, sensors);

   /** \result
    * Spread should be the raw spread 23.0 (no wrapping correction for different sensors).
    */
   DOUBLES_EQUAL(23.0F, spread, tolerance);
}

/**
 * \purpose  Verify that when both detections are from the same sensor but v_wrapping is zero
 *           (erroneous input; not expected at runtime but guarded against), the raw spread
 *           is returned without wrapping correction.
 * \req    NA
 */
TEST(f360_compute_wrapping_aware_spread, Compute_Wrapping_Aware_Spread__Same_Sensor_Zero_V_Wrapping_Guard)
{
   /** \precond
    * Both detections from sensor 1 with v_wrapping overridden to 0.0 (erroneous input guard).
    * Range rates: 1.0 and 24.0, raw spread = 23.0.
    */
   sensors[0].constant.v_wrapping[0] = 0.0F;

   /** \action
    * Call Compute_Wrapping_Aware_Spread with range rates 1.0 and 24.0.
    */
   const float32_t spread = Compute_Wrapping_Aware_Spread(rng_rate_1, rng_rate_2, det1, det2, sensors);

   /** \result
    * Spread should be the raw spread 23.0 (zero v_wrapping guard prevents wrapping correction).
    */
   DOUBLES_EQUAL(23.0F, spread, tolerance);
}

/**
 * \purpose  Verify that for same-sensor detections the smaller Doppler spread is returned.
 * \req    NA
 */
TEST(f360_compute_wrapping_aware_spread, Compute_Wrapping_Aware_Spread__Same_Sensor_Remainder_Smaller)
{
   /** \precond
    * Both detections from sensor 1 with v_wrapping = 25.0.
    * Range rates: 0.0 and 3.0, raw_spread = 3.0.
    * Since both detections are from the same sensor, raw_spread < v_wrap holds.
    * remainder = 3.0, spread = min(3.0, 25.0 - 3.0) = min(3.0, 22.0) = 3.0.
    */
   rng_rate_1 = 0.0F;
   rng_rate_2 = 3.0F;
   
   /** \action
    * Call Compute_Wrapping_Aware_Spread with range rates 0.0 and 3.0.
    */
   const float32_t spread = Compute_Wrapping_Aware_Spread(rng_rate_1, rng_rate_2, det1, det2, sensors);

   /** \result
    * Spread should be 3.0 (remainder is smaller than v_wrap - remainder).
    */
   DOUBLES_EQUAL(3.0F, spread, tolerance);
}

/**
 * \purpose  Verify that for same-sensor detections the corrected spread (v_wrap - remainder)
 *           is returned when v_wrap minus remainder is smaller than the remainder.
 * \req    NA
 */
TEST(f360_compute_wrapping_aware_spread, Compute_Wrapping_Aware_Spread__Same_Sensor_Complement_Smaller)
{
   /** \precond
    * Both detections from sensor 1 with v_wrapping = 25.0.
    * Range rates: 0.0 and 19.0, raw_spread = 19.0.
    * Since both detections are from the same sensor, raw_spread < v_wrap holds.
    * remainder = 19.0, spread = min(19.0, 25.0 - 19.0) = min(19.0, 6.0) = 6.0.
    */
   rng_rate_1 = 0.0F;
   rng_rate_2 = 19.0F;

   /** \action
    * Call Compute_Wrapping_Aware_Spread with range rates 0.0 and 19.0.
    */
   const float32_t spread = Compute_Wrapping_Aware_Spread(rng_rate_1, rng_rate_2, det1, det2, sensors);

   /** \result
    * Spread should be 6.0 (v_wrap - remainder is smaller, indicating rates are close via wrapping).
    */
   DOUBLES_EQUAL(6.0F, spread, tolerance);
}

/**
 * \purpose  Verify that for same-sensor detections whose raw spread is close to the wrapping interval
 *           boundary (spread near v_wrapping, above half_wrap), the corrected spread is computed correctly.
 * \req    NA
 */
TEST(f360_compute_wrapping_aware_spread, Compute_Wrapping_Aware_Spread__Same_Sensor_Spread_Near_Interval_Boundary)
{
   /** \precond
    * Both detections from sensor 1 with v_wrapping = 25.0.
    * Range rates: 0.0 and 24.5, raw_spread = 24.5 (near maximum realistic spread for same-sensor detections).
    * Since both detections are from the same sensor, raw_spread < v_wrap holds.
    * remainder = 24.5, spread = min(24.5, 25.0 - 24.5) = min(24.5, 0.5) = 0.5.
    */
   rng_rate_1 = 0.0F;
   rng_rate_2 = 24.5F;

   /** \action
    * Call Compute_Wrapping_Aware_Spread with range rates 0.0 and 24.5.
    */
   const float32_t spread = Compute_Wrapping_Aware_Spread(rng_rate_1, rng_rate_2, det1, det2, sensors);

   /** \result
    * Spread should be 0.5 (v_wrap - remainder is smaller, indicating the two rates are close via circular distance).
    */
   DOUBLES_EQUAL(0.5F, spread, tolerance);
}

/** @}*/

/** \defgroup  f360_compensate_det_range
 *  @{
 */

/** \brief
 * Test group for F360_Compensate_Det_Range function which updates range_dealiased and
 * vcs_position of a detection after range rate dealiasing.
 */
TEST_GROUP(f360_compensate_det_range)
{
   rspp_variant_A::RSPP_Detection_T detection = {};
   F360_Detection_Props_T det_prop = {};
   const float32_t tolerance = 1e-4F;
   float32_t dealiasing_interval;
   float32_t r_wrapping;

   /** \setup
    * Initialize detection and detection_prop to default zero-initialized state.
    * Set detection raw range to 20.0 m, vcs position to (5.0, 3.0),
    * and azimuth direction aligned with x-axis (cos=1, sin=0).
    */
   TEST_SETUP()
   {
      detection.raw.range = 20.0F;
      detection.processed.cos_vcs_az = 1.0F;
      detection.processed.sin_vcs_az = 0.0F;

      r_wrapping = 0.5F;

      det_prop.vcs_position.Set_Position(5.0F, 3.0F);
   }
};

/** \purpose
 * Verify that when dealiasing_interval is zero, range_dealiased equals raw range and
 * vcs_position remains unchanged.
 * \req
 * NA
 */
TEST(f360_compensate_det_range, F360_Compensate_Det_Range__Zero_Interval_No_Change)
{
   /** \precond
    * detection.raw.range = 20.0, r_wrapping = 0.5, dealiasing_interval = 0.
    * delta_range = 0.5 * 0 = 0. Initial vcs_position = (5.0, 3.0).
    */
   dealiasing_interval = 0.0F;

   /** \action
    * Call F360_Compensate_Det_Range with dealiasing_interval = 0.
    */
   F360_Compensate_Det_Range(dealiasing_interval, r_wrapping, detection, det_prop);

   /** \result
    * range_dealiased = 20.0, vcs_position = (5.0, 3.0) — no change.
    */
   DOUBLES_EQUAL(20.0F, det_prop.range_dealiased, tolerance);
   DOUBLES_EQUAL(5.0F, det_prop.vcs_position.x, tolerance);
   DOUBLES_EQUAL(3.0F, det_prop.vcs_position.y, tolerance);
}

/** \purpose
 * Verify that a positive dealiasing_interval increases range_dealiased and displaces
 * vcs_position in the detection azimuth direction.
 * \req
 * NA
 */
TEST(f360_compensate_det_range, F360_Compensate_Det_Range__Positive_Interval_Updates_Range_And_Position)
{
   /** \precond
    * detection.raw.range = 20.0, r_wrapping = 0.5, dealiasing_interval = 2.
    * delta_range = 0.5 * 2 = 1.0. cos_vcs_az = 1.0, sin_vcs_az = 0.0.
    * Initial vcs_position = (5.0, 3.0). Expected: range_dealiased = 21.0, pos = (6.0, 3.0).
    */
   dealiasing_interval = 2.0F;

   /** \action
    * Call F360_Compensate_Det_Range with dealiasing_interval = 2.
    */
   F360_Compensate_Det_Range(dealiasing_interval, r_wrapping, detection, det_prop);

   /** \result
    * range_dealiased = 21.0, vcs_position shifted by 1.0 along x-axis to (6.0, 3.0).
    */
   DOUBLES_EQUAL(21.0F, det_prop.range_dealiased, tolerance);
   DOUBLES_EQUAL(6.0F, det_prop.vcs_position.x, tolerance);
   DOUBLES_EQUAL(3.0F, det_prop.vcs_position.y, tolerance);
}

/** \purpose
 * Verify that a negative dealiasing_interval decreases range_dealiased and displaces
 * vcs_position in the opposite direction to the detection azimuth.
 * \req
 * NA
 */
TEST(f360_compensate_det_range, F360_Compensate_Det_Range__Negative_Interval_Updates_Range_And_Position)
{
   /** \precond
    * detection.raw.range = 20.0, r_wrapping = 0.5, dealiasing_interval = -1.
    * delta_range = 0.5 * (-1) = -0.5. cos_vcs_az = 1.0, sin_vcs_az = 0.0.
    * Initial vcs_position = (5.0, 3.0). Expected: range_dealiased = 19.5, pos = (4.5, 3.0).
    */
   dealiasing_interval = -1.0F;

   /** \action
    * Call F360_Compensate_Det_Range with dealiasing_interval = -1.
    */
   F360_Compensate_Det_Range(dealiasing_interval, r_wrapping, detection, det_prop);

   /** \result
    * range_dealiased = 19.5, vcs_position shifted by -0.5 along x-axis to (4.5, 3.0).
    */
   DOUBLES_EQUAL(19.5F, det_prop.range_dealiased, tolerance);
   DOUBLES_EQUAL(4.5F, det_prop.vcs_position.x, tolerance);
   DOUBLES_EQUAL(3.0F, det_prop.vcs_position.y, tolerance);
}

/** \purpose
 * Verify that vcs_position is updated correctly when the detection azimuth has both
 * lateral and longitudinal components.
 * \req
 * NA
 */
TEST(f360_compensate_det_range, F360_Compensate_Det_Range__Diagonal_Azimuth_Updates_Both_Axes)
{
   /** \precond
    * detection.raw.range = 10.0, r_wrapping = 0.5, dealiasing_interval = 1.
    * delta_range = 0.5. cos_vcs_az = 0.6, sin_vcs_az = 0.8 (3-4-5 triangle).
    * Initial vcs_position = (0.0, 0.0).
    * Expected: range_dealiased = 10.5, pos.x = 0.3, pos.y = 0.4.
    */
   dealiasing_interval = 1.0F;
   detection.raw.range = 10.0F;
   detection.processed.cos_vcs_az = 0.6F;
   detection.processed.sin_vcs_az = 0.8F;
   det_prop.vcs_position.Set_Position(0.0F, 0.0F);

   /** \action
    * Call F360_Compensate_Det_Range with a diagonal azimuth direction.
    */
   F360_Compensate_Det_Range(dealiasing_interval, r_wrapping, detection, det_prop);

   /** \result
    * range_dealiased = 10.5, vcs_position = (0.3, 0.4).
    */
   DOUBLES_EQUAL(10.5F, det_prop.range_dealiased, tolerance);
   DOUBLES_EQUAL(0.3F, det_prop.vcs_position.x, tolerance);
   DOUBLES_EQUAL(0.4F, det_prop.vcs_position.y, tolerance);
}

/** @}*/
