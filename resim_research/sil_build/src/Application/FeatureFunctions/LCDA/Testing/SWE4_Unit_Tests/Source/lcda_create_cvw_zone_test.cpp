/**
 * @file lcda_create_cvw_zone_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for lcda_create_cvw_zone.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42607}
 */

#include "lcda_create_cvw_zone_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "lcda_create_cvw_zone.c"
#include "lcda_process_cvw.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
}


/**
 * Create CVW initial zone based on vehicle length and lane width. Set vehicle width to zero and check that a minimal vehicle width
 * as defined by calibration parameters is used to create the zone. \uts{CSCSA-42608} \sdd{SF-6603}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test,
       Lcda_Get_Lw_Based_Initial_Cvw_Zones__zone_point0_y_is_multiple_of_k_min_lane_width_for_lane_width_smaller_than_range)
{
   /** \arrange Set up zone parameters for zone creation. */
   lcda_core_input.lane_width         = 0.0f;
   lcda_core_input.lane_center_offset = 0.0f;

   Fbk_Field_Of_Interest_T cvw_zone{};
   Fbk_Field_Of_Interest_T cvw_zone_hys{};

   cvw_object.ego_side  = FBK_SIDE_RIGHT;
   tracker_object.index = 0u;

   /** \action Call Lcda_Get_Lw_Based_Initial_Cvw_Zones to create zone. */
   Lcda_Get_Lw_Based_Initial_Cvw_Zones(&cvw_zone, &cvw_zone_hys, f_use_small_lc_intention_zone, &cvw_object, &lcda_core_input,
                                       &lcda_cals);

   /** \assert Check for representative coordinate that that it was computed correctly. */
   EXPECT_FLOAT_EQ(cvw_zone.points[0].y, lcda_cals.k_lcda_min_lane_width * lcda_cals.k_cvw_zone_y[0]);
}

/**
 * Create CVW initial zone based on vehicle length and lane width. Set lane width too large and check that a maximal lane width as
 * defined by calibration parameters is used to create the zone. \uts{CSCSA-42609} \sdd{SF-6603} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test, Lcda_Get_Lw_Based_Initial_Cvw_Zones__outer_zone_points_are_limited_to_k_lcda_max_lane_width)
{
   /** \arrange Set up zone parameters for zone creation. */
   lcda_core_input.lane_width         = lcda_cals.k_lcda_max_lane_width * 2.0f;
   lcda_core_input.lane_center_offset = 0.0f;

   Fbk_Field_Of_Interest_T cvw_zone{};
   Fbk_Field_Of_Interest_T cvw_zone_hys{};

   cvw_object.ego_side  = FBK_SIDE_RIGHT;
   tracker_object.index = 0u;

   /** \action Call Lcda_Get_Lw_Based_Initial_Cvw_Zones to create zone. */
   Lcda_Get_Lw_Based_Initial_Cvw_Zones(&cvw_zone, &cvw_zone_hys, f_use_small_lc_intention_zone, &cvw_object, &lcda_core_input,
                                       &lcda_cals);

   /** \assert Check for representative coordinate that that it was computed correctly. */
   EXPECT_FLOAT_EQ(cvw_zone.points[FRONT_OUTER_SIDE].y, (Fbk_Half(lcda_core_input.lane_width) + lcda_cals.k_lcda_max_lane_width));
   EXPECT_FLOAT_EQ(cvw_zone.points[MIDDLE_OUTER_SIDE].y, (Fbk_Half(lcda_core_input.lane_width) + lcda_cals.k_lcda_max_lane_width));
   EXPECT_FLOAT_EQ(cvw_zone.points[REAR_OUTER_SIDE].y, (Fbk_Half(lcda_core_input.lane_width) + lcda_cals.k_lcda_max_lane_width));
}

/**
 * Create CVW initial zone based on vehicle length and lane width. Set vehicle width too large and create zone with/without object
 * width correction. With the correction deactivated, the zone should be larger. \uts{CSCSA-42610} \sdd{SF-6603}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test,
       Lcda_Get_Lw_Based_Initial_Cvw_Zones__zone_point0_y_is_multiple_of_k_max_lane_width_for_lane_width_larger_than_range2)
{
   /** \arrange Set up zone parameters for zone creation. */
   lcda_core_input.lane_width         = lcda_cals.k_lcda_max_lane_width + 2.0f;
   lcda_core_input.lane_center_offset = 0.0f;

   Fbk_Field_Of_Interest_T cvw_zone{};
   Fbk_Field_Of_Interest_T cvw_zone_hys{};

   cvw_object.ego_side  = FBK_SIDE_RIGHT;
   tracker_object.index = 0u;

   /** \action Call Lcda_Get_Lw_Based_Initial_Cvw_Zones to create zone twice, with and without object width correction,
    * respectively. */

   lcda_cals.k_zone_hys_obj_width_correction = 1.0f;
   Lcda_Get_Lw_Based_Initial_Cvw_Zones(&cvw_zone, &cvw_zone_hys, f_use_small_lc_intention_zone, &cvw_object, &lcda_core_input,
                                       &lcda_cals);
   double zone_hys_y_correction_on = cvw_zone_hys.points[0].y;

   lcda_cals.k_zone_hys_obj_width_correction = 0.0f;
   Lcda_Get_Lw_Based_Initial_Cvw_Zones(&cvw_zone, &cvw_zone_hys, f_use_small_lc_intention_zone, &cvw_object, &lcda_core_input,
                                       &lcda_cals);
   double zone_hys_y_correction_off = cvw_zone_hys.points[0].y;

   /** \assert Check for representative coordinate that that it was computed correctly with/without object width correction.
    * Without correction, the coordinate should be larger. */
   EXPECT_GE(zone_hys_y_correction_off, zone_hys_y_correction_on);
}

