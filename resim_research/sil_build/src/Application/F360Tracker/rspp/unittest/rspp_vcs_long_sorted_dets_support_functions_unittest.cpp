/** \file
   This file contains tests to verify that functions related to detections sorted in VCS-longitudinal order.
   It contains tests for both the actual sorting function and also support functions for efficient use of the sorted list.
*/

#include "rspp_vcs_long_sorted_dets_support_functions.h"
#include "rspp_constants.h"
#include "rspp_math.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>

using namespace rspp_variant_A;

static constexpr int32_t RSPP_INVALID_ID = -1;

static const float32_t vcs_long_sorted_ref_points[MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS] = {
    -100.0F, -90.0F, -80.0F, -70.0F, -60.0F, -50.0F,
    -40.0F, -30.0F, -20.0F, -10.0F, 0.0F, 10.0F,
    20.0F, 30.0F, 40.0F, 50.0F, 60.0F, 70.0F,
    80.0F, 90.0F, 100.0F, 110.0F, 120.0F, 130.0F,
    140.0F, 150.0F, 160.0F, 170.0F, 180.0F, 190.0F, 200.0F};

/*===========================================================================*
 * Common Test Support Functions
 *===========================================================================*/

/**
 * \brief Helper function to setup a detection list with sorted detections
 *
 * \param raw_detect_list Detection list to populate
 * \param rspp_calibs Calibration structure for reference points
 * \param num_detections Number of detections to create
 * \param det_positions Array of VCS longitudinal positions for each detection
 */
static void Setup_Sorted_Detection_List(
    RSPP_Detection_List_T &raw_detect_list,
    const uint32_t num_detections,
    const float32_t det_positions[])
{
   raw_detect_list.number_of_valid_detections = num_detections;

   for (uint32_t i = 0U; i < num_detections; i++)
   {
      raw_detect_list.detections[i].processed.vcs_position_x = det_positions[i];
   }

   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);
}

/**
 * \brief Helper function to verify detection index is valid
 *
 * \param det_idx Detection index to verify
 * \return true if index is valid, false otherwise
 */
static bool Is_Valid_Detection_Index(const int32_t det_idx)
{
   return (det_idx != RSPP_INVALID_ID) && (det_idx >= 0) && (det_idx < MAX_NUMBER_OF_DETECTIONS);
}

/**
 * \brief Helper function to populate detection list with arbitrary data
 *
 * \param raw_detect_list Detection list to populate
 * \param min_idx Value to set for vcslong_det_idx_min
 * \param max_idx Value to set for vcslong_det_idx_max
 */
static void Populate_Detection_List_With_Data(
    RSPP_Detection_List_T &raw_detect_list,
    const int16_t min_idx,
    const int16_t max_idx)
{
   raw_detect_list.vcslong_det_idx_min = min_idx;
   raw_detect_list.vcslong_det_idx_max = max_idx;

   // Fill reference array with arbitrary valid indices
   for (uint32_t i = 0U; i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS; i++)
   {
      raw_detect_list.vcslong_sorted_ref_det_idx[i] = static_cast<int16_t>(i % MAX_NUMBER_OF_DETECTIONS);
   }
}

/**
 * \brief Helper function to verify all reference indices are cleared
 *
 * \param raw_detect_list Detection list to verify
 * \return true if all reference indices are RSPP_INVALID_ID, false otherwise
 */
static bool All_Reference_Indices_Cleared(const RSPP_Detection_List_T &raw_detect_list)
{
   for (uint32_t i = 0U; i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS; i++)
   {
      if (raw_detect_list.vcslong_sorted_ref_det_idx[i] != RSPP_INVALID_ID)
      {
         return false;
      }
   }
   return true;
}

/** \defgroup  test_RSPP_Sort_Detections_Vcs_Long
 *  @{
 */

/** \brief
 *  Tests for RSPP_Sort_Detections_Vcs_Long
 */
TEST_GROUP(test_RSPP_Sort_Detections_Vcs_Long)
{
   RSPP_Detection_List_T raw_detect_list;

   /** \setup
    * Initialize calibrations and clear detection list for each test.
    */
   TEST_SETUP()
   {
      // Clear detection list
      RSPP_Clear_Dets_Vcs_Long_Sorted_Info(raw_detect_list);
      raw_detect_list.number_of_valid_detections = 0U;

      // Initialize all detection fields
      for (uint32_t i = 0U; i < MAX_NUMBER_OF_DETECTIONS; i++)
      {
         raw_detect_list.detections[i].processed.vcs_position_x = 0.0F;
         raw_detect_list.detections[i].processed.prev_sorted_idx = static_cast<int16_t>(RSPP_INVALID_ID);
         raw_detect_list.detections[i].processed.next_sorted_idx = static_cast<int16_t>(RSPP_INVALID_ID);
      }
   }

   /** \teardown
    * No cleanup required for this test group.
    */
   TEST_TEARDOWN()
   {
   }
};

/*===========================================================================*
 * Test Cases - Basic Functionality
 *===========================================================================*/

/** \purpose
 * Verify function handles empty detection list correctly
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_001_No_Detections)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Detection list is already empty (number_of_valid_detections = 0)
    */

   /** \action
    * Call sort function with empty list
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * Min/max indices should remain invalid, no crashes should occur
    */
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_min,
                    "Min index should be RSPP_INVALID_ID for empty list");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_max,
                    "Max index should be RSPP_INVALID_ID for empty list");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_sorted_ref_det_idx[0],
                    "First reference should be RSPP_INVALID_ID");
}

/** \purpose
 * Verify function handles single detection correctly
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_002_Single_Detection)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create single detection at position 25.0
    */
   raw_detect_list.number_of_valid_detections = 1U;
   raw_detect_list.detections[0].processed.vcs_position_x = 25.0F;

   /** \action
    * Call sort function
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * Min and max should point to same detection, prev/next should be invalid
    */
   CHECK_EQUAL_TEXT(0, raw_detect_list.vcslong_det_idx_min,
                    "Min index should point to detection 0");
   CHECK_EQUAL_TEXT(0, raw_detect_list.vcslong_det_idx_max,
                    "Max index should point to detection 0");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.detections[0].processed.prev_sorted_idx,
                    "Single detection should have invalid prev index");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.detections[0].processed.next_sorted_idx,
                    "Single detection should have invalid next index");
   CHECK_EQUAL_TEXT(0, raw_detect_list.vcslong_sorted_ref_det_idx[0],
                    "Reference [0] should point to detection 0");
   CHECK_EQUAL_TEXT(0, raw_detect_list.vcslong_sorted_ref_det_idx[1],
                    "Reference [1] should point to detection 0");
}

/** \purpose
 * Verify function sorts two already-ordered detections correctly
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_003_Two_Detections_Ascending)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create two detections already in ascending order
    */
   raw_detect_list.number_of_valid_detections = 2U;
   raw_detect_list.detections[0].processed.vcs_position_x = 10.0F;
   raw_detect_list.detections[1].processed.vcs_position_x = 30.0F;

   /** \action
    * Call sort function
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * Detections should maintain order with correct linked list
    */
   CHECK_EQUAL_TEXT(0, raw_detect_list.vcslong_det_idx_min,
                    "Min should point to detection 0");
   CHECK_EQUAL_TEXT(1, raw_detect_list.vcslong_det_idx_max,
                    "Max should point to detection 1");

   // Check detection 0 (minimum)
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.detections[0].processed.prev_sorted_idx,
                    "First detection prev should be invalid");
   CHECK_EQUAL_TEXT(1, raw_detect_list.detections[0].processed.next_sorted_idx,
                    "First detection next should point to detection 1");

   // Check detection 1 (maximum)
   CHECK_EQUAL_TEXT(0, raw_detect_list.detections[1].processed.prev_sorted_idx,
                    "Second detection prev should point to detection 0");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.detections[1].processed.next_sorted_idx,
                    "Second detection next should be invalid");
}

/** \purpose
 * Verify function correctly reorders detections
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_004_Two_Detections_Descending)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create two detections in descending order (need to be sorted)
    */
   raw_detect_list.number_of_valid_detections = 2U;
   raw_detect_list.detections[0].processed.vcs_position_x = 50.0F;
   raw_detect_list.detections[1].processed.vcs_position_x = 20.0F;

   /** \action
    * Call sort function
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * Min should point to detection 1, max to detection 0
    */
   CHECK_EQUAL_TEXT(1, raw_detect_list.vcslong_det_idx_min,
                    "Min should point to detection 1 (20.0)");
   CHECK_EQUAL_TEXT(0, raw_detect_list.vcslong_det_idx_max,
                    "Max should point to detection 0 (50.0)");

   // Check detection 1 (minimum after sort)
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.detections[1].processed.prev_sorted_idx,
                    "Detection 1 prev should be invalid");
   CHECK_EQUAL_TEXT(0, raw_detect_list.detections[1].processed.next_sorted_idx,
                    "Detection 1 next should point to detection 0");

   // Check detection 0 (maximum after sort)
   CHECK_EQUAL_TEXT(1, raw_detect_list.detections[0].processed.prev_sorted_idx,
                    "Detection 0 prev should point to detection 1");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.detections[0].processed.next_sorted_idx,
                    "Detection 0 next should be invalid");
}

/*===========================================================================*
 * Test Cases - Multiple Detections
 *===========================================================================*/

/** \purpose
 * Verify function correctly sorts multiple detections
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_005_Three_Detections)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create three detections in random order
    */
   raw_detect_list.number_of_valid_detections = 3U;
   raw_detect_list.detections[0].processed.vcs_position_x = 30.0F;
   raw_detect_list.detections[1].processed.vcs_position_x = 10.0F;
   raw_detect_list.detections[2].processed.vcs_position_x = 20.0F;

   /** \action
    * Call sort function
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * Sorted order should be: det[1](10.0) -> det[2](20.0) -> det[0](30.0)
    */
   CHECK_EQUAL_TEXT(1, raw_detect_list.vcslong_det_idx_min,
                    "Min should point to detection 1");
   CHECK_EQUAL_TEXT(0, raw_detect_list.vcslong_det_idx_max,
                    "Max should point to detection 0");

   // Check detection 1 (minimum)
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.detections[1].processed.prev_sorted_idx,
                    "Detection 1 (min) prev should be invalid");
   CHECK_EQUAL_TEXT(2, raw_detect_list.detections[1].processed.next_sorted_idx,
                    "Detection 1 next should point to detection 2");

   // Check detection 2 (middle)
   CHECK_EQUAL_TEXT(1, raw_detect_list.detections[2].processed.prev_sorted_idx,
                    "Detection 2 prev should point to detection 1");
   CHECK_EQUAL_TEXT(0, raw_detect_list.detections[2].processed.next_sorted_idx,
                    "Detection 2 next should point to detection 0");

   // Check detection 0 (maximum)
   CHECK_EQUAL_TEXT(2, raw_detect_list.detections[0].processed.prev_sorted_idx,
                    "Detection 0 (max) prev should point to detection 2");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.detections[0].processed.next_sorted_idx,
                    "Detection 0 (max) next should be invalid");
}

/** \purpose
 * Verify function correctly populates reference array with multiple detections
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_006_Five_Detections)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create five detections at various positions
    */
   raw_detect_list.number_of_valid_detections = 5U;
   raw_detect_list.detections[0].processed.vcs_position_x = vcs_long_sorted_ref_points[5] + 1.0F;
   raw_detect_list.detections[1].processed.vcs_position_x = vcs_long_sorted_ref_points[2] + 1.0F;
   raw_detect_list.detections[2].processed.vcs_position_x = vcs_long_sorted_ref_points[10] + 1.0F;
   raw_detect_list.detections[3].processed.vcs_position_x = vcs_long_sorted_ref_points[0] + 1.0F;
   raw_detect_list.detections[4].processed.vcs_position_x = vcs_long_sorted_ref_points[8] + 1.0F;

   /** \action
    * Call sort function
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * Sorted order should be: det[3] -> det[1] -> det[0] -> det[4] -> det[2]
    * Reference array should be populated appropriately
    */
   CHECK_EQUAL_TEXT(3, raw_detect_list.vcslong_det_idx_min,
                    "Min should point to detection 3");
   CHECK_EQUAL_TEXT(2, raw_detect_list.vcslong_det_idx_max,
                    "Max should point to detection 2");

   // Verify linked list integrity
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.detections[3].processed.prev_sorted_idx,
                    "First detection prev should be invalid");
   CHECK_EQUAL_TEXT(1, raw_detect_list.detections[3].processed.next_sorted_idx,
                    "Detection 3 next should point to detection 1");

   CHECK_EQUAL_TEXT(3, raw_detect_list.detections[1].processed.prev_sorted_idx,
                    "Detection 1 prev should point to detection 3");
   CHECK_EQUAL_TEXT(0, raw_detect_list.detections[1].processed.next_sorted_idx,
                    "Detection 1 next should point to detection 0");

   CHECK_EQUAL_TEXT(4, raw_detect_list.detections[2].processed.prev_sorted_idx,
                    "Detection 2 prev should point to detection 4");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.detections[2].processed.next_sorted_idx,
                    "Last detection next should be invalid");

   // Verify reference array has minimum detection
   CHECK_EQUAL_TEXT(3, raw_detect_list.vcslong_sorted_ref_det_idx[0],
                    "Reference [0] should point to minimum detection");
}

/** \purpose
 * Verify function handles equal positions correctly
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_007_Equal_Positions)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create three detections at same position
    */
   raw_detect_list.number_of_valid_detections = 3U;
   raw_detect_list.detections[0].processed.vcs_position_x = 25.0F;
   raw_detect_list.detections[1].processed.vcs_position_x = 25.0F;
   raw_detect_list.detections[2].processed.vcs_position_x = 25.0F;

   /** \action
    * Call sort function
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * All detections should be linked in some order
    */
   CHECK_TRUE_TEXT(Is_Valid_Detection_Index(raw_detect_list.vcslong_det_idx_min),
                   "Min should be a valid detection");
   CHECK_TRUE_TEXT(Is_Valid_Detection_Index(raw_detect_list.vcslong_det_idx_max),
                   "Max should be a valid detection");

   // Verify linked list is complete (can traverse all detections)
   int16_t current_idx = raw_detect_list.vcslong_det_idx_min;
   uint32_t count = 0U;
   while (current_idx != RSPP_INVALID_ID && count < 10U)
   {
      count++;
      current_idx = raw_detect_list.detections[current_idx].processed.next_sorted_idx;
   }
   CHECK_EQUAL_TEXT(3U, count, "Should be able to traverse all 3 detections");
}

/*===========================================================================*
 * Test Cases - Linked List Integrity
 *===========================================================================*/

/** \purpose
 * Verify next_sorted_idx links form valid forward chain
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_008_Forward_Traversal)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create 4 detections in random order
    */
   raw_detect_list.number_of_valid_detections = 4U;
   raw_detect_list.detections[0].processed.vcs_position_x = 40.0F;
   raw_detect_list.detections[1].processed.vcs_position_x = 10.0F;
   raw_detect_list.detections[2].processed.vcs_position_x = 30.0F;
   raw_detect_list.detections[3].processed.vcs_position_x = 20.0F;

   /** \action
    * Call sort function and traverse forward
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * Should be able to traverse all detections in ascending order
    */
   int16_t current_idx = raw_detect_list.vcslong_det_idx_min;
   float32_t prev_position = -1000.0F;
   uint32_t count = 0U;

   while (current_idx != RSPP_INVALID_ID && count < 10U)
   {
      float32_t current_position = raw_detect_list.detections[current_idx].processed.vcs_position_x;
      CHECK_TRUE_TEXT(current_position >= prev_position,
                      "Forward traversal should be in ascending order");
      prev_position = current_position;
      current_idx = raw_detect_list.detections[current_idx].processed.next_sorted_idx;
      count++;
   }
   CHECK_EQUAL_TEXT(4U, count, "Forward traversal should visit all 4 detections");
}

/** \purpose
 * Verify prev_sorted_idx links form valid backward chain
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_009_Backward_Traversal)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create 4 detections in random order
    */
   raw_detect_list.number_of_valid_detections = 4U;
   raw_detect_list.detections[0].processed.vcs_position_x = 40.0F;
   raw_detect_list.detections[1].processed.vcs_position_x = 10.0F;
   raw_detect_list.detections[2].processed.vcs_position_x = 30.0F;
   raw_detect_list.detections[3].processed.vcs_position_x = 20.0F;

   /** \action
    * Call sort function and traverse backward
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * Should be able to traverse all detections in descending order
    */
   int16_t current_idx = raw_detect_list.vcslong_det_idx_max;
   float32_t prev_position = 1000.0F;
   uint32_t count = 0U;

   while (current_idx != RSPP_INVALID_ID && count < 10U)
   {
      float32_t current_position = raw_detect_list.detections[current_idx].processed.vcs_position_x;
      CHECK_TRUE_TEXT(current_position <= prev_position,
                      "Backward traversal should be in descending order");
      prev_position = current_position;
      current_idx = raw_detect_list.detections[current_idx].processed.prev_sorted_idx;
      count++;
   }
   CHECK_EQUAL_TEXT(4U, count, "Backward traversal should visit all 4 detections");
}

/** \purpose
 * Verify prev and next pointers are consistent
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_010_Bidirectional_Consistency)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create 5 detections
    */
   raw_detect_list.number_of_valid_detections = 5U;
   for (uint32_t i = 0U; i < 5U; i++)
   {
      raw_detect_list.detections[i].processed.vcs_position_x = static_cast<float32_t>(i) * 15.0F;
   }

   /** \action
    * Call sort function
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * For each detection, if next exists, next->prev should point back to current
    */
   int16_t current_idx = raw_detect_list.vcslong_det_idx_min;
   while (current_idx != RSPP_INVALID_ID)
   {
      int16_t next_idx = raw_detect_list.detections[current_idx].processed.next_sorted_idx;
      if (next_idx != RSPP_INVALID_ID)
      {
         int16_t next_prev = raw_detect_list.detections[next_idx].processed.prev_sorted_idx;
         CHECK_EQUAL_TEXT(current_idx, next_prev,
                          "Next detection's prev should point back to current");
      }
      current_idx = next_idx;
   }
}

/*===========================================================================*
 * Test Cases - Reference Array
 *===========================================================================*/

/** \purpose
 * Verify first reference always points to minimum detection
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_011_Reference_Minimum)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create multiple detections
    */
   raw_detect_list.number_of_valid_detections = 3U;
   raw_detect_list.detections[0].processed.vcs_position_x = 50.0F;
   raw_detect_list.detections[1].processed.vcs_position_x = 20.0F;
   raw_detect_list.detections[2].processed.vcs_position_x = 35.0F;

   /** \action
    * Call sort function
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * Reference [0] should always point to minimum detection
    */
   CHECK_EQUAL_TEXT(raw_detect_list.vcslong_det_idx_min,
                    raw_detect_list.vcslong_sorted_ref_det_idx[0],
                    "Reference [0] should point to minimum detection");
   CHECK_EQUAL_TEXT(1, raw_detect_list.vcslong_sorted_ref_det_idx[0],
                    "Minimum detection should be detection 1");
}

/** \purpose
 * Verify last filled reference points to maximum detection
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_012_Reference_Maximum)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create detections spanning several reference points
    */
   raw_detect_list.number_of_valid_detections = 3U;
   raw_detect_list.detections[0].processed.vcs_position_x = vcs_long_sorted_ref_points[5] + 1.0F;
   raw_detect_list.detections[1].processed.vcs_position_x = vcs_long_sorted_ref_points[1] + 1.0F;
   raw_detect_list.detections[2].processed.vcs_position_x = vcs_long_sorted_ref_points[3] + 1.0F;

   /** \action
    * Call sort function
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * Find last filled reference and verify it points to maximum
    */
   bool found_max_in_ref = false;
   for (uint32_t i = 1U; i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS; i++)
   {
      if (raw_detect_list.vcslong_sorted_ref_det_idx[i] == raw_detect_list.vcslong_det_idx_max)
      {
         found_max_in_ref = true;
         break;
      }
   }
   CHECK_TRUE_TEXT(found_max_in_ref,
                   "Maximum detection should appear in reference array");
}

/** \purpose
 * Verify reference array detection indices are non-decreasing
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_013_Reference_Monotonic)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create detections at various positions
    */
   raw_detect_list.number_of_valid_detections = 6U;
   for (uint32_t i = 0U; i < 6U; i++)
   {
      raw_detect_list.detections[i].processed.vcs_position_x =
          vcs_long_sorted_ref_points[i * 3] + 1.0F;
   }

   /** \action
    * Call sort function
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * Reference array positions should be non-decreasing
    */
   float32_t prev_position = -1000.0F;
   for (uint32_t i = 0U; i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS; i++)
   {
      int16_t ref_idx = raw_detect_list.vcslong_sorted_ref_det_idx[i];
      if (ref_idx != RSPP_INVALID_ID)
      {
         float32_t current_position = raw_detect_list.detections[ref_idx].processed.vcs_position_x;
         CHECK_TRUE_TEXT(current_position >= prev_position,
                         "Reference array positions should be non-decreasing");
         prev_position = current_position;
      }
   }
}

/*===========================================================================*
 * Test Cases - Negative and Zero Positions
 *===========================================================================*/

/** \purpose
 * Verify function handles negative VCS longitudinal positions
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_014_Negative_Positions)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create detections with negative positions
    */
   raw_detect_list.number_of_valid_detections = 3U;
   raw_detect_list.detections[0].processed.vcs_position_x = -10.0F;
   raw_detect_list.detections[1].processed.vcs_position_x = -50.0F;
   raw_detect_list.detections[2].processed.vcs_position_x = -30.0F;

   /** \action
    * Call sort function
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * Detections should be sorted: det[1](-50) -> det[2](-30) -> det[0](-10)
    */
   CHECK_EQUAL_TEXT(1, raw_detect_list.vcslong_det_idx_min,
                    "Min should be detection 1 at -50.0");
   CHECK_EQUAL_TEXT(0, raw_detect_list.vcslong_det_idx_max,
                    "Max should be detection 0 at -10.0");

   // Verify forward traversal gives ascending order
   int16_t current_idx = raw_detect_list.vcslong_det_idx_min;
   CHECK_EQUAL_TEXT(1, current_idx, "First should be detection 1");
   current_idx = raw_detect_list.detections[current_idx].processed.next_sorted_idx;
   CHECK_EQUAL_TEXT(2, current_idx, "Second should be detection 2");
   current_idx = raw_detect_list.detections[current_idx].processed.next_sorted_idx;
   CHECK_EQUAL_TEXT(0, current_idx, "Third should be detection 0");
}

/** \purpose
 * Verify function handles mix of positive and negative positions
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_015_Mixed_Signs)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create detections with mixed signs
    */
   raw_detect_list.number_of_valid_detections = 4U;
   raw_detect_list.detections[0].processed.vcs_position_x = 20.0F;
   raw_detect_list.detections[1].processed.vcs_position_x = -20.0F;
   raw_detect_list.detections[2].processed.vcs_position_x = -5.0F;
   raw_detect_list.detections[3].processed.vcs_position_x = 10.0F;

   /** \action
    * Call sort function
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * Should be sorted: det[1](-20) -> det[2](-5) -> det[3](10) -> det[0](20)
    */
   CHECK_EQUAL_TEXT(1, raw_detect_list.vcslong_det_idx_min,
                    "Min should be detection 1 at -20.0");
   CHECK_EQUAL_TEXT(0, raw_detect_list.vcslong_det_idx_max,
                    "Max should be detection 0 at 20.0");

   // Verify ascending order through traversal
   int16_t idx = raw_detect_list.vcslong_det_idx_min;
   float32_t prev_pos = -1000.0F;
   while (idx != RSPP_INVALID_ID)
   {
      float32_t curr_pos = raw_detect_list.detections[idx].processed.vcs_position_x;
      CHECK_TRUE_TEXT(curr_pos >= prev_pos, "Should be in ascending order");
      prev_pos = curr_pos;
      idx = raw_detect_list.detections[idx].processed.next_sorted_idx;
   }
}

/** \purpose
 * Verify function handles zero position correctly
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_016_Zero_Position)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create detections including zero
    */
   raw_detect_list.number_of_valid_detections = 3U;
   raw_detect_list.detections[0].processed.vcs_position_x = 10.0F;
   raw_detect_list.detections[1].processed.vcs_position_x = 0.0F;
   raw_detect_list.detections[2].processed.vcs_position_x = -10.0F;

   /** \action
    * Call sort function
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * Should be sorted: det[2](-10) -> det[1](0) -> det[0](10)
    */
   CHECK_EQUAL_TEXT(2, raw_detect_list.vcslong_det_idx_min,
                    "Min should be detection 2 at -10.0");

   // Verify middle detection is at zero
   int16_t middle_idx = raw_detect_list.detections[raw_detect_list.vcslong_det_idx_min].processed.next_sorted_idx;
   CHECK_EQUAL_TEXT(1, middle_idx, "Middle detection should be detection 1");
   CHECK_EQUAL_TEXT(0.0F, raw_detect_list.detections[middle_idx].processed.vcs_position_x,
                    "Middle detection should be at 0.0");
}

