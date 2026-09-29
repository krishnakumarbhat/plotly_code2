/**
 * @file lcda_create_bsw_zone_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for lcda_create_bsw_zone.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42579}
 */

#include "lcda_create_bsw_zone_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "lcda_create_bsw_zone.c"
#include "pa_reuse.h"
}


/**
 * Create BSW initial zone based on vehicle length and lane width. Set vehicle width to zero and check that a minimal vehicle width
 * as defined by calibration parameters is used to create the zone. \uts{CSCSA-42580} \sdd{SF-6593}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test,
       Lcda_Get_Vl_Lw_Based_Initial_Bsw_Zones__zone_point5_y_is_multiple_of_k_min_ego_vehicle_width_for_ego_width_smaller_than_range)
{
   /** \arrange Set up zone parameters for zone creation. */
   lcda_core_input.bsw_zone_calculation_mode = BSW_ZONE_CALC_VL_LW;

   p_vehicle_data->host_width            = 0.0f;
   lcda_cals.k_bsw_lateral_distance_zone = 0.0f; // So that point 5
                                                 // starts
                                                 // immediately at
                                                 // the vehicle edge

   Fbk_Field_Of_Interest_T bsw_zone{};
   Fbk_Field_Of_Interest_T bsw_zone_hys{};

   bsw_object.ego_side  = FBK_SIDE_RIGHT;
   tracker_object.index = 0u;

   /** \action Call Lcda_Get_Vl_Lw_Based_Initial_Bsw_Zones to create zone. */
   Lcda_Get_Vl_Lw_Based_Initial_Bsw_Zones(&bsw_zone, &bsw_zone_hys, &bsw_object, &lcda_core_input, p_vehicle_data, &lcda_cals);

   /** \assert Check for representative coordinate that it was computed correctly. */
   EXPECT_FLOAT_EQ(bsw_zone.points[5].y, lcda_cals.k_lcda_min_ego_vehicle_width * 0.5f);
}

/**
 * Create BSW initial zone based on vehicle length and lane width. Set vehicle width too large and check that a maximal vehicle
 * width as defined by calibration parameters is used to create the zone. \uts{CSCSA-42581} \sdd{SF-6593}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test,
       Lcda_Get_Vl_Lw_Based_Initial_Bsw_Zones__zone_point5_y_is_multiple_of_k_max_ego_vehicle_width_for_ego_width_larger_than_range)
{
   /** \arrange Set up zone parameters for zone creation. */
   lcda_core_input.bsw_zone_calculation_mode = BSW_ZONE_CALC_VL_LW;
   lcda_cals.k_bsw_lateral_distance_zone     = 0.0f; // So that point 5
                                                     // starts
                                                     // immediately at
                                                     // the vehicle edge

   p_vehicle_data->host_width = lcda_cals.k_lcda_max_ego_vehicle_width + 2.0f;
   Fbk_Field_Of_Interest_T bsw_zone{};
   Fbk_Field_Of_Interest_T bsw_zone_hys{};

   bsw_object.ego_side  = FBK_SIDE_LEFT;
   tracker_object.index = 0u;

   /** \action Call Lcda_Get_Vl_Lw_Based_Initial_Bsw_Zones to create zone. */
   Lcda_Get_Vl_Lw_Based_Initial_Bsw_Zones(&bsw_zone, &bsw_zone_hys, &bsw_object, &lcda_core_input, p_vehicle_data, &lcda_cals);

   /** \assert Check for representative coordinate that it was computed correctly. */
   EXPECT_FLOAT_EQ(bsw_zone.points[5].y, lcda_cals.k_lcda_max_ego_vehicle_width * 0.5f);
}

/**
 * Create BSW initial zone based on fixed calibration values.
 * \uts{CSCSA-42582} \sdd{SF-6927} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Get_Vcs_Fixed_Initial_Bsw_Zones__zones_are_set_correctly_based_on_cal_values)
{
   /** \arrange Set up zone parameters for zone creation. */
   Fbk_Field_Of_Interest_T bsw_zone{};
   Fbk_Field_Of_Interest_T bsw_zone_hys{};
   float32_T vehicle_length = 4.0f;
   float32_T object_width   = 2.0f;

   /** \action Call Lcda_Get_Vl_Lw_Based_Initial_Bsw_Zones to create zone. */
   Lcda_Get_Vcs_Fixed_Initial_Bsw_Zones(&bsw_zone, &bsw_zone_hys, vehicle_length, object_width, &lcda_cals);

   /** \assert Check that all coordinates were computed correctly. */
   EXPECT_FLOAT_EQ(bsw_zone.points[FRONT_OUTER_SIDE].x, -(vehicle_length * lcda_cals.k_bsw_fixed_zone_x[FRONT_EGO_SIDE]));
   EXPECT_FLOAT_EQ(bsw_zone.points[MIDDLE_OUTER_SIDE].x, -(vehicle_length) + lcda_cals.k_bsw_fixed_zone_x[MIDDLE_OUTER_SIDE]);
   EXPECT_FLOAT_EQ(bsw_zone.points[REAR_OUTER_SIDE].x, -(vehicle_length) + lcda_cals.k_bsw_fixed_zone_x[REAR_OUTER_SIDE]);
   EXPECT_FLOAT_EQ(bsw_zone.points[REAR_EGO_SIDE].x, -(vehicle_length) + lcda_cals.k_bsw_fixed_zone_x[REAR_EGO_SIDE]);
   EXPECT_FLOAT_EQ(bsw_zone.points[MIDDLE_EGO_SIDE].x, -(vehicle_length) + lcda_cals.k_bsw_fixed_zone_x[MIDDLE_EGO_SIDE]);
   EXPECT_FLOAT_EQ(bsw_zone.points[FRONT_EGO_SIDE].x, -(vehicle_length * lcda_cals.k_bsw_fixed_zone_x[FRONT_EGO_SIDE]));

   for (uint8_t i = 0; i < LCDA_NUMBER_OF_ZONE_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(bsw_zone_hys.points[i].x, bsw_zone.points[i].x + lcda_cals.k_bsw_fixed_zone_x_hys[i]);

      EXPECT_FLOAT_EQ(bsw_zone.points[i].y, lcda_cals.k_bsw_fixed_zone_y[i]);
      EXPECT_FLOAT_EQ(bsw_zone_hys.points[i].y,
                      lcda_cals.k_bsw_fixed_zone_y[i] + (lcda_cals.k_bsw_fixed_zone_y_hys[i] * object_width));
   }
}

/**
 * The BSW zone is shrunk depending on the ego vehicle speed. Use ego speed outside of allowed range such that the zone should not
 * be shrunk. \uts{CSCSA-42583} \sdd{SF-6594} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test,
       Lcda_Shrink_Bsw_Zone_Dropback_Obj__zone_unchanged_when_ego_speed_greater_than_k_bsw_dynzone_speed_dropback_max)
{
   /** \arrange Create BSW zones and a slowly backfalling target object. */
   Fbk_Field_Of_Interest_T final_zone     = default_bsw_zone;
   Fbk_Field_Of_Interest_T final_zone_hys = default_bsw_zone_hys;

   uint8_t points;
   uint8_t index_track_with_negative_rel_long_vel = 10;

   lcda_cals.k_bsw_dynzone_speed_dropback[0u] = -2.75f;
   lcda_cals.k_bsw_dynzone_speed_dropback[1u] = -2.22f;
   lcda_cals.k_bsw_dynzone_speed_dropback[2u] = -1.95f;
   lcda_cals.k_bsw_dynzone_speed_dropback[3u] = -1.4f;
   lcda_cals.k_bsw_dynzone_speed_dropback[4u] = -1.11f;
   lcda_cals.k_bsw_dynzone_speed_dropback[5u] = -0.83f;
   lcda_cals.k_bsw_dynzone_speed_dropback[6u] = -0.0f;
   lcda_cals.k_bsw_dynzone_speed_dropback[7u] = -0.0f;

   lcda_cals.k_bsw_dynzone_range_dropback[0u] = 0.55f;
   lcda_cals.k_bsw_dynzone_range_dropback[1u] = 0.65f;
   lcda_cals.k_bsw_dynzone_range_dropback[2u] = 0.7f;
   lcda_cals.k_bsw_dynzone_range_dropback[3u] = 0.8f;
   lcda_cals.k_bsw_dynzone_range_dropback[4u] = 0.9f;
   lcda_cals.k_bsw_dynzone_range_dropback[5u] = 0.95f;
   lcda_cals.k_bsw_dynzone_range_dropback[6u] = 1.0f;
   lcda_cals.k_bsw_dynzone_range_dropback[7u] = 1.0f;
   lcda_cals.k_bsw_shrink_zone_method         = FBK_FALSE;
   lcda_cals.k_bsw_dynzone_speed_dropback_max = 35.0f;

   Lcda_Create_Bsw_Track(&tracker_object, index_track_with_negative_rel_long_vel, -20.0f, 4.5f);
   tracker_object.curvi_vel_rel.x = -4.0f;
   tracker_object.vcs_vel_rel.x   = -4.0f;

   p_vehicle_data->host_speed = 75.0f;

   /** \action Call Lcda_Shrink_Bsw_Zone_Dropback_Obj to shrink BSW zone. */
   Lcda_Shrink_Bsw_Zone_Dropback_Obj(&final_zone, &final_zone_hys, &bsw_object, p_vehicle_data, &lcda_cals);

   /** \assert Check that zone is not altered. */
   for (points = 0; points < LCDA_NUMBER_OF_ZONE_POINTS; points++)
   {
      EXPECT_FLOAT_EQ(final_zone.points[points].x, default_bsw_zone.points[points].x);
      EXPECT_FLOAT_EQ(final_zone.points[points].y, default_bsw_zone.points[points].y);

      EXPECT_FLOAT_EQ(final_zone_hys.points[points].x, default_bsw_zone_hys.points[points].x);
      EXPECT_FLOAT_EQ(final_zone_hys.points[points].y, default_bsw_zone_hys.points[points].y);
   }
}

/**
 * The BSW zone is shrunk depending on the target objects relative velocity. Use relative velocity outside of allowed range such
 * that the zone should not be shrunk. \uts{CSCSA-42584} \sdd{SF-6594} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test,
       Lcda_Shrink_Bsw_Zone_Dropback_Obj__zone_unchanged_when_track_long_vel_rel_greater_than_k_bsw_dynzone_speed_dropback_7)
{
   /** \arrange Create BSW zones and an overtaking target object. */
   Fbk_Field_Of_Interest_T final_zone     = default_bsw_zone;
   Fbk_Field_Of_Interest_T final_zone_hys = default_bsw_zone_hys;
   uint8_t points;
   uint8_t index_track_with_positive_rel_long_vel = 15;

   // For this test, set ego abs_speed < dynzone_speed_dropback_max
   p_vehicle_data->host_speed = 30.0f;
   Lcda_Create_Bsw_Track(&tracker_object, index_track_with_positive_rel_long_vel, -10.0f, 4.5f);
   tracker_object.curvi_vel_rel.x = 15.0f;
   tracker_object.vcs_vel_rel.x   = 15.0f;

   /** \action Call Lcda_Shrink_Bsw_Zone_Dropback_Obj to shrink BSW zone. */
   Lcda_Shrink_Bsw_Zone_Dropback_Obj(&final_zone, &final_zone_hys, &bsw_object, p_vehicle_data, &lcda_cals);

   /** \assert Check that zone is not altered. */
   for (points = 0; points < LCDA_NUMBER_OF_ZONE_POINTS; points++)
   {
      EXPECT_FLOAT_EQ(final_zone.points[points].x, default_bsw_zone.points[points].x);
      EXPECT_FLOAT_EQ(final_zone.points[points].y, default_bsw_zone.points[points].y);

      EXPECT_FLOAT_EQ(final_zone_hys.points[points].x, default_bsw_zone_hys.points[points].x);
      EXPECT_FLOAT_EQ(final_zone_hys.points[points].y, default_bsw_zone_hys.points[points].y);
   }
}

