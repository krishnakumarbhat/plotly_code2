/*===================================================================================*\
* FILE: f360_msmt_update_support_functions_common.cpp
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains common functions for CCA and CTCA methods
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_msmt_update_support_functions_common.h"
#include "f360_math.h"
#include "f360_math_func.h"
#include "f360_occlusion_types.h"
#include <algorithm>

namespace f360_variant_A
{
/*===========================================================================*\
* FUNCTION: Select_Dets_For_RR_Update()
*===========================================================================
* RETURN VALUE:
* None
*
* PARAMETERS:
* const F360_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS]
* const uint32_t& k_min_num_selected_dets_per_sensor_for_binning
* F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS]
* F360_Object_Track_T& object_track
* uint32_t (&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK]
* uint32_t& selected_dets_num
* uint32_t(&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT]
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* This function updates list of detections that are valid to be used in the
* range-rate measurement update. It also calculates how many detections there
* are in each bin.
\*===========================================================================*/
void Select_Dets_For_RR_Update(const F360_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
                               const uint32_t &k_min_num_selected_dets_per_sensor_for_binning,
                               F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
                               const F360_Object_Track_T &object_track, uint32_t (&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
                               uint32_t &selected_dets_num, uint32_t (&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT])
{
   selected_dets_num = 0U;

   // Loop over associated detections and select valid ones for measurement update.
   for (uint32_t i_det = 0U; i_det < object_track.ndets; i_det++)
   {
      const uint32_t det_idx = object_track.detids[i_det] - 1U;

      if (det_props[det_idx].f_rr_inlier)
      {
         selected_dets_idx[selected_dets_num] = det_idx;
         selected_dets_num++;
      }
   }

   // Rough initial check if object is eligible for detection binning to reduce selected_dets_num
   if ((object_track.movable_prob > MOVABLE_PROB_THRESHOLD) &&
       (selected_dets_num > k_min_num_selected_dets_per_sensor_for_binning))
   {
      Try_To_Decrease_Num_Selected_Dets(detections, det_props, k_min_num_selected_dets_per_sensor_for_binning, selected_dets_idx,
                                        selected_dets_num, selected_dets_score);
   }

   // Logkeeping of which dets were selected for the measurement update
   for (uint32_t i_det = 0U; i_det < selected_dets_num; i_det++)
   {
      const uint32_t det_idx                      = selected_dets_idx[i_det];
      det_props[det_idx].f_used_in_rr_msmt_update = true;
   }
}

/*===========================================================================*\
* FUNCTION: Try_To_Decrease_Num_Selected_Dets()
*===========================================================================
* RETURN VALUE:
* None
*
* PARAMETERS:
* const F360_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS]
* const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
* const int32_t& k_min_num_selected_dets_per_sensor_for_binning
* uint32_t (&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK]
* uint32_t& selected_dets_num
* uint32_t(&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT]
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Binning is done over detection azimuth only (not range or position). The reason for
* this is that the measured range rate from a point on an object is correlated only with
* detection azimuth and not range (even for a yawing object for which different points
* of the object has different velocities). This is because radar can only measure the radial
* component of the velocity so the cross radial velocity differenece due to yawing can not be measured.
\*===========================================================================*/
void Try_To_Decrease_Num_Selected_Dets(const F360_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
                                       const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
                                       const uint32_t &k_min_num_selected_dets_per_sensor_for_binning,
                                       uint32_t (&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK], uint32_t &selected_dets_num,
                                       uint32_t (&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT])
{
   uint32_t selected_dets_idx_final[MAX_DETS_IN_OBJ_TRK];
   uint32_t selected_dets_num_final = 0U;

   float32_t max_det_az_per_sensor[MAX_NUMBER_OF_SENSORS];
   float32_t min_det_az_per_sensor[MAX_NUMBER_OF_SENSORS];
   uint32_t det_idx_per_sensor[MAX_NUMBER_OF_SENSORS][MAX_DETS_IN_OBJ_TRK];
   uint32_t num_det_per_sensor[MAX_NUMBER_OF_SENSORS];
   Extract_Detection_Properties_Per_Sensor(detections, selected_dets_idx, selected_dets_num, min_det_az_per_sensor,
                                           max_det_az_per_sensor, det_idx_per_sensor, num_det_per_sensor);

   for (uint8_t i_sensor = 0U; i_sensor < MAX_NUMBER_OF_SENSORS; i_sensor++)
   {
      if (num_det_per_sensor[i_sensor] > k_min_num_selected_dets_per_sensor_for_binning)
      {
         // Enough detections from this sensor -> can bin detections
         uint32_t bin_array_det_idx[MSMT_UPDATE_NUM_BINS_PER_SENSOR][MAX_DETS_IN_OBJ_TRK];
         uint32_t bin_array_num_det_in_bin[MSMT_UPDATE_NUM_BINS_PER_SENSOR];
         Bin_Detections_By_Azimuth(detections, det_idx_per_sensor[i_sensor], num_det_per_sensor[i_sensor],
                                   min_det_az_per_sensor[i_sensor], max_det_az_per_sensor[i_sensor], bin_array_det_idx,
                                   bin_array_num_det_in_bin);

         // Compute representative detection for each bin and add to the selected dets
         Select_Median_Range_Rate_Detection_For_Each_Bin(det_props, bin_array_det_idx, bin_array_num_det_in_bin,
                                                         selected_dets_idx_final, selected_dets_num_final, selected_dets_score);
      }
      else
      {
         // Not enough detections from this sensor -> keep these detections in selected dets
         for (uint32_t i_det = 0U; i_det < num_det_per_sensor[i_sensor]; i_det++)
         {
            selected_dets_idx_final[selected_dets_num_final] = det_idx_per_sensor[i_sensor][i_det];
            selected_dets_num_final++;
         }
      }
   }
   // Save final detections to output structure
   selected_dets_num = selected_dets_num_final;
   (void)std::copy(cmn::begin(selected_dets_idx_final), cmn::end(selected_dets_idx_final), cmn::begin(selected_dets_idx));
}

/*===========================================================================*\
 * FUNCTION: Extract_Detection_Properties_Per_Sensor
 *===========================================================================
 * RETURN VALUE:
 * None
 *
 * PARAMETERS:
 * const F360_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
 * const uint32_t(&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
 * const uint32_t& selected_dets_num,
 * float32_t(&min_det_az_per_sensor)[MAX_NUMBER_OF_SENSORS],
 * float32_t(&max_det_az_per_sensor)[MAX_NUMBER_OF_SENSORS],
 * uint32_t(&det_idx_per_sensor)[MAX_NUMBER_OF_SENSORS][MAX_DETS_IN_OBJ_TRK],
 * uint8_t(&num_det_per_sensor)[MAX_NUMBER_OF_SENSORS])
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * For the selected detections, separates these based on their sensor id and
 * extracts the min and max azimuth per sensor.
\*===========================================================================*/
void Extract_Detection_Properties_Per_Sensor(const F360_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
                                             const uint32_t (&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
                                             const uint32_t &selected_dets_num,
                                             float32_t (&min_det_az_per_sensor)[MAX_NUMBER_OF_SENSORS],
                                             float32_t (&max_det_az_per_sensor)[MAX_NUMBER_OF_SENSORS],
                                             uint32_t (&det_idx_per_sensor)[MAX_NUMBER_OF_SENSORS][MAX_DETS_IN_OBJ_TRK],
                                             uint32_t (&num_det_per_sensor)[MAX_NUMBER_OF_SENSORS])
{
   std::fill(cmn::begin(num_det_per_sensor), cmn::end(num_det_per_sensor), 0U);
   std::fill(cmn::begin(max_det_az_per_sensor), cmn::end(max_det_az_per_sensor), -F360_2PI);
   std::fill(cmn::begin(min_det_az_per_sensor), cmn::end(min_det_az_per_sensor), F360_2PI);

   for (uint32_t i = 0U; i < selected_dets_num; i++)
   {
      const uint32_t det_idx   = selected_dets_idx[i];
      const int32_t sensor_idx = detections[det_idx].sensor_id - 1;

      if (detections[det_idx].azimuth > max_det_az_per_sensor[sensor_idx])
      {
         max_det_az_per_sensor[sensor_idx] = detections[det_idx].azimuth;
      }
      if (detections[det_idx].azimuth < min_det_az_per_sensor[sensor_idx])
      {
         min_det_az_per_sensor[sensor_idx] = detections[det_idx].azimuth;
      }

      det_idx_per_sensor[sensor_idx][num_det_per_sensor[sensor_idx]] = det_idx;
      num_det_per_sensor[sensor_idx]++;
   }
}

/*===========================================================================*\
 * FUNCTION: Bin_Detections_By_Azimuth
 *===========================================================================
 * RETURN VALUE:
 * None
 *
 * PARAMETERS:
 * const F360_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
 * const uint32_t(&det_idx_for_sensor)[MAX_DETS_IN_OBJ_TRK],
 * const uint8_t num_det_for_sensor,
 * const float32_t min_az_for_sensor,
 * const float32_t max_az_for_sensor,
 * uint32_t(&bin_array_det_idx)[MSMT_UPDATE_NUM_BINS_PER_SENSOR][MAX_DETS_IN_OBJ_TRK],
 * uint32_t(&bin_array_num_det_in_bin)[MSMT_UPDATE_NUM_BINS_PER_SENSOR])
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * Binns detections by azimuth using a set number of bins with a varying bin
 * width that is determined by the delta between the min and max azimuth.
 *
 * PRECONDITIONS:
 * min_az_for_sensor must be < max_az_for_sensor
 *
\*===========================================================================*/
void Bin_Detections_By_Azimuth(const F360_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
                               const uint32_t (&det_idx_for_sensor)[MAX_DETS_IN_OBJ_TRK], const uint32_t num_det_for_sensor,
                               const float32_t min_az_for_sensor, const float32_t max_az_for_sensor,
                               uint32_t (&bin_array_det_idx)[MSMT_UPDATE_NUM_BINS_PER_SENSOR][MAX_DETS_IN_OBJ_TRK],
                               uint32_t (&bin_array_num_det_in_bin)[MSMT_UPDATE_NUM_BINS_PER_SENSOR])
{
   std::fill(cmn::begin(bin_array_num_det_in_bin), cmn::end(bin_array_num_det_in_bin), 0U);

   const float32_t azimuth_spread                          = max_az_for_sensor - min_az_for_sensor;
   constexpr float32_t INV_MSMT_UPDATE_NUM_BINS_PER_SENSOR = 1.0F / static_cast<float32_t>(MSMT_UPDATE_NUM_BINS_PER_SENSOR);
   const float32_t bin_azimuth_width =
      std::max(azimuth_spread * INV_MSMT_UPDATE_NUM_BINS_PER_SENSOR,
               azimuth_spread * INV_MSMT_UPDATE_NUM_BINS_PER_SENSOR + F360_EPSILON); // 0-division protection

   for (uint32_t i_det = 0U; i_det < num_det_for_sensor; i_det++)
   {
      const uint32_t det_idx      = det_idx_for_sensor[i_det];
      const float32_t det_azimuth = detections[det_idx].azimuth;

      // Compute in which bin the detection fit
      const float32_t bin_idx_float = (det_azimuth - min_az_for_sensor) / (bin_azimuth_width);

      // To account for precision errors in a safe way when det_azimuth = min_az or max_az.
      // Note that the static_cast is intentionally intended to also have a truncating effect similar to a floorf() function.
      // Also note that bin_idx_clamped is intentionally signed such that if bin_idx_float is -0.00.., it won't overflow
      const int32_t bin_idx_clamped = Clamp(static_cast<int32_t>(bin_idx_float), 0, MSMT_UPDATE_NUM_BINS_PER_SENSOR - 1);

      // Assign detection to bin
      bin_array_det_idx[bin_idx_clamped][bin_array_num_det_in_bin[bin_idx_clamped]] = det_idx_for_sensor[i_det];
      bin_array_num_det_in_bin[bin_idx_clamped]++;
   }
}

/*===========================================================================*\
 * FUNCTION: Select_Median_Range_Rate_Detection_For_Each_Bin
 *===========================================================================
 * RETURN VALUE:
 * None
 *
 * PARAMETERS:
 * const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
 * const uint32_t(&bin_array_det_idx)[MSMT_UPDATE_NUM_BINS_PER_SENSOR][MAX_DETS_IN_OBJ_TRK],
 * const uint32_t(&bin_array_num_det_in_bin)[MSMT_UPDATE_NUM_BINS_PER_SENSOR],
 * uint32_t(&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
 * uint32_t& selected_dets_num)
 * uint32_t(&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT]
 * --------------------------------------------------------------------------
 * ABSTRACT:
 * --------------------------------------------------------------------------
 * This function picks the detection with median range rate for each bin and gives the score
 * that is based on the amount of detections in each bin. If a bin slot is empty,
 * nothing happens. The selected detection has the median value of range rate to minimize
 * the impact of the noise.
\*===========================================================================*/
void Select_Median_Range_Rate_Detection_For_Each_Bin(
   const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
   const uint32_t (&bin_array_det_idx)[MSMT_UPDATE_NUM_BINS_PER_SENSOR][MAX_DETS_IN_OBJ_TRK],
   const uint32_t (&bin_array_num_det_in_bin)[MSMT_UPDATE_NUM_BINS_PER_SENSOR], uint32_t (&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
   uint32_t &selected_dets_num, uint32_t (&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT])
{
   for (int8_t i_bin = 0; i_bin < MSMT_UPDATE_NUM_BINS_PER_SENSOR; i_bin++)
   {
      const uint32_t num_dets_in_bin = bin_array_num_det_in_bin[i_bin];

      if (num_dets_in_bin > 0U)
      {
         // Pick detection with median range rate
         uint32_t median_det_idx = 0U;
         if (num_dets_in_bin == 1U)
         {
            // Choose this single detection
            median_det_idx = bin_array_det_idx[i_bin][0];
         }
         else
         {
            // Extract range rate for detections
            float32_t det_rr_array[MAX_DETS_IN_OBJ_TRK];
            for (uint32_t i_det = 0U; i_det < num_dets_in_bin; i_det++)
            {
               det_rr_array[i_det] = det_props[bin_array_det_idx[i_bin][i_det]].range_rate_compensated;
            }

            uint32_t perm[MAX_DETS_IN_OBJ_TRK];
            (void)F360_Sort(num_dets_in_bin, true, det_rr_array, perm); // det_rr_array is now sorted in ascending order

            const uint32_t median_idx = (num_dets_in_bin - 1U) / 2U;
            median_det_idx            = bin_array_det_idx[i_bin][perm[median_idx]];
         }
         selected_dets_idx[selected_dets_num]   = median_det_idx;
         selected_dets_score[selected_dets_num] = bin_array_num_det_in_bin[i_bin];
         selected_dets_num++;
      }
   }
}

/*===========================================================================*\
* FUNCTION: Is_Occluded_Oncoming_High_Speed()
*===========================================================================
* RETURN VALUE:
* bool - true if the object meets the criteria for occluded oncoming high speed
*
* PARAMETERS:
* const F360_Calibrations_T& calib - calibration structure
* const F360_Object_Track_T& object_track - single object track
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* This function checks if an object is occluded, oncoming, and has high speed.
* Used by Decrease_Kmat_For_Occluded functions for both CTCA and CCA models.
*
\*===========================================================================*/
bool Is_Occluded_Oncoming_High_Speed(const F360_Calibrations_T &calib, const F360_Object_Track_T &object_track)
{
   const bool is_high_speed = object_track.speed > calib.k_speed_threshold_for_kmat_decrease;
   const bool is_ahead      = object_track.vcs_position.x > calib.k_x_pos_threshold_for_kmat_decrease;
   const bool is_oncoming =
      std::abs((std::abs(object_track.vcs_heading.Value()) - F360_PI)) < calib.k_heading_threshold_for_kmat_decrease;
   const bool is_occluded =
      (object_track.occlusion_status == OCCLUSION_STATUS_OCCLUDED) || (object_track.occlusion_status == OCCLUSION_STATUS_ON_EDGE);
   const bool is_not_behind_sep = object_track.behind_sep_id == F360_INVALID_UNSIGNED_ID;
   return is_high_speed && is_ahead && is_oncoming && is_occluded && is_not_behind_sep;
}

/*===========================================================================*\
* FUNCTION: Kmat_Times_Rmat()
*===========================================================================
* RETURN VALUE:
* None
*
* PARAMETERS:
* const float32_t(&Kmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT] - the K matrix (Kalman gain)
* const float32_t(&Rmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] - the R matrix (measurement error covariance)
* const uint32_t num_measurements - total number of measurements
* float32_t(&KRmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT] - the resulting matrix multiplication of Kmat * Rmat
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* This function computes the matrix multiplication of Kmat * Rmat in the KF measurement update step.
* It utilizes that Rmat has the following structure in order to optimize the computations with respect
* to run time:
* Rmat = [* * 0 0 0 0;
*         * * 0 0 0 0;
*         0 0 * 0 0 0; (this row, where the * value is located at the matrix diagonal, is repeated for each available range rate
measurement)
*         ....]
*
\*===========================================================================*/
void Kmat_Times_Rmat(const float32_t (&Kmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT],
                     const float32_t (&Rmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT],
                     const uint32_t num_measurements, float32_t (&KRmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT])
{
   // Compute first two columns of result mat as [KR]_i0/1 = sum_l k_il*r_l0/1 where we utilize that h_l0/1 = 0 for all l > 1
   for (uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx++)
   {
      for (uint32_t col_idx = 0U; col_idx < 2U; col_idx++)
      {
         KRmat[row_idx][col_idx] = Kmat[row_idx][0] * Rmat[0][col_idx] + Kmat[row_idx][1] * Rmat[1][col_idx];
      }
   }

   // Compute remaining columns of result mat as [KR]_ij = sum_l k_il*r_lj where we utilize that r_lj = 0 for all l != j
   for (uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx++)
   {
      for (uint32_t col_idx = 2U; col_idx < num_measurements; col_idx++)
      {
         KRmat[row_idx][col_idx] = Kmat[row_idx][col_idx] * Rmat[col_idx][col_idx];
      }
   }
}

/*===========================================================================*\
* FUNCTION: Error_Cov_Update_Joseph_Form()
*===========================================================================
* RETURN VALUE:
* None
*
* PARAMETERS:
* float32_t (&ikh_mat)[STATE_DIMENSION][STATE_DIMENSION]   - negative K*H matrix, will be modified to I - K*H
* const float32_t (&k_mat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT]      - kalman gain
* const float32_t (&r_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT]             - covariance of measurement
uncertainty
* const uint32_t nr_total_msnmts                            - number of valid measurements
* float32_t (&p_mat)[STATE_DIMENSION][STATE_DIMENSION]      - state error covariance matrix, updated in this function
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* This function updates an object's error covariance matrix using the Joseph form:
* P_update = (I - KH) * P * (I - KH)' + KRK'
*
* The caller must compute -K*H into ikh_mat using the model-specific Negative_Kmat_Times_Hmat function
* before calling this function. This function then adds the identity matrix to obtain (I - KH) and
* completes the Joseph form update.
*
\*===========================================================================*/
void Error_Cov_Update_Joseph_Form(float32_t (&ikh_mat)[STATE_DIMENSION][STATE_DIMENSION],
                                  const float32_t (&k_mat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT],
                                  const float32_t (&r_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT],
                                  const uint32_t nr_total_msnmts, float32_t (&p_mat)[STATE_DIMENSION][STATE_DIMENSION])
{
   // Add identity matrix to -K*H to get I - K*H
   for (uint32_t loop_index = 0U; loop_index < STATE_DIMENSION; loop_index++)
   {
      ikh_mat[loop_index][loop_index] += 1.0F;
   }

   // KF equation: (I - (K * H)) * P
   float32_t temp3_mat[STATE_DIMENSION][STATE_DIMENSION] = {};
   F360_Matmul_6x6_6x6(ikh_mat, p_mat, temp3_mat);

   // KF equation: ((I - (K * H)) * P) * (I - K * H)'
   float32_t temp4_mat[STATE_DIMENSION][STATE_DIMENSION];
   F360_Matmul_6x6_6x6_Transpose_Symmetric(temp3_mat, ikh_mat, temp4_mat);

   // KF equation: K * R
   float32_t temp0_mat[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT];
   Kmat_Times_Rmat(k_mat, r_mat, nr_total_msnmts, temp0_mat);

   // KF equation: K * R * K'
   float32_t temp5_mat[STATE_DIMENSION][STATE_DIMENSION];
   F360_Matmul_MxN_MxN_Transpose_Symmetric(temp0_mat, k_mat, temp5_mat, nr_total_msnmts);

   //  KF equation: ((I - (K * H)) * P) * (I - K * H)'  +  K * R * K'
   F360_Matadd_6x6_6x6(temp4_mat, temp5_mat, p_mat);
}

/*===========================================================================*\
* FUNCTION: Add_Rmat_To_Smat()
*===========================================================================
* RETURN VALUE:
* None
*
* PARAMETERS:
* const float32_t (&r_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] - measurement uncertainty covariance matrix
* const uint32_t nr_total_msnmts - total number of measurements
* float32_t (&s_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] - innovation covariance matrix, updated in this
function
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* This function adds the measurement uncertainty covariance matrix (R) to the
* innovation covariance matrix (S). The operation exploits the structure of R:
* the first two rows/columns contain cross-covariances between measurements,
* while remaining elements are diagonal variance terms for range rate measurements.
*
\*===========================================================================*/
void Add_Rmat_To_Smat(const float32_t (&r_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT],
                      const uint32_t nr_total_msnmts, float32_t (&s_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT])
{
   s_mat[0][0] += r_mat[0][0];
   s_mat[0][1] += r_mat[0][1];
   s_mat[1][0] = s_mat[0][1];
   s_mat[1][1] += r_mat[1][1];
   for (uint32_t loop_index_k = 2U; loop_index_k < nr_total_msnmts; loop_index_k++)
   {
      s_mat[loop_index_k][loop_index_k] += r_mat[loop_index_k][loop_index_k];
   }
}

/*===========================================================================*\
* FUNCTION: Update_Obj_Pos_And_Bbox_Pos()
*===========================================================================
* RETURN VALUE:
* None
*
* PARAMETERS:
* const float32_t new_pos_x
* const float32_t new_pos_y
* F360_Object_Track_T& object_track
*
* --------------------------------------------------------------------------
* ABSTRACT:
* --------------------------------------------------------------------------
* Updates object position and translates bbox with the same position delta.
\*===========================================================================*/
void Update_Obj_Pos_And_Bbox_Pos(const float32_t new_pos_x, const float32_t new_pos_y, F360_Object_Track_T &object_track)
{
   const float32_t delta_pos_x = new_pos_x - object_track.vcs_position.x;
   const float32_t delta_pos_y = new_pos_y - object_track.vcs_position.y;

   object_track.vcs_position.x = new_pos_x;
   object_track.vcs_position.y = new_pos_y;
   object_track.bbox.Translate(delta_pos_x, delta_pos_y);
}
} // namespace f360_variant_A
