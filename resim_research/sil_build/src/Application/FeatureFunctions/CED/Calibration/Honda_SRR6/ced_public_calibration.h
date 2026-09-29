# ifndef CED_PUBLIC_CALIBRATION_H
# define CED_PUBLIC_CALIBRATION_H

/**
* @file ced_public_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in ced_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "ced_public_calibration_t.h"
#include "pa_reuse.h" // IWYU pragma: keep
#ifdef CT_ACTIVATE_CAL_PRINT
#include <stdio.h>
#endif

/*===========================================================================*\
* Defines
\*===========================================================================*/

/* Macros for minimum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_ACCELERATION_WEIGHT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_HEADING_PREDICTED_WEIGHT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_EXISTENCE_PROBABILITY_MIN ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_HEADING_ABS_ANGLE_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_LONG_VEL_REL_MIN ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_LONG_VEL_MIN ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_LAT_VEL_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_MAX_WIDTH_INCREASE_FACTOR_WITH_PATH_MATCH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_MAX_WIDTH_INCREASE_FACTOR_WITHOUT_PATH_MATCH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_HEADING_EXP_MOVING_AVERAGE_ALPHA ((float32_T)(0.00f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_FTM_EXISTENCE_PROBABILITY_MIN ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_FTM_HEADING_ABS_ANGLE_MIN ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_FTM_LONG_VEL_REL_MIN ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_FTM_LONG_VEL_MIN ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_FTM_LAT_VEL_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_FIRST_WARNING_TTC_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_SECOND_WARNING_TTC_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_THIRD_WARNING_TTC_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_SECOND_WARNING_PRED_LAT_DIST_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_THIRD_WARNING_PRED_LAT_DIST_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_ALERT_TTP_MIN ((float32_T)(-1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_EGO_ABS_SPEED_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_CRASH_LINE_HOST_LENGTH_PERCENTAGE ((float32_T)(0.00f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_WIDTH_SAFETY_MARGIN_FOR_ACTIVE_ALERT ((float32_T)(0.00f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_WIDTH_SAFETY_MARGIN_FOR_CRITICAL_PATH_MATCH ((float32_T)(0.00f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_MIN_DIST_TO_CRASH_LINE_FOR_PATH_MATCH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OFFSET_TO_PATH_WEIGHT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_COLLISION_ZONE_WIDTH ((float32_T)(0.5f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_FUNNEL_ZONE_LENGTH ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_FUNNEL_ZONE_WIDTH ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_SLOW_OBJECTS_LONG_VEL_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_EGO_LANE_WIDTH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_EGO_LANE_PARKING_RANGE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_EGO_LANE_PARKING_MANEUVER_SPEED ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_ALERT_HOLDING_OBJ_ABS_HEADING_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_ALERT_HOLDING_OBJ_LONG_VEL_MIN ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_SUPPRESS_PT_HEADING_DIFF_CED_ALERT_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_SUPPRESS_RANGE_TO_NEAREST_PATH_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_LONG_VEL_REL_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_FTM_LONG_VEL_REL_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_VEL_MAX ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_SLIGHT_TURN_LAT_VEL_TABLE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_SLIGHT_TURN_POSITION_LIMITS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_LAT_POS_OF_BORDER ((float32_T)(-2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_LAT_POS_MAX_SHIFT ((float32_T)(-5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_LAT_POS_SHIFT_LONG_DIST_THRESHOLDS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_LAT_POS_SHIFT_LAT_DIST_THRESHOLDS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_BMW_CED_SPEED_MAX_HYSTERESIS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_PREDICTED_MAX_WIDTH_SLOPE_REDUCE_FACTOR ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_PREDICTED_MAX_WIDTH_SLOPE_OFFSET ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_LAT_POS_SHIFT_WIDTH_THRESH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_WARNING_PRED_LAT_DIST_MAX_HISTERESIS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_F_ENABLE_HEADING_EXP_MOVING_AVERAGE ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_F_SECOND_WARNING_LEVEL_ENABLE ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_F_THIRD_WARNING_LEVEL_ENABLE ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_F_SUPPRESS_ALERT_HOLDING_FOR_UNCRITICAL_OBJECTS ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_F_SUPPRESS_ALERT_HOLDING_FOR_OBJ_BELOW_MIN_TTP ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_F_PATH_TRACKING_ENABLE ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_F_HANDLE_BOTH_SIDE_ALERTS_AS_OBJECT_SIDE ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_F_ALLOW_EGO_LANE_ALERTS ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_F_USE_ONLY_MATURE_PATHS ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_F_ALLOW_COASTED_OBJECT_ALERTS ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_F_ADAPT_HEADING_EGO_LANE ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_F_OBJECT_LAT_ON_ONE_SIDE_OF_BORDER ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_LAT_POS_SHIFT_ENABLE ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_ALERT_QUALIFYING_CYCLES ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_ALERT_QUALIFYING_CYCLES_SLOW_OBJECTS ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_ALERT_HOLDING_CYCLES ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_ALLOW_OPPOSITE_SIDE_ALERTS ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_SUPPRESS_ALERT_OBJECT_AGE_MAX ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_MIN_CYCLES_FOR_PATH_MATCH_FOR_NO_SUPPRESS ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_AGE_MIN ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_OBJECT_FTM_AGE_MIN ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_F_CHOOSE_REF_POINT_FUNNEL_CHECK ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_HYSTERESIS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_HONDA_SRR6_LONG_DIST_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_HONDA_MIN_ALERT_DURATION ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_HONDA_ELATCH_ZONES_WIDTH_TABLE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_HONDA_MIN_ERATCH_ALERT_DURATION ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_HONDA_OBJECT_ACCELERATION_WEIGHT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MIN_K_CED_F_HONDA_USE_ALERT_TTC_THRESHOLD ((boolean_T)(0u)) 

/* Macros for maximum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_ACCELERATION_WEIGHT ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_HEADING_PREDICTED_WEIGHT ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_EXISTENCE_PROBABILITY_MIN ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_HEADING_ABS_ANGLE_MAX ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_LONG_VEL_REL_MIN ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_LONG_VEL_MIN ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_LAT_VEL_MAX ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_MAX_WIDTH_INCREASE_FACTOR_WITH_PATH_MATCH ((float32_T)(3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_MAX_WIDTH_INCREASE_FACTOR_WITHOUT_PATH_MATCH ((float32_T)(3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_HEADING_EXP_MOVING_AVERAGE_ALPHA ((float32_T)(1.00f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_FTM_EXISTENCE_PROBABILITY_MIN ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_FTM_HEADING_ABS_ANGLE_MIN ((float32_T)(3.14f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_FTM_LONG_VEL_REL_MIN ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_FTM_LONG_VEL_MIN ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_FTM_LAT_VEL_MAX ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_FIRST_WARNING_TTC_THRESHOLD ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_SECOND_WARNING_TTC_THRESHOLD ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_THIRD_WARNING_TTC_THRESHOLD ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_SECOND_WARNING_PRED_LAT_DIST_MAX ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_THIRD_WARNING_PRED_LAT_DIST_MAX ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_ALERT_TTP_MIN ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_EGO_ABS_SPEED_MAX ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_CRASH_LINE_HOST_LENGTH_PERCENTAGE ((float32_T)(1.00f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_WIDTH_SAFETY_MARGIN_FOR_ACTIVE_ALERT ((float32_T)(1.00f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_WIDTH_SAFETY_MARGIN_FOR_CRITICAL_PATH_MATCH ((float32_T)(1.00f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_MIN_DIST_TO_CRASH_LINE_FOR_PATH_MATCH ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OFFSET_TO_PATH_WEIGHT ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_COLLISION_ZONE_WIDTH ((float32_T)(3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_FUNNEL_ZONE_LENGTH ((float32_T)(80.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_FUNNEL_ZONE_WIDTH ((float32_T)(40.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_SLOW_OBJECTS_LONG_VEL_MAX ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_EGO_LANE_WIDTH ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_EGO_LANE_PARKING_RANGE ((float32_T)(50.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_EGO_LANE_PARKING_MANEUVER_SPEED ((float32_T)(13.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_ALERT_HOLDING_OBJ_ABS_HEADING_MAX ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_ALERT_HOLDING_OBJ_LONG_VEL_MIN ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_SUPPRESS_PT_HEADING_DIFF_CED_ALERT_MAX ((float32_T)(0.5f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_SUPPRESS_RANGE_TO_NEAREST_PATH_MAX ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_LONG_VEL_REL_MAX ((float32_T)(80.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_FTM_LONG_VEL_REL_MAX ((float32_T)(80.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_VEL_MAX ((float32_T)(80.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_SLIGHT_TURN_LAT_VEL_TABLE ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_SLIGHT_TURN_POSITION_LIMITS ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_LAT_POS_OF_BORDER ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_LAT_POS_MAX_SHIFT ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_LAT_POS_SHIFT_LONG_DIST_THRESHOLDS ((float32_T)(1000.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_LAT_POS_SHIFT_LAT_DIST_THRESHOLDS ((float32_T)(1000.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_BMW_CED_SPEED_MAX_HYSTERESIS ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_PREDICTED_MAX_WIDTH_SLOPE_REDUCE_FACTOR ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_PREDICTED_MAX_WIDTH_SLOPE_OFFSET ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_LAT_POS_SHIFT_WIDTH_THRESH ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_WARNING_PRED_LAT_DIST_MAX_HISTERESIS ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_F_ENABLE_HEADING_EXP_MOVING_AVERAGE ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_F_SECOND_WARNING_LEVEL_ENABLE ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_F_THIRD_WARNING_LEVEL_ENABLE ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_F_SUPPRESS_ALERT_HOLDING_FOR_UNCRITICAL_OBJECTS ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_F_SUPPRESS_ALERT_HOLDING_FOR_OBJ_BELOW_MIN_TTP ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_F_PATH_TRACKING_ENABLE ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_F_HANDLE_BOTH_SIDE_ALERTS_AS_OBJECT_SIDE ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_F_ALLOW_EGO_LANE_ALERTS ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_F_USE_ONLY_MATURE_PATHS ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_F_ALLOW_COASTED_OBJECT_ALERTS ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_F_ADAPT_HEADING_EGO_LANE ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_F_OBJECT_LAT_ON_ONE_SIDE_OF_BORDER ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_LAT_POS_SHIFT_ENABLE ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_ALERT_QUALIFYING_CYCLES ((uint8_t)(10u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_ALERT_QUALIFYING_CYCLES_SLOW_OBJECTS ((uint8_t)(10u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_ALERT_HOLDING_CYCLES ((uint8_t)(50u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_ALLOW_OPPOSITE_SIDE_ALERTS ((uint8_t)(2u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_SUPPRESS_ALERT_OBJECT_AGE_MAX ((uint8_t)(25u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_MIN_CYCLES_FOR_PATH_MATCH_FOR_NO_SUPPRESS ((uint8_t)(10u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_AGE_MIN ((uint8_t)(5u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_OBJECT_FTM_AGE_MIN ((uint8_t)(5u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_F_CHOOSE_REF_POINT_FUNNEL_CHECK ((uint8_t)(2u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_THRESHOLD ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_HONDA_SRR6_CUSTOM_TTC_ALERT_HYSTERESIS ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_HONDA_SRR6_LONG_DIST_THRESHOLD ((float32_T)(40.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_HONDA_MIN_ALERT_DURATION ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_HONDA_ELATCH_ZONES_WIDTH_TABLE ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_HONDA_MIN_ERATCH_ALERT_DURATION ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_HONDA_OBJECT_ACCELERATION_WEIGHT ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define CED_MAX_K_CED_F_HONDA_USE_ALERT_TTC_THRESHOLD ((boolean_T)(1u)) 


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
void Ced_Public_Cal_Reverse_Array_Ced_Cal(Ced_Public_Calibration_T* cal_dst);
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
void Ced_Public_Cal_Print(FILE* c_file_ptr, const Ced_Public_Calibration_T* p_cals);

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
void Ced_Public_Cal_Update_Defaults(Ced_Public_Calibration_T* cal_dst);



#endif /* CED_PUBLIC_CALIBRATION_H */
