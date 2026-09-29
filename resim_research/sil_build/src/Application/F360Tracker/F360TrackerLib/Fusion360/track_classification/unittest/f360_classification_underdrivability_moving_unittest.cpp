/** \file
 * This file contains unit tests for content of f360_classification_underdrivability_moving.cpp file
 */

#include "f360_classification_underdrivability_moving.h"
#include "f360_object_underdrivability_classification.h"
#include "f360_clear_object_track.h"
#include "f360_get_wall_time.h"
#include <CppUTest/TestHarness.h>
#include <vector>

//#include "headerfile_needed.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  Test_Assign_Underdrivability_Status_To_Moving_Objects
 *  @{
 */

/** \brief
 * Test group of Assign_underdrivability_Status_To_Moving_Objects() function. Test checks if
 * Assign_underdrivability_Status_To_Moving_Objects() and all of
 * the subfunctions are called properly.
 */

TEST_GROUP(Test_Assign_Underdrivability_Status_To_Moving_Objects)
{
   // Declare common variables used within all tests in this test group.
   F360_Calibrations_T calib;
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS];
   F360_TRKR_TIMING_INFO_T timing_info;
   F360_Host_T host;
   const float32_t calib_ud_mov_height_threshold = 6.5F;

   /** \setup
    * Init tracker calibration
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      Clear_Object_Track(object_tracks[0]);
   }
};

/** \purpose  
 * This test checks if the object underdrivability status is set to
 * UNDERDRIVABLE_STATUS_CAN_PASS_UNDER if all of the conditions are met when objects
 * position x changes to be below the threshold.
 * This test also checks if the underdrivable probability is set correctly 
 * \req
 * NA.
 */
TEST(Test_Assign_Underdrivability_Status_To_Moving_Objects, check_if_status_is_assigned_if_conditions_met_obj_enters_area)
{
   /** \precond
    * ud status UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
    * otg_height above threshold
    * vcs position x below the threshold
    * ud_mov_cnt_underdrivable above minimal value for assignemnt of the status
   */
   object_tracks[0].underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
   object_tracks[0].vcs_position.x = calib.ud_mov_posx_max_limit - 0.01F;
   object_tracks[0].otg_height = calib_ud_mov_height_threshold + 0.01F;
   object_tracks[0].ud_mov_cnt_underdrivable = calib.ud_mov_cnt_consecutive_scans + 1U;
   object_tracks[0].ndets = 1;
   object_tracks[0].detids[0] = 1U;

   /** \action
    * call Assign_Underdrivability_Status_To_Moving_Object() function
   */
   Assign_Underdrivability_Status_To_Moving_Object(calib, host, object_tracks[0], timing_info);

   /** \result
    * Check if underdrivable_status_ocg is properly set to underdrivable.
    * Check that the assigned underdrivability probability corresponds to "UNDERDRIVABLE_STATUS_CAN_PASS_UNDER"
    */
   CHECK_TRUE(object_tracks[0].underdrivable_status_ocg == ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER);
   DOUBLES_EQUAL(calib.ud_mov_prob_can_pass_under, object_tracks[0].probability_underdrivable_ocg, F360_EPSILON);
}

/** \purpose
 * This test checks if the object underdrivability status is set to
 * UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER if all of the conditions are met when objects
 * position x changes to be above the threshold.
 * This test also checks if the underdrivable probability is set correctly
 * \req
 * NA.
 */
TEST(Test_Assign_Underdrivability_Status_To_Moving_Objects, check_if_status_is_assigned_if_conditions_met_obj_exits_area)
{
   /** \precond
    * ud status UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
    * otg_height above threshold
    * vcs position x above the threshold
    * ud_mov_cnt_underdrivable above minimal value for assignemnt of the status
   */
   object_tracks[0].underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
   object_tracks[0].vcs_position.x = calib.ud_mov_posx_max_limit + 0.01F;
   object_tracks[0].otg_height = calib_ud_mov_height_threshold + 0.01F;
   object_tracks[0].ud_mov_cnt_underdrivable = calib.ud_mov_cnt_consecutive_scans + 1U;
   object_tracks[0].ndets = 1;
   object_tracks[0].detids[0] = 1U;

   /** \action
    * call Assign_Underdrivability_Status_To_Moving_Object() function
   */
   Assign_Underdrivability_Status_To_Moving_Object(calib, host, object_tracks[0], timing_info);

   /** \result
    * Check if underdrivable_status_ocg is properly set to not to consider.
    * Check that the assigned underdrivability probability corresponds to "UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER"
    */
   CHECK_TRUE(object_tracks[0].underdrivable_status_ocg == ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER);
   DOUBLES_EQUAL(calib.ud_mov_prob_not_to_consider, object_tracks[0].probability_underdrivable_ocg, F360_EPSILON);
}

/** \purpose
 * This test checks if the counter is incremented when the track is underdrivable
 * \req
 * NA
 */
TEST(Test_Assign_Underdrivability_Status_To_Moving_Objects, check_if_cnt_underdrivable_is_incremented)
{
   /** \precond
    * make sure that the object historic_height_mean is above underdrivable threshold
    * set the cnt_ud_underdrivable counter to zero
    */

   object_tracks[0].otg_height = calib_ud_mov_height_threshold + 0.1F;
   object_tracks[0].ud_mov_cnt_underdrivable = 0U;

   /** \action
    * Call Assign_Underdrivability_Status_To_Moving_Object().
    */
   Assign_Underdrivability_Status_To_Moving_Object(calib, host, object_tracks[0], timing_info);

   /** \result
    * Verify that cnt_ud_underdrivable is incremented
    */
   CHECK_TRUE(object_tracks[0].ud_mov_cnt_underdrivable  > 0U);
}

/** \purpose
 * This test checks if the counter is set to 0 since the track is not underdrivable
 * \req
 * NA
 */
TEST(Test_Assign_Underdrivability_Status_To_Moving_Objects, check_if_cnt_underdrivable_is_reset)
{
   /** \precond
    * Make sure that the object is not underdrivable (historic_height_mean below the calibration threshold)
    * The counter is not zero to check if the value is set to zero after
    */

   object_tracks[0].otg_height = calib_ud_mov_height_threshold - 0.1F;
   object_tracks[0].ud_mov_cnt_underdrivable = 1U;
   object_tracks[0].vcs_position.x = 18.0F;  // object off the road and ego lane
   object_tracks[0].vcs_position.y = 18.0F;  // object off the road and ego lane

   /** \action
    * Call Assign_Underdrivability_Status_To_Moving_Object().
    */
   Assign_Underdrivability_Status_To_Moving_Object(calib, host, object_tracks[0], timing_info);

   /** \result
    * ud_mov_cnt_underdrivable counter did reset
    */
   CHECK_TRUE(object_tracks[0].ud_mov_cnt_underdrivable == 0U);
}

/** \purpose
 * This test checks if the track is not in front of the host and is thus not considered for underdrivability
 * vcs position x < 0
 * This test also checks if the underdrivable probability is set correctly
 * \req
 * NA.
 */
TEST(Test_Assign_Underdrivability_Status_To_Moving_Objects, check_if_obj_is_not_considered_when_behind)
{
   /** \precond
    * The track is not in front of the host and is thus not evaluated for underdrivability
    */

   object_tracks[0].vcs_position.x = calib.ud_mov_posx_min_limit - 0.1F;

   /** \action
    * Call Assign_Underdrivability_Status_To_Moving_Object().
    */
   Assign_Underdrivability_Status_To_Moving_Object(calib, host, object_tracks[0], timing_info);

   /** \result
    * underdrivable_status_ocg is properlty set as not to be considered
    * Check that the assigned underdrivability probability corresponds to "UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER"
    */
   CHECK_TRUE(object_tracks[0].underdrivable_status_ocg == ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER);
   DOUBLES_EQUAL(calib.ud_mov_prob_not_to_consider, object_tracks[0].probability_underdrivable_ocg, F360_EPSILON);
}

/** \purpose
 * This test checks if the track is too far ahead in front of the host and is thus not considered for underdrivability
 * and its status is set to 
 * (100 < vcs position x)
 * This test also checks if the underdrivable probability is set correctly
 * \req
 * NA
 */
TEST(Test_Assign_Underdrivability_Status_To_Moving_Objects, check_if_obj_is_not_considered_when_too_far)
{
   /** \precond
    * The track is too far ahead in front of the host and is thus not evaluated for underdrivability
    */

   object_tracks[0].vcs_position.x = calib.ud_mov_posx_max_limit + 0.1F;

   /** \action
    * Call Assign_Underdrivability_Status_To_Moving_Object().
    */
   Assign_Underdrivability_Status_To_Moving_Object(calib, host, object_tracks[0], timing_info);

   /** \result
    * underdrivable_status_ocg is properlty set as not to be considered
    * Check that the assigned underdrivability probability corresponds to "UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER"
    */
   CHECK_TRUE(object_tracks[0].underdrivable_status_ocg == ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER);
   DOUBLES_EQUAL(calib.ud_mov_prob_not_to_consider, object_tracks[0].probability_underdrivable_ocg, F360_EPSILON);
}

/** \purpose
 * This test checks if the track will have its underdrivability status set to UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
 * since it satisfies all conditions - i.e is within longitudinal gates of underdrivability classification, its
 * historic height mean is above the set threshold at current and sufficient number of previous scan indexes.
 * This test also checks if the underdrivable probability is set correctly
 * \req
 * NA
 */
TEST(Test_Assign_Underdrivability_Status_To_Moving_Objects, check_if_obj_is_classified_ud)
{
   /** \precond
    * Make sure that the track is considered for underdrivability
    * Make sure that the object is underdrivable
    * Make sure that there has been enough consecutive scans in which the object was underdrivable to set a new status
    */

   object_tracks[0].vcs_position.x = calib.ud_mov_posx_max_limit - 0.1F;
   object_tracks[0].otg_height = calib_ud_mov_height_threshold + 0.1F;
   object_tracks[0].ud_mov_cnt_underdrivable = calib.ud_mov_cnt_consecutive_scans + 1U;

   /** \action
    * Call Assign_Underdrivability_Status_To_Moving_Object().
    */
   Assign_Underdrivability_Status_To_Moving_Object(calib, host, object_tracks[0], timing_info);

   /** \result
    * objects underdrivable_status_ocg is properlty set as underdrivable
    * Check that the assigned underdrivability probability corresponds to "UNDERDRIVABLE_STATUS_CAN_PASS_UNDER"
    */
   CHECK_TRUE(object_tracks[0].underdrivable_status_ocg == ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER);
   DOUBLES_EQUAL(calib.ud_mov_prob_can_pass_under, object_tracks[0].probability_underdrivable_ocg, F360_EPSILON);
}

/** \purpose
 * This test checks if the track will have its underdrivability status set to UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER 
 * when all conditions are met -  it is within longitudinal gates of underdrivability classification, its
 * historic height mean is above the set threshold at current scan index, but was not above it for
 * required number of scan indexes.
 * set threshold.
 * This test also checks if the underdrivable probability is set correctly
 * \req
 * NA
 */
TEST(Test_Assign_Underdrivability_Status_To_Moving_Objects, check_if_obj_is_classified_not_ud)
{
   /** \precond
    * Make sure that the track is considered for underdrivability
    * Make sure that the object historic_height_mean is above threshold for underdrivable
    * Make sure that there has not been enough consecutive scans in which the object was underdrivable to set a new status
    */

   object_tracks[0].vcs_position.x = calib.ud_mov_posx_max_limit - 0.1F;
   object_tracks[0].otg_height = calib_ud_mov_height_threshold + 0.1F;
   object_tracks[0].otg_height = calib.ud_mov_cnt_consecutive_scans - 1U;

   /** \action
    * Call Assign_Underdrivability_Status_To_Moving_Object().
    */
   Assign_Underdrivability_Status_To_Moving_Object(calib, host, object_tracks[0], timing_info);

   /** \result
    * objects underdrivable_status_ocg is properlty set as UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
    * Check that the assigned underdrivability probability corresponds to "UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER"
    */
   CHECK_TRUE(object_tracks[0].underdrivable_status_ocg == ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER);
   DOUBLES_EQUAL(calib.ud_mov_prob_can_not_pass_under, object_tracks[0].probability_underdrivable_ocg, F360_EPSILON);
}

/** \purpose
 * This test checks if the track will have its underdrivability status set to sg::SG_Drivability_Class_T::NONDRIVABLE 
 * when all the following conditions are met
 * It is outside the longitudinal gates of underdrivability classification
 * Its historic height mean is above the set threshold at current scan index, and was above it for required number of scan indexes.
 * This test also checks if the underdrivable probability is set correctly
 * \req
 * NA
 */
TEST(Test_Assign_Underdrivability_Status_To_Moving_Objects, check_if_obj_is_classified_not_ud_outside_roi_is_ud)
{
   /** \precond
    * Make sure that the track is considered for underdrivability
    * Make sure that there has been enough consecutive scans in which the object was underdrivable to set a new status
    */
   object_tracks[0].vcs_position.x = calib.ud_mov_posx_max_limit + 0.1F;
   object_tracks[0].otg_height = calib_ud_mov_height_threshold + 0.1F;
   object_tracks[0].ud_mov_cnt_underdrivable = calib.ud_mov_cnt_consecutive_scans + 1U;

   /** \action
    * Call Assign_Underdrivability_Status_To_Moving_Object().
    */
   Assign_Underdrivability_Status_To_Moving_Object(calib, host, object_tracks[0], timing_info);

   /** \result
    * Check that drivable_status_sg is set as NONRDRIVEABLE
    * Check that the assigned underdrivability probability corresponds to 100
    */
   CHECK_TRUE(object_tracks[0].drivable_status_sg == sg::SG_Drivability_Class_T::NONDRIVABLE);
   CHECK_EQUAL(100U, object_tracks[0].drivable_confidence_sg);
}

/** \purpose
 * This test checks if the track will have its underdrivability status set to sg::SG_Drivability_Class_T::NONDRIVABLE 
 * when all the following conditions are met
 * It is within the longitudinal gates of underdrivability classification
 * Its historic height mean is below the set threshold at current scan index, but was not above it for required number of scan indexes.
 * This test also checks if the underdrivable probability is set correctly
 * \req
 * NA
 */
TEST(Test_Assign_Underdrivability_Status_To_Moving_Objects, check_if_obj_is_classified_not_ud_inside_roi_not_ud)
{
   /** \precond
    * Make sure that the track is considered for underdrivability
    * Make sure that there has not been enough consecutive scans in which the object was underdrivable to set a new status
    */
   object_tracks[0].vcs_position.x = calib.ud_mov_posx_max_limit - 0.1F;
   object_tracks[0].otg_height = calib_ud_mov_height_threshold + 0.1F;
   object_tracks[0].ud_mov_cnt_underdrivable = calib.ud_mov_cnt_consecutive_scans - 1U;

   /** \action
    * Call Assign_Underdrivability_Status_To_Moving_Object().
    */
   Assign_Underdrivability_Status_To_Moving_Object(calib, host, object_tracks[0], timing_info);

   /** \result
    * Check that drivable_status_sg is set as NONRDRIVEABLE
    * Check that the assigned underdrivability probability corresponds to 100
    */
   CHECK_TRUE(object_tracks[0].drivable_status_sg == sg::SG_Drivability_Class_T::NONDRIVABLE);
   CHECK_EQUAL(100U, object_tracks[0].drivable_confidence_sg);
}

/** \purpose
 * This test checks if the track will have its underdrivability status set to sg::SG_Drivability_Class_T::UNDERDRIVABLE 
 * when all the following conditions are met
 * It is within the longitudinal gates of underdrivability classification
 * Tts historic height mean is above the set threshold at current scan index, and was above it for required number of scan indexes.
 * This test also checks if the underdrivable probability is set correctly
 * \req
 * NA
 */
TEST(Test_Assign_Underdrivability_Status_To_Moving_Objects, check_if_obj_is_classified_ud_inside_roi)
{
   /** \precond
    * Make sure that the track is considered for underdrivability
    * Make sure that there has not been enough consecutive scans in which the object was underdrivable to set a new status
    */
   object_tracks[0].vcs_position.x = calib.ud_mov_posx_max_limit - 0.1F;
   object_tracks[0].otg_height = calib_ud_mov_height_threshold + 0.1F;
   object_tracks[0].ud_mov_cnt_underdrivable = calib.ud_mov_cnt_consecutive_scans + 1U;

   /** \action
    * Call Assign_Underdrivability_Status_To_Moving_Object().
    */
   Assign_Underdrivability_Status_To_Moving_Object(calib, host, object_tracks[0], timing_info);

   /** \result
    * Check that drivable_status_sg is set as UNDERDRIVABLE
    * Check that the assigned underdrivability probability corresponds to 100
    */
   CHECK_TRUE(object_tracks[0].drivable_status_sg == sg::SG_Drivability_Class_T::UNDERDRIVABLE);
   CHECK_EQUAL(100U, object_tracks[0].drivable_confidence_sg);
}


/** \purpose
 * This test checks if the track will have its underdrivability status set to sg::SG_Drivability_Class_T::UNDERDRIVABLE / sg::SG_Drivability_Class_T::NONDRIVABLE
 * when different conditions are met / violated in:
 * host speed
 * object otg height
 * object position / on ego lane
 * time since object initialized
 * time since object start moving
 * \req
 * NA
 */
TEST(Test_Assign_Underdrivability_Status_To_Moving_Objects, ud_classified_correctly_given_a_new_moving_object_and_host_speeds)
{

   struct object_and_host_states_ut_case_T  // The struct to save all parameters for one test
   {
      float32_t object_vcs_x;   // vcs_x position of test object
      float32_t object_vcs_y;    // vcs_y position of test object
      float32_t object_time_since_initialization;  // time since test object initialized
      float32_t object_time_since_started_move;     // time since test object started moving
      float32_t object_otg_height;     // otg height of test object
      float32_t host_speed;  // speed of host
      bool underdrivable_expected;  // reference for underdrivability test result
      std::string test_description;  // describe the test case and purpose
   };

   /** \precond
    * Make sure that the track is considered for underdrivability
    * Make sure that there has not been enough consecutive scans in which the object was underdrivable to set a new status
    */
   object_tracks[0].ud_mov_cnt_underdrivable = 0U;

   // Define all test cases: covering all different branches
   std::vector<object_and_host_states_ut_case_T> all_test_cases = {

   // object_vcs_x, object_vcs_y, object_time_since_initialization, object_time_since_started_move, object_otg_height,                    host_speed, underdrivable_expected,  test_description
     {26.0F,        0.0F,         0.56F,                            0.06F,                          calib_ud_mov_height_threshold + 0.1F, 18.1F,      true,                    "Test1: test object far from host, height threshold saturated, on ego lane, initialized for a while, just started moving, height larger than saturated threshold, host moves fast"},
     {23.0F,        0.0F,         0.56F,                            0.06F,                          6.1F,                                 18.1F,      true,                    "Test2: test object medium distance, height threshold in linear range, on ego lane, initialized for a while, just started moving, height larger than threshold, host moves fast"},
     {23.0F,        0.0F,         0.56F,                            0.06F,                          5.9F,                                 18.1F,      false,                   "Test3: test object medium distance, height threshold in linear range, on ego lane, initialized for a while, just started moving, height lower than threshold, host moves fast"},
     {10.0F,        0.0F,         0.56F,                            0.06F,                          4.1F,                                 18.1F,      true,                    "Test4: test object near host, height threshold saturated, on ego lane, initialized for a while, just started moving, height larger than saturated limit, host moves fast"},
     {10.0F,        0.0F,         0.56F,                            0.06F,                          2.7F,                                 18.1F,      false,                   "Test5: test object near host, height threshold saturated, on ego lane, initialized for a while, just started moving, height lower than saturated limit, host moves fast"},

     {25.3F,        6.0F,         0.56F,                            0.06F,                          calib_ud_mov_height_threshold + 0.1F, 18.1F,      false,                   "Test6: test object far from host, height threshold saturated, off ego lane, initialized for a while, just started moving, height larger than saturated threshold, host moves fast"},
     {22.2F,        6.0F,         0.56F,                            0.06F,                          6.0F,                                 18.1F,      false,                   "Test7: test object medium distance, height threshold in linear range, off ego lane, initialized for a while, just started moving, height larger than threshold, host moves fast"},
     {22.2F,        6.0F,         0.56F,                            0.06F,                          5.9F,                                 18.1F,      false,                   "Test8: test object medium distance, height threshold in linear range, off ego lane, initialized for a while, just started moving, height lower than threshold, host moves fast"},
     {8.0F,         6.0F,         0.56F,                            0.06F,                          4.1F,                                 18.1F,      false,                   "Test9: test object near host, height threshold saturated, off ego lane, initialized for a while, just started moving, height larger than saturated limit, host moves fast"},
     {8.0F,         6.0F,         0.56F,                            0.06F,                          2.7F,                                 18.1F,      false,                   "Test10: test object near host, height threshold saturated, off ego lane, initialized for a while, just started moving, height lower than saturated limit, host moves fast"},

     {26.0F,        0.0F,         0.51F,                            0.06F,                          calib_ud_mov_height_threshold + 0.1F, 18.1F,      false,                    "Test11: test object far from host, height threshold saturated, on ego lane, just initialized, just started moving, height larger than saturated threshold, host moves fast"},
     {23.0F,        0.0F,         0.51F,                            0.06F,                          6.0F,                                 18.1F,      false,                    "Test12: test object medium distance, height threshold in linear range, on ego lane, just initialized, just started moving, height larger than threshold, host moves fast"},
     {23.0F,        0.0F,         0.51F,                            0.06F,                          5.9F,                                 18.1F,      false,                    "Test13: test object medium distance, height threshold in linear range, on ego lane, just initialized, just started moving, height lower than threshold, host moves fast"},
     {10.0F,        0.0F,         0.51F,                            0.06F,                          4.1F,                                 18.1F,      false,                    "Test14: test object near host, height threshold saturated, on ego lane, just initialized, just started moving, height larger than saturated limit, host moves fast"},
     {10.0F,        0.0F,         0.51F,                            0.06F,                          2.7F,                                 18.1F,      false,                    "Test15: test object near host, height threshold saturated, on ego lane, just initialized, just started moving, height lower than saturated limit, host moves fast"},

     {26.0F,        0.0F,         0.56F,                            0.15F,                          calib_ud_mov_height_threshold + 0.1F, 18.1F,      false,                    "Test16: test object far from host, height threshold saturated, on ego lane, initialized for a while, started moving for a while, height larger than saturated threshold, host moves fast"},
     {23.0F,        0.0F,         0.56F,                            0.15F,                          6.0F,                                 18.1F,      false,                    "Test17: test object medium distance, height threshold in linear range, on ego lane, initialized for a while, started moving for a while, height larger than threshold, host moves fast"},
     {23.0F,        0.0F,         0.56F,                            0.15F,                          5.9F,                                 18.1F,      false,                    "Test18: test object medium distance, height threshold in linear range, on ego lane, initialized for a while, started moving for a while, height lower than threshold, host moves fast"},
     {10.0F,        0.0F,         0.56F,                            0.15F,                          4.1F,                                 18.1F,      false,                    "Test19: test object near host, height threshold saturated, on ego lane, initialized for a while, started moving for a while, height larger than saturated limit, host moves fast"},
     {10.0F,        0.0F,         0.56F,                            0.15F,                          2.7F,                                 18.1F,      false,                    "Test20: test object near host, height threshold saturated, on ego lane, initialized for a while, started moving for a while, height lower than saturated limit, host moves fast"},

     {26.0F,        0.0F,         0.56F,                            0.06F,                          calib_ud_mov_height_threshold + 0.1F, 18.0F,      false,                    "Test21: test object far from host, height threshold saturated, on ego lane, initialized for a while, just started moving, height larger than saturated threshold, host moves slow"},
     {23.0F,        0.0F,         0.56F,                            0.06F,                          6.0F,                                 18.0F,      false,                    "Test22: test object medium distance, height threshold in linear range, on ego lane, initialized for a while, just started moving, height larger than threshold, host moves slow"},
     {23.0F,        0.0F,         0.56F,                            0.06F,                          5.9F,                                 18.0F,      false,                    "Test23: test object medium distance, height threshold in linear range, on ego lane, initialized for a while, just started moving, height lower than threshold, host moves slow"},
     {10.0F,        0.0F,         0.56F,                            0.06F,                          4.1F,                                 18.0F,      false,                    "Test24: test object near host, height threshold saturated, on ego lane, initialized for a while, just started moving, height larger than saturated limit, host moves slow"},
     {10.0F,        0.0F,         0.56F,                            0.06F,                          2.7F,                                 18.0F,      false,                    "Test25: test object near host, height threshold saturated, on ego lane, initialized for a while, just started moving, height lower than saturated limit, host moves slow"},
   };

   for (const object_and_host_states_ut_case_T &test_case_i : all_test_cases)
   {
      // Arrange all test parameters for the current test case
      object_tracks[0].vcs_position.x = test_case_i.object_vcs_x;
      object_tracks[0].vcs_position.y = test_case_i.object_vcs_y;
      object_tracks[0].time_since_initialization = test_case_i.object_time_since_initialization;
      object_tracks[0].time_since_started_move = test_case_i.object_time_since_started_move;
      object_tracks[0].otg_height = test_case_i.object_otg_height;

      host.speed = test_case_i.host_speed;
      const sg::SG_Drivability_Class_T expected_underdrivability = (true == test_case_i.underdrivable_expected) ? sg::SG_Drivability_Class_T::UNDERDRIVABLE : sg::SG_Drivability_Class_T::NONDRIVABLE;

      /** \action
       * Call Assign_Underdrivability_Status_To_Moving_Object().
       */
      Assign_Underdrivability_Status_To_Moving_Object(calib, host, object_tracks[0], timing_info);

      /** \result
       * Check that drivable_status_sg is set correctly
       * Check that the assigned underdrivability probability corresponds to 100
       */
      CHECK_TRUE_TEXT(object_tracks[0].drivable_status_sg == expected_underdrivability, test_case_i.test_description.c_str());
      CHECK_EQUAL_TEXT(100U, object_tracks[0].drivable_confidence_sg, test_case_i.test_description.c_str());
   }
}

/** \purpose
 * This test checks if the track will have its underdrivability status set correctly when the object is:
 * Coasting / Not coasting
 * In / Outside of the vcs_x range
 * \req
 * NA
 */
TEST(Test_Assign_Underdrivability_Status_To_Moving_Objects, coasting_object_UD_classified_outside_the_front_area)
{

   struct object_states_ut_case_T  // The struct to save all parameters for one test
   {
      float32_t object_vcs_x;   // vcs_x position of test object
      bool coasting;
      bool underdrivable_expected;  // reference for underdrivability test result
      std::string test_description;  // describe the test case and purpose
   };

   // Define all test cases: covering all different branches

   std::vector<object_states_ut_case_T> all_test_cases = {
      // object vcs_x,   coasting,  underdrivable_expected,  test_description
      {  20.0F,          false,     true,                    "Test1: object in the front area of the host, not coasting, all other UD criteria satisfied"},
      {  -20.0F,         false,     false,                   "Test2: object outside the front area of the host, not coasting, all other UD criteria satisfied"},
      {  -20.0F,         true,      true,                    "Test3: object outside the front area of the host, coasting, all other UD criteria satisfied"},
      {  20.0F,          true,      true,                    "Test4: object in the front area of the host, coasting, all other UD criteria satisfied"},
   };

   /** \precond
    * Make sure that the track is considered for underdrivability
    * Make sure that there has not been enough consecutive scans in which the object was underdrivable to set a new status
    */
   object_tracks[0].vcs_position.y = 0.0F;
   object_tracks[0].otg_height = 6.6F;
   object_tracks[0].time_since_initialization = 0.56F;
   object_tracks[0].time_since_started_move = 0.06F;
   host.speed = 22.3F;

   for (const object_states_ut_case_T &test_case_i : all_test_cases)
   {
      // Arrange the variables for the current test
      object_tracks[0].vcs_position.x = test_case_i.object_vcs_x;
      object_tracks[0].status = (true == test_case_i.coasting) ? F360_OBJECT_STATUS_COASTED : F360_OBJECT_STATUS_UPDATED;
      const sg::SG_Drivability_Class_T expected_underdrivability = (true == test_case_i.underdrivable_expected) ? sg::SG_Drivability_Class_T::UNDERDRIVABLE : sg::SG_Drivability_Class_T::NONDRIVABLE;

      /** \action
       * Call Assign_Underdrivability_Status_To_Moving_Object().
       */
      Assign_Underdrivability_Status_To_Moving_Object(calib, host, object_tracks[0], timing_info);

      /** \result
       * Check that drivable_status_sg is set correctly
       * Check that the assigned underdrivability probability corresponds to 100
       */
      CHECK_TRUE_TEXT(object_tracks[0].drivable_status_sg == expected_underdrivability, test_case_i.test_description.c_str());
      CHECK_EQUAL_TEXT(100U, object_tracks[0].drivable_confidence_sg, test_case_i.test_description.c_str());
   }
}

/** \purpose
 * This test checks if the track will have its underdrivability threshold set correctly
 * if it is suspected of being overdrivable. It is done by checking if the ud_mov_cnt_underdrivable
 * was incremented correctly
 * \req
 * NA
 */
TEST(Test_Assign_Underdrivability_Status_To_Moving_Objects, threshold_for_overdrivable)
{

   struct object_states_ut_case_T  // The struct to save all parameters for one test
   {
      float32_t object_vcs_x;                // vcs_x position of test object
      float32_t object_vcs_y;                // vcs_y position of test object
      float32_t object_overdrivable_det_pct; // percentage of object's detections that are below the ground
      float32_t object_mov_historic_ndets;   // number of current + historic detections associated to object
      uint32_t exp_mov_cnt_underdrivable;    // expected number of counter for underdrivibility
   };

   // Define all test cases: covering all different branches
   std::vector<object_states_ut_case_T> all_test_cases = {
      // object vcs_x,   object vcs_y, object_overdrivable_det_pct,  object_mov_historic_ndets, exp_mov_cnt_underdrivable
      {  39.9F,          49.9F,        0.71F,                        20.1F,                     3U},
      {  39.9F,          49.9F,        0.69F,                        20.1F,                     0U},
      {  39.9F,          49.9F,        0.71F,                        19.9F,                     0U},
      {  40.1F,          49.9F,        0.71F,                        20.1F,                     0U},
      {  39.9F,          50.1F,        0.71F,                        20.1F,                     0U},
   };

   /** \precond
    * Make sure that the track is considered for underdrivability
    */
   object_tracks[0].otg_height = 2.1F;
   object_tracks[0].ud_mov_cnt_underdrivable = 2U;

   for (const object_states_ut_case_T &test_case_i : all_test_cases)
   {
      // Arrange the variables for the current test
      object_tracks[0].vcs_position.x = test_case_i.object_vcs_x;
      object_tracks[0].vcs_position.y = test_case_i.object_vcs_y;
      object_tracks[0].ud_overdrivable_det_pct = test_case_i.object_overdrivable_det_pct;
      object_tracks[0].ud_mov_historic_ndets = test_case_i.object_mov_historic_ndets;

      /** \action
       * Call Assign_Underdrivability_Status_To_Moving_Object().
       */
      Assign_Underdrivability_Status_To_Moving_Object(calib, host, object_tracks[0], timing_info);

      /** \result
       * Check that ud_mov_cnt_underdrivable is set correctly
       */
      CHECK_EQUAL(test_case_i.exp_mov_cnt_underdrivable, object_tracks[0].ud_mov_cnt_underdrivable)
   }
}
/** @}*/

