/**
 * @file ced_post_run_boardnet_optical_elements_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for BMW SP25 boardnet interior light module
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-112838}
 */

#include "ced_post_run_boardnet_optical_elements_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "ced_bmw_sp25_init.h"
#include "ced_bmw_sp25_types.h"
#include "ced_post_run_boardnet_optical_element.c"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "pt_output_t.h"
}


/**
 * Check that Ced_Is_Optical_Element_Warning_Enabled is working fine.
 * \uts{CSCSA-112918} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Optical_Elements_Test, Ced_Is_Optical_Element_Warning_Enabled__general)
{
   /** \arrange setup typical data */
   door_position = CED_DOOR_POSITION_FRONT_LEFT;
   boolean_T result;

   /** \action check if optical warning should be enabled */
   result = Ced_Is_Optical_Element_Warning_Enabled(&ced_input, door_position);

   /** \assert expect optical warning */
   EXPECT_TRUE(result);
}

/**
 * Check that Parental Control on rear seats for optical element is active by passenger rear.
 * \uts{CSCSA-112919} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Optical_Elements_Test,
       Ced_Is_Parental_Control_Active_On_Rear_Seat_For_Optical_Element__general_passenger_rear)
{
   /** \arrange setup passanger rear as active */
   ced_input.ced_input_bus_signals.f_ced_parental_controls_driver_rear    = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_parental_controls_passenger_rear = FBK_TRUE;
   boolean_T result;

   /** \action checks if Parental Control on Rear Seats are active or not */
   result = Ced_Is_Parental_Control_Active_On_Rear_Seat_For_Optical_Element(&ced_input);

   /** \assert expect active control */
   EXPECT_TRUE(result);
}

/**
 * Check that Parental Control on rear seats for optical element is active by driver rear.
 * \uts{CSCSA-112920} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Optical_Elements_Test, Ced_Is_Parental_Control_Active_On_Rear_Seat_For_Optical_Element__general_driver_rear)
{
   /** \arrange setup driver rear as active */
   ced_input.ced_input_bus_signals.f_ced_parental_controls_driver_rear    = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_parental_controls_passenger_rear = FBK_TRUE;
   boolean_T result;

   /** \action checks if Parental Control on Rear Seats are active or not */
   result = Ced_Is_Parental_Control_Active_On_Rear_Seat_For_Optical_Element(&ced_input);

   /** \assert expect active control */
   EXPECT_TRUE(result);
}


/**
 * Check that rear right door is opened while having parental control.
 * \uts{CSCSA-112921} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Optical_Elements_Test,
       Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_With_Parental_Control__rear_right_lock_opened)
{
   /** \arrange setup rear right lock as true */
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left  = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_right = FBK_FALSE;
   boolean_T result;

   /** \action checks if Parental Control on Rear Seats are active or not */
   result = Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_With_Parental_Control(&ced_input);

   /** \assert expect rear right door opened */
   EXPECT_TRUE(result);
}

/**
 * Check that rear left door is requested to open while having parental control.
 * \uts{CSCSA-112922} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Optical_Elements_Test,
       Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_With_Parental_Control__rear_left_requested_to_open)
{
   /** \arrange setup rear right lock as true */
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left         = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_right        = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_left = FBK_TRUE;
   boolean_T result;

   /** \action checks if Parental Control on Rear Seats are active or not */
   result = Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_With_Parental_Control(&ced_input);

   /** \assert expect rear left door is requested to open */
   EXPECT_TRUE(result);
}

/**
 * Check that rear right door is requested to open while having parental control.
 * \uts{CSCSA-112923} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Optical_Elements_Test,
       Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_With_Parental_Control__rear_right_requested_to_open)
{
   /** \arrange setup rear right lock as true */
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left          = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_right         = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_left  = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_right = FBK_TRUE;
   boolean_T result;

   /** \action checks if Parental Control on Rear Seats are active or not */
   result = Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_With_Parental_Control(&ced_input);

   /** \assert expect rear right door is requested to open */
   EXPECT_TRUE(result);
}

