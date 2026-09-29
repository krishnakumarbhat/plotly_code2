/*===========================================================================*\
* FILE: f360_msmt_update_support_functions_ctca.h
*============================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains functions related to Kalman measurement update step of CTCA objects
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_MSMT_UPDATE_KALMAN_SUPPORT_FUNCTIONS_CTCA_H
#define F360_MSMT_UPDATE_KALMAN_SUPPORT_FUNCTIONS_CTCA_H

#include "f360_reuse.h"
#include "f360_constants.h"
#include "f360_object_track.h"
#include "f360_detection_props.h"
#include "f360_calibrations.h"
#include "f360_host.h"
#include "rspp_detection_list.h"

namespace f360_variant_A
{
   void Fill_H_Z_Mat_Single_Row_With_RR_Info_CTCA(
      const F360_Object_Track_T& object_track,
      const F360_Detection_Props_T& det_prop,
      const rspp_variant_A::RSPP_Detection_T& det,
      const uint32_t row_idx,
      float32_t(&h_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
      float32_t(&z_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT],
      float32_t(&zhat_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT]
   );

   void Filling_Msmt_Cov_By_H_RR_CTCA(
      const F360_Calibrations_T& calib,
      const uint32_t selected_dets_num,
      const uint32_t(&selected_dets_score)[MSMT_UPDATE_MAX_NUM_OF_MSMT],
      float32_t(&r_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT]
   );

   void Filling_Msmt_Cov_By_Pos_CTCA(
      const F360_Object_Track_T& object_track,
      float32_t(&r_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT]
   );


   void Kalman_Gain_Update_CTCA(
      const float32_t(&h_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
      const float32_t(&p_mat)[STATE_DIMENSION][STATE_DIMENSION],
      const float32_t(&r_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const uint32_t nr_total_msnmts,
      float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      float32_t(&s_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT]
   );

   void Error_Cov_Update_CTCA(
      const float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const float32_t(&r_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const float32_t(&h_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
      const uint32_t nr_total_msnmts,
      float32_t(&p_mat)[STATE_DIMENSION][STATE_DIMENSION]
   );

   void State_Update_CTCA(
      const float32_t(&z_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const float32_t(&zhat_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const uint32_t nr_total_msnmts,
      const float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      float32_t(&state)[STATE_DIMENSION]
   );

   void Hmat_Times_Pmat_CTCA(
      const float32_t(&Hmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
      const float32_t(&Pmat)[STATE_DIMENSION][STATE_DIMENSION],
      const uint32_t num_measurements,
      float32_t(&HPmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION]
   );

   void HPmat_Times_Hmat_Transpose_CTCA(
      const float32_t(&HPmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
      const float32_t(&Hmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
      const uint32_t num_measurements,
      float32_t(&HPHTmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT]
   );

   void Negative_Kmat_Times_Hmat_CTCA(
      const float32_t(&Kmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const float32_t(&Hmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
      const uint32_t num_measurements,
      float32_t(&negKHmat)[STATE_DIMENSION][STATE_DIMENSION]
   );

   void Kmat_Times_Rmat_CTCA(
      const float32_t(&Kmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const float32_t(&Rmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const uint32_t num_measurements,
      float32_t(&KRmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT]
   );
   void Reset_Pmat_state(
       const float32_t uncertainty_variance,
       const uint8_t state,
       float32_t(&p_mat)[STATE_DIMENSION][STATE_DIMENSION]
   );

   void Decrease_Kmat_For_Occluded_CTCA(
      const F360_Calibrations_T& calib,
      const F360_Object_Track_T& object_track,
      float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT]
   );
}


#endif
