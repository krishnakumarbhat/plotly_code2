/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <assert.h>
#include "ml_bool.h"
#include "ml_macros.h"
#include "ml_runtime_parameters.h"


/**
 * Sets the error flags in the given error structure based on the given set flag and the given legal state
 *
 * \return           generated runtime parameter
 * \sdd{WI-13601}
 */
static void Runtime_Parameter_Error_Setting(
   Runtime_Parameter_Legal_State_T legal_state,
   boolean_T                       is_set,
   Runtime_Parameter_Error_T      *p_error);

/**
 * Returns TRUE if the given runtime parameter can be used based on the legal state. Returns FALSE if the standard value needs to be used.
 *
 * \return         TRUE if the runtime parameter can be used
 */
static boolean_T Is_Runtime_Parameter_Settable(Runtime_Parameter_Legal_State_T legal_state);

/* Macros are used to generate nearly identical functions */
/* PRQA S 881 EOF */
/* Therefore the macros define code fragments*/
/* PRQA S 3412 EOF */

/* The return type of these functions is a macro parameter and can therefore not be enclosed in () */
/* PRQA S 3410 EOF */
#define GENERATE_RUNTIME_VALUE(type, type_name, name)                                                      \
   type Generate_Runtime_Value_ ## name(const Runtime_Parameter_ ## name ## _T * p_runtime_parameter,      \
                                             Runtime_Parameter_Legal_State_T legal_state,                  \
                                             type default_value,                                           \
                                             Runtime_Parameter_Error_T * p_error)                          \
   {                                                                                                       \
      type value_out = default_value;                                                                      \
      Runtime_Parameter_Error_Setting(legal_state, p_runtime_parameter->is_set, p_error);                  \
      if (Is_True(Is_Runtime_Parameter_Settable(legal_state)) && Is_True(p_runtime_parameter->is_set))     \
      {                                                                                                    \
         value_out = p_runtime_parameter->value;                                                           \
      }                                                                                                    \
      return value_out;                                                                                    \
   }

GENERATE_RUNTIME_VALUE(boolean_T, boolean_T, Boolean)
GENERATE_RUNTIME_VALUE(uint8_t, uint8_T, Uint8)
GENERATE_RUNTIME_VALUE(uint16_t, uint16_T, Uint16)
GENERATE_RUNTIME_VALUE(uint32_t, uint32_T, Uint32)
GENERATE_RUNTIME_VALUE(int8_t, int8_T, Int8)
GENERATE_RUNTIME_VALUE(int16_t, int16_T, Int16)
GENERATE_RUNTIME_VALUE(int32_t, int32_T, Int32)
GENERATE_RUNTIME_VALUE(float32_T, float32_T, Float32)

#define TEST_RUNTIME_VALUE(type, type_name)                                                                            \
   type Test_Runtime_Value_ ## type_name(type runtime_parameter,                                                       \
                                         type max_value,                                                               \
                                         type min_value,                                                               \
                                         type default_value,                                                           \
                                         Runtime_Parameter_Error_T * p_error /**< The runtime parameter error flags*/) \
   {                                                                                                                   \
      type ret_value;                                                                                                  \
      assert((max_value >= default_value) && (min_value <= default_value));                                            \
      assert(max_value >= min_value);                                                                                  \
      if ((max_value >= runtime_parameter) && (min_value <= runtime_parameter))                                        \
      {                                                                                                                \
         ret_value = runtime_parameter;                                                                                \
      }                                                                                                                \
      else                                                                                                             \
      {                                                                                                                \
         ret_value             = default_value;                                                                        \
         p_error->out_of_range = TRUE;                                                                                 \
      }                                                                                                                \
      return ret_value;                                                                                                \
   }

/* The statement indeed has an effect */
/* PRQA S 3112 ++ */
TEST_RUNTIME_VALUE(uint8_t, Uint8)
TEST_RUNTIME_VALUE(int8_t, Int8)
TEST_RUNTIME_VALUE(uint16_t, Uint16)
TEST_RUNTIME_VALUE(int16_t, Int16)
TEST_RUNTIME_VALUE(uint32_t, Uint32)
TEST_RUNTIME_VALUE(int32_t, Int32)
TEST_RUNTIME_VALUE(float32_T, Float32)
/* PRQA S 3112 -- */


#define SET_RUNTIME_VALUE_EXTERN(type_out, type_in, type_in_name)        \
   type_out Set_Runtime_Parameter_Extern_ ## type_in_name(type_in input) \
   {                                                                     \
      type_out runtime_param;                                            \
      runtime_param.is_set = TRUE;                                       \
      runtime_param.value  = input;                                      \
      return runtime_param;                                              \
   }


SET_RUNTIME_VALUE_EXTERN(Runtime_Parameter_Boolean_T, boolean_T, Boolean)
SET_RUNTIME_VALUE_EXTERN(Runtime_Parameter_Uint8_T, uint8_t, Uint8)
SET_RUNTIME_VALUE_EXTERN(Runtime_Parameter_Uint16_T, uint16_t, Uint16)
SET_RUNTIME_VALUE_EXTERN(Runtime_Parameter_Uint32_T, uint32_t, Uint32)
SET_RUNTIME_VALUE_EXTERN(Runtime_Parameter_Int8_T, int8_t, Int8)
SET_RUNTIME_VALUE_EXTERN(Runtime_Parameter_Int16_T, int16_t, Int16)
SET_RUNTIME_VALUE_EXTERN(Runtime_Parameter_Int32_T, int32_t, Int32)
SET_RUNTIME_VALUE_EXTERN(Runtime_Parameter_Float32_T, float32_T, Float32)

void Init_Runtime_Param_Error(Runtime_Parameter_Error_T *p_error)
{
   p_error->parameter_must_be_set  = FALSE;
   p_error->parameter_not_settable = FALSE;
   p_error->out_of_range           = FALSE;
}


Runtime_Parameter_Legal_State_T Map_Cal_Settings_To_Runtime_Parameter_Legal_State(
   boolean_T must_be_set,
   boolean_T prohibited_to_set)
{
   Runtime_Parameter_Legal_State_T legal_state;

   if (Is_True(prohibited_to_set))
   {
      legal_state = RUNTIME_PARAMETER_PROHIBITED;
   }
   else if (Is_True(must_be_set))
   {
      legal_state = RUNTIME_PARAMETER_MANDATORY;
   }
   else
   {
      legal_state = RUNTIME_PARAMETER_OPTIONAL;
   }


   return legal_state;
}


static void Runtime_Parameter_Error_Setting(
   Runtime_Parameter_Legal_State_T legal_state,
   boolean_T                       is_set,
   Runtime_Parameter_Error_T      *p_error)
{
   assert(NULL != p_error);

   if ((legal_state == RUNTIME_PARAMETER_PROHIBITED) && (Is_True(is_set)))
   {
      p_error->parameter_not_settable = TRUE;
   }
   if ((legal_state == RUNTIME_PARAMETER_MANDATORY) && (Is_False(is_set)))
   {
      p_error->parameter_must_be_set = TRUE;
   }
}


static boolean_T Is_Runtime_Parameter_Settable(Runtime_Parameter_Legal_State_T legal_state)
{
   return (legal_state == RUNTIME_PARAMETER_OPTIONAL) || (legal_state == RUNTIME_PARAMETER_MANDATORY);
}

