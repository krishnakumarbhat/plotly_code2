/** \file
 * This file contains unit tests for content of f360_estimate_velocity_by_position_change_helpers.cpp file
 */

#include <CppUTest/TestHarness.h>
#include <cstring>
#include "f360_iterator.h"
#include "f360_math_func.h"
#include "f360_estimate_velocity_by_position_change_helpers.h"
#include "f360_set_variant.h"
#include "f360_detection_hist.h"
#include "rspp_detection_list.h"
#include "f360_detection_props.h"
#include "f360_radar_sensor.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_estimate_velocity_by_position_change_helpers__Update_Unique_Data
 *  @{
 */

/** \brief
 * This is the test group for testing the functionality of Update_Unique_Data(). Update_Unique_Data() is a function for
 * adding a detection timestamp to an array if there are not already any similar timestamp in this array. The array of
 * timestamps outputted from the function is also organized such that it is being tracked from which sensor the detection
 * timestamps originate.
 */
TEST_GROUP(f360_estimate_velocity_by_position_change_helpers__Update_Unique_Data)
{
   // Input variables to function
   float32_t unique_ts[MAX_NUMBER_OF_SENSORS][max_ts]; // Array with unique timestamps per sensor
   int32_t num_ts[MAX_NUMBER_OF_SENSORS]; // Array with number of unique timestamps per senorr
   float32_t ts; // New timestamp
   int32_t sens_idx; // Sensor index from which the new timestamp originates

   // Test pass threshold
   const float32_t test_pass_th = F360_EPSILON;

   /** \setup
    * Set up a default unique_ts and corrsponding num_ts:
    *    - Three active sensors with idx 0, 1 and 5
    *    - Sensor 0 has no timestamps added to uniqe array
    *    - Sensor 1 has 3 timestamps added to unique array - [0.05:0.05:0.15]
    *    - Sensor 5 has 14 (maximum size of array) timestamps added to unique array - [0.05:0.05:0.7]
    *
    *    Clear the new timestamp and timestamp sensor index
    */

   TEST_SETUP()
   {
      // Clear arrays
      std::memset(unique_ts, 0.0F, sizeof(unique_ts));
      std::memset(num_ts, 0, sizeof(num_ts));

      // Sensor 0 needs no setup except clearing above

      // Sensor 1 setup
      for (uint8_t i = 0U; i < 3U; i++)
      {
         unique_ts[1][i] = 0.05F*static_cast<float32_t>(i);
      }
      num_ts[1] = 3;

      // Sensor 5 setup
      for (uint8_t i = 0U; i < 14U; i++)
      {
         unique_ts[5][i] = 0.05F*static_cast<float32_t>(i);
      }
      num_ts[5] = 14;

      // Clear new timestamp and sensor index
      ts = 0.0F;
      sens_idx = 0;
   }
};

/** \purpose
 * Testing that a new unique timestamp is added to sensor 0 that has no previous timestamps when new timestamp is non-zero
 */
TEST(f360_estimate_velocity_by_position_change_helpers__Update_Unique_Data, Test_New_Unique_Nonzero_Timestamp_Added_To_Sensor0)
{
   /** \precond
    * Preconditions from test group setup:
    *    - Three active sensors with idx 0, 1 and 5
    *    - Sensor 0 has no timestamps added to uniqe array
    *    - Sensor 1 has 3 timestamps added to unique array - [0.05:0.05:0.15]
    *    - Sensor 5 has 14 (maximum size of array) timestamps added to unique array - [0.05:0.05:0.7]
    *
    * New time stamp is set to 0.05. New sensor index is set to 0
    */
   ts = 0.05F;
   sens_idx = 0;

   /** \action
    * Copy unique_ts and num_ts before calling Update_Unique_Data() such that we can use this info later to generate the expected output.
    *
    * Call function Update_Unique_Data()
    */
   // Copy arrays
   float32_t exp_unique_ts[MAX_NUMBER_OF_SENSORS][max_ts];
   std::copy(cmn::begin(unique_ts), cmn::end(unique_ts), cmn::begin(exp_unique_ts));

   int32_t exp_num_ts[MAX_NUMBER_OF_SENSORS];
   std::copy(cmn::begin(num_ts), cmn::end(num_ts), cmn::begin(exp_num_ts));

   // Call function
   Update_Unique_Data(sens_idx, ts, unique_ts, num_ts);

   /** \result
    * Expected output from Update_Unique_Data(). Expected is that unique_ts and num_ts are unchanged by the function call except for:
    *    - unique_ts[0][0] = 0.05 (== ts)
    *    - num_ts[0] = 1;
    *
    * Test output from function by comparing to expected data
    */
   // Setup expected data
   exp_unique_ts[0][0] = 0.05F;
   exp_num_ts[0] = 1;

   // Test output
   for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      for (uint8_t j = 0U; j < max_ts; j++)
      {
         DOUBLES_EQUAL_TEXT(exp_unique_ts[i][j], unique_ts[i][j], test_pass_th, "unique_ts is not as expected after the function call");
      }
   }

   for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      DOUBLES_EQUAL_TEXT(exp_num_ts[i], num_ts[i], test_pass_th, "num_ts is not as expected after the function call");
   }
}

/** \purpose
 * Testing that a new unique timestamp is added to sensor 0 that has no previous timestamps when new timestamp is zero
 */
TEST(f360_estimate_velocity_by_position_change_helpers__Update_Unique_Data, Test_New_Unique_Zero_Timestamp_Added_To_Sensor0)
{
   /** \precond
    * Preconditions from test group setup:
    *    - Three active sensors with idx 0, 1 and 5
    *    - Sensor 0 has no timestamps added to uniqe array
    *    - Sensor 1 has 3 timestamps added to unique array - [0.05:0.05:0.15]
    *    - Sensor 5 has 14 (maximum size of array) timestamps added to unique array - [0.05:0.05:0.7]
    *
    * New time stamp is set to 0.0. New sensor index is set to 0
    */
   ts = 0.0F;
   sens_idx = 0;

   /** \action
    * Copy unique_ts and num_ts before calling Update_Unique_Data() such that we can use this info later to generate the expected output.
    *
    * Call function Update_Unique_Data()
    */
   // Copy arrays
   float32_t exp_unique_ts[MAX_NUMBER_OF_SENSORS][max_ts];
   std::copy(cmn::begin(unique_ts), cmn::end(unique_ts), cmn::begin(exp_unique_ts));

   int32_t exp_num_ts[MAX_NUMBER_OF_SENSORS];
   std::copy(cmn::begin(num_ts), cmn::end(num_ts), cmn::begin(exp_num_ts));

   // Call function
   Update_Unique_Data(sens_idx, ts, unique_ts, num_ts);

   /** \result
    * Expected output from Update_Unique_Data(). Expected is that unique_ts and num_ts are unchanged by the function call except for:
    *    - unique_ts[0][0] = 0.0 (== ts)
    *    - num_ts[0] = 1;
    *
    * Test output from function by comparing to expected data
    */
   // Setup expected data
   exp_unique_ts[0][0] = 0.0F;
   exp_num_ts[0] = 1;

   // Test output
   for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      for (uint8_t j = 0U; j < max_ts; j++)
      {
         DOUBLES_EQUAL_TEXT(exp_unique_ts[i][j], unique_ts[i][j], test_pass_th, "unique_ts is not as expected after the function call");
      }
   }

   for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      DOUBLES_EQUAL_TEXT(exp_num_ts[i], num_ts[i], test_pass_th, "num_ts is not as expected after the function call");
   }
}

/** \purpose
 * Testing that a new unique timestamp is added to sensor 1 (that has some previously added timestamps)
 */
TEST(f360_estimate_velocity_by_position_change_helpers__Update_Unique_Data, Test_New_Unique_Timestamp_Added_To_Sensor1)
{
   /** \precond
    * Preconditions from test group setup:
    *    - Three active sensors with idx 0, 1 and 5
    *    - Sensor 0 has no timestamps added to uniqe array
    *    - Sensor 1 has 3 timestamps added to unique array - [0.05:0.05:0.15]
    *    - Sensor 5 has 14 (maximum size of array) timestamps added to unique array - [0.05:0.05:0.7]
    *
    * New time stamp is set to 0.5. New sensor index is set to 1
    */
   ts = 0.5F;
   sens_idx = 1;

   /** \action
    * Copy unique_ts and num_ts before calling Update_Unique_Data() such that we can use this info later to generate the expected output.
    *
    * Call function Update_Unique_Data()
    */
   // Copy arrays
   float32_t exp_unique_ts[MAX_NUMBER_OF_SENSORS][max_ts];
   std::copy(cmn::begin(unique_ts), cmn::end(unique_ts), cmn::begin(exp_unique_ts));

   int32_t exp_num_ts[MAX_NUMBER_OF_SENSORS];
   std::copy(cmn::begin(num_ts), cmn::end(num_ts), cmn::begin(exp_num_ts));

   // Call function
   Update_Unique_Data(sens_idx, ts, unique_ts, num_ts);

   /** \result
    * Expected output from Update_Unique_Data(). Expected is that unique_ts and num_ts are unchanged by the function call except for:
    *    - unique_ts[1][3] = 0.5 (== ts)
    *    - num_ts[1] = 4;
    *
    * Test output from function by comparing to expected data
    */
   // Setup expected data
   exp_unique_ts[1][3] = 0.5F; // Unique
   exp_num_ts[1] = 4;

   // Test output
   for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      for (uint8_t j = 0U; j < max_ts; j++)
      {
         DOUBLES_EQUAL_TEXT(exp_unique_ts[i][j], unique_ts[i][j], test_pass_th, "unique_ts is not as expected after the function call");
      }
   }

   for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      DOUBLES_EQUAL_TEXT(exp_num_ts[i], num_ts[i], test_pass_th, "num_ts is not as expected after the function call");
   }
}

/** \purpose
 * Testing that a new non-unique timestamp is not added to sensor 1 (that has some previously added timestamps)
 */
TEST(f360_estimate_velocity_by_position_change_helpers__Update_Unique_Data, Test_New_Nonunique_Timestamp_Not_Added_To_Sensor1)
{
   /** \precond
    * Preconditions from test group setup:
    *    - Three active sensors with idx 0, 1 and 5
    *    - Sensor 0 has no timestamps added to uniqe array
    *    - Sensor 1 has 3 timestamps added to unique array - [0.05:0.05:0.15]
    *    - Sensor 5 has 14 (maximum size of array) timestamps added to unique array - [0.05:0.05:0.7]
    *
    * New time stamp is set to 0.05. New sensor index is set to 1
    */
   ts = 0.05F; // Nont unique
   sens_idx = 1;

   /** \action
    * Copy unique_ts and num_ts before calling Update_Unique_Data() such that we can use this info later to generate the expected output.
    *
    * Call function Update_Unique_Data()
    */
   // Copy arrays
   float32_t exp_unique_ts[MAX_NUMBER_OF_SENSORS][max_ts];
   std::copy(cmn::begin(unique_ts), cmn::end(unique_ts), cmn::begin(exp_unique_ts));

   int32_t exp_num_ts[MAX_NUMBER_OF_SENSORS];
   std::copy(cmn::begin(num_ts), cmn::end(num_ts), cmn::begin(exp_num_ts));

   // Call function
   Update_Unique_Data(sens_idx, ts, unique_ts, num_ts);

   /** \result
    * Expected output from Update_Unique_Data(). Expected is that unique_ts and num_ts are unchanged by the function call
    *
    * Test output from function by comparing to expected data
    */
   // Test output
   for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      for (uint8_t j = 0U; j < max_ts; j++)
      {
         DOUBLES_EQUAL_TEXT(exp_unique_ts[i][j], unique_ts[i][j], test_pass_th, "unique_ts is not as expected after the function call");
      }
   }

   for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      DOUBLES_EQUAL_TEXT(exp_num_ts[i], num_ts[i], test_pass_th, "num_ts is not as expected after the function call");
   }
}

/** \purpose
 * Testing that a new unique timestamp is not added to sensor 5 (array is already filled with other unique timestamps)
 */
