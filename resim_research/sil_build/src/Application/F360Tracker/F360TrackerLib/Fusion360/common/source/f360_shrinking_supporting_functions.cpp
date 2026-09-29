/******************************************************************************
* Copyright 2025 Aptiv, All Rights Reserved.
* Aptiv Confidential
******************************************************************************/
/*===================================================================================*\
* FILE: f360_shrinking_supporting_functions.cpp
*====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* 
* 
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[September 06, 2020]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
**************************************************************************************/

#include "f360_shrinking_supporting_functions.h"
#include "f360_calculate_curvi_position.h"

namespace f360_variant_A
{
   /*===========================================================================*\
 * FUNCTION: Get_CIPV_Long_Pos
 *===========================================================================
 * RETURN VALUE:
 * Bool
 *
 * PARAMETERS:
 *  const F360_Tracker_Info_T& tracker_info,
 *  const F360_Host_T& host,
 *  const F360_Calibrations_T& calib
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
 * Function to get CIPV's longitudinal position if captured.
 *
 * PRECONDITIONS:
 * None
 *
 * POSTCONDITIONS:
 * None
 *
 \*===========================================================================*/
 float32_t Get_CIPV_Long_Pos(
    const F360_Tracker_Info_T& tracker_info,
    const F360_Host_T& host,
    const F360_Calibrations_T& calib)
 {
    F360_Object_Track_T* curr_trk = tracker_info.vcslong_sorted_first_infront_of_host;
    // CIPV longitudinal position if being found
    float32_t cipv_long_pos = 0.0F;
    if (NULL != curr_trk)
    {
       for (int32_t obj_idx = 0; obj_idx < tracker_info.num_active_objs; obj_idx++)
       {
          constexpr float32_t curvi_lat_pos_thres = 1.8F;
          bool f_cipv_found = false;
          const float32_t y_curvi_lat_pos = Calculate_Curvi_Lat_Pos(host, curr_trk->vcs_position.x, curr_trk->vcs_position.y);
          if ((curr_trk->movable_prob > 0.5F) && (std::abs(y_curvi_lat_pos) < curvi_lat_pos_thres))
          {
             cipv_long_pos = curr_trk->vcs_position.x;
             f_cipv_found = true;
          }

          curr_trk = tracker_info.vcslong_sorted_next_track[curr_trk->id - 1];

          if (f_cipv_found
             || (NULL == curr_trk)
             || (curr_trk->vcs_position.x > calib.k_object_motion_queue_zone_long_dist))
          {
             break;
          }
       }
    }
    return cipv_long_pos;
 }

 /*===========================================================================*\
 * FUNCTION: Determine_CIPV_Status
 *=========================================================================== 
 * RETURN VALUE:
 * bool 
 *
 * PARAMETERS:
 * const F360_Object_Track_T& object 
 * float32_t cipv_long_pos           
 * const F360_Calibrations_T& calib  
 *
 * EXTERNAL REFERENCES:
 * F360_PI
 *
 * DEVIATIONS FROM STANDARDS:
 * None.
 *
 * ABSTRACT:
 * Encapsulates logic to decide whether an object lies inside a symmetric
 * longitudinal window around the current CIPV (if one exists) and is not
 * oriented almost perpendicular to host direction.
 *
 * PRECONDITIONS:
 * cipv_long_pos already derived (e.g. by Get_CIPV_Long_Pos()).
 *
 * POSTCONDITIONS:
 * None.
 *
 \*===========================================================================*/
 bool Determine_CIPV_Status(
     const F360_Object_Track_T& object,
     const float32_t cipv_long_pos,
     const F360_Calibrations_T& calib)
 {
    const float32_t orientation_abs = std::abs(object.bbox.Get_Orientation().Value());
    const bool f_orient_susp =
       (calib.k_object_motion_max_abs_orient_diff_to_host < orientation_abs) &&
       (orientation_abs < (F360_PI - calib.k_object_motion_max_abs_orient_diff_to_host));

    bool f_is_in_cipv_zone = false;
    if (cipv_long_pos > 0.0F)
    {
       constexpr float32_t buffer = 2.0F;
       const float32_t long_pos_thres = cipv_long_pos + buffer;
       if ((-long_pos_thres < object.vcs_position.x) && (object.vcs_position.x < long_pos_thres))
       {
          f_is_in_cipv_zone = true;
       }
    }
    return (f_is_in_cipv_zone) && (!f_orient_susp);
 }
 /*===========================================================================*\
  * FUNCTION: In_Special_Zone_For_No_Shrinking()
  *===========================================================================
  * RETURN VALUE:
  * None
  *
  * PARAMETERS:
  * const Point& obj_vcs_pos,
  * const F360_Tracker_Variant_T& variant_type
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
  * Functions determines whether the object is in special zones defined for specific variants where shrinking does not apply.
  *
  * PRECONDITIONS:
  * None
  *
  * POSTCONDITIONS:
  * None
  *
  \*===========================================================================*/
 bool In_Special_Zone_For_No_Shrinking(
     const Point& obj_vcs_pos,
     const F360_Tracker_Variant_T& tracker_variant_type)
 {
     bool f_in_zone = false;
     switch (tracker_variant_type)
     {
     case F360_VARIANT_TYPE_K:
     {
         // Variant K zone boundaries
         const float32_t variantK_zone1_min_x = -40.0F;
         const float32_t variantK_zone1_max_x = 25.0F;
         const float32_t variantK_zone1_max_abs_y = 22.0F;

         const float32_t variantK_zone2_min_x = -100.0F;
         const float32_t variantK_zone2_max_x = 0.0F;
         const float32_t variantK_zone2_max_abs_y = 10.0F;

         const bool f_in_zone_1 =
             (obj_vcs_pos.x < variantK_zone1_max_x) &&
             (obj_vcs_pos.x > variantK_zone1_min_x) &&
             (std::abs(obj_vcs_pos.y) < variantK_zone1_max_abs_y);

         const bool f_in_zone_2 =
             (obj_vcs_pos.x < variantK_zone2_max_x) &&
             (obj_vcs_pos.x > variantK_zone2_min_x) &&
             (std::abs(obj_vcs_pos.y) < variantK_zone2_max_abs_y);

         f_in_zone = (f_in_zone_1) || (f_in_zone_2);
         break;
     }
     default:
     {
         f_in_zone = false;
         break;
     }
     }

     return f_in_zone;
 }
}
