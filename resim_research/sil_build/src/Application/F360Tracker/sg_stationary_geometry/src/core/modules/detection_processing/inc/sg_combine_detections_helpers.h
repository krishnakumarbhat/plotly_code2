/*=============================================================================================*\
* FILE: sg_combine_detections_helpers.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains an algorithm for combining new and old detections.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/
#ifndef SG_COMBINE_DETECTIONS_HELPERS_H
#define SG_COMBINE_DETECTIONS_HELPERS_H

#include <bitset>

#include "rspp_detection_list.h"
#include "sg_calibrations.h"
#include "sg_detection_storage.h"

namespace sg
{
   /**
    * @brief          Iterates over detections and removes the ones, which are close to (0, 0)
    *
    * @param[out]     detections - container holding internal detections
    **/
   void validate_position(DetectionStorage &detections);

   /**
    * @brief       Converts RSPP_Detection_T detection into Detection_T internal_detection
    *
    * @param[in]   calibrations - calibrations used in SG
    * @param[in]   dist_rear_axle_to_vcs - distance between rear axle and the center of the front bumper (VCS origin)
    * @param[in]   importance - importance of the detection to be converted
    * @param[in]   input_detection - input detection to be converted
    * @param[in]   sensors - sensors data
    * @param[out]  internal_detection - converted input detection
    **/
   Detection_T convert_detection_to_internal_type(const Common_Calibrations_T &calibrations,
                                                  const float dist_rear_axle_to_vcs,
                                                  const float importance,
                                                  const rspp::RSPP_Detection_T &input_detection,
                                                  const rspp::F360_Radar_Sensor_T (&sensors)[rspp::MAX_NUMBER_OF_SENSORS]);

   /**
    * @brief       Updates importance parameter of detections
    *
    * @param[in]   importance_calibrations - needed for importance calculation
    * @param[in]   host - host data
    * @param[out]  detections - detections which importance will be updated
    **/
   void update_importance(const Importance_Calibrations_T &importance_calibrations,
                          const RSPP_Host_T &host,
                          DetectionStorage &detections);

   /**
    * @brief       Calculates importance of selected input detections
    *
    * @param[in]   input_detections - used to check number of valid detections
    * @param[in]   host - host data
    * @param[in]   importance_calibrations - needed for importance calculation
    * @param[in]   input_detections_mask - array used to check which input detections are valid for combining
    * @param[out]  input_detections_importance - output array with calculated input detections importances
    **/
   void calculate_input_detections_importance(const SG_Input_Detections_T &input_detections,
                                              const RSPP_Host_T &host,
                                              const Importance_Calibrations_T &importance_calibrations,
                                              const std::bitset<SG_MAX_NUM_INPUT_DETS> &input_detections_mask,
                                              std::array<float, SG_MAX_NUM_INPUT_DETS> &input_detections_importance);

   /**
    * @brief          Calculates order of internal detections based on importance values.
    *                    Iterators are sorted in ascending order using stable sort algorithm.
    *
    * @param[in]      detections - detection container which will be sorted
    * @param[out]     detection_iterators_sorted - array with sorted DetectionStorage iterators
    **/
   void sort_by_importance(const DetectionStorage &detections,
                           std::array<DetectionList::iterator, SG_MAX_NUM_INTERNAL_DETS> &detection_iterators_sorted);

   /**
    * @brief          Select internal detections that belong to one drivability class
    *
    * @param[in]      detection_iterators - array with DetectionStorage iterators
    * @param[in]      drivability - drivability class that should be accepted
    * @param[out]     selected_detection_iterators - output array with selected DetectionStorage iterators
    *
    * @return         num_selected_detections - number of detections that were selected
    **/
   uint16_t select_by_drivability(const std::array<DetectionList::iterator, SG_MAX_NUM_INTERNAL_DETS> &detection_iterators,
                                  const SG_Drivability_Class_T drivability,
                                  std::array<DetectionList::iterator, SG_MAX_NUM_INTERNAL_DETS> &selected_detection_iterators);

   /**
    * @brief         Calculates order of input detections indices based on corresponding importance values.
    *                Values are sorted in ascending order using stable sort algorithm.
    *
    * @param[in]     importance - input detection importance array
    * @param[out]    input_detection_indices_sorted - array with sorted indices
    * @tparam        T - type of importance value
    * @tparam        N - size of std::array
    **/
   template <typename T, std::size_t N>
   void get_list_of_sorted_indices(const std::array<T, N> &importance, std::array<std::size_t, N> &input_detection_indices_sorted)
   {
      for (std::size_t index = 0U; index < N; index++)
      {
         input_detection_indices_sorted[index] = index;
      }

      std::sort(std::begin(input_detection_indices_sorted), std::end(input_detection_indices_sorted),
                [&importance](const std::size_t i, const std::size_t j) { return importance[i] > importance[j]; });
   }

   /**
    * @brief         Calculate how many input detections should be stored and how many internal detections should be removed.
    *                This comparison is focused on keeping only detections with the biggest importance.
    *
    * @param[in]     detection_iterators_sorted - sorted DetectionStorage iterators
    * @param[in]     input_detections_importance - input detections importance array
    * @param[in]     input_detection_indices_sorted - sorted input detections indices array
    * @param[in]     max_num_internal_detections_after_combine - maximum number of detections that may be stored
    * @param[in]     num_internal_detections_before_combine - number of internal detections before combine
    * @param[out]    num_input_detections_after_combine - how many input detections should be stored
    * @param[out]    num_internal_detections_to_remove - how many old/internal detections should be removed
    **/
   void calculate_num_of_input_detections_to_store(
      const std::array<DetectionList::iterator, SG_MAX_NUM_INTERNAL_DETS> &detection_iterators_sorted,
      const std::array<float, SG_MAX_NUM_INPUT_DETS> &input_detections_importance,
      const std::array<std::size_t, SG_MAX_NUM_INPUT_DETS> &input_detection_indices_sorted,
      const uint16_t max_num_internal_detections_after_combine,
      const uint16_t num_internal_detections_before_combine,
      uint16_t &num_input_detections_after_combine,
      uint16_t &num_internal_detections_to_remove);

   /**
    * @brief          Removes detections at the end of the list
    *
    * @param[in]      detection_iterators - DetectionStorage iterators
    * @param[in]      num_internal_detections - num of old/internal detections
    * @param[in]      num_internal_detections_to_remove - how many old/internal detections should be removed
    * @param[out]     detections - internal detections container which will be modified
    **/
   void remove_ultimate_internal_detections(const std::array<sg::DetectionList::iterator, SG_MAX_NUM_INTERNAL_DETS> &detection_iterators,
                                            const uint16_t num_internal_detections,
                                            const uint16_t num_internal_detections_to_remove,
                                            DetectionStorage &detections);
}
#endif