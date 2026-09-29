/** \file
   This file contains basic unit tests for f360_detection_double_bounce_detector_unittest.cpp
*/

#include "f360_detection_double_bounce_detector.h"
#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <cfloat>

/** \defgroup  f360_detection_double_bounce_detector
 *  @{
 **/
using namespace f360_variant_A;
/** \brief
*  Basic test to verify that detection gets flagged as double bounce when expected and vice versa
**/
TEST_GROUP(f360_detection_double_bounce_detector)
{
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   rspp_variant_A::RSPP_Detection_T dets[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Calibrations_T calibs = {};
   uint32_t num_dets = {};

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      num_dets = 2U;
      det_props[0].f_ok_to_use = true;
      dets[0].raw.range = 15.0F;
      dets[0].raw.sensor_id = 1;

      det_props[1].f_ok_to_use = true;
      dets[1].raw.range = 10.0F;
      dets[1].raw.sensor_id = 1;

      sensors[0].constant.range_limits[0] = 200.0F;
      dets[0].raw.azimuth = 0.11F;
      dets[1].raw.azimuth = 0.11F;
      dets[1].raw.range_rate = -1.0F;
      dets[0].raw.range_rate = 2.0F;
      calibs.k_db_max_nr_multi_bounces = 3;
      dets[1].processed.vcs_az = 1.6;
      dets[0].processed.vcs_az = 1.6;

      calibs.k_db_min_range_threshold = 20;
      calibs.k_db_max_range_threshold = 50;
      calibs.k_db_range_rate_threshold = 3;
   }
};


/**
*\purpose Test Double_Bounce_Detection_Countermeasure with 2 detection with double bounce in the cone of silence
*\req
*/
TEST(f360_detection_double_bounce_detector, Test_with_2_detection_with_double_bounce)
{
   /** \precond
    * Set up the 2 detection to be bounce in the cone of silence.
    **/

    /** \action
     * Call Double_Bounce_Detection_Countermeasure.
     **/
   Double_Bounce_Detection_Countermeasure(det_props, dets, num_dets, sensors, calibs);
   /** \result
    * Check that secondary detection is double bounce and other detections is not double bounce
    **/
   CHECK_TRUE(det_props[0].f_double_bounce);
   for (int32_t i = 1; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      CHECK_FALSE(det_props[i].f_double_bounce);
   }
}

/**
*\purpose Test Double_Bounce_Detection_Countermeasure with 2 detection outside gate for being in cone of silence
*\req
*/
TEST(f360_detection_double_bounce_detector, Test_with_2_detection_outside_gate_4)
{
   /** \precond
   * Set up 2 detections outside the gate: vcs azimuth for the secondary detection is out of border.
   **/
   dets[0].processed.vcs_az = 2;

   /** \action
   * Call Double_Bounce_Detection_Countermeasure.
   **/
   Double_Bounce_Detection_Countermeasure(det_props, dets, num_dets, sensors, calibs);

   /** \result
   * Check that all detections are not flagged as double bounce.
   **/
   for (const auto& det_prop : det_props)
   {
      CHECK_FALSE(det_prop.f_double_bounce);
   }
}

/**
*\purpose Test Double_Bounce_Detection_Countermeasure with 2 detection outside gate for being in cone of silence
*\req
*/
TEST(f360_detection_double_bounce_detector, Test_with_2_detection_outside_gate_3)
{
   /** \precond
    * Set up 2 detections outside the gate: The primary detection is less than range_rate_threshold.
    **/
   calibs.k_db_range_rate_threshold = 2;

   /** \action
    * Call Double_Bounce_Detection_Countermeasure.
    **/
   Double_Bounce_Detection_Countermeasure(det_props, dets, num_dets, sensors, calibs);

   /** \result
    * Check that all detections are not flagged as double bounce.
    **/
   for (const auto& det_prop : det_props)
   {
      CHECK_FALSE(det_prop.f_double_bounce);
   }
}

/**
*\purpose Test Double_Bounce_Detection_Countermeasure with 2 detection outside gate for being in cone of silence
*\req
*/
TEST(f360_detection_double_bounce_detector, Test_with_2_detection_outside_gate_2)
{
   /** \precond
    * Set up 2 detections outside the gate: vcs azimuth for the primary detection is out of border.
    **/
   calibs.k_db_range_rate_threshold = 0;

   /** \action
    * Call Double_Bounce_Detection_Countermeasure.
    **/
   Double_Bounce_Detection_Countermeasure(det_props, dets, num_dets, sensors, calibs);

   /** \result
    * Check that all detections are not flagged as double bounce.
    **/
   for (const auto& det_prop : det_props)
   {
      CHECK_FALSE(det_prop.f_double_bounce);
   }
}

