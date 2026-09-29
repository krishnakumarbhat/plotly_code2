#ifndef PT_OUTPUT_T_H
#define PT_OUTPUT_T_H
/**
 * @file pt_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the output type definition of path tracking algorithm. This type definition is
 * part of the interface.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_obj_in.h"
#include "pa_reuse.h"
#include "pt_types.h"

/*===========================================================================*\
* Defines
\*===========================================================================*/

#define PT_DEFAULT_MATCH_INDEX (255u)

/*===========================================================================*\
* typedefs
\*===========================================================================*/
/*
 * Describes the direction of the path in vcs-coordinate system.
 */
typedef enum
{
   PATH_DIRECTION_NONE          = (0), /**< Path has no direction yet. */
   PATH_DIRECTION_LAT_LEFT      = (1), /**< Path mainly goes from positive y to negative y.*/
   PATH_DIRECTION_LAT_RIGHT     = (2), /**< Path mainly goes from negative y to positive y.*/
   PATH_DIRECTION_LONG_FORWARD  = (3), /**< Path mainly goes from positive x to negative x.*/
   PATH_DIRECTION_LONG_BACKWARD = (4)  /**< Path mainly goes from negative x to positive x.*/
} Pt_Path_Direction_T;

/**
 * Struct summarizing the properties of nearest path for the path object pair
 *
 * @SAE{SF-2919}
 * @SDD{SF-7573}
 */
typedef struct
{
   float32_T range_vcs_proj_to_path_segment; /**< Describes the range between the object and the vcs projected point on the path
                                                segment.*/
   float32_T segment_heading_diff;           /**< Describes the difference between object heading and path segment heading.*/
   uint8_t track_idx_nearest_path;           /**< Describes the path index of the path which is located nearest to the object.*/
} Pt_Nearest_Path_T;


/**
 * Struct summarizing the information about path matches.
 *
 * @SAE{}
 * @SDD{}
 */
typedef struct
{
   float32_T range_at_zero; /**< Describes the range between ego vehicle and considered path at grid point index zero.*/
   float32_T range_to_current_path_part; /**< Describes the range between target and its matched path. This is done in a vcs
                            fashion. It is positive, when the object is on the right side of the path and negative when it is on
                            the left side of the path. The direction of a path is defined like the positive direction in vcs.*/
   float32_T range_at_host_edge; /**< Descibes the range between ego vehicle and considered path at the respective edge of the host
                    vehicle. In case of long. paths this is the rear and in case of lat. paths this is one of the rear corners*/
   float32_T length_of_trajectory;     /**< Length of the path between the orthogonally projected center point of the object to
                                       the surrounding path segment and the path point at the intersection with coordinate axis.*/
   float32_T path_heading;             /**< Describes the path heading to the intersection of the object with vcs*/
   Pt_Path_Direction_T path_direction; /**< Describes the path direction.*/
   Pt_Path_Status_T path_state;        /**< Age of path also a measure of trustability for the considered path*/
   uint8_t track_match;                /**< Indicates to which track the considered object is matched in the current scanindex.*/
   uint8_t track_match_age; /**< Indicates how long the object has been matched to any path (changes in matching are not reset).*/
   uint8_t track_match_last_cycle; /**< Indicates internally to which track the considered object was matched before.*/
} Pt_Path_Object_Pair_Output_T;

/**
 * Struct summarizing information about the nearest path, the path match properties and additional information about the state of
 * path tracking.
 *
 * @SAE{SF-2916}
 * @SDD{SF-7277}
 */
typedef struct
{
   Pt_Nearest_Path_T nearest_path_output[PA_OBJ_NUMBER_OF_OBJECTS];             /**< Describes properties of the path object pair
                                                                            where the path is located nearest to the object.*/
   Pt_Path_Object_Pair_Output_T path_obj_pair_output[PA_OBJ_NUMBER_OF_OBJECTS]; /**< Describes properties of the matched path for
                                                                                the
                                                                                respective object.*/
   boolean_T f_pt_operational; /**< Flag indicating whether path tracking is operational*/
} Pt_Output_T;

#endif
