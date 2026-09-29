#ifndef ESA_POST_RUN_H
#define ESA_POST_RUN_H

/**
 * @file esa_post_run.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for all customer specific ESA post run files.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/*===========================================================================
 * Includes
 *=========================================================================*/

#include "esa_input_t.h"
#include "esa_instance_t.h"
#include "esa_output_t.h"

/*===========================================================================
 * External Function Prototypes
 *=========================================================================*/

/**
 * @brief Initializes the Esa output.
 *
 * @return void
 *
 * @SRD{CSCSA-68594,CSCSA-83856}
 * @SAD{CSCSA-83856}
 * @SDD{CSCSA-66565}
 * @verification{}
 */
void Esa_Post_Run_Init(Esa_Instance_T *p_esa_instance /**< ESA instance */);

/**
 * @brief Runs the customer specific ESA pre-run.
 *
 * @return void
 *
 * @SRD{CSCSA-68594}
 * @SAD{CSCSA-83856}
 * @SDD{CSCSA-66566}
 * @verification{}
 */
void Esa_Post_Run(const Esa_Instance_T *p_esa_instance /**< ESA instance */,
                  const Esa_Input_T *p_esa_input /**< ESA input data */,
                  Esa_Output_T *p_esa_output /**< ESA output data */);

#endif /* ESA_POST_RUN_H */
