/** \file
 * This file contains unit tests for content of f360_cvt_generate_measurement.cpp file
 */

#include "f360_constants.h"
#include "f360_math.h"
#include "f360_cvt_generate_measurement.h"
#include <CppUTest/TestHarness.h>
#include <vector>
#include <string>
#include <cstring>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines
/** \defgroup  f360_cvt_calc_range_rate_gate
 *  @{
 */

using namespace f360_variant_A;

/** \brief
 * Test group for the Calc_Range_Rate_Gate function in the f360_variant_A namespace.
 * This group tests the calculation of range rate gates under various conditions.
 */
TEST_GROUP(f360_cvt_calc_range_rate_gate)
{
   // Declare common variables used within all tests in this test group.
   float32_t host_yaw_rate;
   bool f_reversing;
   float32_t epsilon;

   // The struct to save all parameters for one test
   struct range_rate_gate_case_T
   {
      float32_t host_yaw_rate;
      bool f_reversing;
      float32_t expected_range_rate_gate;
      std::string test_description;  // Descriptor of the test
   };

   /** \setup
    * Initialize common variables before each test.
    */
   TEST_SETUP()
   {
      host_yaw_rate = 0.0F;
      f_reversing = false;
      epsilon = 1E-6F;
   }
};

/** \purpose
 * Test the Calc_Range_Rate_Gate function when reversing.
 * \req
 * NA
 */
TEST(f360_cvt_calc_range_rate_gate, range_rate_gate_should_be_correct_for_different_host_yaw_rate_and_directions)
{
   /** \precond
    * Set up a series of scenarios with different host yaw rate and moving directions
    */
   // Define all test cases

   std::vector<range_rate_gate_case_T> all_test_cases = {
      //host_yaw_rate,         f_reversing,    expected_range_rate_gate,   test_description
       {F360_DEG2RAD(0.0F),    false,           0.6F,                      "Test1: Going Forward, 0 deg/s yaw rate, the lower border of interpolation range"},
       {F360_DEG2RAD(7.5F),    false,           1.05F,                     "Test2: Going Forward, 7.5 deg/s yaw rate to right, the center of interpolation range"},
       {F360_DEG2RAD(16.0F),   false,           1.50F,                     "Test3: Going Forward, 16 deg/s yaw rate to right, the higher border of interpolation range"},
       {F360_DEG2RAD(-7.5F),   false,           1.05F,                     "Test4: Going Forward, 7.5 deg/s yaw rate to left, the center of interpolation range"},
       {F360_DEG2RAD(-16.0F),  false,           1.50F,                     "Test5: Going Forward, 16 deg/s yaw rate to left, the higher border of interpolation range"},
       {F360_DEG2RAD(0.0F),    true,            0.3F,                      "Test6: Reversing, 0 deg/s yaw rate, the lower border of interpolation range"},
       {F360_DEG2RAD(7.5F),    true,            0.525F,                    "Test7: Reversing, 7.5 deg/s yaw rate to right, the center of interpolation range"},
       {F360_DEG2RAD(16.0F),   true,            0.75F,                     "Test8: Reversing, 16 deg/s yaw rate to right, the higher border of interpolation range"},
       {F360_DEG2RAD(-7.5F),   true,            0.525F,                    "Test9: Reversing, 7.5 deg/s yaw rate to left, the center of interpolation range"},
       {F360_DEG2RAD(-16.0F),  true,            0.75F,                     "Test10: Reversing, 16 deg/s yaw rate to left, the higher border of interpolation range"},
   };

   // Loop over all test cases
   for (const range_rate_gate_case_T &test_case_i : all_test_cases)
   {
      host_yaw_rate = test_case_i.host_yaw_rate;
      f_reversing = test_case_i.f_reversing;
      const float32_t expected_range_rate_gate = test_case_i.expected_range_rate_gate;

      /** \action
       * Call the Calc_Range_Rate_Gate function
       */
      float32_t result = Calc_Range_Rate_Gate(host_yaw_rate, f_reversing);

      /** \result
       * Check that the range rate gate is correct
       */
      DOUBLES_EQUAL_TEXT(expected_range_rate_gate, result, epsilon, test_case_i.test_description.c_str());
   }
}

/** \defgroup  f360_cvt_find_valid_detections
 *  @{
 */

/** \brief
 * Test group for the Find_Valid_Detections function in the f360_variant_A namespace.
 * This group tests the identification of valid detections for line detection.
 */
TEST_GROUP(f360_cvt_find_valid_detections)
{
   // Declare common variables used within all tests in this test group.
   static const int32_t calib_MAX_NUMBER_OF_VALID_DETECTIONS_CV_TRAILER = 40;
   F360_CVT_Input_Data_T cvt_input = {};
   F360_CVT_State_T cvt_state = {};
   float32_t range_rate_gate = {};
   int32_t valid_det_idx[calib_MAX_NUMBER_OF_VALID_DETECTIONS_CV_TRAILER] = {};
   int32_t counter_valid_det = {};

   /** \setup
    * Initialize common variables before each test.
    */
   TEST_SETUP()
   {
      // Initialize cvt_input
      cvt_input.host_speed = 2.1F;

      // Initialize cvt_state
      cvt_state.radar_vcs_longpos = -2.0F;
      cvt_state.host_length = 5.0F;
      cvt_state.host_width = 2.2F;

      range_rate_gate = 1.0F;
      counter_valid_det = 0;

      // First detection: high range rate, in region, not in host
      cvt_input.detections[0].range_rate = range_rate_gate + 0.1F;
      cvt_input.detections[0].vcs_longpos = -2.0F + cvt_state.radar_vcs_longpos;
      cvt_input.detections[0].vcs_latpos = 3.0F;

      // Second detection: Low range rate, outside the region
      cvt_input.detections[1].range_rate = range_rate_gate / 2.0F;
      cvt_input.detections[1].vcs_longpos = cvt_state.radar_vcs_longpos;
      cvt_input.detections[1].vcs_latpos = 30.1F;

      // Third detection: Low range rate, in region and in host
      cvt_input.detections[2].range_rate = range_rate_gate / 2.0F;
      cvt_input.detections[2].vcs_longpos = -cvt_state.host_length + 1.0F;
      cvt_input.detections[2].vcs_latpos = -cvt_state.host_width / 2.0F;


      // Fourth detection: Low range rate, in region, not in host
      cvt_input.detections[3].range_rate = range_rate_gate / 2.0F;
      cvt_input.detections[3].vcs_longpos = -2.0F + cvt_state.radar_vcs_longpos;
      cvt_input.detections[3].vcs_latpos = 3.0F;




      // Update the number of detections
      cvt_input.n_detections = 4;
   }
};

