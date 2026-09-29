#ifndef RECW_CORE_OUTPUT_T_H
#define RECW_CORE_OUTPUT_T_H

/**
 * @file recw_core_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the RECW core output tag header file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_reuse.h"

/*===================================================================*\
* Enums
\*===================================================================*/

/**
 * @brief Contains Recw alert level
 *
 * @SDD{SF-7877}
 */
typedef enum
{
   RECW_NO_ALERT             = (0),
   RECW_ALERT_ACTIVE_LEVEL_1 = (1),
   RECW_ALERT_ACTIVE_LEVEL_2 = (2)
} Recw_Alert_T;

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Recw_Core_Output_T structure
 *
 * Defines core output interface data
 *
 * @SRS{SF-1697}
 * @SAE{SF-2985}
 * @SDD{SF-7822}
 */
typedef struct
{
   float32_T recw_crash_prob_braking;  /** Probability to avoid collision by object breaking */
   float32_T recw_crash_prob_steering; /** Probability to avoid collision by object steering */
   float32_T recw_crash_prob_combined; /** Probability to avoid collision combined */

   float32_T recw_ttc;      /** Time to collision */
   uint8_t recw_index;      /** Radar Tracker index */
   uint8_t recw_id;         /** Radar Tracker ID */
   uint32_t recw_unique_id; /** Radar Tracker unique ID */

   Recw_Alert_T recw_alert_level; /** RECW Alert level */

   float32_T ttc_threshold_alert_level_1; /** Threshold for level 1 active alert */
   float32_T ttc_threshold_alert_level_2; /** Threshold for level 2 active alert */

} Recw_Core_Output_T;

#endif /* RECW_CORE_OUTPUT_T_H */
