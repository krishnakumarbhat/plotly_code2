#include "sg_detection_storage.h"

#include <algorithm>

namespace sg
{
   void DetectionStorage::move_detection(const ClusterList::iterator &destination_cluster, const DetectionList::iterator &det_it)
   {
      const auto source_cluster_ptr = det_it->cluster;

      assert(!source_cluster_ptr->empty());

      if (source_cluster_ptr != &(*destination_cluster))
      {
         if (source_cluster_ptr->begin() == det_it)
         {
            (source_cluster_ptr->m_begin)++;
         }

         if (destination_cluster->empty())
         {
            m_detections.move(det_it, m_detections.end());
            destination_cluster->m_begin = det_it;
         }
         else
         {
            m_detections.move(det_it, destination_cluster->end());
         }

         (source_cluster_ptr->m_size)--;
         (destination_cluster->m_size)++;

         if (source_cluster_ptr->empty())
         {
            source_cluster_ptr->m_begin = m_detections.end();
         }

         det_it->cluster_id = destination_cluster->m_unique_id;
         det_it->cluster    = &(*destination_cluster);
      }
   }

   DetectionList::iterator DetectionStorage::push_back(const Detection_T &detection)
   {
      auto &unclustered = m_clusters.unclustered();
      DetectionList::iterator added_detection_it;
      if (unclustered.empty())
      {
         added_detection_it  = m_detections.push_front(detection);
         unclustered.m_begin = added_detection_it;
      }
      else
      {
         added_detection_it = m_detections.insert(unclustered.end(), detection);
      }

      if (added_detection_it != m_detections.end())
      {
         Detection_T *const det_ptr = &(*added_detection_it);
         m_cache_detections.add_detection(det_ptr);
         added_detection_it->unique_id = m_id_handler.get_id();
         added_detection_it->cluster   = &(*(m_clusters.begin()));
         unclustered.m_size++;
      }
      else
      {
         // Detection couldn't be added to DetectionStorage
         // function will return default initialized nullptr iterator
      }

      return added_detection_it;
   }

   DetectionList::iterator DetectionStorage::erase(const DetectionList::iterator &it)
   {
      const auto det_ptr = &(*(it));
      m_cache_detections.remove_detection(det_ptr);

      const auto current_cluster = std::find_if(m_clusters.begin(), m_clusters.end(),
                                                [&](const auto &local_cluster)
                                                { return local_cluster.unique_id() == it->cluster_id; });
      (current_cluster->m_size)--;
      if ((current_cluster->size() == 0U) && (current_cluster != m_clusters.begin()) && (current_cluster != m_clusters.end()))
      {
         (void) m_clusters.erase(current_cluster);
      }
      if (current_cluster->begin() == it)
      {
         (current_cluster->m_begin)++;
      }
      const DetectionList::iterator return_it = m_detections.erase(it);
      return return_it;
   }

   void DetectionStorage::clear()
   {
      m_id_handler.reset();
      m_clusters.clear();
      m_cache_detections.clear();
      m_detections.clear();
      clear_detection_bins();
   }

   void DetectionStorage::update_cluster_ages()
   {
      Clusters &all_clusters = get_clusters();
      const auto end_it      = all_clusters.end();
      for (auto cluster_it = all_clusters.begin(); cluster_it != end_it; cluster_it++)
      {
         cluster_it->increment_age();
      }
   }

   ClusterList::iterator DetectionStorage::find_cluster_or_create_new(const uint16_t cluster_id)
   {
      ClusterList::iterator cluster_it = std::find_if(m_clusters.begin(), m_clusters.end(),
                                                      [&cluster_id](const auto &local_cluster)
                                                      { return local_cluster.unique_id() == cluster_id; });

      if (cluster_it == m_clusters.end())
      {
         cluster_it = m_clusters.create_new();
         // TODO: FZD-822 Unhandled situation when cluster couldn't be created
         assert(cluster_it != nullptr);
      }
      return cluster_it;
   }

