/**
 * @file fbk_ref_point_calc_test_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for FBK reference point calculation.
 *
 * @copyright Copyright (C) 2021 Aptiv. All rights reserved.
 *
 */
/* Root work item under which all workitems within this file are created
 * \uts_heading_wi{CSCSA-42243}
 */

#include "fbk_ref_point_calc_test.hpp"
#include "gtest/gtest_pred_impl.h"
#include <gtest/gtest-death-test.h>
#include <gtest/gtest-message.h>
#include <gtest/gtest-test-part.h>

extern "C"
{
#include "fbk_macros.h"
#include "fbk_ref_point.h"
#include "fbk_ref_point_calc.h"
#include "ml_math.h"
#include "pa_reuse.h"
}

#ifndef NDEBUG
/**
 * Tests functionality of target corner calculation. Here an assertion is expected, since target vcs position is pointing to NULL.
 * \uts{CSCSA-42466} \sdd{SF-4095} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ref_Point_Calc_Test, Fbk_Calculate_Target_Corners__target_vcs_pos_NULL)
{
   EXPECT_DEATH(
      {
         /** \arrange target pos set to NULL */
         float32_T heading = 0.0f;
         float32_T length  = 0.0f;
         float32_T width   = 0.0f;
         /** \action executes function to test */
         Fbk_Calculate_Target_Corners(&target_corners, NULL, &heading, &length, &width);
         /** \assert expect assertion since attributes are set to NULL */
      },
      ".*p_target_vcs_pos.*");
}

/**
 * Tests functionality of target corner calculation. Here an assertion is expected, since target corners are pointing to NULL.
 * \uts{CSCSA-42467} \sdd{SF-4095} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ref_Point_Calc_Test, Fbk_Calculate_Target_Corners__target_corners_NULL)
{
   EXPECT_DEATH(
      {
         /** \arrange target corners set to NULL */
         float32_T heading = 0.0f;
         float32_T length  = 0.0f;
         float32_T width   = 0.0f;
         /** \action executes function to test */
         Fbk_Calculate_Target_Corners(NULL, &target_vcs_pos, &heading, &length, &width);
         /** \assert expect assertion since target corners are pointing to NULL */
      },
      ".*p_target_corners.*");
}

/**
 * Tests functionality of target corner calculation. Here an assertion is expected, since heading is NULL.
 * \uts{CSCSA-42468} \sdd{SF-4095} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ref_Point_Calc_Test, Fbk_Calculate_Target_Corners__heading_NULL)
{
   EXPECT_DEATH(
      {
         /** \arrange target heading set to NULL */
         float32_T length = 0.0f;
         float32_T width  = 0.0f;
         /** \action executes function to test */
         Fbk_Calculate_Target_Corners(&target_corners, &target_vcs_pos, NULL, &length, &width);
         /** \assert expect assertion since heading is NULL */
      },
      ".*p_heading.*");
}


/**
 * Tests functionality of target corner calculation. Here an assertion is expected, since lengthis NULL.
 * \uts{CSCSA-42469} \sdd{SF-4095} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ref_Point_Calc_Test, Fbk_Calculate_Target_Corners__length_NULL)
{
   EXPECT_DEATH(
      {
         /** \arrange target length set to NULL */
         float32_T heading = 0.0f;
         float32_T width   = 0.0f;
         /** \action executes function to test */
         Fbk_Calculate_Target_Corners(&target_corners, &target_vcs_pos, &heading, NULL, &width);
         /** \assert expect assertion since length is NULL */
      },
      ".*p_length.*");
}

/**
 * Tests functionality of target corner calculation. Here an assertion is expected, since width is NULL.
 * \uts{CSCSA-42470} \sdd{SF-4095} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ref_Point_Calc_Test, Fbk_Calculate_Target_Corners__width_NULL)
{
   EXPECT_DEATH(
      {
         /** \arrange target width set to NULL */
         float32_T heading = 0.0f;
         float32_T length  = 0.0f;
         /** \action executes function to test */
         Fbk_Calculate_Target_Corners(&target_corners, &target_vcs_pos, &heading, &length, NULL);
         /** \assert expect assertion since length is NULL */
      },
      ".*p_width.*");
}

#endif

/**
 * Tests funtionality of target corner calculation. Here an object is created in the fourth quadrant of vcs. All corners of the
 * object are expected to lie within that quadrant. \uts{CSCSA-42471} \sdd{SF-4095} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ref_Point_Calc_Test, Fbk_Calculate_Target_Corners__some_vals)
{
   /** \arrange object in the fourth quadrant */
   float32_T length  = 4.0f;
   float32_T width   = 2.0f;
   float32_T heading = 0.3490658f;
   target_vcs_pos.x  = -40;
   target_vcs_pos.y  = 50;
   /** \action executes function to test */
   Fbk_Calculate_Target_Corners(&target_corners, &target_vcs_pos, &heading, &length, &width);
   /** \assert Expect corners to lie within fourth quadrant */
   for (uint8_t loopidx_check = 0; loopidx_check < 7; loopidx_check++)
   {
      EXPECT_LT(target_corners.points[loopidx_check].x, 0.0f);
      EXPECT_GT(target_corners.points[loopidx_check].y, 0.0f);
   }
}


