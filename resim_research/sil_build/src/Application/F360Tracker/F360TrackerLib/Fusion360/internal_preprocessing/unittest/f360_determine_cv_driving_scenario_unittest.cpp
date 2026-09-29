/** \file
 * This file contains unit tests for content of f360_determine_cv_driving_scenario.cpp file
 */

#include "f360_determine_cv_driving_scenario.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** @}*/

/** \defgroup  f360_internal_preprocessing_determine_cv_driving_scenario
*  @{
*/

/** \brief
*  Sets up necessary structs for testing Determine_CV_Driving_Scenario().
**/
TEST_GROUP(f360_internal_preprocessing_determine_cv_driving_scenario)
{
   /** \setup
   * Initialize common variables used within all tests for this test group.
   **/
   bool f_highway_suspected = false;
   F360_Host_T host = {};

   TEST_TEARDOWN()
   {
      Reset_Determine_CV_Driving_Scenario_Variables();
   }
};

/**
*\purpose  Tests that Determine_CV_Driving_Scenario is not evaluating if the host type is not commercial vehicle.
*\
*\req    NA
*/
TEST(f360_internal_preprocessing_determine_cv_driving_scenario, determine_driving_scenario_wrong_type)
{
   /** \precond
   * host type is not commercial vehicle
   **/
   Reset_Determine_CV_Driving_Scenario_Variables();
   host.host_type = F360_HOST_TYPE_PASSENGER_VEHICLE;
   host.vcs_speed = 10.0F;
   /** \action
   * Call Determine_CV_Driving_Scenario() over 100 times.
   **/
   for (uint8_t i = 0U; i < 101; i++)
   {
      f_highway_suspected = Determine_CV_Driving_Scenario(host);
   }

   /** \result
   * Check that the bool flag after Determine_CV_Driving_Scenario is called sufficient number of times does not change.
   **/
  CHECK_FALSE_TEXT(f_highway_suspected, "Determine_CV_Driving_Scenario returned true even though the host is not a CV")
}

/**
*\purpose  Tests that Determine_CV_Driving_Scenario is not evaluating if the host type is not commercial vehicle but it drives steadily at significant speed.
*\
*\req    NA
*/
TEST(f360_internal_preprocessing_determine_cv_driving_scenario, determine_driving_scenario_wrong_type_enough_speed)
{
   /** \precond
   * - host type is not commercial vehicle
   **/
   Reset_Determine_CV_Driving_Scenario_Variables();
   host.host_type = F360_HOST_TYPE_PASSENGER_VEHICLE;
   host.vcs_speed = 25.0F;

   /** \action
   * Call Determine_CV_Driving_Scenario() over 100 times.
   **/
   for (uint8_t i = 0U; i < 101; i++)
   {
      f_highway_suspected = Determine_CV_Driving_Scenario(host);
   }

   /** \result
   * Check that the bool flag after Determine_CV_Driving_Scenario is called sufficient number of times does not change.
   **/
  CHECK_FALSE_TEXT(f_highway_suspected, "Determine_CV_Driving_Scenario returned true even though the host is not a CV")
}

/**
*\purpose  Tests that Determine_CV_Driving_Scenario returns false if the host type is commercial vehicle but speed threshold is not met.
*\
*\req    NA
*/
TEST(f360_internal_preprocessing_determine_cv_driving_scenario, determine_driving_scenario_host_slow)
{
   /** \precond
   * host type is commercial vehicle
   * host speed is below 9.5 mps
   **/
   Reset_Determine_CV_Driving_Scenario_Variables();
   host.host_type = F360_HOST_TYPE_COMMERCIAL_VEHICLE;
   host.vcs_speed = 9.0F;

   /** \action
   * Call Determine_CV_Driving_Scenario() over 100 times.
   **/
   for (uint8_t i = 0U; i < 101; i++)
   {
      f_highway_suspected = Determine_CV_Driving_Scenario(host);
   }

   /** \result
   * Check that the bool flag after Determine_CV_Driving_Scenario called sufficient number of times does not change.
   **/
  CHECK_FALSE_TEXT(f_highway_suspected, "Determine_CV_Driving_Scenario returned true even though the host did not have sufficient speed")
}

