/** \file
 * This file contains unit tests for content of rspp.cpp file
 */

#include "rspp.h"
#include "rspp_internal.h"
#include "rspp_state.h"
#include <CppUTest/TestHarness.h>
#include <cstring>
#include <cmath>
#include <limits>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace rspp_variant_A;

static constexpr int32_t RSPP_INVALID_ID = -1;

/** \defgroup  test_RSPP_Initialize
 *  @{
 */

/** \brief
 * Tests for RSPP_Initialize function
 *
 * \details
 * Function under test: void RSPP_Initialize(void)
 *
 * Initializes internal data of RSPP and updates internal state.
 * Must be called before any other RSPP functions.
 */
TEST_GROUP(test_RSPP_Initialize)
{
   // Named constants for test clarity
   static constexpr float32_t TOLERANCE = 0.0001F;
   static constexpr uint8_t MAX_SENSOR_COUNT = 10U;
   static constexpr uint8_t STRESS_TEST_ITERATIONS = 100U;
   static constexpr uint8_t VALID_SENSOR_ID_MIN = 1U;
   static constexpr uint8_t VALID_SENSOR_ID_MAX = MAX_NUMBER_OF_SENSORS;

   /** \setup
    * Setup for RSPP_Initialize tests
    */
   TEST_SETUP()
   {
      // No setup required - each test will call RSPP_Initialize as needed
   }

   /** \teardown
    * Teardown for RSPP_Initialize tests
    */
   TEST_TEARDOWN()
   {
      RSPP_State_Reset();
   }
};

// ==============================================================================
// Basic Functionality Tests
// ==============================================================================

/** \purpose
 * Test that RSPP_Initialize sets state to INITIALIZED
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Initialize, Initialize_TC_Initialize_Sets_State_To_Initialized)
{
   /** \step{1}
    * Verifies that RSPP_Initialize sets the module state to INITIALIZED.
    */

   /** \precond
    * RSPP module exists and can be initialized
    */

   /** \action
    * Call RSPP_Initialize
    */
   RSPP_Initialize();

   /** \result
    * State should be RSPP_STATE_INITIALIZED
    */
   RSPP_States_Type_T state = RSPP_State_Get_Current_State();
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state);
}

/** \purpose
 * Test that RSPP_Initialize can be called multiple times (idempotent)
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Initialize, Initialize_TC_Multiple_Initialize_Calls_Are_Idempotent)
{
   /** \step{1}
    * Checks that multiple calls to RSPP_Initialize do not change the state after the first call.
    */

   /** \precond
    * RSPP module exists
    */

   /** \action
    * Call RSPP_Initialize multiple times
    */
   RSPP_Initialize();
   RSPP_States_Type_T state1 = RSPP_State_Get_Current_State();

   RSPP_Initialize();
   RSPP_States_Type_T state2 = RSPP_State_Get_Current_State();

   RSPP_Initialize();
   RSPP_States_Type_T state3 = RSPP_State_Get_Current_State();

   /** \result
    * All calls should result in INITIALIZED state
    */
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state1);
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state2);
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state3);
}

// ==============================================================================
// State Transition Tests
// ==============================================================================

/** \purpose
 * Test that Initialize transitions from any state to INITIALIZED
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Initialize, Initialize_TC_Initialize_From_Uninitialized_State)
{
   /** \step{1}
    * Ensures RSPP_Initialize transitions from any state to INITIALIZED.
    */

   /** \precond
    * RSPP starts in uninitialized state
    */
   RSPP_States_Type_T state = RSPP_State_Get_Current_State();
   CHECK_EQUAL(RSPP_STATE_UNINITIALIZED, state);

   /** \action
    * Call RSPP_Initialize to transition to INITIALIZED
    */
   RSPP_Initialize();

   /** \result
    * State should be INITIALIZED regardless of starting state
    */
   state = RSPP_State_Get_Current_State();
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state);
}

/** \purpose
 * Test that Initialize resets state from INIT_COMPLETE back to INITIALIZED
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Initialize, Initialize_TC_Reinitialize_From_Init_Complete_State)
{
   /** \step{1}
    * Checks that re-initializing from INIT_COMPLETE resets state to INITIALIZED.
    */

   /** \precond
    * RSPP is initialized and calibration is set (reaching INIT_COMPLETE)
    */
   RSPP_Initialize();

   // Set up sensor calibration to reach INIT_COMPLETE state
   ConstantProps_T sensor_calibration;
   memset(&sensor_calibration, 0, sizeof(sensor_calibration));
   sensor_calibration.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
   sensor_calibration.polarity = 1;
   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      sensor_calibration.fov_min_az_rad[look_id] = -1.0F;
      sensor_calibration.fov_max_az_rad[look_id] = 1.0F;
      sensor_calibration.fov_min_el_rad[look_id] = -0.5F;
      sensor_calibration.fov_max_el_rad[look_id] = 0.5F;
      sensor_calibration.r_wrapping[look_id] = 1.0F;
      sensor_calibration.v_wrapping[look_id] = 30.0F;
   }

   // Calibrate all sensors to reach INIT_COMPLETE
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));
   for (uint8_t sensor_id = VALID_SENSOR_ID_MIN; sensor_id <= VALID_SENSOR_ID_MAX; ++sensor_id)
   {
      sensor_array[sensor_id - 1].constant.id = sensor_id;
      sensor_array[sensor_id - 1].constant.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      sensor_array[sensor_id - 1].constant.polarity = 1;
      for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
      {
         sensor_array[sensor_id - 1].constant.fov_min_az_rad[look_id] = -1.0F;
         sensor_array[sensor_id - 1].constant.fov_max_az_rad[look_id] = 1.0F;
         sensor_array[sensor_id - 1].constant.fov_min_el_rad[look_id] = -0.5F;
         sensor_array[sensor_id - 1].constant.fov_max_el_rad[look_id] = 0.5F;
         sensor_array[sensor_id - 1].constant.r_wrapping[look_id] = 1.0F;
         sensor_array[sensor_id - 1].constant.v_wrapping[look_id] = 30.0F;
      }
   }
   RSPP_Set_Sensor_Calibrations(sensor_array);
   /** \action
    * Call RSPP_Initialize again to reset
    */
   RSPP_Initialize();

   /** \result
    * State should be reset to INITIALIZED (not INIT_COMPLETE)
    */
   RSPP_States_Type_T state = RSPP_State_Get_Current_State();
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state);
}

// ==============================================================================
// Calibration State Tests
// ==============================================================================

/** \purpose
 * Test that Initialize allows subsequent calibration setting
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Initialize, Initialize_TC_Initialize_Enables_Sensor_Calibration)
{
   /** \step{1}
    * Confirms that sensor calibration can be set after initialization.
    */

   /** \precond
    * Call RSPP_Initialize
    */
   RSPP_Initialize();

   /** \action
    * Try to set sensor calibration after initialization
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));
   sensor_array[0].constant.id = VALID_SENSOR_ID_MIN;
   sensor_array[0].constant.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
   sensor_array[0].constant.polarity = 1;
   sensor_array[0].constant.mounting_position.vcs_position.height = 0.5F; // Must be >= 0.3F
   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      sensor_array[0].constant.fov_min_az_rad[look_id] = -1.0F;
      sensor_array[0].constant.fov_max_az_rad[look_id] = 1.0F;
      sensor_array[0].constant.fov_min_el_rad[look_id] = -0.5F;
      sensor_array[0].constant.fov_max_el_rad[look_id] = 0.5F;
      sensor_array[0].constant.r_wrapping[look_id] = 1.0F;
      sensor_array[0].constant.v_wrapping[look_id] = 30.0F;
      sensor_array[0].constant.range_limits[look_id] = 200.0F;
      sensor_array[0].constant.min_aliaised_range_rate[look_id] = -10.0F; // Must be in [-200, -5]
   }
   // For remaining sensors, copy the first sensor configuration with different IDs
   for (uint8_t sensor_id = VALID_SENSOR_ID_MIN + 1; sensor_id <= VALID_SENSOR_ID_MAX; ++sensor_id)
   {
      sensor_array[sensor_id - 1].constant = sensor_array[0].constant;
      sensor_array[sensor_id - 1].constant.id = sensor_id;
   }

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Calibration should succeed after initialization
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

// ==============================================================================
// Consistency Tests
// ==============================================================================

/** \purpose
 * Test that Get_Current_State returns consistent value after Initialize
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Initialize, Initialize_TC_State_Remains_Consistent_After_Initialize)
{
   /** \step{1}
    * Ensures Get_Current_State returns consistent value after initialization.
    */

   /** \precond
    * Call RSPP_Initialize
    */
   RSPP_Initialize();

   /** \action
    * Query state multiple times
    */
   RSPP_States_Type_T state1 = RSPP_State_Get_Current_State();
   RSPP_States_Type_T state2 = RSPP_State_Get_Current_State();
   RSPP_States_Type_T state3 = RSPP_State_Get_Current_State();

   /** \result
    * All queries should return same state
    */
   CHECK_EQUAL(state1, state2);
   CHECK_EQUAL(state2, state3);
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state1);
}

// ==============================================================================
// Interaction Tests
// ==============================================================================

/** \purpose
 * Test Initialize before and after sensor calibration operations
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Initialize, Initialize_TC_Initialize_Before_And_After_Calibration)
{
   /** \step{1}
    * Checks initialization before and after sensor calibration operations.
    */

   /** \precond
    * Start fresh
    */

   /** \action
    * Initialize, calibrate, then initialize again
    */
   RSPP_Initialize();
   RSPP_States_Type_T state_after_init = RSPP_State_Get_Current_State();

   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));
   for (uint8_t sensor_id = VALID_SENSOR_ID_MIN; sensor_id <= VALID_SENSOR_ID_MAX; ++sensor_id)
   {
      sensor_array[sensor_id - 1].constant.id = sensor_id;
      sensor_array[sensor_id - 1].constant.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      sensor_array[sensor_id - 1].constant.polarity = 1;
      for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
      {
         sensor_array[sensor_id - 1].constant.fov_min_az_rad[look_id] = -1.0F;
         sensor_array[sensor_id - 1].constant.fov_max_az_rad[look_id] = 1.0F;
         sensor_array[sensor_id - 1].constant.fov_min_el_rad[look_id] = -0.5F;
         sensor_array[sensor_id - 1].constant.fov_max_el_rad[look_id] = 0.5F;
         sensor_array[sensor_id - 1].constant.r_wrapping[look_id] = 1.0F;
         sensor_array[sensor_id - 1].constant.v_wrapping[look_id] = 30.0F;
      }
   }
   RSPP_Set_Sensor_Calibrations(sensor_array);

   RSPP_Initialize();
   RSPP_States_Type_T state_after_reinit = RSPP_State_Get_Current_State();

   /** \result
    * Both initializations should result in INITIALIZED state
    */
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state_after_init);
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state_after_reinit);
}

/** \purpose
 * Test that Initialize allows all sensors to be calibrated subsequently
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Initialize, Initialize_TC_Initialize_Allows_All_Sensors_Calibration)
{
   /** \step{1}
    * Verifies that all sensors can be calibrated after initialization.
    */

   /** \precond
    * Call RSPP_Initialize
    */
   RSPP_Initialize();

   /** \action
    * Set calibration for all sensors
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));
   for (uint8_t sensor_id = VALID_SENSOR_ID_MIN; sensor_id <= VALID_SENSOR_ID_MAX; ++sensor_id)
   {
      sensor_array[sensor_id - 1].constant.id = sensor_id;
      sensor_array[sensor_id - 1].constant.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      sensor_array[sensor_id - 1].constant.polarity = 1;
      sensor_array[sensor_id - 1].constant.mounting_position.vcs_position.height = 0.5F;
      for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
      {
         sensor_array[sensor_id - 1].constant.fov_min_az_rad[look_id] = -1.0F;
         sensor_array[sensor_id - 1].constant.fov_max_az_rad[look_id] = 1.0F;
         sensor_array[sensor_id - 1].constant.fov_min_el_rad[look_id] = -0.5F;
         sensor_array[sensor_id - 1].constant.fov_max_el_rad[look_id] = 0.5F;
         sensor_array[sensor_id - 1].constant.r_wrapping[look_id] = 1.0F;
         sensor_array[sensor_id - 1].constant.v_wrapping[look_id] = 30.0F;
         sensor_array[sensor_id - 1].constant.range_limits[look_id] = 200.0F;
         sensor_array[sensor_id - 1].constant.min_aliaised_range_rate[look_id] = -10.0F;
      }
   }

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * All sensors should be calibrated successfully
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

// ==============================================================================
// Negative Tests
// ==============================================================================

/** \purpose
 * Test that Initialize properly resets calibration count
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Initialize, Initialize_TC_Initialize_Resets_Sensor_Count)
{
   /** \step{1}
    * Ensures that re-initializing resets the sensor calibration count.
    */

   /** \precond
    * RSPP is initialized and one sensor is calibrated
    */
   RSPP_Initialize();

   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));
   for (uint8_t sensor_id = VALID_SENSOR_ID_MIN; sensor_id <= VALID_SENSOR_ID_MAX; ++sensor_id)
   {
      sensor_array[sensor_id - 1].constant.id = sensor_id;
      sensor_array[sensor_id - 1].constant.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      sensor_array[sensor_id - 1].constant.polarity = 1;
      sensor_array[sensor_id - 1].constant.mounting_position.vcs_position.height = 0.5F;
      for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
      {
         sensor_array[sensor_id - 1].constant.fov_min_az_rad[look_id] = -1.0F;
         sensor_array[sensor_id - 1].constant.fov_max_az_rad[look_id] = 1.0F;
         sensor_array[sensor_id - 1].constant.fov_min_el_rad[look_id] = -0.5F;
         sensor_array[sensor_id - 1].constant.fov_max_el_rad[look_id] = 0.5F;
         sensor_array[sensor_id - 1].constant.r_wrapping[look_id] = 1.0F;
         sensor_array[sensor_id - 1].constant.v_wrapping[look_id] = 30.0F;
         sensor_array[sensor_id - 1].constant.range_limits[look_id] = 200.0F;
         sensor_array[sensor_id - 1].constant.min_aliaised_range_rate[look_id] = -10.0F;
      }
   }

   RSPP_Return_Type_T first_cal = RSPP_Set_Sensor_Calibrations(sensor_array);
   CHECK_EQUAL(RSPP_E_OK, first_cal);

   /** \action
    * Call RSPP_Initialize to reset, then calibrate same sensor again
    */
   RSPP_Initialize();
   RSPP_Return_Type_T second_cal = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should be able to calibrate the same sensor ID again after reinitialize
    */
   CHECK_EQUAL(RSPP_E_OK, second_cal);
}

/** \purpose
 * Test behavior when Initialize is called with all sensors already calibrated
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Initialize, Initialize_TC_Initialize_With_All_Sensors_Already_Calibrated)
{
   /** \step{1}
    * Checks behavior when initializing with all sensors already calibrated.
    */

   /** \precond
    * RSPP is initialized and all sensors are calibrated
    */
   RSPP_Initialize();

   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));
   for (uint8_t sensor_id = VALID_SENSOR_ID_MIN; sensor_id <= VALID_SENSOR_ID_MAX; ++sensor_id)
   {
      sensor_array[sensor_id - 1].constant.id = sensor_id;
      sensor_array[sensor_id - 1].constant.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      sensor_array[sensor_id - 1].constant.polarity = 1;
      sensor_array[sensor_id - 1].constant.mounting_position.vcs_position.height = 0.5F;
      for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
      {
         sensor_array[sensor_id - 1].constant.fov_min_az_rad[look_id] = -1.0F;
         sensor_array[sensor_id - 1].constant.fov_max_az_rad[look_id] = 1.0F;
         sensor_array[sensor_id - 1].constant.fov_min_el_rad[look_id] = -0.5F;
         sensor_array[sensor_id - 1].constant.fov_max_el_rad[look_id] = 0.5F;
         sensor_array[sensor_id - 1].constant.r_wrapping[look_id] = 1.0F;
         sensor_array[sensor_id - 1].constant.v_wrapping[look_id] = 30.0F;
         sensor_array[sensor_id - 1].constant.range_limits[look_id] = 200.0F;
         sensor_array[sensor_id - 1].constant.min_aliaised_range_rate[look_id] = -10.0F;
      }
   }

   RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \action
    * Call RSPP_Initialize again
    */
   RSPP_Initialize();
   RSPP_States_Type_T state_after_reinit = RSPP_State_Get_Current_State();

   /** \result
    * State should be INITIALIZED (calibrations cleared/reset)
    */
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state_after_reinit);
}

/** \purpose
 * Test that RSPP_Set_Sensor_Calibration works after Initialize is called
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Initialize, Initialize_TC_Set_Calibration_Works_After_Initialize)
{
   /** \step{1}
    * Verifies that sensor calibration can be set after initialization.
    */

   /** \precond
    * RSPP module exists
    */

   /** \action
    * Initialize then attempt to set sensor calibration
    */
   RSPP_Initialize();

   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));
   for (uint8_t sensor_id = VALID_SENSOR_ID_MIN; sensor_id <= VALID_SENSOR_ID_MAX; ++sensor_id)
   {
      sensor_array[sensor_id - 1].constant.id = sensor_id;
      sensor_array[sensor_id - 1].constant.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      sensor_array[sensor_id - 1].constant.polarity = 1;
      sensor_array[sensor_id - 1].constant.mounting_position.vcs_position.height = 0.5F;
      for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
      {
         sensor_array[sensor_id - 1].constant.fov_min_az_rad[look_id] = -1.0F;
         sensor_array[sensor_id - 1].constant.fov_max_az_rad[look_id] = 1.0F;
         sensor_array[sensor_id - 1].constant.fov_min_el_rad[look_id] = -0.5F;
         sensor_array[sensor_id - 1].constant.fov_max_el_rad[look_id] = 0.5F;
         sensor_array[sensor_id - 1].constant.r_wrapping[look_id] = 1.0F;
         sensor_array[sensor_id - 1].constant.v_wrapping[look_id] = 30.0F;
         sensor_array[sensor_id - 1].constant.range_limits[look_id] = 200.0F;
         sensor_array[sensor_id - 1].constant.min_aliaised_range_rate[look_id] = -10.0F;
      }
   }

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Calibration should succeed after Initialize (verifies Initialize enables operations)
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

// ==============================================================================
// Edge Case Tests
// ==============================================================================

/** \purpose
 * Test that Initialize works after partial sensor calibration
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Initialize, Initialize_TC_Initialize_After_Partial_Sensor_Calibration)
{
   /** \step{1}
    * Checks that Initialize works after only some sensors are calibrated.
    */

   /** \precond
    * RSPP is initialized and some sensors are calibrated
    */
   RSPP_Initialize();

   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));
   // Only calibrate first 3 sensors with invalid data for the rest
   for (uint8_t sensor_id = VALID_SENSOR_ID_MIN; sensor_id <= VALID_SENSOR_ID_MIN + 2; ++sensor_id)
   {
      sensor_array[sensor_id - 1].constant.id = sensor_id;
      sensor_array[sensor_id - 1].constant.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      sensor_array[sensor_id - 1].constant.polarity = 1;
      for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
      {
         sensor_array[sensor_id - 1].constant.fov_min_az_rad[look_id] = -1.0F;
         sensor_array[sensor_id - 1].constant.fov_max_az_rad[look_id] = 1.0F;
         sensor_array[sensor_id - 1].constant.fov_min_el_rad[look_id] = -0.5F;
         sensor_array[sensor_id - 1].constant.fov_max_el_rad[look_id] = 0.5F;
         sensor_array[sensor_id - 1].constant.r_wrapping[look_id] = 1.0F;
         sensor_array[sensor_id - 1].constant.v_wrapping[look_id] = 30.0F;
      }
   }
   // Invalidate sensor 4 to cause partial failure
   sensor_array[3].constant.id = 0; // Invalid ID

   RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \action
    * Call RSPP_Initialize to reset
    */
   RSPP_Initialize();
   RSPP_States_Type_T state = RSPP_State_Get_Current_State();

   /** \result
    * State should be reset to INITIALIZED
    */
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state);
}

/** @}*/

/** \defgroup  test_RSPP_Set_Sensor_Calibrations
 *  @{
 */

/** \brief
 * Tests for RSPP_Set_Sensor_Calibrations function
 *
 * \details
 * Function under test: RSPP_Return_Type_T RSPP_Set_Sensor_Calibrations(
 *     const F360_Radar_Sensor_T (&dynamic_sensor_data)[MAX_NUMBER_OF_SENSORS])
 *
 * Sets the sensor calibration for all sensors in a single batch operation.
 * Each sensor at array index i must have sensor_calibration.id = i + 1.
 * On success, transitions system state to INIT_COMPLETE.
 */
