#ifndef CTA_OUTPUT_T_H
#define CTA_OUTPUT_T_H

/**
 * @file cta_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Nissan_SRR6 output declaration.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
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
 * @brief structure providing information for the consumer e.g. statemachine
 */
typedef struct
{
   uint8_t f_cta_enabled; /**< Flag indicating that CTA is enabled*/

   uint8_t f_cta_alert_left;                /**< Flag indicating that there is an active CTA alert for the left side*/
   uint8_t cta_id_left;                     /**< ID of the object responsible for the CTA alert on the left side (0 for none)*/
   float32_T cta_ttc_left;                  /**< TTC (time to conflict) of the object given by CTA_id_left*/
   float32_T cta_objPoseX_left;             /**< target left side front-left corner lon-position*/
   float32_T cta_objPoseY_left;             /**< target left side front-left corner lat-position*/
   float32_T cta_objVelocityX_left;         /**< target left side lon-velocity*/
   float32_T cta_objVelocityY_left;         /**< target left side lat-velocity*/
   float32_T cta_heading_left;              /**< target left side cta heading*/
   float32_T cta_intersection_point_x_left; /**< intersection point on x axis calulated for left side object*/

   uint8_t f_cta_alert_right;                /**< Flag indicating that there is an active CTA alert for the left side*/
   uint8_t cta_id_right;                     /**< ID of the object responsible for the CTA alert on the right side (0 for none)*/
   float32_T cta_ttc_right;                  /**< TTC (time to conflict) of the object given by CTA_id_right*/
   float32_T cta_objPoseX_right;             /**< target right side front-right corner lon-position*/
   float32_T cta_objPoseY_right;             /**< target right side front-right corner lat-position*/
   float32_T cta_objVelocityX_right;         /**< target right side lon-velocity*/
   float32_T cta_objVelocityY_right;         /**< target right side lat-velocity*/
   float32_T cta_heading_right;              /**< target right side cta heading*/
   float32_T cta_intersection_point_x_right; /**< intersection point on x axis calulated for right side object*/

} Cta_Output_T;

#endif
