#ifndef CED_POST_RUN_BOARDNET_H
#define CED_POST_RUN_BOARDNET_H

/**
 * @file ced_post_run_boardnet.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the output adaptations for the boardnet signals.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_input_t.h"
#include "ced_output_t.h"

/*===========================================================================*\
* External Function Prototypes
\*===========================================================================*/

/**
 * @brief Fills the output with the information about is there is a occupant who might leave the ego vehicle.
 *
 * @return filling p_output_occupant_detection
 *
 * @SRS{n/a}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
void Ced_Check_And_Set_Occupant_Detection(Ced_Output_Occupant_Detection_T *p_output_occupant_detection,
                                          const Ced_Bmw_Boardnet_T *p_bmw_boardnet);

#endif /* CED_POST_RUN_BOARDNET_H */
