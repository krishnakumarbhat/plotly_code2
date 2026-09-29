/** \file
   This file contains qualification tests for Track Classicication module.
*/

#include "f360_track_classification.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include "f360_math_func.h"
#include <cfloat>
#include <cstring>
#include "f360_object_motion_classification_helpers.h"
#include "f360_is_object_suspected_stationary.h"
#include "f360_trk_fltr_ctca_states.h"
#include "f360_trk_fltr_cca_states.h"
#include "f360_calculate_object_class_probabilities.h"
#include "f360_assign_underdrivability_status_to_tracks_ocg.h"
#include "f360_set_variant.h"
#include "f360_occlusion.h"

using namespace f360_variant_A;

/** \defgroup  f360_assign_underdrivability_status_to_tracks_ocg_QT
 *  @{
 */

/** \brief
 * Test group of Assign_Underdrivability_Status_To_Tracks. Tests verify whether tracks are properly
 * checked whether they can be marked as underdrivable, whether they do not have their underdrivability
 * status modified when are not out of zones FOV and whether they are properly assigned new underdrivability
 * status.
 *
 */
TEST_GROUP(f360_assign_underdrivability_status_to_tracks_ocg_QT)
{
   F360_TRKR_TIMING_INFO_T timing_info = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Calibrations_T calib;
   F360_OCG_INTERNAL_T ocg_internal;
   F360_Tracker_Info_T tracker_info = {};
   F360_Host_T host = {};
   ocg::OCG_Outputs_T occupancy_grid = {};

   TEST_SETUP()
   {
      // Set Calibration Properties
      Initialize_Tracker_Calibrations(calib);

      // Set Tracker info properties
      Set_Tracker_Variant(tracker_info.variant);

      // Set Host Properties
      host.dist_rear_axle_to_vcs_m = 3.53F;
      host.curvature_rear = 0.0F;
      host.yaw_rate_rad = 0.0F;

      // Set Occupancy Grid Properties
      occupancy_grid.timestamp = 6974010.000; // Arbitrary

      tracker_info.time_us = occupancy_grid.timestamp;

      occupancy_grid.f_valid = true;
      occupancy_grid.grid_definition.cell_length = 2.0F;
      occupancy_grid.grid_definition.cell_width = 6.0F;
      occupancy_grid.grid_definition.cell_width_extension_factor = 1.666666666F;
      occupancy_grid.grid_definition.num_cells_x_close = ocg::NUM_CELLS_X_CLOSE;
      occupancy_grid.grid_definition.num_cells_x_mid = ocg::NUM_CELLS_X_MID;
      occupancy_grid.grid_definition.num_cells_x_far = ocg::NUM_CELLS_X_FAR;
      occupancy_grid.grid_definition.num_cells_y = ocg::NUM_CELLS_Y;

      occupancy_grid.underdrivability.grid_curvature = host.curvature_rear;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.x = -host.dist_rear_axle_to_vcs_m;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.y = 0.0F;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.z = 0.0F;
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw = 0.0F;
      
      for (uint8_t i = 0U; i < ocg::NUM_CELLS_X; i++)
      {
         for (uint8_t j = 0U; j < ocg::NUM_CELLS_Y; j++)
         {
            occupancy_grid.underdrivability.underdrivability_classification[i][j].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
            occupancy_grid.underdrivability.underdrivability_classification[i][j].probs[0] = 0.0F;
            occupancy_grid.underdrivability.underdrivability_classification[i][j].probs[1] = 0.0F;
            occupancy_grid.underdrivability.underdrivability_classification[i][j].probs[2] = 0.0F;
            occupancy_grid.underdrivability.underdrivability_classification[i][j].probs[3] = 0.0F;
         }
      }

      // Make sure ocg_internal is filled with valid information
      (void) Preprocess_OCG(host, &occupancy_grid, static_cast<float32_t>(tracker_info.time_us), ocg_internal);
   }

   void Set_Object_Params_To_Make_It_Valid(
      F360_Object_Track_T& object,
      const F360_Calibrations_T& calib)
   {
      object.status = F360_OBJECT_STATUS_NEW;
      object.f_moving = false;
      object.movable_prob = 0.0F;
      object.vcs_position.x = calib.k_underdrive_min_trk_long_posn + 0.01F;
      object.vcs_position.y = (ocg_internal.ocg_max_grid_width_half * calib.k_underdrive_lat_buffer_factor) - 0.01F;
      object.probability_underdrivable_ocg = 0.0F;
   }
};

/** \purpose
* Purpose if this test is to verify whether object should be considered but its 
* position is outside zones FOV, its underdrivability status is set to UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
* it also check if the object's probability_underdrivable_ocg is correct  
* \req
* CPR-3833
*/
TEST(f360_assign_underdrivability_status_to_tracks_ocg_QT, Assign_Underdrivability_Status_To_Tracks__Object_Not_In_FOV)
{
   /** \precond
   * Set up object parameters to make it valid.
   * Set up its status to UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
   * Set object VCS position to be outside zones FOV.
   * Set the expected_probability_underdrivable_ocg to comapre with results of the tested method
   */
   const float32_t expected_probability_underdrivable_ocg = 0.0F;

   Set_Object_Params_To_Make_It_Valid(object_tracks[0], calib);
   object_tracks[0].underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
   object_tracks[0].vcs_position.x = calib.k_underdrive_min_zone_long_posn - 0.01F;

   /** \action
   * Call tested function.
   */
   Assign_Underdrivability_Status_To_Stationary_Object_OCG(calib, occupancy_grid, ocg_internal, host, object_tracks[0], timing_info);


   /** \result
   * Check whether object status was not changed.
   * Check if probability underdrivable is correct
   */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER, object_tracks[0].underdrivable_status_ocg);
   DOUBLES_EQUAL(expected_probability_underdrivable_ocg, object_tracks[0].probability_underdrivable_ocg, F360_EPSILON);
}

/** \purpose
* Purpose if this test is to verify whether when current object status is UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
* but new one is UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER - status is not lowered.
* it also check if the object's probability_underdrivable_ocg is correct
* \req
* CPR-3833
*/
TEST(f360_assign_underdrivability_status_to_tracks_ocg_QT, Assign_Underdrivability_Status_To_Tracks__Current_Status_Can_Pass_Under_New_One_Is_Likely_To_Pass)
{
   /** \precond
   * Pick zone where track should be placed.
   * Verify whether zone is not outside limits.
   * Set up object parameters to make it valid.
   * Set up its status to UNDERDRIVABLE_STATUS_CAN_PASS_UNDER.
   * Set status of zone where object is located to UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER.
   */
   const uint16_t zone_long_idx = 10U;
   const uint16_t zone_lat_idx = 0U;
   const float32_t expected_probability_underdrivable_ocg = 0.75F;
   
   // Check verifies whether selected zone is not out of limits.
   CHECK_TRUE_TEXT(zone_long_idx < ocg::NUM_CELLS_X, "Selected Longitudinal zone is greater than total number of longitudinal zones, modify test.");
   CHECK_TRUE_TEXT(zone_lat_idx < ocg::NUM_CELLS_Y, "Selected Lateral zone is greater than total number of lateral zones, modify test.");
  
   Set_Object_Params_To_Make_It_Valid(object_tracks[0], calib);
   object_tracks[0].underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
   object_tracks[0].vcs_position.x = static_cast<float32_t>(zone_long_idx) * occupancy_grid.grid_definition.cell_length;
   object_tracks[0].vcs_position.y = static_cast<float32_t>(zone_lat_idx) * occupancy_grid.grid_definition.cell_width * 0.5F - 0.1F;
   object_tracks[0].probability_underdrivable_ocg = expected_probability_underdrivable_ocg;

   occupancy_grid.underdrivability.underdrivability_classification[zone_long_idx][zone_lat_idx].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER;
   
   
   /** \action
   * Call tested function.
   */
   Assign_Underdrivability_Status_To_Stationary_Object_OCG(calib, occupancy_grid, ocg_internal, host, object_tracks[0], timing_info);

   /** \result
   * Check whether object status was not changed.
   * Check if probability underdrivable is correct
   */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER, object_tracks[0].underdrivable_status_ocg);
   DOUBLES_EQUAL(expected_probability_underdrivable_ocg, object_tracks[0].probability_underdrivable_ocg, F360_EPSILON);
}

