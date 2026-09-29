#ifndef CED_INPUT_T_H
#define CED_INPUT_T_H

/**
 * @file ced_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Generic customer input declaration.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
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
 * @brief Generic Ced_Input_T structure.
 *
 *
 * @SRS{CSCSA-122154}
 * @SAD{CSCSA-121740}
 * @SDD{CSCSA-122324}
 */
typedef struct
{
   boolean_T f_ced_enable; /* Flag indicating the status of the CED function */

   boolean_T f_ced_front_mode; /* Flag indicating the front mode is enabled */
   boolean_T f_ced_rear_mode;  /* Flag indicating the rear mode is enabled */
} Ced_Input_T;

#endif /* CED_INPUT_T_H */
