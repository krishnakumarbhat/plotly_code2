/** \file
   Give a detailed description of what  this unit-test file contain.
*/

#include "f360_track_downselection_internal_functions.h"
#include "f360_bounding_box.h"
#include "f360_math.h"
#include "f360_set_variant.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>
#include <cstring>

using namespace f360_variant_A;

/** \defgroup  Decrease_Priority_and_Confidence_for_Implausible_Tracks
 *  @{
 */

/** \brief
 *  Test group for Decrease_Priority_and_Confidence_for_Implausible_Tracks()
 **/
TEST_GROUP(Decrease_Priority_and_Confidence_for_Implausible_Tracks)
{
   F360_Host_T host;
   F360_Calibrations_T calib = {};
   F360_Tracker_Info_T tracker_info;
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS];
   float32_t initial_priority = 0.5F;
   float32_t initial_confidenceLevel = 1.0F;
   const float32_t test_pass_th = 1e-8F;
   float32_t priority;

   TEST_SETUP()
   {
      priority = initial_priority;
      object_tracks[0].conf_overall = CONF3_HIGH;
      Initialize_Tracker_Calibrations(calib);
      Set_Tracker_Variant(tracker_info.variant);
   }
};

/** \brief
 *  Test checks if priority and confidence are set correclty for third type of ghosts - reflective tracks behind guardrail in CV trailer scenario.
 **/
TEST(Decrease_Priority_and_Confidence_for_Implausible_Tracks, check_if_function_sets_correct_priority_and_confidence_for_CV_trailer_scenario)
{
   /** \precond
    * f_highway_suspected fla gis set to true
    * First object satisfies all conditions to be type 3 ghost
    * Second object satisfies all conditions to be type 3 ghost except is too slow
    * Third object satisfies all conditions to be type 3 ghost except moves in opposite direction
    * Fourth object satisfies all conditions to be type 3 ghost except is not behind guardrail
    * Fifth object is behind guardrail, but moves too slow in opposite direction
   */
  tracker_info.f_highway_suspected = true;
  for (uint16_t i = 0U; i < 5; i++)
  {
      object_tracks[i].confidenceLevel = initial_confidenceLevel;
  }
   object_tracks[0].behind_sep_id = 1U;
   object_tracks[0].vcs_heading = Angle{0.0F};
   object_tracks[0].speed = 11.0F;

   object_tracks[1].behind_sep_id = 1U;
   object_tracks[1].vcs_heading = Angle{0.0F};
   object_tracks[1].speed = 8.0F;

   object_tracks[2].behind_sep_id = 1U;
   object_tracks[2].vcs_heading = Angle{3.0F};
   object_tracks[2].speed = 11.0F;

   object_tracks[3].behind_sep_id = F360_INVALID_UNSIGNED_ID;
   object_tracks[3].vcs_heading = Angle{0.0F};
   object_tracks[3].speed = 11.0F;

   object_tracks[4].behind_sep_id = 1U;
   object_tracks[4].vcs_heading = Angle{3.0F};
   object_tracks[4].speed = 8.0F;

  float32_t priorities[5] = {};
  for (uint16_t i = 0U; i < 5; i++)
  {
   priorities[i] = initial_priority;

   /** \action
      Call Decrease_Priority_and_Confidence_for_Implausible_Tracks for each track
   */
   Decrease_Priority_and_Confidence_for_Implausible_Tracks(tracker_info, calib, object_tracks[i], priorities[i]);

  }

   /** \result
      Check result for each track. First track should have confidenceLevel decreased and priority set to 0 (not a candidate), rest of tracks should have confidenceLevel and priority not modified.
   */

   CHECK_TRUE(object_tracks[0].confidenceLevel < initial_confidenceLevel);
   DOUBLES_EQUAL(0.0F, priorities[0], test_pass_th);

   for (uint16_t i = 1U; i < 5; i++)
   {
      DOUBLES_EQUAL(initial_confidenceLevel, object_tracks[i].confidenceLevel, test_pass_th);
      DOUBLES_EQUAL(initial_priority, priorities[i], test_pass_th);
   }
}

/** \brief
 *  Test checks if mirror object track has modified priority and confidence when mirror probability exceeds threshold.
 **/
TEST(Decrease_Priority_and_Confidence_for_Implausible_Tracks, check_if_function_change_priority_and_confidence_for_mirror_object)
{
   /** \precond
    * mirror_prob is set above the threshold to trigger mirror logic
    * movable_prob is set to 0 to avoid ghost type 1 conditions
    * object confidence is set to initial value to check if it gets decreased
    * object is not behind guardrail to avoid ghost type 1 conditions
   */
   object_tracks[0].confidenceLevel = initial_confidenceLevel;

   object_tracks[0].behind_sep_id = F360_INVALID_UNSIGNED_ID;
   object_tracks[0].on_sep_id = F360_INVALID_UNSIGNED_ID;
   object_tracks[0].movable_prob = 0.0F; // not movable to avoid other ghost conditions
   
   object_tracks[0].mirror_prob = calib.k_mirror_prob_threshold + 0.1F; // Above threshold to trigger mirror logic

   /** \action
      Call Decrease_Priority_and_Confidence_for_Implausible_Tracks
   */
   Decrease_Priority_and_Confidence_for_Implausible_Tracks(tracker_info, calib, object_tracks[0], priority);

   /** \result
      After calling the function the priority should be set to 0 (not a candidate) and confidence should be modified
   */
   CHECK_TRUE(object_tracks[0].confidenceLevel < initial_confidenceLevel);
   DOUBLES_EQUAL(0.0F, priority, test_pass_th);
}

/** \brief
 *  Test checks if object with low mirror probability has not modified priority and confidence.
 **/
TEST(Decrease_Priority_and_Confidence_for_Implausible_Tracks, check_if_function_not_change_priority_and_confidence_for_low_mirror_prob_object)
{
   /** \precond
    * mirror_prob is set below the threshold to not trigger mirror logic
    * movable_prob is set to 0 to avoid ghost type 1 conditions
    * object confidence is set to initial value to check if it gets decreased
    * object is not behind guardrail to avoid ghost type 1 conditions
   */
   object_tracks[0].confidenceLevel = initial_confidenceLevel;

   object_tracks[0].behind_sep_id = F360_INVALID_UNSIGNED_ID;
   object_tracks[0].on_sep_id = F360_INVALID_UNSIGNED_ID;
   object_tracks[0].movable_prob = 0.0F; // not movable to avoid other ghost conditions
   
   object_tracks[0].mirror_prob = calib.k_mirror_prob_threshold - 0.1F; // Below threshold to not trigger mirror logic

   /** \action
      Call Decrease_Priority_and_Confidence_for_Implausible_Tracks
   */
   Decrease_Priority_and_Confidence_for_Implausible_Tracks(tracker_info, calib, object_tracks[0], priority);

   /** \result
      After calling the function the priority and confidence should NOT be modified
   */
   DOUBLES_EQUAL(initial_confidenceLevel, object_tracks[0].confidenceLevel, test_pass_th);
   DOUBLES_EQUAL(initial_priority, priority, test_pass_th);
}

/** \brief
 *  Test checks if function correctly handles ghost object that already has very low confidence.
 *  This test covers the branch where the ghost condition is true but confidence
 *  is already below the threshold, so confidence doesn't get modified but priority still increases.
 **/
TEST(Decrease_Priority_and_Confidence_for_Implausible_Tracks, check_if_function_handles_ghost_object_with_already_low_confidence)
{
   /** \precond
    * suspected highway condition is true
    * object is behind guardrail
    * object has high movable probability
    * object has low confidence below the threshold for lowering confidence
    * object has high enough speed and low enough heading to satisfy highway ghost conditions
    * mirror probability is set to 0 to avoid mirror logic
   */
   
   // Set confidence below the threshold (0.9 * 0.3 = 0.27)
   object_tracks[0].confidenceLevel = 0.25F; // Below threshold, should not be modified
   const float32_t initial_low_confidence = object_tracks[0].confidenceLevel;
   
   // Configure as ghost type 1 - behind guardrail, movable, insufficient detections
   tracker_info.f_highway_suspected = true;
   object_tracks[0].behind_sep_id = 1.0F; // Behind guardrail
   object_tracks[0].movable_prob = 1.0F; // High movable probability  
   object_tracks[0].speed = 11.0F; // Satisfy speed condition for highway ghost flag
   object_tracks[0].vcs_heading = Angle{ 0.0F }; // Satisfy heading condition for highway ghost flag

   
   // Ensure other ghost condition is false
   object_tracks[0].mirror_prob = 0.0F;

   /** \action
      Call Decrease_Priority_and_Confidence_for_Implausible_Tracks
   */
   Decrease_Priority_and_Confidence_for_Implausible_Tracks(tracker_info, calib, object_tracks[0], priority);

   /** \result
      Priority should be set to 0 (not a candidate since ghost detected), but confidence should remain unchanged
      since it's already below the lowering threshold
   */
   DOUBLES_EQUAL(initial_low_confidence, object_tracks[0].confidenceLevel, test_pass_th); // Confidence unchanged
   DOUBLES_EQUAL(0.0F, priority, test_pass_th); // Priority set to 0 (not a candidate)
}

/** @}*/

/** \defgroup  Increase_Priority_and_Adjust_EP_Variant_Based
 *  @{
 */

/** \brief
 *  Test group for Increase_Priority_and_Adjust_EP_Variant_Based()
 **/
TEST_GROUP(Increase_Priority_and_Adjust_EP_Variant_Based)
{
   F360_Host_T host = {};
   F360_Calibrations_T calib = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Object_Track_T obj = {};
   float32_t priority;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      Set_Tracker_Variant(tracker_info.variant); // defaults variant
      host.speed = 12.0F; // nominal forward speed
      obj.exist_prob = 0.95F;
      obj.movable_prob = 1.0F; // movable by default
      obj.time_since_initialization = 0.20F; // > age threshold (0.140F) unless modified
      obj.vcs_heading = Angle{0.05F}; // near straight
      obj.vcs_position.y = 5.0F; // within lateral window (<20)
      obj.vcs_position.x = 30.0F; // arbitrary forward position
      obj.speed = 12.0F;
      obj.priority = 0.8F; // object-stored priority assigned by function in variant-D path
      // function is called when priority <= 0, so start at 0
      priority = 0.0F;
   }
};

/**
*\purpose  Verify that for non-variant-D tracker, object priority and existence probability remain unchanged.
*\req    NA.
*/
TEST(Increase_Priority_and_Adjust_EP_Variant_Based, non_variant_D_no_change)
{
   /** \precond
    * Tracker variant set to a type other than D; object satisfies all potential variant-D criteria.
    */
   tracker_info.variant.type = F360_VARIANT_TYPE_A;
   const float32_t starting_priority = priority;
   const float32_t starting_ep = obj.exist_prob;

   /** \action
    * Call Increase_Priority_and_Adjust_EP_Variant_Based.
    */
   Increase_Priority_and_Adjust_EP_Variant_Based(tracker_info.variant.type, obj, priority);

   /** \result
    * Existence probability and priority unchanged for non-D variant.
    */
   DOUBLES_EQUAL(starting_priority, priority, 1e-6F);
   DOUBLES_EQUAL(starting_ep, obj.exist_prob, 1e-6F);
}

/**
*\purpose  Verify that variant D straight-moving movable object after age threshold has recalculated priority and capped existence probability.
*\req    NA.
*/
TEST(Increase_Priority_and_Adjust_EP_Variant_Based, variant_D_reduces_EP_and_recalculates_priority)
{
   /** \precond
    * Variant set to D; object movable, sufficiently old, moving straight, lateral distance within window; EP high.
    */
   tracker_info.variant.type = F360_VARIANT_TYPE_D;
   const float32_t starting_priority = priority;

   /** \action
    * Call Increase_Priority_and_Adjust_EP_Variant_Based.
    */
   Increase_Priority_and_Adjust_EP_Variant_Based(tracker_info.variant.type, obj, priority);

   /** \result
    * Existence probability lowered to 0.61F; priority increased from 0 to obj.priority.
    */
   DOUBLES_EQUAL(0.61F, obj.exist_prob, 1e-6F);
   DOUBLES_EQUAL(obj.priority, priority, 1e-6F);
   CHECK_TRUE(priority > starting_priority); // increased from 0 to obj.priority
}

/**
*\purpose  Verify that variant D object below age threshold does not trigger priority/EP adjustment.
*\req    NA.
*/
TEST(Increase_Priority_and_Adjust_EP_Variant_Based, variant_D_age_below_threshold_no_change)
{
   /** \precond
    * Variant D; object meets all criteria except age (<0.140F).
    */
   tracker_info.variant.type = F360_VARIANT_TYPE_D;
   obj.time_since_initialization = 0.05F; // below threshold
   const float32_t starting_priority = priority;
   const float32_t starting_ep = obj.exist_prob;

   /** \action */
   Increase_Priority_and_Adjust_EP_Variant_Based(tracker_info.variant.type, obj, priority);

   /** \result */
   DOUBLES_EQUAL(starting_priority, priority, 1e-6F);
   DOUBLES_EQUAL(starting_ep, obj.exist_prob, 1e-6F);
}

