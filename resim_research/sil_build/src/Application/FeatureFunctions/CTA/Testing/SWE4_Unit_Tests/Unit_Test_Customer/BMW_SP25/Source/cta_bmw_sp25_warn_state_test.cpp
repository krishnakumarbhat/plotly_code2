/**
 * @file cta_bmw_sp25_warn_state_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for BMW SP25 CTA state machine
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-85494}
 */

#include "cta_bmw_sp25_warn_state_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "cta_bmw_sp25_warn_state.c"
#include "cta_core_calibration_t.h"
#include "fbk_macros.h"
}

/**
 * Check whether bmw warn state is correctly returned
 * \uts{CSCSA-85495} \sdd{SF-4043} \testtype{positive}
 */
TEST_F(Cta_Bmw_Sp25_Warn_State_Test, Cta_Bmw_Ctb_Get_Warn_Output_Control_State__general_test)
{
   /** \arrange empty output */
   Bmw_Ctb_Warn_Control_Output_State_T bmw_warn_state;

   /** \action call function */
   bmw_warn_state = Cta_Bmw_Ctb_Get_Warn_Output_Control_State(&cta_input, &cta_alert_level, p_vehicle_data);

   /** \assert expect no warning */
   EXPECT_EQ(bmw_warn_state, BMW_CTB_WARN_STATE_NO_WARNING);
}

/**
 * Check whether bmw warn state is correctly returned. front module is enabled
 * \uts{CSCSA-85496} \sdd{SF-4043} \testtype{positive}
 */
TEST_F(Cta_Bmw_Sp25_Warn_State_Test, Cta_Bmw_Ctb_Get_Warn_Output_Control_State__front_test)
{
   /** \arrange output with enabled front module */

   /* set front_rear_disabled false and front_enabled true*/
   Bmw_Ctb_Warn_Control_Output_State_T bmw_warn_state;
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled                 = true;
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant                 = BMW_CTB_VARIANT_CTB_REAR_AND_FRONT;
   p_vehicle_data->prndl                                             = PA_VEH_PRNDL_STATE_DRIVE;
   cta_input.bmw_ctb_input_signals.control_cross_traffic_alert_front = true;

   /* set info_warning_standstill_condition true*/
   cta_input.bmw_ctb_coding_parameters.c_ctb_acute_warning_same_time_with_advance_warning = FBK_FALSE;
   cta_input.bmw_ctb_input_signals.vehicle_moving_direction                               = BMW_CTB_VEHICLE_IS_IN_STANDSTILL;
   cta_alert_level                                                                        = CTA_CRIT_LEVEL_1;

   /** \action call function */
   bmw_warn_state = Cta_Bmw_Ctb_Get_Warn_Output_Control_State(&cta_input, &cta_alert_level, p_vehicle_data);

   /** \assert expect front warning */
   EXPECT_EQ(bmw_warn_state, BMW_CTB_WARN_STATE_INFO_WARNING_FRONT);
}

/**
 * Check whether bmw warn state is correctly returned. front module is enabled
 * \uts{CSCSA-85497} \sdd{SF-4043} \testtype{positive}
 */
TEST_F(Cta_Bmw_Sp25_Warn_State_Test, Cta_Bmw_Ctb_Get_Warn_Output_Control_State__front_with_info_warning_test)
{
   /** \arrange output with enabled front module */

   /* set front_rear_disabled false and front_enabled true*/
   Bmw_Ctb_Warn_Control_Output_State_T bmw_warn_state;
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled                 = true;
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant                 = BMW_CTB_VARIANT_CTB_REAR_AND_FRONT;
   p_vehicle_data->prndl                                             = PA_VEH_PRNDL_STATE_DRIVE;
   cta_input.bmw_ctb_input_signals.control_cross_traffic_alert_front = true;

   /* set info_warning_forward_condition true*/
   cta_input.bmw_ctb_coding_parameters.c_ctb_acute_warning_same_time_with_advance_warning = FBK_FALSE;
   cta_input.bmw_ctb_input_signals.vehicle_moving_direction                               = BMW_CTB_VEHICLE_IS_MOVING_FORWARDS;
   cta_alert_level                                                                        = CTA_CRIT_LEVEL_1;

   /** \action call function */
   bmw_warn_state = Cta_Bmw_Ctb_Get_Warn_Output_Control_State(&cta_input, &cta_alert_level, p_vehicle_data);

   /** \assert expect front warning */
   EXPECT_EQ(bmw_warn_state, BMW_CTB_WARN_STATE_INFO_WARNING_FRONT);
}