/** \purpose
* Purpose if this test is to verify whether when current object status is UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
* and new one is UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER - status is lowered.
* it also check if the object's probability_underdrivable_ocg is correct
* \req
* CPR-3833
*/
TEST(f360_assign_underdrivability_status_to_tracks_ocg_QT, Assign_Underdrivability_Status_To_Tracks__Current_Status_Can_Pass_Under_New_One_Can_Not_Pass)
{
   /** \precond
   * Pick zone where track should be placed.
   * Verify whether zone is not outside limits.
   * Set up object parameters to make it valid.
   * Set up its status to UNDERDRIVABLE_STATUS_CAN_PASS_UNDER.
   * Set status of zone where object is located to UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER.
   * Set probabilities associated with the zones 0 to 3 to 0.5F, 0.2F, 0.1F, 0.3F respectively
   * Set expected_probability_underdrivable_ocg to 0.3 since this is the probability that should be calculated
   * for the target object 
   * The object is located in the cell with longitudinal index 15. The status and probabilities
   * should be takes from the cell where the object is and not an neighbouring cell (second picked).
   *
   * To insure that final test result neighbouring cells are set to invalid statuses.
   */
   const uint16_t zone_long_idx = 15U;
   const uint16_t zone_lat_idx = 0U;
   const float32_t expected_probability_underdrivable_ocg = 0.3F;
   
   // Check verifies whether selected zone is not out of limits.
   CHECK_TRUE_TEXT(zone_long_idx < ocg::NUM_CELLS_X, "Selected Longitudinal zone is greater than total number of longitudinal zones, modify test.");
   CHECK_TRUE_TEXT(zone_lat_idx < ocg::NUM_CELLS_Y, "Selected Lateral zone is greater than total number of lateral zones, modify test.");
  
   Set_Object_Params_To_Make_It_Valid(object_tracks[0], calib);
   object_tracks[0].underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
   object_tracks[0].vcs_position.x = (static_cast<float32_t>(zone_long_idx) * occupancy_grid.grid_definition.cell_length) + occupancy_grid.grid_definition.cell_length/2;

   const float32_t extended_cell_width = 6.667F;
   object_tracks[0].vcs_position.y = (extended_cell_width * 0.5F) - 0.01F;

   occupancy_grid.underdrivability.underdrivability_classification[zone_long_idx - 1][zone_lat_idx].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
   occupancy_grid.underdrivability.underdrivability_classification[zone_long_idx + 1][zone_lat_idx].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;

   occupancy_grid.underdrivability.underdrivability_classification[zone_long_idx][zone_lat_idx].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
   occupancy_grid.underdrivability.underdrivability_classification[zone_long_idx][zone_lat_idx].probs[0] = 0.5F;
   occupancy_grid.underdrivability.underdrivability_classification[zone_long_idx][zone_lat_idx].probs[1] = 0.2F;
   occupancy_grid.underdrivability.underdrivability_classification[zone_long_idx][zone_lat_idx].probs[2] = 0.1F;
   occupancy_grid.underdrivability.underdrivability_classification[zone_long_idx][zone_lat_idx].probs[3] = 0.3F;

   /** \action
   * Call tested function.
   */
   Assign_Underdrivability_Status_To_Stationary_Object_OCG(calib, occupancy_grid, ocg_internal, host, object_tracks[0], timing_info);

   /** \result
   * Check whether object status was changed to UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER.
   * Check whether object probability underdrivable was changed to to the expected value 
   */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER, object_tracks[0].underdrivable_status_ocg);
   DOUBLES_EQUAL(expected_probability_underdrivable_ocg, object_tracks[0].probability_underdrivable_ocg, F360_EPSILON);
}


/** \purpose
*  Purpose of this test is to verify whether when current object status is UNDERDRIVABLE_STATUS_NOT_CAN_PASS_UNDER
*  and the object is placed laterally outside ocupancy grid, object's status is changed to UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
*  it also check if the object's probability_underdrivable_ocg is correct
*  \req
*  CPR-3833
*/
TEST(f360_assign_underdrivability_status_to_tracks_ocg_QT, Assign_Underdrivability_Status_To_Tracks__Not_To_Consider_After_Status_Determined)
{
   /** \precond
   * Pick zone where track should be placed.
   * Verify whether zone is outside limits.
   * Set up object parameters to make it valid.
   * Set up its status to UNDERDRIVABLE_STATUS_NOT_CAN_PASS_UNDER.
   * Set expected_probability_underdrivable_ocg to 0 since this is the probability that should be calculated
   * for the target object 
   * 
   * The object is located in the cell with longitudinal index 14. The status and probabilities
   * are not relevant in this case since the object should not be considered for underdrivability check.
   */
   const uint16_t zone_long_idx = 14U;
   const uint16_t zone_lat_idx = 0U;
   const float32_t expected_probability_underdrivable_ocg = 0.0F;
   
   // Check verifies whether selected zone is not out of limits.
   CHECK_TRUE_TEXT(zone_long_idx < ocg::NUM_CELLS_X, "Selected Longitudinal zone is greater than total number of longitudinal zones, modify test.");
   CHECK_TRUE_TEXT(zone_lat_idx < ocg::NUM_CELLS_Y, "Selected Lateral zone is greater than total number of lateral zones, modify test.");
  
   Set_Object_Params_To_Make_It_Valid(object_tracks[0], calib);
   object_tracks[0].underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
   object_tracks[0].vcs_position.x = (static_cast<float32_t>(zone_long_idx) * occupancy_grid.grid_definition.cell_length);
   object_tracks[0].probability_underdrivable_ocg = 2.0F;

   const float32_t extended_cell_width = 6.667F;
   object_tracks[0].vcs_position.y = (extended_cell_width * 0.5F) - 0.01F;

   /** \action
   * Call tested function.
   */
   Assign_Underdrivability_Status_To_Stationary_Object_OCG(calib, occupancy_grid, ocg_internal, host, object_tracks[0], timing_info);

   /** \result
   * Check whether object status was changed to UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER.
   * Check whether object probability underdrivable was changed to to the expected value
   */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER, object_tracks[0].underdrivable_status_ocg);
   DOUBLES_EQUAL(expected_probability_underdrivable_ocg, object_tracks[0].probability_underdrivable_ocg, F360_EPSILON);
}

/** \purpose
* Purpose if this test is to verify whether when current object status is UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
* and new one is UNDERDRIVABLE_STATUS_CAN_PASS_UNDER - status is increased.
* it also check if the object's probability_underdrivable_ocg is correct
* \req
* CPR-3833
*/
TEST(f360_assign_underdrivability_status_to_tracks_ocg_QT, Assign_Underdrivability_Status_To_Tracks__Current_Status_Can_Not_Pass_New_Can_Pass)
{
   /** \precond
   * Pick zone where track should be placed.
   * Verify whether zone is not outside limits.
   * Set up 2 object parameters to make it valid.
   * Set up its status to UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER.
   * Set status of zone where object is located to UNDERDRIVABLE_STATUS_CAN_PASS_UNDER.
   * Set probabilities associated with the zones 0 to 3 to 0.2F, 0.1F, 0.5F, 0.3F respectively
   * Set expected_probability_underdrivable_ocg to 0.3 since this is the probability that should be calculated
   * for the target object 
   */
   const uint16_t zone_long_idx = 15U;
   const uint16_t zone_lat_idx = 0U;
   const float32_t expected_probability_underdrivable_ocg = 0.6F;
   
   // Check verifies whether selected zone is not out of limits.
   CHECK_TRUE_TEXT(zone_long_idx < ocg::NUM_CELLS_X, "Selected Longitudinal zone is greater than total number of longitudinal zones, modify test.");
   CHECK_TRUE_TEXT(zone_lat_idx < ocg::NUM_CELLS_Y, "Selected Lateral zone is greater than total number of lateral zones, modify test.");
  
   Set_Object_Params_To_Make_It_Valid(object_tracks[0], calib);
   object_tracks[0].underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
   object_tracks[0].vcs_position.x = 0.0F + static_cast<float32_t>(zone_long_idx) * occupancy_grid.grid_definition.cell_length;

   const float32_t extended_cell_width = 6.667F;
   object_tracks[0].vcs_position.y = (extended_cell_width * 0.5F) - 0.01F;

   occupancy_grid.underdrivability.underdrivability_classification[zone_long_idx][zone_lat_idx].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
   occupancy_grid.underdrivability.underdrivability_classification[zone_long_idx][zone_lat_idx].probs[0] = 0.2F;
   occupancy_grid.underdrivability.underdrivability_classification[zone_long_idx][zone_lat_idx].probs[1] = 0.1F;
   occupancy_grid.underdrivability.underdrivability_classification[zone_long_idx][zone_lat_idx].probs[2] = 0.5F;
   occupancy_grid.underdrivability.underdrivability_classification[zone_long_idx][zone_lat_idx].probs[3] = 0.3F;

   /** \action
   * Call tested function.
   */
   Assign_Underdrivability_Status_To_Stationary_Object_OCG(calib, occupancy_grid, ocg_internal, host, object_tracks[0], timing_info);

   /** \result
   * Check whether object status was changed to UNDERDRIVABLE_STATUS_CAN_PASS_UNDER.
   * Check whether object probability underdrivable was changed to to the expected value
   */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER, object_tracks[0].underdrivable_status_ocg);
   DOUBLES_EQUAL(expected_probability_underdrivable_ocg, object_tracks[0].probability_underdrivable_ocg, F360_EPSILON);
}

