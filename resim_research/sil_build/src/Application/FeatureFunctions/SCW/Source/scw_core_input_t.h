#ifndef SCW_CORE_INPUT_T_H
#define SCW_CORE_INPUT_T_H

/**
 * @file scw_core_input_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the core input data structure for SCW.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "scw_types.h"

/*===========================================================================*\
 * typedefs
\*===========================================================================*/

/**
 * @brief Scw_Core_Input_T structure
 */
typedef struct
{
   boolean_T f_scw_enable;           /**< enable/disable SCW feature */
   boolean_T f_scw_enable_dynamic;   /**< enable/disable SCW dynamic object subfeature */
   boolean_T f_scw_enable_guardrail; /**< enable/disable SCW guardrail subfeature */

   Scw_Guardrail_Sources_T guardrail_data[FBK_NUMBER_OF_SIDES]; /**< contains guardrail information */

   Scw_Trailer_Object_T trailer; /**< Information about the trailer that might be attached to the host vehicle */
   const Pa_Data_T *p_pa_data;   /**< abstraction layer consumed by SCW */

} Scw_Core_Input_T;

#endif /* SCW_CORE_INPUT_T_H */
