#ifndef CTA_DEBUG_INTERFACE_H
#define CTA_DEBUG_INTERFACE_H

/**
 * @file cta_debug_writer.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations for CTA bin writer functions.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_core_calibration_t.h"
#include "cta_core_input_t.h"
#include "cta_core_output_t.h"
#include "cta_input_t.h"
#include "cta_instance.h"
#include "cta_output_t.h"
#include "cta_persistent_t.h"
#include "cta_types.h"
#include "fbk_macros.h"
#include "fbk_vehicle_data_t.h"
#include "ml_float_range_t.h"
#include "ml_vector_2d.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
* typedefs
\*===========================================================================*/

typedef struct
{
   /* Intersection zones where all factors are already applied on as well as hysteresis */
   Float_Range_T cta_isect_rcta_level_one[PA_OBJ_NUMBER_OF_OBJECTS];
   Float_Range_T cta_isect_rcta_level_two[PA_OBJ_NUMBER_OF_OBJECTS];
   Float_Range_T cta_isect_fcta_level_one[PA_OBJ_NUMBER_OF_OBJECTS];
   Float_Range_T cta_isect_fcta_level_two[PA_OBJ_NUMBER_OF_OBJECTS];

   /* TTC zones where hysteresis may be already applied */
   float32_T cta_ttc_rcta_level_one[PA_OBJ_NUMBER_OF_OBJECTS];
   float32_T cta_ttc_rcta_level_two[PA_OBJ_NUMBER_OF_OBJECTS];
   float32_T cta_ttc_fcta_level_one[PA_OBJ_NUMBER_OF_OBJECTS];
   float32_T cta_ttc_fcta_level_two[PA_OBJ_NUMBER_OF_OBJECTS];

   /* Object attributes which are interesting for debug purposes */
   int8_t cta_index_of_path_match_to_obj[PA_OBJ_NUMBER_OF_OBJECTS];
   uint8_t crit_level_rcta[PA_OBJ_NUMBER_OF_OBJECTS];
   uint8_t crit_level_fcta[PA_OBJ_NUMBER_OF_OBJECTS];

   /* Reached branches */
   uint8_t f_object_is_valid[PA_OBJ_NUMBER_OF_OBJECTS];

   /* Persistent Data */
   int16_t cta_most_critical_rcta_obj_index_left;
   int16_t cta_most_critical_rcta_obj_index_right;
   int16_t cta_most_critical_fcta_obj_index_left;
   int16_t cta_most_critical_fcta_obj_index_right;

} Cta_Debug_Internals_T;

typedef struct
{
   uint16_t cta_sw_major_version;
   uint16_t cta_sw_minor_version;
} Cta_Debug_Version_T;

typedef struct
{
   uint8_t f_ctb_ttc_qualifier[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];
   uint8_t f_ctb_safety_dist_qualifier[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];
   uint8_t ctb_brake_qualification_counter[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];
   uint8_t ctb_brake_holding_counter[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];
   float32_T distance_to_driving_tube[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];
   float32_T event_time[CTA_NUM_MODES][FBK_NUMBER_OF_SIDES];
   float32_T min_safety_dist[CTA_NUM_MODES];
   float32_T max_safety_dist[CTA_NUM_MODES];
} Ctb_Debug_Internals_T;

typedef struct
{
   Cta_Debug_Internals_T cta_internals;
   Ctb_Debug_Internals_T ctb_internals;
} Cta_Debug_Output_T;

typedef struct
{
   Cta_Core_Input_T cta_core_input;
   Cta_Core_Output_T cta_core_output;
   Cta_Persistent_T cta_persistent;
   Cta_Core_Calibration_T cta_calibration;
   Cta_Debug_Version_T cta_version;
   Cta_Object_Attributes_T cta_object_attributes[PA_OBJ_NUMBER_OF_OBJECTS];
   Cta_Object_Persistent_T cta_object_persistent[PA_OBJ_NUMBER_OF_OBJECTS];
   Cta_Debug_Output_T cta_debug_output;
} Cta_Debug_Data_T;

