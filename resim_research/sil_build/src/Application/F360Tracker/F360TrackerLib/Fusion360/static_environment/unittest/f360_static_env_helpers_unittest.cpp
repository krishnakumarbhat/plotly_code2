/** \file
 * This file contains unit tests for content of f360_static_env_helpers.cpp file
 */

#include "f360_static_env_helpers.h"
#include <CppUTest/TestHarness.h>


using namespace f360_variant_A;

/** \defgroup  f360_static_env_helpers_Reset_Single_Static_Env_Poly
 *  @{
 */

/** \brief
 * Test group related to test of function Reset_Single_Static_Env_Poly()
 */
TEST_GROUP(f360_static_env_helpers_Reset_Single_Static_Env_Poly)
{
   
};

/** \purpose  
 * Purpose is to verify that static polynomial is reset as expected
 * \req
 * NA
 */
TEST(f360_static_env_helpers_Reset_Single_Static_Env_Poly, Reset_Single_Static_Env_Poly__Verify_Reset)
{
   /** \precond
    * Fill a static polynomial with arbitrary data
    */
   Static_Env_Poly_T static_env_poly;
   static_env_poly.age = 100U;
   static_env_poly.confidence = 1.0F;
   static_env_poly.lower_limit = 1.0F;
   static_env_poly.upper_limit = 1.0F;
   static_env_poly.p0 = 1.0F;
   static_env_poly.p1 = 1.0F;
   static_env_poly.p2 = 1.0F;
   static_env_poly.status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   static_env_poly.poly_type = F360_STATIC_ENV_POLY_TYPE_LSC;

   /** \action
    * Call Reset_Single_Static_Env_Poly()
    */
   Reset_Single_Static_Env_Poly(static_env_poly);

   /** \result
    * Verify that data have been reset
    */
   CHECK_EQUAL(0U, static_env_poly.age);
   CHECK_EQUAL(0.0F, static_env_poly.confidence);
   CHECK_EQUAL(0.0F, static_env_poly.lower_limit);
   CHECK_EQUAL(0.0F, static_env_poly.upper_limit);
   CHECK_EQUAL(0.0F, static_env_poly.p0);
   CHECK_EQUAL(0.0F, static_env_poly.p1);
   CHECK_EQUAL(0.0F, static_env_poly.p2);
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_INVALID, static_env_poly.status);
   CHECK_EQUAL(F360_STATIC_ENV_POLY_TYPE_INVALID, static_env_poly.poly_type);
}
/** @}*/

/** \defgroup  f360_static_env_helpers_Map_Single_LSC_To_Static_Env_Poly
 *  @{
 */

 /** \brief
  * Test group related to test of function Map_Single_LSC_To_Static_Env_Poly()
  */
TEST_GROUP(f360_static_env_helpers_Map_Single_LSC_To_Static_Env_Poly)
{
   F360_Longi_Stat_Curve_T lsc = {};

   /** \setup
    * Initialize a valid LSC with arbitrary data
    */
   TEST_SETUP()
   {
      lsc.f_valid = true;
      lsc.a = 1.0F;
      lsc.b = 2.0F;
      lsc.c = -3.0F;
      lsc.x_min = -10.0F;
      lsc.x_max = 10.0F;
      lsc.mean_lat_pos = -3.0F;
   }
};

/** \purpose
 * Purpose is to verify that static polynomial is reset as expected when 
 * the LSC we try to map is invalid
 * \req
 * NA
 */
TEST(f360_static_env_helpers_Map_Single_LSC_To_Static_Env_Poly, Map_Single_LSC_To_Static_Env_Poly__Invalid_LSC)
{
   /** \precond
    * Fill a static polynomial with arbitrary data
    * Set LSC as invalid
    */
   Static_Env_Poly_T static_env_poly = {};
   static_env_poly.age = 100U;
   static_env_poly.confidence = 1.0F;
   static_env_poly.lower_limit = 1.0F;
   static_env_poly.upper_limit = 1.0F;
   static_env_poly.p0 = 1.0F;
   static_env_poly.p1 = 1.0F;
   static_env_poly.p2 = 1.0F;
   static_env_poly.status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   static_env_poly.poly_type = F360_STATIC_ENV_POLY_TYPE_LSC;

   lsc.f_valid = false;

   /** \action
    * Call Map_Single_LSC_To_Static_Env_Poly()
    */
   Map_Single_LSC_To_Static_Env_Poly(lsc, static_env_poly);

   /** \result
    * Verify that data have been reset
    */
   CHECK_EQUAL(0U, static_env_poly.age);
   CHECK_EQUAL(0.0F, static_env_poly.confidence);
   CHECK_EQUAL(0.0F, static_env_poly.lower_limit);
   CHECK_EQUAL(0.0F, static_env_poly.upper_limit);
   CHECK_EQUAL(0.0F, static_env_poly.p0);
   CHECK_EQUAL(0.0F, static_env_poly.p1);
   CHECK_EQUAL(0.0F, static_env_poly.p2);
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_INVALID, static_env_poly.status);
   CHECK_EQUAL(F360_STATIC_ENV_POLY_TYPE_INVALID, static_env_poly.poly_type);
}

/** \purpose
 * Purpose is to verify that static polynomial is mapped correctly when
 * the LSC we try to map is valid
 * \req
 * NA
 */
TEST(f360_static_env_helpers_Map_Single_LSC_To_Static_Env_Poly, Map_Single_LSC_To_Static_Env_Poly__Valid_LSC)
{
   /** \precond
    * Initialize a static environment polynomial
    * LSC contains valid information set in test group
    */
   Static_Env_Poly_T static_env_poly = {};

   /** \action
    * Call Map_Single_LSC_To_Static_Env_Poly()
    */
   Map_Single_LSC_To_Static_Env_Poly(lsc, static_env_poly);

   /** \result
    * Verify that data have been updated with data from LSC
    */
   CHECK_EQUAL(1U, static_env_poly.age);
   CHECK_EQUAL(1.0F, static_env_poly.confidence);
   CHECK_EQUAL(lsc.x_min, static_env_poly.lower_limit);
   CHECK_EQUAL(lsc.x_max, static_env_poly.upper_limit);
   CHECK_EQUAL(lsc.c, static_env_poly.p0);
   CHECK_EQUAL(lsc.b, static_env_poly.p1);
   CHECK_EQUAL(lsc.a, static_env_poly.p2);
   CHECK_EQUAL(F360_STATIC_ENV_POLY_STATUS_UPDATED, static_env_poly.status);
   CHECK_EQUAL(F360_STATIC_ENV_POLY_TYPE_LSC, static_env_poly.poly_type);
}
/** @}*/

/** \defgroup  SEP_Lateral_Pos_At
 *  @{
 */

 /** \brief
  *  Test group for SEP_Lateral_Pos_At()
  **/
TEST_GROUP(SEP_Lateral_Pos_At)
{
   Static_Env_Poly_T poly;
   const float32_t test_pass_th = 1e-6F;

   /** \setup
    * Initialize a polynomial with known coefficients
    */
   TEST_SETUP()
   {
      poly.p0 = 0.0F;
      poly.p1 = 0.0F;
      poly.p2 = 0.0F;
   }
};

/** \purpose
 * Verify that for a constant polynomial (p1=0, p2=0) the function returns p0
 * regardless of the longitudinal position.
 * \req
 * NA
 */
TEST(SEP_Lateral_Pos_At, SEP_Lateral_Pos_At__parallel_to_host_case)
{
   /** \precond
    * p0 = 3.0, p1 = 0.0, p2 = 0.0
    */
   poly.p0 = 3.0F;

   /** \action
    * Call SEP_Lateral_Pos_At at an arbitrary longitudinal position.
    */
   const float32_t result = poly.SEP_Lateral_Pos_At(10.0F);

   /** \result
    * Returned value should equal p0.
    */
   DOUBLES_EQUAL(3.0F, result, test_pass_th);
}

/** \purpose
 * Verify that for a linear polynomial (p2=0) the function returns p1*t + p0.
 * \req
 * NA
 */
TEST(SEP_Lateral_Pos_At, SEP_Lateral_Pos_At__linear_polynomial_case)
{
   /** \precond
    * p0 = 1.0, p1 = 2.0, p2 = 0.0, t = 5.0
    * Expected: 2.0 * 5.0 + 1.0 = 11.0
    */
   poly.p0 = 1.0F;
   poly.p1 = 2.0F;

   /** \action
    * Call SEP_Lateral_Pos_At at t = 5.0.
    */
   const float32_t result = poly.SEP_Lateral_Pos_At(5.0F);

   /** \result
    * Returned value should equal 11.0.
    */
   DOUBLES_EQUAL(11.0F, result, test_pass_th);
}

/** \purpose
 * Verify that the full quadratic polynomial is evaluated correctly.
 * \req
 * NA
 */
TEST(SEP_Lateral_Pos_At, SEP_Lateral_Pos_At__quadratic_polynomial_case)
{
   /** \precond
    * p0 = 1.0, p1 = 2.0, p2 = 0.5, t = 4.0
    * Expected: 0.5 * 16.0 + 2.0 * 4.0 + 1.0 = 8.0 + 8.0 + 1.0 = 17.0
    */
   poly.p0 = 1.0F;
   poly.p1 = 2.0F;
   poly.p2 = 0.5F;

   /** \action
    * Call SEP_Lateral_Pos_At at t = 4.0.
    */
   const float32_t result = poly.SEP_Lateral_Pos_At(4.0F);

   /** \result
    * Returned value should equal 17.0.
    */
   DOUBLES_EQUAL(17.0F, result, test_pass_th);
}

