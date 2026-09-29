/**
 * @file ced_post_run_boardnet_interior_light_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for BMW SP25 boardnet interior light module
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-73116}
 */

#include "ced_post_run_boardnet_interior_light_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "ced_bmw_sp25_init.h"
#include "ced_bmw_sp25_types.h"
#include "ced_post_run_boardnet_interior_light.c"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "pt_output_t.h"
}


/**
 * Check that Ced_Control_Interior_Lights is working fine.
 * \uts{CSCSA-73117} \sdd{SF-3413} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Interior_Light_Test, Ced_Control_Interior_Lights__general)
{
   /** \arrange setup typical data */
   door_position = CED_DOOR_POSITION_FRONT_LEFT;
   boolean_T result;

   /** \action check if light warning should be enabled */
   result = Ced_Is_Interior_Light_Warning_Enabled(&ced_input, door_position);

   /** \assert expect warning light */
   EXPECT_TRUE(result);
}

/**
 * Check that light warning for rear seats based on passenger rear seat is working fine.
 * \uts{CSCSA-112875} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Interior_Light_Test, Ced_Is_Parental_Control_Active_On_Rear_Seat_For_Interior_Light__passenger_rear_enabled)
{
   /** \arrange setup data for passeneger rear control */
   ced_input.ced_input_bus_signals.f_ced_parental_controls_driver_rear    = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_parental_controls_passenger_rear = FBK_TRUE;
   boolean_T result;

   /** \action check if light warning for rear seats should be enabled */
   result = Ced_Is_Parental_Control_Active_On_Rear_Seat_For_Interior_Light(&ced_input);

   /** \assert expect warning light */
   EXPECT_TRUE(result);
}


/**
 * Check for interior light that door is requested to be opened by rear right door lock.
 * \uts{CSCSA-112876} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Interior_Light_Test,
       Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_In_Parental_Control__rear_right_door_lock_control)
{
   /** \arrange setup data for rear right door lock */
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left  = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_right = FBK_FALSE;
   boolean_T result;

   /** \action check if door is opened */
   result = Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_In_Parental_Control(&ced_input);

   /** \assert expect door opened */
   EXPECT_TRUE(result);
}

/**
 * Check for interior light that door is requested to be opened by rear left door lock request.
 * \uts{CSCSA-112877} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Interior_Light_Test,
       Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_In_Parental_Control__rear_left_door_lock_request)
{
   /** \arrange setup data for only rear left door lock request */
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left         = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_right        = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_left = FBK_TRUE;
   boolean_T result;

   /** \action check if door is requested to be opened */
   result = Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_In_Parental_Control(&ced_input);

   /** \assert expect door opened request */
   EXPECT_TRUE(result);
}

/**
 * Check for interior light that door is requested to be opened by rear left door lock request.
 * \uts{CSCSA-112878} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Interior_Light_Test,
       Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_In_Parental_Control__rear_right_door_lock_request)
{
   /** \arrange setup data for only rear right door lock request */
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left          = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_right         = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_left  = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_right = FBK_TRUE;
   boolean_T result;

   /** \action check if door is requested to be opened */
   result = Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_In_Parental_Control(&ced_input);

   /** \assert expect door opened request */
   EXPECT_TRUE(result);
}


/**
 * Check that acute warning for interior light is not suppressed by no door opened.
 * \uts{CSCSA-112879} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Interior_Light_Test,
       Ced_Is_Acute_Warning_Needs_To_Be_Suppressed_For_Interior_Light__not_suppress_by_no_door_opened)
{
   /** \arrange setup data for no opened door */
   ced_input.ced_input_bus_signals.f_ced_parental_controls_driver_rear        = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left          = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_right         = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_left  = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_right = FBK_FALSE;
   boolean_T result;

   /** \action check that warning isn't suppressed */
   result = Ced_Is_Acute_Warning_Needs_To_Be_Suppressed_For_Interior_Light(&ced_input);

   /** \assert expect no suppressed warning */
   EXPECT_FALSE(result);
}

