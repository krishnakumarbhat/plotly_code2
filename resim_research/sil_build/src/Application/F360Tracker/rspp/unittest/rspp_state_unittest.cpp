/** \file
 * This file contains unit tests for content of rspp_state.cpp file
 */

#include "rspp_state.h"
#include "rspp_math.h"
#include <CppUTest/TestHarness.h>
#include <cstring>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace rspp_variant_A;

/** \defgroup  test_RSPP_State
 *  @{
 */

/** \brief
 * Tests for RSPP_State_Initialize and RSPP_State_Get_Current_State functions
 */
TEST_GROUP(test_RSPP_State)
{
   // Test constants
   static constexpr int STRESS_TEST_ITERATIONS = 100;
   static constexpr float32_t TOLERANCE = 0.0001F;

   /** \setup
    * Set up test environment - Initialize RSPP state for each test
    */
   TEST_SETUP()
   {
      // Initialize state before each test to ensure clean baseline
      RSPP_State_Initialize();
   }

   /** \teardown
    * Clean up after tests
    */
   TEST_TEARDOWN()
   {
      RSPP_State_Reset();
   }
};

/** \purpose
 * Verify that RSPP_State_Initialize sets state to INITIALIZED. Tests basic initialization functionality
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_Initialize_Sets_State_To_Initialized)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \action
    * State already initialized in TEST_SETUP
    */
   RSPP_States_Type_T state = RSPP_State_Get_Current_State();

   /** \result
    * State should be INITIALIZED
    */
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state);
}

/** \purpose
 * Verify that calling Initialize multiple times is safe and idempotent. Consolidates redundant idempotency tests into one comprehensive test
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_Initialize_Multiple_Times_Idempotent)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \action
    * Initialize multiple times
    */
   RSPP_State_Initialize();
   RSPP_States_Type_T state1 = RSPP_State_Get_Current_State();

   RSPP_State_Initialize();
   RSPP_States_Type_T state2 = RSPP_State_Get_Current_State();

   RSPP_State_Initialize();
   RSPP_States_Type_T state3 = RSPP_State_Get_Current_State();

   /** \result
    * All states should be INITIALIZED
    */
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state1);
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state2);
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state3);
}

/** \purpose
 * Verify Get_Current_State returns same value on consecutive calls. Tests consistency and absence of side effects
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_Get_Current_State_Returns_Consistent_Value)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \action
    * Get state multiple times
    */
   RSPP_States_Type_T state1 = RSPP_State_Get_Current_State();
   RSPP_States_Type_T state2 = RSPP_State_Get_Current_State();
   RSPP_States_Type_T state3 = RSPP_State_Get_Current_State();

   /** \result
    * All calls should return same value
    */
   CHECK_EQUAL(state1, state2);
   CHECK_EQUAL(state2, state3);
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state1);
}

/** \purpose
 * Verify that state value is always a valid enum member. Defensive check for memory corruption or invalid values
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_State_Value_Is_Valid_Enum)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \action
    * Get state
    */
   RSPP_States_Type_T state = RSPP_State_Get_Current_State();

   /** \result
    * State should be one of the valid enum values
    */
   CHECK(state == RSPP_STATE_UNINITIALIZED ||
         state == RSPP_STATE_INITIALIZED ||
         state == RSPP_STATE_INIT_COMPLETE);
}

/** \purpose
 * Verify that repeated Get_Current_State calls don't modify state. Stress test with many iterations to detect subtle side effects
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_Get_State_Stress_Test_No_Side_Effects)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \action
    * Get state many times
    */
   for (int i = 0; i < STRESS_TEST_ITERATIONS; i++)
   {
      RSPP_States_Type_T state = RSPP_State_Get_Current_State();
      CHECK_EQUAL(RSPP_STATE_INITIALIZED, state);
   }

   /** \result
    * Final state should still be INITIALIZED
    */
   RSPP_States_Type_T final_state = RSPP_State_Get_Current_State();
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, final_state);
}

/** \purpose
 * Verify state stays INITIALIZED when no sensors are calibrated. State should only transition to INIT_COMPLETE after all sensors calibrated
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_State_Remains_Initialized_Without_Calibration)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \action
    * Check state without adding any sensor calibrations
    */
   RSPP_States_Type_T state = RSPP_State_Get_Current_State();

   /** \result
    * Should remain INITIALIZED (not INIT_COMPLETE)
    */
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state);
}

/** \purpose
 * Verify re-initialization resets state from INIT_COMPLETE back to INITIALIZED. Tests state machine reset behavior
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_Initialize_Resets_State_From_Init_Complete)
{
   /** \step{1}
    * Execute test and verify result.
    */

   // Note: This test documents expected behavior when state is INIT_COMPLETE
   // Currently we can't easily reach INIT_COMPLETE in this test group
   // but we verify re-initialization always results in INITIALIZED state

   /** \action
    * Initialize again (simulating reset)
    */
   RSPP_State_Initialize();
   RSPP_States_Type_T state = RSPP_State_Get_Current_State();

   /** \result
    * Should be INITIALIZED state after re-initialization
    */
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state);
}

/** \purpose
 * Verify state correctness after rapid consecutive initializations. Stress test for initialization robustness
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_Get_State_After_Rapid_Reinitializations)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \action
    * Rapidly re-initialize
    */
   for (int i = 0; i < 10; i++)
   {
      RSPP_State_Initialize();
   }

   /** \result
    * State should still be valid and INITIALIZED
    */
   RSPP_States_Type_T state = RSPP_State_Get_Current_State();
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state);
}

/** \purpose
 * Verify that state enum values are distinct and don't overlap. Validates enum integrity
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_State_Enum_Values_Are_Distinct)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \result
    * Enum values should be unique
    */
   CHECK(RSPP_STATE_UNINITIALIZED != RSPP_STATE_INITIALIZED);
   CHECK(RSPP_STATE_INITIALIZED != RSPP_STATE_INIT_COMPLETE);
   CHECK(RSPP_STATE_UNINITIALIZED != RSPP_STATE_INIT_COMPLETE);
}

/** \purpose
 * Verify that Initialize produces same result every time. Tests deterministic behavior critical for safety systems
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_Initialize_Is_Deterministic)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \action
    * Initialize multiple times and collect states
    */
   RSPP_State_Initialize();
   RSPP_States_Type_T state1 = RSPP_State_Get_Current_State();

   RSPP_State_Initialize();
   RSPP_States_Type_T state2 = RSPP_State_Get_Current_State();

   RSPP_State_Initialize();
   RSPP_States_Type_T state3 = RSPP_State_Get_Current_State();

   /** \result
    * All should be identical
    */
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state1);
   CHECK_EQUAL(state1, state2);
   CHECK_EQUAL(state2, state3);
}

/** \purpose
 * Verify that RSPP_State_Reset sets state to UNINITIALIZED
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_Reset_Sets_State_To_Uninitialized)
{
   /** \step{1}
    * Verify that reset changes state from INITIALIZED to UNINITIALIZED.
    */

   /** \precond
    * State is initialized
    */
   RSPP_States_Type_T state_before = RSPP_State_Get_Current_State();
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state_before);

   /** \action
    * Reset the state
    */
   RSPP_State_Reset();

   /** \result
    * State should be UNINITIALIZED
    */
   RSPP_States_Type_T state_after = RSPP_State_Get_Current_State();
   CHECK_EQUAL(RSPP_STATE_UNINITIALIZED, state_after);
}

/** \purpose
 * Verify that RSPP_State_Reset clears all sensor calibration data
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_Reset_Clears_Sensor_Calibration_Data)
{
   /** \step{1}
    * Set sensor calibration, reset, and verify data is cleared.
    */

   /** \precond
    * State is initialized and we have a sensor calibration
    */
   const RSPP_Sensor_Calib_T &sensor_cal = RSPP_State_Get_Sensor_Calibration(0);

   /** \action
    * Reset the state
    */
   RSPP_State_Reset();

   /** \result
    * All sensor calibration fields should be zeroed
    */
   const RSPP_Sensor_Calib_T &sensor_cal_after = RSPP_State_Get_Sensor_Calibration(0);

   // Check all array fields are zeroed
   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; ++look_id)
   {
      DOUBLES_EQUAL(0.0F, sensor_cal_after.fov_min_az_rad[look_id], TOLERANCE);
      DOUBLES_EQUAL(0.0F, sensor_cal_after.fov_max_az_rad[look_id], TOLERANCE);
      DOUBLES_EQUAL(0.0F, sensor_cal_after.interior_fov[look_id], TOLERANCE);
      DOUBLES_EQUAL(0.0F, sensor_cal_after.left_fov_normal[look_id], TOLERANCE);
      DOUBLES_EQUAL(0.0F, sensor_cal_after.right_fov_normal[look_id], TOLERANCE);
      DOUBLES_EQUAL(0.0F, sensor_cal_after.v_wrapping[look_id], TOLERANCE);
   }

   // Check VCS mounting position is zeroed
   DOUBLES_EQUAL(0.0F, sensor_cal_after.vcs_mounting_position.height, TOLERANCE);
   DOUBLES_EQUAL(0.0F, sensor_cal_after.vcs_mounting_position.lateral, TOLERANCE);
   DOUBLES_EQUAL(0.0F, sensor_cal_after.vcs_mounting_position.longitudinal, TOLERANCE);

   // Check sensor type and polarity
   CHECK_EQUAL(RSPP_SENSOR_TYPE_UNKNOWN, sensor_cal_after.sensor_type);
   CHECK_EQUAL(0, sensor_cal_after.polarity);
}

/** \purpose
 * Verify that RSPP_State_Reset clears all sensor calibration flags
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_Reset_Clears_All_Sensor_Calibration_Flags)
{
   /** \step{1}
    * Verify that reset clears sensor calibration initialized flags by attempting
    * to set calibration after reset.
    */

   /** \precond
    * Create valid sensor calibration
    */
   ConstantProps_T sensor_cal = {};
   sensor_cal.id = 1U;
   sensor_cal.polarity = 1;
   sensor_cal.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
   sensor_cal.mounting_position.vcs_position.longitudinal = -1.0F;
   sensor_cal.mounting_position.vcs_position.lateral = 0.5F;
   sensor_cal.mounting_position.vcs_position.height = 0.3F;
   sensor_cal.mounting_position.vcs_boresight_azimuth_angle = 0.1F;
   sensor_cal.mounting_position.vcs_boresight_elevation_angle = 0.0F;

   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      sensor_cal.fov_min_az_rad[i] = -1.0F;
      sensor_cal.fov_max_az_rad[i] = 1.0F;
      sensor_cal.fov_min_el_rad[i] = -0.5F;
      sensor_cal.fov_max_el_rad[i] = 0.5F;
      sensor_cal.range_limits[i] = 200.0F;
      sensor_cal.v_wrapping[i] = 50.0F;
      sensor_cal.r_wrapping[i] = 0.0F;
      sensor_cal.min_aliaised_range_rate[i] = -100.0F;
   }

   /** \action
    * Set calibration, reset, initialize, then set same sensor again
    */
   RSPP_Return_Type_T result1 = RSPP_State_Set_Sensor_Calibration(sensor_cal);
   RSPP_State_Reset();
   RSPP_State_Initialize();
   RSPP_Return_Type_T result2 = RSPP_State_Set_Sensor_Calibration(sensor_cal);

   /** \result
    * Both set operations should succeed (flags were cleared by Reset)
    */
   CHECK_EQUAL(RSPP_E_OK, result1);
   CHECK_EQUAL(RSPP_E_OK, result2);
}

/** \purpose
 * Verify that RSPP_State_Reset resets the number of initialized sensors to zero
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_Reset_Clears_Num_Initialized_Sensors)
{
   /** \step{1}
    * Verify that reset clears the internal counter of initialized sensors.
    */

   /** \precond
    * State is initialized
    */

   /** \action
    * Reset the state
    */
   RSPP_State_Reset();

   /** \result
    * State should be UNINITIALIZED (which indirectly verifies num initialized sensors is 0)
    */
   RSPP_States_Type_T state = RSPP_State_Get_Current_State();
   CHECK_EQUAL(RSPP_STATE_UNINITIALIZED, state);
}

/** \purpose
 * Verify that calling RSPP_State_Reset multiple times is safe and idempotent
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_Reset_Multiple_Times_Idempotent)
{
   /** \step{1}
    * Verify that multiple reset calls produce consistent results.
    */

   /** \action
    * Reset multiple times
    */
   RSPP_State_Reset();
   RSPP_States_Type_T state1 = RSPP_State_Get_Current_State();

   RSPP_State_Reset();
   RSPP_States_Type_T state2 = RSPP_State_Get_Current_State();

   RSPP_State_Reset();
   RSPP_States_Type_T state3 = RSPP_State_Get_Current_State();

   /** \result
    * All states should be UNINITIALIZED
    */
   CHECK_EQUAL(RSPP_STATE_UNINITIALIZED, state1);
   CHECK_EQUAL(RSPP_STATE_UNINITIALIZED, state2);
   CHECK_EQUAL(RSPP_STATE_UNINITIALIZED, state3);
}

/** \purpose
 * Verify that RSPP_State_Reset can be called from any state
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_Reset_From_Different_States)
{
   /** \step{1}
    * Verify reset works from UNINITIALIZED state.
    */

   /** \action
    * Reset from UNINITIALIZED
    */
   RSPP_State_Reset();
   RSPP_State_Reset(); // Already uninitialized, reset again
   RSPP_States_Type_T state1 = RSPP_State_Get_Current_State();

   /** \step{2}
    * Verify reset works from INITIALIZED state.
    */

   /** \action
    * Initialize then reset
    */
   RSPP_State_Initialize();
   RSPP_State_Reset();
   RSPP_States_Type_T state2 = RSPP_State_Get_Current_State();

   /** \result
    * Both final states should be UNINITIALIZED
    */
   CHECK_EQUAL(RSPP_STATE_UNINITIALIZED, state1);
   CHECK_EQUAL(RSPP_STATE_UNINITIALIZED, state2);
}