TEST_GROUP(test_RSPP_Set_Sensor_Calibrations)
{
   // Named constants for test clarity
   static constexpr uint8_t VALID_SENSOR_ID_MIN = 1U;
   static constexpr uint8_t VALID_SENSOR_ID_MAX = MAX_NUMBER_OF_SENSORS;
   static constexpr uint8_t INVALID_SENSOR_ID_ZERO = 0U;
   static constexpr uint8_t INVALID_SENSOR_ID_ABOVE_MAX = MAX_NUMBER_OF_SENSORS + 1U;

   // Valid range constants
   static constexpr float32_t VALID_LONG_MIN = -10.0F;
   static constexpr float32_t VALID_LONG_MAX = 1.0F;
   static constexpr float32_t VALID_LAT_MIN = -1.5F;
   static constexpr float32_t VALID_LAT_MAX = 1.5F;
   static constexpr float32_t VALID_HEIGHT_MIN = 0.3F;
   static constexpr float32_t VALID_HEIGHT_MAX = 1.3F;
   static constexpr float32_t VALID_FOV_MIN_AZ = -3.14159265358979323846F; // -PI
   static constexpr float32_t VALID_FOV_MAX_AZ = 3.14159265358979323846F;  // PI
   static constexpr float32_t VALID_FOV_MIN_EL = -3.14159265358979323846F;
   static constexpr float32_t VALID_FOV_MAX_EL = 3.14159265358979323846F;
   static constexpr float32_t VALID_V_WRAP_MIN = 10.0F;
   static constexpr float32_t VALID_V_WRAP_MAX = 100.0F;
   static constexpr float32_t VALID_R_WRAP_MIN = -2.0F;
   static constexpr float32_t VALID_R_WRAP_MAX = 2.0F;
   static constexpr float32_t VALID_RANGE_LIMIT_MIN = 0.0F;
   static constexpr float32_t VALID_RANGE_LIMIT_MAX = 500.0F;
   static constexpr float32_t VALID_MIN_ALIASED_RR_MIN = -200.0F;
   static constexpr float32_t VALID_MIN_ALIASED_RR_MAX = -5.0F;

   // Typical valid values
   static constexpr float32_t TYPICAL_LONGITUDINAL = -0.8F;
   static constexpr float32_t TYPICAL_LATERAL = 0.8F;
   static constexpr float32_t TYPICAL_HEIGHT = 0.5F;
   static constexpr float32_t TYPICAL_BORESIGHT_AZ = 0.0F;
   static constexpr float32_t TYPICAL_FOV_MIN_AZ = -1.0F;
   static constexpr float32_t TYPICAL_FOV_MAX_AZ = 1.0F;
   static constexpr float32_t TYPICAL_FOV_MIN_EL = -0.5F;
   static constexpr float32_t TYPICAL_FOV_MAX_EL = 0.5F;
   static constexpr float32_t TYPICAL_R_WRAPPING = 1.0F;
   static constexpr float32_t TYPICAL_V_WRAPPING = 30.0F;
   static constexpr float32_t TYPICAL_RANGE_LIMIT = 250.0F;
   static constexpr float32_t TYPICAL_MIN_ALIASED_RR = -100.0F;

   static constexpr int32_t VALID_POLARITY_POSITIVE = 1;
   static constexpr int32_t VALID_POLARITY_NEGATIVE = -1;
   static constexpr int32_t INVALID_POLARITY = 0;

   static constexpr float32_t TOLERANCE = 0.0001F;

   /**
    * Helper function to create a valid sensor calibration structure
    */
   void create_valid_sensor_calibration(ConstantProps_T & sensor_calibration, uint8_t sensor_id)
   {
      memset(&sensor_calibration, 0, sizeof(sensor_calibration));
      sensor_calibration.id = sensor_id;
      sensor_calibration.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      sensor_calibration.polarity = VALID_POLARITY_POSITIVE;

      sensor_calibration.mounting_position.vcs_position.longitudinal = TYPICAL_LONGITUDINAL;
      sensor_calibration.mounting_position.vcs_position.lateral = TYPICAL_LATERAL;
      sensor_calibration.mounting_position.vcs_position.height = TYPICAL_HEIGHT;
      sensor_calibration.mounting_position.vcs_boresight_azimuth_angle = TYPICAL_BORESIGHT_AZ;

      for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
      {
         sensor_calibration.fov_min_az_rad[look_id] = TYPICAL_FOV_MIN_AZ;
         sensor_calibration.fov_max_az_rad[look_id] = TYPICAL_FOV_MAX_AZ;
         sensor_calibration.fov_min_el_rad[look_id] = TYPICAL_FOV_MIN_EL;
         sensor_calibration.fov_max_el_rad[look_id] = TYPICAL_FOV_MAX_EL;
         sensor_calibration.r_wrapping[look_id] = TYPICAL_R_WRAPPING;
         sensor_calibration.v_wrapping[look_id] = TYPICAL_V_WRAPPING;
         sensor_calibration.range_limits[look_id] = TYPICAL_RANGE_LIMIT;
         sensor_calibration.min_aliaised_range_rate[look_id] = TYPICAL_MIN_ALIASED_RR;
      }
   }

   /**
    * Helper function to create a valid sensor array for all sensors
    * Sets up all sensors with valid calibrations
    */
   void create_valid_sensor_array(F360_Radar_Sensor_T(&sensor_array)[MAX_NUMBER_OF_SENSORS])
   {
      // Zero-initialize the entire array
      memset(sensor_array, 0, sizeof(sensor_array));

      // Create valid calibration for each sensor
      for (uint8_t sensor_id = VALID_SENSOR_ID_MIN; sensor_id <= VALID_SENSOR_ID_MAX; ++sensor_id)
      {
         create_valid_sensor_calibration(sensor_array[sensor_id - 1].constant, sensor_id);
      }
   }

   /** \setup
    * Setup for RSPP_Set_Sensor_Calibration tests
    */
   TEST_SETUP()
   {
      RSPP_Initialize();
   }

   /** \teardown
    * Teardown for RSPP_Set_Sensor_Calibration tests
    */
   TEST_TEARDOWN()
   {
      RSPP_State_Reset();
   }
};

// ==============================================================================
// Basic Functionality Tests
// ==============================================================================

/** \purpose
 * Test setting valid sensor calibration for sensor ID 1
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Valid_Calibration_Sensor_1)
{
   /** \step{1}
    * Verifies that setting a valid calibration for sensor ID 1 succeeds.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set valid calibration for all sensors
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should return RSPP_E_OK
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

/** \purpose
 * Test that valid calibration is stored correctly
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Calibration_Data_Stored_Correctly)
{
   /** \step{1}
    * Checks that calibration data is stored and retrieved correctly.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set valid calibration and retrieve it
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);

   // Customize sensor 1 with specific test values
   ConstantProps_T &sensor_calibration = sensor_array[0].constant;
   sensor_calibration.mounting_position.vcs_position.longitudinal = 0.8F;
   sensor_calibration.mounting_position.vcs_position.lateral = 1.2F;
   sensor_calibration.mounting_position.vcs_position.height = 0.8F;

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);
   const RSPP_Sensor_Calib_T &stored_cal = RSPP_State_Get_Sensor_Calibration(0);

   /** \result
    * Stored values should match set values
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   DOUBLES_EQUAL(0.8F, stored_cal.vcs_mounting_position.longitudinal, TOLERANCE);
   DOUBLES_EQUAL(1.2F, stored_cal.vcs_mounting_position.lateral, TOLERANCE);
   DOUBLES_EQUAL(0.8F, stored_cal.vcs_mounting_position.height, TOLERANCE);
   CHECK_EQUAL(sensor_calibration.polarity, stored_cal.polarity);
   CHECK_EQUAL(sensor_calibration.sensor_type, stored_cal.sensor_type);

   // Check all array parameters for all look IDs
   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      DOUBLES_EQUAL(sensor_calibration.fov_min_az_rad[look_id], stored_cal.fov_min_az_rad[look_id], TOLERANCE);
      DOUBLES_EQUAL(sensor_calibration.fov_max_az_rad[look_id], stored_cal.fov_max_az_rad[look_id], TOLERANCE);
      DOUBLES_EQUAL(sensor_calibration.v_wrapping[look_id], stored_cal.v_wrapping[look_id], TOLERANCE);
      // Note: interior_fov, left_fov_normal, and right_fov_normal are calculated from fov angles,
      // not directly copied from input, so they are not checked here
   }
}

/** \purpose
 * Test that calculated FOV parameters are correctly computed and stored
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Calculated_FOV_Parameters)
{
   /** \step{1}
    * Verifies that interior_fov, left_fov_normal, and right_fov_normal are correctly calculated
    * from the input FOV angles.
    */

   /** \precond
    * RSPP is initialized
    */
   RSPP_Initialize();

   /** \action
    * Set calibration with specific FOV angles and retrieve stored calibration
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);

   // Customize sensor 1 with specific FOV angles for testing
   // Using different values for each look ID to verify correct indexing
   ConstantProps_T &sensor_calibration = sensor_array[0].constant;
   sensor_calibration.fov_min_az_rad[RSPP_DET_LOOK_ID_0] = -0.5F;
   sensor_calibration.fov_max_az_rad[RSPP_DET_LOOK_ID_0] = 0.5F;
   sensor_calibration.fov_min_az_rad[RSPP_DET_LOOK_ID_1] = -0.6F;
   sensor_calibration.fov_max_az_rad[RSPP_DET_LOOK_ID_1] = 0.6F;
   sensor_calibration.fov_min_az_rad[RSPP_DET_LOOK_ID_2] = -0.7F;
   sensor_calibration.fov_max_az_rad[RSPP_DET_LOOK_ID_2] = 0.7F;
   sensor_calibration.fov_min_az_rad[RSPP_DET_LOOK_ID_3] = -0.8F;
   sensor_calibration.fov_max_az_rad[RSPP_DET_LOOK_ID_3] = 0.8F;
   sensor_calibration.mounting_position.vcs_boresight_azimuth_angle = 0.0F;

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);
   const RSPP_Sensor_Calib_T &stored_cal = RSPP_State_Get_Sensor_Calibration(0);

   /** \result
    * interior_fov should be calculated correctly:
    * - LOOK_ID_0: min of fov_min_az for LR range (min(-0.5, -0.6) = -0.6)
    * - LOOK_ID_1: max of fov_max_az for LR range (max(0.5, 0.6) = 0.6)
    * - LOOK_ID_2: min of fov_min_az for MR range (min(-0.7, -0.8) = -0.8)
    * - LOOK_ID_3: max of fov_max_az for MR range (max(0.7, 0.8) = 0.8)
    */
   CHECK_EQUAL(RSPP_E_OK, result);

   // Check interior_fov values
   DOUBLES_EQUAL(-0.6F, stored_cal.interior_fov[RSPP_DET_LOOK_ID_0], TOLERANCE);
   DOUBLES_EQUAL(0.6F, stored_cal.interior_fov[RSPP_DET_LOOK_ID_1], TOLERANCE);
   DOUBLES_EQUAL(-0.8F, stored_cal.interior_fov[RSPP_DET_LOOK_ID_2], TOLERANCE);
   DOUBLES_EQUAL(0.8F, stored_cal.interior_fov[RSPP_DET_LOOK_ID_3], TOLERANCE);

   // Check left_fov_normal and right_fov_normal
   // For boresight = 0.0F:
   // LR range: min_vcs_angle = 0.0 + (-0.6) = -0.6, max_vcs_angle = 0.0 + 0.6 = 0.6
   // MR range: min_vcs_angle = 0.0 + (-0.8) = -0.8, max_vcs_angle = 0.0 + 0.8 = 0.8

   // left_fov_normal[0] = -sin(min_vcs_az_angle_lr) = -sin(-0.6)
   // left_fov_normal[1] = cos(min_vcs_az_angle_lr) = cos(-0.6)
   DOUBLES_EQUAL(-sinf(-0.6F), stored_cal.left_fov_normal[RSPP_DET_LOOK_ID_0], TOLERANCE);
   DOUBLES_EQUAL(cosf(-0.6F), stored_cal.left_fov_normal[RSPP_DET_LOOK_ID_1], TOLERANCE);

   // right_fov_normal[0] = sin(max_vcs_az_angle_lr) = sin(0.6)
   // right_fov_normal[1] = -cos(max_vcs_az_angle_lr) = -cos(0.6)
   DOUBLES_EQUAL(sinf(0.6F), stored_cal.right_fov_normal[RSPP_DET_LOOK_ID_0], TOLERANCE);
   DOUBLES_EQUAL(-cosf(0.6F), stored_cal.right_fov_normal[RSPP_DET_LOOK_ID_1], TOLERANCE);

   // left_fov_normal[2] = -sin(min_vcs_az_angle_mr) = -sin(-0.8)
   // left_fov_normal[3] = cos(min_vcs_az_angle_mr) = cos(-0.8)
   DOUBLES_EQUAL(-sinf(-0.8F), stored_cal.left_fov_normal[RSPP_DET_LOOK_ID_2], TOLERANCE);
   DOUBLES_EQUAL(cosf(-0.8F), stored_cal.left_fov_normal[RSPP_DET_LOOK_ID_3], TOLERANCE);

   // right_fov_normal[2] = sin(max_vcs_az_angle_mr) = sin(0.8)
   // right_fov_normal[3] = -cos(max_vcs_az_angle_mr) = -cos(0.8)
   DOUBLES_EQUAL(sinf(0.8F), stored_cal.right_fov_normal[RSPP_DET_LOOK_ID_2], TOLERANCE);
   DOUBLES_EQUAL(-cosf(0.8F), stored_cal.right_fov_normal[RSPP_DET_LOOK_ID_3], TOLERANCE);
}

/** \purpose
 * Test setting calibration for all valid sensor IDs
 * \req
 * CPR-7940_Derived CPR-7950_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_All_Sensor_IDs_Valid)
{
   /** \step{1}
    * Ensures calibration can be set for all valid sensor IDs.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration for all sensors (1 through MAX_NUMBER_OF_SENSORS)
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * All MAX_NUMBER_OF_SENSORS sensors should be calibrated successfully
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

/** \purpose
 * Test that state transitions to INIT_COMPLETE after all sensors calibrated
 * \req
 * CPR-7940_Derived CPR-7950_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_State_Transitions_To_Init_Complete)
{
   /** \step{1}
    * Verifies that state transitions to INIT_COMPLETE after all sensors are calibrated.
    */

   /** \precond
    * RSPP is initialized, state is INITIALIZED
    */
   RSPP_States_Type_T initial_state = RSPP_State_Get_Current_State();
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, initial_state);

   /** \action
    * Calibrate all sensors
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   RSPP_Set_Sensor_Calibrations(sensor_array);

   RSPP_States_Type_T final_state = RSPP_State_Get_Current_State();

   /** \result
    * State should transition to INIT_COMPLETE
    */
   CHECK_EQUAL(RSPP_STATE_INIT_COMPLETE, final_state);
}

// ==============================================================================
// Initialization State Tests
// ==============================================================================

/** \purpose
 * Test that calling RSPP_Set_Sensor_Calibration before RSPP_Initialize fails
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Called_Before_Initialize_Invalid)
{
   /** \step{1}
    * Ensures that calling RSPP_Set_Sensor_Calibration before RSPP_Initialize is rejected.
    */

   /** \precond
    * Reset RSPP_State to uninitialized
    */
   RSPP_State_Reset();

   /** \action
    * Attempt to set sensor calibration without prior initialization in this test
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should return RSPP_E_NOT_INITIALIZED
    */
   CHECK(result == RSPP_E_NOT_INITIALIZED);
}

/** \purpose
 * Test that partial calibration does not transition to INIT_COMPLETE
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Partial_Calibration_Stays_Initialized)
{
   /** \step{1}
    * Checks that partial calibration does not transition state to INIT_COMPLETE.
    */

   /** \precond
    * RSPP is initialized, state is INITIALIZED
    */
   RSPP_States_Type_T initial_state = RSPP_State_Get_Current_State();
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, initial_state);

   /** \action
    * Set up sensor array with invalid sensor ID in the middle to cause partial calibration failure
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);

   // Invalidate sensor 5 by setting its ID to 0 (invalid)
   sensor_array[4].constant.id = INVALID_SENSOR_ID_ZERO;

   RSPP_Set_Sensor_Calibrations(sensor_array);

   RSPP_States_Type_T partial_state = RSPP_State_Get_Current_State();

   /** \result
    * State should remain INITIALIZED (not INIT_COMPLETE)
    */
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, partial_state);
}

// ==============================================================================
// Invalid Sensor ID Tests
// ==============================================================================

/** \purpose
 * Test that sensor ID 0 is rejected
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Sensor_ID_Zero_Invalid)
{
   /** \step{1}
    * Ensures that sensor ID 0 is rejected as invalid.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Try to set calibration with sensor ID 0 on first sensor
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);

   // Set first sensor to have invalid ID 0
   sensor_array[0].constant.id = INVALID_SENSOR_ID_ZERO;

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should return RSPP_E_INVALID_SENSOR_ID (sensor ID validation fails first)
    */
   CHECK_EQUAL(RSPP_E_INVALID_SENSOR_ID, result);
}

/** \purpose
 * Test that sensor ID above MAX_NUMBER_OF_SENSORS is rejected
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Sensor_ID_Above_Max_Invalid)
{
   /** \step{1}
    * Ensures that sensor IDs above the maximum are rejected as invalid.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Try to set calibration with sensor ID > MAX_NUMBER_OF_SENSORS
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);

   // Set first sensor to have invalid ID above max
   sensor_array[0].constant.id = INVALID_SENSOR_ID_ABOVE_MAX;

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should return RSPP_E_INVALID_SENSOR_ID
    */
   CHECK_EQUAL(RSPP_E_INVALID_SENSOR_ID, result);
}

/** \purpose
 * Test that sensor ID must match array position (ID mismatch rejected)
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_ID_Array_Position_Mismatch_Rejected)
{
   /** \step{1}
    * Checks that sensor ID must match its expected position in array (id = index + 1).
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set up array where sensor at index 1 has ID 5 instead of ID 2 (mismatch)
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);

   // Set sensor at index 1 to have ID 5 instead of 2 (wrong position)
   sensor_array[1].constant.id = 5U;

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should fail with INVALID_SENSOR_ID when ID doesn't match array position
    */
   CHECK_EQUAL(RSPP_E_INVALID_SENSOR_ID, result);
}

/** \purpose
 * Test that extreme sensor ID (255) is rejected
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Sensor_ID_Extreme_255_Invalid)
{
   /** \step{1}
    * Ensures that extreme sensor ID values (e.g., 255) are rejected.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Try to set calibration with sensor ID 255 (extreme value)
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);

   // Set first sensor to have extreme invalid ID 255
   sensor_array[0].constant.id = 255U;

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should return RSPP_E_INVALID_SENSOR_ID
    */
   CHECK_EQUAL(RSPP_E_INVALID_SENSOR_ID, result);
}

// ==============================================================================
// Invalid Polarity Tests
// ==============================================================================