/**
 * Tests calculation of target reference point for RCTA Reference point is expected to be the front mid corner point.
 * \uts{CSCSA-42472} \sdd{SF-4096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ref_Point_Calc_Test, Fbk_Calculate_Ref_Point__right_approach_middle_front_bumper_set_reference_point)
{
   /** \arrange object approaching from the right side */
   float32_T length  = 5.0f;
   float32_T width   = 2.0f;
   float32_T heading = -0.5f * PI;
   target_vcs_pos.x  = -host_length;
   target_vcs_pos.y  = 10.0f;


   Fbk_Calculate_Target_Corners(&target_corners, &target_vcs_pos, &heading, &length, &width);

   /** \action executes function to test */
   Fbk_Calculate_Ref_Point(&ref_point, &host_ref_point, &target_corners, FBK_FALSE);

   /** \assert reference point shall be the mid front bumper */
   EXPECT_FLOAT_EQ(ref_point.point.x, target_vcs_pos.x);
   EXPECT_FLOAT_EQ(ref_point.point.y, target_vcs_pos.y - 0.5f * length);
}


/**
 * Tests calculation of target reference point for RCTA Reference point is expected to be the front right corner point of the
 * target. \uts{CSCSA-42473} \sdd{SF-4096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ref_Point_Calc_Test, Fbk_Calculate_Ref_Point__right_approach_front_right_corner_set_reference_point)
{
   /** \arrange object approaching from right */

   float32_T heading = -90.0f * PI / 180.0f;
   float32_T length  = 5.0f;
   float32_T width   = 2.0f;
   target_vcs_pos.x  = -host_length - 0.5f * width;
   target_vcs_pos.y  = 10.0f;

   Fbk_Calculate_Target_Corners(&target_corners, &target_vcs_pos, &heading, &length, &width);

   /** \action executes function to test */
   Fbk_Calculate_Ref_Point(&ref_point, &host_ref_point, &target_corners, FBK_FALSE);

   /** \assert reference point shall be the front right corner of the object */
   EXPECT_FLOAT_EQ(ref_point.point.x, target_vcs_pos.x + 0.5f * width);
   EXPECT_FLOAT_EQ(ref_point.point.y, target_vcs_pos.y - 0.5f * length);
}

/**
 * Tests calculation of target reference point for RCTA Reference point is expected to be the front left corner point of the
 * target. \uts{CSCSA-42474} \sdd{SF-4096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ref_Point_Calc_Test, Fbk_Calculate_Ref_Point__right_approach_front_left_corner_set_reference_point)
{
   /** \arrange object approaching from right side */
   float32_T heading = -90.0f * PI / 180.0f;
   float32_T length  = 5.0f;
   float32_T width   = 2.0f;
   target_vcs_pos.x  = -host_length + 0.5f * width;
   target_vcs_pos.y  = 10.0f;

   Fbk_Calculate_Target_Corners(&target_corners, &target_vcs_pos, &heading, &length, &width);

   /** \action executes function to test */
   Fbk_Calculate_Ref_Point(&ref_point, &host_ref_point, &target_corners, FBK_FALSE);

   /** \assert reference point shall be the front left corner of the object */
   EXPECT_FLOAT_EQ(ref_point.point.x, target_vcs_pos.x - 0.5f * width);
   EXPECT_FLOAT_EQ(ref_point.point.y, target_vcs_pos.y - 0.5f * length);
}

/**
 * Tests calculation of target reference point for FCTA Reference point is expected to be the middle of the front bumper point of
 * the target. \uts{CSCSA-42475} \sdd{SF-4096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ref_Point_Calc_Test, Fbk_Calculate_Ref_Point__right_approach_fcta_middle_front_bumper_set_reference_point)
{
   /** \arrange object approaching from right side */
   float32_T heading = -90.0f * PI / 180.0f;
   float32_T length  = 5.0f;
   float32_T width   = 2.0f;
   target_vcs_pos.x  = 0.0f;
   target_vcs_pos.y  = 10.0f;

   host_ref_point = Create_2d_Vector_Coordinates(0.0f, 0.0f);

   Fbk_Calculate_Target_Corners(&target_corners, &target_vcs_pos, &heading, &length, &width);

   /** \action executes function to test */
   Fbk_Calculate_Ref_Point(&ref_point, &host_ref_point, &target_corners, FBK_FALSE);

   /** \assert reference point shall be the middle of the front bumper of the object */
   EXPECT_FLOAT_EQ(ref_point.point.x, target_vcs_pos.x);
   EXPECT_FLOAT_EQ(ref_point.point.y, target_vcs_pos.y - 0.5f * length);
}


