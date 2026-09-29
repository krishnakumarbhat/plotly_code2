#ifndef FBK_VEHICLE_VALIDATION_H
#define FBK_VEHICLE_VALIDATION_H

/**
 * @file fbk_vehicle_validation.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Header file with functions for vehicle data validation.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_vehicle_data_t.h"
#include "pa_context.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Global Function Definition
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif
   /**
    * @brief Fills vehicle data from provided data.
    *
    * @return void
    *
    * @SRS{}
    * @SAE{}
    * @SDD{SF-4191}
    * @verification{}
    */
   void Fbk_Fill_Vehicle_Information(Fbk_Vehicle_Data_T *p_fbk_vehicle_data /**< internal fbk vehicle data structure */,
                                     const Pa_Context_T *p_context /**< context data */);
   /**
    * @brief Check if the vehicle data is within the defined range.
    *
    * @return void
    *
    * @SRS{}
    * @SAE{}
    * @SDD{CSCSA-307401}
    * @verification{Check if the vehicle data is within the defined range}
    */
   boolean_T Fbk_Verify_Vehicle_Data_Range(const Fbk_Vehicle_Data_T *p_vehicle_data /**< internal fbk vehicle data structure */);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* FBK_VEHICLE_VALIDATION_H*/
