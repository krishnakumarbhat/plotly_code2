/*===================================================================================*\
* FILE: dc_assign_detections_to_subsegments_and_update_features.cpp
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains implementation of assign_detections_to_subsegments_and_update_features function.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#include "dc_assign_detections_to_subsegments_and_update_features.h"

#include <algorithm>
#include <cmath>

#include "dc_calculate_subsegment_features.h"

namespace sg
{
   namespace dc
   {
      void assign_detections_to_subsegments_and_update_features(DCContourStorage &dc_contours,
                                                                const rot::F360_Detection_Log_Output_T &rot_detections,
                                                                const SG_Input_Detections_T &input_detections,
                                                                const float dist_rear_axle_to_vcs,
                                                                const Drivability_Classification_Calibrations_T &cfg)
      {
         std::bitset<SG_MAX_NUM_INPUT_DETS> downselected_dets_mask{};
         get_valid_stationary_dets_mask(downselected_dets_mask, rot_detections, input_detections,
                                        cfg.features_min_rr_comp_of_stat_dealias_det);

         const std::size_t num_downselected_dets = downselected_dets_mask.count();
         std::array<geometry::Point3D_T, SG_MAX_NUM_INPUT_DETS> det_iso_positions{};
         std::array<uint16_t, SG_MAX_NUM_INPUT_DETS> sorted_det_indices{};
         get_sorted_det_iso_coordinates(det_iso_positions, sorted_det_indices, input_detections, downselected_dets_mask,
                                        num_downselected_dets, dist_rear_axle_to_vcs);

         const float det_seg_assignment_threshold_squared = std::pow(cfg.det_seg_assignment_threshold, 2.0F);
         (void) det_seg_assignment_threshold_squared; // MISRA
         const auto gate_half_width = cfg.subsegment_length / 2.0F + cfg.det_seg_assignment_threshold;
         (void) gate_half_width; // MISRA
         const float max_contour_bb_radius = std::sqrt(2.0F) * gate_half_width;
         (void) max_contour_bb_radius; // MISRA

         for (auto &contour : dc_contours)
         {
            if (contour.get_id() != INVALID_CONTOUR_ID)
            {
               std::array<uint16_t, SG_MAX_NUM_INPUT_DETS> contour_det_indices{};
               const uint8_t num_contour_dets = get_contour_bounding_box_det_indices(
                  contour_det_indices, contour.subsegments, det_iso_positions, num_downselected_dets, max_contour_bb_radius);
               (void) num_contour_dets; // MISRA
               for (auto &subsegment : contour.subsegments)
               {
                  // Increment subsegment past data age - it's done here, because we want to increment it even if that's not
                  // critical now or we have no detections found
                  subsegment.past_data.increment_age();
                  if (subsegment.begin_vertex.f_critical)
                  {
                     if (num_contour_dets > 0U)
                     {
                        std::array<uint16_t, SG_MAX_NUM_INPUT_DETS> subsegment_det_indices;
                        const uint8_t num_subsegment_dets = get_subsegment_det_indices(subsegment_det_indices, subsegment,
                                                                                       det_iso_positions, contour_det_indices,
                                                                                       num_contour_dets, gate_half_width);
                        if (num_subsegment_dets > 0U)
                        {
                           SubsegmentDetections_T current_data;
                           uint8_t num_associated_points{0U};
                           uint8_t num_valid_associated_points{0U};
                           for (uint16_t i{0U}; i < num_subsegment_dets; ++i)
                           {
                              const auto det_idx      = subsegment_det_indices[i];
                              const auto dist_squared = calculate_point_to_segment_dist_squared(
                                 subsegment.begin_vertex.position, subsegment.end_vertex.position, det_iso_positions[det_idx]);
                              if (dist_squared < det_seg_assignment_threshold_squared)
                              {
                                 const auto &detection      = input_detections.detections[sorted_det_indices[det_idx]];
                                 const float det_position_z = det_iso_positions[det_idx].z;
                                 if (validate_detection(detection.raw.confid_azimuth, detection.raw.confid_elevation,
                                                        det_position_z, cfg))
                                 {
                                    current_data.range[num_valid_associated_points]     = static_cast<float>(detection.raw.range);
                                    current_data.snr[num_valid_associated_points]       = static_cast<float>(detection.raw.snr);
                                    current_data.rcs[num_valid_associated_points]       = static_cast<float>(detection.raw.rcs);
                                    current_data.z_scs[num_valid_associated_points]     = det_position_z;
                                    current_data.z_scs_abs[num_valid_associated_points] = std::fabs(det_position_z);
                                    current_data.confid_azimuth[num_valid_associated_points]   = detection.raw.confid_azimuth;
                                    current_data.confid_elevation[num_valid_associated_points] = detection.raw.confid_elevation;
                                    current_data.f_super_res[num_valid_associated_points]      = detection.raw.f_super_res;
                                    current_data.f_bistatic[num_valid_associated_points]       = detection.raw.f_bistatic;
                                    ++num_valid_associated_points;
                                 }
                                 ++num_associated_points;
                                 if (num_associated_points == SG_MAX_NUM_DETS_PER_SUBSEGMENT)
                                 {
                                    break;
                                 }
                              }
                           }
                           subsegment.num_of_dets_associated_last_scan = num_associated_points;
                           current_data.num_associated_dets            = num_associated_points;
                           current_data.num_valid_dets                 = num_valid_associated_points;
                           calculate_subsegment_features(subsegment, current_data);

#ifdef SG_SAVE_DETECTIONS_ASSIGNED_TO_SUBSEGMENTS
                           DCContourStorage::assigned_detections[subsegment.subsegment_id] = std::move(current_data);
#endif
                        }
                        else
                        {
                           subsegment.num_of_dets_associated_last_scan = 0U;
                        }
                     }
                     else
                     {
                        subsegment.num_of_dets_associated_last_scan = 0U;
                     }
                     assign_features(subsegment);
                  }
                  else
                  {
                     subsegment.num_of_dets_associated_last_scan = 0U;
                  }
               }
            }
         }
      }

      float calculate_point_to_segment_dist_squared(const geometry::Point2D_T &vertex1,
                                                    const geometry::Point2D_T &vertex2,
                                                    const geometry::Point3D_T &point)
      {
         const auto segment = vertex2 - vertex1;
         const geometry::Point2D_T projected_point(point.x, point.y);
         const auto relative_position = segment * (projected_point - vertex1) / (segment * segment);
         geometry::Point2D_T closest_segment_point{};

         if (relative_position <= 0.0F)
         {
            closest_segment_point = vertex1;
         }
         else if (relative_position >= 1.0F)
         {
            closest_segment_point = vertex2;
         }
         else
         {
            closest_segment_point = vertex1 + (relative_position * segment);
         }
         const auto distance_segment = closest_segment_point - projected_point;
         return std::pow(distance_segment.x, 2.0F) + std::pow(distance_segment.y, 2.0F);
      }

      uint8_t get_contour_bounding_box_det_indices(std::array<uint16_t, SG_MAX_NUM_INPUT_DETS> &contour_det_indices,
                                                   const DC_Contour_T::SubsegmentList &subsegments,
                                                   const std::array<geometry::Point3D_T, SG_MAX_NUM_INPUT_DETS> &det_iso_positions,
                                                   const std::size_t num_downselected_dets,
                                                   const float max_contour_bb_radius)
      {
         float longitudinal_max = subsegments.back().end_vertex.position.x;
         float longitudinal_min = longitudinal_max;
         float lateral_max      = subsegments.back().end_vertex.position.y;
         float lateral_min      = lateral_max;
         for (const auto &subsegment : subsegments)
         {
            longitudinal_max = std::max(longitudinal_max, subsegment.begin_vertex.position.x);
            longitudinal_min = std::min(longitudinal_min, subsegment.begin_vertex.position.x);
            lateral_max      = std::max(lateral_max, subsegment.begin_vertex.position.y);
            lateral_min      = std::min(lateral_min, subsegment.begin_vertex.position.y);
         }
         longitudinal_max += max_contour_bb_radius;
         longitudinal_min -= max_contour_bb_radius;
         lateral_max += max_contour_bb_radius;
         lateral_min -= max_contour_bb_radius;

         uint8_t num_contour_dets = 0U;
         for (uint16_t det_idx{0U}; det_idx < static_cast<uint16_t>(num_downselected_dets); ++det_idx)
         {
            if (det_iso_positions[det_idx].x < longitudinal_min)
            {
               continue;
            }
            if (det_iso_positions[det_idx].x > longitudinal_max)
            {
               break;
            }
            if ((det_iso_positions[det_idx].y >= lateral_min) && (det_iso_positions[det_idx].y <= lateral_max))
            {
               contour_det_indices[num_contour_dets++] = det_idx;
            }
         }
         (void) longitudinal_max; // MISRA
         (void) longitudinal_min; // MISRA
         (void) lateral_max;      // MISRA
         (void) lateral_min;      // MISRA
         return num_contour_dets;
      }

      void get_sorted_det_iso_coordinates(std::array<geometry::Point3D_T, SG_MAX_NUM_INPUT_DETS> &det_iso_positions,
                                          std::array<uint16_t, SG_MAX_NUM_INPUT_DETS> &sorted_det_indices,
                                          const sg::SG_Input_Detections_T &input_detections,
                                          const std::bitset<SG_MAX_NUM_INPUT_DETS> &dets_mask,
                                          const std::size_t num_downselected_dets,
                                          const float dist_rear_axle_to_vcs)
      {
         if (num_downselected_dets > 0U)
         {
            uint16_t det_iso_idx = 0U;
            assert(input_detections.vcslong_det_idx_max >= 0); // algorithm uses sorting info from RSPP
            uint16_t input_det_idx = static_cast<uint16_t>(input_detections.vcslong_det_idx_min);
            for (uint16_t i{0U}; i < SG_MAX_NUM_INPUT_DETS; ++i)
            {
               if (dets_mask[input_det_idx])
               {
                  det_iso_positions[det_iso_idx].x =
                     input_detections.detections[input_det_idx].processed.vcs_position_x + dist_rear_axle_to_vcs;
                  det_iso_positions[det_iso_idx].y = -input_detections.detections[input_det_idx].processed.vcs_position_y;
                  det_iso_positions[det_iso_idx].z = -input_detections.detections[input_det_idx].processed.vcs_position_z;
                  sorted_det_indices[det_iso_idx]  = input_det_idx;
                  ++det_iso_idx;
               }
               if (det_iso_idx == num_downselected_dets)
               {
                  break;
               }
               assert(input_detections.detections[input_det_idx].processed.next_sorted_idx >= 0); // if we get negative value,
                                                                                                  // sorting info is incorrect
               input_det_idx = static_cast<uint16_t>(input_detections.detections[input_det_idx].processed.next_sorted_idx);
            }
            (void) input_det_idx; // MISRA
            (void) det_iso_idx;   // MISRA
         }
      }

      uint8_t get_subsegment_det_indices(std::array<uint16_t, SG_MAX_NUM_INPUT_DETS> &subsegment_det_indices,
                                         const Subsegment_T &subsegment,
                                         const std::array<geometry::Point3D_T, SG_MAX_NUM_INPUT_DETS> &det_iso_positions,
                                         const std::array<uint16_t, SG_MAX_NUM_INPUT_DETS> &contour_det_indices,
                                         const uint8_t num_contour_dets,
                                         const float gate_half_width)
      {
         uint8_t num_subsegment_dets = 0U;
         const auto segment_middle_x = (subsegment.begin_vertex.position.x + subsegment.end_vertex.position.x) / 2.0F;
         const auto gate_min_x       = segment_middle_x - gate_half_width;
         const auto gate_max_x       = segment_middle_x + gate_half_width;
         (void) gate_min_x; // MISRA
         (void) gate_max_x; // MISRA
         const auto segment_middle_y = (subsegment.begin_vertex.position.y + subsegment.end_vertex.position.y) / 2.0F;
         const auto gate_min_y       = segment_middle_y - gate_half_width;
         const auto gate_max_y       = segment_middle_y + gate_half_width;
         (void) gate_min_y; // MISRA
         (void) gate_max_y; // MISRA

         for (std::size_t i{0U}; i < num_contour_dets; ++i)
         {
            const auto det_idx = contour_det_indices[i];
            if ((det_iso_positions[det_idx].x > gate_min_x) && (det_iso_positions[det_idx].x < gate_max_x)
                && (det_iso_positions[det_idx].y > gate_min_y) && (det_iso_positions[det_idx].y < gate_max_y))
            {
               subsegment_det_indices[num_subsegment_dets] = det_idx;
               num_subsegment_dets++;
            }
         }
         return num_subsegment_dets;
      }

      void get_valid_stationary_dets_mask(std::bitset<SG_MAX_NUM_INPUT_DETS> &downselected_dets_mask,
                                          const rot::F360_Detection_Log_Output_T &rot_detections,
                                          const sg::SG_Input_Detections_T &input_detections,
                                          const float min_rr_comp_of_det)
      {
         if (input_detections.number_of_valid_detections > 0U)
         {
            for (std::size_t i{0U}; i < static_cast<size_t>(input_detections.number_of_valid_detections); ++i)
            {
               const int8_t motion_status_current_scan = input_detections.detections[i].processed.motion_status;
               const bool f_moving_dealiased           = (rot_detections.detection[i].f_dealiased == 1U)
                                               && (std::fabs(rot_detections.detection[i].rngrate_comp) > min_rr_comp_of_det);

               const auto f_motion_status = (motion_status_current_scan == MOTION_STATUS_STATIONARY_ID)
                                            || (motion_status_current_scan == MOTION_STATUS_AMBIGUOUS_ID);

               (void) downselected_dets_mask.set(i, (f_motion_status && (!f_moving_dealiased)));
               (void) f_moving_dealiased; // MISRA
            }
         }
      }

      bool validate_detection(const int8_t confid_azimuth,
                              const int8_t confid_elevation,
                              const float z_scs,
                              const Drivability_Classification_Calibrations_T &cfg)
      {
         return (confid_azimuth <= cfg.features_max_azimuth_confidence)
                && (confid_elevation <= cfg.features_max_elevation_confidence) && (z_scs >= cfg.features_z_min_threshold)
                && (z_scs <= cfg.features_z_max_threshold);
      }
   }
}
