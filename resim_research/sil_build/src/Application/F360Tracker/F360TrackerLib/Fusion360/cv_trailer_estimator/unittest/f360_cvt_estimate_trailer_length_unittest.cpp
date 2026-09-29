/** \file
 * This file contains unit tests for content of f360_cvt_estimate_trailer_length.cpp file
 */

#include "f360_cvt_estimate_trailer_length.h"
#include "f360_math.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_cvt_estimate_trailer_length
 *  @{
 */

/** \brief
 * Test group for Run_Length_Filter() in CV trailer estimator.
 */
TEST_GROUP(f360_cvt_estimate_trailer_length_unittest)
{
   F360_Calibrations_T calibrations{};
   F360_CVT_State_T cvt_state{};
   float32_t epsilon;

   // Function to compute low-pass weight for given number of updates, using same logic as in Run_Length_Filter(). This is to help compute expected values in tests.
   float32_t compute_low_pass_weight(const int32_t n_updates) const
   {
      constexpr int32_t min_n_updates_low_pass = 35;
      const float32_t length_lp_sig_slope = 0.5F;
      const float32_t length_lp_gains[2] = { 0.1F, 0.005F };

      const int32_t n_updates_over_min = n_updates - min_n_updates_low_pass;
      float32_t weight_low_pass = 1.0F / (F360_Expf(static_cast<float32_t>(n_updates_over_min) * -length_lp_sig_slope) + 1.0F);
      weight_low_pass = ((1.0F - weight_low_pass) * length_lp_gains[0]) + (length_lp_gains[1] * weight_low_pass);
      return weight_low_pass;
   }

   /** \setup
    * Initialize common variables before each test.
    */
   TEST_SETUP()
   {
      calibrations.k_cvt_trailer_width = 2.55F;
      epsilon = 1E-4F;
   }
};

/** \purpose
 * Verify one-link low-pass update and clamping when measurement is valid.
 * \req
 * NA
 */
TEST(f360_cvt_estimate_trailer_length_unittest, one_link_is_updated_and_clamped_from_higher_bound_when_measurement_is_valid)
{
   /** \precond
    * Set one-link state so raw measurement exceeds max bound.
    */
   cvt_state.one_link.n_updates = 0;
   cvt_state.one_link.full_vehicle_length = 20.0F;
   cvt_state.one_link.joint_vcs_longpos = -3.0F;
   cvt_state.one_link.joint_dist_to_wheels = 28.1F; // To clamp at k_max_veh_len

   cvt_state.two_link.full_vehicle_length = 17.0F; // Should remain unchanged in this test

   const float32_t expected_weight = compute_low_pass_weight(cvt_state.one_link.n_updates);
   const float32_t expected_msmt_after_clamp = 34.5F;
   const float32_t expected_full_vehicle_length = ((1.0F - expected_weight) * 20.0F) + (expected_weight * expected_msmt_after_clamp);
   const float32_t expected_full_trailer_length = expected_full_vehicle_length + cvt_state.one_link.joint_vcs_longpos + 1.7F;
   const float32_t expected_joint_dist_to_center = (expected_full_trailer_length / 2.0F) - 1.7F;
   const bool valid_measurement_ekf_1 = true;
   const bool valid_measurement_ekf_2 = false;

   /** \action
    * Call Run_Length_Filter().
    */
   Run_Length_Filter(calibrations, valid_measurement_ekf_1, valid_measurement_ekf_2, cvt_state);

   /** \result
    * Verify one-link update, one-link geometry and trailer width.
    */
   DOUBLES_EQUAL_TEXT(expected_full_vehicle_length, cvt_state.one_link.full_vehicle_length, epsilon, "Unexpected one-link full_vehicle_length");
   DOUBLES_EQUAL_TEXT(expected_full_trailer_length, cvt_state.one_link.trailer_length, epsilon, "Unexpected one-link trailer_length");
   DOUBLES_EQUAL_TEXT(expected_joint_dist_to_center, cvt_state.one_link.joint_dist_to_center, epsilon, "Unexpected one-link joint_dist_to_center");
   DOUBLES_EQUAL_TEXT(calibrations.k_cvt_trailer_width, cvt_state.one_link.trailer_width, epsilon, "Unexpected one-link trailer_width");

   // Two-link full vehicle length shall remain unchanged when measurement is invalid.
   DOUBLES_EQUAL_TEXT(17.0F, cvt_state.two_link.full_vehicle_length, epsilon, "two-link full_vehicle_length should stay unchanged");
}

/** \purpose
 * Verify one-link length is not low-pass updated when measurement is invalid,
 * but derived one-link geometry is still refreshed.
 * \req
 * NA
 */
TEST(f360_cvt_estimate_trailer_length_unittest, one_link_is_not_updated_when_measurement_is_invalid)
{
   /** \precond
    * Set one-link state to known values.
    */
   cvt_state.one_link.full_vehicle_length = 22.0F;
   cvt_state.one_link.joint_vcs_longpos = -2.0F;
   cvt_state.one_link.joint_dist_to_wheels = 5.0F;

   const float32_t expected_full_trailer_length = cvt_state.one_link.full_vehicle_length + cvt_state.one_link.joint_vcs_longpos + 1.7F;
   const float32_t expected_joint_dist_to_center = (expected_full_trailer_length / 2.0F) - 1.7F;
   const bool valid_measurement_ekf_1 = false;
   const bool valid_measurement_ekf_2 = false;

   /** \action
    * Call Run_Length_Filter().
    */
   Run_Length_Filter(calibrations, valid_measurement_ekf_1, valid_measurement_ekf_2, cvt_state);

   /** \result
    * Verify full_vehicle_length unchanged and derived values updated.
    */
   DOUBLES_EQUAL_TEXT(22.0F, cvt_state.one_link.full_vehicle_length, epsilon, "one-link full_vehicle_length should stay unchanged");
   DOUBLES_EQUAL_TEXT(expected_full_trailer_length, cvt_state.one_link.trailer_length, epsilon, "Unexpected one-link trailer_length");
   DOUBLES_EQUAL_TEXT(expected_joint_dist_to_center, cvt_state.one_link.joint_dist_to_center, epsilon, "Unexpected one-link joint_dist_to_center");
   DOUBLES_EQUAL_TEXT(calibrations.k_cvt_trailer_width, cvt_state.one_link.trailer_width, epsilon, "Unexpected one-link trailer_width");
}

/** \purpose
 * Verify two-link low-pass update and clamping when measurement is valid.
 * \req
 * NA
 */
TEST(f360_cvt_estimate_trailer_length_unittest, two_link_is_updated_and_clamped_from_lower_bound_when_measurement_is_valid)
{
   /** \precond
    * Set two-link state so raw full carriage measurement is below minimum bound.
    */
   cvt_state.two_link.n_updates = 90;
   cvt_state.two_link.full_vehicle_length = 30.0F;
   cvt_state.two_link.joint1_vcs_longpos = -1.0F;
   cvt_state.two_link.joint1_dist_to_wheels = 2.0F;
   cvt_state.two_link.joint2_dist_to_wheels = 3.4F;  // Such that the overall full carriage length measurement is 9.9 m < 10 m

   const float32_t expected_weight = compute_low_pass_weight(cvt_state.two_link.n_updates);
   const float32_t expected_msmt_after_clamp = 10.0F;
   const float32_t expected_full_vehicle_length = ((1.0F - expected_weight) * 30.0F) + (expected_weight * expected_msmt_after_clamp);

   const float32_t expected_trailer1_length = 1.7F + cvt_state.two_link.joint1_dist_to_wheels;
   const float32_t expected_trailer2_length = cvt_state.two_link.joint2_dist_to_wheels + 3.5F;
   const float32_t expected_joint1_dist_to_center = (expected_trailer1_length / 2.0F) - 1.7F;
   const float32_t expected_joint2_dist_to_center = (expected_trailer2_length / 2.0F) - 1.7F;
   const bool valid_measurement_ekf_1 = false;
   const bool valid_measurement_ekf_2 = true;

   /** \action
    * Call Run_Length_Filter().
    */
   Run_Length_Filter(calibrations, valid_measurement_ekf_1, valid_measurement_ekf_2, cvt_state);

   /** \result
    * Verify two-link update and geometry outputs.
    */
   DOUBLES_EQUAL_TEXT(expected_full_vehicle_length, cvt_state.two_link.full_vehicle_length, epsilon, "Unexpected two-link full_vehicle_length");

   DOUBLES_EQUAL_TEXT(expected_trailer1_length, cvt_state.two_link.trailer1_length, epsilon, "Unexpected trailer1_length");
   DOUBLES_EQUAL_TEXT(expected_joint1_dist_to_center, cvt_state.two_link.joint1_dist_to_center, epsilon, "Unexpected joint1_dist_to_center");
   DOUBLES_EQUAL_TEXT(calibrations.k_cvt_trailer_width, cvt_state.two_link.trailer1_width, epsilon, "Unexpected trailer1_width");

   DOUBLES_EQUAL_TEXT(expected_trailer2_length, cvt_state.two_link.trailer2_length, epsilon, "Unexpected trailer2_length");
   DOUBLES_EQUAL_TEXT(expected_joint2_dist_to_center, cvt_state.two_link.joint2_dist_to_center, epsilon, "Unexpected joint2_dist_to_center");
   DOUBLES_EQUAL_TEXT(calibrations.k_cvt_trailer_width, cvt_state.two_link.trailer2_width, epsilon, "Unexpected trailer2_width");
}

/** \purpose
 * Verify two-link low-pass update and clamping when measurement is valid.
 * \req
 * NA
 */
TEST(f360_cvt_estimate_trailer_length_unittest, two_link_is_updated_and_clamped_from_higher_bound_when_measurement_is_valid)
{
   /** \precond
    * Set two-link state so raw full carriage measurement is above maximum bound.
    */
   cvt_state.two_link.n_updates = 67;
   cvt_state.two_link.full_vehicle_length = 30.0F;
   cvt_state.two_link.joint1_vcs_longpos = -1.0F;
   cvt_state.two_link.joint1_dist_to_wheels = 2.0F;
   cvt_state.two_link.joint2_dist_to_wheels = 28.1F;  // Such that the overall full carriage length measurement is 34.6 m > 34.5 m

   const float32_t expected_weight = compute_low_pass_weight(cvt_state.two_link.n_updates);
   const float32_t expected_msmt_after_clamp = 34.5F;
   const float32_t expected_full_vehicle_length = ((1.0F - expected_weight) * 30.0F) + (expected_weight * expected_msmt_after_clamp);

   const float32_t expected_trailer1_length = 1.7F + cvt_state.two_link.joint1_dist_to_wheels;
   const float32_t expected_trailer2_length = cvt_state.two_link.joint2_dist_to_wheels + 3.5F;
   const float32_t expected_joint1_dist_to_center = (expected_trailer1_length / 2.0F) - 1.7F;
   const float32_t expected_joint2_dist_to_center = (expected_trailer2_length / 2.0F) - 1.7F;
   const bool valid_measurement_ekf_1 = false;
   const bool valid_measurement_ekf_2 = true;

   /** \action
    * Call Run_Length_Filter().
    */
   Run_Length_Filter(calibrations, valid_measurement_ekf_1, valid_measurement_ekf_2, cvt_state);

   /** \result
    * Verify two-link update and geometry outputs.
    */
   DOUBLES_EQUAL_TEXT(expected_full_vehicle_length, cvt_state.two_link.full_vehicle_length, epsilon, "Unexpected two-link full_vehicle_length");

   DOUBLES_EQUAL_TEXT(expected_trailer1_length, cvt_state.two_link.trailer1_length, epsilon, "Unexpected trailer1_length");
   DOUBLES_EQUAL_TEXT(expected_joint1_dist_to_center, cvt_state.two_link.joint1_dist_to_center, epsilon, "Unexpected joint1_dist_to_center");
   DOUBLES_EQUAL_TEXT(calibrations.k_cvt_trailer_width, cvt_state.two_link.trailer1_width, epsilon, "Unexpected trailer1_width");

   DOUBLES_EQUAL_TEXT(expected_trailer2_length, cvt_state.two_link.trailer2_length, epsilon, "Unexpected trailer2_length");
   DOUBLES_EQUAL_TEXT(expected_joint2_dist_to_center, cvt_state.two_link.joint2_dist_to_center, epsilon, "Unexpected joint2_dist_to_center");
   DOUBLES_EQUAL_TEXT(calibrations.k_cvt_trailer_width, cvt_state.two_link.trailer2_width, epsilon, "Unexpected trailer2_width");
}

/** \purpose
 * Verify two-link full vehicle length is not updated when measurement is invalid,
 * while geometry outputs are still refreshed from kinematic distances.
 * \req
 * NA
 */
TEST(f360_cvt_estimate_trailer_length_unittest, two_link_is_not_updated_when_measurement_is_invalid)
{
   /** \precond
    * Set two-link state to known values.
    */
   cvt_state.two_link.full_vehicle_length = 12.0F;
   cvt_state.two_link.joint1_dist_to_wheels = 6.0F;
   cvt_state.two_link.joint2_dist_to_wheels = 4.0F;

   const float32_t expected_trailer1_length = 1.7F + cvt_state.two_link.joint1_dist_to_wheels;
   const float32_t expected_trailer2_length = cvt_state.two_link.joint2_dist_to_wheels + 3.5F;
   const float32_t expected_joint1_dist_to_center = (expected_trailer1_length / 2.0F) - 1.7F;
   const float32_t expected_joint2_dist_to_center = (expected_trailer2_length / 2.0F) - 1.7F;
   const bool valid_measurement_ekf_1 = false;
   const bool valid_measurement_ekf_2 = false;

   /** \action
    * Call Run_Length_Filter().
    */
   Run_Length_Filter(calibrations, valid_measurement_ekf_1, valid_measurement_ekf_2, cvt_state);

   /** \result
    * Verify full_vehicle_length unchanged and geometry outputs updated.
    */
   DOUBLES_EQUAL_TEXT(12.0F, cvt_state.two_link.full_vehicle_length, epsilon, "two-link full_vehicle_length should stay unchanged");

   DOUBLES_EQUAL_TEXT(expected_trailer1_length, cvt_state.two_link.trailer1_length, epsilon, "Unexpected trailer1_length");
   DOUBLES_EQUAL_TEXT(expected_joint1_dist_to_center, cvt_state.two_link.joint1_dist_to_center, epsilon, "Unexpected joint1_dist_to_center");
   DOUBLES_EQUAL_TEXT(calibrations.k_cvt_trailer_width, cvt_state.two_link.trailer1_width, epsilon, "Unexpected trailer1_width");

   DOUBLES_EQUAL_TEXT(expected_trailer2_length, cvt_state.two_link.trailer2_length, epsilon, "Unexpected trailer2_length");
   DOUBLES_EQUAL_TEXT(expected_joint2_dist_to_center, cvt_state.two_link.joint2_dist_to_center, epsilon, "Unexpected joint2_dist_to_center");
   DOUBLES_EQUAL_TEXT(calibrations.k_cvt_trailer_width, cvt_state.two_link.trailer2_width, epsilon, "Unexpected trailer2_width");
}

/** \purpose
 * Verify one-link low-pass update clamps from lower bound when measurement is valid.
 * \req
 * NA
 */
TEST(f360_cvt_estimate_trailer_length_unittest, one_link_is_updated_and_clamped_from_lower_bound_when_measurement_is_valid)
{
   /** \precond
    * Set one-link state so raw full carriage length is below minimum bound.
    */
   cvt_state.one_link.n_updates = 100;
   cvt_state.one_link.full_vehicle_length = 30.0F;
   cvt_state.one_link.joint_vcs_longpos = -1.0F;
   cvt_state.one_link.joint_dist_to_wheels = 5.4F; // To make the overall full carriage length measurement 9.9 m < 10 m

   const float32_t expected_weight = compute_low_pass_weight(cvt_state.one_link.n_updates);
   const float32_t expected_msmt_after_clamp = 10.0F;
   const float32_t expected_full_vehicle_length = ((1.0F - expected_weight) * 30.0F) + (expected_weight * expected_msmt_after_clamp);
   const float32_t expected_full_trailer_length = expected_full_vehicle_length + cvt_state.one_link.joint_vcs_longpos + 1.7F;
   const float32_t expected_joint_dist_to_center = (expected_full_trailer_length / 2.0F) - 1.7F;
   const bool valid_measurement_ekf_1 = true;
   const bool valid_measurement_ekf_2 = false;

   /** \action
    * Call Run_Length_Filter().
    */
   Run_Length_Filter(calibrations, valid_measurement_ekf_1, valid_measurement_ekf_2, cvt_state);

   /** \result
    * Verify one-link lower-bound clamping and derived geometry outputs.
    */
   DOUBLES_EQUAL_TEXT(expected_full_vehicle_length, cvt_state.one_link.full_vehicle_length, epsilon, "Unexpected one-link full_vehicle_length for lower clamp");
   DOUBLES_EQUAL_TEXT(expected_full_trailer_length, cvt_state.one_link.trailer_length, epsilon, "Unexpected one-link trailer_length for lower clamp");
   DOUBLES_EQUAL_TEXT(expected_joint_dist_to_center, cvt_state.one_link.joint_dist_to_center, epsilon, "Unexpected one-link joint_dist_to_center for lower clamp");
}

/** \purpose
 * Verify one-link low-pass update uses raw measurement when value is inside clamp bounds.
 * \req
 * NA
 */
TEST(f360_cvt_estimate_trailer_length_unittest, one_link_uses_raw_measurement_when_not_clamped)
{
   /** \precond
    * Set one-link state so raw full carriage length is within [10.0, 34.5].
    */
   cvt_state.one_link.n_updates = 30;
   cvt_state.one_link.full_vehicle_length = 20.0F;
   cvt_state.one_link.joint_vcs_longpos = -3.0F;
   cvt_state.one_link.joint_dist_to_wheels = 15.0F; // Raw msmt = 21.5, not clamped

   const float32_t expected_weight = compute_low_pass_weight(cvt_state.one_link.n_updates);
   const float32_t expected_raw_measurement = 21.5F;
   const float32_t expected_full_vehicle_length = ((1.0F - expected_weight) * 20.0F) + (expected_weight * expected_raw_measurement);
   const bool valid_measurement_ekf_1 = true;
   const bool valid_measurement_ekf_2 = false;

   /** \action
    * Call Run_Length_Filter().
    */
   Run_Length_Filter(calibrations, valid_measurement_ekf_1, valid_measurement_ekf_2, cvt_state);

   /** \result
    * Verify one-link full vehicle length follows raw (unclamped) measurement.
    */
   DOUBLES_EQUAL_TEXT(expected_full_vehicle_length, cvt_state.one_link.full_vehicle_length, epsilon, "Unexpected one-link full_vehicle_length for non-clamped measurement");
}

/** \purpose
 * Verify two-link low-pass update uses raw measurement when value is inside clamp bounds.
 * \req
 * NA
 */
TEST(f360_cvt_estimate_trailer_length_unittest, two_link_uses_raw_measurement_when_not_clamped)
{
   /** \precond
    * Set two-link state so raw full carriage length is within [10.0, 34.5].
    */
   cvt_state.two_link.n_updates = 60;
   cvt_state.two_link.full_vehicle_length = 22.0F;
   cvt_state.two_link.joint1_vcs_longpos = -4.0F;
   cvt_state.two_link.joint1_dist_to_wheels = 10.0F;
   cvt_state.two_link.joint2_dist_to_wheels = 8.0F; // Raw msmt = 25.5, not clamped

   const float32_t expected_weight = compute_low_pass_weight(cvt_state.two_link.n_updates);
   const float32_t expected_raw_measurement = 25.5F;
   const float32_t expected_full_vehicle_length = ((1.0F - expected_weight) * 22.0F) + (expected_weight * expected_raw_measurement);
   const bool valid_measurement_ekf_1 = false;
   const bool valid_measurement_ekf_2 = true;

   /** \action
    * Call Run_Length_Filter().
    */
   Run_Length_Filter(calibrations, valid_measurement_ekf_1, valid_measurement_ekf_2, cvt_state);

   /** \result
    * Verify two-link full vehicle length follows raw (unclamped) measurement.
    */
   DOUBLES_EQUAL_TEXT(expected_full_vehicle_length, cvt_state.two_link.full_vehicle_length, epsilon, "Unexpected two-link full_vehicle_length for non-clamped measurement");
}

/** @}*/