/** \purpose
 * Test that polarity 0 is rejected
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Polarity_Zero_Invalid)
{
   /** \step{1}
    * Verifies that polarity value 0 is rejected as invalid.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with polarity = 0
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.polarity = INVALID_POLARITY;

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should return RSPP_E_INVALID_CALIBRATION
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
}

/** \purpose
 * Test that polarity +1 is valid
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Polarity_Positive_One_Valid)
{
   /** \step{1}
    * Checks that polarity +1 is accepted as valid.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with polarity = +1
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.polarity = VALID_POLARITY_POSITIVE;

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should return RSPP_E_OK
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

/** \purpose
 * Test that polarity -1 is valid
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Polarity_Negative_One_Valid)
{
   /** \step{1}
    * Checks that polarity -1 is accepted as valid.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with polarity = -1
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.polarity = VALID_POLARITY_NEGATIVE;

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should return RSPP_E_OK
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

/** \purpose
 * Test that polarity +2 is rejected
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Polarity_Plus_Two_Invalid)
{
   /** \step{1}
    * Ensures that polarity +2 is rejected as invalid.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with polarity = +2
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.polarity = 2;

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should return RSPP_E_INVALID_CALIBRATION
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
}

// ==============================================================================
// Mounting Position Validation Tests
// ==============================================================================

/** \purpose
 * Test longitudinal position at minimum valid boundary
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Longitudinal_At_Min_Boundary_Valid)
{
   /** \step{1}
    * Verifies that minimum valid longitudinal position is accepted.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with longitudinal = -10.0
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.mounting_position.vcs_position.longitudinal = VALID_LONG_MIN;

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should return RSPP_E_OK
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

/** \purpose
 * Test longitudinal position at maximum valid boundary
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Longitudinal_At_Max_Boundary_Valid)
{
   /** \step{1}
    * Verifies that maximum valid longitudinal position is accepted.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with longitudinal = +10.0
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.mounting_position.vcs_position.longitudinal = VALID_LONG_MAX;

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should return RSPP_E_OK
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

/** \purpose
 * Test longitudinal position below minimum boundary
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Longitudinal_Below_Min_Invalid)
{
   /** \step{1}
    * Ensures that longitudinal position below minimum is rejected.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with longitudinal = -1.1 (below min of -1.0)
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.mounting_position.vcs_position.longitudinal = VALID_LONG_MIN - 0.1F;

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should return RSPP_E_INVALID_CALIBRATION
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
}

/** \purpose
 * Test longitudinal position above maximum boundary
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Longitudinal_Above_Max_Invalid)
{
   /** \step{1}
    * Ensures that longitudinal position above maximum is rejected.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with longitudinal = +10.1
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.mounting_position.vcs_position.longitudinal = VALID_LONG_MAX + 0.1F;

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should return RSPP_E_INVALID_CALIBRATION
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
}

/** \purpose
 * Test lateral position at boundaries
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Lateral_At_Boundaries_Valid)
{
   /** \step{1}
    * Verifies that lateral positions at min and max boundaries are accepted.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with lateral at min and max boundaries
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.mounting_position.vcs_position.lateral = VALID_LAT_MIN;
   sensor_array[1].constant.mounting_position.vcs_position.lateral = VALID_LAT_MAX;

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Both should return RSPP_E_OK
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

/** \purpose
 * Test lateral position out of bounds
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Lateral_Out_Of_Bounds_Invalid)
{
   /** \step{1}
    * Ensures that lateral positions out of bounds are rejected.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with lateral below min
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.mounting_position.vcs_position.lateral = VALID_LAT_MIN - 0.1F;
   RSPP_Return_Type_T result_below = RSPP_Set_Sensor_Calibrations(sensor_array);

   // Reset and test above max
   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.mounting_position.vcs_position.lateral = VALID_LAT_MAX + 0.1F;
   RSPP_Return_Type_T result_above = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Both should return RSPP_E_INVALID_CALIBRATION
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result_below);
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result_above);
}

/** \purpose
 * Test height position at boundaries
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Height_At_Boundaries_Valid)
{
   /** \step{1}
    * Verifies that height at min and max boundaries is accepted.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with height at min and max boundaries
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.mounting_position.vcs_position.height = VALID_HEIGHT_MIN;
   sensor_array[1].constant.mounting_position.vcs_position.height = VALID_HEIGHT_MAX;

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Both should return RSPP_E_OK
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

/** \purpose
 * Test height position out of bounds
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Height_Out_Of_Bounds_Invalid)
{
   /** \step{1}
    * Ensures that height values out of bounds are rejected.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with height out of bounds
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.mounting_position.vcs_position.height = VALID_HEIGHT_MIN - 0.1F;
   RSPP_Return_Type_T result_below = RSPP_Set_Sensor_Calibrations(sensor_array);

   // Reset and test above max
   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.mounting_position.vcs_position.height = VALID_HEIGHT_MAX + 0.1F;
   RSPP_Return_Type_T result_above = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Both should return RSPP_E_INVALID_CALIBRATION
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result_below);
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result_above);
}

// ==============================================================================
// FOV Validation Tests
// ==============================================================================

/** \purpose
 * Test FOV azimuth min at boundary
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_FOV_Azimuth_Min_At_Boundary_Valid)
{
   /** \step{1}
    * Verifies that FOV azimuth min at boundary is accepted.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with fov_min_az_rad at -PI boundary
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      sensor_array[0].constant.fov_min_az_rad[look_id] = VALID_FOV_MIN_AZ;
   }

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should return RSPP_E_OK
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

/** \purpose
 * Test FOV azimuth max at boundary
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_FOV_Azimuth_Max_At_Boundary_Valid)
{
   /** \step{1}
    * Verifies that FOV azimuth max at boundary is accepted.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with fov_max_az_rad at +PI boundary
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      sensor_array[0].constant.fov_max_az_rad[look_id] = VALID_FOV_MAX_AZ;
   }

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should return RSPP_E_OK
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

/** \purpose
 * Test FOV azimuth min/max reversed (min >= max)
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_FOV_Azimuth_Min_Max_Reversed_Invalid)
{
   /** \step{1}
    * Ensures that FOV azimuth min >= max is rejected as invalid.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with fov_min_az >= fov_max_az
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.fov_min_az_rad[0] = 0.5F;
   sensor_array[0].constant.fov_max_az_rad[0] = 0.5F; // Equal (invalid)

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should return RSPP_E_INVALID_CALIBRATION
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
}

/** \purpose
 * Test FOV elevation min/max reversed
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_FOV_Elevation_Min_Max_Reversed_Invalid)
{
   /** \step{1}
    * Ensures that FOV elevation min >= max is rejected as invalid.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with fov_min_el >= fov_max_el
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.fov_min_el_rad[0] = 0.3F;
   sensor_array[0].constant.fov_max_el_rad[0] = 0.2F; // min > max (invalid)

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should return RSPP_E_INVALID_CALIBRATION
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
}

/** \purpose
 * Test FOV elevation min at boundary
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_FOV_Elevation_Min_At_Boundary_Valid)
{
   /** \step{1}
    * Verifies that FOV elevation min at boundary is accepted.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with fov_min_el_rad at -PI boundary
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      sensor_array[0].constant.fov_min_el_rad[look_id] = VALID_FOV_MIN_EL;
      sensor_array[0].constant.fov_max_el_rad[look_id] = VALID_FOV_MAX_EL;
   }

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should return RSPP_E_OK
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

/** \purpose
 * Test FOV elevation max at boundary
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_FOV_Elevation_Max_At_Boundary_Valid)
{
   /** \step{1}
    * Verifies that FOV elevation max at boundary is accepted.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with fov_max_el_rad at +PI boundary
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      sensor_array[0].constant.fov_min_el_rad[look_id] = VALID_FOV_MIN_EL;
      sensor_array[0].constant.fov_max_el_rad[look_id] = VALID_FOV_MAX_EL;
   }

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should return RSPP_E_OK
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

// ==============================================================================
// Wrapping Parameter Validation Tests
// ==============================================================================

/** \purpose
 * Test v_wrapping at valid boundaries
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_V_Wrapping_At_Boundaries_Valid)
{
   /** \step{1}
    * Verifies that v_wrapping at min and max boundaries is accepted.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with v_wrapping at min and max boundaries
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      sensor_array[0].constant.v_wrapping[look_id] = VALID_V_WRAP_MIN;
   }
   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      sensor_array[1].constant.v_wrapping[look_id] = VALID_V_WRAP_MAX;
   }
   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Both should return RSPP_E_OK
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

/** \purpose
 * Test v_wrapping values just out of bounds
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_V_Wrapping_Just_Out_Of_Bounds_Invalid)
{
   /** \step{1}
    * Ensures that v_wrapping values out of bounds are rejected.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with v_wrapping out of bounds
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.v_wrapping[0] = VALID_V_WRAP_MIN - 0.1F;
   RSPP_Return_Type_T result_below = RSPP_Set_Sensor_Calibrations(sensor_array);

   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.v_wrapping[0] = VALID_V_WRAP_MAX + 0.1F;
   RSPP_Return_Type_T result_above = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Both should return RSPP_E_INVALID_CALIBRATION
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result_below);
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result_above);
}

// ==============================================================================
// Range Limits Validation Tests
// ==============================================================================

/** \purpose
 * Test range_limits at valid boundaries
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Range_Limits_At_Boundaries_Valid)
{
   /** \step{1}
    * Verifies that range_limits at min and max boundaries are accepted.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with range_limits at min and max boundaries
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      sensor_array[0].constant.range_limits[look_id] = VALID_RANGE_LIMIT_MIN;
   }
   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      sensor_array[1].constant.range_limits[look_id] = VALID_RANGE_LIMIT_MAX;
   }
   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Both should return RSPP_E_OK
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

/** \purpose
 * Test range_limits just out of bounds
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Range_Limits_Just_Out_Of_Bounds_Invalid)
{
   /** \step{1}
    * Ensures that range_limits values out of bounds are rejected.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with range_limits out of bounds
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.range_limits[0] = VALID_RANGE_LIMIT_MIN - 0.1F;
   RSPP_Return_Type_T result_below = RSPP_Set_Sensor_Calibrations(sensor_array);

   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.range_limits[0] = VALID_RANGE_LIMIT_MAX + 0.1F;
   RSPP_Return_Type_T result_above = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Both should return RSPP_E_INVALID_CALIBRATION
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result_below);
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result_above);
}

// ==============================================================================
// Min Aliased Range Rate Validation Tests
// ==============================================================================

/** \purpose
 * Test min_aliased_range_rate at valid boundaries
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Min_Aliased_RR_At_Boundaries_Valid)
{
   /** \step{1}
    * Verifies that min_aliased_range_rate at min and max boundaries is accepted.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with min_aliaised_range_rate at min and max boundaries
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      sensor_array[0].constant.min_aliaised_range_rate[look_id] = VALID_MIN_ALIASED_RR_MIN;
   }
   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      sensor_array[1].constant.min_aliaised_range_rate[look_id] = VALID_MIN_ALIASED_RR_MAX;
   }
   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Both should return RSPP_E_OK
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

/** \purpose
 * Test min_aliased_range_rate just out of bounds
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Min_Aliased_RR_Just_Out_Of_Bounds_Invalid)
{
   /** \step{1}
    * Ensures that min_aliased_range_rate values out of bounds are rejected.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with min_aliaised_range_rate out of bounds
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.min_aliaised_range_rate[0] = VALID_MIN_ALIASED_RR_MIN - 0.1F;
   RSPP_Return_Type_T result_below = RSPP_Set_Sensor_Calibrations(sensor_array);

   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.min_aliaised_range_rate[0] = VALID_MIN_ALIASED_RR_MAX + 0.1F;
   RSPP_Return_Type_T result_above = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Both should return RSPP_E_INVALID_CALIBRATION
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result_below);
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result_above);
}

// ==============================================================================
// Data Persistence Tests
// ==============================================================================

/** \purpose
 * Test that all look IDs are stored correctly
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_All_Look_IDs_Stored)
{
   /** \step{1}
    * Checks that all look ID values are stored correctly in calibration.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with different values for each look ID
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);

   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      sensor_array[0].constant.v_wrapping[look_id] = 20.0F + (float32_t)look_id * 5.0F;
      sensor_array[0].constant.fov_min_az_rad[look_id] = -1.0F + (float32_t)look_id * 0.1F;
      sensor_array[0].constant.fov_max_az_rad[look_id] = 1.0F - (float32_t)look_id * 0.1F;
   }

   RSPP_Set_Sensor_Calibrations(sensor_array);
   const RSPP_Sensor_Calib_T &stored_cal = RSPP_State_Get_Sensor_Calibration(0);

   /** \result
    * All look ID values should be stored correctly
    */
   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      DOUBLES_EQUAL(sensor_array[0].constant.v_wrapping[look_id], stored_cal.v_wrapping[look_id], TOLERANCE);
      DOUBLES_EQUAL(sensor_array[0].constant.fov_min_az_rad[look_id], stored_cal.fov_min_az_rad[look_id], TOLERANCE);
      DOUBLES_EQUAL(sensor_array[0].constant.fov_max_az_rad[look_id], stored_cal.fov_max_az_rad[look_id], TOLERANCE);
   }
}

/** \purpose
 * Test that all 10 sensors are calibrated with unique values in single batch call
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_All_Sensors_Batch_Calibrated)
{
   /** \step{1}
    * Verifies that all 10 sensors are calibrated with different values in one API call.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set unique calibrations for all sensors in single call
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);

   // Give each sensor unique longitudinal position and alternating polarity
   for (uint8_t i = 0; i < MAX_NUMBER_OF_SENSORS; ++i)
   {
      sensor_array[i].constant.mounting_position.vcs_position.longitudinal = -0.9F + (static_cast<float32_t>(i) * 0.18F);
      sensor_array[i].constant.polarity = (i % 2 == 0) ? 1 : -1;
   }

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * All sensors calibrated successfully with their unique values
    */
   CHECK_EQUAL(RSPP_E_OK, result);

   // Verify first, middle, and last sensors have correct unique values
   const RSPP_Sensor_Calib_T &stored_cal_0 = RSPP_State_Get_Sensor_Calibration(0);
   const RSPP_Sensor_Calib_T &stored_cal_4 = RSPP_State_Get_Sensor_Calibration(4);
   const RSPP_Sensor_Calib_T &stored_cal_9 = RSPP_State_Get_Sensor_Calibration(9);

   DOUBLES_EQUAL(-0.9F, stored_cal_0.vcs_mounting_position.longitudinal, TOLERANCE);
   CHECK_EQUAL(1, stored_cal_0.polarity);
   DOUBLES_EQUAL(-0.18F, stored_cal_4.vcs_mounting_position.longitudinal, TOLERANCE);
   CHECK_EQUAL(1, stored_cal_4.polarity);
   DOUBLES_EQUAL(0.72F, stored_cal_9.vcs_mounting_position.longitudinal, TOLERANCE);
   CHECK_EQUAL(-1, stored_cal_9.polarity);
}

/** \purpose
 * Test that boresight azimuth angle is accepted correctly (used to derive FOV boundaries)
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Boresight_Azimuth_Accepted)
{
   /** \step{1}
    * Checks that boresight azimuth angle is accepted and used.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with specific boresight azimuth angle (used to compute FOV boundaries)
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.mounting_position.vcs_boresight_azimuth_angle = 0.785F; // ~45 degrees

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Function should accept the boresight azimuth angle
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

/** \purpose
 * Test that range_limits and min_aliased_range_rate are accepted for all look IDs
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Range_And_Aliased_RR_Accepted)
{
   /** \step{1}
    * Verifies that range_limits and min_aliased_range_rate are accepted for all look IDs.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with different range_limits and min_aliased_range_rate for each look ID
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);

   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      sensor_array[0].constant.range_limits[look_id] = 100.0F + (float32_t)look_id * 50.0F;
      sensor_array[0].constant.min_aliaised_range_rate[look_id] = -50.0F - (float32_t)look_id * 10.0F;
   }

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Function should accept all look ID values
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

// ==============================================================================
// Edge Case Tests
// ==============================================================================

/** \purpose
 * Test calibration with extreme valid values
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Extreme_Valid_Values)
{
   /** \step{1}
    * Checks calibration with all values at extreme but valid boundaries.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with all values at extreme but valid boundaries
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.mounting_position.vcs_position.longitudinal = VALID_LONG_MAX;
   sensor_array[0].constant.mounting_position.vcs_position.lateral = VALID_LAT_MAX;
   sensor_array[0].constant.mounting_position.vcs_position.height = VALID_HEIGHT_MAX;
   sensor_array[0].constant.polarity = VALID_POLARITY_NEGATIVE;

   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      sensor_array[0].constant.v_wrapping[look_id] = VALID_V_WRAP_MAX;
      sensor_array[0].constant.r_wrapping[look_id] = VALID_R_WRAP_MAX;
      sensor_array[0].constant.range_limits[look_id] = VALID_RANGE_LIMIT_MAX;
      sensor_array[0].constant.min_aliaised_range_rate[look_id] = VALID_MIN_ALIASED_RR_MAX;
   }

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should return RSPP_E_OK
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

/** \purpose
 * Test sensor type is stored correctly
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Sensor_Type_Stored)
{
   /** \step{1}
    * Verifies that the sensor type is stored correctly in calibration.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set calibration with specific sensor type
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);
   sensor_array[0].constant.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;

   RSPP_Set_Sensor_Calibrations(sensor_array);
   const RSPP_Sensor_Calib_T &stored_cal = RSPP_State_Get_Sensor_Calibration(0);

   /** \result
    * Sensor type should match
    */
   CHECK_EQUAL(RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR, stored_cal.sensor_type);
};

/** \purpose
 * Test that validation failure on one sensor prevents calibration of all sensors (atomicity)
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Validation_Failure_Prevents_All_Calibration)
{
   /** \step{1}
    * Checks that if one sensor has invalid data, no sensors are calibrated (atomic operation).
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Set sensor 5 with invalid polarity while others are valid
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);

   // Make sensor 5 (index 4) have invalid polarity
   sensor_array[4].constant.polarity = INVALID_POLARITY;

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * API should fail and state should remain INITIALIZED (no sensors calibrated)
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);

   RSPP_States_Type_T state = RSPP_State_Get_Current_State();
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state);
}

/** \purpose
 * Test that calling RSPP_Set_Sensor_Calibrations twice is rejected
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_Second_Call_Rejected_After_Success)
{
   /** \step{1}
    * Checks that once all sensors are calibrated, calling the API again is rejected.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Call RSPP_Set_Sensor_Calibrations successfully, then call it again
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   create_valid_sensor_array(sensor_array);

   RSPP_Return_Type_T first_result = RSPP_Set_Sensor_Calibrations(sensor_array);
   CHECK_EQUAL(RSPP_E_OK, first_result);

   // Try to calibrate again
   RSPP_Return_Type_T second_result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Second call should be rejected since sensors are already calibrated
    */
   CHECK_EQUAL(RSPP_E_INVALID_SENSOR_ID, second_result);
}

/** \purpose
 * Test that array with all sensor IDs set to zero is rejected
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Set_Sensor_Calibrations, Set_Sensor_Calibrations_TC_All_Sensors_Invalid_ID_Zero)
{
   /** \step{1}
    * Checks that completely uninitialized array (all IDs = 0) is rejected.
    */

   /** \precond
    * RSPP is initialized
    */

   /** \action
    * Create array with all sensor IDs set to 0 (invalid)
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));

   RSPP_Return_Type_T result = RSPP_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should fail with INVALID_SENSOR_ID on first sensor
    */
   CHECK_EQUAL(RSPP_E_INVALID_SENSOR_ID, result);

   RSPP_States_Type_T state = RSPP_State_Get_Current_State();
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state);
}

/** @}*/

/** \defgroup  test_RSPP_Calculate_Refined_Sensor_Data
 *  @{
 */

/** \brief
 * Tests for RSPP_Calculate_Refined_Sensor_Data function
 */
TEST_GROUP(test_RSPP_Calculate_Refined_Sensor_Data)
{
   static constexpr float32_t TOLERANCE = 0.0001F;
   uint64_t current_time_us;
   RSPP_Sensor_Calib_T rspp_sensor_calibration;
   VariableProps_T sensor_data;
   RefinedProps_T refined_sensor_data;

   /** \setup */
   TEST_SETUP()
   {
      current_time_us = 0U;
      memset(&rspp_sensor_calibration, 0, sizeof(RSPP_Sensor_Calib_T));
      memset(&sensor_data, 0, sizeof(VariableProps_T));
      memset(&refined_sensor_data, 0, sizeof(RefinedProps_T));

      sensor_data.is_valid = true;
      sensor_data.timestamp_us = 0U;

      for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
      {
         rspp_sensor_calibration.interior_fov[look_id] = 0.1F + static_cast<float32_t>(look_id);
         rspp_sensor_calibration.left_fov_normal[look_id] = -0.2F - static_cast<float32_t>(look_id);
         rspp_sensor_calibration.right_fov_normal[look_id] = 0.3F + static_cast<float32_t>(look_id);
      }
   }

   /** \teardown */
   TEST_TEARDOWN()
   {
      RSPP_State_Reset();
   }
};

/** \purpose
 * Test refined data is updated for valid sensor
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Calculate_Refined_Sensor_Data, Calculate_Refined_Sensor_Data_TC_Valid_Sensor_Updates_All_Fields)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Valid sensor data with known timestamp
    */
   current_time_us = 2000000U;          // 2.0 s
   sensor_data.timestamp_us = 1500000U; // 1.5 s
   sensor_data.is_valid = true;

   /** \action
    * Calculate refined sensor data
    */
   RSPP_Calculate_Refined_Sensor_Data(current_time_us, rspp_sensor_calibration, sensor_data, refined_sensor_data);

   /** \result
    * Time delta and FOV arrays should be updated
    */
   DOUBLES_EQUAL(0.5F, refined_sensor_data.time_since_measurement_s, TOLERANCE);

   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      DOUBLES_EQUAL(rspp_sensor_calibration.interior_fov[look_id],
                    refined_sensor_data.interior_fov[look_id], TOLERANCE);
      DOUBLES_EQUAL(rspp_sensor_calibration.left_fov_normal[look_id],
                    refined_sensor_data.left_fov_normal[look_id], TOLERANCE);
      DOUBLES_EQUAL(rspp_sensor_calibration.right_fov_normal[look_id],
                    refined_sensor_data.right_fov_normal[look_id], TOLERANCE);
   }
}

/** \purpose
 * Test refined data is not updated for invalid sensor
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Calculate_Refined_Sensor_Data, Calculate_Refined_Sensor_Data_TC_Invalid_Sensor_Does_Not_Update)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Invalid sensor with pre-filled refined data
    */
   sensor_data.is_valid = false;
   current_time_us = 2000000U;
   sensor_data.timestamp_us = 1000000U;

   refined_sensor_data.time_since_measurement_s = 9.9F;
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      refined_sensor_data.interior_fov[look_id] = 7.0F;
      refined_sensor_data.left_fov_normal[look_id] = 8.0F;
      refined_sensor_data.right_fov_normal[look_id] = 9.0F;
   }

   /** \action
    * Calculate refined sensor data
    */
   RSPP_Calculate_Refined_Sensor_Data(current_time_us, rspp_sensor_calibration, sensor_data, refined_sensor_data);

   /** \result
    * Refined data should remain unchanged
    */
   DOUBLES_EQUAL(9.9F, refined_sensor_data.time_since_measurement_s, TOLERANCE);
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      DOUBLES_EQUAL(7.0F, refined_sensor_data.interior_fov[look_id], TOLERANCE);
      DOUBLES_EQUAL(8.0F, refined_sensor_data.left_fov_normal[look_id], TOLERANCE);
      DOUBLES_EQUAL(9.0F, refined_sensor_data.right_fov_normal[look_id], TOLERANCE);
   }
}

/** \purpose
 * Test refined data time delta with zero difference
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Calculate_Refined_Sensor_Data, Calculate_Refined_Sensor_Data_TC_Zero_Time_Delta)
{
   /** \step{1}
    * Execute test
    */

   /** \precond
    * Current time equals timestamp
    */
   current_time_us = 123456U;
   sensor_data.timestamp_us = 123456U;
   sensor_data.is_valid = true;

   /** \action
    * Calculate refined sensor data
    */
   RSPP_Calculate_Refined_Sensor_Data(current_time_us, rspp_sensor_calibration, sensor_data, refined_sensor_data);

   /** \result
    * Time since measurement should be zero
    */
   DOUBLES_EQUAL(0.0F, refined_sensor_data.time_since_measurement_s, TOLERANCE);
}

/** \purpose
 * Test behavior when timestamp is in the future
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Calculate_Refined_Sensor_Data, Calculate_Refined_Sensor_Data_TC_Timestamp_In_Future)
{
   /** \step{1}
    * Execute test with timestamp greater than current time
    */

   /** \precond
    * Timestamp is in the future (underflow condition)
    */
   current_time_us = 1000000U;
   sensor_data.timestamp_us = 2000000U;
   sensor_data.is_valid = true;

   /** \action
    * Calculate refined sensor data
    */
   RSPP_Calculate_Refined_Sensor_Data(current_time_us, rspp_sensor_calibration, sensor_data, refined_sensor_data);

   /** \result
    * This is currently not handled. The time since measurement will have a large positive value due to underflow.
    */
   DOUBLES_EQUAL(UINT64_MAX / 1e6F + 1.0F, refined_sensor_data.time_since_measurement_s, TOLERANCE);
}

/** \purpose
 * Test large time delta conversion accuracy
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Calculate_Refined_Sensor_Data, Calculate_Refined_Sensor_Data_TC_Large_Time_Delta)
{
   /** \step{1}
    * Execute test with large time difference (1 hour)
    */

   /** \precond
    * Time difference is 1 hour (3.6 billion microseconds)
    */
   current_time_us = 3600000000ULL; // 1 hour in microseconds
   sensor_data.timestamp_us = 0U;
   sensor_data.is_valid = true;

   /** \action
    * Calculate refined sensor data
    */
   RSPP_Calculate_Refined_Sensor_Data(current_time_us, rspp_sensor_calibration, sensor_data, refined_sensor_data);

   /** \result
    * Time should be 3600 seconds (1 hour) with acceptable float precision
    */
   DOUBLES_EQUAL(3600.0F, refined_sensor_data.time_since_measurement_s, TOLERANCE);
}

/** \purpose
 * Test microsecond precision in time conversion
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Calculate_Refined_Sensor_Data, Calculate_Refined_Sensor_Data_TC_Microsecond_Precision)
{
   /** \step{1}
    * Execute test with sub-millisecond time difference
    */

   /** \precond
    * Time difference is 500 microseconds (0.0005 seconds)
    */
   current_time_us = 1000500U;
   sensor_data.timestamp_us = 1000000U;
   sensor_data.is_valid = true;

   /** \action
    * Calculate refined sensor data
    */
   RSPP_Calculate_Refined_Sensor_Data(current_time_us, rspp_sensor_calibration, sensor_data, refined_sensor_data);

   /** \result
    * Time should be 0.0005 seconds with high precision
    */
   DOUBLES_EQUAL(0.0005F, refined_sensor_data.time_since_measurement_s, TOLERANCE);
}

/** \purpose
 * Test FOV arrays are copied correctly for all look IDs
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Calculate_Refined_Sensor_Data, Calculate_Refined_Sensor_Data_TC_All_Look_IDs_Updated)
{
   /** \step{1}
    * Verify each look_id index gets its corresponding FOV values
    */

   /** \precond
    * Calibration has distinct values for each look_id
    */
   current_time_us = 1000000U;
   sensor_data.timestamp_us = 900000U;
   sensor_data.is_valid = true;

   // Set distinct values per look_id in setup (already done in TEST_SETUP)
   // Setup values: interior_fov[i] = 0.1F + i, left_fov_normal[i] = -0.2F - i, right_fov_normal[i] = 0.3F + i

   /** \action
    * Calculate refined sensor data
    */
   RSPP_Calculate_Refined_Sensor_Data(current_time_us, rspp_sensor_calibration, sensor_data, refined_sensor_data);

   /** \result
    * Each look_id should have its specific FOV values copied
    */
   for (uint8_t look_id = 0U; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      float32_t expected_interior = 0.1F + static_cast<float32_t>(look_id);
      float32_t expected_left = -0.2F - static_cast<float32_t>(look_id);
      float32_t expected_right = 0.3F + static_cast<float32_t>(look_id);

      DOUBLES_EQUAL(expected_interior, refined_sensor_data.interior_fov[look_id], TOLERANCE);
      DOUBLES_EQUAL(expected_left, refined_sensor_data.left_fov_normal[look_id], TOLERANCE);
      DOUBLES_EQUAL(expected_right, refined_sensor_data.right_fov_normal[look_id], TOLERANCE);
   }
}

