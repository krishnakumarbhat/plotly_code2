#ifndef CTA_OUTPUT_T_H
#define CTA_OUTPUT_T_H

/**
 * @file cta_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief STLA_Thunder customer output declaration.
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

/**
 * @brief Summarizes the STLA position-based criticality levels
 */
typedef enum
{
   CTA_STLA_CRIT_ZONE_NONE = (0),
   CTA_STLA_CRIT_ZONE_1    = (1),
   CTA_STLA_CRIT_ZONE_2    = (2),
   CTA_STLA_CRIT_ZONE_3    = (3),
   CTA_STLA_CRIT_ZONE_4    = (4),
   CTA_STLA_CRIT_ZONE_5    = (5),
   CTA_STLA_CRIT_ZONE_6    = (6),
   CTA_STLA_CRIT_ZONE_7    = (7),
   CTA_STLA_CRIT_ZONE_8    = (8)
} CTA_STLA_CRIT_ZONE_T;

/**
 * @brief Cta_Status_T indicates the status of CTA in the current cycle.
 */
typedef enum
{
   RCP_STATUS_ACTIVE                = (0), /**< CTA is in active state */
   RCP_STATUS_DISABLED              = (1), /**< CTA is disabled by calibration */
   RCP_STATUS_DEACTIVATED_EGO_SPEED = (2)  /**< CTA is disabled by host speed */
} Rcp_Status_T;
typedef struct
{
   float32_T cta_obj_ttc_left;
   float32_T cta_obj_ttc_right;
   uint8_t cta_alert_level_left;
   uint8_t cta_alert_level_right;
   uint8_t cta_id_left;
   uint8_t cta_id_right;
   uint8_t cta_warn_hold_cnt_left;
   uint8_t cta_warn_hold_cnt_right;
   uint8_t cta_brake_hold_cnt_left;
   uint8_t cta_brake_hold_cnt_right;
   uint8_t cta_brake_supp_cnt_left;
   uint8_t cta_brake_supp_cnt_right;
   boolean_T f_brake_qualifier_left;
   boolean_T f_brake_qualifier_right;
   boolean_T f_cta_enabled;
   CTA_STLA_CRIT_ZONE_T cta_stla_crit_zone_left;
   CTA_STLA_CRIT_ZONE_T cta_stla_crit_zone_right;
   Rcp_Status_T rcp_status;

   /* Butterfly zone coordinates*/
   float32_T DBG_Crit_Zone_Right_P0_PositionX;
   float32_T DBG_Crit_Zone_Right_P0_PositionY;
   float32_T DBG_Crit_Zone_Right_P1_PositionX;
   float32_T DBG_Crit_Zone_Right_P1_PositionY;
   float32_T DBG_Crit_Zone_Right_P2_PositionX;
   float32_T DBG_Crit_Zone_Right_P2_PositionY;
   float32_T DBG_Crit_Zone_Right_P3_PositionX;
   float32_T DBG_Crit_Zone_Right_P3_PositionY;
} Cta_Output_T;

#endif
