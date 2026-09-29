/** \file
   File contains unit tests for Is_Pos_Dist_Inside_Ellipse function
*/

#include "f360_is_pos_dist_inside_ellipse.h"
#include "f360_cluster_grouping_data_generator.h"
#include "f360_math_func.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

using namespace f360_variant_A;

/** \defgroup  f360_is_pos_dist_inside_ellipse
 *  @{
 */

/** \brief
*  Test Group for Is_Pos_Dist_Inside_Ellipse function
**/
TEST_GROUP(f360_is_pos_dist_inside_ellipse)
{
   /** \setup
   * Setting up elipse's axis, cluster, tolerance, angle's cos and sin, delta of position
   **/
   float32_t orth_axis = 0.0;
   float32_t az_axis = 0.0;

   F360_Cluster_T cluster_older = {};

   float32_t tolerance = 1e-3; //here is made to avoid numeric accuracy errors

   bool f_success = false;

   float32_t trk_az_rep_cos = 0.0;
   float32_t trk_az_rep_sin = 0.0;
   float32_t delta_x = 0.0;
   float32_t delta_y = 0.0;

};

/**
*\purpose  For simple nominal case where elipse is rotated by 0 degree. Checking one of 4 cases is point placed on different edge of elipse will be still inside
*\req    NA
*/
TEST(f360_is_pos_dist_inside_ellipse, Is_Pos_Dist_Inside_Ellipse__PointOnOneOfEdgeOfElipse1)
{
   /** \precond
   * Filling up axes, angles and dinstance from center for flag checking
   **/
   orth_axis = 2.0;
   az_axis = 1.0;

   trk_az_rep_cos = 1.0;
   trk_az_rep_sin = 0.0;

   delta_x = 0.0;
   delta_y = 2.0 - tolerance;

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Is_Pos_Dist_Inside_Ellipse(delta_x, delta_y, trk_az_rep_cos, trk_az_rep_sin,
      orth_axis, az_axis);

   /** \result
   * Checking if gating flag is true
   **/
   CHECK_TRUE(f_success);
}

/**
*\purpose  For simple nominal case where elipse is rotated by 0 degree. Checking one of 4 cases is point placed on different edge of elipse will be still inside
*\req    NA
*/
TEST(f360_is_pos_dist_inside_ellipse, Is_Pos_Dist_Inside_Ellipse__PointOnOneOfEdgeOfElipse2)
{
   /** \precond
   * Filling up axes, angles and dinstance from center for flag checking
   **/
   orth_axis = 2.0;
   az_axis = 1.0;

   trk_az_rep_cos = 1.0;
   trk_az_rep_sin = 0.0;

   delta_x = 0.0;
   delta_y = -2.0 + tolerance;

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Is_Pos_Dist_Inside_Ellipse(delta_x, delta_y, trk_az_rep_cos, trk_az_rep_sin,
      orth_axis, az_axis);

   /** \result
   * Checking if gating flag is true
   **/
   CHECK_TRUE(f_success);
}

/**
*\purpose  For simple nominal case where elipse is rotated by 0 degree. Checking one of 4 cases is point placed on different edge of elipse will be still inside
*\req    NA
*/
TEST(f360_is_pos_dist_inside_ellipse, Is_Pos_Dist_Inside_Ellipse__PointOnOneOfEdgeOfElipse3)
{
   /** \precond
   * Filling up axes, angles and dinstance from center for flag checking
   **/
   orth_axis = 2.0;
   az_axis = 1.0;

   trk_az_rep_cos = 1.0;
   trk_az_rep_sin = 0.0;

   delta_x = 1.0 - tolerance;
   delta_y = 0.0;

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Is_Pos_Dist_Inside_Ellipse(delta_x, delta_y, trk_az_rep_cos, trk_az_rep_sin,
      orth_axis, az_axis);

   /** \result
   * Checking if gating flag is true
   **/
   CHECK_TRUE(f_success);
}

