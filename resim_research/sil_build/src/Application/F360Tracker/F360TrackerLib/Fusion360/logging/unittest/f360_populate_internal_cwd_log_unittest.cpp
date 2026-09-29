/** \file
 * This file contains unit tests for content of f360_populate_internal_cwd_log.cpp file
 */

#include "f360_populate_internal_cwd_log.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_populate_internal_cwd_log
 *  @{
 */

/** \brief
 * Test suit for functions in f360_populate_internal_cwd_log.cpp
 * Verify those functions return the value as expected.
 */
TEST_GROUP(f360_populate_internal_cwd_log)
{
   // Declare common variables used within all tests in this test group.
   CWD_Data_T cwd_data;
   F360_Internal_CWD_T cwd_log[MAX_NUMBER_OF_SENSORS];
   /** \setup
    * Initialize the default calibration values and sensor parameters
    */
   TEST_SETUP()
   {
      // Set up a default scenario for your tests. E.g. assign values to common variables declared above.
   }
};

/** \purpose
 * Verifty Populate_Internal_CWD_Log_Data() returns the values as expected
 * \req NA
 */
TEST(f360_populate_internal_cwd_log, Check_Populate_Internal_CWD_Log_Data)
{
   /** \precond
    * Use the test group default values
    */

   /** \action
    * Initialized the static enviroment class object and call  Populate_Internal_CWD_Log_Data().
    */
   for (uint32_t sample_idx = 0U; sample_idx < 5U; ++sample_idx)
   {
      float32_t lateral_position = 0.5F + static_cast<float32_t>(sample_idx) * 0.1F;
      cwd_data.circular_buffer[0][sample_idx] = lateral_position;
   }
   cwd_data.buffer_index[0] = 2;

   Populate_Internal_CWD_Log_Data(cwd_log, cwd_data);

   /** \result
    * check that the output match expected data.
    */
   DOUBLES_EQUAL(0.7F, cwd_log[0].measurement_lateral_position[0], F360_EPSILON)
   DOUBLES_EQUAL(0.8F, cwd_log[0].measurement_lateral_position[1], F360_EPSILON)
   DOUBLES_EQUAL(0.9F, cwd_log[0].measurement_lateral_position[2], F360_EPSILON)
   DOUBLES_EQUAL(0.5F, cwd_log[0].measurement_lateral_position[3], F360_EPSILON)
   DOUBLES_EQUAL(0.6F, cwd_log[0].measurement_lateral_position[4], F360_EPSILON)
}

TEST(f360_populate_internal_cwd_log, Check_Populate_Internal_CWD_Data)
{
   /** \precond
    * Set up the initial values before calling Populate_Internal_CWD_Data
    */
   for (uint32_t sample_idx = 0U; sample_idx < 5U; ++sample_idx)
   {
      float32_t lateral_position = 0.5F + static_cast<float32_t>(sample_idx) * 0.1F;
      cwd_log[0].measurement_lateral_position[sample_idx] = lateral_position;
      cwd_log[0].measurement_is_valid[sample_idx] = true;
   }

   /** \action
    * call Populate_Internal_CWD_Data().
    */
   Populate_Internal_CWD_Data(cwd_data, cwd_log);
   /** \result
    * Check that the cwd output match expected data.
    */
   DOUBLES_EQUAL(0.5F, cwd_data.circular_buffer[0][0], F360_EPSILON)
   DOUBLES_EQUAL(0.6F, cwd_data.circular_buffer[0][1], F360_EPSILON)
   DOUBLES_EQUAL(0.7F, cwd_data.circular_buffer[0][2], F360_EPSILON)
   DOUBLES_EQUAL(0.8F, cwd_data.circular_buffer[0][3], F360_EPSILON)
   DOUBLES_EQUAL(0.9F, cwd_data.circular_buffer[0][4], F360_EPSILON)
   CHECK_EQUAL(0, cwd_data.buffer_index[0])
}
/** @}*/
