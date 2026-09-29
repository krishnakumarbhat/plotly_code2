#ifndef LCDA_PRE_RUN_H
#define LCDA_PRE_RUN_H

/**
 * @file lcda_pre_run.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for all customer specific LCDA pre run files.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#include "fbk_output.h"
#include "lcda_input_t.h"
#include "lcda_instance.h"

/*===========================================================================*\
* Global Function Prototypess
\*===========================================================================*/
#ifdef __cplusplus
extern "C"
{
#endif
   extern void Lcda_Init_Input(Lcda_Input_T *p_lcda_input);
#ifdef __cplusplus
}
#endif

/**
 * @brief This function initializes the customer input
 *
 * @return void
 *
 * @SRS{SF-1049}
 * @SAE{SF-2780}
 * @SDD{SF-6840}
 * @verification{}
 */
void Lcda_Pre_Run_Init(Lcda_Instance_T *p_lcda_instance /**< Lcda input */);

/**
 * @brief Customer specific LCDA pre-run function.
 *
 * @return void
 *
 * @SRS{SF-1049}
 * @SAE{SF-2780}
 * @SDD{SF-6836}
 * @verification{}
 */
void Lcda_Pre_Run(Lcda_Instance_T *p_lcda_instance /**< Lcda core input */,
                  const Lcda_Input_T *p_lcda_input /**< Lcda input */,
                  const Fbk_Output_T *p_fbk_output);

#endif /* LCDA_PRE_RUN_H */
