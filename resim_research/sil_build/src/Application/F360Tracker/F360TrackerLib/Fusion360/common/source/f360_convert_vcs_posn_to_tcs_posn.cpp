/*===================================================================================*\
* FILE: f360_convert_vcs_posn_to_tcs_posn.cpp
*====================================================================================
* Copyright © 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains function definition of Convert_VCS_Posn_To_TCS_Posn()
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [May 26, 2019]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#include "f360_convert_vcs_posn_to_tcs_posn.h"
#include "f360_math.h"
#include "f360_math_func.h"
namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Convert_VCS_Posn_To_TCS_Posn()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t vcs_x - Longitudinal position of point to be converted in VCS
   * const float32_t vcs_y - Lateral position of point to be converted in VCS
   * const float32_t target_vcs_position_x - Longitudinal position of target's centroid in VCS
   * const float32_t target_vcs_position_y - Lateral position of target's centroid in VCS
   * const Angle & vcs_pointing - target pointing in VCS
   * float32_t * const tcs_x - X position of converted point in TCS
   * float32_t * const tcs_y - Y position of converted point in TCS
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function converts a position in VCS to TCS by translating and rotating 
   * the point.
   *
   \*===========================================================================*/
   void Convert_VCS_Posn_To_TCS_Posn(
      const float32_t vcs_x,
      const float32_t vcs_y,
      const float32_t target_vcs_position_x,
      const float32_t target_vcs_position_y,
      const Angle & vcs_pointing,
      float32_t & tcs_x,
      float32_t & tcs_y)
   {
      float32_t temp_x;
      float32_t temp_y;
      F360_Translate_2D_Position(vcs_x, vcs_y, -target_vcs_position_x, -target_vcs_position_y, temp_x, temp_y);
      F360_Rotate_2D_Vector(temp_x, temp_y, vcs_pointing.Cos(), -vcs_pointing.Sin(), tcs_x, tcs_y);
   }
}
