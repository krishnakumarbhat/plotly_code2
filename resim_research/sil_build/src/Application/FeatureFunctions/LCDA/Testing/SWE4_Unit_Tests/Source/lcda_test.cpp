/**
 * @file lcda_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for lcda.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42857}
 */

#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "lcda.c"
#include "lcda_core_calibration.h"
#include "lcda_core_calibration_check.h"
#include "lcda_core_input_t.h"
#include "lcda_core_output_t.h"
#include "lcda_persistent_t.h"
#include "lcda_test_helpers.h"
#include "lcda_types.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}

#include "lcda_test.hpp"

#ifndef NDEBUG
/**
 * Check that an exception is thrown if function to reset LCDA is called with null pointer for core output.
 * \uts{CSCSA-42858} \sdd{SF-6553} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Reset__NULL_lcda_core_output_pointer_throws_assertion)
{
   /** \arrange */
   /** \action See assert. */
   /** \assert Verify that an assert is thrown, when Lcda_Reset function is called with null pointer for core output. */
   EXPECT_DEATH({ Lcda_Reset(NULL, NULL); }, ".*p_lcda_instance.*");
}
#endif // !NDEBUG

/**
 * Check that LCDA is not active if it is disabled by the corresponding calibration flag.
 * \uts{CSCSA-42860} \sdd{SF-6552} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Core_Run__core_output_set_to_default_values_when_lcda_disabled_by_calibration)
{
   /** \arrange Set up calibration values and core input such that LCDA is disabled by calibrations. */
   lcda_cals.k_lcda_enable_via_cal              = 1u;
   lcda_cals.k_lcda_enable                      = 0u;
   lcda_core_input.enabled_flags.f_lcda_enabled = FBK_FALSE;

   /** \action Call Lcda_Core_Run to execute the LCDA core. */
   Lcda_Core_Run(&lcda_instance, &fbk_output);

   /** \assert Verify that LCDA is not active because it was disabled by cals and core output is default. */
   EXPECT_EQ(lcda_core_output.lcda_status, LCDA_STATUS_DISABLED_BY_CAL);

   EXPECT_TRUE(Lcda_Is_Core_Output_Default(&lcda_core_output));
}

/**
 * Check that LCDA is not active if it is disabled by the corresponding core input flag.
 * \uts{CSCSA-42861} \sdd{SF-6552} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Core_Run__core_output_set_to_default_values_when_lcda_disabled_by_input_flag)
{
   /** \arrange Set up calibration values and core input such that LCDA is disabled by core input. */
   lcda_cals.k_lcda_enable_via_cal              = 0u;
   lcda_cals.k_lcda_enable                      = 1u;
   lcda_core_input.enabled_flags.f_lcda_enabled = FBK_FALSE;

   /** \action Call Lcda_Core_Run to execute the LCDA core. */
   Lcda_Core_Run(&lcda_instance, &fbk_output);

   /** \assert Verify that LCDA is not active because it was disabled by core input and core output is default. */
   EXPECT_EQ(lcda_core_output.lcda_status, LCDA_STATUS_DISABLED_BY_INPUT);

   EXPECT_TRUE(Lcda_Is_Core_Output_Default(&lcda_core_output));
}

/**
 * Check that LCDA is not active if ego speed is too low for LCDA to be active.
 * \uts{CSCSA-42862} \sdd{SF-6552} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Core_Run__core_output_set_to_default_values_when_host_below_activation_speed)
{
   /** \arrange Set up calibration values and vehicle data such that ego speed is too low for LCDA to be active. */
   lcda_cals.k_lcda_host_activation_speed_min = 20.0f;

   p_vehicle_data->host_speed = 3.0f;

   lcda_core_input.enabled_flags.f_lcda_enabled = FBK_TRUE;

   /** \action Call Lcda_Core_Run to execute the LCDA core. */
   Lcda_Core_Run(&lcda_instance, &fbk_output);

   /** \assert Verify that LCDA is not active due to ego speed and core output is default. */
   EXPECT_EQ(lcda_core_output.lcda_status, LCDA_STATUS_DEACTIVATED_LOW_EGO_SPEED);

   EXPECT_TRUE(Lcda_Is_Core_Output_Default(&lcda_core_output));
}

/**
 * Check that LCDA is active and core output flags are set to disabled if submodules are disabled by calibrations.
 * \uts{CSCSA-42864} \sdd{SF-6552} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Core_Run__core_output_set_to_default_values_if_submodules_disabled_by_cal)
{
   /** \arrange Set up calibration values and core input such that submodules are disabled by core input. */
   lcda_cals.k_bsw_enable_via_cal = 1u;
   lcda_cals.k_bsw_enable         = 0u;
   lcda_cals.k_cvw_enable_via_cal = 1u;
   lcda_cals.k_cvw_enable         = 0u;
   lcda_cals.k_slc_enable_via_cal = 1u;
   lcda_cals.k_slc_enable         = 0u;
   lcda_cals.k_elc_enable_via_cal = 1u;
   lcda_cals.k_elc_enable         = 0u;

   lcda_core_input.enabled_flags.f_bsw_enabled  = FBK_TRUE;
   lcda_core_input.enabled_flags.f_cvw_enabled  = FBK_TRUE;
   lcda_core_input.enabled_flags.f_slc_enabled  = FBK_TRUE;
   lcda_core_input.enabled_flags.f_elc_enabled  = FBK_TRUE;
   lcda_core_input.enabled_flags.f_lcda_enabled = FBK_TRUE;

   /** \action Call Lcda_Core_Run to execute the LCDA core. */
   Lcda_Core_Run(&lcda_instance, &fbk_output);

   /** \assert Verify that LCDA is active, core output flags are set to disabled and core output of submodules is default. */
   EXPECT_EQ(lcda_core_output.lcda_status, LCDA_STATUS_ACTIVE);

   EXPECT_TRUE(Lcda_Is_Bsw_Output_Default(&lcda_core_output.bsw_core_output));
   EXPECT_TRUE(Lcda_Is_Cvw_Output_Default(&lcda_core_output.cvw_core_output));
   EXPECT_TRUE(Lcda_Is_Slc_Output_Default(&lcda_core_output.slc_core_output));
   EXPECT_TRUE(Lcda_Is_Elc_Output_Default(&lcda_core_output.elc_core_output));
   EXPECT_FALSE(lcda_core_output.bsw_core_output.f_bsw_is_enabled);
   EXPECT_FALSE(lcda_core_output.cvw_core_output.f_cvw_is_enabled);
   EXPECT_FALSE(lcda_core_output.slc_core_output.f_slc_is_enabled);
   EXPECT_FALSE(lcda_core_output.elc_core_output.f_elc_is_enabled);
}

/**
 * Check that LCDA is active and core output flags are set to enabled if submodules are enabled by calibrations.
 * \uts{CSCSA-42865} \sdd{SF-6552} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Core_Run__core_output_enabled_flags_are_true_if_submodules_enabled_by_cal)
{
   /** \arrange Set up calibration values and core input such that submodules are enabled by calibrations. */

   lcda_cals.k_bsw_enable_via_cal = 1u;
   lcda_cals.k_bsw_enable         = 1u;
   lcda_cals.k_cvw_enable_via_cal = 1u;
   lcda_cals.k_cvw_enable         = 1u;
   lcda_cals.k_slc_enable_via_cal = 1u;
   lcda_cals.k_slc_enable         = 1u;
   lcda_cals.k_elc_enable_via_cal = 1u;
   lcda_cals.k_elc_enable         = 1u;

   lcda_core_input.enabled_flags.f_bsw_enabled  = FBK_FALSE;
   lcda_core_input.enabled_flags.f_cvw_enabled  = FBK_FALSE;
   lcda_core_input.enabled_flags.f_slc_enabled  = FBK_FALSE;
   lcda_core_input.enabled_flags.f_elc_enabled  = FBK_FALSE;
   lcda_core_input.enabled_flags.f_lcda_enabled = FBK_TRUE;

   /** \action Call Lcda_Core_Run to execute the LCDA core. */
   Lcda_Core_Run(&lcda_instance, &fbk_output);

   /** \assert Verify that LCDA is active and core output flags are set to enabled. */
   EXPECT_EQ(lcda_core_output.lcda_status, LCDA_STATUS_ACTIVE);

   EXPECT_TRUE(lcda_core_output.bsw_core_output.f_bsw_is_enabled);
   EXPECT_TRUE(lcda_core_output.cvw_core_output.f_cvw_is_enabled);
   EXPECT_TRUE(lcda_core_output.slc_core_output.f_slc_is_enabled);
   EXPECT_TRUE(lcda_core_output.elc_core_output.f_elc_is_enabled);
}

/**
 * Check that LCDA is active and core output flags are set to enabled if submodules are enabled by core input.
 * \uts{CSCSA-42866} \sdd{SF-6552} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Core_Run__core_output_enabled_flags_are_true_if_submodules_enabled_by_core_input)
{
   /** \arrange Set up calibration values and core input such that submodules are enabled by core input. */

   lcda_cals.k_bsw_enable_via_cal               = 0u;
   lcda_cals.k_cvw_enable_via_cal               = 0u;
   lcda_cals.k_slc_enable_via_cal               = 0u;
   lcda_cals.k_elc_enable_via_cal               = 0u;
   lcda_core_input.enabled_flags.f_bsw_enabled  = FBK_TRUE;
   lcda_core_input.enabled_flags.f_cvw_enabled  = FBK_TRUE;
   lcda_core_input.enabled_flags.f_slc_enabled  = FBK_TRUE;
   lcda_core_input.enabled_flags.f_elc_enabled  = FBK_TRUE;
   lcda_core_input.enabled_flags.f_lcda_enabled = FBK_TRUE;

   /** \action Call Lcda_Core_Run to execute the LCDA core. */
   Lcda_Core_Run(&lcda_instance, &fbk_output);

   /** \assert Verify that LCDA is active and core output flags are set to enabled. */
   EXPECT_EQ(lcda_core_output.lcda_status, LCDA_STATUS_ACTIVE);

   EXPECT_TRUE(lcda_core_output.bsw_core_output.f_bsw_is_enabled);
   EXPECT_TRUE(lcda_core_output.cvw_core_output.f_cvw_is_enabled);
   EXPECT_TRUE(lcda_core_output.slc_core_output.f_slc_is_enabled);
   EXPECT_TRUE(lcda_core_output.elc_core_output.f_elc_is_enabled);
}

/**
 * Check that an object is classified as valid for LCDA if all conditions are fulfilled.
 * \uts{CSCSA-42867} \sdd{SF-6544} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Valid_Object__returns_TRUE_for_valid_object)
{
   /** \arrange Set up calibrations and tracker output such that object is valid for LCDA. */
   uint8_t obj_idx  = 4u;
   boolean_T result = FBK_FALSE;

   lcda_cals.k_lcda_min_exist_prop = 0.8f;
   lcda_cals.k_lcda_min_track_age  = 5u;
   lcda_cals.k_lcda_max_range      = 50.0f;

   object_data[obj_idx].existence_probability = 1.0f;
   object_data[obj_idx].age                   = lcda_cals.k_lcda_min_track_age + 1u;
   object_data[obj_idx].vcs_pos.x             = 3.0f;
   object_data[obj_idx].status                = PA_OBJ_STATUS_MATURE;
   object_data[obj_idx].obj_class             = PA_OBJ_CLASS_CAR;
   object_data[obj_idx].f_moveable            = FBK_TRUE;
   object_data[obj_idx].f_reflection          = FBK_FALSE;


   /** \action Call Lcda_Is_Valid_Object to evaluate if object is valid for LCDA. */
   result = Lcda_Is_Valid_Object(object_data + obj_idx, &lcda_cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}