/** \purpose
 * Test the Find_Valid_Detections function with low host speed.
 * \req
 * NA
 */
TEST(f360_cvt_find_valid_detections, do_not_count_valid_dets_if_host_speed_is_low)
{
   /** \precond
    * Make the host speed lower than threshold
    */
   cvt_input.host_speed = 1.9F;

   /** \action
    * Call the Find_Valid_Detections function
    */
   Find_Valid_Detections(cvt_input, cvt_state, range_rate_gate, valid_det_idx, counter_valid_det);

   /** \result
    * Check that no valid detections were found
    */
   CHECK_EQUAL_TEXT(0, counter_valid_det, "Should find no valid detections, as host speed is low");
}

/** \purpose
 * Test the Find_Valid_Detections function with high range rate detections.
 * \req
 * NA
 */
TEST(f360_cvt_find_valid_detections, high_range_rate_detection_is_invalid)
{
   /** \precond
    * Only send the high range rate detection, by changing n_detections
    */
   cvt_input.n_detections = 1;

   /** \action
    * Call the Find_Valid_Detections function
    */
   Find_Valid_Detections(cvt_input, cvt_state, range_rate_gate, valid_det_idx, counter_valid_det);

   /** \result
    * Check that no valid detections were found
    */
   CHECK_EQUAL_TEXT(0, counter_valid_det, "Should find no valid detections, as the only detection has high range rat");
}

/** \purpose
 * Test the Find_Valid_Detections function with far detections.
 * \req
 * NA
 */
TEST(f360_cvt_find_valid_detections, detections_outside_region_is_invalid)
{
   /** \precond
    * Include the outside region detection as extra detection, by changing n_detections
    */
   cvt_input.n_detections = 2;

   /** \action
    * Call the Find_Valid_Detections function
    */
   Find_Valid_Detections(cvt_input, cvt_state, range_rate_gate, valid_det_idx, counter_valid_det);

   /** \result
    * Check that no valid detections were found
    */
   CHECK_EQUAL_TEXT(0, counter_valid_det, "Should find no valid detections, as the detection is not in region");
}

/** \purpose
 * Test the Find_Valid_Detections function with in host detection.
 * \req
 * NA
 */
TEST(f360_cvt_find_valid_detections, high_rr_detection_in_host_should_be_invalid)
{
   /** \precond
    * Include the in host detection as extra detection, by changing n_detections
    */
   cvt_input.n_detections = 3;

   /** \action
    * Call the Find_Valid_Detections function
    */
   Find_Valid_Detections(cvt_input, cvt_state, range_rate_gate, valid_det_idx, counter_valid_det);

   /** \result
    * Check that no valid detections were found
    */
   CHECK_EQUAL_TEXT(0, counter_valid_det, "Should find no valid detections, as the detection is in host");
}

/** \purpose
 * Test the Find_Valid_Detections function with in host detection.
 * \req
 * NA
 */
TEST(f360_cvt_find_valid_detections, low_rr_detection_in_region_should_be_valid)
{
   /** \precond
    * Include the in region detection as extra detection by changing n_detections
    */
   cvt_input.n_detections = 4;

   /** \action
    * Call the Find_Valid_Detections function
    */
   Find_Valid_Detections(cvt_input, cvt_state, range_rate_gate, valid_det_idx, counter_valid_det);

   /** \result
    * Check that 1 valid detections were found
    */
   CHECK_EQUAL_TEXT(1, counter_valid_det, "Should find 1 valid detections, as the detection is in region");
   CHECK_EQUAL_TEXT(cvt_input.n_detections-1, valid_det_idx[counter_valid_det-1], "The last detection should be added into the valid detection list");
}

/** \purpose
 * Test the Find_Valid_Detections function with all detections valid.
 * \req
 * NA
 */
TEST(f360_cvt_find_valid_detections, counter_valid_det_should_be_saturated)
{
   /** \precond
    * make more than enough detections valid
    */
   cvt_input.n_detections = calib_MAX_NUMBER_OF_VALID_DETECTIONS_CV_TRAILER + 2;

   for (int32_t i = 0; i < cvt_input.n_detections; i++)
   {
      cvt_input.detections[i].range_rate = range_rate_gate * 0.5F;
      cvt_input.detections[i].vcs_longpos = -2.0F + cvt_state.radar_vcs_longpos;
      cvt_input.detections[i].vcs_latpos = 3.0F;
   }

   /** \action
    * Call the Find_Valid_Detections function
    */
   Find_Valid_Detections(cvt_input, cvt_state, range_rate_gate, valid_det_idx, counter_valid_det);

   /** \result
    * Check that all of the first calib_MAX_NUMBER_OF_VALID_DETECTIONS_CV_TRAILER detections are valid.
    * The rest are unknown, as there is a break loop to saturate the detections to be checked.
    */
   CHECK_EQUAL_TEXT(calib_MAX_NUMBER_OF_VALID_DETECTIONS_CV_TRAILER, counter_valid_det, "Should find 40 valid detections, as there is a break loop to saturate the counter");
}

