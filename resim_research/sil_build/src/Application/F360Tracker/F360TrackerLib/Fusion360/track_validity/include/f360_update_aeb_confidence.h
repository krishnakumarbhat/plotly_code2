/*===================================================================================*\
* FILE: f360_update_aeb_confidence.h
*====================================================================================
* Copyright (C) 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential – Restricted Aptiv information. Do not disclose."
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
* This file contains function declaration of Update_AEB_Confidence() function
*
* Applicable Standards (in order of precedence: highest first):
* ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
* ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#ifndef F360_UPDATE_AEB_CONFIDENCE_H
#define F360_UPDATE_AEB_CONFIDENCE_H

#include "f360_tracker_info.h"
#include "f360_reuse.h"
#include "f360_constants.h"
#include "f360_host.h"
#include "f360_radar_sensor.h"
#include "rspp_detection_list.h"
#include "f360_object_track.h"
#include "f360_calibrations.h"
#include "f360_timing_info.h"
#include "f360_static_env_poly_types.h"
#include "f360_occlusion_types.h"
#include "f360_trailer_manager.h"

namespace f360_variant_A
{
   void Update_AEB_Confidence(
      const F360_Host_T& host,
      const F360_Tracker_Info_T& tracker_info,
      F360_Object_Track_T(&object_tracks)[NUMBER_OF_OBJECT_TRACKS]);
}
#endif