/**
*\purpose  Verify that variant D object outside lateral window does not trigger adjustment.
*\req    NA.
*/
TEST(Increase_Priority_and_Adjust_EP_Variant_Based, variant_D_outside_lateral_window_no_change)
{
   /** \precond
    * Variant D; object meets all criteria except lateral window (|y| >= 20).
    */
   tracker_info.variant.type = F360_VARIANT_TYPE_D;
   obj.vcs_position.y = 25.0F; // outside window
   const float32_t starting_priority = priority;
   const float32_t starting_ep = obj.exist_prob;

   /** \action */
   Increase_Priority_and_Adjust_EP_Variant_Based(tracker_info.variant.type, obj, priority);

   /** \result */
   DOUBLES_EQUAL(starting_priority, priority, 1e-6F);
   DOUBLES_EQUAL(starting_ep, obj.exist_prob, 1e-6F);
}

/**
*\purpose  Verify that variant D object with large heading deviation (not straight) does not trigger adjustment.
*\req    NA.
*/
TEST(Increase_Priority_and_Adjust_EP_Variant_Based, variant_D_not_moving_straight_no_change)
{
   /** \precond
    * Variant D; object meets all criteria except straight heading.
    */
   tracker_info.variant.type = F360_VARIANT_TYPE_D;
   obj.vcs_heading = Angle{1.2F}; // exceeds 45 deg threshold (0.785398...)
   const float32_t starting_priority = priority;
   const float32_t starting_ep = obj.exist_prob;

   /** \action */
   Increase_Priority_and_Adjust_EP_Variant_Based(tracker_info.variant.type, obj, priority);

   /** \result */
   DOUBLES_EQUAL(starting_priority, priority, 1e-6F);
   DOUBLES_EQUAL(starting_ep, obj.exist_prob, 1e-6F);
}

/**
*\purpose  Verify that variant D non-movable object does not trigger adjustment.
*\req    NA.
*/
TEST(Increase_Priority_and_Adjust_EP_Variant_Based, variant_D_not_movable_no_change)
{
   /** \precond
    * Variant D; object meets all criteria except movable probability (<=0.5).
    */
   tracker_info.variant.type = F360_VARIANT_TYPE_D;
   obj.movable_prob = 0.2F; // below threshold
   const float32_t starting_priority = priority;
   const float32_t starting_ep = obj.exist_prob;

   /** \action */
   Increase_Priority_and_Adjust_EP_Variant_Based(tracker_info.variant.type, obj, priority);

   /** \result */
   DOUBLES_EQUAL(starting_priority, priority, 1e-6F);
   DOUBLES_EQUAL(starting_ep, obj.exist_prob, 1e-6F);
}

/**
*\purpose  Verify that variant D adjustment clamps existence probability but does not raise it if already below cap.
*\req    NA.
*/
TEST(Increase_Priority_and_Adjust_EP_Variant_Based, variant_D_already_low_EP_priority_recalc_only)
{
   /** \precond
    * Variant D; object satisfies all criteria, exist_prob already below cap (0.55 < 0.61).
    */
   tracker_info.variant.type = F360_VARIANT_TYPE_D;
   obj.exist_prob = 0.55F; // already lower
   const float32_t starting_ep = obj.exist_prob;
   const float32_t starting_priority = priority;

   /** \action */
   Increase_Priority_and_Adjust_EP_Variant_Based(tracker_info.variant.type, obj, priority);

   /** \result */
   DOUBLES_EQUAL(starting_ep, obj.exist_prob, 1e-6F); // not increased
   CHECK_TRUE(priority != starting_priority);
}
/** @}*/


/** \defgroup  Pop_Reduced_Id
 *  @{
 */

/** \brief
 *  Test group for Pop_Reduced_Id()
 **/
TEST_GROUP(Pop_Reduced_Id)
{
   F360_Tracker_Info_T tracker_info = {};

   TEST_SETUP()
   {
      Set_Tracker_Variant(tracker_info.variant);
   }
};

/** \brief
 *  Test checks if first track idx from tracker_info.reduced_inactive_obj_ids is issued.
 **/
TEST(Pop_Reduced_Id, check_if_function_issue_track_idx)
{

   /** \precond
   */
   int32_t reduced_id = 0;
   tracker_info.reduced_num_active_objs = 4;
   int32_t num_obj = tracker_info.reduced_num_active_objs;

   for (unsigned int i = 0; i < NUMBER_OF_REDUCED_OBJECT_TRACKS - tracker_info.reduced_num_active_objs; i++)
   {
      tracker_info.reduced_inactive_obj_ids[i] = i + 1 + tracker_info.reduced_num_active_objs;
   }

   int32_t expexted_track_id = tracker_info.reduced_inactive_obj_ids[0];

   /** \action
      Call Pop_Reduced_Id
   */
   reduced_id = Pop_Reduced_Id(tracker_info);

   /** \result
      After pop first element array is shifted to the front and last element should be equal 0
   */
   CHECK_EQUAL(0, tracker_info.reduced_inactive_obj_ids[NUMBER_OF_REDUCED_OBJECT_TRACKS - tracker_info.reduced_num_active_objs]);
   CHECK_EQUAL(expexted_track_id, reduced_id);
   CHECK_EQUAL(reduced_id, tracker_info.reduced_active_obj_ids[num_obj]);
   CHECK_EQUAL(num_obj + 1, tracker_info.reduced_num_active_objs);
}

/** \defgroup  Calc_Track_Priority
 *  @{
 */

/** \brief
 *  Test group for Calc_Track_Priority()
 **/
TEST_GROUP(Calc_Track_Priority)
{
   F360_Host_T host;
   F360_Calibrations_T calib = {};
   F360_Tracker_Info_T tracker_info;
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS];
   Static_Env_Poly_T stat_env_poly[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   BoundingBox overall_confidence_exclusion_box{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   bool f_limited_FOV;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      f_limited_FOV = false;
   }
};

/**
*\purpose  Test checks if function sets priority to 0 (not a candidate) when object status lower than F360_OBJECT_STATUS_NEW_UPDATED (lower than 3).
           Priority value 0.0 means that object is not a downselection candidate.
*\req    NA
**/
TEST(Calc_Track_Priority, Check_if_set_max_priority_when_object_has_status_lower_than_F360_OBJECT_STATUS_NEW_UPDATED)
{
   /** \precond
   */
   object_tracks[0].status = F360_OBJECT_STATUS_NEW_COASTED;
   tracker_info.elapsed_time_s = 0.001F;
   float32_t priority = 0.0F;

   /** \action
      Call Calc_Track_Priority
   */
   priority = Calc_Track_Priority(host, calib, tracker_info, overall_confidence_exclusion_box, sensors, f_limited_FOV, object_tracks[0]);

   /** \result
   */
   CHECK_EQUAL(0.0F, priority);
}

/**
*\purpose  Test checks if function sets priority to 0 (not a candidate) when object status is equal F360_OBJECT_STATUS_UPDATED and
           reduced_status == F360_OBJECT_STATUS_INVALID and confidenceLevel < calib->re_down_select_confidence_thresh.
           Priority value 0.0 means that object is not a downselection candidate.
*\req    NA
**/
TEST(Calc_Track_Priority, if_set_max_priority)
{
   /** \precond
   */
   tracker_info.elapsed_time_s = 0.001F;
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].reduced_status = F360_OBJECT_STATUS_INVALID;
   object_tracks[0].confidenceLevel = 0.3F;
   float32_t priority = 0.0F;

   /** \action
      Call Calc_Track_Priority
   */
   priority = Calc_Track_Priority(host, calib, tracker_info, overall_confidence_exclusion_box, sensors, f_limited_FOV, object_tracks[0]);

   /** \result
   */
   CHECK_EQUAL(0.0F, priority);
}

/**
*\purpose  Test checks if function sets priority to 0 (not a candidate) when object status is equal F360_OBJECT_STATUS_COASTED and
           reduced_status == F360_OBJECT_STATUS_INVALID.
           Priority value 0.0 means that object is not a downselection candidate.
*\req    NA
**/
TEST(Calc_Track_Priority, Check_if_not_downselected_before_coasting_set_max_priority)
{
   /** \precond
   */
   tracker_info.elapsed_time_s = 0.001F;
   object_tracks[0].status = F360_OBJECT_STATUS_COASTED;
   object_tracks[0].reduced_status = F360_OBJECT_STATUS_INVALID;
   float32_t priority = 0.0F;

   /** \action
      Call Calc_Track_Priority
   */
   priority = Calc_Track_Priority(host, calib, tracker_info, overall_confidence_exclusion_box, sensors, f_limited_FOV, object_tracks[0]);

   /** \result
   */
   CHECK_EQUAL(0.0F, priority);
}

/**
*\purpose  Test checks if function sets priority to 0 (not a candidate) when object f_hide_occluded_track_behind_host is set to True.
           Priority value 0.0 means that object is not a downselection candidate.
*\req    NA
**/
TEST(Calc_Track_Priority, Check_if_hide_occluded_track_behind_host_set_max_priority)
{
   /** \precond
   */
   tracker_info.elapsed_time_s = 3.0F;
   object_tracks[0].confidenceLevel = 1.0F;
   object_tracks[0].conf_overall = CONF3_HIGH;
   object_tracks[0].f_hide_occluded_track_behind_host = true;
   float32_t priority = 0.0F;

   /** \action
      Call Calc_Track_Priority
   */
   priority = Calc_Track_Priority(host, calib, tracker_info, overall_confidence_exclusion_box, sensors, f_limited_FOV, object_tracks[0]);

   /** \result
   */
   CHECK_EQUAL(0.0F, priority);
}

/**
*\purpose  Test checks that objects with low confidence level (below threshold) are given
           priority 0 (not a candidate for downselection) unless flagged as suspectable for detection drop.
           Priority value 0.0 means that object is not a downselection candidate.
*\req    NA
**/
TEST(Calc_Track_Priority, Check_if_low_confidence_object_gets_max_priority)
{
   /** \precond
    * Set up an object with:
    * - Status UPDATED (to pass basic checks)
    * - Confidence level below low_confidence_level_thresh
    * - conf_overall set to CONF3_NONE (low overall confidence)
    * - f_suspectable_for_det_drop set to false (not suspected for detection drop)
    * - Valid position away from host
    */
   tracker_info.elapsed_time_s = 0.001F;
   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].reduced_status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].confidenceLevel = calib.low_confidence_level_thresh - 0.1F; // Below threshold
   object_tracks[0].conf_overall = CONF3_NONE; // Low overall confidence
   object_tracks[0].f_suspectable_for_det_drop = false; // NOT suspected for detection drop
   object_tracks[0].vcs_position.x = 20.0F;
   object_tracks[0].vcs_position.y = 10.0F;
   object_tracks[0].movable_prob = 0.6F; // Movable object
   host.dist_rear_axle_to_vcs_m = 3.0F;
   float32_t priority = 0.0F;

   /** \action
    * Call Calc_Track_Priority
    */
   priority = Calc_Track_Priority(host, calib, tracker_info, overall_confidence_exclusion_box, sensors, f_limited_FOV, object_tracks[0]);

   /** \result
    * Priority should equal 0.0 (object is not a candidate for downselection)
    */
   CHECK_EQUAL(0.0F, priority);
}

/** @}*/


/** \defgroup  Assign_Reduced_Idxs_To_Prioritized_Tracks
 *  @{
 */

/** \brief
 *  Test group for Assign_Reduced_Idxs_To_Prioritized_Tracks()
 **/
TEST_GROUP(Assign_Reduced_Idxs_To_Prioritized_Tracks)
{
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS];
   F360_Tracker_Info_T tracker_info = {};

   TEST_SETUP()
   {
      Set_Tracker_Variant(tracker_info.variant);
   }
};

/**
*\purpose  Test checks if reduced ids are assigned to tracks
*\req    NA
**/
TEST(Assign_Reduced_Idxs_To_Prioritized_Tracks, Check_if_reduced_idxs_are_assigned_to_tracks)
{
   /** \precond
   */
   int32_t candidates_ids[NUMBER_OF_OBJECT_TRACKS]{1,2,3,4,5,6,7,8,9,10};
   uint32_t ids_of_objs_sorted_by_priority[NUMBER_OF_OBJECT_TRACKS] = {3,5,7};
   uint32_t candidates_cnt = 3;
   object_tracks[4].reduced_id = 1;
   object_tracks[4].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[4].reduced_status = F360_OBJECT_STATUS_NEW_COASTED;
   object_tracks[6].reduced_id = 2;
   object_tracks[6].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[6].reduced_status = F360_OBJECT_STATUS_NEW_COASTED;
   object_tracks[8].reduced_id = 3;
   object_tracks[8].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[8].reduced_status = F360_OBJECT_STATUS_NEW_COASTED;

   /** \action
      Call Assign_Reduced_Idxs_To_Prioritized_Tracks
   */
   Assign_Reduced_Idxs_To_Prioritized_Tracks( object_tracks, tracker_info, candidates_ids, ids_of_objs_sorted_by_priority, candidates_cnt);

   /** \result
   */
   CHECK_EQUAL(object_tracks[4].reduced_status, F360_OBJECT_STATUS_UPDATED)
   CHECK_EQUAL(tracker_info.reduced_obj_ids[object_tracks[4].reduced_id - 1], 4+1)
   CHECK_EQUAL(object_tracks[6].reduced_status, F360_OBJECT_STATUS_UPDATED)
   CHECK_EQUAL(tracker_info.reduced_obj_ids[object_tracks[6].reduced_id - 1], 6 + 1)
   CHECK_EQUAL(object_tracks[8].reduced_status, F360_OBJECT_STATUS_UPDATED)
   CHECK_EQUAL(tracker_info.reduced_obj_ids[object_tracks[8].reduced_id - 1], 8 + 1)
}

