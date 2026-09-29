# ifndef ESA_CORE_CALIBRATION_H
# define ESA_CORE_CALIBRATION_H

/**
* @file esa_core_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in esa_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "esa_core_calibration_t.h"
#include "pa_reuse.h" // IWYU pragma: keep
#ifdef CT_ACTIVATE_CAL_PRINT
#include <stdio.h>
#endif

/*===========================================================================*\
* Defines
\*===========================================================================*/

/* Macros for minimum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_ZONE_X ((float32_T)(-100.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_ZONE_X_HYS ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_ZONE_Y ((float32_T)(-100.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_ZONE_Y_HYS ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_MAX_RANGE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_MIN_LANE_WIDTH ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_MAX_LANE_WIDTH ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_MIN_EXIST_PROB ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_MIN_CURVE_RADIUS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_MIN_CURVE_RADIUS_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_MAX_CURVI_HEADING_ABS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_MIN_OBJ_CURVI_LONG_VEL_ABS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_CRITICAL_LONGITUDINAL_TTC ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_CRITICAL_LONGITUDINAL_TTC_HYS ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_OBJ_SAFE_DECELERATION_THRESHOLD ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_OBJ_SAFE_DECELERATION_THRESHOLD_HYS ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_HOST_ACTIVATION_SPEED_MIN ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_HOST_ACTIVATION_SPEED_MIN_HYS ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_HOST_ACTIVATION_SPEED_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_HOST_ACTIVATION_SPEED_MAX_HYS ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_F_ENABLE_VIA_CAL ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_F_ENABLE ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_F_ALLOW_MIN_CURVE_RADIUS ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_F_ALLOW_OBJ_CRITICAL_TTC_AND_DECELERATION ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_F_ALLOW_OBJ_SELECTION_TTC ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_F_ALLOW_OBJ_SELECTION_DECELERATION ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_F_ALLOW_OBJ_SELECTION_LONG_DISTANCE ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_MIN_TRACK_AGE ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_MIN_MATURE_CYCLES ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_ESA_ALERT_HOLDING_CYCLES ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_UNUSED_PADDING_BYTE_0 ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MIN_K_UNUSED_PADDING_BYTE_1 ((uint8_t)(0u)) 

/* Macros for maximum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_ZONE_X ((float32_T)(100.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_ZONE_X_HYS ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_ZONE_Y ((float32_T)(100.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_ZONE_Y_HYS ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_MAX_RANGE ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_MIN_LANE_WIDTH ((float32_T)(15.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_MAX_LANE_WIDTH ((float32_T)(15.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_MIN_EXIST_PROB ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_MIN_CURVE_RADIUS ((float32_T)(50.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_MIN_CURVE_RADIUS_HYS ((float32_T)(50.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_MAX_CURVI_HEADING_ABS ((float32_T)(3.142f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_MIN_OBJ_CURVI_LONG_VEL_ABS ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_CRITICAL_LONGITUDINAL_TTC ((float32_T)(30.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_CRITICAL_LONGITUDINAL_TTC_HYS ((float32_T)(30.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_OBJ_SAFE_DECELERATION_THRESHOLD ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_OBJ_SAFE_DECELERATION_THRESHOLD_HYS ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_HOST_ACTIVATION_SPEED_MIN ((float32_T)(70.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_HOST_ACTIVATION_SPEED_MIN_HYS ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_HOST_ACTIVATION_SPEED_MAX ((float32_T)(150.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_HOST_ACTIVATION_SPEED_MAX_HYS ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_F_ENABLE_VIA_CAL ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_F_ENABLE ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_F_ALLOW_MIN_CURVE_RADIUS ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_F_ALLOW_OBJ_CRITICAL_TTC_AND_DECELERATION ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_F_ALLOW_OBJ_SELECTION_TTC ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_F_ALLOW_OBJ_SELECTION_DECELERATION ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_F_ALLOW_OBJ_SELECTION_LONG_DISTANCE ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_MIN_TRACK_AGE ((uint8_t)(100u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_MIN_MATURE_CYCLES ((uint8_t)(10u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_ESA_ALERT_HOLDING_CYCLES ((uint8_t)(20u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_UNUSED_PADDING_BYTE_0 ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_MAX_K_UNUSED_PADDING_BYTE_1 ((uint8_t)(255u)) 


/*===========================================================================*\
* Global function declarations
\*===========================================================================*/

#ifdef CT_BIG_ENDIAN
/**
 * @brief Reverses arrays with variable length of 1, 2 or 4 bytes. Dependent on whether arrays are given.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{CSCSA-216530}
 * @verification{}
 **/
void Esa_Core_Cal_Reverse_Array_Esa_Cal(Esa_Core_Calibration_T* cal_dst);
#endif /*CT_BIG_ENDIAN*/

#ifdef CT_ACTIVATE_CAL_PRINT

/**
 * @brief Prints values of calibrations.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{CSCSA-216528}
 * @verification{}
 **/
void Esa_Core_Cal_Print(FILE* c_file_ptr, const Esa_Core_Calibration_T* p_cals);

#endif /*CT_ACTIVATE_CAL_PRINT*/

/**
 * @brief This function updates all calibrations of the component to their respective defaults given by the customer specific xml sheet.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{CSCSA-216529}
 * @verification{}
 **/
void Esa_Core_Cal_Update_Defaults(Esa_Core_Calibration_T* cal_dst);



#endif /* ESA_CORE_CALIBRATION_H */
