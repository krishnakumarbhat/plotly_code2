#ifndef CED_OUTPUT_T_H
#define CED_OUTPUT_T_H

/**
 * @file ced_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the CED output header file of the RNA_SWEET400 customer.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
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
 * @brief Ced_Output_T RNA_SWEET400 specific CED output data
 *
 * @SRS{}
 * @SAE{}
 * @SDD{SF-3487}
 */
typedef struct
{
   uint8_t CED_alert_left;  /**<Alert indicator for left side*/
   uint8_t CED_id_left;     /**<Id of object with highest severity on left side*/
   uint8_t CED_alert_right; /**<Alert indicator for left side*/
   uint8_t CED_id_right;    /**<Id of object with highest severity on right side*/

   uint8_t CED_front_alert_left;  /**<Alert indicator for left side - front coming traffic.*/
   uint8_t CED_front_id_left;     /**<Id of object with highest severity on left side - front coming traffic.*/
   uint8_t CED_front_alert_right; /**<Alert indicator for left side - front coming traffic.*/
   uint8_t CED_front_id_right;    /**<Id of object with highest severity on right side - front coming traffic.*/

   float32_T CED_ttc_left;        /**<TTC for most severe object on left side in [s]*/
   float32_T CED_ttc_right;       /**<TTC for most severe object on right side in [s]*/
   float32_T CED_front_ttc_left;  /**<TTC for most severe object on left side in [s] - front coming traffic.*/
   float32_T CED_front_ttc_right; /**<TTC for most severe object on right side in [s] - front coming traffic.*/

} Ced_Output_T;

#endif /* CED_OUTPUT_T_H */
