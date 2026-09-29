#ifndef CTA_INPUT_T_H
#define CTA_INPUT_T_H

/**
 * @file cta_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Nissan_SRR6 input declaration.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_types.h"
#include "pa_context.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief structure providing the input interface to the feature function.
 */
typedef struct
{
   uint8_t f_cta_switch; /* Flag indicating if the CTA feature should be operational */
} Cta_Input_T;

#endif
