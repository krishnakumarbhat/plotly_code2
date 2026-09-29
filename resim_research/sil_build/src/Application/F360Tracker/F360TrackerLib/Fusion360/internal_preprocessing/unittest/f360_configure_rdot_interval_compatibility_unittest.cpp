/** \file
Unit test for rdot interval compatability, validates the different compatible intervals for various otg speeed cases.
 */

#include "f360_configure_rdot_interval_compatibility.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>
using namespace f360_variant_A;

bool rdot_interval_compatability_status (
      float32_t unique_rdot_interval_widths_index,
      float32_t unique_rdot_interval_widths_index_1,
      float32_t max_range_of_rdot_comp);

/** \defgroup  f360_configure_rdot_interval_compatibility
 *  @{
 */

/** \brief
 *  The tests check the different rdot compatible intervals for various otg speeed cases
 **/
TEST_GROUP(f360_configure_rdot_interval_compatibility)
{
   /** \setup
    * Initialize the data for all the test cases, by making all sensors valid and taking 4 different range rate interval widths
    **/
   F360_Tracker_Info_T tracker_info = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals = {};
   F360_Calibrations_T calibs = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};

   int num_unique_intervals;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      num_unique_intervals = 4;
      globals.max_otg_speed = 0;

      for (int32_t index = 0; index < MAX_NUMBER_OF_SENSORS; index++)
      {

         sensors[index].variable.is_valid = true;

            sensors[index].constant.v_wrapping[0] = 5;
            sensors[index].constant.v_wrapping[1] = 10;
            sensors[index].constant.v_wrapping[2] = 15;
            sensors[index].constant.v_wrapping[3] = 20;


      }
   }

   /** \teardown
    * Nothing to teardown in this test group
    **/
   TEST_TEARDOWN()
   {

   }

};

/** \defgroup  f360_configure_rdot_interval_non_compatibility
 *  @{
 */

/** \brief
 *  The tests check the different rdot non compatible intervals for various otg speeed cases
 **/
TEST_GROUP(f360_configure_rdot_interval_non_compatibility)
{
   /** \setup
    * Initialize the data for all the test cases, by making all sensors valid and taking 4 different range rate interval widths
    **/
   F360_Tracker_Info_T tracker_info = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals = {};
   F360_Calibrations_T calibs = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};

   int num_unique_intervals;

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      num_unique_intervals = 4;
      globals.max_otg_speed = 0;

      for (int32_t index = 0; index < MAX_NUMBER_OF_SENSORS; index++)
      {

         sensors[index].variable.is_valid = true;

            sensors[index].constant.v_wrapping[0] = 0;
            sensors[index].constant.v_wrapping[1] = 10;
            sensors[index].constant.v_wrapping[2] = 15;
            sensors[index].constant.v_wrapping[3] = 20;


      }
   }

   /** \teardown
    * Nothing to teardown in this test group
    **/
   TEST_TEARDOWN()
   {

   }

};



/**
 *\purpose  This tests that all rdot intervals are compatible at 0 max_otg_speed
 *\req
 */

TEST(f360_configure_rdot_interval_compatibility, all_intervals_compatible)
{

   /** \precond
    Set the max_otg_speed to 0
    **/
   globals.max_otg_speed = 0;

   /** \action
    *Call the function Configure_Rdot_Interval_Compatibility to verify the interval compatibility
    **/
   Configure_Rdot_Interval_Compatibility(sensors, globals, calibs, tracker_info, timing_info);
   /** \result
    *At 0 max_otg_speed and 4 different unique intervals, the rdot_interval_compatibility should be set to true for 4 intervals
    **/
   for (int i = 0; i< num_unique_intervals; i++)
   {
      for (int j = 0; j< num_unique_intervals; j++)
         {
         CHECK_EQUAL(tracker_info.rdot_interval_compatibility[i][j], true);
         }

   }


}


/**
 *\purpose  This tests that rdot interval that is close the range rate interval width is compatible
 *\req
 */
TEST(f360_configure_rdot_interval_compatibility, only_one_interval_compatible)
{
   /** \step{1}
    *describe test step
    **/

   /** \precond
    describe preconditions
    **/
   globals.max_otg_speed = 18;

   /** \action
    *Call the function Configure_Rdot_Interval_Compatibility to verify the interval compatibility
    **/
   Configure_Rdot_Interval_Compatibility(sensors, globals, calibs, tracker_info, timing_info);
   /** \result
    *rdot interval that is close the range rate interval width is compatible
    **/
   CHECK_EQUAL(tracker_info.rdot_interval_compatibility[2][3], true);
   CHECK_EQUAL(tracker_info.rdot_interval_compatibility[3][2], true);

}


