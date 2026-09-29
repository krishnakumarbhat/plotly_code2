#ifndef LCDA_OUTPUT_T_H
#define LCDA_OUTPUT_T_H

/**
 * @file lcda_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the LCDA output header file of the STLA_Thunder customer.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_macros.h"
#include "lcda_core_output_t.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

typedef struct
{
   uint8_t f_lcda_enabled;
   uint8_t f_bsw_enabled;
   uint8_t f_cvw_enabled;

   uint8_t bsw_alert_left;
   uint8_t bsw_id_left;
   uint8_t bsw_alert_right;
   uint8_t bsw_id_right;
   float32_T bsw_distance_left;
   float32_T bsw_distance_right;
   boolean_T f_bsw_hold_alert[FBK_NUMBER_OF_SIDES];

   uint8_t cvw_alert_left;
   uint8_t cvw_id_left;
   float32_T cvw_ttc_left;
   uint8_t cvw_alert_right;
   uint8_t cvw_id_right;
   float32_T cvw_ttc_right;
   float32_T cvw_distance_left;
   float32_T cvw_distance_right;
   boolean_T f_cvw_hold_alert[FBK_NUMBER_OF_SIDES];

   Lcda_Status_T lcda_status;
} Lcda_Output_T;

#endif /* LCDA_OUTPUT_T_H */
