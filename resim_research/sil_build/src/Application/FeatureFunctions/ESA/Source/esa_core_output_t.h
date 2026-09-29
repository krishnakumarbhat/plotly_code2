#ifndef ESA_CORE_OUTPUT_T_H
#define ESA_CORE_OUTPUT_T_H

/**
 * @file esa_core_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the core output data structure for ESA.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "esa_types.h"
#include "fbk_macros.h"
#include "pa_reuse.h"

/*===========================================================================*\
* typedefs
\*===========================================================================*/

/**
 * @brief Esa_Status_T summarizes Enable states of the overall feature.
 *
 * @SDD{CSCSA-65949}
 */
typedef enum
{
   ESA_CORE_STATUS_ENABLED                      = (0), /**< Enabled */
   ESA_CORE_STATUS_ACTIVE                       = (1), /**< In active state */
   ESA_CORE_STATUS_DISABLED_BY_CAL              = (2), /**< Disabled by calibration */
   ESA_CORE_STATUS_DISABLED_BY_INPUT            = (3), /**< Disabled by input */
   ESA_CORE_STATUS_DEACTIVATED_LOW_EGO_SPEED    = (4), /**< Deactivated due to low host speed */
   ESA_CORE_STATUS_DEACTIVATED_HIGH_EGO_SPEED   = (5), /**< Deactivated due to high host speed */
   ESA_CORE_STATUS_DEACTIVATED_LOW_CURVE_RADIUS = (6), /**< Deactivated by low curve radius */
   ESA_CORE_STATUS_DEACTIVATED_INTERNAL_ERROR   = (7)  /**< Deactivated by internal error */
} Esa_Core_Status_T;


/**
 * @brief Esa_Core_Output_T structure
 *
 * @SRD{}
 * @SAD{}
 * @SDD{CSCSA-65124}
 */
typedef struct
{
   Esa_Core_Status_T esa_core_status; /**< Status of ESA functionality */

   float32_T esa_ttc[FBK_NUMBER_OF_SIDES]; /**< [s] Calculated TTC of critical ESA object on the left/right side if present */
   float32_T esa_ttp[FBK_NUMBER_OF_SIDES]; /**< [s] Calculated TTP of critical ESA object on the left/right side if present */
   float32_T esa_decel_to_reach_host_speed[FBK_NUMBER_OF_SIDES]; /**< [m/s^2] Required deceleration of critical ESA object to
                                                                reach host speed */
   float32_T esa_long_distance[FBK_NUMBER_OF_SIDES]; /**< [m] longitudinal distance of critical ESA object to the host */
   uint8_t esa_index[FBK_NUMBER_OF_SIDES];   /**< Index of critical ESA object on the left/right side if present (255 for none)*/
   uint8_t esa_id[FBK_NUMBER_OF_SIDES];      /**< ID of critical ESA object on the left/right side if present (0 for none)*/
   boolean_T esa_alert[FBK_NUMBER_OF_SIDES]; /**< Flag indicating if there is a critical ESA object for left/right side */
} Esa_Core_Output_T;

#endif /* ESA_CORE_OUTPUT_T_H */
