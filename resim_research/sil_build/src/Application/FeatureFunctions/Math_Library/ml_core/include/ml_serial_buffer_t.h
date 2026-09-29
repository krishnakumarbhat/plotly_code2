#ifndef ML_SERIAL_BUFFER_T_H
#define ML_SERIAL_BUFFER_T_H
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
* A buffer to store serialized fast math tables data in.
* \ingroup trigonometric_functions_serialization
*/
typedef struct Shared_Toolbox_Serial_Buffer_Tag
{
   char*	p_data; /**< Pointer to start of buffer. Rationale for using char
                 is to show that this is a pure data pointer without
                 semantics like uint8_t, do not use for integer
                 arithmetic */
   size_t	length; /**< Size of buffer starting at p_data */
} Shared_Toolbox_Serial_Buffer_T;

#ifdef __cplusplus
}
#endif
#endif