/**
*\purpose  For simple nominal case where elipse is rotated by 0 degree. Checking one of 4 cases is point placed on different edge of elipse will be still inside
*\req    NA
*/
TEST(f360_is_pos_dist_inside_ellipse, Is_Pos_Dist_Inside_Ellipse__PointOnOneOfEdgeOfElipse4)
{
   /** \precond
   * Filling up axes, angles and dinstance from center for flag checking
   **/
   orth_axis = 2.0;
   az_axis = 1.0;

   trk_az_rep_cos = 1.0;
   trk_az_rep_sin = 0.0;

   delta_x = -1.0 + tolerance;
   delta_y = 0.0;

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Is_Pos_Dist_Inside_Ellipse(delta_x, delta_y, trk_az_rep_cos, trk_az_rep_sin,
      orth_axis, az_axis);

   /** \result
   * Checking if gating flag is true
   **/
   CHECK_TRUE(f_success);
}

/**
*\purpose  For simple nominal case where elipse is rotated by 45 degree. Checking one of 4 cases is point placed on different edge of elipse will be still inside
*\req    NA
*/
TEST(f360_is_pos_dist_inside_ellipse, Is_Pos_Dist_Inside_Ellipse__PointOnOneOfEdgeOfRotatedElipse1)
{
   /** \precond
   * Filling up axes, angles and dinstance from center for flag checking
   **/
   orth_axis = 2.0;
   az_axis = 4.0;

   Fill_Cluster(1.0, 1.0, 0.0, 0.0, cluster_older);
   trk_az_rep_cos = cluster_older.cos_vcs_az;
   trk_az_rep_sin = cluster_older.sin_vcs_az;

   delta_x = 4.0 * cluster_older.cos_vcs_az - tolerance;
   delta_y = 4.0 * cluster_older.sin_vcs_az - tolerance;

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Is_Pos_Dist_Inside_Ellipse(delta_x, delta_y, trk_az_rep_cos, trk_az_rep_sin,
      orth_axis, az_axis);

   /** \result
   * Checking if gating flag is true
   **/
   CHECK_TRUE(f_success);
}

/**
*\purpose  For simple nominal case where elipse is rotated by 45 degree. Checking one of 4 cases is point placed on different edge of elipse will be still inside
*\req    NA
*/
TEST(f360_is_pos_dist_inside_ellipse, Is_Pos_Dist_Inside_Ellipse__PointOnOneOfEdgeOfRotatedElipse2)
{
   /** \precond
   * Filling up axes, angles and dinstance from center for flag checking
   **/
   orth_axis = 2.0;
   az_axis = 4.0;

   Fill_Cluster(1.0, 1.0, 0.0, 0.0, cluster_older);
   trk_az_rep_cos = cluster_older.cos_vcs_az;
   trk_az_rep_sin = cluster_older.sin_vcs_az;

   delta_x = -4.0 * cluster_older.cos_vcs_az + tolerance;
   delta_y = -4.0 * cluster_older.sin_vcs_az + tolerance;

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Is_Pos_Dist_Inside_Ellipse(delta_x, delta_y, trk_az_rep_cos, trk_az_rep_sin,
      orth_axis, az_axis);

   /** \result
   * Checking if gating flag is true
   **/
   CHECK_TRUE(f_success);
}

/**
*\purpose  For simple nominal case where elipse is rotated by 45 degree. Checking one of 4 cases is point placed on different edge of elipse will be still inside
*\req    NA
*/
TEST(f360_is_pos_dist_inside_ellipse, Is_Pos_Dist_Inside_Ellipse__PointOnOneOfEdgeOfRotatedElipse3)
{
   /** \precond
   * Filling up axes, angles and dinstance from center for flag checking
   **/
   orth_axis = 2.0;
   az_axis = 4.0;

   Fill_Cluster(1.0, 1.0, 0.0, 0.0, cluster_older);
   trk_az_rep_cos = cluster_older.cos_vcs_az;
   trk_az_rep_sin = cluster_older.sin_vcs_az;

   delta_x = -2.0 * cluster_older.cos_vcs_az + tolerance;
   delta_y = 2.0 * cluster_older.sin_vcs_az - tolerance;

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Is_Pos_Dist_Inside_Ellipse(delta_x, delta_y, trk_az_rep_cos, trk_az_rep_sin,
      orth_axis, az_axis);

   /** \result
   * Checking if gating flag is true
   **/
   CHECK_TRUE(f_success);
}

