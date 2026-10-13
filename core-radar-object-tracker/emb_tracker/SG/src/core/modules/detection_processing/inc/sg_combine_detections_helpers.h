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
#include "sg_input.h"

namespace sg
{
/**
 * @brief            Combines inputs and internal detections. Selects no more than max_num_internal_detections_after_combine
 *                   detections in detection storage with highest importance.
 *
 * @param[in, out]   detections
 * @param[in]        max_num_internal_detections_after_combine
 * @param[in]        input_detections
 * @param[in]        num_internal_detections
 * @param[in]        num_input_detections_before_combine
 * @param[in]        common_calibrations
 * @param[in]        dist_rear_axle_to_vcs
 * @param[in]        input_detections_importance
 * @param[in]        input_detection_indices_sorted
 * @param[in]        internal_detection_iterators_sorted
 */
void combine_input_and_internal_dets(
   DetectionStorage &detections, const uint16_t max_num_internal_detections_after_combine,
   const SG_Input_Detections_T &input_detections, const uint16_t num_internal_detections,
   const uint16_t num_input_detections_before_combine, const CommonCalibs &common_calibrations, const float dist_rear_axle_to_vcs,
   const std::array<float, SG_MAX_NUM_INPUT_DETS> &input_detections_importance,
   const std::array<uint16_t, SG_MAX_NUM_INPUT_DETS> &input_detection_indices_sorted,
   const rspp::F360_Radar_Sensor_T (&sensors)[rspp::MAX_NUMBER_OF_SENSORS],
   const std::array<DetectionList::iterator, SG_MAX_NUM_INTERNAL_DETS> &internal_detection_iterators_sorted);

/**
 * @brief       Calculate importance of detection.
 *
 * @param[in]   position_x
 * @param[in]   position_y
 * @param[in]   position_z
 * @param[in]   age
 * @param[in]   vertex_age
 * @param[in]   existence_probability
 * @param[in]   host_curvature
 * @param[in]   host_speed
 * @param[in]   importance_calibrations
 *
 * @return      importance
 */
float calculate_detection_importance(const float position_x, const float position_y, const float position_z, const uint16_t age,
                                     const uint16_t vertex_age, const float existence_probability, const float host_curvature,
                                     const float host_speed, const ImportanceCalibs &importance_calibrations);

/**
 * @brief       Converts RSPP_Detection_T detection into Detection_T internal_detection
 *
 * @param[in]   calibrations - calibrations used in SG
 * @param[in]   dist_rear_axle_to_vcs - distance between rear axle and the center of the front bumper (VCS origin)
 * @param[in]   importance - importance of the detection to be converted
 * @param[in]   input_detection - input detection to be converted
 * @param[in]   sensors - sensors data
 *
 * @return      internal_detection - converted input detection
 **/
Detection_T convert_detection_to_internal_type(const CommonCalibs &calibrations, const float dist_rear_axle_to_vcs,
                                               const float importance, const rspp::RSPP_Detection_T &input_detection,
                                               const rspp::F360_Radar_Sensor_T (&sensors)[rspp::MAX_NUMBER_OF_SENSORS]);

/**
 * @brief       Updates importance parameter of detections
 *
 * @param[in]   importance_calibrations - needed for importance calculation
 * @param[in]   host - host data
 * @param[out]  detections - detections which importance will be updated
 **/
void update_importance(const ImportanceCalibs &importance_calibrations, const RSPP_Host_T &host, DetectionStorage &detections);
} // namespace sg
#endif
