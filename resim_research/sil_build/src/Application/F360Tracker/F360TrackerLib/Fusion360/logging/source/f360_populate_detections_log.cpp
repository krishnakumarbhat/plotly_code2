/*===================================================================================*\
* FILE:  f360_populate_detections_log.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose."
*------------------------------------------------------------------------------------
* Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN, "APTIV C++ Coding Standards"[May 26, 2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "APTIV C Coding Standards" [12-Mar-2006]
*/
#include "f360_populate_detections_log.h"

void f360_variant_A::Populate_Detections_Log(F360_Detection_Log_Output_T* const log, const rspp_variant_A::RSPP_Detection_List_T& det_list,
   const F360_Detection_Props_T(&detections)[MAX_NUMBER_OF_DETECTIONS])
{
   for (uint16_t i = 0U; i < det_list.number_of_valid_detections; i++)
   {
      const rspp_variant_A::RSPP_Detection_T* const p_raw_det = &(det_list.detections[i]);
      const F360_Detection_Props_T* const p_det_props = &(detections[i]);
      F360_Detection_Log_T* const p_detsLog = &log->detection[i];

      p_detsLog->sensorID = static_cast<uint8_t>(p_raw_det->raw.sensor_id);
      p_detsLog->raw_det_id = static_cast<uint16_t>(p_raw_det->raw.det_id);
      p_detsLog->vcs_x = p_det_props->vcs_position.x;
      p_detsLog->vcs_y = p_det_props->vcs_position.y;
      p_detsLog->rngrate_dealiased = p_det_props->range_rate_dealiased;
      p_detsLog->rngrate_comp = p_det_props->range_rate_compensated;
      p_detsLog->objTrkID = static_cast<uint16_t>(p_det_props->object_track_id);
      p_detsLog->clusterID = static_cast<uint16_t>(p_det_props->cluster_id);
      p_detsLog->motion_status = static_cast<int8_t>(p_det_props->motion_status);
      p_detsLog->wheel_spin = static_cast<uint8_t>(p_det_props->wheel_spin_type);
      p_detsLog->f_dealiased = p_det_props->f_dealiased ? 1U : 0U;
      p_detsLog->f_double_bounce = p_det_props->f_double_bounce ? 1U : 0U;
      p_detsLog->f_FOV_edge = p_det_props->f_FOV_edge ? 1U : 0U;
      p_detsLog->f_rr_inlier = p_det_props->f_rr_inlier ? 1U : 0U;
      p_detsLog->f_used_in_rr_msmt_update = p_det_props->f_used_in_rr_msmt_update ? 1U : 0U;
      p_detsLog->f_close_target = p_det_props->f_close_target ? 1U : 0U;
      p_detsLog->f_inside_gate = p_det_props->f_inside_gate ? 1U : 0U;
      p_detsLog->f_inside_mov_gate = p_det_props->f_inside_mov_gate ? 1U : 0U;
      p_detsLog->f_ok_to_use = p_det_props->f_ok_to_use ? 1U : 0U;
      p_detsLog->f_trailer_related_det = p_det_props->f_trailer_related_det ? 1U : 0U;
      p_detsLog->f_on_guardrail = (F360_INVALID_UNSIGNED_ID == p_det_props->on_sep_id) ? 0U : 1U;
   }
}
