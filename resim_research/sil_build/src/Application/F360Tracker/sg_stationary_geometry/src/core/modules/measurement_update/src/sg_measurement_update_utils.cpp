/*=============================================================================================*\
* FILE: sg_measurement_update_utils.cpp
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definitions for measurement update utility functions
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#include "sg_measurement_update_utils.h"

#include <algorithm>

#include "geometry/geo_distance.h"
#include "geometry/geo_length.h"
#include "geometry/geo_point_on_segment_line.h"
#include "geometry/geo_projection.h"
#include "sg_matrix_operations.h"

namespace sg
{
   void collect_dets_assoc_to_first_and_last_segment(
      std::array<ContourAssociatedDets, SG_MAX_NUM_CONTOURS> &dets_and_contours_data_for_length_measurement,
      uint16_t &num_of_contours_with_assoc_ending_dets,
      const ContourStorage::ContourList::iterator &contour_it,
      const Detection_T &detection,
      const SegmentPlacement &segment_flag)
   {
      uint16_t idx = 0U;
      while ((idx < num_of_contours_with_assoc_ending_dets)
             && (contour_it->unique_id() != dets_and_contours_data_for_length_measurement[idx].contour_it->unique_id())
             && (idx < SG_MAX_NUM_CONTOURS))
      {
         ++idx;
      }

      if (idx < num_of_contours_with_assoc_ending_dets)
      {
         if ((segment_flag == SegmentPlacement::FIRST)
             && (dets_and_contours_data_for_length_measurement[idx].first_segment_num_dets < SG_MAX_NUM_ENDING_SEGMENT_DETS))
         {
            dets_and_contours_data_for_length_measurement[idx]
               .assoc_to_first_segment[dets_and_contours_data_for_length_measurement[idx].first_segment_num_dets++] = &detection;
         }
         if ((segment_flag == SegmentPlacement::LAST)
             && (dets_and_contours_data_for_length_measurement[idx].last_segment_num_dets < SG_MAX_NUM_ENDING_SEGMENT_DETS))
         {
            dets_and_contours_data_for_length_measurement[idx]
               .assoc_to_last_segment[dets_and_contours_data_for_length_measurement[idx].last_segment_num_dets++] = &detection;
         }
      }
      else
      {
         dets_and_contours_data_for_length_measurement[idx].contour_it = contour_it;
         if (segment_flag == SegmentPlacement::FIRST)
         {
            dets_and_contours_data_for_length_measurement[idx].assoc_to_first_segment[0U] = &detection;
            dets_and_contours_data_for_length_measurement[idx].first_segment_num_dets++;
         }
         if (segment_flag == SegmentPlacement::LAST)
         {
            dets_and_contours_data_for_length_measurement[idx].assoc_to_last_segment[0U] = &detection;
            dets_and_contours_data_for_length_measurement[idx].last_segment_num_dets++;
         }
         num_of_contours_with_assoc_ending_dets++;
      }
   }

   /**
    * @brief   Gets pseudomeasurement.
    *
    * @param   position
    * @param   length_measurement_noise
    *
    * @return  detection with position covariance set with length_measurement_noise
    **/
   static Detection_T get_pseudomeasurement(const geometry::Point2D_T &position, const float length_measurement_noise)
   {
      Detection_T result{};
      result.position.x      = position.x;
      result.position.y      = position.y;
      result.position_cov.x  = length_measurement_noise;
      result.position_cov.y  = length_measurement_noise;
      result.position_cov.xy = 0.0F;

      return result;
   }

   static std::size_t map_segment_id(const Contour_T &contour, const std::size_t segment_id)
   {
      std::size_t local_segment_id = 0U;
      const auto contour_size      = contour.size();
      auto vertex_it               = contour.vertices.begin();

      // Map global segment id to local (in range of contour).
      for (size_t vertex_idx = 0U; vertex_idx < contour_size; ++vertex_idx)
      {
         if (vertex_it->segment_id == segment_id)
         {
            local_segment_id = vertex_idx + 1U;
            break;
         }
         else
         {
            ++vertex_it;
         }
      }
      (void) vertex_it; // MISRA
      return local_segment_id;
   }

   std::array<sg::geometry::Footpoint_T, SG_MAX_NUM_ENDING_SEGMENT_DETS>
   calc_footpoints(const geometry::Segment2D_T &segment,
                   const uint16_t num_dets,
                   const std::array<const Detection_T *, SG_MAX_NUM_ENDING_SEGMENT_DETS> &dets)
   {
      std::array<sg::geometry::Footpoint_T, SG_MAX_NUM_ENDING_SEGMENT_DETS> foot_points{{}};

      for (uint16_t idx = 0U; idx < num_dets; ++idx)
      {
         const geometry::Point2D_T point{dets[idx]->position.x, dets[idx]->position.y};

         foot_points[idx] = geometry::make_projection(segment, point, false);
      }

      return foot_points;
   }

   // for one segment contours
   std::pair<Detection_T, Detection_T>
   calc_pseudomeasurement(const std::array<const Detection_T *, SG_MAX_NUM_ENDING_SEGMENT_DETS> &associated_dets,
                          const uint16_t &num_associated_dets,
                          const geometry::Segment2D_T &segment,
                          const Measurement_Update_Calibrations_T &calibrations)
   {
      std::pair<Detection_T, Detection_T> result{};

      if (num_associated_dets == 1U) // only one detection is associated
      {
         const geometry::Point2D_T detection_position{associated_dets[0]->position.x, associated_dets[0]->position.y};

         const sg::geometry::Footpoint_T normalized_length = sg::geometry::make_projection(segment, detection_position, false);
         const auto first_pseudo_measurement_position =
            calculate_point_on_segment_line(segment, normalized_length.s - calibrations.minimal_length_spread);
         const auto last_pseudo_measurement_position =
            calculate_point_on_segment_line(segment, normalized_length.s + calibrations.minimal_length_spread);

         result.first  = get_pseudomeasurement(first_pseudo_measurement_position, calibrations.length_measurement_noise);
         result.second = get_pseudomeasurement(last_pseudo_measurement_position, calibrations.length_measurement_noise);
      }
      else // more than one detection is associated
      {
         const std::array<sg::geometry::Footpoint_T, SG_MAX_NUM_ENDING_SEGMENT_DETS> normalized_lengths =
            calc_footpoints(segment, num_associated_dets, associated_dets);

         const auto first_last_pseudo_measure = std::minmax_element(
            normalized_lengths.begin(), std::next(normalized_lengths.begin(), static_cast<std::ptrdiff_t>(num_associated_dets)),
            [](const auto &lhs, const auto &rhs) { return (lhs.s < rhs.s); });

         const auto first_pseudo_measurement_position = calculate_point_on_segment_line(segment, first_last_pseudo_measure.first->s);
         const auto last_pseudo_measurement_position = calculate_point_on_segment_line(segment, first_last_pseudo_measure.second->s);

         result.first  = get_pseudomeasurement(first_pseudo_measurement_position, calibrations.length_measurement_noise);
         result.second = get_pseudomeasurement(last_pseudo_measurement_position, calibrations.length_measurement_noise);
      }
      return result;
   }

   // for multiple segment contours
   Detection_T calc_pseudomeasurement(const std::array<const Detection_T *, SG_MAX_NUM_ENDING_SEGMENT_DETS> &associated_dets,
                                      const uint16_t &num_associated_dets,
                                      const geometry::Segment2D_T segment,
                                      const SegmentPlacement &segment_place,
                                      const Measurement_Update_Calibrations_T &calibrations)
   {
      Detection_T result{};

      if (num_associated_dets == 1U)
      {
         const geometry::Point2D_T detection_position{associated_dets[0]->position.x, associated_dets[0]->position.y};
         const sg::geometry::Footpoint_T normalized_length = sg::geometry::make_projection(segment, detection_position, false);

         if (segment_place == SegmentPlacement::FIRST)
         {
            const geometry::Point2D_T pseudo_measurement_position =
               calculate_point_on_segment_line(segment, normalized_length.s - calibrations.minimal_length_spread);

            result = get_pseudomeasurement(pseudo_measurement_position, calibrations.length_measurement_noise);
         }
         if (segment_place == SegmentPlacement::LAST)
         {
            const geometry::Point2D_T pseudo_measurement_position =
               calculate_point_on_segment_line(segment, normalized_length.s + calibrations.minimal_length_spread);

            result = get_pseudomeasurement(pseudo_measurement_position, calibrations.length_measurement_noise);
         }
         (void) normalized_length; // MISRA
      }
      else
      {
         const std::array<sg::geometry::Footpoint_T, SG_MAX_NUM_ENDING_SEGMENT_DETS> normalized_lengths =
            calc_footpoints(segment, num_associated_dets, associated_dets);

         if (segment_place == SegmentPlacement::FIRST)
         {
            const auto last_pseudo_measure = std::min_element(
               normalized_lengths.begin(), std::next(normalized_lengths.begin(), static_cast<std::ptrdiff_t>(num_associated_dets)),
               [](const auto &lhs, const auto &rhs) { return (lhs.s < rhs.s); });

            const geometry::Point2D_T pseudo_measurement_position = calculate_point_on_segment_line(segment, last_pseudo_measure->s);

            result = get_pseudomeasurement(pseudo_measurement_position, calibrations.length_measurement_noise);
         }
         if (segment_place == SegmentPlacement::LAST)
         {
            const auto last_pseudo_measure = std::max_element(
               normalized_lengths.begin(), std::next(normalized_lengths.begin(), static_cast<std::ptrdiff_t>(num_associated_dets)),
               [](const auto &lhs, const auto &rhs) { return (lhs.s < rhs.s); });

            const geometry::Point2D_T pseudo_measurement_position = calculate_point_on_segment_line(segment, last_pseudo_measure->s);

            result = get_pseudomeasurement(pseudo_measurement_position, calibrations.length_measurement_noise);
         }
         (void) normalized_lengths; // MISRA
      }
      return result;
   }

   SubState extract_subcontour_data(const Contour_T &contour, const std::size_t segment_id)
   {
      SubState out_state{};

      const std::size_t local_segment_id = map_segment_id(contour, segment_id);

      if ((0U < local_segment_id) && (local_segment_id < contour.size()))
      {
         if (local_segment_id == 1U) // first segment of contour
         {
            out_state.begin_it     = contour.vertices.begin();
            out_state.size         = std::min(static_cast<std::size_t>(3U), contour.vertices.size());
            out_state.segment_flag = SegmentPlacement::FIRST;
         }
         else if (local_segment_id == contour.vertices.size() - static_cast<std::size_t>(1U)) // last segment of contour
         {
            out_state.begin_it =
               std::prev(contour.vertices.end(), static_cast<std::ptrdiff_t>(contour.vertices.size() - local_segment_id + 2U));
            out_state.size         = std::min(static_cast<std::size_t>(3U), contour.vertices.size());
            out_state.segment_flag = SegmentPlacement::LAST;
         }
         else // middle segment of contour
         {
            out_state.begin_it     = std::next(contour.vertices.begin(), static_cast<std::ptrdiff_t>(local_segment_id - 2U));
            out_state.size         = 4U;
            out_state.segment_flag = SegmentPlacement::INVALID;
         }

         out_state.segment_idx = (local_segment_id == 1U ? local_segment_id : 2U);
      }
      return out_state;
   }

   float calc_regularity(const sg::Contour_T::VertexList::iterator &vertex_it)
   {
      const sg::geometry::Point2D_T first_vertex{vertex_it->position};
      const sg::geometry::Point2D_T second_vertex{std::next(vertex_it)->position};
      const sg::geometry::Point2D_T third_vertex{std::next(vertex_it, 2)->position};

      const sg::geometry::Point2D_T middle_point = 0.5F * (third_vertex + first_vertex);

      return sg::geometry::squared_euclidean_distance(middle_point, second_vertex);
   }

   void regularize_subcontour(const SubState &contour_sub_state,
                              const float position_forgetting_factor,
                              const float regularity_weight,
                              const float regularity_threshold)
   {
      const float pseudo_measurement_variance = 1.0F / regularity_weight;
      const float R                           = pseudo_measurement_variance;

      switch (contour_sub_state.size)
      {
         case 3U:
            if (calc_regularity(contour_sub_state.begin_it) > regularity_threshold)
            {
               Matrix<float, 1U, 3U> H{};
               H[0U][1U]                     = 1.0F;
               Matrix<float, 3U, 2U> state   = create_pre_state<3U>(contour_sub_state);
               const float endpoint_center_y = 0.5F * (state[0U][0U] + state[2U][0U]);
               const float endpoint_center_x = 0.5F * (state[0U][1U] + state[2U][1U]);
               const Matrix<float, 1U, 2U> pseudo_measurement{{{endpoint_center_y, endpoint_center_x}}};

               Matrix<float, 3U, 3U> P = covariance_struct_to_matrix<3U>(contour_sub_state);

               kalman_update<3U, 2U>(state, P, H, R, pseudo_measurement, position_forgetting_factor);

               update_position(state, contour_sub_state);

               update_variance(P, contour_sub_state);
            }
            break;
         case 4U:
            // update first three vertices
            if (calc_regularity(contour_sub_state.begin_it) > regularity_threshold)
            {
               Matrix<float, 1U, 4U> H1{};
               H1[0U][1U]                    = 1.0F;
               Matrix<float, 4U, 2U> state   = create_pre_state<4U>(contour_sub_state);
               const float endpoint_center_y = 0.5F * (state[0U][0U] + state[2U][0U]);
               const float endpoint_center_x = 0.5F * (state[0U][1U] + state[2U][1U]);
               const Matrix<float, 1U, 2U> pseudo_measurement{{{endpoint_center_y, endpoint_center_x}}};

               Matrix<float, 4U, 4U> P = covariance_struct_to_matrix<4U>(contour_sub_state);

               kalman_update<4U, 2U>(state, P, H1, R, pseudo_measurement, position_forgetting_factor);

               update_position(state, contour_sub_state);

               update_variance(P, contour_sub_state);
            }

            // update last three vertices
            if (calc_regularity(std::next(contour_sub_state.begin_it)) > regularity_threshold)
            {
               Matrix<float, 1U, 4U> H2{};
               H2[0U][2U]                    = 1.0F;
               Matrix<float, 4U, 2U> state   = create_pre_state<4U>(contour_sub_state);
               const float endpoint_center_y = 0.5F * (state[1U][0U] + state[3U][0U]);
               const float endpoint_center_x = 0.5F * (state[1U][1U] + state[3U][1U]);
               const Matrix<float, 1U, 2U> pseudo_measurement{{{endpoint_center_y, endpoint_center_x}}};

               Matrix<float, 4U, 4U> P = covariance_struct_to_matrix<4U>(contour_sub_state);

               kalman_update<4U, 2U>(state, P, H2, R, pseudo_measurement, position_forgetting_factor);

               update_position(state, contour_sub_state);

               update_variance(P, contour_sub_state);
            }
            break;
         default:
            assert(false);
            break;
      }
   }
}