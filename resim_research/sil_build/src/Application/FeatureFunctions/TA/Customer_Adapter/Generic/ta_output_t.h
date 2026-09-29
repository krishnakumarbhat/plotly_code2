#ifndef TA_OUTPUT_T_H
#define TA_OUTPUT_T_H

/**
 * @file ta_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the output data structure for TA.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_reuse.h"
#include "ta_core_output_t.h"

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

/**
 * @brief Ta_Critical_Object_T structure
 *
 * @SDD{}
 */
typedef struct
{
   Vector_2d_T ta_waypoint_at_collision_m; /**< [m] coordinates of collision waypoint */
   float32_T ta_ttc_s;                     /**< [s] time-to-collision */
   float32_T ta_ttp_s;                     /**< [s] time-to-pass */
   float32_T ta_ttb_s;                     /**< [s] time-to-brake */
   float32_T ta_decel_estimate_mps2;       /**< [m/s^2] deceleration estimate to avoid collision */
   float32_T ta_distance_m;                /**< [m] object distance */

   uint8_t ta_id;    /**< TA object tracker ID */
   uint8_t ta_index; /**< TA object tracker index */

   boolean_T ta_f_obj_in_danger_zone; /**< flag indicating object is in danger zone */
   boolean_T ta_f_obj_in_info_zone;   /**< flag indicating object is in info zone */
   boolean_T ta_f_obj_in_wing_zone;   /**< flag indicating object is in wing zone */
} Ta_Critical_Object_T;

/**
 * @brief Ta_Output_T structure
 *
 * @SDD{}
 */
typedef struct
{
   boolean_T f_ta_enable;                 /**< Flag indicating that TA is enabled */
   boolean_T ta_f_vehicle_state_relevant; /**< Flag indicating that the vehicle state is relevant */

   uint8_t ta_most_critical_side; /**< most critical TA side based on alert levels and lowest ttc */
   uint8_t ta_n_valid_objects;    /**< Number of objects that are valid for TA */
   uint8_t ta_n_relevant_objects; /**< Number of objects that are relevant for TA */
   uint8_t ta_n_critical_objects; /**< Number of objects that are critical for TA */

   Ta_Algorithm_State_T ta_algorithm_state;              /**< Contains the state for the TA algorithm in current cycle */
   Ta_Alert_State_T ta_alert_level[FBK_NUMBER_OF_SIDES]; /**< Overall TA alert level */
   Ta_Critical_Object_T ta_object[FBK_NUMBER_OF_SIDES];  /**< TA alert object with parameters */
} Ta_Output_T;

#endif /* TA_OUTPUT_T_H */
