#ifndef CED_POST_RUN_H
#define CED_POST_RUN_H

/**
 * @file ced_post_run.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for all customer specific CED post run files.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================
 * Includes
 *=========================================================================*/

#include "ced_input_t.h"
#include "ced_instance.h"
#include "ced_output_t.h"

/*===========================================================================
 * External Function Prototypes
 *=========================================================================*/

/**
 * @brief Initialice Generic Ced_Output_T with default values
 *
 * @return void
 *
 * @SRS{SF-106}
 * @SAE{SF-2404}
 * @SDD{SF-3472}
 * @verification{}
 */
void Ced_Post_Run_Init(Ced_Instance_T *p_ced_instance);

/**
 * @brief Runs the customer specific CED pre-run.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2404}
 * @SDD{SF-3397}
 * @verification{}
 */
void Ced_Post_Run(Ced_Instance_T *p_ced_instance /**< CED instance */,
                  const Ced_Input_T *p_ced_input /**< CED input data */,
                  Ced_Output_T *p_ced_output /**< CED output data */
);

#endif /* CED_POST_RUN_H */
