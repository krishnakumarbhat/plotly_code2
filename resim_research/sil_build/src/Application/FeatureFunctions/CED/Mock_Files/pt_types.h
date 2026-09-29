#ifndef PT_TYPES_H
#define PT_TYPES_H
/**
 * @file pt_types.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains path tracking internal types which are not persistent.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "pa_reuse.h"
#include "pt_directions.h"
#include "pt_output_t.h"

/*===========================================================================*\
* Global Defines
\*===========================================================================*/
#define PT_DEFAULT_DISCR_BORDER (255u)
#define PT_NUM_HEADING_SEGMENTS (3u)

/* Number of maximum array size for all available objects */
#define PT_OBJ_MAX_ARRAY_SIZE ((uint8_t) PA_OBJ_NUMBER_OF_OBJECTS + FBK_ONE_UINT)

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * This struct provides additional information about possible path-object pairs.
 */
typedef struct
{
   float32_T distance_to_path;     /**< distance to the most nearby path. This is filled independently of the path confidence*/
   float32_T segment_heading_diff; /**< heading diff to the most nearby path. This is filled independently of the path confidence*/
   uint8_t path_index;             /**< path index of the most nearby path. This does not need to be a valid match here*/
} Pt_Path_Obj_Pair_Consumer_Info_T;


/**
 * Stores all object specific information from the tracker.
 */
typedef struct
{
   Fbk_Object_Data_T tracker_data; /* Tracker data provided by PA */
} Pt_Object_T;


/**
 * Summarizes all reasons available for a path reset. This is mainly used to justify a reset of a path and to debug path reset
 * behavior.
 */
typedef enum
{
   PATH_RESET_NO_RESET = (0), /**< No path reset happened */

   PATH_RESET_PATH_TRACKING_SHUTDOWN = (1), /**< pt_iface.c - Path tracking algorithm gets initialized. This could be a wake up or
                                               shutdown routine. */

   PATH_RESET_SHORT_PATH_ORIENTATION_CHANGE = (2), /**< pt.c - Path too short when object orientation changed */
   PATH_RESET_FAR_FIELD_PATH                = (3), /**< pt.c - Path exceeds distance thresholds */
   PATH_RESET_MAKE_PATH_ROOM                = (4), /**< pt.c - Low priority path makes room for other paths */
   PATH_RESET_IMPLAUSIBLE_PATH              = (5), /**< pt.c - Path is implausible */
   PATH_RESET_INCOMPLETE_PATH_TOO_SHORT     = (6), /**< pt.c - Incomplete path is too short */
   PATH_RESET_EXTRAPOLATED_PATH_IMPLAUSIBLE = (7), /**< pt.c - Extrapolated path part is extrapolated through host */

   PATH_RESET_FIRST_P_EQ_LAST_P                         = (8), /**< pt_group_paths.c - First and last point are the same */
   PATH_RESET_GROUP_OVERLAP_REDUNDANT_TO_MULTIPLE_PATHS = (9), /**< pt_group_paths.c - Path is redundant to multiple other paths */
   PATH_RESET_GROUP_OVERLAP_GROUPED_INTO_OTHER_PATH     = (10), /**< pt_group_paths.c - Path is grouped into another path */
   PATH_RESET_GROUP_OVERLAP_LESS_ESTABLISHED_PATH = (11), /**< pt_group_paths.c - Path is less established than another path */
   PATH_RESET_GROUP_CROSS_PATH                    = (12), /**< pt_group_paths.c - Path crosses other paths */

   PATH_RESET_ROTATE_PATHS_BOUNDARY_EXCEED_LIMITS = (13),  /**< pt_path_rotation.c - Path boundary exceed limits after
                                                                       rotation */
   PATH_RESET_ROTATE_PATHS_TOO_SHORT_AFTER_ROTATION = (14) /**< pt_path_rotation.c - Path is too short after rotation */

} Pt_Path_Reset_Reason_T;

/**
 * @brief This enumeration summarizes all possible path states.
 */
typedef enum
{
   PATH_STATUS_DEFAULT  = (0), /**< Indicates that the path slot is empty*/
   PATH_STATUS_CREATION = (1), /**< The path is currently in its build up phase, meaning that it is not extrapolated yet*/
   PATH_STATUS_MATURE   = (2), /**< The path has finished its build up phase and is extrapolated*/
   PATH_STATUS_GROUPED  = (3), /**< Two paths have been grouped*/
   PATH_STATUS_GROUPED_IN_CURRENT_CYCLE = (4), /**< Two paths have been grouped grouped in current cycle*/
   PATH_STATUS_HOST_TRAIL               = (5)  /**< Path was created based on the host trail.*/
} Pt_Path_Status_T;

/**
 * Summarizes grid point distances which are used for confidence metrics.
 */
typedef struct
{
   uint8_t dist_border_to_mid; /**< distance between relevant discrete path border and mid point index depending on approach side
                                 of object*/
   uint8_t dist_border_to_obj; /**< distance between relevant discrete path border and object*/
} Pt_Pair_Border_Info_T;

/**
 * Summarizes all entities which are used for confidence metrics.
 */
typedef struct
{
   float32_T closest_to_successive_pt_heading; /**< path heading between the closest grid point and the successive point*/
   float32_T rad_object_to_path_dist;          /**< radial distance between object and next path point*/
   float32_T relevant_dist_comp_matching;      /**< distance between target center and interpolated value coordinate of the path*/
   float32_T weighted_mean_diff_last_points; /**< weighted mean of the difference between last two successive trail points and path
                                              candidate points*/
   float32_T heading_diff_pt_segment[PT_NUM_HEADING_SEGMENTS]; /**< heading difference between vcs heading of object and path*/
   Pt_Pair_Border_Info_T border_info;                          /**< information about the objects distance to path borders*/
} Pt_Path_Obj_Pair_Info_T;

/**
 * Summarizes all confidence metrics which are combined to the total confidence factor.
 */
typedef struct
{
   float32_T dist_obj_to_border_confidence;    /**< confidence for distance between object and path border metric */
   float32_T dist_border_to_isect_confidence;  /**< confidence for distance between isect and path border metric */
   float32_T dist_obj_to_path_confidence;      /**< confidence for distance between isect and path point metric */
   float32_T similarity_trail_path_confidence; /**< confidence for similarity between isect and trail metric */
   float32_T heading_difference_confidence;    /**< confidence for the heading differences of the next three heading diffs between
                                                  object heading and path heading*/
   float32_T confidence_factor_total;          /**< point which is created second last behind the target*/
} Pt_Path_Obj_Pair_Confidence_T;


#endif
