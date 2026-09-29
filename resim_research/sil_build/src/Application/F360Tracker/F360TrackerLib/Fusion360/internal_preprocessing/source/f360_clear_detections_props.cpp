/*===========================================================================*\
* FILE: f360_clear_detections_props.cpp
*============================================================================
* Copyright (C) 2019-2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
*----------------------------------------------------------------------------
* DESCRIPTION:
* This file contains the function definition to reset the detection properties.
*
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "Aptiv C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Aptiv C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/
#include "f360_clear_detections_props.h"
#include "f360_math_func.h"
#include <cstring>

namespace f360_variant_A
{
   /*===========================================================================*\
   * FUNCTION: Clear_Detections_Props()
   *===========================================================================
   * RETURN VALUE:
   * None
   *
   * PARAMETERS:
   * F360_Detection_Props_T (&det_p)[MAX_NUMBER_OF_DETECTIONS] - Pointer to detection property structure
   *
   * EXTERNAL REFERENCES:
   * None.
   *
   * DEVIATIONS FROM STANDARDS:
   * None.
   *
   * --------------------------------------------------------------------------
   * ABSTRACT:
   * --------------------------------------------------------------------------
   * This function reset the detection properties.
   *
   * PRECONDITIONS:
   * All the Pointers should Point to valid structures.
   *
   * POSTCONDITIONS:
   * None
   *
   \*===========================================================================*/
   void Clear_Detections_Props(
      F360_Detection_Props_T (&det_p)[MAX_NUMBER_OF_DETECTIONS])
   {
      const uint32_t DET_SIZE = sizeof(F360_Detection_Props_T);
      uint32_t det_num = 0U;

      det_p[det_num].vcs_position.x = 0.0F;
      det_p[det_num].vcs_position.y = 0.0F;
      det_p[det_num].range_rate_dealiased = 0.0F;
      det_p[det_num].range_dealiased = 0.0F;
      det_p[det_num].range_rate_compensated = 0.0F;
      det_p[det_num].range_rate_predicted = 0.0F;
      det_p[det_num].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_INVALID;
      det_p[det_num].cluster_id = 0;
      det_p[det_num].object_track_id = 0;
      det_p[det_num].f_dealiased = false;
      det_p[det_num].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
      det_p[det_num].f_double_bounce = false;
      det_p[det_num].f_close_target = false;
      det_p[det_num].f_FOV_edge = false;
      det_p[det_num].f_rr_inlier = false;
      det_p[det_num].f_used_in_rr_msmt_update = false;
      det_p[det_num].f_inside_gate = false;
      det_p[det_num].f_inside_mov_gate = false;
      det_p[det_num].f_ok_to_use = true;
      det_p[det_num].f_det_pair = false;
      det_p[det_num].f_potential_angle_jump = false;
      det_p[det_num].f_object_based_angle_jump = false;
      det_p[det_num].f_water_spray = false;
      det_p[det_num].f_stationary_bounce= false;
      det_p[det_num].f_azimuth_rdot_outlier = false;
      det_p[det_num].f_trailer_related_det = false;
      det_p[det_num].f_low_az_conf_det = false;
      det_p[det_num].f_unreliable_in_clutter = false;
      det_p[det_num].f_has_close_det_with_similar_rr = false;
      det_p[det_num].behind_sep_id = F360_INVALID_UNSIGNED_ID;
      det_p[det_num].on_sep_id = F360_INVALID_UNSIGNED_ID;
      det_p[det_num].f_angle_amb = false;
      det_p[det_num].f_rr_ambiguity = false;
      det_p[det_num].f_nd_target = false;

      det_num++;

      while (det_num < MAX_NUMBER_OF_DETECTIONS)
      {
         (void)memcpy(&det_p[det_num], &det_p[0], DET_SIZE);
         det_num++;
      }
   }
}
