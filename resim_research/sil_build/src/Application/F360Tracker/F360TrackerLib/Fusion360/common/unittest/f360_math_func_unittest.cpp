/** \file
 *   Tests for f360_math_func
 */

#include "f360_math_func.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>
#include <algorithm>
#include <string.h>
#include <math.h>
#include <cstdio>
#include "f360_iterator.h"
#include "f360_constants.h"

//#include "headerfile_needed.h"
using namespace f360_variant_A;
//sneak in mocked functions

//Declaration of stubbed/mock functions
//Implementation of stubbed interfaces

/** \defgroup  f360_math_func
 *  @{
**/

/** \brief
 *  Tests for f360_math_func
 **/
TEST_GROUP(f360_math_func)
{
   /** \setup
    * Setting up float comparison thresholds for passing tests
    **/
   const float TEST_PASS_TH_LARGE = 1e-3f;
   const float TEST_PASS_TH_MID = 1e-5f;
   const float TEST_PASS_TH_SMALL = FLT_EPSILON;

   TEST_SETUP()
   {
      // nothing to set up;
   }

   /** \teardown
    * Nothing to teardown in this test group
    **/
   TEST_TEARDOWN()
   {
      //mock.clear();
   }

};

/**
 *\purpose  Check if F360_Get_Hypotenuse returns the correct value when inputs are negative.
 *\req    NA
 */
TEST(f360_math_func, test_f360_get_hypotenuse_negative_input)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float f1 = -3.0f;
   float f2 = -4.0f;
   float expected = 5.0f;
   float result;


   /** \action
    *Call function.
    **/
   result = F360_Get_Hypotenuse(f1, f2);

   /** \result
    *Ensure function output is equal to the expected result.
    **/

   DOUBLES_EQUAL(expected, result, TEST_PASS_TH_SMALL);
}


/**
 *\purpose  Check if F360_Get_Hypotenuse returns the correct value when inputs are positive.
 *\req    NA
 */
TEST(f360_math_func, test_f360_get_hypotenuse_positive_input)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float f1 = 3.0f;
   float f2 = 4.0f;
   float expected = 5.0f;
   float result;


   /** \action
    *Call function.
    **/
   result = F360_Get_Hypotenuse(f1, f2);

   /** \result
    *Ensure function output is equal to the expected result.
    **/

   DOUBLES_EQUAL(expected, result, TEST_PASS_TH_SMALL);
}


/**
 *\purpose  Check if F360_Get_Hypotenuse returns the correct value when inputs are zero.
 *\req    NA
 */
TEST(f360_math_func, test_f360_get_hypotenuse_zero_input)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float f1 = 0.0f;
   float f2 = 0.0f;
   float expected = 0.0f;
   float result;


   /** \action
    *Call function.
    **/
   result = F360_Get_Hypotenuse(f1, f2);

   /** \result
    *Ensure function output is equal to the expected result.
    **/

   DOUBLES_EQUAL(expected, result, TEST_PASS_TH_SMALL);
}


/**
 *\purpose  Check if F360_Get_Hypotenuse_Squared returns the correct value when inputs are negative.
 *\req    NA
 */
TEST(f360_math_func, test_f360_get_hypotenuse_squared_negative_input)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float f1 = -3.0f;
   float f2 = -4.0f;
   float expected = 25.0f;
   float result;


   /** \action
    *Call function.
    **/
   result = F360_Get_Hypotenuse_Squared(f1, f2);

   /** \result
    *Ensure function output is equal to the expected result.
    **/

   DOUBLES_EQUAL(expected, result, TEST_PASS_TH_SMALL);
}


/**
 *\purpose  Check if F360_Get_Hypotenuse_Squared returns the correct value when inputs are positive.
 *\req    NA
 */
TEST(f360_math_func, test_f360_get_hypotenuse_squared_positive_input)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float f1 = 3.0f;
   float f2 = 4.0f;
   float expected = 25.0f;
   float result;


   /** \action
    *Call function.
    **/
   result = F360_Get_Hypotenuse_Squared(f1, f2);

   /** \result
    *Ensure function output is equal to the expected result.
    **/

   DOUBLES_EQUAL(expected, result, TEST_PASS_TH_SMALL);
}


/**
 *\purpose  Check if F360_Get_Hypotenuse_Squared returns the correct value when inputs are zero.
 *\req    NA
 */
TEST(f360_math_func, test_f360_get_hypotenuse_squared_zero_input)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float f1 = 0.0f;
   float f2 = 0.0f;
   float expected = 0.0f;
   float result;


   /** \action
    *Call function.
    **/
   result = F360_Get_Hypotenuse_Squared(f1, f2);

   /** \result
    *Ensure function output is equal to the expected result.
    **/

   DOUBLES_EQUAL(expected, result, TEST_PASS_TH_SMALL);
}





/**
 *\purpose  Check if function returns the correct matrix.
 *\req    NA
 */
TEST(f360_math_func, F360_Matmul_MxN_NxP)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float M1[2][3] = {{3.44470f,38.15590f, 79.52000f}, {43.87450f, 76.55170f,18.68730f}};
   float M2[3][2] = {{48.97650f, 44.55870f}, {64.63140f, 70.93650f}, {75.46870f, 27.60260f}};
   float expected1[2][2] = {{8636.04960881f, 5055.09610624f}, {8506.76923014f, 7901.11841718f}};
   float result1[2][2];


   /** \action
    *Call function.
    **/
   F360_Matmul_MxN_NxP(M1, M2, result1);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   for (int i = 0; i < 2; i++)
   {
      for (int j = 0; i < 2; i++)
      {
         DOUBLES_EQUAL(expected1[i][j], result1[i][j], TEST_PASS_TH_LARGE);
      }
   }

   /** \step{2}
    *Compare function output to expected output when the maximum matrix sizes
    *are larger than the utilized matrix sizes.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result
    **/
   float M3[3][4] = {{3.44470f, 38.15590f, 79.52000f, 0.0f}, {43.87450f, 76.55170f, 18.68730f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}};
   float M4[4][5] = {{48.97650f, 44.55870f, 0.0f, 0.0f, 0.0f}, {64.63140f, 70.93650f, 0.0f, 0.0f, 0.0f}, {75.46870f, 27.60260f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f}};
   float expected2[2][2] = {{8636.04960881f, 5055.09610624f}, {8506.76923014f, 7901.11841718f}};
   float result2[3][5];


   /** \action
    *Call function.
    **/
   F360_Matmul_MxN_NxP(M3, M4, result2, 2U, 3U, 3U);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   for (int i = 0; i < 2; i++)
   {
      for (int j = 0; i < 2; i++)
      {
         DOUBLES_EQUAL(expected2[i][j], result2[i][j], TEST_PASS_TH_LARGE);
      }
   }
}

/**
 *\purpose  Check if function returns the correct matrix.
 *\req    NA
 */
TEST(f360_math_func, F360_Matmul_MxN_NxP_Transpose)
{
   /** \step{1}
    *Compare function output to expected output using full matrices.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float M1_full[2][3] = {{3.44470f,38.15590f, 79.52000f}, {43.87450f, 76.55170f,18.68730f}};
   float M2_full[2][3] = {{48.97650f, 64.63140f, 75.46870f}, {44.55870f, 70.93650f, 27.60260f}};
   float expected_full[2][2] = {{8636.04960881f, 5055.09610624f}, {8506.76923014f, 7901.11841718f}};
   float result_full[2][2];


   /** \action
    *Call function.
    **/
   F360_Matmul_MxN_PxN_Transpose(M1_full, M2_full, result_full);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   for (int i = 0; i < 2; i++)
   {
      for (int j = 0; i < 2; i++)
      {
         DOUBLES_EQUAL(expected_full[i][j], result_full[i][j], TEST_PASS_TH_LARGE);
      }
   }

   /** \step{2}
    *Compare function output to expected output using partially empty matrices.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float M1_empty[27][66];
   M1_empty[0][0] = 3.44470f;
   M1_empty[0][1] = 38.15590f;
   M1_empty[0][2] = 79.52000f;
   M1_empty[1][0] = 43.87450f;
   M1_empty[1][1] = 76.55170f;
   M1_empty[1][2] = 18.68730f;
   float M2_empty[86][66];
   M2_empty[0][0] = 48.97650f;
   M2_empty[0][1] = 64.63140f;
   M2_empty[0][2] = 75.46870f;
   M2_empty[1][0] = 44.55870f;
   M2_empty[1][1] = 70.93650f;
   M2_empty[1][2] = 27.60260f;
   float expected_empty[24][43];
   expected_empty[0][0] = 8636.04960881f;
   expected_empty[0][1] = 5055.09610624f;
   expected_empty[1][0] = 8506.76923014f;
   expected_empty[1][1] = 7901.11841718f;
   float result_empty[27][86];


   /** \action
    *Call function.
    **/
   F360_Matmul_MxN_PxN_Transpose(M1_empty, M2_empty, result_empty,
         2U, 3U, 3U);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   for (int i = 0; i < 2; i++)
   {
      for (int j = 0; i < 2; i++)
      {
         DOUBLES_EQUAL(expected_empty[i][j], result_empty[i][j], TEST_PASS_TH_LARGE);
      }
   }
}

/**
 *\purpose  Check if function returns the correct matrix.
 *\req    NA
 */
TEST(f360_math_func, F360_Matmul_MxN_NxP_Transpose_symmetric_matrix)
{
   /** \step{1}
    *Compare function output to expected output using full matrices.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float32_t M1_full[3][2] = {{13.847135000000002F, -27.226219000000000F}, {-17.474364000000001F, 6.696682000000000F}, {15.191402000000000F, -21.150673000000001F}};
   float32_t M2_full[3][2] = {{2.124000000000000F, -3.575000000000000F}, {-4.834000000000000F, -0.216000000000000F}, {3.009000000000000F, -2.432000000000000F}};
   float32_t expected_full[3][3] = {{126.7450476650000F, -61.0561872860000F, 107.8801938230000F}, {-61.0561872860000F, 83.0245922640000F, -68.8666919000000F}, {107.8801938230000F, -68.8666919000000F, 97.1493653540000F}};
   float32_t result_full[3][3];


   /** \action
    *Call function.
    **/
   F360_Matmul_MxN_PxN_Transpose(M1_full, M2_full, result_full);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   for (int32_t i = 0; i < 3; i++)
   {
      for (int32_t j = 0; i < 3; i++)
      {
         DOUBLES_EQUAL(expected_full[i][j], result_full[i][j], TEST_PASS_TH_LARGE);
      }
   }

   /** \step{2}
    *Compare function output to expected output using partially empty matrices.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float32_t M1_empty[27][66];
   M1_empty[0][0] = 13.847135000000002F;
   M1_empty[0][1] = -27.226219000000000F;
   M1_empty[1][0] = -17.474364000000001F;
   M1_empty[1][1] = 6.696682000000000F;
   M1_empty[2][0] = 15.191402000000000F;
   M1_empty[2][1] = -21.150673000000001F;
   
   float32_t M2_empty[86][66];
   M2_empty[0][0] = 2.124000000000000F;
   M2_empty[0][1] = -3.575000000000000F;
   M2_empty[1][0] = -4.834000000000000F;
   M2_empty[1][1] = -0.216000000000000F;
   M2_empty[2][0] = 3.009000000000000F;
   M2_empty[2][1] = -2.432000000000000F;
   
   float32_t expected_empty[24][43];
   expected_empty[0][0] = 126.7450476650000F;
   expected_empty[0][1] = -61.0561872860000F;
   expected_empty[0][2] = 107.8801938230000F;
   expected_empty[1][0] = -61.0561872860000F;
   expected_empty[1][1] = 83.0245922640000F;
   expected_empty[1][2] = -68.8666919000000F;
   expected_empty[2][0] = 107.8801938230000F;
   expected_empty[2][1] = -68.8666919000000F;
   expected_empty[2][2] = 97.1493653540000F;
   float32_t result_empty[27][86];


   /** \action
    *Call function.
    **/
   F360_Matmul_MxN_PxN_Transpose(M1_empty, M2_empty, result_empty, 3U, 2U, 3U);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   for (int32_t i = 0; i < 3; i++)
   {
      for (int32_t j = 0; i < 3; i++)
      {
         DOUBLES_EQUAL(expected_empty[i][j], result_empty[i][j], TEST_PASS_TH_LARGE);
      }
   }
}

/**
 *\purpose  Check if function returns the correct value.
 *\req    NA
 */
TEST(f360_math_func, F360_Saturate)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float max = 10.0f;
   float min = -10.0f;
   float result1;
   float result2;
   float result3;

   /** \action
    *Call function.
    **/
   result1 = F360_Saturate(1.0f, min, max);
   result2 = F360_Saturate(11.0f, min, max);
   result3 = F360_Saturate(-11.0f, min, max);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL(1.0f, result1, TEST_PASS_TH_SMALL);
   DOUBLES_EQUAL(max, result2, TEST_PASS_TH_SMALL);
   DOUBLES_EQUAL(min, result3, TEST_PASS_TH_SMALL);
}

/**
 *\purpose  Check that function is not sorting and return false when asked to sort more values than elements in array.
 *\req    NA
 */
TEST(f360_math_func, F360_Sort_fail)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on. Expected values are identical as no change is expected.
    **/
   const unsigned int length = 5;
   float v[length] = {1.0f, 4.0f, 3.0f, 5.0f, 2.0f};
   unsigned int idx_arr[length] = {1U, 2U, 3U, 4U, 5U};

   float v_expected[length] = {1.0f, 4.0f, 3.0f, 5.0f, 2.0f};
   unsigned int idx_arr_expected[length] = {1U, 2U, 3U, 4U, 5U};

   /** \action
    *Call function.
    **/
   bool sorting_success = F360_Sort(length+1, true, v, idx_arr);

   /** \result
    *Ensure function output is equal to the expected result.
    *No sorting of input value array and index array and return flag from sorting should be false.
    **/
   CHECK_FALSE(sorting_success);
   for (unsigned int i=0; i<length; i++)
   {
      CHECK_EQUAL(v[i], v_expected[i]);
      CHECK_EQUAL(idx_arr[i], idx_arr_expected[i]);
   }
}

/**
 *\purpose  Check that function returns true when sorting.
 *\req    NA
 */
TEST(f360_math_func, F360_Sort_success)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on.
    **/
   const unsigned int length = 5;
   float v[length] = {1.0f, 4.0f, 3.0f, 5.0f, 2.0f};
   unsigned int idx_arr[length];

   /** \action
    *Call function.
    **/
   bool sorting_success = F360_Sort(length, true, v, idx_arr);

   /** \result
    * When sorting is done, sorting_success should return true.
    **/
   CHECK_TRUE(sorting_success);
}


/**
 *\purpose  Check if function returns the correct value.
 *\req    NA
 */
TEST(f360_math_func, F360_Sort_asc)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   const unsigned int length = 5;
   float v[length] = {1.0f, 4.0f, 3.0f, 5.0f, 2.0f};
   unsigned int idx_arr[length];
   unsigned int idx_arr_asc[length] = {0U, 4U, 2U, 1U, 3U};

   /** \action
    *Call function.
    **/
   bool sorting_success = F360_Sort(length, true, v, idx_arr);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   CHECK_TRUE(sorting_success);
   for (unsigned int i=0; i<length-1; i++)
   {
      CHECK_TRUE(v[i+1] > v[i]);
      CHECK_EQUAL(idx_arr_asc[i], idx_arr[i]);
   }
   CHECK_EQUAL(idx_arr_asc[length-1], idx_arr[length-1]);
}

/**
 *\purpose  Check that sorting function just sorts, but does not modify sorted values.
 *\req    NA
 */
TEST(f360_math_func, F360_Sort_sorted_values_not_modified)
{
   /** \precond
    *Set up array for sorting, and it's backup. Prepare expected permutation indexes after sorting.
    **/
   const unsigned int length = 5;
   float v_original[length] = {1.11111f, 4.44444f, 3.33333f, 5.55555f, 2.22222f};
   float v[length]; // Copy of v_original that will be sorted.
   for (unsigned int i=0; i<length; i++)
      {
         v[i] = v_original[i];
      }
   unsigned int idx_arr[length];

   /** \action
    *Call function.
    **/
   bool sorting_success = F360_Sort(length, true, v, idx_arr);

   /** \result
    *Ensure function output is equal to the expected result.
    *Permutation saved in idx_arr should map to original unsorted array. Floats must be equal without any tolerance, it is copy!
    **/
   CHECK_TRUE(sorting_success);
   for (unsigned int i=0; i<length; i++)
   {
      CHECK_EQUAL(v_original[idx_arr[i]], v[i]);
   }
}

/**
 *\purpose  Check that sorting function can sort only specified portion of input. (Leaving remainder of input unsorted.)
 *\req    NA
 */
TEST(f360_math_func, F360_Sort_partial)
{
   /** \precond
    *Set up array for sorting, and it's backup. Prepare expected permutation indexes after sorting.
    **/
   const unsigned int length = 5;
   float v_original[length] = {1.0f, 4.0f, 3.0f, 5.0f, 2.0f};
   float v[length]; // Copy of v_original that will be sorted.
   for (unsigned int i=0; i<length; i++)
      {
         v[i] = v_original[i];
      }
   unsigned int idx_arr[length] = {0U, 1U, 2U, 3U, 4U}; // If only portion of input is sorted, then it is good to define whole array. Not defining it will lead to undefined values in not sorted indexes.
   unsigned int do_not_sort_last_n_values = 2;
   unsigned int idx_arr_asc[length] = {0U, 2U, 1U, 3U, 4U}; // first 3 elements sorted, last 2 elements keep their original position.

   /** \action
    *Call function.
    **/
   bool sorting_success = F360_Sort(length - do_not_sort_last_n_values, true, v, idx_arr);

   /** \result
    *Ensure function output is equal to the expected result.
    *Compare sorted(first 3) and also unsorted(last 2) indexes of inputs. All should match to expected idx_arr_asc. Observe that last two values remain unsorted.
    *Only po
    **/
   CHECK_TRUE(sorting_success);
   for (unsigned int i=0; i<length; i++)
   {
      CHECK_EQUAL(idx_arr[i], idx_arr_asc[i]);
   }
}

/**
 *\purpose  Check that sorting function can sort only specified portion of input. (Leaving remainder of input unsorted.)
 *\req    NA
 */
TEST(f360_math_func, F360_Sort_partial_n_elements_equal)
{
   /** \precond
    *Set up array for sorting, and it's backup. Prepare expected permutation indexes after sorting.
    **/
   const unsigned int length = 5;
   float v_original[length] = {1.0f, 4.0f, 3.0f, 5.0f, 2.0f};
   float v[length]; // Copy of v_original that will be sorted.
   for (unsigned int i=0; i<length; i++)
      {
         v[i] = v_original[i];
      }
   unsigned int idx_arr[length] = {1U, 4U, 3U, 5U, 2U}; // Array is not supposed to be sorted.
   unsigned int do_not_sort_last_n_values = 5;
   unsigned int idx_arr_asc[length] = {1U, 4U, 3U, 5U, 2U}; // No elements are sorted.

   /** \action
    *Call function.
    **/
   bool sorting_success = F360_Sort(length - do_not_sort_last_n_values, true, v, idx_arr);

   /** \result
    *Ensure function output is equal to the expected result.
    *Observe that all values shall remain unsorted.
    **/
   CHECK_TRUE(sorting_success);
   for (unsigned int i=0; i<length; i++)
   {
      CHECK_EQUAL(idx_arr[i], idx_arr_asc[i]);
   }
}

