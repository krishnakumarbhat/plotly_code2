#ifndef F360_MSMT_UPDATE_KALMAN_SUPPORT_FUNCTIONS_CCA_MOVEABLE_H
#define F360_MSMT_UPDATE_KALMAN_SUPPORT_FUNCTIONS_CCA_MOVEABLE_H
/*===========================================================================*\
* FILE: f360_msmt_update_support_functions_cca_moveable.h
*============================================================================
* Copyright (C) 2023 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains functions related to Kalman measurement update step of CCA objects
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#include "f360_constants.h"
#include "f360_object_track.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void Kalman_Gain_Update_CCA_Moveable(
      const float32_t(&h_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
      const float32_t(&p_mat)[STATE_DIMENSION][STATE_DIMENSION],
      const float32_t(&r_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const uint32_t nr_total_msnmts,
      const bool& obj_ok_to_decay_states,
      float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      float32_t(&s_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT]);

   void Error_Cov_Update_CCA_Moveable(
      const float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const float32_t(&r_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const float32_t(&h_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
      const uint32_t nr_total_msnmts,
      float32_t(&p_mat)[STATE_DIMENSION][STATE_DIMENSION]);

   void State_Update_CCA_Moveable(
      const float32_t(&z_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const float32_t(&zhat_mat)[MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const uint32_t nr_total_msnmts,
      const float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      float32_t(&state)[STATE_DIMENSION]);

   void Measurement_Update_Pointing_Heading_Rate_CCA(
      const F360_Calibrations_T& calibs,
      F360_Object_Track_T& obj);

   void Hmat_Times_Pmat_CCA_Moveable(
      const float32_t(&Hmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
      const float32_t(&Pmat)[STATE_DIMENSION][STATE_DIMENSION],
      const uint32_t num_measurements,
      float32_t(&HPmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION]
   );

   void HPmat_Times_Hmat_Transpose_CCA_Moveable(
      const float32_t(&HPmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
      const float32_t(&Hmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
      const uint32_t num_measurements,
      float32_t(&HPHTmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT]
   );

   void Negative_Kmat_Times_Hmat_CCA_Moveable(
      const float32_t(&Kmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const float32_t(&Hmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION],
      const uint32_t num_measurements,
      float32_t(&negKHmat)[STATE_DIMENSION][STATE_DIMENSION]
   );

   void Kmat_Times_Rmat_CCA_Moveable(
      const float32_t(&Kmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const float32_t(&Rmat)[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT],
      const uint32_t num_measurements,
      float32_t(&KRmat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT]
   );

   float32_t Saturate_Heading_Rate(
      const float32_t heading_rate,
      const float32_t abs_obj_speed);

   void Decrease_Kmat_For_Occluded_CCA(
      const F360_Calibrations_T& calib,
      const F360_Object_Track_T& object_track,
      float32_t(&k_mat)[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT]
   );
}


#endif
