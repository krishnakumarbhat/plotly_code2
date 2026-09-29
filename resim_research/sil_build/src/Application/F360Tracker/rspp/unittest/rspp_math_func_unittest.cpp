/** \file
 *   Tests for rspp_math_func
 */

#include "rspp_math_func.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>
#include <algorithm>
#include <string.h>
#include <math.h>
#include <cstdio>
#include "rspp_iterator.h"
#include "rspp_constants.h"

using namespace rspp_variant_A;
// sneak in mocked functions

// Declaration of stubbed/mock functions
// Implementation of stubbed interfaces

/** \defgroup  rspp_math_func
 *  @{
 **/

/** \brief
 * Tests for RSPP math utility functions including hypotenuse calculation,
 * matrix multiplication, saturation, sorting, and piecewise linear equations.
 */
TEST_GROUP(rspp_math_func)
{
   // Float comparison thresholds for passing tests
   const float32_t TEST_PASS_TH_LARGE = 1e-3f;
   const float32_t TEST_PASS_TH_MID = 1e-5f;
   const float32_t TEST_PASS_TH_SMALL = FLT_EPSILON;

   /** \setup
    * No specific setup required for this test group.
    */
   TEST_SETUP()
   {
      // nothing to set up;
   }

   /** \teardown
    * No cleanup required for this test group.
    */
   TEST_TEARDOWN()
   {
      // mock.clear();
   }
};

/** \purpose
 * Check if RSPP_Get_Hypotenuse_Squared returns the correct value when inputs are negative.
 * \req
 * NA
 */
TEST(rspp_math_func, test_f360_get_hypotenuse_squared_negative_input)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float32_t values to operate on and compute expected result.
    **/
   float32_t f1 = -3.0f;
   float32_t f2 = -4.0f;
   float32_t expected = 25.0f;
   float32_t result;

   /** \action
    *Call function.
    **/
   result = RSPP_Get_Hypotenuse_Squared(f1, f2);

   /** \result
    *Ensure function output is equal to the expected result.
    **/

   DOUBLES_EQUAL(expected, result, TEST_PASS_TH_SMALL);
}

/** \purpose
 * Check if RSPP_Get_Hypotenuse_Squared returns the correct value when inputs are positive.
 * \req
 * NA
 */
TEST(rspp_math_func, test_f360_get_hypotenuse_squared_positive_input)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float32_t values to operate on and compute expected result.
    **/
   float32_t f1 = 3.0f;
   float32_t f2 = 4.0f;
   float32_t expected = 25.0f;
   float32_t result;

   /** \action
    *Call function.
    **/
   result = RSPP_Get_Hypotenuse_Squared(f1, f2);

   /** \result
    *Ensure function output is equal to the expected result.
    **/

   DOUBLES_EQUAL(expected, result, TEST_PASS_TH_SMALL);
}

/** \purpose
 * Check if RSPP_Get_Hypotenuse_Squared returns the correct value when inputs are zero.
 * \req
 * NA
 */
TEST(rspp_math_func, test_f360_get_hypotenuse_squared_zero_input)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float32_t values to operate on and compute expected result.
    **/
   float32_t f1 = 0.0f;
   float32_t f2 = 0.0f;
   float32_t expected = 0.0f;
   float32_t result;

   /** \action
    *Call function.
    **/
   result = RSPP_Get_Hypotenuse_Squared(f1, f2);

   /** \result
    *Ensure function output is equal to the expected result.
    **/

   DOUBLES_EQUAL(expected, result, TEST_PASS_TH_SMALL);
}

/** \purpose
 * Check if function returns the correct matrix.
 * \req
 * NA
 */
TEST(rspp_math_func, RSPP_Matmul_MxN_NxP)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float32_t values to operate on and compute expected result.
    **/
   float32_t M1[2][3] = {{3.44470f, 38.15590f, 79.52000f}, {43.87450f, 76.55170f, 18.68730f}};
   float32_t M2[3][2] = {{48.97650f, 44.55870f}, {64.63140f, 70.93650f}, {75.46870f, 27.60260f}};
   float32_t expected1[2][2] = {{8636.04960881f, 5055.09610624f}, {8506.76923014f, 7901.11841718f}};
   float32_t result1[2][2];

   /** \action
    *Call function.
    **/
   RSPP_Matmul_MxN_NxP(M1, M2, result1);

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
    *Set up float32_t values to operate on and compute expected result
    **/
   float32_t M3[3][4] = {{3.44470f, 38.15590f, 79.52000f, 0.0f}, {43.87450f, 76.55170f, 18.68730f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f}};
   float32_t M4[4][5] = {{48.97650f, 44.55870f, 0.0f, 0.0f, 0.0f}, {64.63140f, 70.93650f, 0.0f, 0.0f, 0.0f}, {75.46870f, 27.60260f, 0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f, 0.0f, 0.0f}};
   float32_t expected2[2][2] = {{8636.04960881f, 5055.09610624f}, {8506.76923014f, 7901.11841718f}};
   float32_t result2[3][5];

   /** \action
    *Call function.
    **/
   RSPP_Matmul_MxN_NxP(M3, M4, result2, 2U, 3U, 3U);

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

