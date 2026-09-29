#ifndef CED_OUTPUT_T_H
#define CED_OUTPUT_T_H

/**
 * @file ced_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Generic customer output declaration.
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

typedef enum
{
   OSE_NO_ALERT      = (0), /**< No alert level */
   OSE_ALERT_LEVEL_1 = (1), /**< Alert level 1 */
   OSE_ALERT_LEVEL_2 = (2)  /**< Alert level 2 */
} Ose_Alert_T;

/**
 * @brief Ced_Output_T structure.
 *
 *
 * @SRS{SF-65}
 * @SAE{SF-2429}
 * @SDD{SF-3460}
 */
typedef struct
{
   /* Nissan SRR6 OSE Outputs for State Machine and UDP Logging */
   uint8_t f_ced_enable;                         /* Flag indicating if OSE is enabled */
   Ose_Alert_T CED_alert_right;                  /* Alert indicator for left side (0 -> NO_WARNING, 1 -> LEVEL_1, 2 -> LEVEL_2) */
   Ose_Alert_T CED_alert_left;                   /* Alert indicator for right side (0 -> NO_WARNING, 1 -> LEVEL_1, 2 -> LEVEL_2) */
   uint8_t CED_id_right;                         /* Id of object with highest severity on right side */
   uint8_t CED_id_left;                          /* Id of object with highest severity on left side */
   float32_T CED_ttc_left;                       /* [s] TTC for most severe object on left side */
   float32_T CED_ttc_right;                      /* [s] TTC for most severe object on right side */
   float32_T CED_object_predicted_lat_pos_right; /* [m] Lateral predicted intersection point for most severe object on right side */
   float32_T CED_object_predicted_lat_pos_left;  /* [m] Lateral predicted intersection point for most severe object on left side */
} Ced_Output_T;

#endif /* CED_OUTPUT_T_H */
