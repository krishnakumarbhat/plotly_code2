/*===================================================================================*\
* FILE: dc_assigned_detections_dump.h
*====================================================================================
* Copyright (C) 2025 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains definition of detections assigned to subsegment for data dump
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_ASSIGNED_DETECTIONS_DUMP_H
#define DC_ASSIGNED_DETECTIONS_DUMP_H
#include "sg_constants.h"

namespace sg
{
   struct DC_Assigned_Detections_Dump_T
   {
      float range[SG_MAX_NUM_DETS_PER_SUBSEGMENT];
      float snr[SG_MAX_NUM_DETS_PER_SUBSEGMENT];
      float rcs[SG_MAX_NUM_DETS_PER_SUBSEGMENT];
      float z_scs[SG_MAX_NUM_DETS_PER_SUBSEGMENT];
      float z_scs_abs[SG_MAX_NUM_DETS_PER_SUBSEGMENT];
      float confid_azimuth[SG_MAX_NUM_DETS_PER_SUBSEGMENT];
      float confid_elevation[SG_MAX_NUM_DETS_PER_SUBSEGMENT];
      float f_super_res[SG_MAX_NUM_DETS_PER_SUBSEGMENT];
      float f_bistatic[SG_MAX_NUM_DETS_PER_SUBSEGMENT];
      uint8_t num_associated_dets;
      uint8_t num_valid_dets;
   };
}

#endif
