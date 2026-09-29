# ifndef LTB_PUBLIC_CALIBRATION_H
# define LTB_PUBLIC_CALIBRATION_H

/**
* @file ltb_public_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in ltb_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "ltb_public_calibration_t.h"
#include "pa_reuse.h" // IWYU pragma: keep
#ifdef CT_ACTIVATE_CAL_PRINT
#include <stdio.h>
#endif

/*===========================================================================*\
* Defines
\*===========================================================================*/

/* Macros for minimum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_ZONE_LENGTH ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_ZONE_WIDTH ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_EGO_ACCELERATION_WEIGHT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_EGO_SHAPE_GAIN_FIXED ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_EGO_CIRCLE_OFFSET ((float32_T)(-0.5f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_EGO_CIRCLE_HOST_LENGTH_FACTOR ((float32_T)(0.5f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_EGO_DECELERATION_WEIGHT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_EGO_MAX_PRED_YAW_ANGLE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_EGO_YAWANGLE_INTEGRATION_YAWRATE_MIN ((float32_T)(0.01f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_EGO_SHAPE_GAIN_PER_PRED_STEP ((float32_T)(0.1f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_OBJ_PRED_SPEED_MIN ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_OBJ_SHAPE_GAIN_FIXED ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_OBJ_SHAPE_GAIN_PER_PRED_STEP ((float32_T)(0.1f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_CRITICAL_APPROACH_MIN_SAFE_DISTANCE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_CRITICAL_APPROACH_ANGLE_DIFF_MIN ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_ALERT_LVL_1_TTC_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_ALERT_LVL_2_TTC_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_ALERT_LVL_2_TTB_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_ALERT_LVL_3_TTC_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_ALERT_LVL_3_DECEL_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_BRAKE_DECELERATION_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_BRAKE_DEAD_TIME ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_BRAKE_GRADIENT ((float32_T)(-100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_OBJECT_LONG_VEL_MIN ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_BMW_SP25_V_EGO_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_BMW_SP25_V_EGO_MAX_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_F_ONLY_ALLOW_CONSECUTIVE_TTC_BASED_ALERT_LEVELS ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_F_SKIP_HOLDING_FOR_SINGLE_ALERT_LEVEL_DROP ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_F_LTB_ENABLE_BRAKE_GRADIENT_LOGIC ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_EGO_PRED_CONST_VELOCITY_PRED_STEPS_MIN ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_PREDICTION_STEPS_MAX ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_ALERT_QUALIFYING_CYCLES ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MIN_K_LTB_ALERT_HOLDING_CYCLES ((uint8_t)(0u)) 

/* Macros for maximum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_ZONE_LENGTH ((float32_T)(60.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_ZONE_WIDTH ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_EGO_ACCELERATION_WEIGHT ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_EGO_SHAPE_GAIN_FIXED ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_EGO_CIRCLE_OFFSET ((float32_T)(0.5f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_EGO_CIRCLE_HOST_LENGTH_FACTOR ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_EGO_DECELERATION_WEIGHT ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_EGO_MAX_PRED_YAW_ANGLE ((float32_T)(3.14f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_EGO_YAWANGLE_INTEGRATION_YAWRATE_MIN ((float32_T)(0.10f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_EGO_SHAPE_GAIN_PER_PRED_STEP ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_OBJ_PRED_SPEED_MIN ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_OBJ_SHAPE_GAIN_FIXED ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_OBJ_SHAPE_GAIN_PER_PRED_STEP ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_CRITICAL_APPROACH_MIN_SAFE_DISTANCE ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_CRITICAL_APPROACH_ANGLE_DIFF_MIN ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_ALERT_LVL_1_TTC_THRESHOLD ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_ALERT_LVL_2_TTC_THRESHOLD ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_ALERT_LVL_2_TTB_THRESHOLD ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_ALERT_LVL_3_TTC_THRESHOLD ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_ALERT_LVL_3_DECEL_THRESHOLD ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_BRAKE_DECELERATION_MAX ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_BRAKE_DEAD_TIME ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_BRAKE_GRADIENT ((float32_T)(-25.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_OBJECT_LONG_VEL_MIN ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_BMW_SP25_V_EGO_MAX ((float32_T)(50.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_BMW_SP25_V_EGO_MAX_HYS ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_F_ONLY_ALLOW_CONSECUTIVE_TTC_BASED_ALERT_LEVELS ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_F_SKIP_HOLDING_FOR_SINGLE_ALERT_LEVEL_DROP ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_F_LTB_ENABLE_BRAKE_GRADIENT_LOGIC ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_EGO_PRED_CONST_VELOCITY_PRED_STEPS_MIN ((uint8_t)(40u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_PREDICTION_STEPS_MAX ((uint8_t)(20u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_ALERT_QUALIFYING_CYCLES ((uint8_t)(20u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LTB_MAX_K_LTB_ALERT_HOLDING_CYCLES ((uint8_t)(20u)) 


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
 * @SDD{n/a}
 * @verification{}
 **/
void Ltb_Public_Cal_Reverse_Array_Ltb_Cal(Ltb_Public_Calibration_T* cal_dst);
#endif /*CT_BIG_ENDIAN*/

#ifdef CT_ACTIVATE_CAL_PRINT

/**
 * @brief Prints values of calibrations.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
void Ltb_Public_Cal_Print(FILE* c_file_ptr, const Ltb_Public_Calibration_T* p_cals);

#endif /*CT_ACTIVATE_CAL_PRINT*/

/**
 * @brief This function updates all calibrations of the component to their respective defaults given by the customer specific xml sheet.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{n/a}
 * @verification{}
 **/
void Ltb_Public_Cal_Update_Defaults(Ltb_Public_Calibration_T* cal_dst);



#endif /* LTB_PUBLIC_CALIBRATION_H */
