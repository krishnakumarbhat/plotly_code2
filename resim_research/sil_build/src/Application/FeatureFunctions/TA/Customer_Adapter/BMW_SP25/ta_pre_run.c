/**
 * @file ta_pre_run.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW SRR5 pre run logic for TA.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

#include "ta_pre_run.h"
#include "fbk_macros.h"
#include "fbk_vehicle_data_t.h"
#include "pa_data.h"
#include "pa_reuse.h"
#include "ta_bmw_boardnet_t.h"
#include "ta_bmw_diagnostic.h"
#include "ta_bmw_enums.h"
#include "ta_bmw_sp25_types.h"
#include "ta_core_calibration_t.h"
#include "ta_core_input_t.h"
#include "ta_instance_t.h"
#include "ta_state_machine.h"
#include <assert.h>

#define TA_BMW_DEFAULT_MIN_VEL_LOWER_LIMIT (0.0f)
#define TA_BMW_DEFAULT_MIN_VEL_UPPER_LIMIT (0.0f)
#define TA_BMW_DEFAULT_MAX_VEL_LOWER_LIMIT (45.0f)
#define TA_BMW_DEFAULT_MAX_VEL_UPPER_LIMIT (50.0f)

/* coverity[misra_c_2012_rule_8_9_violation][Defined static variable in dedicated section for overview of memory consumption] */
static Ta_BMW_Boardnet_T Boardnet_Signals;
static TA_FF_State_T Ta_current_state = TA_STATE_NOT_AVAILABLE;

/*============================================================================*\
* EXPORTED FUNCTIONS
\*============================================================================*/

/**
 * @brief Getter function for TA states
 *
 */
TA_FF_State_T *Ta_Get_State_Output_Ptr(void)
{
   return &Ta_current_state;
}

/* coverity[misra_c_2012_rule_8_7_violation][Interface function must be defined with external linkage]*/
void Ta_Init_Input(Ta_Input_T *p_ta_input, const Radar_Position_T radar_position)
{

   p_ta_input->bmw_boardnet_signals                     = Ta_Get_Ta_Bmw_Boardnet_Ptr();
   p_ta_input->bmw_boardnet_signals->ta_warntrigger_hmi = BMW_HMI_WARNTRIGGER_NORMAL; /* Obsolete, but kept for old interface */

   /*
    * Following coding parameters and input bus signals default values
    * is kept in such a way that Component-Resim in Jenkins pass.
    * These values will require to be changed in future as per the requirement.
    * currently has to kept like these to make Component-Resim in Jenkins pass.
    */
   /*BMW SP25 specific coding parameters*/
   p_ta_input->ta_coding_parameters.c_f_ta_enabled           = FBK_FALSE;
   p_ta_input->ta_coding_parameters.c_f_dynamic_ta_enabled   = FBK_FALSE;
   p_ta_input->ta_coding_parameters.c_f_static_ta_enabled    = FBK_FALSE;
   p_ta_input->ta_coding_parameters.c_f_cross_ta_enabled     = FBK_FALSE;
   p_ta_input->ta_coding_parameters.c_ta_min_vel_lower_limit = TA_BMW_DEFAULT_MIN_VEL_LOWER_LIMIT;
   p_ta_input->ta_coding_parameters.c_ta_min_vel_upper_limit = TA_BMW_DEFAULT_MIN_VEL_UPPER_LIMIT;
   p_ta_input->ta_coding_parameters.c_ta_max_vel_lower_limit = TA_BMW_DEFAULT_MAX_VEL_LOWER_LIMIT;
   p_ta_input->ta_coding_parameters.c_ta_max_vel_upper_limit = TA_BMW_DEFAULT_MAX_VEL_UPPER_LIMIT;
   /*BMW SP25 specific input signals*/
   p_ta_input->ta_input_signals.vehicle_driving_direction = TA_SIGNAL_UNFILLED;
   p_ta_input->ta_input_signals.pwf_state                 = TA_PARKENBN_NIO;
   p_ta_input->ta_input_signals.status_dynamometer_mode   = NO_DYNAMOMETER;
   p_ta_input->ta_input_signals.status_end_of_line_mode   = TA_END_OF_LINE_MODE_NOT_SET;
   p_ta_input->ta_input_signals.ta_function_error         = FBK_FALSE;

   /* Check mounting position for enabling TA front mode */
   if ((UNKNOWN_POSITION == radar_position) || (FRONT_LEFT == radar_position) || (FRONT_RIGHT == radar_position))
   {
      p_ta_input->f_fta_enable = FBK_ONE_UINT;
   }
   else
   {
      p_ta_input->f_fta_enable = FBK_ZERO_UINT;
   }

   /* Check mounting position for enabling TA rear mode */
   if ((UNKNOWN_POSITION == radar_position) || (REAR_RIGHT == radar_position))
   {
      p_ta_input->f_rta_enable              = FBK_ONE_UINT;
      p_ta_input->f_rta_enable_dynamic_area = FBK_ONE_UINT;
      p_ta_input->f_rta_enable_turning_area = FBK_ONE_UINT;
   }
   else
   {
      p_ta_input->f_rta_enable              = FBK_ZERO_UINT;
      p_ta_input->f_rta_enable_dynamic_area = FBK_ZERO_UINT;
      p_ta_input->f_rta_enable_turning_area = FBK_ZERO_UINT;
   }

   p_ta_input->fta_steering_angle_max_left  = FBK_ZERO_UINT; /* Init with straight maneuver disabled */
   p_ta_input->fta_steering_angle_max_right = FBK_ZERO_UINT; /* Init with straight maneuver disabled */

   p_ta_input->fta_obj_offset_x_positive = FBK_ZERO_F; /* Init with NO object shift */
   p_ta_input->fta_obj_offset_x_negative = FBK_ZERO_F; /* Init with NO object shift */
   p_ta_input->fta_obj_offset_y_positive = FBK_ZERO_F; /* Init with NO object shift */
   p_ta_input->fta_obj_offset_y_negative = FBK_ZERO_F; /* Init with NO object shift */
}

