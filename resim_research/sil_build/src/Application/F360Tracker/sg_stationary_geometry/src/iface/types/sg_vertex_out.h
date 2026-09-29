/*===================================================================================*\
* FILE: sg_vertex_out.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains SG component output vertex type definition
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef SG_VERTEX_OUT_H
#define SG_VERTEX_OUT_H

#include <cstdint>

#include "sg_drivability_class.h"

namespace sg
{
   struct SG_Vertex_Out_T
   {
      float position_x{0.0F};             // [m] longitudinal position
      float position_y{0.0F};             // [m] lateral position
      float position_variance_x{0.0F};    // [m^2] longitudinal position variance
      float position_variance_y{0.0F};    // [m^2] lateral position variance
      float position_covariance_xy{0.0F}; // [m^2] longitudinal/lateral position covariance
      uint16_t cycles_since_created{0U};  // [-] number of SG cycles this vertex has been tracked (since its creation)
      uint16_t cycles_since_coasted{0U};  // [-] number of SG cycles while this vertex was not updated by measurement
      SG_Drivability_Class_T drivability{SG_Drivability_Class_T::UNCLASSIFIED}; // [-] drivability class saying if the host can
                                                                                // drive through it
      uint8_t drivability_confidence{0U}; // [%] confidence regarding drivability classification result (0..100)
      uint8_t padding[2]{};
   };
}

#endif
