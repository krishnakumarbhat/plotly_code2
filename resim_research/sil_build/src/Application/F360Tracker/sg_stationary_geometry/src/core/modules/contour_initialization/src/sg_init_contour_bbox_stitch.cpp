/*=============================================================================================*\
* FILE: sg_init_contour_bbox_stitch.cpp
* ====================================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition for init_bbox_stitch and helper functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#include "sg_init_contour_bbox_stitch.h"

#include "geometry/geo_distance.h"
#include "geometry/geo_is_inside.h"

namespace sg
{
   void init_bbox_stitch(Contour_T::VertexList &vertices,
                         const Cluster *const current_cluster_ptr,
                         const float init_look_distance,
                         const float min_segment_length,
                         const geometry::Point2D_T host_position)
   {
      Vertex_T previous_vertex;
      std::pair<Vertex_T, bool> result;

      const auto contour_size = current_cluster_ptr->size();

      // get the consecutive vertices of contour
      for (auto i = 0U; i < contour_size; i++)
      {
         if (i == 0U)
         {
            const geometry::Point2D_T starting_pos = determine_starting_position(current_cluster_ptr);
            previous_vertex.position               = {starting_pos.x, starting_pos.y};
            (void) vertices.push_back(previous_vertex); // FZD-822: Handle situation when vertex couldn't be added
         }
         else
         {
            previous_vertex = result.first;
            (void) vertices.push_back(result.first); // FZD-822: Handle situation when vertex couldn't be added
         }

         const geometry::Rectangle_T roi_rect({previous_vertex.position.x, previous_vertex.position.y}, init_look_distance,
                                              init_look_distance, 0.0F);

         result = calculate_vertex_inside_roi(current_cluster_ptr, roi_rect, min_segment_length, host_position);

         if (result.second)
         {
            break;
         }
      }
   }

   void scale_to_max_distant_detection(Vertex_T &next_vertex,
                                       const geometry::Point2D_T previous_vertex_position,
                                       float scale_factor,
                                       const float min_segment_length,
                                       const geometry::Point2D_T host_position)
   {
      // if 2 detections have exactly the same lat / long position, next_vertex
      // can become equal to previous_vertex, in that case create a minimum length segment
      if ((fabsf(next_vertex.position.x - previous_vertex_position.x) <= std::numeric_limits<float>::epsilon())
          && (fabsf(next_vertex.position.y - previous_vertex_position.y) <= std::numeric_limits<float>::epsilon()))
      {
         calc_min_length_segment_vertex(next_vertex, previous_vertex_position, min_segment_length, host_position);
      }
      else
      {
         // in Matlab it's next_vertex - previous , but direction = [y , x] , beaouse previous_vertex = [y, x]
         geometry::Point2D_T direction = {next_vertex.position.x - previous_vertex_position.x,
                                          next_vertex.position.y - previous_vertex_position.y};
         const float norm_2D           = sqrtf((direction.x * direction.x) + (direction.y * direction.y));
         direction.x                   = direction.x / norm_2D;
         direction.y                   = direction.y / norm_2D;

         if (scale_factor < min_segment_length)
         {
            scale_factor = min_segment_length;
         }

         next_vertex.position.x = previous_vertex_position.x + direction.x * scale_factor;
         next_vertex.position.y = previous_vertex_position.y + direction.y * scale_factor;
      }
   }

   void calc_min_length_segment_vertex(Vertex_T &next_vertex,
                                       const geometry::Point2D_T previous_vertex_position,
                                       const float min_segment_length,
                                       const geometry::Point2D_T host_position)
   {
      // Calculate the normed direction vector towards host / in matlab [y, x]
      auto distance_vector = previous_vertex_position - host_position;
      const float norm_2D  = sqrtf((distance_vector.x * distance_vector.x) + (distance_vector.y * distance_vector.y));
      assert(norm_2D > 0.0F);
      distance_vector = distance_vector / norm_2D;
      // Calculate the perpendicular vector
      const geometry::Point2D_T direction = {distance_vector.y, -distance_vector.x};
      next_vertex.position.x              = previous_vertex_position.x + direction.x * min_segment_length;
      next_vertex.position.y              = previous_vertex_position.y + direction.y * min_segment_length;
   }

   std::pair<Vertex_T, bool> calculate_vertex_inside_roi(const Cluster *const current_cluster_ptr,
                                                         const geometry::Rectangle_T &region_of_interest,
                                                         const float min_segment_length,
                                                         const geometry::Point2D_T host_position)
   {
      bool f_terminate          = false;
      uint16_t det_counter      = 1U;
      float sum_x               = region_of_interest.center().x;
      float sum_y               = region_of_interest.center().y;
      float max_distance        = 0.0F;
      auto current_det          = current_cluster_ptr->begin();
      const auto num_detections = current_cluster_ptr->size();

      for (size_t i = 0U; i < num_detections; i++)
      {
         if (current_det->f_subset && (geometry::is_inside(region_of_interest, current_det->position)))
         {
            const float pos_x = current_det->position.x;
            const float pos_y = current_det->position.y;

            sum_x += pos_x;
            sum_y += pos_y;

            det_counter++;
            current_det->f_subset = false;

            const float distance = euclidean_distance({pos_x, pos_y}, region_of_interest.center());

            if (distance > max_distance)
            {
               max_distance = distance;
            }

            if ((fabsf(pos_x - region_of_interest.center().x) <= std::numeric_limits<float>::epsilon())
                && (fabsf(pos_y - region_of_interest.center().y) <= std::numeric_limits<float>::epsilon()))
            {
               f_terminate = true;
            }
         }
         ++current_det;
      }
      (void) current_det; // MISRA

      Vertex_T next_vertex;
      next_vertex.position.x = sum_x / static_cast<float>(det_counter);
      next_vertex.position.y = sum_y / static_cast<float>(det_counter);
      scale_to_max_distant_detection(next_vertex, region_of_interest.center(), max_distance, min_segment_length, host_position);

      if (!((det_counter == 2U) && f_terminate))
      {
         f_terminate = (det_counter == 1U);
      }

      return {next_vertex, f_terminate};
   }

   geometry::Point2D_T determine_starting_position(const Cluster *const current_cluster_ptr)
   {
      DetectionList::iterator lateral_min_det_it{};
      DetectionList::iterator longitudinal_max_det_it{};

      float lateral_max      = 0.0F;
      float lateral_min      = 0.0F;
      float longitudinal_max = 0.0F;
      float longitudinal_min = 0.0F;

      auto det_iter                = current_cluster_ptr->begin();
      const auto detections_end_it = current_cluster_ptr->end();

      for (; det_iter != detections_end_it; ++det_iter)
      {
         if (det_iter->f_subset)
         {
            lateral_max             = det_iter->position.y;
            lateral_min             = det_iter->position.y;
            longitudinal_max        = det_iter->position.x;
            longitudinal_min        = det_iter->position.x;
            lateral_min_det_it      = det_iter;
            longitudinal_max_det_it = det_iter;
            break;
         }
      }
      ++det_iter;

      for (; det_iter != detections_end_it; ++det_iter)
      {
         if (det_iter->f_subset)
         {
            const float position_x = det_iter->position.x;
            const float position_y = det_iter->position.y;

            if (position_y > lateral_max)
            {
               lateral_max = position_y;
            }
            else if (position_y < lateral_min)
            {
               lateral_min        = position_y;
               lateral_min_det_it = det_iter;
            }
            else
            {
               // MISRA
            }

            if (position_x > longitudinal_max)
            {
               longitudinal_max        = position_x;
               longitudinal_max_det_it = det_iter;
            }
            else if (position_x < longitudinal_min)
            {
               longitudinal_min = position_x;
            }
            else
            {
               // MISRA
            }
         }
      }

      const float lateral_spread      = lateral_max - lateral_min;
      const float longitudinal_spread = longitudinal_max - longitudinal_min;

      // if detections are spread more in longitudinal direction - we take the most distant detection
      // otherwise - we take the detection with smallest lateral coordinate
      const DetectionList::iterator starting_det = (longitudinal_spread >= lateral_spread) ? longitudinal_max_det_it
                                                                                           : lateral_min_det_it;
      const geometry::Point2D_T starting_pos     = {starting_det->position.x, starting_det->position.y};
      return starting_pos;
   }
}