/** \purpose
* Purpose if this test is to verify whether when current object status is UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
* and new one is UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER - status is increased.
* it also check if the object's probability_underdrivable_ocg is correct
* \req
* CPR-3833
*/
TEST(f360_assign_underdrivability_status_to_tracks_ocg_QT, Assign_Underdrivability_Status_To_Tracks__Current_Status_Can_Not_Pass_New_Is_Likely_To_Pass)
{
   /** \precond
   * Pick zone where track should be placed.
   * Verify whether zone is not outside limits.
   * Set up object parameters to make it valid.
   * Set up its status to UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER.
   * Set status of zone where object is located to UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER.
   * Set probabilities associated with the zones 0 to 3 to 0.1F, 0.4F, 0.25F, 0.15F respectively
   * Set expected_probability_underdrivable_ocg to 0.65 since this is the probability that should be calculated
   * for the target object
   */
   const uint16_t zone_long_idx = 15U;
   const uint16_t zone_lat_idx = 0U;
   const float32_t expected_probability_underdrivable_ocg = 0.65F;

   // Check verifies whether selected zone is not out of limits.
   CHECK_TRUE_TEXT(zone_long_idx < ocg::NUM_CELLS_X, "Selected Longitudinal zone is greater than total number of longitudinal zones, modify test.");
   CHECK_TRUE_TEXT(zone_lat_idx < ocg::NUM_CELLS_Y, "Selected Lateral zone is greater than total number of lateral zones, modify test.");
  
   Set_Object_Params_To_Make_It_Valid(object_tracks[0], calib);
   object_tracks[0].underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
   object_tracks[0].vcs_position.x = 0.0F + static_cast<float32_t>(zone_long_idx) * occupancy_grid.grid_definition.cell_length;

   const float32_t extended_cell_width = 6.6667F;
   object_tracks[0].vcs_position.y = (extended_cell_width * 0.5F) - 0.01F;

   occupancy_grid.underdrivability.underdrivability_classification[zone_long_idx][zone_lat_idx].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER;
   occupancy_grid.underdrivability.underdrivability_classification[zone_long_idx][zone_lat_idx].probs[0] = 0.1F;
   occupancy_grid.underdrivability.underdrivability_classification[zone_long_idx][zone_lat_idx].probs[1] = 0.4F;
   occupancy_grid.underdrivability.underdrivability_classification[zone_long_idx][zone_lat_idx].probs[2] = 0.25F;
   occupancy_grid.underdrivability.underdrivability_classification[zone_long_idx][zone_lat_idx].probs[3] = 0.15F;

   /** \action
   * Call tested function.
   */
   Assign_Underdrivability_Status_To_Stationary_Object_OCG(calib, occupancy_grid, ocg_internal, host, object_tracks[0], timing_info);

   /** \result
   * Check whether object status was changed to UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER.
   * Check whether object probability underdrivable was changed to to the expected value
   */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER, object_tracks[0].underdrivable_status_ocg);
   DOUBLES_EQUAL(expected_probability_underdrivable_ocg, object_tracks[0].probability_underdrivable_ocg, F360_EPSILON);
}

/** \defgroup  object_motion_status_should_be_updated
 *  @{
 */

 /** \brief
  * Test group of Object_Motion_Status_Should_Be_Updated function.
  * Tests verify whether function properly verifies whether object motion status should be updated.
  */
