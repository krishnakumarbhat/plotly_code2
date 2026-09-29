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
#include "f360_math_func.h"
#include "f360_calculate_curvi_position.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Find_And_Prioritize_Detections()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * const F360_Host_T& host,
   * F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
   * int32_t &valid_det_count,
   * int16_t(&sorted_det_idxs)[MAX_NUMBER_OF_DETECTIONS],
   * bool(&valid_dets)[MAX_NUMBER_OF_DETECTIONS]
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
   * This function sorts list of detection indexes based on detection
   * vcs position. There are five priority zones in vcs coordinates:
   * first - in front of the host within pos.y threshold
   * second - behind the host within pos.y threshold
   * third - within semicircle radius in front of the host
   * fourth - rest of detections in front of the host
   * fifth - rest of detections behind the host
   * 
   * Within single zone detections are sorted by longitudinal position.
   * 
   * PRECONDITIONS:
   * All the Pointers should Point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Find_And_Prioritize_Detections(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Host_T& host,
      const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS],
      const bool f_cluster_moving,
      int16_t &valid_det_count,
      int16_t(&sorted_det_idxs)[MAX_NUMBER_OF_DETECTIONS],
      bool(&valid_dets)[MAX_NUMBER_OF_DETECTIONS])
   {
      const float32_t pos_y_threshold = 20.0F;
      int16_t fourth_fifth_priority_zone[MAX_NUMBER_OF_DETECTIONS];
      int16_t third_priority_zone[MAX_NUMBER_OF_DETECTIONS];
      int16_t first_second_priority_zone_det_count = 0;
      int16_t fourth_fifth_priority_zone_det_count = 0;
      int16_t third_priority_zone_det_count = 0;
      (void)memset(valid_dets, 0, sizeof(valid_dets));

      const Traverse_Starting_Det_Indexes det_starting_indexes = Find_Forward_And_Backward_Starting_Det_Indexes(raw_detection_list, detection_props);

      // Traverse forward
      int16_t det_idx = det_starting_indexes.forward_det_idx;
      for (uint32_t i = 0U; i < raw_detection_list.number_of_valid_detections; i++)
      {
         if (det_idx != F360_INVALID_ID)
         {
            const int32_t sensor_idx = raw_detection_list.detections[det_idx].raw.sensor_id - 1;
            const bool f_det_valid_for_clustering = Detection_Clustering_Validity_Check(sensors[sensor_idx], detection_props[det_idx], raw_detection_list.detections[det_idx], f_cluster_moving, host.vcs_speed);

            if (f_det_valid_for_clustering)
            {
               const float32_t curvi_lat_pos = Calculate_Curvi_Lat_Pos(host, detection_props[det_idx].vcs_position.x, detection_props[det_idx].vcs_position.y);
               const bool f_det_within_first_zone = fabsf(curvi_lat_pos) < pos_y_threshold;

               //first priority zone
               if (f_det_within_first_zone)
               {
                  sorted_det_idxs[first_second_priority_zone_det_count] = det_idx;
                  first_second_priority_zone_det_count++;
                  valid_dets[det_idx] = true;
               }

               //third priority zone
               else if (Is_Detection_In_Third_Priority_Zone(detection_props[det_idx], host))
               {
                  third_priority_zone[third_priority_zone_det_count] = det_idx;
                  third_priority_zone_det_count++;
                  valid_dets[det_idx] = true;
               }

               //fourth priority zone
               else
               {
                  fourth_fifth_priority_zone[fourth_fifth_priority_zone_det_count] = det_idx;
                  fourth_fifth_priority_zone_det_count++;
                  valid_dets[det_idx] = true;
               }
            }
            det_idx = raw_detection_list.detections[det_idx].processed.next_sorted_idx;
         }
         else
         {
            // Reached vcslong positive end
            break;
         }
      }

      // Traverse backwards
      det_idx = det_starting_indexes.backward_det_idx;
      for (uint32_t i = 0U; i < raw_detection_list.number_of_valid_detections; i++)
      {
         if (det_idx != F360_INVALID_ID)
         {
            const int32_t sensor_idx = raw_detection_list.detections[det_idx].raw.sensor_id - 1;
            const bool f_det_valid_for_clustering = Detection_Clustering_Validity_Check(sensors[sensor_idx], detection_props[det_idx], raw_detection_list.detections[det_idx], f_cluster_moving, host.vcs_speed);

            if (f_det_valid_for_clustering)
            {
               const bool f_det_within_second_zone = fabsf(detection_props[det_idx].vcs_position.y) < pos_y_threshold;

               //second priority zone
               if (f_det_within_second_zone)
               {
                  sorted_det_idxs[first_second_priority_zone_det_count] = det_idx;
                  first_second_priority_zone_det_count++;
                  valid_dets[det_idx] = true;
               }

               //fifth priority zone
               else
               {
                  fourth_fifth_priority_zone[fourth_fifth_priority_zone_det_count] = det_idx;
                  fourth_fifth_priority_zone_det_count++;
                  valid_dets[det_idx] = true;
               }
            }
            det_idx = raw_detection_list.detections[det_idx].processed.prev_sorted_idx;
         }
         else
         {
            // Reached vcslong negative end
            break;
         }
      }

      // Concate priority lists
      valid_det_count = first_second_priority_zone_det_count + third_priority_zone_det_count + fourth_fifth_priority_zone_det_count;
      for (int16_t i = 0; i < third_priority_zone_det_count; i++)
      {
         sorted_det_idxs[(first_second_priority_zone_det_count + i)] = third_priority_zone[i];
      }
      for (int16_t i = 0; i < fourth_fifth_priority_zone_det_count; i++)
      {
         sorted_det_idxs[(first_second_priority_zone_det_count + third_priority_zone_det_count + i)] = fourth_fifth_priority_zone[i];
      }
   }

   /*===========================================================================*\
   * FUNCTION: Is_Detection_In_Third_Priority_Zone()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
   * const F360_Host_T& host,
   * const int16_t &det_idx
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
   * This function checks if given detection is inside third priority zone thresholds.
   *
   * PRECONDITIONS:
   * All the Pointers should Point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   bool Is_Detection_In_Third_Priority_Zone(
      const F360_Detection_Props_T& det_p,
      const F360_Host_T& host)
   {
      constexpr float32_t third_priority_zone_ttc_sq = 25.0F;
      constexpr float32_t min_semicircle_radius = 20.0F;
      const float32_t det_sq_dist = det_p.vcs_position.Distance_To_Origin_Squared();
      const float32_t semicircle_sq_radius = third_priority_zone_ttc_sq * host.vcs_speed * host.vcs_speed;
      const float32_t saturated_semicircle_sq_radius = Clamp(semicircle_sq_radius, min_semicircle_radius, INFTY);
      return det_sq_dist < saturated_semicircle_sq_radius;
   }

   /*===========================================================================*\
   * FUNCTION: Find_Forward_And_Backward_Starting_Det_Indexes()
   *===========================================================================
   * RETURN VALUE:
   * Traverse_Starting_Det_Indexes det_starting_indexes
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list
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
   * This function find the detections which are closest to +-vcs longposition 0.
   *
   * If there are no detections >= 0, the forward_traverse_starting_det_idx 
   * will be F360_INVALID_ID and backward_traverse_starting_det_idx will be the
   * the detections with the largest vcs long position.
   * 
   * If there are no detections at all, both forward_traverse_starting_det_idx
   * and backward_traverse_starting_det_idx will be F360_INVALID_ID
   *
   * PRECONDITIONS:
   * raw_detection_list.vcslong_det_idx_min is set as intended, i.e. it is either
   * a valid index or F360_INVALID_ID.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   Traverse_Starting_Det_Indexes Find_Forward_And_Backward_Starting_Det_Indexes(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const F360_Detection_Props_T(&detection_props)[MAX_NUMBER_OF_DETECTIONS])
   {
      constexpr float32_t det_starting_xpos = 0.0F;

      // Find forward/backward starting positions
      Traverse_Starting_Det_Indexes det_starting_indexes;
      det_starting_indexes.backward_det_idx = static_cast<int16_t>(F360_INVALID_ID);
      det_starting_indexes.forward_det_idx = raw_detection_list.vcslong_det_idx_min;

      for (uint32_t i = 0U; i < raw_detection_list.number_of_valid_detections; i++)
      {
         if (det_starting_indexes.forward_det_idx != F360_INVALID_ID)
         {
            if (detection_props[det_starting_indexes.forward_det_idx].vcs_position.x >= det_starting_xpos)
            {
               // Reached desired starting position
               break;
            }
            det_starting_indexes.backward_det_idx = det_starting_indexes.forward_det_idx;
            det_starting_indexes.forward_det_idx = raw_detection_list.detections[det_starting_indexes.backward_det_idx].processed.next_sorted_idx;
         }
         else
         {
            // Reached end of list before finding the forward starting position
            // avoid using one more break to comply misra warning 6-4-4
            continue;
         }
      }

      return det_starting_indexes;
   }

   /*===========================================================================*\
   * FUNCTION: Detection_Clustering_Validity_Check()
   *===========================================================================
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Determine if a detection is fulfilling the criteria for clustering, and 
   * eventually track initialization.
   \*===========================================================================*/
   bool Detection_Clustering_Validity_Check(
      const F360_Radar_Sensor_T& sensor,
      const F360_Detection_Props_T& det_p,
      const rspp_variant_A::RSPP_Detection_T& det,
      const bool f_cluster_moving,
      const float32_t host_vcs_speed)
   {
      constexpr float32_t k_mrr360_max_abs_elev_angle_rad = F360_DEG2RAD(6.0F);
      const float32_t k_mrr360_min_host_speed_el_check = 2.0F;

      const bool f_preconditions_moving = f_cluster_moving && (det_p.motion_status == rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING) && (det_p.on_sep_id == F360_INVALID_UNSIGNED_ID);
      const bool f_preconditions_ambig = (!f_cluster_moving) && (0 == det_p.cluster_id);

      const bool result = (f_preconditions_ambig || f_preconditions_moving)
         && (det_p.object_track_id == 0)
         && (det_p.f_ok_to_use)
         && (!det_p.f_double_bounce)
         && (!det_p.f_close_target)
         && (!det_p.f_det_pair)
         && (!det_p.f_FOV_edge)
         && (!det_p.f_trailer_related_det)  // block the creation of objects if the det is trailer related, while it is still possible to be associated before clustering
         && (!det_p.f_water_spray)
         && (!det_p.f_unreliable_in_clutter)
         && (!det_p.f_low_az_conf_det)
         && (!det_p.f_angle_amb)
         && (!det.raw.f_host_veh_clutter)
         && (!det.raw.f_bistatic)
         && (!det_p.f_nd_target)
         && (F360_DETECTION_WHEELSPIN_TYPE_INVALID == det_p.wheel_spin_type)
         && (!((fabsf(det.raw.elevation) > k_mrr360_max_abs_elev_angle_rad) &&
            (sensor.constant.sensor_type == F360_SENSOR_TYPE_MRR360_RADAR) &&
            (host_vcs_speed > k_mrr360_min_host_speed_el_check)));

      return result;
   }
}
