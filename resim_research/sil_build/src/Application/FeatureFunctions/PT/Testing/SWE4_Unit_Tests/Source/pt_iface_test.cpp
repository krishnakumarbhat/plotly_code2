/**
 * @file pt_iface_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for pt_iface.c functions
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-43673}
 */

#include "pt_iface_test.hpp"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include <type_traits>

extern "C"
{
#include "fbk_iface.c"
#include "fbk_iface.h"
#include "fbk_macros.h"
#include "pt_iface.c"
#include "pt_output_t.h"
#include "pt_persistent_t.h"
#include "pt_public_calibration.h"
}

using ::testing::Eq;
using ::testing::Not;

/**
 * Test the path tracking functionality to update default calibration values.
 * \uts{CSCSA-43674} \sdd{SF-7426} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Iface_Test, Pt_Update_Core_Cal_By_Core_To_Defaults_Helper__updates_cal_values)
{
   /* \arrange Set path tracking version cal value. */
   uint16_t invalid_version = 0u;

   /* \action Call Pt_Update_Core_Cal_By_Core_To_Defaults */
   Pt_Core_Cal_Update_Defaults(&pt_instance.calibration);

   /* \assert Check if version is set to value unequal to invalid version count. */
   EXPECT_NE(pt_instance.calibration.Header.version, invalid_version);
}


/**
 * Test the path tracking functionality to initialize the feature.
 * \uts{CSCSA-43675} \sdd{SF-7425} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Iface_Test, Pt_Init_Platform__initializes_path_tracking)
{
   /* \arrange Path tracking is uninitialzed. */
   p_pt_input->grid_pt_array[0]                              = 0;
   p_pt_input->Num_Grid_Pts_Dep_Cals.k_pt_find_max_long_posn = 0;

   /* \action Call Pt_Init_Platform to initialize PT. */
   Pt_Init_Platform(&pt_instance);

   /* \assert Check if all pointers for path tracking are initialized. */
   EXPECT_TRUE(0 != p_pt_input->grid_pt_array[0]);
   EXPECT_TRUE(0 != p_pt_input->Num_Grid_Pts_Dep_Cals.k_pt_find_max_long_posn);
}


/**
 * Test the general path tracking functionality for valid inputs.
 * \uts{CSCSA-43676} \sdd{SF-7424} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Iface_Test, Pt_Run_Platform__run_pt_for_valid_inputs)
{
   /* \arrange Initialize Path tracking. */
   Pt_Init_Platform(&pt_instance);
   p_vehicle_data->host_speed = 0.0f;

   /* \action Run Pt_Run_Platform. */
   Pt_Run_Platform(&pt_instance, &Pt_Output, &fbk_output);

   /* \assert Check if PT function executes correctly */
   EXPECT_TRUE(pt_instance.persistent.f_was_pt_executed);
}


/**
 * Test the general path tracking functionality for valid inputs after initial run.
 * \uts{CSCSA-43677} \sdd{SF-7424} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Iface_Test, Pt_Run__run_pt_after_initialization)
{
   /* \arrange Initialize Path tracking and run it once. */
   Pt_Init_Platform(&pt_instance);
   p_vehicle_data->host_speed = 0.0f;
   Pt_Run_Platform(&pt_instance, &Pt_Output, &fbk_output);

   /* \action Run Pt_Run_Platform a second time after initialization. */
   Pt_Run_Platform(&pt_instance, &Pt_Output, &fbk_output);

   /* \assert Check if PT function executes correctly again. */
   EXPECT_TRUE(pt_instance.persistent.f_was_pt_executed);
}

