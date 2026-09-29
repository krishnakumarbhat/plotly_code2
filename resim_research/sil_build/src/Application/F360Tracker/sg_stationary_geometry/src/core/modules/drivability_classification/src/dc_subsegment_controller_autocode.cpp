/*===================================================================================*\
* FILE: dc_subsegment_controller.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration of SubsegmentCotroller class which controls flow of DC functions.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_subsegment_controller_autocode.h"

#include "assign_detections_to_subsegments_and_update_features.h"
#include "create_critical_region.h"
#include "sg_algorithm_step.h"
#include "sg_stationary_geometries.h"
#include "time_update_subsegments.h"
#include "update_subsegments.h"
#include "update_subsegments_drivability.h"

namespace sg
{
   namespace dc
   {
      void SubsegmentControllerAutocode::create_critical_region(
         float (&critical_region)[DC_MAX_COORDINATES_REGION_SIZE][DC_COORDINATES_DIMENSION],
         const float curvature,
         const MATLAB::Calibrations_T &cfg) const
      {
         // check if produced number of vertices will be below threshold
         constexpr uint8_t base_critical_region_size = sizeof(cfg.critical_region_polygon[0]) / sizeof(float);
         (void) base_critical_region_size;
         assert((base_critical_region_size * static_cast<uint8_t>(cfg.critical_region_subdivisions)
                 - 2U * (static_cast<uint8_t>(cfg.critical_region_subdivisions) - 1U))
                <= DC_MAX_COORDINATES_REGION_SIZE);

         float region_data[DC_MAX_COORDINATES_REGION_SIZE * DC_COORDINATES_DIMENSION];
         int region_size[DC_COORDINATES_DIMENSION];
         MATLAB::create_critical_region(-1.0F * curvature, &cfg, region_data, region_size);

         const uint8_t num_valid_vertices = static_cast<uint8_t>(region_size[1U]);
         if (num_valid_vertices > 0U)
         {
            for (uint8_t i{0U}; i < DC_MAX_COORDINATES_REGION_SIZE; ++i)
            {
               if (i < num_valid_vertices) // valid vertex
               {
                  critical_region[i][0U] = region_data[DC_COORDINATES_DIMENSION * i];
                  critical_region[i][1U] = region_data[DC_COORDINATES_DIMENSION * i + 1U];
               }
               else // assign position of the last valid vertex to the invalid vertices
                    // it is an error prevention if it's used somewhere with all its points
               {
                  critical_region[i][0U] = critical_region[num_valid_vertices - 1U][0U];
                  critical_region[i][1U] = critical_region[num_valid_vertices - 1U][1U];
               }
            }
         }
      }

      void SubsegmentControllerAutocode::step(TimingInfo &m_timing_info,
                                              MATLAB::Internals_T &matlab_internals,
                                              const MATLAB::Host_T &host,
                                              const MATLAB::Input_Detections_List_T &detections_list,
                                              const MATLAB::Calibrations_T &cfg)
      {
         if (cfg.create_subsegments)
         {
            auto start_time = m_timing_info.elapsed();
            MATLAB::time_update_subsegments(matlab_internals.DC_contours, &host);
            m_timing_info.details.dc_steps[static_cast<uint8_t>(DC_AlgorithmStep_T::TIME_UPDATE_SUBSEGMENTS)] =
               m_timing_info.elapsed() - start_time;

            start_time = m_timing_info.elapsed();
            create_critical_region(matlab_internals.critical_region, host.curvature, cfg);
            m_timing_info.details.dc_steps[static_cast<uint8_t>(DC_AlgorithmStep_T::CREATE_CRITICAL_REGION)] =
               m_timing_info.elapsed() - start_time;

            start_time = m_timing_info.elapsed();
            MATLAB::update_subsegments(matlab_internals.contours, matlab_internals.DC_contours, matlab_internals.critical_region,
                                       &cfg);
            m_timing_info.details.dc_steps[static_cast<uint8_t>(DC_AlgorithmStep_T::UPDATE_SUBSEGMENTS)] =
               m_timing_info.elapsed() - start_time;

            if (cfg.classify_subsegments)
            {
               start_time = m_timing_info.elapsed();
               MATLAB::assign_detections_to_subsegments_and_update_features(matlab_internals.DC_contours, &detections_list,
                                                                            host.dist_rear_axle_to_vcs, &cfg);
               m_timing_info.details.dc_steps[static_cast<uint8_t>(DC_AlgorithmStep_T::ASSIGN_DETECTIONS_AND_UPDATE_FEATURES)] =
                  m_timing_info.elapsed() - start_time;

               start_time = m_timing_info.elapsed();
               MATLAB::update_subsegments_drivability(matlab_internals.DC_contours, &cfg);
               m_timing_info.details.dc_steps[static_cast<uint8_t>(DC_AlgorithmStep_T::UPDATE_SUBSEGMENTS_DRIVABILITY)] =
                  m_timing_info.elapsed() - start_time;
            }
         }
      }
   }
}
