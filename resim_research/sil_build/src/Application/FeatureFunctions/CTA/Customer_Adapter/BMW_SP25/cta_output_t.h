#ifndef CTA_OUTPUT_T_H
#define CTA_OUTPUT_T_H

/**
 * @file cta_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for BMW_SP25 specific CTA output
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_bmw_sp25_types.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * Cta_Output_T structure
 *
 * Note: Cta_Output_T structure has to be 4-byte aligned for resim. Do not remove Unused bytes added
 *
 * @SRS{SF-163}
 * @SAE{SF-2462}
 * @SDD{SF-3965}
 */
typedef struct
{
   /* Algo Post Run outputs */
   Bmw_Ctb_Output_Algo_State_T bmw_ctb_output_algo_state; /* Contains BMW specific requested content (not mapped to Bus-Signals) */
   Bmw_Ctb_Output_Edr_T bmw_ctb_output_edr;               /* Contains BMW specific requested content (not mapped to Bus-Signals) */

   /* Statemachine Post Run outputs */
   Bmw_Ctb_Output_Bus_Signals_T bmw_ctb_output_bus_signals; /* Contains BMW specific requested outputs mapped to the specific
                                                               Bus-Signal format */
} Cta_Output_T;


/*===========================================================================*\
* Global Function Prototypess
\*===========================================================================*/


#endif /* CTA_OUTPUT_T_H */