/** \purpose
 * Check if function returns the correct matrix.
 * \req
 * NA
 */
TEST(rspp_math_func, RSPP_Matmul_MxN_NxP_Transpose)
{
   /** \step{1}
    *Compare function output to expected output using full matrices.
    **/

   /** \precond
    *Set up float32_t values to operate on and compute expected result.
    **/
   float32_t M1_full[2][3] = {{3.44470f, 38.15590f, 79.52000f}, {43.87450f, 76.55170f, 18.68730f}};
   float32_t M2_full[2][3] = {{48.97650f, 64.63140f, 75.46870f}, {44.55870f, 70.93650f, 27.60260f}};
   float32_t expected_full[2][2] = {{8636.04960881f, 5055.09610624f}, {8506.76923014f, 7901.11841718f}};
   float32_t result_full[2][2];

   /** \action
    *Call function.
    **/
   RSPP_Matmul_MxN_PxN_Transpose(M1_full, M2_full, result_full);

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
    *Set up float32_t values to operate on and compute expected result.
    **/
   float32_t M1_empty[27][66];
   M1_empty[0][0] = 3.44470f;
   M1_empty[0][1] = 38.15590f;
   M1_empty[0][2] = 79.52000f;
   M1_empty[1][0] = 43.87450f;
   M1_empty[1][1] = 76.55170f;
   M1_empty[1][2] = 18.68730f;
   float32_t M2_empty[86][66];
   M2_empty[0][0] = 48.97650f;
   M2_empty[0][1] = 64.63140f;
   M2_empty[0][2] = 75.46870f;
   M2_empty[1][0] = 44.55870f;
   M2_empty[1][1] = 70.93650f;
   M2_empty[1][2] = 27.60260f;
   float32_t expected_empty[24][43];
   expected_empty[0][0] = 8636.04960881f;
   expected_empty[0][1] = 5055.09610624f;
   expected_empty[1][0] = 8506.76923014f;
   expected_empty[1][1] = 7901.11841718f;
   float32_t result_empty[27][86];

   /** \action
    *Call function.
    **/
   RSPP_Matmul_MxN_PxN_Transpose(M1_empty, M2_empty, result_empty,
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

/** \purpose
 * Check if function returns the correct value.
 * \req
 * NA
 */
TEST(rspp_math_func, RSPP_Saturate)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float32_t values to operate on and compute expected result.
    **/
   float32_t max = 10.0f;
   float32_t min = -10.0f;
   float32_t result1;
   float32_t result2;
   float32_t result3;

   /** \action
    *Call function.
    **/
   result1 = RSPP_Saturate(1.0f, min, max);
   result2 = RSPP_Saturate(11.0f, min, max);
   result3 = RSPP_Saturate(-11.0f, min, max);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL(1.0f, result1, TEST_PASS_TH_SMALL);
   DOUBLES_EQUAL(max, result2, TEST_PASS_TH_SMALL);
   DOUBLES_EQUAL(min, result3, TEST_PASS_TH_SMALL);
}

/** \purpose
 * Check that function is not sorting and return false when asked to sort more values than elements in array.
 * \req
 * NA
 */
TEST(rspp_math_func, RSPP_Sort_fail)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float32_t values to operate on. Expected values are identical as no change is expected.
    **/
   const unsigned int length = 5;
   float32_t v[length] = {1.0f, 4.0f, 3.0f, 5.0f, 2.0f};
   unsigned int idx_arr[length] = {1U, 2U, 3U, 4U, 5U};

   float32_t v_expected[length] = {1.0f, 4.0f, 3.0f, 5.0f, 2.0f};
   unsigned int idx_arr_expected[length] = {1U, 2U, 3U, 4U, 5U};

   /** \action
    *Call function.
    **/
   bool sorting_success = RSPP_Sort(v, length + 1, true, idx_arr);

   /** \result
    *Ensure function output is equal to the expected result.
    *No sorting of input value array and index array and return flag from sorting should be false.
    **/
   CHECK_FALSE(sorting_success);
   for (unsigned int i = 0; i < length; i++)
   {
      CHECK_EQUAL(v[i], v_expected[i]);
      CHECK_EQUAL(idx_arr[i], idx_arr_expected[i]);
   }
}

