#ifndef ML_RUNTIME_PARAMETERS_H
#define ML_RUNTIME_PARAMETERS_H
#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

/**
 * \defgroup Runtime_Parameters Runtime Parameters
 * \brief Runtime parameters are parameters that can be set while the module is running.
 *
 * Contrary to calibrations runtime parameters may be set by a customer. The mechanism to do this is named "data set download" or CAF depending
 * on the customer. The functions in this module provide functions to use such parameters.
 * The module initialization function receives a feature_runtime_parameter structure. This structure holds a RUNTIME_PARAMETER_*_T
 * for each runtime parameter the feature supports.
 * The calibration of the feature has two values for each runtime parameter:
 *    * \ref Runtime_Parameter_Legal_State_T
 *    * A standard value
 * Based on the cal values and the RUNTIME_PARAMETER_*_T content the generate_runtime_value_*_T functions return the chosen value.
 * This is either the received value from RUNTIME_PARAMETER_*_T or the standard value from the calibration. In both cases the
 * Runtime_Parameter_Error_T holds information on what happened.
 * The features initialization function uses the generate_runtime_value_*_T to generate an internal feature_chosen_runtime_parameter structure
 * that does not hold any flags anymore, just the values for each parameter.
 *
 * Since no module should trust any of its external inputs Runtime_Parameters.h provides test_runtime_value_*_T functions.
 * These can be called with
 *    * the return value of generate_runtime_value_*_T
 *    * a min max value (these should be stored in the modules calibration)
 *    * the standard value from the calibration
 *    * Runtime_Parameter_Error_T
 * These functions then return either the given runtime parameter value or the standard value (if out of range).
 * The error flag Runtime_Parameter_Error_T::out_of_range is set accordingly.
 *
 \*===================================================================*/

#include "reuse.h"
#include "ml_runtime_parameter_boolean_t.h"
#include "ml_runtime_parameter_error_t.h"
#include "ml_runtime_parameter_float32_t.h"
#include "ml_runtime_parameter_int16_t.h"
#include "ml_runtime_parameter_int32_t.h"
#include "ml_runtime_parameter_int8_t.h"
#include "ml_runtime_parameter_legal_state_t.h"
#include "ml_runtime_parameter_uint16_t.h"
#include "ml_runtime_parameter_uint32_t.h"
#include "ml_runtime_parameter_uint8_t.h"

/*===========================================================================*\
* Type Definitions
\*===========================================================================*/



/*===========================================================================*\
* Global Function Declarations
\*===========================================================================*/

/**
 * Returns the runtime parameter value based on the is_set flag in p_runtime_parameter and the given legal_state
 * \ingroup Runtime_Parameters
 * \return           generated runtime parameter
 * \sdd{WI-13601}
 * \sdd{WI-13608}
 */
boolean_T Generate_Runtime_Value_Boolean(
   const Runtime_Parameter_Boolean_T *p_runtime_parameter /**< [in] Structure containing a set flag and a value*/,
   Runtime_Parameter_Legal_State_T    legal_state /**< [in] Can this runtime parameter be set */,
   boolean_T                          default_value /**< [in] The standard value to fall back to */,
   Runtime_Parameter_Error_T         *p_error /**< [out] The runtime parameter error flags*/);

/**
 * Returns the runtime parameter value based on the is_set flag in p_runtime_parameter and the given legal_state
 *\ingroup Runtime_Parameters
 * \return           generated runtime parameter
 * \sdd{WI-13601}
 * \sdd{WI-13607}
 */
uint8_t Generate_Runtime_Value_Uint8(
   const Runtime_Parameter_Uint8_T *p_runtime_parameter /**< [in] Structure containing a set flag and a value*/,
   Runtime_Parameter_Legal_State_T  legal_state /**< [in] Can this runtime parameter be set */,
   uint8_t                          default_value /**< [in] The standard value to fall back to */,
   Runtime_Parameter_Error_T       *p_error /**< [out] The runtime parameter error flags*/);

/**
 * Returns the runtime parameter value based on the is_set flag in p_runtime_parameter and the given legal_state
 *\ingroup Runtime_Parameters
 * \return           generated runtime parameter
 * \sdd{WI-13601}
 * \sdd{WI-13614}
 */
