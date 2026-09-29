#ifndef TA_COMMON_FUNCTIONS_H
#define TA_COMMON_FUNCTIONS_H

/**
 * @file ta_common_functions.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module contains common functions that are used by multiple other modules.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_reuse.h"
#include "ta_core_output_t.h"

/*============================================================================*\
 * EXPORTED FUNCTIONS PROTOTYPES
\*============================================================================*/

/**
 * @brief Resets the core output data.
 *
 * @return void
 *
 * @SRS{SF-2278}
 * @SAE{SF-3238}
 * @SDD{SF-8657}
 * @verification{Check that the core output is reset to its default.}
 */
void Ta_Reset_Core_Output(Ta_Core_Output_T *p_ta_core_output /**< TA Core Output */,
                          const uint8_t side_index /**< indicates ego side */);

#endif /* TA_COMMON_FUNCTIONS_H */
