/**
 * @file esa_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for esa.c functions
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-123900}
 */

#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "esa.c"
#include "esa_core_calibration_t.h"
#include "esa_core_input_t.h"
#include "esa_core_output_t.h"
#include "esa_persistent_t.h"
#include "esa_test.hpp"
#include "esa_types.h"
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "fbk_vehicle_data_t.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "pa_shared_types.h"
}

#ifndef NDEBUG
/**
 * Check that an exception is thrown if function to reset ESA is called with null pointer for core output.
 * \uts{CSCSA-123901} \sdd{CSCSA-65958} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Reset__NULL_esa_core_output_pointer_throws_assertion)
{
   /** \arrange */
   /** \action See assert. */
   /** \assert Verify that an assert is thrown, when Esa_Reset function is called with null pointer for core output. */
   EXPECT_DEATH({ Esa_Reset(p_esa_core_input, NULL, p_esa_persistent); }, ".*p_esa_core_output.*");
}

/**
 * Check that an exception is thrown if function to reset ESA is called with null pointer for persistents.
 * \uts{CSCSA-185755} \sdd{CSCSA-65958} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Reset__NULL_esa_persistent_pointer_throws_assertion)
{
   /** \arrange */
   /** \action See assert. */
   /** \assert Verify that an assert is thrown, when Esa_Reset function is called with null pointer for persistents. */
   EXPECT_DEATH({ Esa_Reset(p_esa_core_input, p_esa_core_output, NULL); }, ".*p_esa_persistent.*");
}
#endif //! NDEBUG

/**
 * Check that ESA is not active if it is disabled by the corresponding calibration flag.
 * \uts{CSCSA-123903} \sdd{CSCSA-65956} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Core_Run__core_output_set_to_default_values_when_esa_disabled_by_calibration)
{
   /** \arrange Set up calibration values and core input such that ESA is disabled by calibrations. */
   p_esa_calibration->k_esa_f_enable_via_cal = 1u;
   p_esa_calibration->k_esa_f_enable         = 0u;
   p_esa_core_input->f_esa_enabled           = FBK_FALSE;

   /** \action Call Esa_Core_Run to execute the ESA core. */
   Esa_Core_Run(p_esa_core_output, p_esa_core_input, p_esa_persistent, p_esa_calibration);

   /** \assert Verify that ESA is not active because it was disabled by cals. */
   EXPECT_EQ(p_esa_core_output->esa_core_status, ESA_CORE_STATUS_DISABLED_BY_CAL);
}

/**
 * Check that ESA is not active if it is disabled by the corresponding core input flag.
 * \uts{CSCSA-123904} \sdd{CSCSA-65956} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Core_Run__core_output_set_to_default_values_when_esa_disabled_by_input_flag)
{
   /** \arrange Set up calibration values and core input such that ESA is disabled by core input. */
   p_esa_calibration->k_esa_f_enable_via_cal = 0u;
   p_esa_calibration->k_esa_f_enable         = 1u;
   p_esa_core_input->f_esa_enabled           = FBK_FALSE;

   /** \action Call Esa_Core_Run to execute the ESA core. */
   Esa_Core_Run(p_esa_core_output, p_esa_core_input, p_esa_persistent, p_esa_calibration);

   /** \assert Verify that ESA is not active because it was disabled by core input. */
   EXPECT_EQ(p_esa_core_output->esa_core_status, ESA_CORE_STATUS_DISABLED_BY_INPUT);
}

/**
 * Check that ESA is not active if ego speed is too low for ESA to be active.
 * \uts{CSCSA-123905} \sdd{CSCSA-65956} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Core_Run__core_output_set_to_default_values_when_host_below_activation_speed)
{
   /** \arrange Set up calibration values and vehicle data such that ego speed is too low for ESA to be active. */
   p_esa_calibration->k_esa_host_activation_speed_min = 20.0f;

   p_vehicle_data->host_speed = 3.0f;

   p_esa_core_input->f_esa_enabled = FBK_TRUE;

   /** \action Call Esa_Core_Run to execute the ESA core. */
   Esa_Core_Run(p_esa_core_output, p_esa_core_input, p_esa_persistent, p_esa_calibration);

   /** \assert Verify that ESA is not active due to ego speed. */
   EXPECT_EQ(p_esa_core_output->esa_core_status, ESA_CORE_STATUS_DEACTIVATED_LOW_EGO_SPEED);
}

/**
 * Check that ESA is not active if ego curve radius is too low for ESA to be active.
 * \uts{CSCSA-123906} \sdd{CSCSA-65956} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Core_Run__core_output_set_to_default_values_low_curve_radius)
{
   /** \arrange Set up calibration values and vehicle data such that ego curve radius is too low for ESA to be active. */
   p_esa_calibration->k_esa_f_allow_min_curve_radius = 1u;
   p_esa_calibration->k_esa_min_curve_radius         = 2.0f;
   p_esa_calibration->k_esa_min_curve_radius_hys     = 0.2f;

   p_vehicle_data->curvature = 3.0f;

   p_esa_core_input->f_esa_enabled = FBK_TRUE;

   /** \action Call Esa_Core_Run to execute the ESA core. */
   Esa_Core_Run(p_esa_core_output, p_esa_core_input, p_esa_persistent, p_esa_calibration);

   /** \assert Verify that ESA is not active due to low curve radius. */
   EXPECT_EQ(p_esa_core_output->esa_core_status, ESA_CORE_STATUS_DEACTIVATED_LOW_CURVE_RADIUS);
}


/**
 * Check that ESA is enabled.
 * \uts{CSCSA-123907} \sdd{CSCSA-65956} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Core_Run__esa_is_enabled)
{
   /** \arrange Set up calibration values and vehicle data such that ego curve radius is too low for ESA to be active. */
   p_esa_calibration->k_esa_f_enable_via_cal          = 1u;
   p_esa_calibration->k_esa_f_enable                  = 1u;
   p_esa_calibration->k_esa_host_activation_speed_min = 20.0f;
   p_esa_calibration->k_esa_f_allow_min_curve_radius  = 1u;
   p_esa_calibration->k_esa_min_curve_radius          = 2.0f;
   p_esa_calibration->k_esa_min_curve_radius_hys      = 0.2f;

   p_esa_core_input->f_esa_enabled = FBK_TRUE;
   p_vehicle_data->host_speed      = 30.0f;
   p_vehicle_data->curvature       = 0.0f;

   /** \action Call Esa_Core_Run to execute the ESA core. */
   Esa_Core_Run(p_esa_core_output, p_esa_core_input, p_esa_persistent, p_esa_calibration);

   /** \assert Verify that ESA is not active due to low curve radius and core output is default. */
   EXPECT_EQ(p_esa_core_output->esa_core_status, ESA_CORE_STATUS_ENABLED);
}


/**
 * Check that an object is classified as valid for ESA if all conditions are fulfilled.
 * \uts{CSCSA-123908} \sdd{CSCSA-65978} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Valid_Object__returns_TRUE_for_valid_object)
{
   /** \arrange Set up calibrations and tracker output such that object is valid for ESA. */
   boolean_T result = FBK_FALSE;
   Fbk_Object_Data_T tracker_object;

   p_esa_calibration->k_esa_min_exist_prob = 0.8f;
   p_esa_calibration->k_esa_min_track_age  = 5u;
   p_esa_calibration->k_esa_max_range      = 50.0f;

   tracker_object.existence_probability = 1.0f;
   tracker_object.age                   = p_esa_calibration->k_esa_min_track_age + 1u;
   tracker_object.vcs_pos.x             = 3.0f;
   tracker_object.status                = PA_OBJ_STATUS_MATURE;

   /** \action Call Esa_Is_Valid_Object to evaluate if object is valid for ESA. */
   result = Esa_Is_Valid_Object(&tracker_object, p_esa_calibration);

   /** \assert Verify that object is classified as valid. */
   EXPECT_TRUE(result);
}


/**
 * Check that an object is classified as invalid for ESA if the object below the threshold for the minimal object age.
 * \uts{CSCSA-123909} \sdd{CSCSA-65978} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Valid_Object__returns_FALSE_if_obj_is_too_young)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid due to object age being too small. */
   boolean_T result = FBK_TRUE;
   Fbk_Object_Data_T tracker_object;

   p_esa_calibration->k_esa_min_exist_prob = 0.8f;
   p_esa_calibration->k_esa_min_track_age  = 5u;
   p_esa_calibration->k_esa_max_range      = 50.0f;

   tracker_object.existence_probability = 1.0f;
   tracker_object.age                   = p_esa_calibration->k_esa_min_track_age - 1u;
   tracker_object.vcs_pos.x             = 3.0f;
   tracker_object.status                = PA_OBJ_STATUS_MATURE;

   /** \action Call Esa_Is_Valid_Object to evaluate if object is valid for ESA. */
   result = Esa_Is_Valid_Object(&tracker_object, p_esa_calibration);

   /** \assert Verify that object is classified as not valid. */
   EXPECT_FALSE(result);
}


/**
 * Check that an object is classified as invalid for ESA if the object is out of range for ESA.
 * \uts{CSCSA-123910} \sdd{CSCSA-65978} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Valid_Object__returns_FALSE_if_obj_out_of_esa_range)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid due to being out of range for ESA. */
   boolean_T result = FBK_TRUE;
   Fbk_Object_Data_T tracker_object;

   p_esa_calibration->k_esa_min_exist_prob = 0.8f;
   p_esa_calibration->k_esa_min_track_age  = 5u;
   p_esa_calibration->k_esa_max_range      = 50.0f;

   tracker_object.existence_probability = 1.0f;
   tracker_object.age                   = p_esa_calibration->k_esa_min_track_age + 1u;
   tracker_object.vcs_pos.x             = -55.0f;
   tracker_object.status                = PA_OBJ_STATUS_MATURE;

   /** \action Call Esa_Is_Valid_Object to evaluate if object is valid for ESA. */
   result = Esa_Is_Valid_Object(&tracker_object, p_esa_calibration);

   /** \assert Verify that object is classified as not valid. */
   EXPECT_FALSE(result);
}


/**
 * Check that an object is classified as invalid for ESA if the object status is invalid.
 * \uts{CSCSA-123911} \sdd{CSCSA-65978} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Valid_Object__returns_FALSE_if_obj_status_is_invalid)
{
   /** \arrange Set up calibrations and tracker output such that object is invalid due to the object status being invalid. */
   boolean_T result = FBK_TRUE;
   Fbk_Object_Data_T tracker_object;

   p_esa_calibration->k_esa_min_exist_prob = 0.8f;
   p_esa_calibration->k_esa_min_track_age  = 5u;
   p_esa_calibration->k_esa_max_range      = 50.0f;

   tracker_object.existence_probability = 1.0f;
   tracker_object.age                   = p_esa_calibration->k_esa_min_track_age + 1u;
   tracker_object.vcs_pos.x             = 3.0f;
   tracker_object.status                = PA_OBJ_STATUS_INVALID;

   /** \action Call Esa_Is_Valid_Object to evaluate if object is valid for ESA. */
   result = Esa_Is_Valid_Object(&tracker_object, p_esa_calibration);

   /** \assert Verify that object is classified as not valid. */
   EXPECT_FALSE(result);
}


/**
 * Check that ESA status is active, if ego speed and curve radius are within defined boundaries.
 * \uts{CSCSA-123912} \sdd{CSCSA-65991} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Get_Activation_Status__returns_active_esa_status_if_ego_speed_and_curve_radius_are_okay)
{
   /** \arrange Set up persistent data flags for ego speed and curve radius. */
   Esa_Core_Status_T esa_status = ESA_CORE_STATUS_DEACTIVATED_INTERNAL_ERROR;

   p_esa_persistent->f_host_speed_in_activation_range = FBK_TRUE;
   p_esa_persistent->f_esa_disabled_low_curve_radius  = FBK_FALSE;

   /** \action Call Esa_Get_Activation_Status to get Esa activation status. */
   esa_status = Esa_Get_Activation_Status(p_esa_calibration, p_vehicle_data, p_esa_persistent);

   /** \assert Verify that ESA status is active. */
   EXPECT_EQ(esa_status, ESA_CORE_STATUS_ACTIVE);
}