/**
 * Set host vehicle status sucht that it drives a curve. Create parameters for curve zone adaption and check that the corresponding
 * factor is reduced due to the curve driven for the left side. \uts{CSCSA-42611} \sdd{SF-6601} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test, Lcda_Curve_Zone_Adaptation__adapts_cvw_zone_length_side_left)
{
   /** \arrange Set up parameters for curve zone adaption. */
   uint8_t object_side                  = FBK_SIDE_LEFT;
   float32_T curve_zone_factor_original = 1.0f;

   p_vehicle_data->yawrate  = 0.5f;
   p_vehicle_data->long_vel = 10.0f;

   lcda_cals.k_lcda_curve_radius_threshold_for_zone_adaptation = 100.0f;
   lcda_cals.k_cvw_curve_zone_factor_outer                     = 0.5f;
   lcda_cals.k_cvw_curve_zone_factor_inner                     = 0.5f;
   lcda_cals.k_lcda_distance_traveled_scale_factor             = 0.6f;

   Lcda_Store_Pers_Cvw_Curve_Zone_Factor(&cvw_persistent, curve_zone_factor_original, object_side);

   /** \action Call Lcda_Curve_Zone_Adaptation to compute the curve zone factor. */
   Lcda_Curve_Zone_Adaptation(&lcda_cals, &lcda_core_input, p_vehicle_data, object_side, &cvw_persistent);
   float32_T curve_zone_factor = Lcda_Get_Pers_Prev_Cvw_Curve_Zone_Factor(object_side, &cvw_persistent);

   /** \assert Check that curve zone factor was decreased. */
   EXPECT_TRUE(curve_zone_factor_original > curve_zone_factor);
}

/**
 * Set host vehicle status sucht that it drives a curve. Create parameters for curve zone adaption and check that the corresponding
 * factor is reduced due to the curve driven for the right side. \uts{CSCSA-42612} \sdd{SF-6601} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test, Lcda_Curve_Zone_Adaptation__adapts_cvw_zone_length_side_right)
{
   /** \arrange Set up parameters for curve zone adaption. */
   uint8_t object_side                  = FBK_SIDE_RIGHT;
   float32_T curve_zone_factor_original = 1.0f;

   p_vehicle_data->yawrate  = -0.5f;
   p_vehicle_data->long_vel = 10.0f;

   lcda_cals.k_lcda_curve_radius_threshold_for_zone_adaptation = 100.0f;
   lcda_cals.k_cvw_curve_zone_factor_outer                     = 0.5f;
   lcda_cals.k_cvw_curve_zone_factor_inner                     = 0.5f;
   lcda_cals.k_lcda_distance_traveled_scale_factor             = 0.6f;
   lcda_cals.k_cvw_zone_x[1]                                   = 1.0f;

   Lcda_Store_Pers_Cvw_Curve_Zone_Factor(&cvw_persistent, curve_zone_factor_original, object_side);

   /** \action Call Lcda_Curve_Zone_Adaptation to compute the curve zone factor. */
   Lcda_Curve_Zone_Adaptation(&lcda_cals, &lcda_core_input, p_vehicle_data, object_side, &cvw_persistent);
   float32_T curve_zone_factor = Lcda_Get_Pers_Prev_Cvw_Curve_Zone_Factor(object_side, &cvw_persistent);

   /** \assert Check that curve zone factor was decreased. */
   EXPECT_TRUE(curve_zone_factor_original > curve_zone_factor);
}

/**
 * Set host vehicle status sucht that it drives a curve. Create parameters for curve zone adaption and check that CVW zone is
 * shortened correctly. \uts{CSCSA-42613} \sdd{SF-6600} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test, Lcda_Apply_Curve_Zone_Adaptation__applies_cvw_zone_adaption)
{
   /** \arrange Set up parameters for curve zone adaption. */
   uint8_t object_index = 0;
   uint8_t object_side  = FBK_SIDE_LEFT;

   Fbk_Field_Of_Interest_T original_cvw_zone{};
   Fbk_Field_Of_Interest_T original_cvw_zone_hys{};

   cvw_object.ego_side  = object_side;
   tracker_object.index = object_index;

   lcda_cals.k_cvw_zone_calculation_mode = CVW_ZONE_CALC_VL_LW;

   Lcda_Get_Initial_Cvw_Zones(&original_cvw_zone, &original_cvw_zone_hys, f_use_small_lc_intention_zone, &cvw_object,
                              &lcda_core_input, &lcda_cals);

   Fbk_Field_Of_Interest_T shortened_cvw_zone = original_cvw_zone;

   Lcda_Store_Pers_Cvw_Curve_Zone_Factor(&cvw_persistent, 0.5f, object_side);

   /** \action Call Lcda_Apply_Curve_Zone_Adaptation to modify the CVW zone. */
   Lcda_Apply_Curve_Zone_Adaptation(&cvw_persistent, &shortened_cvw_zone, object_side);

   /** \assert Check that the CVW zone was shortened as expected. */
   EXPECT_TRUE(shortened_cvw_zone.points[REAR_EGO_SIDE].x > original_cvw_zone.points[REAR_EGO_SIDE].x);
   EXPECT_TRUE(shortened_cvw_zone.points[REAR_OUTER_SIDE].x > original_cvw_zone.points[REAR_OUTER_SIDE].x);
}


