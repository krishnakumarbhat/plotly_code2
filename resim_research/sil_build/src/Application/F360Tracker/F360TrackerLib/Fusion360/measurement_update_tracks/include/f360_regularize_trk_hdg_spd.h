/*===================================================================================*\
* FILE: f360_regularize_trk_hdg_spd.h
*====================================================================================
* Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*    This file contains function declaration of Regularize_Trk_Hdg_Spd()
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#ifndef F360_REGULARIZE_TRK_HDG_SPD_H
#define F360_REGULARIZE_TRK_HDG_SPD_H

#include "f360_object_track.h"
#include "f360_calibrations.h"

namespace f360_variant_A
{
   void Regularize_Trk_Hdg_Spd(
         const F360_Calibrations_T & calib,
         F360_Object_Track_T & object_track
         );
}
#endif
