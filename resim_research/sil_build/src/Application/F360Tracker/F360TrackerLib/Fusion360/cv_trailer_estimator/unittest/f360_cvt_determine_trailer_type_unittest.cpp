/** \file
 * This file contains unit tests for content of f360_cvt_determine_trailer_type.cpp file
 */

#include "f360_cvt_determine_trailer_type.h"
#include "f360_math_func.h"
#include "f360_constants.h"
#include "f360_math.h"
#include <CppUTest/TestHarness.h>
#include <vector>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

/** \defgroup  f360_cvt_determine_trailer_type_trailers_not_aligned
 *  @{
 */
using namespace f360_variant_A;
/** \brief
 * Test group for the Determine_Trailer_Type function.
 * This group tests the determination of trailer type with trailers not aligned.
 * Tests cover various conditions.
 */
TEST_GROUP(f360_cvt_determine_trailer_type_trailers_not_aligned)
{
   // Declare common variables used within all tests in this test group.
   float32_t host_speed;
   float32_t vcs_sideslip;
   F360_CVT_Detection_Info_T detections[CVT_MAX_NUMBER_OF_DETECTIONS] = {};
   F360_CVT_State_T cvt_state = {};
   const bool f_radar_left_sides_list[2] = {true, false};
   /** \setup
    * Initialize common variables before each test.
    */
   TEST_SETUP()
   {
      host_speed = 0.1F;
      vcs_sideslip = F360_DEG2RAD(6.0F);

      cvt_state.radar_vcs_latpos = -1.0F; // Left side radar

      //  one link model, 30 degrees to left
      cvt_state.one_link.ekf_state[0] = F360_DEG2RAD(30.0F);
      cvt_state.one_link.joint_vcs_longpos = -3.0F;
      cvt_state.one_link.joint_vcs_latpos = 0.0F;
      cvt_state.one_link.joint_dist_to_center = 5.0F;
      cvt_state.one_link.trailer_length = 11.0F;
      cvt_state.one_link.trailer_width = 2.55F;


      //  two link model, 1st trailer 30 degrees to left, overlaps the one-link model
      cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(30.0F);
      cvt_state.two_link.joint1_vcs_longpos = -3.0F;
      cvt_state.two_link.joint1_vcs_latpos = 0.0F;
      cvt_state.two_link.joint1_dist_to_center = 5.0F;
      cvt_state.two_link.trailer1_length = 11.0F;
      cvt_state.two_link.trailer1_width = 2.55F;

      // 2nd trailer 15 degrees further to the left.
      cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(45.0F);  // Note that the 4th state is the vcs angle of the 2nd trailer
      cvt_state.two_link.joint2_vcs_longpos = cvt_state.two_link.joint1_vcs_longpos - cvt_state.two_link.trailer1_length * F360_Cosf(cvt_state.two_link.ekf_state[0]);
      cvt_state.two_link.joint2_vcs_latpos = - cvt_state.two_link.trailer1_length * F360_Sinf(cvt_state.two_link.ekf_state[0]);
      cvt_state.two_link.joint2_dist_to_center = 6.0F;
      cvt_state.two_link.trailer2_length = 13.0F;
      cvt_state.two_link.trailer2_width = 2.55F;


      // Detections
      // 5 detections in the 1st trailer and 7 other detections in the 2nd trailer
      for (int32_t i = 0; i < 5; i++)
      {
         // along the central line of the 1st trailer
         detections[i].vcs_longpos = cvt_state.two_link.joint1_vcs_longpos - (2.0F * static_cast<float32_t>(i) + 1.0F) * F360_Cosf(cvt_state.two_link.ekf_state[0]);
         detections[i].vcs_latpos = - (2.0F * static_cast<float32_t>(i) + 1.0F) * F360_Sinf(cvt_state.two_link.ekf_state[0]);
         cvt_state.valid_det_idx[i] = i;
      }

      for (int32_t i = 0; i < 7; i++)
      {
         // along the central line of the end trailer
         detections[i+5].vcs_longpos = cvt_state.two_link.joint2_vcs_longpos - (2.0F * static_cast<float32_t>(i) + 1.0F) * F360_Cosf(cvt_state.two_link.ekf_state[3]);
         detections[i+5].vcs_latpos = cvt_state.two_link.joint2_vcs_latpos - (2.0F * static_cast<float32_t>(i) + 1.0F) * F360_Sinf(cvt_state.two_link.ekf_state[3]);
         cvt_state.valid_det_idx[i+5] = i+5;
      }

      // Add Outlier: 2 dets at the left and right outside area of the 1st trailer, respectively
      detections[12].vcs_longpos = cvt_state.two_link.joint1_vcs_longpos - (2.0F * static_cast<float32_t>(3) + 1.0F) * F360_Cosf(cvt_state.two_link.ekf_state[0]);
      detections[12].vcs_latpos = - (2.0F * static_cast<float32_t>(3) + 1.0F) * F360_Sinf(cvt_state.two_link.ekf_state[0] + F360_DEG2RAD(25.0F));
      cvt_state.valid_det_idx[12] = 12;

      detections[13].vcs_longpos = cvt_state.two_link.joint1_vcs_longpos - (2.0F * static_cast<float32_t>(3) + 1.0F) * F360_Cosf(cvt_state.two_link.ekf_state[0]);
      detections[13].vcs_latpos = - (2.0F * static_cast<float32_t>(3) + 1.0F) * F360_Sinf(cvt_state.two_link.ekf_state[0] - F360_DEG2RAD(25.0F));
      cvt_state.valid_det_idx[13] = 13;


      cvt_state.n_valid_dets = 14;
      cvt_state.best_trailer_model = TRAILER_MODEL_NOT_AVAILABLE;
      cvt_state.best_model_change_counter = 0U;
   }

   void test_both_left_and_right_side_by_mirroring_trailer(
      const float32_t host_speed,
      const float32_t vcs_sideslip,
      F360_CVT_Detection_Info_T(&detections)[CVT_MAX_NUMBER_OF_DETECTIONS],
      F360_CVT_State_T& cvt_state,
      F360_CVT_Model_Tag expected_trailer_type)
   {
      // Test for both left and right side radar, with the detections flipped
      for (const bool &f_radar_left_side : f_radar_left_sides_list)
      {
         if (f_radar_left_side)
         {
            cvt_state.radar_vcs_latpos = -1.0F;
         }
         else
         {
            cvt_state.radar_vcs_latpos = 1.0F;
            for (int32_t i = 0; i < cvt_state.n_valid_dets; i++)
            {
               detections[i].vcs_latpos = -detections[i].vcs_latpos;
            }
            cvt_state.one_link.ekf_state[0] = -cvt_state.one_link.ekf_state[0];
            cvt_state.two_link.ekf_state[0] = -cvt_state.two_link.ekf_state[0];
            cvt_state.two_link.ekf_state[3] = -cvt_state.two_link.ekf_state[3];
         }
         /** \action
          * Call the Determine_Trailer_Type function
          */
         Determine_Trailer_Type(host_speed, vcs_sideslip, detections, cvt_state);

         /** \result
          * Check that the trailer model remains unchanged
          */
         CHECK_EQUAL(expected_trailer_type, cvt_state.best_trailer_model);
      }
   }

};