/**
 * Check that in case of missing lane change intention the default zone properties are used.
 * \uts{CSCSA-42614} \sdd{SF-6812} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test, Lcda_Return_Cvw_Zone_Properties__check_that_default_zone_parameters_are_used_when_flag_equals_false)
{
   /** \arrange set up lane change intention mode to false */
   Lcda_Cvw_Zone_Properties_T res;
   f_use_small_lc_intention_zone[FBK_SIDE_LEFT]                       = FBK_FALSE;
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_FALSE;

   /** \action Call function to test */
   res = Lcda_Return_Cvw_Zone_Properties(f_use_small_lc_intention_zone[FBK_SIDE_LEFT], &lcda_core_input, &lcda_cals);

   /** \assert Check that standard zones are used. */
   EXPECT_EQ(&lcda_cals.k_cvw_zone_x[0], res.p_x_points);
   EXPECT_EQ(&lcda_cals.k_cvw_zone_y[0], res.p_y_points);
   EXPECT_EQ(&lcda_cals.k_cvw_zone_y_hys[0], res.p_y_hys);
}


/**
 * Check that in case of lane change intention the normal lane change intention zone properties are used, if small zone flag is
 * false. \uts{CSCSA-42615} \sdd{SF-6812} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test,
       Lcda_Return_Cvw_Zone_Properties__check_that_normal_lane_change_intention_zone_parameters_are_used_when_lc_flag_true_and_small_zone_flag_false)
{
   /** \arrange set up lane change intention mode to true and use small zone flag to false */
   Lcda_Cvw_Zone_Properties_T res;
   f_use_small_lc_intention_zone[FBK_SIDE_LEFT]                       = FBK_FALSE;
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_TRUE;

   /** \action Call function to test */
   res = Lcda_Return_Cvw_Zone_Properties(f_use_small_lc_intention_zone[FBK_SIDE_LEFT], &lcda_core_input, &lcda_cals);

   /** \assert Check that lane change zone properties are used. */
   EXPECT_EQ(&lcda_cals.k_cvw_lane_change_intention_zone_x[0], res.p_x_points);
   EXPECT_EQ(&lcda_cals.k_cvw_lane_change_intention_zone_y[0], res.p_y_points);
   EXPECT_EQ(&lcda_cals.k_cvw_lane_change_intention_zone_hys_y[0], res.p_y_hys);
}

/**
 * Check that in case of lane change intention the small lane change intention zone properties are used, if small zone flag is
 * true. \uts{CSCSA-42616} \sdd{SF-6812} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test,
       Lcda_Return_Cvw_Zone_Properties__check_that_small_lane_change_intention_zone_parameters_are_used_when_lc_flag_true_and_small_zone_flag_true)
{
   /** \arrange set up lane change intention mode to true and use small zone flag to true */
   Lcda_Cvw_Zone_Properties_T res;
   f_use_small_lc_intention_zone[FBK_SIDE_LEFT]                       = FBK_TRUE;
   lcda_core_input.warn_settings.f_use_cvw_lane_change_intention_zone = FBK_TRUE;

   /** \action Call function to test */
   res = Lcda_Return_Cvw_Zone_Properties(f_use_small_lc_intention_zone[FBK_SIDE_LEFT], &lcda_core_input, &lcda_cals);

   /** \assert Check that lane change zone properties are used. */
   EXPECT_EQ(&lcda_cals.k_cvw_lane_change_intention_zone_small_x[0], res.p_x_points);
   EXPECT_EQ(&lcda_cals.k_cvw_lane_change_intention_zone_small_y[0], res.p_y_points);
   EXPECT_EQ(&lcda_cals.k_cvw_lane_change_intention_zone_hys_y[0], res.p_y_hys);
}

/**
 * The left and right CVW zones should be symmetric along the X-axis. Create CVW zones and check that the Y-coordinates are
 * mirrored. \uts{CSCSA-42617} \sdd{SF-6606} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test, Lcda_Create_Cvw_Zone__left_zone_and_right_zone_are_mirrored_along_long_axis)
{
   /** \arrange Set parameters for CVW zone creation. */
   Fbk_Field_Of_Interest_T zone_left{};
   Fbk_Field_Of_Interest_T zone_hys_left{};

   Fbk_Field_Of_Interest_T zone_right{};
   Fbk_Field_Of_Interest_T zone_hys_right{};

   lcda_cals.k_enable_cvw_curve_zone_adaptation = 0u;
   lcda_cals.k_cvw_zone_calculation_mode        = CVW_ZONE_CALC_VL_LW;

   /** \action Call Lcda_Create_Cvw_Zone to create the left and right CVW zones, respectively. */
   cvw_object.ego_side = FBK_SIDE_RIGHT;
   Lcda_Create_Cvw_Zone(&zone_right, &zone_hys_right, f_use_small_lc_intention_zone, &cvw_object, &lcda_core_input, p_vehicle_data,
                        &lcda_cals, &cvw_persistent);
   cvw_object.ego_side = FBK_SIDE_LEFT;
   Lcda_Create_Cvw_Zone(&zone_left, &zone_hys_left, f_use_small_lc_intention_zone, &cvw_object, &lcda_core_input, p_vehicle_data,
                        &lcda_cals, &cvw_persistent);

   /** \assert Check that zones are mirrored along the X-axis. */
   for (uint8_t ipoint = 0; ipoint < LCDA_NUMBER_OF_ZONE_POINTS; ipoint++)
   {
      EXPECT_FLOAT_EQ(zone_right.points[ipoint].x, zone_left.points[ipoint].x);
      EXPECT_FLOAT_EQ(zone_right.points[ipoint].y, -zone_left.points[ipoint].y);

      EXPECT_FLOAT_EQ(zone_hys_right.points[ipoint].x, zone_hys_left.points[ipoint].x);
      EXPECT_FLOAT_EQ(zone_hys_right.points[ipoint].y, -zone_hys_left.points[ipoint].y);
   }
}

