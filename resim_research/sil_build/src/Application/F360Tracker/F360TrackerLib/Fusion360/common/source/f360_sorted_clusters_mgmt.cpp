/*===================================================================================*\
* FILE: f360_sorted_clusters_mgmt.cpp
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function declarations for functions to sort clusters
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include <cstring>
#include "f360_sorted_clusters_mgmt.h"
#include "f360_iterator.h"
#include <algorithm>
namespace f360_variant_A
{
   void Sorted_Clusters_Re_Sort(
      F360_Tracker_Info_T & tracker_info)
   {
      bool f_done = false;
      for (int16_t i = 0; (i < tracker_info.num_active_clusters - 1) && (!f_done); i++)
      {
         f_done = true;
         F360_Cluster_T * curr_cluster = tracker_info.vcslong_sorted_cluster_start;
         for (int16_t j = 0; j < tracker_info.num_active_clusters - i - 1; j++)
         {
            F360_Cluster_T * const next_cluster = tracker_info.vcslong_sorted_cluster_next[curr_cluster->id - 1];
            if (NULL == next_cluster)
            {
               break;
            }

            if (curr_cluster->vcs_position_x > next_cluster->vcs_position_x)
            {
               F360_Cluster_T * const prev_cluster = tracker_info.vcslong_sorted_cluster_prev[curr_cluster->id - 1];
               F360_Cluster_T * const next2_cluster = tracker_info.vcslong_sorted_cluster_next[next_cluster->id - 1];
               if (NULL != next2_cluster)
               {
                  tracker_info.vcslong_sorted_cluster_prev[next2_cluster->id - 1] = curr_cluster;
               }
               tracker_info.vcslong_sorted_cluster_next[curr_cluster->id - 1] = next2_cluster;
               tracker_info.vcslong_sorted_cluster_prev[curr_cluster->id - 1] = next_cluster;

               tracker_info.vcslong_sorted_cluster_next[next_cluster->id - 1] = curr_cluster;
               tracker_info.vcslong_sorted_cluster_prev[next_cluster->id - 1] = prev_cluster;

               if (tracker_info.vcslong_sorted_cluster_start == curr_cluster)
               {
                  tracker_info.vcslong_sorted_cluster_start = next_cluster;
               }
               else
               {
                  tracker_info.vcslong_sorted_cluster_next[prev_cluster->id - 1] = next_cluster;
               }

               f_done = false;
            }
            else
            {
               curr_cluster = next_cluster;
            }
         }
      }
   }

   void Sorted_Clusters_Update_List(
      F360_Tracker_Info_T & tracker_info)
   {
      Sorted_Clusters_Re_Sort(tracker_info);

      std::fill(cmn::begin(tracker_info.vcslong_sorted_cluster_list), cmn::end(tracker_info.vcslong_sorted_cluster_list), static_cast<int16_t>(0));
      if (NULL != tracker_info.vcslong_sorted_cluster_start)
      {
         int16_t cluster_idx = tracker_info.vcslong_sorted_cluster_start->id - 1;
         tracker_info.vcslong_sorted_cluster_list[0] = cluster_idx + 1;

         for (int16_t i = 1; i < tracker_info.num_active_clusters; i++)
         {
            cluster_idx = tracker_info.vcslong_sorted_cluster_next[cluster_idx]->id - 1;
            tracker_info.vcslong_sorted_cluster_list[i] = cluster_idx + 1;
         }
      }
   }

   void Sorted_Clusters_Insert(
      F360_Tracker_Info_T & tracker_info,
      F360_Cluster_T (&clusters)[NUMBER_OF_CLUSTERS],
      F360_Cluster_T & new_cluster,
      const int16_t initial_guess_id)
   {
      bool f_cluster_succesfully_inserted;
      if (NULL == tracker_info.vcslong_sorted_cluster_start)
      {
         f_cluster_succesfully_inserted = true;
         tracker_info.vcslong_sorted_cluster_start = &new_cluster;
      }
      else
      {
         f_cluster_succesfully_inserted = false;
      }

      int16_t cluster_idx = (initial_guess_id > 0) ? (initial_guess_id - 1) : (tracker_info.vcslong_sorted_cluster_start->id - 1);

      bool check_next = true;
      bool check_prev = true;
      while (!f_cluster_succesfully_inserted)
      {
         F360_Cluster_T* const next_cluster = tracker_info.vcslong_sorted_cluster_next[cluster_idx];
         F360_Cluster_T* const prev_cluster = tracker_info.vcslong_sorted_cluster_prev[cluster_idx];

         // check endpoints
         if (check_prev &&
            (new_cluster.vcs_position_x < clusters[cluster_idx].vcs_position_x) &&
            (prev_cluster == NULL))
         {
            // found the new smallest vcs_long
            tracker_info.vcslong_sorted_cluster_next[new_cluster.id - 1] = &clusters[cluster_idx];
            tracker_info.vcslong_sorted_cluster_prev[cluster_idx] = &new_cluster;
            tracker_info.vcslong_sorted_cluster_prev[new_cluster.id - 1] = NULL;
            tracker_info.vcslong_sorted_cluster_start = &new_cluster;
            f_cluster_succesfully_inserted = true;
         }
         else if (check_next &&
            (new_cluster.vcs_position_x >= clusters[cluster_idx].vcs_position_x) &&
            (next_cluster == NULL))
         {
            // found the new largest vcs_long
            tracker_info.vcslong_sorted_cluster_prev[new_cluster.id - 1] = &clusters[cluster_idx];
            tracker_info.vcslong_sorted_cluster_next[cluster_idx] = &new_cluster;
            tracker_info.vcslong_sorted_cluster_next[new_cluster.id - 1] = NULL;
            f_cluster_succesfully_inserted = true;
         }
         else
         {
            // figure out if we need to step or insert
            if ((NULL != next_cluster) &&
               (new_cluster.vcs_position_x >= next_cluster->vcs_position_x))
            {
               cluster_idx = next_cluster->id - 1;
               check_prev = false;
            }
            else if ((NULL != prev_cluster) &&
               (new_cluster.vcs_position_x < prev_cluster->vcs_position_x))
            {
               cluster_idx = prev_cluster->id - 1;
               check_next = false;
            }
            else
            {
               // figure out if we're insering next or prev
               if ((new_cluster.vcs_position_x >= clusters[cluster_idx].vcs_position_x) && 
                  (NULL != next_cluster))
               {
                  // inserting between current and next
                  tracker_info.vcslong_sorted_cluster_next[cluster_idx] = &new_cluster;
                  tracker_info.vcslong_sorted_cluster_next[new_cluster.id - 1] = next_cluster;
                  tracker_info.vcslong_sorted_cluster_prev[new_cluster.id - 1] = &clusters[cluster_idx];
                  tracker_info.vcslong_sorted_cluster_prev[next_cluster->id - 1] = &new_cluster;
                  f_cluster_succesfully_inserted = true;
               }
               else if (NULL != prev_cluster)
               {
                  // inserting between current and prev
                  tracker_info.vcslong_sorted_cluster_next[new_cluster.id - 1] = &clusters[cluster_idx];
                  tracker_info.vcslong_sorted_cluster_next[prev_cluster->id - 1] = &new_cluster;
                  tracker_info.vcslong_sorted_cluster_prev[cluster_idx] = &new_cluster;
                  tracker_info.vcslong_sorted_cluster_prev[new_cluster.id - 1] = prev_cluster;
                  f_cluster_succesfully_inserted = true;
               }
               else
               {
                  // Do nothing
               }
            }
         }
      }
   }

   void Sorted_Clusters_Insert_Batch(
      F360_Tracker_Info_T& tracker_info,
      F360_Cluster_T(&clusters)[NUMBER_OF_CLUSTERS],
      float32_t(&vcs_long)[MAX_TRACKER_POSN_CLUSTERS],
      int16_t(&new_cluster_ids)[MAX_TRACKER_POSN_CLUSTERS],
      const uint16_t num_clusters)
   {
      for (uint16_t i = 0U; i < num_clusters; i++)
      {
         for (uint16_t j = 1U; j < num_clusters - i; j++)
         {
            if (vcs_long[j - 1U] > vcs_long[j])
            {
               const float32_t temp_flt = vcs_long[j];
               const int16_t temp_int = new_cluster_ids[j];
               vcs_long[j] = vcs_long[j - 1U];
               new_cluster_ids[j] = new_cluster_ids[j - 1U];
               vcs_long[j - 1U] = temp_flt;
               new_cluster_ids[j - 1U] = temp_int;
            }
         }
      }

      uint16_t new_cluster_idx = 0U;
      F360_Cluster_T* curr_cluster = tracker_info.vcslong_sorted_cluster_start;
      if (curr_cluster == NULL)
      {
         tracker_info.vcslong_sorted_cluster_start = &clusters[new_cluster_ids[new_cluster_idx] - 1];
         curr_cluster = tracker_info.vcslong_sorted_cluster_start;
         new_cluster_idx++;
      }
      else if (curr_cluster->vcs_position_x > vcs_long[new_cluster_idx])
      {
         tracker_info.vcslong_sorted_cluster_start = &clusters[new_cluster_ids[new_cluster_idx] - 1];
         tracker_info.vcslong_sorted_cluster_next[new_cluster_ids[new_cluster_idx] - 1] = curr_cluster;
         tracker_info.vcslong_sorted_cluster_prev[curr_cluster->id - 1] = tracker_info.vcslong_sorted_cluster_start;
         curr_cluster = tracker_info.vcslong_sorted_cluster_start;
         new_cluster_idx++;
      }
      else
      {
         // Do nothing
      }

      while ((curr_cluster != NULL) && (new_cluster_idx != num_clusters))
      {
         F360_Cluster_T* const next_cluster = tracker_info.vcslong_sorted_cluster_next[curr_cluster->id - 1];
         if ((next_cluster == NULL) || (vcs_long[new_cluster_idx] < next_cluster->vcs_position_x))
         {
            tracker_info.vcslong_sorted_cluster_next[curr_cluster->id - 1] = &clusters[new_cluster_ids[new_cluster_idx] - 1];
            tracker_info.vcslong_sorted_cluster_prev[new_cluster_ids[new_cluster_idx] - 1] = curr_cluster;
            if (next_cluster != NULL)
            {
               tracker_info.vcslong_sorted_cluster_next[new_cluster_ids[new_cluster_idx] - 1] = next_cluster;
               tracker_info.vcslong_sorted_cluster_prev[next_cluster->id - 1] = &clusters[new_cluster_ids[new_cluster_idx] - 1];
            }
            new_cluster_idx++;
         }
         curr_cluster = tracker_info.vcslong_sorted_cluster_next[curr_cluster->id - 1];
      }
   }

   void Sorted_Clusters_Remove(
      F360_Tracker_Info_T & tracker_info,
      const F360_Cluster_T & dead_cluster)
   {
      F360_Cluster_T * const next_cluster = tracker_info.vcslong_sorted_cluster_next[dead_cluster.id - 1];
      F360_Cluster_T * const prev_cluster = tracker_info.vcslong_sorted_cluster_prev[dead_cluster.id - 1];

      if (next_cluster != NULL)
      {
         tracker_info.vcslong_sorted_cluster_prev[next_cluster->id - 1] = prev_cluster;
      }
      if (prev_cluster != NULL)
      {
         tracker_info.vcslong_sorted_cluster_next[prev_cluster->id - 1] = next_cluster;
      }
      if ((next_cluster == NULL) && (prev_cluster == NULL))
      {
         tracker_info.vcslong_sorted_cluster_start = NULL;
      }
      if (tracker_info.vcslong_sorted_cluster_start == &dead_cluster)
      {
         tracker_info.vcslong_sorted_cluster_start = next_cluster;
      }
      tracker_info.vcslong_sorted_cluster_next[dead_cluster.id - 1] = NULL;
      tracker_info.vcslong_sorted_cluster_prev[dead_cluster.id - 1] = NULL;
   }
}
