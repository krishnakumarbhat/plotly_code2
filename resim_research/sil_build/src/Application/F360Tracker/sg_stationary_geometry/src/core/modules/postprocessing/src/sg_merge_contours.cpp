#include "sg_merge_contours.h"

#include <algorithm>

#include "sg_merge_contours_helpers.h"

namespace sg
{
   void merge_contours(ContourStorage &contours,
                       const Contour_Postprocessing_Calibrations_T::Merge_Contours_T &calibrations,
                       const float curvature_rear)
   {
      uint16_t min_cluster_id               = 1U;
      uint16_t max_cluster_id               = min_cluster_id + NUM_CLUSTERS_IN_ONE_ITERATION;
      uint16_t number_of_processed_contours = 0U;

      std::array<Contour_it, SG_MAX_NUM_CONTOURS> contours_array;
      std::array<CommonClusterContoursRow_T, NUM_CLUSTERS_IN_ONE_ITERATION> clustered_contours;

      const auto number_of_contours = select_contours_belonging_to_cluster(contours_array, contours);

      while (number_of_processed_contours < number_of_contours)
      {
         for (uint16_t idx = 0U; idx < number_of_contours; idx++)
         {
            if ((contours_array[idx]->cluster_id >= min_cluster_id) && (contours_array[idx]->cluster_id < max_cluster_id))
            {
               const uint16_t index_to_insert_contour = (contours_array[idx]->cluster_id) % NUM_CLUSTERS_IN_ONE_ITERATION;

               if (index_to_insert_contour < NUM_CLUSTERS_IN_ONE_ITERATION)
               {
                  clustered_contours[index_to_insert_contour].push_back(contours_array[idx]);
                  number_of_processed_contours++;
               }
            }
         }

         for (auto &common_cluster_contours : clustered_contours)
         {
            if (common_cluster_contours.number_of_contours() >= 2U)
            {
               bool f_pair_found = false;
               do
               {
                  f_pair_found = find_and_merge_pair(common_cluster_contours, contours, calibrations, curvature_rear);
               } while (f_pair_found);
            }
         }
         min_cluster_id += NUM_CLUSTERS_IN_ONE_ITERATION;
         max_cluster_id += NUM_CLUSTERS_IN_ONE_ITERATION;

         clear_common_cluster_contours(clustered_contours);
      }
      // MISRA
      (void) min_cluster_id;
      (void) max_cluster_id;
   }

   uint16_t select_contours_belonging_to_cluster(std::array<Contour_it, SG_MAX_NUM_CONTOURS> &contour_its,
                                                 const ContourStorage &contours)
   {
      size_t idx = 0U;
      for (auto it = contours.begin(); it != contours.end(); it++)
      {
         if (it->cluster_id != INVALID_CLUSTER_ID)
         {
            contour_its[idx++] = it;
         }
      }
      return idx;
   }

   void clear_common_cluster_contours(std::array<CommonClusterContoursRow_T, NUM_CLUSTERS_IN_ONE_ITERATION> &common_cluster_contours)
   {
      for (auto &row : common_cluster_contours)
      {
         row.clear_number_of_contours();
      }
   }

