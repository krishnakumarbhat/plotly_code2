#ifndef F360_ASSOCIATE_DETECTION_TO_OBJECT_H
#define F360_ASSOCIATE_DETECTION_TO_OBJECT_H
/*===========================================================================*\
* FILE: f360_associate_detection_to_object.h
*============================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*----------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains functions declarations for Associate_Detection_To_Object().
*
* ABBREVIATIONS:
*   None
*
* TRACEABILITY INFO:
*   Requirements Document(s):
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===========================================================================*/

#include "f360_reuse.h"
#include "f360_object_track.h"
#include "f360_detection_props.h"
#include "f360_tracker_info.h"

namespace f360_variant_A
{
   bool Associate_Detection_To_Object(
      const F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T &object_track,
      F360_Detection_Props_T &detection_prop,
      const uint32_t det_id);

}
#endif
