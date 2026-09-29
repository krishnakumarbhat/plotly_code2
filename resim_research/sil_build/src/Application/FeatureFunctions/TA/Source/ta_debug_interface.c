/**
 * @file ta_debug_interface.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the functions for copying internal data to the debug output interface.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 */

#include "ta_debug_interface.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "pa_obj_in.h"
#include "pa_vehicle_in.h"
#include <assert.h>
#include <string.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
 * File Scope variables
\*===========================================================================*/

static Ta_Debug_Data_T ta_debug_data;

/*===========================================================================*\
* Debug interface
\*===========================================================================*/

Ta_Debug_Data_T *Ta_Get_Debug_Data(void)
{
   return (&ta_debug_data);
}

/*===========================================================================*\
* Helper functions to fill and reset debug data
\*===========================================================================*/

void Ta_Debug_Reset_Data(void)
{
   memset(&ta_debug_data, 0, sizeof(Ta_Debug_Data_T));
}

void Ta_Debug_Pass_General_Data(const Ta_Core_Input_T *p_ta_core_input,
                                const Ta_Core_Output_T *p_ta_core_output,
                                const Ta_Persistent_T *p_ta_persistent,
                                const Ta_Core_Calibration_T *p_ta_cal)
{
   /* Assert that all passed pointers are valid. */
   assert(NULL != p_ta_core_input);
   assert(NULL != p_ta_core_output);
   assert(NULL != p_ta_persistent);
   assert(NULL != p_ta_cal);

   /* Copy data from internal interfaces to debug output interface. */
   ta_debug_data.ta_core_input  = *p_ta_core_input;
   ta_debug_data.ta_core_output = *p_ta_core_output;
   ta_debug_data.ta_persistent  = *p_ta_persistent;
   memcpy(&ta_debug_data.ta_calibration, p_ta_cal, sizeof(Ta_Core_Calibration_T));
}

void Ta_Debug_Pass_Sw_Version(const uint16_t ta_sw_major_version, const uint16_t ta_sw_minor_version)
{
   /* Store version from FF iface. */
   ta_debug_data.ta_version.ta_sw_major_version = ta_sw_major_version;
   ta_debug_data.ta_version.ta_sw_minor_version = ta_sw_minor_version;
}

void Ta_Debug_Pass_Object_Attributes(const Ta_Object_Attributes_T *p_ta_object_attributes, const uint8_t object_index)
{
   /* Asserts */
   assert(NULL != p_ta_object_attributes);
   assert(object_index < PA_OBJ_NUMBER_OF_OBJECTS);

   /* Copy data from internal structure to debug output interface. */
   ta_debug_data.ta_object_attributes[object_index] = *p_ta_object_attributes;
}

void Ta_Debug_Pass_Ego_Data(const Fbk_Trajectory_T *p_ego_trajectory)
{
   /* Asserts */
   assert(NULL != p_ego_trajectory);

   /* Fill TA object debug structure */
   ta_debug_data.ta_debug_output.ego_trajectory = *p_ego_trajectory;
}

void Ta_Debug_Set_Side_Specific_Object_Trajectories(const Pa_Data_T *p_pa_data, const Ta_Core_Output_T *p_ta_core_output)
{
   uint8_t object_index;
   uint8_t ta_potential_obj_index_l = PA_INVALID_OBJ_INDEX;
   uint8_t ta_potential_obj_index_r = PA_INVALID_OBJ_INDEX;

   for (object_index = FBK_ZERO_UINT; object_index < PA_OBJ_NUMBER_OF_OBJECTS; object_index++)
   {
      const Ta_Object_Attributes_T *ta_obj_attributes = &(ta_debug_data.ta_object_attributes[object_index]);

      /* Look for potentially critical object for visualization */
      if (Fbk_Is_True(ta_obj_attributes->f_obj_in_danger_zone) || Fbk_Is_True(ta_obj_attributes->f_obj_in_wing_zone))
      {
         if (p_pa_data->object_data[object_index].vcs_pos.y < FBK_ZERO_F)
         {
            /* Potential obj for left side */
            ta_potential_obj_index_l = object_index;
         }
         else
         {
            /* Potential obj for right side */
            ta_potential_obj_index_r = object_index;
         }
      }
   }

   /* Find most critical or potentially critical obj for left side */
   if (p_ta_core_output->ta_alert_level[FBK_SIDE_LEFT] >= TA_ALERT_STATE_LEVEL_2)
   {
      ta_debug_data.ta_debug_output.most_crit_object_trajectory_left =
         ta_debug_data.ta_object_attributes[p_ta_core_output->ta_index[FBK_SIDE_LEFT]].trajectory;
   }
   else if (PA_INVALID_OBJ_INDEX != ta_potential_obj_index_l)
   {
      ta_debug_data.ta_debug_output.most_crit_object_trajectory_left =
         ta_debug_data.ta_object_attributes[ta_potential_obj_index_l].trajectory;
   }
   else
   {
      /* No interesting object for left side found */
      memset(&ta_debug_data.ta_debug_output.most_crit_object_trajectory_left, 0, sizeof(Fbk_Trajectory_T));
   }

   /* Find most critical or potentially critical obj for right side */
   if (p_ta_core_output->ta_alert_level[FBK_SIDE_RIGHT] >= TA_ALERT_STATE_LEVEL_2)
   {
      ta_debug_data.ta_debug_output.most_crit_object_trajectory_right =
         ta_debug_data.ta_object_attributes[p_ta_core_output->ta_index[FBK_SIDE_RIGHT]].trajectory;
   }
   else if (PA_INVALID_OBJ_INDEX != ta_potential_obj_index_r)
   {
      ta_debug_data.ta_debug_output.most_crit_object_trajectory_right =
         ta_debug_data.ta_object_attributes[ta_potential_obj_index_r].trajectory;
   }
   else
   {
      /* No interesting object for right side found */
      memset(&ta_debug_data.ta_debug_output.most_crit_object_trajectory_right, 0, sizeof(Fbk_Trajectory_T));
   }
}

#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */
