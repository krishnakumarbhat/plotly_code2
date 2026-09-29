/**
 * @file fbk_debug_writer.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations for FBK bin writer functions.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_debug_interface.h"
#include "fbk_core_calibration_t.h"
#include "fbk_guardrail_validation.h"
#include "fbk_macros.h"
#include "fbk_object_validation.h"
#include "fbk_vehicle_validation.h"
#include "pa_context.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"
#include "pa_vehicle_in.h"
#include "string.h"
#include <assert.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
 * File Scope variables
\*===========================================================================*/

static Fbk_Debug_Data_T fbk_debug_data;

/*===========================================================================*\
* Debug interface
\*===========================================================================*/

Fbk_Debug_Data_T *Fbk_Get_Debug_Data(void)
{
   return (&fbk_debug_data);
}

/*===========================================================================*\
* Helper functions to fill and reset debug data
\*===========================================================================*/

void Fbk_Debug_Reset_Data(void)
{
   memset(&fbk_debug_data, 0, sizeof(Fbk_Debug_Data_T));
}

void Fbk_Debug_Pass_General_Data(const Fbk_Core_Calibration_T *p_fbk_cal, const Pa_Data_T *p_pa_data, const Sfl_Status_T sfl_status)
{
   uint8_t idx;

   /* Assert that all passed pointers are valid. */
   assert(NULL != p_fbk_cal);
   assert(NULL != p_pa_data);

   /* Copy data from internal interfaces to debug output interface. */
   memcpy(&fbk_debug_data.fbk_calibration, p_fbk_cal, sizeof(Fbk_Core_Calibration_T));

   /* Store cycle time. */
   fbk_debug_data.fbk_debug_output.pa_time_diff_to_last_cycle = p_pa_data->time_diff_to_last_cycle;
   fbk_debug_data.fbk_debug_output.sfl_status                 = sfl_status;

   /* Store tracker object data. */
   for (idx = FBK_ZERO_UINT; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      fbk_debug_data.fbk_object_data[idx] = p_pa_data->object_data[idx];
   }

   /* Store guardrail data. */
   for (idx = FBK_ZERO_UINT; idx < PA_OBJ_NUMBER_OF_GUARDRAILS; idx++)
   {
      fbk_debug_data.fbk_guardrail_data[idx] = p_pa_data->guardrail_data[idx];
   }

   /* Store vehicle data. */
   fbk_debug_data.fbk_vehicle_data = p_pa_data->vehicle_data;
}

void Fbk_Debug_Pass_Sw_Version(const uint16_t fbk_sw_major_version, const uint16_t fbk_sw_minor_version)
{
   /* Store version from FF iface. */
   fbk_debug_data.fbk_version.fbk_sw_major_version = fbk_sw_major_version;
   fbk_debug_data.fbk_version.fbk_sw_minor_version = fbk_sw_minor_version;
}

void Fbk_Debug_Pass_Idx_Lut(const uint8_t idx_lut[PA_OBJ_NUMBER_OF_OBJECTS + FBK_ONE_UINT])
{
   uint8_t idx;

   for (idx = FBK_ZERO_UINT; idx < PA_OBJ_NUMBER_OF_OBJECTS + FBK_ONE_UINT; idx++)
   {
      fbk_debug_data.fbk_debug_output.fbk_idx_lut[idx] = idx_lut[idx];
   }
}

void Fbk_Debug_Pass_Stage_Age(const Fbk_Age_Ctr_T *p_age_ctr)
{
   uint8_t idx;
   for (idx = FBK_ZERO_UINT; idx < PA_OBJ_NUMBER_OF_OBJECTS; idx++)
   {
      fbk_debug_data.fbk_debug_output.fbk_stage_age[idx] = p_age_ctr->stage_age[idx];
      fbk_debug_data.fbk_debug_output.fbk_stage[idx]     = ((uint8_t) p_age_ctr->stage[idx]);
   }
}

void Fbk_Debug_Pass_Host_Trail(const Fbk_Host_Trail_T *p_host_trail)
{
   fbk_debug_data.fbk_host_trail = *p_host_trail;
}

#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */
