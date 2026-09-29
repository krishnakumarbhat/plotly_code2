#ifndef CTA_CORE_OUTPUT_T_H
#define CTA_CORE_OUTPUT_T_H

/**
 * @file cta_core_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Core output declaration.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_types.h"
#include "fbk_macros.h"
#include "pa_reuse.h"


/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Cta_Status_T indicates the status of CTA in the current cycle.
 * @SDD{CSCSA-88010}
 */
typedef enum
{
   CTA_STATUS_ACTIVE                = (0), /**< CTA is in active state */
   CTA_STATUS_DISABLED              = (1), /**< CTA is disabled by calibration */
   CTA_STATUS_DEACTIVATED_EGO_SPEED = (2)  /**< CTA is disabled by host speed */
} Cta_Status_T;

/**
 * @brief Structure summarizing the CTA core output
 *
 * @SRS{SF-163}
 * @SAE{SF-2502}
 * @SDD{SF-3678}
 */
typedef struct
{
   uint32_t cta_unique_id[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];           /**< Unique Id of an object*/
   Cta_Crit_Level_T cta_alert_level[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES]; /**< calculated CTA Alert level*/
   float32_T cta_obj_ttc[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];            /**< ttc of objects with highest criticality*/
   float32_T cta_obj_ttp[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];            /**< ttp of objects with highest criticality*/
   float32_T cta_long_intersection[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];  /**< longitudinal intersection of objects with
                                                                                   highest criticality*/
   float32_T cta_heading[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];            /**< used heading of objects with highest criticality*/
   uint8_t cta_id[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];                   /**< CTA object tracker ID */
   uint8_t cta_index[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];                /**< CTA object tracker index */
   boolean_T f_brake_qualifier[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];      /**< Flag indicating whether the brake qualifier is
                                                           set for the respective side*/
   uint8_t cta_warn_hold_cnt[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];        /**< Holding counter for each approach side*/
   uint8_t cta_brake_hold_cnt[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];       /**< Holding counter for the brake qualifier */
   uint8_t cta_brake_supp_cnt[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES]; /**< Counter used for suppression/qualification of brake */
   boolean_T f_standstill_qualifier[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];
   float32_T brake_deceleration[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];

   Cta_Status_T cta_status; /**< Status of the CTA as defined in enum Cta_Status_T  */
   boolean_T f_cta_enabled; /**< Flag indicating that CTA is enabled*/


} Cta_Core_Output_T;


#endif /* CTA_CORE_OUTPUT_T_H */
