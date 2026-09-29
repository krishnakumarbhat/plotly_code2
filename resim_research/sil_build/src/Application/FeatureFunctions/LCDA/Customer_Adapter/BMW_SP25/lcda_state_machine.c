/**
 * @file lcda_state_machine.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW SP25 State Machine logic for LCDA.
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 */
/*===========================================================================
 * Includes
 *=========================================================================*/
#include "lcda_state_machine.h"
#include "fbk_macros.h"
#include "pa_reuse.h"

/**
 * @brief Checks if all subfunctions are disabled or not.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static boolean_T Lcda_Are_All_Subfunctions_Disabled(const Lcda_Input_T *p_lcda_input);


/**
 * @brief Checks if Curve Radius is less than a threshold.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static boolean_T Lcda_Is_Curve_Radii_Less_Than_Threshold(const Lcda_Input_T *p_lcda_input);

/**
 * @brief Checks if Speed is less than Ego Speed Max Speed.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static boolean_T Lcda_Is_Speed_Less_Than_Equal_To_Upper_Limit_Ego_Speed(const Lcda_Input_T *p_lcda_input,
                                                                        const Fbk_Vehicle_Data_T *p_vehicle_data);


/**
 * @brief LCDA State Machine Transitions From Not-Available State.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Lcda_State_Machine_Transitions_From_Not_Available_State(const Lcda_Input_T *p_lcda_input,
                                                                    LCDA_FF_State_T *p_lcda_current_state);


/**
 * @brief LCDA State Machine Transitions From Available State.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Lcda_State_Machine_Transitions_From_Available_State(const Lcda_Input_T *p_lcda_input,
                                                                LCDA_FF_State_T *p_lcda_current_state);

/* @brief LCDA State Machine Transitions From Inactive State.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Lcda_State_Machine_Transitions_From_InActive_State(const Lcda_Input_T *p_lcda_input,
                                                               LCDA_FF_State_T *p_lcda_current_state,
                                                               const Fbk_Vehicle_Data_T *p_vehicle_data);


/* @brief LCDA State Machine Transitions From Active State.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Lcda_State_Machine_Transitions_From_Active_State(const Lcda_Input_T *p_lcda_input,
                                                             LCDA_FF_State_T *p_lcda_current_state,
                                                             const Fbk_Vehicle_Data_T *p_vehicle_data);


/* @brief LCDA State Machine Transitions From Error State.
 *
 * @return void
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 *
 * @verification{}
 */
static void Lcda_State_Machine_Transitions_From_Error_State(const Lcda_Input_T *p_lcda_input,
                                                            LCDA_FF_State_T *p_lcda_current_state,
                                                            const Fbk_Vehicle_Data_T *p_vehicle_data);


static boolean_T Lcda_Are_All_Subfunctions_Disabled(const Lcda_Input_T *p_lcda_input)
{
   boolean_T f_all_subfunctions_disabled =
      (boolean_T) ((FBK_ZERO_UINT == p_lcda_input->lcda_coding_parameters.c_f_lcda_enable_bsw)
                   && (FBK_ZERO_UINT == p_lcda_input->lcda_coding_parameters.c_f_lcda_enable_cvw)
                   && (FBK_ZERO_UINT == p_lcda_input->lcda_coding_parameters.c_f_lcda_enable_slc));
   return f_all_subfunctions_disabled;
}

static boolean_T Lcda_Is_Curve_Radii_Less_Than_Threshold(const Lcda_Input_T *p_lcda_input)
{
   const float32_T k_min_curve_radius = p_lcda_input->lcda_coding_parameters.c_min_curve_radii;
   boolean_T f_curve_radii            = (boolean_T) (p_lcda_input->lcda_input_signals.curve_radii < k_min_curve_radius);
   return f_curve_radii;
}

static boolean_T Lcda_Is_Speed_Less_Than_Equal_To_Upper_Limit_Ego_Speed(const Lcda_Input_T *p_lcda_input,
                                                                        const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   const float32_T k_upper_limit_ego_speed_lcda = p_lcda_input->lcda_coding_parameters.c_lcda_max_vel_upper_limit;
   boolean_T f_speed = (boolean_T) ((p_vehicle_data->host_speed * LCDA_MPS_2_KPH) <= k_upper_limit_ego_speed_lcda);
   return f_speed;
}

static void Lcda_State_Machine_Transitions_From_Not_Available_State(const Lcda_Input_T *p_lcda_input,
                                                                    LCDA_FF_State_T *p_lcda_current_state)
{
   if (Fbk_Is_True(p_lcda_input->lcda_coding_parameters.c_f_lcda_enabled))
   {
      *p_lcda_current_state = LCDA_STATE_AVAILABLE;
   }
   else
   {
      /*do nothing*/
   }
}

