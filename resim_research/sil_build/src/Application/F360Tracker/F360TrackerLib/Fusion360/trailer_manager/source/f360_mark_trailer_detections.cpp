/*===========================================================================*\
* FILE: f360_mark_trailer_detections.cpp
*============================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains functionality for marking the detections on the trailer.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===========================================================================*/
#include <algorithm>
#include "f360_mark_trailer_detections.h"
#include "f360_math_func.h"

namespace f360_variant_A
{
   static Point Cal_Center(const Point& point_tow_hitch, const BoundingBox& bbox)
   {
      Point new_center{ -0.5F * bbox.Get_Length() , 0.0F };
      new_center.Rotate_About_Origin(bbox.Get_Orientation());
      new_center.Translate(point_tow_hitch.x, point_tow_hitch.y);

      return new_center;
   }

   void Mark_Trailer_Dets(
      const F360_Calibrations_T& calibs,
      const F360_Host_T& f360_host,
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Trailer_Estimator_Output_T& trailer,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Detection_Props_T(&det_Props)[MAX_NUMBER_OF_DETECTIONS])
   {
      if (0U < raw_detections.number_of_valid_detections)
      {
         if ((f360_host.host_type == F360_HOST_TYPE_PASSENGER_VEHICLE) &&
            (TRAILER_PRESENCE_STATE_DETECTED == trailer.trailer_presence[0]) &&
            (0.0F < trailer.trailer_length[0]) &&
            (0.0F < trailer.trailer_width[0]))
         {
            //create bounding boxes for trailer countermeasures 
            constexpr float32_t hitch_offset = 0.4F;
            constexpr float32_t k_trailer_angle_thres = 0.06F;
            constexpr float32_t k_host_yaw_rate_thres = 0.15F;

            BoundingBox trailer_bbox = {};
            BoundingBox trailer_bbox_region_rejection = {};
            float32_t extend_width = 0.0F;
            float32_t extend_length = 0.0F;
            trailer_bbox = Get_Trailer_Bounding_Box(calibs, f360_host, trailer, hitch_offset);
            // special region extension for bike carrier case
            trailer_bbox_region_rejection = Get_Trailer_Bounding_Box(calibs, f360_host, trailer, hitch_offset);
            if ((k_trailer_angle_thres < std::abs(trailer.trailer_angle[0])) || (k_host_yaw_rate_thres < std::abs(f360_host.yaw_rate_rad)))
            {
               constexpr float32_t k_trailer_high_dynamics_width_scale = 0.20F;
               extend_width = trailer_bbox.Get_Width() * k_trailer_high_dynamics_width_scale;
               extend_length = 1.0F;
            }
            else
            {
               constexpr float32_t k_trailer_low_dynamics_width_scale = 0.075F;
               extend_width = trailer_bbox.Get_Width() * k_trailer_low_dynamics_width_scale;
            }

            float32_t extend_width_region_rejection = 0.0F;
            constexpr float32_t min_width_thres_for_region_rej = 2.8F;
            constexpr float32_t front_length_ext_for_region_rej = 0.7F;
            if (trailer_bbox.Get_Width() < min_width_thres_for_region_rej)
            {
               extend_width_region_rejection = (min_width_thres_for_region_rej - trailer_bbox.Get_Width()) * 0.5F;
            }
            else
            {
               extend_width_region_rejection = extend_width;
            }
            trailer_bbox.Extend_Boundaries(extend_width, extend_width, extend_length, extend_length);
            trailer_bbox_region_rejection.Extend_Boundaries(extend_width_region_rejection, extend_width_region_rejection, extend_length, front_length_ext_for_region_rej);

            //Trailer multireflections
            Stationary_Dets_Zones_Count_T stat_dets = {};
            float32_t rho_i = 0.0F;
            float32_t alpha_i = 0.0F;
            uint8_t d1_det_num = 0U;

            uint32_t det_idx = static_cast<uint32_t>(raw_detections.vcslong_det_idx_min);

            const float32_t dist_vcs_to_tow_hitch = f360_host.dist_rear_axle_to_vcs_m + calibs.k_trailer_distance_rear_axle_to_tow_hitch;

            Trailer_BBox_Fov_Limits trailer_fov_left_sensor = {};
            Trailer_BBox_Fov_Limits trailer_fov_right_sensor = {};
            for (int32_t snsr_idx = 0; snsr_idx < static_cast<int32_t>(MAX_NUMBER_OF_SENSORS); snsr_idx++)
            {
               if (sensors[snsr_idx].constant.mounting_location == F360_MOUNTING_LOCATION_LEFT_REAR)
               {
                  Get_Trailer_Corner_Fov_Limits_For_Sensor(sensors, snsr_idx, trailer_bbox, trailer_fov_left_sensor);
               }
               else if (sensors[snsr_idx].constant.mounting_location == F360_MOUNTING_LOCATION_RIGHT_REAR)
               {
                  Get_Trailer_Corner_Fov_Limits_For_Sensor(sensors, snsr_idx, trailer_bbox, trailer_fov_right_sensor);
               }
               else
               {
                  //MISRA. Do Nothing
               }
            }

            const float32_t pos_x_threshold_for_filtering_dets = -(dist_vcs_to_tow_hitch - calibs.k_trailer_bbox_extend_length);

            for (uint32_t i = 0U; i < raw_detections.number_of_valid_detections; i++)
            {
               if (i > 0U)
               {
                  det_idx = static_cast<uint32_t>(raw_detections.detections[det_idx].processed.next_sorted_idx);
               }
               const rspp_variant_A::RSPP_Detection_T& raw_det = raw_detections.detections[det_idx];
               const int32_t sensor_idx = raw_detections.detections[det_idx].raw.sensor_id - 1;
               const F360_Radar_Sensor_T& sensor = sensors[sensor_idx];
               F360_Detection_Props_T& det_prop = det_Props[det_idx];

               const float32_t det_scs_az = raw_det.raw.azimuth * static_cast<float32_t>(sensor.constant.polarity);

               if (pos_x_threshold_for_filtering_dets < det_prop.vcs_position.x)
               {
                  break;
               }

               const bool f_det_in_trailer_bbox = trailer_bbox_region_rejection.Contains(det_prop.vcs_position);
               const bool f_det_occluded_left_rear = (sensor.constant.mounting_location == F360_MOUNTING_LOCATION_LEFT_REAR) && (det_scs_az > trailer_fov_left_sensor.leftmost_angle) && (det_scs_az < trailer_fov_left_sensor.rightmost_angle);
               const bool f_det_occluded_right_rear = (sensor.constant.mounting_location == F360_MOUNTING_LOCATION_RIGHT_REAR) && (det_scs_az > trailer_fov_right_sensor.leftmost_angle) && (det_scs_az < trailer_fov_right_sensor.rightmost_angle);

               constexpr float32_t k_margin_for_x_pos = 6.5F;
               const float32_t k_max_trailer_length = 12.0F;
               const float32_t k_min_x_pos_for_trailer_related_dets = -1.0F * k_max_trailer_length - k_margin_for_x_pos;
               constexpr float32_t k_max_y_pos_for_trailer_related_dets = 7.5F;
               constexpr float32_t elevation_threshold_for_filtering_road_dets = 0.11F;

               const bool road_to_trailer_reflection = (std::abs(raw_det.processed.vcs_el) > elevation_threshold_for_filtering_road_dets)
                  && (det_prop.vcs_position.x > k_min_x_pos_for_trailer_related_dets)
                  && (std::abs(det_prop.vcs_position.y) < k_max_y_pos_for_trailer_related_dets)
                  && ((sensor.constant.mounting_location == F360_MOUNTING_LOCATION_RIGHT_REAR) || (sensor.constant.mounting_location == F360_MOUNTING_LOCATION_LEFT_REAR));

               if (f_det_in_trailer_bbox || f_det_occluded_left_rear || f_det_occluded_right_rear || road_to_trailer_reflection)
               {
                  det_prop.f_ok_to_use = false;
                  det_prop.f_trailer_related_det = true;
               }

               Calc_Stationary_Dets_In_Zones(raw_det, det_prop, sensors, stat_dets, d1_det_num, rho_i, alpha_i);
            }
            Flag_Reflection_Dets(raw_detections, sensors, f360_host, d1_det_num, rho_i, alpha_i, stat_dets, det_Props);
         }
         else if (f360_host.host_type == F360_HOST_TYPE_COMMERCIAL_VEHICLE)
         {
            const bool f_cv_1st_trailer_parameter_valid = ((trailer.trailer_presence[0] == TRAILER_PRESENCE_STATE_DETECTED) &&
                                                           (0.0F < trailer.trailer_length[0]) &&
                                                           (0.0F < trailer.trailer_width[0]));  // Should always be true, as long as the cv trailer is initialized and parsed properly
            if (f_cv_1st_trailer_parameter_valid)
            {
               // If the reversing countermeasures are active, drop low range-rate detections within rear semi-circle area of the trailer
               if (trailer.f_reversing_countermeasures_active)
               {
                  // When the host had previously reversed, drop the low raw range-rate detections within rear semi-circle area
                  const float32_t total_trailer_length = (trailer.trailer_length[0]  + trailer.trailer_length[1]) + 3.0F;
                  const float32_t det_dist_square_thres_not_to_use = total_trailer_length * total_trailer_length;
                  int16_t det_idx = raw_detections.vcslong_det_idx_min;
                  for (uint32_t j = 0U; j < raw_detections.number_of_valid_detections; j++)
                  {
                     if (j > 0U)
                     {
                        det_idx = raw_detections.detections[det_idx].processed.next_sorted_idx;
                     }
                     if (det_idx >= 0)
                     {
                        F360_Detection_Props_T& det_prop = det_Props[det_idx];
                        const float32_t det_vcs_x_dist_to_joint1 = (det_prop.vcs_position.x - trailer.joint_position_vcs_long[0]);
                        const float32_t det_dist_to_joint1_square =(det_vcs_x_dist_to_joint1) * (det_vcs_x_dist_to_joint1) + det_prop.vcs_position.y * det_prop.vcs_position.y;
                        const bool f_det_close_to_trailer = (det_dist_to_joint1_square < det_dist_square_thres_not_to_use) && (det_prop.vcs_position.x < (trailer.joint_position_vcs_long[0] + 2.0F));
                        if (f_det_close_to_trailer && (std::abs(raw_detections.detections[det_idx].raw.range_rate) < 1.0F))
                        {
                           // the detection can still be ok to use - for object association, but not object creation.
                           det_prop.f_trailer_related_det = true;
                        }
                     }
                  }
               }
               else
               {
                  for (uint32_t i = 0U; i < 2U; i++)
                  {
                     if (TRAILER_PRESENCE_STATE_NOT_DETECTED == trailer.trailer_presence[i])
                     {
                        break;
                     }
                     const float32_t cos_angle = F360_Cosf(trailer.trailer_angle[i]);
                     const float32_t sin_angle = F360_Sinf(trailer.trailer_angle[i]);
                     const float32_t front_margin = 2.0F;
                     const float32_t rear_margin = 20.0F;
                     const float32_t width_margin_by_angle = fmaxf(0.35F, fminf(1.5F, 0.35F + (fabsf(trailer.trailer_angle[i]) - 0.09F) * 3.3F)); // linear gate between trailer angles (5 deg : 25 deg)
                     const float32_t width_margin_yawrate = fmaxf(0.0F, fminf(1.5F, (fabsf(f360_host.curvature_rear) - 0.01F) * 150.0F)); // linear gate between curvatures (0.01 : 0.02)
                     const float32_t width_margin = fmaxf(width_margin_by_angle, width_margin_yawrate);

                     const float32_t trailer_para = cos_angle * trailer.joint_position_vcs_long[i] + sin_angle * trailer.joint_position_vcs_lat[i];
                     const float32_t trailer_orth = -sin_angle * trailer.joint_position_vcs_long[i] + cos_angle * trailer.joint_position_vcs_lat[i];

                     const float32_t trailer_front_without_margin = trailer_para - trailer.joint2center[i] + trailer.trailer_length[i] * 0.5F;
                     const float32_t trailer_front = trailer_front_without_margin + front_margin;
                     const float32_t trailer_rear_without_margin = trailer_para - trailer.joint2center[i] - trailer.trailer_length[i] * 0.5F;
                     const float32_t trailer_rear = trailer_rear_without_margin - rear_margin;
                     const float32_t trailer_left_without_margin = trailer_orth - trailer.trailer_width[i] * 0.5F;
                     const float32_t trailer_right_without_margin = trailer_orth + trailer.trailer_width[i] * 0.5F;
                     const float32_t trailer_left = trailer_left_without_margin - width_margin;
                     const float32_t trailer_right = trailer_right_without_margin + width_margin;

                     // Parameters of a line "line", in format line[0] x + line[1] y + line[2]c = 0.
                     // The convention is the same as the "Line" Class
                     Trailer_line left_line_of_sight;
                     Trailer_line right_line_of_sight;
                     Trailer_line trailer_edge;

                     Find_Occlusion_Area_CV_Trailer_All_Sensors(sensors, trailer, i, left_line_of_sight, right_line_of_sight, trailer_edge);

                     int16_t det_idx = raw_detections.vcslong_det_idx_min;
                     for (uint32_t j = 0U; j < raw_detections.number_of_valid_detections; j++)
                     {
                        if (j > 0U)
                        {
                           det_idx = raw_detections.detections[det_idx].processed.next_sorted_idx;
                        }

                        if (det_idx >= 0)
                        {
                           F360_Detection_Props_T& det_prop = det_Props[det_idx];
                           const float32_t det_para = cos_angle * det_prop.vcs_position.x + sin_angle * det_prop.vcs_position.y;
                           const float32_t det_orth = -sin_angle * det_prop.vcs_position.x + cos_angle * det_prop.vcs_position.y;

                           const bool f_within_trailer_width = (det_orth < trailer_right) && (det_orth > trailer_left);  // within trailer laterally
                           const bool f_inside_trailer = (det_para < trailer_front) && (det_para > trailer_rear) && (f_within_trailer_width);  // also within trailer longitudinally

                           const int32_t sensor_idx = raw_detections.detections[det_idx].raw.sensor_id - 1;
                           const F360_Radar_Sensor_T& sensor = sensors[sensor_idx];

                           const bool f_left_radar_trailer_det_expected = (((sensor.constant.mounting_location == F360_MOUNTING_LOCATION_LEFT_REAR) ||
                              (sensor.constant.mounting_location == F360_MOUNTING_LOCATION_LEFT_FORWARD)) &&
                              (trailer.trailer_angle[i] > F360_DEG2RAD(-3.0F)));
                           const bool f_right_radar_trailer_det_expected = (((sensor.constant.mounting_location == F360_MOUNTING_LOCATION_RIGHT_REAR) ||
                              (sensor.constant.mounting_location == F360_MOUNTING_LOCATION_RIGHT_FORWARD)) &&
                              (trailer.trailer_angle[i] < F360_DEG2RAD(3.0F)));

                           const float32_t current_det_point[2] = { det_prop.vcs_position.x, det_prop.vcs_position.y };

                           bool f_occluded_by_trailer= false;  // occluded both by the extended trailer edge, and by the line connecting the sensor and the trailer rear corner
                           bool f_trailer_clutter_angle_match= false;  // clutter area within a small angle of the trailer edge
                           bool f_in_double_bounce_sector= false;  // a sector starting from the line slightly off the trailer edges, towards the blind zone behind the trailer
                           const bool f_first_trailer = (i == 0U);
                           if (f_left_radar_trailer_det_expected)
                           {
                              f_occluded_by_trailer = ((!Check_Detection_To_Left_of_Line(current_det_point, left_line_of_sight)) && (!Check_Detection_To_Left_of_Line(current_det_point, trailer_edge)));
                              const float32_t angle_w_r_t_trailer_edge = F360_Atan2f(trailer_left_without_margin - det_orth, trailer_front_without_margin - det_para);  // Positive means angle to the left of the trailer edge
                              const bool f_trailer_slightly_at_left = trailer.trailer_angle[i] < F360_DEG2RAD(10.0F);
                              f_trailer_clutter_angle_match = f_trailer_slightly_at_left && (angle_w_r_t_trailer_edge < F360_DEG2RAD(15.0F));
                              f_in_double_bounce_sector = f_first_trailer && (angle_w_r_t_trailer_edge < F360_DEG2RAD(4.0F));
                           }
                           else if (f_right_radar_trailer_det_expected)
                           {
                              f_occluded_by_trailer = ((Check_Detection_To_Left_of_Line(current_det_point, right_line_of_sight)) && (Check_Detection_To_Left_of_Line(current_det_point, trailer_edge)));
                              const float32_t angle_w_r_t_trailer_edge = F360_Atan2f(trailer_right_without_margin - det_orth, trailer_front_without_margin - det_para);  // Negative means angle to the right of the trailer edge
                              const bool f_trailer_slightly_at_right = trailer.trailer_angle[i] > F360_DEG2RAD(-10.0F);
                              f_trailer_clutter_angle_match = f_trailer_slightly_at_right  && (angle_w_r_t_trailer_edge > F360_DEG2RAD(-15.0F));
                              f_in_double_bounce_sector = f_first_trailer && (angle_w_r_t_trailer_edge > F360_DEG2RAD(-4.0F));
                           }
                           else
                           {
                              // MISRA. Do nothing
                           }

                           const float32_t host_speed_based_margin_factor = fminf(1.0F, fabsf(f360_host.vcs_speed) * 0.5F); // max 1.0 at 2m/s and above
                           const float32_t range_rate_thres_inside_trailer = fmaxf(0.4F, fminf(4.0F, host_speed_based_margin_factor * 4.0F)); // dynamic threshold between 0.4 m/s to 4.0 m/s
                           const bool f_inside_trailer_confirmed = (f_inside_trailer && (std::abs(raw_detections.detections[det_idx].raw.range_rate) < range_rate_thres_inside_trailer));

                           // the clutter detection should be within a small angle of the trailer edge, have low range rate, and be within a certain range
                           const float32_t range_rate_gate_clutter = 0.4F;
                           const float32_t clutter_range_max = 30.0F;
                           const bool f_trailer_clutter = f_trailer_clutter_angle_match &&
                                                         (std::abs(raw_detections.detections[det_idx].raw.range_rate) < range_rate_gate_clutter) &&
                                                         (raw_detections.detections[det_idx].raw.range < clutter_range_max);

                           // the double bounce detection should be within a certain angle, have low range rate, and be within a certain range
                           const float32_t range_rate_gate_double_bounces = 0.4F;
                           const float32_t double_bounce_test_area_length = 50.0F;  // the radius should be this length plus the trailer length
                           const float32_t double_bounce_range_thres = -trailer_rear_without_margin + double_bounce_test_area_length;  // limit the radius of the double bounce sector in vcs
                           const bool f_trailer_double_bounce = (f_in_double_bounce_sector && 
                                                                 (std::abs(raw_detections.detections[det_idx].raw.range_rate) < range_rate_gate_double_bounces) &&
                                                                 (raw_detections.detections[det_idx].raw.range < double_bounce_range_thres));

                           if (f_inside_trailer_confirmed || f_occluded_by_trailer)
                           {
                              // If the detection is inside the trailer bounding box, or occluded by the trailer, it is not ok to use for any objects
                              det_prop.f_ok_to_use = false;
                              det_prop.f_trailer_related_det = true;
                           }
                           else if (f_trailer_double_bounce || f_trailer_clutter)
                           {
                              // If the detection is likely a trailer clutter or double bounce, it shouldn't be used for object creation, but association might still be ok.
                              det_prop.f_trailer_related_det = true;
                           }
                           else
                           {
                              // MISRA. Do nothing
                           }
                        }
                     }
                  }
               }
            }
         }
         else
         {
            // do nothing
         }
      }
   }


