/** \file
   This file contains tests to verify that functions related to detections sorted in VCS-longitudinal order.
   It contains tests for both the actual sorting function and also support functions for efficient use of the sorted list.
*/

#include "f360_vcs_long_sorted_dets_support_functions.h"
#include "f360_internal_preprocessing.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

static const float32_t vcs_long_sorted_ref_points[MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS] = {
          -100.0F, -90.0F, -80.0F, -70.0F, -60.0F, -50.0F,
          -40.0F, -30.0F, -20.0F, -10.0F, 0.0F, 10.0F,
          20.0F, 30.0F, 40.0F, 50.0F, 60.0F, 70.0F,
          80.0F, 90.0F, 100.0F, 110.0F, 120.0F, 130.0F,
          140.0F, 150.0F, 160.0F, 170.0F, 180.0F, 190.0F, 200.0F};

/** \defgroup  f360_clear_dets_vcs_long_sorted_info
*  @{
*/

/** \brief
*  Tests to ensure that function Clear_Dets_Vcs_Long_Sorted_Info works as expected
*/
TEST_GROUP(f360_clear_dets_vcs_long_sorted_info)
{
   /** \setup
   * Declare raw_detect_list structure
   */
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list{};

   TEST_SETUP()
   {
      // Put some arbitrary data in raw detection list which we expect to be cleared
      raw_detect_list.vcslong_det_idx_min = 5;
      raw_detect_list.vcslong_det_idx_max = 9;

      for (uint32_t ref_point_idx = 0U; ref_point_idx < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS; ref_point_idx++)
      {
         raw_detect_list.vcslong_sorted_ref_det_idx[ref_point_idx] = 10;
      }
   }
};

/**
*\purpose  Purpose is to test that data in raw detection list related to longitdunal reference points are cleared
*          correctly.
*\req    NA
*/
TEST(f360_clear_dets_vcs_long_sorted_info, Test_Update_Dets_Vcs_Long_Ref_Sorted_Info_Valid_Det_Over_Zero)
{
   /** \action
   * Call function
   **/
   Clear_Dets_Vcs_Long_Sorted_Info(raw_detect_list);

   /** \result
   * Check that all relevant fields have been cleared
   **/
   CHECK_EQUAL_TEXT(F360_INVALID_ID, raw_detect_list.vcslong_det_idx_min, "Data in raw detection list not cleared correctly");
   CHECK_EQUAL_TEXT(F360_INVALID_ID, raw_detect_list.vcslong_det_idx_max, "Data in raw detection list not cleared correctly");

   for (uint32_t ref_point_idx = 0U; ref_point_idx < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS; ref_point_idx++)
   {
      CHECK_EQUAL_TEXT(F360_INVALID_ID, raw_detect_list.vcslong_sorted_ref_det_idx[ref_point_idx], "Data in raw detection list not cleared correctly");
   }
}

/** @}*/

/** \defgroup  f360_update_dets_vcs_long_ref_sorted_info
 *  @{
 */

/** \brief
 *  Tests for Update_Dets_Vcs_Long_Ref_Sorted_Info().
 *  Purpose is to test function Update_Dets_Vcs_Long_Ref_Sorted_Info() in all possible branches and extreme situations
 *  when detection is below/above all reference points in vcslong_sorted_ref_det_idx to an arbitrary valid index as this 
 *  is expected to be untouched.
 */
TEST_GROUP(f360_update_dets_vcs_long_ref_sorted_info)
{
   /** \setup
   * Declare and setup common data for all test cases
   * Set detection vcs long position greater than zero and give it arbitrary index
   * Also set first and last element in raw_detect_list.vcslong_sorted_ref_det_idx which is expected to be untouched
   */
   float32_t det_vcs_long = 0.1F;
   int32_t det_idx = 10;
   uint32_t calib_ref_start_idx = 0U;
   int32_t first_and_last_element = 5;
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list{};
   uint32_t exp_ref_idx;

   TEST_SETUP()
   {
      // Initialize raw_detect_list
      Clear_Dets_Vcs_Long_Sorted_Info(raw_detect_list);

      raw_detect_list.vcslong_sorted_ref_det_idx[0] = first_and_last_element;
      raw_detect_list.vcslong_sorted_ref_det_idx[MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS-1] = first_and_last_element;

      // Set reference points manually so that test don't break if calibrations are changed

      // Find index of first reference point greater than zero. We do this so that test don't break if calibrations are changed.
      for (uint32_t i = 0U; i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS; i++)
      {
         if (vcs_long_sorted_ref_points[i] > 0.0F)
         {
            exp_ref_idx = i + 1U; // First element in raw detection list is reserved for the smallest detection, thus offset by 1
            break;
         }
      }
   }
};