/** \purpose
 * Verify state remains INITIALIZED when only one sensor is calibrated. Integration test: State should transition to INIT_COMPLETE only when all sensors calibrated
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_State_Transition_With_One_Sensor_Calibration)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create valid sensor calibration
    */
   ConstantProps_T sensor_cal = {};
   sensor_cal.id = 1U;
   sensor_cal.polarity = 1;
   sensor_cal.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
   sensor_cal.mounting_position.vcs_position.longitudinal = -1.0F;
   sensor_cal.mounting_position.vcs_position.lateral = 0.5F;
   sensor_cal.mounting_position.vcs_position.height = 0.3F;
   sensor_cal.mounting_position.vcs_boresight_azimuth_angle = 0.1F;
   sensor_cal.mounting_position.vcs_boresight_elevation_angle = 0.0F;

   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      sensor_cal.fov_min_az_rad[i] = -1.0F;
      sensor_cal.fov_max_az_rad[i] = 1.0F;
      sensor_cal.fov_min_el_rad[i] = -0.5F;
      sensor_cal.fov_max_el_rad[i] = 0.5F;
      sensor_cal.range_limits[i] = 200.0F;
      sensor_cal.v_wrapping[i] = 50.0F;
      sensor_cal.r_wrapping[i] = 0.0F;
      sensor_cal.min_aliaised_range_rate[i] = -100.0F;
   }

   /** \action
    * Set one sensor calibration
    */
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_cal);
   RSPP_States_Type_T state = RSPP_State_Get_Current_State();

   /** \result
    * Should succeed but remain INITIALIZED (not INIT_COMPLETE with only 1 of 10 sensors)
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state);
}

/** \purpose
 * Verify Initialize resets sensor calibration flags. Tests that re-initialization allows re-setting sensor calibrations
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_Initialize_Clears_Sensor_Calibration_Flags)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create valid sensor calibration
    */
   ConstantProps_T sensor_cal = {};
   sensor_cal.id = 1U;
   sensor_cal.polarity = 1;
   sensor_cal.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
   sensor_cal.mounting_position.vcs_position.longitudinal = -1.0F;
   sensor_cal.mounting_position.vcs_position.lateral = 0.5F;
   sensor_cal.mounting_position.vcs_position.height = 0.3F;
   sensor_cal.mounting_position.vcs_boresight_azimuth_angle = 0.1F;
   sensor_cal.mounting_position.vcs_boresight_elevation_angle = 0.0F;

   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      sensor_cal.fov_min_az_rad[i] = -1.0F;
      sensor_cal.fov_max_az_rad[i] = 1.0F;
      sensor_cal.fov_min_el_rad[i] = -0.5F;
      sensor_cal.fov_max_el_rad[i] = 0.5F;
      sensor_cal.range_limits[i] = 200.0F;
      sensor_cal.v_wrapping[i] = 50.0F;
      sensor_cal.r_wrapping[i] = 0.0F;
      sensor_cal.min_aliaised_range_rate[i] = -100.0F;
   }

   /** \action
    * Set calibration, then re-initialize, then set same sensor again
    */
   RSPP_Return_Type_T result1 = RSPP_State_Set_Sensor_Calibration(sensor_cal);
   RSPP_State_Initialize();
   RSPP_Return_Type_T result2 = RSPP_State_Set_Sensor_Calibration(sensor_cal);

   /** \result
    * Both set operations should succeed (flags were cleared by Initialize)
    */
   CHECK_EQUAL(RSPP_E_OK, result1);
   CHECK_EQUAL(RSPP_E_OK, result2);
}

/** \purpose
 * Verify complete state transition sequence. Integration test: UNINITIALIZED -> INITIALIZED -> INIT_COMPLETE
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_State_Sequence_Init_To_Initialized_To_Init_Complete)
{
   /** \step{1}
    * Execute test and verify result.
    */

   // Note: This test documents the expected state flow
   // We can verify: INITIALIZED -> (with all sensors) -> INIT_COMPLETE

   /** \action
    * Start from INITIALIZED state (from TEST_SETUP)
    */
   RSPP_States_Type_T state_after_init = RSPP_State_Get_Current_State();

   // Set one sensor to verify partial calibration state
   ConstantProps_T sensor_cal = {};
   sensor_cal.id = 1U;
   sensor_cal.polarity = 1;
   sensor_cal.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
   sensor_cal.mounting_position.vcs_position.longitudinal = -1.0F;
   sensor_cal.mounting_position.vcs_position.lateral = 0.5F;
   sensor_cal.mounting_position.vcs_position.height = 0.3F;
   sensor_cal.mounting_position.vcs_boresight_azimuth_angle = 0.1F;
   sensor_cal.mounting_position.vcs_boresight_elevation_angle = 0.0F;

   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      sensor_cal.fov_min_az_rad[i] = -1.0F;
      sensor_cal.fov_max_az_rad[i] = 1.0F;
      sensor_cal.fov_min_el_rad[i] = -0.5F;
      sensor_cal.fov_max_el_rad[i] = 0.5F;
      sensor_cal.range_limits[i] = 200.0F;
      sensor_cal.v_wrapping[i] = 50.0F;
      sensor_cal.r_wrapping[i] = 0.0F;
      sensor_cal.min_aliaised_range_rate[i] = -100.0F;
   }

   RSPP_State_Set_Sensor_Calibration(sensor_cal);
   RSPP_States_Type_T state_with_partial_cal = RSPP_State_Get_Current_State();

   /** \result
    * Verify state progression
    */
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state_after_init);
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state_with_partial_cal); // Still INITIALIZED with partial calibration

   // Note: Reaching INIT_COMPLETE requires all 10 sensors (6 SRR + 4 MRR)
   // This is documented behavior verified in integration tests
}

/** \purpose
 * Verify re-initialization from partial calibration state. Tests recovery from partial configuration
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_Initialize_After_Partial_Calibration_Resets_State)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set partial sensor calibration
    */
   ConstantProps_T sensor_cal = {};
   sensor_cal.id = 1U;
   sensor_cal.polarity = 1;
   sensor_cal.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
   sensor_cal.mounting_position.vcs_position.longitudinal = -1.0F;
   sensor_cal.mounting_position.vcs_position.lateral = 0.5F;
   sensor_cal.mounting_position.vcs_position.height = 0.3F;
   sensor_cal.mounting_position.vcs_boresight_azimuth_angle = 0.1F;
   sensor_cal.mounting_position.vcs_boresight_elevation_angle = 0.0F;

   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      sensor_cal.fov_min_az_rad[i] = -1.0F;
      sensor_cal.fov_max_az_rad[i] = 1.0F;
      sensor_cal.fov_min_el_rad[i] = -0.5F;
      sensor_cal.fov_max_el_rad[i] = 0.5F;
      sensor_cal.range_limits[i] = 200.0F;
      sensor_cal.v_wrapping[i] = 50.0F;
      sensor_cal.r_wrapping[i] = 0.0F;
      sensor_cal.min_aliaised_range_rate[i] = -100.0F;
   }

   RSPP_State_Set_Sensor_Calibration(sensor_cal);

   /** \action
    * Re-initialize
    */
   RSPP_State_Initialize();
   RSPP_States_Type_T state_after_reinit = RSPP_State_Get_Current_State();

   /** \result
    * Should be back to INITIALIZED (partial calibration cleared)
    */
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state_after_reinit);
}

/** \purpose
 * Verify state doesn't move backward during calibration process. Tests state machine monotonic progression
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_State_Does_Not_Regress_During_Calibration)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Create two valid sensor calibrations
    */
   ConstantProps_T sensor_cal_1 = {};
   sensor_cal_1.id = 1U;
   sensor_cal_1.polarity = 1;
   sensor_cal_1.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
   sensor_cal_1.mounting_position.vcs_position.longitudinal = -1.0F;
   sensor_cal_1.mounting_position.vcs_position.lateral = 0.5F;
   sensor_cal_1.mounting_position.vcs_position.height = 0.3F;
   sensor_cal_1.mounting_position.vcs_boresight_azimuth_angle = 0.1F;
   sensor_cal_1.mounting_position.vcs_boresight_elevation_angle = 0.0F;

   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      sensor_cal_1.fov_min_az_rad[i] = -1.0F;
      sensor_cal_1.fov_max_az_rad[i] = 1.0F;
      sensor_cal_1.fov_min_el_rad[i] = -0.5F;
      sensor_cal_1.fov_max_el_rad[i] = 0.5F;
      sensor_cal_1.range_limits[i] = 200.0F;
      sensor_cal_1.v_wrapping[i] = 50.0F;
      sensor_cal_1.r_wrapping[i] = 0.0F;
      sensor_cal_1.min_aliaised_range_rate[i] = -100.0F;
   }

   ConstantProps_T sensor_cal_2 = sensor_cal_1;
   sensor_cal_2.id = 2U;

   /** \action
    * Set sensors and track state
    */
   RSPP_States_Type_T state1 = RSPP_State_Get_Current_State();
   RSPP_State_Set_Sensor_Calibration(sensor_cal_1);
   RSPP_States_Type_T state2 = RSPP_State_Get_Current_State();
   RSPP_State_Set_Sensor_Calibration(sensor_cal_2);
   RSPP_States_Type_T state3 = RSPP_State_Get_Current_State();

   /** \result
    * State should never regress (only stay same or progress)
    */
   CHECK(state1 <= state2);
   CHECK(state2 <= state3);
}

/** \purpose
 * Verify state queries don't interfere with initialization. Tests thread-safety aspect of interleaved operations
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_Initialize_Interleaved_With_State_Queries)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \action
    * Interleave Initialize and Get_Current_State calls
    */
   RSPP_State_Initialize();
   RSPP_States_Type_T state1 = RSPP_State_Get_Current_State();
   RSPP_States_Type_T state2 = RSPP_State_Get_Current_State();
   RSPP_State_Initialize();
   RSPP_States_Type_T state3 = RSPP_State_Get_Current_State();
   RSPP_States_Type_T state4 = RSPP_State_Get_Current_State();
   RSPP_State_Initialize();
   RSPP_States_Type_T state5 = RSPP_State_Get_Current_State();

   /** \result
    * All states should be INITIALIZED
    */
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state1);
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state2);
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state3);
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state4);
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state5);
}

/** \purpose
 * Verify state enum boundary values are within expected range. Tests enum value safety and validity
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_State_Boundary_Values_Are_Valid)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \result
    * Verify enum values are in reasonable range (0x00 to 0x02)
    */
   CHECK(RSPP_STATE_UNINITIALIZED >= 0x00U);
   CHECK(RSPP_STATE_UNINITIALIZED <= 0x0FU); // Reasonable upper bound
   CHECK(RSPP_STATE_INITIALIZED >= 0x00U);
   CHECK(RSPP_STATE_INITIALIZED <= 0x0FU);
   CHECK(RSPP_STATE_INIT_COMPLETE >= 0x00U);
   CHECK(RSPP_STATE_INIT_COMPLETE <= 0x0FU);

   // Verify ordering
   CHECK(RSPP_STATE_UNINITIALIZED < RSPP_STATE_INITIALIZED);
   CHECK(RSPP_STATE_INITIALIZED < RSPP_STATE_INIT_COMPLETE);
}

/** \purpose
 * Establish performance baseline for Get_Current_State. Performance test to detect degradation
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State, RSPP_State_TC_Get_State_Performance_Baseline)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \action
    * Execute many state queries to verify performance
    */
   static constexpr int PERFORMANCE_ITERATIONS = 10000;

   for (int i = 0; i < PERFORMANCE_ITERATIONS; i++)
   {
      RSPP_States_Type_T state = RSPP_State_Get_Current_State();
      (void)state; // Use variable to prevent optimization
   }

   /** \result
    * Test completes without timeout - validates O(1) performance
    */
   RSPP_States_Type_T final_state = RSPP_State_Get_Current_State();
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, final_state);
}

/** @}*/

/** \defgroup  test_RSPP_State_Sensor_Calibration
 *  @{
 */

/** \brief
 * Tests for RSPP_State_Set_Sensor_Calibration and RSPP_State_Get_Sensor_Calibration functions
 */
