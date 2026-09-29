/**
 * @file ced_create_zones_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for ced_create_zones.c functions
 *
 * @copyright Copyright (C) 2020 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-41450}
 */

#include "ced_create_zones_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "ced_core_calibration.h"
#include "ced_create_zones.c"
#include "ced_types.h"
#include "fbk_field_of_interest.h"
#include "ml_vector_2d_t.h"
}

/**
 * Fill the funnel zone information dependent on vehicle data and calibrations.
 * \uts{CSCSA-41451} \sdd{SF-3599} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Create_Zones_Test, Ced_Create_Funnel_Zone__fills_funnel_zone_and_crash_line_offset_correctly)
{
   /** \arrange Create data structures and set calibration values. */
   Fbk_Field_Of_Interest_T funnel_zone{};

   /** \action Call function to create the funnel zone. */
   Ced_Create_Funnel_Zone(&funnel_zone, p_vehicle_data, &ced_cals);

   /** \assert Verify that zone and crash line offset are calculated according to vehicle data and calibrations. */
   EXPECT_EQ(funnel_zone.size, CED_NUMBER_OF_ZONE_POINTS);
   EXPECT_FLOAT_EQ(funnel_zone.points[CED_POINT_FRONT_LEFT].x, ced_cals.k_ced_funnel_zone_length);
   EXPECT_FLOAT_EQ(funnel_zone.points[CED_POINT_FRONT_RIGHT].x, ced_cals.k_ced_funnel_zone_length);
   EXPECT_FLOAT_EQ(funnel_zone.points[CED_POINT_FRONT_RIGHT].y, ced_cals.k_ced_funnel_zone_width);
}

/**
 * Fill the collision zone information dependent on vehicle data and calibrations.
 * \uts{CSCSA-41452} \sdd{SF-3598} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ced_Create_Zones_Test, Ced_Create_Collision_Zone__fills_collision_zone_correctly)
{
   /** \arrange Create data structures and set calibration values. */
   Fbk_Field_Of_Interest_T collision_zone{};

   /** \action Call function to create the collision zone. */
   Ced_Create_Collision_Zone(&collision_zone, p_vehicle_data, &ced_cals);

   /** \assert Verify that zone is calculated according to vehicle data and calibrations. */
   EXPECT_EQ(collision_zone.size, CED_NUMBER_OF_ZONE_POINTS);
   EXPECT_FLOAT_EQ(collision_zone.points[CED_POINT_FRONT_RIGHT].y,
                   (p_vehicle_data->host_width / 2.0f) + ced_cals.k_ced_collision_zone_width);
   EXPECT_FLOAT_EQ(collision_zone.points[CED_POINT_REAR_RIGHT].y,
                   (p_vehicle_data->host_width / 2.0f) + ced_cals.k_ced_collision_zone_width);
   EXPECT_FLOAT_EQ(collision_zone.points[CED_POINT_REAR_LEFT].x, -p_vehicle_data->host_length);
}
