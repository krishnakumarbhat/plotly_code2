/*===================================================================================*\
* FILE:  f360_assign_underdrivability_status_to_tracks_sg.cpp
*====================================================================================

* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.

* Confidential - Restricted Aptiv information. Do not disclose."

*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains Assign_Underdrivability_Status_To_Tracks_SG() function implementation.
*
*

* Applicable Standards (in order of precedence: highest first):

*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]

*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]

***/

#include "f360_assign_underdrivability_status_to_tracks_sg.h"
#include "f360_get_wall_time.h"
#include "f360_convert_object_prev_vcs_to_current_vcs.h"

namespace f360_variant_A
{
   static float32_t Get_Dist_Between_Vertices(const SG_Vertex_VCS& curr_vertex, const SG_Vertex_VCS& prev_vertex);
   static float32_t Get_Segment_Orientation(const SG_Vertex_VCS& curr_vertex, const  SG_Vertex_VCS& prev_vertex);
   static Point Get_Segment_Center_Point(const SG_Vertex_VCS& curr_vertex, const SG_Vertex_VCS& prev_vertex);
   static void Convert_Vertex_ISO_Pos_To_Vcs(const float32_t dist_rear_axle_to_vcs_m, const sg::SG_Vertex_Out_T& iso_vertex, SG_Vertex_VCS & vcs_vertex);
   static SG_Vertex_VCS Time_Update_Vertex(
      const sg::SG_Vertex_Out_T& curr_vertex_iso,
      const float32_t host_dist_rear_axle_to_vcs_m,
      const F360_Host_Props_T& host_props);
   static float32_t Get_Sq_Dist_Point_To_Segment(
      const SG_Vertex_VCS& vertex_a,
      const SG_Vertex_VCS& vertex_b,
      const Point& position);

