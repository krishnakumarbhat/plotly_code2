#ifndef FBK_FILL_PA_DATA_H
#define FBK_FILL_PA_DATA_H

/**
 * @file pa_selector.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Declares the functions to get the platform abstraction data.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*============================================================================*\
* Includes
\*============================================================================*/

#include "pa_context.h"
#include "pa_data.h"
#include "sfl_status.h"

/*============================================================================*\
* Global Functions Declaration
\*============================================================================*/

/**
 * @brief Process object data, vehicle data, and guardrails to prepare the output structure.
 * Performs validation on object and vehicle data to ensure values are within acceptable ranges.
 *
 * \Requirements
 * \reqtrace{}{}
 */
Sfl_Status_T Fbk_Fill_Pa_Data(Pa_Data_T *output, Pa_Context_T *p_context);


#endif // FBK_FILL_PA_DATA_H
