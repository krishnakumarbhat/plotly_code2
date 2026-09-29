/*===================================================================================*\
* FILE: dc_fused_contour.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains definition of Fused_Contour type.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_fused_contour.h"

namespace sg
{
   namespace dc
   {
      Fused_Contour_T::Fused_Contour_T()
          : vertices{},
            num_of_vertices{static_cast<uint16_t>(vertices.size())},
            f_selected_for_output{false},
            sg_drivability{SG_Drivability_Class_T::NONDRIVABLE},
            m_id{INVALID_SEGMENT_ID}
      {
      }

      Fused_Contour_T::Fused_Contour_T(const std::initializer_list<Fused_Vertex_T> init_list)
          : vertices{init_list},
            num_of_vertices{static_cast<uint16_t>(vertices.size())},
            f_selected_for_output{false},
            sg_drivability{SG_Drivability_Class_T::NONDRIVABLE},
            m_id{INVALID_SEGMENT_ID}
      {
      }

      Fused_Contour_T::Fused_Contour_T(const FusedVertexList &_vertices,
                                       const uint32_t _id,
                                       const uint16_t _num_of_vertices,
                                       const bool _f_selected_for_output,
                                       const SG_Drivability_Class_T _sg_drivability)
          : num_of_vertices{_num_of_vertices}, f_selected_for_output{_f_selected_for_output}, sg_drivability{_sg_drivability}, m_id{_id}
      {
         vertices = _vertices;
      }

      uint32_t Fused_Contour_T::get_id() const
      {
         return m_id;
      }

      void Fused_Contour_T::set_id(const uint32_t id)
      {
         m_id = id;
      }
   }
}
