/*===========================================================================*\
* FILE: f360_calc_obj_height.h
*============================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential ? Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Calc_Obj_Height()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_CALC_OBJ_HEIGHT_H
#define F360_CALC_OBJ_HEIGHT_H

#include "rspp_detection_list.h"
#include "f360_detection_props.h"
#include "f360_object_track.h"
#include <f360_host.h>

namespace f360_variant_A
{
   void Calc_Obj_Height(
      const rspp_variant_A::RSPP_Detection_T(&dets)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Host_T& host,
      F360_Object_Track_T& object_track);
}
#endif
