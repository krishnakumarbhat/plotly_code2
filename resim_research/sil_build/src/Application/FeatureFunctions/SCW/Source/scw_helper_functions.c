/**
 * @file scw_helper_functions.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains SCW specific helper functions.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "scw_helper_functions.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include <assert.h>

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

void Scw_Set_Up_Object_Zone(Fbk_Field_Of_Interest_T *p_object_zone, const Fbk_Object_Corners_T *p_obj_target_corners)
{
   assert(NULL != p_object_zone);
   assert(NULL != p_obj_target_corners);

   /* Set up object zone from target corners (avoids recalculating the corner coordinates). */
   p_object_zone->size      = 4u;
   p_object_zone->points[0] = p_obj_target_corners->points[FBK_FRONT_LEFT_CORNER];
   p_object_zone->points[1] = p_obj_target_corners->points[FBK_FRONT_RIGHT_CORNER];
   p_object_zone->points[2] = p_obj_target_corners->points[FBK_REAR_RIGHT_CORNER];
   p_object_zone->points[3] = p_obj_target_corners->points[FBK_REAR_LEFT_CORNER];
}
