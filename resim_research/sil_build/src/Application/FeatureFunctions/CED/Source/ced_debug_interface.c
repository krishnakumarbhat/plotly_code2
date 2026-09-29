/**
 * @file ced_debug_interface.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the functions for copying internal data to the debug output interface.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#include "ced_debug_interface.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "pa_vehicle_in.h"
#include "pt_output_t.h"
#include <assert.h>
#include <string.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
 * File Scope variables
\*===========================================================================*/

static Ced_Debug_Data_T ced_debug_data;

/*===========================================================================*\
* Debug interface
\*===========================================================================*/

Ced_Debug_Data_T *Ced_Get_Debug_Data(void)
{
   return (&ced_debug_data);
}

/*===========================================================================*\
* Helper functions to fill and reset debug data
\*===========================================================================*/

void Ced_Debug_Reset_Data(void)
{
   uint8_t object_index;

   memset(&ced_debug_data, 0, sizeof(Ced_Debug_Data_T));

   for (object_index = FBK_ZERO_UINT; object_index < PA_OBJ_NUMBER_OF_OBJECTS; object_index++)
   {
      ced_debug_data.ced_object_attributes[object_index].closest_lat_dist_predicted = CED_INVALID_DISTANCE;
      ced_debug_data.ced_object_attributes[object_index].direction                  = FBK_SIDE_UNDEFINED;
      ced_debug_data.ced_object_attributes[object_index].time_to_crash_line         = CED_INVALID_TIME;
      ced_debug_data.ced_object_attributes[object_index].time_to_pass_crash_line    = CED_INVALID_TIME;
      ced_debug_data.ced_object_attributes[object_index].distance_to_crash_line     = CED_INVALID_DISTANCE;
      ced_debug_data.ced_debug_output.ced_path_match_index[object_index]            = PT_DEFAULT_MATCH_INDEX;
   }
}

void Ced_Debug_Pass_General_Data(const Ced_Core_Input_T *p_ced_core_input,
                                 const Ced_Core_Output_T *p_ced_core_output,
                                 const Ced_Persistent_T *p_ced_persistent,
                                 const Ced_Core_Calibration_T *p_ced_cal)
{
   /* Assert that all passed pointers are valid. */
   assert(NULL != p_ced_core_input);
   assert(NULL != p_ced_core_output);
   assert(NULL != p_ced_persistent);
   assert(NULL != p_ced_cal);

   /* Copy data from internal interfaces to debug output interface. */
   ced_debug_data.ced_core_input  = *p_ced_core_input;
   ced_debug_data.ced_core_output = *p_ced_core_output;
   ced_debug_data.ced_persistent  = *p_ced_persistent;
   memcpy(&ced_debug_data.ced_calibration, p_ced_cal, sizeof(Ced_Core_Calibration_T));

   /* Determine max alert level */
   if (Fbk_Is_True(p_ced_cal->k_ced_f_third_warning_level_enable))
   {
      ced_debug_data.ced_debug_output.ced_max_alertlevel = CED_ALERT_ACTIVE_LEVEL_3;
   }
   else if (Fbk_Is_True(p_ced_cal->k_ced_f_second_warning_level_enable))
   {
      ced_debug_data.ced_debug_output.ced_max_alertlevel = CED_ALERT_ACTIVE_LEVEL_2;
   }
   else
   {
      ced_debug_data.ced_debug_output.ced_max_alertlevel = CED_ALERT_ACTIVE_LEVEL_1;
   }
}

void Ced_Debug_Pass_Sw_Version(const uint16_t ced_sw_major_version, const uint16_t ced_sw_minor_version)
{
   /* Store version from FF iface. */
   ced_debug_data.ced_version.ced_sw_major_version = ced_sw_major_version;
   ced_debug_data.ced_version.ced_sw_minor_version = ced_sw_minor_version;
}

