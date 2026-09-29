/** \file
* This file contains unit tests for testing requirements connected to f360_find_detection_inliers.cpp file
*/

#include "f360_find_detection_inliers.h"
#include <CppUTest/TestHarness.h>

#include "f360_clear_detections_props.h"
#include "f360_detection_wheelspin_type.h"

using namespace f360_variant_A;

/** \defgroup  f360_find_detection_inliers_single_object
 *  @{
 */

/** \brief
 * Testing of a function that downselects associated detections that
 * have a predicted range rate similar to compensated range rate.
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
   uint32_t det_id_3 = 31U;
   uint32_t det_id_4 = 41U;

   /** \setup
    * Create 4 detections with identical predicted and dealiased range rate
    * Set object filter type to CTCA
    * Set object speed to 10.0 m/s
    * Set object curvature to 0
    * Set object ndets to 4
    * Set object detids to 11, 21, 31, 41
    **/
   TEST_SETUP()
   {
      // Initialize calibrations
      Initialize_Tracker_Calibrations(calibrations);
      Clear_Detections_Props(det_props);

      det_props[det_id_1 - 1].range_rate_predicted = 10.0F;
      det_props[det_id_1 - 1].range_rate_dealiased = 10.0F;
      
      det_props[det_id_2 - 1].range_rate_predicted = 10.0F;
      det_props[det_id_2 - 1].range_rate_dealiased = 10.0F;
      
      det_props[det_id_3 - 1].range_rate_predicted = 10.0F;
      det_props[det_id_3 - 1].range_rate_dealiased = 10.0F;
      
      det_props[det_id_4 - 1].range_rate_predicted = 10.0F;
      det_props[det_id_4 - 1].range_rate_dealiased = 10.0F;
      
      object_track.ndets = 4;
      object_track.detids[0] = det_id_1;
      object_track.detids[1] = det_id_2;
      object_track.detids[2] = det_id_3;
      object_track.detids[3] = det_id_4;

      object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      object_track.speed = 10.0F;
      object_track.curvature = 0.0F;
   }
};


/** \purpose
 * Check that a detection that is flagged as wheel spin is not downselected to track, even if range rate suggests that it is ok
 * \req CPR-3734
 */
TEST(f360_find_detection_inliers_single_object, Find_Detection_Inliers_Close_Target_Detection)
{
   /** \precond
    * Four detections with identical predicted and dealiased range rate have been set up in test group
    * CTCA object track with zero curvature have been set up in test group
    * The 4 detections have been associated to object in test group setup
    * Set detection 1, 2, 3 to be considered as all possible valid wheel spin types
    * Set 4th detection to a invalid wheelspin type
    * Set expected number of reduced detections to 1
    **/
   det_props[det_id_1 - 1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_NEARBY;
   det_props[det_id_2 - 1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   det_props[det_id_3 - 1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_DETECTION_PAIRS;
   det_props[det_id_4 - 1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;

   int32_t expected_num_rr_inlier_dets = 1;
   uint32_t expected_reduced_det_id = det_id_4;

   /** \action
    * Call function
    **/
   Find_Detection_Inliers_For_Single_Object(host, calibrations, object_track, det_props);

   /** \result
    * Expect number of reduced detection for object to be 1
    * Expect 4th detection associated to the object to have flag f_rr_inlier set to true.
    **/
   DOUBLES_EQUAL_TEXT(expected_num_rr_inlier_dets, object_track.num_rr_inlier_dets, 1, "Incorrect number of detections was downselected for object");
   for (uint32_t i_det = 0; i_det < MAX_NUMBER_OF_DETECTIONS; i_det++)
   {
      if(i_det == (expected_reduced_det_id - 1U))
      {
         CHECK_TRUE_TEXT(det_props[i_det].f_rr_inlier, "A detection was not marked as selected by track when it should have been")
      }
      else
      {
         CHECK_FALSE_TEXT(det_props[i_det].f_rr_inlier, "A detection was wrongly marked as selected by track")
      }
   }
}


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
      // Associate the two detections to track
      uint32_t det_id_1 = det_idx_1 + 1U;
      uint32_t det_id_2 = det_idx_2 + 1U;
      object_track.detids[0] = det_id_1;
      object_track.detids[1] = det_id_2;
      object_track.ndets = 2;
   }
};

/** \purpose
* Test that an associated detection with object wheel spin flag set is not considered to be a valid detection
* \req CPR-3734
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
* \req CPR-3734
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
   expected_valid_det_indices[1] = 0U;

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
* \req CPR-3734
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
/** @}*/
