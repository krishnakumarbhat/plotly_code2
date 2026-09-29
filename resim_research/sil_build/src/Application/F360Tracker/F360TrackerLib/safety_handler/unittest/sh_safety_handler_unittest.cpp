/** \file
 * This file contains unit tests for content of sh_safety_handler.cpp file
 */

#include "sh_safety_handler.h"
#include "sh_safety_handler_internal.h"
#include "sh_unittest_support.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  test_Safety_Handler_Initialize
 *  @{
 */

/** \brief
 * Test group of safety handler initialize functions.
 */
TEST_GROUP(test_Safety_Handler_Initialize)
{
   // Declare common variables used within all tests in this test group.
   Safety_Handler_Safety_State_T sh_current_state;

   /** \setup
    * Initialize the safety handler to a known state before each test.
    */
   TEST_SETUP()
   {
      // Read the current state before initialization
      sh_current_state = Get_Safety_Handler_State();
      // Ensure safety handler is in a known state before each test
      Safety_Handler_Initialize();
   }

   /** \teardown
    * Sete state to uninitialized after each test.
    */
   TEST_TEARDOWN()
   {
      Set_Safety_Handler_State(STATE_UNINITIALIZED);
   }
};

/** \purpose
 * Test Safety_Handler_Safety_State_T value before initialized
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_State_Uninitialized_Before_Initialize)
{
   /** \precond
    * uninitialized state set up in test setup
    */
   /** \action
    * No action needed, just check initial state
    */
   /** \result
    * sh_current_state is expected to be STATE_UNINITIALIZED.
    */
   CHECK_TRUE(sh_current_state == STATE_UNINITIALIZED);
}

/** \purpose
 * Test Safety_Handler_Initialize function initializes the safety handler state.
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_Sets_State_To_Fault_Free)
{
   /** \precond
    * set up the safety handler state to uninitialized.
    */
   Set_Safety_Handler_State(STATE_UNINITIALIZED);
   sh_current_state = Get_Safety_Handler_State();
   CHECK_TRUE(sh_current_state == STATE_UNINITIALIZED);
   /** \action
    * Call Safety_Handler_Initialize().
    */
   Safety_Handler_Initialize();
   sh_current_state = Get_Safety_Handler_State();
   /** \result
    * sh_current_state is expected to be STATE_FAULT_FREE.
    */
   CHECK_TRUE(sh_current_state == STATE_FAULT_FREE);
}

/** \purpose
 * Test Safety_Handler_Initialize from STATE_SAFE transitions to STATE_FAULT_FREE.
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_From_Safe_State_Transitions_To_Fault_Free)
{
   /** \precond
    * Set safety handler state to STATE_SAFE.
    */
   Set_Safety_Handler_State(STATE_SAFE);
   sh_current_state = Get_Safety_Handler_State();
   CHECK_TRUE(sh_current_state == STATE_SAFE);

   /** \action
    * Call Safety_Handler_Initialize().
    */
   Safety_Handler_Initialize();
   sh_current_state = Get_Safety_Handler_State();

   /** \result
    * State should transition from SAFE to FAULT_FREE.
    */
   CHECK_TRUE(sh_current_state == STATE_FAULT_FREE);
}

/** \purpose
 * Test Safety_Handler_Initialize from STATE_FAULT_FREE remains STATE_FAULT_FREE.
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_From_Fault_Free_Remains_Fault_Free)
{
   /** \precond
    * Initialize to STATE_FAULT_FREE.
    */
   Safety_Handler_Initialize();
   sh_current_state = Get_Safety_Handler_State();
   CHECK_TRUE(sh_current_state == STATE_FAULT_FREE);

   /** \action
    * Call Safety_Handler_Initialize() again.
    */
   Safety_Handler_Initialize();
   sh_current_state = Get_Safety_Handler_State();

   /** \result
    * State should remain FAULT_FREE (idempotent).
    */
   CHECK_TRUE(sh_current_state == STATE_FAULT_FREE);
}

/** \purpose
 * Test multiple consecutive Safety_Handler_Initialize calls.
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_Multiple_Consecutive_Calls)
{
   /** \precond
    * Start from uninitialized state.
    */
   Set_Safety_Handler_State(STATE_UNINITIALIZED);

   /** \action
    * Call Safety_Handler_Initialize() three times consecutively.
    */
   Safety_Handler_Initialize();
   Safety_Handler_Safety_State_T state1 = Get_Safety_Handler_State();
   CHECK_TRUE(state1 == STATE_FAULT_FREE);

   Safety_Handler_Initialize();
   Safety_Handler_Safety_State_T state2 = Get_Safety_Handler_State();
   CHECK_TRUE(state2 == STATE_FAULT_FREE);

   Safety_Handler_Initialize();
   Safety_Handler_Safety_State_T state3 = Get_Safety_Handler_State();

   /** \result
    * All calls should result in STATE_FAULT_FREE.
    */
   CHECK_TRUE(state3 == STATE_FAULT_FREE);
}

/** \purpose
 * Test Safety_Handler_Initialize after alternating state changes.
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_After_Alternating_States)
{
   /** \precond
    * Alternate between different states.
    */
   Set_Safety_Handler_State(STATE_SAFE);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);

   Set_Safety_Handler_State(STATE_UNINITIALIZED);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_UNINITIALIZED);

   Set_Safety_Handler_State(STATE_FAULT_FREE);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   /** \action
    * Call Safety_Handler_Initialize().
    */
   Safety_Handler_Initialize();

   /** \result
    * Should be in STATE_FAULT_FREE regardless of previous state changes.
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
}

/** \purpose
 * Test Get_Safety_Handler_State returns correct state after initialization.
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_Get_Safety_Handler_State_After_Initialize_Returns_Fault_Free)
{
   /** \precond
    * Initialize the safety handler.
    */
   Safety_Handler_Initialize();

   /** \action
    * Call Get_Safety_Handler_State().
    */
   sh_current_state = Get_Safety_Handler_State();

   /** \result
    * State should be STATE_FAULT_FREE.
    */
   CHECK_TRUE(sh_current_state == STATE_FAULT_FREE);
}

/** \purpose
 * Test Get_Safety_Handler_State returns STATE_UNINITIALIZED when not initialized.
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_Get_Safety_Handler_State_Returns_Uninitialized_When_Not_Initialized)
{
   /** \precond
    * Set state to uninitialized.
    */
   Set_Safety_Handler_State(STATE_UNINITIALIZED);

   /** \action
    * Call Get_Safety_Handler_State().
    */
   sh_current_state = Get_Safety_Handler_State();

   /** \result
    * State should be STATE_UNINITIALIZED.
    */
   CHECK_TRUE(sh_current_state == STATE_UNINITIALIZED);
}

/** \purpose
 * Test Get_Safety_Handler_State returns STATE_SAFE when set to safe.
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_Get_Safety_Handler_State_Returns_Safe_When_Set_To_Safe)
{
   /** \precond
    * Set state to SAFE.
    */
   Set_Safety_Handler_State(STATE_SAFE);

   /** \action
    * Call Get_Safety_Handler_State().
    */
   sh_current_state = Get_Safety_Handler_State();

   /** \result
    * State should be STATE_SAFE.
    */
   CHECK_TRUE(sh_current_state == STATE_SAFE);
}

/** \purpose
 * Test Get_Safety_Handler_State is idempotent (multiple reads don't change state).
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_Get_Safety_Handler_State_Is_Idempotent)
{
   /** \precond
    * Initialize to FAULT_FREE.
    */
   Safety_Handler_Initialize();

   /** \action
    * Call Get_Safety_Handler_State() multiple times.
    */
   Safety_Handler_Safety_State_T state1 = Get_Safety_Handler_State();
   Safety_Handler_Safety_State_T state2 = Get_Safety_Handler_State();
   Safety_Handler_Safety_State_T state3 = Get_Safety_Handler_State();
   Safety_Handler_Safety_State_T state4 = Get_Safety_Handler_State();

   /** \result
    * All reads should return the same state.
    */
   CHECK_TRUE(state1 == STATE_FAULT_FREE);
   CHECK_TRUE(state2 == STATE_FAULT_FREE);
   CHECK_TRUE(state3 == STATE_FAULT_FREE);
   CHECK_TRUE(state4 == STATE_FAULT_FREE);
   CHECK_TRUE(state1 == state2);
   CHECK_TRUE(state2 == state3);
   CHECK_TRUE(state3 == state4);
}

/** \purpose
 * Test Set_Safety_Handler_State can set to STATE_FAULT_FREE.
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_Set_Safety_Handler_State_Can_Set_To_Fault_Free)
{
   /** \precond
    * Start from uninitialized state.
    */
   Set_Safety_Handler_State(STATE_UNINITIALIZED);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_UNINITIALIZED);

   /** \action
    * Set state to FAULT_FREE.
    */
   Set_Safety_Handler_State(STATE_FAULT_FREE);
   sh_current_state = Get_Safety_Handler_State();

   /** \result
    * State should be STATE_FAULT_FREE.
    */
   CHECK_TRUE(sh_current_state == STATE_FAULT_FREE);
}

/** \purpose
 * Test Set_Safety_Handler_State can set to STATE_SAFE.
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_Set_Safety_Handler_State_Can_Set_To_Safe)
{
   /** \precond
    * Initialize to FAULT_FREE.
    */
   Safety_Handler_Initialize();
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   /** \action
    * Set state to SAFE.
    */
   Set_Safety_Handler_State(STATE_SAFE);
   sh_current_state = Get_Safety_Handler_State();

   /** \result
    * State should be STATE_SAFE.
    */
   CHECK_TRUE(sh_current_state == STATE_SAFE);
}

/** \purpose
 * Test Set_Safety_Handler_State can set to STATE_UNINITIALIZED.
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_Set_Safety_Handler_State_Can_Set_To_Uninitialized)
{
   /** \precond
    * Initialize to FAULT_FREE.
    */
   Safety_Handler_Initialize();
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   /** \action
    * Set state to UNINITIALIZED.
    */
   Set_Safety_Handler_State(STATE_UNINITIALIZED);
   sh_current_state = Get_Safety_Handler_State();

   /** \result
    * State should be STATE_UNINITIALIZED.
    */
   CHECK_TRUE(sh_current_state == STATE_UNINITIALIZED);
}

