#ifndef FBK_GUARDRAIL_VALIDATION_H
#define FBK_GUARDRAIL_VALIDATION_H

/**
 * @file fbk_guardrail_validation.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file with functions for guardrail data validation.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_guardrail_data_t.h"
#include "pa_context.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Global Function Definition
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif
   /**
    * @brief Fills guardrail data for given side from provided data.
    *
    * @return void
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-4190}
    * @verification{}
    */
   void Fbk_Fill_Guardrail_Information(Fbk_Guardrail_Data_T *p_fbk_guardrail_data /**< internal fbk guardrail data structure */,
                                       const Pa_Context_T *p_context /**< context data */,
                                       const uint8_t side /**< host vehicle side */);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FBK_GUARDRAIL_VALIDATION_H*/