/**
*\purpose  Test checks if function gets obj id from tracker_info.reduced_inactive_obj_ids[]
*\req    NA
**/
TEST(Assign_Reduced_Idxs_To_Prioritized_Tracks, Check_if_get_obj_idx_from_tracker_info)
{
   /** \precond
   */
   int32_t candidates_ids[NUMBER_OF_OBJECT_TRACKS]{1,2,3,4,5,6,7,8,9,10};
   uint32_t ids_of_objs_sorted_by_priority[NUMBER_OF_OBJECT_TRACKS] = { 3,5,7 };
   uint32_t candidates_cnt = 3;

   for (unsigned int i = 0; i < NUMBER_OF_REDUCED_OBJECT_TRACKS; i++)
   {
      tracker_info.reduced_inactive_obj_ids[i] = i + 1;
   }

   object_tracks[4].reduced_id = 0;
   object_tracks[4].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[4].reduced_status = F360_OBJECT_STATUS_NEW_COASTED;
   object_tracks[6].reduced_id = 0;
   object_tracks[6].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[6].reduced_status = F360_OBJECT_STATUS_NEW_COASTED;
   object_tracks[8].reduced_id = 0;
   object_tracks[8].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[8].reduced_status = F360_OBJECT_STATUS_NEW_COASTED;

   /** \action
      Call Assign_Reduced_Idxs_To_Prioritized_Tracks
   */
   Assign_Reduced_Idxs_To_Prioritized_Tracks(object_tracks, tracker_info, candidates_ids, ids_of_objs_sorted_by_priority, candidates_cnt);

   /** \result
   */
   CHECK_EQUAL(object_tracks[4].reduced_status, F360_OBJECT_STATUS_NEW);
   CHECK_EQUAL(object_tracks[4].reduced_id, 1);
   CHECK_EQUAL(tracker_info.reduced_obj_ids[object_tracks[4].reduced_id - 1], 4 + 1);
   CHECK_EQUAL(tracker_info.reduced_inactive_obj_ids[0], 4 );
}

/**
*\purpose  Test checks if moving tracks get f_movable set correctly
*\req    NA
**/
TEST(Assign_Reduced_Idxs_To_Prioritized_Tracks, Check_f_movable_flag)
{
   /** \precond
   */
   int32_t candidates_idx[NUMBER_OF_OBJECT_TRACKS]{11,12,13,14,15};
   uint32_t processing_order_for_candidates[NUMBER_OF_OBJECT_TRACKS] = { 0,2,3,4 };
   uint32_t candidates_cnt = 4;

   for (unsigned int i = 0; i < NUMBER_OF_REDUCED_OBJECT_TRACKS; i++)
   {
      tracker_info.reduced_inactive_obj_ids[i] = i + 1;
   }

   object_tracks[11].reduced_id = 0;
   object_tracks[11].f_moving = true;
   object_tracks[11].f_moveable = false;

   object_tracks[12].reduced_id = 0;
   object_tracks[12].f_moving = true;
   object_tracks[12].f_moveable = false;

   object_tracks[13].reduced_id = 1;
   object_tracks[13].f_moving = false;
   object_tracks[13].f_moveable = true;

   object_tracks[14].reduced_id = 2;
   object_tracks[14].f_moving = false;
   object_tracks[14].f_moveable = false;

   object_tracks[15].reduced_id = 3;
   object_tracks[15].f_moving = true;
   object_tracks[15].f_moveable = true;


   /** \action
      Call Assign_Reduced_Idxs_To_Prioritized_Tracks
   */
   Assign_Reduced_Idxs_To_Prioritized_Tracks(object_tracks, tracker_info, candidates_idx, processing_order_for_candidates, candidates_cnt);

   /** \result
   */
   CHECK_EQUAL(object_tracks[11].reduced_id, 1);
   CHECK_EQUAL(object_tracks[12].reduced_id, 0);
   CHECK_TRUE(object_tracks[11].f_moveable);
   CHECK_FALSE(object_tracks[12].f_moveable);
   CHECK_TRUE(object_tracks[13].f_moveable);
   CHECK_FALSE(object_tracks[14].f_moveable);
   CHECK_TRUE(object_tracks[15].f_moveable);
}

/** \defgroup  Deselect_Existing_Reduced_Tracks
 *  @{
 */

/** \brief
 *  Test group for Deselect_Existing_Reduced_Tracks()
 **/
TEST_GROUP(Deselect_Existing_Reduced_Tracks)
{
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS];
   F360_Tracker_Info_T tracker_info;

   TEST_SETUP()
   {
      Set_Tracker_Variant(tracker_info.variant);
   }
};

/**
*\purpose  Test checks if function sets reduced status to F360_OBJECT_STATUS_INVALID and reduced id to zero.
*\req    NA
**/
TEST(Deselect_Existing_Reduced_Tracks, Check_if_set_reduced_status_and_reduced_id_on_invalid)
{
   /** \precond
   */
   int32_t candidates_ids[NUMBER_OF_OBJECT_TRACKS]{ 1,2,3,4,5,6,7,8,9,10 };
   uint32_t ids_of_objs_sorted_by_priority[NUMBER_OF_OBJECT_TRACKS]{};
   ids_of_objs_sorted_by_priority[500] = 4;
   uint32_t candidates_cnt = 501;
   tracker_info.reduced_num_active_objs = 1;

   for (unsigned int i = 0; i < NUMBER_OF_REDUCED_OBJECT_TRACKS; i++)
   {
      tracker_info.reduced_inactive_obj_ids[i] = i + 1;
   }

   object_tracks[5].reduced_id = 5;
   object_tracks[5].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[5].reduced_status = F360_OBJECT_STATUS_NEW_COASTED;
   tracker_info.reduced_active_obj_ids[5] = 5;

   /** \action
      Call Deselect_Existing_Reduced_Tracks
   */
   Deselect_Existing_Reduced_Tracks(object_tracks, tracker_info, candidates_ids, ids_of_objs_sorted_by_priority, candidates_cnt);

   /** \result
   */
   CHECK_EQUAL(object_tracks[5].reduced_status, F360_OBJECT_STATUS_INVALID);
   CHECK_EQUAL(object_tracks[5].reduced_id, 0);
   CHECK_EQUAL(candidates_cnt, NUMBER_OF_REDUCED_OBJECT_TRACKS);
}

/** \defgroup  Select_Obj_Tracks_to_Downselect
 *  @{
 */

/** \brief
 *  Test group for Select_Obj_Tracks_to_Downselect()
 **/
TEST_GROUP(Select_Obj_Tracks_to_Downselect)
{
   F360_Host_T host;
   F360_Calibrations_T calib = {};
   F360_Tracker_Info_T tracker_info;
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS];
   Static_Env_Poly_T stat_env_poly[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      Set_Tracker_Variant(tracker_info.variant);
   }
};

/**
*\purpose  Test checks if function issues candidates_ids and priorities.
*\req    NA
**/
TEST(Select_Obj_Tracks_to_Downselect, Check_if_issue_priorities_and_candidates)
{
   /** \precond
   */
   float32_t priorities[NUMBER_OF_OBJECT_TRACKS]{};
   int32_t candidates_ids[NUMBER_OF_OBJECT_TRACKS]{};
   uint32_t candidates_cnt = 0;
   tracker_info.elapsed_time_s = 0.001F;
   tracker_info.num_active_objs = 1;

   for (unsigned int i = 0; i < NUMBER_OF_OBJECT_TRACKS; i++)
   {
      tracker_info.active_obj_ids[i] = i + 1;
   }

   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].reduced_status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].confidenceLevel = 0.65F;
   object_tracks[0].conf_overall = CONF3_HIGH;
   object_tracks[0].vcs_position.x = 15.0F;
   object_tracks[0].vcs_position.y = 9.0F;
   object_tracks[0].vcs_velocity.longitudinal = 20.0F;
   object_tracks[0].vcs_velocity.lateral = 2.0F;
   object_tracks[0].movable_prob = 1.0F;
   object_tracks[0].priority = 0.8F; // Set priority so Calc_Track_Priority passes it through
   object_tracks[0].conf_overall = CONF3_HIGH;
   host.dist_rear_axle_to_vcs_m = 3.0F;
   host.vcs_speed = 10.0F;
   host.vcs_sideslip = 0.01F;

   /** \action
      Call Select_Obj_Tracks_to_Downselect
   */
   Select_Obj_Tracks_to_Downselect(host, stat_env_poly, sensors, tracker_info, calib, object_tracks, priorities, candidates_ids, candidates_cnt);

   /** \result
   */
   CHECK_TRUE(priorities[0] > 0);
   CHECK_TRUE(candidates_cnt > 0 );
}

/**
*\purpose   Test checks if function doesn't select invalid tracks.
*\req    NA
**/
TEST(Select_Obj_Tracks_to_Downselect, Check_if_function_doesnt_select_invalid_tracks)
{
   /** \precond
   */
   float32_t priorities[NUMBER_OF_OBJECT_TRACKS]{};
   int32_t candidates_ids[NUMBER_OF_OBJECT_TRACKS]{};
   uint32_t candidates_cnt = 0;
   tracker_info.elapsed_time_s = 0.001F;
   tracker_info.num_active_objs = 1;
   tracker_info.reduced_num_active_objs = 1;

   for (unsigned int i = 0; i < NUMBER_OF_OBJECT_TRACKS; i++)
   {
      tracker_info.active_obj_ids[i] = i + 1;
   }

   object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].reduced_status = F360_OBJECT_STATUS_UPDATED;
   object_tracks[0].confidenceLevel = 0.5F;
   object_tracks[0].reduced_id = 10;
   object_tracks[0].vcs_position.x = 1000000.0F;
   object_tracks[0].vcs_position.y = 1000000.0F;
   object_tracks[0].vcs_velocity.longitudinal = 20.0F;
   object_tracks[0].vcs_velocity.lateral = 2.0F;
   object_tracks[0].movable_prob = 0.0F;
   host.dist_rear_axle_to_vcs_m = 3.0F;
   host.vcs_speed = 10.0F;
   host.vcs_sideslip = 0.01F;
   tracker_info.reduced_active_obj_ids[0] = 10;

   /** \action
      Call Select_Obj_Tracks_to_Downselect
   */
   Select_Obj_Tracks_to_Downselect(host, stat_env_poly, sensors, tracker_info, calib, object_tracks, priorities, candidates_ids, candidates_cnt);

   /** \result
   */
   CHECK_TRUE(object_tracks[0].reduced_status == F360_OBJECT_STATUS_INVALID);
   CHECK_TRUE(object_tracks[0].reduced_id == 0);
}

/** \defgroup  Is_Unreliable_Low_Conf_Moveable_Track
 *  @{
 */

/** \brief
 *  Test group for Is_Unreliable_Low_Conf_Moveable_Track()
 **/
TEST_GROUP(Is_Unreliable_Low_Conf_Moveable_Track)
{
   F360_Host_T host;
   F360_Calibrations_T calib = {};
   F360_Object_Track_T object_tracks[5];
   Static_Env_Poly_T stat_env_poly[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   BoundingBox overall_confidence_exclusion_box{};
   bool f_recently_entered_FOV;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      host.dist_rear_axle_to_vcs_m = 4.0F;
      overall_confidence_exclusion_box = Define_Overall_Confidence_Exclusion_Box_Around_Host(stat_env_poly, calib, host.dist_rear_axle_to_vcs_m * 0.5F);
      f_recently_entered_FOV = false;

      // Initialize Objects as moveable
      for (int32_t i = 0; i < 5; i++)
      {
         object_tracks[i].movable_prob = 1.0F;
      }

   }
};