/**
*\purpose  Purpose is to test function Update_Dets_Vcs_Long_Ref_Sorted_Info() when all reference indexes are invalid
*\req    NA
*/
TEST(f360_update_dets_vcs_long_ref_sorted_info, Test_Update_Dets_Vcs_Long_Ref_Sorted_Info_Valid_Det_Over_Zero)
{
   /** \action
   * Call function
   **/
   Update_Dets_Vcs_Long_Ref_Sorted_Info(det_vcs_long, det_idx, calib_ref_start_idx, raw_detect_list);

   /** \result
   * Check expected data in raw detection list. 
   **/

   // First and last element should be left untouched
   CHECK_EQUAL_TEXT(first_and_last_element, raw_detect_list.vcslong_sorted_ref_det_idx[0], "Unexpected data in VCS-long sorted reference array");
   CHECK_EQUAL_TEXT(first_and_last_element, raw_detect_list.vcslong_sorted_ref_det_idx[MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS - 1], "Unexpected data in VCS-long sorted reference array");

   // det_vcs_long is above the 0.0 reference point in calibration, expect it to be one below reference element in raw detection list array
   CHECK_EQUAL_TEXT(exp_ref_idx - 1, calib_ref_start_idx, "Unexpected value of calibration index returned");

   // Reference index up until reference point zero should be detection index
   for (uint32_t i = 1U; i < exp_ref_idx; i++)
   {
      CHECK_EQUAL_TEXT(det_idx, raw_detect_list.vcslong_sorted_ref_det_idx[i], "Unexpected data in VCS-long sorted reference array");
   }

   // Remaining reference indexes should be invalid
   for (int32_t i = exp_ref_idx; i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS - 1; i++)
   {
      CHECK_EQUAL_TEXT(F360_INVALID_ID, raw_detect_list.vcslong_sorted_ref_det_idx[i], "Unexpected data in VCS-long sorted reference array");
   }
}

/**
*\purpose  Purpose is to test function Update_Dets_Vcs_Long_Ref_Sorted_Info() when some reference indexes already have been filled
*\req    NA
*/
TEST(f360_update_dets_vcs_long_ref_sorted_info, Test_Update_Dets_Vcs_Long_Ref_Sorted_Info_Valid_Det_Over_Zero_Prev_Ref_Point_Exist)
{
   /** \precond
   * Set the second and third reference indexes in raw detection list to an arbitrary index
   **/
   int32_t arbitrary_index_A = 15;
   int32_t arbitrary_index_B = 25;
   raw_detect_list.vcslong_sorted_ref_det_idx[1] = arbitrary_index_A;
   raw_detect_list.vcslong_sorted_ref_det_idx[2] = arbitrary_index_B;

   /** \action
   * Call function
   **/
   Update_Dets_Vcs_Long_Ref_Sorted_Info(det_vcs_long, det_idx, calib_ref_start_idx, raw_detect_list);

   /** \result
   * Check expected data in raw detection list.
   **/

   // First and last index should be left untouched
   CHECK_EQUAL_TEXT(first_and_last_element, raw_detect_list.vcslong_sorted_ref_det_idx[0], "Unexpected data in VCS-long sorted reference array");
   CHECK_EQUAL_TEXT(first_and_last_element, raw_detect_list.vcslong_sorted_ref_det_idx[MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS - 1], "Unexpected data in VCS-long sorted reference array");

   // The two first reference indexes should be left untouched
   CHECK_EQUAL_TEXT(arbitrary_index_A, raw_detect_list.vcslong_sorted_ref_det_idx[1], "Unexpected data in VCS-long sorted reference array");
   CHECK_EQUAL_TEXT(arbitrary_index_B, raw_detect_list.vcslong_sorted_ref_det_idx[2], "Unexpected data in VCS-long sorted reference array");

   // det_vcs_long is above the 0.0 reference point in calibration, expect it to be one below reference element in raw detection list array
   CHECK_EQUAL_TEXT(exp_ref_idx - 1, calib_ref_start_idx, "Unexpected value of calibration index returned");

   // Reference index up until reference point zero should be detection index
   for (uint32_t i = 3U; i < exp_ref_idx; i++)
   {
      CHECK_EQUAL_TEXT(det_idx, raw_detect_list.vcslong_sorted_ref_det_idx[i], "Unexpected data in VCS-long sorted reference array");
   }

   // Remaining reference indexes should be invalid
   for (uint32_t i = exp_ref_idx; i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS - 1; i++)
   {
      CHECK_EQUAL_TEXT(F360_INVALID_ID, raw_detect_list.vcslong_sorted_ref_det_idx[i], "Unexpected data in VCS-long sorted reference array");
   }
}

