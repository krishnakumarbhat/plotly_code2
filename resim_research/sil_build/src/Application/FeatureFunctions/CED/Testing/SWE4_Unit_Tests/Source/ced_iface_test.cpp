/**
 * @file ced_iface_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for ced_iface.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-41453}
 */

#include "ced_iface_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "ced_core_output_t.h"
#include "ced_iface.c"
#include "ced_iface_test.hpp"
#include "ced_input_t.h"
#include "ced_output_t.h"
#include "ced_public_calibration.h"
#include "ced_types.h"
#include "fbk_iface.c"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "pt_iface.c"
}

/**
 * Call CED init function. Verify that function initializes CED core output properly.
 * \uts{CSCSA-41454} \sdd{CSCSA-186572} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Iface_Test, Ced_Init_Platform__initializes_ced_core_output)
{
   /** \arrange Set up CED core output with non-default values. */
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]  = CED_ALERT_ACTIVE_LEVEL_2;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT] = CED_ALERT_ACTIVE_LEVEL_1;

   /** \action Call Ced_Init_Platform to init the CED feature. */
   Ced_Init_Platform(&ced_instance);

   /** \assert Verify that CED core output are initialized to default values. */
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT], CED_NO_ALERT);
}

/**
 * Call CED main function. Verify that after the run the core output is filled with the default output, if no critical tracker
 * objects are present. \uts{CSCSA-41455} \sdd{CSCSA-186570} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Iface_Test, Ced_Run_Platform__check_that_core_output_is_default_if_no_objects_present)
{
   /** \arrange Set up core output with non-default values. Make sure that no critical tracker objects are present. */
   Ced_Init_Platform(&ced_instance);
   ced_instance.core_output.ced_alert[FBK_SIDE_LEFT]  = CED_ALERT_ACTIVE_LEVEL_2;
   ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT] = CED_ALERT_ACTIVE_LEVEL_1;

   /** \action Call Ced_Run_Platform to run the CED feature. */
   Ced_Run_Platform(&ced_instance, &ced_input, &ced_output, &fbk_output, &pt_output);

   /** \assert Verify that CED core output is filled with default values. */
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_LEFT], CED_NO_ALERT);
   EXPECT_EQ(ced_instance.core_output.ced_alert[FBK_SIDE_RIGHT], CED_NO_ALERT);
}

/**
 * Tests the interface function for the software major version. The major software version shall be equal to the internal one.
 * \uts{CSCSA-41458} \sdd{SF-3629} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Iface_Test, Ced_Get_Sw_Major_Version__returns_correct_major_version)
{
   /** \arrange Declare result variable. */
   uint16_t major_version;

   /** \action Fill result variable with major version. */
   major_version = Ced_Get_Sw_Major_Version();

   /** \assert Verify that resulting major version equals persistent major version value. */
   EXPECT_EQ(major_version, Ced_Sw_Major_Version);
}

/**
 * Tests the interface function for the software minor version. The minor software version shall be equal to the internal one.
 * \uts{CSCSA-41459} \sdd{SF-3630} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Iface_Test, Ced_Get_Sw_Minor_Version__returns_correct_minor_version)
{
   /** \arrange Declare result variable. */
   uint16_t minor_version;

   /** \action Fill result variable with minor version. */
   minor_version = Ced_Get_Sw_Minor_Version();

   /** \assert Verify that resulting minor version equals persistent minor version value. */
   EXPECT_EQ(minor_version, Ced_Sw_Minor_Version);
}

/**
 * Call CED main function. Verify that functions is not running if the core output is filled with null pointers.
 * \uts{CSCSA-41463} \sdd{CSCSA-186570} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Iface_Test, Ced_Run_Platform__no_run_with_null_pointers)
{
   /** \arrange Set up core input as null and false values. Calibration pointer is null. */
   Ced_Init_Platform(&ced_instance);
   /** \action Call Ced_Run_Platform to run the CED feature. */
   Ced_Run_Platform(&ced_instance, nullptr, nullptr, nullptr, nullptr);
   /** \assert Verify that signals are not overwritten. */
   EXPECT_FALSE(ced_instance.core_input.p_pa_data);
   EXPECT_FALSE(ced_instance.core_input.f_ced_front_mode);
   EXPECT_FALSE(ced_instance.core_input.f_ced_rear_mode);
}