/**
 *\purpose  Check that sorting function will do no sorting when num_elements is zero.
 *\req    NA
 */
TEST(f360_math_func, F360_Sort_no_sort)
{
   /** \precond
    *Set up array for sorting, and it's backup. Prepare expected permutation indexes after sorting.
    **/
   const unsigned int length = 5;
   float v_original[length] = {1.0f, 4.0f, 3.0f, 5.0f, 2.0f};
   float v[length]; // Copy of v_original that will be sorted.
   for (unsigned int i=0; i<length; i++)
      {
         v[i] = v_original[i];
      }
   unsigned int idx_arr[length] = {0U, 1U, 2U, 3U, 4U};
   unsigned int idx_arr_asc[length] = {0U, 1U, 2U, 3U, 4U}; // Same as original. (No sorting expected.)

   /** \action
    *Call function.
    **/
   bool sorting_success = F360_Sort(0, true, v, idx_arr);


   /** \result
    *Ensure function output is equal to the expected result.
    *Compare all indexes of input and its values. It should be same before and after the call to sorting function.
    *Sorting function should return true.
    **/
   CHECK_TRUE(sorting_success);
   for (unsigned int i=0; i<length; i++)
   {
      CHECK_EQUAL(idx_arr[i], idx_arr_asc[i]); // Unchanged index.
      CHECK_EQUAL(v_original[idx_arr[i]], v[i]); // Unchanged(not sorted) input.
   }
}


/**
 *\purpose  Check that sorting is stable (preserving order of elements of same value).
 *\req    NA
 */
TEST(f360_math_func, F360_Sort_stability)
{
   /** \precond
    *Set up array for sorting, and it's backup. Prepare expected permutation indexes after sorting.
    **/
   const unsigned int length = 6;
   float v_original[length] = {1.0f, 2.0f, 1.0f, 2.0f, 1.0f, 2.0f};
   float v[length]; // Copy of v_original that will be sorted.
   for (unsigned int i=0; i<length; i++)
      {
         v[i] = v_original[i];
      }

   unsigned int idx_arr[length];
   unsigned int idx_arr_stable[length] = {0U, 2U, 4U, 1U, 3U, 5U}; // Same values preserve order in which they were present in original array.

   /** \action
    *Call function.
    **/
   bool sorting_success = F360_Sort(length, true, v, idx_arr);

   /** \result
    *Ensure function output is equal to the expected result.
    *Compare indexes of sorted array to expected indexes.
    **/
   CHECK_TRUE(sorting_success);
   for (unsigned int i=0; i<length; i++)
   {
      CHECK_EQUAL(idx_arr[i], idx_arr_stable[i]); // Stable sorting.
      CHECK_EQUAL(v_original[idx_arr[i]], v[i]); // Actual values are sorted.
   }
}

/**
 *\purpose  Check if function returns the correct value.
 *\req    NA
 */
TEST(f360_math_func, F360_Sort_dsc)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   const unsigned int length = 5;
   float v[length] = {1.0f, 4.0f, 3.0f, 5.0f, 2.0f};
   unsigned int idx_arr[length];
   unsigned int idx_arr_dsc[length] = {3U, 1U, 2U, 4U, 0U};

   /** \action
    *Call function.
    **/

   bool sorting_success = F360_Sort(length, false, v, idx_arr);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   CHECK_TRUE(sorting_success);
   for (unsigned int i=0; i<length-1; i++)
   {
      CHECK_TRUE(v[i+1] < v[i]);
      CHECK_EQUAL(idx_arr_dsc[i], idx_arr[i]);
   }
   CHECK_EQUAL(idx_arr_dsc[length-1], idx_arr[length-1]);
}

/**
 *\purpose  Verify several input paths for the first branching condition in the target method
 *\req    NA
 */
TEST(f360_math_func, f360_merge_sublists_no_entrance)
{

   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    * Set up several indexes and two lists populated with some data.
    * The indexes are set in such a way as to avoid any sorting activity.
    * The first "if" statement will fail leading to an quick exit from the function.
    **/
   const uint32_t N = SORT_ARRAY_SIZE;

   uint32_t sublist1_start = 3;
   uint32_t sublist2_start = 2;
   uint32_t merged_list_end = 1;

   F360_Sort_Data_T in_list[N] = {
      {0.424052358 , 1 },
      {0.481313974 , 0 },
      {-0.173000470, 2 },
   };

   F360_Sort_Data_T out_list[N] = {
      {0.424052358 , 1 },
      {0.481313974 , 0 },
      {-0.173000470, 2 },
   };

   /** \action
    *Call function.
    **/
   merge_sublists(false, sublist1_start, sublist2_start, merged_list_end, in_list, out_list);

   for (uint8_t i = 0; i < merged_list_end; i++)
   {
      CHECK_TRUE(in_list[i].data == out_list[i].data);
   }

   sublist1_start = 2;
   sublist2_start = 3;
   merged_list_end = 1;

   merge_sublists(false, sublist1_start, sublist2_start, merged_list_end, in_list, out_list);

   for (uint8_t i = 0; i < merged_list_end; i++)
   {
      CHECK_TRUE(in_list[i].data == out_list[i].data);
   }

   sublist1_start = 2;
   sublist2_start = 1;
   merged_list_end = 2;

   merge_sublists(false, sublist1_start, sublist2_start, merged_list_end, in_list, out_list);

   for (uint8_t i = 0; i < merged_list_end; i++)
   {
      CHECK_TRUE(in_list[i].data == out_list[i].data);
   }

   /** \result
    * There should be no change between the two array elements.
    **/
}

/**
 *\purpose Iterate over the first index and satify the condition to exit.
 *\req    NA
 */
TEST(f360_math_func, f360_merge_sublists_place_first_and_break)
{

   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    * Set up two input sublists and their indexes. Check the expected output.
    **/
   const uint32_t N = SORT_ARRAY_SIZE;

   const uint32_t sublist1_start = 0;
   const uint32_t sublist2_start = 2;
   const uint32_t merged_list_end = 3;

   F360_Sort_Data_T in_list[N] = {
      {0.424052358 , 1 },
      {0.481313974 , 0 },
      {-0.173000470, 2 },
   };

   F360_Sort_Data_T out_list[N];

   F360_Sort_Data_T expected[N] = {
      {-0.173000470, 2 },
      {0.424052358 , 1 },
      {0.481313974 , 0 },
   };

   /** \action
    *Call function.
    **/
   merge_sublists(true, sublist1_start, sublist2_start, merged_list_end, in_list, out_list);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   for (uint8_t i = 0; i < merged_list_end; i++)
   {
      CHECK_TRUE(out_list[i].data == expected[i].data);
   }
}

/**
 *\purpose  Iterate over the second index and satify the condition to exit
 *\req    NA
 */
TEST(f360_math_func, f360_merge_sublists_place_second_and_break)
{

   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    * Set up two input sublists and their indexes. Check the expected output.
    **/
   const uint32_t N = SORT_ARRAY_SIZE;

   const uint32_t sublist1_start = 0;
   const uint32_t sublist2_start = 2;
   const uint32_t merged_list_end = 3;

   F360_Sort_Data_T in_list[N] = {
      {0.424052358 , 1 },
      {0.481313974 , 0 },
      {-0.173000470, 2 },
   };

   F360_Sort_Data_T out_list[N];

   F360_Sort_Data_T expected[N] = {
      {-0.173000470, 2 },
      {0.424052358 , 1 },
      {0.481313974 , 0 },
   };

   /** \action
    *Call function.
    **/
   merge_sublists(true, sublist1_start, sublist2_start, merged_list_end, in_list, out_list);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   for (uint8_t i = 0; i < merged_list_end; i++)
   {
      CHECK_TRUE(out_list[i].data == expected[i].data);
   }
}

/**
 *\purpose  Check if function returns the correct value.
 *\req    NA
 */
TEST(f360_math_func, f360_merge_sublists_both_idx_into_out_list)
{

   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   const uint32_t N = SORT_ARRAY_SIZE;

   const uint32_t sublist1_start = 0;
   const uint32_t sublist2_start = 4;
   const uint32_t merged_list_end = 6;

   F360_Sort_Data_T in_list[N] = {
      {-0.340802401, 1},
      {-0.189768866, 0},
      {-0.183838397, 2},
      {-0.183838397, 3},
      {-1.68957269 , 5},
      {0.683543921 , 4},
   };

   F360_Sort_Data_T out_list[N];

   F360_Sort_Data_T expected[N] = {
      {-1.68957269 , 5 },
      {-0.340802401, 1 },
      {-0.189768866, 0 },
      {-0.183838397, 2 },
      {-0.183838397, 3 },
      {0.683543921 , 4 },
   };

   /** \action
    *Call function.
    **/
   merge_sublists(true, sublist1_start, sublist2_start, merged_list_end, in_list, out_list);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   for (uint8_t i = 0; i < merged_list_end; i++)
   {
      CHECK_TRUE(out_list[i].data == expected[i].data);
   }
}

/**
 *\purpose  Check if function returns the correct value.
 *\req    NA
 */
TEST(f360_math_func, f360_merge_sublists_second_is_empty)
{

   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    * Verify the path when the second sublist is empty so only the rest of the first one goes to the resulting array.
    **/
   const uint32_t N = SORT_ARRAY_SIZE;

   const uint32_t sublist1_start = 0;
   const uint32_t sublist2_start = 2;
   const uint32_t merged_list_end = 4; // lower

   F360_Sort_Data_T in_list[N] = {
      {10.0153551, 0},
      {11.1380959, 1},
      {29.0131416, 2},
      {39.5254936, 3},
      {59.8255653, 4},
      {79.5754089, 5},
      {97.9430008, 6},
      {112.824142, 7},
      {138.705872, 8},
      {151.348450, 9},
   };

   F360_Sort_Data_T out_list[N];

   /** \action
    *Call function.
    **/
   merge_sublists(true, sublist1_start, sublist2_start, merged_list_end, in_list, out_list);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   for (uint8_t i = 0; i < merged_list_end; i++)
   {
      CHECK_TRUE(in_list[i].data == out_list[i].data);
   }
}

/**
 *\purpose  Check if function returns the correct value.
 *\req    NA
 */
TEST(f360_math_func, f360_merge_sublists_first_is_empty)
{

   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    * Verify the path when the first sublist is empty so only the rest of the second one goes to the resulting array.
    **/

   const uint32_t N = SORT_ARRAY_SIZE;

   F360_Sort_Data_T in_list[N] = {
      {10.0153551, 0},
      {11.1380959, 1},
      {29.0131416, 2},
      {39.5254936, 3},
      {59.8255653, 4},
      {79.5754089, 5},
      {97.9430008, 6},
      {112.824142, 7},
      {138.705872, 8},
      {151.348450, 9},
   };

   F360_Sort_Data_T out_list[N] = {
      {10.0153551, 0},
      {11.1380959, 1},
      {29.0131416, 2},
      {39.5254936, 3},
      {59.8255653, 4},
      {79.5754089, 5},
      {97.9430008, 6},
      {112.824142, 7},
   };
   
   const uint32_t sublist1_start = 8;
   const uint32_t sublist2_start = 10;
   const uint32_t merged_list_end = 10; // upper

   /** \action
    *Call function.
    **/
   merge_sublists(false, sublist1_start, sublist2_start, merged_list_end, in_list, out_list);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   for (uint8_t i = 0; i < merged_list_end; i++)
   {
      CHECK_TRUE(in_list[i].data == out_list[i].data);
   }

}

/**
 *\purpose  Check f360_mergesort if function returns the correct value after sorting in a descending order.
 *\req    NA
 */
TEST(f360_math_func, f360_mergesort_dsc)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    * Verify the correctness of the descending order sort
    **/
   const unsigned int length = 5;
   float v[length] = {1.0f, 4.0f, 3.0f, 5.0f, 2.0f};
   unsigned int idx_arr_dsc[length] = {3U, 1U, 2U, 4U, 0U};

   F360_Sort_Data_T work_data[SORT_ARRAY_SIZE];
   F360_Sort_Data_T work_buffer[SORT_ARRAY_SIZE];
   bool f_data_in_buffer = false;
   for (uint32_t i = 0U; i < length; i++)
   {
      work_data[i].index = i;
      work_data[i].data = v[i];
   }

   /** \action
    *Call function.
    **/

   f360_mergesort(length, false, work_data, work_buffer, f_data_in_buffer);


   /** \result
    *Ensure function output is equal to the expected result.
    **/
   CHECK_FALSE(f_data_in_buffer);
   for (unsigned int i=0; i<length-1; i++)
   {
      CHECK_TRUE(work_data[i+1].data < work_data[i].data);
      CHECK_EQUAL(idx_arr_dsc[i], work_data[i].index);
   }
   CHECK_EQUAL(idx_arr_dsc[length-1], work_data[length-1].index);
}

/**
 *\purpose  Check if function returns the correct value.
 *\req    NA
 */
TEST(f360_math_func, f360_mergesort_asc)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    * Verify the correctness of the ascending order sort
    **/
   const unsigned int length = 5;
   float v[length] = {1.0f, 4.0f, 3.0f, 5.0f, 2.0f};
   unsigned int idx_arr_asc[length] = {0U, 4U, 2U, 1U, 3U};

   
   F360_Sort_Data_T work_data[SORT_ARRAY_SIZE];
   F360_Sort_Data_T work_buffer[SORT_ARRAY_SIZE];
   bool f_data_in_buffer = false;
   for (uint32_t i = 0U; i < length; i++)
   {
      work_data[i].index = i;
      work_data[i].data = v[i];
   }

   /** \action
    *Call function.
    **/
   f360_mergesort(length, true, work_data, work_buffer, f_data_in_buffer);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   CHECK_FALSE(f_data_in_buffer);
   for (unsigned int i=0; i<length-1; i++)
   {
      CHECK_TRUE(work_data[i+1].data > work_data[i].data);
      CHECK_EQUAL(idx_arr_asc[i], work_data[i].index);
   }
   CHECK_EQUAL(idx_arr_asc[length-1], work_data[length-1].index);
}

/**
 *\purpose  Check if function returns the correct value.
 *\req    NA
 */
TEST(f360_math_func, f360_mergesort_nothing_to_sort)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    * Verify the case when the list is already sorted
    **/
   const unsigned int length = 5;
   float v[length] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
   unsigned int idx_arr_asc[length] = {0U, 1U, 2U, 3U, 4U};

   
   F360_Sort_Data_T work_data[SORT_ARRAY_SIZE];
   F360_Sort_Data_T work_buffer[SORT_ARRAY_SIZE];
   bool f_data_in_buffer = false;
   for (uint32_t i = 0U; i < length; i++)
   {
      work_data[i].index = i;
      work_data[i].data = v[i];
   }

   /** \action
    *Call function.
    **/
   f360_mergesort(length, true, work_data, work_buffer, f_data_in_buffer);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   CHECK_FALSE(f_data_in_buffer);
   for (unsigned int i=0; i<length-1; i++)
   {
      CHECK_TRUE(work_data[i+1].data > work_data[i].data);
      CHECK_EQUAL(idx_arr_asc[i], work_data[i].index);
   }
   CHECK_EQUAL(idx_arr_asc[length-1], work_data[length-1].index);
}


/**
 *\purpose  Check if function returns the correct value.
 *\req    NA
 */
TEST(f360_math_func, f360_mergesort_force_memcopy)
{

   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    * Verify the path where the merged list is currently in the buffer
    **/
   const uint32_t N = SORT_ARRAY_SIZE;

   const uint32_t count = 3;

   F360_Sort_Data_T array_to_sort[N] = {
      {0.486106634, 0 },
      {0.293803453, 1 },
      {0.293803453, 2 },
   };

   F360_Sort_Data_T expected_array[N] = {
      {0.293803453, 1 },
      {0.293803453, 2 },
      {0.486106634, 0 },
   };

   F360_Sort_Data_T work_buffer[N];
   bool f_data_in_buffer = false;
   /** \action
    *Call function.
    **/
   f360_mergesort(count, true, array_to_sort, work_buffer, f_data_in_buffer);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   CHECK_TRUE(f_data_in_buffer);
   for (uint8_t i = 0; i < count; i++)
   {
      CHECK_TRUE(work_buffer[i].data == expected_array[i].data);
   }
}


/**
 *\purpose  Check if function returns the correct value.
 *\req    NA
 */
TEST(f360_math_func, f360_mergesort_both_for_in_buffer)
{

   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    * Inside the f360_mergesort call merge_sublists with either array_to_sort or buffer as input list
    **/
   const uint32_t N = SORT_ARRAY_SIZE;

   const uint32_t count = 7;

   F360_Sort_Data_T array_to_sort[N] = {
      {-0.0548408553, 0 },
      {0.0450601429 , 1 },
      {0.0450601429 , 2 },
      {0.00312166661, 3 },
      {0.0203676373 , 4 },
      {0.145050526  , 5 },
      {0.145050526  , 6 },
   };

   F360_Sort_Data_T expected_array_to_sort[N] = {
      {-0.0548408553, 0 },
      {0.00312166661, 3 },
      {0.0203676373 , 4 },
      {0.0450601429 , 1 },
      {0.0450601429 , 2 },
      {0.145050526  , 5 },
      {0.145050526  , 6 },
   };

   F360_Sort_Data_T work_buffer[N];
   bool f_data_in_buffer = false;
   /** \action
    *Call function.
    **/
   f360_mergesort(count, true, array_to_sort, work_buffer, f_data_in_buffer);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   CHECK_FALSE(f_data_in_buffer);
   for (uint8_t i = 0; i < count; i++)
   {
      CHECK_TRUE(array_to_sort[i].data == expected_array_to_sort[i].data);
   }
}

/**
 *\purpose  Check if F360_Max_Float returns the correct value
 *\req    NA
 */
TEST(f360_math_func, F360_Max_Element_Float)
{
   /** \step{1}
    *Check that function returns the largest element in a list
    **/

   /** \precond
    *Set up list to operate on and expected function output
    **/
   float v[5] = {1.0f, 4.0f, 3.0f, -5.0f, 2.0f};
   /** \action
    *Call function.
    **/
    float max_val = F360_Max_Element(v);
   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL(4.0f, max_val, TEST_PASS_TH_SMALL);
}

/**
 *\purpose  Check if function returns the correct value.
 *\req    NA
 */
TEST(f360_math_func, F360_Min_Element_Float)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float v[5] = {1.0f, 4.0f, -3.0f, 5.0f, 2.0f};
   /** \action
    *Call function.
    **/
   float min_val = F360_Min_Element(v);
   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL(-3.0f, min_val, TEST_PASS_TH_SMALL);
}

/**
 *\purpose  Check if function returns the correct value.
 *\req    NA
 */
TEST(f360_math_func, F360_Min_Index_Float)
{
   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float v[5] = {1.0f, 4.0f, -3.0f, 5.0f, 2.0f};
   /** \action
    *Call function.
    **/
   unsigned int min_idx = F360_Min_Index(v);
   /** \result
    *Ensure function output is equal to the expected result.
    **/
   CHECK_EQUAL(2U, min_idx);
}

/**
 *\purpose  Check if function returns the correct value.
 *\req    NA
 */