   /*===========================================================================*\
   * FUNCTION: Find_Occlusion_Area_CV_Trailer_All_Sensors()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
   * const F360_Trailer_Estimator_Output_T& trailer,
   * const uint32_t trailer_idx,
   * float32_t (&left_line_of_sight)[3],
   * float32_t (&right_line_of_sight)[3],
   * float32_t (&trailer_edge)[3]
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function gets leftmost and rightmost field of view thresholds for
   * given trailer state.
   \*===========================================================================*/
   void Find_Occlusion_Area_CV_Trailer_All_Sensors(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Trailer_Estimator_Output_T& trailer,
      const uint32_t trailer_idx,
      Trailer_line(&left_line_of_sight),
      Trailer_line(&right_line_of_sight),
      Trailer_line(&trailer_edge))
   {
      const float32_t cos_angle = F360_Cosf(trailer.trailer_angle[trailer_idx]);
      const float32_t sin_angle = F360_Sinf(trailer.trailer_angle[trailer_idx]);

      // unit vector to project x, y coordinates of trailer local frame to vcs_x
      const float32_t rotate_trailer_cs_to_vcs_x[2]{ cos_angle, -sin_angle };
      // unit vector to project x, y coordinates of trailer local frame to vcs_y
      const float32_t rotate_trailer_cs_to_vcs_y[2]{ sin_angle, cos_angle };

      const float32_t trailer_i_length = trailer.trailer_length[trailer_idx];
      const float32_t trailer_i_width = trailer.trailer_width[trailer_idx];

      // assume the joint position is in the lateral middle of the trailer width
      // The trailer width is in cv_y direction,
      const float32_t left_front_corner_vcs_xy[2]{ trailer.joint_position_vcs_long[trailer_idx] - 0.5F * trailer_i_width * rotate_trailer_cs_to_vcs_x[1],
         trailer.joint_position_vcs_lat[trailer_idx] - 0.5F * trailer_i_width * rotate_trailer_cs_to_vcs_y[1] };
      const float32_t right_front_corner_vcs_xy[2]{ trailer.joint_position_vcs_long[trailer_idx] + 0.5F * trailer_i_width * rotate_trailer_cs_to_vcs_x[1],
         trailer.joint_position_vcs_lat[trailer_idx] + 0.5F * trailer_i_width * rotate_trailer_cs_to_vcs_y[1] };
      const float32_t left_rear_corner_vcs_xy[2]{ trailer.joint_position_vcs_long[trailer_idx] - trailer_i_length * rotate_trailer_cs_to_vcs_x[0] - 0.5F * trailer_i_width * rotate_trailer_cs_to_vcs_x[1],
         trailer.joint_position_vcs_lat[trailer_idx] - trailer_i_length * rotate_trailer_cs_to_vcs_y[0] - 0.5F * trailer_i_width * rotate_trailer_cs_to_vcs_y[1] };
      const float32_t right_rear_corner_vcs_xy[2]{ trailer.joint_position_vcs_long[trailer_idx] - trailer_i_length * rotate_trailer_cs_to_vcs_x[0] + 0.5F * trailer_i_width * rotate_trailer_cs_to_vcs_x[1],
         trailer.joint_position_vcs_lat[trailer_idx] - trailer_i_length * rotate_trailer_cs_to_vcs_y[0] + 0.5F * trailer_i_width * rotate_trailer_cs_to_vcs_y[1] };


      const float32_t trailer_angle = trailer.trailer_angle[trailer_idx];
      if (trailer_angle > 0.0F) {
         // Trailer to left
         Compute_Line_Parameters(left_rear_corner_vcs_xy, left_front_corner_vcs_xy, trailer_edge);
      }
      else {
         // Trailer to right
         Compute_Line_Parameters(right_rear_corner_vcs_xy, right_front_corner_vcs_xy, trailer_edge);
      }

      for (int32_t snsr_idx = 0; snsr_idx < static_cast<int32_t>(MAX_NUMBER_OF_SENSORS); snsr_idx++) {
         if (sensors[snsr_idx].variable.is_valid) {
            if (sensors[snsr_idx].constant.mounting_location == F360_MOUNTING_LOCATION_LEFT_REAR) {
               const float32_t left_rear_radar_position_vcs_xy[2]{ sensors[snsr_idx].constant.mounting_position.vcs_position.longitudinal,
                  sensors[snsr_idx].constant.mounting_position.vcs_position.lateral };
               if (trailer_angle > 0.0F) {
                  // Trailer on the left side
                  Trailer_line los_sensor_to_left_front;
                  Trailer_line los_sensor_to_left_rear;
                  Compute_Line_Parameters(left_front_corner_vcs_xy, left_rear_radar_position_vcs_xy, los_sensor_to_left_front);
                  Compute_Line_Parameters(left_rear_corner_vcs_xy, left_rear_radar_position_vcs_xy, los_sensor_to_left_rear);
                  const float32_t slope_left_front = -los_sensor_to_left_front.a / los_sensor_to_left_front.b;
                  const float32_t slope_left_rear = -los_sensor_to_left_rear.a / los_sensor_to_left_rear.b;

                  // If left LoS from rear corner is further away from x axis, the slope should be larger
                  left_line_of_sight = (slope_left_rear > slope_left_front) ? los_sensor_to_left_rear : los_sensor_to_left_front;
               }
            }
            else if (sensors[snsr_idx].constant.mounting_location == F360_MOUNTING_LOCATION_RIGHT_REAR)
            {
               const float32_t right_rear_radar_position_vcs_xy[2]{ sensors[snsr_idx].constant.mounting_position.vcs_position.longitudinal,
                  sensors[snsr_idx].constant.mounting_position.vcs_position.lateral };
               if (trailer_angle < 0.0F) {
                  // Trailer is on the right side
                  Trailer_line los_sensor_to_right_front;
                  Trailer_line los_sensor_to_right_rear;
                  Compute_Line_Parameters(right_front_corner_vcs_xy, right_rear_radar_position_vcs_xy, los_sensor_to_right_front);
                  Compute_Line_Parameters(right_rear_corner_vcs_xy, right_rear_radar_position_vcs_xy, los_sensor_to_right_rear);
                  const float32_t slope_right_front = -los_sensor_to_right_front.a / los_sensor_to_right_front.b;
                  const float32_t slope_right_rear = -los_sensor_to_right_rear.a / los_sensor_to_right_rear.b;
                  right_line_of_sight = (slope_right_rear < slope_right_front) ? los_sensor_to_right_rear : los_sensor_to_right_front;
               }
            }
            else
            {
               //MISRA. Do Nothing
            }

         }
      }
   }