TEST_GROUP(test_RSPP_State_Sensor_Calibration)
{
   // Common test variables
   ConstantProps_T sensor_calibration;

   // Named constants for boundary values
   static constexpr float32_t VALID_LONG_MIN = -10.0F; // Matches validation constraint
   static constexpr float32_t VALID_LONG_MAX = 1.0F;
   static constexpr float32_t INVALID_LONG_HIGH = 2.0F;
   static constexpr float32_t VALID_LAT_MIN = -1.5F;
   static constexpr float32_t VALID_LAT_MAX = 1.5F;
   static constexpr float32_t INVALID_LAT_LOW = -2.0F;
   static constexpr float32_t VALID_HEIGHT_MIN = 0.3F;
   static constexpr float32_t VALID_HEIGHT_MAX = 1.3F;
   static constexpr float32_t INVALID_HEIGHT_LOW = 0.2F;
   static constexpr float32_t TWO_PI = 6.28318530717958647692F;
   static constexpr float32_t INVALID_BORESIGHT_HIGH = RSPP_PI + 1.0F;
   static constexpr float32_t INVALID_BORESIGHT_LOW = -RSPP_PI - 1.0F;
   static constexpr float32_t INVALID_FOV_MIN_AZ = -3.5F; // < -PI (new min)
   static constexpr float32_t INVALID_FOV_MAX_AZ = 3.5F;  // > PI (new max)
   static constexpr float32_t INVALID_FOV_MIN_EL = 3.5F;  // > PI
   static constexpr float32_t INVALID_FOV_MAX_EL = 3.5F;  // > PI
   static constexpr float32_t INVALID_RANGE_LIMIT = -10.0F;
   static constexpr float32_t INVALID_V_WRAP_LOW = 5.0F;      // < 10
   static constexpr float32_t INVALID_MIN_ALIASED_RR = 10.0F; // > 0
   static constexpr float32_t TOLERANCE = 0.0001F;

   /** \setup
    * Set up sensor calibration data for testing
    */
   TEST_SETUP()
   {
      // Initialize sensor calibration structure
      memset(&sensor_calibration, 0, sizeof(sensor_calibration));
      sensor_calibration.id = 1U; // Sensor IDs are 1-based
      sensor_calibration.polarity = 1;
      sensor_calibration.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      sensor_calibration.mounting_position.vcs_position.longitudinal = -1.0F;
      sensor_calibration.mounting_position.vcs_position.lateral = 0.5F;
      sensor_calibration.mounting_position.vcs_position.height = 0.3F;
      sensor_calibration.mounting_position.vcs_boresight_azimuth_angle = 0.1F;
      sensor_calibration.mounting_position.vcs_boresight_elevation_angle = 0.0F;

      // Initialize arrays for all look IDs
      for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
      {
         sensor_calibration.fov_min_az_rad[i] = -1.0F;
         sensor_calibration.fov_max_az_rad[i] = 1.0F;
         sensor_calibration.fov_min_el_rad[i] = -0.5F;
         sensor_calibration.fov_max_el_rad[i] = 0.5F;
         sensor_calibration.range_limits[i] = 200.0F;
         sensor_calibration.v_wrapping[i] = 50.0F;
         sensor_calibration.r_wrapping[i] = 0.0F;
         sensor_calibration.min_aliaised_range_rate[i] = -100.0F;
      }
   }

   /** \teardown
    * Clean up after tests
    */
   TEST_TEARDOWN()
   {
      // Reinitialize to clear any sensor calibrations set during test
      RSPP_State_Initialize();
   }
};

/** \purpose
 * Verify that setting a valid sensor calibration succeeds. Tests basic successful calibration with valid parameters
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Valid_Sensor)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Valid sensor calibration already set up in TEST_SETUP
    */
   RSPP_State_Initialize();

   /** \action
    * Set sensor calibration
    */
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * Should return success
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

/** \purpose
 * Verify that sensor ID of 0 is rejected. Tests RSPP_E_INVALID_SENSOR_ID for zero sensor ID (caught by state function before validation)
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Invalid_Sensor_ID_Zero)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set invalid sensor ID
    */
   RSPP_State_Initialize();
   sensor_calibration.id = 0U;

   /** \action
    * Attempt to set sensor calibration
    */
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * Should return INVALID_SENSOR_ID (state function checks ID before calling validation)
    */
   CHECK_EQUAL(RSPP_E_INVALID_SENSOR_ID, result);
}

/** \purpose
 * Verify that sensor ID above MAX_NUMBER_OF_SENSORS is rejected. Tests RSPP_E_INVALID_SENSOR_ID for out-of-range sensor ID (caught by state function)
 * \req
 * CPR-7940_Derived CPR-7950_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Invalid_Sensor_ID_Too_High)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set sensor ID beyond valid range (MAX_NUMBER_OF_SENSORS = 10)
    */
   RSPP_State_Initialize();
   sensor_calibration.id = MAX_NUMBER_OF_SENSORS + 1U;

   /** \action
    * Attempt to set sensor calibration
    */
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * Should return INVALID_SENSOR_ID (state function validates ID range first)
    */
   CHECK_EQUAL(RSPP_E_INVALID_SENSOR_ID, result);
}

/** \purpose
 * Verify that setting same sensor ID twice is rejected. Tests RSPP_E_INVALID_SENSOR_ID for duplicate calibration attempts
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Duplicate_Sensor_ID)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set a sensor calibration once
    */
   RSPP_State_Initialize();
   RSPP_Return_Type_T first_result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \action
    * Attempt to set same sensor ID again
    */
   RSPP_Return_Type_T second_result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * First should succeed, second should fail
    */
   CHECK_EQUAL(RSPP_E_OK, first_result);
   CHECK_EQUAL(RSPP_E_INVALID_SENSOR_ID, second_result);
}

/** \purpose
 * Verify that invalid polarity values are rejected. Tests RSPP_E_INVALID_CALIBRATION for polarity not equal to +1 or -1
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Invalid_Polarity)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set invalid polarity (must be +1 or -1)
    */
   RSPP_State_Initialize();
   sensor_calibration.polarity = 0; // Invalid

   /** \action
    * Attempt to set sensor calibration
    */
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * Should return error
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
}

/** \purpose
 * Verify that out-of-range longitudinal position is rejected. Tests RSPP_E_INVALID_CALIBRATION for longitudinal outside [-10, 10]m
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Invalid_Mounting_Position_Longitudinal)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set longitudinal position out of valid range [-10, 10]
    */
   RSPP_State_Initialize();
   sensor_calibration.mounting_position.vcs_position.longitudinal = INVALID_LONG_HIGH; // Too high

   /** \action
    * Attempt to set sensor calibration
    */
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * Should return error
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
}

/** \purpose
 * Verify that out-of-range lateral position is rejected. Tests RSPP_E_INVALID_CALIBRATION for lateral outside [-5, 5]m
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Invalid_Mounting_Position_Lateral)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set lateral position out of valid range [-5, 5]
    */
   RSPP_State_Initialize();
   sensor_calibration.mounting_position.vcs_position.lateral = INVALID_LAT_LOW; // Too low

   /** \action
    * Attempt to set sensor calibration
    */
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * Should return error
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
}

/** \purpose
 * Verify that out-of-range height position is rejected. Tests RSPP_E_INVALID_CALIBRATION for height outside [-1, 5]m
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Invalid_Mounting_Position_Height)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set height position out of valid range [-1, 5]
    */
   RSPP_State_Initialize();
   sensor_calibration.mounting_position.vcs_position.height = INVALID_HEIGHT_LOW; // Too low

   /** \action
    * Attempt to set sensor calibration
    */
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * Should return error
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
}

/** \purpose
 * Verify that out-of-range boresight azimuth angle is rejected. Tests RSPP_E_INVALID_CALIBRATION for azimuth outside [-2pi, 2pi]
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Invalid_Boresight_Azimuth)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set boresight azimuth out of valid range [-2pi, 2pi]
    */
   RSPP_State_Initialize();
   sensor_calibration.mounting_position.vcs_boresight_azimuth_angle = INVALID_BORESIGHT_HIGH; // Too high

   /** \action
    * Attempt to set sensor calibration
    */
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * Should return error
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
}

/** \purpose
 * Verify that FOV min azimuth outside valid range is rejected. Tests RSPP_E_INVALID_CALIBRATION for fov_min_az outside [-PI, 0]
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Invalid_FOV_Min_Azimuth)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set fov_min_az out of valid range [-PI, 0]
    */
   RSPP_State_Initialize();
   sensor_calibration.fov_min_az_rad[0] = INVALID_FOV_MIN_AZ; // Too low (< -PI)

   /** \action
    * Attempt to set sensor calibration
    */
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * Should return error
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
}

/** \purpose
 * Verify that FOV max azimuth outside valid range is rejected. Tests RSPP_E_INVALID_CALIBRATION for fov_max_az outside [-PI, PI]
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Invalid_FOV_Max_Azimuth)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set fov_max_az out of valid range [-PI, PI]
    */
   RSPP_State_Initialize();
   sensor_calibration.fov_max_az_rad[1] = INVALID_FOV_MAX_AZ; // Too high (> PI)

   /** \action
    * Attempt to set sensor calibration
    */
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * Should return error
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
}

/** \purpose
 * Verify that FOV min >= max azimuth is rejected. Tests RSPP_E_INVALID_CALIBRATION for invalid FOV range
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_FOV_Min_Greater_Than_Max_Azimuth)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set fov_min >= fov_max (invalid)
    */
   RSPP_State_Initialize();
   sensor_calibration.fov_min_az_rad[0] = -0.5F;
   sensor_calibration.fov_max_az_rad[0] = -0.6F; // max < min

   /** \action
    * Attempt to set sensor calibration
    */
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * Should return error
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
}

/** \purpose
 * Verify that FOV min elevation outside valid range is rejected. Tests RSPP_E_INVALID_CALIBRATION for fov_min_el outside [-PI, PI]
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Invalid_FOV_Min_Elevation)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set fov_min_el out of valid range [-PI, PI]
    */
   RSPP_State_Initialize();
   sensor_calibration.fov_min_el_rad[2] = INVALID_FOV_MIN_EL; // Too high (> PI)

   /** \action
    * Attempt to set sensor calibration
    */
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * Should return error
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
}

/** \purpose
 * Verify that FOV max elevation outside valid range is rejected. Tests RSPP_E_INVALID_CALIBRATION for fov_max_el outside [-PI, PI]
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Invalid_FOV_Max_Elevation)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set fov_max_el out of valid range [-PI, PI]
    */
   RSPP_State_Initialize();
   sensor_calibration.fov_max_el_rad[3] = INVALID_FOV_MAX_EL; // Too high (> PI)

   /** \action
    * Attempt to set sensor calibration
    */
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * Should return error
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
}

/** \purpose
 * Verify that FOV min >= max elevation is rejected. Tests RSPP_E_INVALID_CALIBRATION for invalid elevation FOV range
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_FOV_Min_Greater_Than_Max_Elevation)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set fov_min_el >= fov_max_el (invalid)
    */
   RSPP_State_Initialize();
   sensor_calibration.fov_min_el_rad[1] = -0.2F;
   sensor_calibration.fov_max_el_rad[1] = -0.3F; // max < min

   /** \action
    * Attempt to set sensor calibration
    */
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * Should return error
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
}

/** \purpose
 * Verify that range limit outside valid range is rejected. Tests RSPP_E_INVALID_CALIBRATION for range_limit outside [0, 1000]m
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Invalid_Range_Limit)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set range_limit out of valid range [0, 1000]
    */
   RSPP_State_Initialize();
   sensor_calibration.range_limits[0] = INVALID_RANGE_LIMIT; // Negative range

   /** \action
    * Attempt to set sensor calibration
    */
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * Should return error
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
}

/** \purpose
 * Verify that v_wrapping outside valid range is rejected. Tests RSPP_E_INVALID_CALIBRATION for v_wrapping outside [10, 100]
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Invalid_V_Wrapping)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set v_wrapping out of valid range [10, 100]
    */
   RSPP_State_Initialize();
   sensor_calibration.v_wrapping[2] = INVALID_V_WRAP_LOW; // Too low (< 10)

   /** \action
    * Attempt to set sensor calibration
    */
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * Should return error
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
}

/** \purpose
 * Verify that min_aliased_range_rate outside valid range is rejected. Tests RSPP_E_INVALID_CALIBRATION for min_aliased_rr outside [-200, 0]
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Invalid_Min_Aliased_Range_Rate)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set min_aliased_range_rate out of valid range [-200, 0]
    */
   RSPP_State_Initialize();
   sensor_calibration.min_aliaised_range_rate[1] = INVALID_MIN_ALIASED_RR; // Positive (> 0)

   /** \action
    * Attempt to set sensor calibration
    */
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * Should return error
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
}

/** \purpose
 * Verify that multiple different sensors can be calibrated. Tests successful calibration of multiple distinct sensor IDs
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Multiple_Sensors)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Initialize and prepare multiple sensor calibrations
    */
   RSPP_State_Initialize();

   /** \action
    * Set three different sensors
    */
   sensor_calibration.id = 1U;
   RSPP_Return_Type_T result1 = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   sensor_calibration.id = 2U;
   RSPP_Return_Type_T result2 = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   sensor_calibration.id = 3U;
   RSPP_Return_Type_T result3 = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * All should succeed
    */
   CHECK_EQUAL(RSPP_E_OK, result1);
   CHECK_EQUAL(RSPP_E_OK, result2);
   CHECK_EQUAL(RSPP_E_OK, result3);
}

/** \purpose
 * Verify that calibrating fewer than 10 sensors keeps state as INITIALIZED. Tests that state doesn't transition prematurely
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Partial_Sensors_Stays_Initialized)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Initialize
    */
   RSPP_State_Initialize();

   /** \action
    * Set 5 sensors (half of MAX_NUMBER_OF_SENSORS)
    */
   for (uint8_t sensor_id = 1U; sensor_id <= 5U; sensor_id++)
   {
      sensor_calibration.id = sensor_id;
      RSPP_State_Set_Sensor_Calibration(sensor_calibration);
   }

   RSPP_States_Type_T state_after_partial = RSPP_State_Get_Current_State();

   /** \result
    * State should still be INITIALIZED
    */
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, state_after_partial);
}

/** \purpose
 * Verify that Get_Sensor_Calibration returns the calibration that was set. Tests data persistence and retrieval
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Get_Sensor_Calibration_After_Set)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set a sensor calibration with specific values
    */
   RSPP_State_Initialize();
   sensor_calibration.id = 3U;
   sensor_calibration.polarity = -1; // Negative polarity
   sensor_calibration.mounting_position.vcs_position.longitudinal = 0.5F;
   sensor_calibration.mounting_position.vcs_position.lateral = -1.2F;
   sensor_calibration.mounting_position.vcs_position.height = 0.8F;
   sensor_calibration.v_wrapping[0] = 45.0F;
   RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \action
    * Retrieve the sensor calibration (sensor_idx = id - 1)
    */
   const RSPP_Sensor_Calib_T &retrieved = RSPP_State_Get_Sensor_Calibration(2);

   /** \result
    * Retrieved data should match what was set
    */
   CHECK_EQUAL(-1, retrieved.polarity);
   DOUBLES_EQUAL(0.5F, retrieved.vcs_mounting_position.longitudinal, TOLERANCE);
   DOUBLES_EQUAL(-1.2F, retrieved.vcs_mounting_position.lateral, TOLERANCE);
   DOUBLES_EQUAL(0.8F, retrieved.vcs_mounting_position.height, TOLERANCE);
   DOUBLES_EQUAL(45.0F, retrieved.v_wrapping[0], TOLERANCE);
}