TEST(f360_math_func, F360_Mean)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float v[5] = {1.0f, 4.0f, 3.0f, 5.0f, 2.0f};
   float result1;

   /** \action
    *Call function.
    **/
   result1 = F360_Mean(v, 5);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL(3.0f, result1, TEST_PASS_TH_SMALL);

   /** \step{2}
    *Compare function output to expected output when zero number of elements.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float result2;

   /** \action
    *Call function.
    **/
   result2 = F360_Mean(v, 0);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(0.0f, result2, TEST_PASS_TH_SMALL, "Unexpected return value when number of elements is zero");
}

/**
 *\purpose  Check if function returns the correct value.
 *\req    NA
 */
TEST(f360_math_func, F360_Any)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   bool b1[5] = {false, false, false, false, false};
   bool b2[5] = {true, false, true, true, true};
   bool result1;
   bool result2;

   /** \action
    *Call function.
    **/
   result1 = F360_Any(b1, 5);
   result2 = F360_Any(b2, 5);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   CHECK_FALSE(result1);
   CHECK_TRUE(result2);
}

/**
 *\purpose  Check if function returns the correct value.
 *\req    NA
 */
TEST(f360_math_func, F360_Matmul_6x6_6x6)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float A[6][6] = {{6.95f, 7.66f, 7.10f, 1.19f, 7.52f, 5.48f},
         {3.18f, 7.96f, 7.55f, 4.99f, 2.56f, 1.39f},
         {9.51f, 1.87f, 2.77f, 9.60f, 5.06f, 1.50f},
         {0.35f, 4.90f, 6.80f, 3.41f, 7.00f, 2.58f},
         {4.39f, 4.46f, 6.56f, 5.86f, 8.91f, 8.41f},
         {3.82f, 6.47f, 1.63f, 2.24f, 9.60f, 2.55f}};

   float B[6][6] = {{8.15f, 6.17f, 9.18f, 0.76f, 5.69f, 3.12f},
         {2.44f, 4.74f, 2.86f, 0.54f, 4.70f, 5.29f},
         {9.30f, 3.52f, 7.58f, 5.31f, 0.12f, 1.66f},
         {3.50f, 8.31f, 7.54f, 7.80f, 3.38f, 6.02f},
         {1.97f, 5.86f, 3.81f, 9.35f, 1.63f, 2.63f},
         {2.52f, 5.50f, 5.68f, 1.30f, 7.95f, 6.55f}};

   float ans[6][6] = {{174.1519f, 188.2780f, 208.2768f, 133.8374f, 136.2453f, 136.8268f},
         {141.5654f, 148.0405f, 164.4604f, 111.4707f,  88.5017f, 110.4401f},
         {155.1785f, 194.9685f, 213.8292f, 147.0871f, 115.8541f, 125.0865f},
         {110.2751f, 132.8686f, 135.8068f, 134.4220f,  69.2843f,  94.1382f},
         {166.9248f, 218.4821f, 228.6809f, 180.5279f, 147.9179f, 161.9758f},
         {95.2568f, 148.8702f, 133.8768f, 125.5993f,  95.8321f, 104.2858f}};

   float result1[6][6];

   /** \action
    *Call function.
    **/
   F360_Matmul_MxN_NxP(A, B, result1);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   for (int i = 0; i < 6; i++)
   {
      for (int j = 0; j < 6; j++)
      {
         DOUBLES_EQUAL(ans[i][j], result1[i][j], TEST_PASS_TH_LARGE);
      }
   }
}

/**
 *\purpose  Check if function returns the correct value.
 *\req    NA
 */
TEST(f360_math_func, F360_Matmul_6x6_6x6T)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float A[6][6] = {{6.95f, 7.66f, 7.10f, 1.19f, 7.52f, 5.48f},
         {3.18f, 7.96f, 7.55f, 4.99f, 2.56f, 1.39f},
         {9.51f, 1.87f, 2.77f, 9.60f, 5.06f, 1.50f},
         {0.35f, 4.90f, 6.80f, 3.41f, 7.00f, 2.58f},
         {4.39f, 4.46f, 6.56f, 5.86f, 8.91f, 8.41f},
         {3.82f, 6.47f, 1.63f, 2.24f, 9.60f, 2.55f}};

   float B[6][6] = {{8.15f, 6.17f, 9.18f, 0.76f, 5.69f, 3.12f},
         {2.44f, 4.74f, 2.86f, 0.54f, 4.70f, 5.29f},
         {9.30f, 3.52f, 7.58f, 5.31f, 0.12f, 1.66f},
         {3.50f, 8.31f, 7.54f, 7.80f, 3.38f, 6.02f},
         {1.97f, 5.86f, 3.81f, 9.35f, 1.63f, 2.63f},
         {2.52f, 5.50f, 5.68f, 1.30f, 7.95f, 6.55f}};

   float ans[6][6] = {{229.8735f, 138.5482f, 161.7343f, 209.2028f, 123.4266f,  197.197},
         {167.0348f,  89.1623f, 143.9337f, 190.1472f, 136.1607f, 130.6211},
         {155.2404f,  76.8914f, 170.0952f, 170.7233f, 142.1994f, 112.5158},
         {145.9807f,  91.9176f,  95.2769f, 159.0056f, 105.3904f,  143.438},
         {204.9082f, 140.1439f, 152.3974f,  228.342f, 151.2101f, 206.3916},
         {150.2987f, 104.4695f,  87.9352f, 144.6969f,  94.9484f, 150.4043}};

   float result1[6][6];

   /** \action
    *Call function.
    **/
   F360_Matmul_MxN_PxN_Transpose(A, B, result1);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   for (int i = 0; i < 6; i++)
   {
      for (int j = 0; j < 6; j++)
      {
         DOUBLES_EQUAL(ans[i][j], result1[i][j], TEST_PASS_TH_LARGE);
      }
   }
}

/**
 *\purpose  Check if function F360_matmul_6x6_6x6T_symmetric_matrix returns the correct value.
 *\req    NA
 */
TEST(f360_math_func, F360_Matmul_6x6_6x6T_symmetric_matrix)
{
   /** \step{1}
    * Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float32_t A[6][6] = {{31.190700000000003F, -50.119399999999999F, -0.604600000000000F, -18.403000000000002F, -5.389299999999999F, -66.703500000000005F},
         {20.446900000000003F, -31.061699999999995F, -31.496600000000004F, -44.449699999999993F, 3.690600000000000F, 14.476500000000000F},
         {2.547899999999999F, 77.991399999999985F, 62.330299999999994F, 53.455999999999996F, -34.788499999999999F, 65.208100000000002F},
         {-65.824300000000008F, -93.077699999999993F, -62.509799999999998F, -10.024399999999998F, 46.656500000000001F, 3.101000000000001F},
         {0.155800000000000F, -47.890599999999992F, -65.720800000000011F, -28.787999999999997F, 26.684400000000000F, -66.808000000000007F},
         {-22.129199999999994F, -76.557099999999977F, -7.567199999999996F, -34.265400000000000F, 14.717500000000003F, 39.958300000000001F}};

   float32_t B[6][6] = {{4.910000000000000F, -7.320000000000000F, -1.170000000000000F, -0.880000000000000F, -4.040000000000000F, -8.390000000000001F},
         {4.720000000000000F, -5.750000000000000F, -9.740000000000000F, -7.970000000000000F, -9.080000000000000F, 5.540000000000000F},
         {1.230000000000000F, 7.890000000000000F, 7.940000000000000F, 9.900000000000000F, 0.100000000000000F, 8.100000000000000F},
         {-6.320000000000000F, -8.580000000000000F, -6.070000000000000F, -3.360000000000000F, 5.220000000000000F, 0.670000000000000F},
         {1.940000000000000F, -5.160000000000000F, -8.140000000000001F, -4.060000000000000F, 2.620000000000000F, -7.820000000000000F},
         {-4.010000000000000F, -8.930000000000000F, -3.860000000000000F, -8.760000000000000F, -8.210000000000001F, 6.510000000000000F}};

   const float32_t expected_A_times_B[6][6] = {{1118.337504000000F, 267.364822000000F, -1084.905009000000F, 225.579739000000F, 906.265090000000F, 096.041939000000F},
         {267.364822000000F, 982.844298000000F, -792.433450000000F, 506.784519000000F, 533.256606000000F, 770.287349000000F},
         {-1084.905009000000F, -792.433450000000F, 2167.309805000000F, -1381.132564000000F, -1722.965912000000F, -705.431483000000F},
         {225.579739000000F, 506.784519000000F, -1381.132564000000F, 1873.357312000000F, 1000.100836000000F, 1061.378521000000F},
         {906.265090000000F, 533.256606000000F, -1722.965912000000F, 1000.100836000000F, 1491.616028000000F, 278.904464000000F},
         {96.041939000000F, 770.287349000000F, -705.431483000000F, 1061.378521000000F, 278.904464000000F, 1241.065149000000F}};;

   /** \action
    *Call function.
    **/
   float32_t result[6][6];
   F360_Matmul_MxN_PxN_Transpose(A, B, result);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   for (int i = 0; i < 6; i++)
   {
      for (int j = 0; j < 6; j++)
      {
         DOUBLES_EQUAL(expected_A_times_B[i][j], result[i][j], TEST_PASS_TH_LARGE);
      }
   }
}

/**
 *\purpose  Check if function returns the correct value.
 *\req    NA
 */
TEST(f360_math_func, F360_Matmul_4x4_4x4T)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float A[4][4] = {{6.95f, 7.66f, 7.10f, 1.19f},
         {3.18f, 7.96f, 7.55f, 4.99f},
         {9.51f, 1.87f, 2.77f, 9.60f},
         {0.35f, 4.90f, 6.80f, 3.41f}};

   float B[4][4] = {{8.15f, 6.17f, 9.18f, 0.76f},
         {2.44f, 4.74f, 2.86f, 0.54f},
         {9.30f, 3.52f, 7.58f, 5.31f},
         {3.50f, 8.31f, 7.54f, 7.80f}};

   float ans[4][4] = {{169.9871f, 74.2150f, 151.7351f, 150.7956f},
         {148.1316f,  69.7772f, 141.3191f, 173.1266f},
         {121.7690f,  45.1744f, 166.9980f, 144.5905f},
         {98.1011f,  45.3694f,  90.1541f, 119.8140f}};

   float result1[4][4];

   /** \action
    *Call function.
    **/
   F360_Matmul_MxN_PxN_Transpose(A, B, result1);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   for (int i = 0; i < 4; i++)
   {
      for (int j = 0; j < 4; j++)
      {
         DOUBLES_EQUAL(ans[i][j], result1[i][j], TEST_PASS_TH_LARGE);
      }
   }
}

/**
 *\purpose  Check if function F360_matmul_4x4_4x4T_symmetric_matrix returns the correct value.
 *\req    NA
 */
TEST(f360_math_func, F360_Matmul_4x4_4x4T_symmetric_matrix)
{
   /** \step{1}
    * Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
  
   float32_t A[4][4] = {{1.249799999999999F, 1.445700000000000F, -1.761300000000000F, 19.219300000000000F},
         {-3.024600000000000F, 2.384100000000000F, -1.034700000000000F, -7.688100000000000F},
         {-1.704000000000000F, 3.806400000000000F, -7.364399999999999F, 3.750800000000000F},
         {-2.724200000000000F, 4.307700000000000F, -7.962200000000000F, 19.957100000000001F}};

   float32_t B[4][4] = {{4.710000000000000F, 1.860000000000000F, -1.270000000000000F, 4.790000000000000F},
         {-4.440000000000000F, 2.190000000000000F, 0.810000000000000F, -2.160000000000000F},
         {-0.500000000000000F, 1.500000000000000F, -3.840000000000000F, 0.940000000000000F},
         {0.820000000000000F, 2.260000000000000F, -4.430000000000000F, 4.620000000000000F}};
   
   const float32_t expected_A_times_B[4][4] = {{102.8728580000000F, -45.3233700000000F, 26.3731840000000F, 100.8878430000000F},
         {-45.3233700000000F, 34.4185920000000F, 1.8348840000000F, -28.0274070000000F},
         {26.3731840000000F, 1.8348840000000F, 38.3666480000000F, 57.1581720000000F},
         {100.8878430000000F, -28.0274070000000F, 57.1581720000000F, 134.9759060000000F}};

   /** \action
    *Call function.
    **/
   float32_t result[4][4];
   F360_Matmul_MxN_PxN_Transpose(A, B, result);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   for (int i = 0; i < 4; i++)
   {
      for (int j = 0; j < 4; j++)
      {
         DOUBLES_EQUAL(expected_A_times_B[i][j], result[i][j], TEST_PASS_TH_LARGE);
      }
   }
}

/**
 *\purpose  Check if function F360_matadd_6x6() returns the correct matrix.
 *\req    NA
 */
TEST(f360_math_func, F360_matadd_6x6)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on.
    **/
   float M1[6][6];
   float M2[6][6];
   float result[6][6];

   for (int i = 0; i < 6; i++)
   {
      for(int j = 0; j < 6; j++)
      {
         M1[i][j] = (float)i*j*3.14f + 2.0f;
         M2[i][j] = (float)i*j*2.72f + 1.0f;
      }
   }

   /** \action
    *Call function.
    **/
   F360_Matadd_6x6_6x6(M1, M2, result);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   for (int i = 0; i < 6; i++)
   {
      for(int j = 0; j < 6; j++)
      {
         DOUBLES_EQUAL_TEXT(M1[i][j] + M2[i][j], result[i][j], TEST_PASS_TH_MID, "Matrix addition for 6 by 6 matrix returns unexpected result.");
      }
   }
}

/**
 *\purpose  Check if function F360_matadd_6x6_symmetric_matrix() returns the correct matrix.
 *\req    NA
 */
TEST(f360_math_func, F360_matadd_6x6_symmetric_matrix)
{

   /** \precond
    * Set up float values to operate on.
    **/
   const float32_t M1[6][6] = {{5.7400F, 1.8700F, 2.3600F, -2.7200F, -1.4200F, -1.3500F},
         {1.8700F, 8.4500F, -0.1300F, 0.8000F, -1.3500F, 0.2800F},
         {2.3600F, -0.1300F, 7.3800F, -0.9100F, -1.5100F, 1.3600F},
         {-2.7200F, 0.8000F, -0.9100F, 5.8500F, -1.1900F, -0.0100F},
         {-1.4200F, -1.3500, -1.5100F, -1.1900F, 2.4600F, 0.1400F},
         {-1.3500F, 0.2800F, 1.3600F, -0.0100F, 0.1400F, 6.6600F}};
   const float32_t M2[6][6] = {{7.4000F, -1.2300F, -2.0000F, -1.7500F, -3.1100F, 0.4900F},
         {-1.2300F, 2.3400F, 0.5800F, -1.2600F, 1.2800F, 0.3600F},
         {-2.0000F, 0.5800F, 7.3400F, 0.2600F, -1.6700F, -0.1700F},
         {-1.7500F, -1.2600F, 0.2600F, 9.7000F, 0.9400F, 0.4400F},
         {-3.1100F, 1.2800F, -1.6700F, 0.9400F, 8.6600F, -1.0700F},
         {0.4900F, 0.3600F, -0.1700F, 0.4400F, -1.0700F, 0.8600F}};

   const float32_t expected_M1_plus_M2[6][6] = {{13.1400F, 0.6400F, 0.3600F, -4.4700F, -4.5300F, -0.8600F},
         {0.6400F, 10.7900F, 0.4500F, -0.4600F, -0.0700F, 0.6400F},
         {0.3600F, 0.4500F, 14.7200F, -0.6500F, -3.1800F, 1.1900F},
         {-4.4700F, -0.4600F, -0.6500F, 15.5500F, -0.2500F, 0.4300F},
         {-4.5300F, -0.0700F, -3.1800F, -0.2500F, 11.1200F, -0.9300F},
         {-0.8600F, 0.6400F, 1.1900F, 0.4300F, -0.9300F, 7.5200F}};

   /** \action
    *Call function.
    **/
   float32_t result[6][6];
   
   F360_Matadd_6x6_6x6(M1, M2, result);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   for (int i = 0; i < 6; i++)
   {
      for(int j = 0; j < 6; j++)
      {
         DOUBLES_EQUAL_TEXT(expected_M1_plus_M2[i][j] , result[i][j], TEST_PASS_TH_MID, "Matrix addition for 6 by 6 matrix returns unexpected result.");
      }
   }
}


/**
 *\purpose  Check if function returns the correct value.
 *\req    NA
 */
TEST(f360_math_func, F360_Transpose_2D)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float A[6][6] = {{6.95f, 7.66f, 7.10f, 1.19f, 7.52f, 5.48f},
         {3.18f, 7.96f, 7.55f, 4.99f, 2.56f, 1.39f},
         {9.51f, 1.87f, 2.77f, 9.60f, 5.06f, 1.50f},
         {0.35f, 4.90f, 6.80f, 3.41f, 7.00f, 2.58f},
         {4.39f, 4.46f, 6.56f, 5.86f, 8.91f, 8.41f},
         {3.82f, 6.47f, 1.63f, 2.24f, 9.60f, 2.55f}};

   float result1[6][6];
   float result2[6][6];

   /** \action
    *Call function.
    **/
   F360_Transpose_2D(A, result1);
   F360_Transpose_2D(A, result2);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   for (int i = 0; i < 6; i++)
   {
      for (int j = 0; j < 6; j++)
      {
         DOUBLES_EQUAL(A[i][j], result1[j][i], TEST_PASS_TH_SMALL);
         DOUBLES_EQUAL(A[i][j], result2[j][i], TEST_PASS_TH_SMALL);
      }
   }
}

/**
 *\purpose  Check if function returns the correct value.
 *\req    NA
 */