   /*===========================================================================*\
   * FUNCTION: Assign_Underdrivability_Status_To_Stationary_Object_SG()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Tracker_Info_T& tracker_info
   * const sg::SG_Output_T& sg_output
   * const float32_t host_dist_rear_axle_to_vcs_m
   * F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function assigns a drivability status to stationary objects using the segments provided by Stationary Geometries (SG).
   * For a given segment (consisting of two vertices), objects in its vicinity are associated and and assigned the
   * drivability status and confidence of the segment. If an object is associated to more than one segment, the object
   * is assigned the status of the closest segment. When distances are similar (difference < 0.1m),
   * the segment with the highest confidence is preferred.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Assign_Underdrivability_Status_To_Stationary_Object_SG(
      const F360_Tracker_Info_T& tracker_info,
      const F360_Host_Props_T& host_props,
      const sg::SG_Output_T& sg_output,
      const float32_t host_dist_rear_axle_to_vcs_m,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS])
   {

      constexpr uint8_t default_movable_stopped_confidence = 75U;
      for (uint32_t i = 0U; i < static_cast<uint32_t>(tracker_info.num_active_objs); i++)
      {
         const int32_t obj_idx = tracker_info.active_obj_ids[i] - 1;
         if (!object_tracks[obj_idx].f_moving)
         {
            // Reset underdrivable status of all stationary objects.
            object_tracks[obj_idx].drivable_status_sg = sg::SG_Drivability_Class_T::UNCLASSIFIED;
            object_tracks[obj_idx].drivable_confidence_sg = 0U;
            object_tracks[obj_idx].drivable_sg_dist_to_segment_sq = INFTY;

            // Default underdrivable status to NONDRIVABLE with a confidence of 75% for all non-moving but movable objects.
            if (object_tracks[obj_idx].movable_prob > 0.5F)
            {
               object_tracks[obj_idx].drivable_status_sg = sg::SG_Drivability_Class_T::NONDRIVABLE;
               object_tracks[obj_idx].drivable_confidence_sg = default_movable_stopped_confidence;
            }
         }
      }

      constexpr float32_t classification_gate_width = 2.0F;
      uint16_t vertexIndex = 0U;
      SG_Vertex_VCS curr_vertex_vcs = {};
      
      for (uint16_t i = 0U; (i < sg_output.num_contours) && (i < SG_MAX_NUM_OUTPUT_CONTOURS); ++i)
      {
         const sg::SG_Contour_Out_T contour = sg_output.contours[i];
         for (uint16_t j = 0U; (j < contour.num_vertices) && (j < SG_MAX_NUM_OUTPUT_VERTICES); ++j)
         {
             const SG_Vertex_VCS prev_vertex_vcs = curr_vertex_vcs;
             curr_vertex_vcs = Time_Update_Vertex(sg_output.vertices[vertexIndex], host_dist_rear_axle_to_vcs_m,host_props);
             vertexIndex++;

            if (j != 0U)
            {
               // Only consider segments that have a drivable classification.
               // Segment's drivability classification is decided by first vertex in the list.
               const sg::SG_Drivability_Class_T segment_drivability_status = prev_vertex_vcs.drivability;
               const uint8_t segment_drivability_confidence = prev_vertex_vcs.drivability_confidence;
               const bool f_segment_relevant_for_drivable_classification = ((sg::SG_Drivability_Class_T::UNCLASSIFIED != segment_drivability_status)&&(segment_drivability_confidence > 0U));
               if (f_segment_relevant_for_drivable_classification)
               {
                  const BoundingBox classification_gate = Create_Classification_Gate_From_Segment(prev_vertex_vcs, curr_vertex_vcs, classification_gate_width);

                  // Find corners of the VCS box that contains the segment.
                  const float32_t max_segment_x_pos = std::max(curr_vertex_vcs.vcs_position_x, prev_vertex_vcs.vcs_position_x);
                  const float32_t min_segment_x_pos = std::min(curr_vertex_vcs.vcs_position_x, prev_vertex_vcs.vcs_position_x);
                  const float32_t max_segment_y_pos = std::max(curr_vertex_vcs.vcs_position_y, prev_vertex_vcs.vcs_position_y);
                  const float32_t min_segment_y_pos = std::min(curr_vertex_vcs.vcs_position_y, prev_vertex_vcs.vcs_position_y);

                  // Loop over objects sorted by x pos.
                  const F360_Object_Track_T* p_curr_obj = tracker_info.vcslong_sorted_start;
                  if ((min_segment_x_pos > 0.0F + classification_gate_width) && (tracker_info.vcslong_sorted_first_infront_of_host != NULL))
                  {
                     // If the whole segment is in front of host, start at the first object with x pos above 0.
                     p_curr_obj = tracker_info.vcslong_sorted_first_infront_of_host;
                  }

                  if (NULL != p_curr_obj)
                  {
                     for (uint32_t k = 0U; k < static_cast<uint32_t>(tracker_info.num_active_objs); k++)
                     {
                        if ((NULL == p_curr_obj) || (p_curr_obj->vcs_position.x > (max_segment_x_pos + classification_gate_width)))
                        {
                           break; // No more possible object candidates.
                        }
                        else if (!p_curr_obj->f_moving)
                        {
                           // Rough gate: check if the objects is in the VCS box containing the segment.
                           const bool object_in_segment_vicinity = ((p_curr_obj->vcs_position.x > (min_segment_x_pos - classification_gate_width))
                              && (p_curr_obj->vcs_position.y > (min_segment_y_pos - classification_gate_width))
                              && (p_curr_obj->vcs_position.y < (max_segment_y_pos + classification_gate_width)));
                           if (object_in_segment_vicinity)
                           {
                              // Check if object is inside the classification box.
                              if (classification_gate.Contains(p_curr_obj->vcs_position))
                              {
                                 const float32_t dist_to_segment_sq = Get_Sq_Dist_Point_To_Segment(prev_vertex_vcs, curr_vertex_vcs, p_curr_obj->vcs_position);
                                 Assign_Object_Drivable_Status_And_Conf_From_SG_Segment(segment_drivability_status, segment_drivability_confidence, dist_to_segment_sq, object_tracks[p_curr_obj->id - 1]);
                              }
                           }
                           else
                           {
                              // Do nothing, object is not close enough to segment to check if it's contained.
                           }
                        }
                        else
                        {
                           // Do nothing, object is moveable.
                        }
                        // Process next object in the sorted list. Break if it has reached the end.
                        p_curr_obj = tracker_info.vcslong_sorted_next_track[p_curr_obj->id - 1];
                     }
                  } 
               }
               else
               {
                  // Do nothing, segment is not classified, so no reason to associate objects to it.
               }
            }
         }
      }
   }

   static float32_t Get_Dist_Between_Vertices(const SG_Vertex_VCS& curr_vertex, const SG_Vertex_VCS& prev_vertex)
   {
      return F360_Get_Hypotenuse(curr_vertex.vcs_position_y - prev_vertex.vcs_position_y, curr_vertex.vcs_position_x - prev_vertex.vcs_position_x);
   }
   static float32_t Get_Segment_Orientation(const SG_Vertex_VCS& curr_vertex, const SG_Vertex_VCS& prev_vertex)
   {
      return F360_Atan2f((curr_vertex.vcs_position_y - prev_vertex.vcs_position_y), (curr_vertex.vcs_position_x - prev_vertex.vcs_position_x));
   }

   static Point Get_Segment_Center_Point(const SG_Vertex_VCS& curr_vertex, const SG_Vertex_VCS& prev_vertex)
   {
      return Point((prev_vertex.vcs_position_x + curr_vertex.vcs_position_x) * 0.5F, (prev_vertex.vcs_position_y + curr_vertex.vcs_position_y) * 0.5F);
   }

   static void Convert_Vertex_ISO_Pos_To_Vcs(const float32_t dist_rear_axle_to_vcs_m, const sg::SG_Vertex_Out_T & iso_vertex, SG_Vertex_VCS & vcs_vertex)
   {
      vcs_vertex.vcs_position_x = iso_vertex.position_x - dist_rear_axle_to_vcs_m;
      vcs_vertex.vcs_position_y = iso_vertex.position_y * -1.0F;
      vcs_vertex.drivability = iso_vertex.drivability;
      vcs_vertex.drivability_confidence = iso_vertex.drivability_confidence;
      vcs_vertex.position_variance_x = iso_vertex.position_variance_x;
      vcs_vertex.position_variance_y = iso_vertex.position_variance_y;
      vcs_vertex.position_covariance_xy = iso_vertex.position_covariance_xy;
   }

   static SG_Vertex_VCS Time_Update_Vertex(
       const sg::SG_Vertex_Out_T& curr_vertex_iso,
       const float32_t host_dist_rear_axle_to_vcs_m,
       const F360_Host_Props_T& host_props)
   {
       SG_Vertex_VCS curr_vertex_vcs = {};
       Point one_time_frame_delayed_sg_output_vertex;
       Point host_motion_compnesated_sg_output_vertex;

       Convert_Vertex_ISO_Pos_To_Vcs(host_dist_rear_axle_to_vcs_m, curr_vertex_iso, curr_vertex_vcs);

       // SG provide Us with vertices from previous time frame
       // To accomodate that we transform position of the vertex using host motion
       one_time_frame_delayed_sg_output_vertex = Point(curr_vertex_vcs.vcs_position_x, curr_vertex_vcs.vcs_position_y);
       host_motion_compnesated_sg_output_vertex = Transform_Point_From_Prev_To_Current_Vcs(one_time_frame_delayed_sg_output_vertex, host_props);

       curr_vertex_vcs.vcs_position_x = host_motion_compnesated_sg_output_vertex.x;
       curr_vertex_vcs.vcs_position_y = host_motion_compnesated_sg_output_vertex.y;

       return curr_vertex_vcs;
   }
   /*===========================================================================*\
   * FUNCTION: Get_Sq_Dist_Point_To_Segment()
   *===========================================================================
   * RETURN VALUE:
   * float32_t - squared distance from a point to the closest point on the finite segment [m^2]
   *
   * PARAMETERS:
   * const SG_Vertex_VCS& vertex_a - first vertex of the segment
   * const SG_Vertex_VCS& vertex_b - second vertex of the segment
   * const Point& position - point to compute distance from
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Computes the squared shortest distance from a point to a finite line segment
   * defined by two vertices. The projection of the point onto the segment line is
   * clamped to the segment endpoints, so the distance is always to a point on the
   * segment itself, not on the infinite extension of the line.
   * Returns squared distance to avoid costly square root computation.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   static float32_t Get_Sq_Dist_Point_To_Segment(
      const SG_Vertex_VCS& vertex_a,
      const SG_Vertex_VCS& vertex_b,
      const Point& position)
   {
      // Segment direction vector AB
      const float32_t ab_x = vertex_b.vcs_position_x - vertex_a.vcs_position_x;
      const float32_t ab_y = vertex_b.vcs_position_y - vertex_a.vcs_position_y;

      // Vector from segment start A to the query point P
      const float32_t ap_x = position.x - vertex_a.vcs_position_x;
      const float32_t ap_y = position.y - vertex_a.vcs_position_y;

      // Squared length of segment AB (used as denominator for projection)
      const float32_t ab_dot_ab = F360_Get_Hypotenuse_Squared(ab_x, ab_y);

      float32_t closest_x;
      float32_t closest_y;

      if (ab_dot_ab < F360_EPSILON)
      {
         // Degenerate segment (zero length) - distance to vertex_a
         closest_x = vertex_a.vcs_position_x;
         closest_y = vertex_a.vcs_position_y;
      }
      else
      {
         /*Compute projection parameter t = dot(AP, AB) / dot(AB, AB).
         t represents where point P projects onto the line through A and B:
         Clamping t to [0, 1] ensures we measure distance to the finite segment,
         not to the infinite line extension.*/
         const float32_t t = F360_Saturate(((ap_x * ab_x) + (ap_y * ab_y)) / ab_dot_ab, 0.0F, 1.0F);