/*===========================================================================*
 * Test Cases - Large Detection Sets
 *===========================================================================*/

/** \purpose
 * Verify function handles larger detection sets correctly
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_017_Ten_Detections)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create 10 detections in reverse order
    */
   raw_detect_list.number_of_valid_detections = 10U;
   for (uint32_t i = 0U; i < 10U; i++)
   {
      raw_detect_list.detections[i].processed.vcs_position_x = static_cast<float32_t>(10U - i) * 10.0F;
   }

   /** \action
    * Call sort function
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * All detections should be sorted and traversable
    */
   CHECK_EQUAL_TEXT(9, raw_detect_list.vcslong_det_idx_min,
                    "Min should be detection 9 at 10.0");
   CHECK_EQUAL_TEXT(0, raw_detect_list.vcslong_det_idx_max,
                    "Max should be detection 0 at 100.0");

   // Verify all 10 can be traversed in order
   int16_t idx = raw_detect_list.vcslong_det_idx_min;
   uint32_t count = 0U;
   float32_t prev_pos = 0.0F;
   while (idx != RSPP_INVALID_ID && count < 20U)
   {
      float32_t curr_pos = raw_detect_list.detections[idx].processed.vcs_position_x;
      CHECK_TRUE_TEXT(curr_pos >= prev_pos, "Should be ascending");
      prev_pos = curr_pos;
      idx = raw_detect_list.detections[idx].processed.next_sorted_idx;
      count++;
   }
   CHECK_EQUAL_TEXT(10U, count, "Should traverse all 10 detections");
}

/** \purpose
 * Verify function handles large detection set with many reference points
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_018_Twenty_Detections)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create 20 detections at various reference points
    */
   raw_detect_list.number_of_valid_detections = 20U;
   for (uint32_t i = 0U; i < 20U; i++)
   {
      // Spread detections across reference points
      uint32_t ref_idx = (i * 2U) % MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS;
      raw_detect_list.detections[i].processed.vcs_position_x =
          vcs_long_sorted_ref_points[ref_idx] + static_cast<float32_t>(i) * 0.1F;
   }

   /** \action
    * Call sort function
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * Should be able to traverse all 20 detections
    */
   int16_t idx = raw_detect_list.vcslong_det_idx_min;
   uint32_t count = 0U;
   float32_t prev_pos = -1000.0F;

   while (idx != RSPP_INVALID_ID && count < 30U)
   {
      float32_t curr_pos = raw_detect_list.detections[idx].processed.vcs_position_x;
      CHECK_TRUE_TEXT(curr_pos >= prev_pos, "Should be in ascending order");
      prev_pos = curr_pos;
      idx = raw_detect_list.detections[idx].processed.next_sorted_idx;
      count++;
   }
   CHECK_EQUAL_TEXT(20U, count, "Should traverse all 20 detections");
}

/*===========================================================================*
 * Test Cases - Re-sorting
 *===========================================================================*/

/** \purpose
 * Verify function can be called multiple times idempotently
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_019_Resort_Same_Data)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create and sort detection list
    */
   raw_detect_list.number_of_valid_detections = 4U;
   raw_detect_list.detections[0].processed.vcs_position_x = 30.0F;
   raw_detect_list.detections[1].processed.vcs_position_x = 10.0F;
   raw_detect_list.detections[2].processed.vcs_position_x = 40.0F;
   raw_detect_list.detections[3].processed.vcs_position_x = 20.0F;

   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   int16_t first_min = raw_detect_list.vcslong_det_idx_min;
   int16_t first_max = raw_detect_list.vcslong_det_idx_max;

   /** \action
    * Call sort function again without changing data
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * Results should be identical
    */
   CHECK_EQUAL_TEXT(first_min, raw_detect_list.vcslong_det_idx_min,
                    "Min should remain same after resort");
   CHECK_EQUAL_TEXT(first_max, raw_detect_list.vcslong_det_idx_max,
                    "Max should remain same after resort");
}

/** \purpose
 * Verify function correctly re-sorts when positions change
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_020_Resort_Modified_Data)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create and sort detection list
    */
   raw_detect_list.number_of_valid_detections = 3U;
   raw_detect_list.detections[0].processed.vcs_position_x = 30.0F;
   raw_detect_list.detections[1].processed.vcs_position_x = 10.0F;
   raw_detect_list.detections[2].processed.vcs_position_x = 20.0F;

   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);
   CHECK_EQUAL_TEXT(1, raw_detect_list.vcslong_det_idx_min, "Initial min should be detection 1");

   /** \action
    * Modify positions and re-sort
    */
   raw_detect_list.detections[0].processed.vcs_position_x = 5.0F;  // Now smallest
   raw_detect_list.detections[1].processed.vcs_position_x = 25.0F; // Now middle
   raw_detect_list.detections[2].processed.vcs_position_x = 35.0F; // Now largest

   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * New sort order should be: det[0](5) -> det[1](25) -> det[2](35)
    */
   CHECK_EQUAL_TEXT(0, raw_detect_list.vcslong_det_idx_min,
                    "Min should now be detection 0 at 5.0");
   CHECK_EQUAL_TEXT(2, raw_detect_list.vcslong_det_idx_max,
                    "Max should now be detection 2 at 35.0");
}

/*===========================================================================*
 * Test Cases - Edge Cases
 *===========================================================================*/

/** \purpose
 * Verify function clears previous sorting information
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_021_Clears_Previous_Data)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Populate detection list with old sorting data
    */
   raw_detect_list.vcslong_det_idx_min = 5;
   raw_detect_list.vcslong_det_idx_max = 7;
   raw_detect_list.vcslong_sorted_ref_det_idx[0] = 3;
   raw_detect_list.vcslong_sorted_ref_det_idx[1] = 4;

   raw_detect_list.number_of_valid_detections = 2U;
   raw_detect_list.detections[0].processed.vcs_position_x = 20.0F;
   raw_detect_list.detections[1].processed.vcs_position_x = 10.0F;

   /** \action
    * Call sort function
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * Old data should be cleared and replaced with new sort results
    */
   CHECK_EQUAL_TEXT(1, raw_detect_list.vcslong_det_idx_min,
                    "Min should be updated to correct value");
   CHECK_EQUAL_TEXT(0, raw_detect_list.vcslong_det_idx_max,
                    "Max should be updated to correct value");
   CHECK_EQUAL_TEXT(1, raw_detect_list.vcslong_sorted_ref_det_idx[0],
                    "Reference should be updated");
}

/** \purpose
 * Verify function handles detections with small position differences
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_022_Small_Differences)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create detections with very small position differences
    */
   raw_detect_list.number_of_valid_detections = 3U;
   raw_detect_list.detections[0].processed.vcs_position_x = 10.02F;
   raw_detect_list.detections[1].processed.vcs_position_x = 10.00F;
   raw_detect_list.detections[2].processed.vcs_position_x = 10.01F;

   /** \action
    * Call sort function
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * Should be sorted: det[1](10.00) -> det[2](10.01) -> det[0](10.02)
    */
   CHECK_EQUAL_TEXT(1, raw_detect_list.vcslong_det_idx_min,
                    "Min should be detection 1 at 10.00");
   CHECK_EQUAL_TEXT(0, raw_detect_list.vcslong_det_idx_max,
                    "Max should be detection 0 at 10.02");

   // Verify order
   int16_t idx = raw_detect_list.vcslong_det_idx_min;
   CHECK_EQUAL_TEXT(1, idx, "First should be detection 1");
   idx = raw_detect_list.detections[idx].processed.next_sorted_idx;
   CHECK_EQUAL_TEXT(2, idx, "Second should be detection 2");
   idx = raw_detect_list.detections[idx].processed.next_sorted_idx;
   CHECK_EQUAL_TEXT(0, idx, "Third should be detection 0");
}

/** \purpose
 * Verify function handles large position values
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_023_Large_Values)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create detections with large position values
    */
   raw_detect_list.number_of_valid_detections = 3U;
   raw_detect_list.detections[0].processed.vcs_position_x = 500.0F;
   raw_detect_list.detections[1].processed.vcs_position_x = 300.0F;
   raw_detect_list.detections[2].processed.vcs_position_x = 800.0F;

   /** \action
    * Call sort function
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * Should be sorted: det[1](300) -> det[0](500) -> det[2](800)
    */
   CHECK_EQUAL_TEXT(1, raw_detect_list.vcslong_det_idx_min,
                    "Min should be detection 1 at 300.0");
   CHECK_EQUAL_TEXT(2, raw_detect_list.vcslong_det_idx_max,
                    "Max should be detection 2 at 800.0");

   // Verify can traverse
   int16_t idx = raw_detect_list.vcslong_det_idx_min;
   uint32_t count = 0U;
   while (idx != RSPP_INVALID_ID && count < 10U)
   {
      idx = raw_detect_list.detections[idx].processed.next_sorted_idx;
      count++;
   }
   CHECK_EQUAL_TEXT(3U, count, "Should traverse all 3 detections");
}

/** \purpose
 * Verify function handles full detection capacity correctly
 * \req
 * NA
 */
TEST(test_RSPP_Sort_Detections_Vcs_Long, RSPP_Sort_Detections_Vcs_Long_TC_024_Maximum_Detections)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create maximum number of detections in reverse order
    */
   raw_detect_list.number_of_valid_detections = MAX_NUMBER_OF_DETECTIONS;
   for (uint32_t i = 0U; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      // Reverse order: largest position first
      raw_detect_list.detections[i].processed.vcs_position_x =
          static_cast<float32_t>(MAX_NUMBER_OF_DETECTIONS - i) * 1.0F;
   }

   /** \action
    * Call sort function
    */
   RSPP_Sort_Detections_Vcs_Long(raw_detect_list);

   /** \result
    * All detections should be sorted and traversable
    */
   // Min should be last detection (index MAX-1) at position 1.0
   CHECK_EQUAL_TEXT(static_cast<int16_t>(MAX_NUMBER_OF_DETECTIONS - 1U),
                    raw_detect_list.vcslong_det_idx_min,
                    "Min should be last detection (smallest position)");
   // Max should be first detection (index 0) at largest position
   CHECK_EQUAL_TEXT(0, raw_detect_list.vcslong_det_idx_max,
                    "Max should be first detection (largest position)");

   // Verify all detections can be traversed in ascending order
   int16_t idx = raw_detect_list.vcslong_det_idx_min;
   uint32_t count = 0U;
   float32_t prev_pos = 0.0F;
   while (idx != RSPP_INVALID_ID && count < MAX_NUMBER_OF_DETECTIONS + 1U)
   {
      const float32_t curr_pos = raw_detect_list.detections[idx].processed.vcs_position_x;
      CHECK_TRUE_TEXT(curr_pos >= prev_pos, "Should be in ascending order");
      prev_pos = curr_pos;
      idx = raw_detect_list.detections[idx].processed.next_sorted_idx;
      count++;
   }
   CHECK_EQUAL_TEXT(MAX_NUMBER_OF_DETECTIONS, count,
                    "Should traverse all maximum detections");

   // Verify backward traversal works too
   idx = raw_detect_list.vcslong_det_idx_max;
   count = 0U;
   float32_t next_pos = static_cast<float32_t>(MAX_NUMBER_OF_DETECTIONS + 1U);
   while (idx != RSPP_INVALID_ID && count < MAX_NUMBER_OF_DETECTIONS + 1U)
   {
      const float32_t curr_pos = raw_detect_list.detections[idx].processed.vcs_position_x;
      CHECK_TRUE_TEXT(curr_pos <= next_pos, "Should be in descending order (backward)");
      next_pos = curr_pos;
      idx = raw_detect_list.detections[idx].processed.prev_sorted_idx;
      count++;
   }
   CHECK_EQUAL_TEXT(MAX_NUMBER_OF_DETECTIONS, count,
                    "Should traverse all maximum detections backward");
}

/** @}*/

/** \defgroup  test_RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info
 *  @{
 */

/** \brief
 *  Tests for RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info
 */