/**
 * The left and right CVW zones should be symmetric along the X-axis. Set varying curve zone adaption factor for left and right
 * side. Since the right curve zone factor is smaller, the zone coordinate should be greater (zone points are closer to the ego
 * vehicle). \uts{CSCSA-42618} \sdd{SF-6606} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test, Lcda_Create_Cvw_Zone__left_zone_and_right_zone_are_mirrored_along_long_axis2)
{
   /** \arrange Set parameters for CVW zone creation, curve zone adaptions (right side has smaller curve zone factor), and activate
    * curve zone adaption. */
   Fbk_Field_Of_Interest_T zone_left{};
   Fbk_Field_Of_Interest_T zone_hys_left{};
   Fbk_Field_Of_Interest_T zone_right{};
   Fbk_Field_Of_Interest_T zone_hys_right{};

   lcda_cals.k_enable_cvw_curve_zone_adaptation = 1u;
   lcda_cals.k_cvw_zone_calculation_mode        = CVW_ZONE_CALC_VL_LW;

   Lcda_Store_Pers_Cvw_Curve_Zone_Factor(&cvw_persistent, 0.2f, FBK_SIDE_RIGHT);
   Lcda_Store_Pers_Cvw_Curve_Zone_Factor(&cvw_persistent, 1.0f, FBK_SIDE_LEFT);

   /** \action Call Lcda_Create_Cvw_Zone to create the left and right CVW zones, respectively. */
   cvw_object.ego_side = FBK_SIDE_RIGHT;
   Lcda_Create_Cvw_Zone(&zone_right, &zone_hys_right, f_use_small_lc_intention_zone, &cvw_object, &lcda_core_input, p_vehicle_data,
                        &lcda_cals, &cvw_persistent);
   cvw_object.ego_side = FBK_SIDE_LEFT;
   Lcda_Create_Cvw_Zone(&zone_left, &zone_hys_left, f_use_small_lc_intention_zone, &cvw_object, &lcda_core_input, p_vehicle_data,
                        &lcda_cals, &cvw_persistent);

   /** \assert Check that the longitudinal coordinates of the right side are greater than or equal than the coordinates on the left
    * side. */
   for (uint8_t ipoint = 0; ipoint < LCDA_NUMBER_OF_ZONE_POINTS; ipoint++)
   {
      EXPECT_GE(zone_right.points[ipoint].x, zone_left.points[ipoint].x);
      EXPECT_GE(zone_hys_right.points[ipoint].x, zone_hys_left.points[ipoint].x);
   }
}

/**
 * Check that the initial CVW zones are taken from the core input if calculation method CVW_ZONE_CALC_FIXED_INPUT is chosen.
 * \uts{CSCSA-42619} \sdd{SF-6602} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test,
       Lcda_Get_Initial_Cvw_Zones__uses_zones_from_core_input_for_calculation_method_cvw_zone_calc_fixed_input)
{
   /** \arrange Set up zone variables to be filled and some values of core input zones. Select zone calculation method
    * CVW_ZONE_CALC_FIXED_INPUT */
   Fbk_Field_Of_Interest_T zone{};
   Fbk_Field_Of_Interest_T zone_hys{};
   lcda_core_input.initial_cvw_zone.size                           = LCDA_NUMBER_OF_ZONE_POINTS;
   lcda_core_input.initial_cvw_zone.points[REAR_EGO_SIDE].x        = 3.0f;
   lcda_core_input.initial_cvw_zone.points[REAR_OUTER_SIDE].y      = 4.0f;
   lcda_core_input.initial_cvw_zone_hys.size                       = LCDA_NUMBER_OF_ZONE_POINTS;
   lcda_core_input.initial_cvw_zone_hys.points[FRONT_OUTER_SIDE].x = 5.0f;
   lcda_core_input.initial_cvw_zone_hys.points[MIDDLE_EGO_SIDE].y  = 6.0f;

   lcda_cals.k_enable_cvw_curve_zone_adaptation = 0u;
   lcda_cals.k_cvw_zone_calculation_mode        = CVW_ZONE_CALC_FIXED_INPUT;

   cvw_object.ego_side = FBK_SIDE_RIGHT;

   /** \action Call Lcda_Get_Initial_Cvw_Zones to create zones based on chosen calculation method. */
   Lcda_Get_Initial_Cvw_Zones(&zone, &zone_hys, f_use_small_lc_intention_zone, &cvw_object, &lcda_core_input, &lcda_cals);

   /** \assert Check that zones are taken from core input by checking the explicitly defined values. */
   EXPECT_FLOAT_EQ(zone.points[REAR_EGO_SIDE].x, lcda_core_input.initial_cvw_zone.points[REAR_EGO_SIDE].x);
   EXPECT_FLOAT_EQ(zone.points[REAR_OUTER_SIDE].y, lcda_core_input.initial_cvw_zone.points[REAR_OUTER_SIDE].y);
   EXPECT_FLOAT_EQ(zone_hys.points[FRONT_OUTER_SIDE].x, lcda_core_input.initial_cvw_zone_hys.points[FRONT_OUTER_SIDE].x);
   EXPECT_FLOAT_EQ(zone_hys.points[MIDDLE_EGO_SIDE].y, lcda_core_input.initial_cvw_zone_hys.points[MIDDLE_EGO_SIDE].y);
}

