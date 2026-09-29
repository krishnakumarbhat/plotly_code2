#ifndef LCDA_OUTPUT_T_H
#define LCDA_OUTPUT_T_H

/**
 * @file lcda_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the LCDA output header file of the Generic customer.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "lcda_core_output_t.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Typedefs
\*===========================================================================*/

/**
 * @brief Lcda_Output_T structure
 *
 * @SRS{CSCSA-122830}
 * @SAE{CSCSA-122812}
 * @SDD{CSCSA-122802}
 */
typedef struct
{
   Lcda_Status_T lcda_status; /**< Status of the LCDA defined in enum Lcda_Status_T  */

   boolean_T f_bsw_enabled;                           /* Flag indicating that the BSW subfunction is enabled */
   Lcda_Alert_State_T bsw_alert[FBK_NUMBER_OF_SIDES]; /* Flag indicating if there is a critical BSW object for left/right side */
   uint8_t bsw_id[FBK_NUMBER_OF_SIDES]; /* Object ID of critical BSW object on the left/right side if present (0 for none)*/
   uint32_t bsw_unique_id[FBK_NUMBER_OF_SIDES]; /* Unique Object ID of critical BSW object on the left/right side if present (0 for
                                                   none)*/

   boolean_T f_cvw_enabled;                           /* Flag indicating that the CVW subfunction is enabled */
   Lcda_Alert_State_T cvw_alert[FBK_NUMBER_OF_SIDES]; /* Flag indicating if there is a critical CVW object for left/right side */
   uint8_t cvw_id[FBK_NUMBER_OF_SIDES]; /* Object ID of critical CVW object on the left/right side if present (0 for none) */
   uint32_t cvw_unique_id[FBK_NUMBER_OF_SIDES]; /* Unique Object ID of critical CVW object on the left/right side if present (0 for
                                                   none) */
   float32_T cvw_ttc_s[FBK_NUMBER_OF_SIDES];    /* [s] calculated TTC of critical CVW object on the left/right side if present */

   boolean_T f_slc_enabled;                  /* Flag indicating that the SLC subfunction is enabled */
   boolean_T slc_alert[FBK_NUMBER_OF_SIDES]; /* Flag indicating if there is a critical SLC object for left/right side */
   uint8_t slc_id[FBK_NUMBER_OF_SIDES];      /* object ID of critical SLC object on the left/right side if present (0 for none) */
   uint32_t slc_unique_id[FBK_NUMBER_OF_SIDES]; /* Unique object ID of critical SLC object on the left/right side if present (0 for
                                                   none) */
   float32_T slc_ttc_s[FBK_NUMBER_OF_SIDES];    /* [s] calculated TTC of critical SLC object on the left/right side if present */
   float32_T slc_lane_change_probability[FBK_NUMBER_OF_SIDES]; /* lane change probability of critical SLC object on the left/right
                                                              side if present */
} Lcda_Output_T;

#endif /* LCDA_OUTPUT_T_H */
