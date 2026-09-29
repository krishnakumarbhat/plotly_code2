/** \file
 * This file contains unit tests for content of ocg_calc_underdrivability_probabilities_helpers.cpp file
 */

#include "ocg_calc_underdrivability_probabilities_helpers.h"
#include <gtest/gtest.h>

#include "ocg_calibrations.h"

using namespace ocg;
using namespace rspp_variant_A;

/** \defgroup  students_t_betainc_approx
 *  @{
 */

/** \brief
 * Test group of Students_T_Betainc_Approx function. Tests verify
 * whether forgeting factor is properly distinguished between zone ranges.
 */
class students_t_betainc_approx : public ::testing::Test
{
protected:
   OCG_Calibrations_T calib{};
   float sample_value{};
   float dof{};
   void SetUp() override
   {
      Initialize_OCG_Calibrations(calib);
   }
};

/** \purpose
 * Purpose of this test is verify whether when dof are below threshold,
 * probability is properly calculated.
 * \req
 * NA.
 */
TEST_F(students_t_betainc_approx, Students_T_Betainc_Approx__dof_below_threshold)
{
   /** \precond
    * Set dof to be below threshold
    * Set sample value to 0.5F
    */
   dof = 10.0F;
   sample_value = 0.5F;

   /** \action
    * Call Students_T_Betainc_Approx
    */
   const float result = Students_T_Betainc_Approx(calib, sample_value, dof);

   /** \result
    * Check whether probability was properly calculated.
    */
   EXPECT_FLOAT_EQ(0.627893865F, result);
}

/** \purpose
 * Purpose of this test is verify whether when dof are above threshold,
 * probability is properly calculated.
 * \req
 * NA.
 */
TEST_F(students_t_betainc_approx, Students_T_Betainc_Approx__dof_above_threshold)
{
   /** \precond
    * Set dof to be below threshold
    * Set sample value to 0.5F
    */
   dof = 20.0F;
   sample_value = 0.5F;

   /** \action
    * Call Students_T_Betainc_Approx
    */
   const float result = Students_T_Betainc_Approx(calib, sample_value, dof);

   /** \result
    * Check whether probability was properly calculated.
    */
   EXPECT_FLOAT_EQ(0.617786050F, result);
}
/** @}*/

/** \defgroup  students_t_cdf_approx
 *  @{
 */

/** \brief
 * Test group of Students_T_CDF_Approx function. Tests verify
 * whether probability calculation method is properly selected and calculated.
 */
class students_t_cdf_approx : public ::testing::Test
{
protected:
   OCG_Calibrations_T calib{};
   float sample_value{};
   float dof{};
   double threshold{1e-4};
   void SetUp() override
   {
      Initialize_OCG_Calibrations(calib);
   }
};

/** \purpose
 * Purpose of this test is to verify whether when degrees of freedom are sufficient for normal distribution
 * and sample_value is above zero, probability is properly calculated.
 * \req
 * NA.
 */
TEST_F(students_t_cdf_approx, students_t_cdf_approx__dof_greater_than_threshold_sample_value_above_zero)
{
   /** \precond
    * Set dof to be above limit
    * Set sample value to 0.5F;
    */
   dof = calib.underdrive_min_num_dets_for_approx_method + 1.0F;
   sample_value = 0.5F;

   /** \action
    * Call Students_T_CDF_Approx
    */
   const float result = Students_T_CDF_Approx(calib, sample_value, dof);

   /** \result
    * Check whether probability was properly calculated.
    */
   EXPECT_FLOAT_EQ(0.690794826F, result);
}

/** \purpose
 * Purpose of this test is to verify whether when degrees of freedom are sufficient for normal distribution
 * and sample_value is below zero, probability is properly calculated.
 * \req
 * NA.
 */
TEST_F(students_t_cdf_approx, students_t_cdf_approx__dof_greater_than_threshold_sample_value_below_zero)
{
   /** \precond
    * Set dof to be above limit
    * Set sample value to -0.5F;
    */
   dof = calib.underdrive_min_num_dets_for_approx_method + 1.0F;
   sample_value = -0.5F;

   /** \action
    * Call Students_T_CDF_Approx
    */
   const float result = Students_T_CDF_Approx(calib, sample_value, dof);

   /** \result
    * Check whether probability was properly calculated.
    */
   EXPECT_FLOAT_EQ(0.309205204F, result);
}

/** \purpose
 * Purpose of this test is to verify whether when degrees of freedom are not sufficient for normal distribution
 * and sample_value is above zero, probability value is properly calculated using betanic function.
 * \req
 * NA.
 */
TEST_F(students_t_cdf_approx, students_t_cdf_approx__dof_below_threshold_sample_value_above_zero)
{
   /** \precond
    * Set dof to be below limit
    * Set sample value to 0.5F;
    */
   dof = calib.underdrive_min_num_dets_for_approx_method - 5.0F;
   sample_value = 0.5F;

   /** \action
    * Call Students_T_CDF_Approx
    */
   const float result = Students_T_CDF_Approx(calib, sample_value, dof);

   /** \result
    * Check whether probability was properly calculated.
    */
   EXPECT_FLOAT_EQ(0.691177845F, result);
}

/** \purpose
 * Purpose of this test is to verify whether when degrees of freedom are not sufficient for normal distribution
 * and sample value is below zero, probability value is properly calculated using betanic function.
 * \req
 * NA.
 */
TEST_F(students_t_cdf_approx, students_t_cdf_approx__dof_below_threshold_sample_value_below_zero)
{
   /** \precond
    * Set dof to be below limit
    * Set sample value to -0.25F;
    */
   dof = calib.underdrive_min_num_dets_for_approx_method - 5.0F;
   sample_value = -0.25F;

   /** \action
    * Call Students_T_CDF_Approx
    */
   const float result = Students_T_CDF_Approx(calib, sample_value, dof);

   /** \result
    * Check whether probability was properly calculated.
    */
   EXPECT_FLOAT_EQ(0.401334167F, result);
}
/** @}*/

/** \defgroup  compute_forgetting_factor
 *  @{
 */