void Ced_Debug_Pass_Zones(const Fbk_Field_Of_Interest_T *p_funnel_zone,
                          const Fbk_Field_Of_Interest_T *p_collision_zone,
                          const Fbk_Vehicle_Data_T *p_vehicle_data,
                          const Ced_Core_Calibration_T *p_ced_cal)
{
   uint8_t i;

   /* Asserts */
   assert(NULL != p_funnel_zone);
   assert(NULL != p_collision_zone);
   assert(NULL != p_vehicle_data);
   assert(NULL != p_ced_cal);

   /* Calculate crash line positions (VCS)*/
   ced_debug_data.ced_debug_output.ced_crash_line_front =
      -p_vehicle_data->host_length * p_ced_cal->k_ced_crash_line_host_length_percentage[FBK_SIDE_FRONT];
   ced_debug_data.ced_debug_output.ced_crash_line_rear =
      -p_vehicle_data->host_length * p_ced_cal->k_ced_crash_line_host_length_percentage[FBK_SIDE_REAR];

   /* Pass the zone data. */
   for (i = FBK_ZERO_UINT; i < CED_NUMBER_OF_ZONE_POINTS; i++)
   {
      ced_debug_data.ced_debug_output.ced_funnel_zone_x[i] = p_funnel_zone->points[i].x;
      ced_debug_data.ced_debug_output.ced_funnel_zone_y[i] = p_funnel_zone->points[i].y;
   }

   for (i = FBK_ZERO_UINT; i < CED_NUMBER_OF_ZONE_POINTS; i++)
   {
      ced_debug_data.ced_debug_output.ced_collision_zone_x[i] = p_collision_zone->points[i].x;
      ced_debug_data.ced_debug_output.ced_collision_zone_y[i] = p_collision_zone->points[i].y;
   }
}

void Ced_Debug_Pass_Object_Attributes(const Ced_Object_Attributes_T *p_ced_object_attributes, const uint8_t object_index)
{
   /* Assert */
   assert(NULL != p_ced_object_attributes);
   assert(object_index < PA_OBJ_NUMBER_OF_OBJECTS);

   /* Copy data from internal structure to debug output interface. */
   ced_debug_data.ced_object_attributes[object_index] = *p_ced_object_attributes;
}

void Ced_Debug_Pass_Object_To_Path_Match_Index(const Ced_Object_T *p_ced_object, const uint8_t object_index)
{
   /* Assert */
   assert(NULL != p_ced_object);
   assert(object_index < PA_OBJ_NUMBER_OF_OBJECTS);

   /* Debug whether path is attached to object */
   if (NULL != p_ced_object->attributes.p_pt_match_info)
   {
      ced_debug_data.ced_debug_output.ced_path_match_index[object_index] =
         (uint8_t) p_ced_object->attributes.p_pt_match_info->track_match;
   }
   else
   {
      ced_debug_data.ced_debug_output.ced_path_match_index[object_index] = PT_DEFAULT_MATCH_INDEX;
   }
}

void Ced_Debug_Pass_Side_Data(const Ced_Object_T *p_ced_object, uint8_t side_index)
{
   /* Assert */
   assert(NULL != p_ced_object);

   ced_debug_data.ced_debug_output.ced_side_object_id_internal[side_index] = p_ced_object->tracker_data.id;
   ced_debug_data.ced_debug_output.ced_side_alert_internal[side_index]     = p_ced_object->attributes.alert_level;
}

void Ced_Debug_Increase_Object_Importance_Counter(const uint8_t object_index)
{
   ced_debug_data.ced_debug_output.ced_object_importance_counter[object_index]++;
}

void Ced_Debug_Pass_Object_Alert_Suppression_Reason(const Ced_Alert_Suppression_T ced_alert_suppression_reason,
                                                    const uint8_t object_index)
{
   /* Assert */
   assert(object_index < PA_OBJ_NUMBER_OF_OBJECTS);
   ced_debug_data.ced_debug_output.ced_object_alert_suppression_reason[object_index] = ced_alert_suppression_reason;
}

#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */
