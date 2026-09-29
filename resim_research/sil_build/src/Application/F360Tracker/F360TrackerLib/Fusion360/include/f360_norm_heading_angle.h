/*===================================================================================*\
* FILE: f360_norm_heading_angle.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Normalize_Heading_Angle()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_NORM_HEADING_ANGLE_H
#define F360_NORM_HEADING_ANGLE_H

#include "f360_reuse.h"

namespace f360_variant_A
{
   float32_t Normalize_Heading_Angle(const float32_t angle_in,
      const float32_t interval_center);
}
#endif