/**
 * Check that acute warning for interior light is suppressed.
 * \uts{CSCSA-112880} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Interior_Light_Test, Ced_Is_Acute_Warning_Needs_To_Be_Suppressed_For_Interior_Light__warning_suppressed)
{
   /** \arrange setup data for no opened door */
   ced_input.ced_input_bus_signals.f_ced_parental_controls_driver_rear    = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_parental_controls_passenger_rear = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left      = FBK_FALSE;
   boolean_T result;

   /** \action check that warning isn't suppressed */
   result = Ced_Is_Acute_Warning_Needs_To_Be_Suppressed_For_Interior_Light(&ced_input);

   /** \assert expect no suppressed warning */
   EXPECT_TRUE(result);
}

/**
 * Check that interior light are rested due to signal deactivated for alert level 2.
 * \uts{CSCSA-112881} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Interior_Light_Test,
       Ced_Control_Interior_Lights_Side__reset_warning_for_alert_level_2_due_to_interior_light_deactivated)
{
   /** \arrange setup data for reseting interior light */
   Ced_Door_Position_T ced_door                                      = CED_DOOR_POSITION_REAR_LEFT;
   ced_core_output.ced_alert[FBK_SIDE_LEFT]                          = CED_ALERT_ACTIVE_LEVEL_2;
   ced_input.ced_coding_parameters.c_f_sfe_interior_light_activation = FBK_FALSE;


   /** \action execute Ced_Control_Interior_Lights_Side */
   Ced_Control_Interior_Lights_Side(&ced_output, &ced_core_output, &ced_input, ced_door);

   /** \assert warning ambient inidcator corresponds to the warning level */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[ced_door], CED_DOOR_WARNING_LEVEL_NO_WARNING);
}


/**
 * Check that interior light works properly when acute warning is suppressed.
 * \uts{CSCSA-112882} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Interior_Light_Test, Ced_Control_Interior_Lights_Side__acute_warning_suppressed)
{
   /** \arrange setup data for no acute warning */
   Ced_Door_Position_T ced_door                                           = CED_DOOR_POSITION_FRONT_LEFT;
   Ced_Door_Warning_Levels_T ced_warning_level                            = CED_DOOR_WARNING_LEVEL_2;
   ced_core_output.ced_alert[FBK_SIDE_LEFT]                               = CED_ALERT_ACTIVE_LEVEL_2;
   ced_input.ced_input_bus_signals.f_ced_parental_controls_driver_rear    = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_parental_controls_passenger_rear = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left      = FBK_FALSE;

   /** \action execute Ced_Control_Interior_Lights_Side */
   Ced_Control_Interior_Lights_Side(&ced_output, &ced_core_output, &ced_input, ced_door);

   /** \assert warning ambient inidcator corresponds to the warning level */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[ced_door], ced_warning_level);
}

/**
 * Check that interior light are rested due to signal deactivated for alert level 1.
 * \uts{CSCSA-112883} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Interior_Light_Test,
       Ced_Control_Interior_Lights_Side__reset_warning_for_alert_lvl_1_due_to_interior_light_deactivated)
{
   /** \arrange setup data for reseting interior light */
   Ced_Door_Position_T ced_door                                      = CED_DOOR_POSITION_REAR_LEFT;
   ced_core_output.ced_alert[FBK_SIDE_LEFT]                          = CED_ALERT_ACTIVE_LEVEL_1;
   ced_input.ced_coding_parameters.c_f_sfe_interior_light_activation = FBK_FALSE;

   /** \action execute Ced_Control_Interior_Lights_Side */
   Ced_Control_Interior_Lights_Side(&ced_output, &ced_core_output, &ced_input, ced_door);

   /** \assert warning ambient inidcator corresponds to the warning level */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_ambient[ced_door], CED_DOOR_WARNING_LEVEL_NO_WARNING);
}

/**
 * Check that acute warning for interior light is disabled due to interior light deactivated.
 * \uts{CSCSA-112884} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Interior_Light_Test,
       Ced_Is_Interior_Light_Acute_Warning_Enabled__no_acute_warning_interior_light_deactivated)
{
   /** \arrange enabling acute warning for interior light */
   Ced_Door_Position_T ced_door                                                         = CED_DOOR_POSITION_FRONT_RIGHT;
   ced_output.ced_output_occupant_detection.f_occupant_right_front_not_wearing_seatbelt = FBK_TRUE;
   ced_input.ced_coding_parameters.c_f_sfe_interior_light_activation                    = FBK_FALSE;
   boolean_T result;

   /** \action execute Ced_Is_Interior_Light_Acute_Warning_Enabled */
   result = Ced_Is_Interior_Light_Acute_Warning_Enabled(&ced_input, ced_door);

   /** \assert acute warning disabled */
   EXPECT_FALSE(result);
}

