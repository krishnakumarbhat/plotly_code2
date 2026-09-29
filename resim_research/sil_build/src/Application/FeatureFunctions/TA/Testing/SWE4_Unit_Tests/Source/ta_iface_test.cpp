/**
 * @file ta_iface_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for TA unit tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44832}
 */

#include "ta_iface_test.hpp"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_iface.c"
#include "fbk_iface.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "Radar_Config.h"
#include "ta_constants.h"
#include "ta_core_calibration.h"
#include "ta_core_output_t.h"
#include "ta_iface.c"
#include "ta_output_t.h"
#include "ta_public_calibration.h"
#include "ta_types.h"
}
using ::testing::Eq;


/**
 * Test that calibration updating workflow works properly in case of valid cal pointer.
 * \uts{CSCSA-185986} \sdd{CSCSA-122773} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Iface_Test, Ta_Update_Calibration_Platform__valid_cal_ptr)
{
   /** \arrange Set valid calibration pointer. */
   Ta_Init_Platform(&ta_instance);

   Ta_Public_Calibration_T public_cal;
   Ta_Public_Cal_Update_Defaults(&public_cal);

   boolean_T result;

   /** \action Call function to update calibration values. */
   result = Ta_Update_Calibration_Platform(&ta_instance, &public_cal);

   /** \assert Verify that function returns true in case of valid cal ptr. */
   EXPECT_TRUE(result);
}


/**
 * Test that calibration updating doesn't work in case of invalid cal pointer.
 * \uts{CSCSA-185987} \sdd{CSCSA-122773} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Iface_Test, Ta_Update_Calibration_Platform__null_cal_ptr)
{
   /** \arrange Set null calibration pointer. */
   const Ta_Public_Calibration_T *p_ta_calibration = NULL;
   boolean_T result;

   /** \action Call function to update calibration values. */
   result = Ta_Update_Calibration_Platform(&ta_instance, p_ta_calibration);

   /** \assert Verify that function returns false in case of invalid cal ptr. */
   EXPECT_FALSE(result);
}


/*
 * Tests the update procedure of the Turn Assist feature for a configuration where the TA is running on the front left sensor. The
 * calibrations shall be initialized. \uts{CSCSA-44842} \sdd{SF-8717} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Iface_Test, Ta_Init__ta_initializes_cals_correctly)
{
   /** \arrange */
   ta_instance.calibration.k_fta_brake_gradient = 0;
   /** \action Initialize TA with desired mounting position front left */
   Ta_Init_Platform(&ta_instance);

   /** \assert Verify that persistent cal value equals the expected calibrations */
   EXPECT_EQ(-40.f, ta_instance.calibration.k_fta_brake_gradient);
}

/*
 * Tests the main function of the Turn Assist.
 * \uts{CSCSA-185988} \sdd{SF-8718} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Iface_Test, Ta_Run__ta_instance_is_null)
{
   /** \arrange */
   /** \action Run TA main function */
   /** \assert Verify */
   ASSERT_NO_FATAL_FAILURE({ Ta_Run_Platform(nullptr, &ta_input, &fbk_output, &ta_output); });
}

/*
 * Tests the main function of the Turn Assist.
 * \uts{CSCSA-185989} \sdd{SF-8718} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Iface_Test, Ta_Run__ta_input_is_null)
{
   /** \arrange */
   /** \action Run TA main function */
   /** \assert Verify */
   ASSERT_NO_FATAL_FAILURE({ Ta_Run_Platform(&ta_instance, nullptr, &fbk_output, &ta_output); });
}

/*
 * Tests the main function of the Turn Assist.
 * \uts{CSCSA-185990} \sdd{SF-8718} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Iface_Test, Ta_Run__fbk_output_is_null)
{
   /** \arrange */
   /** \action Run TA main function */
   /** \assert Verify */
   ASSERT_NO_FATAL_FAILURE({ Ta_Run_Platform(&ta_instance, &ta_input, nullptr, &ta_output); });
}

/*
 * Tests the main function of the Turn Assist.
 * \uts{CSCSA-185991} \sdd{SF-8718} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Iface_Test, Ta_Run__ta_output_is_null)
{
   /** \arrange */
   /** \action Run TA main function */
   /** \assert Verify */
   ASSERT_NO_FATAL_FAILURE({ Ta_Run_Platform(&ta_instance, &ta_input, &fbk_output, nullptr); });
}

