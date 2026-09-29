#ifndef PT_INPUT_T_H
#define PT_INPUT_T_H
/**
 * @file pt_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the input type definition of path tracking algorithm. This type definition is
 * part of the interface.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

/*Feature Building Kit Includes*/
#include "fbk_output.h"
#include "pa_data.h"
#include "pt_constants.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Summarizes calibrations which are dependent on the amount of grid points.
 */
typedef struct
{
   float32_T k_pt_find_max_long_posn;
   float32_T k_pt_find_max_lat_posn;
   uint8_t k_pt_kill_lane_change_min_diff_path_point;
   uint8_t k_pt_min_path_length_proc_lane_change;
   uint8_t k_pt_min_path_length_after_rot;
   uint8_t k_pt_max_diff_num_path_point;
   uint8_t k_pt_find_min_diff_path_points;
   uint8_t k_pt_find_max_diff_path_points;
   uint8_t k_pt_group_path_min_overlap_count;
   uint8_t k_pt_group_path_min_overlap_count_ad;
   uint8_t k_pt_min_diff_num_path_points;
} Pt_Num_Grid_Pts_Dep_Cals_T;

/**
 * @brief Struct summarizing the path tracking input
 *
 * @SAE{SF-2914}
 * @SDD{SF-7276}
 */
typedef struct
{
   Pt_Num_Grid_Pts_Dep_Cals_T Num_Grid_Pts_Dep_Cals;
   float32_T grid_pt_array[PT_NUM_GRID_POINTS];
   const Fbk_Output_T *p_fbk_output;
} Pt_Input_T;

#endif