TEST(f360_estimate_velocity_by_position_change_helpers__Update_Unique_Data, Test_New_Unique_Timestamp_Not_Added_To_Sensor5)
{
   /** \precond
    * Preconditions from test group setup:
    *    - Three active sensors with idx 0, 1 and 5
    *    - Sensor 0 has no timestamps added to uniqe array
    *    - Sensor 1 has 3 timestamps added to unique array - [0.05:0.05:0.15]
    *    - Sensor 5 has 14 (maximum size of array) timestamps added to unique array - [0.05:0.05:0.7]
    *
    * New time stamp is set to 1.0. New sensor index is set to 5
    */
   ts = 1.0F; // Unique
   sens_idx = 5;

   /** \action
    * Copy unique_ts and num_ts before calling Update_Unique_Data() such that we can use this info later to generate the expected output.
    *
    * Call function Update_Unique_Data()
    */
   // Copy arrays
   float32_t exp_unique_ts[MAX_NUMBER_OF_SENSORS][max_ts];
   std::copy(cmn::begin(unique_ts), cmn::end(unique_ts), cmn::begin(exp_unique_ts));

   int32_t exp_num_ts[MAX_NUMBER_OF_SENSORS];
   std::copy(cmn::begin(num_ts), cmn::end(num_ts), cmn::begin(exp_num_ts));

   // Call function
   Update_Unique_Data(sens_idx, ts, unique_ts, num_ts);

   /** \result
    * Expected output from Update_Unique_Data(). Expected is that unique_ts and num_ts are unchanged by the function call
    *
    * Test output from function by comparing to expected data
    */
   // Test output
   for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      for (uint8_t j = 0U; j < max_ts; j++)
      {
         DOUBLES_EQUAL_TEXT(exp_unique_ts[i][j], unique_ts[i][j], test_pass_th, "unique_ts is not as expected after the function call");
      }
   }

   for (uint8_t i = 0U; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      DOUBLES_EQUAL_TEXT(exp_num_ts[i], num_ts[i], test_pass_th, "num_ts is not as expected after the function call");
   }
}
/** @}*/

/** \defgroup  f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_SRR5
 *  @{
 */

/** \brief
 * This is the test group for testing the functionality of Get_TS_Thresholds() for a SRR5 sensor.
 * Get_TS_Thresholds() is a function for computing thresholds for number of unique timestamps of
 * detections in a cluster to allow intialization. Computations are based on sensor type and range limits
 * as well as cluster range and compensated range rate.
 */
