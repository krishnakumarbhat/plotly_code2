#ifndef RECW_CAR_WASH_DETECTION_H
#define RECW_CAR_WASH_DETECTION_H

/**
 * @file recw_car_wash_detection.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the car wash detection header file.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_vehicle_data_t.h"
#include "pa_reuse.h"
#include "recw_core_calibration_t.h"
#include "recw_persistent_t.h"

#include "recw_types.h"


/*===========================================================================*\
* Typedefs
\*===========================================================================*/

/**
 * @brief Contains states for selection types of car wash algorithm
 * @SRS{}
 * @SAE{}
 * @SDD{SF-7865}
 * @verification{}
 */
typedef enum
{
   RECW_CAR_WASH_OFF          = (0), /**< Car wash algorithm is deactivated */
   RECW_CAR_WASH_TARGET_STATE = (1), /**< Car wash algorithm shall be based on targets state */
   RECW_CAR_WASH_NEUTRAL_GEAR = (2)  /**< Car wash algorithm shall be based gear position*/
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Recw_Car_Wash_Algo_Selection_T;

/*===========================================================================*\
 * Global Function Prototypess
\*===========================================================================*/

/**
 * @brief Checks whether an object is a car wash ghost
 *
 * @return True when object is a car wash ghost
 *
 * @SRS{SF-1693}
 * @SAE{SF-2959}
 * @SDD{SF-7868}
 * @verification{}
 */
boolean_T Recw_Is_Obj_Car_Wash_Ghost(const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                                     const Recw_Core_Calibration_T *p_cals /**< RECW calibrations*/,
                                     const Recw_Object_T *p_recw_object /**< RECW object */,
                                     Car_Wash_Scenario_Flags_T *p_car_wash_scenario_flags /**< Possible car wash scenario flags */);

/**
 * @brief Initializes internal car wash flags
 *
 * @return void
 *
 * @SRS{SF-1662}
 * @SAE{SF-2959}
 * @SDD{SF-7867}
 * @verification{}
 */
void Recw_Init_Car_Wash_Flags(uint8_t possible_car_wash_scenario_flags[RECW_CAR_WASH_FLAG_ARRAY_SIZE]);

#endif /* RECW_CAR_WASH_DETECTION_H */
