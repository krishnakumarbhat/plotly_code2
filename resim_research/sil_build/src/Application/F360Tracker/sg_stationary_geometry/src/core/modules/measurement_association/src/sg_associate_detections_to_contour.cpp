#include "sg_associate_detections_to_contour.h"

#include <cmath>
#include <limits>
#include <utility>

#include "geometry/geo_find_min_max.h"
#include "geometry/geo_is_inside.h"
#include "geometry/geo_length.h"

namespace sg
{
   /**
    * @brief        Calculates vertex reliability.
    *
    * @param[in]    association_impact
    *
    * @return       value of vertex reliability
    *
    **/
   static inline float calc_vertex_reliability(const float association_impact)
   {
      float result{0.0F};

      if (std::numeric_limits<float>::min() < std::abs(association_impact))
      {
         result = std::ceil((association_impact + 2.0F) / 3.0F);
      }

      return result;
   }

   /**
    * @brief    Updates vertex parameters.
    *
    * @param[out]      vertex
    * @param[in]       association_impact
    *
    **/
   static inline void update_vertex(sg::Vertex_T &vertex, const float association_impact)
   {
      vertex.reliability += calc_vertex_reliability(association_impact);
      vertex.num_cycles_no_update = 0U;
   }

   /**
    * @brief             Iterates through contour segments and associate detections to them if found.
    *
    * @param[in,out]     detections
    * @param[in]         contour
    * @param[in]         measurement_association_calibrations
    * @param[in]         common_calibrations
    *
    **/
   static void associate_detections_to_contour_segments(const DetectionStorage &detections,
                                                        const sg::Contour_T &contour,
                                                        const Measurement_Association_Calibrations_T &measurement_association_calibrations,
                                                        const Common_Calibrations_T &common_calibrations)
   {
      // Iterate over contour segments and associate detections.
      std::pair<float, float> association_impact{};

      const auto last_vtx_it            = std::prev(contour.vertices.end());
      const auto one_before_last_vtx_it = std::prev(last_vtx_it);
      uint16_t seg_idx                  = 0U;

      std::array<geometry::Point2D_T, SG_MAX_NUM_VERTICES_PER_CONTOUR> normal_vectors{};
      compute_normal_vectors(normal_vectors, contour, common_calibrations.host_position);

      for (auto vtx_it = contour.vertices.begin(); vtx_it != last_vtx_it; ++vtx_it)
      {
         float assoc_width_extension{};
         const sg::geometry::Segment2D_T segment{vtx_it->position, std::next(vtx_it)->position};
         const geometry::Point2D_T segm_begin_posn    = vtx_it->position;
         const geometry::Point2D_T segm_end_posn      = std::next(vtx_it)->position;
         const geometry::Point2D_T segment_vector     = segm_end_posn - segm_begin_posn;
         const geometry::Point2D_T normal_vector      = normal_vectors[seg_idx];
         const sg::geometry::Rectangle_T segment_bbox = create_segment_bounding_box(
            assoc_width_extension, contour, vtx_it, measurement_association_calibrations, segment_vector, normal_vector);
         vtx_it->bounding_box = segment_bbox;

         const std::pair<DetectionCache::collection_data_type::const_iterator, DetectionCache::collection_data_type::const_iterator> x_pos_interval{
            detections.begin(sg::geometry::find_min_x(segment_bbox)), detections.end(sg::geometry::find_max_x(segment_bbox))};

         associate_detections_to_segment(association_impact, measurement_association_calibrations,
                                         common_calibrations.r_position_covariance, x_pos_interval, contour, segment_bbox, segment,
                                         normal_vector, vtx_it->segment_id, vtx_it->age, assoc_width_extension);

         if (0.0F < association_impact.first)
         {
            update_vertex(*vtx_it, association_impact.first);
         }

         if (vtx_it != one_before_last_vtx_it)
         {
            association_impact.first  = association_impact.second;
            association_impact.second = 0.0F;
         }
         ++seg_idx;
      }

      if (0.0F < association_impact.second)
      {
         update_vertex(*last_vtx_it, association_impact.second); // update last vertex in contour
      }
      (void) one_before_last_vtx_it; // MISRA
      (void) seg_idx;                // MISRA
   }

   void associate_detections_to_contour(const ContourStorage &contours,
                                        const DetectionStorage &detections,
                                        const Measurement_Association_Calibrations_T &measurement_association_calibrations,
                                        const Common_Calibrations_T &common_calibrations)
   {
      for (auto &contour : contours)
      {
         associate_detections_to_contour_segments(detections, contour, measurement_association_calibrations, common_calibrations);

         set_dominant_cluster_id(contour, detections, measurement_association_calibrations.min_dets_assoc_update_cluster_id);
      }
   }
}
