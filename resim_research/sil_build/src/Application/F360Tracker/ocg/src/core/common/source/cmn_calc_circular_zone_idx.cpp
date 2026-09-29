/*===================================================================================*\
* FILE: cmn_calc_circular_zone_idx.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Calc_Circular_Zone_Idx() function definition
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "cmn_calc_circular_zone_idx.h"

namespace ocg
{
   namespace cmn
   {
      /*===========================================================================*\
      * FUNCTION: Calc_Circular_Zone_Idx()
      *===========================================================================
      * RETURN VALUE:
      * uint32_t
      *
      * PARAMETERS:
      * const uint16_t circular_buffer_idx,
      * const int32_t zone_idx
      *
      * EXTERNAL REFERENCES:
      * None.
      *
      * DEVIATIONS FROM STANDARDS:
      * None.
      *
      * --------------------------------------------------------------------------
      * ABSTRACT:
      * --------------------------------------------------------------------------
      * Function calculates index of zone considering circular buffer index.
      *
      * PRECONDITIONS:
      *
      * POSTCONDITIONS:
      * None
      *
      \*===========================================================================*/
      uint32_t Calc_Circular_Zone_Idx(
         const uint16_t circular_buffer_idx,
         const uint32_t zone_idx,
         const uint8_t  num_cells_x)
      {
         uint32_t circular_zone_idx = zone_idx + static_cast<uint32_t>(circular_buffer_idx);
         if ((num_cells_x - 1U) < circular_zone_idx)
         {
            circular_zone_idx -= num_cells_x;
         }

         return circular_zone_idx;
      }
   }
}