/**
 * Check that an object is classified as invalid for LCDA if the object below the threshold for the minimal object age.
 * \uts{CSCSA-42868} \sdd{SF-6544} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Valid_Object__returns_FALSE_if_obj_is_too_young)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid due to object age being too small. */
   uint8_t obj_idx  = 4u;
   boolean_T result = FBK_TRUE;

   lcda_cals.k_lcda_min_exist_prop = 0.8f;
   lcda_cals.k_lcda_min_track_age  = 5u;
   lcda_cals.k_lcda_max_range      = 50.0f;

   object_data[obj_idx].existence_probability = 1.0f;
   object_data[obj_idx].age                   = lcda_cals.k_lcda_min_track_age - 1u;
   object_data[obj_idx].vcs_pos.x             = 3.0f;
   object_data[obj_idx].status                = PA_OBJ_STATUS_MATURE;
   object_data[obj_idx].obj_class             = PA_OBJ_CLASS_CAR;
   object_data[obj_idx].f_moveable            = FBK_TRUE;
   object_data[obj_idx].f_reflection          = FBK_FALSE;

   /** \action Call Lcda_Is_Valid_Object to evaluate if object is valid for LCDA. */
   result = Lcda_Is_Valid_Object(object_data + obj_idx, &lcda_cals);

   /** \assert Verify that object is classified as not valid. */
   EXPECT_FALSE(result);
}

/**
 * Check that an object is classified as invalid for LCDA if the object is out of range for LCDA.
 * \uts{CSCSA-42869} \sdd{SF-6544} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Valid_Object__returns_FALSE_if_obj_out_of_lcda_range)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid due to being out of range for LCDA. */
   uint8_t obj_idx  = 4u;
   boolean_T result = FBK_TRUE;

   lcda_cals.k_lcda_min_exist_prop = 0.8f;
   lcda_cals.k_lcda_min_track_age  = 5u;
   lcda_cals.k_lcda_max_range      = 50.0f;

   object_data[obj_idx].existence_probability = 1.0f;
   object_data[obj_idx].age                   = lcda_cals.k_lcda_min_track_age + 1u;
   object_data[obj_idx].vcs_pos.x             = -55.0f;
   object_data[obj_idx].status                = PA_OBJ_STATUS_MATURE;
   object_data[obj_idx].obj_class             = PA_OBJ_CLASS_CAR;
   object_data[obj_idx].f_moveable            = FBK_TRUE;
   object_data[obj_idx].f_reflection          = FBK_FALSE;

   /** \action Call Lcda_Is_Valid_Object to evaluate if object is valid for LCDA. */
   result = Lcda_Is_Valid_Object(object_data + obj_idx, &lcda_cals);

   /** \assert Verify that object is classified as not valid. */
   EXPECT_FALSE(result);
}

/**
 * Check that an object is classified as invalid for LCDA if the object is not moveable.
 * \uts{CSCSA-138983} \sdd{SF-6544} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Valid_Object__returns_false_for_not_moveable_object)
{
   /** \arrange Set up calibrations and tracker output such that object is valid for LCDA. */
   uint8_t obj_idx  = 4u;
   boolean_T result = FBK_FALSE;

   lcda_cals.k_lcda_min_exist_prop = 0.8f;
   lcda_cals.k_lcda_min_track_age  = 5u;
   lcda_cals.k_lcda_max_range      = 50.0f;

   object_data[obj_idx].existence_probability = 1.0f;
   object_data[obj_idx].age                   = lcda_cals.k_lcda_min_track_age + 1u;
   object_data[obj_idx].vcs_pos.x             = 3.0f;
   object_data[obj_idx].status                = PA_OBJ_STATUS_MATURE;
   object_data[obj_idx].obj_class             = PA_OBJ_CLASS_CAR;
   object_data[obj_idx].f_moveable            = FBK_FALSE;
   object_data[obj_idx].f_reflection          = FBK_FALSE;


   /** \action Call Lcda_Is_Valid_Object to evaluate if object is valid for LCDA. */
   result = Lcda_Is_Valid_Object(&object_data[obj_idx], &lcda_cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_FALSE(result);
}

/**
 * Check that an object is classified as invalid for LCDA if the object status is invalid.
 * \uts{CSCSA-42870} \sdd{SF-6544} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Valid_Object__returns_FALSE_if_obj_status_is_invalid)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid due to the object status being invalid. */
   uint8_t obj_idx  = 4u;
   boolean_T result = FBK_TRUE;

   lcda_cals.k_lcda_min_exist_prop = 0.8f;
   lcda_cals.k_lcda_min_track_age  = 5u;
   lcda_cals.k_lcda_max_range      = 50.0f;

   object_data[obj_idx].existence_probability = 1.0f;
   object_data[obj_idx].age                   = lcda_cals.k_lcda_min_track_age + 1u;
   object_data[obj_idx].vcs_pos.x             = 3.0f;
   object_data[obj_idx].status                = PA_OBJ_STATUS_INVALID;
   object_data[obj_idx].obj_class             = PA_OBJ_CLASS_CAR;
   object_data[obj_idx].f_moveable            = FBK_TRUE;
   object_data[obj_idx].f_reflection          = FBK_FALSE;

   /** \action Call Lcda_Is_Valid_Object to evaluate if object is valid for LCDA. */
   result = Lcda_Is_Valid_Object(object_data + obj_idx, &lcda_cals);

   /** \assert Verify that object is classified as not valid. */
   EXPECT_FALSE(result);
}

/**
 * Check that an object is classified as invalid for LCDA if VRU conditions are not met.
 * \uts{CSCSA-141578} \sdd{SF-6544} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Valid_Object__returns_FALSE_for_invalid_vru_object)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid for LCDA. */
   uint8_t obj_idx  = 4u;
   boolean_T result = FBK_FALSE;
   float32_T len    = Fast_Sqrt(lcda_cals.k_lcda_pedestrian_min_size);
   float32_T width  = Fast_Sqrt(lcda_cals.k_lcda_pedestrian_min_size);

   lcda_cals.k_lcda_min_exist_prop = 0.8f;
   lcda_cals.k_lcda_min_track_age  = 5u;
   lcda_cals.k_lcda_max_range      = 50.0f;

   object_data[obj_idx].existence_probability = 1.0f;
   object_data[obj_idx].age                   = lcda_cals.k_lcda_min_track_age + 1u;
   object_data[obj_idx].vcs_pos.x             = 3.0f;
   object_data[obj_idx].status                = PA_OBJ_STATUS_MATURE;
   object_data[obj_idx].obj_class             = PA_OBJ_CLASS_PEDESTRIAN;
   object_data[obj_idx].length                = len - EPSILON;
   object_data[obj_idx].width                 = width - EPSILON;
   object_data[obj_idx].speed                 = lcda_cals.k_lcda_pedestrian_min_speed - EPSILON;
   object_data[obj_idx].f_moveable            = FBK_TRUE;
   object_data[obj_idx].f_reflection          = FBK_FALSE;


   /** \action Call Lcda_Is_Valid_Object to evaluate if object is valid for LCDA. */
   result = Lcda_Is_Valid_Object(&object_data[obj_idx], &lcda_cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_FALSE(result);
}


/**
 * Check that an object is classified as invalid for LCDA if the object reflection flag checking is enabled and this flag is active
 * \uts{CSCSA-140121} \sdd{SF-6544} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Valid_Object__returns_FALSE_if_obj_reflection_flag_is_active)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid due to the reflection flag being active. */
   uint8_t obj_idx  = 4u;
   boolean_T result = FBK_TRUE;

   lcda_cals.k_lcda_min_exist_prop                     = 0.8f;
   lcda_cals.k_lcda_min_track_age                      = 5u;
   lcda_cals.k_lcda_max_range                          = 50.0f;
   lcda_cals.k_lcda_f_enable_obj_reflection_flag_check = FBK_TRUE;

   object_data[obj_idx].existence_probability = 1.0f;
   object_data[obj_idx].age                   = lcda_cals.k_lcda_min_track_age + 1u;
   object_data[obj_idx].vcs_pos.x             = 3.0f;
   object_data[obj_idx].status                = PA_OBJ_STATUS_MATURE;
   object_data[obj_idx].obj_class             = PA_OBJ_CLASS_CAR;
   object_data[obj_idx].f_moveable            = FBK_TRUE;
   object_data[obj_idx].f_reflection          = FBK_TRUE;

   /** \action Call Lcda_Is_Valid_Object to evaluate if object is valid for LCDA. */
   result = Lcda_Is_Valid_Object(&object_data[obj_idx], &lcda_cals);

   /** \assert Verify that object is classified as not valid. */
   EXPECT_FALSE(result);
}

/**
 * Check that an object is classified as valid for LCDA if the object reflection flag checking is enabled and this flag is inactive
 * \uts{CSCSA-140122} \sdd{SF-6544} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Valid_Object__returns_TRUE_if_obj_reflection_flag_is_inactive)
{
   /** \arrange Set up calibrations and tracker output such that object is valid. */
   uint8_t obj_idx  = 4u;
   boolean_T result = FBK_TRUE;

   lcda_cals.k_lcda_min_exist_prop                     = 0.8f;
   lcda_cals.k_lcda_min_track_age                      = 5u;
   lcda_cals.k_lcda_max_range                          = 50.0f;
   lcda_cals.k_lcda_f_enable_obj_reflection_flag_check = FBK_TRUE;

   object_data[obj_idx].existence_probability = 1.0f;
   object_data[obj_idx].age                   = lcda_cals.k_lcda_min_track_age + 1u;
   object_data[obj_idx].vcs_pos.x             = 3.0f;
   object_data[obj_idx].status                = PA_OBJ_STATUS_MATURE;
   object_data[obj_idx].obj_class             = PA_OBJ_CLASS_CAR;
   object_data[obj_idx].f_moveable            = FBK_TRUE;
   object_data[obj_idx].f_reflection          = FBK_FALSE;

   /** \action Call Lcda_Is_Valid_Object to evaluate if object is valid for LCDA. */
   result = Lcda_Is_Valid_Object(&object_data[obj_idx], &lcda_cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}

/**
 * Check that an car object is classified as valid for LCDA.
 * \uts{CSCSA-141101} \sdd{CSCSA-141096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_VRU_Object_Valid__returns_TRUE_if_object_is_a_car)
{
   /** \arrange Set up calibrations and tracker output such that object is valid. */
   uint8_t obj_idx = 7u;
   boolean_T result;

   object_data[obj_idx].obj_class = PA_OBJ_CLASS_CAR;
   object_data[obj_idx].length    = 4.5f;
   object_data[obj_idx].width     = 2.0f;

   /** \action Call Lcda_Is_VRU_Object_Valid to evaluate if object is valid for LCDA. */
   result = Lcda_Is_VRU_Object_Valid(&object_data[obj_idx], &lcda_cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}