/** \purpose
 * Test typical radar update cycle timing
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Calculate_Refined_Sensor_Data, Calculate_Refined_Sensor_Data_TC_Typical_Radar_Cycle_Time)
{
   /** \step{1}
    * Test with typical radar cycle time (50ms)
    */

   /** \precond
    * Time difference represents typical radar update cycle
    */
   current_time_us = 1050000U;          // 1.05 seconds
   sensor_data.timestamp_us = 1000000U; // 1.00 seconds
   sensor_data.is_valid = true;

   /** \action
    * Calculate refined sensor data
    */
   RSPP_Calculate_Refined_Sensor_Data(current_time_us, rspp_sensor_calibration, sensor_data, refined_sensor_data);

   /** \result
    * Time should be 50ms (0.05 seconds)
    */
   DOUBLES_EQUAL(0.05F, refined_sensor_data.time_since_measurement_s, TOLERANCE);
}

/** @}*/

/** \defgroup  test_RSPP_Process_Detections
 *  @{
 */

/** \brief
 * Tests for RSPP_Process_Detections function
 *
 * \details
 * Function under test: RSPP_Return_Type_T RSPP_Process_Detections(
 *     F360_Radar_Sensor_T (&dynamic_sensor_data)[MAX_NUMBER_OF_SENSORS],
 *     RSPP_Detection_List_T &detection_list,
 *     const RSPP_Host_T &vehicle_state_data,
 *     const uint64_t current_time_us)
 *
 * Processes radar detections for object tracking.
 * Main processing function that applies coordinate transformation,
 * range rate compensation, and motion status classification.
 * Each detection contains sensor_id field used to index into
 * sensor data array (sensor_id - 1 = array index).
 */
TEST_GROUP(test_RSPP_Process_Detections)
{
   // Named constants for test clarity
   static constexpr float32_t TOLERANCE = 0.0001F;
   static constexpr float32_t VCS_COORD_TOLERANCE = 0.1F; // Relaxed tolerance for VCS coordinate calculations
   static constexpr uint8_t VALID_SENSOR_ID_MIN = 1U;
   static constexpr uint8_t VALID_SENSOR_ID_MAX = MAX_NUMBER_OF_SENSORS;
   static constexpr uint64_t TYPICAL_TIME_US = 1000000ULL; // 1 second

   // Typical valid values for sensor calibration
   static constexpr float32_t TYPICAL_LONGITUDINAL = 0.8F; // Front bumper (must be in [-1.0, 1.0])
   static constexpr float32_t TYPICAL_LATERAL = 0.0F;      // Center
   static constexpr float32_t TYPICAL_HEIGHT = 0.5F;       // 0.5m above ground
   static constexpr float32_t TYPICAL_BORESIGHT_AZ = 0.0F; // Forward facing
   static constexpr float32_t TYPICAL_BORESIGHT_EL = 0.0F; // Forward facing
   static constexpr float32_t TYPICAL_FOV_MIN_AZ = -1.0F;
   static constexpr float32_t TYPICAL_FOV_MAX_AZ = 1.0F;
   static constexpr float32_t TYPICAL_FOV_MIN_EL = -0.5F;
   static constexpr float32_t TYPICAL_FOV_MAX_EL = 0.5F;
   static constexpr float32_t TYPICAL_R_WRAPPING = 1.0F;
   static constexpr float32_t TYPICAL_V_WRAPPING = 30.0F;
   static constexpr float32_t TYPICAL_RANGE_LIMIT = 200.0F;
   static constexpr float32_t TYPICAL_MIN_ALIASED_RR = -10.0F;
   static constexpr int32_t VALID_POLARITY_POSITIVE = 1;

   // Typical detection values
   static constexpr float32_t TYPICAL_RANGE = 50.0F;
   static constexpr float32_t TYPICAL_RANGE_RATE = -10.0F;
   static constexpr float32_t TYPICAL_AZIMUTH = 0.0F;
   static constexpr float32_t TYPICAL_ELEVATION = 0.0F;
   static constexpr float32_t TYPICAL_SNR = 20.0F;
   static constexpr float32_t TYPICAL_RCS = 10.0F;

   // Host vehicle values
   static constexpr float32_t TYPICAL_HOST_SPEED = 20.0F;
   static constexpr float32_t TYPICAL_HOST_YAW_RATE = 0.0F;

   // Test data storage
   F360_Radar_Sensor_T m_dynamic_sensor_data[MAX_NUMBER_OF_SENSORS];
   RSPP_Detection_List_T m_detection_list;
   RSPP_Host_T m_vehicle_state_data;
   RSPP_Core_Info_T m_core_info;

   /**
    * Helper function to initialize RSPP and calibrate all sensors
    */
   void setup_rspp_fully_initialized()
   {
      RSPP_Initialize();

      ConstantProps_T sensor_calibration;
      memset(&sensor_calibration, 0, sizeof(sensor_calibration));
      sensor_calibration.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      sensor_calibration.polarity = VALID_POLARITY_POSITIVE;
      sensor_calibration.mounting_position.vcs_position.longitudinal = TYPICAL_LONGITUDINAL;
      sensor_calibration.mounting_position.vcs_position.lateral = TYPICAL_LATERAL;
      sensor_calibration.mounting_position.vcs_position.height = TYPICAL_HEIGHT;
      sensor_calibration.mounting_position.vcs_boresight_azimuth_angle = TYPICAL_BORESIGHT_AZ;
      sensor_calibration.mounting_position.vcs_boresight_elevation_angle = TYPICAL_BORESIGHT_EL;
      for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
      {
         sensor_calibration.fov_min_az_rad[look_id] = TYPICAL_FOV_MIN_AZ;
         sensor_calibration.fov_max_az_rad[look_id] = TYPICAL_FOV_MAX_AZ;
         sensor_calibration.fov_min_el_rad[look_id] = TYPICAL_FOV_MIN_EL;
         sensor_calibration.fov_max_el_rad[look_id] = TYPICAL_FOV_MAX_EL;
         sensor_calibration.r_wrapping[look_id] = TYPICAL_R_WRAPPING;
         sensor_calibration.v_wrapping[look_id] = TYPICAL_V_WRAPPING;
         sensor_calibration.range_limits[look_id] = TYPICAL_RANGE_LIMIT;
         sensor_calibration.min_aliaised_range_rate[look_id] = TYPICAL_MIN_ALIASED_RR;
      }

      F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
      memset(sensor_array, 0, sizeof(sensor_array));
      for (uint8_t sensor_id = VALID_SENSOR_ID_MIN; sensor_id <= VALID_SENSOR_ID_MAX; ++sensor_id)
      {
         sensor_array[sensor_id - 1].constant = sensor_calibration;
         sensor_array[sensor_id - 1].constant.id = sensor_id;
      }
      RSPP_Set_Sensor_Calibrations(sensor_array);
   }

   /**
    * Helper function to initialize host data
    */
   void setup_valid_host()
   {
      memset(&m_vehicle_state_data, 0, sizeof(m_vehicle_state_data));
      m_vehicle_state_data.speed = TYPICAL_HOST_SPEED;
      m_vehicle_state_data.vcs_speed = TYPICAL_HOST_SPEED;
      m_vehicle_state_data.yaw_rate_rad = TYPICAL_HOST_YAW_RATE;
      m_vehicle_state_data.acceleration = 0.0F;
      m_vehicle_state_data.curvature_rear = 0.0F;
      m_vehicle_state_data.host_type = RSPP_HOST_TYPE_PASSENGER_VEHICLE;
      m_vehicle_state_data.speed_qf = 3U; // ACCURATE
      m_vehicle_state_data.yaw_rate_qf = 3U;
   }

   /**
    * Helper function to initialize core info
    */
   void setup_empty_core_info()
   {
      memset(&m_core_info, 0, sizeof(m_core_info));
      m_core_info.cnt_loops = 1000U;
      m_core_info.elapsed_time_s = 0.05F;
      m_core_info.time_us = TYPICAL_TIME_US;
      m_core_info.prev_time_us = TYPICAL_TIME_US - 50000ULL;
   }

   /**
    * Helper function to initialize sensor array with valid data
    */
   void setup_valid_sensors()
   {
      memset(m_dynamic_sensor_data, 0, sizeof(m_dynamic_sensor_data));

      for (uint8_t sensor_idx = 0; sensor_idx < MAX_NUMBER_OF_SENSORS; ++sensor_idx)
      {
         m_dynamic_sensor_data[sensor_idx].variable.is_valid = true;
         m_dynamic_sensor_data[sensor_idx].variable.timestamp_us = TYPICAL_TIME_US - 10000ULL; // 10ms ago
         m_dynamic_sensor_data[sensor_idx].variable.look_id = RSPP_DET_LOOK_ID_0;
         m_dynamic_sensor_data[sensor_idx].variable.number_of_valid_detections = 0;
         m_dynamic_sensor_data[sensor_idx].variable.vcs_velocity.longitudinal = 0.0F;
         m_dynamic_sensor_data[sensor_idx].variable.vcs_velocity.lateral = 0.0F;
      }
   }

   /**
    * Helper function to initialize detection list
    */
   void setup_empty_detection_list()
   {
      memset(&m_detection_list, 0, sizeof(m_detection_list));
      m_detection_list.number_of_valid_detections = 0;
      m_detection_list.vcslong_det_idx_min = RSPP_INVALID_ID;
      m_detection_list.vcslong_det_idx_max = RSPP_INVALID_ID;
   }

   /**
    * Helper function to create a valid detection
    */
   void create_valid_detection(RSPP_Detection_T & detection, uint8_t sensor_id)
   {
      memset(&detection, 0, sizeof(detection));
      detection.raw.sensor_id = sensor_id;
      detection.raw.det_id = 1;
      detection.raw.range = TYPICAL_RANGE;
      detection.raw.range_rate = TYPICAL_RANGE_RATE;
      detection.raw.azimuth = TYPICAL_AZIMUTH;
      detection.raw.elevation = TYPICAL_ELEVATION;
      detection.raw.std_range = 0.5F;
      detection.raw.std_range_rate = 0.1F;
      detection.raw.std_azimuth = 0.01F;
      detection.raw.std_elevation = 0.02F;
      detection.raw.snr = TYPICAL_SNR;
      detection.raw.rcs = TYPICAL_RCS;
      detection.raw.prob_1stazhypo = 1.0F;
      detection.raw.confid_azimuth = 0; // High confidence
      detection.raw.confid_elevation = 0;
   }

   /** */
   float32_t normalize_heading_angle(const float32_t angle_in)
   {
      constexpr float32_t PI = 3.14159265358979323846F;
      constexpr float32_t TWO_PI = 6.28318530717958647693F;

      float32_t norm_angle = std::remainderf(angle_in, TWO_PI);
      if (norm_angle <= -PI)
      {
         norm_angle += TWO_PI;
      }
      else if (norm_angle > PI)
      {
         norm_angle -= TWO_PI;
      }

      return norm_angle;
   }

   /**
    * Helper function to calculate expected VCS coordinates for a detection
    * Based on sensor mounting position and detection spherical coordinates (range, azimuth, elevation)
    * Assumes typical sensor calibration with polarity = 1 and boresight azimuth = 0
    */
   void calculate_expected_vcs_coordinates(
       const float32_t range,
       const float32_t azimuth,
       const float32_t elevation,
       const float32_t sensor_long,
       const float32_t sensor_lat,
       const float32_t sensor_height,
       const float32_t boresight_az,
       const int32_t polarity,
       float32_t &expected_vcs_x,
       float32_t &expected_vcs_y,
       float32_t &expected_vcs_z)
   {
      // RSPP_Calculate_VCS_Angles adjustments
      const float32_t polarity_factor = static_cast<float32_t>(polarity);
      const float32_t vcs_az = normalize_heading_angle(boresight_az + polarity_factor * azimuth);
      const float32_t vcs_el = TYPICAL_BORESIGHT_EL + polarity_factor * elevation;
      const float32_t cos_vcs_az = static_cast<float32_t>(cosf(vcs_az));
      const float32_t sin_vcs_az = static_cast<float32_t>(sinf(vcs_az));
      const float32_t sin_vcs_el = static_cast<float32_t>(sinf(vcs_el));

      // RSPP_Calculate_VCS_Position adjustments
      expected_vcs_x = sensor_long + range * cos_vcs_az;
      expected_vcs_y = sensor_lat + range * sin_vcs_az;
      expected_vcs_z = -sensor_height + range * sin_vcs_el;
   }

   /**
    * Helper function for filling the detection list with valid detections from one sensor
    */
   void fill_detection_list_with_valid_detections_sensor(
       RSPP_Detection_List_T & r_detection_list,
       const uint8_t sensor_idx,
       const int32_t num_detections_sensor)
   {
      const float32_t azimuth_step = 1.0F / static_cast<float32_t>(MAX_DETS_FOR_SINGLE_SENSOR);
      const uint32_t num_detections_start = r_detection_list.number_of_valid_detections;
      uint32_t det_num = 0;
      for (uint32_t det_idx_sens = 0; det_idx_sens < num_detections_sensor; ++det_idx_sens)
      {
         const uint32_t det_idx = num_detections_start + det_idx_sens;
         const uint8_t sensor_id = VALID_SENSOR_ID_MIN + sensor_idx;
         det_num++;

         create_valid_detection(r_detection_list.detections[det_idx], sensor_id);
         r_detection_list.detections[det_idx].raw.det_id = det_num;
         r_detection_list.detections[det_idx].raw.range = 5.0F + static_cast<float32_t>(det_num) * 0.3F;

         // Create diverse azimuth angles across full FOV
         r_detection_list.detections[det_idx].raw.azimuth = -0.7F + static_cast<float32_t>(det_num) * azimuth_step;

         // Vary elevation
         r_detection_list.detections[det_idx].raw.elevation = -0.15F + static_cast<float32_t>(det_num % 15) * 0.02F;

         // Diverse range rates: approaching vehicles, receding, stationary objects
         float32_t range_rate_base = -40.0F + static_cast<float32_t>(sensor_idx) * 8.0F;
         r_detection_list.detections[det_idx].raw.range_rate = range_rate_base + static_cast<float32_t>(det_num % 30) * 2.0F;

         // Set processed values to NaN to verify they get updated
         r_detection_list.detections[det_idx].processed.vcs_position_x = std::numeric_limits<float32_t>::quiet_NaN();
         r_detection_list.detections[det_idx].processed.vcs_position_y = std::numeric_limits<float32_t>::quiet_NaN();
         r_detection_list.detections[det_idx].processed.vcs_position_z = std::numeric_limits<float32_t>::quiet_NaN();
         r_detection_list.detections[det_idx].processed.vcs_az = std::numeric_limits<float32_t>::quiet_NaN();
         r_detection_list.detections[det_idx].processed.vcs_el = std::numeric_limits<float32_t>::quiet_NaN();
         r_detection_list.detections[det_idx].processed.cos_vcs_az = std::numeric_limits<float32_t>::quiet_NaN();
         r_detection_list.detections[det_idx].processed.sin_vcs_az = std::numeric_limits<float32_t>::quiet_NaN();

         // Set motion status to invalid to verify it gets updated
         r_detection_list.detections[det_idx].processed.motion_status = static_cast<int8_t>(RSPP_DETECTION_MOTION_STATUS_INVALID);

         // Set f_ok_to_use to false to verify it gets updated
         r_detection_list.detections[det_idx].processed.f_ok_to_use = false;

         r_detection_list.number_of_valid_detections++;
      }
   }

   /**
    * Helper function for filling the detection list with valid detections evenly over all sensors
    */
   void fill_detection_list_with_valid_detections(
       RSPP_Detection_List_T & r_detection_list,
       const int32_t num_sensors,
       const int32_t num_detections)
   {
      r_detection_list.number_of_valid_detections = 0;
      int32_t num_detections_sensor = num_detections / num_sensors;
      num_detections_sensor = (num_detections_sensor < MAX_DETS_FOR_SINGLE_SENSOR)
                                  ? num_detections_sensor
                                  : MAX_DETS_FOR_SINGLE_SENSOR;
      for (uint8_t sensor_idx = 0; sensor_idx < num_sensors; ++sensor_idx)
      {
         if (sensor_idx == (num_sensors - 1))
         {
            // Last sensor takes remaining detections
            num_detections_sensor = num_detections - r_detection_list.number_of_valid_detections;
            num_detections_sensor = (num_detections_sensor < MAX_DETS_FOR_SINGLE_SENSOR)
                                        ? num_detections_sensor
                                        : MAX_DETS_FOR_SINGLE_SENSOR;
         }
         fill_detection_list_with_valid_detections_sensor(r_detection_list, sensor_idx, num_detections_sensor);
      }
   }

   /** \setup
    * Setup for RSPP_Process_Detections tests
    */
   TEST_SETUP()
   {
      setup_valid_host();
      setup_empty_core_info();
      setup_valid_sensors();
      setup_empty_detection_list();
   }

   /** \teardown
    * Teardown for RSPP_Process_Detections tests
    */
   TEST_TEARDOWN()
   {
      RSPP_State_Reset();
   }
};

// ==============================================================================
// Precondition Tests
// ==============================================================================

/** \purpose
 * Test that RSPP_Process_Detections returns error when RSPP is not initialized
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Fails_When_Not_Initialized)
{
   /** \step{1}
    * Ensures RSPP_Process_Detections returns error if not initialized.
    */

   /** \precond
    * RSPP is not initialized (fresh state)
    */
   // Don't call RSPP_Initialize

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should return RSPP_E_NOT_INITIALIZED
    */
   CHECK_EQUAL(RSPP_E_NOT_INITIALIZED, result);
}

/** \purpose
 * Test that RSPP_Process_Detections returns error when initialized but not calibrated
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Fails_When_Not_Calibrated)
{
   /** \step{1}
    * Checks that error is returned if RSPP is initialized but not calibrated.
    */

   /** \precond
    * RSPP is initialized but no sensors are calibrated
    */
   RSPP_Initialize();
   // Don't calibrate any sensors

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should return RSPP_E_NOT_INITIALIZED (state is INITIALIZED, not INIT_COMPLETE)
    */
   CHECK_EQUAL(RSPP_E_NOT_INITIALIZED, result);
}

/** \purpose
 * Test that RSPP_Process_Detections returns error when only partially calibrated
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Fails_When_Partially_Calibrated)
{
   /** \step{1}
    * Ensures error is returned if only some sensors are calibrated.
    */

   /** \precond
    * RSPP is initialized but only some sensors are calibrated
    */
   RSPP_Initialize();

   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));

   // Only calibrate first sensor with valid data
   sensor_array[0].constant.id = VALID_SENSOR_ID_MIN;
   sensor_array[0].constant.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
   sensor_array[0].constant.polarity = VALID_POLARITY_POSITIVE;
   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      sensor_array[0].constant.fov_min_az_rad[look_id] = TYPICAL_FOV_MIN_AZ;
      sensor_array[0].constant.fov_max_az_rad[look_id] = TYPICAL_FOV_MAX_AZ;
      sensor_array[0].constant.fov_min_el_rad[look_id] = TYPICAL_FOV_MIN_EL;
      sensor_array[0].constant.fov_max_el_rad[look_id] = TYPICAL_FOV_MAX_EL;
      sensor_array[0].constant.r_wrapping[look_id] = TYPICAL_R_WRAPPING;
      sensor_array[0].constant.v_wrapping[look_id] = TYPICAL_V_WRAPPING;
   }
   // Other sensors remain uncalibrated (id = 0 is invalid)

   RSPP_Set_Sensor_Calibrations(sensor_array);
   // Only calibrated 1 sensor, not all

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should return RSPP_E_NOT_INITIALIZED (state is still INITIALIZED, not INIT_COMPLETE)
    */
   CHECK_EQUAL(RSPP_E_NOT_INITIALIZED, result);
}

/** \purpose
 * Test that RSPP_Process_Detections fails when almost all sensors are calibrated (MAX_SENSORS - 1)
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Fails_When_Missing_One_Sensor_Calibration)
{
   /** \step{1}
    * Ensures error is returned if MAX_SENSORS - 1 sensors are calibrated.
    */

   /** \precond
    * RSPP is initialized but only MAX_SENSORS - 1 sensors are calibrated
    */
   RSPP_Initialize();

   ConstantProps_T sensor_calibration;
   memset(&sensor_calibration, 0, sizeof(sensor_calibration));
   sensor_calibration.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
   sensor_calibration.polarity = VALID_POLARITY_POSITIVE;
   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      sensor_calibration.fov_min_az_rad[look_id] = TYPICAL_FOV_MIN_AZ;
      sensor_calibration.fov_max_az_rad[look_id] = TYPICAL_FOV_MAX_AZ;
      sensor_calibration.fov_min_el_rad[look_id] = TYPICAL_FOV_MIN_EL;
      sensor_calibration.fov_max_el_rad[look_id] = TYPICAL_FOV_MAX_EL;
      sensor_calibration.r_wrapping[look_id] = TYPICAL_R_WRAPPING;
      sensor_calibration.v_wrapping[look_id] = TYPICAL_V_WRAPPING;
   }

   // Calibrate all sensors except the last one
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));
   for (uint8_t sensor_id = VALID_SENSOR_ID_MIN; sensor_id < VALID_SENSOR_ID_MAX; ++sensor_id)
   {
      sensor_array[sensor_id - 1].constant = sensor_calibration;
      sensor_array[sensor_id - 1].constant.id = sensor_id;
   }
   // Last sensor remains uncalibrated (id = 0 is invalid)
   RSPP_Set_Sensor_Calibrations(sensor_array);
   // Did not calibrate sensor with id = VALID_SENSOR_ID_MAX

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should return RSPP_E_NOT_INITIALIZED (state is still INITIALIZED, not INIT_COMPLETE)
    */
   CHECK_EQUAL(RSPP_E_NOT_INITIALIZED, result);
}