   bool find_and_merge_pair(CommonClusterContoursRow_T &clustered_contour_its,
                            ContourStorage &contours,
                            const Contour_Postprocessing_Calibrations_T::Merge_Contours_T &calibrations,
                            const float curvature_rear)
   {
      // create current - partner pairs and pick partner closest to the current contour
      bool f_pair_found                            = false;
      const float merge_distance_threshold_squared = calibrations.merge_distance_threshold * calibrations.merge_distance_threshold;
      for (uint16_t current_idx = 0U; current_idx < clustered_contour_its.number_of_contours(); current_idx++)
      {
         const Contour_it &current_contour_it = clustered_contour_its.contour_its[current_idx];

         if (current_contour_it == nullptr)
         {
            continue;
         }

         contour_merge_setup merge_flags;
         const auto partner_contour_idx = find_closest_partner_contour(merge_flags, clustered_contour_its, current_contour_it,
                                                                       current_idx, curvature_rear, calibrations);

         if (partner_contour_idx >= clustered_contour_its.contour_its.size())
         {
            continue;
         }
         const Contour_it partner_contour_it = clustered_contour_its.contour_its[partner_contour_idx];

         // MISRA
         (void) partner_contour_it;

         if (((merge_flags.f_merge_beginnings == false) && (merge_flags.f_merge_ends == false)) || (partner_contour_it == nullptr))
         {
            continue;
         }

         if ((current_contour_it->vertices.size() + partner_contour_it->vertices.size() - 1) > SG_MAX_NUM_VERTICES_PER_CONTOUR)
         {
            continue;
         }

         auto contour_to_remove_idx = partner_contour_idx;
         Contour_it destination_contour_it;
         Contour_it source_contour_it;

         if (merge_flags.f_merge_beginnings)
         {
            if (merge_flags.f_begin_flip_partner_contour)
            {
               flip_contour(*partner_contour_it);
            }

            const bool f_contours_swapped =
               determine_destination_contour(current_contour_it, partner_contour_it, destination_contour_it, source_contour_it);

            if (f_contours_swapped)
            {
               contour_to_remove_idx = current_idx;
            }
            else
            {
               destination_contour_it->vertices.swap(source_contour_it->vertices);
            }
         }
         else if (merge_flags.f_merge_ends)
         {
            if (merge_flags.f_end_flip_partner_contour)
            {
               flip_contour(*partner_contour_it);
            }
            const bool f_contours_swapped =
               determine_destination_contour(current_contour_it, partner_contour_it, destination_contour_it, source_contour_it);

            if (f_contours_swapped)
            {
               contour_to_remove_idx = current_idx;
               destination_contour_it->vertices.swap(source_contour_it->vertices);
            }
         }
         else
         {
            // MISRA
            (void) contour_to_remove_idx;
            (void) destination_contour_it;
            (void) source_contour_it;

            continue;
         }

         merge_two_contours(destination_contour_it, source_contour_it);

         source_contour_it->vertices.clear();
         (void) contours.erase(source_contour_it);
         clustered_contour_its.remove(contour_to_remove_idx);
         f_pair_found = true;
      }
      (void) merge_distance_threshold_squared; // MISRA
      return f_pair_found;
   }

