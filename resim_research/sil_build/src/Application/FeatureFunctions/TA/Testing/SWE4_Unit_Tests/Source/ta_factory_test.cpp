/**
 * @file ta_factory_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for TA unit tests
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44821}
 */

#include "ta_factory_test.hpp"
#include <gmock/gmock-matchers.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_field_of_interest.h"
#include "fbk_macros.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "ta_core_calibration.h"
#include "ta_factory.c"
}

using ::testing::Eq;
using ::testing::FloatNear;

/*
 * Tests basic functionality of reset of the ego trajectory The trajectory shall be reset to its default values, e.g. the validity
 * flag shall be false. \uts{CSCSA-44827} \sdd{SF-8699} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Factory_Test, Ta_Reset_Trajectory__reset_ego_traj)
{
   /** \arrange Set a value in ego trajectory struct to a non-default value. */
   ego_trajectory.f_trajectory_valid = FBK_TRUE;
   ta_cal.k_ta_prediction_steps_max  = 20;

   /** \action Reset ego trajectory */
   Ta_Reset_Trajectory(&ego_trajectory, &ta_cal);

   /** \assert Check if ego trajectory has default values. */
   EXPECT_FALSE(ego_trajectory.f_trajectory_valid);
}

/*
 * Tests the constructor for the Danger zones. Both the left and right zones are expected to have the same amount of zone points.
 * The zone shall be filled with the predefined calibration values as zone points. \uts{CSCSA-44824} \sdd{SF-8696}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Factory_Test, Ta_Create_Danger_Zones__set_up_zones)
{
   /** \arrange Set up danger zone cal values. */
   Fbk_Field_Of_Interest_T danger_left;
   Fbk_Field_Of_Interest_T danger_right;

   ta_cal.k_fta_danger_zone_point_size = 1;

   ta_cal.k_fta_danger_zone_left_long[0] = 1.0f;
   ta_cal.k_fta_danger_zone_left_lat[0]  = 1.0f;

   ta_cal.k_fta_danger_zone_right_long[0] = 1.0f;
   ta_cal.k_fta_danger_zone_right_lat[0]  = 1.0f;

   /** \action Create danger zone */
   Ta_Create_Danger_Zones(&danger_left, &danger_right, &ta_cal);

   /** \assert Check if created danger zone values are equal to specified cal values. */
   EXPECT_EQ(danger_left.size, ta_cal.k_fta_danger_zone_point_size);
   EXPECT_EQ(danger_right.size, ta_cal.k_fta_danger_zone_point_size);
   EXPECT_FLOAT_EQ(danger_left.points[0].x, ta_cal.k_fta_danger_zone_left_long[0]);
   EXPECT_FLOAT_EQ(danger_left.points[0].y, ta_cal.k_fta_danger_zone_left_lat[0]);
   EXPECT_FLOAT_EQ(danger_right.points[0].x, ta_cal.k_fta_danger_zone_right_long[0]);
   EXPECT_FLOAT_EQ(danger_right.points[0].y, ta_cal.k_fta_danger_zone_right_lat[0]);
}


/*
 * Tests the constructor for the info zones. Both the left and right zones are expected to have the same amount of zone points. The
 * zone shall be filled with the predefined calibration values as zone points. \uts{CSCSA-44825} \sdd{SF-8697}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Factory_Test, Ta_Create_Info_Zones__set_up_zones)
{
   /** \arrange Set up info zone cal values. */
   Fbk_Field_Of_Interest_T info_left;
   Fbk_Field_Of_Interest_T info_right;

   ta_cal.k_rta_info_zone_point_size = 1;

   ta_cal.k_rta_info_zone_left_long[0] = 1.0f;
   ta_cal.k_rta_info_zone_left_lat[0]  = 1.0f;

   ta_cal.k_rta_info_zone_right_long[0] = 1.0f;
   ta_cal.k_rta_info_zone_right_lat[0]  = 1.0f;

   boolean_T f_zone_hysteresis = FBK_FALSE;

   /** \action Create info zone */
   Ta_Create_Info_Zones(&info_left, &info_right, &ta_cal, f_zone_hysteresis);

   /** \assert Check if created info zone values are equal to specified cal values. */
   EXPECT_EQ(info_left.size, ta_cal.k_rta_info_zone_point_size);
   EXPECT_EQ(info_right.size, ta_cal.k_rta_info_zone_point_size);
   EXPECT_FLOAT_EQ(info_left.points[0].x, ta_cal.k_rta_info_zone_left_long[0]);
   EXPECT_FLOAT_EQ(info_left.points[0].y, ta_cal.k_rta_info_zone_left_lat[0]);
   EXPECT_FLOAT_EQ(info_right.points[0].x, ta_cal.k_rta_info_zone_right_long[0]);
   EXPECT_FLOAT_EQ(info_right.points[0].y, ta_cal.k_rta_info_zone_right_lat[0]);
}

/*
 * Tests the constructor for the info hysteresis zones. Both the left and right zones are expected to have the same amount of zone
 * points. The zone shall be filled with the predefined calibration values as zone points. \uts{CSCSA-44828} \sdd{SF-8697}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Factory_Test, Ta_Create_Info_Zones__set_up_zones_with_hysteresis)
{
   /** \arrange Set up info zone cal values. */
   Fbk_Field_Of_Interest_T info_left;
   Fbk_Field_Of_Interest_T info_right;

   boolean_T f_zone_hysteresis = FBK_TRUE;

   /** \action Create info zone */
   Ta_Create_Info_Zones(&info_left, &info_right, &ta_cal, f_zone_hysteresis);

   /** \assert Check if created info zone values are equal to specified cal values. */
   EXPECT_EQ(info_left.size, ta_cal.k_rta_info_zone_point_size);
   EXPECT_FLOAT_EQ(info_left.points[0].x, ta_cal.k_rta_info_zone_left_long[0] + ta_cal.k_rta_info_zone_left_long_hys[0]);
   EXPECT_FLOAT_EQ(info_left.points[0].y, ta_cal.k_rta_info_zone_left_lat[0] + ta_cal.k_rta_info_zone_left_lat_hys[0]);
   EXPECT_FLOAT_EQ(info_left.points[1].x, ta_cal.k_rta_info_zone_left_long[1] + ta_cal.k_rta_info_zone_left_long_hys[1]);
   EXPECT_FLOAT_EQ(info_left.points[1].y, ta_cal.k_rta_info_zone_left_lat[1] + ta_cal.k_rta_info_zone_left_lat_hys[1]);
   EXPECT_FLOAT_EQ(info_left.points[2].x, ta_cal.k_rta_info_zone_left_long[2] + ta_cal.k_rta_info_zone_left_long_hys[2]);
   EXPECT_FLOAT_EQ(info_left.points[2].y, ta_cal.k_rta_info_zone_left_lat[2] + ta_cal.k_rta_info_zone_left_lat_hys[2]);
   EXPECT_FLOAT_EQ(info_left.points[3].x, ta_cal.k_rta_info_zone_left_long[3] + ta_cal.k_rta_info_zone_left_long_hys[3]);
   EXPECT_FLOAT_EQ(info_left.points[3].y, ta_cal.k_rta_info_zone_left_lat[3] + ta_cal.k_rta_info_zone_left_lat_hys[3]);

   EXPECT_EQ(info_right.size, ta_cal.k_rta_info_zone_point_size);
   EXPECT_FLOAT_EQ(info_right.points[0].x, ta_cal.k_rta_info_zone_right_long[0] + ta_cal.k_rta_info_zone_right_long_hys[0]);
   EXPECT_FLOAT_EQ(info_right.points[0].y, ta_cal.k_rta_info_zone_right_lat[0] + ta_cal.k_rta_info_zone_right_lat_hys[0]);
   EXPECT_FLOAT_EQ(info_right.points[1].x, ta_cal.k_rta_info_zone_right_long[1] + ta_cal.k_rta_info_zone_right_long_hys[1]);
   EXPECT_FLOAT_EQ(info_right.points[1].y, ta_cal.k_rta_info_zone_right_lat[1] + ta_cal.k_rta_info_zone_right_lat_hys[1]);
   EXPECT_FLOAT_EQ(info_right.points[2].x, ta_cal.k_rta_info_zone_right_long[2] + ta_cal.k_rta_info_zone_right_long_hys[2]);
   EXPECT_FLOAT_EQ(info_right.points[2].y, ta_cal.k_rta_info_zone_right_lat[2] + ta_cal.k_rta_info_zone_right_lat_hys[2]);
   EXPECT_FLOAT_EQ(info_right.points[3].x, ta_cal.k_rta_info_zone_right_long[3] + ta_cal.k_rta_info_zone_right_long_hys[3]);
   EXPECT_FLOAT_EQ(info_right.points[3].y, ta_cal.k_rta_info_zone_right_lat[3] + ta_cal.k_rta_info_zone_right_lat_hys[3]);
}