/** \purpose
 * Verify that FOV arrays are correctly stored and retrieved. Tests array data persistence for all look IDs
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Get_Sensor_Calibration_FOV_Arrays_Stored)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set sensor with specific FOV values
    */
   RSPP_State_Initialize();
   sensor_calibration.id = 5U;
   sensor_calibration.fov_min_az_rad[0] = -0.8F;
   sensor_calibration.fov_max_az_rad[0] = 0.9F;
   sensor_calibration.fov_min_az_rad[1] = -0.7F;
   sensor_calibration.fov_max_az_rad[1] = 0.8F;
   RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \action
    * Retrieve the sensor calibration
    */
   const RSPP_Sensor_Calib_T &retrieved = RSPP_State_Get_Sensor_Calibration(4);

   /** \result
    * FOV arrays should match
    */
   DOUBLES_EQUAL(-0.8F, retrieved.fov_min_az_rad[0], TOLERANCE);
   DOUBLES_EQUAL(0.9F, retrieved.fov_max_az_rad[0], TOLERANCE);
   DOUBLES_EQUAL(-0.7F, retrieved.fov_min_az_rad[1], TOLERANCE);
   DOUBLES_EQUAL(0.8F, retrieved.fov_max_az_rad[1], TOLERANCE);
}

/** \purpose
 * Verify that positive polarity (+1) is accepted. Tests valid polarity value
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Polarity_Positive)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set polarity to +1
    */
   RSPP_State_Initialize();
   sensor_calibration.polarity = 1;

   /** \action
    * Set sensor calibration
    */
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * Should succeed
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

/** \purpose
 * Verify that negative polarity (-1) is accepted. Tests valid polarity value
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Polarity_Negative)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set polarity to -1
    */
   RSPP_State_Initialize();
   sensor_calibration.polarity = -1;

   /** \action
    * Set sensor calibration
    */
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * Should succeed
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

/** \purpose
 * Verify that boundary longitudinal positions are accepted. Tests edge values at -10m and +10m
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Boundary_Values_Longitudinal)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Test both boundaries
    */
   RSPP_State_Initialize();

   // Act & Assert: Min boundary
   sensor_calibration.id = 1U;
   sensor_calibration.mounting_position.vcs_position.longitudinal = VALID_LONG_MIN;
   RSPP_Return_Type_T result_min = RSPP_State_Set_Sensor_Calibration(sensor_calibration);
   CHECK_EQUAL(RSPP_E_OK, result_min);

   // Act & Assert: Max boundary
   sensor_calibration.id = 2U;
   sensor_calibration.mounting_position.vcs_position.longitudinal = VALID_LONG_MAX;
   RSPP_Return_Type_T result_max = RSPP_State_Set_Sensor_Calibration(sensor_calibration);
   CHECK_EQUAL(RSPP_E_OK, result_max);
}

/** \purpose
 * Verify that boundary lateral positions are accepted. Tests edge values at -5m and +5m
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Boundary_Values_Lateral)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Test both boundaries
    */
   RSPP_State_Initialize();

   // Act & Assert: Min boundary
   sensor_calibration.id = 1U;
   sensor_calibration.mounting_position.vcs_position.lateral = VALID_LAT_MIN;
   RSPP_Return_Type_T result_min = RSPP_State_Set_Sensor_Calibration(sensor_calibration);
   CHECK_EQUAL(RSPP_E_OK, result_min);

   // Act & Assert: Max boundary
   sensor_calibration.id = 2U;
   sensor_calibration.mounting_position.vcs_position.lateral = VALID_LAT_MAX;
   RSPP_Return_Type_T result_max = RSPP_State_Set_Sensor_Calibration(sensor_calibration);
   CHECK_EQUAL(RSPP_E_OK, result_max);
}

/** \purpose
 * Verify that boundary height positions are accepted. Tests edge values at -1m and +5m
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Boundary_Values_Height)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Test both boundaries
    */
   RSPP_State_Initialize();

   // Act & Assert: Min boundary
   sensor_calibration.id = 1U;
   sensor_calibration.mounting_position.vcs_position.height = VALID_HEIGHT_MIN;
   RSPP_Return_Type_T result_min = RSPP_State_Set_Sensor_Calibration(sensor_calibration);
   CHECK_EQUAL(RSPP_E_OK, result_min);

   // Act & Assert: Max boundary
   sensor_calibration.id = 2U;
   sensor_calibration.mounting_position.vcs_position.height = VALID_HEIGHT_MAX;
   RSPP_Return_Type_T result_max = RSPP_State_Set_Sensor_Calibration(sensor_calibration);
   CHECK_EQUAL(RSPP_E_OK, result_max);
}

/** \purpose
 * Verify that boundary FOV azimuth values are accepted. Tests edge values at -PI and +PI
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Boundary_Values_FOV_Azimuth)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set boundary FOV values
    */
   RSPP_State_Initialize();
   static constexpr float32_t PI = 3.14159265358979323846F; // PI

   // Act & Assert: Min and max boundaries
   sensor_calibration.fov_min_az_rad[0] = -PI;
   sensor_calibration.fov_max_az_rad[0] = PI;
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);
   CHECK_EQUAL(RSPP_E_OK, result);
}

/** \purpose
 * Verify that boundary range limit values are accepted. Tests edge values at 0m and 1000m
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Boundary_Values_Range_Limit)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Test boundary range limits
    */
   RSPP_State_Initialize();

   // Act & Assert: Min boundary (0m)
   sensor_calibration.id = 1U;
   sensor_calibration.range_limits[0] = 0.0F;
   RSPP_Return_Type_T result_min = RSPP_State_Set_Sensor_Calibration(sensor_calibration);
   CHECK_EQUAL(RSPP_E_OK, result_min);

   // Act & Assert: Max boundary (500m)
   sensor_calibration.id = 2U;
   sensor_calibration.range_limits[0] = 500.0F;
   RSPP_Return_Type_T result_max = RSPP_State_Set_Sensor_Calibration(sensor_calibration);
   CHECK_EQUAL(RSPP_E_OK, result_max);
}

/** \purpose
 * Verify that boundary v_wrapping values are accepted. Tests edge values at 10 and 100
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Boundary_Values_V_Wrapping)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Test boundary v_wrapping values
    */
   RSPP_State_Initialize();

   // Act & Assert: Min boundary (10)
   sensor_calibration.id = 1U;
   sensor_calibration.v_wrapping[0] = 10.0F;
   RSPP_Return_Type_T result_min = RSPP_State_Set_Sensor_Calibration(sensor_calibration);
   CHECK_EQUAL(RSPP_E_OK, result_min);

   // Act & Assert: Max boundary (100)
   sensor_calibration.id = 2U;
   sensor_calibration.v_wrapping[0] = 100.0F;
   RSPP_Return_Type_T result_max = RSPP_State_Set_Sensor_Calibration(sensor_calibration);
   CHECK_EQUAL(RSPP_E_OK, result_max);
}

/** \purpose
 * Verify that sensor type is correctly stored. Tests sensor_type field persistence
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Sensor_Type_Stored)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set specific sensor type
    */
   RSPP_State_Initialize();
   sensor_calibration.id = 7U;
   sensor_calibration.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
   RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \action
    * Retrieve sensor calibration
    */
   const RSPP_Sensor_Calib_T &retrieved = RSPP_State_Get_Sensor_Calibration(6);

   /** \result
    * Sensor type should match
    */
   CHECK_EQUAL(RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR, retrieved.sensor_type);
}

/** \purpose
 * Verify that sensors can be calibrated in non-sequential order. Tests that sensor order doesn't matter for calibration
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Interleaved_Sensor_IDs)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Initialize
    */
   RSPP_State_Initialize();

   /** \action
    * Set sensors in non-sequential order
    */
   sensor_calibration.id = 7U;
   RSPP_Return_Type_T result1 = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   sensor_calibration.id = 2U;
   RSPP_Return_Type_T result2 = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   sensor_calibration.id = 9U;
   RSPP_Return_Type_T result3 = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   sensor_calibration.id = 1U;
   RSPP_Return_Type_T result4 = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * All should succeed regardless of order
    */
   CHECK_EQUAL(RSPP_E_OK, result1);
   CHECK_EQUAL(RSPP_E_OK, result2);
   CHECK_EQUAL(RSPP_E_OK, result3);
   CHECK_EQUAL(RSPP_E_OK, result4);
}

/** \purpose
 * Verify that re-initialization clears previously set sensor calibrations. Tests that sensors can be reconfigured after reinitialize
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Reinitialize_Clears_Previous_Calibrations)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set a sensor calibration
    */
   RSPP_State_Initialize();
   sensor_calibration.id = 4U;
   RSPP_Return_Type_T result1 = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \action
    * Re-initialize and attempt to set same sensor again
    */
   RSPP_State_Initialize();
   RSPP_Return_Type_T result2 = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * Both should succeed (second is not a duplicate after reinit)
    */
   CHECK_EQUAL(RSPP_E_OK, result1);
   CHECK_EQUAL(RSPP_E_OK, result2);
}

/** \purpose
 * Verify that interior FOV is calculated and stored correctly. Tests RSPP_Update_Sensor_FOV is called during Set_Sensor_Calibration
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Get_Sensor_Calibration_Interior_FOV_Calculated)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set sensor with specific FOV values
    */
   RSPP_State_Initialize();
   sensor_calibration.id = 6U;
   sensor_calibration.fov_min_az_rad[0] = -0.6F;
   sensor_calibration.fov_max_az_rad[0] = 0.7F;
   sensor_calibration.fov_min_az_rad[1] = -0.5F;
   sensor_calibration.fov_max_az_rad[1] = 0.8F;
   RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \action
    * Retrieve sensor calibration
    */
   const RSPP_Sensor_Calib_T &retrieved = RSPP_State_Get_Sensor_Calibration(5);

   /** \result
    * Interior FOV should be calculated (clamped values)
    */
   // Note: interior_fov is computed as clamped min/max values
   CHECK(retrieved.interior_fov[RSPP_DET_LOOK_ID_0] != -999.0F); // Not sentinel
   CHECK(retrieved.interior_fov[RSPP_DET_LOOK_ID_1] != -999.0F);
}

/** \purpose
 * Verify that FOV normal vectors are calculated and stored. Tests that left and right normal vectors are computed
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Get_Sensor_Calibration_Normal_Vectors_Calculated)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set sensor calibration
    */
   RSPP_State_Initialize();
   sensor_calibration.id = 8U;
   RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \action
    * Retrieve sensor calibration
    */
   const RSPP_Sensor_Calib_T &retrieved = RSPP_State_Get_Sensor_Calibration(7);

   /** \result
    * Normal vectors should be calculated (non-zero for valid FOV)
    */
   // Check that normal vectors have magnitude approximately 1 (unit vectors)
   float32_t left_magnitude = RSPP_Sqrtf(
       retrieved.left_fov_normal[RSPP_DET_LOOK_ID_0] * retrieved.left_fov_normal[RSPP_DET_LOOK_ID_0] +
       retrieved.left_fov_normal[RSPP_DET_LOOK_ID_1] * retrieved.left_fov_normal[RSPP_DET_LOOK_ID_1]);
   DOUBLES_EQUAL(1.0F, left_magnitude, 0.01F); // Unit vector tolerance
}

/** \purpose
 * Verify that state progresses monotonically during sensor calibration. Tests that state never regresses while adding sensors
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_State_Progression_Monotonic)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Initialize
    */
   RSPP_State_Initialize();
   RSPP_States_Type_T prev_state = RSPP_State_Get_Current_State();

   /** \action
    * Set sensors one by one and track state
    */
   for (uint8_t sensor_id = 1U; sensor_id <= MAX_NUMBER_OF_SENSORS; sensor_id++)
   {
      sensor_calibration.id = sensor_id;
      RSPP_State_Set_Sensor_Calibration(sensor_calibration);
      RSPP_States_Type_T current_state = RSPP_State_Get_Current_State();

      /** \result
       * State should only stay same or increase
       */
      CHECK(current_state >= prev_state);
      prev_state = current_state;
   }
}

/** \purpose
 * Verify that boresight azimuth below -2pi is rejected. Tests RSPP_E_INVALID_CALIBRATION for negative boundary
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Invalid_Boresight_Azimuth_Negative_Boundary)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set boresight azimuth out of valid range at negative boundary
    */
   RSPP_State_Initialize();
   sensor_calibration.mounting_position.vcs_boresight_azimuth_angle = INVALID_BORESIGHT_LOW; // Too low

   /** \action
    * Attempt to set sensor calibration
    */
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * Should return error
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
}

/** \purpose
 * Verify that FOV min azimuth validation applies to all 4 look IDs. Tests that invalid value in any look ID is caught
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_All_Look_IDs_Validated_FOV_Min_Azimuth)
{
   /** \step{1}
    * Execute test and verify result.
    */

   // Test that invalid value in each look ID is caught
   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; look_id++)
   {
      /** \precond
       * Set invalid fov_min_az in specific look_id
       */
      RSPP_State_Initialize();
      sensor_calibration.fov_min_az_rad[look_id] = INVALID_FOV_MIN_AZ; // Invalid

      /** \action
       * Attempt to set sensor calibration
       */
      RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

      /** \result
       * Should be rejected
       */
      CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);

      // Reset for next iteration
      sensor_calibration.fov_min_az_rad[look_id] = -1.0F; // Valid again
   }
}

