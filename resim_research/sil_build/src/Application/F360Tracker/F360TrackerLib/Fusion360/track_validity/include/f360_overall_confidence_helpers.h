/*===================================================================================*\
* FILE: f360_overall_confidence_helpers.h
*====================================================================================
*Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential - Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* Contains function declarations for functions related to the overall confidence helpers.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef F360_OVERALL_CONFIDENCE_HELPERS_H
#define F360_OVERALL_CONFIDENCE_HELPERS_H

#include "f360_object_track.h"
#include "f360_tracker_info.h"
#include "f360_calibrations.h"
#include "rspp_detection_list.h"

namespace f360_variant_A
{
   void Overall_Confidence_Update(
      F360_Object_Track_T& object_track, 
      const F360_Calibrations_T& calib);

   void Overall_Confidence_Decay(
      F360_Object_Track_T& object_track, 
      const float32_t timeout_to_start_decay_th);

   void Confidence_State_Machine(
      CONF9_T& current_conf, 
      const float32_t difference, 
      const float32_t thresh_easy, 
      const float32_t thresh_medium, 
      const float32_t thresh_hard,
      const float32_t thresh_very_hard);

   bool Is_CCA_Obj_Confidence_Less_Than_Or_Equal_To(
      const F360_Object_Track_T& object_track, 
      const CONF9_T threshold);

   bool Is_CTCA_Obj_Confidence_Less_Than_Or_Equal_To(
      const F360_Object_Track_T& object_track, 
      const CONF9_T threshold);

   bool Update_Low_RCS_Dets_Counter_And_Block_Confidence(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detect_list,
      const F360_Calibrations_T& calib,
      F360_Object_Track_T& object_track);
}
#endif