/** \purpose
 * Check that function returns true when sorting.
 * \req
 * NA
 */
TEST(rspp_math_func, RSPP_Sort_success)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float32_t values to operate on.
    **/
   const unsigned int length = 5;
   float32_t v[length] = {1.0f, 4.0f, 3.0f, 5.0f, 2.0f};
   unsigned int idx_arr[length];

   /** \action
    *Call function.
    **/
   bool sorting_success = RSPP_Sort(v, length, true, idx_arr);

   /** \result
    * When sorting is done, sorting_success should return true.
    **/
   CHECK_TRUE(sorting_success);
}

/** \purpose
 * Check if function returns the correct value.
 * \req
 * NA
 */
TEST(rspp_math_func, RSPP_Sort_asc)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float32_t values to operate on and compute expected result.
    **/
   const unsigned int length = 5;
   float32_t v[length] = {1.0f, 4.0f, 3.0f, 5.0f, 2.0f};
   unsigned int idx_arr[length];
   unsigned int idx_arr_asc[length] = {0U, 4U, 2U, 1U, 3U};

   /** \action
    *Call function.
    **/
   bool sorting_success = RSPP_Sort(v, length, true, idx_arr);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   CHECK_TRUE(sorting_success);
   for (unsigned int i = 0; i < length - 1; i++)
   {
      CHECK_TRUE(v[i + 1] > v[i]);
      CHECK_EQUAL(idx_arr_asc[i], idx_arr[i]);
   }
   CHECK_EQUAL(idx_arr_asc[length - 1], idx_arr[length - 1]);
}

/** \purpose
 * Check that sorting function just sorts, but does not modify sorted values.
 * \req
 * NA
 */
TEST(rspp_math_func, RSPP_Sort_sorted_values_not_modified)
{
   /** \step{1}
    * Execute test and verify results.
    */

   /** \precond
    *Set up array for sorting, and it's backup. Prepare expected permutation indexes after sorting.
    **/
   const unsigned int length = 5;
   float32_t v_original[length] = {1.11111f, 4.44444f, 3.33333f, 5.55555f, 2.22222f};
   float32_t v[length]; // Copy of v_original that will be sorted.
   for (unsigned int i = 0; i < length; i++)
   {
      v[i] = v_original[i];
   }
   unsigned int idx_arr[length];

   /** \action
    *Call function.
    **/
   bool sorting_success = RSPP_Sort(v, length, true, idx_arr);

   /** \result
    *Ensure function output is equal to the expected result.
    *Permutation saved in idx_arr should map to original unsorted array. Floats must be equal without any tolerance, it is copy!
    **/
   CHECK_TRUE(sorting_success);
   for (unsigned int i = 0; i < length; i++)
   {
      CHECK_EQUAL(v_original[idx_arr[i]], v[i]);
   }
}

/** \purpose
 * Check that sorting function can sort only specified portion of input. (Leaving remainder of input unsorted.)
 * \req
 * NA
 */
TEST(rspp_math_func, RSPP_Sort_partial)
{
   /** \step{1}
    * Execute test and verify results.
    */

   /** \precond
    *Set up array for sorting, and it's backup. Prepare expected permutation indexes after sorting.
    **/
   const unsigned int length = 5;
   float32_t v_original[length] = {1.0f, 4.0f, 3.0f, 5.0f, 2.0f};
   float32_t v[length]; // Copy of v_original that will be sorted.
   for (unsigned int i = 0; i < length; i++)
   {
      v[i] = v_original[i];
   }
   unsigned int idx_arr[length] = {0U, 1U, 2U, 3U, 4U}; // If only portion of input is sorted, then it is good to define whole array. Not defining it will lead to undefined values in not sorted indexes.
   unsigned int do_not_sort_last_n_values = 2;
   unsigned int idx_arr_asc[length] = {0U, 2U, 1U, 3U, 4U}; // first 3 elements sorted, last 2 elements keep their original position.

   /** \action
    *Call function.
    **/
   bool sorting_success = RSPP_Sort(v, length - do_not_sort_last_n_values, true, idx_arr);

   /** \result
    *Ensure function output is equal to the expected result.
    *Compare sorted(first 3) and also unsorted(last 2) indexes of inputs. All should match to expected idx_arr_asc. Observe that last two values remain unsorted.
    *Only po
    **/
   CHECK_TRUE(sorting_success);
   for (unsigned int i = 0; i < length; i++)
   {
      CHECK_EQUAL(idx_arr[i], idx_arr_asc[i]);
   }
}

