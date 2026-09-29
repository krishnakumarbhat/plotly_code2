#ifndef ESA_H
#define ESA_H

/**
 * @file esa.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Provides typedefs and main functions for the ESA feature.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "esa_core_calibration_t.h"
#include "esa_core_input_t.h"
#include "esa_core_output_t.h"
#include "esa_persistent_t.h"

/*===========================================================================*\
 * Global Functions	Definition
\*===========================================================================*/

/**
 * @brief Runs the ESA Core Algorithm if ESA is active, otherwise it resets the core feature.
 *
 * @return void
 *
 * @SRD{CSCSA-68625,CSCSA-68626,CSCSA-68629,CSCSA-69932}
 * @SAD{CSCSA-83853}
 * @SDD{CSCSA-65956}
 * @verification{Create a test where an object is a critical esa candidate and thus triggers an alert.}
 */
void Esa_Core_Run(Esa_Core_Output_T *p_esa_core_output /**< ESA core output data */,
                  Esa_Core_Input_T *p_esa_core_input /**< ESA core input data */,
                  Esa_Persistent_T *p_esa_persistent /**< ESA persistent */,
                  const Esa_Core_Calibration_T *p_esa_calibration /**< ESA calibration */);

/**
 * @brief Resets the ESA core output and persistent data.
 *
 * @return void
 *
 * @SRD{CSCSA-68596,CSCSA-68598,CSCSA-68599,CSCSA-72560}
 * @SAD{}
 * @SDD{CSCSA-65958}
 * @verification{Create a test which checks whether the internals of ESA as well as the output is reset correctly.}
 */
void Esa_Reset(Esa_Core_Input_T *p_esa_core_input /**< ESA core input */,
               Esa_Core_Output_T *p_esa_core_output /**< ESA core output */,
               Esa_Persistent_T *p_esa_persistent /**< ESA persistent */);

/**
 * @brief Resets ESA persistent to its default.
 *
 * @return void
 *
 * @SRD{}
 * @SAD{CSCSA-83853}
 * @SDD{CSCSA-65955}
 * @verification{Check whether ESA persistent values are correctly set to their defaults.}
 */
void Esa_Reset_Persistent_Data(Esa_Persistent_T *p_esa_persistent);

#endif /* ESA_H */
