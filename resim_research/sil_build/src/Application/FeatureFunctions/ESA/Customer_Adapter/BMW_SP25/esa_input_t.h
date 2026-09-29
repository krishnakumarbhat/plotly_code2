#ifndef ESA_INPUT_T_H
#define ESA_INPUT_T_H

/**
 * @file esa_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the BMW_SP25 input data structure for ESA.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
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
 * @SRD{}
 * @SAD{}
 * @SDD{n/a}
 */
typedef struct
{

   boolean_T f_esa_enabled; /**< Flag indicating the status of the ESA function */

} Esa_Input_T;


#endif /* ESA_INPUT_T_H */
