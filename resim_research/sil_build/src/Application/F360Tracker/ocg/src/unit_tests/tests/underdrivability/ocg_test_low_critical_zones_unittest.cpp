/** \file
 * This file contains unit tests for content of ocg_test_low_critical_zones.cpp file
 */

#include "ocg_test_low_critical_zones.h"
#include <gtest/gtest.h>

#include "ocg_calibrations.h"

using namespace ocg;
using namespace rspp_variant_A;

/** \defgroup  f360_test_low_critical_zones
 *  @{
 */

/** \brief
 * Test group of Test_Low_Critical_Zones function. Tests verify
 * whether function properly determines new underdrivability status.
 */
class f360_test_low_critical_zones : public ::testing::Test
{
protected:
   OCG_Zones_Probabilities_T probabilities{};
   uint32_t zone_idx;
   uint32_t circ_buff_zone_idx;
   OCG_Calibrations_T calib{};

   void SetUp() override
   {
      Initialize_OCG_Calibrations(calib);
   }
};

/** \purpose
 * Purpose of this test is to verify whether when zone probabilities are not high enough, function
 * returns UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
 * \req
 * NA.
 */
TEST_F(f360_test_low_critical_zones, Test_Low_Critical_Zones__Probabilities_Too_Low)
{
   /** \precond
    * Set zone_idx and circ_buff_zone_idx to same value = 0.
    * Set probabilities indicating that we have an overhead object to 0.
    * Set false probabilities to 1.
    */
   zone_idx = 0U;
   circ_buff_zone_idx = 0U;
   probabilities.is_likely_to_pass_height[zone_idx] = 0.0F;
   probabilities.is_likely_to_pass_RCS[circ_buff_zone_idx] = 0.0F;
   probabilities.can_pass_RCS[circ_buff_zone_idx] = 0.0F;
   probabilities.can_not_pass[zone_idx] = 1.0F;

   /** \action
    * Call tested function.
    */
   OCG_Cell_Classification cell_classification;
   Test_Low_Critical_Zones(probabilities, zone_idx, circ_buff_zone_idx, calib, cell_classification);

   /** \result
    * Check whether new_status is equal to UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER, cell_classification.underdrivability_status);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER]);
   EXPECT_FLOAT_EQ(1.0f, cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
}

/** \purpose
 * Purpose of this test is to verify whether when zone probabilities are high enough, function
 * returns UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER
 * \req
 * NA.
 */
TEST_F(f360_test_low_critical_zones, Test_Low_Critical_Zones__Probabilities_High_Enough)
{
   /** \precond
    * Set zone_idx and circ_buff_zone_idx to same value - 0.
    * Set probabilities indicating that we have an overhead object to 1.
    * Set false probabilities to 0.
    */
   zone_idx = 0U;
   circ_buff_zone_idx = 0U;
   probabilities.is_likely_to_pass_height[zone_idx] = 1.0F;
   probabilities.is_likely_to_pass_RCS[circ_buff_zone_idx] = 1.0F;
   probabilities.can_pass_RCS[circ_buff_zone_idx] = 1.0F;
   probabilities.can_not_pass[zone_idx] = 0.0F;

   /** \action
    * Call tested function.
    */
   OCG_Cell_Classification cell_classification;
   Test_Low_Critical_Zones(probabilities, zone_idx, circ_buff_zone_idx, calib, cell_classification);

   /** \result
    * Check whether new_status is equal to UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER, cell_classification.underdrivability_status);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
   EXPECT_FLOAT_EQ(1.0f, cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER]  + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
}

/** \purpose
 * Purpose of this test is to verify whether when zone probabilities are not high enough, but probabilities
 * indicating tunnel are high enough, function returns UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER
 * \req
 * NA.
 */
TEST_F(f360_test_low_critical_zones, Test_Low_Critical_Zones__Probabilities_Too_Low_Tunnel_Present)
{
   /** \precond
    * Set selected probabilities of all zones to:
    * is_likely_to_pass = 1.0
    * can_not_pass = 0.0
    * Set probabilities of tested zone:
    * Those indicating that we can pass = 0.0
    * Those indicating that we can not pass = 1.0
    */
   for (uint16_t i = 1U; i < (NUM_CELLS_X - 1U); i++)
   {
      probabilities.is_likely_to_pass[i] = 1.0F;
      probabilities.can_not_pass[i] = 0.0F;
   }

   zone_idx = NUM_CELLS_X / 2;
   circ_buff_zone_idx = zone_idx;
   probabilities.is_likely_to_pass_height[zone_idx] = 0.0F;
   probabilities.is_likely_to_pass_RCS[circ_buff_zone_idx] = 0.0F;
   probabilities.can_pass_RCS[circ_buff_zone_idx] = 0.0F;
   probabilities.can_not_pass[zone_idx] = 1.0F;

   /** \action
    * Call tested function.
    */
   OCG_Cell_Classification cell_classification;
   Test_Low_Critical_Zones(probabilities, zone_idx, circ_buff_zone_idx, calib, cell_classification);
   /** \result
    * Check whether new_status is equal to UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER, cell_classification.underdrivability_status);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
   EXPECT_FLOAT_EQ(1.0f, cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
}

/** \purpose
 * Purpose of this test is to verify whether when zone probabilities are not high enough, probabilities
 * indicating tunnel are high enough and false probabilities are high function
 * returns UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
 * \req
 * NA.
 */
TEST_F(f360_test_low_critical_zones, Test_Low_Critical_Zones__Probabilities_Too_Low_Tunnel_Probabilities_High_False_Probabilities_High)
{
   /** \precond
    * Set selected probabilities of all zones to:
    * is_likely_to_pass = 1.0
    * can_not_pass = 1.0
    * Set probabilities of tested zone:
    * ones indicating that we can pass - 0.0
    * ones indicating that we can not pass - 1.0
    */
   for (uint16_t i = 1U; i < (NUM_CELLS_X - 1U); i++)
   {
      probabilities.is_likely_to_pass[i] = 1.0F;
      probabilities.can_not_pass[i] = 1.0F;
   }

   zone_idx = NUM_CELLS_X / 2U;
   circ_buff_zone_idx = zone_idx;
   probabilities.is_likely_to_pass_height[zone_idx] = 0.0F;
   probabilities.is_likely_to_pass_RCS[circ_buff_zone_idx] = 0.0F;
   probabilities.can_pass_RCS[circ_buff_zone_idx] = 0.0F;
   probabilities.can_not_pass[zone_idx] = 1.0F;

   /** \action
    * Call tested function.
    */
   OCG_Cell_Classification cell_classification;
   Test_Low_Critical_Zones(probabilities, zone_idx, circ_buff_zone_idx, calib, cell_classification);
   /** \result
    * Check whether new_status is equal to UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER, cell_classification.underdrivability_status);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER]);
   EXPECT_FLOAT_EQ(1.0f, cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
}
/** @}*/
