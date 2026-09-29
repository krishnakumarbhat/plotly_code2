/** \file
   This file contains unit tests for testing the requirements connected to f360_post_update_track_adjustments module
*/

#include "f360_post_update_track_adjustments.h"
#include "f360_object_track_management.h"
#include "f360_adjust_fltr_type_dependent_params.h"
#include "f360_trk_fltr_ccv_states.h"
#include "f360_update_object_track_properties.h"
#include "f360_calibrations.h"
#include "f360_math.h"
#include "f360_cancel_new_updated_trk_overlapping_confirmed_trks.h"
#include "f360_set_variant.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

using namespace f360_variant_A;

/** \defgroup  f360_update_object_track_properties_Obj_Size
 *  @{
 */

/** \brief
 * Group of tests to check that object length and width are updated under correct circumstances.
 */
TEST_GROUP(f360_update_object_track_properties_Calc_Obj_Size)
{
   /** \setup
   * Set up an object with some detections
   */
   F360_Calibrations_T calibs;
   F360_Globals_T globals = {};
   F360_Tracker_Info_T tracker_info = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list = {};
   F360_Host_T host = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   const float32_t test_eps = 1e-5F;

   float32_t initial_obj_len;
   float32_t initial_obj_wid;

   /** \setup
    * Set up a general scenario with calibrations, one object with one detection on each side of
    * the object and relevant tracker info parameters.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 1;
      object_tracks[0].id = 1;

      globals.f_single_front_center_radar_only = false;

      object_tracks[0U].bbox.Set_Length(5.0F);
      object_tracks[0U].bbox.Set_Width(1.5F);
      object_tracks[0U].reference_point = F360_REFERENCE_POINT_LEFT;
      object_tracks[0U].vcs_position = {0.0F, 8.0F};
      object_tracks[0U].ndets = 2;
      object_tracks[0U].detids[0] = 1;
      object_tracks[0U].detids[1] = 2;
      object_tracks[0U].status = F360_OBJECT_STATUS_UPDATED;
      object_tracks[0U].movable_prob = 1.0F;
      object_tracks[0U].f_vehicular_trk = true;
      object_tracks[0U].length_uncertainty = 0.2F;
      object_tracks[0U].width_uncertainty = 0.2F;
      object_tracks[0U].speed = 8.0F;

      det_props[0].vcs_position = {3.0F, 6.7F};
      det_props[0].object_track_id = 1;
      raw_detect_list.detections[0U].raw.range = 10.0F;
      det_props[1].vcs_position = {-3.0F, 8.0F};
      det_props[1].object_track_id = 1;
      raw_detect_list.detections[1U].raw.range = 10.0F;   
   }
};

/** \purpose
 * Test that object size is updated when object has status updated and 2 associated detections
 *\req    CPR-3745
 */
TEST(f360_update_object_track_properties_Calc_Obj_Size, Obj_Size_Is_Updated)
{
   /** \precond
    * An object and two detections have been set up
    * Assign objects inital length and width to temporary variables
    */
   initial_obj_len = object_tracks[0].bbox.Get_Length();
   initial_obj_wid = object_tracks[0].bbox.Get_Width();

   /** \action
    * Call Update_Object_Track_Properties()
    */
   Update_Object_Track_Properties(calibs, globals, raw_detect_list, host, sensors, tracker_info, det_props, object_tracks, timing_info);

   /** \result
    * Check that object dimensions have been updated
    */
   CHECK_FALSE_TEXT(initial_obj_len == object_tracks[0].bbox.Get_Length(), "Object length was not updated");
   CHECK_FALSE_TEXT(initial_obj_wid == object_tracks[0].bbox.Get_Width(), "Object width was not updated");
}

/** \purpose
 * Test that object size is not updated when object has status coasted and 2 associated detections
 * \req    CPR-3745
 */
TEST(f360_update_object_track_properties_Calc_Obj_Size, Obj_Size_Is_Not_Updated_Obj_Coasting)
{
   /** \precond
    * An object and two detections have been set up
    * Assign objects inital length and width to temporary variables
    */
   object_tracks[0U].status = F360_OBJECT_STATUS_COASTED;
   initial_obj_len = object_tracks[0].bbox.Get_Length();
   initial_obj_wid = object_tracks[0].bbox.Get_Width();

   /** \action
    * Call Update_Object_Track_Properties()
    */
   Update_Object_Track_Properties(calibs, globals, raw_detect_list, host, sensors, tracker_info, det_props, object_tracks, timing_info);

   /** \result
    * Check that object dimensions have not been updated
    */
   CHECK_TRUE_TEXT(initial_obj_len == object_tracks[0].bbox.Get_Length(), "Object length was updated");
   CHECK_TRUE_TEXT(initial_obj_wid == object_tracks[0].bbox.Get_Width(), "Object width was updated");
}

/** \purpose
 * Test that object size is not updated when object has status updated but only 1 associated detection
 * \req    CPR-3745
 */
TEST(f360_update_object_track_properties_Calc_Obj_Size, Obj_Size_Is_Not_Updated_Obj_One_Assoc_Det)
{
   /** \precond
    * An object and two detections have been set up
    * Assign objects inital length and width to temporary variables
    */
   object_tracks[0U].ndets = 1;
   det_props[1].object_track_id = 0;
   initial_obj_len = object_tracks[0].bbox.Get_Length();
   initial_obj_wid = object_tracks[0].bbox.Get_Width();

   /** \action
    * Call Update_Object_Track_Properties()
    */
   Update_Object_Track_Properties(calibs, globals, raw_detect_list, host, sensors, tracker_info, det_props, object_tracks, timing_info);

   /** \result
    * Check that object dimensions have been updated
    */
   CHECK_TRUE_TEXT(initial_obj_len == object_tracks[0].bbox.Get_Length(), "Object length was updated");
   CHECK_TRUE_TEXT(initial_obj_wid == object_tracks[0].bbox.Get_Width(), "Object width was updated");
}

/** @}*/

/** \defgroup  f360_object_track_management_qualtest
 *  @{
 **/

/** \brief
 *  Includes tests that will test the behavior of functions implemented in f360_object_track_management.cpp.
 **/
TEST_GROUP(f360_object_track_management_qualtest)
{
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Object_Track_T object_track[NUMBER_OF_OBJECT_TRACKS];
   F360_Tracker_Info_T tracker_info;
   F360_TRKR_TIMING_INFO_T timing_info;
   float32_t time_since_stage_start_bef;
   float32_t time_since_track_updated_bef;
   F360_Calibrations_T calibs;

   /** \setup
    * Setting up common variables for test group f360_object_track_management.
    **/
   TEST_SETUP()
   {
      calibs.k_puta_max_vcs_xposn_for_ghost_NU_2_C = 50.0F;
      calibs.k_puta_max_vcs_yposn_for_ghost_NU_2_C = 30.0F;

      det_props[MAX_NUMBER_OF_DETECTIONS] =
         { };
      object_track[NUMBER_OF_OBJECT_TRACKS] =
         { };
      tracker_info =
         { };
      timing_info =
         { };

      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 1;
      Set_Tracker_Variant(tracker_info.variant);

      object_track[0].detids[0] = 1;
      object_track[0].ndets = 1;
   }
};

/**
 *\purpose  This test will test Obj_Trk_Status_Book_Keeping() when the status is NEW in some way.
 *\req    CPR-3743
 */
TEST(f360_object_track_management_qualtest, Obj_Trk_Status_Book_Keeping_NEW)
{
   /** \step{1}
    * Check that a NEW_UPDATED object which is set to COASTED also gets f_ghost_NU_2_C set to true.
    **/

   /** \precond
    * Create initialized input. Set parameters so we will be able to enter wanted cases.
    **/
   object_track[0].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_track[0].time_since_initialization = 0.05F;
   object_track[0].time_since_stage_start = 0.05F;
   object_track[0].time_since_track_updated = 0.05F;
   time_since_track_updated_bef = object_track[0].time_since_track_updated;
   object_track[0].num_rr_inlier_dets = 0;
   object_track[0].vcs_position.x = 20.0F;
   object_track[0].vcs_position.y = 10.0F;

   det_props[0].f_rr_inlier = false;
   det_props[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;

   /** \action
    * Call Obj_Trk_Status_Book_Keeping() which affects time_since_stage_start, time_since_track_updated, status and f_ghost_NU_2_C.
    **/
   Obj_Trk_Status_Book_Keeping(det_props, tracker_info, calibs, object_track, timing_info);

   /** \result
    * The status is expected to be COASTED, time_since_stage_start = 0, time_since_track_updated to be constant and f_ghost_NU_2_C to be true.
    **/
   CHECK_EQUAL(object_track[0].status, F360_OBJECT_STATUS_COASTED);
   DOUBLES_EQUAL(object_track[0].time_since_stage_start, 0.0F, F360_EPSILON);
   DOUBLES_EQUAL(object_track[0].time_since_track_updated, time_since_track_updated_bef, F360_EPSILON);
   CHECK(object_track[0].f_ghost_NU_2_C);

   /** \step{2}
    * Check that a NEW_UPDATED object which receives valid detections is set to UPDATED.
    **/

   /** \precond
    * Create initialized input. Set parameters so we will be able to enter wanted cases.
    **/
   object_track[0].status = F360_OBJECT_STATUS_NEW_UPDATED;
   object_track[0].time_since_initialization = 0.05F;
   object_track[0].time_since_stage_start = 0.05F;
   time_since_stage_start_bef = object_track[0].time_since_stage_start;
   object_track[0].time_since_track_updated = 0.05F;
   object_track[0].num_rr_inlier_dets = 1;

   det_props[0].f_rr_inlier = true;
   det_props[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;

   /** \action
    * Call Obj_Trk_Status_Book_Keeping() which affects time_since_stage_start, time_since_track_updated, status and f_ghost_NU_2_C.
    **/
   Obj_Trk_Status_Book_Keeping(det_props, tracker_info, calibs, object_track, timing_info);

   /** \result
    * The status is expected to be UPDATED, time_since_stage_start to be constant, time_since_track_updated = 0 and f_ghost_NU_2_C to be false.
    **/
   CHECK_EQUAL(object_track[0].status, F360_OBJECT_STATUS_UPDATED);
   DOUBLES_EQUAL(object_track[0].time_since_stage_start, time_since_stage_start_bef, F360_EPSILON);
   DOUBLES_EQUAL(object_track[0].time_since_track_updated, 0.0F, F360_EPSILON);
   CHECK(!object_track[0].f_ghost_NU_2_C);
}

/**
 *\purpose  This test will test Obj_Trk_Status_Book_Keeping() when the status is F360_OBJECT_STATUS_UPDATED.
 *\req    CPR-3744
 */
TEST(f360_object_track_management_qualtest, Obj_Trk_Status_Book_Keeping_UPDATED)
{
   /** \step{1}
    * Check that an UPDATED object stays UPDATED when enough approved detections have been associated,
    * i.e. when there are enough num_rr_inlier_dets (> 0 and > #wheelspin detections).
    **/

   /** \precond
    Create initialized input. Set parameters so we will be able to enter wanted cases.
    **/
   object_track[0].status = F360_OBJECT_STATUS_UPDATED;
   object_track[0].time_since_initialization = 1.00F;
   object_track[0].time_since_stage_start = 0.50F;
   time_since_stage_start_bef = object_track[0].time_since_stage_start;
   object_track[0].time_since_track_updated = 0.05F;
   object_track[0].num_rr_inlier_dets = 1;

   det_props[0].f_rr_inlier = true;
   det_props[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   /** \action
    * Call Obj_Trk_Status_Book_Keeping() which affects time_since_stage_start, time_since_track_updated, status and f_ghost_NU_2_C.
    **/
   Obj_Trk_Status_Book_Keeping(det_props, tracker_info, calibs, object_track, timing_info);

   /** \result
    * The status is expected to stay as UPDATED, time_since_stage_start to be constant, time_since_track_updated = 0 and f_ghost_NU_2_C to be false.
    **/
   CHECK_EQUAL(object_track[0].status, F360_OBJECT_STATUS_UPDATED);
   DOUBLES_EQUAL(object_track[0].time_since_stage_start, time_since_stage_start_bef, F360_EPSILON);
   DOUBLES_EQUAL(object_track[0].time_since_track_updated, 0.0F, F360_EPSILON);
   CHECK(!object_track[0].f_ghost_NU_2_C);

   /** \step{2}
    * Check that an UPDATED object is set to COASTED when there are no satisfying detections associated.
    **/

   /** \precond
    * Create initialized input. Set parameters so we will be able to enter wanted cases.
    **/
   object_track[0].status = F360_OBJECT_STATUS_UPDATED;
   object_track[0].time_since_initialization = 1.00F;
   object_track[0].time_since_stage_start = 0.50F;
   object_track[0].time_since_track_updated = 0.05F;
   time_since_track_updated_bef = object_track[0].time_since_track_updated;
   object_track[0].num_rr_inlier_dets = 1;

   det_props[0].f_rr_inlier = true;
   det_props[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   /** \action
    * Call Obj_Trk_Status_Book_Keeping() which affects time_since_stage_start, time_since_track_updated, status and f_ghost_NU_2_C.
    **/
   Obj_Trk_Status_Book_Keeping(det_props, tracker_info, calibs, object_track, timing_info);

   /** \result
    * The status is expected to be set as COASTED, time_since_track_updated to be constant, time_since_stage_start = 0 and f_ghost_NU_2_C to be false.
    **/
   CHECK_EQUAL(object_track[0].status, F360_OBJECT_STATUS_COASTED);
   DOUBLES_EQUAL(object_track[0].time_since_track_updated, time_since_track_updated_bef, F360_EPSILON);
   DOUBLES_EQUAL(object_track[0].time_since_stage_start, 0.0F, F360_EPSILON);
   CHECK(!object_track[0].f_ghost_NU_2_C);
}

/** @}*/

/** \defgroup  f360_adjust_fltr_type_dependet_params_qualtest
 *  @{
 */

 /** \brief
  *  This test group tests that objects's filter types are changed correctly.
  */

using namespace f360_variant_A;

TEST_GROUP(f360_adjust_fltr_type_dependet_params_qualtest)
{
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Calibrations_T calibrations = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};

   float32_t object_errorcov_ref[STATE_DIMENSION][STATE_DIMENSION] = {};
   float32_t test_epsilon = 0.001F;

   /** \setup
   **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);
      calibrations.fast_moving_thresh = 5.0F;

      for (unsigned int idx = 0; idx < NUMBER_OF_OBJECT_TRACKS; idx++)
      {
         object_tracks[idx].trk_fltr_type = F360_TRACKER_TRKFLTR_INVALID;
         object_tracks[idx].f_moving = false;
         object_tracks[idx].speed = calibrations.fast_moving_thresh - test_epsilon;
      }

      object_tracks[3].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      object_tracks[3].f_moving = true;
      object_tracks[3].speed = calibrations.fast_moving_thresh + test_epsilon;

      for (int row = 0; row < STATE_DIMENSION; row++)
      {
         for (int col = 0; col < STATE_DIMENSION; col++)
         {
            object_errorcov_ref[row][col] = (row + col) * 0.01F;
            object_tracks[3].errcov[row][col] = object_errorcov_ref[row][col];
         }
      }

      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 4;
      Set_Tracker_Variant(tracker_info.variant);
   }
};

/**
*\purpose   Correct switching to CTCA (CCV before)
*/
TEST(f360_adjust_fltr_type_dependet_params_qualtest, Adjust_Fltr_Type_Dependent_Params__switch_to_CTCA)
{
   /** \precond
   **/
    object_tracks[3].time_since_initialization = 2.0F + 0.1F;
    object_tracks[3].curvature = 0.018F - 0.001F;

   /** \action
   **/
   Adjust_Fltr_Type_Dependent_Params(tracker_info, object_tracks, calibrations, timing_info);

   /** \result
   **/
   CHECK_EQUAL(object_tracks[3].trk_fltr_type, F360_TRACKER_TRKFLTR_CTCA);
}

/** @}*/


// REPLACE WITH NEW SIZE TEST (?)

/** \defgroup  f360_cancel_new_updated_trk_overlapping_confirmed_trks_qualtest
 *  @{
 */

 /** \brief
  * Tests that an objects is terminated correctly when overlapping with another.
  */
TEST_GROUP(f360_cancel_new_updated_trk_overlapping_confirmed_trks_qualtest)
{
   F360_Calibrations_T calib;
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Tracker_Info_T tracker_info;
   F360_TRKR_TIMING_INFO_T timing_info;
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS];

   /** \setup
    * This tests sets up a scenario with two objects whose areas are overlapping.
    */
   TEST_SETUP()
   {
      tracker_info.num_active_objs = 2U;
      tracker_info.active_obj_ids[0] = 1U;
      tracker_info.active_obj_ids[1] = 2U;
      Set_Tracker_Variant(tracker_info.variant);

      // Object 1 NEW_UPDATED
      object_tracks[0].id = 1;
      object_tracks[0].f_moving = true;
      object_tracks[0].vcs_position.x = 10.0F;
      object_tracks[0].vcs_position.y = 10.0F;
      object_tracks[0].Set_Bbox_Orientation(Angle{ 0.0F });
      object_tracks[0].status = F360_OBJECT_STATUS_NEW_UPDATED;
      object_tracks[0].Update_Bbox_Size(4.0F, 2.0F);
      object_tracks[0].Set_Bbox_Orientation(Angle{ 0.0F });
      // Object 2 UPDATED
      object_tracks[1].id = 2;
      object_tracks[1].f_moving = true;
      object_tracks[1].vcs_position.x = 10.1F;
      object_tracks[1].vcs_position.y = 10.1F;
      object_tracks[1].Set_Bbox_Orientation(Angle{ 0.0F });
      object_tracks[1].status = F360_OBJECT_STATUS_UPDATED;
      object_tracks[1].time_since_cluster_created = 1.0F;
      object_tracks[1].confidenceLevel = 1.0F;
      object_tracks[1].Update_Bbox_Size(4.0F, 2.0F);
      object_tracks[1].Set_Bbox_Orientation(Angle{ 0.0F });

      Initialize_Tracker_Calibrations(calib);
   }

};

/** \purpose
 * Purpose of this test is to check that when two moving objects, one with status F360_OBJECT_STATUS_NEW_UPDATED
 * and the other with F360_OBJECT_STATUS_UPDATED, have bounding boxes with enough overlap, the object with
 * status F360_OBJECT_STATUS_NEW_UPDATED is terminated, given that the object with status F360_OBJECT_STATUS_UPDATED
 * has a confidence level above 0.5 and time_since_cluster_created above 0.4 seconds.
 * \req
 * CPR-3746
 */
TEST(f360_cancel_new_updated_trk_overlapping_confirmed_trks_qualtest, Cancel_New_Updated_Trk_Overlapping_Confirmed_Trks_Second_Obj_Updated)
{
   /** \precond
    * - Two objects have been set up close to each other such that their bounding boxes overlap is > 75%.
    * - The first object's is moving and its status is set to F360_OBJECT_STATUS_NEW_UPDATED.
    * - The second object's is moving and its status is set to F360_OBJECT_STATUS_UPDATED, it's confidence level and
    *   time_since_cluster_created above their respective thresholds.
    * - detection_posn_hist has been set up with some arbitrary properties for a slot reserved for the first object.
    */

   /** \action
    * Call function
    */
   Cancel_New_Updated_Trk_Overlapping_Confirmed_Trks(calib, object_tracks, tracker_info, det_props, timing_info);

   /** \result
    * Verify that all object 1's properties that were assigned in TEST_SETUP are reset to default values and that
    * the slot reserved for object 1 in detection_posn_hist has been reset.
    */
   CHECK_EQUAL(1U, tracker_info.num_active_objs);
   DOUBLES_EQUAL(0.0F, object_tracks[0].vcs_position.x, F360_EPSILON);
   DOUBLES_EQUAL(0.0F, object_tracks[0].vcs_position.y, F360_EPSILON);
   DOUBLES_EQUAL(0.0F, object_tracks[0].bbox.Get_Orientation().Value(), F360_EPSILON);
   DOUBLES_EQUAL(0.0F, object_tracks[0].bbox.Get_Length(), F360_EPSILON);
   DOUBLES_EQUAL(0.0F, object_tracks[0].bbox.Get_Width(), F360_EPSILON);
   CHECK_FALSE(object_tracks[0].f_moving);
   CHECK_EQUAL(F360_OBJECT_STATUS_INVALID, object_tracks[0].status)
}

/** \purpose
 * Purpose of this test is to check that when two moving objects, one with status F360_OBJECT_STATUS_NEW_UPDATED
 * and the other with F360_OBJECT_STATUS_COASTED, have bounding boxes with enough overlap, the object with
 * status F360_OBJECT_STATUS_NEW_UPDATED is terminated, given that the object with status F360_OBJECT_STATUS_COASTED
 * has a confidence level above 0.5 and time_since_cluster_created above 0.4 seconds.
 * \req
 * CPR-3746
 */
TEST(f360_cancel_new_updated_trk_overlapping_confirmed_trks_qualtest, Cancel_New_Updated_Trk_Overlapping_Confirmed_Trks_Second_Obj_Coasted)
{
   /** \precond
    * - Two objects have been set up close to each other such that their bounding boxes overlap is > 75%.
    * - The first object's is moving and its status is set to F360_OBJECT_STATUS_NEW_UPDATED.
    * - The second object's is moving and its status is set to F360_OBJECT_STATUS_COASTED, it's confidence level and
    *   time_since_cluster_created above their respective thresholds.
    * - detection_posn_hist has been set up with some arbitrary properties for a slot reserved for the first object.
    */
   object_tracks[1].status = F360_OBJECT_STATUS_COASTED;
   /** \action
    * Call function
    */
   Cancel_New_Updated_Trk_Overlapping_Confirmed_Trks(calib, object_tracks, tracker_info, det_props, timing_info);

   /** \result
    * Verify that all object 1's properties that were assigned in TEST_SETUP are reset to default values and that
    * the slot reserved for object 1 in detection_posn_hist has been reset.
    */
   CHECK_EQUAL(1U, tracker_info.num_active_objs);
   DOUBLES_EQUAL(0.0F, object_tracks[0].vcs_position.x, F360_EPSILON);
   DOUBLES_EQUAL(0.0F, object_tracks[0].vcs_position.y, F360_EPSILON);
   DOUBLES_EQUAL(0.0F, object_tracks[0].bbox.Get_Orientation().Value(), F360_EPSILON);
   DOUBLES_EQUAL(0.0F, object_tracks[0].bbox.Get_Length(), F360_EPSILON);
   DOUBLES_EQUAL(0.0F, object_tracks[0].bbox.Get_Width(), F360_EPSILON);
   CHECK_FALSE(object_tracks[0].f_moving);
   CHECK_EQUAL(F360_OBJECT_STATUS_INVALID, object_tracks[0].status)
}
/** @}*/

/** \defgroup  f360_update_object_track_properties_obj_height
 *  @{
 */

/** \brief
 * Group of tests to check that objects over the ground height is updated under correct circumstances.
 */
TEST_GROUP(f360_update_object_track_properties_calc_obj_height)
{
   /** \setup
    * Set up common variables used by Update_Object_Track_Properties
    */
   F360_Calibrations_T calibs;
   F360_Globals_T globals = {};
   F360_Tracker_Info_T tracker_info = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list = {};
   F360_Host_T host = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   const float32_t test_eps = 1e-5F;
   /** \setup
    * Initialize tracker calibrations
    * Set up a valid moving object in front of the host
    * Set up initial over the ground height to 0.0F
    * Set up initial historic ndets count to 10.0F
    * Assign valid detections to the object with non-zero vcs_position_z
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 1;
      object_tracks[0].id = 1;
      object_tracks[0].f_moving = true;
      object_tracks[0].otg_height = 0.0F;
      object_tracks[0].ud_mov_historic_ndets = 10.0F;
      object_tracks[0].ndets = 3;
      for (uint8_t i=0U; i<object_tracks[0].ndets; i++) 
      {
         raw_detect_list.detections[i].processed.vcs_position_z = -1.01F * pow(10,i);
         raw_detect_list.detections[i].processed.motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
         raw_detect_list.detections[i].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_HIGH;
         object_tracks[0].detids[i] = i+1;
      }
   }
};

/** \purpose
 * Test that object over the ground height is updated when object is valid and has valid associated detections
 *\req
 * CPR-3747
 */
TEST(f360_update_object_track_properties_calc_obj_height, obj_height_is_updated_when_valid)
{
   /** \precond
    * Default test setup from TEST GROUP is used.
    */
   
   /** \action
    * Call Update_Object_Track_Properties()
    */
   Update_Object_Track_Properties(calibs, globals, raw_detect_list, host, sensors, tracker_info, det_props, object_tracks, timing_info);

   /** \result
    * Check if object height has been updated
    */
   DOUBLES_EQUAL(8.82755852, object_tracks[0].otg_height, F360_EPSILON);
}

/** \purpose
 * Test that object over the ground height is constant when object is valid but has no detections associated
 *\req
 * CPR-3747
 */
TEST(f360_update_object_track_properties_calc_obj_height, obj_height_decays_when_no_dets)
{
   /** \precond
    * Object has no detections assigned
    * Object over the ground height is above zero
    */
   object_tracks[0].ndets = 0;
   object_tracks[0].otg_height_raw = 101.0F;

   /** \action
    * Call Update_Object_Track_Properties()
    */
   Update_Object_Track_Properties(calibs, globals, raw_detect_list, host, sensors, tracker_info, det_props, object_tracks, timing_info);

   /** \result
    * Check if object height has decayed properly
    */
   DOUBLES_EQUAL(101.0F, object_tracks[0].otg_height, F360_EPSILON);
}

/** \purpose
 * Test that object over the ground height does not reach negative values when decaying effect could bring the value below zero 
 *\req
 * CPR-3747
 */
TEST(f360_update_object_track_properties_calc_obj_height, obj_height_decay_below_zero)
{
   /** \precond
    * Object has no detections assigned
    */
   object_tracks[0].ndets = 0;
   /** \action
    * Call Update_Object_Track_Properties()
    */
   Update_Object_Track_Properties(calibs, globals, raw_detect_list, host, sensors, tracker_info, det_props, object_tracks, timing_info);

   /** \result
    * Check if object height did not reach negative values
    */
   DOUBLES_EQUAL(0.0F, object_tracks[0].otg_height, F360_EPSILON);
}
/** @}*/
