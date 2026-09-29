#ifndef TA_H
#define TA_H

/**
 * @file ta.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements the functions that are called by the platform SW
 * for initialization, cyclic execution and obtaining outputs of the TA.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_ego_traj_predictor_instance.h"
#include "ta_core_calibration_t.h"
#include "ta_core_input_t.h"
#include "ta_core_output_t.h"
#include "ta_persistent_t.h"
/*===========================================================================*\
* Global Function Prototypes
\*===========================================================================*/

/**
 * @brief This function calls the core algorithm
 *
 * @return void
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{SF-3238}
 * @SDD{SF-8658}
 * @verification{Test that no active alert is set when no valid object data is provided.}
 */
void Ta_Core_Run(Ta_Core_Output_T *p_ta_core_output /**< TA Core Output */,
                 const Ta_Core_Input_T *p_ta_core_input /**< TA Core Input */,
                 const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */,
                 Fbk_Ego_Traj_Predictor_Instance_T *p_ego_traj_predictor_instance,
                 Ta_Persistent_T *p_ta_persistent);

/**
 * @brief Resets the TA data structures (input, output) to default values.
 *
 * @return void
 *
 * @SRS{SF-2278}
 * @SAE{}
 * @SDD{SF-8656}
 * @verification{Check that the core output, prediction times and output is reset.}
 */
void Ta_Reset(Ta_Core_Output_T *p_ta_core_output /**< TA Core Output */,
              const Ta_Core_Calibration_T *p_ta_cal /**< TA Calibration */,
              Ta_Persistent_T *p_ta_persistent);

#endif /* TA_H */
