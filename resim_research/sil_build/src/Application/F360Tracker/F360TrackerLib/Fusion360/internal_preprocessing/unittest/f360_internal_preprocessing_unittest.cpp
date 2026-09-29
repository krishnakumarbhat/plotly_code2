/** \file
 * This file contains unit tests for content of f360_internal_preprocessing.cpp file
 */

#include "f360_internal_preprocessing.h"
#include <CppUTest/TestHarness.h>
#include "f360_set_variant.h"

using namespace f360_variant_A;

/** \defgroup  f360_internal_preprocessing
 *  @{
 */

/** \brief
 * Test basic functions from f360_internal_preprocessing
 */
TEST_GROUP(f360_internal_preprocessing)
{	
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS]{};
   const float32_t threshold = 0.00000001F;
   const float32_t test_pass_th = 1e-6F;

   TEST_SETUP()
   {
      raw_detection_list.number_of_valid_detections = MAX_NUMBER_OF_DETECTIONS;
      for (unsigned k = 0; k < raw_detection_list.number_of_valid_detections; k++)
      {
         raw_detection_list.detections[k].processed.range_rate_compensated = 0.1F;
         raw_detection_list.detections[k].raw.sensor_id = 1U;
      }
   }
};

/** \purpose  
 * Verify if detections are copied properly
 * \req NA.
 */
TEST(f360_internal_preprocessing, Copy_Detections_Info__Test)
{
   /** \action
    * call Copy_Detections_Info().
    */
    Copy_Detections_Info(raw_detection_list, sensors, det_props);

   /** \result
    * Compare detections
    */	
    for (unsigned k = 0; k < raw_detection_list.number_of_valid_detections; k++)
    {
        CHECK_EQUAL(det_props[k].motion_status, raw_detection_list.detections[k].processed.motion_status);
        CHECK_EQUAL(det_props[k].f_ok_to_use, raw_detection_list.detections[k].processed.f_ok_to_use);
        DOUBLES_EQUAL(det_props[k].vcs_position.x, raw_detection_list.detections[k].processed.vcs_position_x, threshold);
        DOUBLES_EQUAL(det_props[k].vcs_position.y, raw_detection_list.detections[k].processed.vcs_position_y, threshold);
    }
}
/** @}*/

/**
*\purpose Tests that a historical detection's position is updated to new VCS when host has turned right.
*\req NA
*/
TEST(f360_internal_preprocessing, Update_Detection_Position)
{
   /** \precond
   * - position in previous VCS is [-2,-2].
   * - deltax, delta_y = [1,1], corresponding to host moving pi/2 degrees along a circle of radius 1m
   * - Rotation matrix is set up for a rotation of pi/2 radians.
   * - Expected position in new VCS is [-3,3]
   **/
   float32_t det_x_pos_vcs = -2.0F;
   float32_t det_y_pos_vcs = -2.0F;

   float32_t delta_x_old_vcs = 1.0F;
   float32_t delta_y_old_vcs = 1.0F;

   // Rotation matrix for clockwise rotation of pi/2 radians:
   float32_t rot_mat[2][2] = { 0.0F };
   rot_mat[0U][0U] = 0.0F;
   rot_mat[0U][1U] = 1.0F;
   rot_mat[1U][0U] = -1.0F;
   rot_mat[1U][1U] = 0.0F;

   float32_t expected_x_pos = -3.0F;
   float32_t expected_y_pos = 3.0F;

   /** \action
   * Call Update_Detection_Position.
   **/
   Update_Detection_Position(delta_x_old_vcs, delta_y_old_vcs, rot_mat, det_x_pos_vcs, det_y_pos_vcs);

   /** \result
   * Checks that the position is correctly updated to new VCS.
   **/
   CHECK_EQUAL(expected_x_pos, det_x_pos_vcs);
   CHECK_EQUAL(expected_y_pos, det_y_pos_vcs);
}

