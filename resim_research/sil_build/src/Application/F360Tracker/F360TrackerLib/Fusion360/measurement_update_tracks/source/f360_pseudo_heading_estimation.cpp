/*===========================================================================*\
* FILE: f360_pseudo_heading_estimation.cpp
*============================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Pseudo_Heading_Estimation().
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#include "f360_pseudo_heading_estimation.h"
#include "f360_math.h"
#include "f360_norm_heading_angle.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Pseudo_Heading_Estimation()
   * ===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]    - Structure with detetcuion information
   * F360_Object_Track_T & obj                                              - Object to perform pseudo position estimation for
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
   * This function estimates a pseudo heading for an object based on the detection position trail
   * of current and previously associated detections.
   *
   * The function fits a straight line to the detection position trail. By minimizing the weighted
   * sum of the squared distances from detections to the line. The estimation is done according to
   * below explanation:
   *
   * The line to be fitted is represented as a point [px, py]
   * and a unit vector [cos(pseudo_heading); sin(pseudo_heading)]
   *
   *    line = [px; py] + t * [cos(pseudo_heading); sin(pseudo_heading)]    (where t is some real number).
   *
   * The normal to the line is
   *
   *    line normal = [-sin(pseudo_heading); cos(pseudo_heading)]
   *
   * The position of detection i is denoted [x_i; y_i]. The distance from this detection
   * to the line is given by the projection of the vector [x_i; y_i] - [px; py] onto the line normal:
   *
   *    distance from detection i to the line = scalar_product([x_i; y_i] - [px; py], [-sin(pseudo_heading); cos(pseudo_heading)])
   *
   * The estimation problem is resolved by minimizing the weighted sum of the squared distances
   * from detections to the line. I.e. the follwing cost function is minimized:
   *
   *    cost function = sum over all detections i [ weight_i * "distance from detection i to the line"^2  ] =
   *                  = sum over all detections i [ weight_i * scalar_product([x_i; y_i] - [px; py], [-sin(pseudo_heading); cos(pseudo_heading)])^2]
   *
   * The problem is resolved iteratively where the necessary detection statitics are saved in a
   * state vector rather than saving all historical detections. The state vector used has six states:
   *    1: sum over all detections i [weight_i]
   *    2: sum over all detections i [weight_i * x_i]
   *    3: sum over all detections i [weight_i * y_i]
   *    4: sum over all detections i [weight_i * x_i^2]
   *    5: sum over all detections i [weight_i * y_i^2]
   *    6: sum over all detections i [weight_i * x_i * y_i]
   *
   * The weight is implemented as a forgetting factor which is multiplied to the state vector each
   * tracker iteration (i.e. the older the the detection is the smaller the corresponding weight will be).
   *
   * A full analytical solution to the optimization problem has not been found. The analytical solution
   * to px and py is known given a pseudo heading estimate but the pseudo heading estimate is here obtained
   * through grid search where the cost function value is computed and the solution which gives the smallest
   * cost is choosen.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * The obtained pseudo heading is normalized to be inside of the interval [-pi, pi]
   *
   \*===========================================================================*/

   void Pseudo_Heading_Estimation(
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_Object_Track_T& obj)
   {
      // Forget previous pseudo heading states a little
      const float32_t forgetting_factor = obj.f_moving ? F360_Powf(0.5F, 0.05F * obj.speed / 3.0F) : 0.93F;
      obj.pseudo_hdg_state_vec[0] *= forgetting_factor;
      obj.pseudo_hdg_state_vec[1] *= forgetting_factor;
      obj.pseudo_hdg_state_vec[2] *= forgetting_factor;
      obj.pseudo_hdg_state_vec[3] *= forgetting_factor;
      obj.pseudo_hdg_state_vec[4] *= forgetting_factor;
      obj.pseudo_hdg_state_vec[5] *= forgetting_factor;

      // Update pseudo heading state vector with new information from new associated detections
      for (uint32_t i = 0U; i < obj.ndets; i++)
      {
         const uint32_t det_idx = obj.detids[i] - 1U;
         obj.pseudo_hdg_state_vec[0] += 1.0F;
         obj.pseudo_hdg_state_vec[1] += det_props[det_idx].vcs_position.x;
         obj.pseudo_hdg_state_vec[2] += det_props[det_idx].vcs_position.y;
         obj.pseudo_hdg_state_vec[3] += det_props[det_idx].vcs_position.x * det_props[det_idx].vcs_position.x;
         obj.pseudo_hdg_state_vec[4] += det_props[det_idx].vcs_position.y * det_props[det_idx].vcs_position.y;
         obj.pseudo_hdg_state_vec[5] += det_props[det_idx].vcs_position.x * det_props[det_idx].vcs_position.y;
      }

      // Compute the pseudo heading
      if (obj.f_moving && (obj.trk_fltr_type == F360_TRACKER_TRKFLTR_CCA)) // Only need to compute the psuedo heading for moving cca objects despite state vector being updated for all objects
      {
         float32_t best_angle = obj.vcs_heading.Value();
         float32_t best_cost_function_value = INFTY;
         for (uint32_t i = 0U; i < 180U; i++) // Grid search over possible pseudo heading angle hypotheses
         {
            // Compute current angle to iterate over
            const float32_t pseudo_angle = F360_DEG2RAD(-90.0F) + F360_DEG2RAD(1.0F) * static_cast<float32_t>(i) + obj.vcs_heading.Value();
            const float32_t cos_angle = F360_Cosf(pseudo_angle);
            const float32_t sin_angle = F360_Sinf(pseudo_angle);

            /* Division by zero protection.
             * If line is close to y = constant (i.e.angle is close to 0 or +-180 degrees and sin(angle) = 0)
             * then we have to do things differently compared to if
             * line is close to x = constant (i.e.angle is close to +-90 degrees and cos(angle) = 0) */
            const bool f_crossing = (std::abs(pseudo_angle) > F360_PI * 0.25F) && (std::abs(pseudo_angle) < 3.0F * F360_PI * 0.25F);
            float32_t px;
            float32_t py;
            if (f_crossing)
            {
               // Analytical solution for px and py given the current pseudo heading hypothesis
               py = obj.vcs_position.y;
               px = (obj.pseudo_hdg_state_vec[1] * sin_angle * sin_angle - cos_angle * sin_angle * (obj.pseudo_hdg_state_vec[2] - obj.pseudo_hdg_state_vec[0] * py)) / (obj.pseudo_hdg_state_vec[0] * sin_angle * sin_angle);
            }
            else
            {
               // Analytical solution for px and py given the current pseudo heading hypothesis
               px = obj.vcs_position.x;
               py = (obj.pseudo_hdg_state_vec[2] * cos_angle * cos_angle - cos_angle * sin_angle * (obj.pseudo_hdg_state_vec[1] - obj.pseudo_hdg_state_vec[0] * px)) / (obj.pseudo_hdg_state_vec[0] * cos_angle * cos_angle);
            }

            // Compute cost function value given current px, py and pseudo heading
            const float32_t cost_function_value = sin_angle * sin_angle * (obj.pseudo_hdg_state_vec[3] - 2.0F * obj.pseudo_hdg_state_vec[1] * px + obj.pseudo_hdg_state_vec[0] * px * px) +
               cos_angle * cos_angle * (obj.pseudo_hdg_state_vec[4] - 2.0F * obj.pseudo_hdg_state_vec[2] * py + obj.pseudo_hdg_state_vec[0] * py * py) -
               2.0F * cos_angle * sin_angle * (obj.pseudo_hdg_state_vec[5] - obj.pseudo_hdg_state_vec[2] * px - obj.pseudo_hdg_state_vec[1] * py + obj.pseudo_hdg_state_vec[0] * px * py);

            // If current cost function vaue is smaller than any previous then save the current solution as the best one (and continue to iterate over remaining angles to see if we can find any one even better)
            if (cost_function_value < best_cost_function_value)
            {
               best_cost_function_value = cost_function_value;
               best_angle = pseudo_angle;
            }
         }
         obj.pseudo_hdg = Normalize_Heading_Angle(best_angle, 0.0F);
      }
      else
      {
         obj.pseudo_hdg = 0.0F;
      }
   }
}
