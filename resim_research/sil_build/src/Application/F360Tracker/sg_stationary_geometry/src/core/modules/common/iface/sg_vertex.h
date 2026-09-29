/*===================================================================================*\
* FILE: sg_contour_list.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of Vertex_T,

*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/


#ifndef SG_VERTEX_H
#define SG_VERTEX_H

#include "geometry/geo_rectangle.h"
#include "sg_basic.h"

namespace sg
{
   struct Vertex_T
   {
      Vertex_T() = default;

      Vertex_T(const geometry::Point2D_T &_position) : position{_position} {};

      Vertex_T(const geometry::Point2D_T &_position, const uint32_t _segment_id) : position{_position}, segment_id{_segment_id} {};

      geometry::Rectangle_T bounding_box{}; // bounding box for association
      geometry::Point2D_T position{};       // [m] position
      Pos_2D_Cov pos_cov{};                 // [m^2] position covariance
      Pos_2D_Cross_Cov pos_cross_cov{};     // [m^2] position cross-covariance

      float reliability{};   // [-] measure of how reliable vertex information is
      uint32_t segment_id{}; // [-] gloablly unique id of segment starting with this vertex (0 for the last vertex of a contour)
      uint16_t age{};        // [-] number of SG steps this vertex has been tracked (since its creation)
      uint16_t num_cycles_no_update{}; // [-] number of SG steps while this vertex was not updated
   };
}

#endif
