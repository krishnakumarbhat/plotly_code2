/*===========================================================================*\
* FILE: f360_filter_out_low_quality_detections_on_or_behind_guardrail.h
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_FILTER_OUT_LOW_QUALITY_DETECTIONS_ON_OR_BEHIND_GUARDRAIL
#define F360_FILTER_OUT_LOW_QUALITY_DETECTIONS_ON_OR_BEHIND_GUARDRAIL

#include "rspp_detection_list.h"
#include "f360_calibrations.h"
#include "f360_detection_props.h"

namespace f360_variant_A
{
   void Filter_Out_Low_Quality_Detections_On_Or_Behind_Guardrail(
      const rspp_variant_A::RSPP_Detection_List_T &raw_detection_list,
      const F360_Calibrations_T &calib,
      F360_Detection_Props_T (&det_p)[MAX_NUMBER_OF_DETECTIONS]
   );

   void Filter_Out_Low_Quality_Detection_On_Or_Behind_Guardrail(
      const rspp_variant_A::RSPP_Detection_T &detection,
      const F360_Calibrations_T &calib,
      F360_Detection_Props_T &detection_prop
   );
}

#endif
