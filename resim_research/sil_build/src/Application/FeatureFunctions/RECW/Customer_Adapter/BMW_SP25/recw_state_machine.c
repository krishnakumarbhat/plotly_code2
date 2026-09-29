/**
 * @file recw_state_machine.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief This is the RECW state machine implementation file shared by all customers.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */
/*===========================================================================*\
 * Includes
\*===========================================================================*/

#include "recw_state_machine.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_data.h"
#include "pa_reuse.h"


/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

static Recw_SM_State_T Recw_Current_State = RECW_SM_NOT_AVAILABLE;

static Recw_SM_State_T Recw_Sm_Pr_State_Transitions_Ready(const Recw_Input_T *p_recw_input);

static Recw_SM_State_T Recw_Sm_Pr_State_Transitions_Active(const Recw_Input_T *p_recw_input);

static Recw_SM_State_T Recw_Sm_Pr_State_Transitions_Degraded(const Recw_Input_T *p_recw_input);

static uint8_t Recw_Sm_Pr_Compute_Test_Mode(const Recw_Input_T *p_recw_input);

/**
 * @brief Calculates the overlap of an tracker object and the ego vehicle.
 *
 * @return overlap (measured between 0,0 and 1,0)
 *
 * @SRD{WI-21344}
 * @SAD{WI-17537}
 * @SDD{WI-19468}
 * @verification{}
 */

static float32_T Recw_Calculate_Collision_Overlap(const Pa_Data_T *p_pa_data, uint8_t obj_index);

static float32_T Recw_Calculate_Relative_Velocity(const Pa_Data_T *p_pa_data, uint8_t obj_index);

static float32_T Recw_Calculate_Object_Distance(const Pa_Data_T *p_pa_data, uint8_t obj_index);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

Recw_SM_State_T Recw_Get_State(void)
{
   return Recw_Current_State;
}

void Recw_Set_State(Recw_SM_State_T state)
{
   Recw_Current_State = state;
}

static uint8_t Recw_Sm_Pr_Compute_Test_Mode(const Recw_Input_T *p_recw_input)
{
   uint8_t recw_test_mode;
   if ((RECW_STATUS_DYNAMOMETER_FRONT_AXLE_ON_DYNAMOMETER == p_recw_input->status_roller_dynamometer)
       || (RECW_STATUS_DYNAMOMETER_BACK_AXLE_ON_DYNAMOMETER == p_recw_input->status_roller_dynamometer)
       || (RECW_STATUS_DYNAMOMETER_TWO_AXLE_DYNAMOMETER == p_recw_input->status_roller_dynamometer)
       || (RECW_STATUS_END_OF_LINE_MODE_SET == p_recw_input->status_end_of_line))
   {
      recw_test_mode = 1U;
   }
   else
   {
      recw_test_mode = 0U;
   }
   return recw_test_mode;
}

static Recw_SM_State_T Recw_Sm_Pr_State_Transitions_Ready(const Recw_Input_T *p_recw_input)
{
   Recw_SM_State_T new_state = Recw_Current_State;
   if (RECW_STATE_DISABLED == p_recw_input->c_recw_enable)
   {
      new_state = RECW_SM_NOT_AVAILABLE;
   }
   else if ((RECW_NO_ERROR == p_recw_input->recw_error) && (RECW_VEHICLE_MOVING_BACKWARD != p_recw_input->vehicle_movement_status)
            && (RECW_NO_TRAILER_AVAILABLE == p_recw_input->status_trailer)
            && (RECW_STATUS_DYNAMOMETER_NO_DYNAMOMETER == p_recw_input->status_roller_dynamometer)
            && (RECW_STATUS_END_OF_LINE_MODE_NOT_SET == p_recw_input->status_end_of_line))
   {
      new_state = RECW_SM_ACTIVE;
   }
   else if ((RECW_NON_CRITICAL_ERROR == p_recw_input->recw_error)
            && (RECW_STATUS_DYNAMOMETER_NO_DYNAMOMETER == p_recw_input->status_roller_dynamometer)
            && (RECW_STATUS_END_OF_LINE_MODE_NOT_SET == p_recw_input->status_end_of_line))
   {
      new_state = RECW_SM_DEGRADED;
   }
   else if (RECW_CRITICAL_ERROR == p_recw_input->recw_error)
   {
      new_state = RECW_SM_ERROR;
   }
   else
   {
      /* Do nothing*/
   }
   return new_state;
}

