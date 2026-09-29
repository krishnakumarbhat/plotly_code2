/*================================================================================*\
 * Copyright 2023 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/

// Add pragmas to throw error if struct is padded
#if defined _MSC_VER
#pragma warning(push)
#pragma warning(error : 4820)
#elif 0
#pragma GCC diagnostic push
#pragma GCC diagnostic error "-Wpadded"
#endif

static const uint32_t OCG_NUM_CELLS_INTERNALS = 100;
static_assert(OCG_NUM_CELLS_INTERNALS >= ocg::NUM_CELLS_X * ocg::NUM_CELLS_Y, "OCG_NUM_CELLS_INTERNALS is greater than NUM_CELLS_X * NUM_CELLS_Y");

static const uint32_t OCG_INTERNALS_LOG_STREAM_NUM = 170;
static const uint32_t OCG_INTERNALS_LOG_STREAM_VERSION = 1;

typedef struct OCG_Internals_Log_Tag
{
   double timestamp;
   uint64_t timestamp_us;
   uint64_t prev_timestamp_us;
   
   float state_height_can_pass_mean_elevation[OCG_NUM_CELLS_INTERNALS];
   float state_height_can_pass_mean_sq_elevation[OCG_NUM_CELLS_INTERNALS];
   float state_height_can_pass_num_dets[OCG_NUM_CELLS_INTERNALS];
   
   float state_height_is_likely_to_pass_mean_elevation[OCG_NUM_CELLS_INTERNALS];
   float state_height_is_likely_to_pass_mean_sq_elevation[OCG_NUM_CELLS_INTERNALS];
   float state_height_is_likely_to_pass_num_dets[OCG_NUM_CELLS_INTERNALS];
   
   float state_height_can_not_pass_upper_mean_elevation[OCG_NUM_CELLS_INTERNALS];
   float state_height_can_not_pass_upper_mean_sq_elevation[OCG_NUM_CELLS_INTERNALS];
   float state_height_can_not_pass_upper_num_dets[OCG_NUM_CELLS_INTERNALS];
   
   float state_height_can_not_pass_lower_mean_elevation[OCG_NUM_CELLS_INTERNALS];
   float state_height_can_not_pass_lower_mean_sq_elevation[OCG_NUM_CELLS_INTERNALS];
   float state_height_can_not_pass_lower_num_dets[OCG_NUM_CELLS_INTERNALS];
   
   float state_RCS_slope_can_pass_mean_range[OCG_NUM_CELLS_INTERNALS];
   float state_RCS_slope_can_pass_mean_sq_range[OCG_NUM_CELLS_INTERNALS];
   float state_RCS_slope_can_pass_mean_rcs[OCG_NUM_CELLS_INTERNALS];
   float state_RCS_slope_can_pass_mean_sq_rcs[OCG_NUM_CELLS_INTERNALS];
   float state_RCS_slope_can_pass_rcs_range[OCG_NUM_CELLS_INTERNALS];
   float state_RCS_slope_can_pass_num_dets[OCG_NUM_CELLS_INTERNALS];
   
   float state_RCS_slope_is_likely_to_pass_mean_range[OCG_NUM_CELLS_INTERNALS];
   float state_RCS_slope_is_likely_to_pass_mean_sq_range[OCG_NUM_CELLS_INTERNALS];
   float state_RCS_slope_is_likely_to_pass_mean_rcs[OCG_NUM_CELLS_INTERNALS];
   float state_RCS_slope_is_likely_to_pass_mean_sq_rcs[OCG_NUM_CELLS_INTERNALS];
   float state_RCS_slope_is_likely_to_pass_rcs_range[OCG_NUM_CELLS_INTERNALS];
   float state_RCS_slope_is_likely_to_pass_num_dets[OCG_NUM_CELLS_INTERNALS];
   
   float state_RCS_slope_can_not_pass_upper_mean_range[OCG_NUM_CELLS_INTERNALS];
   float state_RCS_slope_can_not_pass_upper_mean_sq_range[OCG_NUM_CELLS_INTERNALS];
   float state_RCS_slope_can_not_pass_upper_mean_rcs[OCG_NUM_CELLS_INTERNALS];
   float state_RCS_slope_can_not_pass_upper_mean_sq_rcs[OCG_NUM_CELLS_INTERNALS];
   float state_RCS_slope_can_not_pass_upper_rcs_range[OCG_NUM_CELLS_INTERNALS];
   float state_RCS_slope_can_not_pass_upper_num_dets[OCG_NUM_CELLS_INTERNALS];
   
   float state_RCS_slope_can_not_pass_lower_mean_range[OCG_NUM_CELLS_INTERNALS];
   float state_RCS_slope_can_not_pass_lower_mean_sq_range[OCG_NUM_CELLS_INTERNALS];
   float state_RCS_slope_can_not_pass_lower_mean_rcs[OCG_NUM_CELLS_INTERNALS];
   float state_RCS_slope_can_not_pass_lower_mean_sq_rcs[OCG_NUM_CELLS_INTERNALS];
   float state_RCS_slope_can_not_pass_lower_rcs_range[OCG_NUM_CELLS_INTERNALS];
   float state_RCS_slope_can_not_pass_lower_num_dets[OCG_NUM_CELLS_INTERNALS];
   
   float p_height_can_pass[OCG_NUM_CELLS_INTERNALS];
   float p_height_is_likely_to_pass[OCG_NUM_CELLS_INTERNALS];
   float p_height_can_not_pass_upper[OCG_NUM_CELLS_INTERNALS];
   float p_height_can_not_pass_lower[OCG_NUM_CELLS_INTERNALS];
   
   float p_RCS_slope_can_pass[OCG_NUM_CELLS_INTERNALS];
   float p_RCS_slope_is_likely_to_pass[OCG_NUM_CELLS_INTERNALS];
   float p_RCS_slope_can_not_pass_upper[OCG_NUM_CELLS_INTERNALS];
   float p_RCS_slope_can_not_pass_lower[OCG_NUM_CELLS_INTERNALS];
   
   float p_can_pass[OCG_NUM_CELLS_INTERNALS];
   float p_is_likely_to_pass[OCG_NUM_CELLS_INTERNALS];
   float p_can_not_pass[OCG_NUM_CELLS_INTERNALS];
   
   uint32_t iteration_index;
   
   unsigned int ocg_version_major;
   unsigned int ocg_version_minor;
   unsigned int ocg_version_patch;
   float ogcs_host_rear_axle_initial_position_x;
   float ogcs_host_rear_axle_initial_position_y;
   float ogcs_host_rear_axle_initial_position_z;
   float ogcs_host_rear_axle_initial_position_yaw;
   float host_travel_distance;
   uint16_t circular_buffer_idx;
   uint8_t version;
   uint8_t num_cells_x;
   uint8_t num_cells_y;
   uint8_t ocg_variant;
   uint8_t reserved[6];
} OCG_Internals_Log_T;

#if defined _MSC_VER
#pragma warning(pop)
#elif 0
#pragma GCC diagnostic pop
#endif