/**
*\purpose  Check if a low confidence track in the zone of interest is flagged unreliable
*\req    NA
**/
TEST(Is_Unreliable_Low_Conf_Moveable_Track, Check_if_track_is_unreliable)
{
   /** \precond

   */

   host.vcs_speed = 10.0F;

   // Track straight in front
   object_tracks[0].vcs_position.x = 10.0F;
   object_tracks[0].vcs_position.y = 0.0F;
   object_tracks[0].vcs_velocity.longitudinal = 0.0F;
   object_tracks[0].conf_overall = CONF3_LOW;


   //Track at front right
   object_tracks[1].vcs_position.x = 10.0F;
   object_tracks[1].vcs_position.y = 10.0F;
   object_tracks[1].vcs_velocity.longitudinal = 0.0F;
   object_tracks[1].conf_overall = CONF3_LOW;

   // Track at front left
   object_tracks[2].vcs_position.x = 10.0F;
   object_tracks[2].vcs_position.y = -10.0F;
   object_tracks[2].vcs_velocity.longitudinal = 0.0F;
   object_tracks[2].conf_overall = CONF3_LOW;

   // Track at right side
   object_tracks[3].vcs_position.x = 0.0F;
   object_tracks[3].vcs_position.y = 10.0F;
   object_tracks[3].vcs_velocity.longitudinal = 0.0F;
   object_tracks[3].conf_overall = CONF3_LOW;

   // Track at left side
   object_tracks[4].vcs_position.x = 0.0F;
   object_tracks[4].vcs_position.y = -10.0F;
   object_tracks[4].vcs_velocity.longitudinal = 0.0F;
   object_tracks[4].conf_overall = CONF3_LOW;

   host.speed = 10.0F;

   for (int32_t i = 0; i < 5; i++)
   {
      /** \action
         Call Is_Unreliable_Low_Conf_Moveable_Track for each track
      */
      const bool result = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[i], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);

      /** \result
      */
      CHECK_TRUE(result);
   }
}

/**
*\purpose  Check if a track is not flagged unreliable when:
   A track:
   - is in the zone of interest,
   - has medium confidence,
   - has expected negative heading and negative heading difference for CTA scenario
   AND
   - host speed is below CTA speed threshold
*\req    NA
**/
TEST(Is_Unreliable_Low_Conf_Moveable_Track, Check_if_track_is_unreliable_low_host_speed_and_expected_neg_heading_neg_diff_in_CTA_scenario)
{
   /** \precond

   */
   const Angle initial_heading = Angle{F360_DEG2RAD(-89.0F)};

   host.vcs_speed = 0.19F;
   object_tracks[0].vcs_heading = initial_heading;
   object_tracks[1].vcs_heading = initial_heading;
   object_tracks[2].vcs_heading = initial_heading;
   object_tracks[3].vcs_heading = initial_heading;
   object_tracks[4].vcs_heading = initial_heading;

   // Track straight in front
   object_tracks[0].vcs_position.x = 10.0F;
   object_tracks[0].vcs_position.y = 0.0F;
   object_tracks[0].vcs_velocity.longitudinal = 0.0F;
   object_tracks[0].conf_overall = CONF3_MED;


   //Track at front right
   object_tracks[1].vcs_position.x = 10.0F;
   object_tracks[1].vcs_position.y = 10.0F;
   object_tracks[1].vcs_velocity.longitudinal = 0.0F;
   object_tracks[1].conf_overall = CONF3_MED;

   // Track at front left
   object_tracks[2].vcs_position.x = 10.0F;
   object_tracks[2].vcs_position.y = -10.0F;
   object_tracks[2].vcs_velocity.longitudinal = 0.0F;
   object_tracks[2].conf_overall = CONF3_MED;

   // Track at right side
   object_tracks[3].vcs_position.x = 0.0F;
   object_tracks[3].vcs_position.y = 10.0F;
   object_tracks[3].vcs_velocity.longitudinal = 0.0F;
   object_tracks[3].conf_overall = CONF3_MED;

   // Track at left side
   object_tracks[4].vcs_position.x = 0.0F;
   object_tracks[4].vcs_position.y = -10.0F;
   object_tracks[4].vcs_velocity.longitudinal = 0.0F;
   object_tracks[4].conf_overall = CONF3_MED;

   for (int32_t i = 0; i < 5; i++)
   {
      /** \action
         Call Is_Unreliable_Low_Conf_Moveable_Track for each track
      */
      const bool result = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[i], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);

      /** \result
      */
      CHECK_FALSE(result);
   }
}

/**
*\purpose  Check if a track is not flagged unreliable when:
   A track:
   - is in the zone of interest,
   - has medium confidence,
   - has expected negative heading and positive heading difference for CTA scenario
   AND
   - host speed is below CTA speed threshold
*\req    NA
**/
TEST(Is_Unreliable_Low_Conf_Moveable_Track, Check_if_track_is_unreliable_low_host_speed_and_expected_neg_heading_pos_diff_in_CTA_scenario)
{
   /** \precond

   */
   const Angle initial_heading{ F360_DEG2RAD(-91.0F) };

   host.vcs_speed = 0.19F;
   object_tracks[0].vcs_heading = initial_heading;
   object_tracks[1].vcs_heading = initial_heading;
   object_tracks[2].vcs_heading = initial_heading;
   object_tracks[3].vcs_heading = initial_heading;
   object_tracks[4].vcs_heading = initial_heading;

   // Track straight in front
   object_tracks[0].vcs_position.x = 10.0F;
   object_tracks[0].vcs_position.y = 0.0F;
   object_tracks[0].vcs_velocity.longitudinal = 0.0F;
   object_tracks[0].conf_overall = CONF3_MED;

   //Track at front right
   object_tracks[1].vcs_position.x = 10.0F;
   object_tracks[1].vcs_position.y = 10.0F;
   object_tracks[1].vcs_velocity.longitudinal = 0.0F;
   object_tracks[1].conf_overall = CONF3_MED;

   // Track at front left
   object_tracks[2].vcs_position.x = 10.0F;
   object_tracks[2].vcs_position.y = -10.0F;
   object_tracks[2].vcs_velocity.longitudinal = 0.0F;
   object_tracks[2].conf_overall = CONF3_MED;

   // Track at right side
   object_tracks[3].vcs_position.x = 0.0F;
   object_tracks[3].vcs_position.y = 10.0F;
   object_tracks[3].vcs_velocity.longitudinal = 0.0F;
   object_tracks[3].conf_overall = CONF3_MED;

   // Track at left side
   object_tracks[4].vcs_position.x = 0.0F;
   object_tracks[4].vcs_position.y = -10.0F;
   object_tracks[4].vcs_velocity.longitudinal = 0.0F;
   object_tracks[4].conf_overall = CONF3_MED;

   for (int32_t i = 0; i < 5; i++)
   {
       /** \action
          Call Is_Unreliable_Low_Conf_Moveable_Track for each track
       */
       const bool result = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[i], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);

       /** \result
       */
       CHECK_FALSE(result);
   }
}

/**
*\purpose  Check if a track is not flagged unreliable when:
   A track:
   - is in the zone of interest,
   - has medium confidence,
   - has expected positive heading and negative heading difference for CTA scenario
   AND
   - host speed is below CTA speed threshold
*\req    NA
**/
TEST(Is_Unreliable_Low_Conf_Moveable_Track, Check_if_track_is_unreliable_low_host_speed_and_expected_pos_heading_neg_diff_in_CTA_scenario)
{
   /** \precond

   */
   const Angle initial_heading{ F360_DEG2RAD(89.0F) };

   host.vcs_speed = 0.19F;
   object_tracks[0].vcs_heading = initial_heading;
   object_tracks[1].vcs_heading = initial_heading;
   object_tracks[2].vcs_heading = initial_heading;
   object_tracks[3].vcs_heading = initial_heading;
   object_tracks[4].vcs_heading = initial_heading;

   // Track straight in front
   object_tracks[0].vcs_position.x = 10.0F;
   object_tracks[0].vcs_position.y = 0.0F;
   object_tracks[0].vcs_velocity.longitudinal = 0.0F;
   object_tracks[0].conf_overall = CONF3_MED;

   //Track at front right
   object_tracks[1].vcs_position.x = 10.0F;
   object_tracks[1].vcs_position.y = 10.0F;
   object_tracks[1].vcs_velocity.longitudinal = 0.0F;
   object_tracks[1].conf_overall = CONF3_MED;

   // Track at front left
   object_tracks[2].vcs_position.x = 10.0F;
   object_tracks[2].vcs_position.y = -10.0F;
   object_tracks[2].vcs_velocity.longitudinal = 0.0F;
   object_tracks[2].conf_overall = CONF3_MED;

   // Track at right side
   object_tracks[3].vcs_position.x = 0.0F;
   object_tracks[3].vcs_position.y = 10.0F;
   object_tracks[3].vcs_velocity.longitudinal = 0.0F;
   object_tracks[3].conf_overall = CONF3_MED;

   // Track at left side
   object_tracks[4].vcs_position.x = 0.0F;
   object_tracks[4].vcs_position.y = -10.0F;
   object_tracks[4].vcs_velocity.longitudinal = 0.0F;
   object_tracks[4].conf_overall = CONF3_MED;

   for (int32_t i = 0; i < 5; i++)
   {
       /** \action
          Call Is_Unreliable_Low_Conf_Moveable_Track for each track
       */
       const bool result = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[i], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);

       /** \result
       */
       CHECK_FALSE(result);
   }
}

/**
*\purpose  Check if a track is not flagged unreliable when:
   A track:
   - is in the zone of interest,
   - has medium confidence,
   - has expected positive heading and positive heading difference for CTA scenario
   AND
   - host speed is below CTA speed threshold
*\req    NA
**/
TEST(Is_Unreliable_Low_Conf_Moveable_Track, Check_if_track_is_unreliable_low_host_speed_and_expected_pos_heading_pos_diff_in_CTA_scenario)
{
   /** \precond

   */
   const Angle initial_heading{ F360_DEG2RAD(91.0F) };

   host.vcs_speed = 0.19F;
   object_tracks[0].vcs_heading = initial_heading;
   object_tracks[1].vcs_heading = initial_heading;
   object_tracks[2].vcs_heading = initial_heading;
   object_tracks[3].vcs_heading = initial_heading;
   object_tracks[4].vcs_heading = initial_heading;

   // Track straight in front
   object_tracks[0].vcs_position.x = 10.0F;
   object_tracks[0].vcs_position.y = 0.0F;
   object_tracks[0].vcs_velocity.longitudinal = 0.0F;
   object_tracks[0].conf_overall = CONF3_MED;

   //Track at front right
   object_tracks[1].vcs_position.x = 10.0F;
   object_tracks[1].vcs_position.y = 10.0F;
   object_tracks[1].vcs_velocity.longitudinal = 0.0F;
   object_tracks[1].conf_overall = CONF3_MED;

   // Track at front left
   object_tracks[2].vcs_position.x = 10.0F;
   object_tracks[2].vcs_position.y = -10.0F;
   object_tracks[2].vcs_velocity.longitudinal = 0.0F;
   object_tracks[2].conf_overall = CONF3_MED;

   // Track at right side
   object_tracks[3].vcs_position.x = 0.0F;
   object_tracks[3].vcs_position.y = 10.0F;
   object_tracks[3].vcs_velocity.longitudinal = 0.0F;
   object_tracks[3].conf_overall = CONF3_MED;

   // Track at left side
   object_tracks[4].vcs_position.x = 0.0F;
   object_tracks[4].vcs_position.y = -10.0F;
   object_tracks[4].vcs_velocity.longitudinal = 0.0F;
   object_tracks[4].conf_overall = CONF3_MED;

   for (int32_t i = 0; i < 5; i++)
   {
       /** \action
          Call Is_Unreliable_Low_Conf_Moveable_Track for each track
       */
       const bool result = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[i], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);

       /** \result
       */
       CHECK_FALSE(result);
   }
}

/**
*\purpose  Boundary: abs heading diff equals max allowed (CTA)
*\req    NA
**/
TEST(Is_Unreliable_Low_Conf_Moveable_Track, Boundary_abs_heading_diff_equals_threshold_in_CTA_scenario)
{
   /** \precond
    * Single object with `conf_overall = CONF3_MED` and low host speed.
    * Heading set so that abs(obj_heading_difference) equals CTA threshold.
    * TTC kept high (longitudinal vel = 0) and track set mature (age >= thr, CTCA)
    */

   // We neutralize TTC and maturity predicates to validate only the boundary condition on heading difference.
   // Expected CTA heading is perpendicular (+/-90 deg). Set object heading so that
   // abs(obj_heading_difference) == k_low_conf_max_allowed_abs_heading_difference_in_cta_scenarios.
   const float32_t threshold_deg = 35.0F; // equal to current k_low_conf_max_allowed_abs_heading_difference_in_cta_scenarios
   const Angle heading_pos{ F360_DEG2RAD(90.0F - threshold_deg) };   // +55 deg
   const Angle heading_neg{ F360_DEG2RAD(-90.0F + threshold_deg) };  // -55 deg

   host.vcs_speed = 0.19F; // below CTA speed threshold
   object_tracks[0].vcs_heading = heading_pos;

   // Positions within zone of interest, medium confidence required
   // Make TTC high (no approach), and track mature to avoid unreliability even if other predicates are true
   object_tracks[0].conf_overall = CONF3_MED;
   object_tracks[0].vcs_velocity.longitudinal = 0.0F; // high TTC
   object_tracks[0].time_since_initialization = calib.k_low_conf_unreliability_age_thr; // mature
   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[0].vcs_position.x = 10.0F; object_tracks[0].vcs_position.y = 0.0F;

   /** \action
    * Call Is_Unreliable_Low_Conf_Moveable_Track for each track
    */
   const bool result = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[0], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);

   /** \result
    * Expect the object to not be unreliable
    */
   CHECK_FALSE(result);
}