/** \purpose
 * Test the Determine_Trailer_Type function with low speed.
 * \req
 * NA
 */
TEST(f360_cvt_determine_trailer_type_trailers_not_aligned, no_trailer_type_is_determined_if_low_host_speed)
{
   /** \precond
    * Set up initial conditions for low speed
    */
   host_speed = 0.005F;
   cvt_state.n_valid_dets = 0;

   const F360_CVT_Model_Tag expected_trailer_type = TRAILER_MODEL_NOT_AVAILABLE;

   /** \action and result
    * Call the Determine_Trailer_Type function for both left and right side radar,
    * check the Result against "expected_trailer_type"
    */
   test_both_left_and_right_side_by_mirroring_trailer(host_speed, vcs_sideslip, detections, cvt_state, expected_trailer_type);
}

/** \purpose
 * Test the Determine_Trailer_Type function with sufficient speed but small trailer angle.
 * \req
 * NA
 */
TEST(f360_cvt_determine_trailer_type_trailers_not_aligned, no_trailer_type_is_determined_if_small_trailer_angle)
{
   /** \precond
    * Set up initial conditions for sufficient speed but small trailer angle for 1st trailer
    */

   struct cv_trailer_small_trailer_angle_case_T
   {
      float32_t first_trailer_angle_deg;   // Angle of 1st the trailer, following Right hand rule, positive means trailer at rear left
      float32_t second_trailer_angle_deg;  // Angle of 1st the trailer, following Right hand rule, positive means trailer at rear left
      bool f_radar_left_side;
      std::string test_description;  // Descriptor of the test
   };


   host_speed = 1.0F;
   cvt_state.n_valid_dets = 1;

   std::vector<cv_trailer_small_trailer_angle_case_T> all_test_cases = {
      //first_trailer_angle_deg,  second_trailer_angle_deg,  f_radar_left_side,  test_description,
      {5.0F,                     5.0F,                       true,               "Test1: left radar, all angles are small, trailer model should be unavailable"},
      {15.0F,                    5.0F,                       true,               "Test2: left radar, 2nd angle is small, trailer model should be unavailable"},
      {5.0F,                    15.0F,                       true,               "Test3: left radar, 1st angle is small, trailer model should be unavailable"},
      {-5.0F,                   -5.0F,                       false,              "Test1: right radar, all angles are small, trailer model should be unavailable"},
      {-15.0F,                  -5.0F,                       false,              "Test2: right radar, 2nd angle is small, trailer model should be unavailable"},
      {-5.0F,                  -15.0F,                       false,              "Test3: right radar, 1st angle is small, trailer model should be unavailable"},
   };

      // Loop over all test cases
   for (const cv_trailer_small_trailer_angle_case_T &test_case_i : all_test_cases)
   {
      cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(test_case_i.first_trailer_angle_deg); // Large angle
      cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(test_case_i.second_trailer_angle_deg); // Small angle
      if (test_case_i.f_radar_left_side)
      {
         cvt_state.radar_vcs_latpos = -1.0F;
      }
      else
      {
         cvt_state.radar_vcs_latpos = 1.0F;
      }

      /** \action
       * Call the Determine_Trailer_Type function
       */
      Determine_Trailer_Type(host_speed, vcs_sideslip, detections, cvt_state);

      /** \result
       * Check that the trailer model remains unchanged due to small angle of 1st trailer
       */
      CHECK_EQUAL_TEXT(TRAILER_MODEL_NOT_AVAILABLE, cvt_state.best_trailer_model,  test_case_i.test_description.c_str());
   }
}

