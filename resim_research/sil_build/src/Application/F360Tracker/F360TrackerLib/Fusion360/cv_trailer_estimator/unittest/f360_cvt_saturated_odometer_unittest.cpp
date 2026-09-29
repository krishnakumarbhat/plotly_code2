/** \file
 * This file contains unit tests for content of f360_cvt_saturated_odometer.cpp file
 */

#include "f360_cvt_saturated_odometer.h"
#include <CppUTest/TestHarness.h>
#include "f360_calibrations.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

/** \defgroup  f360_cvt_saturated_odometer
 *  @{
 */

using namespace f360_variant_A;

/** \brief
 * Test group for the Update_Saturated_Odometer function in the f360_variant_A namespace.
 * This group tests the odometer update logic for various speed conditions and state transitions.
 */
TEST_GROUP(f360_cvt_saturated_odometer)
{
   // Declare common variables used within all tests in this test group.
   F360_CVT_Input_Data_T cvt_input;
   F360_CVT_State_T cvt_state;
   float32_t epsilon;
   F360_Calibrations_T calibrations{};

   /** \setup
    * Initialize common variables before each test.
    */
   TEST_SETUP()
   {
      // Initialize calibrations
      Initialize_Tracker_Calibrations(calibrations);

      // Initialize input
      cvt_input.host_speed = 0.0F;
      cvt_input.n_detections = 0;
      
      // Initialize state
      cvt_state.saturated_odometer_reversing_countermeasures = 0.0F;
      cvt_state.f_reversing_countermeasures_active = false;
      
      epsilon = 1E-3F;
   }
};

/** \purpose
 * Test that odometer accumulates distance when moving forward at constant speed.
 * Verifies the odometer increments by travel_dist_diff each cycle.
 * \req
 * NA
 */
TEST(f360_cvt_saturated_odometer, Odometer_Works_for_Forward_Movement)
{
   /** \precond
    * Vehicle moving forward at constant speed of 2.0 m/s
    * Initial odometer state is neutral (0.0)
    * Countermeasures are inactive
    */
   cvt_input.host_speed = 2.0F;  // Forward movement
   cvt_state.saturated_odometer_reversing_countermeasures = 49.9F;
   cvt_state.f_reversing_countermeasures_active = false;

   /** \action
    * Call Update_Saturated_Odometer function
    */
   Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);

   /** \result
    * Check that odometer accumulated expected forward distance
    * Expected: 49.9 m + 2.0 m/s * 0.05s = 50.0m
    */
   DOUBLES_EQUAL_TEXT(50.0F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should accumulate forward distance");
}

/** \purpose
 * Test that odometer accumulates distance when moving backward (reversing).
 * Verifies the odometer increments by travel_dist_diff each cycle during reversing.
 * \req
 * NA
 */
TEST(f360_cvt_saturated_odometer, Odometer_Works_for_Reverse_Movement)
{
   /** \precond
    * Vehicle moving backward at speed -2.0 m/s
    * Initial odometer state is neutral (0.0)
    * Countermeasures are inactive
    */
   cvt_input.host_speed = -4.0F;  // Backward movement
   cvt_state.saturated_odometer_reversing_countermeasures = -2.3F;
   cvt_state.f_reversing_countermeasures_active = false;

   /** \action
    * Call Update_Saturated_Odometer function
    */
   Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);

   /** \result
    * Check that odometer accumulated expected backward distance
    * Expected: -2.3 m -4.0 m/s * 0.05s = -2.5m
    */
   DOUBLES_EQUAL_TEXT(-2.5F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should accumulate backward distance");
}

/** \purpose
 * Test that odometer does not update when velocity is near zero (dead zone).
 * Verifies no accumulation when |speed| < 0.1 m/s.
 * \req
 * NA
 */
