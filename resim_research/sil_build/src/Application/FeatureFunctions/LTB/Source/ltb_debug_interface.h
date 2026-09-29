#ifndef LTB_DEBUG_INTERFACE_H
#define LTB_DEBUG_INTERFACE_H

/**
 * @file ltb_debug_interface.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations to copy internal data to LTB debug output.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

#include "fbk_field_of_interest.h"
#include "fbk_vehicle_data_t.h"
#include "ltb_core_calibration_t.h"
#include "ltb_core_input_t.h"
#include "ltb_core_output_t.h"
#include "ltb_output_t.h"
#include "ltb_persistent_t.h"
#include "ltb_types.h"
#include "pa_data.h"

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
* typedefs
\*===========================================================================*/

typedef struct
{
   /* Max alert level (computed individually of LTB algo). */
   Fbk_Trajectory_T ego_trajectory;
   Fbk_Trajectory_T most_crit_object_trajectory_left;
   Fbk_Trajectory_T most_crit_object_trajectory_right;

} Ltb_Debug_Output_T;

typedef struct
{
   uint16_t ltb_sw_major_version;
   uint16_t ltb_sw_minor_version;
} Ltb_Debug_Version_T;

typedef struct
{
   Ltb_Core_Input_T ltb_core_input;
   Ltb_Core_Output_T ltb_core_output;
   Ltb_Persistent_T ltb_persistent;
   Ltb_Core_Calibration_T ltb_calibration;
   Ltb_Debug_Version_T ltb_version;
   Ltb_Object_Attributes_T ltb_object_attributes[PA_OBJ_NUMBER_OF_OBJECTS];
   Ltb_Debug_Output_T ltb_debug_output;
} Ltb_Debug_Data_T;

/*===========================================================================*\
* Debug interface
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

   Ltb_Debug_Data_T *Ltb_Get_Debug_Data(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*===========================================================================*\
* Helper functions to fill and reset debug data
\*===========================================================================*/

void Ltb_Debug_Reset_Data(void);

void Ltb_Debug_Pass_General_Data(const Ltb_Core_Input_T *p_ltb_core_input,
                                 const Ltb_Core_Output_T *p_ltb_core_output,
                                 const Ltb_Persistent_T *p_ltb_persistent,
                                 const Ltb_Core_Calibration_T *p_ltb_cal);

void Ltb_Debug_Pass_Sw_Version(const uint16_t ltb_sw_major_version, const uint16_t ltb_sw_minor_version);

void Ltb_Debug_Pass_Ego_Data(const Fbk_Trajectory_T *p_ego_trajectory);

void Ltb_Debug_Pass_Object_Attributes(const Ltb_Object_Attributes_T *p_ltb_object_attributes, const uint8_t object_index);

void Ltb_Debug_Set_Side_Specific_Object_Trajectories(const Pa_Data_T *p_pa_data, const Ltb_Core_Output_T *p_ltb_core_output);

/*===========================================================================*\
* Macros to disable debug output in production code
\*===========================================================================*/

/* clang-format off */
#define Binary_Ltb_Debug_Reset_Data()                                                                        Ltb_Debug_Reset_Data()
#define Binary_Ltb_Debug_Pass_General_Data(p_ltb_core_input, p_ltb_core_output, p_ltb_persistent, p_ltb_cal) Ltb_Debug_Pass_General_Data(p_ltb_core_input, p_ltb_core_output, p_ltb_persistent, p_ltb_cal)
#define Binary_Ltb_Debug_Pass_Sw_Version(ltb_sw_major_version, ltb_sw_minor_version)                         Ltb_Debug_Pass_Sw_Version(ltb_sw_major_version, ltb_sw_minor_version)
#define Binary_Ltb_Debug_Pass_Ego_Data(p_ego_trajectory)                                                     Ltb_Debug_Pass_Ego_Data(p_ego_trajectory)
#define Binary_Ltb_Debug_Pass_Object_Attributes(p_ltb_object_attributes, object_index)                       Ltb_Debug_Pass_Object_Attributes(p_ltb_object_attributes, object_index)
#define Binary_Ltb_Debug_Set_Side_Specific_Object_Trajectories(p_pa_data, p_ltb_core_output)                 Ltb_Debug_Set_Side_Specific_Object_Trajectories(p_pa_data, p_ltb_core_output)

/* clang-format on */

#else

#define Binary_Ltb_Debug_Reset_Data()
#define Binary_Ltb_Debug_Pass_General_Data(p_ltb_core_input, p_ltb_core_output, p_ltb_persistent, p_ltb_cal)
#define Binary_Ltb_Debug_Pass_Sw_Version(ltb_sw_major_version, ltb_sw_minor_version)
#define Binary_Ltb_Debug_Pass_Ego_Data(p_ego_trajectory)
#define Binary_Ltb_Debug_Pass_Object_Attributes(p_ltb_object, object_index)
#define Binary_Ltb_Debug_Set_Side_Specific_Object_Trajectories(p_context, p_ltb_core_output)

#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */

#endif /* LTB_DEBUG_INTERFACE_H */