/**
 * Test the general path tracking functionality for re-initializing.
 * \uts{CSCSA-43678} \sdd{SF-7424} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Iface_Test, Pt_Run__run_pt_reinitialization)
{
   /* \arrange Initialize Path tracking and run it once. Set high ego speed value. */
   Pt_Init_Platform(&pt_instance);
   p_vehicle_data->host_speed = 0.0f;
   Pt_Run_Platform(&pt_instance, &Pt_Output, &fbk_output);
   p_vehicle_data->host_speed = 100.0f;

   /* \action Run Pt_Run_Platform a second time after ego speed has increased above thresholds. */
   Pt_Run_Platform(&pt_instance, &Pt_Output, &fbk_output);

   /* \assert Check if PT function resets correctly. */
   EXPECT_FALSE(pt_instance.persistent.f_was_pt_executed);
}


/**
 * Test whether information about the nearest path is returned correct.
 * \uts{CSCSA-43680} \sdd{SF-7574} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Iface_Test, Pt_Get_Nearest_Path_Info__returns_correct_pointer)
{
   /* \arrange Set up nearest PT output. */
   const Pt_Nearest_Path_T *path_nearest_out;

   /* \action Get PT output pointer. */
   path_nearest_out = Pt_Get_Nearest_Path_Info(&Pt_Output, 0u);

   /* \assert Check if PT function gives back correct pointer. */
   EXPECT_EQ(path_nearest_out, &Pt_Output.nearest_path_output[0u]);
}


/**
 * Test whether information about the path match is returned correct.
 * \uts{CSCSA-43681} \sdd{SF-7575} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Iface_Test, Pt_Get_Match_Information__returns_correct_pointer)
{
   /* \arrange Set up PT match pointer. */
   const Pt_Path_Object_Pair_Output_T *path_match_out;

   /* \action Get PT output pointer. */
   path_match_out = Pt_Get_Match_Information(&Pt_Output, 0u);

   /* \assert Check if PT function gives back correct pointer. */
   EXPECT_EQ(path_match_out, &Pt_Output.path_obj_pair_output[0u]);
}


/**
 * Tests the path tracking function for handling NULL pointers. Here all pointers are valid. Thus true is expected.
 * \uts{CSCSA-43682} \sdd{SF-7405} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Iface_Test, Pt_Are_All_Pointers_Valid__all_pointers_valid)
{
   /* \arrange Set up return value. */
   boolean_T f_all_pointers_valid = FBK_FALSE;

   /* \action Check PT inputs for NULL pointers */
   f_all_pointers_valid = Pt_Are_All_Pointers_Valid(&pt_instance, &Pt_Output, &fbk_output);

   /* \assert Check if PT function detects all pointers as valid. */
   EXPECT_TRUE(f_all_pointers_valid);
}


/**
 * Tests the path tracking function for handling NULL pointers. Input pointer is Null. Thus false is expected
 * \uts{CSCSA-43683} \sdd{SF-7405} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Iface_Test, Pt_Are_All_Pointers_Valid__instance_pointer_is_null)
{
   /* \arrange Set up return value. */
   boolean_T f_all_pointers_valid = FBK_FALSE;

   /* \action Check PT inputs for NULL pointers */
   f_all_pointers_valid = Pt_Are_All_Pointers_Valid(nullptr, &Pt_Output, &fbk_output);

   /* \assert Expect false since input is NULL */
   EXPECT_FALSE(f_all_pointers_valid);
}

/**
 * Tests the path tracking function for handling NULL pointers. Context pointer is Null. Thus false is expected
 * \uts{CSCSA-43684} \sdd{SF-7405} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Iface_Test, Pt_Are_All_Pointers_Valid__fbk_output_is_null)
{
   /* \arrange Set up return value. */
   boolean_T f_all_pointers_valid = FBK_FALSE;

   /* \action Check PT inputs for NULL pointers */
   f_all_pointers_valid = Pt_Are_All_Pointers_Valid(&pt_instance, &Pt_Output, nullptr);

   /* \assert Expect false since context is NULL */
   EXPECT_FALSE(f_all_pointers_valid);
}

