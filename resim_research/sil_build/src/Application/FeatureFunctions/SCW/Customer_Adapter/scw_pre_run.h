#ifndef SCW_PRE_RUN_H
#define SCW_PRE_RUN_H

/**
 * @file scw_pre_run.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for all customer specific SCW pre run files.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_output.h"
#include "scw_input_t.h"
#include "scw_instance_t.h"

/*===========================================================================*\
* Global Function Definition
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif
   /**
    * @brief This function initializes SCW FF input
    *
    * @return void
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-196754}
    * @verification{}
    */
   extern void Scw_Init_Input(Scw_Input_T *p_scw_input /**< SCW input */);
#ifdef __cplusplus
}
#endif

/**
 * @brief This function initializes the customer input
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2991}
 * @SDD{SF-8151}
 * @verification{}
 */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The customer may want to modify some members of the parameter] */
void Scw_Pre_Run_Init(Scw_Instance_T *p_scw_instance /**< SCW instance */);

/**
 * @brief This function pre processes data before handing it to the SCW algorithm
 *
 * @return void
 *
 * @SRS{}
 * @SAE{SF-2991}
 * @SDD{SF-8150}
 * @verification{}
 */
void Scw_Pre_Run(Scw_Instance_T *p_scw_instance /**< SCW instance */,
                 const Scw_Input_T *p_scw_input /**< SCW input */,
                 const Fbk_Output_T *p_fbk_output /**< FBK output */);

#endif /* SCW_PRE_RUN_H */
