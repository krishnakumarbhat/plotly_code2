#include "sg_merge_contours_helpers.h"

#include "geometry/geo_angle.h"
#include "geometry/geo_length.h"
#include "sg_math.h"

namespace sg
{
   void get_all_merge_distances(float &current_begin_to_partner_begin_dist_sq,
                                float &current_begin_to_partner_end_dist_sq,
                                float &current_end_to_partner_begin_dist_sq,
                                float &current_end_to_partner_end_dist_sq,
                                const Contour_T &current_contour,
                                const Contour_T &partner_contour,
                                const float merge_squeeze_factor,
                                const float curvature_rear)
   {
      geometry::Point2D_T current_contour_beginning = current_contour.vertices.begin()->position;
      geometry::Point2D_T current_contour_end       = std::prev(current_contour.vertices.end())->position;
      geometry::Point2D_T partner_contour_beginning = partner_contour.vertices.begin()->position;
      geometry::Point2D_T partner_contour_end       = std::prev(partner_contour.vertices.end())->position;

      current_contour_beginning.y = calculate_curvi_lat_pos(-curvature_rear, current_contour_beginning);
      current_contour_end.y       = calculate_curvi_lat_pos(-curvature_rear, current_contour_end);
      partner_contour_beginning.y = calculate_curvi_lat_pos(-curvature_rear, partner_contour_beginning);
      partner_contour_end.y       = calculate_curvi_lat_pos(-curvature_rear, partner_contour_end);

      // squeeze longitudinal distances
      auto current_begin_to_partner_begin_vector = current_contour_beginning - partner_contour_beginning;
      auto current_begin_to_partner_end_vector   = current_contour_beginning - partner_contour_end;
      auto current_end_to_partner_begin_vector   = current_contour_end - partner_contour_beginning;
      auto current_end_to_partner_end_vector     = current_contour_end - partner_contour_end;

      current_begin_to_partner_begin_vector.x *= merge_squeeze_factor;
      current_begin_to_partner_end_vector.x *= merge_squeeze_factor;
      current_end_to_partner_begin_vector.x *= merge_squeeze_factor;
      current_end_to_partner_end_vector.x *= merge_squeeze_factor;

      current_begin_to_partner_begin_dist_sq = geometry::length_sq(current_begin_to_partner_begin_vector);
      current_begin_to_partner_end_dist_sq   = geometry::length_sq(current_begin_to_partner_end_vector);
      current_end_to_partner_begin_dist_sq   = geometry::length_sq(current_end_to_partner_begin_vector);
      current_end_to_partner_end_dist_sq     = geometry::length_sq(current_end_to_partner_end_vector);
   }

   bool check_cumulative_angle(const sg::geometry::Segment2D_T &first_contour_last_segment,
                               const sg::geometry::Segment2D_T &second_contour_first_segment,
                               const float max_cumulated_angle_threshold,
                               const float max_merge_distance_to_ignore_cumulated_angle)
   {
      const geometry::Point2D_T connection_vector = {second_contour_first_segment.first - first_contour_last_segment.second};

      float cumulated_angle = 0.0F;
      if (geometry::length(connection_vector) >= max_merge_distance_to_ignore_cumulated_angle)
      {
         const geometry::Point2D_T first_contour_vector = {first_contour_last_segment.second - first_contour_last_segment.first};
         const geometry::Point2D_T second_contour_vector = {second_contour_first_segment.second - second_contour_first_segment.first};
         cumulated_angle += geometry::Angle2D_T(first_contour_vector, connection_vector).deg();
         cumulated_angle += geometry::Angle2D_T(connection_vector, second_contour_vector).deg();
      }

      return (cumulated_angle < max_cumulated_angle_threshold);
   }

   bool check_angles_for_short_contours(const Contour_T &current_contour,
                                        const Contour_T &partner_contour,
                                        const geometry::Segment2D_T &current_contour_segment,
                                        const geometry::Segment2D_T &partner_contour_segment,
                                        const Contour_Postprocessing_Calibrations_T::Merge_Contours_T &calibrations)
   {
      bool f_angle_within_limits = false;

      const bool f_current_contour_single_segment = (current_contour.size() == 2U);
      const bool f_partner_contour_single_segment = (partner_contour.size() == 2U);

      if (f_current_contour_single_segment && f_partner_contour_single_segment)
      {
         const float current_contour_segment_length =
            geometry::euclidean_distance(current_contour_segment.first, current_contour_segment.second);
         const float partner_contour_segment_length =
            geometry::euclidean_distance(partner_contour_segment.first, partner_contour_segment.second);

         if ((current_contour_segment_length < calibrations.merge_max_single_segment_length)
             && (partner_contour_segment_length < calibrations.merge_max_single_segment_length))
         {
            f_angle_within_limits = true;
         }
         (void) partner_contour_segment_length; // MISRA
      }
      else if (f_current_contour_single_segment)
      {
         const float current_contour_segment_length =
            geometry::euclidean_distance(current_contour_segment.first, current_contour_segment.second);

         if (current_contour_segment_length < calibrations.merge_max_single_segment_length)
         {
            const geometry::Point2D_T partner_segment_vector = (partner_contour_segment.first - partner_contour_segment.second);
            const geometry::Point2D_T connection_vector      = (current_contour_segment.second - partner_contour_segment.first);
            const float angle = geometry::Angle2D_T(partner_segment_vector, connection_vector).deg();

            if (std::abs(angle) <= calibrations.merge_max_angle)
            {
               f_angle_within_limits = true;
            }
         }
      }
      else if (f_partner_contour_single_segment)
      {
         const float partner_contour_segment_length =
            geometry::euclidean_distance(partner_contour_segment.first, partner_contour_segment.second);

         if (partner_contour_segment_length < calibrations.merge_max_single_segment_length)
         {
            const geometry::Point2D_T current_segment_vector = (current_contour_segment.first - current_contour_segment.second);
            const geometry::Point2D_T connection_vector      = (current_contour_segment.second - partner_contour_segment.first);
            const float angle = geometry::Angle2D_T(current_segment_vector, connection_vector).deg();

            if (std::abs(angle) <= calibrations.merge_max_angle)
            {
               f_angle_within_limits = true;
            }
         }
      }
      else
      {
         // MISRA
      }

      return f_angle_within_limits;
   }

   bool check_all_merge_angles(const Contour_T &current_contour,
                               const Contour_T &partner_contour,
                               const geometry::Segment2D_T &current_contour_segment,
                               const geometry::Segment2D_T &partner_contour_segment,
                               const Contour_Postprocessing_Calibrations_T::Merge_Contours_T &calibrations)
   {
      bool f_angle_within_limits = check_angles_for_short_contours(current_contour, partner_contour, current_contour_segment,
                                                                   partner_contour_segment, calibrations);

      if (!f_angle_within_limits)
      {
         const auto angle_between_segments_to_be_merged =
            geometry::Angle2D_T(current_contour_segment.first - current_contour_segment.second,
                                partner_contour_segment.first - partner_contour_segment.second);

         if (std::abs(angle_between_segments_to_be_merged.deg()) <= calibrations.merge_max_angle)
         {
            f_angle_within_limits = check_cumulative_angle(current_contour_segment, partner_contour_segment,
                                                           calibrations.merge_max_cumulative_angle,
                                                           calibrations.max_merge_distance_to_ignore_cumulative_angle);
         }
      }
      return f_angle_within_limits;
   }


}
