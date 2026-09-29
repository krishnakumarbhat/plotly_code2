/** \file
 * This file contains unit tests for content of f360_object_list_timestamp_update.cpp file
 */

#include "f360_object_list_timestamp_update.h"
#include <CppUTest/TestHarness.h>

//#include "headerfile_needed.h"

// Unit testing guidelines: http://confluenceprod1.delphiauto.net:8090/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_object_list_timestamp_update
 *  @{
 */

/** \brief
 * Checking if Object_List_Timestamp_Update updating timestamp correctly
 */
TEST_GROUP(f360_object_list_timestamp_update)
{	
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Tracker_Info_T tracker_info = {};

   /** \setup
    * Describe what is done in test setup. Remove test setup function and this tag if it is not used.
    */
   TEST_SETUP()
   {
      // Set up a default scenario for your tests. E.g. assign values to common variables declared above.
   }

   // Defining two support functions
   void Add_Valid_Sensor_With_Certain_Timestamp(
      F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      uint64_t timestamp
   )
   {
      for (uint16_t sensor_idx = 0U; sensor_idx < MAX_NUMBER_OF_SENSORS; sensor_idx++)
      {
         if (sensors[sensor_idx].constant.id == 0U)
         {
            sensors[sensor_idx].constant.id = sensor_idx + 1;
            sensors[sensor_idx].variable.is_valid = true;
            sensors[sensor_idx].variable.f_sensor_fault_detected = false;
            sensors[sensor_idx].variable.look_id = F360_DET_LOOK_ID_0;
            sensors[sensor_idx].variable.timestamp_us = timestamp;
            break;
         }
      }

   }

   void Add_Invalid_Sensor(
      F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const bool f_set_sensor_is_valid_to_false,
      const bool f_set_invalid_look_id,
      const bool f_set_sensor_fault_detected
   )
   {
      for (uint16_t sensor_idx = 0U; sensor_idx < MAX_NUMBER_OF_SENSORS; sensor_idx++)
      {
         if (sensors[sensor_idx].constant.id == 0U)
         {
            sensors[sensor_idx].constant.id = sensor_idx + 1;
            sensors[sensor_idx].variable.is_valid = (f_set_sensor_is_valid_to_false) ? false : true;
            sensors[sensor_idx].variable.look_id = (f_set_invalid_look_id) ? F360_DET_LOOK_ID_INVALID : F360_DET_LOOK_ID_0;
            sensors[sensor_idx].variable.f_sensor_fault_detected = (f_set_sensor_fault_detected) ? true : false;
            break;
         }
      }
   }
};

/** \purpose  
 * Checking if function calculate object list timestamp correctly if all sensor will be valid
 * \req
 * NA.
 */
TEST(f360_object_list_timestamp_update, AllFourSensorsValid)
{
   /** \precond
    * Setting up sensors
    */
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 1000U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 1002U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 1003U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 1005U);
   	
   /** \action
    * Calling Object_List_Timestamp_Update
    */
   Object_List_Timestamp_Update(sensors, tracker_info);

   /** \result
    * Checking if results matching the expectations
    */	
   CHECK_EQUAL(1002, tracker_info.object_list_timestamp)
}

/** \purpose
 * Checking if function calculate object list timestamp correctly if 4 sensor will be valid and 1 at the end invalid based on f_sensor_fault_detected
 * \req
 * NA.
 */
TEST(f360_object_list_timestamp_update, FourSensorsValidOneAtTheEndInvalid_based_on_f_sensor_fault_detected)
{
   /** \precond
    * Setting up sensors
    */
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 2000U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 1002U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 3003U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 1005U);
   
   const bool f_set_invalid_look_id = false;
   const bool f_set_sensor_fault_detected = true;
   const bool f_set_sensor_is_valid_to_false = false;
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);

   /** \action
    * Calling Object_List_Timestamp_Update
    */
   Object_List_Timestamp_Update(sensors, tracker_info);

   /** \result
    * Checking if results matching the expectations
    */
   CHECK_EQUAL(2002, tracker_info.object_list_timestamp)
}

/** \purpose
 * Checking if function calculate object list timestamp correctly if 4 sensor will be valid and 2 will be invalid based on f_sensor_fault_detected
 * \req
 * NA.
 */
