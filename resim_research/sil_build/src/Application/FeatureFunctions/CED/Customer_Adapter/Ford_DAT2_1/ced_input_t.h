#ifndef CED_INPUT_T_H
#define CED_INPUT_T_H

/**
 * @file ced_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Ford DAT2.1 customer input.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_output_t.h"
#include "pa_reuse.h"
#include "pt_iface.h"


/*===========================================================================*\
 * typedefs
\*===========================================================================*/

/**
 * @brief Ced_Input_T structure.
 *
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-3439}
 * @verification{}
 */
typedef struct
{

   uint8_t f_ced_status; /* Flag indicating the status of the CED function*/

   /** enabled flags */
   boolean_T f_ced_enable;     /* Flag indicating the status of the CED function */
   boolean_T f_ced_front_mode; /* Flag indicating the front mode is enabled */
   boolean_T f_ced_rear_mode;  /* Flag indicating the rear mode is enabled */

   Ced_Output_T *p_ford_ced_output; /* CED output pointer for Ford-specific architecture */


} Ced_Input_T;

#endif /* CED_INPUT_T_H */