/**
 * Test that no null pointer is detected if pointer are valid.
 * \uts{CSCSA-41464} \sdd{SF-3613} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Iface_Test, Ced_Null_Pointer_Detected__returns_false_for_valid_pointer)
{
   /** \arrange Set up valid pointers to context and cal data. */
   boolean_T result = FBK_TRUE;
   Ced_Init_Platform(&ced_instance);

   /** \action Call Ced_Null_Pointer_Detected to check for null pointer. */
   result = Ced_Null_Pointer_Detected(&ced_instance, &fbk_output, &ced_input, &ced_output);

   /** \assert Verify that no NULL pointer is detected. */
   EXPECT_FALSE(result);
}

/**
 * Test that null calibration pointer detected.
 * \uts{CSCSA-185575} \sdd{SF-3613} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Iface_Test, Ced_Null_Pointer_Detected__null_ced_instance)
{
   /** \arrange Set up calibration as null. */
   boolean_T result = FBK_TRUE;
   /** \action Call Ced_Null_Pointer_Detected to check for null pointer. */
   result = Ced_Null_Pointer_Detected(nullptr, &fbk_output, &ced_input, &ced_output);

   /** \assert Verify that no NULL pointer is detected. */
   EXPECT_TRUE(result);
}

/**
 * Test that null fbk_output detected.
 * \uts{CSCSA-185576} \sdd{SF-3613} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Iface_Test, Ced_Null_Pointer_Detected__null_fbk_output)
{
   /** \arrange Set up calibration as null. */
   boolean_T result = FBK_TRUE;
   Ced_Init_Platform(&ced_instance);
   /** \action Call Ced_Null_Pointer_Detected to check for null pointer. */
   result = Ced_Null_Pointer_Detected(&ced_instance, nullptr, &ced_input, &ced_output);

   /** \assert Verify that NULL pointers are detected. */
   EXPECT_TRUE(result);
}

/**
 * Test that null ced_input detected.
 * \uts{CSCSA-185577} \sdd{SF-3613} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Iface_Test, Ced_Null_Pointer_Detected__null_ced_input)
{
   /** \arrange Set up calibration as null. */
   boolean_T result = FBK_TRUE;
   Ced_Init_Platform(&ced_instance);
   /** \action Call Ced_Null_Pointer_Detected to check for null pointer. */
   result = Ced_Null_Pointer_Detected(&ced_instance, &fbk_output, nullptr, &ced_output);

   /** \assert Verify that NULL pointers are detected. */
   EXPECT_TRUE(result);
}

/**
 * Test that null ced_output detected.
 * \uts{CSCSA-185578} \sdd{SF-3613} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Iface_Test, Ced_Null_Pointer_Detected__null_ced_output)
{
   /** \arrange Set up calibration as null. */
   boolean_T result = FBK_TRUE;
   Ced_Init_Platform(&ced_instance);
   /** \action Call Ced_Null_Pointer_Detected to check for null pointer. */
   result = Ced_Null_Pointer_Detected(&ced_instance, &fbk_output, &ced_input, nullptr);

   /** \assert Verify that NULL pointers are detected. */
   EXPECT_TRUE(result);
}

/**
 * Tests that calibration is updated correctly.
 * \uts{CSCSA-185579} \sdd{CSCSA-186571} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Iface_Test, Ced_Update_Calibration_Platform__check_returned_flag_test)
{
   /** \arrange declare variable for result and simple imput */
   boolean_T result;

   Ced_Public_Calibration_T public_cal;
   Ced_Public_Cal_Update_Defaults(&public_cal);


   public_cal.k_ced_alert_holding_cycles = 10u;

   Ced_Init_Platform(&ced_instance);

   /** \action call calibration update */
   result = Ced_Update_Calibration_Platform(&ced_instance, &public_cal);

   /** \assert expect succes */
   EXPECT_TRUE(result);
}

/**
 * Tests that calibration is updated incorrectly.
 * \uts{CSCSA-185580} \sdd{CSCSA-186571} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Iface_Test, Ced_Update_Calibration_Platform_calibration_null_pointer)
{
   /** \arrange declare calibration null pointer */
   boolean_T result;
   Ced_Init_Platform(&ced_instance);

   /** \action call calibration update */
   result = Ced_Update_Calibration_Platform(&ced_instance, nullptr);

   /** \assert expect false */
   EXPECT_FALSE(result);
}
