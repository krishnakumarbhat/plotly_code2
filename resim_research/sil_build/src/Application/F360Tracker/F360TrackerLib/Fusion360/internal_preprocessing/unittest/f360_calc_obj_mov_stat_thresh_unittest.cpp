/** \file
   Give a detailed description of what  this unit-test file contain.
 */

#include "f360_calc_obj_mov_stat_thresh.h"
#include "f360_host.h"
#include "f360_timing_info.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

//#include "headerfile_needed.h"

//sneak in mocked functions
//Declaration of stubbed/mock functions

//Implementation of stubbed interfaces

/** \defgroup  f360_calc_obj_mov_stat_thresh
 *  @{
 */
using namespace f360_variant_A;
/** \brief
The test suite for f360_calc_obj_mov_stat_thresh below tests the only function in that file
Calc_Obj_Mov_Stat_Thresh.

For an input host speed this function provides a threshold value as output according to the cases below:

Case 1: Inputs below 10 m/s produces an output threshold value of 1.4.
Case 2: Inputs above 15 m/s produces an output of 1.5.
Case 3: Inputs between 10 and 15 m/s produces an output that is relative to the speed (between 1.4 and 1.5).


The test suite aims to test all of these cases including boundaries.
 **/
TEST_GROUP(f360_calc_obj_mov_stat_thresh)
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
      //mock.clear();
   }

};

/**
 *\purpose
Check that input host speeds for case 1 produce the correct output.
 *\req NA
 */

TEST(f360_calc_obj_mov_stat_thresh, Calc_Obj_Mov_Stat_Thresh_case1)
{
   /** \step{1}
   Test for case 1 with arbitrary positive input
    **/

   /** \precond
   Set up function input arguments, arbitrary positive input
    **/
   float32_t host_vcs_speed1;
   host_vcs_speed1 = 9.0F;

   F360_TRKR_TIMING_INFO_T timing_info1;

   /** \action
   Call function
    **/
   float32_t out1 = f360_variant_A::Calc_Obj_Mov_Stat_Thresh(host_vcs_speed1, &timing_info1);

   /** \result
    *Actual float value compared to expected float value with FLT_EPSILON tolerance
    **/
   DOUBLES_EQUAL(1.4f, out1, FLT_EPSILON);

   /** \step{2}
   Test for case 1 with arbitrary negative input
    **/

   /** \precond
   Set up function input arguments, arbitrary negative input
    **/
   float32_t host_vcs_speed2;
   host_vcs_speed2 = -9.0F;

   F360_TRKR_TIMING_INFO_T timing_info2;

   /** \action
   Call function
    **/
   float32_t out2 = f360_variant_A::Calc_Obj_Mov_Stat_Thresh(host_vcs_speed2, &timing_info2);

   /** \result
    *Actual float value compared to expected float value with FLT_EPSILON tolerance
    **/
   DOUBLES_EQUAL(1.4f, out2, FLT_EPSILON);

   /** \step{3}
   Test for case 1 with zero input
    **/

   /** \precond
   Set up function input arguments, zero input
    **/
   float32_t host_vcs_speed3;
   host_vcs_speed3 = 0.0F;

   F360_TRKR_TIMING_INFO_T timing_info3;

   /** \action
   Call function
    **/
   float32_t out3 = f360_variant_A::Calc_Obj_Mov_Stat_Thresh(host_vcs_speed3, &timing_info3);

   /** \result
    *Actual float value compared to expected float value with FLT_EPSILON tolerance
    **/
   DOUBLES_EQUAL(1.4f, out3, FLT_EPSILON);

   /** \step{4}
   Test for case 1 with minimum float value input
    **/

   /** \precond
   Set up function input arguments, input: FLT_MIN
    **/
   float32_t host_vcs_speed4;
   host_vcs_speed4 = FLT_MIN;

   F360_TRKR_TIMING_INFO_T timing_info4;

   /** \action
   Call function
    **/
   float32_t out4 = f360_variant_A::Calc_Obj_Mov_Stat_Thresh(host_vcs_speed4, &timing_info4);

   /** \result
    *Actual float value compared to expected float value with FLT_EPSILON tolerance
    **/
   DOUBLES_EQUAL(1.4f, out4, FLT_EPSILON);

   /** \step{5}
   Test for case 1 with maximum negative float value input
    **/

   /** \precond
   Set up function input arguments, input: -FLT_MAX
    **/
   float32_t host_vcs_speed5;
   host_vcs_speed5 = -FLT_MAX;

   F360_TRKR_TIMING_INFO_T timing_info5;

   /** \action
   Call function
    **/
   float32_t out5 = f360_variant_A::Calc_Obj_Mov_Stat_Thresh(host_vcs_speed5, &timing_info5);

   /** \result
    *Actual float value compared to expected float value with FLT_EPSILON tolerance
    **/
   DOUBLES_EQUAL(1.4f, out5, FLT_EPSILON);
}


