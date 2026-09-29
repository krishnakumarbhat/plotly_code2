#include "sg_declutter_contours_helpers.h"

#include <bitset>
#include <iterator>
#include <numeric>
#include <utility>

#include "sg_math.h"

namespace sg
{
   void select_contours_for_decluttering(std::array<sg::ContourStorage::ContourList::iterator, SG_MAX_NUM_CONTOURS> &contours_for_decluttering,
                                         std::array<uint16_t, MAX_NUM_CLUSTERS_FOR_DECLUTTER> &num_contours_in_cluster,
                                         const ContourStorage &contours)
   {
      // Select contours only from clusters that have more than one contour.
      uint16_t contours_in_cluster_count{1U};
      uint16_t visited_clusters_idx{0U};
      std::bitset<SG_MAX_NUM_CONTOURS> visited_clusters{false};
      auto contours_for_decluttering_it = contours_for_decluttering.begin();
      auto num_contours_in_cluster_it   = num_contours_in_cluster.begin();
      auto ref_contour_it               = contours.begin();

      for (uint16_t idx = 0U; idx < static_cast<uint16_t>(contours.size()); ++idx)
      {
         if (visited_clusters[idx])
         {
            ++ref_contour_it;
            continue;
         }

         visited_clusters_idx = idx + 1U;

         for (auto curr_contour_it = std::next(ref_contour_it); curr_contour_it != (contours.end()); ++curr_contour_it)
         {
            if (ref_contour_it->cluster_id == curr_contour_it->cluster_id)
            {
               visited_clusters[visited_clusters_idx] = true;
               *contours_for_decluttering_it++        = curr_contour_it;
               ++contours_in_cluster_count;
            }
            ++visited_clusters_idx;
         }
         if (contours_in_cluster_count > 1U)
         {
            *contours_for_decluttering_it++ = ref_contour_it;
            *num_contours_in_cluster_it++   = contours_in_cluster_count;
            contours_in_cluster_count       = 1U;
         }

         ++ref_contour_it;
      }

      // MISRA Rule:0-1-6
      (void) contours_in_cluster_count;
      (void) visited_clusters_idx;
      (void) contours_for_decluttering_it;
      (void) num_contours_in_cluster_it;
      (void) ref_contour_it;
   }

   void calc_vertices_azimuth_and_range(const Contour_T::VertexList &vertices,
                                        std::array<float, SG_MAX_NUM_VERTICES_PER_CONTOUR> &vertices_azimuth,
                                        std::array<float, SG_MAX_NUM_VERTICES_PER_CONTOUR> &vertices_range)
   {
      assert(static_cast<uint16_t>(vertices.size()) <= SG_MAX_NUM_VERTICES_PER_CONTOUR); // vertices_azimuth and vertices_range
                                                                                         // must be less or the same size as
                                                                                         // VertexList

      auto vertices_azimuth_it = vertices_azimuth.begin();
      auto vertices_range_it   = vertices_range.begin();

      for (const auto &vtx : vertices)
      {
         *vertices_azimuth_it++ = calculate_azimuth(vtx.position.x, vtx.position.y);
         *vertices_range_it++   = calculate_point_distance(vtx.position.x, vtx.position.y);
      }

      // MISRA Rule:0-1-6
      (void) vertices_azimuth_it;
      (void) vertices_range_it;
   }

   void mark_occluded_contours(std::array<uint16_t, SG_MAX_NUM_CONTOURS> &num_of_occluders,
                               const std::array<sg::ContourStorage::ContourList::iterator, SG_MAX_NUM_CONTOURS> &contours_for_decluttering,
                               const std::array<uint16_t, MAX_NUM_CLUSTERS_FOR_DECLUTTER> &num_contours_in_clusters,
                               const Contour_Postprocessing_Calibrations_T::Declutter_Contours_T &calibrations,
                               const float azimuth_epsilon)
   {
      std::array<ContourProps, SG_MAX_NUM_CONTOURS> contours_props{};
      calc_contours_properties(contours_props, contours_for_decluttering, num_contours_in_clusters);

      uint16_t idx             = 0U;
      auto num_of_occluders_it = num_of_occluders.begin();
      auto tested_contour_it   = contours_props.begin();
      auto cluster_begin_it    = contours_props.begin();
      auto cluster_end_it      = cluster_begin_it;

      const auto num_contours_in_clusters_size = num_contours_in_clusters.size();

      while ((idx < num_contours_in_clusters_size) && (num_contours_in_clusters[idx] > 0U))
      {
         std::advance(cluster_end_it, num_contours_in_clusters[idx]);

         for (; tested_contour_it != cluster_end_it; ++tested_contour_it)
         {
            for (auto potential_occluder_it = cluster_begin_it; potential_occluder_it != cluster_end_it; ++potential_occluder_it)
            {
               if (tested_contour_it != potential_occluder_it)
               {
                  const bool f_occlusion_result =
                     occlusion_assessment(*tested_contour_it, *potential_occluder_it, calibrations, azimuth_epsilon);
                  if (f_occlusion_result)
                  {
                     (*num_of_occluders_it)++;
                  }
               }
            }
            ++num_of_occluders_it;
         }
         cluster_begin_it = cluster_end_it;
         ++idx;
      }

      // MISRA Rule:0-1-6
      (void) num_of_occluders_it;
      (void) tested_contour_it;
      (void) cluster_begin_it;
      (void) cluster_end_it;
   }

