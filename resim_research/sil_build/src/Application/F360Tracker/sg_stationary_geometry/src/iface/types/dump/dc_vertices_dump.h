/*===================================================================================*\
* FILE: dc_vertices_dump.h
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

#ifndef DC_VERTICES_DUMP_H
#define DC_VERTICES_DUMP_H

#ifdef SG_SAVE_DETECTIONS_ASSIGNED_TO_SUBSEGMENTS
#include "dc_assigned_detections_dump.h"
#endif

#include "dc_features_dump.h"
#include "sg_constants.h"
#include "sg_drivability_class.h"

namespace sg
{
   struct DC_Vertices_Dump_T
   {
      DC_Features_Dump_T dc_features_dump;
      float32_t position_x[SG_MAX_NUM_OUTPUT_VERTICES];
      float32_t position_y[SG_MAX_NUM_OUTPUT_VERTICES];
      SG_Drivability_Class_T drivability[SG_MAX_NUM_OUTPUT_VERTICES];
      uint8_t drivability_confidence[SG_MAX_NUM_OUTPUT_VERTICES];
      uint32_t segment_id[SG_MAX_NUM_OUTPUT_VERTICES];
      uint32_t unique_id[SG_MAX_NUM_OUTPUT_VERTICES];
      uint8_t f_critical[SG_MAX_NUM_OUTPUT_VERTICES];
      uint8_t f_primary[SG_MAX_NUM_OUTPUT_VERTICES];
#ifdef SG_SAVE_DETECTIONS_ASSIGNED_TO_SUBSEGMENTS
      DC_Assigned_Detections_Dump_T assigned_detections[SG_MAX_NUM_OUTPUT_VERTICES];
#endif
   };
}

#endif