/** \brief
 * Test group of Compute_Forgetting_Factor function. Tests verify
 * whether forgeting factor is properly distinguished between zone ranges.
 */
class compute_forgetting_factor : public ::testing::Test
{
protected:
   OCG_Calibrations_T calib{};
   uint32_t zone_idx{};
   void SetUp() override
   {
      Initialize_OCG_Calibrations(calib);
   }
};

/** \purpose
 * Purpose of this test is to verify whether when zone is placed in furthest ranges,
 * it is assigned proper forgeting factor.
 * \req
 * NA.
 */
TEST_F(compute_forgetting_factor, compute_forgetting_factor__furthest_ranges)
{
   /** \precond
    * Calculate zone_idx to be placed in furthest range zones.
    */
   zone_idx = static_cast<uint32_t>((calib.underdrive_forgeting_factor_ranges_limits[0] - GRID_MIN_X_DIST) / CELL_LENGTH) + 1U;

   /** \action
    * Call Compute_Forgetting_Factor
    */
   const float result = Compute_Forgetting_Factor(calib, zone_idx);

   /** \result
    * Check whether forgetting factor was properly assigned.
    */
   EXPECT_FLOAT_EQ(calib.underdrive_forgeting_factor_furthest_ranges, result);
}

/** \purpose
 * Purpose of this test is to verify whether when zone is placed in far ranges,
 * it is assigned proper forgeting factor.
 * \req
 * NA.
 */
TEST_F(compute_forgetting_factor, compute_forgetting_factor__far_ranges)
{
   /** \precond
    * Calculate zone_idx to be placed in far range zones.
    */
   zone_idx = static_cast<uint32_t>((calib.underdrive_forgeting_factor_ranges_limits[1] - GRID_MIN_X_DIST) / CELL_LENGTH) + 1U;

   /** \action
    * Call Compute_Forgetting_Factor
    */
   const float result = Compute_Forgetting_Factor(calib, zone_idx);

   /** \result
    * Check whether forgetting factor was properly assigned.
    */
   EXPECT_FLOAT_EQ(calib.underdrive_forgeting_factor_far_ranges, result);
}

/** \purpose
 * Purpose of this test is to verify whether when zone is placed in medium ranges,
 * it is assigned proper forgeting factor.
 * \req
 * NA.
 */
TEST_F(compute_forgetting_factor, compute_forgetting_factor__medium_ranges)
{
   /** \precond
    * Calculate zone_idx to be placed in medium range zones.
    */
   zone_idx = static_cast<uint32_t>((calib.underdrive_forgeting_factor_ranges_limits[2] - GRID_MIN_X_DIST) / CELL_LENGTH) + 1U;

   /** \action
    * Call Compute_Forgetting_Factor
    */
   const float result = Compute_Forgetting_Factor(calib, zone_idx);

   /** \result
    * Check whether forgetting factor was properly assigned.
    */
   EXPECT_FLOAT_EQ(calib.underdrive_forgeting_factor_medium_ranges, result);
}

/** \purpose
 * Purpose of this test is to verify whether when zone is placed in short ranges,
 * it is assigned proper forgeting factor.
 * \req
 * NA.
 */
TEST_F(compute_forgetting_factor, compute_forgetting_factor__short_ranges)
{
   /** \precond
    * Set zone_idx to 0U
    */
   zone_idx = 0U;

   /** \action
    * Call Compute_Forgetting_Factor
    */
   const float result = Compute_Forgetting_Factor(calib, zone_idx);

   /** \result
    * Check whether forgetting factor was properly assigned.
    */
   EXPECT_FLOAT_EQ(calib.underdrive_forgeting_factor_short_ranges, result);
}
/** @}*/

/** \defgroup  compute_prob_of_height_hypothesis
 *  @{
 */

/** \brief
 * Test group of Compute_Prob_Of_Height_Hypothesis function. Tests verify
 * whether Height state is properly updated and probability properly calculated.
 */
class compute_prob_of_height_hypothesis : public ::testing::Test
{
protected:
   OCG_Calibrations_T calib{};
   float innovation[UD_HEIGHT_STATE_SIZE]{};
   float hypothesis_mean{};
   float forgetting_factor{1.0F};
   float states[UD_HEIGHT_STATE_SIZE]{};
   double threshold{1E-4};
   void SetUp() override
   {
      Initialize_OCG_Calibrations(calib);
   }
};

/** \purpose
 * Purpose of this test is to verify whether height state is properly updated with innovation data.
 * \req
 * NA.
 */
TEST_F(compute_prob_of_height_hypothesis, Compute_Prob_Of_Height_Hypothesis__state_is_updated)
{
   /** \precond
    * Fill innovation with data.
    */
   const float value_to_fill = 1.0F;
   for (auto &innovation_data : innovation)
   {
      innovation_data = value_to_fill;
   }

   /** \action
    * Call Compute_Prob_Of_Height_Hypothesis
    */
   Compute_Prob_Of_Height_Hypothesis(calib, innovation, hypothesis_mean, forgetting_factor, states);

   /** \result
    * Check whether state was properly updated.
    */
   for (const auto &state_data : states)
   {
      EXPECT_FLOAT_EQ(value_to_fill, state_data);
   }
}

/** \purpose
 * Purpose of this test is to verify whether height state is not updated when there is no innovation data.
 * \req
 * NA.
 */
TEST_F(compute_prob_of_height_hypothesis, Compute_Prob_Of_Height_Hypothesis__state_is_not_updated)
{
   /** \precond
    * Fill state with data
    * Fill innovation with data.
    * Reset innovation counter
    */
   const float value_to_fill = 2.0F;

   for (auto &state_data : states)
   {
      state_data = value_to_fill;
   }

   for (auto &innovation_data : innovation)
   {
      innovation_data = value_to_fill;
   }
   innovation[0] = 0.0F;

   /** \action
    * Call Compute_Prob_Of_Height_Hypothesis
    */
   Compute_Prob_Of_Height_Hypothesis(calib, innovation, hypothesis_mean, forgetting_factor, states);

   /** \result
    * Check whether state was not updated.
    */
   for (const auto &state_data : states)
   {
      EXPECT_FLOAT_EQ(value_to_fill, state_data);
   }
}

