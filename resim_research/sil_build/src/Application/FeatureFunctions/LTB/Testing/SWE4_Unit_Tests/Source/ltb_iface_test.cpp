/**
 * @file ltb_iface_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for ltb_iface.c functions
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-46113}
 */

#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_iface.c"
#include "fbk_iface.h"
#include "fbk_macros.h"
#include "ltb_core_output_t.h"
#include "ltb_iface.c"
#include "ltb_iface_test.hpp"
#include "ltb_input_t.h"
#include "ltb_output_t.h"
#include "ltb_public_calibration.h"
#include "ltb_types.h"
#include "pa_reuse.h"
}

/**
 * Call LTB init function. Verify that function initializes LTB core output properly.
 * \uts{CSCSA-46143} \sdd{CSCSA-216614} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Iface_Test, Ltb_Init_Platform__initializes_ltb_core_output)
{
   /** \arrange Set up LTB core output with non-default values. */
   Sfl_Status_T result;

   /** \action Call Ltb_Init to init the LTB feature. */
   result = Ltb_Init_Platform(&ltb_instance);

   /** \assert Verify that LTB core output are initialized to default values. */
   EXPECT_EQ(result, SFL_STATUS_OK);
}

/**
 * Call LTB main function. Verify that after the run the core output is filled with the default output, if no critical tracker
 * objects are present. \uts{CSCSA-46144} \sdd{CSCSA-216615} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Iface_Test, Ltb_Run_Platform__check_that_core_output_is_default_if_no_objects_present)
{
   /** \arrange Set up core output with non-default values. Make sure that no critical tracker objects are present. Also initialize
    * the feature. */
   Ltb_Core_Output_T *p_ltb_core_output               = &ltb_instance.core_output;
   p_ltb_core_output->ltb_alert_level[FBK_SIDE_LEFT]  = ALERT_ACTIVE_LEVEL_2;
   p_ltb_core_output->ltb_alert_level[FBK_SIDE_RIGHT] = ALERT_ACTIVE_LEVEL_1;

   /** \action Call Ltb_Run to run the LTB feature. */
   Ltb_Run_Platform(&ltb_instance, &ltb_input, &fbk_output, &ltb_output);

   /** \assert Verify that LTB core output is filled with default values. */
   EXPECT_EQ(p_ltb_core_output->ltb_alert_level[FBK_SIDE_LEFT], NO_ALERT);
   EXPECT_EQ(p_ltb_core_output->ltb_alert_level[FBK_SIDE_RIGHT], NO_ALERT);
}

/*
 * Tests the main function of the LTB when one input value is out of the specified bounds. All outputs should be reset.
 * \uts{CSCSA-46145} \sdd{CSCSA-216615} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Iface_Test, Ltb_Run_Platform__input_out_of_bounds)
{
   /** \arrange Set alert states to level 4 and set input value out of bounds. */
   Ltb_Core_Output_T *p_ltb_core_output               = &ltb_instance.core_output;
   p_ltb_core_output->ltb_alert_level[FBK_SIDE_LEFT]  = ALERT_ACTIVE_LEVEL_3;
   p_ltb_core_output->ltb_alert_level[FBK_SIDE_RIGHT] = ALERT_ACTIVE_LEVEL_3;

   ltb_input.f_ltb_enable = 100u;

   /** \action Run LTB main function */
   Ltb_Run_Platform(&ltb_instance, &ltb_input, &fbk_output, &ltb_output);

   /** \assert Verify that alert states have value NO ALERT. */
   EXPECT_EQ(NO_ALERT, p_ltb_core_output->ltb_alert_level[FBK_SIDE_LEFT]);
   EXPECT_EQ(NO_ALERT, p_ltb_core_output->ltb_alert_level[FBK_SIDE_RIGHT]);
}

/*
 * Tests the main function of the LTB when inputs are null pointers. All outputs should be reset.
 * \uts{CSCSA-46146} \sdd{CSCSA-216615} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Iface_Test, Ltb_Run_Platform__null_pointers_provided)
{
   /** \arrange Set alert states to level 4 and set input value out of bounds. */
   Ltb_Input_T *p_ltb_input = NULL;

   /** \action Run LTB main function */
   Ltb_Run_Platform(&ltb_instance, p_ltb_input, &fbk_output, &ltb_output);

   /** \assert Verify that alert states have value NO ALERT. */
   EXPECT_EQ(NULL, p_ltb_input);
}

/**
 * Tests the interface function for the software major version. The major software version shall be equal to the internal one.
 * \uts{CSCSA-46149} \sdd{CSCSA-53922} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Iface_Test, Ltb_Get_Sw_Major_Version__returns_correct_major_version)
{
   /** \arrange Declare result variable. */
   uint16_t major_version;

   /** \action Fill result variable with major version. */
   major_version = Ltb_Get_Sw_Major_Version();

   /** \assert Verify that resulting major version equals persistent major version value. */
   EXPECT_EQ(major_version, Ltb_Sw_Major_Version);
}