/**
 * Set host vehicle status such that it drives a curve. Create parameters for curve zone adaption and check that the corresponding
 * factor is reduced due to the curve driven for the left side. \uts{CSCSA-42620} \sdd{SF-6951} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test, Lcda_Compute_Curve_Zone_Factor__adapts_cvw_zone_length_left_side)
{
   /** \arrange Set up parameters for curve zone adaption. */
   uint8_t object_side         = FBK_SIDE_LEFT;
   float32_T curve_zone_factor = 1.0f;

   p_vehicle_data->yawrate = 0.5f;

   lcda_cals.k_lcda_curve_radius_threshold_for_zone_adaptation = 100.0f;
   lcda_cals.k_cvw_curve_zone_factor_outer                     = 0.5f;
   lcda_cals.k_cvw_curve_zone_factor_inner                     = 0.5f;
   lcda_cals.k_lcda_distance_traveled_scale_factor             = 0.6f;

   /** \action Call Lcda_Compute_Curve_Zone_Factor to compute the curve zone factor. */
   Lcda_Compute_Curve_Zone_Factor(&curve_zone_factor, &lcda_cals, p_vehicle_data, object_side, FBK_FALSE,
                                  lcda_cals.k_lcda_curve_radius_threshold_for_zone_adaptation - EPSILON);

   /** \assert Check that curve zone factor was decreased. */
   EXPECT_FLOAT_EQ(lcda_cals.k_cvw_curve_zone_factor_outer, curve_zone_factor);
}

/**
 * Set an active alert on the side, the curve zone factor is constant despite the vehicle's status indicating it drives a curve.
 * \uts{CSCSA-185765} \sdd{SF-6951} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test, Lcda_Compute_Curve_Zone_Factor__disabled_for_active_alert_on_side)
{
   /** \arrange Set up parameters for curve zone adaption. */
   uint8_t object_side         = FBK_SIDE_RIGHT;
   float32_T curve_zone_factor = 0.0f;
   float32_T res               = curve_zone_factor;

   p_vehicle_data->yawrate = 0.5f;

   lcda_cals.k_lcda_curve_radius_threshold_for_zone_adaptation = 100.0f;
   lcda_cals.k_cvw_curve_zone_factor_outer                     = 0.5f;
   lcda_cals.k_cvw_curve_zone_factor_inner                     = 0.5f;
   lcda_cals.k_lcda_distance_traveled_scale_factor             = 0.6f;

   /** \action Call Lcda_Compute_Curve_Zone_Factor to compute the curve zone factor. */
   Lcda_Compute_Curve_Zone_Factor(&curve_zone_factor, &lcda_cals, p_vehicle_data, object_side, FBK_TRUE,
                                  lcda_cals.k_lcda_curve_radius_threshold_for_zone_adaptation - EPSILON);

   /** \assert Check that curve zone factor was decreased. */
   EXPECT_FLOAT_EQ(curve_zone_factor, res);
}


/**
 * Check if the incorrectly set curve value as negative changes to an absolute value.. Set host vehicle status such that it drives
 * a curve. Create parameters for curve zone adaption and check that the corresponding factor is reduced. \uts{CSCSA-185766}
 * \sdd{SF-6951} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test, Lcda_Compute_Curve_Zone_Factor__adapts_cvw_zone_length_when_curve_value_is_negative)
{
   /** \arrange Set up parameters for curve zone adaption. */
   uint8_t object_side         = FBK_SIDE_RIGHT;
   float32_T curve_zone_factor = -1.0f;

   p_vehicle_data->yawrate = -0.5f;

   lcda_cals.k_lcda_curve_radius_threshold_for_zone_adaptation = 100.0f;
   lcda_cals.k_cvw_curve_zone_factor_outer                     = 0.5f;
   lcda_cals.k_cvw_curve_zone_factor_inner                     = 0.5f;
   lcda_cals.k_lcda_distance_traveled_scale_factor             = 0.6f;

   /** \action Call Lcda_Compute_Curve_Zone_Factor to compute the curve zone factor. */
   Lcda_Compute_Curve_Zone_Factor(&curve_zone_factor, &lcda_cals, p_vehicle_data, object_side, FBK_FALSE,
                                  -(lcda_cals.k_lcda_curve_radius_threshold_for_zone_adaptation - EPSILON));

   /** \assert Check that curve zone factor was decreased and curve value was changed to absolute. */
   EXPECT_FLOAT_EQ(lcda_cals.k_cvw_curve_zone_factor_outer, curve_zone_factor);
}


/**
 * Set host vehicle status such that it drives a curve. Create parameters for curve zone adaption and check that the corresponding
 * factor is reduced due to the curve driven for the left side. \uts{CSCSA-42621} \sdd{SF-6951} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test, Lcda_Compute_Curve_Zone_Factor__adapts_cvw_zone_length_right_side)
{
   /** \arrange Set up parameters for curve zone adaption. */
   uint8_t object_side         = FBK_SIDE_RIGHT;
   float32_T curve_zone_factor = 1.0f;

   p_vehicle_data->yawrate = -0.5f;

   lcda_cals.k_lcda_curve_radius_threshold_for_zone_adaptation = 100.0f;
   lcda_cals.k_cvw_curve_zone_factor_outer                     = 0.5f;
   lcda_cals.k_cvw_curve_zone_factor_inner                     = 0.5f;
   lcda_cals.k_lcda_distance_traveled_scale_factor             = 0.6f;

   /** \action Call Lcda_Compute_Curve_Zone_Factor to compute the curve zone factor. */
   Lcda_Compute_Curve_Zone_Factor(&curve_zone_factor, &lcda_cals, p_vehicle_data, object_side, FBK_FALSE,
                                  lcda_cals.k_lcda_curve_radius_threshold_for_zone_adaptation - EPSILON);

   /** \assert Check that curve zone factor was decreased. */
   EXPECT_FLOAT_EQ(lcda_cals.k_cvw_curve_zone_factor_inner, curve_zone_factor);
}