/**
 * Check that an slow moving pedestrian object is classified valid for LCDA.
 * \uts{CSCSA-141102} \sdd{CSCSA-141096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_VRU_Object_Valid__returns_TRUE_if_object_is_a_slow_pedestrian_but_size_over_threshold)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid. */
   uint8_t obj_idx = 9u;
   boolean_T result;
   float32_T len   = Fast_Sqrt(lcda_cals.k_lcda_pedestrian_min_size);
   float32_T width = Fast_Sqrt(lcda_cals.k_lcda_pedestrian_min_size);

   object_data[obj_idx].obj_class = PA_OBJ_CLASS_PEDESTRIAN;
   object_data[obj_idx].length    = len + EPSILON;
   object_data[obj_idx].width     = width + EPSILON;
   object_data[obj_idx].speed     = lcda_cals.k_lcda_pedestrian_min_speed - EPSILON;

   /** \action Call Lcda_Is_VRU_Object_Valid to evaluate if object is valid for LCDA. */
   result = Lcda_Is_VRU_Object_Valid(&object_data[obj_idx], &lcda_cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}

/**
 * Check that an slow moving unknown class object is classified valid for LCDA.
 * \uts{CSCSA-141579} \sdd{CSCSA-141096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_VRU_Object_Valid__returns_TRUE_if_unknown_object_is_slow_moving_but_size_over_threshold)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid. */
   uint8_t obj_idx = 9u;
   boolean_T result;
   float32_T len   = Fast_Sqrt(lcda_cals.k_lcda_pedestrian_min_size);
   float32_T width = Fast_Sqrt(lcda_cals.k_lcda_pedestrian_min_size);

   object_data[obj_idx].obj_class = PA_OBJ_CLASS_UNKNOWN;
   object_data[obj_idx].length    = len + EPSILON;
   object_data[obj_idx].width     = width + EPSILON;
   object_data[obj_idx].speed     = lcda_cals.k_lcda_pedestrian_min_speed - EPSILON;

   /** \action Call Lcda_Is_VRU_Object_Valid to evaluate if object is valid for LCDA. */
   result = Lcda_Is_VRU_Object_Valid(&object_data[obj_idx], &lcda_cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}


/**
 * Check that a pedestrian object is classified valid for LCDA when its size is below threshold
 * \uts{CSCSA-141103} \sdd{CSCSA-141096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_VRU_Object_Valid__returns_TRUE_if_pedestrian_size_below_threshold_but_speed_over_threshold)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid. */
   uint8_t obj_idx = 2u;
   boolean_T result;
   float32_T len   = Fast_Sqrt(lcda_cals.k_lcda_pedestrian_min_size);
   float32_T width = Fast_Sqrt(lcda_cals.k_lcda_pedestrian_min_size);

   object_data[obj_idx].obj_class = PA_OBJ_CLASS_PEDESTRIAN;
   object_data[obj_idx].length    = len - EPSILON;
   object_data[obj_idx].width     = width - EPSILON;
   object_data[obj_idx].speed     = lcda_cals.k_lcda_pedestrian_min_speed + EPSILON;

   /** \action Call Lcda_Is_VRU_Object_Valid to evaluate if object is valid for LCDA. */
   result = Lcda_Is_VRU_Object_Valid(&object_data[obj_idx], &lcda_cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}

/**
 * Check that a unknown object is classified valid for LCDA when its size is below threshold
 * \uts{CSCSA-141580} \sdd{CSCSA-141096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_VRU_Object_Valid__returns_TRUE_if_unknown_object_size_below_threshold_but_speed_over_threshold)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid. */
   uint8_t obj_idx = 2u;
   boolean_T result;
   float32_T len   = Fast_Sqrt(lcda_cals.k_lcda_pedestrian_min_size);
   float32_T width = Fast_Sqrt(lcda_cals.k_lcda_pedestrian_min_size);

   object_data[obj_idx].obj_class = PA_OBJ_CLASS_UNKNOWN;
   object_data[obj_idx].length    = len - EPSILON;
   object_data[obj_idx].width     = width - EPSILON;
   object_data[obj_idx].speed     = lcda_cals.k_lcda_pedestrian_min_speed + EPSILON;

   /** \action Call Lcda_Is_VRU_Object_Valid to evaluate if object is valid for LCDA. */
   result = Lcda_Is_VRU_Object_Valid(&object_data[obj_idx], &lcda_cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}

/**
 * Check that an pedestrian object is classified as valid for LCDA.
 * \uts{CSCSA-141104} \sdd{CSCSA-141096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_VRU_Object_Valid__returns_TRUE_if_pedestrian_shall_be_valid)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid. */
   uint8_t obj_idx = 6u;
   boolean_T result;

   object_data[obj_idx].obj_class = PA_OBJ_CLASS_PEDESTRIAN;
   object_data[obj_idx].length    = 1.0f;
   object_data[obj_idx].width     = 1.0f;
   object_data[obj_idx].speed     = lcda_cals.k_lcda_pedestrian_min_speed + EPSILON;

   /** \action Call Lcda_Is_VRU_Object_Valid to evaluate if object is valid for LCDA. */
   result = Lcda_Is_VRU_Object_Valid(&object_data[obj_idx], &lcda_cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}

/**
 * Check that an unknwon object is classified as valid for LCDA.
 * \uts{CSCSA-141581} \sdd{CSCSA-141096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_VRU_Object_Valid__returns_TRUE_if_unknown_object_shall_be_valid)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid. */
   uint8_t obj_idx = 6u;
   boolean_T result;

   object_data[obj_idx].obj_class = PA_OBJ_CLASS_UNKNOWN;
   object_data[obj_idx].length    = 1.0f;
   object_data[obj_idx].width     = 1.0f;
   object_data[obj_idx].speed     = lcda_cals.k_lcda_pedestrian_min_speed + EPSILON;

   /** \action Call Lcda_Is_VRU_Object_Valid to evaluate if object is valid for LCDA. */
   result = Lcda_Is_VRU_Object_Valid(&object_data[obj_idx], &lcda_cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}

/**
 * Check that a pedestrian object is classified as invalid for LCDA.
 * \uts{CSCSA-141105} \sdd{CSCSA-141096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_VRU_Object_Valid__returns_FALSE_if_pedestrian_object_is_invalid)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid. */
   uint8_t obj_idx = 2u;
   boolean_T result;
   float32_T len   = Fast_Sqrt(lcda_cals.k_lcda_pedestrian_min_size);
   float32_T width = Fast_Sqrt(lcda_cals.k_lcda_pedestrian_min_size);

   object_data[obj_idx].obj_class = PA_OBJ_CLASS_PEDESTRIAN;
   object_data[obj_idx].length    = len - EPSILON;
   object_data[obj_idx].width     = width - EPSILON;
   object_data[obj_idx].speed     = lcda_cals.k_lcda_pedestrian_min_speed - EPSILON;

   /** \action Call Lcda_Is_VRU_Object_Valid to evaluate if object is valid for LCDA. */
   result = Lcda_Is_VRU_Object_Valid(&object_data[obj_idx], &lcda_cals);

   /** \assert Verify that object is classified as invalid. */
   EXPECT_FALSE(result);
}

/**
 * Check that a slow moving 2wheel object is classified as valid for LCDA.
 * \uts{CSCSA-141106} \sdd{CSCSA-141096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_VRU_Object_Valid__returns_TRUE_if_object_is_a_slow_2wheel_but_size_overthreshold)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid. */
   uint8_t obj_idx = 8u;
   boolean_T result;
   float32_T len   = Fast_Sqrt(lcda_cals.k_lcda_2wheel_min_size);
   float32_T width = Fast_Sqrt(lcda_cals.k_lcda_2wheel_min_size);

   object_data[obj_idx].obj_class = PA_OBJ_CLASS_2WHEEL;
   object_data[obj_idx].length    = len + EPSILON;
   object_data[obj_idx].width     = width + EPSILON;
   object_data[obj_idx].speed     = lcda_cals.k_lcda_2wheel_min_speed - EPSILON;

   /** \action Call Lcda_Is_VRU_Object_Valid to evaluate if object is valid for LCDA. */
   result = Lcda_Is_VRU_Object_Valid(&object_data[obj_idx], &lcda_cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}


/**
 * Check that a small 2whell object is classified as valid for LCDA.
 * \uts{CSCSA-141107} \sdd{CSCSA-141096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_VRU_Object_Valid__returns_TRUE_if_2wheel_object_size_below_threshold_but_speed_over_threshold)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid. */
   uint8_t obj_idx = 11u;
   boolean_T result;
   float32_T len   = Fast_Sqrt(lcda_cals.k_lcda_2wheel_min_size);
   float32_T width = Fast_Sqrt(lcda_cals.k_lcda_2wheel_min_size);

   object_data[obj_idx].obj_class = PA_OBJ_CLASS_2WHEEL;
   object_data[obj_idx].length    = len - EPSILON;
   object_data[obj_idx].width     = width - EPSILON;
   object_data[obj_idx].speed     = lcda_cals.k_lcda_2wheel_min_speed + EPSILON;

   /** \action Call Lcda_Is_VRU_Object_Valid to evaluate if object is valid for LCDA. */
   result = Lcda_Is_VRU_Object_Valid(&object_data[obj_idx], &lcda_cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}

/**
 * Check that an 2wheel object is classified as valid for LCDA.
 * \uts{CSCSA-141108} \sdd{CSCSA-141096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_VRU_Object_Valid__returns_TRUE_if_2wheel_shall_be_valid)
{
   /** \arrange Set up calibrations and tracker output such that object is valid. */
   uint8_t obj_idx = 2u;
   boolean_T result;

   object_data[obj_idx].obj_class = PA_OBJ_CLASS_2WHEEL;
   object_data[obj_idx].length    = 3.0f;
   object_data[obj_idx].width     = 1.2f;
   object_data[obj_idx].speed     = lcda_cals.k_lcda_2wheel_min_speed + EPSILON;

   /** \action Call Lcda_Is_VRU_Object_Valid to evaluate if object is valid for LCDA. */
   result = Lcda_Is_VRU_Object_Valid(&object_data[obj_idx], &lcda_cals);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}

/**
 * Check that an 2wheel object is classified as invalid for LCDA.
 * \uts{CSCSA-141109} \sdd{CSCSA-141096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_VRU_Object_Valid__returns_FALSE_if_2wheel_object_is_invalid)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid. */
   uint8_t obj_idx = 2u;
   boolean_T result;
   float32_T len   = Fast_Sqrt(lcda_cals.k_lcda_2wheel_min_size);
   float32_T width = Fast_Sqrt(lcda_cals.k_lcda_2wheel_min_size);


   object_data[obj_idx].obj_class = PA_OBJ_CLASS_2WHEEL;
   object_data[obj_idx].length    = len - EPSILON;
   object_data[obj_idx].width     = width - EPSILON;
   object_data[obj_idx].speed     = lcda_cals.k_lcda_2wheel_min_speed - EPSILON;

   /** \action Call Lcda_Is_VRU_Object_Valid to evaluate if object is valid for LCDA. */
   result = Lcda_Is_VRU_Object_Valid(&object_data[obj_idx], &lcda_cals);

   /** \assert Verify that object is classified as invalid. */
   EXPECT_FALSE(result);
}