TEST(f360_cvt_saturated_odometer, Odometer_Not_Reset_Low_Speed_Forward)
{
   /** \precond
    * Vehicle speed is 0.05 m/s (within dead zone threshold of 0.1 m/s)
    * Initial odometer is at some positive value
    */
   float32_t initial_odometer = -2.5F;
   cvt_input.host_speed = 0.09F;  // Within dead zone
   cvt_state.saturated_odometer_reversing_countermeasures = initial_odometer;
   cvt_state.f_reversing_countermeasures_active = false;

   /** \action
    * Call Update_Saturated_Odometer function
    */
   Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);

   /** \result
    * Check that odometer still counts forward movement (even in dead zone)
    * Expected: -2.5m
    * Note: The dead zone threshold (0.1 m/s) is used for direction detection,
    * but distance accumulation still occurs
    */
   DOUBLES_EQUAL_TEXT(initial_odometer, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should still accumulate small distances");
   CHECK_FALSE_TEXT(cvt_state.f_reversing_countermeasures_active, 
                     "Countermeasures should remain inactive due to increments");

}

/** \purpose
 * Test that odometer does not update when velocity is near zero (dead zone).
 * Verifies no accumulation when |speed| < 0.1 m/s.
 * \req
 * NA
 */
TEST(f360_cvt_saturated_odometer, Odometer_Not_Reset_Low_Speed_Backward)
{
   /** \precond
    * Vehicle speed is 0.05 m/s (within dead zone threshold of 0.1 m/s)
    * Initial odometer is at some positive value
    */
   float32_t initial_odometer = 50.0F;
   cvt_input.host_speed = -0.09F;  // Within dead zone
   cvt_state.saturated_odometer_reversing_countermeasures = initial_odometer;
   cvt_state.f_reversing_countermeasures_active = true;

   /** \action
    * Call Update_Saturated_Odometer function
    */
   Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);

   /** \result
    * Check that odometer still counts forward movement (even in dead zone)
    * Expected: 50.0 + (-0.05) * 0.05 = 49.9950m
    * Note: The dead zone threshold (0.1 m/s) is used for direction detection,
    * but distance accumulation still occurs
    */
   DOUBLES_EQUAL_TEXT(initial_odometer, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should still accumulate small distances");
   CHECK_TRUE_TEXT(cvt_state.f_reversing_countermeasures_active, 
                     "Countermeasures should remain active due to distance decrements");

}

/** \purpose
 * Test forward-to-reverse direction transition (positive to negative).
 * Verifies odometer resets when direction reverses from forward to backward.
 * \req
 * NA
 */
TEST(f360_cvt_saturated_odometer, Odometer_Reset_When_Motion_Switches_From_Forward_To_Reverse)
{
   /** \precond
    * Odometer is positive (vehicle was moving forward)
    * Host speed becomes negative (transitioning to reverse)
    * Initial odometer value: 10.0 m (traveled 10m forward)
    */
   cvt_input.host_speed = -2.0F;  // Now moving backward
   cvt_state.saturated_odometer_reversing_countermeasures = 10.0F;  // Was positive
   cvt_state.f_reversing_countermeasures_active = false;

   /** \action
    * Call Update_Saturated_Odometer function
    */
   Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);

   /** \result
    * Check that odometer resets and starts from new direction
    * Expected: travel_dist_diff = -2.0 * 0.05 = -0.1
    * Odometer should be set to -0.1 (reset triggered)
    */
   DOUBLES_EQUAL_TEXT(-0.1F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should reset when switching from forward to reverse");
}

/** \purpose
 * Test reverse-to-forward direction transition (negative to positive).
 * Verifies odometer resets when direction reverses from backward to forward.
 * \req
 * NA
 */
TEST(f360_cvt_saturated_odometer, Odometer_Reset_When_Motion_Switches_From_Reverse_To_Forward)
{
   /** \precond
    * Odometer is negative (vehicle was reversing)
    * Host speed becomes positive (transitioning to forward)
    * Initial odometer value: -2.0 m (traveled 2m backward)
    */
   cvt_input.host_speed = 20.0F;  // Now moving forward
   cvt_state.saturated_odometer_reversing_countermeasures = -2.0F;  // Was negative
   cvt_state.f_reversing_countermeasures_active = false;

   /** \action
    * Call Update_Saturated_Odometer function
    */
   Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);

   /** \result
    * Check that odometer resets and starts from new direction
    * Expected: travel_dist_diff = 20.0 * 0.05 = 1.0
    * Odometer should be set to 1.0 (reset triggered)
    */
   DOUBLES_EQUAL_TEXT(1.0F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should reset when switching from reverse to forward");
}