/** \purpose
 * Purpose of this test is to verify whether probability is set to -1.0F when
 * there is not enough measurements.
 * \req
 * NA.
 */
TEST_F(compute_prob_of_height_hypothesis, Compute_Prob_Of_Height_Hypothesis__not_enough_measurements)
{
   /** \precond
    * Set number of new measurements to value below limit.
    */
   states[0] = calib.underdrive_t_test_var_iter_min_num_dets - 1.0F;

   /** \action
    * Call Compute_Prob_Of_Height_Hypothesis
    */
   const float result = Compute_Prob_Of_Height_Hypothesis(calib, innovation, hypothesis_mean, forgetting_factor, states);

   /** \result
    * Check whether returned probability was set to -1.0F.
    */
   EXPECT_FLOAT_EQ(-1.0F, result);
}

/** \purpose
 * Purpose of this test is to verify whether probability is set to -1.0F when
 * sample variance is negative.
 * \req
 * NA.
 */
TEST_F(compute_prob_of_height_hypothesis, Compute_Prob_Of_Height_Hypothesis__negative_sample_variance)
{
   /** \precond
    * Set number of measurements to be above limit
    * Set mean elevation angle to 1.0F
    * Set mean squared elevation angle to 0.0F
    */
   states[0] = calib.underdrive_t_test_var_iter_min_num_dets + 5.0F;
   states[1] = 1.0F;
   states[2] = 0.0F;

   /** \action
    * Call Compute_Prob_Of_Height_Hypothesis
    */
   const float result = Compute_Prob_Of_Height_Hypothesis(calib, innovation, hypothesis_mean, forgetting_factor, states);

   /** \result
    * Check whether returned probability was set to -1.0F.
    */
   EXPECT_FLOAT_EQ(-1.0F, result);
}

/** \purpose
 * Purpose of this test is to verify whether probability is properly calculated when
 * sample variance is not close to zero.
 * \req
 * NA.
 */
TEST_F(compute_prob_of_height_hypothesis, Compute_Prob_Of_Height_Hypothesis__sample_variance_not_close_to_zero)
{
   /** \precond
    * Set number of measurements to be above limit
    * Set mean elevation angle to 1.0F
    * Set mean squared elevation angle to 10.0F
    */
   states[0] = calib.underdrive_t_test_var_iter_min_num_dets + 5.0F;
   states[1] = 1.0F;
   states[2] = 10.0F;

   /** \action
    * Call Compute_Prob_Of_Height_Hypothesis
    */
   const float result = Compute_Prob_Of_Height_Hypothesis(calib, innovation, hypothesis_mean, forgetting_factor, states);

   /** \result
    * Check whether returned probability is between 0.0F and 1.0F
    */
   EXPECT_FLOAT_EQ(0.777292132F, result);
}

/** \purpose
 * Purpose of this test is to verify whether when sample variance is close to zero and
 * test innovation is above zero - probability is set to 1.0F
 * \req
 * NA.
 */
TEST_F(compute_prob_of_height_hypothesis, Compute_Prob_Of_Height_Hypothesis__sample_variance_close_to_zero_test_innovation_above_zero)
{
   /** \precond
    * Set number of measurements to be above limit
    * Set mean elevation angle to 1.0F
    * Set mean squared elevation angle to 1.0000001F
    */
   states[0] = calib.underdrive_t_test_var_iter_min_num_dets + 5.0F;
   states[1] = 1.0F;
   states[2] = 1.0000001F;

   /** \action
    * Call Compute_Prob_Of_Height_Hypothesis
    */
   const float result = Compute_Prob_Of_Height_Hypothesis(calib, innovation, hypothesis_mean, forgetting_factor, states);

   /** \result
    * Check whether returned probability was set to 1.0F.
    */
   EXPECT_FLOAT_EQ(1.0F, result);
}

/** \purpose
 * Purpose of this test is to verify whether when sample variance is close to zero and
 * test innovation is below zero - probability is set to 0.0F
 * \req
 * NA.
 */
TEST_F(compute_prob_of_height_hypothesis, Compute_Prob_Of_Height_Hypothesis__sample_variance_close_to_zero_test_innovation_below_zero)
{
   /** \precond
    * Set number of measurements to be above limit
    * Set mean elevation angle to 1.0F
    * Set mean squared elevation angle to 1.0000001F
    * Set hypothesis mean to 2.0F
    */
   states[0] = calib.underdrive_t_test_var_iter_min_num_dets + 5.0F;
   states[1] = 1.0F;
   states[2] = 1.0000001F;

   hypothesis_mean = 2.0F;

   /** \action
    * Call Compute_Prob_Of_Height_Hypothesis
    */
   const float result = Compute_Prob_Of_Height_Hypothesis(calib, innovation, hypothesis_mean, forgetting_factor, states);

   /** \result
    * Check whether returned probability was set to 0.0F.
    */
   EXPECT_FLOAT_EQ(0.0F, result);
}
/** @}*/

/** \defgroup  compute_prob_of_rcs_slope_hypothesis
 *  @{
 */

/** \brief
 * Test group of Compute_Prob_Of_RCS_Slope_Hypothesis function. Tests verify
 * whether RCS state is properly updated and probability properly calculated.
 */
class compute_prob_of_rcs_slope_hypothesis : public ::testing::Test
{
protected:
   OCG_Calibrations_T calib{};
   float innovation[UD_RCS_STATE_SIZE]{};
   float hypothesis_slope{};
   float forgetting_factor{1.0F};
   float states[UD_RCS_STATE_SIZE]{};
   void SetUp() override
   {
      Initialize_OCG_Calibrations(calib);
   }
};

/** \purpose
 * Purpose of this test is to verify whether state is updated when there is innovation data.
 * \req
 * NA.
 */
