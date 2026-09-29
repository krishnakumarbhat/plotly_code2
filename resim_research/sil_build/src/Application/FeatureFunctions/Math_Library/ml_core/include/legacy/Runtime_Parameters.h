#ifndef RUNTIME_PARAMETERS_H
#define RUNTIME_PARAMETERS_H
#ifdef __cplusplus
extern "C"
{
#endif
/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include "st_runtime_parameters.h"

/* Since this file defines a bunch of function-like macros for backwards compatibility
* the QAC check "A function could probably be used instead of this function-like macro."
* Is not needed. Suppress it for the full file.*/
/* PRQA S 3453 EOF */

/**
* This macro is only provided for backwards compatibility. Call Generate_Runtime_Value_Boolean directly.
* \ingroup Runtime_Parameters
*/
#define generate_runtime_value_boolean_T(a,b,c,d) (Generate_Runtime_Value_Boolean(a,b,c,d))

/**
* This macro is only provided for backwards compatibility. Call Generate_Runtime_Value_Uint8 directly.
* \ingroup Runtime_Parameters
*/
#define generate_runtime_value_uint8_T(a,b,c,d) (Generate_Runtime_Value_Uint8(a,b,c,d))

/**
* This macro is only provided for backwards compatibility. Call Generate_Runtime_Value_Uint16 directly.
* \ingroup Runtime_Parameters
*/
#define generate_runtime_value_uint16_T(a,b,c,d) (Generate_Runtime_Value_Uint16(a,b,c,d))

/**
* This macro is only provided for backwards compatibility. Call Generate_Runtime_Value_Uint32 directly.
* \ingroup Runtime_Parameters
*/
#define generate_runtime_value_uint32_T(a,b,c,d) (Generate_Runtime_Value_Uint32(a,b,c,d))

/**
* This macro is only provided for backwards compatibility. Call Generate_Runtime_Value_Int8 directly.
* \ingroup Runtime_Parameters
*/
#define generate_runtime_value_int8_T(a,b,c,d) (Generate_Runtime_Value_Int8(a,b,c,d))

/**
* This macro is only provided for backwards compatibility. Call Generate_Runtime_Value_Int16 directly.
* \ingroup Runtime_Parameters
*/
#define generate_runtime_value_int16_T(a,b,c,d) (Generate_Runtime_Value_Int16(a,b,c,d))

/**
* This macro is only provided for backwards compatibility. Call Generate_Runtime_Value_Int32 directly.
* \ingroup Runtime_Parameters
*/
#define generate_runtime_value_int32_T(a,b,c,d) (Generate_Runtime_Value_Int32(a,b,c,d))

/**
* This macro is only provided for backwards compatibility. Call Generate_Runtime_Value_Float32 directly.
* \ingroup Runtime_Parameters
*/
#define generate_runtime_value_float32_T(a,b,c,d) (Generate_Runtime_Value_Float32(a,b,c,d))

/**
* This macro is only provided for backwards compatibility. Call Init_Runtime_Param_Error directly.
* \ingroup Runtime_Parameters
*/
#define init_runtime_param_error(a) (Init_Runtime_Param_Error(a))

/**
* This macro is only provided for backwards compatibility. Call Map_Cal_Settings_To_Runtime_Parameter_Legal_State directly.
* \ingroup Runtime_Parameters
*/
#define map_cal_settings_to_runtime_parameter_legal_state(a,b) (Map_Cal_Settings_To_Runtime_Parameter_Legal_State(a,b))

/**
* This macro is only provided for backwards compatibility. Call Test_Runtime_Value_Uint8 directly.
* \ingroup Runtime_Parameters
*/
#define test_runtime_value_uint8_T(a,b,c,d,e) (Test_Runtime_Value_Uint8(a,b,c,d,e))

/**
* This macro is only provided for backwards compatibility. Call Test_Runtime_Value_Uint16 directly.
* \ingroup Runtime_Parameters
*/
#define test_runtime_value_uint16_T(a,b,c,d,e) (Test_Runtime_Value_Uint16(a,b,c,d,e))

/**
* This macro is only provided for backwards compatibility. Call Test_Runtime_Value_Uint32 directly.
* \ingroup Runtime_Parameters
*/
#define test_runtime_value_uint32_T(a,b,c,d,e) (Test_Runtime_Value_Uint32(a,b,c,d,e))

/**
* This macro is only provided for backwards compatibility. Call Test_Runtime_Value_Int8 directly.
* \ingroup Runtime_Parameters
*/
#define test_runtime_value_int8_T(a,b,c,d,e) (Test_Runtime_Value_Int8(a,b,c,d,e))

/**
* This macro is only provided for backwards compatibility. Call Test_Runtime_Value_Int16 directly.
* \ingroup Runtime_Parameters
*/
#define test_runtime_value_int16_T(a,b,c,d,e) (Test_Runtime_Value_Int16(a,b,c,d,e))

/**
* This macro is only provided for backwards compatibility. Call Test_Runtime_Value_Int32 directly.
* \ingroup Runtime_Parameters
*/
#define test_runtime_value_int32_T(a,b,c,d,e) (Test_Runtime_Value_Int32(a,b,c,d,e))

/**
* This macro is only provided for backwards compatibility. Call Test_Runtime_Value_Float32 directly.
* \ingroup Runtime_Parameters
*/
#define test_runtime_value_float32_T(a,b,c,d,e) (Test_Runtime_Value_Float32(a,b,c,d,e))

/**
* This macro is only provided for backwards compatibility. Call Set_Runtime_Parameter_Extern_Boolean directly.
* \ingroup Runtime_Parameters
*/
#define set_runtime_parameter_extern_boolean_T(a) (Set_Runtime_Parameter_Extern_Boolean(a))

/**
* This macro is only provided for backwards compatibility. Call Set_Runtime_Parameter_Extern_Uint8 directly.
* \ingroup Runtime_Parameters
*/
#define set_runtime_parameter_extern_uint8_T(a) (Set_Runtime_Parameter_Extern_Uint8(a))

/**
* This macro is only provided for backwards compatibility. Call Set_Runtime_Parameter_Extern_Uint16 directly.
* \ingroup Runtime_Parameters
*/
#define set_runtime_parameter_extern_uint16_T(a) (Set_Runtime_Parameter_Extern_Uint16(a))

/**
* This macro is only provided for backwards compatibility. Call Set_Runtime_Parameter_Extern_Uint32 directly.
* \ingroup Runtime_Parameters
*/
#define set_runtime_parameter_extern_uint32_T(a) (Set_Runtime_Parameter_Extern_Uint32(a))

/**
* This macro is only provided for backwards compatibility. Call Set_Runtime_Parameter_Extern_Int8 directly.
* \ingroup Runtime_Parameters
*/
#define set_runtime_parameter_extern_int8_T(a) (Set_Runtime_Parameter_Extern_Int8(a))

/**
* This macro is only provided for backwards compatibility. Call Set_Runtime_Parameter_Extern_Int16 directly.
* \ingroup Runtime_Parameters
*/
#define set_runtime_parameter_extern_int16_T(a) (Set_Runtime_Parameter_Extern_Int16(a))

/**
* This macro is only provided for backwards compatibility. Call Set_Runtime_Parameter_Extern_Int32 directly.
* \ingroup Runtime_Parameters
*/
#define set_runtime_parameter_extern_int32_T(a) (Set_Runtime_Parameter_Extern_Int32(a))

/**
* This macro is only provided for backwards compatibility. Call Set_Runtime_Parameter_Extern_Float32 directly.
* \ingroup Runtime_Parameters
*/
#define set_runtime_parameter_extern_float32_T(a) (Set_Runtime_Parameter_Extern_Float32(a))

/**
* Used for backwards compatibility. Use Runtime_Parameter_Boolean_Tag instead.
* \ingroup Runtime_Parameters
*/
typedef struct Runtime_Parameter_Boolean_Tag RUNTIME_PARAMETER_BOOLEAN_T;

/**
* Used for backwards compatibility. Use Runtime_Parameter_Legal_State_T instead.
* \ingroup Runtime_Parameters
*/
typedef struct Runtime_Parameter_Error_Tag RUNTIME_PARAMETER_ERROR_T;

/**
* Used for backwards compatibility. Use Runtime_Parameter_Float32_T instead.
* \ingroup Runtime_Parameters
*/
typedef struct Runtime_Parameter_Float32_Tag RUNTIME_PARAMETER_FLOAT32_T;

/**
* Used for backwards compatibility. Use Runtime_Parameter_Int8_T instead.
* \ingroup Runtime_Parameters
*/
typedef struct Runtime_Parameter_Int8_Tag RUNTIME_PARAMETER_INT8_T;

/**
* Used for backwards compatibility. Use Runtime_Parameter_Int16_T instead.
* \ingroup Runtime_Parameters
*/
typedef struct Runtime_Parameter_Int16_Tag RUNTIME_PARAMETER_INT16_T;

/**
* Used for backwards compatibility. Use Runtime_Parameter_Int32_T instead.
* \ingroup Runtime_Parameters
*/
typedef struct Runtime_Parameter_Int32_Tag RUNTIME_PARAMETER_INT32_T;

/**
* Used for backwards compatibility. Use Runtime_Parameter_Legal_State_T instead.
* \ingroup Runtime_Parameters
*/
typedef enum Runtime_Parameter_Legal_State_Tag RUNTIME_PARAMETER_LEGAL_STATE_T;

/**
* Used for backwards compatibility. Use Runtime_Parameter_Uint8_T instead.
* \ingroup Runtime_Parameters
*/
typedef struct Runtime_Parameter_Uint8_Tag RUNTIME_PARAMETER_UINT8_T;

/**
* Used for backwards compatibility. Use Runtime_Parameter_Uint16_T instead.
* \ingroup Runtime_Parameters
*/
typedef struct Runtime_Parameter_Uint16_Tag RUNTIME_PARAMETER_UINT16_T;

/**
* Used for backwards compatibility. Use Runtime_Parameter_Uint32_T instead.
* \ingroup Runtime_Parameters
*/
typedef struct Runtime_Parameter_Uint32_Tag RUNTIME_PARAMETER_UINT32_T;



#ifdef __cplusplus
}
#endif
#endif