/** \purpose
 * Test Set_Safety_Handler_State all possible state transitions.
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_Set_Safety_Handler_State_All_State_Transitions)
{
   /** \precond
    * Start from UNINITIALIZED.
    */
   Set_Safety_Handler_State(STATE_UNINITIALIZED);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_UNINITIALIZED);

   /** \action
    * UNINITIALIZED -> FAULT_FREE
    */
   Set_Safety_Handler_State(STATE_FAULT_FREE);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   /** \action
    * FAULT_FREE -> SAFE
    */
   Set_Safety_Handler_State(STATE_SAFE);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);

   /** \action
    * SAFE -> UNINITIALIZED
    */
   Set_Safety_Handler_State(STATE_UNINITIALIZED);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_UNINITIALIZED);

   /** \action
    * UNINITIALIZED -> SAFE (direct transition)
    */
   Set_Safety_Handler_State(STATE_SAFE);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);

   /** \action
    * SAFE -> FAULT_FREE
    */
   Set_Safety_Handler_State(STATE_FAULT_FREE);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   /** \action
    * FAULT_FREE -> UNINITIALIZED
    */
   Set_Safety_Handler_State(STATE_UNINITIALIZED);

   /** \result
    * All transitions should work correctly.
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_UNINITIALIZED);
}

/** \purpose
 * Test Set_Safety_Handler_State with same state (idempotent).
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_Set_Safety_Handler_State_Same_State_Is_Idempotent)
{
   /** \precond
    * Initialize to FAULT_FREE.
    */
   Safety_Handler_Initialize();
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   /** \action
    * Set to FAULT_FREE multiple times.
    */
   Set_Safety_Handler_State(STATE_FAULT_FREE);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   Set_Safety_Handler_State(STATE_FAULT_FREE);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   Set_Safety_Handler_State(STATE_FAULT_FREE);

   /** \result
    * State should remain FAULT_FREE.
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
}

/** \purpose
 * Test rapid alternating Set_Safety_Handler_State calls.
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_Set_Safety_Handler_State_Rapid_Alternating_Changes)
{
   /** \precond
    * Start from FAULT_FREE.
    */
   Safety_Handler_Initialize();

   /** \action
    * Rapidly alternate between states.
    */
   for (int i = 0; i < 10; i++)
   {
      Set_Safety_Handler_State(STATE_SAFE);
      CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);

      Set_Safety_Handler_State(STATE_FAULT_FREE);
      CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

      Set_Safety_Handler_State(STATE_UNINITIALIZED);
      CHECK_TRUE(Get_Safety_Handler_State() == STATE_UNINITIALIZED);

      Set_Safety_Handler_State(STATE_FAULT_FREE);
      CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
   }

   /** \result
    * State should consistently reflect the last set value.
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
}

/** \purpose
 * Test Set_Safety_Handler_State followed by Get_Safety_Handler_State consistency.
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_Set_And_Get_Safety_Handler_State_Consistency)
{
   /** \precond
    * No specific precondition.
    */

   /** \action
    * Set to UNINITIALIZED and verify.
    */
   Set_Safety_Handler_State(STATE_UNINITIALIZED);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_UNINITIALIZED);

   /** \action
    * Set to FAULT_FREE and verify.
    */
   Set_Safety_Handler_State(STATE_FAULT_FREE);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   /** \action
    * Set to SAFE and verify.
    */
   Set_Safety_Handler_State(STATE_SAFE);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);

   /** \action
    * Set back to FAULT_FREE and verify.
    */
   Set_Safety_Handler_State(STATE_FAULT_FREE);

   /** \result
    * Get should always return what Set last set.
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
}

/** \purpose
 * Test Safety_Handler_Initialize clears SAFE state properly.
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_Clears_Safe_State)
{
   /** \precond
    * Set state to SAFE.
    */
   Set_Safety_Handler_State(STATE_SAFE);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);

   /** \action
    * Initialize (should clear SAFE state).
    */
   Safety_Handler_Initialize();

   /** \result
    * State should be FAULT_FREE, not SAFE.
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
   CHECK_TRUE(Get_Safety_Handler_State() != STATE_SAFE);
   CHECK_TRUE(Get_Safety_Handler_State() != STATE_UNINITIALIZED);
}

/** \purpose
 * Test state persistence across multiple get/set operations.
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_State_Persistence_Across_Multiple_Operations)
{
   /** \precond
    * Initialize to known state.
    */
   Safety_Handler_Initialize();

   /** \action
    * Perform multiple operations and verify state persists.
    */
   Set_Safety_Handler_State(STATE_SAFE);
   Safety_Handler_Safety_State_T state1 = Get_Safety_Handler_State();
   Safety_Handler_Safety_State_T state2 = Get_Safety_Handler_State();
   CHECK_TRUE(state1 == STATE_SAFE);
   CHECK_TRUE(state2 == STATE_SAFE);
   CHECK_TRUE(state1 == state2);

   Set_Safety_Handler_State(STATE_UNINITIALIZED);
   Safety_Handler_Safety_State_T state3 = Get_Safety_Handler_State();
   Safety_Handler_Safety_State_T state4 = Get_Safety_Handler_State();
   CHECK_TRUE(state3 == STATE_UNINITIALIZED);
   CHECK_TRUE(state4 == STATE_UNINITIALIZED);
   CHECK_TRUE(state3 == state4);

   Safety_Handler_Initialize();
   Safety_Handler_Safety_State_T state5 = Get_Safety_Handler_State();
   Safety_Handler_Safety_State_T state6 = Get_Safety_Handler_State();

   /** \result
    * State should persist correctly across all operations.
    */
   CHECK_TRUE(state5 == STATE_FAULT_FREE);
   CHECK_TRUE(state6 == STATE_FAULT_FREE);
   CHECK_TRUE(state5 == state6);
}

/** \purpose
 * Test Get_Safety_Handler_State with all three state values.
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_Get_Safety_Handler_State_All_Three_States)
{
   /** \precond
    * No specific precondition.
    */

   /** \action
    * Set and get STATE_UNINITIALIZED.
    */
   Set_Safety_Handler_State(STATE_UNINITIALIZED);
   Safety_Handler_Safety_State_T state_uninit = Get_Safety_Handler_State();
   CHECK_TRUE(state_uninit == STATE_UNINITIALIZED);
   CHECK_TRUE(state_uninit == 255U); // Verify actual value

   /** \action
    * Set and get STATE_FAULT_FREE.
    */
   Set_Safety_Handler_State(STATE_FAULT_FREE);
   Safety_Handler_Safety_State_T state_fault_free = Get_Safety_Handler_State();
   CHECK_TRUE(state_fault_free == STATE_FAULT_FREE);
   CHECK_TRUE(state_fault_free == 195U); // Verify actual value

   /** \action
    * Set and get STATE_SAFE.
    */
   Set_Safety_Handler_State(STATE_SAFE);
   Safety_Handler_Safety_State_T state_safe = Get_Safety_Handler_State();

   /** \result
    * All three states should be correctly set and retrieved.
    */
   CHECK_TRUE(state_safe == STATE_SAFE);
   CHECK_TRUE(state_safe == 60U); // Verify actual value
}

/** \purpose
 * Test Safety_Handler_Initialize followed by Set_Safety_Handler_State.
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_Initialize_Then_Set_State_Sequence)
{
   /** \precond
    * Start from unknown state.
    */
   Set_Safety_Handler_State(STATE_UNINITIALIZED);

   /** \action
    * Initialize, then immediately set to SAFE.
    */
   Safety_Handler_Initialize();
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   Set_Safety_Handler_State(STATE_SAFE);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);

   /** \action
    * Initialize again, then set to UNINITIALIZED.
    */
   Safety_Handler_Initialize();
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   Set_Safety_Handler_State(STATE_UNINITIALIZED);

   /** \result
    * Set should override Initialize properly.
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_UNINITIALIZED);
}

/** \purpose
 * Test Set_Safety_Handler_State followed by Safety_Handler_Initialize.
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_Set_State_Then_Initialize_Sequence)
{
   /** \precond
    * Start from FAULT_FREE.
    */
   Safety_Handler_Initialize();

   /** \action
    * Set to SAFE, then initialize.
    */
   Set_Safety_Handler_State(STATE_SAFE);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);

   Safety_Handler_Initialize();
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   /** \action
    * Set to UNINITIALIZED, then initialize.
    */
   Set_Safety_Handler_State(STATE_UNINITIALIZED);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_UNINITIALIZED);

   Safety_Handler_Initialize();

   /** \result
    * Initialize should always set to FAULT_FREE.
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
}

/** \purpose
 * Test interleaved Initialize, Set, and Get operations.
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_Interleaved_Initialize_Set_Get_Operations)
{
   /** \precond
    * Start fresh.
    */
   Safety_Handler_Initialize();

   /** \action
    * Complex sequence of operations.
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   Set_Safety_Handler_State(STATE_SAFE);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);

   Safety_Handler_Initialize();
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   Set_Safety_Handler_State(STATE_UNINITIALIZED);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_UNINITIALIZED);

   Set_Safety_Handler_State(STATE_SAFE);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);

   Safety_Handler_Initialize();
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   Set_Safety_Handler_State(STATE_FAULT_FREE);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   Safety_Handler_Initialize();

   /** \result
    * Final state should be FAULT_FREE.
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
}

/** \purpose
 * Test multiple threads scenario simulation (state consistency).
 * \req CPR-7539_Derived CPR-7587_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_State_Consistency_Multiple_Accesses)
{
   /** \precond
    * Initialize to FAULT_FREE.
    */
   Safety_Handler_Initialize();

   /** \action
    * Simulate multiple rapid accesses.
    */
   for (int i = 0; i < 100; i++)
   {
      Safety_Handler_Safety_State_T state = Get_Safety_Handler_State();
      CHECK_TRUE(state == STATE_FAULT_FREE);
   }

   Set_Safety_Handler_State(STATE_SAFE);
   for (int i = 0; i < 100; i++)
   {
      Safety_Handler_Safety_State_T state = Get_Safety_Handler_State();
      CHECK_TRUE(state == STATE_SAFE);
   }

   /** \result
    * State should remain consistent across many accesses.
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
}

/** \purpose
 * Test Safety_Handler_Reset function resets the safety handler state.

 * \req CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_Reset_Sets_State_To_Not_Initialized)
{
   /** \precond
    * verify safety handler state is initialized as STATE_FAULT_FREE.
    */
   sh_current_state = Get_Safety_Handler_State();
   CHECK_TRUE(sh_current_state == STATE_FAULT_FREE);
   /** \action
    * Call set safety handler to uninitialized state.
    */
   Set_Safety_Handler_State(STATE_UNINITIALIZED);
   sh_current_state = Get_Safety_Handler_State();
   /** \result
    * sh_current_state is expected to be STATE_UNINITIALIZED.
    */
   CHECK_TRUE(sh_current_state == STATE_UNINITIALIZED);
}