TEST_GROUP(object_motion_status_should_be_updated_qualtest)
{
   F360_Object_Track_T object;
   F360_Calibrations_T calib;

   /** \setup
    * Initialize calibrations structure
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
   }
};

/** \purpose
 * Purpose of this test is to verify whether function returns true if object status is NEW_UPDATED
 * \req
 * CPR-3827
 */
TEST(object_motion_status_should_be_updated_qualtest, Object_Motion_Status_Should_Be_Updated__NEW_UPDATED)
{
   /** \precond
    * Set status of object to NEW_UPDATED
    */

   object.status = F360_OBJECT_STATUS_NEW_UPDATED;

   /** \action
    * Call tested funciton
    */
   bool result = Object_Motion_Status_Should_Be_Updated(object, calib.k_hyst_time_for_coasted_objects);

   /** \result
    * Check whether returned value is true
    */
   CHECK_TRUE(result);
}

/** \purpose
* Purpose of this test is to verify whether function returns true if object status is UPDATED
* \req
* CPR-3827
*/
TEST(object_motion_status_should_be_updated_qualtest, Object_Motion_Status_Should_Be_Updated__UPDATED)
{
   /** \precond
   * Set status of object to UPDATED
   */

   object.status = F360_OBJECT_STATUS_UPDATED;

   /** \action
   * Call tested funciton
   */
   bool result = Object_Motion_Status_Should_Be_Updated(object, calib.k_hyst_time_for_coasted_objects);

   /** \result
   * Check whether returned value is true
   */
   CHECK_TRUE(result);
}

/** \purpose
* Purpose of this test is to verify whether function returns true if object status is COASTED and time since update
* is below threshold
* \req
* CPR-3827
*/
TEST(object_motion_status_should_be_updated_qualtest, Object_Motion_Status_Should_Be_Updated__COASTED__Time_Below_Threshold)
{
   /** \precond
   * Set status of object to COASTED
   */
   object.status = F360_OBJECT_STATUS_COASTED;
   object.time_since_track_updated = calib.k_hyst_time_for_coasted_objects - 0.01F;

   /** \action
   * Call tested funciton
   */
   bool result = Object_Motion_Status_Should_Be_Updated(object, calib.k_hyst_time_for_coasted_objects);

   /** \result
   * Check whether returned value is true
   */
   CHECK_TRUE(result);
}

/** \purpose
* Purpose of this test is to verify whether function returns false if object status is COASTED and time since update
* is above threshold
* \req
* CPR-3827
*/
TEST(object_motion_status_should_be_updated_qualtest, Object_Motion_Status_Should_Be_Updated__COASTED__Time_Above_Threshold)
{
   /** \precond
   * Set status of object to COASTED
   */
   object.status = F360_OBJECT_STATUS_COASTED;
   object.time_since_track_updated = calib.k_hyst_time_for_coasted_objects + 0.01F;

   /** \action
   * Call tested funciton
   */
   bool result = Object_Motion_Status_Should_Be_Updated(object, calib.k_hyst_time_for_coasted_objects);

   /** \result
   * Check whether returned value is false
   */
   CHECK_FALSE(result);
}

/** \purpose
* Purpose of this test is to verify whether function returns false if object status is NEW_COASTED and time since update
* is below threshold
* \req
* CPR-3827
*/
TEST(object_motion_status_should_be_updated_qualtest, Object_Motion_Status_Should_Be_Updated__NEW_COASTED__Time_Below_Threshold)
{
   /** \precond
   * Set status of object to NEW_COASTED
   */
   object.status = F360_OBJECT_STATUS_NEW_COASTED;
   object.time_since_track_updated = calib.k_hyst_time_for_coasted_objects - 0.01F;

   /** \action
   * Call tested funciton
   */
   bool result = Object_Motion_Status_Should_Be_Updated(object, calib.k_hyst_time_for_coasted_objects);

   /** \result
   * Check whether returned value is false
   */
   CHECK_FALSE(result);
}


/** \defgroup  update_object_properties
*  @{
*/

/** \brief
* Test group of Update_Object_Properties() function.
* Tests verify whether object motion properties are properly updated.
*/
TEST_GROUP(update_object_properties_qualtest)
{
   F360_Object_Track_T object;
   F360_Globals_T globals;

   /** \setup
   * Initialize globals structure
   */
   TEST_SETUP()
   {
      globals.oncoming_speed_thresh = 10.0F;
      globals.obj_vehicular_spd_thresh = 5.0F;
   }
};

/** \purpose
* Purpose of this test is to verify whether movable flag is set when object is marked as moving
* \req
* CPR-3829
*/
TEST(update_object_properties_qualtest, update_object_properties__check_if_changes_moveable_flag)
{
   /** \precond
   * Set status f_moving flag to true
   * Set status movable_prob to false
   */
   object.f_moving = true;
   object.movable_prob = 0.0F;

   /** \action
   * Call tested funciton
   */
   Update_Object_Properties(object, globals);

   /** \result
   * Check whether object movable_prob was set
   */
   CHECK_TRUE(object.movable_prob > 0.5F);
}

/** \purpose
* Purpose of this test is to verify whether movable flag is not reset if object is not moving
* \req
* CPR-3829
*/
TEST(update_object_properties_qualtest, update_object_properties__check_if_moveable_flag_is_not_reset)
{
   /** \precond
   * Set status f_moving flag to false
   * Set status movable_prob to true
   */
   object.f_moving = false;
   object.movable_prob = 1.0F;

   /** \action
   * Call tested funciton
   */
   Update_Object_Properties(object, globals);

   /** \result
   * Check whether object movable_prob was not reset
   */
   CHECK_TRUE(object.movable_prob > 0.5F);
}



/** \defgroup  check_if_object_is_suspected_stationary
*  @{
*/

/** \brief
* Test group of Check_If_Object_Is_Suspected_Stationary() function. Test verify whether
* stationary suspected flag is properly set for both CTCA and CCA tracks.
*/
TEST_GROUP(check_if_object_is_suspected_stationary_qualtest)
{
   F360_Object_Track_T object;
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Host_T host;
   F360_Calibrations_T calib;
   F360_Tracker_Info_T tracker_info = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list;
   F360_Occlusion_Data_T occlusion_data[MAX_NUMBER_OF_SENSORS]{};

   /** \setup
   * Initialize tracker calibrations
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      (void)memset(occlusion_data, 0, sizeof(occlusion_data));
      calib.k_object_motion_min_speed = 1.5F;
      Set_Tracker_Variant(tracker_info.variant);
   }

   void Set_Object_Num_Moving_Dets(
      F360_Object_Track_T& object,
      bool f_desired_moving)
   {
      if (f_desired_moving)
      {
         for (int i = 0; i < 5; i++)
         {
            object.detids[i] = i + 1;
            det_props[i].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
         }
         object.speed = calib.k_object_motion_min_speed + 0.1F;
         object.ndets = 5;
      }
      else
      {
         for (int i = 0; i < 5; i++)
         {
            object.detids[i] = i + 1;
            det_props[i].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS;
         }
         object.speed = calib.k_object_motion_min_speed + 0.1F;
         object.ndets = 5;
      }
   }

   void Set_Object_Nees_Params(
      F360_Object_Track_T& object,
      const bool f_desired_moving)
   {
      if (f_desired_moving)
      {
         object.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX] = 0.0F;
         object.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY] = 0.0F;
         object.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VX] = 0.0F;
         object.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY] = 0.0F;

         object.vcs_velocity.lateral = 10.0F;
         object.vcs_velocity.longitudinal = 10.0F;
      }
      else
      {
         object.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX] = 100.0F;
         object.errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY] = 0.0F;
         object.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VX] = 0.0F;
         object.errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY] = 100.0F;

         object.vcs_velocity.lateral = 10.0F;
         object.vcs_velocity.longitudinal = 10.0F;
      }
   }

   void Set_Object_Sigma_Params(
      F360_Object_Track_T& object,
      const bool f_desired_moving)
   {
      if (f_desired_moving)
      {
         const float32_t speed_std = calib.k_object_motion_min_speed;
         object.speed = calib.k_object_motion_sigma_ctca_th * speed_std + 1.0F;
         object.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_S] = speed_std * speed_std;
      }
      else
      {
         const float32_t speed_std = calib.k_object_motion_min_speed;
         object.speed = calib.k_object_motion_sigma_ctca_th * speed_std - 1.0F;
         object.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_S] = speed_std * speed_std;
      }
   }

   void Set_Object_Parallel_Moving(
      F360_Object_Track_T& object,
      const bool f_desired_moving)
   {
      if (f_desired_moving)
      {
         object.vcs_position.y = calib.k_object_motion_parallel_moving_lat_posn_th - 0.1F;
         object.vcs_position.x = calib.k_object_motion_parallel_moving_lon_posn_th - 0.1F;
         object.vcs_heading = Angle{ calib.k_object_motion_parallel_moving_heading_th - 0.1F };
         object.speed = calib.k_object_motion_min_speed + 0.1F;
         host.speed = calib.k_object_motion_min_speed + 0.1F;
      }
      else
      {
         object.vcs_position.y = calib.k_object_motion_parallel_moving_lat_posn_th + 0.1F;
         object.vcs_position.x = calib.k_object_motion_parallel_moving_lon_posn_th + 0.1F;
         object.vcs_heading = Angle{ calib.k_object_motion_parallel_moving_heading_th + 0.1F };
         object.speed = calib.k_object_motion_min_speed - 0.1F;
         host.speed = calib.k_object_motion_min_speed + 0.1F;
      }
   }

   void Set_Object_Cross_Moving(
      F360_Object_Track_T& object,
      const bool f_desired_moving)
   {
      if (f_desired_moving)
      {
         object.movable_prob = 1.0F;
         object.speed = calib.k_object_motion_min_speed + 0.1F;
         object.vcs_position.y = 0.0F;
         object.vcs_position.x = 10.0F;
         object.vcs_velocity.lateral = -10.0F;
         object.vcs_velocity.longitudinal = 0.0F;
      }
      else
      {
         object.movable_prob = 0.0F;
         object.speed = 0.0F;
         object.vcs_position.y = 0.0F;
         object.vcs_position.x = 10.0F;
         object.vcs_velocity.lateral = 0.0F;
         object.vcs_velocity.longitudinal = 0.0F;
      }
   }

   void Set_Base_Object_Parameters(
      F360_Object_Track_T& object, bool occluding)
   {
      if (occluding)
      {
         object.Set_Bbox_Orientation(Angle{F360_PI / 4.0F});
         object.vcs_heading = Angle{F360_PI / 4.0F};

         object.bbox.Set_Length(20.0F);
      }
      else
      {
         object.Set_Bbox_Orientation(Angle{0.0F});
         object.vcs_heading = Angle{0.0F};

         object.bbox.Set_Length(2.0F);
      }
      
      object.bbox.Set_Width(2.0F);

      object.movable_prob = 1.0F;
      object.status = F360_OBJECT_STATUS_UPDATED;
      
   }

   void Set_Occluding_Objects_Parameters(bool is_object_occluding)
   {
      Set_Base_Object_Parameters(object_tracks[0], is_object_occluding);
      object_tracks[0].reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
      Point center = {-5.0F, 5.0F};
      object_tracks[0].bbox.Set_Center(center);
      object_tracks[0].vcs_position = object_tracks[0].bbox.Get_Corners().Rear_Right();
      object_tracks[0].confidenceLevel = 1.0F; 

      Set_Base_Object_Parameters(object_tracks[1], is_object_occluding);
      object_tracks[1].reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      center = {5.0F, 5.0F};
      object_tracks[1].bbox.Set_Center(center);
      object_tracks[1].vcs_position = object_tracks[1].bbox.Get_Corners().Rear_Left();
      object_tracks[1].confidenceLevel = 1.0F;
      
      tracker_info.num_active_objs = 2;
      tracker_info.active_obj_ids[0] = 1;
      tracker_info.active_obj_ids[1] = 2;
   }
};


/** \purpose
* Purpose of this test is to verify whether CTCA object is marked as stationary if does not meet any conditions to be moving.
* \req
* CPR-3828
*/
TEST(check_if_object_is_suspected_stationary_qualtest, check_if_object_is_suspected_stationary__ctca_marked_as_stationary_if_does_not_meet_any_condition)
{
   /** \precond
   * Set all object parameters to make it stationary
   */
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   const bool f_moving_desired = false;
   Set_Object_Num_Moving_Dets(object, f_moving_desired);
   Set_Object_Sigma_Params(object, f_moving_desired);
   Set_Object_Parallel_Moving(object, f_moving_desired);
   Set_Object_Cross_Moving(object, f_moving_desired);
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);

   /** \action
   * Call tested function
   */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);

   /** \result
   * Check if f_stationary_suspected is true
   */
   CHECK_TRUE(f_stationary_suspected);
}