/**
 * Tests the interface function for the software minor version. The minor software version shall be equal to the internal one.
 * \uts{CSCSA-46150} \sdd{CSCSA-53923} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Iface_Test, Ltb_Get_Sw_Minor_Version__returns_correct_minor_version)
{
   /** \arrange Declare result variable. */
   uint16_t minor_version;

   /** \action Fill result variable with minor version. */
   minor_version = Ltb_Get_Sw_Minor_Version();

   /** \assert Verify that resulting minor version equals persistent minor version value. */
   EXPECT_EQ(minor_version, Ltb_Sw_Minor_Version);
}

/**
 * Test that no null pointer is detected if pointer are valid.
 * \uts{CSCSA-46154} \sdd{CSCSA-53908} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Iface_Test, Ltb_Null_Pointer_Detected__returns_false_for_valid_pointer)
{
   /** \arrange Set up valid pointers to context and cal data. */
   boolean_T result = FBK_TRUE;

   /** \action Call Ltb_Null_Pointer_Detected to check for null pointer. */
   result = Ltb_Null_Pointer_Detected(&ltb_instance, &ltb_input, &fbk_output, &ltb_output);

   /** \assert Verify that no NULL pointer is detected. */
   EXPECT_FALSE(result);
}


/**
 * Test that null ltb instance pointer return false.
 * \uts{} \sdd{CSCSA-53908} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Iface_Test, Ltb_Null_Pointer_Detected__returns_false_for_invalid_ltb_instance)
{
   /** \arrange Set up result. */
   boolean_T result = FBK_TRUE;

   /** \action Call Ltb_Null_Pointer_Detected to check for null pointer. */
   result = Ltb_Null_Pointer_Detected(NULL, &ltb_input, &fbk_output, &ltb_output);

   /** \assert Verify that NULL pointer is detected. */
   EXPECT_TRUE(result);
}

/**
 * Test that null fbk_output pointer return false.
 * \uts{} \sdd{CSCSA-53908} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Iface_Test, Ltb_Null_Pointer_Detected__returns_true_for_invalid_fbk_output_pointer)
{
   /** \arrange Set up result. */
   boolean_T result = FBK_TRUE;

   /** \action Call Ltb_Null_Pointer_Detected to check for null pointer. */
   result = Ltb_Null_Pointer_Detected(&ltb_instance, &ltb_input, NULL, &ltb_output);

   /** \assert Verify that NULL pointer is detected. */
   EXPECT_TRUE(result);
}

/**
 * Test that null ltb output pointer return false.
 * \uts{} \sdd{CSCSA-53908} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Iface_Test, Ltb_Null_Pointer_Detected__returns_true_for_invalid_ltb_output)
{
   /** \arrange Set up result. */
   boolean_T result = FBK_TRUE;

   /** \action Call Ltb_Null_Pointer_Detected to check for null pointer. */
   result = Ltb_Null_Pointer_Detected(&ltb_instance, &ltb_input, &fbk_output, NULL);

   /** \assert Verify that NULL pointer is detected. */
   EXPECT_TRUE(result);
}


/**
 * Test that calibration updating workflow works properly in case of valid cal pointer.
 * \uts{CSCSA-185804} \sdd{CSCSA-122669} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Iface_Test, Ltb_Update_Calibration_Platform__valid_cal_ptr)
{
   /** \arrange Set valid calibration pointer. */
   Ltb_Instance_T ltb_inst{};

   Ltb_Public_Calibration_T public_cal;
   Ltb_Public_Cal_Update_Defaults(&public_cal);

   boolean_T result;

   /** \action Call function to update calibration values. */
   result = Ltb_Update_Calibration_Platform(&ltb_inst, &public_cal);

   /** \assert Verify that function returns true in case of valid cal ptr. */
   EXPECT_TRUE(result);
}


/**
 * Test that calibration updating doesn't work in case of invalid cal pointer.
 * \uts{CSCSA-185805} \sdd{CSCSA-122669} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Iface_Test, Ltb_Update_Calibration_Platform__null_cal_ptr)
{
   /** \arrange Set null calibration pointer. */
   Ltb_Instance_T ltb_inst{};
   const Ltb_Public_Calibration_T *p_ltb_calibration = NULL;
   boolean_T result;

   /** \action Call function to update calibration values. */
   result = Ltb_Update_Calibration_Platform(&ltb_inst, p_ltb_calibration);

   /** \assert Verify that function returns false in case of invalid cal ptr. */
   EXPECT_FALSE(result);
}
