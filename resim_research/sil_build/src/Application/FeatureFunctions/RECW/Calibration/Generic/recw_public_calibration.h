# ifndef RECW_PUBLIC_CALIBRATION_H
# define RECW_PUBLIC_CALIBRATION_H

/**
* @file recw_public_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in recw_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "recw_public_calibration_t.h"
#include "pa_reuse.h" // IWYU pragma: keep
#ifdef CT_ACTIVATE_CAL_PRINT
#include <stdio.h>
#endif

/*===========================================================================*\
* Defines
\*===========================================================================*/

/* Macros for minimum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MIN_REL_VELOCITY ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MIN_REL_VELOCITY_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MAX_HEADING ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MAX_HEADING_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_FACTOR_EGO_WIDTH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_AVERAGE_SENSOR_LATENCY ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MIN_CRASH_PROB ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MIN_EXISTENCE_PROB ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MIN_EXISTENCE_PROB_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MIN_TTC_FOR_ALERT_LEVEL ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MIN_OVERLAP_FOR_ALERT_LEVEL ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MIN_HOST_SPEED ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MIN_HOST_SPEED_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MAX_HOST_SPEED ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MAX_HOST_SPEED_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MIN_REL_VELOCITY_FOR_MAX_TTC_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MAX_TTC_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MIN_DIST_FOR_YOUNG_SLOW_TARGETS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MIN_ABS_SPEED_FOR_YOUNG_CLOSE_TARGETS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MAX_REL_VELOCITY ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MAX_REL_VELOCITY_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MAX_REL_LON_VEL_RELEASE_CAR_WASH ((float32_T)(-70.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MAX_LON_DISTANCE_CAR_WASH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MAX_LAT_DISTANCE_CAR_WASH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MIN_REL_LON_VEL_CAR_WASH ((float32_T)(-20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MAX_SPEED_EGO_CAR_WASH ((float32_T)(-20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_LANE_FILTER_WIDTH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_LANE_FILTER_WIDTH_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_LANE_FILTER_MAX_ABS_EGO_SPEED_VCS_COORD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_LANE_WIDTH_SLOPE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_LOOKUP_BRAKING_DECELERATION ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_LOOKUP_BRAKING_PROBABILITY ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_LOOKUP_STEERING_ACCELERATION ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_LOOKUP_STEERING_PROBABILITY ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MAX_ECLIPSE_VALUE_FOR_VALID_OBJECT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MAX_OBJECT_WIDTH_WARN_ON ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MAX_ALLOWED_REL_VEL_LONG_DIFF ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MAX_ALLOWED_REL_VEL_LAT_DIFF ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MAX_ALLOWED_HEADING_DIFF ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_REAR_BLOCKAGE_SPEED_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_REAR_BLOCKAGE_WIDTH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_REAR_BLOCKAGE_LENGTH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_REAR_BLOCKAGE_EGO_SPEED_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_HEADING_ACCURACY_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MIN_SPEED_NOT_STATIONARY ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECB_MAX_HOST_SPEED ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECB_MAX_HOST_SPEED_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECB_MAX_VELOCITY_HOST_STANDSTILL ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECB_MIN_REL_VELOCITY ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECB_MIN_REL_VELOCITY_HYS ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECB_MAX_TTC ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECB_NOMINAL_ACCELERATION_APPLIED ((float32_T)(-20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECB_ACCELERATOR_PEDAL_GRADIENT_THRESHOLD ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_F_MAKE_USE_OF_GUARDRAIL ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_F_ONLY_ALLOW_CONSECUTIVE_ALERT_LEVELS ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_F_ALLOW_ALERT_ON_COASTED_OBJECTS ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_F_USE_REAR_BLOCKAGE ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_F_APPLY_LANE_FILTER ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_F_ENABLE_HEADING_FILTER ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_F_ENABLE_TRAFFIC_LIGHT_GHOST_DETECTION ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECB_F_ENABLE_RECB ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_ALERT_HOLDING_CYCLES ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_ALERT_QUALIFYING_CYCLES ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MIN_STAGE_AGE_FOR_ALERT_LEVEL ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MAX_CYCLES_ALERT_DURATION ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MIN_AGE_FOR_CLOSE_SLOW_TARGETS ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MIN_OBJECT_AGE ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MIN_CYCLES_WITH_MIN_CRASH_PROB ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_EN_ACTIVE_CAR_WASH_LOGIC ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_LANE_FILTER_NUM_CONSECUTIVE_CYCLES ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_MAX_ALLOWED_CONSECUTIVE_COASTED_CYCLES ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_REAR_BLOCKAGE_QUALIFYING_CYCLES ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECB_MIN_CYCLES_HOST_STANDSTILL ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECB_SSM_BRAKING ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECB_SSM_REQUEST_CANCELLED ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECB_INTEGRITY ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECB_QUALIFIER_NOMINAL_ACCELERATION ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECB_MAX_CYCLES_BRAKING_REQUEST_DURATION ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MIN_K_RECW_F_SUPPRESS_ALERT_LVL_2_FOR_PEDESTRIAN ((uint8_t)(0u)) 

/* Macros for maximum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MIN_REL_VELOCITY ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MIN_REL_VELOCITY_HYS ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MAX_HEADING ((float32_T)(1.5f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MAX_HEADING_HYS ((float32_T)(0.3f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_FACTOR_EGO_WIDTH ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_AVERAGE_SENSOR_LATENCY ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MIN_CRASH_PROB ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MIN_EXISTENCE_PROB ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MIN_EXISTENCE_PROB_HYS ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MIN_TTC_FOR_ALERT_LEVEL ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MIN_OVERLAP_FOR_ALERT_LEVEL ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MIN_HOST_SPEED ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MIN_HOST_SPEED_HYS ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MAX_HOST_SPEED ((float32_T)(80.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MAX_HOST_SPEED_HYS ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MIN_REL_VELOCITY_FOR_MAX_TTC_THRESHOLD ((float32_T)(30.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MAX_TTC_THRESHOLD ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MIN_DIST_FOR_YOUNG_SLOW_TARGETS ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MIN_ABS_SPEED_FOR_YOUNG_CLOSE_TARGETS ((float32_T)(60.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MAX_REL_VELOCITY ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MAX_REL_VELOCITY_HYS ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MAX_REL_LON_VEL_RELEASE_CAR_WASH ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MAX_LON_DISTANCE_CAR_WASH ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MAX_LAT_DISTANCE_CAR_WASH ((float32_T)(100.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MIN_REL_LON_VEL_CAR_WASH ((float32_T)(70.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MAX_SPEED_EGO_CAR_WASH ((float32_T)(70.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_LANE_FILTER_WIDTH ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_LANE_FILTER_WIDTH_HYS ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_LANE_FILTER_MAX_ABS_EGO_SPEED_VCS_COORD ((float32_T)(3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_LANE_WIDTH_SLOPE ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_LOOKUP_BRAKING_DECELERATION ((float32_T)(13.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_LOOKUP_BRAKING_PROBABILITY ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_LOOKUP_STEERING_ACCELERATION ((float32_T)(13.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_LOOKUP_STEERING_PROBABILITY ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MAX_ECLIPSE_VALUE_FOR_VALID_OBJECT ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MAX_OBJECT_WIDTH_WARN_ON ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MAX_ALLOWED_REL_VEL_LONG_DIFF ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MAX_ALLOWED_REL_VEL_LAT_DIFF ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MAX_ALLOWED_HEADING_DIFF ((float32_T)(3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_REAR_BLOCKAGE_SPEED_THRESHOLD ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_REAR_BLOCKAGE_WIDTH ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_REAR_BLOCKAGE_LENGTH ((float32_T)(20.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_REAR_BLOCKAGE_EGO_SPEED_THRESHOLD ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_HEADING_ACCURACY_THRESHOLD ((float32_T)(0.5f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MIN_SPEED_NOT_STATIONARY ((float32_T)(3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECB_MAX_HOST_SPEED ((float32_T)(5.5f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECB_MAX_HOST_SPEED_HYS ((float32_T)(2.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECB_MAX_VELOCITY_HOST_STANDSTILL ((float32_T)(3.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECB_MIN_REL_VELOCITY ((float32_T)(70.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECB_MIN_REL_VELOCITY_HYS ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECB_MAX_TTC ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECB_NOMINAL_ACCELERATION_APPLIED ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECB_ACCELERATOR_PEDAL_GRADIENT_THRESHOLD ((float32_T)(1000.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_F_MAKE_USE_OF_GUARDRAIL ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_F_ONLY_ALLOW_CONSECUTIVE_ALERT_LEVELS ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_F_ALLOW_ALERT_ON_COASTED_OBJECTS ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_F_USE_REAR_BLOCKAGE ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_F_APPLY_LANE_FILTER ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_F_ENABLE_HEADING_FILTER ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_F_ENABLE_TRAFFIC_LIGHT_GHOST_DETECTION ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECB_F_ENABLE_RECB ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_ALERT_HOLDING_CYCLES ((uint8_t)(100u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_ALERT_QUALIFYING_CYCLES ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MIN_STAGE_AGE_FOR_ALERT_LEVEL ((uint8_t)(10u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MAX_CYCLES_ALERT_DURATION ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MIN_AGE_FOR_CLOSE_SLOW_TARGETS ((uint8_t)(100u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MIN_OBJECT_AGE ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MIN_CYCLES_WITH_MIN_CRASH_PROB ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_EN_ACTIVE_CAR_WASH_LOGIC ((uint8_t)(2u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_LANE_FILTER_NUM_CONSECUTIVE_CYCLES ((uint8_t)(20u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_MAX_ALLOWED_CONSECUTIVE_COASTED_CYCLES ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_REAR_BLOCKAGE_QUALIFYING_CYCLES ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECB_MIN_CYCLES_HOST_STANDSTILL ((uint8_t)(10u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECB_SSM_BRAKING ((uint8_t)(7u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECB_SSM_REQUEST_CANCELLED ((uint8_t)(7u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECB_INTEGRITY ((uint8_t)(2u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECB_QUALIFIER_NOMINAL_ACCELERATION ((uint8_t)(6u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECB_MAX_CYCLES_BRAKING_REQUEST_DURATION ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define RECW_MAX_K_RECW_F_SUPPRESS_ALERT_LVL_2_FOR_PEDESTRIAN ((uint8_t)(1u)) 


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
void Recw_Public_Cal_Reverse_Array_Recw_Cal(Recw_Public_Calibration_T* cal_dst);
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
void Recw_Public_Cal_Print(FILE* c_file_ptr, const Recw_Public_Calibration_T* p_cals);

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
void Recw_Public_Cal_Update_Defaults(Recw_Public_Calibration_T* cal_dst);



#endif /* RECW_PUBLIC_CALIBRATION_H */
