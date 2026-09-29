#ifndef LCDA_DEBUG_INTERFACE_H
#define LCDA_DEBUG_INTERFACE_H

/**
 * @file lcda_debug_writer.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations for LCDA bin writer functions.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "fbk_vehicle_data_t.h"
#include "lcda_core_calibration_t.h"
#include "lcda_core_input_t.h"
#include "lcda_core_output_t.h"
#include "lcda_input_t.h"
#include "lcda_output_t.h"
#include "lcda_persistent_t.h"
#include "lcda_types.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
* typedefs
\*===========================================================================*/

typedef struct
{
   /* BSW data */
   Bsw_Object_T bsw_object_data[PA_OBJ_NUMBER_OF_OBJECTS];
   Lcda_Bsw_Persistent_T bsw_persistent;
   Fbk_Field_Of_Interest_T bsw_default_zone;

   /* CVW data */
   Cvw_Object_T cvw_object_data[PA_OBJ_NUMBER_OF_OBJECTS];
   Lcda_Cvw_Persistent_T cvw_persistent;
   Fbk_Field_Of_Interest_T cvw_default_zone;

   /* ELC data */
   Elc_Object_T elc_object_data[PA_OBJ_NUMBER_OF_OBJECTS];
   Lcda_Elc_Persistent_T elc_persistent;
   Fbk_Field_Of_Interest_T elc_default_zone;

   /* SLC data */
   Slc_Object_T slc_object_data[PA_OBJ_NUMBER_OF_OBJECTS];
   Lcda_Slc_Persistent_T slc_persistent;
   Fbk_Field_Of_Interest_T slc_default_zone;

   /*LCDA common data*/
   Vector_2d_T obj_ref_point_bsw[PA_OBJ_NUMBER_OF_OBJECTS];
   Vector_2d_T obj_ref_point_cvw[PA_OBJ_NUMBER_OF_OBJECTS];

} Lcda_Debug_Output_T;

typedef struct
{
   uint16_t lcda_sw_major_version;
   uint16_t lcda_sw_minor_version;
} Lcda_Debug_Version_T;

typedef struct
{
   Lcda_Core_Input_T lcda_core_input;
   Lcda_Core_Output_T lcda_core_output;
   Lcda_Persistent_T lcda_persistent;
   Lcda_Core_Calibration_T lcda_calibration;
   Lcda_Debug_Version_T lcda_version;
   Lcda_Debug_Output_T lcda_debug_output;
} Lcda_Debug_Data_T;

typedef enum
{
   LCDA_UNDEF = (0),
   LCDA_BSW   = (1),
   LCDA_CVW   = (2),
   LCDA_SLC   = (3),
   LCDA_ELC   = (4)
   /* coverity[misra_c_2012_rule_2_3_violation][Type definition needed to comply with Aptiv Coding Guideline (C77)] */
} Lcda_Processed_Module;

/*===========================================================================*\
* Debug interface
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

   Lcda_Debug_Data_T *Lcda_Get_Debug_Data(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*===========================================================================*\
* Helper functions to fill and reset debug data
\*===========================================================================*/

void Lcda_Debug_Reset_Data(void);

void Lcda_Debug_Pass_General_Data(const Lcda_Core_Input_T *p_lcda_core_input,
                                  const Lcda_Core_Output_T *p_lcda_core_output,
                                  const Lcda_Persistent_T *p_lcda_persistent,
                                  const Lcda_Core_Calibration_T *p_lcda_cal);

void Lcda_Debug_Pass_Sw_Version(const uint16_t lcda_sw_major_version, const uint16_t lcda_sw_minor_version);

void Lcda_Debug_Pass_Bsw_Persistent_Data(Lcda_Bsw_Persistent_T *p_Bsw_Persistent_data);
void Lcda_Debug_Pass_Cvw_Persistent_Data(Lcda_Cvw_Persistent_T *p_cvw_persistent_data);
void Lcda_Debug_Pass_Elc_Persistent_Data(Lcda_Elc_Persistent_T *p_elc_persistent_data);
void Lcda_Debug_Pass_Slc_Persistent_Data(Lcda_Slc_Persistent_T *p_slc_persistent_data);

void Lcda_Debug_Pass_Bsw_Object_Attributes(Bsw_Object_T *p_bsw_object);
void Lcda_Debug_Pass_Cvw_Object_Attributes(Cvw_Object_T *p_cvw_object);
void Lcda_Debug_Pass_Elc_Object_Attributes(Elc_Object_T *p_elc_object);
void Lcda_Debug_Pass_Slc_Object_Attributes(Slc_Object_T *p_slc_object);

void Lcda_Debug_Pass_Bsw_Default_Zone(const Lcda_Core_Input_T *p_core_input,
                                      const Fbk_Vehicle_Data_T *p_vehicle_data,
                                      const Lcda_Core_Calibration_T *p_cals);
void Lcda_Debug_Pass_Cvw_Default_Zone(const Lcda_Core_Input_T *p_core_input,
                                      const Fbk_Vehicle_Data_T *p_vehicle_data,
                                      const Lcda_Core_Calibration_T *p_cals,
                                      Lcda_Cvw_Persistent_T *p_cvw_persistent);
void Lcda_Debug_Pass_Elc_Default_Zone(const Lcda_Core_Input_T *p_core_input, const Lcda_Core_Calibration_T *p_cals);
void Lcda_Debug_Pass_Slc_Default_Zone(const Lcda_Core_Input_T *p_core_input, const Lcda_Core_Calibration_T *p_cals);
void Lcda_Debug_Pass_Object_Ref_Point(const Vector_2d_T *p_ref_point, const uint8_t object_id);
void Lcda_Debug_Pass_Processed_Submodule(const Lcda_Processed_Module);

