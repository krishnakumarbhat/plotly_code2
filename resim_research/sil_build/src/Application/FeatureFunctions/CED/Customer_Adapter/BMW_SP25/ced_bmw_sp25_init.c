/**
 * @file ced_bmw_sp25_init.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the BMW_SP25 pre run initialization logic for CED.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_bmw_sp25_init.h"
#include "ced_bmw_sp25_types.h"
#include "fbk_macros.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

/**
 * @brief Initialization of the p_setting_safe_exit_sound struct of the BMW boardnet signals.
 *
 * @return filling p_setting_safe_exit_sound
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static void Ced_Init_Boardnet_Signals_Sound(Setting_Activity_T *p_setting_safe_exit_sound);

/**
 * @brief Initialization of the p_setting_safe_exit_delay_door struct of the BMW boardnet signals.
 *
 * @return filling p_setting_safe_exit_delay_door
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static void Ced_Init_Boardnet_Signals_Delay_Door(Setting_Activity_T *p_setting_safe_exit_delay_door);

/**
 * @brief Initialization of the p_setting_safe_exit struct of the BMW boardnet signals.
 *
 * @return filling p_setting_safe_exit
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static void Ced_Init_Boardnet_Signals_Setting(Setting_Safe_Exit_T *p_setting_safe_exit);

/**
 * @brief Initialization of the seat_status_for_each_seat array struct of the BMW boardnet signals.
 *
 * @return filling seat_status_for_each_seat
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static void Ced_Init_Boardnet_Signals_Seat_Status(Seat_Status_For_Each_Seat_T seat_status_for_each_seat[CED_BMW_NUMBER_OF_SEATS],
                                                  uint8_t size_of_array);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

void Ced_Init_Coding_Parameters(Ced_Coding_Parameters_T *p_ced_coding_parameters)
{
   p_ced_coding_parameters->c_f_sfe_doors_automatic_availability                    = FBK_FALSE;
   p_ced_coding_parameters->c_f_sfe_door_locks_electronic_controllable_availability = FBK_FALSE;
   p_ced_coding_parameters->c_f_sfe_function_activation                             = FBK_TRUE;
   p_ced_coding_parameters->c_f_sfe_function_enabled                                = FBK_FALSE;
   p_ced_coding_parameters->c_f_sfe_door_control_activation                         = FBK_FALSE;
   p_ced_coding_parameters->c_f_sfe_door_lock_control_activation                    = FBK_FALSE;
   p_ced_coding_parameters->c_f_sfe_door_actuator_activation                        = FBK_FALSE;
   p_ced_coding_parameters->c_f_sfe_mirror_light_activation                         = FBK_TRUE;
   p_ced_coding_parameters->c_sfe_mirror_light_information_type                     = CED_MIRROR_LIGHT_WARNING_ON_NO_FLASHING;
   p_ced_coding_parameters->c_sfe_mirror_light_warning_type                         = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_1;
   p_ced_coding_parameters->c_sfe_mirror_light_acute_warning_type                   = CED_MIRROR_LIGHT_WARNING_ON_FLASHING_2;
   p_ced_coding_parameters->c_f_sfe_interior_light_activation                       = FBK_TRUE;
   p_ced_coding_parameters->c_sfe_interior_light_type_information                   = CED_DOOR_WARNING_LEVEL_1;
   p_ced_coding_parameters->c_sfe_interior_light_type_warning                       = CED_DOOR_WARNING_LEVEL_2;
   p_ced_coding_parameters->c_sfe_interior_light_type_acute_warning                 = CED_DOOR_WARNING_LEVEL_2;
   p_ced_coding_parameters->c_f_sfe_sound_activation                                = FBK_TRUE;
   p_ced_coding_parameters->c_f_sfe_optical_display_activation                      = FBK_TRUE;
   p_ced_coding_parameters->c_f_sfe_object_minimum_relative_velocity                = 2.7778f;
   p_ced_coding_parameters->c_f_sfe_fault_monitoring_activation                     = FBK_FALSE;
   p_ced_coding_parameters->c_f_sfe_urgent_warning_during_automatic_door_opening_in_ready = FBK_FALSE;
   p_ced_coding_parameters->c_f_sfe_mirror_light_information_activation                   = FBK_TRUE;
   p_ced_coding_parameters->c_f_sfe_mirror_light_warning_activation                       = FBK_TRUE;
   p_ced_coding_parameters->c_f_sfe_mirror_light_acute_warning_activation                 = FBK_TRUE;
}

void Ced_Init_Input_Bus_Signals(Bmw_Ced_Input_Bus_Signals_T *p_bmw_ced_input_bus_signals)
{

   p_bmw_ced_input_bus_signals->f_ced_function_activation  = FBK_FALSE;
   p_bmw_ced_input_bus_signals->f_ced_function_fault_state = FBK_FALSE;
   p_bmw_ced_input_bus_signals->f_ced_function_degraded    = FBK_FALSE;

   p_bmw_ced_input_bus_signals->f_ced_parental_controls_driver_rear    = FBK_FALSE;
   p_bmw_ced_input_bus_signals->f_ced_parental_controls_passenger_rear = FBK_FALSE;

   p_bmw_ced_input_bus_signals->f_ced_vehicle_door_lock_rear_left  = FBK_FALSE;
   p_bmw_ced_input_bus_signals->f_ced_vehicle_door_lock_rear_right = FBK_FALSE;

   p_bmw_ced_input_bus_signals->f_ced_vehicle_door_lock_request_rear_left  = FBK_FALSE;
   p_bmw_ced_input_bus_signals->f_ced_vehicle_door_lock_request_rear_right = FBK_FALSE;

   p_bmw_ced_input_bus_signals->ced_status_roller_dynamometer = BMW_CED_NO_DYNAMOMETER;
   p_bmw_ced_input_bus_signals->ced_status_end_of_line        = BMW_CED_END_OF_LINE_MODE_NOT_SET;
}

void Ced_Init_Boardnet_Signals(Ced_Bmw_Boardnet_T *p_bmw_boardnet_signals)
{
   p_bmw_boardnet_signals->ego_displayed_speed = FBK_ZERO_F;
   Ced_Init_Boardnet_Signals_Sound(&p_bmw_boardnet_signals->setting_safe_exit_sound);
   Ced_Init_Boardnet_Signals_Delay_Door(&p_bmw_boardnet_signals->setting_safe_exit_delay_door);
   Ced_Init_Boardnet_Signals_Setting(&p_bmw_boardnet_signals->setting_safe_exit);
   Ced_Init_Boardnet_Signals_Seat_Status(p_bmw_boardnet_signals->seat_status_for_each_seat, CED_BMW_NUMBER_OF_SEATS);
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Ced_Init_Boardnet_Signals_Sound(Setting_Activity_T *p_setting_safe_exit_sound)
{
   p_setting_safe_exit_sound->value        = SETTING_ACTIVITY_VALUE_ACTIVATED;
   p_setting_safe_exit_sound->availability = SETTING_DRV_ASS_FCT_AVAILABILITY_OPERABLE;
}

static void Ced_Init_Boardnet_Signals_Delay_Door(Setting_Activity_T *p_setting_safe_exit_delay_door)
{
   p_setting_safe_exit_delay_door->value        = SETTING_ACTIVITY_VALUE_ACTIVATED;
   p_setting_safe_exit_delay_door->availability = SETTING_DRV_ASS_FCT_AVAILABILITY_OPERABLE;
}

static void Ced_Init_Boardnet_Signals_Setting(Setting_Safe_Exit_T *p_setting_safe_exit)
{
   p_setting_safe_exit->value                    = SETTING_SAFE_EXIT_VALUE_ACTIVATED;
   p_setting_safe_exit->availability             = SETTING_DRV_ASS_FCT_AVAILABILITY_OPERABLE;
   p_setting_safe_exit->availability_deactivated = SETTING_DRV_ASS_FCT_AVAILABILITY_OPERABLE;
}

static void Ced_Init_Boardnet_Signals_Seat_Status(Seat_Status_For_Each_Seat_T seat_status_for_each_seat[CED_BMW_NUMBER_OF_SEATS],
                                                  uint8_t size_of_array)
{
   uint8_t i;
   const Absolute_Seat_Location_T absolute_seat_location[CED_BMW_NUMBER_OF_SEATS] = {
      ABSOLUTE_SEAT_LOCATION_SEAT_LEFT_ROW1, ABSOLUTE_SEAT_LOCATION_SEAT_MID_ROW1, ABSOLUTE_SEAT_LOCATION_SEAT_RIGHT_ROW1,
      ABSOLUTE_SEAT_LOCATION_SEAT_LEFT_ROW2, ABSOLUTE_SEAT_LOCATION_SEAT_MID_ROW2, ABSOLUTE_SEAT_LOCATION_SEAT_RIGHT_ROW2,
      ABSOLUTE_SEAT_LOCATION_SEAT_LEFT_ROW3, ABSOLUTE_SEAT_LOCATION_SEAT_MID_ROW3, ABSOLUTE_SEAT_LOCATION_SEAT_RIGHT_ROW3};
   const Relative_Seat_Location_T relative_seat_location[CED_BMW_NUMBER_OF_SEATS] = {
      RELATIVE_SEAT_LOCATION_DRIVER_SEAT_ROW1,
      RELATIVE_SEAT_LOCATION_PASSENGER_MID_SEAT_ROW1,
      RELATIVE_SEAT_LOCATION_PASSENGER_SEAT_ROW1,
      RELATIVE_SEAT_LOCATION_DRIVER_REAR_SEAT_ROW2,
      RELATIVE_SEAT_LOCATION_PASSENGER_MID_REAR_SEAT_ROW2,
      RELATIVE_SEAT_LOCATION_PASSENGER_REAR_SEAT_ROW2,
      RELATIVE_SEAT_LOCATION_DRIVER_REAR_SEAT_ROW3,
      RELATIVE_SEAT_LOCATION_PASSENGER_MID_REAR_SEAT_ROW3,
      RELATIVE_SEAT_LOCATION_PASSENGER_REAR_SEAT_ROW3};

   /* looping over all array indices */
   for (i = FBK_ZERO_UINT; i < size_of_array; i++)
   {
      /* catching a possible error which occures if the size_of_array is larger than CED_BMW_NUMBER_OF_SEATS */
      if (i < CED_BMW_NUMBER_OF_SEATS)
      {
         seat_status_for_each_seat[i].seat_location.absolute_seat_location = absolute_seat_location[i];
         seat_status_for_each_seat[i].seat_location.relative_seat_location = relative_seat_location[i];
      }
      else
      {
         /* if the size_of_array is larger than CED_BMW_NUMBER_OF_SEATS only array position 0 is filled */
         seat_status_for_each_seat[i].seat_location.absolute_seat_location = absolute_seat_location[FBK_ZERO_UINT];
         seat_status_for_each_seat[i].seat_location.relative_seat_location = relative_seat_location[FBK_ZERO_UINT];
      }

      seat_status_for_each_seat[i].status_belt.status_belt_buckle_switch                 = STATUS_BELT_BUCKLE_SWITCH_NOT_BUCKLED;
      seat_status_for_each_seat[i].status_belt.qualifier_value_status_belt_buckle_switch = QUALIFIER_VALUE_OK;
      seat_status_for_each_seat[i].seat_occupancy_status.person_age_category             = PERSON_AGE_CATEGORY_ADULT;
      seat_status_for_each_seat[i].seat_occupancy_status.person_entry_exit               = PERSON_ENTRY_EXIT_EXIT_POSSIBLE;
      seat_status_for_each_seat[i].seat_occupancy_status.qualifier_container_seat_occupancy_status = QUALIFIER_CONTAINER_OK;
      seat_status_for_each_seat[i].seat_occupancy_status.seat_occupancy = SEAT_OCCUPANCY_SEAT_OCCUPIED_PERSON;
   }
}