/**
 * Check that interior light information is disabled due to interior light deactivated.
 * \uts{CSCSA-112885} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Interior_Light_Test, Ced_Is_Interior_Light_Information_Enabled__information_disabled)
{
   /** \arrange setup interior light activation as disabled */
   Ced_Door_Position_T ced_door                                                         = CED_DOOR_POSITION_FRONT_RIGHT;
   ced_output.ced_output_occupant_detection.f_occupant_right_front_not_wearing_seatbelt = FBK_TRUE;
   ced_input.ced_coding_parameters.c_f_sfe_interior_light_activation                    = FBK_FALSE;
   boolean_T result;

   /** \action execute Ced_Is_Interior_Light_Information_Enabled */
   result = Ced_Is_Interior_Light_Information_Enabled(&ced_output, &ced_input, ced_door);

   /** \assert interior light information disabled */
   EXPECT_FALSE(result);
}

/**
 * Check that interior light information is disabled due to unknown door position.
 * \uts{CSCSA-112886} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Interior_Light_Test, Ced_Is_Interior_Light_Information_Enabled__information_disabled_unknown_doors_position)
{
   /** \arrange setup interior light activation as disabled */
   Ced_Door_Position_T ced_door = CED_DOOR_POSITION_UNKNOWN;
   boolean_T result;

   /** \action execute Ced_Is_Interior_Light_Information_Enabled */
   result = Ced_Is_Interior_Light_Information_Enabled(&ced_output, &ced_input, ced_door);

   /** \assert interior light information disabled */
   EXPECT_FALSE(result);
}

/**
 * Check that interior light information is disabled due to unknown door position and interior light deactivated.
 * \uts{CSCSA-112887} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Interior_Light_Test,
       Ced_Is_Interior_Light_Information_Enabled__information_disabled_unknown_doors_position_and_signal_deactivated)
{
   /** \arrange setup interior light activation as and unknwon ced door position */
   Ced_Door_Position_T ced_door                                      = CED_DOOR_POSITION_UNKNOWN;
   ced_input.ced_coding_parameters.c_f_sfe_interior_light_activation = FBK_FALSE;
   boolean_T result;

   /** \action execute Ced_Is_Interior_Light_Information_Enabled */
   result = Ced_Is_Interior_Light_Information_Enabled(&ced_output, &ced_input, ced_door);

   /** \assert interior light information disabled */
   EXPECT_FALSE(result);
}


/**
 * Check that interior light warning is disabled due to interior light deactivated.
 * \uts{CSCSA-112888} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Interior_Light_Test, Ced_Is_Interior_Light_Warning_Enabled__warning_disabled_interior_light_deactivated)
{
   /** \arrange setup interior light activation as disabled */
   Ced_Door_Position_T ced_door                                                         = CED_DOOR_POSITION_FRONT_RIGHT;
   ced_output.ced_output_occupant_detection.f_occupant_right_front_not_wearing_seatbelt = FBK_TRUE;
   ced_input.ced_coding_parameters.c_f_sfe_interior_light_activation                    = FBK_FALSE;
   boolean_T result;

   /** \action execute Ced_Is_Interior_Light_Warning_Enabled */
   result = Ced_Is_Interior_Light_Warning_Enabled(&ced_input, ced_door);

   /** \assert interior light warning disabled */
   EXPECT_FALSE(result);
}

/**
 * Check that interior light warning is disabled due to uknwon door position and interior light deactivated.
 * \uts{CSCSA-112889} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Interior_Light_Test,
       Ced_Is_Interior_Light_Warning_Enabled__warning_disabled_interior_light_deactivated_and_signal_deactivated)
{
   /** \arrange setup unknown door postion interior light activation as disabled */
   Ced_Door_Position_T ced_door                                      = CED_DOOR_POSITION_UNKNOWN;
   ced_input.ced_coding_parameters.c_f_sfe_interior_light_activation = FBK_FALSE;
   boolean_T result;

   /** \action execute Ced_Is_Interior_Light_Warning_Enabled */
   result = Ced_Is_Interior_Light_Warning_Enabled(&ced_input, ced_door);

   /** \assert interior light warning disabled */
   EXPECT_FALSE(result);
}