         // Closest point on segment: A + t * AB
         closest_x = vertex_a.vcs_position_x + (t * ab_x);
         closest_y = vertex_a.vcs_position_y + (t * ab_y);
      }

      // Squared Euclidean distance from query point P to the closest point on the segment
      const float32_t dx = position.x - closest_x;
      const float32_t dy = position.y - closest_y;
      const float32_t dist_to_segment_sq = F360_Get_Hypotenuse_Squared(dx, dy);
      return dist_to_segment_sq;
   }

   /*===========================================================================*\
   * FUNCTION: Create_Classification_Gate_From_Segment()
   *===========================================================================
   * RETURN VALUE:
   * BoundingBox
   *
   * PARAMETERS:
   * const SG_Vertex_VCS & prev_vertex,
   * const SG_Vertex_VCS & curr_vertex,
   * const float32_t classification_gate_width
   * 
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function creates a bounding box around the segment using its length and orientation.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   BoundingBox Create_Classification_Gate_From_Segment(
      const SG_Vertex_VCS & prev_vertex,
      const SG_Vertex_VCS & curr_vertex,
      const float32_t classification_gate_width)
   {
      const float32_t segment_length = Get_Dist_Between_Vertices(curr_vertex, prev_vertex);
      const float32_t segment_orientation = Get_Segment_Orientation(curr_vertex, prev_vertex);
      const Point segment_center = Get_Segment_Center_Point(curr_vertex, prev_vertex);

      // Extend the segment ends by 1m in both directions to allow objects just nect to segment end to get classified
      constexpr float32_t length_buffer = 2.0F;

      return BoundingBox(segment_center, segment_length + length_buffer, classification_gate_width, Angle(segment_orientation));
   }

   /*===========================================================================*\
   * FUNCTION: Assign_Object_Drivable_Status_And_Conf_From_SG_Segment()
   *===========================================================================
   * RETURN VALUE:
   * sg::SG_Drivability_Class_T
   *
   * PARAMETERS:
   * const sg::SG_Drivability_Class_T segment_drivability_status
   * const uint8_t segment_drivability_confidence
   * const float32_t dist_to_segment_sq
   * F360_Object_Track_T& object
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function assignes drivability status and confidence to an object from its associated SG segment.
   * When the new segment is significantly closer to the object (squared distance difference > 0.01m^2,
   * equivalent to ~0.1m linear), the closer segment wins regardless of confidence. When distances are
   * similar, the segment with the higher confidence is preferred. When confidence is also equal, the
   * strictly closer segment wins. If all three are identical, the existing assignment is kept.
   *
   * PRECONDITIONS:
   * None.
   *
   * POSTCONDITIONS:
   * None.
   *
   \*===========================================================================*/
   void Assign_Object_Drivable_Status_And_Conf_From_SG_Segment(
      const sg::SG_Drivability_Class_T segment_drivability_status,
      const uint8_t segment_drivability_confidence,
      const float32_t dist_to_segment_sq,
      F360_Object_Track_T& object)
   {
      // 0.01 m^2 corresponds to 0.1m linear distance threshold
      constexpr float32_t k_sg_dist_sq_significance_threshold = 0.01F;

      const float32_t dist_sq_diff = object.drivable_sg_dist_to_segment_sq - dist_to_segment_sq;

      if (dist_sq_diff > k_sg_dist_sq_significance_threshold)
      {
         // New segment is significantly closer — override regardless of confidence
         object.drivable_status_sg = segment_drivability_status;
         object.drivable_confidence_sg = segment_drivability_confidence;
         object.drivable_sg_dist_to_segment_sq = dist_to_segment_sq;
      }
      else if (dist_sq_diff < -k_sg_dist_sq_significance_threshold)
      {
         // Previously assigned segment is significantly closer — keep current classification
      }
      else
      {
         // Distances are similar — use confidence as tiebreaker; if confidence is also equal, prefer the closer segment
         if (segment_drivability_confidence > object.drivable_confidence_sg)
         {
            object.drivable_status_sg = segment_drivability_status;
            object.drivable_confidence_sg = segment_drivability_confidence;
            object.drivable_sg_dist_to_segment_sq = dist_to_segment_sq;
         }
         else if (segment_drivability_confidence == object.drivable_confidence_sg)
         {
            if (dist_to_segment_sq < object.drivable_sg_dist_to_segment_sq)
            {
               object.drivable_status_sg = segment_drivability_status;
               object.drivable_confidence_sg = segment_drivability_confidence;
               object.drivable_sg_dist_to_segment_sq = dist_to_segment_sq;
            }
         }
         else
         {
            // Previously assigned segment has higher confidence — keep current classification
         }
      }
   }
}