/** \defgroup  f360_cvt_find_approximate_lines
 *  @{
 */

/** \brief
 * Test group for the Find_Approximate_Lines function in the f360_variant_A namespace.
 * This group tests the identification of approximate lines from detection data.
 */
TEST_GROUP(f360_cvt_find_approximate_lines)
{
   F360_CVT_Detection_Info_T detections[CVT_MAX_NUMBER_OF_DETECTIONS] = {};
   float32_t radar_vcs_latpos = {};
   int32_t n_valid_dets = {};
   int32_t valid_det_idxs[MAX_NUMBER_OF_VALID_DETECTIONS_CV_TRAILER] = {};
   Trailer_Measurement_Info_T primary_measurement = {};
   Trailer_Measurement_Info_T secondary_measurement = {};
   // Used for mirroring the sensor and detections, to test the result on the other side
   const float32_t sign_radar_direction[2] = {1.0F, -1.0F};

   struct expected_line_param_T
   {
      bool f_primary_measurement_valid;   // flag the line with smaller absolute angle is measured
      bool f_secondary_measurement_valid;  // flag the line with larger absolute angle is measured
      float32_t primary_line_slope;  // expected slope of primary line
      float32_t primary_line_intersect;  // expected intersect of primary line
      float32_t secondary_line_slope;  // expected slope of secondary line
      float32_t secondary_line_intersect;  // expected intersect of secondary line
      float32_t epsilon_slope;  // Epsilon of the test
      std::string test_description;  // Description of the test
   };

   /** \setup
    * Initialize common variables before each test.
    */
   TEST_SETUP()
   {

      for (int32_t i = 0; i < 10; i++)
      {
         // First line
         detections[i].vcs_longpos = -20.0F - i * 2.0F;
         detections[i].vcs_latpos = -10.0F - i * 1.0F;
         valid_det_idxs[i] = i;
      }


      for (int32_t i = 0; i < 10; i++)
      {
         // Second line starts from the end of the first line
         detections[i + 10].vcs_longpos = detections[9].vcs_longpos - i * 2.0F;
         detections[i + 10].vcs_latpos = detections[9].vcs_latpos - i * 4.0F;
         valid_det_idxs[i + 10] = i + 10;
      }

      n_valid_dets = 20;
      radar_vcs_latpos = -2.0F;
   }

   static void mirror_the_detections(
      F360_CVT_Detection_Info_T(&detections)[CVT_MAX_NUMBER_OF_DETECTIONS],
      float32_t &radar_vcs_latpos,
      const int32_t n_valid_dets,
      const float32_t multiplier_radar_direction)
   {
      radar_vcs_latpos *= multiplier_radar_direction;
      for (int32_t i = 0; i < n_valid_dets; i++)
      {
         // Mirror the sensor and detections to the other side
         detections[i].vcs_latpos *= multiplier_radar_direction;
      }
   }

   static void check_primary_measurement_output_of_find_approximate_lines(
      Trailer_Measurement_Info_T& primary_measurement,
      const expected_line_param_T expected_result,
      const float32_t multiplier_radar_direction)
   {
      if (expected_result.f_primary_measurement_valid)
      {
         CHECK_TRUE_TEXT(primary_measurement.f_line_valid, expected_result.test_description.c_str());
         // NaN means the test should be skipped
         if (!std::isnan(expected_result.primary_line_slope))
         {
            DOUBLES_EQUAL_TEXT(multiplier_radar_direction * expected_result.primary_line_slope, primary_measurement.line_slope, expected_result.epsilon_slope, expected_result.test_description.c_str());
         }
         if (!std::isnan(expected_result.primary_line_intersect))
         {
            DOUBLES_EQUAL_TEXT(expected_result.primary_line_intersect, primary_measurement.line_intersect, 3.0F, expected_result.test_description.c_str());
         }
      }
      else
      {
         CHECK_FALSE_TEXT(primary_measurement.f_line_valid, expected_result.test_description.c_str());
      }
   }

   static void check_secondary_measurement_output_of_find_approximate_lines(
      Trailer_Measurement_Info_T& secondary_measurement,
      const expected_line_param_T expected_result,
      const float32_t multiplier_radar_direction)
   {
      if (expected_result.f_secondary_measurement_valid)
      {
         CHECK_TRUE_TEXT(secondary_measurement.f_line_valid, expected_result.test_description.c_str());
         // NaN means the test should be skipped
         if (!std::isnan(expected_result.secondary_line_slope))
         {
            DOUBLES_EQUAL_TEXT(multiplier_radar_direction * expected_result.secondary_line_slope, secondary_measurement.line_slope, expected_result.epsilon_slope, expected_result.test_description.c_str());
         }
         if (!std::isnan(expected_result.secondary_line_intersect))
         {
            DOUBLES_EQUAL_TEXT(expected_result.secondary_line_intersect, secondary_measurement.line_intersect, 3.0F, expected_result.test_description.c_str());
         }
      }
      else
      {
         CHECK_FALSE_TEXT(secondary_measurement.f_line_valid, expected_result.test_description.c_str());
      }
   }
};

/** \purpose
 * Test the Find_Approximate_Lines function with detections forming one clear line.
 * \req
 * NA
 */
