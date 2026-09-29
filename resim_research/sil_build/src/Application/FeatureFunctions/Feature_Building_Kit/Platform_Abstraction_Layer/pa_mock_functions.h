#ifndef PA_MOCK_FUNCTIONS_H
#define PA_MOCK_FUNCTIONS_H

/**
 * @file pa_mock_functions.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file provides functions that can be used to define any PA function-like macro as a constant value, without
 * producing a compiler warning or coverity finding about unused parameters of the macro.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*============================================================================*\
 * Includes
\*============================================================================*/

#include "pa_reuse.h"
#include "pa_shared_types.h"

/*============================================================================*\
* Mock functions
\*============================================================================*/

#ifdef __GNUC__
/* clang-format off */
/* coverity[misra_c_2012_rule_1_2_violation][Intentional use to avoid gcc compiler warning about unused function.] */
static inline  float32_T Pa_Mock_Float(const Pa_Context_T *p_context, const uint8_t index, const float32_T mock_value) __attribute__((unused));
/* coverity[misra_c_2012_rule_1_2_violation][Intentional use to avoid gcc compiler warning about unused function.] */
static inline  boolean_T Pa_Mock_Boolean(const Pa_Context_T *p_context, const uint8_t index, const boolean_T mock_value) __attribute__((unused));
/* coverity[misra_c_2012_rule_1_2_violation][Intentional use to avoid gcc compiler warning about unused function.] */
static inline  uint8_t Pa_Mock_Uint(const Pa_Context_T *p_context, const uint8_t index, const uint8_t mock_value) __attribute__((unused));
/* coverity[misra_c_2012_rule_1_2_violation][Intentional use to avoid gcc compiler warning about unused function.] */
static inline  Pa_Obj_Curvi_Calc_Method_T Pa_Mock_Curvi_Calc_Method(const Pa_Context_T *p_context, const uint8_t index, const Pa_Obj_Curvi_Calc_Method_T mock_value) __attribute__((unused));
/* coverity[misra_c_2012_rule_1_2_violation][Intentional use to avoid gcc compiler warning about unused function.] */
static inline  Pa_Obj_Status_T Pa_Mock_Obj_Status(const Pa_Context_T *p_context, const uint8_t index, const Pa_Obj_Status_T mock_value) __attribute__((unused));
/* clang-format on */
#endif

static inline float32_T Pa_Mock_Float(const Pa_Context_T *p_context, const uint8_t index, const float32_T mock_value)
{
   (void) p_context;
   (void) index;
   return mock_value;
}

static inline boolean_T Pa_Mock_Boolean(const Pa_Context_T *p_context, const uint8_t index, const boolean_T mock_value)
{
   (void) p_context;
   (void) index;
   return mock_value;
}

static inline uint8_t Pa_Mock_Uint(const Pa_Context_T *p_context, const uint8_t index, const uint8_t mock_value)
{
   (void) p_context;
   (void) index;
   return mock_value;
}

static inline Pa_Obj_Status_T Pa_Mock_Obj_Status(const Pa_Context_T *p_context, const uint8_t index, const Pa_Obj_Status_T mock_value)
{
   (void) p_context;
   (void) index;
   return mock_value;
}

static inline Pa_Obj_Curvi_Calc_Method_T Pa_Mock_Curvi_Calc_Method(const Pa_Context_T *p_context,
                                                                   const uint8_t index,
                                                                   const Pa_Obj_Curvi_Calc_Method_T mock_value)
{
   (void) p_context;
   (void) index;
   return mock_value;
}

#endif /* PA_MOCK_FUNCTIONS_H */
