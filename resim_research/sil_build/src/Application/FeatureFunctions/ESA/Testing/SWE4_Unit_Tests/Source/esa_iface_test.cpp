/**
 * @file esa_iface_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for esa_iface.c functions
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-123884}
 */

#include "esa_iface_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "esa_core_calibration.h"
#include "esa_iface.c"
#include "esa_public_calibration.h"
#include "fbk_iface.c"
#include "fbk_macros.h"
#include "pa_reuse.h"
}

/**
 * Initialize ESA, no exception shall be thrown.
 * \uts{CSCSA-185725} \sdd{CSCSA-216521} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Iface_Test, Esa_Init_Platform__No_Fatal_Error_occurs)
{
   /** \arrange Not applicable. */
   /** \action Not applicable. */
   /** \assert Call Esa_Init and check that no exception is thrown. */
   EXPECT_NO_FATAL_FAILURE(Esa_Init_Platform(&esa_instance););
}


/**
 * Call ESA main function, no exception shall be thrown.
 * \uts{CSCSA-185726} \sdd{CSCSA-216522} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Iface_Test, Esa_Run_Platform__No_Fatal_Error_occurs)
{
   /** \arrange Not applicable. */
   /** \action Not applicable. */
   /** \assert Call Esa_Run and check that no exception is thrown. */
   EXPECT_NO_FATAL_FAILURE(Esa_Run_Platform(&esa_instance, &esa_input, &fbk_output, &esa_output););
}


/**
 * Call ESA main function. Verify that after the run the core output is filled with the default output, if no critical tracker
 * objects are present. \uts{CSCSA-185727} \sdd{CSCSA-216522} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Iface_Test, Esa_Run_Platform__core_output_not_updated_if_input_pointer_is_NULL)
{
   /** \arrange Set up core output with non-default values. Make sure that no critical tracker objects are present. */
   Esa_Init_Platform(&esa_instance);
   esa_instance.core_output.esa_alert[FBK_SIDE_LEFT]  = true;
   esa_instance.core_output.esa_alert[FBK_SIDE_RIGHT] = true;

   /** \action Call Esa_Run to run the ESA feature. */
   Esa_Run_Platform(&esa_instance, NULL, &fbk_output, &esa_output);

   /** \assert Verify that ESA core output is filled with default values. */
   EXPECT_TRUE(esa_instance.core_output.esa_alert[FBK_SIDE_LEFT]);
   EXPECT_TRUE(esa_instance.core_output.esa_alert[FBK_SIDE_RIGHT]);
}


/**
 * Call ESA main function. Verify that after the run the core output is filled with the default output, if no critical tracker
 * objects are present. \uts{CSCSA-185728} \sdd{CSCSA-216522} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Iface_Test, Esa_Run_Platform__check_that_core_output_is_default_if_no_objects_present)
{
   /** \arrange Set up core output with non-default values. Make sure that no critical tracker objects are present. */
   Esa_Init_Platform(&esa_instance);
   esa_instance.core_output.esa_alert[FBK_SIDE_LEFT]  = true;
   esa_instance.core_output.esa_alert[FBK_SIDE_RIGHT] = true;

   /** \action Call Esa_Run to run the ESA feature. */
   Esa_Run_Platform(&esa_instance, &esa_input, &fbk_output, &esa_output);

   /** \assert Verify that ESA core output is filled with default values. */
   EXPECT_FALSE(esa_instance.core_output.esa_alert[FBK_SIDE_LEFT]);
   EXPECT_FALSE(esa_instance.core_output.esa_alert[FBK_SIDE_RIGHT]);
}


/**
 * Get ESAs major version number and check that it is correct.
 * \uts{CSCSA-123891} \sdd{CSCSA-66000} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Iface_Test, Esa_Get_Sw_Major_Version__Get_Esa_Sw_Major_Version_returns_correct_major_version)
{
   /** \arrange Declare version variable. */
   uint16_t major_version;

   /** \action Call Esa_Get_Sw_Major_Version to obtain version. */
   major_version = Esa_Get_Sw_Major_Version();

   /** \assert Check that major version is filled correctly. */
   EXPECT_EQ(major_version, Esa_Sw_Major_Version);
}

/**
 * Get ESAs minor version number and check that it is correct.
 * \uts{CSCSA-123892} \sdd{CSCSA-66001} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Iface_Test, Esa_Get_Sw_Minor_Version__Get_Esa_Sw_Minor_Version_returns_correct_minor_version)
{
   /** \arrange Declare version variable. */
   uint16_t minor_version;

   /** \action Call Esa_Get_Sw_Minor_Version to obtain version. */
   minor_version = Esa_Get_Sw_Minor_Version();

   /** \assert Check that minor version is filled correctly. */
   EXPECT_EQ(minor_version, Esa_Sw_Minor_Version);
}

/**
 * Test that calibration updating workflow works properly in case of valid cal pointer.
 * \uts{CSCSA-123894} \sdd{CSCSA-216523} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Iface_Test, Esa_Update_Calibration_Platform__valid_cal_ptr)
{
   /** \arrange Set valid calibration pointer. */
   boolean_T result;

   Esa_Public_Calibration_T public_cal;
   Esa_Public_Cal_Update_Defaults(&public_cal);

   esa_instance.calibration.k_esa_host_activation_speed_min = 12.3f;
   public_cal.k_esa_host_activation_speed_min               = 10.0f;

   /** \action Call function to update calibration values. */
   result = Esa_Update_Calibration_Platform(&esa_instance, &public_cal);

   /** \assert Verify that function returns true in case of valid cal ptr. */
   EXPECT_TRUE(result);
   EXPECT_FLOAT_EQ(esa_instance.calibration.k_esa_host_activation_speed_min, p_esa_calibration->k_esa_host_activation_speed_min);
}

