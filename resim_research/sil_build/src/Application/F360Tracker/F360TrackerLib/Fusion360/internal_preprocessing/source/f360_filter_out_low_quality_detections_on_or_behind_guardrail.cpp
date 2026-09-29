/*===========================================================================*\
* FILE: f360_filter_out_low_quality_detections_on_or_behind_guardrail.cpp
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_filter_out_low_quality_detections_on_or_behind_guardrail.h" 

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list - Raw detection list
   * const F360_Calibrations_T &calib - Calibration structure
   * F360_Detection_Props_T (&det_p)[MAX_NUMBER_OF_DETECTIONS] - Detection properties
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function marks detections that are on or behind guardrail, 
   * have a high azimuth confidence and a low rcs as not ok to use.
   *
   * PRECONDITIONS:
   * None.
   * 
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail(
      const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
      const F360_Calibrations_T &calib,
      F360_Detection_Props_T (&det_p)[MAX_NUMBER_OF_DETECTIONS])
   {
      for (uint32_t det_idx = 0U; det_idx < raw_detection_list.number_of_valid_detections; det_idx++)
      {
         Filter_Out_Low_Quality_Detection_On_Or_Behind_Guardrail(raw_detection_list.detections[det_idx], calib, det_p[det_idx]);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail()
   * ===========================================================================
   * RETURN VALUE:
   * None.
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_T &detection - raw detection
   * const F360_Calibrations_T &calib - Calibration structure
   * F360_Detection_Props_T &detection_prop - Detection properties
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function marks single detection that is on or behind guardrail,
   * has a high azimuth confidence and a low rcs as not ok to use.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Filter_Out_Low_Quality_Detection_On_Or_Behind_Guardrail(
      const rspp_variant_A::RSPP_Detection_T &detection,
      const F360_Calibrations_T &calib,
      F360_Detection_Props_T &detection_prop)
   {
      if ((detection.raw.confid_azimuth >= calib.k_azimuth_conf_low_quality_detection_filter) &&
         (detection.raw.rcs < calib.k_max_rcs_thr_low_quality_detection_filter) &&
         ((detection_prop.behind_sep_id != F360_INVALID_UNSIGNED_ID) || (detection_prop.on_sep_id != F360_INVALID_UNSIGNED_ID)))
      {
         detection_prop.f_ok_to_use = false;
      }
   }
}
