/*===================================================================================*\
* FILE: dc_fused_vertex.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of Fused_Vertex type.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_FUSED_VERTEX_H
#define DC_FUSED_VERTEX_H

#include "geometry/geo_point.h"
#include "sg_basic.h"
#include "sg_constants.h"
#include "sg_drivability_class.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      struct Fused_Vertex_T
      {
         /**
          * @brief    Default constructor for fused contours.
          **/
         Fused_Vertex_T() = default;

         /**
          * @brief    Constructor of Fused_Vertex_T class. Initializes the fused contour with elements from source.
          *
          * @param[in]    _position - position of fused vertex
          **/
         Fused_Vertex_T(const geometry::Point2D_T &_position);

         Cross_Covariance_2D pos_cross_cov{}; // [m^2] position cross-covariance
         Covariance_2D pos_cov{};             // [m^2] position covariance
         geometry::Point2D_T position{};      // [m] position
         float drivability_confidence{0.0F};  // [-] measure of how reliable vertex information is
         SG_Drivability_Class_T drivability{SG_Drivability_Class_T::UNCLASSIFIED}; // [-] SG_Drivability_Class_T

         uint16_t sg_age{0U};                  // equivalent of age in Vertex_T
         uint16_t sg_cycles_since_coasted{0U}; // equivalent of num_cycles_no_update in Vertex_T
      };
   }
}

#endif
