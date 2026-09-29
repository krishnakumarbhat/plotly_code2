/*===================================================================================*\
* FILE: dc_features_dump.h
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains mandatory SG datatypes for use with SG component resim.
*   They are: Intenals - so called debug data.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_FEATURES_DUMP_H
#define DC_FEATURES_DUMP_H
#include "sg_constants.h"

namespace sg
{
   struct DC_Features_Dump_T
   {
      float32_t detections_number[SG_MAX_NUM_OUTPUT_VERTICES];
      float32_t rcs_recur_mean[SG_MAX_NUM_OUTPUT_VERTICES];
      float32_t z_scs_abs_ewma025_mean[SG_MAX_NUM_OUTPUT_VERTICES];
      float32_t z_scs_abs_max[SG_MAX_NUM_OUTPUT_VERTICES];
      float32_t z_scs_abs_recur_mean[SG_MAX_NUM_OUTPUT_VERTICES];
      float32_t height_under_nondr_bins_proportion[SG_MAX_NUM_OUTPUT_VERTICES];
      float32_t height_over_nondr_bins_proportion[SG_MAX_NUM_OUTPUT_VERTICES];
      float32_t detection_density[SG_MAX_NUM_OUTPUT_VERTICES];
   };
}

#endif
