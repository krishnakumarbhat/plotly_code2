#ifndef ML_RUNTIME_PARAMETER_FLOAT32_T_H
#define ML_RUNTIME_PARAMETER_FLOAT32_T_H
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
 * \brief A float runtime parameter.
 * \ingroup Runtime_Parameters
 * \sdd{WI-13609}
 */
typedef struct Runtime_Parameter_Float32_Tag
{
   float32_T value;  /**< The value of the runtime parameter */
   boolean_T is_set; /**< Flag indicating that this runtime parameter has been set*/
} Runtime_Parameter_Float32_T;


#ifdef __cplusplus
}
#endif
#endif

