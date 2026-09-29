/*===================================================================================*\
* FILE:  f360_update_object_orientation_uncertainty.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition of Update_Object_Orientation_Uncertainty() function.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
\*===================================================================================*/

/******************************
* Includes
*******************************/
#include "f360_update_object_orientation_uncertainty.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Update_Object_Orientation_Uncertainty
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Tracker_Info_T & tracker_info,
   * F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * The purpose of the function is to update object orientation's standard 
   * deviation.
   * --------------------------------------------------------------------------
   * constructor
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Update_Object_Orientation_Uncertainty(
      const F360_Tracker_Info_T & tracker_info,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS])
   {
      constexpr float32_t k_std_linear_step = F360_DEG2RAD(2.0F);

      for (int32_t obj_idx = 0; obj_idx < tracker_info.num_active_objs; obj_idx++)
      {
         F360_Object_Track_T& curr_obj = object_tracks[tracker_info.active_obj_ids[obj_idx] - 1];

         if(curr_obj.movable_prob < 0.5F)
         {
            curr_obj.orientation_std = F360_PI;
         }
         else if((curr_obj.movable_prob >= 0.5F) && (!curr_obj.f_moving))
         {
            if (F360_TRACKER_TRKFLTR_CTCA == curr_obj.trk_fltr_type)
            {
               curr_obj.orientation_std += k_std_linear_step;
               constexpr float32_t k_stopped_CTCA_max_orientation_std = F360_DEG2RAD(30.0F);
               curr_obj.orientation_std = std::min(curr_obj.orientation_std, k_stopped_CTCA_max_orientation_std);
            }
            else if (F360_TRACKER_TRKFLTR_CCA == curr_obj.trk_fltr_type)
            {
               curr_obj.orientation_std = F360_PI;
            }
            else
            {
               curr_obj.orientation_std = F360_PI;  // merely for misra; should not enter here
            }
         }
         else if(curr_obj.f_moving)
         {
            if (F360_TRACKER_TRKFLTR_CTCA == curr_obj.trk_fltr_type)
            {
               // check if the object's orientation becomes stable after the merge
               constexpr float32_t k_heading_rate_thres = F360_DEG2RAD(1.72F);
               if (curr_obj.f_prevent_orientation_std_decrease && (std::abs(curr_obj.heading_rate) < k_heading_rate_thres))
               {
                  curr_obj.f_prevent_orientation_std_decrease = false;
               }

               // orientation std shrinkage trigger conditions:
               const bool f_orientation_std_shrink = ((!curr_obj.f_prevent_orientation_std_decrease) && (F360_OBJECT_STATUS_UPDATED == curr_obj.status));

               //orientation std increase trigger conditions:
               const bool f_orientation_std_increase = (F360_OBJECT_STATUS_COASTED == curr_obj.status);

               if (f_orientation_std_shrink)
               {
                  curr_obj.orientation_std -= k_std_linear_step;
                  constexpr float32_t k_moving_CTCA_min_orientation_std = F360_DEG2RAD(3.0F);
                  curr_obj.orientation_std = std::max(curr_obj.orientation_std, k_moving_CTCA_min_orientation_std);
               }
               
               if (f_orientation_std_increase)
               {
                  curr_obj.orientation_std += k_std_linear_step;
                  constexpr float32_t k_moving_CTCA_max_orientation_std = F360_DEG2RAD(20.0F);
                  curr_obj.orientation_std = std::min(curr_obj.orientation_std, k_moving_CTCA_max_orientation_std);
               }
            }
            else if (F360_TRACKER_TRKFLTR_CCA == curr_obj.trk_fltr_type)
            {
               constexpr float32_t k_moving_CCA_min_orientation_std = F360_DEG2RAD(15.0F);
               curr_obj.orientation_std = std::max(k_moving_CCA_min_orientation_std, curr_obj.cca_pnt_filter_cov[0][0]);
            }
            else
            {
               curr_obj.orientation_std = F360_PI;  // merely for misra; should not enter here
            }
         }
         else
         {
            curr_obj.orientation_std = F360_PI;  // merely for misra; should not enter here
         }
      }
   }
}