TEST_F(compute_prob_of_rcs_slope_hypothesis, Compute_Prob_Of_RCS_Slope_Hypothesis__state_is_updated)
{
   /** \precond
    * Initialize innovation with values different than zero.
    */
   const float value_to_fill = 1.0F;
   for (auto &innovation_variable : innovation)
   {
      innovation_variable = value_to_fill;
   }

   /** \action
    * Call Compute_Prob_Of_RCS_Slope_Hypothesis
    */
   Compute_Prob_Of_RCS_Slope_Hypothesis(calib, innovation, hypothesis_slope, forgetting_factor, states);

   /** \result
    * Check whether states was properly updated.
    */
   for (auto &state_variable : states)
   {
      EXPECT_FLOAT_EQ(value_to_fill, state_variable);
   }
}

/** \purpose
 * Purpose of this test is to verify whether state is not updated when there is no innovation data.
 * \req
 * NA.
 */
TEST_F(compute_prob_of_rcs_slope_hypothesis, Compute_Prob_Of_RCS_Slope_Hypothesis__state_is_not_updated)
{
   /** \precond
    * Initialize state with values different than zero.
    */
   const float value_to_fill = 2.0F;

   for (auto &state_variable : states)
   {
      state_variable = value_to_fill;
   }

   /** \action
    * Call Compute_Prob_Of_RCS_Slope_Hypothesis
    */
   Compute_Prob_Of_RCS_Slope_Hypothesis(calib, innovation, hypothesis_slope, forgetting_factor, states);

   /** \result
    * Check whether states were not updated.
    */
   for (auto &state_variable : states)
   {
      EXPECT_FLOAT_EQ(value_to_fill, state_variable);
   }
}

/** \purpose
 * Purpose of this test is to verify whether probability is set to -1.0F when there is not enough detections.
 * \req
 * NA.
 */
TEST_F(compute_prob_of_rcs_slope_hypothesis, Compute_Prob_Of_RCS_Slope_Hypothesis__not_enough_detections)
{
   /** \precond
    * Set number of detections to value below limit.
    */
   states[0] = calib.underdrive_t_test_var_slope_min_num_dets - 1.0F;

   /** \action
    * Call Compute_Prob_Of_RCS_Slope_Hypothesis
    */
   const float result = Compute_Prob_Of_RCS_Slope_Hypothesis(calib, innovation, hypothesis_slope, forgetting_factor, states);

   /** \result
    * Check whether returned probability is equal to -1.0F
    */
   EXPECT_FLOAT_EQ(-1.0F, result);
}

/** \purpose
 * Purpose of this test is to verify whether probability is set to -1.0F when input sample variance is too low
 * \req
 * NA.
 */
TEST_F(compute_prob_of_rcs_slope_hypothesis, Compute_Prob_Of_RCS_Slope_Hypothesis__input_sample_variance_too_low)
{
   /** \precond
    * Set number of detections to value above limit.
    * Set mean range to 1.0F
    * Set mean squared range to 0.1F * calib.underdrive_t_test_var_slope_min_input_sample_var
    */
   states[0] = calib.underdrive_t_test_var_slope_min_num_dets + 1.0F;
   states[1] = 1.0F;
   states[2] = 0.1F * calib.underdrive_t_test_var_slope_min_input_sample_var;

   /** \action
    * Call Compute_Prob_Of_RCS_Slope_Hypothesis
    */
   const float result = Compute_Prob_Of_RCS_Slope_Hypothesis(calib, innovation, hypothesis_slope, forgetting_factor, states);

   /** \result
    * Check whether returned probability is equal to -1.0F
    */
   EXPECT_FLOAT_EQ(-1.0F, result);
}

/** \purpose
 * Purpose of this test is to verify whether probability is set to -1.0F if prediction sample variance is too low.
 * \req
 * NA.
 */
TEST_F(compute_prob_of_rcs_slope_hypothesis, Compute_Prob_Of_RCS_Slope_Hypothesis__prediction_sample_variance_too_low)
{
   /** \precond
    * Set number of detections to value above limit.
    * Set mean range to 1.0F
    * Set mean squared range to 5.0F * calib.underdrive_t_test_var_slope_min_input_sample_var
    * Set mean amplitude to 5.0F
    * Set mean squared amplitude to 1.0f
    * Set mean product of range and amplitude to 1.0F
    */
   states[0] = calib.underdrive_t_test_var_slope_min_num_dets + 1.0F;
   states[1] = 1.0F;
   states[2] = 5.0F * calib.underdrive_t_test_var_slope_min_input_sample_var;
   states[3] = 5.0F;
   states[4] = 1.0F;
   states[5] = 1.0F;

   /** \action
    * Call Compute_Prob_Of_RCS_Slope_Hypothesis
    */
   const float result = Compute_Prob_Of_RCS_Slope_Hypothesis(calib, innovation, hypothesis_slope, forgetting_factor, states);

   /** \result
    * Check whether returned probability is equal to -1.0F
    */
   EXPECT_FLOAT_EQ(-1.0F, result);
}

/** \purpose
 * Purpose of this test is to verify whether probability is calculated when all conditions are met.
 * \req
 * NA.
 */
TEST_F(compute_prob_of_rcs_slope_hypothesis, Compute_Prob_Of_RCS_Slope_Hypothesis__all_conditions_are_met)
{
   /** \precond
    * Set actual number of detections to 0.1F
    * Set innovation number of detections to 5.0F
    * Set mean range innovation to 1.0F
    * Set mean squared range innovation to 100.0F
    * Set mean amplitude (RCS) innovation to 0.0F
    * Set mean squared amplitude innovation to 5.0F
    * Set mean product of range and aplitude innovation to 0.0F
    */
   states[0] = 0.1F;
   innovation[0] = 5.0F;
   innovation[1] = 1.0F;
   innovation[2] = 100.0F;
   innovation[3] = 0.0F;
   innovation[4] = 5.0F;
   innovation[5] = 0.0F;

   /** \action
    * Call Compute_Prob_Of_RCS_Slope_Hypothesis
    */
   const float result = Compute_Prob_Of_RCS_Slope_Hypothesis(calib, innovation, hypothesis_slope, forgetting_factor, states);

   /** \result
    * Check whether returned probability is equal to 0.5F
    */
   EXPECT_FLOAT_EQ(0.5F, result);
}

