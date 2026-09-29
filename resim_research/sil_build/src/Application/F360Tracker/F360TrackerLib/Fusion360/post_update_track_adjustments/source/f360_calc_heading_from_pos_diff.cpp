/*===========================================================================*\
* FILE: f360_calc_heading_from_pos_diff.cpp
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Calc_Heading_From_Pos_Diff()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_calc_heading_from_pos_diff.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Calc_Heading_From_Pos_Diff()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * F360_Object_Track_T &current_object
   * const F360_Calibrations_T& calibs
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
   * This function updates objects low pass filtered heading based solely
   * on center position change over time
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Calc_Heading_From_Pos_Diff(
      F360_Object_Track_T & object,
      const F360_Calibrations_T & calibs)
   {
      // CTCA objects with too low speed will have their pos_diff_heading frozen 
      if (((F360_TRACKER_TRKFLTR_CTCA == object.trk_fltr_type) || (F360_TRACKER_TRKFLTR_CCA == object.trk_fltr_type)) &&
         (object.speed > calibs.k_min_speed_for_updating_heading))
      {
         if (object.filtered_pos_diff_heading < INFTY)
         {
            // Transformation of previous position to new VCS position should have been taken care of already in time update module
            const float32_t delta_x = object.bbox.Get_Center().x - object.prev_vcs_center_pos.x;
            const float32_t delta_y = object.bbox.Get_Center().y - object.prev_vcs_center_pos.y;

            const float32_t raw_pos_head = F360_Atan2f(delta_y, delta_x);
            object.filtered_pos_diff_heading = F360_Low_Pass_Filter_Angle_First_Order(raw_pos_head, object.filtered_pos_diff_heading, calibs.k_pos_delta_heading_filter_constant);
         }
         else
         {
            // Object is either a new object or have switched from CCA type
            object.filtered_pos_diff_heading = object.vcs_heading.Value();
         }
      }
   }
}