/**
 * Check that the most critical object is set when the relevant object is in zone.
 * \uts{CSCSA-123913} \sdd{CSCSA-65968} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Process_Object__sets_most_critical_object_when_relevant_object_is_in_zone)
{
   /** \arrange Set up persistent data and tracker object data. */
   Fbk_Object_Data_T tracker_object;

   p_esa_core_input->lane_width         = p_esa_calibration->k_esa_min_lane_width;
   tracker_object.id                    = 11u;
   tracker_object.status                = PA_OBJ_STATUS_MATURE;
   tracker_object.curvi_heading         = 0.0f;
   tracker_object.curvi_vel.x           = 50.0f;
   tracker_object.existence_probability = 1.0f;
   tracker_object.curvi_pos.x           = p_esa_calibration->k_esa_zone_x[1];
   tracker_object.curvi_pos.y =
      0.5f * p_esa_core_input->lane_width * (p_esa_calibration->k_esa_zone_y[2] + p_esa_calibration->k_esa_zone_y[3]);
   tracker_object.length                                              = 4.0f;
   tracker_object.width                                               = 2.0f;
   p_vehicle_data->host_speed                                         = 20.0f;
   p_vehicle_data->long_vel                                           = 20.0f;
   tracker_object.curvi_vel_rel.x                                     = tracker_object.curvi_vel.x - p_vehicle_data->long_vel;
   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_TRUE;
   p_esa_persistent->mature_count_in_esa_zone[tracker_object.id - FBK_ONE_UINT] = p_esa_calibration->k_esa_min_mature_cycles;

   /** \action Call Esa_Process_Object to get Esa activation status. */
   Esa_Process_Object(p_esa_core_output, p_esa_persistent, &tracker_object, p_esa_core_input, p_vehicle_data, p_esa_calibration);

   /** \assert Verify that ESA status is active. */
   EXPECT_TRUE(p_esa_core_output->esa_alert[FBK_SIDE_RIGHT]);
   EXPECT_EQ(p_esa_core_output->esa_id[FBK_SIDE_RIGHT], tracker_object.id);
}


/**
 * Check that the most critical object is not set when the relevant object with zero relative velocity is in zone.
 * \uts{CSCSA-209333} \sdd{CSCSA-65968} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Process_Object__most_critical_object_not_set_if_zero_relative_velocity_object_is_in_zone)
{
   /** \arrange Set up persistent data and tracker object data. */
   Fbk_Object_Data_T tracker_object;

   p_esa_core_input->lane_width         = p_esa_calibration->k_esa_min_lane_width;
   tracker_object.id                    = 11u;
   tracker_object.status                = PA_OBJ_STATUS_MATURE;
   tracker_object.curvi_heading         = 0.0f;
   tracker_object.curvi_vel.x           = 20.0f;
   tracker_object.existence_probability = 1.0f;
   tracker_object.curvi_pos.x           = p_esa_calibration->k_esa_zone_x[1];
   tracker_object.curvi_pos.y =
      0.5f * p_esa_core_input->lane_width * (p_esa_calibration->k_esa_zone_y[2] + p_esa_calibration->k_esa_zone_y[3]);
   tracker_object.length                                                        = 4.0f;
   tracker_object.width                                                         = 2.0f;
   p_vehicle_data->host_speed                                                   = 20.0f;
   p_vehicle_data->long_vel                                                     = 20.0f;
   tracker_object.curvi_vel_rel.x                                               = 0.0f;
   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration           = FBK_TRUE;
   p_esa_persistent->mature_count_in_esa_zone[tracker_object.id - FBK_ONE_UINT] = p_esa_calibration->k_esa_min_mature_cycles;

   /** \action Call Esa_Process_Object to get Esa activation status. */
   Esa_Process_Object(p_esa_core_output, p_esa_persistent, &tracker_object, p_esa_core_input, p_vehicle_data, p_esa_calibration);

   /** \assert Verify that ESA status is not active. */
   EXPECT_FALSE(p_esa_core_output->esa_alert[FBK_SIDE_RIGHT]);
   EXPECT_EQ(p_esa_core_output->esa_id[FBK_SIDE_RIGHT], PA_INVALID_OBJ_ID);
}


/**
 * Check that the most critical object is not set when no relevant object is in zone.
 * \uts{CSCSA-123914} \sdd{CSCSA-65968} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Process_Object__does_not_set_most_critical_object_when_relevant_object_is_not_in_zone)
{
   /** \arrange Set up persistent data and tracker object data. */
   Fbk_Object_Data_T tracker_object;

   p_esa_core_input->lane_width         = p_esa_calibration->k_esa_min_lane_width;
   tracker_object.id                    = 11u;
   tracker_object.status                = PA_OBJ_STATUS_MATURE;
   tracker_object.length                = 4.0f;
   tracker_object.width                 = 2.0f;
   p_vehicle_data->host_speed           = 20.0f;
   p_vehicle_data->long_vel             = 20.0f;
   tracker_object.curvi_heading         = 0.0f;
   tracker_object.curvi_vel.x           = 50.0f;
   tracker_object.existence_probability = 1.0f;
   tracker_object.curvi_pos.x           = p_esa_calibration->k_esa_zone_x[1];
   tracker_object.curvi_pos.y = p_esa_core_input->lane_width * p_esa_calibration->k_esa_zone_y[2] + 0.5f * tracker_object.width + 1.0f;
   tracker_object.curvi_vel_rel.x                                     = tracker_object.curvi_vel.x - p_vehicle_data->long_vel;
   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_TRUE;
   p_esa_persistent->mature_count_in_esa_zone[tracker_object.id - FBK_ONE_UINT] = p_esa_calibration->k_esa_min_mature_cycles;

   /** \action Call Esa_Process_Object to get Esa activation status. */
   Esa_Process_Object(p_esa_core_output, p_esa_persistent, &tracker_object, p_esa_core_input, p_vehicle_data, p_esa_calibration);

   /** \assert Verify that ESA status is active. */
   EXPECT_FALSE(p_esa_core_output->esa_alert[FBK_SIDE_RIGHT]);
   EXPECT_EQ(p_esa_core_output->esa_id[FBK_SIDE_RIGHT], 0u);
}


/**
 * Check that ESA status is enabled, if this is decided based on calibration values and those are set accordingly.
 * \uts{CSCSA-123915} \sdd{CSCSA-65990} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Get_Enable_Status__returns_enabled_esa_status_if_enabled_via_cals)
{
   /** \arrange Set up calibration values such that ESA is enabled by calibration */
   Esa_Core_Status_T esa_status = ESA_CORE_STATUS_DEACTIVATED_INTERNAL_ERROR;

   p_esa_calibration->k_esa_f_enable_via_cal = FBK_TRUE;
   p_esa_calibration->k_esa_f_enable         = FBK_TRUE;
   p_esa_core_input->f_esa_enabled           = FBK_FALSE;

   /** \action Call Esa_Get_Enable_Status to get Esa enabled status. */
   esa_status = Esa_Get_Enable_Status(p_esa_calibration, p_esa_core_input);

   /** \assert Verify that ESA status is enabled. */
   EXPECT_EQ(esa_status, ESA_CORE_STATUS_ENABLED);
}


/**
 * Check that ESA is not disabled by small curve radius, if the curve radius is in the allowed range when the hysteresis is
 * applied. \uts{CSCSA-123916} \sdd{CSCSA-65992} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test,
       Esa_Is_Disabled_By_Small_Curve_Radius__returns_true_if_curve_radius_is_in_allowed_range_if_hysteresis_is_applied_positive_radius)
{
   /** \arrange Set up calibration values, vehicle data and persistent data such that curve radius is in allowed range when
    * hysteresis is applied. */
   boolean_T result = FBK_TRUE;

   p_esa_calibration->k_esa_min_curve_radius         = 4.7f;
   p_esa_calibration->k_esa_min_curve_radius_hys     = 0.2f;
   p_esa_calibration->k_esa_f_allow_min_curve_radius = 1u;
   p_vehicle_data->curvature                         = 0.2f; // curve radius = (1.0/0.2)=5.0
   p_esa_persistent->f_esa_disabled_low_curve_radius = FBK_TRUE;

   /** \action Call Esa_Is_Feature_Disabled_By_Small_Curve_Radius to determine if ESA is disabled by small curve radius. */
   result = Esa_Is_Disabled_By_Small_Curve_Radius(p_esa_calibration, p_vehicle_data, p_esa_persistent);

   /** \assert Verify that ESA is not disabled by small curve radius. */
   EXPECT_FALSE(result);
}


/**
 * Check that ESA is not disabled by small curve radius, if the curve radius is in the allowed range when the hysteresis is
 * applied. Negative values for branch coverage applied. \uts{CSCSA-123917} \sdd{CSCSA-65992} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Curvi_Radius_Valid__returns_true_if_curve_radius_is_in_allowed_range_if_hysteresis_is_applied_negative_radius)
{
   /** \arrange Set up calibration values, vehicle data and persistent data such that curve radius is in allowed range when
    * hysteresis is applied. */
   boolean_T result = FBK_TRUE;

   p_esa_calibration->k_esa_min_curve_radius         = 2.0f;
   p_esa_calibration->k_esa_min_curve_radius_hys     = 0.2f;
   p_esa_calibration->k_esa_f_allow_min_curve_radius = 1u;
   p_vehicle_data->curvature                         = -2.0f; // current_radius = 0.5
   p_esa_persistent->f_esa_disabled_low_curve_radius = FBK_TRUE;

   /** \action Call Esa_Is_Feature_Disabled_By_Small_Curve_Radius to determine if ESA is disabled by small curve radius. */
   result = Esa_Is_Disabled_By_Small_Curve_Radius(p_esa_calibration, p_vehicle_data, p_esa_persistent);

   /** \assert Verify that ESA is not disabled by small curve radius. */
   EXPECT_TRUE(result);
}


/**
 * Check that ESA is disabled by zero curve radius, if the curve radius is in the allowed range when the hysteresis is applied.
 * \uts{CSCSA-123918} \sdd{CSCSA-65992} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Curvi_Radius_Valid__returns_true_if_curve_radius_is_in_allowed_range_if_hysteresis_is_applied_zero_radius)
{
   /** \arrange Set up calibration values, vehicle data and persistent data such that curve radius is zero when hysteresis is
    * applied. */
   boolean_T result = FBK_TRUE;

   p_esa_calibration->k_esa_min_curve_radius         = 2.0f;
   p_esa_calibration->k_esa_min_curve_radius_hys     = 0.2f;
   p_esa_calibration->k_esa_f_allow_min_curve_radius = 1u;
   p_vehicle_data->curvature                         = 0.0f;
   p_esa_persistent->f_esa_disabled_low_curve_radius = FBK_TRUE;

   /** \action Call Esa_Is_Feature_Disabled_By_Small_Curve_Radius to determine if ESA is disabled by small curve radius. */
   result = Esa_Is_Disabled_By_Small_Curve_Radius(p_esa_calibration, p_vehicle_data, p_esa_persistent);

   /** \assert Verify that ESA is disabled by small curve radius. */
   EXPECT_FALSE(result);
}


/**
 * Check that ESA is not disabled by host speed, if the host speed is in the allowed range when the low speed hysteresis is
 * applied. \uts{CSCSA-123919} \sdd{CSCSA-65989} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Host_Speed_In_Activation_Range__returns_true_if_host_speed_in_allowed_range_if_low_speed_hysteresis_is_applied)
{
   /** \arrange Set up vehicle data such that host speed is in allowed range when the low speed hysteresis is applied. */
   boolean_T result = FBK_FALSE;
   p_vehicle_data->host_speed =
      p_esa_calibration->k_esa_host_activation_speed_min - p_esa_calibration->k_esa_host_activation_speed_min_hys + EPSILON;
   p_esa_persistent->f_host_speed_in_activation_range = FBK_TRUE;

   /** \action Call Esa_Is_Host_Above_Activation_Speed to determine if host speed is above ESA activation speed. */
   result = Esa_Is_Host_Speed_In_Activation_Range(p_esa_calibration, p_vehicle_data, p_esa_persistent);

   /** \assert Verify that true is returned by function. */
   EXPECT_TRUE(result);
}


