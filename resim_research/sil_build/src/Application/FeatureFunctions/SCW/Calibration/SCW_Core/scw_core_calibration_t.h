# ifndef SCW_CORE_CALIBRATION_T_H
# define SCW_CORE_CALIBRATION_T_H

/**
* @file scw_core_calibration_t.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in scw_cal.xml.
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
#define SCW_K_SCW_CANDIDATE_HEADING_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_K_SCW_CANDIDATE_VELOCITY_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_K_SCW_CANDIDATE_RELATIVE_VELOCITY_ARRAY_SIZE_DIM0 (2u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_K_SCW_INITIAL_ZONE_X_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_K_SCW_INITIAL_ZONE_Y_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_K_SCW_HYS_ZONE_X_OFFSET_ARRAY_SIZE_DIM0 (6u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_K_SCW_HYS_ZONE_Y_OFFSET_ARRAY_SIZE_DIM0 (6u)

/* Macros for dimension size for all array variables */
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_K_SCW_CANDIDATE_HEADING_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_K_SCW_CANDIDATE_VELOCITY_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_K_SCW_CANDIDATE_RELATIVE_VELOCITY_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_K_SCW_INITIAL_ZONE_X_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_K_SCW_INITIAL_ZONE_Y_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_K_SCW_HYS_ZONE_X_OFFSET_ARRAY_DIM_SIZE (1u)
/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_K_SCW_HYS_ZONE_Y_OFFSET_ARRAY_DIM_SIZE (1u)


/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define SCW_CORE_CALIBRATION_SIZE (276u)

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