/** \purpose
 * Check that sorting function will do no sorting when num_elements is zero.
 * \req
 * NA
 */
TEST(rspp_math_func, RSPP_Sort_no_sort)
{
   /** \step{1}
    * Execute test and verify results.
    */

   /** \precond
    *Set up array for sorting, and it's backup. Prepare expected permutation indexes after sorting.
    **/
   const unsigned int length = 5;
   float32_t v_original[length] = {1.0f, 4.0f, 3.0f, 5.0f, 2.0f};
   float32_t v[length]; // Copy of v_original that will be sorted.
   for (unsigned int i = 0; i < length; i++)
   {
      v[i] = v_original[i];
   }
   unsigned int idx_arr[length] = {0U, 1U, 2U, 3U, 4U};
   unsigned int idx_arr_asc[length] = {0U, 1U, 2U, 3U, 4U}; // Same as original. (No sorting expected.)

   /** \action
    *Call function.
    **/
   bool sorting_success = RSPP_Sort(v, 0, true, idx_arr);

   /** \result
    *Ensure function output is equal to the expected result.
    *Compare all indexes of input and its values. It should be same before and after the call to sorting function.
    *Sorting function should return true.
    **/
   CHECK_TRUE(sorting_success);
   for (unsigned int i = 0; i < length; i++)
   {
      CHECK_EQUAL(idx_arr[i], idx_arr_asc[i]);   // Unchanged index.
      CHECK_EQUAL(v_original[idx_arr[i]], v[i]); // Unchanged(not sorted) input.
   }
}

/** \purpose
 * Check that sorting is stable (preserving order of elements of same value).
 * \req
 * NA
 */
TEST(rspp_math_func, RSPP_Sort_stability)
{
   /** \step{1}
    * Execute test and verify results.
    */

   /** \precond
    *Set up array for sorting, and it's backup. Prepare expected permutation indexes after sorting.
    **/
   const unsigned int length = 6;
   float32_t v_original[length] = {1.0f, 2.0f, 1.0f, 2.0f, 1.0f, 2.0f};
   float32_t v[length]; // Copy of v_original that will be sorted.
   for (unsigned int i = 0; i < length; i++)
   {
      v[i] = v_original[i];
   }

   unsigned int idx_arr[length];
   unsigned int idx_arr_stable[length] = {0U, 2U, 4U, 1U, 3U, 5U}; // Same values preserve order in which they were present in original array.

   /** \action
    *Call function.
    **/
   bool sorting_success = RSPP_Sort(v, length, true, idx_arr);

   /** \result
    *Ensure function output is equal to the expected result.
    *Compare indexes of sorted array to expected indexes.
    **/
   CHECK_TRUE(sorting_success);
   for (unsigned int i = 0; i < length; i++)
   {
      CHECK_EQUAL(idx_arr[i], idx_arr_stable[i]); // Stable sorting.
      CHECK_EQUAL(v_original[idx_arr[i]], v[i]);  // Actual values are sorted.
   }
}

/** \purpose
 * Check if function returns the correct value.
 * \req
 * NA
 */
TEST(rspp_math_func, RSPP_Sort_dsc)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up float32_t values to operate on and compute expected result.
    **/
   const unsigned int length = 5;
   float32_t v[length] = {1.0f, 4.0f, 3.0f, 5.0f, 2.0f};
   unsigned int idx_arr[length];
   unsigned int idx_arr_dsc[length] = {3U, 1U, 2U, 4U, 0U};

   /** \action
    *Call function.
    **/

   bool sorting_success = RSPP_Sort(v, length, false, idx_arr);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   CHECK_TRUE(sorting_success);
   for (unsigned int i = 0; i < length - 1; i++)
   {
      CHECK_TRUE(v[i + 1] < v[i]);
      CHECK_EQUAL(idx_arr_dsc[i], idx_arr[i]);
   }
   CHECK_EQUAL(idx_arr_dsc[length - 1], idx_arr[length - 1]);
}

/** \purpose
 * Check that RSPP_Piecewise_Linear_Equation returns the correct
 * value when the input breakpoints are sorted.
 * \req
 * NA
 */
