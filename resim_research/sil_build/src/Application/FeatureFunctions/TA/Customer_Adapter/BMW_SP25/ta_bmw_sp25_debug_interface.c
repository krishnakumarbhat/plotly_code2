/**
 * @file ta_bmw_sp25_debug_interface.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains some debug logic required for debugging
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 *
 */

#include "ta_bmw_sp25_debug_interface.h"
#include "fbk_macros.h"
#include "fbk_vehicle_data_t.h"
#include "ml_lookup_table_2d.h"
#include "pa_vehicle_in.h"
#include "ta_input_t.h"
#include "ta_output_t.h"
#include "ta_types.h"
#include <assert.h>

/* Includes are located outside of BINARY_DEBUG block to ensure ISO C compliance (empty translation units are forbidden)  */
#if defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER)

/*===========================================================================*\
* Static variable definitions
\*===========================================================================*/

static Ta_Bmw_Sp25_Debug_Data_T Ta_Bmw_Sp25_Debug_Data;

/*===========================================================================*\
* Defines
\*===========================================================================*/

#define TA_CORE_MANEUVER_NONE (0u)
#define TA_CORE_MANEUVER_STRAIGHT (1u)
#define TA_CORE_MANEUVER_TURNING (2u)

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

Ta_Bmw_Sp25_Debug_Data_T *Ta_Get_Bmw_Sp25_Debug_Data(void)
{
   return &Ta_Bmw_Sp25_Debug_Data;
}

void Ta_Bmw_Sp25_Fill_Debug_Data(const Ta_Input_T *p_ta_input, const Ta_Output_T *p_ta_output)
{
   /* Assert that all passed pointers are valid. */
   assert(NULL != p_ta_input);
   assert(NULL != p_ta_output);

   /* Copy data from internal interfaces to debug output interface. */
   Ta_Bmw_Sp25_Debug_Data.ta_input  = *p_ta_input;
   Ta_Bmw_Sp25_Debug_Data.ta_output = *p_ta_output;
}


void Pass_Ta_Debug_Bmw_Sp25_Various(const Ta_Input_T *p_ta_input,
                                    const Fbk_Vehicle_Data_T *p_vehicle_data,
                                    const Ta_Core_Calibration_T *p_ta_cal,
                                    const float32_T ta_host_speed_at_brake_start,
                                    const float32_T ta_host_speed_reduction_requested,
                                    const float32_T ta_host_speed_reduction_achieved,
                                    const uint8_t pfgs_qualification_counter_min,
                                    const uint8_t pfgs_qualification_counter)
{
   Ta_Bmw_Sp25_Debug_Data.ta_debug_output.ta_host_speed_at_brake_start      = ta_host_speed_at_brake_start;
   Ta_Bmw_Sp25_Debug_Data.ta_debug_output.ta_host_speed_reduction_requested = ta_host_speed_reduction_requested;
   Ta_Bmw_Sp25_Debug_Data.ta_debug_output.ta_host_speed_reduction_achieved  = ta_host_speed_reduction_achieved;
   Ta_Bmw_Sp25_Debug_Data.ta_debug_output.pfgs_qualification_counter_min    = pfgs_qualification_counter_min;
   Ta_Bmw_Sp25_Debug_Data.ta_debug_output.pfgs_qualification_counter        = pfgs_qualification_counter;


   /* Set f_pfgs_relevant_host_speed */
   if ((p_vehicle_data->host_speed >= p_ta_cal->k_pfgs_ego_speed[TA_MIN])
       && (p_vehicle_data->host_speed <= p_ta_cal->k_pfgs_ego_speed[TA_MAX]))
   {
      /* Host speed is in PFGS relevant range */
      Ta_Bmw_Sp25_Debug_Data.ta_debug_output.f_pfgs_relevant_host_speed = FBK_TRUE;
   }
   else
   {
      Ta_Bmw_Sp25_Debug_Data.ta_debug_output.f_pfgs_relevant_host_speed = FBK_FALSE;
   }

   /* Set ta_core_maneuver */
   if (Fbk_Abs_F(p_vehicle_data->curvature) <= p_ta_cal->k_ta_straight_host_curvature_max)
   {
      Ta_Bmw_Sp25_Debug_Data.ta_debug_output.ta_core_maneuver = TA_CORE_MANEUVER_STRAIGHT;
   }
   else if (Fbk_Abs_F(p_vehicle_data->curvature) >= Get_Value_From_2d_Lookup_Table(
               p_ta_cal->k_ta_lookup_turning_host_speed, p_ta_cal->k_ta_lookup_turning_host_curvature_min,
               TA_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN_ARRAY_SIZE_DIM0, p_vehicle_data->host_speed))
   {
      Ta_Bmw_Sp25_Debug_Data.ta_debug_output.ta_core_maneuver = TA_CORE_MANEUVER_TURNING;
   }
   else
   {
      Ta_Bmw_Sp25_Debug_Data.ta_debug_output.ta_core_maneuver = TA_CORE_MANEUVER_NONE;
   }

   Ta_Bmw_Sp25_Debug_Data.ta_debug_output.ta_lookup_turning_host_curvature_threshold =
      Fbk_Sign(p_vehicle_data->curvature)
      * Get_Value_From_2d_Lookup_Table(p_ta_cal->k_ta_lookup_turning_host_speed, p_ta_cal->k_ta_lookup_turning_host_curvature_min,
                                       TA_K_TA_LOOKUP_TURNING_HOST_CURVATURE_MIN_ARRAY_SIZE_DIM0, p_vehicle_data->host_speed);
}


#endif /* defined(BINARY_DEBUG) || defined(COMPONENT_RESIM_FF_KEG_LOGGER) */
