/** \file
   File with unit test for Calculate_Curvi_Lat_Pos function.
*/

#include "f360_calculate_curvi_position.h"
#include "f360_constants.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>


/** \defgroup f360_calculate_curvi_position
*  @{
*/
using namespace f360_variant_A;
/** \brief
*  Test group for Calculate_Curvi_Lat_Pos
**/
TEST_GROUP(f360_calculate_curvi_position)
{
   /** \precond
   //Setting up host properties
   **/
   F360_Host_T host_props{};
   float32_t y_curvi_lat_pos = 0.0F;
   TEST_SETUP()
   {
     host_props.curvature_rear = 1.0F; // Default
   }
};

/**
*\purpose Test for checking proper calculation of curvi position at close range long_pos=10m
*/
TEST(f360_calculate_curvi_position, Curvi_Lat_Pos_Close_Range)
{
   host_props.curvature_rear = 0.01F;
   float32_t long_pos = 10.0F;
   float32_t lat_pos = 10.0F;
   float32_t expected_y_curvi_lat_pos = 9.5F;
   /** \action
   *Run Calculate_Curvi_Lat_Pos
   **/
    y_curvi_lat_pos = Calculate_Curvi_Lat_Pos(host_props, long_pos, lat_pos);
    /** \result
    * Checking if results matching the expectations
    **/
    DOUBLES_EQUAL(expected_y_curvi_lat_pos, y_curvi_lat_pos, F360_EPSILON);
}
/**
*\purpose Test for checking proper calculation of curvi position at mid range long_pos=35m
*/
TEST(f360_calculate_curvi_position, Curvi_Lat_Pos_Mid_Range)
{
   host_props.curvature_rear = 0.01F;
   float32_t long_pos = 35.0F;
   float32_t lat_pos = -6.0F;
   float32_t expected_y_curvi_lat_pos = -12.125F;
   /** \action
   *Run Calculate_Curvi_Lat_Pos
   **/
   y_curvi_lat_pos = Calculate_Curvi_Lat_Pos(host_props, long_pos, lat_pos);
   /** \result
   * Checking if results matching the expectations
   **/
   DOUBLES_EQUAL(expected_y_curvi_lat_pos, y_curvi_lat_pos, F360_EPSILON);
}
/**
*\purpose Test for checking proper calculation of curvi position at Far range long_pos=70m
*/
TEST(f360_calculate_curvi_position, Curvi_Lat_Pos_Far_Range)
{
   host_props.curvature_rear = 0.01F;
   float32_t long_pos = 70.0F;
   float32_t lat_pos = 10.0F;
   float32_t expected_y_curvi_lat_pos = -14.5F;
   /** \action
   *Run Calculate_Curvi_Lat_Pos
   **/
   y_curvi_lat_pos = Calculate_Curvi_Lat_Pos(host_props, long_pos, lat_pos);
   /** \result
   * Checking if results matching the expectations
   **/
   DOUBLES_EQUAL(expected_y_curvi_lat_pos, y_curvi_lat_pos, 100.0F * F360_EPSILON);
}
/**
*\purpose Test for checking proper calculation of curvi position when all input params are zero
*/
TEST(f360_calculate_curvi_position, Check_If_Function_Returns_Zero_When_All_Input_Params_Are_Zero)
{
   host_props.curvature_rear = 0.0F;
   float32_t long_pos = 0.0F;
   float32_t lat_pos = 0.0F;
   float32_t expected_y_curvi_lat_pos = 0.0F;
   /** \action
   *Run Calculate_Curvi_Lat_Pos
   **/
   y_curvi_lat_pos = Calculate_Curvi_Lat_Pos(host_props, long_pos, lat_pos);
   /** \result
   * Checking if results matching the expectations
   **/
   DOUBLES_EQUAL(expected_y_curvi_lat_pos, y_curvi_lat_pos, F360_EPSILON);
}
/** @}*/