/**
 *\purpose
Check that input host speeds on the boundary for case 1 produce the correct output.
 *\req NA
 */
TEST(f360_calc_obj_mov_stat_thresh, Calc_Obj_Mov_Stat_Thresh_boundary1)
{
   /** \step{1}
   Test for boundary behavior of case 1
    **/

   /** \precond
   Set up function input arguments, input: case boundary value
    **/
   float32_t host_vcs_speed1;
   host_vcs_speed1 = 10.0F;

   F360_TRKR_TIMING_INFO_T timing_info1;

   /** \action
   Call function
    **/
   float32_t out1 = f360_variant_A::Calc_Obj_Mov_Stat_Thresh(host_vcs_speed1, &timing_info1);

   /** \result
    *Actual float value compared to expected float value with FLT_EPSILON tolerance
    **/
   DOUBLES_EQUAL(1.4f, out1, FLT_EPSILON);
}


/**
 *\purpose
Check that input host speeds for case 2 produce the correct output.
 *\req NA
 */
TEST(f360_calc_obj_mov_stat_thresh, Calc_Obj_Mov_Stat_Thresh_case2)
{
   /** \step{1}
   Test for case 2 with arbitrary positive input
    **/

   /** \precond
   Set up function input arguments, arbitrary positive input
    **/
   float32_t host_vcs_speed1;
   host_vcs_speed1 = 20.0F;

   F360_TRKR_TIMING_INFO_T timing_info1;

   /** \action
   Call function
    **/
   float32_t out1 = f360_variant_A::Calc_Obj_Mov_Stat_Thresh(host_vcs_speed1, &timing_info1);

   /** \result
    *Actual float value compared to expected float value with FLT_EPSILON tolerance
    **/
   DOUBLES_EQUAL(1.5f, out1, FLT_EPSILON);

   /** \step{2}
   Test for case 2 with maximum float value input
    **/

   /** \precond
   Set up function input arguments, input: FLT_MAX
    **/
   float32_t host_vcs_speed2;
   host_vcs_speed2 = FLT_MAX;

   F360_TRKR_TIMING_INFO_T timing_info2;

   /** \action
   Call function
    **/
   float32_t out2 = f360_variant_A::Calc_Obj_Mov_Stat_Thresh(host_vcs_speed2, &timing_info2);

   /** \result
    *Actual float value compared to expected float value with FLT_EPSILON tolerance
    **/
   DOUBLES_EQUAL(1.5f, out2, FLT_EPSILON);
}


/**
 *\purpose
Check that input host speeds on the boundary of case 2 produce the correct output.
 *\req NA
 */
TEST(f360_calc_obj_mov_stat_thresh, Calc_Obj_Mov_Stat_Thresh_boundary2)
{
   /** \step{1}
   Test for boundary behavior of case 2
    **/

   /** \precond
   Set up function input arguments, input: case boundary value
    **/
   float32_t host_vcs_speed1;
   host_vcs_speed1 = 15.0F;

   F360_TRKR_TIMING_INFO_T timing_info1;

   /** \action
   Call function
    **/
   float32_t out1 = f360_variant_A::Calc_Obj_Mov_Stat_Thresh(host_vcs_speed1, &timing_info1);

   /** \result
    *Actual float value compared to expected float value with FLT_EPSILON tolerance
    **/
   DOUBLES_EQUAL(1.5f, out1, FLT_EPSILON);
}

/**
 *\purpose
Check that input host speeds for case 3 produce the correct output.
 *\req NA
 */
TEST(f360_calc_obj_mov_stat_thresh, Calc_Obj_Mov_Stat_Thresh_case3)
{
   /** \step{1}
   Test for case 3 with arbitrary positive input
    **/

   /** \precond
   Set up function input arguments, arbitrary input
    **/
   float32_t host_vcs_speed1;
   host_vcs_speed1 = 12.0F;

   F360_TRKR_TIMING_INFO_T timing_info1;

   /** \action
   Call function
    **/
   float32_t out1 = f360_variant_A::Calc_Obj_Mov_Stat_Thresh(host_vcs_speed1, &timing_info1);

   /** \result
    *Actual float value compared to expected float value with FLT_EPSILON tolerance
    **/
   DOUBLES_EQUAL(1.44f, out1, FLT_EPSILON);

   /** \step{2}
   Test for case 3 with arbitrary positive input
    **/

   /** \precond
   Set up function input arguments, arbitrary input
    **/
   float32_t host_vcs_speed2;
   host_vcs_speed2 = 14.0F;

   F360_TRKR_TIMING_INFO_T timing_info2;

   /** \action
   Call function
    **/
   float32_t out2 = f360_variant_A::Calc_Obj_Mov_Stat_Thresh(host_vcs_speed2, &timing_info2);

   /** \result
    *Actual float value compared to expected float value with FLT_EPSILON tolerance
    **/
   DOUBLES_EQUAL(1.48f, out2, FLT_EPSILON);
}
/** @}*/
