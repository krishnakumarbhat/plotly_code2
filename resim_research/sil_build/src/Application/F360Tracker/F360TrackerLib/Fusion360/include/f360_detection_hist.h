/*===================================================================================*\
* FILE: f360_detection_hist.h
*====================================================================================
* Copyright 2017 Delphi Technologies, Inc., All Rights Reserved.
* Delphi Confidential
*-----------------------------------------------------------------------------------------
* %full_filespec: %
* %version: %
* %derived_by: %
* %date_created: %
* or
* $SOURCE: $
* $REVISION: $
* $AUTHOR: $
*-----------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains F360_Object_Det_Prop_T   structure declaration
*
* ABBREVIATIONS:
*  None
*
* TRACEABILITY INFO:
*   Design Document(s): defineFusion360Types.m
*
*   Requirements Document(s):
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
* DEVIATIONS FROM STANDARDS:
*   None.
*
\*==========================================================================================*/
#ifndef F360_DET_HIST_H
#define F360_DET_HIST_H

#include "f360_reuse.h"
#include "f360_constants.h"
#include "f360_point.h"
#include "f360_look_type.h"
#include "f360_range_type.h"
#include "f360_detection_wheelspin_type.h"
#include "rspp_detection_motion_status.h"
#include "f360_matrix_dimension.h"
namespace f360_variant_A
{
   typedef struct F360_Detection_Hist_Data_Tag
   {
      float32_t vcs_position_x;
      float32_t vcs_position_y;
      float32_t vcs_position_z;
      float32_t rdot;
      float32_t rdot_comp;
      float32_t vcs_az;
      float32_t time_since_meas;
      float32_t v_wrapping;
      float32_t r_wrapping;
      float32_t elevation;
      int16_t cluster_idx;
      F360_Detection_Wheelspin_Type_T wheel_spin_type;
      int8_t motion_status;
      bool f_dealiased;
      bool f_FOV_edge;
      bool f_selected;
      bool f_is_range_in_all_looks;
      bool f_potential_angle_jump;
      bool f_super_res;
      int8_t az_conf;
      int8_t el_conf;
      uint8_t sensor_id;
   }F360_Detection_Hist_Data_T;

   typedef struct F360_Detection_Hist_Tag
   {
      F360_Detection_Hist_Data_T det_data[MAX_NUMBER_OF_HISTORIC_DETECTIONS];
      int32_t n_occupied;
      int32_t max_occupation;
      bool f_idx_occupied[MAX_NUMBER_OF_HISTORIC_DETECTIONS];
      uint8_t padding[2];
   } F360_Detection_Hist_T;

}
#endif


