#ifndef TA_CONSTANTS_H
#define TA_CONSTANTS_H

/**
 * @file ta_constants.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module stores and initializes the constants to calculate the TTC (Time To Collision).
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ta_core_calibration_t.h"
#include "ta_persistent_t.h"

/* EXPORTED DEFINES FOR CONSTANTS --------------------------------------------*/

/**
 * @brief TTC (Time-To-Collision) value indicating an invalid or no TTC
 *
 * @SDD{SF-8674}
 * @verification{}
 */
#define TA_INVALID_TTC (100.0f)

/**
 * @brief TTP (Time-To-Pass) value indicating an invalid TTP
 *
 * @SDD{SF-8675}
 * @verification{}
 */
#define TA_INVALID_TTP (100.0f)

/**
 * @brief TTB (Time-To-Brake) value indicating an invalid TTB
 *
 * @SDD{SF-8673}
 * @verification{}
 */
#define TA_INVALID_TTB (100.0f)

/**
 * @brief Value indicating an invalid distance calculation
 *
 * @SDD{SF-8671}
 * @verification{}
 */
#define TA_INVALID_DISTANCE (100.0f)

/*============================================================================*\
 * EXPORTED FUNCTIONS PROTOTYPES
\*============================================================================*/

/**
 * @brief Init prediction time step
 *
 * @return void
 *
 * @SRS{SF-2278}
 * @SAE{}
 * @SDD{SF-8678}
 * @verification{If and only if k_ta_prediction_steps_max is greater than zero and k_ta_alert_lvl_2_ttc_threshold is greater than
 * zero, then a non default value for the prediction time step shall be set.}
 */
void Ta_Init_Prediction_Time_Step(Ta_Persistent_T *p_ta_persistent /**< TA Persistent data */,
                                  const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */);

#endif /* TA_CONSTANTS_H */