TEST(rspp_math_func, Test_RSPP_Piecewise_Linear_Equation_Sorted_Breakpoints)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up breakpoints, query points and expected result.
    **/
   const float32_t breakpoints_x[3] = {1.0F, 2.0F, 3.0F};
   const float32_t breakpoints_y[3] = {10.0F, 20.0F, 40.0F};
   const int num_breakpoints = 3;

   float32_t x_query_1 = 0.0F;
   float32_t expected_y_query_1 = 10.0F;

   float32_t x_query_2 = 1.0F;
   float32_t expected_y_query_2 = 10.0F;

   float32_t x_query_3 = 1.2F;
   float32_t expected_y_query_3 = 12.0F;

   float32_t x_query_4 = 2.0F;
   float32_t expected_y_query_4 = 20.0F;

   float32_t x_query_5 = 2.7F;
   float32_t expected_y_query_5 = 34.0F;

   float32_t x_query_6 = 3.0F;
   float32_t expected_y_query_6 = 40.0F;

   float32_t x_query_7 = 4.0F;
   float32_t expected_y_query_7 = 40.0F;

   /** \action
    *Call function with query point 1.
    **/
   float32_t y_query_result = RSPP_Piecewise_Linear_Equation(x_query_1, breakpoints_x, breakpoints_y, num_breakpoints);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_y_query_1, y_query_result, TEST_PASS_TH_SMALL, "Query 1 for sorted brekpoint array failed");

   /** \action
    *Call function with query point 2.
    **/
   y_query_result = RSPP_Piecewise_Linear_Equation(x_query_2, breakpoints_x, breakpoints_y, num_breakpoints);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_y_query_2, y_query_result, TEST_PASS_TH_SMALL, "Query 2 for sorted brekpoint array failed");

   /** \action
    *Call function with query point 3.
    **/
   y_query_result = RSPP_Piecewise_Linear_Equation(x_query_3, breakpoints_x, breakpoints_y, num_breakpoints);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_y_query_3, y_query_result, TEST_PASS_TH_SMALL, "Query 3 for sorted brekpoint array failed");

   /** \action
    *Call function with query point 4.
    **/
   y_query_result = RSPP_Piecewise_Linear_Equation(x_query_4, breakpoints_x, breakpoints_y, num_breakpoints);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_y_query_4, y_query_result, TEST_PASS_TH_SMALL, "Query 4 for sorted brekpoint array failed");

   /** \action
    *Call function with query point 5.
    **/
   y_query_result = RSPP_Piecewise_Linear_Equation(x_query_5, breakpoints_x, breakpoints_y, num_breakpoints);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_y_query_5, y_query_result, TEST_PASS_TH_SMALL, "Query 5 for sorted brekpoint array failed");

   /** \action
    *Call function with query point 6.
    **/
   y_query_result = RSPP_Piecewise_Linear_Equation(x_query_6, breakpoints_x, breakpoints_y, num_breakpoints);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_y_query_6, y_query_result, TEST_PASS_TH_SMALL, "Query 6 for sorted brekpoint array failed");

   /** \action
    *Call function with query point 7.
    **/
   y_query_result = RSPP_Piecewise_Linear_Equation(x_query_7, breakpoints_x, breakpoints_y, num_breakpoints);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_y_query_7, y_query_result, TEST_PASS_TH_SMALL, "Query 7 for sorted brekpoint array failed");
}

/** \purpose
 * Check that RSPP_Piecewise_Linear_Equation returns the correct
 * value when the input breakpoints are very close.
 * \req
 * NA
 */
TEST(rspp_math_func, Test_RSPP_Piecewise_Linear_Equation_Close_Breakpoints)
{
   /** \step{1}
    *Compare function output to expected output.
    **/

   /** \precond
    *Set up breakpoints, query points and expected result.
    **/
   const float32_t breakpoints_x[2] = {1.0F, 1.0F + 2e-7};
   const float32_t breakpoints_y[2] = {10.0F, 20.0F};
   const int num_breakpoints = 2;

   float32_t x_query = 1.0F + 1e-7;
   float32_t expected_y_query = 15.0F;

   /** \action
    *Call function with query point 1.
    **/
   float32_t y_query_result = RSPP_Piecewise_Linear_Equation(x_query, breakpoints_x, breakpoints_y, num_breakpoints);

   /** \result
    *Ensure function output is equal to the expected result.
    **/
   DOUBLES_EQUAL_TEXT(expected_y_query, y_query_result, TEST_PASS_TH_SMALL, "Failed for close breakpoints");
}

/** @}*/
