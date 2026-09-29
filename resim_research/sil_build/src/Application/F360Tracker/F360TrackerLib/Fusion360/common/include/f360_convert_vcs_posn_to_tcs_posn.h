/*===================================================================================*\
* FILE: f360_convert_vcs_posn_to_tcs_posn.h
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
* This file contains function signature of Convert_VCS_Posn_To_TCS_Posn().
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
#ifndef F360_CONVERT_VCS_POSN_TO_TCS_POSN
#define F360_CONVERT_VCS_POSN_TO_TCS_POSN

#include "f360_reuse.h"
#include "f360_angle.h"

namespace f360_variant_A
{
   void Convert_VCS_Posn_To_TCS_Posn(
      const float32_t vcs_x,
      const float32_t vcs_y,
      const float32_t target_vcs_position_x,
      const float32_t target_vcs_position_y,
      const Angle & vcs_pointing,
      float32_t & tcs_x,
      float32_t & tcs_y
   );
}
#endif
