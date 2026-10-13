/*=============================================================================================*\
* FILE: sg_importance_calibration.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains a class with importance calibration.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN, "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_IMPORTANCE_CALIBRATION_H
#define SG_IMPORTANCE_CALIBRATION_H
#include <cmath>

#include "sg_reuse.h"

namespace sg
{
// class for storing importance calibrations
class ImportanceCalibs
{
 public:
   float distance_factors[3U];
   float shape_correction_factors[2U];
   float age_impact_factor;
   float distance_impact_factor;
   float host_speed_impact_factor;
   uint8_t max_associated_det_vertex_age; // Max vertex age of detection which is already associated
   uint8_t max_det_age;
   bool use_existence_probability;

   /**
    * @brief          Method to adjust distance factors to the host speed.
    *
    * @param[in]      host_speed
    **/
   void adjust_distance_factors(const float host_speed);
};
} // namespace sg

#endif