TEST(f360_math_func, F360_Linear_Equation)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float x1 = 1.0f;
   float x2 = 2.0f;
   float y1 = 1.0f;
   float y2 = 1.5f;

   float x = 3.0f;
   float out1, out2;

   /** \action
    *Call function.
    **/
   out1 = F360_Linear_Equation(x, x1, x2, y1, y2);
   out2 = F360_Linear_Equation(x, x1, x2, y2, y1);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL(2.0f, out1, TEST_PASS_TH_SMALL);
   DOUBLES_EQUAL(0.5f, out2, TEST_PASS_TH_SMALL);

   /** \step{2}
    *Verify result for lines when slope is zero.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/

   /** \action
    *Call function.
    **/
   out1 = F360_Linear_Equation(x, x1, x2, y1, y1);
   out2 = F360_Linear_Equation(x, x2, x1, y1, y1);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL(1.0f, out1, TEST_PASS_TH_SMALL);
   DOUBLES_EQUAL(1.0f, out1, TEST_PASS_TH_SMALL);
}
/**
*\purpose  Check if function behaves correctly for small denominators.
*\req    NA
*/
TEST(f360_math_func, F360_Linear_Equation_small_den)
{
   /** \step{1}
   *Test behavior of zero and close to zero denominator
   *else - first if ((x < x1) && (x1 < x2))
   **/
   /** \precond
   *Set up float values to operate on and compute expected result.
   **/
   float x1_1 = 0.00000002F;
   float x2_1 = 0.00000003F;
   float y1_1 = 1.0f;
   float y2_1 = 2.0f;
   float x_1 = 0.00000001F;
   float out1;
   
   /** \action
   *Call function.
   **/
   out1 = F360_Linear_Equation(x_1, x1_1, x2_1, y1_1, y2_1);
   
   /** \result
   *Ensure function output is equal to the expected result.
   **/
   DOUBLES_EQUAL(1.0f, out1, TEST_PASS_TH_SMALL);

   /** \step{2}
   *Test behavior of zero and close to zero denominator
   *else - first if ((x > x1) && (x1 > x2))
   **/
   /** \precond
   *Set up float values to operate on and compute expected result.
   **/
   float x1_2 = 0.00000002F;
   float x2_2 = 0.00000001F;
   float y1_2 = 1.0f;
   float y2_2 = 2.0f;
   float x_2 = 0.00000003F;
   float out2;
   
   /** \action
   *Call function.
   **/
   out2 = F360_Linear_Equation(x_2, x1_2, x2_2, y1_2, y2_2);
   
   /** \result
   *Ensure function output is equal to the expected result.
   **/
   DOUBLES_EQUAL(1.0f, out2, TEST_PASS_TH_SMALL);

   /** \step{3}
   *Test behavior of zero and close to zero denominator
   *else - second if ((x < x2) && (x2 < x1))
   **/
   /** \precond
   *Set up float values to operate on and compute expected result.
   **/
   float x1_3 = 0.00000003F;
   float x2_3 = 0.00000002F;
   float y1_3 = 2.0f;
   float y2_3 = 1.0f;
   float x_3 = 0.00000001F;
   float out3;
   
   /** \action
   *Call function.
   **/
   out3 = F360_Linear_Equation(x_3, x1_3, x2_3, y1_3, y2_3);
   
   /** \result
   *Ensure function output is equal to the expected result.
   **/
   DOUBLES_EQUAL(1.0f, out3, TEST_PASS_TH_SMALL);

   /** \step{4}
   *Test behavior of zero and close to zero denominator
   *else - second if ((x > x2) && (x2 > x1))
   **/
   /** \precond
   *Set up float values to operate on and compute expected result.
   **/
   float x1_4 = 0.00000001F;
   float x2_4 = 0.00000002F;
   float y1_4 = 2.0f;
   float y2_4 = 1.0f;
   float x_4 = 0.00000003F;
   float out4;
   
   /** \action
   *Call function.
   **/
   out4 = F360_Linear_Equation(x_4, x1_4, x2_4, y1_4, y2_4);
   
   /** \result
   *Ensure function output is equal to the expected result.
   **/
   DOUBLES_EQUAL(1.0f, out4, TEST_PASS_TH_SMALL);

   /** \step{5}
   *Test behavior of zero and close to zero denominator
   *else - else ( x1 == x2 )
   **/
   /** \precond
   *Set up float values to operate on and compute expected result.
   **/
   float x1_5 = 0.0f;
   float x2_5 = 0.0f;
   float y1_5 = 1.0f;
   float y2_5 = 2.0f;
   float x_5 = 3.0f;
   float out5;
   
   /** \action
   *Call function.
   **/
   out5 = F360_Linear_Equation(x_5, x1_5, x2_5, y1_5, y2_5);
   
   /** \result
   *Ensure function output is equal to the expected result.
   **/
   DOUBLES_EQUAL(2.0f, out5, TEST_PASS_TH_SMALL);
   
   /** \step{6}
   *Test behavior of zero and close to zero denominator
   *else - else ( x1 == x2 ) with nonzero x1,x2 and x1>x2
   **/
   /** \precond
   *Set up float values to operate on and compute expected result.
   **/
   float x1_6 = 0.00000002F;
   float x2_6 = 0.00000001F;
   float y1_6 = 1.0f;
   float y2_6 = 2.0f;
   float x_6 = 0.000000015F;
   float out6;
   
   /** \action
   *Call function.
   **/
   out6 = F360_Linear_Equation(x_6, x1_6, x2_6, y1_6, y2_6);
   
   /** \result
   *Ensure function output is equal to the expected result.
   **/
   DOUBLES_EQUAL(1.5f, out6, TEST_PASS_TH_SMALL);
   
   /** \step{7}
   *Test behavior of zero and close to zero denominator
   *else - else ( x1 == x2 ) with nonzero x1,x2 and x2>x1
   **/
   /** \precond
   *Set up float values to operate on and compute expected result.
   **/
   float x1_7 = 0.00000001F;
   float x2_7 = 0.00000002F;
   float y1_7 = 1.0f;
   float y2_7 = 2.0f;
   float x_7 = 0.000000015F;
   float out7;
   
   /** \action
   *Call function.
   **/
   out7 = F360_Linear_Equation(x_7, x1_7, x2_7, y1_7, y2_7);
   
   /** \result
   *Ensure function output is equal to the expected result.
   **/
   DOUBLES_EQUAL(1.5f, out7, TEST_PASS_TH_SMALL);
}

/**
 *\purpose  Check if function returns the correct determinant value.
 *\req    NA
 */
TEST(f360_math_func, Test_F360_2d_Matrix_Determinant)
{
   /** \step{1}
    * Test behaviour of function when inputting a matrix with all elements zero
    **/

   /** \precond
    * Set up matrix and expected output determinant value.
    **/
   float mat1[2][2] = {{0.0f, 0.0f}, {0.0f, 0.0f}};
   float expected_determinant1 = 0.0f;
   float result1;

   /** \action
    *Call function.
    **/
   result1 = F360_2d_Matrix_Determinant(mat1);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_determinant1, result1, TEST_PASS_TH_SMALL, "zero matrix");

   /** \step{2}
    * Test behaviour of function when inputting a matrix with all elements zero except 1
    * (case 1/4, first element is non-zero)
    **/

   /** \precond
    * Set up matrix and expected output determinant value.
    **/
   float mat2[2][2] = {{10.0f, 0.0f}, {0.0f, 0.0f}};
   float expected_determinant2 = 0.0f;
   float result2;

   /** \action
    *Call function.
    **/
   result2 = F360_2d_Matrix_Determinant(mat2);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_determinant2, result2, TEST_PASS_TH_SMALL, "all matrix elements zero except first");

   /** \step{3}
    * Test behaviour of function when inputting a matrix with all elements zero except 1
    * (case 2/4, second element is non-zero)
    **/

   /** \precond
    * Set up matrix and expected output determinant value.
    **/
   float mat3[2][2] = {{0.0f, 10.0f}, {0.0f, 0.0f}};
   float expected_determinant3 = 0.0f;
   float result3;

   /** \action
    *Call function.
    **/
   result3 = F360_2d_Matrix_Determinant(mat3);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_determinant3, result3, TEST_PASS_TH_SMALL, "all matrix elements zero except second");

   /** \step{4}
    * Test behaviour of function when inputting a matrix with all elements zero except 1
    * (case 3/4, third element is non-zero)
    **/

   /** \precond
    * Set up matrix and expected output determinant value.
    **/
   float mat4[2][2] = {{0.0f, 0.0f}, {10.0f, 0.0f}};
   float expected_determinant4 = 0.0f;
   float result4;

   /** \action
    *Call function.
    **/
   result4 = F360_2d_Matrix_Determinant(mat4);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_determinant4, result4, TEST_PASS_TH_SMALL, "all matrix elements zero except third");

   /** \step{5}
    * Test behaviour of function when inputting a matrix with all elements zero except 1
    * (case 4/4, fourth element is non-zero)
    **/

   /** \precond
    * Set up matrix and expected output determinant value.
    **/
   float mat5[2][2] = {{0.0f, 0.0f}, {0.0f, 10.0f}};
   float expected_determinant5 = 0.0f;
   float result5;

   /** \action
    *Call function.
    **/
   result5 = F360_2d_Matrix_Determinant(mat5);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_determinant5, result5, TEST_PASS_TH_SMALL, "all matrix elements zero except fourth");

   /** \step{6}
    * Test behaviour of function when inputting a matrix with all elements zero except 2
    * (case 1/12, first and second element is non-zero)
    **/

   /** \precond
    * Set up matrix and expected output determinant value.
    **/
   float mat6[2][2] = {{10.0f, 6.0f}, {0.0f, 0.0f}};
   float expected_determinant6 = 0.0f;
   float result6;

   /** \action
    *Call function.
    **/
   result6 = F360_2d_Matrix_Determinant(mat6);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_determinant6, result6, TEST_PASS_TH_SMALL, "first and second matrix elements non-zero");

   /** \step{7}
    * Test behaviour of function when inputting a matrix with all elements zero except 2
    * (case 2/12, first and third element is non-zero)
    **/

   /** \precond
    * Set up matrix and expected output determinant value.
    **/
   float mat7[2][2] = {{10.0f, 0.0f}, {6.0f, 0.0f}};
   float expected_determinant7 = 0.0f;
   float result7;

   /** \action
    *Call function.
    **/
   result7 = F360_2d_Matrix_Determinant(mat7);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_determinant7, result7, TEST_PASS_TH_SMALL, "first and third matrix elements non-zero");

   /** \step{8}
    * Test behaviour of function when inputting a matrix with all elements zero except 2
    * (case 3/12, second and fourth element is non-zero)
    **/

   /** \precond
    * Set up matrix and expected output determinant value.
    **/
   float mat8[2][2] = {{0.0f, 10.0f}, {0.0f, 6.0f}};
   float expected_determinant8 = 0.0f;
   float result8;

   /** \action
    *Call function.
    **/
   result8 = F360_2d_Matrix_Determinant(mat8);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_determinant8, result8, TEST_PASS_TH_SMALL, "second and fourth matrix elements non-zero");

   /** \step{9}
    * Test behaviour of function when inputting a matrix with all elements zero except 2
    * (case 4/12, third and fourth element is non-zero)
    **/

   /** \precond
    * Set up matrix and expected output determinant value.
    **/
   float mat9[2][2] = {{0.0f, 0.0f}, {10.0f, 6.0f}};
   float expected_determinant9 = 0.0f;
   float result9;

   /** \action
    *Call function.
    **/
   result9 = F360_2d_Matrix_Determinant(mat9);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_determinant9, result9, TEST_PASS_TH_SMALL, "third and fourth matrix elements non-zero");

   /** \step{10}
    * Test behaviour of function when inputting a matrix with all elements zero except 2
    * (case 5/12, first element positive fourth element positive)
    **/

   /** \precond
    * Set up matrix and expected output determinant value.
    **/
   float mat10[2][2] = {{10.0f, 0.0f}, {0.0f, 6.0f}};
   float expected_determinant10 = 60.0f;
   float result10;

   /** \action
    *Call function.
    **/
   result10 = F360_2d_Matrix_Determinant(mat10);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_determinant10, result10, TEST_PASS_TH_SMALL, "first and fourth matrix elements positive");

   /** \step{11}
    * Test behaviour of function when inputting a matrix with all elements zero except 2
    * (case 6/12, first element positive fourth element negative)
    **/

   /** \precond
    * Set up matrix and expected output determinant value.
    **/
   float mat11[2][2] = {{10.0f, 0.0f}, {0.0f, -6.0f}};
   float expected_determinant11 = -60.0f;
   float result11;

   /** \action
    *Call function.
    **/
   result11 = F360_2d_Matrix_Determinant(mat11);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_determinant11, result11, TEST_PASS_TH_SMALL, "first matrix element positive, fourth negative");

   /** \step{12}
    * Test behaviour of function when inputting a matrix with all elements zero except 2
    * (case 7/12, first element negative fourth element positive)
    **/

   /** \precond
    * Set up matrix and expected output determinant value.
    **/
   float mat12[2][2] = {{-10.0f, 0.0f}, {0.0f, 6.0f}};
   float expected_determinant12 = -60.0f;
   float result12;

   /** \action
    *Call function.
    **/
   result12 = F360_2d_Matrix_Determinant(mat12);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_determinant12, result12, TEST_PASS_TH_SMALL, "first matrix element negative, fourth positive");

   /** \step{13}
    * Test behaviour of function when inputting a matrix with all elements zero except 2
    * (case 8/12, first element negative fourth element negative)
    **/

   /** \precond
    * Set up matrix and expected output determinant value.
    **/
   float mat13[2][2] = {{-10.0f, 0.0f}, {0.0f, -6.0f}};
   float expected_determinant13 = 60.0f;
   float result13;

   /** \action
    *Call function.
    **/
   result13 = F360_2d_Matrix_Determinant(mat13);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_determinant13, result13, TEST_PASS_TH_SMALL, "first and fourth matrix elements negative");

   /** \step{14}
    * Test behaviour of function when inputting a matrix with all elements zero except 2
    * (case 9/12, second element positive third element positive)
    **/

   /** \precond
    * Set up matrix and expected output determinant value.
    **/
   float mat14[2][2] = {{0.0f, 10.0f}, {6.0f, 0.0f}};
   float expected_determinant14 = -60.0f;
   float result14;

   /** \action
    *Call function.
    **/
   result14 = F360_2d_Matrix_Determinant(mat14);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_determinant14, result14, TEST_PASS_TH_SMALL, "second and third matrix elements positive");

   /** \step{15}
    * Test behaviour of function when inputting a matrix with all elements zero except 2
    * (case 10/12, second element positive third element negative)
    **/

   /** \precond
    * Set up matrix and expected output determinant value.
    **/
   float mat15[2][2] = {{0.0f, 10.0f}, {-6.0f, 0.0f}};
   float expected_determinant15 = 60.0f;
   float result15;

   /** \action
    *Call function.
    **/
   result15 = F360_2d_Matrix_Determinant(mat15);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_determinant15, result15, TEST_PASS_TH_SMALL, "second matrix element positive, third negative");

   /** \step{16}
    * Test behaviour of function when inputting a matrix with all elements zero except 2
    * (case 11/12, second element negative third element positive)
    **/

   /** \precond
    * Set up matrix and expected output determinant value.
    **/
   float mat16[2][2] = {{0.0f, -10.0f}, {6.0f, 0.0f}};
   float expected_determinant16 = 60.0f;
   float result16;

   /** \action
    *Call function.
    **/
   result16 = F360_2d_Matrix_Determinant(mat16);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_determinant16, result16, TEST_PASS_TH_SMALL, "second matrix element negative, third positive");

   /** \step{17}
    * Test behaviour of function when inputting a matrix with all elements zero except 2
    * (case 12/12, second element negative third element negative)
    **/

   /** \precond
    * Set up matrix and expected output determinant value.
    **/
   float mat17[2][2] = {{0.0f, -10.0f}, {-6.0f, 0.0f}};
   float expected_determinant17 = -60.0f;
   float result17;

   /** \action
    *Call function.
    **/
   result17 = F360_2d_Matrix_Determinant(mat17);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_determinant17, result17, TEST_PASS_TH_SMALL, "second and third matrix elements negative");

   /** \step{18}
    * Test behaviour of function when inputting a full positive definite covariance matrix
    **/

   /** \precond
    * Set up matrix and expected output determinant value.
    **/
   float mat18[2][2] = {{10.11f, -1.42f}, {-1.42f, 6.63f}};
   float expected_determinant18 = 65.0129f;
   float result18;

   /** \action
    *Call function.
    **/
   result18 = F360_2d_Matrix_Determinant(mat18);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_determinant18, result18, TEST_PASS_TH_MID, "full positive definite matrix (covariance matrix)");

   /** \step{19}
    * Test behaviour of function when inputting a full arbitrary matrix
    **/

   /** \precond
    * Set up matrix and expected output determinant value.
    **/
   float mat19[2][2] = {{1.56f, 3.49f}, {-4.46f, 4.34f}};
   float expected_determinant19 = 22.3358f;
   float result19;

   /** \action
    *Call function.
    **/
   result19 = F360_2d_Matrix_Determinant(mat19);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_determinant19, result19, TEST_PASS_TH_SMALL, "full arbitrary matrix");
}

/**
 *\purpose  Check that F360_Piecewise_Linear_Equation returns the correct
 * value when the input breakpoints are sorted.
 *\req    NA
 */
TEST(f360_math_func, Test_F360_Piecewise_Linear_Equation_Sorted_Breakpoints)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up breakpoints, query points and expected result.
    **/
   const float breakpoints_x[3] = {1.0F, 2.0F, 3.0F};
   const float breakpoints_y[3] = {10.0F, 20.0F, 40.0F};
   const int num_breakpoints = 3;

   float x_query_1 = 0.0F;
   float expected_y_query_1 = 10.0F;

   float x_query_2 = 1.0F;
   float expected_y_query_2 = 10.0F;

   float x_query_3 = 1.2F;
   float expected_y_query_3 = 12.0F;

   float x_query_4 = 2.0F;
   float expected_y_query_4 = 20.0F;

   float x_query_5 = 2.7F;
   float expected_y_query_5 = 34.0F;

   float x_query_6 = 3.0F;
   float expected_y_query_6 = 40.0F;

   float x_query_7 = 4.0F;
   float expected_y_query_7 = 40.0F;

   /** \action
    *Call function with query point 1.
    **/
   float y_query_result = F360_Piecewise_Linear_Equation(x_query_1, breakpoints_x, breakpoints_y, num_breakpoints);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_y_query_1, y_query_result, TEST_PASS_TH_SMALL, "Query 1 for sorted brekpoint array failed");

   /** \action
    *Call function with query point 2.
    **/
   y_query_result = F360_Piecewise_Linear_Equation(x_query_2, breakpoints_x, breakpoints_y, num_breakpoints);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_y_query_2, y_query_result, TEST_PASS_TH_SMALL, "Query 2 for sorted brekpoint array failed");

   /** \action
    *Call function with query point 3.
    **/
   y_query_result = F360_Piecewise_Linear_Equation(x_query_3, breakpoints_x, breakpoints_y, num_breakpoints);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_y_query_3, y_query_result, TEST_PASS_TH_SMALL, "Query 3 for sorted brekpoint array failed");

   /** \action
    *Call function with query point 4.
    **/
   y_query_result = F360_Piecewise_Linear_Equation(x_query_4, breakpoints_x, breakpoints_y, num_breakpoints);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_y_query_4, y_query_result, TEST_PASS_TH_SMALL, "Query 4 for sorted brekpoint array failed");

   /** \action
    *Call function with query point 5.
    **/
   y_query_result = F360_Piecewise_Linear_Equation(x_query_5, breakpoints_x, breakpoints_y, num_breakpoints);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_y_query_5, y_query_result, TEST_PASS_TH_SMALL, "Query 5 for sorted brekpoint array failed");

   /** \action
    *Call function with query point 6.
    **/
   y_query_result = F360_Piecewise_Linear_Equation(x_query_6, breakpoints_x, breakpoints_y, num_breakpoints);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_y_query_6, y_query_result, TEST_PASS_TH_SMALL, "Query 6 for sorted brekpoint array failed");

   /** \action
    *Call function with query point 7.
    **/
   y_query_result = F360_Piecewise_Linear_Equation(x_query_7, breakpoints_x, breakpoints_y, num_breakpoints);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_y_query_7, y_query_result, TEST_PASS_TH_SMALL, "Query 7 for sorted brekpoint array failed");
}


/**
 *\purpose  Check that F360_Piecewise_Linear_Equation returns the correct
 * value when the input breakpoints are very close.
 *\req    NA
 */
TEST(f360_math_func, Test_F360_Piecewise_Linear_Equation_Close_Breakpoints)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up breakpoints, query points and expected result.
    **/
   const float breakpoints_x[2] = {1.0F, 1.0F+2e-7};
   const float breakpoints_y[2] = {10.0F, 20.0F};
   const int num_breakpoints = 2;

   float x_query = 1.0F + 1e-7;
   float expected_y_query = 15.0F;

   /** \action
    *Call function with query point 1.
    **/
   float y_query_result = F360_Piecewise_Linear_Equation(x_query, breakpoints_x, breakpoints_y, num_breakpoints);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_y_query, y_query_result, TEST_PASS_TH_SMALL, "Failed for close breakpoints");
}

