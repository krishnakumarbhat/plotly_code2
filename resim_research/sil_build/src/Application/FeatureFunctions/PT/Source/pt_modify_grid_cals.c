/**
 * @file pt_modify_grid_cals.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module adapts calibrations which are directly dependent in the absolute amount
 * of grid points automatically.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pt_modify_grid_cals.h"
#include "fbk_macros.h"
#include "ml_checked_rounding.h"
#include "ml_math.h"
#include <assert.h>

/*===========================================================================*\
* Global Functions Definitions
\*===========================================================================*/
void Pt_Set_Num_Grid_Pts_Dependend_Cals(const Pt_Core_Calibration_T *p_cals,
                                        const float32_T grid_array[PT_NUM_GRID_POINTS],
                                        Pt_Num_Grid_Pts_Dep_Cals_T *p_num_grid_pts_dep_cals)
{
   float32_T granularity_grid_pt_id_arr;
   uint8_t num_of_points_added;
   uint8_t half_of_num_pts_added;

   /* Asserts */
   assert(NULL != p_cals);
   assert(NULL != grid_array);
   assert(NULL != p_num_grid_pts_dep_cals);

   if (Fbk_Abs_F(grid_array[PT_LOWEST_GRID_POINT_INDEX] + p_cals->k_pt_default_range_of_tracking_zone) < EPSILON) /*Default*/
   {
      p_num_grid_pts_dep_cals->k_pt_find_max_diff_path_points            = p_cals->k_pt_find_max_diff_path_points;
      p_num_grid_pts_dep_cals->k_pt_find_min_diff_path_points            = p_cals->k_pt_find_min_diff_path_points;
      p_num_grid_pts_dep_cals->k_pt_kill_lane_change_min_diff_path_point = p_cals->k_pt_kill_lane_change_min_diff_path_point;
      p_num_grid_pts_dep_cals->k_pt_max_diff_num_path_point              = p_cals->k_pt_max_diff_num_path_point;
      p_num_grid_pts_dep_cals->k_pt_min_path_length_after_rot            = p_cals->k_pt_min_path_length_after_rot;
      p_num_grid_pts_dep_cals->k_pt_min_path_length_proc_lane_change     = p_cals->k_pt_min_path_length_proc_lane_change;
      p_num_grid_pts_dep_cals->k_pt_group_path_min_overlap_count         = p_cals->k_pt_group_path_min_overlap_count;
      p_num_grid_pts_dep_cals->k_pt_group_path_min_overlap_count_ad      = p_cals->k_pt_group_path_min_overlap_count_ad;
      p_num_grid_pts_dep_cals->k_pt_min_diff_num_path_points             = p_cals->k_pt_min_diff_num_path_points;
      p_num_grid_pts_dep_cals->k_pt_find_max_long_posn                   = p_cals->k_pt_find_max_long_posn;
      p_num_grid_pts_dep_cals->k_pt_find_max_lat_posn                    = p_cals->k_pt_find_max_lat_posn;
   }
   else
   {
      granularity_grid_pt_id_arr =
         grid_array[PT_LOWEST_GRID_POINT_INDEX + PT_SINGLE_GRID_POINT_OFFSET] - grid_array[PT_LOWEST_GRID_POINT_INDEX];
      num_of_points_added = Roundf_Checked_Uint8(
         (grid_array[PT_HIGHEST_GRID_POINT_INDEX] - p_cals->k_pt_default_range_of_tracking_zone) / granularity_grid_pt_id_arr);
      half_of_num_pts_added = Roundf_Checked_Uint8(0.5f * (float32_T) num_of_points_added);

      p_num_grid_pts_dep_cals->k_pt_find_max_diff_path_points =
         (uint8_t) (p_cals->k_pt_find_max_diff_path_points + half_of_num_pts_added);
      p_num_grid_pts_dep_cals->k_pt_find_min_diff_path_points =
         (uint8_t) (p_cals->k_pt_find_min_diff_path_points + half_of_num_pts_added);
      p_num_grid_pts_dep_cals->k_pt_kill_lane_change_min_diff_path_point =
         (uint8_t) (p_cals->k_pt_kill_lane_change_min_diff_path_point + num_of_points_added);
      p_num_grid_pts_dep_cals->k_pt_max_diff_num_path_point = (uint8_t) (p_cals->k_pt_max_diff_num_path_point + half_of_num_pts_added);
      p_num_grid_pts_dep_cals->k_pt_min_path_length_after_rot =
         (uint8_t) (p_cals->k_pt_min_path_length_after_rot + num_of_points_added);
      p_num_grid_pts_dep_cals->k_pt_min_path_length_proc_lane_change =
         (uint8_t) (p_cals->k_pt_min_path_length_proc_lane_change + num_of_points_added);
      p_num_grid_pts_dep_cals->k_pt_group_path_min_overlap_count =
         (uint8_t) (p_cals->k_pt_group_path_min_overlap_count + half_of_num_pts_added);
      p_num_grid_pts_dep_cals->k_pt_group_path_min_overlap_count_ad =
         (uint8_t) (p_cals->k_pt_group_path_min_overlap_count_ad + half_of_num_pts_added);
      p_num_grid_pts_dep_cals->k_pt_min_diff_num_path_points =
         (uint8_t) (p_cals->k_pt_min_diff_num_path_points + half_of_num_pts_added);
      p_num_grid_pts_dep_cals->k_pt_find_max_long_posn = grid_array[PT_HIGHEST_GRID_POINT_INDEX];
      p_num_grid_pts_dep_cals->k_pt_find_max_lat_posn  = grid_array[PT_HIGHEST_GRID_POINT_INDEX];
   }
}