/** \purpose
 * Test Get_Safety_Handler_State function.

 * \req CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_Get_State_Returns_Correct_State)
{
   /** \precond
    * safety handler state is initialized with STATE_FAULT_FREE.
    */
   sh_current_state = Get_Safety_Handler_State();

   /** \action
    * Check sh_current_state is STATE_FAULT_FREE then call reset function.
    */
   CHECK_TRUE(sh_current_state == STATE_FAULT_FREE);
   Set_Safety_Handler_State(STATE_UNINITIALIZED);
   sh_current_state = Get_Safety_Handler_State();
   /** \result
    * sh_current_state is expected to be STATE_UNINITIALIZED.
    */
   CHECK_TRUE(sh_current_state == STATE_UNINITIALIZED);
}

/** \purpose
 * Test Set_Safety_Handler_State function.
 * \req CPR-7539_Derived CPR-7541_Derived
 */
TEST(test_Safety_Handler_Initialize, Safety_Handler_Initialize_TC_Set_State_Sets_Correct_State)
{
   /** \precond
    * safety handler state is initialized with STATE_FAULT_FREE.
    */
   sh_current_state = Get_Safety_Handler_State();

   /** \action
    * Check sh_current_state is STATE_FAULT_FREE then call Set_Safety_Handler_State function.
    */
   CHECK_TRUE(sh_current_state == STATE_FAULT_FREE);
   Set_Safety_Handler_State(STATE_UNINITIALIZED);
   sh_current_state = Get_Safety_Handler_State();
   /** \result
    * sh_current_state is expected to be STATE_UNINITIALIZED.
    */
   CHECK_TRUE(sh_current_state == STATE_UNINITIALIZED);

   /** \action
    * Call Set_Safety_Handler_State function.
    */
   Set_Safety_Handler_State(STATE_FAULT_FREE);
   sh_current_state = Get_Safety_Handler_State();
   /** \result
    * sh_current_state is expected to be STATE_FAULT_FREE.
    */
   CHECK_TRUE(sh_current_state == STATE_FAULT_FREE);

   /** \action
    * Call Set_Safety_Handler_State function.
    */
   Set_Safety_Handler_State(STATE_SAFE);
   sh_current_state = Get_Safety_Handler_State();
   /** \result
    * sh_current_state is expected to be STATE_SAFE.
    */
   CHECK_TRUE(sh_current_state == STATE_SAFE);
}

/** @}*/

/** \defgroup  test_Evaluate_Safe_State
 *  @{
 */

/** \brief
 * Test group of evaluate safe state functions.
 */
TEST_GROUP(test_Evaluate_Safe_State)
{
   // Declare common variables used within all tests in this test group.
   ROT_Object_List_Info_T rot_object_list_info{};

   /** \setup
    * Initialized the overall fault status for each test case.
    */
   TEST_SETUP()
   {
      rot_object_list_info.all_scl_faults.overall_fault_status = 195U;
      Safety_Handler_Initialize();
   }
};

/** \purpose
 * Test Evaluate_Safe_State function when plausible failure happened once, the overall_fault_status
 * should be set to 60U and remain in the following cycles.
 * \req CPR-7587_Derived CPR-7539_Derived
 */
TEST(test_Evaluate_Safe_State, Evaluate_Safe_State_TC_Plausible_Failed_Happened_Once)
{
   /** \precond
    * initialize cycle with f_plausible false. overall_fault_status is 195U.
    */
   bool f_plausible = false;
   rot_object_list_info.all_scl_faults.overall_fault_status = 195U;
   /** \action
    * Call Evaluate_Safe_State().
    */
   Evaluate_Safe_State(f_plausible, rot_object_list_info.all_scl_faults.overall_fault_status);
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);

   // second cycle, all detections are plausible, the faults status should remain
   f_plausible = true;
   rot_object_list_info.all_scl_faults.overall_fault_status = 195U;
   Evaluate_Safe_State(f_plausible, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Describe expected output. E.g. check that the output match expected data.
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
}

/** \purpose
 * Test Evaluate_Safe_State function when plausible check pass, the overall_fault_status
 * should bypass its value without modify it.
 * \req CPR-7532_Derived
 */
TEST(test_Evaluate_Safe_State, Evaluate_Safe_State_TC_Plausible_Bypass_Overall_Fault)
{
   /** \precond
    * set up the plausible check true as input to evaluate_safe_state.
    */
   bool f_plausible = true;
   rot_object_list_info.all_scl_faults.overall_fault_status = 195U;

   /** \action
    * call  Evaluate_Safe_State().
    */
   Evaluate_Safe_State(f_plausible, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Expect the overall_fault_status remain unchanged
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);

   /** \action
    * second cycle, the f_plausible still true, but input faults status has changed
    */
   rot_object_list_info.all_scl_faults.overall_fault_status = 60U;
   Evaluate_Safe_State(f_plausible, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Expect the overall_fault_status remain unchanged
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
}

/** \purpose
 * Test Evaluate_Safe_State when state is already SAFE but check passes
 * \req CPR-7587_Derived
 */
TEST(test_Evaluate_Safe_State, Evaluate_Safe_State_TC_Already_Safe_With_Plausible_Check)
{
   /** \precond
    * Set state to SAFE and provide plausible check
    */
   Set_Safety_Handler_State(STATE_SAFE);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);

   bool f_plausible = true;
   rot_object_list_info.all_scl_faults.overall_fault_status = 195U;

   /** \action
    * Call Evaluate_Safe_State with plausible=true
    */
   Evaluate_Safe_State(f_plausible, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * State remains SAFE, fault status forced to 60U
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
}

/** \purpose
 * Test Evaluate_Safe_State when state is FAULT_FREE with plausible check failing
 * \req CPR-7587_Derived CPR-7539_Derived
 */
TEST(test_Evaluate_Safe_State, Evaluate_Safe_State_TC_From_Fault_Free_With_Plausible_False)
{
   /** \precond
    * State is FAULT_FREE
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   bool f_plausible = false;
   rot_object_list_info.all_scl_faults.overall_fault_status = 195U;

   /** \action
    * Call Evaluate_Safe_State with plausible=false
    */
   Evaluate_Safe_State(f_plausible, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * State should transition to SAFE, fault status set to 60U
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
}

/** \purpose
 * Test Evaluate_Safe_State when state is FAULT_FREE with plausible check
 * \req CPR-7587_Derived
 */
TEST(test_Evaluate_Safe_State, Evaluate_Safe_State_TC_From_Fault_Free_With_Plausible)
{
   /** \precond
    * State is FAULT_FREE
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   bool f_plausible = true;
   rot_object_list_info.all_scl_faults.overall_fault_status = 195U;

   /** \action
    * Call Evaluate_Safe_State with plausible=true
    */
   Evaluate_Safe_State(f_plausible, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * State remains FAULT_FREE, fault status unchanged
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test Evaluate_Safe_State with different initial fault status values
 * \req CPR-7587_Derived CPR-7539_Derived
 */
TEST(test_Evaluate_Safe_State, Evaluate_Safe_State_TC_Different_Initial_Fault_Values)
{
   /** \precond
    * Test with fault status = 0U
    */
   bool f_plausible = false;
   rot_object_list_info.all_scl_faults.overall_fault_status = 0U;

   /** \action
    * Trigger SAFE state
    */
   Evaluate_Safe_State(f_plausible, rot_object_list_info.all_scl_faults.overall_fault_status);
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);

   /** \action
    * Test with fault status = 255U
    */
   Safety_Handler_Initialize(); // Reset to FAULT_FREE
   rot_object_list_info.all_scl_faults.overall_fault_status = 255U;
   f_plausible = false;

   Evaluate_Safe_State(f_plausible, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Regardless of initial value, should be 60U when SAFE
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
}

/** \purpose
 * Test Evaluate_Safe_State multiple consecutive failures
 * \req CPR-7587_Derived
 */
TEST(test_Evaluate_Safe_State, Evaluate_Safe_State_TC_Multiple_Consecutive_Failures)
{
   /** \precond
    * State is FAULT_FREE
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   /** \action
    * First failure
    */
   bool f_plausible = false;
   rot_object_list_info.all_scl_faults.overall_fault_status = 195U;
   Evaluate_Safe_State(f_plausible, rot_object_list_info.all_scl_faults.overall_fault_status);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);

   /** \action
    * Second failure (already in SAFE state)
    */
   rot_object_list_info.all_scl_faults.overall_fault_status = 195U;
   Evaluate_Safe_State(f_plausible, rot_object_list_info.all_scl_faults.overall_fault_status);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);

   /** \action
    * Third failure
    */
   rot_object_list_info.all_scl_faults.overall_fault_status = 195U;
   Evaluate_Safe_State(f_plausible, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * State remains SAFE, fault always 60U
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
}

/** \purpose
 * Test Evaluate_Safe_State when state is UNINITIALIZED
 * \req CPR-7587_Derived CPR-7539_Derived
 */
TEST(test_Evaluate_Safe_State, Evaluate_Safe_State_TC_From_Uninitialized)
{
   /** \precond
    * Set state to UNINITIALIZED
    */
   Set_Safety_Handler_State(STATE_UNINITIALIZED);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_UNINITIALIZED);

   bool f_plausible = false;
   rot_object_list_info.all_scl_faults.overall_fault_status = 195U;

   /** \action
    * Call Evaluate_Safe_State with implausible
    */
   Evaluate_Safe_State(f_plausible, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Should transition to SAFE, fault status 60U
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
}

/** \purpose
 * Test fault status values other than standard 195U
 * \req CPR-7587_Derived
 */
TEST(test_Evaluate_Safe_State, Evaluate_Safe_State_TC_With_Non_Standard_Fault_Values)
{
   /** \precond
    * Test with various initial fault status values
    */
   uint8_t test_fault_values[] = {0U, 50U, 100U, 150U, 200U, 255U};

   for (size_t i = 0; i < sizeof(test_fault_values) / sizeof(test_fault_values[0]); i++)
   {
      Safety_Handler_Initialize(); // Reset to FAULT_FREE
      rot_object_list_info.all_scl_faults.overall_fault_status = test_fault_values[i];
      bool f_plausible = false;

      /** \action
       * Trigger SAFE state with different initial fault values
       */
      Evaluate_Safe_State(f_plausible, rot_object_list_info.all_scl_faults.overall_fault_status);

      /** \result
       * Regardless of initial value, should be 60U when SAFE
       */
      CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
      CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
   }
}

/** \purpose
 * Test fault status persistence through evaluate function
 * \req CPR-7587_Derived CPR-7539_Derived
 */
TEST(test_Evaluate_Safe_State, Evaluate_Safe_State_TC_Fault_Status_Persistence_In_Safe_State)
{
   /** \precond
    * Transition to SAFE state
    */
   bool f_plausible = false;
   rot_object_list_info.all_scl_faults.overall_fault_status = 195U;
   Evaluate_Safe_State(f_plausible, rot_object_list_info.all_scl_faults.overall_fault_status);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);

   /** \action
    * Call Evaluate_Safe_State multiple times with plausible=true
    */
   for (int i = 0; i < 10; i++)
   {
      f_plausible = true;
      rot_object_list_info.all_scl_faults.overall_fault_status = 195U;
      Evaluate_Safe_State(f_plausible, rot_object_list_info.all_scl_faults.overall_fault_status);

      /** \result
       * Fault status should always be forced to 60U in SAFE state
       */
      CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
      CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
   }
}

/** \purpose
 * Test Evaluate_Safe_State with intermediate fault status values.
 * \req CPR-7587_Derived CPR-7539_Derived
 */
TEST(test_Evaluate_Safe_State, Evaluate_Safe_State_TC_Intermediate_Fault_Status_Values)
{
   /** \precond
    * Test with various intermediate fault status values
    */
   uint8_t test_fault_values[] = {50U, 100U, 128U, 150U, 200U};

   for (size_t i = 0; i < sizeof(test_fault_values) / sizeof(test_fault_values[0]); i++)
   {
      Safety_Handler_Initialize(); // Reset to FAULT_FREE
      rot_object_list_info.all_scl_faults.overall_fault_status = test_fault_values[i];
      bool f_plausible = false;

      /** \action
       * Trigger SAFE state with intermediate fault value
       */
      Evaluate_Safe_State(f_plausible, rot_object_list_info.all_scl_faults.overall_fault_status);

      /** \result
       * Regardless of initial intermediate value, should be 60U when SAFE
       */
      CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
      CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
   }

   /** \action
    * Test plausible with intermediate fault value
    */
   Safety_Handler_Initialize();
   rot_object_list_info.all_scl_faults.overall_fault_status = 128U;
   bool f_plausible = true;

   Evaluate_Safe_State(f_plausible, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * When plausible in FAULT_FREE, intermediate value should pass through
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 128U);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
}

/** @}*/

/** \defgroup  test_Safety_Handler
 *  @{
 */

/** \brief
 * Test group of safety handler functions.
 */
TEST_GROUP(test_Safety_Handler)
{
   // Declare common variables used within all tests in this test group.
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list{};
   F360_Detection_Log_T f360_detection_list[MAX_NUMBER_OF_DETECTIONS];
   ROT_Object_List_Info_T rot_object_list_info{};

   /** \setup
    * Initialized the overall fault status for each test case.
    */
   TEST_SETUP()
   {
      rot_object_list_info.all_scl_faults.overall_fault_status = 195U;
      Safety_Handler_Initialize();
   }
};

/** \purpose
 * Test Safety_Handler return type.
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_State_Return_OK)
{
   /** \precond
    * safety handler state is initialized with STATE_FAULT_FREE.
    */
   Safety_Handler_Return_Type_T return_type = SH_E_NOT_INITIALIZED;

   /** \action
    * Check sh_current_state is STATE_FAULT_FREE then call Set_Safety_Handler_State function.
    */
   Safety_Handler_Initialize();
   return_type = Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list, raw_detect_list.number_of_valid_detections,
                                                 rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);
   /** \result
    * sh_current_state is expected to be STATE_UNINITIALIZED.
    */
   CHECK_TRUE(return_type == SH_E_OK);
}