/**
 * Check that ESA is not disabled by host speed, if the host speed is in the allowed range when the high speed hysteresis is
 * applied. \uts{CSCSA-123920} \sdd{CSCSA-65989} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Host_Speed_In_Activation_Range__returns_true_if_host_speed_in_allowed_range_if_high_speed_hysteresis_is_applied)
{
   /** \arrange Set up vehicle data such that host speed is in allowed range when the high speed hysteresis is applied. */
   boolean_T result = FBK_FALSE;
   p_vehicle_data->host_speed =
      p_esa_calibration->k_esa_host_activation_speed_max + p_esa_calibration->k_esa_host_activation_speed_max_hys - EPSILON;
   p_esa_persistent->f_host_speed_in_activation_range = FBK_TRUE;

   /** \action Call Esa_Is_Host_Above_Activation_Speed to determine if host speed is above ESA activation speed. */
   result = Esa_Is_Host_Speed_In_Activation_Range(p_esa_calibration, p_vehicle_data, p_esa_persistent);

   /** \assert Verify that true is returned by function. */
   EXPECT_TRUE(result);
}


/**
 * Check that ESA is disabled by host speed, if the host speed is too high
 * \uts{CSCSA-123921} \sdd{CSCSA-65989} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Host_Speed_In_Activation_Range__returns_false_if_host_speed_is_too_high)
{
   /** \arrange Set up vehicle data such that host speed is too high. */
   boolean_T result                                   = FBK_FALSE;
   p_vehicle_data->host_speed                         = p_esa_calibration->k_esa_host_activation_speed_max + EPSILON;
   p_esa_persistent->f_host_speed_in_activation_range = FBK_FALSE;

   /** \action Call Esa_Is_Host_Above_Activation_Speed to determine if host speed is above ESA activation speed. */
   result = Esa_Is_Host_Speed_In_Activation_Range(p_esa_calibration, p_vehicle_data, p_esa_persistent);

   /** \assert Verify that false is returned by function. */
   EXPECT_FALSE(result);
}


/**
 * Check that ESA is disabled by host speed, if the host speed is too low
 * \uts{CSCSA-123922} \sdd{CSCSA-65989} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Host_Speed_In_Activation_Range__returns_false_if_host_speed_is_too_low)
{
   /** \arrange Set up vehicle data such that host speed is too low. */
   boolean_T result                                   = FBK_FALSE;
   p_vehicle_data->host_speed                         = p_esa_calibration->k_esa_host_activation_speed_min - EPSILON;
   p_esa_persistent->f_host_speed_in_activation_range = FBK_FALSE;

   /** \action Call Esa_Is_Host_Above_Activation_Speed to determine if host speed is above ESA activation speed. */
   result = Esa_Is_Host_Speed_In_Activation_Range(p_esa_calibration, p_vehicle_data, p_esa_persistent);

   /** \assert Verify that false is returned by function. */
   EXPECT_FALSE(result);
}


/**
 * Check that function to update vehicle state updates all vehicle related persistent data.
 * \uts{CSCSA-123923} \sdd{CSCSA-65988} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Update_Vehicle_States__updates_all_vehicle_related_persistent_data)
{
   /** \arrange Set up vehicle data such that resulting persistent data will differ from initial values. */
   p_vehicle_data->host_speed                        = p_esa_calibration->k_esa_host_activation_speed_min + EPSILON;
   p_esa_calibration->k_esa_f_allow_min_curve_radius = 0u;

   p_esa_persistent->f_esa_disabled_low_curve_radius  = FBK_TRUE;
   p_esa_persistent->f_host_speed_in_activation_range = FBK_FALSE;

   /** \action Call Esa_Update_Vehicle_States to update all vehicle releated ESA states. */
   Esa_Update_Vehicle_States(p_vehicle_data, p_esa_calibration, p_esa_persistent);

   /** \assert Verify that all vehicle related persistent data is updated. */
   EXPECT_FALSE(p_esa_persistent->f_esa_disabled_low_curve_radius);
   EXPECT_TRUE(p_esa_persistent->f_host_speed_in_activation_range);
}


/**
 * Check that the initialization of the ESA core input works properly.
 * \uts{CSCSA-123924} \sdd{CSCSA-65974} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Init_Core_Input__works_properly)
{
   /** \arrange Set up ESA core input with non-default values. */
   p_esa_core_input->f_esa_enabled = FBK_TRUE;

   /** \action Call Esa_Init_Core_Input to initialize the ESA core input. */
   Esa_Init_Core_Input(p_esa_core_input);

   /** \assert Verify that all ESA core input values are set to default values. */
   EXPECT_FALSE(p_esa_core_input->f_esa_enabled);
}


/**
 * Check that the no critical object is reported for any submodule, if no valid objects are present for ESA.
 * \uts{CSCSA-123925} \sdd{CSCSA-65976} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Process__does_not_alert_any_object_if_no_valid_object_is_present)
{
   /** \arrange Set ESA enabled flags to true for all submodules in core output. Do not set up any valid ESA object. */
   p_esa_core_output->esa_core_status = ESA_CORE_STATUS_ENABLED;

   /** \action Call Esa_Process_All_Active_Submodules to process all active submodules. */
   Esa_Process(p_esa_core_output, p_esa_persistent, p_esa_core_input, p_vehicle_data, p_esa_calibration);

   /** \assert Verify that no alert is active for any submodule. */
   EXPECT_FALSE(p_esa_core_output->esa_alert[FBK_SIDE_LEFT]);
   EXPECT_FALSE(p_esa_core_output->esa_alert[FBK_SIDE_RIGHT]);
}


/**
 * Check that the no critical object is reported for any submodule, if a valid object is present but ESA is disabled
 * \uts{CSCSA-123926} \sdd{CSCSA-65976} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Process__does_not_alert_any_object_if_esa_is_disabled)
{
   /** \arrange Set ESA enabled flags to false for all submodules in core output. Set up a valid ESA object. */
   p_esa_core_output->esa_core_status = ESA_CORE_STATUS_DISABLED_BY_INPUT;

   uint8_t obj_idx                            = FBK_ZERO_UINT;
   object_data[obj_idx].id                    = 4u;
   object_data[obj_idx].existence_probability = 1.0f;
   object_data[obj_idx].age                   = p_esa_calibration->k_esa_min_track_age + 1u;
   object_data[obj_idx].vcs_pos.x             = -p_esa_calibration->k_esa_max_range * 0.5f;
   object_data[obj_idx].status                = PA_OBJ_STATUS_MATURE;

   /** \action Call Esa_Process_All_Active_Submodules to process all active submodules. */
   Esa_Process(p_esa_core_output, p_esa_persistent, p_esa_core_input, p_vehicle_data, p_esa_calibration);

   /** \assert Verify that no alert is active for any submodule. */
   EXPECT_FALSE(p_esa_core_output->esa_alert[FBK_SIDE_LEFT]);
   EXPECT_FALSE(p_esa_core_output->esa_alert[FBK_SIDE_RIGHT]);
}


/**
 * Check that the no critical object is reported, if a valid (but not warning relevant) object is present for ESA
 * \uts{CSCSA-123927} \sdd{CSCSA-65976} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Process__does_not_alert_any_object_if_all_submodules_are_enabled_but_object_in_not_warning_relevant)
{
   /** \arrange Set ESA enabled flags to false for all submodules in core output. Set up a valid ESA object (not warning relevant).
    * Set alerts for all submodules. */
   p_esa_core_output->esa_core_status = ESA_CORE_STATUS_ENABLED;

   uint8_t obj_idx                = FBK_ZERO_UINT;
   object_data[obj_idx].id        = 4u;
   object_data[obj_idx].age       = p_esa_calibration->k_esa_min_track_age + 1u;
   object_data[obj_idx].vcs_pos.x = -p_esa_calibration->k_esa_max_range * 0.5f;
   object_data[obj_idx].status    = PA_OBJ_STATUS_MATURE;

   p_esa_core_output->esa_alert[FBK_SIDE_LEFT]  = FBK_TRUE;
   p_esa_core_output->esa_alert[FBK_SIDE_RIGHT] = FBK_TRUE;

   /** \action Call Esa_Process_All_Active_Submodules to process all active submodules. */
   Esa_Process(p_esa_core_output, p_esa_persistent, p_esa_core_input, p_vehicle_data, p_esa_calibration);

   /** \assert Verify that no alert is active for any submodule. */
   EXPECT_FALSE(p_esa_core_output->esa_alert[FBK_SIDE_LEFT]);
   EXPECT_FALSE(p_esa_core_output->esa_alert[FBK_SIDE_RIGHT]);
}


/**
 * Test that ESA status is deactivate due to high speed.
 * \uts{CSCSA-123928} \sdd{CSCSA-65991} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Get_Activation_Status__high_speed)
{
   /** \arrange Set up ego speed and persistent data flag for ego speed. */
   Esa_Core_Status_T esa_status;
   p_esa_persistent->f_host_speed_in_activation_range = FBK_FALSE;
   p_vehicle_data->host_speed                         = p_esa_calibration->k_esa_host_activation_speed_max + EPSILON;

   /** \action Call Esa_Get_Activation_Status to get Esa activation status. */
   esa_status = Esa_Get_Activation_Status(p_esa_calibration, p_vehicle_data, p_esa_persistent);

   /** \assert Verify that ESA status is not active. */
   EXPECT_EQ(esa_status, ESA_CORE_STATUS_DEACTIVATED_HIGH_EGO_SPEED);
}


/**
 * Test that ESA status is internal error.
 * \uts{CSCSA-123929} \sdd{CSCSA-65991} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Get_Activation_Status__internal_error)
{
   /** \arrange Set up ego speed and persistent data flag for ego speed. */
   Esa_Core_Status_T esa_status;
   p_esa_persistent->f_host_speed_in_activation_range = FBK_FALSE;
   p_vehicle_data->host_speed                         = p_esa_calibration->k_esa_host_activation_speed_max - EPSILON;

   /** \action Call Esa_Get_Activation_Status to get Esa activation status. */
   esa_status = Esa_Get_Activation_Status(p_esa_calibration, p_vehicle_data, p_esa_persistent);

   /** \assert Verify that ESA status is not active. */
   EXPECT_EQ(esa_status, ESA_CORE_STATUS_DEACTIVATED_INTERNAL_ERROR);
}


/**
 * Test that object side is determined based on VCS lateral pos.
 * \uts{CSCSA-123930} \sdd{CSCSA-87222} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Get_Object_Side__returns_side_based_on_VCS_lat_pos)
{
   /** \arrange Set up object lateral positions: VCS and curvilinear. */
   Esa_Object_T esa_object;
   uint8_t obj_idx = 4u;
   uint8_t side;
   uint8_t expected_side = Fbk_Get_Obj_Side(1.0f);

   object_data[obj_idx].vcs_pos.y   = 3.0f;
   object_data[obj_idx].curvi_pos.y = -3.0f;
   esa_object.p_tracker_data        = &object_data[obj_idx];

   /** \action Call Esa_Get_Object_Side to determine object side. */
   side = Esa_Get_Object_Side(&esa_object, ESA_USE_VCS);

   /** \assert Verify that side is determined properly. */
   EXPECT_EQ(side, expected_side);
}


/**
 * Test that object side is determined based on curvilinear lateral pos.
 * \uts{CSCSA-123931} \sdd{CSCSA-87222} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Get_Object_Side__returns_side_based_on_curvi_lat_pos)
{
   /** \arrange Set up object lateral positions: VCS and curvilinear. */
   Esa_Object_T esa_object;
   uint8_t obj_idx = 4u;
   uint8_t side;
   uint8_t expected_side = Fbk_Get_Obj_Side(-1.0f);

   object_data[obj_idx].vcs_pos.y   = 3.0f;
   object_data[obj_idx].curvi_pos.y = -3.0f;
   esa_object.p_tracker_data        = &object_data[obj_idx];

   /** \action Call Esa_Get_Object_Side to determine object side. */
   side = Esa_Get_Object_Side(&esa_object, ESA_USE_CURVI);

   /** \assert Verify that side is determined properly. */
   EXPECT_EQ(side, expected_side);
}


