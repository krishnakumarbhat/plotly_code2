/** \file
 * This file contains unit tests for content of f360_object_underdrivability_classification_qt.cpp file
 */

#include "f360_object_underdrivability_classification.h"
#include "f360_track_classification.h"
#include <CppUTest/TestHarness.h>
#include "ocg_underdrivability_enum.h"
#include "ocg_constants.h"
#include <iostream>

//#include "headerfile_needed.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_object_underdrivability_classification_qt
 *  @{
 */

/** \brief
 * This test group will test the intended functionality of Object_Underdrivability_Classification().
 * Tests are designed to check
 *    - That loop over objects are functioning correctly such that all active objects get their status updated but not inactive objects
 *    - That correct classification method is used for moving and stationary objects respectively
 *    - That stationary objects are not classified when occupancy grid is invalid
 */
TEST_GROUP(f360_object_underdrivability_classification_qt)
{
   // Declare common variables used within all tests in this test group.
   F360_Tracker_Info_T  tracker_info = {};
   F360_Host_Props_T host_props = {};
   ocg::OCG_Outputs_T occupancy_grid = {};
   F360_Host_T host = {};
   F360_Calibrations_T calib = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   
   /** \setup
    * Setup 10 objects that are valid and moving with UD status NOT_TO_CONSIDER and that has a very large estimated height such that they would be classified CAN_PASS_UNDER by Assign_Underdrivability_Status_To_Moving_Object()
    * Setup 10 objects that are invalid and moving with UD status NOT_TO_CONSIDER. Since they are invalid their underdrivable status should be unchanged
    * Remaing objects should be invalid wth UD status NOT_TO_CONSIDER. Since they are invalid their underdrivable status should be unchanged
	 * Setup tracker timestamp to an arbitrary positive value
    * Setup a valid occupancy grid with all zones to CAN_NOT_PASS_UNDER.
	*/
   TEST_SETUP()
   {
      // Use default tracker calibrations
      Initialize_Tracker_Calibrations(calib);

      // Setup tracker timestamp to an arbitrary value
      tracker_info.time_us = 1000.0F;

      // Set up a valid OCG which have all its cells classifed as CAN_NOT_PASS_UNDER
      occupancy_grid.underdrivability.grid_curvature = 0.0F; // No curvature on OCG grid
      occupancy_grid.grid_definition.num_cells_x_close = ocg::NUM_CELLS_X_CLOSE;
      occupancy_grid.grid_definition.num_cells_x_mid = ocg::NUM_CELLS_X_MID;
      occupancy_grid.grid_definition.num_cells_x_far = ocg::NUM_CELLS_X_FAR;
      occupancy_grid.grid_definition.cell_length =  2.0F;
      occupancy_grid.grid_definition.num_cells_y =  ocg::NUM_CELLS_Y;
      occupancy_grid.grid_definition.cell_width = 6.0F;
      occupancy_grid.grid_definition.cell_width_extension_factor = 1.0F; // No extansion of OCG cell width at far distances
      // Set up all cells to have status CAN_NOT_PASS_UNDER
      for (uint8_t row_idx = 0u; row_idx < ocg::NUM_CELLS_X; row_idx ++)
      {
         for (uint8_t col_idx = 0u; col_idx < ocg::NUM_CELLS_Y; col_idx ++)
         {
             occupancy_grid.underdrivability.underdrivability_classification[row_idx][col_idx].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
         }
      }
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.x = 0.0F; // Set up to 0 such that OCG and VCS has the same definition of origin
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.y = 0.0F; // Set up to 0 such that OCG and VCS has the same definition of origin
      occupancy_grid.underdrivability.ogcs_host_rear_axle_position.yaw = 0.0F; // No curvature on host predicted path
      occupancy_grid.timestamp = tracker_info.time_us; // Same time stamp as tracker (such that tracker don't need to do prediction of OCG)
      occupancy_grid.f_valid = true; // // Set occupancy grid to valid


      // Create 10 moving valid objects which should be classified as CAN_PASS_UNDER
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

      // Create 10 moving valid objects which should be classified as CAN_NOT_PASS_UNDER
      obj = {};
      obj.f_moving = true;
      obj.underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
      obj.status = F360_OBJECT_STATUS_UPDATED;
      obj.otg_height = calib.ud_mov_height_threshold - 1.0F; // Estimated height is significantly lower than the threshold for classifying moving objects as CAN_PASS_UNDER
      obj.ud_mov_historic_ndets = 10.0F; // Significantkly larger than 0 such that the estimated mean height is clearly defined
      obj.ud_mov_cnt_underdrivable = 0U; // Zero the counter for scans at which the object was above the height threshold
      obj.vcs_position.x = calib.ud_mov_posx_max_limit - 10.0F;  // Inside the region where UD classification is done for moving objects
      obj.vcs_position.y = 0.0F; // Within the region where UD classification is done for moving objects 
      for(int32_t obj_idx = 10; obj_idx < 20; obj_idx++)
      {
         object_tracks[obj_idx] = obj;
         object_tracks[obj_idx].id = obj_idx + 1;
         tracker_info.active_obj_ids[tracker_info.num_active_objs] = obj_idx + 1;
         tracker_info.num_active_objs++;
      }

      // Create 10 moving valid objects located in the area behind the host that should be classified as NOT_TO_CONSIDER but would be classified as CAN_PASS_UNDER if they were valid
      obj = {};
      obj.f_moving = true;
      obj.underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
      obj.status = F360_OBJECT_STATUS_UPDATED;
      obj.otg_height = calib.ud_mov_height_threshold + 1.0F; // Estimated height is significantly larger than the threshold for classifying moving objects as CAN_PASS_UNDER
      obj.ud_mov_historic_ndets = 10.0F; // Significantkly larger than 0 such that the estimated mean height is clearly defined
      obj.ud_mov_cnt_underdrivable = calib.ud_mov_cnt_consecutive_scans + 1U; // Larger than threshold for classifying moving objects as CAN_PASS_UNDER
      obj.vcs_position.x = calib.ud_mov_posx_min_limit - 10.0F;  // Outside the region where UD classification is done for moving objects
      obj.vcs_position.y = 0.0F; // Within the region where UD classification is done for moving objects 
      for(int32_t obj_idx = 20; obj_idx < 30; obj_idx++)
      {
         object_tracks[obj_idx] = obj;
         object_tracks[obj_idx].id = obj_idx + 1;
         tracker_info.active_obj_ids[tracker_info.num_active_objs] = obj_idx + 1;
         tracker_info.num_active_objs++;
      }

      // Create 10 objects which should not be considered for underdrivability classification by neither Assign_Underdrivability_Status_To_Moving_Object or Assign_Underdrivability_Status_To_Moving_Object functions
      obj = {};
      obj.f_moving = false; // Not considered by Assign_Underdrivability_Status_To_Moving_Object
      obj.underdrivable_status_ocg = ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
      obj.status = F360_OBJECT_STATUS_UPDATED;
      obj.otg_height = calib.ud_mov_height_threshold + 1.0F; // Estimated height is significantly larger than the threshold for classifying moving objects as CAN_PASS_UNDER
      obj.ud_mov_historic_ndets = 10.0F; // Significantkly larger than 0 such that the estimated mean height is clearly defined
      obj.ud_mov_cnt_underdrivable = calib.ud_mov_cnt_consecutive_scans + 1U; // Larger than threshold for classifying moving objects as CAN_PASS_UNDER
      obj.vcs_position.x = calib.ud_mov_posx_min_limit + 10.0F;  // Inside the region where UD classification is done for moving objects
      obj.vcs_position.y = 0.0F; // Within the region where UD classification is done for moving objects 
      for(int32_t obj_idx = 30; obj_idx < static_cast<int32_t>(NUMBER_OF_OBJECT_TRACKS); obj_idx++)
      {
         object_tracks[obj_idx] = obj;
         object_tracks[obj_idx].id = obj_idx + 1;
         tracker_info.active_obj_ids[tracker_info.num_active_objs] = obj_idx + 1;
         tracker_info.num_active_objs++;
      }
   }
};


/** \purpose  
 * The purpose of this test is to check such that valid moving objects with height above the threshhold
 * get their underdrivability status updated by Assign_Underdrivability_Status_To_Moving_Object()
 * \req
 * CPR-3832
 */
TEST(f360_object_underdrivability_classification_qt, test_valid_moving_objs_height_above_threshold)
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
    * i.e. that their underdrivability status is CAN_PASS_UNDER
    */
   for(int32_t obj_idx = 0; obj_idx < 10; obj_idx++)
   {
      CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER, object_tracks[obj_idx].underdrivable_status_ocg);
   }
}

