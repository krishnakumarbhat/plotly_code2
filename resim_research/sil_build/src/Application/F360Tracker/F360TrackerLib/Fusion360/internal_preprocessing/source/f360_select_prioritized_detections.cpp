/*===================================================================================\
 * FILE: f360_select_prioritized_detections.cpp
 *====================================================================================
 * Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
 *------------------------------------------------------------------------------------
 *
 * DESCRIPTION:
 *   This file contains function definition for Select_Prioritized_Detections.
 *
 * Applicable Standards (in order of precedence: highest first):
 *   ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
 *   ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
 *
\*===================================================================================*/

#include "f360_select_prioritized_detections.h"
#include "f360_detection_priority.h"
#include "f360_math_func.h"
#include "f360_calculate_curvi_position.h"

namespace f360_variant_A
{
   void Select_Prioritized_Detections(
      const rspp_variant_A::RSPP_Detection_List_T& det_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Host_T& host,
      F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      for (int32_t i = 0; i < static_cast<int32_t>(MAX_NUMBER_OF_SENSORS); i++)
      {
         float32_t priority[MAX_DETS_FOR_SINGLE_SENSOR];
         uint32_t det_idx_arr[MAX_DETS_FOR_SINGLE_SENSOR];
         /* Calculate priority of detections by sensor */
         uint32_t n_dets = 0U;
         for (uint32_t j = 0U; j < det_list.number_of_valid_detections; j++)
         {
            const rspp_variant_A::RSPP_Detection_T* const p_det = &det_list.detections[j];
            if (p_det->raw.sensor_id == i + 1)
            {
               constexpr float32_t host_curv_threhsold = 0.025F;
               const F360_Radar_Sensor_T* const p_sensor = &sensors[p_det->raw.sensor_id - 1];
               const float32_t vun = p_sensor->constant.v_wrapping[p_sensor->variable.look_id];
               det_idx_arr[n_dets] = j;
               float32_t curvi_lat_pos = INFTY;
               if ((detection_props[j].motion_status == rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING) && (std::abs(host.curvature_rear) < host_curv_threhsold))
               {
                  curvi_lat_pos = Calculate_Curvi_Lat_Pos(host, detection_props[j].vcs_position.x, detection_props[j].vcs_position.y);
               }
               priority[n_dets] = Detection_Priority(p_det->raw.range, 
                                                     p_det->processed.vcs_az, 
                                                     p_det->raw.range_rate, 
                                                     host.speed, 
                                                     sensors->constant.min_aliaised_range_rate[p_sensor->variable.look_id], 
                                                     vun, 
                                                     curvi_lat_pos);
               n_dets++;
            }
         }

         if (n_dets > 0U)
         {
            uint32_t num_requested_dets;
            switch (sensors[i].constant.mounting_location)
            {
               case F360_MOUNTING_LOCATION_RIGHT_FORWARD:
               case F360_MOUNTING_LOCATION_LEFT_FORWARD:
               case F360_MOUNTING_LOCATION_RIGHT_REAR:
               case F360_MOUNTING_LOCATION_LEFT_REAR:
                  num_requested_dets = 200U;
                  break;
               default:
                  num_requested_dets = 0U;
                  break;
            }

            /* Sort the detections if required */
            if (n_dets > num_requested_dets)
            {
               uint32_t perm[MAX_DETS_FOR_SINGLE_SENSOR];
               (void)F360_Sort(n_dets, false, priority, perm);

               for (uint32_t j = num_requested_dets; j < n_dets; j++)
               {
                  const uint32_t det_idx = det_idx_arr[perm[j]];
                  detection_props[det_idx].f_ok_to_use = false;
               }
            }
         }
      }
   }
}
