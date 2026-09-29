/*===================================================================================*\
* FILE:  f360_calculate_priority.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function shared between object and cluster for calculating priority value
*
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
***/

#include "f360_calculate_priority.h"
#include "f360_calculate_curvi_position.h"
#include "f360_math_func.h"
#include <algorithm>

namespace f360_variant_A
{
   static float32_t Calculate_Headway_Priority(
      const F360_Host_T & host_props,
      const float32_t longitudal_pos,
      const float32_t lateral_pos);

   /*===========================================================================*\
   * FUNCTION: Calculate_Priority()
   *===========================================================================
   * RETURN VALUE:
   * none
   *
   * PARAMETERS:
   * const F360_Host_T* const host_props,
   * const float32_t movable_prob,
   * const float32_t confidence,
   * const float32_t longitudal_pos,
   * const float32_t lateral_pos
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
   * This function calculates basic priority value for the cluster or object with
   * given number of detection, position and velocity
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * confidence is in range <0, 1> - lower value means higher prioritized
   *
   \*===========================================================================*/
   float32_t Calculate_Priority(
      const F360_Host_T & host_props,
      const float32_t movable_prob,
      const float32_t confidence,
      const float32_t longitudal_pos,
      const float32_t lateral_pos)
   {
      const float32_t k_priority_distance_coefficient = 0.25F; // Priority Function: this coefficient is multiplied by distance priority, and then it's used for priority normalization
      const float32_t k_priority_confidence_coefficient = 0.1F;  // Priority Function: this coefficient is multiplied by number of detections priority, and then it's used for priority normalization
      const float32_t k_priority_headway_coefficient = 0.15F; // Priority Function: this coefficient is multiplied by headway priority, and then it's used for priority normalization
      const float32_t k_priority_f_movable_coefficient = 0.5F; // Priority Function: this coefficient is multiplied by movable priority, and then it's used for priority normalization

      const float32_t sum_of_weights = k_priority_distance_coefficient 
         + k_priority_confidence_coefficient 
         + k_priority_headway_coefficient 
         + k_priority_f_movable_coefficient;

      const float32_t k_priority_distance_for_min_priority_inverse = 1.0F / 200.0F; // Priority Function: inverse of distance after object do not gain distance priority (F360_MIN_PRIORITY)

      const float32_t headway_priority = Calculate_Headway_Priority(host_props, longitudal_pos, lateral_pos);
      const float32_t movable_priority = (movable_prob > 0.5F) ? F360_MAX_PRIORITY : F360_MIN_PRIORITY;
      const float32_t manhattan_distance = std::abs(longitudal_pos) + std::abs(lateral_pos);
      const float32_t distance_priority = std::max(F360_MIN_PRIORITY, (F360_MAX_PRIORITY - (manhattan_distance * k_priority_distance_for_min_priority_inverse)));

      float32_t priority = confidence * k_priority_confidence_coefficient;
      priority += distance_priority * k_priority_distance_coefficient;
      priority += headway_priority * k_priority_headway_coefficient;
      priority += movable_priority * k_priority_f_movable_coefficient;
      priority /= sum_of_weights;

      return priority;
   }


   /*===========================================================================*\
   * FUNCTION: Calculate_Priority_For_Cluster()
   *===========================================================================
   * RETURN VALUE:
   * none
   *
   * PARAMETERS:
   * const F360_Host_T& host,
   * const F360_Cluster_T& cluster
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
   * This function calculates basic priority value for a cluster. It uses the same
   * prioritization function/algorithm as for objects. However the cluster moving
   * probability and confidence is pre-computed before calling this function (since
   * these properties don't exist for clusters but only for objects).
   *
   * PRECONDITIONS:
   *
   * POSTCONDITIONS:
   * confidence is in range <0, 1> - lower value means higher prioritized
   *
   \*===========================================================================*/
   float32_t Calculate_Priority_For_Cluster(
      const F360_Host_T& host,
      const F360_Cluster_T& cluster)
   {
      const bool f_moving = (cluster.num_types_of_dets[0] > 0) || (cluster.f_dealiased && (std::abs(cluster.rep_rdotcomp) > 0.8F));
      const float32_t k_moving_prob = f_moving ? 1.0F : 0.0F;
      const float32_t k_confidence = 1.0F; // Same for all clusters because we don't have any cluster confidence signal

      const float32_t cluster_priority = Calculate_Priority(host, k_moving_prob, k_confidence, cluster.vcs_position_x, cluster.vcs_position_y);

      return cluster_priority;
   }

    /*===========================================================================*\
    * FUNCTION: Calculate_Headway_Priority()
    *===========================================================================
    * RETURN VALUE:
    * none
    *
    * PARAMETERS:
    * const F360_Host_T* const host_props,
    * const float32_t longitudal_pos,
    * const float32_t lateral_pos
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
    * This function calculates an object's headway priority.
    *
    * PRECONDITIONS:
    *
    * POSTCONDITIONS:
    * confidence is in range <0, 1> - lower value means higher prioritized
    *
    \*===========================================================================*/
   static float32_t Calculate_Headway_Priority(
      const F360_Host_T & host_props,
      const float32_t longitudal_pos,
      const float32_t lateral_pos)
   {
      float32_t headway_priority;
      if ((longitudal_pos > 0.0F) && (host_props.vcs_speed > F360_EPSILON))
      {
         const float32_t k_priority_headway_for_min_priority_inverse = 1.0F / 10.0F; // Priority Function: inverse of headway time that object do not gain priority (F360_MIN_PRIORITY)
         const float32_t k_priority_lat_penalty_max_dist_inverse = 1.0F / 20.0F; // Priority Function: inverse of maximum lateral penalty for lateral distance in curvilinear coordinate system  (F360_MIN_PRIORITY)

         const float32_t headway = longitudal_pos / host_props.vcs_speed;
         const float32_t curvi_lat_pos = Calculate_Curvi_Lat_Pos(host_props, longitudal_pos, lateral_pos);
         const float32_t headway_lon_priority = std::max(F360_MIN_PRIORITY, (F360_MAX_PRIORITY - (headway * k_priority_headway_for_min_priority_inverse)));
         const float32_t headway_lat_penalty = std::min(F360_MAX_PRIORITY, ((std::abs(curvi_lat_pos) * k_priority_lat_penalty_max_dist_inverse)));
         headway_priority = std::max(F360_MIN_PRIORITY, headway_lon_priority - headway_lat_penalty);
      }
      else
      {
         headway_priority = F360_MIN_PRIORITY;
      }
      return headway_priority;
   }
}