static void Lcda_State_Machine_Transitions_From_Available_State(const Lcda_Input_T *p_lcda_input, LCDA_FF_State_T *p_lcda_current_state)
{
   if ((FBK_FALSE == p_lcda_input->lcda_coding_parameters.c_f_lcda_enabled)
       || (Fbk_Is_True(Lcda_Are_All_Subfunctions_Disabled(p_lcda_input))))
   {
      *p_lcda_current_state = LCDA_STATE_NOT_AVAILABLE;
   }
   else
   {
      *p_lcda_current_state = LCDA_STATE_INACTIVE;
   }
}

static void Lcda_State_Machine_Transitions_From_InActive_State(const Lcda_Input_T *p_lcda_input,
                                                               LCDA_FF_State_T *p_lcda_current_state,
                                                               const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   boolean_T f_is_speed_less_than_equal_to_upper_limit_ego_speed =
      (boolean_T) (Fbk_Is_True(Lcda_Is_Speed_Less_Than_Equal_To_Upper_Limit_Ego_Speed(p_lcda_input, p_vehicle_data)));
   boolean_T f_is_vehicle_condition_driving_living_parking =
      (boolean_T) ((p_lcda_input->lcda_input_signals.vehicle_condition == FAHREN)
                   || (p_lcda_input->lcda_input_signals.vehicle_condition == WOHNEN)
                   || (p_lcda_input->lcda_input_signals.vehicle_condition == PARKENBN_IO));
   boolean_T f_is_curve_radii_not_less_than_threshold =
      (boolean_T) (Fbk_Is_False(Lcda_Is_Curve_Radii_Less_Than_Threshold(p_lcda_input)));
   boolean_T f_is_vehicle_not_moving_backward =
      (boolean_T) (p_lcda_input->lcda_input_signals.vehicle_driving_direction != VEHICLE_MOVES_BACKWARD);
   boolean_T f_is_there_no_error = (boolean_T) (p_lcda_input->lcda_input_signals.lcda_function_error == LEVEL0);

   if (f_is_speed_less_than_equal_to_upper_limit_ego_speed && f_is_vehicle_condition_driving_living_parking
       && f_is_vehicle_not_moving_backward && f_is_curve_radii_not_less_than_threshold && f_is_there_no_error)
   {
      *p_lcda_current_state = LCDA_STATE_ACTIVE;
   }
   else if (p_lcda_input->lcda_input_signals.lcda_function_error == LEVEL2)
   {
      *p_lcda_current_state = LCDA_STATE_ERROR;
   }
   else if ((Fbk_Is_True(Lcda_Are_All_Subfunctions_Disabled(p_lcda_input)))
            || (FBK_FALSE == p_lcda_input->lcda_coding_parameters.c_f_lcda_enabled))
   {
      *p_lcda_current_state = LCDA_STATE_NOT_AVAILABLE;
   }
   else
   {
      /*do nothing*/
   }
}

static void Lcda_State_Machine_Transitions_From_Active_State(const Lcda_Input_T *p_lcda_input,
                                                             LCDA_FF_State_T *p_lcda_current_state,
                                                             const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   boolean_T f_is_speed_not_less_than_equal_to_upper_limit_ego_speed =
      (boolean_T) (Fbk_Is_False(Lcda_Is_Speed_Less_Than_Equal_To_Upper_Limit_Ego_Speed(p_lcda_input, p_vehicle_data)));

   boolean_T f_is_vehicle_condition_not_in_driving_living_parking =
      (boolean_T) ((p_lcda_input->lcda_input_signals.vehicle_condition != FAHREN)
                   && (p_lcda_input->lcda_input_signals.vehicle_condition != WOHNEN)
                   && (p_lcda_input->lcda_input_signals.vehicle_condition != PARKENBN_IO));

   boolean_T f_is_vehicle_moving_backward =
      (boolean_T) (p_lcda_input->lcda_input_signals.vehicle_driving_direction == VEHICLE_MOVES_BACKWARD);
   boolean_T f_is_curve_radii_less_than_threshold = (boolean_T) (Fbk_Is_True(Lcda_Is_Curve_Radii_Less_Than_Threshold(p_lcda_input)));
   boolean_T f_is_there_no_error                  = (boolean_T) (p_lcda_input->lcda_input_signals.lcda_function_error == LEVEL0);

   if ((((f_is_speed_not_less_than_equal_to_upper_limit_ego_speed && f_is_vehicle_condition_not_in_driving_living_parking)
         && f_is_vehicle_moving_backward)
        || (f_is_curve_radii_less_than_threshold))
       && (f_is_there_no_error))
   {
      *p_lcda_current_state = LCDA_STATE_INACTIVE;
   }
   else if (p_lcda_input->lcda_input_signals.lcda_function_error == LEVEL2)
   {
      *p_lcda_current_state = LCDA_STATE_ERROR;
   }
   else if ((Fbk_Is_True(Lcda_Are_All_Subfunctions_Disabled(p_lcda_input)))
            || (FBK_FALSE == p_lcda_input->lcda_coding_parameters.c_f_lcda_enabled))
   {
      *p_lcda_current_state = LCDA_STATE_NOT_AVAILABLE;
   }
   else
   {
      /*do nothing*/
   }
}

