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
#define LCDA_CUSTOMER_CALIBRATION_SIZE (64u)

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

#ifdef CT_BIG_ENDIAN
typedef struct
{
   /* Definition of structure for big endian */
   uint8_t k_unused_padding_byte_0; /**<Padded byte for byte packing of 4*/
   boolean_T k_honda_is_slide_through_zone_considered_for_cvw_alert_level_two; /**<Enables the Slide Through Zone for CVW alert level 2. Only if CVW object is fully in the zone between ego and BSW object, the level 2 of alert will be set.*/
   boolean_T k_honda_srr6_enable_alert_hold_due_out_of_fov; /**<Defines if the holding of Honda alert due to object lost in radar fov is turned on*/
   boolean_T k_honda_srr6_enable_alert_hold_due_slow_down; /**<Defines if the holding of Honda alert due to ego slowing down is turned on.*/
   float32_T k_lcda_honda_narrow_beeper_max_speed_l; /**<Maximum host speed below which narrow beeper zone will be used immediately*/
   float32_T k_lcda_honda_narrow_beeper_max_speed_h; /**<Maximum host speed above which narrow beeper zone will not be used*/
   float32_T k_honda_alert_level_two_holding_time; /**<Defines the duration of the Honda level 2 alert*/
   float32_T k_honda_beeper_zone_lat_hys; /**<Beeper zone lateral hysteresis*/
   float32_T k_honda_beeper_zone_long_hys; /**<Beeper zone longitudinal hysteresis*/
   float32_T k_honda_beeper_zone_width; /**<Width of the beeper zone*/
   float32_T k_honda_beeper_zone_length; /**<Length of the beeper zone*/
   float32_T k_honda_object_lat_overlap_slide_through_zone; /**<Distance in [m] of lateral Slide Through Zone overlap with BSW object vehicle.*/
   float32_T k_honda_ego_lat_overlap_slide_through_zone; /**<Distance in [m] of lateral Slide Through Zone overlap with ego vehicle.*/
   float32_T k_honda_min_relative_speed_for_alert_level_two; /**<Defines min relative speed between two objects above which honda alert level two can be triggered.*/
   float32_T k_honda_ego_speed_stop_holding; /**<Defines ego speed below which the alert holding is terminated.*/
   float32_T k_honda_max_hold_time_after_out_of_fov; /**<Defines max alert hold time in case if object h.*/
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
} Lcda_Customer_Calibration_T;
#else
typedef struct
{
   /* Definition of structure for little endian */
   Ct_Header_T Header; /**<Calibration tool internal type for general information*/
   float32_T k_honda_max_hold_time_after_out_of_fov; /**<Defines max alert hold time in case if object h.*/
   float32_T k_honda_ego_speed_stop_holding; /**<Defines ego speed below which the alert holding is terminated.*/
   float32_T k_honda_min_relative_speed_for_alert_level_two; /**<Defines min relative speed between two objects above which honda alert level two can be triggered.*/
   float32_T k_honda_ego_lat_overlap_slide_through_zone; /**<Distance in [m] of lateral Slide Through Zone overlap with ego vehicle.*/
   float32_T k_honda_object_lat_overlap_slide_through_zone; /**<Distance in [m] of lateral Slide Through Zone overlap with BSW object vehicle.*/
   float32_T k_honda_beeper_zone_length; /**<Length of the beeper zone*/
   float32_T k_honda_beeper_zone_width; /**<Width of the beeper zone*/
   float32_T k_honda_beeper_zone_long_hys; /**<Beeper zone longitudinal hysteresis*/
   float32_T k_honda_beeper_zone_lat_hys; /**<Beeper zone lateral hysteresis*/
   float32_T k_honda_alert_level_two_holding_time; /**<Defines the duration of the Honda level 2 alert*/
   float32_T k_lcda_honda_narrow_beeper_max_speed_h; /**<Maximum host speed above which narrow beeper zone will not be used*/
   float32_T k_lcda_honda_narrow_beeper_max_speed_l; /**<Maximum host speed below which narrow beeper zone will be used immediately*/
   boolean_T k_honda_srr6_enable_alert_hold_due_slow_down; /**<Defines if the holding of Honda alert due to ego slowing down is turned on.*/
   boolean_T k_honda_srr6_enable_alert_hold_due_out_of_fov; /**<Defines if the holding of Honda alert due to object lost in radar fov is turned on*/
   boolean_T k_honda_is_slide_through_zone_considered_for_cvw_alert_level_two; /**<Enables the Slide Through Zone for CVW alert level 2. Only if CVW object is fully in the zone between ego and BSW object, the level 2 of alert will be set.*/
   uint8_t k_unused_padding_byte_0; /**<Padded byte for byte packing of 4*/
} Lcda_Customer_Calibration_T;
#endif /* CT_BIG_ENDIAN */
#endif /* LCDA_CUSTOMER_CALIBRATION_T_H */
