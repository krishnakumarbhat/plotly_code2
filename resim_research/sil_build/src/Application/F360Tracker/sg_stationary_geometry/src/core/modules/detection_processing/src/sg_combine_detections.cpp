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
   void combine_detections(DetectionStorage &detections,
                           const Importance_Calibrations_T &importance_calibrations,
                           const Common_Calibrations_T &common_calibrations,
                           const RSPP_Host_T &host,
                           const rspp::F360_Radar_Sensor_T (&sensors)[rspp::MAX_NUMBER_OF_SENSORS],
                           const SG_Input_Detections_T &input_detections,
                           const std::bitset<SG_MAX_NUM_INPUT_DETS> &nondrivable_detections_mask,
                           const std::bitset<SG_MAX_NUM_INPUT_DETS> &underdrivable_detections_mask)
   {
      // 1. Handle and sort DetectionStorage iterators
      validate_position(detections);
      Importance_Calibrations_T tmp_importance_calibrations = importance_calibrations;
      tmp_importance_calibrations.adjust_distance_factors(static_cast<float>(host.speed));
      update_importance(tmp_importance_calibrations, host, detections);
      std::array<DetectionList::iterator, SG_MAX_NUM_INTERNAL_DETS> internal_detection_iterators_sorted;
      std::array<DetectionList::iterator, SG_MAX_NUM_INTERNAL_DETS> nondrivable_internal_detection_iterators_sorted;
      std::array<DetectionList::iterator, SG_MAX_NUM_INTERNAL_DETS> underdrivable_internal_detection_iterators_sorted;
      std::fill(internal_detection_iterators_sorted.begin(), internal_detection_iterators_sorted.end(), DetectionList::iterator{});
      sort_by_importance(detections, internal_detection_iterators_sorted);
      const uint16_t num_nondrivable_internal_detections = select_by_drivability(
         internal_detection_iterators_sorted, SG_Drivability_Class_T::NONDRIVABLE, nondrivable_internal_detection_iterators_sorted);
      const uint16_t num_underdrivable_internal_detections =
         select_by_drivability(internal_detection_iterators_sorted, SG_Drivability_Class_T::UNDERDRIVABLE,
                               underdrivable_internal_detection_iterators_sorted);

      // 2. Handle and sort input detections importance indices
      std::array<float, SG_MAX_NUM_INPUT_DETS> nondrivable_input_detections_importance{};
      std::array<float, SG_MAX_NUM_INPUT_DETS> underdrivable_input_detections_importance{};
      calculate_input_detections_importance(input_detections, host, tmp_importance_calibrations, nondrivable_detections_mask,
                                            nondrivable_input_detections_importance);
      calculate_input_detections_importance(input_detections, host, tmp_importance_calibrations, underdrivable_detections_mask,
                                            underdrivable_input_detections_importance);
      std::array<std::size_t, SG_MAX_NUM_INPUT_DETS> nondrivable_input_detection_indices_sorted{};
      std::array<std::size_t, SG_MAX_NUM_INPUT_DETS> underdrivable_input_detection_indices_sorted{};
      get_list_of_sorted_indices(nondrivable_input_detections_importance, nondrivable_input_detection_indices_sorted);
      get_list_of_sorted_indices(underdrivable_input_detections_importance, underdrivable_input_detection_indices_sorted);

      // 3. Calculate how many input dets should be stored and how many internal dets should be removed within each class
      uint16_t num_nondrivable_input_detections_after_combine{};
      uint16_t num_nondrivable_internal_detections_to_remove{};
      calculate_num_of_input_detections_to_store(
         nondrivable_internal_detection_iterators_sorted, nondrivable_input_detections_importance,
         nondrivable_input_detection_indices_sorted, SG_MAX_NUM_NONDRIVABLE_DETS, num_nondrivable_internal_detections,
         num_nondrivable_input_detections_after_combine, num_nondrivable_internal_detections_to_remove);
      uint16_t num_underdrivable_input_detections_after_combine{};
      uint16_t num_underdrivable_internal_detections_to_remove{};
      calculate_num_of_input_detections_to_store(
         underdrivable_internal_detection_iterators_sorted, underdrivable_input_detections_importance,
         underdrivable_input_detection_indices_sorted, SG_MAX_NUM_UNDERDRIVABLE_DETS, num_underdrivable_internal_detections,
         num_underdrivable_input_detections_after_combine, num_underdrivable_internal_detections_to_remove);

      // 4. Combine detections
      remove_ultimate_internal_detections(nondrivable_internal_detection_iterators_sorted, num_nondrivable_internal_detections,
                                          num_nondrivable_internal_detections_to_remove, detections);
      remove_ultimate_internal_detections(underdrivable_internal_detection_iterators_sorted, num_underdrivable_internal_detections,
                                          num_underdrivable_internal_detections_to_remove, detections);

      const float dist_rear_axle_to_vcs = static_cast<float>(host.dist_rear_axle_to_vcs_m);
      for (auto i = 0U; i < num_nondrivable_input_detections_after_combine; i++)
      {
         const Detection_T internal_detection =
            convert_detection_to_internal_type(common_calibrations, dist_rear_axle_to_vcs,
                                               nondrivable_input_detections_importance[nondrivable_input_detection_indices_sorted[i]],
                                               input_detections.detections[nondrivable_input_detection_indices_sorted[i]], sensors);
         (void) detections.push_back(internal_detection);
         // TODO: Decide what to do with push_back result https://jiraprod.aptiv.com/browse/FZD-822
      }
      for (auto i = 0U; i < num_underdrivable_input_detections_after_combine; i++)
      {
         const Detection_T internal_detection = convert_detection_to_internal_type(
            common_calibrations, dist_rear_axle_to_vcs,
            underdrivable_input_detections_importance[underdrivable_input_detection_indices_sorted[i]],
            input_detections.detections[underdrivable_input_detection_indices_sorted[i]], sensors);

         (void) detections.push_back(internal_detection);
         // TODO: Decide what to do with push_back result https://jiraprod.aptiv.com/browse/FZD-822
      }
      (void) dist_rear_axle_to_vcs; // MISRA
   }
}