/**
 * Check that there is no door opened or no requested to be opened.
 * \uts{CSCSA-112924} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Optical_Elements_Test,
       Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_With_Parental_Control__no_door_opened_or_requested_to_be_opened)
{
   /** \arrange setup rear right lock as true */
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left          = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_right         = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_left  = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_right = FBK_FALSE;
   boolean_T result;

   /** \action checks if Parental Control on Rear Seats are active or not */
   result = Ced_Is_Rear_Door_Opened_Or_Requested_To_Be_Opened_With_Parental_Control(&ced_input);

   /** \assert expect no door opened and no requests */
   EXPECT_FALSE(result);
}

/**
 * Check that for optical element acute warning is not suppressed due to no parental control and no door opened.
 * \uts{CSCSA-112925} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Optical_Elements_Test,
       Ced_Is_Acute_Warning_Needs_To_Be_Suppressed_For_Optical_Element__not_suppressed_due_no_parental_control_and_door_no_opened)
{
   /** \arrange setup doors and controls properties */
   ced_input.ced_input_bus_signals.f_ced_parental_controls_driver_rear        = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_parental_controls_passenger_rear     = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left          = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_right         = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_left  = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_right = FBK_FALSE;
   boolean_T result;

   /** \action checks if acute warning for optical element is suppressed */
   result = Ced_Is_Acute_Warning_Needs_To_Be_Suppressed_For_Optical_Element(&ced_input);

   /** \assert expect no suppression */
   EXPECT_FALSE(result);
}

/**
 * Check that for optical element acute warning is not suppressed due to no door opened or no requested.
 * \uts{CSCSA-112926} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Optical_Elements_Test,
       Ced_Is_Acute_Warning_Needs_To_Be_Suppressed_For_Optical_Element__not_suppressed_due_no_door_opened)
{
   /** \arrange setup doors and controls properties */
   ced_input.ced_input_bus_signals.f_ced_parental_controls_driver_rear        = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_parental_controls_passenger_rear     = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left          = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_right         = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_left  = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_request_rear_right = FBK_FALSE;
   boolean_T result;

   /** \action checks if acute warning for optical element is suppressed */
   result = Ced_Is_Acute_Warning_Needs_To_Be_Suppressed_For_Optical_Element(&ced_input);

   /** \assert expect no suppression */
   EXPECT_FALSE(result);
}

/**
 * Check that for optical element acute warning is suppressed.
 * \uts{CSCSA-112927} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Optical_Elements_Test,
       Ced_Is_Acute_Warning_Needs_To_Be_Suppressed_For_Optical_Element__acute_warning_suppressed)
{
   /** \arrange setup doors and controls properties */
   ced_input.ced_input_bus_signals.f_ced_parental_controls_driver_rear = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left   = FBK_FALSE;
   boolean_T result;

   /** \action checks if acute warning for optical element needs to be suprressed */
   result = Ced_Is_Acute_Warning_Needs_To_Be_Suppressed_For_Optical_Element(&ced_input);

   /** \assert expect acute warning suppressed */
   EXPECT_TRUE(result);
}

/**
 * Check that Ced_Control_Optical_Elements_Side works properly when acute warning is suppressed.
 * \uts{CSCSA-112928} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Optical_Elements_Test, Ced_Control_Optical_Elements_Side__acute_warning_suppressed)
{
   /** \arrange setup data for no acute warning */
   Ced_Door_Position_T ced_door                                           = CED_DOOR_POSITION_FRONT_LEFT;
   Ced_Door_Warning_Levels_T ced_warning_level                            = CED_DOOR_WARNING_LEVEL_2;
   ced_core_output.ced_alert[FBK_SIDE_LEFT]                               = CED_ALERT_ACTIVE_LEVEL_2;
   ced_input.ced_input_bus_signals.f_ced_parental_controls_driver_rear    = FBK_FALSE;
   ced_input.ced_input_bus_signals.f_ced_parental_controls_passenger_rear = FBK_TRUE;
   ced_input.ced_input_bus_signals.f_ced_vehicle_door_lock_rear_left      = FBK_FALSE;

   /** \action execute Ced_Control_Optical_Elements_Side */
   Ced_Control_Optical_Elements_Side(&ced_output, &ced_core_output, &ced_input, ced_door);

   /** \assert warning optical inidcator corresponds to the warning level */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[ced_door], ced_warning_level);
}

