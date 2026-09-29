/*===================================================================================*\
* FILE: f360_convert_tcs_posn_to_vcs_posn.cpp
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
* This file contains function definition of Convert_TCS_Posn_To_VCS_Posn()
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
#include "f360_convert_tcs_posn_to_vcs_posn.h"
#include "f360_math.h"
#include "f360_math_func.h"
namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Convert_TCS_Posn_To_VCS_Posn()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t tcs_x         - X position of point to be converted in TCS
   * const float32_t tcs_y         - Y position of point to be converted in TCS
   * const float32_t target_vcs_position_x - Longitudinal position of target's center in VCS
   * const float32_t target_vcs_position_y - Lateral position of target's center in VCS
   * const Angle & vcs_pointing     - Target pointing in VCS
   * float32_t * const vcs_x       - Longitudinal position of converted point in VCS
   * float32_t * const vcs_y       - Lateral position of converted point in VCS
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function converts a position in TCS to VCS by translating and rotating
   * the point.
   *
   *
   \*===========================================================================*/
   void Convert_TCS_Posn_To_VCS_Posn(
      const float32_t tcs_x,
      const float32_t tcs_y,
      const float32_t target_vcs_position_x,
      const float32_t target_vcs_position_y,
      const Angle & vcs_pointing,
      float32_t & vcs_x,
      float32_t & vcs_y)
   {
      float32_t temp_x;
      float32_t temp_y;
      F360_Rotate_2D_Vector(tcs_x, tcs_y, vcs_pointing.Cos(), vcs_pointing.Sin(), temp_x, temp_y);
      F360_Translate_2D_Position(temp_x, temp_y, target_vcs_position_x, target_vcs_position_y, vcs_x, vcs_y);
   }
}
