/**
 * @file ced_post_run_boardnet_mirror_led_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for BMW SP25 boardnet mirror led module
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-112820}
 */

#include "ced_post_run_boardnet_mirror_led_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "ced_bmw_sp25_init.h"
#include "ced_bmw_sp25_types.h"
#include "ced_post_run_boardnet_mirror_led.c"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "pt_output_t.h"
}


/**
 * Check for mirror led parental control on rear seats is active by passenger rear.
 * \uts{CSCSA-112821} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Mirror_Led_Test, Ced_Is_Parental_Control_Active_On_Rear_Seat_For_Mirror_Led__general_passenger_rear)
{
   /** \arrange setup passanger rear as active */
   ced_input.ced_input_bus_signals.f_ced_parental_controls_driver_rear    = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_parental_controls_passenger_rear = FBK_TRUE;
   boolean_T result;

   /** \action checks if Parental Control on Rear Seats are active or not */
   result = Ced_Is_Parental_Control_Active_On_Rear_Seat_For_Mirror_Led(&ced_input);

   /** \assert expect active control */
   EXPECT_TRUE(result);
}

/**
 * Check for mirror led parental control on rear seats is active by driver rear.
 * \uts{CSCSA-112822} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Mirror_Led_Test, Ced_Is_Parental_Control_Active_On_Rear_Seat_For_Mirror_Led__general_driver_rear)
{
   /** \arrange setup driver rear as active */
   ced_input.ced_input_bus_signals.f_ced_parental_controls_driver_rear    = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_parental_controls_passenger_rear = FBK_TRUE;
   boolean_T result;

   /** \action checks if Parental Control on Rear Seats are active or not */
   result = Ced_Is_Parental_Control_Active_On_Rear_Seat_For_Mirror_Led(&ced_input);

   /** \assert expect active control */
   EXPECT_TRUE(result);
}

/**
 * Check for mirror led that rear right door is opened while having parental control.
 * \uts{CSCSA-112823} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Mirror_Led_Test,
       Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_While_Having_Parental_Control__rear_right_lock_opened)
{
   /** \arrange setup rear right lock as true */
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left  = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_right = FBK_FALSE;
   boolean_T result;

   /** \action checks if Parental Control on Rear Seats are active or not */
   result = Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_While_Having_Parental_Control(&ced_input);

   /** \assert expect rear right door opened */
   EXPECT_TRUE(result);
}

/**
 * Check for mirror led that rear left door is requested to open while having parental control.
 * \uts{CSCSA-112824} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Mirror_Led_Test,
       Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_While_Having_Parental_Control__rear_left_requested_to_open)
{
   /** \arrange setup rear right lock as true */
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left         = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_right        = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_left = FBK_TRUE;
   boolean_T result;

   /** \action checks if Parental Control on Rear Seats are active or not */
   result = Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_While_Having_Parental_Control(&ced_input);

   /** \assert expect rear left door is requested to open */
   EXPECT_TRUE(result);
}

/**
 * Check for mirror led that rear right door is requested to open while having parental control.
 * \uts{CSCSA-112825} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Mirror_Led_Test,
       Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_While_Having_Parental_Control__rear_right_requested_to_open)
{
   /** \arrange setup rear right lock as true */
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left          = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_right         = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_left  = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_right = FBK_TRUE;
   boolean_T result;

   /** \action checks if Parental Control on Rear Seats are active or not */
   result = Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_While_Having_Parental_Control(&ced_input);

   /** \assert expect rear right door is requested to open */
   EXPECT_TRUE(result);
}

/**
 * Check for mirror led that there is no door opened or no requested to be opened.
 * \uts{CSCSA-112826} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Mirror_Led_Test,
       Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_While_Having_Parental_Control__no_door_opened_or_requested_to_be_opened)
{
   /** \arrange setup rear right lock as true */
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left          = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_right         = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_left  = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_right = FBK_FALSE;
   boolean_T result;

   /** \action checks if Parental Control on Rear Seats are active or not */
   result = Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_While_Having_Parental_Control(&ced_input);

   /** \assert expect no door opened and no requests */
   EXPECT_FALSE(result);
}

