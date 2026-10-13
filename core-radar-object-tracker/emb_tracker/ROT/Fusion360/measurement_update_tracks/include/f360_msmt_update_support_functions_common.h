/*===================================================================================*\
* FILE: f360_msmt_update_support_functions_common.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*    This file contains supporting functions declarations for f360_measurmant_update_tracks
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_MSMT_UPDATE_SUPPORT_FUNCTIONS_COMMON_H
#define F360_MSMT_UPDATE_SUPPORT_FUNCTIONS_COMMON_H

#include "f360_calibrations.h"
#include "f360_detection.h"
#include "f360_detection_props.h"
#include "f360_object_track.h"
#include "f360_pseudo_msmt.h"
#include "f360_reuse.h"
#include "f360_trk_fltr_cca_states.h"
#include "f360_trk_fltr_ctca_states.h"

namespace f360_variant_A
{
void Select_Dets_For_RR_Update(const F360_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
                               const uint32_t &k_min_num_selected_dets_per_sensor_for_binning,
                               F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
                               const F360_Object_Track_T &object_track, uint32_t (&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
                               uint32_t &selected_dets_num, uint32_t (&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT]);

void Extract_Detection_Properties_Per_Sensor(const F360_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
                                             const uint32_t (&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
                                             const uint32_t &selected_dets_num,
                                             float32_t (&min_det_az_per_sensor)[MAX_NUMBER_OF_SENSORS],
                                             float32_t (&max_det_az_per_sensor)[MAX_NUMBER_OF_SENSORS],
                                             uint32_t (&det_idx_per_sensor)[MAX_NUMBER_OF_SENSORS][MAX_DETS_IN_OBJ_TRK],
                                             uint32_t (&num_det_per_sensor)[MAX_NUMBER_OF_SENSORS]);

void Bin_Detections_By_Azimuth(const F360_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
                               const uint32_t (&det_idx_for_sensor)[MAX_DETS_IN_OBJ_TRK], const uint32_t num_det_for_sensor,
                               const float32_t min_az_for_sensor, const float32_t max_az_for_sensor,
                               uint32_t (&bin_array_det_idx)[MSMT_UPDATE_NUM_BINS_PER_SENSOR][MAX_DETS_IN_OBJ_TRK],
                               uint32_t (&bin_array_num_det_in_bin)[MSMT_UPDATE_NUM_BINS_PER_SENSOR]);

void Select_Median_Range_Rate_Detection_For_Each_Bin(
   const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
   const uint32_t (&bin_array_det_idx)[MSMT_UPDATE_NUM_BINS_PER_SENSOR][MAX_DETS_IN_OBJ_TRK],
   const uint32_t (&bin_array_num_det_in_bin)[MSMT_UPDATE_NUM_BINS_PER_SENSOR], uint32_t (&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
   uint32_t &selected_dets_num, uint32_t (&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT]);

void Try_To_Decrease_Num_Selected_Dets(const F360_Detection_T (&detections)[MAX_NUMBER_OF_DETECTIONS],
                                       const F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
                                       const uint32_t &k_min_num_selected_dets_per_sensor_for_binning,
                                       uint32_t (&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK], uint32_t &selected_dets_num,
                                       uint32_t (&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT]);

bool Is_Occluded_Oncoming_High_Speed(const F360_Calibrations_T &calib, const F360_Object_Track_T &object_track);

void Kmat_Times_Rmat(const float32_t (&Kmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT],
                     const float32_t (&Rmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT],
                     const uint32_t num_measurements, float32_t (&KRmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT]);

void Error_Cov_Update_Joseph_Form(float32_t (&ikh_mat)[STATE_DIMENSION][STATE_DIMENSION],
                                  const float32_t (&k_mat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT],
                                  const float32_t (&r_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT],
                                  const uint32_t nr_total_msnmts, float32_t (&p_mat)[STATE_DIMENSION][STATE_DIMENSION]);

void Add_Rmat_To_Smat(const float32_t (&r_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT],
                      const uint32_t nr_total_msnmts,
                      float32_t (&s_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT]);

void Update_Obj_Pos_And_Bbox_Pos(const float32_t new_pos_x, const float32_t new_pos_y, F360_Object_Track_T &object_track);

/*===========================================================================*\
* FUNCTION: KF_State_Update()
*===========================================================================
* ABSTRACT:
* This function updates the current state of an object.
* Common template implementation used by CTCA, CCA moveable, and CCA non-moveable.
*
\*===========================================================================*/
template <uint32_t MSMT_DIM>
void KF_State_Update(const float32_t (&z_mat)[MSMT_DIM], const float32_t (&zhat_mat)[MSMT_DIM], const uint32_t nr_total_msnmts,
                     const float32_t (&k_mat)[STATE_DIMENSION][MSMT_DIM], float32_t (&state)[STATE_DIMENSION])
{
   // The Kalman filter equation implemented below is x_updated = x_predicted + K*(z - z_pred)
   for (uint32_t row = 0U; row < STATE_DIMENSION; row++)
   {
      float32_t state_increment = 0.0F;
      for (uint32_t col = 0U; col < nr_total_msnmts; col++)
      {
         state_increment += k_mat[row][col] * (z_mat[col] - zhat_mat[col]);
      }
      state[row] += state_increment;
   }
}

/*===========================================================================*\
* FUNCTION: Setup_Pos_Msmt_Update_Data()
*===========================================================================
* ABSTRACT:
* Initializes pseudo position measurements in z and z_hat, sets the
* corresponding H rows, and copies position covariance into R.
\*===========================================================================*/
template <uint32_t MSMT_DIM>
void Setup_Pos_Msmt_Update_Data(const F360_Object_Track_T &object_track, const float32_t x_posn, const float32_t y_posn,
                                float32_t (&z_mat)[MSMT_DIM], float32_t (&zhat_mat)[MSMT_DIM],
                                float32_t (&h_mat)[MSMT_DIM][STATE_DIMENSION], float32_t (&r_mat)[MSMT_DIM][MSMT_DIM])
{
   const uint32_t state_x_idx    = (object_track.trk_fltr_type == F360_TRACKER_TRKFLTR_CCA) ?
                                      static_cast<uint32_t>(F360_TRK_FLTR_CCA_STATE_X) :
                                      static_cast<uint32_t>(F360_TRK_FLTR_CTCA_STATE_X);
   const uint32_t state_y_idx    = (object_track.trk_fltr_type == F360_TRACKER_TRKFLTR_CCA) ?
                                      static_cast<uint32_t>(F360_TRK_FLTR_CCA_STATE_Y) :
                                      static_cast<uint32_t>(F360_TRK_FLTR_CTCA_STATE_Y);
   z_mat[F360_PSEUDO_MSMT_POS_X] = object_track.pseudo_vcs_position.x;
   z_mat[F360_PSEUDO_MSMT_POS_Y] = object_track.pseudo_vcs_position.y;

   zhat_mat[F360_PSEUDO_MSMT_POS_X] = x_posn;
   zhat_mat[F360_PSEUDO_MSMT_POS_Y] = y_posn;

   h_mat[F360_PSEUDO_MSMT_POS_X][state_x_idx] = 1.0F;
   h_mat[F360_PSEUDO_MSMT_POS_Y][state_y_idx] = 1.0F;

   r_mat[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_X] = object_track.meascov[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_X];
   r_mat[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_Y] = object_track.meascov[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_Y];
   r_mat[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_X] = object_track.meascov[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_X];
   r_mat[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_Y] = object_track.meascov[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_Y];
}
} // namespace f360_variant_A
#endif
