#ifndef RECW_PRE_RUN_H
#define RECW_PRE_RUN_H

/**
 * @file recw_pre_run.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the RECW pre run header file shared by all customers.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_output.h"
#include "recw_input_t.h"

#include "recw_instance.h"

#ifdef __cplusplus
extern "C"
{
#endif
   /**
    * @brief Sets RECW FF Status to true, asserts that input is not NULL.
    *
    * @return void
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-196403}
    * @verification{}
    */
   extern void Recw_Init_Input(Recw_Input_T *p_recw_input /**< RECW Input */);
#ifdef __cplusplus
}
#endif

/*===========================================================================*\
* Global Function Definition
\*===========================================================================*/

/**
 * @brief Initializes the pre run of the given customer interface.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-7970}
 * @verification{Verify that the respective pre run is set up correctly.}
 */
void Recw_Pre_Run_Init(Recw_Instance_T *p_recw_instance);

/**
 * @brief Calls routine for customer dependend Recw prerun
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2960}
 * @SDD{SF-7971}
 * @verification{}
 */
void Recw_Pre_Run(Recw_Instance_T *p_recw_instance /**< Recw instance */,
                  const Recw_Input_T *p_recw_input /**< Recw input */,
                  const Fbk_Output_T *p_fbk_output /**< FBK core input */);

#endif /* RECW_PRE_RUN_H */