TEST_GROUP(test_RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info)
{
   RSPP_Detection_List_T raw_detect_list;
   uint32_t calib_ref_start_idx;

   /** \setup
    * Initialize calibrations and detection list reference indices.
    */
   TEST_SETUP()
   {
      // Clear detection list
      RSPP_Clear_Dets_Vcs_Long_Sorted_Info(raw_detect_list);
      raw_detect_list.number_of_valid_detections = 0U;

      // Initialize calibration reference start index
      calib_ref_start_idx = 0U;

      // Set first reference point to minimum detection (standard initialization)
      raw_detect_list.vcslong_det_idx_min = 0;
      raw_detect_list.vcslong_sorted_ref_det_idx[0] = 0;
   }

   /** \teardown
    * No cleanup required for this test group.
    */
   TEST_TEARDOWN()
   {
   }
};

/*===========================================================================*
 * Test Cases - Basic Functionality
 *===========================================================================*/

/** \purpose
 * Verify function updates reference array when detection is above reference point
 * \req
 * NA
 */
TEST(test_RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info, RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info_TC_001_Detection_Above_First_Reference)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Setup detection above first reference point
    */
   const float32_t det_vcs_long = vcs_long_sorted_ref_points[0] + 10.0F;
   const uint32_t det_idx = 1U;

   /** \action
    * Call update function
    */
   RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       det_vcs_long,
       det_idx,
       calib_ref_start_idx,
       raw_detect_list);

   /** \result
    * Reference index should be updated and start index incremented
    */
   CHECK_EQUAL_TEXT(static_cast<int16_t>(det_idx),
                    raw_detect_list.vcslong_sorted_ref_det_idx[1],
                    "Reference index [1] should be set to detection index");
   CHECK_EQUAL_TEXT(1U, calib_ref_start_idx,
                    "Calibration start index should be incremented to 1");
}

/** \purpose
 * Verify function doesn't update reference array when detection is below reference point
 * \req
 * NA
 */
TEST(test_RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info, RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info_TC_002_Detection_Below_Reference)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Setup detection below first reference point
    */
   const float32_t det_vcs_long = vcs_long_sorted_ref_points[0] - 10.0F;
   const uint32_t det_idx = 1U;

   /** \action
    * Call update function
    */
   RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       det_vcs_long,
       det_idx,
       calib_ref_start_idx,
       raw_detect_list);

   /** \result
    * Reference index should remain RSPP_INVALID_ID and start index unchanged
    */
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID,
                    raw_detect_list.vcslong_sorted_ref_det_idx[1],
                    "Reference index [1] should remain RSPP_INVALID_ID");
   CHECK_EQUAL_TEXT(0U, calib_ref_start_idx,
                    "Calibration start index should remain 0");
}

/** \purpose
 * Verify function behavior when detection is exactly at reference point
 * \req
 * NA
 */
TEST(test_RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info, RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info_TC_003_Detection_At_Reference)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Setup detection exactly at first reference point
    */
   const float32_t det_vcs_long = vcs_long_sorted_ref_points[0];
   const uint32_t det_idx = 1U;

   /** \action
    * Call update function
    */
   RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       det_vcs_long,
       det_idx,
       calib_ref_start_idx,
       raw_detect_list);

   /** \result
    * Detection at exact reference point is not > reference, should not update
    */
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID,
                    raw_detect_list.vcslong_sorted_ref_det_idx[1],
                    "Reference index [1] should remain RSPP_INVALID_ID (not greater than)");
   CHECK_EQUAL_TEXT(0U, calib_ref_start_idx,
                    "Calibration start index should remain 0");
}

/** \purpose
 * Verify function handles small differences above reference point
 * \req
 * NA
 */
TEST(test_RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info, RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info_TC_004_Detection_Just_Above_Reference)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Setup detection just above first reference point
    */
   const float32_t det_vcs_long = vcs_long_sorted_ref_points[0] + 0.01F;
   const uint32_t det_idx = 1U;

   /** \action
    * Call update function
    */
   RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       det_vcs_long,
       det_idx,
       calib_ref_start_idx,
       raw_detect_list);

   /** \result
    * Reference should be updated even for small difference
    */
   CHECK_EQUAL_TEXT(static_cast<int16_t>(det_idx),
                    raw_detect_list.vcslong_sorted_ref_det_idx[1],
                    "Reference index [1] should be set for small positive difference");
   CHECK_EQUAL_TEXT(1U, calib_ref_start_idx,
                    "Calibration start index should be incremented");
}

/*===========================================================================*
 * Test Cases - Multiple Reference Points
 *===========================================================================*/

/** \purpose
 * Verify function updates multiple reference indices for large detection position
 * \req
 * NA
 */
TEST(test_RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info, RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info_TC_005_Detection_Spanning_Multiple_References)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Setup detection well above multiple reference points
    */
   const float32_t det_vcs_long = vcs_long_sorted_ref_points[3] + 5.0F;
   const uint32_t det_idx = 1U;

   /** \action
    * Call update function starting from index 0
    */
   RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       det_vcs_long,
       det_idx,
       calib_ref_start_idx,
       raw_detect_list);

   /** \result
    * Multiple reference indices should be updated (indices 1, 2, 3, 4)
    */
   CHECK_EQUAL_TEXT(static_cast<int16_t>(det_idx),
                    raw_detect_list.vcslong_sorted_ref_det_idx[1],
                    "Reference index [1] should be set");
   CHECK_EQUAL_TEXT(static_cast<int16_t>(det_idx),
                    raw_detect_list.vcslong_sorted_ref_det_idx[2],
                    "Reference index [2] should be set");
   CHECK_EQUAL_TEXT(static_cast<int16_t>(det_idx),
                    raw_detect_list.vcslong_sorted_ref_det_idx[3],
                    "Reference index [3] should be set");
   CHECK_EQUAL_TEXT(static_cast<int16_t>(det_idx),
                    raw_detect_list.vcslong_sorted_ref_det_idx[4],
                    "Reference index [4] should be set");
   CHECK_EQUAL_TEXT(4U, calib_ref_start_idx,
                    "Calibration start index should be at last updated position");
}

/** \purpose
 * Verify function correctly handles sequential sorted detections
 * \req
 * NA
 */
TEST(test_RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info, RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info_TC_006_Sequential_Detections)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Process three detections in ascending order
    */

   /** \action
    * Process three detections in ascending order
    */

   // First detection: just above ref[0]
   const float32_t det1_vcs_long = vcs_long_sorted_ref_points[0] + 1.0F;
   RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       det1_vcs_long, 1U, calib_ref_start_idx, raw_detect_list);

   // Second detection: just above ref[2]
   const float32_t det2_vcs_long = vcs_long_sorted_ref_points[2] + 1.0F;
   RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       det2_vcs_long, 2U, calib_ref_start_idx, raw_detect_list);

   // Third detection: just above ref[5]
   const float32_t det3_vcs_long = vcs_long_sorted_ref_points[5] + 1.0F;
   RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       det3_vcs_long, 3U, calib_ref_start_idx, raw_detect_list);

   /** \result
    * Each detection should update appropriate reference indices
    * Function fills one reference at a time per call, updating calib_ref_start_idx
    * Det1 fills ref[1], det2 continues from ref[1] and fills ref[2] and ref[3], det3 continues and fills ref[4], ref[5], ref[6]
    */
   CHECK_EQUAL_TEXT(1, raw_detect_list.vcslong_sorted_ref_det_idx[1],
                    "Reference [1] should point to detection 1");
   CHECK_EQUAL_TEXT(2, raw_detect_list.vcslong_sorted_ref_det_idx[2],
                    "Reference [2] should point to detection 2");
   CHECK_EQUAL_TEXT(2, raw_detect_list.vcslong_sorted_ref_det_idx[3],
                    "Reference [3] should point to detection 2");
   CHECK_EQUAL_TEXT(3, raw_detect_list.vcslong_sorted_ref_det_idx[4],
                    "Reference [4] should point to detection 3");
   CHECK_EQUAL_TEXT(3, raw_detect_list.vcslong_sorted_ref_det_idx[5],
                    "Reference [5] should point to detection 3");
   CHECK_EQUAL_TEXT(3, raw_detect_list.vcslong_sorted_ref_det_idx[6],
                    "Reference [6] should point to detection 3");
   CHECK_EQUAL_TEXT(6U, calib_ref_start_idx,
                    "Final start index should be 6");
}

/** \purpose
 * Verify function correctly identifies which reference point to update
 * \req
 * NA
 */
TEST(test_RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info, RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info_TC_007_Detection_Between_References)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Setup detection between ref[5] and ref[6]
    */
   const float32_t det_vcs_long = (vcs_long_sorted_ref_points[5] +
                                   vcs_long_sorted_ref_points[6]) /
                                  2.0F;
   const uint32_t det_idx = 1U;

   /** \action
    * Call update function
    */
   RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       det_vcs_long,
       det_idx,
       calib_ref_start_idx,
       raw_detect_list);

   /** \result
    * Should update ref[6] (first above ref[5]) but not ref[7]
    */
   CHECK_EQUAL_TEXT(static_cast<int16_t>(det_idx),
                    raw_detect_list.vcslong_sorted_ref_det_idx[6],
                    "Reference [6] should be updated");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID,
                    raw_detect_list.vcslong_sorted_ref_det_idx[7],
                    "Reference [7] should not be updated");
   CHECK_EQUAL_TEXT(6U, calib_ref_start_idx,
                    "Start index should be 6");
}

/*===========================================================================*
 * Test Cases - Calibration Start Index Optimization
 *===========================================================================*/

/** \purpose
 * Verify function starts from provided start index and skips filled references
 * \req
 * NA
 */
TEST(test_RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info, RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info_TC_008_Start_Index_Optimization)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Pre-fill reference indices 1-3 with a previous detection
    */
   raw_detect_list.vcslong_sorted_ref_det_idx[1] = 5;
   raw_detect_list.vcslong_sorted_ref_det_idx[2] = 5;
   raw_detect_list.vcslong_sorted_ref_det_idx[3] = 5;

   // Start from index 3 (optimization)
   calib_ref_start_idx = 3U;

   // Detection above ref[5]
   const float32_t det_vcs_long = vcs_long_sorted_ref_points[5] + 1.0F;
   const uint32_t det_idx = 6U;

   /** \action
    * Call update function with start index 3
    */
   RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       det_vcs_long,
       det_idx,
       calib_ref_start_idx,
       raw_detect_list);

   /** \result
    * Should not modify already filled references, only update new ones
    */
   CHECK_EQUAL_TEXT(5, raw_detect_list.vcslong_sorted_ref_det_idx[1],
                    "Reference [1] should remain unchanged");
   CHECK_EQUAL_TEXT(5, raw_detect_list.vcslong_sorted_ref_det_idx[2],
                    "Reference [2] should remain unchanged");
   CHECK_EQUAL_TEXT(5, raw_detect_list.vcslong_sorted_ref_det_idx[3],
                    "Reference [3] should remain unchanged");
   CHECK_EQUAL_TEXT(static_cast<int16_t>(det_idx),
                    raw_detect_list.vcslong_sorted_ref_det_idx[4],
                    "Reference [4] should be updated");
   CHECK_EQUAL_TEXT(static_cast<int16_t>(det_idx),
                    raw_detect_list.vcslong_sorted_ref_det_idx[5],
                    "Reference [5] should be updated");
   CHECK_EQUAL_TEXT(static_cast<int16_t>(det_idx),
                    raw_detect_list.vcslong_sorted_ref_det_idx[6],
                    "Reference [6] should be updated");
}

/** \purpose
 * Verify optimization reduces iterations by using start index
 * \req
 * NA
 */
TEST(test_RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info, RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info_TC_009_Start_Index_Prevents_Redundant_Checks)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Fill references up to index 10
    */
   for (uint32_t i = 1U; i <= 10U; i++)
   {
      raw_detect_list.vcslong_sorted_ref_det_idx[i] = 1;
   }
   calib_ref_start_idx = 10U;

   // Detection above ref[12]
   const float32_t det_vcs_long = vcs_long_sorted_ref_points[12] + 1.0F;
   const uint32_t det_idx = 2U;

   /** \action
    * Call update function starting from index 10
    */
   RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       det_vcs_long,
       det_idx,
       calib_ref_start_idx,
       raw_detect_list);

   /** \result
    * Should only update from index 10 onwards
    */
   CHECK_EQUAL_TEXT(1, raw_detect_list.vcslong_sorted_ref_det_idx[10],
                    "Reference [10] should remain from previous detection");
   CHECK_EQUAL_TEXT(static_cast<int16_t>(det_idx),
                    raw_detect_list.vcslong_sorted_ref_det_idx[11],
                    "Reference [11] should be updated");
   CHECK_EQUAL_TEXT(static_cast<int16_t>(det_idx),
                    raw_detect_list.vcslong_sorted_ref_det_idx[12],
                    "Reference [12] should be updated");
   CHECK_EQUAL_TEXT(static_cast<int16_t>(det_idx),
                    raw_detect_list.vcslong_sorted_ref_det_idx[13],
                    "Reference [13] should be updated");
   CHECK_EQUAL_TEXT(13U, calib_ref_start_idx,
                    "Start index should advance to 13");
}