uint16_t Generate_Runtime_Value_Uint16(
   const Runtime_Parameter_Uint16_T *p_runtime_parameter /**< [in] Structure containing a set flag and a value*/,
   Runtime_Parameter_Legal_State_T   legal_state /**< [in] Can this runtime parameter be set */,
   uint16_t                          default_value /**< [in] The standard value to fall back to */,
   Runtime_Parameter_Error_T        *p_error /**< [out] The runtime parameter error flags*/);

/**
 * Returns the runtime parameter value based on the is_set flag in p_runtime_parameter and the given legal_state
 * \ingroup Runtime_Parameters
 * \return           generated runtime parameter
 * \sdd{WI-13601}
 * \sdd{WI-13613}
 */
uint32_t Generate_Runtime_Value_Uint32(
   const Runtime_Parameter_Uint32_T *p_runtime_parameter /**< [in] Structure containing a is_set flag and a value*/,
   Runtime_Parameter_Legal_State_T   legal_state /**< [in] Can this runtime parameter be set */,
   uint32_t                          default_value /**< [in] The standard value to fall back to */,
   Runtime_Parameter_Error_T        *p_error /**< [out] The runtime parameter error flags*/);


/**
 * Returns the runtime parameter value based on the is_set flag in p_runtime_parameter and the given legal_state
 * \ingroup Runtime_Parameters
 * \return           generated runtime parameter
 * \sdd{WI-13601}
 * \sdd{WI-13612}
 */
int8_t Generate_Runtime_Value_Int8(
   const Runtime_Parameter_Int8_T *p_runtime_parameter /**< [in] Structure containing a is_set flag and a value*/,
   Runtime_Parameter_Legal_State_T legal_state /**< [in] Can this runtime parameter be set */,
   int8_t                          default_value /**< [in] The standard value to fall back to */,
   Runtime_Parameter_Error_T      *p_error /**< [out] The runtime parameter error flags*/);

/**
 * Returns the runtime parameter value based on the is_set flag in p_runtime_parameter and the given legal_state
 * \ingroup Runtime_Parameters
 * \return           generated runtime parameter
 * \sdd{WI-13601}
 * \sdd{WI-13611}
 */
int16_t Generate_Runtime_Value_Int16(
   const Runtime_Parameter_Int16_T *p_runtime_parameter /**< [in] Structure containing a is_set flag and a value*/,
   Runtime_Parameter_Legal_State_T  legal_state /**< [in] Can this runtime parameter be set */,
   int16_t                          default_value /**< [in] The standard value to fall back to */,
   Runtime_Parameter_Error_T       *p_error /**< [out] The runtime parameter error flags*/);

/**
 * Returns the runtime parameter value based on the is_set flag in p_runtime_parameter and the given legal_state
 * \ingroup Runtime_Parameters
 * \return           generated runtime parameter
 * \sdd{WI-13601}
 * \sdd{WI-13610}
 */
int32_t Generate_Runtime_Value_Int32(
   const Runtime_Parameter_Int32_T *p_runtime_parameter /**< [in] Structure containing a is_set flag and a value*/,
   Runtime_Parameter_Legal_State_T  legal_state /**< [in] Can this runtime parameter be set */,
   int32_t                          default_value /**< [in] The standard value to fall back to */,
   Runtime_Parameter_Error_T       *p_error /**< [out] The runtime parameter error flags*/);


/**
 * Returns the runtime parameter value based on the is_set flag in p_runtime_parameter and the given legal_state
 * \ingroup Runtime_Parameters
 * \return           generated runtime parameter
 * \sdd{WI-13601}
 * \sdd{WI-13609}
 */
float32_T Generate_Runtime_Value_Float32(
   const Runtime_Parameter_Float32_T *p_runtime_parameter /**< [in] Structure containing a is_set flag and a value*/,
   Runtime_Parameter_Legal_State_T    legal_state /**< [in] Can this runtime parameter be set */,
   float32_T                          default_value /**< [in] The standard value to fall back to */,
   Runtime_Parameter_Error_T         *p_error /**< [out] The runtime parameter error flags*/);

