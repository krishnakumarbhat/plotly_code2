#ifndef PT_PERSISTENT_H
#define PT_PERSISTENT_H
/**
 * @file pt_persistent_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains persistent type definitions of path tracking algorithm which are for internal use only.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pt_constants.h"
#include "pt_output_t.h"
#include "pt_types.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief This struct summarizes properties of the object used for path creation.
 */
typedef struct
{
   uint8_t id;  /**< ID of the object which is used for building up a path. */
   uint8_t age; /**< Age of object. */
} Pt_Object_Data_For_Path_T;

/**
 * @brief This enumeration indicates whether a grid point border was updated.
 */
typedef enum
{
   PATH_POINT_NEW_FIRST  = (0), /**< Path point was added as first_p*/
   PATH_POINT_NEW_LAST   = (1), /**< Path point was added as last_p*/
   PATH_POINT_NEW_MATURE = (2)  /**< Path points are mature */
} Pt_Path_Point_Status_T;

/**
 * @brief This enumeration indicates whether the object position inside the path was updated in the current cycle.
 */
typedef enum
{
   PATH_BOTH_BORDERS_NEW     = (0), /**< Both are new and not allowed to be rotated in current cycle*/
   PATH_BORDER_NEW_FIRST     = (1), /**< First_mat has been updated and is not allowed to be rotated in current cycle*/
   PATH_BORDER_NEW_LAST      = (2), /**< Last_mat has been updated and is not allowed to be rotated in current cycle*/
   PATH_BORDER_POINTS_MATURE = (3)  /**< First and last_mat are mature and are allowed to be both rotated*/
} Pt_Path_Border_Points_Status_T;

/**
 * @brief This structure summarizes everything what a path needs.
 */
typedef struct
{
   float32_T path_points[PT_NUM_GRID_POINTS]; /**< Calculated path point in meters. For every grid point of grid point table there
                                             will be an value calculated, if path reaches maturity.*/
   Vector_2d_T first;    /**< Position of target used to create path. This is aligned with ego coordinate systems, this means, that
                           first.x > last_mat.x in    longitudinal case. */
   Vector_2d_T last_mat; /**< Same as first, but the other boundary of the path. */
   float32_T max_speed;  /**< Maximum speed which was detected on the path with the object which was used to create this path
                         (in grouping scenarios it is the mean max speed of both paths). */
   uint16_t path_age;    /**< A measure of how long the path is existing dependend on its state. For CREATION state this will be
                            constantly 1. For MATURE this will be incremented every cycle and in case of grouping the minimum of two
                            paths will be taken */
   Pt_Object_Data_For_Path_T obj_curr_used_for_path_build; /**< Object which is attached to path for the creation of the path. Gets
                                                             Reset when object disappears. */
   Pt_Path_Point_Status_T new_path_point_status;      /**< Indicates if a path point update occured in the current scanindex.*/
   Pt_Path_Border_Points_Status_T path_border_status; /**< Indicates if path borders are updated and if so in what exact way.*/
   Pt_Path_Direction_T direction; /**< Direction of the path. Other direction than NONE needs at least two path points. */
   Pt_Path_Status_T path_state;   /**< Age of path also a measure of trustability for the considered path*/
   uint8_t first_p;       /**< Discrete index of first path point in grid point table where object was used to define path. */
   uint8_t last_p;        /**< Discrete index of last path point in grid point table where object was used to define path. */
   uint8_t path_index;    /**< Index in the internal path array [0...PT_NUMBER_OF_PATHS]. */
   uint8_t num_groupings; /**< Number of previous groupings. Increments on path that is merged into. */
} Pt_Path_T;


/**
 * @brief This struct provides additional information about possible path-object pairs.
 */
typedef struct
{
   float32_T confidence_factor;           /**< confidence value of the best match for the object*/
   float32_T relevant_dist_comp_matching; /**< distance to the path*/
   uint8_t path_index;                    /**< path index which is indicating the best match for the current object*/
} Pt_Best_Path_Obj_Pair_Persistent_T;


/**
 * @brief This structure gathers every persistent structure within Path Tracking e.g. the paths which are build up over consecutive
cycles
 * or
 */
typedef struct
{
   Pt_Path_T paths[PT_NUMBER_OF_PATHS];                                           /**< Struct which summarizes path information*/
   Pt_Best_Path_Obj_Pair_Persistent_T best_path_obj_pairs[PT_OBJ_MAX_ARRAY_SIZE]; /**< Information about possible path-object
                                                                                        pairs */
   uint8_t path_index_last_cycle[PT_OBJ_MAX_ARRAY_SIZE]; /**< path index which is indicating the best match for objects in the
                                                               last cycle*/
   boolean_T f_was_pt_executed;                          /**< Indicates wether path algorithm has been executed. */
} Pt_Persistent_T;


#endif
