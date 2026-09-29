#ifndef SCW_DEBUG_INTERFACE_H
#define SCW_DEBUG_INTERFACE_H

/**
 * @file scw_debug_interface.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations to copy internal data to SCW debug output.
 *
 * @copyright Copyright (C) 2025 Aptiv. All rights reserved.
 */

#include "fbk_field_of_interest.h"
#include "pa_context.h"
#include "scw_core_calibration_t.h"
#include "scw_core_input_t.h"
#include "scw_core_output_t.h"
#include "scw_output_t.h"
#include "scw_persistent_t.h"
#include "scw_types.h"

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
* typedefs
\*===========================================================================*/

typedef struct
{
   // The counter values represent
   // 0: Object not valid
   // 1: Object valid
   // 2: Object relevant
   // 3: Object in zone
   uint8_t scw_object_importance_counter[PA_OBJ_NUMBER_OF_OBJECTS + FBK_ONE_UINT];

   Fbk_Field_Of_Interest_T scw_zone;
   Fbk_Field_Of_Interest_T scw_hysteresis_zone;

} Scw_Debug_Output_T;

typedef struct
{
   uint16_t scw_sw_major_version;
   uint16_t scw_sw_minor_version;
} Scw_Debug_Version_T;

typedef struct Scw_Debug_Data_Tag
{
   Scw_Core_Input_T scw_core_input;
   Scw_Core_Output_T scw_core_output;
   Scw_Persistent_T scw_persistent;
   Scw_Core_Calibration_T scw_calibration;
   Scw_Debug_Version_T scw_version;
   Scw_Debug_Output_T scw_debug_output;
} Scw_Debug_Data_T;

/*===========================================================================*\
* Debug interface
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

   Scw_Debug_Data_T *Scw_Get_Debug_Data(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*===========================================================================*\
* Helper functions to fill and reset debug data
\*===========================================================================*/

void Scw_Debug_Reset_Data(void);

void Scw_Debug_Pass_General_Data(const Scw_Core_Input_T *p_scw_core_input,
                                 const Scw_Core_Output_T *p_scw_core_output,
                                 const Scw_Persistent_T *p_scw_persistent,
                                 const Scw_Core_Calibration_T *p_scw_cal);

void Scw_Debug_Pass_Sw_Version(const uint16_t scw_sw_major_version, const uint16_t scw_sw_minor_version);

void Scw_Debug_Pass_Zones(const Fbk_Field_Of_Interest_T *const p_scw_zone, const Fbk_Field_Of_Interest_T *const p_scw_hysteresis_zone);

void Scw_Debug_Increase_Object_Importance_Counter(const uint8_t obj_id);

/*===========================================================================*\
* Macros to disable debug output in production code
\*===========================================================================*/

/* clang-format off */
#define Binary_Scw_Debug_Reset_Data()                                                                        Scw_Debug_Reset_Data()
#define Binary_Scw_Debug_Pass_General_Data(p_ced_core_input, p_ced_core_output, p_ced_persistent, p_ced_cal) Scw_Debug_Pass_General_Data(p_ced_core_input, p_ced_core_output, p_ced_persistent, p_ced_cal)
#define Binary_Scw_Debug_Pass_Sw_Version(scw_sw_major_version, scw_sw_minor_version)                         Scw_Debug_Pass_Sw_Version(scw_sw_major_version, scw_sw_minor_version)
#define Binary_Scw_Debug_Pass_Zones(p_scw_zone, p_scw_hysteresis_zone)                                       Scw_Debug_Pass_Zones(p_scw_zone, p_scw_hysteresis_zone)
#define Binary_Scw_Debug_Increase_Object_Importance_Counter(obj_id)                                          Scw_Debug_Increase_Object_Importance_Counter(obj_id)
/* clang-format on */

#else

#define Binary_Scw_Debug_Reset_Data()
#define Binary_Scw_Debug_Pass_General_Data(p_ced_core_input, p_ced_core_output, p_ced_persistent, p_ced_cal)
#define Binary_Scw_Debug_Pass_Sw_Version(scw_sw_major_version, scw_sw_minor_version)
#define Binary_Scw_Debug_Pass_Zones(p_scw_zone, p_scw_hysteresis_zone)
#define Binary_Scw_Debug_Increase_Object_Importance_Counter(obj_id)

#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */

#endif /* SCW_DEBUG_INTERFACE_H */
