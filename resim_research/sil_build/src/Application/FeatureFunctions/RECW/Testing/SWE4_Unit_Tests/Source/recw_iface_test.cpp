/**
 * @file recw_iface_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for TA unit tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44282}
 */

/* clang-format off */
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>
#include "recw_iface_test.hpp"
/* clang-format on */

extern "C"
{
#include "fbk_iface.c"
#include "fbk_iface.h"
#include "fbk_macros.h"
#include "pa_reuse.h"
#include "recw_core_calibration.h"
#include "recw_iface.c"
#include "recw_input_t.h"
#include "recw_output_t.h"
#include "recw_public_calibration.h"
}

/**
 * Call RECW initialization function. Verify that tracker output and vehicle pointer are set in RECW input.
 * \uts{CSCSA-44283} \sdd{CSCSA-186512} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Iface_Test, Recw_Init_Platform__sets_vehicle_and_tracker_output_pointer)
{
   /** \arrange Not required. */

   /** \action Call initialization function. */
   Sfl_Status_T result = Recw_Init_Platform(&recw_instance);

   /** \assert Verify that tracker output and vehicle pointer are set in RECW input. */
   // EXPECT_EQ(recw_instance.calibration, recw_cals);

   EXPECT_EQ(recw_instance.persistent.recw_alert_qualifying_counter, FBK_ZERO_UINT);
   EXPECT_EQ(recw_instance.persistent.recw_alert_holding_counter, FBK_ZERO_UINT);
   EXPECT_EQ(recw_instance.persistent.recw_alert_duration_counter, FBK_ZERO_UINT);
   EXPECT_EQ(recw_instance.persistent.recw_rear_blockage_qualifying_counter, FBK_ZERO_UINT);
   EXPECT_EQ(recw_instance.persistent.recw_rear_blockage_object_index, PA_INVALID_OBJ_INDEX);
   EXPECT_EQ(recw_instance.persistent.recw_id_prev_cycle, PA_INVALID_OBJ_ID);
   EXPECT_EQ(recw_instance.persistent.recw_index_prev_cycle, FBK_ZERO_UINT);
   EXPECT_EQ(recw_instance.persistent.recw_alert_prev_cycle, RECW_NO_ALERT);
   EXPECT_EQ(recw_instance.persistent.f_is_rear_blocked, FBK_FALSE);
   EXPECT_EQ(recw_instance.persistent.f_host_speed_in_allowed_range, FBK_FALSE);

   EXPECT_EQ(result, SFL_STATUS_OK);
}


/**
 * Test that calibration updating workflow works properly in case of valid cal pointer.
 * \uts{CSCSA-185881} \sdd{CSCSA-186513} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Iface_Test, Recw_Update_Calibration_Platform__valid_cal_ptr)
{
   /** \arrange Set valid calibration pointer. */
   boolean_T result;

   Recw_Public_Calibration_T public_cal;
   Recw_Public_Cal_Update_Defaults(&public_cal);

   /** \action Call function to update calibration values. */
   result = Recw_Update_Calibration_Platform(&recw_instance, &public_cal);

   /** \assert Verify that function returns true in case of valid cal ptr. */
   EXPECT_TRUE(result);
}

/**
 * Test that calibration updating workflow works properly in NOT valid recw_instance pointer.
 * \uts{CSCSA-204752} \sdd{CSCSA-186513} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Iface_Test, Recw_Update_Calibration_Platform__not_valid_instance_ptr)
{
   /** \arrange Set pointer to valids calibration */
   boolean_T result;

   Recw_Public_Calibration_T public_cal;
   Recw_Public_Cal_Update_Defaults(&public_cal);

   /** \action Call function to update calibration values. */
   result = Recw_Update_Calibration_Platform(nullptr, &public_cal);

   /** \assert Verify that function returns true in case of valid cal ptr. */
   EXPECT_FALSE(result);
}


