# ifndef TA_CORE_CALIBRATION_H
# define TA_CORE_CALIBRATION_H

/**
* @file ta_core_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in ta_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "ta_core_calibration_t.h"
#include "pa_reuse.h" // IWYU pragma: keep
#ifdef CT_ACTIVATE_CAL_PRINT
#include <stdio.h>
#endif

/*===========================================================================*\
* Defines
\*===========================================================================*/

/* Macros for minimum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_ALERT_LVL_1_TTP_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_ACTIVE_OBJ_TTP_OFFSET ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_ALERT_LVL_1_TTP_LATE_TRIGGER_HOST_SPEED_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_ALERT_LVL_1_TTP_NORMAL_TRIGGER_HOST_SPEED_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_ALERT_LVL_2_TTC_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_ALERT_LVL_3_TTC_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_ALERT_LVL_3_TTB_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_ALERT_LVL_4_TTC_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_ALERT_LVL_4_DECEL_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_CRITICAL_APPROACH_MIN_SAFE_DISTANCE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_CRITICAL_APPROACH_ANGLE_DIFF_MIN ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_EGO_MAX_PRED_YAW_ANGLE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_OBJ_PRED_SPEED_MIN ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_OBJ_ACCELERATION_LONG_WEIGHT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_OBJ_ACCELERATION_LAT_WEIGHT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_EGO_SHAPE_GAIN_PER_PRED_STEP ((float32_T)(0.1f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_OBJ_SHAPE_GAIN_PER_PRED_STEP ((float32_T)(0.1f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_EGO_SHAPE_GAIN_FIXED ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_OBJ_SHAPE_GAIN_FIXED ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_EGO_SPEED ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_EGO_SPEED_OFST ((float32_T)(-10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_EGO_YAWRATE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_EGO_YAWRATE_OFST ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_EGO_LONG_ACCELERATION ((float32_T)(-20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_EGO_LONG_ACCELERATION_OFST ((float32_T)(-3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_EGO_ACCELERATION_WEIGHT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_EGO_DECELERATION_WEIGHT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_EGO_CIRCLE_OFFSET ((float32_T)(-0.5f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_EGO_CIRCLE_HOST_LENGTH_FACTOR ((float32_T)(0.5f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_EGO_YAWANGLE_INTEGRATION_YAWRATE_MIN ((float32_T)(0.01f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_EXIST_PRBLTY ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_EXIST_PRBLTY_OFST ((float32_T)(-1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_VCS_LONG_VEL_REL ((float32_T)(-100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_VCS_LONG_VEL_REL_OFST ((float32_T)(-10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_VCS_LAT_VEL_REL ((float32_T)(-100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_VCS_LAT_VEL_REL_OFST ((float32_T)(-10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_VCS_LONG_VEL ((float32_T)(-100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_VCS_LONG_VEL_OFST ((float32_T)(-10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_VCS_LAT_VEL ((float32_T)(-100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_VCS_LAT_VEL_OFST ((float32_T)(-10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_STRAIGHT_HOST_CURVATURE_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_LOOKUP_TURNING_HOST_SPEED ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_VCS_LONG_POS_STRAIGHT_MIN ((float32_T)(-1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_HEADING ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_HEADING_STRAIGHT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_HEADING_OFST ((float32_T)(-2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_HEADING_RATE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_HEADING_RATE_STRAIGHT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_HEADING_RATE_OFST ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_SPEED ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_SPEED_STRAIGHT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_SPEED_OFST ((float32_T)(-2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_LENGTH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_LENGTH_OFST ((float32_T)(-2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_WIDTH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_WIDTH_OFST ((float32_T)(-2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_AREA ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_AREA_OFST ((float32_T)(-2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_VRU_CLASS_PROB ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_VRU_CLASS_PROB_OFST ((float32_T)(-1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_EGO_OBJ_HEADING_DIFF ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_EGO_OBJ_HEADING_DIFF_OFST ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_ECLIPSE_VALUE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_ECLIPSE_VALUE_OFST ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_VELOCITY_HEADING_DIFF_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_BRAKE_DECELERATION_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_BRAKE_DEAD_TIME ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_BRAKE_GRADIENT ((float32_T)(-100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_DANGER_ZONE_LEFT_LONG ((float32_T)(-10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_DANGER_ZONE_LEFT_LAT ((float32_T)(-15.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_DANGER_ZONE_RIGHT_LONG ((float32_T)(-10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_DANGER_ZONE_RIGHT_LAT ((float32_T)(-15.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_PFGS_EGO_SPEED ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_PFGS_QUALIFICATION_TTC_MIN ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TAP_LVL_2_HOST_CURVATURE_MIN ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_EXIST_PRBLTY ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_EXIST_PRBLTY_OFST ((float32_T)(-1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_VCS_LONG_VEL_REL ((float32_T)(-100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_VCS_LONG_VEL_REL_OFST ((float32_T)(-10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_VCS_LAT_VEL_REL ((float32_T)(-100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_VCS_LAT_VEL_REL_OFST ((float32_T)(-10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_VCS_LONG_VEL ((float32_T)(-100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_VCS_LONG_VEL_OFST ((float32_T)(-10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_VCS_LAT_VEL ((float32_T)(-100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_VCS_LAT_VEL_OFST ((float32_T)(-10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_HEADING ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_HEADING_OFST ((float32_T)(-2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_SPEED ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_SPEED_OFST ((float32_T)(-2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_LENGTH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_LENGTH_OFST ((float32_T)(-2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_WIDTH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_WIDTH_OFST ((float32_T)(-2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_VRU_CLASS_PROB ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_VRU_CLASS_PROB_OFST ((float32_T)(-1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_ECLIPSE_VALUE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_ECLIPSE_VALUE_OFST ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_TTP_OBJ_ABS_LAT_VEL_REL_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_TTP_OBJ_ABS_HEADING_DIFF_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_TTP_CURVE_SUPPRESSION_OBJ_DISTANCE_MIN ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_INFO_ZONE_LEFT_LONG ((float32_T)(-40.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_INFO_ZONE_LEFT_LAT ((float32_T)(-10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_INFO_ZONE_RIGHT_LONG ((float32_T)(-40.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_INFO_ZONE_RIGHT_LAT ((float32_T)(-10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_INFO_ZONE_LEFT_LONG_HYS ((float32_T)(-2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_INFO_ZONE_LEFT_LAT_HYS ((float32_T)(-2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_INFO_ZONE_RIGHT_LONG_HYS ((float32_T)(-2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_INFO_ZONE_RIGHT_LAT_HYS ((float32_T)(-2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_WING_ZONE_LEFT_LONG ((float32_T)(-15.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_WING_ZONE_LEFT_LAT ((float32_T)(-15.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_WING_ZONE_RIGHT_LONG ((float32_T)(-15.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_WING_ZONE_RIGHT_LAT ((float32_T)(-15.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_WING_ZONE_LEFT_LONG_HYS ((float32_T)(-2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_WING_ZONE_LEFT_LAT_HYS ((float32_T)(-2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_WING_ZONE_RIGHT_LONG_HYS ((float32_T)(-2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_WING_ZONE_RIGHT_LAT_HYS ((float32_T)(-2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_ALWAYS_OVERWRITE_TA_MODE_TO_BOTH ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_F_ONLY_ALLOW_CONSECUTIVE_TTC_BASED_ALERT_LEVELS ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_F_SKIP_HOLDING_FOR_SINGLE_ALERT_LEVEL_DROP ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_F_ONLY_ALLOW_TTC_BASED_ALERT_LEVEL_FOR_MATURE_OBJ ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_F_APPLY_TTP_HYSTERESIS_GLOBALLY ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_F_TA_ENABLE_DEBUG_MODE ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_F_FTA_ENABLE ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_F_FTA_ENABLE_BRAKE_GRADIENT_LOGIC ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_F_FTA_ENABLE_DANGER_ZONES ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_PFGS_SYMBOL_REQUEST_SIDES_ENABLED ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_PFGS_QUALIFICATION_CHECK_F_STATIONARY ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_F_RTA_ENABLE ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_F_RTA_ENABLE_INFO_ZONES ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_F_HIGHER_OBJ_CRIT_BASED_ON_LOWER_TTP ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_F_RTA_ENABLE_WING_ZONES ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_ALERT_QUALIFYING_CYCLES ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_ALERT_HOLDING_CYCLES ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_PREDICTION_STEPS_MAX ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_CRITICAL_APPROACH_CHECK_EGO_CIRCLES ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_TA_EGO_PRED_CONST_VELOCITY_PRED_STEPS_MIN ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_OBJ_AGE_MIN ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_FTA_DANGER_ZONE_POINT_SIZE ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_PFGS_QUALIFICATION_COUNTER_FAST_OBJ ((uint8_t)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_PFGS_QUALIFICATION_COUNTER_SLOW_OBJ ((uint8_t)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_OBJ_AGE_MIN ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_INFO_ZONE_POINT_SIZE ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_RTA_WING_ZONE_POINT_SIZE ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_UNUSED_PADDING_BYTE_0 ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_UNUSED_PADDING_BYTE_1 ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MIN_K_UNUSED_PADDING_BYTE_2 ((uint8_t)(0u)) 

/* Macros for maximum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_ALERT_LVL_1_TTP_THRESHOLD ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_ACTIVE_OBJ_TTP_OFFSET ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_ALERT_LVL_1_TTP_LATE_TRIGGER_HOST_SPEED_MAX ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_ALERT_LVL_1_TTP_NORMAL_TRIGGER_HOST_SPEED_MAX ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_ALERT_LVL_2_TTC_THRESHOLD ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_ALERT_LVL_3_TTC_THRESHOLD ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_ALERT_LVL_3_TTB_THRESHOLD ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_ALERT_LVL_4_TTC_THRESHOLD ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_ALERT_LVL_4_DECEL_THRESHOLD ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_CRITICAL_APPROACH_MIN_SAFE_DISTANCE ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_CRITICAL_APPROACH_ANGLE_DIFF_MIN ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_EGO_MAX_PRED_YAW_ANGLE ((float32_T)(3.14f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_OBJ_PRED_SPEED_MIN ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_OBJ_ACCELERATION_LONG_WEIGHT ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_OBJ_ACCELERATION_LAT_WEIGHT ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_EGO_SHAPE_GAIN_PER_PRED_STEP ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_OBJ_SHAPE_GAIN_PER_PRED_STEP ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_EGO_SHAPE_GAIN_FIXED ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_OBJ_SHAPE_GAIN_FIXED ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_EGO_SPEED ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_EGO_SPEED_OFST ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_EGO_YAWRATE ((float32_T)(1.5f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_EGO_YAWRATE_OFST ((float32_T)(3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_EGO_LONG_ACCELERATION ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_EGO_LONG_ACCELERATION_OFST ((float32_T)(3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_EGO_ACCELERATION_WEIGHT ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_EGO_DECELERATION_WEIGHT ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_EGO_CIRCLE_OFFSET ((float32_T)(0.5f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_EGO_CIRCLE_HOST_LENGTH_FACTOR ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_EGO_YAWANGLE_INTEGRATION_YAWRATE_MIN ((float32_T)(0.10f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_EXIST_PRBLTY ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_EXIST_PRBLTY_OFST ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_VCS_LONG_VEL_REL ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_VCS_LONG_VEL_REL_OFST ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_VCS_LAT_VEL_REL ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_VCS_LAT_VEL_REL_OFST ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_VCS_LONG_VEL ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_VCS_LONG_VEL_OFST ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_VCS_LAT_VEL ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_VCS_LAT_VEL_OFST ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_STRAIGHT_HOST_CURVATURE_MAX ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_LOOKUP_TURNING_HOST_SPEED ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_VCS_LONG_POS_STRAIGHT_MIN ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_HEADING ((float32_T)(3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_HEADING_STRAIGHT ((float32_T)(3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_HEADING_OFST ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_HEADING_RATE ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_HEADING_RATE_STRAIGHT ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_HEADING_RATE_OFST ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_SPEED ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_SPEED_STRAIGHT ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_SPEED_OFST ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_LENGTH ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_LENGTH_OFST ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_WIDTH ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_WIDTH_OFST ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_AREA ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_AREA_OFST ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_VRU_CLASS_PROB ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_VRU_CLASS_PROB_OFST ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_EGO_OBJ_HEADING_DIFF ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_EGO_OBJ_HEADING_DIFF_OFST ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_ECLIPSE_VALUE ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_ECLIPSE_VALUE_OFST ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_VELOCITY_HEADING_DIFF_MAX ((float32_T)(0.5f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_BRAKE_DECELERATION_MAX ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_BRAKE_DEAD_TIME ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_BRAKE_GRADIENT ((float32_T)(-25.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_DANGER_ZONE_LEFT_LONG ((float32_T)(15.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_DANGER_ZONE_LEFT_LAT ((float32_T)(15.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_DANGER_ZONE_RIGHT_LONG ((float32_T)(15.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_DANGER_ZONE_RIGHT_LAT ((float32_T)(15.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_PFGS_EGO_SPEED ((float32_T)(15.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_PFGS_QUALIFICATION_TTC_MIN ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TAP_LVL_2_HOST_CURVATURE_MIN ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_EXIST_PRBLTY ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_EXIST_PRBLTY_OFST ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_VCS_LONG_VEL_REL ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_VCS_LONG_VEL_REL_OFST ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_VCS_LAT_VEL_REL ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_VCS_LAT_VEL_REL_OFST ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_VCS_LONG_VEL ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_VCS_LONG_VEL_OFST ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_VCS_LAT_VEL ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_VCS_LAT_VEL_OFST ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_HEADING ((float32_T)(3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_HEADING_OFST ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_SPEED ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_SPEED_OFST ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_LENGTH ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_LENGTH_OFST ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_WIDTH ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_WIDTH_OFST ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_VRU_CLASS_PROB ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_VRU_CLASS_PROB_OFST ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_ECLIPSE_VALUE ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_ECLIPSE_VALUE_OFST ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_TTP_OBJ_ABS_LAT_VEL_REL_MAX ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_TTP_OBJ_ABS_HEADING_DIFF_MAX ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_TTP_CURVE_SUPPRESSION_OBJ_DISTANCE_MIN ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_INFO_ZONE_LEFT_LONG ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_INFO_ZONE_LEFT_LAT ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_INFO_ZONE_RIGHT_LONG ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_INFO_ZONE_RIGHT_LAT ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_INFO_ZONE_LEFT_LONG_HYS ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_INFO_ZONE_LEFT_LAT_HYS ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_INFO_ZONE_RIGHT_LONG_HYS ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_INFO_ZONE_RIGHT_LAT_HYS ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_WING_ZONE_LEFT_LONG ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_WING_ZONE_LEFT_LAT ((float32_T)(15.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_WING_ZONE_RIGHT_LONG ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_WING_ZONE_RIGHT_LAT ((float32_T)(15.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_WING_ZONE_LEFT_LONG_HYS ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_WING_ZONE_LEFT_LAT_HYS ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_WING_ZONE_RIGHT_LONG_HYS ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_WING_ZONE_RIGHT_LAT_HYS ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_ALWAYS_OVERWRITE_TA_MODE_TO_BOTH ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_F_ONLY_ALLOW_CONSECUTIVE_TTC_BASED_ALERT_LEVELS ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_F_SKIP_HOLDING_FOR_SINGLE_ALERT_LEVEL_DROP ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_F_ONLY_ALLOW_TTC_BASED_ALERT_LEVEL_FOR_MATURE_OBJ ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_F_APPLY_TTP_HYSTERESIS_GLOBALLY ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_F_TA_ENABLE_DEBUG_MODE ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_F_FTA_ENABLE ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_F_FTA_ENABLE_BRAKE_GRADIENT_LOGIC ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_F_FTA_ENABLE_DANGER_ZONES ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_PFGS_SYMBOL_REQUEST_SIDES_ENABLED ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_PFGS_QUALIFICATION_CHECK_F_STATIONARY ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_F_RTA_ENABLE ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_F_RTA_ENABLE_INFO_ZONES ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_F_HIGHER_OBJ_CRIT_BASED_ON_LOWER_TTP ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_F_RTA_ENABLE_WING_ZONES ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_ALERT_QUALIFYING_CYCLES ((uint8_t)(20u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_ALERT_HOLDING_CYCLES ((uint8_t)(20u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_PREDICTION_STEPS_MAX ((uint8_t)(20u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_CRITICAL_APPROACH_CHECK_EGO_CIRCLES ((uint8_t)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_TA_EGO_PRED_CONST_VELOCITY_PRED_STEPS_MIN ((uint8_t)(40u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_OBJ_AGE_MIN ((uint8_t)(25u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_FTA_DANGER_ZONE_POINT_SIZE ((uint8_t)(8u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_PFGS_QUALIFICATION_COUNTER_FAST_OBJ ((uint8_t)(20u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_PFGS_QUALIFICATION_COUNTER_SLOW_OBJ ((uint8_t)(20u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_OBJ_AGE_MIN ((uint8_t)(20u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_INFO_ZONE_POINT_SIZE ((uint8_t)(8u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_RTA_WING_ZONE_POINT_SIZE ((uint8_t)(8u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_UNUSED_PADDING_BYTE_0 ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_UNUSED_PADDING_BYTE_1 ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_MAX_K_UNUSED_PADDING_BYTE_2 ((uint8_t)(255u)) 


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
 * @SDD{CSCSA-216783}
 * @verification{}
 **/
void Ta_Core_Cal_Reverse_Array_Ta_Cal(Ta_Core_Calibration_T* cal_dst);
#endif /*CT_BIG_ENDIAN*/

#ifdef CT_ACTIVATE_CAL_PRINT

/**
 * @brief Prints values of calibrations.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{CSCSA-216784}
 * @verification{}
 **/
void Ta_Core_Cal_Print(FILE* c_file_ptr, const Ta_Core_Calibration_T* p_cals);

#endif /*CT_ACTIVATE_CAL_PRINT*/

/**
 * @brief This function updates all calibrations of the component to their respective defaults given by the customer specific xml sheet.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{n/a}
 * @SDD{CSCSA-216781}
 * @verification{}
 **/
void Ta_Core_Cal_Update_Defaults(Ta_Core_Calibration_T* cal_dst);



#endif /* TA_CORE_CALIBRATION_H */
