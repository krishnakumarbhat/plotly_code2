/**
 * @file cta_debug_writer.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains function declarations for CTA bin writer functions.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "cta_debug_interface.h"
#include "cta_core_calibration_t.h"
#include "cta_debug_writer.h"
#include "cta_input_t.h"
#include "cta_output_t.h"
#include "cta_persistent_t.h"
#include "cta_types.h"
#include "fbk_macros.h"
#include "fbk_vehicle_data_t.h"
#include "pa_obj_in.h"
#include "pa_reuse.h"
#include "string.h"
#include <assert.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
 * File Scope variables
\*===========================================================================*/

static Cta_Debug_Data_T cta_debug_data;

/*===========================================================================*\
* Debug interface
\*===========================================================================*/

Cta_Debug_Data_T *Cta_Get_Debug_Data(void)
{
   return (&cta_debug_data);
}

/*===========================================================================*\
* Helper functions to fill and reset debug data
\*===========================================================================*/

void Cta_Debug_Reset_Data(void)
{
   memset(&cta_debug_data, 0, sizeof(Cta_Debug_Data_T));
}

void Cta_Debug_Pass_General_Data(Cta_Instance_T *p_cta_instance)
{
   /* Assert that all passed pointers are valid. */
   assert(NULL != p_cta_instance);

   /* Copy data from internal interfaces to debug output interface. */
   cta_debug_data.cta_core_input  = p_cta_instance->core_input;
   cta_debug_data.cta_core_output = p_cta_instance->core_output;
   cta_debug_data.cta_persistent  = p_cta_instance->persistent;
   memcpy(&cta_debug_data.cta_calibration, &p_cta_instance->calibration, sizeof(p_cta_instance->calibration));
}

void Cta_Debug_Pass_Sw_Version(const uint16_t cta_sw_major_version, const uint16_t cta_sw_minor_version)
{
   /* Store version from FF iface. */
   cta_debug_data.cta_version.cta_sw_major_version = cta_sw_major_version;
   cta_debug_data.cta_version.cta_sw_minor_version = cta_sw_minor_version;
}

void Cta_Debug_Pass_Ctb_Condition_Infos(boolean_T f_safety_dist_qualifier,
                                        boolean_T f_time_qualifier,
                                        float32_T distance_to_driving_tube,
                                        float32_T event_time,
                                        float32_T min_safety_dist,
                                        float32_T max_safety_dist,
                                        uint8_t approach_side,
                                        Cta_Mode_T cta_mode)
{
   cta_debug_data.cta_debug_output.ctb_internals.f_ctb_safety_dist_qualifier[cta_mode][approach_side] =
      (uint8_t) f_safety_dist_qualifier;
   cta_debug_data.cta_debug_output.ctb_internals.f_ctb_ttc_qualifier[cta_mode][approach_side]      = (uint8_t) f_time_qualifier;
   cta_debug_data.cta_debug_output.ctb_internals.distance_to_driving_tube[cta_mode][approach_side] = distance_to_driving_tube;
   cta_debug_data.cta_debug_output.ctb_internals.event_time[cta_mode][approach_side]               = event_time;
   cta_debug_data.cta_debug_output.ctb_internals.min_safety_dist[cta_mode]                         = min_safety_dist;
   cta_debug_data.cta_debug_output.ctb_internals.max_safety_dist[cta_mode]                         = max_safety_dist;
}

void Cta_Debug_Pass_Ctb_Counters(uint8_t holding_ctr, uint8_t qualification_ctr, uint8_t approach_side, Cta_Mode_T cta_mode)
{
   cta_debug_data.cta_debug_output.ctb_internals.ctb_brake_qualification_counter[cta_mode][approach_side] = qualification_ctr;
   cta_debug_data.cta_debug_output.ctb_internals.ctb_brake_holding_counter[cta_mode][approach_side]       = holding_ctr;
}