/**
*\purpose  Boundary: abs heading diff just above max allowed (CTA)
*\req    NA
**/
TEST(Is_Unreliable_Low_Conf_Moveable_Track, Boundary_abs_heading_diff_just_over_threshold_in_CTA_scenario)
{
   /** \precond
    * Single object with `conf_overall = CONF3_MED` and low host speed.
    * Heading set so that abs(obj_heading_difference) is just above CTA threshold.
    * TTC made low (approaching longitudinal vel) and track set not mature (age < thr, CCA)
    * to exercise unreliability when CTA condition is false and other predicates apply.
    */

   // Predicates for TTC and maturity are deliberately configured to trigger unreliability when the
   // heading difference exceeds the max allowed threshold.
   const float32_t threshold_plus_deg = 35.1F; // just over threshold
   const Angle heading_pos{ F360_DEG2RAD(90.0F - threshold_plus_deg) };   // ~54.9 deg
   const Angle heading_neg{ F360_DEG2RAD(-90.0F + threshold_plus_deg) };  // ~-54.9 deg

   host.vcs_speed = 0.19F; // below CTA speed threshold
   object_tracks[0].vcs_heading = heading_pos;

   object_tracks[0].conf_overall = CONF3_MED;
   object_tracks[0].vcs_velocity.longitudinal = -5.0F; // approaching -> low TTC
   object_tracks[0].time_since_initialization = 0.0F;   // not mature
   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA; // not CTCA
   object_tracks[0].vcs_position.x = 10.0F; object_tracks[0].vcs_position.y = 0.0F;

   /** \action
    * Call Is_Unreliable_Low_Conf_Moveable_Track for each track
    */
   const bool result = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[0], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);

   /** \result
    * Expect the object to be unreliable
    */
   CHECK_TRUE(result);
}

/**
*\purpose  Check if a track is flagged unreliable when:
   A track:
   - is in the zone of interest,
   - has medium confidence
   - has expected negative heading for CTA scenario
   AND
   - host speed is above CTA speed threhsold
*\req    NA
**/
TEST(Is_Unreliable_Low_Conf_Moveable_Track, Check_if_track_is_unreliable_host_is_moving_and_expected_neg_heading_in_CTA_scenario)
{
   /** \precond

   */
   const Angle initial_heading{ F360_DEG2RAD(-90.0F) };

   host.vcs_speed = 2.0F;
   object_tracks[0].vcs_heading = initial_heading;
   object_tracks[1].vcs_heading = initial_heading;
   object_tracks[2].vcs_heading = initial_heading;
   object_tracks[3].vcs_heading = initial_heading;
   object_tracks[4].vcs_heading = initial_heading;

   // Track straight in front
   object_tracks[0].vcs_position.x = 10.0F;
   object_tracks[0].vcs_position.y = 0.0F;
   object_tracks[0].vcs_velocity.longitudinal = 0.0F;
   object_tracks[0].conf_overall = CONF3_MED;


   //Track at front right
   object_tracks[1].vcs_position.x = 10.0F;
   object_tracks[1].vcs_position.y = 10.0F;
   object_tracks[1].vcs_velocity.longitudinal = 0.0F;
   object_tracks[1].conf_overall = CONF3_MED;

   // Track at front left
   object_tracks[2].vcs_position.x = 10.0F;
   object_tracks[2].vcs_position.y = -10.0F;
   object_tracks[2].vcs_velocity.longitudinal = 0.0F;
   object_tracks[2].conf_overall = CONF3_MED;

   // Track at right side
   object_tracks[3].vcs_position.x = 0.0F;
   object_tracks[3].vcs_position.y = 10.0F;
   object_tracks[3].vcs_velocity.longitudinal = 0.0F;
   object_tracks[3].conf_overall = CONF3_MED;

   // Track at left side
   object_tracks[4].vcs_position.x = 0.0F;
   object_tracks[4].vcs_position.y = -10.0F;
   object_tracks[4].vcs_velocity.longitudinal = 0.0F;
   object_tracks[4].conf_overall = CONF3_MED;

   for (int32_t i = 0; i < 5; i++)
   {
      /** \action
         Call Is_Unreliable_Low_Conf_Moveable_Track for each track
      */
      const bool result = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[i], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);

      /** \result
      */
      CHECK_TRUE(result);
   }
}

/**
*\purpose  Check if a track is flagged unreliable when:
   A track:
   - is in the zone of interest,
   - has medium confidence
   - has expected positive heading for CTA scenario
   AND
   - host speed is above CTA speed threhsold
*\req    NA
**/
TEST(Is_Unreliable_Low_Conf_Moveable_Track, Check_if_track_is_unreliable_host_is_moving_and_expected_pos_heading_in_CTA_scenario)
{
   /** \precond

   */
   const Angle initial_heading{ F360_DEG2RAD(90.0F) };

   host.vcs_speed = 2.0F;
   object_tracks[0].vcs_heading = initial_heading;
   object_tracks[1].vcs_heading = initial_heading;
   object_tracks[2].vcs_heading = initial_heading;
   object_tracks[3].vcs_heading = initial_heading;
   object_tracks[4].vcs_heading = initial_heading;

   // Track straight in front
   object_tracks[0].vcs_position.x = 10.0F;
   object_tracks[0].vcs_position.y = 0.0F;
   object_tracks[0].vcs_velocity.longitudinal = 0.0F;
   object_tracks[0].conf_overall = CONF3_MED;


   //Track at front right
   object_tracks[1].vcs_position.x = 10.0F;
   object_tracks[1].vcs_position.y = 10.0F;
   object_tracks[1].vcs_velocity.longitudinal = 0.0F;
   object_tracks[1].conf_overall = CONF3_MED;

   // Track at front left
   object_tracks[2].vcs_position.x = 10.0F;
   object_tracks[2].vcs_position.y = -10.0F;
   object_tracks[2].vcs_velocity.longitudinal = 0.0F;
   object_tracks[2].conf_overall = CONF3_MED;

   // Track at right side
   object_tracks[3].vcs_position.x = 0.0F;
   object_tracks[3].vcs_position.y = 10.0F;
   object_tracks[3].vcs_velocity.longitudinal = 0.0F;
   object_tracks[3].conf_overall = CONF3_MED;

   // Track at left side
   object_tracks[4].vcs_position.x = 0.0F;
   object_tracks[4].vcs_position.y = -10.0F;
   object_tracks[4].vcs_velocity.longitudinal = 0.0F;
   object_tracks[4].conf_overall = CONF3_MED;

   for (int32_t i = 0; i < 5; i++)
   {
       /** \action
          Call Is_Unreliable_Low_Conf_Moveable_Track for each track
       */
       const bool result = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[i], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);

       /** \result
       */
       CHECK_TRUE(result);
   }
}

/**
*\purpose  Check if a track is not flagged unreliable when:
  A track
  - is in the zone of interest
  - has medium confidence
  - has heading below threshold used in CTA scenarios
   AND
  - host speed is below CTA speed threshold
*\req    NA
**/
TEST(Is_Unreliable_Low_Conf_Moveable_Track, Check_if_track_is_unreliable_host_not_moving_and_obj_heading_outside_CTA_scenario)
{
   /** \precond

   */
   const Angle initial_heading{ F360_DEG2RAD(45.0F) };

   host.vcs_speed = 0.0F;
   object_tracks[0].vcs_heading = initial_heading;
   object_tracks[1].vcs_heading = initial_heading;
   object_tracks[2].vcs_heading = initial_heading;
   object_tracks[3].vcs_heading = initial_heading;
   object_tracks[4].vcs_heading = initial_heading;

   // Track straight in front
   object_tracks[0].vcs_position.x = 10.0F;
   object_tracks[0].vcs_position.y = 0.0F;
   object_tracks[0].vcs_velocity.longitudinal = 0.0F;
   object_tracks[0].conf_overall = CONF3_MED;


   //Track at front right
   object_tracks[1].vcs_position.x = 10.0F;
   object_tracks[1].vcs_position.y = 10.0F;
   object_tracks[1].vcs_velocity.longitudinal = 0.0F;
   object_tracks[1].conf_overall = CONF3_MED;

   // Track at front left
   object_tracks[2].vcs_position.x = 10.0F;
   object_tracks[2].vcs_position.y = -10.0F;
   object_tracks[2].vcs_velocity.longitudinal = 0.0F;
   object_tracks[2].conf_overall = CONF3_MED;

   // Track at right side
   object_tracks[3].vcs_position.x = 0.0F;
   object_tracks[3].vcs_position.y = 10.0F;
   object_tracks[3].vcs_velocity.longitudinal = 0.0F;
   object_tracks[3].conf_overall = CONF3_MED;

   // Track at left side
   object_tracks[4].vcs_position.x = 0.0F;
   object_tracks[4].vcs_position.y = -10.0F;
   object_tracks[4].vcs_velocity.longitudinal = 0.0F;
   object_tracks[4].conf_overall = CONF3_MED;

   for (int32_t i = 0; i < 5; i++)
   {
      /** \action
         Call Is_Unreliable_Low_Conf_Moveable_Track for each track
      */
      const bool result = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[i], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);

      /** \result
      */
      CHECK_FALSE(result);
   }
}


/**
*\purpose  Check that a track outside the zone of interest or with high confidence is not flagged unreliable
*\req    NA
**/
TEST(Is_Unreliable_Low_Conf_Moveable_Track, Check_if_track_is_not_unreliable)
{
   /** \precond
   */
   // Track straight in rear
   object_tracks[0].vcs_position.x = -10.0F;
   object_tracks[0].vcs_position.y = 0.0F;
   object_tracks[0].vcs_velocity.longitudinal = 0.0F;
   object_tracks[0].conf_overall = CONF3_LOW;

   //Track at rear right
   object_tracks[1].vcs_position.x = -10.0F;
   object_tracks[1].vcs_position.y = 5.0F;
   object_tracks[1].vcs_velocity.longitudinal = 0.0F;
   object_tracks[1].conf_overall = CONF3_LOW;

   // Track at rear left
   object_tracks[2].vcs_position.x = -10.0F;
   object_tracks[2].vcs_position.y = -5.0F;
   object_tracks[2].vcs_velocity.longitudinal = 0.0F;
   object_tracks[2].conf_overall = CONF3_LOW;

   // Track with high confidence
   object_tracks[3].vcs_position.x = 10.0F;
   object_tracks[3].vcs_position.y = 0.0F;
   object_tracks[3].vcs_velocity.longitudinal = 0.0F;
   object_tracks[3].conf_overall = CONF3_HIGH;

   host.speed = 10.0F;

   for (int32_t i = 0; i < 4; i++)
   {
      /** \action
         Call Is_Unreliable_Low_Conf_Moveable_Track for each track
      */
      const bool result = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[i], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);

      /** \result
      */
      CHECK_FALSE(result);
   }
}

/**
*\purpose  Ensure the function does not break down when TTX is infinite
*\req    NA
**/
TEST(Is_Unreliable_Low_Conf_Moveable_Track, Check_infinite_TTX_behavior)
{
   /** \precond
   */
   // Track straight in rear
   object_tracks[0].vcs_position.x = -10.0F;
   object_tracks[0].vcs_position.y = 0.0F;
   object_tracks[0].vcs_velocity.longitudinal = 0.0F;
   object_tracks[0].conf_overall = CONF3_LOW;

   host.speed = 0.0F;

   /** \action
      Call Is_Unreliable_Low_Conf_Moveable_Track for each track
   */
   const bool result = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[0], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);

   /** \result
   */
   CHECK_FALSE(result);
}

/**
*\purpose  Don't hide objects in the exclusion zone
*\req    NA
**/
TEST(Is_Unreliable_Low_Conf_Moveable_Track, Exclusion_Zone_Check)
{
   /** \precond
   */
   object_tracks[0].vcs_position.x = 5.0F;
   object_tracks[0].vcs_position.y = 0.0F;
   object_tracks[0].vcs_velocity.longitudinal = -1.0F;
   object_tracks[0].conf_overall = CONF3_LOW;

   object_tracks[1].vcs_position.x = -5.0F;
   object_tracks[1].vcs_position.y = 0.0F;
   object_tracks[1].vcs_velocity.longitudinal = 1.0F;
   object_tracks[1].conf_overall = CONF3_LOW;

   object_tracks[2].vcs_position.x = 1.0F;
   object_tracks[2].vcs_position.y = 4.0F;
   object_tracks[2].vcs_velocity.longitudinal = -1.0F;
   object_tracks[2].conf_overall = CONF3_LOW;

   object_tracks[3].vcs_position.x = 1.0F;
   object_tracks[3].vcs_position.y = -4.0F;
   object_tracks[3].vcs_velocity.longitudinal = -1.0F;
   object_tracks[3].conf_overall = CONF3_LOW;

   host.speed = 0.0F;

   /** \action
      Call Is_Unreliable_Low_Conf_Moveable_Track for each track
   */
   const bool result1 = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[0], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);
   const bool result2 = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[1], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);
   const bool result3 = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[2], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);
   const bool result4 = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[3], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);

   /** \result
   */
   CHECK_FALSE(result1);
   CHECK_FALSE(result2);
   CHECK_FALSE(result3);
   CHECK_FALSE(result4);
}

/**
*\purpose  Check if object is unreliable due to being CTCA and having high heading
*\req    NA
**/
TEST(Is_Unreliable_Low_Conf_Moveable_Track, Exclude_Exclusion_Zone_Check_When_Object_Is_CTCA_And_High_Heading)
{
   /** \precond
   * CTCA object with high (higher than 30 deg) heading, low overall confidence
   */
   object_tracks[0].vcs_position.x = 5.0F;
   object_tracks[0].vcs_position.y = 0.0F;
   object_tracks[0].vcs_velocity.longitudinal = -1.0F;
   object_tracks[0].conf_overall = CONF3_LOW;
   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[0].vcs_heading = Angle{ F360_DEG2RAD(31.0F) };

   host.speed = 0.0F;

   /** \action
      Call Is_Unreliable_Low_Conf_Moveable_Track for each track
   */
   const bool result = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[0], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);

   /** \result
   * Object is unreliable
   */
   CHECK_TRUE(result);
}

/**
*\purpose  Check if object is reliable due to being CCA and having high heading
*\req    NA
**/
TEST(Is_Unreliable_Low_Conf_Moveable_Track, Exclusion_Zone_Check_When_Object_Is_CCA_And_High_Heading)
{
   /** \precond
   * CCA object with high (higher than 30 deg) heading, low overall confidence
   */
   object_tracks[0].vcs_position.x = 5.0F;
   object_tracks[0].vcs_position.y = 0.0F;
   object_tracks[0].vcs_velocity.longitudinal = -1.0F;
   object_tracks[0].conf_overall = CONF3_LOW;
   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[0].vcs_heading = Angle{ F360_DEG2RAD(31.0F) };

   host.speed = 0.0F;

   /** \action
      Call Is_Unreliable_Low_Conf_Moveable_Track for each track
   */
   const bool result = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[0], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);

   /** \result
   * Object is reliable
   */
   CHECK_FALSE(result);
}

/**
*\purpose  Check if object is reliable due to being CCA and having low heading
*\req    NA
**/
TEST(Is_Unreliable_Low_Conf_Moveable_Track, Exclusion_Zone_Check_When_Object_Is_CCA_And_Low_Heading)
{
   /** \precond
   * CCA object with low (less than 30 deg) heading, low overall confidence
   */
   object_tracks[0].vcs_position.x = 5.0F;
   object_tracks[0].vcs_position.y = 0.0F;
   object_tracks[0].vcs_velocity.longitudinal = -1.0F;
   object_tracks[0].conf_overall = CONF3_LOW;
   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[0].vcs_heading = Angle{ F360_DEG2RAD(29.0F) };

   host.speed = 0.0F;

   /** \action
      Call Is_Unreliable_Low_Conf_Moveable_Track for each track
   */
   const bool result = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[0], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);

   /** \result
   * Object is reliable
   */
   CHECK_FALSE(result);
}

/**
*\purpose  Check if object is reliable due to being CTCA and having low heading
*\req    NA
**/
TEST(Is_Unreliable_Low_Conf_Moveable_Track, Exclude_Exclusion_Zone_Check_When_Object_Is_CTCA_And_Low_Heading)
{
   /** \precond
   * CTCA object with low (less than 30 deg) heading, low overall confidence
   */
   object_tracks[0].vcs_position.x = 5.0F;
   object_tracks[0].vcs_position.y = 0.0F;
   object_tracks[0].vcs_velocity.longitudinal = -1.0F;
   object_tracks[0].conf_overall = CONF3_LOW;
   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[0].vcs_heading = Angle{ F360_DEG2RAD(29.0F) };

   host.speed = 0.0F;

   /** \action
      Call Is_Unreliable_Low_Conf_Moveable_Track for each track
   */
   const bool result = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[0], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);

   /** \result
   * Object is reliable
   */
   CHECK_FALSE(result);
}

/**
*\purpose  Ensure the function checks if the track is unreliable when f_moveable = false
*\req    NA
**/
TEST(Is_Unreliable_Low_Conf_Moveable_Track, Check_if_unreliable_when_non_moveable)
{
   /** \precond
   */
   // Set to non_moveable
   object_tracks[0].movable_prob = 0.0F;

   host.vcs_speed = 10.0F;

   // Track straight in front
   object_tracks[0].vcs_position.x = 10.0F;
   object_tracks[0].vcs_position.y = 0.0F;
   object_tracks[0].vcs_velocity.longitudinal = 0.0F;
   object_tracks[0].conf_overall = CONF3_LOW;

   /** \action
      Call Is_Unreliable_Low_Conf_Moveable_Track for each track
   */
   const bool result = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[0], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);

   /** \result
   */
   CHECK_FALSE(result);
}
/**
*\purpose  Ensure the function checks if the track is unreliable when given different time_since_initialization, filter type and position
*\req    NA
**/
TEST(Is_Unreliable_Low_Conf_Moveable_Track, Check_if_unreliable_when_new_CCA_behind)
{
   /** \precond
    * Object 1 old CTCA track behind host
    * Object 2 old CTCA track in front of host, other conditions to be unreliable are met
   */


   // Old CTCA track in behind
   object_tracks[0].vcs_position.x = -5.0F;
   object_tracks[0].vcs_position.y = 7.0F;
   object_tracks[0].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[0].time_since_initialization = 6.0F;

   // Old CTCA track in front, all other conditions met
   object_tracks[1].vcs_position.x = 10.0F;
   object_tracks[1].vcs_position.y = 7.0F;
   object_tracks[1].vcs_velocity.longitudinal = -5.0F;
   object_tracks[1].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   object_tracks[1].time_since_initialization = 6.0F;
   object_tracks[1].conf_overall = CONF3_LOW;

   /** \action
      Call Is_Unreliable_Low_Conf_Moveable_Track for each track
   */
   const bool result_1 = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[0], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);
   const bool result_2 = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[1], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);

   /** \result
   */
   CHECK_FALSE(result_1);
   CHECK_FALSE(result_2);
}
/**
*\purpose  Check if object is reliable because it with medium conf_overall when it has recently entered FOV
*\req    NA
**/
TEST(Is_Unreliable_Low_Conf_Moveable_Track, Recently_Entered_FOV)
{
   /** \precond
   * Object that recently entered FOV with MED conf_overall
   */
   object_tracks[0].vcs_position.x = 20.0F;
   object_tracks[0].vcs_position.y = 5.0F;
   object_tracks[0].conf_overall = CONF3_MED;
   f_recently_entered_FOV = true;

   host.speed = 10.0F;

   /** \action
      Call Is_Unreliable_Low_Conf_Moveable_Track
   */
   const bool result = Is_Unreliable_Low_Conf_Moveable_Track(object_tracks[0], host, calib, overall_confidence_exclusion_box, f_recently_entered_FOV);

   /** \result
   * Object is reliable
   */
   CHECK_FALSE(result);
}

/** @}*/

/** \defgroup  check_calibration_values_used_in_downselection
 *  @{
 */

 /** \brief
  * Test group verifies if calibration parameters used in downselection have correct values
  */
TEST_GROUP(check_calibration_values_used_in_downselection)
{
   F360_Calibrations_T calib = {};
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
   }
};


/**
*\purpose  Check if k_low_conf_max_allowed_host_speed_in_cta_scenarios has correct value
*\req    NA
**/
TEST(check_calibration_values_used_in_downselection, check_k_low_conf_max_allowed_host_speed_in_cta_scenarios)
{
   DOUBLES_EQUAL(0.2F, calib.k_low_conf_max_allowed_host_speed_in_cta_scenarios, F360_EPSILON);
}

/**
*\purpose  Check if k_low_conf_expected_abs_object_heading_vcs_in_cta_scenarios has correct value
*\req    NA
**/
TEST(check_calibration_values_used_in_downselection, check_k_low_conf_expected_abs_object_heading_vcs_in_cta_scenarios)
{
   DOUBLES_EQUAL(1.57079633F, calib.k_low_conf_expected_abs_object_heading_vcs_in_cta_scenarios, F360_EPSILON);
}

/**
*\purpose  Check if k_low_conf_max_allowed_abs_heading_difference_in_cta_scenarios has correct value
*\req    NA
**/
TEST(check_calibration_values_used_in_downselection, check_k_low_conf_max_allowed_abs_heading_difference_in_cta_scenarios)
{
   DOUBLES_EQUAL(0.6108652F, calib.k_low_conf_max_allowed_abs_heading_difference_in_cta_scenarios, F360_EPSILON);
}
/** @}*/

/** \defgroup  define_overall_confidence_lateral_exclusion_box
 *  @{
 */

 /** \brief
  * Test group of Define_Overall_Confidence_Exclusion_Box_Around_Host . Tests verify whether
  * exclusion box lateral limits are properly determined.
  */