/**
*\purpose  For simple nominal case where elipse is rotated by 45 degree. Checking one of 4 cases is point placed on different edge of elipse will be still inside
*\req    NA
*/
TEST(f360_is_pos_dist_inside_ellipse, Is_Pos_Dist_Inside_Ellipse__PointOnOneOfEdgeOfRotatedElipse4)
{
   /** \precond
   * Filling up axes, angles and dinstance from center for flag checking
   **/
   orth_axis = 2.0;
   az_axis = 4.0;

   Fill_Cluster(1.0, 1.0, 0.0, 0.0, cluster_older);
   trk_az_rep_cos = cluster_older.cos_vcs_az;
   trk_az_rep_sin = cluster_older.sin_vcs_az;

   delta_x = 2.0 * cluster_older.cos_vcs_az - tolerance;
   delta_y = -2.0 * cluster_older.sin_vcs_az + tolerance;

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Is_Pos_Dist_Inside_Ellipse(delta_x, delta_y, trk_az_rep_cos, trk_az_rep_sin,
      orth_axis, az_axis);

   /** \result
   * Checking if gating flag is true
   **/
   CHECK_TRUE(f_success);
}

/**
*\purpose  For simple nominal case where elipse is rotated by 0 degree. Checking one of 4 cases is point placed slightly outside the edge of elipse will be considered as ousider
*\req    NA
*/
TEST(f360_is_pos_dist_inside_ellipse, Is_Pos_Dist_Inside_Ellipse__PointOutOfElipse1)
{
   /** \precond
   * Filling up axes, angles and dinstance from center for flag checking
   **/
   orth_axis = 2.0;
   az_axis = 1.0;

   trk_az_rep_cos = 1.0;
   trk_az_rep_sin = 0.0;

   delta_x = 0.0;
   delta_y = 2.0 + tolerance;

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Is_Pos_Dist_Inside_Ellipse(delta_x, delta_y, trk_az_rep_cos, trk_az_rep_sin,
      orth_axis, az_axis);

   /** \result
   * Checking if gating flag is false
   **/
   CHECK_FALSE(f_success);
}

/**
*\purpose  For simple nominal case where elipse is rotated by 0 degree. Checking one of 4 cases is point placed slightly outside the edge of elipse will be considered as ousider
*\req    NA
*/
TEST(f360_is_pos_dist_inside_ellipse, Is_Pos_Dist_Inside_Ellipse__PointOutOfElipse2)
{
   /** \precond
   * Filling up axes, angles and dinstance from center for flag checking
   **/
   orth_axis = 2.0;
   az_axis = 1.0;

   trk_az_rep_cos = 1.0;
   trk_az_rep_sin = 0.0;

   delta_x = 0.0;
   delta_y = -2.0 - tolerance;

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Is_Pos_Dist_Inside_Ellipse(delta_x, delta_y, trk_az_rep_cos, trk_az_rep_sin,
      orth_axis, az_axis);

   /** \result
   * Checking if gating flag is false
   **/
   CHECK_FALSE(f_success);
}

