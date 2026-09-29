/**
 * @file ced_post_run_boardnet_seat_doors.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the output adaptations for the boardnet signals.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_post_run_boardnet_seat_doors.h"
#include "ced_types.h"
#include "fbk_macros.h"

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

/**
 * @brief Returns the FKB Side definitions dependent on the given door.
 *
 * @return value of FBK side definitions
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static uint8_t Ced_Get_Fbk_Side_From_Door_Position(const Ced_Door_Position_T ced_door_position);

/**
 * @brief Checks if the door where the seat is located is relevant for the safe exit warning.
 *
 * @return true if the seat position is relevant
 *
 * @SRS{n/a}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static boolean_T Ced_Is_Seat_Relevant(const Seat_Status_For_Each_Seat_T *p_seat_status_for_each_seat);

/**
 * @brief Checks the qualifiers from the BMW boardnet and indicated whether a necessary qualifier is not valid.
 *
 * @return boolean: TRUE = the qualifiers are valid
 *
 * @SRS{n/a}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static boolean_T Ced_Is_Occupant_Detection_Qualifier_Valid(const Seat_Status_For_Each_Seat_T *p_seat_status_for_each_seat);

/**
 * @brief Checks the conditions are fulfilled for an occupant to leave the vehicle.
 *
 * @return true if an occupant might disengage
 *
 * @SRS{n/a}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static boolean_T Ced_Is_Occupant_Exit_Possible(const Seat_Status_For_Each_Seat_T *p_seat_status_for_each_seat);

/**
 * @brief Returns an index for the boardnet seat array based on the selected door.
 *
 * @return Absolute_Seat_Location_T seat location from door
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static Absolute_Seat_Location_T Ced_Get_Absolute_Seat_Location_From_Door_Position(const Ced_Door_Position_T ced_door_position);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

Ced_Target_Travel_Direction_T Ced_Get_Core_Alert_Direction(const Ced_Core_Output_T *p_ced_core_output,
                                                           const Ced_Door_Position_T ced_door_position)
{
   uint8_t fbk_side;
   Ced_Target_Travel_Direction_T ced_target_travel_direction;

   fbk_side = Ced_Get_Fbk_Side_From_Door_Position(ced_door_position);

   if (FBK_SIDE_UNDEFINED == fbk_side)
   {
      ced_target_travel_direction = UNDEF_DIRECTION;
   }
   else
   {
      switch (p_ced_core_output->ced_object_direction[fbk_side])
      {
         case FBK_SIDE_FRONT:
         {
            ced_target_travel_direction = FRONT_DIRECTION;
            break;
         }
         case FBK_SIDE_REAR:
         {
            ced_target_travel_direction = REAR_DIRECTION;
            break;
         }
         default:
         {
            ced_target_travel_direction = UNDEF_DIRECTION;
            break;
         }
      }
   }
   return ced_target_travel_direction;
}

Ced_Alert_T Ced_Get_Core_Alert_State(const Ced_Core_Output_T *p_ced_core_output, const Ced_Door_Position_T ced_door_position)
{
   Ced_Alert_T alert_level;
   uint8_t side_of_door = Ced_Get_Fbk_Side_From_Door_Position(ced_door_position);
   alert_level          = (FBK_SIDE_UNDEFINED == side_of_door) ? CED_NO_ALERT : p_ced_core_output->ced_alert[side_of_door];

   return alert_level;
}

Ced_Door_Position_T Ced_Get_Door_From_Side(const Ced_Sides_T ced_sides)
{
   Ced_Door_Position_T ced_door_position;
   if (CED_SIDE_LEFT == ced_sides)
   {
      ced_door_position = CED_DOOR_POSITION_FRONT_LEFT;
   }
   else
   {
      ced_door_position = CED_DOOR_POSITION_FRONT_RIGHT;
   }

   return ced_door_position;
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static uint8_t Ced_Get_Fbk_Side_From_Door_Position(const Ced_Door_Position_T ced_door_position)
{
   uint8_t fbk_side;
   /* checks the door position */
   switch (ced_door_position)
   {
      case CED_DOOR_POSITION_FRONT_LEFT:
      {
         /* maps front left to left */
         fbk_side = (uint8_t) FBK_SIDE_LEFT;
         break;
      }
      case CED_DOOR_POSITION_FRONT_RIGHT:
      {
         /* maps front right to right */
         fbk_side = (uint8_t) FBK_SIDE_RIGHT;
         break;
      }
      case CED_DOOR_POSITION_REAR_LEFT:
      {
         /* maps rear left to left */
         fbk_side = (uint8_t) FBK_SIDE_LEFT;
         break;
      }
      case CED_DOOR_POSITION_REAR_RIGHT:
      {
         /* maps rear right to right */
         fbk_side = (uint8_t) FBK_SIDE_RIGHT;
         break;
      }
      default:
      {
         /* maps any other case to undefined */
         fbk_side = (uint8_t) FBK_SIDE_UNDEFINED;
         break;
      }
   }

   return fbk_side; /*  returning */
}

