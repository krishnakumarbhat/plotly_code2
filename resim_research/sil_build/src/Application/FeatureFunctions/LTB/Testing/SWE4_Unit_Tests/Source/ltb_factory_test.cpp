/**
 * @file ltb_factory_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Unit test implementation for ltb_factory.c functions
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-46111}
 */

#include "ltb_factory_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_field_of_interest.h"
#include "ltb_core_calibration.h"
#include "ltb_factory.c"
#include "ltb_types.h"
#include "ml_vector_2d_t.h"
}

/**
 * Fill the funnel zone information dependent on vehicle data and calibrations.
 * \uts{CSCSA-46112} \sdd{CSCSA-53906} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Ltb_Factory_Test, Ltb_Create_Zone__fills_zone_correctly)
{
   /** \arrange Create data structures and set calibration values. */
   Fbk_Field_Of_Interest_T zone_left{};
   Fbk_Field_Of_Interest_T zone_right{};

   /* Contains the LTB points of the zone check - finally enum in ltb_types */
   uint8_t ltb_point_front_right = 1u; /**< Front right corner of zone */
   uint8_t ltb_point_rear_right  = 2u; /**< Rear right corner of zone */

   /** \action Call function to create the funnel zone. */
   Ltb_Create_Zone(&zone_left, &zone_right, &ltb_cals);

   /** \assert Verify that zone is calculated according to calibrations. */
   EXPECT_EQ(zone_left.size, LTB_NUMBER_OF_ZONE_POINTS);
   EXPECT_EQ(zone_right.size, LTB_NUMBER_OF_ZONE_POINTS);

   EXPECT_EQ(zone_left.points[ltb_point_front_right].y, ltb_cals.k_ltb_zone_width);
   EXPECT_EQ(zone_left.points[ltb_point_rear_right].x, -ltb_cals.k_ltb_zone_length);
   EXPECT_EQ(zone_left.points[ltb_point_rear_right].y, ltb_cals.k_ltb_zone_width);

   EXPECT_EQ(zone_right.points[ltb_point_front_right].y, -ltb_cals.k_ltb_zone_width);
   EXPECT_EQ(zone_right.points[ltb_point_rear_right].x, -ltb_cals.k_ltb_zone_length);
   EXPECT_EQ(zone_right.points[ltb_point_rear_right].y, -ltb_cals.k_ltb_zone_width);
}