/*
 * Tests the constructor for the info hysteresis zones. Both the left and right zones are expected to have the same amount of zone
 * points. The zone shall be filled with the regular calibration values as zone points due to the hysteresis zone being smaller
 * than the regular zone. \uts{CSCSA-44830} \sdd{SF-8697} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Factory_Test, Ta_Create_Info_Zones__set_up_zones_with_hysteresis_fallback_to_regular_zone)
{
   /** \arrange Set up info zone cal values. Reverse direction of hysteresis. */
   Fbk_Field_Of_Interest_T info_left;
   Fbk_Field_Of_Interest_T info_right;

   boolean_T f_zone_hysteresis = FBK_TRUE;

   ta_cal.k_rta_info_zone_left_long_hys[0] = -ta_cal.k_rta_info_zone_left_long_hys[0];
   ta_cal.k_rta_info_zone_left_lat_hys[0]  = -ta_cal.k_rta_info_zone_left_lat_hys[0];
   ta_cal.k_rta_info_zone_left_long_hys[1] = -ta_cal.k_rta_info_zone_left_long_hys[1];
   ta_cal.k_rta_info_zone_left_lat_hys[1]  = -ta_cal.k_rta_info_zone_left_lat_hys[1];
   ta_cal.k_rta_info_zone_left_long_hys[2] = -ta_cal.k_rta_info_zone_left_long_hys[2];
   ta_cal.k_rta_info_zone_left_lat_hys[2]  = -ta_cal.k_rta_info_zone_left_lat_hys[2];
   ta_cal.k_rta_info_zone_left_long_hys[3] = -ta_cal.k_rta_info_zone_left_long_hys[3];
   ta_cal.k_rta_info_zone_left_lat_hys[3]  = -ta_cal.k_rta_info_zone_left_lat_hys[3];

   ta_cal.k_rta_info_zone_right_long_hys[0] = -ta_cal.k_rta_info_zone_right_long_hys[0];
   ta_cal.k_rta_info_zone_right_lat_hys[0]  = -ta_cal.k_rta_info_zone_right_lat_hys[0];
   ta_cal.k_rta_info_zone_right_long_hys[1] = -ta_cal.k_rta_info_zone_right_long_hys[1];
   ta_cal.k_rta_info_zone_right_lat_hys[1]  = -ta_cal.k_rta_info_zone_right_lat_hys[1];
   ta_cal.k_rta_info_zone_right_long_hys[2] = -ta_cal.k_rta_info_zone_right_long_hys[2];
   ta_cal.k_rta_info_zone_right_lat_hys[2]  = -ta_cal.k_rta_info_zone_right_lat_hys[2];
   ta_cal.k_rta_info_zone_right_long_hys[3] = -ta_cal.k_rta_info_zone_right_long_hys[3];
   ta_cal.k_rta_info_zone_right_lat_hys[3]  = -ta_cal.k_rta_info_zone_right_lat_hys[3];

   /** \action Create info zone */
   Ta_Create_Info_Zones(&info_left, &info_right, &ta_cal, f_zone_hysteresis);

   /** \assert Check if created info zone values are equal to specified cal values. */
   EXPECT_EQ(info_left.size, ta_cal.k_rta_info_zone_point_size);
   EXPECT_FLOAT_EQ(info_left.points[0].x, ta_cal.k_rta_info_zone_left_long[0]);
   EXPECT_FLOAT_EQ(info_left.points[0].y, ta_cal.k_rta_info_zone_left_lat[0]);
   EXPECT_FLOAT_EQ(info_left.points[1].x, ta_cal.k_rta_info_zone_left_long[1]);
   EXPECT_FLOAT_EQ(info_left.points[1].y, ta_cal.k_rta_info_zone_left_lat[1]);
   EXPECT_FLOAT_EQ(info_left.points[2].x, ta_cal.k_rta_info_zone_left_long[2]);
   EXPECT_FLOAT_EQ(info_left.points[2].y, ta_cal.k_rta_info_zone_left_lat[2]);
   EXPECT_FLOAT_EQ(info_left.points[3].x, ta_cal.k_rta_info_zone_left_long[3]);
   EXPECT_FLOAT_EQ(info_left.points[3].y, ta_cal.k_rta_info_zone_left_lat[3]);

   EXPECT_EQ(info_right.size, ta_cal.k_rta_info_zone_point_size);
   EXPECT_FLOAT_EQ(info_right.points[0].x, ta_cal.k_rta_info_zone_right_long[0]);
   EXPECT_FLOAT_EQ(info_right.points[0].y, ta_cal.k_rta_info_zone_right_lat[0]);
   EXPECT_FLOAT_EQ(info_right.points[1].x, ta_cal.k_rta_info_zone_right_long[1]);
   EXPECT_FLOAT_EQ(info_right.points[1].y, ta_cal.k_rta_info_zone_right_lat[1]);
   EXPECT_FLOAT_EQ(info_right.points[2].x, ta_cal.k_rta_info_zone_right_long[2]);
   EXPECT_FLOAT_EQ(info_right.points[2].y, ta_cal.k_rta_info_zone_right_lat[2]);
   EXPECT_FLOAT_EQ(info_right.points[3].x, ta_cal.k_rta_info_zone_right_long[3]);
   EXPECT_FLOAT_EQ(info_right.points[3].y, ta_cal.k_rta_info_zone_right_lat[3]);
}


