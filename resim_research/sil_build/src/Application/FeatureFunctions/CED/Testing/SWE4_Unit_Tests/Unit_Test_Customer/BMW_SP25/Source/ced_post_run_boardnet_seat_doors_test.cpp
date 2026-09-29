/**
 * @file ced_post_run_boardnet_seat_doors_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for BMW SP25 boardnet seat doors module
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-112852}
 */

#include "ced_post_run_boardnet_seat_doors_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "ced_bmw_sp25_init.h"
#include "ced_bmw_sp25_types.h"
#include "ced_post_run_boardnet_seat_doors.c"
#include "ced_types.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "pt_output_t.h"
}

/**
 * Check that qualifiers is not valid due to errors in all signals.
 * \uts{CSCSA-112853} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Seat_Doors_Test, Ced_Is_Occupant_Detection_Qualifier_Valid__all_qualifiers_set_as_error)
{
   /** \arrange setup typical data */
   uint8_t i = 0;
   for (i = FBK_ZERO_UINT; i < CED_BMW_NUMBER_OF_SEATS; i++)
   {
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].seat_occupancy_status.qualifier_container_seat_occupancy_status =
         QUALIFIER_CONTAINER_ERROR;
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].seat_occupancy_status.seat_occupancy = SEAT_OCCUPANCY_ERROR;
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].status_belt.qualifier_value_status_belt_buckle_switch =
         QUALIFIER_VALUE_ERROR;
   }
   boolean_T result;

   /** \action check if qualifiers are valid */
   result = Ced_Is_Occupant_Detection_Qualifier_Valid(ced_input.bmw_boardnet_signals.seat_status_for_each_seat);

   /** \assert expect not valid qualifiers */
   EXPECT_FALSE(result);
}


/**
 * Check that qualifiers is not valid due to errors in status seat occupandy and belt buckle switch.
 * \uts{CSCSA-112854} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Seat_Doors_Test, Ced_Is_Occupant_Detection_Qualifier_Valid__seat_occupancy_and_belt_buckle_set_as_error)
{
   /** \arrange setup typical data */
   uint8_t i = 0;
   for (i = FBK_ZERO_UINT; i < CED_BMW_NUMBER_OF_SEATS; i++)
   {
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].seat_occupancy_status.qualifier_container_seat_occupancy_status =
         QUALIFIER_CONTAINER_OK;
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].seat_occupancy_status.seat_occupancy = SEAT_OCCUPANCY_ERROR;
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].status_belt.qualifier_value_status_belt_buckle_switch =
         QUALIFIER_VALUE_ERROR;
   }
   boolean_T result;

   /** \action check if qualifiers are valid */
   result = Ced_Is_Occupant_Detection_Qualifier_Valid(ced_input.bmw_boardnet_signals.seat_status_for_each_seat);

   /** \assert expect not valid qualifiers */
   EXPECT_FALSE(result);
}

/**
 * Check that qualifiers is not valid due to belt buckle switch status error.
 * \uts{CSCSA-112855} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Seat_Doors_Test, Ced_Is_Occupant_Detection_Qualifier_Valid__qualifiers_set_as_error)
{
   /** \arrange setup typical data */
   uint8_t i = 0;
   for (i = FBK_ZERO_UINT; i < CED_BMW_NUMBER_OF_SEATS; i++)
   {
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].seat_occupancy_status.qualifier_container_seat_occupancy_status =
         QUALIFIER_CONTAINER_OK;
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].seat_occupancy_status.seat_occupancy = SEAT_OCCUPANCY_ANIMAL;
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].status_belt.qualifier_value_status_belt_buckle_switch =
         QUALIFIER_VALUE_ERROR;
   }
   boolean_T result;

   /** \action check if qualifiers are valid */
   result = Ced_Is_Occupant_Detection_Qualifier_Valid(ced_input.bmw_boardnet_signals.seat_status_for_each_seat);

   /** \assert expect not valid qualifiers */
   EXPECT_FALSE(result);
}


/**
 * Check thatcondition to leave vehicle is not valid due to seat occupied with an unknown object.
 * \uts{CSCSA-112856} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Seat_Doors_Test, Ced_Is_Occupant_Exit_Possible__unknown_object_occupied_seat)
{
   /** \arrange setup unkown (ufo) occupancy on all seats */
   uint8_t i = 0;
   for (i = FBK_ZERO_UINT; i < CED_BMW_NUMBER_OF_SEATS; i++)
   {
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].seat_occupancy_status.seat_occupancy = SEAT_OCCUPANCY_UNKNOWN;
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].status_belt.status_belt_buckle_switch =
         STATUS_BELT_BUCKLE_SWITCH_NOT_EQUIPPED;
   }
   boolean_T result;

   /** \action check occupant to leave the vehicle */
   result = Ced_Is_Occupant_Exit_Possible(ced_input.bmw_boardnet_signals.seat_status_for_each_seat);

   /** \assert expect not valid qualifiers */
   EXPECT_FALSE(result);
}