/** \purpose
 * Test upper bound saturation (forward travel limit).
 * Verifies odometer is clamped to higher_bound_travel_dist = 50.0 m.
 * \req
 * NA
 */
TEST(f360_cvt_saturated_odometer, Odometer_Saturated_At_Higher_Bound_With_Countermeasures_OFF)
{
   /** \precond
    * Vehicle moving forward at constant speed
    * Odometer is already near upper limit (49.5 m)
    */
   cvt_input.host_speed = 2.0F;  // Forward movement
   cvt_state.saturated_odometer_reversing_countermeasures = 49.95F;  // Near upper limit
   cvt_state.f_reversing_countermeasures_active = true;

   /** \action
    * Call Update_Saturated_Odometer function
    */
   Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);

   /** \result
    * Check that odometer is saturated at upper bound
    * Expected: min(50.0, max(-2.5, 49.95 + 0.1)) = 50.0
    */
   DOUBLES_EQUAL_TEXT(50.0F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should saturate at upper bound (50.0 m)");
   CHECK_FALSE_TEXT(cvt_state.f_reversing_countermeasures_active, 
             "Countermeasures should be deactivated when reaching upper bound");

}

/** \purpose
 * Test lower bound saturation (backward travel limit).
 * Verifies odometer is clamped to lower_bound_travel_dist = -2.5 m.
 * \req
 * NA
 */
TEST(f360_cvt_saturated_odometer, Odometer_Saturated_At_Lower_Bound_With_Countermeasures_ON)
{
   /** \precond
    * Vehicle moving backward at constant speed
    * Odometer is already near lower limit (-2.0 m)
    */
   cvt_input.host_speed = -2.0F;  // Backward movement
   cvt_state.saturated_odometer_reversing_countermeasures = -2.45F;  // Near lower limit
   cvt_state.f_reversing_countermeasures_active = false;

   /** \action
    * Call Update_Saturated_Odometer function
    */
   Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);

   /** \result
    * Check that odometer is saturated at lower bound
    * Expected: min(50.0, max(-2.5, -2.45 - 0.1)) = -2.5
    */
   DOUBLES_EQUAL_TEXT(-2.5F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should saturate at lower bound (-2.5 m)");
   CHECK_TRUE_TEXT(cvt_state.f_reversing_countermeasures_active, 
             "Countermeasures should be activated when reaching lower bound");
}


/** \purpose
 * Test that countermeasures remain inactive while within bounds.
 * Verifies no state change when odometer is between limits.
 * \req
 * NA
 */
TEST(f360_cvt_saturated_odometer, Countermeasures_Remain_Inactive_Between_Bounds_Reverse)
{
   /** \precond
    * Vehicle reversing
    * Countermeasures are currently active
    * Odometer is between bounds (not at saturation limit)
    */
   cvt_input.host_speed = -2.0F;  // Backward movement
   cvt_state.saturated_odometer_reversing_countermeasures = -2.39F;  // Between bounds
   cvt_state.f_reversing_countermeasures_active = false;

   /** \action
    * Call Update_Saturated_Odometer function
    */
   Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);

   /** \result
    * Check that countermeasures remain active
    */
   CHECK_FALSE_TEXT(cvt_state.f_reversing_countermeasures_active, 
             "Countermeasures should remain inactive while between bounds");
   // Expected odometer: -2.39 + (-0.1) = -2.49
   DOUBLES_EQUAL_TEXT(-2.49F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should continue accumulating");
}

/** \purpose
 * Test that countermeasures remain inactive while within bounds.
 * Verifies no state change when odometer is between limits and active.
 * \req
 * NA
 */
