/**
 * @file scw_iface_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for SCW unit tests
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44450}
 */

#include "scw_iface_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_iface.c"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "scw_core_calibration.h"
#include "scw_iface.c"
#include "scw_public_calibration.h"
}

/**
 * Initialize SCW, no exception shall be thrown.
 * \uts{CSCSA-185963} \sdd{CSCSA-186568} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Iface_Test, Scw_Init_Platform__No_Fatal_Error_occurs)
{
   /** \arrange Not applicable. */
   /** \action Not applicable. */
   /** \assert Call Scw_Init and check that no exception is thrown. */
   EXPECT_NO_FATAL_FAILURE(Scw_Init_Platform(&scw_instance););
}

/**
 * Call SCW main function, no exception shall be thrown.
 * \uts{CSCSA-185964} \sdd{CSCSA-186566} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Iface_Test, Scw_Run_Platform__No_Fatal_Error_occurs)
{
   /** \arrange Not applicable. */
   /** \action Not applicable. */
   /** \assert Call Scw_Run and check that no exception is thrown. */
   EXPECT_NO_FATAL_FAILURE(Scw_Run_Platform(&scw_instance, &scw_input, &fbk_output, &scw_output););
}

/*
 * Tests whether null pointers are available. Here no null pointer shall be detected.
 * \uts{CSCSA-44451} \sdd{SF-8071} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Iface_Test, Scw_Null_Pointer_Detected__all_pointers_valid)
{
   /** \arrange Declare result variable. */
   boolean_T result;

   /** \action Call init function and check if null-pointers exist */
   result = Scw_Null_Pointer_Detected(&scw_instance, &scw_input, &fbk_output, &scw_output);

   /** \assert Verify that no null pointers were detected. */
   EXPECT_FALSE(result);
}

/*
 * Checks if NULL pointer is detected, when instance pointer is NULL
 * \uts{CSCSA-185965} \sdd{SF-8071} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Iface_Test, Scw_Null_Pointer_Detected__instance_pointer_null)
{
   /** \arrange Declare result variable */
   boolean_T result;

   /** \action Call init function and check if null-pointers exist */
   result = Scw_Null_Pointer_Detected(NULL, &scw_input, &fbk_output, &scw_output);

   /** \assert Verify that null pointers is detected. */
   EXPECT_TRUE(result);
}

/*
 * Checks if NULL pointer is detected, when input pointer is NULL
 * \uts{CSCSA-185966} \sdd{SF-8071} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Iface_Test, Scw_Null_Pointer_Detected__input_pointer_null)
{
   /** \arrange Declare result variable */
   boolean_T result;

   /** \action Call init function and check if null-pointers exist */
   result = Scw_Null_Pointer_Detected(&scw_instance, NULL, &fbk_output, &scw_output);

   /** \assert Verify that null pointers is detected. */
   EXPECT_TRUE(result);
}

/*
 * Checks if NULL pointer is detected, when fbk output pointer is NULL
 * \uts{CSCSA-185967} \sdd{SF-8071} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Iface_Test, Scw_Null_Pointer_Detected__fbk_output_pointer_null)
{
   /** \arrange Declare result variable */
   boolean_T result;

   /** \action Call init function and check if null-pointers exist */
   result = Scw_Null_Pointer_Detected(&scw_instance, &scw_input, NULL, &scw_output);

   /** \assert Verify that null pointers is detected. */
   EXPECT_TRUE(result);
}

/*
 * Checks if NULL pointer is detected, when output pointer is NULL
 * \uts{CSCSA-185968} \sdd{SF-8071} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Iface_Test, Scw_Null_Pointer_Detected__output_pointer_null)
{
   /** \arrange Declare result variable */
   boolean_T result;

   /** \action Call init function and check if null-pointers exist */
   result = Scw_Null_Pointer_Detected(&scw_instance, &scw_input, &fbk_output, NULL);

   /** \assert Verify that null pointers is detected. */
   EXPECT_TRUE(result);
}