/**
*\purpose Tests that a historical detection's position is updated to new VCS when host has turned left.
*\req NA
*/
TEST(f360_internal_preprocessing, Update_Detection_Position_Opposite_Direction)
{
   /** \precond
   * - position in previous VCS is [4,2].
   * - deltax, delta_y = [1,-1], corresponding to host moving -pi/2 degrees along a circle of radius 1m
   * - Rotation matrix is set up for a rotation of -pi/2 radians.
   * - Expected position in new VCS is [-3,3]
   **/
   float32_t det_x_pos_vcs = 4.0F;
   float32_t det_y_pos_vcs = 2.0F;

   float32_t delta_x_old_vcs = 1.0F;
   float32_t delta_y_old_vcs = -1.0F;

   // Rotation matrix for clockwise rotation of -pi/2 radians:
   float32_t rot_mat[2][2] = { 0.0F };
   rot_mat[0U][0U] = 0.0F;
   rot_mat[0U][1U] = -1.0F;
   rot_mat[1U][0U] = 1.0F;
   rot_mat[1U][1U] = 0.0F;

   float32_t expected_x_pos = -3.0F;
   float32_t expected_y_pos = 3.0F;
   /** \action
   * Call Update_Detection_Position.
   **/
   Update_Detection_Position(delta_x_old_vcs, delta_y_old_vcs, rot_mat, det_x_pos_vcs, det_y_pos_vcs);

   /** \result
   * Checks that the position is correctly updated to new VCS.
   **/
   CHECK_EQUAL(expected_x_pos, det_x_pos_vcs);
   CHECK_EQUAL(expected_y_pos, det_y_pos_vcs);
}

/**
*\purpose Tests that a historical detection's position is updated to new VCS when host has moved backwards.
*\req NA
*/
TEST(f360_internal_preprocessing, Update_Detection_Position_Backwards_Movement)
{
   /** \precond
   * - position in previous VCS is [3,0].
   * - deltax, delta_y = [-1,-1], corresponding to host moving backwards pi/2 degrees along a circle of radius 1m
   * - Rotation matrix is set up for a rotation of pi/2 radians.
   * - Expected position in new VCS is [1,-4]
   **/
   float32_t det_x_pos_vcs = 3.0F;
   float32_t det_y_pos_vcs = 0.0F;

   float32_t delta_x_old_vcs = -1.0F;
   float32_t delta_y_old_vcs = -1.0F;

   // Rotation matrix for clockwise rotation of pi/2 radians:
   float32_t rot_mat[2][2] = { 0.0F };
   rot_mat[0U][0U] = 0.0F;
   rot_mat[0U][1U] = 1.0F;
   rot_mat[1U][0U] = -1.0F;
   rot_mat[1U][1U] = 0.0F;

   float32_t expected_x_pos = 1.0F;
   float32_t expected_y_pos = -4.0F;

   /** \action
   * Call Update_Detection_Position.
   **/
   Update_Detection_Position(delta_x_old_vcs, delta_y_old_vcs, rot_mat, det_x_pos_vcs, det_y_pos_vcs);

   /** \result
   * Checks that the position is correctly updated to new VCS.
   **/
   CHECK_EQUAL(expected_x_pos, det_x_pos_vcs);
   CHECK_EQUAL(expected_y_pos, det_y_pos_vcs);
}

/** \defgroup  f360_internal_preprocessing_update_detection_history_position
*  @{
*/

/** \brief
*  Sets up necessary structs for testing Update_Detection_History_Position().
**/
TEST_GROUP(f360_internal_preprocessingg_update_detection_history_position)
{
   /** \setup
   * Initialize common variables used within all tests for this test group
   **/
   const float32_t TEST_PASS_TH = 1e-6F;
   F360_Host_Props_T host_props = {};
   F360_Detection_Hist_T detection_hist = {};
   F360_Tracker_Info_T tracker_info = {};
   int32_t obj_idx;
   int32_t det_idx_1;

   TEST_SETUP()
   {
      Set_Tracker_Variant(tracker_info.variant);
      obj_idx = 0U;
      det_idx_1 = 0U;

      // Set initial value for a detection in Detection_Hist
      detection_hist.det_data[det_idx_1].vcs_position_x = -2.0F;
      detection_hist.det_data[det_idx_1].vcs_position_y = -1.0F;
      detection_hist.f_idx_occupied[det_idx_1] = true;
   }
};