/**
 * Check that LCDA persistent data is initialized properly.
 * \uts{CSCSA-42871} \sdd{SF-6537} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Init_Persistent__works_properly)
{
   /** \arrange Set up LCDA persistent data with non-default values. */
   lcda_persistent.f_bsw_prev_reset                 = FBK_TRUE;
   lcda_persistent.f_cvw_prev_reset                 = FBK_TRUE;
   lcda_persistent.f_host_speed_in_activation_range = FBK_TRUE;
   // lcda_persistent.f_lcda_disabled_low_curve_radius = FBK_TRUE;
   lcda_persistent.f_curve_radius_valid     = FBK_TRUE;
   lcda_persistent.turn_signal_held         = TURN_SIGNAL_LEFT;
   lcda_persistent.turn_signal_held_counter = 3u;

   /** \action Call Lcda_Init_Persistent to initialize LCDA persistent data. */
   Lcda_Init_Persistent(&lcda_persistent);

   /** \assert Verify that all LCDA persistent data are set to default values. */
   EXPECT_FALSE(lcda_persistent.f_bsw_prev_reset);
   EXPECT_FALSE(lcda_persistent.f_cvw_prev_reset);
   EXPECT_FALSE(lcda_persistent.f_host_speed_in_activation_range);
   // EXPECT_FALSE(lcda_persistent.f_lcda_disabled_low_curve_radius);
   EXPECT_FALSE(lcda_persistent.f_curve_radius_valid);
   EXPECT_EQ(lcda_persistent.turn_signal_held, TURN_SIGNAL_NONE);
   EXPECT_EQ(lcda_persistent.turn_signal_held_counter, FBK_ZERO_UINT);
}

/**
 * Check that BSW is enabled, if this is decided based on calibration values and those are set accordingly.
 * \uts{CSCSA-42872} \sdd{SF-6538} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Bsw_Activated__returns_true_if_enabled_via_cals)
{
   /** \arrange Set up calibration values and input signal such that BSW is enabled by p_lcda_cals */
   boolean_T f_enabled = FBK_FALSE;

   lcda_cals.k_bsw_enable_via_cal              = FBK_TRUE;
   lcda_cals.k_bsw_enable                      = FBK_TRUE;
   lcda_core_input.enabled_flags.f_bsw_enabled = FBK_FALSE;

   /** \action Call Lcda_Is_Bsw_Activated to determine if BSW is enabled. */
   f_enabled = Lcda_Is_Bsw_Activated(&lcda_cals, &lcda_core_input);

   /** \assert Verify that true is returned. */
   EXPECT_TRUE(f_enabled);
}

/**
 * Check that BSW is enabled, if this is decided based on input signal.
 * \uts{CSCSA-70877} \sdd{SF-6538} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Bsw_Activated__returns_true_if_enabled_via_input)
{
   /** \arrange Set up calibration values and input signal such that BSW is enabled by Lcda_Core_Input_T */
   boolean_T f_enabled = FBK_FALSE;

   lcda_cals.k_bsw_enable_via_cal              = FBK_FALSE;
   lcda_cals.k_bsw_enable                      = FBK_FALSE;
   lcda_core_input.enabled_flags.f_bsw_enabled = FBK_TRUE;

   /** \action Call Lcda_Is_Bsw_Activated to determine if BSW is enabled. */
   f_enabled = Lcda_Is_Bsw_Activated(&lcda_cals, &lcda_core_input);

   /** \assert Verify that true is returned. */
   EXPECT_TRUE(f_enabled);
}

/**
 * Check that BSW is disabled, if this is decided based on input signal.
 * \uts{CSCSA-70878} \sdd{SF-6538} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Bsw_Activated__returns_true_if_disabled_by_input)
{
   /** \arrange Set up calibration values and input signal such that BSW is disabled */
   boolean_T f_enabled = FBK_FALSE;

   lcda_cals.k_bsw_enable_via_cal              = FBK_FALSE;
   lcda_cals.k_bsw_enable                      = FBK_FALSE;
   lcda_core_input.enabled_flags.f_bsw_enabled = FBK_FALSE;


   /** \action Call Lcda_Is_Bsw_Activated to determine if BSW is disabled. */
   f_enabled = Lcda_Is_Bsw_Activated(&lcda_cals, &lcda_core_input);

   /** \assert Verify that true is returned. */
   EXPECT_FALSE(f_enabled);
}

/**
 * Check that BSW is disabled, if this is decided based on calibration value.
 * \uts{CSCSA-70879} \sdd{SF-6538} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Bsw_Activated__returns_true_if_disabled_by_calibration)
{
   /** \arrange Set up calibration values and input signal such that BSW is disabled */
   boolean_T f_enabled = FBK_FALSE;

   lcda_cals.k_bsw_enable_via_cal              = FBK_TRUE;
   lcda_cals.k_bsw_enable                      = FBK_FALSE;
   lcda_core_input.enabled_flags.f_bsw_enabled = FBK_FALSE;


   /** \action Call Lcda_Is_Bsw_Activated to determine if BSW is disabled. */
   f_enabled = Lcda_Is_Bsw_Activated(&lcda_cals, &lcda_core_input);

   /** \assert Verify that true is returned. */
   EXPECT_FALSE(f_enabled);
}

/**
 * Check that CVW is enabled, if this is decided based on calibration values and those are set accordingly.
 * \uts{CSCSA-42873} \sdd{SF-6539} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Cvw_Activated__returns_true_if_enabled_via_cals)
{
   /** \arrange Set up calibration values and input signal such that CVW is enabled. */
   boolean_T f_enabled                         = FBK_FALSE;
   lcda_cals.k_cvw_enable_via_cal              = FBK_TRUE;
   lcda_cals.k_cvw_enable                      = FBK_TRUE;
   lcda_core_input.enabled_flags.f_cvw_enabled = FBK_FALSE;

   /** \action Call Lcda_Is_Cvw_Activated to determine if CVW is enabled. */
   f_enabled = Lcda_Is_Cvw_Activated(&lcda_cals, &lcda_core_input);

   /** \assert Verify that true is returned. */
   EXPECT_TRUE(f_enabled);
}

/**
 * Check that CVW is enabled, if this is decided based on input signal and this is set accordingly.
 * \uts{CSCSA-70880} \sdd{SF-6539} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Cvw_Activated__returns_true_if_enabled_via_input)
{
   /** \arrange Set up calibration values and input signal such that CVW is enabled. */
   boolean_T f_enabled                         = FBK_FALSE;
   lcda_cals.k_cvw_enable_via_cal              = FBK_FALSE;
   lcda_cals.k_cvw_enable                      = FBK_FALSE;
   lcda_core_input.enabled_flags.f_cvw_enabled = FBK_TRUE;

   /** \action Call Lcda_Is_Cvw_Activated to determine if CVW is enabled. */
   f_enabled = Lcda_Is_Cvw_Activated(&lcda_cals, &lcda_core_input);

   /** \assert Verify that true is returned. */
   EXPECT_TRUE(f_enabled);
}

/**
 * Check that CVW is disabled, if this is decided based on input signal and this is set accordingly.
 * \uts{CSCSA-70881} \sdd{SF-6539} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Cvw_Activated__returns_true_if_disabled_via_input)
{
   /** \arrange Set up calibration values and input signal such that CVW is disabled. */
   boolean_T f_enabled                         = FBK_FALSE;
   lcda_cals.k_cvw_enable_via_cal              = FBK_FALSE;
   lcda_cals.k_cvw_enable                      = FBK_FALSE;
   lcda_core_input.enabled_flags.f_cvw_enabled = FBK_FALSE;


   /** \action Call Lcda_Is_Cvw_Activated to determine if CVW is disabled. */
   f_enabled = Lcda_Is_Cvw_Activated(&lcda_cals, &lcda_core_input);

   /** \assert Verify that true is returned. */
   EXPECT_FALSE(f_enabled);
}

/**
 * Check that CVW is disabled, if this is decided based calibration values and those are set accordingly.
 * \uts{CSCSA-70882} \sdd{SF-6539} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Cvw_Activated__returns_true_if_disabled_via_calibrations)
{
   /** \arrange Set up calibration values and input signal such that CVW is disabled. */
   boolean_T f_enabled                         = FBK_FALSE;
   lcda_cals.k_cvw_enable_via_cal              = FBK_FALSE;
   lcda_cals.k_cvw_enable                      = FBK_FALSE;
   lcda_core_input.enabled_flags.f_cvw_enabled = FBK_FALSE;

   /** \action Call Lcda_Is_Cvw_Activated to determine if CVW is disabled. */
   f_enabled = Lcda_Is_Cvw_Activated(&lcda_cals, &lcda_core_input);

   /** \assert Verify that true is returned. */
   EXPECT_FALSE(f_enabled);
}

/**
 * Check that SLC is enabled, if this is decided based on calibration values and those are set accordingly.
 * \uts{CSCSA-42874} \sdd{SF-6543} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Slc_Activated__returns_true_if_enabled_via_cals)
{
   /** \arrange Set up calibration values and input signal such that SLC is enabled. */
   boolean_T f_enabled = FBK_FALSE;

   lcda_cals.k_slc_enable_via_cal              = FBK_TRUE;
   lcda_cals.k_slc_enable                      = FBK_TRUE;
   lcda_core_input.enabled_flags.f_slc_enabled = FBK_FALSE;


   /** \action Call Lcda_Is_Slc_Activated to determine if SLC is enabled. */
   f_enabled = Lcda_Is_Slc_Activated(&lcda_cals, &lcda_core_input);

   /** \assert Verify that true is returned. */
   EXPECT_TRUE(f_enabled);
}

/**
 * Check that SLC is enabled, if this is decided based on input signal and this is set accordingly.
 * \uts{CSCSA-70883} \sdd{SF-6543} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Slc_Activated__returns_true_if_enabled_via_input)
{
   /** \arrange Set up calibration values and input signal such that CVW is enabled. */
   boolean_T f_enabled = FBK_FALSE;

   lcda_cals.k_slc_enable_via_cal              = FBK_FALSE;
   lcda_cals.k_slc_enable                      = FBK_FALSE;
   lcda_core_input.enabled_flags.f_slc_enabled = FBK_TRUE;


   /** \action Call Lcda_Is_Slc_Activated to determine if SLC is enabled. */
   f_enabled = Lcda_Is_Slc_Activated(&lcda_cals, &lcda_core_input);

   /** \assert Verify that true is returned. */
   EXPECT_TRUE(f_enabled);
}