static Recw_SM_State_T Recw_Sm_Pr_State_Transitions_Active(const Recw_Input_T *p_recw_input)
{
   Recw_SM_State_T new_state = Recw_Current_State;

   if ((((RECW_VEHICLE_MOVING_BACKWARD == p_recw_input->vehicle_movement_status)
         || (RECW_TRAILER_AVAILABLE == p_recw_input->status_trailer) || (1U == p_recw_input->f_recw_enable_m_drive))
        && (RECW_NO_ERROR == p_recw_input->recw_error))
       || (1U == Recw_Sm_Pr_Compute_Test_Mode(p_recw_input)))
   {
      new_state = RECW_SM_READY;
   }
   else if (RECW_NON_CRITICAL_ERROR == p_recw_input->recw_error)
   {
      new_state = RECW_SM_DEGRADED;
   }
   else if (RECW_CRITICAL_ERROR == p_recw_input->recw_error)
   {
      new_state = RECW_SM_ERROR;
   }
   else
   {
      /* Do nothing*/
   }
   return new_state;
}

static Recw_SM_State_T Recw_Sm_Pr_State_Transitions_Degraded(const Recw_Input_T *p_recw_input)
{
   Recw_SM_State_T new_state = Recw_Current_State;
   if ((((RECW_VEHICLE_MOVING_BACKWARD == p_recw_input->vehicle_movement_status)
         || (RECW_TRAILER_AVAILABLE == p_recw_input->status_trailer) || (1U == p_recw_input->f_recw_enable_m_drive))
        && (RECW_NO_ERROR == p_recw_input->recw_error))
       || (1U == Recw_Sm_Pr_Compute_Test_Mode(p_recw_input)))
   {
      new_state = RECW_SM_READY;
   }
   else if ((RECW_NO_ERROR == p_recw_input->recw_error) && (RECW_VEHICLE_MOVING_BACKWARD != p_recw_input->vehicle_movement_status)
            && (RECW_NO_TRAILER_AVAILABLE == p_recw_input->status_trailer)
            && (RECW_STATUS_DYNAMOMETER_NO_DYNAMOMETER == p_recw_input->status_roller_dynamometer)
            && (RECW_STATUS_END_OF_LINE_MODE_NOT_SET == p_recw_input->status_end_of_line))
   {
      new_state = RECW_SM_ACTIVE;
   }
   else if (RECW_CRITICAL_ERROR == p_recw_input->recw_error)
   {
      new_state = RECW_SM_ERROR;
   }
   else
   {
      /* Do nothing*/
   }
   return new_state;
}

void Recw_State_Machine_Pre_Run(const Recw_Input_T *p_recw_input)
{
   Recw_SM_State_T new_state = Recw_Current_State;
   switch (Recw_Current_State)
   {
      case (RECW_SM_NOT_AVAILABLE):
         if (RECW_STATE_ENABLED == p_recw_input->c_recw_enable)
         {
            new_state = RECW_SM_READY;
         }
         else
         {
            /* Do nothing*/
         }
         break;
      case (RECW_SM_READY):
         new_state = Recw_Sm_Pr_State_Transitions_Ready(p_recw_input);
         break;
      case (RECW_SM_ACTIVE):
         new_state = Recw_Sm_Pr_State_Transitions_Active(p_recw_input);
         break;
      case (RECW_SM_DEGRADED):
         new_state = Recw_Sm_Pr_State_Transitions_Degraded(p_recw_input);
         break;
      case (RECW_SM_ERROR):
         if (RECW_NO_ERROR == p_recw_input->recw_error)
         {
            new_state = RECW_SM_READY;
         }
         else
         {
            /* Do nothing*/
         }
         break;
      default:
         new_state = RECW_SM_ERROR;
         break;
   }

   Recw_Current_State = new_state;
}

