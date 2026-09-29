/*===========================================================================*\
* FILE: f360_trk_fltr_ccv_states.h
*============================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains F360_Trk_Fltr_CCV_States_T which contains information about the
*   order of the states in the CCV state vector. The files also contains
*   F360_Trk_Fltr_CCV_Supplemental_Cov_Elements_Logging_T
*   and F360_Trk_Fltr_CCV_Other_Cov_Elements_Logging_T enum declarations. These contain 
*   information about how the state vector covariance elements are being vectorized during 
*   logging.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_TRK_FLTR_CCV_STATES_VARIANT_A_H
#define F360_TRK_FLTR_CCV_STATES_VARIANT_A_H

#include "f360_reuse.h"

namespace f360_variant_A
{
   typedef enum F360_Trk_Fltr_CCV_States_Tag : uint8_t
   {
      F360_TRK_FLTR_CCV_STATE_X = 0, // VCS position x
      F360_TRK_FLTR_CCV_STATE_VX = 1, // VCS over the ground velocity x
      F360_TRK_FLTR_CCV_STATE_Y = 2, // VCS position y
      F360_TRK_FLTR_CCV_STATE_VY = 3, // VCS over the ground velocity y
      F360_TRK_FLTR_CCV_STATE_UNUSED_1 = 4,
      F360_TRK_FLTR_CCV_STATE_UNUSED_2 = 5
   }F360_Trk_Fltr_CCV_States_T;

   typedef enum F360_Trk_Fltr_CCV_Supplemental_Cov_Elements_Logging_Tag : uint8_t
   {
       F360_TRK_FLTR_CCV_SUPPLEMENTAL_COV_LOGGING_X_Y = 0, // Cross covariance of VCS position x and VCS position x
       F360_TRK_FLTR_CCV_SUPPLEMENTAL_COV_LOGGING_VX_VY = 1, // Cross covariance of VCS over the ground velocity x and VCS over the ground velocity y
       F360_TRK_FLTR_CCV_SUPPLEMENTAL_COV_LOGGING_AX_AY = 2 // Cross covariance of VCS over the ground acceleration x and VCS over the ground acceleration y
   }F360_Trk_Fltr_CCV_Supplemental_Cov_Elements_Logging_T;

   typedef enum F360_Trk_Fltr_CCV_Other_Cov_Elements_Logging_Tag : uint8_t
   {
      F360_TRK_FLTR_CCV_OTHER_COV_LOGGING_X_VX = 0, // Cross covariance of VCS position x and VCS over the ground velocity x
      F360_TRK_FLTR_CCV_OTHER_COV_LOGGING_X_VY = 1, // Cross covariance of VCS position x and VCS over the ground velocity y
      F360_TRK_FLTR_CCV_OTHER_COV_LOGGING_VX_Y = 2, // Cross covariance of VCS over the ground velocity x and VCS position y
      F360_TRK_FLTR_CCV_OTHER_COV_LOGGING_Y_VY = 3, // Cross covariance of VCS position y and VCS over the ground velocity y
      F360_TRK_FLTR_CCV_OTHER_COV_LOGGING_X_AX = 4, // Cross covariance of VCS position x and VCS over the ground acceleration x
      F360_TRK_FLTR_CCV_OTHER_COV_LOGGING_X_AY = 5, // Cross covariance of VCS position x and VCS over the ground acceleration y
      F360_TRK_FLTR_CCV_OTHER_COV_LOGGING_VX_AX = 6, // Cross covariance of VCS over the ground velocity x and VCS over the ground acceleration x
      F360_TRK_FLTR_CCV_OTHER_COV_LOGGING_VX_AY = 7, // Cross covariance of VCS over the ground velocity x and VCS over the ground acceleration y
      F360_TRK_FLTR_CCV_OTHER_COV_LOGGING_Y_AX = 8, // Cross covariance of VCS position y and VCS over the ground acceleration x
      F360_TRK_FLTR_CCV_OTHER_COV_LOGGING_Y_AY = 9, // Cross covariance of VCS position y and VCS over the ground acceleration y
      F360_TRK_FLTR_CCV_OTHER_COV_LOGGING_VY_AX = 10, // Cross covariance of VCS over the ground velocity y and VCS over the ground acceleration x
      F360_TRK_FLTR_CCV_OTHER_COV_LOGGING_VY_AY = 11  // Cross covariance of VCS over the ground velocity y and VCS over the ground acceleration y
   }F360_Trk_Fltr_CCV_Other_Cov_Elements_Logging_T;
}
#endif