/**
 *\purpose  Check if F360_Linear_Equation_With_Saturation returns the correct value for finite slopes.
 *\req    NA
 */
TEST(f360_math_func, Test_F360_Linear_Equation_With_Saturation)
{
   /** \step{1}
    *Compare function output to expected output when not saturating and slope of line is non-zero and finite.
    **/

   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float x1 = 1.0f;
   float x2 = 10.0f;
   float y1 = 0.25f;
   float y2 = 4.75f;

   float x_query_1 = 3.0f;
   float expected_output_1_1 = 1.25f;
   float expected_output_1_2 = 3.75f;

   /** \action
    *Call function.
    **/
   float output_1_1 = F360_Linear_Equation_With_Saturation(x_query_1, x1, x2, y1, y2);
   float output_1_2 = F360_Linear_Equation_With_Saturation(x_query_1, x1, x2, y2, y1);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_output_1_1, output_1_1, TEST_PASS_TH_SMALL, "Failed when not saturating and slope of line is non-zero and finite (1)");
   DOUBLES_EQUAL_TEXT(expected_output_1_2, output_1_2, TEST_PASS_TH_SMALL, "Failed when not saturating and slope of line is non-zero and finite (2)");

   /** \step{2}
    *Compare function output to expected output when saturating due to too large x query value and slope of line is non-zero and finite.
    **/

   /** \precond
    *Set up expected result.
    **/

   float x_query_2 = 15.0f;
   float expected_output_2_1 = 4.75f;
   float expected_output_2_2 = 0.25f;

   /** \action
    *Call function.
    **/
   float output_2_1 = F360_Linear_Equation_With_Saturation(x_query_2, x1, x2, y1, y2);
   float output_2_2 = F360_Linear_Equation_With_Saturation(x_query_2, x1, x2, y2, y1);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_output_2_1, output_2_1, TEST_PASS_TH_SMALL, "Failed when not saturating due to too large x query value and slope of line is non-zero and finite(1)");
   DOUBLES_EQUAL_TEXT(expected_output_2_2, output_2_2, TEST_PASS_TH_SMALL, "Failed when not saturating due to too large x query value and slope of line is non-zero and finite(2)");

   /** \step{3}
    *Compare function output to expected output when saturating due to too small x query value and slope of line is non-zero and finite.
    **/

   /** \precond
    *Set up expected result.
    **/

   float x_query_3 = -2.0f;
   float expected_output_3_1 = 0.25f;
   float expected_output_3_2 = 4.75f;

   /** \action
    *Call function.
    **/
   float output_3_1 = F360_Linear_Equation_With_Saturation(x_query_3, x1, x2, y1, y2);
   float output_3_2 = F360_Linear_Equation_With_Saturation(x_query_3, x1, x2, y2, y1);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_output_3_1, output_3_1, TEST_PASS_TH_SMALL, "Failed when not saturating due to too small x query value (1)");
   DOUBLES_EQUAL_TEXT(expected_output_3_2, output_3_2, TEST_PASS_TH_SMALL, "Failed when not saturating due to too small x query value (2)");

   /** \step{4}
    *Compare function output to expected output when not saturating and slope of line is zero.
    **/

   /** \precond
    *Set up expected result.
    **/
   float x_query_4 = 3.0f;
   float expected_output_4_1 = 0.25f;
   float expected_output_4_2 = 4.75f;

   /** \action
    *Call function.
    **/
   float output_4_1 = F360_Linear_Equation_With_Saturation(x_query_4, x1, x2, y1, y1);
   float output_4_2 = F360_Linear_Equation_With_Saturation(x_query_4, x2, x1, y2, y2);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_output_4_1, output_4_1, TEST_PASS_TH_SMALL, "Failed when not saturating and slope of line is zero (1)");
   DOUBLES_EQUAL_TEXT(expected_output_4_2, output_4_2, TEST_PASS_TH_SMALL, "Failed when not saturating and slope of line is zero (2)");

   /** \step{5}
    *Compare function output to expected output when saturating due to too large x query value and slope of line is zero.
    **/

   /** \precond
    *Set up expected result.
    **/
   float x_query_5 = 15.0f;
   float expected_output_5_1 = 0.25f;
   float expected_output_5_2 = 4.75f;

   /** \action
    *Call function.
    **/
   float output_5_1 = F360_Linear_Equation_With_Saturation(x_query_5, x1, x2, y1, y1);
   float output_5_2 = F360_Linear_Equation_With_Saturation(x_query_5, x2, x1, y2, y2);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_output_5_1, output_5_1, TEST_PASS_TH_SMALL, "Failed when saturating due to too large x query value and slope of line is zero (1)");
   DOUBLES_EQUAL_TEXT(expected_output_5_2, output_5_2, TEST_PASS_TH_SMALL, "Failed when saturating due to too large x query value and slope of line is zero (2)");

   /** \step{6}
    *Compare function output to expected output when saturating due to too large x query value and slope of line is zero.
    **/

   /** \precond
    *Set up expected result.
    **/
   float x_query_6 = -2.0f;
   float expected_output_6_1 = 0.25f;
   float expected_output_6_2 = 4.75f;

   /** \action
    *Call function.
    **/
   float output_6_1 = F360_Linear_Equation_With_Saturation(x_query_6, x1, x2, y1, y1);
   float output_6_2 = F360_Linear_Equation_With_Saturation(x_query_6, x2, x1, y2, y2);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_output_6_1, output_6_1, TEST_PASS_TH_SMALL, "Failed when saturating due to too large x query value and slope of line is zero (1)");
   DOUBLES_EQUAL_TEXT(expected_output_6_2, output_6_2, TEST_PASS_TH_SMALL, "Failed when saturating due to too large x query value and slope of line is zero (2)");
}

/**
*\purpose  Check if F360_Linear_Equation_With_Saturation behaves correctly for infinite slopes.
*\req    NA
*/
TEST(f360_math_func, F360_Linear_Equation_With_Saturation_small_den)
{
   /** \step{1}
    *Compare function output to expected output when not saturating and slope of line is close to infinite.
    **/

   /** \precond
    *Set up float values to operate on and expected result.
    **/
   float x1_1 = 0.0F;
   float x2_1 = 0.00000001F;
   float y1_1 = 1.0f;
   float y2_1 = 2.0f;

   float x_query_1 = 0.000000005f;
   float expected_output_min_1 = 1.0f;
   float expected_output_max_1 = 2.0f;

   /** \action
    *Call function.
    **/
   float output_1_1 = F360_Linear_Equation_With_Saturation(x_query_1, x1_1, x2_1, y1_1, y2_1);
   float output_1_2 = F360_Linear_Equation_With_Saturation(x_query_1, x1_1, x2_1, y2_1, y1_1);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   CHECK_TRUE_TEXT(expected_output_min_1 <= output_1_1, "Failed when not saturating and slope of line is close to infinite (1, min test)");
   CHECK_TRUE_TEXT(expected_output_max_1 >= output_1_1, "Failed when not saturating and slope of line is close to infinite (1, max test)");
   CHECK_TRUE_TEXT(expected_output_min_1 <= output_1_2, "Failed when not saturating and slope of line is close to infinite (2, min test)");
    CHECK_TRUE_TEXT(expected_output_max_1 >= output_1_2, "Failed when not saturating and slope of line is close to infinite (2, max test)");

   /** \step{2}
    *Compare function output to expected output when saturating due to too large x query value and slope of line is close to infinite.
    **/

   /** \precond
    *Set up expected result.
    **/
   float x_query_2 = 2.0f;
   float expected_output_2_1 = 2.0f;
   float expected_output_2_2 = 1.0f;

   /** \action
    *Call function.
    **/
   float output_2_1 = F360_Linear_Equation_With_Saturation(x_query_2, x1_1, x2_1, y1_1, y2_1);
   float output_2_2 = F360_Linear_Equation_With_Saturation(x_query_2, x1_1, x2_1, y2_1, y1_1);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_output_2_1, output_2_1, TEST_PASS_TH_SMALL, "Failed when saturating due to too large x query value and slope of line is close to infinite (1)");
   DOUBLES_EQUAL_TEXT(expected_output_2_2, output_2_2, TEST_PASS_TH_SMALL, "Failed when saturating due to too large x query value and slope of line is close to infinite (2)");

   /** \step{3}
    *Compare function output to expected output when saturating due to too small x query value and slope of line is close to infinite.
    **/

   /** \precond
    *Set up expected result.
    **/
   float x_query_3 = -2.0f;
   float expected_output_3_1 = 1.0f;
   float expected_output_3_2 = 2.0f;

   /** \action
    *Call function.
    **/
   float output_3_1 = F360_Linear_Equation_With_Saturation(x_query_3, x1_1, x2_1, y1_1, y2_1);
   float output_3_2 = F360_Linear_Equation_With_Saturation(x_query_3, x1_1, x2_1, y2_1, y1_1);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_output_3_1, output_3_1, TEST_PASS_TH_SMALL, "Failed when saturating due to too small x query value and slope of line is close to infinite (1)");
   DOUBLES_EQUAL_TEXT(expected_output_3_2, output_3_2, TEST_PASS_TH_SMALL, "Failed when saturating due to too small x query value and slope of line is close to infinite (2)");

   /** \step{4}
    *Compare function output to expected output when not saturating and slope of line is infinite.
    **/

   /** \precond
    *Set up float values to operate on and expected result.
    **/
   float x1_2 = 1.0F;
   float x2_2 = 1.0F;
   float y1_2 = 4.0f;
   float y2_2 = 5.0f;

   float x_query_4 = 1.0f;
   float expected_output_min_2 = 4.0f;
   float expected_output_max_2 = 5.0f;

   /** \action
    *Call function.
    **/
   float output_4_1 = F360_Linear_Equation_With_Saturation(x_query_4, x1_2, x2_2, y1_2, y2_2);
   float output_4_2 = F360_Linear_Equation_With_Saturation(x_query_4, x1_2, x2_2, y2_2, y1_2);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   CHECK_TRUE_TEXT(expected_output_min_2 <= output_4_1, "Failed when not saturating and slope of line is infinite (1, min test)");
   CHECK_TRUE_TEXT(expected_output_max_2 >= output_4_1, "Failed when not saturating and slope of line is infinite (1, max test)");
   CHECK_TRUE_TEXT(expected_output_min_2 <= output_4_2, "Failed when not saturating and slope of line is infinite (2, min test)");
    CHECK_TRUE_TEXT(expected_output_max_2 >= output_4_2, "Failed when not saturating and slope of line is infinite (2, max test)");

   /** \step{5}
    *Compare function output to expected output when saturating due to too large x query value and slope of line is infinite.
    **/

   /** \precond
    *Set up expected result.
    **/
   float x_query_5 = 2.0f;
   float expected_output_5_1 = 5.0f;
   float expected_output_5_2 = 4.0f;

   /** \action
    *Call function.
    **/
   float output_5_1 = F360_Linear_Equation_With_Saturation(x_query_5, x1_2, x2_2, y1_2, y2_2);
   float output_5_2 = F360_Linear_Equation_With_Saturation(x_query_5, x1_2, x2_2, y2_2, y1_2);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_output_5_1, output_5_1, TEST_PASS_TH_SMALL, "Failed when saturating due to too large x query value and slope of line is infinite (1)");
   DOUBLES_EQUAL_TEXT(expected_output_5_2, output_5_2, TEST_PASS_TH_SMALL, "Failed when saturating due to too large x query value and slope of line is infinite (2)");

   /** \step{6}
    *Compare function output to expected output when saturating due to too small x query value and slope of line is infinite.
    **/

   /** \precond
    *Set up float values to operate on and expected result.
    **/
   float x_query_6 = -2.0f;
   float expected_output_6_1 = 4.0f;
   float expected_output_6_2 = 5.0f;

   /** \action
    *Call function.
    **/
   float output_6_1 = F360_Linear_Equation_With_Saturation(x_query_6, x1_2, x2_2, y1_2, y2_2);
   float output_6_2 = F360_Linear_Equation_With_Saturation(x_query_6, x1_2, x2_2, y2_2, y1_2);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_output_6_1, output_6_1, TEST_PASS_TH_SMALL, "Failed when saturating due to too small x query value and slope of line is infinite (1)");
   DOUBLES_EQUAL_TEXT(expected_output_6_2, output_6_2, TEST_PASS_TH_SMALL, "Failed when saturating due to too small x query value and slope of line is infinite (2)");
}


/**
*\purpose  Check if F360_Low_Pass_Filter_First_Order returns correct output.
*\req    NA
*/
TEST(f360_math_func, Test_F360_Low_Pass_Filter_First_Order)
{
   /** \step{1}
    *Compare function output to expected low pass filtered value
    **/

   /** \precond
    *Set up float values to operate on and expected result.
    **/
   float old_value = 4.587f;
   float measurement_value = 0.463785f;
   float filter_constant = 0.845f;
   float expected_filtered_value = (1.0f - filter_constant)*old_value + filter_constant*measurement_value;

   /** \action
    *Call function.
    **/
   float filtered_value = F360_Low_Pass_Filter_First_Order(measurement_value, old_value, filter_constant);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_filtered_value, filtered_value, TEST_PASS_TH_MID, "Unexpected return value from low pass filter");
}

/**
*\purpose  Check if F360_Low_Pass_Filter_Angle_First_Order returns correct output for values far from edges +-pi.
*\req    NA
*/
TEST(f360_math_func, Test_F360_Low_Pass_Filter_Angle_First_Order)
{
   /** \step{1}
    *Compare function output to expected low pass filtered value
    **/

   /** \precond
    *Set up float values to operate on and expected result.
    **/
   float old_value = F360_DEG2RAD(30.0F);
   float measurement_value = F360_DEG2RAD(35.0F);
   float filter_constant = 0.75F;
   float expected_filtered_value = (1.0f - filter_constant)*old_value + filter_constant*measurement_value;

   /** \action
    *Call function.
    **/
   float filtered_value = F360_Low_Pass_Filter_Angle_First_Order(measurement_value, old_value, filter_constant);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_filtered_value, filtered_value, TEST_PASS_TH_MID, "Unexpected return value from low pass filter");
}

/**
*\purpose  Check if F360_Low_Pass_Filter_Angle_First_Order returns correct output where old value > -pi and measured value < pi.
*\req    NA
*/
TEST(f360_math_func, Test_F360_Low_Pass_Filter_Angle_First_Order_neg_filt_pos_meas)
{
   /** \step{1}
    *Compare function output to expected low pass filtered value
    **/

   /** \precond
    *Set up float values to operate on and expected result.
    **/
   float old_value = F360_DEG2RAD(-178.0F);
   float measurement_value = F360_DEG2RAD(177.0F);
   float filter_constant = 0.75F;
   float expected_filtered_value = (1.0f - filter_constant) * (old_value + 2.0F * F360_PI) + filter_constant*measurement_value;

   /** \action
    *Call function.
    **/
   float filtered_value = F360_Low_Pass_Filter_Angle_First_Order(measurement_value, old_value, filter_constant);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_filtered_value, filtered_value, TEST_PASS_TH_MID, "Unexpected return value from low pass filter");
}

/**
*\purpose  Check if F360_Low_Pass_Filter_Angle_First_Order returns correct output where old value < pi and measured value > -pi.
*\req    NA
*/
TEST(f360_math_func, Test_F360_Low_Pass_Filter_Angle_First_Order_pos_filt_neg_meas)
{
   /** \step{1}
    *Compare function output to expected low pass filtered value
    **/

   /** \precond
    *Set up float values to operate on and expected result.
    **/
   float old_value = F360_DEG2RAD(178.0F);
   float measurement_value = F360_DEG2RAD(-177.0F);
   float filter_constant = 0.75F;
   float expected_filtered_value = (1.0f - filter_constant)*(old_value - 2.0F * F360_PI) + filter_constant*measurement_value;

   /** \action
    *Call function.
    **/
   float filtered_value = F360_Low_Pass_Filter_Angle_First_Order(measurement_value, old_value, filter_constant);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_filtered_value, filtered_value, TEST_PASS_TH_MID, "Unexpected return value from low pass filter");
}

/**
*\purpose  Check if F360_Safe_Log10f returns correct output.
*\req    NA
*/
TEST(f360_math_func, Test_F360_Safe_Log10f)
{
   /** \step{1}
    * Compare function output to expected output for non-zero input.
    **/

   /** \precond
    *Set up float values to operate on and expected result.
    **/
   const float input_1 = 74.678f;
   const float expected_output_1 = 1.87319268f;

   /** \action
    *Call function.
    **/
   float output_1 = F360_Safe_Log10f(input_1);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_output_1, output_1, TEST_PASS_TH_SMALL, "Unexpected return value for non-zero input");

   /** \step{2}
    * Compare function output to expected output for zero input.
    **/

   /** \precond
    *Set up float values to operate on and expected result.
    **/
   const float input_2 = 0.0f;
   const float expected_output_2 = -6.92445304f; // corresponds to log(1.19e-7)

   /** \action
    *Call function.
    **/
   float output_2 = F360_Safe_Log10f(input_2);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_output_2, output_2, TEST_PASS_TH_SMALL, "Unexpected return value for zero input");
}

/**
*\purpose  Check if F360_Sign returns 1 for positive input numbers, -1 for negative input numbers and 0 for zero input
*\req    NA
*/
TEST(f360_math_func, Test_F360_Sign)
{
   /** \step{1}
    * Check that -1 is returned for negative input.
    **/

   /** \precond
    *Set up float values to operate on and expected result.
    **/
   const float val_1 = -16.44f;
   const int expected_output_1 = -1;

   /** \action
    *Call function.
    **/
   int output_1 = F360_Sign(val_1);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   CHECK_EQUAL_TEXT(expected_output_1, output_1, "Fail when input is negative");

   /** \step{2}
    * Check that 0 is returned for 0 input.
    **/

   /** \precond
    *Set up float values to operate on and expected result.
    **/
   const float val_2 = 0.0f;
   const int expected_output_2 = 0;

   /** \action
    *Call function.
    **/
   int output_2 = F360_Sign(val_2);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   CHECK_EQUAL_TEXT(expected_output_2, output_2, "Fail when input is zero");

   /** \step{3}
    * Check that 1 is returned for positive input.
    **/

   /** \precond
    *Set up float values to operate on and expected result.
    **/
   const float val_3 = 567.76f;
   const int expected_output_3 = 1;

   /** \action
    *Call function.
    **/
   int output_3 = F360_Sign(val_3);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   CHECK_EQUAL_TEXT(expected_output_3, output_3, "Fail when input is positive");
}


/**
*\purpose  Check if F360_matmul_2x2_2x1 returns correct 2-by-2 and 2-by-1 matrix multiplication
*\req    NA
*/
TEST(f360_math_func, Test_F360_matmul_2x2_2x1)
{
   /** \step{1}
    * Check that correct output is returned.
    **/

   /** \precond
    *Set up values to operate on and expected result.
    **/
   float mat_2x2[2][2] = {{44.567f, 12.563f},{-76.023f, 9.664f}};
   float mat_2x1[2][1] = {-32.456f, 65.213f};
   const float expected_output[2][1] = {-627.1956f, 3097.6209f};

   /** \action
    *Call function.
    **/
   float output[2][1];
   F360_matmul_2x2_2x1(mat_2x2, mat_2x1, output);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_output[0][0], output[0][0], TEST_PASS_TH_LARGE, "Fail for output[0][0]");
   DOUBLES_EQUAL_TEXT(expected_output[1][0], output[1][0], TEST_PASS_TH_LARGE, "Fail for output[1][0]");
}