/**
 * Check for mirror led that acute warning is not suppressed due to no parental control and no door opened.
 * \uts{CSCSA-112827} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Mirror_Led_Test,
       Ced_Is_Acute_Warning_Needs_To_Be_Suppressed_For_Mirror_Led__not_suppressed_due_no_parental_control_and_door_no_opened)
{
   /** \arrange setup doors and controls properties */
   ced_input.ced_input_bus_signals.f_ced_parental_controls_driver_rear        = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_parental_controls_passenger_rear     = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left          = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_right         = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_left  = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_right = FBK_FALSE;
   boolean_T result;

   /** \action checks if acute warning for mirror led is suppressed */
   result = Ced_Is_Acute_Warning_Needs_To_Be_Suppressed_For_Mirror_Led(&ced_input);

   /** \assert expect no suppression */
   EXPECT_FALSE(result);
}

/**
 * Check for mirror led that acute warning is not suppressed due to no door opened or no requested.
 * \uts{CSCSA-112828} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Mirror_Led_Test,
       Ced_Is_Acute_Warning_Needs_To_Be_Suppressed_For_Mirror_Led__not_suppressed_due_no_door_opened)
{
   /** \arrange setup doors and controls properties */
   ced_input.ced_input_bus_signals.f_ced_parental_controls_driver_rear        = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_parental_controls_passenger_rear     = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left          = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_right         = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_left  = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_right = FBK_FALSE;
   boolean_T result;

   /** \action checks if acute warning for mirror led is suppressed */
   result = Ced_Is_Acute_Warning_Needs_To_Be_Suppressed_For_Mirror_Led(&ced_input);

   /** \assert expect no suppression */
   EXPECT_FALSE(result);
}

/**
 * Check for mirror led that acute warning is suppressed.
 * \uts{CSCSA-112829} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Mirror_Led_Test, Ced_Is_Acute_Warning_Needs_To_Be_Suppressed_For_Mirror_Led__acute_warning_suppressed)
{
   /** \arrange setup doors and controls properties */
   ced_input.ced_input_bus_signals.f_ced_parental_controls_driver_rear = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left   = FBK_FALSE;
   boolean_T result;

   /** \action checks if acute warning for mirror led is suppressed */
   result = Ced_Is_Acute_Warning_Needs_To_Be_Suppressed_For_Mirror_Led(&ced_input);

   /** \assert expect acute warning suppressed */
   EXPECT_TRUE(result);
}

/**
 * Check that acute warning for mirror led is disabled due to light deactivated.
 * \uts{CSCSA-112830} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Mirror_Led_Test, Ced_Is_Mirror_Led_Acute_Warning_Enabled__acute_warning_disabled)
{
   /** \arrange setup mirror led deactivated */
   const Ced_Sides_T ced_side                                                    = CED_SIDE_RIGHT;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_activation               = FBK_FALSE;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_acute_warning_activation = FBK_TRUE;

   boolean_T result;

   /** \action checks if acute warning for mirror led is enabled */
   result = Ced_Is_Mirror_Led_Acute_Warning_Enabled(&ced_input, ced_side);

   /** \assert expect mirror led acute warning is disabled */
   EXPECT_FALSE(result);
}

/**
 * Check that mirror led are rested due to signal deactivated for alert level 2.
 * \uts{CSCSA-112831} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Mirror_Led_Test,
       Ced_Control_Mirror_Led_Side__reset_warning_for_alert_level_2_due_to_interior_light_deactivated)
{
   /** \arrange setup data for reseting mirror led */
   Ced_Sides_T ced_side                                            = CED_SIDE_LEFT;
   ced_core_output.ced_alert[FBK_SIDE_LEFT]                        = CED_ALERT_ACTIVE_LEVEL_2;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_activation = FBK_FALSE;


   /** \action execute Ced_Control_Mirror_Led_Side */
   Ced_Control_Mirror_Led_Side(&ced_output, &ced_core_output, &ced_input, ced_side);

   /** \assert warning ambient inidcator corresponds to the warning level */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[ced_side], CED_MIRROR_LIGHT_WARNING_OFF);
}

/**
 * Check that mirror led works properly when acute warning is suppressed.
 * \uts{CSCSA-112832} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Mirror_Led_Test, Ced_Control_Mirror_Led_Side__acute_warning_suppressed)
{
   /** \arrange setup data for no acute warning */
   Ced_Sides_T ced_side                                                   = CED_SIDE_LEFT;
   ced_core_output.ced_alert[FBK_SIDE_LEFT]                               = CED_ALERT_ACTIVE_LEVEL_2;
   ced_input.ced_input_bus_signals.f_ced_parental_controls_driver_rear    = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_parental_controls_passenger_rear = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left      = FBK_FALSE;

   /** \action execute Ced_Control_Mirror_Led_Side */
   Ced_Control_Mirror_Led_Side(&ced_output, &ced_core_output, &ced_input, ced_side);

   /** \assert warning ambient inidcator corresponds to the warning level */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[ced_side],
             ced_input.ced_coding_parameters.c_sfe_mirror_light_warning_type);
}


/**
 * Check that mirror led are rested due to signal deactivated for alert level 1.
 * \uts{CSCSA-112833} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Mirror_Led_Test,
       Ced_Control_Mirror_Led_Side__reset_warning_for_alert_lvl_1_due_to_interior_light_deactivated)
{
   /** \arrange setup data for reseting mirror led */
   Ced_Sides_T ced_side                                            = CED_SIDE_LEFT;
   ced_core_output.ced_alert[FBK_SIDE_LEFT]                        = CED_ALERT_ACTIVE_LEVEL_1;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_activation = FBK_FALSE;

   /** \action execute Ced_Control_Mirror_Led_Side */
   Ced_Control_Mirror_Led_Side(&ced_output, &ced_core_output, &ced_input, ced_side);

   /** \assert warning ambient inidcator corresponds to the warning level */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_mirror[ced_side], CED_MIRROR_LIGHT_WARNING_OFF);
}


/**
 * Check that information in mirror led is disabled due to light deactivated.
 * \uts{CSCSA-112834} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Mirror_Led_Test, Ced_Is_Mirror_Led_Information_Enabled__acute_warning_disabled_due_to_light_deactivated)
{
   /** \arrange setup mirror light deactivated */
   const Ced_Sides_T ced_side                                                  = CED_SIDE_RIGHT;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_activation             = FBK_FALSE;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_information_activation = FBK_TRUE;

   boolean_T result;

   /** \action checks if information for mirror led is enabled */
   result = Ced_Is_Mirror_Led_Information_Enabled(&ced_output, &ced_input, ced_side);

   /** \assert expect mirror led acute information is disabled */
   EXPECT_FALSE(result);
}

/**
 * Check that led information in mirror led is disabled due acute_warning deactivated.
 * \uts{CSCSA-112835} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Mirror_Led_Test, Ced_Is_Mirror_Led_Information_Enabled__acute_warning_disabled_due_to_paramater_deactivated)
{
   /** \arrange setup acute warning deactivated */
   const Ced_Sides_T ced_side                                                  = CED_SIDE_RIGHT;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_activation             = FBK_TRUE;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_information_activation = FBK_FALSE;

   boolean_T result;

   /** \action checks if information for mirror led is enabled */
   result = Ced_Is_Mirror_Led_Information_Enabled(&ced_output, &ced_input, ced_side);

   /** \assert expect mirror led information is disabled */
   EXPECT_FALSE(result);
}

/**
 * Check that led warning in mirror led is disabled due light parameter deactivated.
 * \uts{CSCSA-112836} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Mirror_Led_Test, Ced_Is_Mirror_Led_Warning_Enabled__led_warning_disabled_due_to_light_deactivated)
{
   /** \arrange setup acute warning deactivated */
   const Ced_Sides_T ced_side                                              = CED_SIDE_RIGHT;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_activation         = FBK_FALSE;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_warning_activation = FBK_TRUE;

   boolean_T result;

   /** \action checks if led warning is enabled */
   result = Ced_Is_Mirror_Led_Warning_Enabled(&ced_input, ced_side);

   /** \assert expect mirror led information is disabled */
   EXPECT_FALSE(result);
}

/**
 * Check that led warning in mirror led is disabled due acute warning deactivated.
 * \uts{CSCSA-112837} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Mirror_Led_Test, Ced_Is_Mirror_Led_Warning_Enabled__led_warning_disabled_due_to_parameter_deactivated)
{
   /** \arrange setup acute warning deactivated */
   const Ced_Sides_T ced_side                                              = CED_SIDE_RIGHT;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_activation         = FBK_TRUE;
   ced_input.ced_coding_parameters.c_f_sfe_mirror_light_warning_activation = FBK_FALSE;

   boolean_T result;

   /** \action checks if led warning is enabled */
   result = Ced_Is_Mirror_Led_Warning_Enabled(&ced_input, ced_side);

   /** \assert expect mirror led information is disabled */
   EXPECT_FALSE(result);
}