/**
 * The BSW zone is shrunk depending on the target objects relative velocity. Use relative velocity outside of allowed range such
 * that the zone should not be shrunk. Host speed zero. \uts{CSCSA-187411} \sdd{SF-6594} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test,
       Lcda_Shrink_Bsw_Zone_Dropback_Obj__zone_unchanged_when_track_long_vel_rel_greater_than_k_bsw_dynzone_speed_zero)
{
   /** \arrange Create BSW zones and an overtaking target object. */
   Fbk_Field_Of_Interest_T final_zone     = default_bsw_zone;
   Fbk_Field_Of_Interest_T final_zone_hys = default_bsw_zone_hys;
   uint8_t points;
   uint8_t index_track_with_positive_rel_long_vel = 15;

   // For this test, set ego abs_speed < dynzone_speed_dropback_max
   p_vehicle_data->host_speed = 0.0f;
   Lcda_Create_Bsw_Track(&tracker_object, index_track_with_positive_rel_long_vel, -10.0f, 4.5f);
   tracker_object.curvi_vel_rel.x = 15.0f;
   tracker_object.vcs_vel_rel.x   = 15.0f;

   /** \action Call Lcda_Shrink_Bsw_Zone_Dropback_Obj to shrink BSW zone. */
   Lcda_Shrink_Bsw_Zone_Dropback_Obj(&final_zone, &final_zone_hys, &bsw_object, p_vehicle_data, &lcda_cals);

   /** \assert Check that zone is not altered. */
   for (points = 0; points < LCDA_NUMBER_OF_ZONE_POINTS; points++)
   {
      EXPECT_FLOAT_EQ(final_zone.points[points].x, default_bsw_zone.points[points].x);
      EXPECT_FLOAT_EQ(final_zone.points[points].y, default_bsw_zone.points[points].y);

      EXPECT_FLOAT_EQ(final_zone_hys.points[points].x, default_bsw_zone_hys.points[points].x);
      EXPECT_FLOAT_EQ(final_zone_hys.points[points].y, default_bsw_zone_hys.points[points].y);
   }
}

/**
 * The BSW zone is shrunk depending on the target objects relative velocity. Create slowly backfalling target object such that the
 * zone is shrunk. \uts{CSCSA-42585} \sdd{SF-6594} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Shrink_Bsw_Zone_Dropback_Obj__zone_shrinks_0p7_when_track_long_vel_rel_minus1p95)
{
   /** \arrange Create BSW zones and slowly backfalling target object. */
   Fbk_Field_Of_Interest_T final_zone     = default_bsw_zone;
   Fbk_Field_Of_Interest_T final_zone_hys = default_bsw_zone_hys;
   uint8_t points;
   uint8_t index_track_with_negative_rel_long_vel = 0;

   lcda_cals.k_bsw_dynzone_speed_dropback[0u] = -2.75f;
   lcda_cals.k_bsw_dynzone_speed_dropback[1u] = -2.22f;
   lcda_cals.k_bsw_dynzone_speed_dropback[2u] = -1.95f;
   lcda_cals.k_bsw_dynzone_speed_dropback[3u] = -1.4f;
   lcda_cals.k_bsw_dynzone_speed_dropback[4u] = -1.11f;
   lcda_cals.k_bsw_dynzone_speed_dropback[5u] = -0.83f;
   lcda_cals.k_bsw_dynzone_speed_dropback[6u] = -0.0f;
   lcda_cals.k_bsw_dynzone_speed_dropback[7u] = -0.0f;

   lcda_cals.k_bsw_dynzone_range_dropback[0u] = 0.55f;
   lcda_cals.k_bsw_dynzone_range_dropback[1u] = 0.65f;
   lcda_cals.k_bsw_dynzone_range_dropback[2u] = 0.7f;
   lcda_cals.k_bsw_dynzone_range_dropback[3u] = 0.8f;
   lcda_cals.k_bsw_dynzone_range_dropback[4u] = 0.9f;
   lcda_cals.k_bsw_dynzone_range_dropback[5u] = 0.95f;
   lcda_cals.k_bsw_dynzone_range_dropback[6u] = 1.0f;
   lcda_cals.k_bsw_dynzone_range_dropback[7u] = 1.0f;
   lcda_cals.k_bsw_shrink_zone_method         = FBK_FALSE;

   // For this test, set ego speed < dynzone_speed_dropback_max
   p_vehicle_data->host_speed = 30.0f;
   Lcda_Create_Bsw_Track(&tracker_object, index_track_with_negative_rel_long_vel, -20.0f, 4.5f);
   tracker_object.curvi_vel_rel.x = -1.95f;
   tracker_object.vcs_vel_rel.x   = -1.95f;

   /** \action Call Lcda_Shrink_Bsw_Zone_Dropback_Obj to shrink BSW zone. */
   Lcda_Shrink_Bsw_Zone_Dropback_Obj(&final_zone, &final_zone_hys, &bsw_object, p_vehicle_data, &lcda_cals);

   /** \assert Check that zone is shrunk as expected. */
   // only x-coord of points 2 and 3 should reduce by 0.7
   EXPECT_FLOAT_EQ(final_zone.points[0].x, default_bsw_zone.points[0].x);
   EXPECT_FLOAT_EQ(final_zone.points[1].x, default_bsw_zone.points[1].x);
   EXPECT_FLOAT_EQ(final_zone.points[2].x, default_bsw_zone.points[2].x * 0.7f);
   EXPECT_FLOAT_EQ(final_zone.points[3].x, default_bsw_zone.points[3].x * 0.7f);
   EXPECT_FLOAT_EQ(final_zone.points[4].x, default_bsw_zone.points[4].x);
   EXPECT_FLOAT_EQ(final_zone.points[5].x, default_bsw_zone.points[5].x);

   EXPECT_FLOAT_EQ(final_zone_hys.points[0].x, default_bsw_zone_hys.points[0].x);
   EXPECT_FLOAT_EQ(final_zone_hys.points[1].x, default_bsw_zone_hys.points[1].x);
   EXPECT_FLOAT_EQ(final_zone_hys.points[2].x, default_bsw_zone_hys.points[2].x * 0.7f);
   EXPECT_FLOAT_EQ(final_zone_hys.points[3].x, default_bsw_zone_hys.points[3].x * 0.7f);
   EXPECT_FLOAT_EQ(final_zone_hys.points[4].x, default_bsw_zone_hys.points[4].x);
   EXPECT_FLOAT_EQ(final_zone_hys.points[5].x, default_bsw_zone_hys.points[5].x);

   // no change in the y-coord
   for (points = 0; points < LCDA_NUMBER_OF_ZONE_POINTS; points++)
   {
      EXPECT_FLOAT_EQ(final_zone.points[points].y, default_bsw_zone.points[points].y);
      EXPECT_FLOAT_EQ(final_zone_hys.points[points].y, default_bsw_zone_hys.points[points].y);
   }
}

/**
 * The BSW zone is shrunk depending on the target objects relative velocity. Create slowly backfalling target object such that the
 * zone is shrunk. Check that the zone expands at least to the ego rear bumper. \uts{CSCSA-42586} \sdd{SF-6594}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test,
       Lcda_Shrink_Bsw_Zone_Dropback_Obj__zone_pt2x_pt3x_saturates_at_ego_rear_bumper_when_zone_shrinks_in_front_of_ego_rear_bumper)
{
   /** \arrange Create BSW zones and slowly backfalling target object. Choose parameters such that the zone would not expand to the
    * egos rear bumper anymore. */
   Fbk_Field_Of_Interest_T final_zone     = default_bsw_zone;
   Fbk_Field_Of_Interest_T final_zone_hys = default_bsw_zone_hys;
   uint8_t points;
   uint8_t index_track_with_negative_rel_long_vel = 10;
   lcda_cals.k_bsw_shrink_zone_method             = FBK_FALSE;
   lcda_cals.k_bsw_dynzone_range_dropback[0]      = 0.5f;


   // For this test, set ego speed < dynzone_speed_dropback_max
   p_vehicle_data->host_speed = 30.0f;
   Lcda_Create_Bsw_Track(&tracker_object, index_track_with_negative_rel_long_vel, -20.0f, 4.5f);
   tracker_object.curvi_vel_rel.x = -4.0f;
   tracker_object.vcs_vel_rel.x   = -4.0f;

   // Set x-cord of points 2 and 3 so that zone shrinks in front of ego rear bumper
   final_zone.points[2].x = -4.8f;
   final_zone.points[3].x = -4.8f;

   /** \action Call Lcda_Shrink_Bsw_Zone_Dropback_Obj to shrink BSW zone. */
   Lcda_Shrink_Bsw_Zone_Dropback_Obj(&final_zone, &final_zone_hys, &bsw_object, p_vehicle_data, &lcda_cals);

   /** \assert Check that zone is shrunk as expected. */
   // only x-coord of points 2 and 3 should be set to vehicle length 4.5m
   EXPECT_FLOAT_EQ(final_zone.points[0].x, default_bsw_zone.points[0].x);
   EXPECT_FLOAT_EQ(final_zone.points[1].x, Fbk_Max(default_bsw_zone.points[1].x, -p_vehicle_data->host_length));
   EXPECT_FLOAT_EQ(final_zone.points[2].x, -p_vehicle_data->host_length);
   EXPECT_FLOAT_EQ(final_zone.points[3].x, -p_vehicle_data->host_length);
   EXPECT_FLOAT_EQ(final_zone.points[4].x, Fbk_Max(default_bsw_zone.points[4].x, -p_vehicle_data->host_length));
   EXPECT_FLOAT_EQ(final_zone.points[5].x, default_bsw_zone.points[5].x);

   EXPECT_FLOAT_EQ(final_zone_hys.points[0].x, default_bsw_zone_hys.points[0].x);
   EXPECT_FLOAT_EQ(final_zone_hys.points[1].x, Fbk_Max(default_bsw_zone_hys.points[1].x, -p_vehicle_data->host_length));
   EXPECT_FLOAT_EQ(final_zone_hys.points[2].x, -p_vehicle_data->host_length);
   EXPECT_FLOAT_EQ(final_zone_hys.points[3].x, -p_vehicle_data->host_length);
   EXPECT_FLOAT_EQ(final_zone_hys.points[4].x, Fbk_Max(default_bsw_zone_hys.points[4].x, -p_vehicle_data->host_length));
   EXPECT_FLOAT_EQ(final_zone_hys.points[5].x, default_bsw_zone_hys.points[5].x);

   // no change in the y-coord
   for (points = 0; points < LCDA_NUMBER_OF_ZONE_POINTS; points++)
   {
      EXPECT_FLOAT_EQ(final_zone.points[points].y, default_bsw_zone.points[points].y);
      EXPECT_FLOAT_EQ(final_zone_hys.points[points].y, default_bsw_zone_hys.points[points].y);
   }
}

/**
 * The BSW zone shall not be shrunk if dynamic BSW zones are disabled. Create slowly backfalling target object such that the zone
 * should be shrunk, but disable dynamic BSW zones. Check that the zone is not altered. \uts{CSCSA-42587} \sdd{SF-6597}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Create_Bsw_Zone__zone_is_not_adjusted_for_ego_speed_when_dyn_zone_disabled_by_cal)
{
   /** \arrange Create BSW zones and slowly backfalling target object. Disable dynamic BSW zones. */
   Fbk_Field_Of_Interest_T bsw_zone_speed1{};
   Fbk_Field_Of_Interest_T bsw_zone_hys_speed1{};

   Fbk_Field_Of_Interest_T bsw_zone_speed2{};
   Fbk_Field_Of_Interest_T bsw_zone_hys_speed2{};

   uint8_t ipoint;

   lcda_core_input.bsw_zone_calculation_mode       = BSW_ZONE_CALC_FIXED_INPUT;
   lcda_cals.k_bsw_enable_dynspeed_zone            = FBK_FALSE;
   lcda_cals.k_bsw_enable_trailer_zone_extension   = FBK_FALSE;
   lcda_cals.k_bsw_f_enable_object_rel_vel_dynzone = FBK_FALSE;

   lcda_core_input.lane_width = 0.0f;
   p_vehicle_data->lane_width = 0.0f;

   // Set default zone points in the core input
   lcda_core_input.initial_bsw_zone     = default_bsw_zone;
   lcda_core_input.initial_bsw_zone_hys = default_bsw_zone_hys;

   bsw_object.ego_side  = FBK_SIDE_RIGHT;
   tracker_object.index = 0u;

   // First create the zone for the given ego speed
   Lcda_Create_Bsw_Zone(&bsw_zone_speed1, &bsw_zone_hys_speed1, &bsw_object, &lcda_core_input, &lcda_cals);

   // Now quarter the ego speed and then create the BSW zones again. The zones for speed1 and speed2 should be identical
   p_vehicle_data->host_speed = p_vehicle_data->host_speed * 0.25f;

   /** \action Call Lcda_Shrink_Bsw_Zone_Dropback_Obj to shrink BSW zone. */
   Lcda_Create_Bsw_Zone(&bsw_zone_speed2, &bsw_zone_hys_speed2, &bsw_object, &lcda_core_input, &lcda_cals);

   /** \assert Check that zone is not modified. */
   for (ipoint = 0; ipoint < LCDA_NUMBER_OF_ZONE_POINTS; ipoint++)
   {
      EXPECT_FLOAT_EQ(bsw_zone_speed1.points[ipoint].x, bsw_zone_speed2.points[ipoint].x);
      EXPECT_FLOAT_EQ(bsw_zone_speed1.points[ipoint].y, bsw_zone_speed2.points[ipoint].y);

      EXPECT_FLOAT_EQ(bsw_zone_hys_speed1.points[ipoint].x, bsw_zone_hys_speed2.points[ipoint].x);
      EXPECT_FLOAT_EQ(bsw_zone_hys_speed1.points[ipoint].y, bsw_zone_hys_speed2.points[ipoint].y);
   }
}

