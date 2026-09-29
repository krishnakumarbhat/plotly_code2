#ifndef ML_RUNTIME_PARAMETER_BOOLEAN_T_H
#define ML_RUNTIME_PARAMETER_BOOLEAN_T_H
#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "reuse.h"

/**
 * \brief A boolean runtime parameter.
 * \ingroup Runtime_Parameters
 * \sdd{WI-13608}
 */
typedef struct Runtime_Parameter_Boolean_Tag
{
   boolean_T value;  /**< The value of the runtime parameter */
   boolean_T is_set; /**< Flag indicating that this runtime parameter has been set*/
} Runtime_Parameter_Boolean_T;


#ifdef __cplusplus
}
#endif
#endif