TEST_GROUP(define_overall_confidence_lateral_exclusion_box)
{
   Static_Env_Poly_T static_env_polys[F360_NUM_OF_STATIC_ENV_POLYS];
   const float32_t max_exclusion_box_lat_dist = 8.0F;
   F360_Calibrations_T calib = {};

   /** \setup
    * Create two valid SEPs that spans from -100m to 100m longitudinally.
    * Set lateral position of the first polynomials to minus half the distance of max exclusion box lateral distance.
    * Set lateral position of the second polynomials to half the distance of max exclusion box lateral distance.
    * Call Set function to assign the SEPs to the static environment class.
    *
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      // Left side
      static_env_polys[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      static_env_polys[0].p2 = 0.0F;
      static_env_polys[0].p1 = 0.0F;
      static_env_polys[0].p0 = -0.5F * max_exclusion_box_lat_dist;
      static_env_polys[0].lower_limit = -100.0F;
      static_env_polys[0].upper_limit = 100.0F;

      // Right side
      static_env_polys[1].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      static_env_polys[1].p2 = 0.0F;
      static_env_polys[1].p1 = 0.0F;
      static_env_polys[1].p0 = 0.5F * max_exclusion_box_lat_dist;
      static_env_polys[1].lower_limit = -100.0F;
      static_env_polys[1].upper_limit = 100.0F;
   }
};

/** \purpose
 * Purpose of this test is to verify that left limit is properly determined when there is no valid SEP on the left side of host.
 * \req
 * NA.
 */
TEST(define_overall_confidence_lateral_exclusion_box, Define_Overall_Confidence_Lateral_Exclusion_Box__Left_SEP_Not_Present)
{
   /** \precond
    * Set status of left SEP to invalid.
    */
   static_env_polys[0].status = F360_STATIC_ENV_POLY_STATUS_INVALID;

   /** \action
    * Call Define_Overall_Confidence_Exclusion_Box_Around_Host
    */
   const BoundingBox exclusion_box = Define_Overall_Confidence_Exclusion_Box_Around_Host(static_env_polys, calib, max_exclusion_box_lat_dist);

   Point exclusion_box_center = exclusion_box.Get_Center();
   float32_t left_limit = exclusion_box_center.y - 0.5*exclusion_box.Get_Width();

   /** \result
    * Check whether returned left limit is equal to -1.0F * max_exclusion_box_lat_dist
    */
   DOUBLES_EQUAL(-1.0F * max_exclusion_box_lat_dist, left_limit, F360_EPSILON);
}

/** \purpose
 * Purpose of this test is to verify that left limit is properly determined when there is a valid SEP on the left side of host.
 * \req
 * NA.
 */
TEST(define_overall_confidence_lateral_exclusion_box, Define_Overall_Confidence_Lateral_Exclusion_Box__Left_SEP_Present)
{
   /** \precond
    * In test group the following has been set up:
    * - Max lateral exclusion box distance has been set to 8.
    * - A SEP has been set up on the left side of host with a lateral position that is half the max lateral exclusion box distance.
    * - The lower and upper longitudinal limit of the SEP has been set to -100m and 100m.
    */

   /** \action
    * Call tested function
    */
   const BoundingBox exclusion_box = Define_Overall_Confidence_Exclusion_Box_Around_Host(static_env_polys, calib, max_exclusion_box_lat_dist);
   Point exclusion_box_center = exclusion_box.Get_Center();
   float32_t left_limit = exclusion_box_center.y - 0.5*exclusion_box.Get_Width();
   /** \result
    * Check whether returned left limit is equal to left SEP "p0" coefficient
    */
   DOUBLES_EQUAL(static_env_polys[0].p0, left_limit, F360_EPSILON);
}

/** \purpose
 * Purpose of this test is to verify that right limit is properly determined when there is no valid SEP on the right side of host.
 * \req
 * NA.
 */
TEST(define_overall_confidence_lateral_exclusion_box, Define_Overall_Confidence_Lateral_Exclusion_Box__Right_LSC_Not_Present)
{
   /** \precond
    * In test group the following has been set up:
    * - Max lateral exclusion box distance has been set to 8.
    * - A SEP has been set up on the right side of host with a lateral position that is half the max lateral exclusion box distance.
    * - The lower and upper longitudinal limit of the SEP has been set to -100m and 100m.
    * Set the status of right SEP to invalid
    */
   static_env_polys[1].status = F360_STATIC_ENV_POLY_STATUS_INVALID;

   /** \action
    * Call Define_Overall_Confidence_Exclusion_Box_Around_Host
    */
   const BoundingBox  exclusion_box = Define_Overall_Confidence_Exclusion_Box_Around_Host(static_env_polys, calib, max_exclusion_box_lat_dist);

   Point exclusion_box_center = exclusion_box.Get_Center();
   float32_t right_limit = exclusion_box_center.y + 0.5*exclusion_box.Get_Width();
   /** \result
    * Check that returned right limit is equal to max_exclusion_box_lat_dist
    */
   DOUBLES_EQUAL(max_exclusion_box_lat_dist, right_limit, F360_EPSILON);
}

/** \purpose
 * Purpose of this test is to verify that right limit is properly determined when there is a valid SEP on the right side of host.
 * \req
 * NA.
 */
TEST(define_overall_confidence_lateral_exclusion_box, Define_Overall_Confidence_Lateral_Exclusion_Box__Right_LSC_Present)
{
   /** \precond
    * In test group the following has been set up:
    * - Max lateral exclusion box distance has been set to 8.
    * - A SEP has been set up on the right side of host with a lateral position that is half the max lateral exclusion box distance.
    * - The lower and upper longitudinal limit of the SEP has been set to -100m and 100m.
    */

    /** \action
     * Call Define_Overall_Confidence_Exclusion_Box_Around_Host
     */
   const BoundingBox exclusion_box = Define_Overall_Confidence_Exclusion_Box_Around_Host(static_env_polys, calib, max_exclusion_box_lat_dist);

   Point exclusion_box_center = exclusion_box.Get_Center();
   float32_t right_limit = exclusion_box_center.y + 0.5*exclusion_box.Get_Width();

   /** \result
    * Check that returned right limit is equal to right SEP p0 coefficient
    */
   DOUBLES_EQUAL(static_env_polys[1].p0, right_limit, F360_EPSILON);
}
/** @}*/

/** \defgroup  Is_In_Area_Of_Interest
 *  @{
 */

/** \brief
 *  Test group for Is_In_Area_Of_Interest()
 **/
TEST_GROUP(Is_In_Area_Of_Interest)
{
    const float32_t zone_longpos_shift = 2.0F;
};

/** \brief
 *  Test checks if object is inside the area of interest on the right hand side.
 **/
TEST(Is_In_Area_Of_Interest, check_if_inside_area_right_side)
{
    /** \precond
    */
    const Point track_pos{ 5.0F, 5.0F };
    /** \action
       Call Is_In_Area_Of_Interest
    */
    const bool f_inside_area_of_interest = Is_Outside_Triangular_Zone_Behind_Host(track_pos, zone_longpos_shift);

    /** \result
       check if flag is true
    */
    CHECK_TRUE(f_inside_area_of_interest);
}

/** \brief
 *  Test checks if object is outside the area of interest on the right hand side.
 **/
TEST(Is_In_Area_Of_Interest, check_if_outside_area_right_side)
{
    /** \precond
    */
    const Point track_pos{ -5.0F, 5.0F };

    /** \action
        Call Is_In_Area_Of_Interest
    */
    const bool f_inside_area_of_interest = Is_Outside_Triangular_Zone_Behind_Host(track_pos, zone_longpos_shift);

    /** \result
        check if flag is false
    */
    CHECK_FALSE(f_inside_area_of_interest);
}

/** \brief
 *  Test checks if object is inside the area of interest on the left hand side.
 **/
TEST(Is_In_Area_Of_Interest, check_if_inside_area_left_side)
{
    /** \precond
    */
    const Point track_pos{ 5.0F, -5.0F };

    /** \action
       Call Is_In_Area_Of_Interest
    */
    const bool f_inside_area_of_interest = Is_Outside_Triangular_Zone_Behind_Host(track_pos, zone_longpos_shift);

    /** \result
       check if flag is true
    */
    CHECK_TRUE(f_inside_area_of_interest);
}

/** \brief
 *  Test checks if object is outside the area of interest on the left hand side.
 **/
TEST(Is_In_Area_Of_Interest, check_if_outside_area_left_side)
{
    /** \precond
    */
    const Point track_pos{ -5.0F, -5.0F };

    /** \action
       Call Is_In_Area_Of_Interest
    */
    const bool f_inside_area_of_interest = Is_Outside_Triangular_Zone_Behind_Host(track_pos, zone_longpos_shift);

    /** \result
       check if flag is false
    */
    CHECK_FALSE(f_inside_area_of_interest);
}
/** @}*/

/** \defgroup  Is_Moving_In_Specified_Direction
 *  @{
 */

/** \brief
 *  Test group for Is_Moving_In_Specified_Direction()
 **/
TEST_GROUP(Is_Moving_In_Specified_Direction)
{
    F360_Calibrations_T calib = {};
    F360_Object_Track_T obj_trk;
    float32_t tolerance = 0.1F;
    TEST_SETUP()
    {
       Initialize_Tracker_Calibrations(calib);
       obj_trk.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
       obj_trk.vcs_heading =  Angle{ std::abs(calib.k_low_conf_unreliability_min_heading) + tolerance };
    }
};

/** \brief
 *  Test checks that object is relevant for exclusion zone when it is a CCTA track and its heading is above threshold.
 **/
TEST(Is_Moving_In_Specified_Direction, check_CCTA_above_threshold_relevant)
{
    /** \precond
    */

    /** \action
       Call Is_Moving_In_Specified_Direction
    */
    const bool f_moving_in_specified_direction = Is_Heading_Different_Than_Host(obj_trk, calib);

    /** \result
       Check if flag is true
    */
    CHECK_TRUE(f_moving_in_specified_direction);
}

/** \brief
 *  Test checks that object is irrelevant for exclusion zone when it is a CCTA track and its heading is below threshold.
 **/
TEST(Is_Moving_In_Specified_Direction, check_CCTA_below_threshold_irrelevant)
{
    /** \precond
    */
    obj_trk.vcs_heading = Angle{ std::abs(calib.k_low_conf_unreliability_min_heading) - tolerance };

    /** \action
       Call Is_Moving_In_Specified_Direction
    */
    const bool f_moving_in_specified_direction = Is_Heading_Different_Than_Host(obj_trk, calib);

    /** \result
       Check if flag is false
    */
    CHECK_FALSE(f_moving_in_specified_direction);
}

/** \brief
 *  Test checks that object is irrelevant for exclusion zone when it is a CCA track and its heading is above threshold.
 **/
TEST(Is_Moving_In_Specified_Direction, check_CCA_above_threshold_irrelevant)
{
    /** \precond
    */
    obj_trk.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;

    /** \action
       Call Is_Moving_In_Specified_Direction
    */
    const bool f_moving_in_specified_direction = Is_Heading_Different_Than_Host(obj_trk, calib);

    /** \result
       Check if flag is false
    */
    CHECK_FALSE(f_moving_in_specified_direction);
}

/** \brief
 *  Test checks that object is irrelevant for exclusion zone when it is a CCA track and its heading is below threshold.
 **/
TEST(Is_Moving_In_Specified_Direction, check_CCA_below_threshold_irrelevant)
{
    /** \precond
    */
    obj_trk.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
    obj_trk.vcs_heading = Angle{ std::abs(calib.k_low_conf_unreliability_min_heading) - tolerance };
    obj_trk.speed = calib.fast_moving_thresh + tolerance;

    /** \action
       Call Is_Moving_In_Specified_Direction
    */
    const bool f_moving_in_specified_direction = Is_Heading_Different_Than_Host(obj_trk, calib);

    /** \result
       Check if flag is false
    */
    CHECK_FALSE(f_moving_in_specified_direction);
}
/** @}*/

/** \defgroup  Is_Outside_Exclusion_Box
 *  @{
 */

/** \brief
 *  Test group for Is_Outside_Exclusion_Box()
 **/
TEST_GROUP(Is_Outside_Exclusion_Box)
{
   //center(x,    y)     len   wid   ori
   const BoundingBox exclusion_box = BoundingBox(Point(2.0F, 2.0F), 6.0F, 4.0F, Angle{0.0F});
   F360_Host_T host = {};
   F360_Object_Track_T obj_trk = {};
   bool f_recently_entered_FOV;


};

/** \brief
 *  Test checks if track is outside of a specified exclusion box.
 **/
TEST(Is_Outside_Exclusion_Box, check_if_outside_exclusion_box)
{
    /** \precond
     * Object position is outside of exclusion box
     * Object has not recently entered FOV
    */
    obj_trk.vcs_position.x = 3.0F;
    obj_trk.vcs_position.y = 5.0F;
    f_recently_entered_FOV = false;

    /** \action
       Call Is_Outside_Exclusion_Box
    */
    const bool f_outside_exclusion_box = Is_Outside_Exclusion_Box(host, obj_trk, exclusion_box, f_recently_entered_FOV);

    /** \result
       Check if flag is true
    */
    CHECK_TRUE(f_outside_exclusion_box);
}

/** \brief
 *  Test checks if track is inside of a specified exclusion box.
 **/
TEST(Is_Outside_Exclusion_Box, check_if_inside_exclusion_box)
{
    /** \precond
     * Object position is inside exclusion box
     * Object has not recently entered FOV
    */
    obj_trk.vcs_position.x = 3.0F;
    obj_trk.vcs_position.y = 3.0F;
    f_recently_entered_FOV = false;

    /** \action
       Call Is_Outside_Exclusion_Box
    */
    const bool f_outside_exclusion_box = Is_Outside_Exclusion_Box(host, obj_trk, exclusion_box, f_recently_entered_FOV);

    /** \result
       Check if flag is false
    */
    CHECK_FALSE(f_outside_exclusion_box);
}
/** \brief
 *  Test checks if track is inside of a specified (extended) exclusion box when it has recently entered FOV.
 **/
TEST(Is_Outside_Exclusion_Box, check_if_inside_extended_exclusion_box)
{
    /** \precond
     * Object position is inside exclusion box extended by speed difference
     * Object has recently entered FOV
    */
    obj_trk.vcs_position.x = 6.0F;
    obj_trk.vcs_position.y = 3.0F;
    f_recently_entered_FOV = true;
    obj_trk.speed = 20.0F;
    host.speed = 5.0F;

    /** \action
       Call Is_Outside_Exclusion_Box
    */
    const bool f_outside_exclusion_box = Is_Outside_Exclusion_Box(host, obj_trk, exclusion_box, f_recently_entered_FOV);

    /** \result
       Check if flag is true
    */
    CHECK_FALSE(f_outside_exclusion_box);
}

/** @}*/

/** \defgroup  Has_Low_TTC
 *  @{
 */

/** \brief
 *  Test group for Has_Low_TTC()
 **/
TEST_GROUP(Has_Low_TTC)
{
    F360_Calibrations_T calib = {};
    const float32_t test_pass_th = 1e-5F;
    float32_t host_speed = 1.0F;
    TEST_SETUP()
    {
        Initialize_Tracker_Calibrations(calib);
    }
};

/** \brief
 *  Test checks if track TTC is below threshold.
 **/
TEST(Has_Low_TTC, check_if_TTC_below_threshold)
{
    /** \precond
    */
    F360_VCS_Velocity_T track_velocity;
    track_velocity.longitudinal = 0.0F;
    Point track_pos = { calib.k_low_conf_unreliability_max_ttc - test_pass_th, 0.0F };

    /** \action
       Call Has_Low_TTC
    */
    const bool f_low_ttc = Has_Low_TTC(host_speed, track_velocity, track_pos, calib.k_low_conf_unreliability_max_ttc);

    /** \result
       Check if flag is true
    */
    CHECK_TRUE(f_low_ttc);
}

/** \brief
 *  Test checks if track TTC is above threshold.
 **/
TEST(Has_Low_TTC, check_if_TTC_above_threshold)
{
    /** \precond
    */
    F360_VCS_Velocity_T track_velocity;
    track_velocity.longitudinal = 0.0F;
    Point track_pos = { calib.k_low_conf_unreliability_max_ttc + test_pass_th, 0.0F };

    /** \action
       Call Has_Low_TTC
    */
    const bool f_low_ttc = Has_Low_TTC(host_speed, track_velocity, track_pos, calib.k_low_conf_unreliability_max_ttc);

    /** \result
       Check if flag is false
    */
    CHECK_FALSE(f_low_ttc);
}

/** \brief
 *  Test checks if track TTC is above threshold if velocity difference is zero.
 **/
TEST(Has_Low_TTC, check_if_TTC_zero)
{
    /** \precond
    */
    F360_VCS_Velocity_T track_velocity;
    track_velocity.longitudinal = 1.0F;
    Point track_pos = { 10.0F, 0.0F };

    /** \action
       Call Has_Low_TTC
    */
    const bool f_low_ttc = Has_Low_TTC(host_speed, track_velocity, track_pos, calib.k_low_conf_unreliability_max_ttc);

    /** \result
       Check if flag is false
    */
    CHECK_FALSE(f_low_ttc);
}

/** \brief
 *  Test checks if track low_TTC flag is false if TTC is at threshold.
 **/
TEST(Has_Low_TTC, check_lowTTC_at_threshold)
{
    /** \precond
    */
    F360_VCS_Velocity_T track_velocity;
    track_velocity.longitudinal = 0.0F;
    Point track_pos = { calib.k_low_conf_unreliability_max_ttc, 0.0F };

    /** \action
       Call Has_Low_TTC
    */
    const bool f_low_ttc = Has_Low_TTC(host_speed, track_velocity, track_pos, calib.k_low_conf_unreliability_max_ttc);

    /** \result
       Check if flag is false
    */
    CHECK_FALSE(f_low_ttc);
}
/** @}*/

/** \defgroup  Cond_LP_Filter_Reduced_Det_Num
 *  @{
 */

 /** \brief
   *  Test group of Cond_LP_Filter_Reduced_Det_Num
   */
TEST_GROUP(Cond_LP_Filter_Reduced_Det_Num)
{
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS];
   F360_Tracker_Info_T tracker_info = {};
   F360_Calibrations_T calib;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
   }
};