/**
 * Check that state of optical elements are rested due to signal deactivated for alert level 2.
 * \uts{CSCSA-112929} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Optical_Elements_Test,
       Ced_Control_Mirror_Led_Side__reset_warning_for_alert_level_2_due_to_optical_display_deactivated)
{
   /** \arrange setup data for reseting optical elements */
   Ced_Door_Position_T ced_door                                       = CED_DOOR_POSITION_FRONT_LEFT;
   ced_core_output.ced_alert[FBK_SIDE_LEFT]                           = CED_ALERT_ACTIVE_LEVEL_2;
   ced_input.ced_coding_parameters.c_f_sfe_optical_display_activation = FBK_FALSE;


   /** \action execute Ced_Control_Optical_Elements_Side */
   Ced_Control_Optical_Elements_Side(&ced_output, &ced_core_output, &ced_input, ced_door);

   /** \assert warning optical inidcator corresponds to the warning level */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[ced_door], CED_MIRROR_LIGHT_WARNING_OFF);
}

/**
 * Check that state of optical elements are rested due to signal deactivated for alert level 1.
 * \uts{CSCSA-112930} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Optical_Elements_Test,
       Ced_Control_Optical_Elements_Side__reset_warning_for_alert_lvl_1_due_to_optical_display_deactivated)
{
   /** \arrange setup data for reseting optical element led */
   Ced_Door_Position_T ced_door                                       = CED_DOOR_POSITION_FRONT_LEFT;
   ced_core_output.ced_alert[FBK_SIDE_LEFT]                           = CED_ALERT_ACTIVE_LEVEL_1;
   ced_input.ced_coding_parameters.c_f_sfe_optical_display_activation = FBK_FALSE;

   /** \action execute Ced_Control_Optical_Elements_Side */
   Ced_Control_Optical_Elements_Side(&ced_output, &ced_core_output, &ced_input, ced_door);

   /** \assert warning optical inidcator corresponds to the warning level */
   EXPECT_EQ(ced_output.ced_output_warning_indicators.ced_output_warning_optical[ced_door], CED_MIRROR_LIGHT_WARNING_OFF);
}

/**
 * Check that for optical element acute warning is disabled due to optical_display deactivated.
 * \uts{CSCSA-112931} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Optical_Elements_Test,
       Ced_Is_Optical_Element_Acute_Warning_Enabled__no_acute_warning_optical_display_deactivated)
{
   /** \arrange setup optical dispaly deactivated */
   Ced_Door_Position_T ced_door                                                        = CED_DOOR_POSITION_FRONT_LEFT;
   ced_output.ced_output_occupant_detection.f_occupant_left_front_not_wearing_seatbelt = FBK_TRUE;
   ced_input.ced_coding_parameters.c_f_sfe_optical_display_activation                  = FBK_FALSE;
   boolean_T result;

   /** \action execute Ced_Is_Optical_Element_Acute_Warning_Enabled */
   result = Ced_Is_Optical_Element_Acute_Warning_Enabled(&ced_input, ced_door);

   /** \assert acute warning disabled */
   EXPECT_FALSE(result);
}

/**
 * Check that optical element information is disabled due to dispaly parameter disabled.
 * \uts{CSCSA-112932} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Optical_Elements_Test, Ced_Is_Optical_Element_Information_Enabled__information_disabled)
{
   /** \arrange setup optical element activation as disabled */
   Ced_Door_Position_T ced_door                                                        = CED_DOOR_POSITION_REAR_RIGHT;
   ced_output.ced_output_occupant_detection.f_occupant_right_rear_not_wearing_seatbelt = FBK_TRUE;
   ced_input.ced_coding_parameters.c_f_sfe_optical_display_activation                  = FBK_FALSE;
   boolean_T result;

   /** \action execute Ced_Is_Optical_Element_Information_Enabled */
   result = Ced_Is_Optical_Element_Information_Enabled(&ced_output, &ced_input, ced_door);

   /** \assert optical element information disabled */
   EXPECT_FALSE(result);
}

/**
 * Check that acute warning for optical element is disabled due to not critical door position
 * \uts{CSCSA-112933} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Optical_Elements_Test,
       Ced_Is_Optical_Element_Information_Enabled__no_acute_warning_optical_element_no_critical_door_posistion)
{
   /** \arrange setup unknown door postion */
   Ced_Door_Position_T ced_door = CED_DOOR_POSITION_UNKNOWN;
   boolean_T result;

   /** \action execute Ced_Is_Interior_Light_Acute_Warning_Enabled */
   result = Ced_Is_Optical_Element_Information_Enabled(&ced_output, &ced_input, ced_door);

   /** \assert acute warning disabled */
   EXPECT_FALSE(result);
}

/**
 * Check that acute warning for optical element is disabled due to unknown door position and optical display deactivated
 * \uts{CSCSA-112934} \sdd{} \testtype{positive}
 */
TEST_F(Ced_Post_Run_Boardnet_Optical_Elements_Test,
       Ced_Is_Optical_Element_Information_Enabled__no_acute_warning_optical_element_no_critical_door_posistion_and_optical_display_deactivated)
{
   /** \arrange setup unknown door postion and optical element activation as disabled */
   Ced_Door_Position_T ced_door                                       = CED_DOOR_POSITION_UNKNOWN;
   ced_input.ced_coding_parameters.c_f_sfe_optical_display_activation = FBK_FALSE;
   boolean_T result;

   /** \action execute Ced_Is_Interior_Light_Acute_Warning_Enabled */
   result = Ced_Is_Optical_Element_Information_Enabled(&ced_output, &ced_input, ced_door);

   /** \assert acute warning disabled */
   EXPECT_FALSE(result);
}
/**
 * Check that optical element warning is disabled due to dispaly parameter disabled.
 * \uts{CSCSA-112935} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Optical_Elements_Test, Ced_Is_Optical_Element_Warning_Enabled__warning_disabled_optical_display_deactivated)
{
   /** \arrange setup optical element activation as disabled */
   Ced_Door_Position_T ced_door                                                         = CED_DOOR_POSITION_FRONT_RIGHT;
   ced_output.ced_output_occupant_detection.f_occupant_right_front_not_wearing_seatbelt = FBK_TRUE;
   ced_input.ced_coding_parameters.c_f_sfe_optical_display_activation                   = FBK_FALSE;
   boolean_T result;

   /** \action execute Ced_Is_Optical_Element_Warning_Enabled */
   result = Ced_Is_Optical_Element_Warning_Enabled(&ced_input, ced_door);

   /** \assert optical element warning disabled */
   EXPECT_FALSE(result);
}

/**
 * Check that optical element warning is disabled due to unknown door position and dispaly parameter disabled.
 * \uts{CSCSA-112936} \sdd{} \testtype{negative}
 */
TEST_F(Ced_Post_Run_Boardnet_Optical_Elements_Test,
       Ced_Is_Optical_Element_Warning_Enabled__warning_disabled__no_critical_door_posistion_and_optical_display_deactivated)
{
   /** \arrange setup optical element activation as disabled */
   Ced_Door_Position_T ced_door                                       = CED_DOOR_POSITION_UNKNOWN;
   ced_input.ced_coding_parameters.c_f_sfe_optical_display_activation = FBK_FALSE;
   boolean_T result;

   /** \action execute Ced_Is_Optical_Element_Warning_Enabled */
   result = Ced_Is_Optical_Element_Warning_Enabled(&ced_input, ced_door);

   /** \assert optical element warning disabled */
   EXPECT_FALSE(result);
}