void Ta_Pre_Run(Ta_Instance_T *p_ta_instance /**< TA Instance */,
                const Ta_Input_T *p_ta_input /**< TA Input */,
                const Fbk_Output_T *p_fbk_output)
{
   Ta_Core_Input_T *p_ta_core_input;
   const Ta_Core_Calibration_T *p_ta_cal;

   Fbk_Vehicle_Data_T vehicle_data;
   float32_T host_vehicle_speed;
   Ta_Alert_Trigger_T alert_trigger_ttp;

   /* Check if all input pointers are valid */
   assert(NULL != p_ta_instance);
   assert(NULL != p_ta_input);
   assert(NULL != p_fbk_output);

   p_ta_core_input = &p_ta_instance->core_input;
   p_ta_cal        = &p_ta_instance->calibration;

   /* Update context pointer. */
   p_ta_core_input->p_pa_data = p_fbk_output->p_pa_data;

   /* Fill vehicle data */
   vehicle_data = p_fbk_output->p_pa_data->vehicle_data;

   /* Diagnostic Job 0x404B (Move tracker object) */
   p_ta_core_input->f_enable_debug_mode = Ta_Is_Diagnostic_Mode_Enabled(p_ta_input, p_ta_cal);
   if (Fbk_Is_True(p_ta_core_input->f_enable_debug_mode))
   {
      p_ta_core_input->debug_mode_obj_pos_lat_offset  = Ta_Get_Target_Shift_Offset_Lat(p_ta_input);
      p_ta_core_input->debug_mode_obj_pos_long_offset = Ta_Get_Target_Shift_Offset_Long(p_ta_input);
   }

   Ta_Set_Current_Ta_Functional_State(p_ta_input, &Ta_current_state, &vehicle_data);

   if (TA_STATE_ACTIVE == Ta_current_state)
   {
      /* Enable global TA flag */
      if (Fbk_Is_True(p_ta_input->f_fta_enable) || Fbk_Is_True(p_ta_input->f_rta_enable))
      {
         p_ta_core_input->f_ta_enable = FBK_TRUE;
      }
      else
      {
         p_ta_core_input->f_ta_enable = FBK_FALSE;
      }
   }
   else
   {
      p_ta_core_input->f_ta_enable = FBK_FALSE;
   }
   if (p_ta_core_input->f_enable_debug_mode == FBK_TRUE)
   {
      p_ta_core_input->f_ta_enable = FBK_TRUE;
   }
   /* Set speed dependent TTP thresholds */
   host_vehicle_speed = vehicle_data.host_speed;
   if (host_vehicle_speed < p_ta_cal->k_ta_alert_lvl_1_ttp_late_trigger_host_speed_max)
   {
      alert_trigger_ttp = TA_ALERT_TRIGGER_LATE;
   }
   else if (host_vehicle_speed < p_ta_cal->k_ta_alert_lvl_1_ttp_normal_trigger_host_speed_max)
   {
      alert_trigger_ttp = TA_ALERT_TRIGGER_NORMAL;
   }
   else
   {
      alert_trigger_ttp = TA_ALERT_TRIGGER_EARLY;
   }
   p_ta_core_input->alert_ttp_threshold = p_ta_cal->k_ta_alert_lvl_1_ttp_threshold[alert_trigger_ttp];


   /* Set core input based on project input */
   p_ta_core_input->f_fta_enable = (boolean_T) (Fbk_Is_True(p_ta_input->f_fta_enable));
   p_ta_core_input->f_rta_enable = (boolean_T) (Fbk_Is_True(p_ta_input->f_rta_enable));
}

/* coverity[misra_c_2012_rule_8_7_violation][External use by customer is intended] */
Ta_BMW_Boardnet_T *Ta_Get_Ta_Bmw_Boardnet_Ptr(void)
{
   return (&Boardnet_Signals);
}
