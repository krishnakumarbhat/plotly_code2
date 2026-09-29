/*===========================================================================*\
* FILE: f360_allocate_id_for_initialized_object.h
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains function declaration of Allocate_Id_For_Initialized_Object();
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef F360_ALLOCATE_ID_FOR_INITIALIZED_OBJECT_H
#define F360_ALLOCATE_ID_FOR_INITIALIZED_OBJECT_H

#include "f360_tracker_info.h"
#include "f360_object_track.h"
#include "f360_detection_props.h"

namespace f360_variant_A
{
   int32_t Allocate_Id_For_Initialized_Object(
      F360_Tracker_Info_T & tracker_info,
      F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS],
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]);
}
#endif
