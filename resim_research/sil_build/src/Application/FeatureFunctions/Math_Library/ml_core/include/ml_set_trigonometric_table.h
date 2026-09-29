#ifndef ML_SET_TRIGONOMETRIC_TABLE_H
#define ML_SET_TRIGONOMETRIC_TABLE_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include "ml_serialization_error_t.h"
#include "ml_serial_buffer_t.h"

/**
* Chooses a predefined trigonometric table based on the given serialized checksum.
* \return error code
* If the ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE mode does not equal ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
* SHARED_TOOLBOX_SRL_ERR_UNKNOWN is returned.
* \ingroup trigonometric_functions_serialization
*/
Shared_Toolbox_Serialization_Error_T Set_Trig_Table_By_Checksum(const Shared_Toolbox_Serial_Buffer_T *p_buffer /**< Serialization buffer */);

#ifdef __cplusplus
}
#endif
#endif
