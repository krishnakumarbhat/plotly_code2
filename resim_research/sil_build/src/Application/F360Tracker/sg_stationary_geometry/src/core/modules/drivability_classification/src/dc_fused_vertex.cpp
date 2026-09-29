/*===================================================================================*\
* FILE: dc_fused_vertex.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains definition of Fused_Vertex type.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_fused_vertex.h"

namespace sg
{
   namespace dc
   {
      Fused_Vertex_T::Fused_Vertex_T(const geometry::Point2D_T &_position) : position{_position}
      {
      }
   }
}