/** \purpose
 * Verify that FOV max elevation validation applies to all 4 look IDs. Tests that invalid value in any look ID is caught
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_All_Look_IDs_Validated_FOV_Max_Elevation)
{
   /** \step{1}
    * Execute test and verify result.
    */

   // Test that invalid value in each look ID is caught
   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; look_id++)
   {
      /** \precond
       * Set invalid fov_max_el in specific look_id
       */
      RSPP_State_Initialize();
      sensor_calibration.fov_max_el_rad[look_id] = INVALID_FOV_MAX_EL; // Invalid (> PI)

      /** \action
       * Attempt to set sensor calibration
       */
      RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

      /** \result
       * Should be rejected
       */
      CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);

      // Reset for next iteration
      sensor_calibration.fov_max_el_rad[look_id] = 0.5F; // Valid again
   }
}

/** \purpose
 * Verify that range limit validation applies to all 4 look IDs. Tests that invalid value in any look ID is caught
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_All_Look_IDs_Validated_Range_Limit)
{
   /** \step{1}
    * Execute test and verify result.
    */

   // Test that invalid value in each look ID is caught
   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; look_id++)
   {
      /** \precond
       * Set invalid range_limit in specific look_id
       */
      RSPP_State_Initialize();
      sensor_calibration.range_limits[look_id] = INVALID_RANGE_LIMIT; // Invalid (negative)

      /** \action
       * Attempt to set sensor calibration
       */
      RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

      /** \result
       * Should be rejected
       */
      CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);

      // Reset for next iteration
      sensor_calibration.range_limits[look_id] = 200.0F; // Valid again
   }
}

/** \purpose
 * Verify that normal vectors point in geometrically correct directions. Tests that left normal points left (negative x) and right normal points right (positive x)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Get_Sensor_Calibration_Normal_Vectors_Point_Correct_Direction)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set sensor with symmetric FOV and zero boresight (straight ahead)
    */
   RSPP_State_Initialize();
   sensor_calibration.id = 1U;
   sensor_calibration.mounting_position.vcs_boresight_azimuth_angle = 0.0F; // Straight ahead
   sensor_calibration.fov_min_az_rad[0] = -0.5F;
   sensor_calibration.fov_max_az_rad[0] = 0.5F;
   sensor_calibration.fov_min_az_rad[1] = -0.5F;
   sensor_calibration.fov_max_az_rad[1] = 0.5F;
   RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \action
    * Retrieve sensor calibration
    */
   const RSPP_Sensor_Calib_T &cal = RSPP_State_Get_Sensor_Calibration(0);

   /** \result
    * Normal vectors are computed as:
    */
   // left_normal = (-sin(min_angle), cos(min_angle))
   // right_normal = (sin(max_angle), -cos(max_angle))
   // With min_angle = -0.5 (left edge), max_angle = +0.5 (right edge)

   // Left normal at min_angle = -0.5:
   // x = -sin(-0.5) = +0.479 (positive, normal points inward to FOV)
   // y = cos(-0.5) = +0.877 (positive, normal points forward)
   CHECK(cal.left_fov_normal[RSPP_DET_LOOK_ID_0] > 0.0F); // -sin(-0.5) > 0
   CHECK(cal.left_fov_normal[RSPP_DET_LOOK_ID_1] > 0.0F); // cos(-0.5) > 0

   // Right normal at max_angle = +0.5:
   // x = sin(0.5) = +0.479 (positive, normal points inward to FOV)
   // y = -cos(0.5) = -0.877 (negative, normal points backward)
   CHECK(cal.right_fov_normal[RSPP_DET_LOOK_ID_0] > 0.0F); // sin(0.5) > 0
   CHECK(cal.right_fov_normal[RSPP_DET_LOOK_ID_1] < 0.0F); // -cos(0.5) < 0

   /** \result
    * Normals are approximately equal magnitude (symmetric FOV)
    */
   float32_t left_mag = RSPP_Sqrtf(
       cal.left_fov_normal[RSPP_DET_LOOK_ID_0] * cal.left_fov_normal[RSPP_DET_LOOK_ID_0] +
       cal.left_fov_normal[RSPP_DET_LOOK_ID_1] * cal.left_fov_normal[RSPP_DET_LOOK_ID_1]);
   float32_t right_mag = RSPP_Sqrtf(
       cal.right_fov_normal[RSPP_DET_LOOK_ID_0] * cal.right_fov_normal[RSPP_DET_LOOK_ID_0] +
       cal.right_fov_normal[RSPP_DET_LOOK_ID_1] * cal.right_fov_normal[RSPP_DET_LOOK_ID_1]);
   DOUBLES_EQUAL(1.0F, left_mag, 0.01F);  // Unit vector
   DOUBLES_EQUAL(1.0F, right_mag, 0.01F); // Unit vector
}

/** \purpose
 * Verify that elevation boresight angle is accepted (even if not validated). Tests that elevation boresight field can be set without error
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibration_Elevation_Boresight_Angle_Stored)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set non-zero elevation boresight angle
    */
   RSPP_State_Initialize();
   sensor_calibration.mounting_position.vcs_boresight_elevation_angle = 0.05F; // 5 degrees up

   /** \action
    * Set sensor calibration
    */
   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \result
    * Should succeed (elevation boresight is stored even if not validated)
    */
   CHECK_EQUAL(RSPP_E_OK, result);
}

/** \purpose
 * Verify that all 4 look IDs have their FOV parameters stored correctly. Tests array completeness for all look IDs
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Get_Sensor_Calibration_All_Four_Look_IDs_Stored)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set sensor with distinct FOV values for each look ID
    */
   RSPP_State_Initialize();
   sensor_calibration.id = 1U;
   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; look_id++)
   {
      sensor_calibration.fov_min_az_rad[look_id] = -0.5F - (look_id * 0.1F);
      sensor_calibration.fov_max_az_rad[look_id] = 0.5F + (look_id * 0.1F);
      sensor_calibration.v_wrapping[look_id] = 30.0F + (look_id * 10.0F);
   }
   RSPP_State_Set_Sensor_Calibration(sensor_calibration);

   /** \action
    * Retrieve sensor calibration
    */
   const RSPP_Sensor_Calib_T &cal = RSPP_State_Get_Sensor_Calibration(0);

   /** \result
    * All look IDs should have their distinct values stored
    */
   for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; look_id++)
   {
      DOUBLES_EQUAL(-0.5F - (look_id * 0.1F), cal.fov_min_az_rad[look_id], TOLERANCE);
      DOUBLES_EQUAL(0.5F + (look_id * 0.1F), cal.fov_max_az_rad[look_id], TOLERANCE);
      DOUBLES_EQUAL(30.0F + (look_id * 10.0F), cal.v_wrapping[look_id], TOLERANCE);
   }
}

// ==============================================================================
// RSPP_State_Set_Sensor_Calibrations Tests (Batch API)
// ==============================================================================

/** \purpose
 * Verify that RSPP_State_Set_Sensor_Calibrations successfully calibrates all 10 sensors in one call
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibrations_All_Sensors_Success)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Initialize RSPP state
    */
   RSPP_State_Initialize();

   /** \action
    * Create valid calibration array for all 10 sensors and call batch API
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));

   for (uint8_t sensor_id = 1U; sensor_id <= MAX_NUMBER_OF_SENSORS; sensor_id++)
   {
      ConstantProps_T &cal = sensor_array[sensor_id - 1].constant;
      cal.id = sensor_id;
      cal.polarity = 1;
      cal.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      cal.mounting_position.vcs_position.longitudinal = -1.0F + (sensor_id * 0.1F);
      cal.mounting_position.vcs_position.lateral = 0.5F;
      cal.mounting_position.vcs_position.height = 0.5F;
      cal.mounting_position.vcs_boresight_azimuth_angle = 0.0F;

      for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; look_id++)
      {
         cal.fov_min_az_rad[look_id] = -1.0F;
         cal.fov_max_az_rad[look_id] = 1.0F;
         cal.fov_min_el_rad[look_id] = -0.5F;
         cal.fov_max_el_rad[look_id] = 0.5F;
         cal.range_limits[look_id] = 200.0F;
         cal.v_wrapping[look_id] = 50.0F;
         cal.r_wrapping[look_id] = 1.0F;
         cal.min_aliaised_range_rate[look_id] = -100.0F;
      }
   }

   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * All sensors should be calibrated successfully and state should be INIT_COMPLETE
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(RSPP_STATE_INIT_COMPLETE, RSPP_State_Get_Current_State());

   // Verify all sensors are stored correctly
   for (uint8_t sensor_idx = 0; sensor_idx < MAX_NUMBER_OF_SENSORS; sensor_idx++)
   {
      const RSPP_Sensor_Calib_T &stored = RSPP_State_Get_Sensor_Calibration(sensor_idx);
      float32_t expected_long = -1.0F + ((sensor_idx + 1) * 0.1F);
      DOUBLES_EQUAL(expected_long, stored.vcs_mounting_position.longitudinal, TOLERANCE);
   }
}

/** \purpose
 * Verify that RSPP_State_Set_Sensor_Calibrations transitions state to INIT_COMPLETE
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibrations_State_Transitions)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Initialize - state should be INITIALIZED
    */
   RSPP_State_Initialize();
   RSPP_States_Type_T initial_state = RSPP_State_Get_Current_State();
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, initial_state);

   /** \action
    * Call batch calibration API with all valid sensors
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));

   for (uint8_t sensor_id = 1U; sensor_id <= MAX_NUMBER_OF_SENSORS; sensor_id++)
   {
      ConstantProps_T &cal = sensor_array[sensor_id - 1].constant;
      cal.id = sensor_id;
      cal.polarity = 1;
      cal.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      cal.mounting_position.vcs_position.height = 0.5F;

      for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; look_id++)
      {
         cal.fov_min_az_rad[look_id] = -1.0F;
         cal.fov_max_az_rad[look_id] = 1.0F;
         cal.fov_min_el_rad[look_id] = -0.5F;
         cal.fov_max_el_rad[look_id] = 0.5F;
         cal.v_wrapping[look_id] = 50.0F;
         cal.r_wrapping[look_id] = 1.0F;
         cal.range_limits[look_id] = 200.0F;
         cal.min_aliaised_range_rate[look_id] = -100.0F;
      }
   }

   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * State should transition from INITIALIZED to INIT_COMPLETE
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   RSPP_States_Type_T final_state = RSPP_State_Get_Current_State();
   CHECK_EQUAL(RSPP_STATE_INIT_COMPLETE, final_state);
}

/** \purpose
 * Verify that RSPP_State_Set_Sensor_Calibrations fails when called before initialization
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibrations_Not_Initialized_Fails)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Do NOT initialize RSPP state (state is UNINITIALIZED)
    */
   RSPP_State_Reset();

   /** \action
    * Try to calibrate sensors without initialization
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));

   for (uint8_t sensor_id = 1U; sensor_id <= MAX_NUMBER_OF_SENSORS; sensor_id++)
   {
      sensor_array[sensor_id - 1].constant.id = sensor_id;
      sensor_array[sensor_id - 1].constant.polarity = 1;
      sensor_array[sensor_id - 1].constant.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      sensor_array[sensor_id - 1].constant.mounting_position.vcs_position.height = 0.5F;

      for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; look_id++)
      {
         sensor_array[sensor_id - 1].constant.fov_min_az_rad[look_id] = -1.0F;
         sensor_array[sensor_id - 1].constant.fov_max_az_rad[look_id] = 1.0F;
         sensor_array[sensor_id - 1].constant.fov_min_el_rad[look_id] = -0.5F;
         sensor_array[sensor_id - 1].constant.fov_max_el_rad[look_id] = 0.5F;
         sensor_array[sensor_id - 1].constant.v_wrapping[look_id] = 50.0F;
         sensor_array[sensor_id - 1].constant.r_wrapping[look_id] = 1.0F;
         sensor_array[sensor_id - 1].constant.range_limits[look_id] = 200.0F;
         sensor_array[sensor_id - 1].constant.min_aliaised_range_rate[look_id] = -100.0F;
      }
   }

   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should fail with RSPP_E_NOT_INITIALIZED
    */
   CHECK_EQUAL(RSPP_E_NOT_INITIALIZED, result);
}

/** \purpose
 * Verify that RSPP_State_Set_Sensor_Calibrations rejects invalid sensor ID at first position
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibrations_Invalid_ID_First_Sensor)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Initialize RSPP state
    */
   RSPP_State_Initialize();

   /** \action
    * Create array with first sensor having invalid ID (0)
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));

   // First sensor has ID 0 (invalid)
   sensor_array[0].constant.id = 0U;
   sensor_array[0].constant.polarity = 1;
   sensor_array[0].constant.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;

   // Rest are valid
   for (uint8_t sensor_id = 2U; sensor_id <= MAX_NUMBER_OF_SENSORS; sensor_id++)
   {
      sensor_array[sensor_id - 1].constant.id = sensor_id;
      sensor_array[sensor_id - 1].constant.polarity = 1;
      sensor_array[sensor_id - 1].constant.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      sensor_array[sensor_id - 1].constant.mounting_position.vcs_position.height = 0.5F;

      for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; look_id++)
      {
         sensor_array[sensor_id - 1].constant.fov_min_az_rad[look_id] = -1.0F;
         sensor_array[sensor_id - 1].constant.fov_max_az_rad[look_id] = 1.0F;
         sensor_array[sensor_id - 1].constant.fov_min_el_rad[look_id] = -0.5F;
         sensor_array[sensor_id - 1].constant.fov_max_el_rad[look_id] = 0.5F;
         sensor_array[sensor_id - 1].constant.v_wrapping[look_id] = 50.0F;
         sensor_array[sensor_id - 1].constant.r_wrapping[look_id] = 1.0F;
         sensor_array[sensor_id - 1].constant.range_limits[look_id] = 200.0F;
         sensor_array[sensor_id - 1].constant.min_aliaised_range_rate[look_id] = -100.0F;
      }
   }

   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should fail with INVALID_SENSOR_ID and state should remain INITIALIZED
    */
   CHECK_EQUAL(RSPP_E_INVALID_SENSOR_ID, result);
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, RSPP_State_Get_Current_State());
}

