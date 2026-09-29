#ifndef LTB_POST_RUN_H
#define LTB_POST_RUN_H

/**
 * @file ltb_post_run.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for all customer specific LTB post run files.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/*===========================================================================
 * Includes
 *=========================================================================*/

#include "ltb_input_t.h"
#include "ltb_instance.h"
#include "ltb_output_t.h"

/*===========================================================================
 * External Function Prototypes
 *=========================================================================*/

/**
 * @brief Runs the customer specific LTB pre-run.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-54001}
 * @verification{}
 */
void Ltb_Post_Run(const Ltb_Instance_T *p_ltb_instance /** LTB instance */,
                  Ltb_Output_T *p_ltb_output /**< LTB output data */,
                  const Ltb_Input_T *p_ltb_input /**< LTB input data */);

#endif /* LTB_POST_RUN_H */