/*===========================================================================*
 * Test Cases - Boundary Conditions
 *===========================================================================*/

/** \purpose
 * Verify function handles detection at very low position
 * \req
 * NA
 */
TEST(test_RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info, RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info_TC_010_Minimum_Position)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Detection well below first reference point
    */
   const float32_t det_vcs_long = vcs_long_sorted_ref_points[0] - 100.0F;
   const uint32_t det_idx = 1U;

   /** \action
    * Call update function
    */
   RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       det_vcs_long,
       det_idx,
       calib_ref_start_idx,
       raw_detect_list);

   /** \result
    * No references should be updated, start index unchanged
    */
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID,
                    raw_detect_list.vcslong_sorted_ref_det_idx[1],
                    "No reference should be updated for detection below all references");
   CHECK_EQUAL_TEXT(0U, calib_ref_start_idx,
                    "Start index should remain 0");
}

/** \purpose
 * Verify function handles detection above many reference points
 * \req
 * NA
 */
TEST(test_RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info, RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info_TC_011_Maximum_Position)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Detection at a far position above multiple reference points
    */
   const float32_t det_vcs_long = vcs_long_sorted_ref_points[10] + 10.0F;
   const uint32_t det_idx = 1U;

   /** \action
    * Call update function
    */
   RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       det_vcs_long,
       det_idx,
       calib_ref_start_idx,
       raw_detect_list);

   /** \result
    * Function should update all references from 1 up to where detection is above reference point
    */
   // Verify references 1-11 are updated (detection is above ref[0] through ref[10])
   for (uint32_t i = 1U; i <= 11U; i++)
   {
      CHECK_EQUAL_TEXT(static_cast<int16_t>(det_idx),
                       raw_detect_list.vcslong_sorted_ref_det_idx[i],
                       "Reference indices should be updated for references below detection");
   }
   // Verify ref[12] is not updated (detection is not above ref[11])
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID,
                    raw_detect_list.vcslong_sorted_ref_det_idx[12],
                    "Reference index [12] should not be updated");
   CHECK_EQUAL_TEXT(11U, calib_ref_start_idx,
                    "Start index should be 11");
}

/** \purpose
 * Verify function handles edge case of starting at last reference
 * \req
 * NA
 */
TEST(test_RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info, RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info_TC_012_Start_Index_At_Limit)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set start index to last valid position
    */
   calib_ref_start_idx = MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS - 1U;
   const uint32_t last_ref = calib_ref_start_idx;
   const float32_t det_vcs_long = vcs_long_sorted_ref_points[last_ref] + 10.0F;
   const uint32_t det_idx = 1U;

   /** \action
    * Call update function
    */
   RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       det_vcs_long,
       det_idx,
       calib_ref_start_idx,
       raw_detect_list);

   /** \result
    * Only last reference position should be updated
    */
   CHECK_EQUAL_TEXT(static_cast<int16_t>(det_idx),
                    raw_detect_list.vcslong_sorted_ref_det_idx[last_ref + 1],
                    "Last reference should be updated");
   CHECK_EQUAL_TEXT(MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS, calib_ref_start_idx,
                    "Start index should be at maximum");
}

/** \purpose
 * Verify function handles maximum detection index
 * \req
 * NA
 */
TEST(test_RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info, RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info_TC_013_Maximum_Detection_Index)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Use maximum valid detection index
    */
   const uint32_t det_idx = MAX_NUMBER_OF_DETECTIONS - 1U;
   const float32_t det_vcs_long = vcs_long_sorted_ref_points[5] + 1.0F;

   /** \action
    * Call update function
    */
   RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       det_vcs_long,
       det_idx,
       calib_ref_start_idx,
       raw_detect_list);

   /** \result
    * Should correctly store maximum detection index
    */
   CHECK_EQUAL_TEXT(static_cast<int16_t>(det_idx),
                    raw_detect_list.vcslong_sorted_ref_det_idx[6],
                    "Should handle maximum detection index");
}

/*===========================================================================*
 * Test Cases - Early Exit Behavior
 *===========================================================================*/

/** \purpose
 * Verify function exits loop when detection is below next reference
 * \req
 * NA
 */
TEST(test_RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info, RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info_TC_014_Early_Exit_Below_Next_Reference)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Detection between ref[2] and ref[3]
    */
   const float32_t det_vcs_long = vcs_long_sorted_ref_points[2] + 0.5F;
   const uint32_t det_idx = 1U;

   // Verify ref[3] is greater than detection position
   CHECK_TRUE_TEXT(vcs_long_sorted_ref_points[3] > det_vcs_long,
                   "Setup: ref[3] should be greater than detection");

   /** \action
    * Call update function
    */
   RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       det_vcs_long,
       det_idx,
       calib_ref_start_idx,
       raw_detect_list);

   /** \result
    * Should update up to ref[3] and stop, not continue to ref[4]
    */
   CHECK_EQUAL_TEXT(static_cast<int16_t>(det_idx),
                    raw_detect_list.vcslong_sorted_ref_det_idx[3],
                    "Reference [3] should be updated");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID,
                    raw_detect_list.vcslong_sorted_ref_det_idx[4],
                    "Reference [4] should not be updated (early exit)");
   CHECK_EQUAL_TEXT(3U, calib_ref_start_idx,
                    "Start index should be 3");
}

/** \purpose
 * Verify function skips already filled references when starting iteration
 * \req
 * NA
 */
TEST(test_RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info, RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info_TC_015_Skip_Filled_References)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Pre-fill references 1-5 and start from index 5
    */
   for (uint32_t i = 1U; i <= 5U; i++)
   {
      raw_detect_list.vcslong_sorted_ref_det_idx[i] = 10;
   }
   calib_ref_start_idx = 5U;

   // Detection below ref[5], should exit immediately
   const float32_t det_vcs_long = vcs_long_sorted_ref_points[5] - 1.0F;
   const uint32_t det_idx = 11U;

   /** \action
    * Call update function
    */
   RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       det_vcs_long,
       det_idx,
       calib_ref_start_idx,
       raw_detect_list);

   /** \result
    * No new references should be updated, detection is below next reference
    */
   CHECK_EQUAL_TEXT(10, raw_detect_list.vcslong_sorted_ref_det_idx[5],
                    "Reference [5] should remain unchanged");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_sorted_ref_det_idx[6],
                    "Reference [6] should not be updated");
   CHECK_EQUAL_TEXT(5U, calib_ref_start_idx,
                    "Start index should remain 5");
}

/*===========================================================================*
 * Test Cases - Negative Positions
 *===========================================================================*/

/** \purpose
 * Verify function handles negative VCS longitudinal positions
 * \req
 * NA
 */
TEST(test_RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info, RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info_TC_016_Negative_Position)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Detection at negative position - use position below first reference point
    * First reference point is typically negative (around -200m)
    */
   const float32_t first_ref_point = vcs_long_sorted_ref_points[0];
   const float32_t det_vcs_long = first_ref_point - 10.0F; // Below first reference
   const uint32_t det_idx = 1U;

   /** \action
    * Call update function
    */
   RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       det_vcs_long,
       det_idx,
       calib_ref_start_idx,
       raw_detect_list);

   /** \result
    * Detection below first reference point should not update any references
    */
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID,
                    raw_detect_list.vcslong_sorted_ref_det_idx[1],
                    "Reference [1] should not be updated for detection below first reference");
   CHECK_EQUAL_TEXT(0U, calib_ref_start_idx,
                    "Start index should remain 0 for detection below all references");
}

/** \purpose
 * Verify function handles zero position correctly
 * \req
 * NA
 */
TEST(test_RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info, RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info_TC_017_Zero_Position)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Detection at zero position
    */
   const float32_t det_vcs_long = 0.0F;
   const uint32_t det_idx = 1U;

   /** \action
    * Call update function
    */
   RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       det_vcs_long,
       det_idx,
       calib_ref_start_idx,
       raw_detect_list);

   /** \result
    * Should correctly evaluate zero against reference points
    */
   for (uint32_t i = 0U; i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS; i++)
   {
      if (vcs_long_sorted_ref_points[i] < 0.0F)
      {
         // If reference is negative and detection at zero is above it
         CHECK_EQUAL_TEXT(static_cast<int16_t>(det_idx),
                          raw_detect_list.vcslong_sorted_ref_det_idx[i + 1],
                          "Zero position should update references below zero");
      }
   }
}

/*===========================================================================*
 * Test Cases - Integration Scenarios
 *===========================================================================*/

/** \purpose
 * Verify function works correctly in realistic multi-detection scenario
 * \req
 * NA
 */
TEST(test_RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info, RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info_TC_018_Realistic_Multi_Detection)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Simulate sorting 5 detections at various positions
    */
   const float32_t positions[] = {
       vcs_long_sorted_ref_points[1] + 1.0F,
       vcs_long_sorted_ref_points[4] + 1.0F,
       vcs_long_sorted_ref_points[8] + 1.0F,
       vcs_long_sorted_ref_points[15] + 1.0F,
       vcs_long_sorted_ref_points[25] + 1.0F};

   /** \action
    * Simulate sorting 5 detections at various positions
    */
   for (uint32_t det_idx = 0U; det_idx < 5U; det_idx++)
   {
      RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
          positions[det_idx],
          det_idx + 1,
          calib_ref_start_idx,
          raw_detect_list);
   }

   /** \result
    * Verify reference array progression is correct
    */
   CHECK_EQUAL_TEXT(1, raw_detect_list.vcslong_sorted_ref_det_idx[2],
                    "Early reference should point to first detection");
   CHECK_EQUAL_TEXT(2, raw_detect_list.vcslong_sorted_ref_det_idx[5],
                    "Mid-early reference should point to second detection");
   CHECK_EQUAL_TEXT(3, raw_detect_list.vcslong_sorted_ref_det_idx[9],
                    "Mid reference should point to third detection");
   CHECK_EQUAL_TEXT(4, raw_detect_list.vcslong_sorted_ref_det_idx[16],
                    "Mid-late reference should point to fourth detection");
   CHECK_EQUAL_TEXT(5, raw_detect_list.vcslong_sorted_ref_det_idx[26],
                    "Late reference should point to fifth detection");
   CHECK_EQUAL_TEXT(26U, calib_ref_start_idx,
                    "Final start index should be 26");
}

/** \purpose
 * Verify function handles detections with small spacing
 * \req
 * NA
 */
TEST(test_RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info, RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info_TC_019_Dense_Detections)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create detections with small increments between ref[5] and ref[6]
    */
   const float32_t base_pos = vcs_long_sorted_ref_points[5];

   /** \action
    * Create detections with small increments between ref[5] and ref[6]
    */
   // Three detections closely spaced
   RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       base_pos + 0.1F, 1U, calib_ref_start_idx, raw_detect_list);
   RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       base_pos + 0.2F, 2U, calib_ref_start_idx, raw_detect_list);
   RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
       base_pos + 0.3F, 3U, calib_ref_start_idx, raw_detect_list);

   /** \result
    * First detection should fill ref[6], subsequent ones find it already filled
    */
   CHECK_EQUAL_TEXT(1, raw_detect_list.vcslong_sorted_ref_det_idx[6],
                    "First detection should update reference");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_sorted_ref_det_idx[7],
                    "Next reference should not be updated (detections still below ref[6])");
}

/** \purpose
 * Verify reference array maintains monotonic detection index property
 * \req
 * NA
 */
