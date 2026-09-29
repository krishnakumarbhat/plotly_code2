/**
 * @file scw_debug_interface.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the functions for copying internal data to the debug output interface.
 *
 * @copyright Copyright (C) 2025 Aptiv. All rights reserved.
 */

#include "scw_debug_interface.h"
#include "fbk_field_of_interest_factory.h"
#include "fbk_macros.h"
#include "pa_vehicle_in.h"
#include <assert.h>
#include <string.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
 * File Scope variables
\*===========================================================================*/

static Scw_Debug_Data_T scw_debug_data;

/*===========================================================================*\
* Debug interface
\*===========================================================================*/

Scw_Debug_Data_T *Scw_Get_Debug_Data(void)
{
   return (&scw_debug_data);
}

/*===========================================================================*\
* Helper functions to fill and reset debug data
\*===========================================================================*/

void Scw_Debug_Reset_Data(void)
{
   memset(&scw_debug_data, 0, sizeof(Scw_Debug_Data_T));
}

void Scw_Debug_Pass_General_Data(const Scw_Core_Input_T *p_scw_core_input,
                                 const Scw_Core_Output_T *p_scw_core_output,
                                 const Scw_Persistent_T *p_scw_persistent,
                                 const Scw_Core_Calibration_T *p_scw_cal)
{
   /* Assert that all passed pointers are valid. */
   assert(NULL != p_scw_core_input);
   assert(NULL != p_scw_core_output);
   assert(NULL != p_scw_persistent);
   assert(NULL != p_scw_cal);

   /* Copy data from internal interfaces to debug output interface. */
   scw_debug_data.scw_core_input  = *p_scw_core_input;
   scw_debug_data.scw_core_output = *p_scw_core_output;
   scw_debug_data.scw_persistent  = *p_scw_persistent;
   memcpy(&scw_debug_data.scw_calibration, p_scw_cal, sizeof(Scw_Core_Calibration_T));
}

void Scw_Debug_Pass_Sw_Version(const uint16_t scw_sw_major_version, const uint16_t scw_sw_minor_version)
{
   /* Store version from FF iface. */
   scw_debug_data.scw_version.scw_sw_major_version = scw_sw_major_version;
   scw_debug_data.scw_version.scw_sw_minor_version = scw_sw_minor_version;
}

void Scw_Debug_Pass_Zones(const Fbk_Field_Of_Interest_T *const p_scw_zone, const Fbk_Field_Of_Interest_T *const p_scw_hysteresis_zone)
{
   scw_debug_data.scw_debug_output.scw_zone            = *p_scw_zone;
   scw_debug_data.scw_debug_output.scw_hysteresis_zone = *p_scw_hysteresis_zone;
}

void Scw_Debug_Increase_Object_Importance_Counter(const uint8_t obj_id)
{
   scw_debug_data.scw_debug_output.scw_object_importance_counter[obj_id]++;
}

#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */
