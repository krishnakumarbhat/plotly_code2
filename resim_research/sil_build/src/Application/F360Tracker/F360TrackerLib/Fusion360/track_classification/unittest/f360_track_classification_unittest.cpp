/** \file
   This file contains unit tests for content of f360_track_classification.cpp file
*/

#include "f360_track_classification.h"
#include <CppUTest/TestHarness.h>
#include "f360_clear_object_track.h"
#include "ocg_occupancy_grid_types.h"
#include "f360_occlusion.h"

using namespace f360_variant_A;

/** \defgroup  f360_track_classification_check_obj_occlusion_classification_is_called
 *  @{
 */

/** \brief
* Test group dedicated to verify the function Object_Occlusion_Classification() is called 
* inside of the main function Track_Classification().
* It is done by setting up a scenario with a single forward looking sensor and 2 objects. 
* 1 object is intended to be tested and while the other object is placed between the test object and 
* the sensor, occluding the test object.
**/
TEST_GROUP(f360_track_classification_check_obj_occlusion_classification_is_called)
{	
   // Declare common variables used within all tests in this test group.
   F360_Tracker_Info_T tracker_info = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   const uint32_t obj_id_occluding = 3U;
   const uint32_t obj_id_to_be_tested = 6U;

   // For occlusion
   F360_Calibrations_T calib = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   F360_Occlusion_Data_T occlusion_data[MAX_NUMBER_OF_SENSORS];

   F360_Host_T host = {};
   F360_Host_Props_T host_props = {};
   ocg::OCG_Outputs_T occupancy_grid = {};
   sg::SG_Output_T p_sg_output = {};
   F360_Globals_T globals = {};
   rspp_variant_A::RSPP_Detection_List_T dets_raw = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};

   /** \setup
    * Initialize tracker calibrations.
    * Setup a single forward looking sensor such that
    * - its mounting position is in vcs origo.
    * - its boresight is vcs forward
    * - is valid
    * - has look id F360_DET_LOOK_ID_0
    * - for that look id, has a fov_min_az_rad of -1.5F
    * - for that look id, has a fov_max_az_rad of 1.5F
    * - for that look id, has a range limit of 150.0F
    * Setup tracker_info to contain 2 active objects and object ids
    * Setup the occluding object to
    * - have vcs position (25,0)
    * - be f_moving
    * - be f_moveable
    * - have status updated
    * - have confidence_level 1
    * - have reference point rear
    * - have heading and pointing 0
    * - have a length of 2 and width of 10
    * - update bbox center to correctly set all private parameters in the bbox class
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      Setup_Front_Center_Sensor_For_Occlusion(sensors[0]);
      (void)memset(occlusion_data, 0, sizeof(occlusion_data));

      tracker_info.num_active_objs = 2;
      tracker_info.active_obj_ids[0] = obj_id_occluding;
      tracker_info.active_obj_ids[1] = obj_id_to_be_tested;

      object_tracks[obj_id_occluding - 1].id = obj_id_occluding;
      object_tracks[obj_id_to_be_tested - 1].id = obj_id_to_be_tested;

      for (F360_Object_Track_T& obj : object_tracks)
      {
         Clear_Object_Track(obj);
      }

      const Point occluding_obj_position = Point{25.0F, 0.0F};
      Set_Object_Parameters_To_Be_Valid_For_Occlusion(occluding_obj_position, object_tracks[obj_id_occluding - 1]);
   }

   // Helper functions for occlusion initialization
   void Set_Object_Parameters_To_Be_Valid_For_Occlusion(
      const Point& vcs_pos,
      F360_Object_Track_T& object)
   {
      object.f_moving = true;
      object.movable_prob = 1.0F;
      object.status = F360_OBJECT_STATUS_UPDATED;
      object.confidenceLevel = 1.0F;
      object.reference_point = F360_REFERENCE_POINT_REAR;
      object.vcs_position = vcs_pos;

      object.vcs_heading = Angle{0.0F};
      object.bbox.Set_Orientation(object.vcs_heading);

      object.bbox.Set_Length(2.0F);
      object.bbox.Set_Width(10.0F);
      object.Update_Bbox_Center();
   }
   
   void Setup_Front_Center_Sensor_For_Occlusion(
      F360_Radar_Sensor_T& sensor)
   {
      sensor.constant.mounting_position.vcs_position.lateral = 0.0F;
      sensor.constant.mounting_position.vcs_position.longitudinal = 0.0F;
      sensor.constant.mounting_position.vcs_boresight_azimuth_angle = 0.0F;

      sensor.variable.is_valid = true;
      sensor.variable.look_id = F360_DET_LOOK_ID_0;
      sensor.constant.fov_min_az_rad[sensor.variable.look_id] = -1.5F;
      sensor.constant.fov_max_az_rad[sensor.variable.look_id] = 1.5F;
      sensor.constant.range_limits[sensor.variable.look_id] = 150.0F;
   } 
};

/** \purpose  
 * Verify that an object that which reference point is positioned behind another object is marked as occluded.
 * \req
 * NA.
 */