/**
*\purpose  Purpose is to test function Update_Dets_Vcs_Long_Ref_Sorted_Info() when detection is above all reference points.
*          We thus expect all reference indexes to have the index of the detection
*\req    NA
*/
TEST(f360_update_dets_vcs_long_ref_sorted_info, Test_Update_Dets_Vcs_Long_Ref_Sorted_Info_Valid_Det_Over_All_Ref_Points)
{
   /** \precond
   * Set detection long position to greater than highest reference point defined in calibration
   */
   det_vcs_long = INFTY;

   /** \action
   * Call function
   **/
   Update_Dets_Vcs_Long_Ref_Sorted_Info(det_vcs_long, det_idx, calib_ref_start_idx, raw_detect_list);

   /** \result
   * Check expected data in raw detection list.
   **/

   // First and last element should be left untouched
   CHECK_EQUAL_TEXT(first_and_last_element, raw_detect_list.vcslong_sorted_ref_det_idx[0], "Unexpected data in VCS-long sorted reference array");
   CHECK_EQUAL_TEXT(first_and_last_element, raw_detect_list.vcslong_sorted_ref_det_idx[MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS -  1], "Unexpected data in VCS-long sorted reference array");

   // det_vcs_long is above all reference point set in calibration, expect cal_idx to be maximum value
   CHECK_EQUAL_TEXT(MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS, calib_ref_start_idx, "Unexpected value of calibration index returned");

   // All reference indexes should be detection index except first and last one
   for (uint32_t i = 1U; i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS - 1; i++)
   {
      CHECK_EQUAL_TEXT(det_idx, raw_detect_list.vcslong_sorted_ref_det_idx[i], "Unexpected data in VCS-long sorted reference array");
   }
}

/**
*\purpose  Purpose is to test function Update_Dets_Vcs_Long_Ref_Sorted_Info() when detection is lower than all reference points.
*          We thus expect all reference indexes to be invalid
*\req    NA
*/
TEST(f360_update_dets_vcs_long_ref_sorted_info, Test_Update_Dets_Vcs_Long_Ref_Sorted_Info_Valid_Det_Below_All_Ref_Points)
{
   /** \precond
   * Set detection long position to lower than lowest reference point defined in calibration
   */
   det_vcs_long = -INFTY;

   /** \action
   * Call function
   **/
   Update_Dets_Vcs_Long_Ref_Sorted_Info(det_vcs_long, det_idx, calib_ref_start_idx, raw_detect_list);

   /** \result
   * Check expected data in raw detection list.
   **/
   // First and last element should be left untouched
   CHECK_EQUAL_TEXT(first_and_last_element, raw_detect_list.vcslong_sorted_ref_det_idx[0], "Unexpected data in VCS-long sorted reference array");
   CHECK_EQUAL_TEXT(first_and_last_element, raw_detect_list.vcslong_sorted_ref_det_idx[MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS-1], "Unexpected data in VCS-long sorted reference array");

   // det_vcs_long is never above any reference point set in calibration, expect cal_idx to be zero
   CHECK_EQUAL_TEXT(0U, calib_ref_start_idx, "Unexpected value of calibration index returned");

   // All reference indexes should be invalid except first and last element
   for (uint32_t i = 1U; i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS - 1; i++)
   {
      CHECK_EQUAL_TEXT(F360_INVALID_ID, raw_detect_list.vcslong_sorted_ref_det_idx[i], "Unexpected data in VCS-long sorted reference array");
   }
}

/** @}*/

/** \defgroup  f360_sort_detections_vcs_long
*  @{
*/

/** \brief
*  Tests for Sort_Detections_Vcs_Long
*/
TEST_GROUP(f360_sort_detections_vcs_long)
{
    /** \setup
    * Declare and setup common data for several tests
    */
    int32_t ndets;
    int32_t exp_sorted_idx[MAX_NUMBER_OF_DETECTIONS] = {};
    rspp_variant_A::RSPP_Detection_List_T raw_detect_list = {};

   TEST_SETUP()
   {

      raw_detect_list.number_of_valid_detections = 6;
      // Set detection data
      raw_detect_list.detections[0].processed.vcs_position_x = vcs_long_sorted_ref_points[4] + 1.0F;
      raw_detect_list.detections[1].processed.vcs_position_x = vcs_long_sorted_ref_points[2] + 1.0F;
      raw_detect_list.detections[2].processed.vcs_position_x = vcs_long_sorted_ref_points[8] + 1.0F;
      raw_detect_list.detections[3].processed.vcs_position_x = vcs_long_sorted_ref_points[10] + 1.0F;
      raw_detect_list.detections[4].processed.vcs_position_x = vcs_long_sorted_ref_points[15] + 1.0F;
      raw_detect_list.detections[5].processed.vcs_position_x = vcs_long_sorted_ref_points[12] + 1.0F;

      // Set expected sorted index
      exp_sorted_idx[0] = 1;
      exp_sorted_idx[1] = 0;
      exp_sorted_idx[2] = 2;
      exp_sorted_idx[3] = 3;
      exp_sorted_idx[4] = 5;
      exp_sorted_idx[5] = 4;
   }
};