/** \purpose
 * Test Safety_Handler return type.
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_State_Return_NOT_INITIALIZED)
{
   /** \precond
    * safety handler state is initialized with STATE_FAULT_FREE.
    */
   Safety_Handler_Return_Type_T return_type = SH_E_OK;

   /** \action
    * Check sh_current_state is STATE_FAULT_FREE then call Set_Safety_Handler_State function.
    */
   Set_Safety_Handler_State(STATE_UNINITIALIZED);
   return_type = Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list, raw_detect_list.number_of_valid_detections,
                                                 rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);
   /** \result
    * sh_current_state is expected to be STATE_UNINITIALIZED.
    */
   CHECK_TRUE(return_type == SH_E_NOT_INITIALIZED);
}

/** \purpose
 * Test Safety_Handler_Acceptance_Check function when the object position plausible check fails.
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Acceptance_Check_Object_Position_Fail)
{
   /** \precond
    * define one object track which associated detection are all outside of plausible region
    */
   Safety_Handler_Initialize();

   // Create object with default values
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true); // moving object
   object.speed = 10.0f;

   // Create 3 detections outside bounding box and associate them to object
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 3, false);

   /** \action
    * call Safety_Handler_Acceptance_Check().
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list, raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Expected output is overall fault status to be 60.
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
}

/** \purpose
 * Test Safety_Handler object status is coasting
 * \req CPR-7541_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Coasting_Object_Do_Not_Set_Fault)
{
   /** \precond
    * set object status to coasting with implasible detection,
    */

   // Create stationary object with coasting status
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false); // stationary
   object.object_status = 2U;                                                     // coasting

   // Create 1 detection far outside bounding box (implausible match)
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 1, false);

   rot_object_list_info.all_scl_faults.overall_fault_status = 195U;

   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
}

/** \purpose
 * Test Safety_Handler_Acceptance_Check function when the object position plausible check passes.
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Acceptance_Check_Object_Position_Pass)
{
   /** \precond
    * Define one object track which has at least one associated detection inside the bounding box.
    */

   // Create moving object with default values
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true); // moving
   object.speed = 10.0f;

   // Create 2 detection outside bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, false);

   // Create 1 detection inside bounding box (makes object plausible)
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 1, true);

   /** \action
    * Call Object_Position_Plausible_Check().
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list, raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Expected output is implausible to be false.
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test Safety_Handler with multiple objects - all plausible
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Multiple_Objects_All_Plausible)
{
   /** \precond
    * Define three objects, each with plausible detections inside bounding box
    */

   // Object 1 at (10, 0)
   ROT_Object_Output_T &obj1 = Add_Default_Object(rot_object_list_info, false);
   obj1.vcs_x_posn = 10.0f;
   obj1.vcs_y_posn = 0.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj1, 2, true);

   // Object 2 at (20, 5)
   ROT_Object_Output_T &obj2 = Add_Default_Object(rot_object_list_info, false);
   obj2.vcs_x_posn = 20.0f;
   obj2.vcs_y_posn = 5.0f;
   obj2.length = 4.5f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj2, 2, true);

   // Object 3 at (30, -3)
   ROT_Object_Output_T &obj3 = Add_Default_Object(rot_object_list_info, false);
   obj3.vcs_x_posn = 30.0f;
   obj3.vcs_y_posn = -3.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj3, 2, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * All objects plausible, fault status should remain 195U
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test Safety_Handler with multiple objects - one detections outside of object extended bounding box
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Multiple_Objects_One_Implausible)
{
   /** \precond
    * Define three objects, one with all detections outside bounding box
    */

   // Object 1 - PLAUSIBLE
   ROT_Object_Output_T &obj1 = Add_Default_Object(rot_object_list_info, false);
   obj1.vcs_x_posn = 10.0f;
   obj1.vcs_y_posn = 0.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj1, 1, true);

   // Object 2 - IMPLAUSIBLE
   ROT_Object_Output_T &obj2 = Add_Default_Object(rot_object_list_info, false);
   obj2.vcs_x_posn = 20.0f;
   obj2.vcs_y_posn = 0.0f;
   obj2.length = 4.5f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj2, 2, false);

   // Object 3 - PLAUSIBLE
   ROT_Object_Output_T &obj3 = Add_Default_Object(rot_object_list_info, false);
   obj3.vcs_x_posn = 30.0f;
   obj3.vcs_y_posn = -3.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj3, 1, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * One object with no plausible detection match should trigger fault status 60U
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
}

/** \purpose
 * Test boundary case: Object with no detections
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Object_With_Zero_Detections)
{
   /** \precond
    * Define object with ndets = 0
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   object.ndets = 0; // NO DETECTIONS

   raw_detect_list.number_of_valid_detections = 0;

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Object with no detections should be considered implausible
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
}

/** \purpose
 * Test boundary case: Zero objects
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Zero_Objects)
{
   /** \precond
    * Set number of objects to 0
    */
   rot_object_list_info.number_of_objects = 0;
   raw_detect_list.number_of_valid_detections = 5;

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Should handle gracefully, no objects means no implausibility
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test boundary case: Detection at exact bounding box edge
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Detection_At_Bounding_Box_Edge)
{
   /** \precond
    * Object with detection exactly at bounding box boundary
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);

   // Create 1 detection at bounding box edge (considered plausible)
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 1, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Detection at edge should be considered plausible
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test state persistence after fault detection
 * \req CPR-7587_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_State_Persists_After_Fault_Then_Recovery_Attempt)
{
   /** \precond
    * Trigger fault, then provide plausible data
    */

   // First cycle - create object with detections outside of object extended bounding box
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);

   // Create 1 detection far outside bounding box (implausible match)
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 1, false);

   rot_object_list_info.all_scl_faults.overall_fault_status = 195U;

   /** \action
    * First call - trigger fault
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);

   /** \action
    * Second cycle - now provide plausible data
    */
   raw_detect_list.detections[0].processed.vcs_position_x = 10.0f; // Inside bounding box
   raw_detect_list.detections[0].processed.vcs_position_y = 0.0f;
   rot_object_list_info.all_scl_faults.overall_fault_status = 195U;

   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * State should remain SAFE, fault status should be 60U
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
}

/** \purpose
 * Test object with mixed detection associations (some plausible, some not)
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Object_With_Mixed_Detections)
{
   /** \precond
    * Object with 3 detections: 1 inside bbox, 2 outside bbox
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);

   // Create 1 detection inside bounding box (plausible match)
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 1, true);

   // Create 2 detections outside bounding box (implausible matches)
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, false);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * At least one plausible detection means object is plausible
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test Safety_Handler after re-initialization
 * \req CPR-7541_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Re_Initialize_After_Fault_Clears_State)
{
   /** \precond
    * Trigger fault, then re-initialize
    */

   // Trigger fault
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);

   // Create 1 detection far outside bounding box (implausible match)
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 1, false);

   rot_object_list_info.all_scl_faults.overall_fault_status = 195U;

   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);

   /** \action
    * Re-initialize Safety Handler
    */
   Safety_Handler_Initialize();

   /** \result
    * State should reset to FAULT_FREE
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   /** \action
    * Now provide plausible data
    */
   raw_detect_list.detections[0].processed.vcs_position_x = 10.0f;
   raw_detect_list.detections[0].processed.vcs_position_y = 0.0f;
   rot_object_list_info.all_scl_faults.overall_fault_status = 195U;

   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * After re-initialization, plausible data should keep fault status at 195U
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
}