TEST(f360_track_classification_check_obj_occlusion_classification_is_called, Verify_Single_Object_Status_Occluded)
{
   /** \precond
    * Setup the test object to be positioned at (28,0), i.e. behind the occluding object
    * Set the test object to also
    * - be f_moving
    * - moveable_prob be 1.0
    * - have status updated
    * - have confidence_level 1
    * - have reference point rear
    * - have heading and pointing 0
    * - have a length of 2 and width of 10
    * - update bbox center to correctly set all private parameters in the bbox class
    * Initialize occlusion
    */

   const Point test_obj_position = Point{28.0F, 0.0F};
   Set_Object_Parameters_To_Be_Valid_For_Occlusion(test_obj_position, object_tracks[obj_id_to_be_tested - 1]);
   object_tracks[obj_id_to_be_tested - 1].bbox.Set_Width(2.0f);

   Update_Occlusion_Data(tracker_info, sensors, object_tracks, occlusion_data, timing_info);

   /** \action
    * Call Track_Classification().
    */
   Track_Classification(host, host_props, &occupancy_grid, &p_sg_output, globals, dets_raw, det_props, sensors, calib, occlusion_data, tracker_info, timing_info, object_tracks);
   
   /** \result
    * Verify the function Object_Occlusion_Classification() was called by verifying the test object occlusion_status.at_vcs_position have status OCCLUDED
    */
   const F360_Object_Track_T& test_obj = object_tracks[obj_id_to_be_tested - 1];
   CHECK_EQUAL(OCCLUSION_STATUS_OCCLUDED, test_obj.occlusion_status);

}

/** \defgroup  f360_track_classification_check_underdrivability_is_assigned_correctly
 *  @{
 */

/** \brief
* Test group dedicated to checking if the correct underdrivability assignment is done, in two cases:
* 1) When OCG input is available for underdrivability status assignmnent
* 2) When OCG input is not available, thus the default status should be set
**/
TEST_GROUP(f360_track_classification_check_underdrivability_is_assigned_correctly)
{	
   // Declare common variables used within all tests in this test group.
   F360_Tracker_Info_T tracker_info = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Calibrations_T calib = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};
   F360_Host_T host = {};
   F360_Host_Props_T host_props = {};
   ocg::OCG_Outputs_T occupancy_grid = {};
   sg::SG_Output_T p_sg_output = {};
   F360_Globals_T globals = {};
   rspp_variant_A::RSPP_Detection_List_T dets_raw = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Occlusion_Data_T occlusion_data[MAX_NUMBER_OF_SENSORS];

   /** \setup
    * Initialize tracker calibrations.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      (void)memset(occlusion_data, 0, sizeof(occlusion_data));

      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0U] = 1;
      tracker_info.time_us = 6974010;  // Arbitrary

      for (F360_Object_Track_T& obj : object_tracks)
      {
         Clear_Object_Track(obj);
      }

      // Set Relevant Object Properties
      object_tracks[0].movable_prob = 0.0F;
      object_tracks[0].status = F360_OBJECT_STATUS_UPDATED;

      // Set Host Properties
      host.dist_rear_axle_to_vcs_m = 3.53F;
      host.curvature_rear = 0.0F;

      Set_Occupancy_Grid_Properties(host, tracker_info, occupancy_grid);
   }

   void Set_Occupancy_Grid_Properties(const F360_Host_T host, const F360_Tracker_Info_T tracker_info, ocg::OCG_Outputs_T& occupancy_grid)
   {
      // Set Occupancy Grid Properties
      occupancy_grid.f_valid = true;
      occupancy_grid.timestamp = tracker_info.time_us - 1.0; // OCG is 1us behind ROT in timestamp
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
         }
      }
   }
};

/** \purpose  
 * The purpose of this test is to check, if the occupancy grid input is availble (by checking f_valid), then the correct UD status is returned for the given object
 * \req
 * NA.
 */