#ifdef CT_BIG_ENDIAN
typedef struct
{
   /* Definition of structure for big endian */
   uint8_t k_scw_guardrail_cycles_in_zone_threshold; /**<A guardrail is required to be in the SCW zone for at least this many consecutive cycles before it qualifies as critical object.*/
   uint8_t k_scw_candidate_mature_cycles_in_zone_threshold; /**<An object is required to be in the SCW zone with mature object status for at least this many cycles before it qualifies as critical object.*/
   uint8_t k_scw_min_candidate_age; /**<Minimum age of the object to be considered as a valid candidate for SCW.*/
   uint8_t k_scw_guardrail_freeze_period; /**<Lateral distance is frozen for k_scw_guardrail_freeze_period consecutive cycles if the guardrail is unplausible.*/
   uint8_t k_scw_min_guardrail_age; /**<Minimum age of the guardrail to be considered as valid for SCW.*/
   uint8_t k_unused_padding_byte_1; /**<Padded byte for byte packing of 4*/
   uint8_t k_unused_padding_byte_0; /**<Padded byte for byte packing of 4*/
   boolean_T k_scw_f_enable_trailer_ttc_extension; /**<Allows to increase of lateral ttc of a dynamic object or guardrail if a trailer is attached.*/
   boolean_T k_scw_f_enable_trailer_zone_extension; /**<Enables an additional adjustment of the SCW zone based on trailer information in core input.*/
   boolean_T k_scw_f_enable_guardrail; /**<Decides whether subfunction SCW guardrail shall be enabled or disabled if CAF input is not used.*/
   boolean_T k_scw_f_enable_dynamic; /**<Decides whether subfunction SCW dynamic objects shall be enabled or disabled if CAF input is not used.*/
   boolean_T k_scw_f_enable; /**<Decides whether SCW shall be enabled or disabled if CAF input is not used.*/
   boolean_T k_scw_f_guardrail_enable_via_cal; /**<Decides whether vehicle input or cal shall be used to enable/disable subfunction SCW guardrail.*/
   boolean_T k_scw_f_dynamic_enable_via_cal; /**<Decides whether vehicle input or cal shall be used to enable/disable subfunction SCW dynamic objects.*/
   boolean_T k_scw_f_enable_via_cal; /**<Decides whether vehicle input or cal shall be used to enable/disable SCW.*/
   boolean_T k_scw_f_adjust_zones_to_ego_size; /**<If enabled the SCW zones defined by k_scw_initial_zone_x and k_scw_initial_zone_y will be relative to the host vehicle side and will be extended by the host vehicle length. */
   float32_T k_scw_max_zone_width; /**<Maximum width of the SCW zone.*/
   float32_T k_scw_max_zone_length; /**<Maximum length of the SCW zone.*/
   float32_T k_scw_trailer_zone_ext_safety_margin_lat; /**<Lateral extension of the SCW zone.*/
   float32_T k_scw_trailer_zone_ext_safety_margin; /**<Longitudinal extension of the rear edge of the SCW zone behind the trailer edge.*/
   float32_T k_scw_ttp_default; /**<Default value of TTP, usually equal to the max value.*/
   float32_T k_scw_ttp_max; /**<Max value of TTP.*/
   float32_T k_scw_ttle_default; /**<Default value of TTLE, usually equal to the max value.*/
   float32_T k_scw_ttle_max; /**<Max value of TTLE.*/
   float32_T k_scw_lateral_ttc_default; /**<Default value of lateral TTC, usually equal to the max value.*/
   float32_T k_scw_lateral_ttc_max; /**<Max value of lateral TTC.*/
   float32_T k_scw_lateral_distance_default; /**<Max value of lateral distance, usually equal to the max value.*/
   float32_T k_scw_hys_zone_y_offset[SCW_K_SCW_HYS_ZONE_Y_OFFSET_ARRAY_SIZE_DIM0]; /**<Offsets to be added to the y-coordinate of the initial zone to get the hysteresis zone.*/
   float32_T k_scw_hys_zone_x_offset[SCW_K_SCW_HYS_ZONE_X_OFFSET_ARRAY_SIZE_DIM0]; /**<Offsets to be added to the x-coordinate of the initial zone to get the hysteresis zone.*/
   float32_T k_scw_initial_zone_y[SCW_K_SCW_INITIAL_ZONE_Y_ARRAY_SIZE_DIM0]; /**<Y-coordinate of corners of polygon defining SCW zone of the right side. First coordinate should be right front corner of zone, subsequent coordinates should be clockwise.*/
   float32_T k_scw_initial_zone_x[SCW_K_SCW_INITIAL_ZONE_X_ARRAY_SIZE_DIM0]; /**<X-coordinate of corners of polygon defining SCW zone of the right side. First coordinate should be right front corner of zone, subsequent coordinates should be clockwise.*/
   float32_T k_scw_critical_lat_distance_hys; /**<The critical lateral distance hysteresis. */
   float32_T k_scw_critical_lat_ttc_hys; /**<The critical lateral ttc hysteresis. */
   float32_T k_scw_max_guardrail_lat_distance; /**<The maximum critical lateral distance to the guardrail to trigger an alert. */
   float32_T k_scw_min_guardrail_lat_distance; /**<The minimum critical lateral distance to the guardrail to trigger an alert. */
   float32_T k_scw_max_dynamic_lat_distance; /**<The maximum critical lateral distance to the dynamic object to trigger an alert. */
   float32_T k_scw_min_dynamic_lat_distance; /**<The minimum critical lateral distance to the dynamic object to trigger an alert. */
   float32_T k_scw_trailer_lat_ttc_extension; /**<The extension of the min lateral TTC when the trailer is attached. */
   float32_T k_scw_max_guardrail_lat_ttc; /**<The maximum critical lateral TTC of the guardrail to trigger an alert. */
   float32_T k_scw_min_guardrail_lat_ttc; /**<The minimum critical lateral TTC of the guardrail to trigger an alert. */
   float32_T k_scw_max_dynamic_lat_ttc; /**<The maximum critical lateral TTC of the dynamic object to trigger an alert. */
   float32_T k_scw_min_dynamic_lat_ttc; /**<The minimum critical lateral TTC of the dynamic object to trigger an alert. */
   float32_T k_scw_min_exist_prob_radar_guardrail; /**<The minimum existence probability of a radar guardrail to determine, if an SCW candidate is in conflict with its environment.*/
   float32_T k_scw_min_candidate_existence_probability; /**<Minimum existence probability of the object to be considered as a valid candidate for SCW.*/
   float32_T k_scw_candidate_relative_vel_hys; /**<Value subtracted from min relative longitudinal velocity threshold or added to the max threshold, to apply hysteresis for the SCW candidate.*/
   float32_T k_scw_candidate_relative_velocity[SCW_K_SCW_CANDIDATE_RELATIVE_VELOCITY_ARRAY_SIZE_DIM0]; /**<Only objects with relative longitudinal velocity in the specified value range are considered SCW candidates.*/
   float32_T k_scw_candidate_velocity_hys; /**<Value subtracted from minimum relative velocity or added to maximum relative velocity, to apply hysteresis for the SCW candidate.*/
   float32_T k_scw_candidate_velocity[SCW_K_SCW_CANDIDATE_VELOCITY_ARRAY_SIZE_DIM0]; /**<Only objects with an absolute velocity in the specified value range are considered SCW candidates.*/
   float32_T k_scw_candidate_yawrate_hys; /**<Value added to maximum yawrate, to apply hysteresis for the SCW candidate.*/
   float32_T k_scw_candidate_yawrate; /**<Only objects with yawrate in the specified value range are considered SCW candidates.*/
   float32_T k_scw_candidate_heading_hys; /**<Value subtracted from minimum heading or added to maximum heading, to apply hysteresis for the SCW candidate.*/
   float32_T k_scw_candidate_heading[SCW_K_SCW_CANDIDATE_HEADING_ARRAY_SIZE_DIM0]; /**<Only objects with heading in the specified value range are considered SCW candidates.*/
   float32_T k_scw_max_lat_pos_ratio; /**<Maximum ratio between current and previous values of guardrail lateral distance. Lateral distance is frozen for k_scw_guardrail_freeze_period consecutive cycles above this threshold. */
   float32_T k_scw_min_host_speed_hys; /**<Value subtracted from the k_scw_min_host_speed to apply hysteresis for the activation of SCW function.*/
   float32_T k_scw_min_host_speed; /**<Minimum ego speed at which SCW feature is enabled.*/
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
} Scw_Core_Calibration_T;
#else
typedef struct
{
   /* Definition of structure for little endian */
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
   float32_T k_scw_min_host_speed; /**<Minimum ego speed at which SCW feature is enabled.*/
   float32_T k_scw_min_host_speed_hys; /**<Value subtracted from the k_scw_min_host_speed to apply hysteresis for the activation of SCW function.*/
   float32_T k_scw_max_lat_pos_ratio; /**<Maximum ratio between current and previous values of guardrail lateral distance. Lateral distance is frozen for k_scw_guardrail_freeze_period consecutive cycles above this threshold. */
   float32_T k_scw_candidate_heading[SCW_K_SCW_CANDIDATE_HEADING_ARRAY_SIZE_DIM0]; /**<Only objects with heading in the specified value range are considered SCW candidates.*/
   float32_T k_scw_candidate_heading_hys; /**<Value subtracted from minimum heading or added to maximum heading, to apply hysteresis for the SCW candidate.*/
   float32_T k_scw_candidate_yawrate; /**<Only objects with yawrate in the specified value range are considered SCW candidates.*/
   float32_T k_scw_candidate_yawrate_hys; /**<Value added to maximum yawrate, to apply hysteresis for the SCW candidate.*/
   float32_T k_scw_candidate_velocity[SCW_K_SCW_CANDIDATE_VELOCITY_ARRAY_SIZE_DIM0]; /**<Only objects with an absolute velocity in the specified value range are considered SCW candidates.*/
   float32_T k_scw_candidate_velocity_hys; /**<Value subtracted from minimum relative velocity or added to maximum relative velocity, to apply hysteresis for the SCW candidate.*/
   float32_T k_scw_candidate_relative_velocity[SCW_K_SCW_CANDIDATE_RELATIVE_VELOCITY_ARRAY_SIZE_DIM0]; /**<Only objects with relative longitudinal velocity in the specified value range are considered SCW candidates.*/
   float32_T k_scw_candidate_relative_vel_hys; /**<Value subtracted from min relative longitudinal velocity threshold or added to the max threshold, to apply hysteresis for the SCW candidate.*/
   float32_T k_scw_min_candidate_existence_probability; /**<Minimum existence probability of the object to be considered as a valid candidate for SCW.*/
   float32_T k_scw_min_exist_prob_radar_guardrail; /**<The minimum existence probability of a radar guardrail to determine, if an SCW candidate is in conflict with its environment.*/
   float32_T k_scw_min_dynamic_lat_ttc; /**<The minimum critical lateral TTC of the dynamic object to trigger an alert. */
   float32_T k_scw_max_dynamic_lat_ttc; /**<The maximum critical lateral TTC of the dynamic object to trigger an alert. */
   float32_T k_scw_min_guardrail_lat_ttc; /**<The minimum critical lateral TTC of the guardrail to trigger an alert. */
   float32_T k_scw_max_guardrail_lat_ttc; /**<The maximum critical lateral TTC of the guardrail to trigger an alert. */
   float32_T k_scw_trailer_lat_ttc_extension; /**<The extension of the min lateral TTC when the trailer is attached. */
   float32_T k_scw_min_dynamic_lat_distance; /**<The minimum critical lateral distance to the dynamic object to trigger an alert. */
   float32_T k_scw_max_dynamic_lat_distance; /**<The maximum critical lateral distance to the dynamic object to trigger an alert. */
   float32_T k_scw_min_guardrail_lat_distance; /**<The minimum critical lateral distance to the guardrail to trigger an alert. */
   float32_T k_scw_max_guardrail_lat_distance; /**<The maximum critical lateral distance to the guardrail to trigger an alert. */
   float32_T k_scw_critical_lat_ttc_hys; /**<The critical lateral ttc hysteresis. */
   float32_T k_scw_critical_lat_distance_hys; /**<The critical lateral distance hysteresis. */
   float32_T k_scw_initial_zone_x[SCW_K_SCW_INITIAL_ZONE_X_ARRAY_SIZE_DIM0]; /**<X-coordinate of corners of polygon defining SCW zone of the right side. First coordinate should be right front corner of zone, subsequent coordinates should be clockwise.*/
   float32_T k_scw_initial_zone_y[SCW_K_SCW_INITIAL_ZONE_Y_ARRAY_SIZE_DIM0]; /**<Y-coordinate of corners of polygon defining SCW zone of the right side. First coordinate should be right front corner of zone, subsequent coordinates should be clockwise.*/
   float32_T k_scw_hys_zone_x_offset[SCW_K_SCW_HYS_ZONE_X_OFFSET_ARRAY_SIZE_DIM0]; /**<Offsets to be added to the x-coordinate of the initial zone to get the hysteresis zone.*/
   float32_T k_scw_hys_zone_y_offset[SCW_K_SCW_HYS_ZONE_Y_OFFSET_ARRAY_SIZE_DIM0]; /**<Offsets to be added to the y-coordinate of the initial zone to get the hysteresis zone.*/
   float32_T k_scw_lateral_distance_default; /**<Max value of lateral distance, usually equal to the max value.*/
   float32_T k_scw_lateral_ttc_max; /**<Max value of lateral TTC.*/
   float32_T k_scw_lateral_ttc_default; /**<Default value of lateral TTC, usually equal to the max value.*/
   float32_T k_scw_ttle_max; /**<Max value of TTLE.*/
   float32_T k_scw_ttle_default; /**<Default value of TTLE, usually equal to the max value.*/
   float32_T k_scw_ttp_max; /**<Max value of TTP.*/
   float32_T k_scw_ttp_default; /**<Default value of TTP, usually equal to the max value.*/
   float32_T k_scw_trailer_zone_ext_safety_margin; /**<Longitudinal extension of the rear edge of the SCW zone behind the trailer edge.*/
   float32_T k_scw_trailer_zone_ext_safety_margin_lat; /**<Lateral extension of the SCW zone.*/
   float32_T k_scw_max_zone_length; /**<Maximum length of the SCW zone.*/
   float32_T k_scw_max_zone_width; /**<Maximum width of the SCW zone.*/
   boolean_T k_scw_f_adjust_zones_to_ego_size; /**<If enabled the SCW zones defined by k_scw_initial_zone_x and k_scw_initial_zone_y will be relative to the host vehicle side and will be extended by the host vehicle length. */
   boolean_T k_scw_f_enable_via_cal; /**<Decides whether vehicle input or cal shall be used to enable/disable SCW.*/
   boolean_T k_scw_f_dynamic_enable_via_cal; /**<Decides whether vehicle input or cal shall be used to enable/disable subfunction SCW dynamic objects.*/
   boolean_T k_scw_f_guardrail_enable_via_cal; /**<Decides whether vehicle input or cal shall be used to enable/disable subfunction SCW guardrail.*/
   boolean_T k_scw_f_enable; /**<Decides whether SCW shall be enabled or disabled if CAF input is not used.*/
   boolean_T k_scw_f_enable_dynamic; /**<Decides whether subfunction SCW dynamic objects shall be enabled or disabled if CAF input is not used.*/
   boolean_T k_scw_f_enable_guardrail; /**<Decides whether subfunction SCW guardrail shall be enabled or disabled if CAF input is not used.*/
   boolean_T k_scw_f_enable_trailer_zone_extension; /**<Enables an additional adjustment of the SCW zone based on trailer information in core input.*/
   boolean_T k_scw_f_enable_trailer_ttc_extension; /**<Allows to increase of lateral ttc of a dynamic object or guardrail if a trailer is attached.*/
   uint8_t k_unused_padding_byte_0; /**<Padded byte for byte packing of 4*/
   uint8_t k_unused_padding_byte_1; /**<Padded byte for byte packing of 4*/
   uint8_t k_scw_min_guardrail_age; /**<Minimum age of the guardrail to be considered as valid for SCW.*/
   uint8_t k_scw_guardrail_freeze_period; /**<Lateral distance is frozen for k_scw_guardrail_freeze_period consecutive cycles if the guardrail is unplausible.*/
   uint8_t k_scw_min_candidate_age; /**<Minimum age of the object to be considered as a valid candidate for SCW.*/
   uint8_t k_scw_candidate_mature_cycles_in_zone_threshold; /**<An object is required to be in the SCW zone with mature object status for at least this many cycles before it qualifies as critical object.*/
   uint8_t k_scw_guardrail_cycles_in_zone_threshold; /**<A guardrail is required to be in the SCW zone for at least this many consecutive cycles before it qualifies as critical object.*/
} Scw_Core_Calibration_T;
#endif /* CT_BIG_ENDIAN */
#endif /* SCW_CORE_CALIBRATION_T_H */