/** \purpose
* Purpose of this test is to verify whether CTCA object is not marked as stationary if has enough moving detections
* \req
* CPR-3828
*/
TEST(check_if_object_is_suspected_stationary_qualtest, check_if_object_is_suspected_stationary__ctca_marked_as_not_stationary_if_has_enough_moving_detections)
{
   /** \precond
   * Set all but one object parameters to make it stationary
   */
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   const bool f_moving_desired = false;
   Set_Object_Sigma_Params(object, f_moving_desired);
   Set_Object_Parallel_Moving(object, f_moving_desired);
   Set_Object_Cross_Moving(object, f_moving_desired);
   Set_Object_Num_Moving_Dets(object, true);
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);

   /** \action
   * Call tested function
   */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);

   /** \result
   * Check if f_stationary_suspected is false
   */
   CHECK_FALSE(f_stationary_suspected);
}

/** \purpose
* Purpose of this test is to verify whether CTCA object is not marked as stationary if meets sigma condition
* \req
* CPR-3828
*/
TEST(check_if_object_is_suspected_stationary_qualtest, check_if_object_is_suspected_stationary__ctca_marked_as_not_stationary_if_meets_sigma_condition)
{
   /** \precond
   * Set all but one object parameters to make it stationary
   */
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   const bool f_moving_desired = false;
   Set_Object_Num_Moving_Dets(object, f_moving_desired);
   Set_Object_Parallel_Moving(object, f_moving_desired);
   Set_Object_Cross_Moving(object, f_moving_desired);
   Set_Object_Sigma_Params(object, true);
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   /** \action
   * Call tested function
   */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);

   /** \result
   * Check if f_stationary_suspected is false
   */
   CHECK_FALSE(f_stationary_suspected);
}

/** \purpose
* Purpose of this test is to verify whether CTCA object is not marked as stationary if is cross moving
* \req
* CPR-3828
*/
TEST(check_if_object_is_suspected_stationary_qualtest, check_if_object_is_suspected_stationary__ctca_marked_as_not_stationary_if_is_cross_moving)
{
   /** \precond
   * Set all but one object parameters to make it stationary
   */
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   const bool f_moving_desired = false;
   Set_Object_Num_Moving_Dets(object, f_moving_desired);
   Set_Object_Parallel_Moving(object, f_moving_desired);
   Set_Object_Sigma_Params(object, f_moving_desired);
   Set_Object_Cross_Moving(object, true);
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   /** \action
   * Call tested function
   */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);

   /** \result
   * Check if f_stationary_suspected is false
   */
   CHECK_FALSE(f_stationary_suspected);
}

/** \purpose
* Purpose of this test is to verify whether CTCA object is not marked as stationary if is parallel moving
* \req
* CPR-3828
*/
TEST(check_if_object_is_suspected_stationary_qualtest, check_if_object_is_suspected_stationary__ctca_marked_as_not_stationary_if_is_parallel_moving)
{
   /** \precond
   * Set all but one object parameters to make it stationary
   */
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   const bool f_moving_desired = false;
   Set_Object_Num_Moving_Dets(object, f_moving_desired);
   Set_Object_Sigma_Params(object, f_moving_desired);
   Set_Object_Cross_Moving(object, f_moving_desired);
   Set_Object_Parallel_Moving(object, true);
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);

   /** \action
   * Call tested function
   */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);

   /** \result
   * Check if f_stationary_suspected is false
   */
   CHECK_FALSE(f_stationary_suspected);
}


/** \purpose  
 * Purpose of this test is to verify whether CCA object is marked as stationary if does not meet any conditions to be moving.
 * \req
 * CPR-3828
 */
TEST(check_if_object_is_suspected_stationary_qualtest, check_if_object_is_suspected_stationary__cca_marked_as_stationary_if_does_not_meet_any_condition)
{
   /** \precond
    * Create occlusion object
    * Set occlusion status to not occluded, so, that moving_speed_threshold = calib.k_object_motion_min_speed
    * Set object speed to be greater than calib.k_object_motion_min_speed, so that CCA conditions are reached
    * Call Set_Object_Num_Moving_Dets(f_desired_moving = false), so that the detections suggest that the object is stationary
    * Set_Object_Nees_Params(f_desired_moving = false), so that the object is stationary
    * Call Set_Object_Parallel_Moving(f_desired_moving = false), so that the object is suspected to be stationary
    */

   // Set up objects
   bool is_occluding = false;
   Set_Occluding_Objects_Parameters(is_occluding);
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   Set_Object_Num_Moving_Dets(object, false);
   Set_Object_Nees_Params(object, false);
   Set_Object_Parallel_Moving(object,false);
   object.movable_prob = 0.0F;
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object.speed = calib.k_object_motion_min_speed + 0.1F;

   /** \action
    * Call Is_Object_Suspected_Stationary
    */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);

   
   /** \result
    * Check if the object is suspected to be stationary
    */
   CHECK_TRUE(f_stationary_suspected);
}

/** \purpose  
 * Purpose of this test is to verify whether CCA object is marked as stationary if is moveable and does not meet any other conditions to be moving.
 * \req
 * CPR-3828
 */
TEST(check_if_object_is_suspected_stationary_qualtest, check_if_object_is_suspected_stationary__cca_marked_as_stationary_if_moveable_and_does_not_meet_any_moving_condition)
{
   /** \precond
    * Create occlusion object
    * Set occlusion status to not occluded, so, that moving_speed_threshold = calib.k_object_motion_min_speed
    * Set object speed to be greater than calib.k_object_motion_min_speed, so that CCA conditions are reached
    * Call Set_Object_Num_Moving_Dets(f_desired_moving = false), so that the detections suggest that the object is stationary
    * Call Set_Object_Nees_Params(f_desired_moving = false), so that speed variance suggests that the object is stationary
    * Call Set_Object_Parallel_Moving(f_desired_moving = false), so that the object is suspected to be stationary
    * Call Set_Object_Cross_Moving(f_desired_moving = false), so that the object is suspected to be stationary
    */

   // Set up objects
   bool is_occluding = false;
   Set_Occluding_Objects_Parameters(is_occluding);
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   Set_Object_Num_Moving_Dets(object, false);
   Set_Object_Nees_Params(object, false);
   Set_Object_Parallel_Moving(object, false);
   Set_Object_Cross_Moving(object, false);
   object.movable_prob = 1.0F;
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object.speed = calib.k_object_motion_min_speed + 0.1F;

   /** \action
    * Call Is_Object_Suspected_Stationary
    */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);

   
   /** \result
    * Check if the object is suspected to be stationary
    */
   CHECK_TRUE(f_stationary_suspected);
}

/** \purpose  
 * Purpose of this test is to verify whether CCA object is not marked as stationary if has enough moving detections
 * \req
 * CPR-3828
 */
TEST(check_if_object_is_suspected_stationary_qualtest, check_if_object_is_suspected_stationary__cca_marked_as_not_stationary_if_has_enough_moving_detections)
{
   /** \precond
    * Create occlusion object
    * Set occlusion status to not occluded, so, that moving_speed_threshold = calib.k_object_motion_min_speed
    * Set object speed to be greater than calib.k_object_motion_min_speed, so that CCA conditions are reached
    * Call Set_Object_Num_Moving_Dets(f_desired_moving = true), so that the detections suggest that the object is not stationary
    * Call Set_Object_Nees_Params(f_desired_moving = false), so that speed variance suggests that the object is stationary
    * Call Set_Object_Parallel_Moving(f_desired_moving = false), so that the object is suspected to be stationary
    */

   // Set up objects
   bool is_occluding = false;
   Set_Occluding_Objects_Parameters(is_occluding);
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   Set_Object_Num_Moving_Dets(object, true);
   Set_Object_Nees_Params(object, false);
   Set_Object_Parallel_Moving(object,false);
   object.movable_prob = 0.0F;
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object.speed = calib.k_object_motion_min_speed + 0.1F;


   /** \action
    * Call Is_Object_Suspected_Stationary
    */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);

   
   /** \result
    * Check if the object is suspected to be not stationary
    */
   CHECK_FALSE(f_stationary_suspected);
}

/** \purpose  
 * Purpose of this test is to verify whether CTCA object is not marked as stationary if meets nees condition
 * \req
 * CPR-3828
 */
