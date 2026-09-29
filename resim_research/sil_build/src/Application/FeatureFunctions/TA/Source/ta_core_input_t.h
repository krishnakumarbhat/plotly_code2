#ifndef TA_CORE_INPUT_T_H
#define TA_CORE_INPUT_T_H

/**
 * @file ta_core_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the core input data structure for TA.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_data.h"
#include "pa_reuse.h"
#include "ta_types.h"

/**
 * @brief Ta_Core_Input_T structure
 *
 * @SRS{SF-2365}
 * @SDD{SF-8637}
 */
typedef struct
{
   boolean_T f_ta_enable;  /**< Global enable flag for TA */
   boolean_T f_fta_enable; /**< Flag to enable Front Turn Assist (FTA) */
   boolean_T f_rta_enable; /**< Flag to enable Rear Turn Assist (RTA) */

   boolean_T f_enable_debug_mode;            /**< Flag to enable debug mode */
   float32_T debug_mode_obj_pos_long_offset; /**< Object position offset for debug mode */
   float32_T debug_mode_obj_pos_lat_offset;  /**< Object position offset for debug mode */

   float32_T alert_ttp_threshold; /**< Alert threshold for TTP */

   const Pa_Data_T *p_pa_data; /**< Context containing the vehicle and tracker data */

} Ta_Core_Input_T;

#endif /* TA_CORE_INPUT_T_H */
