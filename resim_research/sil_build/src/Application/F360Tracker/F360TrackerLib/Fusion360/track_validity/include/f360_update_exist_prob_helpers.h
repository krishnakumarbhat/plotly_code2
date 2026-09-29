/*===================================================================================*\
* FILE: f360_update_exist_prob_helpers.h
*====================================================================================
*Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains functions declarations used in Update_Existence_Probability()
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef F360_UPDATE_EXIST_PROB_HELPERS_H
#define F360_UPDATE_EXIST_PROB_HELPERS_H


#include "f360_object_track.h"
#include "f360_calibrations.h"
#include "rspp_detection.h"
#include "f360_host.h"

namespace f360_variant_A
{
   void Update_Track_EP_With_New_Information(
      const float32_t & p_real_obj_detected,
      const float32_t & p_clutter_detected,
      const float32_t & min_allowed_ep,
      float32_t & ep);

   void Update_Track_EP_With_No_Information(
      const float32_t & p_real_obj_detected,
      const float32_t & p_clutter_detected,
      const float32_t & min_allowed_ep,
      float32_t & ep);

   float32_t Calculate_P_Track_State(
      const F360_Object_Track_T & object_track,
      const F360_Calibrations_T & calib);

   void Normalize_State_Variances(
      const F360_Object_Track_T & object_track,
      const F360_Calibrations_T & calib,
      const uint32_t & actual_state_dimension,
      float32_t(&s2)[STATE_DIMENSION]);

   void Normalize_Information(
      const float32_t(&s2)[STATE_DIMENSION],
      const F360_Calibrations_T & calib,
      const uint32_t & actual_state_dimension,
      float32_t & information_normalized);

   void Normalize_State(
      float32_t(&s2)[STATE_DIMENSION],
      const float32_t(&s2_th)[STATE_DIMENSION],
      const uint32_t & actual_state_dimension);

   void Decrease_EP_If_Low_RCS(
       const F360_Host_T& host,
       const float32_t cos_host_sideslip,
       const float32_t sin_host_sideslip,
       const F360_Calibrations_T& calib,
       F360_Object_Track_T& obj);
   
   bool Allow_IDM_variant(
       const F360_Tracker_Variant_T tracker_variant);

   void Decrease_EP_Based_On_IDM_Detections(
       const rspp_variant_A::RSPP_Detection_T(&detections)[MAX_NUMBER_OF_DETECTIONS],
       F360_Object_Track_T& obj);
}
#endif
