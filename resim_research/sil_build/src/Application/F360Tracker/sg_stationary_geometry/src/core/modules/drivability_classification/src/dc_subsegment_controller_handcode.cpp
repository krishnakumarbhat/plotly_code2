/*===================================================================================*\
* FILE: dc_subsegment_controller_handcode.cpp
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

#include "dc_subsegment_controller_handcode.h"

#include "dc_assign_detections_to_subsegments_and_update_features.h"
#include "dc_critical_region.h"
#include "dc_time_update_subsegments.h"
#include "dc_update_subsegments.h"
#include "dc_update_subsegments_drivability.h"

namespace sg
{
   namespace dc
   {
      void SubsegmentControllerHandcode::step(TimingInfo &m_timing_info,
                                              DCContourStorage &dc_contour_list,
                                              CriticalRegion &critical_region,
                                              const ContourStorage &contour_list,
                                              const rot::F360_Detection_Log_Output_T &rot_detections,
                                              const SG_Input_Detections_T &rspp_detections,
                                              const float elapsed_time,
                                              const RSPP_Host_T &host,
                                              const HostProps &host_props,
                                              const Drivability_Classification_Calibrations_T &cfg)
      {
         if (cfg.create_subsegments)
         {
            auto start_time = m_timing_info.elapsed();
            TimeUpdateSubsegments::timeUpdateSubsegments(dc_contour_list, elapsed_time, host.speed, host_props.cos_delta_pointing,
                                                         host_props.sin_delta_pointing);
            m_timing_info.details.dc_steps[static_cast<uint8_t>(DC_AlgorithmStep_T::TIME_UPDATE_SUBSEGMENTS)] =
               m_timing_info.elapsed() - start_time;

            start_time = m_timing_info.elapsed();
            // we need to change host data from VCS to ISO CS
            const float host_curvature = static_cast<float>(-host.curvature_rear);
            critical_region.create(host_curvature, cfg);
            m_timing_info.details.dc_steps[static_cast<uint8_t>(DC_AlgorithmStep_T::CREATE_CRITICAL_REGION)] =
               m_timing_info.elapsed() - start_time;

            start_time = m_timing_info.elapsed();
            DC_Update_Subsegments::update_subsegments(dc_contour_list, contour_list, critical_region, cfg);
            m_timing_info.details.dc_steps[static_cast<uint8_t>(DC_AlgorithmStep_T::UPDATE_SUBSEGMENTS)] =
               m_timing_info.elapsed() - start_time;

            if (cfg.classify_subsegments)
            {
               start_time = m_timing_info.elapsed();
               assign_detections_to_subsegments_and_update_features(dc_contour_list, rot_detections, rspp_detections,
                                                                    host.dist_rear_axle_to_vcs_m, cfg);
               m_timing_info.details.dc_steps[static_cast<uint8_t>(DC_AlgorithmStep_T::ASSIGN_DETECTIONS_AND_UPDATE_FEATURES)] =
                  m_timing_info.elapsed() - start_time;

               start_time = m_timing_info.elapsed();
               DC_Update_Subsegments_Drivability::update_subsegments_drivability(dc_contour_list, cfg);
               m_timing_info.details.dc_steps[static_cast<uint8_t>(DC_AlgorithmStep_T::UPDATE_SUBSEGMENTS_DRIVABILITY)] =
                  m_timing_info.elapsed() - start_time;
            }
         }
      }
   }
}
