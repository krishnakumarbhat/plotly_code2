/*===================================================================================*\
* FILE: f360_convert_bbox_vcs_to_tcs.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declarations of Convert_Bbox_VCS_To_TCS function.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*===================================================================================*/
#ifndef F360_CONVERT_BBOX_VCS_TO_TCS_H
#define F360_CONVERT_BBOX_VCS_TO_TCS_H

#include "f360_object_track.h"
#include "f360_bounding_box.h"

namespace f360_variant_A
{
   BboxCorners Convert_Bbox_VCS_To_TCS(
      const BboxCorners & vcs_bbox,
      const F360_Object_Track_T & obj);
}
#endif