   /*===========================================================================*\
   * FUNCTION: Compute_Line_Parameters()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const float32_t(&rear_point)[2]  Coordinate of rear point
   * const float32_t(&front_point)[2]  Coordinate of front point
   * const float32_t(&line_param)[3]  Line by connecting rear and front points, in format l[0] x + l[1] y + l[2] = 0
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function computes a line connecting two points, from rear to front.
   * Then for point (x1, y1), if l[0] x1 + l[1] y1 + l[2] > 0, the point is to
   * the left of the line vector connecting from rear to front points, vice versa.
   \*===========================================================================*/
   void Compute_Line_Parameters(
      const float32_t(&rear_point)[2],
      const float32_t(&front_point)[2],
      Trailer_line(&line_param))
   {
      line_param.a = front_point[1] - rear_point[1];
      line_param.b = rear_point[0] - front_point[0];
      line_param.c = (front_point[0] * rear_point[1]) - (rear_point[0] * front_point[1]);  // cross-product magnitude
   }


   /*===========================================================================*\
   * FUNCTION: Check_Detection_To_Left_of_Line()
   *===========================================================================
   * RETURN VALUE:
   * bool f_detection_in_left  True when detection is to the left of the line
   *  connecting from rear to front
   *
   * PARAMETERS:
   * const float32_t(&detection)[2]  Coordinate of detection
   * const float32_t(&line_param)[3]  Line to evaluate
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function evaluates the location of a detection to a line.
   * For point (x1, y1), if l[0] x1 + l[1] y1 + l[2] > 0, the point is to
   * the left of the line vector in vcs, vice versa.
   * The distance can also be computed if l is normalized, i.e.,  (l[0]^2 + l[1]^2 + l[2]^2) = 1 .
   \*===========================================================================*/
   bool Check_Detection_To_Left_of_Line(
      const float32_t(&detection)[2],
      const Trailer_line(&line_param))
   {
      const bool f_detection_in_left = (line_param.a * detection[0] + line_param.b * detection[1] + line_param.c) > 0.0F;
      return f_detection_in_left;
   }

