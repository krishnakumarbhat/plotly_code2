/*===========================================================================*\
* FILE: f360_get_track_bbox_in_vcs.cpp
*============================================================================
* Copyright (C) 2020-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*---------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function definition of Get_Track_Bbox_In_VCS().
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/


/******************************
* Includes
*******************************/
#include "f360_get_track_bbox_in_vcs.h"
#include "f360_convert_tcs_posn_to_vcs_posn.h"
#include "f360_math_func.h"
#include "f360_check_if_point_is_inside_box.h"
#include "f360_bounding_box.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Get_TCS_Box_Corners_In_VCS()
   *===========================================================================
   * RETURN VALUE:
   * BboxCorners bbox_corners_vcs - Structure containing the four corners of the 
   *                                box. Each corner is represented by its 
   *                                longitudinal and lateral position in VCS.
   *
   * PARAMETERS:
   * const F360_Object_Track_T & object_track - Object whose coordinate system (TCS) the box 
   *                                            is defined in.
   * float32_t (&box_tcs)[2][2]              - Min and max values of box. First row: min and 
   *                                            max longitudinal, second row: min and max lateral.
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
   * This function takes a box defined in a given targets coordinate system (TCS)
   * and converts the corners of the box to VCS.
   *
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   BboxCorners Get_TCS_Box_Corners_In_VCS(
      const F360_Object_Track_T & object_track,
      const float32_t (&box_tcs)[2][2])
   {
      const float32_t box_tcs_maxpara = box_tcs[0][1];
      const float32_t box_tcs_minpara = box_tcs[0][0];
      const float32_t box_tcs_maxorth = box_tcs[1][1];
      const float32_t box_tcs_minorth = box_tcs[1][0];

      BboxCorners box_corners_vcs = {};

      const Point rear_left_corner_tcs = { box_tcs_minpara, box_tcs_minorth };
      Convert_TCS_Posn_To_VCS_Posn(
         rear_left_corner_tcs.x,
         rear_left_corner_tcs.y,
         object_track.bbox.Get_Center().x,
         object_track.bbox.Get_Center().y,
         object_track.bbox.Get_Orientation(),
         box_corners_vcs.Rear_Left().x,
         box_corners_vcs.Rear_Left().y);

      const Point rear_right_corner_tcs = { box_tcs_minpara, box_tcs_maxorth };
      Convert_TCS_Posn_To_VCS_Posn(
         rear_right_corner_tcs.x,
         rear_right_corner_tcs.y,
         object_track.bbox.Get_Center().x,
         object_track.bbox.Get_Center().y,
         object_track.bbox.Get_Orientation(),
         box_corners_vcs.Rear_Right().x,
         box_corners_vcs.Rear_Right().y);

      const Point front_left_corner_tcs = { box_tcs_maxpara, box_tcs_minorth };
      Convert_TCS_Posn_To_VCS_Posn(
         front_left_corner_tcs.x,
         front_left_corner_tcs.y,
         object_track.bbox.Get_Center().x,
         object_track.bbox.Get_Center().y,
         object_track.bbox.Get_Orientation(),
         box_corners_vcs.Front_Left().x,
         box_corners_vcs.Front_Left().y);

      const Point front_right_corner_tcs = { box_tcs_maxpara, box_tcs_maxorth };
      Convert_TCS_Posn_To_VCS_Posn(
         front_right_corner_tcs.x,
         front_right_corner_tcs.y,
         object_track.bbox.Get_Center().x,
         object_track.bbox.Get_Center().y,
         object_track.bbox.Get_Orientation(),
         box_corners_vcs.Front_Right().x,
         box_corners_vcs.Front_Right().y);

      return box_corners_vcs;
   }

   /*===========================================================================*\
   * FUNCTION: Get_Min_And_Max_Of_VCS_Box()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const BboxCorners & box_corners - corners of the box
   * float32_t (&min_max_vals)[2][2] - struct to save min and max values. First row: min and max longitudinal, second row: min and max lateral.
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
   * This function takes the four corners of a box and calculates the min and max longitudinal
   * and lateral values of the box.
   *
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Get_Min_And_Max_Of_VCS_Box(
      const BboxCorners & box_corners,
      float32_t (&min_max_vals)[2][2])
   {
      const float32_t corners_pos_longitudinal[4] = {
         box_corners.Front_Left().x,
         box_corners.Front_Right().x,
         box_corners.Rear_Left().x,
         box_corners.Rear_Right().x };

      const float32_t corners_pos_lateral[4] = {
         box_corners.Front_Left().y,
         box_corners.Front_Right().y,
         box_corners.Rear_Left().y,
         box_corners.Rear_Right().y };


      min_max_vals[0][0] = F360_Min_Element(corners_pos_longitudinal);
      min_max_vals[0][1] = F360_Max_Element(corners_pos_longitudinal);
      min_max_vals[1][0] = F360_Min_Element(corners_pos_lateral);
      min_max_vals[1][1] = F360_Max_Element(corners_pos_lateral);
   }
}
