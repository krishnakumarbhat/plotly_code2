#ifndef CED_CREATE_ZONES_H
#define CED_CREATE_ZONES_H

/**
 * @file ced_create_zones.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function prototypes to set up CED zones.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "ced_core_calibration_t.h"
#include "fbk_field_of_interest.h"
#include "fbk_vehicle_data_t.h"

/*===========================================================================*\
 * Global Functions	Definition
\*===========================================================================*/

/**
 * @brief Creates the funnel zone based on calibration values and host vehicle size.
 *
 * @return void
 *
 * @SRS{SF-83,SF-85,CSCSA-123159}
 * @SAE{SF-2402}
 * @SDD{SF-3599}
 * @verification{Create a test which checks that the funnel zones is setup correctly.}
 */
void Ced_Create_Funnel_Zone(Fbk_Field_Of_Interest_T *p_funnel_zone /**< Field of Interest for funnel zone */,
                            const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                            const Ced_Core_Calibration_T *p_ced_cal /**< CED Calibration */);

/**
 * @brief Creates the collision zone based on calibration values and host vehicle size.
 *
 * @return void
 *
 * @SRS{SF-86,SF-87,CSCSA-122438}
 * @SAE{SF-2402}
 * @SDD{SF-3598}
 * @verification{Create a test which checks whether collision zone is updated correctly.}
 */
void Ced_Create_Collision_Zone(Fbk_Field_Of_Interest_T *p_collision_zone /**< Field of Interest for collision zone */,
                               const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
                               const Ced_Core_Calibration_T *p_ced_cal /**< CED Calibration */);

#endif /* CED_CREATE_ZONES_H */
