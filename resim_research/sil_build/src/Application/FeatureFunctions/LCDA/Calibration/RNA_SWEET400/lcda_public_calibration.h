# ifndef LCDA_PUBLIC_CALIBRATION_H
# define LCDA_PUBLIC_CALIBRATION_H

/**
* @file lcda_public_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in lcda_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "lcda_public_calibration_t.h"
#include "pa_reuse.h" // IWYU pragma: keep
#ifdef CT_ACTIVATE_CAL_PRINT
#include <stdio.h>
#endif

/*===========================================================================*\
* Defines
\*===========================================================================*/

/* Macros for minimum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_HOST_ACTIVATION_SPEED_MIN ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_HOST_ACTIVATION_SPEED_MIN_HYS ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_HOST_ACTIVATION_SPEED_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_HOST_ACTIVATION_SPEED_MAX_HYS ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_DISTANCE_TRAVELED_SCALE_FACTOR ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_MIN_CURVE_RADIUS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_MIN_CURVE_RADIUS_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_ZONE_INTERSECT_CRITICAL_POINT_LATERAL_RATIO ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_LANE_CHANGE_INTENTION_VEL_LAT_THRESH ((float32_T)(-5.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_LANE_CHANGE_INTENTION_POS_LAT_THRES ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_LANE_CHANGE_INTENTION_POS_LONG_THRES ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_EGO_LANE_EFFECTIVE_LANE_WIDTH_FACTOR ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_ZONE_X ((float32_T)(-20.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_ZONE_Y ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_ZONE_X_HYS ((float32_T)(-10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_ZONE_Y_HYS ((float32_T)(-10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_ZONE_Y_HYS_MIN ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_ZONE_Y_HYS_MAX ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_FIXED_ZONE_X ((float32_T)(-20.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_FIXED_ZONE_Y ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_FIXED_ZONE_X_HYS ((float32_T)(-10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_FIXED_ZONE_Y_HYS ((float32_T)(-10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_OVERLAP_AREA_THRESHOLD ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_FALLBACK_REL_VEL_THRES ((float32_T)(-50.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_FALLBACK_REL_VEL_THRES_HYS ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_DYNZONE_SPEED ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_DYNZONE_RANGE ((float32_T)(-255.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_N_LINE_POSITION_FOR_LONG_OBJECT_SOT_SCENARIO ((float32_T)(-100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_LINE_TO_STOP_TOS_ALERT ((float32_T)(-10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_SUPPRESS_LATE_WARNING_MAX_TIME_TILL_LEAVE ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_MAX_HEADING_ABS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_MAX_HEADING_ABS_HYSTERESIS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_MIN_OBJ_LONG_VEL ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_MAX_OBJ_LONG_VEL ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_MIN_OBJ_LONG_VEL_HYSTERESIS ((float32_T)(-10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_MAX_OBJ_LONG_VEL_HYSTERESIS ((float32_T)(-10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_TRAILER_ZONE_EXT_SAFETY_MARGIN ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_TRAILER_ZONE_EXT_SAFETY_MARGIN_HYS ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_ZONE_X ((float32_T)(-255.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_ZONE_Y ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_ZONE_Y_HYS ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_ZONE_Y_HYS_MAX ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_ZONE_Y_HYS_MIN ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_GAP_BRIDGE ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_CANDIDATE_TTC ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_TTC ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_TTC_HYS ((float32_T)(0.001f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_MAX_CURVI_HEADING_ABS ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_MIN_OBJ_CURVI_LONG_VEL ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_Y_WIDTH1 ((float32_T)(0.1f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_X_LENGTH1 ((float32_T)(-100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_Y1 ((float32_T)(0.3f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_X_LENGTH0 ((float32_T)(-50.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_Y_WIDTH0 ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_Y0 ((float32_T)(0.3f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_X0 ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_Y_WIDTH ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_Y0 ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_X_LENGTH ((float32_T)(-11.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_X0 ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LKA_OV_ZONE_WIDTH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_HONDA_EGO_SPEED_STOP_HOLDING ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_HONDA_BEEPER_ZONE_LENGTH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_HONDA_BEEPER_ZONE_WIDTH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_HONDA_ALERT_LEVEL_TWO_HOLDING_TIME ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_OBJ_MAX_REL_VEL_THRESH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_OBJ_MAX_REL_VEL_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_ZONE_FRONT_EGO_SIDE_X ((float32_T)(-10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_ZONE_FRONT_EGO_SIDE_Y ((float32_T)(-1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_ZONE_REAR_OUTER_SIDE_X ((float32_T)(-20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_ZONE_REAR_OUTER_SIDE_Y ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_ZONE_FRONT_EGO_SIDE_X_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_ZONE_FRONT_EGO_SIDE_Y_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_ZONE_REAR_OUTER_SIDE_X_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_ZONE_REAR_OUTER_SIDE_Y_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_PEDESTRIAN_MIN_SIZE ((float32_T)(0.01f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_PEDESTRIAN_MIN_SPEED ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_2WHEEL_MIN_SIZE ((float32_T)(0.01f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_2WHEEL_MIN_SPEED ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_OBJECT_POSITION_CORRECTION_DELAY_TIME ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_MIN_SPEED_FOR_TOS_SCENARIO ((float32_T)(-2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_MIN_OBJECT_CURVI_RELATIVE_SPEED ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_MAX_OBJECT_CURVI_RELATIVE_SPEED ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_OBJECT_CURVI_RELATIVE_SPEED_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_CVW_TTC_CONST ((float32_T)(0.1f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_CVW_TTC_ACCEL ((float32_T)(0.01f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_HONDA_NARROW_BEEPER_MAX_SPEED_H ((float32_T)(0.01f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_HONDA_NARROW_BEEPER_MAX_SPEED_L ((float32_T)(0.01f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_DYN_CVW_TTC_SPEED_PARAMETER ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_OBJECT_POSITION_CORRECTION_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_DYN_CVW_TTC_COMPENS_REL_VEL_THRESH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_DYN_CVW_TTC_COMPENS_TIME ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_DYNZONE_OBJECT_REL_VEL ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_DYNZONE_OBJECT_RANGE ((float32_T)(-255.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_F_DISABLE_DUE_TO_SMALL_CURVE_RADIUS ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_F_ENABLE_OBJ_IN_EGO_LANE_CHECK ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_F_ENABLE_ALERT_OBJ_IN_EGO_LANE ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_EGO_LANE_CHECK_CENTER_POINT_ONLY ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_USES_CVW_ALERT_STATE_ENABLED ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_OVERLAP_AREA_CHECK_ENABLE ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_USE_CURVI_COORDINATES ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_ENABLE_DYNSPEED_ZONE ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_ENABLE_TRAILER_ZONE_EXTENSION ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_ENABLE ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_ENABLE_VIA_CAL ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_HONDA_SRR6_ENABLE_ALERT_HOLD_DUE_SLOW_DOWN ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_HONDA_SRR6_ENABLE_ALERT_HOLD_DUE_OUT_OF_FOV ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_F_ENABLE_OBJ_REFLECTION_FLAG_CHECK ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_F_ENABLE_FALLBACK_HANDLER ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_F_ENABLE_DYN_CVW_TTC_THRESHOLD ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_SHRINK_ZONE_METHOD ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_F_USE_FRONT_ZONE_AS_N_LINE ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_F_ENABLE_OBJECT_REL_VEL_DYNZONE ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_ALERT_HOLDING_CYCLES ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_ALERT_HOLDING_CYCLES ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_ZONE_CHECK_METHOD ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_MIN_MATURE_CYCLES ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_LCDA_MIN_TRACK_AGE ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_BSW_ALERT_TRACK_AGE ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MIN_K_CVW_MIN_MATURE_CYCLES ((uint8_t)(0u)) 

/* Macros for maximum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_HOST_ACTIVATION_SPEED_MIN ((float32_T)(70.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_HOST_ACTIVATION_SPEED_MIN_HYS ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_HOST_ACTIVATION_SPEED_MAX ((float32_T)(150.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_HOST_ACTIVATION_SPEED_MAX_HYS ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_DISTANCE_TRAVELED_SCALE_FACTOR ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_MIN_CURVE_RADIUS ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_MIN_CURVE_RADIUS_HYS ((float32_T)(50.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_ZONE_INTERSECT_CRITICAL_POINT_LATERAL_RATIO ((float32_T)(1.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_LANE_CHANGE_INTENTION_VEL_LAT_THRESH ((float32_T)(5.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_LANE_CHANGE_INTENTION_POS_LAT_THRES ((float32_T)(1.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_LANE_CHANGE_INTENTION_POS_LONG_THRES ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_EGO_LANE_EFFECTIVE_LANE_WIDTH_FACTOR ((float32_T)(1.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_ZONE_X ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_ZONE_Y ((float32_T)(20.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_ZONE_X_HYS ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_ZONE_Y_HYS ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_ZONE_Y_HYS_MIN ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_ZONE_Y_HYS_MAX ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_FIXED_ZONE_X ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_FIXED_ZONE_Y ((float32_T)(20.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_FIXED_ZONE_X_HYS ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_FIXED_ZONE_Y_HYS ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_OVERLAP_AREA_THRESHOLD ((float32_T)(100.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_FALLBACK_REL_VEL_THRES ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_FALLBACK_REL_VEL_THRES_HYS ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_DYNZONE_SPEED ((float32_T)(120.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_DYNZONE_RANGE ((float32_T)(255.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_N_LINE_POSITION_FOR_LONG_OBJECT_SOT_SCENARIO ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_LINE_TO_STOP_TOS_ALERT ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_SUPPRESS_LATE_WARNING_MAX_TIME_TILL_LEAVE ((float32_T)(5.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_MAX_HEADING_ABS ((float32_T)(3.142f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_MAX_HEADING_ABS_HYSTERESIS ((float32_T)(0.5f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_MIN_OBJ_LONG_VEL ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_MAX_OBJ_LONG_VEL ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_MIN_OBJ_LONG_VEL_HYSTERESIS ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_MAX_OBJ_LONG_VEL_HYSTERESIS ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_TRAILER_ZONE_EXT_SAFETY_MARGIN ((float32_T)(100.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_TRAILER_ZONE_EXT_SAFETY_MARGIN_HYS ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_ZONE_X ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_ZONE_Y ((float32_T)(255.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_ZONE_Y_HYS ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_ZONE_Y_HYS_MAX ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_ZONE_Y_HYS_MIN ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_GAP_BRIDGE ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_CANDIDATE_TTC ((float32_T)(50.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_TTC ((float32_T)(25.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_TTC_HYS ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_MAX_CURVI_HEADING_ABS ((float32_T)(3.142f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_MIN_OBJ_CURVI_LONG_VEL ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_Y_WIDTH1 ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_X_LENGTH1 ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_Y1 ((float32_T)(3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_X_LENGTH0 ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_Y_WIDTH0 ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_Y0 ((float32_T)(1.5f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_X0 ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_Y_WIDTH ((float32_T)(3.8f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_Y0 ((float32_T)(1.8f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_X_LENGTH ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_X0 ((float32_T)(4.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LKA_OV_ZONE_WIDTH ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_HONDA_EGO_SPEED_STOP_HOLDING ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_HONDA_BEEPER_ZONE_LENGTH ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_HONDA_BEEPER_ZONE_WIDTH ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_HONDA_ALERT_LEVEL_TWO_HOLDING_TIME ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_OBJ_MAX_REL_VEL_THRESH ((float32_T)(55.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_OBJ_MAX_REL_VEL_HYS ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_ZONE_FRONT_EGO_SIDE_X ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_ZONE_FRONT_EGO_SIDE_Y ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_ZONE_REAR_OUTER_SIDE_X ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_ZONE_REAR_OUTER_SIDE_Y ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_ZONE_FRONT_EGO_SIDE_X_HYS ((float32_T)(3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_ZONE_FRONT_EGO_SIDE_Y_HYS ((float32_T)(3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_ZONE_REAR_OUTER_SIDE_X_HYS ((float32_T)(3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_ZONE_REAR_OUTER_SIDE_Y_HYS ((float32_T)(3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_PEDESTRIAN_MIN_SIZE ((float32_T)(5.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_PEDESTRIAN_MIN_SPEED ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_2WHEEL_MIN_SIZE ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_2WHEEL_MIN_SPEED ((float32_T)(25.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_OBJECT_POSITION_CORRECTION_DELAY_TIME ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_MIN_SPEED_FOR_TOS_SCENARIO ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_MIN_OBJECT_CURVI_RELATIVE_SPEED ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_MAX_OBJECT_CURVI_RELATIVE_SPEED ((float32_T)(1000.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_OBJECT_CURVI_RELATIVE_SPEED_HYS ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_CVW_TTC_CONST ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_CVW_TTC_ACCEL ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_HONDA_NARROW_BEEPER_MAX_SPEED_H ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_HONDA_NARROW_BEEPER_MAX_SPEED_L ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_DYN_CVW_TTC_SPEED_PARAMETER ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_OBJECT_POSITION_CORRECTION_THRESHOLD ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_DYN_CVW_TTC_COMPENS_REL_VEL_THRESH ((float32_T)(69.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_DYN_CVW_TTC_COMPENS_TIME ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_DYNZONE_OBJECT_REL_VEL ((float32_T)(120.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_DYNZONE_OBJECT_RANGE ((float32_T)(255.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_F_DISABLE_DUE_TO_SMALL_CURVE_RADIUS ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_F_ENABLE_OBJ_IN_EGO_LANE_CHECK ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_F_ENABLE_ALERT_OBJ_IN_EGO_LANE ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_EGO_LANE_CHECK_CENTER_POINT_ONLY ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_USES_CVW_ALERT_STATE_ENABLED ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_OVERLAP_AREA_CHECK_ENABLE ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_USE_CURVI_COORDINATES ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_ENABLE_DYNSPEED_ZONE ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_ENABLE_TRAILER_ZONE_EXTENSION ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_ENABLE ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_ENABLE_VIA_CAL ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_HONDA_SRR6_ENABLE_ALERT_HOLD_DUE_SLOW_DOWN ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_HONDA_SRR6_ENABLE_ALERT_HOLD_DUE_OUT_OF_FOV ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_F_ENABLE_OBJ_REFLECTION_FLAG_CHECK ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_F_ENABLE_FALLBACK_HANDLER ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_F_ENABLE_DYN_CVW_TTC_THRESHOLD ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_SHRINK_ZONE_METHOD ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_F_USE_FRONT_ZONE_AS_N_LINE ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_F_ENABLE_OBJECT_REL_VEL_DYNZONE ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_ALERT_HOLDING_CYCLES ((uint8_t)(20u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_ALERT_HOLDING_CYCLES ((uint8_t)(20u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_ZONE_CHECK_METHOD ((uint8_t)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_MIN_MATURE_CYCLES ((uint8_t)(20u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_LCDA_MIN_TRACK_AGE ((uint8_t)(100u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_BSW_ALERT_TRACK_AGE ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_MAX_K_CVW_MIN_MATURE_CYCLES ((uint8_t)(20u)) 


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
void Lcda_Public_Cal_Reverse_Array_Lcda_Cal(Lcda_Public_Calibration_T* cal_dst);
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
void Lcda_Public_Cal_Print(FILE* c_file_ptr, const Lcda_Public_Calibration_T* p_cals);

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
void Lcda_Public_Cal_Update_Defaults(Lcda_Public_Calibration_T* cal_dst);



#endif /* LCDA_PUBLIC_CALIBRATION_H */