/** \purpose
 * Purpose of this test is to verify whether probability is set to 1.0F when slope sample variance
 * is too low and test innovation is positive.
 * \req
 * NA.
 */
TEST_F(compute_prob_of_rcs_slope_hypothesis, Compute_Prob_Of_RCS_Slope_Hypothesis__slope_sample_var_too_low_positive_test_innovation)
{
   /** \precond
    * Set actual number of detections to 0.1F
    * Set innovation number of detections to 5.0F
    * Set mean range innovation to 1.0F
    * Set mean squared range innovation to 100.0F
    * Set mean amplitude (RCS) innovation to 0.0F
    * Set mean squared amplitude innovation to 0.0001F
    * Set mean product of range and aplitude innovation to 0.0F
    * Set hypothesis slope to -1.0F;
    */
   states[0] = 0.1F;
   innovation[0] = 5.0F;
   innovation[1] = 1.0F;
   innovation[2] = 100.0F;
   innovation[3] = 0.0F;
   innovation[4] = 0.0001F;
   innovation[5] = 0.0F;

   hypothesis_slope = -1.0F;

   /** \action
    * Call Compute_Prob_Of_RCS_Slope_Hypothesis
    */
   const float result = Compute_Prob_Of_RCS_Slope_Hypothesis(calib, innovation, hypothesis_slope, forgetting_factor, states);

   /** \result
    * Check whether returned probability is equal to 1.0F
    */
   EXPECT_FLOAT_EQ(1.0F, result);
}

/** \purpose
 * Purpose of this test is to verify whether probability is set to 0.0F when sample variance is too low and
 * test innovation is either zero or negative.
 * \req
 * NA.
 */
TEST_F(compute_prob_of_rcs_slope_hypothesis, Compute_Prob_Of_RCS_Slope_Hypothesis__slope_sample_var_too_low_negative_test_innovation)
{
   /** \precond
    * Set actual number of detections to 0.1F
    * Set innovation number of detections to 5.0F
    * Set mean range innovation to 1.0F
    * Set mean squared range innovation to 100.0F
    * Set mean amplitude (RCS) innovation to 0.0F
    * Set mean squared amplitude innovation to 0.0001F
    * Set mean product of range and aplitude innovation to 0.0F
    * Set hypothesis slope to 0.0F;
    */
   states[0] = 0.1F;
   innovation[0] = 5.0F;
   innovation[1] = 1.0F;
   innovation[2] = 100.0F;
   innovation[3] = 0.0F;
   innovation[4] = 0.0001F;
   innovation[5] = 0.0F;

   hypothesis_slope = 0.0F;

   /** \action
    * Call Compute_Prob_Of_RCS_Slope_Hypothesis
    */
   const float result = Compute_Prob_Of_RCS_Slope_Hypothesis(calib, innovation, hypothesis_slope, forgetting_factor, states);

   /** \result
    * Check whether returned probability is equal to 0.0F
    */
   EXPECT_FLOAT_EQ(0.0F, result);
}
/** @}*/

/** \defgroup  calc_single_zone_probabilities
 *  @{
 */

/** \brief
 * Test group of Calc_Single_Zone_Probabilities function. Tests verify
 * whether probabilities are calculated.
 */
class calc_single_zone_probabilities : public ::testing::Test
{
protected:
   OCG_Single_Underdrivability_Zone_T zone_state{};
   OCG_Zones_Innovation_T zone_innovation{};
   OCG_Calibrations_T calib{};
   uint32_t zone_idx{0U};
   void SetUp() override
   {
      Initialize_OCG_Calibrations(calib);
   }

   void Set_Height_Coef_To_Make_Valid_Probability(
       float (&innovation)[UD_HEIGHT_STATE_SIZE])
   {
      innovation[0] = 30.0F;
      innovation[1] = 0.25F;
      innovation[2] = 0.5F;
   }

   void Set_RCS_Coef_To_Make_Valid_Probability(
       float (&state)[UD_RCS_STATE_SIZE],
       float (&innovation)[UD_RCS_STATE_SIZE])
   {

      state[0] = 0.1F;
      innovation[0] = 5.0F;
      innovation[1] = 1.0F;
      innovation[2] = 100.0F;
      innovation[3] = 0.0F;
      innovation[4] = 5.0F;
      innovation[5] = 0.0F;
   }

   bool Is_Between_Zero_And_One(float prob)
   {
      return ((0.0F < prob) && (prob < 1.0F));
   }
};

/** \purpose
 * Purpose of this test is to verify whether probabilities are calculated.
 * \req
 * NA.
 */
TEST_F(calc_single_zone_probabilities, Calc_Single_Zone_Probabilities__probabilities_are_calculated)
{
   /** \precond
    * Set innovations and states to make probabilities valid
    * Reset tested probabilities
    */
   Set_Height_Coef_To_Make_Valid_Probability(zone_innovation.height_can_pass);
   Set_Height_Coef_To_Make_Valid_Probability(zone_innovation.height_is_likely_to_pass);
   Set_Height_Coef_To_Make_Valid_Probability(zone_innovation.height_can_not_pass_upper);
   Set_Height_Coef_To_Make_Valid_Probability(zone_innovation.height_can_not_pass_lower);

   Set_RCS_Coef_To_Make_Valid_Probability(zone_state.state_RCS_slope_can_pass, zone_innovation.RCS_slope_can_pass);
   Set_RCS_Coef_To_Make_Valid_Probability(zone_state.state_RCS_slope_is_likely_to_pass, zone_innovation.RCS_slope_is_likely_to_pass);
   Set_RCS_Coef_To_Make_Valid_Probability(zone_state.state_RCS_slope_can_not_pass_upper, zone_innovation.RCS_slope_can_not_pass);

   zone_state.p_height_can_pass = 0.0F;
   zone_state.p_height_is_likely_to_pass = 0.0F;
   zone_state.p_height_can_not_pass_upper = 0.0F;
   zone_state.p_height_can_not_pass_lower = 0.0F;

   zone_state.p_RCS_slope_can_pass = 0.0F;
   zone_state.p_RCS_slope_is_likely_to_pass = 0.0F;
   zone_state.p_RCS_slope_can_not_pass_upper = 0.0F;
   zone_state.p_RCS_slope_can_not_pass_lower = 0.0F;

   /** \action
    * Call Calc_Single_Zone_Probabilities
    */
   Calc_Single_Zone_Probabilities(zone_state, zone_innovation, calib, zone_idx);

   /** \result
    * Check whether zone p_height_can_pass probability is between 0 and 1
    */
   EXPECT_TRUE(Is_Between_Zero_And_One(zone_state.p_height_can_pass));
   EXPECT_TRUE(Is_Between_Zero_And_One(zone_state.p_height_is_likely_to_pass));
   EXPECT_TRUE(Is_Between_Zero_And_One(zone_state.p_height_can_not_pass_upper));
   EXPECT_TRUE(Is_Between_Zero_And_One(zone_state.p_height_can_not_pass_lower));

   EXPECT_TRUE(Is_Between_Zero_And_One(zone_state.p_RCS_slope_can_pass));
   EXPECT_TRUE(Is_Between_Zero_And_One(zone_state.p_RCS_slope_is_likely_to_pass));
   EXPECT_TRUE(Is_Between_Zero_And_One(zone_state.p_RCS_slope_can_not_pass_upper));
   EXPECT_TRUE(Is_Between_Zero_And_One(zone_state.p_RCS_slope_can_not_pass_lower));
}