/**
 *\purpose  Check if function Rotate_2D_Covariance_Matrix_With_Precalc_Coeff
            correctly returns values of Covariance coefficients for PI angle.
 *\req    NA
 */
TEST(f360_math_func, Test_Rotate_2D_Covariance_Matrix_With_Precalc_Coeff_For_Pi_Angle)
{
   /** \precond
    *Set up values to operate on and expected result for PI angle
    **/
   float32_t sin_sq_angle = 0.0F;
   float32_t cos_sq_angle = 1.0F;
   float32_t sin_cos_angle = 0.0F;
   float32_t cos_2_angle = 1.0F;
   float32_t const cov_mat[2][2] = { {32.77389450F, 6.90205461F},
                         {6.90205461F, 8.55933439F} };
   float32_t rotated_cov_mat[2][2] = {};
   float32_t expected_cov_mat[2][2] = { {cov_mat[0][0], cov_mat[0][1]},
                                         {cov_mat[1][0], cov_mat[1][1]} };
   
   /** \action
    *Call function Rotate_2D_Covariance_Matrix_With_Precalc_Coeff()
    **/
   Rotate_2D_Covariance_Matrix_With_Precalc_Coeff(cov_mat, rotated_cov_mat, sin_sq_angle, cos_sq_angle, sin_cos_angle, cos_2_angle);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   for (int row = 0; row < 2; row++)
   {
      for (int col = 0; col < 2; col++)
      {
         DOUBLES_EQUAL(expected_cov_mat[row][col], rotated_cov_mat[row][col], TEST_PASS_TH_MID);
      }
   }
}
/**
 *\purpose  Check if function Calculate_Rotated_2D_Covariance_Coefficients
            correctly returns values of Covariance coefficients for PI/3 angle.
 *\req    NA
 */
TEST(f360_math_func, Test_Rotate_2D_Covariance_Matrix_With_Precalc_Coeff_For_Pi_Over_3)
{
   /** \precond
    *Set up values to operate on and expected result for PI/3 angle.
    **/
   float32_t sin_sq_angle = 0.75F;
   float32_t cos_sq_angle = 0.25F;
   float32_t sin_cos_angle =0.43301270F;
   float32_t cos_2_angle = -0.5F;
   float32_t const cov_mat[2][2] = { {32.77389450F, 6.90205461F},
                         {6.90205461F, 8.55933439F} };
   float32_t rotated_cov_mat[2][2] = {};
   float32_t expected_cov_mat[2][2] = { {8.63561978F, 7.03418479F},
                                         {7.03418479F, 32.69760910F} };

   /** \action
    *Call function Rotate_2D_Covariance_Matrix_With_Precalc_Coeff()
    **/
   Rotate_2D_Covariance_Matrix_With_Precalc_Coeff(cov_mat, rotated_cov_mat, sin_sq_angle, cos_sq_angle, sin_cos_angle, cos_2_angle);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   for (int row = 0; row < 2; row++)
   {
      for (int col = 0; col < 2; col++)
      {
         DOUBLES_EQUAL(expected_cov_mat[row][col], rotated_cov_mat[row][col], TEST_PASS_TH_MID);
      }
   }
}

/**
 *\purpose  Check if function Rotate_2D_Covariance_Matrix_With_Precalc_Coeff
            correctly returns values of Covariance coefficients for 6PI/5 angle.
 *\req    NA
 */
TEST(f360_math_func, Test_Rotate_2D_Covariance_Matrix_With_Precalc_Coeff_For_6_Pi_Over_5)
{
   /** \precond
    *Set up values to operate on and expected result for 6PI/5 angle.
    **/
   float32_t sin_sq_angle = 0.34549150F;
   float32_t cos_sq_angle = 0.65450849F;
   float32_t sin_cos_angle = 0.47552825F;
   float32_t cos_2_angle = 0.30901699F;
   float32_t const cov_mat[2][2] = { {32.77389450F, 6.90205461F},
                         {6.90205461F, 8.55933439F} };
   float32_t rotated_cov_mat[2][2] = {};
   float32_t expected_cov_mat[2][2] = { {17.84372710F, 13.64755976F},
                                         {13.64755976F, 23.48950200F} };

   /** \action
    *Call function Rotate_2D_Covariance_Matrix_With_Precalc_Coeff()
    **/
   Rotate_2D_Covariance_Matrix_With_Precalc_Coeff(cov_mat, rotated_cov_mat, sin_sq_angle, cos_sq_angle, sin_cos_angle, cos_2_angle);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   for (int row = 0; row < 2; row++)
   {
      for (int col = 0; col < 2; col++)
      {
         DOUBLES_EQUAL(expected_cov_mat[row][col], rotated_cov_mat[row][col], TEST_PASS_TH_MID);
      }
   }
}


/**
 *\purpose  Check if function Rotate_2D_Covariance_Matrix_With_Precalc_Coeff
            correctly returns values of Covariance coefficients for -PI/3 angle.
 *\req    NA
 */
TEST(f360_math_func, Test_Rotate_2D_Covariance_Matrix_With_Precalc_Coeff_For_Minus_Pi_Over_3)
{
   /** \precond
    *Set up values to operate on and expected result for -PI/3 angle.
    **/
   float32_t sin_sq_angle = 0.75F;
   float32_t cos_sq_angle = 0.25F;
   float32_t sin_cos_angle = -0.43301270F;
   float32_t cos_2_angle = -0.5F;
   float32_t const cov_mat[2][2] = { {32.77389450F, 6.90205461F},
                         {6.90205461F, 8.55933439F} };
   float32_t rotated_cov_mat[2][2] = {};
   float32_t expected_cov_mat[2][2] = { {20.59033010F, -13.93623940F},
                                         {-13.93623940F, 20.74290080F} };

   /** \action
    *Call function Calculate_Rotated_2D_Covariance_Coefficients()
    **/
   Rotate_2D_Covariance_Matrix_With_Precalc_Coeff(cov_mat, rotated_cov_mat, sin_sq_angle, cos_sq_angle, sin_cos_angle, cos_2_angle);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   for (int row = 0; row < 2; row++)
   {
      for (int col = 0; col < 2; col++)
      {
         DOUBLES_EQUAL(expected_cov_mat[row][col], rotated_cov_mat[row][col], TEST_PASS_TH_MID);
      }
   }
}

/**
 *\purpose  Check if function Rotate_2D_Covariance_Matrix is correctly
            transforming a covariance matrix between two differently oriented
            coordinate systems. Testing rotation angles from all 4 different
            quadrants.
 *\req    NA
 */
TEST(f360_math_func, Test_Rotate_2D_Covariance_Matrix)
{
   /** \step{1}
    * Check that covariance matrix is unchanged when rotating pi radians
    **/

    /** \precond
     *Set up values to operate on and expected result.
     **/
   float rot_angle_1 = M_PI;
   float cos_rot_angle_1 = cosf(rot_angle_1);
   float sin_rot_angle_1 = sinf(rot_angle_1);
   float cov_mat_1[2][2] = {{32.77389450F, 6.90205461F},
                            {6.90205461F, 8.55933439F}};
   float outputted_rot_cov_mat_1[2][2] = {};
   float expected_rot_cov_mat_1[2][2] = {{cov_mat_1[0][0], cov_mat_1[0][1]},
                                         {cov_mat_1[1][0], cov_mat_1[1][1]}};

   /** \action
    *Call function.
    **/
   Rotate_2D_Covariance_Matrix(cos_rot_angle_1, sin_rot_angle_1, cov_mat_1, outputted_rot_cov_mat_1);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   for (int row = 0; row < 2; row++)
   {
      for (int col = 0; col < 2; col++)
      {
         DOUBLES_EQUAL_TEXT(expected_rot_cov_mat_1[row][col], outputted_rot_cov_mat_1[row][col], TEST_PASS_TH_MID, "Rotated covariance matrix is wrong for rotation angle pi radians");
      }
   }


   /** \step{2}
    * Check that covariance matrix is unchanged when rotating -pi radians
    **/

    /** \precond
     *Set up values to operate on and expected result.
     **/
   float rot_angle_2 = -M_PI;
   float cos_rot_angle_2 = cosf(rot_angle_2);
   float sin_rot_angle_2 = sinf(rot_angle_2);
   float cov_mat_2[2][2] = {{31.61796231F, -5.50112402F},
                            {-5.50112402F, 4.87702024F}};
   float outputted_rot_cov_mat_2[2][2] = {};
   float expected_rot_cov_mat_2[2][2] = {{cov_mat_2[0][0], cov_mat_2[0][1]},
                                         {cov_mat_2[1][0], cov_mat_2[1][1]}};

   /** \action
    *Call function.
    **/
   Rotate_2D_Covariance_Matrix(cos_rot_angle_2, sin_rot_angle_2, cov_mat_2, outputted_rot_cov_mat_2);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   for (int row = 0; row < 2; row++)
   {
      for (int col = 0; col < 2; col++)
      {
         DOUBLES_EQUAL_TEXT(expected_rot_cov_mat_2[row][col], outputted_rot_cov_mat_2[row][col], TEST_PASS_TH_MID, "Rotated covariance matrix is wrong for rotation angle -pi radians");
      }
   }


   /** \step{3}
    * Check that rotated covariance matrix is correct when rotating with an angle from fourth quadrant
    **/

    /** \precond
     *Set up values to operate on and expected result.
     **/
   float rot_angle_3 = -1.202470958924280F;
   float cos_rot_angle_3 = cosf(rot_angle_3);
   float sin_rot_angle_3 = sinf(rot_angle_3);
   float cov_mat_3[2][2] = {{1.72230402F, -1.45606155F},
                            {-1.45606155F, 21.93721798F}};
   float outputted_rot_cov_mat_3[2][2] = {};
   float expected_rot_cov_mat_3[2][2] = {{18.33838675F, 7.86883680F},
                                         {7.86883680F, 5.32113524F}};

   /** \action
    *Call function.
    **/
   Rotate_2D_Covariance_Matrix(cos_rot_angle_3, sin_rot_angle_3, cov_mat_3, outputted_rot_cov_mat_3);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   for (int row = 0; row < 2; row++)
   {
      for (int col = 0; col < 2; col++)
      {
         DOUBLES_EQUAL_TEXT(expected_rot_cov_mat_3[row][col], outputted_rot_cov_mat_3[row][col], TEST_PASS_TH_MID, "Rotated covariance matrix is wrong for rotation angle in first quadrant");
      }
   }


   /** \step{4}
    * Check that rotated covariance matrix is correct when rotating with an angle from third quadrant
    **/

    /** \precond
     *Set up values to operate on and expected result.
     **/
   float rot_angle_4 = -2.004375503847542F;
   float cos_rot_angle_4 = cosf(rot_angle_4);
   float sin_rot_angle_4 = sinf(rot_angle_4);
   float cov_mat_4[2][2] = {{32.31565050F, 17.24497269F},
                            {17.24497269F, 35.46824154F}};
   float outputted_rot_cov_mat_4[2][2] = {};
   float expected_rot_cov_mat_4[2][2] = {{21.76262342F, -12.35934157F},
                                         {-12.35934157F, 46.02126861F}};

   /** \action
    *Call function.
    **/
   Rotate_2D_Covariance_Matrix(cos_rot_angle_4, sin_rot_angle_4, cov_mat_4, outputted_rot_cov_mat_4);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   for (int row = 0; row < 2; row++)
   {
      for (int col = 0; col < 2; col++)
      {
         DOUBLES_EQUAL_TEXT(expected_rot_cov_mat_4[row][col], outputted_rot_cov_mat_4[row][col], TEST_PASS_TH_MID, "Rotated covariance matrix is wrong for rotation angle in second quadrant");
      }
   }


   /** \step{5}
    * Check that rotated covariance matrix is correct when rotating with an angle from second quadrant
    **/

    /** \precond
     *Set up values to operate on and expected result.
     **/
   float rot_angle_5 = 2.490132760225441F;
   float cos_rot_angle_5 = cosf(rot_angle_5);
   float sin_rot_angle_5 = sinf(rot_angle_5);
   float cov_mat_5[2][2] = {{24.91820259F, -11.03882307F},
                            {-11.03882307F, 47.98719792F}};
   float outputted_rot_cov_mat_5[2][2] = {};
   float expected_rot_cov_mat_5[2][2] = {{22.75457499F, 8.20131790F},
                                         {8.20131790F, 50.15082551F}};

   /** \action
    *Call function.
    **/
   Rotate_2D_Covariance_Matrix(cos_rot_angle_5, sin_rot_angle_5, cov_mat_5, outputted_rot_cov_mat_5);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   for (int row = 0; row < 2; row++)
   {
      for (int col = 0; col < 2; col++)
      {
         DOUBLES_EQUAL_TEXT(expected_rot_cov_mat_5[row][col], outputted_rot_cov_mat_5[row][col], TEST_PASS_TH_MID, "Rotated covariance matrix is wrong for rotation angle in third quadrant");
      }
   }


   /** \step{6}
    * Check that rotated covariance matrix is correct when rotating with an angle from first quadrant
    **/

    /** \precond
     *Set up values to operate on and expected result.
     **/
   float rot_angle_6 = 0.794755478271581F;
   float cos_rot_angle_6 = cosf(rot_angle_6);
   float sin_rot_angle_6 = sinf(rot_angle_6);
   float cov_mat_6[2][2] = {{11.19059697F, -10.04237063F},
                            {-10.04237063F, 37.56335296F}};
   float outputted_rot_cov_mat_6[2][2] = {};
   float expected_rot_cov_mat_6[2][2] = {{34.66435081F, -12.99614060F},
                                         {-12.99614060F, 14.08959911F}};

   /** \action
    *Call function.
    **/
   Rotate_2D_Covariance_Matrix(cos_rot_angle_6, sin_rot_angle_6, cov_mat_6, outputted_rot_cov_mat_6);

   /** \result
    * Ensure function output is equal to the expected result.
    **/
   for (int row = 0; row < 2; row++)
   {
      for (int col = 0; col < 2; col++)
      {
         DOUBLES_EQUAL_TEXT(expected_rot_cov_mat_6[row][col], outputted_rot_cov_mat_6[row][col], TEST_PASS_TH_MID, "Rotated covariance matrix is wrong for rotation angle in fourth quadrant");
      }
   }
}


/**
*\purpose Check that function F360_Translate_2D_Position works as intended.
*\req    NA
*/
TEST(f360_math_func, Test_F360_Translate_2D_Position)
{

   /** \precond
   * Set up point to be translated and translation vector properties.
   **/
   const float32_t prev_x = 10.0F;
   const float32_t prev_y = 10.0F;
   const float32_t diff_x = 1.0F;
   const float32_t diff_y = -1.0F;
   float32_t next_x = 0.0F;
   float32_t next_y = 0.0F;

   /** \action
   * Call function.
   **/
   F360_Translate_2D_Position(prev_x, prev_y, diff_x, diff_y, next_x, next_y);

   // Define expected result
   const float32_t exp_next_x = 11.0F;
   const float32_t exp_next_y = 9.0F;

   /** \result
   *Ensure function output is equal to the expected result.
   **/
   DOUBLES_EQUAL_TEXT(exp_next_x, next_x, TEST_PASS_TH_SMALL, "Unexpected x position after translation.");
   DOUBLES_EQUAL_TEXT(exp_next_y, next_y, TEST_PASS_TH_SMALL, "Unexpected Y position after translation.");
}

/** @}*/

/** \defgroup  f360_math_func_f360_rotate_2d_vector
 *  @{
**/

/** \brief
 *  Tests for f360_math_func_f360_rotate_2d_vector
 **/
TEST_GROUP(f360_math_func_f360_rotate_2d_vector)
{
   /** \setup
    * Set up float comparison threshold for passing tests
    * Set a fixed initial position
    * Declare common variables used within all tests
    **/
   const float TEST_PASS_TH_SMALL = FLT_EPSILON;
   const float32_t prev_x = 10.0F;
   const float32_t prev_y = -5.0F;
   float32_t next_x;
   float32_t next_y;
   float32_t cos_angle;
   float32_t sin_angle;
   float32_t exp_next_x;
   float32_t exp_next_y;

   TEST_SETUP()
   {
   }

   /** \teardown
    * Nothing to teardown in this test group
    **/
   TEST_TEARDOWN()
   {
   }

};

/**
*\purpose Check that function F360_Rotate_2D_Vector works as intended when angle is zero.
*\req    NA
*/
TEST(f360_math_func_f360_rotate_2d_vector, Test_F360_Rotate_2D_Vector_Angle_Is_Zero)
{

   /** \precond
   * Set up vector to be rotated and rotation angle properties when angle is 0 degrees.
   **/
   cos_angle = 1.0F;
   sin_angle = 0.0F;

   /** \action
   * Call function.
   **/
   F360_Rotate_2D_Vector(prev_x, prev_y, cos_angle, sin_angle, next_x, next_y);

   // Define expected result
   exp_next_x = 10.0F;
   exp_next_y = -5.0F;

   /** \result
   * Ensure function output is equal to the expected result.
   **/
   DOUBLES_EQUAL_TEXT(exp_next_x, next_x, TEST_PASS_TH_SMALL, "Unexpected x value after rotation.");
   DOUBLES_EQUAL_TEXT(exp_next_y, next_y, TEST_PASS_TH_SMALL, "Unexpected y value after rotation.");
}

/**
*\purpose Check that function F360_Rotate_2D_Vector works as intended when angle is 180 degrees.
*\req    NA
*/
TEST(f360_math_func_f360_rotate_2d_vector, Test_F360_Rotate_2D_Vector_Angle_Is_180)
{

   /** \precond
   * Set up vector to be rotated and rotation angle properties when angle is 180 degrees.
   **/
   cos_angle = -1.0F;
   sin_angle = 0.0F;

   /** \action
   * Call function.
   **/
   F360_Rotate_2D_Vector(prev_x, prev_y, cos_angle, sin_angle, next_x, next_y);

   // Define expected result
   exp_next_x = -10.0F;
   exp_next_y = 5.0F;

   /** \result
   * Ensure function output is equal to the expected result.
   **/
   DOUBLES_EQUAL_TEXT(exp_next_x, next_x, TEST_PASS_TH_SMALL, "Unexpected x value after rotation.");
   DOUBLES_EQUAL_TEXT(exp_next_y, next_y, TEST_PASS_TH_SMALL, "Unexpected y value after rotation.");
}

/**
*\purpose Check that function F360_Rotate_2D_Vector works as intended when angle is 90 degrees.
*\req    NA
*/
TEST(f360_math_func_f360_rotate_2d_vector, Test_F360_Rotate_2D_Vector_Angle_Is_90)
{

   /** \precond
   * Set up vector to be rotated and rotation angle properties when angle is 90 degrees.
   **/
   cos_angle = 0.0F;
   sin_angle = 1.0F;

   /** \action
   * Call function.
   **/
   F360_Rotate_2D_Vector(prev_x, prev_y, cos_angle, sin_angle, next_x, next_y);

   // Define expected result
   exp_next_x = 5.0F;
   exp_next_y = 10.0F;

   /** \result
   * Ensure function output is equal to the expected result.
   **/
   DOUBLES_EQUAL_TEXT(exp_next_x, next_x, TEST_PASS_TH_SMALL, "Unexpected x value after rotation.");
   DOUBLES_EQUAL_TEXT(exp_next_y, next_y, TEST_PASS_TH_SMALL, "Unexpected y value after rotation.");
}

