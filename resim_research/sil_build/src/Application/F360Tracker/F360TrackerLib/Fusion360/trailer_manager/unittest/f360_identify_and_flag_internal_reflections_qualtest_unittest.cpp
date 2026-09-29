/** \file
 * This file contains qualification tests for content of f360_identify_and_flag_internal_reflections.cpp file
 */

#include "f360_identify_and_flag_internal_reflections.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \brief
 * This test group tests the functionality of the main function inside f360_identify_and_flag_internal_reflections.cpp
 * through a set of artificial scenarios by only modifying input rather than intermittent variables and/or structures.
 */
TEST_GROUP(f360_identify_and_flag_internal_reflections__scenario_based_tests)
{

   /** \setup
    * Initialize and reset all variables used in function call.
    * In TEST_SETUP(), the following is done;
    * - Set all relevant sensors for all sensors to some reasonable value
    * - Set the OTG speed of all sensors to be slightly higher than internal_reflections_min_host_vel
    * - Reset all entries in all buffer slots for all sensors to 0.
    * - Reset all entries in raw_det_list to 0
    * - Reset f_ok_to_use to true for for all dets in dets_props
    * Setup helper function Copy_Relevant_Detection_Properties() that copies relevant detection properties from a single
    * detection to another detection.
    */

   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   rspp_variant_A::RSPP_Detection_List_T raw_det_list = {};
   F360_Radar_Sensor_Props_T sensor_props[MAX_NUMBER_OF_SENSORS] = {};
   F360_Detection_Props_T dets_props[MAX_NUMBER_OF_DETECTIONS] = {};

   bool f_mark_internal_reflections_enabled = true;
   const rspp_variant_A::RSPP_Detection_T empty_det = {};
   rspp_variant_A::RSPP_Detection_T test_det = {};

   TEST_SETUP()
   {

      for (int i_sens = 0; i_sens < MAX_NUMBER_OF_SENSORS; i_sens++)
      {
         sensors[i_sens].variable.is_valid = true;

         sensors[i_sens].constant.internal_reflections.f_enable = true;

         sensors[i_sens].constant.internal_reflections.min_host_vel = 0.5F;
         sensors[i_sens].constant.internal_reflections.age_threshold = 200U;
         sensors[i_sens].constant.internal_reflections.occurrence_lowerlimit = 0.09F;
         sensors[i_sens].constant.internal_reflections.occurrence_threshold = 0.1F;

         sensors[i_sens].constant.internal_reflections.rcs_tolerance = 0.1F;
         sensors[i_sens].constant.internal_reflections.azimuth_tolerance = 0.1F;
         sensors[i_sens].constant.internal_reflections.range_tolerance = 0.1F;

         sensors[i_sens].constant.internal_reflections.max_abs_range_rate = 0.1F;
         sensors[i_sens].constant.internal_reflections.rcs_max = -15.0F;
         sensors[i_sens].constant.internal_reflections.range_max = 5.0F;

         sensors[i_sens].variable.vcs_velocity.longitudinal = 1.1F * sensors[i_sens].constant.internal_reflections.min_host_vel;

         for (int i_det = 0; i_det < INTERNAL_REFLECTIONS_BUFFER_SIZE; i_det++)
         {
            sensor_props[i_sens].internal_reflections_buffer[i_det] = Internal_Reflection_Buffer_Slot{};
         }
      }

      raw_det_list = {};

      for (int i_det = 0; i_det < MAX_NUMBER_OF_DETECTIONS; i_det++)
      {
         dets_props[i_det].f_ok_to_use = true;
      }
   }

   void Copy_Relevant_Detection_Properties(
         const rspp_variant_A::RSPP_Detection_T &input_det,
         rspp_variant_A::RSPP_Detection_T &target_det)
   {
      target_det.raw.range = input_det.raw.range;
      target_det.raw.range_rate = input_det.raw.range_rate;
      target_det.raw.rcs = input_det.raw.rcs;
      target_det.raw.azimuth = input_det.raw.azimuth;
   };

};


/** \purpose
 * Verify that an empty buffer fills up and starts flagging new detections as expected for
 * an identical detection that does not always reoccur, but often enough to eventually be
 * flagged as an internal reflection.
 * \req
 *  CPR-3863
 */
