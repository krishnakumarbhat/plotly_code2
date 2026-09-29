/*===================================================================================*\
* FILE: ocg_underdrivability_type.h
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Underdrivability related type definitions
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifndef UNDERDRIVABILITY_TYPE_H
#define UNDERDRIVABILITY_TYPE_H

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4820)
#endif

#include "ocg_reuse.h"
#include "ocg_underdrivability_enum.h"
#include "ocg_underdrivability_states.h"
#include "ocg_position.h"
#include "ocg_constants.h"

namespace ocg
{
   struct OCG_Zones_Probabilities_T
   {
      float can_pass_height[NUM_CELLS_X];
      float can_pass_RCS[NUM_CELLS_X];
      float is_likely_to_pass[NUM_CELLS_X];
      float is_likely_to_pass_height[NUM_CELLS_X];
      float is_likely_to_pass_RCS[NUM_CELLS_X];
      float can_not_pass[NUM_CELLS_X];
   };

   struct OCG_Zones_Innovation_T
   {
      float height_can_pass[UD_HEIGHT_STATE_SIZE];
      float height_is_likely_to_pass[UD_HEIGHT_STATE_SIZE];
      float height_can_not_pass_upper[UD_HEIGHT_STATE_SIZE];
      float height_can_not_pass_lower[UD_HEIGHT_STATE_SIZE];

      float RCS_slope_can_pass[UD_RCS_STATE_SIZE];
      float RCS_slope_is_likely_to_pass[UD_RCS_STATE_SIZE];
      float RCS_slope_can_not_pass[UD_RCS_STATE_SIZE];
   };

   struct OCG_Cell_Classification
   {
      // Underdrivability status value for a cell on the grid
      OCG_Underdrivable_Status_T underdrivability_status;
      // Underdrivability probabilities values for all status for a cell.
      float probs[UNDERDRIVABLE_STATUS_TOTAL];
   };

   struct OCG_Single_Underdrivability_Zone_T
   {
       // Overhead suspicious level for different range zones.
      OCG_Cell_Classification cell_classification;
      /* States for computing mean and variance of gathered detection height data in the different range zones
       * First state is number of detections
       * Second state is mean elevation angle of detections
       * Third state is mean squared elevation angle of detections
       * Angle is normalized i.e. the difference between elev angle to a height of an obstacle in certain hypothesis and the actual elev angle of detection
       */
      float state_height_can_pass[UD_HEIGHT_STATE_SIZE];
      float state_height_is_likely_to_pass[UD_HEIGHT_STATE_SIZE];
      float state_height_can_not_pass_upper[UD_HEIGHT_STATE_SIZE];
      float state_height_can_not_pass_lower[UD_HEIGHT_STATE_SIZE];

      /* States for computing mean and variance of gathered detection RCS slope data (slope of RCS vs range curve) in the different range zones
       * First state is number of detections
       * Second state is mean range of detections
       * Third state is  mean squared range of detections
       * Fourth state is mean RCS of detections
       * Fifth state is mean squared RCS of detections
       * Sixth state is mean RCS and range product of detections
       */
      float state_RCS_slope_can_pass[UD_RCS_STATE_SIZE];
      float state_RCS_slope_is_likely_to_pass[UD_RCS_STATE_SIZE];
      float state_RCS_slope_can_not_pass_upper[UD_RCS_STATE_SIZE];
      float state_RCS_slope_can_not_pass_lower[UD_RCS_STATE_SIZE];

      // Probabilities for different hypotheses in different range zones - height
      float p_height_can_pass;
      float p_height_is_likely_to_pass;
      float p_height_can_not_pass_upper;
      float p_height_can_not_pass_lower;

      // Probabilities for different hypotheses in different range zones - RCS slope
      float p_RCS_slope_can_pass;
      float p_RCS_slope_is_likely_to_pass;
      float p_RCS_slope_can_not_pass_upper;
      float p_RCS_slope_can_not_pass_lower;

      // Probabilities for different hypotheses considering both height and RCS slope together in different range zones
      float p_can_pass;
      float p_is_likely_to_pass;
      float p_can_not_pass;
   };

   struct OCG_Underdrivability_Props_T
   {
      OCG_Position_T ogcs_host_rear_axle_initial_position;
      float host_travel_distance;
      float grid_curvature;
      float timestamp_delta_s;
      uint64_t timestamp_us;
      uint64_t prev_timestamp_us;
      uint16_t circular_buffer_idx;
   };

   struct OCG_Underdrivability_Internal_T
   {
      uint16_t global_zone_idx[NUM_CELLS_X];
      OCG_Single_Underdrivability_Zone_T zones[NUM_CELLS_X];
      OCG_Underdrivability_Props_T props;
   };

   struct OCG_Underdrivability_T
   {
      // Host rear axle positon in Occupancy Grid Coordinate System (OGCS)
      OCG_Position_T ogcs_host_rear_axle_position;
      // Underdrivability status values and probabilities for all cells on the grid
      OCG_Cell_Classification underdrivability_classification[NUM_CELLS_X][NUM_CELLS_Y];
      
      // Zone curvature defined for host rear axle postion
      float grid_curvature;
   };

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif
