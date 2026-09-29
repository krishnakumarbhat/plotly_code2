/** \file
   This file contains unit tests for content of f360_norm_heading_angle.cpp file
*/

#include "f360_norm_heading_angle.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include <cfloat>
#include <cmath>
#include <cstdio>

/** \defgroup  f360_norm_heading_angle
 *  @{
 */

using namespace f360_variant_A;
/** \brief
*  This test group contains tests for Normalize_Heading_Angle function.
*  Normalize_Heading_Angle
**/
TEST_GROUP(f360_norm_heading_angle)
{
   /** \setup
   * Nothing to setup in this test group
   **/
   TEST_SETUP()
   {
   }

   /** \teardown
   * Nothing to teardown in this test group
   **/
   TEST_TEARDOWN()
   {
   }

};

/**
*\purpose  Test functionally of Normalize_Heading_Angle function.
*\req    NA
*/
TEST(f360_norm_heading_angle, TestCall2Function_zero_input)
{
   /** \step{1}
    * Test with zero input
    **/

   /** \precond
   * Set up zero data input for testing function
   **/
   float32_t inputAngle = 0.0F;
   float32_t inputIntervalCenter = 0.0F;
   float32_t expectedAngle = 0.0F;

   /** \action
   * Execute the function.
   **/
   float retVal= Normalize_Heading_Angle(inputAngle, inputIntervalCenter);

   /** \result
   *Check that function returns a normalized heading angle
   **/
   DOUBLES_EQUAL_TEXT(expectedAngle, retVal, FLT_EPSILON, "zero input test");

   /** \step{2}
    * Test with Max Angle
    **/

   /** \precond
   * Set angle to maximum values
   **/
   inputAngle = FLT_MAX;
   inputIntervalCenter = 0.0F;
   expectedAngle = -M_PI;

   /** \action
   * Execute the function.
   **/
   retVal= Normalize_Heading_Angle(inputAngle, inputIntervalCenter);

   /** \result
   *Check that function returns a normalized heading angle
   **/
   DOUBLES_EQUAL_TEXT(expectedAngle, retVal, FLT_EPSILON, "max input angle");

   /** \step{3}
    * Test with minimum Angle
    **/

   /** \precond
   * Set angle to minimum values
   **/
   inputAngle = FLT_MIN;
   inputIntervalCenter = 0.0F;
   expectedAngle = 0.0F;

   /** \action
   * Execute the function.
   **/
   retVal= Normalize_Heading_Angle(inputAngle, inputIntervalCenter);

   /** \result
   *Check that function returns a normalized heading angle
   **/
   DOUBLES_EQUAL_TEXT(expectedAngle, retVal, FLT_EPSILON, "min input angle");

   /** \step{4}
    * Test with maximum interval center value.
    **/

   /** \precond
   * Set center to maximum values
   **/
   inputAngle = 0.0F;
   inputIntervalCenter = FLT_MAX;
   expectedAngle = FLT_MAX;

   /** \action
   * Execute the function.
   **/
   retVal= Normalize_Heading_Angle(inputAngle, inputIntervalCenter);

   /** \result
   *Check that function returns a normalized heading angle
   **/
   DOUBLES_EQUAL_TEXT(expectedAngle, retVal, FLT_EPSILON, "max interval center");

   /** \step{5}
    * Test with minimum interval center value.
    **/

   /** \precond
   * Set center to minimum value.
   **/
   inputAngle = 0.0F;
   inputIntervalCenter = FLT_MIN;
   expectedAngle = 0.0F;
   /** \action
   * Execute the function.
   **/
   retVal= Normalize_Heading_Angle(inputAngle, inputIntervalCenter);

   /** \result
   *Check that function returns a normalized heading angle
   **/
   DOUBLES_EQUAL_TEXT(expectedAngle, retVal, FLT_EPSILON, "min interval center");

   /** \step{6}
    * Test with values in typical range of parameters between -PI and +PI.
    **/

   /** \precond
   * Set the arrays with inputs .
   **/
   char textFailure[100];
   const uint8_t DIM = 5U;
   const float32_t PI_1_2 = M_PI / 2.0;
   const float32_t PI_3_2 = 3.0 * M_PI / 2.0;
   const float32_t PI_2 = 2.0 * M_PI;
  
   float32_t inputAngleList[DIM] =          {-M_PI, -PI_1_2,  0.0F,     PI_1_2,   M_PI};
   float32_t inputIntervalCenterList[DIM] = {-M_PI, -PI_1_2,  0.0F,     PI_1_2,   M_PI};

   float32_t expectedNormList[DIM][DIM] = { {-M_PI, -PI_1_2,  -PI_2,    -PI_3_2,  -M_PI},
                                             {-M_PI, -PI_1_2,  0.0F,     -PI_3_2,  -M_PI},
                                             {-M_PI, -PI_1_2,  0.0F,     PI_1_2,   -M_PI},
                                             {M_PI,  -PI_1_2,  0.0F,     PI_1_2,   M_PI},
                                             {M_PI,  PI_3_2,   0.0F,     PI_1_2,   M_PI} };
   for (uint8_t i = 0U; i < DIM ; i++)
   {
      for (uint8_t j = 0U; j < DIM ; j++)
      {
         /** \action
         * Execute the function.
         **/
         retVal= Normalize_Heading_Angle(inputAngleList[j], inputIntervalCenterList[i]);

         /** \result
         *Check that function returns a normalized heading angle
         **/
         (void)sprintf(textFailure, "Test on index interval=%d, angle=%d failed: \n", i ,j);
         DOUBLES_EQUAL_TEXT(expectedNormList[i][j], retVal, FLT_EPSILON * 2.0F, textFailure);
      }
   }
}

/** @}*/
