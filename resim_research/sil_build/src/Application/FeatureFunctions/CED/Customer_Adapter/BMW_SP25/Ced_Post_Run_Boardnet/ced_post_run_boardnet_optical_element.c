/**
 * @file ced_post_run_boardnet_optical_elements.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the output adaptations for the boardnet signals.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_post_run_boardnet_optical_element.h"
#include "ced_bmw_sp25_types.h"
#include "ced_post_run_boardnet_seat_doors.h"
#include "ced_types.h"
#include "fbk_macros.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

/**
 * @brief Resets the mirror led value for the given side.
 *
 * @return writes into p_ced_output
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static void Ced_Control_Optical_Element_Reset(Ced_Output_T *p_ced_output, const Ced_Door_Position_T ced_door);
/**
 * @brief Sets the optical elements for a single side (left /right) dependent on the inputs and core warnings.
 *
 * @return writes the value for the optical elements into p_ced_output
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static void Ced_Control_Optical_Elements_Side(Ced_Output_T *p_ced_output,
                                              const Ced_Core_Output_T *p_ced_core_output,
                                              const Ced_Input_T *p_ced_input,
                                              const Ced_Door_Position_T ced_door);

/**
 * @brief Returns if the pre-conditions for setting optical elements at a specific door position are fulfilled.
 *
 * @return FBK_TRUE = warning is ready to be set.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static boolean_T Ced_Is_Optical_Element_Warning_Enabled(const Ced_Input_T *p_ced_input, const Ced_Door_Position_T ced_door);

/**
 * @brief Checks if the actute warning is ready to be set resp. that all conditions are fulfilled for the warning to be set.
 *
 * @return FBK_TRUE = acute warning is ready to be set.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static boolean_T Ced_Is_Optical_Element_Acute_Warning_Enabled(const Ced_Input_T *p_ced_input, const Ced_Door_Position_T ced_door);

/**
 * @brief Checks if the information is ready to be set resp. that all conditions are fulfilled for the warning to be set.
 *
 * @return FBK_TRUE = information is ready to be set.
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static boolean_T Ced_Is_Optical_Element_Information_Enabled(const Ced_Output_T *p_ced_output,
                                                            const Ced_Input_T *p_ced_input,
                                                            const Ced_Door_Position_T ced_door);

/**
 * @brief Sets the Optical Element warning type equals to the given value into Ced_Output_T.
 *
 * @return fills p_ced_output with the warning state
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static void Ced_Set_Optical_Element_By_Type(Ced_Output_T *p_ced_output,
                                            const Ced_Target_Travel_Direction_T ced_travel_direction,
                                            const Ced_Door_Position_T ced_door);

/**
 * @brief Checks if Parental Control on Rear Seats are Active Or not
 *
 * @return boolean_T
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static boolean_T Ced_Is_Parental_Control_Active_On_Rear_Seat_For_Optical_Element(const Ced_Input_T *p_ced_input);


/**
 * @brief Checks id a Rear Door is Opened or Requested to be opened
 *
 * @return boolean_T
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static boolean_T Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_With_Parental_Control(const Ced_Input_T *p_ced_input);

/**
 * @brief Checks if Acute Warning Needs to be Suppressed
 *
 * @return boolean_T
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static boolean_T Ced_Is_Acute_Warning_Needs_To_Be_Suppressed_For_Optical_Element(const Ced_Input_T *p_ced_input);


/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

void Ced_Control_Optical_Elements(Ced_Output_T *p_ced_output, const Ced_Core_Output_T *p_ced_core_output, const Ced_Input_T *p_ced_input)
{
   Ced_Control_Optical_Elements_Side(p_ced_output, p_ced_core_output, p_ced_input, CED_DOOR_POSITION_FRONT_LEFT);
   Ced_Control_Optical_Elements_Side(p_ced_output, p_ced_core_output, p_ced_input, CED_DOOR_POSITION_FRONT_RIGHT);
   Ced_Control_Optical_Elements_Side(p_ced_output, p_ced_core_output, p_ced_input, CED_DOOR_POSITION_REAR_LEFT);
   Ced_Control_Optical_Elements_Side(p_ced_output, p_ced_core_output, p_ced_input, CED_DOOR_POSITION_REAR_RIGHT);
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static boolean_T Ced_Is_Parental_Control_Active_On_Rear_Seat_For_Optical_Element(const Ced_Input_T *p_ced_input)
{
   boolean_T f_is_parental_control_active_on_rear_seat = (boolean_T) FBK_FALSE;
   if (Fbk_Is_True(p_ced_input->ced_input_bus_signals.f_ced_parental_controls_driver_rear)
       || Fbk_Is_True(p_ced_input->ced_input_bus_signals.f_ced_parental_controls_passenger_rear))
   {
      f_is_parental_control_active_on_rear_seat = (boolean_T) FBK_TRUE;
   }
   return f_is_parental_control_active_on_rear_seat;
}

static boolean_T Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_With_Parental_Control(const Ced_Input_T *p_ced_input)
{
   boolean_T f_door_opened_or_requested_to_be_opened = (boolean_T) FBK_FALSE;
   if (Fbk_Is_False(p_ced_input->ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left)
       || Fbk_Is_False(p_ced_input->ced_input_bus_signals.f_ced_vehicle_door_lock_rear_right)
       || Fbk_Is_True(p_ced_input->ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_left)
       || Fbk_Is_True(p_ced_input->ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_right))
   {
      f_door_opened_or_requested_to_be_opened = (boolean_T) FBK_TRUE;
   }
   return f_door_opened_or_requested_to_be_opened;
}

static boolean_T Ced_Is_Acute_Warning_Needs_To_Be_Suppressed_For_Optical_Element(const Ced_Input_T *p_ced_input)
{
   boolean_T f_is_parental_controls_active_on_rear_seat = Ced_Is_Parental_Control_Active_On_Rear_Seat_For_Optical_Element(p_ced_input);
   boolean_T f_is_door_opened_or_requested_to_be_opened =
      Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_With_Parental_Control(p_ced_input);
   boolean_T f_result_is_acute_warning_needs_to_be_suppressed;
   if (f_is_parental_controls_active_on_rear_seat && f_is_door_opened_or_requested_to_be_opened)
   {
      f_result_is_acute_warning_needs_to_be_suppressed = (boolean_T) (FBK_TRUE);
   }
   else
   {
      f_result_is_acute_warning_needs_to_be_suppressed = (boolean_T) (FBK_FALSE);
   }
   return f_result_is_acute_warning_needs_to_be_suppressed;
}


static void Ced_Control_Optical_Element_Reset(Ced_Output_T *p_ced_output, const Ced_Door_Position_T ced_door)
{
   p_ced_output->ced_output_warning_indicators.ced_output_warning_optical[ced_door] = UNDEF_DIRECTION;
}

static void Ced_Control_Optical_Elements_Side(Ced_Output_T *p_ced_output,
                                              const Ced_Core_Output_T *p_ced_core_output,
                                              const Ced_Input_T *p_ced_input,
                                              const Ced_Door_Position_T ced_door)
{
   const Ced_Alert_T alert_state = Ced_Get_Core_Alert_State(p_ced_core_output, ced_door);
   const boolean_T f_ced_is_acute_warning_needs_to_be_suppressed =
      Ced_Is_Acute_Warning_Needs_To_Be_Suppressed_For_Optical_Element(p_ced_input);
   /* checks for an alert state level 3 which is the acute warning */
   /* here level 2 is reffered. this shall be updated to level 3 once DDG-2973 is resolved */
   if ((CED_ALERT_ACTIVE_LEVEL_2 <= alert_state) && (Fbk_Is_True(Ced_Is_Optical_Element_Acute_Warning_Enabled(p_ced_input, ced_door)))
       && ((Fbk_Is_False(f_ced_is_acute_warning_needs_to_be_suppressed))))
   {
      /* the acute warning needs to be called. All conditions are fulfilled */
      Ced_Set_Optical_Element_By_Type(p_ced_output, Ced_Get_Core_Alert_Direction(p_ced_core_output, ced_door), ced_door);
   }
   else if ((CED_ALERT_ACTIVE_LEVEL_2 <= alert_state) && Fbk_Is_True(Ced_Is_Optical_Element_Warning_Enabled(p_ced_input, ced_door)))
   {
      /* the warning needs to be called. All conditions are fulfilled */
      Ced_Set_Optical_Element_By_Type(p_ced_output, Ced_Get_Core_Alert_Direction(p_ced_core_output, ced_door), ced_door);
   }
   else if ((CED_ALERT_ACTIVE_LEVEL_1 <= alert_state)
            && Fbk_Is_True(Ced_Is_Optical_Element_Information_Enabled(p_ced_output, p_ced_input, ced_door)))
   {
      /* the inforamtion needs to be called. All conditions are fulfilled */
      Ced_Set_Optical_Element_By_Type(p_ced_output, Ced_Get_Core_Alert_Direction(p_ced_core_output, ced_door), ced_door);
   }
   else
   {
      /* inital reset for the mirror led warnings in case that soset of the warning level will happen */
      Ced_Control_Optical_Element_Reset(p_ced_output, ced_door);
   }
}