   uint16_t find_closest_partner_contour(contour_merge_setup &merge_flags,
                                         const CommonClusterContoursRow_T &clustered_contours,
                                         const Contour_it current_contour,
                                         const uint16_t current_idx,
                                         const float curvature_rear,
                                         const Contour_Postprocessing_Calibrations_T::Merge_Contours_T &calibrations)
   {
      uint16_t nearest_partner_beginning_idx = 0U;
      uint16_t nearest_partner_end_idx       = 0U;

      float min_beginning_distance_sq = std::numeric_limits<float>::max();
      float min_end_distance_sq       = std::numeric_limits<float>::max();

      for (uint16_t partner_idx = 0U; partner_idx < clustered_contours.number_of_contours(); partner_idx++)
      {
         if ((clustered_contours.contour_its[partner_idx] == nullptr) || (current_idx == partner_idx))
         {
            continue;
         }

         const Contour_it partner_contour_it = clustered_contours.contour_its[partner_idx];

         float current_begin_to_partner_begin_dist_sq{};
         float current_begin_to_partner_end_dist_sq{};
         float current_end_to_partner_begin_dist_sq{};
         float current_end_to_partner_end_dist_sq{};

         get_all_merge_distances(current_begin_to_partner_begin_dist_sq, current_begin_to_partner_end_dist_sq,
                                 current_end_to_partner_begin_dist_sq, current_end_to_partner_end_dist_sq, *current_contour,
                                 *partner_contour_it, calibrations.merge_longitudinal_squeeze_factor, curvature_rear);

         if (current_begin_to_partner_begin_dist_sq < min_beginning_distance_sq)
         {
            const geometry::Segment2D_T current_contour_segment = {std::next(current_contour->vertices.begin())->position,
                                                                   current_contour->vertices.begin()->position};
            const geometry::Segment2D_T partner_contour_segment = {partner_contour_it->vertices.begin()->position,
                                                                   std::next(partner_contour_it->vertices.begin())->position};

            const bool f_angles_within_limits = check_all_merge_angles(*current_contour, *partner_contour_it, current_contour_segment,
                                                                       partner_contour_segment, calibrations);

            if (f_angles_within_limits)
            {
               min_beginning_distance_sq                = current_begin_to_partner_begin_dist_sq;
               nearest_partner_beginning_idx            = partner_idx;
               merge_flags.f_begin_flip_partner_contour = true;
            }
         }
         if (current_begin_to_partner_end_dist_sq < min_beginning_distance_sq)
         {
            const geometry::Segment2D_T current_contour_segment = {std::next(current_contour->vertices.begin())->position,
                                                                   current_contour->vertices.begin()->position};
            const geometry::Segment2D_T partner_contour_segment = {partner_contour_it->vertices.rbegin()->position,
                                                                   std::next(partner_contour_it->vertices.rbegin())->position};

            const bool f_angles_within_limits = check_all_merge_angles(*current_contour, *partner_contour_it, current_contour_segment,
                                                                       partner_contour_segment, calibrations);

            if (f_angles_within_limits)
            {
               min_beginning_distance_sq                = current_begin_to_partner_end_dist_sq;
               nearest_partner_beginning_idx            = partner_idx;
               merge_flags.f_begin_flip_partner_contour = false;
            }
         }
         if (current_end_to_partner_begin_dist_sq < min_end_distance_sq)
         {
            const geometry::Segment2D_T current_contour_segment = {std::next(current_contour->vertices.rbegin())->position,
                                                                   current_contour->vertices.rbegin()->position};
            const geometry::Segment2D_T partner_contour_segment = {partner_contour_it->vertices.begin()->position,
                                                                   std::next(partner_contour_it->vertices.begin())->position};

            const bool f_angles_within_limits = check_all_merge_angles(*current_contour, *partner_contour_it, current_contour_segment,
                                                                       partner_contour_segment, calibrations);

            if (f_angles_within_limits)
            {
               min_end_distance_sq                    = current_end_to_partner_begin_dist_sq;
               nearest_partner_end_idx                = partner_idx;
               merge_flags.f_end_flip_partner_contour = false;
            }
         }
         if (current_end_to_partner_end_dist_sq < min_end_distance_sq)
         {
            const geometry::Segment2D_T current_contour_segment = {std::next(current_contour->vertices.rbegin())->position,
                                                                   current_contour->vertices.rbegin()->position};
            const geometry::Segment2D_T partner_contour_segment = {partner_contour_it->vertices.rbegin()->position,
                                                                   std::next(partner_contour_it->vertices.rbegin())->position};

            const bool f_angles_within_limits = check_all_merge_angles(*current_contour, *partner_contour_it, current_contour_segment,
                                                                       partner_contour_segment, calibrations);

            if (f_angles_within_limits)
            {
               min_end_distance_sq                    = current_end_to_partner_end_dist_sq;
               nearest_partner_end_idx                = partner_idx;
               merge_flags.f_end_flip_partner_contour = true;
            }
         }
      }

      // MISRA
      (void) current_contour;
      (void) nearest_partner_beginning_idx;
      (void) nearest_partner_end_idx;

      // decide which vertex of current contour to consider for merging with endpoint of other contours
      uint16_t partner_contour_idx                 = 0U;
      const float merge_distance_threshold_squared = calibrations.merge_distance_threshold * calibrations.merge_distance_threshold;

      if ((min_beginning_distance_sq < min_end_distance_sq) && (min_beginning_distance_sq < merge_distance_threshold_squared))
      {
         merge_flags.f_merge_beginnings = true;
         partner_contour_idx            = nearest_partner_beginning_idx;
      }
      else if ((min_beginning_distance_sq > min_end_distance_sq) && (min_end_distance_sq < merge_distance_threshold_squared))
      {
         merge_flags.f_merge_ends = true;
         partner_contour_idx      = nearest_partner_end_idx;
      }
      else
      {
         // MISRA
      }

      return partner_contour_idx;
   }