/**
 * The BSW zone shall not be shrunk if dynamic BSW zones are disabled. Create slowly backfalling target object such that the zone
 * should be shrunk, but disable dynamic BSW zones and set negative vehicle speed. Check that the zone is not altered.
 * \uts{CSCSA-187412} \sdd{SF-6597} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Create_Bsw_Zone__zone_is_not_adjusted_for_ego_negative_speed)
{
   /** \arrange Create BSW zones and slowly backfalling target object. Disable dynamic BSW zones. */
   Fbk_Field_Of_Interest_T bsw_zone_speed1{};
   Fbk_Field_Of_Interest_T bsw_zone_hys_speed1{};

   Fbk_Field_Of_Interest_T bsw_zone_speed2{};
   Fbk_Field_Of_Interest_T bsw_zone_hys_speed2{};

   uint8_t ipoint;

   lcda_core_input.bsw_zone_calculation_mode       = BSW_ZONE_CALC_FIXED_INPUT;
   lcda_cals.k_bsw_enable_dynspeed_zone            = FBK_FALSE;
   lcda_cals.k_bsw_enable_trailer_zone_extension   = FBK_FALSE;
   lcda_cals.k_bsw_f_enable_object_rel_vel_dynzone = FBK_FALSE;

   lcda_core_input.lane_width = 0.0f;
   p_vehicle_data->lane_width = 0.0f;
   p_vehicle_data->host_speed = -p_vehicle_data->host_speed;

   // Set default zone points in the core input
   lcda_core_input.initial_bsw_zone     = default_bsw_zone;
   lcda_core_input.initial_bsw_zone_hys = default_bsw_zone_hys;

   bsw_object.ego_side  = FBK_SIDE_RIGHT;
   tracker_object.index = 0u;

   // First create the zone for the given ego speed
   Lcda_Create_Bsw_Zone(&bsw_zone_speed1, &bsw_zone_hys_speed1, &bsw_object, &lcda_core_input, &lcda_cals);

   // Now quarter the ego speed and then create the BSW zones again. The zones for speed1 and speed2 should be identical
   p_vehicle_data->host_speed = p_vehicle_data->host_speed * 0.25f;

   /** \action Call Lcda_Shrink_Bsw_Zone_Dropback_Obj to shrink BSW zone. */
   Lcda_Create_Bsw_Zone(&bsw_zone_speed2, &bsw_zone_hys_speed2, &bsw_object, &lcda_core_input, &lcda_cals);

   /** \assert Check that zone is not modified. */
   for (ipoint = 0; ipoint < LCDA_NUMBER_OF_ZONE_POINTS; ipoint++)
   {
      EXPECT_FLOAT_EQ(bsw_zone_speed1.points[ipoint].x, bsw_zone_speed2.points[ipoint].x);
      EXPECT_FLOAT_EQ(bsw_zone_speed1.points[ipoint].y, bsw_zone_speed2.points[ipoint].y);

      EXPECT_FLOAT_EQ(bsw_zone_hys_speed1.points[ipoint].x, bsw_zone_hys_speed2.points[ipoint].x);
      EXPECT_FLOAT_EQ(bsw_zone_hys_speed1.points[ipoint].y, bsw_zone_hys_speed2.points[ipoint].y);
   }
}


/**
 * Create bsw scenario with attached trailer. Check that the zone is adjusted for a trailer.
 * \uts{CSCSA-99044} \sdd{SF-6597} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Create_Bsw_Zone__zone_is_adjusted_for_trailer)
{
   /** \arrange Create BSW zones and object with trailer. */
   Fbk_Field_Of_Interest_T bsw_zone{};
   Fbk_Field_Of_Interest_T bsw_zone_hys{};

   lcda_core_input.bsw_zone_calculation_mode          = BSW_ZONE_CALC_FIXED_INPUT;
   (&lcda_cals)->k_bsw_enable_dynspeed_zone           = FBK_FALSE;
   (&lcda_cals)->k_bsw_enable_trailer_zone_extension  = FBK_TRUE;
   (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin = FBK_ZERO_F;

   lcda_cals.k_bsw_f_enable_object_rel_vel_dynzone = FBK_FALSE;

   lcda_core_input.lane_width = 0.0f;
   p_vehicle_data->lane_width = 0.0f;

   lcda_core_input.trailer.f_trailer_present = FBK_TRUE;
   lcda_core_input.trailer.length            = 3.0f;
   lcda_core_input.trailer.width             = 0.5f;

   lcda_core_input.initial_bsw_zone     = default_bsw_zone;
   lcda_core_input.initial_bsw_zone_hys = default_bsw_zone_hys;

   bsw_object.ego_side  = FBK_SIDE_RIGHT;
   tracker_object.index = 0u;

   /** \action Call function Lcda_Create_Bsw_Zone. */
   Lcda_Create_Bsw_Zone(&bsw_zone, &bsw_zone_hys, &bsw_object, &lcda_core_input, &lcda_cals);

   /** \assert Check that zone is modified properly. */

   EXPECT_FLOAT_EQ(bsw_zone.points[REAR_OUTER_SIDE].x,
                   lcda_core_input.initial_bsw_zone.points[REAR_OUTER_SIDE].x - lcda_core_input.trailer.length);
}


/**
 * For fixed input sized BSW zones, the left and right BSW zones should be symmetric along the X-axis. Create BSW zones and check
 * that the Y-coordinates are mirrored. \uts{CSCSA-42588} \sdd{SF-6597} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test,
       Lcda_Create_Bsw_Zone__left_zone_and_right_zone_are_mirrored_along_long_axis_when_BSW_ZONE_CALC_FIXED_INPUT_is_used)
{
   /** \arrange Set parameters for BSW zone creation. Set BSW zone calculation method to BSW_ZONE_CALC_FIXED_INPUT. */
   Fbk_Field_Of_Interest_T zone_left{};
   Fbk_Field_Of_Interest_T zone_hys_left{};

   Fbk_Field_Of_Interest_T zone_right{};
   Fbk_Field_Of_Interest_T zone_hys_right{};
   uint8_t ipoint;

   lcda_core_input.initial_bsw_zone     = default_bsw_zone;
   lcda_core_input.initial_bsw_zone_hys = default_bsw_zone_hys;

   lcda_core_input.warn_settings.bsw_len_factor     = 1.0f;
   lcda_core_input.enabled_flags.f_dropback_enabled = FBK_FALSE;
   lcda_cals.k_bsw_enable_dynspeed_zone             = 0;
   lcda_cals.k_bsw_f_enable_object_rel_vel_dynzone  = FBK_FALSE;
   lcda_core_input.bsw_zone_calculation_mode        = BSW_ZONE_CALC_FIXED_INPUT;

   bsw_object.ego_side  = FBK_SIDE_RIGHT;
   tracker_object.index = 0u;

   /** \action Call Lcda_Create_Bsw_Zone to create the left and right BSW zones, respectively. */
   Lcda_Create_Bsw_Zone(&zone_right, &zone_hys_right, &bsw_object, &lcda_core_input, &lcda_cals);
   bsw_object.ego_side = FBK_SIDE_LEFT;
   Lcda_Create_Bsw_Zone(&zone_left, &zone_hys_left, &bsw_object, &lcda_core_input, &lcda_cals);

   /** \assert Check that zones are mirrored along the X-axis. */
   for (ipoint = 0; ipoint < LCDA_NUMBER_OF_ZONE_POINTS; ipoint++)
   {
      EXPECT_FLOAT_EQ(zone_right.points[ipoint].x, zone_left.points[ipoint].x);
      EXPECT_FLOAT_EQ(zone_right.points[ipoint].y, -zone_left.points[ipoint].y);

      EXPECT_FLOAT_EQ(zone_hys_right.points[ipoint].x, zone_hys_left.points[ipoint].x);
      EXPECT_FLOAT_EQ(zone_hys_right.points[ipoint].y, -zone_hys_left.points[ipoint].y);
   }
}


