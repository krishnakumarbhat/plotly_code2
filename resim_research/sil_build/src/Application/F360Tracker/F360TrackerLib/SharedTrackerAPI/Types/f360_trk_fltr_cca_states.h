#ifndef F360_TRK_FLTR_CCA_STATES_VARIANT_A_H
#define F360_TRK_FLTR_CCA_STATES_VARIANT_A_H
/*===================================================================================*\
* FILE: f360_trk_fltr_cca_states.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains F360_Trk_Fltr_CCA_States_T enum declaration
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

namespace f360_variant_A
{
   enum F360_Trk_Fltr_CCA_States : uint8_t
   {
      F360_TRK_FLTR_CCA_STATE_X = 0U, // VCS position x
      F360_TRK_FLTR_CCA_STATE_VX = 1U, // VCS over the ground velocity x
      F360_TRK_FLTR_CCA_STATE_AX = 2U, // VCS over the ground acceleration x
      F360_TRK_FLTR_CCA_STATE_Y = 3U, // VCS position y
      F360_TRK_FLTR_CCA_STATE_VY = 4U, // VCS over the ground velocity y
      F360_TRK_FLTR_CCA_STATE_AY = 5U // VCS over the ground acceleration y
   };
}
#endif
