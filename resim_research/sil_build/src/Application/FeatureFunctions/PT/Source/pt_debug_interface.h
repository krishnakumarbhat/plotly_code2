#ifndef PT_DEBUG_INTERFACE_H
#define PT_DEBUG_INTERFACE_H

/**
 * @file pt_debug_interface.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations to copy internal data to PT debug output.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#include "fbk_field_of_interest.h"
#include "pa_data.h"
#include "pt_core_calibration_t.h"
#include "pt_input_t.h"
#include "pt_output_t.h"
#include "pt_persistent_t.h"
#include "pt_types.h"

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
* typedefs
\*===========================================================================*/

typedef enum
{
   PATH_MATCH_NO_MATCH                              = (0),  /**< No path match happened */
   PATH_MATCH_BY_CONFIDENCE                         = (1),  /**< Match happened only by confidence.*/
   PATH_MATCH_BY_GROUPING_CURRENT_PATH_BETTER_MATCH = (2),  /**< Current path index is more established than previous best match*/
   PATH_MATCH_BY_GROUPING_PREVIOUS_PATH_BETTER_MATCH = (3), /**< Previous path index is more established than previous best match */
   PATH_MATCH_BY_NEAREST_PATH                        = (4), /**< Nearest path is matched*/
   PATH_MATCH_PREVIOUS_MATCH_UPDATES_CONFIDENCE      = (5)  /**< Previous matched path updates its confidence*/
} Path_Match_Reason_T;

typedef struct
{
   /* Debug information for the best path matches of an object */
   float32_T best_dist_obj_to_border_confidence[PA_OBJ_NUMBER_OF_OBJECTS];
   float32_T best_dist_border_to_isect_confidence[PA_OBJ_NUMBER_OF_OBJECTS];
   float32_T best_dist_obj_to_path_confidence[PA_OBJ_NUMBER_OF_OBJECTS];
   float32_T best_similarity_trail_path_confidence[PA_OBJ_NUMBER_OF_OBJECTS];
   float32_T best_heading_diff_confidence[PA_OBJ_NUMBER_OF_OBJECTS];
   float32_T best_heading_segment_confidence[PA_OBJ_NUMBER_OF_OBJECTS][PT_NUM_HEADING_SEGMENTS];
   float32_T best_confidence_factor_total[PA_OBJ_NUMBER_OF_OBJECTS];
   uint8_t best_path_index[PA_OBJ_NUMBER_OF_OBJECTS];

   /* Debug information for paths which are suiting an object best, but have been mitigated by e.g. too
   high minimum confidence value */
   float32_T dist_obj_to_border_confidence[PA_OBJ_NUMBER_OF_OBJECTS][PT_NUMBER_OF_PATHS];
   float32_T dist_border_to_isect_confidence[PA_OBJ_NUMBER_OF_OBJECTS][PT_NUMBER_OF_PATHS];
   float32_T dist_obj_to_path_confidence[PA_OBJ_NUMBER_OF_OBJECTS][PT_NUMBER_OF_PATHS];
   float32_T similarity_trail_path_confidence[PA_OBJ_NUMBER_OF_OBJECTS][PT_NUMBER_OF_PATHS];
   float32_T heading_diff_confidence[PA_OBJ_NUMBER_OF_OBJECTS][PT_NUMBER_OF_PATHS];
   float32_T confidence_factor_total[PA_OBJ_NUMBER_OF_OBJECTS][PT_NUMBER_OF_PATHS];
   float32_T closest_to_successive_pt_heading[PA_OBJ_NUMBER_OF_OBJECTS][PT_NUMBER_OF_PATHS];
   float32_T rad_object_to_path_dist[PA_OBJ_NUMBER_OF_OBJECTS][PT_NUMBER_OF_PATHS];
   float32_T relevant_dist_comp_matching[PA_OBJ_NUMBER_OF_OBJECTS][PT_NUMBER_OF_PATHS];
   float32_T weighted_mean_diff_last_points[PA_OBJ_NUMBER_OF_OBJECTS][PT_NUMBER_OF_PATHS];
   int8_t dist_border_to_isect[PA_OBJ_NUMBER_OF_OBJECTS][PT_NUMBER_OF_PATHS];
   int8_t dist_border_to_obj[PA_OBJ_NUMBER_OF_OBJECTS][PT_NUMBER_OF_PATHS];
   float32_T heading_diff_pt_segment[PA_OBJ_NUMBER_OF_OBJECTS][PT_NUMBER_OF_PATHS][PT_NUM_HEADING_SEGMENTS];
   float32_T heading_segment_confidence[PA_OBJ_NUMBER_OF_OBJECTS][PT_NUMBER_OF_PATHS][PT_NUM_HEADING_SEGMENTS];
   float32_T weights_to_heading_segments[PA_OBJ_NUMBER_OF_OBJECTS][PT_NUMBER_OF_PATHS][PT_NUM_HEADING_SEGMENTS];
} Pt_Debug_Internal_Data_T;