   /*===========================================================================*\
   * FUNCTION: Get_Trailer_Corner_Fov_Limits_For_Sensor()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
   * const int32_t snsr_idx
   * Trailer_BBox_Fov_Limits& trailer_corner_fov_limits
   * BoundingBox& trailer_bbox
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Function gets leftmost and rightmost field of view thresholds for
   * given trailer state.
   \*===========================================================================*/
   void Get_Trailer_Corner_Fov_Limits_For_Sensor(
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const int32_t snsr_idx,
      const BoundingBox& trailer_bbox,
      Trailer_BBox_Fov_Limits& trailer_corner_fov_limits)
   {
      BoundingBox scs_trailer_bbox = trailer_bbox;
      const Point snsr_mnt_pos = Point(sensors[snsr_idx].constant.mounting_position.vcs_position.longitudinal, sensors[snsr_idx].constant.mounting_position.vcs_position.lateral);
      scs_trailer_bbox.Transform_To_Relative_Coordinate_System(snsr_mnt_pos, Angle(sensors[snsr_idx].constant.mounting_position.vcs_boresight_azimuth_angle));
      const BboxCorners scs_trailer_corners = scs_trailer_bbox.Get_Corners();
      float32_t scs_trailer_corner_azs[4] = {};
      for (uint8_t i = 0U; i <= 3U; i++)
      {
         scs_trailer_corner_azs[i] = F360_Atan2f(scs_trailer_corners.points[i].y, scs_trailer_corners.points[i].x);

      }

      trailer_corner_fov_limits.leftmost_angle = F360_Min_Element(scs_trailer_corner_azs, 4U);
      trailer_corner_fov_limits.rightmost_angle = F360_Max_Element(scs_trailer_corner_azs, 4U);
   }

