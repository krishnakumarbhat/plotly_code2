#ifndef CTA_INPUT_T_H
#define CTA_INPUT_T_H

/**
 * @file cta_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for BMW_SP25 specific CTA input
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_bmw_sp25_types.h"
#include "cta_types.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * Cta_Input_T structure
 *
 * @SRS{SF-164}
 * @SAE{SF-2461}
 * @SDD{SF-3966}
 */
typedef struct
{
   Bmw_Ctb_Input_Bus_Signals_T bmw_ctb_input_signals; /* Contains all customer specific input signals from the vehicle bus */
   Bmw_Ctb_Input_Coding_T bmw_ctb_coding_parameters;  /* Contains all customer specific inputs from the coding parameter list */
} Cta_Input_T;


/*===========================================================================*\
 * Global Function Prototypess
\*===========================================================================*/

#endif /* CTA_INPUT_T_H */