/**
 * Tests calculation of target reference point for RCTA Reference point is expected to be the front left corner point of the target
 * set by the default check. \uts{CSCSA-42476} \sdd{SF-4096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ref_Point_Calc_Test, Fbk_Calculate_Ref_Point__left_approach_middle_front_bumper_set_reference_point)
{
   /** \arrange object approaching from left side */
   float32_T heading = 90.0f * PI / 180.0f;
   float32_T length  = 5.0f;
   float32_T width   = 2.0f;
   target_vcs_pos.x  = -host_length;
   target_vcs_pos.y  = -10.0f;

   Fbk_Calculate_Target_Corners(&target_corners, &target_vcs_pos, &heading, &length, &width);

   /** \action executes function to test */
   Fbk_Calculate_Ref_Point(&ref_point, &host_ref_point, &target_corners, FBK_FALSE);

   /** \assert */
   EXPECT_FLOAT_EQ(ref_point.point.x, target_vcs_pos.x);
   EXPECT_FLOAT_EQ(ref_point.point.y, target_vcs_pos.y + 0.5f * length);
}


/**
 * Tests calculation of target reference point for RCTA Reference point is expected to be the front left corner point of the
 * target. \uts{CSCSA-42477} \sdd{SF-4096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ref_Point_Calc_Test, Fbk_Calculate_Ref_Point__left_approach_front_left_corner_set_reference_point)
{
   /** \arrange object approaching from left side */
   float32_T heading = 90.0f * PI / 180.0f;
   float32_T length  = 5.0f;
   float32_T width   = 2.0f;
   target_vcs_pos.x  = -host_length - 0.5f * width;
   target_vcs_pos.y  = -10.0f;

   Fbk_Calculate_Target_Corners(&target_corners, &target_vcs_pos, &heading, &length, &width);

   /** \action executes function to test */
   Fbk_Calculate_Ref_Point(&ref_point, &host_ref_point, &target_corners, FBK_FALSE);

   /** \assert front left corner point shall be reference point */
   EXPECT_FLOAT_EQ(ref_point.point.x, target_vcs_pos.x + 0.5f * width);
   EXPECT_FLOAT_EQ(ref_point.point.y, target_vcs_pos.y + 0.5f * length);
}

/**
 * Tests calculation of target reference point for RCTA Reference point is expected to be the front right corner point of the
 * target. \uts{CSCSA-42478} \sdd{SF-4096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ref_Point_Calc_Test, Fbk_Calculate_Ref_Point__left_approach_front_right_corner_set_reference_point)
{
   /** \arrange object approaching from left side */
   float32_T heading = 90.0f * PI / 180.0f;
   float32_T length  = 5.0f;
   float32_T width   = 2.0f;
   target_vcs_pos.x  = -host_length + 0.5f * width;
   target_vcs_pos.y  = -10.0f;

   Fbk_Calculate_Target_Corners(&target_corners, &target_vcs_pos, &heading, &length, &width);

   /** \action executes function to test */
   Fbk_Calculate_Ref_Point(&ref_point, &host_ref_point, &target_corners, FBK_FALSE);

   /** \assert front right corner point shall be reference point */
   EXPECT_FLOAT_EQ(ref_point.point.x, target_vcs_pos.x - 0.5f * width);
   EXPECT_FLOAT_EQ(ref_point.point.y, target_vcs_pos.y + 0.5f * length);
}

/**
 * Tests calculation of target reference point for FCTA Reference point is expected to be the front middle corner point of the
 * target. \uts{CSCSA-42479} \sdd{SF-4096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ref_Point_Calc_Test, Fbk_Calculate_Ref_Point__left_approach_fcta_middle_front_bumper_set_reference_point)
{
   /** \arrange object approaching from left side */
   float32_T heading = 90.0f * PI / 180.0f;
   float32_T length  = 5.0f;
   float32_T width   = 2.0f;
   target_vcs_pos.x  = 0.0f;
   target_vcs_pos.y  = -10.0f;
   host_ref_point    = Create_2d_Vector_Coordinates(0.0f, 0.0f);

   Fbk_Calculate_Target_Corners(&target_corners, &target_vcs_pos, &heading, &length, &width);

   /** \action executes function to test */
   Fbk_Calculate_Ref_Point(&ref_point, &host_ref_point, &target_corners, FBK_FALSE);

   /** \assert front middle corner point shall be reference point */
   EXPECT_FLOAT_EQ(ref_point.point.x, target_vcs_pos.x);
   EXPECT_FLOAT_EQ(ref_point.point.y, target_vcs_pos.y + 0.5f * length);
}


