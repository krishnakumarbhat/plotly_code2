/** \file
 * This file contains unit tests for content of f360_object_underdrivability_classification.cpp file
 */

#include "f360_object_underdrivability_classification.h"
#include <CppUTest/TestHarness.h>
#include "ocg_underdrivability_enum.h"
#include "ocg_constants.h"

//#include "headerfile_needed.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_object_underdrivability_classification
 *  @{
 */

/** \brief
 * This test group will test the intended functionality of Object_Underdrivability_Classification().
 * Tests are designed to check
 *    - That loop over objects are functioning correctly such that all active objects get their status updated but not inactive objects
 *    - That correct classification method is used for moving and stationary objects respectively
 *    - That stationary objects are not classified when occupancy grid is invalid
 */
TEST_GROUP(f360_object_underdrivability_classification)
{
   // Declare common variables used within all tests in this test group.
   F360_Tracker_Info_T  tracker_info = {};
   F360_Host_Props_T host_props = {};
   ocg::OCG_Outputs_T occupancy_grid = {};
   sg::SG_Output_T p_sg_output = {};
   F360_Host_T host = {};
   F360_Calibrations_T calib = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   
   /** \setup
    * Setup 10 objects that are valid and moving with UD status NOT_TO_CONSIDER and that has a very large estimated height such that they would be classified CAN_PASS_UNDER by Assign_Underdrivability_Status_To_Moving_Object()
    * Setup 10 objects that are valid and stationary with UD status CAN_NOT_PASS_UNDER and that are located in a OCG zone with status LIKELY_TO_PASS_UNDER such that they will be classified as LIKELY_TO_PASS_UNDER by Assign_Underdrivability_Status_To_Stationary_Object(). When OCG grid is not valid their underdrivable status should be cleared to NOT_TO_CONSIDER
    * Setup 10 objects that are invalid and moving with UD status NOT_TO_CONSIDER. Since they are invalid their underdrivable status should be unchanged
    * Setup 10 objects that are invalid and stationary ith UD status NOT_TO_CONSIDER. Since they are invalid their underdrivable status should be unchanged
    * Remaing objects should be invalid wth UD status NOT_TO_CONSIDER. Since they are invalid their underdrivable status should be unchanged
	 * Setup tracker timestamp to an arbitrary positive value
    * Setup a valid occupancy grid with all zones except one to CAN_NOT_PASS_UNDER. The zone which all the valid objects are positioned inside should be set to LIKELY_TO_PASS_UNDER. The occupancy grid should have the same timestamp as tracker. The number of cells and cell sizes of the occupancy grid can be setup arbitrary. Setup occpancy grid host properties such that the grid is not curved and that OCG origin and VCS origin coinside.
    * Setup host properties such that host path is not curved and VCS and OCG origin coinside
	*/
   TEST_SETUP()
   {
      // Use default tracker calibrations
      Initialize_Tracker_Calibrations(calib);

      // Setup tracker timestamp to an arbitrary value
      tracker_info.time_us = 1000.0F;

      // Setup host
      host.dist_rear_axle_to_vcs_m = 0.0F; // Set up to 0 such that OCG and VCS has the same definition of origin
      host.yaw_rate_rad = 0.0F; // No curvature on host predicted path

      // Set up a valid OCG which have one cell classified as LIKELY_TO_PASS_UNDER and remaining cells classifed as CAN_NOT_PASS_UNDER
      occupancy_grid.underdrivability.grid_curvature = 0.0F; // No curvature on OCG grid
      occupancy_grid.grid_definition.num_cells_x_close = ocg::NUM_CELLS_X_CLOSE;
      occupancy_grid.grid_definition.num_cells_x_mid = ocg::NUM_CELLS_X_MID;
      occupancy_grid.grid_definition.num_cells_x_far = ocg::NUM_CELLS_X_FAR;
      occupancy_grid.grid_definition.cell_length =  2.0F;
      occupancy_grid.grid_definition.num_cells_y =  ocg::NUM_CELLS_Y;
      occupancy_grid.grid_definition.cell_width = 6.0F;
      occupancy_grid.grid_definition.cell_width_extension_factor = 1.0F; // No extansion of OCG cell width at far distances
      // Set up all cells except one to have status CAN_NOT_PASS_UNDER
      for (uint8_t row_idx = 0u; row_idx < ocg::NUM_CELLS_X; row_idx ++)
      {
         for (uint8_t col_idx = 0u; col_idx < ocg::NUM_CELLS_Y; col_idx ++)
         {
             occupancy_grid.underdrivability.underdrivability_classification[row_idx][col_idx].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
         }
      }
      occupancy_grid.underdrivability.underdrivability_classification[25][0].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER; // The cell corrsponding to long pos betwen 50m and 52m and lat pos between -3m and 3m have status LIKELY_TO_PASS_UNDER
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.x = 0.0F; // Set up to 0 such that OCG and VCS has the same definition of origin
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.y = 0.0F; // Set up to 0 such that OCG and VCS has the same definition of origin
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw = 0.0F; // No curvature on host predicted path
      occupancy_grid.timestamp = tracker_info.time_us; // Same time stamp as tracker (such that tracker don't need to do prediction of OCG)
      occupancy_grid.f_valid = true; // // Set occupancy grid to valid


      // Create 10 moving valid objects which should be classified as CAN_PASS_UNDER but whould be classified as LIKELY_TO_PASS_UNDER if they are stationary
      F360_Object_Track_T obj = {};
      obj.f_moving = true;
      obj.status = F360_OBJECT_STATUS_UPDATED;
      obj.ndets = 0; // No new detections such that the height estimate is unchanged (except for small reduction due to forgetting factor)
      obj.underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
      obj.otg_height = calib.ud_mov_height_threshold + 1.0F; // Estimated height is significantly larger than the threshold for classifying moving objects as CAN_PASS_UNDER
      obj.ud_mov_historic_ndets = 10.0F; // Significantkly larger than 0 such that the estimated mean height is clearly defined
      obj.ud_mov_cnt_underdrivable = calib.ud_mov_cnt_consecutive_scans + 1U; // Larger than threshold for classifying moving objects as CAN_PASS_UNDER
      obj.vcs_position.x = 51.0F; // Within the region where UD classification is done for moving objects and in the OCG zone which has status LIKELY_TO_PASS_UNDER
      obj.vcs_position.y = 0.0F; // Within the region where UD classification is done for moving objects 
      for(int32_t obj_idx = 0; obj_idx < 10; obj_idx++)
      {
         object_tracks[obj_idx] = obj;
         object_tracks[obj_idx].id = obj_idx + 1;
         tracker_info.active_obj_ids[tracker_info.num_active_objs] = obj_idx + 1;
         tracker_info.num_active_objs++;
      }

      // Create 10 stationary valid objects that should be clasified as LIKELY_TO_PASS_UNDER by the OCG grid but would be classified as CAN_PASS_UNDER if they were moving.
      // Set their underdrivable status to CAN_NOT_PASS_UNDER such that it is verifyable thet the status is cleared to NOT_TO_CONSIDER when the OCG is invalid
      obj.f_moving = false;
      obj.underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
      for(int32_t obj_idx = 10; obj_idx < 20; obj_idx++)
      {
         object_tracks[obj_idx] = obj;
         tracker_info.active_obj_ids[tracker_info.num_active_objs] = obj_idx + 1;
         object_tracks[obj_idx].id = obj_idx + 1;
         tracker_info.num_active_objs++;
      }

      // Create 10 moving invalid objects that should be classified as NOT_TO_CONSIDER but would be classified as CAN_PASS_UNDER if they were valid
      obj.f_moving = true;
      obj.underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
      obj.status = F360_OBJECT_STATUS_INVALID;
      for(int32_t obj_idx = 20; obj_idx < 30; obj_idx++)
      {
         object_tracks[obj_idx] = obj;
         object_tracks[obj_idx].id = obj_idx + 1;
      }

      // Create 10 stationary invalid objects that should be classified as NOT_TO_CONSIDER but would be classified as LIKELY_TO_PASS_UNDER if they were valid
      obj.f_moving = false;
      for(int32_t obj_idx = 30; obj_idx < 40; obj_idx++)
      {
         object_tracks[obj_idx] = obj;
         object_tracks[obj_idx].id = obj_idx + 1;
      }

      // Set default invalid object values for remaining objects
      F360_Object_Track_T default_obj = {};
      default_obj.underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
      for(int32_t obj_idx = 40; obj_idx < static_cast<int32_t>(NUMBER_OF_OBJECT_TRACKS); obj_idx++)
      {
         object_tracks[obj_idx] = default_obj;
         object_tracks[obj_idx].id = obj_idx + 1;
      }
   }
};

/** \purpose  
 * The purpose of this test is to check such that invalid objects don't get their underdrivability status updated
 */
TEST(f360_object_underdrivability_classification, Test_Invalid_Objs_Not_Updated)
{
   /** \precond
    * Default test setup from TEST GROUP is used.
    * Set expected_probability_underdrivable_ocg to 0.0 since the object will be tagged as UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
    * and will have 0.0 probability_underdrivable_ocg field. This will ensure that those two variables match.
    */
   const float32_t expected_probability_underdrivable_ocg = 0.0F;
	
   /** \action
    * Call Object_Underdrivability_Classification()
    */
   Object_Underdrivability_Classification(tracker_info, host_props, &occupancy_grid, NULL, host, calib, object_tracks, timing_info);

   /** \result
    * Check that the underdrivability status for invalid objects were not updated, i.e. that their undersdrivability status is NOT_TO_CONSIDER
    * Check that the probability_underdrivable_ocg of the object is set to 0 since it has status UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
    */
   for(int32_t obj_idx = 20; obj_idx < static_cast<int32_t>(NUMBER_OF_OBJECT_TRACKS); obj_idx++)
   {
      CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER, object_tracks[obj_idx].underdrivable_status_ocg);
      DOUBLES_EQUAL(expected_probability_underdrivable_ocg, object_tracks[obj_idx].probability_underdrivable_ocg, F360_EPSILON);
   }
}