   void merge_two_contours(const Contour_it &destination_contour_it, const Contour_it &source_contour_it)
   {
      if (destination_contour_it->vertices.rbegin() != destination_contour_it->vertices.rend())
      {
         const auto &destination_last_vertex_position = destination_contour_it->vertices.rbegin()->position;
         const auto &source_first_vertex_position     = source_contour_it->vertices.begin()->position;

         const auto mean_pos_x = (destination_last_vertex_position.x + source_first_vertex_position.x) / 2.0F;
         const auto mean_pos_y = (destination_last_vertex_position.y + source_first_vertex_position.y) / 2.0F;
         const uint16_t mean_age = (destination_contour_it->vertices.rbegin()->age + source_contour_it->vertices.begin()->age) / 2U;

         auto mean_pos       = Vertex_T({mean_pos_x, mean_pos_y});
         mean_pos.segment_id = source_contour_it->vertices.begin()->segment_id;

         mean_pos.pos_cov.x =
            std::max(source_contour_it->vertices.begin()->pos_cov.x, destination_contour_it->vertices.rbegin()->pos_cov.x);
         mean_pos.pos_cov.xy =
            std::max(source_contour_it->vertices.begin()->pos_cov.xy, destination_contour_it->vertices.rbegin()->pos_cov.xy);
         mean_pos.pos_cov.y =
            std::max(source_contour_it->vertices.begin()->pos_cov.y, destination_contour_it->vertices.rbegin()->pos_cov.y);

         // zero cross-covariances
         mean_pos.pos_cross_cov                = Pos_2D_Cross_Cov();
         const auto last_but_one_vertex_it     = std::next(destination_contour_it->vertices.rbegin());
         last_but_one_vertex_it->pos_cross_cov = Pos_2D_Cross_Cov();

         mean_pos.age                  = mean_age;
         mean_pos.num_cycles_no_update = (destination_contour_it->vertices.rbegin()->num_cycles_no_update
                                          + source_contour_it->vertices.begin()->num_cycles_no_update)
                                         / 2U;
         mean_pos.reliability =
            0.5F * (destination_contour_it->vertices.rbegin()->reliability + source_contour_it->vertices.begin()->reliability);

         destination_contour_it->vertices.pop_back();
         source_contour_it->vertices.pop_front();

         (void) destination_contour_it->vertices.push_back(mean_pos);

         for (auto source_vertex = source_contour_it->vertices.begin(); source_vertex != source_contour_it->vertices.end();
              source_vertex++)
         {
            (void) destination_contour_it->vertices.move_back(source_contour_it->vertices, source_vertex);
         }
      }
   }

   bool determine_destination_contour(const Contour_it &current_contour_it,
                                      const Contour_it &partner_contour_it,
                                      Contour_it &destination_contour,
                                      Contour_it &source_contour)
   {
      bool f_contours_swapped = false;

      if (current_contour_it->vertices.size() > partner_contour_it->vertices.size())
      {
         destination_contour = current_contour_it;
         source_contour      = partner_contour_it;
         f_contours_swapped  = false;
      }
      else if (current_contour_it->vertices.size() < partner_contour_it->vertices.size())
      {
         destination_contour = partner_contour_it;
         source_contour      = current_contour_it;
         f_contours_swapped  = true;
      }
      else
      {
         const auto current_contour_max_age_vertex_it =
            std::max_element(current_contour_it->vertices.begin(), current_contour_it->vertices.end(),
                             [](const Vertex_T &lhs, const Vertex_T &rhs) { return lhs.age < rhs.age; });
         const auto current_contour_max_age = current_contour_max_age_vertex_it->age;

         const auto partner_contour_max_age_vertex_it =
            std::max_element(partner_contour_it->vertices.begin(), partner_contour_it->vertices.end(),
                             [](const Vertex_T &lhs, const Vertex_T &rhs) { return lhs.age < rhs.age; });
         const uint16_t partner_contour_max_age = partner_contour_max_age_vertex_it->age;

         if (current_contour_max_age >= partner_contour_max_age)
         {
            destination_contour = current_contour_it;
            source_contour      = partner_contour_it;
            f_contours_swapped  = false;
         }
         else
         {
            destination_contour = partner_contour_it;
            source_contour      = current_contour_it;
            f_contours_swapped  = true;
         }
      }
      return f_contours_swapped;
   }

   void flip_contour(const Contour_T &contour)
   {
      auto forward_vertex            = contour.vertices.begin();
      auto reverse_vertex            = contour.vertices.rbegin();
      const uint16_t number_of_swaps = contour.size() / 2U;

      for (uint16_t it = 0U; it < number_of_swaps; it++)
      {
         // step 1 - swap forward and reverse vertex
         std::swap(*forward_vertex, *reverse_vertex);

         // step 2 - swap last two position cross covariances
         auto next_reverse_vertex = reverse_vertex;
         next_reverse_vertex++;
         std::swap(next_reverse_vertex->pos_cross_cov, reverse_vertex->pos_cross_cov);
         std::swap(next_reverse_vertex->segment_id, reverse_vertex->segment_id);

         // step 3 condition - prevent swapping the same (last) pair twice (only for even contour size)
         if ((contour.size() % 2U == 1U) || (it != number_of_swaps - 1U))
         {
            // step 3 - swap first and last pos cross covariances
            std::swap(forward_vertex->pos_cross_cov, reverse_vertex->pos_cross_cov);
            std::swap(forward_vertex->segment_id, reverse_vertex->segment_id);
         }
         forward_vertex++;
         reverse_vertex = std::move(next_reverse_vertex);
      }
      // MISRA
      (void) forward_vertex;
      (void) reverse_vertex;
   }
}
