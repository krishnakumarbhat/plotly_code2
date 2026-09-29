/**
 * @file fbk_iface_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for FBK interface.
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42231}
 */

#include "fbk_iface_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_iface.c"
#include "fbk_iface_types.h"
#include "fbk_index_lookup.h"
#include "fbk_obj_ageing.h"
#include "fbk_object_data_t.h"
#include "pa_context.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}

/**
 * Tests the interface function for the software major version. The major software version shall be equal to the internal one.
 * \uts{CSCSA-42390} \sdd{SF-4149} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Iface_Test, Fbk_Get_Sw_Major_Version__returns_correct_major_version)
{
   /** \arrange Declare result variable. */
   uint16_t major_version;

   /** \action Fill result variable with major version. */
   major_version = Fbk_Get_Sw_Major_Version();

   /** \assert Verify that resulting major version equals persistent major version value. */
   EXPECT_EQ(major_version, Fbk_Sw_Major_Version);
}

/**
 * Tests the interface function for the software minor version. The minor software version shall be equal to the internal one.
 * \uts{CSCSA-42391} \sdd{SF-4146} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Iface_Test, Fbk_Get_Sw_Minor_Version__returns_correct_minor_version)
{
   /** \arrange Declare result variable. */
   uint16_t minor_version;

   /** \action Fill result variable with minor version. */
   minor_version = Fbk_Get_Sw_Minor_Version();

   /** \assert Verify that resulting minor version equals persistent minor version value. */
   EXPECT_EQ(minor_version, Fbk_Sw_Minor_Version);
}


/**
 * Tests the interface function for the software minimum version check. The return value shall be true for a matching version.
 * \uts{CSCSA-42392} \sdd{SF-4158} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Iface_Test, Fbk_Is_Version_Compatible__returns_true_for_matching_version)
{
   /** \arrange Set up matching major and minor version. */
   uint16_t major_version_min = Fbk_Get_Sw_Major_Version();
   uint16_t minor_version_min = Fbk_Get_Sw_Minor_Version();

   /** \action Run version compatibility check. */
   boolean_T result = Fbk_Is_Version_Compatible(major_version_min, minor_version_min);

   /** \assert Verify that the result value is true for matching versions. */
   EXPECT_TRUE(result);
}

/**
 * Tests the interface function for the software minimum version check. The return value shall be true for a higher major version.
 * \uts{CSCSA-42393} \sdd{SF-4158} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Iface_Test, Fbk_Is_Version_Compatible__returns_true_for_higher_major_version)
{
   /** \arrange Set up a higher major version. */
   uint16_t major_version_min = Fbk_Get_Sw_Major_Version() - 1u;
   uint16_t minor_version_min = Fbk_Get_Sw_Minor_Version() + 1u;

   /** \action Run version compatibility check. */
   boolean_T result = Fbk_Is_Version_Compatible(major_version_min, minor_version_min);

   /** \assert Verify that the result value is true for a higher major version. */
   EXPECT_TRUE(result);
}

/**
 * Tests the interface function for the software minimum version check. The return value shall be false for an outdated version.
 * \uts{CSCSA-42394} \sdd{SF-4158} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Iface_Test, Fbk_Is_Version_Compatible__returns_false_for_outdated_version)
{
   /** \arrange Set up an invalid version. */
   uint16_t major_version_min = Fbk_Get_Sw_Major_Version() + 1u;
   uint16_t minor_version_min = 0u;

   /** \action Run version compatibility check. */
   boolean_T result = Fbk_Is_Version_Compatible(major_version_min, minor_version_min);

   /** \assert Verify that the result value is false for an invalid version. */
   EXPECT_FALSE(result);
}


/**
 * Tests that the initialization function of the Feature Building Kit takes care of all required initializations.
 * \uts{CSCSA-42395} \sdd{SF-4147} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Iface_Test, Fbk_Init_Platform__calls_required_functions)
{
   /** \arrange Prepare fbk_context with NULL pointers and get ageing info pointer. */
   /** \action Call Fbk_Init function. */
   Fbk_Init_Platform(&fbk_instance, &pa_data);

   /** \assert Verify that fbk_context pointers are set and ageing info is initialized. */
   EXPECT_EQ(fbk_instance.fbk_obj_ages.stage_age[0], 1u);
   EXPECT_EQ(fbk_instance.fbk_obj_ages.stage[0], PA_OBJ_STATUS_INVALID);
}

/**
 * Tests that the run function of the Feature Building Kit calls all required functions.
 * \uts{CSCSA-42396} \sdd{SF-4148} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Iface_Test, Fbk_Run_Platform__calls_required_functions)
{
   /** \arrange Init Fbk, get ageing info pointer and set up tracker object */
   Fbk_Init_Platform(&fbk_instance, &pa_data);
   Fbk_Age_Ctr_T *p_fbk_obj_ctr                      = &fbk_instance.fbk_obj_ages;
   uint8_t obj_index                                 = 5u;
   fbk_context.p_data->object_data[obj_index].status = PA_OBJ_STATUS_MATURE;
   fbk_context.p_data->object_data[obj_index].id     = 15u;

   /** \action Call Fbk_Run function. */
   Fbk_Run_Platform(&fbk_instance, &fbk_output, &fbk_context);

   /** \assert Verify that lookup of index from ID works properly and ageing information is updated. */
   EXPECT_EQ(Fbk_Get_Object_Index_From_Id(fbk_output.p_index_id_lookup_table, 15u), obj_index);
   EXPECT_EQ(p_fbk_obj_ctr->stage_age[obj_index], 1u);
   EXPECT_EQ(p_fbk_obj_ctr->stage[obj_index], PA_OBJ_STATUS_MATURE);
}

/**
 * Tests that the run function of the Feature Building Kit calls all required functions.
 * \uts{CSCSA-186080} \sdd{SF-4148} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Iface_Test, Fbk_Run_Platform__use_external_contex)
{
   /** \arrange Init Fbk, get ageing info pointer and set up tracker object */
   const uint8_t obj_index = 9u;
   const uint8_t obj_id    = 15u;

   Fbk_Init_Platform(&fbk_instance, &pa_data);
   Fbk_Age_Ctr_T *p_fbk_obj_ctr = &fbk_instance.fbk_obj_ages;

   Pa_Data_T external_pa_data{};
   Pa_Context_T external_context{&external_pa_data};

   external_pa_data.object_data[obj_index].status = PA_OBJ_STATUS_MATURE;
   external_pa_data.object_data[obj_index].id     = obj_id;

   /** \action Call Fbk_Run function. */
   Fbk_Run_Platform(&fbk_instance, &fbk_output, &external_context);

   /** \assert Verify that lookup of index from ID works properly and ageing information is updated. */
   EXPECT_EQ(Fbk_Get_Object_Index_From_Id(fbk_output.p_index_id_lookup_table, obj_id), obj_index);
   EXPECT_EQ(p_fbk_obj_ctr->stage_age[obj_index], 1u);
   EXPECT_EQ(p_fbk_obj_ctr->stage[obj_index], PA_OBJ_STATUS_MATURE);
}
