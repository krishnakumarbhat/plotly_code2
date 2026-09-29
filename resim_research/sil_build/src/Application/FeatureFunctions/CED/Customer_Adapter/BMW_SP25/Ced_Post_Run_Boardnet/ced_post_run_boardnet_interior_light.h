#ifndef CED_POST_RUN_BOARDNET_INTERIOR_LIGHT_H
#define CED_POST_RUN_BOARDNET_INTERIOR_LIGHT_H

/**
 * @file ced_post_run_boardnet_interior_light.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the output adaptations for the boardnet signals.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_core_output_t.h"
#include "ced_input_t.h"
#include "ced_output_t.h"

/*===========================================================================*\
* External Function Prototypes
\*===========================================================================*/

/**
 * @brief The main function for setting the interior lights dependent on the inputs and core warnings.
 *
 * @return filling p_ced_output
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
void Ced_Control_Interior_Lights(Ced_Output_T *p_ced_output, const Ced_Core_Output_T *p_ced_core_output, const Ced_Input_T *p_ced_input);

#endif /* CED_POST_RUN_BOARDNET_INTERIOR_LIGHT_H */