TEST(check_if_object_is_suspected_stationary_qualtest, check_if_object_is_suspected_stationary__cca_marked_as_not_stationary_if_meets_nees_condition)
{
   /** \precond
    * Create occlusion object
    * Set occlusion status to not occluded, so, that moving_speed_threshold = calib.k_object_motion_min_speed
    * Set object speed to be greater than calib.k_object_motion_min_speed, so that CCA conditions are reached
    * Call Set_Object_Num_Moving_Dets(f_desired_moving = false), so that the detections suggest that the object is stationary
    * Call Set_Object_Nees_Params(f_desired_moving = true), so that speed variance suggests that the object is not stationary
    * Call Set_Object_Parallel_Moving(f_desired_moving = false), so that the object is suspected to be stationary
    */

   // Set up objects
   bool is_occluding = false;
   Set_Occluding_Objects_Parameters(is_occluding);
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   Set_Object_Num_Moving_Dets(object, false);
   Set_Object_Parallel_Moving(object,false);
   Set_Object_Nees_Params(object, true);
   object.movable_prob = 0.0F;
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object.speed = calib.k_object_motion_min_speed + 0.1F;


   /** \action
    * Call Is_Object_Suspected_Stationary
    */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);

   
   /** \result
    * Check if the object is suspected to be not stationary
    */
   CHECK_FALSE(f_stationary_suspected);
}

/** \purpose  
 * Purpose of this test is to verify whether CCA object is not marked as stationary if is parallel moving
 * \req
 * CPR-3828
 */
TEST(check_if_object_is_suspected_stationary_qualtest, check_if_object_is_suspected_stationary__cca_marked_as_not_stationary_if_is_parallel_moving)
{
   /** \precond
    * Create occlusion object
    * Set occlusion status to not occluded, so, that moving_speed_threshold = calib.k_object_motion_min_speed
    * Set object speed to be greater than calib.k_object_motion_min_speed, so that CCA conditions are reached
    * Call Set_Object_Num_Moving_Dets(f_desired_moving = false), so that the detections suggest that the object is stationary
    * Call Set_Object_Nees_Params(f_desired_moving = false), so that speed variance suggests that the object is stationary
    * Call Set_Object_Parallel_Moving(f_desired_moving = true), so that the object is suspected to be not stationary
    */

   // Set up objects
   bool is_occluding = false;
   Set_Occluding_Objects_Parameters(is_occluding);
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   Set_Object_Num_Moving_Dets(object, false);
   Set_Object_Nees_Params(object, false);
   Set_Object_Parallel_Moving(object, true);
   object.movable_prob = 0.0F;
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;

   /** \action
    * Call Is_Object_Suspected_Stationary
    */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);

   
   /** \result
    * Check if the object is suspected to be not stationary
    */
   CHECK_FALSE(f_stationary_suspected);
}

/** \purpose  
 * Purpose of this test is to verify whether CCA object is not marked as stationary if is cross moving
 * \req
 * CPR-3828
 */
TEST(check_if_object_is_suspected_stationary_qualtest, check_if_object_is_suspected_stationary__cca_marked_as_not_stationary_if_is_cross_moving)
{
   /** \precond
    * Create occlusion object
    * Set occlusion status to not occluded, so, that moving_speed_threshold = calib.k_object_motion_min_speed
    * Set object speed to be greater than calib.k_object_motion_min_speed, so that CCA conditions are reached
    * Call Set_Object_Num_Moving_Dets(f_desired_moving = false), so that the detections suggest that the object is stationary
    * Call Set_Object_Nees_Params(f_desired_moving = false), so that speed variance suggests that the object is not stationary
    * Call Set_Object_Parallel_Moving(f_desired_moving = false), so that the object is suspected to be stationary
    * Call Set_Object_Cross_Moving(tf_desired_moving = true), so that the object is suspected to be not stationary
    */

   // Set up objects
   bool is_occluding = false;
   Set_Occluding_Objects_Parameters(is_occluding);
   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);
   Set_Object_Num_Moving_Dets(object, false);
   Set_Object_Nees_Params(object, false);
   Set_Object_Parallel_Moving(object, false);
   Set_Object_Cross_Moving(object, true);
   object.movable_prob = 1.0F;
   object.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;

   /** \action
    * Call Is_Object_Suspected_Stationary
    */
   const bool f_stationary_suspected = Is_Object_Suspected_Stationary(object, raw_detect_list, det_props, host, calib, sensors, occlusion_data);

   
   /** \result
    * Check if the object is suspected to be not stationary
    */
   CHECK_FALSE(f_stationary_suspected);
}


/** \defgroup  calcProbability_qualtest
*  @{
*/

/** \brief
* Test group of calcProbability function.
*/
TEST_GROUP(calcProbability_qualtest)
{
   const float32_t min_prob = 0.65F;
   F360_Object_Track_T object{};
   OBJ_CLASS_A_PRIORI_PROBABILITIES_T apriori;
   F360_Calibrations_T calibs;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      calibs.k_ad_oc_mean_length_pedestrian = 0.8F;
      calibs.k_ad_oc_inv_standard_deviation_length_pedestrian = 1.0F / 0.4F;
      calibs.k_ad_oc_mean_length_2wheel = 1.0F;
      calibs.k_ad_oc_inv_standard_deviation_length_2wheel = 1.0F / 0.5F;
      calibs.k_ad_oc_mean_length_car = 5.3F;
      calibs.k_ad_oc_inv_standard_deviation_length_car = 1.0F / 1.9F;
      calibs.k_ad_oc_mean_length_truck = 18.0F;
      calibs.k_ad_oc_inv_standard_deviation_length_truck = 1.0F / 4.0F;
      calibs.k_ad_oc_step_decrease_prob_unknown = 0.1F;
   }

};


/** \purpose
* Purpose of this test is to verify whether sum of calculated probabilities is equal to maximum defined value (1.0).
* \req
* CPR-3819
*/
TEST(calcProbability_qualtest, calcProbability__check_if_sum_of_calculated_probabilities_is_equal_1)
{
   /** \precond
   Setting required parameters
   */
   object.bbox.Set_Length(2.0F);
   object.speed = 2.0F;
   object.status = F360_OBJECT_STATUS_UPDATED;
   object.probability_undet = 0.0F;
   object.object_class = F360_OBJ_CLASS_CAR; 
   object.num_members_in_slow_moving_obj_cluster = 0U;
   object.average_rcs = 5.0F;
   object.maximum_rcs = 5.0F;

   apriori.car = 0.25F;
   apriori.pedestrian = 0.25F;
   apriori.truck = 0.25F;
   apriori.motorcycle = 0.25F;
   const float32_t sum_of_probabilities_expected = 1.0F;
   float32_t prob_threshold = 0.000001F;

   /** \action
   * Call tested function.
   */
   calcProbability(object, apriori, calibs);
   float32_t sum = object.probability_bicycle + object.probability_car + object.probability_motorcycle + object.probability_pedestrian + object.probability_truck;

   /** \result
   * Check whether sum of calculated probabilities is equal to sum_of_probabilities_expected.
   */
   DOUBLES_EQUAL(sum_of_probabilities_expected, sum, prob_threshold)
}


/** \defgroup  set_object_class
*  @{
*/

/** \brief
* Test group of setObjectClass function. Tests verify whether object class is properly assigned.
*/
TEST_GROUP(set_object_class_qualtest)
{
   F360_Object_Track_T object{};
};

/** \purpose
* Purpose of this test is to verify whether pedestrian class is chosen if it has the highest probability.
* \req
* CPR-7531
*/
TEST(set_object_class_qualtest, set_object_class__check_if_classifies_as_pedestrian)
{
   /** \precond
   * Set pedestrian probability to 0.5
   * Set bicycle, motorcycle, car, undet and truck probability to 0.1
   */
   object.probability_pedestrian = 0.5F;
   object.probability_bicycle = 0.1F;
   object.probability_motorcycle = 0.1F;
   object.probability_car = 0.1F;
   object.probability_truck = 0.1F;
   object.probability_undet = 0.1F;

   /** \action
   * Call tested function.
   */
   Set_Object_Class(object);

   /** \result
   * Check whether object was classified as pedestrian
   */
   CHECK_EQUAL(F360_OBJ_CLASS_PEDESTRIAN, object.object_class)
}

/** \purpose
* Purpose of this test is to verify whether bicycle class is chosen if it has the highest probability.
* \req
* CPR-7531
*/
TEST(set_object_class_qualtest, set_object_class__check_if_classifies_as_bicycle)
{
   /** \precond
   * Set bicycle probability to 0.5
   * Set pedestrian, motorcycle, car, undet and truck probability to 0.1
   */
   object.probability_pedestrian = 0.1F;
   object.probability_bicycle = 0.6F;
   object.probability_motorcycle = 0.1F;
   object.probability_car = 0.1F;
   object.probability_truck = 0.1F;
   object.probability_undet = 0.1F;

   /** \action
   * Call tested function.
   */
   Set_Object_Class(object);

   /** \result
   * Check whether object was classified as bicycle
   */
   CHECK_EQUAL(F360_OBJ_CLASS_BICYCLE, object.object_class)
}

