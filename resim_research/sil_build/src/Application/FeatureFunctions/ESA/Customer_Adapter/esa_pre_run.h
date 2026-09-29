#ifndef ESA_PRE_RUN_H
#define ESA_PRE_RUN_H

/**
 * @file esa_pre_run.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for all customer specific ESA pre run files.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "esa_input_t.h"
#include "esa_instance_t.h"
#include "fbk_output.h"

/*===========================================================================*\
* External Function Prototypes
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif
   extern void Esa_Init_Input(Esa_Input_T *p_esa_input /**< ESA input */);
#ifdef __cplusplus
}
#endif

/**
 * @brief Runs the customer specific ESA pre-run initialization.
 *
 * @return void
 *
 * @SRD{CSCSA-68587,CSCSA-122858}
 * @SAD{CSCSA-83854}
 * @SDD{CSCSA-66557}
 * @verification{Verify that the respective pre run is set up correctly.}
 */
/* coverity[misra_c_2012_rule_2_7_violation][Unused function parameter is used by other customer and cannot be removed] */
/* coverity[misra_c_2012_rule_8_13_violation][The customer may want to modify some members of the parameter] */
void Esa_Pre_Run_Init(Esa_Instance_T *p_esa_instance /**< ESA instance */);

/**
 * @brief Runs the customer specific ESA pre-run.
 *
 * @return void
 *
 * @SRD{CSCSA-122858,CSCSA-68598,CSCSA-68599,CSCSA-122858}
 * @SAD{CSCSA-83854}
 * @SDD{CSCSA-66558}
 * @verification{Check that mapped input is set up correctly.}
 */
void Esa_Pre_Run(Esa_Instance_T *p_esa_instance /**< ESA instance */,
                 const Esa_Input_T *p_esa_input /**< ESA input data */,
                 const Fbk_Output_T *p_fbk_output /**< FBK output */);

#endif /* ESA_PRE_RUN_H */