TEST(f360_cvt_find_approximate_lines, line_measurement_should_work_for_1_line_presence)
{
   /** \precond
    * Set up initial conditions with detections of the first line
    */
   n_valid_dets = 10;

   expected_line_param_T expected_result = {
   // f_primary_measurement_valid, f_secondary_measurement_valid, primary_line_slope,  primary_line_intersect,  secondary_line_slope, secondary_line_intersect,  epsilon_slope, test_description,
      true,                        false,                         2.0F,                0.0F,                    std::nanf(""),                  std::nanf(""),   0.2F,          "Test with only 1 line creating detections, one line should be found!"
   };

   for (const float32_t multiplier_radar_direction : sign_radar_direction)
   {
      // If applicable, mirror the detections w.r.t. vcs_x axis
      mirror_the_detections(detections, radar_vcs_latpos, n_valid_dets, multiplier_radar_direction);

      /** \action
       * Call the Find_Approximate_Lines() function
       */
      Find_Approximate_Lines(detections, radar_vcs_latpos, n_valid_dets, valid_det_idxs, primary_measurement, secondary_measurement);

      /** \result
       * Check that both of the measurements match the expected result
       */
      check_primary_measurement_output_of_find_approximate_lines(primary_measurement, expected_result, multiplier_radar_direction);
      check_secondary_measurement_output_of_find_approximate_lines(secondary_measurement, expected_result, multiplier_radar_direction);
   }
}

/** \purpose
 * Test the Find_Approximate_Lines function with detections forming two clear lines.
 * \req
 * NA
 */
TEST(f360_cvt_find_approximate_lines, line_measurement_should_work_for_2_line_presence)
{
   /** \precond
    * Set up initial conditions with detections forming two clear lines
    */
   n_valid_dets = 20;

   expected_line_param_T expected_result = {
   // f_primary_measurement_valid, f_secondary_measurement_valid, primary_line_slope,  primary_line_intersect,  secondary_line_slope, secondary_line_intersect,  epsilon_slope, test_description,
      true,                        true,                         0.5F,                std::nanf(""),           2.0F,                 std::nanf(""),             0.2F,          "Test with 2 trailers creating detections, two lines should be found!"
   };

   for (const float32_t multiplier_radar_direction : sign_radar_direction)
   {
      // If applicable, mirror the detections w.r.t. vcs_x axis
      mirror_the_detections(detections, radar_vcs_latpos, n_valid_dets, multiplier_radar_direction);

      /** \action
       * Call the Find_Approximate_Lines() function
       */
      Find_Approximate_Lines(detections, radar_vcs_latpos, n_valid_dets, valid_det_idxs, primary_measurement, secondary_measurement);

      /** \result
       * Check that both of the measurements match the expected result
       */
      check_primary_measurement_output_of_find_approximate_lines(primary_measurement, expected_result, multiplier_radar_direction);
      check_secondary_measurement_output_of_find_approximate_lines(secondary_measurement, expected_result, multiplier_radar_direction);
   }
}

/** \purpose
 * Test the Find_Approximate_Lines function with detections not forming any clear line.
 * \req
 * NA
 */
TEST(f360_cvt_find_approximate_lines, no_line_detected_if_detections_are_messy)
{
   /** \precond
    * Set up initial conditions with scattered detections not forming clear lines
    */
   n_valid_dets = 10;
   for (int32_t i = 0; i < n_valid_dets; i++)
   {
      detections[i].vcs_longpos = -20.0F + F360_Cosf(i) * 40.0F;
      detections[i].vcs_latpos = -20.0F + F360_Sinf(i) * 40.0F;
      valid_det_idxs[i] = i;
   }

   expected_line_param_T expected_result = {
   // f_primary_measurement_valid, f_secondary_measurement_valid, primary_line_slope,  primary_line_intersect,  secondary_line_slope, secondary_line_intersect,  epsilon_slope, test_description,
      false,                        false,                        std::nanf(""),       std::nanf(""),           std::nanf(""),        std::nanf(""),             0.2F,          "Test with random detections, no line should be found!"
   };

   for (const float32_t multiplier_radar_direction : sign_radar_direction)
   {
      // If applicable, mirror the detections w.r.t. vcs_x axis
      mirror_the_detections(detections, radar_vcs_latpos, n_valid_dets, multiplier_radar_direction);

      /** \action
       * Call the Find_Approximate_Lines() function
       */
      Find_Approximate_Lines(detections, radar_vcs_latpos, n_valid_dets, valid_det_idxs, primary_measurement, secondary_measurement);

      /** \result
       * Check that both of the measurements match the expected result
       */
      check_primary_measurement_output_of_find_approximate_lines(primary_measurement, expected_result, multiplier_radar_direction);
      check_secondary_measurement_output_of_find_approximate_lines(secondary_measurement, expected_result, multiplier_radar_direction);
   }
}

/** \purpose
 * Test the Find_Approximate_Lines function with detections forming one clear line and some scattered points.
 * \req
 * NA
 */
TEST(f360_cvt_find_approximate_lines, line_measurement_is_robust_to_noises_in_case_of_1_line_presence)
{
   /** \precond
    * Set up initial conditions with detections forming one clear line and some scattered points
    */
   n_valid_dets = 15;
   // Add some noises near the end of the 1st line
   for (int32_t i = 10; i < n_valid_dets; i++)
   {
      detections[i].vcs_longpos = detections[9].vcs_longpos + static_cast<float32_t>(rand()) / RAND_MAX * 40.0F;
      detections[i].vcs_latpos = detections[9].vcs_latpos + static_cast<float32_t>(rand()) / RAND_MAX * 40.0F;
      valid_det_idxs[i] = i;
   }

   expected_line_param_T expected_result = {
   // f_primary_measurement_valid, f_secondary_measurement_valid, primary_line_slope,  primary_line_intersect,  secondary_line_slope, secondary_line_intersect,  epsilon_slope, test_description,
      true,                        false,                         2.0F,                0.0F,                    std::nanf(""),        std::nanf(""),             0.2F,          "Test with 1 trailer and noisy detections, 1 line should still be robustly found!"
   };

   for (const float32_t multiplier_radar_direction : sign_radar_direction)
   {
      // If applicable, mirror the detections w.r.t. vcs_x axis
      mirror_the_detections(detections, radar_vcs_latpos, n_valid_dets, multiplier_radar_direction);

      /** \action
       * Call the Find_Approximate_Lines() function
       */
      Find_Approximate_Lines(detections, radar_vcs_latpos, n_valid_dets, valid_det_idxs, primary_measurement, secondary_measurement);

      /** \result
       * Check that both of the measurements match the expected result
       */
      check_primary_measurement_output_of_find_approximate_lines(primary_measurement, expected_result, multiplier_radar_direction);
      check_secondary_measurement_output_of_find_approximate_lines(secondary_measurement, expected_result, multiplier_radar_direction);
   }}

