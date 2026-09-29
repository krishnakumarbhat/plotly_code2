#ifndef LCDA_CREATE_CVW_ZONE_H
#define LCDA_CREATE_CVW_ZONE_H

/**
 * @file lcda_create_cvw_zone.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Creates object specific zones on adjacent lanes.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "fbk_vehicle_data_t.h"
#include "lcda_core_calibration_t.h"
#include "lcda_core_input_t.h"
#include "lcda_persistent_t.h"
#include "lcda_types.h"
#include "pa_reuse.h"

/*============================================================================*\
 * Global Function Prototype
\*============================================================================*/

/**
 * @brief Main function for the creation of object specific cvw zone.
 *
 * @return void
 *
 * @SRS{SF-993}
 * @SAE{SF-2779}
 * @SDD{SF-6606}
 * @verification{Check that the input zone pointers are initialized after calling of this function.}
 */
void Lcda_Create_Cvw_Zone(
   Fbk_Field_Of_Interest_T *p_zone /**< Cvw zone */,
   Fbk_Field_Of_Interest_T *p_zone_hys /**< Cvw zone with hysteresis applied */,
   const boolean_T f_use_small_lc_intention_zone[FBK_NUMBER_OF_SIDES] /**< Flags indicating which zone to use */,
   const Cvw_Object_T *p_cvw_object /**< Cvw object */,
   const Lcda_Core_Input_T *p_core_input /**< Lcda core input*/,
   const Fbk_Vehicle_Data_T *p_vehicle_data /**< FBK vehicle data */,
   const Lcda_Core_Calibration_T *p_cals /**< Lcda core input */,
   Lcda_Cvw_Persistent_T *p_cvw_persistent);

#endif /* LCDA_CREATE_CVW_ZONE_H */
