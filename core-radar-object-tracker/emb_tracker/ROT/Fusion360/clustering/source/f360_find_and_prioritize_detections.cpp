/*===================================================================================*\
 * FILE: f360_find_and_prioritize_detections.cpp
 *====================================================================================
 * Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose."
 *-----------------------------------------------------------------------------------------
 * DESCRIPTION:
 * The file contains the definition of functions for clustering of detections
 *
 *
 * Applicable Standards (in order of precedence: highest first):
 *     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
 *     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
 ***/

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "f360_find_and_prioritize_detections.h"
#include "f360_calculate_priority.h"

namespace f360_variant_A
{
/*===========================================================================*\
* FUNCTION: Find_And_Prioritize_Detections()
*===========================================================================
* RETURN VALUE:
* None
*
* PARAMETERS:
* const F360_Detection_List_T& raw_detection_list                   - Raw detection list
* const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]       - Sensor array
* const F360_Host_T& host                                          - Host vehicle properties
* const F360_Detection_Props_List_T& detection_props_list           - Detection properties
* const bool f_cluster_moving                                      - True for moving detections, false for ambiguous
* const float32_t cipv_long_pos                                     - Cached CIPV longitudinal VCS position [m]
* int16_t &valid_det_count                                         - Output: number of valid detections
* int16_t(&sorted_det_idxs)[MAX_NUMBER_OF_DETECTIONS]              - Output: detection indices sorted by position priority
descending
* bool(&valid_dets)[MAX_NUMBER_OF_DETECTIONS]                      - Output: validity flags per detection index
*
* EXTERNAL REFERENCES:
* Detection_Clustering_Validity_Check(), Compute_Detection_Position_Priority(),
* F360_Sort(), Map_Curvature_For_Priority()
*
* DEVIATIONS FROM STANDARDS:
* None.
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* This function collects detections valid for clustering, computes their
* position-based priority, and sorts them by priority in descending order.
* Higher priority detections are processed first during clustering.
*
* Priority is computed only for valid detections (not for all detections),
* using Calculate_Position_Priority based on VCS position and curvature.
*
* PRECONDITIONS:
* Detection VCS positions must be computed before calling this function.
*
* POSTCONDITIONS:
* sorted_det_idxs contains valid detection indices sorted by position
* priority descending. valid_dets marks which detections are valid for
* clustering.
*
\*===========================================================================*/
void Find_And_Prioritize_Detections(const F360_Detection_List_T &raw_detection_list,
                                    const F360_Radar_Sensor_T (&sensors)[MAX_NUMBER_OF_SENSORS], const F360_Host_T &host,
                                    const F360_Detection_Props_List_T &detection_props_list, const bool f_cluster_moving,
                                    const float32_t cipv_long_pos, int16_t &valid_det_count,
                                    int16_t (&sorted_det_idxs)[MAX_NUMBER_OF_DETECTIONS],
                                    bool (&valid_dets)[MAX_NUMBER_OF_DETECTIONS])
{
   valid_det_count = 0;
   (void)memset(valid_dets, 0, sizeof(valid_dets));

   // Collect valid detection indices
   for (uint32_t i = 0U; i < raw_detection_list.number_of_valid_detections; i++)
   {
      const int16_t det_idx    = static_cast<int16_t>(i);
      const int32_t sensor_idx = raw_detection_list.detections[det_idx].sensor_id - 1;
      const bool f_det_valid_for_clustering =
         Detection_Clustering_Validity_Check(sensors[sensor_idx], detection_props_list.detection_props[det_idx],
                                             raw_detection_list.detections[det_idx], f_cluster_moving, host.vcs_speed);

      if (f_det_valid_for_clustering)
      {
         sorted_det_idxs[valid_det_count] = det_idx;
         valid_dets[det_idx]              = true;
         valid_det_count++;
      }
   }

   // Compute position priority and sort valid detections descending (merge sort)
   const float32_t curvature = Map_Curvature_For_Priority(host.curvature_rear);
   float32_t prio_values[MAX_NUMBER_OF_DETECTIONS];
   uint32_t sort_perm[MAX_NUMBER_OF_DETECTIONS];
   int16_t unsorted_det_idxs[MAX_NUMBER_OF_DETECTIONS];

   for (int16_t i = 0; i < valid_det_count; i++)
   {
      const int16_t idx = sorted_det_idxs[i];
      prio_values[i]    = Compute_Detection_Position_Priority(
            raw_detection_list.detections[idx], detection_props_list.detection_props[idx], host.vcs_speed, cipv_long_pos, curvature);
      unsorted_det_idxs[i] = idx;
   }

   (void)F360_Sort(static_cast<uint32_t>(valid_det_count), false, prio_values, sort_perm);
   for (int16_t i = 0; i < valid_det_count; i++)
   {
      sorted_det_idxs[i] = unsorted_det_idxs[sort_perm[i]];
   }
}

/*===========================================================================*\
* FUNCTION: Detection_Clustering_Validity_Check()
*===========================================================================
* ABSTRACT:
* --------------------------------------------------------------------------
* Determine if a detection is fulfilling the criteria for clustering, and
* eventually track initialization.
\*===========================================================================*/
bool Detection_Clustering_Validity_Check(const F360_Radar_Sensor_T &sensor, const F360_Detection_Props_T &det_p,
                                         const F360_Detection_T &det, const bool f_cluster_moving, const float32_t host_vcs_speed)
{
   constexpr float32_t k_mrr360_max_abs_elev_angle_rad = F360_DEG2RAD(6.0F);
   const float32_t k_mrr360_min_host_speed_el_check    = 2.0F;

   const bool f_preconditions_moving = f_cluster_moving && (det_p.motion_status == F360_DETECTION_MOTION_STATUS_MOVING) &&
                                       (det_p.on_sep_id == F360_INVALID_UNSIGNED_ID);
   const bool f_preconditions_ambig = (!f_cluster_moving) && (0 == det_p.cluster_id);

   const bool result =
      (f_preconditions_ambig || f_preconditions_moving) && (det_p.object_track_id == 0) && (det_p.f_ok_to_use) &&
      (!det_p.f_double_bounce) && (!det_p.f_close_target) && (!det_p.f_det_pair) && (!det_p.f_FOV_edge) &&
      (!det_p.f_trailer_related_det) // block the creation of objects if the det is trailer related, while it is still possible to
                                     // be associated before clustering
      && (!det_p.f_water_spray) && (!det_p.f_unreliable_in_clutter) && (!det_p.f_low_az_conf_det) && (!det_p.f_angle_amb) &&
      (!det.f_host_veh_clutter) && (!det.f_bistatic) && (!det_p.f_nd_target) &&
      (F360_DETECTION_WHEELSPIN_TYPE_INVALID == det_p.wheel_spin_type) &&
      (!((fabsf(det.elevation) > k_mrr360_max_abs_elev_angle_rad) && (sensor.sensor_type == F360_SENSOR_TYPE_MRR360_RADAR) &&
         (host_vcs_speed > k_mrr360_min_host_speed_el_check)));

   return result;
}

} // namespace f360_variant_A
