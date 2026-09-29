#ifndef CED_COMMON_FUNCTIONS_T_H
#define CED_COMMON_FUNCTIONS_T_H

/**
 * @file ced_common_functions.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief This file contains the core output data structure for CED.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "pa_reuse.h"

/*===========================================================================*\
 * Global Functions	Definition
 \*===========================================================================*/

/**
 * @brief Calculates the time it takes to cover a given distance based on velocity and acceleration.
 *
 * @return Time in seconds
 *
 * @SRS{SF-109,CSCSA-124054}
 * @SAE{SF-2402}
 * @SDD{SF-3450}
 * @verification{Function returns the correct time for given distance, velocity and acceleration.}
 */
float32_T Ced_Get_Time_To_Travel_Given_Distance(const float32_T distance /**< Distance [m] */,
                                                const float32_T velocity /**< Velocity [m/s] */,
                                                const float32_T acceleration /**< Acceleration [m/s^2] */);

#endif /* CED_COMMON_FUNCTIONS_T_H */