TEST_GROUP(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_SRR5)
{
   // Input variables to function
   ConstantProps_T constant_sensor_props;
   float32_t cluster_range;
   float32_t cluster_rdotcomp;
   int32_t min_num_ts;
   int32_t min_num_single_sensor_ts;

   /** \setup
    * Set up a SRR5 sensor (range limits don't need to be setup for SRR5 since not used for this sensor type)
    *
    * Setup a short range cluster below (70m) with large compensated range rate (above 2m/s).
    *
    * Clear min_num_ts and min_num_single_sensor_ts
    */

   TEST_SETUP()
   {
      // Set sensor data
      constant_sensor_props.sensor_type = F360_SENSOR_TYPE_SRR5_RADAR;

      // Set cluster data
      cluster_range = 69.9F; // Below 70me
      cluster_rdotcomp = 2.1F; // Above 2.0F

      // Clear thresholds
      min_num_ts = 0;
      min_num_single_sensor_ts = 0U;
   }
};

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an SRR5 sensor, close range cluster with
 * large compensated range rate and when the threshold do not have any prior values.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_SRR5, Test_Thresholds_When_SRR5_Close_Fast_NoPriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - SRR5 sensor
    *   - Cluster range below 70m and compensated range rate above 2m/s
    *   - Thresholds set to zeros
   */

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is 6
    *    - min_num_single_sensor_ts is 4
   */
   CHECK_EQUAL_TEXT(6, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(4, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an SRR5 sensor, close range cluster with
 * small compensated range rate and when the thresholds do not have any prior values.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_SRR5, Test_Thresholds_When_SRR5_Close_Slow_NoPriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - SRR5 sensor
    *   - Cluster range below 70m
    *   - Thresholds set to zeros
    *
    * Set cluster compensated range rate to below 2m/s
   */
   cluster_rdotcomp = 1.9F; // Below 2.0F

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is 9 (3 more  compared to when range rate is large)
    *    - min_num_single_sensor_ts is 6 (2 more  compared to when range rate is large)
   */
   CHECK_EQUAL_TEXT(9, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(6, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an SRR5 sensor, close range cluster with
 * small compensated range rate and when the thresholds do not have some prior values that are larger than the expected
 *  new thresholds.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_SRR5, Test_Thresholds_When_SRR5_Close_Fast_LargePriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - SRR5 sensor
    *   - Cluster range below 70m and compensated range rate to above 2m/s
    *   - Thresholds set to zeros
    *
    * Set thresholds to some large values (larger than 6 and 4 which is the expected new thresholds with no prior)
   */
   min_num_ts = 100;
   min_num_single_sensor_ts = 70;

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is unchanged by the function call, i.e. it is 100
    *    - min_num_single_sensor_ts is is unchanged by the function call, i.e. it is 70
   */
   CHECK_EQUAL_TEXT(100, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(70, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an SRR5 sensor, mid range cluster with
 * large compensated range rate and when the threshold do not have any prior values.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_SRR5, Test_Thresholds_When_SRR5_MidDist_Fast_NoPriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - SRR5 sensor
    *   - Cluster with compensated range rate above 2m/s
    *   - Thresholds set to zeros
    *
    * Set cluster range to above 70m and below 100m
   */
  cluster_range = 85.0F;

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is 8
    *    - min_num_single_sensor_ts is 5
   */
   CHECK_EQUAL_TEXT(8, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(5, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an SRR5 sensor, long range cluster with
 * large compensated range rate and when the threshold do not have any prior values.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_SRR5, Test_Thresholds_When_SRR5_Far_Fast_NoPriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - SRR5 sensor
    *   - Cluster with compensated range rate above 2m/s
    *   - Thresholds set to zeros
    *
    * Set cluster range to above 100m
   */
  cluster_range = 100.1F;

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is 11
    *    - min_num_single_sensor_ts is 8
   */
   CHECK_EQUAL_TEXT(11, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(8, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}
/** @}*/

/** \defgroup  f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_MRR3
 *  @{
 */

/** \brief
 * This is the test group for testing the functionality of Get_TS_Thresholds() for a MRR3 sensor.
 * Get_TS_Thresholds() is a function for computing thresholds for number of unique timestamps of
 * detections in a cluster to allow intialization. Computations are based on sensor type and range limits
 * as well as cluster range and compensated range rate.
 */
TEST_GROUP(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_MRR3)
{
   // Input variables to function
   ConstantProps_T constant_sensor_props;
   float32_t cluster_range;
   float32_t cluster_rdotcomp;
   int32_t min_num_ts;
   int32_t min_num_single_sensor_ts;

   /** \setup
    * Set up a MRR3 sensor (range limits don't need to be setup for MRR3 since not used for this sensor type)
    *
    * Setup a short range cluster below (70m) with large compensated range rate (above 2m/s).
    *
    * Clear min_num_ts and min_num_single_sensor_ts
    */

   TEST_SETUP()
   {
      // Set sensor data
      constant_sensor_props.sensor_type = F360_SENSOR_TYPE_MRR3_RADAR;

      // Set cluster data
      cluster_range = 69.9F; // Below 70me
      cluster_rdotcomp = 2.1F; // Above 2.0F

      // Clear thresholds
      min_num_ts = 0;
      min_num_single_sensor_ts = 0U;
   }
};

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an MRR3 sensor, close range cluster with
 * large compensated range rate and when the threshold do not have any prior values.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_MRR3, Test_Thresholds_When_MRR3_Close_Fast_NoPriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - MRR3 sensor
    *   - Cluster range below 70m and compensated range rate above 2m/s
    *   - Thresholds set to zeros
   */

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is 6
    *    - min_num_single_sensor_ts is 4
   */
   CHECK_EQUAL_TEXT(6, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(4, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an MRR3 sensor, close range cluster with
 * small compensated range rate and when the thresholds do not have any prior values.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_MRR3, Test_Thresholds_When_MRR3_Close_Slow_NoPriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - MRR3 sensor
    *   - Cluster range below 70m
    *   - Thresholds set to zeros
    *
    * Set cluster compensated range rate to below 2m/s
   */
   cluster_rdotcomp = 1.9F; // Below 2.0F

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is 9 (3 more  compared to when range rate is large)
    *    - min_num_single_sensor_ts is 6 (2 more  compared to when range rate is large)
   */
   CHECK_EQUAL_TEXT(9, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(6, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an MRR3 sensor, close range cluster with
 * small compensated range rate and when the thresholds do not have some prior values that are larger than the expected
 *  new thresholds.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_MRR3, Test_Thresholds_When_MRR3_Close_Fast_LargePriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - MRR3 sensor
    *   - Cluster range below 70m and compensated range rate to above 2m/s
    *   - Thresholds set to zeros
    *
    * Set thresholds to some large values (larger than 6 and 4 which is the expected new thresholds with no prior)
   */
   min_num_ts = 100;
   min_num_single_sensor_ts = 70;

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is unchanged by the function call, i.e. it is 100
    *    - min_num_single_sensor_ts is is unchanged by the function call, i.e. it is 70
   */
   CHECK_EQUAL_TEXT(100, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(70, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an MRR3 sensor, mid range cluster with
 * large compensated range rate and when the threshold do not have any prior values.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_MRR3, Test_Thresholds_When_MRR3_MidDist_Fast_NoPriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - MRR3 sensor
    *   - Cluster with compensated range rate above 2m/s
    *   - Thresholds set to zeros
    *
    * Set cluster range to above 70m and below 100m
   */
  cluster_range = 85.0F;

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is 8
    *    - min_num_single_sensor_ts is 5
   */
   CHECK_EQUAL_TEXT(8, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(5, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an MRR3 sensor, long range cluster with
 * large compensated range rate and when the threshold do not have any prior values.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_MRR3, Test_Thresholds_When_MRR3_Far_Fast_NoPriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - MRR3 sensor
    *   - Cluster with compensated range rate above 2m/s
    *   - Thresholds set to zeros
    *
    * Set cluster range to above 100m
   */
  cluster_range = 100.1F;

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is 11
    *    - min_num_single_sensor_ts is 8
   */
   CHECK_EQUAL_TEXT(11, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(8, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}
/** @}*/

/** \defgroup  f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_FLR4
 *  @{
 */

/** \brief
 * This is the test group for testing the functionality of Get_TS_Thresholds() for a FLR4 sensor.
 * Get_TS_Thresholds() is a function for computing thresholds for number of unique timestamps of
 * detections in a cluster to allow intialization. Computations are based on sensor type and range limits
 * as well as cluster range and compensated range rate.
 */
TEST_GROUP(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_FLR4)
{
   // Input variables to function
   ConstantProps_T constant_sensor_props;
   float32_t cluster_range;
   float32_t cluster_rdotcomp;
   int32_t min_num_ts;
   int32_t min_num_single_sensor_ts;

   /** \setup
    * Set up a FLR4 sensor (range limits don't need to be setup for FLR4 since not used for this sensor type)
    *
    * Setup a short range cluster below (150m) with large compensated range rate (above 2m/s).
    *
    * Clear min_num_ts and min_num_single_sensor_ts
    */

   TEST_SETUP()
   {
      // Set sensor data
      constant_sensor_props.sensor_type = F360_SENSOR_TYPE_FLR4_RADAR;

      // Set cluster data
      cluster_range = 149.9F; // Below 150m
      cluster_rdotcomp = 2.1F; // Above 2.0m/s

      // Clear thresholds
      min_num_ts = 0;
      min_num_single_sensor_ts = 0U;
   }
};

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an FLR4 sensor, close range cluster with
 * large compensated range rate and when the threshold do not have any prior values.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_FLR4, Test_Thresholds_When_FLR4_Close_Fast_NoPriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - FLR4 sensor
    *   - Cluster range below 150m and compensated range rate above 2m/s
    *   - Thresholds set to zeros
   */

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is 10
    *    - min_num_single_sensor_ts is 6
   */
   CHECK_EQUAL_TEXT(10, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(6, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an FLR4 sensor, close range cluster with
 * small compensated range rate and when the thresholds do not have any prior values.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_FLR4, Test_Thresholds_When_FLR4_Close_Slow_NoPriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - FLR4 sensor
    *   - Cluster range below 150m
    *   - Thresholds set to zeros
    *
    * Set cluster compensated range rate to below 2m/s
   */
   cluster_rdotcomp = 1.9F; // Below 2.0F

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is 13 (3 more  compared to when range rate is large)
    *    - min_num_single_sensor_ts is 8 (2 more  compared to when range rate is large)
   */
   CHECK_EQUAL_TEXT(13, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(8, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an FLR4 sensor, close range cluster with
 * small compensated range rate and when the thresholds do not have some prior values that are larger than the expected
 *  new thresholds.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_FLR4, Test_Thresholds_When_FLR4_Close_Fast_LargePriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - FLR4 sensor
    *   - Cluster range below 150m and compensated range rate to above 2m/s
    *   - Thresholds set to zeros
    *
    * Set thresholds to some large values (larger than 10 and 6 which is the expected new thresholds with no prior)
   */
   min_num_ts = 100;
   min_num_single_sensor_ts = 70;

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is unchanged by the function call, i.e. it is 100
    *    - min_num_single_sensor_ts is is unchanged by the function call, i.e. it is 70
   */
   CHECK_EQUAL_TEXT(100, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(70, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an FLR4 sensor, mid range cluster with
 * large compensated range rate and when the threshold do not have any prior values.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_FLR4, Test_Thresholds_When_FLR4_MidDist_Fast_NoPriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - FLR4 sensor
    *   - Cluster with compensated range rate above 2m/s
    *   - Thresholds set to zeros
    *
    * Set cluster range to above 150m and below 200m
   */
  cluster_range = 175.0F;

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is 11
    *    - min_num_single_sensor_ts is 8
   */
   CHECK_EQUAL_TEXT(11, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(8, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an FLR4 sensor, long range cluster with
 * large compensated range rate and when the threshold do not have any prior values.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_FLR4, Test_Thresholds_When_FLR4_Far_Fast_NoPriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - FLR4 sensor
    *   - Cluster with compensated range rate above 2m/s
    *   - Thresholds set to zeros
    *
    * Set cluster range to above 200m
   */
  cluster_range = 200.1F;

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is 12
    *    - min_num_single_sensor_ts is 9
   */
   CHECK_EQUAL_TEXT(12, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(9, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}
/** @}*/

/** \defgroup  f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_MRR360_FMCW
 *  @{
 */

/** \brief
 * This is the test group for testing the functionality of Get_TS_Thresholds() for a MRR360 sensor in FMCW multi-mode.
 * Get_TS_Thresholds() is a function for computing thresholds for number of unique timestamps of
 * detections in a cluster to allow intialization. Computations are based on sensor type and range limits
 * as well as cluster range and compensated range rate.
 */
TEST_GROUP(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_MRR360_FMCW)
{
   // Input variables to function
   ConstantProps_T constant_sensor_props;
   float32_t cluster_range;
   float32_t cluster_rdotcomp;
   int32_t min_num_ts;
   int32_t min_num_single_sensor_ts;

   /** \setup
    * Set up a MRR360 sensor with range limits such that range_limits[1] - range_limits[2] is larger than 40m (meaning sensor is in FMCW multi-mode)
    *
    * Setup a short range cluster below (90m) with large compensated range rate (above 2m/s).
    *
    * Clear min_num_ts and min_num_single_sensor_ts
    */

   TEST_SETUP()
   {
      // Set sensor data
      constant_sensor_props.sensor_type = F360_SENSOR_TYPE_MRR360_RADAR;
      constant_sensor_props.range_limits[0] = 50.0F;
      constant_sensor_props.range_limits[1] = 50.0F;
      constant_sensor_props.range_limits[2] = 90.1F;
      constant_sensor_props.range_limits[3] = 90.1F;

      // Set cluster data
      cluster_range = 89.9F; // Below 90m
      cluster_rdotcomp = 2.1F; // Above 2.0m/s

      // Clear thresholds
      min_num_ts = 0;
      min_num_single_sensor_ts = 0U;
   }
};

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an MRR360 sensor, close range cluster with
 * large compensated range rate and when the threshold do not have any prior values.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_MRR360_FMCW, Test_Thresholds_When_MRR360_Close_Fast_NoPriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - MRR360 sensor in FMCW multi-mode (i.e. range limits difference larger than 40m)
    *   - Cluster range below 90m and compensated range rate above 2m/s
    *   - Thresholds set to zeros
   */

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is 6
    *    - min_num_single_sensor_ts is 4
   */
   CHECK_EQUAL_TEXT(6, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(4, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an MRR360 sensor, close range cluster with
 * small compensated range rate and when the thresholds do not have any prior values.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_MRR360_FMCW, Test_Thresholds_When_MRR360_Close_Slow_NoPriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - MRR360 sensor in FMCW multi-mode (i.e. range limits difference larger than 40m)
    *   - Cluster range below 90m
    *   - Thresholds set to zeros
    *
    * Set cluster compensated range rate to below 2m/s
   */
   cluster_rdotcomp = 1.9F; // Below 2.0F

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is 9 (3 more  compared to when range rate is large)
    *    - min_num_single_sensor_ts is 6 (2 more  compared to when range rate is large)
   */
   CHECK_EQUAL_TEXT(9, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(6, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an MRR360 sensor, close range cluster with
 * small compensated range rate and when the thresholds do not have some prior values that are larger than the expected
 *  new thresholds.
 */
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_MRR360_FMCW, Test_Thresholds_When_MRR360_Close_Fast_LargePriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - MRR360 sensor in FMCW multi-mode (i.e. range limits difference larger than 40m)
    *   - Cluster range below 90m and compensated range rate to above 2m/s
    *   - Thresholds set to zeros
    *
    * Set thresholds to some large values (larger than 6 and 4 which is the expected new thresholds with no prior)
    */
   min_num_ts = 100;
   min_num_single_sensor_ts = 70;

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is unchanged by the function call, i.e. it is 100
    *    - min_num_single_sensor_ts is is unchanged by the function call, i.e. it is 70
    */
   CHECK_EQUAL_TEXT(100, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(70, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an MRR360 sensor, mid range cluster with
 * large compensated range rate and when the threshold do not have any prior values.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_MRR360_FMCW, Test_Thresholds_When_MRR360_MidDist_Fast_NoPriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - MRR360 sensor in FMCW multi-mode (i.e. range limits difference larger than 40m)
    *   - Cluster with compensated range rate above 2m/s
    *   - Thresholds set to zeros
    *
    * Set cluster range to above 90m and below 130m
   */
  cluster_range = 110.0F;

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is 8
    *    - min_num_single_sensor_ts is 5
   */
   CHECK_EQUAL_TEXT(8, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(5, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an MRR360 sensor, long range cluster with
 * large compensated range rate and when the threshold do not have any prior values.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_MRR360_FMCW, Test_Thresholds_When_MRR360_Far_Fast_NoPriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - MRR360 sensor in FMCW multi-mode (i.e. range limits difference larger than 40m)
    *   - Cluster with compensated range rate above 2m/s
    *   - Thresholds set to zeros
    *
    * Set cluster range to above 130m
   */
  cluster_range = 130.1F;

   /** \action
    * Call function Get_TS_Thresholds()
   */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is 11
    *    - min_num_single_sensor_ts is 8
   */
   CHECK_EQUAL_TEXT(11, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(8, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}
/** @}*/

/** \brief
 * This is the test group for testing the functionality of Get_TS_Thresholds() for a MRR360 sensor in SFW mode.
 * Get_TS_Thresholds() is a function for computing thresholds for number of unique timestamps of
 * detections in a cluster to allow intialization. Computations are based on sensor type and range limits
 * as well as cluster range and compensated range rate.
 */
TEST_GROUP(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_MRR360_SFW)
{
   // Input variables to function
   ConstantProps_T constant_sensor_props;
   float32_t cluster_range;
   float32_t cluster_rdotcomp;
   int32_t min_num_ts;
   int32_t min_num_single_sensor_ts;

   /** \setup
    * Set up a MRR360 sensor with range limits such that range_limits[1] - range_limits[2] is smaller than 40m (meaning sensor is in SFW)
    *
    * Setup a short range cluster below (120m) with large compensated range rate (above 2m/s).
    *
    * Clear min_num_ts and min_num_single_sensor_ts
    */

   TEST_SETUP()
   {
      // Set sensor data
      constant_sensor_props.sensor_type = F360_SENSOR_TYPE_MRR360_RADAR;
      constant_sensor_props.range_limits[0] = 50.0F;
      constant_sensor_props.range_limits[1] = 50.0F;
      constant_sensor_props.range_limits[2] = 89.9F;
      constant_sensor_props.range_limits[3] = 89.9F;

      // Set cluster data
      cluster_range = 119.9F; // Below 120m
      cluster_rdotcomp = 2.1F; // Above 2.0m/s

      // Clear thresholds
      min_num_ts = 0;
      min_num_single_sensor_ts = 0U;
   }
};

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an MRR360 sensor, close range cluster with
 * large compensated range rate and when the threshold do not have any prior values.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_MRR360_SFW, Test_Thresholds_When_MRR360_Close_Fast_NoPriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - MRR360 sensor in SFW (i.e. range limits difference smaller than 40m)
    *   - Cluster range below 120m and compensated range rate above 2m/s
    *   - Thresholds set to zeros
   */

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is 10
    *    - min_num_single_sensor_ts is 6
   */
   CHECK_EQUAL_TEXT(10, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(6, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an MRR360 sensor, close range cluster with
 * small compensated range rate and when the thresholds do not have any prior values.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_MRR360_SFW, Test_Thresholds_When_MRR360_Close_Slow_NoPriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - MRR360 sensor in SFW (i.e. range limits difference smaller than 40m)
    *   - Cluster range below 120m
    *   - Thresholds set to zeros
    *
    * Set cluster compensated range rate to below 2m/s
   */
   cluster_rdotcomp = 1.9F; // Below 2.0F

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is 13 (3 more  compared to when range rate is large)
    *    - min_num_single_sensor_ts is 8 (2 more  compared to when range rate is large)
   */
   CHECK_EQUAL_TEXT(13, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(8, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an MRR360 sensor, close range cluster with
 * small compensated range rate and when the thresholds do not have some prior values that are larger than the expected
 *  new thresholds.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_MRR360_SFW, Test_Thresholds_When_MRR360_Close_Fast_LargePriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - MRR360 sensor in SFW (i.e. range limits difference smaller than 40m)
    *   - Cluster range below 120m and compensated range rate to above 2m/s
    *   - Thresholds set to zeros
    *
    * Set thresholds to some large values (larger than 10 and 8 which is the expected new thresholds with no prior)
   */
   min_num_ts = 100;
   min_num_single_sensor_ts = 70;

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is unchanged by the function call, i.e. it is 100
    *    - min_num_single_sensor_ts is is unchanged by the function call, i.e. it is 70
   */
   CHECK_EQUAL_TEXT(100, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(70, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an MRR360 sensor, mid range cluster with
 * large compensated range rate and when the threshold do not have any prior values.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_MRR360_SFW, Test_Thresholds_When_MRR360_MidDist_Fast_NoPriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - MRR360 sensor in SFW (i.e. range limits difference larger than 40m)
    *   - Cluster with compensated range rate above 2m/s
    *   - Thresholds set to zeros
    *
    * Set cluster range to above 120m and below 150m
   */
  cluster_range = 135.0F;

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is 11
    *    - min_num_single_sensor_ts is 8
   */
   CHECK_EQUAL_TEXT(11, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(8, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}

/** \purpose
 * Testing that Get_TS_Thresholds() returns correct thresholds for the case of an MRR360 sensor, long range cluster with
 * large compensated range rate and when the threshold do not have any prior values.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_TS_Thresholds_MRR360_SFW, Test_Thresholds_When_MRR360_Far_Fast_NoPriors)
{
   /** \precond
    * Preconditions from test group setup:
    *   - MRR360 sensor in SFW (i.e. range limits difference smaller than 40m)
    *   - Cluster with compensated range rate above 2m/s
    *   - Thresholds set to zeros
    *
    * Set cluster range to above 150m
   */
  cluster_range = 150.1F;

   /** \action
    * Call function Get_TS_Thresholds()
    */
   Get_TS_Thresholds(constant_sensor_props, cluster_range, cluster_rdotcomp, min_num_ts, min_num_single_sensor_ts);

   /** \result
    * Test output from Update_Unique_Data(). Expected is that
    *    - min_num_ts is 12
    *    - min_num_single_sensor_ts is 9
   */
   CHECK_EQUAL_TEXT(12, min_num_ts, "min_num_ts has incorrect value");
   CHECK_EQUAL_TEXT(9, min_num_single_sensor_ts, "min_num_single_sensor_ts has incorrect value");
}
/** @}*/



/** \defgroup  f360_estimate_velocity_by_position_change_helpers__Determine_Base_Weight
 *  @{
 */

/** \brief
 * This is the test group for testing the functionality of Determine_Base_Weight()
 * Determine_Base_Weight() is a function for computing base weight of the detection based on its azimuth
 * and elevation confidence, elevation angle and super resolution flag.
 */
TEST_GROUP(f360_estimate_velocity_by_position_change_helpers__Determine_Base_Weight)
{
   // Input variables to function
   bool f_super_res[6]{}; // super resolution flags for the detections
   int8_t az_conf[6]{}; // azimuth confidence values for the detections
   int8_t el_conf[6]{}; // elevation confidence values for the detections
   float32_t elevation[6]{}; // elevation values for the detections

   /** \setup
    * set up 6 input sets - [f_super_res, az_conf, el_conf, elevation]:
    * [true, 0, 0, 0]
    * [false, 3, 0, 0]
    * [false, 2, 0, 0]
    * [false, 0, 1, 0]
    * [false, 0, 0, 0.122173 (7 deg in rad)]
    * [false, 0, 0, 0.191986 (11 deg in rad)]
    */

   TEST_SETUP()
   {
      f_super_res[0]=true;
      az_conf[1]=3;
      az_conf[2]=2;
      el_conf[3]=1;
      elevation[4]=F360_DEG2RAD(7.0F);
      elevation[5]=F360_DEG2RAD(11.0F);


   }
};

/** \purpose
 * Testing if function returns correct weight if f_super_res is set to true.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Determine_Base_Weight, Test_With_f_super_res)
{
   /** \precond
    * Detection properties as follows:
    * [f_super_res, az_conf, el_conf, elevation] = [true, 0, 0, 0]
   */

   /** \action
    * Call function Determine_Base_Weight()
    */
   float32_t base_weight = Determine_Base_Weight(f_super_res[0], az_conf[0], el_conf[0], elevation[0]);

   /** \result
    * Test output from Determine_Base_Weight(). Expected output is:
    * base_weight = 0.8F
   */
   DOUBLES_EQUAL_TEXT(0.8F, base_weight, F360_EPSILON, "base weight computed incorrectly when f_super_res set");
}

/** \purpose
 * Testing if function returns correct weight if az_conf is set to 3.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Determine_Base_Weight, Test_With_az_conf_3)
{
   /** \precond
    * Detection properties as follows:
    * [f_super_res, az_conf, el_conf, elevation] = [false, 3, 0, 0]
   */

   /** \action
    * Call function Determine_Base_Weight()
    */
   float32_t base_weight = Determine_Base_Weight(f_super_res[1], az_conf[1], el_conf[1], elevation[1]);

   /** \result
    * Test output from Determine_Base_Weight(). Expected output is:
    * base_weight = 0.8F
   */
   DOUBLES_EQUAL_TEXT(0.8F, base_weight, F360_EPSILON, "base weight computed incorrectly when az_conf equals 3");
}

/** \purpose
 * Testing if function returns correct weight if az_conf is set to 2.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Determine_Base_Weight, Test_With_az_conf_2)
{
   /** \precond
    * Detection properties as follows:
    * [f_super_res, az_conf, el_conf, elevation] = [false, 2, 0, 0]
   */

   /** \action
    * Call function Determine_Base_Weight()
    */
   float32_t base_weight = Determine_Base_Weight(f_super_res[2], az_conf[2], el_conf[2], elevation[2]);

   /** \result
    * Test output from Determine_Base_Weight(). Expected output is:
    * base_weight = 0.9F
   */
   DOUBLES_EQUAL_TEXT(0.9F, base_weight, F360_EPSILON, "base weight computed incorrectly when az_conf equals 2");
}

/** \purpose
 * Testing if function returns correct weight if el_conf > 0.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Determine_Base_Weight, Test_With_el_conf_Greater_Than_Zero)
{
   /** \precond
    * Detection properties as follows:
    * [f_super_res, az_conf, el_conf, elevation] = [false, 0, 1, 0]
   */

   /** \action
    * Call function Determine_Base_Weight()
    */
   float32_t base_weight = Determine_Base_Weight(f_super_res[3], az_conf[3], el_conf[3], elevation[3]);

   /** \result
    * Test output from Determine_Base_Weight(). Expected output is:
    * base_weight = 0.95F
   */
   DOUBLES_EQUAL_TEXT(0.95F, base_weight, F360_EPSILON, "base weight computed incorrectly when el_conf > 0");
}


/** \purpose
 * Testing if function returns correct weight if 6.0F DEG < elevation < 10.0F DEG
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Determine_Base_Weight, Test_With_elevation_Whithin_6_to_10_deg)
{
   /** \precond
    * Detection properties as follows:
    * [f_super_res, az_conf, el_conf, elevation] = [false, 0, 0, 0.122173 (7 deg in rad)]
   */

   /** \action
    * Call function Determine_Base_Weight()
    */
   float32_t base_weight = Determine_Base_Weight(f_super_res[4], az_conf[4], el_conf[4], elevation[4]);

   /** \result
    * Test output from Determine_Base_Weight(). Expected output is:
    * base_weight = 0.85F
   */
   DOUBLES_EQUAL_TEXT(0.85F, base_weight, F360_EPSILON, "base weight computed incorrectly when 6.0F < elevation < 10.0F");
}


/** \purpose
 * Testing if function returns correct weight if elevation > 10.0F DEG
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Determine_Base_Weight, Test_With_elevation_Whithin_above_10_deg)
{
   /** \precond
    * Detection properties as follows:
    * [f_super_res, az_conf, el_conf, elevation] = [false, 0, 0, 0.191986 (11 deg in rad)]
   */

   /** \action
    * Call function Determine_Base_Weight()
    */
   float32_t base_weight = Determine_Base_Weight(f_super_res[5], az_conf[5], el_conf[5], elevation[5]);

   /** \result
    * Test output from Determine_Base_Weight(). Expected output is:
    * base_weight = 0.5F
   */
   DOUBLES_EQUAL_TEXT(0.5F, base_weight, F360_EPSILON, "base weight computed incorrectly when elevation > 10.0F");
}


/** \purpose
 * Testing if function returns correct weight if f_super_res is set to true, az_conf = 2, el_conf > 0, 6.0F < elevation < 10.0F.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Determine_Base_Weight, Test_With_More_Nonzero_Inputs)
{
   /** \precond
    * Detection properties as follows:
    * [f_super_res, az_conf, el_conf, elevation] = [true, 2, 1, 0.122173 (7 deg in rad)]
   */

   /** \action
    * Call function Determine_Base_Weight()
    */
   float32_t base_weight = Determine_Base_Weight(f_super_res[0], az_conf[2], el_conf[3], elevation[4]);

   /** \result
    * Test output from Determine_Base_Weight(). Expected output is:
    * base_weight = 0.5814F
   */
   DOUBLES_EQUAL_TEXT(0.5814F, base_weight, F360_EPSILON, "base weight computed incorrectly when elevation > 10.0F");
}
/** @}*/


/** \defgroup  f360_estimate_velocity_by_position_change_helpers__Get_Inlier_Thresholds_for_FLR7_PLT_RADAR
 *  @{
 */

/** \brief
 * This is the test group for testing the functionality of Get_Inlier_Thresholds() with FLR7_PLT_RADAR
 * configuration Get_Inlier_Thresholds() is a function for computing the position thresholds for the
 * detections used in the inlier classification in the IRLS (Iterative Reweighted Least Squares)
 * algorithm for the object initialization.
 */
TEST_GROUP(f360_estimate_velocity_by_position_change_helpers__Get_Inlier_Thresholds_for_FLR7_PLT_RADAR)
{
   // Input variables to function
   ConstantProps_T constant_sensor_props{}; // properties of the sensor
   float32_t cluster_range;   // range of the considered cluster
   float32_t cluster_sin_az;  // sine of the azimuth of the considered cluster
   float32_t cluster_cos_az;  // cosine of the azimuth of the considered cluster
   float32_t k_delta_long;    // computed longitudinal threshold coefficient
   float32_t k_delta_lat;     // computed lateral threshold coefficient

   /** \setup
    * Initialize the sensor with F360_SENSOR_TYPE_FLR7_PLT_RADAR type
    *
    */

   TEST_SETUP()
   {
      constant_sensor_props.sensor_type = F360_SENSOR_TYPE_FLR7_PLT_RADAR;
   }
};

/** \purpose
 * Testing if function returns correct coefficients for close range cluster
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_Inlier_Thresholds_for_FLR7_PLT_RADAR, Test_With_Close_Range_Cluster)
{
   /** \precond
    * sensor type F360_SENSOR_TYPE_FLR7_PLT_RADAR
    * set the cluster range to be close
    * set the azimuth of the cluster to 10 deg
   */
   cluster_range= 8.0F;
   cluster_sin_az= F360_Sinf(F360_DEG2RAD(10.0F));
   cluster_sin_az= F360_Cosf(F360_DEG2RAD(10.0F));


   /** \action
    * Call function Get_Inlier_Thresholds()
    */
   Get_Inlier_Thresholds(constant_sensor_props, cluster_range, cluster_sin_az, cluster_cos_az, k_delta_long, k_delta_lat);


   /** \result
    * Test output from Get_Inlier_Thresholds(). Expected output is:
    * k_delta_long = 0.25
    * k_delta_lat = 0.25
    *
   */
   DOUBLES_EQUAL_TEXT(0.25F, k_delta_long, F360_EPSILON, "k_delta_long value wrong with FLR7_PLT_RADAR and close cluster");
   DOUBLES_EQUAL_TEXT(0.25F, k_delta_lat, F360_EPSILON, "k_delta_lat value wrong with FLR7_PLT_RADAR and close cluster");
}


/** \purpose
 * Testing if function returns correct coefficients for far away cluster
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_Inlier_Thresholds_for_FLR7_PLT_RADAR, Test_With_Far_Range_Cluster)
{
   /** \precond
    * sensor type F360_SENSOR_TYPE_FLR7_PLT_RADAR
    * set the cluster range to be far away
    * set the azimuth of the cluster to 10 deg
   */
   cluster_range= 130.0F;
   cluster_sin_az= F360_Sinf(F360_DEG2RAD(10.0F));
   cluster_sin_az= F360_Cosf(F360_DEG2RAD(10.0F));


   /** \action
    * Call function Get_Inlier_Thresholds()
    */
   Get_Inlier_Thresholds(constant_sensor_props, cluster_range, cluster_sin_az, cluster_cos_az, k_delta_long, k_delta_lat);


   /** \result
    * Test output from Get_Inlier_Thresholds(). Expected output is:
    * k_delta_long = 2.04840016
    * k_delta_lat = 1.70700014
   */
   DOUBLES_EQUAL_TEXT(2.04840016F, k_delta_long, F360_EPSILON, "k_delta_long value wrong with FLR7_PLT_RADAR and far away cluster");
   DOUBLES_EQUAL_TEXT(1.70700014F, k_delta_lat, F360_EPSILON, "k_delta_lat value wrong with FLR7_PLT_RADAR and far away cluster");
}


/** \defgroup  f360_estimate_velocity_by_position_change_helpers__Get_Inlier_Thresholds_for_FLR4_PLUS_PLT_STANDALONE_RADAR
 *  @{
 */

/** \brief
 * This is the test group for testing the functionality of Get_Inlier_Thresholds() for FLR4_PLUS_PLT_STANDALONE
 * Get_Inlier_Thresholds() is a function for computing the position thresholds for the detections
 * used in the inlier classification in the IRLS (Iterative Reweighted Least Squares) algorithm
 * for the object initialization.
 */
TEST_GROUP(f360_estimate_velocity_by_position_change_helpers__Get_Inlier_Thresholds_for_FLR4_PLUS_PLT_STANDALONE_RADAR)
{
   // Input variables to function
   ConstantProps_T constant_sensor_props{}; // properties of the sensor
   float32_t cluster_range;   // range of the considered cluster
   float32_t cluster_sin_az;  // sine of the azimuth of the considered cluster
   float32_t cluster_cos_az;  // cosine of the azimuth of the considered cluster
   float32_t k_delta_long;    // computed longitudinal threshold coefficient
   float32_t k_delta_lat;     // computed lateral threshold coefficient

   /** \setup
    * Initialize the sensor with F360_SENSOR_TYPE_FLR4_PLUS_PLT_STANDALONE_RADAR type
    *
    */

   TEST_SETUP()
   {
      constant_sensor_props.sensor_type = F360_SENSOR_TYPE_FLR4_PLUS_PLT_STANDALONE_RADAR;
      cluster_sin_az= F360_Sinf(F360_DEG2RAD(10.0F));
      cluster_sin_az= F360_Cosf(F360_DEG2RAD(10.0F));
   }
};

/** \purpose
 * Testing if function returns correct coefficients for close cluster
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_Inlier_Thresholds_for_FLR4_PLUS_PLT_STANDALONE_RADAR, Test_With_Close_Range_Cluster)
{
   /** \precond
    * sensor type F360_SENSOR_TYPE_FLR4_PLUS_PLT_STANDALONE_RADAR
    * set the cluster range to be close
    * set the azimuth of the cluster to 10 deg
   */
  cluster_range= 8.0F;

   /** \action
    * Call function Get_Inlier_Thresholds()
    */
   Get_Inlier_Thresholds(constant_sensor_props, cluster_range, cluster_sin_az, cluster_cos_az, k_delta_long, k_delta_lat);


   /** \result
    * Test output from Get_Inlier_Thresholds(). Expected output is:
    * k_delta_long = 0.25
    * k_delta_lat = 0.25
    *
   */
   DOUBLES_EQUAL_TEXT(0.25F, k_delta_long, F360_EPSILON, "k_delta_long value wrong with FLR4_PLUS_PLT_STANDALONE_RADAR and close cluster");
   DOUBLES_EQUAL_TEXT(0.25F, k_delta_lat, F360_EPSILON, "k_delta_lat value wrong with FLR4_PLUS_PLT_STANDALONE_RADAR and close cluster");
}


/** \purpose
 * Testing if function returns correct coefficients for far away cluster
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_Inlier_Thresholds_for_FLR4_PLUS_PLT_STANDALONE_RADAR, Test_With_Far_Range_Cluster)
{
   /** \precond
    * sensor type F360_SENSOR_TYPE_FLR4_PLUS_PLT_STANDALONE_RADAR
    * set the cluster range to be far away
    * set the azimuth of the cluster to 10 deg
   */
  cluster_range= 130.0F;

   /** \action
    * Call function Get_Inlier_Thresholds()
    */
   Get_Inlier_Thresholds(constant_sensor_props, cluster_range, cluster_sin_az, cluster_cos_az, k_delta_long, k_delta_lat);


   /** \result
    * Test output from Get_Inlier_Thresholds(). Expected output is:
    * k_delta_long = 2.56049991
    * k_delta_lat = 1.70700014
    *
   */
   DOUBLES_EQUAL_TEXT(2.56049991F, k_delta_long, F360_EPSILON, "k_delta_long value wrong with FLR4_PLUS_PLT_STANDALONE_RADAR and far away cluster");
   DOUBLES_EQUAL_TEXT(1.70700014F, k_delta_lat, F360_EPSILON, "k_delta_lat value wrong with FLR4_PLUS_PLT_STANDALONE_RADAR and far away cluster");
}
/** @}*/


/** \defgroup  f360_estimate_velocity_by_position_change_helpers__Get_Inlier_Thresholds_for_SRR5_RADAR
 *  @{
 */

/** \brief
 * This is the test group for testing the functionality of Get_Inlier_Thresholds() for SRR5_RADAR
 * Get_Inlier_Thresholds() is a function for computing the position thresholds for the detections
 * used in the inlier classification in the IRLS (Iterative Reweighted Least Squares) algorithm
 * for the object initialization.
 */
TEST_GROUP(f360_estimate_velocity_by_position_change_helpers__Get_Inlier_Thresholds_for_SRR5_RADAR)
{
   // Input variables to function
   ConstantProps_T constant_sensor_props{}; // properties of the sensor
   float32_t cluster_range;   // range of the considered cluster
   float32_t cluster_sin_az;  // sine of the azimuth of the considered cluster
   float32_t cluster_cos_az;  // cosine of the azimuth of the considered cluster
   float32_t k_delta_long;    // computed longitudinal threshold coefficient
   float32_t k_delta_lat;     // computed lateral threshold coefficient

   /** \setup
    * Initialize the sensor with F360_SENSOR_TYPE_SRR5_RADAR type
    *
    */

   TEST_SETUP()
   {
      constant_sensor_props.sensor_type = F360_SENSOR_TYPE_SRR5_RADAR;
      cluster_sin_az= F360_Sinf(F360_DEG2RAD(10.0F));
      cluster_sin_az= F360_Cosf(F360_DEG2RAD(10.0F));
   }
};

/** \purpose
 * Testing if function returns correct coefficients for close cluster
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_Inlier_Thresholds_for_SRR5_RADAR, Test_With_Close_Range_Cluster)
{
   /** \precond
    * sensor type F360_SENSOR_TYPE_SRR5_RADAR
    * set the cluster range to be close
    * set the azimuth of the cluster to 10 deg
   */
   cluster_range= 8.0F;

   /** \action
    * Call function Get_Inlier_Thresholds()
    */
   Get_Inlier_Thresholds(constant_sensor_props, cluster_range, cluster_sin_az, cluster_cos_az, k_delta_long, k_delta_lat);

   /** \result
    * Test output from Get_Inlier_Thresholds(). Expected output is:
    * k_delta_long = 0.25
    * k_delta_lat = 0.25
   */
   DOUBLES_EQUAL_TEXT(0.25F, k_delta_long, F360_EPSILON, "k_delta_long value wrong with SRR5_RADAR and close cluster");
   DOUBLES_EQUAL_TEXT(0.25F, k_delta_lat, F360_EPSILON, "k_delta_lat value wrong with SRR5_RADAR and close cluster");
}


/** \purpose
 * Testing if function returns correct coefficients for far away cluster
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_Inlier_Thresholds_for_SRR5_RADAR, Test_With_Far_Range_Cluster)
{
   /** \precond
    * sensor type F360_SENSOR_TYPE_SRR5_RADAR
    * set the cluster range to be far away
    * set the azimuth of the cluster to 10 deg
   */
   cluster_range= 130.0F;

   /** \action
    * Call function Get_Inlier_Thresholds()
    */
   Get_Inlier_Thresholds(constant_sensor_props, cluster_range, cluster_sin_az, cluster_cos_az, k_delta_long, k_delta_lat);


   /** \result
    * Test output from Get_Inlier_Thresholds(). Expected output is:
    * k_delta_long = 1.96961546
    * k_delta_lat = 1.47721159
   */
   DOUBLES_EQUAL_TEXT(1.96961546F, k_delta_long, F360_EPSILON, "k_delta_long value wrong with SRR5_RADAR and far away cluster");
   DOUBLES_EQUAL_TEXT(1.47721159F, k_delta_lat, F360_EPSILON, "k_delta_lat value wrong with SRR5_RADAR and far away cluster");
}
/** @}*/


/** \defgroup  f360_estimate_velocity_by_position_change_helpers__Get_Inlier_Thresholds_for_MRR360_RADAR_SFW_mode
 *  @{
 */

/** \brief
 * This is the test group for testing the functionality of Get_Inlier_Thresholds() for MRR360_RADAR_SFW
 * Get_Inlier_Thresholds() is a function for computing the position thresholds for the detections
 * used in the inlier classification in the IRLS (Iterative Reweighted Least Squares) algorithm
 * for the object initialization.
 */
TEST_GROUP(f360_estimate_velocity_by_position_change_helpers__Get_Inlier_Thresholds_for_MRR360_RADAR_SFW_mode)
{
   // Input variables to function
   ConstantProps_T constant_sensor_props{}; // properties of the sensor
   float32_t cluster_range;   // range of the considered cluster
   float32_t cluster_sin_az;  // sine of the azimuth of the considered cluster
   float32_t cluster_cos_az;  // cosine of the azimuth of the considered cluster
   float32_t k_delta_long;    // computed longitudinal threshold coefficient
   float32_t k_delta_lat;     // computed lateral threshold coefficient

   /** \setup
    * Initialize the sensor with F360_SENSOR_TYPE_MRR360_RADAR_SFW_mode type
   */

   TEST_SETUP()
   {
      constant_sensor_props.sensor_type = F360_SENSOR_TYPE_MRR360_RADAR;
      cluster_sin_az= F360_Sinf(F360_DEG2RAD(10.0F));
      cluster_sin_az= F360_Cosf(F360_DEG2RAD(10.0F));
   }
};

/** \purpose
 * Testing if function returns correct coefficients for close cluster
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_Inlier_Thresholds_for_MRR360_RADAR_SFW_mode, Test_With_Close_Range_Cluster)
{
   /** \precond
    * sensor type F360_SENSOR_TYPE_MRR360_RADAR
    * set the cluster range to be close
    * set the azimuth of the cluster to 10 deg
   */
   cluster_range= 8.0F;

   /** \action
    * Call function Get_Inlier_Thresholds()
   */
   Get_Inlier_Thresholds(constant_sensor_props, cluster_range, cluster_sin_az, cluster_cos_az, k_delta_long, k_delta_lat);


   /** \result
    * Test output from Get_Inlier_Thresholds(). Expected output is:
    * k_delta_long = 0.25
    * k_delta_lat = 0.25
   */
   DOUBLES_EQUAL_TEXT(0.25F, k_delta_long, F360_EPSILON, "k_delta_long value wrong with MRR360_RADAR_SFW_mode and close cluster");
   DOUBLES_EQUAL_TEXT(0.25F, k_delta_lat, F360_EPSILON, "k_delta_lat value wrong with MRR360_RADAR_SFW_mode and close cluster");
}


/** \purpose
 * Testing if function returns correct coefficients for far away cluster
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_Inlier_Thresholds_for_MRR360_RADAR_SFW_mode, Test_With_Far_Range_Cluster)
{
   /** \precond
    * sensor type F360_SENSOR_TYPE_MRR360_RADAR
    * set the cluster range to be far away
    * set the azimuth of the cluster to 10 deg
   */
   cluster_range= 130.0F;

   /** \action
    * Call function Get_Inlier_Thresholds()
    */
   Get_Inlier_Thresholds(constant_sensor_props, cluster_range, cluster_sin_az, cluster_cos_az, k_delta_long, k_delta_lat);


   /** \result
    * Test output from Get_Inlier_Thresholds(). Expected output is:
    * k_delta_long = 2.56049991
    * k_delta_lat = 1.70700014
   */
   DOUBLES_EQUAL_TEXT(2.56049991F, k_delta_long, F360_EPSILON, "k_delta_long value wrong with MRR360_RADAR_SFW_mode and far away cluster");
   DOUBLES_EQUAL_TEXT(1.70700014F, k_delta_lat, F360_EPSILON, "k_delta_lat value wrong with MRR360_RADAR_SFW_mode and far away cluster");
}
/** @}*/


/** \defgroup  f360_estimate_velocity_by_position_change_helpers__Get_Inlier_Thresholds_for_MRR360_RADAR_FMCW_mode
 *  @{
 */

/** \brief
 * This is the test group for testing the functionality of Get_Inlier_Thresholds() for MRR360_RADAR_FMCW
 * Get_Inlier_Thresholds() is a function for computing the position thresholds for the detections
 * used in the inlier classification in the IRLS (Iterative Reweighted Least Squares) algorithm
 * for the object initialization.
 */
TEST_GROUP(f360_estimate_velocity_by_position_change_helpers__Get_Inlier_Thresholds_for_MRR360_RADAR_FMCW_mode)
{
   // Input variables to function
   ConstantProps_T constant_sensor_props{}; // properties of the sensor
   float32_t cluster_range;   // range of the considered cluster
   float32_t cluster_sin_az;  // sine of the azimuth of the considered cluster
   float32_t cluster_cos_az;  // cosine of the azimuth of the considered cluster
   float32_t k_delta_long;    // computed longitudinal threshold coefficient
   float32_t k_delta_lat;     // computed lateral threshold coefficient

   /** \setup
    * Initialize the sensor with F360_SENSOR_TYPE_MRR360_RADAR_FMCW_mode type
    * with range limits such that range_limits[1] - range_limits[2] is greater than 40m (meaning sensor is in FMCW)
   */

   TEST_SETUP()
   {
      constant_sensor_props.sensor_type = F360_SENSOR_TYPE_MRR360_RADAR;
      constant_sensor_props.range_limits[1] = 100.0F; //only used in test for medium range
      constant_sensor_props.range_limits[2] = 150.0F; //only used in test for long range
      cluster_sin_az= F360_Sinf(F360_DEG2RAD(10.0F));
      cluster_sin_az= F360_Cosf(F360_DEG2RAD(10.0F));
   }
};

/** \purpose
 * Testing if function returns correct coefficients for close cluster
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_Inlier_Thresholds_for_MRR360_RADAR_FMCW_mode, Test_With_Close_Range_Cluster)
{
   /** \precond
    * sensor type F360_SENSOR_TYPE_MRR360_RADAR
    * set the cluster range to be close
    * set the azimuth of the cluster to 10 deg
   */
   cluster_range= 8.0F;

   /** \action
    * Call function Get_Inlier_Thresholds()
   */
   Get_Inlier_Thresholds(constant_sensor_props, cluster_range, cluster_sin_az, cluster_cos_az, k_delta_long, k_delta_lat);

   /** \result
    * Test output from Get_Inlier_Thresholds(). Expected output is:
    * k_delta_long = 0.25
    * k_delta_lat = 0.25
   */
   DOUBLES_EQUAL_TEXT(0.25F, k_delta_long, F360_EPSILON, "k_delta_long value wrong with MRR360_RADAR_FMCW_mode and close cluster");
   DOUBLES_EQUAL_TEXT(0.25F, k_delta_lat, F360_EPSILON, "k_delta_lat value wrong with MRR360_RADAR_FMCW_mode and close cluster");
}


/** \purpose
 * Testing if function returns correct coefficients for far away cluster
*/
TEST(f360_estimate_velocity_by_position_change_helpers__Get_Inlier_Thresholds_for_MRR360_RADAR_FMCW_mode, Test_With_Far_Range_Cluster)
{
   /** \precond
    * sensor type F360_SENSOR_TYPE_MRR360_RADAR
    * set the cluster range to be far away
    * set the azimuth of the cluster to 10 deg
   */
  cluster_range= 130.0F;

   /** \action
    * Call function Get_Inlier_Thresholds()
    */
  Get_Inlier_Thresholds(constant_sensor_props, cluster_range, cluster_sin_az, cluster_cos_az, k_delta_long, k_delta_lat);

   /** \result
    * Test output from Get_Inlier_Thresholds(). Expected output is:
    * k_delta_long = 1.96961546
    * k_delta_lat = 1.47721159
   */
   DOUBLES_EQUAL_TEXT(1.96961546F, k_delta_long, F360_EPSILON, "k_delta_long value wrong with MRR360_RADAR_FMCW_mode and far away cluster");
   DOUBLES_EQUAL_TEXT(1.47721159F, k_delta_lat, F360_EPSILON, "k_delta_lat value wrong with MRR360_RADAR_FMCW_mode and far away cluster");
}
/** @}*/


/** \defgroup  f360_estimate_velocity_by_position_change_helpers__IRLS_2D
 *  @{
 */

/** \brief
 * This is the test group for testing the functionality of IRLS_2D()
 * IRLS_2D() is a function for estimating the initial position and velocity.
 */
TEST_GROUP(f360_estimate_velocity_by_position_change_helpers__IRLS_2D)
{

   F360_Cluster_T cluster{};

   int32_t num_msmt;
   int32_t num_ts[MAX_NUMBER_OF_SENSORS]{};
   int32_t num_confirmed_ts;
   float32_t unique_ts[MAX_NUMBER_OF_SENSORS][max_ts]{};
   float32_t ts[MAX_DETS_IN_OBJ_TRK * 2U]{};

   float32_t k_delta=-1.0F;
   float32_t k_delta_long=-1.0F;
   float32_t k_delta_lat=-1.0F;

   float32_t base_weight[MAX_DETS_IN_OBJ_TRK * 2U]{};
   float32_t pos_reference[MAX_DETS_IN_OBJ_TRK * 2U]{};

   float32_t estimated_longvel{};
   float32_t estimated_latvel{};
   float32_t inlier_ratio_long{};
   float32_t inlier_ratio_lat{};

   float32_t longpos[MAX_DETS_IN_OBJ_TRK * 2U]{};
   float32_t latpos[MAX_DETS_IN_OBJ_TRK * 2U]{};

   F360_Radar_Sensor_T sensor;

   F360_Detection_Hist_T det_hist;
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list;
   F360_Detection_Props_T detections[MAX_NUMBER_OF_DETECTIONS]{};

   /** \setup
    * Setting up calibration parameters for the sensor.
    */

   TEST_SETUP()
   {
      sensor.constant.mounting_position.vcs_boresight_azimuth_angle = 0.0F;
      sensor.refined.time_since_measurement_s = 0.00F;
      sensor.variable.is_valid = true;
      sensor.constant.sensor_type =F360_SENSOR_TYPE_FLR7_PLT_RADAR;
   }

   // Set up a cluster (as well as its 22 detections) with position specified in by xpos and ypos and with velocity xvel and yvel.
   // cluster does not contain any jumpaheads in timestamps

   void setup_moving_cluster_for_IRLS_no_jumpahead(
         F360_Cluster_T& cluster,
         rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
         F360_Detection_Hist_T& det_hist,
         const float32_t xpos, const float32_t ypos,
         const float32_t xvel, const float32_t yvel)
   {
      cluster.vcs_position_x = xpos;
      cluster.vcs_position_y = ypos;
      cluster.rep_vcs_az = atan2f(ypos, xpos);
      cluster.cos_vcs_az = cosf(cluster.rep_vcs_az);
      cluster.sin_vcs_az = sinf(cluster.rep_vcs_az);
      cluster.ndets = 0;
      cluster.num_old_dets = 0;
      cluster.rep_rdotcomp = cluster.cos_vcs_az * xvel + cluster.sin_vcs_az * yvel;
      cluster.f_dealiased = true;

      for (int32_t i = 0; i < 2; i++)
      {
         int32_t n = raw_detect_list.number_of_valid_detections;
         raw_detect_list.number_of_valid_detections++;
         cluster.detids[i] = n + 1;
         cluster.ndets++;
         raw_detect_list.detections[n].raw.azimuth = atan2f(cluster.vcs_position_y, cluster.vcs_position_x);
         raw_detect_list.detections[n].raw.elevation = 0.0F;
         raw_detect_list.detections[n].raw.confid_azimuth = 0;
         raw_detect_list.detections[n].raw.confid_elevation = 0;
         raw_detect_list.detections[n].processed.vcs_az = atan2f(cluster.vcs_position_y, cluster.vcs_position_x);
         raw_detect_list.detections[n].processed.vcs_position_x = cluster.vcs_position_x;
         raw_detect_list.detections[n].processed.vcs_position_y = cluster.vcs_position_y;
         raw_detect_list.detections[n].processed.cos_vcs_az = cosf(raw_detect_list.detections[n].raw.azimuth);
         raw_detect_list.detections[n].processed.sin_vcs_az = sinf(raw_detect_list.detections[n].raw.azimuth);
         raw_detect_list.detections[n].processed.range_rate_compensated = cluster.rep_rdotcomp;

         detections[n].f_angle_amb = false;
         detections[n].f_potential_angle_jump = false;
         detections[n].vcs_position.x = raw_detect_list.detections[n].processed.vcs_position_x;
         detections[n].vcs_position.y = raw_detect_list.detections[n].processed.vcs_position_y;
         detections[n].range_rate_compensated = raw_detect_list.detections[n].processed.range_rate_compensated;
      }

      for (int32_t i = 0; i < 10; i++)
      {
         int32_t i1 = i*2;
         int32_t i2 = i*2+1;
         float32_t T = 0.05F;

         cluster.num_old_dets++;
         int32_t n = det_hist.n_occupied;
         cluster.old_det_idx[i1] = n;
         det_hist.n_occupied++;
         det_hist.det_data[n].vcs_position_x = cluster.vcs_position_x - T * (i + 1) * xvel;
         det_hist.det_data[n].vcs_position_y = cluster.vcs_position_y - T * (i + 1) * yvel;
         det_hist.det_data[n].vcs_az = atan2f(det_hist.det_data[n].vcs_position_y, det_hist.det_data[n].vcs_position_x);
         det_hist.det_data[n].rdot_comp = cosf(det_hist.det_data[n].vcs_az) * xvel + sinf(det_hist.det_data[n].vcs_az) * yvel;
         det_hist.det_data[n].time_since_meas = (i + 1) * T;

         cluster.num_old_dets++;
         n = det_hist.n_occupied;
         cluster.old_det_idx[i2] = n;
         det_hist.n_occupied++;
         det_hist.det_data[n].vcs_position_x = cluster.vcs_position_x - T * (i + 1) * xvel;
         det_hist.det_data[n].vcs_position_y = cluster.vcs_position_y - T * (i + 1) * yvel;
         det_hist.det_data[n].vcs_az = atan2f(det_hist.det_data[n].vcs_position_y, det_hist.det_data[n].vcs_position_x);
         det_hist.det_data[n].rdot_comp = cosf(det_hist.det_data[n].vcs_az) * xvel + sinf(det_hist.det_data[n].vcs_az) * yvel;
         det_hist.det_data[n].time_since_meas = (i + 1) * T;
      }
      cluster.num_types_of_dets[0] = cluster.num_old_dets + cluster.ndets;
   }

   // Set up a cluster (as well as its 22 detections) with position specified in by xpos and ypos and with velocity xvel and yvel.
   // cluster contains jumpahead in timestamp index jumpahead_index_1

   void setup_moving_cluster_for_IRLS_with_jumpahead(
      F360_Cluster_T& cluster,
      rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      F360_Detection_Hist_T& det_hist,
      const float32_t xpos, const float32_t ypos,
      const float32_t xvel, const float32_t yvel,
      const int32_t jumpahead_index_1)
   {
      cluster.vcs_position_x = xpos;
      cluster.vcs_position_y = ypos;
      cluster.rep_vcs_az = atan2f(ypos, xpos);
      cluster.cos_vcs_az = cosf(cluster.rep_vcs_az);
      cluster.sin_vcs_az = sinf(cluster.rep_vcs_az);
      cluster.ndets = 0;
      cluster.num_old_dets = 0;
      cluster.rep_rdotcomp = cluster.cos_vcs_az * xvel + cluster.sin_vcs_az * yvel;
      cluster.f_dealiased = true;

      for (int32_t i = 0; i < 2; i++)
      {
         int32_t n = raw_detect_list.number_of_valid_detections;
         raw_detect_list.number_of_valid_detections++;
         cluster.detids[i] = n + 1;
         cluster.ndets++;
         raw_detect_list.detections[n].raw.azimuth = atan2f(cluster.vcs_position_y, cluster.vcs_position_x);
         raw_detect_list.detections[n].raw.elevation = 0.0F;
         raw_detect_list.detections[n].raw.confid_azimuth = 0;
         raw_detect_list.detections[n].raw.confid_elevation = 0;
         raw_detect_list.detections[n].processed.vcs_az = atan2f(cluster.vcs_position_y, cluster.vcs_position_x);
         raw_detect_list.detections[n].processed.vcs_position_x = cluster.vcs_position_x;
         raw_detect_list.detections[n].processed.vcs_position_y = cluster.vcs_position_y;
         raw_detect_list.detections[n].processed.cos_vcs_az = cosf(raw_detect_list.detections[n].raw.azimuth);
         raw_detect_list.detections[n].processed.sin_vcs_az = sinf(raw_detect_list.detections[n].raw.azimuth);
         raw_detect_list.detections[n].processed.range_rate_compensated = cluster.rep_rdotcomp;

         detections[n].f_angle_amb = false;
         detections[n].f_potential_angle_jump = false;
         detections[n].vcs_position.x = raw_detect_list.detections[n].processed.vcs_position_x;
         detections[n].vcs_position.y = raw_detect_list.detections[n].processed.vcs_position_y;
         detections[n].range_rate_compensated = raw_detect_list.detections[n].processed.range_rate_compensated;
      }

      float32_t timestamp_increment = 0.05F;
      float32_t previous_timestamp = 0.0F;

      float32_t prev_vcs_x_pos = 0.0F;
      float32_t prev_vcs_y_pos = 0.0F;

      for (int32_t i = 0; i < 6; i++)
      {
         int32_t i1 = i*2;
         int32_t i2 = i*2+1;
         float32_t current_iteration_timestamp_increment = timestamp_increment;
         cluster.num_old_dets++;
         int32_t n = det_hist.n_occupied;
         cluster.old_det_idx[i1] = n;
         det_hist.n_occupied++;
         if(i == 0)
         {
            det_hist.det_data[n].vcs_position_x=cluster.vcs_position_x;
            det_hist.det_data[n].vcs_position_y=cluster.vcs_position_y;
         }
         else
         {
            det_hist.det_data[n].vcs_position_x = prev_vcs_x_pos - current_iteration_timestamp_increment * xvel;
            det_hist.det_data[n].vcs_position_y = prev_vcs_y_pos - current_iteration_timestamp_increment * yvel;
         }
         det_hist.det_data[n].vcs_az = atan2f(det_hist.det_data[n].vcs_position_y, det_hist.det_data[n].vcs_position_x);
         det_hist.det_data[n].rdot_comp = cosf(det_hist.det_data[n].vcs_az) * xvel + sinf(det_hist.det_data[n].vcs_az) * yvel;
         det_hist.det_data[n].time_since_meas = previous_timestamp + current_iteration_timestamp_increment;

         if(i == jumpahead_index_1)
         {
            current_iteration_timestamp_increment = 4*timestamp_increment;
         }
         else{
            //do nothing
         }

         cluster.num_old_dets++;
         n = det_hist.n_occupied;
         cluster.old_det_idx[i2] = n;
         det_hist.n_occupied++;
         if(i == 0)
         {
            det_hist.det_data[n].vcs_position_x=cluster.vcs_position_x;
            det_hist.det_data[n].vcs_position_y=cluster.vcs_position_y;
         }
         else
         {
            det_hist.det_data[n].vcs_position_x = prev_vcs_x_pos - current_iteration_timestamp_increment * xvel;
            det_hist.det_data[n].vcs_position_y = prev_vcs_y_pos - current_iteration_timestamp_increment * yvel;
         }
         det_hist.det_data[n].vcs_az = atan2f(det_hist.det_data[n].vcs_position_y, det_hist.det_data[n].vcs_position_x);
         det_hist.det_data[n].rdot_comp = cosf(det_hist.det_data[n].vcs_az) * xvel + sinf(det_hist.det_data[n].vcs_az) * yvel;
         det_hist.det_data[n].time_since_meas = previous_timestamp + current_iteration_timestamp_increment;

         prev_vcs_x_pos = det_hist.det_data[n].vcs_position_x;
         prev_vcs_y_pos = det_hist.det_data[n].vcs_position_y;
         previous_timestamp = det_hist.det_data[n].time_since_meas;

      }
      cluster.num_types_of_dets[0] = cluster.num_old_dets + cluster.ndets;
   }


   // Set up a cluster (as well as its 22 detections) with position specified in by xpos and ypos and with velocity xvel and yvel.
   // cluster contains jumpaheads in timestamp index jumpahead_index_1 and jumpahead_index_2

   void setup_moving_cluster_for_IRLS_with_two_jumpaheads(
      F360_Cluster_T& cluster,
      rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      F360_Detection_Hist_T& det_hist,
      const float32_t xpos, const float32_t ypos,
      const float32_t xvel, const float32_t yvel,
      const int32_t jumpahead_index_1,
      const int32_t jumpahead_index_2)
   {
      cluster.vcs_position_x = xpos;
      cluster.vcs_position_y = ypos;
      cluster.rep_vcs_az = atan2f(ypos, xpos);
      cluster.cos_vcs_az = cosf(cluster.rep_vcs_az);
      cluster.sin_vcs_az = sinf(cluster.rep_vcs_az);
      cluster.ndets = 0;
      cluster.num_old_dets = 0;
      cluster.rep_rdotcomp = cluster.cos_vcs_az * xvel + cluster.sin_vcs_az * yvel;
      cluster.f_dealiased = true;


      for (int32_t i = 0; i < 2; i++)
      {
         int32_t n = raw_detect_list.number_of_valid_detections;
         raw_detect_list.number_of_valid_detections++;
         cluster.detids[i] = n + 1;
         cluster.ndets++;
         raw_detect_list.detections[n].raw.azimuth = atan2f(cluster.vcs_position_y, cluster.vcs_position_x);
         raw_detect_list.detections[n].raw.elevation = 0.0F;
         raw_detect_list.detections[n].raw.confid_azimuth = 0;
         raw_detect_list.detections[n].raw.confid_elevation = 0;
         raw_detect_list.detections[n].processed.vcs_az = atan2f(cluster.vcs_position_y, cluster.vcs_position_x);
         raw_detect_list.detections[n].processed.vcs_position_x = cluster.vcs_position_x;
         raw_detect_list.detections[n].processed.vcs_position_y = cluster.vcs_position_y;
         raw_detect_list.detections[n].processed.cos_vcs_az = cosf(raw_detect_list.detections[n].raw.azimuth);
         raw_detect_list.detections[n].processed.sin_vcs_az = sinf(raw_detect_list.detections[n].raw.azimuth);
         raw_detect_list.detections[n].processed.range_rate_compensated = cluster.rep_rdotcomp;

         detections[n].f_angle_amb = false;
         detections[n].f_potential_angle_jump = false;
         detections[n].vcs_position.x = raw_detect_list.detections[n].processed.vcs_position_x;
         detections[n].vcs_position.y = raw_detect_list.detections[n].processed.vcs_position_y;
         detections[n].range_rate_compensated = raw_detect_list.detections[n].processed.range_rate_compensated;
      }

      float32_t timestamp_increment = 0.05F;
      float32_t previous_timestamp = 0.0F;

      float32_t prev_vcs_x_pos = 0.0F;
      float32_t prev_vcs_y_pos = 0.0F;

      for (int32_t i = 0; i < 10; i++)
      {
         int32_t i1 = i*2;
         int32_t i2 = i*2+1;
         float32_t current_iteration_timestamp_increment = timestamp_increment;

         cluster.num_old_dets++;
         int32_t n = det_hist.n_occupied;
         cluster.old_det_idx[i1] = n;
         det_hist.n_occupied++;
         if(i == 0)
         {
            det_hist.det_data[n].vcs_position_x=cluster.vcs_position_x;
            det_hist.det_data[n].vcs_position_y=cluster.vcs_position_y;
         }
         else
         {
            det_hist.det_data[n].vcs_position_x = prev_vcs_x_pos - current_iteration_timestamp_increment * xvel;
            det_hist.det_data[n].vcs_position_y = prev_vcs_y_pos - current_iteration_timestamp_increment * yvel;
         }
         det_hist.det_data[n].vcs_az = atan2f(det_hist.det_data[n].vcs_position_y, det_hist.det_data[n].vcs_position_x);
         det_hist.det_data[n].rdot_comp = cosf(det_hist.det_data[n].vcs_az) * xvel + sinf(det_hist.det_data[n].vcs_az) * yvel;
         det_hist.det_data[n].time_since_meas = previous_timestamp + current_iteration_timestamp_increment;

         if(i == jumpahead_index_1 || i == jumpahead_index_2)
         {
            current_iteration_timestamp_increment = 4*timestamp_increment;
         }
         else{
            // do nothing
         }

         cluster.num_old_dets++;
         n = det_hist.n_occupied;
         cluster.old_det_idx[i2] = n;
         det_hist.n_occupied++;

         if(i == 0)
         {
            det_hist.det_data[n].vcs_position_x=cluster.vcs_position_x;
            det_hist.det_data[n].vcs_position_y=cluster.vcs_position_y;
         }
         else
         {
            det_hist.det_data[n].vcs_position_x = prev_vcs_x_pos - current_iteration_timestamp_increment * xvel;
            det_hist.det_data[n].vcs_position_y = prev_vcs_y_pos - current_iteration_timestamp_increment * yvel;
         }
         det_hist.det_data[n].vcs_az = atan2f(det_hist.det_data[n].vcs_position_y, det_hist.det_data[n].vcs_position_x);
         det_hist.det_data[n].rdot_comp = cosf(det_hist.det_data[n].vcs_az) * xvel + sinf(det_hist.det_data[n].vcs_az) * yvel;
         det_hist.det_data[n].time_since_meas = previous_timestamp + current_iteration_timestamp_increment;

         prev_vcs_x_pos = det_hist.det_data[n].vcs_position_x;
         prev_vcs_y_pos = det_hist.det_data[n].vcs_position_y;
         previous_timestamp = det_hist.det_data[n].time_since_meas;

      }
      cluster.num_types_of_dets[0] = cluster.num_old_dets + cluster.ndets;
   }


   // function determines base weights for each detection from the cluster and collects
   // ts, longpos and latpos arrays from cluster detections
   void Prepare_Data_And_Determine_Base_Weight(
      F360_Cluster_T cluster,
      rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      F360_Detection_Hist_T& det_hist,
      F360_Detection_Props_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
      float32_t (&ts)[MAX_DETS_IN_OBJ_TRK * 2U],
      float32_t (&longpos)[MAX_DETS_IN_OBJ_TRK * 2U],
      float32_t (&latpos)[MAX_DETS_IN_OBJ_TRK * 2U],
      float32_t (&base_weight)[MAX_DETS_IN_OBJ_TRK * 2U])
   {
      int32_t ndet = 0;
      /* Prepare data and determine the base weight for each detection from current sensor */
      for (int16_t i = 0; i < cluster.num_old_dets; i++)
      {
         const int16_t det_idx = cluster.old_det_idx[i];
         const F360_Detection_Hist_Data_T& det = det_hist.det_data[det_idx];
         ts[ndet] = -det.time_since_meas;
         longpos[ndet] = det.vcs_position_x;
         latpos[ndet] = det.vcs_position_y;
         base_weight[ndet] = Determine_Base_Weight(det.f_super_res, det.az_conf, det.el_conf, det.elevation);
         ndet++;
      }

      for (int16_t i = 0; i < cluster.ndets; i++)
      {
         const int16_t det_idx = cluster.detids[i] - 1;
         const rspp_variant_A::Raw_Detection_T& det = raw_detect_list.detections[det_idx].raw;
         ts[ndet] = -sensor.refined.time_since_measurement_s;
         longpos[ndet] = detections[i].vcs_position.x;
         latpos[ndet] = detections[i].vcs_position.y;
         base_weight[ndet] = Determine_Base_Weight(det.f_super_res, det.confid_azimuth, det.confid_elevation, det.elevation);
         ndet++;
      }
   }

   // function finds unique timestamps from all the detections from a cluster as well as computes ts thresholds
   void Find_Unique_Timestamps_And_Get_Ts_Thresholds(
      F360_Cluster_T cluster,
      F360_Detection_Hist_T det_hist,
      float32_t (&unique_ts)[MAX_NUMBER_OF_SENSORS][max_ts],
      int32_t (&num_ts)[MAX_NUMBER_OF_SENSORS])
   {
      /* Find unique timestamps */
      for (int32_t i = 0; i < cluster.num_old_dets; i++)
      {
         const int16_t det_idx = cluster.old_det_idx[i];
         const float32_t ts = -det_hist.det_data[det_idx].time_since_meas;
         Update_Unique_Data(0, ts, unique_ts, num_ts);
      }

      for (int32_t i = 0; i < cluster.ndets; i++)
      {
         const float32_t ts = -sensor.refined.time_since_measurement_s;
         Update_Unique_Data(0, ts, unique_ts, num_ts);
      }

      int32_t total_num_ts = 0;
      int32_t num_single_sensor_ts = 0;
      int32_t min_num_ts = 0;
      int32_t min_num_single_sensor_ts = 0;
      const float32_t cluster_range = F360_Get_Hypotenuse(cluster.vcs_position_x, cluster.vcs_position_y);

      if (num_ts[0] > 0)
      {
         total_num_ts += num_ts[0];
         num_single_sensor_ts = (num_ts[0] > num_single_sensor_ts) ? num_ts[0] : num_single_sensor_ts;
         Get_TS_Thresholds(sensor.constant, cluster_range, cluster.rep_rdotcomp, min_num_ts, min_num_single_sensor_ts);
      }
   }
};


/** \purpose
 * Test behaviour of the IRLS_2D with the estimation of the velocity of the cluster without any jumpaheads in detection timestamps.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__IRLS_2D, Cluster_Without_Jumpahead_In_ts)
{
   /** \precond
    * Set up the clusterwith 22 detections, xpos = 11, ypos = 12, xvel = 6, yvel = 14, with no jumpahead, Prepare all required data
   */
   setup_moving_cluster_for_IRLS_no_jumpahead(cluster, raw_detect_list, det_hist, 11.0F, 12.0F, 6.0F, 14.0F);

   float32_t cluster_range = F360_Get_Hypotenuse(cluster.vcs_position_x, cluster.vcs_position_y);
   Get_Inlier_Thresholds(sensor.constant, cluster_range, cluster.sin_vcs_az, cluster.cos_vcs_az, k_delta_long, k_delta_lat);
   Prepare_Data_And_Determine_Base_Weight(cluster, raw_detect_list,det_hist, detections, ts, longpos, latpos, base_weight);
   Find_Unique_Timestamps_And_Get_Ts_Thresholds(cluster, det_hist, unique_ts, num_ts);

   /** \action
    * Call function IRLS_2D() separately for longitudinal and lateral velocity
   */
   const bool f_valid_result_long = IRLS_2D(cluster.num_old_dets, num_ts[0], k_delta_long, base_weight, unique_ts[0],
      ts, longpos, num_confirmed_ts, estimated_longvel, inlier_ratio_long);
   const bool f_valid_result_lat = IRLS_2D(cluster.num_old_dets, num_ts[0], k_delta_lat, base_weight, unique_ts[0],
      ts, latpos, num_confirmed_ts, estimated_latvel, inlier_ratio_lat);

   /** \result
    * longitudinal result should be valid,
    * lateral result should be valid,
    * estimated longitudinal velocidy should be equal to 5.9999F
    * estimated lateral velocity should be equal to 13.9999F
   */
   CHECK_TEXT(f_valid_result_long, "longitudinal estimation not valid");
   CHECK_TEXT(f_valid_result_lat, "lateral estimation not valid");
   DOUBLES_EQUAL_TEXT(5.9999F, estimated_longvel, 0.0001F, "estimated longitudinal velocity not equal to expected");
   DOUBLES_EQUAL_TEXT(13.9999F, estimated_latvel,  0.0001F, "estimated lateral velocity not equal to expected");
 }


/** \purpose
 * Test behaviour of the IRLS_2D with the estimation of the velocity of the cluster with the detections with one early jumpahead in timestamps.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__IRLS_2D, Cluster_With_One_Early_Jumpahead_In_ts)
{
   /** \precond
    * Set up the cluster with 22 detections, xpos = 11, ypos = 12, xvel = 6, yvel = 14, with timestamp jumpahead on timestamp 1. (timestamp jumpahead is equal to 3 times
    * regular timestamp increment), Prepare all required data
   */
   setup_moving_cluster_for_IRLS_with_jumpahead(cluster, raw_detect_list, det_hist, 11.0F, 12.0F, 6.0F, 14.0F, 0);

   float32_t cluster_range = F360_Get_Hypotenuse(cluster.vcs_position_x, cluster.vcs_position_y);
   Get_Inlier_Thresholds(sensor.constant, cluster_range, cluster.sin_vcs_az, cluster.cos_vcs_az, k_delta_long, k_delta_lat);
   Prepare_Data_And_Determine_Base_Weight(cluster, raw_detect_list,det_hist, detections, ts, longpos, latpos, base_weight);
   Find_Unique_Timestamps_And_Get_Ts_Thresholds(cluster, det_hist, unique_ts, num_ts);

   /** \action
    * Call function IRLS_2D() separately for longitudinal and lateral velocity
    */
   const bool f_valid_result_long = IRLS_2D(cluster.num_old_dets, num_ts[0], k_delta_long, base_weight, unique_ts[0],
      ts, longpos, num_confirmed_ts, estimated_longvel, inlier_ratio_long);
   const bool f_valid_result_lat = IRLS_2D(cluster.num_old_dets, num_ts[0], k_delta_lat, base_weight, unique_ts[0],
      ts, latpos, num_confirmed_ts, estimated_latvel, inlier_ratio_lat);

   /** \result
    * longitudinal result should be valid,
    * lateral result should be valid,
    * estimated longitudinal velocidy should be equal to 4.6166F
    * estimated lateral velocity should be equal to 13.9999F
   */
   CHECK_TEXT(f_valid_result_long, "longitudinal estimation not valid and should be valid");
   CHECK_TEXT(f_valid_result_lat, "lateral estimation not valid and should be valid");
   DOUBLES_EQUAL_TEXT(4.6166F, estimated_longvel, 0.0001F, "estimated longitudinal velocity not equal to expected");
   DOUBLES_EQUAL_TEXT(13.9999F, estimated_latvel, 0.0001F, "estimated lateral velocity not equal to expected");
 }


/** \purpose
 * Test behaviour of the IRLS_2D with the estimation of the velocity of the cluster with the detections with two early jumpaheads in timestamps.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__IRLS_2D, Cluster_With_Two_Early_Jumpaheads_In_ts)
{
   /** \precond
    * Set up the cluster with 22 detections, xpos = 11, ypos = 12, xvel = 6, yvel = 14, with timestamps jumpaheads on timestamps 0 and 1. (timestamp jumpahead is equal to 3 times
    * regular timestamp increment), Prepare all required data
   */
   setup_moving_cluster_for_IRLS_with_two_jumpaheads(cluster, raw_detect_list, det_hist, 11.0F, 12.0F, 6.0F, 14.0F, 0, 1);

   float32_t cluster_range = F360_Get_Hypotenuse(cluster.vcs_position_x, cluster.vcs_position_y);
   Get_Inlier_Thresholds(sensor.constant, cluster_range, cluster.sin_vcs_az, cluster.cos_vcs_az, k_delta_long, k_delta_lat);
   Prepare_Data_And_Determine_Base_Weight(cluster, raw_detect_list,det_hist, detections, ts, longpos, latpos, base_weight);
   Find_Unique_Timestamps_And_Get_Ts_Thresholds(cluster, det_hist, unique_ts, num_ts);

   /** \action
    * Call function IRLS_2D() separately for longitudinal and lateral velocity
   */
   const bool f_valid_result_long = IRLS_2D(cluster.num_old_dets, num_ts[0], k_delta_long, base_weight, unique_ts[0],
      ts, longpos, num_confirmed_ts, estimated_longvel, inlier_ratio_long);
   const bool f_valid_result_lat = IRLS_2D(cluster.num_old_dets, num_ts[0], k_delta_lat, base_weight, unique_ts[0],
      ts, latpos, num_confirmed_ts, estimated_latvel, inlier_ratio_lat);


   /** \result
    * longitudinal result should be valid,
    * lateral result should be invalid (inlier ratio < 0.7),
    * estimated longitudinal velocidy should be equal to 5.6642F
    * estimated lateral velocity should be equal to 13.6673F
   */
   CHECK_TEXT(f_valid_result_long, "longitudinal estimation not valid and should be valid");
   CHECK_TEXT(f_valid_result_lat, "lateral estimation not valid and should be valid");
   DOUBLES_EQUAL_TEXT(5.6642F, estimated_longvel, 0.0001F, "estimated longitudinal velocity not equal to expected");
   DOUBLES_EQUAL_TEXT(13.6673F, estimated_latvel, 0.0001F, "estimated lateral velocity not equal to expected");
 }


/** \purpose
 * Test behaviour of the IRLS_2D with the estimation of the velocity of the cluster with the detections with two jumpaheads in timestamps- one early, one later.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__IRLS_2D, Cluster_With_Early_And_Later_Jumpaheads_In_ts)
{
   /** \precond
    * Set up the cluster with 22 detections, xpos = 11, ypos = 12, xvel = 6, yvel = 14, with timestamps jumpaheads on timestamps 1 and 6. (timestamp jumpahead is equal to 3 times
    * regular timestamp increment), Prepare all required data
   */
   setup_moving_cluster_for_IRLS_with_two_jumpaheads(cluster, raw_detect_list, det_hist, 11.0F, 12.0F, 6.0F, 14.0F, 1, 6);

   float32_t cluster_range = F360_Get_Hypotenuse(cluster.vcs_position_x, cluster.vcs_position_y);
   Get_Inlier_Thresholds(sensor.constant, cluster_range, cluster.sin_vcs_az, cluster.cos_vcs_az, k_delta_long, k_delta_lat);
   Prepare_Data_And_Determine_Base_Weight(cluster, raw_detect_list,det_hist, detections, ts, longpos, latpos, base_weight);
   Find_Unique_Timestamps_And_Get_Ts_Thresholds(cluster, det_hist, unique_ts, num_ts);

   /** \action
    * Call function IRLS_2D() separately for longitudinal and lateral velocity
   */
   const bool f_valid_result_long = IRLS_2D(cluster.num_old_dets, num_ts[0], k_delta_long, base_weight, unique_ts[0],
         ts, longpos, num_confirmed_ts, estimated_longvel, inlier_ratio_long);
   const bool f_valid_result_lat = IRLS_2D(cluster.num_old_dets, num_ts[0], k_delta_lat, base_weight, unique_ts[0],
      ts, latpos, num_confirmed_ts, estimated_latvel, inlier_ratio_lat);

   /** \result
    * longitudinal result should be valid,
    * lateral result should be valid,
    * estimated longitudinal velocidy should be equal to 6.0F
    * estimated lateral velocity should be equal to 13.9999F
   */
   CHECK_TEXT(f_valid_result_long, "longitudinal estimation not valid and should be valid");
   CHECK_TEXT(f_valid_result_lat, "lateral estimation not valid and should be valid");
   DOUBLES_EQUAL_TEXT(6.0F, estimated_longvel, 0.0001F, "estimated longitudinal velocity not equal to expected");
   DOUBLES_EQUAL_TEXT(13.9999F, estimated_latvel, 0.0001F, "estimated lateral velocity not equal to expected");
}


/** \purpose
 * Test behaviour of the IRLS_2D function when the value of the determinant in the IRLS algorithm is too close to being 0.
*/
TEST(f360_estimate_velocity_by_position_change_helpers__IRLS_2D, Invalid_IRLS_Determinant)
{
   /** \precond
    * Set up the cluster with 22 detections, xpos = 11, ypos = 12, xvel = 6, yvel = 14, with timestamps jumpaheads on timestamps 1 and 6. (timestamp jumpahead is equal to 3 times
    * regular timestamp increment), Prepare all required data
   */
   setup_moving_cluster_for_IRLS_no_jumpahead(cluster, raw_detect_list, det_hist, 0.0F, 12.0F, 0.0F, 5.0F);

   float32_t cluster_range = F360_Get_Hypotenuse(cluster.vcs_position_x, cluster.vcs_position_y);
   Get_Inlier_Thresholds(sensor.constant, cluster_range, cluster.sin_vcs_az, cluster.cos_vcs_az, k_delta_long, k_delta_lat);
   for(int i=0; i<cluster.num_old_dets; i++){
      det_hist.det_data[i].time_since_meas = 1.0F;
   }
   sensor.refined.time_since_measurement_s = 1.0F;
   Prepare_Data_And_Determine_Base_Weight(cluster, raw_detect_list,det_hist, detections, ts, longpos, latpos, base_weight);
   Find_Unique_Timestamps_And_Get_Ts_Thresholds(cluster, det_hist, unique_ts, num_ts);

   /** \action
    * Call function IRLS_2D() separately for longitudinal and lateral velocity
   */
   const bool f_valid_result_long = IRLS_2D(cluster.num_old_dets, num_ts[0], k_delta_long, base_weight, unique_ts[0],
         ts, longpos, num_confirmed_ts, estimated_longvel, inlier_ratio_long);
   const bool f_valid_result_lat = IRLS_2D(cluster.num_old_dets, num_ts[0], k_delta_lat, base_weight, unique_ts[0],
      ts, latpos, num_confirmed_ts, estimated_latvel, inlier_ratio_lat);

   /** \result
    * longitudinal result should not be valid,
    * lateral result should not be valid
   */
   CHECK_TEXT(!f_valid_result_long, "longitudinal estimation valid and should not be");
   CHECK_TEXT(!f_valid_result_lat, "lateral estimation valid and should not be");
}
/** @}*/