/** \purpose
 * Test that RSPP_Process_Detections succeeds when fully initialized and calibrated
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Succeeds_When_Fully_Calibrated)
{
   /** \step{1}
    * Verifies that processing succeeds when fully initialized and calibrated.
    */

   /** \precond
    * RSPP is fully initialized and all sensors are calibrated
    */
   setup_rspp_fully_initialized();

   /** \action
    * Call RSPP_Process_Detections with empty detection list
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should return RSPP_E_OK
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

// ==============================================================================
// Basic Functionality Tests
// ==============================================================================

/** \purpose
 * Test processing with empty detection list
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Empty_Detection_List)
{
   /** \step{1}
    * Checks processing with an empty detection list.
    */

   /** \precond
    * RSPP is fully initialized
    */
   setup_rspp_fully_initialized();
   m_detection_list.number_of_valid_detections = 0;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and detection count should remain 0
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(0U, m_detection_list.number_of_valid_detections);
}

/** \purpose
 * Test processing with single detection
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Single_Detection)
{
   /** \step{1}
    * Verifies processing with a single valid detection.
    */

   /** \precond
    * RSPP is fully initialized with one valid detection
    */
   setup_rspp_fully_initialized();
   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and detection count should remain 1
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(1U, m_detection_list.number_of_valid_detections);
}

/** \purpose
 * Test processing with multiple detections from same sensor
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Multiple_Detections_Same_Sensor)
{
   /** \step{1}
    * Checks processing with multiple detections from the same sensor.
    */

   /** \precond
    * RSPP is fully initialized with multiple valid detections from same sensor
    */
   setup_rspp_fully_initialized();

   const uint32_t num_detections = 5U;
   for (uint32_t i = 0; i < num_detections; ++i)
   {
      create_valid_detection(m_detection_list.detections[i], VALID_SENSOR_ID_MIN);
      m_detection_list.detections[i].raw.det_id = static_cast<int32_t>(i + 1);
      m_detection_list.detections[i].raw.range = TYPICAL_RANGE + static_cast<float32_t>(i) * 10.0F;
   }
   m_detection_list.number_of_valid_detections = num_detections;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and all detections should be processed
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(num_detections, m_detection_list.number_of_valid_detections);
}

/** \purpose
 * Test processing with detections from different sensors
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Multiple_Detections_Different_Sensors)
{
   /** \step{1}
    * Verifies processing with detections from different sensors.
    */

   /** \precond
    * RSPP is fully initialized with detections from different sensors
    */
   setup_rspp_fully_initialized();

   // Create detections from different sensors
   for (uint8_t sensor_id = VALID_SENSOR_ID_MIN; sensor_id <= VALID_SENSOR_ID_MAX; ++sensor_id)
   {
      uint32_t det_idx = sensor_id - VALID_SENSOR_ID_MIN;
      create_valid_detection(m_detection_list.detections[det_idx], sensor_id);
      m_detection_list.detections[det_idx].raw.range = TYPICAL_RANGE + static_cast<float32_t>(det_idx) * 5.0F;
   }
   m_detection_list.number_of_valid_detections = MAX_NUMBER_OF_SENSORS;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and all detections should be processed
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(static_cast<uint32_t>(MAX_NUMBER_OF_SENSORS), m_detection_list.number_of_valid_detections);
}

/** \purpose
 * Test processing with 4 sensors each having 100 detections (realistic scenario)
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Four_Sensors_100_Detections_Each)
{
   /** \step{1}
    * Verifies processing with multiple sensors having many detections each.
    */

   /** \precond
    * RSPP is fully initialized with 4 sensors, each with 100 detections
    */
   setup_rspp_fully_initialized();

   const uint8_t num_sensors = 4U;
   const uint32_t detections_per_sensor = 100U;
   const uint32_t total_detections = num_sensors * detections_per_sensor;

   uint32_t det_idx = 0;
   for (uint8_t sensor_id = VALID_SENSOR_ID_MIN; sensor_id < VALID_SENSOR_ID_MIN + num_sensors; ++sensor_id)
   {
      for (uint32_t det_id = 1; det_id <= detections_per_sensor; ++det_id)
      {
         create_valid_detection(m_detection_list.detections[det_idx], sensor_id);
         m_detection_list.detections[det_idx].raw.det_id = static_cast<int32_t>(det_id);
         m_detection_list.detections[det_idx].raw.range = 5.0F + static_cast<float32_t>(det_id) * 0.5F;
         m_detection_list.detections[det_idx].raw.azimuth = -0.5F + static_cast<float32_t>(det_id % 20) * 0.05F;
         ++det_idx;
      }
   }
   m_detection_list.number_of_valid_detections = total_detections;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and all detections should be processed
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(total_detections, m_detection_list.number_of_valid_detections);
}

/** \purpose
 * Test processing with 4 sensors having varying detection counts (realistic scenario)
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Four_Sensors_Varying_Detection_Counts)
{
   /** \step{1}
    * Verifies processing with sensors having different numbers of detections.
    */

   /** \precond
    * RSPP is fully initialized with 4 sensors having 50, 100, 150, and 200 detections respectively
    */
   setup_rspp_fully_initialized();

   const uint8_t num_sensors = 4U;
   const uint32_t detections_per_sensor[] = {50U, 100U, 150U, 200U};
   uint32_t total_detections = 0;

   uint32_t det_idx = 0;
   for (uint8_t sensor_idx = 0; sensor_idx < num_sensors; ++sensor_idx)
   {
      uint8_t sensor_id = VALID_SENSOR_ID_MIN + sensor_idx;
      uint32_t num_dets = detections_per_sensor[sensor_idx];
      total_detections += num_dets;

      for (uint32_t det_id = 1; det_id <= num_dets; ++det_id)
      {
         create_valid_detection(m_detection_list.detections[det_idx], sensor_id);
         m_detection_list.detections[det_idx].raw.det_id = static_cast<int32_t>(det_id);
         m_detection_list.detections[det_idx].raw.range = 10.0F + static_cast<float32_t>(det_id) * 0.3F;
         m_detection_list.detections[det_idx].raw.range_rate = -5.0F + static_cast<float32_t>(det_id % 30) * 0.5F;
         ++det_idx;
      }
   }
   m_detection_list.number_of_valid_detections = total_detections;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and all detections should be processed
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(total_detections, m_detection_list.number_of_valid_detections);
}

/** \purpose
 * Test processing with all sensors having moderate detection counts (realistic scenario)
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_All_Sensors_Moderate_Detections)
{
   /** \step{1}
    * Verifies processing with all configured sensors, each having 75 detections.
    */

   /** \precond
    * RSPP is fully initialized with all sensors, each with 75 detections
    */
   setup_rspp_fully_initialized();

   const uint32_t detections_per_sensor = 75U;
   const uint32_t total_detections = MAX_NUMBER_OF_SENSORS * detections_per_sensor;

   uint32_t det_idx = 0;
   for (uint8_t sensor_id = VALID_SENSOR_ID_MIN; sensor_id <= VALID_SENSOR_ID_MAX; ++sensor_id)
   {
      for (uint32_t det_id = 1; det_id <= detections_per_sensor; ++det_id)
      {
         create_valid_detection(m_detection_list.detections[det_idx], sensor_id);
         m_detection_list.detections[det_idx].raw.det_id = static_cast<int32_t>(det_id);
         m_detection_list.detections[det_idx].raw.range = 8.0F + static_cast<float32_t>(det_id) * 0.4F;
         m_detection_list.detections[det_idx].raw.elevation = -0.2F + static_cast<float32_t>(det_id % 10) * 0.04F;
         ++det_idx;
      }
   }
   m_detection_list.number_of_valid_detections = total_detections;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and all detections should be processed
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(total_detections, m_detection_list.number_of_valid_detections);
}

/** \purpose
 * Test processing with typical automotive scenario: front, rear, and corner sensors
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Typical_Automotive_Scenario)
{
   /** \step{1}
    * Verifies processing with realistic automotive sensor layout and detection distribution.
    */

   /** \precond
    * RSPP is fully initialized
    * Front sensors (2): 120 detections each
    * Rear sensors (2): 80 detections each
    * Corner sensors (4): 60 detections each
    */
   setup_rspp_fully_initialized();

   struct SensorConfig
   {
      uint8_t sensor_id;
      uint32_t num_detections;
   };

   const SensorConfig sensor_configs[] = {
       {1, 120U},
       {2, 120U},
       {3, 80U},
       {4, 80U},
       {5, 60U},
       {6, 60U},
       {7, 60U},
       {8, 60U}};

   const uint8_t num_sensors = 8U;
   uint32_t total_detections = 0;
   uint32_t det_idx = 0;

   for (uint8_t sensor_idx = 0; sensor_idx < num_sensors; ++sensor_idx)
   {
      uint8_t sensor_id = sensor_configs[sensor_idx].sensor_id;
      uint32_t num_dets = sensor_configs[sensor_idx].num_detections;
      total_detections += num_dets;

      for (uint32_t det_id = 1; det_id <= num_dets; ++det_id)
      {
         create_valid_detection(m_detection_list.detections[det_idx], sensor_id);
         m_detection_list.detections[det_idx].raw.det_id = static_cast<int32_t>(det_id);
         // Vary range based on sensor type (front sensors see further)
         float32_t base_range = (sensor_idx < 2) ? 15.0F : 8.0F;
         m_detection_list.detections[det_idx].raw.range = base_range + static_cast<float32_t>(det_id) * 0.2F;
         m_detection_list.detections[det_idx].raw.azimuth = -0.6F + static_cast<float32_t>(det_id % 25) * 0.05F;
         ++det_idx;
      }
   }
   m_detection_list.number_of_valid_detections = total_detections;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and all detections should be processed
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(total_detections, m_detection_list.number_of_valid_detections);
}

// ==============================================================================
// Input Validation Tests
// ==============================================================================

/** \purpose
 * Test that detections with invalid sensor_id (0) invalidate the detection list
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Invalid_Sensor_ID_Zero)
{
   /** \step{1}
    * Ensures detections with sensor_id 0 invalidate the detection list.
    */

   /** \precond
    * RSPP is fully initialized with detection having sensor_id = 0
    */
   setup_rspp_fully_initialized();
   create_valid_detection(m_detection_list.detections[0], 0); // Invalid sensor ID
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed but detection list should be invalidated (count = 0)
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(0U, m_detection_list.number_of_valid_detections);
}

/** \purpose
 * Test that detections with sensor_id above max invalidate the detection list
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Invalid_Sensor_ID_Above_Max)
{
   /** \step{1}
    * Ensures detections with sensor_id above max invalidate the detection list.
    */

   /** \precond
    * RSPP is fully initialized with detection having sensor_id > MAX
    */
   setup_rspp_fully_initialized();
   create_valid_detection(m_detection_list.detections[0], MAX_NUMBER_OF_SENSORS + 1);
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed but detection list should be invalidated (count = 0)
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(0U, m_detection_list.number_of_valid_detections);
}

/** \purpose
 * Test that too many detections invalidates the detection list
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Too_Many_Detections)
{
   /** \step{1}
    * Checks that too many detections invalidates the detection list.
    */

   /** \precond
    * RSPP is fully initialized with number_of_valid_detections > MAX
    */
   setup_rspp_fully_initialized();
   m_detection_list.number_of_valid_detections = MAX_NUMBER_OF_DETECTIONS + 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed but detection list should be invalidated (count = 0)
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(0U, m_detection_list.number_of_valid_detections);
}

/** \purpose
 * Test processing at maximum detection capacity
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Maximum_Detections)
{
   /** \step{1}
    * Verifies processing at maximum detection capacity.
    */

   /** \precond
    * RSPP is fully initialized with maximum number of detections
    */
   setup_rspp_fully_initialized();

   for (uint32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; ++i)
   {
      uint8_t sensor_id = static_cast<uint8_t>((i % MAX_NUMBER_OF_SENSORS) + 1);
      create_valid_detection(m_detection_list.detections[i], sensor_id);
      // det_id must be per-sensor and <= MAX_DETS_FOR_SINGLE_SENSOR
      uint32_t det_per_sensor = (i / MAX_NUMBER_OF_SENSORS) + 1;
      m_detection_list.detections[i].raw.det_id = static_cast<int32_t>(det_per_sensor);
      m_detection_list.detections[i].raw.range = 10.0F + static_cast<float32_t>(i) * 0.5F;
   }
   m_detection_list.number_of_valid_detections = MAX_NUMBER_OF_DETECTIONS;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and all detections should be processed
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(static_cast<uint32_t>(MAX_NUMBER_OF_DETECTIONS), m_detection_list.number_of_valid_detections);
}

/** \purpose
 * Test that critical fault in last detection (invalid sensor_id) invalidates entire list
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Invalid_Sensor_ID_In_Last_Detection_Invalidates_All)
{
   /** \step{1}
    * Verifies that a faulty sensor_id in the last detection causes the entire detection list to be invalidated.
    */

   /** \precond
    * RSPP is fully initialized with maximum number of detections, last detection has invalid sensor_id
    */
   setup_rspp_fully_initialized();

   int32_t det_id_cnt[MAX_NUMBER_OF_SENSORS] = {0};
   for (uint32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; ++i)
   {
      uint8_t sensor_id = static_cast<uint8_t>((i % MAX_NUMBER_OF_SENSORS) + 1);
      create_valid_detection(m_detection_list.detections[i], sensor_id);
      m_detection_list.detections[i].raw.det_id = ++det_id_cnt[sensor_id - 1];
      m_detection_list.detections[i].raw.range = 10.0F + static_cast<float32_t>(i) * 0.5F;
   }

   // Make last detection have invalid sensor_id (0 is invalid, must be >= 1)
   m_detection_list.detections[MAX_NUMBER_OF_DETECTIONS - 1].raw.sensor_id = 0;

   m_detection_list.number_of_valid_detections = MAX_NUMBER_OF_DETECTIONS;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should return RSPP_E_OK but detection list should be invalidated (number_of_valid_detections = 0)
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(0U, m_detection_list.number_of_valid_detections);

   // Make last detection have ok
   m_detection_list.detections[MAX_NUMBER_OF_DETECTIONS - 1].raw.sensor_id = 1;

   m_detection_list.number_of_valid_detections = MAX_NUMBER_OF_DETECTIONS;

   /** \action
    * Call RSPP_Process_Detections
    */
   result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should return RSPP_E_OK but detection list should be invalidated (number_of_valid_detections = 0)
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(MAX_NUMBER_OF_DETECTIONS, m_detection_list.number_of_valid_detections);

   // Make last detection have invalid sensor_id (MAX_NUMBER_OF_SENSORS + 1 is invalid, must be >= 1)
   m_detection_list.detections[MAX_NUMBER_OF_DETECTIONS - 1].raw.sensor_id = MAX_NUMBER_OF_SENSORS + 1;

   m_detection_list.number_of_valid_detections = MAX_NUMBER_OF_DETECTIONS;

   /** \action
    * Call RSPP_Process_Detections
    */
   result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should return RSPP_E_OK but detection list should be invalidated (number_of_valid_detections = 0)
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(0U, m_detection_list.number_of_valid_detections);
}

/** \purpose
 * Test that critical fault in last detection (invalid det_id) invalidates entire list
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Invalid_Det_ID_In_Last_Detection_Invalidates_All)
{
   /** \step{1}
    * Verifies that a faulty det_id in the last detection causes the entire detection list to be invalidated.
    */

   /** \precond
    * RSPP is fully initialized with maximum number of detections, last detection has invalid det_id
    */
   setup_rspp_fully_initialized();

   int32_t det_id_cnt[MAX_NUMBER_OF_SENSORS] = {0};
   for (uint32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; ++i)
   {
      uint8_t sensor_id = static_cast<uint8_t>((i % MAX_NUMBER_OF_SENSORS) + 1);
      create_valid_detection(m_detection_list.detections[i], sensor_id);
      m_detection_list.detections[i].raw.det_id = ++det_id_cnt[sensor_id - 1];
      m_detection_list.detections[i].raw.range = 10.0F + static_cast<float32_t>(i) * 0.5F;
   }

   // Make last detection have invalid det_id (0 is invalid, must be >= 1)
   m_detection_list.detections[MAX_NUMBER_OF_DETECTIONS - 1].raw.det_id = 0;

   m_detection_list.number_of_valid_detections = MAX_NUMBER_OF_DETECTIONS;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should return RSPP_E_OK but detection list should be invalidated (number_of_valid_detections = 0)
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(0U, m_detection_list.number_of_valid_detections);

   // Make last detection ok
   m_detection_list.detections[MAX_NUMBER_OF_DETECTIONS - 1].raw.det_id = 1;

   m_detection_list.number_of_valid_detections = MAX_NUMBER_OF_DETECTIONS;

   /** \action
    * Call RSPP_Process_Detections
    */
   result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should return RSPP_E_OK but detection list should be invalidated (number_of_valid_detections = 0)
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(MAX_NUMBER_OF_DETECTIONS, m_detection_list.number_of_valid_detections);

   // Make last detection have invalid det_id (MAX_DETS_FOR_SINGLE_SENSOR + 1 is invalid, must be <= MAX_DETS_FOR_SINGLE_SENSOR)
   m_detection_list.detections[MAX_NUMBER_OF_DETECTIONS - 1].raw.det_id = MAX_DETS_FOR_SINGLE_SENSOR + 1;

   m_detection_list.number_of_valid_detections = MAX_NUMBER_OF_DETECTIONS;

   /** \action
    * Call RSPP_Process_Detections
    */
   result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should return RSPP_E_OK but detection list should be invalidated (number_of_valid_detections = 0)
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(0U, m_detection_list.number_of_valid_detections);
}

// ==============================================================================
// Coordinate Transformation Tests
// ==============================================================================

/** \purpose
 * Test VCS coordinate calculation for detection directly ahead
 * \req
 * CPR-7939_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_VCS_Coordinates_Straight_Ahead)
{
   /** \step{1}
    * Checks VCS coordinate calculation for detection directly ahead.
    */

   /** \precond
    * RSPP is fully initialized with detection directly ahead (azimuth = 0)
    */
   setup_rspp_fully_initialized();
   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[0].raw.range = 100.0F;
   m_detection_list.detections[0].raw.azimuth = 0.0F;
   m_detection_list.detections[0].raw.elevation = 0.0F;
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * VCS position should be range + longitudinal mounting, lateral ~= 0
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   DOUBLES_EQUAL(100.0F + TYPICAL_LONGITUDINAL,
                 m_detection_list.detections[0].processed.vcs_position_x,
                 TOLERANCE);
   DOUBLES_EQUAL(TYPICAL_LATERAL,
                 m_detection_list.detections[0].processed.vcs_position_y,
                 TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation for detection at 45 degrees left
 * \req
 * CPR-7939_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_VCS_Coordinates_45_Degrees_Left)
{
   /** \step{1}
    * Verifies VCS coordinate calculation for detection at 45 degrees left.
    */

   /** \precond
    * RSPP is fully initialized with detection at 45 degrees left
    */
   setup_rspp_fully_initialized();
   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   const float32_t range = 100.0F;
   const float32_t azimuth = 0.7854F; // ~45 degrees (PI/4)
   m_detection_list.detections[0].raw.range = range;
   m_detection_list.detections[0].raw.azimuth = azimuth;
   m_detection_list.detections[0].raw.elevation = 0.0F;
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * VCS X and Y should be ~equal (45 degree angle)
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   // For 45 degree angle, cos(45) = sin(45) ~= 0.707
   const float32_t expected_x = range * std::cos(azimuth) + TYPICAL_LONGITUDINAL;
   const float32_t expected_y = range * std::sin(azimuth) + TYPICAL_LATERAL;
   DOUBLES_EQUAL(expected_x,
                 m_detection_list.detections[0].processed.vcs_position_x,
                 TOLERANCE);
   DOUBLES_EQUAL(expected_y,
                 m_detection_list.detections[0].processed.vcs_position_y,
                 TOLERANCE);
}

/** \purpose
 * Test VCS coordinate calculation for detection at 45 degrees right
 * \req
 * CPR-7939_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_VCS_Coordinates_45_Degrees_Right)
{
   /** \step{1}
    * Verifies VCS coordinate calculation for detection at 45 degrees right.
    */

   /** \precond
    * RSPP is fully initialized with detection at -45 degrees (right)
    */
   setup_rspp_fully_initialized();
   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   const float32_t range = 100.0F;
   const float32_t azimuth = -0.7854F; // ~-45 degrees
   m_detection_list.detections[0].raw.range = range;
   m_detection_list.detections[0].raw.azimuth = azimuth;
   m_detection_list.detections[0].raw.elevation = 0.0F;
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * VCS Y should be negative (right side)
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK(m_detection_list.detections[0].processed.vcs_position_y < 0.0F);
}

/** \purpose
 * Test VCS coordinate transformation with normal scenario: 4 sensors with multiple detections
 * \req
 * CPR-7939_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_VCS_Coordinates_Normal_Four_Sensors)
{
   /** \step{1}
    * Verifies VCS coordinate transformation with 4 sensors having 100 detections each.
    */

   /** \precond
    * RSPP is fully initialized with 4 sensors, each with 100 detections at various angles
    */
   setup_rspp_fully_initialized();
   const uint8_t num_sensors = 4U;
   const uint32_t detections_per_sensor = 100U;
   const uint32_t total_detections = num_sensors * detections_per_sensor;
   fill_detection_list_with_valid_detections(m_detection_list, num_sensors, total_detections);

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and all detections should have valid VCS coordinates
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(total_detections, m_detection_list.number_of_valid_detections);

   // Verify VCS coordinates match expected values for all detections
   for (uint32_t i = 0; i < total_detections; ++i)
   {
      // Calculate expected VCS coordinates based on detection raw values
      float32_t expected_vcs_x, expected_vcs_y, expected_vcs_z;
      calculate_expected_vcs_coordinates(
          m_detection_list.detections[i].raw.range,
          m_detection_list.detections[i].raw.azimuth,
          m_detection_list.detections[i].raw.elevation,
          TYPICAL_LONGITUDINAL, TYPICAL_LATERAL, TYPICAL_HEIGHT,
          TYPICAL_BORESIGHT_AZ, VALID_POLARITY_POSITIVE,
          expected_vcs_x, expected_vcs_y, expected_vcs_z);

      // Verify calculated VCS coordinates are close to expected values
      DOUBLES_EQUAL(expected_vcs_x, m_detection_list.detections[i].processed.vcs_position_x, TOLERANCE);
      DOUBLES_EQUAL(expected_vcs_y, m_detection_list.detections[i].processed.vcs_position_y, TOLERANCE);
      DOUBLES_EQUAL(expected_vcs_z, m_detection_list.detections[i].processed.vcs_position_z, TOLERANCE);
   }
}