/**
 * Check that SLC is disabled, if this is decided based on input signal and this is set accordingly.
 * \uts{CSCSA-70884} \sdd{SF-6543} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Slc_Activated__returns_true_if_disabled_via_input)
{
   /** \arrange Set up calibration values and input signal such that SLC is disabled. */
   boolean_T f_enabled = FBK_FALSE;

   lcda_cals.k_slc_enable_via_cal              = FBK_FALSE;
   lcda_cals.k_slc_enable                      = FBK_FALSE;
   lcda_core_input.enabled_flags.f_slc_enabled = FBK_FALSE;


   /** \action Call Lcda_Is_Slc_Activated to determine if SLC is disabled. */
   f_enabled = Lcda_Is_Slc_Activated(&lcda_cals, &lcda_core_input);

   /** \assert Verify that true is returned. */
   EXPECT_FALSE(f_enabled);
}

/**
 * Check that SLC is disabled, if this is decided based calibration values and those are set accordingly.
 * \uts{CSCSA-70885} \sdd{SF-6543} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Slc_Activated__returns_true_if_disabled_via_calibrations)
{
   /** \arrange Set up calibration values and input signal such that SLC is disabled. */
   boolean_T f_enabled = FBK_FALSE;

   lcda_cals.k_slc_enable_via_cal              = FBK_FALSE;
   lcda_cals.k_slc_enable                      = FBK_FALSE;
   lcda_core_input.enabled_flags.f_slc_enabled = FBK_FALSE;


   /** \action Call Lcda_Is_Slc_Activated to determine if SLC is disabled. */
   f_enabled = Lcda_Is_Slc_Activated(&lcda_cals, &lcda_core_input);

   /** \assert Verify that true is returned. */
   EXPECT_FALSE(f_enabled);
}

/**
 * Check that ELC is enabled, if this is decided based on calibration values and those are set accordingly.
 * \uts{CSCSA-42875} \sdd{SF-6540} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Elc_Activated__returns_true_if_enabled_via_cals)
{
   /** \arrange Set up calibration values and input signal such that ELC is enabled. */
   boolean_T f_enabled = FBK_FALSE;

   lcda_cals.k_elc_enable_via_cal              = FBK_TRUE;
   lcda_cals.k_elc_enable                      = FBK_TRUE;
   lcda_core_input.enabled_flags.f_elc_enabled = FBK_FALSE;


   /** \action Call Lcda_Is_Elc_Activated to determine if ELC is enabled. */
   f_enabled = Lcda_Is_Elc_Activated(&lcda_cals, &lcda_core_input);

   /** \assert Verify that true is returned. */
   EXPECT_TRUE(f_enabled);
}

/**
 * Check that Elc is enabled, if this is decided based on input signal and this is set accordingly.
 * \uts{CSCSA-70886} \sdd{SF-6540} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Elc_Activated__returns_true_if_enabled_via_input)
{
   /** \arrange Set up calibration values and input signal such that ELC is enabled. */
   boolean_T f_enabled = FBK_FALSE;

   lcda_cals.k_elc_enable_via_cal              = FBK_FALSE;
   lcda_cals.k_elc_enable                      = FBK_FALSE;
   lcda_core_input.enabled_flags.f_elc_enabled = FBK_TRUE;


   /** \action Call Lcda_Is_Elc_Activated to determine if ELC is enabled. */
   f_enabled = Lcda_Is_Elc_Activated(&lcda_cals, &lcda_core_input);

   /** \assert Verify that true is returned. */
   EXPECT_TRUE(f_enabled);
}

/**
 * Check that ELC is disabled, if this is decided based on input signal and this is set accordingly.
 * \uts{CSCSA-70887} \sdd{SF-6540} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Elc_Activated__returns_true_if_disabled_via_input)
{
   /** \arrange Set up calibration values and input signal such that ELC is disabled. */
   boolean_T f_enabled = FBK_FALSE;

   lcda_cals.k_elc_enable_via_cal              = FBK_FALSE;
   lcda_cals.k_elc_enable                      = FBK_FALSE;
   lcda_core_input.enabled_flags.f_elc_enabled = FBK_FALSE;


   /** \action Call Lcda_Is_Elc_Activated to determine if ELC is disabled. */
   f_enabled = Lcda_Is_Elc_Activated(&lcda_cals, &lcda_core_input);

   /** \assert Verify that true is returned. */
   EXPECT_FALSE(f_enabled);
}

/**
 * Check that ELC is disabled, if this is decided based calibration values and those are set accordingly.
 * \uts{CSCSA-70888} \sdd{SF-6540} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Elc_Activated__returns_true_if_disabled_via_calibrations)
{
   /** \arrange Set up calibration values and input signal such that ELC is disabled. */
   boolean_T f_enabled = FBK_FALSE;

   lcda_cals.k_elc_enable_via_cal              = FBK_FALSE;
   lcda_cals.k_elc_enable                      = FBK_FALSE;
   lcda_core_input.enabled_flags.f_elc_enabled = FBK_FALSE;


   /** \action Call Lcda_Is_Elc_Activated to determine if ELC is disabled. */
   f_enabled = Lcda_Is_Elc_Activated(&lcda_cals, &lcda_core_input);

   /** \assert Verify that true is returned. */
   EXPECT_FALSE(f_enabled);
}

/**
 * Check that LCDA status is active, if ego speed and curve radius are within defined boundaries.
 * \uts{CSCSA-42876} \sdd{SF-6534} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Get_Activation_Status__returns_active_lcda_status_if_ego_speed_and_curve_radius_are_valid)
{
   /** \arrange Set up persistent data flags for ego speed and curve radius. */
   Lcda_Status_T lcda_status = LCDA_STATUS_DEACTIVATED_INTERNAL_ERROR;

   lcda_persistent.f_host_speed_in_activation_range = FBK_TRUE;
   lcda_persistent.f_curve_radius_valid             = FBK_TRUE;

   /** \action Call Lcda_Get_Activation_Status to get Lcda activation status. */
   lcda_status = Lcda_Get_Activation_Status(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that LCDA status is active. */
   EXPECT_EQ(lcda_status, LCDA_STATUS_ACTIVE);
}

/**
 * Check that LCDA status is active, if ego speed and curve radius are within defined boundaries.
 * \uts{CSCSA-187420} \sdd{SF-6534} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Get_Activation_Status__returns_inactive_lcda_status_if_radius_invalid)
{
   /** \arrange Set up persistent data flags for ego speed and curve radius. */
   Lcda_Status_T lcda_status = LCDA_STATUS_DEACTIVATED_INTERNAL_ERROR;

   lcda_persistent.f_host_speed_in_activation_range     = FBK_TRUE;
   lcda_persistent.f_curve_radius_valid                 = FBK_FALSE;
   lcda_cals.k_lcda_f_disable_due_to_small_curve_radius = FBK_TRUE;

   /** \action Call Lcda_Get_Activation_Status to get Lcda activation status. */
   lcda_status = Lcda_Get_Activation_Status(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that LCDA status is active. */
   EXPECT_EQ(lcda_status, LCDA_STATUS_DEACTIVATED_LOW_CURVE_RADIUS);
}

/**
 * Check that LCDA status is deactivated due to high speed.
 * \uts{CSCSA-42893} \sdd{SF-6534} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Get_Activation_Status__high_speed)
{
   /** \arrange Set up persistent data flags for ego speed. */
   Lcda_Status_T lcda_status;
   lcda_persistent.f_host_speed_in_activation_range = FBK_FALSE;
   p_vehicle_data->host_speed                       = lcda_cals.k_lcda_host_activation_speed_max + EPSILON;

   /** \action Call Lcda_Get_Activation_Status to get Lcda activation status. */
   lcda_status = Lcda_Get_Activation_Status(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that LCDA status is not active. */
   EXPECT_EQ(lcda_status, LCDA_STATUS_DEACTIVATED_HIGH_EGO_SPEED);
}

/**
 * Check that LCDA status is deactivated due to low speed.
 * \uts{CSCSA-70889} \sdd{SF-6534} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Get_Activation_Status__low_speed)
{
   /** \arrange Set up persistent data flags for ego speed. */
   Lcda_Status_T lcda_status;
   lcda_persistent.f_host_speed_in_activation_range = FBK_FALSE;
   p_vehicle_data->host_speed                       = lcda_cals.k_lcda_host_activation_speed_min - EPSILON;

   /** \action Call Lcda_Get_Activation_Status to get Lcda activation status. */
   lcda_status = Lcda_Get_Activation_Status(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that LCDA status is not active. */
   EXPECT_EQ(lcda_status, LCDA_STATUS_DEACTIVATED_LOW_EGO_SPEED);
}

/**
 * Check that LCDA status is internal error.
 * \uts{CSCSA-42894} \sdd{SF-6534} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Get_Activation_Status__internal_error)
{
   /** \arrange Set up persistent data flags for ego speed. */
   Lcda_Status_T lcda_status;
   lcda_persistent.f_host_speed_in_activation_range = FBK_FALSE;
   p_vehicle_data->host_speed                       = lcda_cals.k_lcda_host_activation_speed_max - EPSILON;

   /** \action Call Lcda_Get_Activation_Status to get Lcda activation status. */
   lcda_status = Lcda_Get_Activation_Status(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that LCDA status is not active. */
   EXPECT_EQ(lcda_status, LCDA_STATUS_DEACTIVATED_INTERNAL_ERROR);
}


/**
 * Check that LCDA status is deactivated, if curve radius is below defined boundaries.
 * \uts{CSCSA-70890} \sdd{SF-6534} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Get_Activation_Status__returns_deactivated_if_curve_radius_invalid)
{
   /** \arrange Set up persistent data flags for ego speed and curve radius. */
   Lcda_Status_T lcda_status = LCDA_STATUS_DEACTIVATED_INTERNAL_ERROR;

   lcda_persistent.f_host_speed_in_activation_range     = FBK_TRUE;
   lcda_persistent.f_curve_radius_valid                 = FBK_FALSE;
   lcda_cals.k_lcda_f_disable_due_to_small_curve_radius = FBK_TRUE;

   /** \action Call Lcda_Get_Activation_Status to get Lcda activation status. */
   lcda_status = Lcda_Get_Activation_Status(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that LCDA status is not active. */
   EXPECT_EQ(lcda_status, LCDA_STATUS_DEACTIVATED_LOW_CURVE_RADIUS);
}


/**
 * Check that LCDA status is enabled, if this is decided based on calibration values and those are set accordingly.
 * \uts{CSCSA-42877} \sdd{SF-6535} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Get_Enable_Status__returns_enabled_lcda_status_if_enabled_via_cals)
{
   /** \arrange Set up calibration values such that LCDA is enabled by p_lcda_cals */
   Lcda_Status_T lcda_status = LCDA_STATUS_DEACTIVATED_INTERNAL_ERROR;

   lcda_cals.k_lcda_enable_via_cal              = FBK_TRUE;
   lcda_cals.k_lcda_enable                      = FBK_TRUE;
   lcda_core_input.enabled_flags.f_lcda_enabled = FBK_FALSE;

   /** \action Call Lcda_Get_Enable_Status to get Lcda enabled status. */
   lcda_status = Lcda_Get_Enable_Status(&lcda_cals, &lcda_core_input);

   /** \assert Verify that LCDA status is enabled. */
   EXPECT_EQ(lcda_status, LCDA_STATUS_ENABLED);
}