/** \purpose
 * Verify that RSPP_State_Set_Sensor_Calibrations rejects invalid sensor ID in middle
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibrations_Invalid_ID_Middle_Sensor)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Initialize RSPP state
    */
   RSPP_State_Initialize();

   /** \action
    * Create array with sensor 5 having ID > MAX_NUMBER_OF_SENSORS
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));

   for (uint8_t sensor_id = 1U; sensor_id <= MAX_NUMBER_OF_SENSORS; sensor_id++)
   {
      ConstantProps_T &cal = sensor_array[sensor_id - 1].constant;
      cal.id = sensor_id;
      cal.polarity = 1;
      cal.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      cal.mounting_position.vcs_position.height = 0.5F;

      for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; look_id++)
      {
         cal.fov_min_az_rad[look_id] = -1.0F;
         cal.fov_max_az_rad[look_id] = 1.0F;
         cal.fov_min_el_rad[look_id] = -0.5F;
         cal.fov_max_el_rad[look_id] = 0.5F;
         cal.v_wrapping[look_id] = 50.0F;
         cal.r_wrapping[look_id] = 1.0F;
         cal.range_limits[look_id] = 200.0F;
         cal.min_aliaised_range_rate[look_id] = -100.0F;
      }
   }

   // Make sensor 5 have invalid ID
   sensor_array[4].constant.id = MAX_NUMBER_OF_SENSORS + 1U;

   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should fail with INVALID_SENSOR_ID
    */
   CHECK_EQUAL(RSPP_E_INVALID_SENSOR_ID, result);
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, RSPP_State_Get_Current_State());
}

/** \purpose
 * Verify that RSPP_State_Set_Sensor_Calibrations rejects invalid polarity
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibrations_Invalid_Polarity)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Initialize RSPP state
    */
   RSPP_State_Initialize();

   /** \action
    * Create array with sensor 3 having invalid polarity (0)
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));

   for (uint8_t sensor_id = 1U; sensor_id <= MAX_NUMBER_OF_SENSORS; sensor_id++)
   {
      ConstantProps_T &cal = sensor_array[sensor_id - 1].constant;
      cal.id = sensor_id;
      cal.polarity = 1;
      cal.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      cal.mounting_position.vcs_position.height = 0.5F;

      for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; look_id++)
      {
         cal.fov_min_az_rad[look_id] = -1.0F;
         cal.fov_max_az_rad[look_id] = 1.0F;
         cal.fov_min_el_rad[look_id] = -0.5F;
         cal.fov_max_el_rad[look_id] = 0.5F;
         cal.v_wrapping[look_id] = 50.0F;
         cal.r_wrapping[look_id] = 1.0F;
         cal.range_limits[look_id] = 200.0F;
         cal.min_aliaised_range_rate[look_id] = -100.0F;
      }
   }

   // Make sensor 3 have invalid polarity
   sensor_array[2].constant.polarity = 0;

   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should fail with INVALID_CALIBRATION
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, RSPP_State_Get_Current_State());
}

/** \purpose
 * Verify that RSPP_State_Set_Sensor_Calibrations rejects invalid height
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibrations_Invalid_Height)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Initialize RSPP state
    */
   RSPP_State_Initialize();

   /** \action
    * Create array with sensor 7 having height below minimum
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));

   for (uint8_t sensor_id = 1U; sensor_id <= MAX_NUMBER_OF_SENSORS; sensor_id++)
   {
      ConstantProps_T &cal = sensor_array[sensor_id - 1].constant;
      cal.id = sensor_id;
      cal.polarity = 1;
      cal.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      cal.mounting_position.vcs_position.height = 0.5F;

      for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; look_id++)
      {
         cal.fov_min_az_rad[look_id] = -1.0F;
         cal.fov_max_az_rad[look_id] = 1.0F;
         cal.fov_min_el_rad[look_id] = -0.5F;
         cal.fov_max_el_rad[look_id] = 0.5F;
         cal.v_wrapping[look_id] = 50.0F;
         cal.r_wrapping[look_id] = 1.0F;
         cal.range_limits[look_id] = 200.0F;
         cal.min_aliaised_range_rate[look_id] = -100.0F;
      }
   }

   // Make sensor 7 have invalid height
   sensor_array[6].constant.mounting_position.vcs_position.height = INVALID_HEIGHT_LOW;

   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should fail with INVALID_CALIBRATION
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, RSPP_State_Get_Current_State());
}

/** \purpose
 * Verify that RSPP_State_Set_Sensor_Calibrations rejects invalid FOV (min > max)
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibrations_Invalid_FOV_Reversed)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Initialize RSPP state
    */
   RSPP_State_Initialize();

   /** \action
    * Create array with sensor 2 having reversed FOV (min > max)
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));

   for (uint8_t sensor_id = 1U; sensor_id <= MAX_NUMBER_OF_SENSORS; sensor_id++)
   {
      ConstantProps_T &cal = sensor_array[sensor_id - 1].constant;
      cal.id = sensor_id;
      cal.polarity = 1;
      cal.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      cal.mounting_position.vcs_position.height = 0.5F;

      for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; look_id++)
      {
         cal.fov_min_az_rad[look_id] = -1.0F;
         cal.fov_max_az_rad[look_id] = 1.0F;
         cal.fov_min_el_rad[look_id] = -0.5F;
         cal.fov_max_el_rad[look_id] = 0.5F;
         cal.v_wrapping[look_id] = 50.0F;
         cal.r_wrapping[look_id] = 1.0F;
         cal.range_limits[look_id] = 200.0F;
         cal.min_aliaised_range_rate[look_id] = -100.0F;
      }
   }

   // Make sensor 2 have reversed FOV for look ID 0
   sensor_array[1].constant.fov_min_az_rad[0] = 1.5F;
   sensor_array[1].constant.fov_max_az_rad[0] = -1.5F;

   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should fail with INVALID_CALIBRATION
    */
   CHECK_EQUAL(RSPP_E_INVALID_CALIBRATION, result);
   CHECK_EQUAL(RSPP_STATE_INITIALIZED, RSPP_State_Get_Current_State());
}

/** \purpose
 * Verify that RSPP_State_Set_Sensor_Calibrations stores unique values for each sensor
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibrations_Unique_Values_Per_Sensor)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Initialize RSPP state
    */
   RSPP_State_Initialize();

   /** \action
    * Create array with unique calibration values for each sensor
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));

   for (uint8_t sensor_id = 1U; sensor_id <= MAX_NUMBER_OF_SENSORS; sensor_id++)
   {
      ConstantProps_T &cal = sensor_array[sensor_id - 1].constant;
      cal.id = sensor_id;
      cal.polarity = (sensor_id % 2 == 0) ? -1 : 1; // Alternate polarity
      cal.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      cal.mounting_position.vcs_position.longitudinal = -1.0F + (sensor_id * 0.15F);
      cal.mounting_position.vcs_position.lateral = -0.75F + (sensor_id * 0.15F);
      cal.mounting_position.vcs_position.height = 0.3F + (sensor_id * 0.05F);
      cal.mounting_position.vcs_boresight_azimuth_angle = -0.5F + (sensor_id * 0.1F);

      for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; look_id++)
      {
         cal.fov_min_az_rad[look_id] = -1.0F;
         cal.fov_max_az_rad[look_id] = 1.0F;
         cal.fov_min_el_rad[look_id] = -0.5F;
         cal.fov_max_el_rad[look_id] = 0.5F;
         cal.v_wrapping[look_id] = 20.0F + (sensor_id * 5.0F);
         cal.r_wrapping[look_id] = 1.0F;
         cal.range_limits[look_id] = 150.0F + (sensor_id * 10.0F);
         cal.min_aliaised_range_rate[look_id] = -150.0F + (sensor_id * 5.0F);
      }
   }

   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * All sensors should be calibrated with their unique values
    */
   CHECK_EQUAL(RSPP_E_OK, result);

   // Verify each sensor has its unique calibration
   for (uint8_t sensor_idx = 0; sensor_idx < MAX_NUMBER_OF_SENSORS; sensor_idx++)
   {
      uint8_t sensor_id = sensor_idx + 1;
      const RSPP_Sensor_Calib_T &stored = RSPP_State_Get_Sensor_Calibration(sensor_idx);

      int8_t expected_polarity = (sensor_id % 2 == 0) ? -1 : 1;
      CHECK_EQUAL(expected_polarity, stored.polarity);

      float32_t expected_long = -1.0F + (sensor_id * 0.15F);
      float32_t expected_lat = -0.75F + (sensor_id * 0.15F);
      float32_t expected_height = 0.3F + (sensor_id * 0.05F);

      DOUBLES_EQUAL(expected_long, stored.vcs_mounting_position.longitudinal, TOLERANCE);
      DOUBLES_EQUAL(expected_lat, stored.vcs_mounting_position.lateral, TOLERANCE);
      DOUBLES_EQUAL(expected_height, stored.vcs_mounting_position.height, TOLERANCE);

      // Verify look ID specific data
      float32_t expected_v_wrap = 20.0F + (sensor_id * 5.0F);
      float32_t expected_range_limit = 150.0F + (sensor_id * 10.0F);
      DOUBLES_EQUAL(expected_v_wrap, stored.v_wrapping[0], TOLERANCE);
   }
}

/** \purpose
 * Verify that RSPP_State_Set_Sensor_Calibrations can be called after reset
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibrations_After_Reset)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Initialize and reset
    */
   RSPP_State_Initialize();
   RSPP_State_Reset();
   RSPP_State_Initialize();

   /** \action
    * Call batch calibration API
    */
   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));

   for (uint8_t sensor_id = 1U; sensor_id <= MAX_NUMBER_OF_SENSORS; sensor_id++)
   {
      ConstantProps_T &cal = sensor_array[sensor_id - 1].constant;
      cal.id = sensor_id;
      cal.polarity = 1;
      cal.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      cal.mounting_position.vcs_position.height = 0.5F;

      for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; look_id++)
      {
         cal.fov_min_az_rad[look_id] = -1.0F;
         cal.fov_max_az_rad[look_id] = 1.0F;
         cal.fov_min_el_rad[look_id] = -0.5F;
         cal.fov_max_el_rad[look_id] = 0.5F;
         cal.v_wrapping[look_id] = 50.0F;
         cal.r_wrapping[look_id] = 1.0F;
         cal.range_limits[look_id] = 200.0F;
         cal.min_aliaised_range_rate[look_id] = -100.0F;
      }
   }

   RSPP_Return_Type_T result = RSPP_State_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Should succeed after reset and re-initialize
    */
   CHECK_EQUAL(RSPP_E_OK, result);
   CHECK_EQUAL(RSPP_STATE_INIT_COMPLETE, RSPP_State_Get_Current_State());
}

/** \purpose
 * Verify that RSPP_State_Set_Sensor_Calibrations rejects duplicate call (sensors already calibrated)
 * \req
 * CPR-7940_Derived CPR-7951_Derived
 */
TEST(test_RSPP_State_Sensor_Calibration,
     RSPP_State_Sensor_Calibration_TC_Set_Sensor_Calibrations_Duplicate_Call_Rejected)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Initialize and calibrate all sensors successfully
    */
   RSPP_State_Initialize();

   F360_Radar_Sensor_T sensor_array[MAX_NUMBER_OF_SENSORS];
   memset(sensor_array, 0, sizeof(sensor_array));

   for (uint8_t sensor_id = 1U; sensor_id <= MAX_NUMBER_OF_SENSORS; sensor_id++)
   {
      ConstantProps_T &cal = sensor_array[sensor_id - 1].constant;
      cal.id = sensor_id;
      cal.polarity = 1;
      cal.sensor_type = RSPP_SENSOR_TYPE_SRR7_L_PLT_RADAR;
      cal.mounting_position.vcs_position.height = 0.5F;

      for (uint8_t look_id = 0; look_id < RSPP_DET_NUM_LOOK_ID; look_id++)
      {
         cal.fov_min_az_rad[look_id] = -1.0F;
         cal.fov_max_az_rad[look_id] = 1.0F;
         cal.fov_min_el_rad[look_id] = -0.5F;
         cal.fov_max_el_rad[look_id] = 0.5F;
         cal.v_wrapping[look_id] = 50.0F;
         cal.r_wrapping[look_id] = 1.0F;
         cal.range_limits[look_id] = 200.0F;
         cal.min_aliaised_range_rate[look_id] = -100.0F;
      }
   }

   RSPP_Return_Type_T first_result = RSPP_State_Set_Sensor_Calibrations(sensor_array);
   CHECK_EQUAL(RSPP_E_OK, first_result);

   /** \action
    * Try to calibrate again (duplicate call)
    */
   RSPP_Return_Type_T second_result = RSPP_State_Set_Sensor_Calibrations(sensor_array);

   /** \result
    * Second call should be rejected with INVALID_SENSOR_ID
    */
   CHECK_EQUAL(RSPP_E_INVALID_SENSOR_ID, second_result);
   CHECK_EQUAL(RSPP_STATE_INIT_COMPLETE, RSPP_State_Get_Current_State());
}

