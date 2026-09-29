/*===================================================================================*\
* FILE: f360_clear_detections_props.h
*====================================================================================
*Copyright (C) <2020> Aptiv Advanced Safety and User Experience. All rights reserved.
*Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains the function declaration to reset the detection properties.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_CLEAR_DETECTIONS_PROPS_H
#define F360_CLEAR_DETECTIONS_PROPS_H

#include "f360_detection_props.h"
#include "f360_constants.h"

namespace f360_variant_A
{
   void Clear_Detections_Props (F360_Detection_Props_T (&det_p) [MAX_NUMBER_OF_DETECTIONS]);
}
#endif
