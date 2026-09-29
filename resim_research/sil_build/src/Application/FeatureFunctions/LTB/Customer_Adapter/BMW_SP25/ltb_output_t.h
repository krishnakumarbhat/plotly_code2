#ifndef LTB_OUTPUT_T_H
#define LTB_OUTPUT_T_H

/**
 * @file ltb_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the BMW_SP25 output data structure for LTB.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ltb_bmw_sp25_types.h"
#include "ltb_types.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/


/**
 * @brief Ltb_Output_T structure.
 *
 *
 * @SRD{}
 * @SAD{}
 * @SDD{n/a}
 */
typedef struct
{
   Ltb_Alert_State_T LTB_bmw_sp25_alert_left;  /**< LTB alert level left */
   Ltb_Alert_State_T LTB_bmw_sp25_alert_right; /**< LTB alert level right */

   float32_T LTB_bmw_sp25_ttc_left;  /**< [s] time-to-collision left */
   float32_T LTB_bmw_sp25_ttc_right; /**< [s] time-to-collision right */
   float32_T LTB_bmw_sp25_ttb_left;  /**< [s] time-to-break left */
   float32_T LTB_bmw_sp25_ttb_right; /**< [s] time-to-break right */

} Ltb_Output_T;


#endif /* LTB_OUTPUT_T_H */
