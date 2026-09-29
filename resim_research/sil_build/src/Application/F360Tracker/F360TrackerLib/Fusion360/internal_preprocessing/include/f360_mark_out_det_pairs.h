/*===========================================================================*\
* FILE: f360_mark_out_det_pairs.h
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*-----------------------------------------------------------------------------------------
* DESCRIPTION:
*   This file contains Mark_Out_Det_Pairs() and Mark_Out_Det_Pair() functions declarations.
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*==========================================================================================*/

#ifndef F360_MARK_OUT_DET_PAIRS_H
#define F360_MARK_OUT_DET_PAIRS_H

#include "f360_reuse.h"
#include "rspp_detection.h"
#include "f360_radar_sensor.h"
#include "f360_detection_props.h"
#include "f360_timing_info.h"

namespace f360_variant_A
{
   void Mark_Out_Det_Pairs(
      const rspp_variant_A::RSPP_Detection_T (&dets)[MAX_NUMBER_OF_DETECTIONS],
      const uint32_t num_valid_dets,
      const F360_Radar_Sensor_T(&sensors)[MAX_NUMBER_OF_SENSORS],
      F360_Detection_Props_T (&det_props)[MAX_NUMBER_OF_DETECTIONS],
      F360_TRKR_TIMING_INFO_T &timing_info
   );
}
#endif
