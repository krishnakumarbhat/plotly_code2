/*===================================================================================*\
* FILE: f360_is_two_look_type_ok_combine.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*   ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*   ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#include "f360_math_func.h"
#include "f360_is_two_look_type_ok_combine.h"
#include "f360_calibrations.h"
#include "f360_constants.h"

namespace f360_variant_A
{
   /*===========================================================================*\
     * FUNCTION: Is_Two_Look_Type_Ok_Combine_For_Zero_Intervals
     *===========================================================================
     * RETURN VALUE:
     * None
     *
     * PARAMETERS:
     *
     * const F360_Calibrations_T &calib,
     * const float32_t &rdot_diff,
     * bool &f_success)
     * 
     * DEVIATIONS FROM STANDARDS:
     * None.
     *
     * --------------------------------------------------------------------------
     * ABSTRACT:
     * --------------------------------------------------------------------------
     * Checking if range rate difference is below certain threshold
     *
     * PRECONDITIONS:
     * None
     *
     * POSTCONDITIONS:
     * None
     *
   \*===========================================================================*/
   void Is_Two_Look_Type_Ok_Combine_For_Zero_Intervals(
      const F360_Calibrations_T &calib,
      const float32_t &rdot_diff,
      bool &f_success)
   {
      if (std::abs(rdot_diff) < calib.k_rdot_half_gate)
      {
         f_success = true;
      }
   }

   /*===========================================================================*\
     * FUNCTION: Is_Two_Look_Type_Ok_Combine_With_One_Cluster_Dealiased
     *===========================================================================
     * RETURN VALUE:
     * None
     *
     * PARAMETERS:
     *
     * const F360_Globals_T& globals,
     * const F360_Calibrations_T& calib,
     * const float32_t rdot_dealiased,
     * const float32_t rdot_2,
     * const float32_t speed_interval,
     * bool &f_success,
     * float32_t &index)
     *
     * DEVIATIONS FROM STANDARDS:
     * None.
     *
     * --------------------------------------------------------------------------
     * ABSTRACT:
     * --------------------------------------------------------------------------
     * Checking range rate diff combined with intertals for dealiasing
     *
     * PRECONDITIONS:
     * None
     *
     * POSTCONDITIONS:
     * None
     *
   \*===========================================================================*/
   void Is_Two_Look_Type_Ok_Combine_With_One_Cluster_Dealiased(
      const F360_Globals_T& globals,
      const F360_Calibrations_T& calib,
      const float32_t rdot_dealiased,
      const float32_t rdot_2,
      const float32_t speed_interval,
      bool &f_success,
      float32_t &index)
   {
      f_success = false;
      index = 0.0F;

      if (speed_interval < (globals.max_otg_speed * 2.0F))
      {
         const float32_t rdot_diff = rdot_dealiased - rdot_2;

         Is_Two_Look_Type_Ok_Combine_For_Zero_Intervals(calib, rdot_diff, f_success);

         if (!f_success)
         {            
            const float32_t one_over_interval = 1.0F / speed_interval;

            const int32_t max_interval = static_cast<int32_t>(F360_Floorf((globals.max_otg_speed + calib.k_rdot_half_gate - rdot_2) * one_over_interval));
            const int32_t min_interval = static_cast<int32_t>(F360_Ceilf((-globals.max_otg_speed - calib.k_rdot_half_gate - rdot_2) * one_over_interval));

            for (int32_t interval_val = min_interval; interval_val <= max_interval; interval_val++)
            {
               if (!(interval_val == 0))
               {
                  const float32_t interval_ind = static_cast<float32_t>(interval_val);
                  if (std::abs(rdot_diff - interval_ind * speed_interval) < calib.k_rdot_half_gate)
                  {
                     index = interval_ind;
                     f_success = true;
                     break;
                  }
               }
            }
         }
      }
   }

   /*===========================================================================*\
     * FUNCTION: Is_Two_Look_Type_Ok_Combine_With_None_Cluster_Dealiased
     *===========================================================================
     * RETURN VALUE:
     * None
     *
     * PARAMETERS:
     *
     * const F360_Globals_T& globals,
     * const F360_Calibrations_T& calib,
     * const float32_t rdot_1,
     * const float32_t rdot_2,
     * const float32_t interval_1,
     * const float32_t interval_2,
     * bool &f_success,
     * float32_t &index_1,
     * float32_t &index_2)
     *
     * DEVIATIONS FROM STANDARDS:
     * None.
     *
     * --------------------------------------------------------------------------
     * ABSTRACT:
     * --------------------------------------------------------------------------
     * Checking range rate diff combined with intertals for dealiasing
     *
     * PRECONDITIONS:
     * None
     *
     * POSTCONDITIONS:
     * None
     *
   \*===========================================================================*/
   void Is_Two_Look_Type_Ok_Combine_With_None_Cluster_Dealiased(
      const F360_Globals_T& globals,
      const F360_Calibrations_T& calib,
      const float32_t rdot_1,
      const float32_t rdot_2,
      const float32_t interval_1,
      const float32_t interval_2,
      bool &f_success,
      float32_t &index_1,
      float32_t &index_2)
   {
      f_success = false;
      index_1 = 0.0F;
      index_2 = 0.0F;

      if (!((fabsf(interval_1 - interval_2) < F360_EPSILON) && (interval_1 < (globals.max_otg_speed * 2.0F))))
      {
         const float32_t rdot_diff = rdot_1 - rdot_2;
         float32_t current_min_diff = calib.k_rdot_half_gate;

         const float32_t one_over_interval_1 = 1.0F / interval_1;
         const float32_t one_over_interval_2 = 1.0F / interval_2;

         const int32_t max_interval_1 = static_cast<int32_t>(F360_Floorf((globals.max_otg_speed + calib.k_rdot_half_gate - rdot_1) * one_over_interval_1));
         const int32_t min_interval_1 = static_cast<int32_t>(F360_Ceilf((-globals.max_otg_speed - calib.k_rdot_half_gate - rdot_1) * one_over_interval_1));

         const int32_t max_interval_2 = static_cast<int32_t>(F360_Floorf((globals.max_otg_speed + calib.k_rdot_half_gate - rdot_2) * one_over_interval_2));
         const int32_t min_interval_2 = static_cast<int32_t>(F360_Ceilf((-globals.max_otg_speed - calib.k_rdot_half_gate - rdot_2) * one_over_interval_2));

         for (int32_t interval_val1 = min_interval_1; interval_val1 <= max_interval_1; interval_val1++)
         {
            for (int32_t interval_val2 = min_interval_2; interval_val2 <= max_interval_2; interval_val2++)
            {
                const float32_t interval1_ind = static_cast<float32_t>(interval_val1);
                const float32_t interval2_ind = static_cast<float32_t>(interval_val2);
                const float32_t rdot_diff_interval_shift = std::abs(rdot_diff + (interval1_ind * interval_1) - (interval2_ind * interval_2));
                // choose the aliasing intervals that give the smallest range-rate difference
                if (rdot_diff_interval_shift < current_min_diff)
                {
                    index_1 = interval1_ind;
                    index_2 = interval2_ind;
                    current_min_diff = rdot_diff_interval_shift;
                    f_success = true;
                }
            }
         }
      }
   }
}
