/*===================================================================================*\
* FILE: dc_subsegment.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file implements Subsegment_T class related to DC subsegments.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_subsegment.h"

#include <utility>

namespace sg
{
   namespace dc
   {
      Subsegment_T::Subsegment_T(const Subsegment_Vertex_T &vertex_begin, const Subsegment_Vertex_T &vertex_end)
          : past_data{},
            features{},
            begin_vertex{vertex_begin},
            end_vertex{vertex_end},
            segment_id{0U},
            subsegment_id{0U},
            sg_age{0U},
            sg_cycles_since_coasted{0U},
            drivability(SG_Drivability_Class_T::UNCLASSIFIED)
      {
      }

      bool Subsegment_T::is_critical() const
      {
         return begin_vertex.f_critical && end_vertex.f_critical;
      }
   }
}