/*
 * Tests the constructor for the wing zones. Both the left and right zones are expected to have the same amount of zone points. The
 * zone shall be filled with the predefined calibration values as zone points. \uts{CSCSA-44826} \sdd{SF-8698}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Factory_Test, Ta_Create_Wing_Zones__set_up_zones)
{
   /** \arrange Set up wing zone cal values. */
   Fbk_Field_Of_Interest_T wing_left;
   Fbk_Field_Of_Interest_T wing_right;

   ta_cal.k_rta_wing_zone_point_size = 1;

   ta_cal.k_rta_wing_zone_left_long[0] = 1.0f;
   ta_cal.k_rta_wing_zone_left_lat[0]  = 1.0f;

   ta_cal.k_rta_wing_zone_right_long[0] = 1.0f;
   ta_cal.k_rta_wing_zone_right_lat[0]  = 1.0f;

   boolean_T f_zone_hysteresis = FBK_FALSE;

   /** \action Create wing zone */
   Ta_Create_Wing_Zones(&wing_left, &wing_right, &ta_cal, f_zone_hysteresis);

   /** \assert Check if created wing zone values are equal to specified cal values. */
   EXPECT_EQ(wing_left.size, ta_cal.k_rta_wing_zone_point_size);
   EXPECT_EQ(wing_right.size, ta_cal.k_rta_wing_zone_point_size);
   EXPECT_FLOAT_EQ(wing_left.points[0].x, ta_cal.k_rta_wing_zone_left_long[0]);
   EXPECT_FLOAT_EQ(wing_left.points[0].y, ta_cal.k_rta_wing_zone_left_lat[0]);
   EXPECT_FLOAT_EQ(wing_right.points[0].x, ta_cal.k_rta_wing_zone_right_long[0]);
   EXPECT_FLOAT_EQ(wing_right.points[0].y, ta_cal.k_rta_wing_zone_right_lat[0]);
}

