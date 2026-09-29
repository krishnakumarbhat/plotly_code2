/*===================================================================================*\
* FILE: f360_trk_fltr_cca_states_logging.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains enum declaration of F360_Trk_Fltr_CCA_States_Logging_T enum
*   reflecting the state order of previously used CCA motion model. It also contains
*   enum declarations of F360_Trk_Fltr_CCA_Supplemental_Cov_Elements_Logging_T and 
*   F360_Trk_Fltr_CCA_Supplemental_Cov_Elements_Resim_Init_T which reflects how the 
*   CCA state covariance matix were vectorized during logging. All these three 
*   enumerations are to be used in resim initialization when converting error 
*   covariance matrix from CCA tracks to match the current CCV state representation.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/
#ifndef F360_TRK_FLTR_CCA_STATES_LOGGING_VARIANT_A_H
#define F360_TRK_FLTR_CCA_STATES_LOGGING_VARIANT_A_H

#include "f360_reuse.h"

namespace f360_variant_A
{
   typedef enum F360_Trk_Fltr_CCA_States_Log_Tag : uint8_t
   {
      F360_TRK_FLTR_CCA_STATE_LOG_X = 0, // VCS position x
      F360_TRK_FLTR_CCA_STATE_LOG_VX = 1, // VCS over the ground velocity x
      F360_TRK_FLTR_CCA_STATE_LOG_AX = 2, // VCS over the ground acceleration x
      F360_TRK_FLTR_CCA_STATE_LOG_Y = 3, // VCS position y
      F360_TRK_FLTR_CCA_STATE_LOG_VY = 4, // VCS over the ground velocity y
      F360_TRK_FLTR_CCA_STATE_LOG_AY = 5 // VCS over the ground acceleration y
   }F360_Trk_Fltr_CCA_States_Log_T;

   typedef enum F360_Trk_Fltr_CCA_Suppl_Cov_Elem_Log_Tag : uint8_t
   {
       F360_TRK_FLTR_CCA_SUPPL_COV_LOG_X_Y = 0, // Cross covariance of VCS position x and VCS position x
       F360_TRK_FLTR_CCA_SUPPL_COV_LOG_VX_VY = 1, // Cross covariance of VCS over the ground velocity x and VCS over the ground velocity y
       F360_TRK_FLTR_CCA_SUPPL_COV_LOG_AX_AY = 2, // Cross covariance of VCS over the ground acceleration x and VCS over the ground acceleration y
   }F360_Trk_Fltr_CCA_Suppl_Cov_Elem_Log_T;

   typedef enum F360_Trk_Fltr_CCA_Other_Cov_Elem_Log_Tag : uint8_t
   {
       F360_TRK_FLTR_CCA_OTHER_COV_LOG_X_VX = 0, // Cross covariance of VCS position x and VCS over the ground velocity x
       F360_TRK_FLTR_CCA_OTHER_COV_LOG_X_AX = 1, // Cross covariance of VCS position x and VCS over the ground acceleration x
       F360_TRK_FLTR_CCA_OTHER_COV_LOG_X_VY = 2, // Cross covariance of VCS position x and VCS over the ground velocity y
       F360_TRK_FLTR_CCA_OTHER_COV_LOG_X_AY = 3, // Cross covariance of VCS position x and VCS over the ground acceleration y
       F360_TRK_FLTR_CCA_OTHER_COV_LOG_VX_AX = 4, // Cross covariance of VCS over the ground velocity x and VCS over the ground acceleration x
       F360_TRK_FLTR_CCA_OTHER_COV_LOG_VX_Y = 5, // Cross covariance of VCS over the ground velocity x and VCS position y
       F360_TRK_FLTR_CCA_OTHER_COV_LOG_VX_AY = 6, // Cross covariance of VCS over the ground velocity x and VCS over the ground acceleration y
       F360_TRK_FLTR_CCA_OTHER_COV_LOG_AX_Y = 7, // Cross covariance of VCS over the ground acceleration x and VCS position y
       F360_TRK_FLTR_CCA_OTHER_COV_LOG_AX_VY = 8, // Cross covariance of VCS over the ground acceleration x and VCS over the ground velocity y
       F360_TRK_FLTR_CCA_OTHER_COV_LOG_Y_VY = 9, // Cross covariance of VCS position y and VCS over the ground velocity y
       F360_TRK_FLTR_CCA_OTHER_COV_LOG_Y_AY = 10, // Cross covariance of VCS position y and VCS over the ground acceleration y
       F360_TRK_FLTR_CCA_OTHER_COV_LOG_VY_AY = 11, // Cross covariance of VCS over the ground velocity y and VCS over the ground acceleration y
   }F360_Trk_Fltr_CCA_Other_Cov_Elem_Log_T;
}
#endif
