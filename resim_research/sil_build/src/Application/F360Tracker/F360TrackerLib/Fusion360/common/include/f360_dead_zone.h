/*===================================================================================*\
* FILE: f360_dead_zone.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
* This file contains declaration of structure used to define longitudinal limits.
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#ifndef F360_DEAD_ZONE_T
#define F360_DEAD_ZONE_T

#include "f360_interval.h"

namespace f360_variant_A
{
   struct Dead_Zone_T
   {
      Dead_Zone_T() : basic(), extended() {}
      Interval<float32_t> basic;
      Interval<float32_t> extended;
   };
}
#endif
