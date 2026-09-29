/*===================================================================================*\
* FILE: f360_update_object_confidence_levels.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function declaration of Update_Object_Confidence_Levels() function
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_UPDATE_OBJECT_CONFIDENCE_LEVELS_H
#define F360_UPDATE_OBJECT_CONFIDENCE_LEVELS_H

#include "f360_tracker_info.h"
#include "f360_calibrations.h"
#include "f360_object_track.h"
namespace f360_variant_A
{
   void Update_Object_Confidence_Levels(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Calibrations_T& calib,
      F360_Object_Track_T& object);
}
#endif
