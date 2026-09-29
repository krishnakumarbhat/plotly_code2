/** \file
 * This file contains unit tests for content of ocg_shift_circular_zones.cpp file
 */

#include "ocg_shift_circular_zones.h"
#include <gtest/gtest.h>

using namespace ocg;
using namespace rspp_variant_A;

/** \defgroup  f360_shift_circular_zones
 *  @{
 */
static void Init(OCG_Underdrivability_Internal_T &underdrivability)
{
   for (uint16_t i = 0; i < NUM_CELLS_X; i++)
   {
      underdrivability.zones[i].state_height_can_pass[0] = static_cast<float>(i);
      underdrivability.zones[i].state_height_is_likely_to_pass[0] = 2.0F * static_cast<float>(i);
      underdrivability.zones[i].state_RCS_slope_can_not_pass_upper[0] = 3.0F * static_cast<float>(i);

      underdrivability.zones[i].cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
   }
}
/** \brief
 * This test group tests function: Shift_Circular_Zones. Unit tests verify whether
 * all zones are shifted correctly basing on host traveled distance.
 */
class f360_shift_circular_zones : public ::testing::Test
{
protected:
   OCG_Underdrivability_Internal_T underdrivability{};
   RSPP_Host_T host{};
   void SetUp() override
   {
      Init(underdrivability);
      underdrivability.props.timestamp_delta_s = 0.05F;
   }
};

/** \purpose
 * Purpose of this test is to verify whether when host traveled distance is smaller than
 * zone length, circular buffer idx is not incremented.
 * \req
 * NA.
 */
TEST_F(f360_shift_circular_zones, Shift_Circular_Zones__Host_Traveled_Dist_Smaller_Than_Zone_Length)
{
   /** \precond
    * Set up host traveled distance as 0.5 * CELL_LENGTH
    * Set circular buffer idx as specific value
    */
   const uint16_t circ_buff_idx = 3U;
   // host_props.delta_position.x = 0.5F * CELL_LENGTH;
   underdrivability.props.circular_buffer_idx = circ_buff_idx;
   /** \action
    * Call tested function
    */
   Shift_Circular_Zones(underdrivability, host);
   /** \result
    * Check whether circular buffer idx did not change
    */
   EXPECT_FLOAT_EQ(circ_buff_idx, underdrivability.props.circular_buffer_idx);
}
/** @}*/

/** \purpose
 * Purpose of this test is to verify whether when host traveled distance is greater than
 * zone length, circular buffer idx is incremented.
 * \req
 * NA.
 */
TEST_F(f360_shift_circular_zones, Shift_Circular_Zones__Host_Travled_Dist_Greater_Than_Zone_Length)
{
   /** \precond
    * Set up host traveled distance as 1.5 * CELL_LENGTH
    * Set circular buffer idx as specific value
    */
   const uint16_t circ_buff_idx = 3U;
   // host_props.delta_position.x = 1.5F * CELL_LENGTH;
   host.vcs_speed = 40.0F;
   underdrivability.props.host_travel_distance = 0.8F * CELL_LENGTH;
   underdrivability.props.circular_buffer_idx = circ_buff_idx;
   /** \action
    * Call tested function
    */
   Shift_Circular_Zones(underdrivability, host);
   /** \result
    * Check whether circular buffer idx was increment
    */
   EXPECT_FLOAT_EQ((circ_buff_idx + 1U), underdrivability.props.circular_buffer_idx);
}
/** @}*/

/** \purpose
 * Purpose of this test is to verify whether circular buffer idx is reset correctly
 * \req
 * NA.
 */
TEST_F(f360_shift_circular_zones, Shift_Circular_Zones__Circular_Buffer_Idx_Reset)
{
   /** \precond
    * Set circular buffer idx as NUM_CELLS_X - 1
    * Set host traveled dist as 1.5 * CELL_LENGTH
    */
   host.vcs_speed = 40.0F;
   underdrivability.props.host_travel_distance = 0.8F * CELL_LENGTH;
   underdrivability.props.circular_buffer_idx = NUM_CELLS_X - 1U;
   // host_props.delta_position.x = 1.5F * CELL_LENGTH;
   /** \action
    * Call tested function
    */
   Shift_Circular_Zones(underdrivability, host);
   /** \result
    * Check whether circular buffer idx was reset.
    */
   EXPECT_FLOAT_EQ(0U, underdrivability.props.circular_buffer_idx);
}
/** @}*/

/** \purpose
 * Purpose of this test is to verify whether not circular arrays are not shifted
 * if host traveled dist is below limit.
 * \req
 * NA.
 */
TEST_F(f360_shift_circular_zones, Shift_Circular_Zones__Arrays_Not_Shifted_If_Too_Low_Traveled_Dist)
{
   /** \precond
    * Set up host traveled distance as 0.5 * CELL_LENGTH.
    * Set circular buffer idx as 0.
    * Save input struct in temporary variable.
    */
   // host_props.delta_position.x = 0.0F;
   host.vcs_speed = 2.0F;
   underdrivability.props.circular_buffer_idx = 0U;
   const OCG_Underdrivability_Internal_T expected_underdrivability = underdrivability;
   /** \action
    * Call tested function
    */
   Shift_Circular_Zones(underdrivability, host);
   /** \result
    * Check whether input and output struct did not change.
    */
   for (uint16_t i = 0U; i < NUM_CELLS_X; i++)
   {
      EXPECT_EQ(expected_underdrivability.zones[i].cell_classification.underdrivability_status, underdrivability.zones[i].cell_classification.underdrivability_status);
   }
}
/** @}*/

