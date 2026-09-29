/*===================================================================================*\
* FILE: f360_overall_confidence.cpp
*====================================================================================
*Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* Contains function definitions for functions related to the overall confidence.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#include "f360_overall_confidence.h"
#include "f360_overall_confidence_helpers.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Overall_Confidence
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS] - reference to objects tracks struct
   * F360_Tracker_Info_T& tracker_info - reference to struct containing information about tracker
   * const F360_Calibrations_T& calib - reference to calibrations struct
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
   * Main function to be called to determine object overall confidence.
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   void Overall_Confidence(
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS], 
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list, 
      const F360_Tracker_Info_T& tracker_info, 
      const F360_Calibrations_T& calib)
   {
      for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
      {
         const int32_t obj_idx = tracker_info.active_obj_ids[i] - 1;

         const bool f_blocked = Update_Low_RCS_Dets_Counter_And_Block_Confidence(raw_detect_list, calib, object_tracks[obj_idx]);
         
         if ((F360_OBJECT_STATUS_UPDATED == object_tracks[obj_idx].status) && (!f_blocked))
         {
            Overall_Confidence_Update(object_tracks[obj_idx], calib);
         }
         else
         {
            Overall_Confidence_Decay(object_tracks[obj_idx], calib.k_conf_overall_timeout_to_start_decay);
         }

         if (F360_TRACKER_TRKFLTR_CTCA == object_tracks[obj_idx].trk_fltr_type)
         {
            if (Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To(object_tracks[obj_idx], CONF9_NONE))
            {
               object_tracks[obj_idx].conf_overall = CONF3_NONE;
            }
            else if (Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To(object_tracks[obj_idx], CONF9_LOW4))
            {
               object_tracks[obj_idx].conf_overall = CONF3_LOW;
            }
            else if (Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To(object_tracks[obj_idx], CONF9_MED4))
            {
               object_tracks[obj_idx].conf_overall = CONF3_MED;
            }
            else
            {
               object_tracks[obj_idx].conf_overall = CONF3_HIGH;
            }
         }
         else // CCA
         {
            if (Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To(object_tracks[obj_idx], CONF9_NONE))
            {
               object_tracks[obj_idx].conf_overall = CONF3_NONE;
            }
            else if (Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To(object_tracks[obj_idx], CONF9_LOW4))
            {
               object_tracks[obj_idx].conf_overall = CONF3_LOW;
            }
            else if (Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To(object_tracks[obj_idx], CONF9_MED4))
            {
               object_tracks[obj_idx].conf_overall = CONF3_MED;
            }
            else
            {
               object_tracks[obj_idx].conf_overall = CONF3_HIGH;
            }
         }
      }
   }
}
