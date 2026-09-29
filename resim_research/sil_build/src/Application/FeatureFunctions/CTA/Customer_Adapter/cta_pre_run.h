#ifndef CTA_PRE_RUN_H
#define CTA_PRE_RUN_H

/**
 * @file cta_pre_run.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file for all customer specific CTA pre run files.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_input_t.h"
#include "cta_instance.h"
#include "fbk_output.h"
#include "pt_output_t.h"

/*===========================================================================*\
* External Function Prototypes
\*===========================================================================*/
/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */

#ifdef __cplusplus
extern "C"
{
#endif
   extern void Cta_Init_Input(Cta_Input_T *p_cta_input /**< CTA Input */);
   /**
    * @brief Initializes the pre run of the given customer interface.
    *
    * @return void
    *
    * @SRS{SF-169,SF-167}
    * @SAE{SF-2460}
    * @SDD{SF-3949}
    * @verification{Verify that the respective pre run is set up correctly.}
    */
   void Cta_Pre_Run_Init(Cta_Instance_T *p_cta_instance);


   /**
    * @brief Executes the pre run of the given customer interface.
    *
    * @return void
    *
    * @SRS{SF-164}
    * @SAE{SF-2460}
    * @SDD{SF-3945}
    * @verification{Check that mapped input is set up correctly.}
    */
   void Cta_Pre_Run(Cta_Instance_T *p_cta_instance /**< CTA instance */,
                    const Cta_Input_T *p_cta_input /**< CTA Input */,
                    const Fbk_Output_T *p_fbk_output /**< Fbk output */,
                    const Pt_Output_T *p_pt_output /**< PT output */);

#ifdef __cplusplus
}
#endif
#endif /* CTA_PRE_RUN_H */
