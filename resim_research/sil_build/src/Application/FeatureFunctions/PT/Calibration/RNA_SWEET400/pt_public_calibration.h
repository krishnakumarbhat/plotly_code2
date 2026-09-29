# ifndef PT_PUBLIC_CALIBRATION_H
# define PT_PUBLIC_CALIBRATION_H

/**
* @file pt_public_calibration.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in pt_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "pt_public_calibration_t.h"
#include "pa_reuse.h" // IWYU pragma: keep
#ifdef CT_ACTIVATE_CAL_PRINT
#include <stdio.h>
#endif

/*===========================================================================*\
* Defines
\*===========================================================================*/

/* Macros for minimum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_ZONE_MAX_POSN ((float32_T)(50.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_MIN_OBJ_SPEED ((float32_T)(1.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_OVERLAP_MAX_MATCH_VALUE ((float32_T)(15.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_MOVE_MAX_VALUE ((float32_T)(15.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_GROUP_MAX_MATCH_VALUE ((float32_T)(1.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_GROUP_MAX_MATCH_VALUE_AD ((float32_T)(0.5f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_GROUP_DIR_MAX_DIFF_VALUE ((float32_T)(0.5f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_GROUP_DIR_MAX_AVG_DIFF_VALUE ((float32_T)(1.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_FIND_MAX_LAT_POSN ((float32_T)(30.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_FIND_MIN_SPEED ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_FIND_MAX_LONG_POSN ((float32_T)(30.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_PATH_CHANGE_MATCH_HYST_DEFAULT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_PATH_CHANGE_DIFFERING_STATES_HYST_DEFAULT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_PATH_CHANGE_MATCH_HYST_MORE_ESTABLISHED ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_PATH_CHANGE_ONE_GROUPED_ONE_MATURE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_PATH_CHANGE_ONE_GROUPED_ONE_CREATION ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_KILL_PATH_EXCEED_DIST_THRES ((float32_T)(5.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_KILL_PATH_MAX_DIFF_POSN ((float32_T)(1.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_LOWER_LIM_OBJ_ORIENT_LAT ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_UPPER_LIM_OBJ_ORIENT_LAT ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_APPLY_MOVE_POINT_MIN_SPEED ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_APPLY_MOVE_POINT_MIN_YAW_RATE ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_MOVE_POINT_YAW_RATE_THRES_CALC_EGO_SHIFT ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_GROUP_PATHS_MIN_INTERVAL_DIST ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_PATH_TRACK_LONG_RANGE_LIMIT ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_PATH_TRACK_LAT_RANGE_LIMIT ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_GROUP_OVERLAP_PATHS_MIN_DIFF ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_EN_ALGO_MIN_VEL_INACTIVE ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_EN_ALGO_MAX_VAL_ACTIVE ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_POINT_DIFF_WEIGHTING_FACTOR_LUT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_DEFAULT_RANGE_OF_TRACKING_ZONE ((float32_T)(20.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_MIN_EXIST_PROB_TO_BE_VALID ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_DIST_OBJ_TO_BORDER_CONF_LUT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_DIST_BORDER_TO_ISECT_CONF_LUT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_DIST_OBJ_TO_PATH_LUT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_DIST_OBJ_TO_PATH_CONF_LUT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_SIMILARITY_TRAIL_PATH_LUT ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_SIMILARITY_TRAIL_PATH_CONF_LUT ((float32_T)(0.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_WEIGHT_OF_LAST_TRAIL_POINT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_WEIGHT_OF_SEC_LAST_TRAIL_POINT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_HEADING_DIFF_LUT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_HEADING_DIFF_CONF_LUT ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_WEIGHT_HEADING_DIFF_CONFIDENCE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_MIN_CONFIDENCE_VALID_MATCH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_DIST_BETW_PATHS_SIMILARITY_MATCHING ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_HOST_IMPLAUSIBILTY_RANGE ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_TRAIL_MAX_SPEED_TRAIL_TO_PATH_CONV ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_MINIMUM_HOST_TRAIL_LENGTH ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_MAX_HEADING_DIFF_VALID_INTERVAL ((float32_T)(0.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_F_APPLY_MOVE_POINT ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_F_CHECK_OBJECT_AGE_PLAUSIBILITY ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_ENABLE_HOST_TRAIL ((boolean_T)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_MAX_DIFF_NUM_PATH_POINT ((uint8_t)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_GROUP_PATH_MIN_OVERLAP_COUNT ((uint8_t)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_GROUP_PATH_MIN_OVERLAP_COUNT_AD ((uint8_t)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_FIND_MIN_DIFF_PATH_POINTS ((uint8_t)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_FIND_MAX_DIFF_PATH_POINTS ((uint8_t)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_KILL_LANE_CHANGE_MIN_DIFF_PATH_POINT ((uint8_t)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_MIN_DIFF_NUM_PATH_POINTS ((uint8_t)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_MIN_PATH_LENGTH_PROC_LANE_CHANGE ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_POINT_DIFF_GROUPING_BORDERS_LUT ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_COND_KILL_IMPLAUS_PATH ((uint8_t)(2u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_MIN_PATH_LENGTH_AFTER_ROT ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_DIST_OBJ_TO_BORDER_LUT ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_DIST_BORDER_TO_ISECT_LUT ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_MIN_PATH_LENGTH_OBJ_TRAIL ((uint8_t)(2u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_RANGE_NEAREST_BORDER_IMPL_PATH ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_START_OF_LANE_CHANGE_PROCESSING ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_END_OF_LANE_CHANGE_PROCESSING ((uint8_t)(0u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MIN_K_PT_MINIMUM_AMOUNT_OF_TRAIL_POINTS ((uint8_t)(1u)) 

/* Macros for maximum range of calibrations */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_ZONE_MAX_POSN ((float32_T)(300.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_MIN_OBJ_SPEED ((float32_T)(20.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_OVERLAP_MAX_MATCH_VALUE ((float32_T)(50.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_MOVE_MAX_VALUE ((float32_T)(50.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_GROUP_MAX_MATCH_VALUE ((float32_T)(5.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_GROUP_MAX_MATCH_VALUE_AD ((float32_T)(5.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_GROUP_DIR_MAX_DIFF_VALUE ((float32_T)(5.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_GROUP_DIR_MAX_AVG_DIFF_VALUE ((float32_T)(5.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_FIND_MAX_LAT_POSN ((float32_T)(70.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_FIND_MIN_SPEED ((float32_T)(30.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_FIND_MAX_LONG_POSN ((float32_T)(70.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_PATH_CHANGE_MATCH_HYST_DEFAULT ((float32_T)(0.15f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_PATH_CHANGE_DIFFERING_STATES_HYST_DEFAULT ((float32_T)(0.2f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_PATH_CHANGE_MATCH_HYST_MORE_ESTABLISHED ((float32_T)(0.35f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_PATH_CHANGE_ONE_GROUPED_ONE_MATURE ((float32_T)(0.35f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_PATH_CHANGE_ONE_GROUPED_ONE_CREATION ((float32_T)(0.45f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_KILL_PATH_EXCEED_DIST_THRES ((float32_T)(30.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_KILL_PATH_MAX_DIFF_POSN ((float32_T)(5.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_LOWER_LIM_OBJ_ORIENT_LAT ((float32_T)(3.1459f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_UPPER_LIM_OBJ_ORIENT_LAT ((float32_T)(3.1459f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_APPLY_MOVE_POINT_MIN_SPEED ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_APPLY_MOVE_POINT_MIN_YAW_RATE ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_MOVE_POINT_YAW_RATE_THRES_CALC_EGO_SHIFT ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_GROUP_PATHS_MIN_INTERVAL_DIST ((float32_T)(10.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_PATH_TRACK_LONG_RANGE_LIMIT ((float32_T)(100.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_PATH_TRACK_LAT_RANGE_LIMIT ((float32_T)(100.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_GROUP_OVERLAP_PATHS_MIN_DIFF ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_EN_ALGO_MIN_VEL_INACTIVE ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_EN_ALGO_MAX_VAL_ACTIVE ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_POINT_DIFF_WEIGHTING_FACTOR_LUT ((float32_T)(5.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_DEFAULT_RANGE_OF_TRACKING_ZONE ((float32_T)(80.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_MIN_EXIST_PROB_TO_BE_VALID ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_DIST_OBJ_TO_BORDER_CONF_LUT ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_DIST_BORDER_TO_ISECT_CONF_LUT ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_DIST_OBJ_TO_PATH_LUT ((float32_T)(6.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_DIST_OBJ_TO_PATH_CONF_LUT ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_SIMILARITY_TRAIL_PATH_LUT ((float32_T)(6.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_SIMILARITY_TRAIL_PATH_CONF_LUT ((float32_T)(6.f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_WEIGHT_OF_LAST_TRAIL_POINT ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_WEIGHT_OF_SEC_LAST_TRAIL_POINT ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_HEADING_DIFF_LUT ((float32_T)(3.14f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_HEADING_DIFF_CONF_LUT ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_WEIGHT_HEADING_DIFF_CONFIDENCE ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_MIN_CONFIDENCE_VALID_MATCH ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_DIST_BETW_PATHS_SIMILARITY_MATCHING ((float32_T)(0.45f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_HOST_IMPLAUSIBILTY_RANGE ((float32_T)(1.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_TRAIL_MAX_SPEED_TRAIL_TO_PATH_CONV ((float32_T)(10.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_MINIMUM_HOST_TRAIL_LENGTH ((float32_T)(30.0f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_MAX_HEADING_DIFF_VALID_INTERVAL ((float32_T)(0.5f)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_F_APPLY_MOVE_POINT ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_F_CHECK_OBJECT_AGE_PLAUSIBILITY ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_ENABLE_HOST_TRAIL ((boolean_T)(1u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_MAX_DIFF_NUM_PATH_POINT ((uint8_t)(10u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_GROUP_PATH_MIN_OVERLAP_COUNT ((uint8_t)(10u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_GROUP_PATH_MIN_OVERLAP_COUNT_AD ((uint8_t)(10u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_FIND_MIN_DIFF_PATH_POINTS ((uint8_t)(10u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_FIND_MAX_DIFF_PATH_POINTS ((uint8_t)(10u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_KILL_LANE_CHANGE_MIN_DIFF_PATH_POINT ((uint8_t)(24u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_MIN_DIFF_NUM_PATH_POINTS ((uint8_t)(63u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_MIN_PATH_LENGTH_PROC_LANE_CHANGE ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_POINT_DIFF_GROUPING_BORDERS_LUT ((uint8_t)(5u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_COND_KILL_IMPLAUS_PATH ((uint8_t)(10u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_MIN_PATH_LENGTH_AFTER_ROT ((uint8_t)(4u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_DIST_OBJ_TO_BORDER_LUT ((uint8_t)(5u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_DIST_BORDER_TO_ISECT_LUT ((uint8_t)(5u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_MIN_PATH_LENGTH_OBJ_TRAIL ((uint8_t)(2u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_RANGE_NEAREST_BORDER_IMPL_PATH ((uint8_t)(7u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_START_OF_LANE_CHANGE_PROCESSING ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_END_OF_LANE_CHANGE_PROCESSING ((uint8_t)(255u)) 
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define PT_MAX_K_PT_MINIMUM_AMOUNT_OF_TRAIL_POINTS ((uint8_t)(10u)) 


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
void Pt_Public_Cal_Reverse_Array_Pt_Cal(Pt_Public_Calibration_T* cal_dst);
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
void Pt_Public_Cal_Print(FILE* c_file_ptr, const Pt_Public_Calibration_T* p_cals);

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
void Pt_Public_Cal_Update_Defaults(Pt_Public_Calibration_T* cal_dst);



#endif /* PT_PUBLIC_CALIBRATION_H */
