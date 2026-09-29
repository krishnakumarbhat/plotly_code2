/*===================================================================================*\
* FILE:  f360_adjust_fltr_type_dependent_params.h
*====================================================================================

*Copyright (C) 2020 Aptiv Advanced Safety and User Experience. All rights reserved.

* Confidential - Restricted Aptiv information. Do not disclose."

*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains Adjust_Fltr_Type_Dependet_Params()  function declaration
*
*

* Applicable Standards (in order of precedence: highest first):

*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]

*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]

***/

#ifndef F360_ADJUST_FLTR_TYPE_DEPENDENT_PARAMS_H
#define F360_ADJUST_FLTR_TYPE_DEPENDENT_PARAMS_H

#include "f360_object_track.h"
#include "f360_tracker_info.h"
#include "f360_calibrations.h"
#include "f360_timing_info.h"

namespace f360_variant_A
{
   void Adjust_Fltr_Type_Dependent_Params(
      const F360_Tracker_Info_T& tracker_Info,
      F360_Object_Track_T(&obj_tracks)[NUMBER_OF_OBJECT_TRACKS],
      const F360_Calibrations_T& calib,
      F360_TRKR_TIMING_INFO_T& timing_info);

   void Adjust_Fltr_Type_CTCA_To_CCA(F360_Object_Track_T& obj_track);
}

#endif
