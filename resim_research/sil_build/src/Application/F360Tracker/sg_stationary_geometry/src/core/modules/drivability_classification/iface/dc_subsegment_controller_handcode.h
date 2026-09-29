/*===================================================================================*\
* FILE: dc_subsegment_controller_handcode.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of SubsegmentCotrollerHandcode class which controls flow of DC functions.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef DC_SUBSEGMENT_CONTROLLER_HANDCODE_H
#define DC_SUBSEGMENT_CONTROLLER_HANDCODE_H

#include "dc_contour_storage.h"
#include "dc_critical_region.h"
#include "geometry/geo_point.h"
#include "rspp_host.h"
#include "sg_calibrations.h"
#include "sg_contour_storage.h"
#include "sg_host_props.h"
#include "sg_input.h"
#include "sg_reuse.h"
#include "sg_timing_info.h"

namespace sg
{
   namespace dc
   {
      class SubsegmentControllerHandcode
      {
        public:
         SubsegmentControllerHandcode() = default;

         /**
          * @brief    Executes one iteration of the algorithm.
          *
          * @param[in, out]   m_timing_info - runtime measurement objects
          * @param[in, out]   dc_contour_list - container of DC contours
          * @param[in, out]   critical_region - critical region object
          * @param[in]        contour_list - container of SG contours
          * @param[in]        rot_detections - list of ROT detections
          * @param[in]        rspp_detections - list of RSPP detections
          * @param[in]        elapsed_time - elapsed time between two scan indices
          * @param[in]        host - host data storage
          * @param[in]        host_props - properties of host
          * @param[in]        cfg - configuration data
          *
          **/
         void step(TimingInfo &m_timing_info,
                   DCContourStorage &dc_contour_list,
                   CriticalRegion &critical_region,
                   const ContourStorage &contour_list,
                   const rot::F360_Detection_Log_Output_T &rot_detections,
                   const SG_Input_Detections_T &rspp_detections,
                   const float elapsed_time,
                   const RSPP_Host_T &host,
                   const HostProps &host_props,
                   const Drivability_Classification_Calibrations_T &cfg);
      };
   }
}

#endif