/**
*\purpose Test that the Detection_Hist struct is correctly updated when Update_Detection_History_Position is
* called when host turns right. Checks that nothing is modified when the f_idx_occupied flag is set to false.
*\req NA
*/
TEST(f360_internal_preprocessingg_update_detection_history_position, Update_Detection_History_Position_Not_Occupied)
{
   /** \precond
   * - f_idx_occupied set to false even though a detection is stored.
   * - The detection has position [-2,-1] in old VCS.
   * - Host takes a pi/2 radians turn along a 1m radius circle.
   * -delta_x, delta_y = [1,1]  (aligned with VCS from previous tracker iteration)
   * - As f_idx_occupied is false, position should not be updated and expected position in new VCS is [-2,-1].
   **/
   detection_hist.f_idx_occupied[det_idx_1] = false;
   host_props.delta_pointing = F360_PI_2;
   host_props.cos_delta_pointing = 0.0F;
   host_props.sin_delta_pointing = 1.0F;
   host_props.delta_position_x = 1.0F;
   host_props.delta_position_y = 1.0F;

   float32_t expected_x_pos = -2.0F;
   float32_t expected_y_pos = -1.0F;
   /** \action
   * Call Update_Detection_History_Position().
   **/

   Update_Detections_History(host_props, tracker_info, detection_hist);

   /** \result
   * Checks that position is same in new and previous VCS.
   **/
   DOUBLES_EQUAL(expected_x_pos, detection_hist.det_data[det_idx_1].vcs_position_x, TEST_PASS_TH);
   DOUBLES_EQUAL(expected_y_pos, detection_hist.det_data[det_idx_1].vcs_position_y, TEST_PASS_TH);
}

/**
*\purpose Test that the Detection_Hist struct is correctly updated when Update_Detection_History_Position is
* called when host turns right. Two detections are tested.
*\req NA
*/
TEST(f360_internal_preprocessingg_update_detection_history_position, Update_Detection_History_Position_Det_Hist_Struct_Two_Dets)
{
   /** \precond
   * - The first detection has position [-2,-1] in previous VCS as set in test group.
   * - The second detection has position [-1,3] in old VCS.
   * - Host takes a pi/2 radians turn along a 1m radius circle.
   * - delta_x, delta_y = [1,1]  (aligned with VCS from previous tracker iteration)
   * - The expected positions of detections after host movement are [-2,3] and [2,2] respectively in the new VCS
   **/
   int32_t det_idx_2 = 5U;
   detection_hist.det_data[det_idx_2].vcs_position_x = -1.0F;
   detection_hist.det_data[det_idx_2].vcs_position_y = 3.0F;
   detection_hist.f_idx_occupied[det_idx_2] = true;

   host_props.delta_pointing = F360_PI_2;
   host_props.cos_delta_pointing = 0.0F;
   host_props.sin_delta_pointing = 1.0F;
   host_props.delta_position_x = 1.0F;
   host_props.delta_position_y = 1.0F;

   float32_t expected_x_pos_1 = -2.0F;
   float32_t expected_y_pos_1 = 3.0F;
   float32_t expected_x_pos_2 = 2.0F;
   float32_t expected_y_pos_2 = 2.0F;
   /** \action
   * Call Update_Detection_History_Position().
   **/

   Update_Detections_History(host_props, tracker_info, detection_hist);

   /** \result
   * Checks that detection's position are correct in new VCS.
   * All other detections should remain unchanged.
   **/
   DOUBLES_EQUAL(expected_x_pos_1, detection_hist.det_data[det_idx_1].vcs_position_x, TEST_PASS_TH);
   DOUBLES_EQUAL(expected_y_pos_1, detection_hist.det_data[det_idx_1].vcs_position_y, TEST_PASS_TH);

   DOUBLES_EQUAL(expected_x_pos_2, detection_hist.det_data[det_idx_2].vcs_position_x, TEST_PASS_TH);
   DOUBLES_EQUAL(expected_y_pos_2, detection_hist.det_data[det_idx_2].vcs_position_y, TEST_PASS_TH);

   // Check that all other positions remain unchanged (i.e. == 0 as initialized).
   float32_t expected_x_pos_unchanged = 0.0F;
   float32_t expected_y_pos_unchanged = 0.0F;
   for (int32_t det_idx = 0U; det_idx < MAX_NUMBER_OF_HISTORIC_DETECTIONS; det_idx++)
   {
      if (det_idx == det_idx_1 || det_idx == det_idx_2)
      {
         // Do nothing (already tested).
      }
      else
      {
         DOUBLES_EQUAL(expected_x_pos_unchanged, detection_hist.det_data[det_idx].vcs_position_x, TEST_PASS_TH);
         DOUBLES_EQUAL(expected_y_pos_unchanged, detection_hist.det_data[det_idx].vcs_position_y, TEST_PASS_TH);
      }
   }

}

/** @}*/

/** \defgroup  f360_internal_preprocessing_update_detection_history_azimuth
*  @{
*/

/** \brief
*  Sets up necessary structs for testing Update_Detection_History_Azimuth().
**/
TEST_GROUP(f360_internal_preprocessing_update_detection_history_azimuth)
{
   /** \setup
   * Initialize common variables used within all tests for this test group.
   **/
   const float32_t TEST_PASS_TH = 1e-6F;
   F360_Host_Props_T host_props = {};
   F360_Detection_Hist_T detection_hist = {};
};

/**
*\purpose  Tests that Update_Detection_History_Azimuth correctly updates vcs_azimuth according to host movement.
*\
*\req    NA
*/
TEST(f360_internal_preprocessing_update_detection_history_azimuth, Update_Detection_History_Azimuth)
{
   /** \precond
   * - Detection has an azimuth of -PI/4 radians in previous VCS.
   * - Host turns PI/4 degrees to the right, i.e. delta_pointing is set to PI/4.
   * - Expected azimuth is -PI/2 radians after host movement.
   **/
   host_props.delta_pointing = F360_PI / 4.0F;
   host_props.cos_delta_pointing = 0.707106781186547F;
   host_props.sin_delta_pointing = 0.707106781186547F;
   int32_t det_idx_1 = 0U;
   detection_hist.det_data[det_idx_1].vcs_az = -F360_PI / 4.0F;
   detection_hist.f_idx_occupied[det_idx_1] = true;

   float32_t expected_az = -F360_PI_2;

   /** \action
   * Call Update_Detection_History_Azimuth().
   **/
   for (uint32_t hist_det_idx = 0U; hist_det_idx < MAX_NUMBER_OF_HISTORIC_DETECTIONS; hist_det_idx++)
   {
      F360_Detection_Hist_Data_T &detection_history_data = detection_hist.det_data[hist_det_idx];
      if (detection_hist.f_idx_occupied[hist_det_idx])
      {
         Update_Detection_History_Azimuth(host_props, detection_history_data);
      }

   }

   /** \result
   * Checks that the azimuth after host movement is correct in new VCS.
   * All other detections should remain unchanged.
   **/
   DOUBLES_EQUAL(expected_az, detection_hist.det_data[det_idx_1].vcs_az, TEST_PASS_TH);

   // Check all other detections are unchanged (still == 0 as initialized).
   float32_t expected_az_unchanged = 0.0F;
   for (int32_t det_idx = 0U; det_idx < MAX_NUMBER_OF_HISTORIC_DETECTIONS; det_idx++)
   {
      if (det_idx == det_idx_1)
      {
         // Do nothing (already tested).
      }
      else
      {
         DOUBLES_EQUAL(expected_az_unchanged, detection_hist.det_data[det_idx].vcs_az, TEST_PASS_TH);
      }
   }
}