/**
*\purpose  Purpose is to test function Sort_Detections_Vcs_Long() in the general case where there are more than 1 detections
*\req    NA
*/
TEST(f360_sort_detections_vcs_long, Test_Sort_Detections_Vcs_Long_General)
{
   /** \action
   * Call function
   **/
   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
   * Check expected data in raw detection list and detection props
   **/
   // Expected data in raw_detect_list structure
   CHECK_EQUAL_TEXT(exp_sorted_idx[0], raw_detect_list.vcslong_det_idx_min, "Unexpected data in raw detection list");
   CHECK_EQUAL_TEXT(exp_sorted_idx[5], raw_detect_list.vcslong_det_idx_max, "Unexpected data in raw detection list");

   // Check reference indexes towards expected values in raw_detect_list
   for (uint32_t i = 0U; i < 4U; i++)
   {
      CHECK_EQUAL_TEXT(exp_sorted_idx[0], raw_detect_list.vcslong_sorted_ref_det_idx[i], "Unexpected data in raw detection list");
   }
   for (uint32_t i = 4U; i < 6U; i++)
   {
      CHECK_EQUAL_TEXT(exp_sorted_idx[1], raw_detect_list.vcslong_sorted_ref_det_idx[i], "Unexpected data in raw detection list");
   }
   for (uint32_t i = 6U; i < 10U; i++)
   {
      CHECK_EQUAL_TEXT(exp_sorted_idx[2], raw_detect_list.vcslong_sorted_ref_det_idx[i], "Unexpected data in raw detection list");
   }
   for (uint32_t i = 10U; i < 12U; i++)
   {
      CHECK_EQUAL_TEXT(exp_sorted_idx[3], raw_detect_list.vcslong_sorted_ref_det_idx[i], "Unexpected data in raw detection list");
   }
   for (uint32_t i = 12U; i < 14U; i++)
   {
      CHECK_EQUAL_TEXT(exp_sorted_idx[4], raw_detect_list.vcslong_sorted_ref_det_idx[i], "Unexpected data in raw detection list");
   }
   for (uint32_t i = 14U; i < 17U; i++)
   {
      CHECK_EQUAL_TEXT(exp_sorted_idx[5], raw_detect_list.vcslong_sorted_ref_det_idx[i], "Unexpected data in raw detection list");
   }
   // The last invalid index is expected to be replaced with the detection index of the detection with largest VCS-long position
   CHECK_EQUAL_TEXT(exp_sorted_idx[5], raw_detect_list.vcslong_sorted_ref_det_idx[17], "Unexpected data in raw detection list");

   // Remaining indexes are expected to be invalid
   for (uint32_t i = 18U; i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS; i++)
   {
      CHECK_EQUAL_TEXT(F360_INVALID_ID, raw_detect_list.vcslong_sorted_ref_det_idx[i], "Unexpected data in raw detection list");
   }


   // Expected detection props data
   CHECK_EQUAL(exp_sorted_idx[1], raw_detect_list.detections[exp_sorted_idx[0]].processed.next_sorted_idx);
   for (int32_t i = 1; i < ndets - 1; i++)
   {
      CHECK_EQUAL(exp_sorted_idx[i - 1], raw_detect_list.detections[exp_sorted_idx[i]].processed.prev_sorted_idx);
      CHECK_EQUAL(exp_sorted_idx[i + 1], raw_detect_list.detections[exp_sorted_idx[i]].processed.next_sorted_idx);
   }
   CHECK_EQUAL(exp_sorted_idx[4], raw_detect_list.detections[exp_sorted_idx[5]].processed.prev_sorted_idx);
}

/**
*\purpose  Purpose is to test function Sort_Detections_Vcs_Long() in the case when there are only 1 detection
*\req    NA
*/
TEST(f360_sort_detections_vcs_long, Test_Sort_Detections_Vcs_Long_One_Detection)
{
   /** \precond
   * Set number of detections to 1
   */
   raw_detect_list.number_of_valid_detections = 1;
   /** \action
   * Call function
   **/
   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
   * Check expected data in raw detection list and detection props
   **/
   // Expected data in raw_detect_list structure
   CHECK_EQUAL_TEXT(0, raw_detect_list.vcslong_det_idx_min, "Unexpected data in raw detection list");
   CHECK_EQUAL_TEXT(0, raw_detect_list.vcslong_det_idx_max, "Unexpected data in raw detection list");

   CHECK_EQUAL_TEXT(0, raw_detect_list.vcslong_sorted_ref_det_idx[0], "Unexpected data in raw detection list");
   CHECK_EQUAL_TEXT(0, raw_detect_list.vcslong_sorted_ref_det_idx[1], "Unexpected data in raw detection list");
   for (int32_t i = 2; i < (MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS); i++)
   {
      CHECK_EQUAL_TEXT(F360_INVALID_ID, raw_detect_list.vcslong_sorted_ref_det_idx[i], "Unexpected data in raw detection list");
   }

   // Expected detection props data
   CHECK_EQUAL(F360_INVALID_ID, raw_detect_list.detections[0].processed.next_sorted_idx);
   CHECK_EQUAL(F360_INVALID_ID, raw_detect_list.detections[0].processed.prev_sorted_idx);
}

