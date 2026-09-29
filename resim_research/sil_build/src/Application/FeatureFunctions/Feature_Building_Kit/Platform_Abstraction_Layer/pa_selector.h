#ifndef PA_SELECTOR_H
#define PA_SELECTOR_H

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

/*============================================================================*\
* Global Functions Declaration
\*============================================================================*/

/**
 * Wrapper for return of tracker data. This additional level of abstraction is build in
 * in order to have an interface independent of the tracker version.
 *
 * \Requirements
 * \reqtrace{}{}
 */
#ifdef __cplusplus
extern "C"
{
#endif
#if !defined PA_Generic
   void Pa_Get_Perception_Data(Pa_Context_T *p_context);
#endif /* !DEFINED PA_GENERIC */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
