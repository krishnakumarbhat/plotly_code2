/*===================================================================================*\
* FILE: f360_calc_obj_height.cpp
*====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*
* 
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_calc_obj_height.h"
#include "f360_reuse.h"
#include "f360_math.h"
#include "f360_calculate_curvi_position.h"

namespace f360_variant_A
{
   static bool Stationary_Object_In_Zone(const Point& vcs_position, const F360_Host_T& host);

   /*===========================================================================*\
   * FUNCTION: Calc_Obj_Height
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_T(&dets)[MAX_NUMBER_OF_DETECTIONS],
   * const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]
   * const F360_Host_T& host
   * F360_Object_Track_T& object_track
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * Function calculates height over time primarily for moving objects but also
   * for stationary objects in a zone relevant for LSC clustering.
   * --------------------------------------------------------------------------
   * 
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   void Calc_Obj_Height(
      const rspp_variant_A::RSPP_Detection_T(&dets)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const F360_Host_T& host,
      F360_Object_Track_T& object_track)
   {
      uint32_t n_valid_dets = 0U;
      float32_t det_heights_sum = 0.0F;
      uint32_t num_dets_below_ground = 0U;

      if ((object_track.speed > 5.0F) &&
         (fabsf(object_track.vcs_velocity.longitudinal) > fabsf(object_track.vcs_velocity.lateral * 2.0F)))
      {
         // Objects moving in largely longitudinal direction with some speed
         for (uint32_t i = 0U; i < object_track.ndets; i++)
         {
            const uint32_t det_idx = object_track.detids[i] - 1U;
            if ((det_props[det_idx].motion_status == rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING) &&
               (dets[det_idx].raw.confid_azimuth != rspp_variant_A::RSPP_CONF_AZIMUTH_LOW) &&
               (dets[det_idx].raw.confid_elevation != rspp_variant_A::RSPP_CONF_AZIMUTH_LOW))
            {
               // Use signed z-position
               det_heights_sum += dets[det_idx].processed.vcs_position_z;
               n_valid_dets++;
            }
         }
      }
      else 
      {
         // Stationary and all other moving objects
         if(object_track.f_moving || Stationary_Object_In_Zone(object_track.vcs_position, host))
         {
            for (uint32_t i = 0U; i < object_track.ndets; i++)
            {
               const uint32_t det_idx = object_track.detids[i] - 1U;
               if (dets[det_idx].raw.confid_azimuth != rspp_variant_A::RSPP_CONF_AZIMUTH_LOW)
               {
                  // Use abs value of z-position
                  det_heights_sum += std::abs(dets[det_idx].processed.vcs_position_z);
                  n_valid_dets++;

                  // Calculate number of detections below ground,The z-plane is defined at ground level and negative above ground
                  if (dets[det_idx].processed.vcs_position_z > 0.0F)
                  {
                     num_dets_below_ground++;
                  }
               }
            }
         }
      }

      const float32_t k_hist_det_forgetting_factor = 0.97F;
      const float32_t k_max_historic_dets = 80.0F;

      object_track.ud_mov_historic_ndets = object_track.ud_mov_historic_ndets * k_hist_det_forgetting_factor;
      const float32_t sum_ndets = object_track.ud_mov_historic_ndets + static_cast<float32_t>(n_valid_dets);
      const float32_t sum_heights = object_track.ud_mov_historic_ndets * object_track.otg_height_raw + det_heights_sum;

      if (sum_ndets > F360_EPSILON)
      {
         object_track.otg_height_raw = sum_heights / sum_ndets;
         // update percentage of detections below ground
         object_track.ud_overdrivable_det_pct = (object_track.ud_mov_historic_ndets * object_track.ud_overdrivable_det_pct + 
            static_cast<float32_t>(num_dets_below_ground)) / sum_ndets;
      }

      object_track.otg_height = fabsf(object_track.otg_height_raw);
      object_track.ud_mov_historic_ndets = std::min(sum_ndets, k_max_historic_dets); // if this saturation is changed make sure to check intended use case in f360_classification_underdrivability_moving.cpp 

   }
   /*===========================================================================*\
   * FUNCTION: Stationary_Object_In_Zone
   *===========================================================================
   * RETURN VALUE:
   * bool
   *
   * PARAMETERS:
   * const F360_Object_Track_T& object_track
   * const F360_Host_T& host
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * This function check if an object is within in the zone that otg_height is 
   * calculated for stationary objects.
   * --------------------------------------------------------------------------
   * 
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   static bool Stationary_Object_In_Zone(const Point& vcs_position, const F360_Host_T& host)
   {
      // Zone boundaries
      constexpr float32_t k_max_x_pos = 80.0F;
      constexpr float32_t k_max_y_pos_zone_front = 30.0F;
      constexpr float32_t k_max_y_pos_zone_rear = 10.0F;
      // Two zones, one in the rear and one in the front:
      // 1. Rear zone:
      // x: [-80m, 0m]
      // y: [-10m, 10m]
      
      // 2. Front zone:
      // x: [0m, 80m]
      // y: [-30m, 30m]
      
      // Calculate absolute curvilinear lateral position of object
      const float32_t curvi_latpos = std::abs(Calculate_Curvi_Lat_Pos(host, vcs_position.x, vcs_position.y));

      const bool f_in_zone_front = 
         (vcs_position.x < k_max_x_pos) && 
         (vcs_position.x > 0.0F) &&
         (curvi_latpos < k_max_y_pos_zone_front);

      const bool f_in_zone_rear = 
         (vcs_position.x > -k_max_x_pos) &&
         (vcs_position.x <= 0.0F) && 
         (curvi_latpos < k_max_y_pos_zone_rear);

      // True if object is either in front or rear zone
      return f_in_zone_front || f_in_zone_rear;
   }
}