/**
*\purpose  Purpose is to test function Sort_Detections_Vcs_Long() in the case when there are no detections
*\req    NA
*/
TEST(f360_sort_detections_vcs_long, Test_Sort_Detections_Vcs_Long_No_Detection)
{
   /** \precond
   * Set number of detections to 0
   */
   raw_detect_list.number_of_valid_detections = 0;
   /** \action
   * Call function
   **/
   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
   * Check expected data in raw detection list.
   **/
   CHECK_EQUAL_TEXT(F360_INVALID_ID, raw_detect_list.vcslong_det_idx_min, "Unexpected data in raw detection list");
   CHECK_EQUAL_TEXT(F360_INVALID_ID, raw_detect_list.vcslong_det_idx_max, "Unexpected data in raw detection list");

   for (int32_t i = 0; i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS; i++)
   {
      CHECK_EQUAL_TEXT(F360_INVALID_ID, raw_detect_list.vcslong_sorted_ref_det_idx[i], "Unexpected data in raw detection list");
   }
}

/**
*\purpose  Purpose is to test function Sort_Detections_Vcs_Long() in the case when a detection is larger than the largest reference point
*\req    NA
*/
TEST(f360_sort_detections_vcs_long, Test_Sort_Detections_Vcs_Long_Detection_Above_Largest_Ref_Point)
{
   /** \precond
   * Set longitudinal position of largest vcs-long detection larger than the largest reference point
   */
   raw_detect_list.detections[exp_sorted_idx[5]].processed.vcs_position_x = vcs_long_sorted_ref_points[MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS - 1] + 1.0F;

   /** \action
   * Call function
   **/
   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
   * Check expected data in raw detection list and detection props structure
   **/
   CHECK_EQUAL_TEXT(exp_sorted_idx[0], raw_detect_list.vcslong_det_idx_min, "Unexpected data in raw detection list");
   CHECK_EQUAL_TEXT(exp_sorted_idx[5], raw_detect_list.vcslong_det_idx_max, "Unexpected data in raw detection list");

   // Check reference index array
   CHECK_EQUAL_TEXT(exp_sorted_idx[0], raw_detect_list.vcslong_sorted_ref_det_idx[0], "Unexpected data in raw detection list");
   CHECK_EQUAL_TEXT(exp_sorted_idx[5], raw_detect_list.vcslong_sorted_ref_det_idx[MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS - 1], "Unexpected data in raw detection list");

   for (uint32_t i = 0U; i < 4U; i++)
   {
      CHECK_EQUAL_TEXT(exp_sorted_idx[0], raw_detect_list.vcslong_sorted_ref_det_idx[i], "Unexpected data in raw detection list");
   }
   for (uint32_t i = 4U; i < 6U; i++)
   {
      CHECK_EQUAL_TEXT(exp_sorted_idx[1], raw_detect_list.vcslong_sorted_ref_det_idx[i], "Unexpected data in raw detection list");
   }
   for (uint32_t i = 6U; i < 10U; i++)
   {
      CHECK_EQUAL_TEXT(exp_sorted_idx[2], raw_detect_list.vcslong_sorted_ref_det_idx[i], "Unexpected data in raw detection list");
   }
   for (uint32_t i = 10U; i < 12U; i++)
   {
      CHECK_EQUAL_TEXT(exp_sorted_idx[3], raw_detect_list.vcslong_sorted_ref_det_idx[i], "Unexpected data in raw detection list");
   }
   for (uint32_t i = 12U; i < 14U; i++)
   {
      CHECK_EQUAL_TEXT(exp_sorted_idx[4], raw_detect_list.vcslong_sorted_ref_det_idx[i], "Unexpected data in raw detection list");
   }
   for (uint32_t i = 14U; i < (MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS - 1); i++)
   {
      CHECK_EQUAL_TEXT(exp_sorted_idx[5], raw_detect_list.vcslong_sorted_ref_det_idx[i], "Unexpected data in raw detection list");
   }

   // Expected detection props data
   CHECK_EQUAL(exp_sorted_idx[1], raw_detect_list.detections[exp_sorted_idx[0]].processed.next_sorted_idx);
   for (int32_t i = 1; i < ndets - 1; i++)
   {
      CHECK_EQUAL(exp_sorted_idx[i - 1], raw_detect_list.detections[exp_sorted_idx[i]].processed.prev_sorted_idx);
      CHECK_EQUAL(exp_sorted_idx[i + 1], raw_detect_list.detections[exp_sorted_idx[i]].processed.next_sorted_idx);
   }
   CHECK_EQUAL(exp_sorted_idx[4], raw_detect_list.detections[exp_sorted_idx[5]].processed.prev_sorted_idx);
}

/** @}*/


/** \defgroup  f360_get_first_relevant_long_sorted_det_idx
*  @{
*/

