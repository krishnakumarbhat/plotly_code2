# ifndef TA_CORE_CALIBRATION_T_H
# define TA_CORE_CALIBRATION_T_H

/**
* @file ta_core_calibration_t.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in ta_cal.xml.
* This file is auto-generated with SFL calibration tool v5.0.3 and shall not be edited manually.
*
* @copyright Copyright (C) 2025 Aptiv. All rights reserved.
*/

/*===========================================================================*\
* Includes
\*===========================================================================*/
#include "ct_calibration_header_t.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Defines
\*===========================================================================*/

/* Macros for all calibrations */
/* Macros for array sizes for all array variables */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_TA_ALERT_LVL_1_TTP_THRESHOLD_ARRAY_SIZE_DIM0 (3u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_TA_EGO_SPEED_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_TA_EGO_SPEED_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_TA_EGO_YAWRATE_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_TA_EGO_YAWRATE_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_TA_EGO_LONG_ACCELERATION_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_TA_EGO_LONG_ACCELERATION_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_EXIST_PRBLTY_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_EXIST_PRBLTY_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_VCS_LONG_VEL_REL_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_VCS_LONG_VEL_REL_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_VCS_LAT_VEL_REL_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_VCS_LAT_VEL_REL_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_VCS_LONG_VEL_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_VCS_LONG_VEL_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_VCS_LAT_VEL_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_VCS_LAT_VEL_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_TA_LOOKUP_TURNING_HOST_SPEED_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_HEADING_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_HEADING_STRAIGHT_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_HEADING_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_HEADING_RATE_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_HEADING_RATE_STRAIGHT_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_HEADING_RATE_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_SPEED_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_SPEED_STRAIGHT_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_SPEED_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_LENGTH_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_LENGTH_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_WIDTH_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_WIDTH_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_AREA_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_AREA_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_VRU_CLASS_PROB_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_VRU_CLASS_PROB_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_EGO_OBJ_HEADING_DIFF_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_EGO_OBJ_HEADING_DIFF_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_ECLIPSE_VALUE_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_ECLIPSE_VALUE_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_DANGER_ZONE_LEFT_LONG_ARRAY_SIZE_DIM0 (7u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_DANGER_ZONE_LEFT_LAT_ARRAY_SIZE_DIM0 (7u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_DANGER_ZONE_RIGHT_LONG_ARRAY_SIZE_DIM0 (7u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_DANGER_ZONE_RIGHT_LAT_ARRAY_SIZE_DIM0 (7u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_PFGS_EGO_SPEED_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_EXIST_PRBLTY_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_EXIST_PRBLTY_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_VCS_LONG_VEL_REL_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_VCS_LONG_VEL_REL_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_VCS_LAT_VEL_REL_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_VCS_LAT_VEL_REL_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_VCS_LONG_VEL_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_VCS_LONG_VEL_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_VCS_LAT_VEL_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_VCS_LAT_VEL_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_HEADING_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_HEADING_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_SPEED_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_SPEED_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_LENGTH_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_LENGTH_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_WIDTH_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_WIDTH_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_VRU_CLASS_PROB_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_VRU_CLASS_PROB_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_ECLIPSE_VALUE_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_ECLIPSE_VALUE_OFST_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_INFO_ZONE_LEFT_LONG_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_INFO_ZONE_LEFT_LAT_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_INFO_ZONE_RIGHT_LONG_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_INFO_ZONE_RIGHT_LAT_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_INFO_ZONE_LEFT_LONG_HYS_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_INFO_ZONE_LEFT_LAT_HYS_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_INFO_ZONE_RIGHT_LONG_HYS_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_INFO_ZONE_RIGHT_LAT_HYS_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_WING_ZONE_LEFT_LONG_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_WING_ZONE_LEFT_LAT_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_WING_ZONE_RIGHT_LONG_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_WING_ZONE_RIGHT_LAT_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_WING_ZONE_LEFT_LONG_HYS_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_WING_ZONE_LEFT_LAT_HYS_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_WING_ZONE_RIGHT_LONG_HYS_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_WING_ZONE_RIGHT_LAT_HYS_ARRAY_SIZE_DIM0 (4u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_TA_CRITICAL_APPROACH_CHECK_EGO_CIRCLES_ARRAY_SIZE_DIM0 (3u)

/* Macros for dimension size for all array variables */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_TA_ALERT_LVL_1_TTP_THRESHOLD_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_TA_EGO_SPEED_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_TA_EGO_SPEED_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_TA_EGO_YAWRATE_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_TA_EGO_YAWRATE_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_TA_EGO_LONG_ACCELERATION_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_TA_EGO_LONG_ACCELERATION_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_EXIST_PRBLTY_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_EXIST_PRBLTY_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_VCS_LONG_VEL_REL_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_VCS_LONG_VEL_REL_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_VCS_LAT_VEL_REL_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_VCS_LAT_VEL_REL_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_VCS_LONG_VEL_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_VCS_LONG_VEL_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_VCS_LAT_VEL_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_VCS_LAT_VEL_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_TA_LOOKUP_TURNING_HOST_SPEED_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_HEADING_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_HEADING_STRAIGHT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_HEADING_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_HEADING_RATE_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_HEADING_RATE_STRAIGHT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_HEADING_RATE_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_SPEED_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_SPEED_STRAIGHT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_SPEED_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_LENGTH_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_LENGTH_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_WIDTH_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_WIDTH_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_AREA_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_AREA_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_VRU_CLASS_PROB_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_VRU_CLASS_PROB_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_EGO_OBJ_HEADING_DIFF_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_EGO_OBJ_HEADING_DIFF_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_ECLIPSE_VALUE_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_OBJ_ECLIPSE_VALUE_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_DANGER_ZONE_LEFT_LONG_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_DANGER_ZONE_LEFT_LAT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_DANGER_ZONE_RIGHT_LONG_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_FTA_DANGER_ZONE_RIGHT_LAT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_PFGS_EGO_SPEED_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_EXIST_PRBLTY_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_EXIST_PRBLTY_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_VCS_LONG_VEL_REL_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_VCS_LONG_VEL_REL_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_VCS_LAT_VEL_REL_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_VCS_LAT_VEL_REL_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_VCS_LONG_VEL_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_VCS_LONG_VEL_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_VCS_LAT_VEL_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_VCS_LAT_VEL_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_HEADING_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_HEADING_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_SPEED_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_SPEED_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_LENGTH_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_LENGTH_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_WIDTH_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_WIDTH_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_VRU_CLASS_PROB_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_VRU_CLASS_PROB_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_ECLIPSE_VALUE_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_OBJ_ECLIPSE_VALUE_OFST_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_INFO_ZONE_LEFT_LONG_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_INFO_ZONE_LEFT_LAT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_INFO_ZONE_RIGHT_LONG_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_INFO_ZONE_RIGHT_LAT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_INFO_ZONE_LEFT_LONG_HYS_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_INFO_ZONE_LEFT_LAT_HYS_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_INFO_ZONE_RIGHT_LONG_HYS_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_INFO_ZONE_RIGHT_LAT_HYS_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_WING_ZONE_LEFT_LONG_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_WING_ZONE_LEFT_LAT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_WING_ZONE_RIGHT_LONG_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_WING_ZONE_RIGHT_LAT_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_WING_ZONE_LEFT_LONG_HYS_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_WING_ZONE_LEFT_LAT_HYS_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_WING_ZONE_RIGHT_LONG_HYS_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_RTA_WING_ZONE_RIGHT_LAT_HYS_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_K_TA_CRITICAL_APPROACH_CHECK_EGO_CIRCLES_ARRAY_DIM_SIZE (1u)


/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define TA_CORE_CALIBRATION_SIZE (1072u)

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

#ifdef CT_BIG_ENDIAN
typedef struct
{
   /* Definition of structure for big endian */
   uint8_t k_rta_wing_zone_point_size; /**<Sets the number of points used to construct the wing zones.*/
   uint8_t k_rta_info_zone_point_size; /**<Sets the number of points used to construct the info zones.*/
   uint8_t k_rta_obj_age_min; /**<RTA only adds objects with an object age greater than the specified threshold on its list of active objects.*/
   uint8_t k_pfgs_qualification_counter_slow_obj; /**<Maximum qualification counter value for PFGS alert and brake request. Threshold will be scaled based on object speed.*/
   uint8_t k_pfgs_qualification_counter_fast_obj; /**<Minimum qualification counter value for PFGS alert and brake request. Threshold will be scaled based on object speed. Value 3 means in third relevant cycle.*/
   uint8_t k_fta_danger_zone_point_size; /**<Sets the number of points used to construct the danger zones.*/
   uint8_t k_fta_obj_age_min; /**<FTA only adds objects with an object age greater than the specified threshold on its list of active objects.*/
   uint8_t k_ta_ego_pred_const_velocity_pred_steps_min; /**<This value indicates at which prediction step to switch to a constant velocity prediction model when TA alert level 4 was active in previous cycle.*/
   uint8_t k_ta_critical_approach_check_ego_circles[TA_K_TA_CRITICAL_APPROACH_CHECK_EGO_CIRCLES_ARRAY_SIZE_DIM0]; /**<TA only checks the enabled ego circle path predictions. [front middle rear].*/
   uint8_t k_ta_prediction_steps_max; /**<Maximum number of steps to predict trajectories of the ego and the relevant objects.*/
   uint8_t k_ta_alert_holding_cycles; /**<Number of cycle for holding an alert level.*/
   uint8_t k_ta_alert_qualifying_cycles; /**<Number of cycle to qualify an alert level.*/
   uint8_t k_unused_padding_byte_2; /**<Padded byte for byte packing of 4*/
   uint8_t k_unused_padding_byte_1; /**<Padded byte for byte packing of 4*/
   uint8_t k_unused_padding_byte_0; /**<Padded byte for byte packing of 4*/
   boolean_T k_f_rta_enable_wing_zones; /**<RTA uses this flag in order to turn on/off the wing zone area.*/
   boolean_T k_rta_f_higher_obj_crit_based_on_lower_ttp; /**<Objects that have a lower TTP will be evaluated as more critical. If disabled the criticality is based on closer distance to host vehicle.*/
   boolean_T k_f_rta_enable_info_zones; /**<RTA uses this flag in order to turn on/off the information zone area.*/
   boolean_T k_f_rta_enable; /**<TA uses this flag in order to turn on/off the RTA subfunction.*/
   boolean_T k_pfgs_qualification_check_f_stationary; /**<Enables the check for f_stationary. If active, only objects that are not stationary can increase the qualification counter.*/
   boolean_T k_pfgs_symbol_request_sides_enabled; /**<Enable output signal to show side of critical object.*/
   boolean_T k_f_fta_enable_danger_zones; /**<FTA uses this flag in order to turn on/off the danger zone area.*/
   boolean_T k_f_fta_enable_brake_gradient_logic; /**<FTA considers the brake gradient or jerk if this flag is enabled. It uses the value from k_fta_brake_gradient.*/
   boolean_T k_f_fta_enable; /**<TA uses this flag in order to turn on/off the FTA subfunction.*/
   boolean_T k_f_ta_enable_debug_mode; /**<TA uses this flag in order to turn on/off the debug mode which enables object position manipulation.*/
   boolean_T k_ta_f_apply_ttp_hysteresis_globally; /**<If active, k_ta_active_obj_ttp_offset is applied to all objects if a warning is currently raised. By default, k_ta_active_obj_ttp_offset is only applied to the currently warned object. */
   boolean_T k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj; /**<Flag to only allow TTC based alerts for tracker objects with status MATURE. Applies to alert levels 2 and higher.*/
   boolean_T k_ta_f_skip_holding_for_single_alert_level_drop; /**<Flag to skip alert level holding if the drop is only to one level lower. Only affects drop from level 4.*/
   boolean_T k_ta_f_only_allow_consecutive_ttc_based_alert_levels; /**<Flag to only allow consecutive alerts. Applies to alert levels 3 and higher.*/
   boolean_T k_ta_always_overwrite_ta_mode_to_both; /**<If any alert is raised, set TA mode to BOTH (dont distinguish between front/rear for qualifications).*/
   float32_T k_rta_wing_zone_right_lat_hys[TA_K_RTA_WING_ZONE_RIGHT_LAT_HYS_ARRAY_SIZE_DIM0]; /**<List of lat coordinates of wing zone addition on ego right side for hysteresis.*/
   float32_T k_rta_wing_zone_right_long_hys[TA_K_RTA_WING_ZONE_RIGHT_LONG_HYS_ARRAY_SIZE_DIM0]; /**<List of long coordinates of wing zone addition on ego right side for hysteresis.*/
   float32_T k_rta_wing_zone_left_lat_hys[TA_K_RTA_WING_ZONE_LEFT_LAT_HYS_ARRAY_SIZE_DIM0]; /**<List of lat coordinates of wing zone addition on ego left side for hysteresis.*/
   float32_T k_rta_wing_zone_left_long_hys[TA_K_RTA_WING_ZONE_LEFT_LONG_HYS_ARRAY_SIZE_DIM0]; /**<List of long coordinates of wing zone addition on ego left side for hysteresis.*/
   float32_T k_rta_wing_zone_right_lat[TA_K_RTA_WING_ZONE_RIGHT_LAT_ARRAY_SIZE_DIM0]; /**<List of lat coordinates of wing zone on ego right side.*/
   float32_T k_rta_wing_zone_right_long[TA_K_RTA_WING_ZONE_RIGHT_LONG_ARRAY_SIZE_DIM0]; /**<List of long coordinates of wing zone on ego right side.*/
   float32_T k_rta_wing_zone_left_lat[TA_K_RTA_WING_ZONE_LEFT_LAT_ARRAY_SIZE_DIM0]; /**<List of lat coordinates of wing zone on ego left side.*/
   float32_T k_rta_wing_zone_left_long[TA_K_RTA_WING_ZONE_LEFT_LONG_ARRAY_SIZE_DIM0]; /**<List of long coordinates of wing zone on ego left side.*/
   float32_T k_rta_info_zone_right_lat_hys[TA_K_RTA_INFO_ZONE_RIGHT_LAT_HYS_ARRAY_SIZE_DIM0]; /**<List of lat coordinates of info zone addition on ego right side for hysteresis.*/
   float32_T k_rta_info_zone_right_long_hys[TA_K_RTA_INFO_ZONE_RIGHT_LONG_HYS_ARRAY_SIZE_DIM0]; /**<List of long coordinates of info zone addition on ego right side for hysteresis.*/
   float32_T k_rta_info_zone_left_lat_hys[TA_K_RTA_INFO_ZONE_LEFT_LAT_HYS_ARRAY_SIZE_DIM0]; /**<List of lat coordinates of info zone addition on ego left side for hysteresis.*/
   float32_T k_rta_info_zone_left_long_hys[TA_K_RTA_INFO_ZONE_LEFT_LONG_HYS_ARRAY_SIZE_DIM0]; /**<List of long coordinates of info zone addition on ego left side for hysteresis.*/
   float32_T k_rta_info_zone_right_lat[TA_K_RTA_INFO_ZONE_RIGHT_LAT_ARRAY_SIZE_DIM0]; /**<List of lat coordinates of info zone on ego right side.*/
   float32_T k_rta_info_zone_right_long[TA_K_RTA_INFO_ZONE_RIGHT_LONG_ARRAY_SIZE_DIM0]; /**<List of long coordinates of info zone on ego right side.*/
   float32_T k_rta_info_zone_left_lat[TA_K_RTA_INFO_ZONE_LEFT_LAT_ARRAY_SIZE_DIM0]; /**<List of lat coordinates of info zone on ego left side.*/
   float32_T k_rta_info_zone_left_long[TA_K_RTA_INFO_ZONE_LEFT_LONG_ARRAY_SIZE_DIM0]; /**<List of long coordinates of info zone on ego left side.*/
   float32_T k_rta_ttp_curve_suppression_obj_distance_min; /**<RTA only calculates a TTP value when the host vehicle is not driving straight for close objects or objects that have curvi info available.*/
   float32_T k_rta_ttp_obj_abs_heading_diff_max; /**<RTA only calculates a TTP value for objects that are not heading away from the info zone.*/
   float32_T k_rta_ttp_obj_abs_lat_vel_rel_max; /**<RTA only calculates a TTP value for objects that are not moving away laterally from the info zone.*/
   float32_T k_rta_obj_eclipse_value_ofst[TA_K_RTA_OBJ_ECLIPSE_VALUE_OFST_ARRAY_SIZE_DIM0]; /**<RTA removes objects with an eclipse value outside of the sum of the specified values and k_rta_obj_eclipse_value entries from its list of active objects.*/
   float32_T k_rta_obj_eclipse_value[TA_K_RTA_OBJ_ECLIPSE_VALUE_ARRAY_SIZE_DIM0]; /**<RTA only adds objects with an eclipse value in the specified value range on its list of active objects.*/
   float32_T k_rta_obj_vru_class_prob_ofst[TA_K_RTA_OBJ_VRU_CLASS_PROB_OFST_ARRAY_SIZE_DIM0]; /**<RTA removes objects with a VRU class probability outside of the sum of the specified values and k_rta_obj_vru_class_prob entries from its list of active objects.*/
   float32_T k_rta_obj_vru_class_prob[TA_K_RTA_OBJ_VRU_CLASS_PROB_ARRAY_SIZE_DIM0]; /**<RTA only adds objects with a VRU class probability in the specified value range on its list of active objects.*/
   float32_T k_rta_obj_width_ofst[TA_K_RTA_OBJ_WIDTH_OFST_ARRAY_SIZE_DIM0]; /**<RTA removes objects with a width outside of the sum of the specified values and k_rta_obj_width entries from its list of active objects.*/
   float32_T k_rta_obj_width[TA_K_RTA_OBJ_WIDTH_ARRAY_SIZE_DIM0]; /**<RTA only adds objects with a width in the specified value range on its list of active objects.*/
   float32_T k_rta_obj_length_ofst[TA_K_RTA_OBJ_LENGTH_OFST_ARRAY_SIZE_DIM0]; /**<RTA removes objects with a length outside of the sum of the specified values and k_rta_obj_length entries from its list of active objects.*/
   float32_T k_rta_obj_length[TA_K_RTA_OBJ_LENGTH_ARRAY_SIZE_DIM0]; /**<RTA only adds objects with a length in the specified value range on its list of active objects.*/
   float32_T k_rta_obj_speed_ofst[TA_K_RTA_OBJ_SPEED_OFST_ARRAY_SIZE_DIM0]; /**<RTA removes objects with a speed outside of the sum of the specified values and k_rta_obj_speed entries from its list of active objects.*/
   float32_T k_rta_obj_speed[TA_K_RTA_OBJ_SPEED_ARRAY_SIZE_DIM0]; /**<RTA only adds objects with a speed in the specified value range on its list of active objects.*/
   float32_T k_rta_obj_heading_ofst[TA_K_RTA_OBJ_HEADING_OFST_ARRAY_SIZE_DIM0]; /**<RTA removes objects with an absolute heading outside of the sum of the specified values and k_rta_obj_heading entries from its list of active objects.*/
   float32_T k_rta_obj_heading[TA_K_RTA_OBJ_HEADING_ARRAY_SIZE_DIM0]; /**<RTA only adds objects with an absolute heading in the specified value range on its list of active objects.*/
   float32_T k_rta_obj_vcs_lat_vel_ofst[TA_K_RTA_OBJ_VCS_LAT_VEL_OFST_ARRAY_SIZE_DIM0]; /**<RTA removes objects with a vcs lat velocity outside of the sum of the specified values and k_rta_obj_vcs_lat_vel_rel entries from its list of active objects.*/
   float32_T k_rta_obj_vcs_lat_vel[TA_K_RTA_OBJ_VCS_LAT_VEL_ARRAY_SIZE_DIM0]; /**<RTA only adds objects with a vcs lat velocity in the specified value range to its list of active objects.*/
   float32_T k_rta_obj_vcs_long_vel_ofst[TA_K_RTA_OBJ_VCS_LONG_VEL_OFST_ARRAY_SIZE_DIM0]; /**<RTA removes objects with a vcs long velocity outside of the sum of the specified values and k_rta_obj_vcs_long_vel_rel entries from its list of active objects.*/
   float32_T k_rta_obj_vcs_long_vel[TA_K_RTA_OBJ_VCS_LONG_VEL_ARRAY_SIZE_DIM0]; /**<RTA adds only objects with a vcs long velocity in the specified value range to its list of active objects.*/
   float32_T k_rta_obj_vcs_lat_vel_rel_ofst[TA_K_RTA_OBJ_VCS_LAT_VEL_REL_OFST_ARRAY_SIZE_DIM0]; /**<RTA removes objects with a relative vcs lat velocity outside of the sum of the specified values and k_rta_obj_vcs_lat_vel_rel entries from its list of active objects.*/
   float32_T k_rta_obj_vcs_lat_vel_rel[TA_K_RTA_OBJ_VCS_LAT_VEL_REL_ARRAY_SIZE_DIM0]; /**<RTA adds only objects with a relative vcs lat velocity in the specified value range to its list of active objects.*/
   float32_T k_rta_obj_vcs_long_vel_rel_ofst[TA_K_RTA_OBJ_VCS_LONG_VEL_REL_OFST_ARRAY_SIZE_DIM0]; /**<RTA removes objects with a relative vcs long velocity outside of the sum of the specified values and k_rta_obj_vcs_long_vel_rel entries from its list of active objects.*/
   float32_T k_rta_obj_vcs_long_vel_rel[TA_K_RTA_OBJ_VCS_LONG_VEL_REL_ARRAY_SIZE_DIM0]; /**<RTA adds only objects with a relative vcs long velocity in the specified value range to its list of active objects.*/
   float32_T k_rta_obj_exist_prblty_ofst[TA_K_RTA_OBJ_EXIST_PRBLTY_OFST_ARRAY_SIZE_DIM0]; /**<RTA removes objects with an existence probability outside of the sum of the specified values and k_rta_obj_exist_prblty entries from its list of active objects.*/
   float32_T k_rta_obj_exist_prblty[TA_K_RTA_OBJ_EXIST_PRBLTY_ARRAY_SIZE_DIM0]; /**<RTA puts only objects with an existence probability in the specified value range on its list of active objects.*/
   float32_T k_tap_lvl_2_host_curvature_min; /**<For TAP warnings of level 2 and higher the host curvature must be greater than this parameter.*/
   float32_T k_pfgs_qualification_ttc_min; /**<Minimum TTC value of most critical object to start qualification process for PFGS.*/
   float32_T k_pfgs_ego_speed[TA_K_PFGS_EGO_SPEED_ARRAY_SIZE_DIM0]; /**<PFGS only increases qualification counter for ego vehicle in the specified speed range.*/
   float32_T k_fta_danger_zone_right_lat[TA_K_FTA_DANGER_ZONE_RIGHT_LAT_ARRAY_SIZE_DIM0]; /**<List of lat coordinates of danger zone on ego right side.*/
   float32_T k_fta_danger_zone_right_long[TA_K_FTA_DANGER_ZONE_RIGHT_LONG_ARRAY_SIZE_DIM0]; /**<List of long coordinates of danger zone on ego right side.*/
   float32_T k_fta_danger_zone_left_lat[TA_K_FTA_DANGER_ZONE_LEFT_LAT_ARRAY_SIZE_DIM0]; /**<List of lat coordinates of danger zone on ego left side.*/
   float32_T k_fta_danger_zone_left_long[TA_K_FTA_DANGER_ZONE_LEFT_LONG_ARRAY_SIZE_DIM0]; /**<List of long coordinates of danger zone on ego left side.*/
   float32_T k_fta_brake_gradient; /**<FTA assumes that the vehicle deceleration will ramp up with the specified gradient or jerk. Only active if k_f_fta_enable_brake_gradient_logic is enabled.*/
   float32_T k_fta_brake_dead_time; /**<FTA assumes that the vehicle will start deceleration after the specified dead time of the braking system.*/
   float32_T k_fta_brake_deceleration_max; /**<FTA caps the maximum braking deceleration to this value.*/
   float32_T k_fta_obj_velocity_heading_diff_max; /**<FTA removes objects with a difference of heading and velocity vector based heading greater than the specified threshold from its list of active objects.*/
   float32_T k_fta_obj_eclipse_value_ofst[TA_K_FTA_OBJ_ECLIPSE_VALUE_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with an eclipse value outside of the sum of the specified values and k_fta_obj_eclipse_value entries from its list of active objects.*/
   float32_T k_fta_obj_eclipse_value[TA_K_FTA_OBJ_ECLIPSE_VALUE_ARRAY_SIZE_DIM0]; /**<FTA only adds objects with an eclipse value in the specified value range on its list of active objects.*/
   float32_T k_fta_ego_obj_heading_diff_ofst[TA_K_FTA_EGO_OBJ_HEADING_DIFF_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with an absolute heading difference to the turning host vehicle outside of the sum of the specified values and k_fta_ego_obj_heading_diff entries from its list of active objects.*/
   float32_T k_fta_ego_obj_heading_diff[TA_K_FTA_EGO_OBJ_HEADING_DIFF_ARRAY_SIZE_DIM0]; /**<FTA only adds objects with an absolute heading difference to the turning host vehicle in the specified value range on its list of active objects.*/
   float32_T k_fta_obj_vru_class_prob_ofst[TA_K_FTA_OBJ_VRU_CLASS_PROB_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with a VRU class probability outside of the sum of the specified values and k_fta_obj_vru_class_prob entries from its list of active objects.*/
   float32_T k_fta_obj_vru_class_prob[TA_K_FTA_OBJ_VRU_CLASS_PROB_ARRAY_SIZE_DIM0]; /**<FTA only adds objects with a VRU class probability in the specified value range on its list of active objects.*/
   float32_T k_fta_obj_area_ofst[TA_K_FTA_OBJ_AREA_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with an area outside of the sum of the specified values and k_fta_obj_area entries from its list of active objects.*/
   float32_T k_fta_obj_area[TA_K_FTA_OBJ_AREA_ARRAY_SIZE_DIM0]; /**<FTA only adds objects with an area in the specified value range on its list of active objects.*/
   float32_T k_fta_obj_width_ofst[TA_K_FTA_OBJ_WIDTH_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with a width outside of the sum of the specified values and k_fta_obj_width entries from its list of active objects.*/
   float32_T k_fta_obj_width[TA_K_FTA_OBJ_WIDTH_ARRAY_SIZE_DIM0]; /**<FTA only adds objects with a width in the specified value range on its list of active objects.*/
   float32_T k_fta_obj_length_ofst[TA_K_FTA_OBJ_LENGTH_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with a length outside of the sum of the specified values and k_fta_obj_length entries from its list of active objects.*/
   float32_T k_fta_obj_length[TA_K_FTA_OBJ_LENGTH_ARRAY_SIZE_DIM0]; /**<FTA only adds objects with a length in the specified value range on its list of active objects.*/
   float32_T k_fta_obj_speed_ofst[TA_K_FTA_OBJ_SPEED_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with a speed outside of the sum of the specified values and k_fta_obj_speed entries from its list of active objects.*/
   float32_T k_fta_obj_speed_straight[TA_K_FTA_OBJ_SPEED_STRAIGHT_ARRAY_SIZE_DIM0]; /**<FTA only adds objects with a speed in the specified value range on its list of active objects when driving straight.*/
   float32_T k_fta_obj_speed[TA_K_FTA_OBJ_SPEED_ARRAY_SIZE_DIM0]; /**<FTA only adds objects with a speed in the specified value range on its list of active objects.*/
   float32_T k_fta_obj_heading_rate_ofst[TA_K_FTA_OBJ_HEADING_RATE_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with an absolute heading rate outside of the sum of the specified values and k_fta_obj_heading_rate entries from its list of active objects.*/
   float32_T k_fta_obj_heading_rate_straight[TA_K_FTA_OBJ_HEADING_RATE_STRAIGHT_ARRAY_SIZE_DIM0]; /**<In straight scenarios, FTA only adds objects with an absolute heading rate in the specified value range on its list of active objects.*/
   float32_T k_fta_obj_heading_rate[TA_K_FTA_OBJ_HEADING_RATE_ARRAY_SIZE_DIM0]; /**<In turning scenarios, FTA only adds objects with an absolute heading rate in the specified value range on its list of active objects.*/
   float32_T k_fta_obj_heading_ofst[TA_K_FTA_OBJ_HEADING_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with an absolute heading outside of the sum of the specified values and k_fta_obj_heading entries from its list of active objects.*/
   float32_T k_fta_obj_heading_straight[TA_K_FTA_OBJ_HEADING_STRAIGHT_ARRAY_SIZE_DIM0]; /**<In straight scenarios, FTA only adds objects with an absolute heading in the specified value range on its list of active objects.*/
   float32_T k_fta_obj_heading[TA_K_FTA_OBJ_HEADING_ARRAY_SIZE_DIM0]; /**<In curved scenarios, FTA only adds objects with an absolute heading in the specified value range on its list of active objects.*/
   float32_T k_fta_obj_vcs_long_pos_straight_min; /**<In straight scenarios, FTA only adds objects that are generally positioned in front of the host vehicle to its list of active objects.*/
   float32_T k_ta_lookup_turning_host_curvature_min[TA_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN_ARRAY_SIZE_DIM0]; /**<For TA to assume that the host vehicle is turning, the host curvature is must be greater than this lookup parameter depending on host vehicle speed set in k_ta_lookup_turning_host_speed.*/
   float32_T k_ta_lookup_turning_host_speed[TA_K_TA_LOOKUP_TURNING_HOST_SPEED_ARRAY_SIZE_DIM0]; /**<This host speed parameter configures the lookup table for the host turning logic depending on curvature.*/
   float32_T k_ta_straight_host_curvature_max; /**<For TA to assume that the host vehicle is driving straight, the host curvature is must be lower than this parameter.*/
   float32_T k_fta_obj_vcs_lat_vel_ofst[TA_K_FTA_OBJ_VCS_LAT_VEL_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with a vcs lat velocity outside of the sum of the specified values and k_fta_obj_vcs_lat_vel_rel entries from its list of active objects.*/
   float32_T k_fta_obj_vcs_lat_vel[TA_K_FTA_OBJ_VCS_LAT_VEL_ARRAY_SIZE_DIM0]; /**<FTA only adds objects with a vcs lat velocity in the specified value range to its list of active objects.*/
   float32_T k_fta_obj_vcs_long_vel_ofst[TA_K_FTA_OBJ_VCS_LONG_VEL_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with a vcs long velocity outside of the sum of the specified values and k_fta_obj_vcs_long_vel_rel entries from its list of active objects.*/
   float32_T k_fta_obj_vcs_long_vel[TA_K_FTA_OBJ_VCS_LONG_VEL_ARRAY_SIZE_DIM0]; /**<FTA adds only objects with a vcs long velocity in the specified value range to its list of active objects.*/
   float32_T k_fta_obj_vcs_lat_vel_rel_ofst[TA_K_FTA_OBJ_VCS_LAT_VEL_REL_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with a relative vcs lat velocity outside of the sum of the specified values and k_fta_obj_vcs_lat_vel_rel entries from its list of active objects.*/
   float32_T k_fta_obj_vcs_lat_vel_rel[TA_K_FTA_OBJ_VCS_LAT_VEL_REL_ARRAY_SIZE_DIM0]; /**<FTA adds only objects with a relative vcs lat velocity in the specified value range to its list of active objects.*/
   float32_T k_fta_obj_vcs_long_vel_rel_ofst[TA_K_FTA_OBJ_VCS_LONG_VEL_REL_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with a relative vcs long velocity outside of the sum of the specified values and k_fta_obj_vcs_long_vel_rel entries from its list of active objects.*/
   float32_T k_fta_obj_vcs_long_vel_rel[TA_K_FTA_OBJ_VCS_LONG_VEL_REL_ARRAY_SIZE_DIM0]; /**<FTA adds only objects with a relative vcs long velocity in the specified value range to its list of active objects.*/
   float32_T k_fta_obj_exist_prblty_ofst[TA_K_FTA_OBJ_EXIST_PRBLTY_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with an existence probability outside of the sum of the specified values and k_fta_obj_exist_prblty entries from its list of active objects.*/
   float32_T k_fta_obj_exist_prblty[TA_K_FTA_OBJ_EXIST_PRBLTY_ARRAY_SIZE_DIM0]; /**<FTA puts only objects with an existence probability in the specified value range on its list of active objects.*/
   float32_T k_ta_ego_yawangle_integration_yawrate_min; /**<This value indicates the minimum (absolute) yawrate value to start integrating the ego yawangle. Below this threshold the yawangle resets.*/
   float32_T k_ta_ego_circle_host_length_factor; /**<This weight factor controls the influence of the host vehicle length when calculating the host vehicle circle offsets.*/
   float32_T k_ta_ego_circle_offset; /**<This weight factor adds an additional offset to the host circles.*/
   float32_T k_ta_ego_deceleration_weight; /**<This weight factor controls the influence of the negative acceleration / deceleration value on the predicted velocity in each prediction step.*/
   float32_T k_ta_ego_acceleration_weight; /**<This weight factor controls the influence of the positive acceleration value on the predicted velocity in each prediction step.*/
   float32_T k_ta_ego_long_acceleration_ofst[TA_K_TA_EGO_LONG_ACCELERATION_OFST_ARRAY_SIZE_DIM0]; /**<TA adds this offset to k_ta_ego_long_acceleration for objects that have been active previously.*/
   float32_T k_ta_ego_long_acceleration[TA_K_TA_EGO_LONG_ACCELERATION_ARRAY_SIZE_DIM0]; /**<TA only marks objects as relevant for an ego acceleration in the specified range.*/
   float32_T k_ta_ego_yawrate_ofst[TA_K_TA_EGO_YAWRATE_OFST_ARRAY_SIZE_DIM0]; /**<TA adds this offset to k_ta_ego_yawrate for objects that have been active previously.*/
   float32_T k_ta_ego_yawrate[TA_K_TA_EGO_YAWRATE_ARRAY_SIZE_DIM0]; /**<TA only marks objects as relevant for an absolute ego yawrate in the specified range.*/
   float32_T k_ta_ego_speed_ofst[TA_K_TA_EGO_SPEED_OFST_ARRAY_SIZE_DIM0]; /**<TA adds these offsets to k_ta_ego_speed for objects that have been active previously.*/
   float32_T k_ta_ego_speed[TA_K_TA_EGO_SPEED_ARRAY_SIZE_DIM0]; /**<TA only marks objects as relevant for ego vehicle in the specified speed range.*/
   float32_T k_ta_obj_shape_gain_fixed; /**<TA grows or shrinks an object's shape by multiplying the specified value with the object's dimensions once at the beginning of the prediction phase. Value greater than 1: object shape will be increased, value lower than 1: object shape will be decreased*/
   float32_T k_ta_ego_shape_gain_fixed; /**<TA grows or shrinks the ego shape by multiplying the specified value with the ego dimensions once at the beginning of the prediction phase. Value greater than 1: ego shape will be increased, value lower than 1: ego shape will be decreased*/
   float32_T k_ta_obj_shape_gain_per_pred_step; /**<TA grows or shrinks the ego shape by multiplying the specified value with the ego dimensions at every prediction step. This means, the more in the future a prediction step is, the more the shape will be grown or shrunk. This is in addition to the fixed gain specified by k_obj_shape_gain_fixed. Value greater than 1: object shape will grow, value lower than 1: object shape will shrink*/
   float32_T k_ta_ego_shape_gain_per_pred_step; /**<TA grows or shrinks an object's shape by multiplying the specified value with the object's dimensions at every prediction step. This means, the more in the future a prediction step is, the more the shape will be grown or shrunk. This is in addition to the fixed gain specified by k_ego_shape_gain_fixed. Value greater than 1: ego shape will grow, value lower than 1: ego shape will shrink*/
   float32_T k_ta_obj_acceleration_lat_weight; /**<This weight factor controls the influence of the acceleration value for the object in lat direction.*/
   float32_T k_ta_obj_acceleration_long_weight; /**<This weight factor controls the influence of the acceleration value for the object in long direction.*/
   float32_T k_ta_obj_pred_speed_min; /**<TA only continues object trajectory calculation if the predicted waypoint speed is equal or above this threshold.*/
   float32_T k_ta_ego_max_pred_yaw_angle; /**<TA only continues the ego trajectory calculation if the (absolute) predicted yaw angle to the last straight section is below this threshold.*/
   float32_T k_ta_critical_approach_angle_diff_min; /**<TA considers an approach of an object to the ego only as critical above this angle difference.*/
   float32_T k_ta_critical_approach_min_safe_distance; /**<TA considers an approach of an object to the ego with a distance smaller than the specified value as a collision.*/
   float32_T k_ta_alert_lvl_4_decel_threshold; /**<TA raises an alert level 4 for an object with an estimated host deceleration to avoid a collision greater than or equal to the specified value.*/
   float32_T k_ta_alert_lvl_4_ttc_threshold; /**<TA raises an alert level 4 for an object with a calculated TTC smaller than or equal to the specified value.*/
   float32_T k_ta_alert_lvl_3_ttb_threshold; /**<TA raises an alert level 3 only for an object with a calculated TTB (time-to-brake) smaller than or equal to the specified value.*/
   float32_T k_ta_alert_lvl_3_ttc_threshold; /**<TA raises an alert level 3 for an object with a calculated TTC smaller than or equal to the specified value.*/
   float32_T k_ta_alert_lvl_2_ttc_threshold; /**<TA raises an alert level 2 for an object with a calculated TTC smaller than or equal to the specified value.*/
   float32_T k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max; /**<TA uses the normal alert trigger below the specified vehicle host speed. At and above this host speed, the early trigger is used.*/
   float32_T k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max; /**<TA uses the late alert trigger below the specified vehicle host speed.*/
   float32_T k_ta_active_obj_ttp_offset; /**<TA raises an alert level 1 for a previously active object with a calculated TTP smaller than or equal to k_ta_alert_lvl_1_ttp_threshold plus this specified value.*/
   float32_T k_ta_alert_lvl_1_ttp_threshold[TA_K_TA_ALERT_LVL_1_TTP_THRESHOLD_ARRAY_SIZE_DIM0]; /**<TA raises an alert level 1 for an object with a calculated TTP smaller than or equal to the specified value based on alert trigger sensitivity [EARLY NORMAL LATE].*/
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
} Ta_Core_Calibration_T;
#else
typedef struct
{
   /* Definition of structure for little endian */
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
   float32_T k_ta_alert_lvl_1_ttp_threshold[TA_K_TA_ALERT_LVL_1_TTP_THRESHOLD_ARRAY_SIZE_DIM0]; /**<TA raises an alert level 1 for an object with a calculated TTP smaller than or equal to the specified value based on alert trigger sensitivity [EARLY NORMAL LATE].*/
   float32_T k_ta_active_obj_ttp_offset; /**<TA raises an alert level 1 for a previously active object with a calculated TTP smaller than or equal to k_ta_alert_lvl_1_ttp_threshold plus this specified value.*/
   float32_T k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max; /**<TA uses the late alert trigger below the specified vehicle host speed.*/
   float32_T k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max; /**<TA uses the normal alert trigger below the specified vehicle host speed. At and above this host speed, the early trigger is used.*/
   float32_T k_ta_alert_lvl_2_ttc_threshold; /**<TA raises an alert level 2 for an object with a calculated TTC smaller than or equal to the specified value.*/
   float32_T k_ta_alert_lvl_3_ttc_threshold; /**<TA raises an alert level 3 for an object with a calculated TTC smaller than or equal to the specified value.*/
   float32_T k_ta_alert_lvl_3_ttb_threshold; /**<TA raises an alert level 3 only for an object with a calculated TTB (time-to-brake) smaller than or equal to the specified value.*/
   float32_T k_ta_alert_lvl_4_ttc_threshold; /**<TA raises an alert level 4 for an object with a calculated TTC smaller than or equal to the specified value.*/
   float32_T k_ta_alert_lvl_4_decel_threshold; /**<TA raises an alert level 4 for an object with an estimated host deceleration to avoid a collision greater than or equal to the specified value.*/
   float32_T k_ta_critical_approach_min_safe_distance; /**<TA considers an approach of an object to the ego with a distance smaller than the specified value as a collision.*/
   float32_T k_ta_critical_approach_angle_diff_min; /**<TA considers an approach of an object to the ego only as critical above this angle difference.*/
   float32_T k_ta_ego_max_pred_yaw_angle; /**<TA only continues the ego trajectory calculation if the (absolute) predicted yaw angle to the last straight section is below this threshold.*/
   float32_T k_ta_obj_pred_speed_min; /**<TA only continues object trajectory calculation if the predicted waypoint speed is equal or above this threshold.*/
   float32_T k_ta_obj_acceleration_long_weight; /**<This weight factor controls the influence of the acceleration value for the object in long direction.*/
   float32_T k_ta_obj_acceleration_lat_weight; /**<This weight factor controls the influence of the acceleration value for the object in lat direction.*/
   float32_T k_ta_ego_shape_gain_per_pred_step; /**<TA grows or shrinks an object's shape by multiplying the specified value with the object's dimensions at every prediction step. This means, the more in the future a prediction step is, the more the shape will be grown or shrunk. This is in addition to the fixed gain specified by k_ego_shape_gain_fixed. Value greater than 1: ego shape will grow, value lower than 1: ego shape will shrink*/
   float32_T k_ta_obj_shape_gain_per_pred_step; /**<TA grows or shrinks the ego shape by multiplying the specified value with the ego dimensions at every prediction step. This means, the more in the future a prediction step is, the more the shape will be grown or shrunk. This is in addition to the fixed gain specified by k_obj_shape_gain_fixed. Value greater than 1: object shape will grow, value lower than 1: object shape will shrink*/
   float32_T k_ta_ego_shape_gain_fixed; /**<TA grows or shrinks the ego shape by multiplying the specified value with the ego dimensions once at the beginning of the prediction phase. Value greater than 1: ego shape will be increased, value lower than 1: ego shape will be decreased*/
   float32_T k_ta_obj_shape_gain_fixed; /**<TA grows or shrinks an object's shape by multiplying the specified value with the object's dimensions once at the beginning of the prediction phase. Value greater than 1: object shape will be increased, value lower than 1: object shape will be decreased*/
   float32_T k_ta_ego_speed[TA_K_TA_EGO_SPEED_ARRAY_SIZE_DIM0]; /**<TA only marks objects as relevant for ego vehicle in the specified speed range.*/
   float32_T k_ta_ego_speed_ofst[TA_K_TA_EGO_SPEED_OFST_ARRAY_SIZE_DIM0]; /**<TA adds these offsets to k_ta_ego_speed for objects that have been active previously.*/
   float32_T k_ta_ego_yawrate[TA_K_TA_EGO_YAWRATE_ARRAY_SIZE_DIM0]; /**<TA only marks objects as relevant for an absolute ego yawrate in the specified range.*/
   float32_T k_ta_ego_yawrate_ofst[TA_K_TA_EGO_YAWRATE_OFST_ARRAY_SIZE_DIM0]; /**<TA adds this offset to k_ta_ego_yawrate for objects that have been active previously.*/
   float32_T k_ta_ego_long_acceleration[TA_K_TA_EGO_LONG_ACCELERATION_ARRAY_SIZE_DIM0]; /**<TA only marks objects as relevant for an ego acceleration in the specified range.*/
   float32_T k_ta_ego_long_acceleration_ofst[TA_K_TA_EGO_LONG_ACCELERATION_OFST_ARRAY_SIZE_DIM0]; /**<TA adds this offset to k_ta_ego_long_acceleration for objects that have been active previously.*/
   float32_T k_ta_ego_acceleration_weight; /**<This weight factor controls the influence of the positive acceleration value on the predicted velocity in each prediction step.*/
   float32_T k_ta_ego_deceleration_weight; /**<This weight factor controls the influence of the negative acceleration / deceleration value on the predicted velocity in each prediction step.*/
   float32_T k_ta_ego_circle_offset; /**<This weight factor adds an additional offset to the host circles.*/
   float32_T k_ta_ego_circle_host_length_factor; /**<This weight factor controls the influence of the host vehicle length when calculating the host vehicle circle offsets.*/
   float32_T k_ta_ego_yawangle_integration_yawrate_min; /**<This value indicates the minimum (absolute) yawrate value to start integrating the ego yawangle. Below this threshold the yawangle resets.*/
   float32_T k_fta_obj_exist_prblty[TA_K_FTA_OBJ_EXIST_PRBLTY_ARRAY_SIZE_DIM0]; /**<FTA puts only objects with an existence probability in the specified value range on its list of active objects.*/
   float32_T k_fta_obj_exist_prblty_ofst[TA_K_FTA_OBJ_EXIST_PRBLTY_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with an existence probability outside of the sum of the specified values and k_fta_obj_exist_prblty entries from its list of active objects.*/
   float32_T k_fta_obj_vcs_long_vel_rel[TA_K_FTA_OBJ_VCS_LONG_VEL_REL_ARRAY_SIZE_DIM0]; /**<FTA adds only objects with a relative vcs long velocity in the specified value range to its list of active objects.*/
   float32_T k_fta_obj_vcs_long_vel_rel_ofst[TA_K_FTA_OBJ_VCS_LONG_VEL_REL_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with a relative vcs long velocity outside of the sum of the specified values and k_fta_obj_vcs_long_vel_rel entries from its list of active objects.*/
   float32_T k_fta_obj_vcs_lat_vel_rel[TA_K_FTA_OBJ_VCS_LAT_VEL_REL_ARRAY_SIZE_DIM0]; /**<FTA adds only objects with a relative vcs lat velocity in the specified value range to its list of active objects.*/
   float32_T k_fta_obj_vcs_lat_vel_rel_ofst[TA_K_FTA_OBJ_VCS_LAT_VEL_REL_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with a relative vcs lat velocity outside of the sum of the specified values and k_fta_obj_vcs_lat_vel_rel entries from its list of active objects.*/
   float32_T k_fta_obj_vcs_long_vel[TA_K_FTA_OBJ_VCS_LONG_VEL_ARRAY_SIZE_DIM0]; /**<FTA adds only objects with a vcs long velocity in the specified value range to its list of active objects.*/
   float32_T k_fta_obj_vcs_long_vel_ofst[TA_K_FTA_OBJ_VCS_LONG_VEL_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with a vcs long velocity outside of the sum of the specified values and k_fta_obj_vcs_long_vel_rel entries from its list of active objects.*/
   float32_T k_fta_obj_vcs_lat_vel[TA_K_FTA_OBJ_VCS_LAT_VEL_ARRAY_SIZE_DIM0]; /**<FTA only adds objects with a vcs lat velocity in the specified value range to its list of active objects.*/
   float32_T k_fta_obj_vcs_lat_vel_ofst[TA_K_FTA_OBJ_VCS_LAT_VEL_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with a vcs lat velocity outside of the sum of the specified values and k_fta_obj_vcs_lat_vel_rel entries from its list of active objects.*/
   float32_T k_ta_straight_host_curvature_max; /**<For TA to assume that the host vehicle is driving straight, the host curvature is must be lower than this parameter.*/
   float32_T k_ta_lookup_turning_host_speed[TA_K_TA_LOOKUP_TURNING_HOST_SPEED_ARRAY_SIZE_DIM0]; /**<This host speed parameter configures the lookup table for the host turning logic depending on curvature.*/
   float32_T k_ta_lookup_turning_host_curvature_min[TA_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN_ARRAY_SIZE_DIM0]; /**<For TA to assume that the host vehicle is turning, the host curvature is must be greater than this lookup parameter depending on host vehicle speed set in k_ta_lookup_turning_host_speed.*/
   float32_T k_fta_obj_vcs_long_pos_straight_min; /**<In straight scenarios, FTA only adds objects that are generally positioned in front of the host vehicle to its list of active objects.*/
   float32_T k_fta_obj_heading[TA_K_FTA_OBJ_HEADING_ARRAY_SIZE_DIM0]; /**<In curved scenarios, FTA only adds objects with an absolute heading in the specified value range on its list of active objects.*/
   float32_T k_fta_obj_heading_straight[TA_K_FTA_OBJ_HEADING_STRAIGHT_ARRAY_SIZE_DIM0]; /**<In straight scenarios, FTA only adds objects with an absolute heading in the specified value range on its list of active objects.*/
   float32_T k_fta_obj_heading_ofst[TA_K_FTA_OBJ_HEADING_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with an absolute heading outside of the sum of the specified values and k_fta_obj_heading entries from its list of active objects.*/
   float32_T k_fta_obj_heading_rate[TA_K_FTA_OBJ_HEADING_RATE_ARRAY_SIZE_DIM0]; /**<In turning scenarios, FTA only adds objects with an absolute heading rate in the specified value range on its list of active objects.*/
   float32_T k_fta_obj_heading_rate_straight[TA_K_FTA_OBJ_HEADING_RATE_STRAIGHT_ARRAY_SIZE_DIM0]; /**<In straight scenarios, FTA only adds objects with an absolute heading rate in the specified value range on its list of active objects.*/
   float32_T k_fta_obj_heading_rate_ofst[TA_K_FTA_OBJ_HEADING_RATE_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with an absolute heading rate outside of the sum of the specified values and k_fta_obj_heading_rate entries from its list of active objects.*/
   float32_T k_fta_obj_speed[TA_K_FTA_OBJ_SPEED_ARRAY_SIZE_DIM0]; /**<FTA only adds objects with a speed in the specified value range on its list of active objects.*/
   float32_T k_fta_obj_speed_straight[TA_K_FTA_OBJ_SPEED_STRAIGHT_ARRAY_SIZE_DIM0]; /**<FTA only adds objects with a speed in the specified value range on its list of active objects when driving straight.*/
   float32_T k_fta_obj_speed_ofst[TA_K_FTA_OBJ_SPEED_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with a speed outside of the sum of the specified values and k_fta_obj_speed entries from its list of active objects.*/
   float32_T k_fta_obj_length[TA_K_FTA_OBJ_LENGTH_ARRAY_SIZE_DIM0]; /**<FTA only adds objects with a length in the specified value range on its list of active objects.*/
   float32_T k_fta_obj_length_ofst[TA_K_FTA_OBJ_LENGTH_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with a length outside of the sum of the specified values and k_fta_obj_length entries from its list of active objects.*/
   float32_T k_fta_obj_width[TA_K_FTA_OBJ_WIDTH_ARRAY_SIZE_DIM0]; /**<FTA only adds objects with a width in the specified value range on its list of active objects.*/
   float32_T k_fta_obj_width_ofst[TA_K_FTA_OBJ_WIDTH_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with a width outside of the sum of the specified values and k_fta_obj_width entries from its list of active objects.*/
   float32_T k_fta_obj_area[TA_K_FTA_OBJ_AREA_ARRAY_SIZE_DIM0]; /**<FTA only adds objects with an area in the specified value range on its list of active objects.*/
   float32_T k_fta_obj_area_ofst[TA_K_FTA_OBJ_AREA_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with an area outside of the sum of the specified values and k_fta_obj_area entries from its list of active objects.*/
   float32_T k_fta_obj_vru_class_prob[TA_K_FTA_OBJ_VRU_CLASS_PROB_ARRAY_SIZE_DIM0]; /**<FTA only adds objects with a VRU class probability in the specified value range on its list of active objects.*/
   float32_T k_fta_obj_vru_class_prob_ofst[TA_K_FTA_OBJ_VRU_CLASS_PROB_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with a VRU class probability outside of the sum of the specified values and k_fta_obj_vru_class_prob entries from its list of active objects.*/
   float32_T k_fta_ego_obj_heading_diff[TA_K_FTA_EGO_OBJ_HEADING_DIFF_ARRAY_SIZE_DIM0]; /**<FTA only adds objects with an absolute heading difference to the turning host vehicle in the specified value range on its list of active objects.*/
   float32_T k_fta_ego_obj_heading_diff_ofst[TA_K_FTA_EGO_OBJ_HEADING_DIFF_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with an absolute heading difference to the turning host vehicle outside of the sum of the specified values and k_fta_ego_obj_heading_diff entries from its list of active objects.*/
   float32_T k_fta_obj_eclipse_value[TA_K_FTA_OBJ_ECLIPSE_VALUE_ARRAY_SIZE_DIM0]; /**<FTA only adds objects with an eclipse value in the specified value range on its list of active objects.*/
   float32_T k_fta_obj_eclipse_value_ofst[TA_K_FTA_OBJ_ECLIPSE_VALUE_OFST_ARRAY_SIZE_DIM0]; /**<FTA removes objects with an eclipse value outside of the sum of the specified values and k_fta_obj_eclipse_value entries from its list of active objects.*/
   float32_T k_fta_obj_velocity_heading_diff_max; /**<FTA removes objects with a difference of heading and velocity vector based heading greater than the specified threshold from its list of active objects.*/
   float32_T k_fta_brake_deceleration_max; /**<FTA caps the maximum braking deceleration to this value.*/
   float32_T k_fta_brake_dead_time; /**<FTA assumes that the vehicle will start deceleration after the specified dead time of the braking system.*/
   float32_T k_fta_brake_gradient; /**<FTA assumes that the vehicle deceleration will ramp up with the specified gradient or jerk. Only active if k_f_fta_enable_brake_gradient_logic is enabled.*/
   float32_T k_fta_danger_zone_left_long[TA_K_FTA_DANGER_ZONE_LEFT_LONG_ARRAY_SIZE_DIM0]; /**<List of long coordinates of danger zone on ego left side.*/
   float32_T k_fta_danger_zone_left_lat[TA_K_FTA_DANGER_ZONE_LEFT_LAT_ARRAY_SIZE_DIM0]; /**<List of lat coordinates of danger zone on ego left side.*/
   float32_T k_fta_danger_zone_right_long[TA_K_FTA_DANGER_ZONE_RIGHT_LONG_ARRAY_SIZE_DIM0]; /**<List of long coordinates of danger zone on ego right side.*/
   float32_T k_fta_danger_zone_right_lat[TA_K_FTA_DANGER_ZONE_RIGHT_LAT_ARRAY_SIZE_DIM0]; /**<List of lat coordinates of danger zone on ego right side.*/
   float32_T k_pfgs_ego_speed[TA_K_PFGS_EGO_SPEED_ARRAY_SIZE_DIM0]; /**<PFGS only increases qualification counter for ego vehicle in the specified speed range.*/
   float32_T k_pfgs_qualification_ttc_min; /**<Minimum TTC value of most critical object to start qualification process for PFGS.*/
   float32_T k_tap_lvl_2_host_curvature_min; /**<For TAP warnings of level 2 and higher the host curvature must be greater than this parameter.*/
   float32_T k_rta_obj_exist_prblty[TA_K_RTA_OBJ_EXIST_PRBLTY_ARRAY_SIZE_DIM0]; /**<RTA puts only objects with an existence probability in the specified value range on its list of active objects.*/
   float32_T k_rta_obj_exist_prblty_ofst[TA_K_RTA_OBJ_EXIST_PRBLTY_OFST_ARRAY_SIZE_DIM0]; /**<RTA removes objects with an existence probability outside of the sum of the specified values and k_rta_obj_exist_prblty entries from its list of active objects.*/
   float32_T k_rta_obj_vcs_long_vel_rel[TA_K_RTA_OBJ_VCS_LONG_VEL_REL_ARRAY_SIZE_DIM0]; /**<RTA adds only objects with a relative vcs long velocity in the specified value range to its list of active objects.*/
   float32_T k_rta_obj_vcs_long_vel_rel_ofst[TA_K_RTA_OBJ_VCS_LONG_VEL_REL_OFST_ARRAY_SIZE_DIM0]; /**<RTA removes objects with a relative vcs long velocity outside of the sum of the specified values and k_rta_obj_vcs_long_vel_rel entries from its list of active objects.*/
   float32_T k_rta_obj_vcs_lat_vel_rel[TA_K_RTA_OBJ_VCS_LAT_VEL_REL_ARRAY_SIZE_DIM0]; /**<RTA adds only objects with a relative vcs lat velocity in the specified value range to its list of active objects.*/
   float32_T k_rta_obj_vcs_lat_vel_rel_ofst[TA_K_RTA_OBJ_VCS_LAT_VEL_REL_OFST_ARRAY_SIZE_DIM0]; /**<RTA removes objects with a relative vcs lat velocity outside of the sum of the specified values and k_rta_obj_vcs_lat_vel_rel entries from its list of active objects.*/
   float32_T k_rta_obj_vcs_long_vel[TA_K_RTA_OBJ_VCS_LONG_VEL_ARRAY_SIZE_DIM0]; /**<RTA adds only objects with a vcs long velocity in the specified value range to its list of active objects.*/
   float32_T k_rta_obj_vcs_long_vel_ofst[TA_K_RTA_OBJ_VCS_LONG_VEL_OFST_ARRAY_SIZE_DIM0]; /**<RTA removes objects with a vcs long velocity outside of the sum of the specified values and k_rta_obj_vcs_long_vel_rel entries from its list of active objects.*/
   float32_T k_rta_obj_vcs_lat_vel[TA_K_RTA_OBJ_VCS_LAT_VEL_ARRAY_SIZE_DIM0]; /**<RTA only adds objects with a vcs lat velocity in the specified value range to its list of active objects.*/
   float32_T k_rta_obj_vcs_lat_vel_ofst[TA_K_RTA_OBJ_VCS_LAT_VEL_OFST_ARRAY_SIZE_DIM0]; /**<RTA removes objects with a vcs lat velocity outside of the sum of the specified values and k_rta_obj_vcs_lat_vel_rel entries from its list of active objects.*/
   float32_T k_rta_obj_heading[TA_K_RTA_OBJ_HEADING_ARRAY_SIZE_DIM0]; /**<RTA only adds objects with an absolute heading in the specified value range on its list of active objects.*/
   float32_T k_rta_obj_heading_ofst[TA_K_RTA_OBJ_HEADING_OFST_ARRAY_SIZE_DIM0]; /**<RTA removes objects with an absolute heading outside of the sum of the specified values and k_rta_obj_heading entries from its list of active objects.*/
   float32_T k_rta_obj_speed[TA_K_RTA_OBJ_SPEED_ARRAY_SIZE_DIM0]; /**<RTA only adds objects with a speed in the specified value range on its list of active objects.*/
   float32_T k_rta_obj_speed_ofst[TA_K_RTA_OBJ_SPEED_OFST_ARRAY_SIZE_DIM0]; /**<RTA removes objects with a speed outside of the sum of the specified values and k_rta_obj_speed entries from its list of active objects.*/
   float32_T k_rta_obj_length[TA_K_RTA_OBJ_LENGTH_ARRAY_SIZE_DIM0]; /**<RTA only adds objects with a length in the specified value range on its list of active objects.*/
   float32_T k_rta_obj_length_ofst[TA_K_RTA_OBJ_LENGTH_OFST_ARRAY_SIZE_DIM0]; /**<RTA removes objects with a length outside of the sum of the specified values and k_rta_obj_length entries from its list of active objects.*/
   float32_T k_rta_obj_width[TA_K_RTA_OBJ_WIDTH_ARRAY_SIZE_DIM0]; /**<RTA only adds objects with a width in the specified value range on its list of active objects.*/
   float32_T k_rta_obj_width_ofst[TA_K_RTA_OBJ_WIDTH_OFST_ARRAY_SIZE_DIM0]; /**<RTA removes objects with a width outside of the sum of the specified values and k_rta_obj_width entries from its list of active objects.*/
   float32_T k_rta_obj_vru_class_prob[TA_K_RTA_OBJ_VRU_CLASS_PROB_ARRAY_SIZE_DIM0]; /**<RTA only adds objects with a VRU class probability in the specified value range on its list of active objects.*/
   float32_T k_rta_obj_vru_class_prob_ofst[TA_K_RTA_OBJ_VRU_CLASS_PROB_OFST_ARRAY_SIZE_DIM0]; /**<RTA removes objects with a VRU class probability outside of the sum of the specified values and k_rta_obj_vru_class_prob entries from its list of active objects.*/
   float32_T k_rta_obj_eclipse_value[TA_K_RTA_OBJ_ECLIPSE_VALUE_ARRAY_SIZE_DIM0]; /**<RTA only adds objects with an eclipse value in the specified value range on its list of active objects.*/
   float32_T k_rta_obj_eclipse_value_ofst[TA_K_RTA_OBJ_ECLIPSE_VALUE_OFST_ARRAY_SIZE_DIM0]; /**<RTA removes objects with an eclipse value outside of the sum of the specified values and k_rta_obj_eclipse_value entries from its list of active objects.*/
   float32_T k_rta_ttp_obj_abs_lat_vel_rel_max; /**<RTA only calculates a TTP value for objects that are not moving away laterally from the info zone.*/
   float32_T k_rta_ttp_obj_abs_heading_diff_max; /**<RTA only calculates a TTP value for objects that are not heading away from the info zone.*/
   float32_T k_rta_ttp_curve_suppression_obj_distance_min; /**<RTA only calculates a TTP value when the host vehicle is not driving straight for close objects or objects that have curvi info available.*/
   float32_T k_rta_info_zone_left_long[TA_K_RTA_INFO_ZONE_LEFT_LONG_ARRAY_SIZE_DIM0]; /**<List of long coordinates of info zone on ego left side.*/
   float32_T k_rta_info_zone_left_lat[TA_K_RTA_INFO_ZONE_LEFT_LAT_ARRAY_SIZE_DIM0]; /**<List of lat coordinates of info zone on ego left side.*/
   float32_T k_rta_info_zone_right_long[TA_K_RTA_INFO_ZONE_RIGHT_LONG_ARRAY_SIZE_DIM0]; /**<List of long coordinates of info zone on ego right side.*/
   float32_T k_rta_info_zone_right_lat[TA_K_RTA_INFO_ZONE_RIGHT_LAT_ARRAY_SIZE_DIM0]; /**<List of lat coordinates of info zone on ego right side.*/
   float32_T k_rta_info_zone_left_long_hys[TA_K_RTA_INFO_ZONE_LEFT_LONG_HYS_ARRAY_SIZE_DIM0]; /**<List of long coordinates of info zone addition on ego left side for hysteresis.*/
   float32_T k_rta_info_zone_left_lat_hys[TA_K_RTA_INFO_ZONE_LEFT_LAT_HYS_ARRAY_SIZE_DIM0]; /**<List of lat coordinates of info zone addition on ego left side for hysteresis.*/
   float32_T k_rta_info_zone_right_long_hys[TA_K_RTA_INFO_ZONE_RIGHT_LONG_HYS_ARRAY_SIZE_DIM0]; /**<List of long coordinates of info zone addition on ego right side for hysteresis.*/
   float32_T k_rta_info_zone_right_lat_hys[TA_K_RTA_INFO_ZONE_RIGHT_LAT_HYS_ARRAY_SIZE_DIM0]; /**<List of lat coordinates of info zone addition on ego right side for hysteresis.*/
   float32_T k_rta_wing_zone_left_long[TA_K_RTA_WING_ZONE_LEFT_LONG_ARRAY_SIZE_DIM0]; /**<List of long coordinates of wing zone on ego left side.*/
   float32_T k_rta_wing_zone_left_lat[TA_K_RTA_WING_ZONE_LEFT_LAT_ARRAY_SIZE_DIM0]; /**<List of lat coordinates of wing zone on ego left side.*/
   float32_T k_rta_wing_zone_right_long[TA_K_RTA_WING_ZONE_RIGHT_LONG_ARRAY_SIZE_DIM0]; /**<List of long coordinates of wing zone on ego right side.*/
   float32_T k_rta_wing_zone_right_lat[TA_K_RTA_WING_ZONE_RIGHT_LAT_ARRAY_SIZE_DIM0]; /**<List of lat coordinates of wing zone on ego right side.*/
   float32_T k_rta_wing_zone_left_long_hys[TA_K_RTA_WING_ZONE_LEFT_LONG_HYS_ARRAY_SIZE_DIM0]; /**<List of long coordinates of wing zone addition on ego left side for hysteresis.*/
   float32_T k_rta_wing_zone_left_lat_hys[TA_K_RTA_WING_ZONE_LEFT_LAT_HYS_ARRAY_SIZE_DIM0]; /**<List of lat coordinates of wing zone addition on ego left side for hysteresis.*/
   float32_T k_rta_wing_zone_right_long_hys[TA_K_RTA_WING_ZONE_RIGHT_LONG_HYS_ARRAY_SIZE_DIM0]; /**<List of long coordinates of wing zone addition on ego right side for hysteresis.*/
   float32_T k_rta_wing_zone_right_lat_hys[TA_K_RTA_WING_ZONE_RIGHT_LAT_HYS_ARRAY_SIZE_DIM0]; /**<List of lat coordinates of wing zone addition on ego right side for hysteresis.*/
   boolean_T k_ta_always_overwrite_ta_mode_to_both; /**<If any alert is raised, set TA mode to BOTH (dont distinguish between front/rear for qualifications).*/
   boolean_T k_ta_f_only_allow_consecutive_ttc_based_alert_levels; /**<Flag to only allow consecutive alerts. Applies to alert levels 3 and higher.*/
   boolean_T k_ta_f_skip_holding_for_single_alert_level_drop; /**<Flag to skip alert level holding if the drop is only to one level lower. Only affects drop from level 4.*/
   boolean_T k_ta_f_only_allow_ttc_based_alert_level_for_mature_obj; /**<Flag to only allow TTC based alerts for tracker objects with status MATURE. Applies to alert levels 2 and higher.*/
   boolean_T k_ta_f_apply_ttp_hysteresis_globally; /**<If active, k_ta_active_obj_ttp_offset is applied to all objects if a warning is currently raised. By default, k_ta_active_obj_ttp_offset is only applied to the currently warned object. */
   boolean_T k_f_ta_enable_debug_mode; /**<TA uses this flag in order to turn on/off the debug mode which enables object position manipulation.*/
   boolean_T k_f_fta_enable; /**<TA uses this flag in order to turn on/off the FTA subfunction.*/
   boolean_T k_f_fta_enable_brake_gradient_logic; /**<FTA considers the brake gradient or jerk if this flag is enabled. It uses the value from k_fta_brake_gradient.*/
   boolean_T k_f_fta_enable_danger_zones; /**<FTA uses this flag in order to turn on/off the danger zone area.*/
   boolean_T k_pfgs_symbol_request_sides_enabled; /**<Enable output signal to show side of critical object.*/
   boolean_T k_pfgs_qualification_check_f_stationary; /**<Enables the check for f_stationary. If active, only objects that are not stationary can increase the qualification counter.*/
   boolean_T k_f_rta_enable; /**<TA uses this flag in order to turn on/off the RTA subfunction.*/
   boolean_T k_f_rta_enable_info_zones; /**<RTA uses this flag in order to turn on/off the information zone area.*/
   boolean_T k_rta_f_higher_obj_crit_based_on_lower_ttp; /**<Objects that have a lower TTP will be evaluated as more critical. If disabled the criticality is based on closer distance to host vehicle.*/
   boolean_T k_f_rta_enable_wing_zones; /**<RTA uses this flag in order to turn on/off the wing zone area.*/
   uint8_t k_unused_padding_byte_0; /**<Padded byte for byte packing of 4*/
   uint8_t k_unused_padding_byte_1; /**<Padded byte for byte packing of 4*/
   uint8_t k_unused_padding_byte_2; /**<Padded byte for byte packing of 4*/
   uint8_t k_ta_alert_qualifying_cycles; /**<Number of cycle to qualify an alert level.*/
   uint8_t k_ta_alert_holding_cycles; /**<Number of cycle for holding an alert level.*/
   uint8_t k_ta_prediction_steps_max; /**<Maximum number of steps to predict trajectories of the ego and the relevant objects.*/
   uint8_t k_ta_critical_approach_check_ego_circles[TA_K_TA_CRITICAL_APPROACH_CHECK_EGO_CIRCLES_ARRAY_SIZE_DIM0]; /**<TA only checks the enabled ego circle path predictions. [front middle rear].*/
   uint8_t k_ta_ego_pred_const_velocity_pred_steps_min; /**<This value indicates at which prediction step to switch to a constant velocity prediction model when TA alert level 4 was active in previous cycle.*/
   uint8_t k_fta_obj_age_min; /**<FTA only adds objects with an object age greater than the specified threshold on its list of active objects.*/
   uint8_t k_fta_danger_zone_point_size; /**<Sets the number of points used to construct the danger zones.*/
   uint8_t k_pfgs_qualification_counter_fast_obj; /**<Minimum qualification counter value for PFGS alert and brake request. Threshold will be scaled based on object speed. Value 3 means in third relevant cycle.*/
   uint8_t k_pfgs_qualification_counter_slow_obj; /**<Maximum qualification counter value for PFGS alert and brake request. Threshold will be scaled based on object speed.*/
   uint8_t k_rta_obj_age_min; /**<RTA only adds objects with an object age greater than the specified threshold on its list of active objects.*/
   uint8_t k_rta_info_zone_point_size; /**<Sets the number of points used to construct the info zones.*/
   uint8_t k_rta_wing_zone_point_size; /**<Sets the number of points used to construct the wing zones.*/
} Ta_Core_Calibration_T;
#endif /* CT_BIG_ENDIAN */
#endif /* TA_CORE_CALIBRATION_T_H */
