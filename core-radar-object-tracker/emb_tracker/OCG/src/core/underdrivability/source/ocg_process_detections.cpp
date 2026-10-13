/*===================================================================================*\
* FILE: ocg_process_detections.cpp
*====================================================================================
* Copyright (C) 2026 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Process_Detections() function definition
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "ocg_process_detections.h"
#include <algorithm>
#include <cmath>

namespace ocg
{
/*===========================================================================*\
* FUNCTION: Process_Detections()
*===========================================================================
* RETURN VALUE:
* None
*
* PARAMETERS:
* const OCG_Host_T& host,
* const OCG_Sensor_T& sensor,
* OCG_Detection_List_T& detection_list
* OCG_Detection_Properties_T(&det_props)[OCG_NUM_DETS]
*
* EXTERNAL REFERENCES:
* None.
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Process Detections definition.
*
* PRECONDITIONS:
*
* POSTCONDITIONS:
* None
*
\*===========================================================================*/
void Process_Detections(const OCG_Host_T &host, const OCG_Sensor_T &sensor, const OCG_Detection_List_T &detection_list,
                        OCG_Detection_Properties_T (&det_props)[OCG_NUM_DETS])
{
   const float vwrapping          = sensor.v_wrapping;
   const float half_vwrapping     = vwrapping * 0.5F;
   const float forward_host_speed = host.speed * cosf(host.vcs_sideslip);
   const int16_t num_dets         = std::min(detection_list.number_of_valid_detections, OCG_NUM_DETS);

   for (int16_t i = 0; i < num_dets; i++)
   {
      const OCG_Detection_T &det           = detection_list.detections[i];
      OCG_Detection_Properties_T &det_prop = det_props[i];
      const float pol                      = (sensor.polarity < 0.0F) ? -1.0F : 1.0F;
      const float det_az                   = det.azimuth;
      const float det_el                   = det.elevation;
      const float det_rng                  = det.range;
      const float cos_az                   = cosf(det_az);
      const float sin_az                   = sinf(det_az);
      const float sin_el                   = sinf(det_el);
      float range_rate_comp                = det.range_rate + cos_az * forward_host_speed;

      det_prop.vcs_position_x = cos_az * det_rng + sensor.vcs_position_x;
      det_prop.vcs_position_y = sin_az * det_rng * pol + sensor.vcs_position_y;
      det_prop.vcs_position_z = sin_el * det_rng * pol + sensor.vcs_position_z;

      // Unfolding towards a stationary hypothesis
      for (int8_t j = 0; j < 5; j++)
      {
         if (range_rate_comp < half_vwrapping)
         {
            range_rate_comp += vwrapping;
         }
         else if (range_rate_comp >= half_vwrapping)
         {
            range_rate_comp -= vwrapping;
         }
         else
         {
            break;
         }
      }
      det_prop.range_rate_compensated = range_rate_comp;
   }
}
} // namespace ocg