/**
 * Check curve radius is valid, if its curve radius is positive and absolute value is above histeresis when the hysteresis is NOT
 * applied. \uts{CSCSA-68138} \sdd{CSCSA-46128} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test,
       Lcda_Is_Curve_Radius_Valid__returns_true_if_curve_radius_is_positive_and_above_histeresis_and_histeresis_is_not_applied)
{
   /** \arrange Set up calibration values, vehicle data and persistent data. */
   boolean_T result = FBK_TRUE;

   lcda_cals.k_lcda_min_curve_radius     = 4.7f;
   lcda_cals.k_lcda_min_curve_radius_hys = 0.2f;
   p_vehicle_data->curvature             = 0.2f; // curve radius = (1.0/0.2)=5.0
   lcda_persistent.f_curve_radius_valid  = FBK_TRUE;

   /** \action Call Lcda_Is_Curve_Radius_Valid. */
   result = Lcda_Is_Curve_Radius_Valid(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that curve radius is valid. */
   EXPECT_TRUE(result);
}

/**
 * Check curve radius is valid, if its curve radius is negative and absolute value is above histeresis when the hysteresis is NOT
 * applied. \uts{CSCSA-68139} \sdd{CSCSA-46128} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test,
       Lcda_Is_Curve_Radius_Valid__returns_true_if_curve_radius_is_negative_and_above_histeresis_and_histeresis_is_not_applied)
{
   /** \arrange Set up calibration values, vehicle data and persistent data. */
   boolean_T result = FBK_TRUE;

   lcda_cals.k_lcda_min_curve_radius     = 4.7f;
   lcda_cals.k_lcda_min_curve_radius_hys = 0.2f;
   p_vehicle_data->curvature             = -0.2f; // curve radius = (1.0/0.2)=5.0
   lcda_persistent.f_curve_radius_valid  = FBK_TRUE;

   /** \action Call Lcda_Is_Curve_Radius_Valid. */
   result = Lcda_Is_Curve_Radius_Valid(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that curve radius is valid. */
   EXPECT_TRUE(result);
}

/**
 * Check curve radius is valid, if its curve radius is positive and absolute value is between threshold and histeresis when the
 * hysteresis is NOT applied. \uts{CSCSA-68140} \sdd{CSCSA-46128} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test,
       Lcda_Is_Curve_Radius_Valid__returns_true_if_curve_radius_is_positive_and_below_histeresis_and_histeresis_is_not_applied)
{
   /** \arrange Set up calibration values, vehicle data and persistent data. */
   boolean_T result = FBK_TRUE;

   lcda_cals.k_lcda_min_curve_radius     = 4.8f;
   lcda_cals.k_lcda_min_curve_radius_hys = 0.5f;
   p_vehicle_data->curvature             = 0.2f; // curve radius = (1.0/0.2)=5.0
   lcda_persistent.f_curve_radius_valid  = FBK_TRUE;

   /** \action Call Lcda_Is_Curve_Radius_Valid. */
   result = Lcda_Is_Curve_Radius_Valid(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that curve radius is valid. */
   EXPECT_TRUE(result);
}

/**
 * Check curve radius is valid, if its curve radius is negative and absolute value is between threshold and histeresis when the
 * hysteresis is NOT applied. \uts{CSCSA-68141} \sdd{CSCSA-46128} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test,
       Lcda_Is_Curve_Radius_Valid__returns_true_if_curve_radius_is_negative_and_below_histeresis_and_histeresis_is_not_applied)
{
   /** \arrange Set up calibration values, vehicle data and persistent data. */
   boolean_T result = FBK_TRUE;

   lcda_cals.k_lcda_min_curve_radius     = 4.8f;
   lcda_cals.k_lcda_min_curve_radius_hys = 0.5f;
   p_vehicle_data->curvature             = -0.2f; // curve radius = (1.0/0.2)=5.0
   lcda_persistent.f_curve_radius_valid  = FBK_TRUE;

   /** \action Call Lcda_Is_Curve_Radius_Valid. */
   result = Lcda_Is_Curve_Radius_Valid(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that curve radius is valid. */
   EXPECT_TRUE(result);
}

/**
 * Check curve radius is invalid, if its curve radius is positive and absolute value is below threshold when the hysteresis is NOT
 * applied. \uts{CSCSA-68142} \sdd{CSCSA-46128} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test,
       Lcda_Is_Curve_Radius_Valid__returns_false_if_curve_radius_is_positive_and_below_threshold_and_histeresis_is_not_applied)
{
   /** \arrange Set up calibration values, vehicle data and persistent data. */
   boolean_T result = FBK_TRUE;

   lcda_cals.k_lcda_min_curve_radius     = 2.0f;
   lcda_cals.k_lcda_min_curve_radius_hys = 0.2f;
   p_vehicle_data->curvature             = 2.0f; // curve_radius = 0.5
   lcda_persistent.f_curve_radius_valid  = FBK_TRUE;

   /** \action Call Lcda_Is_Curve_Radius_Valid. */
   result = Lcda_Is_Curve_Radius_Valid(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that curve radius is invalid. */
   EXPECT_FALSE(result);
}

/**
 * Check curve radius is invalid, if its curve radius is negative and absolute value is below threshold when the hysteresis is NOT
 * applied. \uts{CSCSA-68143} \sdd{CSCSA-46128} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test,
       Lcda_Is_Curve_Radius_Valid__returns_false_if_curve_radius_is_negative_and_below_threshold_and_histeresis_is_not_applied)
{
   /** \arrange Set up calibration values, vehicle data and persistent data. */
   boolean_T result = FBK_TRUE;

   lcda_cals.k_lcda_min_curve_radius     = 2.0f;
   lcda_cals.k_lcda_min_curve_radius_hys = 0.2f;
   p_vehicle_data->curvature             = -2.0f; // curve_radius = 0.5
   lcda_persistent.f_curve_radius_valid  = FBK_TRUE;

   /** \action Call Lcda_Is_Curve_Radius_Valid. */
   result = Lcda_Is_Curve_Radius_Valid(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that curve radius is invalid. */
   EXPECT_FALSE(result);
}

/**
 * Check curve radius is valid, if its curve radius is positive and absolute value is above histeresis when the hysteresis is
 * applied. \uts{CSCSA-68144} \sdd{CSCSA-46128} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Curve_Radius_Valid__returns_true_if_curve_radius_is_positive_and_above_histeresis_and_histeresis_is_applied)
{
   /** \arrange Set up calibration values, vehicle data and persistent data. */
   boolean_T result = FBK_TRUE;

   lcda_cals.k_lcda_min_curve_radius     = 4.7f;
   lcda_cals.k_lcda_min_curve_radius_hys = 0.2f;
   p_vehicle_data->curvature             = 0.2f; // curve radius = (1.0/0.2)=5.0
   lcda_persistent.f_curve_radius_valid  = FBK_FALSE;

   /** \action Call Lcda_Is_Curve_Radius_Valid. */
   result = Lcda_Is_Curve_Radius_Valid(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that curve radius is valid. */
   EXPECT_TRUE(result);
}

/**
 * Check curve radius is valid, if its curve radius is negative and absolute value is above histeresis when the hysteresis is
 * applied. \uts{CSCSA-68145} \sdd{CSCSA-46128} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Curve_Radius_Valid__returns_true_if_curve_radius_is_negative_and_above_histeresis_and_histeresis_is_applied)
{
   /** \arrange Set up calibration values, vehicle data and persistent data. */
   boolean_T result = FBK_TRUE;

   lcda_cals.k_lcda_min_curve_radius     = 4.7f;
   lcda_cals.k_lcda_min_curve_radius_hys = 0.2f;
   p_vehicle_data->curvature             = -0.2f; // curve radius = (1.0/0.2)=5.0
   lcda_persistent.f_curve_radius_valid  = FBK_FALSE;

   /** \action Call Lcda_Is_Curve_Radius_Valid. */
   result = Lcda_Is_Curve_Radius_Valid(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that curve radius is valid. */
   EXPECT_TRUE(result);
}

/**
 * Check curve radius is invalid, if its curve radius is positive and absolute value is between threshold and histeresis when the
 * hysteresis is applied. \uts{CSCSA-68146} \sdd{CSCSA-46128} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Curve_Radius_Valid__returns_true_if_curve_radius_is_positive_and_below_histeresis_and_histeresis_is_applied)
{
   /** \arrange Set up calibration values, vehicle data and persistent data. */
   boolean_T result = FBK_TRUE;

   lcda_cals.k_lcda_min_curve_radius     = 4.8f;
   lcda_cals.k_lcda_min_curve_radius_hys = 0.5f;
   p_vehicle_data->curvature             = 0.2f; // curve radius = (1.0/0.2)=5.0
   lcda_persistent.f_curve_radius_valid  = FBK_FALSE;

   /** \action Call Lcda_Is_Curve_Radius_Valid. */
   result = Lcda_Is_Curve_Radius_Valid(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that curve radius is invalid. */
   EXPECT_FALSE(result);
}

/**
 * Check curve radius is invalid, if its curve radius is negative and absolute value is between threshold and histeresis when the
 * hysteresis is applied. \uts{CSCSA-68147} \sdd{CSCSA-46128} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Curve_Radius_Valid__returns_true_if_curve_radius_is_negative_and_below_histeresis_and_histeresis_is_applied)
{
   /** \arrange Set up calibration values, vehicle data and persistent data. */
   boolean_T result = FBK_TRUE;

   lcda_cals.k_lcda_min_curve_radius     = 4.8f;
   lcda_cals.k_lcda_min_curve_radius_hys = 0.5f;
   p_vehicle_data->curvature             = -0.2f; // curve radius = (1.0/0.2)=5.0
   lcda_persistent.f_curve_radius_valid  = FBK_FALSE;

   /** \action Call Lcda_Is_Curve_Radius_Valid. */
   result = Lcda_Is_Curve_Radius_Valid(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that curve radius is invalid. */
   EXPECT_FALSE(result);
}

/**
 * Check curve radius is invalid, if its curve radius is positive and absolute value is below threshold when the hysteresis is
 * applied. \uts{CSCSA-68148} \sdd{CSCSA-46128} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Curve_Radius_Valid__returns_true_if_curve_radius_is_positive_and_below_threshold_and_histeresis_is_applied)
{
   /** \arrange Set up calibration values, vehicle data and persistent data. */
   boolean_T result = FBK_TRUE;

   lcda_cals.k_lcda_min_curve_radius     = 2.0f;
   lcda_cals.k_lcda_min_curve_radius_hys = 0.2f;
   p_vehicle_data->curvature             = 2.0f; // curve_radius = 0.5
   lcda_persistent.f_curve_radius_valid  = FBK_FALSE;

   /** \action Call Lcda_Is_Curve_Radius_Valid. */
   result = Lcda_Is_Curve_Radius_Valid(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that curve radius is invalid. */
   EXPECT_FALSE(result);
}

