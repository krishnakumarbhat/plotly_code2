#ifndef FBK_IFACE_TYPES_H
#define FBK_IFACE_TYPES_H

/**
 * @file fbk_iface_types.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains types for fbk iface.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ml_angle_t.h"
#include "ml_vector_2d_t.h"
#include "pa_const_macros.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"

/*===========================================================================*\
* Macros
\*===========================================================================*/

/* Num of host trail points */
#define FBK_NUM_HOST_TRAIL_POINTS (20u)

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

/* This structure describes the interval of the host lane which shall be used for path creation. Due to the ringbuffer in host lane
 * module, it is possible that attributes are not strictly ordered.*/
typedef struct
{
   uint8_t interval_start; /**< Start of the interval (nearer to the host position at least from the ringbuffer point of view)*/
   uint8_t interval_end;   /**< End of the interval (farer away from host position at least from the ringbuffer point of view)*/
   uint8_t interval_range; /**< Range of the interval*/
} Fbk_Host_Lane_Interval_T;

/**
 * @brief point pair consisting of the nearest grid point index to host and the next grid point index depending on path direction
 */
typedef struct
{
   uint8_t passed; /**<depending on path direction and position of the target the last point which has been passed*/
   uint8_t next;   /**< depending on path direction the next point which will be passed by the object*/
} Fbk_Point_Pair_T;

typedef struct
{
   Vector_2d_T point;             /**< [m] coordinate of trail point in world coordinates*/
   float32_T distance_traveled;   /**< [m] distance traveled*/
   float32_T dist_between_points; /**< [m] distance (Pythagorean) to the next older trail point*/
   float32_T heading;             /**< [rad] heading angle of host at trail point in world coordinates*/
} Fbk_Host_Path_Point_Info_T;

typedef struct
{
   Fbk_Host_Path_Point_Info_T segments[FBK_NUM_HOST_TRAIL_POINTS]; /**< [m] coordinate of trail point in world coordinates*/
   Vector_2d_T trail_host_position;                                /**< [m] host position in wcs*/
   Angle_T trail_host_heading;                                     /**< host heading angle in world coordinates*/
   float32_T trail_diff_dist;                                      /**< [m] distance traveled since lastly added host path point*/
   float32_T trail_diff_heading;                                   /**< [m] heading change since lastly added host path point*/
   float32_T trail_host_dist;                                      /**< [m] total host distance traveled*/
   uint8_t trail_index;                                            /**< index into next slot in trail buffer*/
   uint8_t oldest_trail_index;                   /**< index into the slot in trail buffer where the oldest point is stored*/
   boolean_T f_trail_full_buffer;                /**< true if buffer is full*/
   boolean_T f_was_trail_point_added_this_cycle; /**< true if a trail point was added this cycle*/
   /* coverity[misra_c_2012_rule_1_1_violation][typedef name has already been declared (with same type)] */
} Fbk_Host_Trail_T;

typedef struct
{
   Pa_Obj_Status_T stage[PA_OBJ_NUMBER_OF_OBJECTS];
   uint8_t stage_age[PA_OBJ_NUMBER_OF_OBJECTS];
} Fbk_Age_Ctr_T;

#endif /*FBK_IFACE_TYPES_H*/
