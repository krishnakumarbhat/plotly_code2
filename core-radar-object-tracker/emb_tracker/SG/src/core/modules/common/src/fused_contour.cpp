/*===================================================================================*\
* FILE: fused_contour.cpp
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

#include "fused_contour.h"

namespace sg
{
Fused_Contour_T::~Fused_Contour_T()
{
   vertices.clear();
   reset_fields(*this);
}

Fused_Contour_T::Fused_Contour_T(Fused_Contour_T &&in_contour) noexcept
{
   this->vertices = std::move(in_contour.vertices);
   copy_fields(in_contour);
   reset_fields(in_contour);
}

Fused_Contour_T::Fused_Contour_T(Fused_Contour_T &in_contour) noexcept
{
   this->vertices = in_contour.vertices;
   copy_fields(in_contour);
}

Fused_Contour_T &Fused_Contour_T::operator=(Fused_Contour_T &&source) noexcept
{
   this->vertices = std::move(source.vertices);
   copy_fields(source);
   reset_fields(source);
   return *this;
}

Fused_Contour_T &Fused_Contour_T::operator=(const Fused_Contour_T &source) noexcept
{
   this->vertices = source.vertices;
   copy_fields(source);
   return *this;
}

Fused_Contour_T::Fused_Contour_T()
    : vertices{}, m_id{INVALID_SEGMENT_ID}, num_of_vertices{static_cast<uint16_t>(vertices.size())}, cycles_since_created{0U},
      sg_drivability{SG_Drivability_Class_T::NONDRIVABLE}, f_selected_for_output{false}

{
}

Fused_Contour_T::Fused_Contour_T(const std::initializer_list<Fused_Vertex_T> init_list)
    : vertices{init_list}, m_id{INVALID_SEGMENT_ID}, num_of_vertices{static_cast<uint16_t>(vertices.size())},
      cycles_since_created{0U}, sg_drivability{SG_Drivability_Class_T::NONDRIVABLE}, f_selected_for_output{false}
{
}

Fused_Contour_T::Fused_Contour_T(const FusedVertexList &in_vertices, const uint32_t in_id, const uint16_t in_num_of_vertices,
                                 const uint16_t in_cycles_since_created, const SG_Drivability_Class_T in_sg_drivability,
                                 const bool in_f_selected_for_output)
    : m_id{in_id}, num_of_vertices{in_num_of_vertices}, cycles_since_created{in_cycles_since_created},
      sg_drivability{in_sg_drivability}, f_selected_for_output{in_f_selected_for_output}
{
   vertices = in_vertices;
}

uint32_t Fused_Contour_T::get_id() const
{
   return m_id;
}

void Fused_Contour_T::set_id(const uint32_t id)
{
   m_id = id;
}
} // namespace sg
