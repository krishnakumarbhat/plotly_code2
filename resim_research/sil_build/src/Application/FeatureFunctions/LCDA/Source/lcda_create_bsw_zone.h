#ifndef LCDA_CREATE_BSW_ZONE_H
#define LCDA_CREATE_BSW_ZONE_H

/**
 * @file lcda_create_bsw_zone.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Exported function for zone creation in blind spot area.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_field_of_interest.h"
#include "lcda_core_calibration_t.h"
#include "lcda_core_input_t.h"
#include "lcda_types.h"

/*============================================================================*\
 * Global Function Prototype
\*============================================================================*/

/**
 * @brief Main function for the creation of object specific bsw zone.
 *
 * @return void
 *
 * @SRS{SF-993}
 * @SAE{SF-2779}
 * @SDD{SF-6597}
 * @verification{Check that the input zone pointers are initialized after calling of this function.}
 */
void Lcda_Create_Bsw_Zone(Fbk_Field_Of_Interest_T *p_zone /**< Bsw zone */,
                          Fbk_Field_Of_Interest_T *p_zone_hys /**< Bsw zone with hysteresis applied */,
                          const Bsw_Object_T *p_bsw_object /**< Bsw object */,
                          const Lcda_Core_Input_T *p_core_input /**< Lcda core input */,
                          const Lcda_Core_Calibration_T *p_cals /**< Lcda calibrations */);

#endif /* LCDA_CREATE_BSW_ZONE_H */