/**
 * If actived via cal value, the BSW zones can be enlarged when a trailer is attached. Create BSW zones with trailer present and
 * check that the BSW zone size is increased correctly. \uts{CSCSA-42590} \sdd{SF-6587} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Adjust_Bsw_Zone_Size_For_Trailer__zone_is_adjusted)
{
   /** \arrange Create BSW zone with size zero for simple verification. Set trailer present flag. */
   Fbk_Field_Of_Interest_T zone{};
   Fbk_Field_Of_Interest_T zone_hys{};

   Fbk_Field_Of_Interest_T default_zone     = zone;
   Fbk_Field_Of_Interest_T default_zone_hys = zone_hys;

   lcda_core_input.trailer.f_trailer_present                      = FBK_TRUE;
   lcda_core_input.trailer.length                                 = 3.0f;
   (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin             = 1.0f;
   lcda_cals.k_bsw_f_zone_extension_by_diff_width_host_vs_trailer = FBK_FALSE;

   /** \action Call Lcda_Adjust_Bsw_Zone_Size_For_Trailer to enlarge BSW zone sizes. */
   Lcda_Adjust_Bsw_Zone_Size_For_Trailer(&zone, &zone_hys, &lcda_core_input, &lcda_cals, p_vehicle_data);

   /** \assert Check that BSW zone size is as expected. */
   EXPECT_FLOAT_EQ(zone.points[REAR_EGO_SIDE].x, -lcda_core_input.trailer.length - lcda_cals.k_bsw_trailer_zone_ext_safety_margin);
   EXPECT_FLOAT_EQ(zone.points[REAR_OUTER_SIDE].x, -lcda_core_input.trailer.length - lcda_cals.k_bsw_trailer_zone_ext_safety_margin);
   EXPECT_FLOAT_EQ(zone.points[REAR_EGO_SIDE].y, default_zone.points[REAR_EGO_SIDE].y);
   EXPECT_FLOAT_EQ(zone.points[REAR_OUTER_SIDE].y, default_zone.points[REAR_OUTER_SIDE].y);

   EXPECT_FLOAT_EQ(zone_hys.points[REAR_EGO_SIDE].x, -lcda_core_input.trailer.length - (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin
                                                        - (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin_hys);
   EXPECT_FLOAT_EQ(zone_hys.points[REAR_OUTER_SIDE].x, -lcda_core_input.trailer.length - (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin
                                                          - (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin_hys);

   EXPECT_FLOAT_EQ(zone.points[REAR_EGO_SIDE].y, default_zone_hys.points[REAR_EGO_SIDE].y);
   EXPECT_FLOAT_EQ(zone.points[REAR_OUTER_SIDE].y, default_zone_hys.points[REAR_OUTER_SIDE].y);
}

/**
 * If actived via cal value, the BSW zones can be enlarged when a trailer is attached. Create BSW zones with trailer present and
 * check that the BSW zone size is increased correctly. The trailer is narrower than the ego, so the lateral dimension remains the
 * same. \uts{CSCSA-42591} \sdd{SF-6587} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Adjust_Bsw_Zone_Size_For_Trailer__zone_is_adjusted_trailer_is_too_narrow_for_lateral_extension)
{
   /** \arrange Create BSW zone with size zero for simple verification. Set trailer present flag. */
   Fbk_Field_Of_Interest_T zone{};
   Fbk_Field_Of_Interest_T zone_hys{};

   Fbk_Field_Of_Interest_T default_zone     = zone;
   Fbk_Field_Of_Interest_T default_zone_hys = zone_hys;

   lcda_core_input.trailer.f_trailer_present                      = FBK_TRUE;
   lcda_core_input.trailer.length                                 = 3.0f;
   lcda_core_input.trailer.width                                  = 0.5f;
   (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin             = 1.0f;
   (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin             = 0.25f;
   lcda_cals.k_bsw_f_zone_extension_by_diff_width_host_vs_trailer = FBK_TRUE;

   /** \action Call Lcda_Adjust_Bsw_Zone_Size_For_Trailer to enlarge BSW zone sizes. */
   Lcda_Adjust_Bsw_Zone_Size_For_Trailer(&zone, &zone_hys, &lcda_core_input, &lcda_cals, p_vehicle_data);

   /** \assert Check that BSW zone size is as expected. */
   EXPECT_FLOAT_EQ(zone.points[REAR_EGO_SIDE].x, -lcda_core_input.trailer.length - lcda_cals.k_bsw_trailer_zone_ext_safety_margin);
   EXPECT_FLOAT_EQ(zone.points[REAR_OUTER_SIDE].x, -lcda_core_input.trailer.length - lcda_cals.k_bsw_trailer_zone_ext_safety_margin);
   EXPECT_FLOAT_EQ(zone.points[FRONT_EGO_SIDE].y, default_zone.points[FRONT_EGO_SIDE].y);
   EXPECT_FLOAT_EQ(zone.points[MIDDLE_EGO_SIDE].y, default_zone.points[MIDDLE_EGO_SIDE].y);
   EXPECT_FLOAT_EQ(zone.points[REAR_EGO_SIDE].y, default_zone.points[REAR_EGO_SIDE].y);
   EXPECT_FLOAT_EQ(zone.points[FRONT_OUTER_SIDE].y, default_zone.points[FRONT_OUTER_SIDE].y);
   EXPECT_FLOAT_EQ(zone.points[MIDDLE_OUTER_SIDE].y, default_zone.points[MIDDLE_OUTER_SIDE].y);
   EXPECT_FLOAT_EQ(zone.points[REAR_OUTER_SIDE].y, default_zone.points[REAR_OUTER_SIDE].y);

   EXPECT_FLOAT_EQ(zone_hys.points[REAR_EGO_SIDE].x, -lcda_core_input.trailer.length - (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin
                                                        - (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin_hys);
   EXPECT_FLOAT_EQ(zone_hys.points[REAR_OUTER_SIDE].x, -lcda_core_input.trailer.length - (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin
                                                          - (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin_hys);
   EXPECT_FLOAT_EQ(zone.points[FRONT_EGO_SIDE].y, default_zone_hys.points[FRONT_EGO_SIDE].y);
   EXPECT_FLOAT_EQ(zone.points[MIDDLE_EGO_SIDE].y, default_zone_hys.points[MIDDLE_EGO_SIDE].y);
   EXPECT_FLOAT_EQ(zone.points[REAR_EGO_SIDE].y, default_zone_hys.points[REAR_EGO_SIDE].y);
   EXPECT_FLOAT_EQ(zone.points[FRONT_OUTER_SIDE].y, default_zone_hys.points[FRONT_OUTER_SIDE].y);
   EXPECT_FLOAT_EQ(zone.points[MIDDLE_OUTER_SIDE].y, default_zone_hys.points[MIDDLE_OUTER_SIDE].y);
   EXPECT_FLOAT_EQ(zone.points[REAR_OUTER_SIDE].y, default_zone_hys.points[REAR_OUTER_SIDE].y);
}

/**
 * If actived via cal value, the BSW zones can be enlarged when a trailer is attached. Create BSW zones with trailer present and
 * check that the BSW zone size is increased correctly. \uts{CSCSA-42592} \sdd{SF-6587} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Adjust_Bsw_Zone_Size_For_Trailer__long_and_lat_dimension_of_zone_is_adjusted)
{
   /** \arrange Create BSW zone with size zero for simple verification. Set trailer present flag. */
   Fbk_Field_Of_Interest_T zone{};
   Fbk_Field_Of_Interest_T zone_hys{};

   Fbk_Field_Of_Interest_T default_zone     = zone;
   Fbk_Field_Of_Interest_T default_zone_hys = zone_hys;

   lcda_core_input.trailer.f_trailer_present                      = FBK_TRUE;
   lcda_core_input.trailer.length                                 = 5.0f;
   lcda_core_input.trailer.width                                  = 4.0f;
   (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin             = 1.0f;
   (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin_hys         = 0.5f;
   lcda_cals.k_bsw_f_zone_extension_by_diff_width_host_vs_trailer = FBK_TRUE;
   lcda_cals.k_bsw_f_enable_trailer_zone_adjustment_on_outer_side = FBK_TRUE;
   lcda_cals.k_bsw_f_enable_trailer_zone_adjustment_on_ego_side   = FBK_TRUE;

   float32_T width_diff_trailer_vs_host = Fbk_Half(lcda_core_input.trailer.width - p_vehicle_data->host_width);

   /** \action Call Lcda_Adjust_Bsw_Zone_Size_For_Trailer to enlarge BSW zone sizes. */
   Lcda_Adjust_Bsw_Zone_Size_For_Trailer(&zone, &zone_hys, &lcda_core_input, &lcda_cals, p_vehicle_data);

   /** \assert Check that BSW zone size is as expected. */
   EXPECT_FLOAT_EQ(zone.points[REAR_EGO_SIDE].x, -lcda_core_input.trailer.length - lcda_cals.k_bsw_trailer_zone_ext_safety_margin);
   EXPECT_FLOAT_EQ(zone.points[REAR_OUTER_SIDE].x, -lcda_core_input.trailer.length - lcda_cals.k_bsw_trailer_zone_ext_safety_margin);

   EXPECT_FLOAT_EQ(zone.points[FRONT_EGO_SIDE].y, default_zone.points[FRONT_EGO_SIDE].y + width_diff_trailer_vs_host);
   EXPECT_FLOAT_EQ(zone.points[MIDDLE_EGO_SIDE].y, default_zone.points[MIDDLE_EGO_SIDE].y + width_diff_trailer_vs_host);
   EXPECT_FLOAT_EQ(zone.points[REAR_EGO_SIDE].y, default_zone.points[REAR_EGO_SIDE].y + width_diff_trailer_vs_host);
   EXPECT_FLOAT_EQ(zone.points[FRONT_OUTER_SIDE].y, default_zone.points[FRONT_OUTER_SIDE].y + width_diff_trailer_vs_host);
   EXPECT_FLOAT_EQ(zone.points[MIDDLE_OUTER_SIDE].y, default_zone.points[MIDDLE_OUTER_SIDE].y + width_diff_trailer_vs_host);
   EXPECT_FLOAT_EQ(zone.points[REAR_OUTER_SIDE].y, default_zone.points[REAR_OUTER_SIDE].y + width_diff_trailer_vs_host);

   EXPECT_FLOAT_EQ(zone_hys.points[REAR_EGO_SIDE].x, -lcda_core_input.trailer.length - (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin
                                                        - (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin_hys);
   EXPECT_FLOAT_EQ(zone_hys.points[REAR_OUTER_SIDE].x, -lcda_core_input.trailer.length - (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin
                                                          - (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin_hys);

   EXPECT_FLOAT_EQ(zone_hys.points[FRONT_EGO_SIDE].y, default_zone_hys.points[FRONT_EGO_SIDE].y + width_diff_trailer_vs_host);
   EXPECT_FLOAT_EQ(zone_hys.points[MIDDLE_EGO_SIDE].y, default_zone_hys.points[MIDDLE_EGO_SIDE].y + width_diff_trailer_vs_host);
   EXPECT_FLOAT_EQ(zone_hys.points[REAR_EGO_SIDE].y, default_zone_hys.points[REAR_EGO_SIDE].y + width_diff_trailer_vs_host);
   EXPECT_FLOAT_EQ(zone_hys.points[FRONT_OUTER_SIDE].y, default_zone_hys.points[FRONT_OUTER_SIDE].y + width_diff_trailer_vs_host);
   EXPECT_FLOAT_EQ(zone_hys.points[MIDDLE_OUTER_SIDE].y, default_zone_hys.points[MIDDLE_OUTER_SIDE].y + width_diff_trailer_vs_host);
   EXPECT_FLOAT_EQ(zone_hys.points[REAR_OUTER_SIDE].y, default_zone_hys.points[REAR_OUTER_SIDE].y + width_diff_trailer_vs_host);
}

/**
 * If actived via cal value, the BSW zones can be enlarged when a trailer is attached. Create BSW zones with trailer present and
 * check that the BSW zone size is increased correctly. \uts{CSCSA-42593} \sdd{SF-6587} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Adjust_Bsw_Zone_Size_For_Trailer__long_and_lat_dimension_of_zone_is_adjusted_threshold_check)
{
   /** \arrange Create BSW zone with size zero for simple verification. Set trailer present flag. */
   Fbk_Field_Of_Interest_T zone{};
   Fbk_Field_Of_Interest_T zone_hys{};

   Fbk_Field_Of_Interest_T default_zone     = zone;
   Fbk_Field_Of_Interest_T default_zone_hys = zone_hys;

   lcda_core_input.trailer.f_trailer_present                      = FBK_TRUE;
   lcda_core_input.trailer.length                                 = 5.0f;
   lcda_core_input.trailer.width                                  = 10.0f;
   (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin             = 1.0f;
   lcda_cals.k_bsw_f_zone_extension_by_diff_width_host_vs_trailer = FBK_TRUE;
   lcda_cals.k_bsw_f_enable_trailer_zone_adjustment_on_outer_side = FBK_TRUE;
   lcda_cals.k_bsw_f_enable_trailer_zone_adjustment_on_ego_side   = FBK_TRUE;

   float32_T width_diff_trailer_vs_host = lcda_cals.k_bsw_y_width - lcda_cals.k_bsw_trailer_zone_min_width;

   /** \action Call Lcda_Adjust_Bsw_Zone_Size_For_Trailer to enlarge BSW zone sizes. */
   Lcda_Adjust_Bsw_Zone_Size_For_Trailer(&zone, &zone_hys, &lcda_core_input, &lcda_cals, p_vehicle_data);

   /** \assert Check that BSW zone size is as expected. */
   EXPECT_FLOAT_EQ(zone.points[REAR_EGO_SIDE].x, -lcda_core_input.trailer.length - lcda_cals.k_bsw_trailer_zone_ext_safety_margin);
   EXPECT_FLOAT_EQ(zone.points[REAR_OUTER_SIDE].x, -lcda_core_input.trailer.length - lcda_cals.k_bsw_trailer_zone_ext_safety_margin);

   EXPECT_FLOAT_EQ(zone.points[FRONT_EGO_SIDE].y, default_zone.points[FRONT_EGO_SIDE].y + width_diff_trailer_vs_host);
   EXPECT_FLOAT_EQ(zone.points[MIDDLE_EGO_SIDE].y, default_zone.points[MIDDLE_EGO_SIDE].y + width_diff_trailer_vs_host);
   EXPECT_FLOAT_EQ(zone.points[REAR_EGO_SIDE].y, default_zone.points[REAR_EGO_SIDE].y + width_diff_trailer_vs_host);
   EXPECT_FLOAT_EQ(zone.points[FRONT_OUTER_SIDE].y, default_zone.points[FRONT_OUTER_SIDE].y + width_diff_trailer_vs_host);
   EXPECT_FLOAT_EQ(zone.points[MIDDLE_OUTER_SIDE].y, default_zone.points[MIDDLE_OUTER_SIDE].y + width_diff_trailer_vs_host);
   EXPECT_FLOAT_EQ(zone.points[REAR_OUTER_SIDE].y, default_zone.points[REAR_OUTER_SIDE].y + width_diff_trailer_vs_host);

   EXPECT_FLOAT_EQ(zone_hys.points[REAR_EGO_SIDE].x, -lcda_core_input.trailer.length - (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin
                                                        - (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin_hys);
   EXPECT_FLOAT_EQ(zone_hys.points[REAR_OUTER_SIDE].x, -lcda_core_input.trailer.length - (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin
                                                          - (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin_hys);

   EXPECT_FLOAT_EQ(zone_hys.points[FRONT_EGO_SIDE].y, default_zone_hys.points[FRONT_EGO_SIDE].y + width_diff_trailer_vs_host);
   EXPECT_FLOAT_EQ(zone_hys.points[MIDDLE_EGO_SIDE].y, default_zone_hys.points[MIDDLE_EGO_SIDE].y + width_diff_trailer_vs_host);
   EXPECT_FLOAT_EQ(zone_hys.points[REAR_EGO_SIDE].y, default_zone_hys.points[REAR_EGO_SIDE].y + width_diff_trailer_vs_host);
   EXPECT_FLOAT_EQ(zone_hys.points[FRONT_OUTER_SIDE].y, default_zone_hys.points[FRONT_OUTER_SIDE].y + width_diff_trailer_vs_host);
   EXPECT_FLOAT_EQ(zone_hys.points[MIDDLE_OUTER_SIDE].y, default_zone_hys.points[MIDDLE_OUTER_SIDE].y + width_diff_trailer_vs_host);
   EXPECT_FLOAT_EQ(zone_hys.points[REAR_OUTER_SIDE].y, default_zone_hys.points[REAR_OUTER_SIDE].y + width_diff_trailer_vs_host);
}

/**
 * Test zone adjustment for trailer if trailer is not present.
 * \uts{CSCSA-42594} \sdd{SF-6587} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Adjust_Bsw_Zone_Size_For_Trailer__no_trailer)
{
   /** \arrange Create BSW zone with size zero for simple verification. Set trailer present flag. */
   Fbk_Field_Of_Interest_T zone{};
   Fbk_Field_Of_Interest_T zone_hys{};
   Lcda_Get_Initial_Bsw_Zones(&zone, &zone_hys, &bsw_object, &lcda_core_input, p_vehicle_data, &lcda_cals);

   Fbk_Field_Of_Interest_T init_zone     = zone;
   Fbk_Field_Of_Interest_T init_zone_hys = zone_hys;

   lcda_core_input.trailer.f_trailer_present          = FBK_FALSE;
   lcda_core_input.trailer.length                     = 3.0f;
   (&lcda_cals)->k_bsw_trailer_zone_ext_safety_margin = 1.0f;

   /** \action Call Lcda_Adjust_Bsw_Zone_Size_For_Trailer to enlarge BSW zone sizes. */
   Lcda_Adjust_Bsw_Zone_Size_For_Trailer(&zone, &zone_hys, &lcda_core_input, &lcda_cals, p_vehicle_data);

   /** \assert Check that BSW zone size not changed. */
   EXPECT_FLOAT_EQ(zone.points[REAR_EGO_SIDE].x, init_zone.points[REAR_EGO_SIDE].x);
   EXPECT_FLOAT_EQ(zone.points[REAR_OUTER_SIDE].x, init_zone.points[REAR_OUTER_SIDE].x);
   EXPECT_FLOAT_EQ(zone_hys.points[REAR_EGO_SIDE].x, init_zone_hys.points[REAR_EGO_SIDE].x);
   EXPECT_FLOAT_EQ(zone_hys.points[REAR_OUTER_SIDE].x, init_zone_hys.points[REAR_OUTER_SIDE].x);
}


/**
 * Test that BSW zone enlargement at the outer side and rear end of the zone works correctly.
 * \uts{CSCSA-42595} \sdd{SF-6590} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Enlarge_Bsw_Zone__zone_is_correctly_enlarged_at_the_outer_side_and_rear_end)
{
   /** \arrange Set up a zone and specify lateral and longitudinal enlargement. */
   Fbk_Field_Of_Interest_T enlarged_zone                          = default_bsw_zone;
   lcda_cals.k_bsw_f_enable_trailer_zone_adjustment_on_outer_side = FBK_TRUE;
   lcda_cals.k_bsw_f_enable_trailer_zone_adjustment_on_ego_side   = FBK_FALSE;
   float32_T lat_adjustment                                       = 2.0f;
   float32_T lon_adjustment                                       = 1.0f;

   /** \action Call function Lcda_Enlarge_Bsw_Zone. */
   Lcda_Enlarge_Bsw_Zone(&enlarged_zone, lon_adjustment, lat_adjustment, &lcda_cals);

   /** \assert Check that zone is enlargement at the outer side and rear end. */
   EXPECT_FLOAT_EQ(enlarged_zone.points[FRONT_OUTER_SIDE].x, default_bsw_zone.points[FRONT_OUTER_SIDE].x);
   EXPECT_FLOAT_EQ(enlarged_zone.points[MIDDLE_OUTER_SIDE].x, default_bsw_zone.points[MIDDLE_OUTER_SIDE].x);
   EXPECT_FLOAT_EQ(enlarged_zone.points[REAR_OUTER_SIDE].x, default_bsw_zone.points[REAR_OUTER_SIDE].x - lon_adjustment);
   EXPECT_FLOAT_EQ(enlarged_zone.points[REAR_EGO_SIDE].x, default_bsw_zone.points[REAR_EGO_SIDE].x - lon_adjustment);
   EXPECT_FLOAT_EQ(enlarged_zone.points[MIDDLE_EGO_SIDE].x, default_bsw_zone.points[MIDDLE_EGO_SIDE].x);
   EXPECT_FLOAT_EQ(enlarged_zone.points[FRONT_EGO_SIDE].x, default_bsw_zone.points[FRONT_EGO_SIDE].x);

   EXPECT_FLOAT_EQ(enlarged_zone.points[FRONT_OUTER_SIDE].y, default_bsw_zone.points[FRONT_OUTER_SIDE].y + lat_adjustment);
   EXPECT_FLOAT_EQ(enlarged_zone.points[MIDDLE_OUTER_SIDE].y, default_bsw_zone.points[MIDDLE_OUTER_SIDE].y + lat_adjustment);
   EXPECT_FLOAT_EQ(enlarged_zone.points[REAR_OUTER_SIDE].y, default_bsw_zone.points[REAR_OUTER_SIDE].y + lat_adjustment);
   EXPECT_FLOAT_EQ(enlarged_zone.points[REAR_EGO_SIDE].y, default_bsw_zone.points[REAR_EGO_SIDE].y);
   EXPECT_FLOAT_EQ(enlarged_zone.points[MIDDLE_EGO_SIDE].y, default_bsw_zone.points[MIDDLE_EGO_SIDE].y);
   EXPECT_FLOAT_EQ(enlarged_zone.points[FRONT_EGO_SIDE].y, default_bsw_zone.points[FRONT_EGO_SIDE].y);
}

/**
 * Test that BSW zone enlargement at the ego side of the zone works correctly.
 * \uts{CSCSA-42596} \sdd{SF-6590} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Enlarge_Bsw_Zone__zone_is_correctly_enlarged_at_the_ego_side)
{
   /** \arrange Set up a zone and specify lateral and longitudinal enlargement. */
   Fbk_Field_Of_Interest_T enlarged_zone                          = default_bsw_zone;
   lcda_cals.k_bsw_f_enable_trailer_zone_adjustment_on_outer_side = FBK_FALSE;
   lcda_cals.k_bsw_f_enable_trailer_zone_adjustment_on_ego_side   = FBK_TRUE;
   float32_T lat_adjustment                                       = 1.5f;
   float32_T lon_adjustment                                       = 0.5f;

   /** \action Call function Lcda_Enlarge_Bsw_Zone. */
   Lcda_Enlarge_Bsw_Zone(&enlarged_zone, lon_adjustment, lat_adjustment, &lcda_cals);

   /** \assert Check that zone is enlargement at the ego side. */
   EXPECT_FLOAT_EQ(enlarged_zone.points[FRONT_EGO_SIDE].y, default_bsw_zone.points[FRONT_EGO_SIDE].y + lat_adjustment);
   EXPECT_FLOAT_EQ(enlarged_zone.points[MIDDLE_EGO_SIDE].y, default_bsw_zone.points[MIDDLE_EGO_SIDE].y + lat_adjustment);
   EXPECT_FLOAT_EQ(enlarged_zone.points[REAR_EGO_SIDE].y, default_bsw_zone.points[REAR_EGO_SIDE].y + lat_adjustment);
   EXPECT_FLOAT_EQ(enlarged_zone.points[FRONT_OUTER_SIDE].y, default_bsw_zone.points[FRONT_OUTER_SIDE].y);
   EXPECT_FLOAT_EQ(enlarged_zone.points[MIDDLE_OUTER_SIDE].y, default_bsw_zone.points[MIDDLE_OUTER_SIDE].y);
   EXPECT_FLOAT_EQ(enlarged_zone.points[REAR_OUTER_SIDE].y, default_bsw_zone.points[REAR_OUTER_SIDE].y);
}

/**
 * Test that BSW zone enlargement at the outer side and ego side of the zone works correctly.
 * \uts{CSCSA-42597} \sdd{SF-6590} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Enlarge_Bsw_Zone__zone_is_correctly_enlarged_at_the_outer_and_ego_side)
{
   /** \arrange Set up a zone and specify lateral and longitudinal enlargement. */
   Fbk_Field_Of_Interest_T enlarged_zone                          = default_bsw_zone;
   lcda_cals.k_bsw_f_enable_trailer_zone_adjustment_on_outer_side = FBK_TRUE;
   lcda_cals.k_bsw_f_enable_trailer_zone_adjustment_on_ego_side   = FBK_TRUE;
   float32_T lat_adjustment                                       = 3.0f;
   float32_T lon_adjustment                                       = 1.0f;

   /** \action Call function Lcda_Enlarge_Bsw_Zone. */
   Lcda_Enlarge_Bsw_Zone(&enlarged_zone, lon_adjustment, lat_adjustment, &lcda_cals);

   /** \assert Check that zone is enlargement at the ego side. */
   EXPECT_FLOAT_EQ(enlarged_zone.points[FRONT_EGO_SIDE].y, default_bsw_zone.points[FRONT_EGO_SIDE].y + lat_adjustment);
   EXPECT_FLOAT_EQ(enlarged_zone.points[MIDDLE_EGO_SIDE].y, default_bsw_zone.points[MIDDLE_EGO_SIDE].y + lat_adjustment);
   EXPECT_FLOAT_EQ(enlarged_zone.points[REAR_EGO_SIDE].y, default_bsw_zone.points[REAR_EGO_SIDE].y + lat_adjustment);
   EXPECT_FLOAT_EQ(enlarged_zone.points[FRONT_OUTER_SIDE].y, default_bsw_zone.points[FRONT_OUTER_SIDE].y + lat_adjustment);
   EXPECT_FLOAT_EQ(enlarged_zone.points[MIDDLE_OUTER_SIDE].y, default_bsw_zone.points[MIDDLE_OUTER_SIDE].y + lat_adjustment);
   EXPECT_FLOAT_EQ(enlarged_zone.points[REAR_OUTER_SIDE].y, default_bsw_zone.points[REAR_OUTER_SIDE].y + lat_adjustment);
}

/**
 * Test that BSW zone enlargement at the rear end of the zone works correctly.
 * \uts{CSCSA-42598} \sdd{SF-6590} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Enlarge_Bsw_Zone__zone_is_correctly_enlarged_at_the_rear_end)
{
   /** \arrange Set up a zone and specify lateral and longitudinal enlargement. */
   Fbk_Field_Of_Interest_T enlarged_zone                          = default_bsw_zone;
   lcda_cals.k_bsw_f_enable_trailer_zone_adjustment_on_outer_side = FBK_FALSE;
   lcda_cals.k_bsw_f_enable_trailer_zone_adjustment_on_ego_side   = FBK_FALSE;
   float32_T lat_adjustment                                       = 2.0f;
   float32_T lon_adjustment                                       = 1.0f;

   /** \action Call function Lcda_Enlarge_Bsw_Zone. */
   Lcda_Enlarge_Bsw_Zone(&enlarged_zone, lon_adjustment, lat_adjustment, &lcda_cals);

   /** \assert Check that zone is enlargement at the outer side and rear end. */
   EXPECT_FLOAT_EQ(enlarged_zone.points[FRONT_OUTER_SIDE].x, default_bsw_zone.points[FRONT_OUTER_SIDE].x);
   EXPECT_FLOAT_EQ(enlarged_zone.points[MIDDLE_OUTER_SIDE].x, default_bsw_zone.points[MIDDLE_OUTER_SIDE].x);
   EXPECT_FLOAT_EQ(enlarged_zone.points[REAR_OUTER_SIDE].x, default_bsw_zone.points[REAR_OUTER_SIDE].x - lon_adjustment);
   EXPECT_FLOAT_EQ(enlarged_zone.points[REAR_EGO_SIDE].x, default_bsw_zone.points[REAR_EGO_SIDE].x - lon_adjustment);
   EXPECT_FLOAT_EQ(enlarged_zone.points[MIDDLE_EGO_SIDE].x, default_bsw_zone.points[MIDDLE_EGO_SIDE].x);
   EXPECT_FLOAT_EQ(enlarged_zone.points[FRONT_EGO_SIDE].x, default_bsw_zone.points[FRONT_EGO_SIDE].x);

   EXPECT_FLOAT_EQ(enlarged_zone.points[FRONT_OUTER_SIDE].y, default_bsw_zone.points[FRONT_OUTER_SIDE].y);
   EXPECT_FLOAT_EQ(enlarged_zone.points[MIDDLE_OUTER_SIDE].y, default_bsw_zone.points[MIDDLE_OUTER_SIDE].y);
   EXPECT_FLOAT_EQ(enlarged_zone.points[REAR_OUTER_SIDE].y, default_bsw_zone.points[REAR_OUTER_SIDE].y);
   EXPECT_FLOAT_EQ(enlarged_zone.points[REAR_EGO_SIDE].y, default_bsw_zone.points[REAR_EGO_SIDE].y);
   EXPECT_FLOAT_EQ(enlarged_zone.points[MIDDLE_EGO_SIDE].y, default_bsw_zone.points[MIDDLE_EGO_SIDE].y);
   EXPECT_FLOAT_EQ(enlarged_zone.points[FRONT_EGO_SIDE].y, default_bsw_zone.points[FRONT_EGO_SIDE].y);
}

/**
 * Test that dropback factor is applied correctly, if rear of shrinked zone is still behind ego rear bumper.
 * \uts{CSCSA-42599} \sdd{SF-6589} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Apply_Dropback_Factor__shrinkes_zone_correctly_if_rear_of_shrinked_zone_behind_ego_rear)
{
   /** \arrange Set up a zone and specify dropback factor and vehicle length such that rear of shrinked zone is still behind ego
    * rear bumper. */
   default_bsw_zone.points[REAR_OUTER_SIDE].x = -10.0f;
   default_bsw_zone.points[REAR_EGO_SIDE].x   = -10.0f;
   Fbk_Field_Of_Interest_T zone_dropback      = default_bsw_zone;
   float32_T dropback_factor                  = 0.8f;
   float32_T vehicle_length                   = 5.0f;
   lcda_cals.k_bsw_shrink_zone_method         = FBK_FALSE;

   /** \action Call function Lcda_Apply_Dropback_Factor. */
   Lcda_Apply_Dropback_Factor(&zone_dropback, &lcda_cals, dropback_factor, vehicle_length);

   /** \assert Check that zone is shrinked according to dropback factor. */
   EXPECT_FLOAT_EQ(zone_dropback.points[REAR_OUTER_SIDE].x, default_bsw_zone.points[REAR_OUTER_SIDE].x * dropback_factor);
   EXPECT_FLOAT_EQ(zone_dropback.points[REAR_EGO_SIDE].x, default_bsw_zone.points[REAR_EGO_SIDE].x * dropback_factor);
}

/**
 * Test that dropback factor is applied correctly, if rear of shrinked zone is not behind ego rear bumper.
 * \uts{CSCSA-42600} \sdd{SF-6589} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Apply_Dropback_Factor__shrinkes_zone_correctly_if_rear_of_shrinked_zone_not_behind_ego_rear)
{
   /** \arrange Set up a zone and specify dropback factor and vehicle length such that rear of shrinked zone is no longer behind
    * ego rear bumper. */
   default_bsw_zone.points[REAR_OUTER_SIDE].x = -10.0f;
   default_bsw_zone.points[REAR_EGO_SIDE].x   = -10.0f;
   Fbk_Field_Of_Interest_T zone_dropback      = default_bsw_zone;
   float32_T dropback_factor                  = 0.4f;
   float32_T vehicle_length                   = 5.0f;
   lcda_cals.k_bsw_shrink_zone_method         = FBK_FALSE;

   /** \action Call function Lcda_Apply_Dropback_Factor. */
   Lcda_Apply_Dropback_Factor(&zone_dropback, &lcda_cals, dropback_factor, vehicle_length);

   /** \assert Check that zone is shrinked such that rear of zone is at ego rear. */
   EXPECT_FLOAT_EQ(zone_dropback.points[REAR_OUTER_SIDE].x, -vehicle_length);
   EXPECT_FLOAT_EQ(zone_dropback.points[REAR_EGO_SIDE].x, -vehicle_length);
}


/**
 * Test that dropback factor with subtraction mode is applied correctly, if rear of shrinked zone is still behind ego rear bumper.
 * \uts{CSCSA-243956} \sdd{SF-6589} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Apply_Dropback_Factor__shrinkes_zone_correctly_if_rear_of_shrinked_zone_behind_ego_rear_mode2)
{
   /** \arrange Set up a zone and specify dropback factor and vehicle length such that rear of shrinked zone is still behind ego
    * rear bumper. */
   default_bsw_zone.points[REAR_OUTER_SIDE].x = -10.0f;
   default_bsw_zone.points[REAR_EGO_SIDE].x   = -10.0f;
   Fbk_Field_Of_Interest_T zone_dropback      = default_bsw_zone;
   float32_T dropback_factor                  = 0.8f;
   float32_T vehicle_length                   = 5.0f;
   lcda_cals.k_bsw_shrink_zone_method         = FBK_TRUE;

   /** \action Call function Lcda_Apply_Dropback_Factor. */
   Lcda_Apply_Dropback_Factor(&zone_dropback, &lcda_cals, dropback_factor, vehicle_length);

   /** \assert Check that zone is shrinked according to dropback factor. */
   EXPECT_FLOAT_EQ(zone_dropback.points[REAR_OUTER_SIDE].x, default_bsw_zone.points[REAR_OUTER_SIDE].x + dropback_factor);
   EXPECT_FLOAT_EQ(zone_dropback.points[REAR_EGO_SIDE].x, default_bsw_zone.points[REAR_EGO_SIDE].x + dropback_factor);
}

/**
 * Test that dropback factor with subtraction mode is applied correctly, if rear of shrinked zone is not behind ego rear bumper.
 * \uts{CSCSA-243957} \sdd{SF-6589} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Apply_Dropback_Factor__shrinkes_zone_correctly_if_rear_of_shrinked_zone_not_behind_ego_rear_mode2)
{
   /** \arrange Set up a zone and specify dropback factor and vehicle length such that rear of shrinked zone is no longer behind
    * ego rear bumper. */
   default_bsw_zone.points[REAR_OUTER_SIDE].x = -6.0f;
   default_bsw_zone.points[REAR_EGO_SIDE].x   = -6.0f;
   Fbk_Field_Of_Interest_T zone_dropback      = default_bsw_zone;
   float32_T dropback_factor                  = 2.0f;
   float32_T vehicle_length                   = 5.0f;
   lcda_cals.k_bsw_shrink_zone_method         = FBK_TRUE;

   /** \action Call function Lcda_Apply_Dropback_Factor. */
   Lcda_Apply_Dropback_Factor(&zone_dropback, &lcda_cals, dropback_factor, vehicle_length);

   /** \assert Check that zone is shrinked such that rear of zone is at ego rear. */
   EXPECT_FLOAT_EQ(zone_dropback.points[REAR_OUTER_SIDE].x, -vehicle_length);
   EXPECT_FLOAT_EQ(zone_dropback.points[REAR_EGO_SIDE].x, -vehicle_length);
}


/**
 * Check that the initial BSW zones are taken from the core input if calculation method BSW_ZONE_CALC_FIXED_INPUT is chosen.
 * \uts{CSCSA-42601} \sdd{SF-6591} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test,
       Lcda_Get_Initial_Bsw_Zones__uses_zones_from_core_input_for_calculation_method_BSW_ZONE_CALC_FIXED_INPUT)
{
   /** \arrange Set up zone variables to be filled and some values of core input zones. Select zone calculation method
    * BSW_ZONE_CALC_FIXED_INPUT */
   Fbk_Field_Of_Interest_T zone{};
   Fbk_Field_Of_Interest_T zone_hys{};
   lcda_core_input.initial_bsw_zone.size                           = LCDA_NUMBER_OF_ZONE_POINTS;
   lcda_core_input.initial_bsw_zone.points[REAR_EGO_SIDE].x        = -7.0f;
   lcda_core_input.initial_bsw_zone.points[REAR_OUTER_SIDE].y      = 4.0f;
   lcda_core_input.initial_bsw_zone_hys.size                       = LCDA_NUMBER_OF_ZONE_POINTS;
   lcda_core_input.initial_bsw_zone_hys.points[FRONT_OUTER_SIDE].x = 5.0f;
   lcda_core_input.initial_bsw_zone_hys.points[MIDDLE_EGO_SIDE].y  = 6.0f;

   lcda_cals.k_enable_cvw_curve_zone_adaptation = 0u;
   lcda_core_input.bsw_zone_calculation_mode    = BSW_ZONE_CALC_FIXED_INPUT;

   bsw_object.ego_side = FBK_SIDE_RIGHT;

   /** \action Call Lcda_Get_Initial_Bsw_Zones to create zones based on chosen calculation method. */
   Lcda_Get_Initial_Bsw_Zones(&zone, &zone_hys, &bsw_object, &lcda_core_input, p_vehicle_data, &lcda_cals);

   /** \assert Check that zones are taken from core input by checking the explicitly defined values. */
   EXPECT_FLOAT_EQ(zone.points[REAR_EGO_SIDE].x, lcda_core_input.initial_bsw_zone.points[REAR_EGO_SIDE].x);
   EXPECT_FLOAT_EQ(zone.points[REAR_OUTER_SIDE].y, lcda_core_input.initial_bsw_zone.points[REAR_OUTER_SIDE].y);
   EXPECT_FLOAT_EQ(zone_hys.points[FRONT_OUTER_SIDE].x, lcda_core_input.initial_bsw_zone_hys.points[FRONT_OUTER_SIDE].x);
   EXPECT_FLOAT_EQ(zone_hys.points[MIDDLE_EGO_SIDE].y, lcda_core_input.initial_bsw_zone_hys.points[MIDDLE_EGO_SIDE].y);
}

/**
 * Check that the initial BSW zones are taken from the core input and take lane width and similar into account if calculation
 * method BSW_ZONE_CALC_FIXED_ZONE_VCS is chosen. \uts{CSCSA-42602} \sdd{SF-6591} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test,
       Lcda_Get_Initial_Bsw_Zones__uses_zones_from_core_input_for_calculation_method_BSW_ZONE_CALC_FIXED_ZONE_VCS)
{
   /** \arrange Set up zone variables to be filled and some values of core input zones. Select zone calculation method
    * BSW_ZONE_CALC_FIXED_ZONE_VCS */
   Fbk_Field_Of_Interest_T zone{};
   Fbk_Field_Of_Interest_T zone_hys{};
   lcda_core_input.initial_bsw_zone.size                           = LCDA_NUMBER_OF_ZONE_POINTS;
   lcda_core_input.initial_bsw_zone.points[REAR_EGO_SIDE].x        = -7.0f;
   lcda_core_input.initial_bsw_zone.points[REAR_OUTER_SIDE].y      = 4.0f;
   lcda_core_input.initial_bsw_zone_hys.size                       = LCDA_NUMBER_OF_ZONE_POINTS;
   lcda_core_input.initial_bsw_zone_hys.points[FRONT_OUTER_SIDE].x = 5.0f;
   lcda_core_input.initial_bsw_zone_hys.points[MIDDLE_EGO_SIDE].y  = 6.0f;

   lcda_cals.k_enable_cvw_curve_zone_adaptation = 0u;
   lcda_core_input.bsw_zone_calculation_mode    = BSW_ZONE_CALC_FIXED_ZONE_VCS;

   bsw_object.ego_side = FBK_SIDE_RIGHT;

   /** \action Call Lcda_Get_Initial_Bsw_Zones to create zones based on chosen calculation method. */
   Lcda_Get_Initial_Bsw_Zones(&zone, &zone_hys, &bsw_object, &lcda_core_input, p_vehicle_data, &lcda_cals);

   /** \assert Check that zones are modified correctly based on the input data. */
   EXPECT_LE(zone.points[REAR_EGO_SIDE].x, lcda_core_input.initial_bsw_zone.points[REAR_EGO_SIDE].x);
   EXPECT_GE(zone.points[REAR_OUTER_SIDE].y, lcda_core_input.initial_bsw_zone.points[REAR_OUTER_SIDE].y);
   EXPECT_LE(zone_hys.points[FRONT_OUTER_SIDE].x, lcda_core_input.initial_bsw_zone_hys.points[FRONT_OUTER_SIDE].x);
   EXPECT_LE(zone_hys.points[MIDDLE_EGO_SIDE].y, lcda_core_input.initial_bsw_zone_hys.points[MIDDLE_EGO_SIDE].y);
}

/**
 * Check that the initial BSW zones are taken from the vehicle dimension and lane width information if calculation method is
 * BSW_ZONE_CALC_DEFAULT (same for BSW_ZONE_CALC_VL_LW). \uts{CSCSA-42603} \sdd{SF-6591} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Get_Initial_Bsw_Zones__uses_zones_from_core_input_for_calculation_method_BSW_ZONE_CALC_DEFAULT)
{
   /** \arrange Set up zone variables to be filled and some values of core input zones. Select zone calculation method
    * BSW_ZONE_CALC_VL_LW */
   Fbk_Field_Of_Interest_T zone{};
   Fbk_Field_Of_Interest_T zone_hys{};
   lcda_core_input.initial_bsw_zone.size                           = LCDA_NUMBER_OF_ZONE_POINTS;
   lcda_core_input.initial_bsw_zone.points[REAR_EGO_SIDE].x        = -7.0f;
   lcda_core_input.initial_bsw_zone.points[REAR_OUTER_SIDE].y      = 4.0f;
   lcda_core_input.initial_bsw_zone_hys.size                       = LCDA_NUMBER_OF_ZONE_POINTS;
   lcda_core_input.initial_bsw_zone_hys.points[FRONT_OUTER_SIDE].x = 5.0f;
   lcda_core_input.initial_bsw_zone_hys.points[MIDDLE_EGO_SIDE].y  = 6.0f;

   lcda_cals.k_enable_cvw_curve_zone_adaptation = 0u;
   lcda_core_input.bsw_zone_calculation_mode    = BSW_ZONE_CALC_VL_LW;

   bsw_object.ego_side = FBK_SIDE_RIGHT;

   /** \action Call Lcda_Get_Initial_Bsw_Zones to create zones based on chosen calculation method. */
   Lcda_Get_Initial_Bsw_Zones(&zone, &zone_hys, &bsw_object, &lcda_core_input, p_vehicle_data, &lcda_cals);

   /** \assert Check that zones are modified correctly based on the input data. */
   EXPECT_LE(zone.points[REAR_EGO_SIDE].x, lcda_core_input.initial_bsw_zone.points[REAR_EGO_SIDE].x);
   EXPECT_GE(zone.points[REAR_OUTER_SIDE].y, lcda_core_input.initial_bsw_zone.points[REAR_OUTER_SIDE].y);
   EXPECT_LE(zone_hys.points[FRONT_OUTER_SIDE].x, lcda_core_input.initial_bsw_zone_hys.points[FRONT_OUTER_SIDE].x);
   EXPECT_LE(zone_hys.points[MIDDLE_EGO_SIDE].y, lcda_core_input.initial_bsw_zone_hys.points[MIDDLE_EGO_SIDE].y);
}

/**
 * Check that zone is adjusted according to value from lookup table if factor based host speed adjustment is disabled.
 * \uts{CSCSA-187413} \sdd{SF-6588} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Adjust_Zones_For_Ego_Speed__zone_adapted_with_lookup_table_if_factor_based_approach_disabled)
{
   /** \arrange Set up zones, lookup table and calibration values such that adjustment is known and factor based host speed
    * adjustment is disabled */
   Fbk_Field_Of_Interest_T zone     = default_bsw_zone;
   Fbk_Field_Of_Interest_T zone_hys = default_bsw_zone_hys;
   float32_T ego_length             = 5.0f;
   float32_T ego_abs_speed          = 60.0f;
   float32_T adjustment             = -6.0f;

   lcda_cals.k_bsw_dynzone_speed[0] = 0.0f;
   lcda_cals.k_bsw_dynzone_speed[1] = 20.0f;
   lcda_cals.k_bsw_dynzone_speed[2] = 40.0f;
   lcda_cals.k_bsw_dynzone_speed[3] = 60.0f;
   lcda_cals.k_bsw_dynzone_speed[4] = 80.0f;
   lcda_cals.k_bsw_dynzone_speed[5] = 100.0f;

   lcda_cals.k_bsw_dynzone_range[0u] = 0.0f;
   lcda_cals.k_bsw_dynzone_range[1u] = 2.0f;
   lcda_cals.k_bsw_dynzone_range[2u] = 4.0f;
   lcda_cals.k_bsw_dynzone_range[3u] = adjustment;
   lcda_cals.k_bsw_dynzone_range[4u] = 8.0f;
   lcda_cals.k_bsw_dynzone_range[5u] = 10.0f;

   lcda_cals.k_bsw_enable_factor_based_host_speed_adjustment = 0u;

   /** \action Call Lcda_Adjust_Zones_For_Ego_Speed to adjust zones according to ego speed. */
   Lcda_Adjust_Zones_For_Ego_Speed(&zone, &zone_hys, ego_length, ego_abs_speed, &lcda_cals);

   /** \assert Check that zone rear points are adjusted by values from lookup table. */
   EXPECT_FLOAT_EQ(zone.points[REAR_EGO_SIDE].x, default_bsw_zone.points[REAR_EGO_SIDE].x + adjustment);
   EXPECT_FLOAT_EQ(zone.points[REAR_OUTER_SIDE].x, default_bsw_zone.points[REAR_OUTER_SIDE].x + adjustment);
   EXPECT_FLOAT_EQ(zone_hys.points[REAR_EGO_SIDE].x, default_bsw_zone_hys.points[REAR_EGO_SIDE].x + adjustment);
   EXPECT_FLOAT_EQ(zone_hys.points[REAR_OUTER_SIDE].x, default_bsw_zone_hys.points[REAR_OUTER_SIDE].x + adjustment);
}

/**
 * Check that zone is shortened according to value from lookup table.
 * \uts{CSCSA-185763} \sdd{SF-6588} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Adjust_Zones_For_Ego_Speed__zone_is_shortened_according_to_value_from_lookup_table)
{
   /** \arrange Set up zones, lookup table and calibration values such that adjustment is known and factor based host speed
    * adjustment is disabled */
   Fbk_Field_Of_Interest_T zone     = default_bsw_zone;
   Fbk_Field_Of_Interest_T zone_hys = default_bsw_zone_hys;
   float32_T ego_length             = 5.0f;
   float32_T ego_abs_speed          = 2.0f;
   float32_T adjustment             = 1.0f;

   lcda_cals.k_bsw_dynzone_speed[0] = 5.0f;
   lcda_cals.k_bsw_dynzone_speed[1] = 20.0f;
   lcda_cals.k_bsw_dynzone_speed[2] = 40.0f;
   lcda_cals.k_bsw_dynzone_speed[3] = 60.0f;
   lcda_cals.k_bsw_dynzone_speed[4] = 80.0f;
   lcda_cals.k_bsw_dynzone_speed[5] = 100.0f;

   lcda_cals.k_bsw_dynzone_range[0] = adjustment;
   lcda_cals.k_bsw_dynzone_range[1] = -2.0f;
   lcda_cals.k_bsw_dynzone_range[2] = -4.0f;
   lcda_cals.k_bsw_dynzone_range[3] = -6.0f;
   lcda_cals.k_bsw_dynzone_range[4] = -8.0f;
   lcda_cals.k_bsw_dynzone_range[5] = -10.0f;

   lcda_cals.k_bsw_enable_factor_based_host_speed_adjustment = 1u;

   /** \action Call Lcda_Adjust_Zones_For_Ego_Speed to adjust zones according to ego speed. */
   Lcda_Adjust_Zones_For_Ego_Speed(&zone, &zone_hys, ego_length, ego_abs_speed, &lcda_cals);

   /** \assert Check that zone rear points are adjusted by values from lookup table. */
   EXPECT_FLOAT_EQ(zone.points[REAR_EGO_SIDE].x, default_bsw_zone.points[REAR_EGO_SIDE].x + adjustment);
   EXPECT_FLOAT_EQ(zone.points[REAR_OUTER_SIDE].x, default_bsw_zone.points[REAR_OUTER_SIDE].x + adjustment);
   EXPECT_FLOAT_EQ(zone_hys.points[REAR_EGO_SIDE].x, default_bsw_zone_hys.points[REAR_EGO_SIDE].x + adjustment);
   EXPECT_FLOAT_EQ(zone_hys.points[REAR_OUTER_SIDE].x, default_bsw_zone_hys.points[REAR_OUTER_SIDE].x + adjustment);
}

/**
 * Check if the zone is longer than the default, taking into account the host speed.
 * \uts{CSCSA-185764} \sdd{SF-6588} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Adjust_Zones_For_Ego_Speed__zone_is_longer_based_on_host_speed)
{
   /** \arrange Set up zones, lookup table and calibration values such that adjustment is known and factor based host speed
    * adjustment is disabled */
   Fbk_Field_Of_Interest_T zone     = default_bsw_zone;
   Fbk_Field_Of_Interest_T zone_hys = default_bsw_zone_hys;
   float32_T ego_length             = 5.0f;
   float32_T ego_abs_speed          = 60.0f;

   lcda_cals.k_bsw_enable_factor_based_host_speed_adjustment = 1u;

   /** \action Call Lcda_Adjust_Zones_For_Ego_Speed to adjust zones according to ego speed. */
   Lcda_Adjust_Zones_For_Ego_Speed(&zone, &zone_hys, ego_length, ego_abs_speed, &lcda_cals);

   /** \assert Check that zone rear points are adjusted, zone is bigger than defualt. */
   EXPECT_TRUE(zone.points[REAR_EGO_SIDE].x < default_bsw_zone.points[REAR_EGO_SIDE].x);
   EXPECT_TRUE(zone.points[REAR_OUTER_SIDE].x < default_bsw_zone.points[REAR_OUTER_SIDE].x);
   EXPECT_TRUE(zone_hys.points[REAR_EGO_SIDE].x < default_bsw_zone_hys.points[REAR_EGO_SIDE].x);
   EXPECT_TRUE(zone_hys.points[REAR_OUTER_SIDE].x < default_bsw_zone_hys.points[REAR_OUTER_SIDE].x);
}


/**
 * Create BSW initial zone based on vehicle length and lane width. Set vehicle width to zero and check that a minimal vehicle width
 * as defined by calibration parameters is used to create the zone. Core lane width is lower than calibration. \uts{CSCSA-42606}
 * \sdd{SF-6593} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Get_Vl_Lw_Based_Initial_Bsw_Zones__core_lane_width_lower_than_calibration)
{
   /** \arrange Set up zone parameters for zone creation. */
   lcda_core_input.bsw_zone_calculation_mode = BSW_ZONE_CALC_VL_LW;
   lcda_core_input.lane_width                = lcda_cals.k_lcda_min_lane_width - EPSILON;
   p_vehicle_data->host_width                = 0.0f;
   lcda_cals.k_bsw_lateral_distance_zone     = 0.0f; // So that point 5
                                                     // starts
                                                     // immediately at
                                                     // the vehicle edge

   Fbk_Field_Of_Interest_T bsw_zone{};
   Fbk_Field_Of_Interest_T bsw_zone_hys{};

   bsw_object.ego_side  = FBK_SIDE_RIGHT;
   tracker_object.index = 0u;
   tracker_object.width = 3.0f;
   /** \action Call Lcda_Get_Vl_Lw_Based_Initial_Bsw_Zones to create zone. */
   Lcda_Get_Vl_Lw_Based_Initial_Bsw_Zones(&bsw_zone, &bsw_zone_hys, &bsw_object, &lcda_core_input, p_vehicle_data, &lcda_cals);

   /** \assert Check for representative coordinate that it was computed correctly. */
   EXPECT_FLOAT_EQ(bsw_zone.points[5].y, lcda_cals.k_lcda_min_ego_vehicle_width * 0.5f);
}


/**
 * The BSW zone shall not be shrunk if dynamic BSW zones are disabled. Create slowly backfalling target object such that the zone
 * should be shrunk, but disable dynamic BSW zones and set positive vehicle speed. Check that the zone is not altered.
 * \uts{CSCSA-211375} \sdd{SF-6597} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Create_Bsw_Zone__zone_is_not_adjusted_for_ego_positive_speed)
{
   /** \arrange Create BSW zones and slowly backfalling target object. Disable dynamic BSW zones. */
   Fbk_Field_Of_Interest_T bsw_zone_speed1{};
   Fbk_Field_Of_Interest_T bsw_zone_hys_speed1{};

   Fbk_Field_Of_Interest_T bsw_zone_speed2{};
   Fbk_Field_Of_Interest_T bsw_zone_hys_speed2{};

   uint8_t ipoint;

   lcda_core_input.bsw_zone_calculation_mode       = BSW_ZONE_CALC_FIXED_INPUT;
   lcda_cals.k_bsw_enable_dynspeed_zone            = FBK_FALSE;
   lcda_cals.k_bsw_enable_trailer_zone_extension   = FBK_FALSE;
   lcda_cals.k_bsw_f_enable_object_rel_vel_dynzone = FBK_FALSE;

   lcda_core_input.lane_width = 0.0f;
   p_vehicle_data->lane_width = 0.0f;

   // Set default zone points in the core input
   lcda_core_input.initial_bsw_zone     = default_bsw_zone;
   lcda_core_input.initial_bsw_zone_hys = default_bsw_zone_hys;

   bsw_object.ego_side  = FBK_SIDE_RIGHT;
   tracker_object.index = 0u;

   // First create the zone for the given ego speed
   Lcda_Create_Bsw_Zone(&bsw_zone_speed1, &bsw_zone_hys_speed1, &bsw_object, &lcda_core_input, &lcda_cals);

   // Now quarter the ego speed and then create the BSW zones again. The zones for speed1 and speed2 should be identical
   p_vehicle_data->host_speed = p_vehicle_data->host_speed * 0.25f;

   /** \action Call Lcda_Shrink_Bsw_Zone_Dropback_Obj to shrink BSW zone. */
   Lcda_Create_Bsw_Zone(&bsw_zone_speed2, &bsw_zone_hys_speed2, &bsw_object, &lcda_core_input, &lcda_cals);

   /** \assert Check that zone is not modified. */
   for (ipoint = 0; ipoint < LCDA_NUMBER_OF_ZONE_POINTS; ipoint++)
   {
      EXPECT_FLOAT_EQ(bsw_zone_speed1.points[ipoint].x, bsw_zone_speed2.points[ipoint].x);
      EXPECT_FLOAT_EQ(bsw_zone_speed1.points[ipoint].y, bsw_zone_speed2.points[ipoint].y);

      EXPECT_FLOAT_EQ(bsw_zone_hys_speed1.points[ipoint].x, bsw_zone_hys_speed2.points[ipoint].x);
      EXPECT_FLOAT_EQ(bsw_zone_hys_speed1.points[ipoint].y, bsw_zone_hys_speed2.points[ipoint].y);
   }
}


/**
 * Check if the zone is longer than the default, taking into account the host speed.
 * \uts{CSCSA-211376} \sdd{SF-6588} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Adjust_Zones_For_Ego_Speed__zone_is_longer_based_on_host_speed2)
{
   /** \arrange Set up zones, lookup table and calibration values such that adjustment is known and factor based host speed
    * adjustment is disabled */
   Fbk_Field_Of_Interest_T zone     = default_bsw_zone;
   Fbk_Field_Of_Interest_T zone_hys = default_bsw_zone_hys;
   float32_T ego_length             = -1.0f;
   float32_T ego_abs_speed          = 60.0f;

   zone.points[2].x = 0;
   zone.points[3].x = 0;

   lcda_cals.k_bsw_enable_factor_based_host_speed_adjustment = 1u;

   lcda_cals.k_bsw_dynzone_speed[0] = 0.0f;
   lcda_cals.k_bsw_dynzone_speed[1] = 17.36f;
   lcda_cals.k_bsw_dynzone_speed[2] = 34.72f;
   lcda_cals.k_bsw_dynzone_speed[3] = 52.08f;
   lcda_cals.k_bsw_dynzone_speed[4] = 69.45f;
   lcda_cals.k_bsw_dynzone_speed[5] = 100.0f;

   lcda_cals.k_bsw_dynzone_range[0] = 0.0f;
   lcda_cals.k_bsw_dynzone_range[1] = -5.0f;
   lcda_cals.k_bsw_dynzone_range[2] = -8.5f;
   lcda_cals.k_bsw_dynzone_range[3] = -12.0f;
   lcda_cals.k_bsw_dynzone_range[4] = -15.0f;
   lcda_cals.k_bsw_dynzone_range[5] = -15.0f;

   /** \action Call Lcda_Adjust_Zones_For_Ego_Speed to adjust zones according to ego speed. */
   Lcda_Adjust_Zones_For_Ego_Speed(&zone, &zone_hys, ego_length, ego_abs_speed, &lcda_cals);

   /** \assert Check that zone rear points are adjusted, zone is bigger than defualt. */
   EXPECT_TRUE(zone.points[REAR_EGO_SIDE].x < default_bsw_zone.points[REAR_EGO_SIDE].x);
   EXPECT_TRUE(zone.points[REAR_OUTER_SIDE].x < default_bsw_zone.points[REAR_OUTER_SIDE].x);
   EXPECT_TRUE(zone_hys.points[REAR_EGO_SIDE].x < default_bsw_zone_hys.points[REAR_EGO_SIDE].x);
   EXPECT_TRUE(zone_hys.points[REAR_OUTER_SIDE].x < default_bsw_zone_hys.points[REAR_OUTER_SIDE].x);
}


/**
 * Check that the initial BSW zones are taken from the core input if calculation method BSW_ZONE_CALC_FIXED_INPUT is chosen.
 * \uts{CSCSA-211377} \sdd{SF-6591} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test,
       Lcda_Get_Initial_Bsw_Zones__uses_zones_from_core_input_for_calculation_method_BSW_ZONE_CALC_FIXED_INPUT2)
{
   /** \arrange Set up zone variables to be filled and some values of core input zones. Select zone calculation method
    * BSW_ZONE_CALC_FIXED_INPUT */
   Fbk_Field_Of_Interest_T zone{};
   Fbk_Field_Of_Interest_T zone_hys{};
   lcda_core_input.initial_bsw_zone.size                           = LCDA_NUMBER_OF_ZONE_POINTS;
   lcda_core_input.initial_bsw_zone.points[REAR_EGO_SIDE].x        = -7.0f;
   lcda_core_input.initial_bsw_zone.points[REAR_OUTER_SIDE].y      = 4.0f;
   lcda_core_input.initial_bsw_zone_hys.size                       = LCDA_NUMBER_OF_ZONE_POINTS;
   lcda_core_input.initial_bsw_zone_hys.points[FRONT_OUTER_SIDE].x = 5.0f;
   lcda_core_input.initial_bsw_zone_hys.points[MIDDLE_EGO_SIDE].y  = 6.0f;

   lcda_cals.k_enable_cvw_curve_zone_adaptation = 0u;
   lcda_core_input.bsw_zone_calculation_mode    = BSW_ZONE_CALC_FIXED_INPUT;

   bsw_object.ego_side = FBK_SIDE_RIGHT;

   p_vehicle_data->host_length = 10.0f;

   /** \action Call Lcda_Get_Initial_Bsw_Zones to create zones based on chosen calculation method. */
   Lcda_Get_Initial_Bsw_Zones(&zone, &zone_hys, &bsw_object, &lcda_core_input, p_vehicle_data, &lcda_cals);

   /** \assert Check that zones are taken from core input by checking the explicitly defined values. */
   EXPECT_FLOAT_EQ(zone.points[REAR_EGO_SIDE].x, -p_vehicle_data->host_length);
   EXPECT_FLOAT_EQ(zone.points[REAR_OUTER_SIDE].y, lcda_core_input.initial_bsw_zone.points[REAR_OUTER_SIDE].y);
   EXPECT_FLOAT_EQ(zone_hys.points[FRONT_OUTER_SIDE].x, lcda_core_input.initial_bsw_zone_hys.points[FRONT_OUTER_SIDE].x);
   EXPECT_FLOAT_EQ(zone_hys.points[MIDDLE_EGO_SIDE].y, lcda_core_input.initial_bsw_zone_hys.points[MIDDLE_EGO_SIDE].y);
}