/** \purpose
 * Test the Find_Approximate_Lines function with detections forming two clear lines and some scattered points.
 * \req
 * NA
 */
TEST(f360_cvt_find_approximate_lines, line_measurement_is_robust_to_noises_in_case_of_2_line_presence)
{
   /** \precond
    * Set up initial conditions with detections forming two clear lines
    */
   n_valid_dets = 25;
   // Add some noises near the end of the 1st line
   for (int32_t i = 20; i < n_valid_dets; i++)
   {
      detections[i].vcs_longpos = detections[19].vcs_longpos + F360_Cosf(static_cast<float32_t>(i)) * 40.0F;
      detections[i].vcs_latpos = detections[19].vcs_latpos + F360_Sinf(static_cast<float32_t>(i)) * 40.0F;
      valid_det_idxs[i] = i;
   }

   // Expected result
   expected_line_param_T expected_result = {
   // f_primary_measurement_valid, f_secondary_measurement_valid, primary_line_slope,  primary_line_intersect,  secondary_line_slope, secondary_line_intersect,  epsilon_slope, test_description,
      true,                        true,                          0.5F,                std::nanf(""),           2.0F,                 std::nanf(""),             0.2F,          "Test with 2 trailers and noisy detections, 2 lines should still be robustly found!"
   };

   for (const float32_t multiplier_radar_direction : sign_radar_direction)
   {
      // If applicable, mirror the detections w.r.t. vcs_x axis
      mirror_the_detections(detections, radar_vcs_latpos, n_valid_dets, multiplier_radar_direction);

      /** \action
       * Call the Find_Approximate_Lines() function
       */
      Find_Approximate_Lines(detections, radar_vcs_latpos, n_valid_dets, valid_det_idxs, primary_measurement, secondary_measurement);

      /** \result
       * Check that both of the measurements match the expected result
       */
      check_primary_measurement_output_of_find_approximate_lines(primary_measurement, expected_result, multiplier_radar_direction);
      check_secondary_measurement_output_of_find_approximate_lines(secondary_measurement, expected_result, multiplier_radar_direction);
   }}

/** \purpose
 * Test the Find_Approximate_Lines function with the 2nd line (larger angle) being clearer than the 1st (smaller angle).
 * the primary line should always be the smaller angle one, regardless of #detections associated
 * \req
 * NA
 */
TEST(f360_cvt_find_approximate_lines, the_primary_line_is_always_with_smaller_angle)
{
   /** \precond
    * Set up initial conditions with detections forming two clear lines
    */
   n_valid_dets = 20;
   // Reduce the #detections of the 1st trailer line, which has smaller angle
   // Replace 4 detections of the 1st line with some of the 2nd line - To make the 2nd line best fit
   for (int32_t i = 6; i < 10; i++)
   {
      detections[i].vcs_longpos = detections[i+5].vcs_longpos + F360_Cosf(static_cast<float32_t>(i)) * 40.0F;
      detections[i].vcs_latpos = detections[i+5].vcs_latpos + F360_Sinf(static_cast<float32_t>(i)) * 40.0F;
      valid_det_idxs[i] = i;
   }


   // Expected result
   expected_line_param_T expected_result = {
   // f_primary_measurement_valid, f_secondary_measurement_valid, primary_line_slope,  primary_line_intersect,  secondary_line_slope, secondary_line_intersect,  epsilon_slope, test_description,
      true,                        true,                          0.5F,                std::nanf(""),           2.0F,                 std::nanf(""),             0.2F,          "Test with uneven #detections created by 2 trailers, 2 lines should still be robustly found in specific order!"
   };

   for (const float32_t multiplier_radar_direction : sign_radar_direction)
   {
      // If applicable, mirror the detections w.r.t. vcs_x axis
      mirror_the_detections(detections, radar_vcs_latpos, n_valid_dets, multiplier_radar_direction);

      /** \action
       * Call the Find_Approximate_Lines() function
       */
      Find_Approximate_Lines(detections, radar_vcs_latpos, n_valid_dets, valid_det_idxs, primary_measurement, secondary_measurement);

      /** \result
       * Check that both of the measurements match the expected result
       */
      check_primary_measurement_output_of_find_approximate_lines(primary_measurement, expected_result, multiplier_radar_direction);
      check_secondary_measurement_output_of_find_approximate_lines(secondary_measurement, expected_result, multiplier_radar_direction);
   }}

/** \defgroup  f360_cvt_generate_measurement
 *  @{
 */

/** \brief
 * Test group for the Generate_Measurement function in the f360_variant_A namespace.
 * This group tests the identification of approximate lines from detection data.
 */
