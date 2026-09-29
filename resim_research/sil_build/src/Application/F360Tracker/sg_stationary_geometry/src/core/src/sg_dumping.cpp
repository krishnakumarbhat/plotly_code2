#include "sg_dumping.h"

#include <cmath>

namespace sg
{
   namespace dumping
   {
      void dump(SG_Detection_Dump_T &dumped_detection, const Detection_T &detection)
      {
         dumped_detection.position.x          = detection.position.x;
         dumped_detection.position.y          = detection.position.y;
         dumped_detection.position.z          = detection.position.z;
         dumped_detection.position_cov.xx     = detection.position_cov.x;
         dumped_detection.position_cov.yy     = detection.position_cov.y;
         dumped_detection.position_cov.xy     = detection.position_cov.xy;
         dumped_detection.position_squeezed.x = detection.position_squeezed.x;
         dumped_detection.position_squeezed.y = detection.position_squeezed.y;
         dumped_detection.position_squeezed.z = detection.position_squeezed.z;

         dumped_detection.existence_probability           = detection.existence_probability;
         dumped_detection.probability_of_detection        = detection.probability_of_detection;
         dumped_detection.range_rate_compensated          = detection.range_rate_compensated;
         dumped_detection.importance                      = detection.importance;
         dumped_detection.db_scan.current_num_neighbors   = detection.current_num_neighbors;
         dumped_detection.db_scan.cumulated_num_neighbors = detection.cumulated_num_neighbors;

         dumped_detection.unique_id     = detection.unique_id;
         dumped_detection.contour_id    = detection.contour_id;
         dumped_detection.segment_id[0] = detection.segment_id[0];
         dumped_detection.segment_id[1] = detection.segment_id[1];
         dumped_detection.cluster_id    = detection.cluster_id;
         dumped_detection.age           = detection.age;

         dumped_detection.drivability = detection.drivability;
         dumped_detection.look_id     = detection.look_id;

         dumped_detection.db_scan.f_core               = detection.f_dbscan_core;
         dumped_detection.db_scan.f_visited            = detection.f_dbscan_visited;
         dumped_detection.f_used_in_measurement_update = detection.f_used_in_measurement_update;
         dumped_detection.f_valid                      = true; // TODO FZD-2026
      }

      void dump(SG_Vertex_Dump_T &dumped_vertex, const Vertex_T &vertex)
      {
         dumped_vertex.position.x         = vertex.position.x;
         dumped_vertex.position.y         = vertex.position.y;
         dumped_vertex.b_box_center.x     = vertex.bounding_box.center().x;
         dumped_vertex.b_box_center.y     = vertex.bounding_box.center().y;
         dumped_vertex.pos_cov.xx         = vertex.pos_cov.x;
         dumped_vertex.pos_cov.yy         = vertex.pos_cov.y;
         dumped_vertex.pos_cov.xy         = vertex.pos_cov.xy;
         dumped_vertex.pos_cross_cov.x1x2 = vertex.pos_cross_cov.x1x2;
         dumped_vertex.pos_cross_cov.y1y2 = vertex.pos_cross_cov.y1y2;
         dumped_vertex.pos_cross_cov.x1y2 = vertex.pos_cross_cov.x1y2;
         dumped_vertex.pos_cross_cov.y1x2 = vertex.pos_cross_cov.y1x2;

         dumped_vertex.b_box_length         = vertex.bounding_box.length();
         dumped_vertex.b_box_width          = vertex.bounding_box.width();
         dumped_vertex.b_box_rotation_angle = vertex.bounding_box.rotation_angle();
         dumped_vertex.reliability          = vertex.reliability;
         dumped_vertex.segment_id           = vertex.segment_id;
         dumped_vertex.age                  = vertex.age;
         dumped_vertex.num_cycles_no_update = vertex.num_cycles_no_update;
      }

      std::size_t dump(SG_Vertex_Dump_T (&dumped_vertices)[SG_MAX_NUM_VERTICES],
                       const Contour_T::VertexList &vertices,
                       std::size_t pos_offset)
      {
         for (const auto &vertex : vertices)
         {
            if (pos_offset >= SG_MAX_NUM_VERTICES)
            {
               assert(false);
               break;
            }

            dumping::dump(dumped_vertices[pos_offset], vertex);
            pos_offset++;
         }

         return pos_offset;
      }

      void dump(SG_Contour_Dump_T &dumped_contour, const Contour_T &contour)
      {
         dumped_contour.num_vertices          = contour.size();
         dumped_contour.unique_id             = contour.unique_id();
         dumped_contour.cluster_id            = contour.cluster_id;
         dumped_contour.f_selected_for_output = contour.f_selected_for_output;
         dumped_contour.priority              = contour.priority;
         dumped_contour.drivability           = static_cast<uint8_t>(contour.drivability);
      }

      void dump(SG_Vertex_Out_T &dumped_vertex, const dc::Fused_Vertex_T &vertex)
      {
         dumped_vertex.drivability            = vertex.drivability;
         dumped_vertex.drivability_confidence = static_cast<std::uint8_t>(std::round(vertex.drivability_confidence));
         dumped_vertex.position_x             = vertex.position.x;
         dumped_vertex.position_y             = vertex.position.y;
         dumped_vertex.position_variance_x    = vertex.pos_cov.x;
         dumped_vertex.position_variance_y    = vertex.pos_cov.y;
         dumped_vertex.position_covariance_xy = vertex.pos_cov.xy;
         dumped_vertex.cycles_since_created   = vertex.sg_age;
         dumped_vertex.cycles_since_coasted   = vertex.sg_cycles_since_coasted;
      }

      void dump(SG_Contour_Out_T &dumped_contour, const dc::Fused_Contour_T &contour)
      {
         dumped_contour.unique_id    = contour.get_id();
         dumped_contour.num_vertices = contour.vertices.size();
         dumped_contour.type         = SG_Contour_Type_T::POLYLINE;
      }
   }
}