/**
 * Check whether bmw warn state is correctly returned. rear module is enabled
 * \uts{CSCSA-85498} \sdd{SF-4043} \testtype{positive}
 */
TEST_F(Cta_Bmw_Sp25_Warn_State_Test, Cta_Bmw_Ctb_Get_Warn_Output_Control_State__rear_with_info_warning_test)
{
   /** \arrange output with enabled rear module */

   /* set front_rear_disabled false and rear_enabled true*/
   Bmw_Ctb_Warn_Control_Output_State_T bmw_warn_state;
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled                        = true;
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant                        = BMW_CTB_VARIANT_CTB_REAR;
   p_vehicle_data->prndl                                                    = PA_VEH_PRNDL_STATE_REVERSE;
   cta_input.bmw_ctb_input_signals.control_cross_traffic_alert_rear         = true;
   cta_input.bmw_ctb_input_signals.control_cross_traffic_alert_rear_braking = true;

   /* set info_warning_backwards_condition true*/
   cta_input.bmw_ctb_coding_parameters.c_ctb_acute_warning_same_time_with_advance_warning = FBK_FALSE;
   cta_input.bmw_ctb_input_signals.vehicle_moving_direction                               = BMW_CTB_VEHICLE_IS_MOVING_BACKWARDS;
   cta_alert_level                                                                        = CTA_CRIT_LEVEL_1;

   /** \action call function */
   bmw_warn_state = Cta_Bmw_Ctb_Get_Warn_Output_Control_State(&cta_input, &cta_alert_level, p_vehicle_data);

   /** \assert expect front warning */
   EXPECT_EQ(bmw_warn_state, BMW_CTB_WARN_STATE_INFO_WARNING_REAR);
}

/**
 * Check whether bmw warn state is correctly returned. rear module is enabled
 * \uts{CSCSA-85499} \sdd{SF-4043} \testtype{positive}
 */
TEST_F(Cta_Bmw_Sp25_Warn_State_Test, Cta_Bmw_Ctb_Get_Warn_Output_Control_State__rear_test)
{
   /** \arrange output with enabled rear module */

   /* set front_rear_disabled false and rear_enabled true*/
   Bmw_Ctb_Warn_Control_Output_State_T bmw_warn_state;
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled                = true;
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant                = BMW_CTB_VARIANT_CTB_REAR;
   p_vehicle_data->prndl                                            = PA_VEH_PRNDL_STATE_REVERSE;
   cta_input.bmw_ctb_input_signals.control_cross_traffic_alert_rear = true;

   /* set info_warning_standstill_condition true*/
   cta_input.bmw_ctb_coding_parameters.c_ctb_acute_warning_same_time_with_advance_warning = FBK_FALSE;
   cta_input.bmw_ctb_input_signals.vehicle_moving_direction                               = BMW_CTB_VEHICLE_IS_IN_STANDSTILL;
   cta_alert_level                                                                        = CTA_CRIT_LEVEL_1;

   /** \action call function */
   bmw_warn_state = Cta_Bmw_Ctb_Get_Warn_Output_Control_State(&cta_input, &cta_alert_level, p_vehicle_data);

   /** \assert expect rear warning */
   EXPECT_EQ(bmw_warn_state, BMW_CTB_WARN_STATE_INFO_WARNING_REAR);
}

/**
 * Check whether bmw warn state is correctly returned. front module is enabled with acute warning
 * \uts{CSCSA-85500} \sdd{SF-4043} \testtype{positive}
 */