TEST_GROUP(f360_cvt_generate_measurement)
{
   int32_t n_defined_dets = {};
   F360_CVT_Input_Data_T cvt_input = {};
   F360_CVT_State_T cvt_state = {};
   // Used for mirroring the sensor and detections, to test the result on the other side
   const float32_t sign_radar_direction[2] = {1.0F, -1.0F};

   struct expected_line_param_T
   {
      bool f_primary_measurement_valid;   // flag a line with smaller absolute angle is measured and is VALID for measurement update
      bool f_secondary_measurement_valid;  // flag the line with larger absolute angle is measured and is VALID for measurement update
      float32_t primary_msmt_trailer_angle_vcs;  // expected slope of primary line
      float32_t primary_line_intersect;  // expected intersect of primary line
      float32_t secondary_msmt_trailer_angle_vcs;  // expected slope of secondary line
      float32_t secondary_line_intersect;  // expected intersect of secondary line
      float32_t epsilon_vcs_angle;  // Epsilon of the test
      std::string test_description;  // Description of the test
   };

   /** \setup
    * Initialize common variables before each test.
    */
   TEST_SETUP()
   {
      // Initialize cvt_input
      cvt_input.host_speed = 2.1F;

      // Initialize cvt_state
      cvt_state.radar_vcs_longpos = -2.0F;
      cvt_state.host_length = 5.0F;
      cvt_state.host_width = 2.2F;
      cvt_state.radar_vcs_latpos = -2.0F;

      for (int32_t i = 0; i < 10; i++)
      {
         // First line
         cvt_input.detections[i].range_rate = 0.1F;
         cvt_input.detections[i].vcs_longpos = -6.0F - i * 1.0F;
         cvt_input.detections[i].vcs_latpos = -3.0F - i * 0.5F;
      }

      for (int32_t i = 0; i < 10; i++)
      {
         // Second line starts from the end of the first line
         cvt_input.detections[i + 10].range_rate = 0.1F;
         cvt_input.detections[i + 10].vcs_longpos = cvt_input.detections[9].vcs_longpos - i * 0.5F;
         cvt_input.detections[i + 10].vcs_latpos = cvt_input.detections[9].vcs_latpos - i * 1.0F;
      }

      cvt_input.n_detections = 20;

   }

   static void mirror_the_detections(
      F360_CVT_Input_Data_T &cvt_input,
      F360_CVT_State_T &cvt_state,
      const float32_t multiplier_radar_direction)
   {
      cvt_state.radar_vcs_latpos *= multiplier_radar_direction;
      for (int32_t i = 0; i < cvt_input.n_detections; i++)
      {
         // Mirror the sensor and detections to the other side
         cvt_input.detections[i].vcs_latpos *= multiplier_radar_direction;
      }
   }

   static void check_primary_measurement_output_of_generate_measurement(
      const F360_CVT_State_T &cvt_state,
      const expected_line_param_T expected_result,
      const float32_t multiplier_radar_direction)
   {
      if (expected_result.f_primary_measurement_valid)
      {
         CHECK_TRUE_TEXT(cvt_state.primary_measurement.f_msmt_valid, expected_result.test_description.c_str());
         // NaN means the test should be skipped
         if (!std::isnan(expected_result.primary_msmt_trailer_angle_vcs))
         {
            DOUBLES_EQUAL_TEXT(multiplier_radar_direction * expected_result.primary_msmt_trailer_angle_vcs, cvt_state.primary_measurement.trailer_angle_vcs, expected_result.epsilon_vcs_angle, expected_result.test_description.c_str());
         }
         if (!std::isnan(expected_result.primary_line_intersect))
         {
            DOUBLES_EQUAL_TEXT(expected_result.primary_line_intersect, cvt_state.primary_measurement.line_intersect, 3.0F, expected_result.test_description.c_str());
         }
      }
      else
      {
         CHECK_FALSE_TEXT(cvt_state.primary_measurement.f_msmt_valid, expected_result.test_description.c_str());
      }
   }

   static void check_secondary_measurement_output_of_generate_measurement(
      const F360_CVT_State_T &cvt_state,
      const expected_line_param_T expected_result,
      const float32_t multiplier_radar_direction)
   {
      if (expected_result.f_secondary_measurement_valid)
      {
         CHECK_TRUE_TEXT(cvt_state.secondary_measurement.f_msmt_valid, expected_result.test_description.c_str());
         // NaN means the test should be skipped
         if (!std::isnan(expected_result.secondary_msmt_trailer_angle_vcs))
         {
            DOUBLES_EQUAL_TEXT(multiplier_radar_direction * expected_result.secondary_msmt_trailer_angle_vcs, cvt_state.secondary_measurement.trailer_angle_vcs, expected_result.epsilon_vcs_angle, expected_result.test_description.c_str());
         }
         if (!std::isnan(expected_result.secondary_line_intersect))
         {
            DOUBLES_EQUAL_TEXT(expected_result.secondary_line_intersect, cvt_state.secondary_measurement.line_intersect, 3.0F, expected_result.test_description.c_str());
         }
      }
      else
      {
         CHECK_FALSE_TEXT(cvt_state.secondary_measurement.f_msmt_valid, expected_result.test_description.c_str());
      }
   }
};


/** \purpose
 * Test the Generate_Measurement function with detections forming one clear line.
 * \req
 * NA
 */
TEST(f360_cvt_generate_measurement, generate_measurement_should_work_for_1_line_presence)
{
   /** \precond
    * Set up initial conditions with detections of the first line
    */
   cvt_input.n_detections = 10;

   expected_line_param_T expected_result = {
   // f_primary_measurement_valid, f_secondary_measurement_valid, primary_msmt_trailer_angle_vcs,  primary_line_intersect,  secondary_msmt_trailer_angle_vcs, secondary_line_intersect,  epsilon_vcs_angle, test_description,
      true,                        false,                         0.43F,                            0.0F,                    std::nanf(""),                    std::nanf(""),             0.01F,              "Test with only 1 line creating detections, one line should be valid!"
   };

   for (const float32_t multiplier_radar_direction : sign_radar_direction)
   {
      // mirror the detections w.r.t. vcs_x axis
      mirror_the_detections(cvt_input, cvt_state, multiplier_radar_direction);

      /** \action
       * Call the generate_measurement() function
       */
      Generate_Measurement(cvt_input, cvt_state);

      /** \result
       * Check that both of the measurements match the expected result
       */
      check_primary_measurement_output_of_generate_measurement(cvt_state, expected_result, multiplier_radar_direction);
      check_secondary_measurement_output_of_generate_measurement(cvt_state, expected_result, multiplier_radar_direction);
   }
}