TEST(f360_identify_and_flag_internal_reflections__scenario_based_tests, flag_recurring_detections)
{
   /** \precond
    * - Set all relevant sensors for all sensors to some reasonable value
    * - Set the OTG speed of all sensors to be slightly higher than internal_reflections.min_host_vel
    * - Reset all entries in all buffer slots for all sensors to 0.
    * - Reset all entries in raw_det_list to 0
    * - Reset f_ok_to_use to true for for all dets in dets_props
    * - Set sensors[0].internal_reflections.age_threshold to 200
    * - Setup 1 detection that is relevant to buffer
    * - Set all other detections in raw_det_list to be irrelevant for buffer (i.e. by having properties such as
    *   rcs > internal_reflections.rcs_max).
    * - Feed the relevant detection into Identify_And_Remove_Internal_Reflections() not always but often enough to get
    *   an occurence_rate higher than internal_reflections_occurrence_threshold.
    * - Change the detection slot index for the relevant detection between iterations
    * - Call the function in this manner 200 times such that the buffer slot containing the relevant detection
    *   has an age and occurence_count such that it will be flagged as internal reflection.
    * - Setup the relevant detection in an arbitrary detection slot and store its slot index
    */
   sensors[0].constant.internal_reflections.age_threshold = 200;

   raw_det_list.number_of_valid_detections = 50;
   for (unsigned int i_det = 0; i_det < raw_det_list.number_of_valid_detections; i_det++)
   {
      raw_det_list.detections[i_det].raw.sensor_id = 1;
   }

   test_det.raw.range = 2.0F;
   test_det.raw.rcs = -15.0F;
   test_det.raw.azimuth = 0.0F;
   test_det.raw.range_rate = 0.0F;

   int det_reoccurrence = 10;
   int detidx = 0;

   for (int iteration = 0; iteration < sensors[0].constant.internal_reflections.age_threshold; iteration++)
   {

     // The detection will only be fed to the function every det_reoccurrence'th iteration
     if ((iteration % det_reoccurrence) == 0)
     {
        // To switch around the detection slot used to feed the detection
        detidx = iteration % raw_det_list.number_of_valid_detections;

        Copy_Relevant_Detection_Properties(test_det, raw_det_list.detections[detidx]);
     }

     const bool bike_carrier_attached = Is_Bike_Carrier_Attached(sensors, sensor_props);

     for (unsigned int det_idx = 0; det_idx < raw_det_list.number_of_valid_detections; det_idx++)
     {
        const rspp_variant_A::RSPP_Detection_T &current_detection = raw_det_list.detections[det_idx];
        F360_Detection_Props_T &current_detection_prop = dets_props[det_idx];
        const int32_t current_sensor_id = current_detection.raw.sensor_id;
        F360_Radar_Sensor_Props_T &current_sensor_props = sensor_props[current_sensor_id - 1];
        const F360_Radar_Sensor_T& current_sensor = sensors[current_sensor_id - 1];
        if (bike_carrier_attached) Identify_And_Mark_Internal_Reflections(current_detection, current_sensor, current_sensor_props, current_detection_prop);
     }

     Copy_Relevant_Detection_Properties(empty_det, raw_det_list.detections[detidx]);
   }

   unsigned int det_idx_to_check = 39;
   Copy_Relevant_Detection_Properties(test_det, raw_det_list.detections[det_idx_to_check]);

   /** \action
    * Call function Identify_And_Flag_Internal_Reflections().
    */
   const bool bike_carrier_attached = Is_Bike_Carrier_Attached(sensors, sensor_props);

   for (unsigned int det_idx = 0; det_idx < raw_det_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_detection = raw_det_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = dets_props[det_idx];
      const int32_t current_sensor_id = current_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      F360_Radar_Sensor_Props_T &current_sensor_props = sensor_props[current_sensor_id - 1];
      if (bike_carrier_attached) Identify_And_Mark_Internal_Reflections(current_detection, current_sensor, current_sensor_props, current_detection_prop);
   }

   /** \result
    * Verify that the relevant detection was flagged as not ok to use
    * Verify that all other detections is ok to use
    */
   for (unsigned int det_idx = 0; det_idx < raw_det_list.number_of_valid_detections; det_idx++)
   {
      if (det_idx == det_idx_to_check)
      {
         CHECK_FALSE(dets_props[det_idx].f_ok_to_use);
      }
      else
      {
         CHECK_TRUE(dets_props[det_idx].f_ok_to_use);
      }
   }
}


/** \purpose
 * Verify that a buffer resets for entries with a low occurrence rate by feeding the function a detection that fills
 * a buffer slot enough times such that it will flag similar detections as internal reflections.
 * Then call the function without any new detections such that the buffer entry's occurrence rate will drop and
 * reset the buffer entry. Then ultimately feed the same detection again and verify that it does not get flagged.
 * \req
 *  CPR-3863
 */