/** \purpose
 * Test VCS coordinate transformation with upper boundary: 10 sensors with maximum detections
 * \req
 * CPR-7939_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_VCS_Coordinates_Upper_Boundary_Ten_Sensors)
{
   /** \step{1}
    * Verifies VCS coordinate transformation with 10 sensors at near-maximum capacity.
    */

   /** \precond
    * RSPP is fully initialized with 10 sensors at high detection count
    */
   setup_rspp_fully_initialized();
   const uint8_t num_sensors = MAX_NUMBER_OF_SENSORS;
   const uint32_t total_detections = MAX_NUMBER_OF_DETECTIONS;
   fill_detection_list_with_valid_detections(m_detection_list, num_sensors, total_detections);

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and all detections should have valid VCS coordinates
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(MAX_NUMBER_OF_DETECTIONS, m_detection_list.number_of_valid_detections);

   // Verify VCS coordinates are calculated correctly for all detections
   for (uint32_t det_idx = 0; det_idx < MAX_NUMBER_OF_DETECTIONS; ++det_idx)
   {
      // Verify coordinates are computed
      float32_t expected_vcs_x, expected_vcs_y, expected_vcs_z;
      calculate_expected_vcs_coordinates(
          m_detection_list.detections[det_idx].raw.range,
          m_detection_list.detections[det_idx].raw.azimuth,
          m_detection_list.detections[det_idx].raw.elevation,
          TYPICAL_LONGITUDINAL, TYPICAL_LATERAL, TYPICAL_HEIGHT,
          TYPICAL_BORESIGHT_AZ, VALID_POLARITY_POSITIVE,
          expected_vcs_x, expected_vcs_y, expected_vcs_z);
      const bool big_diff = (m_detection_list.detections[det_idx].raw.range > 280.0F);
      const float32_t y_tolerance = big_diff ? TOLERANCE * 10.0F : TOLERANCE;
      DOUBLES_EQUAL(expected_vcs_x, m_detection_list.detections[det_idx].processed.vcs_position_x, TOLERANCE);
      DOUBLES_EQUAL(expected_vcs_y, m_detection_list.detections[det_idx].processed.vcs_position_y, y_tolerance);
      DOUBLES_EQUAL(expected_vcs_z, m_detection_list.detections[det_idx].processed.vcs_position_z, TOLERANCE);
   }
}

// ==============================================================================
// Range Rate Compensation Tests
// ==============================================================================

/** \purpose
 * Test range rate compensation with stationary host
 * \req
 * CPR-7942_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Range_Rate_Compensation_Stationary_Host)
{
   /** \step{1}
    * Checks range rate compensation with stationary host vehicle.
    */

   /** \precond
    * RSPP is fully initialized with stationary host
    */
   setup_rspp_fully_initialized();
   m_vehicle_state_data.speed = 0.0F;
   m_vehicle_state_data.vcs_speed = 0.0F;

   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[0].raw.range_rate = -10.0F;
   m_detection_list.detections[0].raw.azimuth = 0.0F;
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Compensated range rate should be close to raw range rate (no host motion to compensate)
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK(m_detection_list.detections[0].processed.f_ok_to_use);
   // With stationary host, compensated RR should be approximately equal to raw RR
   DOUBLES_EQUAL(-10.0F, m_detection_list.detections[0].processed.range_rate_compensated, TOLERANCE);
}

/** \purpose
 * Test range rate compensation with moving host approaching stationary target
 * \req
 * CPR-7942_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Range_Rate_Compensation_Moving_Host)
{
   /** \step{1}
    * Verifies range rate compensation with moving host vehicle.
    */

   /** \precond
    * RSPP is fully initialized with moving host
    */
   setup_rspp_fully_initialized();
   m_vehicle_state_data.speed = 20.0F;
   m_vehicle_state_data.vcs_speed = 20.0F;

   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[0].raw.range_rate = -20.0F; // Approaching at host speed
   m_detection_list.detections[0].raw.azimuth = 0.0F;
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and compensated range rate should be different from raw range rate
    * (since host is moving, compensation should be applied)
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK(m_detection_list.detections[0].processed.f_ok_to_use);
   // Compensated range rate should be different from raw (compensation was applied)
   // The exact value depends on sensor position and VCS transformation
   CHECK(m_detection_list.detections[0].processed.range_rate_compensated != m_detection_list.detections[0].raw.range_rate);
}

/** \purpose
 * Test range rate compensation with normal scenario: 4 sensors with multiple detections and moving host
 * \req
 * CPR-7942_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Range_Rate_Compensation_Normal_Four_Sensors)
{
   /** \step{1}
    * Verifies range rate compensation with 4 sensors having 100 detections each and moving host.
    */

   /** \precond
    * RSPP is fully initialized with 4 sensors, each with 100 detections, and host vehicle moving
    */
   setup_rspp_fully_initialized();
   m_vehicle_state_data.speed = 25.0F;
   m_vehicle_state_data.vcs_speed = 25.0F;

   const uint8_t num_sensors = 4U;
   const uint32_t detections_per_sensor = 100U;
   const uint32_t total_detections = num_sensors * detections_per_sensor;
   fill_detection_list_with_valid_detections(m_detection_list, num_sensors, total_detections);

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and all detections should have compensated range rate calculated
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(total_detections, m_detection_list.number_of_valid_detections);

   // Verify range rate compensation was applied for all detections
   uint32_t valid_compensation_count = 0;
   for (uint32_t i = 0; i < total_detections; ++i)
   {
      // All detections should be marked ok to use
      if (m_detection_list.detections[i].processed.f_ok_to_use)
      {
         // Compensated range rate should be finite
         if (std::isfinite(m_detection_list.detections[i].processed.range_rate_compensated))
         {
            ++valid_compensation_count;
         }
      }
   }

   // All detections should have valid compensated range rate
   CHECK_EQUAL(total_detections, valid_compensation_count);
}

/** \purpose
 * Test range rate compensation with upper boundary: 10 sensors with maximum detections
 * \req
 * CPR-7942_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Range_Rate_Compensation_Upper_Boundary_Ten_Sensors)
{
   /** \step{1}
    * Verifies range rate compensation with 10 sensors at near-maximum capacity with moving host.
    */

   /** \precond
    * RSPP is fully initialized with 10 sensors at high detection count, host vehicle moving at highway speed
    */
   setup_rspp_fully_initialized();
   m_vehicle_state_data.speed = 30.0F; // Highway speed
   m_vehicle_state_data.vcs_speed = 30.0F;
   m_vehicle_state_data.yaw_rate_rad = 0.05F; // Slight curve

   const uint8_t num_sensors = MAX_NUMBER_OF_SENSORS;
   const uint32_t total_detections = MAX_NUMBER_OF_DETECTIONS;
   fill_detection_list_with_valid_detections(m_detection_list, num_sensors, total_detections);

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and all detections should have valid compensated range rate
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(MAX_NUMBER_OF_DETECTIONS, m_detection_list.number_of_valid_detections);

   // Verify range rate compensation was applied correctly for all detections
   uint32_t valid_compensation_count = 0;
   for (uint32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; ++i)
   {
      // Count detections marked ok to use
      if (m_detection_list.detections[i].processed.f_ok_to_use)
      {
         // Verify compensated range rate is finite (not NaN or infinite)
         if (std::isfinite(m_detection_list.detections[i].processed.range_rate_compensated))
         {
            ++valid_compensation_count;
         }
      }
   }

   // All detections should be ok to use and have valid compensated range rate
   CHECK_EQUAL(MAX_NUMBER_OF_DETECTIONS, valid_compensation_count);
}

// ==============================================================================
// f_ok_to_use Flag Tests
// ==============================================================================

/** \purpose
 * Test that f_ok_to_use is set to true for valid detection
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_OK_To_Use_Flag_Valid_Detection)
{
   /** \step{1}
    * Checks that f_ok_to_use is set for a valid detection.
    */

   /** \precond
    * RSPP is fully initialized with valid detection
    */
   setup_rspp_fully_initialized();
   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * f_ok_to_use should be true for valid detection
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK(m_detection_list.detections[0].processed.f_ok_to_use);
}

/** \purpose
 * Test that all valid detections have f_ok_to_use set appropriately
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_OK_To_Use_Flag_All_Valid_Detections)
{
   /** \step{1}
    * Verifies f_ok_to_use is set for all valid detections.
    */

   /** \precond
    * RSPP is fully initialized with multiple valid detections
    */
   setup_rspp_fully_initialized();

   const uint32_t num_detections = 5U;
   for (uint32_t i = 0; i < num_detections; ++i)
   {
      create_valid_detection(m_detection_list.detections[i], VALID_SENSOR_ID_MIN);
      m_detection_list.detections[i].raw.det_id = static_cast<int32_t>(i + 1);
      m_detection_list.detections[i].raw.range = TYPICAL_RANGE + static_cast<float32_t>(i) * 10.0F;
   }
   m_detection_list.number_of_valid_detections = num_detections;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * All detections should have f_ok_to_use set
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   for (uint32_t i = 0; i < num_detections; ++i)
   {
      CHECK(m_detection_list.detections[i].processed.f_ok_to_use);
   }
}

/** \purpose
 * Test f_ok_to_use flag with normal scenario: 4 sensors with 100+ detections each, some invalid
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_OK_To_Use_Flag_Normal_Four_Sensors_Mixed_Validity)
{
   /** \step{1}
    * Verifies f_ok_to_use flag correctly identifies valid and invalid detections in a realistic scenario.
    */

   /** \precond
    * RSPP is fully initialized with 4 sensors, each with 120 detections
    * Each sensor has 5 invalid detections (negative range, invalid det_id, etc.)
    */
   setup_rspp_fully_initialized();

   const uint8_t num_sensors = 4U;
   const uint32_t detections_per_sensor = 120U;
   const uint32_t invalid_detections_per_sensor = 5U;
   const uint32_t total_detections = num_sensors * detections_per_sensor;
   const uint32_t total_invalid = num_sensors * invalid_detections_per_sensor;
   const uint32_t total_valid = total_detections - total_invalid;
   fill_detection_list_with_valid_detections(m_detection_list, num_sensors, total_detections);

   // Create incalid detections. (Valid detections are filled per sensor first)
   bool expected_invalid[total_detections];
   for (uint32_t i = 0; i < total_detections; ++i)
   {
      expected_invalid[i] = false;
   }
   for (uint8_t sensor_idx = 0; sensor_idx < num_sensors; ++sensor_idx)
   {
      for (uint8_t invalid_det_idx_sensor = 0; invalid_det_idx_sensor < invalid_detections_per_sensor; ++invalid_det_idx_sensor)
      {
         uint32_t det_idx = sensor_idx * detections_per_sensor + invalid_det_idx_sensor;

         // Invalid: negative range causes VCS transformation to fail
         m_detection_list.detections[det_idx].raw.range = -10.0F;

         expected_invalid[det_idx] = true;
      }
   }

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and only invalid detections should have f_ok_to_use = false
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(total_detections, m_detection_list.number_of_valid_detections);

   // Count detections marked as ok to use and not ok to use
   uint32_t ok_to_use_count = 0;
   uint32_t not_ok_to_use_count = 0;

   for (uint32_t i = 0; i < total_detections; ++i)
   {
      if (m_detection_list.detections[i].processed.f_ok_to_use)
      {
         ++ok_to_use_count;
         // Verify that detections marked ok to use were not expected to be invalid
         CHECK_FALSE(expected_invalid[i]);
      }
      else
      {
         ++not_ok_to_use_count;
         // Verify that detections marked not ok to use were expected to be invalid
         CHECK_TRUE(expected_invalid[i]);
      }
   }

   // Verify counts match expectations
   CHECK_EQUAL(total_valid, ok_to_use_count);
   CHECK_EQUAL(total_invalid, not_ok_to_use_count);
}

/** \purpose
 * Test f_ok_to_use flag with maximum detections: only first detection invalid
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_OK_To_Use_Flag_Max_Detections_First_Invalid)
{
   /** \step{1}
    * Verifies f_ok_to_use flag with maximum detections where only the first is invalid.
    */

   /** \precond
    * RSPP is fully initialized with maximum detections, first detection has invalid range
    */
   setup_rspp_fully_initialized();

   const uint8_t num_sensors = MAX_NUMBER_OF_SENSORS;
   const uint32_t total_detections = MAX_NUMBER_OF_DETECTIONS;
   fill_detection_list_with_valid_detections(m_detection_list, num_sensors, total_detections);

   // Make first detection invalid with negative range
   m_detection_list.detections[0].raw.range = -10.0F;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed, first detection should have f_ok_to_use = false, rest should be true
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(static_cast<uint32_t>(MAX_NUMBER_OF_DETECTIONS), m_detection_list.number_of_valid_detections);

   // First detection should NOT be ok to use (it has negative range)
   CHECK_FALSE(m_detection_list.detections[0].processed.f_ok_to_use);
   // All other detection should be ok to use
   for (uint32_t det_idx = 1U; det_idx < MAX_NUMBER_OF_DETECTIONS; ++det_idx)
   {
      CHECK_TRUE(m_detection_list.detections[det_idx].processed.f_ok_to_use);
   }
}

/** \purpose
 * Test f_ok_to_use flag with maximum detections: only last detection invalid
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_OK_To_Use_Flag_Max_Detections_Last_Invalid)
{
   /** \step{1}
    * Verifies f_ok_to_use flag with maximum detections where only the last is invalid.
    */

   /** \precond
    * RSPP is fully initialized with maximum detections, last detection has invalid range
    */
   setup_rspp_fully_initialized();

   const uint8_t num_sensors = MAX_NUMBER_OF_SENSORS;
   const uint32_t total_detections = MAX_NUMBER_OF_DETECTIONS;
   fill_detection_list_with_valid_detections(m_detection_list, num_sensors, total_detections);

   // Make last detection invalid with negative range
   m_detection_list.detections[MAX_NUMBER_OF_DETECTIONS - 1].raw.range = -10.0F;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed, last detection should have f_ok_to_use = false, rest should be true
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(static_cast<uint32_t>(MAX_NUMBER_OF_DETECTIONS), m_detection_list.number_of_valid_detections);

   // Last detection should NOT be ok to use (it has negative range)
   CHECK_FALSE(m_detection_list.detections[MAX_NUMBER_OF_DETECTIONS - 1].processed.f_ok_to_use);
   // All other detection should be ok to use
   for (uint32_t det_idx = 0U; det_idx < (MAX_NUMBER_OF_DETECTIONS - 1U); ++det_idx)
   {
      CHECK_TRUE(m_detection_list.detections[det_idx].processed.f_ok_to_use);
   }
}

// ==============================================================================
// Motion Status Tests
// ==============================================================================

/** \purpose
 * Test motion status calculation for potentially stationary detection
 * \req
 * CPR-3848_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Motion_Status_Stationary)
{
   /** \step{1}
    * Checks motion status calculation for a stationary detection.
    */

   /** \precond
    * RSPP is fully initialized with detection that appears stationary
    */
   setup_rspp_fully_initialized();
   m_vehicle_state_data.speed = 20.0F;
   m_vehicle_state_data.vcs_speed = 20.0F;

   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   // Range rate = -host_speed means target is stationary (host approaching at host speed)
   m_detection_list.detections[0].raw.range_rate = -20.0F;
   m_detection_list.detections[0].raw.azimuth = 0.0F;
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Motion status should be calculated as a valid enum value
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   // Motion status should be a valid enum value: INVALID(-1), STATIONARY(0), MOVING(1), or AMBIGUOUS(2)
   CHECK(m_detection_list.detections[0].processed.motion_status >=
         static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_INVALID));
   CHECK(m_detection_list.detections[0].processed.motion_status <=
         static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS));
}

/** \purpose
 * Test motion status calculation for moving detection
 * \req
 * CPR-3848_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Motion_Status_Moving)
{
   /** \step{1}
    * Verifies motion status calculation for a moving detection.
    */

   /** \precond
    * RSPP is fully initialized with detection that appears moving
    */
   setup_rspp_fully_initialized();
   m_vehicle_state_data.speed = 20.0F;
   m_vehicle_state_data.vcs_speed = 20.0F;

   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   // Target with different relative velocity than stationary objects
   m_detection_list.detections[0].raw.range_rate = -30.0F; // Moving towards us faster than host speed
   m_detection_list.detections[0].raw.azimuth = 0.0F;
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Motion status should be calculated as a valid enum value
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   // Motion status should be a valid enum value: INVALID(-1), STATIONARY(0), MOVING(1), or AMBIGUOUS(2)
   CHECK(m_detection_list.detections[0].processed.motion_status >=
         static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_INVALID));
   CHECK(m_detection_list.detections[0].processed.motion_status <=
         static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS));
}

/** \purpose
 * Test motion status with stationary host: lower boundary (1 sensor, 1 detection)
 * \req
 * CPR-3848_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Motion_Status_Stationary_Host_Lower_Boundary)
{
   /** \step{1}
    * Verifies motion status with stationary host and minimal detections.
    */

   /** \precond
    * RSPP is fully initialized with stationary host (speed = 0), 1 sensor, 1 detection
    */
   setup_rspp_fully_initialized();
   m_vehicle_state_data.speed = 0.0F;
   m_vehicle_state_data.vcs_speed = 0.0F;
   m_vehicle_state_data.yaw_rate_rad = 0.0F;

   const int32_t num_sensors = 1;
   const int32_t total_detections = 1;
   fill_detection_list_with_valid_detections(m_detection_list, num_sensors, total_detections);

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and motion status should be calculated
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(static_cast<uint32_t>(total_detections), m_detection_list.number_of_valid_detections);
   for (uint32_t i = 0; i < m_detection_list.number_of_valid_detections; ++i)
   {
      // Motion status should be a valid enum value: MOVING(1), or AMBIGUOUS(2)
      CHECK(m_detection_list.detections[i].processed.motion_status ==
                static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING) ||
            m_detection_list.detections[i].processed.motion_status ==
                static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS));
   }
}

/** \purpose
 * Test motion status with stationary host: normal scenario (4 sensors, 250 detections)
 * \req
 * CPR-3848_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Motion_Status_Stationary_Host_Normal)
{
   /** \step{1}
    * Verifies motion status with stationary host and normal detection load.
    */

   /** \precond
    * RSPP is fully initialized with stationary host, 4 sensors, 250 detections
    */
   setup_rspp_fully_initialized();
   m_vehicle_state_data.speed = 0.0F;
   m_vehicle_state_data.vcs_speed = 0.0F;
   m_vehicle_state_data.yaw_rate_rad = 0.0F;

   const int32_t num_sensors = 4;
   const int32_t total_detections = 250;
   fill_detection_list_with_valid_detections(m_detection_list, num_sensors, total_detections);

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and all detections processed
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(static_cast<uint32_t>(total_detections), m_detection_list.number_of_valid_detections);
   for (uint32_t i = 0; i < m_detection_list.number_of_valid_detections; ++i)
   {
      // Motion status should be a valid enum value: MOVING(1), or AMBIGUOUS(2)
      CHECK(m_detection_list.detections[i].processed.motion_status ==
                static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING) ||
            m_detection_list.detections[i].processed.motion_status ==
                static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS));
   }
}

/** \purpose
 * Test motion status with stationary host: upper boundary (10 sensors, max detections)
 * \req
 * CPR-3848_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Motion_Status_Stationary_Host_Upper_Boundary)
{
   /** \step{1}
    * Verifies motion status with stationary host at maximum capacity.
    */

   /** \precond
    * RSPP is fully initialized with stationary host, 10 sensors, maximum detections
    */
   setup_rspp_fully_initialized();
   m_vehicle_state_data.speed = 0.0F;
   m_vehicle_state_data.vcs_speed = 0.0F;
   m_vehicle_state_data.yaw_rate_rad = 0.0F;

   const int32_t num_sensors = 10;
   const int32_t total_detections = MAX_NUMBER_OF_DETECTIONS;
   fill_detection_list_with_valid_detections(m_detection_list, num_sensors, total_detections);

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and all detections processed
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(static_cast<uint32_t>(total_detections), m_detection_list.number_of_valid_detections);
   for (uint32_t i = 0; i < m_detection_list.number_of_valid_detections; ++i)
   {
      // Motion status should be a valid enum value: MOVING(1), or AMBIGUOUS(2)
      CHECK(m_detection_list.detections[i].processed.motion_status ==
                static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING) ||
            m_detection_list.detections[i].processed.motion_status ==
                static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS));
   }
}

/** \purpose
 * Test motion status with moving host: lower boundary (1 sensor, 1 detection)
 * \req
 * CPR-3848_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Motion_Status_Moving_Host_Lower_Boundary)
{
   /** \step{1}
    * Verifies motion status with moving host and minimal detections.
    */

   /** \precond
    * RSPP is fully initialized with moving host (25 m/s), 1 sensor, 1 detection
    */
   setup_rspp_fully_initialized();
   m_vehicle_state_data.speed = 25.0F;
   m_vehicle_state_data.vcs_speed = 25.0F;
   m_vehicle_state_data.yaw_rate_rad = 0.0F;

   const int32_t num_sensors = 1;
   const int32_t total_detections = 1;
   fill_detection_list_with_valid_detections(m_detection_list, num_sensors, total_detections);

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and motion status should be calculated
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(static_cast<uint32_t>(total_detections), m_detection_list.number_of_valid_detections);
   for (uint32_t i = 0; i < m_detection_list.number_of_valid_detections; ++i)
   {
      // Motion status should be a valid enum value: MOVING(1), or AMBIGUOUS(2)
      CHECK(m_detection_list.detections[i].processed.motion_status ==
                static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING) ||
            m_detection_list.detections[i].processed.motion_status ==
                static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS));
   }
}

/** \purpose
 * Test motion status with moving host: normal scenario (6 sensors, 420 detections)
 * \req
 * CPR-3848_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Motion_Status_Moving_Host_Normal)
{
   /** \step{1}
    * Verifies motion status with moving host and varied detection load.
    */

   /** \precond
    * RSPP is fully initialized with moving host, 6 sensors, 420 detections
    */
   setup_rspp_fully_initialized();
   m_vehicle_state_data.speed = 25.0F;
   m_vehicle_state_data.vcs_speed = 25.0F;
   m_vehicle_state_data.yaw_rate_rad = 0.0F;

   const int32_t num_sensors = 6;
   const int32_t total_detections = 420;
   fill_detection_list_with_valid_detections(m_detection_list, num_sensors, total_detections);

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and all detections processed
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(static_cast<uint32_t>(total_detections), m_detection_list.number_of_valid_detections);
   for (uint32_t i = 0; i < m_detection_list.number_of_valid_detections; ++i)
   {
      // Motion status should be a valid enum value: MOVING(1), or AMBIGUOUS(2)
      CHECK(m_detection_list.detections[i].processed.motion_status ==
                static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING) ||
            m_detection_list.detections[i].processed.motion_status ==
                static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS));
   }
}

