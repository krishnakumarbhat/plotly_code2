/** \file
 * This file contains unit tests for content of ocg_test_highly_critical_zones.cpp file
 */

#include "ocg_test_highly_critical_zones.h"
#include <gtest/gtest.h>

#include "ocg_calibrations.h"

using namespace ocg;
using namespace rspp_variant_A;

/** \defgroup  f360_test_highly_critical_zones
 *  @{
 */

/** \brief
 * Test group of Test_Highly_Critical_Zones function. Tests verify
 * whether function properly determines new underdrivability status.
 */
class f360_test_highly_critical_zones : public ::testing::Test
{
protected:
   OCG_Zones_Probabilities_T probabilities{};
   uint32_t zone_idx;
   uint32_t circ_buff_zone_idx;
   OCG_Calibrations_T calib{};
   OCG_Underdrivable_Status_T current_underdrivability_status;
   float n_meas;

   void SetUp() override
   {
      Initialize_OCG_Calibrations(calib);
   }

   void Set_Can_Pass_Flag_To_False(OCG_Zones_Probabilities_T &prob, uint32_t zone_index, uint32_t circ_buff_zone_index)
   {
      prob.can_pass_height[circ_buff_zone_index] = 0.0F;
      prob.can_pass_RCS[circ_buff_zone_index] = 0.0F;
      prob.can_not_pass[zone_index] = 1.0F;
   }

   void Set_Can_Pass_Flag_To_True(OCG_Zones_Probabilities_T &prob, uint32_t zone_index, uint32_t circ_buff_zone_index)
   {
      prob.can_pass_height[circ_buff_zone_index] = 1.0F;
      prob.can_pass_RCS[circ_buff_zone_index] = 1.0F;
      prob.can_not_pass[zone_index] = 0.0F;
   }

   void Set_Is_Likely_To_Pass_Flag_To_True(OCG_Zones_Probabilities_T &prob, uint32_t zone_index, uint32_t circ_buff_zone_index)
   {
      prob.is_likely_to_pass_height[circ_buff_zone_index] = 1.0F;
      prob.is_likely_to_pass_RCS[circ_buff_zone_index] = 1.0F;
      prob.can_not_pass[zone_index] = 0.0F;
   }

   void Set_Is_Likely_To_Pass_Flag_To_False(OCG_Zones_Probabilities_T &prob, uint32_t zone_index, uint32_t circ_buff_zone_index)
   {
      prob.is_likely_to_pass_height[circ_buff_zone_index] = 0.0F;
      prob.is_likely_to_pass_RCS[circ_buff_zone_index] = 0.0F;
      prob.can_not_pass[zone_index] = 1.0F;
   }

   void Set_Tunnel_Detected_Flag_To_True(OCG_Zones_Probabilities_T &prob)
   {
      for (uint32_t i = 0U; i < NUM_CELLS_X; i++)
      {
         prob.is_likely_to_pass[i] = 1.0F;
         prob.can_not_pass[i] = 0.0F;
      }
   }

   void Set_Tunnel_Detected_Flag_To_False(OCG_Zones_Probabilities_T &prob)
   {
      for (uint32_t i = 0U; i < NUM_CELLS_X; i++)
      {
         prob.is_likely_to_pass[i] = 0.0F;
         prob.can_not_pass[i] = 1.0F;
      }
   }
};

/** \purpose
 * Purpose of this test is to verify whether when all flags are false, function returns
 * UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
 * \req
 * NA.
 */