/** \purpose
 * Test the Generate_Measurement function with detections forming two clear lines.
 * \req
 * NA
 */
TEST(f360_cvt_generate_measurement, line_measurement_should_work_for_2_line_presence)
{
   /** \precond
    * Set up initial conditions with detections forming two clear lines
    */
   cvt_input.n_detections = 20;

   expected_line_param_T expected_result = {
   // f_primary_measurement_valid, f_secondary_measurement_valid, primary_msmt_trailer_angle_vcs,  primary_line_intersect,  secondary_msmt_trailer_angle_vcs, secondary_line_intersect,  epsilon_vcs_angle, test_description,
      true,                        true,                          1.05F,                           -10.9F,                  0.44F,                             0.4F,             0.01F,             "Test with 2 trailers creating detections, two lines should be found!"
   };
   for (const float32_t multiplier_radar_direction : sign_radar_direction)
   {
      // mirror the detections w.r.t. vcs_x axis
      mirror_the_detections(cvt_input, cvt_state, multiplier_radar_direction);

      /** \action
       * Call the generate_measurement() function
       */
      Generate_Measurement(cvt_input, cvt_state);

      /** \result
       * Check that both of the measurements match the expected result
       */
      check_primary_measurement_output_of_generate_measurement(cvt_state, expected_result, multiplier_radar_direction);
      check_secondary_measurement_output_of_generate_measurement(cvt_state, expected_result, multiplier_radar_direction);
   }

}

/** \purpose
 * Test the Generate_Measurement function with detections not forming any clear line.
 * \req
 * NA
 */
TEST(f360_cvt_generate_measurement, no_line_detected_if_detections_are_messy)
{
   /** \precond
    * Set up initial conditions with scattered detections not forming clear lines
    */
   cvt_input.n_detections = 10;
   for (int32_t i = 0; i < cvt_input.n_detections; i++)
   {
      cvt_input.detections[i].vcs_longpos = -20.0F + F360_Cosf(i) * 40.0F;
      cvt_input.detections[i].vcs_latpos = -20.0F + F360_Sinf(i) * 40.0F;
   }

   expected_line_param_T expected_result = {
   // f_primary_measurement_valid, f_secondary_measurement_valid, primary_msmt_trailer_angle_vcs,  primary_line_intersect,  secondary_msmt_trailer_angle_vcs, secondary_line_intersect,  epsilon_vcs_angle, test_description,
      false,                        false,                        std::nanf(""),                   std::nanf(""),           std::nanf(""),                    std::nanf(""),             0.2F,              "Test with random detections, no line should be found!"
   };

   for (const float32_t multiplier_radar_direction : sign_radar_direction)
   {
      // mirror the detections w.r.t. vcs_x axis
      mirror_the_detections(cvt_input, cvt_state, multiplier_radar_direction);

      /** \action
       * Call the generate_measurement() function
       */
      Generate_Measurement(cvt_input, cvt_state);

      /** \result
       * Check that both of the measurements match the expected result
       */
      check_primary_measurement_output_of_generate_measurement(cvt_state, expected_result, multiplier_radar_direction);
      check_secondary_measurement_output_of_generate_measurement(cvt_state, expected_result, multiplier_radar_direction);
   }

}

/** \purpose
 * Test the Generate_Measurement function with detections forming one clear line and some scattered points.
 * \req
 * NA
 */
TEST(f360_cvt_generate_measurement, line_measurement_is_robust_to_noises_in_case_of_1_line_presence)
{
   /** \precond
    * Set up initial conditions with detections forming one clear line and some scattered points
    */
   cvt_input.n_detections = 15;
   // Add some noises near the end of the 1st line
   for (int32_t i = 10; i < cvt_input.n_detections; i++)
   {
      cvt_input.detections[i].vcs_longpos = cvt_input.detections[9].vcs_longpos + static_cast<float32_t>(rand()) / RAND_MAX * 40.0F;
      cvt_input.detections[i].vcs_latpos = cvt_input.detections[9].vcs_latpos + static_cast<float32_t>(rand()) / RAND_MAX * 40.0F;
   }

   expected_line_param_T expected_result = {
   // f_primary_measurement_valid, f_secondary_measurement_valid, primary_msmt_trailer_angle_vcs,  primary_line_intersect,  secondary_msmt_trailer_angle_vcs, secondary_line_intersect,  epsilon_vcs_angle, test_description,
      true,                        false,                         0.43F,                            0.4F,                    std::nanf(""),                     std::nanf(""),             0.01F,             "Test with 1 trailer and noisy detections, 1 line should still be robustly found!"
   };

   for (const float32_t multiplier_radar_direction : sign_radar_direction)
   {
      // mirror the detections w.r.t. vcs_x axis
      mirror_the_detections(cvt_input, cvt_state, multiplier_radar_direction);

      /** \action
       * Call the generate_measurement() function
       */
      Generate_Measurement(cvt_input, cvt_state);

      /** \result
       * Check that both of the measurements match the expected result
       */
      check_primary_measurement_output_of_generate_measurement(cvt_state, expected_result, multiplier_radar_direction);
      check_secondary_measurement_output_of_generate_measurement(cvt_state, expected_result, multiplier_radar_direction);
   }
}

/** \purpose
 * Test the Generate_Measurement function with detections forming two clear lines and some scattered points.
 * \req
 * NA
 */