typedef struct
{
   /* Debug pure rotated paths */
   float32_T path_points_pure_rotated[PT_NUMBER_OF_PATHS][PT_NUM_GRID_POINTS];
   float32_T rotated_grid_array_values[PT_NUMBER_OF_PATHS][PT_NUM_GRID_POINTS];
   boolean_T f_path_extended_upper_border[PT_NUMBER_OF_PATHS];
   boolean_T f_path_extended_lower_border[PT_NUMBER_OF_PATHS];
   boolean_T f_path_shrinked_upper_border[PT_NUMBER_OF_PATHS];
   boolean_T f_path_shrinked_lower_border[PT_NUMBER_OF_PATHS];

} Pt_Debug_Rotation_Data_T;

/* Persistent storage for metrics of heading difference confidence. This buffer is needed due to control logic of object matching
 * in path tracking */
typedef struct
{
   float32_T heading_segment_confidence[PA_OBJ_NUMBER_OF_OBJECTS][PT_NUMBER_OF_PATHS][PT_NUM_HEADING_SEGMENTS];
   float32_T weights_to_heading_segments[PA_OBJ_NUMBER_OF_OBJECTS][PT_NUMBER_OF_PATHS][PT_NUM_HEADING_SEGMENTS];
   float32_T heading_diff_pt_segment[PA_OBJ_NUMBER_OF_OBJECTS][PT_NUMBER_OF_PATHS][PT_NUM_HEADING_SEGMENTS];
} Pt_Debug_Internal_Heading_Diff_Metrics_T;

typedef struct
{
   Pt_Path_T paths[PT_NUMBER_OF_PATHS];
   uint8_t number_of_paths;
   uint8_t number_of_finished_paths;
} Pt_Debug_Paths_T;

typedef struct
{
   Pt_Debug_Paths_T pt_paths;
   Pt_Debug_Internal_Data_T Pt_Debug_Internal;
   Pt_Debug_Internal_Heading_Diff_Metrics_T Pt_Debug_Heading_Diff_Metrics;
   Pt_Path_Reset_Reason_T Pt_Debug_Reset_Reason[PT_NUMBER_OF_PATHS];
   Path_Match_Reason_T Pt_Debug_Match_Reason[PA_OBJ_NUMBER_OF_OBJECTS];
   Pt_Debug_Rotation_Data_T Pt_Debug_Rotation_Data;
} Pt_Debug_Output_T;

typedef struct
{
   uint16_t pt_sw_major_version;
   uint16_t pt_sw_minor_version;
} Pt_Debug_Version_T;


typedef struct
{
   Pt_Input_T pt_core_input;
   Pt_Output_T pt_core_output;
   Pt_Persistent_T pt_persistent;
   Pt_Core_Calibration_T pt_calibration;
   Pt_Debug_Version_T pt_version;
   Pt_Debug_Output_T pt_debug_output;
} Pt_Debug_Data_T;

/*===========================================================================*\
* Debug interface
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

   Pt_Debug_Data_T *Pt_Get_Debug_Data(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*===========================================================================*\
* Helper functions to fill and reset debug data
\*===========================================================================*/

void Pt_Debug_Reset_Data(void);

void Pt_Debug_Pass_General_Data(const Pt_Input_T *p_pt_core_input,
                                const Pt_Output_T *p_pt_core_output,
                                const Pt_Persistent_T *p_pt_persistent,
                                const Pt_Core_Calibration_T *p_pt_cal);

void Pt_Debug_Pass_Sw_Version(const uint16_t pt_sw_major_version, const uint16_t pt_sw_minor_version);

void Pt_Debug_Pass_Path_Data(const Pt_Path_T p_paths[PT_NUMBER_OF_PATHS]);

void Pt_Debug_Pass_Path_Size_Change_Flags(const boolean_T lower_size_flag,
                                          const boolean_T upper_size_flag,
                                          uint8_t path_index,
                                          boolean_T f_debug_shrinkage);

void Pt_Debug_Pass_Best_Pair_Conf_To_Intern(const Pt_Path_Obj_Pair_Confidence_T *p_path_obj_pair_confidence,
                                            const uint32_t object_index,
                                            const uint8_t path_index);

void Pt_Debug_Pass_Pair_Confidence_Metrics_To_Intern(const Pt_Path_Obj_Pair_Confidence_T *p_path_obj_pair_confidence,
                                                     const Pt_Path_Obj_Pair_Info_T *p_path_obj_info,
                                                     const uint32_t object_index,
                                                     const uint8_t path_index);

void Pt_Debug_Pass_Heading_Diff_Segments_Metrics(const Pt_Path_Obj_Pair_Info_T *p_path_obj_pair_info,
                                                 const float32_T heading_segment_confidence[PT_NUM_HEADING_SEGMENTS],
                                                 const float32_T weights_to_heading_segments[PT_NUM_HEADING_SEGMENTS],
                                                 const uint32_t object_index,
                                                 const uint8_t path_index);