/**
 * Set host vehicle status such that it drives a straight. Check that curve zone factor is constant. uts{}
 * \uts{CSCSA-185767} \sdd{SF-6951} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test, Lcda_Compute_Curve_Zone_Factor__no_adaptation_cvw_zone_curve_over_thrshold)
{
   /** \arrange Set up parameters for curve zone adaption. */
   uint8_t object_side         = FBK_SIDE_LEFT;
   float32_T curve_zone_factor = 0.0f;
   float32_T res               = curve_zone_factor;

   p_vehicle_data->yawrate = -0.5f;

   lcda_cals.k_lcda_curve_radius_threshold_for_zone_adaptation = 100.0f;
   lcda_cals.k_cvw_curve_zone_factor_outer                     = 0.5f;
   lcda_cals.k_cvw_curve_zone_factor_inner                     = 0.5f;
   lcda_cals.k_lcda_distance_traveled_scale_factor             = 0.6f;

   /** \action Call Lcda_Compute_Curve_Zone_Factor to compute the curve zone factor. */
   Lcda_Compute_Curve_Zone_Factor(&curve_zone_factor, &lcda_cals, p_vehicle_data, object_side, FBK_FALSE,
                                  lcda_cals.k_lcda_curve_radius_threshold_for_zone_adaptation + EPSILON);

   /** \assert Check that curve zone factor was decreased. */
   EXPECT_FLOAT_EQ(curve_zone_factor, res);
}


/**
 * Set host vehicle status such that it drives a straight. Check that curve zone factor is constant. uts{}
 * \uts{CSCSA-275214} \sdd{SF-6951} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test, Lcda_Compute_Curve_Zone_Factor__no_adaptation_cvw_zone_curve_over_thrshold_right)
{
   /** \arrange Set up parameters for curve zone adaption. */
   uint8_t object_side         = FBK_SIDE_LEFT;
   float32_T curve_zone_factor = 0.0f;
   float32_T res               = curve_zone_factor;

   p_vehicle_data->yawrate = 0.5f;

   lcda_cals.k_lcda_curve_radius_threshold_for_zone_adaptation = 100.0f;
   lcda_cals.k_cvw_curve_zone_factor_outer                     = 0.5f;
   lcda_cals.k_cvw_curve_zone_factor_inner                     = 0.5f;
   lcda_cals.k_lcda_distance_traveled_scale_factor             = 0.6f;

   /** \action Call Lcda_Compute_Curve_Zone_Factor to compute the curve zone factor. */
   Lcda_Compute_Curve_Zone_Factor(&curve_zone_factor, &lcda_cals, p_vehicle_data, object_side, FBK_FALSE,
                                  lcda_cals.k_lcda_curve_radius_threshold_for_zone_adaptation + EPSILON);

   /** \assert Check that curve zone factor was decreased. */
   EXPECT_FLOAT_EQ(curve_zone_factor, res);
}


/**
 * Set host vehicle status such that it drives a curve. Create parameters for curve zone adaption and check that the corresponding
 * factor is reduced due to the curve driven for the left side. \uts{CSCSA-42622} \sdd{SF-6956} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test, Lcda_Compute_Applied_Curve_Zone_Factor__shorten)
{
   /** \arrange Set up parameters for curve zone adaption. */
   uint8_t object_side                  = FBK_SIDE_LEFT;
   float32_T curve_zone_factor_original = 1.0f;
   float32_T curve_zone_factor_applied  = 1.0f;

   p_vehicle_data->yawrate  = 0.5f;
   p_vehicle_data->long_vel = 10.0f;

   lcda_cals.k_lcda_curve_radius_threshold_for_zone_adaptation = 100.0f;
   lcda_cals.k_cvw_curve_zone_factor_outer                     = 0.5f;
   lcda_cals.k_cvw_curve_zone_factor_inner                     = 0.5f;
   lcda_cals.k_lcda_distance_traveled_scale_factor             = 0.6f;

   Lcda_Store_Pers_Cvw_Curve_Zone_Factor(&cvw_persistent, curve_zone_factor_original, object_side);

   /** \action Call Lcda_Compute_Applied_Curve_Zone_Factor to compute the curve zone factor. */
   Lcda_Compute_Applied_Curve_Zone_Factor(&curve_zone_factor_applied, &lcda_core_input, p_vehicle_data, &lcda_cals, object_side,
                                          100.0f, 20.0f, 0.5f, &cvw_persistent);

   /** \assert Check that curve zone factor was decreased. */
   EXPECT_TRUE(curve_zone_factor_original > curve_zone_factor_applied);
}