/** \purpose  
 * The purpose of this test is to check such that valid moving objects get their underdrivability status updated by Assign_Underdrivability_Status_To_Moving_Object()
 * when there is a valid OCG
 */
TEST(f360_object_underdrivability_classification, Test_Valid_Moving_Objs_Updated_Valid_OCG)
{
   /** \precond
    * Default test setup from TEST GROUP is used.
    */
	
   /** \action
    * Call Object_Underdrivability_Classification()
    */
   Object_Underdrivability_Classification(tracker_info, host_props, &occupancy_grid, NULL, host, calib, object_tracks, timing_info);

   /** \result
    * Check that the underdrivability status for valid moving objects were updated by Assign_Underdrivability_Status_To_Moving_Object(),
    * i.e. that their underdrivability status is UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
    */
   for(int32_t obj_idx = 0; obj_idx < 10; obj_idx++)
   {
      CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER, object_tracks[obj_idx].underdrivable_status_ocg);
   }
}

/** \purpose  
 * The purpose of this test is to check such that valid moving objects get their underdrivability status updated by Assign_Underdrivability_Status_To_Moving_Object()
 * also when there is an invalid OCG
 */
TEST(f360_object_underdrivability_classification, Test_Valid_Moving_Objs_Updated_Invalid_OCG)
{
   /** \precond
    * Default test setup from TEST_GROUP is used except for
    * Set OCG f_valid flag to false
    */
	occupancy_grid.f_valid = false;

   /** \action
    * Call Object_Underdrivability_Classification()
    */
   Object_Underdrivability_Classification(tracker_info, host_props, &occupancy_grid, NULL, host, calib, object_tracks, timing_info);

   /** \result
    * Check that the underdrivability status for valid moving objects were updated by Assign_Underdrivability_Status_To_Moving_Object(),
    * i.e. that their underdrivability status is UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
    */
   for(int32_t obj_idx = 0; obj_idx < 10; obj_idx++)
   {
      CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER, object_tracks[obj_idx].underdrivable_status_ocg);
   }
}


