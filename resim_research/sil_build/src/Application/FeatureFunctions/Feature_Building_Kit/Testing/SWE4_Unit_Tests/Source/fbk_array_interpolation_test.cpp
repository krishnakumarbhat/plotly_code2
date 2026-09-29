/**
 * @file fbk_array_interpolation_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for FBK array interpolation test.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42218}
 */

#include "fbk_array_interpolation_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_array_interpolation.c"
#include "ml_math.h"
#include "pa_reuse.h"
}


/**
 * Tests the routine for returning of index from an array. Here x shall be less than the value at the third place of the input
 * lookuptable. The returned index shall be 2. \uts{CSCSA-42250} \sdd{SF-4085} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Array_Interpolation_Test, Fbk_Get_Uint8_Idx_Of_Uint8_Asc_Arr__return_some_val)
{
   /** \arrange quanitity being below the third index of the input array */
   uint8_t x = uint8_test_array[2] - 1u;

   /** \action executes function to test */
   uint8_t index = Fbk_Get_Uint8_Idx_Of_Uint8_Asc_Arr(uint8_test_array, Array_Sizes, x);

   /** \assert index shall be 2 */
   EXPECT_EQ(index, 2);
}

/**
 * Tests the routine for returning of index from an array. Here x shall be the same value as the second place of the input
 * lookuptable. The returned index shall be 1 as the evaluation of the while loop fails and the index is not incremented. Thus the
 * default value is returned \uts{CSCSA-42251} \sdd{SF-4085} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Array_Interpolation_Test, Fbk_Get_Uint8_Idx_Of_Uint8_Asc_Arr__index_not_incremented)
{
   /** \arrange quanitity being the same as the second index of the input array */
   uint8_t x = uint8_test_array[1];

   /** \action executes function to test */

   uint8_t index = Fbk_Get_Uint8_Idx_Of_Uint8_Asc_Arr(uint8_test_array, Array_Sizes, x);
   /** \assert index shall be 1 */
   EXPECT_EQ(index, 1);
}

/**
 * Tests the routine for returning of index from an array. Here x shall be less than the value at the third place of the input
 * lookuptable. The returned index shall be 2. \uts{CSCSA-42252} \sdd{SF-4086} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Array_Interpolation_Test, Fbk_Get_Uint8_Idx_Of_Float_Asc_Arr__return_some_val)
{
   /** \arrange quanitity being below the third index of the input array */
   float32_T x = floating_point_test_array[2] - EPSILON;

   /** \action executes function to test */
   uint8_t index = Fbk_Get_Uint8_Idx_Of_Float_Asc_Arr(floating_point_test_array, Array_Sizes, x);

   /** \assert index shall be 2 */
   EXPECT_EQ(index, 2);
}

/**
 * Tests the routine for returning of index from an array. Here x shall be the same value as the second place of the input
 * lookuptable. The returned index shall be 1 as the evaluation of the while loop fails and the index is not incremented. Thus the
 * default value is returned \uts{CSCSA-42253} \sdd{SF-4086} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Array_Interpolation_Test, Fbk_Get_Uint8_Idx_Of_Float_Asc_Arr__index_not_incremented)
{
   /** \arrange quanitity being the same as the second index of the input array */
   float32_T x = floating_point_test_array[1];

   /** \action executes function to test */
   uint8_t index = Fbk_Get_Uint8_Idx_Of_Float_Asc_Arr(floating_point_test_array, Array_Sizes, x);

   /** \assert index shall be 1 */
   EXPECT_EQ(index, 1);
}

