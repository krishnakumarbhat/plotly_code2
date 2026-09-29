#ifndef ESA_CREATE_ZONES_H
#define ESA_CREATE_ZONES_H

/**
 * @file esa_create_zones.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function prototypes to set up ESA zones.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "esa_core_calibration_t.h"
#include "esa_core_input_t.h"
#include "esa_types.h"
#include "pa_reuse.h"

/*===========================================================================*\
 * Global Functions	Definition
\*===========================================================================*/

/**
 * @brief Creates the zone based on calibration values and host vehicle size.
 *
 * @return void
 *
 * @SRD{CSCSA-68627,CSCSA-68628}
 * @SAD{CSCSA-83853}
 * @SDD{CSCSA-66547}
 * @verification{Create a test which checks that the funnel zones is setup correctly.}
 */
void Esa_Create_Zone(Esa_Object_T *p_esa_object /**< ESA object */,
                     const uint8_t mature_count_in_esa_zone /**< number of cycles in ESA zone */,
                     const Esa_Core_Input_T *p_esa_core_input /**< ESA core input */,
                     const Esa_Core_Calibration_T *p_esa_calibration /**< ESA Calibration */);

#endif /* ESA_CREATE_ZONES_H */