/**
 * Check curve radius is invalid, if its curve radius is negative and absolute value is below threshold when the hysteresis is
 * applied. \uts{CSCSA-68149} \sdd{CSCSA-46128} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Curve_Radius_Valid__returns_true_if_curve_radius_is_negative_and_below_threshold_and_histeresis_is_applied)
{
   /** \arrange Set up calibration values, vehicle data and persistent data. */
   boolean_T result = FBK_TRUE;

   lcda_cals.k_lcda_min_curve_radius     = 2.0f;
   lcda_cals.k_lcda_min_curve_radius_hys = 0.2f;
   p_vehicle_data->curvature             = -2.0f; // curve_radius = 0.5
   lcda_persistent.f_curve_radius_valid  = FBK_FALSE;

   /** \action Call Lcda_Is_Curve_Radius_Valid. */
   result = Lcda_Is_Curve_Radius_Valid(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that curve radius is invalid. */
   EXPECT_FALSE(result);
}

/**
 * Check that curve radius is valid, if its value is equal to zero, when the hysteresis is NOT applied. zero-value radius refers to
 * straight line. \uts{CSCSA-68150} \sdd{CSCSA-46128} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Curve_Radius_Valid__returns_true_if_curve_radius_zero_and_hysteresis_is_not_applied)
{
   /** \arrange Set up calibration values, vehicle data and persistent data. */
   boolean_T result = FBK_TRUE;

   lcda_cals.k_lcda_min_curve_radius     = 2.0f;
   lcda_cals.k_lcda_min_curve_radius_hys = 0.2f;
   p_vehicle_data->curvature             = 0.0f;
   lcda_persistent.f_curve_radius_valid  = FBK_TRUE;

   /** \action Call Lcda_Is_Curve_Radius_Valid. */
   result = Lcda_Is_Curve_Radius_Valid(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that curve radius is valid. */
   EXPECT_TRUE(result);
}

/**
 * Check that curve radius is valid, if its value is equal to zero, when the hysteresis is applied. zero-value radius refers to
 * straight line. \uts{CSCSA-42879} \sdd{CSCSA-46128} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Curve_Radius_Valid__returns_true_if_curve_radius_zero_and_hysteresis_is_applied)
{
   /** \arrange Set up calibration values, vehicle data and persistent data. */
   boolean_T result = FBK_TRUE;

   lcda_cals.k_lcda_min_curve_radius     = 2.0f;
   lcda_cals.k_lcda_min_curve_radius_hys = 0.2f;
   p_vehicle_data->curvature             = 0.0f;
   lcda_persistent.f_curve_radius_valid  = FBK_FALSE;

   /** \action Call Lcda_Is_Curve_Radius_Valid to verify curve radius. */
   result = Lcda_Is_Curve_Radius_Valid(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that curve radius is considered as valid. */
   EXPECT_TRUE(result);
}


/**
 * Check that a turn signal from the vehicle data is properly filled into the LCDA persistent data.
 * \uts{CSCSA-42880} \sdd{SF-6546} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Process_Veh_Turn_Signal__fills_turn_signal_from_vehicle_data_to_persistent_data)
{
   /** \arrange Set up vehicle data such that there is a turn signal left. */
   p_vehicle_data->turn_signal              = TURN_SIGNAL_LEFT;
   lcda_persistent.turn_signal_held         = TURN_SIGNAL_NONE;
   lcda_persistent.turn_signal_held_counter = 4u;

   /** \action Call Lcda_Process_Veh_Turn_Signal to process the turn signal from the vehicle data. */
   Lcda_Process_Veh_Turn_Signal(p_vehicle_data, &lcda_cals, &lcda_persistent);

   /** \assert Verify that turn signal from vehicle data is filled to persistent data and holding counter is reseted. */
   EXPECT_EQ(lcda_persistent.turn_signal_held_counter, FBK_ZERO_UINT);
   EXPECT_EQ(lcda_persistent.turn_signal_held, TURN_SIGNAL_LEFT);
}

/**
 * Check that a turn signal from the LCDA persistent data is held if holding counter is below threshold.
 * \uts{CSCSA-42881} \sdd{SF-6546} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Process_Veh_Turn_Signal__holds_turn_signal_from_persistent_data_if_below_threshold)
{
   /** \arrange Set up vehicle data such that there is no signal left. Set up persistent data such that there is a held turn signal
    * below threshold of holding counter. */
   p_vehicle_data->turn_signal               = TURN_SIGNAL_NONE;
   lcda_persistent.turn_signal_held          = TURN_SIGNAL_LEFT;
   lcda_cals.k_lcda_turn_signal_coast_cycles = 3u;
   lcda_persistent.turn_signal_held_counter  = lcda_cals.k_lcda_turn_signal_coast_cycles - 1u;

   /** \action Call Lcda_Process_Veh_Turn_Signal to process the turn signal from the vehicle data. */
   Lcda_Process_Veh_Turn_Signal(p_vehicle_data, &lcda_cals, &lcda_persistent);

   /** \assert Verify that turn signal from persistent data is held and holding counter is increased. */
   EXPECT_EQ(lcda_persistent.turn_signal_held_counter, lcda_cals.k_lcda_turn_signal_coast_cycles);
   EXPECT_EQ(lcda_persistent.turn_signal_held, TURN_SIGNAL_LEFT);
}

/**
 * Check that a turn signal from the LCDA persistent data is no longer held if holding counter is above threshold.
 * \uts{CSCSA-42882} \sdd{SF-6546} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Process_Veh_Turn_Signal__dont_hold_turn_signal_from_persistent_data_if_above_threshold)
{
   /** \arrange Set up vehicle data such that there is no signal left. Set up persistent data such that there is a held turn signal
    * above threshold of holding counter. */
   p_vehicle_data->turn_signal               = TURN_SIGNAL_NONE;
   lcda_persistent.turn_signal_held          = TURN_SIGNAL_LEFT;
   lcda_cals.k_lcda_turn_signal_coast_cycles = 3u;
   lcda_persistent.turn_signal_held_counter  = lcda_cals.k_lcda_turn_signal_coast_cycles + 1u;

   /** \action Call Lcda_Process_Veh_Turn_Signal to process the turn signal from the vehicle data. */
   Lcda_Process_Veh_Turn_Signal(p_vehicle_data, &lcda_cals, &lcda_persistent);

   /** \assert Verify that turn signal from persistent data is no longer held. */
   EXPECT_EQ(lcda_persistent.turn_signal_held, TURN_SIGNAL_NONE);
}


/**
 * Check that LCDA is not disabled by host speed, if the host speed is in the allowed range when the low speed hysteresis is
 * applied. \uts{CSCSA-42883} \sdd{SF-6541} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Host_Speed_In_Activation_Range__returns_true_if_host_speed_in_allowed_range_if_low_speed_hysteresis_is_applied)
{
   /** \arrange Set up vehicle data such that host speed is in allowed range when the low speed hysteresis is applied. */
   boolean_T result           = FBK_FALSE;
   p_vehicle_data->host_speed = lcda_cals.k_lcda_host_activation_speed_min - lcda_cals.k_lcda_host_activation_speed_min_hys + EPSILON;
   lcda_persistent.f_host_speed_in_activation_range = FBK_TRUE;

   /** \action Call Lcda_Is_Host_Above_Activation_Speed to determine if host speed is above LCDA activation speed. */
   result = Lcda_Is_Host_Speed_In_Activation_Range(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that true is returned by function. */
   EXPECT_TRUE(result);
}

/**
 * Check that LCDA is not disabled by host speed, if the host speed is in the allowed range when the high speed hysteresis is
 * applied. \uts{CSCSA-42884} \sdd{SF-6541} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test,
       Lcda_Is_Host_Speed_In_Activation_Range__returns_true_if_host_speed_in_allowed_range_if_high_speed_hysteresis_is_applied)
{
   /** \arrange Set up vehicle data such that host speed is in allowed range when the high speed hysteresis is applied. */
   boolean_T result           = FBK_FALSE;
   p_vehicle_data->host_speed = lcda_cals.k_lcda_host_activation_speed_max + lcda_cals.k_lcda_host_activation_speed_max_hys - EPSILON;
   lcda_persistent.f_host_speed_in_activation_range = FBK_TRUE;

   /** \action Call Lcda_Is_Host_Above_Activation_Speed to determine if host speed is above LCDA activation speed. */
   result = Lcda_Is_Host_Speed_In_Activation_Range(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that true is returned by function. */
   EXPECT_TRUE(result);
}

/**
 * Check that LCDA is disabled by host speed, if the host speed is too high. applied.
 * \uts{CSCSA-42885} \sdd{SF-6541} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Host_Speed_In_Activation_Range__returns_false_if_host_speed_is_too_high)
{
   /** \arrange Set up vehicle data such that host speed is too high. */
   boolean_T result                                 = FBK_FALSE;
   p_vehicle_data->host_speed                       = lcda_cals.k_lcda_host_activation_speed_max + EPSILON;
   lcda_persistent.f_host_speed_in_activation_range = FBK_FALSE;

   /** \action Call Lcda_Is_Host_Above_Activation_Speed to determine if host speed is above LCDA activation speed. */
   result = Lcda_Is_Host_Speed_In_Activation_Range(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that false is returned by function. */
   EXPECT_FALSE(result);
}

/**
 * Check that LCDA is disabled by host speed, if the host speed is too low. applied.
 * \uts{CSCSA-42886} \sdd{SF-6541} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Is_Host_Speed_In_Activation_Range__returns_false_if_host_speed_is_too_low)
{
   /** \arrange Set up vehicle data such that host speed is too low. */
   boolean_T result                                 = FBK_FALSE;
   p_vehicle_data->host_speed                       = lcda_cals.k_lcda_host_activation_speed_min - EPSILON;
   lcda_persistent.f_host_speed_in_activation_range = FBK_FALSE;

   /** \action Call Lcda_Is_Host_Above_Activation_Speed to determine if host speed is above LCDA activation speed. */
   result = Lcda_Is_Host_Speed_In_Activation_Range(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that false is returned by function. */
   EXPECT_FALSE(result);
}

/**
 * Check that function to update vehicle state updates all vehicle related persistent data.
 * \uts{CSCSA-42887} \sdd{SF-6547} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Update_Vehicle_States__updates_all_vehicle_related_persistent_data)
{
   /** \arrange Set up vehicle data such that resulting persistent data will differ from initial values. */
   p_vehicle_data->host_speed  = lcda_cals.k_lcda_host_activation_speed_min + EPSILON;
   p_vehicle_data->turn_signal = TURN_SIGNAL_LEFT;

   lcda_persistent.f_curve_radius_valid             = FBK_TRUE;
   lcda_persistent.f_host_speed_in_activation_range = FBK_FALSE;
   lcda_persistent.turn_signal_held                 = TURN_SIGNAL_NONE;

   /** \action Call Lcda_Update_Vehicle_States to update all vehicle releated LCDA states. */
   Lcda_Update_Vehicle_States(p_vehicle_data, &lcda_cals, &lcda_persistent);

   /** \assert Verify that all vehicle related persistent data is updated. */
   EXPECT_TRUE(lcda_persistent.f_curve_radius_valid);
   EXPECT_TRUE(lcda_persistent.f_host_speed_in_activation_range);
   EXPECT_EQ(lcda_persistent.turn_signal_held, TURN_SIGNAL_LEFT);
}

