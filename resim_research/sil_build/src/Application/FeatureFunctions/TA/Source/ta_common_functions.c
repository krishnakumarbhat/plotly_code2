/**
 * @file ta_common_functions.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains common functions that are used by multiple other modules.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ta_common_functions.h"
#include "fbk_macros.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pa_shared_types.h"
#include "ta_constants.h"
#include "ta_types.h"
#include <assert.h>

/*============================================================================*\
 * EXPORTED FUNCTIONS
\*============================================================================*/

void Ta_Reset_Core_Output(Ta_Core_Output_T *p_ta_core_output, const uint8_t side_index)
{
   /* Assert */
   assert(NULL != p_ta_core_output);
   assert(FBK_SIDE_UNDEFINED > side_index);

   /* Reset object properties */
   p_ta_core_output->ta_waypoint_at_collision[side_index] = Create_2d_Vector_Origin();
   p_ta_core_output->ta_ttc[side_index]                   = TA_INVALID_TTC;
   p_ta_core_output->ta_ttp[side_index]                   = TA_INVALID_TTP;
   p_ta_core_output->ta_ttb[side_index]                   = TA_INVALID_TTB;
   p_ta_core_output->ta_decel_estimate[side_index]        = FBK_ZERO_F;
   p_ta_core_output->ta_distance[side_index]              = TA_INVALID_DISTANCE;

   /* Reset alert level properties */
   p_ta_core_output->ta_alert_level[side_index] = TA_ALERT_STATE_NONE;
   p_ta_core_output->ta_id[side_index]          = PA_INVALID_OBJ_ID;
   p_ta_core_output->ta_index[side_index]       = PA_INVALID_OBJ_INDEX;
   p_ta_core_output->ta_most_critical_side      = FBK_SIDE_UNDEFINED;

   /* Reset zone flags */
   p_ta_core_output->ta_f_obj_in_danger_zone[side_index] = FBK_FALSE;
   p_ta_core_output->ta_f_obj_in_info_zone[side_index]   = FBK_FALSE;
   p_ta_core_output->ta_f_obj_in_wing_zone[side_index]   = FBK_FALSE;
}
