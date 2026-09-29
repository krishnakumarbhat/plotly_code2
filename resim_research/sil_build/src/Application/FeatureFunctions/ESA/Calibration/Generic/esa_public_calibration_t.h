# ifndef ESA_PUBLIC_CALIBRATION_T_H
# define ESA_PUBLIC_CALIBRATION_T_H

/**
* @file esa_public_calibration_t.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in esa_cal.xml.
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
#define ESA_K_ESA_ZONE_X_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_K_ESA_ZONE_X_HYS_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_K_ESA_ZONE_Y_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_K_ESA_ZONE_Y_HYS_ARRAY_SIZE_DIM0 (6u)

/* Macros for dimension size for all array variables */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_K_ESA_ZONE_X_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_K_ESA_ZONE_X_HYS_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_K_ESA_ZONE_Y_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_K_ESA_ZONE_Y_HYS_ARRAY_DIM_SIZE (1u)


/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define ESA_PUBLIC_CALIBRATION_SIZE (182u)

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

#ifdef CT_BIG_ENDIAN
typedef struct
{
   /* Definition of structure for big endian */
   uint8_t k_esa_alert_holding_cycles; /**<Number of holding cycles for ESA alert.*/
   uint8_t k_esa_min_mature_cycles; /**<When the mature count in zone exceeds this threshold the object is allowed to create an ESA alert.*/
   uint8_t k_esa_min_track_age; /**<Minimum Track age for an object to be selected as a valid candidate for ESA.*/
   boolean_T k_esa_f_allow_obj_selection_long_distance; /**<Allow longitudinal distance to the Host criterion for most critical object selection*/
   boolean_T k_esa_f_allow_obj_selection_deceleration; /**<Allow deceleration criterion for most critical object selection*/
   boolean_T k_esa_f_allow_obj_selection_ttc; /**<Allow TTC criterion for most critical object selection*/
   boolean_T k_esa_f_allow_obj_critical_ttc_and_deceleration; /**<Allow TTC and deceleration criteria for object criticality assessment*/
   boolean_T k_esa_f_allow_min_curve_radius; /**<Decides whether ESA functionality should be disabled when the host is moving with a very small curve radius such as a round about.*/
   boolean_T k_esa_f_enable; /**<Enables the Emergency Steering Assist feature when the k_esa_enable_via_cal is also set to TRUE.*/
   boolean_T k_esa_f_enable_via_cal; /**<Used to decide whether Emergency Steering Assist should be enabled based on the calibration. If TRUE then k_esa_enable is used to enable ESA, if FALSE then ESAEnabled flag from the core input is used to enable ESA.*/
   float32_T k_esa_host_activation_speed_max_hys; /**<Hysteresis for k_esa_host_activation_speed_min_max. This value will be added to maximal activation speed threshold if ESA is currently active.*/
   float32_T k_esa_host_activation_speed_max; /**<Maximal host speed for which ESA shall be active.*/
   float32_T k_esa_host_activation_speed_min_hys; /**<Hysteresis for k_esa_host_activation_speed_min_min. This value will be subtracted from minimal activation speed threshold if ESA is currently active.*/
   float32_T k_esa_host_activation_speed_min; /**<Minimal host speed for which ESA shall be active.*/
   float32_T k_esa_obj_safe_deceleration_threshold_hys; /**<Deceleration threshold above which a target needs to stay to remain critical for ESA.*/
   float32_T k_esa_obj_safe_deceleration_threshold; /**<Deceleration threshold above which a target is considered as critical for ESA.*/
   float32_T k_esa_critical_longitudinal_ttc_hys; /**<Longitudinal TTC threshold below which a target needs to stay to remain critical for ESA.*/
   float32_T k_esa_critical_longitudinal_ttc; /**<Longitudinal TTC threshold below which a target is considered as critical for ESA.*/
   float32_T k_esa_min_obj_curvi_long_vel_abs; /**<The minimum longitudinal absolute curvi velocity of the object to be considered as a valid candidate for Emergency Steering Assist.*/
   float32_T k_esa_max_curvi_heading_abs; /**<The maximum allowed absolute curvi heading for objects to be considered as valid Emergency Steering Assist candidates.*/
   float32_T k_esa_min_curve_radius_hys; /**<Hysteresis value to be applied before ESA is disabled for small curve radius.*/
   float32_T k_esa_min_curve_radius; /**<ESA is disabled when the host is navigating a curve with a radius smaller than this value.*/
   float32_T k_esa_min_exist_prob; /**<Min existence probability for Tracks*/
   float32_T k_esa_max_lane_width; /**<Maximum lane width - used for plausibility checking during zone creation. If input lane width is larger than this, this max lane width is used.*/
   float32_T k_esa_min_lane_width; /**<Minimum lane width - used for plausibility checking during zone creation. If input lane width is lower than this, this value width is used.*/
   float32_T k_esa_max_range; /**<Maximum Range of a track that can trigger a warning.*/
   float32_T k_esa_zone_y_hys[ESA_K_ESA_ZONE_Y_HYS_ARRAY_SIZE_DIM0]; /**<Offset that is to be added to the initial ESA zone y-coord to get the hysteresis zone y-coord. Hys zone is always larger than the initial zone.*/
   float32_T k_esa_zone_y[ESA_K_ESA_ZONE_Y_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining the ESA zone in curvi coordinates. It is specified as a factor of the lane width.*/
   float32_T k_esa_zone_x_hys[ESA_K_ESA_ZONE_X_HYS_ARRAY_SIZE_DIM0]; /**<Offset that is to be added to the initial ESA zone x-coord to get the hysteresis zone x-coord. Hys zone is always larger than the initial zone.*/
   float32_T k_esa_zone_x[ESA_K_ESA_ZONE_X_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining Emergency Steering Assist zone in curvi coordinates.*/
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
} Esa_Public_Calibration_T;
#else
typedef struct
{
   /* Definition of structure for little endian */
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
   float32_T k_esa_zone_x[ESA_K_ESA_ZONE_X_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining Emergency Steering Assist zone in curvi coordinates.*/
   float32_T k_esa_zone_x_hys[ESA_K_ESA_ZONE_X_HYS_ARRAY_SIZE_DIM0]; /**<Offset that is to be added to the initial ESA zone x-coord to get the hysteresis zone x-coord. Hys zone is always larger than the initial zone.*/
   float32_T k_esa_zone_y[ESA_K_ESA_ZONE_Y_ARRAY_SIZE_DIM0]; /**<Corners of polygon defining the ESA zone in curvi coordinates. It is specified as a factor of the lane width.*/
   float32_T k_esa_zone_y_hys[ESA_K_ESA_ZONE_Y_HYS_ARRAY_SIZE_DIM0]; /**<Offset that is to be added to the initial ESA zone y-coord to get the hysteresis zone y-coord. Hys zone is always larger than the initial zone.*/
   float32_T k_esa_max_range; /**<Maximum Range of a track that can trigger a warning.*/
   float32_T k_esa_min_lane_width; /**<Minimum lane width - used for plausibility checking during zone creation. If input lane width is lower than this, this value width is used.*/
   float32_T k_esa_max_lane_width; /**<Maximum lane width - used for plausibility checking during zone creation. If input lane width is larger than this, this max lane width is used.*/
   float32_T k_esa_min_exist_prob; /**<Min existence probability for Tracks*/
   float32_T k_esa_min_curve_radius; /**<ESA is disabled when the host is navigating a curve with a radius smaller than this value.*/
   float32_T k_esa_min_curve_radius_hys; /**<Hysteresis value to be applied before ESA is disabled for small curve radius.*/
   float32_T k_esa_max_curvi_heading_abs; /**<The maximum allowed absolute curvi heading for objects to be considered as valid Emergency Steering Assist candidates.*/
   float32_T k_esa_min_obj_curvi_long_vel_abs; /**<The minimum longitudinal absolute curvi velocity of the object to be considered as a valid candidate for Emergency Steering Assist.*/
   float32_T k_esa_critical_longitudinal_ttc; /**<Longitudinal TTC threshold below which a target is considered as critical for ESA.*/
   float32_T k_esa_critical_longitudinal_ttc_hys; /**<Longitudinal TTC threshold below which a target needs to stay to remain critical for ESA.*/
   float32_T k_esa_obj_safe_deceleration_threshold; /**<Deceleration threshold above which a target is considered as critical for ESA.*/
   float32_T k_esa_obj_safe_deceleration_threshold_hys; /**<Deceleration threshold above which a target needs to stay to remain critical for ESA.*/
   float32_T k_esa_host_activation_speed_min; /**<Minimal host speed for which ESA shall be active.*/
   float32_T k_esa_host_activation_speed_min_hys; /**<Hysteresis for k_esa_host_activation_speed_min_min. This value will be subtracted from minimal activation speed threshold if ESA is currently active.*/
   float32_T k_esa_host_activation_speed_max; /**<Maximal host speed for which ESA shall be active.*/
   float32_T k_esa_host_activation_speed_max_hys; /**<Hysteresis for k_esa_host_activation_speed_min_max. This value will be added to maximal activation speed threshold if ESA is currently active.*/
   boolean_T k_esa_f_enable_via_cal; /**<Used to decide whether Emergency Steering Assist should be enabled based on the calibration. If TRUE then k_esa_enable is used to enable ESA, if FALSE then ESAEnabled flag from the core input is used to enable ESA.*/
   boolean_T k_esa_f_enable; /**<Enables the Emergency Steering Assist feature when the k_esa_enable_via_cal is also set to TRUE.*/
   boolean_T k_esa_f_allow_min_curve_radius; /**<Decides whether ESA functionality should be disabled when the host is moving with a very small curve radius such as a round about.*/
   boolean_T k_esa_f_allow_obj_critical_ttc_and_deceleration; /**<Allow TTC and deceleration criteria for object criticality assessment*/
   boolean_T k_esa_f_allow_obj_selection_ttc; /**<Allow TTC criterion for most critical object selection*/
   boolean_T k_esa_f_allow_obj_selection_deceleration; /**<Allow deceleration criterion for most critical object selection*/
   boolean_T k_esa_f_allow_obj_selection_long_distance; /**<Allow longitudinal distance to the Host criterion for most critical object selection*/
   uint8_t k_esa_min_track_age; /**<Minimum Track age for an object to be selected as a valid candidate for ESA.*/
   uint8_t k_esa_min_mature_cycles; /**<When the mature count in zone exceeds this threshold the object is allowed to create an ESA alert.*/
   uint8_t k_esa_alert_holding_cycles; /**<Number of holding cycles for ESA alert.*/
} Esa_Public_Calibration_T;
#endif /* CT_BIG_ENDIAN */
#endif /* ESA_PUBLIC_CALIBRATION_T_H */
