/*===================================================================================*\
* FILE: sg_contour_type.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of enum for Contour Type
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/


#ifndef SG_CONTOUR_TYPE_ENUM_H
#define SG_CONTOUR_TYPE_ENUM_H

#include "sg_reuse.h"

namespace sg
{
   enum class SG_Contour_Type_T : std::uint8_t
   {
      INVALID  = 0,
      POINT    = 1,
      POLYLINE = 2,
      POLYGON  = 3
   };
}

#endif
