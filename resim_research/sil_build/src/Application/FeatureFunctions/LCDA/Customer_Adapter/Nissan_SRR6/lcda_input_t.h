#ifndef LCDA_INPUT_T_H
#define LCDA_INPUT_T_H

/**
 * @file lcda_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the Nissan SRR6 input data structure for LCDA.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_data.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

/**
 * @brief Lcda_Input_T structure
 *
 *
 * @SRS{SF-1039}
 * @SAE{SF-2801}
 * @SDD{SF-6949}
 */
typedef struct
{
   uint8_t f_lcda_enable;     /* enable/disable LCDA feature */
   uint8_t f_lcda_enable_bsw; /* enable/disable LCDA BSW subfeature */
   uint8_t f_lcda_enable_cvw; /* enable/disable LCDA CVW subfeature */

} Lcda_Input_T;

#endif /* LCDA_INPUT_T_H */
