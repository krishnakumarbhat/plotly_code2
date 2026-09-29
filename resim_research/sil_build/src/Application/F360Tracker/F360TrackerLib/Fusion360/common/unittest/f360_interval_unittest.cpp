/** \file
 * This file contains unit tests for content of f360_interval.h file
 */
 // Unit testing guidelines: http://confluenceprod1.delphiauto.net:8090/display/F360Core/Unit+testing+guidelines


#include "f360_interval.h"
#include <CppUTest/TestHarness.h>
using namespace f360_variant_A;

/** \defgroup  Interval
 *  @{
 */

 /** \brief
 * This test group is testing Interval class
  */
TEST_GROUP(f360_interval)
{
};

/** \purpose
 * Test Contains method with int32_t type input parameters, tested value outside interval
 * \req
 * NA.
 */
TEST(f360_interval, test_contains_method_with_int32_t_type_input_outside_interval)
{
   /** \precond
    * interval definition
    * set value to compare to outside interval
    */
   Interval<int32_t> interval(-15, 10);
   int32_t value_to_compare = -30;

   /** \action
    * Run Contains method on value outside interval
    */
   bool f_interval_contains = interval.Contains(value_to_compare);

   /** \result
    * Verify if result returned by Contains is correct, value should be outside interval
    */
   CHECK_FALSE(f_interval_contains);
}

/** \purpose
 * Test Contains method with int32_t type input parameters, tested value inside interval
 * \req
 * NA.
 */
TEST(f360_interval, test_contains_method_with_int32_t_type_input_inside_interval)
{
   /** \precond
    * interval definition
    * set value to compare to inside interval
    */
   Interval<int32_t> interval(-15, 10);
   int32_t value_to_compare = -3;

   /** \action
    * Run Contains method on value inside interval
    */
   bool f_interval_contains = interval.Contains(value_to_compare);

   /** \result
    * Verify if result returned by Contains is correct, value should be inside interval
    */
   CHECK_TRUE(f_interval_contains);
}

/** \purpose
 * Test Contains method with int32_t type input parameters, tested value on lower bound of interval
 * Value should be detected as inside interval since interval is lower bound inclusive
 * \req
 * NA.
 */
TEST(f360_interval, test_contains_method_with_int32_t_type_input_on_lower_bound_of_interval)
{
   /** \precond
    * interval definition
    * set value to compare on lower bound of interval
    */
   Interval<int32_t> interval(-7, 10);
   int32_t value_to_compare = -7;

   /** \action
    * Run Contains method on value on lower bound of interval
    */
   bool f_interval_contains = interval.Contains(value_to_compare);

   /** \result
    * Verify if result returned by Contains is correct, value should be inside interval since interval is lower bound inclusive
    */
   CHECK_TRUE(f_interval_contains);
}

/** \purpose
 * Test Contains method with int32_t type input parameters, tested value on upper bound of interval
 * Value should be detected as outside interval since interval is upper bound exclusive
 * \req
 * NA.
 */
TEST(f360_interval, test_contains_method_with_int32_t_type_input_on_upper_bound_of_interval)
{
   /** \precond
    * interval definition
    * set value to compare on upper bound of interval
    */
   Interval<int32_t> interval(-9, 11);
   int32_t value_to_compare = 11;

   /** \action
    * Run Contains method on value on upper bound of interval
    */
   bool f_interval_contains = interval.Contains(value_to_compare);

   /** \result
    * Verify if result returned by Contains is correct, value should be outside interval since interval is lower bound exclusive
    */
   CHECK_FALSE(f_interval_contains);
}


/** \purpose
 * Test Contains method with F360_UINT32_T type input parameters, tested value inside interval
 * \req
 * NA.
 */
TEST(f360_interval, test_contains_method_with_F360_UINT32_T_type_input_inside_interval)
{
   /** \precond
    * interval definition
    * set value to compare to inside interval
    */
   Interval<uint32_t> interval(10U, 20U);
   uint32_t value_to_compare = 15U;

   /** \action
    * Run Contains method on example value inside interval
    */
   bool f_interval_contains = interval.Contains(value_to_compare);

   /** \result
    * Verify if result returned by Contains is correct, value should be inside interval
    */
   CHECK_TRUE(f_interval_contains);
}

/** \purpose
 * Test Contains method with F360_UINT32_T type input parameters, tested value outside interval
 * \req
 * NA.
 */
TEST(f360_interval, test_contains_method_with_F360_UINT32_T_type_input_outside_interval)
{
   /** \precond
    * interval definition
    * set value to compare to outside interval
    */
   Interval<uint32_t> interval(1U, 3U);
   uint32_t value_to_compare = 0U;

   /** \action
    * Run Contains method on example value outside interval
    */
   bool f_interval_contains = interval.Contains(value_to_compare);

   /** \result
    * Verify if result returned by Contains is correct, value should be outside interval
    */
   CHECK_FALSE(f_interval_contains);
}

/** \purpose
 * Test Contains method with float32_t type input parameters, tested value inside interval
 * \req
 * NA.
 */
TEST(f360_interval, test_contains_method_with_float32_t_type_input_inside_interval)
{
   /** \precond
    * interval definition
    * set value to compare to inside interval
    */
   Interval<float32_t> interval(-3.0F, -1.0F);
   float32_t value_to_compare = -2.0F;

   /** \action
    * Run Contains method on example value inside interval
    */
   bool f_interval_contains = interval.Contains(value_to_compare);

   /** \result
    * Verify if result returned by Contains is correct, value should be inside interval
    */
   CHECK_TRUE(f_interval_contains);
}

/** \purpose
 * Test Contains method with float32_t type input parameters, tested value outside interval
 * \req
 * NA.
 */
TEST(f360_interval, test_contains_method_with_float32_t_type_input_outside_interval)
{
   /** \precond
    * interval definition
    * set value to compare to outside interval
    */
   Interval<float32_t> interval(-10.0F, -2.0F);
   float32_t value_to_compare = 7.0F;

   /** \action
    * Run Contains method on example value outside interval
    */
   bool f_interval_contains = interval.Contains(value_to_compare);

   /** \result
    * Verify if result returned by Contains is correct, value should be outside interval
    */
   CHECK_FALSE(f_interval_contains);
}


/** @}*/

