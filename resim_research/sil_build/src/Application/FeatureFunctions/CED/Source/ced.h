#ifndef CED_H
#define CED_H

/**
 * @file ced.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Provides typedefs and main functions for the CED feature.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "ced_core_calibration_t.h"
#include "ced_core_input_t.h"
#include "ced_core_output_t.h"
#include "ced_persistent_t.h"
#include "fbk_index_lookup.h"
#include "fbk_output.h"

/*===========================================================================*\
 * Global Functions	Definition
\*===========================================================================*/

/**
 * @brief Runs the CED Core Algorithm if CED is active, otherwise it resets the core feature.
 *
 * @return void
 *
 * @SRS{SF-81,SF-119,SF-120,CSCSA-120933,CSCSA-120934,CSCSA-122790}
 * @SAE{SF-2402}
 * @SDD{SF-3584}
 * @verification{Create a test where an object is a critical ced candidate and thus triggers an alert.}
 */
void Ced_Core_Run(Ced_Core_Output_T *p_ced_core_output /**< CED core output data */,
                  const Ced_Core_Input_T *p_ced_core_input /**< CED core input data */,
                  Ced_Persistent_T *p_ced_persistance,
                  const Ced_Core_Calibration_T *p_ced_cal /**< CED calibration */,
                  const Fbk_Output_T *p_fbk_output /**< FBK output data */);

/**
 * @brief Resets the CED core output and persistent data.
 *
 * @return void
 *
 * @SRS{SF-82}
 * @SAE{SF-2402}
 * @SDD{SF-3585}
 * @verification{Create a test which checks whether the internals of CED as well as the output is reset correctly.}
 */
void Ced_Reset(Ced_Core_Output_T *p_ced_core_output /**< CED core output data */,
               Ced_Persistent_T *p_ced_persistent, /* CED persitent data */
               const Fbk_Index_Id_Lookup_Table_T *p_index_id_lookup_table /**< List of objects from FBK  */);

#endif /* CED_H */
