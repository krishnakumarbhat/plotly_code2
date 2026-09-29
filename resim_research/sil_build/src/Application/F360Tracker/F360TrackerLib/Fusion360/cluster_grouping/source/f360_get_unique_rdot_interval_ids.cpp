/*===================================================================================*\
* FILE: f360_get_unique_rdot_interval_ids.cpp
*====================================================================================
*Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains fucnction to get unique rdot interval ids
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/


/******************************
* Includes
*******************************/
#include "f360_get_unique_rdot_interval_ids.h"
#include "f360_math_func.h"

/******************************
*   Function prototypes
*******************************/
namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Get_Unique_Rdot_Interval_Ids()
   *===========================================================================
   * RETURN VALUE:
   * void
   *
   * PARAMETERS:
   * const float32_t (&unique_rdot_interval_widths)[MAX_NUM_UNIQUE_RDOT_INTERVAL]
   * const int32_t ndets
   * const float32_t (&rdot_interval_width)[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER]
   * int32_t (&rdot_interval_ids)[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER]
   * int32_t (&unique_rdot_interval_ids)[MAX_NUM_UNIQUE_RDOT_INTERVAL]
   * int32_t & num_unique_rdot_intervals
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
   * This function get the unique rdot interval ids
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   void Get_Unique_Rdot_Interval_Ids(
      const float32_t (&unique_rdot_interval_widths)[MAX_NUM_UNIQUE_RDOT_INTERVAL],
      const int32_t num_unique_rdot_interval_widths,
      const int32_t ndets,
      const float32_t (&rdot_interval_width)[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER],
      int32_t (&rdot_interval_ids)[MAX_CURRENT_AND_HIST_DETS_IN_CLUSTER],
      int32_t (&unique_rdot_interval_ids)[MAX_NUM_UNIQUE_RDOT_INTERVAL],
      int32_t &num_unique_rdot_intervals)
   {
      bool rdot_interval_found[MAX_NUM_UNIQUE_RDOT_INTERVAL] = { };

      for (int32_t i = 0; i < ndets; i++)
      {
         for (int32_t j = 0; j < num_unique_rdot_interval_widths; j++)
         {
            const float32_t rdot_diff = rdot_interval_width[i] - unique_rdot_interval_widths[j];
            if (std::abs(rdot_diff) < (2.0F * F360_EPSILON))
            {
               rdot_interval_ids[i] = j + 1;
               rdot_interval_found[j] = true;
               break;
            }
         }
      }
      for (int32_t i = 0; i < num_unique_rdot_interval_widths; i++)
      {
         if (rdot_interval_found[i])
         {
            unique_rdot_interval_ids[num_unique_rdot_intervals] = i + 1;
            num_unique_rdot_intervals++;
         }
      }
   }
}
