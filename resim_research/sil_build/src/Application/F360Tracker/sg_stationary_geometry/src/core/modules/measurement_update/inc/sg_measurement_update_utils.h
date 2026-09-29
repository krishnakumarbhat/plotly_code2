/*=============================================================================================*\
* FILE: sg_measurement_update_utils.h
* ====================================================================================
* Copyright (C) 2024 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
* -----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains declarations for measurement update utility functions
*
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*=============================================================================================*/

#ifndef SG_MEASUREMENT_UPDATE_UTILS_H
#define SG_MEASUREMENT_UPDATE_UTILS_H

#include "geometry/geo_point.h"
#include "geometry/geo_projection.h"
#include "geometry/geo_segment.h"
#include "sg_calibrations.h"
#include "sg_contour_storage.h"
#include "sg_detection.h"
#include "sg_matrix_operations.h"

namespace sg
{
   /**
    * @brief   Eneum defines place of the segment in the contour,
    *          invalid if it is not first or last segment
    **/
   enum class SegmentPlacement : std::uint8_t
   {
      INVALID = 0,
      FIRST,
      LAST
   };

   /**
    * @brief   Contains the begin iterator and size for the sub state of a contour,
    * @brief   i.e. the close neighborhood of vertices relevant for the measurement update.
    *
    * @param   N/A
    *
    * @return  N/A
    **/
   struct SubState
   {
      sg::Contour_T::VertexList::iterator begin_it;
      std::size_t size;
      std::size_t segment_idx;
      SegmentPlacement segment_flag;
   };

   /**
    * @brief   Structure contains information about detections associated
    * @brief   to the first and last segments of contour. Also contain iterator to this contour
    * @brief   and numer of associated detections.
    **/
   struct ContourAssociatedDets
   {
      std::array<const Detection_T *, SG_MAX_NUM_ENDING_SEGMENT_DETS> assoc_to_first_segment;
      std::array<const Detection_T *, SG_MAX_NUM_ENDING_SEGMENT_DETS> assoc_to_last_segment;
      ContourStorage::ContourList::iterator contour_it;
      uint8_t first_segment_num_dets;
      uint8_t last_segment_num_dets;
   };

   /**
    * @brief   Predicate function to use in filtering detections associated to contours.
    *
    * @param   det
    *
    * @return  true/false
    **/
   inline bool is_useable_as_measurement(const Detection_T &det)
   {
      return (det.contour_id != INVALID_CONTOUR_ID) && (!det.f_used_in_measurement_update)
             && ((det.segment_id[0] != INVALID_SEGMENT_ID) || (det.segment_id[1] != INVALID_SEGMENT_ID));
   }

   /**
    * @brief   Function gathers pointers of detections assigned
    * @brief   to the first and last segments of the contour.
    *
    * @param [in, out]  contour_endings
    * @param [in, out]  num_contours_to_length_measurement_update
    * @param [in]       contour_it
    * @param [in]       detection
    * @param [in]       segment_flag
    *
    * @return  N/A
    **/
   void collect_dets_assoc_to_first_and_last_segment(std::array<ContourAssociatedDets, SG_MAX_NUM_CONTOURS> &dets_assoc_to_contour_endings,
                                                     uint16_t &num_of_contours_with_assoc_ending_dets,
                                                     const ContourStorage::ContourList::iterator &contour_it,
                                                     const Detection_T &detection,
                                                     const SegmentPlacement &segment_flag);

   /**
    * @brief   Function calculates points array based on input data.
    *
    * @param [in]       segment
    * @param [in]       num_dets
    * @param [in]       dets
    *
    * @return           footpoints array
    **/
   std::array<sg::geometry::Footpoint_T, SG_MAX_NUM_ENDING_SEGMENT_DETS>
   calc_footpoints(const geometry::Segment2D_T &segment,
                   const uint16_t num_dets,
                   const std::array<const Detection_T *, SG_MAX_NUM_ENDING_SEGMENT_DETS> &dets);

   /**
    * @brief   Function returns H matrix regards to vertex position in segment.
    *
    * @param [in]       state
    *
    * @return           H matrix
    **/
   inline Matrix<float, 1U, 2U> calc_length_observation_matrix(const SubState &state)
   {
      Matrix<float, 1U, 2U> H{};

      if (state.segment_flag == SegmentPlacement::FIRST)
      {
         H[0U][0U] = 1.0F;
      }
      if (state.segment_flag == SegmentPlacement::LAST)
      {
         H[0U][1U] = 1.0F;
      }
      return H;
   }

   /**
    * @brief   Function calculates pseudomeasurements for first and last vertex in contour segment.
    *
    * @param [in]       associated_dets
    * @param [in]       num_associated_dets
    * @param [in]       segment
    * @param [in]       calibrations
    *
    * @return           pseudomeasurements
    **/
   std::pair<Detection_T, Detection_T>
   calc_pseudomeasurement(const std::array<const Detection_T *, SG_MAX_NUM_ENDING_SEGMENT_DETS> &associated_dets,
                          const uint16_t &num_associated_dets,
                          const geometry::Segment2D_T &segment,
                          const Measurement_Update_Calibrations_T &calibrations);