/**
*\purpose  For simple nominal case where elipse is rotated by 0 degree. Checking one of 4 cases is point placed slightly outside the edge of elipse will be considered as ousider
*\req    NA
*/
TEST(f360_is_pos_dist_inside_ellipse, Is_Pos_Dist_Inside_Ellipse__PointOutOfElipse3)
{
   /** \precond
   * Filling up axes, angles and dinstance from center for flag checking
   **/
   orth_axis = 2.0;
   az_axis = 1.0;

   trk_az_rep_cos = 1.0;
   trk_az_rep_sin = 0.0;

   delta_x = 1.0 + tolerance;
   delta_y = 0.0;

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Is_Pos_Dist_Inside_Ellipse(delta_x, delta_y, trk_az_rep_cos, trk_az_rep_sin,
      orth_axis, az_axis);

   /** \result
   * Checking if gating flag is false
   **/
   CHECK_FALSE(f_success);
}

/**
*\purpose  For simple nominal case where elipse is rotated by 0 degree. Checking one of 4 cases is point placed slightly outside the edge of elipse will be considered as ousider
*\req    NA
*/
TEST(f360_is_pos_dist_inside_ellipse, Is_Pos_Dist_Inside_Ellipse__PointOutOfElipse4)
{
   /** \precond
   * Filling up axes, angles and dinstance from center for flag checking
   **/
   orth_axis = 2.0;
   az_axis = 1.0;

   trk_az_rep_cos = 1.0;
   trk_az_rep_sin = 0.0;

   delta_x = -1.0 - tolerance;
   delta_y = 0.0;

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Is_Pos_Dist_Inside_Ellipse(delta_x, delta_y, trk_az_rep_cos, trk_az_rep_sin,
      orth_axis, az_axis);

   /** \result
   * Checking if gating flag is false
   **/
   CHECK_FALSE(f_success);
}

/**
*\purpose  For simple nominal case where elipse is rotated by 45 degree. 
* Checking if flag will be calculated correctly if point will be slightly outside the elipse
*\req    NA
*/
TEST(f360_is_pos_dist_inside_ellipse, Is_Pos_Dist_Inside_Ellipse__PointOusideOfRotatedElipse1)
{
   /** \precond
   * Filling up axes, angles and dinstance from center for flag checking
   **/
   orth_axis = 2.0;
   az_axis = 4.0;

   Fill_Cluster(1.0, 1.0, 0.0, 0.0, cluster_older);
   trk_az_rep_cos = cluster_older.cos_vcs_az;
   trk_az_rep_sin = cluster_older.sin_vcs_az;

   delta_x = 2.0 * cluster_older.cos_vcs_az + tolerance;
   delta_y = -2.0 * cluster_older.sin_vcs_az - tolerance;

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Is_Pos_Dist_Inside_Ellipse(delta_x, delta_y, trk_az_rep_cos, trk_az_rep_sin,
      orth_axis, az_axis);

   /** \result
   * Checking if gating flag is false
   **/
   CHECK_FALSE(f_success);
}

/**
*\purpose  For simple nominal case where elipse is rotated by 45 degree.
* Checking if flag will be calculated correctly if point will be slightly outside the elipse
*\req    NA
*/
TEST(f360_is_pos_dist_inside_ellipse, Is_Pos_Dist_Inside_Ellipse__PointOusideOfRotatedElipse2)
{
   /** \precond
   * Filling up axes, angles and dinstance from center for flag checking
   **/
   orth_axis = 2.0;
   az_axis = 4.0;

   Fill_Cluster(1.0, 1.0, 0.0, 0.0, cluster_older);
   trk_az_rep_cos = cluster_older.cos_vcs_az;
   trk_az_rep_sin = cluster_older.sin_vcs_az;

   delta_x = 4.0 * cluster_older.cos_vcs_az + tolerance;
   delta_y = 4.0 * cluster_older.sin_vcs_az + tolerance;

   /** \action
   * Call Fine_Cluster_Gate function
   **/
   f_success = Is_Pos_Dist_Inside_Ellipse(delta_x, delta_y, trk_az_rep_cos, trk_az_rep_sin,
      orth_axis, az_axis);

   /** \result
   * Checking if gating flag is false
   **/
   CHECK_FALSE(f_success);
}

/** @}*/
