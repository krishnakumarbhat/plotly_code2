/*===================================================================================*\
* FILE: dc_assign_detections_to_subsegments_and_update_features.h
*====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains declaration assign_detections_to_subsegments_and_update_features function.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef DC_ASSIGN_DETECTIONS_TO_SUBSEGMENTS_AND_UPDATE_FEATURES_H
#define DC_ASSIGN_DETECTIONS_TO_SUBSEGMENTS_AND_UPDATE_FEATURES_H

#include <bitset>

#include "dc_contour_storage.h"
#include "sg_calibrations.h"
#include "sg_reuse.h"

namespace sg
{
   namespace dc
   {
      /**
       * @brief            Assigns RSPP detections to DC subsegments and calculates features
       *
       * @param[in, out]   dc_contours - list of contours
       * @param[in]        input_detections - RSPP detections
       * @param[in]        dist_rear_axle_to_vcs - distance between rear axle and the center of the front bumper (VCS origin)
       * @param[in]        calibrations
       **/
      void assign_detections_to_subsegments_and_update_features(DCContourStorage &dc_contours,
                                                                const rot::F360_Detection_Log_Output_T &rot_detections,
                                                                const SG_Input_Detections_T &input_detections,
                                                                const float dist_rear_axle_to_vcs,
                                                                const Drivability_Classification_Calibrations_T &cfg);
      /**
       * @brief            Calculates the squared distance between a point and a segment
       *
       * @param[in]        vertex1 - first vertex of the segment
       * @param[in]        vertex2 - second vertex of the segment
       * @param[in]        point
       * @return           float - sqaure of the distance
       **/
      float calculate_point_to_segment_dist_squared(const geometry::Point2D_T &vertex1,
                                                    const geometry::Point2D_T &vertex2,
                                                    const geometry::Point3D_T &point);

      /**
       * @brief            Gets indices of detections in the bounding box of a DC contour
       *
       * @param[out]       contour_det_indices - array of indices of detections in the bounding box of the DC contour
       * @param[in]        subsegments - list of subsegments of the DC contour
       * @param[in]        det_iso_positions - array of positions of detections (in ISO)
       * @param[in]        num_downselected_dets - number of valid stationary detections
       * @param[in]        max_contour_bb_radius - max distance between a segment and its bounding box
       * @return           num_contour_dets - number of dets in the bounding box of the DC contour
       **/
      uint8_t get_contour_bounding_box_det_indices(std::array<uint16_t, SG_MAX_NUM_INPUT_DETS> &contour_det_indices,
                                                   const DC_Contour_T::SubsegmentList &subsegments,
                                                   const std::array<geometry::Point3D_T, SG_MAX_NUM_INPUT_DETS> &det_iso_positions,
                                                   const std::size_t num_downselected_dets,
                                                   const float max_contour_bb_radius);

      /**
       * @brief            Uses sorting information from RSPP to create a sorted list of detections in ISO CS
       *
       * @param[out]       det_iso_positions - array of detection positions in ISO
       * @param[out]       sorted_det_indices - array of indices of sorted detections
       * @param[in]        input_detections - RSPP detections
       * @param[in]        dets_mask - mask indicating valid stationary detections
       * @param[in]        num_downselected_dets - number of valid stationary detections
       * @param[in]        dist_rear_axle_to_vcs - distance between rear axle and the center of the front bumper (VCS origin)
       **/
      void get_sorted_det_iso_coordinates(std::array<geometry::Point3D_T, SG_MAX_NUM_INPUT_DETS> &det_iso_positions,
                                          std::array<uint16_t, SG_MAX_NUM_INPUT_DETS> &sorted_det_indices,
                                          const sg::SG_Input_Detections_T &input_detections,
                                          const std::bitset<SG_MAX_NUM_INPUT_DETS> &dets_mask,
                                          const std::size_t num_downselected_dets,
                                          const float dist_rear_axle_to_vcs);

      /**
       * @brief            Gets indices of detections in the bounding box of a DC subsegment
       *
       * @param[out]       subsegment_det_indices - array of indices of detections in the bounding box of the subsegment
       * @param[in]        subsegment - subsegment of  a DC contour
       * @param[in]        det_iso_positions - array of positions of detections
       * @param[in]        contour_det_indices - array of indices of detections in the bounding box of the contour
       * @param[in]        num_contour_dets - number of contour detections
       * @param[in]        gate_half_width - half of the width of the gate
       * @return           num_segment_dets - number of dets in the bounding box of the segment
       **/
      uint8_t get_subsegment_det_indices(std::array<uint16_t, SG_MAX_NUM_INPUT_DETS> &subsegment_det_indices,
                                         const Subsegment_T &subsegment,
                                         const std::array<geometry::Point3D_T, SG_MAX_NUM_INPUT_DETS> &det_iso_positions,
                                         const std::array<uint16_t, SG_MAX_NUM_INPUT_DETS> &contour_det_indices,
                                         const uint8_t num_contour_dets,
                                         const float gate_half_width);

      /**
       * @brief            Returns a mask of valid stationary RSPP detections
       *
       * @param[out]       downselected_dets_mask - mask of valid stationary detections
       * @param[in]        input_detections - RSPP detections
       **/
      void get_valid_stationary_dets_mask(std::bitset<SG_MAX_NUM_INPUT_DETS> &downselected_dets_mask,
                                          const rot::F360_Detection_Log_Output_T &rot_detections,
                                          const sg::SG_Input_Detections_T &input_detections,
                                          const float min_rr_comp_of_det);

      /**
       * @brief            Returns a flag - if a detection is valid
       *
       * @param[in]        confid_azimuth - detection's azimuth confidence
       * @param[in]        confid_elevation - detection's elevation confidence
       * @param[in]        z_scs - detection's vertical position
       * @param[in]        cfg - calibrations
       * @return           bool - flag indicating if the detection is valid
       **/
      bool validate_detection(const int8_t confid_azimuth,
                              const int8_t confid_elevation,
                              const float z_scs,
                              const Drivability_Classification_Calibrations_T &cfg);
   }
}
#endif
