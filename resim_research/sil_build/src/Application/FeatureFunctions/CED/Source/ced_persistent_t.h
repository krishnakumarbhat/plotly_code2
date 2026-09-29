#ifndef CED_PERSISTENT_T_H
#define CED_PERSISTENT_T_H

/**
 * @file ced_persistent_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the persistent data structure.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_types.h"
#include "fbk_macros.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Ced_Persistent_T structure
 * @SDD{SF-3559}
 */
typedef struct
{
   /* CED object specific */
   float32_T ced_object_heading_predicted[CED_OBJ_MAX_ARRAY_SIZE];

   /* CED side specific */
   Ced_Alert_T ced_side_alert_prev_cycle[FBK_NUMBER_OF_SIDES];
   uint8_t ced_side_id_prev_cycle[FBK_NUMBER_OF_SIDES];
   uint32_t ced_side_unique_id_prev_cycle[FBK_NUMBER_OF_SIDES];
   uint8_t ced_side_path_match_index_prev_cycle[FBK_NUMBER_OF_SIDES];
   uint8_t ced_side_alert_qualifying_counter[FBK_NUMBER_OF_SIDES];
   uint8_t ced_side_alert_holding_counter[FBK_NUMBER_OF_SIDES];

   uint8_t ced_side_direction_prev_cycle[FBK_NUMBER_OF_SIDES];

} Ced_Persistent_T;

#endif /* CED_PERSISTENT_T_H */