TEST(f360_cvt_saturated_odometer, Countermeasures_Remain_Active_Between_Bounds_Forward)
{
   /** \precond
    * Vehicle moving forward
    * Countermeasures are currently inactive
    * Odometer is between bounds (not at saturation limit)
    */
   cvt_input.host_speed = 2.0F;  // Forward movement
   cvt_state.saturated_odometer_reversing_countermeasures = 49.89F;  // Between bounds
   cvt_state.f_reversing_countermeasures_active = true;

   /** \action
    * Call Update_Saturated_Odometer function
    */
   Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);

   /** \result
    * Check that countermeasures remain inactive
    */
   CHECK_TRUE_TEXT(cvt_state.f_reversing_countermeasures_active, 
             "Countermeasures should remain active while between bounds");
   // Expected odometer: 49.89 + 0.1 = 49.99
   DOUBLES_EQUAL_TEXT(49.99F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should continue accumulating");
}

/** \purpose
 * Test multiple consecutive forward distance accumulations.
 * Verifies continuous forward movement accumulates correctly.
 * \req
 * NA
 */
TEST(f360_cvt_saturated_odometer, Odometer_Works_For_Continuously_Forward_Movement)
{
   /** \precond
    * Vehicle moving forward at constant speed
    */
   cvt_input.host_speed = 2.0F;
   cvt_state.saturated_odometer_reversing_countermeasures = 49.49F;
   cvt_state.f_reversing_countermeasures_active = true;

   /** \action
    * Call Update_Saturated_Odometer function multiple times
    */
   for (int i = 0; i < 5; ++i)
   {
      Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);
   }

   /** \result
    * Check that odometer accumulated total distance after 5 updates
    * Expected: 5 * (2.0 * 0.05) = 0.5m
    */
   DOUBLES_EQUAL_TEXT(49.99F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should accumulate distance over multiple updates");
   CHECK_TRUE_TEXT(cvt_state.f_reversing_countermeasures_active, 
             "Countermeasures should remain active after multiple forward updates, as it doesn't reach bounds");
}

/** \purpose
 * Test multiple consecutive backward distance accumulations.
 * Verifies continuous backward movement accumulates correctly.
 * \req
 * NA
 */
TEST(f360_cvt_saturated_odometer, Odometer_Works_For_Continuously_Reverse_Movement)
{
   /** \precond
    * Vehicle moving backward at constant speed
    */
   cvt_input.host_speed = -2.0F;
   cvt_state.saturated_odometer_reversing_countermeasures = -2.19F;
   cvt_state.f_reversing_countermeasures_active = false;

   /** \action
    * Call Update_Saturated_Odometer function multiple times
    */
   for (int i = 0; i < 3; ++i)
   {
      Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);
   }

   /** \result
    * Check that odometer accumulated total backward distance after 3 updates
    * Expected: -2.19 + 3 * (-2.0 * 0.05) = -2.49m
    */
   DOUBLES_EQUAL_TEXT(-2.49F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should accumulate backward distance over multiple updates");
   CHECK_FALSE_TEXT(cvt_state.f_reversing_countermeasures_active, 
             "Countermeasures should remain inactive after multiple forward updates, as it doesn't reach bounds");
}

/** \purpose
 * Test zero velocity (stationary vehicle).
 * Verifies no distance accumulation when host speed is zero.
 * \req
 * NA
 */
TEST(f360_cvt_saturated_odometer, Odometer_No_Change_At_Zero_Velocity)
{
   /** \precond
    * Vehicle is stationary (speed = 0.0)
    * Odometer has some value
    */
   float32_t initial_odometer = 15.0F;
   cvt_input.host_speed = 0.0F;
   cvt_state.saturated_odometer_reversing_countermeasures = initial_odometer;
   cvt_state.f_reversing_countermeasures_active = false;

   /** \action
    * Call Update_Saturated_Odometer function
    */
   Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);

   /** \result
    * Check that odometer remains unchanged
    * Expected: 15.0 + (0.0 * 0.05) = 15.0
    */
   DOUBLES_EQUAL_TEXT(initial_odometer, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should not change at zero velocity");
   CHECK_FALSE_TEXT(cvt_state.f_reversing_countermeasures_active, 
             "Countermeasures state should remain unchanged at zero velocity");
}