/**
*\purpose Test Double_Bounce_Detection_Countermeasure with 2 detection outside gate for being in cone of silence
*\req
*/
TEST(f360_detection_double_bounce_detector, Test_with_2_detection_outside_gate_1)
{
   /** \precond
    * Set up 2 detections outside the gate: vcs azimuth for the primary detection is out of border.
    **/
   dets[1].processed.vcs_az = 3;

   /** \action
    * Call Double_Bounce_Detection_Countermeasure.
    **/
   Double_Bounce_Detection_Countermeasure(det_props, dets, num_dets, sensors, calibs);

   /** \result
    * Check that all detections are not flagged as double bounce.
    **/
   for (const auto& det_prop : det_props)
   {
      CHECK_FALSE(det_prop.f_double_bounce);
   }
}

/**
*\purpose Test Double_Bounce_Detection_Countermeasure with 2 detection inside Gate
*\req
*/
TEST(f360_detection_double_bounce_detector, Test_with_2_detection_inside_gate)
{
   /** \precond
    * Set up 2 detections inside the gate by changing the calibration values.
    **/
   sensors[0].constant.v_wrapping[0] = 59.0F;
   sensors[0].constant.min_aliaised_range_rate[0] = -45.0F;

   /** \action
    * Call Double_Bounce_Detection_Countermeasure.
    **/
   Double_Bounce_Detection_Countermeasure(det_props, dets, num_dets, sensors, calibs);

   /** \result
    * Check that the secondary detection is flagged as double bounce and other detections are not flagged as double bounce.
    **/
   CHECK_TRUE(det_props[0].f_double_bounce);
   for (int32_t i = 1; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      CHECK_FALSE(det_props[i].f_double_bounce);
   }
}
/**
*\purpose Test Double_Bounce_Detection_Countermeasure with 2 detection not in limit
*\req
*/
TEST(f360_detection_double_bounce_detector, Test_with_2_detection_not_in_limit_3)
{
   /** \precond
    * Set up 2 detections not in the limit: max number of multi-bounce is less than the number of multi-bounce.
    **/
   calibs.k_db_max_nr_multi_bounces = 0;

   /** \action
    * Call Double_Bounce_Detection_Countermeasure.
    **/
   Double_Bounce_Detection_Countermeasure(det_props, dets, num_dets, sensors, calibs);

   /** \result
    * Check that all detections are not flagged as double bounce.
    **/
   for (auto det_prop : det_props)
   {
      CHECK_FALSE(det_prop.f_double_bounce);
   }
}
/**
*\purpose Test Double_Bounce_Detection_Countermeasure with 2 detection not in limit
*\req
*/
TEST(f360_detection_double_bounce_detector, Test_with_2_detection_not_in_limit_2)
{
   /** \precond
    * Set up 2 detections not in the limit: number of multi-bounce is less than 1 (The object range is higher than double the limit).
    **/
   calibs.k_db_max_range = 150;
   dets[0].raw.range = 110.0F;
   dets[1].raw.range = 110.0F;

   /** \action
    * Call Double_Bounce_Detection_Countermeasure.
    **/
   Double_Bounce_Detection_Countermeasure(det_props, dets, num_dets, sensors, calibs);

   /** \result
    * Check that all detections are not flagged as double bounce.
    **/
   for (auto det_prop : det_props)
   {
      CHECK_FALSE(det_prop.f_double_bounce);
   }
}
/**
*\purpose Test Double_Bounce_Detection_Countermeasure with 2 detection not in limit
*\req
*/
TEST(f360_detection_double_bounce_detector, Test_with_2_detection_not_in_limit_1)
{
   /** \precond
    * Set up 2 detections not in the limit: range is 0.
    **/
   dets[0].raw.range = 0.0F;
   dets[1].raw.range = 0.0F;

   /** \action
    * Call Double_Bounce_Detection_Countermeasure.
    **/
   Double_Bounce_Detection_Countermeasure(det_props, dets, num_dets, sensors, calibs);

   /** \result
    * Check that all detections are not flagged as double bounce.
    **/
   for (auto det_prop : det_props)
   {
      CHECK_FALSE(det_prop.f_double_bounce);
   }
}

/**
*\purpose Test Double_Bounce_Detection_Countermeasure with only one detection per sensor
*\req
*/
TEST(f360_detection_double_bounce_detector, Test_one_detection_per_sensor)
{
   /** \precond
    * Set up 4 detections with different sensor IDs.
    **/
   num_dets = 5U;
   dets[0].raw.sensor_id = 1;
   dets[1].raw.sensor_id = 2;

   det_props[2].f_ok_to_use = true;
   dets[2].raw.range = 45.0F;
   dets[2].raw.sensor_id = 3;

   det_props[3].f_ok_to_use = true;
   dets[3].raw.range = 10.0F;
   dets[3].raw.sensor_id = 4;
   dets[3].raw.f_bistatic = 1;

   dets[4].raw.sensor_id = 5;

   /** \action
    * Call Double_Bounce_Detection_Countermeasure.
    **/
   Double_Bounce_Detection_Countermeasure(det_props, dets, num_dets, sensors, calibs);

   /** \result
    * Check that all detections are not flagged as double bounce.
    **/
   for (auto det_prop : det_props)
   {
      CHECK_FALSE(det_prop.f_double_bounce);
   }
}


/** @}*/