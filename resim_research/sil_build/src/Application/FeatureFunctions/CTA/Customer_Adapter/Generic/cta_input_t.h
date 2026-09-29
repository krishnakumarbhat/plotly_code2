#ifndef CTA_INPUT_T_H
#define CTA_INPUT_T_H

/**
 * @file cta_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Generic customer input declaration.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
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
 * structure providing the input interface to the feature function
 */
typedef struct
{
   boolean_T f_cta_enable;
   boolean_T f_rear_cta_enable;
   boolean_T f_front_cta_enable;
} Cta_Input_T;

#endif