/** \purpose
 * Test motion status with moving host: upper boundary (10 sensors, max detections)
 * \req
 * CPR-3848_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Motion_Status_Moving_Host_Upper_Boundary)
{
   /** \step{1}
    * Verifies motion status with moving host at maximum capacity.
    */

   /** \precond
    * RSPP is fully initialized with moving host, 10 sensors, maximum detections
    */
   setup_rspp_fully_initialized();
   m_vehicle_state_data.speed = 25.0F;
   m_vehicle_state_data.vcs_speed = 25.0F;
   m_vehicle_state_data.yaw_rate_rad = 0.0F;

   const int32_t num_sensors = 10;
   const int32_t total_detections = MAX_NUMBER_OF_DETECTIONS;
   fill_detection_list_with_valid_detections(m_detection_list, num_sensors, total_detections);

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and all detections processed
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(static_cast<uint32_t>(total_detections), m_detection_list.number_of_valid_detections);
   for (uint32_t i = 0; i < m_detection_list.number_of_valid_detections; ++i)
   {
      // Motion status should be a valid enum value: MOVING(1), or AMBIGUOUS(2)
      CHECK(m_detection_list.detections[i].processed.motion_status ==
                static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING) ||
            m_detection_list.detections[i].processed.motion_status ==
                static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS));
   }
}

/** \purpose
 * Test motion status with turning host: lower boundary (1 sensor, 1 detection)
 * \req
 * CPR-3848_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Motion_Status_Turning_Host_Lower_Boundary)
{
   /** \step{1}
    * Verifies motion status with turning host and minimal detections.
    */

   /** \precond
    * RSPP is fully initialized with turning host (20 m/s, 0.1 rad/s yaw rate), 1 sensor, 1 detection
    */
   setup_rspp_fully_initialized();
   m_vehicle_state_data.speed = 20.0F;
   m_vehicle_state_data.vcs_speed = 20.0F;
   m_vehicle_state_data.yaw_rate_rad = 0.1F; // Turning

   const int32_t num_sensors = 1;
   const int32_t total_detections = 1;
   fill_detection_list_with_valid_detections(m_detection_list, num_sensors, total_detections);

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and motion status should be calculated
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(static_cast<uint32_t>(total_detections), m_detection_list.number_of_valid_detections);
   for (uint32_t i = 0; i < m_detection_list.number_of_valid_detections; ++i)
   {
      // Motion status should be a valid enum value: MOVING(1), or AMBIGUOUS(2)
      CHECK(m_detection_list.detections[i].processed.motion_status ==
                static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING) ||
            m_detection_list.detections[i].processed.motion_status ==
                static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS));
   }
}

/** \purpose
 * Test motion status with turning host: normal scenario (8 sensors, 600 detections)
 * \req
 * CPR-3848_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Motion_Status_Turning_Host_Normal)
{
   /** \step{1}
    * Verifies motion status with turning host and varied detection load.
    */

   /** \precond
    * RSPP is fully initialized with turning host, 8 sensors, 600 detections
    */
   setup_rspp_fully_initialized();
   m_vehicle_state_data.speed = 20.0F;
   m_vehicle_state_data.vcs_speed = 20.0F;
   m_vehicle_state_data.yaw_rate_rad = 0.1F; // Turning

   const int32_t num_sensors = 8;
   const int32_t total_detections = 600;
   fill_detection_list_with_valid_detections(m_detection_list, num_sensors, total_detections);

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and all detections processed
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(static_cast<uint32_t>(total_detections), m_detection_list.number_of_valid_detections);
   for (uint32_t i = 0; i < m_detection_list.number_of_valid_detections; ++i)
   {
      // Motion status should be a valid enum value: MOVING(1), or AMBIGUOUS(2)
      CHECK(m_detection_list.detections[i].processed.motion_status ==
                static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING) ||
            m_detection_list.detections[i].processed.motion_status ==
                static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS));
   }
}

/** \purpose
 * Test motion status with turning host: upper boundary (10 sensors, max detections)
 * \req
 * CPR-3848_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Motion_Status_Turning_Host_Upper_Boundary)
{
   /** \step{1}
    * Verifies motion status with turning host at maximum capacity.
    */

   /** \precond
    * RSPP is fully initialized with turning host, 10 sensors, maximum detections
    */
   setup_rspp_fully_initialized();
   m_vehicle_state_data.speed = 20.0F;
   m_vehicle_state_data.vcs_speed = 20.0F;
   m_vehicle_state_data.yaw_rate_rad = 0.1F; // Turning

   const int32_t num_sensors = 10;
   const int32_t total_detections = MAX_NUMBER_OF_DETECTIONS;
   fill_detection_list_with_valid_detections(m_detection_list, num_sensors, total_detections);

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and all detections processed
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(static_cast<uint32_t>(total_detections), m_detection_list.number_of_valid_detections);
   for (uint32_t i = 0; i < m_detection_list.number_of_valid_detections; ++i)
   {
      // Motion status should be a valid enum value: MOVING(1), or AMBIGUOUS(2)
      CHECK(m_detection_list.detections[i].processed.motion_status ==
                static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING) ||
            m_detection_list.detections[i].processed.motion_status ==
                static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS));
   }
}

// ==============================================================================
// VCS Azimuth/Elevation Tests
// ==============================================================================

/** \purpose
 * Test that VCS azimuth is calculated correctly
 * \req
 * CPR-7939_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_VCS_Azimuth_Calculated)
{
   /** \step{1}
    * Verifies that VCS azimuth is calculated correctly for a detection.
    */

   /** \precond
    * RSPP is fully initialized
    */
   setup_rspp_fully_initialized();
   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[0].raw.azimuth = 0.5F;
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * VCS azimuth should be populated
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   // VCS azimuth should be close to raw azimuth for forward-facing sensor with 0 boresight
   DOUBLES_EQUAL(0.5F, m_detection_list.detections[0].processed.vcs_az, TOLERANCE);
}

/** \purpose
 * Test that cos and sin of VCS azimuth are calculated
 * \req
 * CPR-7939_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_VCS_Azimuth_Trig_Values)
{
   /** \step{1}
    * Checks that cos and sin of VCS azimuth are calculated as expected.
    */

   /** \precond
    * RSPP is fully initialized
    */
   setup_rspp_fully_initialized();
   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[0].raw.azimuth = 0.0F; // Straight ahead
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * cos(0) should be ~1, sin(0) should be ~0
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   DOUBLES_EQUAL(1.0F, m_detection_list.detections[0].processed.cos_vcs_az, TOLERANCE);
   DOUBLES_EQUAL(0.0F, m_detection_list.detections[0].processed.sin_vcs_az, TOLERANCE);
}

// ==============================================================================
// Sorting Tests
// ==============================================================================

/** \purpose
 * Test that detections are sorted by VCS longitudinal position
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Detections_Sorted_VCS_Long)
{
   /** \step{1}
    * Verifies that detections are sorted by VCS longitudinal position.
    */

   /** \precond
    * RSPP is fully initialized with multiple detections at different ranges
    */
   setup_rspp_fully_initialized();

   // Create detections in reverse order (far to near)
   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[0].raw.range = 100.0F;
   m_detection_list.detections[0].raw.azimuth = 0.0F;

   create_valid_detection(m_detection_list.detections[1], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[1].raw.range = 50.0F;
   m_detection_list.detections[1].raw.azimuth = 0.0F;

   create_valid_detection(m_detection_list.detections[2], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[2].raw.range = 75.0F;
   m_detection_list.detections[2].raw.azimuth = 0.0F;

   m_detection_list.number_of_valid_detections = 3;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and sorted indices should be populated correctly
    * Detection 1 (range 50) should be min, Detection 0 (range 100) should be max
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   // Verify sorted indices are populated
   CHECK(m_detection_list.vcslong_det_idx_min != RSPP_INVALID_ID);
   CHECK(m_detection_list.vcslong_det_idx_max != RSPP_INVALID_ID);
   // Verify min index points to the closest detection (range 50 -> det index 1)
   CHECK_EQUAL(1, m_detection_list.vcslong_det_idx_min);
   // Verify max index points to the farthest detection (range 100 -> det index 0)
   CHECK_EQUAL(0, m_detection_list.vcslong_det_idx_max);
   // Verify linked list order: follow next_sorted_idx from min to max
   int16_t current_idx = m_detection_list.vcslong_det_idx_min;
   float32_t prev_vcs_x = m_detection_list.detections[current_idx].processed.vcs_position_x;
   while (m_detection_list.detections[current_idx].processed.next_sorted_idx != RSPP_INVALID_ID)
   {
      current_idx = m_detection_list.detections[current_idx].processed.next_sorted_idx;
      float32_t current_vcs_x = m_detection_list.detections[current_idx].processed.vcs_position_x;
      CHECK(current_vcs_x >= prev_vcs_x); // VCS X should be increasing
      prev_vcs_x = current_vcs_x;
   }
   // Should end at max index
   CHECK_EQUAL(m_detection_list.vcslong_det_idx_max, current_idx);
}

/** \purpose
 * Test sorting of detections from multiple sensors with varying ranges
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Sorting_Multiple_Sensors_Normal)
{
   /** \step{1}
    * Verifies that detections from multiple sensors are sorted correctly by VCS longitudinal position.
    */

   /** \precond
    * RSPP is fully initialized with detections from 4 sensors
    */
   setup_rspp_fully_initialized();

   // Create 4 sensors with 50 detections each = 200 total
   fill_detection_list_with_valid_detections(m_detection_list, 4, 200);

   // Update specific detections with varying ranges for testing
   // Sensor 0: mix of near, mid, and far ranges
   m_detection_list.detections[0].raw.range = 150.0F; // Far
   m_detection_list.detections[0].raw.azimuth = 0.0F;
   m_detection_list.detections[10].raw.range = 30.0F; // Near
   m_detection_list.detections[10].raw.azimuth = 0.0F;
   m_detection_list.detections[20].raw.range = 80.0F; // Mid
   m_detection_list.detections[20].raw.azimuth = 0.0F;

   // Sensor 1: different range distribution
   m_detection_list.detections[50].raw.range = 120.0F; // Far
   m_detection_list.detections[50].raw.azimuth = 0.0F;
   m_detection_list.detections[60].raw.range = 45.0F; // Mid
   m_detection_list.detections[60].raw.azimuth = 0.0F;
   m_detection_list.detections[70].raw.range = 15.0F; // Very near
   m_detection_list.detections[70].raw.azimuth = 0.0F;

   // Sensor 2: another range distribution
   m_detection_list.detections[100].raw.range = 200.0F; // Very far
   m_detection_list.detections[100].raw.azimuth = 0.0F;
   m_detection_list.detections[110].raw.range = 90.0F; // Mid
   m_detection_list.detections[110].raw.azimuth = 0.0F;

   // Sensor 3: close range detections
   m_detection_list.detections[150].raw.range = 25.0F; // Near
   m_detection_list.detections[150].raw.azimuth = 0.0F;
   m_detection_list.detections[160].raw.range = 50.0F; // Mid-near
   m_detection_list.detections[160].raw.azimuth = 0.0F;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and detections should be sorted in ascending VCS X order
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK(m_detection_list.vcslong_det_idx_min != RSPP_INVALID_ID);
   CHECK(m_detection_list.vcslong_det_idx_max != RSPP_INVALID_ID);

   // Traverse sorted list and verify ascending VCS X positions
   int16_t current_idx = m_detection_list.vcslong_det_idx_min;
   float32_t prev_vcs_x = m_detection_list.detections[current_idx].processed.vcs_position_x;
   int32_t sorted_count = 1;

   while (m_detection_list.detections[current_idx].processed.next_sorted_idx != RSPP_INVALID_ID)
   {
      current_idx = m_detection_list.detections[current_idx].processed.next_sorted_idx;
      float32_t current_vcs_x = m_detection_list.detections[current_idx].processed.vcs_position_x;
      CHECK(current_vcs_x >= prev_vcs_x); // VCS X should be non-decreasing
      prev_vcs_x = current_vcs_x;
      sorted_count++;
   }

   CHECK_EQUAL(m_detection_list.vcslong_det_idx_max, current_idx);
   // Verify all valid detections are in the sorted list
   CHECK_EQUAL(200, sorted_count);
}

/** \purpose
 * Test sorting with maximum detections from multiple sensors
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Sorting_Upper_Boundary_Max_Detections)
{
   /** \step{1}
    * Verifies that maximum detections from multiple sensors are sorted correctly.
    */

   /** \precond
    * RSPP is fully initialized with maximum detections from 10 sensors
    */
   setup_rspp_fully_initialized();

   // Create 10 sensors with max detections
   fill_detection_list_with_valid_detections(m_detection_list, 10, MAX_NUMBER_OF_DETECTIONS);

   // Update detections at various positions with specific ranges
   m_detection_list.detections[0].raw.range = 250.0F; // Very far
   m_detection_list.detections[0].raw.azimuth = 0.0F;
   m_detection_list.detections[1000].raw.range = 10.0F; // Very near
   m_detection_list.detections[1000].raw.azimuth = 0.0F;
   m_detection_list.detections[5000].raw.range = 100.0F; // Mid
   m_detection_list.detections[5000].raw.azimuth = 0.0F;
   m_detection_list.detections[9215].raw.range = 180.0F; // Far (last detection)
   m_detection_list.detections[9215].raw.azimuth = 0.0F;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and all detections should be in sorted order
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK(m_detection_list.vcslong_det_idx_min != RSPP_INVALID_ID);
   CHECK(m_detection_list.vcslong_det_idx_max != RSPP_INVALID_ID);

   // Traverse sorted list and verify ascending VCS X positions
   int16_t current_idx = m_detection_list.vcslong_det_idx_min;
   float32_t prev_vcs_x = m_detection_list.detections[current_idx].processed.vcs_position_x;
   int32_t sorted_count = 1;

   while (m_detection_list.detections[current_idx].processed.next_sorted_idx != RSPP_INVALID_ID)
   {
      current_idx = m_detection_list.detections[current_idx].processed.next_sorted_idx;
      float32_t current_vcs_x = m_detection_list.detections[current_idx].processed.vcs_position_x;
      CHECK(current_vcs_x >= prev_vcs_x);
      prev_vcs_x = current_vcs_x;
      sorted_count++;
   }

   CHECK_EQUAL(m_detection_list.vcslong_det_idx_max, current_idx);
   CHECK_EQUAL(MAX_NUMBER_OF_DETECTIONS, sorted_count);
}

/** \purpose
 * Test sorting with detections at identical ranges
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Sorting_Identical_Ranges)
{
   /** \step{1}
    * Verifies stable sorting when multiple detections have identical ranges.
    */

   /** \precond
    * RSPP is fully initialized with detections at same range
    */
   setup_rspp_fully_initialized();

   // Create 6 sensors with 120 detections
   fill_detection_list_with_valid_detections(m_detection_list, 6, 120);

   // Set multiple detections to identical range but different sensors
   for (int32_t i = 0; i < 3; i++)
   {
      for (int32_t j = 0; j < 6; j++)
      {
         const int32_t wanted_det_idx = i + j * 20;
         m_detection_list.detections[wanted_det_idx].raw.range = 75.0F;
         m_detection_list.detections[wanted_det_idx].raw.azimuth = 0.0F;
         m_detection_list.detections[wanted_det_idx].raw.elevation = 0.0F;
      }
   }
   const int32_t num_identical_range_inserted = 3 * 6;

   // Add some different ranges for variety
   m_detection_list.detections[50].raw.range = 50.0F;
   m_detection_list.detections[50].raw.azimuth = 0.0F;
   m_detection_list.detections[90].raw.range = 100.0F;
   m_detection_list.detections[90].raw.azimuth = 0.0F;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and maintain stable sort order
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK(m_detection_list.vcslong_det_idx_min != RSPP_INVALID_ID);
   CHECK(m_detection_list.vcslong_det_idx_max != RSPP_INVALID_ID);

   // Traverse sorted list
   int16_t current_idx = m_detection_list.vcslong_det_idx_min;
   float32_t prev_vcs_x = m_detection_list.detections[current_idx].processed.vcs_position_x;
   int32_t sorted_count = 1;
   int32_t identical_range_count = 0;

   while (m_detection_list.detections[current_idx].processed.next_sorted_idx != RSPP_INVALID_ID)
   {
      if (fabsf(m_detection_list.detections[current_idx].raw.range - 75.0F) < TOLERANCE)
      {
         identical_range_count++;
      }

      current_idx = m_detection_list.detections[current_idx].processed.next_sorted_idx;
      float32_t current_vcs_x = m_detection_list.detections[current_idx].processed.vcs_position_x;
      CHECK(current_vcs_x >= prev_vcs_x);
      prev_vcs_x = current_vcs_x;
      sorted_count++;
   }

   CHECK_EQUAL(m_detection_list.vcslong_det_idx_max, current_idx);
   CHECK_EQUAL(120, sorted_count);
   CHECK_EQUAL(num_identical_range_inserted, identical_range_count);
}

/** \purpose
 * Test sorting with single sensor and multiple detections
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Sorting_Single_Sensor_Many_Detections)
{
   /** \step{1}
    * Verifies sorting works correctly with detections from a single sensor.
    */

   /** \precond
    * RSPP is fully initialized with detections from 1 sensor
    */
   setup_rspp_fully_initialized();

   // Create 1 sensor with 150 detections
   fill_detection_list_with_valid_detections(m_detection_list, 1, 150);

   // Set varying ranges to test sorting
   m_detection_list.detections[0].raw.range = 200.0F;
   m_detection_list.detections[0].raw.azimuth = 0.0F;
   m_detection_list.detections[25].raw.range = 50.0F;
   m_detection_list.detections[25].raw.azimuth = 0.0F;
   m_detection_list.detections[50].raw.range = 125.0F;
   m_detection_list.detections[50].raw.azimuth = 0.0F;
   m_detection_list.detections[75].raw.range = 20.0F;
   m_detection_list.detections[75].raw.azimuth = 0.0F;
   m_detection_list.detections[100].raw.range = 175.0F;
   m_detection_list.detections[100].raw.azimuth = 0.0F;
   m_detection_list.detections[149].raw.range = 10.0F; // Last detection, smallest range
   m_detection_list.detections[149].raw.azimuth = 0.0F;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and detections sorted correctly
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK(m_detection_list.vcslong_det_idx_min != RSPP_INVALID_ID);
   CHECK(m_detection_list.vcslong_det_idx_max != RSPP_INVALID_ID);

   // Verify the minimum is detection 149 (range 10.0)
   float32_t min_vcs_x = m_detection_list.detections[m_detection_list.vcslong_det_idx_min].processed.vcs_position_x;
   CHECK(min_vcs_x < 15.0F); // Should be close to 10m

   // Traverse and verify sorting
   int16_t current_idx = m_detection_list.vcslong_det_idx_min;
   float32_t prev_vcs_x = m_detection_list.detections[current_idx].processed.vcs_position_x;
   int32_t sorted_count = 1;

   while (m_detection_list.detections[current_idx].processed.next_sorted_idx != RSPP_INVALID_ID)
   {
      current_idx = m_detection_list.detections[current_idx].processed.next_sorted_idx;
      float32_t current_vcs_x = m_detection_list.detections[current_idx].processed.vcs_position_x;
      CHECK(current_vcs_x >= prev_vcs_x);
      prev_vcs_x = current_vcs_x;
      sorted_count++;
   }

   CHECK_EQUAL(m_detection_list.vcslong_det_idx_max, current_idx);
   CHECK_EQUAL(150, sorted_count);
}

// ==============================================================================
// Edge Case Tests
// ==============================================================================

/** \purpose
 * Test processing with zero range detection
 * \req
 * CPR-7939_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Zero_Range_Detection)
{
   /** \step{1}
    * Checks processing with a detection at zero range.
    */

   /** \precond
    * RSPP is fully initialized with zero range detection
    */
   setup_rspp_fully_initialized();
   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[0].raw.range = 0.0F;
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and VCS position should be at sensor mounting position
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   // With zero range, VCS position should be approximately at sensor mounting position
   DOUBLES_EQUAL(TYPICAL_LONGITUDINAL, m_detection_list.detections[0].processed.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(TYPICAL_LATERAL, m_detection_list.detections[0].processed.vcs_position_y, TOLERANCE);
}

