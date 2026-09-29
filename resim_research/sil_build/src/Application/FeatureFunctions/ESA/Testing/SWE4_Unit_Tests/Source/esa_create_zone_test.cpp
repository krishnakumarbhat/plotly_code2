/**
 * @file esa_create_zone_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for esa_create_zone.c functions
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-123880}
 */

#include "esa_create_zone_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "esa_create_zone.c"
#include "fbk_macros.h"
#include "pa_reuse.h"
}


/**
 * Check that the Y-coordinates are mirrored.
 * \uts{CSCSA-123881} \sdd{CSCSA-87218} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Create_Zone_Test, Esa_Mirror_Zone_Across_Long_Axis__left_zone_and_right_zone_are_mirrored_across_long_axis)
{
   /** \arrange Set parameters for zone creation. */
   Fbk_Field_Of_Interest_T zone_left;
   Fbk_Field_Of_Interest_T zone_right;
   uint8_t ipoint;

   zone_right.size        = 4u;
   zone_right.points[0].x = 0.0f;
   zone_right.points[1].x = 0.0f;
   zone_right.points[2].x = -1.0f;
   zone_right.points[3].x = -1.0f;
   zone_right.points[0].y = 1.0f;
   zone_right.points[1].y = 2.0f;
   zone_right.points[2].y = 2.0f;
   zone_right.points[3].y = 1.0f;
   zone_left              = zone_right;

   /** \action Call Esa_Mirror_Zone_Across_Long_Axis to create the left zone. */
   Esa_Mirror_Zone_Across_Long_Axis(&zone_left);

   /** \assert Check that zone is mirrored along the X-axis. */
   for (ipoint = 0; ipoint < 4u; ipoint++)
   {
      EXPECT_FLOAT_EQ(zone_left.points[ipoint].x, zone_right.points[ipoint].x);
      EXPECT_FLOAT_EQ(zone_left.points[ipoint].y, -zone_right.points[ipoint].y);
   }
}


/**
 * The left and right ESA zones should be symmetric along the X-axis. Create ESA zones and check that the Y-coordinates are
 * mirrored. \uts{CSCSA-123882} \sdd{CSCSA-66547} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Create_Zone_Test, Esa_Create_Zone__left_zone_and_right_zone_are_mirrored_along_long_axis_when_lane_width_higher_than_cal)
{
   /** \arrange Set parameters for ESA zone creation. */
   Fbk_Field_Of_Interest_T zone_right{};
   uint8_t ipoint;

   esa_object.ego_side          = FBK_SIDE_RIGHT;
   tracker_object.index         = 0u;
   p_esa_core_input->lane_width = p_esa_calibration->k_esa_min_lane_width + 1.0f;

   /** \action Call Esa_Create_Zone to create the left and right ESA zones, respectively. */
   Esa_Create_Zone(&esa_object, FBK_ZERO_UINT, p_esa_core_input, p_esa_calibration);
   zone_right          = esa_object.zone;
   esa_object.ego_side = FBK_SIDE_LEFT;
   Esa_Create_Zone(&esa_object, FBK_ZERO_UINT, p_esa_core_input, p_esa_calibration);

   /** \assert Check that zones are mirrored along the X-axis. */
   for (ipoint = 0; ipoint < ESA_NUMBER_OF_ZONE_POINTS; ipoint++)
   {
      EXPECT_FLOAT_EQ(zone_right.points[ipoint].x, esa_object.zone.points[ipoint].x);
      EXPECT_FLOAT_EQ(zone_right.points[ipoint].y, -esa_object.zone.points[ipoint].y);
   }
}


/**
 * The left and right ESA hys zones should be symmetric along the X-axis. Create ESA hys zones and check that the Y-coordinates are
 * mirrored. \uts{CSCSA-123883} \sdd{CSCSA-66547} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Create_Zone_Test,
       Esa_Create_Zone__left_hys_zone_and_right_hys_zone_are_mirrored_along_long_axis_when_lane_width_lower_than_cal)
{
   /** \arrange Set parameters for ESA zone creation. */
   Fbk_Field_Of_Interest_T zone_hys_right{};
   uint8_t ipoint;

   esa_object.ego_side          = FBK_SIDE_RIGHT;
   tracker_object.index         = 1u;
   p_esa_core_input->lane_width = 0.0f;

   /** \action Call Esa_Create_Zone to create the left and right ESA zones, respectively. */
   Esa_Create_Zone(&esa_object, 10u, p_esa_core_input, p_esa_calibration);
   zone_hys_right      = esa_object.zone;
   esa_object.ego_side = FBK_SIDE_LEFT;
   Esa_Create_Zone(&esa_object, 10u, p_esa_core_input, p_esa_calibration);

   /** \assert Check that zones are mirrored along the X-axis. */
   for (ipoint = 0; ipoint < ESA_NUMBER_OF_ZONE_POINTS; ipoint++)
   {
      EXPECT_FLOAT_EQ(zone_hys_right.points[ipoint].x, esa_object.zone.points[ipoint].x);
      EXPECT_FLOAT_EQ(zone_hys_right.points[ipoint].y, -esa_object.zone.points[ipoint].y);
   }
}


