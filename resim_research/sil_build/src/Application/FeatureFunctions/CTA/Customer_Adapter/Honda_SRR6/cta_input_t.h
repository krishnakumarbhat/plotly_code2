#ifndef CTA_INPUT_T_H
#define CTA_INPUT_T_H

/**
 * @file cta_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Honda SRR6 customer input declaration.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
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
 * @brief Structure summarizing the CTA input for Honda SRR6.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4012}
 */
/* coverity[misra_c_2012_rule_2_4_violation] */
typedef struct Cta_Input_Tag
{
   uint8_t f_cta_switch; /**< Flag indicating if the CTA feature should be operational */
} Cta_Input_T;

#endif
