/**
 * @file esa_debug_interface.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the functions for copying internal data to the debug output interface.
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 */

#include "esa_debug_interface.h"
#include "esa_create_zone.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "pa_vehicle_in.h"
#include <assert.h>
#include <string.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden) */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
 * File Scope variables
\*===========================================================================*/

static Esa_Debug_Data_T esa_debug_data;

/*===========================================================================*\
* Debug interface
\*===========================================================================*/

Esa_Debug_Data_T *Esa_Get_Debug_Data(void)
{
   return (&esa_debug_data);
}

/*===========================================================================*\
* Helper functions to fill and reset debug data
\*===========================================================================*/

void Esa_Debug_Reset_Data(void)
{
   memset(&esa_debug_data, 0, sizeof(Esa_Debug_Data_T));
}

void Esa_Debug_Pass_General_Data(const Esa_Core_Input_T *p_esa_core_input,
                                 const Esa_Core_Output_T *p_esa_core_output,
                                 const Esa_Persistent_T *p_esa_persistent,
                                 const Esa_Core_Calibration_T *p_esa_calibration)
{
   /* Assert that all passed pointers are valid. */
   assert(NULL != p_esa_core_input);
   assert(NULL != p_esa_core_output);
   assert(NULL != p_esa_persistent);
   assert(NULL != p_esa_calibration);

   /* Copy data from internal interfaces to debug output interface. */
   esa_debug_data.esa_core_input                  = *p_esa_core_input;
   esa_debug_data.esa_core_output                 = *p_esa_core_output;
   esa_debug_data.esa_debug_output.esa_persistent = *p_esa_persistent;

   memcpy(&esa_debug_data.esa_calibration, p_esa_calibration, sizeof(Esa_Core_Calibration_T));
}

void Esa_Debug_Pass_Sw_Version(const uint16_t esa_sw_major_version, const uint16_t esa_sw_minor_version)
{
   /* Store version from FF iface. */
   esa_debug_data.esa_version.esa_sw_major_version = esa_sw_major_version;
   esa_debug_data.esa_version.esa_sw_minor_version = esa_sw_minor_version;
}

void Esa_Debug_Pass_Persistent_Data(Esa_Persistent_T *p_esa_persistent)
{
   esa_debug_data.esa_debug_output.esa_persistent = *p_esa_persistent;
}

void Esa_Debug_Pass_Default_Zone(const Esa_Core_Input_T *p_core_input, const Esa_Core_Calibration_T *p_esa_calibration)
{
   Esa_Object_T esa_object          = {0};
   Fbk_Object_Data_T tracker_object = {0};

   esa_object.ego_side       = FBK_SIDE_RIGHT;
   esa_object.p_tracker_data = &tracker_object;
   tracker_object.width      = FBK_ZERO_UINT;

   /* Asserts */
   assert(NULL != p_core_input);
   assert(NULL != p_esa_calibration);

   /* Create a default zone and set it for the first object */
   Esa_Create_Zone(&esa_object, FBK_ZERO_UINT, p_core_input, p_esa_calibration);

   /* Store default zone in debug structure */
   esa_debug_data.esa_debug_output.esa_default_zone = esa_object.zone;
}

void Esa_Debug_Pass_Object_Attributes(const Esa_Object_T *p_esa_object)
{
   uint8_t object_index = p_esa_object->p_tracker_data->index;

   /* Assert */
   assert(NULL != p_esa_object);

   /* Copy data from internal structure to debug output interface. */
   esa_debug_data.esa_debug_output.esa_object_data[object_index] = *p_esa_object;
}

#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */
