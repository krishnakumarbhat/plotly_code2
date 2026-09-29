/*===========================================================================*\
* FILE: f360_get_track_bbox_in_vcs.h
*============================================================================
* Copyright (C) 2020-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Get_Track_Bbox_In_VCS().
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef F360_GET_TRACK_BBOX_IN_VCS_H
#define F360_GET_TRACK_BBOX_IN_VCS_H

#include "f360_reuse.h"
#include "f360_object_track.h"
#include "f360_bounding_box.h"

namespace f360_variant_A
{
   BboxCorners Get_TCS_Box_Corners_In_VCS(
      const F360_Object_Track_T & object_track,
      const float32_t(&box_tcs)[2][2]);

   void Get_Min_And_Max_Of_VCS_Box(
      const BboxCorners & box_corners,
      float32_t(&min_max_vals)[2][2]);
}
#endif