   /**
    * @brief   Function calculates pseudomeasurement for first or last vertex in contour
    *          regard to input parameter 'segment_place'.
    *
    * @param [in]       associated_dets
    * @param [in]       num_associated_dets
    * @param [in]       segment
    * @param [in]       segment_place
    * @param [in]       calibrations
    *
    * @return           pseudomeasurement
    **/
   Detection_T calc_pseudomeasurement(const std::array<const Detection_T *, SG_MAX_NUM_ENDING_SEGMENT_DETS> &associated_dets,
                                      const uint16_t &num_associated_dets,
                                      const geometry::Segment2D_T segment,
                                      const SegmentPlacement &segment_place,
                                      const Measurement_Update_Calibrations_T &calibrations);

   /**
    * @brief   Based on segment id extracts information about vertices related to this segment.
    *
    * @param [in] contour
    * @param [in] segment_id
    *
    * @return  iterator for first subsegment vertex and number of vertices related to this subsegment.
    **/
   SubState extract_subcontour_data(const Contour_T &contour, const std::size_t segment_id);

   /**
    * @brief   Calculates the measurement matrix for a single detection.
    *
    * @param [in] detection
    * @param [in] state
    *
    * @return  matrix H
    **/
   template <size_t num_states>
   Matrix<float, 1U, num_states> calc_single_det_measurement_matrix(const Detection_T &detection, const SubState &state)
   {
      Matrix<float, 1U, num_states> H{};

      if ((0U < state.segment_idx) && (1U < state.size))
      {
         const auto segment_offset = static_cast<std::ptrdiff_t>(state.segment_idx - 1U);
         auto state_it             = std::next(state.begin_it, segment_offset);

         geometry::Segment2D_T segment{};

         segment.first = geometry::Point2D_T(state_it->position.x, state_it->position.y);
         state_it++;
         segment.second = geometry::Point2D_T(state_it->position.x, state_it->position.y);

         const geometry::Point2D_T point{detection.position.x, detection.position.y};
         const bool f_limit_footpoint = true;
         const auto footpoint         = geometry::make_projection(segment, point, f_limit_footpoint);

         H[0U][state.segment_idx - 1U] = 1.0F - footpoint.s;
         H[0U][state.segment_idx]      = footpoint.s;
      }
      return H;
   }

   /**
    * @brief   Regularizes the contour shape so that the angles between segments are reduced
    *
    * @param [in] contour_sub_state
    * @param [in] position_forgetting_factor
    * @param [in] regularity_weight
    * @param [in] regularity_threshold
    *
    * @return  N/A
    **/
   void regularize_subcontour(const SubState &contour_sub_state,
                              const float position_forgetting_factor,
                              const float regularity_weight,
                              const float regularity_threshold);

   /**
    * @brief   Parses structured data to matrix.
    *
    * @param   SubState contour_sub_state
    *
    * @return  Matrix<float, size, size>
    **/
   template <size_t num_states>
   Matrix<float, num_states, num_states> covariance_struct_to_matrix(const SubState &contour_sub_state)
   {
      Matrix<float, num_states, num_states> P{};
      auto sub_state_it = contour_sub_state.begin_it;

      ++sub_state_it;
      for (size_t i = 1U; i < contour_sub_state.size - 1U; ++i)
      {
         P[i][i]      = sub_state_it->pos_cov.x;
         P[i][i + 1U] = sub_state_it->pos_cross_cov.x1x2;
         --sub_state_it;
         P[i][i - 1U] = sub_state_it->pos_cross_cov.x1x2;
         ++sub_state_it;
         ++sub_state_it;
      }
      (void) sub_state_it; // MISRA

      sub_state_it = contour_sub_state.begin_it;
      P[0U][0U]    = sub_state_it->pos_cov.x;
      P[0U][1U]    = sub_state_it->pos_cross_cov.x1x2;
      sub_state_it = std::next(sub_state_it, static_cast<std::ptrdiff_t>(contour_sub_state.size - 2U));
      P[contour_sub_state.size - 1U][contour_sub_state.size - 2U] = sub_state_it->pos_cross_cov.x1x2;
      ++sub_state_it;
      P[contour_sub_state.size - 1U][contour_sub_state.size - 1U] = sub_state_it->pos_cov.x;

      return P;
   }