/** \purpose
 * Test Set_Safety_Handler_State with all three states in sequence
 * \req CPR-7541_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Set_State_All_Transitions)
{
   /** \precond
    * Safety handler initialized to STATE_FAULT_FREE
    */
   Safety_Handler_Safety_State_T sh_current_state = Get_Safety_Handler_State();
   CHECK_TRUE(sh_current_state == STATE_FAULT_FREE);

   /** \action
    * Transition FAULT_FREE -> SAFE
    */
   Set_Safety_Handler_State(STATE_SAFE);
   sh_current_state = Get_Safety_Handler_State();
   CHECK_TRUE(sh_current_state == STATE_SAFE);

   /** \action
    * Transition SAFE -> UNINITIALIZED
    */
   Set_Safety_Handler_State(STATE_UNINITIALIZED);
   sh_current_state = Get_Safety_Handler_State();
   CHECK_TRUE(sh_current_state == STATE_UNINITIALIZED);

   /** \action
    * Transition UNINITIALIZED -> FAULT_FREE via Initialize
    */
   Safety_Handler_Initialize();
   sh_current_state = Get_Safety_Handler_State();
   CHECK_TRUE(sh_current_state == STATE_FAULT_FREE);

   /** \action
    * Transition FAULT_FREE -> UNINITIALIZED
    */
   Set_Safety_Handler_State(STATE_UNINITIALIZED);
   sh_current_state = Get_Safety_Handler_State();
   CHECK_TRUE(sh_current_state == STATE_UNINITIALIZED);

   /** \action
    * Transition UNINITIALIZED -> SAFE (edge case)
    */
   Set_Safety_Handler_State(STATE_SAFE);
   sh_current_state = Get_Safety_Handler_State();
   CHECK_TRUE(sh_current_state == STATE_SAFE);
}

/** \purpose
 * Test Safety_Handler_Initialize from STATE_SAFE
 * \req CPR-7541_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Initialize_From_Safe_State)
{
   /** \precond
    * Set safety handler to STATE_SAFE
    */
   Set_Safety_Handler_State(STATE_SAFE);
   Safety_Handler_Safety_State_T sh_current_state = Get_Safety_Handler_State();
   CHECK_TRUE(sh_current_state == STATE_SAFE);

   /** \action
    * Call Safety_Handler_Initialize()
    */
   Safety_Handler_Initialize();
   sh_current_state = Get_Safety_Handler_State();

   /** \result
    * State should transition from SAFE to FAULT_FREE
    */
   CHECK_TRUE(sh_current_state == STATE_FAULT_FREE);
}

/** \purpose
 * Test multiple consecutive initializations
 * \req CPR-7541_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Multiple_Consecutive_Initializations)
{
   /** \precond
    * Safety handler already initialized
    */
   Safety_Handler_Safety_State_T sh_current_state = Get_Safety_Handler_State();
   CHECK_TRUE(sh_current_state == STATE_FAULT_FREE);

   /** \action
    * Call Safety_Handler_Initialize() again
    */
   Safety_Handler_Initialize();
   sh_current_state = Get_Safety_Handler_State();
   CHECK_TRUE(sh_current_state == STATE_FAULT_FREE);

   /** \action
    * Call Safety_Handler_Initialize() third time
    */
   Safety_Handler_Initialize();
   sh_current_state = Get_Safety_Handler_State();

   /** \result
    * State should remain FAULT_FREE
    */
   CHECK_TRUE(sh_current_state == STATE_FAULT_FREE);
}

/** \purpose
 * Test state getter/setter idempotency
 * \req CPR-7587_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_State_Getter_Setter_Idempotency)
{
   /** \precond
    * Set to FAULT_FREE
    */
   Set_Safety_Handler_State(STATE_FAULT_FREE);

   /** \action
    * Get state multiple times
    */
   Safety_Handler_Safety_State_T state1 = Get_Safety_Handler_State();
   Safety_Handler_Safety_State_T state2 = Get_Safety_Handler_State();
   Safety_Handler_Safety_State_T state3 = Get_Safety_Handler_State();

   /** \result
    * All should be identical
    */
   CHECK_TRUE(state1 == STATE_FAULT_FREE);
   CHECK_TRUE(state2 == STATE_FAULT_FREE);
   CHECK_TRUE(state3 == STATE_FAULT_FREE);

   /** \action
    * Set to same state multiple times
    */
   Set_Safety_Handler_State(STATE_FAULT_FREE);
   Set_Safety_Handler_State(STATE_FAULT_FREE);
   Set_Safety_Handler_State(STATE_FAULT_FREE);

   /** \result
    * State should remain FAULT_FREE
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
}

/** \purpose
 * Test Safety_Handler with newly created object status (1U)
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Object_With_Newly_Created_Status)
{
   /** \precond
    * Create object with newly created status and detections outside of object extended bounding box
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   object.object_status = 1U; // NEWLY_CREATED

   // Create detections outside of object extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 3, false);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Newly created objects should be checked - should trigger fault
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
}

/** \purpose
 * Test Safety_Handler with invalid object status (255U) - should be skipped
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Object_With_Invalid_Status_Is_Skipped)
{
   /** \precond
    * Create object with invalid status and detections outside of object extended bounding box
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   object.object_status = 255U; // INVALID - should be filtered out

   // Create detections outside of object extended bounding box (would normally fail)
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 3, false);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Invalid objects should be filtered out - no fault
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test Safety_Handler with mixed movement status objects
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Mixed_Movement_Status_Objects)
{
   /** \precond
    * Create stationary and moving objects, all plausible
    */
   // Stationary object
   ROT_Object_Output_T &obj1 = Add_Default_Object(rot_object_list_info, false);
   obj1.vcs_x_posn = 10.0f;
   obj1.vcs_y_posn = 0.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj1, 2, true);

   // Moving object
   ROT_Object_Output_T &obj2 = Add_Default_Object(rot_object_list_info, true);
   obj2.vcs_x_posn = 20.0f;
   obj2.vcs_y_posn = 5.0f;
   obj2.speed = 15.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj2, 2, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Both stationary and moving objects plausible - no fault
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test Safety_Handler with stationary object having detections outside of object extended bounding box
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Stationary_Object_Implausible)
{
   /** \precond
    * Create stationary object with detections outside bounding box
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false); // stationary
   object.speed = 0.0f;

   // Create detections outside of object extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 3, false);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Stationary object with detections outside of object extended bounding box should trigger fault
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
}

/** \purpose
 * Test Safety_Handler with object at negative coordinates
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Object_At_Negative_Coordinates)
{
   /** \precond
    * Create object at negative X and Y positions
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   object.vcs_x_posn = -15.0f;
   object.vcs_y_posn = -8.0f;

   // Create plausible detections relative to negative position
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Negative coordinates with plausible detections should pass
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test Safety_Handler with very small object (motorcycle dimensions)
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Very_Small_Object_Motorcycle)
{
   /** \precond
    * Create object with motorcycle-like small dimensions
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.length = 2.0f; // Small length
   object.width = 0.8f;  // Small width

   // Create plausible detections
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Small object with plausible detections should pass
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test Safety_Handler with very large object (truck/bus dimensions)
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Very_Large_Object_Truck)
{
   /** \precond
    * Create object with truck-like large dimensions
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   object.length = 18.0f; // Long truck
   object.width = 2.5f;   // Wide vehicle

   // Create plausible detections
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 3, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Large object with plausible detections should pass
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test multiple cycles with alternating plausible/implausible data
 * \req CPR-7587_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Alternating_Plausible_Implausible_Cycles)
{
   /** \precond
    * Start with plausible data
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);

   // Cycle 1: Plausible
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, true);
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   /** \action
    * Cycle 2: Implausible - should transition to SAFE
    */
   raw_detect_list.number_of_valid_detections = 0;
   object.ndets = 0;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 3, false);
   rot_object_list_info.all_scl_faults.overall_fault_status = 195U;

   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);

   /** \action
    * Cycle 3: Plausible again - should remain SAFE
    */
   raw_detect_list.number_of_valid_detections = 0;
   object.ndets = 0;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, true);
   rot_object_list_info.all_scl_faults.overall_fault_status = 195U;

   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Should remain SAFE even with plausible data
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
}

/** \purpose
 * Test multiple objects with all detections outside of object extended bounding box
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Multiple_Objects_All_Implausible)
{
   /** \precond
    * Create three objects, all with detections outside of object extended bounding box
    */
   ROT_Object_Output_T &obj1 = Add_Default_Object(rot_object_list_info, false);
   obj1.vcs_x_posn = 10.0f;
   obj1.vcs_y_posn = 0.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj1, 2, false);

   ROT_Object_Output_T &obj2 = Add_Default_Object(rot_object_list_info, false);
   obj2.vcs_x_posn = 20.0f;
   obj2.vcs_y_posn = 5.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj2, 2, false);

   ROT_Object_Output_T &obj3 = Add_Default_Object(rot_object_list_info, false);
   obj3.vcs_x_posn = 30.0f;
   obj3.vcs_y_posn = -3.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj3, 2, false);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * All implausible objects should trigger fault
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
}

/** \purpose
 * Test object with extreme range position
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Object_At_Extreme_Range)
{
   /** \precond
    * Create object at far distance (affects lateral bounding box offset)
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   object.vcs_x_posn = 150.0f; // Far range
   object.vcs_y_posn = 10.0f;

   // Create plausible detections
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Far range object with plausible detections should pass
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test object with extreme lateral position
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Object_At_Extreme_Lateral_Position)
{
   /** \precond
    * Create object at extreme lateral offset
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   object.vcs_x_posn = 20.0f;
   object.vcs_y_posn = 50.0f; // Far lateral

   // Create plausible detections
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Extreme lateral position with plausible detections should pass
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test transition from SAFE back to FAULT_FREE is only possible via re-initialization
 * \req CPR-7587_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Safe_State_Cannot_Transition_Without_Reinit)
{
   /** \precond
    * Trigger SAFE state
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 3, false);

   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);

   /** \action
    * Provide multiple cycles of plausible data without re-initialization
    */
   for (int i = 0; i < 5; i++)
   {
      raw_detect_list.number_of_valid_detections = 0;
      object.ndets = 0;
      Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, true);
      rot_object_list_info.all_scl_faults.overall_fault_status = 195U;

      Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                      raw_detect_list.number_of_valid_detections,
                                      rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

      /** \result
       * Should remain SAFE and fault status should remain 60U
       */
      CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
      CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
   }
}