/** \purpose
 * Verify that the function evaluates correctly at t = 0, returning p0.
 * \req
 * NA
 */
TEST(SEP_Lateral_Pos_At, SEP_Lateral_Pos_At__evaluation_at_zero_case)
{
   /** \precond
    * p0 = 5.0, p1 = 3.0, p2 = 1.0, t = 0.0
    */
   poly.p0 = 5.0F;
   poly.p1 = 3.0F;
   poly.p2 = 1.0F;

   /** \action
    * Call SEP_Lateral_Pos_At at t = 0.0.
    */
   const float32_t result = poly.SEP_Lateral_Pos_At(0.0F);

   /** \result
    * Returned value should equal p0 = 5.0.
    */
   DOUBLES_EQUAL(5.0F, result, test_pass_th);
}

/** @}*/

/** \defgroup  LSC_Lateral_Pos_At
 *  @{
 */

 /** \brief
  *  Test group for LSC_Lateral_Pos_At()
  **/
TEST_GROUP(LSC_Lateral_Pos_At)
{
   F360_Longi_Stat_Curve_T lsc;
   const float32_t test_pass_th = 1e-6F;
   /** \setup
 * Initialize a polynomial with known coefficients
 */
   TEST_SETUP()
   {
      lsc.a = 0.0F;
      lsc.b = 0.0F;
      lsc.c = 0.0F;
      lsc.f_valid = true;
   }
};

/** \purpose
 * Verify that for a constant polynomial (b=0, a=0) the function returns c
 * regardless of the longitudinal position.
 * \req
 * NA
 */
TEST(LSC_Lateral_Pos_At, LSC_Lateral_Pos_At__parallel_to_host_case)
{
   /** \precond
    * a = 0.0, b = 0.0, c = 3.0
    */
   lsc.c = 3.0F;

   /** \action
    * Call LSC_Lateral_Pos_At at an arbitrary longitudinal position.
    */
   const float32_t result = lsc.LSC_Lateral_Pos_At(10.0F);

   /** \result
    * Returned value should equal c.
    */
   DOUBLES_EQUAL(3.0F, result, test_pass_th);
}

/** \purpose
 * Verify that for a linear polynomial (a=0) the function returns b*x + c.
 * \req
 * NA
 */
TEST(LSC_Lateral_Pos_At, LSC_Lateral_Pos_At__linear_polynomial_case)
{
   /** \precond
    * a = 0.0, b = 2.0, c = 1.0, x = 5.0
    * Expected: 2.0 * 5.0 + 1.0 = 11.0
    */
   lsc.b = 2.0F;
   lsc.c = 1.0F;

   /** \action
    * Call LSC_Lateral_Pos_At at x = 5.0.
    */
   const float32_t result = lsc.LSC_Lateral_Pos_At(5.0F);

   /** \result
    * Returned value should equal 11.0.
    */
   DOUBLES_EQUAL(11.0F, result, test_pass_th);
}

/** \purpose
 * Verify that the full quadratic polynomial is evaluated correctly.
 * \req
 * NA
 */
TEST(LSC_Lateral_Pos_At, LSC_Lateral_Pos_At__quadratic_polynomial_case)
{
   /** \precond
    * a = 0.5, b = 2.0, c = 1.0, x = 4.0
    * Expected: 0.5 * 16.0 + 2.0 * 4.0 + 1.0 = 8.0 + 8.0 + 1.0 = 17.0
    */
   lsc.a = 0.5F;
   lsc.b = 2.0F;
   lsc.c = 1.0F;

   /** \action
    * Call LSC_Lateral_Pos_At at x = 4.0.
    */
   const float32_t result = lsc.LSC_Lateral_Pos_At(4.0F);

   /** \result
    * Returned value should equal 17.0.
    */
   DOUBLES_EQUAL(17.0F, result, test_pass_th);
}

/** \purpose
 * Verify that the function evaluates correctly at x = 0, returning c.
 * \req
 * NA
 */
TEST(LSC_Lateral_Pos_At, LSC_Lateral_Pos_At__evaluation_at_zero_case)
{
   /** \precond
    * a = 1.0, b = 3.0, c = 5.0, x = 0.0
    */
   lsc.a = 1.0F;
   lsc.b = 3.0F;
   lsc.c = 5.0F;

   /** \action
    * Call LSC_Lateral_Pos_At at x = 0.0.
    */
   const float32_t result = lsc.LSC_Lateral_Pos_At(0.0F);

   /** \result
    * Returned value should equal c = 5.0.
    */
   DOUBLES_EQUAL(5.0F, result, test_pass_th);
}

/** @}*/
