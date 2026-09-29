/**
 * @file recw_debug_interface.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the functions for copying internal data to the debug output interface.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#include "recw_debug_interface.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include <assert.h>
#include <string.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
 * File Scope variables
\*===========================================================================*/

static Recw_Debug_Data_T recw_debug_data;

/*===========================================================================*\
* Debug interface
\*===========================================================================*/

Recw_Debug_Data_T *Recw_Get_Debug_Data(void)
{
   return (&recw_debug_data);
}

/*===========================================================================*\
* Helper functions to fill and reset debug data
\*===========================================================================*/

void Recw_Debug_Reset_Data(void)
{
   uint8_t index;

   memset(&recw_debug_data, 0, sizeof(Recw_Debug_Data_T));

   /* Create default attributes to reset the object specific data. */
   for (index = FBK_ZERO_UINT; index < PA_OBJ_NUMBER_OF_OBJECTS; index++)
   {
      recw_debug_data.recw_object_attributes[index].ttc = RECW_MAX_TTC;
   }
}

void Recw_Debug_Pass_General_Data(const Recw_Core_Input_T *p_recw_core_input,
                                  const Recw_Core_Output_T *p_recw_core_output,
                                  const Recw_Persistent_T *p_recw_persistent,
                                  const Recw_Core_Calibration_T *p_recw_cal)
{
   /* Assert that all passed pointers are valid. */
   assert(NULL != p_recw_core_input);
   assert(NULL != p_recw_core_output);
   assert(NULL != p_recw_persistent);
   assert(NULL != p_recw_cal);

   /* Copy data from internal interfaces to debug output interface. */
   recw_debug_data.recw_core_input  = *p_recw_core_input;
   recw_debug_data.recw_core_output = *p_recw_core_output;
   recw_debug_data.recw_persistent  = *p_recw_persistent;
   memcpy(&recw_debug_data.recw_calibration, p_recw_cal, sizeof(Recw_Core_Calibration_T));
}

void Recw_Debug_Pass_Sw_Version(const uint16_t recw_sw_major_version, const uint16_t recw_sw_minor_version)
{
   /* Store version from FF iface. */
   recw_debug_data.recw_version.recw_sw_major_version = recw_sw_major_version;
   recw_debug_data.recw_version.recw_sw_minor_version = recw_sw_minor_version;
}

void Recw_Debug_Pass_Ego_Lane_Filter_Zones(const Fbk_Vehicle_Data_T *p_vehicle_data, const Recw_Core_Calibration_T *p_recw_cal)
{
   const float32_T recw_lane_filter_zone_length = 90.0f;

   float32_T zone_front          = -p_vehicle_data->host_length;
   float32_T zone_rear           = zone_front - recw_lane_filter_zone_length;
   float32_T zone_width_at_front = p_recw_cal->k_recw_lane_filter_width + Fbk_Abs_F(zone_front) * p_recw_cal->k_recw_lane_width_slope;
   float32_T zone_width_at_rear = p_recw_cal->k_recw_lane_filter_width + Fbk_Abs_F(zone_rear) * p_recw_cal->k_recw_lane_width_slope;
   float32_T zone_hys_width_at_front = zone_width_at_front + p_recw_cal->k_recw_lane_filter_width_hys;
   float32_T zone_hys_width_at_rear  = zone_width_at_rear + p_recw_cal->k_recw_lane_filter_width_hys;

   /* Store lane filter zone */
   recw_debug_data.recw_debug_output.lane_filter_zone.size        = RECW_DEBUG_NUMBER_OF_LANE_ZONE_POINTS;
   recw_debug_data.recw_debug_output.lane_filter_zone.points[0].x = zone_front;
   recw_debug_data.recw_debug_output.lane_filter_zone.points[1].x = zone_front;
   recw_debug_data.recw_debug_output.lane_filter_zone.points[2].x = zone_rear;
   recw_debug_data.recw_debug_output.lane_filter_zone.points[3].x = zone_rear;
   recw_debug_data.recw_debug_output.lane_filter_zone.points[0].y = 0.5f * zone_width_at_front;
   recw_debug_data.recw_debug_output.lane_filter_zone.points[1].y = -0.5f * zone_width_at_front;
   recw_debug_data.recw_debug_output.lane_filter_zone.points[2].y = -0.5f * zone_width_at_rear;
   recw_debug_data.recw_debug_output.lane_filter_zone.points[3].y = 0.5f * zone_width_at_rear;

   /* Store lane filter hysteresis zone */
   recw_debug_data.recw_debug_output.lane_filter_zone_hys.size        = RECW_DEBUG_NUMBER_OF_LANE_ZONE_POINTS;
   recw_debug_data.recw_debug_output.lane_filter_zone_hys.points[0].x = zone_front;
   recw_debug_data.recw_debug_output.lane_filter_zone_hys.points[1].x = zone_front;
   recw_debug_data.recw_debug_output.lane_filter_zone_hys.points[2].x = zone_rear;
   recw_debug_data.recw_debug_output.lane_filter_zone_hys.points[3].x = zone_rear;
   recw_debug_data.recw_debug_output.lane_filter_zone_hys.points[0].y = Fbk_Half(zone_hys_width_at_front);
   recw_debug_data.recw_debug_output.lane_filter_zone_hys.points[1].y = -Fbk_Half(zone_hys_width_at_front);
   recw_debug_data.recw_debug_output.lane_filter_zone_hys.points[2].y = -Fbk_Half(zone_hys_width_at_rear);
   recw_debug_data.recw_debug_output.lane_filter_zone_hys.points[3].y = Fbk_Half(zone_hys_width_at_rear);
}

void Recw_Debug_Pass_Object_Attributes(const Recw_Object_Attributes_T *p_recw_object_attributes, const uint8_t object_index)
{
   /* Asserts */
   assert(NULL != p_recw_object_attributes);
   assert(object_index < PA_OBJ_NUMBER_OF_OBJECTS);

   /* Copy data from internal structure to debug output interface. */
   recw_debug_data.recw_object_attributes[object_index] = *p_recw_object_attributes;
}

#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */
