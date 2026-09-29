/*===================================================================================*\
* FILE: dc_contours_dump.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains mandatory SG datatypes for use with SG component resim.
*   They are: Intenals - so called debug data.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_CONTOURS_DUMP_H
#define DC_CONTOURS_DUMP_H
#include "sg_constants.h"
#include "sg_drivability_class.h"

namespace sg
{
   struct DC_Contours_Dump_T
   {
      uint32_t contour_id[SG_MAX_NUM_CONTOURS];
      uint16_t num_vertices[SG_MAX_NUM_CONTOURS];
      uint8_t f_valid[SG_MAX_NUM_CONTOURS];
      SG_Drivability_Class_T sg_drivability[SG_MAX_NUM_CONTOURS];
   };
}

#endif
