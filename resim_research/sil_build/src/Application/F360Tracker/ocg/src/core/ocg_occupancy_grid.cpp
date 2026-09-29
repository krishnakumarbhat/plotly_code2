/*===================================================================================*\
* FILE: ocg_occupancy_grid.cpp
*====================================================================================
* Copyright (C) 2022 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential � Restricted Aptiv information. Do not disclose.
*------------------------------------------------------------------------------------
*
* DESCRIPTION:
*   This file contains Assign_Detections_To_Underdrivability_Zones() function definition
*
*   Applicable Standards (in order of precedence: highest first):
*     ESGW_4-2_PE-SWX_00-01-A01_EN "Aptiv C++ Coding Standards" [26-May-2019]
*     ESGW_4-2_PE-SWX_00-01-A02_EN "Delphi C Coding Standards" [12-Mar-2006]
*
\*===================================================================================*/

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 5045)
#endif

#include "ocg_occupancy_grid.h"
#include "ocg_underdrivability.h"
#include "ocg_underdrivability_type.h"
#include "ocg_calibrations.h"
#include "ocg_initialize_underdrivability.h"
#include "cmn_calc_circular_zone_idx.h"
#include "cmn_fill_internals_helper.h"
#include "ocg_constants.h"
#include "ocg_version.h"
#include "ocg_calibrations.h"
#include "cmn_utilities.h"

namespace ocg
{
   void Occupancy_Grid::initialize(const OCG_Inputs_T &input)
   {
      m_timestamp = 0;
      m_active = false;
      Initialize_OCG_Calibrations(m_calibrations);
      Initialize_Underdrivability(input.host, m_underdrivability);
   }

   void Occupancy_Grid::step(const double timestamp, const OCG_Inputs_T &input)
   {
      m_timestamp = timestamp;

      if (!m_active)
      {
         for (uint32_t i_sen = 0; i_sen < rspp_variant_A::MAX_NUMBER_OF_SENSORS; i_sen++)
         {
            if (is_sensor_valid(input.sensors[i_sen], m_calibrations))
            {
               m_active = true;
               break;
            }
         }
      }
      if (m_active)
      {
         Underdrivability(
             input.detection_list,
             input.sensors,
             input.host,
             m_calibrations,
             m_underdrivability);
      }
   }