/**
 *\purpose  This tests that no rdot interval is compatible for an unrealistic max_otg_speed
 *\req
 */

TEST(f360_configure_rdot_interval_compatibility, no_interval_compatible)
{
   /** \precond
    unrealistic max_otg_speed = 50
    **/
   globals.max_otg_speed = 50;

   /** \action
    *Call the function Configure_Rdot_Interval_Compatibility to verify the interval compatibility
    **/
   Configure_Rdot_Interval_Compatibility(sensors, globals, calibs, tracker_info, timing_info);
   /** \result
     *At unrealistic max_otg_speed and 4 different unique intervals, the rdot_interval_compatibility should be set to false for all intervals
     **/
   for (int i = 0; i< MAX_NUMBER_OF_SENSORS*num_unique_intervals; i++)
   {
      for (int j = 0; j< MAX_NUMBER_OF_SENSORS*num_unique_intervals; j++)
         {
         CHECK_EQUAL(tracker_info.rdot_interval_compatibility[i][j], false);
         }

   }

}



/**
 *\purpose  This tests that all rdot intervals are not compatible when one of the v_wrapping is 0
 *\req
 */

TEST(f360_configure_rdot_interval_non_compatibility, one_interval_non_compatible)
{

   /** \precond
    Set the max_otg_speed to 0
    **/
   globals.max_otg_speed = 0;

   /** \action
    *Call the function Configure_Rdot_Interval_Compatibility to verify the interval non compatibility
    **/
   Configure_Rdot_Interval_Compatibility(sensors, globals, calibs, tracker_info, timing_info);
   /** \result
    *At 0 max_otg_speed and 4 different unique intervals, the rdot_interval_compatibility should be set to true for 4 intervals
    **/

    CHECK_EQUAL(tracker_info.rdot_interval_compatibility[0][1], false);
    CHECK_EQUAL(tracker_info.rdot_interval_compatibility[1][0], false);
    


}

/**
 *\purpose  This tests that all rdot intervals are not compatible when second of the v_wrapping is 0
 *\req
 */

TEST(f360_configure_rdot_interval_non_compatibility, second_interval_non_compatible)
{

   /** \precond
    Set the max_otg_speed to 0
    **/
   globals.max_otg_speed = 0;
         for (int32_t index = 0; index < MAX_NUMBER_OF_SENSORS; index++)
      {

         sensors[index].variable.is_valid = true;

            sensors[index].constant.v_wrapping[0] = 5;
            sensors[index].constant.v_wrapping[1] = 0;



      }

   /** \action
    *Call the function Configure_Rdot_Interval_Compatibility to verify the interval non compatibility
    **/
   Configure_Rdot_Interval_Compatibility(sensors, globals, calibs, tracker_info, timing_info);
   /** \result
    *At 0 max_otg_speed and 4 different unique intervals, the rdot_interval_compatibility should be set to true for 4 intervals
    **/

    CHECK_EQUAL(tracker_info.rdot_interval_compatibility[0][1], false);
    CHECK_EQUAL(tracker_info.rdot_interval_compatibility[1][0], false);

}


/**
 *\purpose  This tests that all rdot intervals are not compatible when both of the v_wrapping is 0
 *\req
 */

TEST(f360_configure_rdot_interval_non_compatibility, both_interval_non_compatible)
{

   /** \precond
    Set the max_otg_speed to 0
    **/
   globals.max_otg_speed = 0;
      for (int32_t index = 0; index < MAX_NUMBER_OF_SENSORS; index++)
      {

         sensors[index].variable.is_valid = true;

            sensors[index].constant.v_wrapping[0] = 0;
            sensors[index].constant.v_wrapping[1] = 0;



      }

   /** \action
    *Call the function Configure_Rdot_Interval_Compatibility to verify the interval non compatibility
    **/
   Configure_Rdot_Interval_Compatibility(sensors, globals, calibs, tracker_info, timing_info);
   /** \result
    *At 0 max_otg_speed and 4 different unique intervals, the rdot_interval_compatibility should be set to true for 4 intervals
    **/

    CHECK_EQUAL(tracker_info.rdot_interval_compatibility[0][1], false);
    CHECK_EQUAL(tracker_info.rdot_interval_compatibility[1][0], false);
    


}
/** @}*/
