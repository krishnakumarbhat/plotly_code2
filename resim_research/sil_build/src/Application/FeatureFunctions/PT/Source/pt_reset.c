/**
 * @file pt_reset.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains reset functions for path tracking types.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pt_reset.h"
#include "fbk_macros.h"
#include "ml_vector_2d.h"
#include "pa_reuse.h"
#include "pt_common_functions.h"
#include "pt_debug_interface.h"
#include "pt_persistent_handler.h"
#include <assert.h>

/*===========================================================================*\
* Local Functions Declaration
\*===========================================================================*/

/**
 * @brief resets a single path to its default values.
 *
 * @return void
 *
 * @SRS{SF-1572}
 * @SAE{SF-2918}
 * @SDD{SF-7543}
 * @verification{}
 */
static void Pt_Reset_Path(Pt_Path_T *p_path /**< respective path*/);

/**
 * @brief resets every path-object pair which refers to the given input path
 *
 * @return void
 *
 * @SRS{SF-1572}
 * @SAE{SF-2918}
 * @SDD{SF-7544}
 * @verification{}
 */
static void Pt_Reset_Path_Associations(
   const Pt_Path_T *p_path /**< respective path*/,
   Pt_Best_Path_Obj_Pair_Persistent_T best_path_object_pairs
      [PA_OBJ_NUMBER_OF_OBJECTS] /**< Pairs which might need to be reset when associated to reset candidate*/);

/*===========================================================================*\
* Global Functions Definition
\*===========================================================================*/

void Pt_Reset_All_Paths(Pt_Path_T p_paths[PT_NUMBER_OF_PATHS],
                        Pt_Output_T *p_path_output,
                        Pt_Best_Path_Obj_Pair_Persistent_T best_path_object_pairs[PA_OBJ_NUMBER_OF_OBJECTS])
{
   uint8_t idx;

   /* Asserts */
   assert(NULL != p_paths);

   if (NULL != p_path_output)
   {
      p_path_output->f_pt_operational = FBK_FALSE;
      Pt_Reset_Path_Output(p_path_output);
   }

   for (idx = 0; idx < PT_NUMBER_OF_PATHS; idx++)
   {
      p_paths[idx].path_index = idx;
      Pt_Reset_Path_And_Associations_To_It(&p_paths[idx], best_path_object_pairs, PATH_RESET_PATH_TRACKING_SHUTDOWN);
   }
}

void Pt_Reset_Single_Path_Output(Pt_Path_Object_Pair_Output_T *p_path_obj_pair_output, Pt_Nearest_Path_T *p_nearest_path_output)
{
   /* Assert */
   assert(NULL != p_path_obj_pair_output);
   assert(NULL != p_nearest_path_output);

   p_nearest_path_output->track_idx_nearest_path         = PT_DEFAULT_MATCH_INDEX;
   p_nearest_path_output->range_vcs_proj_to_path_segment = PT_HIGH_DISTANCE_DEFAULT_VAL;
   p_nearest_path_output->segment_heading_diff           = FBK_ZERO_F;
   p_path_obj_pair_output->track_match                   = PT_DEFAULT_MATCH_INDEX;
   p_path_obj_pair_output->track_match_last_cycle        = PT_DEFAULT_MATCH_INDEX;
   p_path_obj_pair_output->track_match_age               = FBK_ZERO_UINT;
   p_path_obj_pair_output->range_at_zero                 = PT_HIGH_DISTANCE_DEFAULT_VAL;
   p_path_obj_pair_output->range_at_host_edge            = PT_HIGH_DISTANCE_DEFAULT_VAL;
   p_path_obj_pair_output->range_to_current_path_part    = PT_HIGH_DISTANCE_DEFAULT_VAL;
   p_path_obj_pair_output->length_of_trajectory          = -FBK_ONE_F;
   p_path_obj_pair_output->path_heading                  = FBK_ZERO_F;
   p_path_obj_pair_output->path_direction                = PATH_DIRECTION_NONE;
   p_path_obj_pair_output->path_state                    = PATH_STATUS_DEFAULT;
}

void Pt_Reset_Path_Output(Pt_Output_T *p_pt_output)
{
   uint8_t obj_loop_index;

   /* Asserts */
   assert(NULL != p_pt_output);

   for (obj_loop_index = FBK_ZERO_UINT; obj_loop_index < PA_OBJ_NUMBER_OF_OBJECTS; obj_loop_index++)
   {
      Pt_Reset_Single_Path_Output(&(p_pt_output->path_obj_pair_output[obj_loop_index]),
                                  &(p_pt_output->nearest_path_output[obj_loop_index]));
   }
}

void Pt_Reset_Path_And_Associations_To_It(Pt_Path_T *p_path,
                                          Pt_Best_Path_Obj_Pair_Persistent_T best_path_object_pairs[PA_OBJ_NUMBER_OF_OBJECTS],
                                          const Pt_Path_Reset_Reason_T path_reset_reason)
{
   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != best_path_object_pairs);

   if (PATH_RESET_NO_RESET != path_reset_reason)
   {
      /* Reset path associations */
      Pt_Reset_Path_Associations(p_path, best_path_object_pairs);

      /* Reset path */
      Pt_Reset_Path(p_path);

      /* Store reset reason for debug info */
      Binary_Pt_Debug_Pass_Path_Reset_Reason(p_path->path_index, path_reset_reason);
   }
}

/*===========================================================================*\
* Local Functions Definition
\*===========================================================================*/

static void Pt_Reset_Path(Pt_Path_T *p_path)
{
   uint8_t i;

   /* Assert */
   assert(NULL != p_path);

   p_path->obj_curr_used_for_path_build.id  = FBK_ZERO_UINT;
   p_path->obj_curr_used_for_path_build.age = FBK_ZERO_UINT;
   p_path->first_p                          = PT_DEFAULT_DISCR_BORDER;
   p_path->last_p                           = PT_DEFAULT_DISCR_BORDER;
   p_path->path_state                       = PATH_STATUS_DEFAULT;
   p_path->direction                        = PATH_DIRECTION_NONE;
   p_path->max_speed                        = FBK_ZERO_F;
   p_path->path_age                         = FBK_ZERO_UINT;
   p_path->num_groupings                    = FBK_ZERO_UINT;
   p_path->first                            = Create_2d_Vector_Origin();
   p_path->last_mat                         = Create_2d_Vector_Origin();
   p_path->new_path_point_status            = PATH_POINT_NEW_FIRST;
   p_path->path_border_status               = PATH_BOTH_BORDERS_NEW;

   for (i = 0; i < PT_NUM_GRID_POINTS; i++)
   {
      p_path->path_points[i] = PT_PATH_POINTS_DEFAULT_VAL;
   }
}

static void Pt_Reset_Path_Associations(const Pt_Path_T *p_path,
                                       Pt_Best_Path_Obj_Pair_Persistent_T best_path_object_pairs[PA_OBJ_NUMBER_OF_OBJECTS])
{
   uint8_t i;

   /* Asserts */
   assert(NULL != p_path);
   assert(NULL != best_path_object_pairs);

   for (i = 0; i < PA_OBJ_NUMBER_OF_OBJECTS; i++)
   {
      if (p_path->path_index == best_path_object_pairs[i].path_index)
      {
         Pt_Reset_Single_Matching_Pair(&(best_path_object_pairs[i]));
      }
   }
}