/**
 * Check that the initialization of the LCDA core input works properly.
 * \uts{CSCSA-42888} \sdd{SF-6536} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Init_Core_Input__works_properly)
{
   /** \arrange Set up LCDA core input with non-default values. */
   lcda_core_input.p_pa_data                        = NULL;
   lcda_core_input.initial_bsw_zone.size            = 0u;
   lcda_core_input.initial_cvw_zone_hys.size        = 0u;
   lcda_core_input.initial_cvw_zone.points[4u].x    = 2.0f;
   lcda_core_input.warn_settings.slc_ttc_thres_lon  = 1.2f;
   lcda_core_input.enabled_flags.f_elc_enabled      = FBK_TRUE;
   lcda_core_input.enabled_flags.f_dropback_enabled = FBK_TRUE;

   /** \action Call Lcda_Init_Core_Input to initialize the LCDA core input. */
   Lcda_Init_Core_Input(&lcda_core_input, &data);

   /** \assert Verify that all LCDA core input values are set to default values. */
   EXPECT_FALSE(lcda_core_input.enabled_flags.f_elc_enabled);
   EXPECT_FALSE(lcda_core_input.enabled_flags.f_dropback_enabled);
   EXPECT_FLOAT_EQ(lcda_core_input.warn_settings.slc_ttc_thres_lon, FBK_ZERO_F);
   EXPECT_FLOAT_EQ(lcda_core_input.initial_cvw_zone.points[4u].x, FBK_ZERO_F);
   EXPECT_EQ(lcda_core_input.initial_cvw_zone_hys.size, LCDA_NUMBER_OF_ZONE_POINTS);
   EXPECT_EQ(lcda_core_input.initial_bsw_zone.size, LCDA_NUMBER_OF_ZONE_POINTS);
   EXPECT_EQ(lcda_core_input.p_pa_data, &data);
}

/**
 * Check that the no critical object is reported for any submodule, if no valid objects are present for LCDA.
 * \uts{CSCSA-42889} \sdd{SF-6545} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Process_All_Active_Submodules__does_not_alert_any_object_if_no_valid_object_is_present)
{
   /** \arrange Set LCDA enabled flags to true for all submodules in core output. Do not set up any valid LCDA object. */
   lcda_core_output.bsw_core_output.f_bsw_is_enabled = 1u;
   lcda_core_output.cvw_core_output.f_cvw_is_enabled = 1u;
   lcda_core_output.elc_core_output.f_elc_is_enabled = 1u;
   lcda_core_output.slc_core_output.f_slc_is_enabled = 1u;

   /** \action Call Lcda_Process_All_Active_Submodules to process all active submodules. */
   Lcda_Process_All_Active_Submodules(&lcda_instance, &fbk_output);

   /** \assert Verify that no alert is active for any submodule. */
   EXPECT_EQ(lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_LEFT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(lcda_core_output.slc_core_output.slc_alert[FBK_SIDE_LEFT], LCDA_ALERT_STATE_NONE);

   EXPECT_EQ(lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(lcda_core_output.slc_core_output.slc_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_NONE);
}

/**
 * Check that the no critical object is reported for any submodule, if a valid objects is present for LCDA but all submodules are
 * disabled. \uts{CSCSA-42890} \sdd{SF-6545} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Process_All_Active_Submodules__does_not_alert_any_object_if_all_submodules_are_disabled)
{
   /** \arrange Set LCDA enabled flags to false for all submodules in core output. Set up a valid LCDA object. */
   lcda_core_output.bsw_core_output.f_bsw_is_enabled = 0u;
   lcda_core_output.cvw_core_output.f_cvw_is_enabled = 0u;
   lcda_core_output.elc_core_output.f_elc_is_enabled = 0u;
   lcda_core_output.slc_core_output.f_slc_is_enabled = 0u;

   uint8_t obj_idx                            = 4u;
   object_data[obj_idx].existence_probability = 1.0f;
   object_data[obj_idx].age                   = lcda_cals.k_lcda_min_track_age + 1u;
   object_data[obj_idx].vcs_pos.x             = -lcda_cals.k_lcda_max_range * 0.5f;
   object_data[obj_idx].status                = PA_OBJ_STATUS_MATURE;
   object_data[obj_idx].f_moveable            = FBK_TRUE;

   /** \action Call Lcda_Process_All_Active_Submodules to process all active submodules. */
   Lcda_Process_All_Active_Submodules(&lcda_instance, &fbk_output);

   /** \assert Verify that no alert is active for any submodule. */
   EXPECT_EQ(lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_LEFT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(lcda_core_output.slc_core_output.slc_alert[FBK_SIDE_LEFT], LCDA_ALERT_STATE_NONE);

   EXPECT_EQ(lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(lcda_core_output.slc_core_output.slc_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_NONE);
}

/**
 * Check that the no critical object is reported for any submodule, if a valid (but not warning relevant) objects is present for
 * LCDA and all submodules are enabled. \uts{CSCSA-42891} \sdd{SF-6545} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test,
       Lcda_Process_All_Active_Submodules__does_not_alert_any_object_if_all_submodules_are_enabled_but_object_in_not_warning_relevant)
{
   /** \arrange Set LCDA enabled flags to false for all submodules in core output. Set up a valid LCDA object (not warning
    * relevant). Set alerts for all submodules. */
   lcda_core_output.bsw_core_output.f_bsw_is_enabled = FBK_TRUE;
   lcda_core_output.cvw_core_output.f_cvw_is_enabled = FBK_TRUE;
   lcda_core_output.elc_core_output.f_elc_is_enabled = FBK_TRUE;
   lcda_core_output.slc_core_output.f_slc_is_enabled = FBK_TRUE;

   uint8_t obj_idx                 = 0u;
   object_data[obj_idx].age        = lcda_cals.k_lcda_min_track_age + 1u;
   object_data[obj_idx].vcs_pos.x  = -lcda_cals.k_lcda_max_range * 0.5f;
   object_data[obj_idx].status     = PA_OBJ_STATUS_MATURE;
   object_data[obj_idx].f_moveable = FBK_TRUE;

   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT] = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT] = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_LEFT] = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.slc_core_output.slc_alert[FBK_SIDE_LEFT] = LCDA_ALERT_STATE_LEVEL_1;

   lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_1;
   lcda_core_output.slc_core_output.slc_alert[FBK_SIDE_RIGHT] = LCDA_ALERT_STATE_LEVEL_1;

   /** \action Call Lcda_Process_All_Active_Submodules to process all active submodules. */
   Lcda_Process_All_Active_Submodules(&lcda_instance, &fbk_output);

   /** \assert Verify that no alert is active for any submodule. */
   EXPECT_EQ(lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_LEFT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_LEFT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_LEFT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(lcda_core_output.slc_core_output.slc_alert[FBK_SIDE_LEFT], LCDA_ALERT_STATE_NONE);

   EXPECT_EQ(lcda_core_output.bsw_core_output.bsw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(lcda_core_output.cvw_core_output.cvw_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(lcda_core_output.elc_core_output.elc_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_NONE);
   EXPECT_EQ(lcda_core_output.slc_core_output.slc_alert[FBK_SIDE_RIGHT], LCDA_ALERT_STATE_NONE);
}

/**
 * Check that bsw and cvw core is not reset while persistent data is reset.
 * \uts{CSCSA-42892} \sdd{SF-6552} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Core_Run__bsw_cvw_not_reset_persistent_reset)
{
   /** \arrange Bsw core was reset in previus run */

   lcda_persistent.f_bsw_prev_reset                  = FBK_TRUE;
   lcda_persistent.f_cvw_prev_reset                  = FBK_TRUE;
   lcda_core_output.bsw_core_output.f_bsw_is_enabled = FBK_TRUE;
   lcda_core_output.cvw_core_output.f_cvw_is_enabled = FBK_TRUE;
   lcda_core_input.enabled_flags.f_lcda_enabled      = FBK_TRUE;

   /** \action Call Lcda_Core_Run to execute the LCDA core. */
   Lcda_Core_Run(&lcda_instance, &fbk_output);

   /** \assert Verify that bsw core is reset. */
   EXPECT_TRUE(lcda_core_output.cvw_core_output.f_cvw_is_enabled);
   EXPECT_TRUE(lcda_core_output.cvw_core_output.f_cvw_is_enabled);
}


/**
 * Check that Lcda_Core_Cal_In_Boundary is coreclty working for k_elc_max_curvi_heading_abs In boundry values applied.
 * \uts{CSCSA-68151} \sdd{CSCSA-186587} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Core_Cal_In_Boundary__check_k_elc_max_curvi_heading_abs_border_case)
{
   /** \arrange */
   EXPECT_TRUE(Lcda_Core_Cal_In_Boundary(&lcda_cals));

   /** \action Set k_elc_max_curvi_heading_abs to border values */
   lcda_cals.k_elc_max_curvi_heading_abs = LCDA_MIN_K_ELC_MAX_CURVI_HEADING_ABS;
   EXPECT_TRUE(Lcda_Core_Cal_In_Boundary(&lcda_cals));

   lcda_cals.k_elc_max_curvi_heading_abs = LCDA_MAX_K_ELC_MAX_CURVI_HEADING_ABS;
   /** \assert Verify that lcda_cals is in boundary if equel maximum */
   EXPECT_TRUE(Lcda_Core_Cal_In_Boundary(&lcda_cals));
}

/**
 * Check that Lcda_Core_Cal_In_Boundary is coreclty working for k_elc_max_curvi_heading_abs Out of bounary values applied.
 * \uts{CSCSA-68152} \sdd{CSCSA-186587} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Core_Cal_In_Boundary__check_k_elc_max_curvi_heading_abs_out_of_border)
{
   /** \arrange */
   /** \action Set k_elc_max_curvi_heading_abs out of boundary values */
   lcda_cals.k_elc_max_curvi_heading_abs = LCDA_MIN_K_ELC_MAX_CURVI_HEADING_ABS - 1;
   EXPECT_FALSE(Lcda_Core_Cal_In_Boundary(&lcda_cals));

   lcda_cals.k_elc_max_curvi_heading_abs = LCDA_MAX_K_ELC_MAX_CURVI_HEADING_ABS + 1;
   /** \assert Verify that lcda_cals is in boundary if equel maximum */
   EXPECT_FALSE(Lcda_Core_Cal_In_Boundary(&lcda_cals));
}

/**
 * Check that LCDA status is active, if ego speed and curve radius are within defined boundaries.
 * \uts{CSCSA-211419} \sdd{SF-6534} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Test, Lcda_Get_Activation_Status__returns_active_lcda_status_if_ego_speed_and_curve_radius_are_valid_case2)
{
   /** \arrange Set up persistent data flags for ego speed and curve radius. */
   Lcda_Status_T lcda_status = LCDA_STATUS_DEACTIVATED_INTERNAL_ERROR;

   lcda_persistent.f_host_speed_in_activation_range     = FBK_TRUE;
   lcda_persistent.f_curve_radius_valid                 = FBK_TRUE;
   lcda_cals.k_lcda_f_disable_due_to_small_curve_radius = FBK_TRUE;

   /** \action Call Lcda_Get_Activation_Status to get Lcda activation status. */
   lcda_status = Lcda_Get_Activation_Status(&lcda_cals, p_vehicle_data, &lcda_persistent);

   /** \assert Verify that LCDA status is active. */
   EXPECT_EQ(lcda_status, LCDA_STATUS_ACTIVE);
}