   /*===========================================================================*\
   * FUNCTION: Get_Trailer_Boundary_Box()
   *===========================================================================
   * RETURN VALUE:
   * trailer_bbox
   *
   * PARAMETERS:
   * const F360_Calibrations_T calibs,
   * const F360_Host_T &f360_host,
   * const Trailer_Detector_Flt_Fus_Output& trailer)
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function creates a boundary box object for given trailer state.
   \*===========================================================================*/
   BoundingBox Get_Trailer_Bounding_Box(
      const F360_Calibrations_T& calibs,
      const F360_Host_T& f360_host,
      const F360_Trailer_Estimator_Output_T& trailer,
      const float32_t hitch_offset)
   {
      BoundingBox trailer_bbox = {};
      const Point point_tow_hitch = { -(f360_host.dist_rear_axle_to_vcs_m + calibs.k_trailer_distance_rear_axle_to_tow_hitch + hitch_offset), 0.0F };
      trailer_bbox.Set_Length(trailer.trailer_length[0]);
      trailer_bbox.Set_Width(trailer.trailer_width[0]);
      trailer_bbox.Set_Orientation(-trailer.trailer_angle[0]);  //Add minus here as the trailer angle is defined opposite to the VCS
      trailer_bbox.Set_Center(Cal_Center(point_tow_hitch, trailer_bbox));
      return trailer_bbox;
   }