   void calc_contours_properties(std::array<ContourProps, SG_MAX_NUM_CONTOURS> &contours_props,
                                 const std::array<sg::ContourStorage::ContourList::iterator, SG_MAX_NUM_CONTOURS> &contours_for_decluttering,
                                 const std::array<uint16_t, MAX_NUM_CLUSTERS_FOR_DECLUTTER> &num_contours_in_clusters)
   {
      const auto num_contours = std::accumulate(num_contours_in_clusters.begin(), num_contours_in_clusters.end(), 0U);

      for (uint16_t idx = 0U; idx < num_contours; ++idx)
      {
         calc_vertices_azimuth_and_range(contours_for_decluttering[idx]->vertices, contours_props[idx].vertices_azimuth,
                                         contours_props[idx].vertices_range);
         contours_props[idx].vertices_number = static_cast<uint16_t>(contours_for_decluttering[idx]->vertices.size());
      }

      // MISRA Rule:0-1-6
      (void) num_contours;
   }

   void remove_occluded_contours(ContourStorage &contours,
                                 const std::array<sg::ContourStorage::ContourList::iterator, SG_MAX_NUM_CONTOURS> &contours_for_decluttering,
                                 const std::array<uint16_t, SG_MAX_NUM_CONTOURS> &num_of_occluders,
                                 const uint8_t occluders_num_threshold)
   {
      for (uint16_t idx = 0U; idx < num_of_occluders.size(); ++idx)
      {
         if (num_of_occluders[idx] > occluders_num_threshold)
         {
            (void) contours.erase(contours_for_decluttering[idx]);
         }
      }
   }

   bool occlusion_assessment(const ContourProps &tested_contour,
                             const ContourProps &potential_occluder,
                             const Contour_Postprocessing_Calibrations_T::Declutter_Contours_T &calibrations,
                             const float azimuth_epsilon)
   {
      const float delta_azimuth = DEG2RAD(calibrations.contour_occlusion_azimuth_margin + azimuth_epsilon);

      std::bitset<SG_MAX_NUM_VERTICES_PER_CONTOUR> vertices_azimuth_within_limits{false};
      std::bitset<SG_MAX_NUM_VERTICES_PER_CONTOUR> vertices_range_within_limits{false};

      // Loop over segments of contour that can be occluder contour
      for (uint16_t idx = 0U; idx < potential_occluder.vertices_number - 1U; ++idx)
      {
         const std::pair<float, float> azimuth_limits =
            std::minmax(potential_occluder.vertices_azimuth[idx], potential_occluder.vertices_azimuth[idx + 1U]);

         // check azimuth condition
         // mark vertices that are within segments vertices azimuths
         if ((azimuth_limits.second - azimuth_limits.first) < PI)
         {
            const float min_azimuth_threshold_low  = azimuth_limits.first - delta_azimuth;
            const float max_azimuth_threshold_high = azimuth_limits.second + delta_azimuth;
            for (uint16_t i = 0U; i < tested_contour.vertices_number; ++i)
            {
               vertices_azimuth_within_limits[i] = vertices_azimuth_within_limits[i]
                                                   || ((tested_contour.vertices_azimuth[i] > min_azimuth_threshold_low)
                                                       && (tested_contour.vertices_azimuth[i] < max_azimuth_threshold_high));
            }
            // MISRA Rule:0-1-6
            (void) min_azimuth_threshold_low;
            (void) max_azimuth_threshold_high;
         }
         else
         {
            const float max_azimuth_threshold_low  = azimuth_limits.second - delta_azimuth;
            const float min_azimuth_threshold_high = azimuth_limits.first + delta_azimuth;
            for (uint16_t i = 0U; i < tested_contour.vertices_number; ++i)
            {
               vertices_azimuth_within_limits[i] = vertices_azimuth_within_limits[i]
                                                   || ((tested_contour.vertices_azimuth[i] < min_azimuth_threshold_high)
                                                       || (tested_contour.vertices_azimuth[i] > max_azimuth_threshold_low));
            }
            // MISRA Rule:0-1-6
            (void) max_azimuth_threshold_low;
            (void) min_azimuth_threshold_high;
         }

         // check range condition
         const float min_range = std::min(potential_occluder.vertices_range[idx], potential_occluder.vertices_range[idx + 1U]);

         for (uint16_t i = 0U; i < tested_contour.vertices_number; ++i)
         {
            vertices_range_within_limits[i] =
               vertices_azimuth_within_limits[i]
               && (tested_contour.vertices_range[i] > min_range + calibrations.contour_occlusion_range_margin[0U])
               && (tested_contour.vertices_range[i] < min_range + calibrations.contour_occlusion_range_margin[1U]);
         }
         // MISRA Rule:0-1-6
         (void) min_range;
      }
      // MISRA Rule:0-1-6
      (void) delta_azimuth;

      const auto occluded_vertices = vertices_azimuth_within_limits & vertices_range_within_limits;
      const float occlusion_ratio = static_cast<float>(occluded_vertices.count()) / static_cast<float>(tested_contour.vertices_number);

      return (calibrations.contour_occlusion_threshold < occlusion_ratio);
   }
}