boolean_T Ced_Is_Door_Critical(const Ced_Input_T *p_ced_input, const Ced_Door_Position_T ced_door_position)
{
   boolean_T f_is_door_critical = FBK_TRUE;
   /* for DDG-2974 add the door conditions here which are for now missing in the boardnet signals */
   if ((p_ced_input->f_ced_enable)
       && ((ced_door_position == CED_DOOR_POSITION_FRONT_LEFT) || (ced_door_position == CED_DOOR_POSITION_FRONT_RIGHT)))
   {
      f_is_door_critical = FBK_TRUE;
   }

   return f_is_door_critical;
}

static boolean_T Ced_Is_Occupant_Detection_Qualifier_Valid(const Seat_Status_For_Each_Seat_T *p_seat_status_for_each_seat)
{
   boolean_T f_qualifier_no_error_input = FBK_FALSE;

   /* checking for all relevant qualifiers to be valid */
   if ((QUALIFIER_CONTAINER_OK == p_seat_status_for_each_seat->seat_occupancy_status.qualifier_container_seat_occupancy_status)
       && (SEAT_OCCUPANCY_ERROR != p_seat_status_for_each_seat->seat_occupancy_status.seat_occupancy)
       && (QUALIFIER_VALUE_OK == p_seat_status_for_each_seat->status_belt.qualifier_value_status_belt_buckle_switch))
   {
      f_qualifier_no_error_input = FBK_TRUE; /* setting the ouput to true as all qualifiers are valid */
   }

   return f_qualifier_no_error_input;
}

static boolean_T Ced_Is_Occupant_Exit_Possible(const Seat_Status_For_Each_Seat_T *p_seat_status_for_each_seat)
{
   boolean_T f_exit_possible = FBK_FALSE; /**< variable used to provide the output of this function */

   /* is the seat occupied with a person and is the seat belt bluckled? */
   if ((SEAT_OCCUPANCY_SEAT_OCCUPIED_PERSON == p_seat_status_for_each_seat->seat_occupancy_status.seat_occupancy)
       && (STATUS_BELT_BUCKLE_SWITCH_NOT_BUCKLED == p_seat_status_for_each_seat->status_belt.status_belt_buckle_switch))
   {
      /* setting the ouput to true as there is a person who might disengage the vehicle */
      f_exit_possible = FBK_TRUE;
   }

   return f_exit_possible;
}

boolean_T Ced_Is_Occupant_Not_Waring_Seatbelt_Door(const Ced_Bmw_Boardnet_T *p_bmw_boardnet, const Ced_Door_Position_T ced_door_position)
{
   boolean_T f_occupant_not_wearing_seatbelt = FBK_FALSE;

   uint8_t absolute_seat_location_index = Ced_Get_Index_From_Door(p_bmw_boardnet->seat_status_for_each_seat, ced_door_position);

   /* determining if the passenger is in the car and might leave it at a given door */
   if (Ced_Is_Seat_Relevant(&p_bmw_boardnet->seat_status_for_each_seat[absolute_seat_location_index])
       && Ced_Is_Occupant_Detection_Qualifier_Valid(&p_bmw_boardnet->seat_status_for_each_seat[absolute_seat_location_index])
       && Ced_Is_Occupant_Exit_Possible(&p_bmw_boardnet->seat_status_for_each_seat[absolute_seat_location_index]))
   {
      /* the passenger might leave the vehicle */
      f_occupant_not_wearing_seatbelt = FBK_TRUE;
   }

   return f_occupant_not_wearing_seatbelt;
}

