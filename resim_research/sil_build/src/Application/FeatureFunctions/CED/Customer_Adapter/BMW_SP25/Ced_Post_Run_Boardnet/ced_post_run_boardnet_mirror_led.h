#ifndef CED_POST_RUN_BOARDNET_MIRROR_LED_H
#define CED_POST_RUN_BOARDNET_MIRROR_LED_H

/**
 * @file ced_post_run_boardnet_mirror_led.h
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
 * @brief The main function for setting the mirror lights dependent on the inputs and core warnings.
 *
 * @return filling p_output_occupant_detection
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
void Ced_Control_Mirror_Led(Ced_Output_T *p_ced_output, const Ced_Core_Output_T *p_ced_core_output, const Ced_Input_T *p_ced_input);

#endif /* CED_POST_RUN_BOARDNET_MIRROR_LED_H */