TEST(f360_object_list_timestamp_update, FourSensorsValidTwoSensorsInvalid_based_on_f_sensor_fault_detected)
{
   /** \precond
    * Setting up sensors
    */
   const bool f_set_invalid_look_id = false;
   const bool f_set_sensor_fault_detected = true;
   const bool f_set_sensor_is_valid_to_false = false;

   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 2000U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 1002U);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 3003U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 1005U);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);

   /** \action
    * Calling Object_List_Timestamp_Update
    */
   Object_List_Timestamp_Update(sensors, tracker_info);

   /** \result
    * Checking if results matching the expectations
    */
   CHECK_EQUAL(2002, tracker_info.object_list_timestamp)
}

/** \purpose
 * Checking if function calculate object list timestamp correctly if 4 sensor will be valid and 1 in the middle will be invalid based on f_sensor_fault_detected
 * \req
 * NA.
 */
TEST(f360_object_list_timestamp_update, FourSensorsValidOneInTheMiddleInvalid_based_on_f_sensor_fault_detected)
{
   /** \precond
    * Setting up sensors
    */
   const bool f_set_invalid_look_id = false;
   const bool f_set_sensor_fault_detected = true;
   const bool f_set_sensor_is_valid_to_false = false;

   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 4000U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 3002U);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 2003U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 1005U);

   /** \action
    * Calling Object_List_Timestamp_Update
    */
   Object_List_Timestamp_Update(sensors, tracker_info);

   /** \result
    * Checking if results matching the expectations
    */
   CHECK_EQUAL(2502, tracker_info.object_list_timestamp)
}

/** \purpose
 * Checking if function calculate object list timestamp correctly if 1 sensor will be valid and 4 invalid based on f_sensor_fault_detected
 * \req
 * NA.
 */
TEST(f360_object_list_timestamp_update, OneSensorsValidFourInvalid_based_on_f_sensor_fault_detected)
{
   /** \precond
    * Setting up sensors
    */
   const bool f_set_invalid_look_id = false;
   const bool f_set_sensor_fault_detected = true;
   const bool f_set_sensor_is_valid_to_false = false;

   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 1005U);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);

   /** \action
    * Calling Object_List_Timestamp_Update
    */
   Object_List_Timestamp_Update(sensors, tracker_info);

   /** \result
    * Checking if results matching the expectations
    */
   CHECK_EQUAL(1005, tracker_info.object_list_timestamp)
}

/** \purpose
 * Checking if function calculate object list timestamp correctly if 1 sensor will be valid and 4 invalid based on f_sensor_fault_detected
 * \req
 * NA.
 */
TEST(f360_object_list_timestamp_update, AllFourSensorsInvalid_based_on_f_sensor_fault_detected)
{
   /** \precond
    * Setting up sensors
    */
   const bool f_set_invalid_look_id = false;
   const bool f_set_sensor_fault_detected = true;
   const bool f_set_sensor_is_valid_to_false = false;

   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);

   /** \action
    * Calling Object_List_Timestamp_Update
    */
   Object_List_Timestamp_Update(sensors, tracker_info);

   /** \result
    * Checking if results matching the expectations
    */
   CHECK_EQUAL(0, tracker_info.object_list_timestamp)
}

/** \purpose
 * Checking if function calculate object list timestamp correctly if 4 sensor will be valid and 1 at the end invalid based on look_id
 * \req
 * NA.
 */
TEST(f360_object_list_timestamp_update, FourSensorsValidOneAtTheEndInvalid_based_on_lookid)
{
   /** \precond
    * Setting up sensors
    */
   const bool f_set_invalid_look_id = true;
   const bool f_set_sensor_fault_detected = false;
   const bool f_set_sensor_is_valid_to_false = false;

   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 2000U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 1002U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 3003U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 1005U);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);

   /** \action
    * Calling Object_List_Timestamp_Update
    */
   Object_List_Timestamp_Update(sensors, tracker_info);

   /** \result
    * Checking if results matching the expectations
    */
   CHECK_EQUAL(2002, tracker_info.object_list_timestamp)
}

/** \purpose
 * Checking if function calculate object list timestamp correctly if 4 sensor will be valid and 2 will be invalid based on look_id
 * \req
 * NA.
 */
TEST(f360_object_list_timestamp_update, FourSensorsValidTwoSensorsInvalid_based_on_lookid)
{
   /** \precond
    * Setting up sensors
    */
   const bool f_set_invalid_look_id = true;
   const bool f_set_sensor_fault_detected = false;
   const bool f_set_sensor_is_valid_to_false = false;

   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 2000U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 1002U);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 3003U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 1005U);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);

   /** \action
    * Calling Object_List_Timestamp_Update
    */
   Object_List_Timestamp_Update(sensors, tracker_info);

   /** \result
    * Checking if results matching the expectations
    */
   CHECK_EQUAL(2002, tracker_info.object_list_timestamp)
}

/** \purpose
 * Checking if function calculate object list timestamp correctly if 4 sensor will be valid and 1 in the middle will be invalid based on look_id
 * \req
 * NA.
 */
TEST(f360_object_list_timestamp_update, FourSensorsValidOneInTheMiddleInvalid_based_on_lookid)
{
   /** \precond
    * Setting up sensors
    */
   const bool f_set_invalid_look_id = true;
   const bool f_set_sensor_fault_detected = false;
   const bool f_set_sensor_is_valid_to_false = false;

   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 4000U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 3002U);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 2003U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 1005U);

   /** \action
    * Calling Object_List_Timestamp_Update
    */
   Object_List_Timestamp_Update(sensors, tracker_info);

   /** \result
    * Checking if results matching the expectations
    */
   CHECK_EQUAL(2502, tracker_info.object_list_timestamp)
}

/** \purpose
 * Checking if function calculate object list timestamp correctly if 1 sensor will be valid and 4 invalid based on look_id
 * \req
 * NA.
 */
TEST(f360_object_list_timestamp_update, OneSensorsValidFourInvalid_based_on_lookid)
{
   /** \precond
    * Setting up sensors
    */
   const bool f_set_invalid_look_id = true;
   const bool f_set_sensor_fault_detected = false;
   const bool f_set_sensor_is_valid_to_false = false;

   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 1005U);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);

   /** \action
    * Calling Object_List_Timestamp_Update
    */
   Object_List_Timestamp_Update(sensors, tracker_info);

   /** \result
    * Checking if results matching the expectations
    */
   CHECK_EQUAL(1005, tracker_info.object_list_timestamp)
}

/** \purpose
 * Checking if function calculate object list timestamp correctly if 1 sensor will be valid and 4 invalid based on look_id
 * \req
 * NA.
 */
TEST(f360_object_list_timestamp_update, AllFourSensorsInvalid_based_on_lookid)
{
   /** \precond
    * Setting up sensors
    */
   const bool f_set_invalid_look_id = true;
   const bool f_set_sensor_fault_detected = false;
   const bool f_set_sensor_is_valid_to_false = false;
   
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);

   /** \action
    * Calling Object_List_Timestamp_Update
    */
   Object_List_Timestamp_Update(sensors, tracker_info);

   /** \result
    * Checking if results matching the expectations
    */
   CHECK_EQUAL(0, tracker_info.object_list_timestamp)
}

/** \purpose
 * Checking if function calculate object list timestamp correctly if all sensors in all slots will be valid
 * \req
 * NA.
 */
TEST(f360_object_list_timestamp_update, AllSensorsInAllSlotsValid)
{
   /** \precond
    * Setting up sensors
    */
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 2005U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 2005U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 3005U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 11005U); // max
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 4005U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 5005U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 6005U);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 1005U); // min

   /** \action
    * Calling Object_List_Timestamp_Update
    */
   Object_List_Timestamp_Update(sensors, tracker_info);

   /** \result
    * Checking if results matching the expectations
    */
   CHECK_EQUAL(6005, tracker_info.object_list_timestamp)
}


/** \purpose
 * Checking if function calculate object list timestamp correctly if all sensor in all slots will be invalid based on f_sensor_fault_detected
 * \req
 * NA.
 */
TEST(f360_object_list_timestamp_update, AllSensorsInAllSlotsInvalid_based_on_f_sensor_fault_detected)
{
   /** \precond
    * Setting up sensors
    */
   const bool f_set_invalid_look_id = false;
   const bool f_set_sensor_fault_detected = true;
   const bool f_set_sensor_is_valid_to_false = false;
   
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);

   /** \action
    * Calling Object_List_Timestamp_Update
    */
   Object_List_Timestamp_Update(sensors, tracker_info);

   /** \result
    * Checking if results matching the expectations
    */
   CHECK_EQUAL(0, tracker_info.object_list_timestamp)
}


