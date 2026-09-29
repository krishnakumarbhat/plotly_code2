#ifndef ML_RUNTIME_PARAMETER_INT16_T_H
#define ML_RUNTIME_PARAMETER_INT16_T_H
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
 * \brief An int16_t runtime parameter.
 * \ingroup Runtime_Parameters
 * \sdd{WI-13611}
 */
typedef struct Runtime_Parameter_Int16_Tag
{
   int16_t   value;  /**< The value of the runtime parameter */
   boolean_T is_set; /**< Flag indicating that this runtime parameter has been set*/
} Runtime_Parameter_Int16_T;




#ifdef __cplusplus
}
#endif
#endif

