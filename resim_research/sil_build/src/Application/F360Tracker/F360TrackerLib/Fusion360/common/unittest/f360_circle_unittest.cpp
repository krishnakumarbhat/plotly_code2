/** \file
 * This file contains unit tests for content of f360_circle.cpp file
 */

#include "f360_circle.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_circle
 *  @{
 */

/** \brief
 * Test Circle class functionality
 */
TEST_GROUP(f360_circle)
{
   const Circle circle{ { 6.0F, 4.0F }, 3.0F };
   const float tolerance = 0.00001F;
};

/** \purpose  
 * Verify if method Value_At_X() returns correct output when "x" is out of cricle range (too far to the left)
 * \req
 * NA.
 */
TEST(f360_circle, Value_At_X__no_value__too_left)
{
   /** \precond
    * Same as setup - set circle
    * Set x value
    */
   const float x = 3.0F - tolerance;
	
   /** \action
    * call Value_At_X()
    */
   float result[2] = {};
   unsigned values_cnt = circle.Y_Intersects_At_X(x, result);

   /** \result
    * No output
    */	
   CHECK_EQUAL(0U, values_cnt);
}

/** \purpose
 * Verify if method Value_At_X() returns correct output when "x" is out of cricle range (to far to the right)
 * \req
 * NA.
 */
TEST(f360_circle, Value_At_X__no_value__too_right)
{
   /** \precond
    * Same as setup - set circle
    * Set x value
    */
   const float x = 9.0F + tolerance;

   /** \action
    * call Value_At_X()
    */
   float result[2] = {};
   unsigned values_cnt = circle.Y_Intersects_At_X(x, result);

   /** \result
    * No output
    */
   CHECK_EQUAL(0U, values_cnt);
}

/** \purpose
 * Verify if method Value_At_X() returns correct output when "x" is on circle range (left case)
 * \req
 * NA.
 */
TEST(f360_circle, Value_At_X__one_value__left)
{
   /** \precond
    * Same as setup - set circle
    * Set x value
    */
   const float x = 3.0F;

   /** \action
    * call Value_At_X()
    */
   float result[2] = {};
   unsigned values_cnt = circle.Y_Intersects_At_X(x, result);

   /** \result
    * Returns one value
    */
   CHECK_EQUAL(1U, values_cnt);
   DOUBLES_EQUAL(4.0F, result[0], tolerance);
}

/** \purpose
 * Verify if method Value_At_X() returns correct output when "x" is on circle range (right case)
 * \req
 * NA.
 */
TEST(f360_circle, Value_At_X__one_value__right)
{
   /** \precond
    * Same as setup - set circle
    * Set x value
    */
   const float x = 3.0F;

   /** \action
    * call Value_At_X()
    */
   float result[2] = {};
   unsigned values_cnt = circle.Y_Intersects_At_X(x, result);

   /** \result
    * Returns one value
    */
   CHECK_EQUAL(1U, values_cnt);
   DOUBLES_EQUAL(4.0F, result[0], tolerance);
}

/** \purpose
 * Verify if method Value_At_X() returns correct output when "x" is within circle range (left case)
 * \req
 * NA.
 */
TEST(f360_circle, Value_At_X__two_values__more_left)
{
   /** \precond
    * Same as setup - set circle
    * Set x value
    */
   const float x = 3.0F + tolerance;

   /** \action
    * call Value_At_X()
    */
   float result[2] = {};
   unsigned values_cnt = circle.Y_Intersects_At_X(x, result);

   /** \result
    * Returns two values
    */
   CHECK_EQUAL(2U, values_cnt);
   DOUBLES_EQUAL(4.007743F, result[0], tolerance);
   DOUBLES_EQUAL(3.992254F, result[1], tolerance);
}

/** \purpose
 * Verify if method Value_At_X() returns correct output when "x" is within circle range (right case)
 * \req
 * NA.
 */
TEST(f360_circle, Value_At_X__two_values__more_right)
{
   /** \precond
    * Same as setup - set circle
    * Set x value
    */
   const float x = 9.0F - tolerance;

   /** \action
    * call Value_At_X()
    */
   float result[2] = {};
   unsigned values_cnt = circle.Y_Intersects_At_X(x, result);

   /** \result
    * Returns two values
    */
   CHECK_EQUAL(2U, values_cnt);
   DOUBLES_EQUAL(4.00757F, result[0], tolerance);
   DOUBLES_EQUAL(3.99244F, result[1], tolerance);
}

/** \purpose
 * Verify if method Value_At_X() returns correct output when "x" is within circle range (middle case)
 * \req
 * NA.
 */
TEST(f360_circle, Value_At_X__two_values__close_middle)
{
   /** \precond
    * Same as setup - set circle
    * Set x value
    */
   const float x = 5.8F;

   /** \action
    * call Value_At_X()
    */
   float result[2] = {};
   unsigned values_cnt = circle.Y_Intersects_At_X(x, result);

   /** \result
    * Returns two values
    */
   CHECK_EQUAL(2U, values_cnt);
   DOUBLES_EQUAL(6.99332F, result[0], tolerance);
   DOUBLES_EQUAL(1.00667F, result[1], tolerance);
}

/** \purpose
 * Verify if method Value_At_Y() returns correct output when "y" is out of cricle range (too far to the left)
 * \req
 * NA.
 */
TEST(f360_circle, Value_At_Y__no_value__too_left)
{
   /** \precond
    * Same as setup - set circle
    * Set y value
    */
   const float y = 1.0F - tolerance;

   /** \action
    * call Value_At_Y()
    */
   float result[2] = {};
   unsigned values_cnt = circle.X_Intersects_At_Y(y, result);

   /** \result
    * No output
    */
   CHECK_EQUAL(0U, values_cnt);
}

/** \purpose
 * Verify if method Value_At_Y() returns correct output when "y" is out of cricle range (too far to the right)
 * \req
 * NA.
 */
TEST(f360_circle, Value_At_Y__no_value__too_right)
{
   /** \precond
    * Same as setup - set circle
    * Set y value
    */
   const float y = 7.0F + tolerance;

   /** \action
    * call Value_At_Y()
    */
   float result[2] = {};
   unsigned values_cnt = circle.X_Intersects_At_Y(y, result);

   /** \result
    * No output
    */
   CHECK_EQUAL(0U, values_cnt);
}

/** \purpose
 * Verify if method Value_At_Y() returns correct output when "y" is on circle edge (left case)
 * \req
 * NA.
 */
TEST(f360_circle, Value_At_Y__one_value__left)
{
   /** \precond
    * Same as setup - set circle
    * Set y value
    */
   const float y = 1.0F;

   /** \action
    * call Value_At_Y()
    */
   float result[2] = {};
   unsigned values_cnt = circle.X_Intersects_At_Y(y, result);

   /** \result
    * Returns one value
    */
   CHECK_EQUAL(1U, values_cnt);
   DOUBLES_EQUAL(6.0F, result[0], tolerance);
}

/** \purpose
 * Verify if method Value_At_Y() returns correct output when "y" is on circle edge (right case)
 * \req
 * NA.
 */
TEST(f360_circle, Value_At_Y__one_value__right)
{
   /** \precond
    * Same as setup - set circle
    * Set y value
    */
   const float y = 7.0F;

   /** \action
    * call Value_At_Y()
    */
   float result[2] = {};
   unsigned values_cnt = circle.X_Intersects_At_Y(y, result);

   /** \result
    * Returns one value
    */
   CHECK_EQUAL(1U, values_cnt);
   DOUBLES_EQUAL(6.0F, result[0], tolerance);
}

/** \purpose
 * Verify if method Value_At_Y() returns correct output when "y" is within circle range (left case)
 * \req
 * NA.
 */
TEST(f360_circle, Value_At_Y__two_values__more_left)
{
   /** \precond
    * Same as setup - set circle
    * Set x value
    */
   const float y = 1.0F + tolerance;

   /** \action
    * call Value_At_Y()
    */
   float result[2] = {};
   unsigned values_cnt = circle.X_Intersects_At_Y(y, result);

   /** \result
    * Returns two values
    */
   CHECK_EQUAL(2U, values_cnt);
   DOUBLES_EQUAL(6.007746F, result[0], tolerance);
   DOUBLES_EQUAL(5.992254F, result[1], tolerance);
}

/** \purpose
 * Verify if method Value_At_Y() returns correct output when "y" is within circle range (right case)
 * \req
 * NA.
 */
TEST(f360_circle, Value_At_Y__two_values__more_right)
{
   /** \precond
    * Same as setup - set circle
    * Set x value
    */
   const float y = 7.0F - tolerance;

   /** \action
    * call Value_At_Y()
    */
   float result[2] = {};
   unsigned values_cnt = circle.X_Intersects_At_Y(y, result);

   /** \result
    * Returns two values
    */
   CHECK_EQUAL(2U, values_cnt);
   DOUBLES_EQUAL(6.007746F, result[0], tolerance);
   DOUBLES_EQUAL(5.992254F, result[1], tolerance);
}

/** \purpose
 * Verify if method Value_At_Y() returns correct output when "y" is within circle range (middle case)
 * \req
 * NA.
 */
TEST(f360_circle, Value_At_Y__two_values__close_middle)
{
   /** \precond
    * Same as setup - set circle
    * Set y value
    */
   const float y = 4.4F;

   /** \action
    * call Value_At_Y()
    */
   float result[2] = {};
   unsigned values_cnt = circle.X_Intersects_At_Y(y, result);

   /** \result
    * Returns two values
    */
   CHECK_EQUAL(2U, values_cnt);
   DOUBLES_EQUAL(8.973214F, result[0], tolerance);
   DOUBLES_EQUAL(3.026786F, result[1], tolerance);
}
/** @}*/
