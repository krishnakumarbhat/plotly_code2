/*===================================================================================*\
* FILE: sg_contour_storage_dump.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of SG_Contour_Storage_Dump_T dump type.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_CONTOUR_STORAGE_DUMP_H
#define SG_CONTOUR_STORAGE_DUMP_H

#include "sg_constants.h"
#include "sg_contour_dump.h"
#include "sg_vertex_dump.h"

namespace sg
{
   struct SG_Contour_Storage_Dump_T
   {
      SG_Vertex_Dump_T vertices[SG_MAX_NUM_VERTICES];
      SG_Contour_Dump_T contours[SG_MAX_NUM_CONTOURS];
      uint32_t number_of_contours;
   };
}

#endif
