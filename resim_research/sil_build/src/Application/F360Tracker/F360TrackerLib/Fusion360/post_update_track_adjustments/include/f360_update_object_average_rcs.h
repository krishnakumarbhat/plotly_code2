/*===========================================================================*\
* FILE: f360_update_object_average_rcs.h
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Update_Object_Average_Rcs()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_UPDATE_OBJECT_AVERAGE_RCS_H
#define F360_UPDATE_OBJECT_AVERAGE_RCS_H

#include "f360_object_track.h"
#include "rspp_detection.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void Update_Object_Average_Rcs(
      const rspp_variant_A::RSPP_Detection_T (&raw_dets)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Calibrations_T& calibs,
      F360_Object_Track_T& object);
}


#endif
