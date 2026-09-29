#ifndef SCW_POST_RUN_H
#define SCW_POST_RUN_H

/**
 * @file scw_post_run.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for all customer specific SCW post run files.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "scw_input_t.h"
#include "scw_instance_t.h"
#include "scw_output_t.h"

/*===========================================================================*\
* Global Function Definition
\*===========================================================================*/

/**
 * @brief This function provides the reset routine for customer specific outputs.
 * This function can be adapted for every customer to reset the values of the
 * output struct to their defaults.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2992}
 * @SDD{SF-8187}
 * @verification{Test that output is initialized correctly}
 */
void Scw_Post_Run_Init(Scw_Instance_T *p_scw_instance /**< SCW instance */);

/**
 * @brief This function post processes data after running the SCW core algorithm.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2992}
 * @SDD{SF-8156}
 * @verification{Test that output is mapped correctly}
 */
void Scw_Post_Run(const Scw_Instance_T *p_scw_instance /**< SCW instance */,
                  const Scw_Input_T *p_scw_input /**< SCW input */,
                  Scw_Output_T *p_scw_output /**< SCW output */);

#endif /* SCW_POST_RUN_H */
