#ifndef LTB_OUTPUT_T_H
#define LTB_OUTPUT_T_H

/**
 * @file ltb_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Generic customer output declaration.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "ltb_core_output_t.h"
#include "pa_reuse.h"

/*===========================================================================*\
 * typedefs
\*===========================================================================*/

typedef struct
{
   uint8_t ltb_id;                    /**< LTB object tracker ID */
   float32_T ltb_ttc_s;               /**< [s] time-to-collision */
   float32_T ltb_ttb_s;               /**< [s] time-to-brake */
   float32_T ltb_decel_estimate_mps2; /**< [m/s^2] deceleration estimate to avoid collision */
   float32_T ltb_distance_m;          /**< [m] object distance */
} Ltb_Critical_Object_T;

typedef struct
{
   Ltb_Critical_Object_T ltb_object[FBK_NUMBER_OF_SIDES];  /** Ltb Object data*/
   Ltb_Alert_State_T ltb_alert_level[FBK_NUMBER_OF_SIDES]; /**< Overall LTB alert level */
   uint8_t ltb_most_critical_side;                         /**< most critical LTB side based on alert levels and lowest ttc */
} Ltb_Output_T;

#endif /* LTB_OUTPUT_T_H */
