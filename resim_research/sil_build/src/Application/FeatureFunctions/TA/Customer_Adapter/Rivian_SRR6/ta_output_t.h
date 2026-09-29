#ifndef TA_OUTPUT_T_H
#define TA_OUTPUT_T_H

/**
 * @file ta_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the Rivian_SRR6 specific output data structure for TA.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_macros.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

typedef enum
{
   RIVIAN_TA_ALGORITHM_DISABLED       = (0),
   RIVIAN_TA_NO_VALID_OBJECTS         = (1),
   RIVIAN_TA_VEHICLE_STATE_INVALID    = (2),
   RIVIAN_TA_NO_RELEVANT_OBJECTS      = (3),
   RIVIAN_TA_NO_CRITICAL_OBJECTS      = (4),
   RIVIAN_TA_CRITICAL_OBJECT_DETECTED = (5)
} Ta_Rivian_Status_T;

/**
 * @brief Ta_Output_T structure
 *
 * @SDD{}
 */
typedef struct
{
   Vector_2d_T ta_waypoint_at_collision[FBK_NUMBER_OF_SIDES]; /**< [m] coordinates of collision waypoint */
   float32_T ta_ttc[FBK_NUMBER_OF_SIDES];                     /**< [s] time-to-collision */
   float32_T ta_ttp[FBK_NUMBER_OF_SIDES];                     /**< [s] time-to-pass */
   float32_T ta_ttb[FBK_NUMBER_OF_SIDES];                     /**< [s] time-to-brake */
   float32_T ta_decel_estimate[FBK_NUMBER_OF_SIDES];          /**< [m/s^2] deceleration estimate to avoid collision */
   float32_T ta_distance[FBK_NUMBER_OF_SIDES];                /**< [m] object distance */

   uint8_t ta_id[FBK_NUMBER_OF_SIDES];    /**< TA object tracker ID */
   uint8_t ta_index[FBK_NUMBER_OF_SIDES]; /**< TA object tracker index */
   uint8_t ta_most_critical_side;         /**< most critical TA side based on alert levels and lowest ttc */

   boolean_T ta_alert[FBK_NUMBER_OF_SIDES];                /**< flag indicating TA alert */
   boolean_T ta_f_obj_in_danger_zone[FBK_NUMBER_OF_SIDES]; /**< flag indicating object is in danger zone */
   boolean_T ta_f_obj_in_info_zone[FBK_NUMBER_OF_SIDES];   /**< flag indicating object is in info zone */
   boolean_T ta_f_obj_in_wing_zone[FBK_NUMBER_OF_SIDES];   /**< flag indicating object is in wing zone */

   uint8_t ta_n_valid_objects;    /**< Number of objects that are valid for TA */
   uint8_t ta_n_relevant_objects; /**< Number of objects that are relevant for TA */
   uint8_t ta_n_critical_objects; /**< Number of objects that are critical for TA */

   Ta_Rivian_Status_T ta_status; /**< Contains TA algorithm states */

} Ta_Output_T;

#endif /* TA_OUTPUT_T_H */
