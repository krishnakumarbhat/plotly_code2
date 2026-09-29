#ifndef TA_BMW_DIAGNOSTIC_H
#define TA_BMW_DIAGNOSTIC_H

/**
 * @file ta_bmw_diagnostic.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the BMW SRR5 diagnostic header file.
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "pa_reuse.h"
#include "ta_core_calibration_t.h"
#include "ta_input_t.h"

/**
 * @brief Function to get the longitudinal target shift offset for diagnostic job 0x404B.
 *
 * @return longitudinal target shift offset
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{}
 * @SDD{SF-8587}
 * @verification{}
 */
float32_T Ta_Get_Target_Shift_Offset_Long(const Ta_Input_T *p_ta_input /**< TA Input */);

/**
 * @brief Function to get the lateral target shift offset for diagnostic job 0x404B.
 *
 * @return lateral target shift offset
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{}
 * @SDD{SF-8588}
 * @verification{}
 */
float32_T Ta_Get_Target_Shift_Offset_Lat(const Ta_Input_T *p_ta_input /**< TA Input */);

/**
 * @brief Function to determine if diagnostic mode is enabled.
 *
 * @return true if diagnostic mode is enabled, false otherwise
 *
 * @SRS{SF-2352,SF-2278}
 * @SAE{}
 * @SDD{SF-8589}
 * @verification{}
 */
boolean_T Ta_Is_Diagnostic_Mode_Enabled(const Ta_Input_T *p_ta_input /**< TA Input */,
                                        const Ta_Core_Calibration_T *p_ta_cals /**< TA Calibration */);

#endif /* TA_BMW_DIAGNOSTIC_H */