/*
 * Tests the constructor for the wing hysteresis zones. Both the left and right zones are expected to have the same amount of zone
 * points. The zone shall be filled with the predefined calibration values as zone points. \uts{CSCSA-44829} \sdd{SF-8698}
 * \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Factory_Test, Ta_Create_Wing_Zones__set_up_zones_with_hysteresis)
{
   /** \arrange Set up wing zone cal values. */
   Fbk_Field_Of_Interest_T wing_left;
   Fbk_Field_Of_Interest_T wing_right;

   boolean_T f_zone_hysteresis = FBK_TRUE;

   /** \action Create wing zone */
   Ta_Create_Wing_Zones(&wing_left, &wing_right, &ta_cal, f_zone_hysteresis);

   /** \assert Check if created wing zone values are equal to specified cal values. */
   EXPECT_EQ(wing_left.size, ta_cal.k_rta_wing_zone_point_size);
   EXPECT_FLOAT_EQ(wing_left.points[0].x, ta_cal.k_rta_wing_zone_left_long[0] + ta_cal.k_rta_wing_zone_left_long_hys[0]);
   EXPECT_FLOAT_EQ(wing_left.points[0].y, ta_cal.k_rta_wing_zone_left_lat[0] + ta_cal.k_rta_wing_zone_left_lat_hys[0]);
   EXPECT_FLOAT_EQ(wing_left.points[1].x, ta_cal.k_rta_wing_zone_left_long[1] + ta_cal.k_rta_wing_zone_left_long_hys[1]);
   EXPECT_FLOAT_EQ(wing_left.points[1].y, ta_cal.k_rta_wing_zone_left_lat[1] + ta_cal.k_rta_wing_zone_left_lat_hys[1]);
   EXPECT_FLOAT_EQ(wing_left.points[2].x, ta_cal.k_rta_wing_zone_left_long[2] + ta_cal.k_rta_wing_zone_left_long_hys[2]);
   EXPECT_FLOAT_EQ(wing_left.points[2].y, ta_cal.k_rta_wing_zone_left_lat[2] + ta_cal.k_rta_wing_zone_left_lat_hys[2]);
   EXPECT_FLOAT_EQ(wing_left.points[3].x, ta_cal.k_rta_wing_zone_left_long[3] + ta_cal.k_rta_wing_zone_left_long_hys[3]);
   EXPECT_FLOAT_EQ(wing_left.points[3].y, ta_cal.k_rta_wing_zone_left_lat[3] + ta_cal.k_rta_wing_zone_left_lat_hys[3]);

   EXPECT_EQ(wing_right.size, ta_cal.k_rta_wing_zone_point_size);
   EXPECT_FLOAT_EQ(wing_right.points[0].x, ta_cal.k_rta_wing_zone_right_long[0] + ta_cal.k_rta_wing_zone_right_long_hys[0]);
   EXPECT_FLOAT_EQ(wing_right.points[0].y, ta_cal.k_rta_wing_zone_right_lat[0] + ta_cal.k_rta_wing_zone_right_lat_hys[0]);
   EXPECT_FLOAT_EQ(wing_right.points[1].x, ta_cal.k_rta_wing_zone_right_long[1] + ta_cal.k_rta_wing_zone_right_long_hys[1]);
   EXPECT_FLOAT_EQ(wing_right.points[1].y, ta_cal.k_rta_wing_zone_right_lat[1] + ta_cal.k_rta_wing_zone_right_lat_hys[1]);
   EXPECT_FLOAT_EQ(wing_right.points[2].x, ta_cal.k_rta_wing_zone_right_long[2] + ta_cal.k_rta_wing_zone_right_long_hys[2]);
   EXPECT_FLOAT_EQ(wing_right.points[2].y, ta_cal.k_rta_wing_zone_right_lat[2] + ta_cal.k_rta_wing_zone_right_lat_hys[2]);
   EXPECT_FLOAT_EQ(wing_right.points[3].x, ta_cal.k_rta_wing_zone_right_long[3] + ta_cal.k_rta_wing_zone_right_long_hys[3]);
   EXPECT_FLOAT_EQ(wing_right.points[3].y, ta_cal.k_rta_wing_zone_right_lat[3] + ta_cal.k_rta_wing_zone_right_lat_hys[3]);
}