/*
 * Tests the update of calibration value function. The cal shall be updated after the call of the function.
 * \uts{CSCSA-185970} \sdd{CSCSA-186567} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Iface_Test, Scw_Update_Calibration_Platform__update_cals)
{
   /** \arrange Set desired cal value. */
   p_scw_calibration->k_scw_min_candidate_age = (uint16_t) 0;

   Scw_Public_Calibration_T public_cal;
   Scw_Public_Cal_Update_Defaults(&public_cal);

   public_cal.k_scw_min_candidate_age = (uint16_t) 1;
   boolean_T result                   = FBK_FALSE;

   /** \action Update persistent cal values */
   result = Scw_Update_Calibration_Platform(&scw_instance, &public_cal);

   /** \assert Verify that persistent cal value equals desired cal value. */
   EXPECT_TRUE(result);
   EXPECT_EQ(p_scw_calibration->k_scw_min_candidate_age, 1);
}

/*
 * Tests the update of calibration value function. The cal shall not be updated if the public cal is out of range.
 * \uts{CSCSA-267749} \sdd{CSCSA-186567} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Iface_Test, Scw_Update_Calibration_Platform__not_update_cals_if_public_cal_out_of_range)
{
   /** \arrange Set desired cal value. */
   p_scw_calibration->k_scw_min_candidate_age = (uint16_t) 0;

   Scw_Public_Calibration_T public_cal;
   Scw_Public_Cal_Update_Defaults(&public_cal);

   public_cal.k_scw_min_candidate_age = SCW_MAX_K_SCW_MIN_CANDIDATE_AGE + 1u;
   boolean_T result;

   /** \action Update persistent cal values */
   result = Scw_Update_Calibration_Platform(&scw_instance, &public_cal);

   /** \assert Verify that persistent cal value equals desired cal value. */
   EXPECT_FALSE(result);
   EXPECT_EQ(p_scw_calibration->k_scw_min_candidate_age, 0);
}

/*
 * Tests the initialization procedure of the SCW feature. The calibrations shall be initialized correctly.
 * \uts{CSCSA-185971} \sdd{CSCSA-186568} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Iface_Test, Scw_Init_Platform__scw_initializes_correctly)
{
   /** \arrange Set up SCW to be uninitialized. */
   scw_input.f_scw_enable                              = FBK_TRUE;
   scw_input.f_scw_enable_dynamic                      = FBK_TRUE;
   scw_input.f_scw_enable_guardrail                    = FBK_TRUE;
   scw_instance.core_output.alert_level[FBK_SIDE_LEFT] = SCW_ALERT_LEVEL_1;

   /** \action Initialize SCW. */
   Scw_Init_Platform(&scw_instance);

   /** \assert Verify that SCW input and persistent cal value is set correctly. */
   EXPECT_TRUE(scw_input.f_scw_enable);
   EXPECT_TRUE(scw_input.f_scw_enable_dynamic);
   EXPECT_TRUE(scw_input.f_scw_enable_guardrail);
   EXPECT_EQ(scw_instance.core_output.alert_level[FBK_SIDE_LEFT], SCW_NO_ALERT);
}

/*
 * Tests the main function of the Side Collision Warning. Since no object information is available, the arranged alert states shall
 * be reset to the alert state none. \uts{CSCSA-185972} \sdd{CSCSA-186566} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Iface_Test, Scw_Run_Platform__check_scw_output)
{
   /** \arrange Set left side alert state to level 1. */
   scw_input.f_scw_enable                              = FBK_FALSE;
   scw_input.f_scw_enable_dynamic                      = FBK_FALSE;
   scw_input.f_scw_enable_guardrail                    = FBK_FALSE;
   scw_instance.core_output.alert_level[FBK_SIDE_LEFT] = SCW_ALERT_LEVEL_1;

   /** \action Run SCW main function */
   Scw_Run_Platform(&scw_instance, &scw_input, &fbk_output, &scw_output);

   /** \assert Verify that alert states have value SCW_NO_ALERT. */
   EXPECT_EQ(scw_instance.core_output.alert_level[FBK_SIDE_LEFT], SCW_NO_ALERT);
}

#ifndef NDEBUG
/*
 * Tests the main function of the Side Collision Warning if the assert is thrown in case of NULL instance pointer.
 * \uts{CSCSA-204179} \sdd{CSCSA-186566} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Iface_Test, Scw_Run_Platform__check_scw_output_instance_pointer_null)
{
   /** \arrange */
   /** \action Run SCW main function */
   /** \assert Verify that an assert is thrown, if Scw_Run function is called with NULL instance pointer. */
   EXPECT_DEATH({ Scw_Run_Platform(nullptr, &scw_input, &fbk_output, &scw_output); }, ".*p_scw_instance.*");
}
#endif