/**
 * Tests calculation of target reference point for RCTA Reference point is expected to be the the mid of the targets left side
 * point. \uts{CSCSA-42480} \sdd{SF-4096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ref_Point_Calc_Test, Fbk_Calculate_Ref_Point__left_next_to_ego_front_left_corner_set_reference_point)
{
   /** \arrange object approaching from left side */
   float32_T heading = 90.0f * PI / 180.0f;
   float32_T length  = 5.0f;
   float32_T width   = 2.0f;
   target_vcs_pos.x  = -2.5f;
   target_vcs_pos.y  = -2.5f;

   Fbk_Calculate_Target_Corners(&target_corners, &target_vcs_pos, &heading, &length, &width);

   /** \action executes function to test */
   Fbk_Calculate_Ref_Point(&ref_point, &host_ref_point, &target_corners, FBK_TRUE);

   /** \assert mid of the targets left side point shall be the reference point */
   EXPECT_FLOAT_EQ(ref_point.point.x, target_vcs_pos.x + 0.5f * width);
   EXPECT_FLOAT_EQ(ref_point.point.y, target_vcs_pos.y + 0.5f * length);
}


#ifndef NDEBUG
/**
 * Tests functionality of reference point calculation. Here an assertion is expected, since the target reference point address is
 * NULL. \uts{CSCSA-42481} \sdd{SF-4096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ref_Point_Calc_Test, Fbk_Calculate_Ref_Point__ref_point_NULL)
{
   EXPECT_DEATH(
      {
         /** \arrange target ref point set to NULL */
         /** \action executes function to test */
         Fbk_Calculate_Ref_Point(NULL, &host_ref_point, &target_corners, FBK_TRUE);
         /** \assert expect assertion since target ref point is pointing to NULL */
      },
      ".*p_target_ref_point.*");
}


/**
 * Tests functionality of reference point calculation. Here an assertion is expected, since the host reference point address is
 * NULL. \uts{CSCSA-42482} \sdd{SF-4096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ref_Point_Calc_Test, Fbk_Calculate_Ref_Point__host_ref_point_NULL)
{
   EXPECT_DEATH(
      {
         /** \arrange host ref point set to NULL */
         /** \action executes function to test */
         Fbk_Calculate_Ref_Point(&ref_point, NULL, &target_corners, FBK_TRUE);
         /** \assert expect assertion since host ref point is pointing to NULL */
      },
      ".*p_host_ref_point.*");
}


/**
 * Tests functionality of reference point calculation. Here an assertion is expected, since target corners address is NULL.
 * \uts{CSCSA-42483} \sdd{SF-4096} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ref_Point_Calc_Test, Fbk_Calculate_Ref_Point__target_corners_NULL)
{
   EXPECT_DEATH(
      {
         /** \arrange target corners are set to NULL */
         /** \action executes function to test */
         Fbk_Calculate_Ref_Point(&ref_point, &host_ref_point, NULL, FBK_TRUE);
         /** \assert expect assertion since target corners are pointing to NULL */
      },
      ".*p_target_corners.*");
}


#endif

/**
 * Test if the opposite reference point is correctly calculated point.
 * \uts{CSCSA-42484} \sdd{SF-4252} \testtype{SoftwareUpdateTesting}
 */
TEST_F(Fbk_Ref_Point_Calc_Test, Fbk_Get_Opposite_Point__test_all)
{
   /** \arrange expected result table */
   uint8_t idx;
   Fbk_Reference_Position_T results[FBK_NUM_OF_OBJECT_CORNERS];
   Fbk_Reference_Position_T expected[FBK_NUM_OF_OBJECT_CORNERS];
   expected[0] = FBK_FRONT_RIGHT_CORNER;
   expected[1] = FBK_FRONT_MID;
   expected[2] = FBK_FRONT_LEFT_CORNER;
   expected[3] = FBK_RIGHT_MID;
   expected[4] = FBK_REAR_LEFT_CORNER;
   expected[5] = FBK_REAR_MID;
   expected[6] = FBK_REAR_RIGHT_CORNER;
   expected[7] = FBK_LEFT_MID;

   /** \action executes function to test */
   for (idx = 0; idx < FBK_NUM_OF_OBJECT_CORNERS; idx++)
   {
      results[idx] = Fbk_Get_Opposite_Point((Fbk_Reference_Position_T) idx);
   }

   /** \assert compare the results and expected table */
   for (idx = 0; idx < FBK_NUM_OF_OBJECT_CORNERS; idx++)
   {
      EXPECT_EQ(results[idx], expected[idx]);
   }

   EXPECT_DEBUG_DEATH({ Fbk_Get_Opposite_Point(FBK_NUM_OF_OBJECT_CORNERS); }, "");
}
