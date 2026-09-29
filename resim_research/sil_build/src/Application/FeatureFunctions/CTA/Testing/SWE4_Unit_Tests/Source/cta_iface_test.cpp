/**
 * @file cta_iface_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for cta_iface.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-41945}
 */

#include "cta_iface_test.hpp"

#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <math.h>

extern "C"
{
#include "cta.h"
#include "cta_iface.c"
#include "cta_public_calibration.h"
#include "fbk_iface.c"
#include "fbk_iface.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
}


/**
 * Tests initialization method of CTA feature function. Expect that calibrations are set to valid values and that context data is
 * available. \uts{CSCSA-41946} \sdd{CSCSA-186416} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Iface_Test, Cta_Init_Platform__initialization_of_cta)
{
   /** \arrange Reset of pointer */
   Sfl_Status_T res;
   /** \action executes function to test */
   res = Cta_Init_Platform(&cta_instance);

   /** \assert Expect true */
   EXPECT_EQ(res, SFL_STATUS_OK);
}


/**
 * Checks update routine for a given cal value.
 * \uts{CSCSA-41947} \sdd{CSCSA-186433} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Iface_Test, Cta_Core_Calibration_Update_Defaults__test_that_update_is_occuring)
{
   /** \arrange Modify calibration of rsds internally */
   cta_instance.calibration.k_cta_butterfly_long[0] = INFINITY;

   /** \action executes function to test */
   Cta_Core_Cal_Update_Defaults(&cta_instance.calibration);

   /** \assert Expect that the default is not chosen to be infinity */
   EXPECT_TRUE(INFINITY != cta_instance.calibration.k_cta_butterfly_long[0]);
}


/**
 * Tests main function for execution. Expect main function shall be execute in case of valid input pointers.
 * \uts{CSCSA-41949} \sdd{CSCSA-186415} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Iface_Test, Cta_Run_Platform__pointers_valid)
{
   /** \arrange Set calibration pointer to valid address */
   Cta_Init_Input(&cta_input);
   Cta_Init_Platform(&cta_instance);
   // cta_input.f_cta_switch = FBK_ZERO_UINT;
   /** \action Execute function to test */
   /** \assert Expect no fatal failure */
   EXPECT_NO_FATAL_FAILURE({ Cta_Run_Platform(&cta_input, &fbk_output, &pt_output, &cta_instance, &cta_output); });
}

/**
 * Tests main function for execution. Expect the main function to fail in execute in case of invalid input pointers.
 * \uts{CSCSA-185716} \sdd{CSCSA-186415} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Iface_Test, Cta_Run_Platform__pointers_invalid)
{
   /** \arrange Set calibration pointer to valid address */
   Cta_Init_Platform(&cta_instance);
   /** \action Execute function to test */
   /** \assert Expect no fatal failure */
   Cta_Run_Platform(&cta_input, nullptr, &pt_output, &cta_instance, &cta_output);
}

/**
 * Tests whether all input pointer are valid. Expect that all pointers are valid.
 * \uts{CSCSA-41950} \sdd{SF-3826} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Iface_Test, Cta_Are_All_Pointers_Valid__all_pointers_valid)
{
   /** \arrange context in cta input */
   boolean_T result;

   /** \action executes function to test */
   result = Cta_Are_All_Pointers_Valid(&cta_input, &cta_output, &cta_instance, &fbk_output);

   /** \assert Expect true */
   EXPECT_TRUE(result);
}


/**
 * Tests whether all input pointer are valid. Expect false since output is not valid.
 * \uts{CSCSA-41951} \sdd{SF-3826} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Iface_Test, Cta_Are_All_Pointers_Valid__out_pointer_invalid)
{

   /** \arrange result and context */
   boolean_T result;
   /** \action executes function to test */
   result = Cta_Are_All_Pointers_Valid(&cta_input, nullptr, &cta_instance, &fbk_output);
   /** \assert Expect false */
   EXPECT_FALSE(result);
}


/**
 * Tests whether all input pointer are valid. Expect false since input is not valid.
 * \uts{CSCSA-41952} \sdd{SF-3826} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Iface_Test, Cta_Are_All_Pointers_Valid__in_pointer_invalid)
{
   /** \arrange result and context */
   boolean_T result;
   /** \action executes function to test */
   result = Cta_Are_All_Pointers_Valid(nullptr, &cta_output, &cta_instance, &fbk_output);
   /** \assert Expect false */
   EXPECT_FALSE(result);
}


/**
 * Tests whether all input pointer are valid. Expect false since cta_instance is not valid.
 * \uts{CSCSA-41954} \sdd{SF-3826} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Iface_Test, Cta_Are_All_Pointers_Valid__cta_instance_pointer_invalid)
{
   /** \arrange result */
   boolean_T result;
   /** \action executes function to test */
   result = Cta_Are_All_Pointers_Valid(&cta_input, &cta_output, nullptr, &fbk_output);
   /** \assert Expect false */
   EXPECT_FALSE(result);
}

/**
 * Tests whether all input pointer are valid. Expect false since fbk output is not valid.
 * \uts{CSCSA-185717} \sdd{SF-3826} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Iface_Test, Cta_Are_All_Pointers_Valid__fbk_output_pointer_invalid)
{
   /** \arrange result */
   boolean_T result;
   /** \action executes function to test */
   result = Cta_Are_All_Pointers_Valid(&cta_input, &cta_output, &cta_instance, nullptr);
   /** \assert Expect false */
   EXPECT_FALSE(result);
}


