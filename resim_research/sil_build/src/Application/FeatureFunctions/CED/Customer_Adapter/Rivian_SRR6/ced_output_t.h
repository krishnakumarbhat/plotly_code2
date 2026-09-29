#ifndef CED_OUTPUT_T_H
#define CED_OUTPUT_T_H

/**
 * @file ced_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Rivian_SRR6 customer output declaration.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "pa_reuse.h"

/*===========================================================================*\
 * typedefs
\*===========================================================================*/

typedef struct
{
   float32_T ced_ttc_left;
   float32_T ced_ttc_right;
   float32_T ced_ttp_left;
   float32_T ced_ttp_right;
   uint8_t ced_id_left;
   uint8_t ced_id_right;
   uint8_t ced_alert_left;
   uint8_t ced_alert_right;

} Ced_Output_T;

#endif /* CED_OUTPUT_T_H */
