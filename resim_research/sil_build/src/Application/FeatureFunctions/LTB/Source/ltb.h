#ifndef LTB_H
#define LTB_H

/**
 * @file ltb.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Provides typedefs and main functions for the LTB feature.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "fbk_ego_traj_predictor_instance.h"
#include "ltb_core_calibration_t.h"
#include "ltb_core_input_t.h"
#include "ltb_core_output_t.h"
#include "ltb_persistent_t.h"

/*===========================================================================*\
 * Global Functions	Definition
\*===========================================================================*/

/**
 * @brief Runs the LTB Core Algorithm if LTB is active, otherwise it resets the core feature.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53897}
 * @verification{Create a test where an object is a critical ltb candidate and thus triggers an alert.}
 */
void Ltb_Core_Run(Ltb_Core_Output_T *p_ltb_core_output /**< LTB core output data */,
                  const Ltb_Core_Input_T *p_ltb_core_input /**< LTB core input data */,
                  const Ltb_Core_Calibration_T *p_ltb_cals /**< LTB calibration */,
                  Ltb_Persistent_T *p_ltb_persistent,
                  Fbk_Ego_Traj_Predictor_Instance_T *p_ego_traj_predictor_instance);

/**
 * @brief Resets the LTB core output and persistent data.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53895}
 * @verification{Create a test which checks whether the internals of LTB as well as the output is reset correctly.}
 */
void Ltb_Reset(Ltb_Core_Output_T *p_ltb_core_output /**< LTB core output data */,
               const Ltb_Core_Calibration_T *p_ltb_cals /**< LTB calibration */,
               Ltb_Persistent_T *p_ltb_persistent /** LTB persistent */);

#endif /* LTB_H */
