/*===================================================================================*\
* FILE: cmn_calc_circular_zone_idx.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Calc_Circular_Zone_Idx() function declaration
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef CALC_CIRCULAR_ZONE_IDX_H
#define CALC_CIRCULAR_ZONE_IDX_H

#include "ocg_reuse.h"

namespace ocg
{
   namespace cmn
   {
      uint32_t Calc_Circular_Zone_Idx(
         const uint16_t circular_buffer_idx,
         const uint32_t zone_idx,
         const uint8_t  in_num_cells_x);
   }
}
#endif
