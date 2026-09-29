#ifndef FBK_DEBUG_INTERFACE_H
#define FBK_DEBUG_INTERFACE_H

/**
 * @file fbk_debug_writer.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations for FBK bin writer functions.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_core_calibration_t.h"
#include "fbk_guardrail_data_t.h"
#include "fbk_host_trail.h"
#include "fbk_iface_types.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "pa_context.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"
#include "sfl_status.h"

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
* typedefs
\*===========================================================================*/

typedef struct
{
   /* Debug data for index lookup calculation*/
   uint8_t fbk_idx_lut[PA_OBJ_NUMBER_OF_OBJECTS + 1u];

   /* Debug data stage age*/
   uint8_t fbk_stage[PA_OBJ_NUMBER_OF_OBJECTS];
   uint8_t fbk_stage_age[PA_OBJ_NUMBER_OF_OBJECTS];

   /* Timestep since last cycle. */
   float32_T pa_time_diff_to_last_cycle;
   Sfl_Status_T sfl_status;

} Fbk_Debug_Output_T;

typedef struct
{
   uint16_t fbk_sw_major_version;
   uint16_t fbk_sw_minor_version;
} Fbk_Debug_Version_T;

typedef struct
{
   Fbk_Core_Calibration_T fbk_calibration;
   Fbk_Debug_Version_T fbk_version;
   Fbk_Object_Data_T fbk_object_data[PA_OBJ_NUMBER_OF_OBJECTS];
   Fbk_Guardrail_Data_T fbk_guardrail_data[PA_OBJ_NUMBER_OF_GUARDRAILS];
   Fbk_Vehicle_Data_T fbk_vehicle_data;
   Fbk_Host_Trail_T fbk_host_trail;
   Fbk_Debug_Output_T fbk_debug_output;
} Fbk_Debug_Data_T;

/*===========================================================================*\
* Debug interface
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

   Fbk_Debug_Data_T *Fbk_Get_Debug_Data(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*===========================================================================*\
* Helper functions to fill and reset debug data
\*===========================================================================*/

void Fbk_Debug_Reset_Data(void);
void Fbk_Debug_Pass_General_Data(const Fbk_Core_Calibration_T *p_fbk_cal, const Pa_Data_T *p_pa_data, const Sfl_Status_T sfl_status);
void Fbk_Debug_Pass_Sw_Version(const uint16_t fbk_sw_major_version, const uint16_t fbk_sw_minor_version);
void Fbk_Debug_Pass_Idx_Lut(const uint8_t idx_lut[PA_OBJ_NUMBER_OF_OBJECTS + FBK_ONE_UINT]);
void Fbk_Debug_Pass_Stage_Age(const Fbk_Age_Ctr_T *p_age_ctr);
void Fbk_Debug_Pass_Host_Trail(const Fbk_Host_Trail_T *p_host_trail);

/*===========================================================================*\
* Macros to disable debug output in production code
\*===========================================================================*/

/* clang-format off */
#define Binary_Fbk_Debug_Reset_Data()                                                Fbk_Debug_Reset_Data()
#define Binary_Fbk_Debug_Pass_General_Data(p_fbk_cal, p_pa_data, sfl_status)         Fbk_Debug_Pass_General_Data(p_fbk_cal, p_pa_data, sfl_status)
#define Binary_Fbk_Debug_Pass_Sw_Version(fbk_sw_major_version, fbk_sw_minor_version) Fbk_Debug_Pass_Sw_Version(fbk_sw_major_version, fbk_sw_minor_version)
#define Binary_Fbk_Debug_Pass_Idx_Lut(idx_lut)                                       Fbk_Debug_Pass_Idx_Lut(idx_lut)
#define Binary_Fbk_Debug_Pass_Stage_Age(p_age_ctr)                                   Fbk_Debug_Pass_Stage_Age(p_age_ctr)
#define Binary_Fbk_Debug_Pass_Host_Trail(p_host_trail)                               Fbk_Debug_Pass_Host_Trail(p_host_trail)
/* clang-format on */

#else

#define Binary_Fbk_Debug_Reset_Data()
#define Binary_Fbk_Debug_Pass_General_Data(p_fbk_cal, p_pa_data, sfl_status)
#define Binary_Fbk_Debug_Pass_Sw_Version(fbk_sw_major_version, fbk_sw_minor_version)
#define Binary_Fbk_Debug_Pass_Idx_Lut(idx_lut)
#define Binary_Fbk_Debug_Pass_Stage_Age(p_age_ctr)
#define Binary_Fbk_Debug_Pass_Host_Trail(p_host_trail)

#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */

#endif /* FBK_DEBUG_INTERFACE_H */