/**
*\purpose Check that function F360_Rotate_2D_Vector works as intended when angle is -90 degrees.
*\req    NA
*/
TEST(f360_math_func_f360_rotate_2d_vector, Test_F360_Rotate_2D_Vector_Angle_Is_Minus_90)
{

   /** \precond
   * Set up vector to be rotated and rotation angle properties when angle is -90 degrees.
   **/
   cos_angle = 0.0F;
   sin_angle = -1.0F;

   /** \action
   * Call function.
   **/
   F360_Rotate_2D_Vector(prev_x, prev_y, cos_angle, sin_angle, next_x, next_y);

   // Define expected result
   exp_next_x = -5.0F;
   exp_next_y = -10.0F;

   /** \result
   * Ensure function output is equal to the expected result.
   **/
   DOUBLES_EQUAL_TEXT(exp_next_x, next_x, TEST_PASS_TH_SMALL, "Unexpected x value after rotation.");
   DOUBLES_EQUAL_TEXT(exp_next_y, next_y, TEST_PASS_TH_SMALL, "Unexpected y value after rotation.");
}


/** \purpose Function should return a correct result for valid input values
* \req   NA
*/
TEST(f360_math_func, F360_Huber_Weight_Should_Return_Correct_Result_For_Valid_Input)
{
   /** \precond
   * Set example input values
   */
   const float32_t residual = 4.0F;
   const float32_t tuning_constant = 0.2F;

   /** \action
   * Call tested function
   */
   float32_t actual_result = F360_Huber_Weight(residual, tuning_constant);

   /** \result
   * Check expected result for example input values
   */
   float32_t expected_result = 0.05F;
   DOUBLES_EQUAL(expected_result, actual_result, TEST_PASS_TH_SMALL);
}

/** \purpose
* Function should return zero for tuning_constant equal to zero
* \req   NA
*/
TEST(f360_math_func, F360_Huber_Weight_Should_Return_Zero_For_Tuning_Constant_Equal_To_Zero)
{
   /** \precond
   * Set tuning_constant equal to zero
   */
   const float32_t residual = 4.0F;
   const float32_t tuning_constant = 0.0F;

   /** \action
   * Call tested function
   */
   float32_t actual_result = F360_Huber_Weight(residual, tuning_constant);

   /** \result
   * Function result should be equal to zero
   */
   float32_t expected_result = 0.0F;
   DOUBLES_EQUAL(expected_result, actual_result, TEST_PASS_TH_SMALL);
}

/** \purpose
* Function should return zero for negative value of tuning_constant
* \req   NA
*/
TEST(f360_math_func, F360_Huber_Weight_Should_Return_Zero_For_Negative_Tuning_Constant)
{
   /** \precond
   * Set tuning_constant to negative value
   */
   const float32_t residual = 4.0F;
   const float32_t tuning_constant = -1.0F;

   /** \action
   * Call tested function
   */
   float32_t actual_result = F360_Huber_Weight(residual, tuning_constant);

   /** \result
   * Function result should be equal to zero
   */
   float32_t expected_result = 0.0F;
   DOUBLES_EQUAL(expected_result, actual_result, TEST_PASS_TH_SMALL);
}

/** \purpose
* Function should return zero for tuning_constant value equal to F360_EPSILON
* \req   NA
*/
TEST(f360_math_func, F360_Huber_Weight_Should_Return_Zero_For_Tuning_Constant_Equal_To_Min_Denominator)
{
   /** \precond
   * Set tuning_constant equal to F360_EPSILON
   */
   const float32_t residual = 4.0F;
   const float32_t tuning_constant = F360_EPSILON;

   /** \action
   * Call tested function
   */
   float32_t actual_result = F360_Huber_Weight(residual, tuning_constant);

   /** \result
   * Function result should be equal to zero
   */
   float32_t expected_result = 0.0F;
   DOUBLES_EQUAL(expected_result, actual_result, TEST_PASS_TH_SMALL);
}

/** \purpose
* Function should return zero for tuning_constant value slightly smaller than F360_EPSILON
* \req   NA
*/
TEST(f360_math_func, F360_Huber_Weight_Should_Return_Zero_For_Tuning_Constant_Smaller_Than_Min_Denominator)
{
   /** \precond
   * Set tuning_constant equal slightly smaller than F360_EPSILON
   */
   const float32_t residual = 3.5F;
   const float32_t tuning_constant = F360_EPSILON - 0.01F;

   /** \action
   * Call tested function
   */
   float32_t actual_result = F360_Huber_Weight(residual, tuning_constant);

   /** \result
   * Function result should be equal to zero
   */
   float32_t expected_result = 0.0F;
   DOUBLES_EQUAL(expected_result, actual_result, TEST_PASS_TH_SMALL);
}

/** \purpose
* Function should return valid result for tuning_constant value slightly greater than F360_EPSILON
* \req   NA
*/
TEST(f360_math_func, F360_Huber_Weight_Should_Return_Zero_For_Tuning_Constant_Greater_Than_Min_Denominator)
{
   /** \precond
   * Set tuning_constant equal slightly greater than F360_EPSILON
   */
   const float32_t residual = 6.0F;
   const float32_t tuning_constant = F360_EPSILON + 0.01F;

   /** \action
   * Call tested function
   */
   float32_t actual_result = F360_Huber_Weight(residual, tuning_constant);

   /** \result
   * Function result return a valid result
   */
   float32_t expected_result = 0.001666668F;
   DOUBLES_EQUAL(expected_result, actual_result, TEST_PASS_TH_SMALL);
}

/** \purpose
* Function should return one for tuning_constant greater than positive residual
* \req   NA
*/
TEST(f360_math_func, F360_Huber_Weight_Should_Return_One_For_Tuning_Constant_Greater_Than_Positive_Residual)
{
   /** \precond
   * Set tuning_constant to value greater than positive residual
   */
   const float32_t residual = 0.1F;
   const float32_t tuning_constant = 0.2F;

   /** \action
   * Call tested function
   */
   float32_t actual_result = F360_Huber_Weight(residual, tuning_constant);

   /** \result
   * Function result should be equal to one
   */
   float32_t expected_result = 1.0F;
   DOUBLES_EQUAL(expected_result, actual_result, TEST_PASS_TH_SMALL);
}

/** \purpose
* Function should return one for tuning_constant equal to positive residual
* \req   NA
*/
TEST(f360_math_func, F360_Huber_Weight_Should_Return_One_For_Tuning_Constant_Equal_To_Positive_Residual)
{
   /** \precond
   * Set tuning_constant equal to positive residual
   */
   const float32_t residual = 0.5F;
   const float32_t tuning_constant = 0.5F;

   /** \action
   * Call tested function
   */
   float32_t actual_result = F360_Huber_Weight(residual, tuning_constant);

   /** \result
   * Check expected result for example input values
   */
   float32_t expected_result = 1.0F;
   DOUBLES_EQUAL(expected_result, actual_result, TEST_PASS_TH_SMALL);
}

/** \purpose
*  Function should return valid result for tuning_constant smaller than positive residual
* \req   NA
*/
TEST(f360_math_func, F360_Huber_Weight_Should_Return_Valid_Result_For_Tuning_Constant_Smaller_Than_Positive_Residual)
{
   /** \precond
   * Set tuning_constant smaller than positive residual
   */
   const float32_t residual = 2.0F;
   const float32_t tuning_constant = 1.4F;

   /** \action
   * Call tested function
   */
   float32_t actual_result = F360_Huber_Weight(residual, tuning_constant);

   /** \result
   * Check for valid output value
   */
   float32_t expected_result = 0.7F;
   DOUBLES_EQUAL(expected_result, actual_result, TEST_PASS_TH_SMALL);
}

/** \purpose
* Function should return one for tuning_constant greater than positive residual
* \req   NA
*/
TEST(f360_math_func, F360_Huber_Weight_Should_Return_One_For_Tuning_Constant_Greater_Than_Absolute_Value_Of_Negative_Residual)
{
   /** \precond
   * Set tuning_constant to value greater than absolute value of negative residual
   */
   const float32_t residual = -0.1F;
   const float32_t tuning_constant = 0.2F;

   /** \action
   * Call tested function
   */
   float32_t actual_result = F360_Huber_Weight(residual, tuning_constant);

   /** \result
   * Function result should be equal to one
   */
   float32_t expected_result = 1.0F;
   DOUBLES_EQUAL(expected_result, actual_result, TEST_PASS_TH_SMALL);
}

/** \purpose
*  Function should return valid result for tuning_constant smaller than absolute value of negative residual
* \req   NA
*/
TEST(f360_math_func, F360_Huber_Weight_Should_Return_Valid_Result_For_Tuning_Constant_Smaller_Than_Absolute_Value_Of_Negative_Residual)
{
   /** \precond
   * Set tuning_constant smaller than absolute value of negative residual
   */
   const float32_t residual = -2.0F;
   const float32_t tuning_constant = 1.4F;

   /** \action
   * Call tested function
   */
   float32_t actual_result = F360_Huber_Weight(residual, tuning_constant);

   /** \result
   * Check for valid output value
   */
   float32_t expected_result = 0.7F;
   DOUBLES_EQUAL(expected_result, actual_result, TEST_PASS_TH_SMALL);
}

/** \purpose
*  Function should return zero for both input parameters equal to zero
* \req   NA
*/
TEST(f360_math_func, F360_Huber_Weight_Should_Return_Zero_For_Both_Input_Values_Equal_To_Zero)
{
   /** \precond
   * Set both input value to zero
   */
   const float32_t residual = 0.0F;
   const float32_t tuning_constant = 0.0F;

   /** \action
   * Call tested function
   */
   float32_t actual_result = F360_Huber_Weight(residual, tuning_constant);

   /** \result
   * Check for valid output value
   */
   float32_t expected_result = 0.0F;
   DOUBLES_EQUAL(expected_result, actual_result, TEST_PASS_TH_SMALL);
}

/** \purpose
*  Function should return one for both residual equal to zero and non-zero tuning_constant 
* \req   NA
*/
TEST(f360_math_func, F360_Huber_Weight_Should_Return_One_For_Residual_Equal_To_Zero_And_Non_Zero_Tuning_Constant)
{
   /** \precond
   * Set residual equal to zero and tuning_constant to non_zero value
   */
   const float32_t residual = 0.0F;
   const float32_t tuning_constant = 2.0F;

   /** \action
   * Call tested function
   */
   float32_t actual_result = F360_Huber_Weight(residual, tuning_constant);

   /** \result
   * Check for valid output value
   */
   float32_t expected_result = 1.0F;
   DOUBLES_EQUAL(expected_result, actual_result, TEST_PASS_TH_SMALL);
}

/** \purpose
*  Test that squared distance from a point to a line without any slope is calculated correctly
* \req   NA
*/
TEST(f360_math_func, Compute_Dist_From_Point_To_Line_Squared_Line_No_Slope)
{
   /** \precond
   * Define a point to evaluate and create a line without any slope
   */
   const float32_t x = 3.0F;
   const float32_t y = 3.0F;
   const float32_t k = 0.0F;
   const float32_t m = 2.0F;

   /** \action
   * Call tested function
   */
   float32_t actual_result = Compute_Dist_From_Point_To_Line_Squared(x, y, k, m);

   /** \result
   * Check for valid output value
   */
   float32_t expected_result = 1.0F;
   DOUBLES_EQUAL(expected_result, actual_result, F360_EPSILON);
}

/** \purpose
*  Test that squared distance from point to a line with non-zero slope is calculated correctly
* \req   NA
*/
TEST(f360_math_func, Compute_Dist_From_Point_To_Line_Squared_Line_Non_Zero_Slope)
{
   /** \precond
   * Define a point to evaluate and create a line with non-zero slope
   */
   const float32_t x = 3.0F;
   const float32_t y = 3.0F;
   const float32_t k = 1.0F;
   const float32_t m = 2.0F;

   /** \action
   * Call tested function
   */
   float32_t actual_result = Compute_Dist_From_Point_To_Line_Squared(x, y, k, m);

   /** \result
   * Check for valid output value
   */
   float32_t expected_result = 2.0F;
   DOUBLES_EQUAL(expected_result, actual_result, F360_EPSILON);
}

/** \purpose
*  Test that projection of point on a line without any slope is calculated correctly
* \req   NA
*/
TEST(f360_math_func, Compute_Projection_Of_Point_On_Line_Line_With_No_Slope)
{
   /** \precond
   * Define a point to evaluate and create a line without a slope
   */
   const float32_t x = 3.0F;
   const float32_t y = 3.0F;
   const float32_t k = 0.0F;
   const float32_t m = -2.0F;

   /** \action
   * Call tested function
   */
   float32_t actual_proj_x;
   float32_t actual_proj_y;
   Compute_Projection_Of_Point_On_Line(x, y, k, m, actual_proj_x, actual_proj_y);

   /** \result
   * Check for valid output value
   */
   float32_t expected_proj_x = 3.0F;
   float32_t expected_proj_y = -2.0F;
   DOUBLES_EQUAL(expected_proj_x, actual_proj_x, F360_EPSILON);
   DOUBLES_EQUAL(expected_proj_y, actual_proj_y, F360_EPSILON);
}

/** \purpose
*  Test that projection of point on a line with non-zero slope is calculated correctly
* \req   NA
*/
TEST(f360_math_func, Compute_Projection_Of_Point_On_Line_Line_With_Non_Zero_Slope)
{
   /** \precond
   * Define a point to evaluate and create a line with non-zero slope
   */
   const float32_t x = 3.0F;
   const float32_t y = 3.0F;
   const float32_t k = -1.0F;
   const float32_t m = 2.0F;

   /** \action
   * Call tested function
   */
   float32_t actual_proj_x;
   float32_t actual_proj_y;
   Compute_Projection_Of_Point_On_Line(x, y, k, m, actual_proj_x, actual_proj_y);

   /** \result
   * Check for valid output value
   */
   float32_t expected_proj_x = 1.0F;
   float32_t expected_proj_y = 1.0F;
   DOUBLES_EQUAL(expected_proj_x, actual_proj_x, F360_EPSILON);
   DOUBLES_EQUAL(expected_proj_y, actual_proj_y, F360_EPSILON);
}


/** \purpose
*  Test that conversion from finite vector to linear
*  equation works as expected when we expect a 
*  positive slope with no offset.
* \req   NA
*/
TEST(f360_math_func, Get_Vector_As_Linear_Equation_Positive_K_Zero_M)
{
   /** \precond
   * Define a finite vector
   * Initialize resulting k and m variables
   */
   const float32_t x_start = 0.0F;
   const float32_t y_start = 0.0F;
   const float32_t x_end = 3.0F;
   const float32_t y_end = 6.0F;

   float32_t k;
   float32_t m;

   /** \action
   * Call function
   */
   Get_Vector_As_Linear_Equation(x_start, y_start, x_end, y_end, k, m);

   /** \result
   * Check if result match to expected value.
   */
   DOUBLES_EQUAL(2.0F, k, F360_EPSILON);
   DOUBLES_EQUAL(0.0F, m, F360_EPSILON);
}

/** \purpose
*  Test that conversion from finite vector to linear
*  equation works as expected when we expect a
*  negative slope with positive offset.
* \req   NA
*/
TEST(f360_math_func, Get_Vector_As_Linear_Equation_Negative_K_Positive_M)
{
   /** \precond
   * Define a finite vector
   * Initialize resulting k and m variables
   */
   const float32_t x_start = 0.0F;
   const float32_t y_start = 1.0F;
   const float32_t x_end = 3.0F;
   const float32_t y_end = -5.0F;

   float32_t k;
   float32_t m;

   /** \action
   * Call function
   */
   Get_Vector_As_Linear_Equation(x_start, y_start, x_end, y_end, k, m);

   /** \result
   * Check if result match to expected value.
   */
   DOUBLES_EQUAL(-2.0F, k, F360_EPSILON);
   DOUBLES_EQUAL(1.0F, m, F360_EPSILON);
}

/** \purpose
*  Test that conversion from finite vector to linear
*  equation works as expected when x start and end
*  points are almost equal.
* \req   NA
*/
TEST(f360_math_func, Get_Vector_As_Linear_Equation_Negative_Infinite_K_Zero_M)
{
   /** \precond
   * Define a finite vector
   * Initialize resulting k and m variables
   */
   const float32_t x_start = 0.0F;
   const float32_t y_start = 0.0F;
   const float32_t x_end = -0.5F * F360_EPSILON;
   const float32_t y_end = 5.0F;

   float32_t k;
   float32_t m;

   /** \action
   * Call function
   */
   Get_Vector_As_Linear_Equation(x_start, y_start, x_end, y_end, k, m);

   /** \result
   * Check if result match to expected value.
   */
   DOUBLES_EQUAL(-4.201681e+7F, k, F360_EPSILON);
   DOUBLES_EQUAL(0.0F, m, F360_EPSILON);
}

/** \purpose
*  Test that conversion from finite vector to linear
*  equation works as expected when x start and end
*  points are almost equal.
* \req   NA
*/
TEST(f360_math_func, Get_Vector_As_Linear_Equation_Positive_Infinite_K_Positive_M)
{
   /** \precond
   * Define a finite vector
   * Initialize resulting k and m variables
   */
   const float32_t x_start = 0.0F;
   const float32_t y_start = 1.0F;
   const float32_t x_end = 0.5F * F360_EPSILON;
   const float32_t y_end = 5.0F;

   float32_t k;
   float32_t m;

   /** \action
   * Call function
   */
   Get_Vector_As_Linear_Equation(x_start, y_start, x_end, y_end, k, m);

   /** \result
   * Check if result match to expected value.
   */
   DOUBLES_EQUAL(33613445.3782F, k, F360_EPSILON);
   DOUBLES_EQUAL(1.0F, m, F360_EPSILON);
}

/** \purpose
*  Test finding of intersection between two lines
*  works as expected when the lines are parallel
* \req   NA
*/
TEST(f360_math_func, Find_X_Intersect_Between_Two_Lines_Parallel_Lines)
{
   /** \precond
   * Define two parallel lines
   * Initialize resulting x intersection varaible
   */
   const float32_t k1 = 0.0F;
   const float32_t m1 = 1.0F;
   const float32_t k2 = 0.0F;
   const float32_t m2 = 5.0F;

   float32_t x_int;

   /** \action
   * Call function
   */
   bool f_intersect_found = Find_X_Intersect_Between_Two_Lines(k1, m1, k2, m2, x_int);

   /** \result
   * Check if result match to expected value.
   */
   CHECK_FALSE(f_intersect_found);
   DOUBLES_EQUAL(INFTY, x_int, F360_EPSILON);
}

/** \purpose
*  Test finding of intersection between two lines
*  works as expected when an intersection exists
* \req   NA
*/
TEST(f360_math_func, Find_X_Intersect_Between_Two_Lines_Intersection_Exists)
{
   /** \precond
   * Define two lines with different slopes and offset
   * Initialize resulting x intersection varaible
   */
   const float32_t k1 = 0.0F;
   const float32_t m1 = 1.0F;
   const float32_t k2 = 100.0F;
   const float32_t m2 = 5.0F;

   float32_t x_int;

   /** \action
   * Call function
   */
   bool f_intersect_found = Find_X_Intersect_Between_Two_Lines(k1, m1, k2, m2, x_int);

   /** \result
   * Check if result match to expected value.
   */
   CHECK_TRUE(f_intersect_found);
   DOUBLES_EQUAL(-0.0399999991F, x_int, F360_EPSILON);
}