TEST(f360_track_classification_check_underdrivability_is_assigned_correctly, Verify_Non_Default_UD_Status_Is_Assigned)
{
   /** \precond
   * OCG f_valid flag is set to true
   * Set longitudinal position of track to be placed on edge of selected zone.
   * Assume that there is 1 microsecond time difference between OCG and F360Tracker modules
   * Assumen object is non_moveable
   * The object is setup to be in the 10th cell
   * The 10th cell status is set to be UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
   */
   
   const uint16_t selected_long_zone_idx_1 = 10U;
   const uint16_t selected_lat_zone_idx_1 = 0U;

   object_tracks[0].vcs_position.x = static_cast<float32_t>(selected_long_zone_idx_1) * occupancy_grid.grid_definition.cell_length + 0.01F;
   object_tracks[0].vcs_position.y = static_cast<float32_t>(selected_lat_zone_idx_1) * occupancy_grid.grid_definition.cell_width;
   
   // Set OCG cell status tp CAN_PASS_UNDER, the object should be located in this cell
   occupancy_grid.underdrivability.underdrivability_classification[selected_long_zone_idx_1][selected_lat_zone_idx_1].underdrivability_status = ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;

   /** \action
   * Call Track_Classification().
   */
   Track_Classification(host, host_props, &occupancy_grid, NULL, globals, dets_raw, det_props, sensors, calib, occlusion_data, tracker_info, timing_info, object_tracks);

   /** \result
   * Check whether first status is equal to UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
   * The test checks, if the object UD status is the same as the 10th OCG cell
   */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_CAN_PASS_UNDER, object_tracks[0].underdrivable_status_ocg);
}

/** \purpose  
 * The purpose of this test is to check, if the occupancy grid input is not availble (by checking f_valid), then the default UD status is returned for the given object
 * \req
 * NA.
 */
TEST(f360_track_classification_check_underdrivability_is_assigned_correctly, Verify_Default_UD_Status_Is_Assigned_f_valid_check)
{
   /** \precond
   * OCG f_valid flag is set to false 
   * Set longitudinal position of track to be placed on edge of selected zone.
   * Assume object is non_moveable
   */
   occupancy_grid.f_valid = false;
   occupancy_grid.timestamp = 0.0;

   tracker_info.num_active_objs = 1U;
   tracker_info.active_obj_ids[0] = 1U;

   const uint16_t selected_long_zone_idx_1 = 10U;
   const uint16_t selected_lat_zone_idx_1 = 0U;

   object_tracks[0].vcs_position.x = static_cast<float32_t>(selected_long_zone_idx_1) * occupancy_grid.grid_definition.cell_length + 0.01F;
   object_tracks[0].vcs_position.y = static_cast<float32_t>(selected_lat_zone_idx_1) * occupancy_grid.grid_definition.cell_width;

   /** \action
   * Call Track_Classification().
   */
   Track_Classification(host, host_props, &occupancy_grid, NULL, globals, dets_raw, det_props, sensors, calib, occlusion_data, tracker_info, timing_info, object_tracks);

   /** \result
   * Check whether UD status is equal to UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
   * The test checks, if the object UD status is eqaul to default status
   */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER, object_tracks[0].underdrivable_status_ocg);
}

/** \purpose  
 * The purpose of this test is to check, if the occupancy grid input is not availble (due to ointer to OCG structure bing NULL), then the default UD status is returned for the given object
 * \req
 * NA.
 */
TEST(f360_track_classification_check_underdrivability_is_assigned_correctly, Verify_Default_UD_Status_Is_Assigned_null_pointer_check)
{
   /** \precond
   * Set longitudinal position of track to be placed on edge of selected zone.
   * Assume object is non_moveable
   */
   occupancy_grid.f_valid = false;
   occupancy_grid.timestamp = 0.0;

   tracker_info.num_active_objs = 1U;
   tracker_info.active_obj_ids[0] = 1U;

   const uint16_t selected_long_zone_idx_1 = 10U;
   const uint16_t selected_lat_zone_idx_1 = 0U;

   object_tracks[0].vcs_position.x = static_cast<float32_t>(selected_long_zone_idx_1) * occupancy_grid.grid_definition.cell_length + 0.01F;
   object_tracks[0].vcs_position.y = static_cast<float32_t>(selected_lat_zone_idx_1) * occupancy_grid.grid_definition.cell_width;

   /** \action
   * Call Track_Classification() with NULL as pointer to the OCG inout
   */
   Track_Classification(host, host_props, NULL, NULL, globals, dets_raw, det_props, sensors, calib, occlusion_data, tracker_info, timing_info, object_tracks);

   /** \result
   * Check whether UD status is equal to UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER
   * The test checks, if the object UD status is eqaul to default status
   */
   CHECK_EQUAL(ocg::UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER, object_tracks[0].underdrivable_status_ocg);
}
/** @}*/
