/*===================================================================================*\
* FILE: f360_check_if_objects_can_overlap_significantly.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of function 
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "f360_check_if_objects_can_overlap_significantly.h"

namespace f360_variant_A
{
   /*=========================================================================
   * Method         Check_If_Objects_Can_Overlap_Significantly
   *
   * Description
   * Treat objects as circles with a simplification, diameter = length
   * If circles can intersect, then they can overlap 'significantly'.
   * 
   * This is in contrast to using the diagonal from object center to a corner.
   * Using the diagonal would be more correct but also more expensive and would 
   * not yield useful additional information if user is interested in significant
   * overlap.
   * 
   * Parameters
   * const Point& obj1_center_position_vcs,
   * const float32_t obj1_length,
   * const Point& obj2_center_position_vcs,
   * const float32_t obj2_length
   *
   * Returns        Bool - if 2 objects can overlap significantly.
   *
   * Externals:     None.
   *
   * Precondition   None.
   *
   * Postcondition  None.
   *
   * Note           None.
   *========================================================================*/
   bool Check_If_Objects_Can_Overlap_Significantly(
      const Point& obj1_center_position_vcs,
      const float32_t obj1_length,
      const Point& obj2_center_position_vcs,
      const float32_t obj2_length)
   {
      const float32_t obj1_half_length = obj1_length * 0.5F;
      const float32_t obj2_half_length = obj2_length * 0.5F;
      const float32_t max_dist_for_objects_to_possibly_overlap_significantly_sq = (obj1_half_length + obj2_half_length) * (obj1_half_length + obj2_half_length);

      const float32_t pos_diff_x = obj1_center_position_vcs.x - obj2_center_position_vcs.x;
      const float32_t pos_diff_y = obj1_center_position_vcs.y - obj2_center_position_vcs.y;

      const float32_t dist_between_obj_sq = (pos_diff_x * pos_diff_x) + (pos_diff_y * pos_diff_y);

      const bool f_can_overlap_significantly = dist_between_obj_sq < max_dist_for_objects_to_possibly_overlap_significantly_sq;

      return f_can_overlap_significantly;
   }
}