/** \purpose
 * Purpose of this test is to verify whether complementary can_not_pass probabilities are not calculated when result is negative.
 * \req
 * NA.
 */
TEST_F(calc_single_zone_probabilities, Calc_Single_Zone_Probabilities__complementary_probabilities_are_negative)
{
   /** \precond
    * Set p_height_can_not_pass_upper to 0.0F
    * Set p_RCS_slope_can_not_pass_upper to 0.0F
    */

   zone_state.p_height_can_not_pass_upper = 0.0F;
   zone_state.p_RCS_slope_can_not_pass_upper = 0.0F;
   zone_state.p_height_can_not_pass_upper = 0.0F;
   zone_state.p_height_can_not_pass_lower = 0.0F;

   /** \action
    * Call Calc_Single_Zone_Probabilities
    */
   Calc_Single_Zone_Probabilities(zone_state, zone_innovation, calib, zone_idx);

   /** \result
    * Check whether zone p_height_can_not_pass_upper and p_RCS_slope_can_not_pass_upper are equal to -1.0F
    */
   EXPECT_FLOAT_EQ(-1.0F, zone_state.p_height_can_not_pass_upper);
   EXPECT_FLOAT_EQ(-1.0F, zone_state.p_RCS_slope_can_not_pass_upper);
}
/** @}*/

/** \defgroup  assign_single_zone_probabilities
 *  @{
 */

/** \brief
 * Test group of Assign_Single_Zone_Probabilities function. Tests verify
 * whether probabilities are properly aggregated.
 */
class assign_single_zone_probabilities : public ::testing::Test
{
protected:
   OCG_Single_Underdrivability_Zone_T zones[NUM_CELLS_X]{};
   uint32_t zone_idx{0U};
   uint32_t circ_buff_zone_idx{0U};
};

/** \purpose
 * Purpose of this test is to verify whether when p_height_can_pass probability is below zero, p_can_pass probability is set to -1.0F
 * \req
 * NA.
 */
TEST_F(assign_single_zone_probabilities, Assign_Single_Zone_Probabilities__p_height_can_pass_below_zero)
{
   /** \precond
    * Set p_height_can_pass to value below zero.
    * Set p_RCS_slope_can_pass to value above zero.
    */
   zones[circ_buff_zone_idx].p_height_can_pass = -0.5F;
   zones[circ_buff_zone_idx].p_RCS_slope_can_pass = 0.5F;

   /** \action
    * Call Assign_Single_Zone_Probabilities
    */
   Assign_Single_Zone_Probabilities(zones, zone_idx, circ_buff_zone_idx);

   /** \result
    * Check whether zone p_can_pass was set to -1.0F;
    */
   EXPECT_FLOAT_EQ(-1.0F, zones[zone_idx].p_can_pass);
}


/** \purpose
 * Purpose of this test is to verify whether when both p_height_can_pass and p_RCS_slope_can_pass probability
 * are above zero, p_can_pass probability is calculated.
 * \req
 * NA.
 */
TEST_F(assign_single_zone_probabilities, Assign_Single_Zone_Probabilities__both_can_pass_prob_above_zero)
{
   /** \precond
    * Set p_height_can_pass to value below zero.
    * Set p_RCS_slope_can_pass to value above zero.
    */
   zones[circ_buff_zone_idx].p_height_can_pass = 0.5F;
   zones[circ_buff_zone_idx].p_RCS_slope_can_pass = 0.5F;

   /** \action
    * Call Assign_Single_Zone_Probabilities
    */
   Assign_Single_Zone_Probabilities(zones, zone_idx, circ_buff_zone_idx);

   /** \result
    * Check whether zone p_can_pass was set to 0.25F;
    */
   EXPECT_FLOAT_EQ(0.5F, zones[zone_idx].p_can_pass);
}

/** \purpose
 * Purpose of this test is to verify whether when p_height_is_likely_to_pass probability is below zero, p_is_likely_to_pass probability is set to -1.0F
 * \req
 * NA.
 */
TEST_F(assign_single_zone_probabilities, Assign_Single_Zone_Probabilities__p_height_is_likely_to_pass_below_zero)
{
   /** \precond
    * Set p_height_can_pass to value below zero.
    * Set p_RCS_slope_can_pass to value above zero.
    */
   zones[circ_buff_zone_idx].p_height_is_likely_to_pass = -0.5F;
   zones[circ_buff_zone_idx].p_RCS_slope_is_likely_to_pass = 0.5F;

   /** \action
    * Call Assign_Single_Zone_Probabilities
    */
   Assign_Single_Zone_Probabilities(zones, zone_idx, circ_buff_zone_idx);

   /** \result
    * Check whether zone p_is_likely_to_pass was set to -1.0F;
    */
   EXPECT_FLOAT_EQ(-1.0F, zones[zone_idx].p_is_likely_to_pass);
}