/**
 * Check that condition to leave vehicle is not valid due to not seat belt equipped.
 * \uts{CSCSA-112857} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Seat_Doors_Test, Ced_Is_Occupant_Exit_Possible__seat_belt_are_not_equipped)
{
   /** \arrange setup not seat belt equipped */
   uint8_t i = 0;
   for (i = FBK_ZERO_UINT; i < CED_BMW_NUMBER_OF_SEATS; i++)
   {
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].seat_occupancy_status.seat_occupancy =
         SEAT_OCCUPANCY_SEAT_OCCUPIED_PERSON;
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].status_belt.status_belt_buckle_switch =
         STATUS_BELT_BUCKLE_SWITCH_NOT_EQUIPPED;
   }
   boolean_T result;

   /** \action check occupant to leave the vehicle */
   result = Ced_Is_Occupant_Exit_Possible(ced_input.bmw_boardnet_signals.seat_status_for_each_seat);

   /** \assert expect not valid qualifiers */
   EXPECT_FALSE(result);
}

/**
 * Check that there is a occupant who might leave the ego vehicle for a single seat next to a door.
 * \uts{CSCSA-112858} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Seat_Doors_Test, Ced_Is_Occupant_Not_Waring_Seatbelt_Door__general_test)
{
   /** \arrange setup general properites */
   uint8_t i                             = 0;
   Ced_Door_Position_T ced_door_position = CED_DOOR_POSITION_REAR_LEFT;
   for (i = FBK_ZERO_UINT; i < CED_BMW_NUMBER_OF_SEATS; i++)
   {
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].seat_location.absolute_seat_location =
         ABSOLUTE_SEAT_LOCATION_SEAT_LEFT_ROW2;
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].status_belt.qualifier_value_status_belt_buckle_switch =
         QUALIFIER_VALUE_OK;
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].seat_occupancy_status.qualifier_container_seat_occupancy_status =
         QUALIFIER_CONTAINER_OK;
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].seat_occupancy_status.seat_occupancy =
         SEAT_OCCUPANCY_SEAT_OCCUPIED_PERSON;
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].status_belt.status_belt_buckle_switch =
         STATUS_BELT_BUCKLE_SWITCH_NOT_BUCKLED;
   }
   boolean_T result;

   /** \action check occupant to leave the vehicle */
   result = Ced_Is_Occupant_Not_Waring_Seatbelt_Door(&ced_input.bmw_boardnet_signals, ced_door_position);

   /** \assert expect seatbelt */
   EXPECT_TRUE(result);
}


/**
 * Check that there is a occupant who might leave the ego vehicle for a single seat next to a door. Set uknonwn location and not
 * equipped in belt. \uts{CSCSA-112859} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Seat_Doors_Test, Ced_Is_Occupant_Not_Waring_Seatbelt_Door__unknown_seat_location_belt_not_equipped)
{
   /** \arrange setup unknown absolute seat location */
   uint8_t i                             = 0;
   Ced_Door_Position_T ced_door_position = CED_DOOR_POSITION_FRONT_LEFT;
   for (i = FBK_ZERO_UINT; i < CED_BMW_NUMBER_OF_SEATS; i++)
   {
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].seat_location.absolute_seat_location = ABSOLUTE_SEAT_LOCATION_UNKNOWN;
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].status_belt.status_belt_buckle_switch =
         STATUS_BELT_BUCKLE_SWITCH_NOT_EQUIPPED;
   }
   boolean_T result;

   /** \action check occupant to leave the vehicle */
   result = Ced_Is_Occupant_Not_Waring_Seatbelt_Door(&ced_input.bmw_boardnet_signals, ced_door_position);

   /** \assert expect seatbelt */
   EXPECT_FALSE(result);
}

/**
 * Check that there is a occupant who might leave the ego vehicle for a single seat next to a door. Correct seat but belt not
 * available. \uts{CSCSA-112860} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Seat_Doors_Test, Ced_Is_Occupant_Not_Waring_Seatbelt_Door__seat_not_relevant_belt_not_available)
{
   /** \arrange setup not available belt */
   uint8_t i                             = 0;
   Ced_Door_Position_T ced_door_position = CED_DOOR_POSITION_FRONT_LEFT;
   for (i = FBK_ZERO_UINT; i < CED_BMW_NUMBER_OF_SEATS; i++)
   {
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].seat_location.absolute_seat_location =
         ABSOLUTE_SEAT_LOCATION_SEAT_LEFT_ROW1;
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].status_belt.qualifier_value_status_belt_buckle_switch =
         QUALIFIER_VALUE_NOT_AVAILABLE;
   }
   boolean_T result;

   /** \action check occupant to leave the vehicle */
   result = Ced_Is_Occupant_Not_Waring_Seatbelt_Door(&ced_input.bmw_boardnet_signals, ced_door_position);

   /** \assert expect seatbelt */
   EXPECT_FALSE(result);
}

/**
 * Check that there is a occupant who might leave the ego vehicle for a single seat next to a door with belt buckled.
 * \uts{CSCSA-112861} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Seat_Doors_Test, Ced_Is_Occupant_Not_Waring_Seatbelt_Door__buckle_buckled)
{
   /** \arrange setup belt buckled */
   uint8_t i                             = 0;
   Ced_Door_Position_T ced_door_position = CED_DOOR_POSITION_FRONT_LEFT;
   for (i = FBK_ZERO_UINT; i < CED_BMW_NUMBER_OF_SEATS; i++)
   {
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].seat_location.absolute_seat_location =
         ABSOLUTE_SEAT_LOCATION_SEAT_LEFT_ROW1;
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].status_belt.qualifier_value_status_belt_buckle_switch =
         QUALIFIER_VALUE_OK;
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].seat_occupancy_status.qualifier_container_seat_occupancy_status =
         QUALIFIER_CONTAINER_OK;
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].seat_occupancy_status.seat_occupancy = SEAT_OCCUPANCY_LIVING_BEING;
      ced_input.bmw_boardnet_signals.seat_status_for_each_seat[i].status_belt.status_belt_buckle_switch =
         STATUS_BELT_BUCKLE_SWITCH_BUCKLED;
   }
   boolean_T result;

   /** \action check occupant to leave the vehicle */
   result = Ced_Is_Occupant_Not_Waring_Seatbelt_Door(&ced_input.bmw_boardnet_signals, ced_door_position);

   /** \assert expect seatbelt */
   EXPECT_FALSE(result);
}

/**
 * Check that there is a unknown location of seat based on door position
 * \uts{CSCSA-112862} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Seat_Doors_Test, Ced_Get_Absolute_Seat_Location_From_Door_Position__unknown_location)
{
   /** \arrange setup uknown door postion */

   Absolute_Seat_Location_T result;
   Ced_Door_Position_T ced_door_position = CED_DOOR_POSITION_UNKNOWN;

   /** \action check absolute seat location */
   result = Ced_Get_Absolute_Seat_Location_From_Door_Position(ced_door_position);

   /** \assert expect unknown location */
   EXPECT_EQ(result, ABSOLUTE_SEAT_LOCATION_UNKNOWN);
}

/**
 * Check that there is a no information about wearing seat belt based on unknown postion
 * \uts{CSCSA-112863} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Seat_Doors_Test, Ced_Is_Occupant_Wearing_Seat_Belt__unknown_location)
{
   /** \arrange setup uknown door postion */

   boolean_T result;

   Ced_Door_Position_T ced_door_position = CED_DOOR_POSITION_UNKNOWN;

   /** \action check occupant wearing seat belt */
   result = Ced_Is_Occupant_Wearing_Seat_Belt(&ced_output.ced_output_occupant_detection, ced_door_position);

   /** \assert expect false */
   EXPECT_FALSE(result);
}

/**
 * Check that there is no alert for undefined side due to unknown postion of seat.
 * \uts{CSCSA-112953} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Seat_Doors_Test, Ced_Get_Core_Alert_State__unknown_location)
{
   /** \arrange setup uknown door postion */
   Ced_Alert_T result;
   Ced_Door_Position_T ced_door_position = CED_DOOR_POSITION_UNKNOWN;

   /** \action check side door position */
   result = Ced_Get_Core_Alert_State(&ced_core_output, ced_door_position);

   /** \assert expect false */
   EXPECT_EQ(result, CED_NO_ALERT);
}

/**
 * Check that there is undefined side due to unknown postion of seat
 * \uts{CSCSA-112954} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Seat_Doors_Test, Ced_Get_Fbk_Side_From_Door_Position__unknown_location)
{
   /** \arrange setup unknown door postion */
   uint8_t result;
   Ced_Door_Position_T ced_door_position = CED_DOOR_POSITION_UNKNOWN;

   /** \action check side door position */
   result = Ced_Get_Fbk_Side_From_Door_Position(ced_door_position);

   /** \assert expect false */
   EXPECT_EQ(result, FBK_SIDE_UNDEFINED);
}

/**
 * Check that there is undefined direction of object due to unknown postion of seat
 * \uts{CSCSA-112955} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Seat_Doors_Test, Ced_Get_Core_Alert_Direction__unknown_location)
{
   /** \arrange setup unknown door postion */
   Ced_Target_Travel_Direction_T result;
   Ced_Door_Position_T ced_door_position = CED_DOOR_POSITION_UNKNOWN;

   /** \action check side door position */
   result = Ced_Get_Core_Alert_Direction(&ced_core_output, ced_door_position);

   /** \assert expect false */
   EXPECT_EQ(result, UNDEF_DIRECTION);
}
