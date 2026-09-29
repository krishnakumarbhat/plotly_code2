#ifndef CED_OUTPUT_T_H
#define CED_OUTPUT_T_H

/**
 * @file ced_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Honda_SRR6 customer output declaration.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "fbk_macros.h"
#include "pa_reuse.h"

/*===========================================================================*\
 * typedefs
\*===========================================================================*/

typedef enum
{
   OSE_NO_ALERT     = (0), /**< No alert level */
   OSE_ACTIVE_ALERT = (1)  /**< Active alert level */
} Ose_Alert_T;

typedef enum
{
   E_RATCH_NO_ALERT     = (0), /**< No e-latch alert  */
   E_RATCH_ACTIVE_ALERT = (1)  /**< Active e-latch alert  */
} E_Ratch_Alert_T;

typedef enum
{
   OSE_FRONT_DIRECTION = (0), /**< Front direction */
   OSE_REAR_DIRECTION  = (1), /**< Rear direction */
   OSE_UNDEF_DIRECTION = (2)  /**< Undefined direction */
} Ose_Target_Travel_Direction_T;

/**
 * @brief Ced_Output_T Structure summarizing Honda SRR6 specific CED output data.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-3492}
 */
typedef struct
{
   /* Nissan SRR6 OSE Outputs for State Machine and UDP Logging */
   uint8_t f_ced_enable;                         /* Flag indicating if OSE is enabled */
   Ose_Alert_T CED_alert_right;                  /**Alert indicator for left side (0 -> NO_WARNING, 1 -> Active alert)*/
   Ose_Alert_T CED_alert_left;                   /**Alert indicator for right side (0 -> NO_WARNING, 1 -> Active alert)*/
   Ose_Target_Travel_Direction_T CED_dir_right;  /**Direction indicator for right side for most severe object (Enum
                                                    Ced_Target_Travel_Direction_T)*/
   Ose_Target_Travel_Direction_T CED_dir_left;   /**Direction indicator for left side for most severe object (Enum
                                                    Ced_Target_Travel_Direction_T)*/
   uint8_t CED_id_right;                         /**Id of object with highest severity on right side*/
   uint8_t CED_id_left;                          /**Id of object with highest severity on left side*/
   float32_T CED_ttc_left;                       /**[s] TTC for most severe object on left side*/
   float32_T CED_ttc_right;                      /**[s] TTC for most severe object on right side*/
   float32_T CED_object_predicted_lat_pos_right; /**[m] Lateral predicted intersection point for most severe object on right side */
   float32_T CED_object_predicted_lat_pos_left;  /**[m] Lateral predicted intersection point for most severe object on left side */

   float32_T CED_current_alert_duration[FBK_NUMBER_OF_SIDES]; /**[s] Current alert duration */
   boolean_T CED_f_hold_alert[FBK_NUMBER_OF_SIDES];           /**[s] Flag indicating if alert shoulf be held */

   float32_T CED_honda_custom_ttc[FBK_NUMBER_OF_SIDES]; /**[s] Honda custom ttc values */

   /* E-ratch alert info. Be sure to not reset this signals in each cycle because of the holding logic. Reset is provide insing
    * holding function*/
   E_Ratch_Alert_T CED_eratch_alert_left;                            /** E-latch alert indicator for left side (0-1)*/
   E_Ratch_Alert_T CED_eratch_alert_right;                           /** E-latch alert indicator for right side (0-1)*/
   float32_T CED_current_eratch_alert_duration[FBK_NUMBER_OF_SIDES]; /**[s] Current alert eratch alert duration */

} Ced_Output_T;

#endif /* CED_OUTPUT_T_H */
