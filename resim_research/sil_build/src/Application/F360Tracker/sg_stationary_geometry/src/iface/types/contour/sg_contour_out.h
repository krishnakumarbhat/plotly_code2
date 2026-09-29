/*===================================================================================*\
* FILE: sg_contour_out.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains SG component output contour type definition
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_CONTOUR_OUT_H
#define SG_CONTOUR_OUT_H

#include "sg_contour_type.h"
#include "sg_drivability_class.h"
#include "sg_reuse.h"

namespace sg
{
   struct SG_Contour_Out_T
   {
      uint32_t unique_id{0U};                             // [-] unique ID of the contour (0 means invalid contour)
      uint16_t num_vertices{0U};                          // [-] number of vertices
      SG_Contour_Type_T type{SG_Contour_Type_T::INVALID}; // [-] type of a contour
      uint8_t padding[1]{};
   };
}

#endif