uint8_t Ced_Get_Index_From_Door(const Seat_Status_For_Each_Seat_T seat_status_for_each_seat[CED_BMW_NUMBER_OF_SEATS],
                                const Ced_Door_Position_T ced_door_position)
{
   uint8_t i;
   const Absolute_Seat_Location_T absolute_seat_location = Ced_Get_Absolute_Seat_Location_From_Door_Position(ced_door_position);
   uint8_t index                                         = FBK_ZERO_UINT;

   for (i = FBK_ZERO_UINT; i < CED_BMW_NUMBER_OF_SEATS; i++)
   {
      if (seat_status_for_each_seat[i].seat_location.absolute_seat_location == absolute_seat_location)
      {
         index = i;
      }
   }
   return index;
}

static Absolute_Seat_Location_T Ced_Get_Absolute_Seat_Location_From_Door_Position(const Ced_Door_Position_T ced_door_position)
{
   Absolute_Seat_Location_T absolute_seat_location;

   switch (ced_door_position)
   {
      case CED_DOOR_POSITION_FRONT_LEFT:
      {
         absolute_seat_location = ABSOLUTE_SEAT_LOCATION_SEAT_LEFT_ROW1;
         break;
      }
      case CED_DOOR_POSITION_FRONT_RIGHT:
      {
         absolute_seat_location = ABSOLUTE_SEAT_LOCATION_SEAT_RIGHT_ROW1;
         break;
      }
      case CED_DOOR_POSITION_REAR_LEFT:
      {
         absolute_seat_location = ABSOLUTE_SEAT_LOCATION_SEAT_LEFT_ROW2;
         break;
      }
      case CED_DOOR_POSITION_REAR_RIGHT:
      {
         absolute_seat_location = ABSOLUTE_SEAT_LOCATION_SEAT_RIGHT_ROW2;
         break;
      }
      default:
      {
         absolute_seat_location = ABSOLUTE_SEAT_LOCATION_UNKNOWN;
         break;
      }
   }
   return absolute_seat_location;
}

boolean_T Ced_Is_Occupant_Wearing_Seat_Belt(const Ced_Output_Occupant_Detection_T *ced_output_occupant_detection,
                                            const Ced_Door_Position_T ced_door_position)
{
   boolean_T f_occupant_wearing_seat_belt;

   /* checking which door position was the input */
   switch (ced_door_position)
   {
      case CED_DOOR_POSITION_FRONT_LEFT:
      {
         /* getting the data from left front position */
         f_occupant_wearing_seat_belt = ced_output_occupant_detection->f_occupant_left_front_not_wearing_seatbelt;
         break;
      }
      case CED_DOOR_POSITION_FRONT_RIGHT:
      {
         /* getting the data from right front position */
         f_occupant_wearing_seat_belt = ced_output_occupant_detection->f_occupant_right_front_not_wearing_seatbelt;
         break;
      }
      case CED_DOOR_POSITION_REAR_LEFT:
      {
         /* getting the data from left rear position */
         f_occupant_wearing_seat_belt = ced_output_occupant_detection->f_occupant_left_rear_not_wearing_seatbelt;
         break;
      }
      case CED_DOOR_POSITION_REAR_RIGHT:
      {
         /* getting the data from right rear position */
         f_occupant_wearing_seat_belt = ced_output_occupant_detection->f_occupant_right_rear_not_wearing_seatbelt;
         break;
      }
      default:
      {
         /* default case. Should not be called actually... */
         f_occupant_wearing_seat_belt = FBK_FALSE;
         break;
      }
   }
   return f_occupant_wearing_seat_belt;
}

static boolean_T Ced_Is_Seat_Relevant(const Seat_Status_For_Each_Seat_T *p_seat_status_for_each_seat)
{
   boolean_T f_seat_relevant = FBK_FALSE;

   /* is the seat next to any door? The assumption is that the vehcile got four doors */
   if ((ABSOLUTE_SEAT_LOCATION_SEAT_LEFT_ROW1 == p_seat_status_for_each_seat->seat_location.absolute_seat_location)
       || (ABSOLUTE_SEAT_LOCATION_SEAT_LEFT_ROW2 == p_seat_status_for_each_seat->seat_location.absolute_seat_location)
       || (ABSOLUTE_SEAT_LOCATION_SEAT_RIGHT_ROW1 == p_seat_status_for_each_seat->seat_location.absolute_seat_location)
       || (ABSOLUTE_SEAT_LOCATION_SEAT_RIGHT_ROW2 == p_seat_status_for_each_seat->seat_location.absolute_seat_location))
   {
      /* setting the ouput to true as the seat is relevant for a warning */
      f_seat_relevant = FBK_TRUE;
   }

   return f_seat_relevant;
}
