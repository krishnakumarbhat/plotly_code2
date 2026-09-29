#ifndef ML_RUNTIME_PARAMETER_LEGAL_STATE_T_H
#define ML_RUNTIME_PARAMETER_LEGAL_STATE_T_H
#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

/**
 * \brief How to treat a runtime parameter. It either must, may or must not be set.
 * \ingroup Runtime_Parameters
 */
typedef enum Runtime_Parameter_Legal_State_Tag
{
   RUNTIME_PARAMETER_PROHIBITED, /**< The runtime parameter must NOT be set */
   RUNTIME_PARAMETER_MANDATORY,  /**< The runtime parameter MUST be set */
   RUNTIME_PARAMETER_OPTIONAL    /**< The runtime parameter MAY be set */
} Runtime_Parameter_Legal_State_T;



#ifdef __cplusplus
}
#endif
#endif

