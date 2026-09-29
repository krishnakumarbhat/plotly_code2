/*===================================================================================*\
* FILE: dc_subsegment_controller_autocode.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of SubsegmentCotrollerAutocode class which controls flow of DC functions.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef DC_SUBSEGMENT_CONTROLLER_AUTOCODE_H
#define DC_SUBSEGMENT_CONTROLLER_AUTOCODE_H

#include "contra_init_types.h"
#include "sg_constants.h"
#include "sg_reuse.h"
#include "sg_timing_info.h"

namespace sg
{
   namespace dc
   {
      class SubsegmentControllerAutocode
      {
        public:
         SubsegmentControllerAutocode() = default;
         void create_critical_region(float (&critical_region)[DC_MAX_COORDINATES_REGION_SIZE][DC_COORDINATES_DIMENSION],
                                     const float curvature,
                                     const MATLAB::Calibrations_T &cfg) const;

         void step(TimingInfo &m_timing_info,
                   MATLAB::Internals_T &matlab_internals,
                   const MATLAB::Host_T &host,
                   const MATLAB::Input_Detections_List_T &detections_list,
                   const MATLAB::Calibrations_T &cfg);
      };
   }
}

#endif