   void Occupancy_Grid::get_internals(OCG_Internals_T &ocg_internal) const
   {
      ocg_internal.timestamp = m_timestamp;
      ocg_internal.props.ogcs_host_rear_axle_initial_position = m_underdrivability.props.ogcs_host_rear_axle_initial_position;
      ocg_internal.props.host_travel_distance = m_underdrivability.props.host_travel_distance;
      ocg_internal.props.circular_buffer_idx = m_underdrivability.props.circular_buffer_idx;
      ocg_internal.props.timestamp_us = m_underdrivability.props.timestamp_us;
      ocg_internal.props.prev_timestamp_us = m_underdrivability.props.prev_timestamp_us;

      // Map underdrivability 1D grid into universal 2D grid interface
      for (uint32_t idx = 0U; idx < NUM_CELLS_X; idx++)
      {
         const uint32_t adjusted_idx = cmn::Calc_Circular_Zone_Idx(m_underdrivability.props.circular_buffer_idx, idx, NUM_CELLS_X);

         ocg_internal.cells[idx][0].global_zone_idx = m_underdrivability.global_zone_idx[idx];
         ocg_internal.cells[idx][0].p_height_can_pass = m_underdrivability.zones[adjusted_idx].p_height_can_pass;
         ocg_internal.cells[idx][0].p_height_is_likely_to_pass = m_underdrivability.zones[adjusted_idx].p_height_is_likely_to_pass;
         ocg_internal.cells[idx][0].p_height_can_not_pass_upper = m_underdrivability.zones[adjusted_idx].p_height_can_not_pass_upper;
         ocg_internal.cells[idx][0].p_height_can_not_pass_lower = m_underdrivability.zones[adjusted_idx].p_height_can_not_pass_lower;
         ocg_internal.cells[idx][0].p_RCS_slope_can_pass = m_underdrivability.zones[adjusted_idx].p_RCS_slope_can_pass;
         ocg_internal.cells[idx][0].p_RCS_slope_is_likely_to_pass = m_underdrivability.zones[adjusted_idx].p_RCS_slope_is_likely_to_pass;
         ocg_internal.cells[idx][0].p_RCS_slope_can_not_pass_upper = m_underdrivability.zones[adjusted_idx].p_RCS_slope_can_not_pass_upper;
         ocg_internal.cells[idx][0].p_RCS_slope_can_not_pass_lower = m_underdrivability.zones[adjusted_idx].p_RCS_slope_can_not_pass_lower;
         ocg_internal.cells[idx][0].p_can_pass = m_underdrivability.zones[idx].p_can_pass;
         ocg_internal.cells[idx][0].p_is_likely_to_pass = m_underdrivability.zones[idx].p_is_likely_to_pass;
         ocg_internal.cells[idx][0].p_can_not_pass = m_underdrivability.zones[idx].p_can_not_pass;

         ocg_internal.cells[idx][0].state_height_can_pass = Fill_Height_States(m_underdrivability.zones[adjusted_idx].state_height_can_pass);
         ocg_internal.cells[idx][0].state_height_is_likely_to_pass = Fill_Height_States(m_underdrivability.zones[adjusted_idx].state_height_is_likely_to_pass);
         ocg_internal.cells[idx][0].state_height_can_not_pass_upper = Fill_Height_States(m_underdrivability.zones[adjusted_idx].state_height_can_not_pass_upper);
         ocg_internal.cells[idx][0].state_height_can_not_pass_lower = Fill_Height_States(m_underdrivability.zones[adjusted_idx].state_height_can_not_pass_lower);

         ocg_internal.cells[idx][0].state_RCS_slope_can_pass = Fill_RCS_States(m_underdrivability.zones[adjusted_idx].state_RCS_slope_can_pass);
         ocg_internal.cells[idx][0].state_RCS_slope_is_likely_to_pass = Fill_RCS_States(m_underdrivability.zones[adjusted_idx].state_RCS_slope_is_likely_to_pass);
         ocg_internal.cells[idx][0].state_RCS_slope_can_not_pass_upper = Fill_RCS_States(m_underdrivability.zones[adjusted_idx].state_RCS_slope_can_not_pass_upper);
         ocg_internal.cells[idx][0].state_RCS_slope_can_not_pass_lower = Fill_RCS_States(m_underdrivability.zones[adjusted_idx].state_RCS_slope_can_not_pass_lower);
      }
   }