/** \purpose
 * Test processing with very large range detection
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Large_Range_Detection)
{
   /** \step{1}
    * Verifies processing with a detection at large range.
    */

   /** \precond
    * RSPP is fully initialized with large range detection
    */
   setup_rspp_fully_initialized();
   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[0].raw.range = 300.0F; // Max typical range
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and detection should be processed correctly
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK(m_detection_list.detections[0].processed.f_ok_to_use);
   // Calculate expected VCS coordinates
   float32_t expected_vcs_x, expected_vcs_y, expected_vcs_z;
   calculate_expected_vcs_coordinates(
       300.0F, TYPICAL_AZIMUTH, TYPICAL_ELEVATION,
       TYPICAL_LONGITUDINAL, TYPICAL_LATERAL, TYPICAL_HEIGHT,
       TYPICAL_BORESIGHT_AZ, VALID_POLARITY_POSITIVE,
       expected_vcs_x, expected_vcs_y, expected_vcs_z);
   // Verify VCS position matches expected values
   DOUBLES_EQUAL(expected_vcs_x, m_detection_list.detections[0].processed.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(expected_vcs_y, m_detection_list.detections[0].processed.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(expected_vcs_z, m_detection_list.detections[0].processed.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test processing with boundary azimuth values
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Boundary_Azimuth_Values)
{
   /** \step{1}
    * Checks processing with detections at FOV azimuth boundaries.
    */

   /** \precond
    * RSPP is fully initialized with detections at FOV boundaries
    */
   setup_rspp_fully_initialized();

   // Detection at left FOV boundary
   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[0].raw.azimuth = TYPICAL_FOV_MAX_AZ - 0.01F;

   // Detection at right FOV boundary
   create_valid_detection(m_detection_list.detections[1], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[1].raw.azimuth = TYPICAL_FOV_MIN_AZ + 0.01F;

   m_detection_list.number_of_valid_detections = 2;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and both detections should be processed correctly
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(2U, m_detection_list.number_of_valid_detections);
   // Verify both detections are valid
   CHECK(m_detection_list.detections[0].processed.f_ok_to_use);
   CHECK(m_detection_list.detections[1].processed.f_ok_to_use);
   // Calculate and verify expected VCS coordinates for detection 0 (left FOV boundary)
   float32_t expected_vcs_x0, expected_vcs_y0, expected_vcs_z0;
   calculate_expected_vcs_coordinates(
       TYPICAL_RANGE, TYPICAL_FOV_MAX_AZ - 0.01F, TYPICAL_ELEVATION,
       TYPICAL_LONGITUDINAL, TYPICAL_LATERAL, TYPICAL_HEIGHT,
       TYPICAL_BORESIGHT_AZ, VALID_POLARITY_POSITIVE,
       expected_vcs_x0, expected_vcs_y0, expected_vcs_z0);
   DOUBLES_EQUAL(expected_vcs_x0, m_detection_list.detections[0].processed.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(expected_vcs_y0, m_detection_list.detections[0].processed.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(expected_vcs_z0, m_detection_list.detections[0].processed.vcs_position_z, TOLERANCE);
   // Calculate and verify expected VCS coordinates for detection 1 (right FOV boundary)
   float32_t expected_vcs_x1, expected_vcs_y1, expected_vcs_z1;
   calculate_expected_vcs_coordinates(
       TYPICAL_RANGE, TYPICAL_FOV_MIN_AZ + 0.01F, TYPICAL_ELEVATION,
       TYPICAL_LONGITUDINAL, TYPICAL_LATERAL, TYPICAL_HEIGHT,
       TYPICAL_BORESIGHT_AZ, VALID_POLARITY_POSITIVE,
       expected_vcs_x1, expected_vcs_y1, expected_vcs_z1);
   DOUBLES_EQUAL(expected_vcs_x1, m_detection_list.detections[1].processed.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(expected_vcs_y1, m_detection_list.detections[1].processed.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(expected_vcs_z1, m_detection_list.detections[1].processed.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test processing with invalid sensor (is_valid = false)
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Invalid_Sensor_Data)
{
   /** \step{1}
    * Verifies processing when a sensor has is_valid = false.
    */

   /** \precond
    * RSPP is fully initialized but one sensor has is_valid = false
    */
   setup_rspp_fully_initialized();
   m_dynamic_sensor_data[0].variable.is_valid = false;

   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed - detection is still processed
    * The detection still gets transformed since calibration data is valid
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   // Function succeeds - VCS coordinates are still calculated
   // Verify processed data is populated (transformation still happens)
   CHECK(m_detection_list.detections[0].processed.vcs_position_x > 0.0F);
}

/** \purpose
 * Test processing with zero time
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Zero_Timestamp)
{
   /** \step{1}
    * Checks processing with a zero timestamp value.
    */

   /** \precond
    * RSPP is fully initialized
    */
   setup_rspp_fully_initialized();
   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.number_of_valid_detections = 1;
   m_core_info.time_us = 0ULL;

   /** \action
    * Call RSPP_Process_Detections with zero timestamp
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and detection should be processed correctly despite zero timestamp
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(1U, m_detection_list.number_of_valid_detections);
   CHECK(m_detection_list.detections[0].processed.f_ok_to_use);
   // Calculate and verify expected VCS coordinates
   float32_t expected_vcs_x, expected_vcs_y, expected_vcs_z;
   calculate_expected_vcs_coordinates(
       TYPICAL_RANGE, TYPICAL_AZIMUTH, TYPICAL_ELEVATION,
       TYPICAL_LONGITUDINAL, TYPICAL_LATERAL, TYPICAL_HEIGHT,
       TYPICAL_BORESIGHT_AZ, VALID_POLARITY_POSITIVE,
       expected_vcs_x, expected_vcs_y, expected_vcs_z);
   DOUBLES_EQUAL(expected_vcs_x, m_detection_list.detections[0].processed.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(expected_vcs_y, m_detection_list.detections[0].processed.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(expected_vcs_z, m_detection_list.detections[0].processed.vcs_position_z, TOLERANCE);
}

/** \purpose
 * Test processing with maximum timestamp value
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Max_Timestamp)
{
   /** \step{1}
    * Verifies processing with the maximum possible timestamp value.
    */

   /** \precond
    * RSPP is fully initialized
    */
   setup_rspp_fully_initialized();
   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.number_of_valid_detections = 1;
   m_core_info.time_us = UINT64_MAX;

   /** \action
    * Call RSPP_Process_Detections with max timestamp
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and detection should be processed correctly despite max timestamp
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(1U, m_detection_list.number_of_valid_detections);
   CHECK(m_detection_list.detections[0].processed.f_ok_to_use);
   // Calculate and verify expected VCS coordinates
   float32_t expected_vcs_x, expected_vcs_y, expected_vcs_z;
   calculate_expected_vcs_coordinates(
       TYPICAL_RANGE, TYPICAL_AZIMUTH, TYPICAL_ELEVATION,
       TYPICAL_LONGITUDINAL, TYPICAL_LATERAL, TYPICAL_HEIGHT,
       TYPICAL_BORESIGHT_AZ, VALID_POLARITY_POSITIVE,
       expected_vcs_x, expected_vcs_y, expected_vcs_z);
   DOUBLES_EQUAL(expected_vcs_x, m_detection_list.detections[0].processed.vcs_position_x, TOLERANCE);
   DOUBLES_EQUAL(expected_vcs_y, m_detection_list.detections[0].processed.vcs_position_y, TOLERANCE);
   DOUBLES_EQUAL(expected_vcs_z, m_detection_list.detections[0].processed.vcs_position_z, TOLERANCE);
}

// ==============================================================================
// Negative Polarity Sensor Tests
// ==============================================================================

/** \purpose
 * Test processing with negative polarity sensor (flipped)
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Negative_Polarity_Sensor)
{
   /** \step{1}
    * Checks processing with a sensor that has negative polarity (flipped orientation).
    */

   /** \precond
    * RSPP is initialized with negative polarity sensor
    */
   RSPP_Initialize();

   ConstantProps_T sensor_calibration;
   memset(&sensor_calibration, 0, sizeof(sensor_calibration));
   sensor_calibration.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
   sensor_calibration.mounting_position.vcs_position.longitudinal = TYPICAL_LONGITUDINAL;
   sensor_calibration.mounting_position.vcs_position.lateral = TYPICAL_LATERAL;
   sensor_calibration.mounting_position.vcs_position.height = TYPICAL_HEIGHT;
   sensor_calibration.mounting_position.vcs_boresight_azimuth_angle = TYPICAL_BORESIGHT_AZ;

   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      sensor_calibration.fov_min_az_rad[look_id] = TYPICAL_FOV_MIN_AZ;
      sensor_calibration.fov_max_az_rad[look_id] = TYPICAL_FOV_MAX_AZ;
      sensor_calibration.fov_min_el_rad[look_id] = TYPICAL_FOV_MIN_EL;
      sensor_calibration.fov_max_el_rad[look_id] = TYPICAL_FOV_MAX_EL;
      sensor_calibration.r_wrapping[look_id] = TYPICAL_R_WRAPPING;
      sensor_calibration.v_wrapping[look_id] = TYPICAL_V_WRAPPING;
      sensor_calibration.range_limits[look_id] = TYPICAL_RANGE_LIMIT;
      sensor_calibration.min_aliaised_range_rate[look_id] = TYPICAL_MIN_ALIASED_RR;
   }

   // Set up sensor array: first sensor with negative polarity, rest with positive polarity
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));

   // First sensor has negative polarity
   sensor_array[0].constant = sensor_calibration;
   sensor_array[0].constant.id = VALID_SENSOR_ID_MIN;
   sensor_array[0].constant.polarity = -1;

   // Rest have positive polarity
   for (uint8_t sensor_id = VALID_SENSOR_ID_MIN + 1; sensor_id <= VALID_SENSOR_ID_MAX; ++sensor_id)
   {
      sensor_array[sensor_id - 1].constant = sensor_calibration;
      sensor_array[sensor_id - 1].constant.id = sensor_id;
      sensor_array[sensor_id - 1].constant.polarity = 1;
   }
   RSPP_Set_Sensor_Calibrations(sensor_array);

   // Create detection from flipped sensor
   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[0].raw.azimuth = 0.5F;
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and azimuth should be flipped
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   // VCS azimuth should be flipped (negative of raw)
   DOUBLES_EQUAL(-0.5F, m_detection_list.detections[0].processed.vcs_az, TOLERANCE);
}

// ==============================================================================
// Consecutive Processing Tests
// ==============================================================================

/** \purpose
 * Test multiple consecutive calls to Process_Detections
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Consecutive_Calls)
{
   /** \step{1}
    * Verifies multiple consecutive calls to RSPP_Process_Detections work as expected.
    */

   /** \precond
    * RSPP is fully initialized
    */
   setup_rspp_fully_initialized();

   /** \action
    * Call RSPP_Process_Detections multiple times
    */
   for (uint32_t iteration = 0; iteration < 10; ++iteration)
   {
      setup_empty_detection_list();
      create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
      m_detection_list.detections[0].raw.range = TYPICAL_RANGE + static_cast<float32_t>(iteration);
      m_detection_list.number_of_valid_detections = 1;
      m_core_info.time_us = TYPICAL_TIME_US + iteration * 10000ULL;

      RSPP_Return_Type_T result = RSPP_Process_Detections(
          m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

      CHECK_EQUAL(RSPP_E_OK, result);
      CHECK_EQUAL(1U, m_detection_list.number_of_valid_detections);
      CHECK(m_detection_list.detections[0].processed.f_ok_to_use);
      // Calculate and verify expected VCS coordinates for varying range
      float32_t expected_vcs_x, expected_vcs_y, expected_vcs_z;
      const float32_t current_range = TYPICAL_RANGE + static_cast<float32_t>(iteration);
      calculate_expected_vcs_coordinates(
          current_range, TYPICAL_AZIMUTH, TYPICAL_ELEVATION,
          TYPICAL_LONGITUDINAL, TYPICAL_LATERAL, TYPICAL_HEIGHT,
          TYPICAL_BORESIGHT_AZ, VALID_POLARITY_POSITIVE,
          expected_vcs_x, expected_vcs_y, expected_vcs_z);
      DOUBLES_EQUAL(expected_vcs_x, m_detection_list.detections[0].processed.vcs_position_x, TOLERANCE);
      DOUBLES_EQUAL(expected_vcs_y, m_detection_list.detections[0].processed.vcs_position_y, TOLERANCE);
   }
}

/** \purpose
 * Test that processing clears sorted indices for new batch
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Sorted_Indices_Reset_Each_Call)
{
   /** \step{1}
    * Checks that sorted indices are reset and recalculated for each call.
    */

   /** \precond
    * RSPP is fully initialized
    */
   setup_rspp_fully_initialized();

   // First call with multiple detections
   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[0].raw.range = 50.0F;
   create_valid_detection(m_detection_list.detections[1], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[1].raw.range = 100.0F;
   m_detection_list.number_of_valid_detections = 2;

   RSPP_Process_Detections(m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   int16_t first_call_min = m_detection_list.vcslong_det_idx_min;
   int16_t first_call_max = m_detection_list.vcslong_det_idx_max;

   // Verify first call produced valid sorted indices for 2 detections
   CHECK(first_call_min != RSPP_INVALID_ID);
   CHECK(first_call_max != RSPP_INVALID_ID);
   CHECK(first_call_min != first_call_max); // min and max should differ for 2 detections

   // Second call with single detection
   setup_empty_detection_list();
   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[0].raw.range = 75.0F;
   m_detection_list.number_of_valid_detections = 1;
   m_core_info.time_us = TYPICAL_TIME_US + 10000ULL;

   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Sorted indices should be recalculated for new detection set
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   // For single detection, min and max should be the same index
   CHECK_EQUAL(m_detection_list.vcslong_det_idx_min, m_detection_list.vcslong_det_idx_max);
}

// ==============================================================================
// Additional Edge Case Tests
// ==============================================================================

/** \purpose
 * Test processing with det_id of zero (invalid)
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Invalid_Det_ID_Zero)
{
   /** \step{1}
    * Verifies processing with a detection that has det_id = 0 (invalid).
    */

   /** \precond
    * RSPP is fully initialized with detection that has det_id = 0
    */
   setup_rspp_fully_initialized();
   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[0].raw.det_id = 0; // Invalid det_id
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed but detection should be marked as not ok to use
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   // Detection with invalid det_id should be flagged
   CHECK_FALSE(m_detection_list.detections[0].processed.f_ok_to_use);
}

/** \purpose
 * Test processing with negative det_id (invalid)
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Invalid_Det_ID_Negative)
{
   /** \step{1}
    * Checks processing with a detection that has a negative det_id (invalid).
    */

   /** \precond
    * RSPP is fully initialized with detection that has negative det_id
    */
   setup_rspp_fully_initialized();
   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[0].raw.det_id = -1; // Invalid det_id
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed but detection should be marked as not ok to use
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   // Detection with invalid det_id should be flagged
   CHECK_FALSE(m_detection_list.detections[0].processed.f_ok_to_use);
}

/** \purpose
 * Test processing with negative range (invalid)
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Negative_Range)
{
   /** \step{1}
    * Verifies processing with a detection that has a negative range (invalid).
    */

   /** \precond
    * RSPP is fully initialized with detection that has negative range
    */
   setup_rspp_fully_initialized();
   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[0].raw.range = -10.0F; // Invalid negative range
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed but detection should be marked as not ok to use
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   // Detection with invalid range should be flagged
   CHECK_FALSE(m_detection_list.detections[0].processed.f_ok_to_use);
}

/** \purpose
 * Test that VCS elevation is populated correctly
 * \req
 * CPR-7939_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_VCS_Elevation_Calculated)
{
   /** \step{1}
    * Checks that VCS elevation is calculated and populated correctly.
    */

   /** \precond
    * RSPP is fully initialized with detection that has non-zero elevation
    */
   setup_rspp_fully_initialized();
   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[0].raw.elevation = 0.1F; // 0.1 rad elevation
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * VCS elevation should be populated (compensated by polarity)
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK(m_detection_list.detections[0].processed.f_ok_to_use);
   // VCS elevation should be populated (with polarity = 1, should be same as raw)
   DOUBLES_EQUAL(0.1F, m_detection_list.detections[0].processed.vcs_el, TOLERANCE);
}

/** \purpose
 * Test processing with detection from different sensor types
 * \req
 * CPR-7939_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Multiple_Sensor_Types)
{
   /** \step{1}
    * Verifies processing with detections from different sensor types/positions.
    */

   /** \precond
    * RSPP is fully initialized with detections from different sensor positions
    */
   setup_rspp_fully_initialized();

   // Create detections from sensors at different positions (front vs corner)
   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[0].raw.range = 50.0F;
   m_detection_list.detections[0].raw.azimuth = 0.0F; // Straight ahead

   create_valid_detection(m_detection_list.detections[1], VALID_SENSOR_ID_MIN + 1);
   m_detection_list.detections[1].raw.range = 30.0F;
   m_detection_list.detections[1].raw.azimuth = 0.3F; // Off-boresight

   m_detection_list.number_of_valid_detections = 2;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and both detections should be processed correctly
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK(m_detection_list.detections[0].processed.f_ok_to_use);
   CHECK(m_detection_list.detections[1].processed.f_ok_to_use);
   CHECK_EQUAL(2U, m_detection_list.number_of_valid_detections);
   // Both detections should have valid VCS positions
   CHECK(m_detection_list.detections[0].processed.vcs_position_x > 0.0F);
   CHECK(m_detection_list.detections[1].processed.vcs_position_x > 0.0F);
}

// ==============================================================================
// Host Dynamics Tests
// ==============================================================================

/** \purpose
 * Test processing with high host speed
 * \req
 * CPR-7942_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_High_Host_Speed)
{
   /** \step{1}
    * Checks processing with high host speed and range rate compensation.
    */

   /** \precond
    * RSPP is fully initialized with high host speed
    */
   setup_rspp_fully_initialized();
   m_vehicle_state_data.speed = 50.0F; // 180 km/h
   m_vehicle_state_data.vcs_speed = 50.0F;

   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[0].raw.range_rate = -50.0F; // Stationary target
   m_detection_list.detections[0].raw.azimuth = 0.0F;
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and detection should be processed
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK(m_detection_list.detections[0].processed.f_ok_to_use);
   // Range rate should be compensated
   CHECK(m_detection_list.detections[0].processed.range_rate_compensated !=
         m_detection_list.detections[0].raw.range_rate);
}

/** \purpose
 * Test processing with host yaw rate (turning vehicle)
 * \req
 * CPR-3848_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Host_With_Yaw_Rate)
{
   /** \step{1}
    * Verifies processing with non-zero host yaw rate (turning vehicle).
    */

   /** \precond
    * RSPP is fully initialized with non-zero yaw rate
    */
   setup_rspp_fully_initialized();
   m_vehicle_state_data.speed = 20.0F;
   m_vehicle_state_data.vcs_speed = 20.0F;
   m_vehicle_state_data.yaw_rate_rad = 0.1F; // Turning left

   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[0].raw.range_rate = -20.0F;
   m_detection_list.detections[0].raw.azimuth = 0.0F;
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed - yaw rate affects motion status calculation
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK(m_detection_list.detections[0].processed.f_ok_to_use);
   // Motion status should still be valid
   CHECK(m_detection_list.detections[0].processed.motion_status >=
         static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_INVALID));
   CHECK(m_detection_list.detections[0].processed.motion_status <=
         static_cast<int8_t>(rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS));
}

/** \purpose
 * Test processing with host acceleration
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Host_With_Acceleration)
{
   /** \step{1}
    * Checks processing with host vehicle acceleration.
    */

   /** \precond
    * RSPP is fully initialized with accelerating host
    */
   setup_rspp_fully_initialized();
   m_vehicle_state_data.speed = 20.0F;
   m_vehicle_state_data.vcs_speed = 20.0F;
   m_vehicle_state_data.acceleration = 3.0F; // Accelerating

   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK(m_detection_list.detections[0].processed.f_ok_to_use);
}

// ==============================================================================
// Boresight Angle Tests
// ==============================================================================

/** \purpose
 * Test processing with non-zero boresight angle sensor
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Non_Zero_Boresight_Angle)
{
   /** \step{1}
    * Verifies processing with a sensor having a non-zero boresight angle.
    */

   /** \precond
    * RSPP is initialized with sensor having 45 degree boresight angle
    */
   RSPP_Initialize();

   ConstantProps_T sensor_calibration;
   memset(&sensor_calibration, 0, sizeof(sensor_calibration));
   sensor_calibration.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
   sensor_calibration.polarity = VALID_POLARITY_POSITIVE;
   sensor_calibration.mounting_position.vcs_position.longitudinal = TYPICAL_LONGITUDINAL;
   sensor_calibration.mounting_position.vcs_position.lateral = TYPICAL_LATERAL;
   sensor_calibration.mounting_position.vcs_position.height = TYPICAL_HEIGHT;
   // 45 degree boresight angle (corner sensor looking diagonally)
   sensor_calibration.mounting_position.vcs_boresight_azimuth_angle = 0.7854F; // PI/4

   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      sensor_calibration.fov_min_az_rad[look_id] = TYPICAL_FOV_MIN_AZ;
      sensor_calibration.fov_max_az_rad[look_id] = TYPICAL_FOV_MAX_AZ;
      sensor_calibration.fov_min_el_rad[look_id] = TYPICAL_FOV_MIN_EL;
      sensor_calibration.fov_max_el_rad[look_id] = TYPICAL_FOV_MAX_EL;
      sensor_calibration.r_wrapping[look_id] = TYPICAL_R_WRAPPING;
      sensor_calibration.v_wrapping[look_id] = TYPICAL_V_WRAPPING;
      sensor_calibration.range_limits[look_id] = TYPICAL_RANGE_LIMIT;
      sensor_calibration.min_aliaised_range_rate[look_id] = TYPICAL_MIN_ALIASED_RR;
   }

   // Set up sensor array: first sensor with boresight, rest with zero boresight
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));

   // First sensor with boresight
   sensor_array[0].constant = sensor_calibration;
   sensor_array[0].constant.id = VALID_SENSOR_ID_MIN;

   // Rest with zero boresight
   sensor_calibration.mounting_position.vcs_boresight_azimuth_angle = 0.0F;
   for (uint8_t sensor_id = VALID_SENSOR_ID_MIN + 1; sensor_id <= VALID_SENSOR_ID_MAX; ++sensor_id)
   {
      sensor_array[sensor_id - 1].constant = sensor_calibration;
      sensor_array[sensor_id - 1].constant.id = sensor_id;
   }
   RSPP_Set_Sensor_Calibrations(sensor_array);

   // Create detection at sensor boresight (azimuth = 0 in sensor frame)
   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[0].raw.azimuth = 0.0F; // Straight ahead in sensor frame
   m_detection_list.detections[0].raw.range = 50.0F;
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and detection should be processed with boresight configuration
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK(m_detection_list.detections[0].processed.f_ok_to_use);
   // VCS position X should be populated
   CHECK(m_detection_list.detections[0].processed.vcs_position_x > 0.0F);
}

// ==============================================================================
// Azimuth Boundary Tests
// ==============================================================================

/** \purpose
 * Test processing with detection at PI azimuth (rear-facing)
 * \req
 * CPR-7939_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_PI_Azimuth)
{
   /** \step{1}
    * Checks processing with a detection at PI azimuth (rear-facing).
    */

   /** \precond
    * RSPP is fully initialized
    */
   setup_rspp_fully_initialized();

   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[0].raw.azimuth = 3.14159F; // PI (rear-facing, outside typical FOV)
   m_detection_list.detections[0].raw.range = 50.0F;
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed - function handles out-of-FOV detections
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   // VCS X should be negative (behind sensor)
   CHECK(m_detection_list.detections[0].processed.vcs_position_x < TYPICAL_LONGITUDINAL);
}

/** \purpose
 * Test processing with detection at -PI azimuth
 * \req
 * CPR-7939_Derived CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_Process_Detections, Process_Detections_TC_Negative_PI_Azimuth)
{
   /** \step{1}
    * Verifies processing with a detection at -PI azimuth (rear-facing).
    */

   /** \precond
    * RSPP is fully initialized
    */
   setup_rspp_fully_initialized();

   create_valid_detection(m_detection_list.detections[0], VALID_SENSOR_ID_MIN);
   m_detection_list.detections[0].raw.azimuth = -3.14159F; // -PI
   m_detection_list.detections[0].raw.range = 50.0F;
   m_detection_list.number_of_valid_detections = 1;

   /** \action
    * Call RSPP_Process_Detections
    */
   RSPP_Return_Type_T result = RSPP_Process_Detections(
       m_dynamic_sensor_data, m_detection_list, m_vehicle_state_data, m_core_info);

   /** \result
    * Should succeed and produce similar result to +PI
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   // VCS X should be negative (behind sensor)
   CHECK(m_detection_list.detections[0].processed.vcs_position_x < TYPICAL_LONGITUDINAL);
}

/** @}*/
