/*===========================================================================*\
* FILE: sg_measurement_update_calibrations.h
*============================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains calibrations used in measurement update step.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef SG_MEASUREMENT_UPDATE_CALIBRATIONS_H
#define SG_MEASUREMENT_UPDATE_CALIBRATIONS_H

#include "sg_constants.h"
#include "sg_reuse.h"

namespace sg
{
struct MeasurementUpdateCalibs
{
   float length_measurement_noise{};
   float length_forgetting_factor{};
   float minimal_length_spread{};
   float position_forgetting_factor{};
   float regularity_weight{};    // (1/regularity variance), must be balanced with R_position_covariance_x, higher value = more
                                 // regularization
   float regularity_threshold{}; // squared distance between 3 vertex subcontour endvertices average and second vertex,
                                 // higher value = regularization terminates earlier
   uint8_t min_dets_for_shrinking{};
   uint8_t max_num_ending_segment_dets{};
};
} // namespace sg
#endif