/**
 * Initializes the given error structure
 * \ingroup Runtime_Parameters
 * \sdd{WI-13601}
 */
void Init_Runtime_Param_Error(Runtime_Parameter_Error_T *p_error /**< [out] The runtime parameter error flags*/);

/**
 * maps external runtime parameter to internal runtime parameters.
 * checks if the runtime parameter  must/may/can't be set.
 * checks if the value of the external runtime parameter is in the allowed range.
 * prohibited_to_set is stronger than must_be_set.
 * \ingroup Runtime_Parameters
 * \return         chosen legal state
 * \sdd{WI-13616}
 */
Runtime_Parameter_Legal_State_T Map_Cal_Settings_To_Runtime_Parameter_Legal_State(
   boolean_T must_be_set,/**< [in] indicating that the parameter has to be set */
   boolean_T prohibited_to_set /**< [in] indicating that this parameter is prohibited to be setThe runtime parameter error flags*/);

/**
 * Returns the default_value if runtime_parameter is outside the limits defined  by min_value and max_value, runtime_parameter otherwise
 * \ingroup Runtime_Parameters
 * \return
 * - Value of given runtime_parameter if given runtime_parameter is within range min_value...max_value
 * - Value of given default_value if given runtime_parameter is outside range min_value...max_value
 * \sdd{WI-13601}
 * \sdd{WI-13622}
 */
uint8_t Test_Runtime_Value_Uint8(
   uint8_t                    runtime_parameter, /**< [in] Runtime parameter value to be tested */
   uint8_t                    max_value,         /**< [in] The maximal value allowed for this runtime parameter */
   uint8_t                    min_value,         /**< [in] The minimal value allowed for this runtime parameter */
   uint8_t                    default_value,     /**< [in] The value to be returned if runtime_parameter is outside the range min_value...max_value */
   Runtime_Parameter_Error_T *p_error /**< [in, out] Structure containing error flags. This function sets Runtime_Parameter_Error_T::out_of_range in case runtime_parameter is outside the range
                                       * min_value...max_value. In this case the given default_value is returned. */);

/**
 * Returns the default_value if runtime_parameter is outside the limits defined  by min_value and max_value, runtime_parameter otherwise
 * \ingroup Runtime_Parameters
 * \return
 * - Value of given runtime_parameter if given runtime_parameter is within range min_value...max_value
 * - Value of given default_value if given runtime_parameter is outside range min_value...max_value
 * \sdd{WI-13601}
 * \sdd{WI-13619}
 */
int8_t Test_Runtime_Value_Int8(
   int8_t                     runtime_parameter, /**< [in] Runtime parameter value to be tested */
   int8_t                     max_value,         /**< [in] The maximal value allowed for this runtime parameter */
   int8_t                     min_value,         /**< [in] The minimal value allowed for this runtime parameter */
   int8_t                     default_value,     /**< [in] The value to be returned if runtime_parameter is outside the range min_value...max_value */
   Runtime_Parameter_Error_T *p_error /**< [in, out] Structure containing error flags. This function sets Runtime_Parameter_Error_T::out_of_range in case runtime_parameter is outside the range
                                       * min_value...max_value. In this case the given default_value is returned. */);

/**
 * Returns the default_value if runtime_parameter is outside the limits defined  by min_value and max_value, runtime_parameter otherwise
 * \ingroup Runtime_Parameters
 * \return
 * - Value of given runtime_parameter if given runtime_parameter is within range min_value...max_value
 * - Value of given default_value if given runtime_parameter is outside range min_value...max_value
 * \sdd{WI-13601}
 * \sdd{WI-13621}
 */

uint16_t Test_Runtime_Value_Uint16(
   uint16_t                   runtime_parameter, /**< [in] Runtime parameter value to be tested */
   uint16_t                   max_value,         /**< [in] The maximal value allowed for this runtime parameter */
   uint16_t                   min_value,         /**< [in] The minimal value allowed for this runtime parameter */
   uint16_t                   default_value,     /**< [in] The value to be returned if runtime_parameter is outside the range min_value...max_value */
   Runtime_Parameter_Error_T *p_error /**< [in, out] Structure containing error flags. This function sets Runtime_Parameter_Error_T::out_of_range in case runtime_parameter is outside the range
                                       * min_value...max_value. In this case the given default_value is returned. */);