/** \purpose
 * Checking if function calculate object list timestamp correctly if all sensor in all slots will be invalid based on look_id
 * \req
 * NA.
 */
TEST(f360_object_list_timestamp_update, AllSensorsInAllSlotsInvalid_based_on_lookid)
{
   /** \precond
    * Setting up sensors
    */
   const bool f_set_invalid_look_id = true;
   const bool f_set_sensor_fault_detected = false;
   const bool f_set_sensor_is_valid_to_false = false;

   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);

   /** \action
    * Calling Object_List_Timestamp_Update
    */
   Object_List_Timestamp_Update(sensors, tracker_info);

   /** \result
    * Checking if results matching the expectations
    */
   CHECK_EQUAL(0, tracker_info.object_list_timestamp)
}

/** \purpose
 * Checking if function calculate object list timestamp correctly if all sensor in all slots will be invalid based on look_id and f_sensor_fault_detected
 * \req
 * NA.
 */
TEST(f360_object_list_timestamp_update, AllSensorsInAllSlotsInvalid_based_on_lookid_And_f_sensor_fault_detected)
{
   /** \precond
    * Setting up sensors
    */
   const bool f_set_invalid_look_id = true;
   const bool f_set_sensor_fault_detected = true;
   const bool f_set_sensor_is_valid_to_false = true;
   
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);

   /** \action
    * Calling Object_List_Timestamp_Update
    */
   Object_List_Timestamp_Update(sensors, tracker_info);

   /** \result
    * Checking if results matching the expectations
    */
   CHECK_EQUAL(0, tracker_info.object_list_timestamp)
}

/** \purpose
 * Checking if function calculate object list timestamp correctly if timestamps will be close to max int
 * \req
 * NA.
 */
TEST(f360_object_list_timestamp_update, TimestampsCloseToMaxIntTest)
{
   /** \precond
    * Setting up sensors
    */
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, UINT64_MAX - 4);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, UINT64_MAX);

   /** \action
    * Calling Object_List_Timestamp_Update
    */
   Object_List_Timestamp_Update(sensors, tracker_info);

   /** \result
    * Checking if results matching the expectations
    */
   CHECK_EQUAL(UINT64_MAX - 2, tracker_info.object_list_timestamp)
}

/** \purpose
 * Checking if function calculate object list timestamp correctly if timestamps diff will be close to max int
 * \req
 * NA.
 */
TEST(f360_object_list_timestamp_update, TimestampsDiffCloseToMaxIntTest)
{
   /** \precond
    * Setting up sensors
    */
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, UINT64_MAX);
   Add_Valid_Sensor_With_Certain_Timestamp(sensors, 0U);

   /** \action
    * Calling Object_List_Timestamp_Update
    */
   Object_List_Timestamp_Update(sensors, tracker_info);

   /** \result
    * Checking if results matching the expectations
    */
   CHECK_EQUAL(UINT64_MAX / 2, tracker_info.object_list_timestamp)
}

/** \purpose
 * Checking if function calculate object list timestamp correctly if there will be no valid sensors and
 * object list timestamp from previous time instance will be different than 0
 * \req
 * NA.
 */
TEST(f360_object_list_timestamp_update, TimestampsDiffEstimateByElapsedTime)
{
   /** \precond
    * Setting up invalid sensors and tracker info data
    */
   const bool f_set_invalid_look_id = true;
   const bool f_set_sensor_fault_detected = true;
   const bool f_set_sensor_is_valid_to_false = false;

   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);
   Add_Invalid_Sensor(sensors, f_set_sensor_is_valid_to_false, f_set_invalid_look_id, f_set_sensor_fault_detected);

   tracker_info.elapsed_time_s = 0.05F;
   tracker_info.object_list_timestamp = 100000U;

   uint64_t expected_object_list_timstamp = 150000U;

   /** \action
    * Calling Object_List_Timestamp_Update
    */
   Object_List_Timestamp_Update(sensors, tracker_info);

   /** \result
    * Checking if results matching the expectations
    */
   CHECK_EQUAL(expected_object_list_timstamp, tracker_info.object_list_timestamp)
}
/** @}*/
