/*===================================================================================*\
* FILE: dc_calculate_subsegment_features.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of calculate_subsegment_features function.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_CALCULATE_SUBSEGMENT_FEATURES
#define DC_CALCULATE_SUBSEGMENT_FEATURES

#include "dc_contour_storage.h"
#include "dc_subsegment_detections.h"
#include "sg_calibrations.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      void calculate_subsegment_features(Subsegment_T &subsegment, const SubsegmentDetections_T &current_data);

      void incorporate_current_data_to_past_data(Subsegment_T &subsegment, const SubsegmentDetections_T &current_data);

      void assign_features(Subsegment_T &subsegment);
   }
}
#endif
