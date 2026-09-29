/** \file
 * This file contains unit tests for content of f360_define_dead_zone.cpp file
 */

#include "f360_define_dead_zone.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_define_dead_zone
 *  @{
 */

/** \brief
 * Test group of Define_Dead_Zone function. Tests verify whether extended dead zone is properly defined
 */
TEST_GROUP(f360_define_dead_zone)
{
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] {};
   const float32_t k_dead_zone_extended_long_limit = 5.0F;
   
   /** \setup
    * Set up longitudinal positions of two selected sensors
    */
   TEST_SETUP()
   {
      sensors[0].variable.is_valid = true;
      sensors[0].constant.mounting_position.vcs_position.longitudinal = 1.0F;

      sensors[1].variable.is_valid = true;
      sensors[1].constant.mounting_position.vcs_position.longitudinal = -7.0F;

      sensors[2].variable.is_valid = true;
      sensors[2].constant.mounting_position.vcs_position.longitudinal = -2.0F;
   }
};

/** \purpose  
 * Purpose of this test is to verify whether extended zone properly extends simplified dead zone
 * \req
 * NA.
 */
TEST(f360_define_dead_zone, Define_Dead_Zone__Extended_Zone_Has_Proper_Limits)
{
   /** \precond
    * Three valid sensors were set in TEST_SETUP()
    * Longitudinal positions of sesnors were set to: [-7.0, -2.0, 1.0] [m]
    * Extension parameter was set to 5 [m]
    */
	
   /** \action
    * Call tested function
    */
   const Dead_Zone_T dead_zone = Define_Dead_Zone(sensors, k_dead_zone_extended_long_limit);

   /** \result
    * Check whether front limit is equal to 1.0F
    * Check whether extended front limit is equal to 6.0F
    * Check whether rear limit is equal to -7.0F
    * Check whether extened rear limit is equal to -12.0F
    */	
   DOUBLES_EQUAL(1.0F, dead_zone.basic.upper, F360_EPSILON);
   DOUBLES_EQUAL(6.0F, dead_zone.extended.upper, F360_EPSILON);
   DOUBLES_EQUAL(-7.0F, dead_zone.basic.lower, F360_EPSILON);
   DOUBLES_EQUAL(-12.0F, dead_zone.extended.lower, F360_EPSILON);
}
/** @}*/