/**
 * Check that ESA zone is created correctly if the lane width is in calibration limits.
 * \uts{CSCSA-209328} \sdd{CSCSA-66547} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Create_Zone_Test, Esa_Create_Zone__lane_width_in_the_limits)
{
   /** \arrange Set parameters for ESA zone creation. */
   uint8_t ipoint;

   esa_object.ego_side          = FBK_SIDE_RIGHT;
   p_esa_core_input->lane_width = 0.5f * (p_esa_calibration->k_esa_min_lane_width + p_esa_calibration->k_esa_max_lane_width);
   p_esa_core_input->lane_center_offset = 0.0f;

   /** \action Call Esa_Create_Zone to create ESA zones. */
   Esa_Create_Zone(&esa_object, FBK_ZERO_UINT, p_esa_core_input, p_esa_calibration);

   /** \assert Check that y coors of the zone are calculated corectly. */
   for (ipoint = 0; ipoint < ESA_NUMBER_OF_ZONE_POINTS; ipoint++)
   {
      EXPECT_FLOAT_EQ(esa_object.zone.points[ipoint].y, p_esa_core_input->lane_width * p_esa_calibration->k_esa_zone_y[ipoint]);
   }
}


/**
 * Check that ESA zone is created correctly if the lane width is below the minimum.
 * \uts{CSCSA-209329} \sdd{CSCSA-66547} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Create_Zone_Test, Esa_Create_Zone__lane_width_below_the_minimum)
{
   /** \arrange Set parameters for ESA zone creation. */
   uint8_t ipoint;

   esa_object.ego_side                  = FBK_SIDE_RIGHT;
   p_esa_core_input->lane_width         = 0.5f * p_esa_calibration->k_esa_min_lane_width;
   p_esa_core_input->lane_center_offset = 0.0f;

   /** \action Call Esa_Create_Zone to create ESA zones. */
   Esa_Create_Zone(&esa_object, FBK_ZERO_UINT, p_esa_core_input, p_esa_calibration);

   /** \assert Check that y coors of the zone are calculated corectly. */
   for (ipoint = 0; ipoint < ESA_NUMBER_OF_ZONE_POINTS; ipoint++)
   {
      EXPECT_FLOAT_EQ(esa_object.zone.points[ipoint].y,
                      p_esa_calibration->k_esa_min_lane_width * p_esa_calibration->k_esa_zone_y[ipoint]);
   }
}


/**
 * Check that ESA zone is created correctly if the lane width is above the maximum.
 * \uts{CSCSA-209330} \sdd{CSCSA-66547} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Esa_Create_Zone_Test, Esa_Create_Zone__lane_width_above_the_maximum)
{
   /** \arrange Set parameters for ESA zone creation. */
   uint8_t ipoint;

   esa_object.ego_side                  = FBK_SIDE_RIGHT;
   p_esa_core_input->lane_width         = 1.1f * p_esa_calibration->k_esa_max_lane_width;
   p_esa_core_input->lane_center_offset = 0.0f;

   /** \action Call Esa_Create_Zone to create ESA zones. */
   Esa_Create_Zone(&esa_object, FBK_ZERO_UINT, p_esa_core_input, p_esa_calibration);

   /** \assert Check that y coors of the zone are calculated corectly. */
   for (ipoint = 0; ipoint < ESA_NUMBER_OF_ZONE_POINTS; ipoint++)
   {
      EXPECT_FLOAT_EQ(esa_object.zone.points[ipoint].y,
                      p_esa_calibration->k_esa_max_lane_width * p_esa_calibration->k_esa_zone_y[ipoint]);
   }
}
