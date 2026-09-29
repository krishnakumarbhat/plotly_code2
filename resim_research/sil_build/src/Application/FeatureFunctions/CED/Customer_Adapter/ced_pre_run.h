#ifndef CED_PRE_RUN_H
#define CED_PRE_RUN_H

/**
 * @file ced_pre_run.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for all customer specific CED pre run files.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_input_t.h"
#include "ced_instance.h"
#include "fbk_output.h"
#include "pt_output_t.h"

/*===========================================================================*\
* External Function Prototypes
\*===========================================================================*/


#ifdef __cplusplus
extern "C"
{
#endif
   /**
    * @brief Sets Generic Ced_Input_T To Default Values
    *
    * @return void
    *
    * @SRS{SF-50}
    * @SAD{SF-2403}
    * @SDD{CSCSA-122291}
    * @verification{ Set non default value to generic input, run Ced_Init_Input and check if its values are set to default }
    */
   void Ced_Init_Input(Ced_Input_T *p_ced_input /**< CED input data */);
#ifdef __cplusplus
}
#endif

/**
 * @brief Runs the customer specific CED pre-run initialization.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2403}
 * @SDD{SF-3398}
 * @verification{}
 */
void Ced_Pre_Run_Init(Ced_Instance_T *p_ced_instance /**< CED instance data */);

/**
 * @brief Runs the customer specific CED pre-run.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2403}
 * @SDD{SF-3402}
 * @verification{}
 */
void Ced_Pre_Run(Ced_Instance_T *p_ced_instance /**< CED instance */,
                 const Ced_Input_T *p_ced_input /**< CED input data */,
                 const Pt_Output_T *p_pt_output /**< PathTracking output structure*/,
                 const Fbk_Output_T *p_fbk_output /**< FBK output */);

#endif /* CED_PRE_RUN_H */