/** \brief
*  Tests for Get_First_Relevant_Long_Sorted_Det_Idx
*/
TEST_GROUP(f360_get_first_relevant_long_sorted_det_idx)
{
   /** \setup
   * Declare and setup common data for several tests
   */
   int32_t ndets;
   float32_t vcs_long_value;
   int32_t relevant_idx;
   int32_t exp_relevant_idx;
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list = {};

   TEST_SETUP()
   {

      ndets = 6;
      raw_detect_list.number_of_valid_detections = ndets;
      // Set detection data
      raw_detect_list.detections[0].processed.vcs_position_x = vcs_long_sorted_ref_points[4] + 1.0F;
      raw_detect_list.detections[1].processed.vcs_position_x = vcs_long_sorted_ref_points[2] + 1.0F;
      raw_detect_list.detections[2].processed.vcs_position_x = vcs_long_sorted_ref_points[8] + 1.0F;
      raw_detect_list.detections[3].processed.vcs_position_x = vcs_long_sorted_ref_points[10] + 1.0F;
      raw_detect_list.detections[4].processed.vcs_position_x = vcs_long_sorted_ref_points[15] + 1.0F;
      raw_detect_list.detections[5].processed.vcs_position_x = vcs_long_sorted_ref_points[12] + 1.0F;
      
      Sort_Detections_Vcs_Long(raw_detect_list);
   }
};

/**
*\purpose  Purpose is to test function Get_First_Relevant_Long_Sorted_Det_Idx() in the general case when
*          a detection exists above one of the reference points in raw detection list
*\req    NA
*/
TEST(f360_get_first_relevant_long_sorted_det_idx, Test_Get_First_Relevant_Long_Sorted_Det_Idx_General_Case)
{
   /** \precond
   * Set desired vcs_long_value
   */
   exp_relevant_idx = 3;
   vcs_long_value = raw_detect_list.detections[exp_relevant_idx].processed.vcs_position_x + 2.0F;

   /** \action
   * Call function
   **/
   relevant_idx = Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);

   /** \result
   * Check expected data in raw detection list.
   **/
   CHECK_EQUAL_TEXT(exp_relevant_idx, relevant_idx, "Unexpected detection index returned");
}

/**
*\purpose  Purpose is to test function Get_First_Relevant_Long_Sorted_Det_Idx() in the case when
*          there are no detections above desired value
*\req    NA
*/
TEST(f360_get_first_relevant_long_sorted_det_idx, Test_Get_First_Relevant_Long_Sorted_Det_Idx_No_Dets_Above_Desired_Value)
{
   /** \precond
   * Set desired vcs_long_value
   */
   exp_relevant_idx = F360_INVALID_ID;
   vcs_long_value = INFTY;

   /** \action
   * Call function
   **/
   relevant_idx = Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);

   /** \result
   * Check expected data in raw detection list.
   **/
   CHECK_EQUAL_TEXT(exp_relevant_idx, relevant_idx, "Unexpected detection index returned");
}

/**
*\purpose  Purpose is to test function Get_First_Relevant_Long_Sorted_Det_Idx() in the case when
*          the desired value is above the last reference point
*\req    NA
*/
TEST(f360_get_first_relevant_long_sorted_det_idx, Test_Get_First_Relevant_Long_Sorted_Det_Idx_No_Dets_Above_Desired_Value_Full_Loop)
{
   /** \precond
   * Set desired vcs_long_value and detection position to something very large to ensure the detection is
   * last in the reference index array in raw detection list. Since we modifiy detection in this test we also
   * need to sort the detections
   */
   exp_relevant_idx = F360_INVALID_ID;
   raw_detect_list.detections[5].processed.vcs_position_x = INFTY - 10.0F;
   vcs_long_value = INFTY - 5.0F;

   Sort_Detections_Vcs_Long(raw_detect_list);

   /** \action
   * Call function
   **/
   relevant_idx = Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);

   /** \result
   * Check expected data in raw detection list.
   **/
   CHECK_EQUAL_TEXT(exp_relevant_idx, relevant_idx, "Unexpected detection index returned");
}

/** @}*/

/** \defgroup  f360_is_more_relevant_dets_in_vcs_zone
*  @{
*/

/** \brief
*  Tests for Is_More_Relevant_Dets_In_Vcs_Zone
*/
TEST_GROUP(f360_is_more_relevant_dets_in_vcs_zone)
{
   /** \setup
   * Declare and setup common data for tests
   */
   int32_t det_idx;
   float32_t vcs_x_max = 10.0F;
   F360_Detection_Props_T det_p[MAX_NUMBER_OF_DETECTIONS] = {};
};

/**
*\purpose  Purpose is to test function Is_More_Relevant_Dets_In_Vcs_Zone() when detection is valid and
*          is above maximum vcs-long position
*\req    NA
*/
TEST(f360_is_more_relevant_dets_in_vcs_zone, Test_Is_More_Relevant_Dets_In_Vcs_Zone_Valid_And_Above_Limit)
{
   /** \precond
   * Set arbitrary valid detection index and detection position above max value
   */
   det_idx = 5;
   det_p[det_idx].vcs_position.x = vcs_x_max + 1.0F;

   /** \action
   * Call function
   **/
   bool f_is_dets_remaining = Is_More_Relevant_Dets_In_Vcs_Zone(det_idx, vcs_x_max, det_p);

   /** \result
   * Check expected data.
   **/
   CHECK_FALSE_TEXT(f_is_dets_remaining, "Unexpected boolean value returned");
}

