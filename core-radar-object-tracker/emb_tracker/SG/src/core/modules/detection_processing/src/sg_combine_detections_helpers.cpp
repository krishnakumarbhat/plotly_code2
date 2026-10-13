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

#include "sg_determine_drivability_class.h"
#include "sg_drivability_class.h"

namespace sg
{
void combine_input_and_internal_dets(
   DetectionStorage &detections, const uint16_t max_num_internal_detections_after_combine,
   const SG_Input_Detections_T &input_detections, const uint16_t num_internal_detections,
   const uint16_t num_input_detections_before_combine, const CommonCalibs &common_calibrations, const float dist_rear_axle_to_vcs,
   const std::array<float, SG_MAX_NUM_INPUT_DETS> &input_detections_importance,
   const std::array<uint16_t, SG_MAX_NUM_INPUT_DETS> &input_detection_indices_sorted,
   const rspp::F360_Radar_Sensor_T (&sensors)[rspp::MAX_NUMBER_OF_SENSORS],
   const std::array<DetectionList::iterator, SG_MAX_NUM_INTERNAL_DETS> &internal_detection_iterators_sorted)
{
   if ((num_internal_detections + num_input_detections_before_combine) > max_num_internal_detections_after_combine)
   {
      std::size_t input_det_index = 0U;
      if (num_internal_detections < max_num_internal_detections_after_combine)
      {
         // push input detections with higher importance to detection storage until max_num_internal_detections_after_combine
         // reached
         for (std::size_t num_pushed_input_detections = num_internal_detections;
              num_pushed_input_detections < max_num_internal_detections_after_combine; ++num_pushed_input_detections)
         {
            const Detection_T internal_detection = convert_detection_to_internal_type(
               common_calibrations, dist_rear_axle_to_vcs,
               input_detections_importance[input_detection_indices_sorted[input_det_index]],
               input_detections.detections[input_detection_indices_sorted[input_det_index]], sensors);
            (void)detections.push_back(internal_detection);
            // TODO: Decide what to do with push_back result https://jiraprod.aptiv.com/browse/FZD-822
            ++input_det_index;
         }
      }
      // replace internal detections with lowest importance by input dets
      for (std::size_t idx = num_internal_detections; idx > 0U; --idx)
      {
         const auto &det_it = internal_detection_iterators_sorted[idx - 1U];
         if (det_it->importance < input_detections_importance[input_detection_indices_sorted[input_det_index]])
         {
            (void)detections.erase(det_it);
            const Detection_T internal_detection = convert_detection_to_internal_type(
               common_calibrations, dist_rear_axle_to_vcs,
               input_detections_importance[input_detection_indices_sorted[input_det_index]],
               input_detections.detections[input_detection_indices_sorted[input_det_index]], sensors);
            (void)detections.push_back(internal_detection);
            // TODO: Decide what to do with push_back result https://jiraprod.aptiv.com/browse/FZD-822
            ++input_det_index;
         }
         else
         {
            break;
         }
      }
      (void)input_det_index; // MISRA
   }
   else
   {
      // push all input detections to detection storage
      for (std::size_t i = 0U; i < num_input_detections_before_combine; ++i)
      {
         const Detection_T internal_detection = convert_detection_to_internal_type(
            common_calibrations, dist_rear_axle_to_vcs, input_detections_importance[input_detection_indices_sorted[i]],
            input_detections.detections[input_detection_indices_sorted[i]], sensors);
         (void)detections.push_back(internal_detection);
         // TODO: Decide what to do with push_back result https://jiraprod.aptiv.com/browse/FZD-822
      }
   }
}

float calculate_detection_importance(const float position_x, const float position_y, const float position_z, const uint16_t age,
                                     const uint16_t vertex_age, const float existence_probability, const float host_curvature,
                                     const float host_speed, const ImportanceCalibs &importance_calibrations)
{
   float adjusted_position_y = position_y;
   float importance          = 1.0F;

   if (vertex_age > importance_calibrations.max_associated_det_vertex_age)
   {
      importance = 0.0F;
   }
   else
   {
      // taking into account host curvature - using approximate formula as described in FZD-657
      if (std::fabs(host_curvature) > 1e-5F)
      {
         adjusted_position_y -= 0.5F * position_x * position_x * host_curvature;
      }

      // adjust detection position
      float adjusted_position_x        = position_x - host_speed;
      const float amplification_factor = adjusted_position_x * host_speed;
      if (amplification_factor <= 0.0F)
      {
         adjusted_position_x *= (1.0F - importance_calibrations.shape_correction_factors[0U] * amplification_factor);
      }
      else
      {
         adjusted_position_y /= (1.0F + importance_calibrations.shape_correction_factors[1U] * amplification_factor);
      }

      // calculate Manhattan distance using adjusted positions
      const float adjusted_distance_to_host = importance_calibrations.distance_factors[0U] * std::fabs(adjusted_position_x) +
                                              importance_calibrations.distance_factors[1U] * std::fabs(adjusted_position_y) +
                                              importance_calibrations.distance_factors[2U] * std::fabs(position_z);
      if (importance_calibrations.use_existence_probability)
      {
         importance = existence_probability;
      }

      importance /= (1.0F + importance_calibrations.age_impact_factor * static_cast<float>(age)) *
                    (1.0F + importance_calibrations.distance_impact_factor * adjusted_distance_to_host);
   }
   (void)adjusted_position_y; // MISRA
   return importance;
}

Detection_T convert_detection_to_internal_type(const CommonCalibs &calibrations, const float dist_rear_axle_to_vcs,
                                               const float importance, const rspp::RSPP_Detection_T &input_detection,
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

   const int32_t sensor_id    = input_detection.raw.sensor_id;
   internal_detection.look_id = ((0 < sensor_id) && (sensor_id <= static_cast<int32_t>(rspp::MAX_NUMBER_OF_SENSORS))) ?
                                   sensors[sensor_id - 1U].variable.look_id :
                                   RSPP_Det_Look_ID_T::RSPP_DET_LOOK_ID_INVALID;

   return internal_detection;
}

void update_importance(const ImportanceCalibs &importance_calibrations, const RSPP_Host_T &host, DetectionStorage &detections)
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
         detection.importance = calculate_detection_importance(
            detection.position.x, detection.position.y, detection.position.z, detection.age, detection.vertex_age,
            detection.existence_probability, host_curvature, host_speed, importance_calibrations);
         detection_it++;
      }
   }
   (void)host_curvature; // MISRA
   (void)host_speed;     // MISRA
}
} // namespace sg
