/*===================================================================================*\
* FILE: dc_subsegment_vertex.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file implements Subsegment_T Vertex that is in Subsegment.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_subsegment_vertex.h"

namespace sg
{
   namespace dc
   {
      Subsegment_Vertex_T::Subsegment_Vertex_T(const geometry::Point2D_T &vertex_position,
                                               const bool vertex_f_critical,
                                               const bool vertex_f_primary)
          : position{vertex_position}, f_critical{vertex_f_critical}, f_primary{vertex_f_primary}
      {
      }
   }
}
