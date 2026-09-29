/*===================================================================================*\
* FILE: dc_subsegment_vertex.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file declares Subsegment_Vertex_T that is in Subsegment.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_SUBSEGMENT_VERTEX_H
#define DC_SUBSEGMENT_VERTEX_H

#include "geometry/geo_point.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      class Subsegment_Vertex_T
      {
        public:
         Subsegment_Vertex_T() = default;
         Subsegment_Vertex_T(const geometry::Point2D_T &_position, const bool _f_critical = false, const bool _f_primary = false);
         geometry::Point2D_T position{}; // [m] position
         bool f_critical{false};
         bool f_primary{false};
      };
   }
}

#endif