/**
 * Create BSW initial zone based on fixed calibration values.
 * \uts{CSCSA-211378} \sdd{SF-6927} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Get_Vcs_Fixed_Initial_Bsw_Zones__zones_are_set_correctly_based_on_cal_values_case2)
{
   /** \arrange Set up zone parameters for zone creation. */
   Fbk_Field_Of_Interest_T bsw_zone{};
   Fbk_Field_Of_Interest_T bsw_zone_hys{};
   float32_T vehicle_length = 4.0f;
   float32_T object_width   = 2.0f;

   lcda_cals.k_bsw_fixed_zone_y_hys[0] = 5.0f;
   lcda_cals.k_bsw_fixed_zone_y_hys[1] = 5.0f;
   lcda_cals.k_bsw_fixed_zone_y_hys[2] = 5.0f;
   lcda_cals.k_bsw_fixed_zone_y_hys[3] = 5.0f;
   lcda_cals.k_bsw_fixed_zone_y_hys[4] = 5.0f;
   lcda_cals.k_bsw_fixed_zone_y_hys[5] = 5.0f;

   /** \action Call Lcda_Get_Vl_Lw_Based_Initial_Bsw_Zones to create zone. */
   Lcda_Get_Vcs_Fixed_Initial_Bsw_Zones(&bsw_zone, &bsw_zone_hys, vehicle_length, object_width, &lcda_cals);

   /** \assert Check that all coordinates were computed correctly. */
   EXPECT_FLOAT_EQ(bsw_zone.points[FRONT_OUTER_SIDE].x, -(vehicle_length * lcda_cals.k_bsw_fixed_zone_x[FRONT_EGO_SIDE]));
   EXPECT_FLOAT_EQ(bsw_zone.points[MIDDLE_OUTER_SIDE].x, -(vehicle_length) + lcda_cals.k_bsw_fixed_zone_x[MIDDLE_OUTER_SIDE]);
   EXPECT_FLOAT_EQ(bsw_zone.points[REAR_OUTER_SIDE].x, -(vehicle_length) + lcda_cals.k_bsw_fixed_zone_x[REAR_OUTER_SIDE]);
   EXPECT_FLOAT_EQ(bsw_zone.points[REAR_EGO_SIDE].x, -(vehicle_length) + lcda_cals.k_bsw_fixed_zone_x[REAR_EGO_SIDE]);
   EXPECT_FLOAT_EQ(bsw_zone.points[MIDDLE_EGO_SIDE].x, -(vehicle_length) + lcda_cals.k_bsw_fixed_zone_x[MIDDLE_EGO_SIDE]);
   EXPECT_FLOAT_EQ(bsw_zone.points[FRONT_EGO_SIDE].x, -(vehicle_length * lcda_cals.k_bsw_fixed_zone_x[FRONT_EGO_SIDE]));

   for (uint8_t i = 0; i < LCDA_NUMBER_OF_ZONE_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(bsw_zone_hys.points[i].x, bsw_zone.points[i].x + lcda_cals.k_bsw_fixed_zone_x_hys[i]);

      EXPECT_FLOAT_EQ(bsw_zone.points[i].y, lcda_cals.k_bsw_fixed_zone_y[i]);

      EXPECT_FLOAT_EQ(bsw_zone_hys.points[i].y, bsw_zone.points[i].y + lcda_cals.k_bsw_zone_y_hys_max);
   }
}


