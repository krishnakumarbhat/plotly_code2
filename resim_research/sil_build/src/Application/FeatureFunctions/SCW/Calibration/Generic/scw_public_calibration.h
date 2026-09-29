# ifndef SCW_PUBLIC_CALIBRATION_H
# define SCW_PUBLIC_CALIBRATION_H

/**
* @file scw_public_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in scw_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "scw_public_calibration_t.h"
#include "pa_reuse.h" // IWYU pragma: keep
#ifdef CT_ACTIVATE_CAL_PRINT
#include <stdio.h>
#endif

/*===========================================================================*\
* Defines
\*===========================================================================*/

/* Macros for minimum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_MIN_HOST_SPEED ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_MIN_HOST_SPEED_HYS ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_MAX_LAT_POS_RATIO ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_CANDIDATE_HEADING ((float32_T)(-2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_CANDIDATE_HEADING_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_CANDIDATE_YAWRATE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_CANDIDATE_YAWRATE_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_CANDIDATE_VELOCITY ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_CANDIDATE_VELOCITY_HYS ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_CANDIDATE_RELATIVE_VELOCITY ((float32_T)(-20.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_CANDIDATE_RELATIVE_VEL_HYS ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_MIN_CANDIDATE_EXISTENCE_PROBABILITY ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_MIN_EXIST_PROB_RADAR_GUARDRAIL ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_MIN_DYNAMIC_LAT_TTC ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_MAX_DYNAMIC_LAT_TTC ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_MIN_GUARDRAIL_LAT_TTC ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_MAX_GUARDRAIL_LAT_TTC ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_TRAILER_LAT_TTC_EXTENSION ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_MIN_DYNAMIC_LAT_DISTANCE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_MAX_DYNAMIC_LAT_DISTANCE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_MIN_GUARDRAIL_LAT_DISTANCE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_MAX_GUARDRAIL_LAT_DISTANCE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_CRITICAL_LAT_TTC_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_CRITICAL_LAT_DISTANCE_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_INITIAL_ZONE_X ((float32_T)(-15.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_INITIAL_ZONE_Y ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_HYS_ZONE_X_OFFSET ((float32_T)(-10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_HYS_ZONE_Y_OFFSET ((float32_T)(-10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_LATERAL_DISTANCE_DEFAULT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_LATERAL_TTC_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_LATERAL_TTC_DEFAULT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_TTLE_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_TTLE_DEFAULT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_TTP_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_TTP_DEFAULT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_TRAILER_ZONE_EXT_SAFETY_MARGIN ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_TRAILER_ZONE_EXT_SAFETY_MARGIN_LAT ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_MAX_ZONE_LENGTH ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_MAX_ZONE_WIDTH ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_F_ADJUST_ZONES_TO_EGO_SIZE ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_F_ENABLE_VIA_CAL ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_F_DYNAMIC_ENABLE_VIA_CAL ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_F_GUARDRAIL_ENABLE_VIA_CAL ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_F_ENABLE ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_F_ENABLE_DYNAMIC ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_F_ENABLE_GUARDRAIL ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_F_ENABLE_TRAILER_ZONE_EXTENSION ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_F_ENABLE_TRAILER_TTC_EXTENSION ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_MIN_GUARDRAIL_AGE ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_GUARDRAIL_FREEZE_PERIOD ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_MIN_CANDIDATE_AGE ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_CANDIDATE_MATURE_CYCLES_IN_ZONE_THRESHOLD ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MIN_K_SCW_GUARDRAIL_CYCLES_IN_ZONE_THRESHOLD ((uint8_t)(0u)) 

/* Macros for maximum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_MIN_HOST_SPEED ((float32_T)(100.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_MIN_HOST_SPEED_HYS ((float32_T)(100.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_MAX_LAT_POS_RATIO ((float32_T)(1.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_CANDIDATE_HEADING ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_CANDIDATE_HEADING_HYS ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_CANDIDATE_YAWRATE ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_CANDIDATE_YAWRATE_HYS ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_CANDIDATE_VELOCITY ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_CANDIDATE_VELOCITY_HYS ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_CANDIDATE_RELATIVE_VELOCITY ((float32_T)(20.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_CANDIDATE_RELATIVE_VEL_HYS ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_MIN_CANDIDATE_EXISTENCE_PROBABILITY ((float32_T)(1.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_MIN_EXIST_PROB_RADAR_GUARDRAIL ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_MIN_DYNAMIC_LAT_TTC ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_MAX_DYNAMIC_LAT_TTC ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_MIN_GUARDRAIL_LAT_TTC ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_MAX_GUARDRAIL_LAT_TTC ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_TRAILER_LAT_TTC_EXTENSION ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_MIN_DYNAMIC_LAT_DISTANCE ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_MAX_DYNAMIC_LAT_DISTANCE ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_MIN_GUARDRAIL_LAT_DISTANCE ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_MAX_GUARDRAIL_LAT_DISTANCE ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_CRITICAL_LAT_TTC_HYS ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_CRITICAL_LAT_DISTANCE_HYS ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_INITIAL_ZONE_X ((float32_T)(15.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_INITIAL_ZONE_Y ((float32_T)(15.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_HYS_ZONE_X_OFFSET ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_HYS_ZONE_Y_OFFSET ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_LATERAL_DISTANCE_DEFAULT ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_LATERAL_TTC_MAX ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_LATERAL_TTC_DEFAULT ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_TTLE_MAX ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_TTLE_DEFAULT ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_TTP_MAX ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_TTP_DEFAULT ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_TRAILER_ZONE_EXT_SAFETY_MARGIN ((float32_T)(100.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_TRAILER_ZONE_EXT_SAFETY_MARGIN_LAT ((float32_T)(2.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_MAX_ZONE_LENGTH ((float32_T)(30.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_MAX_ZONE_WIDTH ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_F_ADJUST_ZONES_TO_EGO_SIZE ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_F_ENABLE_VIA_CAL ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_F_DYNAMIC_ENABLE_VIA_CAL ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_F_GUARDRAIL_ENABLE_VIA_CAL ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_F_ENABLE ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_F_ENABLE_DYNAMIC ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_F_ENABLE_GUARDRAIL ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_F_ENABLE_TRAILER_ZONE_EXTENSION ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_F_ENABLE_TRAILER_TTC_EXTENSION ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_MIN_GUARDRAIL_AGE ((uint8_t)(20u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_GUARDRAIL_FREEZE_PERIOD ((uint8_t)(100u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_MIN_CANDIDATE_AGE ((uint8_t)(20u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_CANDIDATE_MATURE_CYCLES_IN_ZONE_THRESHOLD ((uint8_t)(100u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_MAX_K_SCW_GUARDRAIL_CYCLES_IN_ZONE_THRESHOLD ((uint8_t)(100u)) 


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
void Scw_Public_Cal_Reverse_Array_Scw_Cal(Scw_Public_Calibration_T* cal_dst);
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
void Scw_Public_Cal_Print(FILE* c_file_ptr, const Scw_Public_Calibration_T* p_cals);

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
void Scw_Public_Cal_Update_Defaults(Scw_Public_Calibration_T* cal_dst);



#endif /* SCW_PUBLIC_CALIBRATION_H */