TEST_F(Cta_Bmw_Sp25_Warn_State_Test, Cta_Bmw_Ctb_Get_Warn_Output_Control_State__front_acute_test)
{
   /** \arrange output with enabled front module */

   /* set front_rear_disabled false and front_enabled true*/
   Bmw_Ctb_Warn_Control_Output_State_T bmw_warn_state;
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled = true;
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant = BMW_CTB_VARIANT_CTB_REAR_AND_FRONT;
   p_vehicle_data->prndl                             = PA_VEH_PRNDL_STATE_DRIVE;

   /* set acute_warning_forward_condition true*/
   cta_input.bmw_ctb_coding_parameters.c_ctb_acute_warning_same_time_with_advance_warning = FBK_FALSE;
   cta_input.bmw_ctb_input_signals.vehicle_moving_direction                               = BMW_CTB_VEHICLE_IS_MOVING_FORWARDS;
   cta_alert_level                                                                        = CTA_CRIT_LEVEL_2;

   /** \action call function */
   bmw_warn_state = Cta_Bmw_Ctb_Get_Warn_Output_Control_State(&cta_input, &cta_alert_level, p_vehicle_data);

   /** \assert expect front acute warning */
   EXPECT_EQ(bmw_warn_state, BMW_CTB_WARN_STATE_ACUTE_WARNING_FRONT);
}

/**
 * Check whether bmw warn state is correctly returned. rear module is enabled witha cute warning
 * \uts{CSCSA-85501} \sdd{SF-4043} \testtype{positive}
 */
TEST_F(Cta_Bmw_Sp25_Warn_State_Test, Cta_Bmw_Ctb_Get_Warn_Output_Control_State__rear_acute_test)
{
   /** \arrange output with enabled rear module */

   /* set front_rear_disabled false and rear true*/
   Bmw_Ctb_Warn_Control_Output_State_T bmw_warn_state;
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled = true;
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant = BMW_CTB_VARIANT_CTB_REAR;
   p_vehicle_data->prndl                             = PA_VEH_PRNDL_STATE_REVERSE;

   /* set acute_warning_backwards_condition true*/
   cta_input.bmw_ctb_coding_parameters.c_ctb_acute_warning_same_time_with_advance_warning = FBK_FALSE;
   cta_input.bmw_ctb_input_signals.vehicle_moving_direction                               = BMW_CTB_VEHICLE_IS_MOVING_BACKWARDS;
   cta_alert_level                                                                        = CTA_CRIT_LEVEL_2;

   /** \action call function */
   bmw_warn_state = Cta_Bmw_Ctb_Get_Warn_Output_Control_State(&cta_input, &cta_alert_level, p_vehicle_data);

   /** \assert expect rear acute warning */
   EXPECT_EQ(bmw_warn_state, BMW_CTB_WARN_STATE_ACUTE_WARNING_REAR);
}

/**
 * Check whether bmw warn state is correctly returned. front module is enabled with acute warning
 * \uts{CSCSA-85502} \sdd{SF-4043} \testtype{positive}
 */
TEST_F(Cta_Bmw_Sp25_Warn_State_Test, Cta_Bmw_Ctb_Get_Warn_Output_Control_State__front_acute_test_with_same_time)
{
   /** \arrange output with enabled front module */

   /* set front_rear_disabled false and front_enabled true*/
   Bmw_Ctb_Warn_Control_Output_State_T bmw_warn_state;
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled = true;
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant = BMW_CTB_VARIANT_CTB_REAR_AND_FRONT;
   p_vehicle_data->prndl                             = PA_VEH_PRNDL_STATE_DRIVE;

   /* set acute_warning_same_time_front_condition true*/
   cta_input.bmw_ctb_coding_parameters.c_ctb_acute_warning_same_time_with_advance_warning = FBK_TRUE;
   cta_input.bmw_ctb_input_signals.vehicle_moving_direction                               = BMW_CTB_VEHICLE_IS_IN_STANDSTILL;
   cta_alert_level                                                                        = CTA_CRIT_LEVEL_1;

   /** \action call function */
   bmw_warn_state = Cta_Bmw_Ctb_Get_Warn_Output_Control_State(&cta_input, &cta_alert_level, p_vehicle_data);

   /** \assert expect front acute warning */
   EXPECT_EQ(bmw_warn_state, BMW_CTB_WARN_STATE_ACUTE_WARNING_FRONT);
}