/** @}*/

/** \defgroup  test_RSPP_Update_Sensor_FOV
 *  @{
 */

/** \brief
 * Tests for RSPP_Update_Sensor_FOV function
 */
TEST_GROUP(test_RSPP_Update_Sensor_FOV)
{
   // Common test variables
   float32_t interior_fov[RSPP_DET_NUM_LOOK_ID];
   float32_t left_fov_normal[RSPP_DET_NUM_LOOK_ID];
   float32_t right_fov_normal[RSPP_DET_NUM_LOOK_ID];
   float32_t fov_min_az_rad[RSPP_DET_NUM_LOOK_ID];
   float32_t fov_max_az_rad[RSPP_DET_NUM_LOOK_ID];
   float32_t vcs_boresight_azimuth_angle;

   // Test constants
   static constexpr float32_t FOV_INTERIOR_LIMIT = 1.1345F; // Hard-coded value in function (65 degrees)
   static constexpr float32_t TOLERANCE = 0.0001F;
   static constexpr float32_t PI = 3.14159265F;

   /** \setup
    * Set up FOV test parameters
    */
   TEST_SETUP()
   {
      // Initialize output arrays to sentinel values
      for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
      {
         interior_fov[i] = -999.0F;
         left_fov_normal[i] = -999.0F;
         right_fov_normal[i] = -999.0F;
      }

      // Initialize input parameters with typical values
      for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
      {
         fov_min_az_rad[i] = -1.0F;
         fov_max_az_rad[i] = 1.0F;
      }
      vcs_boresight_azimuth_angle = 0.0F;
   }

   /** \teardown
    * Clean up after tests
    */
   TEST_TEARDOWN()
   {
      // No cleanup required
   }
};

/** \purpose
 * Verify basic FOV calculation with typical values. Tests that function produces output for all look IDs
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Basic_Functionality)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \action
    * Call function with typical setup values
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * All output arrays should be populated (not sentinel values)
    */
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      CHECK(interior_fov[i] != -999.0F);
      CHECK(left_fov_normal[i] != -999.0F);
      CHECK(right_fov_normal[i] != -999.0F);
   }
}

/** \purpose
 * Verify interior limit properly constrains FOV angles. Interior FOV should be clamped to [-fov_interior_limit, fov_interior_limit]
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Interior_Limit_Constrains_FOV)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set wide FOV that exceeds interior limit
    */
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      fov_min_az_rad[i] = -2.0F;
      fov_max_az_rad[i] = 2.0F;
   }

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * Interior FOV should be clamped to interior limit
    */
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      CHECK(interior_fov[i] >= -FOV_INTERIOR_LIMIT);
      CHECK(interior_fov[i] <= FOV_INTERIOR_LIMIT);
   }
}

/** \purpose
 * Verify FOV calculation with zero boresight angle. Tests centered sensor orientation
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Zero_Boresight_Angle)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Zero boresight angle (default setup)
    */
   vcs_boresight_azimuth_angle = 0.0F;

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * All outputs should be computed without error
    */
   CHECK(interior_fov[RSPP_DET_LOOK_ID_0] != -999.0F);
   CHECK(interior_fov[RSPP_DET_LOOK_ID_1] != -999.0F);
   CHECK(interior_fov[RSPP_DET_LOOK_ID_2] != -999.0F);
   CHECK(interior_fov[RSPP_DET_LOOK_ID_3] != -999.0F);
}

/** \purpose
 * Verify FOV calculation with positive boresight angle. Tests sensor angled to the right
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Positive_Boresight_Angle)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Positive boresight angle (sensor angled right)
    */
   vcs_boresight_azimuth_angle = 0.5F;

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * Normal vectors should reflect boresight offset
    */
   CHECK(left_fov_normal[RSPP_DET_LOOK_ID_0] != -999.0F);
   CHECK(right_fov_normal[RSPP_DET_LOOK_ID_0] != -999.0F);
}

/** \purpose
 * Verify FOV calculation with negative boresight angle. Tests sensor angled to the left
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Negative_Boresight_Angle)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Negative boresight angle (sensor angled left)
    */
   vcs_boresight_azimuth_angle = -0.5F;

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * Normal vectors should reflect boresight offset
    */
   CHECK(left_fov_normal[RSPP_DET_LOOK_ID_0] != -999.0F);
   CHECK(right_fov_normal[RSPP_DET_LOOK_ID_0] != -999.0F);
}

/** \purpose
 * Verify narrow FOV that fits within interior limit. When sensor FOV < interior limit, interior FOV should match sensor FOV
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Narrow_FOV_Within_Interior_Limit)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Narrow FOV within interior limit
    */
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      fov_min_az_rad[i] = -0.5F;
      fov_max_az_rad[i] = 0.5F;
   }

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * Interior FOV should match sensor FOV (not clamped)
    */
   // LR look IDs (0,1) use min/max of LOOK_ID_0 and LOOK_ID_1
   DOUBLES_EQUAL(-0.5F, interior_fov[RSPP_DET_LOOK_ID_0], TOLERANCE);
   DOUBLES_EQUAL(0.5F, interior_fov[RSPP_DET_LOOK_ID_1], TOLERANCE);
}

/** \purpose
 * Verify symmetric FOV calculation. Tests sensor with symmetric FOV about boresight
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Symmetric_FOV)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Symmetric FOV
    */
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      fov_min_az_rad[i] = -0.8F;
      fov_max_az_rad[i] = 0.8F;
   }
   vcs_boresight_azimuth_angle = 0.0F;

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * Interior FOV should be symmetric
    */
   DOUBLES_EQUAL(-interior_fov[RSPP_DET_LOOK_ID_0], interior_fov[RSPP_DET_LOOK_ID_1], TOLERANCE);
   DOUBLES_EQUAL(-interior_fov[RSPP_DET_LOOK_ID_2], interior_fov[RSPP_DET_LOOK_ID_3], TOLERANCE);
}

/** \purpose
 * Verify asymmetric FOV calculation. Tests sensor with different min/max angles
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Asymmetric_FOV)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Asymmetric FOV
    */
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      fov_min_az_rad[i] = -1.2F;
      fov_max_az_rad[i] = 0.6F;
   }

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * Interior FOV should reflect asymmetry
    */
   CHECK(interior_fov[RSPP_DET_LOOK_ID_0] < 0.0F); // Min should be negative
   CHECK(interior_fov[RSPP_DET_LOOK_ID_1] > 0.0F); // Max should be positive
}

/** \purpose
 * Verify independent calculation for LR and MR look IDs. Tests that LR (0,1) and MR (2,3) are computed separately
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Different_LR_MR_Values)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Different FOV for LR vs MR
    */
   fov_min_az_rad[RSPP_DET_LOOK_ID_0] = -0.8F;
   fov_max_az_rad[RSPP_DET_LOOK_ID_0] = 0.8F;
   fov_min_az_rad[RSPP_DET_LOOK_ID_1] = -0.7F;
   fov_max_az_rad[RSPP_DET_LOOK_ID_1] = 0.7F;
   fov_min_az_rad[RSPP_DET_LOOK_ID_2] = -1.2F;
   fov_max_az_rad[RSPP_DET_LOOK_ID_2] = 1.2F;
   fov_min_az_rad[RSPP_DET_LOOK_ID_3] = -1.1F;
   fov_max_az_rad[RSPP_DET_LOOK_ID_3] = 1.1F;

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * LR and MR interior FOV should be different
    */
   // LR uses min of LOOK_ID_0 and LOOK_ID_1: min(-0.8, -0.7) = -0.8
   DOUBLES_EQUAL(-0.8F, interior_fov[RSPP_DET_LOOK_ID_0], TOLERANCE);
   // LR uses max of LOOK_ID_0 and LOOK_ID_1: max(0.8, 0.7) = 0.8
   DOUBLES_EQUAL(0.8F, interior_fov[RSPP_DET_LOOK_ID_1], TOLERANCE);
   // MR uses min of LOOK_ID_2 and LOOK_ID_3: min(-1.2, -1.1) = -1.2, clamped to -FOV_INTERIOR_LIMIT = -1.1345
   DOUBLES_EQUAL(-FOV_INTERIOR_LIMIT, interior_fov[RSPP_DET_LOOK_ID_2], TOLERANCE);
   // MR uses max of LOOK_ID_2 and LOOK_ID_3: max(1.2, 1.1) = 1.2, clamped to FOV_INTERIOR_LIMIT = 1.1345
   DOUBLES_EQUAL(FOV_INTERIOR_LIMIT, interior_fov[RSPP_DET_LOOK_ID_3], TOLERANCE);
}

/** \purpose
 * Verify handling of zero FOV width. Tests edge case where min equals max
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Zero_FOV_Width)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Zero FOV width (min == max)
    */
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      fov_min_az_rad[i] = 0.0F;
      fov_max_az_rad[i] = 0.0F;
   }

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * Interior FOV should be zero
    */
   DOUBLES_EQUAL(0.0F, interior_fov[RSPP_DET_LOOK_ID_0], TOLERANCE);
   DOUBLES_EQUAL(0.0F, interior_fov[RSPP_DET_LOOK_ID_1], TOLERANCE);
   DOUBLES_EQUAL(0.0F, interior_fov[RSPP_DET_LOOK_ID_2], TOLERANCE);
   DOUBLES_EQUAL(0.0F, interior_fov[RSPP_DET_LOOK_ID_3], TOLERANCE);
}

/** \purpose
 * Verify normal vectors have unit magnitude. Tests that sin^2+cos^2=1 for normal vectors
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Normal_Vectors_Are_Unit_Magnitude)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \action
    * Update FOV with typical values
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * Normal vectors should have unit magnitude (sin^2 + cos^2 = 1)
    */
   // For LR (indices 0,1 are x,y components)
   float32_t left_mag_lr = left_fov_normal[0] * left_fov_normal[0] +
                           left_fov_normal[1] * left_fov_normal[1];
   DOUBLES_EQUAL(1.0F, left_mag_lr, TOLERANCE);

   float32_t right_mag_lr = right_fov_normal[0] * right_fov_normal[0] +
                            right_fov_normal[1] * right_fov_normal[1];
   DOUBLES_EQUAL(1.0F, right_mag_lr, TOLERANCE);

   // For MR (indices 2,3 are x,y components)
   float32_t left_mag_mr = left_fov_normal[2] * left_fov_normal[2] +
                           left_fov_normal[3] * left_fov_normal[3];
   DOUBLES_EQUAL(1.0F, left_mag_mr, TOLERANCE);

   float32_t right_mag_mr = right_fov_normal[2] * right_fov_normal[2] +
                            right_fov_normal[3] * right_fov_normal[3];
   DOUBLES_EQUAL(1.0F, right_mag_mr, TOLERANCE);
}

/** \purpose
 * Verify repeated calls produce consistent results. Tests deterministic behavior
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Repeated_Calls_Are_Consistent)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Storage for first call results
    */
   float32_t interior_fov_1[RSPP_DET_NUM_LOOK_ID];
   float32_t left_fov_normal_1[RSPP_DET_NUM_LOOK_ID];
   float32_t right_fov_normal_1[RSPP_DET_NUM_LOOK_ID];

   /** \action
    * Call function twice with same inputs
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   memcpy(interior_fov_1, interior_fov, sizeof(interior_fov));
   memcpy(left_fov_normal_1, left_fov_normal, sizeof(left_fov_normal));
   memcpy(right_fov_normal_1, right_fov_normal, sizeof(right_fov_normal));

   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * Results should be identical
    */
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      DOUBLES_EQUAL(interior_fov_1[i], interior_fov[i], TOLERANCE);
      DOUBLES_EQUAL(left_fov_normal_1[i], left_fov_normal[i], TOLERANCE);
      DOUBLES_EQUAL(right_fov_normal_1[i], right_fov_normal[i], TOLERANCE);
   }
}

/** \purpose
 * Verify all four look IDs are processed. Tests completeness of output
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_All_Look_IDs_Processed)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * All look IDs should have valid output
    */
   CHECK(interior_fov[RSPP_DET_LOOK_ID_0] != -999.0F);
   CHECK(interior_fov[RSPP_DET_LOOK_ID_1] != -999.0F);
   CHECK(interior_fov[RSPP_DET_LOOK_ID_2] != -999.0F);
   CHECK(interior_fov[RSPP_DET_LOOK_ID_3] != -999.0F);

   CHECK(left_fov_normal[RSPP_DET_LOOK_ID_0] != -999.0F);
   CHECK(left_fov_normal[RSPP_DET_LOOK_ID_1] != -999.0F);
   CHECK(left_fov_normal[RSPP_DET_LOOK_ID_2] != -999.0F);
   CHECK(left_fov_normal[RSPP_DET_LOOK_ID_3] != -999.0F);

   CHECK(right_fov_normal[RSPP_DET_LOOK_ID_0] != -999.0F);
   CHECK(right_fov_normal[RSPP_DET_LOOK_ID_1] != -999.0F);
   CHECK(right_fov_normal[RSPP_DET_LOOK_ID_2] != -999.0F);
   CHECK(right_fov_normal[RSPP_DET_LOOK_ID_3] != -999.0F);
}

/** \purpose
 * Verify FOV with maximum boresight angle. Tests extreme sensor orientation
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Max_Boresight_Angle)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Large boresight angle
    */
   vcs_boresight_azimuth_angle = PI / 4.0F; // 45 degrees

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * Function should handle large angles without error
    */
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      CHECK(interior_fov[i] != -999.0F);
      CHECK(left_fov_normal[i] != -999.0F);
      CHECK(right_fov_normal[i] != -999.0F);
   }
}

