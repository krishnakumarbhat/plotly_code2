/** \file
 * This file contains unit tests for content of f360_are_two_objects_preconditions_ok_for_merge.cpp file
 */

#include "f360_are_two_objects_preconditions_ok_for_merge.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_are_two_objects_preconditions_ok_for_merge
 *  @{
 */

/** \brief
 *  Test group for testing Are_Two_Objects_Preconditions_OK_For_Merge function.
 */
TEST_GROUP(f360_are_two_objects_preconditions_ok_for_merge)
{	
   // Initialize needed variables 
   F360_Calibrations_T calib;
   F360_Object_Track_T first_object = {};
   F360_Object_Track_T second_object = {};
   F360_Detection_Props_T detections[MAX_NUMBER_OF_DETECTIONS] = {};
   float32_t coarse_gate_score = 0.0F;

   const float32_t test_epsilon = 1e-5F;
   /** \setup
    * Initialize calibrations and set default properties of objects. It should make Are_Two_Objects_Preconditions_OK_For_Merge function return true.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      first_object.reference_point = F360_REFERENCE_POINT_CENTER;
      first_object.vcs_position = { 10.0F, 10.0F};
      first_object.bbox.Set_Center(first_object.vcs_position);
      first_object.vcs_heading = Angle{ F360_DEG2RAD(0.0F) };
      first_object.hdg_ptng_disagmt = 0.0F;
      first_object.Set_Bbox_Orientation(first_object.vcs_heading + first_object.hdg_ptng_disagmt);
      first_object.bbox.Set_Length(4.0F);
      first_object.bbox.Set_Width(2.0F);
      first_object.speed = calib.merging_m2m_max_obj_speed + test_epsilon;

      second_object.reference_point = F360_REFERENCE_POINT_CENTER;
      second_object.vcs_position = first_object.vcs_position;
      second_object.bbox.Set_Center(second_object.vcs_position);
      second_object.vcs_heading = first_object.vcs_heading;
      second_object.hdg_ptng_disagmt = first_object.hdg_ptng_disagmt;
      second_object.Set_Bbox_Orientation(second_object.vcs_heading + second_object.hdg_ptng_disagmt);
      second_object.bbox.Set_Length(4.0F);
      second_object.bbox.Set_Width(2.0F);
      second_object.speed = calib.merging_m2m_max_obj_speed + test_epsilon;
   }
};

/** \purpose
 *  Check whether merge preconditions pass if all conditions are met.
 * \req  NA.
 */
TEST(f360_are_two_objects_preconditions_ok_for_merge, return_true_if_all_conditions_met)
{
   /** \precond
    * Default properties of two objects, should make function pass.
    */

    /** \action
     * Call Are_Two_Objects_Preconditions_OK_For_Merge().
     */
   const bool f_two_objects_precond_ok = Are_Two_Objects_Preconditions_OK_For_Merge(calib, first_object, second_object, detections, coarse_gate_score);

   /** \result
    * Function should return false.
    */
   CHECK_TRUE(f_two_objects_precond_ok);
}

/** \purpose  
 *  Check whether merge preconditions fails if coarse gating returns false - distance between objects is too far. Set other objects parameters to meet other conditions.
 * \req  NA.
 */
TEST(f360_are_two_objects_preconditions_ok_for_merge, return_false_if_distance_between_objects_too_large)
{
   /** \precond
    * Set properties of two objects.
    */
   second_object.vcs_position.x = first_object.vcs_position.x + first_object.bbox.Get_Length() + second_object.bbox.Get_Length() + test_epsilon;
   second_object.bbox.Set_Center(second_object.vcs_position); // Note: Object reference_point was set to CENTER in TEST_SETUP()
   
   /** \action
    * Call Are_Two_Objects_Preconditions_OK_For_Merge().
    */
   const bool f_two_objects_precond_ok = Are_Two_Objects_Preconditions_OK_For_Merge(calib, first_object, second_object, detections, coarse_gate_score);

   /** \result
    * Function should return false. 
    */	
   CHECK_FALSE(f_two_objects_precond_ok);
}

/** \purpose
 *  Check whether merge preconditions fails if bounding boxes don't overlap. Set other objects parameters to meet other conditions.
 * \req  NA.
 */
TEST(f360_are_two_objects_preconditions_ok_for_merge, return_false_if_bboxes_not_overlap)
{
   /** \precond
    * Set properties of two objects. Set second object position vcs x to don't let bboxes overlap
    */
   second_object.vcs_position.x = first_object.vcs_position.x + 
                                  0.5F*first_object.bbox.Get_Length() + 
                                  0.5F*second_object.bbox.Get_Length() +
                                  1.5012F +      // 2*minimum possible bbox extension in Merge_Bbox_Overlap_Test()
                                   test_epsilon;
   second_object.bbox.Set_Center(second_object.vcs_position); // Note: Objects reference_point was set to CENTER in TEST_SETUP()

   /** \action
    * Call Are_Two_Objects_Preconditions_OK_For_Merge().
    */
   const bool f_two_objects_precond_ok = Are_Two_Objects_Preconditions_OK_For_Merge(calib, first_object, second_object, detections, coarse_gate_score);

   /** \result
    * Function should return false.
    */
   CHECK_FALSE(f_two_objects_precond_ok);
}

/** \purpose
 *  Check whether merge preconditions fails if metal to metal distance condition is not met. Set other objects parameters to meet other conditions.
 * \req  NA.
 */
TEST(f360_are_two_objects_preconditions_ok_for_merge, return_false_if_metal_to_metal_condition_fails)
{
   /** \precond
    * Set properties of two objects. Test case when objects has 
    */
   first_object.speed = calib.merging_m2m_max_obj_speed - test_epsilon;
   second_object.speed = calib.merging_m2m_max_obj_speed - test_epsilon;
   second_object.vcs_position.x = first_object.vcs_position.x + calib.merging_m2m_distance_threshold + test_epsilon;
   second_object.bbox.Set_Center(second_object.vcs_position); // Note: Object reference_point was set to CENTER in TEST_SETUP()

   /** \action
    * Call Are_Two_Objects_Preconditions_OK_For_Merge().
    */
   const bool f_two_objects_precond_ok = Are_Two_Objects_Preconditions_OK_For_Merge(calib, first_object, second_object, detections, coarse_gate_score);

   /** \result
    * Function should return false.
    */
   CHECK_FALSE(f_two_objects_precond_ok);
}

/** \purpose
 *  Check whether merge preconditions pass if bounding box gap between two long objects are within the long object threshold.
 * \req  NA.
 */
TEST(f360_are_two_objects_preconditions_ok_for_merge, bbox_gap_within_threshold_for_long_objects)
{
   /** \precond
    * Set both objects' length to 12 meters
    * Set both objects' reference point to CENTER REAR
    * Set bounding box gap between two objects to 1.9 meters
    */
   first_object.bbox.Set_Length(12.0F);
   first_object.reference_point = F360_REFERENCE_POINT_REAR;
   first_object.bbox.Set_Center(first_object.vcs_position.x + 0.5F*first_object.bbox.Get_Length(), first_object.vcs_position.y);
   second_object.bbox.Set_Length(12.0F);
   second_object.reference_point = F360_REFERENCE_POINT_REAR;
   second_object.vcs_position.x = first_object.vcs_position.x + first_object.bbox.Get_Length() + 1.9F;
   second_object.bbox.Set_Center(second_object.vcs_position.x + 0.5F*second_object.bbox.Get_Length(), second_object.vcs_position.y);

    /** \action
     * Call Are_Two_Objects_Preconditions_OK_For_Merge().
     */
   const bool f_two_objects_precond_ok = Are_Two_Objects_Preconditions_OK_For_Merge(calib, first_object, second_object, detections,coarse_gate_score);

   /** \result
    * Function should return True.
    */
   CHECK_TRUE(f_two_objects_precond_ok);
}

/** \purpose
 *  Check whether merge preconditions fail if bounding box gap between two long objects slightly exceeds the long object threshold.
 * \req  NA.
 */
TEST(f360_are_two_objects_preconditions_ok_for_merge, bbox_gap_exceed_threshold_for_long_objects)
{
   /** \precond
    * Set both objects' length to 12 meters
    * Set both objects' reference point to CENTER REAR
    * Set bounding box gap between two objects to 2.1 meters while the threshold is 2.0 meters
    */
   first_object.bbox.Set_Length(12.0F);
   first_object.reference_point = F360_REFERENCE_POINT_REAR;
   first_object.bbox.Set_Center(first_object.vcs_position.x + 0.5F*first_object.bbox.Get_Length(), first_object.vcs_position.y);
   second_object.bbox.Set_Length(12.0F);
   second_object.reference_point = F360_REFERENCE_POINT_REAR;
   second_object.vcs_position.x = first_object.vcs_position.x + first_object.bbox.Get_Length() + 2.1F;
   second_object.bbox.Set_Center(second_object.vcs_position.x + 0.5F*second_object.bbox.Get_Length(), second_object.vcs_position.y);

    /** \action
     * Call Are_Two_Objects_Preconditions_OK_For_Merge().
     */
   const bool f_two_objects_precond_ok = Are_Two_Objects_Preconditions_OK_For_Merge(calib, first_object, second_object, detections,coarse_gate_score);

   /** \result
    * Function should return false.
    */
   CHECK_FALSE(f_two_objects_precond_ok);
}

/** \purpose
 *  Check whether merge preconditions pass if bounding box gap between two long objects are within the saturation cap.
 * \req  NA.
 */
TEST(f360_are_two_objects_preconditions_ok_for_merge, bbox_gap_within_thres_cap_for_long_objects)
{
   /** \precond
    * Set both objects' length to 16 meters
    * Set both objects' reference point to CENTER REAR
    * Set bounding box gap between two objects to 2.49 meters while the threshold is 2.5 meters
    */
   first_object.bbox.Set_Length(16.0F);
   first_object.reference_point = F360_REFERENCE_POINT_REAR;
   first_object.bbox.Set_Center(first_object.vcs_position.x + 0.5F*first_object.bbox.Get_Length(), first_object.vcs_position.y);
   second_object.bbox.Set_Length(16.0F);
   second_object.reference_point = F360_REFERENCE_POINT_REAR;
   second_object.vcs_position.x = first_object.vcs_position.x + first_object.bbox.Get_Length() + 2.49F;
   second_object.bbox.Set_Center(second_object.vcs_position.x + 0.5F*second_object.bbox.Get_Length(), second_object.vcs_position.y);

    /** \action
     * Call Are_Two_Objects_Preconditions_OK_For_Merge().
     */
   const bool f_two_objects_precond_ok = Are_Two_Objects_Preconditions_OK_For_Merge(calib, first_object, second_object, detections, coarse_gate_score);

   /** \result
    * Function should return True.
    */
   CHECK_TRUE(f_two_objects_precond_ok);
}

/** \purpose
 *  Check whether merge preconditions fail if bounding box gap between two long objects slightly exceeds the saturation cap.
 * \req  NA.
 */
TEST(f360_are_two_objects_preconditions_ok_for_merge, bbox_gap_exceed_thres_cap_for_long_objects)
{
   /** \precond
    * Set both objects' length to 16 meters
    * Set both objects' reference point to CENTER REAR
    * Set bounding box gap between two objects to 2.51 meters while the threshold is 2.5 meters
    */
   first_object.bbox.Set_Length(16.0F);
   first_object.reference_point = F360_REFERENCE_POINT_REAR;
   first_object.bbox.Set_Center(first_object.vcs_position.x + 0.5F*first_object.bbox.Get_Length(), first_object.vcs_position.y);
   second_object.bbox.Set_Length(16.0F);
   second_object.reference_point = F360_REFERENCE_POINT_REAR;
   second_object.vcs_position.x = first_object.vcs_position.x + first_object.bbox.Get_Length() + 2.51F;
   second_object.bbox.Set_Center(second_object.vcs_position.x + 0.5F*second_object.bbox.Get_Length(), second_object.vcs_position.y);

    /** \action
     * Call Are_Two_Objects_Preconditions_OK_For_Merge().
     */
   const bool f_two_objects_precond_ok = Are_Two_Objects_Preconditions_OK_For_Merge(calib, first_object, second_object, detections, coarse_gate_score);

   /** \result
    * Function should return false.
    */
   CHECK_FALSE(f_two_objects_precond_ok);
}

/** \purpose
 *  Verify coarse_gate_score is propagated when all three preconditions pass.
 *  Wrapper should return true and expose the same coarse_gate_score as the coarse gate function.
 * \req  NA.
 */
TEST(f360_are_two_objects_preconditions_ok_for_merge, coarse_score_propagated_when_all_true)
{
   /** \precond
    *  Use default setup: coarse gate OK, bbox overlap OK, metal-to-metal OK.
    */
   float coarse_gate_score_wrapper = -1.0F;
   float coarse_gate_score_direct = -2.0F;

   // Compute expected score directly from the coarse gate function
   const bool cg_ok = Are_Merging_Coarse_Gate_Conditions_Met(
      first_object, second_object,
      calib.k_track_grouping_hdg_gate,
      calib.k_track_grouping_speed_gate,
      calib.k_track_grouping_curvature_gate,
      calib.k_merging_coarse_score_gate,
      coarse_gate_score_direct);
   CHECK_TRUE(cg_ok);

   /** \action
    *  Call wrapper Are_Two_Objects_Preconditions_OK_For_Merge.
    */
   const bool f_ok = Are_Two_Objects_Preconditions_OK_For_Merge(
      calib, first_object, second_object, detections, coarse_gate_score_wrapper);

   /** \result
    *  Wrapper returns true and coarse_gate_score is identical to direct computation.
    */
   CHECK_TRUE(f_ok);

   const float32_t test_pass_thres = 1e-4F;
   DOUBLES_EQUAL(coarse_gate_score_direct, coarse_gate_score_wrapper, test_pass_thres);
}

/** \purpose
 *  Verify coarse_gate_score is still propagated when bbox overlap fails.
 *  Wrapper should return false but coarse_gate_score must match the coarse gate function.
 * \req  NA.
 */
TEST(f360_are_two_objects_preconditions_ok_for_merge, coarse_score_propagated_when_bbox_fails)
{
   /** \precond
    *  Force bbox overlap to fail by moving second bbox just beyond overlap along x.
    *  Keep other parameters unchanged so coarse gate remains true.
    */
   second_object.vcs_position.x =
      first_object.vcs_position.x +
      0.5F * first_object.bbox.Get_Length() +
      0.5F * second_object.bbox.Get_Length() +
      1.5012F + // 2*minimum possible bbox extension in Merge_Bbox_Overlap_Test()
      1e-5F;
   second_object.bbox.Set_Center(second_object.vcs_position);

   float coarse_gate_score_wrapper = -1.0F;
   float coarse_gate_score_direct = -2.0F;

   // Coarse gate should still be OK and produce a score
   const bool cg_ok = Are_Merging_Coarse_Gate_Conditions_Met(
      first_object, second_object,
      calib.k_track_grouping_hdg_gate,
      calib.k_track_grouping_speed_gate,
      calib.k_track_grouping_curvature_gate,
      calib.k_merging_coarse_score_gate,
      coarse_gate_score_direct);
   CHECK_TRUE(cg_ok);

   /** \action
    *  Call wrapper Are_Two_Objects_Preconditions_OK_For_Merge.
    */
   const bool f_ok = Are_Two_Objects_Preconditions_OK_For_Merge(
      calib, first_object, second_object, detections, coarse_gate_score_wrapper);

   /** \result
    *  Wrapper returns false due to bbox overlap; coarse_gate_score matches coarse gate output.
    */
   CHECK_FALSE(f_ok);
   const float32_t test_pass_thres = 1e-4F;
   DOUBLES_EQUAL(coarse_gate_score_direct, coarse_gate_score_wrapper, test_pass_thres);
}

/** \purpose
 *  Verify coarse_gate_score is propagated when coarse gate fails.
 *  Wrapper should return false and coarse_gate_score must match the coarse gate function.
 * \req  NA.
 */
TEST(f360_are_two_objects_preconditions_ok_for_merge, coarse_score_propagated_when_coarse_gate_fails)
{
   /** \precond
    *  Force coarse gate to fail with heading beyond gate.
    *  Keep bbox overlap and M2M otherwise satisfiable (they will be short-circuited).
    */
   second_object.vcs_heading = first_object.vcs_heading + calib.k_track_grouping_hdg_gate + 1e-5F;
   second_object.bbox.Set_Orientation(second_object.vcs_heading + second_object.hdg_ptng_disagmt);

   float coarse_gate_score_wrapper = -1.0F;
   float coarse_gate_score_direct = -2.0F;

   // Coarse gate should fail and produce a score
   const bool cg_ok = Are_Merging_Coarse_Gate_Conditions_Met(
      first_object, second_object,
      calib.k_track_grouping_hdg_gate,
      calib.k_track_grouping_speed_gate,
      calib.k_track_grouping_curvature_gate,
      calib.k_merging_coarse_score_gate,
      coarse_gate_score_direct);
   CHECK_FALSE(cg_ok);

   /** \action
    *  Call wrapper Are_Two_Objects_Preconditions_OK_For_Merge.
    */
   const bool f_ok = Are_Two_Objects_Preconditions_OK_For_Merge(
      calib, first_object, second_object, detections, coarse_gate_score_wrapper);

   /** \result
    *  Wrapper returns false due to coarse gate; coarse_gate_score matches coarse gate output.
    */
   CHECK_FALSE(f_ok);
   const float32_t test_pass_thres = 1e-4F;
   DOUBLES_EQUAL(coarse_gate_score_direct, coarse_gate_score_wrapper, test_pass_thres);
}
/** @}*/

/** \defgroup  f360_merge_metal_to_metal_test
 *  @{
 */

 /** \brief
  *  Test group for testing Merge_Metal_To_Metal_Test function.
  */
TEST_GROUP(f360_merge_metal_to_metal_test)
{
   // Initialize needed variables 
   F360_Calibrations_T calib;
   F360_Object_Track_T first_object = {};
   F360_Object_Track_T second_object = {};
   F360_Detection_Props_T detections[MAX_NUMBER_OF_DETECTIONS] = {};
   const float32_t test_epsilon = 1e-5F;

   /** \setup
    * Initialize calibrations and set default properties of objects and detections. It should make Merge_Metal_To_Metal_Test function return true.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      detections[0].vcs_position = { 1.0F, -1.0F};

      detections[1].vcs_position = { 5.0F, 1.0F};

      detections[2].vcs_position.x = detections[1].vcs_position.x + calib.merging_m2m_distance_threshold - test_epsilon;
      detections[2].vcs_position.y = detections[0].vcs_position.y;

      detections[3].vcs_position.x = detections[2].vcs_position.x + 3.0F;
      detections[3].vcs_position.y = detections[0].vcs_position.y + calib.merging_lateral_det_spread_threshold - test_epsilon;

      first_object.detids[0] = 1U;
      first_object.detids[1] = 2U;
      first_object.ndets = 2;
      first_object.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(0.0F) });
      first_object.speed = calib.merging_m2m_max_obj_speed - test_epsilon;

      second_object.detids[0] = 3U;
      second_object.detids[1] = 4U;
      second_object.ndets = 2;
      second_object.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(0.0F) });
      second_object.speed = calib.merging_m2m_max_obj_speed - test_epsilon;
   }
};

/** \purpose
 *  Check if function returns true, if closest distance (metal2metal) and lateral spread are fine.
 * \req  NA.
 */
TEST(f360_merge_metal_to_metal_test, m2m_distance_and_lateral_spread_are_valid)
{
   /** \precond
    *  Default test setup - Merge_Metal_To_Metal_Test should return true.
    */

    /** \action
     * Call Merge_Metal_To_Metal_Test.
     */
   const bool m2m_test_valid = Merge_Metal_To_Metal_Test(first_object, second_object, detections, calib);

   /** \result
    * Merge_Metal_To_Metal_Test should return true.
    */
   CHECK_TRUE(m2m_test_valid);
}

/** \purpose
 *  Check if function returns False, if closest distance (metal2metal) is greater then maximum allowed value.
 * \req  NA.
 */
TEST(f360_merge_metal_to_metal_test, metal_to_metal_distance_too_large)
{
   /** \precond
    *  Set positions of detections associated to second object.
    */
   detections[2].vcs_position.x = detections[1].vcs_position.x + calib.merging_m2m_distance_threshold + test_epsilon;
   detections[2].vcs_position.y = detections[0].vcs_position.y;

   detections[3].vcs_position.x = detections[2].vcs_position.x + 3.0F;
   detections[3].vcs_position.y = detections[2].vcs_position.y + 2.0F;

   /** \action
    * Call Merge_Metal_To_Metal_Test.
    */
   const bool m2m_test_valid = Merge_Metal_To_Metal_Test(first_object, second_object, detections, calib);

   /** \result
    * Merge_Metal_To_Metal_Test should return false.
    */
   CHECK_FALSE(m2m_test_valid);
}

/** \purpose
 *  Check if function returns False, if closest distance (metal2metal) is fine but lateral spread is greater then maximum allowed value.
 * \req  NA.
 */
TEST(f360_merge_metal_to_metal_test, m2m_distance_ok_lateral_spread_too_large)
{
   /** \precond
    *  Set positions of detections associated to second object.
    */
   detections[2].vcs_position.x = detections[1].vcs_position.x + calib.merging_m2m_distance_threshold - test_epsilon;
   detections[2].vcs_position.y = detections[0].vcs_position.y;

   detections[3].vcs_position.x = detections[2].vcs_position.x + 3.0F;
   detections[3].vcs_position.y = detections[0].vcs_position.y + calib.merging_lateral_det_spread_threshold + test_epsilon;

   /** \action
    * Call Merge_Metal_To_Metal_Test.
    */
   const bool m2m_test_valid = Merge_Metal_To_Metal_Test(first_object, second_object, detections, calib);

   /** \result
    * Merge_Metal_To_Metal_Test should return false.
    */
   CHECK_FALSE(m2m_test_valid);
}

/** \purpose
 *  Check if function returns true, if speed of first object is over threshold value, but second one is below. Any other condition is met.
 * \req  NA.
 */
TEST(f360_merge_metal_to_metal_test, speed_of_first_object_too_large)
{
   /** \precond
    *  Set speed of first object.
    */
   first_object.speed = calib.merging_m2m_max_obj_speed + test_epsilon;

   /** \action
    * Call Merge_Metal_To_Metal_Test.
    */
   const bool m2m_test_valid = Merge_Metal_To_Metal_Test(first_object, second_object, detections, calib);

   /** \result
    * Merge_Metal_To_Metal_Test should return false.
    */
   CHECK_TRUE(m2m_test_valid);
}

/** \purpose
 *  Check if function returns true, if speed of second object is over threshold value, but first one is below. Any other condition is met.
 * \req  NA.
 */
TEST(f360_merge_metal_to_metal_test, speed_of_second_object_too_large)
{
   /** \precond
    *  Set speed of second object.
    */
   second_object.speed = calib.merging_m2m_max_obj_speed + test_epsilon;

   /** \action
    * Call Merge_Metal_To_Metal_Test.
    */
   const bool m2m_test_valid = Merge_Metal_To_Metal_Test(first_object, second_object, detections, calib);

   /** \result
    * Merge_Metal_To_Metal_Test should return false.
    */
   CHECK_TRUE(m2m_test_valid);
}

/** \purpose
 *  Check if function returns true, if speed of both objects is over threshold value. Any other condition is met.
 * \req  NA.
 */
TEST(f360_merge_metal_to_metal_test, speed_of_both_objects_too_large)
{
   /** \precond
    *  Set speeds of both objects.
    */
   first_object.speed = calib.merging_m2m_max_obj_speed + test_epsilon;
   second_object.speed = calib.merging_m2m_max_obj_speed + test_epsilon;

   /** \action
    * Call Merge_Metal_To_Metal_Test.
    */
   const bool m2m_test_valid = Merge_Metal_To_Metal_Test(first_object, second_object, detections, calib);

   /** \result
    * Merge_Metal_To_Metal_Test should return false.
    */
   CHECK_TRUE(m2m_test_valid);
}

/** \defgroup  f360_are_merging_coarse_gate_conditions_met
 *  @{
 */

 /** \brief
  *  Test group for testing Are_Merging_Coarse_Gate_Conditions_Met function.
  */
TEST_GROUP(f360_are_merging_coarse_gate_conditions_met)
{
   // Initialize needed variables 
   F360_Calibrations_T calib;
   F360_Object_Track_T first_object = {};
   F360_Object_Track_T second_object = {};
   const float32_t test_epsilon = 1e-5F;

   /** \setup
    * Initialize calibrations and set default properties of objects. It should make Are_Merging_Coarse_Gate_Conditions_Met function return true.
    */

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
	  
      first_object.reference_point = F360_REFERENCE_POINT_CENTER;
      first_object.vcs_position = { 10.0F, 10.0F};
      first_object.bbox.Set_Center(first_object.vcs_position);
      first_object.vcs_heading = Angle{ 0.0F };
      first_object.hdg_ptng_disagmt = 0.0F;
      first_object.bbox.Set_Orientation(first_object.vcs_heading + first_object.hdg_ptng_disagmt);
      first_object.bbox.Set_Length(4.0F);
      first_object.bbox.Set_Width(2.0F);
      first_object.speed = 10.0F;
      first_object.f_moving = true;
      first_object.movable_prob = 1.0F;

      second_object.reference_point = F360_REFERENCE_POINT_CENTER;
      second_object.vcs_position = first_object.vcs_position;
      second_object.bbox.Set_Center(second_object.vcs_position);
      second_object.vcs_heading = first_object.vcs_heading;
      second_object.hdg_ptng_disagmt = first_object.hdg_ptng_disagmt;
      second_object.bbox.Set_Orientation(second_object.vcs_heading + second_object.hdg_ptng_disagmt);
      second_object.bbox.Set_Length(first_object.bbox.Get_Length());
      second_object.bbox.Set_Width(first_object.bbox.Get_Width());
      second_object.speed = first_object.speed;
      second_object.f_moving = true;
      second_object.movable_prob = 1.0F;
   }
};

/** \purpose
 *  Check if function returns true, if both objects are moving. Set other objects parameters to be excatly the same
    to be sure if other conditions are met.
 * \req  NA.
 */
TEST(f360_are_merging_coarse_gate_conditions_met, return_true_if_both_objects_moving)
{
   /** \precond
    *  Default test setup - Are_Merging_Coarse_Gate_Conditions_Met should return true..
    */
   first_object.speed = 3.0F;
   second_object.speed = 3.0F;

   float coarse_gate_score = 0.0F;

   /** \action
    * Call Are_Merging_Coarse_Gate_Conditions_Met.
    */
   const bool conditions_met =
      Are_Merging_Coarse_Gate_Conditions_Met(
         first_object,
         second_object,
         calib.k_track_grouping_hdg_gate,
         calib.k_track_grouping_speed_gate,
         calib.k_track_grouping_curvature_gate,
         calib.k_merging_coarse_score_gate,
         coarse_gate_score);
   /** \result
    * Are_Merging_Coarse_Gate_Conditions_Met should return true.
    */
   CHECK_TRUE(conditions_met);
}

/** \purpose
 *  Check if function returns true, if both objects are stationary. Set other objects parameters to be excatly the same
    to be sure if other conditions are met.
 * \req  NA.
 */
TEST(f360_are_merging_coarse_gate_conditions_met, return_true_if_both_objects_stationary)
{
   /** \precond
    *  Set two objects movement status to be stationary.
    */
   first_object.f_moving = false;
   first_object.movable_prob = 0.0F;

   second_object.f_moving = false;
   second_object.movable_prob = 0.0F;
   float coarse_gate_score = 0.0F;

   /** \action
    * Call Are_Merging_Coarse_Gate_Conditions_Met.
    */
   const bool conditions_met =
      Are_Merging_Coarse_Gate_Conditions_Met(
         first_object,
         second_object,
         calib.k_track_grouping_hdg_gate,
         calib.k_track_grouping_speed_gate,
         calib.k_track_grouping_curvature_gate,
         calib.k_merging_coarse_score_gate,
         coarse_gate_score);

   /** \result
    * Are_Merging_Coarse_Gate_Conditions_Met should return true.
    */
   CHECK_TRUE(conditions_met);
}

/** \purpose
 *  Check if function returns false, if first object is moving and second is stopped. Set other objects parameters to be excatly the same
    to be sure if other conditions are met.
 * \req  NA.
 */
TEST(f360_are_merging_coarse_gate_conditions_met, return_false_if_first_moving_second_stopped)
{
   /** \precond
    *  Set second object movement status to 'stopped'.
    */
   second_object.f_moving = false;
   second_object.movable_prob = 1.0F;

   float coarse_gate_score = 0.0F;

   /** \action
    * Call Are_Merging_Coarse_Gate_Conditions_Met.
    */
   const bool conditions_met =
      Are_Merging_Coarse_Gate_Conditions_Met(
         first_object,
         second_object,
         calib.k_track_grouping_hdg_gate,
         calib.k_track_grouping_speed_gate,
         calib.k_track_grouping_curvature_gate,
         calib.k_merging_coarse_score_gate,
         coarse_gate_score);
   /** \result
    * Are_Merging_Coarse_Gate_Conditions_Met should return false.
    */
   CHECK_FALSE(conditions_met);
}

/** \purpose
 *  Check if function returns false, if first object is moving and second is stationary. Set other objects parameters to be excatly the same
    to be sure if other conditions are met.
 * \req  NA.
 */
TEST(f360_are_merging_coarse_gate_conditions_met, return_false_if_first_moving_second_stationary)
{
   /** \precond
    *  Set second object movement status to 'stationary'.
    */
   second_object.f_moving = false;
   second_object.movable_prob = 0.0F;

   float coarse_gate_score = 0.0F;

   /** \action
    * Call Are_Merging_Coarse_Gate_Conditions_Met.
    */
   const bool conditions_met =
      Are_Merging_Coarse_Gate_Conditions_Met(
         first_object,
         second_object,
         calib.k_track_grouping_hdg_gate,
         calib.k_track_grouping_speed_gate,
         calib.k_track_grouping_curvature_gate,
         calib.k_merging_coarse_score_gate,
         coarse_gate_score);
   /** \result
    * Are_Merging_Coarse_Gate_Conditions_Met should return false.
    */
   CHECK_FALSE(conditions_met);
}

/** \purpose
 *  Check if function returns false, if first object is stopped and second is moving. Set other objects parameters to be excatly the same
    to be sure if other conditions are met.
 * \req  NA.
 */
TEST(f360_are_merging_coarse_gate_conditions_met, return_false_if_first_stoped_second_moving)
{
   /** \precond
    *  Set first object movement status to 'stopped'.
    */
   first_object.f_moving = false;
   first_object.movable_prob = 1.0F;

   float coarse_gate_score = 0.0F;

   /** \action
    * Call Are_Merging_Coarse_Gate_Conditions_Met.
    */
   const bool conditions_met =
      Are_Merging_Coarse_Gate_Conditions_Met(
         first_object,
         second_object,
         calib.k_track_grouping_hdg_gate,
         calib.k_track_grouping_speed_gate,
         calib.k_track_grouping_curvature_gate,
         calib.k_merging_coarse_score_gate,
         coarse_gate_score);
   /** \result
    * Are_Merging_Coarse_Gate_Conditions_Met should return false.
    */
   CHECK_FALSE(conditions_met);
}

/** \purpose
 *  Check if function returns false, if both objects are stopped. Set other objects parameters to be excatly the same
    to be sure if other conditions are met.
 * \req  NA.
 */
TEST(f360_are_merging_coarse_gate_conditions_met, return_false_if_both_stopped)
{
   /** \precond
    *  Set both objects movement status to 'stopped'.
    */
   first_object.f_moving = false;
   first_object.movable_prob = 1.0F;

   second_object.f_moving = false;
   second_object.movable_prob = 1.0F;

   float coarse_gate_score = 0.0F;

   /** \action
    * Call Are_Merging_Coarse_Gate_Conditions_Met.
    */
   const bool conditions_met =
      Are_Merging_Coarse_Gate_Conditions_Met(
         first_object,
         second_object,
         calib.k_track_grouping_hdg_gate,
         calib.k_track_grouping_speed_gate,
         calib.k_track_grouping_curvature_gate,
         calib.k_merging_coarse_score_gate,
         coarse_gate_score);
   /** \result
    * Are_Merging_Coarse_Gate_Conditions_Met should return false.
    */
   CHECK_FALSE(conditions_met);
}

/** \purpose
 *  Check if function returns false, if first object is stopped and second stationary. Set other objects parameters to be excatly the same
    to be sure if other conditions are met.
 * \req  NA.
 */
TEST(f360_are_merging_coarse_gate_conditions_met, return_false_if_first_stopped_second_stationary)
{
   /** \precond
    *  Set first object movement status to 'stopped' and second to 'stationary'.
    */
   first_object.f_moving = false;
   first_object.movable_prob = 1.0F;

   second_object.f_moving = false;
   second_object.movable_prob = 0.0F;

   float coarse_gate_score = 0.0F;

   /** \action
    * Call Are_Merging_Coarse_Gate_Conditions_Met.
    */
   const bool conditions_met =
      Are_Merging_Coarse_Gate_Conditions_Met(
         first_object,
         second_object,
         calib.k_track_grouping_hdg_gate,
         calib.k_track_grouping_speed_gate,
         calib.k_track_grouping_curvature_gate,
         calib.k_merging_coarse_score_gate,
         coarse_gate_score);
   /** \result
    * Are_Merging_Coarse_Gate_Conditions_Met should return false.
    */
   CHECK_FALSE(conditions_met);
}


/** \purpose
 *  Check if function returns false, if first object is stationary and second is moving. Set other objects parameters to be excatly the same
    to be sure if other conditions are met.
 * \req  NA.
 */
TEST(f360_are_merging_coarse_gate_conditions_met, return_false_if_first_stationary_second_moving)
{
   /** \precond
    *  Set first object movement status to 'stationary'.
    */
   first_object.f_moving = false;
   first_object.movable_prob = 0.0F;

   float coarse_gate_score = 0.0F;

   /** \action
    * Call Are_Merging_Coarse_Gate_Conditions_Met.
    */
   const bool conditions_met =
      Are_Merging_Coarse_Gate_Conditions_Met(
         first_object,
         second_object,
         calib.k_track_grouping_hdg_gate,
         calib.k_track_grouping_speed_gate,
         calib.k_track_grouping_curvature_gate,
         calib.k_merging_coarse_score_gate,
         coarse_gate_score);
   /** \result
    * Are_Merging_Coarse_Gate_Conditions_Met should return false.
    */
   CHECK_FALSE(conditions_met);
}

/** \purpose
 *  Check if function returns false, if first object is stationary and second is stopped. Set other objects parameters to be excatly the same
    to be sure if other conditions are met.
 * \req  NA.
 */
TEST(f360_are_merging_coarse_gate_conditions_met, return_false_if_first_stationary_second_stopped)
{
   /** \precond
    *  Set first object movement status to 'stationary' and second to 'stopped'.
    */
   first_object.f_moving = false;
   first_object.movable_prob = 0.0F;

   second_object.f_moving = false;
   second_object.movable_prob = 1.0F;

   float coarse_gate_score = 0.0F;

   /** \action
    * Call Are_Merging_Coarse_Gate_Conditions_Met.
    */
   const bool conditions_met =
      Are_Merging_Coarse_Gate_Conditions_Met(
         first_object,
         second_object,
         calib.k_track_grouping_hdg_gate,
         calib.k_track_grouping_speed_gate,
         calib.k_track_grouping_curvature_gate,
         calib.k_merging_coarse_score_gate,
         coarse_gate_score);
   /** \result
    * Are_Merging_Coarse_Gate_Conditions_Met should return false.
    */
   CHECK_FALSE(conditions_met);
}

/** \purpose
 *  Check if function returns false, if longitudinal distance between objects is too large. Set other objects parameters to be excatly the same
    to be sure if other conditions are met.
 * \req  NA.
 */
TEST(f360_are_merging_coarse_gate_conditions_met, return_false_if_long_distance_between_objects_too_large)
{
   /** \precond
    *  Set position of second object.
    */
   second_object.vcs_position.x = first_object.vcs_position.x + first_object.bbox.Get_Length() + second_object.bbox.Get_Length() + test_epsilon;
   second_object.vcs_position.y = first_object.vcs_position.y;
   second_object.bbox.Set_Center(second_object.vcs_position); // Note: Object reference_point was set to CENTER in TEST_SETUP()
   float coarse_gate_score = 0.0F;

   /** \action
    * Call Are_Merging_Coarse_Gate_Conditions_Met.
    */
   const bool conditions_met =
      Are_Merging_Coarse_Gate_Conditions_Met(
         first_object,
         second_object,
         calib.k_track_grouping_hdg_gate,
         calib.k_track_grouping_speed_gate,
         calib.k_track_grouping_curvature_gate,
         calib.k_merging_coarse_score_gate,
         coarse_gate_score);
   /** \result
    * Are_Merging_Coarse_Gate_Conditions_Met should return false.
    */
   CHECK_FALSE(conditions_met);
}

/** \purpose
 *  Check if function returns false, if lateral distance between objects is too large. Set other objects parameters to be excatly the same
    to be sure if other conditions are met.
 * \req  NA.
 */
TEST(f360_are_merging_coarse_gate_conditions_met, return_false_if_lat_distance_between_objects_too_large)
{
   /** \precond
    *  Define two objects properties.
    */
   second_object.vcs_position.x = first_object.vcs_position.x;
   second_object.vcs_position.y = first_object.vcs_position.y + first_object.bbox.Get_Length() + second_object.bbox.Get_Length() + test_epsilon;
   second_object.bbox.Set_Center(second_object.vcs_position); // Note: Object reference_point was set to CENTER in TEST_SETUP()
   
   float coarse_gate_score = 0.0F;

   /** \action
    * Call Are_Merging_Coarse_Gate_Conditions_Met.
    */
   const bool conditions_met =
      Are_Merging_Coarse_Gate_Conditions_Met(
         first_object,
         second_object,
         calib.k_track_grouping_hdg_gate,
         calib.k_track_grouping_speed_gate,
         calib.k_track_grouping_curvature_gate,
         calib.k_merging_coarse_score_gate,
         coarse_gate_score);
   /** \result
    * Are_Merging_Coarse_Gate_Conditions_Met should return false.
    */
   CHECK_FALSE(conditions_met);
}

/** \purpose
 *  Check if function returns false, if longitudinal and lateral distances between objects is too large. Set other objects parameters to be excatly the same
    to be sure if other conditions are met.
 * \req  NA.
 */
TEST(f360_are_merging_coarse_gate_conditions_met, return_false_if_long_and_lat_distances_between_objects_too_large)
{
   /** \precond
    *  Define two objects properties.
    */
   second_object.vcs_position.x = first_object.vcs_position.x + first_object.bbox.Get_Length() + second_object.bbox.Get_Length() + test_epsilon;
   second_object.vcs_position.y = first_object.vcs_position.y + first_object.bbox.Get_Length() + second_object.bbox.Get_Length() + test_epsilon;
   second_object.bbox.Set_Center(second_object.vcs_position); // Note: Object reference_point was set to CENTER in TEST_SETUP()
   
   float coarse_gate_score = 0.0F;

   /** \action
    * Call Are_Merging_Coarse_Gate_Conditions_Met.
    */
   const bool conditions_met =
      Are_Merging_Coarse_Gate_Conditions_Met(
         first_object,
         second_object,
         calib.k_track_grouping_hdg_gate,
         calib.k_track_grouping_speed_gate,
         calib.k_track_grouping_curvature_gate,
         calib.k_merging_coarse_score_gate,
         coarse_gate_score);
   /** \result
    * Are_Merging_Coarse_Gate_Conditions_Met should return false.
    */
   CHECK_FALSE(conditions_met);
}

/** \purpose
 *  Check if function returns false, if heading difference between objects is too large. Set other objects parameters to be excatly the same
    to be sure if other conditions are met.
 * \req  NA.
 */
TEST(f360_are_merging_coarse_gate_conditions_met, return_false_if_heading_diff_too_large)
{
   /** \precond
    *  Set heading of second object.
    */
   second_object.vcs_heading = first_object.vcs_heading + calib.k_track_grouping_hdg_gate + test_epsilon;
   second_object.bbox.Set_Orientation(second_object.vcs_heading + second_object.hdg_ptng_disagmt);
   
   float coarse_gate_score = 0.0F;

   /** \action
    * Call Are_Merging_Coarse_Gate_Conditions_Met.
    */
   const bool conditions_met =
      Are_Merging_Coarse_Gate_Conditions_Met(
         first_object,
         second_object,
         calib.k_track_grouping_hdg_gate,
         calib.k_track_grouping_speed_gate,
         calib.k_track_grouping_curvature_gate,
         calib.k_merging_coarse_score_gate,
         coarse_gate_score);
   /** \result
    * Are_Merging_Coarse_Gate_Conditions_Met should return false.
    */
   CHECK_FALSE(conditions_met);
}

/** \purpose
 *  Check if function returns true, when heading difference between objects is within threshold. Set other objects parameters to be excatly the same
    to be sure if other conditions are met.
 * \req  NA.
 */
TEST(f360_are_merging_coarse_gate_conditions_met, return_true_if_heading_diff_within_threshold)
{
   /** \precond
    *  Define two objects properties.
    */
   float32_t add_to_heading =  F360_DEG2RAD(29.9F); // Value that appends to the heading of first object
   second_object.vcs_heading = first_object.vcs_heading + add_to_heading; // Set the heading of second object, such that the heading difference between the two objects is close to threshold but still within threshold
   second_object.bbox.Set_Orientation(second_object.vcs_heading + second_object.hdg_ptng_disagmt);

   float coarse_gate_score = 0.0F;

   /** \action
    * Call Are_Merging_Coarse_Gate_Conditions_Met.
    */
   const bool conditions_met =
      Are_Merging_Coarse_Gate_Conditions_Met(
         first_object,
         second_object,
         calib.k_track_grouping_hdg_gate,
         calib.k_track_grouping_speed_gate,
         calib.k_track_grouping_curvature_gate,
         calib.k_merging_coarse_score_gate,
         coarse_gate_score);
   /** \result
    * Are_Merging_Coarse_Gate_Conditions_Met, because of heading, should return true.
    */
   CHECK_TRUE(conditions_met);
}

/** \purpose
 *  Check if function returns false, if speed difference between objects is too large. Set other objects parameters to be excatly the same
    to be sure if other conditions are met.
 * \req  NA.
 */
TEST(f360_are_merging_coarse_gate_conditions_met, return_false_if_speed_diff_too_large)
{
   /** \precond
    *  Define two objects properties.
    */
   second_object.speed = first_object.speed + calib.k_track_grouping_speed_gate + test_epsilon;
   second_object.bbox.Set_Center(second_object.vcs_position); // Note: Object reference_point was set to CENTER in TEST_SETUP()
   
   float coarse_gate_score = 0.0F;

   /** \action
    * Call Are_Merging_Coarse_Gate_Conditions_Met.
    */
   const bool conditions_met =
      Are_Merging_Coarse_Gate_Conditions_Met(
         first_object,
         second_object,
         calib.k_track_grouping_hdg_gate,
         calib.k_track_grouping_speed_gate,
         calib.k_track_grouping_curvature_gate,
         calib.k_merging_coarse_score_gate,
         coarse_gate_score);
   /** \result
    * Are_Merging_Coarse_Gate_Conditions_Met should return false.
    */
   CHECK_FALSE(conditions_met);
}

/** \purpose
 *  Check if function returns true, when speed difference between objects is within threshold. Set other objects parameters to be excatly the same
    to be sure if other conditions are met.
 * \req  NA.
 */
TEST(f360_are_merging_coarse_gate_conditions_met, return_true_if_speed_diff_within_threshold)
{
   /** \precond
    *  Define two objects properties.
    */
   float32_t add_to_speed =  0.99F; // Value that appends to the speed of first object
   second_object.speed = first_object.speed + add_to_speed; // Set the speed of second object, such that the speed difference between the two objects is close to threshold but still within threshold
   second_object.bbox.Set_Center(second_object.vcs_position); // Note: Object reference_point was set to CENTER in TEST_SETUP()

   float coarse_gate_score = 0.0F;

   /** \action
    * Call Are_Merging_Coarse_Gate_Conditions_Met.
    */
   const bool conditions_met =
      Are_Merging_Coarse_Gate_Conditions_Met(
         first_object,
         second_object,
         calib.k_track_grouping_hdg_gate,
         calib.k_track_grouping_speed_gate,
         calib.k_track_grouping_curvature_gate,
         calib.k_merging_coarse_score_gate,
         coarse_gate_score);
   /** \result
    * Are_Merging_Coarse_Gate_Conditions_Met, because of speed, should return true.
    */
   CHECK_TRUE(conditions_met);
}

/** \purpose
 *  Check if function returns false, if curvature difference between objects is too large. Set other objects parameters to be excatly the same
    to be sure if other conditions are met.
 * \req  NA.
 */
TEST(f360_are_merging_coarse_gate_conditions_met, return_false_if_curvature_diff_too_large)
{
   /** \precond
    *  Define two objects properties.
    */
   second_object.curvature = first_object.curvature + calib.k_track_grouping_curvature_gate + test_epsilon;

   float coarse_gate_score = 0.0F;

   /** \action
    * Call Are_Merging_Coarse_Gate_Conditions_Met.
    */
   const bool conditions_met =
      Are_Merging_Coarse_Gate_Conditions_Met(
         first_object,
         second_object,
         calib.k_track_grouping_hdg_gate,
         calib.k_track_grouping_speed_gate,
         calib.k_track_grouping_curvature_gate,
         calib.k_merging_coarse_score_gate,
         coarse_gate_score);

   /** \result
    * Are_Merging_Coarse_Gate_Conditions_Met, because of curvature, should return false.
    */
   CHECK_FALSE(conditions_met);
}

/** \purpose
 *  Check if function returns true, when curvature difference between objects is within threshold. Set other objects parameters to be excatly the same
    to be sure if other conditions are met.
 * \req  NA.
 */
TEST(f360_are_merging_coarse_gate_conditions_met, return_true_if_curvature_diff_within_threshold)
{
   /** \precond
    *  Define two objects properties.
    */
   float32_t add_to_curvature =  0.0199F; // Value that appends to the curvature of first object
   second_object.curvature = first_object.curvature + add_to_curvature; // Set the curvature of second object, such that the curvature difference between the two objects is close to threshold but still within threshold
   float coarse_gate_score = 0.0F;

   /** \action
    * Call Are_Merging_Coarse_Gate_Conditions_Met.
    */
   const bool conditions_met =
      Are_Merging_Coarse_Gate_Conditions_Met(
         first_object,
         second_object,
         calib.k_track_grouping_hdg_gate,
         calib.k_track_grouping_speed_gate,
         calib.k_track_grouping_curvature_gate,
         calib.k_merging_coarse_score_gate,
         coarse_gate_score);

   /** \result
    * Are_Merging_Coarse_Gate_Conditions_Met, because of curvature, should return true.
    */
   CHECK_TRUE(conditions_met);
}

/** \purpose
 *  Check if function returns false, when both objects have low speed and all parameters are within thresholds but overall score is too big.
 * \req  NA.
 */
TEST(f360_are_merging_coarse_gate_conditions_met, return_false_if_score_too_large)
{
   /** \purpose
    * Verify function returns false when both objects are slow and the combined score is too large,
    * while individual gates still pass.
    * \req NA.
    */

   /** \precond
    * Set speeds below 4.0F to trigger slow-speed multipliers.
    * Increase longitudinal separation and heading difference to push the score above threshold,
    * but still within individual gates.
    */
   first_object.speed = 2.5F;
   second_object.speed = 3.0F;

   second_object.vcs_position.x = first_object.vcs_position.x + 5.0F;
   second_object.vcs_position.y = first_object.vcs_position.y;
   second_object.bbox.Set_Center(second_object.vcs_position);

   // Keep curvature equal so curvature gate passes.
   second_object.curvature = first_object.curvature;

   // Set a modest heading difference that is within the heading gate but contributes to the score.
   second_object.vcs_heading.Value(first_object.vcs_heading.Value() + F360_DEG2RAD(15.0F));

   float coarse_gate_score = 0.0F;

   /** \action
    * Call Are_Merging_Coarse_Gate_Conditions_Met.
    */
   const bool conditions_met =
      Are_Merging_Coarse_Gate_Conditions_Met(
         first_object,
         second_object,
         calib.k_track_grouping_hdg_gate,
         calib.k_track_grouping_speed_gate,
         calib.k_track_grouping_curvature_gate,
         calib.k_merging_coarse_score_gate,
         coarse_gate_score);

   /** \result
    * Because both objects are slow, the final condition requires score < threshold.
    * With the configured diffs, the score is too large, so the function should return false.
    */
   CHECK_FALSE(conditions_met);
}
/** @}*/
