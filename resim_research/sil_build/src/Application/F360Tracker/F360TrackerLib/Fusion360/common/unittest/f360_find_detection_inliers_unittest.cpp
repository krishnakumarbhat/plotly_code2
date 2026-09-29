/** \file
* This file contains unit tests for content of f360_find_detection_inliers.cpp file
*/

#include "f360_find_detection_inliers.h"
#include <CppUTest/TestHarness.h>

#include "f360_clear_detections_props.h"
#include "f360_detection_wheelspin_type.h"
#include "f360_calculate_curvi_position.h"

using namespace f360_variant_A;

/** \defgroup  f360_Find_Detection_Inliers
 *  @{
 */

/** \brief
 * Testing of the function f360_Find_Detection_Inliers
 **/
TEST_GROUP(f360_Find_Detection_Inliers)
{
   // Common variables used within all tests in group
   F360_Host_T host = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Calibrations_T calibrations = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};

   const int32_t obj_id_1 = 5;
   const int32_t obj_id_2 = 74;

   const int32_t det_id_1 = 2;
   const int32_t det_id_2 = 98;

   /** \setup
    * Two non-movable object are set up, with one associated detection each.
    * The first object is associated with a detection that has a low range rate error.
    * The second object is associated with a detection that has a high range rate error.
    **/
   TEST_SETUP()
   {
      // Initialize calibrations
      Initialize_Tracker_Calibrations(calibrations);

      tracker_info.num_active_objs = 2;

      tracker_info.active_obj_ids[0] = obj_id_1;
      tracker_info.active_obj_ids[1] = obj_id_2;

      det_props[det_id_1 - 1].range_rate_predicted = 8.9F;
      det_props[det_id_1 - 1].range_rate_dealiased = 8.7F;

      det_props[det_id_2 - 1].range_rate_predicted = 1.9F;
      det_props[det_id_2 - 1].range_rate_dealiased = -1.7F;

      object_tracks[obj_id_1 - 1].movable_prob = 0.0F;
      object_tracks[obj_id_1 - 1].ndets = 1;
      object_tracks[obj_id_1 - 1].detids[0] = det_id_1;
      object_tracks[obj_id_1 - 1].filtered_hist_assoc_n_dets = 3.0F;
      object_tracks[obj_id_1 - 1].filtered_hist_assoc_det_rr_err_mean = 0.4F;
      object_tracks[obj_id_1 - 1].filtered_hist_assoc_det_rr_err_var = 0.1F;

      object_tracks[obj_id_2 - 1].movable_prob = 0.0F;
      object_tracks[obj_id_2 - 1].ndets = 1;
      object_tracks[obj_id_2 - 1].detids[0] = det_id_2;
      object_tracks[obj_id_2 - 1].filtered_hist_assoc_n_dets = 7.0F;
      object_tracks[obj_id_2 - 1].filtered_hist_assoc_det_rr_err_mean = 0.2F;
      object_tracks[obj_id_2 - 1].filtered_hist_assoc_det_rr_err_var = 0.05F;
   }
};

/** \purpose
 * Check that two objects have the correct number of inlier detections and that the detections are correctly flagged when
 * there are two objects with one detection associated each. The first object has an associated detection with a small range rate
 * error while the other object's detection has a large error.
 * \req      NA
 */
TEST(f360_Find_Detection_Inliers, Find_Detection_Inliers)
{
   /** \precond
    * Two objects and two detections have been set up in the test group.
    **/

   /** \action
    * Call function Find_Detection_Inliers
    **/
   Find_Detection_Inliers(host, tracker_info, calibrations, object_tracks, det_props);

   /** \result
    * Check that the objects have the correct number of inliers and that all detections are correctly flagged.
    **/
   CHECK_EQUAL_TEXT(1, object_tracks[obj_id_1 - 1].num_rr_inlier_dets, "Object has an incorrect number of inlier detections")
   CHECK_EQUAL_TEXT(0, object_tracks[obj_id_2 - 1].num_rr_inlier_dets, "Object has an incorrect number of inlier detections")
   for (int32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      if(i == (det_id_1 - 1))
      {
         CHECK_TRUE_TEXT(det_props[i].f_rr_inlier, "A detection was not marked as inlier when it should have been")
      }
      else
      {
         CHECK_FALSE_TEXT(det_props[i].f_rr_inlier, "A detection was wrongly marked as inlier")
      }
   }
}

/** @}*/

/** \defgroup  f360_Update_Historic_Range_Rate_Error_Statistics
 *  @{
 */

/** \brief
 * Testing of the function Update_Historic_Range_Rate_Error_Statistics
 **/
TEST_GROUP(f360_Update_Historic_Range_Rate_Error_Statistics)
{
   // Common variables used within all tests in group
   F360_Calibrations_T calibrations = {};
   float32_t abs_rdot_diffs[MAX_DETS_IN_OBJ_TRK] = {};
   uint32_t n_valid_dets;
   F360_Object_Track_T object = {};

   const float32_t test_threshold = 0.001F;

   /** \setup
    * A non-movable object is set up with:
    *    - 3 valid detections and
    *    - rdot diffs from those 3 detections in current scan: [1.0000    1.9500    0.3000]
    *    - Assumption is that the object had 4 detection is the previous scans
    *      with rdot diffs = [0.2000    0.1000    0.3000    0.4000] such that
    *    - number of historic dets = 4
    *    - historic mean = 0.25
    *    - and historic variance = 0.0167
    **/
   TEST_SETUP()
   {
      // Initialize calibrations
      Initialize_Tracker_Calibrations(calibrations);

      object.movable_prob = 0.0F;
      object.filtered_hist_assoc_n_dets = 4.0F;
      object.filtered_hist_assoc_det_rr_err_mean = 0.25F;
      object.filtered_hist_assoc_det_rr_err_var = 0.0167F;

      n_valid_dets = 3U;
      abs_rdot_diffs[0] = 1.0F;
      abs_rdot_diffs[1] = 1.95F;
      abs_rdot_diffs[2] = 0.3F;
   }
};

/** \purpose
 * Check that, when setting forgetting factor to 0, i.e. ignoring history, the calculated new mean and variance is corretly using only
 * range rate diffs from current detections.
 * \req      NA
 */
TEST(f360_Update_Historic_Range_Rate_Error_Statistics, Update_Historic_Range_Rate_Error_Statistics_No_History)
{
   /** \precond
    * Range rate diffs and historical info has been set up in test group.
    * Set forgetting factor to 0.
    * Set expected data such that it only includes current detection info
    * - number of historic dets = 3
    * - historic mean = mean(1.0, 1.95, 0.3) = 1.0833
    * - historic var = var(1.0, 1.95, 0.3) = 0.6858
    **/
   calibrations.k_rr_error_statistics_forgetting_factor = 0.0F;
   
   const float32_t exp_filtered_hist_assoc_n_dets = 3.0F;
   const float32_t exp_filtered_hist_assoc_det_rr_err_mean = 1.08333F;
   const float32_t exp_filtered_hist_assoc_det_rr_err_var = 0.68583F;

   /** \action
    * Call function Update_Historic_Range_Rate_Error_Statistics
    **/
   Update_Historic_Range_Rate_Error_Statistics(calibrations, abs_rdot_diffs, n_valid_dets, object);

   /** \result
    * Check that the updated historic properties are correct.
    **/
   DOUBLES_EQUAL_TEXT(exp_filtered_hist_assoc_n_dets, object.filtered_hist_assoc_n_dets, test_threshold, "Updated filtered_hist_assoc_n_dets is incorrect.")
   DOUBLES_EQUAL_TEXT(exp_filtered_hist_assoc_det_rr_err_mean, object.filtered_hist_assoc_det_rr_err_mean, test_threshold, "Updated filtered_hist_assoc_det_rr_err_mean is incorrect.")
   DOUBLES_EQUAL_TEXT(exp_filtered_hist_assoc_det_rr_err_var, object.filtered_hist_assoc_det_rr_err_var, test_threshold, "Updated filtered_hist_assoc_det_rr_err_var is incorrect.")
}

/** \purpose
 * Check that, when setting forgetting factor to 1, i.e. using all history, the calculated new mean and variance is corretly using
 * range rate error information from previous and current scan with equal influence.
 * \req      NA
 */
TEST(f360_Update_Historic_Range_Rate_Error_Statistics, Update_Historic_Range_Rate_Error_Statistics_No_Forgetting_Factor)
{
   /** \precond
    * Range rate diffs and historical info has been set up in test group.
    * Set forgetting factor to 1.
    * Set expected data such that it includes current and historic detection info
    * - number of historic dets = 7
    * - historic mean = mean(hist and current dets rr error) = 0.60714
    * - historic var = var(hist and current dets rr error) = 0.43536
    **/
   calibrations.k_rr_error_statistics_forgetting_factor = 1.0F;
   
   const float32_t exp_filtered_hist_assoc_n_dets = 7.0F;
   const float32_t exp_filtered_hist_assoc_det_rr_err_mean = 0.607142857F;
   const float32_t exp_filtered_hist_assoc_det_rr_err_var = 0.435357143F;

   /** \action
    * Call function Update_Historic_Range_Rate_Error_Statistics
    **/
   Update_Historic_Range_Rate_Error_Statistics(calibrations, abs_rdot_diffs, n_valid_dets, object);

   /** \result
    * Check that the updated historic properties are correct.
    **/
   DOUBLES_EQUAL_TEXT(exp_filtered_hist_assoc_n_dets, object.filtered_hist_assoc_n_dets, test_threshold, "Updated filtered_hist_assoc_n_dets is incorrect.")
   DOUBLES_EQUAL_TEXT(exp_filtered_hist_assoc_det_rr_err_mean, object.filtered_hist_assoc_det_rr_err_mean, test_threshold, "Updated filtered_hist_assoc_det_rr_err_mean is incorrect.")
   DOUBLES_EQUAL_TEXT(exp_filtered_hist_assoc_det_rr_err_var, object.filtered_hist_assoc_det_rr_err_var, test_threshold, "Updated filtered_hist_assoc_det_rr_err_var is incorrect.")
}

/** \purpose
 * Check that the number of historic detections is saturated correctly for a movable object.
 * \req      NA
 */
