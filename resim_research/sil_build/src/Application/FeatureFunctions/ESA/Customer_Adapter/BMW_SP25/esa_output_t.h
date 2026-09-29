#ifndef ESA_OUTPUT_T_H
#define ESA_OUTPUT_T_H

/**
 * @file ced_output_t.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the BMW_SP25 output data structure for ESA.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
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
   ESA_STATUS_NO_VEHICLE_IN_ZONE        = (0), /**< There is no "critical" object on the ESA zone */
   ESA_STATUS_VEHICLE_IN_ZONE           = (1), /**< "Critical" object exists on the ESA zone */
   ESA_STATUS_OUTSIDE_SYSTEM_BOUNDARIES = (2)  /**< information whether a critical object is in the ESA zone cannot be taken
                                    because the activation conditions are not met or an error (sensor blindness,
                                    sensor deadjustment, sensor not calibrated, trailer, input is missing) is detected */
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)]  */
} Esa_Status_T;


/**
 * @brief Esa_Output_T structure.
 *
 *
 * @SRD{}
 * @SAD{}
 * @SDD{n/a}
 */
typedef struct
{
   Esa_Status_T Esa_Status_Left;
   Esa_Status_T Esa_Status_Right;

   /**< [] current object ID of the critical object on the left and right side */
   uint8_t Object_ID_Left;
   uint8_t Object_ID_Right;

   /**< time on which the detections of the object have been recorded for the critical object on the left and right side */
   float32_T Object_Timestamp_Left;
   float32_T Object_Timestamp_Right;

   /**< [m] longitudinal distance to the center of the critical object on the left and right side */
   float32_T Object_Position_X_Left;
   float32_T Object_Position_X_Right;

   /**< [m] lateral distance to the center of the critical object on the left and right side */
   float32_T Object_Position_Y_Left;
   float32_T Object_Position_Y_Right;

   /**< [m] width of the critical object on the left and right side */
   float32_T Object_Width_Left;
   float32_T Object_Width_Right;

   /**< [m] length of the critical object on the left and right side */
   float32_T Object_Length_Left;
   float32_T Object_Length_Right;

   /**< [m/s] absolute longitudinal speed of the critical object on the left and right side */
   float32_T Object_Speed_X_Left;
   float32_T Object_Speed_X_Right;

   /**< [m/s] absolute lateral speed of the critical object on the left and right side */
   float32_T Object_Speed_Y_Left;
   float32_T Object_Speed_Y_Right;

   /**< [s] time until the critical object has completely passed the front of Egovehicle on the left and right side */
   float32_T Object_Time_To_Pass_Left;
   float32_T Object_Time_To_Pass_Right;

   /**< [%] existence probability of the critical object on the left and right side */
   float32_T Object_Existence_Probability_Left;
   float32_T Object_Existence_Probability_Right;

} Esa_Output_T;


#endif /* ESA_OUTPUT_T_H */
