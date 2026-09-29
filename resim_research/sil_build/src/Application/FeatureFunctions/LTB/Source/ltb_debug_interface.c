/**
 * @file ltb_debug_interface.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the functions for copying internal data to the debug output interface.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */

#include "ltb_debug_interface.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "pa_vehicle_in.h"
#include <assert.h>
#include <string.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
 * File Scope variables
\*===========================================================================*/

static Ltb_Debug_Data_T ltb_debug_data;

/*===========================================================================*\
* Debug interface
\*===========================================================================*/

Ltb_Debug_Data_T *Ltb_Get_Debug_Data(void)
{
   return (&ltb_debug_data);
}

/*===========================================================================*\
* Helper functions to fill and reset debug data
\*===========================================================================*/

void Ltb_Debug_Reset_Data(void)
{

   memset(&ltb_debug_data, 0, sizeof(Ltb_Debug_Data_T));
}

void Ltb_Debug_Pass_General_Data(const Ltb_Core_Input_T *p_ltb_core_input,
                                 const Ltb_Core_Output_T *p_ltb_core_output,
                                 const Ltb_Persistent_T *p_ltb_persistent,
                                 const Ltb_Core_Calibration_T *p_ltb_cal)
{
   /* Assert that all passed pointers are valid. */
   assert(NULL != p_ltb_core_input);
   assert(NULL != p_ltb_core_output);
   assert(NULL != p_ltb_persistent);
   assert(NULL != p_ltb_cal);

   /* Copy data from internal interfaces to debug output interface. */
   ltb_debug_data.ltb_core_input  = *p_ltb_core_input;
   ltb_debug_data.ltb_core_output = *p_ltb_core_output;
   ltb_debug_data.ltb_persistent  = *p_ltb_persistent;
   memcpy(&ltb_debug_data.ltb_calibration, p_ltb_cal, sizeof(Ltb_Core_Calibration_T));
}

void Ltb_Debug_Pass_Sw_Version(const uint16_t ltb_sw_major_version, const uint16_t ltb_sw_minor_version)
{
   /* Store version from FF iface. */
   ltb_debug_data.ltb_version.ltb_sw_major_version = ltb_sw_major_version;
   ltb_debug_data.ltb_version.ltb_sw_minor_version = ltb_sw_minor_version;
}

void Ltb_Debug_Pass_Object_Attributes(const Ltb_Object_Attributes_T *p_ltb_object_attributes, const uint8_t object_index)
{
   /* Assert */
   assert(NULL != p_ltb_object_attributes);
   assert(object_index < PA_OBJ_NUMBER_OF_OBJECTS);

   /* Copy data from internal structure to debug output interface. */
   ltb_debug_data.ltb_object_attributes[object_index] = *p_ltb_object_attributes;
}

void Ltb_Debug_Pass_Ego_Data(const Fbk_Trajectory_T *p_ego_trajectory)
{
   /* Asserts */
   assert(NULL != p_ego_trajectory);

   /* Fill LTB object debug structure */
   ltb_debug_data.ltb_debug_output.ego_trajectory = *p_ego_trajectory;
}

void Ltb_Debug_Set_Side_Specific_Object_Trajectories(const Pa_Data_T *p_pa_data, const Ltb_Core_Output_T *p_ltb_core_output)
{
   uint8_t object_index;
   uint8_t ltb_potential_obj_index_l = PA_INVALID_OBJ_INDEX;
   uint8_t ltb_potential_obj_index_r = PA_INVALID_OBJ_INDEX;

   for (object_index = FBK_ZERO_UINT; object_index < PA_OBJ_NUMBER_OF_OBJECTS; object_index++)
   {
      const Ltb_Object_Attributes_T *ltb_obj_attributes = &(ltb_debug_data.ltb_object_attributes[object_index]);

      /* Look for potentially critical object for visualization */
      if (Fbk_Is_True(ltb_obj_attributes->f_obj_in_zone))
      {
         if (p_pa_data->object_data[object_index].vcs_pos.y < FBK_ZERO_F)
         {
            /* Potential obj for left side */
            ltb_potential_obj_index_l = object_index;
         }
         else
         {
            /* Potential obj for right side */
            ltb_potential_obj_index_r = object_index;
         }
      }
   }

   /* Find most critical or potentially critical obj for left side */
   if (p_ltb_core_output->ltb_alert_level[FBK_SIDE_LEFT] >= ALERT_ACTIVE_LEVEL_1)
   {
      ltb_debug_data.ltb_debug_output.most_crit_object_trajectory_left =
         ltb_debug_data.ltb_object_attributes[p_ltb_core_output->ltb_index[FBK_SIDE_LEFT]].trajectory;
   }
   else if (PA_INVALID_OBJ_INDEX != ltb_potential_obj_index_l)
   {
      ltb_debug_data.ltb_debug_output.most_crit_object_trajectory_left =
         ltb_debug_data.ltb_object_attributes[ltb_potential_obj_index_l].trajectory;
   }
   else
   {
      /* No interesting object for left side found */
      memset(&ltb_debug_data.ltb_debug_output.most_crit_object_trajectory_left, 0, sizeof(Fbk_Trajectory_T));
   }

   /* Find most critical or potentially critical obj for right side */
   if (p_ltb_core_output->ltb_alert_level[FBK_SIDE_RIGHT] >= ALERT_ACTIVE_LEVEL_1)
   {
      ltb_debug_data.ltb_debug_output.most_crit_object_trajectory_right =
         ltb_debug_data.ltb_object_attributes[p_ltb_core_output->ltb_index[FBK_SIDE_RIGHT]].trajectory;
   }
   else if (PA_INVALID_OBJ_INDEX != ltb_potential_obj_index_r)
   {
      ltb_debug_data.ltb_debug_output.most_crit_object_trajectory_right =
         ltb_debug_data.ltb_object_attributes[ltb_potential_obj_index_r].trajectory;
   }
   else
   {
      /* No interesting object for right side found */
      memset(&ltb_debug_data.ltb_debug_output.most_crit_object_trajectory_right, 0, sizeof(Fbk_Trajectory_T));
   }
}

#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */
