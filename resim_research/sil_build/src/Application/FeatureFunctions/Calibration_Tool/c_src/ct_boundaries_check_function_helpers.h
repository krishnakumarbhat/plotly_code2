#ifndef CT_BOUNDARIES_CHECK_FUNCTION_HELPERS_H
#define CT_BOUNDARIES_CHECK_FUNCTION_HELPERS_H


/**
 * @file ct_endianness_switch.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Declaration of array reversing of SFL calibration tool.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "reuse.h"

/*===========================================================================*\
 * Function declaration
\*===========================================================================*/

/**
 * @brief Checks if the given float calibration value is between the boundaries and sets value (tru/false) of the main control parameter.
 * Takes into account current value.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 */
void Ct_Is_Float_In_Bondaries(boolean_T *f_lcda_calibration_in_boundaries,
                                                     float32_T min,
                                                     float32_T value,
                                                     float32_T max);
/**
 * @brief Checks if the given integer calibration value is between the boundaries and sets value (tru/false) of the main control parameter.
 * Takes into account current value.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 */
void Ct_Is_Uint8_In_Bondaries(boolean_T *f_lcda_calibration_in_boundaries, uint8_t min, uint8_t value, uint8_t max);

/**
 * @brief Checks if the given integer calibration value is between the boundaries and sets value (tru/false) of the main control
 * parameter. Takes into account current value.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 */
void Ct_Is_Uint16_In_Bondaries(boolean_T *f_lcda_calibration_in_boundaries, uint16_t min, uint16_t value, uint16_t max);

/**
 * @brief Checks if the given integer calibration value is between the boundaries and sets value (tru/false) of the main control
 * parameter. Takes into account current value.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 */
void Ct_Is_Uint32_In_Bondaries(boolean_T *f_lcda_calibration_in_boundaries, uint32_t min, uint32_t value, uint32_t max);

/**
 * @brief Checks if the given integer calibration value is between the boundaries and sets value (tru/false) of the main control
 * parameter. Takes into account current value.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 */
void Ct_Is_Int8_In_Bondaries(boolean_T *f_lcda_calibration_in_boundaries, int8_t min, int8_t value, int8_t max);

/**
 * @brief Checks if the given integer calibration value is between the boundaries and sets value (tru/false) of the main control
 * parameter. Takes into account current value.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 */
void Ct_Is_Int16_In_Bondaries(boolean_T *f_lcda_calibration_in_boundaries, int16_t min, int16_t value, int16_t max);

/**
 * @brief Checks if the given integer calibration value is between the boundaries and sets value (tru/false) of the main control
 * parameter. Takes into account current value.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 */
void Ct_Is_Int32_In_Bondaries(boolean_T *f_lcda_calibration_in_boundaries, int32_t min, int32_t value, int32_t max);

/**
 * @brief Checks if the given boolean calibration value is between the boundaries and sets value (tru/false) of the main control
 * parameter. Takes into account current value.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{}
 */
void Ct_Is_Bool_In_Bondaries(boolean_T *f_lcda_calibration_in_boundaries, boolean_T min, boolean_T value, boolean_T max);

#endif /* CT_BOUNDARIES_CHECK_FUNCTION_HELPERS_H*/