/**
 * Create BSW initial zone based on fixed calibration values.
 * \uts{CSCSA-211379} \sdd{SF-6927} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Get_Vcs_Fixed_Initial_Bsw_Zones__zones_are_set_correctly_based_on_cal_values_case3)
{
   /** \arrange Set up zone parameters for zone creation. */
   Fbk_Field_Of_Interest_T bsw_zone{};
   Fbk_Field_Of_Interest_T bsw_zone_hys{};
   float32_T vehicle_length = 4.0f;
   float32_T object_width   = 0.01f;

   lcda_cals.k_bsw_fixed_zone_y_hys[0] = 5.0f;
   lcda_cals.k_bsw_fixed_zone_y_hys[1] = 5.0f;
   lcda_cals.k_bsw_fixed_zone_y_hys[2] = 5.0f;
   lcda_cals.k_bsw_fixed_zone_y_hys[3] = 5.0f;
   lcda_cals.k_bsw_fixed_zone_y_hys[4] = 5.0f;
   lcda_cals.k_bsw_fixed_zone_y_hys[5] = 5.0f;

   /** \action Call Lcda_Get_Vl_Lw_Based_Initial_Bsw_Zones to create zone. */
   Lcda_Get_Vcs_Fixed_Initial_Bsw_Zones(&bsw_zone, &bsw_zone_hys, vehicle_length, object_width, &lcda_cals);

   /** \assert Check that all coordinates were computed correctly. */
   EXPECT_FLOAT_EQ(bsw_zone.points[FRONT_OUTER_SIDE].x, -(vehicle_length * lcda_cals.k_bsw_fixed_zone_x[FRONT_EGO_SIDE]));
   EXPECT_FLOAT_EQ(bsw_zone.points[MIDDLE_OUTER_SIDE].x, -(vehicle_length) + lcda_cals.k_bsw_fixed_zone_x[MIDDLE_OUTER_SIDE]);
   EXPECT_FLOAT_EQ(bsw_zone.points[REAR_OUTER_SIDE].x, -(vehicle_length) + lcda_cals.k_bsw_fixed_zone_x[REAR_OUTER_SIDE]);
   EXPECT_FLOAT_EQ(bsw_zone.points[REAR_EGO_SIDE].x, -(vehicle_length) + lcda_cals.k_bsw_fixed_zone_x[REAR_EGO_SIDE]);
   EXPECT_FLOAT_EQ(bsw_zone.points[MIDDLE_EGO_SIDE].x, -(vehicle_length) + lcda_cals.k_bsw_fixed_zone_x[MIDDLE_EGO_SIDE]);
   EXPECT_FLOAT_EQ(bsw_zone.points[FRONT_EGO_SIDE].x, -(vehicle_length * lcda_cals.k_bsw_fixed_zone_x[FRONT_EGO_SIDE]));

   for (uint8_t i = 0; i < LCDA_NUMBER_OF_ZONE_POINTS; i++)
   {
      EXPECT_FLOAT_EQ(bsw_zone_hys.points[i].x, bsw_zone.points[i].x + lcda_cals.k_bsw_fixed_zone_x_hys[i]);

      EXPECT_FLOAT_EQ(bsw_zone.points[i].y, lcda_cals.k_bsw_fixed_zone_y[i]);

      EXPECT_FLOAT_EQ(bsw_zone_hys.points[i].y, bsw_zone.points[i].y + lcda_cals.k_bsw_zone_y_hys_min);
   }
}