/**
 * Check that maturity counter is incremented when the object is mature.
 * \uts{CSCSA-123932} \sdd{CSCSA-65982} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Increment_Mature_Count_In_Zone__increments_maturity_counter_when_object_mature)
{
   /** \arrange Set up maturity counter. */
   uint8_t count = 10u;

   /** \action Esa_Increment_Mature_Count_In_Zone . */
   Esa_Increment_Mature_Count_In_Zone(&count, PA_OBJ_STATUS_MATURE);

   /** \assert Verify that object maturity counter is incremented. */
   EXPECT_EQ(count, 11u);
}


/**
 * Check that maturity counter is not incremented when the object is not mature.
 * \uts{CSCSA-123933} \sdd{CSCSA-65982} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Increment_Mature_Count_In_Zone__increments_maturity_counter_when_object_not_mature)
{
   /** \arrange Set up maturity counter. */
   uint8_t count = 10u;

   /** \action Esa_Increment_Mature_Count_In_Zone . */
   Esa_Increment_Mature_Count_In_Zone(&count, PA_OBJ_STATUS_COASTED);

   /** \assert Verify that object maturity counter is not incremented. */
   EXPECT_EQ(count, 10u);
}


/**
 * Test that TTC is correctly calculated when conditions are fullfiled
 * \uts{CSCSA-123934} \sdd{CSCSA-65970} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Get_Longitudinal_TTC__calculates_TTC_when_conditions_fulfilled)
{
   /** \arrange Set up tracker object data and ego length. */
   Fbk_Object_Data_T p_tracker_data;
   float32_T ego_length = 4.0f;
   float32_T ttc;
   float32_T expected_ttc;

   p_tracker_data.curvi_vel_rel.x = 2.0f;
   p_tracker_data.curvi_pos.x     = -18.0f;
   p_tracker_data.length          = 4.0f;
   expected_ttc = (-(p_tracker_data.curvi_pos.x) - 0.5f * p_tracker_data.length - ego_length) / p_tracker_data.curvi_vel_rel.x;

   /** \action Call Esa_Get_TTC function */
   ttc = Esa_Get_Longitudinal_TTC((const Fbk_Object_Data_T *) &p_tracker_data, ego_length);

   /** \assert Verify that TTC is calculated correctly */
   EXPECT_FLOAT_EQ(ttc, expected_ttc);
}


/**
 * Test that TTC function will not calculate TTC when conditions are not met
 * \uts{CSCSA-123935} \sdd{CSCSA-65970} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Get_Longitudinal_TTC__calculates_TTC_when_conditions_not_fulfilled)
{
   /** \arrange Set up tracker object data and ego length. */
   Fbk_Object_Data_T p_tracker_data;
   float32_T ego_length = 4.0f;
   float32_T ttc;

   p_tracker_data.curvi_vel_rel.x = -2.0f;
   p_tracker_data.curvi_pos.x     = 1.0f;
   p_tracker_data.length          = 4.0f;

   /** \action Call Esa_Get_TTC */
   ttc = Esa_Get_Longitudinal_TTC((const Fbk_Object_Data_T *) &p_tracker_data, ego_length);

   /** \assert Verify that TTC has invalid value */
   EXPECT_FLOAT_EQ(ttc, ESA_DEFAULT_LARGE_TTC);
}


/**
 * Test that TTC function will not calculate TTC when relative velocity is positive but long. posision is negative
 * \uts{CSCSA-209334} \sdd{CSCSA-65970} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Get_Longitudinal_TTC__positive_rel_vel_negative_long_pos)
{
   /** \arrange Set up tracker object data and ego length. */
   Fbk_Object_Data_T p_tracker_data;
   float32_T ego_length = 4.0f;
   float32_T ttc;

   p_tracker_data.curvi_vel_rel.x = 2.0f;
   p_tracker_data.curvi_pos.x     = 1.0f;
   p_tracker_data.length          = 4.0f;

   /** \action Call Esa_Get_TTC */
   ttc = Esa_Get_Longitudinal_TTC((const Fbk_Object_Data_T *) &p_tracker_data, ego_length);

   /** \assert Verify that TTC has invalid value */
   EXPECT_FLOAT_EQ(ttc, ESA_DEFAULT_LARGE_TTC);
}


/**
 * Test that TTP is properly calculated, when conditions are met
 * \uts{CSCSA-123936} \sdd{CSCSA-65971} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Get_TTP__calculates_TTP_when_object_curvi_rel_long_vel_GT_0)
{
   /** \arrange Set up tracker object data */
   Fbk_Object_Data_T p_tracker_data;
   float32_T ttp;
   float32_T expected_ttp;

   p_tracker_data.curvi_vel_rel.x = 2.0f;
   p_tracker_data.curvi_pos.x     = -18.0f;
   p_tracker_data.length          = 4.0f;
   expected_ttp                   = -(p_tracker_data.curvi_pos.x - 0.5f * p_tracker_data.length) / p_tracker_data.curvi_vel_rel.x;

   /** \action Call Esa_Get_TTP */
   ttp = Esa_Get_TTP((const Fbk_Object_Data_T *) &p_tracker_data);

   /** \assert Verify that TTP have correct value */
   EXPECT_FLOAT_EQ(ttp, expected_ttp);
}


/**
 * Check that TTP is zero when the object with relative curvi velocity greater then zero is in front.
 * \uts{CSCSA-123937} \sdd{CSCSA-65971} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Get_TTP__calculates_TTP_when_object_curvi_rel_long_vel_GT_0_and_object_in_front)
{
   /** \arrange Set up tracker object data. */
   Fbk_Object_Data_T p_tracker_data;
   float32_T ttp;

   p_tracker_data.curvi_vel_rel.x = 2.0f;
   p_tracker_data.curvi_pos.x     = 10.0f;
   p_tracker_data.length          = 4.0f;

   /** \action Call Esa_Get_TTP . */
   ttp = Esa_Get_TTP((const Fbk_Object_Data_T *) &p_tracker_data);

   /** \assert Verify that TTP is zero . */
   EXPECT_FLOAT_EQ(ttp, FBK_ZERO_F);
}


/**
 * Test that TTP will not be calculated when relative velocity is 0
 * \uts{CSCSA-123938} \sdd{CSCSA-65971} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Get_TTP__calculates_TTP_when_object_curvi_rel_long_vel_LE_0)
{
   /** \arrange Set up tracker object data */
   Fbk_Object_Data_T p_tracker_data;
   float32_T ttp;

   p_tracker_data.curvi_vel_rel.x = -2.0f;
   p_tracker_data.curvi_pos.x     = -18.0f;
   p_tracker_data.length          = 4.0f;

   /** \action Call Esa_Get_TTP . */
   ttp = Esa_Get_TTP((const Fbk_Object_Data_T *) &p_tracker_data);

   /** \assert Verify that TTP has invalid value */
   EXPECT_FLOAT_EQ(ttp, ESA_DEFAULT_LARGE_TTC);
}


/**
 * Test that esa hold counter will be reset for each side when alert is active
 * \uts{CSCSA-123939} \sdd{CSCSA-65969} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Process_Output__processes_output_for_alerts_active)
{
   /** \arrange Set up side alerts */
   uint8_t side_idx;

   for (side_idx = FBK_ZERO_UINT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
   {
      p_esa_core_output->esa_alert[side_idx]       = true;
      p_esa_persistent->esa_hold_counter[side_idx] = 1u;
   }

   /** \action Call Esa_Process_Output . */
   Esa_Process_Output(p_esa_core_output, p_esa_persistent, p_esa_calibration);

   /** \assert Verify that persistent esa_hold_counter is reset for each side */
   for (side_idx = FBK_ZERO_UINT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
   {
      EXPECT_EQ(p_esa_persistent->esa_hold_counter[side_idx], FBK_ZERO_UINT);
   }
}


/**
 * Test that persistent and output values are set correctly for each side. When alert is inactive and hold conditions are met
 * \uts{CSCSA-123940} \sdd{CSCSA-65969} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Process_Output__processes_output_for_alerts_inactive_and_with_hold_conditions)
{
   /** \arrange Set up sid */
   uint8_t side_idx;
   p_esa_calibration->k_esa_alert_holding_cycles = 2u;

   for (side_idx = FBK_ZERO_UINT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
   {
      p_esa_core_output->esa_alert[side_idx]               = false;
      p_esa_persistent->prev_esa_alert_obj_index[side_idx] = 10u;
      p_esa_persistent->prev_esa_alert_obj_id[side_idx]    = 4u;
      p_esa_persistent->esa_hold_counter[side_idx]         = p_esa_calibration->k_esa_alert_holding_cycles - 1u;
   }

   /** \action Call Esa_Process_Output . */
   Esa_Process_Output(p_esa_core_output, p_esa_persistent, p_esa_calibration);

   /** \assert Verify that persistent and output values are set correctly */
   for (side_idx = FBK_ZERO_UINT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
   {
      EXPECT_EQ(p_esa_persistent->esa_hold_counter[side_idx], p_esa_calibration->k_esa_alert_holding_cycles);
      EXPECT_TRUE(p_esa_core_output->esa_alert[side_idx]);
      EXPECT_EQ(p_esa_core_output->esa_index[side_idx], p_esa_persistent->prev_esa_alert_obj_index[side_idx]);
      EXPECT_EQ(p_esa_core_output->esa_id[side_idx], p_esa_persistent->prev_esa_alert_obj_id[side_idx]);
   }
}


/**
 * Test that perstistent values are zero for inactive alerts and without holding conditions
 * \uts{CSCSA-123941} \sdd{CSCSA-65969} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Process_Output__processes_output_for_alerts_inactive_and_without_hold_conditions)
{
   /** \arrange Set up esa alerts for both sides. */
   uint8_t side_idx;
   p_esa_calibration->k_esa_alert_holding_cycles = 10u;

   for (side_idx = FBK_ZERO_UINT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
   {
      p_esa_core_output->esa_alert[side_idx]            = false;
      p_esa_persistent->prev_esa_alert_obj_id[side_idx] = PA_INVALID_OBJ_ID;
      p_esa_persistent->esa_hold_counter[side_idx]      = p_esa_calibration->k_esa_alert_holding_cycles - 1u;
   }

   /** \action Call Esa_Process_Output . */
   Esa_Process_Output(p_esa_core_output, p_esa_persistent, p_esa_calibration);

   /** \assert Verify that persistent values are zero . */
   for (side_idx = FBK_ZERO_UINT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
   {
      EXPECT_EQ(p_esa_persistent->esa_hold_counter[side_idx], FBK_ZERO_UINT);
   }
}


/**
 * Test that perstistent values are zero for inactive alerts and without holding conditions
 * \uts{CSCSA-209335} \sdd{CSCSA-65969} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Process_Output__alerts_inactive_without_hold_conditions_hold_counter_above_the_threshold)
{
   /** \arrange Set up esa alerts for both sides. */
   uint8_t side_idx;

   for (side_idx = FBK_ZERO_UINT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
   {
      p_esa_core_output->esa_alert[side_idx]            = false;
      p_esa_persistent->prev_esa_alert_obj_id[side_idx] = side_idx + 1u;
      p_esa_persistent->esa_hold_counter[side_idx]      = p_esa_calibration->k_esa_alert_holding_cycles;
   }

   /** \action Call Esa_Process_Output . */
   Esa_Process_Output(p_esa_core_output, p_esa_persistent, p_esa_calibration);

   /** \assert Verify that persistent values are zero . */
   for (side_idx = FBK_ZERO_UINT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
   {
      EXPECT_EQ(p_esa_persistent->esa_hold_counter[side_idx], FBK_ZERO_UINT);
   }
}

/**
 * Check that persistents are reset.
 * \uts{CSCSA-123943} \sdd{CSCSA-65955} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Reset_Persistent_Data__resets_persistents)
{
   /** \arrange */
   uint8_t object_idx;
   uint8_t side_idx;
   p_esa_persistent->f_host_speed_in_activation_range = FBK_TRUE;
   p_esa_persistent->f_esa_disabled_low_curve_radius  = FBK_TRUE;

   for (object_idx = FBK_ZERO_UINT; object_idx < PA_OBJ_NUMBER_OF_OBJECTS; object_idx++)
   {
      p_esa_persistent->mature_count_in_esa_zone[object_idx] = FBK_ONE_UINT;
   }

   for (side_idx = FBK_ZERO_UINT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
   {
      p_esa_persistent->prev_esa_alert_obj_index[side_idx] = FBK_ONE_UINT;
      p_esa_persistent->prev_esa_alert_obj_id[side_idx]    = FBK_ONE_UINT;
      p_esa_persistent->esa_hold_counter[side_idx]         = FBK_ONE_UINT;
   }

   /** \action Call Esa_Reset_Persistents. */
   Esa_Reset_Persistent_Data(p_esa_persistent);

   /** \assert Verify that all persistents are reset. */
   for (object_idx = FBK_ZERO_UINT; object_idx < PA_OBJ_NUMBER_OF_OBJECTS; object_idx++)
   {
      EXPECT_EQ(p_esa_persistent->mature_count_in_esa_zone[object_idx], FBK_ZERO_UINT);
   }

   for (side_idx = FBK_ZERO_UINT; side_idx < FBK_NUMBER_OF_SIDES; side_idx++)
   {
      EXPECT_EQ(p_esa_persistent->prev_esa_alert_obj_index[side_idx], PA_INVALID_OBJ_INDEX);
      EXPECT_EQ(p_esa_persistent->prev_esa_alert_obj_id[side_idx], PA_INVALID_OBJ_ID);
      EXPECT_EQ(p_esa_persistent->esa_hold_counter[side_idx], FBK_ZERO_UINT);
   }

   EXPECT_FALSE(p_esa_persistent->f_host_speed_in_activation_range);
   EXPECT_FALSE(p_esa_persistent->f_esa_disabled_low_curve_radius);
}

#ifndef NDEBUG
/**
 * Check that NULL persistents pointer throws an assertion.
 * \uts{CSCSA-123942} \sdd{CSCSA-65955} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Reset_Persistent_Data__NULL_persistents_pointer_throws_assertion)
{
#ifdef NDEBUG
   GTEST_SKIP()
#endif

   /** \arrange */
   /** \action See assert. */
   /** \assert Verify that an assert is thrown, when Esa_Reset_Persistent_Data function is called with null pointer for
    * persistents. */
   EXPECT_DEATH({ Esa_Reset_Persistent_Data(NULL); }, ".*p_esa_persistent.*");
}

/**
 * Check that NULL esa object pointer throws an assertion.
 * \uts{CSCSA-123944} \sdd{CSCSA-65984} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Init_Object_Data__NULL_esa_object_pointer_throws_assertion)
{
   /** \arrange */
   Fbk_Object_Data_T p_tracker_data;

   /** \action See assert. */
   /** \assert Verify that an assert is thrown, when Esa_Init_Object_Data function is called with null pointer for esa object
    * pointer. */
   EXPECT_DEATH({ Esa_Init_Object_Data(NULL, &p_tracker_data); }, ".*p_esa_object.*");
}


/**
 * Check that NULL tracker object pointer throws an assertion.
 * \uts{CSCSA-123945} \sdd{CSCSA-65984} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Init_Object_Data__NULL_tracker_object_pointer_throws_assertion)
{
   /** \arrange */
   Esa_Object_T esa_object;

   /** \action See assert. */
   /** \assert Verify that an assert is thrown, when Esa_Init_Object_Data function is called with null pointer for tracker object
    * pointer. */
   EXPECT_DEATH({ Esa_Init_Object_Data(&esa_object, NULL); }, ".*tracker_object.*");
}
#endif

/**
 * Check that esa object data is initialized.
 * \uts{CSCSA-123946} \sdd{CSCSA-65984} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Init_Object_Data__initializes_esa_object_data)
{
   /** \arrange */
   Esa_Object_T esa_object;
   Fbk_Object_Data_T p_tracker_data;
   uint8_t point_idx;

   p_tracker_data.id    = 11u;
   p_tracker_data.index = 4u;

   /** \action Call Esa_Init_Object_Data. */
   Esa_Init_Object_Data(&esa_object, &p_tracker_data);

   /** \assert Verify that esa object is initialized. */
   EXPECT_EQ(esa_object.long_ttc, ESA_DEFAULT_LARGE_TTC);
   EXPECT_EQ(esa_object.ttp, FBK_ZERO_F);
   EXPECT_EQ(esa_object.obj_decel_to_reach_host_speed, FBK_ZERO_F);
   EXPECT_EQ(esa_object.obj_long_dist, -ESA_DEFAULT_OBJ_DIST);
   EXPECT_EQ(esa_object.ego_side, FBK_SIDE_UNDEFINED);
   EXPECT_EQ(esa_object.f_obj_in_zone, FBK_FALSE);
   EXPECT_EQ(esa_object.f_obj_ttc_below_threshold, FBK_FALSE);
   EXPECT_EQ(esa_object.f_obj_decel_above_threshold, FBK_FALSE);
   EXPECT_EQ(esa_object.p_tracker_data, &p_tracker_data);
   for (point_idx = FBK_ZERO_UINT; point_idx < FBK_MAX_SIZE_OF_FOI; point_idx++)
   {
      EXPECT_FLOAT_EQ(esa_object.zone.points[point_idx].x, FBK_ZERO_F);
      EXPECT_FLOAT_EQ(esa_object.zone.points[point_idx].y, FBK_ZERO_F);
   }
}


