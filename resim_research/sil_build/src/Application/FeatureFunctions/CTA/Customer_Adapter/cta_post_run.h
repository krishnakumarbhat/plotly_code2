#ifndef CTA_POST_RUN_H
#define CTA_POST_RUN_H

/**
 * @file cta_post_run.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for all customer specific CTA post run files.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================
 * Includes
 *=========================================================================*/

#include "cta_input_t.h"
#include "cta_instance.h"
#include "cta_output_t.h"

/*===========================================================================
 * External Function Prototypes
 *=========================================================================*/

/**
 * @brief Initializes the post run of the given customer interface.
 *
 * @return void
 *
 * @SRS{SF-169,SF-167}
 * @SAE{SF-2458}
 * @SDD{SF-3954}
 * @verification{}
 */
void Cta_Post_Run_Init(Cta_Instance_T *p_cta_instance /**< CTA instance */);

/**
 * @brief This function post processes data after running the CTA core algorithm.
 *
 * @return void
 *
 * @SRS{SF-163}
 * @SAE{SF-2458}
 * @SDD{SF-3947}
 * @verification{}
 */
void Cta_Post_Run(Cta_Instance_T *p_cta_instance, const Cta_Input_T *p_cta_input, Cta_Output_T *p_cta_output);

#endif /* CTA_POST_RUN_H */
