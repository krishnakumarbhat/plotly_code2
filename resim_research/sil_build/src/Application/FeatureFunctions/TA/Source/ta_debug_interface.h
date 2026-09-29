#ifndef TA_DEBUG_INTERFACE_H
#define TA_DEBUG_INTERFACE_H

/**
 * @file ta_debug_interface.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations to copy internal data to TA debug output.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#include "fbk_field_of_interest.h"
#include "pa_data.h"
#include "ta_core_calibration_t.h"
#include "ta_core_input_t.h"
#include "ta_core_output_t.h"
#include "ta_output_t.h"
#include "ta_persistent_t.h"
#include "ta_types.h"

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
* typedefs
\*===========================================================================*/

typedef struct
{
   Fbk_Trajectory_T ego_trajectory;
   Fbk_Trajectory_T most_crit_object_trajectory_left;
   Fbk_Trajectory_T most_crit_object_trajectory_right;
} Ta_Debug_Output_T;

typedef struct
{
   uint16_t ta_sw_major_version;
   uint16_t ta_sw_minor_version;
} Ta_Debug_Version_T;

typedef struct
{
   Ta_Core_Input_T ta_core_input;
   Ta_Core_Output_T ta_core_output;
   Ta_Persistent_T ta_persistent;
   Ta_Core_Calibration_T ta_calibration;
   Ta_Debug_Version_T ta_version;
   Ta_Object_Attributes_T ta_object_attributes[PA_OBJ_NUMBER_OF_OBJECTS];
   Ta_Debug_Output_T ta_debug_output;
} Ta_Debug_Data_T;

/*===========================================================================*\
* Debug interface
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

   Ta_Debug_Data_T *Ta_Get_Debug_Data(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*===========================================================================*\
* Helper functions to fill and reset debug data
\*===========================================================================*/

void Ta_Debug_Reset_Data(void);

void Ta_Debug_Pass_General_Data(const Ta_Core_Input_T *p_ta_core_input,
                                const Ta_Core_Output_T *p_ta_core_output,
                                const Ta_Persistent_T *p_ta_persistent,
                                const Ta_Core_Calibration_T *p_ta_cal);

void Ta_Debug_Pass_Sw_Version(const uint16_t ta_sw_major_version, const uint16_t ta_sw_minor_version);

void Ta_Debug_Pass_Object_Attributes(const Ta_Object_Attributes_T *p_ta_object_attributes, const uint8_t object_index);

void Ta_Debug_Pass_Ego_Data(const Fbk_Trajectory_T *p_ego_trajectory);

void Ta_Debug_Set_Side_Specific_Object_Trajectories(const Pa_Data_T *p_pa_data, const Ta_Core_Output_T *p_ta_core_output);

/*===========================================================================*\
* Macros to disable debug output in production code
\*===========================================================================*/

/* clang-format off */
#define Binary_Ta_Debug_Reset_Data()                                                                    Ta_Debug_Reset_Data()
#define Binary_Ta_Debug_Pass_General_Data(p_ta_core_input, p_ta_core_output, p_ta_persistent, p_ta_cal) Ta_Debug_Pass_General_Data(p_ta_core_input, p_ta_core_output, p_ta_persistent, p_ta_cal)
#define Binary_Ta_Debug_Pass_Sw_Version(ta_sw_major_version, ta_sw_minor_version)                       Ta_Debug_Pass_Sw_Version(ta_sw_major_version, ta_sw_minor_version)
#define Binary_Ta_Debug_Pass_Object_Attributes(p_ta_object_attributes, object_index)                    Ta_Debug_Pass_Object_Attributes(p_ta_object_attributes, object_index)
#define Binary_Ta_Debug_Pass_Ego_Data(p_ego_trajectory)                                                 Ta_Debug_Pass_Ego_Data(p_ego_trajectory)
#define Binary_Ta_Debug_Set_Side_Specific_Object_Trajectories(p_pa_data, p_ta_core_output)              Ta_Debug_Set_Side_Specific_Object_Trajectories(p_pa_data, p_ta_core_output)
/* clang-format on */

#else

#define Binary_Ta_Debug_Reset_Data()
#define Binary_Ta_Debug_Pass_General_Data(p_ta_core_input, p_ta_core_output, p_ta_persistent, p_ta_cal)
#define Binary_Ta_Debug_Pass_Sw_Version(ta_sw_major_version, ta_sw_minor_version)
#define Binary_Ta_Debug_Pass_Object_Attributes(p_ta_object_attributes, object_index)
#define Binary_Ta_Debug_Pass_Ego_Data(p_ego_trajectory)
#define Binary_Ta_Debug_Set_Side_Specific_Object_Trajectories(p_context, p_ta_core_output)

#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */

#endif /* TA_DEBUG_INTERFACE_H */
