/** \file
 * This file contains unit tests for content of f360_define_simplified_dead_zone.cpp file
 */

#include "f360_define_simplified_dead_zone.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_define_simplified_dead_zone
 *  @{
 */

 /** \brief
  * Test group of Define_Simplified_Dead_Zone function. Tests verify whether longitudinal limits of dead zone are properly picked.
  */
TEST_GROUP(f360_define_simplified_dead_zone)
{
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};

   /** \setup
    * Initialize two sensors parameters to make dead zone be defined in range <-5.0F, 0.5F>
    */
   TEST_SETUP()
   {
      sensors[0].variable.is_valid = true;
      sensors[0].constant.mounting_position.vcs_position.longitudinal = 0.5F;

      sensors[1].variable.is_valid = true;
      sensors[1].constant.mounting_position.vcs_position.longitudinal = -5.0F;
   }
};

/** \purpose
 * Purpose of this test is to verify whether function properly picks extreme sensor longitudinal positions
 * \req
 * NA.
 */
TEST(f360_define_simplified_dead_zone, Define_Simplified_Dead_Zone__Extreme_Positions_Properly_Picked)
{
   /** \precond
    * All is set in TEST_SETUP
    */

    /** \action
     * call tested function
     */
   const Interval<float32_t> zone = Define_Simplified_Dead_Zone(sensors);

   /** \result
    * Check whether front limit is equal to 0.5F
    * Check whether rear limit is equal to -5.0F
    */
   DOUBLES_EQUAL(0.5F, zone.upper, F360_EPSILON);
   DOUBLES_EQUAL(-5.0F, zone.lower, F360_EPSILON);
}

/** \purpose
 * Purpose of this test is to verify whether when there are no valid sensors
 * function returns zone defined as <-INFTY, INFTY>
 * \req
 * NA.
 */
TEST(f360_define_simplified_dead_zone, Define_Simplified_Dead_Zone__No_Valid_Sensors)
{
   /** \precond
    * All is set in TEST_SETUP
    * Reset sensors is_valid to false
    */
   sensors[0].variable.is_valid = false;
   sensors[1].variable.is_valid = false;

   /** \action
    * call tested function
    */
   const Interval<float32_t> zone = Define_Simplified_Dead_Zone(sensors);

   /** \result
    * Check whether front limit is equal to -INFTY
    * Check whether rear limit is equal to INFTY
    */
   DOUBLES_EQUAL(-INFTY, zone.upper, F360_EPSILON);
   DOUBLES_EQUAL(INFTY, zone.lower, F360_EPSILON);
}
/** @}*/