/*===========================================================================*\
* Debug interface
\*===========================================================================*/

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

   Cta_Debug_Data_T *Cta_Get_Debug_Data(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

/*===========================================================================*\
* Helper functions to fill and reset debug data
\*===========================================================================*/

void Cta_Debug_Reset_Data(void);

void Cta_Debug_Pass_General_Data(Cta_Instance_T *p_cta_instance);

void Cta_Debug_Pass_Sw_Version(const uint16_t cta_sw_major_version, const uint16_t cta_sw_minor_version);

void Cta_Debug_Pass_Ctb_Condition_Infos(boolean_T f_safety_dist_qualifier,
                                        boolean_T f_time_qualifier,
                                        float32_T distance_to_driving_tube,
                                        float32_T event_time,
                                        float32_T min_safety_dist,
                                        float32_T max_safety_dist,
                                        uint8_t approach_side,
                                        Cta_Mode_T cta_mode);

void Cta_Debug_Pass_Ctb_Counters(uint8_t holding_ctr, uint8_t qualification_ctr, uint8_t approach_side, Cta_Mode_T cta_mode);

void Cta_Debug_Pass_Object_Validity_Flag(const Cta_Object_Data_T *p_object, const boolean_T f_obj_is_valid);

void Cta_Debug_Pass_Obj_Persistent_Data(const Cta_Object_Data_T *p_object);

void Cta_Debug_Pass_Internals(const Cta_Object_Data_T *p_object, const Cta_Crit_Level_Calibration_T *p_criticality_level_calibration);

void Cta_Debug_Pass_Initials_And_Paths(const Cta_Object_Data_T *p_object);

void Cta_Debug_Pass_Object_With_Highest_Criticality(const Cta_Comparison_Data_T *p_cta_comparison_data);

/*===========================================================================*\
* Macros to disable debug output in production code
\*===========================================================================*/

/* clang-format off */

#define Binary_Cta_Debug_Reset_Data()                                                                                                                                                         Cta_Debug_Reset_Data()
#define Binary_Cta_Debug_Pass_General_Data(p_cta_instance)                                                                                                                                    Cta_Debug_Pass_General_Data(p_cta_instance)
#define Binary_Cta_Debug_Pass_Sw_Version(cta_sw_major_version, cta_sw_minor_version)                                                                                                          Cta_Debug_Pass_Sw_Version(cta_sw_major_version, cta_sw_minor_version)
#define Binary_Cta_Debug_Pass_Ctb_Condition_Infos(f_safety_dist_qualifier, f_time_qualifier, distance_to_driving_tube, event_time, min_safety_dist, max_safety_dist, approach_side, cta_mode) Cta_Debug_Pass_Ctb_Condition_Infos(f_safety_dist_qualifier, f_time_qualifier, distance_to_driving_tube, event_time, min_safety_dist, max_safety_dist, approach_side, cta_mode)
#define Binary_Cta_Debug_Pass_Ctb_Counters(holding_ctr, qualification_ctr, approach_side, cta_mode)                                                                                           Cta_Debug_Pass_Ctb_Counters(holding_ctr, qualification_ctr, approach_side, cta_mode)
#define Binary_Cta_Debug_Pass_Object_Validity_Flag(p_object, f_obj_is_valid)                                                                                                                  Cta_Debug_Pass_Object_Validity_Flag(p_object, f_obj_is_valid)
#define Binary_Cta_Debug_Pass_Obj_Persistent_Data(p_object)                                                                                                                                   Cta_Debug_Pass_Obj_Persistent_Data(p_object)
#define Binary_Cta_Debug_Pass_Internals(p_object, p_criticality_level_calibration)                                                                                                            Cta_Debug_Pass_Internals(p_object, p_criticality_level_calibration)    
#define Binary_Cta_Debug_Pass_Initials_And_Paths(p_object)                                                                                                                                    Cta_Debug_Pass_Initials_And_Paths(p_object)
#define Binary_Cta_Debug_Pass_Object_With_Highest_Criticality(p_cta_comparison_data)                                                                                                          Cta_Debug_Pass_Object_With_Highest_Criticality(p_cta_comparison_data)
/* clang-format on */

#else

#define Binary_Cta_Debug_Reset_Data()
#define Binary_Cta_Debug_Pass_General_Data(p_cta_instance)
#define Binary_Cta_Debug_Pass_Sw_Version(cta_sw_major_version, cta_sw_minor_version)
#define Binary_Cta_Debug_Pass_Ctb_Condition_Infos(f_safety_dist_qualifier, f_time_qualifier, distance_to_driving_tube, \
                                                  event_time, min_safety_dist, max_safety_dist, approach_side, cta_mode)
#define Binary_Cta_Debug_Pass_Ctb_Counters(holding_ctr, qualification_ctr, approach_side, cta_mode)
#define Binary_Cta_Debug_Pass_Object_Validity_Flag(p_object, f_obj_is_valid)
#define Binary_Cta_Debug_Pass_Obj_Persistent_Data(p_object)
#define Binary_Cta_Debug_Pass_Internals(p_object, p_criticality_level_calibration)
#define Binary_Cta_Debug_Pass_Initials_And_Paths(p_object)
#define Binary_Cta_Debug_Pass_Object_With_Highest_Criticality(p_cta_comparison_data)

#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */

#endif /* CTA_DEBUG_INTERFACE_H */
