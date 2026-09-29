# ifndef LCDA_CUSTOMER_CALIBRATION_T_H
# define LCDA_CUSTOMER_CALIBRATION_T_H

/**
* @file lcda_customer_calibration_t.h
* @author SFL (Side Feature Logic) scrum team
* @brief Provides the declaration of the calibrations defined in lcda_cal.xml.
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

/* Macros for dimension size for all array variables */


/* coverity[misra_c_2012_rule_2_5_violation][Macro definition shall be available for external usage] */
#define LCDA_CUSTOMER_CALIBRATION_SIZE (60u)

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

#ifdef CT_BIG_ENDIAN
typedef struct
{
   /* Definition of structure for big endian */
   uint8_t k_bmw_sp25_camera_lane_plausibilisation_counter_max; /**<Defines the maximum value for the plausibilisation counters.*/
   uint8_t k_bmw_sp25_lane_change_counter_max; /**<Defines the maximum number of cycles to reset the lane change flag.*/
   uint8_t k_bmw_sp25_lane_change_counter_min; /**<Defines the minimum number of cycles to set the lane change flag.*/
   uint8_t k_bmw_sp25_max_bad_guardrail_holding_counter; /**<Counter to hold previous camera input signals for guardrail calculation in case of bad quality.*/
   uint8_t k_bmw_sp25_guardrail_age_stage_thresh; /**<Minimum Guardrail age to be considered by LCDA */
   uint8_t k_unused_padding_byte_0; /**<Padded byte for byte packing of 4*/
   boolean_T k_bmw_sp25_f_enable_lane_change_detection; /**<Enables the lane change detection functionality.*/
   boolean_T k_bmw_sp25_smooth_camera_signals; /**<The BMW camera input signals (distance and probability) are smoothed for the guardrail calculation if activated.*/
   float32_T k_bmw_sp25_camera_lane_plausibilisation_exist_prob_min; /**<Defines the minimum lane probability to count up plausibilisation counters.*/
   float32_T k_bmw_sp25_lane_change_dist_to_laneline_max; /**<Defines the threshold distance to a lineline for triggering the lane change counter.*/
   float32_T k_bmw_sp25_lane_change_detection_host_speed_min; /**<Defines the minimum host vehicle speed to activate the lane change detection.*/
   float32_T k_bmw_sp25_trailer_mode_max_bike_carrier_buffer; /**<Define the maximum lateral offset between ego outer edge and center of bike carrier object for BSW alert suppression when the trailer mode is active.*/
   float32_T k_bmw_sp25_trailer_mode_max_bike_carrier_distance; /**<Defines the maximum expected distance of the bike carrier to the ego rear for BSW alert suppression when the trailer mode is active.*/
   float32_T k_bmw_sp25_trailer_mode_max_trailer_length; /**<Defines the maximum expected trailer length for BSW alert suppression when the trailer mode is active.*/
   float32_T k_bmw_sp25_lowest_probabilty_percentage_cal_for_adjustment; /**<Definition of the lowest camera input probabiliy. The damping for the guardrail calculation is linearly interpolated between this value and 1. Values below this will be cut off.*/
   float32_T k_bmw_sp25_cvw_limit_zone_range_hys; /**<Hysteresis for BMW-specific CVW zone limit for China. Distance here describes object front to host vehicle rear. Hysteresis is added to input value lcda_cvw_limit_zone_range.*/
   float32_T k_bmw_sp25_exist_prob_lc_intention; /**<Boundary of existence probability which is returned when lane change intention is given*/
   float32_T k_bmw_sp25_guardrail_rel_diff_thresh; /**<Relative difference between lateral positions (recent and previous one) which indicates if guardrail is valid. */
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
} Lcda_Customer_Calibration_T;
#else
typedef struct
{
   /* Definition of structure for little endian */
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
   float32_T k_bmw_sp25_guardrail_rel_diff_thresh; /**<Relative difference between lateral positions (recent and previous one) which indicates if guardrail is valid. */
   float32_T k_bmw_sp25_exist_prob_lc_intention; /**<Boundary of existence probability which is returned when lane change intention is given*/
   float32_T k_bmw_sp25_cvw_limit_zone_range_hys; /**<Hysteresis for BMW-specific CVW zone limit for China. Distance here describes object front to host vehicle rear. Hysteresis is added to input value lcda_cvw_limit_zone_range.*/
   float32_T k_bmw_sp25_lowest_probabilty_percentage_cal_for_adjustment; /**<Definition of the lowest camera input probabiliy. The damping for the guardrail calculation is linearly interpolated between this value and 1. Values below this will be cut off.*/
   float32_T k_bmw_sp25_trailer_mode_max_trailer_length; /**<Defines the maximum expected trailer length for BSW alert suppression when the trailer mode is active.*/
   float32_T k_bmw_sp25_trailer_mode_max_bike_carrier_distance; /**<Defines the maximum expected distance of the bike carrier to the ego rear for BSW alert suppression when the trailer mode is active.*/
   float32_T k_bmw_sp25_trailer_mode_max_bike_carrier_buffer; /**<Define the maximum lateral offset between ego outer edge and center of bike carrier object for BSW alert suppression when the trailer mode is active.*/
   float32_T k_bmw_sp25_lane_change_detection_host_speed_min; /**<Defines the minimum host vehicle speed to activate the lane change detection.*/
   float32_T k_bmw_sp25_lane_change_dist_to_laneline_max; /**<Defines the threshold distance to a lineline for triggering the lane change counter.*/
   float32_T k_bmw_sp25_camera_lane_plausibilisation_exist_prob_min; /**<Defines the minimum lane probability to count up plausibilisation counters.*/
   boolean_T k_bmw_sp25_smooth_camera_signals; /**<The BMW camera input signals (distance and probability) are smoothed for the guardrail calculation if activated.*/
   boolean_T k_bmw_sp25_f_enable_lane_change_detection; /**<Enables the lane change detection functionality.*/
   uint8_t k_unused_padding_byte_0; /**<Padded byte for byte packing of 4*/
   uint8_t k_bmw_sp25_guardrail_age_stage_thresh; /**<Minimum Guardrail age to be considered by LCDA */
   uint8_t k_bmw_sp25_max_bad_guardrail_holding_counter; /**<Counter to hold previous camera input signals for guardrail calculation in case of bad quality.*/
   uint8_t k_bmw_sp25_lane_change_counter_min; /**<Defines the minimum number of cycles to set the lane change flag.*/
   uint8_t k_bmw_sp25_lane_change_counter_max; /**<Defines the maximum number of cycles to reset the lane change flag.*/
   uint8_t k_bmw_sp25_camera_lane_plausibilisation_counter_max; /**<Defines the maximum value for the plausibilisation counters.*/
} Lcda_Customer_Calibration_T;
#endif /* CT_BIG_ENDIAN */
#endif /* LCDA_CUSTOMER_CALIBRATION_T_H */