   void DetectionStorage::remove_empty_clusters()
   {
      const auto cluster_end_it = m_clusters.m_clusters.end();
      for (auto cluster_it = ++m_clusters.m_clusters.begin(); cluster_it != cluster_end_it;)
      {
         if (cluster_it->empty())
         {
            cluster_it = m_clusters.erase(cluster_it);
         }
         else
         {
            ++cluster_it;
         }
      }
   }

   void DetectionStorage::update_bin_info()
   {
      float bin_boundary               = SG_BIN_BOUNDS_MIN;
      const auto detection_cache_end   = m_cache_detections.cend();
      const auto is_detection_valid    = [&bin_boundary](const auto &det) { return det->position.x >= bin_boundary; };
      const auto detection_cache_begin = m_cache_detections.cbegin();
      auto detection_it                = std::find_if(detection_cache_begin, detection_cache_end, is_detection_valid);
      auto bin_idx                     = 0U;

      while (bin_boundary < SG_BIN_BOUNDS_MAX)
      {
         detection_it = std::find_if(detection_cache_begin, detection_cache_end, is_detection_valid);
         if (bin_idx < SG_NUM_BINS)
         {
            m_bin_start_its[bin_idx] = detection_it;
         }
         bin_idx++;
         bin_boundary += SG_BIN_WIDTH;
      }
      (void) bin_idx;      // MISRA
      (void) bin_boundary; // MISRA
      (void) detection_it; // MISRA
   }

   /**
    * @brief   Function object to sort detections longitudinally ascending
    *
    * @param   lhs
    * @param   rhs
    *
    * @return  bool
    */
   struct longitudinally_less
   {
      bool operator()(const Detection_T *const lhs, const Detection_T *const rhs) const
      {
         return lhs->position.x < rhs->position.x;
      }
   };

   DetectionCache::collection_data_type::const_iterator DetectionStorage::begin(const float position_x) const
   {
      DetectionCache::collection_data_type::const_iterator det_it{};
      if (position_x < SG_BIN_BOUNDS_MIN)
      {
         det_it = m_cache_detections.cbegin();
      }
      else if (position_x <= SG_BIN_BOUNDS_MAX)
      {
         const auto bin_idx = static_cast<std::uint16_t>(std::floor((position_x - SG_BIN_BOUNDS_MIN) / SG_BIN_WIDTH));
         det_it             = m_bin_start_its[bin_idx];
      }
      else
      {
         det_it = *std::prev(m_bin_start_its.cend());
      }
      return det_it;
   }

   DetectionCache::collection_data_type::const_iterator DetectionStorage::end(const float position_x) const
   {
      DetectionCache::collection_data_type::const_iterator det_it = m_cache_detections.cend();
      if (position_x < SG_BIN_BOUNDS_MIN)
      {
         det_it = m_bin_start_its[0U];
      }
      else if (position_x <= SG_BIN_BOUNDS_MAX)
      {
         const auto bin_idx = static_cast<std::uint16_t>(std::ceil((position_x - SG_BIN_BOUNDS_MIN) / SG_BIN_WIDTH));

         if (bin_idx < SG_NUM_BINS)
         {
            det_it = m_bin_start_its[bin_idx];
         }
      }
      else
      {
         det_it = m_cache_detections.cend();
      }
      return det_it;
   }

   void DetectionStorage::sort_by_x_pos()
   {
      std::sort(m_cache_detections.begin(), m_cache_detections.end(), longitudinally_less());
   }

   bool DetectionStorage::is_sorted_by_x() const
   {
      return std::is_sorted(m_cache_detections.cbegin(), m_cache_detections.cend(), longitudinally_less());
   }

   void DetectionStorage::initialize(const SG_Detection_Storage_Dump_T &dumped_internal_detections) const
   {
      (void) dumped_internal_detections; // TODO: FZD-1861
   }
}