/** \purpose
 * Test the Determine_Trailer_Type function for one-link trailer detection.
 * \req
 * NA
 */
TEST(f360_cvt_determine_trailer_type_trailers_not_aligned, select_one_trailer_model_if_all_dets_in_1st_trailer_and_filter_initializing)
{
   /** \precond
    * Set up initial conditions for one-link trailer detection, 5 dets in total
    */
   cvt_state.one_link.filter_state = TRAILER_FILTER_STATE_INIT;
   cvt_state.two_link.filter_state = TRAILER_FILTER_STATE_INIT;

   cvt_state.n_valid_dets = 5;

   const F360_CVT_Model_Tag expected_trailer_type = TRAILER_MODEL_ONE_LINK;

   /** \action and result
    * Call the Determine_Trailer_Type function for both left and right side radar,
    * check the Result against "expected_trailer_type"
    */
   test_both_left_and_right_side_by_mirroring_trailer(host_speed, vcs_sideslip, detections, cvt_state, expected_trailer_type);
}

/** \purpose
 * Test the Determine_Trailer_Type function for one-link trailer detection.
 * \req
 * NA
 */
TEST(f360_cvt_determine_trailer_type_trailers_not_aligned, no_trailer_model_selected_if_all_dets_in_1st_trailer_but_filter_not_started)
{
   /** \precond
    * Set up initial conditions for one-link trailer detection, 5 dets in total
    */
   cvt_state.one_link.filter_state = TRAILER_FILTER_STATE_NOT_STARTED;
   cvt_state.two_link.filter_state = TRAILER_FILTER_STATE_NOT_STARTED;

   cvt_state.n_valid_dets = 5;

   const F360_CVT_Model_Tag expected_trailer_type = TRAILER_MODEL_NOT_AVAILABLE;

   /** \action and result
    * Call the Determine_Trailer_Type function for both left and right side radar,
    * check the Result against "expected_trailer_type"
    */
   test_both_left_and_right_side_by_mirroring_trailer(host_speed, vcs_sideslip, detections, cvt_state, expected_trailer_type);
}

