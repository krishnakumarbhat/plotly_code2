/** \file
 * This file contains unit tests for testing functions related to the f360_msmt_update_support_functions_common.cpp file
 */
#include "f360_msmt_update_support_functions_common.h"
#include "f360_clear_detections_props.h"
#include "f360_constants.h"
#include "f360_iterator.h"
#include <limits>

#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;


/** \defgroup  Select_Dets_For_RR_Update
*  @{
*/
/** \brief
*  The purpose of this test group is to verify that function Select_Dets_For_RR_Update works as intended.
**/
TEST_GROUP(Select_Dets_For_RR_Update)
{
   /** \setup
   * Declare all input parameters to Select_Dets_For_RR_Update, these will be reused in all tests in this group.
   **/
   rspp_variant_A::RSPP_Detection_T detections[MAX_NUMBER_OF_DETECTIONS];
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Object_Track_T object_track;
   uint32_t k_min_num_selected_dets_per_sensor_for_binning;
   uint32_t selected_dets_idx[MAX_DETS_IN_OBJ_TRK];
   uint32_t selected_dets_num;
   uint32_t selected_dets_score[MSMT_UPDATE_MAX_NUM_OF_MSMT]{};

   /** \setup
   * Before each test:
   * - Clear detections props by calling Clear_Detections_Props(det_props)
   * - Set k_min_num_selected_dets_per_sensor_for_binning to 5
   * - Set object_track.movable_prob to 0.0
   * - Set object_track.ndets to 7
   * - Set object_track.detids[0] to 1
   * - Set object_track.detids[1] to 2
   * - Set object_track.detids[2] to 3
   * - Set object_track.detids[3] to 4
   * - Set object_track.detids[4] to 5
   * - Set object_track.detids[5] to 6
   * - Set object_track.detids[6] to 7
   * - Set det_props[0].f_rr_inlier to true
   * - Set det_props[1].f_rr_inlier to false
   * - Set det_props[2].f_rr_inlier to true
   * - Set det_props[3].f_rr_inlier to true
   * - Set det_props[4].f_rr_inlier to true
   * - Set det_props[5].f_rr_inlier to true
   * - Set det_props[6].f_rr_inlier to true
   * - Set detections[0].sensor_id to 1
   * - Set detections[1].sensor_id to 1
   * - Set detections[2].sensor_id to 1
   * - Set detections[3].sensor_id to 1
   * - Set detections[4].sensor_id to 1
   * - Set detections[5].sensor_id to 1
   * - Set detections[6].sensor_id to 1
   * - Set detections[0].azimuth to 0.0F
   * - Set detections[1].azimuth to 0.01F
   * - Set detections[2].azimuth to 0.01F
   * - Set detections[3].azimuth to 0.011F
   * - Set detections[4].azimuth to 0.12F
   * - Set detections[5].azimuth to 0.13F
   * - Set detections[6].azimuth to 1.0F
   */
   TEST_SETUP()
   {
      Clear_Detections_Props(det_props);

      k_min_num_selected_dets_per_sensor_for_binning = 5U;

      object_track.movable_prob = 0.0F;

      object_track.ndets = 7;
      object_track.detids[0] = 1;
      object_track.detids[1] = 2;
      object_track.detids[2] = 3;
      object_track.detids[3] = 4;
      object_track.detids[4] = 5;
      object_track.detids[5] = 6;
      object_track.detids[6] = 7;
      
      det_props[0].f_rr_inlier = true;
      det_props[1].f_rr_inlier = false;
      det_props[2].f_rr_inlier = true;
      det_props[3].f_rr_inlier = true;
      det_props[4].f_rr_inlier = true;
      det_props[5].f_rr_inlier = true;
      det_props[6].f_rr_inlier = true;

      detections[0].raw.sensor_id = 1;
      detections[1].raw.sensor_id = 1; // not selected by track
      detections[2].raw.sensor_id = 1;
      detections[3].raw.sensor_id = 1;
      detections[4].raw.sensor_id = 1;
      detections[5].raw.sensor_id = 1;
      detections[6].raw.sensor_id = 1;

      detections[0].raw.azimuth = 0.0F; // bin 1
      detections[1].raw.azimuth = 0.01F; // not selected by track
      detections[2].raw.azimuth = 0.01F; // bin 1
      detections[3].raw.azimuth = 0.011F; // bin 1, median az
      detections[4].raw.azimuth = 0.12F; // bin 1
      detections[5].raw.azimuth = 0.13F; // bin 1
      detections[6].raw.azimuth = 1.0F; // bin 5

      std::fill(cmn::begin(selected_dets_score), cmn::end(selected_dets_score), 1U);
   }
};

/*\purpose
* Verify selected_dets_num is set to 0 when an object has no associated detections
* \req NA
*/
TEST(Select_Dets_For_RR_Update, Check_Selected_Dets_Num_Is_Set_To_Zero_When_There_Are_No_Dets)
{

   /** \precond
    * - Set selected_dets_num to some number != 0
    * - Set object_track.ndets to 0
    */
   selected_dets_num = 123U;
   object_track.ndets = 0;

   /** \action
   * Call the function Select_Dets_For_RR_Update
   **/
   Select_Dets_For_RR_Update(
      detections,
      k_min_num_selected_dets_per_sensor_for_binning,
      det_props,
      object_track,
      selected_dets_idx,
      selected_dets_num,
      selected_dets_score);

   /** \result
   * Verify selected_dets_num is set to 0
   **/
   CHECK_EQUAL_TEXT(0U, selected_dets_num, "Number of selected detections does not match expected data");
}

/*\purpose
* Verify object_track.num_dets_used_in_rr_msmt_update is set to 0 when selected_dets_num is 0
* \req NA
*/
TEST(Select_Dets_For_RR_Update, Check_Num_Dets_Used_In_RR_Msmt_Update_Is_Set_To_Zero_When_There_Are_No_Dets)
{

   /** \precond
    * - Set object_track.num_dets_used_in_rr_msmt_update to some number != 0
    * - Set object_track.ndets to 0
    */
   object_track.num_dets_used_in_rr_msmt_update = 123U;
   object_track.ndets = 0;

   /** \action
   * Call the function Select_Dets_For_RR_Update
   **/
   Select_Dets_For_RR_Update(
      detections,
      k_min_num_selected_dets_per_sensor_for_binning,
      det_props,
      object_track,
      selected_dets_idx,
      selected_dets_num,
      selected_dets_score);

   /** \result
   * Verify object_track.num_dets_used_in_rr_msmt_update is set to 0
   **/
   CHECK_EQUAL_TEXT(0U, object_track.num_dets_used_in_rr_msmt_update, "Number of selected detections does not match expected data");
}

/*\purpose
* Verify no detection is marked as f_used_in_rr_msmt_update when selected_dets_num is 0
* \req NA
*/
TEST(Select_Dets_For_RR_Update, Check_No_Det_Is_Marked_As_Used_In_RR_Msmt_Update_When_There_Are_No_Dets)
{

   /** \precond
    * - Set object_track.ndets to 0
    */
   object_track.ndets = 0;

   /** \action
   * Call the function Select_Dets_For_RR_Update
   **/
   Select_Dets_For_RR_Update(
      detections,
      k_min_num_selected_dets_per_sensor_for_binning,
      det_props,
      object_track,
      selected_dets_idx,
      selected_dets_num,
      selected_dets_score);

   /** \result
   * Verify that no detection in det_props is marked as f_used_in_rr_msmt_update
   **/
  for (uint32_t i_det = 0; i_det < MAX_NUMBER_OF_DETECTIONS; i_det++)
  {
      CHECK_FALSE_TEXT(det_props[i_det].f_used_in_rr_msmt_update, "A detection was marked as f_used_in_rr_msmt_update when expected not to.");
  }
}

