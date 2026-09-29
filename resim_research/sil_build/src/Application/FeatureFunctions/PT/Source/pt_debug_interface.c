/**
 * @file pt_debug_interface.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the functions for copying internal data to the debug output interface.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#include "pt_debug_interface.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "pa_vehicle_in.h"
#include "pt_output_t.h"
#include "pt_reset.h"
#include <assert.h>
#include <string.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
 * File Scope variables
\*===========================================================================*/

static Pt_Debug_Data_T pt_debug_data;

/*===========================================================================*\
* Debug interface
\*===========================================================================*/

Pt_Debug_Data_T *Pt_Get_Debug_Data(void)
{
   return (&pt_debug_data);
}

/*===========================================================================*\
* Helper functions to fill and reset debug data
\*===========================================================================*/

void Pt_Debug_Reset_Data(void)
{
   memset(&pt_debug_data, 0, sizeof(Pt_Debug_Data_T));
}

void Pt_Debug_Pass_General_Data(const Pt_Input_T *p_pt_core_input,
                                const Pt_Output_T *p_pt_core_output,
                                const Pt_Persistent_T *p_pt_persistent,
                                const Pt_Core_Calibration_T *p_pt_cal)
{
   /* Assert that all passed pointers are valid. */
   assert(NULL != p_pt_core_input);
   assert(NULL != p_pt_core_output);
   assert(NULL != p_pt_persistent);
   assert(NULL != p_pt_cal);

   /* Copy data from internal interfaces to debug output interface. */
   pt_debug_data.pt_core_input  = *p_pt_core_input;
   pt_debug_data.pt_core_output = *p_pt_core_output;
   pt_debug_data.pt_persistent  = *p_pt_persistent;
   memcpy(&pt_debug_data.pt_calibration, p_pt_cal, sizeof(Pt_Core_Calibration_T));
}

void Pt_Debug_Pass_Sw_Version(const uint16_t pt_sw_major_version, const uint16_t pt_sw_minor_version)
{
   /* Store version from FF iface. */
   pt_debug_data.pt_version.pt_sw_major_version = pt_sw_major_version;
   pt_debug_data.pt_version.pt_sw_minor_version = pt_sw_minor_version;
}

void Pt_Debug_Pass_Path_Data(const Pt_Path_T p_paths[PT_NUMBER_OF_PATHS])
{
   uint8_t i;
   uint8_t number_of_paths          = FBK_ZERO_UINT;
   uint8_t number_of_finished_paths = FBK_ZERO_UINT;

   /* Assert */
   assert(NULL != p_paths);

   for (i = FBK_ZERO_UINT; i < PT_NUMBER_OF_PATHS; i++)
   {
      if (PATH_STATUS_DEFAULT != p_paths[i].path_state)
      {
         number_of_paths = (uint8_t) (number_of_paths + FBK_ONE_UINT);
         if (PATH_STATUS_CREATION != p_paths[i].path_state)
         {
            number_of_finished_paths = (uint8_t) (number_of_finished_paths + FBK_ONE_UINT);
         }
      }
   }

   pt_debug_data.pt_debug_output.pt_paths.number_of_paths          = number_of_paths;
   pt_debug_data.pt_debug_output.pt_paths.number_of_finished_paths = number_of_finished_paths;

   for (i = FBK_ZERO_UINT; i < PT_NUMBER_OF_PATHS; i++)
   {
      pt_debug_data.pt_debug_output.pt_paths.paths[i] = p_paths[i];
   }
}

void Pt_Debug_Pass_Path_Size_Change_Flags(const boolean_T lower_size_flag,
                                          const boolean_T upper_size_flag,
                                          uint8_t path_index,
                                          boolean_T f_debug_shrinkage)
{
   /* Assert */
   assert(PT_NUMBER_OF_PATHS > path_index);

   if (Fbk_Is_True(f_debug_shrinkage))
   {
      pt_debug_data.pt_debug_output.Pt_Debug_Rotation_Data.f_path_shrinked_upper_border[path_index] = upper_size_flag;
      pt_debug_data.pt_debug_output.Pt_Debug_Rotation_Data.f_path_shrinked_lower_border[path_index] = lower_size_flag;
   }
   else
   {
      pt_debug_data.pt_debug_output.Pt_Debug_Rotation_Data.f_path_extended_upper_border[path_index] = upper_size_flag;
      pt_debug_data.pt_debug_output.Pt_Debug_Rotation_Data.f_path_extended_lower_border[path_index] = lower_size_flag;
   }
}

