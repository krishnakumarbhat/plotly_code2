#ifndef RECW_OUTPUT_T_H
#define RECW_OUTPUT_T_H

/**
 * @file recw_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the RECW output header file of the generic customer.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_reuse.h"
#include "recw_core_output_t.h"

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

typedef struct
{
   float32_T recw_crash_probability;        /**< [0-1] Crash probabilty of the object responsible for the RECW alert */
   float32_T recw_ttc_s;                    /**< [s] TTC (time to conflict) of the object responsible for the RECW alert */
   uint8_t recw_id;                         /**< ID of the object responsible for the RECW alert */
   uint32_t recw_unique_id;                 /**< Unique ID of the object responsible for the RECW alert */
   Recw_Alert_T recw_alert_level;           /** RECW Alert level */
   float32_T ttc_threshold_alert_level_1_s; /**< [s] Max possible TTC warning threshold based on current relative speed between ego
                                             and target for alert level 1 */
   float32_T ttc_threshold_alert_level_2_s; /**< [s] Max possible TTC warning threshold based on current relative speed between ego
                                             and target for alert level 2 */
} Recw_Output_T;

#endif /* RECW_OUTPUT_T_H */