static boolean_T Ced_Is_Optical_Element_Acute_Warning_Enabled(const Ced_Input_T *p_ced_input, const Ced_Door_Position_T ced_door)
{
   boolean_T f_is_door_critical                         = FBK_FALSE;
   boolean_T f_is_acute_warning_active                  = FBK_FALSE;
   boolean_T f_is_optical_element_acute_warning_enabled = FBK_FALSE;

   /* an acute warning shall only be raised whenever the door is critical opened */
   if (Fbk_Is_True(Ced_Is_Door_Critical(p_ced_input, ced_door)))
   {
      f_is_door_critical = FBK_TRUE;
   }

   /* checks if the mirror output is enabled */
   if (Fbk_Is_True(p_ced_input->ced_coding_parameters.c_f_sfe_optical_display_activation))
   {
      f_is_acute_warning_active = FBK_TRUE;
   }

   /* verifies if both conditions are met */
   if (Fbk_Is_True(f_is_door_critical) && Fbk_Is_True(f_is_acute_warning_active))
   {
      f_is_optical_element_acute_warning_enabled = FBK_TRUE;
   }

   return f_is_optical_element_acute_warning_enabled;
}

static boolean_T Ced_Is_Optical_Element_Information_Enabled(const Ced_Output_T *p_ced_output,
                                                            const Ced_Input_T *p_ced_input,
                                                            const Ced_Door_Position_T ced_door)
{
   boolean_T f_is_seat_critical                       = FBK_FALSE;
   boolean_T f_is_information_enabled                 = FBK_FALSE;
   boolean_T f_is_optical_element_information_enabled = FBK_FALSE;

   /* an information shall only be raised if a occupant is not waring a seatbelt and might disengage the vehicle */
   if (Fbk_Is_True(Ced_Is_Occupant_Wearing_Seat_Belt(&p_ced_output->ced_output_occupant_detection, ced_door)))
   {
      f_is_seat_critical = FBK_TRUE;
   }

   /* checks if the mirror output is enabled */
   if (p_ced_input->ced_coding_parameters.c_f_sfe_optical_display_activation)
   {
      f_is_information_enabled = FBK_TRUE;
   }

   /* verifies if both conditions are met */
   if (Fbk_Is_True(f_is_seat_critical) && Fbk_Is_True(f_is_information_enabled))
   {
      f_is_optical_element_information_enabled = FBK_TRUE;
   }

   return f_is_optical_element_information_enabled;
}

