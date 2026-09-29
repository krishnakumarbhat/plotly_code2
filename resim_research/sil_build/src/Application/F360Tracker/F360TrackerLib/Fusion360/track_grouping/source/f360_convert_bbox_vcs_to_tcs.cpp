/*===================================================================================*\
* FILE: f360_convert_bbox_vcs_to_tcs.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains definition of Convert_Bbox_VCS_To_TCS() function.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
* None.
*
\*===================================================================================*/

#include "f360_convert_bbox_vcs_to_tcs.h"
#include "f360_convert_vcs_posn_to_tcs_posn.h"


namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Convert_Bbox_VCS_To_TCS()
   *===========================================================================
   * RETURN VALUE:
   * BboxCorners obj_bbox_in_tcs
   *
   * PARAMETERS:
   * const BboxCorners & vcs_bbox
   * const F360_Object_Track_T & obj
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * The function converts an objects bounding box corners in VCS to another 
   * objects target coordiante system (TCS)
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   \*===========================================================================*/
   BboxCorners Convert_Bbox_VCS_To_TCS(
      const BboxCorners & vcs_bbox,
      const F360_Object_Track_T & obj)
   {
      BboxCorners obj_bbox_in_tcs;

      // Convert front right corner
      Convert_VCS_Posn_To_TCS_Posn(
         vcs_bbox.Front_Right().x,
         vcs_bbox.Front_Right().y,
         obj.bbox.Get_Center().x,
         obj.bbox.Get_Center().y,
         obj.bbox.Get_Orientation(),
         obj_bbox_in_tcs.Front_Right().x,
         obj_bbox_in_tcs.Front_Right().y);

      // Convert front left corner
      Convert_VCS_Posn_To_TCS_Posn(
         vcs_bbox.Front_Left().x,
         vcs_bbox.Front_Left().y,
         obj.bbox.Get_Center().x,
         obj.bbox.Get_Center().y,
         obj.bbox.Get_Orientation(),
         obj_bbox_in_tcs.Front_Left().x,
         obj_bbox_in_tcs.Front_Left().y);

      // Convert rear right corner
      Convert_VCS_Posn_To_TCS_Posn(
         vcs_bbox.Rear_Right().x,
         vcs_bbox.Rear_Right().y,
         obj.bbox.Get_Center().x,
         obj.bbox.Get_Center().y,
         obj.bbox.Get_Orientation(),
         obj_bbox_in_tcs.Rear_Right().x,
         obj_bbox_in_tcs.Rear_Right().y);

      // Convert rear left corner
      Convert_VCS_Posn_To_TCS_Posn(
         vcs_bbox.Rear_Left().x,
         vcs_bbox.Rear_Left().y,
         obj.bbox.Get_Center().x,
         obj.bbox.Get_Center().y,
         obj.bbox.Get_Orientation(),
         obj_bbox_in_tcs.Rear_Left().x,
         obj_bbox_in_tcs.Rear_Left().y);

      return obj_bbox_in_tcs;
   }
}
