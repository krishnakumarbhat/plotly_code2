/** \file
 * This file contains unit tests for content of ocg_test_med_critical_zones.cpp file
 */

#include "ocg_test_med_critical_zones.h"
#include <gtest/gtest.h>

#include "ocg_calibrations.h"

using namespace ocg;
using namespace rspp_variant_A;

/** \defgroup  f360_test_med_critical_zones
 *  @{
 */

/** \brief
 * Test group of Test_Med_Critical_Zones function. Tests verify
 * whether function properly determines new underdrivability status.
 */
class f360_test_med_critical_zones : public ::testing::Test
{
protected:
   OCG_Zones_Probabilities_T probabilities{};
   uint32_t zone_idx;
   uint32_t circ_buff_zone_idx;
   OCG_Calibrations_T calib{};
   float n_meas;

   void SetUp() override
   {
      Initialize_OCG_Calibrations(calib);
   }

   void Set_Can_Pass_Strict_Flag_To_False(OCG_Zones_Probabilities_T &prob,
                                          const uint32_t zone_index,
                                          const uint32_t circ_buff_zone_index)
   {
      prob.can_pass_height[circ_buff_zone_index] = 0.0F;
      prob.can_pass_RCS[circ_buff_zone_index] = 0.0F;
      prob.can_not_pass[zone_index] = 1.0F;
   }

   void Set_Can_Pass_Strict_Flag_To_True(OCG_Zones_Probabilities_T & prob,
                                         const uint32_t zone_index,
                                         const uint32_t circ_buff_zone_index)
   {
      prob.can_pass_height[circ_buff_zone_index] = 1.0F;
      prob.can_pass_RCS[circ_buff_zone_index] = 1.0F;
      prob.can_not_pass[zone_index] = 0.0F;
   }

   void Set_Is_Likely_To_Pass_Strict_Flag_To_False(OCG_Zones_Probabilities_T & prob,
                                                   const uint32_t zone_index,
                                                   const uint32_t circ_buff_zone_index)
   {
      prob.is_likely_to_pass_height[circ_buff_zone_index] = 0.0F;
      prob.is_likely_to_pass_RCS[circ_buff_zone_index] = 0.0F;
      prob.can_not_pass[zone_index] = 1.0F;
   }

   void Set_Is_Likely_To_Pass_Strict_Flag_To_True(OCG_Zones_Probabilities_T &prob,
                                                  const uint32_t zone_index,
                                                  const uint32_t circ_buff_zone_index)
   {
      prob.is_likely_to_pass_height[circ_buff_zone_index] = 1.0F;
      prob.is_likely_to_pass_RCS[circ_buff_zone_index] = 1.0F;
      prob.can_not_pass[zone_index] = 0.0F;
   }

   void Set_Is_Likely_To_Pass_Weak_Flag_To_True(OCG_Zones_Probabilities_T &prob,
                                                const uint32_t zone_index,
                                                const uint32_t circ_buff_zone_index)
   {
      prob.is_likely_to_pass_height[circ_buff_zone_index] = 0.8F;
      prob.is_likely_to_pass_RCS[circ_buff_zone_index] = 0.6F;
      prob.can_pass_RCS[circ_buff_zone_index] = 0.6F;
      prob.can_not_pass[zone_index] = 0.0F;
   }

   void Set_Tunnel_Detected_Strict_Flag_To_False(OCG_Zones_Probabilities_T &prob)
   {
      for (int i = 0; i < NUM_CELLS_X; i++)
      {
         prob.is_likely_to_pass[i] = 0.0F;
         prob.can_not_pass[i] = 1.0F;
      }
   }

   void Set_Tunnel_Detected_Strict_Flag_To_True(OCG_Zones_Probabilities_T &prob)
   {
      for (int i = 0; i < NUM_CELLS_X; i++)
      {
         prob.is_likely_to_pass[i] = 1.0F;
         prob.can_not_pass[i] = 0.0F;
      }
   }

