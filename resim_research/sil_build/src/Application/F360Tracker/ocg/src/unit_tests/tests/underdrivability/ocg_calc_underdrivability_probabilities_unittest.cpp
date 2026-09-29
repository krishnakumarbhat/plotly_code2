/** \file
 * This file contains unit tests for content of ocg_calc_underdrivability_probabilities.cpp file
 */

#include "ocg_calc_underdrivability_probabilities.h"
#include <gtest/gtest.h>

#include "ocg_calibrations.h"

using namespace ocg;
using namespace rspp_variant_A;

/** \defgroup  f360_calc_underdrivability_probabilities
 *  @{
 */

/** \brief
 * Test group of Calc_Underdrivability_Probabilities function. Tests verify
 * whether zones probabilities are calculated.
 */
class f360_calc_underdrivability_probabilities : public ::testing::Test
{
protected:
   OCG_Underdrivability_Internal_T underdrivability{};
   OCG_Zones_Innovation_T zones_innovation[NUM_CELLS_X]{};
   OCG_Calibrations_T calib{};

   double threshold{1e-4};

   void SetUp() override
   {
      Initialize_OCG_Calibrations(calib);
   }
};

/** \purpose
 * Purpose of this test is to verify whether when zones states are updated.
 * \req
 * NA.
 */
TEST_F(f360_calc_underdrivability_probabilities, Calc_Underdrivability_Probabilities__state_is_updated)
{
   /** \precond
    * Initialize zones innovation with data.
    */
   for (auto &x : zones_innovation)
   {
      x.height_can_pass[0] = 1.0F;
      x.height_can_pass[1] = 1.0F;
      x.height_can_pass[2] = 1.0F;
   }

   /** \action
    * Call tested function.
    */
   Calc_Underdrivability_Probabilities(underdrivability, zones_innovation, calib);

   /** \result
    * Check whether zones state were updated.
    */
   for (auto &x : underdrivability.zones)
   {
      EXPECT_FLOAT_EQ(1.0F, x.state_height_can_pass[0]);
      EXPECT_FLOAT_EQ(1.0F, x.state_height_can_pass[1]);
      EXPECT_FLOAT_EQ(1.0F, x.state_height_can_pass[2]);
   }
}

/** \purpose
 * Purpose of this test is to verify whether zones probabilities are set to -1.0F
 * \req
 * NA.
 */
TEST_F(f360_calc_underdrivability_probabilities, Calc_Underdrivability_Probabilities__probabilities_are_reset)
{
   /** \precond
    * Initialize zones state with data to make valid probabilities.
    * Set degrees of freedom below threshold.
    */
   for (auto &x : underdrivability.zones)
   {
      x.state_height_can_pass[0] = calib.underdrive_t_test_var_iter_min_num_dets - 1.0F;
      x.state_height_can_pass[1] = 0.25F;
      x.state_height_can_pass[2] = 0.5F;

      x.state_RCS_slope_can_pass[0] = calib.underdrive_t_test_var_slope_min_num_dets - 1.0F;
      x.state_RCS_slope_can_pass[1] = 1.0F;
      x.state_RCS_slope_can_pass[2] = 100.0F;
      x.state_RCS_slope_can_pass[3] = 0.0F;
      x.state_RCS_slope_can_pass[4] = 5.0F;
      x.state_RCS_slope_can_pass[5] = 0.0F;
   }

   /** \action
    * Call tested function.
    */
   Calc_Underdrivability_Probabilities(underdrivability, zones_innovation, calib);

   /** \result
    * Check whether zone p_can_pass probabilities were set to -1.0F
    */

   for (const auto &x : underdrivability.zones)
   {
      EXPECT_FLOAT_EQ(-1.0F, x.p_can_pass);
   }
}

/** \purpose
 * Purpose of this test is to verify whether when given specific input, returned p_can_pass probability is equal to 1.0F.
 * \req
 * NA.
 */
