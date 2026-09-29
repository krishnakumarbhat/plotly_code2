#ifndef LCDA_OUTPUT_T_H
#define LCDA_OUTPUT_T_H

/**
 * @file lcda_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the LCDA Rivian_SRR6 header file of the STLA_Thunder customer.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_reuse.h"

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

typedef enum
{
   RIVIAN_LCDA_ACTIVE                       = (0), /**< Overall feature is in active state*/
   RIVIAN_LCDA_DISABLED                     = (1), /**< Overall feature is disabled */
   RIVIAN_LCDA_DEACTIVATED_LOW_EGO_SPEED    = (2), /**< Overall feature is deactivated due to low host speed */
   RIVIAN_LCDA_DEACTIVATED_HIGH_EGO_SPEED   = (3), /**< Overall feature is deactivated due to high host speed */
   RIVIAN_LCDA_DEACTIVATED_LOW_CURVE_RADIUS = (4), /**< Overall feature is deactivated due to low curve radius */
   RIVIAN_LCDA_DEACTIVATED_INTERNAL_ERROR   = (5)  /**< Overall feature is deactivated due to internal error */
} Lcda_Rivian_Status_T;

typedef struct
{
   Lcda_Rivian_Status_T lcda_status;
   uint8_t f_bsw_enabled;
   uint8_t f_cvw_enabled;

   uint8_t bsw_alert_left;
   uint8_t bsw_id_left;
   uint8_t bsw_alert_right;
   uint8_t bsw_id_right;

   uint8_t cvw_alert_left;
   uint8_t cvw_id_left;
   float32_T cvw_ttc_left;
   uint8_t cvw_alert_right;
   uint8_t cvw_id_right;
   float32_T cvw_ttc_right;

} Lcda_Output_T;

#endif /* LCDA_OUTPUT_T_H */
