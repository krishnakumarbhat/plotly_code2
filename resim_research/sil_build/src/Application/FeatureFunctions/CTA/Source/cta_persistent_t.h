#ifndef CTA_PERSISTENT_T_H
#define CTA_PERSISTENT_T_H

/**
 * @file cta_persistent_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Exports type definitions of persistent data structures of Cta.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_types.h"
#include "fbk_macros.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Summarizes persistent variables of the CTA feature function
 *
 * @SAE{SF-2459}
 * @SDD{SF-3692}
 */
typedef struct
{
   boolean_T f_function_execution_state; /**< Flag indicating whether CTA has been executed in the previous cycle*/
   uint8_t warning_holding_counter[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];    /**< Holding counter for each approach side*/
   uint8_t brake_holding_counter[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];      /**< Holding counter for the brake qualifier */
   uint8_t brake_suppression_counter[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];  /**< Counter used for suppression/qualification of brake
                                                                              qualifier */
   boolean_T previous_brake_qualifier[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES]; /**< Brake qualifier of each side of the last cycle*/
   Cta_Crit_Level_T previous_crit_level[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];  /**< last criticality level for each approach side*/
   uint8_t previous_most_critical_obj_id[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES]; /**< last object id for each side*/
   uint32_t previous_most_critical_unique_obj_id[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES]; /**< last object id for each side*/
} Cta_Persistent_T;

#endif /* CTA_PERSISTENT_T_H */
