/** \file
 * This file contains unit tests for content of f360_detection_multipath_filter.cpp file
 */

#include "f360_detection_multipath_filter.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_detection_multipath_filter
 *  @{
 */

/** \brief
 * Test that the functionality in Filter_Out_Multipath_Detections() works as intended.
 */
TEST_GROUP(f360_detection_multipath_filter)
{
   F360_Host_T host = {};
   F360_Radar_Sensor_T sens = {};
   rspp_variant_A::RSPP_Detection_T dets[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Calibrations_T f360_calibs = {};
   uint32_t first_detection_list_idx;
   /** \setup
    * Set up a correct calibration structure and specify fields so that the filter is activated and
    * modifies the detection property f_ok_to_use.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(f360_calibs);
      sens.variable.number_of_valid_detections = 100U;

      host.speed = 1.01F;

      first_detection_list_idx = 0U;

      for (uint32_t det_idx = 0; det_idx < sens.variable.number_of_valid_detections; det_idx++)
      {
         if (det_idx < 34U)
         {
            dets[det_idx].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_LOW;
            det_props[det_idx].f_ok_to_use = true;
         }
         else
         {
            dets[det_idx].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_HIGH;
            det_props[det_idx].f_ok_to_use = true;
         }
      }

      sens.constant.sensor_type = F360_SENSOR_TYPE_SRR5_RADAR;
   }
};

/** \purpose
 * Test that function Filter_Out_Multipath_Detections() works as intended and doesn't mark the detections as not ok to use
 * when the first_detection_list_idx in sensor props is invalid for the given sensor.
 * \req
 * NA
 */
TEST(f360_detection_multipath_filter, Detection_Multipath_Filter__Filter_First_Det_Idx_Invalid)
{
   /** \precond
   * Set first detection index in sensor props to F360_INVALID_ID
   */
   first_detection_list_idx = F360_INVALID_ID;

   /** \action
    * Call Filter_Out_Multipath_Detections()
    */
   Filter_Out_Multipath_Detections(host, sens, f360_calibs, first_detection_list_idx, dets, det_props);

   /** \result
    * Check that the detections are not marked as not ok to use.
    */
   bool all_dets_ok_to_use = true;
   for (uint32_t det_idx = 0; det_idx < sens.variable.number_of_valid_detections; det_idx++)
   {
      if (!det_props[det_idx].f_ok_to_use)
      {
         all_dets_ok_to_use = false;
         break;
      }
   }
   CHECK_TRUE_TEXT(all_dets_ok_to_use, "Detection property f_ok_to_use was not modified as expected for all detections.")
}

/** \purpose
 * Test that function Filter_Out_Multipath_Detections() works as intended and marks the detections as not ok to use
 * when the fraction is higher than the threshold for an SRR5 sensor.
 * \req
 * NA
 */
TEST(f360_detection_multipath_filter, Detection_Multipath_Filter__Filter_Num_Bad_Dets_Above_Thresh)
{
   /** \action
    * Call Filter_Out_Multipath_Detections()
    */
   Filter_Out_Multipath_Detections(host, sens, f360_calibs, first_detection_list_idx, dets, det_props);

   /** \result
    * Check that the detections with confid_az == rspp_variant_A::RSPP_CONF_AZIMUTH_LOW are marked as not ok to use and that those
    * with higher azimuth confidence are still ok to use.
    */
   bool all_dets_low_conf_not_ok_to_use = true;
   bool all_dets_high_conf_ok_to_use = true;
   for (uint32_t det_idx = 0; det_idx < sens.variable.number_of_valid_detections; det_idx++)
   {
      if (dets[det_idx].raw.confid_azimuth == rspp_variant_A::RSPP_CONF_AZIMUTH_LOW)
      {
         if (det_props[det_idx].f_ok_to_use == true)
         {
            all_dets_low_conf_not_ok_to_use = false;
            break;
         }
      }
      else
      {
         if (det_props[det_idx].f_ok_to_use == false)
         {
            all_dets_high_conf_ok_to_use = false;
            break;
         }
      }
   }
   CHECK_TRUE_TEXT(all_dets_low_conf_not_ok_to_use, "A detection with low azimuth confidence was flagged as f_ok_to_use.")
   CHECK_TRUE_TEXT(all_dets_high_conf_ok_to_use, "A detection with high azimuth confidence was flagged as not f_ok_to_use.")
}

/** \purpose
 * Test that function Filter_Out_Multipath_Detections() works as intended and doesn't mark the detections as not ok to use
 * when the fraction is below than the threshold for an SRR5 sensor.
 * \req
 * NA
 */
TEST(f360_detection_multipath_filter, Detection_Multipath_Filter__Filter_Num_Bad_Dets_Below_Thresh)
{
   /** \precond
   * Change azimuth confidence of one detection from rspp_variant_A::RSPP_CONF_AZIMUTH_LOW to rspp_variant_A::RSPP_CONF_AZIMUTH_HIGH such that the fraction of dets with bad azimuth is slightly below threshold.
   */
   dets[0].raw.confid_azimuth = rspp_variant_A::RSPP_CONF_AZIMUTH_HIGH;

   /** \action
    * Call Filter_Out_Multipath_Detections()
    */
   Filter_Out_Multipath_Detections(host, sens, f360_calibs, first_detection_list_idx, dets, det_props);

   /** \result
    * Check that the detections are not marked as not ok to use.
    */
   bool all_dets_ok_to_use = true;
   for (uint32_t det_idx = 0; det_idx < sens.variable.number_of_valid_detections; det_idx++)
   {
      if (!det_props[det_idx].f_ok_to_use)
      {
         all_dets_ok_to_use = false;
         break;
      }
   }
   CHECK_TRUE_TEXT(all_dets_ok_to_use, "Detection property f_ok_to_use was not modified as expected for all detections.")
}

/** \purpose
 * Test that function Filter_Out_Multipath_Detections() works as intended when there are not enough valid detections present for the sensor.
 * \req
 * NA
 */
TEST(f360_detection_multipath_filter, Detection_Multipath_Filter__Num_Valid_Dets_Below_Threshold)
{
   /** \precond
   * Set number of valid detections for the sensor to 50.
   */
   sens.variable.number_of_valid_detections = 50U;

   /** \action
    * Call Filter_Out_Multipath_Detections()
    */
   Filter_Out_Multipath_Detections(host, sens, f360_calibs, first_detection_list_idx, dets, det_props);

   /** \result
    * Check that the detection is still marked as ok to use.
    */
   CHECK_TRUE_TEXT(det_props[0].f_ok_to_use, "Detection property f_ok_to_use was not modified as expected.")
}

/** \purpose
 * Test that function Filter_Out_Multipath_Detections() works as intended when host is driving too slow to activate the filter.
 * \req
 * NA
 */
TEST(f360_detection_multipath_filter, Detection_Multipath_Filter__Host_Slow)
{
   /** \precond
   * Set host speed to something below calibration value 1.0 m/s.
   */
   host.speed = 0.99F;

   /** \action
    * Call Filter_Out_Multipath_Detections()
    */
   Filter_Out_Multipath_Detections(host, sens, f360_calibs, first_detection_list_idx, dets, det_props);

   /** \result
    * Check that the detection is still marked as ok to use.
    */
   CHECK_TRUE_TEXT(det_props[0].f_ok_to_use, "Detection property f_ok_to_use was not modified as expected.")
}

/** \purpose
 * Test that function Filter_Out_Multipath_Detections() works as intended and marks the detection as ok to use when the sensor is of a different type than SRR5.
 * \req
 * NA
 */
TEST(f360_detection_multipath_filter, Detection_Multipath_Filter__Not_SRR5)
{
   /** \precond
   * Define array of possible sensor types (where SRR5 is excluded).
   */
   F360_Sensor_Type_T sensor_types[12] = {
      F360_SENSOR_TYPE_UNKNOWN,
      F360_SENSOR_TYPE_SRR2_RADAR,
      F360_SENSOR_TYPE_SRR4_RADAR,
      F360_SENSOR_TYPE_SRR4_MM_RADAR,
      F360_SENSOR_TYPE_MRR360_RADAR,
      F360_SENSOR_TYPE_ESR_RADAR,
      F360_SENSOR_TYPE_MRR1_RADAR,
      F360_SENSOR_TYPE_MRR2_RADAR,
      F360_SENSOR_TYPE_MRR3_RADAR,
      F360_SENSOR_TYPE_LIDAR,
      F360_SENSOR_TYPE_VISION,
      F360_SENSOR_TYPE_VEHICLE};

   for (uint32_t i = 0U; i < 12U; i++)
   {
      /** \precond
      * Set sensor type.
      */
      sens.constant.sensor_type = sensor_types[i];

      /** \action
       * Call Filter_Out_Multipath_Detections()
       */
      Filter_Out_Multipath_Detections(host, sens, f360_calibs, first_detection_list_idx, dets, det_props);

      /** \result
       * Check that the detection is still marked as ok to use.
       */
      CHECK_TRUE_TEXT(det_props[0].f_ok_to_use, "Detection property f_ok_to_use was not modified as expected.")
   }
}
/** @}*/
