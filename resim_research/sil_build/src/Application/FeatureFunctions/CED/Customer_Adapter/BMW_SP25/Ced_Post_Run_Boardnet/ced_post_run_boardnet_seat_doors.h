#ifndef CED_POST_RUN_BOARDNET_SEAT_DOORS_H
#define CED_POST_RUN_BOARDNET_SEAT_DOORS_H

/**
 * @file ced_post_run_boardnet_seat_doors.h
 * @author SFL (Side Feature Logic) scrum team
 * @brief Contains the output adaptations for the boardnet signals.
 *
 * @copyright Copyright (C) 2022 Aptiv. All rights reserved.
 */

/*===========================================================================*\
* Includes
\*===========================================================================*/

#include "ced_bmw_sp25_types.h"
#include "ced_core_output_t.h"
#include "ced_input_t.h"
#include "ced_output_t.h"
#include "ced_types.h"
#include "pa_reuse.h"

/*===========================================================================*\
* External Function Prototypes
\*===========================================================================*/

/**
 * @brief Returns the travel direction of the warning for a given door position.
 *
 * @return Travel direction from core output
 *
 * @SRS{n/a}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
Ced_Target_Travel_Direction_T Ced_Get_Core_Alert_Direction(const Ced_Core_Output_T *p_ced_core_output,
                                                           const Ced_Door_Position_T ced_door_position);

/**
 * @brief Returns the Core alert state from the core output for a given door position.
 *
 * @return Core Alert state from core output
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
Ced_Alert_T Ced_Get_Core_Alert_State(const Ced_Core_Output_T *p_ced_core_output, const Ced_Door_Position_T ced_door_position);

/**
 * @brief Converting a side to a front door position.
 *
 * @return Ced_Door_Position_T corresponding to the input side
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
Ced_Door_Position_T Ced_Get_Door_From_Side(const Ced_Sides_T ced_sides);

/**
 * @brief Returns if the door setting is critical.
 *
 * @return FBK_TRUE = door is critical
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
boolean_T Ced_Is_Door_Critical(const Ced_Input_T *p_ced_input, const Ced_Door_Position_T ced_door_position);

/**
 * @brief Fills the output with the inforamtion about is there is a occupant who might leave the ego vehicle for a single seat next
 * to a door.
 *
 * @return filling p_output_occupant_detection
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
boolean_T Ced_Is_Occupant_Not_Waring_Seatbelt_Door(const Ced_Bmw_Boardnet_T *p_bmw_boardnet,
                                                   const Ced_Door_Position_T ced_door_position);

/**
 * @brief Checks whether an occupant is waring a seatbelt and if an occupant is present.
 *
 * @return FBK_TRUE = an occupant is there who does not wear a seatbelt
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
boolean_T Ced_Is_Occupant_Wearing_Seat_Belt(const Ced_Output_Occupant_Detection_T *ced_output_occupant_detection,
                                            const Ced_Door_Position_T ced_door_position);

/**
 * @brief Returns an index for the boardnet seat array based on the selected door.
 *
 * @return uint8_t array index
 *
 * @SRS{}
 * @SAE{}
 * @SDD{n/a}
 * @verification{}
 */
uint8_t Ced_Get_Index_From_Door(const Seat_Status_For_Each_Seat_T seat_status_for_each_seat[CED_BMW_NUMBER_OF_SEATS],
                                const Ced_Door_Position_T ced_door_position);


#endif /* CED_POST_RUN_BOARDNET_H */
