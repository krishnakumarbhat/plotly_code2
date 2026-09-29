#ifndef CED_INPUT_T_H
#define CED_INPUT_T_H

/**
 * @file ced_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Generic customer input declaration.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_reuse.h"
#include "pt_iface.h"

/*===========================================================================*\
 * typedefs
\*===========================================================================*/

/**
 * @brief Ced_Input_T structure.
 *
 *
 * @SRS{SF-64}
 * @SAE{SF-2431}
 * @SDD{SF-3462}
 */
typedef struct
{
   boolean_T f_ced_enable; /* Flag indicating if OSE is enabled */
} Ced_Input_T;

#endif /* CED_INPUT_T_H */
