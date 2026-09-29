#ifndef RECW_CRASH_PROB_STEER_BRAKE_H
#define RECW_CRASH_PROB_STEER_BRAKE_H

/**
 * @file recw_crash_prob_steer_brake.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is crash probability calculation header file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_guardrail_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "pa_const_macros.h"
#include "recw_core_calibration_t.h"
#include "recw_types.h"

/*===========================================================================*\
* Global function declaration
\*===========================================================================*/

/**
 * @brief   Calculates accelerations necessary to avoid a crash based on pure steering and breaking respectively.
 *          Uses a look-up approach to match the accelerations to crash probabilities for each maneuver.
 *          Combines the two resulting probabilities in order to achieve overall crash probability.
 *
 * @return void
 *
 * @SRS{SF-1705}
 * @SAE{SF-2959}
 * @SDD{SF-7886}
 * @verification{}
 */
void Recw_Calculate_Crash_Probabilities(
   Recw_Object_T *p_recw_object /**< RECW object */,
   const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
   const Fbk_Guardrail_Data_T guardrail_data[PA_OBJ_NUMBER_OF_GUARDRAILS] /**< FBK guard rail data */,
   const Recw_Core_Calibration_T *p_cals /**< RECW calibrations */);

#endif /* RECW_CRASH_PROB_STEER_BRAKE_H */