/**
 * Check that FOI is calculated in VCS coordinates.
 * \uts{CSCSA-123947} \sdd{CSCSA-65979} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Create_Object_Field_Of_Interest__calculates_FOI_in_VCS_coordinates)
{
   /** \arrange Set up tracker object data. */
   Coordinate_System_T coordinate_system;
   Fbk_Field_Of_Interest_T p_object_foi;
   Fbk_Object_Data_T tracker_object;

   coordinate_system            = ESA_USE_VCS;
   tracker_object.curvi_pos.x   = -10.0f;
   tracker_object.curvi_pos.y   = -2.0f;
   tracker_object.length        = 4.0f;
   tracker_object.width         = 2.0f;
   tracker_object.vcs_heading   = 0.0f;
   tracker_object.curvi_heading = 0.25f * PI;

   /** \action Call Esa_Create_Object_Field_Of_Interest. */
   Esa_Create_Object_Field_Of_Interest(&p_object_foi, &tracker_object, coordinate_system);

   /** \assert Verify that FOI is calculated properly. */
   EXPECT_FLOAT_EQ(p_object_foi.points[0].x, -8.0f);
   EXPECT_FLOAT_EQ(p_object_foi.points[0].y, -3.0f);
}


/**
 * Check that FOI is calculated in curvi coordinates.
 * \uts{CSCSA-123948} \sdd{CSCSA-65979} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Create_Object_Field_Of_Interest__calculates_FOI_in_curvi_coordinates)
{
   /** \arrange Set up tracker object data . */
   Coordinate_System_T coordinate_system;
   Fbk_Field_Of_Interest_T p_object_foi;
   Fbk_Object_Data_T tracker_object;

   coordinate_system            = ESA_USE_CURVI;
   tracker_object.curvi_pos.x   = -10.0f;
   tracker_object.curvi_pos.y   = -2.0f;
   tracker_object.length        = 4.0f;
   tracker_object.width         = 2.0f;
   tracker_object.vcs_heading   = 0.0f;
   tracker_object.curvi_heading = 0.25f * PI;

   /** \action Call Esa_Create_Object_Field_Of_Interest. */
   Esa_Create_Object_Field_Of_Interest(&p_object_foi, &tracker_object, coordinate_system);

   /** \assert Verify that FOI is calculated properly . */
   EXPECT_FLOAT_EQ(p_object_foi.points[0].x, -7.87868023f);
   EXPECT_FLOAT_EQ(p_object_foi.points[0].y, -1.29289329f);
}


/**
 * Check that object in zone existence is determined.
 * \uts{CSCSA-123949} \sdd{CSCSA-65981} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Object_In_Zone__calculates_if_object_is_in_zone)
{
   /** \arrange Set up tracker object data and esa zone. */
   Coordinate_System_T coordinate_system;
   Fbk_Field_Of_Interest_T p_zone;
   Fbk_Object_Data_T tracker_object;
   boolean_T f_object_in_zone;

   coordinate_system          = ESA_USE_VCS;
   tracker_object.curvi_pos.x = -10.0f;
   tracker_object.curvi_pos.y = 2.0f;
   tracker_object.length      = 4.0f;
   tracker_object.width       = 2.0f;
   tracker_object.vcs_heading = 0.0f;
   p_zone.size                = 6u;
   p_zone.points[0].x         = 0.0f;
   p_zone.points[1].x         = -10.0f;
   p_zone.points[2].x         = -20.0f;
   p_zone.points[0].y         = 5.0f;
   p_zone.points[5].y         = 0.0f;
   p_zone.points[5].x         = p_zone.points[0].x;
   p_zone.points[4].x         = p_zone.points[1].x;
   p_zone.points[3].x         = p_zone.points[2].x;
   p_zone.points[1].y         = p_zone.points[0].y;
   p_zone.points[2].y         = p_zone.points[0].y;
   p_zone.points[4].y         = p_zone.points[5].y;
   p_zone.points[3].y         = p_zone.points[5].y;

   /** \action Call Esa_Is_Object_In_Zone. */
   f_object_in_zone = Esa_Is_Object_In_Zone(&tracker_object, &p_zone, coordinate_system);

   /** \assert Verify that object detected in zone. */
   EXPECT_TRUE(f_object_in_zone);
}


/**
 * Check that object deceleration is considered critical when the object was not critical previously.
 * \uts{CSCSA-123950} \sdd{CSCSA-65975} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Deceleration_Critical__calculates_if_object_deceleration_is_critical_and_object_was_not_most_critical)
{
   /** \arrange Set up esa object data and persistents. */
   Esa_Object_T p_esa_object;
   Fbk_Object_Data_T p_tracker_data;
   boolean_T f_obj_decel_above_threshold;

   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_TRUE;
   p_esa_object.p_tracker_data                                        = &p_tracker_data;
   p_tracker_data.id                                                  = 11u;
   p_esa_persistent->prev_esa_alert_obj_id[FBK_SIDE_LEFT]             = 0u;
   p_esa_persistent->prev_esa_alert_obj_id[FBK_SIDE_RIGHT]            = 0u;
   p_esa_object.obj_decel_to_reach_host_speed = p_esa_calibration->k_esa_obj_safe_deceleration_threshold + 0.1f;

   /** \action Call Esa_Is_Deceleration_Critical. */
   f_obj_decel_above_threshold = Esa_Is_Deceleration_Critical(&p_esa_object, p_esa_calibration, p_esa_persistent);

   /** \assert Verify that deceleration is critical. */
   EXPECT_TRUE(f_obj_decel_above_threshold);
}


/**
 * Check that object deceleration is considered critical when the object was critical previously.
 * \uts{CSCSA-123951} \sdd{CSCSA-65975} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Deceleration_Critical__calculates_if_object_deceleration_is_critical_and_object_was_most_critical)
{
   /** \arrange Set up esa object data and persistents. */
   Esa_Object_T p_esa_object;
   Fbk_Object_Data_T p_tracker_data;
   boolean_T f_obj_decel_above_threshold;

   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_TRUE;
   p_esa_object.p_tracker_data                                        = &p_tracker_data;
   p_tracker_data.id                                                  = 11u;
   p_esa_persistent->prev_esa_alert_obj_id[FBK_SIDE_LEFT]             = 11u;
   p_esa_persistent->prev_esa_alert_obj_id[FBK_SIDE_RIGHT]            = 0u;
   p_esa_object.obj_decel_to_reach_host_speed = p_esa_calibration->k_esa_obj_safe_deceleration_threshold_hys + 0.1f;

   /** \action Call Esa_Is_Deceleration_Critical. */
   f_obj_decel_above_threshold = Esa_Is_Deceleration_Critical(&p_esa_object, p_esa_calibration, p_esa_persistent);

   /** \assert Verify that deceleration is critical. */
   EXPECT_TRUE(f_obj_decel_above_threshold);
}


