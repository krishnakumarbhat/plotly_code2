/*=============================================================================================*\
* FILE: sg_position_measurement_update.cpp
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains definition for position_measurement_update function.
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#include "sg_position_measurement_update.h"

#include <functional>

#include "container_view.h"
#include "sg_matrix_operations.h"
#include "sg_measurement_update_utils.h"

namespace sg
{
   void position_measurement_update(DetectionStorage &detections,
                                    ContourStorage &contours,
                                    std::array<ContourAssociatedDets, SG_MAX_NUM_CONTOURS> &dets_and_contours_data_for_length_measurement,
                                    uint16_t &num_contours_to_length_measurement_update,
                                    const float position_forgetting_factor,
                                    const float regularity_weight,
                                    const float regularity_threshold)
   {
      ContainerView<DetectionList, std::function<decltype(is_useable_as_measurement)> > dets_associated_to_segment(
         detections.m_detections, is_useable_as_measurement);

      const auto contours_end_it   = contours.end();
      const auto contours_begin_it = contours.begin();
      (void) contours_end_it;   // MISRA
      (void) contours_begin_it; // MISRA

      for (auto &detection : dets_associated_to_segment)
      {
         for (const auto &segment_id : detection.segment_id)
         {
            if (segment_id != INVALID_SEGMENT_ID)
            {
               const auto contour_it = std::find_if(contours_begin_it, contours_end_it,
                                                    [&detection](const Contour_T &contour)
                                                    { return contour.unique_id() == detection.contour_id; });

               if (contour_it != contours_end_it)
               {
                  const auto sub_state = extract_subcontour_data(*contour_it, segment_id);

                  if (sub_state.segment_flag != SegmentPlacement::INVALID)
                  {
                     collect_dets_assoc_to_first_and_last_segment(dets_and_contours_data_for_length_measurement,
                                                                  num_contours_to_length_measurement_update, contour_it, detection,
                                                                  sub_state.segment_flag);
                  }

                  if (sub_state.size == 2U)
                  {
                     const Matrix<float, 1U, 2U> H = calc_single_det_measurement_matrix<2U>(detection, sub_state);
                     subcontour_kalman_update<2U>(sub_state, H, detection, position_forgetting_factor);
                  }
                  else if (sub_state.size == 3U)
                  {
                     const Matrix<float, 1U, 3U> H = calc_single_det_measurement_matrix<3U>(detection, sub_state);
                     subcontour_kalman_update<3U>(sub_state, H, detection, position_forgetting_factor);
                     if (regularity_weight > SG_EPSILON)
                     {
                        regularize_subcontour(sub_state, position_forgetting_factor, regularity_weight, regularity_threshold);
                     }
                  }
                  else if (sub_state.size == 4U)
                  {
                     const Matrix<float, 1U, 4U> H = calc_single_det_measurement_matrix<4U>(detection, sub_state);
                     subcontour_kalman_update<4U>(sub_state, H, detection, position_forgetting_factor);
                     if (regularity_weight > SG_EPSILON)
                     {
                        regularize_subcontour(sub_state, position_forgetting_factor, regularity_weight, regularity_threshold);
                     }
                  }
                  else
                  {
                     assert(false);
                  }

                  detection.f_used_in_measurement_update = true;
               }
            }
         }
      }
   }
}
