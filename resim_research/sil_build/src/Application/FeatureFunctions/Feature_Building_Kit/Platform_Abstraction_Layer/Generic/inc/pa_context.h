#ifndef PA_CONTEXT_H
#define PA_CONTEXT_H

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
#include "pa_data.h"


/**
 * @brief Defines the PA context
 *
 * @SDD{}
 */
typedef struct
{
   Pa_Data_T *p_data;
} Pa_Context_T;


/*============================================================================*\
 * Global Function Declaration
\*============================================================================*/

#endif /* PA_CONTEXT_H */