void Pt_Debug_Pass_Match_Information(const boolean_T f_path_change_due_to_grouping,
                                     const boolean_T f_path_change_due_to_nearer_path,
                                     const boolean_T f_prev_best_pair_updates_confidence,
                                     const float32_T path_change_matching_hysteresis,
                                     const uint8_t obj_index,
                                     const Pt_Core_Calibration_T *p_cals);

void Pt_Debug_Pass_Path_Reset_Reason(const uint8_t path_index, const Pt_Path_Reset_Reason_T path_reset_reason);

/*===========================================================================*\
* Macros to disable debug output in production code
\*===========================================================================*/

/* clang-format off */
#define Binary_Pt_Debug_Reset_Data()                                                                                                                                                                     Pt_Debug_Reset_Data()
#define Binary_Pt_Debug_Pass_General_Data(p_pt_core_input, p_pt_core_output, p_pt_persistent, p_pt_cal)                                                                                                  Pt_Debug_Pass_General_Data(p_pt_core_input, p_pt_core_output, p_pt_persistent, p_pt_cal)
#define Binary_Pt_Debug_Pass_Sw_Version(pt_sw_major_version, pt_sw_minor_version)                                                                                                                        Pt_Debug_Pass_Sw_Version(pt_sw_major_version, pt_sw_minor_version)
#define Binary_Pt_Debug_Pass_Path_Data(p_paths)                                                                                                                                                          Pt_Debug_Pass_Path_Data(p_paths)
#define Binary_Pt_Debug_Pass_Path_Size_Change_Flags(lower_size_flag, upper_size_flag, path_index, f_debug_shrinkage)                                                                                     Pt_Debug_Pass_Path_Size_Change_Flags(lower_size_flag, upper_size_flag, path_index, f_debug_shrinkage)
#define Binary_Pt_Debug_Pass_Best_Pair_Conf_To_Intern(p_path_obj_pair_confidence, object_index, path_index)                                                                                              Pt_Debug_Pass_Best_Pair_Conf_To_Intern(p_path_obj_pair_confidence, object_index, path_index)
#define Binary_Pt_Debug_Pass_Pair_Confidence_Metrics_To_Intern(p_path_obj_pair_confidence, p_path_obj_info, object_index, path_index)                                                                    Pt_Debug_Pass_Pair_Confidence_Metrics_To_Intern(p_path_obj_pair_confidence, p_path_obj_info, object_index, path_index)
#define Binary_Pt_Debug_Pass_Heading_Diff_Segments_Metrics(p_path_obj_pair_info, heading_segment_confidence, weights_to_heading_segments, object_index, path_index)                                      Pt_Debug_Pass_Heading_Diff_Segments_Metrics(p_path_obj_pair_info, heading_segment_confidence, weights_to_heading_segments, object_index, path_index)
#define Binary_Pt_Debug_Pass_Match_Information(f_path_change_due_to_grouping, f_path_change_due_to_nearer_path, f_prev_best_pair_updates_confidence, path_change_matching_hysteresis, obj_index, p_cals) Pt_Debug_Pass_Match_Information(f_path_change_due_to_grouping, f_path_change_due_to_nearer_path, f_prev_best_pair_updates_confidence, path_change_matching_hysteresis, obj_index, p_cals)
#define Binary_Pt_Debug_Pass_Path_Reset_Reason(path_index, path_reset_reason)                                                                                                                            Pt_Debug_Pass_Path_Reset_Reason(path_index, path_reset_reason)
/* clang-format on */

#else

/* clang-format off */
#define Binary_Pt_Debug_Reset_Data()
#define Binary_Pt_Debug_Pass_General_Data(p_pt_core_input, p_pt_core_output, p_pt_persistent, p_pt_cal)
#define Binary_Pt_Debug_Pass_Sw_Version(pt_sw_major_version, pt_sw_minor_version)
#define Binary_Pt_Debug_Pass_Path_Data(p_paths)
#define Binary_Pt_Debug_Pass_Path_Size_Change_Flags(lower_size_flag, upper_size_flag, path_index, f_debug_shrinkage)
#define Binary_Pt_Debug_Pass_Best_Pair_Conf_To_Intern(p_path_obj_pair_confidence, object_index, path_index)
#define Binary_Pt_Debug_Pass_Pair_Confidence_Metrics_To_Intern(p_path_obj_pair_confidence, p_path_obj_info, object_index, path_index)
#define Binary_Pt_Debug_Pass_Heading_Diff_Segments_Metrics(p_path_obj_pair_info, heading_segment_confidence, weights_to_heading_segments, object_index, path_index)
#define Binary_Pt_Debug_Pass_Match_Information(f_path_change_due_to_grouping, f_path_change_due_to_nearer_path, f_prev_best_pair_updates_confidence, path_change_matching_hysteresis, obj_index, p_cals)
#define Binary_Pt_Debug_Pass_Path_Reset_Reason(path_index, path_reset_reason)
/* clang-format on */

#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */

#endif /* PT_DEBUG_INTERFACE_H */