/**
*\purpose  Tests that Update_Detection_History_Azimuth correctly updates vcs_azimuth according to host movement in the case
* where f_idx_occupied is set to false.
*\
*\req    NA
*/
TEST(f360_internal_preprocessing_update_detection_history_azimuth, Update_Detection_History_Azimuth_Occupied_False)
{
   /** \precond
   * - Detection has an azimuth of -45 degrees in previous VCS.
   * - Host turns PI/4 degrees to the right.
   * - f_idx_occupied set to false.
   * - Expected azimuth is same as before function call (-PI/4) since f_idx_occupied is false and therefore it shouldn't be modified.
   **/
   host_props.delta_pointing = F360_PI / 4.0F;
   host_props.cos_delta_pointing = 0.707106781186547F;
   host_props.sin_delta_pointing = 0.707106781186547F;
   int32_t det_idx_1 = 0U;
   detection_hist.det_data[det_idx_1].vcs_az = -F360_PI / 4.0F;
   detection_hist.f_idx_occupied[det_idx_1] = false;

   float32_t expected_az = -F360_PI / 4.0F;
   /** \action
   * Call Update_Detection_History_Azimuth().
   **/
   for (uint32_t hist_det_idx = 0U; hist_det_idx < MAX_NUMBER_OF_HISTORIC_DETECTIONS; hist_det_idx++)
   {
      F360_Detection_Hist_Data_T &detection_history_data = detection_hist.det_data[hist_det_idx];
      if (detection_hist.f_idx_occupied[hist_det_idx])
      {
         Update_Detection_History_Azimuth(host_props, detection_history_data);
      }

   }

   /** \result
   * Checks that  azimuth after host movement is correct in new VCS.
   * All other detections should remain unchanged.
   **/
   DOUBLES_EQUAL(expected_az, detection_hist.det_data[det_idx_1].vcs_az, TEST_PASS_TH);

   // Check all other detections are unchanged (still == 0 as initialized).
   float32_t expected_az_unchanged = 0.0F;
   for (int32_t det_idx = 0U; det_idx < MAX_NUMBER_OF_HISTORIC_DETECTIONS; det_idx++)
   {
      if (det_idx == det_idx_1)
      {
         // Do nothing (already tested).
      }
      else
      {
         DOUBLES_EQUAL(expected_az_unchanged, detection_hist.det_data[det_idx].vcs_az, TEST_PASS_TH);
      }
   }
}

/**
*\purpose  Tests that Update_Detection_History_Azimuth correctly updates vcs_azimuth according to host movement.
* Two separate detections are tested at the same time.
*\
*\req    NA
*/
TEST(f360_internal_preprocessing_update_detection_history_azimuth, Update_Detection_History_Azimuth_Two_Dets)
{
   /** \precond
   * - First detection has an azimuth of -PI/4 radians in previous VCS.
   * - Second detection has an azimuth of 3PI/4 radians in previous VCS.
   * - Host turns PI/4 radians.
   * - Expected azimuths for the detections are -PI/2 and -PI respectively in new VCS.
   *
   **/
   host_props.delta_pointing = F360_PI / 4.0F;
   host_props.cos_delta_pointing = 0.707106781186547F;
   host_props.sin_delta_pointing = 0.707106781186547F;

   int32_t det_idx_1 = 0U;
   detection_hist.det_data[det_idx_1].vcs_az = -F360_PI / 4.0F;
   detection_hist.f_idx_occupied[det_idx_1] = true;

   int32_t det_idx_2 = 5U;
   detection_hist.det_data[det_idx_2].vcs_az = -3.0F * F360_PI / 4.0F;
   detection_hist.f_idx_occupied[det_idx_2] = true;

   float32_t expected_az_1 = -F360_PI_2;
   float32_t expected_az_2 = -F360_PI;
   /** \action
   * Call Update_Detection_History_Azimuth().
   **/

   for (uint32_t hist_det_idx = 0U; hist_det_idx < MAX_NUMBER_OF_HISTORIC_DETECTIONS; hist_det_idx++)
   {
      F360_Detection_Hist_Data_T &detection_history_data = detection_hist.det_data[hist_det_idx];
      if (detection_hist.f_idx_occupied[hist_det_idx])
      {
         Update_Detection_History_Azimuth(host_props, detection_history_data);
      }

   }

   /** \result
   * Checks that azimuth after host movement is correct in new VCS.
   * All other detections should remain unchanged.
   **/
   DOUBLES_EQUAL(expected_az_1, detection_hist.det_data[det_idx_1].vcs_az, TEST_PASS_TH);
   DOUBLES_EQUAL(expected_az_2, detection_hist.det_data[det_idx_2].vcs_az, TEST_PASS_TH);

   // Check all other detections are unchanged (still == 0 as initialized).
   float32_t expected_az_unchanged = 0.0F;
   for (int32_t det_idx = 0U; det_idx < MAX_NUMBER_OF_HISTORIC_DETECTIONS; det_idx++)
   {
      if (det_idx == det_idx_1 || det_idx == det_idx_2)
      {
         // Do nothing (already tested).
      }
      else
      {
         DOUBLES_EQUAL(expected_az_unchanged, detection_hist.det_data[det_idx].vcs_az, TEST_PASS_TH);
      }
   }
}
/** @}*/