/** \purpose  
 * The purpose of this test is to check such that valid moving objects with height below the threshhold
 * get their underdrivability status updated by Assign_Underdrivability_Status_To_Moving_Object()
 * \req
 * CPR-3832
 */
TEST(f360_object_underdrivability_classification_qt, test_valid_moving_objs_height_below_threshold)
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
    * i.e. that their underdrivability status is CAN_NOT_PASS_UNDER
    */
   for(int32_t obj_idx = 10; obj_idx < 20; obj_idx++)
   {
      CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER, object_tracks[obj_idx].underdrivable_status_ocg);
   }
}

/** \purpose  
 * The purpose of this test is to check such that valid (historical height mean is above the threshold for required number of consecutive scans)
 * moving objects do not get their underdrivability status updated by Assign_Underdrivability_Status_To_Moving_Object()
 * when they are behind the host vehicle.
 * \req
 * CPR-3832
 * CPR-3834
 */
TEST(f360_object_underdrivability_classification_qt, test_valid_moving_objs_behind_host)
{
   /** \precond
    * Default test setup from TEST GROUP is used.
    */
	
   /** \action
    * Call Object_Underdrivability_Classification()
    */
   Object_Underdrivability_Classification(tracker_info, host_props, &occupancy_grid, NULL, host, calib, object_tracks, timing_info);

   /** \result
    * Check that the underdrivability status for moving objects outside of classification area were properly classified by Assign_Underdrivability_Status_To_Stationary_Object(),
    * i.e. that their underdrivability status is NOT_TO_CONSIDER
    */
   for(int32_t obj_idx = 20; obj_idx < 30; obj_idx++)
   {
      CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER, object_tracks[obj_idx].underdrivable_status_ocg);
   }
}

/** \purpose  
 * The purpose of this test is to check if objects are set as UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER when their underdrivability status is not
 * updated by either Assign_Underdrivability_Status_To_Moving_Object or Assign_Underdrivability_Status_To_Moving_Object functions.
 * \req
 * CPR-3834
 */
TEST(f360_object_underdrivability_classification_qt, test_objs_invalid)
{
   /** \precond
    * Occupancy grid not valid, otherwise default test setup from TEST GROUP is used.
    */
   occupancy_grid.f_valid = false; // // Set occupancy grid to invalid

   /** \action
    * Call Object_Underdrivability_Classification()
    */
   Object_Underdrivability_Classification(tracker_info, host_props, &occupancy_grid, NULL, host, calib, object_tracks, timing_info);

   /** \result
    * Check that the underdrivability status for invalid objects were not updated, i.e. that their undersdrivability status is NOT_TO_CONSIDER
    */
   for(int32_t obj_idx = 30; obj_idx < static_cast<int32_t>(NUMBER_OF_OBJECT_TRACKS); obj_idx++)
   {
      CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER, object_tracks[obj_idx].underdrivable_status_ocg);
   }
}

/** @}*/

/** \defgroup  f360_assign_underdrivability_status_to_tracks_sg
 *  @{
 */

/** \brief
 * The aim of this test group is to test the functionality of assigning drivibility status to objects based on
 * the classification of nearby SG segments.
 */