/**
 * Check that object deceleration is considered critical when the host is stationary.
 * \uts{CSCSA-123952} \sdd{CSCSA-65975} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Deceleration_Critical__calculates_if_object_deceleration_is_critical_when_host_host_is_stationary)
{
   /** \arrange Set up esa object data and persistents. */
   Esa_Object_T p_esa_object;
   Fbk_Object_Data_T p_tracker_data;
   boolean_T f_obj_decel_above_threshold;

   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_TRUE;
   p_esa_object.p_tracker_data                                        = &p_tracker_data;
   p_tracker_data.id                                                  = 11u;
   p_esa_persistent->prev_esa_alert_obj_id[FBK_SIDE_LEFT]             = 11u;
   p_esa_persistent->prev_esa_alert_obj_id[FBK_SIDE_RIGHT]            = 0u;
   p_esa_object.obj_decel_to_reach_host_speed                         = 0.0f;

   /** \action Call Esa_Is_Deceleration_Critical. */
   f_obj_decel_above_threshold = Esa_Is_Deceleration_Critical(&p_esa_object, p_esa_calibration, p_esa_persistent);

   /** \assert Verify that deceleration is not critical. */
   EXPECT_FALSE(f_obj_decel_above_threshold);
}


/**
 * Check that object deceleration is considered critical when the host velocity is too small.
 * \uts{CSCSA-123953} \sdd{CSCSA-65975} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Deceleration_Critical__calculates_if_object_deceleration_is_critical_when_host_velocity_is_too_small)
{
   /** \arrange Set up esa object data and persistents. */
   Esa_Object_T p_esa_object;
   Fbk_Object_Data_T p_tracker_data;
   boolean_T f_obj_decel_above_threshold;

   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_TRUE;
   p_esa_object.p_tracker_data                                        = &p_tracker_data;
   p_tracker_data.id                                                  = 11u;
   p_esa_persistent->prev_esa_alert_obj_id[FBK_SIDE_LEFT]             = 11u;
   p_esa_persistent->prev_esa_alert_obj_id[FBK_SIDE_RIGHT]            = 0u;
   p_esa_object.obj_decel_to_reach_host_speed                         = 0.1f;

   /** \action Call Esa_Is_Deceleration_Critical. */
   f_obj_decel_above_threshold = Esa_Is_Deceleration_Critical(&p_esa_object, p_esa_calibration, p_esa_persistent);

   /** \assert Verify that deceleration is not critical. */
   EXPECT_FALSE(f_obj_decel_above_threshold);
}


/**
 * Check that object deceleration is not considered critical when TTC-and-deceleration condition is disabled.
 * \uts{CSCSA-209336} \sdd{CSCSA-65975} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Deceleration_Critical__ttc_and_deceleration_disabled)
{
   /** \arrange Set up esa object data and persistents. */
   Esa_Object_T p_esa_object;
   Fbk_Object_Data_T p_tracker_data;
   boolean_T f_obj_decel_above_threshold;

   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_FALSE;
   p_esa_object.p_tracker_data                                        = &p_tracker_data;
   p_tracker_data.id                                                  = 11u;
   p_esa_persistent->prev_esa_alert_obj_id[FBK_SIDE_LEFT]             = 11u;
   p_esa_persistent->prev_esa_alert_obj_id[FBK_SIDE_RIGHT]            = 0u;
   p_esa_object.obj_decel_to_reach_host_speed = p_esa_calibration->k_esa_obj_safe_deceleration_threshold + 0.1f;

   /** \action Call Esa_Is_Deceleration_Critical. */
   f_obj_decel_above_threshold = Esa_Is_Deceleration_Critical(&p_esa_object, p_esa_calibration, p_esa_persistent);

   /** \assert Verify that deceleration is not critical. */
   EXPECT_FALSE(f_obj_decel_above_threshold);
}


/**
 * Check that objects TTC is considered critical when the object was not critical.
 * \uts{CSCSA-123954} \sdd{CSCSA-65973} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Ttc_Below_Threshold__calculates_if_object_ttc_is_critical_and_object_was_not_most_critical)
{
   /** \arrange Set up esa object data and persistents. */
   Esa_Object_T p_esa_object;
   Fbk_Object_Data_T p_tracker_data;
   boolean_T f_obj_ttc_below_threshold;

   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_TRUE;
   p_esa_object.p_tracker_data                                        = &p_tracker_data;
   p_tracker_data.id                                                  = 11u;
   p_esa_persistent->prev_esa_alert_obj_id[FBK_SIDE_LEFT]             = 0u;
   p_esa_persistent->prev_esa_alert_obj_id[FBK_SIDE_RIGHT]            = 0u;
   p_esa_object.long_ttc                                              = p_esa_calibration->k_esa_critical_longitudinal_ttc - 0.1f;

   /** \action Call Esa_Is_Ttc_Below_Threshold. */
   f_obj_ttc_below_threshold = Esa_Is_Ttc_Below_Threshold(&p_esa_object, p_esa_calibration, p_esa_persistent);

   /** \assert Verify that ttc is critical. */
   EXPECT_TRUE(f_obj_ttc_below_threshold);
}


/**
 * Check that objects TTC is considered critical when the object was critical.
 * \uts{CSCSA-123955} \sdd{CSCSA-65973} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Ttc_Below_Threshold__calculates_if_object_ttc_is_critical_and_object_was_most_critical)
{
   /** \arrange Set up esa object data and persistents. */
   Esa_Object_T p_esa_object;
   Fbk_Object_Data_T p_tracker_data;
   boolean_T f_obj_ttc_below_threshold;

   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_TRUE;
   p_esa_object.p_tracker_data                                        = &p_tracker_data;
   p_tracker_data.id                                                  = 11u;
   p_esa_persistent->prev_esa_alert_obj_id[FBK_SIDE_LEFT]             = 11u;
   p_esa_persistent->prev_esa_alert_obj_id[FBK_SIDE_RIGHT]            = 0u;
   p_esa_object.long_ttc = p_esa_calibration->k_esa_critical_longitudinal_ttc_hys - 0.1f;

   /** \action Call Esa_Is_Ttc_Below_Threshold. */
   f_obj_ttc_below_threshold = Esa_Is_Ttc_Below_Threshold(&p_esa_object, p_esa_calibration, p_esa_persistent);

   /** \assert Verify that ttc is critical. */
   EXPECT_TRUE(f_obj_ttc_below_threshold);
}


/**
 * Check that objects TTC is considered not critical when TTC is zero.
 * \uts{CSCSA-123956} \sdd{CSCSA-65973} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Ttc_Below_Threshold__calculates_if_object_ttc_is_not_critical_when_ttc_is_equal_zero)
{
   /** \arrange Set up esa object data and persistents. */
   Esa_Object_T p_esa_object;
   Fbk_Object_Data_T p_tracker_data;
   boolean_T f_obj_ttc_below_threshold;

   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_TRUE;
   p_esa_object.p_tracker_data                                        = &p_tracker_data;
   p_tracker_data.id                                                  = 11u;
   p_esa_persistent->prev_esa_alert_obj_id[FBK_SIDE_LEFT]             = 0u;
   p_esa_persistent->prev_esa_alert_obj_id[FBK_SIDE_RIGHT]            = 0u;
   p_esa_object.long_ttc                                              = 0.0f;

   /** \action Call Esa_Is_Ttc_Below_Threshold. */
   f_obj_ttc_below_threshold = Esa_Is_Ttc_Below_Threshold(&p_esa_object, p_esa_calibration, p_esa_persistent);

   /** \assert Verify that ttc is not critical. */
   EXPECT_FALSE(f_obj_ttc_below_threshold);
}


/**
 * Check that objects TTC is considered not critical when the TTC is above the threshold.
 * \uts{CSCSA-123957} \sdd{CSCSA-65973} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Ttc_Below_Threshold__calculates_if_object_ttc_is_not_critical_when_ttc_is_too_high)
{
   /** \arrange Set up esa object data and persistents. */
   Esa_Object_T p_esa_object;
   Fbk_Object_Data_T p_tracker_data;
   boolean_T f_obj_ttc_below_threshold;

   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_TRUE;
   p_esa_object.p_tracker_data                                        = &p_tracker_data;
   p_tracker_data.id                                                  = 11u;
   p_esa_persistent->prev_esa_alert_obj_id[FBK_SIDE_LEFT]             = 0u;
   p_esa_persistent->prev_esa_alert_obj_id[FBK_SIDE_RIGHT]            = 0u;
   p_esa_object.long_ttc = p_esa_calibration->k_esa_critical_longitudinal_ttc_hys + 0.1f;

   /** \action Call Esa_Is_Ttc_Below_Threshold. */
   f_obj_ttc_below_threshold = Esa_Is_Ttc_Below_Threshold(&p_esa_object, p_esa_calibration, p_esa_persistent);

   /** \assert Verify that ttc is not critical. */
   EXPECT_FALSE(f_obj_ttc_below_threshold);
}


/**
 * Check that objects TTC is not considered critical when TTC-and-deceleration condition is disabled.
 * \uts{CSCSA-209337} \sdd{CSCSA-65973} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Ttc_Below_Threshold__ttc_and_deceleration_disabled)
{
   /** \arrange Set up esa object data and persistents. */
   Esa_Object_T p_esa_object;
   Fbk_Object_Data_T p_tracker_data;
   boolean_T f_obj_ttc_below_threshold;

   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_FALSE;
   p_esa_object.p_tracker_data                                        = &p_tracker_data;
   p_tracker_data.id                                                  = 11u;
   p_esa_persistent->prev_esa_alert_obj_id[FBK_SIDE_LEFT]             = 11u;
   p_esa_persistent->prev_esa_alert_obj_id[FBK_SIDE_RIGHT]            = 0u;
   p_esa_object.long_ttc                                              = p_esa_calibration->k_esa_critical_longitudinal_ttc + 0.1f;

   /** \action Call Esa_Is_Ttc_Below_Threshold. */
   f_obj_ttc_below_threshold = Esa_Is_Ttc_Below_Threshold(&p_esa_object, p_esa_calibration, p_esa_persistent);

   /** \assert Verify that ttc is not critical. */
   EXPECT_FALSE(f_obj_ttc_below_threshold);
}


/**
 * Check that most critical object is not set when ttc-and-deceleration criterion is disabled.
 * \uts{CSCSA-123958} \sdd{CSCSA-65986} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Set_Most_Critical_Object__does_not_set_most_critical_object_when_ttc_and_deceleration_criterion_is_disabled)
{
   /** \arrange Set up esa object data and esa cals. */
   Esa_Object_T p_esa_object;

   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_FALSE;
   p_esa_calibration->k_esa_f_allow_obj_selection_ttc                 = FBK_FALSE;
   p_esa_calibration->k_esa_f_allow_obj_selection_deceleration        = FBK_FALSE;
   p_esa_calibration->k_esa_f_allow_obj_selection_long_distance       = FBK_FALSE;
   p_esa_object.f_obj_ttc_below_threshold                             = FBK_FALSE;
   p_esa_object.f_obj_decel_above_threshold                           = FBK_FALSE;
   p_esa_object.ego_side                                              = FBK_SIDE_LEFT;

   /** \action Call Esa_Set_Most_Critical_Object. */
   Esa_Set_Most_Critical_Object(p_esa_core_output, &p_esa_object, p_esa_calibration);

   /** \assert Verify that most critical object is not set . */
   EXPECT_FALSE(p_esa_core_output->esa_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(p_esa_core_output->esa_id[FBK_SIDE_LEFT], 0u);
}


/**
 * Check that most critical object is not set when TTC is not critical.
 * \uts{CSCSA-123959} \sdd{CSCSA-65986} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Set_Most_Critical_Object__does_not_set_most_critical_object_if_object_ttc_is_above_threshold)
{
   /** \arrange Set up esa object data and esa cals. */
   Esa_Object_T p_esa_object;

   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_TRUE;
   p_esa_object.f_obj_ttc_below_threshold                             = FBK_FALSE;
   p_esa_object.f_obj_decel_above_threshold                           = FBK_TRUE;
   p_esa_object.ego_side                                              = FBK_SIDE_LEFT;

   /** \action Call Esa_Set_Most_Critical_Object. */
   Esa_Set_Most_Critical_Object(p_esa_core_output, &p_esa_object, p_esa_calibration);

   /** \assert Verify that most critical object is not set . */
   EXPECT_FALSE(p_esa_core_output->esa_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(p_esa_core_output->esa_id[FBK_SIDE_LEFT], 0u);
}


