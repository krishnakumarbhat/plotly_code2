/*===========================================================================*\
* FILE: f360_calc_heading_from_pos_diff.h
*============================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Calc_Heading_From_Pos_Diff()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_CALC_HEADING_FROM_POS_DIFF_H
#define F360_CALC_HEADING_FROM_POS_DIFF_H

#include "f360_calibrations.h"
#include "f360_tracker_info.h"
#include "f360_object_track.h"

namespace f360_variant_A
{
   void Calc_Heading_From_Pos_Diff(
      F360_Object_Track_T & object,
      const F360_Calibrations_T & calibs);
}

#endif