/**
 * Check whether bmw warn state is correctly returned. rear module is enabled witha cute warning
 * \uts{CSCSA-85503} \sdd{SF-4043} \testtype{positive}
 */
TEST_F(Cta_Bmw_Sp25_Warn_State_Test, Cta_Bmw_Ctb_Get_Warn_Output_Control_State__rear_acute_test_with_same_time)
{
   /** \arrange output with enabled rear module */

   /* set front_rear_disabled false and rear true*/
   Bmw_Ctb_Warn_Control_Output_State_T bmw_warn_state;
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled = true;
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant = BMW_CTB_VARIANT_CTB_REAR;
   p_vehicle_data->prndl                             = PA_VEH_PRNDL_STATE_REVERSE;

   /* set acute_warning_same_time_rear_condition true*/
   cta_input.bmw_ctb_coding_parameters.c_ctb_acute_warning_same_time_with_advance_warning = FBK_TRUE;
   cta_input.bmw_ctb_input_signals.vehicle_moving_direction                               = BMW_CTB_VEHICLE_IS_IN_STANDSTILL;
   cta_alert_level                                                                        = CTA_CRIT_LEVEL_1;

   /** \action call function */
   bmw_warn_state = Cta_Bmw_Ctb_Get_Warn_Output_Control_State(&cta_input, &cta_alert_level, p_vehicle_data);

   /** \assert expect rear acute warning */
   EXPECT_EQ(bmw_warn_state, BMW_CTB_WARN_STATE_ACUTE_WARNING_REAR);
}

/**
 * Check whether bmw warn state is correctly returned. here default value is expected due to no additional alerts
 * \uts{CSCSA-85504} \sdd{SF-4043} \testtype{positive}
 */
TEST_F(Cta_Bmw_Sp25_Warn_State_Test, Cta_Bmw_Ctb_Get_Warn_Output_Control_State__default_case_no_warning_front)
{
   /** \arrange output with enabled rear module */

   /* set front_rear_disabled false*/
   Bmw_Ctb_Warn_Control_Output_State_T bmw_warn_state;
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled = true;
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant = BMW_CTB_VARIANT_CTB_REAR_AND_FRONT;
   p_vehicle_data->prndl                             = PA_VEH_PRNDL_STATE_DRIVE;

   /** \action call function */
   bmw_warn_state = Cta_Bmw_Ctb_Get_Warn_Output_Control_State(&cta_input, &cta_alert_level, p_vehicle_data);

   /** \assert expect rear acute warning */
   EXPECT_EQ(bmw_warn_state, BMW_CTB_WARN_STATE_NO_WARNING);
}

/**
 * Check whether bmw warn state is correctly returned. here default value is expected due to no additional alerts
 * \uts{CSCSA-85505} \sdd{SF-4043} \testtype{positive}
 */
TEST_F(Cta_Bmw_Sp25_Warn_State_Test, Cta_Bmw_Ctb_Get_Warn_Output_Control_State__default_case_no_warning_rear)
{
   /** \arrange output with enabled rear module */

   /* set front_rear_disabled false*/
   Bmw_Ctb_Warn_Control_Output_State_T bmw_warn_state;
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled = true;
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant = BMW_CTB_VARIANT_CTB_REAR;
   p_vehicle_data->prndl                             = PA_VEH_PRNDL_STATE_DRIVE;

   /** \action call function */
   bmw_warn_state = Cta_Bmw_Ctb_Get_Warn_Output_Control_State(&cta_input, &cta_alert_level, p_vehicle_data);

   /** \assert expect rear acute warning */
   EXPECT_EQ(bmw_warn_state, BMW_CTB_WARN_STATE_NO_WARNING);
}