TEST(test_RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info, RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info_TC_020_Reference_Array_Consistency)
{
   /** \step{1}
    * Execute test and verify result.
    */

   for (uint32_t i = 0U; i < 10U; i++)
   {
      /** \precond
       * Update references with monotonically increasing positions
       */
      const float32_t det_vcs_long = vcs_long_sorted_ref_points[i * 2] + 1.0F;

      /** \action
       * Update references with monotonically increasing positions
       */
      RSPP_Update_Dets_Vcs_Long_Ref_Sorted_Info(
          det_vcs_long,
          i + 1,
          calib_ref_start_idx,
          raw_detect_list);
   }

   /** \result
    * Reference array should be monotonically non-decreasing (or INVALID)
    */
   int16_t prev_idx = 0;
   for (uint32_t i = 1U; i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS; i++)
   {
      const int16_t curr_idx = raw_detect_list.vcslong_sorted_ref_det_idx[i];
      if (curr_idx != RSPP_INVALID_ID)
      {
         CHECK_TRUE_TEXT(curr_idx >= prev_idx,
                         "Reference indices should be non-decreasing");
         prev_idx = curr_idx;
      }
   }
}

/** @}*/

/** \defgroup  test_RSPP_Clear_Dets_Vcs_Long_Sorted_Info
 *  @{
 */

/** \brief
 *  Tests for RSPP_Clear_Dets_Vcs_Long_Sorted_Info
 */
TEST_GROUP(test_RSPP_Clear_Dets_Vcs_Long_Sorted_Info)
{
   RSPP_Detection_List_T raw_detect_list;

   /** \setup
    * Initialize detection list with arbitrary test data.
    */
   TEST_SETUP()
   {
      // Initialize detection list with some arbitrary data
      Populate_Detection_List_With_Data(raw_detect_list, 5, 10);
   }

   /** \teardown
    * No cleanup required for this test group.
    */
   TEST_TEARDOWN()
   {
   }
};

/*===========================================================================*
 * Test Cases - Basic Functionality
 *===========================================================================*/

/** \purpose
 * Verify function clears all VCS longitudinal sorting information
 * \req
 * NA
 */
TEST(test_RSPP_Clear_Dets_Vcs_Long_Sorted_Info, RSPP_Clear_Dets_Vcs_Long_Sorted_Info_TC_001_Basic_Clear)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Detection list already populated in TEST_SETUP with min=5, max=10
    */

   /** \action
    * Call clear function
    */
   RSPP_Clear_Dets_Vcs_Long_Sorted_Info(raw_detect_list);

   /** \result
    * All fields should be set to RSPP_INVALID_ID
    */
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_min,
                    "Min index should be RSPP_INVALID_ID");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_max,
                    "Max index should be RSPP_INVALID_ID");
   CHECK_TRUE_TEXT(All_Reference_Indices_Cleared(raw_detect_list),
                   "All reference indices should be RSPP_INVALID_ID");
}

/** \purpose
 * Verify function is idempotent and safe to call multiple times
 * \req
 * NA
 */
TEST(test_RSPP_Clear_Dets_Vcs_Long_Sorted_Info, RSPP_Clear_Dets_Vcs_Long_Sorted_Info_TC_002_Clear_Already_Cleared)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Clear the detection list first
    */
   RSPP_Clear_Dets_Vcs_Long_Sorted_Info(raw_detect_list);

   /** \action
    * Call clear function again on already cleared list
    */
   RSPP_Clear_Dets_Vcs_Long_Sorted_Info(raw_detect_list);

   /** \result
    * All fields should still be RSPP_INVALID_ID (idempotent operation)
    */
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_min,
                    "Min index should remain RSPP_INVALID_ID");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_max,
                    "Max index should remain RSPP_INVALID_ID");
   CHECK_TRUE_TEXT(All_Reference_Indices_Cleared(raw_detect_list),
                   "All reference indices should remain RSPP_INVALID_ID");
}

/** \purpose
 * Verify function handles maximum valid indices correctly
 * \req
 * NA
 */
TEST(test_RSPP_Clear_Dets_Vcs_Long_Sorted_Info, RSPP_Clear_Dets_Vcs_Long_Sorted_Info_TC_003_Max_Index_Values)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set detection list with maximum valid indices
    */
   const int16_t max_valid_idx = static_cast<int16_t>(MAX_NUMBER_OF_DETECTIONS - 1);
   Populate_Detection_List_With_Data(raw_detect_list, max_valid_idx, max_valid_idx);

   // Fill all reference indices with maximum value
   for (uint32_t i = 0U; i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS; i++)
   {
      raw_detect_list.vcslong_sorted_ref_det_idx[i] = max_valid_idx;
   }

   /** \action
    * Call clear function
    */
   RSPP_Clear_Dets_Vcs_Long_Sorted_Info(raw_detect_list);

   /** \result
    * All fields should be cleared to RSPP_INVALID_ID
    */
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_min,
                    "Min index should be cleared");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_max,
                    "Max index should be cleared");
   CHECK_TRUE_TEXT(All_Reference_Indices_Cleared(raw_detect_list),
                   "All reference indices should be cleared");
}

/** \purpose
 * Verify function handles minimum valid indices (zero) correctly
 * \req
 * NA
 */
TEST(test_RSPP_Clear_Dets_Vcs_Long_Sorted_Info, RSPP_Clear_Dets_Vcs_Long_Sorted_Info_TC_004_Min_Index_Values)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set detection list with minimum valid indices (zero)
    */
   Populate_Detection_List_With_Data(raw_detect_list, 0, 0);

   // Fill all reference indices with zero
   for (uint32_t i = 0U; i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS; i++)
   {
      raw_detect_list.vcslong_sorted_ref_det_idx[i] = 0;
   }

   /** \action
    * Call clear function
    */
   RSPP_Clear_Dets_Vcs_Long_Sorted_Info(raw_detect_list);

   /** \result
    * All fields should be cleared to RSPP_INVALID_ID
    */
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_min,
                    "Min index should be cleared");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_max,
                    "Max index should be cleared");
   CHECK_TRUE_TEXT(All_Reference_Indices_Cleared(raw_detect_list),
                   "All reference indices should be cleared");
}

/*===========================================================================*
 * Test Cases - Reference Array Clearing
 *===========================================================================*/

/** \purpose
 * Verify function clears all reference indices including partially filled array
 * \req
 * NA
 */
TEST(test_RSPP_Clear_Dets_Vcs_Long_Sorted_Info, RSPP_Clear_Dets_Vcs_Long_Sorted_Info_TC_005_Partial_Reference_Array)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Fill only first half of reference array with valid indices
    */
   const uint32_t half_size = MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS / 2U;
   for (uint32_t i = 0U; i < half_size; i++)
   {
      raw_detect_list.vcslong_sorted_ref_det_idx[i] = static_cast<int16_t>(i);
   }
   // Second half already has RSPP_INVALID_ID or arbitrary values
   for (uint32_t i = half_size; i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS; i++)
   {
      raw_detect_list.vcslong_sorted_ref_det_idx[i] = RSPP_INVALID_ID;
   }

   /** \action
    * Call clear function
    */
   RSPP_Clear_Dets_Vcs_Long_Sorted_Info(raw_detect_list);

   /** \result
    * Entire reference array should be cleared
    */
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_min,
                    "Min index should be cleared");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_max,
                    "Max index should be cleared");
   CHECK_TRUE_TEXT(All_Reference_Indices_Cleared(raw_detect_list),
                   "All reference indices should be cleared including partially filled array");
}

/** \purpose
 * Verify function clears complex reference array patterns
 * \req
 * NA
 */
TEST(test_RSPP_Clear_Dets_Vcs_Long_Sorted_Info, RSPP_Clear_Dets_Vcs_Long_Sorted_Info_TC_006_Alternating_Pattern)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Fill reference array with alternating valid and invalid indices
    */
   for (uint32_t i = 0U; i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS; i++)
   {
      if ((i % 2U) == 0U)
      {
         raw_detect_list.vcslong_sorted_ref_det_idx[i] = static_cast<int16_t>(i / 2U);
      }
      else
      {
         raw_detect_list.vcslong_sorted_ref_det_idx[i] = RSPP_INVALID_ID;
      }
   }

   /** \action
    * Call clear function
    */
   RSPP_Clear_Dets_Vcs_Long_Sorted_Info(raw_detect_list);

   /** \result
    * All reference indices should be cleared regardless of initial pattern
    */
   CHECK_TRUE_TEXT(All_Reference_Indices_Cleared(raw_detect_list),
                   "All reference indices should be cleared including alternating pattern");
}

/** \purpose
 * Verify function clears boundary reference indices correctly
 * \req
 * NA
 */
TEST(test_RSPP_Clear_Dets_Vcs_Long_Sorted_Info, RSPP_Clear_Dets_Vcs_Long_Sorted_Info_TC_007_Boundary_Indices)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set specific values for first and last reference indices
    */
   raw_detect_list.vcslong_sorted_ref_det_idx[0] = 0;
   raw_detect_list.vcslong_sorted_ref_det_idx[MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS - 1] =
       static_cast<int16_t>(MAX_NUMBER_OF_DETECTIONS - 1);

   /** \action
    * Call clear function
    */
   RSPP_Clear_Dets_Vcs_Long_Sorted_Info(raw_detect_list);

   /** \result
    * First and last reference indices should be cleared
    */
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_sorted_ref_det_idx[0],
                    "First reference index should be cleared");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID,
                    raw_detect_list.vcslong_sorted_ref_det_idx[MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS - 1],
                    "Last reference index should be cleared");
}

/*===========================================================================*
 * Test Cases - Integration with Sorting
 *===========================================================================*/

/** \purpose
 * Verify function properly clears sorting information after a sort operation
 * \req
 * NA
 */
TEST(test_RSPP_Clear_Dets_Vcs_Long_Sorted_Info, RSPP_Clear_Dets_Vcs_Long_Sorted_Info_TC_008_After_Sorting)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create and sort a detection list
    */
   const float32_t det_positions[] = {10.0F, 30.0F, 50.0F, 70.0F};
   Setup_Sorted_Detection_List(raw_detect_list, 4U, det_positions);

   // Verify sorting populated the fields
   CHECK_TRUE_TEXT(raw_detect_list.vcslong_det_idx_min != RSPP_INVALID_ID,
                   "Min index should be set after sorting");
   CHECK_TRUE_TEXT(raw_detect_list.vcslong_det_idx_max != RSPP_INVALID_ID,
                   "Max index should be set after sorting");

   /** \action
    * Call clear function
    */
   RSPP_Clear_Dets_Vcs_Long_Sorted_Info(raw_detect_list);

   /** \result
    * All sorting information should be cleared
    */
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_min,
                    "Min index should be cleared after sorting");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_max,
                    "Max index should be cleared after sorting");
   CHECK_TRUE_TEXT(All_Reference_Indices_Cleared(raw_detect_list),
                   "All reference indices should be cleared after sorting");
}

/** \purpose
 * Verify function clears sorting information for single detection case
 * \req
 * NA
 */
TEST(test_RSPP_Clear_Dets_Vcs_Long_Sorted_Info, RSPP_Clear_Dets_Vcs_Long_Sorted_Info_TC_009_Single_Detection)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create and sort a single detection
    */
   const float32_t det_positions[] = {25.0F};
   Setup_Sorted_Detection_List(raw_detect_list, 1U, det_positions);

   /** \action
    * Call clear function
    */
   RSPP_Clear_Dets_Vcs_Long_Sorted_Info(raw_detect_list);

   /** \result
    * All sorting information should be cleared
    */
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_min,
                    "Min index should be cleared for single detection");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_max,
                    "Max index should be cleared for single detection");
   CHECK_TRUE_TEXT(All_Reference_Indices_Cleared(raw_detect_list),
                   "All reference indices should be cleared for single detection");
}

/** \purpose
 * Verify function handles clearing after sorting maximum number of detections
 * \req
 * NA
 */
TEST(test_RSPP_Clear_Dets_Vcs_Long_Sorted_Info, RSPP_Clear_Dets_Vcs_Long_Sorted_Info_TC_010_Maximum_Detections)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create detection list with many detections
    */
   const uint32_t num_dets = 20U; // Use a reasonable number for testing
   float32_t det_positions[20];
   for (uint32_t i = 0U; i < num_dets; i++)
   {
      det_positions[i] = static_cast<float32_t>(i) * 10.0F;
   }
   Setup_Sorted_Detection_List(raw_detect_list, num_dets, det_positions);

   /** \action
    * Call clear function
    */
   RSPP_Clear_Dets_Vcs_Long_Sorted_Info(raw_detect_list);

   /** \result
    * All sorting information should be cleared even with many detections
    */
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_min,
                    "Min index should be cleared for many detections");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_max,
                    "Max index should be cleared for many detections");
   CHECK_TRUE_TEXT(All_Reference_Indices_Cleared(raw_detect_list),
                   "All reference indices should be cleared for many detections");
}

