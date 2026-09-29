/** \file
   This file contains unit tests for testing requirements connected to f360_msmt_update_support_functions_common.cpp
*/

#include "f360_msmt_update_support_functions_common.h"
#include "f360_msmt_update_support_functions_ctca.h" 

#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;


/** \defgroup  f360_msmt_update_supporting_functions_Select_Dets_For_RR_Update_qualtest
*  @{
*/

/** \brief
*  Purpose is to very that function Select_Dets_For_RR_Update works as intended.
*  Initialize an object and assign some detections.
*  Assign detection properties to cover all possible branches inside the function.
**/
TEST_GROUP(f360_msmt_update_supporting_functions_Select_Dets_For_RR_Update_qualtest)
{
   /** \setup
   * Create an object, detections and return parameters along with expected data.
   **/
   rspp_variant_A::RSPP_Detection_T detections[MAX_NUMBER_OF_DETECTIONS];
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Object_Track_T object_track;
   uint32_t k_min_num_selected_dets_per_sensor_for_binning;
   uint32_t selected_dets_idx[MAX_DETS_IN_OBJ_TRK];
   uint32_t selected_dets_num;
   uint32_t selected_dets_score[MSMT_UPDATE_MAX_NUM_OF_MSMT]{};

   uint32_t exp_selected_dets_idx[MAX_DETS_IN_OBJ_TRK];
   uint32_t exp_selected_dets_num;

   /** \setup
   * Create detections to cover all possible branches
   */
   TEST_SETUP()
   {
      k_min_num_selected_dets_per_sensor_for_binning = 5;
      object_track.movable_prob = 0.0F;

      // Associate detections to object
      object_track.ndets = 5;
      object_track.detids[0] = 1;
      object_track.detids[1] = 2;
      object_track.detids[2] = 3;
      object_track.detids[3] = 4;
      object_track.detids[4] = 5;

      // Expected to be selected
      det_props[0].f_rr_inlier = true;

      // Expected to not be selected
      det_props[1].f_rr_inlier = false;

      // Expected to not be selected
      det_props[2].f_rr_inlier = true;

      // Expected to not be selected
      det_props[3].f_rr_inlier = false;

      // Expected to be selected
      det_props[4].f_rr_inlier = true;

      exp_selected_dets_num = 3U;
      exp_selected_dets_idx[0] = 0U;
      exp_selected_dets_idx[1] = 2U;
      exp_selected_dets_idx[2] = 4U;

      std::fill(cmn::begin(selected_dets_score), cmn::end(selected_dets_score), 1U);
   }
};


/*\purpose
* Verify that only detections marked as f_selected_by_track are selected by Select_Dets_For_RR_Update()
* \req CPR-3742
*/
TEST(f360_msmt_update_supporting_functions_Select_Dets_For_RR_Update_qualtest, Test_Select_Dets_For_RR_Update)
{
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
   CHECK_EQUAL_TEXT(exp_selected_dets_num, selected_dets_num, "Number of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(exp_selected_dets_idx[0], selected_dets_idx[0], "Index of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(exp_selected_dets_idx[1], selected_dets_idx[1], "Index of selected detections does not match expected data");
   CHECK_EQUAL_TEXT(exp_selected_dets_idx[2], selected_dets_idx[2], "Index of selected detections does not match expected data");
}
/** @}*/