   /*===========================================================================*\
   * FUNCTION: Calc_Stationary_Dets_In_Zones()
   *===========================================================================
   * RETURN VALUE:
   * none
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_T& raw_det,
   * const F360_Detection_Props_T& det_prop,
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
   * Stationary_Dets_Zones_Count_T &stat_dets
   * uint8_t &d1_det_num
   * float32_t &rho_i
   * float32_t &alpha_i
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function calculates stationaray detections in ROI behind the host.
   * ROI is partitioned into different segments, each having its own counter.
   \*===========================================================================*/
   void Calc_Stationary_Dets_In_Zones(
      const rspp_variant_A::RSPP_Detection_T& raw_det,
      const F360_Detection_Props_T& det_prop,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      Stationary_Dets_Zones_Count_T& stat_dets,
      uint8_t& d1_det_num,
      float32_t& rho_i,
      float32_t& alpha_i)
   {
      constexpr float32_t k_ambiguous_stationary_threshold = 0.5F;   //Vel. threshold to check if ambiguous objs are actually stationary

      constexpr float32_t k_ws_abs_lat_posn_lwr_bound = 3.0F;        //Defining region for stationary detection count
      constexpr float32_t k_ws_abs_lat_posn_upr_bound = 70.0F;
      constexpr float32_t k_ws_long_posn_upr_bound = 1.0F;
      constexpr float32_t k_ws_long_posn_lwr_bound = -60.0F;

      constexpr float32_t k_d1_range_lwr_bound = 0.8F;               //Defining region where trailer edge detection (D1) is expected to be present
      constexpr float32_t k_d1_range_upr_bound = 5.0F;
      constexpr float32_t k_d1_abs_azimuth_lwr_bound = 0.75F;
      constexpr float32_t k_d1_abs_azimuth_upr_bound = 1.3F;

      const int32_t sensor_idx = raw_det.raw.sensor_id - 1;
      const float32_t raw_det_az = raw_det.raw.azimuth * static_cast<float32_t>(sensors[sensor_idx].constant.polarity);

      const bool is_det_stationary = (det_prop.motion_status == rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_STATIONARY) ||
         ((det_prop.motion_status == rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_AMBIGUOUS) &&
            (det_prop.range_rate_compensated < k_ambiguous_stationary_threshold));

      const bool is_det_within_roi = (std::abs(det_prop.vcs_position.y) < k_ws_abs_lat_posn_upr_bound) &&
         (std::abs(det_prop.vcs_position.y) > k_ws_abs_lat_posn_lwr_bound) &&
         (det_prop.vcs_position.x > k_ws_long_posn_lwr_bound) &&
         (det_prop.vcs_position.x < k_ws_long_posn_upr_bound);

      if (is_det_stationary && is_det_within_roi)
      {
         stat_dets.stationary_det_num++;

         //3-10m case
         if (std::abs(det_prop.vcs_position.y) < 10.0F)
         {
            stat_dets.stationary_det_num_3_10++;
            stat_dets.stationary_lat_dist_between_3_10 += std::abs(det_prop.vcs_position.y);
         }

         //10-15m case
         else if ((std::abs(det_prop.vcs_position.y) > 10.0F) && (std::abs(det_prop.vcs_position.y) < 15.0F))
         {
            stat_dets.stationary_det_num_10_15++;
            stat_dets.stationary_lat_dist_between_10_15 += std::abs(det_prop.vcs_position.y);
         }

         //15-20m case
         else if ((std::abs(det_prop.vcs_position.y) > 15.0F) && (std::abs(det_prop.vcs_position.y) < 20.0F))
         {
            stat_dets.stationary_det_num_15_20++;
            stat_dets.stationary_lat_dist_between_15_20 += std::abs(det_prop.vcs_position.y);
         }

         //20-25m case
         else if ((std::abs(det_prop.vcs_position.y) > 20.0F) && (std::abs(det_prop.vcs_position.y) < 25.0F))
         {
            stat_dets.stationary_det_num_20_25++;
            stat_dets.stationary_lat_dist_between_20_25 += std::abs(det_prop.vcs_position.y);
         }

         //25-30m case
         else if ((std::abs(det_prop.vcs_position.y) > 25.0F) && (std::abs(det_prop.vcs_position.y) < 30.0F))
         {
            stat_dets.stationary_det_num_25_30++;
            stat_dets.stationary_lat_dist_between_25_30 += std::abs(det_prop.vcs_position.y);
         }

         //30-50m case
         else if ((std::abs(det_prop.vcs_position.y) > 30.0F) && (std::abs(det_prop.vcs_position.y) < 50.0F))
         {
            stat_dets.stationary_det_num_30_50++;
            stat_dets.stationary_lat_dist_between_30_50 += std::abs(det_prop.vcs_position.y);
         }

         //50-70m case
         else if ((std::abs(det_prop.vcs_position.y) > 50.0F) && (std::abs(det_prop.vcs_position.y) < 70.0F))
         {
            stat_dets.stationary_det_num_50_70++;
            stat_dets.stationary_lat_dist_between_50_70 += std::abs(det_prop.vcs_position.y);
         }

         else
         {
            // Misra. Do nothing
         }
      }

      const bool is_det_in_d1_region = (raw_det.raw.range < k_d1_range_upr_bound) &&
         (raw_det.raw.range > k_d1_range_lwr_bound) &&
         (std::abs(raw_det_az) > k_d1_abs_azimuth_lwr_bound) &&
         (std::abs(raw_det_az) < k_d1_abs_azimuth_upr_bound);

      if (is_det_in_d1_region)
      {
         d1_det_num++;
         rho_i += raw_det.raw.range;
         alpha_i += raw_det_az;
      }
   }