TEST(f360_Update_Historic_Range_Rate_Error_Statistics, Update_Historic_Range_Rate_Error_Statistics_Num_Hist_Dets_Saturated_Obj_Movable)
{
   /** \precond
    * Range rate diffs and historical info has been set up in test group.
    * Set object movable_prob = 1.0.
    * Set forgetting factor to 1.
    * Set expected data such that it includes current and historic detection info
    * - number of historic dets = 4 (due to saturation, would have been 7 otherwise)
    * - historic mean = mean(hist and current dets rr error) = 0.60714
    * - historic var = var(hist and current dets rr error) = 0.43536
    **/
   calibrations.k_rr_error_statistics_forgetting_factor = 1.0F;
   object.movable_prob = 1.0F;
   
   const float32_t exp_filtered_hist_assoc_n_dets = 4.0F;
   const float32_t exp_filtered_hist_assoc_det_rr_err_mean = 0.607142857F;
   const float32_t exp_filtered_hist_assoc_det_rr_err_var = 0.435357143F;

   /** \action
    * Call function Update_Historic_Range_Rate_Error_Statistics
    **/
   Update_Historic_Range_Rate_Error_Statistics(calibrations, abs_rdot_diffs, n_valid_dets, object);

   /** \result
    * Check that the updated historic properties are correct.
    **/
   DOUBLES_EQUAL_TEXT(exp_filtered_hist_assoc_n_dets, object.filtered_hist_assoc_n_dets, test_threshold, "Updated filtered_hist_assoc_n_dets is incorrect.")
   DOUBLES_EQUAL_TEXT(exp_filtered_hist_assoc_det_rr_err_mean, object.filtered_hist_assoc_det_rr_err_mean, test_threshold, "Updated filtered_hist_assoc_det_rr_err_mean is incorrect.")
   DOUBLES_EQUAL_TEXT(exp_filtered_hist_assoc_det_rr_err_var, object.filtered_hist_assoc_det_rr_err_var, test_threshold, "Updated filtered_hist_assoc_det_rr_err_var is incorrect.")
}

/** \purpose
 * Check that the number of historic detections is not saturated a for movable object when the number does not exceed the upper limit.
 * \req      NA
 */
TEST(f360_Update_Historic_Range_Rate_Error_Statistics, Update_Historic_Range_Rate_Error_Statistics_Num_Hist_Dets_Not_Saturated_Obj_Movable)
{
   /** \precond
    * Range rate diffs and historical info has been set up in test group.
    * Set object movable_prob = 1.0.
    * Set forgetting factor to 0, such that only current detections are considered.
    * Set historic values for mean, variance and n dets to 0.
    * Set expected data such that it includes only current detection info
    * - number of historic dets = 3 (i.e. not saturated)
    * - historic mean = mean(current dets) = 0.60714
    * - historic var = var(current dets) = 0.43536
    **/
   calibrations.k_rr_error_statistics_forgetting_factor = 0.0F;
   object.movable_prob = 1.0F;
   object.filtered_hist_assoc_n_dets = 0.0F;
   object.filtered_hist_assoc_det_rr_err_mean = 0.0F;
   object.filtered_hist_assoc_det_rr_err_var = 0.0F;
   
   const float32_t exp_filtered_hist_assoc_n_dets = 3.0F;
   const float32_t exp_filtered_hist_assoc_det_rr_err_mean = 1.0833333F;
   const float32_t exp_filtered_hist_assoc_det_rr_err_var = 0.6858333F;

   /** \action
    * Call function Update_Historic_Range_Rate_Error_Statistics
    **/
   Update_Historic_Range_Rate_Error_Statistics(calibrations, abs_rdot_diffs, n_valid_dets, object);

   /** \result
    * Check that the updated historic properties are correct.
    **/
   DOUBLES_EQUAL_TEXT(exp_filtered_hist_assoc_n_dets, object.filtered_hist_assoc_n_dets, test_threshold, "Updated filtered_hist_assoc_n_dets is incorrect.")
   DOUBLES_EQUAL_TEXT(exp_filtered_hist_assoc_det_rr_err_mean, object.filtered_hist_assoc_det_rr_err_mean, test_threshold, "Updated filtered_hist_assoc_det_rr_err_mean is incorrect.")
   DOUBLES_EQUAL_TEXT(exp_filtered_hist_assoc_det_rr_err_var, object.filtered_hist_assoc_det_rr_err_var, test_threshold, "Updated filtered_hist_assoc_det_rr_err_var is incorrect.")
}

/** \purpose
 * Check that the number of historic detections is saturated correctly for a non-movable object.
 * \req      NA
 */
TEST(f360_Update_Historic_Range_Rate_Error_Statistics, Update_Historic_Range_Rate_Error_Statistics_Num_Hist_Dets_Saturated_Obj_Non_Movable)
{
   /** \precond
    * Range rate diffs and historical info has been set up in test group.
    * Object movable_prob is set to false in test group.
    * Set forgetting factor to 1.
    * Add 4 detections (with rr error = 0) such that total number of dets exceed max for non-moveable objects
    * Set expected data such that it includes current and historic detection info
    * - number of historic dets = 10 (due to saturation, would have been 11 otherwise)
    * - historic mean = mean(hist and current dets rr error) = 0.60714
    * - historic var = var(hist and current dets rr error) = 0.43536
    **/
   calibrations.k_rr_error_statistics_forgetting_factor = 1.0F;
   n_valid_dets = 7U;
   abs_rdot_diffs[3] = 0.0F;
   abs_rdot_diffs[4] = 0.0F;
   abs_rdot_diffs[5] = 0.0F;
   abs_rdot_diffs[6] = 0.0F;
   
   const float32_t exp_filtered_hist_assoc_n_dets = 8.0F;
   const float32_t exp_filtered_hist_assoc_det_rr_err_mean = 0.3863636F;
   const float32_t exp_filtered_hist_assoc_det_rr_err_var = 0.35504545F;

   /** \action
    * Call function Update_Historic_Range_Rate_Error_Statistics
    **/
   Update_Historic_Range_Rate_Error_Statistics(calibrations, abs_rdot_diffs, n_valid_dets, object);

   /** \result
    * Check that the updated historic properties are correct.
    **/
   DOUBLES_EQUAL_TEXT(exp_filtered_hist_assoc_n_dets, object.filtered_hist_assoc_n_dets, test_threshold, "Updated filtered_hist_assoc_n_dets is incorrect.")
   DOUBLES_EQUAL_TEXT(exp_filtered_hist_assoc_det_rr_err_mean, object.filtered_hist_assoc_det_rr_err_mean, test_threshold, "Updated filtered_hist_assoc_det_rr_err_mean is incorrect.")
   DOUBLES_EQUAL_TEXT(exp_filtered_hist_assoc_det_rr_err_var, object.filtered_hist_assoc_det_rr_err_var, test_threshold, "Updated filtered_hist_assoc_det_rr_err_var is incorrect.")
}

/** \purpose
 * Check that when the calculated new historic variance exceeds the maximum value, it is saturated.
 * \req      NA
 */
TEST(f360_Update_Historic_Range_Rate_Error_Statistics, Update_Historic_Range_Rate_Error_Statistics_Hist_Var_Saturated)
{
   /** \precond
    * Range rate diffs and historical info has been set up in test group.
    * Set forgetting factor to 0.
    * Change current range rate errors to have very high variance.
    * Set expected data such that it only includes current detection info
    * - number of historic dets = 3
    * - historic mean = 5.0
    * - historic var = 2.0 (because of saturation, would be 25 otherwise)
    **/
   calibrations.k_rr_error_statistics_forgetting_factor = 0.0F;
   abs_rdot_diffs[0] = 0.0F;
   abs_rdot_diffs[1] = 5.0F;
   abs_rdot_diffs[2] = 10.0F;
   
   const float32_t exp_filtered_hist_assoc_n_dets = 3.0F;
   const float32_t exp_filtered_hist_assoc_det_rr_err_mean = 5.0F;
   const float32_t exp_filtered_hist_assoc_det_rr_err_var = 2.0F;

   /** \action
    * Call function Update_Historic_Range_Rate_Error_Statistics
    **/
   Update_Historic_Range_Rate_Error_Statistics(calibrations, abs_rdot_diffs, n_valid_dets, object);

   /** \result
    * Check that the updated historic properties are correct.
    **/
   DOUBLES_EQUAL_TEXT(exp_filtered_hist_assoc_n_dets, object.filtered_hist_assoc_n_dets, test_threshold, "Updated filtered_hist_assoc_n_dets is incorrect.")
   DOUBLES_EQUAL_TEXT(exp_filtered_hist_assoc_det_rr_err_mean, object.filtered_hist_assoc_det_rr_err_mean, test_threshold, "Updated filtered_hist_assoc_det_rr_err_mean is incorrect.")
   DOUBLES_EQUAL_TEXT(exp_filtered_hist_assoc_det_rr_err_var, object.filtered_hist_assoc_det_rr_err_var, test_threshold, "Updated filtered_hist_assoc_det_rr_err_var is incorrect.")
}

/** \purpose
 * Check that when the updated filtered number of historic detections is below 2, variance is not updated. 
 * \req      NA
 */
TEST(f360_Update_Historic_Range_Rate_Error_Statistics, Update_Historic_Range_Rate_Error_Statistics_Var_Not_Updated)
{
   /** \precond
    * Range rate diffs and historical info has been set up in test group.
    * Set forgetting factor to 0 such that only current detections are considered.
    * Set number of valid detections to 2
    * Set expected data such that it only includes current detection info
    * - number of historic dets = 3
    * - historic mean = 1.47500
    * - historic var = 0.0167 (unchanged)
    **/
   calibrations.k_rr_error_statistics_forgetting_factor = 0.0F;
   n_valid_dets = 2U;
   
   const float32_t exp_filtered_hist_assoc_n_dets = 2.0F;
   const float32_t exp_filtered_hist_assoc_det_rr_err_mean = 1.47500F;
   const float32_t exp_filtered_hist_assoc_det_rr_err_var = object.filtered_hist_assoc_det_rr_err_var;

   /** \action
    * Call function Update_Historic_Range_Rate_Error_Statistics
    **/
   Update_Historic_Range_Rate_Error_Statistics(calibrations, abs_rdot_diffs, n_valid_dets, object);

   /** \result
    * Check that the updated historic properties are correct.
    **/
   DOUBLES_EQUAL_TEXT(exp_filtered_hist_assoc_n_dets, object.filtered_hist_assoc_n_dets, test_threshold, "Updated filtered_hist_assoc_n_dets is incorrect.")
   DOUBLES_EQUAL_TEXT(exp_filtered_hist_assoc_det_rr_err_mean, object.filtered_hist_assoc_det_rr_err_mean, test_threshold, "Updated filtered_hist_assoc_det_rr_err_mean is incorrect.")
   DOUBLES_EQUAL_TEXT(exp_filtered_hist_assoc_det_rr_err_var, object.filtered_hist_assoc_det_rr_err_var, test_threshold, "Updated filtered_hist_assoc_det_rr_err_var is incorrect.")
}


/** \defgroup  f360_Is_Obj_In_Curvi_Roi
 *  @{
 */

/** \brief
 * Testing of f360_Is_Obj_In_Curvi_Roi. The purpose is to test that this function correctly
 * checks if an object is in the the region of interest. 
 **/