/*
 * Tests the main SCW function that it does not update the SCW output in case of NULL fbk output pointer.
 * \uts{CSCSA-204437} \sdd{CSCSA-186566} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Iface_Test, Scw_Run_Platform__check_scw_output_null_fbk_pointer)
{
   /** \arrange Set input flags to FALSE and output flags to TRUE. */
   scw_input.f_scw_enable             = FBK_FALSE;
   scw_input.f_scw_enable_dynamic     = FBK_FALSE;
   scw_input.f_scw_enable_guardrail   = FBK_FALSE;
   scw_output.f_scw_enabled           = FBK_TRUE;
   scw_output.f_scw_dyn_enabled       = FBK_TRUE;
   scw_output.f_scw_guardrail_enabled = FBK_TRUE;

   /** \action Run SCW main function */
   Scw_Run_Platform(&scw_instance, &scw_input, nullptr, &scw_output);

   /** \assert Verify that the SCW output is not updated if Scw_Run function is called with NULL fbk pointer. */
   EXPECT_TRUE(scw_output.f_scw_enabled);
   EXPECT_TRUE(scw_output.f_scw_dyn_enabled);
   EXPECT_TRUE(scw_output.f_scw_guardrail_enabled);
}

/*
 * Tests the interface function for the software major version. The major software version shall be equal to the internal one.
 * \uts{CSCSA-44458} \sdd{SF-8079} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Iface_Test, Scw_Get_Sw_Major_Version__returns_correct_major_version)
{
   /** \arrange Declare result variable. */
   uint16_t major_version;

   /** \action Fill result variable with major version */
   major_version = Scw_Get_Sw_Major_Version();

   /** \assert Verify that resulting major version equals persistent major version value. */
   EXPECT_EQ(major_version, Scw_Sw_Major_Version);
}

/*
 * Tests the interface function for the software minor version. The minor software version shall be equal to the internal one.
 * \uts{CSCSA-44459} \sdd{SF-8080} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Iface_Test, Scw_Get_Sw_Minor_Version__returns_correct_minor_version)
{
   /** \arrange Declare result variable. */
   uint16_t minor_version;

   /** \action Fill result variable with minor version */
   minor_version = Scw_Get_Sw_Minor_Version();

   /** \assert Verify that resulting minor version equals persistent minor version value. */
   EXPECT_EQ(minor_version, Scw_Sw_Minor_Version);
}


/**
 * Test that calibration updating workflow works properly in case of valid cal pointer.
 * \uts{CSCSA-122820} \sdd{CSCSA-122671} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Iface_Test, Scw_Update_Calibration__valid_cal_ptr)
{
   /** \arrange Set valid calibration pointer. */
   boolean_T result;

   Scw_Public_Calibration_T public_cal;
   Scw_Public_Cal_Update_Defaults(&public_cal);

   /** \action Call function to update calibration values. */
   result = Scw_Update_Calibration_Platform(&scw_instance, &public_cal);

   /** \assert Verify that function returns true in case of valid cal ptr. */
   EXPECT_TRUE(result);
}

/**
 * Test that calibration updating doesn't work in case of invalid cal pointer.
 * \uts{CSCSA-185975} \sdd{CSCSA-186567} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Iface_Test, Scw_Update_Calibration_Platform__null_cal_ptr)
{
   /** \arrange Set null calibration pointer. */
   boolean_T result;

   /** \action Call function to update calibration values. */
   result = Scw_Update_Calibration_Platform(&scw_instance, nullptr);

   /** \assert Verify that function returns false in case of invalid cal ptr. */
   EXPECT_FALSE(result);
}

/**
 * Test that calibration updating doesn't work in case of invalid instance pointer.
 * \uts{CSCSA-204181} \sdd{CSCSA-186567} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Iface_Test, Scw_Update_Calibration_Platform__null_instance_ptr)
{
   /** \arrange Set null instance pointer. */
   boolean_T result;

   Scw_Public_Calibration_T public_cal;
   Scw_Public_Cal_Update_Defaults(&public_cal);

   /** \action Call function to update calibration values. */
   result = Scw_Update_Calibration_Platform(nullptr, &public_cal);

   /** \assert Verify that function returns false in case of invalid instance ptr. */
   EXPECT_FALSE(result);
}