   /*===========================================================================*\
   * FUNCTION: Flag_Reflection_Dets()
   *===========================================================================
   * RETURN VALUE:
   * none
   *
   * PARAMETERS:
   * const rspp_variant_A::RSPP_Detection_List_T& raw_detections
   * const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS]
   * const F360_Host_T& f360_host
   * const uint8_t& d1_det_num
   * const float32_t& rho_i
   * const float32_t& alpha_i
   * F360_Detection_Props_T(&det_Props)[MAX_NUMBER_OF_DETECTIONS]
   * Stationary_Dets_Zones_Count_T& stat_dets
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function flags dets which reflected between the trailer and stationary
   * environment as not ok to use.
   *
   \*===========================================================================*/
   void Flag_Reflection_Dets(
      const rspp_variant_A::RSPP_Detection_List_T& raw_detections,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      const F360_Host_T& f360_host,
      const uint8_t& d1_det_num,
      const float32_t& rho_i,
      const float32_t& alpha_i,
      const Stationary_Dets_Zones_Count_T& stat_dets,
      F360_Detection_Props_T(&det_Props)[MAX_NUMBER_OF_DETECTIONS])
   {
      constexpr uint8_t ns = 1U;                                  //Stationary objects num threshold for window Ws
      if ((stat_dets.stationary_det_num < ns) || (d1_det_num <= 0U))
      {
         //Do nothing
      }
      else
      {

         constexpr float32_t k_azimuth_threshold = 0.5F;             //D2-D1 threshold
         constexpr float32_t k_d2_abs_lat_upr_bound = 10.0F;         //D2 det lateral window upper bound                                 
         constexpr float32_t k_SR_upr_bound = 1.2F;                  //Defining Speed Ratio (SR) bounds
         constexpr float32_t k_SR_lwr_bound = 0.4F;
         constexpr float32_t k_min_theta_refln_triangle = 0.0F;      //Defining bounds for theta from reflection formula
         constexpr float32_t k_max_theta_refln_triangle = 1.55F;
         float32_t stationary_lat_dist = 0.0F;                              //Mean stationary objects lateral distance

         if (stat_dets.stationary_det_num_3_10 > 0U)
         {
            stationary_lat_dist = stat_dets.stationary_lat_dist_between_3_10 /
               static_cast<float32_t>(stat_dets.stationary_det_num_3_10);
         }
         else if (stat_dets.stationary_det_num_10_15 > 0U)
         {
            stationary_lat_dist = stat_dets.stationary_lat_dist_between_10_15 /
               static_cast<float32_t>(stat_dets.stationary_det_num_10_15);
         }
         else if (stat_dets.stationary_det_num_15_20 > 0U)
         {
            stationary_lat_dist = stat_dets.stationary_lat_dist_between_15_20 /
               static_cast<float32_t>(stat_dets.stationary_det_num_15_20);
         }
         else if (stat_dets.stationary_det_num_20_25 > 0U)
         {
            stationary_lat_dist = stat_dets.stationary_lat_dist_between_20_25 /
               static_cast<float32_t>(stat_dets.stationary_det_num_20_25);
         }
         else if (stat_dets.stationary_det_num_25_30 > 0U)
         {
            stationary_lat_dist = stat_dets.stationary_lat_dist_between_25_30 /
               static_cast<float32_t>(stat_dets.stationary_det_num_25_30);
         }
         else if (stat_dets.stationary_det_num_30_50 > 0U)
         {
            stationary_lat_dist = stat_dets.stationary_lat_dist_between_30_50 /
               static_cast<float32_t>(stat_dets.stationary_det_num_30_50);
         }
         else if (stat_dets.stationary_det_num_50_70 > 0U)
         {
            stationary_lat_dist = stat_dets.stationary_lat_dist_between_50_70 /
               static_cast<float32_t>(stat_dets.stationary_det_num_50_70);
         }
         else
         {
            //MISRA. Do Nothing
         }

         const float32_t d1_range = rho_i / static_cast<float32_t>(d1_det_num);                 //Range of D1 detection
         const float32_t d1_azimuth = alpha_i / static_cast<float32_t>(d1_det_num);             //Azimuth of D1 detection
         uint32_t det_idx = static_cast<uint32_t>(raw_detections.vcslong_det_idx_min);

         for (uint32_t i = 0U; i < raw_detections.number_of_valid_detections; i++)
         {
            if (i > 0U)
            {
               det_idx = static_cast<uint32_t>(raw_detections.detections[det_idx].processed.next_sorted_idx);
            }
            F360_Detection_Props_T& det_prop = det_Props[det_idx];
            const rspp_variant_A::RSPP_Detection_T& raw_det = raw_detections.detections[det_idx];
            const int32_t sensor_idx = raw_detections.detections[det_idx].raw.sensor_id - 1;
            const float32_t raw_det_az = raw_det.raw.azimuth * static_cast<float32_t>(sensors[sensor_idx].constant.polarity);
            const float32_t srr_veh_orientation = F360_PI - std::abs(sensors[sensor_idx].constant.mounting_position.vcs_boresight_azimuth_angle);
            const float32_t speed_ratio = std::abs(f360_host.speed / det_prop.range_rate_compensated);
            const bool det_moving_within_d2_bound = (det_prop.motion_status == rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING) &&
               (std::abs(det_prop.vcs_position.y) < k_d2_abs_lat_upr_bound);
            const bool det_range_az_within_threshold = (raw_det.raw.range > d1_range) && (std::abs(raw_det_az - d1_azimuth) <= k_azimuth_threshold);
            const bool det_speed_ratio_within_bounds = (speed_ratio < k_SR_upr_bound) && (speed_ratio > k_SR_lwr_bound);

            if (det_moving_within_d2_bound && det_range_az_within_threshold && det_speed_ratio_within_bounds)
            {
               //D2 detection params
               const float32_t d2_range = raw_det.raw.range;                        //Range of D2 detection
               const float32_t alpha = srr_veh_orientation - std::abs(d1_azimuth);  //Angle between vcs x axis and line of sight of SRR for D1 point

               //Applying reflection formula, calcs angle between D1 of sight and stationary detection line of sight
               const float32_t theta_refln_triangle = Trailer_Reflection(std::abs(d1_range),
                  std::abs(d2_range), std::abs(alpha), std::abs(stationary_lat_dist));

               //Check if there is a solution for trailer reflection formula
               if ((theta_refln_triangle > k_min_theta_refln_triangle) && (theta_refln_triangle < k_max_theta_refln_triangle))
               {
                  det_prop.f_ok_to_use = false;
                  det_prop.f_trailer_related_det = true;
               }
            }
         }
      }
   }