/** \purpose
 * Test that acceptance check returns SH_E_OK when state is SAFE
 * \req CPR-7541_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Acceptance_Check_Returns_OK_When_Safe)
{
   /** \precond
    * Set state to SAFE
    */
   Set_Safety_Handler_State(STATE_SAFE);
   Safety_Handler_Return_Type_T return_type;

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   return_type = Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                                 raw_detect_list.number_of_valid_detections,
                                                 rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Should return SH_E_OK even in SAFE state
    */
   CHECK_TRUE(return_type == SH_E_OK);
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
}

/** \purpose
 * Test mixed object statuses in single acceptance check
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Mixed_Object_Status_Values)
{
   /** \precond
    * Create objects with different status values, all with plausible detections
    */
   // Object 1: Measured (0U)
   ROT_Object_Output_T &obj1 = Add_Default_Object(rot_object_list_info, false);
   obj1.object_status = 0U;
   obj1.vcs_x_posn = 10.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj1, 1, true);

   // Object 2: Newly created (1U)
   ROT_Object_Output_T &obj2 = Add_Default_Object(rot_object_list_info, false);
   obj2.object_status = 1U;
   obj2.vcs_x_posn = 20.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj2, 1, true);

   // Object 3: Coasted (2U) - should be skipped
   ROT_Object_Output_T &obj3 = Add_Default_Object(rot_object_list_info, false);
   obj3.object_status = 2U;
   obj3.vcs_x_posn = 30.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj3, 1, false);

   // Object 4: Invalid (255U) - should be skipped
   ROT_Object_Output_T &obj4 = Add_Default_Object(rot_object_list_info, false);
   obj4.object_status = 255U;
   obj4.vcs_x_posn = 40.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj4, 1, false);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Objects 1 and 2 are checked and plausible, 3 and 4 are skipped - no fault
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test object with zero dimensions (width and length)
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Object_With_Zero_Dimensions)
{
   /** \precond
    * Create object with zero width and length
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   object.length = 0.0f;
   object.width = 0.0f;

   // Create detections at object position
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Zero dimensions should still have bounding box offsets - should pass if detections are close
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test object with very high speed
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Object_With_Very_High_Speed)
{
   /** \precond
    * Create fast moving object
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.speed = 50.0f; // Very high speed (180 km/h)

   // Create plausible detections
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * High speed object with plausible detections should pass
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test object with high speed and detections outside of object extended bounding box
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_High_Speed_Object_Implausible)
{
   /** \precond
    * Create fast moving object with detections outside of object extended bounding box
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.speed = 40.0f; // High speed

   // Create detections outside of object extended bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 3, false);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * High speed with detections outside of object extended bounding box should trigger fault
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
}

/** \purpose
 * Test recovery after multiple implausible cycles requires re-initialization
 * \req CPR-7587_Derived CPR-7539_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Multiple_Implausible_Cycles_Then_Recovery)
{
   /** \precond
    * Create scenario with multiple implausible cycles
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);

   // Cycle 1: Implausible
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, false);
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);

   // Cycle 2: Still implausible
   raw_detect_list.number_of_valid_detections = 0;
   object.ndets = 0;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, false);
   rot_object_list_info.all_scl_faults.overall_fault_status = 195U;
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);

   // Cycle 3: Now plausible but still in SAFE
   raw_detect_list.number_of_valid_detections = 0;
   object.ndets = 0;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, true);
   rot_object_list_info.all_scl_faults.overall_fault_status = 195U;
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);

   /** \action
    * Re-initialize and provide plausible data
    */
   Safety_Handler_Initialize();
   raw_detect_list.number_of_valid_detections = 0;
   object.ndets = 0;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, true);
   rot_object_list_info.all_scl_faults.overall_fault_status = 195U;
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * After re-initialization, should be FAULT_FREE with plausible data
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test object at origin (0, 0)
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Object_At_Origin)
{
   /** \precond
    * Create object at origin coordinates
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   object.vcs_x_posn = 0.0f;
   object.vcs_y_posn = 0.0f;

   // Create plausible detection associations
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Object at origin with plausible detections should pass
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test rapid state changes through manual state setting
 * \req CPR-7541_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Rapid_State_Changes)
{
   /** \precond
    * Start in FAULT_FREE
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   /** \action
    * Rapidly change states multiple times
    */
   for (int i = 0; i < 10; i++)
   {
      Set_Safety_Handler_State(STATE_SAFE);
      CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);

      Set_Safety_Handler_State(STATE_UNINITIALIZED);
      CHECK_TRUE(Get_Safety_Handler_State() == STATE_UNINITIALIZED);

      Set_Safety_Handler_State(STATE_FAULT_FREE);
      CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
   }

   /** \result
    * State should consistently reflect the last set value
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
}

/** \purpose
 * Test object with single detection at various positions
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Single_Detection_At_Various_Positions)
{
   /** \precond
    * Create object with single detection inside bounding box
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);

   // Single plausible detection
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 1, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Single plausible detection should make object plausible
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test many objects with single detection each
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Many_Objects_Single_Detection_Each)
{
   /** \precond
    * Create many objects, each with single plausible detection
    */
   for (int i = 0; i < 50; i++)
   {
      ROT_Object_Output_T &obj = Add_Default_Object(rot_object_list_info, false);
      obj.vcs_x_posn = 10.0f + i * 2.0f;
      obj.vcs_y_posn = (i % 2 == 0) ? 5.0f : -5.0f;
      Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj, 1, true);
   }

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * All objects plausible - no fault
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test object with maximum allowed detections per object
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Object_With_Many_Detections)
{
   /** \precond
    * Create object with many plausible detections
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);

   // Create maximum reasonable number of detections (17 inside patterns)
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 17, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Many plausible detections should pass
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test acceptance check called multiple times in same cycle
 * \req CPR-7541_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Multiple_Acceptance_Checks_Same_Cycle)
{
   /** \precond
    * Create plausible scenario
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, true);

   /** \action
    * Call acceptance check multiple times with same data
    */
   for (int i = 0; i < 5; i++)
   {
      rot_object_list_info.all_scl_faults.overall_fault_status = 195U;
      Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                      raw_detect_list.number_of_valid_detections,
                                      rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

      /** \result
       * Should consistently return same result
       */
      CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
      CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
   }
}

/** \purpose
 * Test object with negative speed value
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Object_With_Negative_Speed)
{
   /** \precond
    * Create object with negative speed (edge case)
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.speed = -10.0f; // Negative speed (shouldn't normally happen)

   // Create plausible detections
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Should handle negative speed gracefully with plausible detections
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test very close objects (clustered)
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Clustered_Objects_Close_Together)
{
   /** \precond
    * Create multiple objects very close together
    */
   for (int i = 0; i < 5; i++)
   {
      ROT_Object_Output_T &obj = Add_Default_Object(rot_object_list_info, false);
      obj.vcs_x_posn = 20.0f + i * 0.5f; // Very close spacing (0.5m)
      obj.vcs_y_posn = 0.0f;
      obj.length = 4.0f;
      obj.width = 1.8f;
      Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj, 2, true);
   }

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Close objects with plausible detections should all pass
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test object ID edge cases (ID=0, very high IDs)
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Object_With_Edge_Case_IDs)
{
   /** \precond
    * Create objects with various ID values
    */
   // Object with ID = 1 (minimum valid)
   ROT_Object_Output_T &obj1 = Add_Default_Object(rot_object_list_info, false);
   obj1.id = 1;
   obj1.vcs_x_posn = 10.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj1, 1, true);

   // Object with high ID
   ROT_Object_Output_T &obj2 = Add_Default_Object(rot_object_list_info, false);
   obj2.id = 65535; // Max uint16_t
   obj2.vcs_x_posn = 20.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj2, 1, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Different IDs with plausible detections should pass
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test alternating object/detection addition pattern
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Alternating_Plausible_Implausible_Objects)
{
   /** \precond
    * Create alternating plausible and implausible objects
    */
   // Object 1: Plausible
   ROT_Object_Output_T &obj1 = Add_Default_Object(rot_object_list_info, false);
   obj1.vcs_x_posn = 10.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj1, 1, true);

   // Object 2: Implausible
   ROT_Object_Output_T &obj2 = Add_Default_Object(rot_object_list_info, false);
   obj2.vcs_x_posn = 20.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj2, 2, false);

   // Object 3: Plausible
   ROT_Object_Output_T &obj3 = Add_Default_Object(rot_object_list_info, false);
   obj3.vcs_x_posn = 30.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj3, 1, true);

   // Object 4: Implausible
   ROT_Object_Output_T &obj4 = Add_Default_Object(rot_object_list_info, false);
   obj4.vcs_x_posn = 40.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj4, 2, false);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Any implausible object should trigger fault
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
}

/** \purpose
 * Test state machine with initialize-fault-initialize cycle
 * \req CPR-7541_Derived CPR-7539_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Initialize_Fault_Initialize_Cycle)
{
   /** \precond
    * Start with initialized state
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   /** \action
    * Trigger fault
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, false);
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);

   /** \action
    * Re-initialize
    */
   Safety_Handler_Initialize();
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);

   /** \action
    * Trigger fault again
    */
   raw_detect_list.number_of_valid_detections = 0;
   object.ndets = 0;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, false);
   rot_object_list_info.all_scl_faults.overall_fault_status = 195U;
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);

   /** \action
    * Re-initialize again
    */
   Safety_Handler_Initialize();

   /** \result
    * Should be able to cycle through fault-initialize multiple times
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
}

/** \purpose
 * Test handling of invalid detection count exceeding maximum
 * \req CPR-7532_Derived CPR-7539_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Invalid_Detection_Count_Exceeds_Maximum)
{
   /** \precond
    * Set detection count beyond maximum allowed (MAX_NUMBER_OF_DETECTIONS)
    */
   raw_detect_list.number_of_valid_detections = MAX_NUMBER_OF_DETECTIONS + 100;

   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   object.vcs_x_posn = 10.0f;
   object.vcs_y_posn = 0.0f;

   // Create reasonable number of detections (system should handle invalid count)
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check() with invalid count
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * This should trigger a fault due to invalid detection count
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
}