/** \purpose  
 * The purpose of this test is to check such that valid stationary objects get their underdrivability status updated by Assign_Underdrivability_Status_To_Stationary_Object()
 * when there is a valid OCG
 */
TEST(f360_object_underdrivability_classification, Test_Valid_Stationary_Objs_Updated_Valid_OCG)
{
   /** \precond
    * Default test setup from TEST GROUP is used.
    */
   
   /** \action
    * Call Object_Underdrivability_Classification()
    */
   Object_Underdrivability_Classification(tracker_info, host_props, &occupancy_grid, NULL, host, calib, object_tracks, timing_info);

   /** \result
    * Check that the underdrivability status for valid stationry objects were updated by Assign_Underdrivability_Status_To_Stationary_Object(),
    * i.e. that their underdrivability status is UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER
    */
   for(int32_t obj_idx = 10; obj_idx < 20; obj_idx++)
   {
      CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER, object_tracks[obj_idx].underdrivable_status_ocg);
   }
}

/** \purpose  
 * The purpose of this test is to check such that valid stationary objects get their underdrivability status set to NOT_TO_CONSIDER
 * when there is an invalid OCG
 */
TEST(f360_object_underdrivability_classification, Test_Valid_Stationry_Objs_Not_Updated_Invalid_OCG)
{
   /** \precond
    * Default test setup from TEST_GROUP is used except for
    * Set OCG f_valid flag to false
    */
	occupancy_grid.f_valid = false;

   /** \action
    * Call Object_Underdrivability_Classification()
    */
   Object_Underdrivability_Classification(tracker_info, host_props, &occupancy_grid, NULL, host, calib, object_tracks, timing_info);

   /** \result
    * Check that the underdrivability status for valid moving objects were set to NOT_TO_CONSIDER
    */
   for(int32_t obj_idx = 10; obj_idx < 20; obj_idx++)
   {
      CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER, object_tracks[obj_idx].underdrivable_status_ocg);
   }
}
/** @}*/
