#ifndef ESA_PERSISTENT_T_H
#define ESA_PERSISTENT_T_H

/**
 * @file esa_persistent_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the persistent data structure.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "esa_types.h"
#include "fbk_macros.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Esa_Persistent_T structure
 *
 * @SDD{CSCSA-65126}
 */
typedef struct
{
   boolean_T f_host_speed_in_activation_range; /**< Flag indicating if the host speed is in the activation range */
   boolean_T f_esa_disabled_low_curve_radius;  /**< Flag indicating the ESA is disabled because of a low curve radius */

   /* ESA object specific */
   uint8_t mature_count_in_esa_zone[PA_OBJ_NUMBER_OF_OBJECTS]; /**< Count of how many cycles an object was in the ESA zone with
                                                                  status MATURE. */
   /* ESA side specific */
   uint8_t prev_esa_alert_obj_index[FBK_NUMBER_OF_SIDES]; /**< Index of the target which previously triggered a ESA alert. Value
                                                              is 255 when no previous ESA alert */
   uint8_t prev_esa_alert_obj_id[FBK_NUMBER_OF_SIDES];    /**< ID of the target which previously triggered a ESA alert. Value is 0
                                                           when no previous ESA alert */
   uint8_t esa_hold_counter[FBK_NUMBER_OF_SIDES];         /**< Counter used to hold alert after conditions are no longer met*/
} Esa_Persistent_T;

#endif /* ESA_PERSISTENT_T_H */
