#ifndef CTA_INPUT_T_H
#define CTA_INPUT_T_H

/**
 * @file cta_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief STLA_Thunder customer input declaration.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_types.h"
#include "fbk_field_of_interest.h"
#include "pa_context.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

typedef struct
{
   uint8_t f_cta_enable;
} Cta_Input_T;

#endif
