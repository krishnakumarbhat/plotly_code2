/** \file
 * This file contains unit tests for content of f360_radar_sensor.cpp file
 */

#include "f360_radar_sensor.h"

#include <map>
#include <utility>

#include <CppUTest/TestHarness.h>

// Unit testing guidelines: http://confluenceprod1.delphiauto.net:8090/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_radar_sensor
 *  @{
 */

 /** \brief
  * Tests mappings between look ID, look type, and range type.
  */
TEST_GROUP(f360_radar_sensor)
{};

/** \purpose
 * Verify that the look type of F360_DET_LOOK_ID_INVALID is F360_DET_LOOK_TYPE_INVALID.
 * \req
 * NA
 */
TEST(f360_radar_sensor, Get_Look_Type_F360_DET_LOOK_ID_INVALID)
{
   /** \precond
    * Look ID = F360_DET_LOOK_ID_INVALID
    */
   const F360_Det_Look_ID_T look_id = F360_DET_LOOK_ID_INVALID;

   /** \action
    * Get the look type for the specified look ID.
    */
   const F360_Det_Look_Type_T look_type = Get_Look_Type(look_id);

   /** \result
    * Expect look type = F360_DET_LOOK_TYPE_INVALID
    */
   CHECK_EQUAL(F360_DET_LOOK_TYPE_INVALID, look_type);
}

/** \purpose
 * Verify that the look type of F360_DET_LOOK_ID_0 is F360_DET_LOOK_TYPE_LONG.
 * \req
 * NA
 */
TEST(f360_radar_sensor, Get_Look_Type_F360_DET_LOOK_ID_0)
{
   /** \precond
    * Look ID = F360_DET_LOOK_ID_0
    */
   const F360_Det_Look_ID_T look_id = F360_DET_LOOK_ID_0;

   /** \action
    * Get the look type for the specified look ID.
    */
   const F360_Det_Look_Type_T look_type = Get_Look_Type(look_id);

   /** \result
    * Expect look type = F360_DET_LOOK_TYPE_LONG
    */
   CHECK_EQUAL(F360_DET_LOOK_TYPE_LONG, look_type);
}

/** \purpose
 * Verify that the look type of F360_DET_LOOK_ID_1 is F360_DET_LOOK_TYPE_MEDIUM.
 * \req
 * NA
 */
TEST(f360_radar_sensor, Get_Look_Type_F360_DET_LOOK_ID_1)
{
   /** \precond
    * Look ID = F360_DET_LOOK_ID_1
    */
   const F360_Det_Look_ID_T look_id = F360_DET_LOOK_ID_1;

   /** \action
    * Get the look type for the specified look ID.
    */
   const F360_Det_Look_Type_T look_type = Get_Look_Type(look_id);

   /** \result
    * Expect look type = F360_DET_LOOK_TYPE_MEDIUM
    */
   CHECK_EQUAL(F360_DET_LOOK_TYPE_MEDIUM, look_type);
}

/** \purpose
 * Verify that the look type of F360_DET_LOOK_ID_2 is F360_DET_LOOK_TYPE_LONG.
 * \req
 * NA
 */
TEST(f360_radar_sensor, Get_Look_Type_F360_DET_LOOK_ID_2)
{
   /** \precond
    * Look ID = F360_DET_LOOK_ID_INVALID
    */
   const F360_Det_Look_ID_T look_id = F360_DET_LOOK_ID_2;

   /** \action
    * Get the look type for the specified look ID.
    */
   const F360_Det_Look_Type_T look_type = Get_Look_Type(look_id);

   /** \result
    * Expect look type = F360_DET_LOOK_TYPE_LONG
    */
   CHECK_EQUAL(F360_DET_LOOK_TYPE_LONG, look_type);
}

/** \purpose
 * Verify that the look type of F360_DET_LOOK_ID_3 is F360_DET_LOOK_TYPE_MEDIUM.
 * \req
 * NA
 */
TEST(f360_radar_sensor, Get_Look_Type_F360_DET_LOOK_ID_3)
{
   /** \precond
    * Look ID = F360_DET_LOOK_ID_3
    */
   const F360_Det_Look_ID_T look_id = F360_DET_LOOK_ID_3;

   /** \action
    * Get the look type for the specified look ID.
    */
   const F360_Det_Look_Type_T look_type = Get_Look_Type(look_id);

   /** \result
    * Expect look type = F360_DET_LOOK_TYPE_MEDIUM
    */
   CHECK_EQUAL(F360_DET_LOOK_TYPE_MEDIUM, look_type);
}

/** \purpose
 * Verify that the look type of F360_DET_NUM_LOOK_ID is F360_DET_LOOK_TYPE_INVALID.
 * \req
 * NA
 */
TEST(f360_radar_sensor, Get_Look_Type_F360_DET_NUM_LOOK_ID)
{
   /** \precond
    * Look ID = F360_DET_NUM_LOOK_ID
    */
   const F360_Det_Look_ID_T look_id = F360_DET_NUM_LOOK_ID;

   /** \action
    * Get the look type for the specified look ID.
    */
   const F360_Det_Look_Type_T look_type = Get_Look_Type(look_id);

   /** \result
    * Expect look type = F360_DET_LOOK_TYPE_INVALID
    */
   CHECK_EQUAL(F360_DET_LOOK_TYPE_INVALID, look_type);
}

/** \purpose
 * Verify that the range type of F360_DET_LOOK_ID_INVALID is F360_DET_RANGE_TYPE_INVALID.
 * \req
 * NA
 */
TEST(f360_radar_sensor, Get_Range_Type_F360_DET_LOOK_ID_INVALID)
{
   /** \precond
    * Look ID = F360_DET_LOOK_ID_INVALID
    */
   const F360_Det_Look_ID_T look_id = F360_DET_LOOK_ID_INVALID;

   /** \action
    * Get the range type for the specified look ID.
    */
   const F360_Det_Range_Type_T range_type = Get_Range_Type(look_id);

   /** \result
    * Expect range type = F360_DET_RANGE_TYPE_INVALID
    */
   CHECK_EQUAL(F360_DET_RANGE_TYPE_INVALID, range_type);
}

/** \purpose
 * Verify that the range type of F360_DET_LOOK_ID_0 is F360_DET_RANGE_TYPE_LONG.
 * \req
 * NA
 */
TEST(f360_radar_sensor, Get_Range_Type_F360_DET_LOOK_ID_0)
{
   /** \precond
    * Look ID = F360_DET_LOOK_ID_0
    */
   const F360_Det_Look_ID_T look_id = F360_DET_LOOK_ID_0;

   /** \action
    * Get the range type for the specified look ID.
    */
   const F360_Det_Range_Type_T range_type = Get_Range_Type(look_id);

   /** \result
    * Expect range type = F360_DET_RANGE_TYPE_LONG
    */
   CHECK_EQUAL(F360_DET_RANGE_TYPE_LONG, range_type);
}

/** \purpose
 * Verify that the range type of F360_DET_LOOK_ID_1 is F360_DET_RANGE_TYPE_LONG.
 * \req
 * NA
 */
TEST(f360_radar_sensor, Get_Range_Type_F360_DET_LOOK_ID_1)
{
   /** \precond
    * Look ID = F360_DET_LOOK_ID_1
    */
   const F360_Det_Look_ID_T look_id = F360_DET_LOOK_ID_1;

   /** \action
    * Get the range type for the specified look ID.
    */
   const F360_Det_Range_Type_T range_type = Get_Range_Type(look_id);

   /** \result
    * Expect range type = F360_DET_RANGE_TYPE_LONG
    */
   CHECK_EQUAL(F360_DET_RANGE_TYPE_LONG, range_type);
}

/** \purpose
 * Verify that the range type of F360_DET_LOOK_ID_2 is F360_DET_RANGE_TYPE_MEDIUM.
 * \req
 * NA
 */
TEST(f360_radar_sensor, Get_Range_Type_F360_DET_LOOK_ID_2)
{
   /** \precond
    * Look ID = F360_DET_LOOK_ID_2
    */
   const F360_Det_Look_ID_T look_id = F360_DET_LOOK_ID_2;

   /** \action
    * Get the range type for the specified look ID.
    */
   const F360_Det_Range_Type_T range_type = Get_Range_Type(look_id);

   /** \result
    * Expect range type = F360_DET_RANGE_TYPE_MEDIUM
    */
   CHECK_EQUAL(F360_DET_RANGE_TYPE_MEDIUM, range_type);
}

/** \purpose
 * Verify that the range type of F360_DET_LOOK_ID_3 is F360_DET_RANGE_TYPE_MEDIUM.
 * \req
 * NA
 */
TEST(f360_radar_sensor, Get_Range_Type_F360_DET_LOOK_ID_3)
{
   /** \precond
    * Look ID = F360_DET_LOOK_ID_3
    */
   const F360_Det_Look_ID_T look_id = F360_DET_LOOK_ID_3;

   /** \action
    * Get the range type for the specified look ID.
    */
   const F360_Det_Range_Type_T range_type = Get_Range_Type(look_id);

   /** \result
    * Expect range type = F360_DET_RANGE_TYPE_MEDIUM
    */
   CHECK_EQUAL(F360_DET_RANGE_TYPE_MEDIUM, range_type);
}

/** \purpose
 * Verify that the range type of F360_DET_NUM_LOOK_ID is F360_DET_RANGE_TYPE_INVALID.
 * \req
 * NA
 */
TEST(f360_radar_sensor, Get_Range_Type_F360_DET_NUM_LOOK_ID)
{
   /** \precond
    * Look ID = F360_DET_NUM_LOOK_ID
    */
   const F360_Det_Look_ID_T look_id = F360_DET_NUM_LOOK_ID;

   /** \action
    * Get the range type for the specified look ID.
    */
   const F360_Det_Range_Type_T range_type = Get_Range_Type(look_id);

   /** \result
    * Expect range type = F360_DET_RANGE_TYPE_INVALID
    */
   CHECK_EQUAL(F360_DET_RANGE_TYPE_INVALID, range_type);
}
/** @}*/