static boolean_T Ced_Is_Optical_Element_Warning_Enabled(const Ced_Input_T *p_ced_input, const Ced_Door_Position_T ced_door)
{
   boolean_T f_is_door_critical                   = FBK_FALSE;
   boolean_T f_is_warning_enabled                 = FBK_FALSE;
   boolean_T f_is_optical_element_warning_enabled = FBK_FALSE;

   /* an acute warning shall only be raised whenever the door is critical opened */
   if (Fbk_Is_True(Ced_Is_Door_Critical(p_ced_input, ced_door)))
   {
      f_is_door_critical = FBK_TRUE;
   }

   /* checks if the mirror output is enabled */
   if (p_ced_input->ced_coding_parameters.c_f_sfe_optical_display_activation)
   {
      f_is_warning_enabled = FBK_TRUE;
   }

   /* verifies if both conditions are met */
   if (Fbk_Is_True(f_is_door_critical) && Fbk_Is_True(f_is_warning_enabled))
   {
      f_is_optical_element_warning_enabled = FBK_TRUE;
   }

   return f_is_optical_element_warning_enabled;
}

static void Ced_Set_Optical_Element_By_Type(Ced_Output_T *p_ced_output,
                                            const Ced_Target_Travel_Direction_T ced_travel_direction,
                                            const Ced_Door_Position_T ced_door)
{
   p_ced_output->ced_output_warning_indicators.ced_output_warning_optical[ced_door] = ced_travel_direction;
}
