#ifndef ESA_INPUT_T_H
#define ESA_INPUT_T_H

/**
 * @file esa_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Generic customer input declaration.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_reuse.h"

/*===========================================================================*\
 * typedefs
\*===========================================================================*/

/**
 * @brief Esa_Input_T structure.
 *
 *
 * @SRD{CSCSA-122858}
 * @SAD{CSCSA-123106}
 * @SDD{CSCSA-123047}
 * @verification{Unit test: Set generic Esa_Input_T by non-defaults, run Esa_Pre_Run_Init function, verify if values are set to
 * default.}
 */
typedef struct
{

   boolean_T f_esa_enabled; /* Flag indicating the status of the ESA function */

} Esa_Input_T;

#endif /* ESA_INPUT_T_H */
