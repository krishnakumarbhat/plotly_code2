/*=============================================================================================*\
* FILE: sg_length_measurement_update.cpp
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definitions for length_measurement_update function and some minor utils functions.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#include "sg_length_measurement_update.h"

#include "geometry/geo_length.h"
#include "sg_math.h"
#include "sg_matrix_operations.h"
#include "sg_measurement_update_utils.h"

namespace sg
{
   static geometry::Segment2D_T get_segment_from_substate(const SubState &in_state)
   {
      geometry::Segment2D_T segment{};
      segment.first  = in_state.begin_it->position;
      segment.second = std::next(in_state.begin_it)->position;
      return segment;
   }

   void length_measurement_update(const std::array<ContourAssociatedDets, SG_MAX_NUM_CONTOURS> &dets_and_contours_data_for_length_measurement,
                                  const uint16_t num_contours,
                                  const Measurement_Update_Calibrations_T &calibrations,
                                  const float min_segment_length)
   {
      for (uint16_t idx = 0U; idx < num_contours; ++idx)
      {
         if (calibrations.min_dets_for_shrinking
             <= dets_and_contours_data_for_length_measurement[idx].first_segment_num_dets) // update first segment
         {
            const auto contour_it = dets_and_contours_data_for_length_measurement[idx].contour_it;
            SubState tmp_state{contour_it->vertices.begin(), 2U, 1U, SegmentPlacement::INVALID};
            const geometry::Segment2D_T segment = get_segment_from_substate(tmp_state);
            if (geometry::length(segment) > min_segment_length)
            {
               if (dets_and_contours_data_for_length_measurement[idx].contour_it->size() == 2U) // one segment contour
               {
                  const std::pair<Detection_T, Detection_T> pseudo_measurements = calc_pseudomeasurement(
                     dets_and_contours_data_for_length_measurement[idx].assoc_to_first_segment,
                     dets_and_contours_data_for_length_measurement[idx].first_segment_num_dets, segment, calibrations);

                  // Update first vertex
                  tmp_state.segment_flag = SegmentPlacement::FIRST;

                  const Matrix<float, 1U, 2U> H_first = calc_length_observation_matrix(tmp_state);
                  subcontour_kalman_update<2U>(tmp_state, H_first, pseudo_measurements.first, calibrations.length_forgetting_factor);

                  // Update last vertex
                  tmp_state.segment_flag = SegmentPlacement::LAST;

                  const Matrix<float, 1U, 2U> H_last = calc_length_observation_matrix(tmp_state);
                  subcontour_kalman_update<2U>(tmp_state, H_last, pseudo_measurements.second, calibrations.length_forgetting_factor);
               }
               else // multiple segments contour
               {
                  tmp_state.segment_flag = SegmentPlacement::FIRST;
                  const Detection_T pseudo_measurement =
                     calc_pseudomeasurement(dets_and_contours_data_for_length_measurement[idx].assoc_to_first_segment,
                                            dets_and_contours_data_for_length_measurement[idx].first_segment_num_dets, segment,
                                            tmp_state.segment_flag, calibrations);

                  // Update first vertex
                  const Matrix<float, 1U, 2U> H_first = calc_length_observation_matrix(tmp_state);
                  subcontour_kalman_update<2U>(tmp_state, H_first, pseudo_measurement, calibrations.length_forgetting_factor);
               }
            }
         }
         if (calibrations.min_dets_for_shrinking <= dets_and_contours_data_for_length_measurement[idx].last_segment_num_dets) // update
                                                                                                                              // last
                                                                                                                              // segment
         {
            const auto contour_it = dets_and_contours_data_for_length_measurement[idx].contour_it;
            const SubState tmp_state{std::prev(contour_it->vertices.end(), 2), 2U, 1U, SegmentPlacement::LAST};
            const geometry::Segment2D_T segment = get_segment_from_substate(tmp_state);
            if (geometry::length(segment) > min_segment_length)
            {
               const Detection_T pseudo_measurement =
                  calc_pseudomeasurement(dets_and_contours_data_for_length_measurement[idx].assoc_to_last_segment,
                                         dets_and_contours_data_for_length_measurement[idx].last_segment_num_dets, segment,
                                         tmp_state.segment_flag, calibrations);

               // Update last vertex
               const Matrix<float, 1U, 2U> H_last = calc_length_observation_matrix(tmp_state);
               subcontour_kalman_update<2U>(tmp_state, H_last, pseudo_measurement, calibrations.length_forgetting_factor);
            }
         }
      }
   }
}
