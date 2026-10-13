/*=============================================================================================*\
* FILE: sg_combine_detections.cpp
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
#include "sg_combine_detections.h"

#include "sg_combine_detections_helpers.h"

namespace sg
{
void combine_detections(DetectionStorage &detections, const ImportanceCalibs &importance_calibrations,
                        const CommonCalibs &common_calibrations, const RSPP_Host_T &host,
                        const rspp::F360_Radar_Sensor_T (&sensors)[rspp::MAX_NUMBER_OF_SENSORS],
                        const SG_Input_Detections_T &input_detections,
                        const std::bitset<SG_MAX_NUM_INPUT_DETS> &nondrivable_detections_mask,
                        const std::bitset<SG_MAX_NUM_INPUT_DETS> &underdrivable_detections_mask)
{
   // 1. Handle and sort DetectionStorage iterators
   ImportanceCalibs tmp_importance_calibrations = importance_calibrations;
   tmp_importance_calibrations.adjust_distance_factors(static_cast<float>(host.speed));
   update_importance(tmp_importance_calibrations, host, detections);

   std::array<DetectionList::iterator, SG_MAX_NUM_INTERNAL_DETS> nondrivable_internal_detection_iterators_sorted;
   std::array<DetectionList::iterator, SG_MAX_NUM_INTERNAL_DETS> underdrivable_internal_detection_iterators_sorted;

   uint16_t num_nondrivable_internal_detections   = 0U;
   uint16_t num_underdrivable_internal_detections = 0U;
   const auto det_end_it                          = detections.end();
   for (auto det_it = detections.begin(); det_it != det_end_it; ++det_it)
   {
      if (det_it->drivability == SG_Drivability_Class_T::NONDRIVABLE)
      {
         nondrivable_internal_detection_iterators_sorted[num_nondrivable_internal_detections] = det_it;
         ++num_nondrivable_internal_detections;
      }
      else if (det_it->drivability == SG_Drivability_Class_T::UNDERDRIVABLE)
      {
         underdrivable_internal_detection_iterators_sorted[num_underdrivable_internal_detections] = det_it;
         ++num_underdrivable_internal_detections;
      }
      else
      {
         // MISRA
      }
   }

   const auto is_importance_greater = [](const DetectionList::iterator &lhs, const DetectionList::iterator &rhs)
   { return lhs->importance > rhs->importance; };
   std::sort(nondrivable_internal_detection_iterators_sorted.begin(),
             std::next(nondrivable_internal_detection_iterators_sorted.begin(),
                       static_cast<ptrdiff_t>(num_nondrivable_internal_detections)),
             is_importance_greater);
   std::sort(underdrivable_internal_detection_iterators_sorted.begin(),
             std::next(underdrivable_internal_detection_iterators_sorted.begin(),
                       static_cast<ptrdiff_t>(num_underdrivable_internal_detections)),
             is_importance_greater);

   // 2. Handle and sort input detections importance indices
   std::array<float, SG_MAX_NUM_INPUT_DETS> nondrivable_input_detections_importance{};
   std::array<float, SG_MAX_NUM_INPUT_DETS> underdrivable_input_detections_importance{};

   const float dist_rear_axle_to_vcs                          = static_cast<float>(host.dist_rear_axle_to_vcs_m);
   const float host_curvature                                 = static_cast<float>(host.curvature_rear);
   const float host_speed                                     = static_cast<float>(host.speed);
   uint16_t num_nondrivable_input_detections_before_combine   = 0U;
   uint16_t num_underdrivable_input_detections_before_combine = 0U;
   for (std::size_t idx = 0U; idx < input_detections.number_of_valid_detections; idx++)
   {
      const uint16_t input_detection_age        = 0U;
      const uint16_t input_detection_vertex_age = 0U;
      const float default_existence_probability = 0.0F;
      const float input_det_position_x =
         static_cast<float>(input_detections.detections[idx].processed.vcs_position_x) + dist_rear_axle_to_vcs;
      const float detection_importance =
         calculate_detection_importance(input_det_position_x,                                       // VCS
                                                                                                    // to
                                                                                                    // ISO
                                        -input_detections.detections[idx].processed.vcs_position_y, // VCS to ISO
                                        -input_detections.detections[idx].processed.vcs_position_z, // VCS to ISO
                                        input_detection_age, input_detection_vertex_age, default_existence_probability,
                                        host_curvature, host_speed, tmp_importance_calibrations);
      if (nondrivable_detections_mask[idx])
      {
         nondrivable_input_detections_importance[idx] = detection_importance;
         ++num_nondrivable_input_detections_before_combine;
      }
      else if (underdrivable_detections_mask[idx])
      {
         underdrivable_input_detections_importance[idx] = detection_importance;
         ++num_underdrivable_input_detections_before_combine;
      }
      else
      {
         // MISRA
      }
      (void)detection_importance; // MISRA
      (void)host_curvature;       // MISRA
      (void)host_speed;           // MISRA
   }

   std::array<uint16_t, SG_MAX_NUM_INPUT_DETS> nondrivable_input_detection_indices_sorted{};
   std::array<uint16_t, SG_MAX_NUM_INPUT_DETS> underdrivable_input_detection_indices_sorted{};
   for (uint16_t index = 0U; index < input_detections.number_of_valid_detections; index++)
   {
      nondrivable_input_detection_indices_sorted[index]   = index;
      underdrivable_input_detection_indices_sorted[index] = index;
   }
   const auto last_nondrivable_input_detection_indices_sorted_it =
      std::next(nondrivable_input_detection_indices_sorted.begin(),
                static_cast<std::ptrdiff_t>(input_detections.number_of_valid_detections));
   std::sort(std::begin(nondrivable_input_detection_indices_sorted), last_nondrivable_input_detection_indices_sorted_it,
             [&nondrivable_input_detections_importance](const std::size_t i, const std::size_t j)
             { return nondrivable_input_detections_importance[i] > nondrivable_input_detections_importance[j]; });
   const auto last_underdrivable_input_detection_indices_sorted_it =
      std::next(underdrivable_input_detection_indices_sorted.begin(),
                static_cast<std::ptrdiff_t>(input_detections.number_of_valid_detections));
   std::sort(std::begin(underdrivable_input_detection_indices_sorted), last_underdrivable_input_detection_indices_sorted_it,
             [&underdrivable_input_detections_importance](const std::size_t i, const std::size_t j)
             { return underdrivable_input_detections_importance[i] > underdrivable_input_detections_importance[j]; });

   // 3. Combine input and internal detections
   combine_input_and_internal_dets(detections, SG_MAX_NUM_NONDRIVABLE_DETS, input_detections, num_nondrivable_internal_detections,
                                   num_nondrivable_input_detections_before_combine, common_calibrations, dist_rear_axle_to_vcs,
                                   nondrivable_input_detections_importance, nondrivable_input_detection_indices_sorted, sensors,
                                   nondrivable_internal_detection_iterators_sorted);
   combine_input_and_internal_dets(detections, SG_MAX_NUM_UNDERDRIVABLE_DETS, input_detections,
                                   num_underdrivable_internal_detections, num_underdrivable_input_detections_before_combine,
                                   common_calibrations, dist_rear_axle_to_vcs, underdrivable_input_detections_importance,
                                   underdrivable_input_detection_indices_sorted, sensors,
                                   underdrivable_internal_detection_iterators_sorted);
   (void)dist_rear_axle_to_vcs; // MISRA
}
} // namespace sg
