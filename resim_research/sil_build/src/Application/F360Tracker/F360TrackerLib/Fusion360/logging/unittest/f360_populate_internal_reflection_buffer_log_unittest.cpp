/** \file
 * This file contains unit tests for content of f360_populate_internal_reflection_buffer_log.cpp file
 */

#include "f360_populate_internal_reflection_buffer_log.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_populate_internal_reflection_buffer_log
 *  @{
 */

/** \brief
 * This test group tests the functionality of f360_populate_internal_reflection_buffer_log.
 */
TEST_GROUP(f360_populate_internal_reflection_buffer_log)
{
   // Declare common variables used within all tests in this test group.
   F360_Radar_Sensor_Props_T input_sensor_props[MAX_NUMBER_OF_SENSORS]{};
   Internal_Reflection_Buffer_Slot internal_reflections_buffer[INTERNAL_REFLECTIONS_BUFFER_SIZE]{}; // Buffer containing internal reflection patterns

   /** \setup
    * Common setup for test group f360_populate_internal_reflection_buffer_log.
    */
   TEST_SETUP()
   {
      // Set up a default scenario for your tests. E.g. assign values to common variables declared above.
      internal_reflections_buffer[0].azimuth = 1.0f;
      internal_reflections_buffer[0].rcs = 1.1f;
      internal_reflections_buffer[0].range = 10.0f;
      internal_reflections_buffer[0].occurrence_count = 1u;
      internal_reflections_buffer[0].age = 1u;

      input_sensor_props[4].internal_reflections_buffer[0] = internal_reflections_buffer[0];
      
      internal_reflections_buffer[1].azimuth = 2.0f;
      internal_reflections_buffer[1].rcs = 1.2f;
      internal_reflections_buffer[1].range = 10.2f;
      internal_reflections_buffer[1].occurrence_count = 2u;
      internal_reflections_buffer[1].age = 2u;

      input_sensor_props[9].internal_reflections_buffer[1] = internal_reflections_buffer[1];
   }

};

/** \purpose  
 * Test functionality of Populate_Internal_Reflection_Buffer_Data function.
 * \req NA
 */
TEST(f360_populate_internal_reflection_buffer_log, Test_Populate_Internal_Reflection_Buffer_Data)
{
   /** \precond
    * Prepare a reflection buffer log buffer to be used when testing Populate_Internal_Reflection_Buffer_Data.
    */
   F360_Internal_Reflection_Buffer_T reflection_buffer_log[MAX_NUMBER_OF_SENSORS]{};
   Populate_Internal_Reflection_Buffer_Log_Data(reflection_buffer_log, input_sensor_props);
      
   /** \action
    * Call tested function Populate_Internal_Reflection_Buffer_Data
    * and collect output data in output_sensor_props. 
    */
   F360_Radar_Sensor_Props_T output_sensor_props[MAX_NUMBER_OF_SENSORS]{};
   Populate_Internal_Reflection_Buffer_Data(output_sensor_props, reflection_buffer_log);
      

   /** \result
    * Test captured ouput data collected in output_sensor_props
    * against prepared expected data defined in internal_reflections_buffer. 
    */	
   CHECK_EQUAL_TEXT(internal_reflections_buffer[0].age, output_sensor_props[4].internal_reflections_buffer[0].age, 
                      "Reflection buffer age does not match expected value");
   CHECK_EQUAL_TEXT(internal_reflections_buffer[0].occurrence_count, output_sensor_props[4].internal_reflections_buffer[0].occurrence_count, 
                      "Reflection buffer occurence count not match expected value");
   CHECK_EQUAL_TEXT(internal_reflections_buffer[0].range, output_sensor_props[4].internal_reflections_buffer[0].range, 
                      "Reflection buffer range does not match expected value");
   CHECK_EQUAL_TEXT(internal_reflections_buffer[0].rcs, output_sensor_props[4].internal_reflections_buffer[0].rcs, 
                      "Reflection buffer rcs does not match expected value");
   CHECK_EQUAL_TEXT(internal_reflections_buffer[0].azimuth, output_sensor_props[4].internal_reflections_buffer[0].azimuth, 
                      "Reflection buffer azimuth does not match expected value");

   CHECK_EQUAL_TEXT(internal_reflections_buffer[1].age, output_sensor_props[9].internal_reflections_buffer[1].age, 
                      "Reflection buffer age does not match expected value");
   CHECK_EQUAL_TEXT(internal_reflections_buffer[1].occurrence_count, output_sensor_props[9].internal_reflections_buffer[1].occurrence_count, 
                      "Reflection buffer occurence count not match expected value");
   CHECK_EQUAL_TEXT(internal_reflections_buffer[1].range, output_sensor_props[9].internal_reflections_buffer[1].range, 
                      "Reflection buffer range does not match expected value");
   CHECK_EQUAL_TEXT(internal_reflections_buffer[1].rcs, output_sensor_props[9].internal_reflections_buffer[1].rcs, 
                      "Reflection buffer rcs does not match expected value");
   CHECK_EQUAL_TEXT(internal_reflections_buffer[1].azimuth, output_sensor_props[9].internal_reflections_buffer[1].azimuth, 
                      "Reflection buffer azimuth does not match expected value");
}
/** \purpose  
 * Test functionality of Populate_Internal_Reflection_Buffer_Log_Data function.
 * \req NA
 */
TEST(f360_populate_internal_reflection_buffer_log, Test_Populate_Internal_Reflection_Buffer_Log_Data)
{
   /** \precond
    * Prepare an output reflection buffer log buffer to be used when testing Populate_Internal_Reflection_Buffer_Log_Data.
    */
   F360_Internal_Reflection_Buffer_T output_reflection_buffer_log[MAX_NUMBER_OF_SENSORS]{};

   //Populate sensor properties
   /** \action
    * Call tested function Populate_Internal_Reflection_Buffer_Log_Data
    * and collect output data in output_reflection_buffer_log. 
    */
   Populate_Internal_Reflection_Buffer_Log_Data(output_reflection_buffer_log, input_sensor_props);

   /** \result
    * Test captured output data collected in output_reflection_buffer_log
    * against prepared expected data defined in internal_reflections_buffer. 
    */
   CHECK_EQUAL_TEXT(internal_reflections_buffer[0].age, output_reflection_buffer_log[4].age[0], 
                     "Reflection buffer age does not match expected value");
   CHECK_EQUAL_TEXT(internal_reflections_buffer[0].occurrence_count, output_reflection_buffer_log[4].occurrence_count[0], 
                     "Reflection buffer occurence count not match expected value");
   CHECK_EQUAL_TEXT(internal_reflections_buffer[0].range, output_reflection_buffer_log[4].range[0], 
                    "Reflection buffer range does not match expected value");
   CHECK_EQUAL_TEXT(internal_reflections_buffer[0].rcs, output_reflection_buffer_log[4].amplitude[0], 
                    "Reflection buffer rcs does not match expected value");

   CHECK_EQUAL_TEXT(internal_reflections_buffer[0].azimuth, output_reflection_buffer_log[4].azimuth[0], 
                     "Reflection buffer azimuth does not match expected value");

   CHECK_EQUAL_TEXT(internal_reflections_buffer[1].age, output_reflection_buffer_log[9].age[1], 
                     "Reflection buffer age does not match expected value");
   CHECK_EQUAL_TEXT(internal_reflections_buffer[1].occurrence_count, output_reflection_buffer_log[9].occurrence_count[1], 
                     "Reflection buffer occurence count not match expected value");
   CHECK_EQUAL_TEXT(internal_reflections_buffer[1].range, output_reflection_buffer_log[9].range[1], 
                     "Reflection buffer range does not match expected value");
   CHECK_EQUAL_TEXT(internal_reflections_buffer[1].rcs, output_reflection_buffer_log[9].amplitude[1], 
                     "Reflection buffer rcs does not match expected value");
   CHECK_EQUAL_TEXT(internal_reflections_buffer[1].azimuth, output_reflection_buffer_log[9].azimuth[1], 
                     "Reflection buffer azimuth does not match expected value");
}
/** @}*/
