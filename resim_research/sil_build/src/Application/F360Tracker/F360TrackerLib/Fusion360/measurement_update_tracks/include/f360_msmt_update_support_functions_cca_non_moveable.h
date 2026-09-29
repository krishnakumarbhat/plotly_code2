#ifndef F360_MSMT_UPDATE_KALMAN_SUPPORT_FUNCTIONS_CCA_NON_MOVEABLE_H
#define F360_MSMT_UPDATE_KALMAN_SUPPORT_FUNCTIONS_CCA_NON_MOVEABLE_H
/*===========================================================================*\
* FILE: f360_msmt_update_support_functions_cca_non_moveable.h
*============================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains functions related to Kalman measurement update step of non-moveable CCA objects
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_constants.h"
#include "f360_object_track.h"
#include "f360_detection_props.h"
#include "f360_calibrations.h"
#include "rspp_detection_list.h"

namespace f360_variant_A
{
   void Kalman_Gain_Update_CCA_Non_Moveable(
      const float32_t(&h_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][STATE_DIMENSION],
      const float32_t(&p_mat)[STATE_DIMENSION][STATE_DIMENSION],
      const float32_t(&r_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE],
      float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE],
      float32_t(&s_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE]);

   void Error_Cov_Update_CCA_Non_Moveable(
      const float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE],
      const float32_t(&r_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE],
      const float32_t(&h_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][STATE_DIMENSION],
      float32_t(&p_mat)[STATE_DIMENSION][STATE_DIMENSION]);

   void State_Update_CCA_Non_Moveable(
      const float32_t(&z_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE],
      const float32_t(&zhat_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE],
      const float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE],
      float32_t(&state)[STATE_DIMENSION]);

   void Hmat_Times_Pmat_CCA_Non_Moveable(
      const float32_t(&h_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][STATE_DIMENSION],
      const float32_t(&p_mat)[STATE_DIMENSION][STATE_DIMENSION],
      float32_t(&hp_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][STATE_DIMENSION]);

   void I_Minus_Kmat_Times_Hmat_CCA_Non_Moveable(
      const float32_t(&h_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][STATE_DIMENSION],
      const float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE],
      float32_t(&ikh_mat)[STATE_DIMENSION][STATE_DIMENSION]);

   void IKHmat_Times_Pmat_CCA_Non_Moveable(
      const float32_t(&ikh_mat)[STATE_DIMENSION][STATE_DIMENSION],
      const float32_t(&p_mat)[STATE_DIMENSION][STATE_DIMENSION],
      float32_t(&ikhp_mat)[STATE_DIMENSION][STATE_DIMENSION]);

   void IKHPmat_Times_IKHmat_Transpose_CCA_Non_Moveable(
      const float32_t(&ikhp_mat)[STATE_DIMENSION][STATE_DIMENSION],
      const float32_t(&ikh_mat)[STATE_DIMENSION][STATE_DIMENSION],
      float32_t(&ikhp_ikh_mat)[STATE_DIMENSION][STATE_DIMENSION]);

   void Smat_CCA_Non_Moveable(
      const float32_t(&h_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][STATE_DIMENSION],
      const float32_t(&p_mat)[STATE_DIMENSION][STATE_DIMENSION],
      const float32_t(&r_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE],
      float32_t(&s_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE]);

   void Kmat_Times_Rmat_Times_Kmat_CCA_Non_Moveable(
      const float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE],
      const float32_t(&r_mat)[MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE],
      float32_t(&krk_mat)[STATE_DIMENSION][STATE_DIMENSION]);

   void Generate_Single_Pseudo_Range_Rate_Compensated_Measurement_CCA(
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const rspp_variant_A::RSPP_Detection_List_T& raw_detection_list,
      const uint32_t(&dets_idx)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t num_dets,
      float32_t& mean_comp_rdot,
      float32_t& mean_cos_vcs_az,
      float32_t& mean_sin_vcs_az);

   bool Is_Obj_Suspected_Moveable_CCA(
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t(&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t selected_dets_num);

   void Limit_Pseudo_Pos_Meas_Impact_On_Obj_Vel_Estim_CCA(
      const F360_Object_Track_T& object_track,
      const float32_t host_yaw_rate,
      const F360_Calibrations_T& calib,
      float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_NUM_OF_MSMT_CCA_NON_MOVEABLE]);

   void Compute_KF_Gain_Decrease_Factor_CCA(
      const F360_Object_Track_T& object_track,
      const float32_t host_yaw_rate,
      const F360_Calibrations_T& calib,
      float32_t& decrease_factor_x,
      float32_t& decrease_factor_y);

   bool Check_For_Crossing_VRU_Type_Obj(
      const float32_t host_curvature_rear,
      const F360_Calibrations_T& calib,
      F360_Object_Track_T& object_track);

   bool Force_Pseudo_Pos_Vel_Lim_Impact_Logic(
      const float32_t host_speed,
      const F360_Object_Track_T& object_track,
      const F360_Detection_Props_T(&det_props)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t(&selected_dets_idx)[MAX_DETS_IN_OBJ_TRK],
      const uint32_t selected_dets_num);
}


#endif
