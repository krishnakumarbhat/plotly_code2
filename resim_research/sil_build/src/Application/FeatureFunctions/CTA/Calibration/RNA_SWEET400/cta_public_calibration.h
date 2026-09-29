# ifndef CTA_PUBLIC_CALIBRATION_H
# define CTA_PUBLIC_CALIBRATION_H

/**
* @file cta_public_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in cta_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "cta_public_calibration_t.h"
#include "pa_reuse.h" // IWYU pragma: keep
#ifdef CT_ACTIVATE_CAL_PRINT
#include <stdio.h>
#endif

/*===========================================================================*\
* Defines
\*===========================================================================*/

/* Macros for minimum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_EGO_ABS_SPEED_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_STOP_ALERT_TTC ((float32_T)(-3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_STOP_ALERT_TTP ((float32_T)(-3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_MIN_SPEED ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_BUTTERFLY_LONG ((float32_T)(-90.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_BUTTERFLY_LAT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_TTC_CRITICALITY_LEVEL ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_SPEED_CRITICALITY_LEVEL ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_MAX_LONG_POINT_CRITICALITY_LEVEL ((float32_T)(-15.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_MIN_LONG_POINT_CRITICALITY_LEVEL ((float32_T)(-20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_MIN_LATERAL_APPROACH_SPEED ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_MAX_SPEED ((float32_T)(7.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_MAX_LENGTH_FOV ((float32_T)(25.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_HEADING_RANGE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_ANGLES_ZONE_DEFINITION ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_MIN_TTC_ADDITIONAL_MATURE_QUALIFICATION ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_MAX_OBJECT_ECLIPSE_FOR_LEVEL_QUALIFICATION ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_INTERSECTION_LINE_HOST_WIDTH_PERCENTAGE ((float32_T)(0.00f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTB_MIN_TTP ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_SENSOR_FOV_BORDER ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_MAX_HEADING_VARIANCE ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_RANGE_TO_PATH_SEGMENT_GHOST_QUALIF ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_BMW_SP25_EGO_ABS_SPEED_MAX_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_BMW_SP25_BANNER_CRITERIA_CHECK_TIME ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_BMW_SP25_BANNER_TIME ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_STLA_CRIT_ZONE_G_E_LINE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_STLA_CRIT_ZONE_D_C_LINE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_STLA_CRIT_ZONE_N_Q_LINE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_STLA_CRIT_ZONE_Q_QH_LINE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_PEDESTRIAN_MIN_SIZE ((float32_T)(0.01f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_PEDESTRIAN_MIN_SPEED ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_2WHEEL_MIN_SIZE ((float32_T)(0.01f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_2WHEEL_MIN_SPEED ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_MIN_DECELERATION_VALUE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_MAX_DECELERATION_VALUE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_F_ADAPT_INTERSECT_LINES_BY_STEERING_ANGLE ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_F_ADAPT_INTERSECT_LINES_BY_OBJ_HEADING ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_F_APPLY_HEADING_COMPENSATION_ON_INTERSECTION_POINT ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_F_CALC_TTC_EGO_SIDE_ENABLED ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_F_USE_HEADING_FOR_RELATIVE_VELOCITY_CALCULATION ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_F_USE_GHOST_DETECTOR ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_F_USE_OBJECT_MIN_OBJECT_AGE_IN_CYCLES ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_F_ENABLE_THRES_CRIT_LEVEL_RESET ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_F_USE_FRONT_CORNERS_DIST_STOP ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_BMW_SP25_BANNER_CRITERIA_CHECK ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_F_STOP_MODE_TTP ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_ENABLE_MODES ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_CYCLE_COUNT_SUPPRESS_TRUE_WARNING ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_CYCLE_COUNT_HOLD_TRUE_WARNING ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_MIN_OBJECT_AGE_CHECK_VALID ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_MIN_OBJECT_AGE_THRES ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_CYCLES_COASTED_TO_IGNORE ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_OBJECT_SUPRESS_COUNTER ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_MIN_MATURE_CYCLES_LEVEL_QUALIFICTION ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_ADDITIONAL_QUALIFICATION_MATURE_CYCLES ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_MIN_AGE_OBJ_OUTSIDE_SENSOR_FOV ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MIN_K_CTA_MIN_QUAL_AGE_OBJ_CROSSING_PATHS ((uint8_t)(0u)) 

/* Macros for maximum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_EGO_ABS_SPEED_MAX ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_STOP_ALERT_TTC ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_STOP_ALERT_TTP ((float32_T)(3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_MIN_SPEED ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_BUTTERFLY_LONG ((float32_T)(90.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_BUTTERFLY_LAT ((float32_T)(90.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_TTC_CRITICALITY_LEVEL ((float32_T)(6.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_SPEED_CRITICALITY_LEVEL ((float32_T)(50.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_MAX_LONG_POINT_CRITICALITY_LEVEL ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_MIN_LONG_POINT_CRITICALITY_LEVEL ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_MIN_LATERAL_APPROACH_SPEED ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_MAX_SPEED ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_MAX_LENGTH_FOV ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_HEADING_RANGE ((float32_T)(3.14f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_ANGLES_ZONE_DEFINITION ((float32_T)(3.14f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_MIN_TTC_ADDITIONAL_MATURE_QUALIFICATION ((float32_T)(100.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_MAX_OBJECT_ECLIPSE_FOR_LEVEL_QUALIFICATION ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_INTERSECTION_LINE_HOST_WIDTH_PERCENTAGE ((float32_T)(1.00f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTB_MIN_TTP ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_SENSOR_FOV_BORDER ((float32_T)(3.14f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_MAX_HEADING_VARIANCE ((float32_T)(3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_RANGE_TO_PATH_SEGMENT_GHOST_QUALIF ((float32_T)(6.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_BMW_SP25_EGO_ABS_SPEED_MAX_HYS ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_BMW_SP25_BANNER_CRITERIA_CHECK_TIME ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_BMW_SP25_BANNER_TIME ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_STLA_CRIT_ZONE_G_E_LINE ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_STLA_CRIT_ZONE_D_C_LINE ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_STLA_CRIT_ZONE_N_Q_LINE ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_STLA_CRIT_ZONE_Q_QH_LINE ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_PEDESTRIAN_MIN_SIZE ((float32_T)(5.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_PEDESTRIAN_MIN_SPEED ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_2WHEEL_MIN_SIZE ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_2WHEEL_MIN_SPEED ((float32_T)(25.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_MIN_DECELERATION_VALUE ((float32_T)(4.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_MAX_DECELERATION_VALUE ((float32_T)(15.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_F_ADAPT_INTERSECT_LINES_BY_STEERING_ANGLE ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_F_ADAPT_INTERSECT_LINES_BY_OBJ_HEADING ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_F_APPLY_HEADING_COMPENSATION_ON_INTERSECTION_POINT ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_F_CALC_TTC_EGO_SIDE_ENABLED ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_F_USE_HEADING_FOR_RELATIVE_VELOCITY_CALCULATION ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_F_USE_GHOST_DETECTOR ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_F_USE_OBJECT_MIN_OBJECT_AGE_IN_CYCLES ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_F_ENABLE_THRES_CRIT_LEVEL_RESET ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_F_USE_FRONT_CORNERS_DIST_STOP ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_BMW_SP25_BANNER_CRITERIA_CHECK ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_F_STOP_MODE_TTP ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_ENABLE_MODES ((uint8_t)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_CYCLE_COUNT_SUPPRESS_TRUE_WARNING ((uint8_t)(10u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_CYCLE_COUNT_HOLD_TRUE_WARNING ((uint8_t)(10u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_MIN_OBJECT_AGE_CHECK_VALID ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_MIN_OBJECT_AGE_THRES ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_CYCLES_COASTED_TO_IGNORE ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_OBJECT_SUPRESS_COUNTER ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_MIN_MATURE_CYCLES_LEVEL_QUALIFICTION ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_ADDITIONAL_QUALIFICATION_MATURE_CYCLES ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_MIN_AGE_OBJ_OUTSIDE_SENSOR_FOV ((uint8_t)(15u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CTA_MAX_K_CTA_MIN_QUAL_AGE_OBJ_CROSSING_PATHS ((uint8_t)(20u)) 


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
void Cta_Public_Cal_Reverse_Array_Cta_Cal(Cta_Public_Calibration_T* cal_dst);
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
void Cta_Public_Cal_Print(FILE* c_file_ptr, const Cta_Public_Calibration_T* p_cals);

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
void Cta_Public_Cal_Update_Defaults(Cta_Public_Calibration_T* cal_dst);



#endif /* CTA_PUBLIC_CALIBRATION_H */
