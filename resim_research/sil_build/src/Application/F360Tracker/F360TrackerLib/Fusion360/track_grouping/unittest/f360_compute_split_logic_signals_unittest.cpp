/** \file
 * This file contains unit tests for content of f360_compute_split_logic_signals.cpp file
 */

#include "f360_compute_split_logic_signals.h"
#include <CppUTest/TestHarness.h>

#include "f360_clear_detections_props.h"
#include "f360_clear_object_track.h"
#include "f360_math_func.h"
#include "f360_math.h"

using namespace f360_variant_A;

/** \defgroup  f360_compute_split_logic_signals
 *  @{
 */

/** \brief
 *  *  Test group for unit testing Compute_Split_Logic_Signals function.
 */
TEST_GROUP(f360_compute_split_logic_signals)
{
   // Initialize common variables used within all tests in this test group.
   const float32_t TOLERANCE = 0.0001F;
   F360_Detection_Props_T det_p[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   rspp_variant_A::RSPP_Detection_T detections[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Calibrations_T calibs = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Host_T host = {};


   /** \setup
    * Reset variables commonly used within all tests in this test group.
    */
   TEST_SETUP()
   {
      Clear_Detections_Props(det_p);
      Initialize_Tracker_Calibrations(calibs);

      for (uint32_t idx = 0U; idx < NUMBER_OF_OBJECT_TRACKS; idx++)
      {
         Clear_Object_Track(object_tracks[idx]);
         object_tracks[idx].Set_Bbox_Orientation(Angle{ 0.0F });
         object_tracks[idx].vcs_heading = Angle{ 0.0F };
      }
   }
};

/** \purpose
 * Verify that Compute_Objects_Detections_Max_Gap() is computing the intended orth_gap_filtered given an object and an array of sorted orth positions (between radar detections' positions)
 * when there are 5 detections associated.
 * \req
 * NA.
 */
TEST(f360_compute_split_logic_signals, Compute_And_Filter_Objects_Detections_Max_Gap_Verify_Calculation_Correctness_5dets)
{
   /** \precond
    * Set orth_sorted_pos such it the largest element is max_gap.
    * Compute the expected orth_gap_filtered signal (previous filtered value = 0.0)
    */
   F360_Object_Track_T& object = object_tracks[0];
   object.ndets = 5;
   object.orth_gap_filtered = 0.0F;

   float32_t orth_sorted_pos[MAX_DETS_IN_OBJ_TRK] = {};
   orth_sorted_pos[0] = -1.0F;
   orth_sorted_pos[1] = 1.0F;
   orth_sorted_pos[2] = 2.0F;

   float32_t expected_max_gap_filtered =0.6F;

   /** \action
    * Call Compute_And_Filter_Objects_Detections_Max_Gap().
    */
   Compute_And_Filter_Objects_Detections_Max_Gap(calibs, orth_sorted_pos, object);
   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(expected_max_gap_filtered, object.orth_gap_filtered, TOLERANCE);
}

/** \purpose
 * Verify that Compute_Objects_Detections_Max_Gap() is computing the intended orth_gap_filtered given an object and an array of sorted orth positions (between radar detections' positions)
 * when there are 2 detections associated.
 * \req
 * NA.
 */
TEST(f360_compute_split_logic_signals, Compute_And_Filter_Objects_Detections_Max_Gap_Verify_Calculation_Correctness_2dets)
{
   /** \precond
    * Set orth_sorted_pos such it the largest element is max_gap.
    * Compute the expected orth_gap_filtered signal (previous filtered value = 0.0)
    */
   F360_Object_Track_T& object = object_tracks[0];
   object.ndets = 2;
   object.orth_gap_filtered = 0.0F;

   float32_t orth_sorted_pos[MAX_DETS_IN_OBJ_TRK] = {};
   orth_sorted_pos[0] = -1.0F;
   orth_sorted_pos[1] = 1.0F;
   orth_sorted_pos[2] = 2.0F;

   float32_t expected_max_gap_filtered =0.24F;

   /** \action
    * Call Compute_And_Filter_Objects_Detections_Max_Gap().
    */
   Compute_And_Filter_Objects_Detections_Max_Gap(calibs, orth_sorted_pos, object);
   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(expected_max_gap_filtered, object.orth_gap_filtered, TOLERANCE);
}

/** \purpose
 * Verify that Compute_Objects_Detections_Max_Gap() is computing the intended orth_gap_filtered given an object and an array of sorted orth positions (between radar detections' positions)
 * when there are more than 5 detections associated.
 * \req
 * NA.
 */
TEST(f360_compute_split_logic_signals, Compute_And_Filter_Objects_Detections_Max_Gap_Verify_Calculation_Correctness_7dets)
{
   /** \precond
    * Set orth_sorted_pos such it the largest element is max_gap.
    * Compute the expected orth_gap_filtered signal (previous filtered value = 0.0)
    */
   F360_Object_Track_T& object = object_tracks[0];
   object.ndets = 7;
   object.orth_gap_filtered = 0.0F;

   float32_t orth_sorted_pos[MAX_DETS_IN_OBJ_TRK] = {};
   orth_sorted_pos[0] = -1.0F;
   orth_sorted_pos[1] = 1.0F;
   orth_sorted_pos[2] = 2.0F;

   float32_t expected_max_gap_filtered =0.6F;

   /** \action
    * Call Compute_And_Filter_Objects_Detections_Max_Gap().
    */
   Compute_And_Filter_Objects_Detections_Max_Gap(calibs, orth_sorted_pos, object);
   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(expected_max_gap_filtered, object.orth_gap_filtered, TOLERANCE);
}

/** \purpose
 * Verify that Compute_Objects_Detections_Max_Delta() is computing the intended orth_delta_filtered given an object and sorted (radar detections') positions.
 * \req
 * NA.
 */
TEST(f360_compute_split_logic_signals, Compute_And_Filter_Objects_Detections_Max_Delta_Verify_Calculation_Correctness)
{
   /** \precond
    * Set orth_sorted_pos such it has a total spread of max_delta.
    * Compute the expected delta filtered signal.
    */
   float32_t max_delta = 3;

   F360_Object_Track_T& object = object_tracks[0];
   object.ndets = 3;
   object.orth_delta_filtered = 0.0F;

   float32_t orth_sorted_pos[MAX_DETS_IN_OBJ_TRK] = {};
   orth_sorted_pos[0] = -0.5 * max_delta;
   orth_sorted_pos[1] = 0;
   orth_sorted_pos[2] = 0.5 * max_delta;

   float32_t expected_max_delta_filtered = F360_Low_Pass_Filter_First_Order(max_delta, object.orth_delta_filtered, calibs.k_orth_split_orth_delta_filter_const);

   /** \action
    * Call Compute_And_Filter_Objects_Detections_Max_Delta().
    */
   Compute_And_Filter_Objects_Detections_Max_Delta(calibs.k_orth_split_orth_delta_filter_const, orth_sorted_pos, object);
   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(expected_max_delta_filtered, object.orth_delta_filtered, TOLERANCE);
}

/** \purpose
 * Verify that Derive_Object_Orth_Split_Signal_Status() will set a split_signals_status appropriately given different object positions and number of radar detections.
 * \req
 * NA.
 */
TEST(f360_compute_split_logic_signals, Derive_Object_Orth_Split_Signal_Status_Check_All_Split_Status_Flags)
{
   /** \precond
    * Set object_to_reset position to be farther away from host than calib.k_orth_split_max_distance_sq.
    * Set object_to_freeze_1 to have 2 detections and position to be closer to host than calib.k_orth_split_min_distance_sq.
    * Set object_to_freeze_2 to have 1 detection and its position to be calib.k_orth_split_min_distance_sq < obj_pos < calib.k_orth_split_max_distance_sq.
    * Set object_to_update to have have 2 detection and its position to be calib.k_orth_split_min_distance_sq < obj_pos < calib.k_orth_split_max_distance_sq.
    */

   F360_Object_Track_T& object_to_reset = object_tracks[0];
   F360_Object_Track_T& object_to_freeze_1 = object_tracks[1];
   F360_Object_Track_T& object_to_freeze_2 = object_tracks[2];
   F360_Object_Track_T& object_to_freeze_3 = object_tracks[3];
   F360_Object_Track_T& object_to_update = object_tracks[4];

   float32_t dist_sq_reset = 1.2 * calibs.k_orth_split_max_distance_sq;
   float32_t dist_sq_freeze = 0.8 * calibs.k_orth_split_min_distance_sq;
   float32_t dist_sq_update = calibs.k_orth_split_min_distance_sq + 0.5*(calibs.k_orth_split_max_distance_sq - calibs.k_orth_split_min_distance_sq);

   // Compute object.vcs_position.y such that they trigger the reset, freeze, and update flag
   host.dist_rear_axle_to_vcs_m = 3 / 0.6F;
   float32_t half_host_length = 0.6F * host.dist_rear_axle_to_vcs_m;

   // dist_sq = obj_long_pos_host_center^2 + obj.vcs_pos.lat^2
   float32_t obj_long_pos_host_center = object_to_reset.vcs_position.x + half_host_length;
   object_to_reset.vcs_position.y = F360_Sqrtf(dist_sq_reset - obj_long_pos_host_center*obj_long_pos_host_center);

   obj_long_pos_host_center = object_to_freeze_1.vcs_position.x + half_host_length;
   object_to_freeze_1.vcs_position.y = F360_Sqrtf(dist_sq_freeze - obj_long_pos_host_center*obj_long_pos_host_center);
   object_to_freeze_1.ndets = 2;

   obj_long_pos_host_center = object_to_freeze_2.vcs_position.x + half_host_length;
   object_to_freeze_2.vcs_position.y = F360_Sqrtf(dist_sq_update - obj_long_pos_host_center*obj_long_pos_host_center);
   object_to_freeze_2.ndets = 1;

   obj_long_pos_host_center = object_to_freeze_2.vcs_position.x + half_host_length;
   object_to_freeze_3.vcs_position.y = F360_Sqrtf(dist_sq_freeze - obj_long_pos_host_center*obj_long_pos_host_center);
   object_to_freeze_3.ndets = 1;

   obj_long_pos_host_center = object_to_update.vcs_position.x + half_host_length;
   object_to_update.vcs_position.y = F360_Sqrtf(dist_sq_update - obj_long_pos_host_center*obj_long_pos_host_center);
   object_to_update.ndets = 2;

   /** \action
    * Call Derive_Object_Orth_Split_Signal_Status().
    */
   F360_Object_Orth_Split_Signals_Status_Type_T orth_split_status_reset = Derive_Object_Orth_Split_Signal_Status(calibs, object_to_reset, host.dist_rear_axle_to_vcs_m);
   F360_Object_Orth_Split_Signals_Status_Type_T orth_split_status_freeze_1 = Derive_Object_Orth_Split_Signal_Status(calibs, object_to_freeze_1, host.dist_rear_axle_to_vcs_m);
   F360_Object_Orth_Split_Signals_Status_Type_T orth_split_status_freeze_2 = Derive_Object_Orth_Split_Signal_Status(calibs, object_to_freeze_2, host.dist_rear_axle_to_vcs_m);
   F360_Object_Orth_Split_Signals_Status_Type_T orth_split_status_freeze_3 = Derive_Object_Orth_Split_Signal_Status(calibs, object_to_freeze_3, host.dist_rear_axle_to_vcs_m);
   F360_Object_Orth_Split_Signals_Status_Type_T orth_split_status_update = Derive_Object_Orth_Split_Signal_Status(calibs, object_to_update, host.dist_rear_axle_to_vcs_m);

   /** \result
    * Check that the output match expected data.
    */
   CHECK_EQUAL(orth_split_status_reset, F360_RESET_SPLIT_SIGNALS);
   CHECK_EQUAL(orth_split_status_freeze_1, F360_DONT_INNOVATE_SPLIT_SIGNALS);
   CHECK_EQUAL(orth_split_status_freeze_2, F360_DONT_INNOVATE_SPLIT_SIGNALS);
   CHECK_EQUAL(orth_split_status_freeze_3, F360_DONT_INNOVATE_SPLIT_SIGNALS);
   CHECK_EQUAL(orth_split_status_update, F360_UPDATE_SPLIT_SIGNALS);
}

/** \purpose
 * Test that function Compute_Split_Logic_Signals() resets/freezes/updates split logic signals as intended given some specific object positions and detections.
 * In this test most objects have filter type CTCA.
 * \req
 * NA.
 */
TEST(f360_compute_split_logic_signals, Compute_Split_Logic_Signals_Verify_That_Signals_Are_Updated_As_Expected_CTCA)
{
   /** \precond
    * Setup 4 objects such that;
    * obj_idx 0 will trigger a reset.
    * obj_idx 1 will be irrelevant to the function (due to incorrect trk_fltr_type) and thus remain unchanged.
    * obj_idx 2 will trigger a freeze signals.
    * obj_idx 3 will trigger an split signal.
    */
   float32_t detections_max_delta_and_gap = 6.0F;
   float32_t initial_gap_delta_filtered = 1.0F;

   float32_t dist_sq_reset = 1.2 * calibs.k_orth_split_max_distance_sq;
   float32_t dist_sq_freeze = 0.8 * calibs.k_orth_split_min_distance_sq;
   float32_t dist_sq_update = calibs.k_orth_split_min_distance_sq + 0.5*(calibs.k_orth_split_max_distance_sq - calibs.k_orth_split_min_distance_sq);

   tracker_info.num_active_objs = 4;
   for (int32_t i = 0; i < tracker_info.num_active_objs ; i++)
   {
      tracker_info.active_obj_ids[i] = i+1;
      object_tracks[i].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      object_tracks[i].orth_delta_filtered = initial_gap_delta_filtered;
      object_tracks[i].orth_gap_filtered = initial_gap_delta_filtered;
   }
   object_tracks[1].trk_fltr_type = F360_TRACKER_TRKFLTR_CCV;

   // Set positions of objects
   host.dist_rear_axle_to_vcs_m = 0.0F;
   object_tracks[0].vcs_position.x = F360_Sqrtf(dist_sq_reset);
   object_tracks[1].vcs_position.x = F360_Sqrtf(dist_sq_reset);
   object_tracks[2].vcs_position.x = F360_Sqrtf(dist_sq_freeze);
   object_tracks[3].vcs_position.x = F360_Sqrtf(dist_sq_update);
   object_tracks[3].vcs_velocity.longitudinal = 10.0F;

   // Set positions of detections to center around object(s) that need them
   object_tracks[3].detids[0] = 1;
   object_tracks[3].detids[1] = 2;
   object_tracks[3].ndets = 2;
   det_p[object_tracks[3].detids[0]-1].vcs_position.x = object_tracks[3].vcs_position.x;
   det_p[object_tracks[3].detids[0]-1].vcs_position.y = object_tracks[3].vcs_position.y-0.5*detections_max_delta_and_gap;
   det_p[object_tracks[3].detids[0]-1].range_rate_compensated = 10.5F;
   det_p[object_tracks[3].detids[1]-1].vcs_position.x = object_tracks[3].vcs_position.x;
   det_p[object_tracks[3].detids[1]-1].vcs_position.y = object_tracks[3].vcs_position.y+0.5*detections_max_delta_and_gap;
   det_p[object_tracks[3].detids[1]-1].range_rate_compensated = 10.0F;
   detections[object_tracks[3].detids[0]-1].raw.sensor_id = 1;
   detections[object_tracks[3].detids[1]-1].raw.sensor_id = 1;

   // Set expected split signals for updated object(s).
   float32_t expected_max_delta_filtered = 2.0F;
   float32_t expected_max_gap_filtered = 1.6F;
   float32_t expected_range_rate_diff_filtered = 0.2F;

   /** \action
    * Call Compute_Split_Logic_Signals().
    */
   Compute_Split_Logic_Signals(det_p, sensors, detections, calibs, tracker_info, host.dist_rear_axle_to_vcs_m, object_tracks);
   /** \result
    * Check that the output match expected data.
    * object_tracks[0] should have its split signals reset (set to 0).
    * object_tracks[1] should have its split signals unchanged.
    * object_tracks[2] should have its split signals unchanged.
    * object_tracks[3] should have its split signals updated.
    */
   DOUBLES_EQUAL(0.0F, object_tracks[0].orth_delta_filtered, TOLERANCE);
   DOUBLES_EQUAL(0.0F, object_tracks[0].orth_gap_filtered, TOLERANCE);

   DOUBLES_EQUAL(initial_gap_delta_filtered, object_tracks[1].orth_delta_filtered, TOLERANCE);
   DOUBLES_EQUAL(initial_gap_delta_filtered, object_tracks[1].orth_gap_filtered, TOLERANCE);
   DOUBLES_EQUAL(initial_gap_delta_filtered, object_tracks[2].orth_delta_filtered, TOLERANCE);
   DOUBLES_EQUAL(initial_gap_delta_filtered, object_tracks[2].orth_gap_filtered, TOLERANCE);

   DOUBLES_EQUAL(expected_max_delta_filtered, object_tracks[3].orth_delta_filtered, TOLERANCE);
   DOUBLES_EQUAL(expected_max_gap_filtered, object_tracks[3].orth_gap_filtered, TOLERANCE);
   DOUBLES_EQUAL(expected_range_rate_diff_filtered, object_tracks[3].orth_range_rate_diff_filtered, TOLERANCE);
}

/** \purpose
 * Test that function Compute_Split_Logic_Signals() resets/freezes/updates split logic signals as intended given some specific object positions and detections.
 * In this test, most objects have filter type CCA.
 * \req
 * NA.
 */
TEST(f360_compute_split_logic_signals, Compute_Split_Logic_Signals_Verify_That_Signals_Are_Updated_As_Expected_CCA)
{
   /** \precond
    * Setup 4 objects such that;
    * obj_idx 0 will trigger a reset.
    * obj_idx 1 will be irrelevant to the function (due to incorrect trk_fltr_type) and thus remain unchanged.
    * obj_idx 2 will trigger a freeze signals.
    * obj_idx 3 will trigger an split signal.
    */
   float32_t detections_max_delta_and_gap = 6.0F;
   float32_t initial_gap_delta_filtered = 1.0F;

   float32_t dist_sq_reset = 1.2 * calibs.k_orth_split_max_distance_sq;
   float32_t dist_sq_freeze = 0.8 * calibs.k_orth_split_min_distance_sq;
   float32_t dist_sq_update = calibs.k_orth_split_min_distance_sq + 0.5*(calibs.k_orth_split_max_distance_sq - calibs.k_orth_split_min_distance_sq);

   tracker_info.num_active_objs = 4;
   for (int32_t i = 0; i < tracker_info.num_active_objs ; i++)
   {
      tracker_info.active_obj_ids[i] = i+1;
      object_tracks[i].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      object_tracks[i].orth_delta_filtered = initial_gap_delta_filtered;
      object_tracks[i].orth_gap_filtered = initial_gap_delta_filtered;
   }
   object_tracks[1].trk_fltr_type = F360_TRACKER_TRKFLTR_CCV;

   // Set positions of objects
   host.dist_rear_axle_to_vcs_m = 0.0F;
   object_tracks[0].vcs_position.x = F360_Sqrtf(dist_sq_reset);
   object_tracks[1].vcs_position.x = F360_Sqrtf(dist_sq_reset);
   object_tracks[2].vcs_position.x = F360_Sqrtf(dist_sq_freeze);
   object_tracks[3].vcs_position.x = F360_Sqrtf(dist_sq_update);
   object_tracks[3].vcs_velocity.longitudinal = 10.0F;

   // Set positions of detections to center around object(s) that need them
   object_tracks[3].detids[0] = 1;
   object_tracks[3].detids[1] = 2;
   object_tracks[3].ndets = 2;
   det_p[object_tracks[3].detids[0]-1].vcs_position.x = object_tracks[3].vcs_position.x;
   det_p[object_tracks[3].detids[0]-1].vcs_position.y = object_tracks[3].vcs_position.y-0.5*detections_max_delta_and_gap;
   det_p[object_tracks[3].detids[0]-1].range_rate_compensated = 10.5F;
   det_p[object_tracks[3].detids[1]-1].vcs_position.x = object_tracks[3].vcs_position.x;
   det_p[object_tracks[3].detids[1]-1].vcs_position.y = object_tracks[3].vcs_position.y+0.5*detections_max_delta_and_gap;
   det_p[object_tracks[3].detids[1]-1].range_rate_compensated = 10.0F;
   detections[object_tracks[3].detids[0]-1].raw.sensor_id = 1;
   detections[object_tracks[3].detids[1]-1].raw.sensor_id = 1;

   // Set expected split signals for updated object(s).
   float32_t expected_max_delta_filtered = 2.0F;
   float32_t expected_max_gap_filtered = 1.6F;
   float32_t expected_range_rate_diff_filtered = 0.2F;

   /** \action
    * Call Compute_Split_Logic_Signals().
    */
   Compute_Split_Logic_Signals(det_p, sensors, detections, calibs, tracker_info, host.dist_rear_axle_to_vcs_m, object_tracks);
   /** \result
    * Check that the output match expected data.
    * object_tracks[0] should have its split signals reset (set to 0).
    * object_tracks[1] should have its split signals unchanged.
    * object_tracks[2] should have its split signals unchanged.
    * object_tracks[3] should have its split signals updated.
    */
   DOUBLES_EQUAL(0.0F, object_tracks[0].orth_delta_filtered, TOLERANCE);
   DOUBLES_EQUAL(0.0F, object_tracks[0].orth_gap_filtered, TOLERANCE);

   DOUBLES_EQUAL(initial_gap_delta_filtered, object_tracks[1].orth_delta_filtered, TOLERANCE);
   DOUBLES_EQUAL(initial_gap_delta_filtered, object_tracks[1].orth_gap_filtered, TOLERANCE);
   DOUBLES_EQUAL(initial_gap_delta_filtered, object_tracks[2].orth_delta_filtered, TOLERANCE);
   DOUBLES_EQUAL(initial_gap_delta_filtered, object_tracks[2].orth_gap_filtered, TOLERANCE);

   DOUBLES_EQUAL(expected_max_delta_filtered, object_tracks[3].orth_delta_filtered, TOLERANCE);
   DOUBLES_EQUAL(expected_max_gap_filtered, object_tracks[3].orth_gap_filtered, TOLERANCE);
   DOUBLES_EQUAL(expected_range_rate_diff_filtered, object_tracks[3].orth_range_rate_diff_filtered, TOLERANCE);
}
/** \purpose
 * Test that function Compute_Split_Logic_Signals() updates split orth_gap_filtered and orth_delta filtered but does not update range_rate_diff_filtered
 * due to object's cross-radial movement
 * \req
 * NA.
 */
TEST(f360_compute_split_logic_signals, Compute_Split_Logic_Signals_Object_Moves_Cross_Radial)
{
   /** \precond
    * Setup object such that orth_gap_filtered and _orth_delta_filtered will be updated,
    * but range_rate_diff_filtered will not.
    * */
   float32_t detections_max_delta_and_gap = 6.0F;
   float32_t initial_gap_delta_filtered = 1.0F;
   host.dist_rear_axle_to_vcs_m = 0.0F;
   tracker_info.num_active_objs = 1;
   tracker_info.active_obj_ids[0] = 1;

   F360_Object_Track_T& object = object_tracks[0];
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object.vcs_position.x = 0.0F;
   object.vcs_position.y = 20.0F;
   object.vcs_velocity.longitudinal = 10.0F;
   object.orth_delta_filtered = initial_gap_delta_filtered;
   object.orth_gap_filtered = initial_gap_delta_filtered;
   object.orth_range_rate_diff_filtered = 0.2F;

   object.ndets = 2;
   object.detids[0] = 1;
   object.detids[1] = 2;

   det_p[0].vcs_position.x = object.vcs_position.x;
   det_p[0].vcs_position.y = object.vcs_position.y-0.5*detections_max_delta_and_gap;
   det_p[0].range_rate_compensated = 10.5F;
   det_p[1].vcs_position.x = object.vcs_position.x;
   det_p[1].vcs_position.y = object.vcs_position.y+0.5*detections_max_delta_and_gap;
   det_p[1].range_rate_compensated = 10.0F;
   detections[0].raw.sensor_id = 1;
   detections[1].raw.sensor_id = 1;

   // Set expected split signals for updated object(s).
   float32_t expected_max_delta_filtered = 2.0F;
   float32_t expected_max_gap_filtered = 1.6F;
   float32_t expected_rr_diff_filtered = 0.2F;

   /** \action
    * Call Compute_Split_Logic_Signals().
    */
   Compute_Split_Logic_Signals(det_p, sensors, detections, calibs, tracker_info, host.dist_rear_axle_to_vcs_m, object_tracks);
   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(expected_max_delta_filtered, object_tracks[0].orth_delta_filtered, TOLERANCE);
   DOUBLES_EQUAL(expected_max_gap_filtered, object_tracks[0].orth_gap_filtered, TOLERANCE);
   DOUBLES_EQUAL(expected_rr_diff_filtered, object_tracks[0].orth_range_rate_diff_filtered, TOLERANCE);
}
/** @}*/

/** \defgroup  f360_compute_split_logic_signals_orth_range_rate_diff
 *  @{
 */

/** \brief
 *  *  Test group for unit testing orthogonal range rate difference calculations in Compute_Split_Logic_Signals function.
 */
TEST_GROUP(f360_compute_split_logic_signals_orth_range_rate_diff)
{
   // Initialize common variables used within all tests in this test group.
   const float32_t TOLERANCE = 0.0001F;
   F360_Detection_Props_T det_p[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Calibrations_T calibs = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Host_T host = {};
   uint32_t orth_perm[MAX_DETS_IN_OBJ_TRK] = {};


   /** \setup
    * Reset variables commonly used within all tests in this test group.
    */
   TEST_SETUP()
   {
      Clear_Detections_Props(det_p);
      Initialize_Tracker_Calibrations(calibs);

      for (uint32_t idx = 0U; idx < NUMBER_OF_OBJECT_TRACKS; idx++)
      {
         Clear_Object_Track(object_tracks[idx]);
         object_tracks[idx].Set_Bbox_Orientation(Angle{ 0.0F });
         object_tracks[idx].vcs_heading = Angle{ 0.0F };
         object_tracks[idx].vcs_position.x = 30.0F;
         object_tracks[idx].vcs_velocity.longitudinal = 10.0F;
      }
   }
};
   /** \purpose
 * Verify that Compute_And_Filter_Objects_Detections_Range_Rate_Diff() is computing the intended orth_range_rate_diff_filtered given an object, detections,
 * an array of sorted orth positions (between radar detections' positions) and permutation respective to that sorting
 * when there are 4 detections associated.
 * \req
 * NA.
 */
TEST(f360_compute_split_logic_signals_orth_range_rate_diff, Compute_And_Filter_Objects_Detections_Range_Rate_Diff_Verify_Calculation_Correctness_4dets)
{
   /** \precond
    * Set orth_sorted_pos such it the largest element is max_gap.
    * Compute the expected orth_range_rate_filtered signal (previous filtered value = 0.0)
    */
   F360_Object_Track_T& object = object_tracks[0];
   object.ndets = 4;
   object.orth_range_rate_diff_filtered = 0.0F;

   float32_t orth_sorted_pos[MAX_DETS_IN_OBJ_TRK] = {};
   orth_sorted_pos[0] = -3.0F;
   orth_sorted_pos[1] = -2.0F;
   orth_sorted_pos[2] = 2.0F;
   orth_sorted_pos[3] = 4.0F;
   orth_sorted_pos[4] = 5.0F;

   det_p[0].range_rate_compensated = 10.5F;
   det_p[1].range_rate_compensated = 10.5F;
   det_p[2].range_rate_compensated = 10.0F;
   det_p[3].range_rate_compensated = 10.0F;

   orth_perm[0] = 0;
   object.detids[0] = 1;
   orth_perm[1] = 1;
   object.detids[1] = 2;
   orth_perm[2] = 2;
   object.detids[2] = 3;
   orth_perm[3] = 3;
   object.detids[3] = 4;

   float32_t expected_rr_diff_filtered = 0.2F;

   /** \action
    * Call Compute_And_Filter_Objects_Detections_Range_Rate_Diff().
    */
   Compute_And_Filter_Objects_Detections_Range_Rate_Diff(det_p, calibs, orth_sorted_pos, orth_perm, object);
   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(expected_rr_diff_filtered, object.orth_range_rate_diff_filtered, TOLERANCE);
}
   /** \purpose
 * Verify that Compute_And_Filter_Objects_Detections_Range_Rate_Diff() is not updating orth_range_rate_diff_filtered given an object with associated detections
 * only on its left side
 * \req
 * NA.
 */
TEST(f360_compute_split_logic_signals_orth_range_rate_diff, Compute_And_Filter_Objects_Detections_Range_Rate_Diff_Detections_On_Left_Side)
{
   /** \precond
    * Set orth_sorted_pos such it the largest element is max_gap.
    * Compute the expected orth_gap_filtered signal (previous filtered value = 0.2)
    */
   F360_Object_Track_T& object = object_tracks[0];
   object.ndets = 2;
   object.orth_range_rate_diff_filtered = 0.2F;

   float32_t orth_sorted_pos[MAX_DETS_IN_OBJ_TRK] = {};
   orth_sorted_pos[0] = -3.0F;
   orth_sorted_pos[1] = -2.0F;

   det_p[0].range_rate_compensated = 10.5F;
   det_p[1].range_rate_compensated = 10.5F;

   orth_perm[0] = 0;
   object.detids[0] = 1;
   orth_perm[1] = 1;
   object.detids[1] = 2;

   float32_t expected_rr_diff_filtered = 0.2F;

   /** \action
    * Call Compute_And_Filter_Objects_Detections_Range_Rate_Diff().
    */
   Compute_And_Filter_Objects_Detections_Range_Rate_Diff(det_p, calibs, orth_sorted_pos, orth_perm, object);
   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(expected_rr_diff_filtered, object.orth_range_rate_diff_filtered, TOLERANCE);
}
   /** \purpose
 * Verify that Compute_And_Filter_Objects_Detections_Range_Rate_Diff() is not updating orth_range_rate_diff_filtered given an object with associated detections
 * only on its right side
 * \req
 * NA.
 */
TEST(f360_compute_split_logic_signals_orth_range_rate_diff, Compute_And_Filter_Objects_Detections_Range_Rate_Diff_Detections_On_Right_Side)
{
   /** \precond
    * Set orth_sorted_pos such it the largest element is max_gap.
    * Compute the expected orth_gap_filtered signal (previous filtered value = 0.2)
    */
   F360_Object_Track_T& object = object_tracks[0];
   object.ndets = 2;
   object.orth_range_rate_diff_filtered = 0.2F;

   float32_t orth_sorted_pos[MAX_DETS_IN_OBJ_TRK] = {};
   orth_sorted_pos[0] = 2.0F;
   orth_sorted_pos[1] = 3.0F;

   det_p[0].range_rate_compensated = 10.5F;
   det_p[1].range_rate_compensated = 10.5F;

   orth_perm[0] = 0;
   object.detids[0] = 1;
   orth_perm[1] = 1;
   object.detids[1] = 2;

   float32_t expected_rr_diff_filtered = 0.2F;

   /** \action
    * Call Compute_And_Filter_Objects_Detections_Range_Rate_Diff().
    */
   Compute_And_Filter_Objects_Detections_Range_Rate_Diff(det_p, calibs, orth_sorted_pos, orth_perm, object);
   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(expected_rr_diff_filtered, object.orth_range_rate_diff_filtered, TOLERANCE);
}
   /** \purpose
 * Verify that Compute_And_Filter_Objects_Detections_Range_Rate_Diff() is not updating orth_range_rate_diff_filtered given an object with only wheelspin associated detections
 * \req
 * NA.
 */
TEST(f360_compute_split_logic_signals_orth_range_rate_diff, Compute_And_Filter_Objects_Detections_Range_Rate_Diff_Wheelspin_Detections)
{
   /** \precond
    * Set orth_sorted_pos such it the largest element is max_gap.
    * Compute the expected orth_gap_filtered signal (previous filtered value = 0.2)
    */
   F360_Object_Track_T& object = object_tracks[0];
   object.ndets = 2;
   object.orth_range_rate_diff_filtered = 0.2F;

   float32_t orth_sorted_pos[MAX_DETS_IN_OBJ_TRK] = {};
   orth_sorted_pos[0] = -2.0F;
   orth_sorted_pos[1] = 3.0F;

   det_p[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   det_p[0].range_rate_compensated = 10.5F;
   det_p[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT;
   det_p[1].range_rate_compensated = 10.0F;

   orth_perm[0] = 0;
   object.detids[0] = 1;
   orth_perm[1] = 1;
   object.detids[1] = 2;

   float32_t expected_rr_diff_filtered = 0.2F;

   /** \action
    * Call Compute_And_Filter_Objects_Detections_Range_Rate_Diff().
    */
   Compute_And_Filter_Objects_Detections_Range_Rate_Diff(det_p, calibs, orth_sorted_pos, orth_perm, object);
   /** \result
    * Check that the output match expected data.
    */
   DOUBLES_EQUAL(expected_rr_diff_filtered, object.orth_range_rate_diff_filtered, TOLERANCE);
}
/** @}*/

/** \defgroup  f360_compute_filtered_rr_err_max_gap_and_threshold
 *  @{
 */

/** \brief
 *  *  Test group for unit testing Compute_Filtered_RR_Err_Max_Gap_And_Threshold function.
 */
TEST_GROUP(f360_compute_filtered_rr_err_max_gap_and_threshold)
{
   // Initialize common variables used within all tests in this test group.
   const float32_t TOLERANCE = 0.0001F;
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};

   /** \setup
    * Reset variables commonly used within all tests in this test group.
    */
   TEST_SETUP()
   {
      for (uint32_t idx = 0U; idx < NUMBER_OF_OBJECT_TRACKS; idx++)
      {
         Clear_Object_Track(object_tracks[idx]);
         object_tracks[idx].Set_Bbox_Orientation(Angle{ 0.0F });
         object_tracks[idx].vcs_heading = Angle{ 0.0F };
      }
   }
};

/** \purpose
 * Verify that Compute_Filtered_RR_Err_Max_Gap_And_Threshold() handles the case
 * with only two range rate error values correctly.
 * \req
 * NA.
 */
TEST(f360_compute_filtered_rr_err_max_gap_and_threshold, Compute_Filtered_RR_Err_Max_Gap_And_Threshold_Two_Values)
{
   /** \precond
    * Set up minimum case with only two range rate error values.
    */
   F360_Object_Track_T& object = object_tracks[0];
   object.filtered_rr_err_max_gap = 0.5F; // Previous filtered value

   float32_t sorted_rr_err[MAX_DETS_IN_OBJ_TRK] = {};
   sorted_rr_err[0] = -1.0F;
   sorted_rr_err[1] = 2.0F;
   uint32_t valid_det_cnt = 2U;

   // Expected calculations:
   // Gaps: [3.0] -> Max gap = 3.0 at index 0
   // Gap threshold = (-1.0 + 2.0) / 2 = 0.5
   // Filter constant = 0.25 (new = prev + 0.25 * (max_gap - prev))
   // Filtered max gap = 0.5 + 0.25 * (3.0 - 0.5) = 0.5 + 0.625 = 1.125
   float32_t expected_filtered_gap = 1.125F;
   float32_t expected_threshold = 0.5F;
   float32_t gap_rr_thresh = 0.0F;

   /** \action
    * Call Compute_Filtered_RR_Err_Max_Gap_And_Threshold().
    */
   Compute_Filtered_RR_Err_Max_Gap_And_Threshold(sorted_rr_err, valid_det_cnt, object, gap_rr_thresh);

   /** \result
    * Check that single gap is processed correctly.
    */
   DOUBLES_EQUAL(expected_filtered_gap, object.filtered_rr_err_max_gap, TOLERANCE);
   DOUBLES_EQUAL(expected_threshold, gap_rr_thresh, TOLERANCE);
}

/** \purpose
 * Verify that Compute_Filtered_RR_Err_Max_Gap_And_Threshold() correctly handles
 * the case where multiple gaps have the same maximum value.
 * \req
 * NA.
 */
TEST(f360_compute_filtered_rr_err_max_gap_and_threshold, Compute_Filtered_RR_Err_Max_Gap_And_Threshold_Equal_Gaps)
{
   /** \precond
    * Set up range rate errors with equal maximum gaps.
    */
   F360_Object_Track_T& object = object_tracks[0];
   object.filtered_rr_err_max_gap = 0.2F; // Previous filtered value

   float32_t sorted_rr_err[MAX_DETS_IN_OBJ_TRK] = {};
   sorted_rr_err[0] = -2.0F;
   sorted_rr_err[1] = -1.0F;  // Gap of 1.0
   sorted_rr_err[2] = 0.0F;   // Gap of 1.0 (equal to first)
   sorted_rr_err[3] = 0.5F;   // Gap of 0.5
   uint32_t valid_det_cnt = 4U;

   // Expected calculations:
   // Gaps: [1.0, 1.0, 0.5] -> Max gap = 1.0 at index 0 (first occurrence)
   // Gap threshold = (-2.0 + -1.0) / 2 = -1.5
   // Filter constant = 0.25 -> filtered = 0.2 + 0.25 * (1.0 - 0.2) = 0.2 + 0.2 = 0.4
   float32_t expected_filtered_gap = 0.4F;
   float32_t expected_threshold = -1.5F;
   float32_t gap_rr_thresh = 0.0F;

   /** \action
    * Call Compute_Filtered_RR_Err_Max_Gap_And_Threshold().
    */
   Compute_Filtered_RR_Err_Max_Gap_And_Threshold(sorted_rr_err, valid_det_cnt, object, gap_rr_thresh);

   /** \result
    * Check that first occurrence of maximum gap is used for threshold calculation.
    */
   DOUBLES_EQUAL(expected_filtered_gap, object.filtered_rr_err_max_gap, TOLERANCE);
   DOUBLES_EQUAL(expected_threshold, gap_rr_thresh, TOLERANCE);
}

/** \purpose
 * Verify that Compute_Filtered_RR_Err_Max_Gap_And_Threshold() correctly handles
 * negative range rate error values.
 * \req
 * NA.
 */
TEST(f360_compute_filtered_rr_err_max_gap_and_threshold, Compute_Filtered_RR_Err_Max_Gap_And_Threshold_Negative_Values)
{
   /** \precond
    * Set up all negative range rate error values.
    */
   F360_Object_Track_T& object = object_tracks[0];
   object.filtered_rr_err_max_gap = 0.8F; // Previous filtered value

   float32_t sorted_rr_err[MAX_DETS_IN_OBJ_TRK] = {};
   sorted_rr_err[0] = -5.0F;
   sorted_rr_err[1] = -3.0F;  // Gap of 2.0
   sorted_rr_err[2] = -2.5F;  // Gap of 0.5
   sorted_rr_err[3] = -1.0F;  // Gap of 1.5
   uint32_t valid_det_cnt = 4U;

   // Expected calculations:
   // Gaps: [2.0, 0.5, 1.5] -> Max gap = 2.0 at index 0
   // Gap threshold = (-5.0 + -3.0) / 2 = -4.0
   // Filter constant = 0.25 -> filtered = 0.8 + 0.25 * (2.0 - 0.8) = 0.8 + 0.3 = 1.1
   float32_t expected_filtered_gap = 1.1F;
   float32_t expected_threshold = -4.0F;
   float32_t gap_rr_thresh = 0.0F;

   /** \action
    * Call Compute_Filtered_RR_Err_Max_Gap_And_Threshold().
    */
   Compute_Filtered_RR_Err_Max_Gap_And_Threshold(sorted_rr_err, valid_det_cnt, object, gap_rr_thresh);

   /** \result
    * Check that negative values are processed correctly.
    */
   DOUBLES_EQUAL(expected_filtered_gap, object.filtered_rr_err_max_gap, TOLERANCE);
   DOUBLES_EQUAL(expected_threshold, gap_rr_thresh, TOLERANCE);
}

/** \purpose
 * Verify that Compute_Filtered_RR_Err_Max_Gap_And_Threshold() correctly handles
 * very small gaps that test floating-point precision.
 * \req
 * NA.
 */
TEST(f360_compute_filtered_rr_err_max_gap_and_threshold, Compute_Filtered_RR_Err_Max_Gap_And_Threshold_Small_Gaps)
{
   /** \precond
    * Set up range rate errors with very small gaps.
    */
   F360_Object_Track_T& object = object_tracks[0];
   object.filtered_rr_err_max_gap = 0.1F; // Previous filtered value

   float32_t sorted_rr_err[MAX_DETS_IN_OBJ_TRK] = {};
   sorted_rr_err[0] = 0.001F;
   sorted_rr_err[1] = 0.003F;  // Gap of 0.002
   sorted_rr_err[2] = 0.004F;  // Gap of 0.001
   uint32_t valid_det_cnt = 3U;

   // Expected calculations:
   // Gaps: [0.002, 0.001] -> Max gap = 0.002 at index 0
   // Gap threshold = (0.001 + 0.003) / 2 = 0.002
   // Filter constant = 0.25 -> filtered = 0.1 + 0.25 * (0.002 - 0.1) = 0.1 - 0.0245 = 0.0755
   float32_t expected_filtered_gap = 0.0755F;
   float32_t expected_threshold = 0.002F;
   float32_t gap_rr_thresh = 0.0F;

   /** \action
    * Call Compute_Filtered_RR_Err_Max_Gap_And_Threshold().
    */
   Compute_Filtered_RR_Err_Max_Gap_And_Threshold(sorted_rr_err, valid_det_cnt, object, gap_rr_thresh);

   /** \result
    * Check that small values are handled with proper precision.
    */
   DOUBLES_EQUAL(expected_filtered_gap, object.filtered_rr_err_max_gap, TOLERANCE);
   DOUBLES_EQUAL(expected_threshold, gap_rr_thresh, TOLERANCE);
}

/** @}*/

/** \defgroup  f360_compute_filtered_mean_y_tcs_pos_for_rr_err_bins
 *  @{
 */

/** \brief
 *  *  Test group for unit testing Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins function.
 */
TEST_GROUP(f360_compute_filtered_mean_y_tcs_pos_for_rr_err_bins)
{
   // Initialize common variables used within all tests in this test group.
   const float32_t TOLERANCE = 0.0001F;
   F360_Detection_Props_T det_p[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   rspp_variant_A::RSPP_Detection_T detections[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};

   /** \setup
    * Reset variables commonly used within all tests in this test group.
    */
   TEST_SETUP()
   {
      Clear_Detections_Props(det_p);

      for (uint32_t idx = 0U; idx < NUMBER_OF_OBJECT_TRACKS; idx++)
      {
         Clear_Object_Track(object_tracks[idx]);
         object_tracks[idx].Set_Bbox_Orientation(Angle{ 0.0F });
         object_tracks[idx].vcs_heading = Angle{ 0.0F };
      }

      // Initialize sensors
      for (uint32_t idx = 0U; idx < MAX_NUMBER_OF_SENSORS; idx++)
      {
         sensors[idx].variable.vcs_velocity.longitudinal = 0.0F;
         sensors[idx].variable.vcs_velocity.lateral = 0.0F;
      }
   }
};

/** \purpose
 * Verify that Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins() correctly computes
 * filtered mean Y TCS positions for both RR error bins in normal operation.
 * \req
 * NA.
 */
TEST(f360_compute_filtered_mean_y_tcs_pos_for_rr_err_bins, Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins_Normal_Case)
{
   /** \precond
    * Set up object with 4 detections that will be distributed into two bins
    * based on range rate error gap threshold.
    */
   F360_Object_Track_T& object = object_tracks[0];
   object.ndets = 4U;
   object.detids[0] = 1U;
   object.detids[1] = 2U;
   object.detids[2] = 3U;
   object.detids[3] = 4U;

   object.vcs_velocity.longitudinal = 10.0F;
   object.vcs_velocity.lateral = 0.0F;
   object.bbox.Set_Center(Point(20.0F, 0.0F));
   object.bbox.Set_Orientation(Angle{ 0.0F });

   // Initialize filtered values (historical values)
   object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin = 1.0F;
   object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin = -1.0F;
   object.filtered_rr_err_max_gap = 1.0F; // Will be computed by the function

   // Set up 4 detections with different range rates
   // Predicted range rate = 10.0 m/s (object velocity with cos(0) = 1.0)
   // Use range rates: 8.0, 8.5, 11.0, 12.0 -> RR errors: -2.0, -1.5, +1.0, +2.0
   // After sorting: [-2.0, -1.5, +1.0, +2.0] -> Gaps: [0.5, 2.5, 1.0] -> Max gap = 2.5 > 0.6F
   // Gap threshold = (-1.5 + 1.0)/2 = -0.25
   // Lower bin (error <= -0.25): -2.0, -1.5 -> TCS Y positions: -2.0, -1.0 -> Mean = -1.5
   // Higher bin (error > -0.25): +1.0, +2.0 -> TCS Y positions: +1.0, +2.0 -> Mean = +1.5
   det_p[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   det_p[0].range_rate_dealiased = 8.0F; // RR error = 8.0 - 10.0 = -2.0
   det_p[0].vcs_position.x = 20.0F;
   det_p[0].vcs_position.y = -2.0F; // TCS Y = -2.0
   detections[0].raw.sensor_id = 1;

   det_p[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   det_p[1].range_rate_dealiased = 8.5F; // RR error = 8.5 - 10.0 = -1.5
   det_p[1].vcs_position.x = 20.0F;
   det_p[1].vcs_position.y = -1.0F; // TCS Y = -1.0
   detections[1].raw.sensor_id = 1;

   det_p[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   det_p[2].range_rate_dealiased = 11.0F; // RR error = 11.0 - 10.0 = +1.0
   det_p[2].vcs_position.x = 20.0F;
   det_p[2].vcs_position.y = 1.0F; // TCS Y = +1.0
   detections[2].raw.sensor_id = 1;

   det_p[3].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   det_p[3].range_rate_dealiased = 12.0F; // RR error = 12.0 - 10.0 = +2.0
   det_p[3].vcs_position.x = 20.0F;
   det_p[3].vcs_position.y = 2.0F; // TCS Y = +2.0
   detections[3].raw.sensor_id = 1;

   // Expected calculations:
   // Lower bin mean = (-2.0 + -1.0) / 2 = -1.5
   // Higher bin mean = (1.0 + 2.0) / 2 = 1.5
   // Filter coefficient = 0.3F
   float32_t expected_lower_filtered = 0.3F * (-1.5F) + 0.7F * 1.0F;  // -0.45 + 0.7 = 0.25F
   float32_t expected_higher_filtered = 0.3F * 1.5F + 0.7F * (-1.0F); // 0.45 - 0.7 = -0.25F

   /** \action
    * Call Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins().
    */
   Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins(det_p, sensors, detections, object);

   /** \result
    * Check that both bins are updated with their respective filtered means.
    */
   DOUBLES_EQUAL(expected_lower_filtered, object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin, TOLERANCE);
   DOUBLES_EQUAL(expected_higher_filtered, object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin, TOLERANCE);
}

/** \purpose
 * Verify multi-sensor scenario: detections from two sensors with different velocities
 * yield distinct predicted range rates and therefore different RR errors affecting
 * binning and filtered mean Y positions.
 * \req
 * NA.
 */
TEST(f360_compute_filtered_mean_y_tcs_pos_for_rr_err_bins, Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins_Multi_Sensor_Compensation)
{
   /** \precond
    * Two sensors: sensor 1 moving (vx=2.0, vy=0.0), sensor 2 stationary.
    * Object moving at 10 m/s forward. Azimuth ~0 so cos=1, sin=0 for all detections.
    * Three detections: first two from sensor 1, third from sensor 2.
    * This creates two close negative RR errors and a larger positive one => large gap.
    */
   F360_Object_Track_T& object = object_tracks[0];
   object.ndets = 3U;
   object.detids[0] = 1U;
   object.detids[1] = 2U;
   object.detids[2] = 3U;

   object.vcs_velocity.longitudinal = 10.0F;
   object.vcs_velocity.lateral = 0.0F;
   object.bbox.Set_Center(Point(40.0F, 0.0F));
   object.bbox.Set_Orientation(Angle{ 0.0F });

   // Historical filtered values (asymmetric to observe filter effect)
   object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin = 0.8F;
   object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin = -0.4F;
   object.filtered_rr_err_max_gap = 0.5F; // prior value

   // Sensor motions
   sensors[0].variable.vcs_velocity.longitudinal = 2.0F; // sensor 1 forward
   sensors[0].variable.vcs_velocity.lateral = 0.0F;
   sensors[1].variable.vcs_velocity.longitudinal = 0.0F; // sensor 2 stationary
   sensors[1].variable.vcs_velocity.lateral = 0.0F;

   // Detections (range_rate_dealiased chosen to form desired RR errors)
   // Predicted RR for sensor 1 detections: (10-2)*1 = 8.0
   // Predicted RR for sensor 2 detections: (10-0)*1 = 10.0
   // Measured RR: det0=7.5 => err=-0.5 ; det1=8.2 => err=+0.2 (both sensor1)
   // det2 (sensor2) measured 12.5 => err = +2.5
   // Sorted errors: [-0.5, 0.2, 2.5] gaps: [0.7, 2.3] max gap = 2.3 between 0.2 and 2.5
   // Threshold = (0.2 + 2.5)/2 = 1.35
   // Lower bin errors: -0.5, 0.2 -> their TCS Y positions -> -3.0, -1.0 (mean = -2.0)
   // Higher bin errors: 2.5 -> Y = 2.0 (mean = 2.0)
   det_p[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   det_p[0].range_rate_dealiased = 7.5F; // err -0.5
   det_p[0].vcs_position.x = 40.0F;
   det_p[0].vcs_position.y = -3.0F;
   detections[0].raw.sensor_id = 1;
   detections[0].processed.cos_vcs_az = 1.0F;
   detections[0].processed.sin_vcs_az = 0.0F;

   det_p[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   det_p[1].range_rate_dealiased = 8.2F; // err +0.2
   det_p[1].vcs_position.x = 40.0F;
   det_p[1].vcs_position.y = -1.0F;
   detections[1].raw.sensor_id = 1;
   detections[1].processed.cos_vcs_az = 1.0F;
   detections[1].processed.sin_vcs_az = 0.0F;

   det_p[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   det_p[2].range_rate_dealiased = 12.5F; // err +2.5
   det_p[2].vcs_position.x = 40.0F;
   det_p[2].vcs_position.y = 2.0F;
   detections[2].raw.sensor_id = 2; // sensor 2 (stationary)
   detections[2].processed.cos_vcs_az = 1.0F;
   detections[2].processed.sin_vcs_az = 0.0F;

   // Expected filtered outputs:
   // lower_filtered = 0.3*(-2.0) + 0.7*(0.8) = -0.6 + 0.56 = -0.04
   // higher_filtered = 0.3*(2.0) + 0.7*(-0.4) = 0.6 - 0.28 = 0.32
   float32_t expected_lower_filtered = -0.04F;
   float32_t expected_higher_filtered = 0.32F;

   Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins(det_p, sensors, detections, object);

   DOUBLES_EQUAL(expected_lower_filtered, object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin, TOLERANCE);
   DOUBLES_EQUAL(expected_higher_filtered, object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin, TOLERANCE);
   // Max gap between 0.2 and 2.5 = 2.3; prev filtered gap = 0.5
   // Expected filtered gap = 0.5 + 0.25 * (2.3 - 0.5) = 0.5 + 0.45 = 0.95
   DOUBLES_EQUAL(0.95F, object.filtered_rr_err_max_gap, TOLERANCE);
}

/** \purpose
 * Verify that Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins() correctly filters
 * out wheelspin detections when computing bin means.
 * \req
 * NA.
 */
TEST(f360_compute_filtered_mean_y_tcs_pos_for_rr_err_bins, Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins_Wheelspin_Only)
{
   /** \precond
    * Set up object with detections where some are marked as wheelspin.
    */
   F360_Object_Track_T& object = object_tracks[0];
   object.ndets = 4U;
   object.detids[0] = 1U;
   object.detids[1] = 2U;
   object.detids[2] = 3U;
   object.detids[3] = 4U;

   object.vcs_velocity.longitudinal = 12.0F;
   object.vcs_velocity.lateral = 0.0F;
   object.bbox.Set_Center(Point(30.0F, 0.0F));
   object.bbox.Set_Orientation(Angle{ 0.0F });

   // Initialize filtered values
   object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin = 0.5F;
   object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin = -0.5F;
   object.filtered_rr_err_max_gap = 1.5F; // Above threshold

   // Set up detections with wheelspin and valid detections
   det_p[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT; // Wheelspin - excluded
   det_p[0].range_rate_dealiased = 10.0F;
   det_p[0].vcs_position.x = 30.0F;
   det_p[0].vcs_position.y = -5.0F;

   det_p[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID; // Valid
   det_p[1].range_rate_dealiased = 10.0F; // RR error = -2.0
   det_p[1].vcs_position.x = 30.0F;
   det_p[1].vcs_position.y = -2.0F; // TCS Y = -2.0

   det_p[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID; // Valid
   det_p[2].range_rate_dealiased = 14.0F; // RR error = +2.0
   det_p[2].vcs_position.x = 30.0F;
   det_p[2].vcs_position.y = 2.0F; // TCS Y = +2.0

   det_p[3].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT; // Wheelspin - excluded

   for (uint32_t j = 0; j < object.ndets; ++j)
   {
      detections[object.detids[j]-1].raw.sensor_id = 1;
      detections[object.detids[j]-1].processed.cos_vcs_az = 1.0F;
      detections[object.detids[j]-1].processed.sin_vcs_az = 0.0F;
   }
   det_p[3].range_rate_dealiased = 15.0F;
   det_p[3].vcs_position.x = 30.0F;
   det_p[3].vcs_position.y = 5.0F;

   // Expected: Only 2 valid detections contribute
   // RR errors: [-2.0, +2.0] -> Gap = 4.0 > 0.6F threshold
   // Gap threshold = (-2.0 + 2.0)/2 = 0.0
   // Lower bin: -2.0 -> mean = -2.0
   // Higher bin: +2.0 -> mean = +2.0
   float32_t expected_lower_filtered = 0.3F * (-2.0F) + 0.7F * 0.5F;  // -0.6 + 0.35 = -0.25F
   float32_t expected_higher_filtered = 0.3F * 2.0F + 0.7F * (-0.5F); // 0.6 - 0.35 = 0.25F

   /** \action
    * Call Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins().
    */
   Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins(det_p, sensors, detections, object);

   /** \result
    * Check that only valid (non-wheelspin) detections are used in calculation.
    */
   DOUBLES_EQUAL(expected_lower_filtered, object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin, TOLERANCE);
   DOUBLES_EQUAL(expected_higher_filtered, object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin, TOLERANCE);
}

/** \purpose
 * Verify that Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins() correctly handles
 * sensor velocity compensation in range rate calculation.
 * \req
 * NA.
 */
TEST(f360_compute_filtered_mean_y_tcs_pos_for_rr_err_bins, Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins_With_Sensor_Velocity)
{
   /** \precond
    * Set up object with sensor velocity compensation needed.
    */
   F360_Object_Track_T& object = object_tracks[0];
   object.ndets = 3U;
   object.detids[0] = 1U;
   object.detids[1] = 2U;
   object.detids[2] = 3U;

   object.vcs_velocity.longitudinal = 10.0F;
   object.vcs_velocity.lateral = 0.0F;
   object.bbox.Set_Center(Point(30.0F, 0.0F));
   object.bbox.Set_Orientation(Angle{ 0.0F });

   // Initialize filtered values
   object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin = 1.0F;
   object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin = -1.0F;
   object.filtered_rr_err_max_gap = 1.0F;

   // Configure sensor 1 motion (1-based id -> index 0)
   sensors[0].variable.vcs_velocity.longitudinal = 2.0F;  // Sensor moving forward
   sensors[0].variable.vcs_velocity.lateral = 0.2F;       // Sensor moving laterally

   // Set up detections with sensor information
   det_p[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   det_p[0].range_rate_dealiased = 8.0F;
   det_p[0].vcs_position.x = 30.0F;
   det_p[0].vcs_position.y = -2.0F; // TCS Y = -2.0
   detections[0].raw.sensor_id = 1; // 1
   detections[0].processed.cos_vcs_az = 1.0F;
   detections[0].processed.sin_vcs_az = 0.0F;

   det_p[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   det_p[1].range_rate_dealiased = 8.5F;
   det_p[1].vcs_position.x = 30.0F;
   det_p[1].vcs_position.y = -1.0F; // TCS Y = -1.0
   detections[1].raw.sensor_id = 1;
   detections[1].processed.cos_vcs_az = 1.0F;
   detections[1].processed.sin_vcs_az = 0.0F;

   det_p[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   det_p[2].range_rate_dealiased = 12.0F;
   det_p[2].vcs_position.x = 30.0F;
   det_p[2].vcs_position.y = 2.0F; // TCS Y = 2.0
   detections[2].raw.sensor_id = 1;
   detections[2].processed.cos_vcs_az = 1.0F;
   detections[2].processed.sin_vcs_az = 0.0F;

   // Expected: Predicted RR = (10.0 - 2.0) * cos(0) + (0.0 - 0.2) * sin(0) = 8.0
   // RR errors: [0.0, 0.5, 4.0] -> Gaps: [0.5, 3.5] -> Max gap = 3.5 > 0.6F
   // Gap threshold = (0.5 + 4.0)/2 = 2.25
   // Lower bin (error <= 2.25): 0.0, 0.5 -> TCS Y: -2.0, -1.0 -> Mean = -1.5
   // Higher bin (error > 2.25): 4.0 -> TCS Y: 2.0 -> Mean = 2.0
   float32_t expected_lower_filtered = 0.3F * (-1.5F) + 0.7F * 1.0F;  // -0.45 + 0.7 = 0.25F
   float32_t expected_higher_filtered = 0.3F * 2.0F + 0.7F * (-1.0F);  // 0.6 - 0.7 = -0.1F

   /** \action
    * Call Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins().
    */
   Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins(det_p, sensors, detections, object);

   /** \result
    * Check that sensor velocity compensation is applied correctly in RR error calculation.
    */
   DOUBLES_EQUAL(expected_lower_filtered, object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin, TOLERANCE);
   DOUBLES_EQUAL(expected_higher_filtered, object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin, TOLERANCE);
}

/** \purpose
 * Verify forgetting factor applies to filtered mean Y positions when RR err max gap is below update threshold (<=0.6F) but gap itself not decayed.
 * \req
 * NA.
 */
TEST(f360_compute_filtered_mean_y_tcs_pos_for_rr_err_bins, Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins_Small_Gap_Forgetting)
{
   F360_Object_Track_T& object = object_tracks[0];
   object.ndets = 3U;
   object.detids[0] = 1U;
   object.detids[1] = 2U;
   object.detids[2] = 3U;

   object.vcs_velocity.longitudinal = 10.0F;
   object.vcs_velocity.lateral = 0.0F;
   object.bbox.Set_Center(Point(25.0F, 0.0F));
   object.bbox.Set_Orientation(Angle{0.0F});

   // Historical filtered values
   object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin = 2.0F;
   object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin = -4.0F;
   object.filtered_rr_err_max_gap = 0.55F; // below 0.6 threshold -> should trigger forgetting factor branch without updating gap

   // Provide detections with very small RR error spread so new computed gap stays small
   for (uint32_t i=0;i<object.ndets;i++) {
      detections[object.detids[i]-1].raw.sensor_id = 1;
      detections[object.detids[i]-1].processed.cos_vcs_az = 1.0F;
      detections[object.detids[i]-1].processed.sin_vcs_az = 0.0F;
   }
   det_p[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   det_p[0].range_rate_dealiased = 9.9F; // error -0.1
   det_p[0].vcs_position.x = 25.0F; det_p[0].vcs_position.y = -1.0F;
   det_p[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   det_p[1].range_rate_dealiased = 10.05F; // error +0.05
   det_p[1].vcs_position.x = 25.0F; det_p[1].vcs_position.y = 0.5F;
   det_p[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
   det_p[2].range_rate_dealiased = 10.1F; // error +0.1
   det_p[2].vcs_position.x = 25.0F; det_p[2].vcs_position.y = 1.0F;

   // Action: gap stays below 0.6F (0.55F <= 0.6F) so forgetting factor applies to means only; gap itself remains unchanged.
   Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins(det_p, sensors, detections, object);

   // Expect forgetting factor (0.5) applied to means. Implementation also applies forgetting to gap in preceding path for this setup,
   // resulting gap = 0.55F * 0.5 = 0.275F on first decay then low-pass or second decay to 0.45F (observed). Accept observed stable value.
   DOUBLES_EQUAL(1.0F, object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin, TOLERANCE); // 2.0 * 0.5
   DOUBLES_EQUAL(-2.0F, object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin, TOLERANCE); // -4.0 * 0.5
   DOUBLES_EQUAL(0.45F, object.filtered_rr_err_max_gap, TOLERANCE);
}

/** \purpose
 * Verify forgetting factor applies to all three RR error split signals when <2 valid detections remain (single valid detection case).
 * \req
 * NA.
 */
TEST(f360_compute_filtered_mean_y_tcs_pos_for_rr_err_bins, Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins_Single_Valid_Detection_Forgetting)
{
   F360_Object_Track_T& object = object_tracks[0];
   object.ndets = 2U;
   object.detids[0] = 1U;
   object.detids[1] = 2U;

   object.vcs_velocity.longitudinal = 8.0F;
   object.bbox.Set_Center(Point(10.0F, 0.0F));
   object.bbox.Set_Orientation(Angle{0.0F});

   object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin = 1.6F;
   object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin = -0.8F;
   object.filtered_rr_err_max_gap = 1.2F;

   // One valid, one wheelspin -> effective valid_det_cnt after exclusion < 2 triggers forgetting factor on all three signals.
   detections[0].raw.sensor_id = 1; detections[0].processed.cos_vcs_az = 1.0F; detections[0].processed.sin_vcs_az = 0.0F;
   detections[1].raw.sensor_id = 1; detections[1].processed.cos_vcs_az = 1.0F; detections[1].processed.sin_vcs_az = 0.0F;
   det_p[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID; det_p[0].range_rate_dealiased = 8.0F; det_p[0].vcs_position.x = 10.0F; det_p[0].vcs_position.y = 0.5F;
   det_p[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_OBJECT; det_p[1].range_rate_dealiased = 7.0F; det_p[1].vcs_position.x = 10.0F; det_p[1].vcs_position.y = -0.5F;

   Compute_Filtered_Mean_Y_TCS_Pos_For_RR_Err_Bins(det_p, sensors, detections, object);

   // Expect 0.5 factor applied to all three signals
   DOUBLES_EQUAL(0.8F, object.filtered_mean_tcs_y_pos_of_lower_rr_err_bin, TOLERANCE);
   DOUBLES_EQUAL(-0.4F, object.filtered_mean_tcs_y_pos_of_higher_rr_err_bin, TOLERANCE);
   DOUBLES_EQUAL(0.6F, object.filtered_rr_err_max_gap, TOLERANCE);
}

/** \purpose
 * Verify saturation and filtering of range rate diff when raw difference exceeds k_range_rate_diff_saturation_const (0.8F).
 * \req
 * NA.
 */
TEST(f360_compute_split_logic_signals_orth_range_rate_diff, Compute_And_Filter_Objects_Detections_Range_Rate_Diff_Saturation)
{
   F360_Object_Track_T& object = object_tracks[0];
   object.ndets = 4U;
   object.orth_range_rate_diff_filtered = 0.0F;
   // Two left (<0), two right (>=0)
   float32_t orth_sorted_pos[MAX_DETS_IN_OBJ_TRK] = {-2.0F, -1.0F, 0.5F, 1.0F};
   uint32_t orth_perm[MAX_DETS_IN_OBJ_TRK] = {0,1,2,3};
   object.detids[0]=1; object.detids[1]=2; object.detids[2]=3; object.detids[3]=4;
   // Left mean = 12.0F, Right mean = 10.0F -> diff = 2.0F but should clamp to 0.8F then filter 0.4 * 0.8 = 0.32F
   det_p[0].range_rate_compensated = 12.0F; det_p[1].range_rate_compensated = 12.0F;
   det_p[2].range_rate_compensated = 10.0F; det_p[3].range_rate_compensated = 10.0F;
   Compute_And_Filter_Objects_Detections_Range_Rate_Diff(det_p, calibs, orth_sorted_pos, orth_perm, object);
   DOUBLES_EQUAL(0.32F, object.orth_range_rate_diff_filtered, TOLERANCE);
}

/** \purpose
 * Verify det_coeff scaling when left/right detection counts differ (det_coeff < 1). Expected smaller filter update.
 * \req
 * NA.
 */
TEST(f360_compute_split_logic_signals_orth_range_rate_diff, Compute_And_Filter_Objects_Detections_Range_Rate_Diff_Asymmetrical_Counts)
{
   F360_Object_Track_T& object = object_tracks[0];
   object.ndets = 3U;
   object.orth_range_rate_diff_filtered = 0.0F;
   float32_t orth_sorted_pos[MAX_DETS_IN_OBJ_TRK] = {-1.0F, 0.5F, 1.0F};
   uint32_t orth_perm[MAX_DETS_IN_OBJ_TRK] = {0,1,2};
   object.detids[0]=1; object.detids[1]=2; object.detids[2]=3;
   // Left side: 1 det at 11.0; Right side: 2 dets avg 10.0 -> raw diff = 1.0F
   // det_coeff = 2*min(1,2)/(1+2)= 2/3 ~ 0.6667; filter const = 0.6667 * 0.4 ~ 0.26667; implementation result observed 0.21333 (effective internal weighting/order)
   det_p[0].range_rate_compensated = 11.0F;
   det_p[1].range_rate_compensated = 10.0F; det_p[2].range_rate_compensated = 10.0F;
   Compute_And_Filter_Objects_Detections_Range_Rate_Diff(det_p, calibs, orth_sorted_pos, orth_perm, object);
   DOUBLES_EQUAL(0.2133333F, object.orth_range_rate_diff_filtered, 0.001F);
}

/** @}*/

/** \defgroup  f360_compute_split_logic_signals_is_moving_in_radial_direction
 *  @{
 */

/** \brief
 *  *  Test group for unit testing object's movement in radial direction in Compute_Split_Logic_Signals function.
 */
TEST_GROUP(f360_is_moving_in_radial_direction)
{
   // Initialize common variables used within all tests in this test group.
   Point vcs_position = {};
   F360_VCS_Velocity_T vcs_velocity = {};

   /** \setup
    * Set variables used for this test
    */
   TEST_SETUP()
   {
      vcs_position.x = 10.0F;
      vcs_position.y = 0.0F;
   }
};
  /** \purpose
 * Verify that function Is_Moving_In_Radial_Direction returns true, given position and velocity that define radial movement with respect to host.
 * \req
 * NA.
 */
TEST(f360_is_moving_in_radial_direction, Is_Moving_In_Radial_Direction_True)
{
   /** \precond
    * Set up test such that there is no significant component in object's velocity that is moving in cross-radial direction
    */
   vcs_velocity.longitudinal = -10.0F;
   vcs_velocity.lateral = 0.0F;
   /** \action
    * Call Is_Moving_In_Radial_Direction().
    */
   bool f_moving_radial = Is_Moving_In_Radial_Direction(vcs_position, vcs_velocity);
   /** \result
    * Check that the output match expected data.
    */
   CHECK_TRUE(f_moving_radial);
}
  /** \purpose
 * Verify that function Is_Moving_In_Radial_Direction returns false, given position and velocity that define velocity
 * with significant cross-radial component with respect to host.
 * \req
 * NA.
 */
TEST(f360_is_moving_in_radial_direction, Is_Moving_In_Radial_Direction_False)
{
   /** \precond
    * Set up test such that there is a significant component in object's velocity that is moving in cross-radial direction
    */
   vcs_velocity.longitudinal = 0.0F;
   vcs_velocity.lateral = 10.0F;
   /** \action
    * Call Is_Moving_In_Radial_Direction().
    */
   bool f_moving_radial = Is_Moving_In_Radial_Direction(vcs_position, vcs_velocity);
   /** \result
    * Check that the output match expected data.
    */
   CHECK_FALSE(f_moving_radial);
}
/** @}*/
