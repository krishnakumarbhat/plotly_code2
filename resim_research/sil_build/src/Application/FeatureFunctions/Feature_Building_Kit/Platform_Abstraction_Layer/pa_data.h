#ifndef PA_DATA_H
#define PA_DATA_H

/**
 * @file pa_context.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the context definition for the generic interface.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*============================================================================*\
 * Includes
\*============================================================================*/

#include "fbk_guardrail_data_t.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "pa_const_macros.h"
#include "pa_reuse.h"

/*============================================================================*\
* Typedefs
\*============================================================================*/

/**
 * @brief Defines the overall PA data
 *
 * @SDD{}
 */
typedef struct
{
   Fbk_Object_Data_T object_data[PA_OBJ_NUMBER_OF_OBJECTS];          /**< object data */
   Fbk_Vehicle_Data_T vehicle_data;                                  /**< vehicle data */
   Fbk_Guardrail_Data_T guardrail_data[PA_OBJ_NUMBER_OF_GUARDRAILS]; /**< guardrail data */

   float32_T time_diff_to_last_cycle; /**< cycle time */
} Pa_Data_T;


#endif /* PA_DATA_H */