void Pt_Debug_Pass_Best_Pair_Conf_To_Intern(const Pt_Path_Obj_Pair_Confidence_T *p_path_obj_pair_confidence,
                                            const uint32_t object_index,
                                            const uint8_t path_index)
{
   uint8_t idx;

   /* Assert */
   assert(NULL != p_path_obj_pair_confidence);
   assert(PT_NUMBER_OF_PATHS > path_index);
   assert(PA_OBJ_NUMBER_OF_OBJECTS > object_index);

   pt_debug_data.pt_debug_output.Pt_Debug_Internal.best_dist_border_to_isect_confidence[object_index] =
      p_path_obj_pair_confidence->dist_border_to_isect_confidence;
   pt_debug_data.pt_debug_output.Pt_Debug_Internal.best_dist_obj_to_border_confidence[object_index] =
      p_path_obj_pair_confidence->dist_obj_to_border_confidence;
   pt_debug_data.pt_debug_output.Pt_Debug_Internal.best_dist_obj_to_path_confidence[object_index] =
      p_path_obj_pair_confidence->dist_obj_to_path_confidence;
   pt_debug_data.pt_debug_output.Pt_Debug_Internal.best_similarity_trail_path_confidence[object_index] =
      p_path_obj_pair_confidence->similarity_trail_path_confidence;
   pt_debug_data.pt_debug_output.Pt_Debug_Internal.best_heading_diff_confidence[object_index] =
      p_path_obj_pair_confidence->heading_difference_confidence;
   pt_debug_data.pt_debug_output.Pt_Debug_Internal.best_path_index[object_index] = path_index;

   for (idx = FBK_ZERO_UINT; idx < PT_NUM_HEADING_SEGMENTS; idx++)
   {
      pt_debug_data.pt_debug_output.Pt_Debug_Internal.best_heading_segment_confidence[object_index][idx] =
         pt_debug_data.pt_debug_output.Pt_Debug_Heading_Diff_Metrics.heading_segment_confidence[object_index][path_index][idx];
   }

   pt_debug_data.pt_debug_output.Pt_Debug_Internal.best_confidence_factor_total[object_index] =
      p_path_obj_pair_confidence->confidence_factor_total;
}

void Pt_Debug_Pass_Pair_Confidence_Metrics_To_Intern(const Pt_Path_Obj_Pair_Confidence_T *p_path_obj_pair_confidence,
                                                     const Pt_Path_Obj_Pair_Info_T *p_path_obj_info,
                                                     const uint32_t object_index,
                                                     const uint8_t path_index)
{
   uint8_t idx;

   /* Asserts */
   assert(NULL != p_path_obj_pair_confidence);
   assert(NULL != p_path_obj_info);
   assert(PT_NUMBER_OF_PATHS > path_index);
   assert(PA_OBJ_NUMBER_OF_OBJECTS > object_index);

   pt_debug_data.pt_debug_output.Pt_Debug_Internal.dist_obj_to_border_confidence[object_index][path_index] =
      p_path_obj_pair_confidence->dist_obj_to_border_confidence;
   pt_debug_data.pt_debug_output.Pt_Debug_Internal.dist_border_to_isect_confidence[object_index][path_index] =
      p_path_obj_pair_confidence->dist_border_to_isect_confidence;
   pt_debug_data.pt_debug_output.Pt_Debug_Internal.dist_obj_to_path_confidence[object_index][path_index] =
      p_path_obj_pair_confidence->dist_obj_to_path_confidence;
   pt_debug_data.pt_debug_output.Pt_Debug_Internal.heading_diff_confidence[object_index][path_index] =
      p_path_obj_pair_confidence->heading_difference_confidence;
   pt_debug_data.pt_debug_output.Pt_Debug_Internal.confidence_factor_total[object_index][path_index] =
      p_path_obj_pair_confidence->confidence_factor_total;
   pt_debug_data.pt_debug_output.Pt_Debug_Internal.similarity_trail_path_confidence[object_index][path_index] =
      p_path_obj_pair_confidence->similarity_trail_path_confidence;

   pt_debug_data.pt_debug_output.Pt_Debug_Internal.closest_to_successive_pt_heading[object_index][path_index] =
      p_path_obj_info->closest_to_successive_pt_heading;
   pt_debug_data.pt_debug_output.Pt_Debug_Internal.rad_object_to_path_dist[object_index][path_index] =
      p_path_obj_info->rad_object_to_path_dist;
   pt_debug_data.pt_debug_output.Pt_Debug_Internal.relevant_dist_comp_matching[object_index][path_index] =
      p_path_obj_info->relevant_dist_comp_matching;
   pt_debug_data.pt_debug_output.Pt_Debug_Internal.weighted_mean_diff_last_points[object_index][path_index] =
      p_path_obj_info->weighted_mean_diff_last_points;

   pt_debug_data.pt_debug_output.Pt_Debug_Internal.dist_border_to_isect[object_index][path_index] =
      (int8_t) p_path_obj_info->border_info.dist_border_to_mid;
   pt_debug_data.pt_debug_output.Pt_Debug_Internal.dist_border_to_obj[object_index][path_index] =
      (int8_t) p_path_obj_info->border_info.dist_border_to_obj;

   for (idx = FBK_ZERO_UINT; idx < PT_NUM_HEADING_SEGMENTS; idx++)
   {
      pt_debug_data.pt_debug_output.Pt_Debug_Internal.heading_diff_pt_segment[object_index][path_index][idx] =
         pt_debug_data.pt_debug_output.Pt_Debug_Heading_Diff_Metrics.heading_diff_pt_segment[object_index][path_index][idx];
      pt_debug_data.pt_debug_output.Pt_Debug_Internal.heading_segment_confidence[object_index][path_index][idx] =
         pt_debug_data.pt_debug_output.Pt_Debug_Heading_Diff_Metrics.heading_segment_confidence[object_index][path_index][idx];
      pt_debug_data.pt_debug_output.Pt_Debug_Internal.weights_to_heading_segments[object_index][path_index][idx] =
         pt_debug_data.pt_debug_output.Pt_Debug_Heading_Diff_Metrics.weights_to_heading_segments[object_index][path_index][idx];
   }
}

void Pt_Debug_Pass_Heading_Diff_Segments_Metrics(const Pt_Path_Obj_Pair_Info_T *p_path_obj_pair_info,
                                                 const float32_T heading_segment_confidence[PT_NUM_HEADING_SEGMENTS],
                                                 const float32_T weights_to_heading_segments[PT_NUM_HEADING_SEGMENTS],
                                                 const uint32_t object_index,
                                                 const uint8_t path_index)
{
   uint8_t idx;

   /* Asserts */
   assert(NULL != p_path_obj_pair_info);
   assert(PT_NUMBER_OF_PATHS > path_index);
   assert(PA_OBJ_NUMBER_OF_OBJECTS > object_index);

   for (idx = FBK_ZERO_UINT; idx < PT_NUM_HEADING_SEGMENTS; idx++)
   {
      pt_debug_data.pt_debug_output.Pt_Debug_Heading_Diff_Metrics.heading_diff_pt_segment[object_index][path_index][idx] =
         p_path_obj_pair_info->heading_diff_pt_segment[idx];
      pt_debug_data.pt_debug_output.Pt_Debug_Heading_Diff_Metrics.heading_segment_confidence[object_index][path_index][idx] =
         heading_segment_confidence[idx];
      pt_debug_data.pt_debug_output.Pt_Debug_Heading_Diff_Metrics.weights_to_heading_segments[object_index][path_index][idx] =
         weights_to_heading_segments[idx];
   }
}

void Pt_Debug_Pass_Match_Information(const boolean_T f_path_change_due_to_grouping,
                                     const boolean_T f_path_change_due_to_nearer_path,
                                     const boolean_T f_prev_best_pair_updates_confidence,
                                     const float32_T path_change_matching_hysteresis,
                                     const uint8_t obj_index,
                                     const Pt_Core_Calibration_T *p_cals)
{
   Path_Match_Reason_T reason_to_apply = PATH_MATCH_NO_MATCH;

   /* Asserts */
   assert(NULL != p_cals);
   assert(PA_OBJ_NUMBER_OF_OBJECTS > obj_index);

   if (f_path_change_due_to_nearer_path)
   {
      reason_to_apply = PATH_MATCH_BY_NEAREST_PATH;
   }
   else if (f_path_change_due_to_grouping && (path_change_matching_hysteresis < FBK_ZERO_F))
   {
      reason_to_apply = PATH_MATCH_BY_GROUPING_CURRENT_PATH_BETTER_MATCH;
   }
   else if (f_path_change_due_to_grouping
            && (Fbk_Abs_F(path_change_matching_hysteresis - p_cals->k_pt_path_change_match_hyst_more_established) < EPSILON))
   {
      reason_to_apply = PATH_MATCH_BY_GROUPING_PREVIOUS_PATH_BETTER_MATCH;
   }
   else if (f_path_change_due_to_grouping)
   {
      reason_to_apply = PATH_MATCH_BY_CONFIDENCE;
   }
   else if (f_prev_best_pair_updates_confidence)
   {
      reason_to_apply = PATH_MATCH_PREVIOUS_MATCH_UPDATES_CONFIDENCE;
   }
   else
   {
      assert(FBK_FALSE && "match reason case is undefined");
   }

   pt_debug_data.pt_debug_output.Pt_Debug_Match_Reason[obj_index] = reason_to_apply;
}

void Pt_Debug_Pass_Path_Reset_Reason(const uint8_t path_index, const Pt_Path_Reset_Reason_T path_reset_reason)
{
   /* Assert */
   assert(PT_NUMBER_OF_PATHS > path_index);

   /* Store the reset reason for given path index */
   pt_debug_data.pt_debug_output.Pt_Debug_Reset_Reason[path_index] = path_reset_reason;
}

#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */
