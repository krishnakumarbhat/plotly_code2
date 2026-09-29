/*===========================================================================*\
* FILE: f360_clear_det_assoc_obj_props.h
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Clear_Det_Assoc_Obj_Props()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_CLEAR_DET_ASSOC_OBJ_PROPS_H
#define F360_CLEAR_DET_ASSOC_OBJ_PROPS_H

#include "f360_detection_props.h"
#include "f360_object_track.h"
#include "f360_constants.h"

namespace f360_variant_A
{
   void Clear_Det_Assoc_Obj_Props(
      const F360_Object_Track_T & obj,
      F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS]);
}
#endif
