#ifndef TA_PERSISTENT_T_H
#define TA_PERSISTENT_T_H

/**
 * @file ta_persistent_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the persistent data of the TA module.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_macros.h"
#include "ta_types.h"

/*============================================================================*\
* EXPORTED TYPEDEF DECLARATIONS
\*============================================================================*/

/* STRUCTS -------------------------------------------------------------------*/

/**
 * @brief Ta_Persistent_T structure
 *
 * Stores persistent data
 *
 * @SDD{SF-8640}
 */
typedef struct
{
   float32_T ta_pred_step_dt;                           /**< [s] delta time value between two prediction time steps */
   float32_T ta_ego_yaw_angle_to_last_straight_section; /**< Integrated ego yaw angle to last known straight section using ego yaw
                                                           rate and cycle time information. */

   Ta_Alert_Mode_T ta_alert_mode[PA_OBJ_NUMBER_OF_OBJECTS]; /**< Indicates previous cycle had active alert for FTA, RTA, or both */

   Ta_Alert_State_T ta_side_alert_prev_cycle[FBK_NUMBER_OF_SIDES]; /**< Alert level of previous cycle */
   uint8_t ta_side_id_prev_cycle[FBK_NUMBER_OF_SIDES];             /**< Object ID of previous cycle alert */
   uint8_t ta_side_index_prev_cycle[FBK_NUMBER_OF_SIDES];          /**< Object index of previous cycle alert */

   uint8_t ta_side_alert_qualifying_counter[FBK_NUMBER_OF_SIDES]; /**< Alert qualifying counter */
   uint8_t ta_side_alert_holding_counter[FBK_NUMBER_OF_SIDES];    /**< Alert holding counter */

} Ta_Persistent_T;

#endif /* TA_PERSISTENT_T_H */