/*
 * Tests the constructor for the wing hysteresis zones. Both the left and right zones are expected to have the same amount of zone
 * points. The zone shall be filled with the regular calibration values as zone points due to the hysteresis zone being smaller
 * than the regular zone. \uts{CSCSA-44831} \sdd{SF-8698} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ta_Factory_Test, Ta_Create_Wing_Zones__set_up_zones_with_hysteresis_fallback_to_regular_zone)
{
   /** \arrange Set up wing zone cal values. Reverse direction of hysteresis */
   Fbk_Field_Of_Interest_T wing_left;
   Fbk_Field_Of_Interest_T wing_right;

   boolean_T f_zone_hysteresis = FBK_TRUE;

   ta_cal.k_rta_wing_zone_left_long_hys[0] = -ta_cal.k_rta_wing_zone_left_long_hys[0];
   ta_cal.k_rta_wing_zone_left_lat_hys[0]  = -ta_cal.k_rta_wing_zone_left_lat_hys[0];
   ta_cal.k_rta_wing_zone_left_long_hys[1] = -ta_cal.k_rta_wing_zone_left_long_hys[1];
   ta_cal.k_rta_wing_zone_left_lat_hys[1]  = -ta_cal.k_rta_wing_zone_left_lat_hys[1];
   ta_cal.k_rta_wing_zone_left_long_hys[2] = -ta_cal.k_rta_wing_zone_left_long_hys[2];
   ta_cal.k_rta_wing_zone_left_lat_hys[2]  = -ta_cal.k_rta_wing_zone_left_lat_hys[2];
   ta_cal.k_rta_wing_zone_left_long_hys[3] = -ta_cal.k_rta_wing_zone_left_long_hys[3];
   ta_cal.k_rta_wing_zone_left_lat_hys[3]  = -ta_cal.k_rta_wing_zone_left_lat_hys[3];

   ta_cal.k_rta_wing_zone_right_long_hys[0] = -ta_cal.k_rta_wing_zone_right_long_hys[0];
   ta_cal.k_rta_wing_zone_right_lat_hys[0]  = -ta_cal.k_rta_wing_zone_right_lat_hys[0];
   ta_cal.k_rta_wing_zone_right_long_hys[1] = -ta_cal.k_rta_wing_zone_right_long_hys[1];
   ta_cal.k_rta_wing_zone_right_lat_hys[1]  = -ta_cal.k_rta_wing_zone_right_lat_hys[1];
   ta_cal.k_rta_wing_zone_right_long_hys[2] = -ta_cal.k_rta_wing_zone_right_long_hys[2];
   ta_cal.k_rta_wing_zone_right_lat_hys[2]  = -ta_cal.k_rta_wing_zone_right_lat_hys[2];
   ta_cal.k_rta_wing_zone_right_long_hys[3] = -ta_cal.k_rta_wing_zone_right_long_hys[3];
   ta_cal.k_rta_wing_zone_right_lat_hys[3]  = -ta_cal.k_rta_wing_zone_right_lat_hys[3];

   /** \action Create wing zone */
   Ta_Create_Wing_Zones(&wing_left, &wing_right, &ta_cal, f_zone_hysteresis);

   /** \assert Check if created wing zone values are equal to specified cal values. */
   EXPECT_EQ(wing_left.size, ta_cal.k_rta_wing_zone_point_size);
   EXPECT_FLOAT_EQ(wing_left.points[0].x, ta_cal.k_rta_wing_zone_left_long[0]);
   EXPECT_FLOAT_EQ(wing_left.points[0].y, ta_cal.k_rta_wing_zone_left_lat[0]);
   EXPECT_FLOAT_EQ(wing_left.points[1].x, ta_cal.k_rta_wing_zone_left_long[1]);
   EXPECT_FLOAT_EQ(wing_left.points[1].y, ta_cal.k_rta_wing_zone_left_lat[1]);
   EXPECT_FLOAT_EQ(wing_left.points[2].x, ta_cal.k_rta_wing_zone_left_long[2]);
   EXPECT_FLOAT_EQ(wing_left.points[2].y, ta_cal.k_rta_wing_zone_left_lat[2]);
   EXPECT_FLOAT_EQ(wing_left.points[3].x, ta_cal.k_rta_wing_zone_left_long[3]);
   EXPECT_FLOAT_EQ(wing_left.points[3].y, ta_cal.k_rta_wing_zone_left_lat[3]);

   EXPECT_EQ(wing_right.size, ta_cal.k_rta_wing_zone_point_size);
   EXPECT_FLOAT_EQ(wing_right.points[0].x, ta_cal.k_rta_wing_zone_right_long[0]);
   EXPECT_FLOAT_EQ(wing_right.points[0].y, ta_cal.k_rta_wing_zone_right_lat[0]);
   EXPECT_FLOAT_EQ(wing_right.points[1].x, ta_cal.k_rta_wing_zone_right_long[1]);
   EXPECT_FLOAT_EQ(wing_right.points[1].y, ta_cal.k_rta_wing_zone_right_lat[1]);
   EXPECT_FLOAT_EQ(wing_right.points[2].x, ta_cal.k_rta_wing_zone_right_long[2]);
   EXPECT_FLOAT_EQ(wing_right.points[2].y, ta_cal.k_rta_wing_zone_right_lat[2]);
   EXPECT_FLOAT_EQ(wing_right.points[3].x, ta_cal.k_rta_wing_zone_right_long[3]);
   EXPECT_FLOAT_EQ(wing_right.points[3].y, ta_cal.k_rta_wing_zone_right_lat[3]);
}