void Cta_Debug_Pass_Object_Validity_Flag(const Cta_Object_Data_T *p_object, const boolean_T f_obj_is_valid)
{
   uint8_t obj_index                                                          = p_object->tracker_data.index;
   cta_debug_data.cta_debug_output.cta_internals.f_object_is_valid[obj_index] = f_obj_is_valid;
}

void Cta_Debug_Pass_Obj_Persistent_Data(const Cta_Object_Data_T *p_object)
{
   uint8_t obj_index = p_object->tracker_data.index;

   /* Copy data from internal structure to debug output interface. */
   cta_debug_data.cta_object_persistent[obj_index] = *(p_object->persistent);
}

void Cta_Debug_Pass_Internals(const Cta_Object_Data_T *p_object, const Cta_Crit_Level_Calibration_T *p_criticality_level_calibration)
{
   uint8_t obj_index = p_object->tracker_data.index;

   /* Assert */
   assert(NULL != p_object);
   assert(NULL != p_criticality_level_calibration);
   assert(obj_index < PA_OBJ_NUMBER_OF_OBJECTS);

   /* Copy data from internal structure to debug output interface. */
   cta_debug_data.cta_object_attributes[obj_index] = *(p_object->attributes);

   /* Since this method here is called after prev_cycle_crit_level is set,
   it depicts the crit level of the current cycle */
   cta_debug_data.cta_debug_output.cta_internals.crit_level_rcta[obj_index] =
      p_object->persistent->prev_cycle_crit_level[CTA_MODE_REAR];
   cta_debug_data.cta_debug_output.cta_internals.crit_level_fcta[obj_index] =
      p_object->persistent->prev_cycle_crit_level[CTA_MODE_FRONT];

   cta_debug_data.cta_debug_output.cta_internals.cta_isect_rcta_level_one[obj_index].min =
      p_criticality_level_calibration->min_long_point_criticality_level[CTA_MODE_REAR][0];
   cta_debug_data.cta_debug_output.cta_internals.cta_isect_rcta_level_two[obj_index].min =
      p_criticality_level_calibration->min_long_point_criticality_level[CTA_MODE_REAR][1];
   cta_debug_data.cta_debug_output.cta_internals.cta_isect_rcta_level_one[obj_index].max =
      p_criticality_level_calibration->max_long_point_criticality_level[CTA_MODE_REAR][0];
   cta_debug_data.cta_debug_output.cta_internals.cta_isect_rcta_level_two[obj_index].max =
      p_criticality_level_calibration->max_long_point_criticality_level[CTA_MODE_REAR][1];

   cta_debug_data.cta_debug_output.cta_internals.cta_isect_fcta_level_one[obj_index].min =
      p_criticality_level_calibration->min_long_point_criticality_level[CTA_MODE_FRONT][0];
   cta_debug_data.cta_debug_output.cta_internals.cta_isect_fcta_level_two[obj_index].min =
      p_criticality_level_calibration->min_long_point_criticality_level[CTA_MODE_FRONT][1];
   cta_debug_data.cta_debug_output.cta_internals.cta_isect_fcta_level_one[obj_index].max =
      p_criticality_level_calibration->max_long_point_criticality_level[CTA_MODE_FRONT][0];
   cta_debug_data.cta_debug_output.cta_internals.cta_isect_fcta_level_two[obj_index].max =
      p_criticality_level_calibration->max_long_point_criticality_level[CTA_MODE_FRONT][1];

   cta_debug_data.cta_debug_output.cta_internals.cta_ttc_rcta_level_one[obj_index] =
      p_criticality_level_calibration->ttc_criticality_level[CTA_MODE_REAR][0];
   cta_debug_data.cta_debug_output.cta_internals.cta_ttc_rcta_level_two[obj_index] =
      p_criticality_level_calibration->ttc_criticality_level[CTA_MODE_REAR][1];
   cta_debug_data.cta_debug_output.cta_internals.cta_ttc_fcta_level_one[obj_index] =
      p_criticality_level_calibration->ttc_criticality_level[CTA_MODE_FRONT][0];
   cta_debug_data.cta_debug_output.cta_internals.cta_ttc_fcta_level_two[obj_index] =
      p_criticality_level_calibration->ttc_criticality_level[CTA_MODE_FRONT][1];
}