/** \purpose
 * Purpose of this test is to verify whether when p_RCS_slope_is_likely_to_pass probability is below zero, p_is_likely_to_pass probability is set to -1.0F
 * \req
 * NA.
 */
TEST_F(assign_single_zone_probabilities, Assign_Single_Zone_Probabilities__p_RCS_slope_is_likely_to_pass_below_zero)
{
   /** \precond
    * Set p_height_can_pass to value below zero.
    * Set p_RCS_slope_can_pass to value above zero.
    */
   zones[circ_buff_zone_idx].p_height_is_likely_to_pass = 0.5F;
   zones[circ_buff_zone_idx].p_RCS_slope_is_likely_to_pass = -0.5F;

   /** \action
    * Call Assign_Single_Zone_Probabilities
    */
   Assign_Single_Zone_Probabilities(zones, zone_idx, circ_buff_zone_idx);

   /** \result
    * Check whether zone p_is_likely_to_pass was set to -1.0F;
    */
   EXPECT_FLOAT_EQ(-1.0F, zones[zone_idx].p_is_likely_to_pass);
}

/** \purpose
 * Purpose of this test is to verify whether when both p_height_is_likely_to_pass and p_RCS_slope_is_likely_to_pass probability
 * are above zero, p_is_likely_to_pass probability is calculated.
 * \req
 * NA.
 */
TEST_F(assign_single_zone_probabilities, Assign_Single_Zone_Probabilities__both_likely_to_pass_prob_above_zero)
{
   /** \precond
    * Set p_height_can_pass to value below zero.
    * Set p_RCS_slope_can_pass to value above zero.
    */
   zones[circ_buff_zone_idx].p_height_is_likely_to_pass = 0.5F;
   zones[circ_buff_zone_idx].p_RCS_slope_is_likely_to_pass = 0.5F;

   /** \action
    * Call Assign_Single_Zone_Probabilities
    */
   Assign_Single_Zone_Probabilities(zones, zone_idx, circ_buff_zone_idx);

   /** \result
    * Check whether zone p_is_likely_to_pass was set to 0.25F;
    */
   EXPECT_FLOAT_EQ(0.75F, zones[zone_idx].p_is_likely_to_pass);
}

/** \purpose
 * Purpose of this test is to verify whether when p_height_can_not_pass_lower probability is below zero, p_can_not_pass probability is set to -1.0F
 * \req
 * NA.
 */
TEST_F(assign_single_zone_probabilities, Assign_Single_Zone_Probabilities__p_height_can_not_pass_lower_below_zero)
{
   /** \precond
    * Set p_height_can_not_pass_lower to value below zero.
    * Set p_RCS_slope_can_not_pass_lower to value above zero.
    * Set p_height_can_not_pass_upper to value above zero.
    * Set p_RCS_slope_can_not_pass_upper to value above zero.
    */
   zones[circ_buff_zone_idx].p_height_can_not_pass_lower = -0.5F;
   zones[circ_buff_zone_idx].p_RCS_slope_can_not_pass_lower = 0.5F;
   zones[circ_buff_zone_idx].p_height_can_not_pass_upper = 0.5F;
   zones[circ_buff_zone_idx].p_RCS_slope_can_not_pass_upper = 0.5F;

   /** \action
    * Call Assign_Single_Zone_Probabilities
    */
   Assign_Single_Zone_Probabilities(zones, zone_idx, circ_buff_zone_idx);

   /** \result
    * Check whether zone p_can_not_pass was set to -1.0F;
    */
   EXPECT_FLOAT_EQ(-1.0F, zones[zone_idx].p_can_not_pass);
}

/** \purpose
 * Purpose of this test is to verify whether when p_RCS_slope_can_not_pass_lower probability is below zero, p_can_not_pass probability is set to -1.0F
 * \req
 * NA.
 */
TEST_F(assign_single_zone_probabilities, Assign_Single_Zone_Probabilities__p_RCS_slope_can_not_pass_lower_below_zero)
{
   /** \precond
    * Set p_height_can_not_pass_lower to value above zero.
    * Set p_RCS_slope_can_not_pass_lower to value below zero.
    * Set p_height_can_not_pass_upper to value above zero.
    * Set p_RCS_slope_can_not_pass_upper to value above zero.
    */
   zones[circ_buff_zone_idx].p_height_can_not_pass_lower = 0.5F;
   zones[circ_buff_zone_idx].p_RCS_slope_can_not_pass_lower = -0.5F;
   zones[circ_buff_zone_idx].p_height_can_not_pass_upper = 0.5F;
   zones[circ_buff_zone_idx].p_RCS_slope_can_not_pass_upper = 0.5F;

   /** \action
    * Call Assign_Single_Zone_Probabilities
    */
   Assign_Single_Zone_Probabilities(zones, zone_idx, circ_buff_zone_idx);

   /** \result
    * Check whether zone p_can_not_pass was set to -1.0F;
    */
   EXPECT_FLOAT_EQ(-1.0F, zones[zone_idx].p_can_not_pass);
}

/** \purpose
 * Purpose of this test is to verify whether when p_height_can_not_pass_upper probability is below zero, p_can_not_pass probability is set to -1.0F
 * \req
 * NA.
 */
TEST_F(assign_single_zone_probabilities, Assign_Single_Zone_Probabilities__p_height_can_not_pass_upper_below_zero)
{
   /** \precond
    * Set p_height_can_not_pass_lower to value above zero.
    * Set p_RCS_slope_can_not_pass_lower to value above zero.
    * Set p_height_can_not_pass_upper to value below zero.
    * Set p_RCS_slope_can_not_pass_upper to value above zero.
    */
   zones[circ_buff_zone_idx].p_height_can_not_pass_lower = 0.5F;
   zones[circ_buff_zone_idx].p_RCS_slope_can_not_pass_lower = 0.5F;
   zones[circ_buff_zone_idx].p_height_can_not_pass_upper = -0.5F;
   zones[circ_buff_zone_idx].p_RCS_slope_can_not_pass_upper = 0.5F;

   /** \action
    * Call Assign_Single_Zone_Probabilities
    */
   Assign_Single_Zone_Probabilities(zones, zone_idx, circ_buff_zone_idx);

   /** \result
    * Check whether zone p_can_not_pass was set to -1.0F;
    */
   EXPECT_FLOAT_EQ(-1.0F, zones[zone_idx].p_can_not_pass);
}

/** \purpose
 * Purpose of this test is to verify whether when p_RCS_slope_can_not_pass_upper probability is below zero, p_can_not_pass probability is set to -1.0F
 * \req
 * NA.
 */
TEST_F(assign_single_zone_probabilities, Assign_Single_Zone_Probabilities__p_RCS_slope_can_not_pass_upper_below_zero)
{
   /** \precond
    * Set p_height_can_not_pass_lower to value above zero.
    * Set p_RCS_slope_can_not_pass_lower to value above zero.
    * Set p_height_can_not_pass_upper to value above zero.
    * Set p_RCS_slope_can_not_pass_upper to value below zero.
    */
   zones[circ_buff_zone_idx].p_height_can_not_pass_lower = 0.5F;
   zones[circ_buff_zone_idx].p_RCS_slope_can_not_pass_lower = 0.5F;
   zones[circ_buff_zone_idx].p_height_can_not_pass_upper = 0.5F;
   zones[circ_buff_zone_idx].p_RCS_slope_can_not_pass_upper = -0.5F;

   /** \action
    * Call Assign_Single_Zone_Probabilities
    */
   Assign_Single_Zone_Probabilities(zones, zone_idx, circ_buff_zone_idx);

   /** \result
    * Check whether zone p_can_not_pass was set to -1.0F;
    */
   EXPECT_FLOAT_EQ(-1.0F, zones[zone_idx].p_can_not_pass);
}

/** \purpose
 * Purpose of this test is to verify whether when all can_not_pass probabilities are above zero, p_can_not_pass probability is properly calculated.
 * \req
 * NA.
 */
TEST_F(assign_single_zone_probabilities, Assign_Single_Zone_Probabilities__all_can_not_pass_prob_above_zero)
{
   /** \precond
    * Set p_height_can_not_pass_lower to value above zero.
    * Set p_RCS_slope_can_not_pass_lower to value above zero.
    * Set p_height_can_not_pass_upper to value above zero.
    * Set p_RCS_slope_can_not_pass_upper to value above zero.
    */
   zones[circ_buff_zone_idx].p_height_can_not_pass_lower = 1.0F;
   zones[circ_buff_zone_idx].p_RCS_slope_can_not_pass_lower = 1.0F;
   zones[circ_buff_zone_idx].p_height_can_not_pass_upper = 1.0F;
   zones[circ_buff_zone_idx].p_RCS_slope_can_not_pass_upper = 0.5F;

   /** \action
    * Call Assign_Single_Zone_Probabilities
    */
   Assign_Single_Zone_Probabilities(zones, zone_idx, circ_buff_zone_idx);

   /** \result
    * Check whether zone p_can_not_pass was set to 0.5F;
    */
   EXPECT_FLOAT_EQ(1.0F, zones[zone_idx].p_can_not_pass);
}

/** \purpose
 * Purpose of this test is to verify whether when can_not_pass probabilities indicate that can_not_pass probability
 * should be set to 1 - it is properly set.
 * \req
 * NA.
 */
TEST_F(assign_single_zone_probabilities, Assign_Single_Zone_Probabilities__can_not_pass_equal_to_one)
{
   /** \precond
    * Set p_height_can_not_pass_lower to value above zero.
    * Set p_RCS_slope_can_not_pass_lower to value above zero.
    * Set p_height_can_not_pass_upper to value above zero.
    * Set p_RCS_slope_can_not_pass_upper to value above zero.
    */
   zones[circ_buff_zone_idx].p_height_can_not_pass_lower = 1.0F;
   zones[circ_buff_zone_idx].p_RCS_slope_can_not_pass_lower = 1.0F;
   zones[circ_buff_zone_idx].p_height_can_not_pass_upper = 1.0F;
   zones[circ_buff_zone_idx].p_RCS_slope_can_not_pass_upper = 1.0F;

   /** \action
    * Call Assign_Single_Zone_Probabilities
    */
   Assign_Single_Zone_Probabilities(zones, zone_idx, circ_buff_zone_idx);

   /** \result
    * Check whether zone p_can_not_pass was set to 0.5F;
    */
   EXPECT_FLOAT_EQ(1.0F, zones[zone_idx].p_can_not_pass);
}

/** \purpose
 * Purpose of this test is to verify whether when can_not_pass probabilities indicate that can_not_pass probability
 * should be set to 0.0F it is properly set.
 * \req
 * NA.
 */
TEST_F(assign_single_zone_probabilities, Assign_Single_Zone_Probabilities__can_not_pass_equal_to_zero)
{
   /** \precond
    * Set p_height_can_not_pass_lower to value above zero.
    * Set p_RCS_slope_can_not_pass_lower to value above zero.
    * Set p_height_can_not_pass_upper to value above zero.
    * Set p_RCS_slope_can_not_pass_upper to value above zero.
    */
   zones[circ_buff_zone_idx].p_height_can_not_pass_lower = 1.0F;
   zones[circ_buff_zone_idx].p_RCS_slope_can_not_pass_lower = 1.0F;
   zones[circ_buff_zone_idx].p_height_can_not_pass_upper = 0.0F;
   zones[circ_buff_zone_idx].p_RCS_slope_can_not_pass_upper = 0.0F;

   /** \action
    * Call Assign_Single_Zone_Probabilities
    */
   Assign_Single_Zone_Probabilities(zones, zone_idx, circ_buff_zone_idx);

   /** \result
    * Check whether zone p_can_not_pass was set to 0.5F;
    */
   EXPECT_FLOAT_EQ(0.0F, zones[zone_idx].p_can_not_pass);
}
/** @}*/
