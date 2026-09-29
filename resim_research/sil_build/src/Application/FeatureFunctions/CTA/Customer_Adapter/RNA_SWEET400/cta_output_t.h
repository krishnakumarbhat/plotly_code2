/**
 * @file cta_output.h
 * @brief Contains the RNA_SWEET400 customer project CTA output definition.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#ifndef CTA_OUTPUT_T_H
#define CTA_OUTPUT_T_H

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/
/* coverity[misra_c_2012_rule_2_4_violation] */
typedef struct CTA_ERRORS_Tag
{
   uint32_t f_input_ptr_null : 1;
   uint32_t f_tracker_ptr_null : 1;
   uint32_t f_vehicle_ptr_null : 1;
   uint32_t f_cals_ptr_null : 1;
   uint32_t f_persistant_ptr_null : 1;
   uint32_t f_error_flag_unused : 27;
} CTA_ERRORS_T;

/**
 * @brief Structure summarizing the CTA output.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-4025}
 */
/* coverity[misra_c_2012_rule_2_4_violation] */
typedef struct Cta_Output_Tag
{
   uint8_t f_cta_enabled; /**< Flag indicating that CTA is enabled */

   /*
      The folloing date is used for debugging and application (warning, alert)
      ID, TTC, Intersection and radial_distance are used for debugging output
   */
   uint8_t f_cta_alert_left;          /**<Flag indicating that there is an active CTA alert for the left side*/
   uint8_t f_cta_warn_left;           /**<Flag indicating that there is a warning on the left side (but no alert)*/
   uint8_t f_cta_prefill_req_left;    /**<Flag indicating that there is a prefill request on the left side*/
   uint8_t f_cta_braking_req_left;    /**<Flag indicating that there is a braking request on the left side*/
   uint8_t f_cta_hold_supp_left;      /**<Flag indicating that there is are conditions for hold request, CTA just support, it, not
                                      request directly*/
   int32_t cta_id_left;               /**<ID of the object responsible for the CTA alert on the left side (-1 for none)*/
   float32_T cta_ttc_left;            /**<TTC (time to conflict) of the object given by CTA_id_left*/
   float32_T cta_intersectionX_left;  /**<intersection of target trajectory with host x-axis*/
   float32_T cta_radialDistance_left; /**<radial distance of target from center of rear bumper*/
   float32_T cta_objPoseX_left;       /**<x coordinate of target position*/
   float32_T cta_objPoseY_left;       /**<y coordinate of target position*/
   float32_T cta_objVelocityX_left;   /**<target velocity (longitudinal to host)*/
   float32_T cta_objVelocityY_left;   /**<target velocity (lateral to host)*/

   uint8_t f_cta_alert_right;         /**< Flag indicating that there is an active CTA alert for the right side*/
   uint8_t f_cta_warn_right;          /**< Flag indicating that there is a warning on the right side (but no alert)*/
   uint8_t f_cta_prefill_req_right;   /**< Flag indicating that there is a prefill request on the right side*/
   uint8_t f_cta_braking_req_right;   /**< Flag indicating that there is a braking request on the right side*/
   uint8_t f_cta_hold_supp_right;     /**< Flag indicating that there is are conditions for hold request, CTA just support, it, not
                                      request directly*/
   int32_t cta_id_right;              /**< ID of the object responsible for the CTA alert on the right side (-1 for none)*/
   float32_T cta_ttc_right;           /**< TTC (time to conflict) of the object given by CTA_id_right*/
   float32_T cta_intersectionX_right; /**< intersection of target trajectory with host x-axis*/
   float32_T cta_radialDistance_right; /**< radial distance of target from center of rear bumper*/
   float32_T cta_objPoseX_right;       /**< x coordinate of target position*/
   float32_T cta_objPoseY_right;       /**< x coordinate of target position*/
   float32_T cta_objVelocityX_right;   /**< target velocity (longitudinal to host)*/
   float32_T cta_objVelocityY_right;   /**< target velocity (lateral to host)*/

   CTA_ERRORS_T error_flags; /**< Error flags for debugging purposes on bench */

   uint8_t RCTA_Criticality_level_right; /**< criticality level on right side */
   uint8_t RCTA_Criticality_level_left;  /**< criticality level on left side */

   float32_T cta_heading_rear_left;  /**< cta core heading of rear left target*/
   float32_T cta_heading_rear_right; /**< cta core heading of rear right target*/

} Cta_Output_T;

/*===========================================================================*\
 * Global Function Prototypess
\*===========================================================================*/


#endif /* CTA_OUTPUT_T_H */