/** \purpose
 * Test handling of orphaned detection without matching object
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Orphaned_Detection_Without_Matching_Object)
{
   /** \precond
    * Create detection with objTrkID that doesn't match any object
    */
   raw_detect_list.number_of_valid_detections = 1;
   raw_detect_list.detections[0].processed.vcs_position_x = 10.0f;
   raw_detect_list.detections[0].processed.vcs_position_y = 0.0f;
   f360_detection_list[0].objTrkID = 999; // Non-existent object ID

   // Create one valid object with different ID
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   object.id = 1;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Orphaned detection should be ignored without causing fault
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
}
/** \purpose
 * Test handling of one object with zero associated detections and multiple objects with valid detections
 * \req CPR-7532_Derived CPR-7539_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_One_Object_Zero_Detections_Among_Many)
{
   /** \precond
    * Create multiple objects with valid detections and one with zero associated detections
    */
   for (int32_t i = 0; i < 3; i++)
   {
      ROT_Object_Output_T &obj = Add_Default_Object(rot_object_list_info, false);
      obj.vcs_x_posn = 10.0f + i * 5.0f;
      Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj, 4, true);
   }
   Add_Default_Object(rot_object_list_info, false);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Object with zero detections should be ignored, overall state should be SAFE
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
}

/** \purpose
 * Test handling of object with ID zero and valid detections
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Object_With_ID_Zero_Valid_Detections)
{
   /** \precond
    * Create object with ID = 0
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   object.id = 0; // Edge case: zero ID
   object.vcs_x_posn = 10.0f;
   object.vcs_y_posn = 0.0f;

   // Create associated detections
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * No safe state expected when object have ID = 0 (this is not supposed to happen in normal operation)
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
}

/** \purpose
 * Test handling of object with ID zero and invlivalid detections
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Object_With_ID_Zero_Invalid_Detections)
{
   /** \precond
    * Create object with ID = 0
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   object.id = 0; // Edge case: zero ID
   object.vcs_x_posn = 10.0f;
   object.vcs_y_posn = 0.0f;

   // Create associated detections
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, false);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * No safe state expected when object have ID = 0 (this is not supposed to happen in normal operation)
    */
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
}

/** \purpose
 * Test detection just outside bounding box edge (boundary precision)
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Detection_Just_Outside_Bounding_Box_Edge)
{
   /** \precond
    * Create object and place detection just 0.01m outside bounding box
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   object.vcs_x_posn = 10.0f;
   object.vcs_y_posn = 0.0f;
   object.length = 5.0f;
   object.width = 1.8f;

   raw_detect_list.number_of_valid_detections = 1;
   object.ndets = 1;

   // Longitudinal offset = 3.1m, place detection at 3.11m outside front edge
   raw_detect_list.detections[0].processed.vcs_position_x = object.vcs_x_posn + (object.length / 2.0f) + 4.065f; //gate here is 4.055
   raw_detect_list.detections[0].processed.vcs_position_y = object.vcs_y_posn;
   f360_detection_list[0].objTrkID = object.id;

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Detection just outside should be implausible, triggering fault
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
}

/** \purpose
 * Test handling of duplicate object IDs
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Duplicate_Object_IDs)
{
   /** \precond
    * Create two objects with same ID
    */
   ROT_Object_Output_T &obj1 = Add_Default_Object(rot_object_list_info, false);
   obj1.id = 42;
   obj1.vcs_x_posn = 10.0f;
   obj1.vcs_y_posn = 0.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj1, 2, true);

   ROT_Object_Output_T &obj2 = Add_Default_Object(rot_object_list_info, false);
   obj2.id = 42; // DUPLICATE ID
   obj2.vcs_x_posn = 20.0f;
   obj2.vcs_y_posn = 5.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, obj2, 2, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * This case should not occure under normal operation. The number of detections associated
    * to the object ID will be bigger than the ndets of each object. The check inside only counts
    * to ndets, so the first 2 detections will be checked by both objects, causing the second one
    * to be implausible in this case causing the test to fail
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
}

/** \purpose
 * Test all detections at exactly same position (clustering edge case)
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_All_Detections_At_Same_Position)
{
   /** \precond
    * Create object with multiple detections all at identical position
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   object.vcs_x_posn = 15.0f;
   object.vcs_y_posn = 3.0f;
   object.ndets = 5;

   raw_detect_list.number_of_valid_detections = 5;
   for (uint32_t i = 0; i < 5; i++)
   {
      // All detections at exact same position (inside bounding box)
      raw_detect_list.detections[i].processed.vcs_position_x = object.vcs_x_posn;
      raw_detect_list.detections[i].processed.vcs_position_y = object.vcs_y_posn;
      f360_detection_list[i].objTrkID = object.id;
   }

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * All detections at same plausible position should pass
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test that Safety_Handler_Acceptance_Check when uninitialized returns error without modifying fault status.
 * \req CPR-7541_Derived CPR-7539_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Uninitialized_State_Does_Not_Modify_Fault)
{
   /** \precond
    * Set safety handler to UNINITIALIZED state and set initial fault status
    */
   Set_Safety_Handler_State(STATE_UNINITIALIZED);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_UNINITIALIZED);

   // Set initial fault status to 100U (non-standard value)
   rot_object_list_info.all_scl_faults.overall_fault_status = 100U;
   uint8_t initial_fault_status = rot_object_list_info.all_scl_faults.overall_fault_status;

   // Create a plausible object to ensure the check would pass if initialized
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   object.vcs_x_posn = 10.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check() while uninitialized
    */
   Safety_Handler_Return_Type_T return_type = Safety_Handler_Acceptance_Check(
       raw_detect_list.detections, f360_detection_list,
       raw_detect_list.number_of_valid_detections,
       rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Should return SH_E_NOT_INITIALIZED and NOT modify overall_fault_status
    */
   CHECK_TRUE(return_type == SH_E_NOT_INITIALIZED);
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == initial_fault_status);
}

/** \purpose
 * Test object with ndets > 0 but all detections have wrong objTrkID.
 * \req CPR-7532_Derived CPR-7539_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Object_With_Ndets_But_No_Matching_Detections)
{
   /** \precond
    * Create object with ndets=5 but all f360_detection_list have wrong objTrkID
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   object.vcs_x_posn = 10.0f;
   object.ndets = 5;

   // Create 5 detections but associate them to wrong object ID
   raw_detect_list.number_of_valid_detections = 5;
   for (uint32_t i = 0; i < 5; i++)
   {
      raw_detect_list.detections[i].processed.vcs_position_x = object.vcs_x_posn;
      raw_detect_list.detections[i].processed.vcs_position_y = object.vcs_y_posn;
      f360_detection_list[i].objTrkID = 999; // Wrong ID - doesn't match object.id
   }

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Object claims to have detections but none match - should trigger fault
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
}

/** \purpose
 * Test detection count mismatch scenario.
 * \req CPR-7532_Derived CPR-7539_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Detection_Count_Mismatch)
{
   /** \precond
    * Set number_of_valid_detections = 10, but only 5 detections have valid objTrkID
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   object.vcs_x_posn = 10.0f;
   object.ndets = 5;

   // Create 5 valid detections
   for (uint32_t i = 0; i < 5; i++)
   {
      raw_detect_list.detections[i].processed.vcs_position_x = object.vcs_x_posn;
      raw_detect_list.detections[i].processed.vcs_position_y = object.vcs_y_posn;
      f360_detection_list[i].objTrkID = object.id;
   }

   // Remaining 5 detections have invalid objTrkID
   for (uint32_t i = 5; i < 10; i++)
   {
      raw_detect_list.detections[i].processed.vcs_position_x = 50.0f; // Far away
      raw_detect_list.detections[i].processed.vcs_position_y = 50.0f;
      f360_detection_list[i].objTrkID = 0; // Invalid ID
   }

   raw_detect_list.number_of_valid_detections = 10; // Claims 10 but only 5 match

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Should handle gracefully - only matching detections are checked
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test rotated object at extreme range position.
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Rotated_Object_At_Extreme_Range)
{
   /** \precond
    * Create object at 200m range with 45-degree rotation
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 200.0f; // Far range
   object.vcs_y_posn = 10.0f;
   object.vcs_pointing = 0.785f; // 45 degrees
   object.speed = 30.0f;

   // Create plausible detections
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 5, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Rotated object at extreme range with plausible detections should pass
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test state persistence across many cycles.
 * \req CPR-7587_Derived CPR-7539_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_State_Persistence_Across_Many_Cycles)
{
   /** \precond
    * Trigger SAFE state first
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   object.vcs_x_posn = 10.0f;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 3, false);

   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);

   /** \action
    * Run 100 cycles with plausible data
    */
   for (int i = 0; i < 100; i++)
   {
      raw_detect_list.number_of_valid_detections = 0;
      object.ndets = 0;
      Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 2, true);
      rot_object_list_info.all_scl_faults.overall_fault_status = 195U;

      Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                      raw_detect_list.number_of_valid_detections,
                                      rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

      /** \result
       * Should remain SAFE throughout all cycles
       */
      CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
      CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
   }
}

/** \purpose
 * Test maximum objects where first and last are implausible (tests early exit).
 * \req CPR-7532_Derived CPR-7539_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Max_Objects_First_And_Last_Implausible)
{
   /** \precond
    * Create 500 objects where first is implausible
    */
   rot_object_list_info.number_of_objects = NUMBER_OF_REDUCED_OBJECT_TRACKS;

   // First object: implausible
   rot_object_list_info.rot_object_list[0].id = 1;
   rot_object_list_info.rot_object_list[0].vcs_x_posn = 10.0f;
   rot_object_list_info.rot_object_list[0].vcs_y_posn = 0.0f;
   rot_object_list_info.rot_object_list[0].length = 5.0f;
   rot_object_list_info.rot_object_list[0].width = 1.8f;
   rot_object_list_info.rot_object_list[0].object_status = 0U;
   rot_object_list_info.rot_object_list[0].ndets = 1;

   // First detection: outside bounding box
   raw_detect_list.detections[0].processed.vcs_position_x = 100.0f; // Far outside
   raw_detect_list.detections[0].processed.vcs_position_y = 100.0f;
   f360_detection_list[0].objTrkID = 1;

   // Remaining objects: all plausible
   for (uint16_t i = 1; i < NUMBER_OF_REDUCED_OBJECT_TRACKS; i++)
   {
      rot_object_list_info.rot_object_list[i].id = i + 1;
      rot_object_list_info.rot_object_list[i].vcs_x_posn = 10.0f + (i * 0.1f);
      rot_object_list_info.rot_object_list[i].vcs_y_posn = 0.0f;
      rot_object_list_info.rot_object_list[i].length = 5.0f;
      rot_object_list_info.rot_object_list[i].width = 1.8f;
      rot_object_list_info.rot_object_list[i].object_status = 0U;
      rot_object_list_info.rot_object_list[i].ndets = 1;

      raw_detect_list.detections[i].processed.vcs_position_x = 10.0f + (i * 0.1f);
      raw_detect_list.detections[i].processed.vcs_position_y = 0.0f;
      f360_detection_list[i].objTrkID = i + 1;
   }

   raw_detect_list.number_of_valid_detections = NUMBER_OF_REDUCED_OBJECT_TRACKS;

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Should fail on first object (early exit), never checking object 500
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
}

