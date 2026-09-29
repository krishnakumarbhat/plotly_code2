#ifndef RECW_INPUT_T_H
#define RECW_INPUT_T_H

/**
 * @file recw_input_t.t
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains type definitions for generic recw input .
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_data.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Definition of the Recw_Input_T structure
 */
typedef struct
{
   uint8_t f_recw_enable; /**< Flag indicating if the RECW feature should be operational */
} Recw_Input_T;

#endif /* RECW_INPUT_T_H */
