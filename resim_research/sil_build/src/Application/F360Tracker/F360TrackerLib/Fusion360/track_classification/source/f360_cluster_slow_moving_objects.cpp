/*===================================================================================*\
* FILE: f360_cluster_slow_moving_objects.cpp
*====================================================================================
* Copyright 2025 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*  This is the function for slow objects clustering.
*
* ABBREVIATIONS:
*   OTG	Over-The-ground
*
* TRACEABILITY INFO:
*   Design Document(s):
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*
*
* DEVIATIONS FROM STANDARDS:
*
*
\*==========================================================================================*/


/******************************
* Includes
*******************************/
#include <cmath>
#include "f360_cluster_slow_moving_objects.h"

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Cluster_Slow_Moving_Objects
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * F360_Tracker_Info_T & tracker_info
   * const F360_Host_T & host
   * F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT: The clustering is instantaneous and looks for slow moving objects
   * close to host. If two objects are within a 2m radius from one another, they're
   * assigned to the same cluster. If another object is close to any object of an
   * existing cluster, it is added to the cluster.
   * --------------------------------------------------------------------------
   * constructor
   *
   * PRECONDITIONS:
   * None
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/

   void Cluster_Slow_Moving_Objects(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Host_T& host,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS])
   {
      // Reset clustering information for all objects
      for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
      {
         const int32_t idx = tracker_info.active_obj_ids[i] - 1;
         object_tracks[idx].slow_moving_cluster_id = 0U;
         object_tracks[idx].num_members_in_slow_moving_obj_cluster = 0U;
         object_tracks[idx].length_of_slow_moving_obj_cluster = 0.0F;
      }
      
      constexpr float32_t max_host_speed = 20.0F;
      
      if (host.speed < max_host_speed)
      {
         constexpr float32_t max_dist_from_host_x = 70.0F;
         constexpr float32_t max_dist_from_host_y = 20.0F;
         constexpr float32_t max_neighbour_dist = 2.0F;
         constexpr float32_t max_neighbour_dist_squared = max_neighbour_dist * max_neighbour_dist;

         uint32_t curr_cluster_id = 0U;
         const F360_Object_Track_T* obj1 = tracker_info.vcslong_sorted_start;
         for (int32_t i = 0; i < tracker_info.num_active_objs; i++) // Outer loop over objects
         {
            if ((NULL == obj1) || (obj1->vcs_position.x > max_dist_from_host_x))
            {
               break;
            }
            const int32_t idx1 = obj1->id - 1;
            constexpr float32_t min_speed_slow_moving = 0.2F;
            constexpr float32_t max_speed_slow_moving = 8.5F; // Todo: Similar threshold as used for speed of bikes in classification code. Use calib value?

            const bool obj1_close_enough_to_host = ((std::abs(obj1->vcs_position.x) < max_dist_from_host_x)
               && (std::abs(obj1->vcs_position.y) < max_dist_from_host_y));
            const bool obj1_slow_moving = (std::abs(obj1->speed) > min_speed_slow_moving) && (std::abs(obj1->speed)< max_speed_slow_moving);
            if (obj1_close_enough_to_host && obj1_slow_moving)
            {
               // Find possible neighbouring objects to cluster in front of obj1
               int32_t idx2 = idx1;
               for (int32_t j = 0; j < tracker_info.num_active_objs; j++)
               {
                  const F360_Object_Track_T* const obj2 = tracker_info.vcslong_sorted_next_track[idx2];
                  if ((NULL == obj2) || (obj2->vcs_position.x > (obj1->vcs_position.x + max_neighbour_dist)))
                  {
                     // Break if no more objects or if next object is too far away from obj1 longitudinally
                     break;
                  }
                  idx2 = obj2->id - 1;

                  const bool obj2_slow_moving = (std::abs(obj2->speed) > min_speed_slow_moving) && (std::abs(obj2->speed) < max_speed_slow_moving);
                  const bool obj2_not_yet_clustered = (obj2->slow_moving_cluster_id == 0U);
                  if (obj2_slow_moving && obj2_not_yet_clustered)
                  {
                     const float32_t obj1_obj_2_dx = obj1->vcs_position.x - obj2->vcs_position.x;
                     const float32_t obj1_obj_2_dy = obj1->vcs_position.y - obj2->vcs_position.y;
                     const float32_t obj1_obj_2_dist_sq = obj1_obj_2_dx * obj1_obj_2_dx + obj1_obj_2_dy * obj1_obj_2_dy;
                     if (obj1_obj_2_dist_sq < max_neighbour_dist_squared)
                     {
                        if (obj1->slow_moving_cluster_id == 0U)
                        {
                           curr_cluster_id++;
                           object_tracks[idx1].slow_moving_cluster_id = curr_cluster_id;
                        }
                        object_tracks[idx2].slow_moving_cluster_id = object_tracks[idx1].slow_moving_cluster_id;
                     }
                  }
               }
            }
            obj1 = tracker_info.vcslong_sorted_next_track[idx1];
         }
         Count_Number_Of_Members_In_Clusters_And_Find_Cluster_Length(tracker_info, curr_cluster_id, object_tracks);
      }
   }

   /*===========================================================================*\
   * FUNCTION: Count_Number_Of_Members_In_Clusters_And_Find_Cluster_Length()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * F360_Tracker_Info_T & tracker_info
   * const uint32_t num_clusters - total number of clusters
   * F360_Object_Track_T (&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT: Analyses each cluster of slow moving objects and computes some
   * statistics. The cluster statistic is assigned to each of its member objects.
   * Statistics thar are computed are
   *   - Number of objects that belong to the cluster
   *   - Cluster length (difference between max and min member object center coordinates (in the direction of the avereage heading of the member objects)
   * --------------------------------------------------------------------------
   * constructor
   *
   * PRECONDITIONS:
   * Code assumes that clusters have ids in the range from 1 - num_clusters
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Count_Number_Of_Members_In_Clusters_And_Find_Cluster_Length(
      const F360_Tracker_Info_T& tracker_info,
      const uint32_t num_clusters,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS])
   {

      // Count the number of members in each cluster and find the object ids in cluster
      for (uint32_t cluster_id = 1U; cluster_id <= num_clusters; cluster_id++)
      {
         // Find which objects are in this cluster
         uint32_t num_objects_in_cluster = 0U;
         int32_t obj_idx_in_cluster[NUMBER_OF_OBJECT_TRACKS];
         for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
         {
            const int32_t obj_idx = tracker_info.active_obj_ids[i] - 1;
            if (object_tracks[obj_idx].slow_moving_cluster_id == cluster_id)
            {
               obj_idx_in_cluster[num_objects_in_cluster] = obj_idx;
               num_objects_in_cluster++;
            }
         }

         // Find average heading of objects in cluster
         if(num_objects_in_cluster > 0U) // Protection against zero division
         {
            // Compute the length of this cluster
            float32_t avg_vcs_heading = 0.0F;
            for (uint32_t i = 0U; i < num_objects_in_cluster; i++)
            {
               const int32_t obj_idx = obj_idx_in_cluster[i];
               avg_vcs_heading += object_tracks[obj_idx].vcs_heading.Value();
            }
            avg_vcs_heading /= static_cast<float32_t>(num_objects_in_cluster);
            const float32_t cos_avg_vcs_hdg = F360_Cosf(avg_vcs_heading);
            const float32_t sin_avg_vcs_hdg = F360_Sinf(avg_vcs_heading);


            // Find max and min object center position in the average heading direction
            float32_t max_center_para = -INFTY;
            float32_t min_center_para = INFTY;
            for (uint32_t i = 0U; i < num_objects_in_cluster; i++)
            {
               const int32_t obj_idx = obj_idx_in_cluster[i];
               const Point obj_bbox_center = object_tracks[obj_idx].bbox.Get_Center();
               const float32_t curr_center_para = cos_avg_vcs_hdg * obj_bbox_center.x + sin_avg_vcs_hdg * obj_bbox_center.y;
               if (curr_center_para > max_center_para)
               {
                  max_center_para = curr_center_para;
               }
               if (curr_center_para < min_center_para)
               {
                     min_center_para = curr_center_para;
               }
            }

            const float32_t cluster_length = max_center_para - min_center_para;


            // Assign values to objects
            for (uint32_t i = 0U; i < num_objects_in_cluster; i++)
            {
               const int32_t obj_idx = obj_idx_in_cluster[i];
               object_tracks[obj_idx].num_members_in_slow_moving_obj_cluster = num_objects_in_cluster;
               object_tracks[obj_idx].length_of_slow_moving_obj_cluster = cluster_length;
            }
         }
      }
   }
}
