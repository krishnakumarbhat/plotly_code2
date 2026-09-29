#include "sg_measurement_association_utils.h"

#include "geometry/geo_distance.h"
#include "geometry/geo_is_inside.h"
#include "geometry/geo_length.h"
#include "geometry/geo_projection.h"
#include "geometry/geo_rotate.h"
#include "sg_common.h"

namespace sg
{
   /**
    * @brief    Updates segments ids the detection has been associated to.
    *
    * @param    segments
    * @param    segment_id
    *
    **/
   static inline void set_segment_id(std::array<uint32_t, 2U> &segments, const uint32_t segment_id)
   {
      if (segments[0U] == INVALID_SEGMENT_ID)
      {
         segments[0U] = segment_id;
      }
      else if (((segments[1U] == INVALID_SEGMENT_ID) && (segments[0U] != segment_id)))
      {
         segments[1U] = segment_id;
      }
      else
      {
         assert(true); // Trying to associate second time to the same segment
      }
   }

   void associate_detections_to_segment(std::pair<float, float> &association_impact,
                                        const Measurement_Association_Calibrations_T &measurement_association_calibrations,
                                        const R_Position_Covariance_T &r_position_covariance,
                                        const det_x_pos_interval &x_pos_interval,
                                        const Contour_T &contour,
                                        const geometry::Rectangle_T &segment_bounding_box,
                                        const geometry::Segment2D_T &segment,
                                        const geometry::Point2D_T normal_vector,
                                        const uint32_t &segment_id,
                                        const uint16_t &vertex_age,
                                        const float assoc_width_extension)
   {
      if (x_pos_interval.first != x_pos_interval.second)
      {
         const auto contour_unique_id = contour.unique_id();

         geometry::Rectangle_T rotated_segment_bbox{segment_bounding_box};
         geometry::rotate(rotated_segment_bbox, -rotated_segment_bbox.rotation_angle());
         const geometry::Intervals_T rotated_segment_bbox_intervals{rotated_segment_bbox};
         const auto segment_bounding_box_rotation_angle = segment_bounding_box.rotation_angle();
         const auto cos_angle                           = cosf(-segment_bounding_box_rotation_angle);
         const auto sin_angle                           = sinf(-segment_bounding_box_rotation_angle);
         const Matrix<float, 2U, 2U> rotation_matrix{{{cos_angle, -sin_angle}, {sin_angle, cos_angle}}};

         for (auto det_it = x_pos_interval.first; det_it != x_pos_interval.second; ++det_it)
         {
            auto &detection = **det_it;

            if (detection.drivability == contour.drivability)
            {
               geometry::Point2D_T rotated_detection_position(detection.position);
               geometry::rotate(rotated_detection_position, rotation_matrix);
               if (geometry::is_inside(rotated_segment_bbox_intervals, rotated_detection_position))
               {
                  const auto foot_point                          = geometry::make_projection(segment, detection.position, false);
                  const geometry::Point2D_T det_footpoint_vector = detection.position - foot_point.point;
                  const float det_to_segment_distance            = det_footpoint_vector * normal_vector;
                  const bool f_det_assigned_to_this_or_none_contour =
                     ((detection.contour_id == INVALID_CONTOUR_ID) || (detection.contour_id == contour_unique_id));

                  if (f_det_assigned_to_this_or_none_contour || (std::abs(det_to_segment_distance) < detection.distance_to_contour))
                  {
                     // if we are overwriting association, clear segment data
                     if (!f_det_assigned_to_this_or_none_contour)
                     {
                        detection.segment_id[0U] = INVALID_SEGMENT_ID;
                        detection.segment_id[1U] = INVALID_SEGMENT_ID;
                     }

                     set_segment_id(detection.segment_id, segment_id);
                     detection.contour_id = contour_unique_id;
                     detection.vertex_age = vertex_age;
                     set_detection_covariances(detection, measurement_association_calibrations, r_position_covariance,
                                               det_to_segment_distance, assoc_width_extension);

                     const auto assoc_impact_tmp =
                        calculate_association_impact(detection.position, segment, VERTEX_ASSOCIATION_IMPACT_TYPE::PROPORTIONAL);
                     association_impact.first += assoc_impact_tmp.first;
                     association_impact.second += assoc_impact_tmp.second;
                  }
               }
            }
         }
         (void) contour_unique_id; // MISRA
         (void) rotation_matrix;   // MISRA
      }
   }

   void set_detection_covariances(Detection_T &detection,
                                  const Measurement_Association_Calibrations_T &calibrations,
                                  const R_Position_Covariance_T &r_position_covariance,
                                  const float det_to_segment_distance,
                                  const float assoc_width_extension)
   {
      detection.distance_to_contour = std::abs(det_to_segment_distance);

      if (det_to_segment_distance > 0.0F)
      {
         detection.position_cov.x = (-(r_position_covariance.x - calibrations.extended_meas_covariance_xx) / assoc_width_extension)
                                       * det_to_segment_distance
                                    + r_position_covariance.x;
         detection.position_cov.y = (-(r_position_covariance.y - calibrations.extended_meas_covariance_yy) / assoc_width_extension)
                                       * det_to_segment_distance
                                    + r_position_covariance.y;

         assert(detection.position_cov.x >= 0.0F);
         assert(detection.position_cov.y >= 0.0F);
      }
   }

   std::pair<float, float> calculate_association_impact(const geometry::Point2D_T &det_position,
                                                        const geometry::Segment2D_T &segment,
                                                        const VERTEX_ASSOCIATION_IMPACT_TYPE impact_type)
   {
      std::pair<float, float> result{};

      if (impact_type == VERTEX_ASSOCIATION_IMPACT_TYPE::PROPORTIONAL)
      {
         const auto diff_first  = det_position - segment.first;
         const auto diff_second = det_position - segment.second;

         const float distance_to_first_vertex  = std::abs(diff_first.x) + std::abs(diff_first.y);
         const float distance_to_second_vertex = std::abs(diff_second.x) + std::abs(diff_second.y);

         const float sum_of_distances = distance_to_first_vertex + distance_to_second_vertex;
         result.first                 = distance_to_second_vertex / sum_of_distances;
         result.second                = distance_to_first_vertex / sum_of_distances;
      }
      else
      {
         assert(false);
         result.first  = 0.0F;
         result.second = 0.0F;
      }

      return result;
   }

   std::pair<uint16_t, uint16_t> get_most_frequent_cluster_id(const DetectionStorage &detections, const uint32_t contour_id)
   {
      std::pair<uint16_t, uint16_t> result{}; // pair.first -> number of ocuurences of value; pair.second -> value
      std::array<uint16_t, 2U * SG_MAX_NUM_INTERNAL_CLUSTERS + 1U> count_of_cluster_ids{};

      for (const auto &det : detections) // bucket sort
      {
         if ((det.contour_id == contour_id) && (det.cluster_id != INVALID_CLUSTER_ID))
         {
            assert(det.cluster_id < count_of_cluster_ids.size());
            count_of_cluster_ids[det.cluster_id]++;

            if (result.first < count_of_cluster_ids[det.cluster_id])
            {
               result.first  = count_of_cluster_ids[det.cluster_id];
               result.second = det.cluster_id;
            }
         }
      }
      (void) count_of_cluster_ids; // MISRA
      return result;
   }

   geometry::Rectangle_T create_segment_bounding_box(float &assoc_width_extension,
                                                     const Contour_T &contour,
                                                     const Contour_T::VertexList::iterator &segment_first_vtx,
                                                     const Measurement_Association_Calibrations_T &calibrations,
                                                     const geometry::Point2D_T segment_vector,
                                                     const geometry::Point2D_T normal_vector)
   {
      const float segment_length                = std::hypot(segment_vector.x, segment_vector.y);
      const geometry::Point2D_T segm_begin_posn = segment_first_vtx->position;
      const geometry::Point2D_T segm_end_posn   = std::next(segment_first_vtx)->position;
      geometry::Point2D_T bbox_center{};

      float segment_bbox_length{};
      const float assoc_width = calculate_association_width(segm_begin_posn.x, calibrations.width_min, calibrations.width_max,
                                                            calibrations.dynamic_gate_lower_distance,
                                                            calibrations.dynamic_gate_upper_distance, calibrations.f_dynamic_gates);

      assoc_width_extension = calculate_association_width(segm_begin_posn.x, calibrations.width_extension_min,
                                                          calibrations.width_extension_max, calibrations.dynamic_gate_lower_distance,
                                                          calibrations.dynamic_gate_upper_distance, calibrations.f_dynamic_gates);

      float segment_bbox_width = (0.5F * assoc_width) + assoc_width_extension;


      if (segment_first_vtx == contour.vertices.begin()) // for begin segment
      {
         segment_bbox_length = segment_length
                               + calculate_association_length_margin(calibrations.length_margin_min, calibrations.length_margin_max)
                               + calibrations.length_margin_ending;
         const float shift_factor = (segment_bbox_length / 2.0F) - calibrations.length_margin_ending;
         bbox_center              = segm_begin_posn + segment_vector * (shift_factor / segment_length);
      }
      else if (segment_first_vtx == std::prev(contour.vertices.end(), 2)) // for end segment
      {
         segment_bbox_length = segment_length
                               + calculate_association_length_margin(calibrations.length_margin_min, calibrations.length_margin_max)
                               + calibrations.length_margin_ending;
         const float shift_factor =
            (segment_bbox_length / 2.0F)
            - calculate_association_length_margin(calibrations.length_margin_min, calibrations.length_margin_max);
         bbox_center = segm_begin_posn + segment_vector * (shift_factor / segment_length);
      }
      else // for middle segments
      {
         segment_bbox_length =
            segment_length
            + (2.0F * (calculate_association_length_margin(calibrations.length_margin_min, calibrations.length_margin_max)));
         bbox_center = (segm_begin_posn + segm_end_posn) / 2.0F;
      }
      (void) segm_end_posn; // MISRA

      bbox_center = bbox_center + (0.5F * (assoc_width_extension - 0.5F * assoc_width)) * normal_vector;

      const float segment_angle = std::atan2(segment_vector.y, segment_vector.x);

      return geometry::Rectangle_T(bbox_center, segment_bbox_width, segment_bbox_length, segment_angle);
   }

   void compute_normal_vectors(std::array<geometry::Point2D_T, SG_MAX_NUM_VERTICES_PER_CONTOUR> &normal_vectors,
                               const Contour_T &contour,
                               const geometry::Point2D_T &host_position)
   {
      int16_t average_normed_dot_product{};

      const auto last_vtx_it = std::prev(contour.vertices.end());
      uint16_t seg_idx       = 0U;

      for (auto vtx_it = contour.vertices.begin(); vtx_it != last_vtx_it; ++vtx_it)
      {
         const geometry::Point2D_T segment_begin_position = vtx_it->position;
         const geometry::Point2D_T segment_end_position   = std::next(vtx_it)->position;
         const geometry::Point2D_T segment_vector         = segment_end_position - segment_begin_position;
         const geometry::Point2D_T segment_midpoint       = 0.5F * (segment_end_position + segment_begin_position);
         const float segment_length                       = std::hypot(segment_vector.x, segment_vector.y);

         const geometry::Point2D_T host_to_segment_midpoint = segment_midpoint - host_position;
         const geometry::Point2D_T segment_direction_vector = segment_vector / segment_length;

         // create normal vector (counter clockwise rotation = swap x/y and switch sign of x)
         geometry::Point2D_T normal_vector = segment_direction_vector;
         std::swap(normal_vector.x, normal_vector.y);
         normal_vector.x *= -1.0F;

         // dot product to determine if normal is facing host
         const float dot_product = host_to_segment_midpoint * normal_vector;

         const int16_t normed_dot_product = (dot_product > 0.0F) ? static_cast<int16_t>(1) : static_cast<int16_t>(-1);
         average_normed_dot_product += normed_dot_product;
         normal_vectors[seg_idx] = normal_vector;
         ++seg_idx;
      }
      (void) seg_idx; // MISRA

      if (average_normed_dot_product > 0)
      {
         // flip normal vectors
         for (auto &normal_vector : normal_vectors)
         {
            normal_vector = -1.0F * normal_vector;
         }
      }
   }
}
