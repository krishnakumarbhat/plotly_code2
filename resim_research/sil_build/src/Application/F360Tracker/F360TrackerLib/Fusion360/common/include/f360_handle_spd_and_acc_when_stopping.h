/*===================================================================================*\
* FILE: f360_handle_spd_and_acc_when_stopping.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains function declaration of Handle_Spd_And_Acc_When_Stopping_CTCA() and Handle_Spd_And_Acc_When_Stopping_CCA()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards" [May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN, "APTIV C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#ifndef F360_HANDLE_SPD_AND_ACC_WHEN_STOPPING_H
#define F360_HANDLE_SPD_AND_ACC_WHEN_STOPPING_H

#include "f360_reuse.h"
#include "f360_object_track.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void Handle_Spd_And_Acc_When_Stopping_CTCA(
      const float32_t previous_speed,
      const F360_Calibrations_T& calibs,
      F360_Object_Track_T& obj);


   void Handle_Spd_And_Acc_When_Stopping_CCA(
      const F360_VCS_Velocity_T& previous_velocity,
      const F360_Calibrations_T& calibs,
      F360_Object_Track_T& obj);
}
#endif