TEST_GROUP(f360_Is_Obj_In_Curvi_Roi)
{
   // Common variables used within all tests
   F360_Host_T host = {};
   F360_Object_Track_T object_track = {};

   /** \setup
    * Specify object position and host curvature rear
    **/
   TEST_SETUP()
   {
      object_track.vcs_position.x = 10.0F;
      object_track.vcs_position.y = 0.0F;
      host.curvature_rear = 0.0F;
   }
};

/** \purpose
 * Check that the given object is outside of curvature region of interest
 * \req      NA
 */
TEST(f360_Is_Obj_In_Curvi_Roi, f360_Is_Obj_In_Curvi_Roi_Long_Max)
{
   /** \precond
    * Set longitudal position to be outside of ROI
    **/
   object_track.vcs_position.x = 100.0F;

   /** \action
    * Call function Is_Obj_In_Curvi_Roi
    **/
   bool f_is_obj_in_roi = Is_Obj_In_Curvi_Roi(host, object_track);

   /** \result
    * Check that the object was correctly flagged as outside of ROI.
    **/
   CHECK_FALSE_TEXT(f_is_obj_in_roi, "Object was incorrectly flagged as in the region of interest");
}

/** \purpose
 * Check that the given object is outside of curvature region of interest
 * \req      NA
 */
TEST(f360_Is_Obj_In_Curvi_Roi, f360_Is_Obj_In_Curvi_Roi_Long_Min)
{
   /** \precond
    * Set longitudal position to be outside of ROI
    **/
   object_track.vcs_position.x = -100.0F;

   /** \action
    * Call function Is_Obj_In_Curvi_Roi
    **/
   bool f_is_obj_in_roi = Is_Obj_In_Curvi_Roi(host, object_track);

   /** \result
    * Check that the object was correctly flagged as outside of ROI.
    **/
   CHECK_FALSE_TEXT(f_is_obj_in_roi, "Object was incorrectly flagged as in the region of interest");
}


/** \defgroup  f360_Is_Obj_In_Rect_Roi
 *  @{
 */

/** \brief
 * Testing of f360_Is_Obj_In_Rect_Roi. The purpose is to test that this function correctly
 * checks if an object is in the the region of interest. 
 **/
TEST_GROUP(f360_Is_Obj_In_Rect_Roi)
{
   // Common variables used within all tests
   F360_Object_Track_T object_track = {};

   /** \setup
    * Specify object position
    **/
   TEST_SETUP()
   {
      object_track.vcs_position.x = 10.0F;
      object_track.vcs_position.y = 0.0F;
   }
};

/** \purpose
 * Check that the given object is outside of rectangular region of interest
 * \req      NA
 */
TEST(f360_Is_Obj_In_Rect_Roi, f360_Is_Obj_In_Rect_Roi_Long_Max)
{
   /** \precond
    * Set longitudal position to be outside of ROI
    **/
   object_track.vcs_position.x = 100.0F;

   /** \action
    * Call function Is_Obj_In_Rect_Roi
    **/
   bool f_is_obj_in_roi = Is_Obj_In_Rect_Roi(object_track);

   /** \result
    * Check that the object was correctly flagged as outside of ROI.
    **/
   CHECK_FALSE_TEXT(f_is_obj_in_roi, "Object was incorrectly flagged as in the region of interest");
}

/** \purpose
 * Check that the given object is outside of rectangular region of interest
 * \req      NA
 */
TEST(f360_Is_Obj_In_Rect_Roi, f360_Is_Obj_In_Rect_Roi_Long_Min)
{
   /** \precond
    * Set longitudal position to be outside of ROI
    **/
   object_track.vcs_position.x = -100.0F;

   /** \action
    * Call function Is_Obj_In_Rect_Roi
    **/
   bool f_is_obj_in_roi = Is_Obj_In_Rect_Roi(object_track);

   /** \result
    * Check that the object was correctly flagged as outside of ROI.
    **/
   CHECK_FALSE_TEXT(f_is_obj_in_roi, "Object was incorrectly flagged as in the region of interest");
}


/** @}*/

/** \defgroup  f360_select_detection_inliers
 *  @{
 */

/** \brief
 * Testing of the function that Select_Detection_Inliers
 **/
TEST_GROUP(f360_select_detection_inliers)
{
   // Common variables used within all tests in group
   F360_Host_T host = {};
   F360_Calibrations_T calibs = {};
   uint32_t valid_det_indices[MAX_DETS_IN_OBJ_TRK] = {};
   uint32_t n_valid_dets;
   float32_t abs_rdot_diffs[MAX_DETS_IN_OBJ_TRK] = {};
   F360_Object_Track_T object_track = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};

   /** \setup
    * An object is set up with
    * - Filtered range rate error mean and variance 0.6 and 0.25 respectively
    * The object has 4 assumed associated detections with different range rate errors.
    **/
   TEST_SETUP()
   {
      // Initialize calibrations
      Initialize_Tracker_Calibrations(calibs);

      n_valid_dets = 4U;
      valid_det_indices[0] = 30U;
      valid_det_indices[1] = 20U;
      valid_det_indices[2] = 10U;
      valid_det_indices[3] = 0U;

      abs_rdot_diffs[0] = 3.0F;
      abs_rdot_diffs[1] = 1.0F;
      abs_rdot_diffs[2] = 2.0F;
      abs_rdot_diffs[3] = 0.1F;

      object_track.filtered_hist_assoc_det_rr_err_mean = 0.6F;
      object_track.filtered_hist_assoc_det_rr_err_var = 0.25;
   }
};

/** \purpose
 * Test that 2 out of the 4 detections are correctly selected as inliers for the standard case when the threshold is not saturated.
 * \req      NA
 */
TEST(f360_select_detection_inliers, Select_Detection_Inliers_No_Threshold_Saturation)
{
   /** \precond
    * Object and detection properties have been set up in the test group.
    * Set expected number of inliers to 2.
    **/
   const int32_t exp_num_rr_inlier_dets = 2;

   /** \action
    * Call function Select_Detection_Inliers
    **/
   Select_Detection_Inliers(host, calibs, valid_det_indices, n_valid_dets, abs_rdot_diffs, object_track, det_props);

   /** \result
    * Check that the correct detections are selected as inliers and that the correct detections are flagged as inliers.
    **/
   CHECK_EQUAL_TEXT(exp_num_rr_inlier_dets, object_track.num_rr_inlier_dets, "Incorrect number of inlier detections selected.")
   for (uint32_t i = 0U; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      if ((i == 0U) || (i == 20U))
      {
         CHECK_TRUE_TEXT(det_props[i].f_rr_inlier, "Detection was incorrectly not flagged as a range rate inlier")
      }
      else
      {
         CHECK_FALSE_TEXT(det_props[i].f_rr_inlier, "Detection was incorrectly flagged as a range rate inlier")
      }
   }
}

/** \purpose
 * Test that 1 out of the 4 detections are correctly selected as inliers for the when the threshold is saturated to the minumum value.
 * \req      NA
 */
TEST(f360_select_detection_inliers, Select_Detection_Inliers_Threshold_Saturation)
{
   /** \precond
    * Object and detection properties have been set up in the test group.
    * Set historic range rate error mean to 0 and variance to 0.001
    * Set expected number of inliers to 1.
    **/
   object_track.filtered_hist_assoc_det_rr_err_mean = 0.0F;
   object_track.filtered_hist_assoc_det_rr_err_var = 0.001F;

   const int32_t exp_num_rr_inlier_dets = 1;

   /** \action
    * Call function Select_Detection_Inliers
    **/
   Select_Detection_Inliers(host, calibs, valid_det_indices, n_valid_dets, abs_rdot_diffs, object_track, det_props);

   /** \result
    * Check that the correct detections are selected as inliers and that the correct detections are flagged as inliers.
    **/
   CHECK_EQUAL_TEXT(exp_num_rr_inlier_dets, object_track.num_rr_inlier_dets, "Incorrect number of inlier detections selected.")
   for (uint32_t i = 0U; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      if (i == 0U)
      {
         CHECK_TRUE_TEXT(det_props[i].f_rr_inlier, "Detection was incorrectly not flagged as a range rate inlier")
      }
      else
      {
         CHECK_FALSE_TEXT(det_props[i].f_rr_inlier, "Detection was incorrectly flagged as a range rate inlier")
      }
   }
}

/** \purpose
 * Test that a singe detection with range rate error equal to the historic mean is not selected
 * when the variance is negative, and thus safely set to 0.  
 * \req      NA
 */
TEST(f360_select_detection_inliers, Select_Detection_Inliers_Negative_Variance)
{
   /** \precond
    * Object and detection properties have been set up in the test group.
    * Set number of valid detections to 1
    * Set the range rate error of that detection to 1
    * Set historic range rate error mean to 1 and variance to -0.001
    * Set expected number of inliers to 0.
    **/
   n_valid_dets = 1;
   abs_rdot_diffs[0] = 1.0F;
   object_track.filtered_hist_assoc_det_rr_err_mean = 1.0F;
   object_track.filtered_hist_assoc_det_rr_err_var = -0.001F;

   const int32_t exp_num_rr_inlier_dets = 0;

   /** \action
    * Call function Select_Detection_Inliers
    **/
   Select_Detection_Inliers(host, calibs, valid_det_indices, n_valid_dets, abs_rdot_diffs, object_track, det_props);

   /** \result
    * Check that the correct detections are selected as inliers and that the correct detections are flagged as inliers.
    **/
   CHECK_EQUAL_TEXT(exp_num_rr_inlier_dets, object_track.num_rr_inlier_dets, "Incorrect number of inlier detections selected.")
   for (uint32_t i = 0U; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      CHECK_FALSE_TEXT(det_props[i].f_rr_inlier, "Detection was incorrectly flagged as a range rate inlier")
   }
}

/** \purpose
 * Test that a 2 out of 4 detections of stationary object are correctly selected as inliers when the host is moving
 * and object is in the zone near the host
 * \req      NA
 */
TEST(f360_select_detection_inliers, Select_Detection_Inliers_Moving_Host_Stationary_Object_In_Zone)
{
   /** \precond
    * Object and detection properties have been set up in the test group.
    * Set expected number of inliers to 2.
    * Set host speed to 20 and curvature rear to 0
    * Set object track movable probability to 0
    * Set object track position x to 10 and y to 0
    * Set object track reduced id to 10
    * Set historic range rate error mean to 0.58
    **/
   const int32_t exp_num_rr_inlier_dets = 2;
   host.speed = 20.0F;
   host.curvature_rear = 0.0F;
   object_track.movable_prob = 0.0F;
   object_track.reduced_id = 10U;
   object_track.vcs_position.x = 10.0F;
   object_track.vcs_position.y = 0.0F;

   object_track.filtered_hist_assoc_det_rr_err_mean = 0.48F;

   /** \action
    * Call function Select_Detection_Inliers
    **/
   Select_Detection_Inliers(host, calibs, valid_det_indices, n_valid_dets, abs_rdot_diffs, object_track, det_props);

   /** \result
    * Check that the correct detections are selected as inliers and that the correct detections are flagged as inliers.
    **/
   CHECK_EQUAL_TEXT(exp_num_rr_inlier_dets, object_track.num_rr_inlier_dets, "Incorrect number of inlier detections selected.")
   for (uint32_t i = 0U; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      if ((i == 0U) || (i == 20U))
      {
         CHECK_TRUE_TEXT(det_props[i].f_rr_inlier, "Detection was incorrectly not flagged as a range rate inlier")
      }
      else
      {
         CHECK_FALSE_TEXT(det_props[i].f_rr_inlier, "Detection was incorrectly flagged as a range rate inlier")
      }
   }
}

