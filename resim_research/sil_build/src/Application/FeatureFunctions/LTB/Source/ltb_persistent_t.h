#ifndef LTB_PERSISTENT_T_H
#define LTB_PERSISTENT_T_H

/**
 * @file ltb_persistent_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the persistent data structure.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_ego_traj_predictor_instance.h"
#include "fbk_macros.h"
#include "ltb_types.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Ltb_Persistent_T structure
 * @SDD{CSCSA-53952}
 */
typedef struct
{
   float32_T ltb_pred_step_dt;                           /**< [s] delta time value between two prediction time steps */
   float32_T ltb_ego_yaw_angle_to_last_straight_section; /**< Integrated ego yaw angle to last known straight section using ego yaw
                                                          rate and cycle time information. */
   /* LTB side specific */
   Ltb_Alert_State_T ltb_side_alert_prev_cycle[FBK_NUMBER_OF_SIDES]; /**< Alert level of previous cycle */
   uint8_t ltb_side_id_prev_cycle[FBK_NUMBER_OF_SIDES];              /**< Object ID of previous cycle alert */

   uint8_t ltb_side_alert_qualifying_counter[FBK_NUMBER_OF_SIDES]; /**< Alert qualifying counter */
   uint8_t ltb_side_alert_holding_counter[FBK_NUMBER_OF_SIDES];    /**< Alert holding counter */

} Ltb_Persistent_T;

#endif /* LTB_PERSISTENT_T_H */
