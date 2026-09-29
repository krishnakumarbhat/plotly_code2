#ifndef SCW_H
#define SCW_H

/**
 * @file scw.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implements the functions that are called by the platform SW
 * for initialization, cyclic execution and obtaining outputs of the SCW.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "scw_core_calibration_t.h"
#include "scw_core_input_t.h"
#include "scw_core_output_t.h"
#include "scw_persistent_t.h"

/*===========================================================================*\
* Global Functions Definition
\*===========================================================================*/

/**
 * @brief Resets the SCW data structures to default values.
 *
 * @return void
 *
 * @SRS{SF-2114}
 * @SAE{SF-2990}
 * @SDD{SF-8051}
 * @verification{Create a test to check if scw is reset properly}
 */
void Scw_Reset(Scw_Core_Input_T *p_scw_core_input /**< Scw core input*/,
               Scw_Core_Output_T *p_scw_core_output /**< Scw core output */,
               Scw_Persistent_T *p_scw_persistent /**< Scw persistent */,
               const Scw_Core_Calibration_T *p_scw_cal);

/**
 * @brief This function calls the SCW core algorithm
 *
 * @return void
 *
 * @SRS{SF-2123}
 * @SAE{SF-2990}
 * @SDD{SF-8050}
 * @verification{Create a test to check if scw core is being invoked properly}
 */
void Scw_Core_Run(Scw_Core_Output_T *p_scw_core_output /**< Scw Core Output */,
                  Scw_Core_Input_T *p_scw_core_input /**< Scw Core Input */,
                  Scw_Persistent_T *p_scw_persistent /**< Scw persistent */,
                  const Scw_Core_Calibration_T *p_scw_cal /**< Scw Calibration */);

#endif /* SCW_H */