/**
*\purpose  Purpose is to test function Is_More_Relevant_Dets_In_Vcs_Zone() when detection is valid and
*          is below maximum vcs-long position
*\req    NA
*/
TEST(f360_is_more_relevant_dets_in_vcs_zone, Test_Is_More_Relevant_Dets_In_Vcs_Zone_Valid_And_Below_Limit)
{
   /** \precond
   * Set arbitrary valid detection index and detection position below max value
   */
   det_idx = 5;
   det_p[det_idx].vcs_position.x = vcs_x_max - 1.0F;

   /** \action
   * Call function
   **/
   bool f_is_dets_remaining = Is_More_Relevant_Dets_In_Vcs_Zone(det_idx, vcs_x_max, det_p);

   /** \result
   * Check expected data
   **/
   CHECK_TRUE_TEXT(f_is_dets_remaining, "Unexpected boolean value returned");
}

/**
*\purpose  Purpose is to test function Is_More_Relevant_Dets_In_Vcs_Zone() when detection index is
*          not a valid index
*\req    NA
*/
TEST(f360_is_more_relevant_dets_in_vcs_zone, Test_Is_More_Relevant_Dets_In_Vcs_Zone_Not_Valid)
{
   /** \precond
   * Set detection index as invalid
   */
   det_idx = F360_INVALID_ID;

   /** \action
   * Call function
   **/
   bool f_is_dets_remaining = Is_More_Relevant_Dets_In_Vcs_Zone(det_idx, vcs_x_max, det_p);

   /** \result
   * Check expected data
   **/
   CHECK_FALSE_TEXT(f_is_dets_remaining, "Unexpected boolean value returned");
}

/** @}*/

/** \defgroup  f360_get_det_indexes_in_vcs_zone
*  @{
*/

/** \brief
*  Tests for Get_Det_Indexes_In_Vcs_Zone
*/