/** \purpose
 * Test the Determine_Trailer_Type function for two-link trailer detection.
 * \req
 * NA
 */
TEST(f360_cvt_determine_trailer_type_trailers_not_aligned, two_trailer_model_is_selected_if_it_contains_4more_dets_and_filter_initializing)
{
   /** \precond
    * Set up initial conditions for two-link trailer detection, 14 dets in total
    */
   cvt_state.n_valid_dets = 14;
   cvt_state.one_link.filter_state = TRAILER_FILTER_STATE_INIT;
   cvt_state.two_link.filter_state = TRAILER_FILTER_STATE_INIT;

   const F360_CVT_Model_Tag expected_trailer_type = TRAILER_MODEL_TWO_LINK;

   /** \action and result
    * Call the Determine_Trailer_Type function for both left and right side radar,
    * check the Result against "expected_trailer_type"
    */
   test_both_left_and_right_side_by_mirroring_trailer(host_speed, vcs_sideslip, detections, cvt_state, expected_trailer_type);
}

/** \purpose
 * Test the Determine_Trailer_Type function for two-link trailer detection.
 * \req
 * NA
 */
TEST(f360_cvt_determine_trailer_type_trailers_not_aligned, no_trailer_model_is_selected_if_it_contains_4more_dets_but_filter_not_started)
{
   /** \precond
    * Set up initial conditions for two-link trailer detection, 14 dets in total
    */
   cvt_state.n_valid_dets = 14;
   cvt_state.one_link.filter_state = TRAILER_FILTER_STATE_NOT_STARTED;
   cvt_state.two_link.filter_state = TRAILER_FILTER_STATE_NOT_STARTED;

   const F360_CVT_Model_Tag expected_trailer_type = TRAILER_MODEL_NOT_AVAILABLE;

   /** \action and result
    * Call the Determine_Trailer_Type function for both left and right side radar,
    * check the Result against "expected_trailer_type"
    */
   test_both_left_and_right_side_by_mirroring_trailer(host_speed, vcs_sideslip, detections, cvt_state, expected_trailer_type);
}

/** \purpose
 * Test the Determine_Trailer_Type function for one-link trailer detection.
 * \req
 * NA
 */
TEST(f360_cvt_determine_trailer_type_trailers_not_aligned, one_trailer_model_is_selected_if_it_contains_more_dets)
{
   /** \precond
    * Set up initial conditions for two-link trailer detection, 14 dets in total
    */
   cvt_state.n_valid_dets = 14;
   cvt_state.one_link.filter_state = TRAILER_FILTER_STATE_INIT;
   cvt_state.two_link.filter_state = TRAILER_FILTER_STATE_INIT;
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(60.0F);
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(60.0F);  // Make the 2-joint model not contain any dets
   cvt_state.two_link.joint2_vcs_longpos = cvt_state.two_link.joint1_vcs_longpos - cvt_state.two_link.trailer1_length * F360_Cosf(cvt_state.two_link.ekf_state[0]);
   cvt_state.two_link.joint2_vcs_latpos = - cvt_state.two_link.trailer1_length * F360_Sinf(cvt_state.two_link.ekf_state[0]);

   const F360_CVT_Model_Tag expected_trailer_type = TRAILER_MODEL_ONE_LINK;

   /** \action and result
    * Call the Determine_Trailer_Type function for both left and right side radar,
    * check the Result against "expected_trailer_type"
    */
   test_both_left_and_right_side_by_mirroring_trailer(host_speed, vcs_sideslip, detections, cvt_state, expected_trailer_type);
}

/** \purpose
 * Test the Determine_Trailer_Type function for model change with high sideslip.
 * \req
 * NA
 */
