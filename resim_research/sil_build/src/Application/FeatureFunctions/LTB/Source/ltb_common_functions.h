#ifndef LTB_COMMON_FUNCTIONS_H
#define LTB_COMMON_FUNCTIONS_H

/**
 * @file ltb_common_functions.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains common functions that are used by multiple other modules.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ltb_core_output_t.h"
#include "pa_reuse.h"

/*============================================================================*\
 * EXPORTED FUNCTIONS PROTOTYPES
\*============================================================================*/

/**
 * @brief Resets all Core output data at given side except for alert and direction.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53902}
 * @verification{Create a test which checks whether the core output side data is correctly reset to its default.}
 */
void Ltb_Reset_Core_Output_Side_Data(Ltb_Core_Output_T *p_ltb_core_output /**< LTB core output */,
                                     uint8_t side_index /**< side index */);

#endif /* LTB_COMMON_FUNCTIONS_H */
