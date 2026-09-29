#ifndef CTA_OUTPUT_T_H
#define CTA_OUTPUT_T_H

/**
 * @file cta_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Generic customer output declaration.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_core_output_t.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/
typedef struct
{
   uint8_t id;
   uint32_t unique_id;
   float32_T objPoseX_m;
   float32_T objPoseY_m;
   float32_T objVelocityX_mps;
   float32_T objVelocityY_mps;
   float32_T heading_rad;

   Cta_Crit_Level_T alert_level;
   float32_T ttc_s;
   float32_T intersection_point_x_m;
   boolean_T f_brake_qualifier;
   boolean_T f_standstill_qualifier;
   float32_T brake_deceleration;
} Cta_Critical_Object_T;

typedef struct
{
   boolean_T f_cta_enabled;
   Cta_Critical_Object_T most_critical_object_by_sides[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];
} Cta_Output_T;

#endif