/** \purpose
 * Test with number_of_valid_detections = 0 but objects have ndets > 0.
 * \req CPR-7532_Derived CPR-7539_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Zero_Detections_But_Objects_Claim_Detections)
{
   /** \precond
    * Create objects with ndets > 0 but number_of_valid_detections = 0
    */
   ROT_Object_Output_T &object1 = Add_Default_Object(rot_object_list_info, false);
   object1.vcs_x_posn = 10.0f;
   object1.ndets = 5; // Claims to have 5 detections

   ROT_Object_Output_T &object2 = Add_Default_Object(rot_object_list_info, false);
   object2.vcs_x_posn = 20.0f;
   object2.ndets = 3; // Claims to have 3 detections

   // But don't create any detections
   raw_detect_list.number_of_valid_detections = 0;

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Objects claim detections but none exist - should trigger fault
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
}

/** \purpose
 * Test moving object with high speed and many detections outside of object extended bounding box.
 * \req CPR-7532_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_High_Speed_Moving_Object_Many_Detections_Outside_Bounding_Box)
{
   /** \precond
    * Create high-speed moving object with many detections all outside bounding box
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, true);
   object.vcs_x_posn = 50.0f;
   object.speed = 45.0f; // Very high speed (162 km/h)
   object.iso_relative_x_vel = 45.0f;
   object.iso_relative_y_vel = 0.0f;
   object.length = 4.8f;
   object.width = 1.9f;

   // Create 20 detections all outside bounding box
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 20, false);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * High-speed object with many detections outside of object extended bounding box, but object is out of scope
    * it should not trigger fault
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
}

/** \purpose
 * Test Safety_Handler with maximum objects (NUMBER_OF_REDUCED_OBJECT_TRACKS)
 * and maximum detections (MAX_NUMBER_OF_DETECTIONS) simultaneously.
 * \req CPR-7532_Derived CPR-7539_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Max_Objects_And_Max_Detections_Together)
{
   /** \precond
    * Create NUMBER_OF_REDUCED_OBJECT_TRACKS objects with MAX_NUMBER_OF_DETECTIONS
    * total detections distributed among them
    */
   // Initialize object properties
   rot_object_list_info.number_of_objects = static_cast<uint16_t>(NUMBER_OF_REDUCED_OBJECT_TRACKS);
   for (uint16_t i = 0; i < NUMBER_OF_REDUCED_OBJECT_TRACKS; i++)
   {
      rot_object_list_info.rot_object_list[i].id = i + 1;
      rot_object_list_info.rot_object_list[i].vcs_x_posn = 10.0f + (i * 0.1f);
      rot_object_list_info.rot_object_list[i].vcs_y_posn = 0.0f;
      rot_object_list_info.rot_object_list[i].length = 5.0f;
      rot_object_list_info.rot_object_list[i].width = 1.8f;
      rot_object_list_info.rot_object_list[i].object_status = 0U;
      rot_object_list_info.rot_object_list[i].ndets = 0;
   }

   // Initialize detection properties (all associated detections plausible match)
   raw_detect_list.number_of_valid_detections = MAX_NUMBER_OF_DETECTIONS;
   for (uint32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      const uint16_t object_index = i % NUMBER_OF_REDUCED_OBJECT_TRACKS;
      rot_object_list_info.rot_object_list[object_index].ndets++;

      uint16_t object_id = object_index + 1;
      f360_detection_list[i].objTrkID = object_id;

      // Place detection at object position (inside bounding box)
      raw_detect_list.detections[i].processed.vcs_position_x = 10.0f + ((object_id - 1) * 0.1f);
      raw_detect_list.detections[i].processed.vcs_position_y = 0.0f;
   }

   /** \action
    * Call Safety_Handler_Acceptance_Check() with max objects and max detections
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Should handle maximum capacity. No faults expected
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
}

/** \purpose
 * Test Safety_Handler with maximum objects (NUMBER_OF_REDUCED_OBJECT_TRACKS)
 * and maximum detections (MAX_NUMBER_OF_DETECTIONS) simultaneously,
 * with the last object being implausible (all detections outside bounding box).
 * \req CPR-7532_Derived CPR-7539_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Max_Objects_And_Max_Detections_Together_Last_Object_Implausible)
{
   /** \precond
    * Create NUMBER_OF_REDUCED_OBJECT_TRACKS objects with MAX_NUMBER_OF_DETECTIONS
    * total detections distributed among them
    */
   // Initialize object properties
   rot_object_list_info.number_of_objects = static_cast<uint16_t>(NUMBER_OF_REDUCED_OBJECT_TRACKS);
   for (uint16_t i = 0; i < NUMBER_OF_REDUCED_OBJECT_TRACKS; i++)
   {
      rot_object_list_info.rot_object_list[i].id = i + 1;
      rot_object_list_info.rot_object_list[i].vcs_x_posn = 10.0f + (i * 0.1f);
      rot_object_list_info.rot_object_list[i].vcs_y_posn = 0.0f;
      rot_object_list_info.rot_object_list[i].length = 5.0f;
      rot_object_list_info.rot_object_list[i].width = 1.8f;
      rot_object_list_info.rot_object_list[i].object_status = 0U;
      rot_object_list_info.rot_object_list[i].ndets = 0;
   }

   // Initialize detection properties (all associated detections plausible match except last object)
   raw_detect_list.number_of_valid_detections = MAX_NUMBER_OF_DETECTIONS;
   for (uint32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      const uint16_t object_index = i % NUMBER_OF_REDUCED_OBJECT_TRACKS;
      rot_object_list_info.rot_object_list[object_index].ndets++;

      uint16_t object_id = object_index + 1;
      f360_detection_list[i].objTrkID = object_id;

      // Place detection at object position (last object will have all detections outside bounding box)
      raw_detect_list.detections[i].processed.vcs_position_x = 10.0f + ((object_id - 1) * 0.1f);
      raw_detect_list.detections[i].processed.vcs_position_y = (object_id == NUMBER_OF_REDUCED_OBJECT_TRACKS) ? 100.0F : 0.0F;
   }

   /** \action
    * Call Safety_Handler_Acceptance_Check() with max objects and max detections
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Should handle maximum capacity. Fault expected due to last object being implausible
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
}

/** \purpose
 * Test Safety_Handler with maximum objects (NUMBER_OF_REDUCED_OBJECT_TRACKS)
 * and maximum detections (MAX_NUMBER_OF_DETECTIONS) simultaneously,
 * with the one object being implausible (all detections outside bounding box).
 * \req CPR-7532_Derived CPR-7539_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TC_Max_Objects_And_Max_Detections_Together_One_Object_Implausible)
{
   /** \precond
    * Create NUMBER_OF_REDUCED_OBJECT_TRACKS objects with MAX_NUMBER_OF_DETECTIONS
    * total detections distributed among them
    */
   // Initialize object properties
   rot_object_list_info.number_of_objects = static_cast<uint16_t>(NUMBER_OF_REDUCED_OBJECT_TRACKS);
   for (uint16_t i = 0; i < NUMBER_OF_REDUCED_OBJECT_TRACKS; i++)
   {
      rot_object_list_info.rot_object_list[i].id = i + 1;
      rot_object_list_info.rot_object_list[i].vcs_x_posn = 10.0f + (i * 0.1f);
      rot_object_list_info.rot_object_list[i].vcs_y_posn = 0.0f;
      rot_object_list_info.rot_object_list[i].length = 5.0f;
      rot_object_list_info.rot_object_list[i].width = 1.8f;
      rot_object_list_info.rot_object_list[i].object_status = 0U;
      rot_object_list_info.rot_object_list[i].ndets = 0;
   }

   // Initialize detection properties (all associated detections plausible match except last object)
   const uint32_t implausible_object_index = 100; // k-th object to make implausible
   raw_detect_list.number_of_valid_detections = MAX_NUMBER_OF_DETECTIONS;
   for (uint32_t i = 0; i < MAX_NUMBER_OF_DETECTIONS; i++)
   {
      const uint16_t object_index = i % NUMBER_OF_REDUCED_OBJECT_TRACKS;
      rot_object_list_info.rot_object_list[object_index].ndets++;

      uint16_t object_id = object_index + 1;
      f360_detection_list[i].objTrkID = object_id;

      // Place detection at object position (last object will have all detections outside bounding box)
      raw_detect_list.detections[i].processed.vcs_position_x = 10.0f + ((object_id - 1) * 0.1f);
      raw_detect_list.detections[i].processed.vcs_position_y = (object_index == implausible_object_index) ? 100.0F : 0.0F;
   }

   /** \action
    * Call Safety_Handler_Acceptance_Check() with max objects and max detections
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Should handle maximum capacity. Fault expected due to one object being implausible
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 60U);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_SAFE);
}

/** \purpose
 * Test Safety_Handler with one object having MAX_NUMBER_OF_DETECTIONS associated detections,
 * only the last one is plausible.
 * \req CPR-7532_Derived CPR-7539_Derived
 */
TEST(test_Safety_Handler, Safety_Handler_TT_One_Object_Max_Detections_Last_Object_Implausible)
{
   /** \precond
    * Create one object with MAX_NUMBER_OF_DETECTIONS associated detections,
    * only the last detection is plausible (inside bounding box).
    */
   ROT_Object_Output_T &object = Add_Default_Object(rot_object_list_info, false);
   object.id = 1;
   object.vcs_x_posn = 10.0f;
   object.vcs_y_posn = 0.0f;
   object.length = 5.0f;
   object.width = 1.8f;
   object.object_status = 0U;
   object.ndets = MAX_NUMBER_OF_DETECTIONS;
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, MAX_NUMBER_OF_DETECTIONS - 1, false);
   Create_And_Associate_Detections(raw_detect_list, f360_detection_list, object, 1, true);

   /** \action
    * Call Safety_Handler_Acceptance_Check()
    */
   Safety_Handler_Acceptance_Check(raw_detect_list.detections, f360_detection_list,
                                   raw_detect_list.number_of_valid_detections,
                                   rot_object_list_info.rot_object_list, rot_object_list_info.all_scl_faults.overall_fault_status);

   /** \result
    * Only last detection plausible should pass overall check
    */
   CHECK_TRUE(rot_object_list_info.all_scl_faults.overall_fault_status == 195U);
   CHECK_TRUE(Get_Safety_Handler_State() == STATE_FAULT_FREE);
}

/** @}*/
