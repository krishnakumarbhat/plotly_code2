#ifndef LTB_PRE_RUN_H
#define LTB_PRE_RUN_H

/**
 * @file ltb_pre_run.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for all customer specific LTB pre run files.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_output.h"
#include "ltb_input_t.h"
#include "ltb_instance.h"

/*===========================================================================*\
* External Function Prototypes
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
   /**
    * @brief Initializes LTB input.
    *
    * @return void
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-216621}
    * @verification{}
    */
   extern void Ltb_Init_Input(Ltb_Input_T *p_ltb_input /**< LTB Input */);
#ifdef __cplusplus
}
#endif /* __cplusplus */
/**
 * @brief Runs the customer specific LTB pre-run.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-54002}
 * @verification{}
 */
void Ltb_Pre_Run(Ltb_Instance_T *p_ltb_instance /** LTB instance */,
                 const Ltb_Input_T *p_ltb_input /**< LTB input data */,
                 const Fbk_Output_T *p_fbk_output /**< LTB calibration */);

#endif /* LTB_PRE_RUN_H */
