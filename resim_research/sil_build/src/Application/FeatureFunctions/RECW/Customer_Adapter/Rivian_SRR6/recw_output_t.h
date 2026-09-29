#ifndef RECW_OUTPUT_T_H
#define RECW_OUTPUT_T_H
/**
 * @file recw_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the RECW output header file of the Rivian_SRR6 customer.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_reuse.h"

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

typedef struct
{
   float32_T recw_crash_prob_braking;
   float32_T recw_crash_prob_steering;
   float32_T recw_crash_prob_combined;

   float32_T recw_ttc;
   uint8_t recw_index;
   uint8_t recw_id;

   uint8_t recw_alert_level;

   float32_T ttc_threshold_alert_level_1;
   float32_T ttc_threshold_alert_level_2;

} Recw_Output_T;

#endif /* RECW_OUTPUT_T_H */