/**
 * Returns the default_value if runtime_parameter is outside the limits defined  by min_value and max_value, runtime_parameter otherwise
 * \ingroup Runtime_Parameters
 * \return
 * - Value of given runtime_parameter if given runtime_parameter is within range min_value...max_value
 * - Value of given default_value if given runtime_parameter is outside range min_value...max_value
 * \sdd{WI-13601}
 * \sdd{WI-13615}
 */
int16_t Test_Runtime_Value_Int16(
   int16_t                    runtime_parameter, /**< [in] Runtime parameter value to be tested */
   int16_t                    max_value,         /**< [in] The maximal value allowed for this runtime parameter */
   int16_t                    min_value,         /**< [in] The minimal value allowed for this runtime parameter */
   int16_t                    default_value,     /**< [in] The value to be returned if runtime_parameter is outside the range min_value...max_value */
   Runtime_Parameter_Error_T *p_error /**< [in, out] Structure containing error flags. This function sets Runtime_Parameter_Error_T::out_of_range in case runtime_parameter is outside the range
                                       * min_value...max_value. In this case the given default_value is returned. */);

/**
 * Returns the default_value if runtime_parameter is outside the limits defined  by min_value and max_value, runtime_parameter otherwise
 * \ingroup Runtime_Parameters
 * \return
 * - Value of given runtime_parameter if given runtime_parameter is within range min_value...max_value
 * - Value of given default_value if given runtime_parameter is outside range min_value...max_value
 * \sdd{WI-13601}
 * \sdd{WI-13620}
 */
uint32_t Test_Runtime_Value_Uint32(
   uint32_t                   runtime_parameter, /**< [in] Runtime parameter value to be tested */
   uint32_t                   max_value,         /**< [in] The maximal value allowed for this runtime parameter */
   uint32_t                   min_value,         /**< [in] The minimal value allowed for this runtime parameter */
   uint32_t                   default_value,     /**< [in] The value to be returned if runtime_parameter is outside the range min_value...max_value */
   Runtime_Parameter_Error_T *p_error /**< [in, out] Structure containing error flags. This function sets Runtime_Parameter_Error_T::out_of_range in case runtime_parameter is outside the range
                                       * min_value...max_value. In this case the given default_value is returned. */);

/**
 * Returns the default_value if runtime_parameter is outside the limits defined  by min_value and max_value, runtime_parameter otherwise
 * \ingroup Runtime_Parameters
 * \sdd{WI-13601}
 * \sdd{WI-13617}
 */
int32_t Test_Runtime_Value_Int32(
   int32_t                    runtime_parameter, /**< [in] Runtime parameter value to be tested */
   int32_t                    max_value,         /**< [in] The maximal value allowed for this runtime parameter */
   int32_t                    min_value,         /**< [in] The minimal value allowed for this runtime parameter */
   int32_t                    default_value,     /**< [in] The value to be returned if runtime_parameter is outside the range min_value...max_value */
   Runtime_Parameter_Error_T *p_error /**< [in, out] Structure containing error flags. This function sets Runtime_Parameter_Error_T::out_of_range in case runtime_parameter is outside the range
                                       * min_value...max_value. In this case the given default_value is returned. */);

/**
 * Returns the default_value if runtime_parameter is outside the limits defined  by min_value and max_value, runtime_parameter otherwise
 * \ingroup Runtime_Parameters
 * \return
 * - Value of given runtime_parameter if given runtime_parameter is within range min_value...max_value
 * - Value of given default_value if given runtime_parameter is outside range min_value...max_value
 * \sdd{WI-13601}
 * \sdd{WI-13618}
 */
