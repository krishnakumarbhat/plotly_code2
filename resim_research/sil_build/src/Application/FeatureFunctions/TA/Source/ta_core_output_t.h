#ifndef TA_CORE_OUTPUT_T_H
#define TA_CORE_OUTPUT_T_H

/**
 * @file ta_core_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the core output data structure for TA.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_macros.h"
#include "ml_vector_2d_t.h"
#include "ta_types.h"

/**
 * @brief Ta_Core_Output_T structure
 *
 * @SDD{SF-8638}
 */
typedef struct
{
   Vector_2d_T ta_waypoint_at_collision[FBK_NUMBER_OF_SIDES]; /**< [m] coordinates of collision waypoint */
   float32_T ta_ttc[FBK_NUMBER_OF_SIDES];                     /**< [s] time-to-collision */
   float32_T ta_ttp[FBK_NUMBER_OF_SIDES];                     /**< [s] time-to-pass */
   float32_T ta_ttb[FBK_NUMBER_OF_SIDES];                     /**< [s] time-to-brake */
   float32_T ta_decel_estimate[FBK_NUMBER_OF_SIDES];          /**< [m/s^2] deceleration estimate to avoid collision */
   float32_T ta_distance[FBK_NUMBER_OF_SIDES];                /**< [m] object distance */

   Ta_Alert_State_T ta_alert_level[FBK_NUMBER_OF_SIDES]; /**< Overall TA alert level */
   uint8_t ta_id[FBK_NUMBER_OF_SIDES];                   /**< TA object tracker ID */
   uint8_t ta_index[FBK_NUMBER_OF_SIDES];                /**< TA object tracker index */
   uint8_t ta_most_critical_side;                        /**< most critical TA side based on alert levels and lowest ttc */

   boolean_T ta_f_obj_in_danger_zone[FBK_NUMBER_OF_SIDES]; /**< flag indicating object is in danger zone */
   boolean_T ta_f_obj_in_info_zone[FBK_NUMBER_OF_SIDES];   /**< flag indicating object is in info zone */
   boolean_T ta_f_obj_in_wing_zone[FBK_NUMBER_OF_SIDES];   /**< flag indicating object is in wing zone */

   boolean_T ta_f_vehicle_state_relevant; /**< Flag indicating that the vehicle state is relevant */
   uint8_t ta_n_valid_objects;            /**< Number of objects that are valid for TA */
   uint8_t ta_n_relevant_objects;         /**< Number of objects that are relevant for TA */
   uint8_t ta_n_critical_objects;         /**< Number of objects that are critical for TA */

   Ta_Algorithm_State_T ta_algorithm_state; /**< Contains the state for the TA algorithm in current cycle */

} Ta_Core_Output_T;

#endif /* TA_CORE_OUTPUT_T_H */