/** \purpose
* Purpose of this test is to verify whether motorcycle class is chosen if it has the highest probability.
* \req
* CPR-7531
*/
TEST(set_object_class_qualtest, set_object_class__check_if_classifies_as_motorcycle)
{
   /** \precond
   * Set motorcycle probability to 0.5
   * Set pedestrian, bicycle, car, undet and truck probability to 0.1
   */
   object.probability_pedestrian = 0.1F;
   object.probability_bicycle = 0.1F;
   object.probability_motorcycle = 0.5F;
   object.probability_car = 0.1F;
   object.probability_truck = 0.1F;
   object.probability_undet = 0.1F;

   /** \action
   * Call tested function.
   */
   Set_Object_Class(object);

   /** \result
   * Check whether object was classified as motorcycle
   */
   CHECK_EQUAL(F360_OBJ_CLASS_MOTORCYCLE, object.object_class)
}

/** \purpose
* Purpose of this test is to verify whether car class is chosen if it has the highest probability.
* \req
* CPR-7531
*/
TEST(set_object_class_qualtest, set_object_class__check_if_classifies_as_car)
{
   /** \precond
   * Set car probability to 0.5
   * Set pedestrian, bicycle, motorcycle, undet and truck probability to 0.1
   */
   object.probability_pedestrian = 0.1F;
   object.probability_bicycle = 0.1F;
   object.probability_motorcycle = 0.1F;
   object.probability_car = 0.5F;
   object.probability_truck = 0.1F;
   object.probability_undet = 0.1F;

   /** \action
   * Call tested function.
   */
   Set_Object_Class(object);

   /** \result
   * Check whether object was classified as car
   */
   CHECK_EQUAL(F360_OBJ_CLASS_CAR, object.object_class)
}

/** \purpose
* Purpose of this test is to verify whether truck class is chosen if it has the highest probability.
* \req
* CPR-7531
*/
TEST(set_object_class_qualtest, set_object_class__check_if_classifies_as_truck)
{
   /** \precond
   * Set truck probability to 0.5
   * Set pedestrian, bicycle, motorcycle, undet and car probability to 0.1
   */
   object.probability_pedestrian = 0.1F;
   object.probability_bicycle = 0.1F;
   object.probability_motorcycle = 0.1F;
   object.probability_car = 0.1F;
   object.probability_truck = 0.5F;
   object.probability_undet = 0.1F;

   /** \action
   * Call tested function.
   */
   Set_Object_Class(object);

   /** \result
   * Check whether object was classified as truck
   */
   CHECK_EQUAL(F360_OBJ_CLASS_TRUCK, object.object_class)
}


/** \purpose
* Purpose of this test is to verify that undetermined class if chosen if it has the highest probability.
* \req
* CPR-3816
*/
TEST(set_object_class_qualtest, set_object_class__check_if_classifies_as_undetermined)
{
   /** \precond
   * Set pedestrian probability to be below threshold
   * Set motorcycle probability to be below threshold
   * Set car probability to be below threshold
   * Set truck probability to be below threshold
   */
   object.probability_pedestrian = 0.1F;
   object.probability_bicycle = 0.1F;
   object.probability_motorcycle = 0.1F;
   object.probability_car = 0.1F;
   object.probability_truck = 0.1F;
   object.probability_undet = 0.5F;

   /** \action
   * Call tested function.
   */
   Set_Object_Class(object);

   /** \result
   * Check whether object was classified as undetermined
   */
   CHECK_EQUAL(F360_OBJ_CLASS_UNDETERMINED, object.object_class)
}


/** \defgroup  filteringAndNormalizationOfProbabilities
*  @{
*/

/** \brief
* Test group of filteringAndNormalizationOfProbabilities function. Tests verify whether probability being undetermined is used to lower probabilities of object being specific class.
*/
TEST_GROUP(filteringAndNormalizationOfProbabilities_qualtest)
{
   OBJECT_CLASS_PROBABILITY_T criteria_Bayes;
   F360_Object_Track_T object{};
};

/** \purpose
* Purpose of this test is to verify whether probability of being undetermined is used to lower probabilities of object being specific class
* \req
* CPR-3821
*/
TEST(filteringAndNormalizationOfProbabilities_qualtest, filteringAndNormalizationOfProbabilities__check_if_undeterm_prob_value_is_used_to_lower_prob_of_other_class)
{
   /** \precond
   *
   Setting required parameters
   */
   object.probability_pedestrian = 0.4F;
   object.probability_car = 0.2F;
   object.probability_bicycle = 0.0F;
   object.probability_motorcycle = 0.1F;
   object.probability_truck = 0.2F;
   object.probability_undet = 0.2F;

   criteria_Bayes.probability_unknown = 0.0F;
   criteria_Bayes.probability_pedestrian = 0.2F;
   criteria_Bayes.probability_motorcycle = 0.2F;
   criteria_Bayes.probability_car = 0.2F;
   criteria_Bayes.probability_truck = 0.2F;
   float32_t start_pedestrian_probability = object.probability_pedestrian;

   /** \action
   * Call tested function.
   */
   filteringAndNormalizationOfProbabilities(object, criteria_Bayes);

   /** \result
   * Check whether object was classified as truck
   */
   CHECK_TRUE(object.probability_pedestrian < start_pedestrian_probability)
}


/** \defgroup calcUndetProb
*  @{
*/