/**
 * Test that calibration updating doesn't work in case of invalid cal pointer.
 * \uts{CSCSA-185882} \sdd{CSCSA-186513} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Iface_Test, Recw_Update_Calibration_Platform__null_cal_ptr)
{
   /** \arrange Set null calibration pointer. */
   const Recw_Public_Calibration_T *p_recw_calibration = NULL;
   boolean_T result;

   /** \action Call function to update calibration values. */
   result = Recw_Update_Calibration_Platform(&recw_instance, p_recw_calibration);

   /** \assert Verify that function returns false in case of invalid cal ptr. */
   EXPECT_FALSE(result);
}


/**
 * Call RECW main function. Verify that function can be called without any fatal failure.
 * \uts{CSCSA-185883} \sdd{CSCSA-186514} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Iface_Test, Recw_Run_Platform__runs_without_failure)
{
   /** \arrange Initialize RECW. */
   Recw_Init_Platform(&recw_instance);

   /** \action See assert. */

   /** \assert Verify that RECW main function runs without any fatal failure. */
   EXPECT_NO_FATAL_FAILURE(Recw_Run_Platform(&recw_instance, &recw_input, &fbk_output, &recw_output));
}

/**
 * Call RECW main function. Verify that function is not crushing when it is started with null pointer.
 * \uts{CSCSA-185884} \sdd{CSCSA-186514} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Iface_Test, Recw_Run_Platform__run_with_null_pointer)
{
   /** \arrange Initialize RECW. Set null pointer to calibrations. */
   Recw_Init_Platform(&recw_instance);

   /** \action See assert. */

   /** \assert Verify that RECW main function runs without any fatal failure. */
   EXPECT_NO_FATAL_FAILURE(Recw_Run_Platform(nullptr, nullptr, nullptr, nullptr););
}

/**
 * Tests the interface function for the software major version. The major software version shall be equal to the internal one.
 * \uts{CSCSA-44288} \sdd{SF-7908} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Iface_Test, Recw_Get_Sw_Major_Version__returns_correct_major_version)
{
   /** \arrange Declare result variable. */
   uint16_t major_version;

   /** \action Fill result variable with major version. */
   major_version = Recw_Get_Sw_Major_Version();

   /** \assert Verify that resulting major version equals persistent major version value. */
   EXPECT_EQ(major_version, Recw_Sw_Major_Version);
}

/**
 * Tests the interface function for the software minor version. The minor software version shall be equal to the internal one.
 * \uts{CSCSA-44289} \sdd{SF-7909} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Iface_Test, Recw_Get_Sw_Minor_Version__returns_correct_minor_version)
{
   /** \arrange Declare result variable. */
   uint16_t minor_version;

   /** \action Fill result variable with minor version. */
   minor_version = Recw_Get_Sw_Minor_Version();

   /** \assert Verify that resulting minor version equals persistent minor version value. */
   EXPECT_EQ(minor_version, Recw_Sw_Minor_Version);
}


/**
 * Tests that the null pointer detection returns false for the default settings.
 * \uts{CSCSA-44290} \sdd{SF-7894} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Iface_Test, Recw_Null_Pointer_Detected__returns_FALSE_for_default_settings)
{
   /** \arrange Declare result variable. */
   Recw_Init_Platform(&recw_instance);
   boolean_T f_null_pointer_detected = FBK_TRUE;

   /** \action Call Recw_Null_Pointer_Detected to check for NULL pointer. */
   f_null_pointer_detected = Recw_Null_Pointer_Detected(&recw_instance, &recw_input, &fbk_output, &recw_output);

   /** \assert Verify that no NULL pointer is detected. */
   EXPECT_FALSE(f_null_pointer_detected);
}

/**
 * Tests that the null pointer detection return true if context pointer is NULL.
 * \uts{CSCSA-44291} \sdd{SF-7894} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Iface_Test, Recw_Null_Pointer_Detected__returns_TRUE_for_calibrations_NULL_pointer)
{
   /** \arrange Declare result variable. */
   boolean_T f_null_pointer_detected = FBK_FALSE;
   Recw_Instance_T *p_recw_instance  = nullptr;

   /** \action Call Recw_Null_Pointer_Detected to check for NULL pointer. */
   f_null_pointer_detected = Recw_Null_Pointer_Detected(p_recw_instance, &recw_input, &fbk_output, &recw_output);

   /** \assert Verify that a NULL pointer is detected. */
   EXPECT_TRUE(f_null_pointer_detected);
}

