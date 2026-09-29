#ifndef LTB_INPUT_T_H
#define LTB_INPUT_T_H

/**
 * @file ltb_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the BMW_SP25 input data structure for LTB.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ltb_bmw_sp25_types.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Ltb_Input_T structure.
 *
 *
 * @SRD{}
 * @SAD{}
 * @SDD{n/a}
 */
typedef struct
{
   boolean_T f_ltb_enable;                      /**< Flag indicating the status of the LTB function */
   Ltb_Vehicle_Inputs_T ltb_vehicle_parameters; /**< Vehicle Parameters for LTB */
   Bmw_Ltb_Error_T ltb_error;                   /*Error input for LTB */

} Ltb_Input_T;


#endif /* LTB_INPUT_T_H */