/** \purpose
 * Test high forward speed accumulation.
 * Verifies correct accumulation at higher speeds.
 * \req
 * NA
 */
TEST(f360_cvt_saturated_odometer, Odometer_Works_for_High_Forward_Speed)
{
   /** \precond
    * Vehicle moving forward at high speed (30.0 m/s)
    */
   cvt_input.host_speed = 30.0F;
   cvt_state.saturated_odometer_reversing_countermeasures = 48.4F;
   cvt_state.f_reversing_countermeasures_active = false;

   /** \action
    * Call Update_Saturated_Odometer function
    */
   Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);

   /** \result
    * Check that odometer accumulated high speed distance
    * Expected: 48.4m + 30.0 * 0.05 = 50.9m
    */
   DOUBLES_EQUAL_TEXT(49.9F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should correctly accumulate at high speeds");
   CHECK_FALSE_TEXT(cvt_state.f_reversing_countermeasures_active, 
             "Countermeasures should remain inactive after multiple forward updates, as it doesn't reach bounds");
}

/** \purpose
 * Test high backward speed accumulation.
 * Verifies correct accumulation at higher backward speeds.
 * \req
 * NA
 */
TEST(f360_cvt_saturated_odometer, Odometer_Works_for_High_Backward_Speed)
{
   /** \precond
    * Vehicle moving backward at high speed (-10.0 m/s)
    */
   cvt_input.host_speed = -10.0F;
   cvt_state.saturated_odometer_reversing_countermeasures = -1.99F;
   cvt_state.f_reversing_countermeasures_active = true;

   /** \action
    * Call Update_Saturated_Odometer function
    */
   Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);

   /** \result
    * Check that odometer accumulated high backward speed distance
    * Expected: -1.99 - 10.0 * 0.05 = -2.49m
    */
   DOUBLES_EQUAL_TEXT(-2.49F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should correctly accumulate at high backward speeds");
   CHECK_TRUE_TEXT(cvt_state.f_reversing_countermeasures_active, 
             "Countermeasures should remain active after multiple forward updates, as it doesn't reach bounds");
}

/** \purpose
 * Test state transition cycle: forward -> reverse -> activate -> forward -> deactivate.
 * Verifies complete lifecycle of countermeasures activation/deactivation.
 * \req
 * NA
 */
TEST(f360_cvt_saturated_odometer, Odometer_Works_for_Multiple_Direction_Changes)
{
   /** \precond
    * Initial state: vehicle at rest, countermeasures inactive
    */
   cvt_input.host_speed = 0.0F;
   cvt_state.saturated_odometer_reversing_countermeasures = 0.0F;
   cvt_state.f_reversing_countermeasures_active = false;

   /** \action & \result
    * Step 1: Move forward slowly (accumulate positive distance)
    */
   cvt_input.host_speed = 20.0F;
   for (int i = 0; i < 10; ++i)
   {
      Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);
   }
   // Expected: 10 * (20.0 * 0.05) = 10.0m
   DOUBLES_EQUAL_TEXT(10.0F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should correctly accumulate going forward");
   CHECK_FALSE_TEXT(cvt_state.f_reversing_countermeasures_active, 
             "Countermeasures should remain inactive while moving forward");

   /** \action & \result
    * Step 2: Switch to reverse (odometer should reset to negative)
    */
   cvt_input.host_speed = -2.0F;
   Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);
   DOUBLES_EQUAL_TEXT(-0.1F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should be reset when switching to reverse");
   CHECK_FALSE_TEXT(cvt_state.f_reversing_countermeasures_active, 
             "Countermeasures should still be inactive");

   /** \action & \result
    * Step 3: Host stationary, but host_speed signal is doing random small around zero
    */
   for (int i = 0; i < 100; i++)
   {
      cvt_input.host_speed = static_cast<float32_t>( (i % 20) - 10 ) * 0.01F;  // small speed between -0.10 and 0.10 m/s
      Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);
      DOUBLES_EQUAL_TEXT(-0.1F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                         "Odometer should stay unchanged if the host speed is small enough");
      CHECK_FALSE_TEXT(cvt_state.f_reversing_countermeasures_active, 
                        "Countermeasures should still be inactive");
   }
   
   cvt_input.host_speed = -2.0F;
   /** \action & \result
    * Step 4: Continue reversing until countermeasures activate (reach -2.5)
    */
   for (int i = 0; i < 25; i++)  // need 25 updates to reach -2.5m
   {
      Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);
   }
   DOUBLES_EQUAL_TEXT(-2.5F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should be at lower bound");
   CHECK_TRUE_TEXT(cvt_state.f_reversing_countermeasures_active, 
             "Countermeasures should be activated at lower bound");

   /** \action & \result
    * Step 5: Switch to forward and accumulate until countermeasures deactivate
    */
   cvt_input.host_speed = 20.0F;
   for (int i = 0; i < 76; i++)  // Approximately 50 updates to reach 50m, 76 updates to ensure the saturation
   {
      Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);
   }
   DOUBLES_EQUAL_TEXT(50.0F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should be at upper bound");
   CHECK_FALSE_TEXT(cvt_state.f_reversing_countermeasures_active, 
             "Countermeasures should be deactivated at upper bound");
}

