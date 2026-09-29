#ifndef CED_DEBUG_INTERFACE_H
#define CED_DEBUG_INTERFACE_H

/**
 * @file ced_debug_interface.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations to copy internal data to CED debug output.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#include "ced_core_calibration_t.h"
#include "ced_core_input_t.h"
#include "ced_core_output_t.h"
#include "ced_output_t.h"
#include "ced_persistent_t.h"
#include "ced_types.h"
#include "fbk_field_of_interest.h"
#include "fbk_vehicle_data_t.h"

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
* typedefs
\*===========================================================================*/

typedef struct
{
   /* CED Zones */
   float32_T ced_funnel_zone_x[CED_NUMBER_OF_ZONE_POINTS];
   float32_T ced_funnel_zone_y[CED_NUMBER_OF_ZONE_POINTS];

   float32_T ced_collision_zone_x[CED_NUMBER_OF_ZONE_POINTS];
   float32_T ced_collision_zone_y[CED_NUMBER_OF_ZONE_POINTS];

   float32_T ced_crash_line_rear;
   float32_T ced_crash_line_front;

   /* Max alert level (computed individually of CED algo). */
   uint8_t ced_max_alertlevel;

   /**
    * The counter values represent
    * 0: Object not valid
    * 1: Object valid
    * 2: Object relevant
    * 3: Object collision critical
    */
   uint8_t ced_object_importance_counter[PA_OBJ_NUMBER_OF_OBJECTS];

   Ced_Alert_Suppression_T ced_object_alert_suppression_reason[PA_OBJ_NUMBER_OF_OBJECTS];

   /* CED side specific (int) */
   uint8_t ced_side_object_id_internal[FBK_NUMBER_OF_SIDES];
   uint8_t ced_side_alert_internal[FBK_NUMBER_OF_SIDES];

   /* Object to Path Match index */
   uint8_t ced_path_match_index[PA_OBJ_NUMBER_OF_OBJECTS];

} Ced_Debug_Output_T;

typedef struct
{
   uint16_t ced_sw_major_version;
   uint16_t ced_sw_minor_version;
} Ced_Debug_Version_T;

typedef struct
{
   Ced_Core_Input_T ced_core_input;
   Ced_Core_Output_T ced_core_output;
   Ced_Persistent_T ced_persistent;
   Ced_Core_Calibration_T ced_calibration;
   Ced_Debug_Version_T ced_version;
   Ced_Object_Attributes_T ced_object_attributes[PA_OBJ_NUMBER_OF_OBJECTS];
   Ced_Debug_Output_T ced_debug_output;
} Ced_Debug_Data_T;

/*===========================================================================*\
* Debug interface
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

   Ced_Debug_Data_T *Ced_Get_Debug_Data(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*===========================================================================*\
* Helper functions to fill and reset debug data
\*===========================================================================*/

void Ced_Debug_Reset_Data(void);

void Ced_Debug_Pass_General_Data(const Ced_Core_Input_T *p_ced_core_input,
                                 const Ced_Core_Output_T *p_ced_core_output,
                                 const Ced_Persistent_T *p_ced_persistent,
                                 const Ced_Core_Calibration_T *p_ced_cal);

void Ced_Debug_Pass_Sw_Version(const uint16_t ced_sw_major_version, const uint16_t ced_sw_minor_version);

void Ced_Debug_Pass_Zones(const Fbk_Field_Of_Interest_T *p_funnel_zone,
                          const Fbk_Field_Of_Interest_T *p_collision_zone,
                          const Fbk_Vehicle_Data_T *p_vehicle_data,
                          const Ced_Core_Calibration_T *p_ced_cal);

void Ced_Debug_Pass_Object_Attributes(const Ced_Object_Attributes_T *p_ced_object_attributes, const uint8_t object_index);

void Ced_Debug_Pass_Object_To_Path_Match_Index(const Ced_Object_T *p_ced_object, const uint8_t object_index);

void Ced_Debug_Pass_Side_Data(const Ced_Object_T *p_ced_object, uint8_t side_index);

void Ced_Debug_Increase_Object_Importance_Counter(const uint8_t object_index);

void Ced_Debug_Pass_Object_Alert_Suppression_Reason(const Ced_Alert_Suppression_T ced_alert_suppression_reason,
                                                    const uint8_t object_index);

/*===========================================================================*\
* Macros to disable debug output in production code
\*===========================================================================*/

/* clang-format off */
#define Binary_Ced_Debug_Reset_Data()                                                                        Ced_Debug_Reset_Data()
#define Binary_Ced_Debug_Pass_General_Data(p_ced_core_input, p_ced_core_output, p_ced_persistent, p_ced_cal) Ced_Debug_Pass_General_Data(p_ced_core_input, p_ced_core_output, p_ced_persistent, p_ced_cal)
#define Binary_Ced_Debug_Pass_Sw_Version(ced_sw_major_version, ced_sw_minor_version)                         Ced_Debug_Pass_Sw_Version(ced_sw_major_version, ced_sw_minor_version)
#define Binary_Ced_Debug_Pass_Zones(p_funnel_zone, p_collision_zone, p_vehicle_data, p_ced_cal)              Ced_Debug_Pass_Zones(p_funnel_zone, p_collision_zone, p_vehicle_data, p_ced_cal)
#define Binary_Ced_Debug_Pass_Object_Attributes(p_ced_object_attributes, object_index)                       Ced_Debug_Pass_Object_Attributes(p_ced_object_attributes, object_index)
#define Binary_Ced_Debug_Pass_Object_To_Path_Match_Index(p_ced_object, object_index)                         Ced_Debug_Pass_Object_To_Path_Match_Index(p_ced_object, object_index)
#define Binary_Ced_Debug_Pass_Side_Data(p_ced_object, side_index)                                            Ced_Debug_Pass_Side_Data(p_ced_object, side_index)
#define Binary_Ced_Debug_Increase_Object_Importance_Counter(object_index)                                    Ced_Debug_Increase_Object_Importance_Counter(object_index)
#define Binary_Ced_Debug_Pass_Object_Alert_Suppression_Reason(ced_alert_suppression_reason, object_index)    Ced_Debug_Pass_Object_Alert_Suppression_Reason(ced_alert_suppression_reason, object_index)
/* clang-format on */

#else

#define Binary_Ced_Debug_Reset_Data()
#define Binary_Ced_Debug_Pass_General_Data(p_ced_core_input, p_ced_core_output, p_ced_persistent, p_ced_cal)
#define Binary_Ced_Debug_Pass_Sw_Version(ced_sw_major_version, ced_sw_minor_version)
#define Binary_Ced_Debug_Pass_Zones(p_funnel_zone, p_collision_zone, p_vehicle_data, p_ced_cal)
#define Binary_Ced_Debug_Pass_Object_Attributes(p_ced_object, object_index)
#define Binary_Ced_Debug_Pass_Object_To_Path_Match_Index(p_ced_object, object_index)
#define Binary_Ced_Debug_Pass_Side_Data(p_ced_object, side_index)
#define Binary_Ced_Debug_Increase_Object_Importance_Counter(object_index)
#define Binary_Ced_Debug_Pass_Object_Alert_Suppression_Reason(ced_alert_suppression_reason, object_index)

#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */

#endif /* CED_DEBUG_INTERFACE_H */