/** \purpose
 * Test that a 1 out of 4 detections of stationary object are correctly selected as inliers when the host is moving
 * and object long position is bigger than threshold, so the object is not in the zone
 * \req      NA
 */
TEST(f360_select_detection_inliers, Select_Detection_Inliers_Moving_Host_Stationary_Object_Outside_Of_Zone)
{
   /** \precond
    * Object and detection properties have been set up in the test group.
    * Set expected number of inliers to 2.
    * Set host speed to 0 and curvature rear to 0
    * Set object track movable probability to 0
    * Set object track position x to 10 and y to 0
    * Set object track reduced id to 10
    * Set historic range rate error mean to 0.58
    **/
   const int32_t exp_num_rr_inlier_dets = 1;
   host.speed = 20.0F;
   host.curvature_rear = 0.0F;
   object_track.movable_prob = 0.0F;
   object_track.reduced_id = 10U;
   object_track.vcs_position.x = 10.0F;
   object_track.vcs_position.y = 100.0F;

   object_track.filtered_hist_assoc_det_rr_err_mean = 0.48F;

   /** \action
    * Call function Select_Detection_Inliers
    **/
   Select_Detection_Inliers(host, calibs, valid_det_indices, n_valid_dets, abs_rdot_diffs, object_track, det_props);

   /** \result
    * Check that the correct detections are selected as inliers and that the correct detections are flagged as inliers.
    **/
   CHECK_EQUAL_TEXT(exp_num_rr_inlier_dets, object_track.num_rr_inlier_dets, "Incorrect number of inlier detections selected.")
   for (uint32_t i = 0U; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      if ((i == 0U))
      {
         CHECK_TRUE_TEXT(det_props[i].f_rr_inlier, "Detection was incorrectly not flagged as a range rate inlier")
      }
      else
      {
         CHECK_FALSE_TEXT(det_props[i].f_rr_inlier, "Detection was incorrectly flagged as a range rate inlier")
      }
   }
}

/** \purpose
 * Test that a 2 out of 4 detections of stationary object are correctly selected as inliers when the host is stationary
 * and object is in the zone near the host
 * \req      NA
 */
TEST(f360_select_detection_inliers, Select_Detection_Inliers_Stationary_Host_Stationary_Object_In_Zone)
{
   /** \precond
    * Object and detection properties have been set up in the test group.
    * Set expected number of inliers to 2.
    * Set host speed to 0 and curvature rear to 0
    * Set object track movable probability to 0
    * Set object track position x to 10 and y to 0
    * Set object track reduced id to 10
    * Set historic range rate error mean to 0.58
    **/
   const int32_t exp_num_rr_inlier_dets = 2;
   host.speed = 0.0F;
   host.curvature_rear = 0.0F;
   object_track.movable_prob = 0.0F;
   object_track.reduced_id = 10U;
   object_track.vcs_position.x = 10.0F;
   object_track.vcs_position.y = 0.0F;

   object_track.filtered_hist_assoc_det_rr_err_mean = 0.48F;

   /** \action
    * Call function Select_Detection_Inliers
    **/
   Select_Detection_Inliers(host, calibs, valid_det_indices, n_valid_dets, abs_rdot_diffs, object_track, det_props);

   /** \result
    * Check that the correct detections are selected as inliers and that the correct detections are flagged as inliers.
    **/
   CHECK_EQUAL_TEXT(exp_num_rr_inlier_dets, object_track.num_rr_inlier_dets, "Incorrect number of inlier detections selected.")
   for (uint32_t i = 0U; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      if ((i == 0U) || (i == 20U))
      {
         CHECK_TRUE_TEXT(det_props[i].f_rr_inlier, "Detection was incorrectly not flagged as a range rate inlier")
      }
      else
      {
         CHECK_FALSE_TEXT(det_props[i].f_rr_inlier, "Detection was incorrectly flagged as a range rate inlier")
      }
   }
}

/** \purpose
 * Test that a 1 out of 4 detections of stationary object are correctly selected as inliers when the host is stationary
 * and object is outside the zone near the host
 * \req      NA
 */
TEST(f360_select_detection_inliers, Select_Detection_Inliers_Stationary_Host_Stationary_Object_Outside_Of_Zone)
{
   /** \precond
    * Object and detection properties have been set up in the test group.
    * Set expected number of inliers to 2.
    * Set host speed to 0 and curvature rear to 0
    * Set object track movable probability to 0
    * Set object track position x to 10 and y to 0
    * Set object track reduced id to 10
    * Set historic range rate error mean to 0.58
    **/
   const int32_t exp_num_rr_inlier_dets = 1;
   host.speed = 0.0F;
   host.curvature_rear = 0.0F;
   object_track.movable_prob = 0.0F;
   object_track.reduced_id = 10U;
   object_track.vcs_position.x = 10.0F;
   object_track.vcs_position.y = 100.0F;

   object_track.filtered_hist_assoc_det_rr_err_mean = 0.48F;

   /** \action
    * Call function Select_Detection_Inliers
    **/
   Select_Detection_Inliers(host, calibs, valid_det_indices, n_valid_dets, abs_rdot_diffs, object_track, det_props);

   /** \result
    * Check that the correct detections are selected as inliers and that the correct detections are flagged as inliers.
    **/
   CHECK_EQUAL_TEXT(exp_num_rr_inlier_dets, object_track.num_rr_inlier_dets, "Incorrect number of inlier detections selected.")
   for (uint32_t i = 0U; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      if ((i == 0U))
      {
         CHECK_TRUE_TEXT(det_props[i].f_rr_inlier, "Detection was incorrectly not flagged as a range rate inlier")
      }
      else
      {
         CHECK_FALSE_TEXT(det_props[i].f_rr_inlier, "Detection was incorrectly flagged as a range rate inlier")
      }
   }
}


/** @}*/

/** \defgroup  f360_find_detection_inliers_for_single_object
 *  @{
 */

/** \brief
 * Testing of the function that calls Find_Detection_Inliers_For_Single_Object for all active objects
 **/
TEST_GROUP(f360_find_detection_inliers)
{
   // Common variables used within all tests in group
   F360_Host_T host = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Object_Track_T object_track[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Calibrations_T calibrations = {};

   const uint32_t obj1_id = 10U;
   const uint32_t obj2_id = 42U;

   const uint32_t det_id_rr_do_match_obj1 = 11U;
   const uint32_t det_id_rr_dont_match_obj1 = 21U;
   
   const uint32_t det_id_do_match_obj2 = 12U;
   const uint32_t det_id_dont_match_obj2 = 22U;

   /** \setup
    * Setup tracker info to have 2 active objects
    * Setup 2 identical objects with different ids
    * Each object shall have:
    * - 1 detection with identical predicted and dealiased range rate
    * - 1 detection where predicted and dealiased range rate differs a lot
    * - object filter type to CTCA
    * - object speed to 10.0 m/s
    * - object curvature to 0
    * Setup corresponding detection props
    **/
   TEST_SETUP()
   {
      // Initialize calibrations
      Initialize_Tracker_Calibrations(calibrations);

      tracker_info.num_active_objs = 2;
      tracker_info.active_obj_ids[0] = obj1_id;
      tracker_info.active_obj_ids[1] = obj2_id;

      Clear_Detections_Props(det_props);

      // Object 1
      det_props[det_id_rr_do_match_obj1 - 1].range_rate_predicted = 10.0F;
      det_props[det_id_rr_dont_match_obj1 - 1].range_rate_dealiased = 15.0F;

      object_track[obj1_id - 1].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      object_track[obj1_id - 1].speed = 10.0F;
      object_track[obj1_id - 1].curvature = 0.0F;
      object_track[obj1_id - 1].ndets = 2;
      object_track[obj1_id - 1].detids[0] = det_id_rr_do_match_obj1;
      object_track[obj1_id - 1].detids[1] = det_id_rr_dont_match_obj1;

      // Object 2
      det_props[det_id_do_match_obj2 - 1].range_rate_predicted = 10.0F;
      det_props[det_id_dont_match_obj2 - 1].range_rate_dealiased = 15.0F;

      object_track[obj2_id - 1].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      object_track[obj2_id - 1].speed = 10.0F;
      object_track[obj2_id - 1].curvature = 0.0F;
      object_track[obj2_id - 1].ndets = 2;
      object_track[obj2_id - 1].detids[0] = det_id_do_match_obj2;
      object_track[obj2_id - 1].detids[1] = det_id_dont_match_obj2;
   }
};

/** @}*/


/** \defgroup  f360_find_detection_inliers_single_object
 *  @{
 */

/** \brief
 * Testing of Find_Detection_Inliers_For_Single_Object. The purpose is to test that associated detections
 * that match the object's properties well enough are selected as inliers. 
 **/
TEST_GROUP(f360_find_detection_inliers_single_object)
{
   // Common variables used within all tests
   F360_Host_T host = {};
   F360_Object_Track_T object_track = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = { };
   F360_Calibrations_T calibrations = {};
   uint32_t det_id_1 = 11U;
   uint32_t det_id_2 = 21U;

   /** \setup
    * Create two detections with different predicted and dealiased range rate on idx (det_id_1 - 1) and (det_id_2 - 1)
    * Set object ndets to 2
    * Set object detids to 11 and 21
    * Set predicted and dealiased range rates such that range rate errors are significantly different for the 2 detections
    * Set object's historic mean and variance to reflect that previous associated detections have had small range rates errors
    * - hisoric number of dets = 4
    * - historic mean = 0.1125
    * - historic var = 0.00396
    **/
   TEST_SETUP()
   {
      // Initialize calibrations
      Initialize_Tracker_Calibrations(calibrations);
      Clear_Detections_Props(det_props);

      det_props[det_id_1 - 1].range_rate_predicted = 3.0F;
      det_props[det_id_1 - 1].range_rate_dealiased = 2.0F;
      det_props[det_id_2 - 1].range_rate_predicted = 4.45F;
      det_props[det_id_2 - 1].range_rate_dealiased = 2.5F;
      object_track.ndets = 2;
      object_track.detids[0] = det_id_1;
      object_track.detids[1] = det_id_2;
      object_track.filtered_hist_assoc_n_dets = 4.0F;
      object_track.filtered_hist_assoc_det_rr_err_mean = 0.1125F;
      object_track.filtered_hist_assoc_det_rr_err_var = 0.00396F;
   }
};

/** \purpose
 * Check that one of the two associated detections is selected as an inlier and the other is rejected when there is
 * historic detection information reflecting lower range rate errors.
 * \req      NA
 */
TEST(f360_find_detection_inliers_single_object, Find_Detection_Inliers_One_Inlier_Detections_Selected)
{
   /** \precond
    * Object and detections have been set up in the test group
    * Set expected number of inlier detections to 1
    **/
   uint32_t expected_num_rr_inlier_dets = 1U;

   /** \action
    * Call function Find_Detection_Inliers_For_Single_Object
    **/
   Find_Detection_Inliers_For_Single_Object(host, calibrations, object_track, det_props);

   /** \result
    * Check that the number of inlier detections for object to be 1
    * Check that the inlier flag is correctly set for all detections.
    **/
   CHECK_EQUAL_TEXT(expected_num_rr_inlier_dets, object_track.num_rr_inlier_dets, "Incorrect number of detections was downselected for object");
   for (uint32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      if(i == (det_id_1 - 1))
      {
         CHECK_TRUE_TEXT(det_props[i].f_rr_inlier, "A detection was not marked as inlier when it should have been")
      }
      else
      {
         CHECK_FALSE_TEXT(det_props[i].f_rr_inlier, "A detection was wrongly marked as inlier")
      }
   }
}

/** \purpose
 * Check that both of the two associated detections are selected as inliers when there is little historic detection
 * information that doesn't indicate small historic range rate errors.
 * \req      NA
 */
TEST(f360_find_detection_inliers_single_object, Find_Detection_Inliers_Two_Inlier_Detections_Selected)
{
   /** \precond
    * Object and detections have been set up in the test group
    * Set historic detection information to
    * - filtered number of historic dets = 2
    * - historic mean and variance = 1.4 and 0.5 respectively
    * Set expected number of inlier detections to 2
    **/
   object_track.filtered_hist_assoc_n_dets = 2.0F;
   object_track.filtered_hist_assoc_det_rr_err_mean = 1.35F;
   object_track.filtered_hist_assoc_det_rr_err_var = 0.5F;

   uint32_t expected_num_rr_inlier_dets = 2U;

   /** \action
    * Call function Find_Detection_Inliers_For_Single_Object
    **/
   Find_Detection_Inliers_For_Single_Object(host, calibrations, object_track, det_props);

   /** \result
    * Check that the number of inlier detections for object to be 2
    * Check that the inlier flag is correctly set for all detections.
    **/
   CHECK_EQUAL_TEXT(expected_num_rr_inlier_dets, object_track.num_rr_inlier_dets, "Incorrect number of detections was downselected for object");
   for (uint32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      if((i == (det_id_1 - 1)) || (i == (det_id_2 - 1)))
      {
         CHECK_TRUE_TEXT(det_props[i].f_rr_inlier, "A detection was not marked as inlier when it should have been")
      }
      else
      {
         CHECK_FALSE_TEXT(det_props[i].f_rr_inlier, "A detection was wrongly marked as inlier")
      }
   }
}

/** \purpose
 * Check that both of the two associated detections are rejected when there is much historic detection
 * information that indicates smaller historic range rate errors.
 * \req      NA
 */
TEST(f360_find_detection_inliers_single_object, Find_Detection_Inliers_No_Inlier_Detections_Selected)
{
   /** \precond
    * Object and detections have been set up in the test group
    * Set historic detection information to
    * - filtered number of historic dets = 12
    * - historic mean and variance = 0.1 and 0.01 respectively
    * Set expected number of inlier detections to 2
    **/
   object_track.filtered_hist_assoc_n_dets = 12.0F;
   object_track.filtered_hist_assoc_det_rr_err_mean = 0.05F;
   object_track.filtered_hist_assoc_det_rr_err_var = 0.01F;

   uint32_t expected_num_rr_inlier_dets = 0U;

   /** \action
    * Call function Find_Detection_Inliers_For_Single_Object
    **/
   Find_Detection_Inliers_For_Single_Object(host, calibrations, object_track, det_props);

   /** \result
    * Check that the number of inlier detections for object to be 0
    * Check that the inlier flag is correctly set for all detections.
    **/
   CHECK_EQUAL_TEXT(expected_num_rr_inlier_dets, object_track.num_rr_inlier_dets, "Incorrect number of detections was downselected for object");
   for (uint32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      CHECK_FALSE_TEXT(det_props[i].f_rr_inlier, "A detection was wrongly marked as inlier")
   }
}

/** \purpose
 * Check that the filtered number of historic detections is updated even when there are no valid associated detections.
 * \req      NA
 */
TEST(f360_find_detection_inliers_single_object, Find_Detection_Inliers_Num_Hist_Dets_Updated_No_Valid_Dets)
{
   /** \precond
    * Object and detections have been set up in the test group
    * Set number of associated detections to 0.
    **/
   object_track.ndets = 0;
   const float32_t previous_filtered_hist_assoc_n_dets = object_track.filtered_hist_assoc_n_dets;

   /** \action
    * Call function Find_Detection_Inliers_For_Single_Object
    **/
   Find_Detection_Inliers_For_Single_Object(host, calibrations, object_track, det_props);

   /** \result
    * Check that filtered number of historic dets is not unchanged.
    **/
   CHECK_FALSE_TEXT(previous_filtered_hist_assoc_n_dets == object_track.filtered_hist_assoc_n_dets, "Number of filtered historic detections was incorrectly not changed.");
}

/** @}*/

/** \defgroup  f360_find_detection_inliers_find_valid_detections
 *  @{
 */

/** \brief
 * Testing of a function that finds detections that are valid candidates to be downselected to track
 **/
TEST_GROUP(f360_find_detection_inliers_find_valid_detections)
{
   // Common variables used within all tests
   F360_Object_Track_T object_track = { };
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = { };

   // Variables to be modified by function
   uint32_t valid_det_indices[MAX_DETS_IN_OBJ_TRK] = { };
   uint32_t n_valid_dets = 0U; 
   
   // Expected data
   uint32_t expected_valid_det_indices[MAX_DETS_IN_OBJ_TRK] = {};
   uint32_t expected_n_valid_dets;

   // Detection indices to use in tests
   uint32_t det_idx_1 = 10U;
   uint32_t det_idx_2 = 20U;
   
   TEST_SETUP()
   {
      Clear_Detections_Props(det_props);

      // Associate the two detections to track
      uint32_t det_id_1 = det_idx_1 + 1U;
      uint32_t det_id_2 = det_idx_2 + 1U;
      object_track.detids[0] = det_id_1;
      object_track.detids[1] = det_id_2;
      object_track.ndets = 2;

      object_track.vcs_position = Point(10.0F,10.0F);
      det_props[det_idx_1].vcs_position = Point(16.9F,10.0F);
      det_props[det_idx_2].vcs_position = Point(17.1F,10.0F);

      object_track.bbox.Set_Length(4.5F);
      object_track.bbox.Set_Width(2.0F);
      object_track.bbox.Set_Orientation(0.0F);
      object_track.reference_point = F360_REFERENCE_POINT_REAR;
      object_track.Update_Bbox_Center();
   }
};

/** \purpose
 * Test that associated detections that have no error flags set are considered valid detections
 * \req      NA
 */
TEST(f360_find_detection_inliers_find_valid_detections, Select_Detections_To_Mark_As_Inliers_Both_Detections_Valid)
{
   /** \precond
    * Two detections, with no error flags set, have been associated to track in test group.
    * Set expected number of valid detections to 2
    * Set expected valid detection indices to det_idx_1 and det_idx_2
    **/
   expected_n_valid_dets = 2U;
   expected_valid_det_indices[0] = det_idx_1;
   expected_valid_det_indices[1] = det_idx_2;

   /** \action
    * Call function
    **/
   Find_Valid_Detections(object_track, det_props, valid_det_indices, n_valid_dets);

   /** \result
    * Expect number of valid detection to be 2
    * Expect detection indices to match the indicies of the two detections in test group
    **/
   
   CHECK_EQUAL_TEXT(expected_n_valid_dets, n_valid_dets, "Incorrect number of detections were considered to be valid");
   for (uint32_t i = 0U; i < expected_n_valid_dets; i++)
   {
      CHECK_EQUAL_TEXT(expected_valid_det_indices[i], valid_det_indices[i], "Index on returned valid detection list does not match expected data");
   }
}

/** \purpose
 * Test that an associated detection with close target flag set is not considered to be a valid detection
 * \req      NA
 */
TEST(f360_find_detection_inliers_find_valid_detections, Select_Detections_To_Mark_As_Inliers_Close_Target_Detection)
{
   /** \precond
    * Two detections, with no error flags set, have been associated to track in test group.
    * Set close target flag for detection det_idx_1 to true
    * Set expected number of valid detections to 1
    * Set expected valid detection indices to det_idx_2
    **/
   det_props[det_idx_1].f_close_target = true;
   expected_n_valid_dets = 1U;
   expected_valid_det_indices[0] = det_idx_2;

   /** \action
    * Call function
    **/
   Find_Valid_Detections(object_track, det_props, valid_det_indices, n_valid_dets);

   /** \result
    * Expect number of valid detection to be 1
    * Expect detection index to match the index of the second detection in test group
    **/

   CHECK_EQUAL_TEXT(expected_n_valid_dets, n_valid_dets, "Incorrect number of detections were considered to be valid");
   for (uint32_t i = 0U; i < expected_n_valid_dets; i++)
   {
      CHECK_EQUAL_TEXT(expected_valid_det_indices[i], valid_det_indices[i], "Index on returned valid detection list does not match expected data");
   }
}

/** \purpose
 * Test that an associated detection with azimuth rdot outlier flag set is not considered to be a valid detection
 * \req      NA
 */
TEST(f360_find_detection_inliers_find_valid_detections, Select_Detections_To_Mark_As_Inliers_Azimuth_Rdot_Outlier_Detection)
{
   /** \precond
    * Two detections, with no error flags set, have been associated to track in test group.
    * Set azimuth rdot outlier flag for detection det_idx_1 to true
    * Set expected number of valid detections to 1
    * Set expected valid detection indices to det_idx_2
    **/
   det_props[det_idx_1].f_azimuth_rdot_outlier = true;
   expected_n_valid_dets = 1U;
   expected_valid_det_indices[0] = det_idx_2;

   /** \action
    * Call function
    **/
   Find_Valid_Detections(object_track, det_props, valid_det_indices, n_valid_dets);

   /** \result
    * Expect number of valid detection to be 1
    * Expect detection index to match the index of the second detection in test group
    **/

   CHECK_EQUAL_TEXT(expected_n_valid_dets, n_valid_dets, "Incorrect number of detections were considered to be valid");
   for (uint32_t i = 0U; i < expected_n_valid_dets; i++)
   {
      CHECK_EQUAL_TEXT(expected_valid_det_indices[i], valid_det_indices[i], "Index on returned valid detection list does not match expected data");
   }
}

/** \purpose
* Test that an associated detection with object wheel spin flag set is not considered to be a valid detection
* \req      NA
*/
TEST(f360_find_detection_inliers_find_valid_detections, Select_Detections_To_Mark_As_Inliers_Object_Wheel_Spin_Detection)
{
   /** \precond
   * Two detections, with no error flags set, have been associated to track in test group.
   * Set object wheel spin flag for detection det_idx_2 to true
   * Set expected number of valid detections to 1
   * Set expected valid detection indices to det_idx_1
   **/
   det_props[det_idx_2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   expected_n_valid_dets = 1U;
   expected_valid_det_indices[0] = det_idx_1;

   /** \action
   * Call function
   **/
   Find_Valid_Detections(object_track, det_props, valid_det_indices, n_valid_dets);

   /** \result
   * Expect number of valid detection to be 1
   * Expect detection index to match the index of the first detection in test group
   **/

   CHECK_EQUAL_TEXT(expected_n_valid_dets, n_valid_dets, "Incorrect number of detections were considered to be valid");
   CHECK_EQUAL_TEXT(expected_valid_det_indices[0], valid_det_indices[0], "Index on returned valid detection list does not match expected data");
}

/** \purpose
* Test that an associated detection with pair wheel spin flag set is not considered to be a valid detection
* \req      NA
*/
TEST(f360_find_detection_inliers_find_valid_detections, Select_Detections_To_Mark_As_Inliers_Pair_Wheel_Spin_Detection)
{
   /** \precond
   * Two detections, with no error flags set, have been associated to track in test group.
   * Set pair wheel spin flag for detection det_idx_2 to true
   * Set expected number of valid detections to 1
   * Set expected valid detection indices to det_idx_1
   **/
   det_props[det_idx_2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS;
   expected_n_valid_dets = 1U;
   expected_valid_det_indices[0] = det_idx_1;

   /** \action
   * Call function
   **/
   Find_Valid_Detections(object_track, det_props, valid_det_indices, n_valid_dets);

   /** \result
   * Expect number of valid detection to be 1
   * Expect detection index to match the index of the first detection in test group
   **/

   CHECK_EQUAL_TEXT(expected_n_valid_dets, n_valid_dets, "Incorrect number of detections were considered to be valid");
   CHECK_EQUAL_TEXT(expected_valid_det_indices[0], valid_det_indices[0], "Index on returned valid detection list does not match expected data");
}

/** \purpose
* Test that an associated detection with nearby wheel spin flag set is not considered to be a valid detection
* \req      NA
*/
TEST(f360_find_detection_inliers_find_valid_detections, Select_Detections_To_Mark_As_Inliers_Nearby_Wheel_Spin_Detection)
{
   /** \precond
   * Two detections, with no error flags set, have been associated to track in test group.
   * Set nearby wheel spin flag for detection det_idx_2 to true
   * Set expected number of valid detections to 1
   * Set expected valid detection indices to det_idx_1
   **/
   det_props[det_idx_2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_NEARBY;
   expected_n_valid_dets = 1U;
   expected_valid_det_indices[0] = det_idx_1;

   /** \action
   * Call function
   **/
   Find_Valid_Detections(object_track, det_props, valid_det_indices, n_valid_dets);

   /** \result
   * Expect number of valid detection to be 1
   * Expect detection index to match the index of the first detection in test group
   **/

   CHECK_EQUAL_TEXT(expected_n_valid_dets, n_valid_dets, "Incorrect number of detections were considered to be valid");
   CHECK_EQUAL_TEXT(expected_valid_det_indices[0], valid_det_indices[0], "Index on returned valid detection list does not match expected data");
}

/** \purpose
* Test that associated detections to a long object with reference point center are valid detections regardless of how far away they are
* \req      NA
*/
TEST(f360_find_detection_inliers_find_valid_detections, Long_Object_Ref_Pnt_Center_Select_Detections_To_Mark_As_Inliers_Far_Away_Dets_OK)
{
   /** \precond
   * Two detections, with no error flags set, have been associated to track in test group.
   * Set expected number of valid detections to 2
   * Set expected valid detection indices to det_idx_1 & det_idx_2
   * Set object length to 7.5m, reference point to center & update bbox center
   **/
   expected_n_valid_dets = 2U;
   expected_valid_det_indices[0] = det_idx_1;
   expected_valid_det_indices[1] = det_idx_2;

   object_track.bbox.Set_Length(7.5F);
   object_track.reference_point = F360_REFERENCE_POINT_CENTER;
   object_track.Update_Bbox_Center();

   /** \action
   * Call function
   **/
   Find_Valid_Detections(object_track, det_props, valid_det_indices, n_valid_dets);

   /** \result
   * Expect number of valid detection to be 2
   * Expect detection index to match the index of the both detection in test group
   **/
   CHECK_EQUAL_TEXT(expected_n_valid_dets, n_valid_dets, "Incorrect number of detections were considered to be valid");
   for (uint32_t i = 0U; i < expected_n_valid_dets; i++)
   {
      CHECK_EQUAL_TEXT(expected_valid_det_indices[i], valid_det_indices[i], "Index on returned valid detection list does not match expected data");
   }
}

/** \purpose
* Test that associated detections to a long object with reference point front are valid detections regardless of how far away they are
* \req      NA
*/
TEST(f360_find_detection_inliers_find_valid_detections, Long_Object_Ref_Pnt_Front_Select_Detections_To_Mark_As_Inliers_Far_Away_Dets_OK)
{
   /** \precond
   * Two detections, with no error flags set, have been associated to track in test group.
   * Set expected number of valid detections to 2
   * Set expected valid detection indices to det_idx_1 & det_idx_2
   * Set object length to 7.5m, reference point to front & update bbox center
   **/
   expected_n_valid_dets = 2U;
   expected_valid_det_indices[0] = det_idx_1;
   expected_valid_det_indices[1] = det_idx_2;

   object_track.bbox.Set_Length(7.5F);
   object_track.reference_point = F360_REFERENCE_POINT_FRONT;
   object_track.Update_Bbox_Center();

   /** \action
   * Call function
   **/
   Find_Valid_Detections(object_track, det_props, valid_det_indices, n_valid_dets);

   /** \result
   * Expect number of valid detection to be 2
   * Expect detection index to match the index of the both detection in test group
   **/
   CHECK_EQUAL_TEXT(expected_n_valid_dets, n_valid_dets, "Incorrect number of detections were considered to be valid");
   for (uint32_t i = 0U; i < expected_n_valid_dets; i++)
   {
      CHECK_EQUAL_TEXT(expected_valid_det_indices[i], valid_det_indices[i], "Index on returned valid detection list does not match expected data");
   }
}

/** \purpose
* Test that associated detections to a long object with reference point front left are valid detections regardless of how far away they are
* \req      NA
*/
TEST(f360_find_detection_inliers_find_valid_detections, Long_Object_Ref_Pnt_Front_Left_Select_Detections_To_Mark_As_Inliers_Far_Away_Dets_OK)
{
   /** \precond
   * Two detections, with no error flags set, have been associated to track in test group.
   * Set expected number of valid detections to 2
   * Set expected valid detection indices to det_idx_1 & det_idx_2
   * Set object length to 7.5m, reference point to front left & update bbox center
   **/
   expected_n_valid_dets = 2U;
   expected_valid_det_indices[0] = det_idx_1;
   expected_valid_det_indices[1] = det_idx_2;

   object_track.bbox.Set_Length(7.5F);
   object_track.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
   object_track.Update_Bbox_Center();

   /** \action
   * Call function
   **/
   Find_Valid_Detections(object_track, det_props, valid_det_indices, n_valid_dets);

   /** \result
   * Expect number of valid detection to be 2
   * Expect detection index to match the index of the both detection in test group
   **/
   CHECK_EQUAL_TEXT(expected_n_valid_dets, n_valid_dets, "Incorrect number of detections were considered to be valid");
   for (uint32_t i = 0U; i < expected_n_valid_dets; i++)
   {
      CHECK_EQUAL_TEXT(expected_valid_det_indices[i], valid_det_indices[i], "Index on returned valid detection list does not match expected data");
   }
}

/** \purpose
* Test that associated detections to a long object with reference point front right are valid detections regardless of how far away they are
* \req      NA
*/
TEST(f360_find_detection_inliers_find_valid_detections, Long_Object_Ref_Pnt_Front_Right_Select_Detections_To_Mark_As_Inliers_Far_Away_Dets_OK)
{
   /** \precond
   * Two detections, with no error flags set, have been associated to track in test group.
   * Set expected number of valid detections to 2
   * Set expected valid detection indices to det_idx_1 & det_idx_2
   * Set object length to 7.5m, reference point to front right & update bbox center
   **/
   expected_n_valid_dets = 2U;
   expected_valid_det_indices[0] = det_idx_1;
   expected_valid_det_indices[1] = det_idx_2;

   object_track.bbox.Set_Length(7.5F);
   object_track.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;
   object_track.Update_Bbox_Center();

   /** \action
   * Call function
   **/
   Find_Valid_Detections(object_track, det_props, valid_det_indices, n_valid_dets);

   /** \result
   * Expect number of valid detection to be 2
   * Expect detection index to match the index of the both detection in test group
   **/
   CHECK_EQUAL_TEXT(expected_n_valid_dets, n_valid_dets, "Incorrect number of detections were considered to be valid");
   for (uint32_t i = 0U; i < expected_n_valid_dets; i++)
   {
      CHECK_EQUAL_TEXT(expected_valid_det_indices[i], valid_det_indices[i], "Index on returned valid detection list does not match expected data");
   }
}

/** \purpose
* Test that associated detections to a long object with reference point left are valid detections regardless of how far away they are
* \req      NA
*/
TEST(f360_find_detection_inliers_find_valid_detections, Long_Object_Ref_Pnt_Left_Select_Detections_To_Mark_As_Inliers_Far_Away_Dets_OK)
{
   /** \precond
   * Two detections, with no error flags set, have been associated to track in test group.
   * Set expected number of valid detections to 2
   * Set expected valid detection indices to det_idx_1 & det_idx_2
   * Set object length to 7.5m, reference point to left & update bbox center
   **/
   expected_n_valid_dets = 2U;
   expected_valid_det_indices[0] = det_idx_1;
   expected_valid_det_indices[1] = det_idx_2;

   object_track.bbox.Set_Length(7.5F);
   object_track.reference_point = F360_REFERENCE_POINT_LEFT;
   object_track.Update_Bbox_Center();

   /** \action
   * Call function
   **/
   Find_Valid_Detections(object_track, det_props, valid_det_indices, n_valid_dets);

   /** \result
   * Expect number of valid detection to be 2
   * Expect detection index to match the index of the both detection in test group
   **/
   CHECK_EQUAL_TEXT(expected_n_valid_dets, n_valid_dets, "Incorrect number of detections were considered to be valid");
   for (uint32_t i = 0U; i < expected_n_valid_dets; i++)
   {
      CHECK_EQUAL_TEXT(expected_valid_det_indices[i], valid_det_indices[i], "Index on returned valid detection list does not match expected data");
   }
}

/** \purpose
* Test that associated detections to a long object with reference point right are valid detections regardless of how far away they are
* \req      NA
*/
TEST(f360_find_detection_inliers_find_valid_detections, Long_Object_Ref_Pnt_Right_Select_Detections_To_Mark_As_Inliers_Far_Away_Dets_OK)
{
   /** \precond
   * Two detections, with no error flags set, have been associated to track in test group.
   * Set expected number of valid detections to 2
   * Set expected valid detection indices to det_idx_1 & det_idx_2
   * Set object length to 7.5m, reference point to front right & update bbox center
   **/
   expected_n_valid_dets = 2U;
   expected_valid_det_indices[0] = det_idx_1;
   expected_valid_det_indices[1] = det_idx_2;

   object_track.bbox.Set_Length(7.5F);
   object_track.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;
   object_track.Update_Bbox_Center();

   /** \action
   * Call function
   **/
   Find_Valid_Detections(object_track, det_props, valid_det_indices, n_valid_dets);

   /** \result
   * Expect number of valid detection to be 2
   * Expect detection index to match the index of the both detection in test group
   **/
   CHECK_EQUAL_TEXT(expected_n_valid_dets, n_valid_dets, "Incorrect number of detections were considered to be valid");
   for (uint32_t i = 0U; i < expected_n_valid_dets; i++)
   {
      CHECK_EQUAL_TEXT(expected_valid_det_indices[i], valid_det_indices[i], "Index on returned valid detection list does not match expected data");
   }
}

/** \purpose
* Test that associated detections to a long object with reference point rear need to be close to the reference point to be valid detections
* \req      NA
*/
TEST(f360_find_detection_inliers_find_valid_detections, Long_Object_Ref_Pnt_Rear_Select_Detections_To_Mark_As_Inliers_Far_Away_Dets_Not_OK)
{
    /** \precond
    * Two detections, with no error flags set, have been associated to track in test group.
    * Set expected number of valid detections to 1
    * Set expected valid detection indices to det_idx_1
    * Set object length to 7.5m, reference point to rear & update bbox center
    * Set the position of the second detections to 17.0 + 0.35 * 0.5
    **/
    expected_n_valid_dets = 1U;
    expected_valid_det_indices[0] = det_idx_1;

    object_track.bbox.Set_Length(7.5F);
    object_track.reference_point = F360_REFERENCE_POINT_REAR;
    object_track.Update_Bbox_Center();

    det_props[det_idx_2].vcs_position = Point(17.0F + 0.35F * 0.5F, 10.0F);

    /** \action
    * Call function
    **/
    Find_Valid_Detections(object_track, det_props, valid_det_indices, n_valid_dets);

    /** \result
    * Expect number of valid detection to be 1
    * Expect detection index to match the index of the both detection in test group
    **/
    CHECK_EQUAL_TEXT(expected_n_valid_dets, n_valid_dets, "Incorrect number of detections were considered to be valid");
    for (uint32_t i = 0U; i < expected_n_valid_dets; i++)
    {
        CHECK_EQUAL_TEXT(expected_valid_det_indices[i], valid_det_indices[i], "Index on returned valid detection list does not match expected data");
    }
}

/** \purpose
* Test that associated detections to a long object with reference point rear right need to be close to the reference point to be valid detections
* \req      NA
*/
TEST(f360_find_detection_inliers_find_valid_detections, Long_Object_Ref_Pnt_Rear_Right_Select_Detections_To_Mark_As_Inliers_Far_Away_Dets_Not_OK)
{
    /** \precond
    * Two detections, with no error flags set, have been associated to track in test group.
    * Set expected number of valid detections to 1
    * Set expected valid detection indices to det_idx_1
    * Set object length to 7.5m, reference point to rear right & update bbox center
    * Set the position of the second detections to 17.0 + 0.35 * 0.5
    **/
    expected_n_valid_dets = 1U;
    expected_valid_det_indices[0] = det_idx_1;

    object_track.bbox.Set_Length(7.5F);
    object_track.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
    object_track.Update_Bbox_Center();

    det_props[det_idx_2].vcs_position = Point(17.0F + 0.35F * 0.5F, 10.0F);

    /** \action
    * Call function
    **/
    Find_Valid_Detections(object_track, det_props, valid_det_indices, n_valid_dets);

    /** \result
    * Expect number of valid detection to be 1
    * Expect detection index to match the index of the both detection in test group
    **/
    CHECK_EQUAL_TEXT(expected_n_valid_dets, n_valid_dets, "Incorrect number of detections were considered to be valid");
    for (uint32_t i = 0U; i < expected_n_valid_dets; i++)
    {
        CHECK_EQUAL_TEXT(expected_valid_det_indices[i], valid_det_indices[i], "Index on returned valid detection list does not match expected data");
    }
}

/** \purpose
* Test that associated detections to a long object with reference point rear left need to be close to the reference point to be valid detections
* \req      NA
*/
TEST(f360_find_detection_inliers_find_valid_detections, Long_Object_Ref_Pnt_Rear_Left_Select_Detections_To_Mark_As_Inliers_Far_Away_Dets_Not_OK)
{
    /** \precond
    * Two detections, with no error flags set, have been associated to track in test group.
    * Set expected number of valid detections to 2
    * Set expected valid detection indices to det_idx_1
    * Set object length to 7.5m, reference point to rear left & update bbox center
    * Set the position of the second detections to 17.0 + 0.35 * 0.5
    **/
    expected_n_valid_dets = 1U;
    expected_valid_det_indices[0] = det_idx_1;

    object_track.bbox.Set_Length(7.5F);
    object_track.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
    object_track.Update_Bbox_Center();

    det_props[det_idx_2].vcs_position = Point(17.0F + 0.35F * 0.5F, 10.0F);


    /** \action
    * Call function
    **/
    Find_Valid_Detections(object_track, det_props, valid_det_indices, n_valid_dets);

    /** \result
    * Expect number of valid detection to be 1
    * Expect detection index to match the index of the both detection in test group
    **/
    CHECK_EQUAL_TEXT(expected_n_valid_dets, n_valid_dets, "Incorrect number of detections were considered to be valid");
    for (uint32_t i = 0U; i < expected_n_valid_dets; i++)
    {
        CHECK_EQUAL_TEXT(expected_valid_det_indices[i], valid_det_indices[i], "Index on returned valid detection list does not match expected data");
    }
}

/** \purpose
* Test that associated detections to a long object, longer than 7 meters, need to be within
* 7.0 + (length_of_object - 7.0) * 0.3 if
*/
TEST(f360_find_detection_inliers_find_valid_detections, Long_Object_Mark_Dets_OK_And_Not_OK_With_Increase)
{
    /** \precond
    * Two detections, with no error flags set, have been associated to track in test group.
    * Set expected number of valid detections to 2
    * Set expected valid detection indices to det_idx_1 and det_idx_2
    * Set object length to 7.5m, reference point to rear left & update bbox center
    * Set the position of the second detection to 0.25 * (object_track.bbox.Get_Length() - 7.0F)
    * Set the position of the third detection to 0.25 * (object_track.bbox.Get_Length() - 7.0F)
    **/
    expected_n_valid_dets = 2U;
    expected_valid_det_indices[0] = det_idx_1;
    expected_valid_det_indices[1] = det_idx_2;


    object_track.bbox.Set_Length(7.5F);
    object_track.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
    object_track.Update_Bbox_Center();

    uint32_t det_idx_3 = 30U;

    // Associate the two detections to track
    uint32_t det_id_3 = det_idx_3 + 1U;
    object_track.detids[2] = det_id_3;
    object_track.ndets = 3;

    object_track.vcs_position = Point(10.0F, 10.0F);
    det_props[det_idx_1].vcs_position = Point(16.9F, 10.0F);
    det_props[det_idx_2].vcs_position = Point(0.25 * (object_track.bbox.Get_Length() - 7.0F) + object_track.vcs_position.x + 7, 10.0F);
    det_props[det_idx_3].vcs_position = Point(0.35 * (object_track.bbox.Get_Length() - 7.0F) + object_track.vcs_position.x + 7, 10.0F);


    /** \action
    * Call function
    **/
    Find_Valid_Detections(object_track, det_props, valid_det_indices, n_valid_dets);

    /** \result
    * Expect number of valid detection to be 2
    * Expect detection index to match the index of the both detection in test group
    **/
    CHECK_EQUAL_TEXT(expected_n_valid_dets, n_valid_dets, "Incorrect number of detections were considered to be valid");
    for (uint32_t i = 0U; i < expected_n_valid_dets; i++)
    {
        CHECK_EQUAL_TEXT(expected_valid_det_indices[i], valid_det_indices[i], "Index on returned valid detection list does not match expected data");
    }
}

/** \purpose
* Test that associated detections to a long object, longer than 7 meters, need to be within
* 7 meters since less than 50% of the associated detections are in the top half of the object
*/
TEST(f360_find_detection_inliers_find_valid_detections, Long_Object_Mark_Dets_OK_And_Not_OK_Without_Increase)
{
    /** \precond
    * Two detections, with no error flags set, have been associated to track in test group.
    * Set expected number of valid detections to 2
    * Set expected valid detection indices to det_idx_1 and det_idx_2
    * Set object length to 7.5m, reference point to rear left & update bbox center
    * Set the position of the second detection to 13 meters
    * Set the position of the third detection to 0.25 * (object_track.bbox.Get_Length() - 7.0F)
    **/
    expected_n_valid_dets = 2U;
    expected_valid_det_indices[0] = det_idx_1;
    expected_valid_det_indices[1] = det_idx_2;

    object_track.bbox.Set_Length(7.5F);
    object_track.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
    object_track.Update_Bbox_Center();

    uint32_t det_idx_3 = 30U;

    // Associate the two detections to track
    uint32_t det_id_3 = det_idx_3 + 1U;
    object_track.detids[2] = det_id_3;
    object_track.ndets = 3;

    object_track.vcs_position = Point(10.0F, 10.0F);
    det_props[det_idx_1].vcs_position = Point(11.0F, 10.0F);
    det_props[det_idx_2].vcs_position = Point(13.0F, 10.0F);
    det_props[det_idx_3].vcs_position = Point(0.35 * (object_track.bbox.Get_Length() - 7.0F) + object_track.vcs_position.x + 7, 10.0F);

    /** \action
    * Call function
    **/
    Find_Valid_Detections(object_track, det_props, valid_det_indices, n_valid_dets);
    /** \result
    * Expect number of valid detection to be 2
    * Expect detection index to match the index of the both detection in test group
    **/
    CHECK_EQUAL_TEXT(expected_n_valid_dets, n_valid_dets, "Incorrect number of detections were considered to be valid");
    for (uint32_t i = 0U; i < expected_n_valid_dets; i++)
    {
        CHECK_EQUAL_TEXT(expected_valid_det_indices[i], valid_det_indices[i], "Index on returned valid detection list does not match expected data");
    }
}

/** @}*/

/** \defgroup  f360_find_detection_inliers_calculate_range_rate_diffs
 *  @{
 */

/** \brief
 * Testing of a function that calculates difference between predicted and dealiased range rate
 **/
TEST_GROUP(f360_find_detection_inliers_calculate_range_rate_diffs)
{
   // Common variables used within all tests
   float32_t TEST_PASS_TH = 1e-6F; 

   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   uint32_t valid_det_indices[MAX_DETS_IN_OBJ_TRK] = {};
   uint32_t n_valid_dets; 

   // Variable to be modified by function
   float32_t abs_rdot_diffs[MAX_DETS_IN_OBJ_TRK] = {};

   // Expected data
   float32_t expected_abs_rdot_diffs[MAX_DETS_IN_OBJ_TRK] = {};

   // Detection indices to use in tests
   uint32_t det_idx_1 = 10U;
   uint32_t det_idx_2 = 20U;

   TEST_SETUP()
   {
      // Set dealiased and predicted range rate to identical values for the two detections
      det_props[det_idx_1].range_rate_dealiased = 10.0F;
      det_props[det_idx_1].range_rate_predicted = 10.0F;

      det_props[det_idx_2].range_rate_dealiased = -15.0F;
      det_props[det_idx_2].range_rate_predicted = -15.0F;

      // Set detections to valid detections
      valid_det_indices[0] = det_idx_1;
      valid_det_indices[1] = det_idx_2;
      n_valid_dets = 2U;
   }
};

/** \purpose
 * Test that function to calculate difference between predicted and dealiased range rate returns correct values
 * for detections with identical range rates
 * \req      NA
 */
TEST(f360_find_detection_inliers_calculate_range_rate_diffs, Calculate_Range_Rate_Diffs_Identical_Range_Rates)
{
   /** \precond
    * Two detections with identical predicted and dealiased range rate have been set up in test group.
    * The number of valid detections have been set to 2 in test group.
    * The array with valid detection indices have been filled with indices of detections in test group.
    * Set expected rdot diffs to 0 for both detections
    **/
   expected_abs_rdot_diffs[0] = 0.0F;
   expected_abs_rdot_diffs[1] = 0.0F;

   /** \action
    * Call function
    **/
   Calculate_Abs_Range_Rate_Diffs(det_props, valid_det_indices, n_valid_dets, abs_rdot_diffs);

   /** \result
    * Expect difference between predicted and dealiased range rate to be 0
    **/
   for (uint32_t i = 0U; i < n_valid_dets; i++)
   {
      DOUBLES_EQUAL_TEXT(expected_abs_rdot_diffs[i], abs_rdot_diffs[i], TEST_PASS_TH, "The calculated difference between predicted and dealiased range rate is wrong.");
   }
}

/** \purpose
 * Test that function to calculate difference between predicted and dealiased range rate returns correct values
 * when predicted range rate is larger than dealiased range rate
 * \req      NA
 */
TEST(f360_find_detection_inliers_calculate_range_rate_diffs, Calculate_Range_Rate_Diffs_Predicted_Range_Rate_Larger)
{
   /** \precond
    * Two detections with identical predicted and dealiased range rate have been set up in test group.
    * The number of valid detections have been set to 2 in test group.
    * The array with valid detection indices have been filled with indices of detections in test group.
    * Add 0.3F and 5.8F to the predicted range rate of the two detections respectively
    * Set expected rdot diffs to 0.3 and 5.8 for the two detections
    **/
   det_props[det_idx_1].range_rate_predicted += 0.3F;
   det_props[det_idx_2].range_rate_predicted += 5.8F;
   expected_abs_rdot_diffs[0] = 0.3F;
   expected_abs_rdot_diffs[1] = 5.8F;

   /** \action
    * Call function
    **/
   Calculate_Abs_Range_Rate_Diffs(det_props, valid_det_indices, n_valid_dets, abs_rdot_diffs);

   /** \result
    * Expect difference between predicted and dealiased range rate to be 0.3 and 5.8 for the two detections
    **/
   for (uint32_t i = 0U; i < n_valid_dets; i++)
   {
      DOUBLES_EQUAL_TEXT(expected_abs_rdot_diffs[i], abs_rdot_diffs[i], TEST_PASS_TH, "The calculated difference between predicted and dealiased range rate is wrong.");
   }
}

/** \purpose
 * Test that function to calculate difference between predicted and dealiased range rate returns correct values
 * when dealiased range rate is larger than predicted range rate
 * \req      NA
 */
TEST(f360_find_detection_inliers_calculate_range_rate_diffs, Calculate_Range_Rate_Diffs_Dealiased_Range_Rate_Larger)
{
   /** \precond
    * Two detections with identical predicted and dealiased range rate have been set up in test group.
    * The number of valid detections have been set to 2 in test group.
    * The array with valid detection indices have been filled with indices of detections in test group.
    * Add 1.6F and 10.2F to the dealiased range rate of the two detections respectively
    * Set expected rdot diffs to 1.6F and 10.2F for the two detections
    **/
   det_props[det_idx_1].range_rate_dealiased += 1.6F;
   det_props[det_idx_2].range_rate_dealiased += 10.2F;
   expected_abs_rdot_diffs[0] = 1.6F;
   expected_abs_rdot_diffs[1] = 10.2F;

   /** \action
    * Call function
    **/
   Calculate_Abs_Range_Rate_Diffs(det_props, valid_det_indices, n_valid_dets, abs_rdot_diffs);

   /** \result
    * Expect difference between predicted and dealiased range rate to be 1.6 and 10.2 for the two detections
    **/
   for (uint32_t i = 0U; i < n_valid_dets; i++)
   {
      DOUBLES_EQUAL_TEXT(expected_abs_rdot_diffs[i], abs_rdot_diffs[i], TEST_PASS_TH, "The calculated difference between predicted and dealiased range rate is wrong.");
   }
}
/** @}*/

/** \brief
* Testing a function that clears the status of downselected detections both in the object and in detection properties
**/
TEST_GROUP(f360_find_detection_inliers_clear_find_detection_inliers)
{
   F360_Object_Track_T object_track = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};

   static const uint32_t NUMBER_OF_ASSOCIATED_DETECTIONS = 3U;
   uint32_t det_ids[NUMBER_OF_ASSOCIATED_DETECTIONS] = { 11U, 21U, 32U };

   TEST_SETUP()
   {
      object_track.ndets = 3;
      object_track.num_rr_inlier_dets = 3;
      for (uint32_t i = 0U; i < NUMBER_OF_ASSOCIATED_DETECTIONS; i++)
      {
         det_props[det_ids[i] - 1].f_rr_inlier = true;
         object_track.detids[i] = det_ids[i];
      }
   }
};


/** \purpose
* Testing a function that clears the status of downselected detections both in the object and in detection properties
* when all detections associated to the object are downselected
*\req      NA
*/
TEST(f360_find_detection_inliers_clear_find_detection_inliers, Clear_Detection_Inliers_To_Object_When_All_Detections_Flagged_As_Selected_By_Tracker)
{
   /** \precond
   * The number of valid detections associated to the object have been set to 3 in test group.
   * An object with number of reduced detections set to 3 has been created in test group
   * Flags f_rr_inlier has been set to true for 3 associated detections
   * Set expected number of reducted detections to 0
   **/
   uint32_t expected_n_reduced_dets = 0U;

   /** \action
   * Call function
   **/
   Clear_Detection_Inliers_To_Object(object_track, det_props);

   /** \result
   * Expect number of reduced detections for object track to be 0
   * Expect none of the detections to be flagged as selected by track.
   **/
   bool f_any_det_selected_by_trk = false;
   for (uint32_t i = 0; i < NUMBER_OF_ASSOCIATED_DETECTIONS; i++)
   {
      if (true == det_props[i].f_rr_inlier)
      {
         f_any_det_selected_by_trk = true;
      }
   }
   CHECK_EQUAL_TEXT(expected_n_reduced_dets, object_track.num_rr_inlier_dets, "The number of reduced detections for object track does not match the expected number.");
   CHECK_FALSE_TEXT(f_any_det_selected_by_trk, "Some detections were wrongly marked as selected by track");
}

/** \purpose
* Testing a function that clears the status of downselected detections both in the object and in detection properties
* when only part of detections associated to the object is downselected
*\req      NA
*/
TEST(f360_find_detection_inliers_clear_find_detection_inliers, Clear_Detection_Inliers_To_Object_When_Some_Of_Detections_Flagged_As_Selected_By_Tracker)
{
   /** \precond
   * The number of valid detections associated to the object have been set to 3 in test group.
   * Setting number of reduced detections associated to the object to 2
   * Flags f_rr_inlier has been set to true for 3 associated detections in test group, reseting the flag for one detection
   * Set expected number of reducted detections to 0
   **/
   object_track.num_rr_inlier_dets = 2U;
   det_props[det_ids[1] - 1].f_rr_inlier = false;
   uint32_t expected_n_reduced_dets = 0U;

   /** \action
   * Call function
   **/
   Clear_Detection_Inliers_To_Object(object_track, det_props);

   /** \result
   * Expect number of reduced detections for object track to be 0
   * Expect none of the detections to be flagged as selected by track.
   **/
   bool f_any_det_selected_by_trk = false;
   for (uint32_t i = 0; i < NUMBER_OF_ASSOCIATED_DETECTIONS; i++)
   {
      if (true == det_props[i].f_rr_inlier)
      {
         f_any_det_selected_by_trk = true;
      }
   }
   CHECK_EQUAL_TEXT(expected_n_reduced_dets, object_track.num_rr_inlier_dets, "The number of reduced detections for object track does not match the expected number.");
   CHECK_FALSE_TEXT(f_any_det_selected_by_trk, "Some detections were wrongly marked as selected by track");
}

/** @}*/