/** \purpose
 * Test the robustness of the odometer against oscillations in host speed
 * Verifies correct accumulation despite speed fluctuations around zero.
 * \req
 * NA
 */
TEST(f360_cvt_saturated_odometer, Odometer_is_robust_against_negative_host_speed_noises_when_stationary)
{
   /** \precond
    * Vehicle speed signal almost zero (-0.1 m/s)
    */
   cvt_input.host_speed = -0.1F;  // Slow creep
   cvt_state.saturated_odometer_reversing_countermeasures = -2.4F;
   cvt_state.f_reversing_countermeasures_active = false;

   /** \action
    * Call Update_Saturated_Odometer function
    */

   for (int i = 0; i < 50; ++i) {
      Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);
   }


   /** \result
    * Check that odometer accumulated high backward speed distance
    * Expected: -2.4 m, same as original
    */
   DOUBLES_EQUAL_TEXT(-2.4F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should correctly accumulate at high backward speeds");
   CHECK_FALSE_TEXT(cvt_state.f_reversing_countermeasures_active, 
             "Countermeasures should remain inactive after multiple small backward motions, as it should be treated as stationary");
}

/** \purpose
 * Test the robustness of the odometer against oscillations in host speed
 * Verifies correct accumulation despite speed fluctuations around zero.
 * \req
 * NA
 */
TEST(f360_cvt_saturated_odometer, Odometer_is_robust_against_forward_host_speed_noises_when_stationary)
{
   /** \precond
    * Vehicle speed signal almost zero (-0.1 m/s)
    */
   cvt_input.host_speed = 0.1F;  // Slow creep
   cvt_state.saturated_odometer_reversing_countermeasures = 49.99F;
   cvt_state.f_reversing_countermeasures_active = true;

   /** \action
    * Call Update_Saturated_Odometer function
    */

   for (int i = 0; i < 50; ++i) {
      Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);
   }


   /** \result
    * Check that odometer accumulated high backward speed distance
    * Expected: 49.99 m, same as original
    */
   DOUBLES_EQUAL_TEXT(49.99F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should correctly accumulate at high backward speeds");
   CHECK_TRUE_TEXT(cvt_state.f_reversing_countermeasures_active, 
             "Countermeasures should remain active after multiple small forward motions, as it should be treated as stationary");
}

/** \purpose
 * Test the robustness of the odometer against oscillations in host speed
 * Verifies correct accumulation although considerable speed fluctuations around zero.
 * \req
 * NA
 */