static float32_T Recw_Calculate_Collision_Overlap(const Pa_Data_T *p_pa_data, uint8_t obj_index)
{
   /* Set overlap to hard coded value for now, until calculation method is clear */
   float32_T return_overlap;
   float32_T ego_left_border;
   float32_T ego_right_border;
   float32_T obj_left_border;
   float32_T obj_right_border;
   float32_T obj_width;

   /* Get ego vehicle borders */
   ego_left_border  = -0.5f * p_pa_data->vehicle_data.host_width;
   ego_right_border = 0.5f * p_pa_data->vehicle_data.host_width;

   /* Get lateral position of front corners of target*/
   obj_width        = p_pa_data->object_data[obj_index].width;
   obj_left_border  = p_pa_data->object_data[obj_index].vcs_pos.y - (0.5f * obj_width);
   obj_right_border = p_pa_data->object_data[obj_index].vcs_pos.y + (0.5f * obj_width);

   /* Check, where target is compared to ego borders */
   if ((obj_right_border > ego_left_border) && (obj_left_border < ego_left_border))
   {
      /* Target overlapping with ego from left ego border */
      return_overlap = Fbk_Abs_F((obj_right_border - ego_left_border) / obj_width);
   }
   else if (obj_right_border <= ego_left_border)
   {
      /* Target left of ego */
      return_overlap = FBK_ZERO_F;
   }
   else if ((obj_right_border > ego_right_border) && (obj_left_border < ego_right_border))
   {
      /* Target overlapping with ego from right ego border */
      return_overlap = Fbk_Abs_F((obj_left_border - ego_right_border) / obj_width);
   }
   else if (((obj_right_border <= ego_right_border) && (obj_left_border >= ego_left_border))
            || ((obj_right_border >= ego_right_border) && (obj_left_border <= ego_left_border)))
   {
      /* Complete overlap of target and ego with possibly different dimensions */
      return_overlap = 1.0f;
   }
   else
   {
      /* Target right of ego */
      return_overlap = FBK_ZERO_F;
   }

   return return_overlap;
}

static float32_T Recw_Calculate_Relative_Velocity(const Pa_Data_T *p_pa_data, uint8_t obj_index)
{
   float32_T rel_vel = p_pa_data->vehicle_data.host_speed - p_pa_data->object_data[obj_index].speed;
   return rel_vel;
}

static float32_T Recw_Calculate_Object_Distance(const Pa_Data_T *p_pa_data, uint8_t obj_index)
{
   float32_T x_dist     = p_pa_data->object_data[obj_index].vcs_pos.x;
   float32_T y_dist     = p_pa_data->object_data[obj_index].vcs_pos.y;
   float32_T total_dist = Fast_Sqrt((x_dist * x_dist) + (y_dist * y_dist));
   return total_dist;
}