TEST(f360_cvt_determine_trailer_type_trailers_not_aligned, model_change_threshold_lower_if_high_sideslip_change_expected)
{
   /** \precond
    * Set up initial conditions for model change with high sideslip
    */
   vcs_sideslip = F360_DEG2RAD(10.0F); // High sideslip
   cvt_state.n_valid_dets = 14;
   cvt_state.best_trailer_model = TRAILER_MODEL_ONE_LINK; // Previous selected trailer is 1-joint
   cvt_state.one_link.filter_state = TRAILER_FILTER_STATE_INIT;
   cvt_state.two_link.filter_state = TRAILER_FILTER_STATE_INIT;
   cvt_state.best_model_change_counter = 20U;

   const F360_CVT_Model_Tag expected_trailer_type = TRAILER_MODEL_TWO_LINK;

   /** \action and result
    * Call the Determine_Trailer_Type function for both left and right side radar,
    * check the Result against "expected_trailer_type"
    */
   test_both_left_and_right_side_by_mirroring_trailer(host_speed, vcs_sideslip, detections, cvt_state, expected_trailer_type);
}

/** \purpose
 * Test the Determine_Trailer_Type function for model change with low sideslip.
 * \req
 * NA
 */
TEST(f360_cvt_determine_trailer_type_trailers_not_aligned, model_change_threshold_higher_if_low_sideslip_change_not_expected)
{
   /** \precond
    * Set up initial conditions for model change with low sideslip
    */
   vcs_sideslip = F360_DEG2RAD(2.0F); // Low sideslip
   cvt_state.n_valid_dets = 14;
   cvt_state.best_trailer_model = TRAILER_MODEL_ONE_LINK; // Previous selected trailer is 1-joint
   cvt_state.one_link.filter_state = TRAILER_FILTER_STATE_INIT;
   cvt_state.two_link.filter_state = TRAILER_FILTER_STATE_INIT;
   cvt_state.best_model_change_counter = 21U;

   const F360_CVT_Model_Tag expected_trailer_type = TRAILER_MODEL_ONE_LINK;

   /** \action and result
    * Call the Determine_Trailer_Type function for both left and right side radar,
    * check the Result against "expected_trailer_type"
    */
   test_both_left_and_right_side_by_mirroring_trailer(host_speed, vcs_sideslip, detections, cvt_state, expected_trailer_type);
}

/** \purpose
 * Test the Determine_Trailer_Type function for model change with low sideslip.
 * \req
 * NA
 */
TEST(f360_cvt_determine_trailer_type_trailers_not_aligned, model_change_threshold_higher_if_low_sideslip_change_expected)
{
   /** \precond
    * Set up initial conditions for model change with low sideslip
    */
   vcs_sideslip = F360_DEG2RAD(2.0F); // Low sideslip
   cvt_state.n_valid_dets = 14;
   cvt_state.best_trailer_model = TRAILER_MODEL_ONE_LINK; // Previous selected trailer is 1-joint
   cvt_state.one_link.filter_state = TRAILER_FILTER_STATE_INIT;
   cvt_state.two_link.filter_state = TRAILER_FILTER_STATE_INIT;
   cvt_state.best_model_change_counter = 50U;

   const F360_CVT_Model_Tag expected_trailer_type = TRAILER_MODEL_TWO_LINK;

   /** \action and result
    * Call the Determine_Trailer_Type function for both left and right side radar,
    * check the Result against "expected_trailer_type"
    */
   test_both_left_and_right_side_by_mirroring_trailer(host_speed, vcs_sideslip, detections, cvt_state, expected_trailer_type);
}

/** \purpose
 * Test the Determine_Trailer_Type function for fill ratio comparison.
 * \req
 * NA
 */
TEST(f360_cvt_determine_trailer_type_trailers_not_aligned, choose_1link_if_its_fill_ratio_is_higher)
{
   /** \precond
    * Set up initial conditions for fill ratio comparison
    */
   cvt_state.best_trailer_model = TRAILER_MODEL_NOT_AVAILABLE;
   cvt_state.one_link.filter_state = TRAILER_FILTER_STATE_INIT;
   cvt_state.two_link.filter_state = TRAILER_FILTER_STATE_INIT;

   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(60.0F);

   for (int32_t i = 0; i < 3; i++)
   {
      // add 3 points inside the 2nd trailer with smaller fill ratio
      detections[i+10].vcs_longpos = cvt_state.two_link.joint2_vcs_longpos - (1.0F + static_cast<float32_t>(i)*0.5F ) * F360_Cosf(cvt_state.two_link.ekf_state[3]);
      detections[i+10].vcs_latpos = cvt_state.two_link.joint2_vcs_latpos - (1.0F + static_cast<float32_t>(i)*0.5F) * F360_Sinf(cvt_state.two_link.ekf_state[3]);
      cvt_state.valid_det_idx[i+10] = i+10;
   }
   cvt_state.n_valid_dets = 13;

   const F360_CVT_Model_Tag expected_trailer_type = TRAILER_MODEL_ONE_LINK;

   /** \action and result
    * Call the Determine_Trailer_Type function for both left and right side radar,
    * check the Result against "expected_trailer_type"
    */
   test_both_left_and_right_side_by_mirroring_trailer(host_speed, vcs_sideslip, detections, cvt_state, expected_trailer_type);
}

/** \purpose
 * Test the Determine_Trailer_Type function for fill ratio comparison.
 * \req
 * NA
 */
TEST(f360_cvt_determine_trailer_type_trailers_not_aligned, choose_2link_if_its_fill_ratio_is_higher)
{
   /** \precond
    * Set up initial conditions for fill ratio comparison
    */
   cvt_state.best_trailer_model = TRAILER_MODEL_NOT_AVAILABLE;
   cvt_state.one_link.filter_state = TRAILER_FILTER_STATE_INIT;
   cvt_state.two_link.filter_state = TRAILER_FILTER_STATE_INIT;

   //  two link model, 1st trailer 30 degrees to left, overlaps the one-link model
   cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(45.0F);
   cvt_state.two_link.joint1_vcs_longpos = -3.0F;
   cvt_state.two_link.joint1_vcs_latpos = 0.0F;
   cvt_state.two_link.joint1_dist_to_center = 5.0F;
   cvt_state.two_link.trailer1_length = 11.0F;
   cvt_state.two_link.trailer1_width = 2.55F;

   // 2nd trailer 15 degrees further to the left.
   cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(60.0F);  // Note that the 4th state is the vcs angle of the 2nd trailer
   cvt_state.two_link.joint2_vcs_longpos = cvt_state.two_link.joint1_vcs_longpos - cvt_state.two_link.trailer1_length * F360_Cosf(cvt_state.two_link.ekf_state[0]);
   cvt_state.two_link.joint2_vcs_latpos = - cvt_state.two_link.trailer1_length * F360_Sinf(cvt_state.two_link.ekf_state[0]);
   cvt_state.two_link.joint2_dist_to_center = 6.0F;
   cvt_state.two_link.trailer2_length = 13.0F;
   cvt_state.two_link.trailer2_width = 2.55F;

   for (int32_t i = 0; i < 5; i++)
   {
      // make the dets inside the 1st trailer identical in position -- leading to low fill ratio
      detections[i].vcs_longpos = detections[1].vcs_longpos;
      detections[i].vcs_latpos = detections[1].vcs_latpos;
   }

   // Skip the other dets, make 2 more dets at the head and tail of the 2nd trailer
   detections[5].vcs_longpos = cvt_state.two_link.joint2_vcs_longpos - (1.0F) * F360_Cosf(cvt_state.two_link.ekf_state[3]);
   detections[5].vcs_latpos = cvt_state.two_link.joint2_vcs_latpos - (1.0F) * F360_Sinf(cvt_state.two_link.ekf_state[3]);
   detections[6].vcs_longpos = cvt_state.two_link.joint2_vcs_longpos - (9.0F + 1.0F) * F360_Cosf(cvt_state.two_link.ekf_state[3]);
   detections[6].vcs_latpos = cvt_state.two_link.joint2_vcs_latpos - (9.0F + 1.0F) * F360_Sinf(cvt_state.two_link.ekf_state[3]);
   cvt_state.n_valid_dets = 7;

   const F360_CVT_Model_Tag expected_trailer_type = TRAILER_MODEL_TWO_LINK;

   /** \action and result
    * Call the Determine_Trailer_Type function for both left and right side radar,
    * check the Result against "expected_trailer_type"
    */
   test_both_left_and_right_side_by_mirroring_trailer(host_speed, vcs_sideslip, detections, cvt_state, expected_trailer_type);
}

/** \defgroup  f360_cvt_determine_trailer_type_trailers_aligned_and_detections_at_edge
 *  @{
 */
using namespace f360_variant_A;
/** \brief
 * Test group for the Determine_Trailer_Type function.
 * This group tests the determination of trailer type when trailers are aligned,
 * and the detections are at the edges of the trailers
 */
TEST_GROUP(f360_cvt_determine_trailer_type_trailers_aligned_and_detections_at_edge)
{
   // Declare common variables used within all tests in this test group.
   float32_t host_speed;
   float32_t vcs_sideslip;
   F360_CVT_Detection_Info_T detections[CVT_MAX_NUMBER_OF_DETECTIONS] = {};
   F360_CVT_State_T cvt_state = {};
   const bool f_radar_left_sides_list[2] = {true, false};
   /** \setup
    * Initialize common variables before each test.
    */
   TEST_SETUP()
   {
      host_speed = 2.0F;
      vcs_sideslip = F360_DEG2RAD(0.0F);

      cvt_state.radar_vcs_latpos = -1.0F; // Left side radar

      //  one link model, 30 degrees to left
      cvt_state.one_link.ekf_state[0] = F360_DEG2RAD(10.1F);
      cvt_state.one_link.joint_vcs_longpos = -3.0F;
      cvt_state.one_link.joint_vcs_latpos = 0.0F;
      cvt_state.one_link.joint_dist_to_center = 5.0F;
      cvt_state.one_link.trailer_length = 11.0F;
      cvt_state.one_link.trailer_width = 2.55F;


      //  two link model, 1st trailer 30 degrees to left, overlaps the one-link model
      cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(10.1F);
      cvt_state.two_link.joint1_vcs_longpos = -3.0F;
      cvt_state.two_link.joint1_vcs_latpos = 0.0F;
      cvt_state.two_link.joint1_dist_to_center = 5.0F;
      cvt_state.two_link.trailer1_length = 11.0F;
      cvt_state.two_link.trailer1_width = 2.55F;

      // 2nd trailer 15 degrees further to the left.
      cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(10.1F);  // Note that the 4th state is the vcs angle of the 2nd trailer
      cvt_state.two_link.joint2_vcs_longpos = cvt_state.two_link.joint1_vcs_longpos - cvt_state.two_link.trailer1_length;
      cvt_state.two_link.joint2_vcs_latpos = 0.0F;
      cvt_state.two_link.joint2_dist_to_center = 6.0F;
      cvt_state.two_link.trailer2_length = 13.0F;
      cvt_state.two_link.trailer2_width = 2.55F;


      // Detections
      // 4 detections at the edge of the 1st trailer and 8 other detections at the edge of the 2nd trailer
      for (int32_t i = 0; i < 4; i++)
      {
         // along the central line of the 1st trailer
         detections[i].vcs_longpos = (cvt_state.two_link.joint1_vcs_longpos - cvt_state.one_link.joint_dist_to_center)  * F360_Cosf(cvt_state.two_link.ekf_state[0]);
         detections[i].vcs_latpos =  -(cvt_state.two_link.trailer1_width + 0.49F)  * F360_Sinf(cvt_state.two_link.ekf_state[0]);
         cvt_state.valid_det_idx[i] = i;
      }

      for (int32_t i = 0; i < 8; i++)
      {
         // along the central line of the end trailer
         detections[i+4].vcs_longpos = (cvt_state.two_link.joint2_vcs_longpos - cvt_state.two_link.joint2_dist_to_center)  * F360_Cosf(cvt_state.two_link.ekf_state[3]);
         detections[i+4].vcs_latpos = -(cvt_state.two_link.trailer2_width + 0.49F) * F360_Sinf(cvt_state.two_link.ekf_state[3]);
         cvt_state.valid_det_idx[i+4] = i+4;
      }


      cvt_state.n_valid_dets = 12;
      cvt_state.best_trailer_model = TRAILER_MODEL_NOT_AVAILABLE;
      cvt_state.best_model_change_counter = 0U;

      cvt_state.one_link.filter_state = TRAILER_FILTER_STATE_INIT;
      cvt_state.two_link.filter_state = TRAILER_FILTER_STATE_INIT;
   }

   void test_both_left_and_right_side_by_mirroring_trailer(
      const float32_t host_speed,
      const float32_t vcs_sideslip,
      F360_CVT_Detection_Info_T(&detections)[CVT_MAX_NUMBER_OF_DETECTIONS],
      F360_CVT_State_T& cvt_state,
      F360_CVT_Model_Tag expected_trailer_type)
   {
      // Test for both left and right side radar, with the detections flipped
      for (const bool &f_radar_left_side : f_radar_left_sides_list)
      {
         if (f_radar_left_side)
         {
            cvt_state.radar_vcs_latpos = -1.0F;
         }
         else
         {
            cvt_state.radar_vcs_latpos = 1.0F;
            for (int32_t i = 0; i < cvt_state.n_valid_dets; i++)
            {
               detections[i].vcs_latpos = -detections[i].vcs_latpos;
            }
            cvt_state.one_link.ekf_state[0] = -cvt_state.one_link.ekf_state[0];
            cvt_state.two_link.ekf_state[0] = -cvt_state.two_link.ekf_state[0];
            cvt_state.two_link.ekf_state[3] = -cvt_state.two_link.ekf_state[3];
         }
         /** \action
          * Call the Determine_Trailer_Type function
          */
         Determine_Trailer_Type(host_speed, vcs_sideslip, detections, cvt_state);

         /** \result
          * Check that the trailer model remains unchanged
          */
         CHECK_EQUAL(expected_trailer_type, cvt_state.best_trailer_model);
      }
   }
};