/** \purpose
*  Test finding of intersection between a line and 2 degree
*  polynomial works correctly when the intersection is
*  imaginary.
* \req   NA
*/
TEST(f360_math_func, Find_X_Intersect_Between_Line_And_2_Deg_Poly_Imaginary_Root)
{
   /** \precond
   * Define a polynomial and a line
   * Initialize resulting x intersection varaibles
   */
   const float32_t a = 1.0F;
   const float32_t b = 1.0F;
   const float32_t c = 1.0F;
   const float32_t k = 1.0F;
   const float32_t m = 0.0F;

   float32_t x1;
   float32_t x2;

   /** \action
   * Call function
   */
   bool f_intersect_found = Find_X_Intersect_Between_Line_And_2_Deg_Poly(a, b, c, k, m, x1, x2);

   /** \result
   * Check if result match to expected value.
   */
   CHECK_FALSE(f_intersect_found);
}

/** \purpose
*  Test finding of intersection between a line and 2 degree
*  polynomial works correctly when the a-coefficient is
*  very small and negative. When a-coefficient is very small
*  we get floating point precision errors and even though
*  we expect x = 1 as a solution we actually get 0.
* \req   NA
*/
TEST(f360_math_func, Find_X_Intersect_Between_Line_And_2_Deg_Poly_Small_Negative_A_Coefficient)
{
   /** \precond
   * Define a polynomial and a line
   * Initialize resulting x intersection varaibles
   */
   const float32_t a = -0.5F * F360_EPSILON;
   const float32_t b = 0.0F;
   const float32_t c = 1.0F;
   const float32_t k = 1.0F;
   const float32_t m = 0.0F;

   float32_t x1;
   float32_t x2;

   /** \action
   * Call function
   */
   bool f_intersect_found = Find_X_Intersect_Between_Line_And_2_Deg_Poly(a, b, c, k, m, x1, x2);

   /** \result
   * Check if result match to expected value.
   */
   CHECK_TRUE(f_intersect_found);
   DOUBLES_EQUAL(-8403362.0F, x1, F360_EPSILON);
   DOUBLES_EQUAL(1.0F, x2, F360_EPSILON);
}

/** \purpose
*  Test finding of intersection between a line and 2 degree
*  polynomial works correctly when the a-coefficient is
*  very small and negative. When a-coefficient is very small
*  we get floating point precision errors and even though
*  we expect x = 1 as a solution we actually get 0.
* \req   NA
*/
TEST(f360_math_func, Find_X_Intersect_Between_Line_And_2_Deg_Poly_Small_Positive_A_Coefficient)
{
   /** \precond
   * Define a polynomial and a line
   * Initialize resulting x intersection varaibles
   */
   const float32_t a = 0.5F * F360_EPSILON;
   const float32_t b = 0.0F;
   const float32_t c = 1.0F;
   const float32_t k = 1.0F;
   const float32_t m = 0.0F;

   float32_t x1;
   float32_t x2;

   /** \action
   * Call function
   */
   bool f_intersect_found = Find_X_Intersect_Between_Line_And_2_Deg_Poly(a, b, c, k, m, x1, x2);

   /** \result
   * Check if result match to expected value.
   */
   CHECK_TRUE(f_intersect_found);
   DOUBLES_EQUAL(1.0F, x1, F360_EPSILON); // In VS this returns as 0.0
   DOUBLES_EQUAL(8403360.0F, x2, F360_EPSILON);
}

/** \purpose
*  Test finding of intersection between a line and 2 degree
*  polynomial works correctly when the a-coefficient is
*  -1, b-coefficient is 0 and c-coefficient is 1.
*  The line has a slope of 1.
* \req   NA
*/
TEST(f360_math_func, Find_X_Intersect_Between_Line_And_2_Deg_Poly_A_Negative_One_B_Zero_Line_Small_Slope)
{
   /** \precond
   * Define a polynomial and a line
   * Initialize resulting x intersection varaibles
   */
   const float32_t a = -1.0F;
   const float32_t b = 0.0F;
   const float32_t c = 1.0F;
   const float32_t k = 1.0F;
   const float32_t m = 0.0F;

   float32_t x1;
   float32_t x2;

   /** \action
   * Call function
   */
   bool f_intersect_found = Find_X_Intersect_Between_Line_And_2_Deg_Poly(a, b, c, k, m, x1, x2);

   /** \result
   * Check if result match to expected value.
   */
   CHECK_TRUE(f_intersect_found);
   DOUBLES_EQUAL(-1.61803401F, x1, F360_EPSILON);
   DOUBLES_EQUAL(0.618034005F, x2, F360_EPSILON);
}

/** \purpose
*  Test finding of intersection between a line and 2 degree
*  polynomial works correctly when the a-coefficient is
*  -1, b-coefficient is 0 and c-coefficient is 1.
*  The line has a large slope of 1000 meaning that
*  we expect one of the intersections close to zero.
* \req   NA
*/
TEST(f360_math_func, Find_X_Intersect_Between_Line_And_2_Deg_Poly_A_Negative_One_B_Zero_Line_Large_Slope)
{
   /** \precond
   * Define a polynomial and a line
   * Initialize resulting x intersection varaibles
   */
   const float32_t a = -1.0F;
   const float32_t b = 0.0F;
   const float32_t c = 1.0F;
   const float32_t k = 1000.0F;
   const float32_t m = 0.0F;

   float32_t x1;
   float32_t x2;

   /** \action
   * Call function
   */
   bool f_intersect_found = Find_X_Intersect_Between_Line_And_2_Deg_Poly(a, b, c, k, m, x1, x2);

   /** \result
   * Check if result match to expected value.
   */
   CHECK_TRUE(f_intersect_found);
   DOUBLES_EQUAL(-1000.00098F, x1, F360_EPSILON);
   DOUBLES_EQUAL(0.00100708008F, x2, F360_EPSILON);
}

/** @}*/



/** \defgroup  is_in_bounds
 *  @{
**/

/** \brief
 *  Test group of Is_In_Bounds function. Tests verify whether function properly analyses whether values are in bounds.
 **/
TEST_GROUP(f360_is_in_bounds)
{

};

/** \purpose
*  Purpose of this test is to verify whether for float variables function properly determines that variable is below min threshold
* \req   
* NA
*/
TEST(f360_is_in_bounds, Is_In_Bounds__Float_Is_Below_Lower_Threshold)
{
   /** \precond
   * Set up tested variable to below lower threshold
   * Set up limits
   */
   const float variable = 1.0F;
   const float min_val = 2.0F;
   const float max_val = 5.0F;

   /** \action
   * Call Is_In_Bounds
   */
   const bool result = Is_In_Bounds<float>(variable, min_val, max_val);

   /** \result
   * Check if result is false
   */
   CHECK_FALSE(result);
}

/** \purpose
*  Purpose of this test is to verify whether for float variables function properly determines that variable is in bounds
* \req
* NA
*/
TEST(f360_is_in_bounds, Is_In_Bounds__Float_Is_In_Bouds)
{
   /** \precond
   * Set up tested variable to be in bounds
   * Set up limits
   */
   const float variable = 4.0F;
   const float min_val = 2.0F;
   const float max_val = 5.0F;

   /** \action
   * Call Is_In_Bounds
   */
   const bool result = Is_In_Bounds<float>(variable, min_val, max_val);

   /** \result
   * Check if result is false
   */
   CHECK_TRUE(result);
}

/** \purpose
*  Purpose of this test is to verify whether for float variables function properly determines that variable is above max threshold
* \req
* NA
*/
TEST(f360_is_in_bounds, Is_In_Bounds__Float_Is_Above_Upper_Threshold)
{
   /** \precond
   * Set up tested variable to be above upper threshold
   * Set up limits
   */
   const float variable = 6.0F;
   const float min_val = 2.0F;
   const float max_val = 5.0F;

   /** \action
   * Call Is_In_Bounds
   */
   const bool result = Is_In_Bounds<float>(variable, min_val, max_val);

   /** \result
   * Check if result is false
   */
   CHECK_FALSE(result);
}

/** \purpose
*  Purpose of this test is to verify whether for int variables function properly determines that variable is below min threshold
* \req
* NA
*/
TEST(f360_is_in_bounds, Is_In_Bounds__Int_Is_Below_Lower_Threshold)
{
   /** \precond
   * Set up tested variable to below lower threshold
   * Set up limits
   */
   const int variable = 1;
   const int min_val = 2;
   const int max_val = 5;

   /** \action
   * Call Is_In_Bounds
   */
   const bool result = Is_In_Bounds<int>(variable, min_val, max_val);

   /** \result
   * Check if result is false
   */
   CHECK_FALSE(result);
}

/** \purpose
*  Purpose of this test is to verify whether for int variables function properly determines that variable is in bounds
* \req
* NA
*/
TEST(f360_is_in_bounds, Is_In_Bounds__Int_Is_In_Bouds)
{
   /** \precond
   * Set up tested variable to be in bounds
   * Set up limits
   */
   const int variable = 4;
   const int min_val = 2;
   const int max_val = 5;

   /** \action
   * Call Is_In_Bounds
   */
   const bool result = Is_In_Bounds<int>(variable, min_val, max_val);

   /** \result
   * Check if result is true
   */
   CHECK_TRUE(result);
}

/** \purpose
*  Purpose of this test is to verify whether for int variables function properly determines that variable is above max threshold
* \req
* NA
*/
TEST(f360_is_in_bounds, Is_In_Bounds__Int_Is_Above_Upper_Threshold)
{
   /** \precond
   * Set up tested variable to be above upper threshold
   * Set up limits
   */
   const int variable = 6;
   const int min_val = 2;
   const int max_val = 5;

   /** \action
   * Call Is_In_Bounds
   */
   const bool result = Is_In_Bounds<int>(variable, min_val, max_val);

   /** \result
   * Check if result is false
   */
   CHECK_FALSE(result);
}

/** \purpose
*  Purpose of this test is to verify whether for int variables function properly determines that variable is within bounds when at lower limit
* \req
* NA
*/
TEST(f360_is_in_bounds, Is_In_Bounds__Int_Is_Within_Bounds_When_At_Lower_Limit)
{
   /** \precond
   * Set up tested variable to be at lower bound
   * Set up limits
   */
   const int variable = 2;
   const int min_val = 2;
   const int max_val = 5;

   /** \action
   * Call Is_In_Bounds
   */
   const bool result = Is_In_Bounds<int>(variable, min_val, max_val);

   /** \result
   * Check if result is true
   */
   CHECK_TRUE(result);
}

/** \purpose
*  Purpose of this test is to verify whether for int variables function properly determines that variable is outside bounds when at upper limit
* \req
* NA
*/
TEST(f360_is_in_bounds, Is_In_Bounds__Int_Is_Outside_Bounds_When_At_Upper_Limit)
{
   /** \precond
   * Set up tested variable to be at upper bound
   * Set up limits
   */
   const int variable = 5;
   const int min_val = 2;
   const int max_val = 5;

   /** \action
   * Call Is_In_Bounds
   */
   const bool result = Is_In_Bounds<int>(variable, min_val, max_val);

   /** \result
   * Check if result is false
   */
   CHECK_FALSE(result);
}


/** @}*/



/** \defgroup  clamp
 *  @{
**/

/** \brief
 *  Test group of Clamp function. Tests verify whether function properly clamps variable into provided limits
 **/
TEST_GROUP(clamp)
{

};

/** \purpose
* Purpose of this test is to verify whether for float variables function properly modifies variable if it is below lower bound
* \req
* NA
*/
TEST(clamp, Clamp__Float_Below_Lower_Limit)
{
   /** \precond
   * Set up tested variable to below lower threshold
   * Set up limits
   */
   const float variable = 1.0F;
   const float min_val = 2.0F;
   const float max_val = 5.0F;

   /** \action
   * Call Clamp
   */
   const float result = Clamp<float>(variable, min_val, max_val);

   /** \result
   * Check whether function returned min_val
   */
   DOUBLES_EQUAL(min_val, result, F360_EPSILON);
}

/** \purpose
*  Purpose of this test is to verify whether for float variables function properly does not modify variable if it is in bounds
* \req
* NA
*/
TEST(clamp, Clamp__Float_In_Limits)
{
   /** \precond
   * Set up tested variable to be in bounds
   * Set up limits
   */
   const float variable = 4.0F;
   const float min_val = 2.0F;
   const float max_val = 5.0F;

   /** \action
   * Call Clamp
   */
   const float result = Clamp<float>(variable, min_val, max_val);

   /** \result
   * Check whether function returned variable
   */
   DOUBLES_EQUAL(variable, result, F360_EPSILON);
}

/** \purpose
* Purpose of this test is to verify whether for float variables function properly modifies variable if it is above upper bound
* \req
* NA
*/
TEST(clamp, Clamp__Float_Above_Limit)
{
   /** \precond
   * Set up tested variable to be above upper threshold
   * Set up limits
   */
   const float variable = 6.0F;
   const float min_val = 2.0F;
   const float max_val = 5.0F;

   /** \action
   * Call Clamp
   */
   const float result = Clamp<float>(variable, min_val, max_val);

   /** \result
   * Check whether function returned max_val
   */
   DOUBLES_EQUAL(max_val, result, F360_EPSILON);
}

/** \purpose
* Purpose of this test is to verify whether for int variables function properly modifies variable if it is below lower bound
* \req
* NA
*/
TEST(clamp, Clamp__Int_Below_Lower_Limit)
{
   /** \precond
   * Set up tested variable to below lower threshold
   * Set up limits
   */
   const int variable = 1;
   const int min_val = 2;
   const int max_val = 5;

   /** \action
   * Call Clamp
   */
   const int result = Clamp<int>(variable, min_val, max_val);

   /** \result
   * Check whether function returned min_val
   */
   CHECK_EQUAL(min_val, result);
}

/** \purpose
*  Purpose of this test is to verify whether for int variables function properly does not modify variable if it is in bounds
* \req
* NA
*/
TEST(clamp, Clamp__Int_In_Limits)
{
   /** \precond
   * Set up tested variable to be in bounds
   * Set up limits
   */
   const int variable = 4;
   const int min_val = 2;
   const int max_val = 5;

   /** \action
   * Call Clamp
   */
   const int result = Clamp<int>(variable, min_val, max_val);

   /** \result
   * Check whether function returned variable
   */
   CHECK_EQUAL(variable, result);
}

/** \purpose
* Purpose of this test is to verify whether for int variables function properly modifies variable if it is above upper bound
* \req
* NA
*/
TEST(clamp, Clamp__Int_Above_Limit)
{
   /** \precond
   * Set up tested variable to be above upper threshold
   * Set up limits
   */
   const int variable = 6;
   const int min_val = 2;
   const int max_val = 5;

   /** \action
   * Call Clamp
   */
   const int result = Clamp<int>(variable, min_val, max_val);

   /** \result
   * Check whether function returned max_val
   */
   CHECK_EQUAL(max_val, result);
}
/** @}*/

/** \defgroup  f360_math_func
 *  @{
**/

/** \brief
 *  Tests for f360_math_func
 **/
TEST_GROUP(F360_Max_Index__functionality)
{

};

/**
 *\purpose  Check if max element is found orrectly.
 *\req    NA
 */
TEST(F360_Max_Index__functionality, one_max_value)
{
   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float v[5] = { 1.0f, 4.0f, -3.0f, 5.0f, 2.0f };
   /** \action
    *Call function.
    **/
   unsigned int max_idx = F360_Max_Index(v);
   /** \result
    *Ensure function output is equal to the expected result.
    **/
   CHECK_EQUAL(3U, max_idx);
}

/**
 *\purpose  Check if max element is found correctly.
 *\req    NA
 */
TEST(F360_Max_Index__functionality, more_than_one_max_value)
{
   /** \precond
    *Set up float values to operate on and compute expected result.
    **/
   float v[5] = { 1.0f, 4.0f, 5.0f, 5.0f, 2.0f };
   /** \action
    *Call function.
    **/
   unsigned int max_idx = F360_Max_Index(v);
   /** \result
    *Ensure function output is equal to the expected result.
    **/
   CHECK_EQUAL(2U, max_idx);
}

/**
 *\purpose  Check if max element is found orrectly.
 *\req    NA
 */
TEST(F360_Max_Index__functionality, all_max_values)
{
   /** \precond 
    *Set up float values to operate on and compute expected result.
    **/
   float v[5] = { 5.0f, 5.0f, 5.0f, 5.0f, 5.0f };
   /** \action
    *Call function.
    **/
   unsigned int max_idx = F360_Max_Index(v);
   /** \result
    *Ensure function output is equal to the expected result.
    **/
   CHECK_EQUAL(0U, max_idx);
}
/** @}*/

/** \defgroup  F360_Hysteresis
 *  @{
**/

/** \brief
 *  Tests for F360_Hysteresis
 **/
TEST_GROUP(F360_Hysteresis)
{
   float32_t input_value = 0.5F;
   bool f_hysteresis_state = true;
   float32_t activation_threshold = 0.6F;
   float32_t deactivation_threshold = 0.4F;
};

/**
 *\purpose  Check if state is properly deactivated when value is lower that lower threshold and initial state is true.
 *\req    NA
 */
TEST(F360_Hysteresis, F360_Hysteresis_Deactivate)
{
   /** \precond
    *Set input value to be just below deactivation threshold. Initial state is true.
    **/
   input_value = 0.39F;

   /** \action
    *Call function.
    **/
   bool f_hysteresis_output = F360_Hysteresis(input_value, f_hysteresis_state, activation_threshold, deactivation_threshold);

   /** \result
    *Ensure the state is switched to false.
    **/
   CHECK_FALSE(f_hysteresis_output);
}

/**
 *\purpose  Check if state is maintained when initial state is true and input value is between thresholds.
 *\req    NA
 */
TEST(F360_Hysteresis, F360_Hysteresis_Maintain_True_State)
{
   /** \precond
    *Input value is above deactivation threshold and below activation threshold. Initial state is true.
    **/

   /** \action
    *Call function.
    **/
   bool f_hysteresis_output = F360_Hysteresis(input_value, f_hysteresis_state, activation_threshold, deactivation_threshold);

   /** \result
    *Ensure the state is maintained as true.
    **/
   CHECK_TRUE(f_hysteresis_output);
}

/**
 *\purpose  Check if state is properly activated when value is higher than upper threshold.
 *\req    NA
 */
TEST(F360_Hysteresis, F360_Hysteresis_Activate)
{
   /** \precond
    *Set input value to be just above activation threshold and initial state to false.
    **/
   input_value = 0.61F;
   f_hysteresis_state = false;

   /** \action
    *Call function.
    **/
   bool f_hysteresis_output = F360_Hysteresis(input_value, f_hysteresis_state, activation_threshold, deactivation_threshold);

   /** \result
    *Ensure the state is switched to true.
    **/
   CHECK_TRUE(f_hysteresis_output);
}

/**
 *\purpose  Check if state is maintained when initial state is false and input value is between thresholds.
 *\req    NA
 */
TEST(F360_Hysteresis, F360_Hysteresis_Maintain_False_State)
{
   /** \precond
    *Input value is above deactivation threshold and below activation threshold.
    **/
   f_hysteresis_state = false;

   /** \action
    *Call function.
    **/
   bool f_hysteresis_output = F360_Hysteresis(input_value, f_hysteresis_state, activation_threshold, deactivation_threshold);

   /** \result
    *Ensure the state is maintained as false.
    **/
   CHECK_FALSE(f_hysteresis_output);
}
/** @}*/
