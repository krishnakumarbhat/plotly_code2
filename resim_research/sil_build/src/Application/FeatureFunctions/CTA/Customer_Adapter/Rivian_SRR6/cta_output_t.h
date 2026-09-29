#ifndef CTA_OUTPUT_T_H
#define CTA_OUTPUT_T_H

/**
 * @file cta_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Rivian_SRR6 customer output declaration.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

typedef enum
{
   RIVIAN_CTA_ACTIVE                = (0), /**< CTA is in active state */
   RIVIAN_CTA_DISABLED              = (1), /**< CTA is disabled by calibration */
   RIVIAN_CTA_DEACTIVATED_EGO_SPEED = (2)  /**< CTA is disabled by host speed */
} Cta_Rivian_Status_T;

typedef struct
{
   float32_T front_cta_obj_ttc_left;            /**< Front CTA object TTC for left side */
   float32_T front_cta_obj_ttc_right;           /**< Front CTA object TTC for right side */
   uint8_t front_cta_alert_level_left;          /**< Front CTA alert level for left side */
   uint8_t front_cta_alert_level_right;         /**< Front CTA alert level for right side */
   uint8_t front_cta_id_left;                   /**< Front CTA object ID for left side */
   uint8_t front_cta_id_right;                  /**< Front CTA object ID for right side */
   boolean_T f_front_cta_brake_qualifier_left;  /**< Front CTA brake qualifier for left side */
   boolean_T f_front_cta_brake_qualifier_right; /**< Front CTA brake qualifier for right side */
   float32_T front_cta_long_intersection_left;
   float32_T front_cta_long_intersection_right;
   float32_T front_cta_heading_left;
   float32_T front_cta_heading_right;
   uint8_t front_cta_warn_hold_cnt_left;
   uint8_t front_cta_warn_hold_cnt_right;

   float32_T rear_cta_obj_ttc_left;            /**< Rear CTA object TTC for left side */
   float32_T rear_cta_obj_ttc_right;           /**< Rear CTA object TTC for right side */
   uint8_t rear_cta_alert_level_left;          /**< Rear CTA alert level for left side */
   uint8_t rear_cta_alert_level_right;         /**< Rear CTA alert level for right side */
   uint8_t rear_cta_id_left;                   /**< Rear CTA object ID for left side */
   uint8_t rear_cta_id_right;                  /**< Rear CTA object ID for right side */
   boolean_T f_rear_cta_brake_qualifier_left;  /**< Rear CTA brake qualifier for left side */
   boolean_T f_rear_cta_brake_qualifier_right; /**< Rear CTA brake qualifier for right side */
   float32_T rear_cta_long_intersection_left;
   float32_T rear_cta_long_intersection_right;
   float32_T rear_cta_heading_left;
   float32_T rear_cta_heading_right;
   uint8_t rear_cta_warn_hold_cnt_left;
   uint8_t rear_cta_warn_hold_cnt_right;

   Cta_Rivian_Status_T cta_status; /**< CTA status */

} Cta_Output_T;


#endif
