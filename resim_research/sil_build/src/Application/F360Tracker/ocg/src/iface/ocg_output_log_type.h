/*================================================================================*\
 * Copyright 2023 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/
#ifndef OCG_OUTPUT_LOG_TYPE_H
#define OCG_OUTPUT_LOG_TYPE_H

#include "ocg_constants.h"

// Add pragmas to throw error if struct is padded
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(error : 4820)
#elif 0
#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wpadded"
#endif

static const uint32_t OCG_NUM_CELLS_OUTPUT = 100;
static_assert(OCG_NUM_CELLS_OUTPUT >= ocg::NUM_CELLS_X * ocg::NUM_CELLS_Y, "OCG_NUM_CELLS_OUTPUT is greater than NUM_CELLS_X * NUM_CELLS_Y");

static const uint32_t OCG_OUTPUT_LOG_STREAM_NUM = 171;
static const uint32_t OCG_OUTPUT_LOG_STREAM_VERSION = 1;

typedef struct OCG_Output_Log_Tag
{
   double timestamp;
   
   unsigned int ocg_version_major;
   unsigned int ocg_version_minor;
   unsigned int ocg_version_patch;
   
   float underdrive_classification_probs_UNDER_CAN_NOT_PASS_UNDER[OCG_NUM_CELLS_OUTPUT];
   float underdrive_classification_probs_UNDER_IS_LIKELY_TO_PASS_UNDER[OCG_NUM_CELLS_OUTPUT];
   float underdrive_classification_probs_UNDER_CAN_PASS_UNDER[OCG_NUM_CELLS_OUTPUT];
   float underdrive_classification_probs_UNDER_NOT_TO_CONSIDER[OCG_NUM_CELLS_OUTPUT];
   uint8_t underdrivability_classification_underdrivability_status[OCG_NUM_CELLS_OUTPUT];
   
   float cell_length;
   float cell_width;
   float cell_width_extension_factor;
   
   float ogcs_host_rear_axle_position_x;
   float ogcs_host_rear_axle_position_y;
   float ogcs_host_rear_axle_position_z;
   float ogcs_host_rear_axle_position_yaw;
   
   float grid_curvature;
   
   uint32_t iteration_index;
   
   uint16_t num_cells_x_far;
   uint16_t num_cells_x_mid;
   uint16_t num_cells_x_close;
   uint8_t num_cells_y;
   
   uint8_t version;
   uint8_t num_cells_x;
   uint8_t ocg_variant;
   
   bool f_valid;
   
   uint8_t reserved;
} OCG_Output_Log_T;

#if defined _MSC_VER
#pragma warning(pop)
#elif 0
#pragma GCC diagnostic pop
#endif

#endif
