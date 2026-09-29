/*===================================================================================*\
* FILE: dc_critical_region_dump.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of DC_Critical_Region_Dump_T dump type.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_CRITICAL_REGION_DUMP_H
#define DC_CRITICAL_REGION_DUMP_H
#include "sg_constants.h"

namespace sg
{
   struct DC_Critical_Region_Dump_T
   {
      struct Position_2D_T
      {
         float x;
         float y;
      };

      Position_2D_T critical_region[DC_MAX_COORDINATES_REGION_SIZE];
   };
}

#endif