#ifndef NDEBUG
/**
 * Tests the routine for returning of index from an array. x shall exceed the maximum value of the array and thus trigger an
 * assertion event. \uts{CSCSA-42254} \sdd{SF-4086} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Array_Interpolation_Test, Fbk_Get_Uint8_Idx_Of_Float_Asc_Arr__assert_value_gt_arrays_max_val)
{
   /** \arrange x exceeding the maximum val */
   float32_T x = 15;
   EXPECT_DEATH(
      {
         /** \action executes function to test */
         Fbk_Get_Uint8_Idx_Of_Float_Asc_Arr(floating_point_test_array, Array_Sizes, x);
         /** \assert overflow due to the exceeding quantity */
      },
      ".*");
}

/**
 * Tests the routine for returning of index from an array. x shall be less than minimum value of the input array and thus trigger
 * an assertion event. \uts{CSCSA-42255} \sdd{SF-4086} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Array_Interpolation_Test, Fbk_Get_Uint8_Idx_Of_Float_Asc_Arr__assert_value_lt_arrays_min_val)
{
   /** \arrange x lt the minimum val */
   float32_T x = -15;

   EXPECT_DEATH(
      {
         /** \action executes function to test */
         Fbk_Get_Uint8_Idx_Of_Float_Asc_Arr(floating_point_test_array, Array_Sizes, x);
         /** \assert overflow due to exceeding x */
      },
      ".*");
}

/**
 * Tests the routine for returning of index from an array. Here the input array is a NULL pointer, thus an assertion is expected.
 * \uts{CSCSA-42256} \sdd{SF-4086} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Array_Interpolation_Test, Fbk_Get_Uint8_Idx_Of_Float_Asc_Arr__assert_array_NULL)
{
   /** \arrange set up input values to something senseful. */
   float32_T x = floating_point_test_array[1];

   EXPECT_DEATH(
      {
         /** \action executes function to test */
         Fbk_Get_Uint8_Idx_Of_Float_Asc_Arr(NULL, Array_Sizes, x);
         /** \assert expect assert since input array is NULL */
      },
      ".*");
}


/**
 * Tests the routine for returning of index from an array. x shall exceed the maximum value of the array and thus trigger an
 * assertion event. \uts{CSCSA-42257} \sdd{SF-4085} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Array_Interpolation_Test, Fbk_Get_Uint8_Idx_Of_Uint8_Asc_Arr__assert_value_gt_arrays_max_val)
{
   /** \arrange x exceeding the maximum val */
   uint8_t x = 15;

   EXPECT_DEATH(
      {
         /** \action executes function to test */
         Fbk_Get_Uint8_Idx_Of_Uint8_Asc_Arr(uint8_test_array, Array_Sizes, x);
         /** \assert overflow due to the exceeding quantity */
      },
      ".*");
}

/**
 * Tests the routine for returning of index from an array. x shall be less than minimum value of the input array and thus trigger
 * an assertion event. \uts{CSCSA-42258} \sdd{SF-4085} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Array_Interpolation_Test, Fbk_Get_Uint8_Idx_Of_Uint8_Asc_Arr__assert_value_lt_arrays_min_val)
{
   /** \arrange x lt the minimum val */
   uint8_t x = uint8_test_array[0] - 1u;

   EXPECT_DEATH(
      {
         /** \action executes function to test */
         Fbk_Get_Uint8_Idx_Of_Uint8_Asc_Arr(uint8_test_array, Array_Sizes, x);
         /** \assert overflow due to exceeding x */
      },
      ".*");
}


/**
 * Tests the routine for returning of index from an array. Here the input array is a NULL pointer, thus an assertion is expected.
 * \uts{CSCSA-42259} \sdd{SF-4085} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Array_Interpolation_Test, Fbk_Get_Uint8_Idx_Of_Uint8_Asc_Arr__assert_array_NULL)
{
   /** \arrange x lt the minimum val */
   uint8_t x = uint8_test_array[0] - 1u;

   EXPECT_DEATH(
      {
         /** \action executes function to test */
         Fbk_Get_Uint8_Idx_Of_Uint8_Asc_Arr(NULL, Array_Sizes, x);
         /** \assert expect assert since input array is NULL */
      },
      ".*");
}
#endif
