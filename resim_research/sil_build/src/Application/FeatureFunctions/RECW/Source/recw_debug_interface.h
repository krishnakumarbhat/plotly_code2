#ifndef RECW_DEBUG_INTERFACE_H
#define RECW_DEBUG_INTERFACE_H

/**
 * @file recw_debug_interface.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations to copy internal data to RECW debug output.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#include "fbk_field_of_interest.h"
#include "fbk_vehicle_data_t.h"
#include "pa_data.h"
#include "recw_core_calibration_t.h"
#include "recw_core_input_t.h"
#include "recw_core_output_t.h"
#include "recw_output_t.h"
#include "recw_persistent_t.h"
#include "recw_types.h"

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
* typedefs
\*===========================================================================*/

#define RECW_DEBUG_NUMBER_OF_LANE_ZONE_POINTS (4u)

typedef struct
{
   Fbk_Field_Of_Interest_T lane_filter_zone;
   Fbk_Field_Of_Interest_T lane_filter_zone_hys;
} Recw_Debug_Output_T;

typedef struct
{
   uint16_t recw_sw_major_version;
   uint16_t recw_sw_minor_version;
} Recw_Debug_Version_T;

typedef struct
{
   Recw_Core_Input_T recw_core_input;
   Recw_Core_Output_T recw_core_output;
   Recw_Persistent_T recw_persistent;
   Recw_Core_Calibration_T recw_calibration;
   Recw_Debug_Version_T recw_version;
   Recw_Object_Attributes_T recw_object_attributes[PA_OBJ_NUMBER_OF_OBJECTS];
   Recw_Debug_Output_T recw_debug_output;
} Recw_Debug_Data_T;

/*===========================================================================*\
* Debug interface
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

   Recw_Debug_Data_T *Recw_Get_Debug_Data(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*===========================================================================*\
* Helper functions to fill and reset debug data
\*===========================================================================*/

void Recw_Debug_Reset_Data(void);

void Recw_Debug_Pass_General_Data(const Recw_Core_Input_T *p_recw_core_input,
                                  const Recw_Core_Output_T *p_recw_core_output,
                                  const Recw_Persistent_T *p_recw_persistent,
                                  const Recw_Core_Calibration_T *p_recw_cal);

void Recw_Debug_Pass_Sw_Version(const uint16_t recw_sw_major_version, const uint16_t recw_sw_minor_version);

void Recw_Debug_Pass_Ego_Lane_Filter_Zones(const Fbk_Vehicle_Data_T *p_vehicle_data, const Recw_Core_Calibration_T *p_recw_cal);

void Recw_Debug_Pass_Object_Attributes(const Recw_Object_Attributes_T *p_recw_object_attributes, const uint8_t object_index);

/*===========================================================================*\
* Macros to disable debug output in production code
\*===========================================================================*/

/* clang-format off */
#define Binary_Recw_Debug_Reset_Data()                                                                            Recw_Debug_Reset_Data()
#define Binary_Recw_Debug_Pass_General_Data(p_recw_core_input, p_recw_core_output, p_recw_persistent, p_recw_cal) Recw_Debug_Pass_General_Data(p_recw_core_input, p_recw_core_output, p_recw_persistent, p_recw_cal)
#define Binary_Recw_Debug_Pass_Sw_Version(recw_sw_major_version, recw_sw_minor_version)                           Recw_Debug_Pass_Sw_Version(recw_sw_major_version, recw_sw_minor_version)
#define Binary_Recw_Debug_Pass_Ego_Lane_Filter_Zones(p_vehicle_data, p_recw_cal)                                  Recw_Debug_Pass_Ego_Lane_Filter_Zones(p_vehicle_data, p_recw_cal)
#define Binary_Recw_Debug_Pass_Object_Attributes(p_recw_object_attributes, object_index)                          Recw_Debug_Pass_Object_Attributes(p_recw_object_attributes, object_index)
/* clang-format on */

#else

#define Binary_Recw_Debug_Reset_Data()
#define Binary_Recw_Debug_Pass_General_Data(p_recw_core_input, p_recw_core_output, p_recw_persistent, p_recw_cal)
#define Binary_Recw_Debug_Pass_Sw_Version(recw_sw_major_version, recw_sw_minor_version)
#define Binary_Recw_Debug_Pass_Ego_Lane_Filter_Zones(p_recw_core_input, p_recw_cal)
#define Binary_Recw_Debug_Pass_Object_Attributes(p_recw_object_attributes, object_index)

#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */

#endif /* RECW_DEBUG_INTERFACE_H */
