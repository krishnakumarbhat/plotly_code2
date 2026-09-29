#ifndef LTB_CORE_INPUT_T_H
#define LTB_CORE_INPUT_T_H

/**
 * @file ltb_core_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the core input data structure for LTB.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ltb_types.h"
#include "pa_data.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Ltb_Core_Input_T structure
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53881}
 */
typedef struct
{
   const Pa_Data_T *p_pa_data; /**< Platform abstraction data */
   boolean_T f_ltb_enable;     /* Flag indicating the status of the LTB function */
} Ltb_Core_Input_T;

#endif /* LTB_CORE_INPUT_T_H */
