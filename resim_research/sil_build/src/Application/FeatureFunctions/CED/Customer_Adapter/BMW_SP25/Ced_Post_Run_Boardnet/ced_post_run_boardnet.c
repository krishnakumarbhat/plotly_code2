/**
 * @file ced_post_run_boardnet.c
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the output adaptations for the boardnet signals.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_post_run_boardnet.h"
#include "ced_bmw_sp25_types.h"
#include "ced_post_run_boardnet_seat_doors.h"
#include "pa_reuse.h"

/*===========================================================================*\
* Local Function Prototypes
\*===========================================================================*/

/**
 * @brief Sets p_output_occupant_detection depending of the seat location with the given boolean f_occupant_present.
 *
 * @return void
 *
 * @SRS{n/a}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static void Ced_Set_Occupant_Detection_Occupant_Not_Wearing_SeatBelt(Ced_Output_Occupant_Detection_T *p_output_occupant_detection,
                                                                     boolean_T f_occupant_present,
                                                                     const Absolute_Seat_Location_T *p_absolute_seat_location);

/**
 * @brief Checks if a passenger might disengage the vehicle.
 *
 * @return writes the value in p_output_occupant_detection
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
static void Ced_Check_And_Set_Occupant_Detection_Door(Ced_Output_Occupant_Detection_T *p_output_occupant_detection,
                                                      const Ced_Bmw_Boardnet_T *p_bmw_boardnet,
                                                      const Ced_Door_Position_T ced_door_position);

/*===========================================================================*\
* Global Function Definitions
\*===========================================================================*/

void Ced_Check_And_Set_Occupant_Detection(Ced_Output_Occupant_Detection_T *p_output_occupant_detection,
                                          const Ced_Bmw_Boardnet_T *p_bmw_boardnet)
{
   /* checks and sets the seat occupancy for each and every door */
   Ced_Check_And_Set_Occupant_Detection_Door(p_output_occupant_detection, p_bmw_boardnet, CED_DOOR_POSITION_FRONT_LEFT);
   Ced_Check_And_Set_Occupant_Detection_Door(p_output_occupant_detection, p_bmw_boardnet, CED_DOOR_POSITION_FRONT_RIGHT);
   Ced_Check_And_Set_Occupant_Detection_Door(p_output_occupant_detection, p_bmw_boardnet, CED_DOOR_POSITION_REAR_LEFT);
   Ced_Check_And_Set_Occupant_Detection_Door(p_output_occupant_detection, p_bmw_boardnet, CED_DOOR_POSITION_REAR_RIGHT);
}

/*===========================================================================*\
* Local Function Definitions
\*===========================================================================*/

static void Ced_Check_And_Set_Occupant_Detection_Door(Ced_Output_Occupant_Detection_T *p_output_occupant_detection,
                                                      const Ced_Bmw_Boardnet_T *p_bmw_boardnet,
                                                      const Ced_Door_Position_T ced_door_position)
{
   boolean_T f_occupant_not_wearing_seatbelt;
   uint8_t absolute_seat_location_index = Ced_Get_Index_From_Door(p_bmw_boardnet->seat_status_for_each_seat, ced_door_position);

   /* is the seat location relevant for the warning */
   f_occupant_not_wearing_seatbelt = Ced_Is_Occupant_Not_Waring_Seatbelt_Door(p_bmw_boardnet, ced_door_position);

   Ced_Set_Occupant_Detection_Occupant_Not_Wearing_SeatBelt(
      p_output_occupant_detection, f_occupant_not_wearing_seatbelt,
      &p_bmw_boardnet->seat_status_for_each_seat[absolute_seat_location_index].seat_location.absolute_seat_location);
}

static void Ced_Set_Occupant_Detection_Occupant_Not_Wearing_SeatBelt(Ced_Output_Occupant_Detection_T *p_output_occupant_detection,
                                                                     boolean_T f_occupant_present,
                                                                     const Absolute_Seat_Location_T *p_absolute_seat_location)
{
   switch (*p_absolute_seat_location)
   {
      case ABSOLUTE_SEAT_LOCATION_SEAT_LEFT_ROW1:
      {
         /* indicating that there is a occupant on the front left seat who might exit the vehicle */
         p_output_occupant_detection->f_occupant_left_front_not_wearing_seatbelt = f_occupant_present;
         break;
      }
      case ABSOLUTE_SEAT_LOCATION_SEAT_LEFT_ROW2:
      {
         /* indicating that there is a occupant on the rear left seat who might exit the vehicle */
         p_output_occupant_detection->f_occupant_left_rear_not_wearing_seatbelt = f_occupant_present;
         break;
      }
      case ABSOLUTE_SEAT_LOCATION_SEAT_RIGHT_ROW1:
      {
         /* indicating that there is a occupant on the front right seat who might exit the vehicle */
         p_output_occupant_detection->f_occupant_right_front_not_wearing_seatbelt = f_occupant_present;
         break;
      }
      case ABSOLUTE_SEAT_LOCATION_SEAT_RIGHT_ROW2:
      {
         /* indicating that there is a occupant on the rear right seat who might exit the vehicle */
         p_output_occupant_detection->f_occupant_right_rear_not_wearing_seatbelt = f_occupant_present;
         break;
      }
      default:
      {
         /* This should not happen. Do nothing in this case. */
         break;
      }
   }
}
