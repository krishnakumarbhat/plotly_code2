/*===================================================================================*\
* FILE: sg_contour_dump.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains mandatory SG datatypes i.e. contour dump type.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_CONTOURS_DUMP_H
#define SG_CONTOURS_DUMP_H
#include "sg_reuse.h"

namespace sg
{
   struct SG_Contour_Dump_T
   {
      float priority;
      uint32_t cluster_id;
      uint32_t unique_id;
      uint16_t num_vertices;
      uint8_t drivability;
      bool f_selected_for_output;
   };
}

#endif
