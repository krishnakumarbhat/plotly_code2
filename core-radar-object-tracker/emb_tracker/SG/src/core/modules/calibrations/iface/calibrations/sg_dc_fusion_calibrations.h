/*===========================================================================*\
* FILE: sg_dc_fusion_calibrations.h
*============================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains calibrations used in fusion step.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/

#ifndef SG_DC_FUSION_CALIBRATIONS_H
#define SG_DC_FUSION_CALIBRATIONS_H

#include "sg_reuse.h"

namespace sg
{
struct SGDCFusionCalibs
{
   uint16_t max_num_vertices_per_fused_contour{};
   uint16_t max_num_fused_contours{};
   uint16_t max_num_fused_vertices{};
};
} // namespace sg
#endif