/*===========================================================================*
 * Test Cases - Repeated Operations
 *===========================================================================*/

/** \purpose
 * Verify function can be called multiple times without side effects
 * \req
 * NA
 */
TEST(test_RSPP_Clear_Dets_Vcs_Long_Sorted_Info, RSPP_Clear_Dets_Vcs_Long_Sorted_Info_TC_011_Multiple_Clears)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Detection list already populated
    */

   /** \action
    * Call clear function multiple times
    */
   RSPP_Clear_Dets_Vcs_Long_Sorted_Info(raw_detect_list);
   RSPP_Clear_Dets_Vcs_Long_Sorted_Info(raw_detect_list);
   RSPP_Clear_Dets_Vcs_Long_Sorted_Info(raw_detect_list);

   /** \result
    * Should remain cleared after multiple calls
    */
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_min,
                    "Min index should remain cleared");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_max,
                    "Max index should remain cleared");
   CHECK_TRUE_TEXT(All_Reference_Indices_Cleared(raw_detect_list),
                   "All reference indices should remain cleared");
}

/** \purpose
 * Verify function works correctly in clear-populate-clear cycles
 * \req
 * NA
 */
TEST(test_RSPP_Clear_Dets_Vcs_Long_Sorted_Info, RSPP_Clear_Dets_Vcs_Long_Sorted_Info_TC_012_Clear_Populate_Clear_Cycle)
{
   /** \step{1}
    * Execute test and verify result.
    */
   /** \precond
    * Perform multiple clear-populate cycles
    */

   /** \action
    * Perform multiple clear-populate cycles
    */
   for (uint32_t cycle = 0U; cycle < 3U; cycle++)
   {
      // Clear
      RSPP_Clear_Dets_Vcs_Long_Sorted_Info(raw_detect_list);

      // Verify cleared
      CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_min,
                       "Min index should be cleared in cycle");
      CHECK_TRUE_TEXT(All_Reference_Indices_Cleared(raw_detect_list),
                      "All indices should be cleared in cycle");

      // Populate
      Populate_Detection_List_With_Data(raw_detect_list,
                                        static_cast<int16_t>(cycle),
                                        static_cast<int16_t>(cycle + 5));

      // Verify populated
      CHECK_EQUAL_TEXT(static_cast<int16_t>(cycle), raw_detect_list.vcslong_det_idx_min,
                       "Min index should be populated in cycle");
   }

   // Final clear
   RSPP_Clear_Dets_Vcs_Long_Sorted_Info(raw_detect_list);

   /** \result
    * Should be properly cleared after multiple cycles
    */
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_min,
                    "Min index should be cleared after cycles");
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_det_idx_max,
                    "Max index should be cleared after cycles");
   CHECK_TRUE_TEXT(All_Reference_Indices_Cleared(raw_detect_list),
                   "All reference indices should be cleared after cycles");
}

/*===========================================================================*
 * Test Cases - Individual Field Verification
 *===========================================================================*/

/** \purpose
 * Verify every single reference index in the array is properly cleared
 * \req
 * NA
 */
TEST(test_RSPP_Clear_Dets_Vcs_Long_Sorted_Info, RSPP_Clear_Dets_Vcs_Long_Sorted_Info_TC_013_Individual_Index_Verification)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Fill all reference indices with unique values
    */
   for (uint32_t i = 0U; i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS; i++)
   {
      raw_detect_list.vcslong_sorted_ref_det_idx[i] = static_cast<int16_t>(i);
   }

   /** \action
    * Call clear function
    */
   RSPP_Clear_Dets_Vcs_Long_Sorted_Info(raw_detect_list);

   /** \result
    * Verify each individual reference index is cleared
    */
   for (uint32_t i = 0U; i < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS_ELEMENTS; i++)
   {
      CHECK_EQUAL_TEXT(RSPP_INVALID_ID, raw_detect_list.vcslong_sorted_ref_det_idx[i],
                       "Each reference index should be individually cleared");
   }
}

/** @}*/

/** \defgroup  test_RSPP_Get_First_Relevant_Long_Sorted_Det_Idx
 *  @{
 */

/** \brief
 *  Tests for RSPP_Get_First_Relevant_Long_Sorted_Det_Idx
 */
TEST_GROUP(test_RSPP_Get_First_Relevant_Long_Sorted_Det_Idx)
{
   RSPP_Detection_List_T raw_detect_list;
   static constexpr float32_t EPSILON = 0.01F;
   static constexpr float32_t LARGE_VALUE = 1000.0F;
   static constexpr float32_t SMALL_VALUE = -1000.0F;

   /** \setup
    * Initialize calibrations and clear detection list.
    */
   TEST_SETUP()
   {
      // Clear detection list
      RSPP_Clear_Dets_Vcs_Long_Sorted_Info(raw_detect_list);
      raw_detect_list.number_of_valid_detections = 0U;
   }

   /** \teardown
    * No cleanup required for this test group.
    */
   TEST_TEARDOWN()
   {
   }
};

/*===========================================================================*
 * Test Cases - Basic Functionality
 *===========================================================================*/

/** \purpose
 * Verify function returns RSPP_INVALID_ID when detection list is empty
 * \req
 * NA
 */
TEST(test_RSPP_Get_First_Relevant_Long_Sorted_Det_Idx, RSPP_Get_First_Relevant_Long_Sorted_Det_Idx_TC_001_No_Detections)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Detection list is already cleared in TEST_SETUP
    */

   /** \action
    * Call function with any VCS long value
    */
   const float32_t vcs_long_value = 50.0F;
   const int32_t result = RSPP_Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);

   /** \result
    * Function should return RSPP_INVALID_ID
    */
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, result, "Expected RSPP_INVALID_ID for empty detection list");
}

/** \purpose
 * Verify function returns INVALID_ID when searching above the only detection
 * \req
 * NA
 */
TEST(test_RSPP_Get_First_Relevant_Long_Sorted_Det_Idx, RSPP_Get_First_Relevant_Long_Sorted_Det_Idx_TC_002_Single_Detection_Below)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create one detection at position 10.0
    */
   const float32_t det_positions[] = {10.0F};
   Setup_Sorted_Detection_List(raw_detect_list, 1U, det_positions);

   /** \action
    * Search for detection with VCS long value greater than the detection
    */
   const float32_t vcs_long_value = 20.0F;
   const int32_t result = RSPP_Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);

   /** \result
    * Function should return INVALID_ID when no detection overshoots the search value
    */
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, result, "Expected RSPP_INVALID_ID when search value above all detections");
}

/** \purpose
 * Verify function returns the minimum detection when search value is below it
 * \req
 * NA
 */
TEST(test_RSPP_Get_First_Relevant_Long_Sorted_Det_Idx, RSPP_Get_First_Relevant_Long_Sorted_Det_Idx_TC_003_Single_Detection_Above)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create one detection at position 50.0
    */
   const float32_t det_positions[] = {50.0F};
   Setup_Sorted_Detection_List(raw_detect_list, 1U, det_positions);

   /** \action
    * Search for detection with VCS long value less than the detection
    */
   const float32_t vcs_long_value = 10.0F;
   const int32_t result = RSPP_Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);

   /** \result
    * Function should return index 0 (minimum detection from vcslong_sorted_ref_det_idx[0])
    */
   CHECK_EQUAL_TEXT(0, result, "Expected detection index 0 when search below all detections");
   CHECK_EQUAL_TEXT(raw_detect_list.vcslong_det_idx_min, result, "Should match minimum detection index");
}

/*===========================================================================*
 * Test Cases - Multiple Detections
 *===========================================================================*/

/** \purpose
 * Verify function returns correct detection index when search value is between detections
 * \req
 * NA
 */
TEST(test_RSPP_Get_First_Relevant_Long_Sorted_Det_Idx, RSPP_Get_First_Relevant_Long_Sorted_Det_Idx_TC_004_Multiple_Middle_Value)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create detections at calibration reference points
    */
   const float32_t det_positions[] = {
       vcs_long_sorted_ref_points[2] + 1.0F,
       vcs_long_sorted_ref_points[5] + 1.0F,
       vcs_long_sorted_ref_points[10] + 1.0F,
       vcs_long_sorted_ref_points[15] + 1.0F};
   Setup_Sorted_Detection_List(raw_detect_list, 4U, det_positions);

   /** \action
    * Search for value between second and third detection
    */
   const float32_t vcs_long_value = vcs_long_sorted_ref_points[8];
   const int32_t result = RSPP_Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);

   /** \result
    * Function should return index of second detection (sorted index 1)
    */
   CHECK_TRUE_TEXT(Is_Valid_Detection_Index(result), "Result should be a valid detection index");
   CHECK_TRUE_TEXT(raw_detect_list.detections[result].processed.vcs_position_x < vcs_long_value,
                   "Returned detection should be below search value");

   // Verify next detection (if exists) is above search value
   const int32_t next_idx = raw_detect_list.detections[result].processed.next_sorted_idx;
   if (next_idx != RSPP_INVALID_ID)
   {
      CHECK_TRUE_TEXT(raw_detect_list.detections[next_idx].processed.vcs_position_x > vcs_long_value,
                      "Next detection should be above search value");
   }
}

/** \purpose
 * Verify function returns minimum detection when search value is below all detections
 * \req
 * NA
 */
TEST(test_RSPP_Get_First_Relevant_Long_Sorted_Det_Idx, RSPP_Get_First_Relevant_Long_Sorted_Det_Idx_TC_005_Multiple_Below_All)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create multiple detections at positive positions
    */
   const float32_t det_positions[] = {10.0F, 20.0F, 30.0F, 40.0F, 50.0F};
   Setup_Sorted_Detection_List(raw_detect_list, 5U, det_positions);

   /** \action
    * Search for value below all detections
    */
   const float32_t vcs_long_value = SMALL_VALUE;
   const int32_t result = RSPP_Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);

   /** \result
    * Function should return minimum detection index (from vcslong_sorted_ref_det_idx[0])
    */
   CHECK_EQUAL_TEXT(raw_detect_list.vcslong_det_idx_min, result, "Expected minimum detection index when search value below all detections");
   CHECK_TRUE_TEXT(Is_Valid_Detection_Index(result), "Result should be a valid detection index");
}

/** \purpose
 * Verify function returns INVALID_ID when search value is above all detections
 * \req
 * NA
 */
TEST(test_RSPP_Get_First_Relevant_Long_Sorted_Det_Idx, RSPP_Get_First_Relevant_Long_Sorted_Det_Idx_TC_006_Multiple_Above_All)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create multiple detections
    */
   const float32_t det_positions[] = {10.0F, 20.0F, 30.0F, 40.0F, 50.0F};
   Setup_Sorted_Detection_List(raw_detect_list, 5U, det_positions);

   /** \action
    * Search for value above all detections
    */
   const float32_t vcs_long_value = LARGE_VALUE;
   const int32_t result = RSPP_Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);

   /** \result
    * Function should return INVALID_ID when no detection overshoots the search value
    */
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, result,
                    "Expected RSPP_INVALID_ID when search value above all detections");
}

/*===========================================================================*
 * Test Cases - Boundary Conditions
 *===========================================================================*/

/** \purpose
 * Verify function behavior when search value equals a detection position
 * \req
 * NA
 */
TEST(test_RSPP_Get_First_Relevant_Long_Sorted_Det_Idx, RSPP_Get_First_Relevant_Long_Sorted_Det_Idx_TC_007_Exact_Match)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create detections at known positions
    */
   const float32_t det_positions[] = {10.0F, 30.0F, 50.0F};
   Setup_Sorted_Detection_List(raw_detect_list, 3U, det_positions);

   /** \action
    * Search for value exactly at middle detection position
    */
   const float32_t vcs_long_value = 30.0F;
   const int32_t result = RSPP_Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);

   /** \result
    * Function should return the detection at or just below the search value
    */
   CHECK_TRUE_TEXT(Is_Valid_Detection_Index(result), "Result should be a valid detection index");
   CHECK_TRUE_TEXT(raw_detect_list.detections[result].processed.vcs_position_x <= vcs_long_value,
                   "Returned detection should be at or below search value");
}

/** \purpose
 * Verify function handles small differences correctly
 * \req
 * NA
 */