TEST_GROUP(f360_assign_underdrivability_status_to_tracks_sg)
{
   F360_Tracker_Info_T tracker_info;
   F360_Host_Props_T host_props;
   sg::SG_Output_T sg_output;
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS];
   F360_Host_T host;
   ocg::OCG_Outputs_T* p_occupancy_grid = NULL;
   F360_Calibrations_T calibs;
   F360_TRKR_TIMING_INFO_T timing_info;
   
   /** \setup
    * Set up a default scenario with
    * - One contour containing two vertices (i.e. one segment) on a straight longitudinal line
    * Three objects with two placed close to the segment
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      host.dist_rear_axle_to_vcs_m = 2.9F;
      host_props.cos_delta_pointing = 1.0F;
      host_props.sin_delta_pointing = 0.0F;
      
      tracker_info.num_active_objs = 3;
      tracker_info.active_obj_ids[0U] = 1U;
      tracker_info.active_obj_ids[1U] = 2U;
      tracker_info.active_obj_ids[2U] = 3U;

      object_tracks[0U].id = 1U;
      object_tracks[0U].vcs_position.x = -50.0F;
      object_tracks[0U].vcs_position.y = -3.0F;
      object_tracks[0U].f_moving = false;
      object_tracks[0U].drivable_confidence_sg = 100U;
      object_tracks[0U].drivable_status_sg = sg::SG_Drivability_Class_T::OVERDRIVABLE;

      object_tracks[1U].id = 2U;
      object_tracks[1U].vcs_position.x = 5.0F;
      object_tracks[1U].vcs_position.y = -3.0F;
      object_tracks[1U].f_moving = false;
      object_tracks[1U].drivable_confidence_sg = 50U;
      object_tracks[1U].drivable_status_sg = sg::SG_Drivability_Class_T::UNDERDRIVABLE;

      object_tracks[2U].id = 3U;
      object_tracks[2U].vcs_position.x = 15.0F;
      object_tracks[2U].vcs_position.y = -3.0F;
      object_tracks[2U].f_moving = false;
      object_tracks[2U].drivable_confidence_sg = 50U;
      object_tracks[2U].drivable_status_sg = sg::SG_Drivability_Class_T::UNDERDRIVABLE;

      // Set up the list of longitudinally sorted objects
      tracker_info.vcslong_sorted_start = &object_tracks[0U];
      tracker_info.vcslong_sorted_next_track[0U] = &object_tracks[1U];
      tracker_info.vcslong_sorted_next_track[1U] = &object_tracks[2U];
      tracker_info.vcslong_sorted_next_track[2U] = NULL;
      tracker_info.vcslong_sorted_first_infront_of_host = &object_tracks[1U];

      // Set up the SG structures
      // (Note that vertex positions are in ISO, so they will be compensated with host dist to rear axle, and flipped y-axis)
      sg_output.num_contours = 1U;
      sg_output.contours[0U].num_vertices = 2U;

      sg_output.vertices[0U].drivability = sg::SG_Drivability_Class_T::NONDRIVABLE;
      sg_output.vertices[0U].drivability_confidence = 90U;
      sg_output.vertices[0U].position_x = 10.0F;
      sg_output.vertices[0U].position_y = 2.8F;

      sg_output.vertices[1U].drivability = sg::SG_Drivability_Class_T::UNDERDRIVABLE;
      sg_output.vertices[1U].drivability_confidence = 95U;
      sg_output.vertices[1U].position_x = 7.0F;
      sg_output.vertices[1U].position_y = 2.8F;
   }
};

/** \purpose  
 * Check that the second object in the sorted list, which is close to an SG segment, is associated and has its drivability status set
 * to the corresponding SG status and that the other objects, that are too far away from the segment, are unclassified.
 * \req
 * CPR-5245
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg, Assign_Underdrivability_Status_To_Stationary_Object_SG_Non_Drivable_SG)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP with
    * - one object close enough to the SG to be classified as NONDRIVABLE
    * - the other objects are too far away and shall be UNCLASSIFIED
    */

   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Object_Underdrivability_Classification(tracker_info, host_props, p_occupancy_grid, &sg_output, host, calibs, object_tracks, timing_info);

   /** \result
    * Check that the first and last objects are unclassified and that the middle object is classified according to its associated SG segment.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::NONDRIVABLE == object_tracks[1U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(90U, object_tracks[1U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[2U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[2U].drivable_confidence_sg, "Drivable confidence was not reset.")
}

/** \purpose  
 * Check that the second object in the sorted list is unclassified when it's associated to a segment that is unclassified.
 * Also check that the other two objects are unclassified since they are too far away from the segment.
 * \req
 * CPR-5246
 */
TEST(f360_assign_underdrivability_status_to_tracks_sg, Assign_Underdrivability_Status_To_Stationary_Object_SG_Unclassified_SG)
{
   /** \precond
    * A default case has been set up in the TEST_GROUP with
    * - One segment with it's vertices indicating it's UNCLASSIFIED
    * - one object close enough to the SG to be classified as UNCLASSIFIED
    * - the other objects are too far away and shall be UNCLASSIFIED
    */
   sg_output.vertices[0U].drivability = sg::SG_Drivability_Class_T::UNCLASSIFIED;

   /** \action
    * Call Assign_Underdrivability_Status_To_Stationary_Object_SG().
    */
   Object_Underdrivability_Classification(tracker_info, host_props, p_occupancy_grid, &sg_output, host, calibs, object_tracks, timing_info);

   /** \result
    * Check that the first and last objects are unclassified and that the middle object is classified according to its associated SG segment.
    */
   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[0U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[0U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[1U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[1U].drivable_confidence_sg, "Drivable confidence was not reset.")

   CHECK_TRUE_TEXT(sg::SG_Drivability_Class_T::UNCLASSIFIED == object_tracks[2U].drivable_status_sg, "Incorrect driving status assigned.");
   CHECK_EQUAL_TEXT(0U, object_tracks[2U].drivable_confidence_sg, "Drivable confidence was not reset.")
}

/** @}*/