/** \purpose
 * Purpose of this test is to verify whether non circular Arrays are shifted correctly.
 * \req
 * NA.
 */
TEST_F(f360_shift_circular_zones, Shift_Circular_Zones__Non_Circular_Arrays_Shifted_Correctly)
{
   /** \precond
    * Set up host traveled distance as 1.5 * CELL_LENGTH
    * Set circular buffer idx as 0
    */
   // host_props.delta_position.x = 1.5F * CELL_LENGTH;
   host.vcs_speed = 40.0F;
   underdrivability.props.host_travel_distance = 0.8F * CELL_LENGTH;
   underdrivability.props.circular_buffer_idx = 0U;
   const uint32_t first_idx_to_verify = 8U;
   const uint32_t second_idx_to_verify = 2U;
   underdrivability.zones[first_idx_to_verify].cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
   underdrivability.zones[second_idx_to_verify].cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
   /** \action
    * Call tested function
    */
   Shift_Circular_Zones(underdrivability, host);
   /** \result
    * Check whether circular arrays were shifted.
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_CAN_PASS_UNDER, underdrivability.zones[first_idx_to_verify - 1U].cell_classification.underdrivability_status);
   EXPECT_EQ(UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER, underdrivability.zones[second_idx_to_verify - 1U].cell_classification.underdrivability_status);
}
/** @}*/

/** \purpose
 * Purpose of this test is to verify whether when host traveled dist is equal to CELL_LENGTH
 * non circular arrays are not shifted.
 * \req
 * NA.
 */
TEST_F(f360_shift_circular_zones, Shift_Circular_Zones__Traveled_Dist_Equal_To_Zone_Length_Non_Circular_Arrays_Not_Shifted)
{
   /** \precond
    * Set up host traveled distance as CELL_LENGTH
    * Set circular buffer idx as 0
    */
   // host_props.delta_position.x = CELL_LENGTH;
   underdrivability.props.circular_buffer_idx = 0U;
   const uint32_t first_idx_to_verify = 8U;
   const uint32_t second_idx_to_verify = 2U;
   underdrivability.zones[first_idx_to_verify].cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
   underdrivability.zones[second_idx_to_verify].cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
   /** \action
    * Call tested function
    */
   Shift_Circular_Zones(underdrivability, host);
   /** \result
    * Check whether circular arrays were shifted.
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_CAN_PASS_UNDER, underdrivability.zones[first_idx_to_verify].cell_classification.underdrivability_status);
   EXPECT_EQ(UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER, underdrivability.zones[second_idx_to_verify].cell_classification.underdrivability_status);
}

TEST_F(f360_shift_circular_zones, Shift_Circular_Zones__Host_Travled_Dist_Greater_Than_2_Zones_Length)
{
    /** \precond
     * Set circular buffer idx as specific value
     */
    const uint16_t circ_buff_idx = 3U;
    host.vcs_speed = 70.0F; // high host speed
    underdrivability.props.host_travel_distance = 0.9F * CELL_LENGTH;
    underdrivability.props.circular_buffer_idx = circ_buff_idx;
    underdrivability.zones[0].cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
    underdrivability.zones[1].cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER;
    underdrivability.zones[2].cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
    underdrivability.zones[3].cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
    underdrivability.zones[4].cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
    underdrivability.zones[5].cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
    underdrivability.zones[6].cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
    underdrivability.zones[7].cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
    underdrivability.zones[8].cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;
    underdrivability.zones[9].cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER;

    /** \action
     * Call tested function
     */
    Shift_Circular_Zones(underdrivability, host);
    /** \result
     * Check whether circular buffer idx was incremented 2 times and underdrivable status was shifted by 2 cells
     */
    EXPECT_FLOAT_EQ((circ_buff_idx + 2U), underdrivability.props.circular_buffer_idx);
    EXPECT_EQ(underdrivability.zones[0].cell_classification.underdrivability_status, UNDERDRIVABLE_STATUS_CAN_PASS_UNDER);
    EXPECT_EQ(underdrivability.zones[1].cell_classification.underdrivability_status, UNDERDRIVABLE_STATUS_CAN_PASS_UNDER);
    EXPECT_EQ(underdrivability.zones[2].cell_classification.underdrivability_status, UNDERDRIVABLE_STATUS_CAN_PASS_UNDER);
    EXPECT_EQ(underdrivability.zones[3].cell_classification.underdrivability_status, UNDERDRIVABLE_STATUS_CAN_PASS_UNDER);
    EXPECT_EQ(underdrivability.zones[4].cell_classification.underdrivability_status, UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER);
    EXPECT_EQ(underdrivability.zones[5].cell_classification.underdrivability_status, UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER);
    EXPECT_EQ(underdrivability.zones[6].cell_classification.underdrivability_status, UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER);
    EXPECT_EQ(underdrivability.zones[7].cell_classification.underdrivability_status, UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER);
    EXPECT_EQ(underdrivability.zones[8].cell_classification.underdrivability_status, UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER);
    EXPECT_EQ(underdrivability.zones[9].cell_classification.underdrivability_status, UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER);
}
