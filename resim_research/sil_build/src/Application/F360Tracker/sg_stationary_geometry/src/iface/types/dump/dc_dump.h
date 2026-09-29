/*===================================================================================*\
* FILE: dc_dump.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of DC_Dump_T dump type.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_DUMP_H
#define DC_DUMP_H

#include "dc_contours_dump.h"
#include "dc_critical_region_dump.h"
#include "dc_vertices_dump.h"
#include "sg_constants.h"

namespace sg
{
   struct DC_Dump_T
   {
      DC_Critical_Region_Dump_T critical_region_dump;
      DC_Vertices_Dump_T dc_vertices_dump;
      DC_Contours_Dump_T dc_contours_dump;
      uint16_t num_DC_contours;
   };
}

#endif
