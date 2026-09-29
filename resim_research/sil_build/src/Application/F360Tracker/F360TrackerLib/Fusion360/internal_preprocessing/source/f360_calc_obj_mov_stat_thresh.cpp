/*===================================================================================*\
* FILE: f360_calc_obj_mov_stat_thresh.cpp
*====================================================================================
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*------------------------------------------------------------------------------------
* %full_filespec: AIT-69%
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains funcions which updates host properties with calculated value.
*
* ABBREVIATIONS:
*   NONE
*
* TRACEABILITY INFO:
*   Design Document(s): calcObjMovStatThresh.m
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/


/******************************
* Includes
*******************************/

#include "f360_math.h"
#include "f360_get_wall_time.h"
#include "f360_calc_obj_mov_stat_thresh.h"
namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Calc_Obj_Mov_Stat_Thresh()
   *===========================================================================
   * RETURN VALUE:
   * float32_t obj_mov_stat_speed_threshold
   *
   * PARAMETERS:
   * const F360_Host_T* const host -  Host properties structure
   * F360_TRKR_TIMING_INFO_T *timing_info
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
   * This function calculate the threshold as a function of host speed
   *
   * PRECONDITIONS:
   * Should point to valid Host  properties structure elements
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   float32_t Calc_Obj_Mov_Stat_Thresh(float32_t const host_vcs_speed,
      F360_TRKR_TIMING_INFO_T* const timing_info)
   {
      float32_t obj_mov_stat_speed_threshold;
      const float32_t max_host_speed_to_slow = 10.0F;
      const float32_t obj_mov_stat_speed_thresh_slow = 1.4F;
      const float32_t min_host_speed_to_fast = 15.0F;
      const float32_t obj_mov_stat_speed_thresh_fast = 1.5F;

      const float32_t start_time = get_wall_time();

      // calculate the threshold as a function of host speed
      if (host_vcs_speed < max_host_speed_to_slow) {
         obj_mov_stat_speed_threshold = obj_mov_stat_speed_thresh_slow;
      }
      else if (host_vcs_speed > min_host_speed_to_fast) {
         obj_mov_stat_speed_threshold = obj_mov_stat_speed_thresh_fast;
      }
      else {
         // ramp up the values a intermediate speeds
         const float32_t temp_1 = (obj_mov_stat_speed_thresh_fast - obj_mov_stat_speed_thresh_slow) / (min_host_speed_to_fast - max_host_speed_to_slow);
         const float32_t temp_0 = ((min_host_speed_to_fast * obj_mov_stat_speed_thresh_slow) - (max_host_speed_to_slow * obj_mov_stat_speed_thresh_fast))
            / (min_host_speed_to_fast - max_host_speed_to_slow);

         obj_mov_stat_speed_threshold = (host_vcs_speed  * temp_1) + temp_0;
      }

      timing_info->calc_obj_mov_stat_thresh = get_wall_time() - start_time;

      return (obj_mov_stat_speed_threshold);
   }
}
