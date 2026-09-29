#ifndef LTB_FACTORY_H
#define LTB_FACTORY_H

/**
 * @file ltb_factory.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This module makes the global variables accessible to other modules and implements
 * the functions declared in its header.
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "fbk_field_of_interest.h"
#include "fbk_traj_predictor_t.h"
#include "ltb_core_calibration_t.h"

/*===========================================================================*\
 * Global Functions	Definition
\*===========================================================================*/

/**
 * @brief Resets the LTB trajectory struct.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53905}
 * @verification{Create a test to check that trajectory properties are reset to their default.}
 */
void Ltb_Reset_Trajectory(Fbk_Trajectory_T *p_ltb_trajectory /**< LTB trajectory */,
                          const Ltb_Core_Calibration_T *p_ltb_cal /**< LTB Calibration */);


/**
 * @brief Creates the funnel zone based on calibration values and host vehicle size.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{CSCSA-53906}
 * @verification{Create a test which checks that the zone is setup correctly.}
 */
void Ltb_Create_Zone(Fbk_Field_Of_Interest_T *p_zone_left /**< Field of Interest for left ltb zone */,
                     Fbk_Field_Of_Interest_T *p_zone_right /**< Field of Interest for right ltb zone */,
                     const Ltb_Core_Calibration_T *p_ltb_cal /**< LTB Calibration */);


#endif /* LTB_FACTORY_H */