TEST_F(f360_calc_underdrivability_probabilities, Calc_Underdrivability_Probabilities__specific_input_p_can_pass_prob_equal_to_one)
{
   /** \precond
    * Initialize state of selected zone to make it's probability be set to 1.0F
    * Set hypothesis of slope_can_pass to -1.0F
    */

   underdrivability.zones[2].state_height_can_pass[0] = calib.underdrive_t_test_var_iter_min_num_dets + 5.0F;
   underdrivability.zones[2].state_height_can_pass[1] = 1.0F;
   underdrivability.zones[2].state_height_can_pass[2] = 1.0000001F;

   underdrivability.zones[2].state_RCS_slope_can_pass[0] = 5.0F;
   underdrivability.zones[2].state_RCS_slope_can_pass[1] = 1.0F;
   underdrivability.zones[2].state_RCS_slope_can_pass[2] = 100.0F;
   underdrivability.zones[2].state_RCS_slope_can_pass[3] = 0.0F;
   underdrivability.zones[2].state_RCS_slope_can_pass[4] = 0.0001F;
   underdrivability.zones[2].state_RCS_slope_can_pass[5] = 0.0F;

   calib.underdrive_hypothesis_slope_can_pass = -1.0F;

   /** \action
    * Call tested function.
    */
   Calc_Underdrivability_Probabilities(underdrivability, zones_innovation, calib);

   /** \result
    * Check whether zone p_can_pass probabilities were set to -1.0F
    */
   EXPECT_FLOAT_EQ(1.0F, underdrivability.zones[2].p_can_pass);
}

/** \purpose
 * Purpose of this test is to verify whether when given specific input, returned p_can_pass probability is equal to 0.0F.
 * \req
 * NA.
 */
TEST_F(f360_calc_underdrivability_probabilities, Calc_Underdrivability_Probabilities__specific_input_p_can_pass_prob_equal_to_zero)
{
   /** \precond
    * Initialize state of selected zone to make it's probabiliti be set to 0.0F
    * Set hypothesis of slope_can_pass to 0.0F
    */

   underdrivability.zones[0].state_height_can_pass[0] = calib.underdrive_t_test_var_iter_min_num_dets + 5.0F;
   underdrivability.zones[0].state_height_can_pass[1] = -1.0F;
   underdrivability.zones[0].state_height_can_pass[2] = 1.0000001F;

   underdrivability.zones[0].state_RCS_slope_can_pass[0] = 5.0F;
   underdrivability.zones[0].state_RCS_slope_can_pass[1] = 1.0F;
   underdrivability.zones[0].state_RCS_slope_can_pass[2] = 100.0F;
   underdrivability.zones[0].state_RCS_slope_can_pass[3] = 0.0F;
   underdrivability.zones[0].state_RCS_slope_can_pass[4] = 0.0001F;
   underdrivability.zones[0].state_RCS_slope_can_pass[5] = 0.0F;

   calib.underdrive_hypothesis_slope_can_pass = 0.0F;

   /** \action
    * Call tested function.
    */
   Calc_Underdrivability_Probabilities(underdrivability, zones_innovation, calib);

   /** \result
    * Check whether zone p_can_pass probabilities were set to 0.0F
    */
   EXPECT_FLOAT_EQ(0.0F, underdrivability.zones[0].p_can_pass);
}

/** \purpose
 * Purpose of this test is to verify whether when given specific input, returned p_can_pass probability is equal to specific value
 * \req
 * NA.
 */
TEST_F(f360_calc_underdrivability_probabilities, Calc_Underdrivability_Probabilities__specific_input_p_can_pass_prob_equal_to_specific_value)
{
   /** \precond
    * Initialize state of selected zone to make it's probabiliti be set to specific value
    */

   underdrivability.zones[0].state_height_can_pass[0] = calib.underdrive_t_test_var_iter_min_num_dets + 5.0F;
   underdrivability.zones[0].state_height_can_pass[1] = 1.0F;
   underdrivability.zones[0].state_height_can_pass[2] = 10.0F;

   underdrivability.zones[0].state_RCS_slope_can_pass[0] = 5.0F;
   underdrivability.zones[0].state_RCS_slope_can_pass[1] = 1.0F;
   underdrivability.zones[0].state_RCS_slope_can_pass[2] = 100.0F;
   underdrivability.zones[0].state_RCS_slope_can_pass[3] = 0.0F;
   underdrivability.zones[0].state_RCS_slope_can_pass[4] = 5.0F;
   underdrivability.zones[0].state_RCS_slope_can_pass[5] = 0.0F;

   /** \action
    * Call tested function.
    */
   Calc_Underdrivability_Probabilities(underdrivability, zones_innovation, calib);

   /** \result
    * Check whether zone p_can_pass probabilities were set to 0.77658242
    */
   EXPECT_FLOAT_EQ(0.77658242F, underdrivability.zones[0].p_can_pass);
}
/** @}*/