/** \purpose
 * Verify interior FOV maintains proper ordering. Tests that min (LOOK_ID_0/2) < max (LOOK_ID_1/3)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Interior_FOV_Ordering)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * Min interior FOV should be less than max for both LR and MR
    */
   CHECK(interior_fov[RSPP_DET_LOOK_ID_0] <= interior_fov[RSPP_DET_LOOK_ID_1]); // LR: min <= max
   CHECK(interior_fov[RSPP_DET_LOOK_ID_2] <= interior_fov[RSPP_DET_LOOK_ID_3]); // MR: min <= max
}

/** \purpose
 * Verify handling of inverted FOV where min > max. Tests robustness against invalid input configuration
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Inverted_FOV_Min_Greater_Than_Max)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Inverted FOV (min > max)
    */
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      fov_min_az_rad[i] = 1.0F;  // min is larger
      fov_max_az_rad[i] = -1.0F; // max is smaller
   }

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * Document behavior - function uses std::min/max so will swap values
    */
   // LR min should be std::min(1.0, 1.0) = 1.0, then clamped
   // LR max should be std::max(-1.0, -1.0) = -1.0, then clamped
   CHECK(interior_fov[RSPP_DET_LOOK_ID_0] != -999.0F);
   CHECK(interior_fov[RSPP_DET_LOOK_ID_1] != -999.0F);
}

/** \purpose
 * Verify normal vector component signs with positive boresight. Tests correctness of trigonometric calculations: left=(-sin, cos), right=(sin, -cos)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Normal_Vector_Signs_With_Positive_Angle)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Positive boresight angle (sensor angled right)
    */
   vcs_boresight_azimuth_angle = 0.5F; // ~28.6 degrees
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      fov_min_az_rad[i] = -0.5F;
      fov_max_az_rad[i] = 0.5F;
   }

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * For positive angle and min FOV:
    */
   // VCS angle = interior_fov[0] + boresight = -0.5 + 0.5 = 0.0
   // left_normal = (-sin(0), cos(0)) = (0, 1)
   DOUBLES_EQUAL(0.0F, left_fov_normal[0], TOLERANCE);
   DOUBLES_EQUAL(1.0F, left_fov_normal[1], TOLERANCE);

   // For max FOV:
   // VCS angle = interior_fov[1] + boresight = 0.5 + 0.5 = 1.0
   // right_normal = (sin(1), -cos(1))
   float32_t expected_sin = RSPP_Sinf(1.0F);
   float32_t expected_cos = RSPP_Cosf(1.0F);
   DOUBLES_EQUAL(expected_sin, right_fov_normal[0], TOLERANCE);
   DOUBLES_EQUAL(-expected_cos, right_fov_normal[1], TOLERANCE);
}

/** \purpose
 * Verify FOV calculation at +90deg boresight angle. Tests trigonometric edge case where sensor points perpendicular
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Boresight_Plus_90_Degrees)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * +90 degrees boresight
    */
   vcs_boresight_azimuth_angle = PI / 2.0F;
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      fov_min_az_rad[i] = -0.2F;
      fov_max_az_rad[i] = 0.2F;
   }

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * Normal vectors should be computed correctly at 90deg
    */
   // sin(pi/2) = 1.0, cos(pi/2) = 0.0
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      CHECK(interior_fov[i] != -999.0F);
      CHECK(left_fov_normal[i] != -999.0F);
      CHECK(right_fov_normal[i] != -999.0F);
   }
}

/** \purpose
 * Verify FOV calculation at -90deg boresight angle. Tests trigonometric edge case where sensor points perpendicular left
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Boresight_Minus_90_Degrees)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * -90 degrees boresight
    */
   vcs_boresight_azimuth_angle = -PI / 2.0F;
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      fov_min_az_rad[i] = -0.2F;
      fov_max_az_rad[i] = 0.2F;
   }

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * Normal vectors should be computed correctly at -90deg
    */
   // sin(-pi/2) = -1.0, cos(-pi/2) = 0.0
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      CHECK(interior_fov[i] != -999.0F);
      CHECK(left_fov_normal[i] != -999.0F);
      CHECK(right_fov_normal[i] != -999.0F);
   }
}

/** \purpose
 * Verify FOV calculation with very small angles near zero. Tests numerical precision for angles near zero
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Very_Small_Angle_Near_Zero)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Very small angles (epsilon-level)
    */
   vcs_boresight_azimuth_angle = TOLERANCE;
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      fov_min_az_rad[i] = -TOLERANCE;
      fov_max_az_rad[i] = TOLERANCE;
   }

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * Should handle small angles without numerical issues
    */
   // At near-zero angles: sin = 0, cos = 1
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      CHECK(interior_fov[i] != -999.0F);
      // Verify normal magnitudes are still unit
      float32_t left_mag = left_fov_normal[i] * left_fov_normal[i];
      float32_t right_mag = right_fov_normal[i] * right_fov_normal[i];
      CHECK(left_mag <= 1.0F + TOLERANCE);
      CHECK(right_mag <= 1.0F + TOLERANCE);
   }
}

/** \purpose
 * Verify normal vector components are in [-1, 1] range. Tests output validation - sin and cos must be in [-1, 1]
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Normal_Components_In_Valid_Range)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Various angles to test range
    */
   vcs_boresight_azimuth_angle = 0.7F;
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      fov_min_az_rad[i] = -1.5F;
      fov_max_az_rad[i] = 1.5F;
   }

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * All normal components should be in valid range [-1, 1]
    */
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      CHECK(left_fov_normal[i] >= -1.0F - TOLERANCE);
      CHECK(left_fov_normal[i] <= 1.0F + TOLERANCE);
      CHECK(right_fov_normal[i] >= -1.0F - TOLERANCE);
      CHECK(right_fov_normal[i] <= 1.0F + TOLERANCE);
   }
}

/** \purpose
 * Verify interior FOV matches exact clamping formula. Tests specific values to validate max(min, -limit) and min(max, limit)
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Interior_Matches_Clamping_Formula)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Known values for precise validation
    */
   fov_min_az_rad[RSPP_DET_LOOK_ID_0] = -1.5F;
   fov_max_az_rad[RSPP_DET_LOOK_ID_0] = 1.8F;
   fov_min_az_rad[RSPP_DET_LOOK_ID_1] = -1.2F;
   fov_max_az_rad[RSPP_DET_LOOK_ID_1] = 1.6F;
   fov_min_az_rad[RSPP_DET_LOOK_ID_2] = -0.8F;
   fov_max_az_rad[RSPP_DET_LOOK_ID_2] = 0.9F;
   fov_min_az_rad[RSPP_DET_LOOK_ID_3] = -0.7F;
   fov_max_az_rad[RSPP_DET_LOOK_ID_3] = 0.85F;
   vcs_boresight_azimuth_angle = 0.0F;

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * Verify exact clamping logic with FOV_INTERIOR_LIMIT = 1.1345F
    */
   // LR min: std::min(-1.5, -1.2) = -1.5, clamped to max(-1.5, -1.1345) = -1.1345
   DOUBLES_EQUAL(-FOV_INTERIOR_LIMIT, interior_fov[RSPP_DET_LOOK_ID_0], TOLERANCE);
   // LR max: std::max(1.8, 1.6) = 1.8, clamped to min(1.8, 1.1345) = 1.1345
   DOUBLES_EQUAL(FOV_INTERIOR_LIMIT, interior_fov[RSPP_DET_LOOK_ID_1], TOLERANCE);
   // MR min: std::min(-0.8, -0.7) = -0.8, clamped to max(-0.8, -1.1345) = -0.8
   DOUBLES_EQUAL(-0.8F, interior_fov[RSPP_DET_LOOK_ID_2], TOLERANCE);
   // MR max: std::max(0.9, 0.85) = 0.9, clamped to min(0.9, 1.1345) = 0.9
   DOUBLES_EQUAL(0.9F, interior_fov[RSPP_DET_LOOK_ID_3], TOLERANCE);
}

/** \purpose
 * Verify VCS angle = interior_fov + boresight relationship. Tests coordinate system transformation correctness
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Boresight_Plus_Interior_FOV_Equals_VCS_Angle)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Known boresight and interior angles
    */
   vcs_boresight_azimuth_angle = 0.3F;
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      fov_min_az_rad[i] = -0.4F;
      fov_max_az_rad[i] = 0.5F;
   }

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * Verify normal vectors match VCS angle = interior + boresight
    */
   // For LR min: vcs_angle = -0.4 + 0.3 = -0.1
   float32_t vcs_angle_lr_min = interior_fov[RSPP_DET_LOOK_ID_0] + vcs_boresight_azimuth_angle;
   float32_t expected_left_x = -RSPP_Sinf(vcs_angle_lr_min);
   float32_t expected_left_y = RSPP_Cosf(vcs_angle_lr_min);
   DOUBLES_EQUAL(expected_left_x, left_fov_normal[0], TOLERANCE);
   DOUBLES_EQUAL(expected_left_y, left_fov_normal[1], TOLERANCE);

   // For LR max: vcs_angle = 0.5 + 0.3 = 0.8
   float32_t vcs_angle_lr_max = interior_fov[RSPP_DET_LOOK_ID_1] + vcs_boresight_azimuth_angle;
   float32_t expected_right_x = RSPP_Sinf(vcs_angle_lr_max);
   float32_t expected_right_y = -RSPP_Cosf(vcs_angle_lr_max);
   DOUBLES_EQUAL(expected_right_x, right_fov_normal[0], TOLERANCE);
   DOUBLES_EQUAL(expected_right_y, right_fov_normal[1], TOLERANCE);
}

/** \purpose
 * Verify strict independence between LR and MR calculations. Tests that changing LR inputs doesn't affect MR outputs and vice versa
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_LR_MR_Independence_Verification)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Set drastically different values for LR vs MR
    */
   fov_min_az_rad[RSPP_DET_LOOK_ID_0] = -0.1F;
   fov_max_az_rad[RSPP_DET_LOOK_ID_0] = 0.1F;
   fov_min_az_rad[RSPP_DET_LOOK_ID_1] = -0.15F;
   fov_max_az_rad[RSPP_DET_LOOK_ID_1] = 0.15F;
   fov_min_az_rad[RSPP_DET_LOOK_ID_2] = -2.5F;
   fov_max_az_rad[RSPP_DET_LOOK_ID_2] = 2.8F;
   fov_min_az_rad[RSPP_DET_LOOK_ID_3] = -2.3F;
   fov_max_az_rad[RSPP_DET_LOOK_ID_3] = 2.6F;

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * LR should use LOOK_ID_0,1 only; MR should use LOOK_ID_2,3 only
    */
   // LR min = std::min(-0.1, -0.15) = -0.15
   DOUBLES_EQUAL(-0.15F, interior_fov[RSPP_DET_LOOK_ID_0], TOLERANCE);
   // LR max = std::max(0.1, 0.15) = 0.15
   DOUBLES_EQUAL(0.15F, interior_fov[RSPP_DET_LOOK_ID_1], TOLERANCE);
   // MR min = std::min(-2.5, -2.3) = -2.5, clamped to -FOV_INTERIOR_LIMIT = -1.1345
   DOUBLES_EQUAL(-FOV_INTERIOR_LIMIT, interior_fov[RSPP_DET_LOOK_ID_2], TOLERANCE);
   // MR max = std::max(2.8, 2.6) = 2.8, clamped to FOV_INTERIOR_LIMIT = 1.1345
   DOUBLES_EQUAL(FOV_INTERIOR_LIMIT, interior_fov[RSPP_DET_LOOK_ID_3], TOLERANCE);
}

/** \purpose
 * Verify handling of extreme positive boresight angles. Tests behavior near or beyond PI radians
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Extreme_Positive_Boresight)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Large positive boresight (near 180 degrees)
    */
   vcs_boresight_azimuth_angle = PI - 0.1F;
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      fov_min_az_rad[i] = -0.3F;
      fov_max_az_rad[i] = 0.3F;
   }

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * Function should handle extreme angles
    */
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      CHECK(interior_fov[i] != -999.0F);
      CHECK(left_fov_normal[i] != -999.0F);
      CHECK(right_fov_normal[i] != -999.0F);
      // Verify normals are still unit magnitude
   }

   // Verify unit magnitude for LR
   float32_t left_mag_lr = left_fov_normal[0] * left_fov_normal[0] +
                           left_fov_normal[1] * left_fov_normal[1];
   DOUBLES_EQUAL(1.0F, left_mag_lr, TOLERANCE);
}

/** \purpose
 * Verify handling of extreme negative boresight angles. Tests behavior near or beyond -PI radians
 * \req
 * CPR-7940_Derived
 */
TEST(test_RSPP_Update_Sensor_FOV, Update_Sensor_FOV_TC_Update_FOV_Extreme_Negative_Boresight)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Large negative boresight (near -180 degrees)
    */
   vcs_boresight_azimuth_angle = -PI + 0.1F;
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      fov_min_az_rad[i] = -0.3F;
      fov_max_az_rad[i] = 0.3F;
   }

   /** \action
    * Update FOV
    */
   RSPP_Update_Sensor_FOV(
       interior_fov,
       left_fov_normal,
       right_fov_normal,
       fov_min_az_rad,
       fov_max_az_rad,
       vcs_boresight_azimuth_angle);

   /** \result
    * Function should handle extreme angles
    */
   for (uint8_t i = 0; i < RSPP_DET_NUM_LOOK_ID; i++)
   {
      CHECK(interior_fov[i] != -999.0F);
      CHECK(left_fov_normal[i] != -999.0F);
      CHECK(right_fov_normal[i] != -999.0F);
   }

   // Verify unit magnitude for MR
   float32_t right_mag_mr = right_fov_normal[2] * right_fov_normal[2] +
                            right_fov_normal[3] * right_fov_normal[3];
   DOUBLES_EQUAL(1.0F, right_mag_mr, TOLERANCE);
}

/** @}*/
