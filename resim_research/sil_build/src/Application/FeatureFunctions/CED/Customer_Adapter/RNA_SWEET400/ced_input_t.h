#ifndef CED_INPUT_T_H
#define CED_INPUT_T_H

/**
 * @file ced_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the CED input header file of the RNA_SWEET400 customer.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
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
 * @brief Ced_Input_T RNA_SWEET400 specific CED input data
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-3485}
 */
typedef struct
{
   uint8_t f_CED_Status; /* Flag indicating the status of the CED function */
} Ced_Input_T;


/*===========================================================================*\
 * Global Function Prototypess
\*===========================================================================*/


#endif /* CED_INPUT_T_H */