/** \purpose
 * Purpose of this test is to verify whether number of filtered dets after LP filter is not changed when number of filtered dets is
 * larger than number of reduced detections and the number of reduced detections is above 0
 *\req    NA
 */
TEST(Cond_LP_Filter_Reduced_Det_Num, Cond_LP_Filter_Reduced_Det_Num__check_whether_number_of_filtered_dets_is_unchanged)
{
   /** \precond
    * Define needed data
    * - Set object status as F360_OBJECT_STATUS_NEW
    * - Set number of filtered detections to 20
    * - Set number of reduced detections to 1
    */
   tracker_info.num_active_objs = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_NEW;
   object_tracks[0].filtered_dets = 20.0F;
   object_tracks[0].num_rr_inlier_dets = 1;

   /** \action
    * Call function Cond_LP_Filter_Reduced_Det_Num
    */
   Cond_LP_Filter_Reduced_Det_Num(tracker_info, calib.k_tv_dets_exp_filter_const, object_tracks);

   /** \result
    * Number of filtered detections should not change
    */
   DOUBLES_EQUAL(object_tracks[0].filtered_dets, 20.0F, F360_EPSILON);
}

/** \purpose
 * Purpose of this test is to verify whether number of filtered dets after LP filter is decreasing when number of filtered dets is
 * not larger than number of reduced detections or the number of reduced detections equals 0
 *\req    NA
 */
TEST(Cond_LP_Filter_Reduced_Det_Num, Cond_LP_Filter_Reduced_Det_Num__check_whether_number_of_filtered_dets_is_decreasing)
{
   /** \precond
    * Define needed data
    * - Set object status as F360_OBJECT_STATUS_NEW
    * - Set number of filtered detections to 2
    * - Set number of reduced detections to 0
    */
   tracker_info.num_active_objs = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_NEW;
   object_tracks[0].filtered_dets = 2.0F;
   object_tracks[0].num_rr_inlier_dets = 0;

   /** \action
    * Call function Cond_LP_Filter_Reduced_Det_Num
    */
   Cond_LP_Filter_Reduced_Det_Num(tracker_info, calib.k_tv_dets_exp_filter_const, object_tracks);

   /** \result
    * Number of filtered detections should decay
    */
   DOUBLES_EQUAL(object_tracks[0].filtered_dets, 1.9F, F360_EPSILON);
}

/**
 *\purpose  Purpose of this test is to verify whether newly updated tracks have their number of filtered dets initialized.
 *\req    NA
 */
TEST(Cond_LP_Filter_Reduced_Det_Num, Cond_LP_Filter_Reduced_Det_Num__check_whether_new_updated_tracks_have_their_filtered_dets_initialized)
{
   /** \precond
    * Define needed data
    * - Set object status as F360_OBJECT_STATUS_NEW_UPDATED
    * - Set number of filtered detections to 0
    * - Set number of reduced detections to 1
    */
   tracker_info.num_active_objs = 1;
   object_tracks[0].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_tracks[0].filtered_dets = 0.0F;
   object_tracks[0].num_rr_inlier_dets = 1;

   /** \action
    * Call function Cond_LP_Filter_Reduced_Det_Num
    */
   Cond_LP_Filter_Reduced_Det_Num(tracker_info, calib.k_tv_dets_exp_filter_const, object_tracks);

   /** \result
   * Number of fitlered dets should be greater than 0
   **/
   DOUBLES_EQUAL(object_tracks[0].filtered_dets, 0.525F, F360_EPSILON);
}

/** @}*/

/** \defgroup  Determine_FOV_Status
 *  @{
 */

 /** \brief
   *  Test group of Determine_FOV_Status
   */
TEST_GROUP(Determine_FOV_Status)
{
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS];
};

/** \purpose
 * Purpose of this test is to verify the function will return true if FOV is limited
 *\req    NA
 */
TEST(Determine_FOV_Status, Determine_FOV_Status__FOV_status_limited)
{
   /** \precond
    * - Set two sensors with limited FOV
    */
   sensors[0].variable.is_valid = true;
   sensors[0].variable.vacs_boresight_az_estimated = 0.5F * F360_PI;
   sensors[0].constant.fov_min_az_rad[F360_DET_LOOK_ID_0] = -0.3F * F360_PI;
   sensors[0].constant.fov_max_az_rad[F360_DET_LOOK_ID_0] = 0.3F * F360_PI;

   sensors[1].variable.is_valid = true;
   sensors[1].variable.vacs_boresight_az_estimated = - 0.5 * F360_PI;
   sensors[1].constant.fov_min_az_rad[F360_DET_LOOK_ID_0] = -0.3F * F360_PI;
   sensors[1].constant.fov_max_az_rad[F360_DET_LOOK_ID_0] = 0.3F * F360_PI;


   /** \action
    * Call function Determine_FOV_Status
    */
   bool f_FOV_limited = Determine_FOV_Status(sensors);

   /** \result
    * Check that FOV is limited
    */
   CHECK_TRUE(f_FOV_limited)
}
/** \purpose
 * Purpose of this test is to verify the function will return true if sensors have full FOV
 *\req    NA
 */
TEST(Determine_FOV_Status, Determine_FOV_Status__FOV_status_full)
{
   /** \precond
    * - Set four sensors that will have combined full FOV
    */
   sensors[0].variable.is_valid = true;
   sensors[0].variable.vacs_boresight_az_estimated = 0.5F * F360_PI;
   sensors[0].constant.fov_min_az_rad[F360_DET_LOOK_ID_0] = -0.3F * F360_PI;
   sensors[0].constant.fov_max_az_rad[F360_DET_LOOK_ID_0] = 0.3F * F360_PI;

   sensors[1].variable.is_valid = true;
   sensors[1].variable.vacs_boresight_az_estimated = 0.0;
   sensors[1].constant.fov_min_az_rad[F360_DET_LOOK_ID_0] = -0.3F * F360_PI;
   sensors[1].constant.fov_max_az_rad[F360_DET_LOOK_ID_0] = 0.3F * F360_PI;

   sensors[2].variable.is_valid = true;
   sensors[2].variable.vacs_boresight_az_estimated = - 0.5 * F360_PI;
   sensors[2].constant.fov_min_az_rad[F360_DET_LOOK_ID_0] = -0.3F * F360_PI;
   sensors[2].constant.fov_max_az_rad[F360_DET_LOOK_ID_0] = 0.3F * F360_PI;

   sensors[3].variable.is_valid = true;
   sensors[3].variable.vacs_boresight_az_estimated = - F360_PI;
   sensors[3].constant.fov_min_az_rad[F360_DET_LOOK_ID_0] = -0.3F * F360_PI;
   sensors[3].constant.fov_max_az_rad[F360_DET_LOOK_ID_0] = 0.25F * F360_PI;

   /** \action
    * Call function Determine_FOV_Status
    */
   bool f_FOV_limited = Determine_FOV_Status(sensors);

   /** \result
    * Check that FOV is not limited
    */
   CHECK_FALSE(f_FOV_limited)
}

/** @}*/

/** \defgroup  Has_Object_Recently_Entered_FOV
 *  @{
 */

 /** \brief
   *  Test group of Has_Object_Recently_Entered_FOV
   */
TEST_GROUP(Has_Object_Recently_Entered_FOV)
{
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Host_T host = {};
   F360_Object_Track_T obj_trk = {};

   // establish one sensor in front of host with limited FOV
   // Set up object driving fast
   TEST_SETUP()
   {
      sensors[0].variable.is_valid = true;

      sensors[0].refined.left_fov_normal[0] = 0.86F;
      sensors[0].refined.left_fov_normal[1] = 0.49F;
      sensors[0].refined.left_fov_normal[2] = 0.86F;
      sensors[0].refined.left_fov_normal[3] = 0.49F;

      sensors[0].refined.right_fov_normal[0] = 0.86F;
      sensors[0].refined.right_fov_normal[1] = -0.49F;
      sensors[0].refined.right_fov_normal[2] = 0.86F;
      sensors[0].refined.right_fov_normal[3] = -0.49F;

      //sensors[0].refined.left_fov_normal =  {0.86F, 0.49F, 0.86F, 0.49F};
      //sensors[0].refined.right_fov_normal =  {0.86F, -0.49F, 0.86F, -0.49F}

      obj_trk.vcs_velocity.longitudinal = 30.0F;
      obj_trk.speed = 30.0F;

   }
};

/** \purpose
 * Purpose of this test is to verify the function will return true if object is new in sensors' FOV
 *\req    NA
 */
TEST(Has_Object_Recently_Entered_FOV, Has_Object_Recently_Entered_FOV__True)
{
   /** \precond
    * Set up object that 1 second ago was not in sensors' FOV
    */
   obj_trk.bbox.Set_Center({15.0F, 3.0F});

   /** \action
    * Call function Has_Object_Recently_Entered_FOV
    */
   bool f_recently_entered_FOV = Has_Object_Recently_Entered_FOV(host, sensors, obj_trk);

   /** \result
    * Check that function returns true
    */
   CHECK_TRUE(f_recently_entered_FOV)
}
/** \purpose
 * Purpose of this test is to verify the function will return true if object is new in sensors' FOV and is oncoming
 *\req    NA
 */
TEST(Has_Object_Recently_Entered_FOV, Has_Object_Recently_Entered_FOV__Oncoming)
{
   /** \precond
    * Set up sensor looking at the side of host
    * Set up object that 1 second ago was not in sensors' FOV
    */
   obj_trk.bbox.Set_Center({0.0F, 3.0F});
   obj_trk.vcs_velocity.longitudinal = -30.0F;
   obj_trk.vcs_heading.Value({F360_PI});
   const float one_over_sqrt2 = 0.707106781186547524401;
   sensors[0].refined.left_fov_normal[0] = one_over_sqrt2;
   sensors[0].refined.left_fov_normal[1] = one_over_sqrt2;
   sensors[0].refined.left_fov_normal[2] = one_over_sqrt2;
   sensors[0].refined.left_fov_normal[3] = one_over_sqrt2;

   sensors[0].refined.right_fov_normal[0] = -one_over_sqrt2;
   sensors[0].refined.right_fov_normal[1] = one_over_sqrt2;
   sensors[0].refined.right_fov_normal[2] = -one_over_sqrt2;
   sensors[0].refined.right_fov_normal[3] = one_over_sqrt2;

   /** \action
    * Call function Has_Object_Recently_Entered_FOV
    */
   bool f_recently_entered_FOV = Has_Object_Recently_Entered_FOV(host, sensors, obj_trk);

   /** \result
    * Check that function returns true
    */
   CHECK_TRUE(f_recently_entered_FOV)
}

/** \purpose
 * Purpose of this test is to verify the function will return false if object is not new in sensors' FOV
 *\req    NA
 */
TEST(Has_Object_Recently_Entered_FOV, Has_Object_Recently_Entered_FOV__False)
{
   /** \precond
    * Set up object that 1 second ago still was in sensors' FOV
    */

    obj_trk.bbox.Set_Center({60.0F, 3.0F});

   /** \action
    * Call function Has_Object_Recently_Entered_FOV
    */
   bool f_recently_entered_FOV = Has_Object_Recently_Entered_FOV(host, sensors, obj_trk);

   /** \result
    * Check that function returns false
    */
   CHECK_FALSE(f_recently_entered_FOV)
}

/** \purpose
 * Purpose of this test is to verify the function will return false if object is crossing into FOV
 *\req    NA
 */
TEST(Has_Object_Recently_Entered_FOV, Has_Object_Recently_Entered_FOV__Crossing)
{
   /** \precond
    * Set up object that 1 second ago still was in sensors' FOV
    */

    obj_trk.bbox.Set_Center({45.0F, -25.0F});
    obj_trk.vcs_heading.Value({F360_PI_2});

   /** \action
    * Call function Has_Object_Recently_Entered_FOV
    */
   bool f_recently_entered_FOV = Has_Object_Recently_Entered_FOV(host, sensors, obj_trk);

   /** \result
    * Check that function returns false
    */
   CHECK_FALSE(f_recently_entered_FOV)
}
/** @}*/