/**
 * Check that most critical object is not set when the TTC is critical but ttc criterion is disabled.
 * \uts{CSCSA-123960} \sdd{CSCSA-65986} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test,
       Esa_Set_Most_Critical_Object__does_not_set_most_critical_object_if_object_ttc_is_low_enough_but_ttc_criterion_is_disabled)
{
   /** \arrange Set up esa object data and esa cals. */
   Esa_Object_T p_esa_object;

   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_TRUE;
   p_esa_object.f_obj_ttc_below_threshold                             = FBK_TRUE;
   p_esa_object.f_obj_decel_above_threshold                           = FBK_TRUE;
   p_esa_object.ego_side                                              = FBK_SIDE_LEFT;
   p_esa_calibration->k_esa_f_allow_obj_selection_ttc                 = FBK_FALSE;
   p_esa_calibration->k_esa_f_allow_obj_selection_deceleration        = FBK_FALSE;
   p_esa_calibration->k_esa_f_allow_obj_selection_long_distance       = FBK_FALSE;
   p_esa_object.long_ttc                                              = 0.1f;

   /** \action Call Esa_Set_Most_Critical_Object. */
   Esa_Set_Most_Critical_Object(p_esa_core_output, &p_esa_object, p_esa_calibration);

   /** \assert Verify that most critical object is not set . */
   EXPECT_FALSE(p_esa_core_output->esa_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(p_esa_core_output->esa_id[FBK_SIDE_LEFT], 0u);
}


/**
 * Check that most critical object is not set when the object deceleration is not critical.
 * \uts{CSCSA-123961} \sdd{CSCSA-65986} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Set_Most_Critical_Object__does_not_set_most_critical_object_if_object_deceleration_is_below_threshold)
{
   /** \arrange Set up esa object data and esa cals . */
   Esa_Object_T p_esa_object;

   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_TRUE;
   p_esa_object.f_obj_ttc_below_threshold                             = FBK_TRUE;
   p_esa_object.f_obj_decel_above_threshold                           = FBK_FALSE;
   p_esa_object.ego_side                                              = FBK_SIDE_LEFT;

   /** \action Call Esa_Set_Most_Critical_Object. */
   Esa_Set_Most_Critical_Object(p_esa_core_output, &p_esa_object, p_esa_calibration);

   /** \assert Verify that most critical object is not set . */
   EXPECT_FALSE(p_esa_core_output->esa_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(p_esa_core_output->esa_id[FBK_SIDE_LEFT], 0u);
}


/**
 * Check that most critical object is not set when the deceleration is critical but deceleration criterion is disabled.
 * \uts{CSCSA-123962} \sdd{CSCSA-65986} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test,
       Esa_Set_Most_Critical_Object__does_not_set_most_critical_object_if_object_deceleration_is_high_enough_but_criterion_is_disabled)
{
   /** \arrange Set up esa object data and esa cals. */
   Esa_Object_T p_esa_object;

   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_TRUE;
   p_esa_object.f_obj_ttc_below_threshold                             = FBK_TRUE;
   p_esa_object.f_obj_decel_above_threshold                           = FBK_TRUE;
   p_esa_object.ego_side                                              = FBK_SIDE_LEFT;
   p_esa_calibration->k_esa_f_allow_obj_selection_deceleration        = FBK_FALSE;
   p_esa_calibration->k_esa_f_allow_obj_selection_long_distance       = FBK_FALSE;
   p_esa_object.obj_decel_to_reach_host_speed = p_esa_calibration->k_esa_obj_safe_deceleration_threshold + 1.0f;

   /** \action Call Esa_Set_Most_Critical_Object. */
   Esa_Set_Most_Critical_Object(p_esa_core_output, &p_esa_object, p_esa_calibration);

   /** \assert Verify that most critical object is not set. */
   EXPECT_FALSE(p_esa_core_output->esa_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(p_esa_core_output->esa_id[FBK_SIDE_LEFT], 0u);
}


/**
 * Check that the most critical object is set using TTC and object TTC is lower than current output TTC.
 * \uts{CSCSA-123963} \sdd{CSCSA-65986} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Set_Most_Critical_Object__set_most_critical_object_using_ttc)
{
   /** \arrange Set up esa object data and esa cals. */
   Esa_Object_T p_esa_object;
   Fbk_Object_Data_T p_tracker_data;

   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_TRUE;
   p_esa_calibration->k_esa_f_allow_obj_selection_ttc                 = FBK_TRUE;
   p_esa_calibration->k_esa_f_allow_obj_selection_deceleration        = FBK_FALSE;
   p_esa_calibration->k_esa_f_allow_obj_selection_long_distance       = FBK_FALSE;
   Esa_Init_Object_Data(&p_esa_object, &p_tracker_data);
   p_esa_object.f_obj_ttc_below_threshold     = FBK_TRUE;
   p_esa_object.f_obj_decel_above_threshold   = FBK_TRUE;
   p_esa_object.ego_side                      = FBK_SIDE_LEFT;
   p_esa_object.p_tracker_data                = &p_tracker_data;
   p_tracker_data.id                          = 11u;
   p_tracker_data.index                       = 4u;
   p_esa_object.long_ttc                      = p_esa_calibration->k_esa_critical_longitudinal_ttc - 1.0f;
   p_esa_object.ttp                           = 5.0f;
   p_esa_object.obj_decel_to_reach_host_speed = p_esa_calibration->k_esa_obj_safe_deceleration_threshold + 1.0f;
   p_esa_object.obj_long_dist                 = 20.0f;

   /** \action Call Esa_Set_Most_Critical_Object. */
   Esa_Set_Most_Critical_Object(p_esa_core_output, &p_esa_object, p_esa_calibration);

   /** \assert Verify that most critical object is set. */
   EXPECT_TRUE(p_esa_core_output->esa_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(p_esa_core_output->esa_id[FBK_SIDE_LEFT], p_tracker_data.id);
}


/**
 * Check that the most critical object is not set using TTC when object TTC is higher than current output TTC.
 * \uts{CSCSA-209338} \sdd{CSCSA-65986} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Set_Most_Critical_Object__most_critical_object_not_set_using_ttc)
{
   /** \arrange Set up esa object data and esa cals. */
   Esa_Object_T p_esa_object;
   Fbk_Object_Data_T p_tracker_data;
   uint8_t expected_id = 3u;

   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_FALSE;
   p_esa_calibration->k_esa_f_allow_obj_selection_ttc                 = FBK_TRUE;
   p_esa_calibration->k_esa_f_allow_obj_selection_deceleration        = FBK_FALSE;
   p_esa_calibration->k_esa_f_allow_obj_selection_long_distance       = FBK_FALSE;
   Esa_Init_Object_Data(&p_esa_object, &p_tracker_data);
   p_esa_core_output->esa_ttc[FBK_SIDE_LEFT]   = p_esa_calibration->k_esa_critical_longitudinal_ttc - 0.1f;
   p_esa_core_output->esa_id[FBK_SIDE_LEFT]    = expected_id;
   p_esa_core_output->esa_alert[FBK_SIDE_LEFT] = FBK_FALSE;
   p_esa_object.ego_side                       = FBK_SIDE_LEFT;
   p_esa_object.p_tracker_data                 = &p_tracker_data;
   p_tracker_data.id                           = expected_id + 1u;
   p_tracker_data.index                        = 4u;
   p_esa_object.long_ttc                       = p_esa_core_output->esa_ttc[FBK_SIDE_LEFT] + 1.0f;
   p_esa_object.ttp                            = 5.0f;
   p_esa_object.obj_decel_to_reach_host_speed  = p_esa_calibration->k_esa_obj_safe_deceleration_threshold + 1.0f;
   p_esa_object.obj_long_dist                  = 20.0f;

   /** \action Call Esa_Set_Most_Critical_Object. */
   Esa_Set_Most_Critical_Object(p_esa_core_output, &p_esa_object, p_esa_calibration);

   /** \assert Verify that most critical object is set. */
   EXPECT_FALSE(p_esa_core_output->esa_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(p_esa_core_output->esa_id[FBK_SIDE_LEFT], expected_id);
}


/**
 * Check that the most critical object is set using deceleration and object deceleration is higher than current output
 * deceleration. \uts{CSCSA-123964} \sdd{CSCSA-65986} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Set_Most_Critical_Object__set_most_critical_object_using_deceleration)
{
   /** \arrange Set up esa object data and esa cals. */
   Esa_Object_T p_esa_object;
   Fbk_Object_Data_T p_tracker_data;

   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_TRUE;
   p_esa_calibration->k_esa_f_allow_obj_selection_ttc                 = FBK_FALSE;
   p_esa_calibration->k_esa_f_allow_obj_selection_deceleration        = FBK_TRUE;
   p_esa_calibration->k_esa_f_allow_obj_selection_long_distance       = FBK_FALSE;
   Esa_Init_Object_Data(&p_esa_object, &p_tracker_data);
   p_esa_object.f_obj_ttc_below_threshold     = FBK_TRUE;
   p_esa_object.f_obj_decel_above_threshold   = FBK_TRUE;
   p_esa_object.ego_side                      = FBK_SIDE_LEFT;
   p_esa_object.p_tracker_data                = &p_tracker_data;
   p_tracker_data.id                          = 11u;
   p_tracker_data.index                       = 4u;
   p_esa_object.long_ttc                      = p_esa_calibration->k_esa_critical_longitudinal_ttc - 1.0f;
   p_esa_object.ttp                           = 5.0f;
   p_esa_object.obj_decel_to_reach_host_speed = p_esa_calibration->k_esa_obj_safe_deceleration_threshold + 1.0f;
   p_esa_object.obj_long_dist                 = 20.0f;

   /** \action Call Esa_Set_Most_Critical_Object. */
   Esa_Set_Most_Critical_Object(p_esa_core_output, &p_esa_object, p_esa_calibration);

   /** \assert Verify that most critical object is set . */
   EXPECT_TRUE(p_esa_core_output->esa_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(p_esa_core_output->esa_id[FBK_SIDE_LEFT], p_tracker_data.id);
}


/**
 * Check that the most critical object is not set using deceleration when object deceleration is lower than current output
 * deceleration. \uts{CSCSA-209339} \sdd{CSCSA-65986} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Set_Most_Critical_Object__most_critical_object_not_set_using_deceleration)
{
   /** \arrange Set up esa object data and esa cals. */
   Esa_Object_T p_esa_object;
   Fbk_Object_Data_T p_tracker_data;
   uint8_t expected_id = 3u;

   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_FALSE;
   p_esa_calibration->k_esa_f_allow_obj_selection_ttc                 = FBK_FALSE;
   p_esa_calibration->k_esa_f_allow_obj_selection_deceleration        = FBK_TRUE;
   p_esa_calibration->k_esa_f_allow_obj_selection_long_distance       = FBK_FALSE;
   Esa_Init_Object_Data(&p_esa_object, &p_tracker_data);
   p_esa_core_output->esa_decel_to_reach_host_speed[FBK_SIDE_LEFT] = p_esa_calibration->k_esa_obj_safe_deceleration_threshold + 0.1f;
   p_esa_core_output->esa_id[FBK_SIDE_LEFT]                        = expected_id;
   p_esa_core_output->esa_alert[FBK_SIDE_LEFT]                     = FBK_FALSE;
   p_esa_object.ego_side                                           = FBK_SIDE_LEFT;
   p_esa_object.p_tracker_data                                     = &p_tracker_data;
   p_tracker_data.id                                               = expected_id + 1u;
   p_tracker_data.index                                            = 4u;
   p_esa_object.long_ttc                                           = p_esa_calibration->k_esa_critical_longitudinal_ttc - 1.0f;
   p_esa_object.ttp                                                = 5.0f;
   p_esa_object.obj_decel_to_reach_host_speed = p_esa_core_output->esa_decel_to_reach_host_speed[FBK_SIDE_LEFT] - 1.0f;
   p_esa_object.obj_long_dist                 = 20.0f;


   /** \action Call Esa_Set_Most_Critical_Object. */
   Esa_Set_Most_Critical_Object(p_esa_core_output, &p_esa_object, p_esa_calibration);

   /** \assert Verify that most critical object is not set. */
   EXPECT_FALSE(p_esa_core_output->esa_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(p_esa_core_output->esa_id[FBK_SIDE_LEFT], expected_id);
}