   /**
    * @brief   Updates position covariance and position cross covariance.
    *
    * @param [in] variance
    * @param [in] contour_sub_state
    *
    * @return  N/A
    **/
   template <size_t num_states>
   void update_variance(const Matrix<float, num_states, num_states> &new_variance, const SubState &contour_sub_state)
   {
      auto sub_state_it = contour_sub_state.begin_it;

      for (std::size_t i = 0U; i < contour_sub_state.size - 1U; ++i)
      {
         sub_state_it->pos_cov.x          = new_variance[i][i];
         sub_state_it->pos_cov.y          = new_variance[i][i];
         sub_state_it->pos_cross_cov.x1x2 = new_variance[i][i + 1U];
         sub_state_it->pos_cross_cov.y1y2 = new_variance[i + 1U][i];
         ++sub_state_it;
      }

      sub_state_it = contour_sub_state.begin_it;
      sub_state_it = std::next(sub_state_it, static_cast<std::ptrdiff_t>(contour_sub_state.size - 1U));

      sub_state_it->pos_cov.x = new_variance[contour_sub_state.size - 1U][contour_sub_state.size - 1U];
      sub_state_it->pos_cov.y = new_variance[contour_sub_state.size - 1U][contour_sub_state.size - 1U];

      (void) sub_state_it; // MISRA
   }

   /**
    * @brief   Updates vertices position in contour.
    *
    * @param [in] position
    * @param [in, out] contour_sub_state
    *
    * @return  N/A
    **/
   template <size_t num_states>
   void update_position(const Matrix<float, num_states, 2U> &new_position, const SubState &contour_sub_state)
   {
      auto sub_state_it = contour_sub_state.begin_it;

      for (auto idx = 0U; idx < contour_sub_state.size; ++idx)
      {
         sub_state_it->position.y = new_position[idx][0U];
         sub_state_it->position.x = new_position[idx][1U];

         ++sub_state_it;
      }
      (void) sub_state_it; // MISRA
   }

   /**
    * @brief   Creates prestate vector from set of vertices(contour_sub_state)
    *
    * @param [in] contour_sub_state
    *
    * @return  Matrix<float, num_states, 1U> pre_state
    **/
   template <size_t num_states>
   Matrix<float, num_states, 2U> create_pre_state(const SubState &contour_sub_state)
   {
      Matrix<float, num_states, 2U> pre_state{};
      auto sub_state_it = contour_sub_state.begin_it;

      for (auto idx = 0U; idx < contour_sub_state.size; ++idx)
      {
         pre_state[idx][0U] = sub_state_it->position.y;
         pre_state[idx][1U] = sub_state_it->position.x;
         ++sub_state_it;
      }
      (void) sub_state_it; // MISRA
      return pre_state;
   }

   /**
    * @brief   Calculates the kalman update.
    *
    * @param [in, out] state
    * @param [in, out] P: state estimate covariance matrix
    * @param [in]      H: observation matrix
    * @param [in]      R: measurement noise covariance matrix
    * @param [in]      measurement
    * @param [in]      position_forgetting_factor
    *
    * @return  N/A
    **/
   template <size_t num_states, size_t num_measurements>
   void kalman_update(Matrix<float, num_states, 2U> &state,
                      Matrix<float, num_states, num_states> &P,
                      const Matrix<float, 1U, num_states> &H,
                      const float &R,
                      const Matrix<float, 1U, num_measurements> &measurement,
                      const float position_forgetting_factor)
   {
      const auto H_transposed = transpose(H);

      // Kalman gain
      const Matrix<float, num_states, 1U> K = P * H_transposed * inverse_matrix(H * P * H_transposed + R * position_forgetting_factor);

      // Innovation
      const Matrix<float, 1U, 2U> e = measurement - H * state;

      // State update
      state = state + K * e;

      const auto I = eye<float, num_states>();

      // post_variance
      const auto IKH = (I - K * H);
      P              = IKH * (P / position_forgetting_factor) * transpose(IKH) + K * R * transpose(K); // Joseph form
   }

   /**
    * @brief   Calculates measurement update.
    *
    * @param [in, out] contour_sub_state
    * @param [in] H:   observation matrix
    * @param [in]      detection
    * @param [in]      position_forgetting_factor
    *
    * @return  N/A
    **/
   template <size_t num_states>
   void subcontour_kalman_update(const SubState &contour_sub_state,
                                 const Matrix<float, 1U, num_states> &H,
                                 const Detection_T &detection,
                                 const float position_forgetting_factor)
   {
      const float R = detection.position_cov.x;

      const Matrix<float, 1U, 2U> measurement{{{detection.position.y, detection.position.x}}};

      Matrix<float, num_states, num_states> P = covariance_struct_to_matrix<num_states>(contour_sub_state);

      // Prepare pre_state
      Matrix<float, num_states, 2U> state = create_pre_state<num_states>(contour_sub_state);

      kalman_update<num_states, 2U>(state, P, H, R, measurement, position_forgetting_factor);

      // Update contour - positions
      update_position(state, contour_sub_state);

      // Update contour - variances
      update_variance(P, contour_sub_state);
   }

   /**
    * @brief   Calculates the regularity measure, i.e. how straight a contour's substate is
    *
    * @param   iterator to a vertex
    *
    * @return  regularity measure
    **/
   float calc_regularity(const sg::Contour_T::VertexList::iterator &vertex_it);
}
#endif
