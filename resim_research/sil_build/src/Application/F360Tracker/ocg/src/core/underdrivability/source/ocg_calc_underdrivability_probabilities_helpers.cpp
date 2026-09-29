/*===================================================================================*\
* FILE: ocg_calc_underdrivability_probabilities_helpers.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Calc_Underdrivability_Probabilities() helper functions definitions
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 5045)
#endif

#include <cmath>
#include "ocg_calc_underdrivability_probabilities_helpers.h"
#include "cmn_math_func.h"
#include "cmn_math_constants.h"
#include "ocg_underdrivability_states.h"

namespace ocg
{

   /*===========================================================================*\
   * FUNCTION: Students_T_Betainc_Approx()
   *===========================================================================
   * RETURN VALUE:
   * float
   *
   * PARAMETERS:
   * const OCG_Calibrations_T& calib
   * const float sample_value
   * const float dof
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function calculates cumulative distribution function coefficient.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float Students_T_Betainc_Approx(
       const OCG_Calibrations_T &calib,
       const float sample_value,
       const float dof)
   {
      /* In order to compute the Cumulative Distribution Function(CDF) for
       * a student's T distribution, the "regularized (incomplete) beta function"
       * has to be computed.Matlab's betainc() does the job, but is unnecessarily
       * general to handle this simpler case.
       *
       * The incomplete beta function requires arguments "a" and
       * "b", where "b" always equals 0.5 for Student's T CDF, thus allowing
       * simplifications.
       *
       * By analytically integrating :
       * integral_0_to_x[t ^ (alfa - 1) / sqrt(1 - t)] dt
       * the solution will be
       * [-2 * sqrt(1 - x) * "2F1(a, b; c; d)"]
       * where "2F1" is the hypergeometric function defined as
       * (a)_n(b)_n   z^n
       *   sum_n_from_0_to_inf------------ * -- -
       * (c)_n      n!
       *
       *   a = 0.5
       *   b = 1 - alfa
       *       where alfa = dof / 2
       *   c = 1.5
       *   z = 1 - x
       *      where x = dof / (tVar ^ 2 + dof)
       *         tVar = Student's T test variable
       *         dof = Student's T degrees of freedom
       *
       * Furthermore, since an approximate answer is sufficient we can limit the
       * number of iterations to compute the hypergeometric function to a number
       * known at build time, allowing for pre - computation of MAX_ITATIONS number
       * of factors. */

      // Only two of the arguments passed to the 2F1 function are varying
      const float b = 1.0F - 0.5F * dof;
      const float z = 1.0F - dof / (sample_value * sample_value + dof);
      const float min_dof_for_approx_calc = 15.0F;

      // Iteratively compute the upper and lower values of the incomplete beta function's integral
      float it_add_x = 1.0F;
      float it_res_at_x = 1.0F;
      for (uint32_t k = 0U; k < calib.underdrive_max_iterations; k++)
      {
         const float new_add_factor = (b + static_cast<float>(k)) * calib.underdrive_pre_computed_factors[k];
         it_add_x = it_add_x * new_add_factor * z;
         it_res_at_x = it_res_at_x + it_add_x;

         if (std::fabs(it_add_x) < ocg::cmn::OCG_EPSILON)
         {
            break;
         }
      }

      float result;
      // for dof < 15 use iterative calculation method
      if (dof < min_dof_for_approx_calc)
      {
         float it_add_zero = 1.0F;
         float it_res_at_zero = 1.0F;
         for (uint32_t k = 0U; k < calib.underdrive_max_iterations; k++)
         {
            const float new_add_factor = (b + static_cast<float>(k)) * calib.underdrive_pre_computed_factors[k];
            it_add_zero = it_add_zero * new_add_factor;
            it_res_at_zero = it_res_at_zero + it_add_zero;

            if (std::fabs(it_add_zero) < (100.0F * ocg::cmn::OCG_EPSILON))
            {
               break;
            }
         }
         result = (-2.0F * sqrtf(z) * it_res_at_x + 2.0F * it_res_at_zero) / (2.0F * it_res_at_zero);
      }
      else
      {
         // For dof >= 15, use an approximation of the Beta function to avoid numerical issues
         const float gamma_approx_high_dof = calib.underdrive_sqrt_pi / sqrtf(0.5F * dof);
         result = ((-2.0F * sqrtf(z) * it_res_at_x + gamma_approx_high_dof) / gamma_approx_high_dof);
      }

      // Limit output to [0, 1]
      result = cmn::Saturate(result, 0.0F, 1.0F);

      return result;
   }

   /*===========================================================================*\
   * FUNCTION: Students_T_CDF_Approx()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const OCG_Calibrations_T& calib
   * const float sample_value
   * const float dof
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function calculates probability using student_t distribution.
   *
   * PRECONDITIONS:
   * dof greater or equal 2
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float Students_T_CDF_Approx(
       const OCG_Calibrations_T &calib,
       const float sample_value,
       const float dof)
   {
      // A constraint for this function is that the input dof must be at least 2 or larger.
      // There is no check for this in the function so it has to be checked before function is called.
      float cdf_value;

      if (calib.underdrive_min_num_dets_for_approx_method < dof)
      {
         /* Above 30 degrees of freedom, the Student's T-test can be
          * approximated with the simpler Normal distribution CDF.
          * Numerical issues can then be avoided */
         const float outer_scale = 0.68465F;
         const float inner_scale = -0.8072F;
         const float inner_shift = 0.6388F;
         if (0.0F <= sample_value)
         {
            const float exp_arg = sample_value * ocg::cmn::OCG_SQRT1_2 + inner_shift;
            cdf_value = 1.0F - outer_scale * expf(inner_scale * exp_arg * exp_arg);
         }
         else
         {
            const float exp_arg = -sample_value * ocg::cmn::OCG_SQRT1_2 + inner_shift;
            cdf_value = outer_scale * expf(inner_scale * exp_arg * exp_arg);
         }
      }
      else
      {
         // For low degrees of freedome we don't use the normal approximation.
         // However, use an approximation of the betainc function to speed up computation
         cdf_value = 1.0F - (0.5F * Students_T_Betainc_Approx(calib, sample_value, dof)); // cdf value given that the sample value is positive
         cdf_value = ((0.0F <= sample_value) ? cdf_value : (1.0F - cdf_value));
      }
      return cdf_value;
   }

   /*===========================================================================*\
   * FUNCTION: Compute_Forgetting_Factor()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const OCG_Calibrations_T& calib
   * const int32_t zone_idx
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function returns forgetting factor assigned to specific zone.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float Compute_Forgetting_Factor(
       const OCG_Calibrations_T &calib,
       const uint32_t zone_idx)
   {
      float forgetting_factor;

      // Compute starting range for zone
      const float range_starting_zone = GRID_MIN_X_DIST + static_cast<float>(zone_idx) * CELL_LENGTH;
      // Set different forgetting factors of the low pass filter depending on the range of the zone
      if (range_starting_zone < calib.underdrive_forgeting_factor_ranges_limits[2] )
      {
          forgetting_factor = calib.underdrive_forgeting_factor_short_ranges;
      }
      else if (range_starting_zone <= calib.underdrive_forgeting_factor_ranges_limits[1])
      {
          forgetting_factor = calib.underdrive_forgeting_factor_medium_ranges;
      }
      else if (range_starting_zone <= calib.underdrive_forgeting_factor_ranges_limits[0])
      {
          forgetting_factor = calib.underdrive_forgeting_factor_far_ranges;
      }
      else // range >range_limit[0]
      {
          forgetting_factor = calib.underdrive_forgeting_factor_furthest_ranges;
      }

      return forgetting_factor;
   }

   /*===========================================================================*\
   * FUNCTION: Compute_Prob_Of_Height_Hypothesis()
   *===========================================================================
   * RETURN VALUE:
   * float - probability of hypothesis
   *
   * PARAMETERS:
   * const OCG_Calibrations_T& calib,
   * const float(&innovation)[UD_HEIGHT_STATE_SIZE],
   * const float hypothesis_mean,
   * const float forgetting_factor,
   * float(&states)[UD_HEIGHT_STATE_SIZE],
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function calculate the probability of the null hypothesis that the difference between
   * the elev angle to the hypothetic obstacle and dets elev angle height comes from a distribution
   * with a hypothesis mean (in this case the hypothesis mean is zero, because the data is prior
   * shifted by subtracting expected elev angle form dets elev thus data is centered around zero).
   * Probability is calculated based on Student's T test of the null hypothesis.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float Compute_Prob_Of_Height_Hypothesis(
       const OCG_Calibrations_T &calib,
       const float (&innovation)[UD_HEIGHT_STATE_SIZE],
       const float hypothesis_mean,
       const float forgetting_factor,
       float (&states)[UD_HEIGHT_STATE_SIZE])
   {
      float &det_count = states[UD_HEIGHT_STATE_DET_COUNT];
      float &mean_elev = states[UD_HEIGHT_STATE_ELEVATION_ANGLE];
      float &mean_sq_elev = states[UD_HEIGHT_STATE_SQUARED_ELEVATION_ANGLE];

      det_count = det_count * forgetting_factor; // Forget a little bit about the old data

      if (innovation[UD_HEIGHT_STATE_DET_COUNT] > 0.0F)
      {
         const float norm_coef = 1.0F / (det_count + innovation[UD_HEIGHT_STATE_DET_COUNT]);
         mean_elev = norm_coef * (det_count * mean_elev + innovation[UD_HEIGHT_STATE_ELEVATION_ANGLE]);               // mean elevation angle
         mean_sq_elev = norm_coef * (det_count * mean_sq_elev + innovation[UD_HEIGHT_STATE_SQUARED_ELEVATION_ANGLE]); // mean squared elevation angle
         det_count = det_count + innovation[UD_HEIGHT_STATE_DET_COUNT];                                               // number of detections
      }

      // Compute test variable and prob_value
      float prob_value;
      const float dof = det_count - 1.0F;
      if (calib.underdrive_t_test_var_iter_min_num_dets < dof) // We have enough detections to compute sample_var
      {
         const float sample_var = det_count * (mean_sq_elev - mean_elev * mean_elev) / dof;
         if (sample_var < 0.0F) // Negative variance, not a valid case, must be positive (only seen happening for small dof due to numerical errors)
         {
            prob_value = -1.0F; // Output invalid value of probability
         }
         else
         {
            const float test_innovation = mean_elev - hypothesis_mean;           // deviation of the actual mean from the expected mean
            if (calib.underdrive_t_test_var_iter_min_sample_var_th < sample_var) // Compute test_var without risk of zero division
            {
               const float test_var = test_innovation * sqrtf(det_count / sample_var);
               prob_value = Students_T_CDF_Approx(calib, test_var, dof);
            }
            else // Data variance is very small, risk of division by zero when computing test_var. Set probability solely based on sign of test innovation
            {
               if (0.0F < test_innovation)
               {
                  /* test_innovation/sample_var is large and positive since test_innovation is positive
                   * and sample_var is close to zero but positive. This means prob_value is close to one.
                   * Approximate prob_value with 1. */
                  prob_value = ocg::cmn::OCG_MAX_PROBABILITY;
               }
               else
               {
                  /* test_innovation/sample_var is large and negative since test_innovation is negative
                   * and sample_var is close to zero but positive. This means prob_value is close to zero.
                   * Approximate prob_value with 0. */
                  prob_value = ocg::cmn::OCG_MIN_PROBABILITY;
               }
            }
         }
      }
      else // Too few degrees of freedome to compute sample_var
      {
         prob_value = -1.0F; // Output invalid value of probability
      }
      return prob_value;
   }

   /*===========================================================================*\
   * FUNCTION: Compute_Prob_Of_RCS_Slope_Hypothesis()
   *===========================================================================
   * RETURN VALUE:
   * float - probability of hypothesis
   *
   * PARAMETERS:
   * const OCG_Calibrations_T& calib
   * const float (&innovation)[UD_RCS_STATE_SIZE]
   * const float hypothesis_slope
   * const float forgetting_factor
   * float (&states)[UD_RCS_STATE_SIZE]
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function updates the states and performs Students t test based on updated tests.
   * Function calculate the probability of the null hypothesis that the measured
   * amplitudes match the linear model (with slope and offset) i.e. they are
   * distributed around the linear model and deviations from the model come from
   * a zero mean distribution.
   * Probability is calculated based on Student's T test of the null hypothesis.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float Compute_Prob_Of_RCS_Slope_Hypothesis(
       const OCG_Calibrations_T &calib,
       const float (&innovation)[UD_RCS_STATE_SIZE],
       const float hypothesis_slope,
       const float forgetting_factor,
       float (&states)[UD_RCS_STATE_SIZE])
   {
      /* innovation = [nr new measurements, sum of inputs generating new measurements,
       * sum of squared input generating new measurements, sum of new measurements,
       * sum of squared new measurements, sum of product between inputs generating new measurements and new measurements]
       * estimated_parameters = [slope, offset]. Model used is linear one
       * measurement = slope * input + offset, where input is range and measurement is amplitude
       */

      float &det_count = states[UD_RCS_STATE_DET_COUNT];
      float &mean_range = states[UD_RCS_STATE_RANGE];
      float &mean_range_sq = states[UD_RCS_STATE_SQUARED_RANGE];
      float &mean_rcs = states[UD_RCS_STATE_RCS];
      float &mean_rcs_sq = states[UD_RCS_STATE_SQUARED_RCS];
      float &mean_range_rcs_prod = states[UD_RCS_STATE_RANGE_RCS_PROD];

      det_count = det_count * forgetting_factor;

      if (0.0F < innovation[0])
      {
         const float norm_coef = 1.0F / (det_count + innovation[UD_RCS_STATE_DET_COUNT]);
         mean_range = norm_coef * (det_count * mean_range + innovation[UD_RCS_STATE_RANGE]);                            // mean mean_range i.e. input (as described above)
         mean_range_sq = norm_coef * (det_count * mean_range_sq + innovation[UD_RCS_STATE_SQUARED_RANGE]);              // mean squared mean_range
         mean_rcs = norm_coef * (det_count * mean_rcs + innovation[UD_RCS_STATE_RCS]);                                  // mean amplitude (RCS) i.e. measurement
         mean_rcs_sq = norm_coef * (det_count * mean_rcs_sq + innovation[UD_RCS_STATE_SQUARED_RCS]);                    // mean squared amplitude
         mean_range_rcs_prod = norm_coef * (det_count * mean_range_rcs_prod + innovation[UD_RCS_STATE_RANGE_RCS_PROD]); // mean product of mean_range and amplitude
         det_count = det_count + innovation[UD_RCS_STATE_DET_COUNT];                                                    // number of detections
      }

      float prob_value;
      const float non_normalized_input_sample_var = (mean_range_sq - mean_range * mean_range);
      if ((calib.underdrive_t_test_var_slope_min_num_dets < det_count) &&
          (calib.underdrive_t_test_var_slope_min_input_sample_var < (det_count / (det_count - 1.0F) * non_normalized_input_sample_var)))
      {
         // There are enough measurements from different ranges to do the test

         // Estimate parameters
         const float est_slope = (mean_range_rcs_prod - mean_range * mean_rcs) / non_normalized_input_sample_var;
         const float est_offset = mean_rcs - mean_range * est_slope;

         // Compute test variable and p-value
         const float dof = det_count - 2.0F; // Note should be N - 2, not N - 1, since 2 parameters have been estimated
         /* prediction is the deviation of estimated amplitude resulting from linear model from actual amplitude
          * est_ampl = (slope * mean_range + offset)   and thus pred = ampl - est_ampl
          * variance of prediction is then var(pred) = var(ampl - est_ampl) = var(ampl) + var(est_ampl) - 2 * cov(ampl, est_ampl)
          * where
          * var(ampl) = sum(ampl^2) / DOF
          * var(est_ampl) = var(slope * mean_range + offset) = sum(slope^2 * mean_range^2 + 2 * slope * mean_range * offset + offset^2) / DOF
          * cov(ampl, est_ampl) = E((ampl - E(ampl))(est_ampl - E(est_ampl)))
          * we assume that expected values of actual and estimated amplitudes are equal i.e. mean(ampl) == mean(est_ampl) so the formula is simplified to:
          * cov(ampl, est_ampl) = E(ampl * est_ampl) = sum(slope * ampl * mean_range + offset * ampl) / DOF, as ampl and est_ampl are independent???
          */

         const float measurement_sample_var = mean_rcs_sq;
         const float est_measurement_sample_var = est_slope * est_slope * mean_range_sq + 2.0F * est_slope * est_offset * mean_range + est_offset * est_offset;
         const float prediction_sample_cov = est_slope * mean_range_rcs_prod + est_offset * mean_rcs;
         const float prediction_sample_var = det_count / dof * (measurement_sample_var + est_measurement_sample_var - 2.0F * prediction_sample_cov); // unit = squared amplitude

         if (0.0F > prediction_sample_var) // Not a valid case, only seen happening for small number of measurements.
         {
            // Output invalid prob_value
            prob_value = -1.0F;
         }
         else
         {
            const float input_sample_var = det_count / (det_count - 1.0F) * non_normalized_input_sample_var;
            const float slope_sample_var = prediction_sample_var / ((det_count - 1.0F) * input_sample_var);
            const float test_innovation = est_slope - hypothesis_slope;
            if (calib.underdrive_t_test_var_slope_min_sample_var_th < slope_sample_var) // Compute test_var without risk of zero division
            {
               const float test_var = test_innovation / sqrtf(slope_sample_var);
               prob_value = Students_T_CDF_Approx(calib, test_var, dof);
            }
            else // Data variance is very small, risk of division with zero when computing test_var. Set probability solely based on sign of test innovation
            {
               if (0.0F < test_innovation)
               {
                  /* test_innovation/sample_var is large and negative since test_innovation is positive
                   * and sample_var is close to zero but positive. This means prob_value is close to one.
                   * Approximate prob_value with 1. */
                  prob_value = ocg::cmn::OCG_MAX_PROBABILITY;
               }
               else
               {
                  /* test_innovation/sample_var is large and negative since test_innovation is negative
                   * and sample_var is close to zero but positive. This means prob_value is close to zero.
                   * Approximate prob_value with 0. */
                  prob_value = ocg::cmn::OCG_MIN_PROBABILITY;
               }
            }
         }
      }
      else
      {
         // Not enough measurements from different ranges to do the test
         prob_value = -1.0F;
      }
      return prob_value;
   }

   /*===========================================================================*\
   * FUNCTION: Calc_Single_Zone_Probabilities()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * OCG_Underdrivability_Zones_T& zone
   * const OCG_Zones_Innovation_T& innovation
   * const OCG_Calibrations_T& calib
   * const uint32_t zone_idx
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function calculates probabilities of a single zone for all hypothesis.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Calc_Single_Zone_Probabilities(
       OCG_Single_Underdrivability_Zone_T &zone,
       const OCG_Zones_Innovation_T &innovation,
       const OCG_Calibrations_T &calib,
       const uint32_t zone_idx)
   {
      const float forgetting_factor = Compute_Forgetting_Factor(calib, zone_idx);
      const float height_hypothesis_zero_mean = 0.0F; // height hypothesis is zero because innovation is shifted (centered) to zero

      // Test underdrivability hypotheses:

      // can_pass test
      zone.p_height_can_pass = Compute_Prob_Of_Height_Hypothesis(calib, innovation.height_can_pass, height_hypothesis_zero_mean, forgetting_factor, zone.state_height_can_pass);
      zone.p_RCS_slope_can_pass = Compute_Prob_Of_RCS_Slope_Hypothesis(calib, innovation.RCS_slope_can_pass, calib.underdrive_hypothesis_slope_can_pass, forgetting_factor, zone.state_RCS_slope_can_pass);

      // is_likely_to_pass_test
      zone.p_height_is_likely_to_pass = Compute_Prob_Of_Height_Hypothesis(calib, innovation.height_is_likely_to_pass, height_hypothesis_zero_mean, forgetting_factor, zone.state_height_is_likely_to_pass);
      zone.p_RCS_slope_is_likely_to_pass = Compute_Prob_Of_RCS_Slope_Hypothesis(calib, innovation.RCS_slope_is_likely_to_pass, calib.underdrive_hypothesis_slope_is_likely_to_pass, forgetting_factor, zone.state_RCS_slope_is_likely_to_pass);

      // can_not_pass_upper_test
      zone.p_height_can_not_pass_upper = Compute_Prob_Of_Height_Hypothesis(calib, innovation.height_can_not_pass_upper, height_hypothesis_zero_mean, forgetting_factor, zone.state_height_can_not_pass_upper);
      if ((0.0F <= zone.p_height_can_not_pass_upper)) // If probability is negative then the test and probability is invalid and we don't want to compute the complemetary probability
      {
         zone.p_height_can_not_pass_upper = ocg::cmn::OCG_MAX_PROBABILITY - zone.p_height_can_not_pass_upper;
      }

      zone.p_RCS_slope_can_not_pass_upper = Compute_Prob_Of_RCS_Slope_Hypothesis(calib, innovation.RCS_slope_can_not_pass, calib.underdrive_hypothesis_slope_can_not_pass_upper, forgetting_factor, zone.state_RCS_slope_can_not_pass_upper);
      if ((0.0F <= zone.p_RCS_slope_can_not_pass_upper)) // If probability is negative then the test and probability is invalid and we don't want to compute the complemetary probability
      {
         // Compute complementary probability
         zone.p_RCS_slope_can_not_pass_upper = ocg::cmn::OCG_MAX_PROBABILITY - zone.p_RCS_slope_can_not_pass_upper;
      }

      // can_not_pass_lower_test
      zone.p_height_can_not_pass_lower = Compute_Prob_Of_Height_Hypothesis(calib, innovation.height_can_not_pass_lower, height_hypothesis_zero_mean, forgetting_factor, zone.state_height_can_not_pass_lower);
      zone.p_RCS_slope_can_not_pass_lower = Compute_Prob_Of_RCS_Slope_Hypothesis(calib, innovation.RCS_slope_can_not_pass, calib.underdrive_hypothesis_slope_can_not_pass_lower, forgetting_factor, zone.state_RCS_slope_can_not_pass_lower);
   }

   /*===========================================================================*\
   * FUNCTION: Assign_Single_Zone_Probabilities()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * OCG_Underdrivability_Zones& zones
   * const uint32_t zone_idx
   * const uint32_t circ_buff_zone_idx
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function acumulates calculated probabilities of a single zone.
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Assign_Single_Zone_Probabilities(
       OCG_Single_Underdrivability_Zone_T (&zones)[NUM_CELLS_X],
       const uint32_t zone_idx,
       const uint32_t circ_buff_zone_idx)
   {
      // Assign probability
      if (zones[circ_buff_zone_idx].p_height_can_pass < ocg::cmn::OCG_MIN_PROBABILITY) // Negative probability indicates invalid test and then we can not compute the total p_can_pass value
      {
         zones[zone_idx].p_can_pass = -1.0F;
      }
      else
      {
         zones[zone_idx].p_can_pass = zones[circ_buff_zone_idx].p_height_can_pass;
      }

      if ((zones[circ_buff_zone_idx].p_height_is_likely_to_pass < ocg::cmn::OCG_MIN_PROBABILITY) ||
          (zones[circ_buff_zone_idx].p_RCS_slope_is_likely_to_pass < ocg::cmn::OCG_MIN_PROBABILITY)) // Negative probability indicates invalid test and then we can not compute the total p_is_likely_to_pass value
      {
         zones[zone_idx].p_is_likely_to_pass = -1.0F;
      }
      else
      {
         zones[zone_idx].p_is_likely_to_pass = ocg::cmn::OCG_MAX_PROBABILITY - 
          (ocg::cmn::OCG_MAX_PROBABILITY - zones[circ_buff_zone_idx].p_height_is_likely_to_pass) * (ocg::cmn::OCG_MAX_PROBABILITY - zones[circ_buff_zone_idx].p_RCS_slope_is_likely_to_pass);
      }

      if ((zones[circ_buff_zone_idx].p_height_can_not_pass_lower < ocg::cmn::OCG_MIN_PROBABILITY) ||
          (zones[circ_buff_zone_idx].p_RCS_slope_can_not_pass_lower < ocg::cmn::OCG_MIN_PROBABILITY) ||
          (zones[circ_buff_zone_idx].p_height_can_not_pass_upper < ocg::cmn::OCG_MIN_PROBABILITY) ||
          (zones[circ_buff_zone_idx].p_RCS_slope_can_not_pass_upper < ocg::cmn::OCG_MIN_PROBABILITY)) // Negative probability indicates invalid test and then we can not compute the total p_can_not_pass value
      {
         zones[zone_idx].p_can_not_pass = -1.0F;
      }
      else
      {
         zones[zone_idx].p_can_not_pass = ocg::cmn::OCG_MAX_PROBABILITY -
             (ocg::cmn::OCG_MAX_PROBABILITY - zones[circ_buff_zone_idx].p_height_can_not_pass_lower * zones[circ_buff_zone_idx].p_height_can_not_pass_upper) *
             (ocg::cmn::OCG_MAX_PROBABILITY - zones[circ_buff_zone_idx].p_RCS_slope_can_not_pass_lower * zones[circ_buff_zone_idx].p_RCS_slope_can_not_pass_upper);
      }
   }
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