/*===========================================================================*\
* Macros to disable debug output in production code
\*===========================================================================*/

/* clang-format off */
#define Binary_Lcda_Debug_Reset_Data()                                                                            Lcda_Debug_Reset_Data()
#define Binary_Lcda_Debug_Pass_General_Data(p_lcda_core_input, p_lcda_core_output, p_lcda_persistent, p_lcda_cal) Lcda_Debug_Pass_General_Data(p_lcda_core_input, p_lcda_core_output, p_lcda_persistent, p_lcda_cal)
#define Binary_Lcda_Debug_Pass_Sw_Version(lcda_sw_major_version, lcda_sw_minor_version)                           Lcda_Debug_Pass_Sw_Version(lcda_sw_major_version, lcda_sw_minor_version)
#define Binary_Lcda_Debug_Pass_Bsw_Persistent_Data(p_Bsw_Persistent_data)                                         Lcda_Debug_Pass_Bsw_Persistent_Data(p_Bsw_Persistent_data)
#define Binary_Lcda_Debug_Pass_Cvw_Persistent_Data(p_cvw_persistent_data)                                         Lcda_Debug_Pass_Cvw_Persistent_Data(p_cvw_persistent_data)
#define Binary_Lcda_Debug_Pass_Elc_Persistent_Data(p_elc_persistent_data)                                         Lcda_Debug_Pass_Elc_Persistent_Data(p_elc_persistent_data)
#define Binary_Lcda_Debug_Pass_Slc_Persistent_Data(p_slc_persistent_data)                                         Lcda_Debug_Pass_Slc_Persistent_Data(p_slc_persistent_data)
#define Binary_Lcda_Debug_Pass_Bsw_Object_Attributes(p_bsw_object)                                                Lcda_Debug_Pass_Bsw_Object_Attributes(p_bsw_object)
#define Binary_Lcda_Debug_Pass_Cvw_Object_Attributes(p_cvw_object)                                                Lcda_Debug_Pass_Cvw_Object_Attributes(p_cvw_object)
#define Binary_Lcda_Debug_Pass_Elc_Object_Attributes(p_slc_object)                                                Lcda_Debug_Pass_Elc_Object_Attributes(p_slc_object)
#define Binary_Lcda_Debug_Pass_Slc_Object_Attributes(p_elc_object)                                                Lcda_Debug_Pass_Slc_Object_Attributes(p_elc_object)
#define Binary_Lcda_Debug_Pass_Bsw_Default_Zone(p_core_input, p_vehicle_data, p_cals)                             Lcda_Debug_Pass_Bsw_Default_Zone(p_core_input, p_vehicle_data, p_cals)
#define Binary_Lcda_Debug_Pass_Cvw_Default_Zone(p_core_input, p_vehicle_data, p_cals, p_cvw_persistent)           Lcda_Debug_Pass_Cvw_Default_Zone(p_core_input, p_vehicle_data, p_cals, p_cvw_persistent)
#define Binary_Lcda_Debug_Pass_Elc_Default_Zone(p_core_input, p_cals)                                             Lcda_Debug_Pass_Elc_Default_Zone(p_core_input, p_cals)
#define Binary_Lcda_Debug_Pass_Slc_Default_Zone(p_core_input, p_cals)                                             Lcda_Debug_Pass_Slc_Default_Zone(p_core_input, p_cals)
#define Binary_Lcda_Debug_Pass_Object_Ref_Point(p_ref_point, object_id)                                           Lcda_Debug_Pass_Object_Ref_Point(p_ref_point, object_id)
#define Binary_Lcda_Debug_Pass_Processed_Submodule(lcda_processed_module)                                         Lcda_Debug_Pass_Processed_Submodule(lcda_processed_module)
/* clang-format on */

#else

#define Binary_Lcda_Debug_Reset_Data()
#define Binary_Lcda_Debug_Pass_General_Data(p_lcda_core_input, p_lcda_core_output, p_lcda_persistent, p_lcda_cal)
#define Binary_Lcda_Debug_Pass_Sw_Version(lcda_sw_major_version, lcda_sw_minor_version)
#define Binary_Lcda_Debug_Pass_Bsw_Persistent_Data(p_Bsw_Persistent_data)
#define Binary_Lcda_Debug_Pass_Cvw_Persistent_Data(p_cvw_persistent_data)
#define Binary_Lcda_Debug_Pass_Elc_Persistent_Data(p_elc_persistent_data)
#define Binary_Lcda_Debug_Pass_Slc_Persistent_Data(p_slc_persistent_data)
#define Binary_Lcda_Debug_Pass_Bsw_Object_Attributes(p_bsw_object)
#define Binary_Lcda_Debug_Pass_Cvw_Object_Attributes(p_cvw_object)
#define Binary_Lcda_Debug_Pass_Elc_Object_Attributes(p_slc_object)
#define Binary_Lcda_Debug_Pass_Slc_Object_Attributes(p_elc_object)
#define Binary_Lcda_Debug_Pass_Bsw_Default_Zone(p_core_input, p_vehicle_data, p_cals)
#define Binary_Lcda_Debug_Pass_Cvw_Default_Zone(p_core_input, p_vehicle_data, p_cals, p_cvw_persistent)
#define Binary_Lcda_Debug_Pass_Elc_Default_Zone(p_core_input, p_cals)
#define Binary_Lcda_Debug_Pass_Slc_Default_Zone(p_core_input, p_cals)
#define Binary_Lcda_Debug_Pass_Object_Ref_Point(p_ref_point, object_id)
#define Binary_Lcda_Debug_Pass_Processed_Submodule(lcda_processed_module)
#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */

#endif /* LCDA_DEBUG_INTERFACE_H */