   /*===========================================================================*\
   * FUNCTION: Trailer_Reflection()
   *===========================================================================
   * RETURN VALUE:
   * float32_t ret
   *
   * PARAMETERS:
   * const float32_t d1_range
   * const float32_t d2_range
   * const float32_t alpha
   * const float32_t stationary_lat_dist
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * Calculates detection multireflection angle.
   *
   \*===========================================================================*/
   float32_t Trailer_Reflection(
      const float32_t d1_range,
      const float32_t d2_range,
      const float32_t alpha,
      const float32_t stationary_lat_dist)
   {
      float32_t ret = -1.0F;
      const float32_t c = 2.0F * stationary_lat_dist * (2.0F * d2_range - d1_range);
      const float32_t h = 4.0F * d2_range * d2_range - 4.0F * d1_range * d2_range;
      const float32_t a = h * F360_Sinf(alpha) + 2.0F * stationary_lat_dist * d1_range;
      const float32_t b = h * F360_Cosf(alpha);
      const float32_t hpt = F360_Get_Hypotenuse(a, b);

      if (hpt > 0.0F)
      {
         float32_t term1 = c / hpt;
         float32_t term2 = a / hpt;

         //Returning -1 if the sin inverse term is greater than 1
         if (term1 <= 1.0F)
         {
            //Adding bounds to prevent reset
            term1 = term1 > 0.0F ? std::min(term1, 1.0F) : std::max(term1, -1.0F);
            term2 = term2 > 0.0F ? std::min(term2, 1.0F) : std::max(term2, -1.0F);
            const float32_t theta_refln_triangle = F360_Asinf(term1) - F360_Asinf(term2);

            //Returning -1 if theta is negative
            ret = theta_refln_triangle >= 0.0F ? theta_refln_triangle : -1.0F;
         }
      }
      return ret;
   }
}