/** \brief
* Purpose of this test group is to verify whether probability unknown is set to max value when object has status:
      F360_OBJECT_STATUS_NEW or
      F360_OBJECT_STATUS_INVALID or
      F360_OBJECT_STATUS_NEW_UPDATED or
      F360_OBJECT_STATUS_NEW_COASTED.
*/
TEST_GROUP(calcUndetProb_qualtest)
{
   F360_Object_Track_T object{};
   F360_Calibrations_T calibs = {};

   /** \setup
   *
   **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      calibs.k_ad_oc_step_decrease_prob_unknown = 0.1F;
   }
};


/** \purpose
* Purpose of this test is to verify whether probability unknown is set to max value when object has status: F360_OBJECT_STATUS_NEW
* \req
* CPR-3820
*/
TEST(calcUndetProb_qualtest, calcUndetProb__check_if_prob_unknown_is_ste_to_max_value_when_object_has_status_F360_OBJECT_STATUS_NEW)
{
   /** \precond
   Setting required parameters
   */
   object.status = F360_OBJECT_STATUS_NEW;
   const float32_t expected_undetermined_probability = 1.0F;

   /** \action
   * Call tested function.
   */
   calcUndetProb(object, calibs);

   /** \result
   * Check whether object has undetermined probability equal 1.
   */
   DOUBLES_EQUAL(expected_undetermined_probability, object.probability_undet, F360_EPSILON)
}

/** \purpose
* Purpose of this test is to verify whether probability unknown is set to max value when object has status: F360_OBJECT_STATUS_INVALID
* \req
* CPR-3820
*/
TEST(calcUndetProb_qualtest, calcUndetProb__check_if_prob_unknown_is_ste_to_max_value_when_object_has_status_F360_OBJECT_STATUS_INVALID)
{
   /** \precond
   Setting required parameters
   */
   object.status = F360_OBJECT_STATUS_INVALID;
   const float32_t expected_undetermined_probability = 1.0F;

   /** \action
   * Call tested function.
   */
   calcUndetProb(object, calibs);

   /** \result
   * Check whether object has undetermined probability equal 1.
   */
   DOUBLES_EQUAL(expected_undetermined_probability, object.probability_undet, F360_EPSILON)
}

/** \purpose
* Purpose of this test is to verify whether probability unknown is set to max value when object has status: F360_OBJECT_STATUS_NEW_UPDATED
* \req
* CPR-3820
*/
TEST(calcUndetProb_qualtest, calcUndetProb__check_if_prob_unknown_is_ste_to_max_value_when_object_has_status_F360_OBJECT_STATUS_NEW_UPDATED)
{
   /** \precond
   Setting required parameters
   */
   object.status = F360_OBJECT_STATUS_NEW_UPDATED;
   const float32_t expected_undetermined_probability = 1.0F;

   /** \action
   * Call tested function.
   */
   calcUndetProb(object, calibs);

   /** \result
   * Check whether object has undetermined probability equal 1.
   */
   DOUBLES_EQUAL(expected_undetermined_probability, object.probability_undet, F360_EPSILON)
}

/** \purpose
* Purpose of this test is to verify whether probability unknown is set to max value when object has status: F360_OBJECT_STATUS_NEW_COASTED
* \req
* CPR-3820
*/
TEST(calcUndetProb_qualtest, calcUndetProb__check_if_prob_unknown_is_ste_to_max_value_when_object_has_status_F360_OBJECT_STATUS_NEW_COASTED)
{
   /** \precond
   Setting required parameters
   */
   object.status = F360_OBJECT_STATUS_NEW_COASTED;
   const float32_t expected_undetermined_probability = 1.0F;

   /** \action
   * Call tested function.
   */
   calcUndetProb(object, calibs);

   /** \result
   * Check whether object has undetermined probability equal 1.
   */
   DOUBLES_EQUAL(expected_undetermined_probability, object.probability_undet, F360_EPSILON)
}

/** @}*/

/** \brief
 *  Test group for run_obj_class. Purpose is to test that all object classes are possible 
 *  object classifications, corresponding to the requirement CPR-3815. The classes are
 *    - pedestrian
 *    - motorcycle
 *    - bicycle
 *    - car
 *    - truck
 *    - undetermined
 */
TEST_GROUP(f360_run_obj_class)
{
   // Initialize common variables for all test cases
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Calibrations_T cals{};
   F360_Object_Class_T exp_obj_class;

   /** \setup
   * - Initialize calibrations
   * - Set host speed to 2 m/s
   * - Set up an object with
   *  - Length 2 m
   *  - Width 1.15 m
   *  - Speed 5.29 m/s
   *  - and status UPDATED
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(cals);
      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 1;

      object_tracks[0].id = 1;
      object_tracks[0].bbox.Set_Length(2.0F);
      object_tracks[0].speed = 5.29F;
      object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;

      object_tracks[0].probability_undet = 0.0F;
      object_tracks[0].object_class = F360_OBJ_CLASS_UNDETERMINED;
      object_tracks[0].num_members_in_slow_moving_obj_cluster = 0U;
      object_tracks[0].average_rcs = 5.0F;
      object_tracks[0].maximum_rcs = 5.0F;
   }
};

/** \purpose
* Purpose of this test is to verify that an object can be classified as a bicycle.
* \req
* CPR-3815
*/
TEST(f360_run_obj_class, f360_run_obj_class_bicycle)
{
   /** \precond
    * An object has been set up in the TEST_GROUP.
    * Change object class to something which is not BICYLE (to ensure that class gets updated to bike after funtion call) and not UNDETERMINED (to make sure countermeasures to delay VRU classification are not active)
    * Set expected object class to bicycle
    */
   object_tracks[0].object_class = F360_OBJ_CLASS_CAR;
   exp_obj_class = F360_OBJ_CLASS_BICYCLE;
   const float32_t expected_probability = 0.4764454F;

   /** \action
   * Call run_obj_class.
   */
   run_obj_class(object_tracks, tracker_info, cals);

   /** \result
   * Check that the object is classified as bicycle.
   */
   CHECK_TRUE_TEXT(object_tracks[0].object_class == exp_obj_class, "Incorrect object class was assigned.");

   DOUBLES_EQUAL(expected_probability, object_tracks[0].probability_bicycle, F360_EPSILON);
}

/** \purpose
* Purpose of this test is to verify that an object can be classified as a pedestrian.
* \req
* CPR-3815
*/
TEST(f360_run_obj_class, f360_run_obj_class_pedestrian)
{
   /** \precond
    * Set expected object class to pedestrian.
    * An object has been set up in the TEST_GROUP.
    *    - Change speed to 1 m/s
    *    - and length to 0.5 m.
    *    - and object class to something which is not PEDESTRIAN (to ensure that class gets updated to ped after funtion call) and not UNDETERMINED (to make sure countermeasures to delay VRU classification are not active)
    */
   object_tracks[0].speed = 1.0F;
   object_tracks[0].bbox.Set_Length(0.5F);
   object_tracks[0].object_class = F360_OBJ_CLASS_CAR;
   exp_obj_class = F360_OBJ_CLASS_PEDESTRIAN;
   const float32_t expected_probability = 0.6977376;

   /** \action
   * Call run_obj_class.
   */
   run_obj_class(object_tracks, tracker_info, cals);

   /** \result
   * Check that the object is classified as pedestrian.
   */
   CHECK_TRUE(object_tracks[0].object_class == exp_obj_class);

   DOUBLES_EQUAL(expected_probability, object_tracks[0].probability_pedestrian, F360_EPSILON);
}

/** \purpose
* Purpose of this test is to verify that an object can be classified as a motorcycle.
* \req
* CPR-3815
*/
TEST(f360_run_obj_class, f360_run_obj_class_motorcycle)
{
   /** \precond
    * Set expected object class to motorcycle.
    * An object has been set up in the TEST_GROUP.
    *    - Change speed to 20 m/s
    *    - and length to 2.0 m.
    */
   object_tracks[0].speed = 20.0F;
   object_tracks[0].bbox.Set_Length(2.0F);
   exp_obj_class = F360_OBJ_CLASS_MOTORCYCLE;
   const float32_t expected_probability = 0.9100202;

   /** \action
   * Call run_obj_class.
   */
   run_obj_class(object_tracks, tracker_info, cals);

   /** \result
   * Check that the object is classified as motorcycle.
   */
   CHECK_TRUE(object_tracks[0].object_class == exp_obj_class);

   DOUBLES_EQUAL(expected_probability, object_tracks[0].probability_motorcycle, F360_EPSILON);
}

/** \purpose
* Purpose of this test is to verify that an object can be classified as a car.
* \req
* CPR-3815
*/
TEST(f360_run_obj_class, f360_run_obj_class_car)
{
   /** \precond
    * Set expected object class to car.
    * An object has been set up in the TEST_GROUP.
    *    - Change speed to 20 m/s
    *    - and length to 5m
    */
   object_tracks[0].speed = 20.0F;
   object_tracks[0].bbox.Set_Length(5.0F);
   exp_obj_class = F360_OBJ_CLASS_CAR;
   const float32_t expected_probability = 0.9741324;

   /** \action
   * Call run_obj_class.
   */
   run_obj_class(object_tracks, tracker_info, cals);

   /** \result
   * Check that the object is classified as car.
   */
   CHECK_TRUE(object_tracks[0].object_class == exp_obj_class);

   DOUBLES_EQUAL(expected_probability, object_tracks[0].probability_car, F360_EPSILON);
}

/** \purpose
* Purpose of this test is to verify that an object can be classified as a truck.
* \req
* CPR-3815
*/
TEST(f360_run_obj_class, f360_run_obj_class_truck)
{
   /** \precond
    * Set expected object class to truck.
    * An object has been set up in the TEST_GROUP.
    *    - Change speed to 20 m/s
    *    - Set length 16m
    */
   object_tracks[0].speed = 20.0F;
   object_tracks[0].bbox.Set_Length(16.0F);
   exp_obj_class = F360_OBJ_CLASS_TRUCK;
   const float32_t expected_probability = 1;

   /** \action
   * Call run_obj_class.
   */
   run_obj_class(object_tracks, tracker_info, cals);

   /** \result
   * Check that the object is classified as truck.
   */
   CHECK_TRUE(object_tracks[0].object_class == exp_obj_class);

   DOUBLES_EQUAL(expected_probability, object_tracks[0].probability_truck, F360_EPSILON);
}

/** \purpose
* Purpose of this test is to verify that an object can be classified as a undetermined.
* \req
* CPR-3815
*/
TEST(f360_run_obj_class, f360_run_obj_class_undetermined)
{
   /** \precond
    * Set expected object class to undetermined.
    * An object has been set up in the TEST_GROUP.
    *    - Change speed to 0.002 m/s
    *    - Set both length and width to 1 m 
    *    - Set status to NEW_UPDATED 
    */
   object_tracks[0].speed = 0.002F;
   object_tracks[0].bbox.Set_Length(1.0F);
   object_tracks[0].status = F360_OBJECT_STATUS_NEW_UPDATED;

   exp_obj_class = F360_OBJ_CLASS_UNDETERMINED;
   const float32_t expected_probability = 1;

   /** \action
   * Call run_obj_class.
   */
   run_obj_class(object_tracks, tracker_info, cals);

   /** \result
   * Check that the object is classified as undetermined.
   */
   CHECK_TRUE(object_tracks[0].object_class == exp_obj_class);

   DOUBLES_EQUAL(expected_probability, object_tracks[0].probability_undet, F360_EPSILON);
}

/** @}*/
