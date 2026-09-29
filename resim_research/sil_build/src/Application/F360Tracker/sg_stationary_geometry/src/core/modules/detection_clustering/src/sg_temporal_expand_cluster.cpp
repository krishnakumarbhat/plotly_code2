#include "sg_temporal_expand_cluster.h"

#include <cassert>

#include "sg_cluster_detections_helpers.h"
#include "sg_constants.h"
#include "sg_expand_cluster_helpers.h"
#include "sg_merge_clusters.h"
#include "sg_temporal_dbscan.h"

namespace sg
{
   void temporal_expand_cluster(NeighborCacheIterators &current_neighbors,
                                DetectionStorage &detections,
                                Detection_T &current_detection,
                                const uint16_t cluster_id,
                                const uint8_t min_cluster_points,
                                const float cluster_radius,
                                const float historical_num_neighbors_forgetting_factor)
   {
      current_detection.temp_cluster_id = cluster_id;

      auto current_neighbor_it = current_neighbors.begin();
      for (std::size_t current_neighbors_counter = 0U; current_neighbors_counter < current_neighbors.size();
           current_neighbors_counter++)
      {
         auto current_neighbor = **current_neighbor_it;

         if ((current_neighbors.size() <= SG_MAX_NUM_DET_CLUSTERING_NEIGHBOURS)
             && (current_neighbor->temp_cluster_id == INVALID_CLUSTER_ID))
         {
            current_neighbor->temp_cluster_id = cluster_id;
         }

         if (!current_neighbor->f_dbscan_visited)
         {
            current_neighbor->f_dbscan_visited = true;
            NewNeighborCacheIterators new_neighbors{};

            const float number_of_new_neighbors =
               static_cast<float>(get_dets_in_circular_region(new_neighbors, detections, *current_neighbor, cluster_radius));
            const float weighted_num_neighbors = number_of_new_neighbors + current_neighbor->cumulated_num_neighbors;
            const float minimum_cluster_points = static_cast<float>(min_cluster_points);
            const bool f_expand_cluster     = (minimum_cluster_points <= weighted_num_neighbors) && (0U < number_of_new_neighbors);
            current_neighbor->f_dbscan_core = (minimum_cluster_points <= weighted_num_neighbors);

            if (f_expand_cluster)
            {
               add_new_neighbors_to_current_neighbors(current_neighbors, new_neighbors);
            }

            current_neighbor->current_num_neighbors = number_of_new_neighbors;
            update_neighbors_history(*current_neighbor, historical_num_neighbors_forgetting_factor);
         }
         current_neighbor_it++;
      }

      merge_clusters(current_neighbors);
   }
}
