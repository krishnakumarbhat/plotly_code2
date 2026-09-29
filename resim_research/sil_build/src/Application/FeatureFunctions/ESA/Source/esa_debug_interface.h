#ifndef ESA_DEBUG_INTERFACE_H
#define ESA_DEBUG_INTERFACE_H

/**
 * @file esa_debug_interface.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations to copy internal data to ESA debug output.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

#include "esa_core_calibration_t.h"
#include "esa_core_input_t.h"
#include "esa_core_output_t.h"
#include "esa_output_t.h"
#include "esa_persistent_t.h"
#include "esa_types.h"
#include "fbk_field_of_interest.h"
#include "fbk_vehicle_data_t.h"
#include "pa_context.h"

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
* typedefs
\*===========================================================================*/

typedef struct
{
   Esa_Object_T esa_object_data[PA_OBJ_NUMBER_OF_OBJECTS];
   Esa_Persistent_T esa_persistent;
   Fbk_Field_Of_Interest_T esa_default_zone;
} Esa_Debug_Output_T;

typedef struct
{
   uint16_t esa_sw_major_version;
   uint16_t esa_sw_minor_version;
} Esa_Debug_Version_T;

typedef struct
{
   Esa_Core_Input_T esa_core_input;
   Esa_Core_Output_T esa_core_output;
   Esa_Core_Calibration_T esa_calibration;
   Esa_Debug_Version_T esa_version;
   Esa_Debug_Output_T esa_debug_output;
} Esa_Debug_Data_T;

/*===========================================================================*\
* Debug interface
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

   Esa_Debug_Data_T *Esa_Get_Debug_Data(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*===========================================================================*\
* Helper functions to fill and reset debug data
\*===========================================================================*/

void Esa_Debug_Reset_Data(void);

void Esa_Debug_Pass_General_Data(const Esa_Core_Input_T *p_esa_core_input,
                                 const Esa_Core_Output_T *p_esa_core_output,
                                 const Esa_Persistent_T *p_esa_persistent,
                                 const Esa_Core_Calibration_T *p_esa_calibration);

void Esa_Debug_Pass_Sw_Version(const uint16_t esa_sw_major_version, const uint16_t esa_sw_minor_version);

void Esa_Debug_Pass_Persistent_Data(Esa_Persistent_T *p_esa_persistent);

void Esa_Debug_Pass_Default_Zone(const Esa_Core_Input_T *p_core_input, const Esa_Core_Calibration_T *p_esa_calibration);

void Esa_Debug_Pass_Object_Attributes(const Esa_Object_T *p_esa_object);

/*===========================================================================*\
* Macros to disable debug output in production code
\*===========================================================================*/

/* clang-format off */
#define Binary_Esa_Debug_Reset_Data()                                                                        Esa_Debug_Reset_Data()
#define Binary_Esa_Debug_Pass_General_Data(p_esa_core_input, p_esa_core_output, p_esa_persistent, p_esa_calibration) Esa_Debug_Pass_General_Data(p_esa_core_input, p_esa_core_output, p_esa_persistent, p_esa_calibration)
#define Binary_Esa_Debug_Pass_Sw_Version(esa_sw_major_version, esa_sw_minor_version)                         Esa_Debug_Pass_Sw_Version(esa_sw_major_version, esa_sw_minor_version)
#define Binary_Esa_Debug_Pass_Persistent_Data(p_esa_persistent)                                              Esa_Debug_Pass_Persistent_Data(p_esa_persistent)
#define Binary_Esa_Debug_Pass_Default_Zone(p_core_input, p_esa_calibration)                                          Esa_Debug_Pass_Default_Zone(p_core_input, p_esa_calibration)
#define Binary_Esa_Debug_Pass_Object_Attributes(p_esa_object)                                                Esa_Debug_Pass_Object_Attributes(p_esa_object)
/* clang-format on */

#else

#define Binary_Esa_Debug_Reset_Data()
#define Binary_Esa_Debug_Pass_General_Data(p_esa_core_input, p_esa_core_output, p_esa_persistent, p_esa_calibration)
#define Binary_Esa_Debug_Pass_Sw_Version(esa_sw_major_version, esa_sw_minor_version)
#define Binary_Esa_Debug_Pass_Persistent_Data(p_esa_persistent)
#define Binary_Esa_Debug_Pass_Default_Zone(p_core_input, p_esa_calibration)
#define Binary_Esa_Debug_Pass_Object_Attributes(p_esa_object)

#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */

#endif /* ESA_DEBUG_INTERFACE_H */
