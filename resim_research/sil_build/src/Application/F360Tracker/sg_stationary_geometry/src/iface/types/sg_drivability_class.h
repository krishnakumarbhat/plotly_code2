/*===================================================================================*\
* FILE: sg_drivability_class.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of enum for drivability class.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_DRIVABILITY_CLASS_H
#define SG_DRIVABILITY_CLASS_H

#include "sg_reuse.h"

namespace sg
{
   enum class SG_Drivability_Class_T : std::uint8_t
   {
      UNCLASSIFIED  = 0,
      OVERDRIVABLE  = 1, // the host can drive over an obstacle
      NONDRIVABLE   = 2, // the host cannot drive through
      UNDERDRIVABLE = 3, // the host can drive under an obstacle
      COUNT         = 4
   };
}

#endif