TEST_GROUP(f360_get_det_indexes_in_vcs_zone)
{
   /** \setup
   * Declare and setup common data for several tests
   */
   int32_t ndets;
   float32_t vcs_x_min = -5.0F;
   float32_t vcs_x_max = 5.0F;
   float32_t vcs_y_min = -5.0F;
   float32_t vcs_y_max = 5.0F;
   uint32_t num_relevant_dets;
   int32_t relevant_det_idx[MAX_NUMBER_OF_DETECTIONS] = {};
   uint32_t exp_num_relevant_dets = 2U;
   int32_t exp_relevant_det_idx[MAX_NUMBER_OF_DETECTIONS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list{};
   F360_Detection_Props_T det_p[MAX_NUMBER_OF_DETECTIONS]{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};

   TEST_SETUP()
   {

      raw_detect_list.number_of_valid_detections = 6;
      // Set detection data
      raw_detect_list.detections[0].processed.vcs_position_x = 0.0F;
      raw_detect_list.detections[0].processed.vcs_position_y = -10.0F;
      raw_detect_list.detections[1].processed.vcs_position_x = 2.0F;
      raw_detect_list.detections[1].processed.vcs_position_y = 0.0F;
      raw_detect_list.detections[2].processed.vcs_position_x = -2.0F;
      raw_detect_list.detections[2].processed.vcs_position_y = 10.0F;
      raw_detect_list.detections[3].processed.vcs_position_x = -20.0F;
      raw_detect_list.detections[3].processed.vcs_position_y = 0.0F;
      raw_detect_list.detections[4].processed.vcs_position_x = 20.0F;
      raw_detect_list.detections[4].processed.vcs_position_y = 0.0F;
      raw_detect_list.detections[5].processed.vcs_position_x = 1.0F;
      raw_detect_list.detections[5].processed.vcs_position_y = 2.0F;

      for (uint32_t det_idx = 0U; det_idx < raw_detect_list.number_of_valid_detections; det_idx++)
      {
         raw_detect_list.detections[det_idx].raw.sensor_id = 1U;
      }

      Sort_Detections_Vcs_Long(raw_detect_list);

      // Set expected data
      exp_num_relevant_dets = 2U;
      exp_relevant_det_idx[0] = 1;
      exp_relevant_det_idx[1] = 5;

      Copy_Detections_Info(raw_detect_list, sensors, det_p);
   }
};

/**
*\purpose  Purpose is to test function Is_More_Relevant_Dets_In_Vcs_Zone() in a general scenario
*          where two relevant detections are expected to be returned.
*\req    NA
*/
TEST(f360_get_det_indexes_in_vcs_zone, Test_Get_Det_Indexes_In_Vcs_Zone_General)
{
   /** \action
   * Call function
   **/
   Get_Det_Indexes_In_Vcs_Zone(vcs_x_min, vcs_x_max, vcs_y_min, vcs_y_max, raw_detect_list, det_p, relevant_det_idx, num_relevant_dets);

   /** \result
   * Check expected data.
   **/
   CHECK_EQUAL_TEXT(exp_num_relevant_dets, num_relevant_dets, "Unexpected number of relevant detections returned");
   CHECK_EQUAL_TEXT(exp_relevant_det_idx[0], exp_relevant_det_idx[0], "Unexpected index of relevant detections returned");
   CHECK_EQUAL_TEXT(exp_relevant_det_idx[1], exp_relevant_det_idx[1], "Unexpected index of relevant detections returned");
}

/**
*\purpose  Purpose is to test function Is_More_Relevant_Dets_In_Vcs_Zone() in a general scenario
*          where three relevant detections are expected to be returned. The last detection inside zone is
*          also the detection with the highest vcs_long value.
*\req    NA
*/
TEST(f360_get_det_indexes_in_vcs_zone, Test_Get_Det_Indexes_In_Vcs_Zone_Full_Loop)
{
   /** \precond
   * Increase VCS zone in longitudinal direction so that last detection is inside the zone.
   * Also adjust expected data.
   */
   vcs_x_max = 25.0F;
   exp_num_relevant_dets = 3U;
   exp_relevant_det_idx[2] = 4;

   /** \action
   * Call function
   **/
   Get_Det_Indexes_In_Vcs_Zone(vcs_x_min, vcs_x_max, vcs_y_min, vcs_y_max, raw_detect_list, det_p, relevant_det_idx, num_relevant_dets);

   /** \result
   * Check expected data.
   **/
   CHECK_EQUAL_TEXT(exp_num_relevant_dets, num_relevant_dets, "Unexpected number of relevant detections returned");
   CHECK_EQUAL_TEXT(exp_relevant_det_idx[0], exp_relevant_det_idx[0], "Unexpected index of relevant detections returned");
   CHECK_EQUAL_TEXT(exp_relevant_det_idx[1], exp_relevant_det_idx[1], "Unexpected index of relevant detections returned");
   CHECK_EQUAL_TEXT(exp_relevant_det_idx[2], exp_relevant_det_idx[2], "Unexpected index of relevant detections returned");
}

/** @}*/

/** \defgroup  Get_Det_Indexes_In_Vcs_Circular_Zone
*  @{
*/

/** \brief
*  Tests for Get_Det_Indexes_In_Vcs_Circular_Zone
*/

TEST_GROUP(Get_Det_Indexes_In_Vcs_Circular_Zone)
{
   /** \setup
   * Declare and setup common data for several tests
   */
   int32_t ndets;
   float32_t radius;
   uint32_t num_relevant_dets;
   uint32_t relevant_det_idx[MAX_NUMBER_OF_DETECTIONS] = {};
   uint32_t exp_num_relevant_dets = 2U;
   uint32_t exp_relevant_det_idx[MAX_NUMBER_OF_DETECTIONS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list{};
   F360_Detection_Props_T det_p[MAX_NUMBER_OF_DETECTIONS]{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   const Point obj_center = {0.0F, 0.0F};

   TEST_SETUP()
   {

      raw_detect_list.number_of_valid_detections = 2;
      // Set detection data
      raw_detect_list.detections[0].processed.vcs_position_x = 0.0F;
      raw_detect_list.detections[0].processed.vcs_position_y = -1.0F;
      raw_detect_list.detections[1].processed.vcs_position_x = -4.99F;
      raw_detect_list.detections[1].processed.vcs_position_y = 4.0F;

      for (uint32_t det_idx = 0U; det_idx < raw_detect_list.number_of_valid_detections; det_idx++)
      {
         raw_detect_list.detections[det_idx].raw.sensor_id = 1U;
      }

      Sort_Detections_Vcs_Long(raw_detect_list);

      // Set expected data
      exp_num_relevant_dets = 1U;
      exp_relevant_det_idx[0] = 0;
      radius = 5.0F;

      Copy_Detections_Info(raw_detect_list, sensors, det_p);
   }
};

/**
*\purpose  Purpose is to verify only det inside circle will set in relevant_det_idx
*\req    NA
*/
TEST(Get_Det_Indexes_In_Vcs_Circular_Zone, Test_Get_Det_Indexes_In_Vcs_Zone_Det_OutSide_Circle)
{
   /** \action
   * Call function
   **/
   Get_Det_Indexes_In_Vcs_Circular_Zone(obj_center, radius, raw_detect_list, det_p, relevant_det_idx, num_relevant_dets);

   /** \result
   * Check expected data.
   **/
   CHECK_EQUAL_TEXT(exp_num_relevant_dets, num_relevant_dets, "Unexpected number of relevant detections returned");
   CHECK_EQUAL_TEXT(exp_relevant_det_idx[0], relevant_det_idx[0], "Unexpected index of relevant detections returned");
}