/**
 * Tests that the null pointer detection return true if recw output is NULL.
 * \uts{CSCSA-185885} \sdd{SF-7894} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Iface_Test, Recw_Null_Pointer_Detected__returns_TRUE_for_recw_output_NULL_pointer)
{
   /** \arrange Declare result variable. */
   boolean_T f_null_pointer_detected = FBK_FALSE;

   /** \action Call Recw_Null_Pointer_Detected to check for NULL pointer. */
   f_null_pointer_detected = Recw_Null_Pointer_Detected(&recw_instance, &recw_input, &fbk_output, nullptr);

   /** \assert Verify that a NULL pointer is detected. */
   EXPECT_TRUE(f_null_pointer_detected);
}

/**
 * Tests that the null pointer detection return true if FBK output is NULL.
 * \uts{CSCSA-185886} \sdd{SF-7894} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Iface_Test, Recw_Null_Pointer_Detected__returns_TRUE_for_fbk_output_NULL_pointer)
{
   /** \arrange Declare result variable. */
   boolean_T f_null_pointer_detected = FBK_FALSE;

   /** \action Call Recw_Null_Pointer_Detected to check for NULL pointer. */
   f_null_pointer_detected = Recw_Null_Pointer_Detected(&recw_instance, &recw_input, nullptr, &recw_output);

   /** \assert Verify that a NULL pointer is detected. */
   EXPECT_TRUE(f_null_pointer_detected);
}

/**
 * Tests that the null pointer detection return true if recw input is NULL.
 * \uts{CSCSA-185887} \sdd{SF-7894} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Iface_Test, Recw_Null_Pointer_Detected__returns_TRUE_for_recw_input_NULL_pointer)
{
   /** \arrange Declare result variable. */
   boolean_T f_null_pointer_detected = FBK_FALSE;

   /** \action Call Recw_Null_Pointer_Detected to check for NULL pointer. */
   f_null_pointer_detected = Recw_Null_Pointer_Detected(&recw_instance, nullptr, &fbk_output, &recw_output);

   /** \assert Verify that a NULL pointer is detected. */
   EXPECT_TRUE(f_null_pointer_detected);
}

/**
 * Tests that the null pointer detection return true if PA data is NULL.
 * \uts{CSCSA-185888} \sdd{SF-7894} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Iface_Test, Recw_Null_Pointer_Detected__returns_TRUE_for_pa_data_NULL_pointer)
{
   /** \arrange Declare pa data as nullptr. */
   boolean_T f_null_pointer_detected = FBK_FALSE;
   fbk_output.p_pa_data              = nullptr;

   /** \action Call Recw_Null_Pointer_Detected to check for NULL pointer. */
   f_null_pointer_detected = Recw_Null_Pointer_Detected(&recw_instance, &recw_input, &fbk_output, &recw_output);

   /** \assert Verify that a NULL pointer is detected. */
   EXPECT_TRUE(f_null_pointer_detected);
}


/**
 * Tests that the null pointer detection return true if context pointer is NULL.
 * \uts{CSCSA-185890} \sdd{CSCSA-186513} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Recw_Iface_Test, Recw_Update_Calibration_Platform__updates_caibration_data_properly)
{
   /** \arrange Declare result variable. */
   boolean_T result;

   Recw_Public_Calibration_T public_cal;
   Recw_Public_Cal_Update_Defaults(&public_cal);

   public_cal.k_recb_max_host_speed = FBK_ZERO_F;

   /** \action Call Recw_Null_Pointer_Detected to check for NULL pointer. */
   result = Recw_Update_Calibration_Platform(&recw_instance, &public_cal);

   /** \assert Verify that a NULL pointer is detected. */
   EXPECT_TRUE(result);
}