TEST(f360_cvt_generate_measurement, line_measurement_is_robust_to_noises_in_case_of_2_line_presence)
{
   /** \precond
    * Set up initial conditions with detections forming two clear lines
    */
   cvt_input.n_detections = 25;
   // Add some noises near the end of the 1st line
   for (int32_t i = 20; i < cvt_input.n_detections; i++)
   {
      cvt_input.detections[i].vcs_longpos = cvt_input.detections[19].vcs_longpos + F360_Cosf(static_cast<float32_t>(i)) * 40.0F;
      cvt_input.detections[i].vcs_latpos = cvt_input.detections[19].vcs_latpos + F360_Sinf(static_cast<float32_t>(i)) * 40.0F;
   }

   // Expected result
   expected_line_param_T expected_result = {
   // f_primary_measurement_valid, f_secondary_measurement_valid, primary_msmt_trailer_angle_vcs,  primary_line_intersect,  secondary_msmt_trailer_angle_vcs, secondary_line_intersect,  epsilon_vcs_angle, test_description,
      true,                        true,                          1.05F,                           -10.9F,                  0.44F,                            0.4F,                      0.01F,             "Test with 2 trailers and noisy detections, 2 lines should still be robustly found!"
   };

   for (const float32_t multiplier_radar_direction : sign_radar_direction)
   {
      // mirror the detections w.r.t. vcs_x axis
      mirror_the_detections(cvt_input, cvt_state, multiplier_radar_direction);

      /** \action
       * Call the generate_measurement() function
       */
      Generate_Measurement(cvt_input, cvt_state);

      /** \result
       * Check that both of the measurements match the expected result
       */
      check_primary_measurement_output_of_generate_measurement(cvt_state, expected_result, multiplier_radar_direction);
      check_secondary_measurement_output_of_generate_measurement(cvt_state, expected_result, multiplier_radar_direction);
   }
}

/** \purpose
 * Test the Generate_Measurement function with the 2nd line (larger angle) being clearer than the 1st (smaller angle).
 * the primary line should always be the smaller angle one, regardless of #detections associated
 * \req
 * NA
 */
TEST(f360_cvt_generate_measurement, the_primary_line_is_always_with_smaller_angle)
{
   /** \precond
    * Set up initial conditions with detections forming two clear lines
    */
   cvt_input.n_detections = 20;
   // Reduce the #detections of the 1st trailer line, which has smaller angle
   // Replace 4 detections of the 1st line with some of the 2nd line - To make the 2nd line best fit
   for (int32_t i = 6; i < 10; i++)
   {
      cvt_input.detections[i].vcs_longpos = cvt_input.detections[i+5].vcs_longpos + F360_Cosf(static_cast<float32_t>(i)) * 40.0F;
      cvt_input.detections[i].vcs_latpos = cvt_input.detections[i+5].vcs_latpos + F360_Sinf(static_cast<float32_t>(i)) * 40.0F;
   }


   // Expected result
   expected_line_param_T expected_result = {
   // f_primary_measurement_valid, f_secondary_measurement_valid, primary_msmt_trailer_angle_vcs,  primary_line_intersect,  secondary_msmt_trailer_angle_vcs, secondary_line_intersect,  epsilon_vcs_angle, test_description,
      true,                        true,                          0.698F,                           -4.72F,                   0.44F,                           0.4F,             0.01F,             "Test with uneven #detections created by 2 trailers, 2 lines should still be robustly found in specific order!"
   };

   for (const float32_t multiplier_radar_direction : sign_radar_direction)
   {
      // mirror the detections w.r.t. vcs_x axis
      mirror_the_detections(cvt_input, cvt_state, multiplier_radar_direction);

      /** \action
       * Call the generate_measurement() function
       */
      Generate_Measurement(cvt_input, cvt_state);

      /** \result
       * Check that both of the measurements match the expected result
       */
      check_primary_measurement_output_of_generate_measurement(cvt_state, expected_result, multiplier_radar_direction);
      check_secondary_measurement_output_of_generate_measurement(cvt_state, expected_result, multiplier_radar_direction);
   }
}

/** \purpose
 * Test the Generate_Measurement function with lines parallel to the host.
 * Such measurment should always be rejected, showing invalid flags
 * \req
 * NA
 */
TEST(f360_cvt_generate_measurement, detections_forming_small_angles_should_be_rejected)
{
   /** \precond
    * Set up initial conditions with detections forming two clear lines
    */
   cvt_input.n_detections = 20;

   // Place the two lines horizontally, such that the vcs angles are zeros
   for (int32_t i = 0; i < 10; i++)
   {
      cvt_input.detections[i].vcs_latpos = -7.0F;
   }

   for (int32_t i = 10; i < 20; i++)
   {
      cvt_input.detections[i].vcs_latpos = -13.0F;
   }

   // Expected result
   expected_line_param_T expected_result = {
   // f_primary_measurement_valid, f_secondary_measurement_valid, primary_msmt_trailer_angle_vcs,  primary_line_intersect,  secondary_msmt_trailer_angle_vcs, secondary_line_intersect,  epsilon_vcs_angle, test_description,
      false,                       false,                         std::nanf(""),                   std::nanf(""),           std::nanf(""),                    std::nanf(""),             0.01F,             "Test with two lines parallel to the host, should return invalid flag!"
   };

   for (const float32_t multiplier_radar_direction : sign_radar_direction)
   {
      // mirror the detections w.r.t. vcs_x axis
      mirror_the_detections(cvt_input, cvt_state, multiplier_radar_direction);

      /** \action
       * Call the generate_measurement() function
       */
      Generate_Measurement(cvt_input, cvt_state);

      /** \result
       * Check that both of the measurements match the expected result
       */
      check_primary_measurement_output_of_generate_measurement(cvt_state, expected_result, multiplier_radar_direction);
      check_secondary_measurement_output_of_generate_measurement(cvt_state, expected_result, multiplier_radar_direction);
   }
}
/** @}*/