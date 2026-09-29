/**
 * @file lcda_iface_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for lcda_iface.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42624}
 */

#include "lcda_iface_test.hpp"

extern "C"
{
#include "fbk_iface.c"
#include "fbk_iface.h"
#include "lcda_core_calibration.h"
#include "lcda_iface.c"
#include "lcda_input_t.h"
#include "lcda_output_t.h"
#include "lcda_public_calibration.h"
#include "pa_reuse.h"
}


Lcda_Instance_T Lcda_Iface_Test::Lcda_Instance;
Lcda_Input_T Lcda_Iface_Test::Lcda_Input;

/**
 * Initialize LCDA, no exception shall be thrown.
 * \uts{CSCSA-185768} \sdd{CSCSA-196458} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Iface_Test, Lcda_Init_Input__No_Fatal_Error_occurs)
{
   /** \arrange Not applicable. */
   /** \action Not applicable. */
   /** \assert Call Lcda_Init and check that no exception is thrown. */
   EXPECT_NO_FATAL_FAILURE(Lcda_Init_Input(&Lcda_Input););
}

/**
 * Initialize LCDA, no exception shall be thrown.
 * \uts{CSCSA-185769} \sdd{CSCSA-186590} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Iface_Test, Lcda_Init_Platform__No_Fatal_Error_occurs)
{
   /** \arrange Not applicable. */
   /** \action Not applicable. */
   /** \assert Call Lcda_Init_Platform and check that no exception is thrown. */
   EXPECT_NO_FATAL_FAILURE(Lcda_Init_Platform(&Lcda_Instance););
}

/**
 * Call LCDA main function, no exception shall be thrown.
 * \uts{CSCSA-185770} \sdd{SF-6639} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Iface_Test, Lcda_Run_Platform_No_Fatal_Error_occurs)
{
   /** \arrange Set PA data for backward compatibility. */
   Fbk_Update_Index_Id_Lookup_Table(&lookup_table, &pa_data);
   Lcda_Init_Input(&Lcda_Input);
   Lcda_Init_Platform(&Lcda_Instance);
   /** \action Not applicable. */
   /** \assert Call Lcda_Run_Platform and check that no exception is thrown. */
   EXPECT_NO_FATAL_FAILURE(Lcda_Run_Platform(&Lcda_Instance, &Lcda_Input, &fbk_output, &lcda_output););
}

/**
 * Call LCDA main function, no exception shall be thrown for lcda instance set as nullptr.
 * \uts{CSCSA-185771} \sdd{SF-6639} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Iface_Test, Lcda_Run_Platform_No_Fatal_Error_occurs_lcda_instance_nullptr)
{
   /** \arrange Not applicable. */
   /** \action Not applicable. */
   /** \assert Call Lcda_Run_Platform and check that no exception is thrown. */
   EXPECT_NO_FATAL_FAILURE(Lcda_Run_Platform(nullptr, &Lcda_Input, &fbk_output, &lcda_output););
}


/**
 * Call LCDA main function, no exception shall be thrown for lcda input set as nullptr.
 * \uts{CSCSA-185772} \sdd{SF-6639} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Iface_Test, Lcda_Run_Platform_No_Fatal_Error_occurs_lcda_input_nullptr)
{
   /** \arrange Not applicable. */
   /** \action Not applicable. */
   /** \assert Call Lcda_Run_Platform and check that no exception is thrown. */
   EXPECT_NO_FATAL_FAILURE(Lcda_Run_Platform(&Lcda_Instance, nullptr, &fbk_output, &lcda_output););
}


/**
 * Call LCDA main function, no exception shall be thrown for fbk output set as nullptr.
 * \uts{CSCSA-185780} \sdd{SF-6639} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Iface_Test, Lcda_Run__No_Fatal_Error_occurs_fbk_output_nullptr)
{
   /** \arrange Not applicable. */
   /** \action Not applicable. */
   /** \assert Call Lcda_Run_Platform and check that no exception is thrown. */
   EXPECT_NO_FATAL_FAILURE(Lcda_Run_Platform(&Lcda_Instance, &Lcda_Input, nullptr, &lcda_output););
}

/**
 * Call LCDA main function, no exception shall be thrown for lcda output set as nullptr.
 * \uts{CSCSA-185791} \sdd{SF-6639} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Iface_Test, Lcda_Run__No_Fatal_Error_occurs_lcda_output_nullptr)
{
   /** \arrange Not applicable. */
   /** \action Not applicable. */
   /** \assert Call Lcda_Run_Platform and check that no exception is thrown. */
   EXPECT_NO_FATAL_FAILURE(Lcda_Run_Platform(&Lcda_Instance, &Lcda_Input, &fbk_output, nullptr););
}


/**
 * Get LCDAs major version number and check that it is correct.
 * \uts{CSCSA-42629} \sdd{SF-6637} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Iface_Test, Lcda_Get_Sw_Major_Version__Get_Lcda_Sw_Major_Version_returns_correct_major_version)
{
   /** \arrange Declare version variable. */
   uint16_t major_version;

   /** \action Call Lcda_Get_Sw_Major_Version to obtain version. */
   major_version = Lcda_Get_Sw_Major_Version();

   /** \assert Check that major version is filled correctly. */
   EXPECT_EQ(major_version, Lcda_Sw_Major_Version);
}

/**
 * Get LCDAs minor version number and check that it is correct.
 * \uts{CSCSA-42630} \sdd{SF-6638} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Iface_Test, Lcda_Get_Sw_Minor_Version__Get_Lcda_Sw_Minor_Version_returns_correct_minor_version)
{
   /** \arrange Declare version variable. */
   uint16_t minor_version;

   /** \action Call Lcda_Get_Sw_Minor_Version to obtain version. */
   minor_version = Lcda_Get_Sw_Minor_Version();

   /** \assert Check that minor version is filled correctly. */
   EXPECT_EQ(minor_version, Lcda_Sw_Minor_Version);
}


/**
 * Tests that calibration is updated correctly.
 * \uts{CSCSA-185792} \sdd{CSCSA-186593} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Iface_Test, Lcda_Update_Calibration_Platform__test)
{
   /** \arrange declare variable for result and simple imput */
   boolean_T result;
   Lcda_Init_Platform(&Lcda_Instance);

   Lcda_Public_Calibration_T public_cal;
   Lcda_Public_Cal_Update_Defaults(&public_cal);

   /** \action call calibration update */
   result = Lcda_Update_Calibration_Platform(&Lcda_Instance, &public_cal);

   /** \assert expect succes */
   EXPECT_TRUE(result);
}

/**
 * Tests that calibration is not updated if source calibration pointer is null.
 * \uts{CSCSA-186405} \sdd{CSCSA-186593} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Iface_Test, Lcda_Update_Calibration_Platform__null_source_calibration_pointer)
{
   /** \arrange declare variable for result and simple imput */
   boolean_T result;
   Lcda_Init_Platform(&Lcda_Instance);

   /** \action call calibration update */
   result = Lcda_Update_Calibration_Platform(&Lcda_Instance, nullptr);

   /** \assert expect false */
   EXPECT_FALSE(result);
}