/**
 * Create BSW initial zone based on vehicle length and lane width. Set vehicle width to zero and check that a minimal vehicle width
 * as defined by calibration parameters is used to create the zone. Core lane width is lower than calibration. \uts{CSCSA-211380}
 * \sdd{SF-6593} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Lcda_Create_Bsw_Zone_Test, Lcda_Get_Vl_Lw_Based_Initial_Bsw_Zones__core_lane_width_lower_than_calibration_case2)
{
   /** \arrange Set up zone parameters for zone creation. */
   lcda_core_input.bsw_zone_calculation_mode = BSW_ZONE_CALC_VL_LW;
   lcda_core_input.lane_width                = lcda_cals.k_lcda_min_lane_width - EPSILON;
   p_vehicle_data->host_width                = 0.0f;
   lcda_cals.k_bsw_lateral_distance_zone     = 0.0f; // So that point 5
                                                     // starts
                                                     // immediately at
                                                     // the vehicle edge

   lcda_cals.k_bsw_zone_y_hys_max = -1.0f;
   Fbk_Field_Of_Interest_T bsw_zone{};
   Fbk_Field_Of_Interest_T bsw_zone_hys{};

   bsw_object.ego_side  = FBK_SIDE_RIGHT;
   tracker_object.index = 0u;
   tracker_object.width = 3.0f;
   /** \action Call Lcda_Get_Vl_Lw_Based_Initial_Bsw_Zones to create zone. */
   Lcda_Get_Vl_Lw_Based_Initial_Bsw_Zones(&bsw_zone, &bsw_zone_hys, &bsw_object, &lcda_core_input, p_vehicle_data, &lcda_cals);

   /** \assert Check for representative coordinate that it was computed correctly. */
   EXPECT_FLOAT_EQ(bsw_zone.points[5].y, lcda_cals.k_lcda_min_ego_vehicle_width * 0.5f);
}