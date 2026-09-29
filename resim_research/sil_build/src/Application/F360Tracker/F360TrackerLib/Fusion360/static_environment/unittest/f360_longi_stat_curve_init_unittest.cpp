/** \file
 * This file contains unit tests for content of f360_longi_stat_curve_init.cpp file
 */

#include "f360_longi_stat_curve_init.h"
#include <CppUTest/TestHarness.h>


using namespace f360_variant_A;

/** \defgroup  f360_longi_stat_curve_init
 *  @{
 */

/** \brief
 * Create a set of longi stat curves and fill with arbitrary values. Purpose of this test group is to
 * define some curves and verify that they are reset correctly.
 */
TEST_GROUP(f360_longi_stat_curve_init)
{

   F360_Longi_Stat_Curve_T longi_stat_curves[MAX_NR_OF_LONGITUDINAL_STAT_CURVES] = {};
   
   /** \setup
    * Set arbitrary values to the curves
    */
   TEST_SETUP()
   {
      for (uint32_t i = 0U; i < MAX_NR_OF_LONGITUDINAL_STAT_CURVES; i++)
      {
         longi_stat_curves[i].f_valid = true;
         longi_stat_curves[i].x_min = -100.0F;
         longi_stat_curves[i].x_max = 100.0F;
         longi_stat_curves[i].a = 1.0F;
         longi_stat_curves[i].b = 1.0F;
         longi_stat_curves[i].c = 1.0F;
         longi_stat_curves[i].mean_lat_pos = 10.0F;
      }
   }

};

/** \purpose  
 * Verify that all the curves have been reset correctly.
 * \req
 * NA
 */
TEST(f360_longi_stat_curve_init, F360_Longi_Stat_Curve_Init_Verify_Correct_Reset)
{

   /** \action
   * Call function
   */
   F360_Longi_Stat_Curve_Init(longi_stat_curves);

   /** \result
   * Verify that curves have been reset
   */
   for (uint32_t i = 0U; i < MAX_NR_OF_LONGITUDINAL_STAT_CURVES; i++)
   {
      CHECK_FALSE(longi_stat_curves[i].f_valid);
      DOUBLES_EQUAL(0.0F, longi_stat_curves[i].x_min, F360_EPSILON);
      DOUBLES_EQUAL(0.0F, longi_stat_curves[i].x_max, F360_EPSILON);
      DOUBLES_EQUAL(0.0F, longi_stat_curves[i].a, F360_EPSILON);
      DOUBLES_EQUAL(0.0F, longi_stat_curves[i].b, F360_EPSILON);
      DOUBLES_EQUAL(0.0F, longi_stat_curves[i].c, F360_EPSILON);
      DOUBLES_EQUAL(0.0F, longi_stat_curves[i].mean_lat_pos, INFTY);
   }
}
/** @}*/
