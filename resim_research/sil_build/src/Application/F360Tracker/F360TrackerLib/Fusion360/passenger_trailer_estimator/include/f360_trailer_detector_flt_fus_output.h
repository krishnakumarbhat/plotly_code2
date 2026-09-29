#ifndef TRAILER_DETECTOR_FLT_FUS_OUTPUT_H
#define TRAILER_DETECTOR_FLT_FUS_OUTPUT_H
/*===========================================================================*\
 * FILE: f360_trailer_detector_flt_fus_output.h
 *============================================================================
 * Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
 * Confidential - Restricted Aptiv information. Do not disclose.
 *-----------------------------------------------------------------------------------------
 * DESCRIPTION:
 *   This file defines trailer detector fused output structure
 *
 *   Applicable Standards (in order of precedence: highest first):
 *     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
 *     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
 *
\*==========================================================================================*/

#include "f360_reuse.h"

namespace f360_variant_A
{
   enum Trailer_Presence_State : uint8_t
   {
      TRAILER_PRESENCE_STATE_NOT_DETECTED = 0U,
      TRAILER_PRESENCE_STATE_DETECTED = 1U,
      TRAILER_PRESENCE_STATE_UNKNOWN = 2U // Default State
   };

   typedef struct F360_Trailer_Estimator_Output_Tag 
   {
      // Common output
      float32_t joint_position_vcs_long[2];                // [m] position of the joint/hitch
      float32_t joint_position_vcs_lat[2];                 // [m] position of the joint/hitch
      float32_t joint2center[2];                           // [m] distance between joint and trailer center (note: trailer edge need not be on joint)
      float32_t trailer_length[2];                         // [m] trailer length between tow hitch and trailer rear bumper
      float32_t trailer_width[2];                          // [m] trailer width
      float32_t trailer_angle[2];                          // [rad] angle between trailer orientation and host orientation; Positive for host right turn (note: VCS for CV, ISO for PV)
      Trailer_Presence_State trailer_presence[2];          // [-] trailer presence state: Detected/Not Detected/Unknown

      // Commercial vehicle trailer only output
      bool f_reversing_countermeasures_active;        // [-] flag indicating if reversing countermeasures are active
   } F360_Trailer_Estimator_Output_T;
}
#endif