   void Occupancy_Grid::get_output(OCG_Outputs_T &ocg_output) const
   {
      // Underdrivability performs all calculations in Aptiv Vehicle Coordinate System (VCS - origin in center of the front bumper);
      // OCG output interface contains information in Occupancy Grid Coordinate System (OGCS - origin in the center of the bottom edge of the grid);
      // Map signals from Aptiv Vehicle Coordinate System to Grid Coordinate System where neccessary
      ocg_output.timestamp = m_timestamp;
      ocg_output.f_valid = m_active;
      ocg_output.underdrivability.grid_curvature = m_underdrivability.props.grid_curvature;
      ocg_output.underdrivability.ogcs_host_rear_axle_position.x = m_underdrivability.props.host_travel_distance + m_underdrivability.props.ogcs_host_rear_axle_initial_position.x;
      ocg_output.underdrivability.ogcs_host_rear_axle_position.y = m_underdrivability.props.ogcs_host_rear_axle_initial_position.y;
      ocg_output.underdrivability.ogcs_host_rear_axle_position.z = m_underdrivability.props.ogcs_host_rear_axle_initial_position.z;
      ocg_output.underdrivability.ogcs_host_rear_axle_position.yaw = m_underdrivability.props.ogcs_host_rear_axle_initial_position.yaw;

      // Map underdrivability 1D grid into universal 2D grid interface
      for (uint32_t idx = 0U; idx < NUM_CELLS_X; idx++)
      {
         ocg_output.underdrivability.underdrivability_classification[idx][0U].underdrivability_status = m_underdrivability.zones[idx].cell_classification.underdrivability_status;
         ocg_output.underdrivability.underdrivability_classification[idx][0U].probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER] = m_underdrivability.zones[idx].cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER];
         ocg_output.underdrivability.underdrivability_classification[idx][0U].probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER] = m_underdrivability.zones[idx].cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER];
         ocg_output.underdrivability.underdrivability_classification[idx][0U].probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER] = m_underdrivability.zones[idx].cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER];
         ocg_output.underdrivability.underdrivability_classification[idx][0U].probs[UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER] = m_underdrivability.zones[idx].cell_classification.probs[UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER];
      }

      ocg_output.grid_definition.num_cells_x_close = NUM_CELLS_X_CLOSE;
      ocg_output.grid_definition.num_cells_x_mid = NUM_CELLS_X_MID;
      ocg_output.grid_definition.num_cells_x_far = NUM_CELLS_X_FAR;
      ocg_output.grid_definition.num_cells_y = NUM_CELLS_Y;

      ocg_output.grid_definition.cell_length = CELL_LENGTH;
      ocg_output.grid_definition.cell_width = CELL_WIDTH;
      ocg_output.grid_definition.cell_width_extension_factor = CELL_WIDTH_EXTENSION_FACTOR;
   }

   void Occupancy_Grid::get_log_internals(OCG_Internals_Log_T &ocg_internals_log) const
   {
      ocg_internals_log.timestamp = m_timestamp;
      ocg_internals_log.ogcs_host_rear_axle_initial_position_x = m_underdrivability.props.ogcs_host_rear_axle_initial_position.x;
      ocg_internals_log.ogcs_host_rear_axle_initial_position_y = m_underdrivability.props.ogcs_host_rear_axle_initial_position.y;
      ocg_internals_log.ogcs_host_rear_axle_initial_position_z = m_underdrivability.props.ogcs_host_rear_axle_initial_position.z;
      ocg_internals_log.ogcs_host_rear_axle_initial_position_yaw = m_underdrivability.props.ogcs_host_rear_axle_initial_position.yaw;
      ocg_internals_log.host_travel_distance = m_underdrivability.props.host_travel_distance;
      ocg_internals_log.circular_buffer_idx = m_underdrivability.props.circular_buffer_idx;
      ocg_internals_log.timestamp_us = m_underdrivability.props.timestamp_us;
      ocg_internals_log.prev_timestamp_us = m_underdrivability.props.prev_timestamp_us;

      for (uint32_t idx = 0U; idx < NUM_CELLS_X; idx++)
      {
         const uint32_t adjusted_idx = cmn::Calc_Circular_Zone_Idx(m_underdrivability.props.circular_buffer_idx, idx, NUM_CELLS_X);
         
         ocg_internals_log.p_height_can_pass[idx] = m_underdrivability.zones[adjusted_idx].p_height_can_pass;
         ocg_internals_log.p_height_is_likely_to_pass[idx] = m_underdrivability.zones[adjusted_idx].p_height_is_likely_to_pass;
         ocg_internals_log.p_height_can_not_pass_upper[idx] = m_underdrivability.zones[adjusted_idx].p_height_can_not_pass_upper;
         ocg_internals_log.p_height_can_not_pass_lower[idx] = m_underdrivability.zones[adjusted_idx].p_height_can_not_pass_lower;
         ocg_internals_log.p_RCS_slope_can_pass[idx] = m_underdrivability.zones[adjusted_idx].p_RCS_slope_can_pass;
         ocg_internals_log.p_RCS_slope_is_likely_to_pass[idx] = m_underdrivability.zones[adjusted_idx].p_RCS_slope_is_likely_to_pass;
         ocg_internals_log.p_RCS_slope_can_not_pass_upper[idx] = m_underdrivability.zones[adjusted_idx].p_RCS_slope_can_not_pass_upper;
         ocg_internals_log.p_RCS_slope_can_not_pass_lower[idx] = m_underdrivability.zones[adjusted_idx].p_RCS_slope_can_not_pass_lower;
         ocg_internals_log.p_can_pass[idx] = m_underdrivability.zones[idx].p_can_pass;
         ocg_internals_log.p_is_likely_to_pass[idx] = m_underdrivability.zones[idx].p_is_likely_to_pass;
         ocg_internals_log.p_can_not_pass[idx] = m_underdrivability.zones[idx].p_can_not_pass;
         
         ocg_internals_log.state_height_can_pass_mean_elevation[idx] = m_underdrivability.zones[adjusted_idx].state_height_can_pass[UD_HEIGHT_STATE_ELEVATION_ANGLE];
         ocg_internals_log.state_height_can_pass_mean_sq_elevation[idx] = m_underdrivability.zones[adjusted_idx].state_height_can_pass[UD_HEIGHT_STATE_SQUARED_ELEVATION_ANGLE];
         ocg_internals_log.state_height_can_pass_num_dets[idx] = m_underdrivability.zones[adjusted_idx].state_height_can_pass[UD_HEIGHT_STATE_DET_COUNT];
         
         ocg_internals_log.state_height_is_likely_to_pass_mean_elevation[idx] = m_underdrivability.zones[adjusted_idx].state_height_is_likely_to_pass[UD_HEIGHT_STATE_ELEVATION_ANGLE];
         ocg_internals_log.state_height_is_likely_to_pass_mean_sq_elevation[idx] = m_underdrivability.zones[adjusted_idx].state_height_is_likely_to_pass[UD_HEIGHT_STATE_SQUARED_ELEVATION_ANGLE];
         ocg_internals_log.state_height_is_likely_to_pass_num_dets[idx] = m_underdrivability.zones[adjusted_idx].state_height_is_likely_to_pass[UD_HEIGHT_STATE_DET_COUNT];
         
         ocg_internals_log.state_height_can_not_pass_upper_mean_elevation[idx] = m_underdrivability.zones[adjusted_idx].state_height_can_not_pass_upper[UD_HEIGHT_STATE_ELEVATION_ANGLE];
         ocg_internals_log.state_height_can_not_pass_upper_mean_sq_elevation[idx] = m_underdrivability.zones[adjusted_idx].state_height_can_not_pass_upper[UD_HEIGHT_STATE_SQUARED_ELEVATION_ANGLE];
         ocg_internals_log.state_height_can_not_pass_upper_num_dets[idx] = m_underdrivability.zones[adjusted_idx].state_height_can_not_pass_upper[UD_HEIGHT_STATE_DET_COUNT];
         
         ocg_internals_log.state_height_can_not_pass_lower_mean_elevation[idx] = m_underdrivability.zones[adjusted_idx].state_height_can_not_pass_lower[UD_HEIGHT_STATE_ELEVATION_ANGLE];
         ocg_internals_log.state_height_can_not_pass_lower_mean_sq_elevation[idx] = m_underdrivability.zones[adjusted_idx].state_height_can_not_pass_lower[UD_HEIGHT_STATE_SQUARED_ELEVATION_ANGLE];
         ocg_internals_log.state_height_can_not_pass_lower_num_dets[idx] = m_underdrivability.zones[adjusted_idx].state_height_can_not_pass_lower[UD_HEIGHT_STATE_DET_COUNT];
         
         ocg_internals_log.state_RCS_slope_can_pass_num_dets[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_can_pass[UD_RCS_STATE_DET_COUNT];
         ocg_internals_log.state_RCS_slope_can_pass_mean_range[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_can_pass[UD_RCS_STATE_RANGE];
         ocg_internals_log.state_RCS_slope_can_pass_mean_sq_range[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_can_pass[UD_RCS_STATE_SQUARED_RANGE];
         ocg_internals_log.state_RCS_slope_can_pass_mean_rcs[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_can_pass[UD_RCS_STATE_RCS];
         ocg_internals_log.state_RCS_slope_can_pass_mean_sq_rcs[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_can_pass[UD_RCS_STATE_SQUARED_RCS];
         ocg_internals_log.state_RCS_slope_can_pass_rcs_range[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_can_pass[UD_RCS_STATE_RANGE_RCS_PROD];
         
         ocg_internals_log.state_RCS_slope_is_likely_to_pass_num_dets[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_is_likely_to_pass[UD_RCS_STATE_DET_COUNT];
         ocg_internals_log.state_RCS_slope_is_likely_to_pass_mean_range[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_is_likely_to_pass[UD_RCS_STATE_RANGE];
         ocg_internals_log.state_RCS_slope_is_likely_to_pass_mean_sq_range[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_is_likely_to_pass[UD_RCS_STATE_SQUARED_RANGE];
         ocg_internals_log.state_RCS_slope_is_likely_to_pass_mean_rcs[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_is_likely_to_pass[UD_RCS_STATE_RCS];
         ocg_internals_log.state_RCS_slope_is_likely_to_pass_mean_sq_rcs[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_is_likely_to_pass[UD_RCS_STATE_SQUARED_RCS];
         ocg_internals_log.state_RCS_slope_is_likely_to_pass_rcs_range[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_is_likely_to_pass[UD_RCS_STATE_RANGE_RCS_PROD];
         
         ocg_internals_log.state_RCS_slope_can_not_pass_upper_num_dets[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_can_not_pass_upper[UD_RCS_STATE_DET_COUNT];
         ocg_internals_log.state_RCS_slope_can_not_pass_upper_mean_range[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_can_not_pass_upper[UD_RCS_STATE_RANGE];
         ocg_internals_log.state_RCS_slope_can_not_pass_upper_mean_sq_range[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_can_not_pass_upper[UD_RCS_STATE_SQUARED_RANGE];
         ocg_internals_log.state_RCS_slope_can_not_pass_upper_mean_rcs[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_can_not_pass_upper[UD_RCS_STATE_RCS];
         ocg_internals_log.state_RCS_slope_can_not_pass_upper_mean_sq_rcs[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_can_not_pass_upper[UD_RCS_STATE_SQUARED_RCS];
         ocg_internals_log.state_RCS_slope_can_not_pass_upper_rcs_range[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_can_not_pass_upper[UD_RCS_STATE_RANGE_RCS_PROD];
         
         ocg_internals_log.state_RCS_slope_can_not_pass_lower_num_dets[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_can_not_pass_lower[UD_RCS_STATE_DET_COUNT];
         ocg_internals_log.state_RCS_slope_can_not_pass_lower_mean_range[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_can_not_pass_lower[UD_RCS_STATE_RANGE];
         ocg_internals_log.state_RCS_slope_can_not_pass_lower_mean_sq_range[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_can_not_pass_lower[UD_RCS_STATE_SQUARED_RANGE];
         ocg_internals_log.state_RCS_slope_can_not_pass_lower_mean_rcs[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_can_not_pass_lower[UD_RCS_STATE_RCS];
         ocg_internals_log.state_RCS_slope_can_not_pass_lower_mean_sq_rcs[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_can_not_pass_lower[UD_RCS_STATE_SQUARED_RCS];
         ocg_internals_log.state_RCS_slope_can_not_pass_lower_rcs_range[idx] = m_underdrivability.zones[adjusted_idx].state_RCS_slope_can_not_pass_lower[UD_RCS_STATE_RANGE_RCS_PROD];
      }
   }

   void Occupancy_Grid::get_log_output(OCG_Output_Log_T &ocg_output_log) const
   {
      // Underdrivability performs all calculations in Aptiv Vehicle Coordinate System (VCS - origin in center of the front bumper);
      // OCG output interface contains information in Occupancy Grid Coordinate System (OGCS - origin in the center of the bottom edge of the grid);
      // Map signals from Aptiv Vehicle Coordinate System to Grid Coordinate System where neccessary
      ocg_output_log.timestamp = m_timestamp;
      ocg_output_log.f_valid = m_active;
      ocg_output_log.grid_curvature = m_underdrivability.props.grid_curvature;
      ocg_output_log.ogcs_host_rear_axle_position_x = m_underdrivability.props.host_travel_distance + m_underdrivability.props.ogcs_host_rear_axle_initial_position.x;
      ocg_output_log.ogcs_host_rear_axle_position_y = m_underdrivability.props.ogcs_host_rear_axle_initial_position.y;
      ocg_output_log.ogcs_host_rear_axle_position_z = m_underdrivability.props.ogcs_host_rear_axle_initial_position.z;
      ocg_output_log.ogcs_host_rear_axle_position_yaw = m_underdrivability.props.ogcs_host_rear_axle_initial_position.yaw;
      
      // Map underdrivability 1D grid into universal 2D grid interface
      for (uint32_t idx = 0U; idx < NUM_CELLS_X; idx++)
      {
         ocg_output_log.underdrivability_classification_underdrivability_status[idx] = m_underdrivability.zones[idx].cell_classification.underdrivability_status;
         ocg_output_log.underdrive_classification_probs_UNDER_CAN_NOT_PASS_UNDER[idx] = m_underdrivability.zones[idx].cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_NOT_PASS_UNDER];
         ocg_output_log.underdrive_classification_probs_UNDER_IS_LIKELY_TO_PASS_UNDER[idx] = m_underdrivability.zones[idx].cell_classification.probs[UNDERDRIVABLE_STATUS_IS_LIKELY_TO_PASS_UNDER];
         ocg_output_log.underdrive_classification_probs_UNDER_CAN_PASS_UNDER[idx] = m_underdrivability.zones[idx].cell_classification.probs[UNDERDRIVABLE_STATUS_CAN_PASS_UNDER];
         ocg_output_log.underdrive_classification_probs_UNDER_NOT_TO_CONSIDER[idx] = m_underdrivability.zones[idx].cell_classification.probs[UNDERDRIVABLE_STATUS_NOT_TO_CONSIDER];
      }
      
      ocg_output_log.num_cells_x_close = NUM_CELLS_X_CLOSE;
      ocg_output_log.num_cells_x_mid = NUM_CELLS_X_MID;
      ocg_output_log.num_cells_x_far = NUM_CELLS_X_FAR;
      ocg_output_log.num_cells_y = NUM_CELLS_Y;
      
      ocg_output_log.cell_length = CELL_LENGTH;
      ocg_output_log.cell_width = CELL_WIDTH;
      ocg_output_log.cell_width_extension_factor = CELL_WIDTH_EXTENSION_FACTOR;
   }

   void Occupancy_Grid::log_internals(OCG_Internals_Log_T& ocg_internals_log)
   {
      fill_log_header(ocg_internals_log.version, ocg_internals_log.ocg_version_major, ocg_internals_log.ocg_version_minor, ocg_internals_log.ocg_version_patch,
         ocg_internals_log.num_cells_x, ocg_internals_log.num_cells_y, ocg_internals_log.ocg_variant);
      get_log_internals(ocg_internals_log);
   }
   void Occupancy_Grid::log_output(OCG_Output_Log_T& ocg_output_log)
   {
      fill_log_header(ocg_output_log.version, ocg_output_log.ocg_version_major, ocg_output_log.ocg_version_minor, ocg_output_log.ocg_version_patch,
         ocg_output_log.num_cells_x, ocg_output_log.num_cells_y, ocg_output_log.ocg_variant);
      get_log_output(ocg_output_log);
   }

   void Occupancy_Grid::fill_log_header(uint8_t& version, unsigned int& ocg_version_major, unsigned int& ocg_version_minor,
        unsigned int& ocg_version_patch, uint8_t& num_cells_x, uint8_t& num_cells_y, uint8_t& ocg_variant)
   {
      OCGVersion in_ocg_version;
      version = ocg_header_version;
      ocg_variant = OCG_VARIANT_TYPE;
      ocg_version_major = in_ocg_version.getMajor();
      ocg_version_minor = in_ocg_version.getMinor();
      ocg_version_patch = in_ocg_version.getPatch();
      num_cells_x = NUM_CELLS_X;
      num_cells_y = NUM_CELLS_Y;
   }

   bool Occupancy_Grid::initialize_from_internals_log(OCG_Internals_Log_T& ocg_internals_log)
   {
      bool f_success = false;
      if ((ocg_internals_log.ocg_variant == OCG_VARIANT_TYPE)&&
         (ocg_internals_log.num_cells_x== NUM_CELLS_X) &&
         (ocg_internals_log.num_cells_y == NUM_CELLS_Y))
      {
         f_success = true;
         m_timestamp= ocg_internals_log.timestamp;
         m_underdrivability.props.ogcs_host_rear_axle_initial_position = CreateOCGPosition(ocg_internals_log);
         m_underdrivability.props.host_travel_distance= ocg_internals_log.host_travel_distance;
         m_underdrivability.props.circular_buffer_idx= ocg_internals_log.circular_buffer_idx;
         m_underdrivability.props.timestamp_us= ocg_internals_log.timestamp_us;
         m_underdrivability.props.prev_timestamp_us= ocg_internals_log.prev_timestamp_us;
         for (uint32_t idx = 0; idx < NUM_CELLS_X; idx++)
         {
            const uint32_t zone_index = cmn::Calc_Circular_Zone_Idx(m_underdrivability.props.circular_buffer_idx, idx, NUM_CELLS_X);
            m_underdrivability.zones[zone_index].p_height_can_pass= ocg_internals_log.p_height_can_pass[idx];
            m_underdrivability.zones[zone_index].p_height_is_likely_to_pass = ocg_internals_log.p_height_is_likely_to_pass[idx];
            m_underdrivability.zones[zone_index].p_height_can_not_pass_upper = ocg_internals_log.p_height_can_not_pass_upper[idx];
            m_underdrivability.zones[zone_index].p_height_can_not_pass_lower = ocg_internals_log.p_height_can_not_pass_lower[idx];
            m_underdrivability.zones[zone_index].p_RCS_slope_can_pass = ocg_internals_log.p_RCS_slope_can_pass[idx];
            m_underdrivability.zones[zone_index].p_RCS_slope_is_likely_to_pass = ocg_internals_log.p_RCS_slope_is_likely_to_pass[idx];
            m_underdrivability.zones[zone_index].p_RCS_slope_can_not_pass_upper = ocg_internals_log.p_RCS_slope_can_not_pass_upper[idx];
            m_underdrivability.zones[zone_index].p_RCS_slope_can_not_pass_lower = ocg_internals_log.p_RCS_slope_can_not_pass_lower[idx];
            m_underdrivability.zones[idx].p_can_pass = ocg_internals_log.p_can_pass[idx];
            m_underdrivability.zones[idx].p_is_likely_to_pass = ocg_internals_log.p_is_likely_to_pass[idx];
            m_underdrivability.zones[idx].p_can_not_pass = ocg_internals_log.p_can_not_pass[idx];
            
            m_underdrivability.zones[zone_index].state_height_can_pass[UD_HEIGHT_STATE_DET_COUNT] = ocg_internals_log.state_height_can_pass_num_dets[idx];
            m_underdrivability.zones[zone_index].state_height_can_pass[UD_HEIGHT_STATE_ELEVATION_ANGLE] = ocg_internals_log.state_height_can_pass_mean_elevation[idx];
            m_underdrivability.zones[zone_index].state_height_can_pass[UD_HEIGHT_STATE_SQUARED_ELEVATION_ANGLE] = ocg_internals_log.state_height_can_pass_mean_sq_elevation[idx];
            
            m_underdrivability.zones[zone_index].state_height_is_likely_to_pass[UD_HEIGHT_STATE_DET_COUNT] = ocg_internals_log.state_height_is_likely_to_pass_num_dets[idx];
            m_underdrivability.zones[zone_index].state_height_is_likely_to_pass[UD_HEIGHT_STATE_ELEVATION_ANGLE] = ocg_internals_log.state_height_is_likely_to_pass_mean_elevation[idx];
            m_underdrivability.zones[zone_index].state_height_is_likely_to_pass[UD_HEIGHT_STATE_SQUARED_ELEVATION_ANGLE] = ocg_internals_log.state_height_is_likely_to_pass_mean_sq_elevation[idx];
            
            m_underdrivability.zones[zone_index].state_height_can_not_pass_upper[UD_HEIGHT_STATE_DET_COUNT] = ocg_internals_log.state_height_can_not_pass_upper_num_dets[idx];
            m_underdrivability.zones[zone_index].state_height_can_not_pass_upper[UD_HEIGHT_STATE_ELEVATION_ANGLE] = ocg_internals_log.state_height_can_not_pass_upper_mean_elevation[idx];
            m_underdrivability.zones[zone_index].state_height_can_not_pass_upper[UD_HEIGHT_STATE_SQUARED_ELEVATION_ANGLE] = ocg_internals_log.state_height_can_not_pass_upper_mean_sq_elevation[idx];
            
            m_underdrivability.zones[zone_index].state_height_can_not_pass_lower[UD_HEIGHT_STATE_DET_COUNT] = ocg_internals_log.state_height_can_not_pass_lower_num_dets[idx];
            m_underdrivability.zones[zone_index].state_height_can_not_pass_lower[UD_HEIGHT_STATE_ELEVATION_ANGLE] = ocg_internals_log.state_height_can_not_pass_lower_mean_elevation[idx];
            m_underdrivability.zones[zone_index].state_height_can_not_pass_lower[UD_HEIGHT_STATE_SQUARED_ELEVATION_ANGLE] = ocg_internals_log.state_height_can_not_pass_lower_mean_sq_elevation[idx];
            
            m_underdrivability.zones[zone_index].state_RCS_slope_can_pass[UD_RCS_STATE_DET_COUNT] = ocg_internals_log.state_RCS_slope_can_pass_num_dets[idx];
            m_underdrivability.zones[zone_index].state_RCS_slope_can_pass[UD_RCS_STATE_RANGE] = ocg_internals_log.state_RCS_slope_can_pass_mean_range[idx];
            m_underdrivability.zones[zone_index].state_RCS_slope_can_pass[UD_RCS_STATE_SQUARED_RANGE] = ocg_internals_log.state_RCS_slope_can_pass_mean_sq_range[idx];
            m_underdrivability.zones[zone_index].state_RCS_slope_can_pass[UD_RCS_STATE_RCS] = ocg_internals_log.state_RCS_slope_can_pass_mean_rcs[idx];
            m_underdrivability.zones[zone_index].state_RCS_slope_can_pass[UD_RCS_STATE_SQUARED_RCS] = ocg_internals_log.state_RCS_slope_can_pass_mean_sq_rcs[idx];
            m_underdrivability.zones[zone_index].state_RCS_slope_can_pass[UD_RCS_STATE_RANGE_RCS_PROD] = ocg_internals_log.state_RCS_slope_can_pass_rcs_range[idx];
         }
      }
      return f_success;
   }
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif