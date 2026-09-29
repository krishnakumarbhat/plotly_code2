#ifndef RECW_TYPES_H
#define RECW_TYPES_H

/**
 * @file recw_types.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the RECW types header file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_ref_point.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"
#include "recw_core_output_t.h"

/*===================================================================*\
* Defines
\*===================================================================*/

#define RECW_INVALID_LON_POS (100.0f)        /** Value used to indicate invalid longitudinal position */
#define RECW_MAX_TTC (100.0f)                /** Value used to indicate invalid TTC */
#define RECW_MIN_POS_DIFF_FILTER_CYCLES (8u) /** Minimal number of cycles with valid data to use filter */
#define RECW_MIN_FILTERED_DIFF_POS (0.01f)   /** Minimal difference in position from cycle to cycle for filter */
#define RECW_MIN_ACCEL_THRESHOLD (0.01f)     /** Minimal acceleration to include acceleration in TTC calculation */

/* Number of maximum array size for all available objects based on IDs */
#define RECW_MAX_ID_ARRAY_SIZE ((uint8_t) PA_OBJ_NUMBER_OF_OBJECTS + FBK_ONE_UINT)

/*===================================================================*\
* Enums
\*===================================================================*/

/**
 * @brief Contains indices for alert level calibration arrays
 *
 * @SDD{SF-7943}
 */
typedef enum
{
   RECW_INDEX_ALERT_LEVEL_1 = (0),
   RECW_INDEX_ALERT_LEVEL_2 = (1),
   RECW_NUMBER_ALERT_LEVEL  = (2)
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Recw_Cal_Array_Indices_T;

/*===========================================================================*\
* Type definitions
\*===========================================================================*/

/**
 * @brief Recw_Object_Attributes_T structure
 *
 * Stores the Recw internally calculated object attribute information
 *
 * @SDD{SF-7825}
 */
typedef struct
{
   Vector_2d_T filtered_diff_pos;
   Vector_2d_T effective_rel_vel;

   float32_T crash_prob_braking;
   float32_T crash_prob_steering;
   float32_T crash_prob_combined;

   float32_T needed_brake_acceleration;
   float32_T needed_steering_acceleration;

   float32_T filtered_heading;
   float32_T overlap; /* Object's predicted overlap with host at rear bumper */
   float32_T overlap_line_y_min;
   float32_T overlap_line_y_max;

   float32_T ttc;
   float32_T ttc_threshold[RECW_NUMBER_ALERT_LEVEL];

   Recw_Alert_T alert_level;

   boolean_T f_object_is_car_wash_ghost;
   boolean_T f_obj_is_within_lane;
} Recw_Object_Attributes_T;


/**
 * @brief Definition of the Recw_Object_T structure
 *
 * @SDD{SF-7827}
 */
typedef struct
{
   Fbk_Object_Data_T tracker_data;
   Recw_Object_Attributes_T attributes;
} Recw_Object_T;

#endif /* RECW_TYPES_H */