static void Lcda_State_Machine_Transitions_From_Error_State(const Lcda_Input_T *p_lcda_input,
                                                            LCDA_FF_State_T *p_lcda_current_state,
                                                            const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   boolean_T f_is_vehicle_condition_not_in_driving_living_parking =
      (boolean_T) ((p_lcda_input->lcda_input_signals.vehicle_condition != FAHREN)
                   && (p_lcda_input->lcda_input_signals.vehicle_condition != WOHNEN)
                   && (p_lcda_input->lcda_input_signals.vehicle_condition != PARKENBN_IO));

   boolean_T f_is_vehicle_moving_backward =
      (boolean_T) (p_lcda_input->lcda_input_signals.vehicle_driving_direction == VEHICLE_MOVES_BACKWARD);
   boolean_T f_is_there_no_error = (boolean_T) (p_lcda_input->lcda_input_signals.lcda_function_error == LEVEL0);
   boolean_T f_is_vehicle_condition_driving_living_parking =
      (boolean_T) ((p_lcda_input->lcda_input_signals.vehicle_condition == FAHREN)
                   || (p_lcda_input->lcda_input_signals.vehicle_condition == WOHNEN)
                   || (p_lcda_input->lcda_input_signals.vehicle_condition == PARKENBN_IO));
   boolean_T f_is_vehicle_moving_forward =
      (boolean_T) (p_lcda_input->lcda_input_signals.vehicle_driving_direction == VEHICLE_MOVES_FORWARD);

   if ((Fbk_Is_True(Lcda_Are_All_Subfunctions_Disabled(p_lcda_input)))
       || (FBK_FALSE == p_lcda_input->lcda_coding_parameters.c_f_lcda_enabled))
   {
      *p_lcda_current_state = LCDA_STATE_NOT_AVAILABLE;
   }
   else if (((Fbk_Is_False(Lcda_Is_Speed_Less_Than_Equal_To_Upper_Limit_Ego_Speed(p_lcda_input, p_vehicle_data)))
             || f_is_vehicle_condition_not_in_driving_living_parking || f_is_vehicle_moving_backward)
            && (f_is_there_no_error))
   {
      *p_lcda_current_state = LCDA_STATE_INACTIVE;
   }
   else if ((Fbk_Is_True(Lcda_Is_Speed_Less_Than_Equal_To_Upper_Limit_Ego_Speed(p_lcda_input, p_vehicle_data)))
            && f_is_vehicle_condition_driving_living_parking && (f_is_vehicle_moving_forward) && (f_is_there_no_error))
   {
      *p_lcda_current_state = LCDA_STATE_ACTIVE;
   }
   else
   {
      /*do nothing*/
   }
}

void Lcda_Set_Current_Lcda_Functional_State(const Lcda_Input_T *p_lcda_input,
                                            LCDA_FF_State_T *p_lcda_current_state,
                                            const Fbk_Vehicle_Data_T *p_vehicle_data)
{
   /*State Transitions from Not Available State*/
   if (*p_lcda_current_state == LCDA_STATE_NOT_AVAILABLE)
   {
      Lcda_State_Machine_Transitions_From_Not_Available_State(p_lcda_input, p_lcda_current_state);
   }
   else
   {
      /* do nothing*/
   }
   /*State Transitions from Available State*/
   if (*p_lcda_current_state == LCDA_STATE_AVAILABLE)
   {
      Lcda_State_Machine_Transitions_From_Available_State(p_lcda_input, p_lcda_current_state);
   }
   else
   {
      /*do nothing*/
   }
   /*State Transitions From InActive State*/
   if (*p_lcda_current_state == LCDA_STATE_INACTIVE)
   {
      /* Get it clarified */
      Lcda_State_Machine_Transitions_From_InActive_State(p_lcda_input, p_lcda_current_state, p_vehicle_data);
   }
   else
   {
      /*do nothing*/
   }
   /*State Transitions From Active State*/
   if (*p_lcda_current_state == LCDA_STATE_ACTIVE)
   {
      Lcda_State_Machine_Transitions_From_Active_State(p_lcda_input, p_lcda_current_state, p_vehicle_data);
   }
   else
   {
      /*do nothing*/
   }
   /*State Transitions From Error State*/
   if (*p_lcda_current_state == LCDA_STATE_ERROR)
   {
      Lcda_State_Machine_Transitions_From_Error_State(p_lcda_input, p_lcda_current_state, p_vehicle_data);
   }
   else
   {
      /*do nothing*/
   }
}