float32_T Test_Runtime_Value_Float32(
   float32_T                  runtime_parameter, /**< [in] Runtime parameter value to be tested */
   float32_T                  max_value,         /**< [in] The maximal value allowed for this runtime parameter */
   float32_T                  min_value,         /**< [in] The minimal value allowed for this runtime parameter */
   float32_T                  default_value,     /**< [in] The value to be returned if runtime_parameter is outside the range min_value...max_value */
   Runtime_Parameter_Error_T *p_error /**< [in, out] Structure containing error flags. This function sets Runtime_Parameter_Error_T::out_of_range in case runtime_parameter is outside the range
                                       * min_value...max_value. In this case the given default_value is returned. */);

/**
 * Sets the value of the runtime_parameter and the is_set flag to TRUE
 * \ingroup Runtime_Parameters
 * \return         Runtime_Parameter_Boolean_T with is_set=TRUE and value set to passed parameter.
 * \sdd{WI-13624}
 */
Runtime_Parameter_Boolean_T Set_Runtime_Parameter_Extern_Boolean(boolean_T input /**< [in] Value to be set in returned runtime parameter structure */);

/**
 * Sets the value of the runtime_parameter and the is_set flag to TRUE
 * \ingroup Runtime_Parameters
 * \return         Runtime_Parameter_Boolean_T with is_set=TRUE and value set to passed parameter.
 * \sdd{WI-13631}
 */
Runtime_Parameter_Uint8_T Set_Runtime_Parameter_Extern_Uint8(uint8_t input /**< [in] Value to be set in returned runtime parameter structure */);

/**
 * Sets the value of the runtime_parameter and the is_set flag to TRUE
 * \ingroup Runtime_Parameters
 * \return         Runtime_Parameter_Boolean_T with is_set=TRUE and value set to passed parameter.
 * \sdd{WI-13630}
 */
Runtime_Parameter_Uint16_T Set_Runtime_Parameter_Extern_Uint16(uint16_t input /**< [in] Value to be set in returned runtime parameter structure */);

/**
 * Sets the value of the runtime_parameter and the is_set flag to TRUE
 * \ingroup Runtime_Parameters
 * \return         Runtime_Parameter_Boolean_T with is_set=TRUE and value set to passed parameter.
 * \sdd{WI-13629}
 */
Runtime_Parameter_Uint32_T Set_Runtime_Parameter_Extern_Uint32(uint32_t input /**< [in] Value to be set in returned runtime parameter structure */);

/**
 * Sets the value of the runtime_parameter and the is_set flag to TRUE
 * \ingroup Runtime_Parameters
 * \return         Runtime_Parameter_Boolean_T with is_set=TRUE and value set to passed parameter.
 * \sdd{WI-13628}
 */
Runtime_Parameter_Int8_T Set_Runtime_Parameter_Extern_Int8(int8_t input /**< [in] Value to be set in returned runtime parameter structure */);

/**
 * Sets the value of the runtime_parameter and the is_set flag to TRUE
 * \ingroup Runtime_Parameters
 * \return         Runtime_Parameter_Boolean_T with is_set=TRUE and value set to passed parameter.
 * \sdd{WI-13627}
 */
Runtime_Parameter_Int16_T Set_Runtime_Parameter_Extern_Int16(int16_t input /**< [in] Value to be set in returned runtime parameter structure */);

/**
 * Sets the value of the runtime_parameter and the is_set flag to TRUE
 * \ingroup Runtime_Parameters
 * \return         Runtime_Parameter_Boolean_T with is_set=TRUE and value set to passed parameter.
 * \sdd{WI-13626}
 */
Runtime_Parameter_Int32_T Set_Runtime_Parameter_Extern_Int32(int32_t input /**< [in] Value to be set in returned runtime parameter structure */);

/**
 * Sets the value of the runtime_parameter and the is_set flat to TRUE
 * \ingroup Runtime_Parameters
 * \return         Runtime_Parameter_Boolean_T with is_set=TRUE and value set to passed parameter.
 * \sdd{WI-13625}
 */
Runtime_Parameter_Float32_T Set_Runtime_Parameter_Extern_Float32(float32_T input /**< [in] Value to be set in returned runtime parameter structure */);

#ifdef __cplusplus
}
#endif
#endif