/*\purpose
* Verify that only detections marked as f_selected_by_track are selected
* \req NA
*/
TEST(Select_Dets_For_RR_Update, Check_Only_Dets_Marked_As_Select_By_Trk_Is_Selected)
{

   /** \precond
    * - Nothing else required than Test_Setup()
    */

   /** \action
   * Call the function Select_Dets_For_RR_Update
   **/
   Select_Dets_For_RR_Update(
      detections,
      k_min_num_selected_dets_per_sensor_for_binning,
      det_props,
      object_track,
      selected_dets_idx,
      selected_dets_num,
      selected_dets_score);

   /** \result
   * - Verify selected_dets_num is set to 6
   * - Verify 0U is equal to selected_dets_idx[0]
   * - Verify 2U is equal to selected_dets_idx[1]
   * - Verify 3U is equal to selected_dets_idx[2]
   * - Verify 4U is equal to selected_dets_idx[3]
   * - Verify 5U is equal to selected_dets_idx[4]
   * - Verify 6U is equal to selected_dets_idx[5]
   **/
   CHECK_EQUAL_TEXT(6U, selected_dets_num, "Number of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(0U, selected_dets_idx[0], "Index of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(2U, selected_dets_idx[1], "Index of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(3U, selected_dets_idx[2], "Index of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(4U, selected_dets_idx[3], "Index of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(5U, selected_dets_idx[4], "Index of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(6U, selected_dets_idx[5], "Index of selected detections does not match expected data");
}

/*\purpose
* Verify that object.num_dets_used_in_rr_msmt_update is updated as expected when there are detections
* \req NA
*/
TEST(Select_Dets_For_RR_Update, Check_Num_Dets_Used_In_RR_Msmt_Update_Is_Updated_As_Expected)
{

   /** \precond
    * - Nothing else required than Test_Setup()
    */

   /** \action
   * Call the function Select_Dets_For_RR_Update
   **/
   Select_Dets_For_RR_Update(
      detections,
      k_min_num_selected_dets_per_sensor_for_binning,
      det_props,
      object_track,
      selected_dets_idx,
      selected_dets_num,
      selected_dets_score);

   /** \result
   * - Verify object_track.num_dets_used_in_rr_msmt_update is equal to selected_dets_num which is 6
   **/
   CHECK_EQUAL_TEXT(6U, object_track.num_dets_used_in_rr_msmt_update, "Number of selected detections does not match expected data");
}

/*\purpose
* Verify that det_prop.f_used_in_rr_msmt_update is updated as expected when there are detections
* \req NA
*/
TEST(Select_Dets_For_RR_Update, Check_F_Used_In_RR_Msmt_Update_Is_Updated_As_Expected)
{

   /** \precond
    * - Nothing else required than Test_Setup()
    */

   /** \action
   * Call the function Select_Dets_For_RR_Update
   **/
   Select_Dets_For_RR_Update(
      detections,
      k_min_num_selected_dets_per_sensor_for_binning,
      det_props,
      object_track,
      selected_dets_idx,
      selected_dets_num,
      selected_dets_score);

   /** \result
   * - Verify that det_props index 0, 2, 3, 4, 5, 6 have f_used_in_rr_msmt_update set to true
   * - Verify that for all other detections in det_props, f_used_in_rr_msmt_update is set to false
   **/
   CHECK_TRUE_TEXT(det_props[0].f_used_in_rr_msmt_update, "A detection was not marked as f_used_in_rr_msmt_update when expected not to.");
   CHECK_FALSE_TEXT(det_props[1].f_used_in_rr_msmt_update, "A detection was marked as f_used_in_rr_msmt_update when expected not to.");
   CHECK_TRUE_TEXT(det_props[2].f_used_in_rr_msmt_update, "A detection was not marked as f_used_in_rr_msmt_update when expected not to.");
   CHECK_TRUE_TEXT(det_props[3].f_used_in_rr_msmt_update, "A detection was not marked as f_used_in_rr_msmt_update when expected not to.");
   CHECK_TRUE_TEXT(det_props[4].f_used_in_rr_msmt_update, "A detection was not marked as f_used_in_rr_msmt_update when expected not to.");
   CHECK_TRUE_TEXT(det_props[5].f_used_in_rr_msmt_update, "A detection was not marked as f_used_in_rr_msmt_update when expected not to.");
   CHECK_TRUE_TEXT(det_props[6].f_used_in_rr_msmt_update, "A detection was not marked as f_used_in_rr_msmt_update when expected not to.");

   for (uint32_t i_det = 7; i_det < MAX_NUMBER_OF_DETECTIONS; i_det++)
   {
      CHECK_FALSE_TEXT(det_props[i_det].f_used_in_rr_msmt_update, "A detection was marked as f_used_in_rr_msmt_update when expected not to.");
   }
}

/*\purpose
* Verify that Try_To_Decrease_Num_Selected_Dets is called when an object is moveable and has enough dets
* \req NA
*/
TEST(Select_Dets_For_RR_Update, Check_Try_To_Decrease_Num_Selected_Dets_Is_Called_Correctly_Obj_Moveable_And_Enough_Dets)
{

   /** \precond
    * - Set object_track.movable_prob to 1.0
    */
   object_track.movable_prob = 1.0F;

   /** \action
   * Call the function Select_Dets_For_RR_Update
   **/
   Select_Dets_For_RR_Update(
      detections,
      k_min_num_selected_dets_per_sensor_for_binning,
      det_props,
      object_track,
      selected_dets_idx,
      selected_dets_num,
      selected_dets_score);

   /** \result
   * - Verify 2U is equal to selected_dets_num
   * - Verify 3U is equal to selected_dets_idx[0]
   * - Verify 6U is equal to selected_dets_idx[1]
   **/
   CHECK_EQUAL_TEXT(2U, selected_dets_num, "Number of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(3U, selected_dets_idx[0], "Index of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(6U, selected_dets_idx[1], "Index of selected detections does not match expected data");
}

/*\purpose
* Verify that Try_To_Decrease_Num_Selected_Dets is not called when an object is moveable but does not have enough dets
* \req NA
*/
TEST(Select_Dets_For_RR_Update, Check_Try_To_Decrease_Num_Selected_Dets_Is_Called_Correctly_Obj_Moveable_But_Not_Enough_Dets)
{
   /** \precond
    * - Set object_track.movable_prob to 1.0
    * - Set k_min_num_selected_dets_per_sensor_for_binning to 6
    */
   object_track.movable_prob = 1.0F;
   k_min_num_selected_dets_per_sensor_for_binning = 6U;

   /** \action
   * Call the function Select_Dets_For_RR_Update
   **/
   Select_Dets_For_RR_Update(
      detections,
      k_min_num_selected_dets_per_sensor_for_binning,
      det_props,
      object_track,
      selected_dets_idx,
      selected_dets_num,
      selected_dets_score);

   /** \result
   * Check that function have selected correct detections by
   * - verifying 6U is equal to selected_dets_num
   * - verifying 0U is equal to selected_dets_idx[0]
   * - verifying 2U is equal to selected_dets_idx[1]
   * - verifying 3U is equal to selected_dets_idx[2]
   * - verifying 4U is equal to selected_dets_idx[3]
   * - verifying 5U is equal to selected_dets_idx[4]
   * - verifying 6U is equal to selected_dets_idx[5]
   **/
   CHECK_EQUAL_TEXT(6U, selected_dets_num, "Number of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(0U, selected_dets_idx[0], "Index of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(2U, selected_dets_idx[1], "Index of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(3U, selected_dets_idx[2], "Index of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(4U, selected_dets_idx[3], "Index of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(5U, selected_dets_idx[4], "Index of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(6U, selected_dets_idx[5], "Index of selected detections does not match expected data");
}

/*\purpose
* Verify that Try_To_Decrease_Num_Selected_Dets is not called when an object is not moveable but have enough dets
* \req NA
*/
TEST(Select_Dets_For_RR_Update, Check_Try_To_Decrease_Num_Selected_Dets_Is_Called_Correctly_Obj_Not_Moveable_But_Enough_Dets)
{
   /** \precond
    * - Nothing else required than Test_Setup()
    */

   /** \action
   * Call the function Select_Dets_For_RR_Update
   **/
   Select_Dets_For_RR_Update(
      detections,
      k_min_num_selected_dets_per_sensor_for_binning,
      det_props,
      object_track,
      selected_dets_idx,
      selected_dets_num,
      selected_dets_score);

   /** \result
   * Check that function have selected correct detections
   **/
   CHECK_EQUAL_TEXT(6U, selected_dets_num, "Number of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(0U, selected_dets_idx[0], "Index of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(2U, selected_dets_idx[1], "Index of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(3U, selected_dets_idx[2], "Index of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(4U, selected_dets_idx[3], "Index of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(5U, selected_dets_idx[4], "Index of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(6U, selected_dets_idx[5], "Index of selected detections does not match expected data");
}

/*\purpose
* Verify that object.num_dets_used_in_rr_msmt_update is updated after Try_To_Decrease_Num_Selected_Dets()
* \req NA
*/
TEST(Select_Dets_For_RR_Update, Verify_Num_Dets_Used_In_RR_Msmt_Update_Is_Updated_After_Try_To_Decrease_Num_Selected_Dets)
{
   /** \precond
    * - set object_track.movable_prob = 1.0F;
    */
   object_track.movable_prob = 1.0F;

   /** \action
   * Call the function Select_Dets_For_RR_Update
   **/
   Select_Dets_For_RR_Update(
      detections,
      k_min_num_selected_dets_per_sensor_for_binning,
      det_props,
      object_track,
      selected_dets_idx,
      selected_dets_num,
      selected_dets_score);

   /** \result
   * - Verify object_track.num_dets_used_in_rr_msmt_update is equal to selected_dets_num which is 2
   **/
   CHECK_EQUAL_TEXT(2U, object_track.num_dets_used_in_rr_msmt_update, "Number of selected detections does not match expected data");
}

/*\purpose
* Verify that det_prop.f_used_in_rr_msmt_update is updated after Try_To_Decrease_Num_Selected_Dets()
* \req NA
*/
TEST(Select_Dets_For_RR_Update, Check_F_Used_In_RR_Msmt_Update_Is_Updated_After_Try_To_Decrease_Num_Selected_Dets)
{
   /** \precond
    * - set object_track.movable_prob = 1.0F;
    */
   object_track.movable_prob = 1.0F;

   /** \action
   * Call the function Select_Dets_For_RR_Update
   **/
   Select_Dets_For_RR_Update(
      detections,
      k_min_num_selected_dets_per_sensor_for_binning,
      det_props,
      object_track,
      selected_dets_idx,
      selected_dets_num,
      selected_dets_score);

   /** \result
   * - Verify that det_props index 3, 6 have f_used_in_rr_msmt_update set to true
   * - Verify that for all other detections in det_props, f_used_in_rr_msmt_update is set to false
   **/
   CHECK_FALSE_TEXT(det_props[0].f_used_in_rr_msmt_update, "A detection was not marked as f_used_in_rr_msmt_update when expected not to.");
   CHECK_FALSE_TEXT(det_props[1].f_used_in_rr_msmt_update, "A detection was marked as f_used_in_rr_msmt_update when expected not to.");
   CHECK_FALSE_TEXT(det_props[2].f_used_in_rr_msmt_update, "A detection was not marked as f_used_in_rr_msmt_update when expected not to.");
   CHECK_TRUE_TEXT(det_props[3].f_used_in_rr_msmt_update, "A detection was not marked as f_used_in_rr_msmt_update when expected not to.");
   CHECK_FALSE_TEXT(det_props[4].f_used_in_rr_msmt_update, "A detection was not marked as f_used_in_rr_msmt_update when expected not to.");
   CHECK_FALSE_TEXT(det_props[5].f_used_in_rr_msmt_update, "A detection was not marked as f_used_in_rr_msmt_update when expected not to.");
   CHECK_TRUE_TEXT(det_props[6].f_used_in_rr_msmt_update, "A detection was not marked as f_used_in_rr_msmt_update when expected not to.");

   for (uint32_t i_det = 7; i_det < MAX_NUMBER_OF_DETECTIONS; i_det++)
   {
      CHECK_FALSE_TEXT(det_props[i_det].f_used_in_rr_msmt_update, "A detection was marked as f_used_in_rr_msmt_update when expected not to.");
   }
}

/** @}*/


/** \defgroup  Extract_Detection_Properties_Per_Sensor
 *  @{
 */
/** \brief
 * Test group for unittesting the function Extract_Detection_Properties_Per_Sensor()
 */
TEST_GROUP(Extract_Detection_Properties_Per_Sensor)
{  
   /** \setup
   * Declare all input parameters to Extract_Detection_Properties_Per_Sensor, these will be reused in all tests in this group.
   **/
   rspp_variant_A::RSPP_Detection_T detections[MAX_NUMBER_OF_DETECTIONS];
   uint32_t selected_dets_idx[MAX_DETS_IN_OBJ_TRK];
   uint32_t selected_dets_num;
   float32_t min_det_az_per_sensor[MAX_NUMBER_OF_SENSORS];
   float32_t max_det_az_per_sensor[MAX_NUMBER_OF_SENSORS];
   uint32_t det_idx_per_sensor[MAX_NUMBER_OF_SENSORS][MAX_DETS_IN_OBJ_TRK];
   uint32_t num_det_per_sensor[MAX_NUMBER_OF_SENSORS];

   /** \setup
   * Define support variables that will improve test readability by
   * - Defining num_exp_dets_sens3 = 3
   * - Defining selected_det_idx_sens3_min_az = 10
   * - Defining selected_det_idx_sens3_mid_az = 20
   * - Defining selected_det_idx_sens3_max_az = 30
   * - Defining num_exp_dets_sens2 = 2
   * - Defining selected_det_idx_sens2_min_az = 40
   * - Defining selected_det_idx_sens2_max_az = 50
   * - Defining num_exp_dets_sens1 = 1
   * - Defining selected_det_idx_sens1_min_max_az = 60
   * - Defining max_az_sens3 = 1.0
   * - Defining min_az_sens3 = -1.0
   * - Defining max_az_sens2 = 0.0
   * - Defining min_az_sens2 = -1.0
   * - Defining max_min_az_sens1 = 0.0
   **/
   const uint32_t num_exp_dets_sens3 = 3U;
   const uint32_t selected_det_idx_sens3_min_az = 10U;
   const uint32_t selected_det_idx_sens3_mid_az = 20U;
   const uint32_t selected_det_idx_sens3_max_az = 30U;

   const uint32_t num_exp_dets_sens2 = 2U;
   const uint32_t selected_det_idx_sens2_min_az = 40U;
   const uint32_t selected_det_idx_sens2_max_az = 50U;

   const uint32_t num_exp_dets_sens1 = 1U;
   const uint32_t selected_det_idx_sens1_min_max_az = 60U;

   const float32_t max_az_sens3 = 1.0F;
   const float32_t min_az_sens3 = -1.0F;

   const float32_t max_az_sens2 = 0.0F;
   const float32_t min_az_sens2 = -1.0F;

   const float32_t max_min_az_sens1 = 0.0F;

   /** \setup
    * Before each test:
    * - Set selected_dets_idx[0] = selected_det_idx_sens3_min_az
    * - Set selected_dets_idx[1] = selected_det_idx_sens3_mid_az
    * - Set selected_dets_idx[2] = selected_det_idx_sens3_max_az
    * - Set selected_dets_idx[3] = selected_det_idx_sens2_min_az
    * - Set selected_dets_idx[4] = selected_det_idx_sens2_max_az
    * - Set selected_dets_idx[5] = selected_det_idx_sens1_min_max_az
    * - Set selected_dets_num = 6;
    * - Set detections[selected_det_idx_sens3_min_az].sensor_id = 3
    * - Set detections[selected_det_idx_sens3_mid_az].sensor_id = 3
    * - Set detections[selected_det_idx_sens3_max_az].sensor_id = 3
    * - Set detections[selected_det_idx_sens3_min_az].azimuth = min_az_sens3
    * - Set detections[selected_det_idx_sens3_mid_az].azimuth = 0.0
    * - Set detections[selected_det_idx_sens3_max_az].azimuth = max_az_sens3
    * - Set detections[selected_det_idx_sens2_min_az].sensor_id = 2
    * - Set detections[selected_det_idx_sens2_max_az].sensor_id = 2
    * - Set detections[selected_det_idx_sens2_min_az].azimuth = min_az_sens2
    * - Set detections[selected_det_idx_sens2_max_az].azimuth = max_az_sens2
    * - Set detections[selected_det_idx_sens1_min_max_az].sensor_id = 1
    * - Set detections[selected_det_idx_sens1_min_max_az].azimuth = max_min_az_sens1
    */
   TEST_SETUP()
   {
      selected_dets_idx[0] = selected_det_idx_sens3_min_az;
      selected_dets_idx[1] = selected_det_idx_sens3_mid_az;
      selected_dets_idx[2] = selected_det_idx_sens3_max_az;
      selected_dets_idx[3] = selected_det_idx_sens2_min_az;
      selected_dets_idx[4] = selected_det_idx_sens2_max_az;
      selected_dets_idx[5] = selected_det_idx_sens1_min_max_az;
      selected_dets_num = 6;

      // Sensor id 3
      detections[selected_det_idx_sens3_min_az].raw.sensor_id = 3;
      detections[selected_det_idx_sens3_mid_az].raw.sensor_id = 3;
      detections[selected_det_idx_sens3_max_az].raw.sensor_id = 3;
      detections[selected_det_idx_sens3_min_az].raw.azimuth = min_az_sens3;
      detections[selected_det_idx_sens3_mid_az].raw.azimuth = 0.0F;
      detections[selected_det_idx_sens3_max_az].raw.azimuth = max_az_sens3;
      
      // Sensor id 2
      detections[selected_det_idx_sens2_min_az].raw.sensor_id = 2;
      detections[selected_det_idx_sens2_max_az].raw.sensor_id = 2;
      detections[selected_det_idx_sens2_min_az].raw.azimuth = min_az_sens2;
      detections[selected_det_idx_sens2_max_az].raw.azimuth = max_az_sens2;

      // Sensor id 1
      detections[selected_det_idx_sens1_min_max_az].raw.sensor_id = 1;
      detections[selected_det_idx_sens1_min_max_az].raw.azimuth = max_min_az_sens1;
   }
   
};

/** \purpose  
 * Verify that the number of detections for each sensor is set to 0 when there are no selected dets
 * \req NA
 */
TEST(Extract_Detection_Properties_Per_Sensor, Check_Correct_Num_Dets_Per_Sensor_When_There_Are_No_Detections)
{
   /** \precond
    * - Set all entries in num_det_per_sensor to some value > 0
    * - Set selected_dets_num to 0
    */
   selected_dets_num = 0U;
   std::fill(cmn::begin(num_det_per_sensor), cmn::end(num_det_per_sensor), 9U);

   /** \action
    * Call Extract_Detection_Properties_Per_Sensor()
    */
   Extract_Detection_Properties_Per_Sensor(detections, selected_dets_idx, selected_dets_num, min_det_az_per_sensor, max_det_az_per_sensor, det_idx_per_sensor, num_det_per_sensor);
   
   /** \result
    * - Verify all entries in num_det_per_sensor is equal to 0
    */
   for (int32_t i_sensor = 0; i_sensor < MAX_NUMBER_OF_SENSORS; i_sensor++)
   {
      CHECK_EQUAL_TEXT(0U, num_det_per_sensor[i_sensor], "The number of detections from a sensor is != 0 when 0 is expected.");
   } 
}

/** \purpose  
 * Verify that the min azimuth was set to the correct value when there are no selected dets for each sensor
 * \req NA
 */
TEST(Extract_Detection_Properties_Per_Sensor, Check_Correct_Min_Az_For_Sensor_When_There_Are_No_Detections)
{
   /** \precond
    * - Set selected_dets_num to 0
    * - Set all entries in min_det_az_per_sensor to some value != 2*pi
    */
   selected_dets_num = 0U;
   std::fill(cmn::begin(min_det_az_per_sensor), cmn::end(min_det_az_per_sensor), 0.0F);
   
   /** \action
    * Call Extract_Detection_Properties_Per_Sensor()
    */
   Extract_Detection_Properties_Per_Sensor(detections, selected_dets_idx, selected_dets_num, min_det_az_per_sensor, max_det_az_per_sensor, det_idx_per_sensor, num_det_per_sensor);
   
   /** \result
    * - Verify the min azimuth is equal to F360_2PI (2*pi) for all sensors
    */
   for (int32_t i_sensor = 0; i_sensor < MAX_NUMBER_OF_SENSORS; i_sensor++)
   {
      DOUBLES_EQUAL_TEXT(F360_2PI, min_det_az_per_sensor[i_sensor], F360_EPSILON, "The min azimuth for a sensor with no detections is not correct.");
   }
}

/** \purpose  
 * Verify that the max azimuth was set to the correct value when there are no selected dets for each sensor
 * \req NA
 */
TEST(Extract_Detection_Properties_Per_Sensor, Check_Correct_Max_Az_For_Sensor_When_There_Are_No_Detections)
{
   /** \precond
    * - Set selected_dets_num to 0
    * - Set all entries in max_det_az_per_sensor to some value != 2*pi
    */
   selected_dets_num = 0U;
   std::fill(cmn::begin(max_det_az_per_sensor), cmn::end(max_det_az_per_sensor), 0.0F);
   
   /** \action
    * Call Extract_Detection_Properties_Per_Sensor()
    */
   Extract_Detection_Properties_Per_Sensor(detections, selected_dets_idx, selected_dets_num, min_det_az_per_sensor, max_det_az_per_sensor, det_idx_per_sensor, num_det_per_sensor);
   
   /** \result
    * - Verify the mix azimuth is equal to -F360_2PI (-2*pi) for all sensors
    */
   for (int32_t i_sensor = 0; i_sensor < MAX_NUMBER_OF_SENSORS; i_sensor++)
   {
      DOUBLES_EQUAL_TEXT(-F360_2PI, max_det_az_per_sensor[i_sensor], F360_EPSILON, "The max azimuth for a sensor with no detections is not correct.");
   }
}


/** \purpose  
 * Verify that all entries in det_idx_per_sensor are unchanged when there are no selected dets for each sensor
 * \req NA
 */
TEST(Extract_Detection_Properties_Per_Sensor, Check_Det_Indices_Per_Sensor_Are_Unchanged_When_There_Are_No_Detections)
{
   /** \precond
    * - Set selected_dets_num to 0
    * - Set all entries in det_idx_per_sensor to some value
    */
   selected_dets_num = 0U;
   for (uint32_t i_sensor = 0; i_sensor < MAX_NUMBER_OF_SENSORS; i_sensor++)
   {
      std::fill(cmn::begin(det_idx_per_sensor[i_sensor]), cmn::end(det_idx_per_sensor[i_sensor]), 99U);
   }
   
   /** \action
    * Call Extract_Detection_Properties_Per_Sensor()
    */
   Extract_Detection_Properties_Per_Sensor(detections, selected_dets_idx, selected_dets_num, min_det_az_per_sensor, max_det_az_per_sensor, det_idx_per_sensor, num_det_per_sensor);
   
   /** \result
    * - Verify all entries in det_idx_per_sensor are unchanged from test setup
    */
   for (uint32_t i_sensor = 0; i_sensor < MAX_NUMBER_OF_SENSORS; i_sensor++)
   {
      for (uint32_t i_det = 0; i_det < MAX_DETS_IN_OBJ_TRK; i_det++)
      {
            CHECK_EQUAL_TEXT(99U, det_idx_per_sensor[i_sensor][i_det], "det_idx_per_sensor was changed after function call when it was expected not to.");
      }
   }
}

/** \purpose  
 * Verify that the correct number of detections was extracted for each sensor
 * \req NA
 */
TEST(Extract_Detection_Properties_Per_Sensor, Check_Correct_Num_Dets_Per_Sensor_When_There_Are_Detections)
{
   /** \precond
    * Nothing additional to Test_Setup()
    */
   
   /** \action
    * Call Extract_Detection_Properties_Per_Sensor()
    */
   Extract_Detection_Properties_Per_Sensor(detections, selected_dets_idx, selected_dets_num, min_det_az_per_sensor, max_det_az_per_sensor, det_idx_per_sensor, num_det_per_sensor);
   
   /** \result
    * - Verify there are 1 det for sensor 1
    * - Verify there are 2 dets for sensor 2
    * - Verify there are 3 dets for sensor 3
    * - Verify there are 0 dets for all other sensors
    */
   CHECK_EQUAL_TEXT(num_exp_dets_sens1, num_det_per_sensor[0], "The number of detections from sensor 1 is not correct.");
   CHECK_EQUAL_TEXT(num_exp_dets_sens2, num_det_per_sensor[1], "The number of detections from sensor 2 is not correct.");
   CHECK_EQUAL_TEXT(num_exp_dets_sens3, num_det_per_sensor[2], "The number of detections from sensor 3 is not correct.");
   for (int32_t i_sensor = 3; i_sensor < MAX_NUMBER_OF_SENSORS; i_sensor++)
   {
      CHECK_EQUAL_TEXT(0U, num_det_per_sensor[i_sensor], "The number of detections from a sensor is != 0 when 0 is expected.");
   } 
}

/** \purpose  
 * Verify that the correct min azimuth was selected per sensor
 * \req NA
 */
TEST(Extract_Detection_Properties_Per_Sensor, Check_Correct_Min_Az_For_Sensor_When_There_Are_Detections)
{
   /** \precond
    * Nothing additional to Test_Setup()
    */
   
   /** \action
    * Call Extract_Detection_Properties_Per_Sensor()
    */
   Extract_Detection_Properties_Per_Sensor(detections, selected_dets_idx, selected_dets_num, min_det_az_per_sensor, max_det_az_per_sensor, det_idx_per_sensor, num_det_per_sensor);
   
   /** \result
    * - Verify that for sensor id 1, the min azimuth is equal to selected_det_idx_sens1_min_max_az's azimuth
    * - Verify that for sensor id 2, the min azimuth is equal to selected_det_idx_sens2_min_az's azimuth
    * - Verify that for sensor id 3, the min azimuth is equal to selected_det_idx_sens3_min_az's azimuth
    * - Verify the min azimuth is equal to F360_2PI (2*pi) for all other sensors
    */
   DOUBLES_EQUAL_TEXT(max_min_az_sens1, min_det_az_per_sensor[0], F360_EPSILON, "The min azimuth for a sensor is not correct.");
   DOUBLES_EQUAL_TEXT(min_az_sens2, min_det_az_per_sensor[1], F360_EPSILON, "The min azimuth for a sensor is not correct.");
   DOUBLES_EQUAL_TEXT(min_az_sens3, min_det_az_per_sensor[2], F360_EPSILON, "The min azimuth for a sensor is not correct.");
   for (int32_t i_sensor = 3; i_sensor < MAX_NUMBER_OF_SENSORS; i_sensor++)
   {
      DOUBLES_EQUAL_TEXT(F360_2PI, min_det_az_per_sensor[i_sensor], F360_EPSILON, "The min azimuth for a sensor with no detections is not correct.");
   }
   
}

/** \purpose  
 * Verify that the correct max azimuth is selected for each sensor
 * \req NA
 */
TEST(Extract_Detection_Properties_Per_Sensor, Check_Correct_Max_Az_For_Sensor_When_There_Are_Detections)
{
   /** \precond
    * Nothing additional to Test_Setup()
    */
   
   /** \action
    * Call Extract_Detection_Properties_Per_Sensor()
    */
   Extract_Detection_Properties_Per_Sensor(detections, selected_dets_idx, selected_dets_num, min_det_az_per_sensor, max_det_az_per_sensor, det_idx_per_sensor, num_det_per_sensor);
   
   /** \result
    * - Verify that for sensor id 1, the max azimuth is equal to selected_det_idx_sens1_min_max_az's azimuth
    * - Verify that for sensor id 2, the max azimuth is equal to selected_det_idx_sens2_max_az's azimuth
    * - Verify that for sensor id 3, the max azimuth is equal to selected_det_idx_sens3_max_az's azimuth
    * - Verify the max azimuth is equal to -F360_2PI (-2*pi) for all other sensors
    */
   DOUBLES_EQUAL_TEXT(max_min_az_sens1, max_det_az_per_sensor[0], F360_EPSILON, "The max azimuth for a sensor is not correct.");
   DOUBLES_EQUAL_TEXT(max_az_sens2, max_det_az_per_sensor[1], F360_EPSILON, "The max azimuth for a sensor is not correct.");
   DOUBLES_EQUAL_TEXT(max_az_sens3, max_det_az_per_sensor[2], F360_EPSILON, "The max azimuth for a sensor is not correct.");
   for (int32_t i_sensor = 3; i_sensor < MAX_NUMBER_OF_SENSORS; i_sensor++)
   {
      DOUBLES_EQUAL_TEXT(-F360_2PI, max_det_az_per_sensor[i_sensor], F360_EPSILON, "The max azimuth for a sensor with no detections is not correct.");
   }
}


/** \purpose  
 * Verify that the correct detection indices are extracted for each sensor
 * \req NA
 */
TEST(Extract_Detection_Properties_Per_Sensor, Check_Correct_Det_Indices_Per_Sensor_When_There_Are_Detections)
{
   /** \precond
    * Nothing additional to Test_Setup()
    */
   
   /** \action
    * Call Extract_Detection_Properties_Per_Sensor()
    */
   Extract_Detection_Properties_Per_Sensor(detections, selected_dets_idx, selected_dets_num, min_det_az_per_sensor, max_det_az_per_sensor, det_idx_per_sensor, num_det_per_sensor);
   
   /** \result
    * - Verify selected_det_idx_sens3_min_az is equal to det_idx_per_sensor[2][0]
    * - Verify selected_det_idx_sens3_mid_az is equal to det_idx_per_sensor[2][1]
    * - Verify selected_det_idx_sens3_max_az is equal to det_idx_per_sensor[2][2]
    * - Verify selected_det_idx_sens2_min_az is equal to det_idx_per_sensor[1][0]
    * - Verify selected_det_idx_sens2_max_az is equal to det_idx_per_sensor[1][1]
    * - Verify selected_det_idx_sens1_min_max_az is equal to det_idx_per_sensor[0][0]
    */
   CHECK_EQUAL_TEXT(selected_det_idx_sens3_min_az, det_idx_per_sensor[2][0], "The detection idx from sensor 3 is not correct.");
   CHECK_EQUAL_TEXT(selected_det_idx_sens3_mid_az, det_idx_per_sensor[2][1], "The detection idx from sensor 3 is not correct.");
   CHECK_EQUAL_TEXT(selected_det_idx_sens3_max_az, det_idx_per_sensor[2][2], "The detection idx from sensor 3 is not correct.");

   CHECK_EQUAL_TEXT(selected_det_idx_sens2_min_az, det_idx_per_sensor[1][0], "The detection idx from sensor 2 is not correct.");
   CHECK_EQUAL_TEXT(selected_det_idx_sens2_max_az, det_idx_per_sensor[1][1], "The detection idx from sensor 2 is not correct.");

   CHECK_EQUAL_TEXT(selected_det_idx_sens1_min_max_az, det_idx_per_sensor[0][0], "The detection idx from sensor 1 is not correct.");
}

/** @}*/


/** \defgroup  Bin_Detections_By_Azimuth
 *  @{
 */
/** \brief
 * Test group for unittesting the function Bin_Detections_By_Azimuth()
 */
TEST_GROUP(Bin_Detections_By_Azimuth)
{  
   /** \setup
   * Declare all input parameters to Bin_Detections_By_Azimuth, these will be reused in all tests in this group.
   **/
   rspp_variant_A::RSPP_Detection_T detections[MAX_NUMBER_OF_DETECTIONS];
   uint32_t det_idx_for_sensor[MAX_DETS_IN_OBJ_TRK];
   uint32_t num_det_for_sensor;
   float32_t min_az_for_sensor;
   float32_t max_az_for_sensor;
   uint32_t bin_array_det_idx[MSMT_UPDATE_NUM_BINS_PER_SENSOR][MAX_DETS_IN_OBJ_TRK];
   uint32_t bin_array_num_det_in_bin[MSMT_UPDATE_NUM_BINS_PER_SENSOR];

   /** \setup
   * Define support variables that will improve test readability by
   * - Defining
   * - Defining det_idx1 = 123
   * - Defining det_idx2 = 13
   * - Defining det_idx3 = 12
   * - Defining det_idx4 = 11
   * - Defining det_idx5 = 1
   * - Defining det_idx6 = 2
   * - Defining det_idx7 = 3
   **/
   const uint32_t det_idx1 = 123U;
   const uint32_t det_idx2 = 13U;
   const uint32_t det_idx3 = 12U;
   const uint32_t det_idx4 = 11U;
   const uint32_t det_idx5 = 1U;
   const uint32_t det_idx6 = 2U;
   const uint32_t det_idx7 = 3U;
   
   /** \setup
    * Before each test, set
    * - max_az_for_sensor = 1.0
    * - min_az_for_sensor = -2.0
    * - detections[det_idx1].azimuth = 1.0F
    * - detections[det_idx2].azimuth = 0.9F
    * - detections[det_idx3].azimuth = 0.75F
    * - detections[det_idx4].azimuth = 0.0F
    * - detections[det_idx5].azimuth = -0.9F
    * - detections[det_idx6].azimuth = -2.0F
    * - detections[det_idx7].azimuth =-2.0F
    * - num_det_for_sensor = 7U
    * - det_idx_for_sensor[0] = det_idx1
    * - det_idx_for_sensor[1] = det_idx2
    * - det_idx_for_sensor[2] = det_idx3
    * - det_idx_for_sensor[3] = det_idx4
    * - det_idx_for_sensor[4] = det_idx5
    * - det_idx_for_sensor[5] = det_idx6
    * - det_idx_for_sensor[6] = det_idx7
    */
   TEST_SETUP()
   {
      max_az_for_sensor = 1.0F;
      min_az_for_sensor = -2.0F;

      detections[det_idx1].raw.azimuth = 1.0F; // bin 5
      detections[det_idx2].raw.azimuth = 0.9F; // bin 5
      detections[det_idx3].raw.azimuth = 0.75F;// bin 5
      detections[det_idx4].raw.azimuth = 0.0F; // bin 4
      detections[det_idx5].raw.azimuth = -0.9F;// bin 2
      detections[det_idx6].raw.azimuth = -2.0F;// bin 1
      detections[det_idx7].raw.azimuth =-2.0F;// bin 1

      num_det_for_sensor = 7U;
      det_idx_for_sensor[0] = det_idx1;
      det_idx_for_sensor[1] = det_idx2;
      det_idx_for_sensor[2] = det_idx3;
      det_idx_for_sensor[3] = det_idx4;
      det_idx_for_sensor[4] = det_idx5;
      det_idx_for_sensor[5] = det_idx6;
      det_idx_for_sensor[6] = det_idx7;
   }
};


/** \purpose
 * Verify that all entries in bin_array_num_det_in_bin are set to 0 when there are no detections to consider
 * \req NA
 */
TEST(Bin_Detections_By_Azimuth, Check_Number_Of_Dets_In_Bin_Is_Reset_To_Zero)
{
   /** \precond
    * - Set num_det_for_sensor to 0
    * - Fill all entries in bin_array_num_det_in_bin with some value > 0
    */
   num_det_for_sensor = 0;
   std::fill(cmn::begin(bin_array_num_det_in_bin), cmn::end(bin_array_num_det_in_bin), 8U);

   /** \action
    * Call Bin_Detections_By_Azimuth()
    */
   Bin_Detections_By_Azimuth(detections, det_idx_for_sensor, num_det_for_sensor, min_az_for_sensor, max_az_for_sensor, bin_array_det_idx, bin_array_num_det_in_bin);
   
   /** \result
    * Verify all entries in bin_array_num_det_in_bin are set to 0
    */
   for (uint32_t i_bin = 0; i_bin < MSMT_UPDATE_NUM_BINS_PER_SENSOR; i_bin++)
   {
      CHECK_EQUAL_TEXT(0U, bin_array_num_det_in_bin[i_bin], "bin_array_num_det_in_bin was not set to 0 when it was expected to.");  
   }
}

/** \purpose
 * Verify that all entries in bin_array_det_idx are unchanged when there are no detections to consider
 * \req NA
 */
TEST(Bin_Detections_By_Azimuth, Check_Det_Indices_Is_Unchanged_When_No_Dets)
{
   /** \precond
    * - Set num_det_for_sensor to 0
    * - Fill all entries in bin_array_det_idx with some value
    */
   num_det_for_sensor = 0;
   for (uint32_t i_bin = 0; i_bin < MSMT_UPDATE_NUM_BINS_PER_SENSOR; i_bin++)
   {
         std::fill(cmn::begin(bin_array_det_idx[i_bin]), cmn::end(bin_array_det_idx[i_bin]), 8U);
   }

   /** \action
    * Call Bin_Detections_By_Azimuth()
    */
   Bin_Detections_By_Azimuth(detections, det_idx_for_sensor, num_det_for_sensor, min_az_for_sensor, max_az_for_sensor, bin_array_det_idx, bin_array_num_det_in_bin);
   
   /** \result
    * Verify bin_array_det_idx is unchanged from test setup
    */
   for (uint32_t i_bin = 0; i_bin < MSMT_UPDATE_NUM_BINS_PER_SENSOR; i_bin++)
   {
      for (uint32_t i_det = 0; i_det < MAX_DETS_IN_OBJ_TRK; i_det++)
      {
         CHECK_EQUAL_TEXT(8U, bin_array_det_idx[i_bin][i_det], "bin_array_det_idx was changed in function call when it was expected not to.");  
      }
   }
}

/** \purpose
 * Verify that detections are binned as expected given their azimuth relative to 
 * max_az_for_sensor and min_az_for_sensor
 * \req NA
 */
TEST(Bin_Detections_By_Azimuth, Check_Detections_Are_Binned_As_Expected)
{
   /** \precond
    * Nothing additional to what is done in Test_Setup()
    */

   /** \action
    * Call Bin_Detections_By_Azimuth()
    */
   Bin_Detections_By_Azimuth(detections, det_idx_for_sensor, num_det_for_sensor, min_az_for_sensor, max_az_for_sensor, bin_array_det_idx, bin_array_num_det_in_bin);
   
   /** \result
    * - Verify det_idx1, det_idx2, det_idx3 is in bin 5
    * - Verify bin 5 has three detections
    * - Verify det_idx4 is in bin 4
    * - Verify bin 4 has 1 det
    * - Verify bin 3 has no dets
    * - Verify det_idx5 is in bin 2
    * - Verify bin 2 has 1 det
    * - Verify det_idx6, det_idx7 is in bin 1
    * - Verify bin 1 has 2 dets
    */
   CHECK_EQUAL_TEXT(det_idx1, bin_array_det_idx[4][0], "A detection index was saved to a bin it was not expect to.");
   CHECK_EQUAL_TEXT(det_idx2, bin_array_det_idx[4][1], "A detection index was saved to a bin it was not expect to.");
   CHECK_EQUAL_TEXT(det_idx3, bin_array_det_idx[4][2], "A detection index was saved to a bin it was not expect to.");
   CHECK_EQUAL_TEXT(3U, bin_array_num_det_in_bin[4], "A bin does not contain the expected number of detections.");

   CHECK_EQUAL_TEXT(det_idx4, bin_array_det_idx[3][0], "A detection index was saved to a bin it was not expect to.");
   CHECK_EQUAL_TEXT(1U, bin_array_num_det_in_bin[3], "A bin does not contain the expected number of detections.");

   CHECK_EQUAL_TEXT(0U, bin_array_num_det_in_bin[2], "A bin does not contain the expected number of detections.");

   CHECK_EQUAL_TEXT(det_idx5, bin_array_det_idx[1][0], "A detection index was saved to a bin it was not expect to.");
   CHECK_EQUAL_TEXT(1U, bin_array_num_det_in_bin[1], "A bin does not contain the expected number of detections.");

   CHECK_EQUAL_TEXT(det_idx6, bin_array_det_idx[0][0], "A detection index was saved to a bin it was not expect to.");
   CHECK_EQUAL_TEXT(det_idx7, bin_array_det_idx[0][1], "A detection index was saved to a bin it was not expect to.");
   CHECK_EQUAL_TEXT(2U, bin_array_num_det_in_bin[0], "A bin does not contain the expected number of detections.");
}
/** @}*/

/** \defgroup  Select_Median_Range_Rate_Detection_For_Each_Bin
 *  @{
 */
/** \brief
 * Test group for unittesting the function Select_Median_Range_Rate_Detection_For_Each_Bin()
 */
TEST_GROUP(Select_Median_Range_Rate_Detection_For_Each_Bin)
{  
   /** \setup
   * Declare all input parameters to Select_Median_Range_Rate_Detection_For_Each_Bin, these will be reused in all tests in this group.
   **/
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   uint32_t bin_array_det_idx[MSMT_UPDATE_NUM_BINS_PER_SENSOR][MAX_DETS_IN_OBJ_TRK];
   uint32_t bin_array_num_det_in_bin[MSMT_UPDATE_NUM_BINS_PER_SENSOR];
   uint32_t selected_dets_idx_final[MAX_DETS_IN_OBJ_TRK];
   uint32_t selected_dets_num_final;
   uint32_t selected_dets_score[MSMT_UPDATE_MAX_NUM_OF_MSMT]{};
   
   /** \setup
   * Define support variables that will improve test readability by
   * - Defining det_bin1_idx1 = 99
   * - Defining det_bin1_idx2 = 97
   * - Defining det_bin1_idx3 = 95
   * - Defining det_bin1_idx4 = 93
   * - Defining det_bin1_idx5 = 91
   * - Defining det_bin2_idx1 = 89
   * - Defining det_bin2_idx2 = 87
   * - Defining det_bin2_idx3 = 85
   * - Defining det_bin2_idx4 = 83
   * - Defining det_bin3_idx1 = 81
   * - Defining det_bin3_idx2 = 79
   * - Defining det_bin3_idx3 = 77
   * - Defining det_bin4_idx1 = 75
   * - Defining det_bin4_idx2 = 73
   * - Defining det_bin5_idx1 = 71
   **/
   const uint32_t det_bin1_idx1 = 99U;
   const uint32_t det_bin1_idx2 = 97U;
   const uint32_t det_bin1_idx3 = 95U;
   const uint32_t det_bin1_idx4 = 93U;
   const uint32_t det_bin1_idx5 = 91U;

   const uint32_t det_bin2_idx1 = 89U;
   const uint32_t det_bin2_idx2 = 87U;
   const uint32_t det_bin2_idx3 = 85U;
   const uint32_t det_bin2_idx4 = 83U;

   const uint32_t det_bin3_idx1 = 81U;
   const uint32_t det_bin3_idx2 = 79U;
   const uint32_t det_bin3_idx3 = 77U;

   const uint32_t det_bin4_idx1 = 75U;
   const uint32_t det_bin4_idx2 = 73U;

   const uint32_t det_bin5_idx1 = 71U;

   /** \setup
    * Before each test, set
    * - selected_dets_idx_final[0] = 2
    * - selected_dets_idx_final[1] = 1
    * - selected_dets_idx_final[2] = 3
    * - selected_dets_num_final = 3
    *
    * - detections[det_bin1_idx1].azimuth = 0.0
    * - detections[det_bin1_idx2].azimuth = 1.0
    * - detections[det_bin1_idx3].azimuth = 2.0
    * - detections[det_bin1_idx4].azimuth = 3.0
    * - detections[det_bin1_idx5].azimuth = 4.0
    * - bin_array_num_det_in_bin[0] = 5
    * - bin_array_det_idx[0][0] = det_bin1_idx1
    * - bin_array_det_idx[0][1] = det_bin1_idx2
    * - bin_array_det_idx[0][2] = det_bin1_idx3
    * - bin_array_det_idx[0][3] = det_bin1_idx4
    * - bin_array_det_idx[0][4] = det_bin1_idx5
    *
    * - detections[det_bin2_idx1].azimuth = -1.0
    * - detections[det_bin2_idx2].azimuth = 0.0
    * - detections[det_bin2_idx3].azimuth = 1.0
    * - detections[det_bin2_idx4].azimuth = 2.0
    * - bin_array_num_det_in_bin[1] = 4
    * - bin_array_det_idx[1][0] = det_bin2_idx1
    * - bin_array_det_idx[1][1] = det_bin2_idx2
    * - bin_array_det_idx[1][2] = det_bin2_idx3
    * - bin_array_det_idx[1][3] = det_bin2_idx4
    *
    * - detections[det_bin3_idx1].azimuth = -1.0
    * - detections[det_bin3_idx2].azimuth = 0.0
    * - detections[det_bin3_idx3].azimuth = 1.0
    * - bin_array_num_det_in_bin[2] = 3
    * - bin_array_det_idx[2][0] = det_bin3_idx1
    * - bin_array_det_idx[2][1] = det_bin3_idx2
    * - bin_array_det_idx[2][2] = det_bin3_idx3
    *
    * - detections[det_bin4_idx1].azimuth = -1.0
    * - detections[det_bin4_idx2].azimuth = 1.0
    * - bin_array_num_det_in_bin[3] = 2
    * - bin_array_det_idx[3][0] = det_bin4_idx1
    * - bin_array_det_idx[3][1] = det_bin4_idx2
    *
    * - detections[det_bin5_idx1].azimuth = 1.0
    * - bin_array_num_det_in_bin[4] = 1
    * - bin_array_det_idx[4][0] = det_bin5_idx1
    **/
   TEST_SETUP()
   {
      std::fill(cmn::begin(selected_dets_score), cmn::end(selected_dets_score), 1U);

      selected_dets_idx_final[0] = 2U;
      selected_dets_idx_final[1] = 1U;
      selected_dets_idx_final[2] = 3U;
      selected_dets_num_final = 3U;

      det_props[det_bin1_idx1].range_rate_compensated = 0.0F;
      det_props[det_bin1_idx2].range_rate_compensated = 1.0F;
      det_props[det_bin1_idx3].range_rate_compensated = 2.0F; // Median
      det_props[det_bin1_idx4].range_rate_compensated = 3.0F;
      det_props[det_bin1_idx5].range_rate_compensated = 4.0F;
      bin_array_num_det_in_bin[0] = 5U;
      bin_array_det_idx[0][0] = det_bin1_idx1;
      bin_array_det_idx[0][1] = det_bin1_idx2;
      bin_array_det_idx[0][2] = det_bin1_idx3;
      bin_array_det_idx[0][3] = det_bin1_idx4;
      bin_array_det_idx[0][4] = det_bin1_idx5;

      det_props[det_bin2_idx1].range_rate_compensated = -1.0F;
      det_props[det_bin2_idx2].range_rate_compensated = 0.0F; // Median
      det_props[det_bin2_idx3].range_rate_compensated = 1.0F;
      det_props[det_bin2_idx4].range_rate_compensated = 2.0F;
      bin_array_num_det_in_bin[1] = 4U;
      bin_array_det_idx[1][0] = det_bin2_idx1;
      bin_array_det_idx[1][1] = det_bin2_idx2;
      bin_array_det_idx[1][2] = det_bin2_idx3;
      bin_array_det_idx[1][3] = det_bin2_idx4;

      det_props[det_bin3_idx1].range_rate_compensated = -1.0F;
      det_props[det_bin3_idx2].range_rate_compensated = 0.0F; // Median
      det_props[det_bin3_idx3].range_rate_compensated = 1.0F;
      bin_array_num_det_in_bin[2] = 3U;
      bin_array_det_idx[2][0] = det_bin3_idx1;
      bin_array_det_idx[2][1] = det_bin3_idx2;
      bin_array_det_idx[2][2] = det_bin3_idx3;

      det_props[det_bin4_idx1].range_rate_compensated = -1.0F; // Median
      det_props[det_bin4_idx2].range_rate_compensated = 1.0F;
      bin_array_num_det_in_bin[3] = 2U;
      bin_array_det_idx[3][0] = det_bin4_idx1;
      bin_array_det_idx[3][1] = det_bin4_idx2;
      
      det_props[det_bin5_idx1].range_rate_compensated = 1.0F; // Median
      bin_array_num_det_in_bin[4] = 1U;
      bin_array_det_idx[4][0] = det_bin5_idx1;
   }
   
};

/** \purpose
 * Verify nothing happens to selected_dets_idx, and selected_dets_num if all binns are empty.
 * \req NA
 */
TEST(Select_Median_Range_Rate_Detection_For_Each_Bin, Check_Nothing_Happens_If_No_Dets)
{
   /** \precond
    * - Set all entries in bin_array_num_det_in_bin to 0
    * - Set all entris in selected_dets_idx_final to 123
    */
   std::fill(cmn::begin(bin_array_num_det_in_bin), cmn::end(bin_array_num_det_in_bin), 0U);
   std::fill(cmn::begin(selected_dets_idx_final), cmn::end(selected_dets_idx_final), 123U);

   /** \action
    * Call Select_Median_Range_Rate_Detection_For_Each_Bin()
    */
   Select_Median_Range_Rate_Detection_For_Each_Bin(det_props, bin_array_det_idx, bin_array_num_det_in_bin, selected_dets_idx_final, selected_dets_num_final, selected_dets_score);
   
   /** \result
    * - Verify selected_dets_idx_final and selected_dets_num_final was unchanged from test setup.
    */
   CHECK_EQUAL_TEXT(3U, selected_dets_num_final, "selected_dets_num_final was altered when it was not expected to.");
   for (uint32_t i_det = 0U; i_det < MAX_DETS_IN_OBJ_TRK; i_det++)
   {
      CHECK_EQUAL_TEXT(123U, selected_dets_idx_final[i_det], "selected_dets_idx_final was altered when it was not expected to.");
   }
}

/** \purpose
 * Verify the correct detection gets added to selected_dets_idx when there is a single 
 * detection in a single bin
 * \req NA
 */
TEST(Select_Median_Range_Rate_Detection_For_Each_Bin, Check_Det_Gets_Selected_Single_Det)
{
   /** \precond
    * Set bin 1, 2, 3, 4 to 0 in bin_array_num_det_in_bin
    */
   bin_array_num_det_in_bin[0] = 0U;
   bin_array_num_det_in_bin[1] = 0U;
   bin_array_num_det_in_bin[2] = 0U;
   bin_array_num_det_in_bin[3] = 0U; 

   /** \action
    * Call Select_Median_Range_Rate_Detection_For_Each_Bin()
    */
   Select_Median_Range_Rate_Detection_For_Each_Bin(det_props, bin_array_det_idx, bin_array_num_det_in_bin, selected_dets_idx_final, selected_dets_num_final, selected_dets_score);
   
   /** \result
    * - Verify selected_dets_idx_final[0] was unchanged from test setup
    * - Verify selected_dets_idx_final[1] was unchanged from test setup
    * - Verify selected_dets_idx_final[2] was unchanged from test setup
    * - Verify det_bin5_idx1 is equal to selected_dets_idx_final[3]
    * - Verify selected_dets_num_final was incremented by 1 (from 3 to 4)
    */
   CHECK_EQUAL_TEXT(4U, selected_dets_num_final, "selected_dets_num_final was not incremented as expected.");
   CHECK_EQUAL_TEXT(2U, selected_dets_idx_final[0], "selected_dets_idx_final was not changed as expected.");
   CHECK_EQUAL_TEXT(1U, selected_dets_idx_final[1], "selected_dets_idx_final was not changed as expected.");
   CHECK_EQUAL_TEXT(3U, selected_dets_idx_final[2], "selected_dets_idx_final was not changed as expected.");
   CHECK_EQUAL_TEXT(det_bin5_idx1, selected_dets_idx_final[3], "selected_dets_idx_final was not changed as expected.");
}

/** \purpose
 * Verify the correct detections gets added to selected_dets_idx when there are 2 
 * detections in a single bin
 * \req NA
 */
TEST(Select_Median_Range_Rate_Detection_For_Each_Bin, Check_Det_Gets_Selected_Two_Det)
{
   /** \precond
    * Set bin 1, 2, 3, 5 to 0 in bin_array_num_det_in_bin
    */
   bin_array_num_det_in_bin[0] = 0U;
   bin_array_num_det_in_bin[1] = 0U;
   bin_array_num_det_in_bin[2] = 0U;
   bin_array_num_det_in_bin[4] = 0U; 

   /** \action
    * Call Select_Median_Range_Rate_Detection_For_Each_Bin()
    */
   Select_Median_Range_Rate_Detection_For_Each_Bin(det_props, bin_array_det_idx, bin_array_num_det_in_bin, selected_dets_idx_final, selected_dets_num_final, selected_dets_score);
   
   /** \result
    * - Verify selected_dets_idx_final[0] was unchanged from test setup
    * - Verify selected_dets_idx_final[1] was unchanged from test setup
    * - Verify selected_dets_idx_final[2] was unchanged from test setup
    * - Verify det_bin4_idx1 is equal to selected_dets_idx_final[3]
    * - Verify selected_dets_num_final was incremented by 1 (from 3 to 4)
    */
   CHECK_EQUAL_TEXT(4U, selected_dets_num_final, "selected_dets_num_final was not incremented as expected.");
   CHECK_EQUAL_TEXT(2U, selected_dets_idx_final[0], "selected_dets_idx_final was not changed as expected.");
   CHECK_EQUAL_TEXT(1U, selected_dets_idx_final[1], "selected_dets_idx_final was not changed as expected.");
   CHECK_EQUAL_TEXT(3U, selected_dets_idx_final[2], "selected_dets_idx_final was not changed as expected.");
   CHECK_EQUAL_TEXT(det_bin4_idx1, selected_dets_idx_final[3], "selected_dets_idx_final was not changed as expected.");
}

/** \purpose
 * Verify the correct detections gets added to selected_dets_idx when there are 3 
 * detections in a single bin
 * \req NA
 */
TEST(Select_Median_Range_Rate_Detection_For_Each_Bin, Check_Det_Gets_Selected_Three_Det)
{
   /** \precond
    * Set bin 1, 2, 4, 5 to 0 in bin_array_num_det_in_bin
    */
   bin_array_num_det_in_bin[0] = 0U;
   bin_array_num_det_in_bin[1] = 0U;
   bin_array_num_det_in_bin[3] = 0U;
   bin_array_num_det_in_bin[4] = 0U; 

   /** \action
    * Call Select_Median_Range_Rate_Detection_For_Each_Bin()
    */
   Select_Median_Range_Rate_Detection_For_Each_Bin(det_props, bin_array_det_idx, bin_array_num_det_in_bin, selected_dets_idx_final, selected_dets_num_final, selected_dets_score);
   
   /** \result
    * - Verify selected_dets_idx_final[0] was unchanged
    * - Verify selected_dets_idx_final[1] was unchanged
    * - Verify selected_dets_idx_final[2] was unchanged
    * - Verify det_bin3_idx2 is equal to selected_dets_idx_final[3]
    * - Verify selected_dets_num_final was incremented by 1 (from 3 to 4)
    */
   CHECK_EQUAL_TEXT(4U, selected_dets_num_final, "selected_dets_num_final was not incremented as expected.");
   CHECK_EQUAL_TEXT(2U, selected_dets_idx_final[0], "selected_dets_idx_final was not changed as expected.");
   CHECK_EQUAL_TEXT(1U, selected_dets_idx_final[1], "selected_dets_idx_final was not changed as expected.");
   CHECK_EQUAL_TEXT(3U, selected_dets_idx_final[2], "selected_dets_idx_final was not changed as expected.");
   CHECK_EQUAL_TEXT(det_bin3_idx2, selected_dets_idx_final[3], "selected_dets_idx_final was not changed as expected.");
}

/** \purpose
 * Verify the correct detections gets added to selected_dets_idx when there are 4 
 * detections in a single bin
 * \req NA
 */
TEST(Select_Median_Range_Rate_Detection_For_Each_Bin, Check_Det_Gets_Selected_Four_Det)
{
   /** \precond
    * Set bin 1, 3, 4, 5 to 0 in bin_array_num_det_in_bin
    */
   bin_array_num_det_in_bin[0] = 0U;
   bin_array_num_det_in_bin[2] = 0U;
   bin_array_num_det_in_bin[3] = 0U;
   bin_array_num_det_in_bin[4] = 0U; 

   /** \action
    * Call Select_Median_Range_Rate_Detection_For_Each_Bin()
    */
   Select_Median_Range_Rate_Detection_For_Each_Bin(det_props, bin_array_det_idx, bin_array_num_det_in_bin, selected_dets_idx_final, selected_dets_num_final, selected_dets_score);
   
   /** \result
    * - Verify selected_dets_idx_final[0] was unchanged
    * - Verify selected_dets_idx_final[1] was unchanged
    * - Verify selected_dets_idx_final[2] was unchanged
    * - Verify det_bin2_idx2 is equal to selected_dets_idx_final[3]
    * - Verify selected_dets_num_final was incremented by 1 (from 3 to 4)
    */
   CHECK_EQUAL_TEXT(4U, selected_dets_num_final, "selected_dets_num_final was not incremented as expected.");
   CHECK_EQUAL_TEXT(2U, selected_dets_idx_final[0], "selected_dets_idx_final was not changed as expected.");
   CHECK_EQUAL_TEXT(1U, selected_dets_idx_final[1], "selected_dets_idx_final was not changed as expected.");
   CHECK_EQUAL_TEXT(3U, selected_dets_idx_final[2], "selected_dets_idx_final was not changed as expected.");
   CHECK_EQUAL_TEXT(det_bin2_idx2, selected_dets_idx_final[3], "selected_dets_idx_final was not changed as expected.");
}

/** \purpose
 * Verify the correct detections gets added to selected_dets_idx when there are 5 
 * detections in a single bin
 * \req NA
 */
TEST(Select_Median_Range_Rate_Detection_For_Each_Bin, Check_Det_Gets_Selected_Five_Det)
{
   /** \precond
    * Set bin 2, 3, 4, 5 to 0 in bin_array_num_det_in_bin
    */
   bin_array_num_det_in_bin[1] = 0U;
   bin_array_num_det_in_bin[2] = 0U;
   bin_array_num_det_in_bin[3] = 0U;
   bin_array_num_det_in_bin[4] = 0U; 

   /** \action
    * Call Select_Median_Range_Rate_Detection_For_Each_Bin()
    */
   Select_Median_Range_Rate_Detection_For_Each_Bin(det_props, bin_array_det_idx, bin_array_num_det_in_bin, selected_dets_idx_final, selected_dets_num_final, selected_dets_score);
   
   /** \result
    * - Verify selected_dets_idx_final[0] was unchanged
    * - Verify selected_dets_idx_final[1] was unchanged
    * - Verify selected_dets_idx_final[2] was unchanged
    * - Verify det_bin1_idx3 is equal to selected_dets_idx_final[3]
    * - Verify selected_dets_num_final was incremented by 1 (from 3 to 4)
    */
   CHECK_EQUAL_TEXT(4U, selected_dets_num_final, "selected_dets_num_final was not incremented as expected.");
   CHECK_EQUAL_TEXT(2U, selected_dets_idx_final[0], "selected_dets_idx_final was not changed as expected.");
   CHECK_EQUAL_TEXT(1U, selected_dets_idx_final[1], "selected_dets_idx_final was not changed as expected.");
   CHECK_EQUAL_TEXT(3U, selected_dets_idx_final[2], "selected_dets_idx_final was not changed as expected.");
   CHECK_EQUAL_TEXT(det_bin1_idx3, selected_dets_idx_final[3], "selected_dets_idx_final was not changed as expected.");
}

/** \purpose
 * Verify the correct detections gets added to selected_dets_idx when there are
 * multiple bins with different number of detections detections
 * \req NA
 */
TEST(Select_Median_Range_Rate_Detection_For_Each_Bin, Check_Det_Gets_Selected_Multiple_Dets)
{
   /** \precond
    * Nothing except for what is done in Test_Setup()
    */

   /** \action
    * Call Select_Median_Range_Rate_Detection_For_Each_Bin()
    */
   Select_Median_Range_Rate_Detection_For_Each_Bin(det_props, bin_array_det_idx, bin_array_num_det_in_bin, selected_dets_idx_final, selected_dets_num_final, selected_dets_score);
   
   /** \result
    * - Verify selected_dets_idx_final[0] was unchanged
    * - Verify selected_dets_idx_final[1] was unchanged
    * - Verify selected_dets_idx_final[2] was unchanged
    * - Verify det_bin1_idx3 is equal to selected_dets_idx_final[3]
    * - Verify det_bin2_idx2 is equal to selected_dets_idx_final[4]
    * - Verify det_bin3_idx2 is equal to selected_dets_idx_final[5]
    * - Verify det_bin4_idx1 is equal to selected_dets_idx_final[6]
    * - Verify det_bin5_idx1 is equal to selected_dets_idx_final[7]
    * - Verify selected_dets_num_final is equal to to 8
    */
   CHECK_EQUAL_TEXT(8U, selected_dets_num_final, "selected_dets_num_final was not incremented as expected.");
   CHECK_EQUAL_TEXT(2U, selected_dets_idx_final[0], "selected_dets_idx_final was not changed as expected.");
   CHECK_EQUAL_TEXT(1U, selected_dets_idx_final[1], "selected_dets_idx_final was not changed as expected.");
   CHECK_EQUAL_TEXT(3U, selected_dets_idx_final[2], "selected_dets_idx_final was not changed as expected.");
   CHECK_EQUAL_TEXT(det_bin1_idx3, selected_dets_idx_final[3], "selected_dets_idx_final was not changed as expected.");
   CHECK_EQUAL_TEXT(det_bin2_idx2, selected_dets_idx_final[4], "selected_dets_idx_final was not changed as expected.");
   CHECK_EQUAL_TEXT(det_bin3_idx2, selected_dets_idx_final[5], "selected_dets_idx_final was not changed as expected.");
   CHECK_EQUAL_TEXT(det_bin4_idx1, selected_dets_idx_final[6], "selected_dets_idx_final was not changed as expected.");
   CHECK_EQUAL_TEXT(det_bin5_idx1, selected_dets_idx_final[7], "selected_dets_idx_final was not changed as expected.");

   const uint32_t expected_score[MSMT_UPDATE_MAX_NUM_OF_MSMT] = {1,1,1,5,4,3,2,1};
   for (uint32_t i = 0; i < selected_dets_num_final; i++)
   {
      CHECK_EQUAL_TEXT(expected_score[i], selected_dets_score[i], "Bad number of detections in current bin");
   }
}
/** @}*/


/** \defgroup  Try_To_Decrease_Num_Selected_Dets
 *  @{
 */
/** \brief
 * Test group for unittesting the function Try_To_Decrease_Num_Selected_Dets()
 */
TEST_GROUP(Try_To_Decrease_Num_Selected_Dets)
{  
   /** \setup
   * Declare all input parameters to Try_To_Decrease_Num_Selected_Dets, these will be reused in all tests in this group.
   **/
   rspp_variant_A::RSPP_Detection_T detections[MAX_NUMBER_OF_DETECTIONS];
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   uint32_t k_min_num_selected_dets_per_sensor_for_binning;
   uint32_t selected_dets_idx[MAX_DETS_IN_OBJ_TRK];
   uint32_t selected_dets_num;
   uint32_t selected_dets_score[MSMT_UPDATE_MAX_NUM_OF_MSMT]{};

   /** \setup
    * Define support variables that will improve test readability by
    * - defining det_sens1_idx1 = 204U;
    * - defining det_sens1_idx2 = 226U;
    * - defining det_sens1_idx3 = 32U;
    * - defining det_sens1_idx4 = 249U;
    * - defining det_sens1_idx5 = 156U;
    * - defining det_sens2_idx1 = 24U;
    * - defining det_sens2_idx2 = 68U;
    * - defining det_sens2_idx3 = 133U;
    * - defining det_sens2_idx4 = 232U;
    * - defining det_sens2_idx5 = 233U;
    * - defining det_sens2_idx6 = 38U;
    * - defining det_sens3_idx1 = 242U;
    * - defining det_sens3_idx2 = 228U;
    * - defining det_sens3_idx3 = 116U;
    * - defining det_sens3_idx4 = 189U;
    * - defining det_sens3_idx5 = 34U;
    * - defining det_sens3_idx6 = 99U;
    * - defining det_sens3_idx7 = 214U;
    * - defining det_sens4_idx1 = 184U;
    **/

   const uint32_t det_sens1_idx1 = 204U;
   const uint32_t det_sens1_idx2 = 226U;
   const uint32_t det_sens1_idx3 = 32U;
   const uint32_t det_sens1_idx4 = 249U;
   const uint32_t det_sens1_idx5 = 156U;
   const uint32_t det_sens2_idx1 = 24U;
   const uint32_t det_sens2_idx2 = 68U;
   const uint32_t det_sens2_idx3 = 133U;
   const uint32_t det_sens2_idx4 = 232U;
   const uint32_t det_sens2_idx5 = 233U;
   const uint32_t det_sens2_idx6 = 38U;
   const uint32_t det_sens3_idx1 = 242U;
   const uint32_t det_sens3_idx2 = 228U;
   const uint32_t det_sens3_idx3 = 116U;
   const uint32_t det_sens3_idx4 = 189U;
   const uint32_t det_sens3_idx5 = 34U;
   const uint32_t det_sens3_idx6 = 99U;
   const uint32_t det_sens3_idx7 = 214U;
   const uint32_t det_sens4_idx1 = 184U;

   /** \setup
    * Before each test, set
    * k_min_num_selected_dets_per_sensor_for_binning = 5U
    * detections[det_sens1_idx1].sensor_id = 1
    * detections[det_sens1_idx2].sensor_id = 1
    * detections[det_sens1_idx3].sensor_id = 1
    * detections[det_sens1_idx4].sensor_id = 1
    * detections[det_sens1_idx5].sensor_id = 1
    * detections[det_sens2_idx1].sensor_id = 2
    * detections[det_sens2_idx2].sensor_id = 2
    * detections[det_sens2_idx3].sensor_id = 2
    * detections[det_sens2_idx4].sensor_id = 2
    * detections[det_sens2_idx5].sensor_id = 2
    * detections[det_sens2_idx6].sensor_id = 2
    * detections[det_sens3_idx1].sensor_id = 3
    * detections[det_sens3_idx2].sensor_id = 3
    * detections[det_sens3_idx3].sensor_id = 3
    * detections[det_sens3_idx4].sensor_id = 3
    * detections[det_sens3_idx5].sensor_id = 3
    * detections[det_sens3_idx6].sensor_id = 3
    * detections[det_sens3_idx7].sensor_id = 3
    * detections[det_sens4_idx1].sensor_id = 4
    * detections[det_sens1_idx1].azimuth = 1.0F
    * detections[det_sens1_idx2].azimuth = -0.97
    * detections[det_sens1_idx3].azimuth = -0.98
    * detections[det_sens1_idx4].azimuth = -0.99
    * detections[det_sens1_idx5].azimuth = -1.0
    * detections[det_sens2_idx1].azimuth = 0.0
    * detections[det_sens2_idx2].azimuth = 0.01
    * detections[det_sens2_idx3].azimuth = 0.02
    * detections[det_sens2_idx4].azimuth = 1.01
    * detections[det_sens2_idx5].azimuth = 1.125
    * detections[det_sens2_idx6].azimuth = 1.25
    * detections[det_sens3_idx1].azimuth = -1.0
    * detections[det_sens3_idx2].azimuth = -0.75
    * detections[det_sens3_idx3].azimuth = -0.5
    * detections[det_sens3_idx4].azimuth = -0.25
    * detections[det_sens3_idx5].azimuth = 0.0
    * detections[det_sens3_idx6].azimuth = 1.0
    * detections[det_sens3_idx7].azimuth = 2.0
    * detections[det_sens4_idx1].azimuth = 1.23456
    * detections[det_sens1_idx1].azimuth = 1.0F
    * detections[det_sens1_idx2].azimuth = -0.97
    * detections[det_sens1_idx3].azimuth = -0.98
    * detections[det_sens1_idx4].azimuth = -0.99
    * detections[det_sens1_idx5].range_rate_compensated = -1.0
    * detections[det_sens2_idx1].range_rate_compensated = 0.0
    * detections[det_sens2_idx2].range_rate_compensated = 0.01
    * detections[det_sens2_idx3].range_rate_compensated = 0.02
    * detections[det_sens2_idx4].range_rate_compensated = 1.01
    * detections[det_sens2_idx5].range_rate_compensated = 1.125
    * detections[det_sens2_idx6].range_rate_compensated = 1.25
    * detections[det_sens3_idx1].range_rate_compensated = -1.0
    * detections[det_sens3_idx2].range_rate_compensated = -0.75
    * detections[det_sens3_idx3].range_rate_compensated = -0.5
    * detections[det_sens3_idx4].range_rate_compensated = -0.25
    * detections[det_sens3_idx5].range_rate_compensated = 0.0
    * detections[det_sens3_idx6].range_rate_compensated = 1.0
    * detections[det_sens3_idx7].range_rate_compensated = 2.0
    * detections[det_sens4_idx1].range_rate_compensated = 1.23456
    */
   TEST_SETUP()
   {
      std::fill(cmn::begin(selected_dets_score), cmn::end(selected_dets_score), 1U);

      k_min_num_selected_dets_per_sensor_for_binning = 5U;

      detections[det_sens1_idx1].raw.sensor_id = 1;
      detections[det_sens1_idx2].raw.sensor_id = 1;
      detections[det_sens1_idx3].raw.sensor_id = 1;
      detections[det_sens1_idx4].raw.sensor_id = 1;
      detections[det_sens1_idx5].raw.sensor_id = 1;
      detections[det_sens2_idx1].raw.sensor_id = 2;
      detections[det_sens2_idx2].raw.sensor_id = 2;
      detections[det_sens2_idx3].raw.sensor_id = 2;
      detections[det_sens2_idx4].raw.sensor_id = 2;
      detections[det_sens2_idx5].raw.sensor_id = 2;
      detections[det_sens2_idx6].raw.sensor_id = 2;
      detections[det_sens3_idx1].raw.sensor_id = 3;
      detections[det_sens3_idx2].raw.sensor_id = 3;
      detections[det_sens3_idx3].raw.sensor_id = 3;
      detections[det_sens3_idx4].raw.sensor_id = 3;
      detections[det_sens3_idx5].raw.sensor_id = 3;
      detections[det_sens3_idx6].raw.sensor_id = 3;
      detections[det_sens3_idx7].raw.sensor_id = 3;
      detections[det_sens4_idx1].raw.sensor_id = 4;

      detections[det_sens1_idx1].raw.azimuth = 1.0F;   // last bin
      detections[det_sens1_idx2].raw.azimuth = -0.97F; // first bin
      detections[det_sens1_idx3].raw.azimuth = -0.98F; // first bin
      detections[det_sens1_idx4].raw.azimuth = -0.99F; // first bin, median az of bin
      detections[det_sens1_idx5].raw.azimuth = -1.0F;  // first bin
      det_props[det_sens1_idx1].range_rate_compensated = 1.0F;   // last bin
      det_props[det_sens1_idx2].range_rate_compensated = -0.97F; // first bin
      det_props[det_sens1_idx3].range_rate_compensated = -0.98F; // first bin
      det_props[det_sens1_idx4].range_rate_compensated = -0.99F; // first bin, median rr of bin
      det_props[det_sens1_idx5].range_rate_compensated = -1.0F;  // first bin

      detections[det_sens2_idx1].raw.azimuth = 0.0F;  // bin 1
      detections[det_sens2_idx2].raw.azimuth = 0.01F; // bin 1, median az of bin
      detections[det_sens2_idx3].raw.azimuth = 0.02F; // bin 1
      detections[det_sens2_idx4].raw.azimuth = 1.01F; // bin 5
      detections[det_sens2_idx5].raw.azimuth = 1.125F; // bin 5, median az of bin
      detections[det_sens2_idx6].raw.azimuth = 1.25F; // bin 5
      det_props[det_sens2_idx1].range_rate_compensated = 0.0F;  // bin 1
      det_props[det_sens2_idx2].range_rate_compensated = 0.01F; // bin 1, median rr of bin
      det_props[det_sens2_idx3].range_rate_compensated = 0.02F; // bin 1
      det_props[det_sens2_idx4].range_rate_compensated = 1.01F; // bin 5
      det_props[det_sens2_idx5].range_rate_compensated = 1.125F; // bin 5, median rr of bin
      det_props[det_sens2_idx6].range_rate_compensated = 1.25F; // bin 5

      detections[det_sens3_idx1].raw.azimuth = -1.0F; // bin 1
      detections[det_sens3_idx2].raw.azimuth = -0.75F;// bin 1, median az of bin
      detections[det_sens3_idx3].raw.azimuth = -0.5F; // bin 1
      detections[det_sens3_idx4].raw.azimuth = -0.25F;// bin 2, median az of bin
      detections[det_sens3_idx5].raw.azimuth = 0.0F;  // bin 2
      detections[det_sens3_idx6].raw.azimuth = 1.0F;  // bin 4
      detections[det_sens3_idx7].raw.azimuth = 2.0F;  // bin 5
      det_props[det_sens3_idx1].range_rate_compensated = -1.0F; // bin 1
      det_props[det_sens3_idx2].range_rate_compensated = -0.75F;// bin 1, median rr of bin
      det_props[det_sens3_idx3].range_rate_compensated = -0.5F; // bin 1
      det_props[det_sens3_idx4].range_rate_compensated = -0.25F;// bin 2, median rr of bin
      det_props[det_sens3_idx5].range_rate_compensated = 0.0F;  // bin 2
      det_props[det_sens3_idx6].range_rate_compensated = 1.0F;  // bin 4
      det_props[det_sens3_idx7].range_rate_compensated = 2.0F;  // bin 5

      detections[det_sens4_idx1].raw.azimuth = 1.23456F;
      det_props[det_sens4_idx1].range_rate_compensated = 1.23456F;
   }
   
};

/** \purpose
 * Verify selected_dets_num is not altered when it is set to 0
 * \req NA
 */
TEST(Try_To_Decrease_Num_Selected_Dets, Check_Nothing_Happens_If_No_Dets)
{
   /** \precond
    * - Set selected_dets_num to 0
    */
   selected_dets_num = 0U;

   /** \action
    * Call Try_To_Decrease_Num_Selected_Dets()
    */
   Try_To_Decrease_Num_Selected_Dets(detections, det_props, k_min_num_selected_dets_per_sensor_for_binning, selected_dets_idx, selected_dets_num, selected_dets_score);
   
   /** \result
    * - Verify selected_dets_num is 0
    */
   CHECK_EQUAL_TEXT(0U, selected_dets_num, "selected_dets_num was changed from 0 when it was not expected to change.");
}

/** \purpose
 * Verify selected_dets_num is unchanged when all detections come from 1 sensor and the 
 * number of detections from that sensor is equal to k_min_num_selected_dets_per_sensor_for_binning
 * \req NA
 */
TEST(Try_To_Decrease_Num_Selected_Dets, Check_Nothing_Happens_If_Too_Few_Dets_For_Sensor)
{
   /** \precond
    * - Set selected_dets_num to 5
    * - Set selected_dets_idx[0] to det_sens1_idx1
    * - Set selected_dets_idx[1] to det_sens1_idx2
    * - Set selected_dets_idx[2] to det_sens1_idx3
    * - Set selected_dets_idx[3] to det_sens1_idx4
    * - Set selected_dets_idx[4] to det_sens1_idx5
    */
   selected_dets_num = 5U;
   selected_dets_idx[0] = det_sens1_idx1;
   selected_dets_idx[1] = det_sens1_idx2;
   selected_dets_idx[2] = det_sens1_idx3;
   selected_dets_idx[3] = det_sens1_idx4;
   selected_dets_idx[4] = det_sens1_idx5;
   
   /** \action
    * Call Try_To_Decrease_Num_Selected_Dets()
    */
   Try_To_Decrease_Num_Selected_Dets(detections, det_props, k_min_num_selected_dets_per_sensor_for_binning, selected_dets_idx, selected_dets_num, selected_dets_score);
   
   /** \result
    * - Verify selected_dets_num is 5
    * - Verify selected_dets_idx[0] is equal to det_sens1_idx1
    * - Verify selected_dets_idx[1] is equal to det_sens1_idx2
    * - Verify selected_dets_idx[2] is equal to det_sens1_idx3
    * - Verify selected_dets_idx[3] is equal to det_sens1_idx4
    * - Verify selected_dets_idx[4] is equal to det_sens1_idx5
    * - Verify the number of selected detections "selected_dets_score" is correct in each bin
    */
   CHECK_EQUAL_TEXT(5U, selected_dets_num, "selected_dets_num was changed from 5 when it was not expected to change.");
   CHECK_EQUAL_TEXT(det_sens1_idx1, selected_dets_idx[0], "selected_dets_idx was not set to the expected detection idx, it was not expected to change.");
   CHECK_EQUAL_TEXT(det_sens1_idx2, selected_dets_idx[1], "selected_dets_idx was not set to the expected detection idx, it was not expected to change.");
   CHECK_EQUAL_TEXT(det_sens1_idx3, selected_dets_idx[2], "selected_dets_idx was not set to the expected detection idx, it was not expected to change.");
   CHECK_EQUAL_TEXT(det_sens1_idx4, selected_dets_idx[3], "selected_dets_idx was not set to the expected detection idx, it was not expected to change.");
   CHECK_EQUAL_TEXT(det_sens1_idx5, selected_dets_idx[4], "selected_dets_idx was not set to the expected detection idx, it was not expected to change.");

   for (uint32_t i = 0; i < selected_dets_num; i++)
   {
      CHECK_EQUAL_TEXT(1U, selected_dets_score[i], "Bad number of detections in current bin");
   }
}

/** \purpose
 * Verify selected_dets_num is unchanged when all detections come from 1 sensor and the 
 * number of detections from that sensor is more than k_min_num_selected_dets_per_sensor_for_binning
 * \req NA
 */
TEST(Try_To_Decrease_Num_Selected_Dets, Check_Binning_Is_Done_As_Expected_When_1_Sensor_Has_More_Dets_Than_Thr)
{
   /** \precond
    * - Set selected_dets_num to 5
    * - Verify selected_dets_idx[0] is equal to det_sens1_idx1
    * - Verify selected_dets_idx[1] is equal to det_sens1_idx2
    * - Verify selected_dets_idx[2] is equal to det_sens1_idx3
    * - Verify selected_dets_idx[3] is equal to det_sens1_idx4
    * - Verify selected_dets_idx[4] is equal to det_sens1_idx5
    * - Set k_min_num_selected_dets_per_sensor_for_binning to 4
    */
   selected_dets_num = 5U;
   selected_dets_idx[0] = det_sens1_idx1;
   selected_dets_idx[1] = det_sens1_idx2;
   selected_dets_idx[2] = det_sens1_idx3;
   selected_dets_idx[3] = det_sens1_idx4;
   selected_dets_idx[4] = det_sens1_idx5;

   k_min_num_selected_dets_per_sensor_for_binning = 4U;
   
   /** \action
    * Call Try_To_Decrease_Num_Selected_Dets()
    */
   Try_To_Decrease_Num_Selected_Dets(detections, det_props, k_min_num_selected_dets_per_sensor_for_binning, selected_dets_idx, selected_dets_num, selected_dets_score);
   
   /** \result
    * - Verify selected_dets_num is 2
    * - Verify selected_dets_idx[0] is equal to det_sens1_idx4
    * - Verify selected_dets_idx[1] is equal to det_sens1_idx1
    * - Verify the number of selected detections "selected_dets_score" is correct in each bin
    */
   CHECK_EQUAL_TEXT(2U, selected_dets_num, "selected_dets_num was not updated as expected when binning was intended to reduce the number of selected detections.");
   CHECK_EQUAL_TEXT(det_sens1_idx4, selected_dets_idx[0], "selected_dets_idx was not set to the expected detection idx when binning was intended to reduce the number of selected detections.");
   CHECK_EQUAL_TEXT(det_sens1_idx1, selected_dets_idx[1], "selected_dets_idx was not set to the expected detection idx when binning was intended to reduce the number of selected detections.");

   const uint32_t expected_score[MSMT_UPDATE_MAX_NUM_OF_MSMT] = {4,1};
   for (uint32_t i = 0; i < selected_dets_num; i++)
   {
      CHECK_EQUAL_TEXT(expected_score[i], selected_dets_score[i], "Bad number of detections in current bin");
   }
}

/** \purpose
 * Verify selected_dets_num is unchanged when all detections come from 2 sensors and the 
 * number of detections from that sensor is more than k_min_num_selected_dets_per_sensor_for_binning
 * \req NA
 */
TEST(Try_To_Decrease_Num_Selected_Dets, Check_Binning_Is_Done_As_Expected_When_1_Sensor_Has_More_Dets_Than_Thr_And_1_Sensor_Has_Less_Than_Thr)
{
   /** \precond
    * - Set selected_dets_num to 11
    * - Set selected_dets_idx[0] is equal to det_sens1_idx1
    * - Set selected_dets_idx[1] is equal to det_sens1_idx2
    * - Set selected_dets_idx[2] is equal to det_sens1_idx3
    * - Set selected_dets_idx[3] is equal to det_sens1_idx4
    * - Set selected_dets_idx[4] is equal to det_sens1_idx5
    * - Set selected_dets_idx[5] is equal to det_sens2_idx1
    * - Set selected_dets_idx[6] is equal to det_sens2_idx2
    * - Set selected_dets_idx[7] is equal to det_sens2_idx3
    * - Set selected_dets_idx[8] is equal to det_sens2_idx4
    * - Set selected_dets_idx[9] is equal to det_sens2_idx5
    * - Set selected_dets_idx[10] is equal to det_sens2_idx6
    */
   selected_dets_num = 11U;
   selected_dets_idx[0] = det_sens1_idx1;
   selected_dets_idx[1] = det_sens1_idx2;
   selected_dets_idx[2] = det_sens1_idx3;
   selected_dets_idx[3] = det_sens1_idx4;
   selected_dets_idx[4] = det_sens1_idx5;
   selected_dets_idx[5] = det_sens2_idx1;
   selected_dets_idx[6] = det_sens2_idx2;
   selected_dets_idx[7] = det_sens2_idx3;
   selected_dets_idx[8] = det_sens2_idx4;
   selected_dets_idx[9] = det_sens2_idx5;
   selected_dets_idx[10] = det_sens2_idx6;
   
   /** \action
    * Call Try_To_Decrease_Num_Selected_Dets()
    */
   Try_To_Decrease_Num_Selected_Dets(detections, det_props, k_min_num_selected_dets_per_sensor_for_binning, selected_dets_idx, selected_dets_num, selected_dets_score);
   
   /** \result
    * - Verify selected_dets_num is 7
    * - Verify selected_dets_idx[0] is equal to det_sens1_idx1
    * - Verify selected_dets_idx[1] is equal to det_sens1_idx2
    * - Verify selected_dets_idx[2] is equal to det_sens1_idx3
    * - Verify selected_dets_idx[3] is equal to det_sens1_idx4
    * - Verify selected_dets_idx[4] is equal to det_sens1_idx5
    * - Verify selected_dets_idx[5] is equal to det_sens2_idx2
    * - Verify selected_dets_idx[6] is equal to det_sens2_idx5
    * - Verify the number of selected detections "selected_dets_score" is correct in each bin
    */
   CHECK_EQUAL_TEXT(7U, selected_dets_num, "selected_dets_num was not updated as expected when binning was intended to reduce the number of selected detections.");
   
   CHECK_EQUAL_TEXT(det_sens1_idx1, selected_dets_idx[0], "selected_dets_idx was not set to the expected detection idx for a sensor that was not supposed to have be binned.");
   CHECK_EQUAL_TEXT(det_sens1_idx2, selected_dets_idx[1], "selected_dets_idx was not set to the expected detection idx for a sensor that was not supposed to have be binned.");
   CHECK_EQUAL_TEXT(det_sens1_idx3, selected_dets_idx[2], "selected_dets_idx was not set to the expected detection idx for a sensor that was not supposed to have be binned.");
   CHECK_EQUAL_TEXT(det_sens1_idx4, selected_dets_idx[3], "selected_dets_idx was not set to the expected detection idx for a sensor that was not supposed to have be binned.");
   CHECK_EQUAL_TEXT(det_sens1_idx5, selected_dets_idx[4], "selected_dets_idx was not set to the expected detection idx for a sensor that was not supposed to have be binned.");

   CHECK_EQUAL_TEXT(det_sens2_idx2, selected_dets_idx[5], "selected_dets_idx was not set to the expected detection idx for a sensor that was supposed to have been binned.");
   CHECK_EQUAL_TEXT(det_sens2_idx5, selected_dets_idx[6], "selected_dets_idx was not set to the expected detection idx for a sensor that was supposed to have been binned.");

   const uint32_t expected_score[MSMT_UPDATE_MAX_NUM_OF_MSMT] = {1,1,1,1,1,3,3};
   for (uint32_t i = 0; i < selected_dets_num; i++)
   {
      CHECK_EQUAL_TEXT(expected_score[i], selected_dets_score[i], "Bad number of detections in current bin");
   }
   
}

/** \purpose
 * Verify selected_dets_num is unchanged when all detections come from 4 sensors and the 
 * number of detections from that sensor is more than k_min_num_selected_dets_per_sensor_for_binning
 * \req NA
 */
TEST(Try_To_Decrease_Num_Selected_Dets, Check_Binning_Is_Done_As_Expected_When_2_Sensors_Have_More_Dets_Than_Thr_And_2_Sensor_Have_Less_Than_Thr)
{
   /** \precond
    * - Set selected_dets_num to 19
    * - Set selected_dets_idx[0] is equal to det_sens1_idx1
    * - Set selected_dets_idx[1] is equal to det_sens1_idx2
    * - Set selected_dets_idx[2] is equal to det_sens1_idx3
    * - Set selected_dets_idx[3] is equal to det_sens1_idx4
    * - Set selected_dets_idx[4] is equal to det_sens1_idx5
    * - Set selected_dets_idx[5] is equal to det_sens2_idx1
    * - Set selected_dets_idx[6] is equal to det_sens2_idx2
    * - Set selected_dets_idx[7] is equal to det_sens2_idx3
    * - Set selected_dets_idx[8] is equal to det_sens2_idx4
    * - Set selected_dets_idx[9] is equal to det_sens2_idx5
    * - Set selected_dets_idx[10] is equal to det_sens2_idx6
    */
   selected_dets_num = 19U;
   selected_dets_idx[0] = det_sens1_idx1;
   selected_dets_idx[1] = det_sens1_idx2;
   selected_dets_idx[2] = det_sens1_idx3;
   selected_dets_idx[3] = det_sens1_idx4;
   selected_dets_idx[4] = det_sens1_idx5;
   selected_dets_idx[5] = det_sens2_idx1;
   selected_dets_idx[6] = det_sens2_idx2;
   selected_dets_idx[7] = det_sens2_idx3;
   selected_dets_idx[8] = det_sens2_idx4;
   selected_dets_idx[9] = det_sens2_idx5;
   selected_dets_idx[10] = det_sens2_idx6;
   selected_dets_idx[11] = det_sens3_idx1;
   selected_dets_idx[12] = det_sens3_idx2;
   selected_dets_idx[13] = det_sens3_idx3;
   selected_dets_idx[14] = det_sens3_idx4;
   selected_dets_idx[15] = det_sens3_idx5;
   selected_dets_idx[16] = det_sens3_idx6;
   selected_dets_idx[17] = det_sens3_idx7;
   selected_dets_idx[18] = det_sens4_idx1;
   
   /** \action
    * Call Try_To_Decrease_Num_Selected_Dets()
    */
   Try_To_Decrease_Num_Selected_Dets(detections, det_props, k_min_num_selected_dets_per_sensor_for_binning, selected_dets_idx, selected_dets_num, selected_dets_score);
   
   /** \result
    * - Verify selected_dets_num is 12
    * - Verify selected_dets_idx[0] is equal to det_sens1_idx1
    * - Verify selected_dets_idx[1] is equal to det_sens1_idx2
    * - Verify selected_dets_idx[2] is equal to det_sens1_idx3
    * - Verify selected_dets_idx[3] is equal to det_sens1_idx4
    * - Verify selected_dets_idx[4] is equal to det_sens1_idx5
    * - Verify selected_dets_idx[5] is equal to det_sens2_idx2
    * - Verify selected_dets_idx[6] is equal to det_sens2_idx5
    * - Verify the number of selected detections "selected_dets_score" is correct in each bin
    */
   CHECK_EQUAL_TEXT(12U, selected_dets_num, "selected_dets_num was not updated as expected when binning was intended to reduce the number of selected detections.");
   
   CHECK_EQUAL_TEXT(det_sens1_idx1, selected_dets_idx[0], "selected_dets_idx was not set to the expected detection idx for a sensor that was not supposed to have be binned.");
   CHECK_EQUAL_TEXT(det_sens1_idx2, selected_dets_idx[1], "selected_dets_idx was not set to the expected detection idx for a sensor that was not supposed to have be binned.");
   CHECK_EQUAL_TEXT(det_sens1_idx3, selected_dets_idx[2], "selected_dets_idx was not set to the expected detection idx for a sensor that was not supposed to have be binned.");
   CHECK_EQUAL_TEXT(det_sens1_idx4, selected_dets_idx[3], "selected_dets_idx was not set to the expected detection idx for a sensor that was not supposed to have be binned.");
   CHECK_EQUAL_TEXT(det_sens1_idx5, selected_dets_idx[4], "selected_dets_idx was not set to the expected detection idx for a sensor that was not supposed to have be binned.");

   CHECK_EQUAL_TEXT(det_sens2_idx2, selected_dets_idx[5], "selected_dets_idx was not set to the expected detection idx for a sensor that was supposed to have been binned.");
   CHECK_EQUAL_TEXT(det_sens2_idx5, selected_dets_idx[6], "selected_dets_idx was not set to the expected detection idx for a sensor that was supposed to have been binned.");

   CHECK_EQUAL_TEXT(det_sens3_idx2, selected_dets_idx[7], "selected_dets_idx was not set to the expected detection idx for a sensor that was supposed to have been binned.");
   CHECK_EQUAL_TEXT(det_sens3_idx4, selected_dets_idx[8], "selected_dets_idx was not set to the expected detection idx for a sensor that was supposed to have been binned.");
   CHECK_EQUAL_TEXT(det_sens3_idx6, selected_dets_idx[9], "selected_dets_idx was not set to the expected detection idx for a sensor that was supposed to have been binned.");
   CHECK_EQUAL_TEXT(det_sens3_idx7, selected_dets_idx[10], "selected_dets_idx was not set to the expected detection idx for a sensor that was supposed to have been binned.");

   CHECK_EQUAL_TEXT(det_sens4_idx1, selected_dets_idx[11], "selected_dets_idx was not set to the expected detection idx for a sensor that was supposed to have been binned.");

   const uint32_t expected_score[MSMT_UPDATE_MAX_NUM_OF_MSMT] = {1,1,1,1,1,3,3,3,2,1,1,1};
   for (uint32_t i = 0; i < selected_dets_num; i++)
   {
      CHECK_EQUAL_TEXT(expected_score[i], selected_dets_score[i], "Bad number of detections in current bin");
   }
}

/** @}*/
