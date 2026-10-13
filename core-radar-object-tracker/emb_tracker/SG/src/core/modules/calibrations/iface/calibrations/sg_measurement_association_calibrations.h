/*===========================================================================*\
* FILE: sg_measurement_association_calibrations.h
*============================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains calibrations used in measurement association step.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef SG_MEASUREMENT_ASSOCIATION_CALIBRATIONS_H
#define SG_MEASUREMENT_ASSOCIATION_CALIBRATIONS_H

#include "sg_reuse.h"

namespace sg
{
struct MeasurementAssociationCalibs
{
   // associate_detections_to_contour
   float width_min{};
   float width_max{};
   float length_margin_min{};
   float length_margin_max{};
   float length_margin_ending{};

   // associate_cluster_boundary_detections_to_contour
   float width_extension_min{};
   float width_extension_max{};
   float extended_meas_covariance_xx{};
   float extended_meas_covariance_yy{};

   float dynamic_gate_lower_distance{};
   float dynamic_gate_upper_distance{};

   uint16_t min_dets_assoc_update_cluster_id{};
   bool f_dynamic_gates{};
};
} // namespace sg
#endif
