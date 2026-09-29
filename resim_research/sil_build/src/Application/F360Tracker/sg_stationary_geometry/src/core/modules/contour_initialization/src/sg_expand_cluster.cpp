#include "sg_expand_cluster.h"

#include <cassert>

#include "sg_constants.h"
#include "sg_dbscan.h"
#include "sg_initialize_contours_helpers.h"

namespace sg
{
   void expand_cluster(NeighborIterators &current_neighbors,
                       Detection_T &current_detection,
                       const Cluster *const current_cluster_ptr,
                       const uint16_t cluster_id,
                       const uint8_t min_cluster_points,
                       const float cluster_radius)
   {
      current_detection.temp_cluster_id = cluster_id;

      std::size_t current_neighbors_counter = 0U;
      for (auto current_neighbor_it = current_neighbors.begin(); (current_neighbors_counter < current_neighbors.size());
           current_neighbors_counter++)
      {
         auto &current_neighbor = *(*(current_neighbor_it));

         if ((current_neighbors.size() < SG_MAX_NUM_DET_CLUSTERING_NEIGHBOURS)
             && (current_neighbor.temp_cluster_id == INVALID_CLUSTER_ID))
         {
            current_neighbor.temp_cluster_id = cluster_id;
         }

         if (!current_neighbor.f_dbscan_visited)
         {
            current_neighbor.f_dbscan_visited = true;
            NewNeighborIterators new_neighbors{};

            const uint8_t num_new_neighbors =
               get_dets_in_circular_region(new_neighbors, current_cluster_ptr, current_neighbor, cluster_radius);

            if (min_cluster_points <= num_new_neighbors)
            {
               add_new_neighbors_to_current_neighbors(new_neighbors, current_neighbors);
            }
         }
         current_neighbor_it++;
      }
   }
}
