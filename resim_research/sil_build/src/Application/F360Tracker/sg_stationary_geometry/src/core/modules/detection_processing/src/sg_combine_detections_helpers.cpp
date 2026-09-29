/*=============================================================================================*\
* FILE: sg_combine_detections_helpers.cpp
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains an algorithm for combining input and old detections.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN, "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#include "sg_combine_detections_helpers.h"

#include <algorithm>

#include "sg_calculate_detection_importance.h"
#include "sg_determine_drivability_class.h"
#include "sg_drivability_class.h"

namespace sg
{
   void validate_position(DetectionStorage &detections)
   {
      auto it = detections.begin();
      while (it != detections.end())
      {
         if ((std::fabs(it->position.x) <= std::numeric_limits<float>::epsilon())
             && (std::fabs(it->position.y) <= std::numeric_limits<float>::epsilon()))
         {
            it = detections.erase(it);
         }
         else
         {
            ++it;
         }
      }
   }

   static int8_t determine_detection_look_id(const rspp::RSPP_Detection_T &detection,
                                             const rspp::F360_Radar_Sensor_T (&sensors)[rspp::MAX_NUMBER_OF_SENSORS])
   {
      int8_t look_id{RSPP_Det_Look_ID_T::RSPP_DET_LOOK_ID_INVALID};
      const int32_t sensor_id = detection.raw.sensor_id;

      if ((0 < sensor_id) && (sensor_id <= static_cast<int32_t>(rspp::MAX_NUMBER_OF_SENSORS)))
      {
         look_id = sensors[sensor_id - 1U].variable.look_id;
      }

      return look_id;
   }

   Detection_T convert_detection_to_internal_type(const Common_Calibrations_T &calibrations,
                                                  const float dist_rear_axle_to_vcs,
                                                  const float importance,
                                                  const rspp::RSPP_Detection_T &input_detection,
                                                  const rspp::F360_Radar_Sensor_T (&sensors)[rspp::MAX_NUMBER_OF_SENSORS])
   {
      Detection_T internal_detection{};
      internal_detection.position.x               = input_detection.processed.vcs_position_x + dist_rear_axle_to_vcs; // VCS to ISO
      internal_detection.position.y               = -input_detection.processed.vcs_position_y;                        // VCS to ISO
      internal_detection.position.z               = -input_detection.processed.vcs_position_z;                        // VCS to ISO
      internal_detection.position_squeezed.z      = internal_detection.position.z;
      internal_detection.range_rate_compensated   = input_detection.processed.range_rate_compensated;
      internal_detection.importance               = importance;
      internal_detection.probability_of_detection = 0.75F; // TODO in FZD-527
      internal_detection.position_cov.x           = calibrations.r_position_covariance.x;
      internal_detection.position_cov.y           = calibrations.r_position_covariance.y;
      internal_detection.position_cov.xy          = calibrations.r_position_covariance.xy;
      const sg::geometry::Point3D_T input_position{input_detection.processed.vcs_position_x, input_detection.processed.vcs_position_y,
                                                   input_detection.processed.vcs_position_z};
      internal_detection.drivability = sg_determine_drivability_class(calibrations.view_ranges, input_position);
      internal_detection.look_id     = determine_detection_look_id(input_detection, sensors);


      return internal_detection;
   }

   void update_importance(const Importance_Calibrations_T &importance_calibrations, const RSPP_Host_T &host, DetectionStorage &detections)
   {
      const float host_curvature = static_cast<float>(host.curvature_rear);
      const float host_speed     = static_cast<float>(host.speed);

      // TODO: https://jiraprod.aptiv.com/browse/FZD-1809
      for (auto detection_it = detections.begin(); detection_it != detections.end();)
      {
         auto &detection = *detection_it;
         if (detection.age > importance_calibrations.max_det_age)
         {
            detection_it = detections.erase(detection_it);
         }
         else
         {
            detection.importance = calculate_detection_importance(detection.position.x, detection.position.y, detection.position.z,
                                                                  detection.age, detection.vertex_age, detection.existence_probability,
                                                                  host_curvature, host_speed, importance_calibrations);
            detection_it++;
         }
      }
      (void) host_curvature; // MISRA
      (void) host_speed;     // MISRA
   }

   void calculate_input_detections_importance(const SG_Input_Detections_T &input_detections,
                                              const RSPP_Host_T &host,
                                              const Importance_Calibrations_T &importance_calibrations,
                                              const std::bitset<SG_MAX_NUM_INPUT_DETS> &input_detections_mask,
                                              std::array<float, SG_MAX_NUM_INPUT_DETS> &input_detections_importance)
   {
      const float dist_rear_axle_to_vcs = static_cast<float>(host.dist_rear_axle_to_vcs_m);
      const float host_curvature        = static_cast<float>(host.curvature_rear);
      const float host_speed            = static_cast<float>(host.speed);
      for (std::size_t idx = 0U; idx < input_detections.number_of_valid_detections; idx++)
      {
         if (input_detections_mask[idx])
         {
            const uint16_t input_detection_age        = 0U;
            const uint16_t input_detection_vertex_age = 0;
            const float existence_probability         = 0.0F;
            input_detections_importance[idx]          = calculate_detection_importance(
               input_detections.detections[idx].processed.vcs_position_x + dist_rear_axle_to_vcs, // VCS to ISO
               -input_detections.detections[idx].processed.vcs_position_y,                        // VCS to ISO
               -input_detections.detections[idx].processed.vcs_position_z,                        // VCS to ISO
               input_detection_age, input_detection_vertex_age, existence_probability, host_curvature, host_speed,
               importance_calibrations);
         }
      }
      (void) dist_rear_axle_to_vcs; // MISRA
      (void) host_curvature;        // MISRA
      (void) host_speed;            // MISRA
   }

   void sort_by_importance(const DetectionStorage &detections,
                           std::array<DetectionList::iterator, SG_MAX_NUM_INTERNAL_DETS> &detection_iterators_sorted)
   {
      auto it                           = detections.begin();
      const std::size_t detections_size = detections.size();
      for (std::size_t idx = 0U; idx < detections_size; idx++)
      {
         detection_iterators_sorted[idx] = it;
         it++;
      }
      (void) it; // MISRA
      const auto detection_iterators_sorted_end =
         std::next(detection_iterators_sorted.begin(), static_cast<ptrdiff_t>(detections_size));
      const auto is_importance_greater = [](const DetectionList::iterator &lhs, const DetectionList::iterator &rhs)
      { return lhs->importance > rhs->importance; };
      std::sort(detection_iterators_sorted.begin(), detection_iterators_sorted_end, is_importance_greater);
   }

   uint16_t select_by_drivability(const std::array<DetectionList::iterator, SG_MAX_NUM_INTERNAL_DETS> &detection_iterators,
                                  const SG_Drivability_Class_T drivability,
                                  std::array<DetectionList::iterator, SG_MAX_NUM_INTERNAL_DETS> &selected_detection_iterators)
   {
      uint16_t num_selected_detections = 0U;
      for (const auto &it : detection_iterators)
      {
         if (it != nullptr)
         {
            if ((*it).drivability == drivability)
            {
               selected_detection_iterators[num_selected_detections] = it;
               num_selected_detections++;
            }
         }
         else
         {
            break;
         }
      }
      return num_selected_detections;
   }

   void calculate_num_of_input_detections_to_store(
      const std::array<DetectionList::iterator, SG_MAX_NUM_INTERNAL_DETS> &detection_iterators_sorted,
      const std::array<float, SG_MAX_NUM_INPUT_DETS> &input_detections_importance,
      const std::array<std::size_t, SG_MAX_NUM_INPUT_DETS> &input_detection_indices_sorted,
      const uint16_t max_num_internal_detections_after_combine,
      const uint16_t num_internal_detections_before_combine,
      uint16_t &num_input_detections_after_combine,
      uint16_t &num_internal_detections_to_remove)
   {
      uint16_t num_all_detections_after_combine = 0U;
      num_input_detections_after_combine        = 0U;
      uint16_t num_input_detections_before_combine =
         std::count_if(input_detections_importance.begin(), input_detections_importance.end(),
                       [](const float det_importance) { return det_importance > 0.0F; });

      uint16_t num_internal_detections_after_combine = 0U;
      while (num_all_detections_after_combine < max_num_internal_detections_after_combine)
      {
         if ((num_internal_detections_after_combine < num_internal_detections_before_combine)
             && (num_input_detections_after_combine < num_input_detections_before_combine))
         {
            static_assert(std::numeric_limits<decltype(num_internal_detections_after_combine)>::max()
                             <= std::numeric_limits<std::ptrdiff_t>::max(),
                          "Possible maximum value is to high.");

            const auto &internal_detection_it = detection_iterators_sorted[num_internal_detections_after_combine];
            const auto new_detection_idx      = input_detection_indices_sorted[num_input_detections_after_combine];
            if ((*internal_detection_it).importance > input_detections_importance[new_detection_idx])
            {
               num_internal_detections_after_combine++;
            }
            else
            {
               num_input_detections_after_combine++;
            }
            num_all_detections_after_combine++;
         }
         else if ((num_internal_detections_after_combine < num_internal_detections_before_combine)
                  && (num_input_detections_after_combine >= num_input_detections_before_combine))
         {
            auto num_remaining_internal_detections =
               static_cast<uint16_t>(num_internal_detections_before_combine - num_internal_detections_after_combine);
            const auto num_remaining_free_slots =
               static_cast<uint16_t>(max_num_internal_detections_after_combine - num_all_detections_after_combine);
            num_remaining_internal_detections = std::min(num_remaining_internal_detections, num_remaining_free_slots);
            num_internal_detections_after_combine += num_remaining_internal_detections;
            num_all_detections_after_combine += num_remaining_internal_detections;
         }
         else if ((num_internal_detections_after_combine >= num_internal_detections_before_combine)
                  && (num_input_detections_after_combine < num_input_detections_before_combine))
         {
            auto num_remaining_input_detections =
               static_cast<uint16_t>(num_input_detections_before_combine - num_input_detections_after_combine);
            const auto num_remaining_free_slots =
               static_cast<uint16_t>(max_num_internal_detections_after_combine - num_all_detections_after_combine);
            num_remaining_input_detections = std::min(num_remaining_input_detections, num_remaining_free_slots);
            num_input_detections_after_combine += num_remaining_input_detections;
            num_all_detections_after_combine += num_remaining_input_detections;
         }
         else
         {
            break;
         }
      }

      (void) num_input_detections_before_combine; // MISRA
      num_internal_detections_to_remove =
         static_cast<uint16_t>(num_internal_detections_before_combine - num_internal_detections_after_combine);
   }

   void remove_ultimate_internal_detections(const std::array<sg::DetectionList::iterator, SG_MAX_NUM_INTERNAL_DETS> &detection_iterators,
                                            const uint16_t num_internal_detections,
                                            const uint16_t num_internal_detections_to_remove,
                                            DetectionStorage &detections)
   {
      assert(num_internal_detections >= num_internal_detections_to_remove);
      for (auto idx = 1U; idx <= num_internal_detections_to_remove; idx++)
      {
         (void) detections.erase(detection_iterators[num_internal_detections - idx]);
      }
   }
}
