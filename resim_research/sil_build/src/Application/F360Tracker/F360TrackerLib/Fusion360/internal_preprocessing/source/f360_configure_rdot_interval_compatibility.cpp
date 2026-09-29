/*===========================================================================*\
* FILE: f360_configure_rdot_interval_compatability.cpp
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains definitions of functions that configure rdot interval compatibility.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_configure_rdot_interval_compatibility.h"
#include "f360_math_func.h"
#include "f360_get_wall_time.h"

namespace f360_variant_A
{
   static bool Check_Rdot_Interval_Compatability_Status(
      const float32_t unique_rdot_interval_widths_index,
      const float32_t unique_rdot_interval_widths_index_1, 
      const float32_t max_range_of_rdot_comp, 
      const float32_t rdot_dealiasing_gate);

   /*===========================================================================*\
   * FUNCTION: Configure_Rdot_Interval_Compatibility
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS]
   * const F360_Globals_T &globals
   * const F360_Calibrations_T &calibs
   * F360_Tracker_Info_T &tracker_info
   * F360_TRKR_TIMING_INFO_T &timing_info
   *
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
   * Function configures rdot interval compatibility.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Configure_Rdot_Interval_Compatibility(
      const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Globals_T &globals,
      const F360_Calibrations_T &calibs,
      F360_Tracker_Info_T &tracker_info,
      F360_TRKR_TIMING_INFO_T &timing_info)
   {
      tracker_info.num_unique_rdot_interval_widths = 0;

      const float32_t start_time = get_wall_time();

      for (uint8_t index = 0U; index < MAX_NUMBER_OF_SENSORS; index++)
      {
         if (!(sensors[index].variable.is_valid))
         {
            continue;
         }

         for (uint8_t index_1 = 0U; index_1 < 4U; index_1++)
         {
            const float32_t interval_width = sensors[index].constant.v_wrapping[index_1];
            if (tracker_info.num_unique_rdot_interval_widths > 0)
            {
               bool unique_rdot_interval_widths_data[MAX_NUM_UNIQUE_RDOT_INTERVAL] = {};
               for (int32_t index_2 = 0; index_2 < tracker_info.num_unique_rdot_interval_widths; index_2++)
               {
                  const float32_t interval_diff_threshold = 1e-3F;
                  const bool condition = std::abs(tracker_info.unique_rdot_interval_widths[index_2] - interval_width) < interval_diff_threshold;
                  unique_rdot_interval_widths_data[index_2] = condition ? true : false;
               }
               if (!F360_Any(unique_rdot_interval_widths_data, 4U))
               {
                  tracker_info.unique_rdot_interval_widths[tracker_info.num_unique_rdot_interval_widths] = interval_width;
                  tracker_info.num_unique_rdot_interval_widths++;
               }
            }
            else
            {
               tracker_info.unique_rdot_interval_widths[0] = interval_width;
               tracker_info.num_unique_rdot_interval_widths = 1;
            }
         }
      }

      for (int32_t index = 0; index < tracker_info.num_unique_rdot_interval_widths; index++)
      {
         const float32_t max_range_of_rdot_comp = 2.0F * globals.max_otg_speed;
         tracker_info.rdot_interval_compatibility[index][index] =
            (tracker_info.unique_rdot_interval_widths[index] >= max_range_of_rdot_comp);

         for (int32_t index_1 = index + 1; index_1 < tracker_info.num_unique_rdot_interval_widths; index_1++)
         {
            const bool f_is_compatibile = Check_Rdot_Interval_Compatability_Status(
               tracker_info.unique_rdot_interval_widths[index],
               tracker_info.unique_rdot_interval_widths[index_1],
               max_range_of_rdot_comp, calibs.rdot_interval_compatability_dealiasing_gate);
            tracker_info.rdot_interval_compatibility[index][index_1] = f_is_compatibile;
            tracker_info.rdot_interval_compatibility[index_1][index] = f_is_compatibile;
         }
      }

      timing_info.configure_rdot_interval_compability = get_wall_time() - start_time;
   }

   /*===========================================================================*\
     * FUNCTION: Check_Rdot_Interval_Compatability_Status
     *===========================================================================
     * RETURN VALUE:
     * None
     *
     * PARAMETERS:
     * const float32_t unique_rdot_interval_widths_index,
     * const float32_t unique_rdot_interval_widths_index_1,
     * const float32_t max_range_of_rdot_comp
     * const float32_t rdot_dealiasing_gate
     *
     * --------------------------------------------------------------------------
     * ABSTRACT:
     * --------------------------------------------------------------------------
     * Checks the compatability status of rdot witin a given inteval
     *
     * PRECONDITIONS:
     * Non zero unique_rdot_interval_widths_index are sent to the function
     *
     * POSTCONDITIONS:
     * None
     *
     \*===========================================================================*/
   static bool Check_Rdot_Interval_Compatability_Status(
      const float32_t unique_rdot_interval_widths_index,
      const float32_t unique_rdot_interval_widths_index_1,
      const float32_t max_range_of_rdot_comp,
      const float32_t rdot_dealiasing_gate)
   {
      bool f_success = true;

      if ((unique_rdot_interval_widths_index < F360_EPSILON) || (unique_rdot_interval_widths_index_1 < F360_EPSILON))
      {
         f_success = false;
      }
      else 
      {
         const int32_t max_interval_1 = static_cast<int32_t>(F360_Floorf(max_range_of_rdot_comp / unique_rdot_interval_widths_index));
         const int32_t max_interval_2 = static_cast<int32_t>(F360_Floorf(max_range_of_rdot_comp / unique_rdot_interval_widths_index_1));

         for (int32_t index_2 = -max_interval_1; f_success && (index_2 < max_interval_1); index_2++)
         {
            for (int32_t index_3 = -max_interval_2; f_success && (index_3 < max_interval_2); index_3++)
            {
               f_success = !(((index_2 != 0) || (index_3 != 0)) &&
                  (std::abs((static_cast<float32_t>(index_2) * unique_rdot_interval_widths_index) -
                     (static_cast<float32_t>(index_3) * unique_rdot_interval_widths_index_1)) < rdot_dealiasing_gate));
            }
         }
      }
      return f_success;
   }
}
