/*===================================================================================*\
* FILE: ocg_occupancy_grid_definition.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Occupancy Grid definiton
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef OCG_OCCUPANCY_GRID_DEFINITION_H
#define OCG_OCCUPANCY_GRID_DEFINITION_H

#include "ocg_reuse.h"

namespace ocg
{
   struct OCG_Definition_T
   {
      uint16_t num_cells_x_far;
      uint16_t num_cells_x_mid;
      uint16_t num_cells_x_close;
      uint16_t num_cells_y;

      float cell_length;
      float cell_width;
      float cell_width_extension_factor;
   };
}

#endif