TEST_F(f360_test_highly_critical_zones, Test_Highly_Critical_Zones__All_Flags_Are_False)
{
   /** \precond
    * Set Tunnel flag to false.
    * Set Can_Pass flag to false.
    * Set Is_Likely_To_Pass flag to false.
    * Set current_underdrivability_status to UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
    * Set n_meas to 0
    */
   zone_idx = 0U;
   circ_buff_zone_idx = zone_idx;
   Set_Tunnel_Detected_Flag_To_False(probabilities);
   Set_Can_Pass_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   Set_Is_Likely_To_Pass_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);

   OCG_Cell_Classification cell_classification;
   cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
   n_meas = 0;

   /** \action
    * Call tested function.
    */
   Test_Highly_Critical_Zones(probabilities, n_meas, zone_idx, circ_buff_zone_idx, calib, cell_classification);

   /** \result
    * Check wheter function returned UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER.
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER, cell_classification.underdrivability_status);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER]);
   EXPECT_FLOAT_EQ(1.0f, cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
}

/** \purpose
 * Purpose of this test is to verify whether when Can_Pass flag is true, function returns
 * UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
 * \req
 * NA.
 */
TEST_F(f360_test_highly_critical_zones, Test_Highly_Critical_Zones__Can_Pass_Test_Flag_True)
{
   /** \precond
    * Set Tunnel flag to false.
    * Set Is_Likely_To_Pass flag to false.
    * Set Can_Pass flag to true.
    * Set current_underdrivability_status to UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
    * Set n_meas to 0
    */
   zone_idx = 0U;
   circ_buff_zone_idx = zone_idx;
   Set_Tunnel_Detected_Flag_To_False(probabilities);
   Set_Is_Likely_To_Pass_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   Set_Can_Pass_Flag_To_True(probabilities, zone_idx, circ_buff_zone_idx);

   OCG_Cell_Classification cell_classification;
   cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
   n_meas = 0.0F;

   /** \action
    * Call tested function.
    */
   Test_Highly_Critical_Zones(probabilities, n_meas, zone_idx, circ_buff_zone_idx, calib, cell_classification);

   /** \result
    * Check wheter function returned UNDERDRIVABLE_STATUS_CAN_PASS_UNDER.
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_CAN_PASS_UNDER, cell_classification.underdrivability_status);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
   EXPECT_FLOAT_EQ(1.0f, cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
}

/** \purpose
 * Purpose of this test is to verify whether when is likely to pass flag is true
 * but number of measurements is too low, function returns UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
 * \req
 * NA.
 */
TEST_F(f360_test_highly_critical_zones, Test_Highly_Critical_Zones__Is_Likely_To_Pass_Flag_True_Not_Enough_Measurements)
{
   /** \precond
    * Set Tunnel flag to false.
    * Set Can_Pass flag to false.
    * Set Is_Likely_To_Pass flag to true.
    * Set current_underdrivability_status to UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
    * Set n_meas to 0
    */
   zone_idx = 0U;
   circ_buff_zone_idx = zone_idx;
   Set_Tunnel_Detected_Flag_To_False(probabilities);
   Set_Can_Pass_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   Set_Is_Likely_To_Pass_Flag_To_True(probabilities, zone_idx, circ_buff_zone_idx);

   OCG_Cell_Classification cell_classification;
   cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
   n_meas = 0.0F;

   /** \action
    * Call tested function.
    */
   Test_Highly_Critical_Zones(probabilities, n_meas, zone_idx, circ_buff_zone_idx, calib, cell_classification);

   /** \result
    * Check wheter function returned UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER.
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER, cell_classification.underdrivability_status);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER]);
   EXPECT_FLOAT_EQ(1.0f, cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
}

/** \purpose
 * Purpose of this test is to verify whether when is_likely_to_pass flag is true
 * and number of meas is high enough, function returns UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
 * \req
 * NA.
 */
TEST_F(f360_test_highly_critical_zones, Test_Highly_Critical_Zones__Is_Likely_To_Pass_Flag_True_Enough_Measurements)
{
   /** \precond
    * Set Tunnel flag to false.
    * Set Can_Pass flag to false.
    * Set Is_Likely_To_Pass flag to true.
    * Set current_underdrivability_status to UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
    * Set n_meas to be above limit.
    */
   zone_idx = 0U;
   circ_buff_zone_idx = zone_idx;
   Set_Tunnel_Detected_Flag_To_False(probabilities);
   Set_Can_Pass_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   Set_Is_Likely_To_Pass_Flag_To_True(probabilities, zone_idx, circ_buff_zone_idx);

   OCG_Cell_Classification cell_classification;
   cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
   n_meas = calib.underdrive_high_crit_zone_is_likely_to_pass_min_number_of_dets + 5.0F;

   /** \action
    * Call tested function.
    */
   Test_Highly_Critical_Zones(probabilities, n_meas, zone_idx, circ_buff_zone_idx, calib, cell_classification);

   /** \result
    * Check wheter function returned UNDERDRIVABLE_STATUS_CAN_PASS_UNDER.
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_CAN_PASS_UNDER, cell_classification.underdrivability_status);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
   EXPECT_FLOAT_EQ(1.0f, cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
}

/** \purpose
 * Purpose of this test is to verify whether when tunnel flag is true
 * but previous status was different than CAN_PASS_UNDER
 * function returns UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
 * \req
 * NA.
 */
TEST_F(f360_test_highly_critical_zones, Test_Highly_Critical_Zones__Tunnel_Flag_True_Prev_Status_Can_Not_Pass)
{
   /** \precond
    * Set Tunnel flag to true.
    * Set Can_Pass flag to false.
    * Set Is_Likely_To_Pass flag to false.
    * Set current_underdrivability_status to UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
    * Set n_meas 0
    */
   zone_idx = NUM_CELLS_X / 2U;
   circ_buff_zone_idx = zone_idx;
   Set_Tunnel_Detected_Flag_To_True(probabilities);
   Set_Can_Pass_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   Set_Is_Likely_To_Pass_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);

   OCG_Cell_Classification cell_classification;
   cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
   n_meas = 0.0F;

   /** \action
    * Call tested function.
    */
   Test_Highly_Critical_Zones(probabilities, n_meas, zone_idx, circ_buff_zone_idx, calib, cell_classification);

   /** \result
    * Check wheter function returned UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER.
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER, cell_classification.underdrivability_status);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER]);
   EXPECT_FLOAT_EQ(1.0f, cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
}

/** \purpose
 * Purpose of this test is to verify whether when tunnel flag is true
 * and previous status was UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
 * function returns UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
 * \req
 * NA.
 */
TEST_F(f360_test_highly_critical_zones, Test_Highly_Critical_Zones__Tunnel_Flag_True_Prev_Status_Can_Pass)
{
   /** \precond
    * Set Tunnel flag to true.
    * Set Can_Pass flag to false.
    * Set Is_Likely_To_Pass flag to false.
    * Set current_underdrivability_status to UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
    * Set n_meas to 0.
    */
   zone_idx = NUM_CELLS_X / 2U;
   circ_buff_zone_idx = zone_idx;
   Set_Tunnel_Detected_Flag_To_True(probabilities);
   Set_Can_Pass_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   Set_Is_Likely_To_Pass_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   probabilities.can_not_pass[zone_idx] = 0.0F;

   OCG_Cell_Classification cell_classification;
   cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_PASS_UNDER;
   n_meas = 0.0F;

   /** \action
    * Call tested function.
    */
   Test_Highly_Critical_Zones(probabilities, n_meas, zone_idx, circ_buff_zone_idx, calib, cell_classification);

   /** \result
    * Check wheter function returned UNDERDRIVABLE_STATUS_CAN_PASS_UNDER.
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_CAN_PASS_UNDER, cell_classification.underdrivability_status);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
   EXPECT_FLOAT_EQ(1.0f, cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
}

/** \purpose
 * Purpose of this test is to verify whether when there is enough measurements but
 * is_likely_to_pass flag is false, function returns UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
 * \req
 * NA.
 */
TEST_F(f360_test_highly_critical_zones, Test_Highly_Critical_Zones__Is_Likely_To_Pass_Flag_False_Enough_Measurements)
{
   /** \precond
    * Set Tunnel flag to false.
    * Set Can_Pass flag to false.
    * Set Is_Likely_To_Pass flag to false.
    * Set current_underdrivability_status to UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
    * Set n_meas to be above limit.
    */
   zone_idx = 0U;
   circ_buff_zone_idx = zone_idx;
   Set_Tunnel_Detected_Flag_To_False(probabilities);
   Set_Can_Pass_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   Set_Is_Likely_To_Pass_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);

   OCG_Cell_Classification cell_classification;
   cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
   n_meas = calib.underdrive_high_crit_zone_is_likely_to_pass_min_number_of_dets + 5.0F;

   /** \action
    * Call tested function.
    */
   Test_Highly_Critical_Zones(probabilities, n_meas, zone_idx, circ_buff_zone_idx, calib, cell_classification);

   /** \result
    * Check wheter function returned UNDERDRIVABLE_STATUS_CAN_PASS_UNDER.
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER, cell_classification.underdrivability_status);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER]);
   EXPECT_FLOAT_EQ(1.0f, cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
}

/** \purpose
 * Purpose of this test is to verify whether when current underdrivability status is
 * UNDERDRIVABLE_STATUS_CAN_PASS_UNDER but tunnel probabilities are too low function returns
 * UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
 * \req
 * NA.
 */
TEST_F(f360_test_highly_critical_zones, Test_Highly_Critical_Zones__To_Low_Tunnel_Prob_Previous_Status_Can_Pass)
{
   /** \precond
    * Set Tunnel flag to false.
    * Set Can_Pass flag to false.
    * Set Is_Likely_To_Pass flag to false.
    * Set current_underdrivability_status to UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
    * Set n_meas to be 0
    */
   zone_idx = 0U;
   circ_buff_zone_idx = zone_idx;
   Set_Tunnel_Detected_Flag_To_False(probabilities);
   Set_Can_Pass_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   Set_Is_Likely_To_Pass_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);

   OCG_Cell_Classification cell_classification;
   cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
   n_meas = 0.0F;

   /** \action
    * Call tested function.
    */
   Test_Highly_Critical_Zones(probabilities, n_meas, zone_idx, circ_buff_zone_idx, calib, cell_classification);

   /** \result
    * Check wheter function returned UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER.
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER, cell_classification.underdrivability_status);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER]);
   EXPECT_FLOAT_EQ(1.0f, cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
}

/** \purpose
 * Purpose of this test is to verify whether when current underdrivability status is
 * UNDERDRIVABLE_STATUS_CAN_PASS_UNDER, tunnel positive probabilities are high enough but
 * false probabilities are too high - function returns UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
 * \req
 * NA.
 */
TEST_F(f360_test_highly_critical_zones, Test_Highly_Critical_Zones__To_High_Tunnel_False_Prob_Previous_Status_Can_Pass)
{
   /** \precond
    * Set Tunnel flag to false.
    * Set Can_Pass flag to false.
    * Set Is_Likely_To_Pass flag to false.
    * Set false probabilities to 1.0
    * Set current_underdrivability_status to UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
    * Set n_meas to 0
    */
   zone_idx = 10U;
   circ_buff_zone_idx = zone_idx;
   Set_Tunnel_Detected_Flag_To_True(probabilities);
   Set_Can_Pass_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   Set_Is_Likely_To_Pass_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);

   for (uint32_t i = 0; i < NUM_CELLS_X; i++)
   {
      probabilities.can_not_pass[i] = 1.0F;
   }

   OCG_Cell_Classification cell_classification;
   cell_classification.underdrivability_status = UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER;
   n_meas = 0.0F;

   /** \action
    * Call tested function.
    */
   Test_Highly_Critical_Zones(probabilities, n_meas, zone_idx, circ_buff_zone_idx, calib, cell_classification);

   /** \result
    * Check wheter function returned UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER.
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER, cell_classification.underdrivability_status);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER]);
   EXPECT_FLOAT_EQ(1.0f, cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
}
/** @}*/
