/*===================================================================================*\
* FILE: dc_interface_handcode.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains interface between DC and SG handcoded.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_interface_handcode.h"

namespace sg
{
   namespace dc
   {
      void DCInterfaceHandcode::step(TimingInfo &m_timing_info,
                                     DCContourStorage &dc_contour_list,
                                     CriticalRegion &critical_region,
                                     ContourStorage &contour_list,
                                     const rot::F360_Detection_Log_Output_T &rot_detections,
                                     const SG_Input_Detections_T &rspp_detections,
                                     const float elapsed_time,
                                     const RSPP_Host_T &host,
                                     const HostProps &host_props,
                                     const Drivability_Classification_Calibrations_T &cfg)
      {
         subsegmentController.step(m_timing_info, dc_contour_list, critical_region, contour_list, rot_detections, rspp_detections,
                                   elapsed_time, host, host_props, cfg);
      }
   }
}
