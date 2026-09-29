/**
 * @file ta_constants.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module stores and initializes the constants to calculate the TTC (Time To Collision) and
 * also implements the exported functions of its header.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ta_constants.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include <assert.h>

/*============================================================================*\
 * EXPORTED FUNCTIONS
\*============================================================================*/

void Ta_Init_Prediction_Time_Step(Ta_Persistent_T *p_ta_persistent, const Ta_Core_Calibration_T *p_ta_cal)
{
   assert(NULL != p_ta_persistent);
   assert(NULL != p_ta_cal);

   /* Initialize value to zero */
   p_ta_persistent->ta_pred_step_dt = FBK_ZERO_F;

   /* Check if this value is not zero as well as not negativ, because this make no sense here. */
   if ((p_ta_cal->k_ta_prediction_steps_max > FBK_ZERO_UINT) && (p_ta_cal->k_ta_alert_lvl_2_ttc_threshold > FBK_ZERO_F))
   {
      /* calculate dT based on current CAL values  */
      p_ta_persistent->ta_pred_step_dt = p_ta_cal->k_ta_alert_lvl_2_ttc_threshold / ((float32_T) p_ta_cal->k_ta_prediction_steps_max);
   }
}
