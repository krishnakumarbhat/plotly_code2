/** \file
   This file contains basic unit tests for algorithm that flags detections as double bounce if they fit the criteria.
*/

#include "f360_detection_double_bounce_detector.h"
#include "f360_math.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

/** \defgroup  f360_detection_double_bounce_detector_qualtest
 *  @{
 **/
using namespace f360_variant_A;
/** \brief
*  Basic test to verify that detection gets flagged as double bounce when expected and vice versa
**/
TEST_GROUP(f360_detection_double_bounce_detector_qualtest)
{
   // Define common data for all tests
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   rspp_variant_A::RSPP_Detection_T dets[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Calibrations_T calibs = {};
   uint32_t num_dets = {};

   /** \setup
   * Setup common data for all tests related to this test group
   **/
   TEST_SETUP()
   {
      // Sensor setup
      sensors[0].variable.look_id = F360_DET_LOOK_ID_0; // Use only LR, LL looktype for test
      sensors[0].constant.range_limits[0] = 200.0F;
      sensors[0].constant.v_wrapping[0] = 59.0F;
      sensors[0].constant.min_aliaised_range_rate[0] = -45.0F;
      sensors[0].constant.polarity = 1;

      // Detections setup
      // Setup 2 detections in a double bounce pattern. Modify these values to define test cases.
      num_dets = 2U;
      dets[0].raw.range = 10.0F;
      dets[0].raw.range_rate = 2.0F;
      dets[0].raw.azimuth = 0.1F;
      dets[0].raw.sensor_id = 1; // Use first sensor for test, id =1, idx = 0
      det_props[0].f_ok_to_use = true;
      det_props[0].f_double_bounce = false;

      dets[1].raw.range = 20.0F;
      dets[1].raw.range_rate = 4.0F;
      dets[1].raw.azimuth = 0.11F;
      dets[1].raw.sensor_id = 1; // Use first sensor for test, id =1, idx = 0
      det_props[1].f_ok_to_use = true;
      det_props[1].f_double_bounce = false;

      // Calibration setup
      Initialize_Tracker_Calibrations(calibs);
   }

   /** \teardown
   * Nothing to teardown in this test group
   **/
   TEST_TEARDOWN()
   {
   }

};

/**
*\purpose  Test that detection is flagged as double bounce
*\req    CPR-3859
*/
TEST(f360_detection_double_bounce_detector_qualtest, DoubleBounce_true)
{

   Double_Bounce_Detection_Countermeasure(det_props, dets, num_dets, sensors, calibs);

   /** \result
   * We only expect the detection with higher range to be flagged as double bounce
   **/
   CHECK_FALSE(det_props[0].f_double_bounce);
   CHECK_TRUE(det_props[1].f_double_bounce);
}

/**
*\purpose  Test that detection is NOT flagged as double bounce since range condition is not met
*\req    CPR-3859
*/
TEST(f360_detection_double_bounce_detector_qualtest, DoubleBounce_false_RangeMismatch)
{

   // Change range of secondary detection to NOT fit double bounce
   dets[1].raw.range = 15.0F;

   Double_Bounce_Detection_Countermeasure(det_props, dets, num_dets, sensors, calibs);

   /** \result
   * We expect no detection to be flagged as double bounce
   **/
   CHECK_FALSE(det_props[0].f_double_bounce);
   CHECK_FALSE(det_props[1].f_double_bounce);
}

/**
*\purpose  Test that detection is NOT flagged as double bounce since azimuth condition is not met
*\req    CPR-3859
*/
TEST(f360_detection_double_bounce_detector_qualtest, DoubleBounce_false_AzimuthMismatch)
{

   // Change azimuth of secondary detection to NOT fit double bounce
   dets[1].raw.azimuth = 0.4F;

   // Since azimuth does not fit double bounce characteristics we will break the loop before reaching range rate comparison
   Double_Bounce_Detection_Countermeasure(det_props, dets, num_dets, sensors, calibs);

   /** \result
   * We expect no detection to be flagged as double bounce
   **/
   CHECK_FALSE(det_props[0].f_double_bounce);
   CHECK_FALSE(det_props[1].f_double_bounce);
}

/**
*\purpose  Test that detection is NOT flagged as double bounce since range rate condition is not met
*\req    CPR-3859
*/
TEST(f360_detection_double_bounce_detector_qualtest, DoubleBounce_false_RdotMismatch)
{
   // Change range rate of secondary detection to NOT fit double bounce
   dets[1].raw.range_rate = -2.0F;

   Double_Bounce_Detection_Countermeasure(det_props, dets, num_dets, sensors, calibs);

   /** \result
   * We expect no detection to be flagged as double bounce
   **/
   CHECK_FALSE(det_props[0].f_double_bounce);
   CHECK_FALSE(det_props[1].f_double_bounce);
}
/** @}*/


/** \defgroup  system_test_detection_double_bounce_detector
 *  @{
 **/

/** \brief
*  Basic test to verify that detection gets flagged as double bounce when expected and vice versa
**/
TEST_GROUP(system_test_detection_double_bounce_detector)
{
   // Define common data for all tests
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   rspp_variant_A::RSPP_Detection_T dets[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Calibrations_T calibs = {};
   uint32_t num_dets = {};


   /** \setup
   * Setup common data for all tests related to this test group
   *
   * Setup 4 detections in a double bounce pattern. Modify these values and maybe add more
   * detections to define test cases.
   * The following detections are setup:
      det[x]       |.range |.range_rate |.azimuth |.sensor_id
      -------------|------ |----------- |-------- |----------
      det[0]       |10.0   |2.0         |0.1      |1
      det[1]       |20.0   |4.0         |0.11     |1
      det[2]       |30.0   |6.0         |0.11     |1
      det[3]       |40.0   |8.0         |0.11     |1
   *
   **/
   TEST_SETUP()
   {
      // Setup calibrations
      Initialize_Tracker_Calibrations(calibs);

      // Sensor setup
      sensors[0].variable.look_id = F360_DET_LOOK_ID_0; // Use only LR, LL looktype for test
      sensors[0].constant.range_limits[0] = 200.0F;
      sensors[0].constant.v_wrapping[0] = 59.0F;
      sensors[0].constant.min_aliaised_range_rate[0] = -45.0F;

      //Detections setup
      // Setup 4 detections in a double bounce pattern. Modify these values and maybe add more
      // detections to define test cases.

      num_dets = 4U;
      dets[0].raw.range = 10.0F;
      dets[0].raw.range_rate = 2.0F;
      dets[0].raw.azimuth = 0.1F;
      dets[0].raw.sensor_id = 1U; // Use first sensor for test, id =1, idx = 0
      det_props[0].f_ok_to_use = true;

      dets[1].raw.range = 20.0F;
      dets[1].raw.range_rate = 4.0F;
      dets[1].raw.azimuth = 0.11F;
      dets[1].raw.sensor_id = 1U;
      det_props[1].f_ok_to_use = true;

      dets[2].raw.range = 30.0F;
      dets[2].raw.range_rate = 6.0F;
      dets[2].raw.azimuth = 0.11F;
      dets[2].raw.sensor_id = 1U;
      det_props[2].f_ok_to_use = true;

      dets[3].raw.range = 39.9F;
      dets[3].raw.range_rate = 8.0F;
      dets[3].raw.azimuth = 0.11F;
      dets[3].raw.sensor_id = 1U;
      det_props[3].f_ok_to_use = true;
   }

   /** \teardown
   * Nothing to teardown in this test group
   **/
   TEST_TEARDOWN()
   {
   }

};

/**
*\purpose  Simple test using two detections only, one is double bounce of the other one
*\req    NA
*/
TEST(system_test_detection_double_bounce_detector, Double_Bounce_True)
{
   /** \step{1}
   * Testing 2 detections
   **/

   /** \precond
   * Testing only 2 detections of the set
   **/
   num_dets = 2U;

   /** \action
   * Call the function
   **/
   Double_Bounce_Detection_Countermeasure(det_props, dets, num_dets, sensors, calibs);

   /** \result
   * We only expect the detection with higher range to be flagged as double bounce
   **/
   CHECK_FALSE(det_props[0].f_double_bounce);
   CHECK_TRUE(det_props[1].f_double_bounce);
}


/**
*\purpose  Checking multiple bounces, one reference detection have yielded 3 double bounces
*\req    NA
*/
TEST(system_test_detection_double_bounce_detector, Multiple_Double_Bounce_True)
{
   /** \step{1}
   * Testing multiple bounces
   **/

   /** \precond
   * Testing 4 detections of the set
   **/
   num_dets = 4U;

   /** \action
   * Call the function
   **/
   Double_Bounce_Detection_Countermeasure(det_props, dets, num_dets, sensors, calibs);

   /** \result
   * We only expect the detection with higher range to be flagged as double bounce
   **/
   CHECK_FALSE(det_props[0].f_double_bounce);
   CHECK_TRUE(det_props[1].f_double_bounce);
   CHECK_TRUE(det_props[2].f_double_bounce);
   CHECK_TRUE(det_props[3].f_double_bounce);
}

/**
*\purpose  Test Arbitrary Scenario 1
*
*\req    NA
*/
TEST(system_test_detection_double_bounce_detector, Arbitrary_Double_Bounce_Scenario)
{
   /** \step{1}
   * Testing arbitrary scenario
   **/

   /** \precond
   * Creating new set of 7 detections:
   *
    -------------X6--------------------
    -----------------------------------
    ---------O3----X4---------x5-------
    -----------------------------------
    -------------O0--O1---O2-----------
    -----------------------------------
    ------------------S0---------------

    |Character | Type                  |
    | :------- | :-------------------- |
    |X         | Multibounce detection |
    |O         | Real detection        |
    |S         | Sensor                |



   **/

   num_dets = 7U;

   dets[0].raw.range = 5.0F;
   dets[0].raw.range_rate = 3.0F;
   dets[0].raw.azimuth = F360_DEG2RAD(-50.0F);
   dets[0].raw.sensor_id = 1U;
   det_props[0].f_ok_to_use = true;

   dets[1].raw.range = 4.0F;
   dets[1].raw.range_rate = 2.5F;
   dets[1].raw.azimuth = F360_DEG2RAD(-40.0F);
   dets[1].raw.sensor_id = 1U;
   det_props[1].f_ok_to_use = true;

   dets[2].raw.range = 4.0F;
   dets[2].raw.range_rate = -2.5F;
   dets[2].raw.azimuth = F360_DEG2RAD(40.0F);
   dets[2].raw.sensor_id = 1U;
   det_props[2].f_ok_to_use = true;

   dets[3].raw.range = 10.0F;
   dets[3].raw.range_rate = -5.0F;
   dets[3].raw.azimuth = F360_DEG2RAD(-52.0F);
   dets[3].raw.sensor_id = 1U;
   det_props[3].f_ok_to_use = true;

   dets[4].raw.range = 8.1F;
   dets[4].raw.range_rate = 5.0F;
   dets[4].raw.azimuth = F360_DEG2RAD(-42.0F);
   dets[4].raw.sensor_id = 1U;
   det_props[4].f_ok_to_use = true;

   dets[5].raw.range = 7.9F;
   dets[5].raw.range_rate = -5.0F;
   dets[5].raw.azimuth = F360_DEG2RAD(44.0F);
   dets[5].raw.sensor_id = 1U;
   det_props[5].f_ok_to_use = true;

   dets[6].raw.range = 12.0F;
   dets[6].raw.range_rate = 7.5F;
   dets[6].raw.azimuth = F360_DEG2RAD(-40.0F);
   dets[6].raw.sensor_id = 1U;
   det_props[6].f_ok_to_use = true;

   /** \action
   * Call the function
   **/
   Double_Bounce_Detection_Countermeasure(det_props, dets, num_dets, sensors, calibs);

   /** \result
   * We only expect the detection with higher range to be flagged as double bounce
   **/
   CHECK_FALSE(det_props[0].f_double_bounce);
   CHECK_FALSE(det_props[1].f_double_bounce);
   CHECK_FALSE(det_props[2].f_double_bounce);
   CHECK_FALSE(det_props[3].f_double_bounce);
   CHECK_TRUE(det_props[4].f_double_bounce);
   CHECK_TRUE(det_props[5].f_double_bounce);
   CHECK_TRUE(det_props[6].f_double_bounce);
}
/** @}*/
