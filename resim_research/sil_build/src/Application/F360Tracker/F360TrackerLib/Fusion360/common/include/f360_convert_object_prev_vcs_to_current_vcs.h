/*===========================================================================*\
* FILE: f360_convert_object_prev_vcs_to_current_vcs.h
*============================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains function declaration of Convert_Object_Prev_Vcs_To_Current_Vcs().
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006
***/
#ifndef F360_CONVERT_OBJECT_PREV_VCS_TO_CURRENT_VCS_H
#define F360_CONVERT_OBJECT_PREV_VCS_TO_CURRENT_VCS_H

#include "f360_object_track.h"
#include "f360_host_props.h"
#include "f360_tracker_info.h"

namespace f360_variant_A
{
   void Convert_Object_Prev_Vcs_To_Current_Vcs(
      const F360_Tracker_Info_T & tracker_info,
      const F360_Host_Props_T & host_p,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]);

   void Convert_Object_Properties(
      const F360_Host_Props_T & host_p,
      F360_Object_Track_T& obj);

   void Convert_Object_Covariance(
      const F360_Host_Props_T & host_p,
      F360_Object_Track_T& obj);

   Point Transform_Point_From_Prev_To_Current_Vcs(
      const Point& prev_vcs_pos,
      const F360_Host_Props_T& host_p
   );
}


#endif