/**
 * Set host vehicle status such that it drives a curve. Create parameters for curve zone adaption and check that the corresponding
 * factor is reduced due to the curve driven for the left side. \uts{CSCSA-187414} \sdd{SF-6956} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test, Lcda_Compute_Applied_Curve_Zone_Factor__dont_shorten)
{
   /** \arrange Set up parameters for curve zone adaption. */
   uint8_t object_side                  = FBK_SIDE_LEFT;
   float32_T curve_zone_factor_original = 1.0f;
   float32_T curve_zone_factor_applied  = 1.0f;

   p_vehicle_data->yawrate  = 0.5f;
   p_vehicle_data->long_vel = 10.0f;

   lcda_cals.k_lcda_curve_radius_threshold_for_zone_adaptation = 100.0f;
   lcda_cals.k_cvw_curve_zone_factor_outer                     = 0.5f;
   lcda_cals.k_cvw_curve_zone_factor_inner                     = 0.5f;
   lcda_cals.k_lcda_distance_traveled_scale_factor             = 0.6f;

   Lcda_Store_Pers_Cvw_Curve_Zone_Factor(&cvw_persistent, curve_zone_factor_original, object_side);

   /** \action Call Lcda_Compute_Applied_Curve_Zone_Factor to compute the curve zone factor. */
   Lcda_Compute_Applied_Curve_Zone_Factor(&curve_zone_factor_applied, &lcda_core_input, p_vehicle_data, &lcda_cals, object_side,
                                          0.0f, 20.0f, 0.5f, &cvw_persistent);

   /** \assert Check that curve zone factor was decreased. */
   EXPECT_TRUE(curve_zone_factor_original == curve_zone_factor_applied);
}

/**
 * Check if max function is working correctly (for branch coverage)
 * \uts{CSCSA-42623} \sdd{SF-6600} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test, Lcda_Apply_Curve_Zone_Adaptation__test_max)
{
   /** \arrange Set up parameters for curve zone adaption. */
   uint8_t object_side = FBK_SIDE_LEFT;
   uint8_t j;
   Fbk_Field_Of_Interest_T cvw_zone{};

   for (j = 1; j < LCDA_NUMBER_OF_ZONE_POINTS; j++)
   {
      cvw_zone.points[j].x = 1.0f;
   }
   cvw_zone.points[0].x = 0.5f;

   /** \action Call Lcda_Apply_Curve_Zone_Adaptation to compute the curve zone factor. */
   Lcda_Apply_Curve_Zone_Adaptation(&cvw_persistent, &cvw_zone, object_side);

   /** \assert Check that the CVW zone was shortened as expected. */
   EXPECT_FLOAT_EQ(cvw_zone.points[REAR_EGO_SIDE].x, 1.0f);
   EXPECT_FLOAT_EQ(cvw_zone.points[REAR_OUTER_SIDE].x, 1.0f);
}


/**
 * Create CVW initial zone based on vehicle length and lane width. Set lane width too large and check that a maximal lane width as
 * defined by calibration parameters is used to create the zone. \uts{CSCSA-211381} \sdd{SF-6603} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test, Lcda_Get_Lw_Based_Initial_Cvw_Zones__outer_zone_points_are_limited_hyst_factor_zero)
{
   /** \arrange Set up zone parameters for zone creation. */
   lcda_core_input.lane_width         = lcda_cals.k_lcda_max_lane_width * 2.0f;
   lcda_core_input.lane_center_offset = 0.0f;


   Fbk_Field_Of_Interest_T cvw_zone{};
   Fbk_Field_Of_Interest_T cvw_zone_hys{};

   cvw_object.ego_side  = FBK_SIDE_RIGHT;
   tracker_object.index = 0u;
   tracker_object.width = -1.0f;

   /** \action Call Lcda_Get_Lw_Based_Initial_Cvw_Zones to create zone. */
   Lcda_Get_Lw_Based_Initial_Cvw_Zones(&cvw_zone, &cvw_zone_hys, f_use_small_lc_intention_zone, &cvw_object, &lcda_core_input,
                                       &lcda_cals);

   /** \assert Check for representative coordinate that that it was computed correctly. */
   EXPECT_FLOAT_EQ(cvw_zone.points[FRONT_OUTER_SIDE].y, (Fbk_Half(lcda_core_input.lane_width) + lcda_cals.k_lcda_max_lane_width));
   EXPECT_FLOAT_EQ(cvw_zone.points[MIDDLE_OUTER_SIDE].y, (Fbk_Half(lcda_core_input.lane_width) + lcda_cals.k_lcda_max_lane_width));
   EXPECT_FLOAT_EQ(cvw_zone.points[REAR_OUTER_SIDE].y, (Fbk_Half(lcda_core_input.lane_width) + lcda_cals.k_lcda_max_lane_width));
}


/**
 * Create CVW initial zone based on vehicle length and lane width. Set lane width too large and check that a maximal lane width as
 * defined by calibration parameters is used to create the zone. \uts{CSCSA-211382} \sdd{SF-6603} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test, Lcda_Get_Lw_Based_Initial_Cvw_Zones__outer_zone_points_are_limited_hyst_factor_one)
{
   /** \arrange Set up zone parameters for zone creation. */
   lcda_core_input.lane_width         = lcda_cals.k_lcda_max_lane_width * 2.0f;
   lcda_core_input.lane_center_offset = 0.0f;


   Fbk_Field_Of_Interest_T cvw_zone{};
   Fbk_Field_Of_Interest_T cvw_zone_hys{};

   cvw_object.ego_side                       = FBK_SIDE_RIGHT;
   tracker_object.index                      = 0u;
   tracker_object.width                      = 2.0f;
   lcda_cals.k_zone_hys_obj_width_correction = 1.0f;
   lcda_cals.k_cvw_zone_y_hys_min            = 2.0f;

   /** \action Call Lcda_Get_Lw_Based_Initial_Cvw_Zones to create zone. */
   Lcda_Get_Lw_Based_Initial_Cvw_Zones(&cvw_zone, &cvw_zone_hys, f_use_small_lc_intention_zone, &cvw_object, &lcda_core_input,
                                       &lcda_cals);

   /** \assert Check for representative coordinate that that it was computed correctly. */
   EXPECT_FLOAT_EQ(cvw_zone.points[FRONT_OUTER_SIDE].y, (Fbk_Half(lcda_core_input.lane_width) + lcda_cals.k_lcda_max_lane_width));
   EXPECT_FLOAT_EQ(cvw_zone.points[MIDDLE_OUTER_SIDE].y, (Fbk_Half(lcda_core_input.lane_width) + lcda_cals.k_lcda_max_lane_width));
   EXPECT_FLOAT_EQ(cvw_zone.points[REAR_OUTER_SIDE].y, (Fbk_Half(lcda_core_input.lane_width) + lcda_cals.k_lcda_max_lane_width));
}


