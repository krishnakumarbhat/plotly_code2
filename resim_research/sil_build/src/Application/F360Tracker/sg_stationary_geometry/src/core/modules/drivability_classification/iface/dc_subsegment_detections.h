/*===================================================================================*\
* FILE: dc_subsegment_detections.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of SubsegmentDetections_T.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_SUBSEGMENT_DETECTIONS_H
#define DC_SUBSEGMENT_DETECTIONS_H

#include "sg_calibrations.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      struct SubsegmentDetections_T
      {
         std::array<float, SG_MAX_NUM_DETS_PER_SUBSEGMENT> range{};
         std::array<float, SG_MAX_NUM_DETS_PER_SUBSEGMENT> rcs{};
         std::array<float, SG_MAX_NUM_DETS_PER_SUBSEGMENT> snr{};
         std::array<float, SG_MAX_NUM_DETS_PER_SUBSEGMENT> z_scs{};
         std::array<float, SG_MAX_NUM_DETS_PER_SUBSEGMENT> z_scs_abs{};
         std::array<int8_t, SG_MAX_NUM_DETS_PER_SUBSEGMENT> confid_azimuth{};
         std::array<int8_t, SG_MAX_NUM_DETS_PER_SUBSEGMENT> confid_elevation{};
         std::array<bool, SG_MAX_NUM_DETS_PER_SUBSEGMENT> f_super_res{};
         std::array<bool, SG_MAX_NUM_DETS_PER_SUBSEGMENT> f_bistatic{};
         uint8_t num_associated_dets{};
         uint8_t num_valid_dets{};
      };
   }
}
#endif