/*
 * Tests the main function of the Turn Assist. Since no object information is available, the arranged alert states shall be reset
 * to the alert state none. \uts{CSCSA-44835} \sdd{SF-8718} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Iface_Test, Ta_Run__check_ta_output)
{
   /** \arrange Set alert states to level 4. */
   Ta_Init_Platform(&ta_instance);
   ta_instance.core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_LEVEL_4;
   ta_instance.core_output.ta_alert_level[FBK_SIDE_RIGHT] = TA_ALERT_STATE_LEVEL_4;

   /** \action Run TA main function */
   Ta_Run_Platform(&ta_instance, &ta_input, &fbk_output, &ta_output);

   /** \assert Verify that alert states have value TA_ALERT_STATE_NONE. */
   EXPECT_EQ(TA_ALERT_STATE_NONE, ta_instance.core_output.ta_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(TA_ALERT_STATE_NONE, ta_instance.core_output.ta_alert_level[FBK_SIDE_RIGHT]);
}

/*
 * Tests the main function of the Turn Assist when one calibration value is out of the specified bounds. All outputs should be
 * reset. \uts{CSCSA-44843} \sdd{SF-8718} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Iface_Test, Ta_Run__calibration_out_of_bounds)
{
   /** \arrange Set alert states to level 4 and set cal value out of bounds. */
   Ta_Init_Platform(&ta_instance);
   ta_instance.core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_LEVEL_4;
   ta_instance.core_output.ta_alert_level[FBK_SIDE_RIGHT] = TA_ALERT_STATE_LEVEL_4;

   ta_instance.calibration.k_fta_brake_deceleration_max = 100.0f;

   /** \action Run TA main function */
   Ta_Run_Platform(&ta_instance, &ta_input, &fbk_output, &ta_output);

   /** \assert Verify that alert states have value TA_ALERT_STATE_NONE. */
   EXPECT_EQ(TA_ALERT_STATE_NONE, ta_instance.core_output.ta_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(TA_ALERT_STATE_NONE, ta_instance.core_output.ta_alert_level[FBK_SIDE_RIGHT]);
   EXPECT_EQ(100.0f, ta_instance.calibration.k_fta_brake_deceleration_max);
}

/*
 * Tests the main function of the Turn Assist when one input value is out of the specified bounds. All outputs should be reset.
 * \uts{CSCSA-44844} \sdd{SF-8718} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Iface_Test, Ta_Run__input_out_of_bounds)
{
   /** \arrange Set alert states to level 4 and set input value out of bounds. */
   Ta_Init_Platform(&ta_instance);
   ta_instance.core_output.ta_alert_level[FBK_SIDE_LEFT]  = TA_ALERT_STATE_LEVEL_4;
   ta_instance.core_output.ta_alert_level[FBK_SIDE_RIGHT] = TA_ALERT_STATE_LEVEL_4;

   ta_input.f_fta_enable = 100u;

   /** \action Run TA main function */
   Ta_Run_Platform(&ta_instance, &ta_input, &fbk_output, &ta_output);

   /** \assert Verify that alert states have value TA_ALERT_STATE_NONE. */
   EXPECT_EQ(TA_ALERT_STATE_NONE, ta_instance.core_output.ta_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(TA_ALERT_STATE_NONE, ta_instance.core_output.ta_alert_level[FBK_SIDE_RIGHT]);
}

/*
 * Tests the interface function for the software major version. The major software version shall be equal to the internal one.
 * \uts{CSCSA-44838} \sdd{SF-8715} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Iface_Test, Ta_Get_Sw_Major_Version__returns_correct_major_version)
{
   /** \arrange Declare result variable. */
   uint16_t major_version;

   /** \action Fill result variable with major version */
   major_version = Ta_Get_Sw_Major_Version();

   /** \assert Verify that resulting major version equals persistent major version value. */
   EXPECT_EQ(major_version, Ta_Sw_Major_Version);
}

/*
 * Tests the interface function for the software minor version. The minor software version shall be equal to the internal one.
 * \uts{CSCSA-44839} \sdd{SF-8716} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Iface_Test, Ta_Get_Sw_Minor_Version__returns_correct_minor_version)
{
   /** \arrange Declare result variable. */
   uint16_t minor_version;

   /** \action Fill result variable with minor version */
   minor_version = Ta_Get_Sw_Minor_Version();

   /** \assert Verify that resulting minor version equals persistent minor version value. */
   EXPECT_EQ(minor_version, Ta_Sw_Minor_Version);
}