/**
*\purpose  Tests that Determine_CV_Driving_Scenario is returns true if the host type is commercial vehicle & speed threshold is met before 5sec time window passes.
*\
*\req    NA
*/
TEST(f360_internal_preprocessing_determine_cv_driving_scenario, determine_driving_scenario_check_time_window)
{
   /** \precond
   * host type is commercial vehicle
   * host speed is above 9.5 mps
   **/
   Reset_Determine_CV_Driving_Scenario_Variables();
   host.host_type = F360_HOST_TYPE_COMMERCIAL_VEHICLE;
   host.vcs_speed = 25.0F;

   /** \action
   * Call Determine_CV_Driving_Scenario() 99 times.
   * Check if flag is false.
   * Call funtion again.
   * Check if it changes properly.
   **/
   for (uint8_t i = 0U; i < 99; i++)
   {
      f_highway_suspected = Determine_CV_Driving_Scenario(host);
      CHECK_FALSE_TEXT(f_highway_suspected, "Determine_CV_Driving_Scenario returned true even though the index did not reach a threshold")
   }
   f_highway_suspected = Determine_CV_Driving_Scenario(host);
   CHECK_TRUE_TEXT(f_highway_suspected, "Determine_CV_Driving_Scenario returned false even though the index reached a threshold")
}

/**
*\purpose  Tests that Determine_CV_Driving_Scenario returns false if the host type is commercial vehicle & speed threshold is met but host starts to accelerate heavily before 5sec time window passes.
*\
*\req    NA
*/
TEST(f360_internal_preprocessing_determine_cv_driving_scenario, determine_driving_scenario_check_time_window_accelerate)
{
   /** \precond
   * host type is commercial vehicle
   * host speed is above 9.5 mps
   **/
   Reset_Determine_CV_Driving_Scenario_Variables();
   host.host_type = F360_HOST_TYPE_COMMERCIAL_VEHICLE;
   host.vcs_speed = 9.5F + 0.1F;

   /** \action
   * Call Determine_CV_Driving_Scenario() 100 times.
   * Check the flag is true.
   * Call Determine_CV_Driving_Scenario() 95 times.
   * Change host speed significantly in the last 5 frames
   * Check if the flag is set to false.
   **/
   for (uint8_t i = 0U; i < 100; i++)
   {
      f_highway_suspected = Determine_CV_Driving_Scenario(host);
   }
   CHECK_TRUE_TEXT(f_highway_suspected, "Determine_CV_Driving_Scenario returned false even though the conditions were met")

   for (uint8_t i = 0U; i < 95; i++)
   {
      f_highway_suspected = Determine_CV_Driving_Scenario(host);
   }
   host.vcs_speed += 2.0F;
   for (uint8_t i = 0U; i < 5; i++)
   {
      host.vcs_speed += 2.0F;
      f_highway_suspected = Determine_CV_Driving_Scenario(host);
   }
   CHECK_FALSE_TEXT(f_highway_suspected, "Determine_CV_Driving_Scenario returned true even though the host accelerated significantly before index reached a threshold")
}

/**
*\purpose  Tests that Determine_CV_Driving_Scenario returns false if the host type is a commercial vehicle that is moving fast, but slows down siginificanty before 5sec time window passes.
*\
*\req    NA
*/
TEST(f360_internal_preprocessing_determine_cv_driving_scenario, determine_driving_scenario_check_time_window_slows_down)
{
   /** \precond
   * host type is commercial vehicle
   * host speed is below 10 mps
   **/
   Reset_Determine_CV_Driving_Scenario_Variables();
   host.host_type = F360_HOST_TYPE_COMMERCIAL_VEHICLE;
   host.vcs_speed = 9.5F + 0.1F;

   /** \action
   * Call Determine_CV_Driving_Scenario() 100 times.
   * Check the flag is true.
   * Call Determine_CV_Driving_Scenario() 95 times.
   * Change host speed significantly in the last 5 frames
   * Check if the flag is set to false.
   **/
   for (uint8_t i = 0U; i < 100; i++)
   {
      f_highway_suspected = Determine_CV_Driving_Scenario(host);
   }
   CHECK_TRUE_TEXT(f_highway_suspected, "Determine_CV_Driving_Scenario returned false even though the conditions were met")

   for (uint8_t i = 0U; i < 95; i++)
   {
      f_highway_suspected = Determine_CV_Driving_Scenario(host);
   }
   host.vcs_speed -= 2.0F;
   for (uint8_t i = 0U; i < 5; i++)
   {
      f_highway_suspected = Determine_CV_Driving_Scenario(host);
   }
   CHECK_FALSE_TEXT(f_highway_suspected, "Determine_CV_Driving_Scenario returned true even though the host slowed_down significantly before index reached a threshold")
}
/** @}*/