/**
 * Check that the most critical object is set using longitudinal distance and object long. dist. is lower than current output long.
 * dist. \uts{CSCSA-123965} \sdd{CSCSA-65986} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Set_Most_Critical_Object__set_most_critical_object_using_distance)
{
   /** \arrange Set up esa object data and esa cals. */
   Esa_Object_T p_esa_object;
   Fbk_Object_Data_T p_tracker_data;

   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_TRUE;
   p_esa_calibration->k_esa_f_allow_obj_selection_ttc                 = FBK_FALSE;
   p_esa_calibration->k_esa_f_allow_obj_selection_deceleration        = FBK_FALSE;
   p_esa_calibration->k_esa_f_allow_obj_selection_long_distance       = FBK_TRUE;
   Esa_Init_Object_Data(&p_esa_object, &p_tracker_data);
   p_esa_object.f_obj_ttc_below_threshold     = FBK_TRUE;
   p_esa_object.f_obj_decel_above_threshold   = FBK_TRUE;
   p_esa_object.ego_side                      = FBK_SIDE_LEFT;
   p_esa_object.p_tracker_data                = &p_tracker_data;
   p_tracker_data.id                          = 11u;
   p_tracker_data.index                       = 4u;
   p_esa_object.long_ttc                      = p_esa_calibration->k_esa_critical_longitudinal_ttc - 1.0f;
   p_esa_object.ttp                           = 5.0f;
   p_esa_object.obj_decel_to_reach_host_speed = p_esa_calibration->k_esa_obj_safe_deceleration_threshold + 1.0f;
   p_esa_object.obj_long_dist                 = 20.0f;

   /** \action Call Esa_Set_Most_Critical_Object. */
   Esa_Set_Most_Critical_Object(p_esa_core_output, &p_esa_object, p_esa_calibration);

   /** \assert Verify that most critical object is set. */
   EXPECT_TRUE(p_esa_core_output->esa_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(p_esa_core_output->esa_id[FBK_SIDE_LEFT], p_tracker_data.id);
}


/**
 * Check that the most critical object is not set using longitudinal distance when object long. dist. is higher than current output
 * long. dist. \uts{CSCSA-209340} \sdd{CSCSA-65986} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Set_Most_Critical_Object__most_critical_object_not_set_using_distance)
{
   /** \arrange Set up esa object data and esa cals. */
   Esa_Object_T p_esa_object;
   Fbk_Object_Data_T p_tracker_data;
   uint8_t expected_id = 3u;

   p_esa_calibration->k_esa_f_allow_obj_critical_ttc_and_deceleration = FBK_FALSE;
   p_esa_calibration->k_esa_f_allow_obj_selection_ttc                 = FBK_FALSE;
   p_esa_calibration->k_esa_f_allow_obj_selection_deceleration        = FBK_FALSE;
   p_esa_calibration->k_esa_f_allow_obj_selection_long_distance       = FBK_TRUE;
   p_esa_core_output->esa_long_distance[FBK_SIDE_LEFT]                = 10.0f;
   p_esa_core_output->esa_id[FBK_SIDE_LEFT]                           = expected_id;
   p_esa_core_output->esa_alert[FBK_SIDE_LEFT]                        = FBK_FALSE;
   Esa_Init_Object_Data(&p_esa_object, &p_tracker_data);
   p_esa_object.ego_side                      = FBK_SIDE_LEFT;
   p_esa_object.p_tracker_data                = &p_tracker_data;
   p_tracker_data.id                          = expected_id + 1u;
   p_tracker_data.index                       = 4u;
   p_esa_object.long_ttc                      = p_esa_calibration->k_esa_critical_longitudinal_ttc - 1.0f;
   p_esa_object.ttp                           = 5.0f;
   p_esa_object.obj_decel_to_reach_host_speed = p_esa_calibration->k_esa_obj_safe_deceleration_threshold + 1.0f;
   p_esa_object.obj_long_dist                 = 20.0f;

   /** \action Call Esa_Set_Most_Critical_Object. */
   Esa_Set_Most_Critical_Object(p_esa_core_output, &p_esa_object, p_esa_calibration);

   /** \assert Verify that most critical object is set. */
   EXPECT_FALSE(p_esa_core_output->esa_alert[FBK_SIDE_LEFT]);
   EXPECT_EQ(p_esa_core_output->esa_id[FBK_SIDE_LEFT], expected_id);
}


/**
 * Check that the object is considered relevant if it's mature.
 * \uts{CSCSA-123966} \sdd{CSCSA-65977} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Object_Relevant__calculates_if_object_is_relevant_when_object_is_mature)
{
   /** \arrange Set up tracker object data. */
   Fbk_Object_Data_T p_tracker_data;
   boolean_T f_is_relevant;

   p_tracker_data.status                = PA_OBJ_STATUS_MATURE;
   p_tracker_data.curvi_heading         = 0.0f;
   p_tracker_data.curvi_vel.x           = p_esa_calibration->k_esa_min_obj_curvi_long_vel_abs + 1.0f;
   p_tracker_data.existence_probability = 1.0f;

   /** \action Call Esa_Is_Ttc_Below_Threshold. */
   f_is_relevant = Esa_Is_Object_Relevant(&p_tracker_data, p_esa_calibration);

   /** \assert Verify that object is relevant. */
   EXPECT_TRUE(f_is_relevant);
}


/**
 * Check that the object is considered relevant if it's coasted.
 * \uts{CSCSA-123967} \sdd{CSCSA-65977} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Object_Relevant__calculates_if_object_is_relevant_when_object_is_coasted)
{
   /** \arrange Set up tracker object data. */
   Fbk_Object_Data_T p_tracker_data;
   boolean_T f_is_relevant;

   p_tracker_data.status                = PA_OBJ_STATUS_COASTED;
   p_tracker_data.curvi_heading         = 0.0f;
   p_tracker_data.curvi_vel.x           = p_esa_calibration->k_esa_min_obj_curvi_long_vel_abs + 1.0f;
   p_tracker_data.existence_probability = 1.0f;

   /** \action Call Esa_Is_Ttc_Below_Threshold. */
   f_is_relevant = Esa_Is_Object_Relevant(&p_tracker_data, p_esa_calibration);

   /** \assert Verify that object is relevant. */
   EXPECT_TRUE(f_is_relevant);
}


/**
 * Check that the object is considered not relevant if it's invalid.
 * \uts{CSCSA-123968} \sdd{CSCSA-65977} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Object_Relevant__calculates_if_object_is_not_relevant_when_object_is_invalid)
{
   /** \arrange Set up tracker object data. */
   Fbk_Object_Data_T p_tracker_data;
   boolean_T f_is_relevant;

   p_tracker_data.status                = PA_OBJ_STATUS_INVALID;
   p_tracker_data.curvi_heading         = 0.0f;
   p_tracker_data.curvi_vel.x           = p_esa_calibration->k_esa_min_obj_curvi_long_vel_abs + 1.0f;
   p_tracker_data.existence_probability = 1.0f;

   /** \action Call Esa_Is_Ttc_Below_Threshold. */
   f_is_relevant = Esa_Is_Object_Relevant(&p_tracker_data, p_esa_calibration);

   /** \assert Verify that object is not relevant. */
   EXPECT_FALSE(f_is_relevant);
}


/**
 * Check that the object is considered not relevant if its heading is above the threshold.
 * \uts{CSCSA-123969} \sdd{CSCSA-65977} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Object_Relevant__calculates_if_object_is_not_relevant_when_object_heading_is_too_high)
{
   /** \arrange Set up tracker object data. */
   Fbk_Object_Data_T p_tracker_data;
   boolean_T f_is_relevant;

   p_tracker_data.status                = PA_OBJ_STATUS_MATURE;
   p_tracker_data.curvi_heading         = p_esa_calibration->k_esa_max_curvi_heading_abs + 0.1f;
   p_tracker_data.curvi_vel.x           = p_esa_calibration->k_esa_min_obj_curvi_long_vel_abs + 1.0f;
   p_tracker_data.existence_probability = 1.0f;

   /** \action Call Esa_Is_Ttc_Below_Threshold. */
   f_is_relevant = Esa_Is_Object_Relevant(&p_tracker_data, p_esa_calibration);

   /** \assert Verify that object is not relevant. */
   EXPECT_FALSE(f_is_relevant);
}


/**
 * Check that the object is considered not relevant if its heading is below the threshold.
 * \uts{CSCSA-123970} \sdd{CSCSA-65977} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Object_Relevant__calculates_if_object_is_not_relevant_when_object_heading_is_too_small)
{
   /** \arrange Set up tracker object data. */
   Fbk_Object_Data_T p_tracker_data;
   boolean_T f_is_relevant;

   p_tracker_data.status                = PA_OBJ_STATUS_MATURE;
   p_tracker_data.curvi_heading         = -(p_esa_calibration->k_esa_max_curvi_heading_abs + 0.1f);
   p_tracker_data.curvi_vel.x           = p_esa_calibration->k_esa_min_obj_curvi_long_vel_abs + 1.0f;
   p_tracker_data.existence_probability = 1.0f;

   /** \action Call Esa_Is_Ttc_Below_Threshold. */
   f_is_relevant = Esa_Is_Object_Relevant(&p_tracker_data, p_esa_calibration);

   /** \assert Verify that object is not relevant. */
   EXPECT_FALSE(f_is_relevant);
}


/**
 * Check that the object is considered not relevant if its velocity is below the threshold.
 * \uts{CSCSA-123971} \sdd{CSCSA-65977} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Object_Relevant__calculates_if_object_is_not_relevant_when_object_velocity_is_too_small)
{
   /** \arrange Set up tracker object data. */
   Fbk_Object_Data_T p_tracker_data;
   boolean_T f_is_relevant;

   p_tracker_data.status                = PA_OBJ_STATUS_MATURE;
   p_tracker_data.curvi_heading         = 0.0f;
   p_tracker_data.curvi_vel.x           = p_esa_calibration->k_esa_min_obj_curvi_long_vel_abs - 0.1f;
   p_tracker_data.existence_probability = 1.0f;

   /** \action Call Esa_Is_Ttc_Below_Threshold. */
   f_is_relevant = Esa_Is_Object_Relevant(&p_tracker_data, p_esa_calibration);

   /** \assert Verify that object is not relevant. */
   EXPECT_FALSE(f_is_relevant);
}


/**
 * Check that the object is considered not relevant if its velocity is above the threshold.
 * \uts{CSCSA-123972} \sdd{CSCSA-65977} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Object_Relevant__calculates_if_object_is_not_relevant_when_negative_object_velocity_is_too_high)
{
   /** \arrange Set up tracker object data. */
   Fbk_Object_Data_T p_tracker_data;
   boolean_T f_is_relevant;

   p_tracker_data.status                = PA_OBJ_STATUS_MATURE;
   p_tracker_data.curvi_heading         = 0.0f;
   p_tracker_data.curvi_vel.x           = -(p_esa_calibration->k_esa_min_obj_curvi_long_vel_abs - 0.1f);
   p_tracker_data.existence_probability = 1.0f;

   /** \action Call Esa_Is_Ttc_Below_Threshold. */
   f_is_relevant = Esa_Is_Object_Relevant(&p_tracker_data, p_esa_calibration);

   /** \assert Verify that object is not relevant. */
   EXPECT_FALSE(f_is_relevant);
}


/**
 * Check that the object is considered not relevant if its existence probability is below the threshold.
 * \uts{CSCSA-123973} \sdd{CSCSA-65977} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Test, Esa_Is_Object_Relevant__calculates_if_object_is_relevant_when_object_existence_probability_is_too_small)
{
   /** \arrange Set up tracker object data. */
   Fbk_Object_Data_T p_tracker_data;
   boolean_T f_is_relevant;

   p_tracker_data.status                = PA_OBJ_STATUS_MATURE;
   p_tracker_data.curvi_heading         = 0.0f;
   p_tracker_data.curvi_vel.x           = p_esa_calibration->k_esa_min_obj_curvi_long_vel_abs + 1.0f;
   p_tracker_data.existence_probability = 0.0f;

   /** \action Call Esa_Is_Ttc_Below_Threshold. */
   f_is_relevant = Esa_Is_Object_Relevant(&p_tracker_data, p_esa_calibration);

   /** \assert Verify that bject is not relevant. */
   EXPECT_FALSE(f_is_relevant);
}
