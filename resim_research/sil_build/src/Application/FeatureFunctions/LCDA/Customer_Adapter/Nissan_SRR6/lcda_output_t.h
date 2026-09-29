#ifndef LCDA_OUTPUT_T_H
#define LCDA_OUTPUT_T_H

/**
 * @file lcda_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the Nissan SRR6 output data structure for LCDA.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

/* fbk includes */
#include "pa_reuse.h"

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

/**
 * @brief Lcda_Output_T structure
 *
 *
 * @SRS{SF-1038}
 * @SAE{SF-2802}
 * @SDD{SF-6950}
 */
typedef struct
{
   uint8_t f_lcda_enabled; /* Flag indicating if LCDA is enabled */
   uint8_t f_bsw_enabled;  /* Flag indicating that the BSW subfunction is enabled */
   uint8_t f_cvw_enabled;  /* Flag indicating that the CVW subfunction is enabled */

   uint8_t bsw_alert_left;  /* flag indicating if there is a BSW alert for left side */
   uint8_t bsw_id_left;     /* object ID of critical BSW object on the left side if present (0 for none)*/
   uint8_t bsw_alert_right; /* flag indicating if there is a BSW alert for right side */
   uint8_t bsw_id_right;    /* object ID of critical BSW object on the right side if present (0 for none)*/

   uint8_t cvw_alert_left;  /* flag indicating if there is a CVW alert for left side */
   uint8_t cvw_id_left;     /* object ID of critical CVW object on the left side if present (0 for none)*/
   float32_T cvw_ttc_left;  /* [s] calculated TTC of critical CVW object on the left side if present */
   uint8_t cvw_alert_right; /* flag indicating if there is a CVW alert for right side */
   uint8_t cvw_id_right;    /* object ID of critical CVW object on the right side if present (0 for none)*/
   float32_T cvw_ttc_right; /* [s] calculated TTC of critical CVW object on the right side if present */

} Lcda_Output_T;

#endif /* LCDA_OUTPUT_T_H */
