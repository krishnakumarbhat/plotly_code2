/** \file
 * This file contains unit tests for content of f360_rectangle.cpp file
 */

#include "f360_rectangle.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_rectangle_zone1
 *  @{
 */

 /** \brief
  * Test group of Rectangle_T class. Tests verify whether it is properly determined whether point is inside rectangle.
  */
TEST_GROUP(f360_rectangle_zone1)
{
   float32_t min_long{};
   float32_t max_long{};
   float32_t min_lat{};
   float32_t max_lat{};

   float32_t lateral{};
   float32_t longitudinal{};

   /** \setup
    * Set up rectangle parameters
    */
   TEST_SETUP()
   {
      min_long = -50.0F;
      max_long = 50.0F;
      min_lat = -20.0F;
      max_lat = 20.0F;
   }
};

/** \purpose
 * Purpose of this test is to verify whether function returns true when object is in inside rectangle
 * \req
 * NA.
 */
TEST(f360_rectangle_zone1, Is_Inside__Returns_True_When_Position_In_Rectangle)
{
   /** \precond
    * Create rectangle object
    * Set up vcs position to be placed in rectangle
    */
   const Rectangle_T rectangle(min_long, max_long, min_lat, max_lat);

   lateral = -2.0F;
   longitudinal = -2.0F;

   /** \action
    * Call tested function
    */
   const bool f_inside = rectangle.Contains(longitudinal, lateral);

   /** \result
    * Check whether f_inside is true
    */
   CHECK_TRUE(f_inside);
}

/** \purpose
 * Purpose of this test is to verify whether function returns false when object lateral position is too low
 * \req
 * NA.
 */
TEST(f360_rectangle_zone1, Is_Inside__Returns_False_When_Too_Low_Lateral_Position)
{
   /** \precond
    * Set up vcs position to be placed outside of rectangle - far left
    */
   const Rectangle_T rectangle(min_long, max_long, min_lat, max_lat);

   lateral = min_lat - 5.0F;
   longitudinal = 2.0F;

   /** \action
    * Call tested function
    */
   const bool f_inside = rectangle.Contains(longitudinal, lateral);

   /** \result
    * Check whether f_inside is false
    */
   CHECK_FALSE(f_inside);
}

/** \purpose
 * Purpose of this test is to verify whether function returns false when object lateral position is too high
 * \req
 * NA.
 */
TEST(f360_rectangle_zone1, Is_Inside__Returns_False_When_Too_High_Lateral_Position)
{
   /** \precond
    * Set up vcs position to be placed outside of rectangle - far right
    */
   const Rectangle_T rectangle(min_long, max_long, min_lat, max_lat);

   lateral = max_lat + 5.0F;
   longitudinal = 2.0F;

   /** \action
    * Call tested function
    */
   const bool f_inside = rectangle.Contains(longitudinal, lateral);

   /** \result
    * Check whether f_inside is false
    */
   CHECK_FALSE(f_inside);
}

/** \purpose
 * Purpose of this test is to verify whether function returns false when object longitudinal position is too low
 * \req
 * NA.
 */
TEST(f360_rectangle_zone1, Is_Inside__Returns_False_When_Too_Low_Longitudinal_Position)
{
   /** \precond
    * Set up vcs position to be placed outside of rectangle - far before host
    */
   const Rectangle_T rectangle(min_long, max_long, min_lat, max_lat);

   lateral = -2.0F;
   longitudinal = min_long - 5.0F;

   /** \action
    * Call tested function
    */
   const bool f_inside = rectangle.Contains(longitudinal, lateral);

   /** \result
    * Check whether f_inside is false
    */
   CHECK_FALSE(f_inside);
}

/** \purpose
 * Purpose of this test is to verify whether function returns false when object longitudinal position is too high
 * \req
 * NA.
 */
TEST(f360_rectangle_zone1, Is_Inside__Returns_False_When_Too_High_Longitudinal_Position)
{
   /** \precond
    * Set up vcs position to be placed outside of rectangle - far before host
    */
   const Rectangle_T rectangle(min_long, max_long, min_lat, max_lat);

   lateral = -2.0F;
   longitudinal = max_long + 5.0F;

   /** \action
    * Call tested function
    */
   const bool f_inside = rectangle.Contains(longitudinal, lateral);

   /** \result
    * Check whether f_inside is false
    */
   CHECK_FALSE(f_inside);
}

/** \purpose
 * Purpose of this test is to verify whether function returns false when object is placed on left front corner of rectangle
 * \req
 * NA.
 */
TEST(f360_rectangle_zone1, Is_Inside__Returns_False_When_Object_Is_On_Front_Left_Corner)
{
   /** \precond
    * Set up vcs position to be placed on left front corner
    */
   const Rectangle_T rectangle(min_long, max_long, min_lat, max_lat);

   lateral = min_lat;
   longitudinal = max_long;

   /** \action
    * Call tested function
    */
   const bool f_inside = rectangle.Contains(longitudinal, lateral);

   /** \result
    * Check whether f_inside is false
    */
   CHECK_FALSE(f_inside);
}

/** \purpose
 * Purpose of this test is to verify whether function returns false when object is placed on right front corner of rectangle
 * \req
 * NA.
 */
TEST(f360_rectangle_zone1, Is_Inside__Returns_False_When_Object_Is_On_Front_Right_Corner)
{
   /** \precond
    * Set up vcs position to be placed on right front corner
    */
   const Rectangle_T rectangle(min_long, max_long, min_lat, max_lat);

   lateral = max_lat;
   longitudinal = max_long;

   /** \action
    * Call tested function
    */
   const bool f_inside = rectangle.Contains(longitudinal, lateral);

   /** \result
    * Check whether f_inside is false
    */
   CHECK_FALSE(f_inside);
}

/** \purpose
 * Purpose of this test is to verify whether function returns false when object is placed on right rear corner of rectangle
 * \req
 * NA.
 */
TEST(f360_rectangle_zone1, Is_Inside__Returns_False_When_Object_Is_On_Rear_Right_Corner)
{
   /** \precond
    * Set up vcs position to be placed on right rear corner
    */
   const Rectangle_T rectangle(min_long, max_long, min_lat, max_lat);

   lateral = max_lat;
   longitudinal = min_long;

   /** \action
    * Call tested function
    */
   const bool f_inside = rectangle.Contains(longitudinal, lateral);

   /** \result
    * Check whether f_inside is false
    */
   CHECK_FALSE(f_inside);
}

/** \purpose
 * Purpose of this test is to verify whether function returns false when object is placed on left rear corner of rectangle
 * \req
 * NA.
 */
TEST(f360_rectangle_zone1, Is_Inside__Returns_False_When_Object_Is_On_Left_Rear_Corner)
{
   /** \precond
    * Set up vcs position to be placed on right rear corner
    */
   const Rectangle_T rectangle(min_long, max_long, min_lat, max_lat);

   lateral = min_lat;
   longitudinal = min_long;

   /** \action
    * Call tested function
    */
   const bool f_inside = rectangle.Contains(longitudinal, lateral);

   /** \result
    * Check whether f_inside is false
    */
   CHECK_FALSE(f_inside);
}
/** @}*/
/** \defgroup  f360_rectangle_zone1
 *  @{
 */



 /** \brief
  * Test group of Rectangle_T class. Tests verify whether it is properly determined whether point is inside rectangle with set of parameters
  */
TEST_GROUP(f360_rectangle_zone2)
{
   float32_t min_long{};
   float32_t max_long{};
   float32_t min_lat{};
   float32_t max_lat{};

   float32_t lateral{};
   float32_t longitudinal{};

   /** \setup
    * Set up rectangle parameters
    */
   TEST_SETUP()
   {
      min_long = -1.0F;
      max_long = 15.0F;
      min_lat = 5.0F;
      max_lat = 7.0F;
   }
};

/** \purpose
 * Purpose of this test is to verify whether function returns true when object is in inside rectangle
 * \req
 * NA.
 */
TEST(f360_rectangle_zone2, Is_Inside__Returns_True_When_Position_In_Rectangle)
{
   /** \precond
    * Create rectangle object
    * Set up vcs position to be placed in rectangle
    */
   const Rectangle_T rectangle(min_long, max_long, min_lat, max_lat);

   lateral = 6.0F;
   longitudinal = 3.0F;

   /** \action
    * Call tested function
    */
   const bool f_inside = rectangle.Contains(longitudinal, lateral);

   /** \result
    * Check whether f_inside is true
    */
   CHECK_TRUE(f_inside);
}

/** \purpose
 * Purpose of this test is to verify whether function returns false when object lateral position is too low
 * \req
 * NA.
 */
TEST(f360_rectangle_zone2, Is_Inside__Returns_False_When_Too_Low_Lateral_Position)
{
   /** \precond
    * Set up vcs position to be placed outside of rectangle - far left
    */
   const Rectangle_T rectangle(min_long, max_long, min_lat, max_lat);

   lateral = min_lat - 5.0F;
   longitudinal = 2.0F;

   /** \action
    * Call tested function
    */
   const bool f_inside = rectangle.Contains(longitudinal, lateral);

   /** \result
    * Check whether f_inside is false
    */
   CHECK_FALSE(f_inside);
}

/** \purpose
 * Purpose of this test is to verify whether function returns false when object lateral position is too high
 * \req
 * NA.
 */
TEST(f360_rectangle_zone2, Is_Inside__Returns_False_When_Too_High_Lateral_Position)
{
   /** \precond
    * Set up vcs position to be placed outside of rectangle - far right
    */
   const Rectangle_T rectangle(min_long, max_long, min_lat, max_lat);

   lateral = max_lat + 5.0F;
   longitudinal = 2.0F;

   /** \action
    * Call tested function
    */
   const bool f_inside = rectangle.Contains(longitudinal, lateral);

   /** \result
    * Check whether f_inside is false
    */
   CHECK_FALSE(f_inside);
}

/** \purpose
 * Purpose of this test is to verify whether function returns false when object longitudinal position is too low
 * \req
 * NA.
 */
TEST(f360_rectangle_zone2, Is_Inside__Returns_False_When_Too_Low_Longitudinal_Position)
{
   /** \precond
    * Set up vcs position to be placed outside of rectangle - far before host
    */
   const Rectangle_T rectangle(min_long, max_long, min_lat, max_lat);

   lateral = -2.0F;
   longitudinal = min_long - 5.0F;

   /** \action
    * Call tested function
    */
   const bool f_inside = rectangle.Contains(longitudinal, lateral);

   /** \result
    * Check whether f_inside is false
    */
   CHECK_FALSE(f_inside);
}

/** \purpose
 * Purpose of this test is to verify whether function returns false when object longitudinal position is too high
 * \req
 * NA.
 */
TEST(f360_rectangle_zone2, Is_Inside__Returns_False_When_Too_High_Longitudinal_Position)
{
   /** \precond
    * Set up vcs position to be placed outside of rectangle - far before host
    */
   const Rectangle_T rectangle(min_long, max_long, min_lat, max_lat);

   lateral = -2.0F;
   longitudinal = max_long + 5.0F;

   /** \action
    * Call tested function
    */
   const bool f_inside = rectangle.Contains(longitudinal, lateral);

   /** \result
    * Check whether f_inside is false
    */
   CHECK_FALSE(f_inside);
}

/** \purpose
 * Purpose of this test is to verify whether function returns false when object is placed on left front corner of rectangle
 * \req
 * NA.
 */
TEST(f360_rectangle_zone2, Is_Inside__Returns_False_When_Object_Is_On_Front_Left_Corner)
{
   /** \precond
    * Set up vcs position to be placed on left front corner
    */
   const Rectangle_T rectangle(min_long, max_long, min_lat, max_lat);

   lateral = min_lat;
   longitudinal = max_long;

   /** \action
    * Call tested function
    */
   const bool f_inside = rectangle.Contains(longitudinal, lateral);

   /** \result
    * Check whether f_inside is false
    */
   CHECK_FALSE(f_inside);
}

/** \purpose
 * Purpose of this test is to verify whether function returns false when object is placed on right front corner of rectangle
 * \req
 * NA.
 */
TEST(f360_rectangle_zone2, Is_Inside__Returns_False_When_Object_Is_On_Front_Right_Corner)
{
   /** \precond
    * Set up vcs position to be placed on right front corner
    */
   const Rectangle_T rectangle(min_long, max_long, min_lat, max_lat);

   lateral = max_lat;
   longitudinal = max_long;

   /** \action
    * Call tested function
    */
   const bool f_inside = rectangle.Contains(longitudinal, lateral);

   /** \result
    * Check whether f_inside is false
    */
   CHECK_FALSE(f_inside);
}

/** \purpose
 * Purpose of this test is to verify whether function returns false when object is placed on right rear corner of rectangle
 * \req
 * NA.
 */
TEST(f360_rectangle_zone2, Is_Inside__Returns_False_When_Object_Is_On_Rear_Right_Corner)
{
   /** \precond
    * Set up vcs position to be placed on right rear corner
    */
   const Rectangle_T rectangle(min_long, max_long, min_lat, max_lat);

   lateral = max_lat;
   longitudinal = min_long;

   /** \action
    * Call tested function
    */
   const bool f_inside = rectangle.Contains(longitudinal, lateral);

   /** \result
    * Check whether f_inside is false
    */
   CHECK_FALSE(f_inside);
}

/** \purpose
 * Purpose of this test is to verify whether function returns false when object is placed on left rear corner of rectangle
 * \req
 * NA.
 */
TEST(f360_rectangle_zone2, Is_Inside__Returns_False_When_Object_Is_On_Left_Rear_Corner)
{
   /** \precond
    * Set up vcs position to be placed on right rear corner
    */
   const Rectangle_T rectangle(min_long, max_long, min_lat, max_lat);

   lateral = min_lat;
   longitudinal = min_long;

   /** \action
    * Call tested function
    */
   const bool f_inside = rectangle.Contains(longitudinal, lateral);

   /** \result
    * Check whether f_inside is false
    */
   CHECK_FALSE(f_inside);
}
/** @}*/
