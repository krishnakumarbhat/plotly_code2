/*===========================================================================*\
* FILE: f360_mark_azimuth_range_rate_outliers.cpp
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Mark_Azimuth_Range_Rate_Outliers()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_mark_azimuth_range_rate_outliers.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
   static float32_t Calc_Range_Rate_Diff_From_Linear_Vel_Profile(
      const F360_Object_Track_T& obj,
      const F360_Detection_Props_T& det_p,
      const rspp_variant_A::RSPP_Detection_T& det);

   /*===========================================================================*\
   * FUNCTION: Mark_Azimuth_Range_Rate_Outliers()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *  const F360_Object_Track_T& obj,
   *  const F360_Calibrations_T& calibs,
   *  const float32_t dist_to_rear_axle,
   *  const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
   *  F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]
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
   * This function flags detections that lie close to other detections in azimuth
   * but have range rates that differs from the objects linear velocity profile.
   * Detections like these can stem from e.g. wheels or swinging arms.
   * These detections may cause bad speed and/or heading/curvature estimates if
   * they are used in the measurement update.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Mark_Azimuth_Range_Rate_Outliers(
      const F360_Object_Track_T& obj,
      const F360_Calibrations_T& calibs,
      const float32_t dist_to_rear_axle,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      if ((obj.ndets > 1U) && (obj.f_moving))
      {
         const float32_t long_offset = -0.6F * dist_to_rear_axle; // Approximated host center
         const float32_t dist_sq = F360_Get_Hypotenuse_Squared(obj.vcs_position.x - long_offset, obj.vcs_position.y);

         if (dist_sq < calibs.k_az_rdot_max_sq_dist)
         {

            uint32_t obj_det_idx[MAX_DETS_IN_OBJ_TRK] = {};
            float32_t vcs_az_array[MAX_DETS_IN_OBJ_TRK] = {};
            for (uint32_t i = 0U; i < obj.ndets; i++)
            {
               const uint32_t det_idx = obj.detids[i] - 1U;

               obj_det_idx[i] = det_idx;
               vcs_az_array[i] = raw_detection_list.detections[det_idx].processed.vcs_az;
            }

            uint32_t az_sorted_idx[MAX_DETS_IN_OBJ_TRK];
            (void)F360_Sort(obj.ndets, true, vcs_az_array, az_sorted_idx);

            for (uint32_t i = 0U; i < (obj.ndets - 1U); i++)
            {
               uint32_t prev_det_idx = obj_det_idx[az_sorted_idx[i]];
               float32_t min_pred_diff = std::abs(Calc_Range_Rate_Diff_From_Linear_Vel_Profile(obj, det_props[prev_det_idx], raw_detection_list.detections[prev_det_idx]));
              
               for (uint32_t k = (i + 1U); k < obj.ndets; k++)
               {
                  const uint32_t current_det_idx = obj_det_idx[az_sorted_idx[k]];

                  if (det_props[current_det_idx].f_azimuth_rdot_outlier)
                  {
                     continue;
                  }

                  const float32_t delta_az = std::abs(raw_detection_list.detections[obj_det_idx[az_sorted_idx[i]]].processed.vcs_az - raw_detection_list.detections[obj_det_idx[az_sorted_idx[k]]].processed.vcs_az);

                  if (delta_az > calibs.k_az_rdot_max_az_diff)
                  {
                     break;
                  }
                  else
                  {
                     const float32_t current_pred_diff = std::abs(Calc_Range_Rate_Diff_From_Linear_Vel_Profile(obj, det_props[current_det_idx], raw_detection_list.detections[current_det_idx]));

                     const bool f_current_err_smaller = (current_pred_diff < min_pred_diff);

                     if (f_current_err_smaller && (min_pred_diff > calibs.k_az_rdot_min_rdot_diff))
                     {
                        // This detection fits better to predicted range rate, flag previous as outlier
                        det_props[prev_det_idx].f_azimuth_rdot_outlier = true;
                        prev_det_idx = current_det_idx;
                        min_pred_diff = current_pred_diff;
                     }
                     else if (current_pred_diff > calibs.k_az_rdot_min_rdot_diff)
                     {
                        // Previous detection fits better, flag current as outlier
                        det_props[current_det_idx].f_azimuth_rdot_outlier = true;
                     }
                     else
                     {
                        // Don't flag any detection, both are close to the predicted velocity profile
                     }
                  }
               }
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Calc_Range_Rate_Diff_From_Linear_Vel_Profile()
   * ===========================================================================
   * RETURN VALUE:
   * float32_t rdot_diff
   *
   * PARAMETERS:
   *   const F360_Object_Track_T& obj
   *   const F360_Detection_Props_T &det_p
   *   const rspp_variant_A::RSPP_Detection_T &det
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
   * This function returns the difference in range rate from the objects
   * linear velocity profile.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static float32_t Calc_Range_Rate_Diff_From_Linear_Vel_Profile(
      const F360_Object_Track_T& obj,
      const F360_Detection_Props_T& det_p,
      const rspp_variant_A::RSPP_Detection_T& det)
   {
      const float32_t rdot_diff = det_p.range_rate_compensated - obj.speed * (det.processed.vcs_az - obj.bbox.Get_Orientation()).Cos();

      return rdot_diff;
   }
}

