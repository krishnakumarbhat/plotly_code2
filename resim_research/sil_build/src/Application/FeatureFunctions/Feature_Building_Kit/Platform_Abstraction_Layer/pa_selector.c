/**
 * @file pa_selector.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the functions to get the platform abstraction data.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*============================================================================*\
* Includes
\*============================================================================*/

#include "pa_selector.h"

#if !defined PA_Generic
/* coverity[misra_c_2012_rule_8_7_violation] */
void Pa_Get_Perception_Data(Pa_Context_T *p_context)
{
#ifdef PA_GDSR
   /* Calls function for return of GDSR tracker data */
   Pa_Get_Gdsr_Perception_Data(p_context);
#elif defined PA_F360
   /* Calls function for return of F360 tracker data */
   Pa_Get_F360_Perception_Data(p_context);
#elif defined PA_U360
   /* Calls function for return of U360 tracker data */
   Pa_Get_U360_Perception_Data(p_context);
#elif defined PA_Generic
   Pa_Get_Generic_Perception_Data(p_context);
#else
/* Throw error */
#error Please define which platform abstraction shall be used. Define PA_GDSR, PA_F360 or PA_U360 for usage of tracker output interface.
#endif
}

#endif /* !DEFINED PA_GENERIC */
