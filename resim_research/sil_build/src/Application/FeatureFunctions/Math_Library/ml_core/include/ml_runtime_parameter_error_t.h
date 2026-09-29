#ifndef ML_RUNTIME_PARAMETER_ERROR_T_H
#define ML_RUNTIME_PARAMETER_ERROR_T_H
#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "reuse.h"

/* using bit fields is needed because the structure needs to be small */
/* PRQA S 635 EOF*/

/**
 * \brief Error codes for runtime parameters.
 * \ingroup Runtime_Parameters
 * \sdd{WI-13601}
 */
typedef struct Runtime_Parameter_Error_Tag
{
   uint8_t parameter_not_settable : 1; /**< The given runtime parameter structure tries to set a forbidden value */ /* PRQA S 0635 */ /* The structure has to have a size of 1 byte. At the moment this
                                        * is more important than being portable */
   uint8_t parameter_must_be_set  : 1; /**< The given runtime parameter structure does not set a mandatory value */ /* PRQA S 0635 */ /* The structure has to have a size of 1 byte. At the moment this
                                        * is more important than being portable */
   uint8_t out_of_range           : 1; /**< The given runtime parameter is outside given min or max value */ /* PRQA S 0635 */ /* The structure has to have a size of 1 byte. At the moment this is more
                                        * important than being portable */
} Runtime_Parameter_Error_T;


#ifdef __cplusplus
}
#endif
#endif

