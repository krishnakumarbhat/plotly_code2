/**
 * @file ltb_common_functions.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains common functions that are used by multiple other modules.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ltb_common_functions.h"
#include "fbk_macros.h"
#include "ltb_types.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"
#include "pa_shared_types.h"
#include <assert.h>

/*============================================================================*\
 * EXPORTED FUNCTIONS
\*============================================================================*/

void Ltb_Reset_Core_Output_Side_Data(Ltb_Core_Output_T *p_ltb_core_output, uint8_t side_index)
{
   /* Assert */
   assert(NULL != p_ltb_core_output);
   assert(side_index < FBK_NUMBER_OF_SIDES);

   /* Reset object properties */
   p_ltb_core_output->ltb_waypoint_at_collision[side_index] = Create_2d_Vector_Origin();
   p_ltb_core_output->ltb_ttc[side_index]                   = LTB_INVALID_TTC;
   p_ltb_core_output->ltb_ttb[side_index]                   = LTB_INVALID_TTB;
   p_ltb_core_output->ltb_decel_estimate[side_index]        = FBK_ZERO_F;
   p_ltb_core_output->ltb_distance[side_index]              = LTB_INVALID_DISTANCE;

   /* Reset alert level properties */
   p_ltb_core_output->ltb_alert_level[side_index] = NO_ALERT;
   p_ltb_core_output->ltb_id[side_index]          = PA_INVALID_OBJ_ID;
   p_ltb_core_output->ltb_index[side_index]       = PA_INVALID_OBJ_INDEX;
   p_ltb_core_output->ltb_most_critical_side      = FBK_SIDE_UNDEFINED;

   /* Reset zone flags */
   p_ltb_core_output->ltb_f_obj_in_zone[side_index] = FBK_FALSE;
}