TEST(f360_cvt_saturated_odometer, Odometer_is_robust_against_host_speed_oscillations_negative_odometry)
{
   /** \precond
    * Reasonable odometer value and flag setting near the border
    */
   cvt_state.saturated_odometer_reversing_countermeasures = -1.99F;
   cvt_state.f_reversing_countermeasures_active = false;

   /** \action
    * Call Update_Saturated_Odometer function
    */
   for (int i = 0; i < 20; ++i)
   {
      cvt_input.host_speed = ((i % 2) == 1) ? 10.0F : -10.0F;
      Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);
      CHECK_FALSE_TEXT(cvt_state.f_reversing_countermeasures_active, 
                       "Countermeasures should remain inactive as the oscillations doesn't reach bounds");
   }


   /** \result
    * Check that odometer has reset and accumulated correctly
    * Expected: 0.5 m, because the last speed is positive (forward)
    */
   DOUBLES_EQUAL_TEXT(0.5F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should have been reset to zero and then increment");
}

/** \purpose
 * Test the robustness of the odometer against oscillations in host speed
 * Verifies correct accumulation although considerable speed fluctuations around zero.
 * \req
 * NA
 */
TEST(f360_cvt_saturated_odometer, Odometer_is_robust_against_host_speed_oscillations_positive_odometry)
{
   /** \precond
    * Reasonable odometer value and flag setting near the border
    */
   cvt_state.saturated_odometer_reversing_countermeasures = 49.4F;
   cvt_state.f_reversing_countermeasures_active = true;

   /** \action
    * Call Update_Saturated_Odometer function
    */
   for (int i = 0; i < 20; ++i)
   {
      cvt_input.host_speed = ((i % 2) == 1) ? 10.0F : -10.0F;
      Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);
      CHECK_TRUE_TEXT(cvt_state.f_reversing_countermeasures_active, 
                       "Countermeasures should remain active as the oscillations doesn't reach bounds");
   }


   /** \result
    * Check that odometer has reset and accumulated correctly
    * Expected: 0.5 m, because the last speed is positive (forward)
    */
   DOUBLES_EQUAL_TEXT(0.5F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should have been reset to zero and then increment");
}


/** \purpose
 * Test the odometer doesn't have accumulated errors in long term
 * Verifies correct accumulation although considerable speed fluctuations around zero.
 * \req
 * NA
 */
TEST(f360_cvt_saturated_odometer, Odometer_doesnt_accumulate_errors_in_long_term)
{
   /** \precond
    * Reasonable odometer value and flag setting near the border
    */
   cvt_state.saturated_odometer_reversing_countermeasures = -2.49F;
   cvt_state.f_reversing_countermeasures_active = false;

   /** \action & \result
    * Case 1: positive small speed
    */
   cvt_input.host_speed = 0.11F;
   for (int i = 0; i < 20; i++)
   {
      Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);
      CHECK_FALSE_TEXT(cvt_state.f_reversing_countermeasures_active, 
                       "Countermeasures should remain inactive as no saturation happened");
   }
   DOUBLES_EQUAL_TEXT(0.11F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should have been reset to zero and then incremented for 20 scans");

   /** \action & \result
    * Case 2: negative small speed, near the border of saturation
    */
   cvt_input.host_speed = -0.11F;
   for (int i = 0; i < 454; i++)
   {
      Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);
      CHECK_FALSE_TEXT(cvt_state.f_reversing_countermeasures_active, 
                       "Countermeasures should remain inactive as no saturation has happened yet");
   }
   DOUBLES_EQUAL_TEXT(-2.497F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Odometer should have been reset to zero and then incremented for 454 scans");

   /** \action & \result
    * Case 2: negative small speed, but saturate the border
    */
   cvt_input.host_speed = -0.11F;
   Update_Saturated_Odometer(calibrations, cvt_input, cvt_state);
   CHECK_TRUE_TEXT(cvt_state.f_reversing_countermeasures_active, 
                       "Countermeasures should be active as saturation has happened");
   DOUBLES_EQUAL_TEXT(-2.5F, cvt_state.saturated_odometer_reversing_countermeasures, epsilon, 
                     "Saturation should have happened at the negative border");

}

/** @}*/