/**
 * Tests reset function of CTA. Expect default values for internals of CTA.
 * \uts{CSCSA-41955} \sdd{SF-3721} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Iface_Test, Cta_Reset__also_reset_lateral_paths)
{
   /** \arrange calibrations for path tracking and values unequal to their default values */
   uint8_t mode                                = CTA_MODE_REAR;
   cta_core_output.cta_id[mode][FBK_SIDE_LEFT] = 1;

   /** \action executes function to test */
   Cta_Reset(&cta_instance);
   /** \assert Expect default values */
   EXPECT_EQ(cta_core_output.cta_id[mode][FBK_SIDE_LEFT], 0u);
}

/**
 * Tests reset function of CTA. Expect default values for warning and braking counters of CTA.
 * \uts{CSCSA-41956} \sdd{SF-3721} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Iface_Test, Cta_Reset__reset_counters)
{
   /** \arrange calibrations for path tracking and values unequal to their default values */
   uint8_t mode                                             = CTA_MODE_REAR;
   cta_core_output.cta_id[mode][FBK_SIDE_LEFT]              = 1;
   cta_core_output.cta_warn_hold_cnt[mode][FBK_SIDE_LEFT]   = 4u;
   cta_core_output.cta_brake_hold_cnt[mode][FBK_SIDE_LEFT]  = 5u;
   cta_core_output.cta_brake_supp_cnt[mode][FBK_SIDE_RIGHT] = 7u;

   /** \action executes function to test */
   Cta_Reset(&cta_instance);

   /** \assert Expect default values */
   EXPECT_EQ(cta_core_output.cta_id[mode][FBK_SIDE_LEFT], 0u);
   EXPECT_EQ(cta_core_output.cta_warn_hold_cnt[mode][FBK_SIDE_LEFT], 0u);
   EXPECT_EQ(cta_core_output.cta_brake_hold_cnt[mode][FBK_SIDE_LEFT], 0u);
   EXPECT_EQ(cta_core_output.cta_brake_supp_cnt[mode][FBK_SIDE_RIGHT], 0u);
}

/**
 * Tests reset function of CTA. Expect default values for internals of CTA but dont call .
 * \uts{CSCSA-41957} \sdd{SF-3721} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Iface_Test, Cta_Reset__dont_call_pt_reset)
{
   /** \arrange calibrations for path tracking and values unequal to their default values */
   uint8_t mode                                = CTA_MODE_REAR;
   cta_core_output.cta_id[mode][FBK_SIDE_LEFT] = 1;
   /** \action executes function to test */
   Cta_Reset(&cta_instance);
   /** \assert Expect default values */
   EXPECT_EQ(cta_core_output.cta_id[mode][FBK_SIDE_LEFT], 0u);
}

/**
 * Tests return of sw major version. Expect software versions to be equal.
 * \uts{CSCSA-41960} \sdd{SF-3842} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Iface_Test, Cta_Get_Sw_Major_Version__return_software_major_version)
{
   /** \arrange sw_version */
   uint16_t returned_sw_major_version;
   /** \action executes function to test */
   returned_sw_major_version = Cta_Get_Sw_Major_Version();
   /** \assert Expect versions to be equal */
   EXPECT_EQ(returned_sw_major_version, Cta_Sw_Major_Version);
}

/**
 * Tests return of sw minor version. Expect software versions to be equal.
 * \uts{CSCSA-41961} \sdd{SF-3843} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Iface_Test, Cta_Get_Sw_Minor_Version__return_software_minor_version)
{
   /** \arrange sw_version */
   uint16_t returned_sw_minor_version;
   /** \action executes function to test */
   returned_sw_minor_version = Cta_Get_Sw_Minor_Version();
   /** \assert Expect versions to be equal */
   EXPECT_EQ(returned_sw_minor_version, Cta_Sw_Minor_Version);
}


/**
 * Tests check if Cta_Update_Calibration is updating calibration.
 * \uts{CSCSA-185718} \sdd{CSCSA-186425} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Iface_Test, Cta_Update_Calibration_Platform__check_if_calibration_is_updated)
{
   /** \arrange */
   boolean_T res                               = FBK_FALSE;
   Cta_Public_Calibration_T public_calibration = {};
   Cta_Public_Cal_Update_Defaults(&public_calibration);

   public_calibration.k_cta_cycle_count_hold_true_warning += 1;

   /** \action run update calibration function */
   ASSERT_NE(public_calibration.k_cta_cycle_count_hold_true_warning, cta_instance.calibration.k_cta_cycle_count_hold_true_warning);
   res = Cta_Update_Calibration_Platform(&cta_instance, &public_calibration);
   /** \assert Are vaue updated */
   ASSERT_EQ(cta_instance.calibration.k_cta_cycle_count_hold_true_warning, public_calibration.k_cta_cycle_count_hold_true_warning);
   EXPECT_EQ(res, (boolean_T) FBK_TRUE);
}

/**
 * Tests check if Cta_Update_Calibration is not updating calibration.
 * \uts{CSCSA-188375} \sdd{CSCSA-186425} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Cta_Iface_Test, Cta_Update_Calibration_Platform__check_if_calibration_is_not_updated)
{
   /** \arrange */
   boolean_T res;
   /** \action run update calibration function */
   res = Cta_Update_Calibration_Platform(&cta_instance, NULL);
   /** \assert Are vaue updated */
   EXPECT_EQ(res, (boolean_T) FBK_FALSE);
}