   void Set_Tunnel_Detected_Weak_Flag_To_True(OCG_Zones_Probabilities_T &prob)
   {
      for (int i = 0; i < NUM_CELLS_X; i++)
      {
         prob.is_likely_to_pass[i] = calib.underdrive_tunnel_weak_min_is_likely_to_pass_mean_val + 0.1F;
         prob.can_not_pass[i] = calib.underdrive_tunnel_weak_max_can_not_pass_mean_val / 2.0F;
      }
   }
};

/** \purpose
 * Purpose of this test is to verify whether when all flags are false, function returns
 * UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
 */
TEST_F(f360_test_med_critical_zones, Test_Med_Critical_Zones__All_Flags_False)
{
   /** \precond
    * Set all flags to false.
    */
   zone_idx = 0U;
   circ_buff_zone_idx = zone_idx;
   n_meas = 0.0F;

   Set_Can_Pass_Strict_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   Set_Is_Likely_To_Pass_Strict_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   Set_Tunnel_Detected_Strict_Flag_To_False(probabilities);

   /** \action
    * Call tested function
    */
   OCG_Cell_Classification cell_classification;
   Test_Med_Critical_Zones(probabilities, n_meas, zone_idx, circ_buff_zone_idx, calib, cell_classification);

   /** \result
    * Check whether returned status is equal to: UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER, cell_classification.underdrivability_status);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER]);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER]);
   EXPECT_FLOAT_EQ(1.0f, cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
}

/** \purpose
 * Purpose of this test is to verify whether when can_pass_strict flag is true
 * function returns UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
 */
TEST_F(f360_test_med_critical_zones, Test_Med_Critical_Zones__Can_Pass_Strict_Flag_True)
{
   /** \precond
    * Set tunnel strict flag to false
    * Set is likely to pass strict flag to false
    * Set can pass strict flag to true
    */
   zone_idx = 0U;
   circ_buff_zone_idx = zone_idx;
   n_meas = 0.0F;

   Set_Tunnel_Detected_Strict_Flag_To_False(probabilities);
   Set_Is_Likely_To_Pass_Strict_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   Set_Can_Pass_Strict_Flag_To_True(probabilities, zone_idx, circ_buff_zone_idx);
   /** \action
    * Call tested function
    */
   OCG_Cell_Classification cell_classification;
   Test_Med_Critical_Zones(probabilities, n_meas, zone_idx, circ_buff_zone_idx, calib, cell_classification);

   /** \result
    * Check whether returned status is equal to: UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_CAN_PASS_UNDER, cell_classification.underdrivability_status);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER]);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
   EXPECT_FLOAT_EQ(1.0f, cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
}

/** \purpose
 * Purpose of this test is to verify whether when is_likely_to_pass strict flag is true,
 * function returns UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
 */
TEST_F(f360_test_med_critical_zones, Test_Med_Critical_Zones__Is_Likely_To_Pass_Strict_Flag_True)
{
   /** \precond
    * Set all flags except is_likely_to_pass_strict to false
    * Set is_likely_to_pass_strict flag to true
    * Set number of meas above threshold
    */
   zone_idx = 0U;
   circ_buff_zone_idx = zone_idx;

   Set_Can_Pass_Strict_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   Set_Tunnel_Detected_Strict_Flag_To_False(probabilities);
   Set_Is_Likely_To_Pass_Strict_Flag_To_True(probabilities, zone_idx, circ_buff_zone_idx);
   n_meas = 30.0F;

   /** \action
    * Call tested function
    */
   OCG_Cell_Classification cell_classification;
   Test_Med_Critical_Zones(probabilities, n_meas, zone_idx, circ_buff_zone_idx, calib, cell_classification);

   /** \result
    * Check whether returned status is equal to: UNDERDRIVABLE_STATUS_CAN_PASS_PASS_UNDER
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_CAN_PASS_UNDER, cell_classification.underdrivability_status);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER]);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
   EXPECT_FLOAT_EQ(1.0f, cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
}

/** \purpose
 * Purpose of this test is to verify whether when is_likely_to_pass strict flag is false because of too
 * low number of detections, is_likely_to_pass weak flag is true and
 * function returns UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER
 */
TEST_F(f360_test_med_critical_zones, Test_Med_Critical_Zones__Is_Likely_To_Pass_Strict_Not_Enough_Detections_Probability_Test_Passes)
{
   /** \precond
    * Set all flags expect is_likely_to_pass_strict to false
    * Set is_likely_to_pass_strict flag to true
    * Set number of meas below threshold
    */
   zone_idx = 0U;
   circ_buff_zone_idx = zone_idx;
   n_meas = 0.0F;

   Set_Can_Pass_Strict_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   Set_Tunnel_Detected_Strict_Flag_To_False(probabilities);
   Set_Is_Likely_To_Pass_Strict_Flag_To_True(probabilities, zone_idx, circ_buff_zone_idx);

   /** \action
    * Call tested function
    */
   OCG_Cell_Classification cell_classification;
   Test_Med_Critical_Zones(probabilities, n_meas, zone_idx, circ_buff_zone_idx, calib, cell_classification);

   /** \result
    * Check whether returned status is equal to: UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER, cell_classification.underdrivability_status);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER]);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
   EXPECT_FLOAT_EQ(1.0f, cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
}

/** \purpose
 * Purpose of this test is to verify whether when there is enough detections but is_likely_to_pass probability
 * test fails function returns UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
 */
TEST_F(f360_test_med_critical_zones, Test_Med_Critical_Zones__Is_Likely_To_Pass_Strict_Enough_Detections_But_Probability_Test_Fails)
{
   /** \precond
    * Set all flags to false
    * Set number of meas above threshold
    */
   zone_idx = 0U;
   circ_buff_zone_idx = zone_idx;
   n_meas = 30.0F;

   Set_Can_Pass_Strict_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   Set_Tunnel_Detected_Strict_Flag_To_False(probabilities);
   Set_Is_Likely_To_Pass_Strict_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);

   /** \action
    * Call tested function
    */
   OCG_Cell_Classification cell_classification;
   Test_Med_Critical_Zones(probabilities, n_meas, zone_idx, circ_buff_zone_idx, calib, cell_classification);

   /** \result
    * Check whether returned status is equal to: UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER, cell_classification.underdrivability_status);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER]);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER]);
   EXPECT_FLOAT_EQ(1.0f, cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
}

/** \purpose
 * Purpose of this test is to verify whether when tunnel strict flag is true
 * function returns UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
 */
TEST_F(f360_test_med_critical_zones, Test_Med_Critical_Zones__Tunel_Strict_Flag_True)
{
   /** \precond
    * Set tunnel strict flag to true
    * Set can pass strict flag to false
    * set is likely to pass strict flag to false
    * reset can not pass probability of tested zone
    */
   zone_idx = 0U;
   circ_buff_zone_idx = zone_idx;
   n_meas = 0.0F;

   Set_Tunnel_Detected_Strict_Flag_To_True(probabilities);
   Set_Can_Pass_Strict_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   Set_Is_Likely_To_Pass_Strict_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   probabilities.can_not_pass[zone_idx] = 0.0F;

   /** \action
    * Call tested function
    */
   OCG_Cell_Classification cell_classification;
   Test_Med_Critical_Zones(probabilities, n_meas, zone_idx, circ_buff_zone_idx, calib, cell_classification);

   /** \result
    * Check whether returned status is equal to: UNDERDRIVABLE_STATUS_CAN_PASS_UNDER
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_CAN_PASS_UNDER, cell_classification.underdrivability_status);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER]);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
   EXPECT_FLOAT_EQ(1.0f, cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
}

/** \purpose
 * Purpose of this test is to verify whether when tunnel strict flag is false
 * when can_not_pass probability is too high and function returns
 * UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
 */
TEST_F(f360_test_med_critical_zones, Test_Med_Critical_Zones__Tunel_Strict_Flag_False_To_High_False_Prob)
{
   /** \precond
    * Set zone_idx to be higher than 0
    * Set tunnel strict flag to true
    * Set can pass strict flag to false
    * Set is likely to pass strict flag to false
    * Set can not pass probability in all zones to 1.0
    */
   zone_idx = 10U;
   circ_buff_zone_idx = zone_idx;
   n_meas = 0.0F;

   Set_Tunnel_Detected_Strict_Flag_To_True(probabilities);
   Set_Can_Pass_Strict_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   Set_Is_Likely_To_Pass_Strict_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   for (uint32_t i = 0; i < NUM_CELLS_X; i++)
   {
      probabilities.can_not_pass[i] = 1.0F;
   }

   /** \action
    * Call tested function
    */
   OCG_Cell_Classification cell_classification;
   Test_Med_Critical_Zones(probabilities, n_meas, zone_idx, circ_buff_zone_idx, calib, cell_classification);

   /** \result
    * Check whether returned status is equal to: UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER, cell_classification.underdrivability_status);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER]);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER]);
   EXPECT_FLOAT_EQ(1.0f, cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
}

/** \purpose
 * Purpose of this test is to verify whether when is_likely_to_pass flag is true,
 * function returns UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER
 */
TEST_F(f360_test_med_critical_zones, Test_Med_Critical_Zones__Is_Likely_To_Pass_Weak_Flag_True)
{
   /** \precond
    * Set can pass strict flag to false
    * Set tunnel strict flag to false
    * Set is likely to pass weak flag to true
    */
   zone_idx = 0U;
   circ_buff_zone_idx = zone_idx;
   n_meas = 0.0F;

   Set_Can_Pass_Strict_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   Set_Tunnel_Detected_Strict_Flag_To_False(probabilities);
   Set_Is_Likely_To_Pass_Weak_Flag_To_True(probabilities, zone_idx, circ_buff_zone_idx);

   /** \action
    * Call tested function
    */
   OCG_Cell_Classification cell_classification;
   Test_Med_Critical_Zones(probabilities, n_meas, zone_idx, circ_buff_zone_idx, calib, cell_classification);

   /** \result
    * Check whether returned status is equal to: UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER, cell_classification.underdrivability_status);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER]);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
   EXPECT_FLOAT_EQ(1.0f, cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
}

/** \purpose
 * Purpose of this test is to verify whether when tunnel weak flag is true
 * function returns UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER
 */
TEST_F(f360_test_med_critical_zones, Test_Med_Critical_Zones__Tunel_Weak_Flag_True)
{
   /** \precond
    * Set zone_idx to be higher than 0
    * Set tunnel strict flag to true
    * Set can pass strict flag to false
    * Set is likely to pass strict flag to false
    * Set false probabilities above strict tunnel threshold
    */
   zone_idx = 10U;
   circ_buff_zone_idx = zone_idx;
   n_meas = 0.0F;

   Set_Tunnel_Detected_Strict_Flag_To_True(probabilities);
   Set_Can_Pass_Strict_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   Set_Is_Likely_To_Pass_Strict_Flag_To_False(probabilities, zone_idx, circ_buff_zone_idx);
   for (uint32_t i = 0; i < NUM_CELLS_X; i++)
   {
      probabilities.can_not_pass[i] = calib.underdrive_tunnel_strict_max_can_not_pass_mean_val_med_zone + 0.01F;
   }
   /** \action
    * Call tested function
    */
   OCG_Cell_Classification cell_classification;
   Test_Med_Critical_Zones(probabilities, n_meas, zone_idx, circ_buff_zone_idx, calib, cell_classification);
   /** \result
    * Check whether returned status is equal to: UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER
    */
   EXPECT_EQ(UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER, cell_classification.underdrivability_status);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER]);
   EXPECT_TRUE(cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] > cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
   EXPECT_FLOAT_EQ(1.0f, cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] + cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER]);
}
/** @}*/