TEST(test_RSPP_Get_First_Relevant_Long_Sorted_Det_Idx, RSPP_Get_First_Relevant_Long_Sorted_Det_Idx_TC_008_Near_Detection)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create detections at known positions
    */
   const float32_t det_positions[] = {10.0F, 20.0F, 30.0F};
   Setup_Sorted_Detection_List(raw_detect_list, 3U, det_positions);

   /** \action
    * Search for value just above middle detection
    */
   const float32_t vcs_long_value = 20.0F + EPSILON;
   const int32_t result = RSPP_Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);

   /** \result
    * Function should return the middle detection
    */
   CHECK_TRUE_TEXT(Is_Valid_Detection_Index(result), "Result should be a valid detection index");
   CHECK_TRUE_TEXT(raw_detect_list.detections[result].processed.vcs_position_x < vcs_long_value,
                   "Returned detection should be below search value");
}

/** \purpose
 * Verify function works correctly at calibration reference points
 * \req
 * NA
 */
TEST(test_RSPP_Get_First_Relevant_Long_Sorted_Det_Idx, RSPP_Get_First_Relevant_Long_Sorted_Det_Idx_TC_009_Reference_Boundaries)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create detections exactly at reference points
    */
   const float32_t det_positions[] = {
       vcs_long_sorted_ref_points[0],
       vcs_long_sorted_ref_points[5],
       vcs_long_sorted_ref_points[15],
       vcs_long_sorted_ref_points[30]};
   Setup_Sorted_Detection_List(raw_detect_list, 4U, det_positions);

   /** \action
    * Search at reference point between two detections
    */
   const float32_t vcs_long_value = vcs_long_sorted_ref_points[10];
   const int32_t result = RSPP_Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);

   /** \result
    * Function should return appropriate detection
    */
   CHECK_TRUE_TEXT(Is_Valid_Detection_Index(result), "Result should be a valid detection index");
   CHECK_TRUE_TEXT(raw_detect_list.detections[result].processed.vcs_position_x <= vcs_long_value,
                   "Returned detection should be at or below search value");
}

/*===========================================================================*
 * Test Cases - Reference Array Edge Cases
 *===========================================================================*/

/** \purpose
 * Verify function handles detection at minimum position correctly
 * \req
 * NA
 */
TEST(test_RSPP_Get_First_Relevant_Long_Sorted_Det_Idx, RSPP_Get_First_Relevant_Long_Sorted_Det_Idx_TC_010_First_Reference)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create single detection at very low position
    */
   const float32_t det_positions[] = {SMALL_VALUE};
   Setup_Sorted_Detection_List(raw_detect_list, 1U, det_positions);

   /** \action
    * Search for value slightly above the detection
    */
   const float32_t vcs_long_value = SMALL_VALUE + 10.0F;
   const int32_t result = RSPP_Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);

   /** \result
    * Function should return INVALID_ID since there's no detection that overshoots the search value
    */
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, result, "Expected RSPP_INVALID_ID when search above only detection");
}

/** \purpose
 * Verify function handles detection at maximum position correctly
 * \req
 * NA
 */
TEST(test_RSPP_Get_First_Relevant_Long_Sorted_Det_Idx, RSPP_Get_First_Relevant_Long_Sorted_Det_Idx_TC_011_Last_Reference)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create detections spanning to last reference point
    */
   const float32_t det_positions[] = {
       vcs_long_sorted_ref_points[0],
       vcs_long_sorted_ref_points[MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS - 1] + 1.0F};
   Setup_Sorted_Detection_List(raw_detect_list, 2U, det_positions);

   /** \action
    * Search for value beyond last detection
    */
   const float32_t vcs_long_value = LARGE_VALUE;
   const int32_t result = RSPP_Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);

   /** \result
    * Function should return INVALID_ID when search value is above all detections
    */
   CHECK_EQUAL_TEXT(RSPP_INVALID_ID, result, "Expected RSPP_INVALID_ID when search value above all detections");
}

/*===========================================================================*
 * Test Cases - Dense Detection Scenarios
 *===========================================================================*/

/** \purpose
 * Verify function performs correctly with high detection density
 * \req
 * NA
 */
TEST(test_RSPP_Get_First_Relevant_Long_Sorted_Det_Idx, RSPP_Get_First_Relevant_Long_Sorted_Det_Idx_TC_012_Dense_Detections)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create many detections at regular intervals
    */
   const uint32_t num_dets = 10U;
   float32_t det_positions[num_dets];
   for (uint32_t i = 0U; i < num_dets; i++)
   {
      det_positions[i] = static_cast<float32_t>(i) * 5.0F;
   }
   Setup_Sorted_Detection_List(raw_detect_list, num_dets, det_positions);

   /** \action
    * Search for value in middle of dense region
    */
   const float32_t vcs_long_value = 22.5F; // Between 20 and 25
   const int32_t result = RSPP_Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);

   /** \result
    * Function should return a valid detection from reference array that is at or below search value
    * The function returns the closest reference detection, not necessarily the immediately
    * preceding detection in the sorted list
    */
   CHECK_TRUE_TEXT(Is_Valid_Detection_Index(result), "Result should be a valid detection index");
   CHECK_TRUE_TEXT(raw_detect_list.detections[result].processed.vcs_position_x <= vcs_long_value,
                   "Returned detection should be at or below search value");
}

/** \purpose
 * Verify function works when reference array is fully populated
 * \req
 * NA
 */
TEST(test_RSPP_Get_First_Relevant_Long_Sorted_Det_Idx, RSPP_Get_First_Relevant_Long_Sorted_Det_Idx_TC_013_All_References_Filled)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create detection at every reference point (limited by MAX_NUMBER_OF_DETECTIONS)
    */
   const uint32_t num_dets = (MAX_NUMBER_OF_DETECTIONS < MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS)
                                 ? MAX_NUMBER_OF_DETECTIONS
                                 : MAX_NR_OF_VCS_LONG_SORTED_DETS_REF_POINTS;
   float32_t det_positions[MAX_NUMBER_OF_DETECTIONS];
   for (uint32_t i = 0U; i < num_dets; i++)
   {
      det_positions[i] = vcs_long_sorted_ref_points[i] + 0.5F;
   }
   Setup_Sorted_Detection_List(raw_detect_list, num_dets, det_positions);

   /** \action
    * Search for value between reference points
    */
   const uint32_t search_ref = num_dets / 2U;
   const float32_t vcs_long_value = vcs_long_sorted_ref_points[search_ref] + 0.25F;
   const int32_t result = RSPP_Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);

   /** \result
    * Function should return appropriate detection
    */
   CHECK_TRUE_TEXT(Is_Valid_Detection_Index(result), "Result should be a valid detection index");
   CHECK_TRUE_TEXT(raw_detect_list.detections[result].processed.vcs_position_x < vcs_long_value,
                   "Returned detection should be below search value");
}

/*===========================================================================*
 * Test Cases - Sparse Detection Scenarios
 *===========================================================================*/

/** \purpose
 * Verify function handles large gaps between detections
 * \req
 * NA
 */
TEST(test_RSPP_Get_First_Relevant_Long_Sorted_Det_Idx, RSPP_Get_First_Relevant_Long_Sorted_Det_Idx_TC_014_Sparse_Detections)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create detections with large gaps
    */
   const float32_t det_positions[] = {
       vcs_long_sorted_ref_points[1],
       vcs_long_sorted_ref_points[10],
       vcs_long_sorted_ref_points[25]};
   Setup_Sorted_Detection_List(raw_detect_list, 3U, det_positions);

   /** \action
    * Search for value in large gap between detections
    */
   const float32_t vcs_long_value = vcs_long_sorted_ref_points[15];
   const int32_t result = RSPP_Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);

   /** \result
    * Function should return the detection before the gap
    */
   CHECK_TRUE_TEXT(Is_Valid_Detection_Index(result), "Result should be a valid detection index");
   CHECK_TRUE_TEXT(raw_detect_list.detections[result].processed.vcs_position_x < vcs_long_value,
                   "Returned detection should be below search value");
}

/*===========================================================================*
 * Test Cases - Negative and Zero Positions
 *===========================================================================*/

/** \purpose
 * Verify function handles negative VCS longitudinal positions
 * \req
 * NA
 */
TEST(test_RSPP_Get_First_Relevant_Long_Sorted_Det_Idx, RSPP_Get_First_Relevant_Long_Sorted_Det_Idx_TC_015_Negative_Positions)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create detections at negative positions
    */
   const float32_t det_positions[] = {-50.0F, -30.0F, -10.0F, 10.0F, 30.0F};
   Setup_Sorted_Detection_List(raw_detect_list, 5U, det_positions);

   /** \action
    * Search for value at zero
    */
   const float32_t vcs_long_value = 0.0F;
   const int32_t result = RSPP_Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);

   /** \result
    * Function should return detection at -10.0
    */
   CHECK_TRUE_TEXT(Is_Valid_Detection_Index(result), "Result should be a valid detection index");
   CHECK_TRUE_TEXT(raw_detect_list.detections[result].processed.vcs_position_x < vcs_long_value,
                   "Returned detection should be below search value");
   CHECK_TRUE_TEXT(raw_detect_list.detections[result].processed.vcs_position_x >= -10.0F - EPSILON,
                   "Returned detection should be at -10.0");
}

/** \purpose
 * Verify function handles zero position correctly
 * \req
 * NA
 */
TEST(test_RSPP_Get_First_Relevant_Long_Sorted_Det_Idx, RSPP_Get_First_Relevant_Long_Sorted_Det_Idx_TC_016_Zero_Position)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create detections including one at zero
    */
   const float32_t det_positions[] = {-20.0F, 0.0F, 20.0F};
   Setup_Sorted_Detection_List(raw_detect_list, 3U, det_positions);

   /** \action
    * Search for value just above zero
    */
   const float32_t vcs_long_value = EPSILON;
   const int32_t result = RSPP_Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);

   /** \result
    * Function should return detection at zero
    */
   CHECK_TRUE_TEXT(Is_Valid_Detection_Index(result), "Result should be a valid detection index");
   CHECK_TRUE_TEXT(raw_detect_list.detections[result].processed.vcs_position_x <= vcs_long_value,
                   "Returned detection should be at or below search value");
}

/*===========================================================================*
 * Test Cases - Sequential Search Scenarios
 *===========================================================================*/

/** \purpose
 * Verify function consistency across multiple sequential calls
 * \req
 * NA
 */
TEST(test_RSPP_Get_First_Relevant_Long_Sorted_Det_Idx, RSPP_Get_First_Relevant_Long_Sorted_Det_Idx_TC_017_Sequential_Increasing)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create detections at regular intervals
    */
   const float32_t det_positions[] = {10.0F, 30.0F, 50.0F, 70.0F, 90.0F};
   Setup_Sorted_Detection_List(raw_detect_list, 5U, det_positions);

   /** \action
    * Perform sequential searches with increasing values
    */
   int32_t prev_result = RSPP_INVALID_ID;
   for (uint32_t i = 0U; i < 5U; i++)
   {
      const float32_t vcs_long_value = 15.0F + (static_cast<float32_t>(i) * 20.0F);
      const int32_t result = RSPP_Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);

      /** \result
       * Each result should be valid or at same/higher position than previous
       */
      if (prev_result != RSPP_INVALID_ID && result != RSPP_INVALID_ID)
      {
         CHECK_TRUE_TEXT(raw_detect_list.detections[result].processed.vcs_position_x >=
                             raw_detect_list.detections[prev_result].processed.vcs_position_x,
                         "Sequential results should be monotonically non-decreasing");
      }
      prev_result = result;
   }
}

/** \purpose
 * Verify function returns consistent results for repeated calls
 * \req
 * NA
 */
TEST(test_RSPP_Get_First_Relevant_Long_Sorted_Det_Idx, RSPP_Get_First_Relevant_Long_Sorted_Det_Idx_TC_018_Repeated_Search)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create detections
    */
   const float32_t det_positions[] = {10.0F, 20.0F, 30.0F, 40.0F};
   Setup_Sorted_Detection_List(raw_detect_list, 4U, det_positions);

   /** \action
    * Call function multiple times with same value
    */
   const float32_t vcs_long_value = 25.0F;
   const int32_t result1 = RSPP_Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);
   const int32_t result2 = RSPP_Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);
   const int32_t result3 = RSPP_Get_First_Relevant_Long_Sorted_Det_Idx(vcs_long_value, raw_detect_list);

   /** \result
    * All results should be identical
    */
   CHECK_EQUAL_TEXT(result1, result2, "Repeated calls should return same result");
   CHECK_EQUAL_TEXT(result1, result3, "Repeated calls should return same result");
}

/** @}*/
