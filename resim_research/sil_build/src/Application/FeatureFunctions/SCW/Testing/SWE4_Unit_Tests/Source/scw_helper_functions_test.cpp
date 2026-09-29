/**
 * @file scw_helper_functions_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for SCW unit tests
 *
 * @copyright Copyright (C) 2024 Aptiv. All rights reserved.
 *
 */

/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-44448}
 */

#include "scw_helper_functions_test.hpp"
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "fbk_object_data_t.h"
#include "ml_vector_2d_t.h"
#include "pa_reuse.h"
#include "scw_helper_functions.c"
#include "scw_helper_functions.h"
}

/*
 * Test if the mapping from Object corners structure to field of interest structure is performed correctly.
 * \uts{CSCSA-44449} \sdd{SF-8189} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Scw_Helper_Functions_Test, Scw_Set_Up_Object_Zone__check_mapping)
{
   /** \arrange Set up SCW object on right ego side. */
   Fbk_Object_Corners_T obj_target_corners;
   Fbk_Field_Of_Interest_T object_zone;

   obj_target_corners.points[FBK_FRONT_LEFT_CORNER].x  = 7.0f;
   obj_target_corners.points[FBK_REAR_LEFT_CORNER].x   = 2.0f;
   obj_target_corners.points[FBK_FRONT_RIGHT_CORNER].x = 7.0f;
   obj_target_corners.points[FBK_REAR_RIGHT_CORNER].x  = 2.0f;

   obj_target_corners.points[FBK_FRONT_LEFT_CORNER].y  = 1.0f;
   obj_target_corners.points[FBK_REAR_LEFT_CORNER].y   = 1.0f;
   obj_target_corners.points[FBK_FRONT_RIGHT_CORNER].y = 4.0f;
   obj_target_corners.points[FBK_REAR_RIGHT_CORNER].y  = 4.0f;


   /** \action Call Scw_Get_Obj_Side to get object side. */
   Scw_Set_Up_Object_Zone(&object_zone, &obj_target_corners);

   /** \assert Verify that right side is returned. */
   EXPECT_FLOAT_EQ(object_zone.points[0].x, 7.0f);
   EXPECT_FLOAT_EQ(object_zone.points[3].x, 2.0f);
   EXPECT_FLOAT_EQ(object_zone.points[1].x, 7.0f);
   EXPECT_FLOAT_EQ(object_zone.points[2].x, 2.0f);
   EXPECT_FLOAT_EQ(object_zone.points[0].y, 1.0f);
   EXPECT_FLOAT_EQ(object_zone.points[3].y, 1.0f);
   EXPECT_FLOAT_EQ(object_zone.points[1].y, 4.0f);
   EXPECT_FLOAT_EQ(object_zone.points[2].y, 4.0f);
}
