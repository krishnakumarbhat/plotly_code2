/** \file
 * This file contains unit tests for content of ocg_initialize_underdrivability.cpp file
 */

#include "ocg_initialize_underdrivability.h"
#include <gtest/gtest.h>

using namespace ocg;
using namespace rspp_variant_A;

/** \defgroup  f360_initialize_underdrivability
 *  @{
 */

/** \brief
 * Test group of f360_initialize_underdrivability.
 * Tests verify whether underdrivability is properly initialized with zeros.
 */
class f360_initialize_underdrivability : public ::testing::Test
{
};

/** \purpose
 * Purpose of this test is to verify whether underdrivability structure is properly initialized with zeros.
 * \req
 * NA.
 */
TEST_F(f360_initialize_underdrivability, FunctionToTest_Descriptive_Tag)
{
   /** \precond
    * Declare underdrivability struct.
    */
   OCG_Underdrivability_Internal_T underdrivability;
   RSPP_Host_T host{};

   /** \action
    * Call tested function.
    */
   Initialize_Underdrivability(host, underdrivability);

   /** \result
    * Verify whether all fields are zeros.
    */
   bool f_success = true;
   f_success &= (0U == underdrivability.props.circular_buffer_idx);
   f_success &= (0.0F == underdrivability.props.host_travel_distance);
   for (uint32_t i = 0; i < NUM_CELLS_X; i++)
   {
      for (uint32_t state_idx = 0; state_idx < UD_HEIGHT_STATE_SIZE; state_idx++)
      {
         f_success &= (0.0F == underdrivability.zones[i].state_height_can_pass[state_idx]);
         f_success &= (0.0F == underdrivability.zones[i].state_height_is_likely_to_pass[state_idx]);
         f_success &= (0.0F == underdrivability.zones[i].state_height_can_not_pass_upper[state_idx]);
         f_success &= (0.0F == underdrivability.zones[i].state_height_can_not_pass_lower[state_idx]);
      }

      for (uint32_t state_idx = 0; state_idx < UD_RCS_STATE_SIZE; state_idx++)
      {
         f_success &= (0.0F == underdrivability.zones[i].state_RCS_slope_can_pass[state_idx]);
         f_success &= (0.0F == underdrivability.zones[i].state_RCS_slope_is_likely_to_pass[state_idx]);
         f_success &= (0.0F == underdrivability.zones[i].state_RCS_slope_can_not_pass_upper[state_idx]);
         f_success &= (0.0F == underdrivability.zones[i].state_RCS_slope_can_not_pass_lower[state_idx]);
      }

      f_success &= (0.0F == underdrivability.zones[i].p_height_can_pass);
      f_success &= (0.0F == underdrivability.zones[i].p_height_is_likely_to_pass);
      f_success &= (0.0F == underdrivability.zones[i].p_height_can_not_pass_upper);
      f_success &= (0.0F == underdrivability.zones[i].p_height_can_not_pass_lower);
      f_success &= (0.0F == underdrivability.zones[i].p_RCS_slope_can_pass);
      f_success &= (0.0F == underdrivability.zones[i].p_RCS_slope_is_likely_to_pass);
      f_success &= (0.0F == underdrivability.zones[i].p_RCS_slope_can_not_pass_upper);
      f_success &= (0.0F == underdrivability.zones[i].p_RCS_slope_can_not_pass_lower);
      f_success &= (0.0F == underdrivability.zones[i].p_can_pass);
      f_success &= (0.0F == underdrivability.zones[i].p_is_likely_to_pass);
      f_success &= (0.0F == underdrivability.zones[i].p_can_not_pass);

      f_success &= (UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER == underdrivability.zones[i].cell_classification.underdrivability_status);
   }
   EXPECT_TRUE(f_success);
}
/** @}*/