TEST(f360_identify_and_flag_internal_reflections__scenario_based_tests, reset_buffer_entries_with_low_occurance)
{
   /** \precond
    * - Set all relevant sensors for all sensors to some reasonable value
    * - Set the OTG speed of all sensors to be slightly higher than internal_reflections.min_host_vel
    * - Reset all entries in all buffer slots for all sensors to 0.
    * - Reset all entries in raw_det_list to 0
    * - Reset f_ok_to_use to true for for all dets in dets_props
    * - Set sensors[0].internal_reflections.age_threshold to 200
    * - Setup 1 detection that is relevant to buffer
    * - Set all other detections in raw_det_list to be irrelevant for buffer (i.e. by having properties such as
    *   rcs > internal_reflections.rcs_max).
    * - Feed the relevant detection into Identify_And_Remove_Internal_Reflections() not always but often enough to get
    *   an occurence_rate higher than internal_reflections.occurrence_threshold.
    * - Change the detection slot index for the relevant detection between iterations
    * - Call the function in this manner 201 times such that the buffer slot containing the relevant detection
    *   has an age and occurence_count such that it will be flagged as internal reflection.
    * - Reset dets_props[0].f_ok_to_use to true as it was flagged during the 201'th call.
    * - Remove the detection that was relevant such that all detections are irrelevant for function.
    * - Call function 33 times to increase buffer age without increasing occurrence count such that the buffer slot's
    *   occurrence rate drops below internal_reflections_occurrence_lowerlimit.
    * - Setup the relevant detection again in raw_det_list on an arbitrary detection slot
    */
   sensors[0].constant.internal_reflections.age_threshold = 200U;

   raw_det_list.number_of_valid_detections = 50;
   for (unsigned int i_det = 0; i_det < raw_det_list.number_of_valid_detections; i_det++)
   {
      raw_det_list.detections[i_det].raw.sensor_id = 1;
   }

   test_det.raw.range = 2.0F;
   test_det.raw.rcs = -15.0F;
   test_det.raw.azimuth = 0.0F;
   test_det.raw.range_rate = 0.0F;

   int det_reoccurrence = 10;
   int detidx = 0;

   for (int iteration = 0; iteration < sensors[0].constant.internal_reflections.age_threshold + 1; iteration++)
   {

     // The detection will only be fed to the algo every det_reoccurrence'th iteration
     if ((iteration % det_reoccurrence) == 0)
     {
        // To switch around the detection slot used to feed the detection
        detidx = iteration % raw_det_list.number_of_valid_detections;

        Copy_Relevant_Detection_Properties(test_det, raw_det_list.detections[detidx]);
     }

     const bool bike_carrier_attached = Is_Bike_Carrier_Attached(sensors, sensor_props);

     for (unsigned int det_idx = 0; det_idx < raw_det_list.number_of_valid_detections; det_idx++)
     {
        const rspp_variant_A::RSPP_Detection_T &current_detection = raw_det_list.detections[det_idx];
        F360_Detection_Props_T &current_detection_prop = dets_props[det_idx];
        const int32_t current_sensor_id = current_detection.raw.sensor_id;
        const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
        F360_Radar_Sensor_Props_T &current_sensor_props = sensor_props[current_sensor_id - 1];
        if (bike_carrier_attached) Identify_And_Mark_Internal_Reflections(current_detection, current_sensor, current_sensor_props, current_detection_prop);
     }

     Copy_Relevant_Detection_Properties(empty_det, raw_det_list.detections[detidx]);
   }

   dets_props[0].f_ok_to_use = true;
   for (int iteration = 0; iteration < 33; iteration++)
   {
      const bool bike_carrier_attached = Is_Bike_Carrier_Attached(sensors, sensor_props);

      for (unsigned int det_idx = 0; det_idx < raw_det_list.number_of_valid_detections; det_idx++)
      {
         const rspp_variant_A::RSPP_Detection_T &current_detection = raw_det_list.detections[det_idx];
         F360_Detection_Props_T &current_detection_prop = dets_props[det_idx];
         const int32_t current_sensor_id = current_detection.raw.sensor_id;
         const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
         F360_Radar_Sensor_Props_T &current_sensor_props = sensor_props[current_sensor_id - 1];
         if (bike_carrier_attached) Identify_And_Mark_Internal_Reflections(current_detection, current_sensor, current_sensor_props, current_detection_prop);
      }
   }

   Copy_Relevant_Detection_Properties(test_det, raw_det_list.detections[21]);

   /** \action
    * Call function again with the same identical, relevant detection that was used during test setup.
    */

   const bool bike_carrier_attached = Is_Bike_Carrier_Attached(sensors, sensor_props);

   for (unsigned int det_idx = 0; det_idx < raw_det_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_detection = raw_det_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = dets_props[det_idx];
      const int32_t current_sensor_id = current_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      F360_Radar_Sensor_Props_T &current_sensor_props = sensor_props[current_sensor_id - 1];
      if (bike_carrier_attached) Identify_And_Mark_Internal_Reflections(current_detection, current_sensor, current_sensor_props, current_detection_prop);
   }

   /** \result
    * Verify that, as the buffer has reset, the detection does is still flagged as ok to use.
    */
   CHECK_TRUE(dets_props[21].f_ok_to_use);
}

/** @}*/

