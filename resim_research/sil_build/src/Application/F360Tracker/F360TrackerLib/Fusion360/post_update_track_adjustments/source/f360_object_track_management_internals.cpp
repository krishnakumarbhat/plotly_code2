/*===================================================================================*\
* FILE:  f360_object_track_management_internals.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* N/A
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/

#include "f360_math.h"
#include "f360_object_track_management_internals.h"
#include <limits>

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Is_Enough_Valid_Dets_For_Update()
   *===========================================================================
   * RETURN VALUE:
   * bool flag indicating whether number of dets is high enough
   *
   * PARAMETERS:
   *   const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
   *   const F360_Object_Track_T &object_track
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
   * Checks if there are enough valid detections for object update
   * (not to much and not too little)
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Enough_Valid_Dets_For_Update(
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Object_Track_T &object_track
   )
   {
      //TODO: DFT-1288 Make enough valid dets logic more consistent for CTCA and CCA objects

      bool f_is_enough_valid_dets = false;

      if (F360_TRACKER_TRKFLTR_CTCA == object_track.trk_fltr_type)
      {
         // For CTCA tracks number of detections is considered enough if there are any associated detections including wheelspins
         f_is_enough_valid_dets = (object_track.ndets > 0U);
      }
      else
      {
         // For CCA tracks count number of detections selected by trk excluding wheelspins
         uint32_t valid_dets_counter = 0U;

         for (uint32_t det_from_obj_idx = 0U; det_from_obj_idx < object_track.ndets; det_from_obj_idx++)
         {
            const uint32_t det_idx = object_track.detids[det_from_obj_idx] - 1U;
            if (det_props[det_idx].f_rr_inlier &&
               (F360_DETECTION_WHEELSPIN_TYPE_INVALID != det_props[det_idx].wheel_spin_type))
            {
               valid_dets_counter++;
            }
         }

         f_is_enough_valid_dets = (object_track.num_rr_inlier_dets > 0U) && (valid_dets_counter < object_track.num_rr_inlier_dets);
      }

      return f_is_enough_valid_dets;
   }

   /*===========================================================================*\
   * FUNCTION: Change_Object_To_Updated_State()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *   F360_Object_Track_T &object_track
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
   * Makes all necessary changes that make object's state UPDATED
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Change_Object_To_Updated_State(
      F360_Object_Track_T &object_track
   )
   {
      if ((F360_OBJECT_STATUS_NEW_COASTED == object_track.status) ||
         (F360_OBJECT_STATUS_COASTED == object_track.status) ||
         (F360_OBJECT_STATUS_NEW == object_track.status))
      {
         // Track was previously coasted but now updated so reset the time_since_stage_start timer
         object_track.time_since_stage_start = 0.0F;
      }

      object_track.status = F360_OBJECT_STATUS_UPDATED;
      object_track.time_since_track_updated = 0.0F;
      object_track.f_ghost_NU_2_C = false;

      constexpr uint8_t uint8_max = 255U;
      if (object_track.num_updates_since_init < uint8_max)
      {
         object_track.num_updates_since_init++;
      }

   }

   /*===========================================================================*\
   * FUNCTION: Change_Object_To_Coasted_State()
   *===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   *   F360_Object_Track_T &object_track
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
   * Makes all necessary changes that make object's state COASTED
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Change_Object_To_Coasted_State(
      F360_Object_Track_T &object_track,
      const F360_Calibrations_T &calibs
   )
   {
      if ((F360_OBJECT_STATUS_NEW_UPDATED == object_track.status) || (F360_OBJECT_STATUS_UPDATED == object_track.status))
      {
         object_track.time_since_stage_start = 0.0F;
      }

      if ((F360_OBJECT_STATUS_NEW_UPDATED == object_track.status) &&
         (std::abs(object_track.vcs_position.x) < calibs.k_puta_max_vcs_xposn_for_ghost_NU_2_C) &&
         (std::abs(object_track.vcs_position.y) < calibs.k_puta_max_vcs_yposn_for_ghost_NU_2_C))
      {
         object_track.f_ghost_NU_2_C = true;
      }

      object_track.status = F360_OBJECT_STATUS_COASTED;
   }
}
