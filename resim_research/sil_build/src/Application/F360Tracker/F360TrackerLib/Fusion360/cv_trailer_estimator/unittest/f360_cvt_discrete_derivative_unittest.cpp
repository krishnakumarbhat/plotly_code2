// APTIV_CODER_START
/** \file
 * This file contains unit tests for content of f360_cvt_discrete_derivative.cpp file
 */

#include "f360_cvt_discrete_derivative.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

/** \defgroup  f360_cvt_discrete_derivative
 *  @{
 */

using namespace f360_variant_A;

/** \brief
 * Test group for the discrete_time_derivative function in the f360_variant_A namespace.
 * This group tests the calculation of discrete time derivatives under various conditions.
 */
TEST_GROUP(f360_cvt_discrete_derivative)
{
   // Declare common variables used within all tests in this test group.
   float32_t u_previous;
   float32_t time_diff_u;
   float32_t u_now;
   float32_t epsilon;

   /** \setup
    * Initialize common variables before each test.
    */
   TEST_SETUP()
   {
      u_previous = 0.0F;
      time_diff_u = 0.0F;
      u_now = 0.0F;
      epsilon = 1E-3F;
   }
};

/** \purpose
 * Test the discrete_time_derivative function with a positive change in value.
 * \req
 * NA
 */
TEST(f360_cvt_discrete_derivative, PositiveDerivative)
{
   /** \precond
    * Set up initial conditions for a positive derivative
    */
   u_previous = 1.0F;
   u_now = 2.0F;

   /** \action
    * Call the discrete_time_derivative function
    */
   discrete_time_derivative(u_now, u_previous, time_diff_u);

   /** \result
    * Check that the time derivative and updated u_previous are correct
    */
   DOUBLES_EQUAL_TEXT(20.0F, time_diff_u, epsilon, "Wrong time derivative result");
   DOUBLES_EQUAL_TEXT(2.0F, u_previous, epsilon, "Previous state not returned correctly");
}

/** \purpose
 * Test the discrete_time_derivative function with a negative change in value.
 * \req
 * NA
 */
TEST(f360_cvt_discrete_derivative, NegativeDerivative)
{
   /** \precond
    * Set up initial conditions for a negative derivative
    */
   u_previous = 2.0F;
   u_now = 1.0F;

   /** \action
    * Call the discrete_time_derivative function
    */
   discrete_time_derivative(u_now, u_previous, time_diff_u);

   /** \result
    * Check that the time derivative and updated u_previous are correct
    */
   DOUBLES_EQUAL_TEXT(-20.0F, time_diff_u, epsilon, "Wrong time derivative result");
   DOUBLES_EQUAL_TEXT(1.0F, u_previous, epsilon, "Previous state not returned correctly");
}

/** \purpose
 * Test the discrete_time_derivative function with no change in value.
 * \req
 * NA
 */
TEST(f360_cvt_discrete_derivative, ZeroDerivative)
{
   /** \precond
    * Set up initial conditions for a zero derivative
    */
   u_previous = 1.0F;
   u_now = 1.0F;

   /** \action
    * Call the discrete_time_derivative function
    */
   discrete_time_derivative(u_now, u_previous, time_diff_u);

   /** \result
    * Check that the time derivative is zero and u_previous remains unchanged
    */
   DOUBLES_EQUAL_TEXT(0.0F, time_diff_u, epsilon, "Wrong time derivative result");
   DOUBLES_EQUAL_TEXT(1.0F, u_previous, epsilon, "Previous state not returned correctly");
}
// APTIV_CODER_END
/** @}*/