void Cta_Debug_Pass_Initials_And_Paths(const Cta_Object_Data_T *p_object)
{
   uint8_t obj_index = p_object->tracker_data.index;

   /* Assert */
   assert(NULL != p_object);
   assert(obj_index < PA_OBJ_NUMBER_OF_OBJECTS);

   /* Debug whether path is attached to obj */
   if (NULL != p_object->attributes->p_pt_match_info)
   {
      cta_debug_data.cta_debug_output.cta_internals.cta_index_of_path_match_to_obj[obj_index] =
         (int8_t) p_object->attributes->p_pt_match_info->track_match;
   }
   else
   {
      cta_debug_data.cta_debug_output.cta_internals.cta_index_of_path_match_to_obj[obj_index] = -1;
   }

   /* Copy data from internal structure to debug output interface. */
   cta_debug_data.cta_object_attributes[obj_index] = *(p_object->attributes);
}

void Cta_Debug_Pass_Object_With_Highest_Criticality(const Cta_Comparison_Data_T *p_cta_comparison_data)
{
   /*Log most critical objects for RCTA*/
   if (PA_INVALID_OBJ_ID != p_cta_comparison_data->object_with_highest_crit[CTA_MODE_REAR][FBK_SIDE_LEFT].tracker_data.id)
   {
      cta_debug_data.cta_debug_output.cta_internals.cta_most_critical_rcta_obj_index_left =
         p_cta_comparison_data->object_with_highest_crit[CTA_MODE_REAR][FBK_SIDE_LEFT].tracker_data.index;
   }
   else
   {
      cta_debug_data.cta_debug_output.cta_internals.cta_most_critical_rcta_obj_index_left = -1;
   }

   if (PA_INVALID_OBJ_ID != p_cta_comparison_data->object_with_highest_crit[CTA_MODE_REAR][FBK_SIDE_RIGHT].tracker_data.id)
   {
      cta_debug_data.cta_debug_output.cta_internals.cta_most_critical_rcta_obj_index_right =
         p_cta_comparison_data->object_with_highest_crit[CTA_MODE_REAR][FBK_SIDE_RIGHT].tracker_data.index;
   }
   else
   {
      cta_debug_data.cta_debug_output.cta_internals.cta_most_critical_rcta_obj_index_right = -1;
   }

   /*Log most critical objects for FCTA*/
   if (PA_INVALID_OBJ_ID != p_cta_comparison_data->object_with_highest_crit[CTA_MODE_FRONT][FBK_SIDE_LEFT].tracker_data.id)
   {
      cta_debug_data.cta_debug_output.cta_internals.cta_most_critical_fcta_obj_index_left =
         p_cta_comparison_data->object_with_highest_crit[CTA_MODE_FRONT][FBK_SIDE_LEFT].tracker_data.index;
   }
   else
   {
      cta_debug_data.cta_debug_output.cta_internals.cta_most_critical_fcta_obj_index_left = -1;
   }

   if (PA_INVALID_OBJ_ID != p_cta_comparison_data->object_with_highest_crit[CTA_MODE_FRONT][FBK_SIDE_RIGHT].tracker_data.id)
   {
      cta_debug_data.cta_debug_output.cta_internals.cta_most_critical_fcta_obj_index_right =
         p_cta_comparison_data->object_with_highest_crit[CTA_MODE_FRONT][FBK_SIDE_RIGHT].tracker_data.index;
   }
   else
   {
      cta_debug_data.cta_debug_output.cta_internals.cta_most_critical_fcta_obj_index_right = -1;
   }
}


#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */
