/**
 * @file ct_boundaries_check_function_helpers.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Implementation of the helper functions for calibration boundaries check.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "ct_boundaries_check_function_helpers.h"

/*===========================================================================*\
 * Function definitions
\*===========================================================================*/

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Ct_Is_Float_In_Bondaries(boolean_T *f_lcda_calibration_in_boundaries,
                                                     float32_T min,
                                                     float32_T value,
                                                     float32_T max)
{
    *f_lcda_calibration_in_boundaries = (boolean_T) ((*f_lcda_calibration_in_boundaries) && (min <= value) && (value <= max));
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Ct_Is_Uint8_In_Bondaries(boolean_T *f_lcda_calibration_in_boundaries, uint8_t min, uint8_t value, uint8_t max)
{
    *f_lcda_calibration_in_boundaries = (boolean_T) ((*f_lcda_calibration_in_boundaries) && (min <= value) && (value <= max));
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Ct_Is_Uint16_In_Bondaries(boolean_T *f_lcda_calibration_in_boundaries, uint16_t min, uint16_t value, uint16_t max)
{
    *f_lcda_calibration_in_boundaries = (boolean_T) ((*f_lcda_calibration_in_boundaries) && (min <= value) && (value <= max));
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Ct_Is_Uint32_In_Bondaries(boolean_T *f_lcda_calibration_in_boundaries, uint32_t min, uint32_t value, uint32_t max)
{
    *f_lcda_calibration_in_boundaries = (boolean_T) ((*f_lcda_calibration_in_boundaries) && (min <= value) && (value <= max));
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Ct_Is_Int8_In_Bondaries(boolean_T *f_lcda_calibration_in_boundaries, int8_t min, int8_t value, int8_t max)
{
    *f_lcda_calibration_in_boundaries = (boolean_T) ((*f_lcda_calibration_in_boundaries) && (min <= value) && (value <= max));
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Ct_Is_Int16_In_Bondaries(boolean_T *f_lcda_calibration_in_boundaries, int16_t min, int16_t value, int16_t max)
{
    *f_lcda_calibration_in_boundaries = (boolean_T) ((*f_lcda_calibration_in_boundaries) && (min <= value) && (value <= max));
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Ct_Is_Int32_In_Bondaries(boolean_T *f_lcda_calibration_in_boundaries, int32_t min, int32_t value, int32_t max)
{
    *f_lcda_calibration_in_boundaries = (boolean_T) ((*f_lcda_calibration_in_boundaries) && (min <= value) && (value <= max));
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage] */
void Ct_Is_Bool_In_Bondaries(boolean_T *f_lcda_calibration_in_boundaries, boolean_T min, boolean_T value, boolean_T max)
{
    *f_lcda_calibration_in_boundaries = (boolean_T) ((*f_lcda_calibration_in_boundaries) && ((min == value) || (value == max)));
}
