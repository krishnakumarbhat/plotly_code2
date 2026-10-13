/*===================================================================================*\
* FILE: dc_features_dump.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains mandatory SG datatypes for use with SG component resim.
*   They are: Internals - so called debug data.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_FEATURES_DUMP_H
#define DC_FEATURES_DUMP_H

#include "sg_variant.h"

namespace sg
{
struct DC_Features_Dump_T
{
   float detections_number[DC_MAX_NUM_OUTPUT_VERTICES];
   float rcs_recur_mean[DC_MAX_NUM_OUTPUT_VERTICES];
   float z_scs_abs_ewma025_mean[DC_MAX_NUM_OUTPUT_VERTICES];
   float z_scs_abs_max[DC_MAX_NUM_OUTPUT_VERTICES];
   float z_scs_abs_recur_mean[DC_MAX_NUM_OUTPUT_VERTICES];
   float height_under_nondr_bins_proportion[DC_MAX_NUM_OUTPUT_VERTICES];
   float height_over_nondr_bins_proportion[DC_MAX_NUM_OUTPUT_VERTICES];
   float detection_density[DC_MAX_NUM_OUTPUT_VERTICES];
};
} // namespace sg

#endif