/**
 * Test that calibration is not updated in case of null cal pointer.
 * \uts{CSCSA-209331} \sdd{CSCSA-216523} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Iface_Test, Esa_Update_Calibration_Platform__null_cal_ptr)
{
   /** \arrange */
   boolean_T result;
   float expected_value                                     = 12.3f;
   esa_instance.calibration.k_esa_host_activation_speed_min = expected_value;

   /** \action Call function to update calibration values. */
   result = Esa_Update_Calibration_Platform(&esa_instance, nullptr);

   /** \assert Verify that function returns false and calibration remains unchanged in case of null cal ptr. */
   EXPECT_FALSE(result);
   EXPECT_FLOAT_EQ(esa_instance.calibration.k_esa_host_activation_speed_min, expected_value);
}

/**
 * Test that calibration is not updated in case of null instance pointer.
 * \uts{CSCSA-209332} \sdd{CSCSA-216523} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Iface_Test, Esa_Update_Calibration_Platform__null_instance_ptr)
{
   /** \arrange */
   boolean_T result;

   Esa_Public_Calibration_T public_cal;
   Esa_Public_Cal_Update_Defaults(&public_cal);

   /** \action Call function to update calibration values. */
   result = Esa_Update_Calibration_Platform(nullptr, &public_cal);

   /** \assert Verify that function returns false in case of null instance ptr. */
   EXPECT_FALSE(result);
}

/**
 * Test that Esa_Update_Calibration updates cals correctly.
 * \uts{CSCSA-185735} \sdd{CSCSA-216523} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Iface_Test, Esa_Update_Calibration_Platform__updates_cals_correctly)
{
   /** \arrange Set desired cal value. */
   p_esa_calibration->k_esa_min_track_age = 0u;

   Esa_Public_Calibration_T public_cal;
   Esa_Public_Cal_Update_Defaults(&public_cal);

   public_cal.k_esa_min_track_age = 1u;
   boolean_T result;

   /** \action Update persistent cal values */
   result = Esa_Update_Calibration_Platform(&esa_instance, &public_cal);

   /** \assert Verify that persistent cal value equals desired cal value. */
   EXPECT_TRUE(result);
   EXPECT_EQ(p_esa_calibration->k_esa_min_track_age, public_cal.k_esa_min_track_age);
}


/**
 * Test that no null pointer is detected if pointer are valid.
 * \uts{CSCSA-123896} \sdd{CSCSA-66535} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Iface_Test, Esa_Null_Pointer_Detected__returns_false_for_valid_pointer)
{
   /** \arrange Not applicable. */
   boolean_T detected;

   /** \action Call function */
   detected = Esa_Null_Pointer_Detected(&esa_instance, &esa_input, &fbk_output, &esa_output);

   /** \assert Verify that false if no NULL pointer is detected. */
   EXPECT_FALSE(detected);
}

/**
 * Test that null pointer is detected if p_esa_instance pointer is not valid.
 * \uts{CSCSA-185730} \sdd{CSCSA-66535} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Iface_Test, Esa_Null_Pointer_Detected__returns_true_for_null_instance_pointer)
{
   /** \arrange Not applicable. */
   boolean_T detected;

   /** \action Call function */
   detected = Esa_Null_Pointer_Detected(NULL, &esa_input, &fbk_output, &esa_output);

   /** \assert Verify that true if p_esa_instance pointer is NULL. */
   EXPECT_TRUE(detected);
}


/**
 * Test that null pointer is detected if p_esa_input pointer is not valid.
 * \uts{CSCSA-185731} \sdd{CSCSA-66535} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Iface_Test, Esa_Null_Pointer_Detected__returns_true_for_null_input_pointer)
{
   /** \arrange Not applicable. */
   boolean_T detected;

   /** \action Call function */
   detected = Esa_Null_Pointer_Detected(&esa_instance, NULL, &fbk_output, &esa_output);

   /** \assert Verify that true if p_esa_input pointer is NULL. */
   EXPECT_TRUE(detected);
}


/**
 * Test that null pointer is detected if p_fbk_output pointer is not valid.
 * \uts{CSCSA-185732} \sdd{CSCSA-66535} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Iface_Test, Esa_Null_Pointer_Detected__returns_true_for_null_fbk_output_pointer)
{
   /** \arrange Not applicable. */
   boolean_T detected;

   /** \action Call function */
   detected = Esa_Null_Pointer_Detected(&esa_instance, &esa_input, NULL, &esa_output);

   /** \assert Verify that true if p_fbk_output pointer is NULL. */
   EXPECT_TRUE(detected);
}


/**
 * Test that null pointer is detected if p_esa_output pointer is not valid.
 * \uts{CSCSA-185733} \sdd{CSCSA-66535} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Iface_Test, Esa_Null_Pointer_Detected__returns_true_for_null_output_pointer)
{
   /** \arrange Not applicable. */
   boolean_T detected;

   /** \action Call function */
   detected = Esa_Null_Pointer_Detected(&esa_instance, &esa_input, &fbk_output, NULL);

   /** \assert Verify that true if p_esa_output pointer is NULL. */
   EXPECT_TRUE(detected);
}