/**
 * Set host vehicle status such that it drives straight. Expect unchanged curve_zone_factor.
 * \uts{CSCSA-211383} \sdd{SF-6951} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test, Lcda_Compute_Curve_Zone_Factor__yawrate_zero)
{
   /** \arrange Set up parameters for drive straight. */
   uint8_t object_side         = FBK_SIDE_UNDEFINED;
   float32_T curve_zone_factor = 5.0f;

   p_vehicle_data->yawrate = 0.0f;

   lcda_cals.k_lcda_curve_radius_threshold_for_zone_adaptation = 100.0f;
   lcda_cals.k_cvw_curve_zone_factor_outer                     = 0.5f;
   lcda_cals.k_cvw_curve_zone_factor_inner                     = 0.5f;
   lcda_cals.k_lcda_distance_traveled_scale_factor             = 0.6f;

   /** \action Call Lcda_Compute_Curve_Zone_Factor to compute the curve zone factor. */
   Lcda_Compute_Curve_Zone_Factor(&curve_zone_factor, &lcda_cals, p_vehicle_data, object_side, FBK_FALSE,
                                  lcda_cals.k_lcda_curve_radius_threshold_for_zone_adaptation - EPSILON);

   /** \assert Check that curve zone factor was not changed. */
   EXPECT_FLOAT_EQ(curve_zone_factor, 5.0f);
}


/**
 * Set host vehicle status such that it drives a curve. Create parameters for curve zone adaption and check that the corresponding
 * factor is reduced due to the curve driven for the left side. \uts{CSCSA-211384} \sdd{SF-6956} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test, Lcda_Compute_Applied_Curve_Zone_Factor__shorten_case2)
{
   /** \arrange Set up parameters for curve zone adaption. */
   uint8_t object_side                  = FBK_SIDE_LEFT;
   float32_T curve_zone_factor_original = 1.0f;
   float32_T curve_zone_factor_applied  = 1.0f;

   p_vehicle_data->yawrate  = 0.5f;
   p_vehicle_data->long_vel = 10.0f;

   lcda_cals.k_lcda_curve_radius_threshold_for_zone_adaptation = 100.0f;
   lcda_cals.k_cvw_curve_zone_factor_outer                     = 0.5f;
   lcda_cals.k_cvw_curve_zone_factor_inner                     = 0.5f;
   lcda_cals.k_lcda_distance_traveled_scale_factor             = 0.6f;

   Lcda_Store_Pers_Cvw_Curve_Zone_Factor(&cvw_persistent, curve_zone_factor_original, object_side);

   /** \action Call Lcda_Compute_Applied_Curve_Zone_Factor to compute the curve zone factor. */
   Lcda_Compute_Applied_Curve_Zone_Factor(&curve_zone_factor_applied, &lcda_core_input, p_vehicle_data, &lcda_cals, object_side,
                                          1.0f, -20.0f, 0.5f, &cvw_persistent);

   /** \assert Check that curve zone factor was decreased. */
   EXPECT_FALSE(curve_zone_factor_original > curve_zone_factor_applied);
}


/**
 * Set host vehicle status such that it drives a curve. Create parameters for curve zone adaption and check that the corresponding
 * factor is reduced due to the curve driven for the left side. \uts{CSCSA-211385} \sdd{SF-6956} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Cvw_Zone_Test, Lcda_Compute_Applied_Curve_Zone_Factor__lengthen)
{
   /** \arrange Set up parameters for curve zone adaption. */
   uint8_t object_side                  = FBK_SIDE_LEFT;
   float32_T curve_zone_factor_original = 5.0f;
   float32_T curve_zone_factor_applied  = 1.0f;

   p_vehicle_data->yawrate  = 0.5f;
   p_vehicle_data->long_vel = 10.0f;

   lcda_cals.k_lcda_curve_radius_threshold_for_zone_adaptation = 100.0f;
   lcda_cals.k_cvw_curve_zone_factor_outer                     = 0.5f;
   lcda_cals.k_cvw_curve_zone_factor_inner                     = 0.5f;
   lcda_cals.k_lcda_distance_traveled_scale_factor             = 0.6f;

   Lcda_Store_Pers_Cvw_Curve_Zone_Factor(&cvw_persistent, curve_zone_factor_original, object_side);

   /** \action Call Lcda_Compute_Applied_Curve_Zone_Factor to compute the curve zone factor. */
   Lcda_Compute_Applied_Curve_Zone_Factor(&curve_zone_factor_applied, &lcda_core_input, p_vehicle_data, &lcda_cals, object_side,
                                          10.0f, 20.0f, 1.0f, &cvw_persistent);

   /** \assert Check that curve zone factor was decreased. */
   EXPECT_TRUE(curve_zone_factor_original > curve_zone_factor_applied);
}