/**
 * Tests the path tracking function for handling NULL pointers. Output pointer is Null. Thus false is expected
 * \uts{CSCSA-43685} \sdd{SF-7405} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Iface_Test, Pt_Are_All_Pointers_Valid__output_is_null)
{
   /* \arrange Set up return value. */
   boolean_T f_all_pointers_valid = FBK_FALSE;

   /* \action Check PT inputs for NULL pointers */
   f_all_pointers_valid = Pt_Are_All_Pointers_Valid(&pt_instance, nullptr, &fbk_output);

   /* \assert Expect false since output is NULL */
   EXPECT_FALSE(f_all_pointers_valid);
}


/**
 * Tests the path tracking function for handling NULL pointers. persistent pointer is Null. Thus false is expected
 * \uts{CSCSA-43686} \sdd{SF-7405} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Iface_Test, Pt_Are_All_Pointers_Valid__host_trail_is_null)
{
   /* \arrange Set up return value. */
   boolean_T f_all_pointers_valid = FBK_FALSE;

   Fbk_Output_T tmp_fbk_output = fbk_output;
   tmp_fbk_output.p_host_trail = nullptr;
   /* \action Check PT inputs for NULL pointers */
   f_all_pointers_valid = Pt_Are_All_Pointers_Valid(&pt_instance, &Pt_Output, &tmp_fbk_output);

   /* \assert Expect false since persistent is NULL */
   EXPECT_FALSE(f_all_pointers_valid);
}


/*
 * Tests the interface function for the software major version. The major software version shall be equal to the internal one.
 * \uts{CSCSA-43690} \sdd{SF-7422} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Iface_Test, Pt_Get_Sw_Major_Version__returns_correct_major_version)
{
   /** \arrange Declare result variable. */
   uint16_t major_version;

   /** \action Fill result variable with major version */
   major_version = Pt_Get_Sw_Major_Version();

   /** \assert Verify that resulting major version equals persistent major version value. */
   EXPECT_EQ(major_version, Pt_Sw_Major_Version);
}

/*
 * Tests the interface function for the software minor version. The minor software version shall be equal to the internal one.
 * \uts{CSCSA-43691} \sdd{SF-7423} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Iface_Test, Pt_Get_Sw_Minor_Version__returns_correct_minor_version)
{
   /** \arrange Declare result variable. */
   uint16_t minor_version;

   /** \action Fill result variable with minor version */
   minor_version = Pt_Get_Sw_Minor_Version();

   /** \assert Verify that resulting minor version equals persistent minor version value. */
   EXPECT_EQ(minor_version, Pt_Sw_Minor_Version);
}

/**
 * Test the general path tracking functionality for invalid inputs.
 * \uts{CSCSA-43692} \sdd{SF-7424} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Iface_Test, Pt_Run__run_pt_for_invalid_inputs)
{
   /* \arrange Initialize Path tracking. */
   Pt_Init_Platform(&pt_instance);
   Fbk_Output_T fbk_local_output{};
   p_vehicle_data->host_speed = 0.0f;

   /* \action Run Pt_Run_Platform. */
   Pt_Run_Platform(&pt_instance, &Pt_Output, &fbk_local_output);

   /* \assert Check if PT function executes correctly */
   EXPECT_FALSE(pt_instance.persistent.f_was_pt_executed);
}

/**
 * Tests that calibration is updated correctly.
 * \uts{CSCSA-185845} \sdd{} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Pt_Iface_Test, Pt_Update_Calibration_Platform__test)
{
   /** \arrange declare variable for result and simple imput */
   boolean_T result;
   Pt_Public_Calibration_T public_cal;
   Pt_Public_Cal_Update_Defaults(&public_cal);

   Pt_Init_Platform(&pt_instance);

   /** \action call calibration update */
   result = Pt_Update_Calibration_Platform(&pt_instance, &public_cal);

   /** \assert expect succes */
   EXPECT_TRUE(result);
}
