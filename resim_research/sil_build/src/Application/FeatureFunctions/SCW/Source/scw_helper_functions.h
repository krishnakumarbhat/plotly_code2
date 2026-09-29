#ifndef SCW_HELPER_FUNCTIONS_H
#define SCW_HELPER_FUNCTIONS_H

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

#include "fbk_field_of_interest.h"
#include "fbk_ref_point.h"


/*===========================================================================*\
* Defines
\*===========================================================================*/

/*===========================================================================*\
* Global Functions Prototypes
\*===========================================================================*/

/**
 * @brief Sets up the object's zone from given object corners.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-8189}
 * @verification{Create a test to check if the object zone is set correctly}
 */
void Scw_Set_Up_Object_Zone(Fbk_Field_Of_Interest_T *p_object_zone, const Fbk_Object_Corners_T *p_obj_target_corners);


#endif /* SCW_HELPER_FUNCTIONS_H */