void Recw_State_Machine_Post_Run(const Recw_Input_T *p_recw_input,
                                 const Recw_Core_Output_T *p_core_output,
                                 const Pa_Data_T *p_pa_data,
                                 Recw_Output_T *p_recw_output)
{
   switch (Recw_Current_State)
   {
      case (RECW_SM_NOT_AVAILABLE):
         p_recw_output->recw_status_collision_warning = RECW_BMW_SP25_SM_HEX_0;
         p_recw_output->recw_status_precrash          = RECW_BMW_SP25_SM_HEX_0;
         p_recw_output->recw_ttc                      = (float32_T) RECW_BMW_SP25_SM_HEX_FC;
         p_recw_output->recw_obj_approach_speed       = (float32_T) RECW_BMW_SP25_SM_HEX_FC;
         p_recw_output->recw_obj_distance             = (float32_T) RECW_BMW_SP25_SM_HEX_0;
         p_recw_output->recw_overlap                  = (float32_T) RECW_BMW_SP25_SM_HEX_0;
         p_recw_output->recw_obj_class                = RECW_BMW_SP25_SM_HEX_0;
         p_recw_output->recw_obj_heading              = (float32_T) RECW_BMW_SP25_SM_HEX_0;

         p_recw_output->edr_drasy_event_ID62_status_collision_warning_side_radar_rear = RECW_BMW_SP25_SM_HEX_0;
         p_recw_output->edr_drasy_event_ID62_status_pre_crash_side_radar_rear         = RECW_BMW_SP25_SM_HEX_0;
         break;
      case (RECW_SM_READY):
         p_recw_output->recw_status_collision_warning = RECW_BMW_SP25_SM_HEX_0;
         p_recw_output->recw_status_precrash          = RECW_BMW_SP25_SM_HEX_0;
         p_recw_output->recw_ttc                      = (float32_T) RECW_BMW_SP25_SM_HEX_FC;
         p_recw_output->recw_obj_approach_speed       = (float32_T) RECW_BMW_SP25_SM_HEX_0;
         p_recw_output->recw_obj_distance             = (float32_T) RECW_BMW_SP25_SM_HEX_0;
         p_recw_output->recw_overlap                  = (float32_T) RECW_BMW_SP25_SM_HEX_0;
         p_recw_output->recw_obj_class                = RECW_BMW_SP25_SM_HEX_0;
         p_recw_output->recw_obj_heading              = (float32_T) RECW_BMW_SP25_SM_HEX_0;

         p_recw_output->edr_drasy_event_ID62_status_collision_warning_side_radar_rear = RECW_BMW_SP25_SM_HEX_0;
         p_recw_output->edr_drasy_event_ID62_status_pre_crash_side_radar_rear         = RECW_BMW_SP25_SM_HEX_0;
         break;
      case (RECW_SM_ACTIVE):
         /*
          * Approach speed precrash
          * TTC precrash
          * Status precrash
          * Distance to obstacle
          * Collision overlap
          * Obstacle type
          * Collision angle
          */
         if ((RECW_ALERT_ACTIVE_LEVEL_2 == p_core_output->recw_alert_level)
             && ((RECW_BMW_SP25_TYPE_WARNING_AND_PRECRASH == p_recw_input->recw_type)
                 || (RECW_BMW_SP25_TYPE_PRECRASH_ONLY == p_recw_input->recw_type)))
         {
            p_recw_output->recw_obj_approach_speed =
               Fbk_Abs_F(Recw_Calculate_Relative_Velocity(p_pa_data, p_core_output->recw_index));
            p_recw_output->recw_ttc                                              = p_core_output->recw_ttc;
            p_recw_output->recw_status_precrash                                  = RECW_BMW_SP25_SM_HEX_4;
            p_recw_output->edr_drasy_event_ID62_status_pre_crash_side_radar_rear = RECW_BMW_SP25_SM_HEX_4;
            p_recw_output->recw_obj_distance = Recw_Calculate_Object_Distance(p_pa_data, p_core_output->recw_index);
            p_recw_output->recw_overlap =
               RECW_BMW_SP25_CONVERT_TO_PERCENTAGE(Recw_Calculate_Collision_Overlap(p_pa_data, p_core_output->recw_index));
            p_recw_output->recw_obj_class   = (uint8_t) p_pa_data->object_data[p_core_output->recw_index].obj_class;
            p_recw_output->recw_obj_heading = p_pa_data->object_data[p_core_output->recw_index].vcs_heading;
         }
         else
         {
            p_recw_output->recw_obj_approach_speed                               = (float32_T) RECW_BMW_SP25_SM_HEX_0;
            p_recw_output->recw_ttc                                              = (float32_T) RECW_BMW_SP25_SM_HEX_FC;
            p_recw_output->recw_status_precrash                                  = RECW_BMW_SP25_SM_HEX_0;
            p_recw_output->edr_drasy_event_ID62_status_pre_crash_side_radar_rear = RECW_BMW_SP25_SM_HEX_0;
            p_recw_output->recw_obj_distance                                     = (float32_T) RECW_BMW_SP25_SM_HEX_0;
            p_recw_output->recw_overlap                                          = (float32_T) RECW_BMW_SP25_SM_HEX_0;
            p_recw_output->recw_obj_class                                        = RECW_BMW_SP25_SM_HEX_0;
            p_recw_output->recw_obj_heading                                      = (float32_T) RECW_BMW_SP25_SM_HEX_0;
         }

         /* Status collision warning*/
         if (((RECW_ALERT_ACTIVE_LEVEL_1 == p_core_output->recw_alert_level)
              || (RECW_ALERT_ACTIVE_LEVEL_2 == p_core_output->recw_alert_level))
             && ((RECW_BMW_SP25_TYPE_WARNING_AND_PRECRASH == p_recw_input->recw_type)
                 || (RECW_BMW_SP25_TYPE_PRECRASH_ONLY == p_recw_input->recw_type)))
         {
            if (RECW_ALERT_ACTIVE_LEVEL_2 == p_core_output->recw_alert_level)
            {
               p_recw_output->recw_status_collision_warning = RECW_BMW_SP25_SM_HEX_4;
            }
            else
            {
               p_recw_output->recw_status_collision_warning = RECW_BMW_SP25_SM_HEX_1;
            }
         }
         else
         {
            p_recw_output->recw_status_collision_warning = RECW_BMW_SP25_SM_HEX_0;
         }

         break;
      case (RECW_SM_DEGRADED):
         p_recw_output->recw_status_collision_warning = RECW_BMW_SP25_SM_HEX_6;
         p_recw_output->recw_status_precrash          = RECW_BMW_SP25_SM_HEX_E;
         p_recw_output->recw_ttc                      = (float32_T) RECW_BMW_SP25_SM_HEX_FE;
         p_recw_output->recw_obj_approach_speed       = (float32_T) RECW_BMW_SP25_SM_HEX_FE;
         p_recw_output->recw_obj_distance             = (float32_T) RECW_BMW_SP25_SM_HEX_FE;
         p_recw_output->recw_overlap                  = (float32_T) RECW_BMW_SP25_SM_HEX_E;
         p_recw_output->recw_obj_class                = RECW_BMW_SP25_SM_HEX_E;
         p_recw_output->recw_obj_heading              = (float32_T) RECW_BMW_SP25_SM_HEX_FE;

         p_recw_output->edr_drasy_event_ID62_status_collision_warning_side_radar_rear = RECW_BMW_SP25_SM_HEX_6;
         p_recw_output->edr_drasy_event_ID62_status_pre_crash_side_radar_rear         = RECW_BMW_SP25_SM_HEX_E;
         break;
      case (RECW_SM_ERROR):
         p_recw_output->recw_status_collision_warning = RECW_BMW_SP25_SM_HEX_6;
         p_recw_output->recw_status_precrash          = RECW_BMW_SP25_SM_HEX_E;
         p_recw_output->recw_ttc                      = (float32_T) RECW_BMW_SP25_SM_HEX_FE;
         p_recw_output->recw_obj_approach_speed       = (float32_T) RECW_BMW_SP25_SM_HEX_FE;
         p_recw_output->recw_obj_distance             = (float32_T) RECW_BMW_SP25_SM_HEX_FE;
         p_recw_output->recw_overlap                  = (float32_T) RECW_BMW_SP25_SM_HEX_E;
         p_recw_output->recw_obj_class                = RECW_BMW_SP25_SM_HEX_E;
         p_recw_output->recw_obj_heading              = (float32_T) RECW_BMW_SP25_SM_HEX_FE;

         p_recw_output->edr_drasy_event_ID62_status_collision_warning_side_radar_rear = RECW_BMW_SP25_SM_HEX_6;
         p_recw_output->edr_drasy_event_ID62_status_pre_crash_side_radar_rear         = RECW_BMW_SP25_SM_HEX_E;
         break;
      default:
         p_recw_output->recw_status_collision_warning = RECW_BMW_SP25_SM_HEX_0;
         p_recw_output->recw_status_precrash          = RECW_BMW_SP25_SM_HEX_0;
         p_recw_output->recw_ttc                      = (float32_T) RECW_BMW_SP25_SM_HEX_0;
         p_recw_output->recw_obj_approach_speed       = (float32_T) RECW_BMW_SP25_SM_HEX_0;
         p_recw_output->recw_obj_distance             = (float32_T) RECW_BMW_SP25_SM_HEX_0;
         p_recw_output->recw_overlap                  = (float32_T) RECW_BMW_SP25_SM_HEX_0;
         p_recw_output->recw_obj_class                = RECW_BMW_SP25_SM_HEX_0;
         p_recw_output->recw_obj_heading              = (float32_T) RECW_BMW_SP25_SM_HEX_0;

         p_recw_output->edr_drasy_event_ID62_status_collision_warning_side_radar_rear = RECW_BMW_SP25_SM_HEX_0;
         p_recw_output->edr_drasy_event_ID62_status_pre_crash_side_radar_rear         = RECW_BMW_SP25_SM_HEX_0;
         break;
   }
   p_recw_output->recw_sm_state = Recw_Current_State;
}