/**
 * Check function determining if cta is disabled runs correctly. Here reverse gear and no ctb variant is choosen, expect true
 * \uts{CSCSA-85506} \sdd{SF-4041} \testtype{positive}
 */
TEST_F(Cta_Bmw_Sp25_Warn_State_Test, Cta_Is_Disabled__stat_reverse)
{
   /** \arrange output with reverse gear */
   boolean_T result;
   p_vehicle_data->prndl = PA_VEH_PRNDL_STATE_REVERSE;
   /** \action call function */

   result = Cta_Is_Disabled(&(cta_input.bmw_ctb_coding_parameters), p_vehicle_data);

   /** \assert expect disabled state */
   EXPECT_TRUE(result);
}

/**
 * Check function determining if cta is disabled runs correctly. Here reverse gear and correct ctb variant is choosen, expect true
 * \uts{CSCSA-85507} \sdd{SF-4041} \testtype{positive}
 */
TEST_F(Cta_Bmw_Sp25_Warn_State_Test, Cta_Is_Disabled__stat_reverse_rear_ctb)
{
   /** \arrange output with reverse gear and correct ctb variant */
   boolean_T result;
   p_vehicle_data->prndl                             = PA_VEH_PRNDL_STATE_REVERSE;
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant = BMW_CTB_VARIANT_CTB_REAR;

   /** \action call function */
   result = Cta_Is_Disabled(&(cta_input.bmw_ctb_coding_parameters), p_vehicle_data);

   /** \assert expect disabled */
   EXPECT_TRUE(result);
}

/**
 * Check function determining if cta front is enabled runs correctly. Here reverse gear is choosen, expect false
 * \uts{CSCSA-85508} \sdd{SF-4051} \testtype{positive}
 */
TEST_F(Cta_Bmw_Sp25_Warn_State_Test, Cta_Is_Front_Enabled__stat_reverse_rear_ctb)
{
   /** \arrange output with reverse gear */
   boolean_T result;
   p_vehicle_data->prndl                             = PA_VEH_PRNDL_STATE_REVERSE;
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant = BMW_CTB_VARIANT_CTB_REAR_AND_FRONT;
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled = FBK_TRUE;

   /** \action call function */
   result = Cta_Is_Front_Enabled(&(cta_input.bmw_ctb_coding_parameters), p_vehicle_data);

   /** \assert expect false enable signal */
   EXPECT_FALSE(result);
}

/**
 * Check function determining if cta rear is enabled runs correctly. Here no correct ctb variant is choosen, expect false
 * \uts{CSCSA-85509} \sdd{SF-4050} \testtype{positive}
 */
TEST_F(Cta_Bmw_Sp25_Warn_State_Test, Cta_Is_Rear_Enabled__no_ctb_variant)
{
   /** \arrange output with no ctb variant */
   boolean_T result;
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant = BMW_CTB_VARIANT_NO_CTB;
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled = FBK_TRUE;

   /** \action call function */
   result = Cta_Is_Rear_Enabled(&(cta_input.bmw_ctb_coding_parameters), p_vehicle_data);

   /** \assert expect false enable signal */
   EXPECT_FALSE(result);
}

/**
 * Check function determining if cta rear is enabled runs correctly. Here reverse gear is choosen, expect false
 * \uts{CSCSA-85510} \sdd{SF-4050} \testtype{positive}
 */
TEST_F(Cta_Bmw_Sp25_Warn_State_Test, Cta_Is_Rear_Enabled__reverse_gear)
{
   /** \arrange output with reverse ger */
   boolean_T result;
   p_vehicle_data->prndl                             = PA_VEH_PRNDL_STATE_REVERSE;
   cta_input.bmw_ctb_coding_parameters.c_ctb_variant = BMW_CTB_VARIANT_CTB_REAR_AND_FRONT;
   cta_input.bmw_ctb_coding_parameters.c_ctb_enabled = FBK_TRUE;

   /** \action call function */

   result = Cta_Is_Rear_Enabled(&(cta_input.bmw_ctb_coding_parameters), p_vehicle_data);
   /** \assert expect true enable signal */

   EXPECT_TRUE(result);
}