/** \purpose
 * Test the Determine_Trailer_Type function with detections near the trailer edge.
 * \req
 * NA
 */
TEST(f360_cvt_determine_trailer_type_trailers_aligned_and_detections_at_edge, choose_nothing_if_no_type_candidates_have_more_detections)
{
   /** \precond
    * Set up initial conditions with detections near the trailer edge
    */
   // Use test group setting, change #valid detections to only 4 detections at front
   cvt_state.n_valid_dets = 5;

   const F360_CVT_Model_Tag expected_trailer_type = TRAILER_MODEL_NOT_AVAILABLE;

   /** \action and result
    * Call the Determine_Trailer_Type function for both left and right side radar,
    * check the Result against "expected_trailer_type"
    */
   test_both_left_and_right_side_by_mirroring_trailer(host_speed, vcs_sideslip, detections, cvt_state, expected_trailer_type);
}

/** \purpose
 * Test the Determine_Trailer_Type function with detections near the trailer edge.
 * \req
 * NA
 */
TEST(f360_cvt_determine_trailer_type_trailers_aligned_and_detections_at_edge, choose_2link_if_there_are_4_more_detections_outside_1st_trailer_but_within_edge_threshold)
{
   /** \precond
    * Set up initial conditions with detections near the trailer edge
    */
   // Use test group setting, change #valid detections to only 4 detections at front
   cvt_state.n_valid_dets = 4;

   /** expected result
    * Check that the trailer model is determined as one-link due to 4 more detections
    */
   const F360_CVT_Model_Tag expected_trailer_type = TRAILER_MODEL_ONE_LINK;

   /** \action and result
    * Call the Determine_Trailer_Type function for both left and right side radar,
    * check the Result against "expected_trailer_type"
    */
   test_both_left_and_right_side_by_mirroring_trailer(host_speed, vcs_sideslip, detections, cvt_state, expected_trailer_type);
}

/** \purpose
 * Test the Determine_Trailer_Type function with detections near the trailer edge.
 * \req
 * NA
 */
TEST(f360_cvt_determine_trailer_type_trailers_aligned_and_detections_at_edge, choose_1link_if_there_are_4_more_detections_outside_2nd_trailer_but_within_edge_threshold)
{
   /** \precond
    * Set up initial conditions with detections near the trailer edge
    */
   // Use test group setting

   /** expected result
    * Check that the trailer model is determined as two-link two-link due to 4 more detections
    */
   const F360_CVT_Model_Tag expected_trailer_type = TRAILER_MODEL_TWO_LINK;

   /** \action and result
    * Call the Determine_Trailer_Type function for both left and right side radar,
    * check the Result against "expected_trailer_type"
    */
   test_both_left_and_right_side_by_mirroring_trailer(host_speed, vcs_sideslip, detections, cvt_state, expected_trailer_type);
}
/** @}*/
