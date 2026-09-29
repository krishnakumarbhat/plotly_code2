/** \file
 * This file contains unit tests for content of f360_sep_support_functions.cpp file
 */

#include "f360_static_env_polys_support_functions.h"
#include "f360_calibrations.h"
#include <CppUTest/TestHarness.h>

#include "f360_lsc_data_generator.h"
#include "f360_math_func.h"
#include "f360_point.h"

using namespace f360_variant_A;

/** \defgroup  f360_sep_support_functions_Calc_X_Intersection_With_Curve
 *  @{
 */

/** \brief
 * This test group sets up 4 arbitrary SEP's, two on each side of host.
 * On both sides there are one straight line and one curved line.
 * It also initializes variables k and m which are input to the function that
 * are given values in each test case.
 */
TEST_GROUP(f360_sep_support_functions_Calc_X_Intersection_With_Curve)
{
   float32_t k;
   float32_t m;
   float32_t x_start;
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS];
   F360_Calibrations_T calibs;

   // Function return variables
   bool f_intersection_found;
   Point intersect_point_1;
   Point intersect_point_2;

   float32_t test_pass_thres = 1.0e-5F;

   /** \setup
    * Initialize calibrations and create the SEP's.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      Create_Four_SEP(sep);
   }
};

/** \purpose
 * Verify that function returns true and correct intersection when the SEP is a straight line.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Calc_X_Intersection_With_Curve, Calc_X_Intersection_With_Curve_Straight_Line_Intersection_Exists)
{
   /** \precond
    * Create a linear equation to a point behind the first SEP.
    */
   k = INFTY;
   m = 0.0F;

   /** \action
    * Call function
    */
   f_intersection_found = Calc_Intersection_With_SEP(k, m, sep[0], calibs, intersect_point_1, intersect_point_2);

   /** \result
    * Verify that intersection exists and is calculated correctly
    * We expect the intersection to lie at X = 0
    * Second solution should be INFTY as the SEP will be approximated
    * as a line and second solution will default to INFTY
    */
   CHECK_TRUE(f_intersection_found);
   DOUBLES_EQUAL(0.0F, intersect_point_1.x, test_pass_thres);
   DOUBLES_EQUAL(INFTY, intersect_point_2.x, test_pass_thres);
}

/** \purpose
 * Verify that function returns false and no intersection when the SEP is a straight line but intersection does not exist
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Calc_X_Intersection_With_Curve, Calc_X_Intersection_With_Curve_Straight_Line_Intersection_Not_Exists)
{
   /** \precond
    * Create a linear equation that is parallel to the SEP
    */
   k = 0.0F;
   m = 0.0F;

   /** \action
    * Call function
    */
   f_intersection_found = Calc_Intersection_With_SEP(k, m, sep[0], calibs, intersect_point_1, intersect_point_2);

   /** \result
    * Verify that no intersection have been found
    */
   CHECK_FALSE(f_intersection_found);
}

/** \purpose
 * Verify that function returns true and correct intersection when the SEP is a curved line.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Calc_X_Intersection_With_Curve, Calc_X_Intersection_With_Curve_Curved_Line_Intersection_Exists)
{
   /** \precond
    * Create a linear equation to a point behind the first SEP.
    */
   k = 1.0F/F360_EPSILON;//Very large slope
   m = 0.0F;

   /** \action
    * Call function
    */
   f_intersection_found = Calc_Intersection_With_SEP(k, m, sep[1], calibs, intersect_point_1, intersect_point_2);

   /** \result
    * Verify that intersection exists and is calculated correctly
    * We expect the intersection to lie at X = 0
    * Second solution should be close to infinity
    */
   CHECK_TRUE(f_intersection_found);
   DOUBLES_EQUAL(0.0F, intersect_point_1.x, test_pass_thres);
   DOUBLES_EQUAL(8.40336026e+09F, intersect_point_2.x, test_pass_thres);
}

/** \purpose
 * Verify that function returns false and no intersection is found when the SEP is a curved line.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Calc_X_Intersection_With_Curve, Calc_X_Intersection_With_Curve_Curved_Line_Intersection_Not_Exists)
{
   /** \precond
    * Create a linear equation that does not intersect the SEP
    * Switch sign of "a" coefficient so that an intersection to the linear equation does not exist
    */
   k = 0.0F;
   m = 0.0F;
   sep[1].p2 *= -1.0F;

   /** \action
    * Call function
    */
   f_intersection_found = Calc_Intersection_With_SEP(k, m, sep[1], calibs, intersect_point_1, intersect_point_2);

   /** \result
    * Verify that no intersection have been found
    */
   CHECK_FALSE(f_intersection_found);
}
/** @}*/

/** \defgroup  f360_sep_support_functions_Check_If_Point_Is_On_SEP
 *  @{
 */

 /** \brief
  * This test group sets up an arbitrary SEP. Properties of the SEP
  * is modified in the different test cases.
  * It also initializes a point which position is modified in the
  * different test cases.
  */
TEST_GROUP(f360_sep_support_functions_Check_If_Point_Is_On_SEP)
{
   float32_t point_x;
   float32_t point_y;
   Static_Env_Poly_T sep;

   float32_t k_on_curve_thres;

   bool f_point_on_curve;

   /** \setup
    * Initialize calibrations and create the SEP.
    */
   TEST_SETUP()
   {
      k_on_curve_thres = 0.5F;

      // SEP to the left of host
      sep.status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      sep.p2 = 0.001F;
      sep.p1 = 0.1F;
      sep.p0 = -5.0F;
      sep.lower_limit = -30.0F;
      sep.upper_limit = 30.0F;
   }
};

/** \purpose
 * Verify that function returns true when point is on the SEP.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_On_SEP, Check_If_Point_Is_On_SEP_Point_Is_On_Curve)
{
   /** \precond
    * Create a point that lies on the curve
    */
   point_x = 0.0F;
   point_y = -5.0F;

   /** \action
    * Call function
    */
   f_point_on_curve = Check_If_Point_Is_On_SEP(point_x, point_y, sep, k_on_curve_thres);

   /** \result
    * Verify that point is on curve
    */
   CHECK_TRUE(f_point_on_curve);
}

/** \purpose
 * Verify that function returns false when point is behind the SEP.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_On_SEP, Check_If_Point_Is_On_SEP_Point_Is_Behind_Curve)
{
   /** \precond
    * Create a point that lies behind the curve
    */
   point_x = 0.0F;
   point_y = -10.0F;

   /** \action
    * Call function
    */
   f_point_on_curve = Check_If_Point_Is_On_SEP(point_x, point_y, sep, k_on_curve_thres);

   /** \result
    * Verify that point is not on curve
    */
   CHECK_FALSE(f_point_on_curve);
}

/** \purpose
 * Verify that function returns false when point is in front of the SEP.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_On_SEP, Check_If_Point_Is_On_SEP_Point_Is_In_Front_Of_Curve)
{
   /** \precond
    * Create a point that lies in front of the curve
    */
   point_x = 0.0F;
   point_y = 0.0F;

   /** \action
    * Call function
    */
   f_point_on_curve = Check_If_Point_Is_On_SEP(point_x, point_y, sep, k_on_curve_thres);

   /** \result
    * Verify that point is not on curve
    */
   CHECK_FALSE(f_point_on_curve);
}

/** \purpose
 * Verify that function returns false when point is too far behind valid interval of curve
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_On_SEP, Check_If_Point_Is_On_SEP_Point_Is_Behind_Valid_Interval)
{
   /** \precond
    * Create a point that lies behind the curves valid x-interval
    */
   point_x = -40.0F;
   point_y = -5.0F;

   /** \action
    * Call function
    */
   f_point_on_curve = Check_If_Point_Is_On_SEP(point_x, point_y, sep, k_on_curve_thres);

   /** \result
    * Verify that point is not on curve
    */
   CHECK_FALSE(f_point_on_curve);
}

/** \purpose
 * Verify that function returns false when point is too far in front of the valid interval of curve
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_On_SEP, Check_If_Point_Is_On_SEP_Point_Is_In_Front_Of_Valid_Interval)
{
   /** \precond
    * Create a point that lies in front of the curves valid x-interval
    */
   point_x = 40.0F;
   point_y = -5.0F;

   /** \action
    * Call function
    */
   f_point_on_curve = Check_If_Point_Is_On_SEP(point_x, point_y, sep, k_on_curve_thres);

   /** \result
    * Verify that point is not on curve
    */
   CHECK_FALSE(f_point_on_curve);
}
/** @}*/

/** \defgroup  f360_sep_support_functions_Flag_Objects_On_SEP
 *  @{
 */

 /** \brief
  * This test group sets up 4 arbitrary SEP's, two on each side of host.
  * On both sides there are one straight line and one curved line.
  * It also initializes 3 objects with various position in the different test cases.
  */
TEST_GROUP(f360_sep_support_functions_Flag_Objects_On_SEP)
{
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS];
   F360_Tracker_Info_T tracker_info;
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS];
   F360_Calibrations_T calibs;

   /** \setup
    * Initialize calibrations and create the SEPs and objects.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      Create_Four_SEP(sep);

      tracker_info.num_active_objs = 3;
      tracker_info.active_obj_ids[0] = 1;
      tracker_info.active_obj_ids[1] = 2;
      tracker_info.active_obj_ids[2] = 3;
   }
};

/** \purpose
 * Verify that function correctly flags objects as on the correct curve when all objects are on left side of host
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Flag_Objects_On_SEP, Flag_Objects_On_SEP_2_Objects_On_Curves_1_Not_On_Curve_Left_Side)
{
   /** \precond
    * Create objects
    *    - Object 0 is on curve 0 (curve id 1)
    *    - Object 1 is on curve 1 (curve id 2)
    *    - Object 2 is in between curve 0 and 1 and should thus not be flagged as on
    */
   objects[0].vcs_position.x = 10.0F;
   objects[0].vcs_position.y = -5.0F;
   objects[1].vcs_position.x = -0.0F;
   objects[1].vcs_position.y = -10.0F;
   objects[2].vcs_position.x = 0.0F;
   objects[2].vcs_position.y = -7.5F;
   objects[0].reference_point = F360_REFERENCE_POINT_CENTER;
   objects[1].reference_point = F360_REFERENCE_POINT_CENTER;
   objects[2].reference_point = F360_REFERENCE_POINT_CENTER;

   Point center = objects[0].vcs_position;
   objects[0].bbox.Set_Center(center);
   center = objects[1].vcs_position;
   objects[1].bbox.Set_Center(center);
   center = objects[2].vcs_position;
   objects[2].bbox.Set_Center(center);

   /** \action
    * Call function
    */
   Flag_Objects_On_SEP(tracker_info, sep, calibs, objects);

   /** \result
    * Verify that object 0 is flagged as on curve id 1
    * Verify that object 1 is flagged as on curve id 2
    * Verify that object 2 is not flagged as on any curve
    */
   CHECK_EQUAL(1U, objects[0].on_sep_id);
   CHECK_EQUAL(2U, objects[1].on_sep_id);
   CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, objects[2].on_sep_id);
}

/** \purpose
 * Verify that function correctly flags objects as on the correct curve when 1 object is on curve on right side,
 * 1 object is in front of host and 1 object is on curve on left side. One curve is invalid.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Flag_Objects_On_SEP, Flag_Objects_On_SEP_Objects_Left_Center_Right_One_Invalid_Curve)
{
   /** \precond
    * Create objects
    *    - Object 0 is on curve 3 (curve id 4)
    *    - Object 1 is in front of host and should thus not be flagged as on
    *    - Object 2 is on curve 0 (curve id 1)
    * Set curve 2 (curve id 3) as invalid
    */
   objects[0].vcs_position.x = 10.0F;
   objects[0].vcs_position.y = 10.0F;
   objects[1].vcs_position.x = 10.0F;
   objects[1].vcs_position.y = 0.0F;
   objects[2].vcs_position.x = 0.0F;
   objects[2].vcs_position.y = -5.0F;
   objects[0].reference_point = F360_REFERENCE_POINT_CENTER;
   objects[1].reference_point = F360_REFERENCE_POINT_CENTER;
   objects[2].reference_point = F360_REFERENCE_POINT_CENTER;

   Point center = objects[0].vcs_position;
   objects[0].bbox.Set_Center(center);
   center = objects[1].vcs_position;
   objects[1].bbox.Set_Center(center);
   center = objects[2].vcs_position;
   objects[2].bbox.Set_Center(center);
   sep[2].status = F360_STATIC_ENV_POLY_STATUS_INVALID;

   /** \action
    * Call function
    */
   Flag_Objects_On_SEP(tracker_info, sep, calibs, objects);

   /** \result
    * Verify that object 0 is flagged as on curve id 4
    * Verify that object 1 is not flagged as on any curve
    * Verify that object 2 is flagged as on curve id 1
    */
   CHECK_EQUAL(4U, objects[0].on_sep_id);
   CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, objects[1].on_sep_id);
   CHECK_EQUAL(1U, objects[2].on_sep_id);
}
/** @}*/


/** \defgroup  f360_sep_support_functions_flag_single_object_behind_sep
 *  @{
 *
 */
 /** \brief
  * Test group for testing that flagging of objects behind SEPs work as expected.
  */
TEST_GROUP(f360_sep_support_functions_flag_single_object_behind_sep)
{
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_Calibrations_T calibs = {};
   F360_Object_Track_T obj = {};
   F360_Globals_T globals = {};

   /** \setup
   * Initialize tracker calibrations
   * Create a straight SEP 5m laterally from VCS origin between longitudinal position x = (-10, 15)
   * Set object to be moveable
   * Set object on lsc id to F360_INVALID_UNSIGNED_ID
   * Set object vcs position (x,y) = (5, 6)
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      sep[0].p2 = 0.0F;
      sep[0].p1 = 0.0F;
      sep[0].p0 = 5.0F;
      sep[0].lower_limit = -10.0F;
      sep[0].upper_limit = 15.0F;

      obj.movable_prob = 1.0F;
      obj.on_sep_id = F360_INVALID_UNSIGNED_ID;
      obj.vcs_position.x = 5.0F;
      obj.vcs_position.y = 6.0F;
      obj.Set_Bbox_Orientation(Angle{ 0.0F });
      obj.bbox.Set_Length(4.0F);
      obj.reference_point = F360_REFERENCE_POINT_CENTER;
      Point center = obj.vcs_position;
      obj.bbox.Set_Center(center);
      obj.bbox.Set_Length(4.0F);


   }
};

/** \purpose
 * Verify that function correctly marks a moveable object as behind SEP when there is only one valid SEP and object is well behind the SEP.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_flag_single_object_behind_sep, Flag_Single_Object_Behind_SEP_Moveable_Obj_Is_Well_Behind_SEP)
{
   /** \precond
    * The following has been set up in test group:
    * - Tracker calibrations have been initialized.
    * - A straight SEP has been created between x = (-10, 15), y = 5
    * - A moveable object has been created and its vcs position has been set to (-5, 6) such that it is behind SEP
    * - Object on lsc id has been set to F360_INVALID_UNSIGNED_ID
    */

   /** \action
    * Call Flag_Single_Object_Behind_SEP
    */
   Flag_Single_Object_Behind_SEP(sep, calibs, globals, obj);

   /** \result
    * Check that obj was marked as behind lsc with id 1
    * Check that object behind lsc ambiguous flag is false
    */
   CHECK_EQUAL_TEXT(1U, obj.behind_sep_id, "Object was not correctly marked as behind lsc with ID 1.");
   CHECK_FALSE_TEXT(obj.f_behind_sep_ambiguous, "Object was marked as behind lsc ambiguous when it should not have been.");

}

/** \purpose
 * Verify that function does not mark a moveable object as behind SEP when the object is clearly not behind the SEP.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_flag_single_object_behind_sep, Flag_Single_Object_Behind_SEP_Moveable_Obj_Is_Not_Behind_SEP)
{
   /** \precond
    * The following has been set up in test group:
    * - Tracker calibrations have been initialized.
    * - A straight SEP has been created between x = (-10, 15), y = 5
    * - A moveable object has been created
    * - Object on lsc id has been set to F360_INVALID_UNSIGNED_ID
    * Set object vcs position to (40, 6)
    */
   obj.vcs_position.x = 40.0F;
   obj.vcs_position.y = 6.0F;
   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = obj.vcs_position;
   obj.bbox.Set_Center(center);

    /** \action
     * Call Flag_Single_Object_Behind_SEP
     */
   Flag_Single_Object_Behind_SEP(sep, calibs, globals, obj);

   /** \result
    * Check that obj behind lsc id is F360_INVALID_UNSIGNED_ID
    * Check that object behind lsc ambiguous flag is false
    */
   CHECK_EQUAL_TEXT(F360_INVALID_UNSIGNED_ID, obj.behind_sep_id, "Object was marked as behind an lsc when it should not have been.");
   CHECK_FALSE_TEXT(obj.f_behind_sep_ambiguous, "Object was marked as behind lsc ambiguous when it should not have been.");

}

/** \purpose
 * Verify that function does not mark nonmoveable object as behind SEP even though it is well behind the SEP.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_flag_single_object_behind_sep, Flag_Single_Object_Behind_SEP_Nonmoveable_Obj_Is_Well_Behind_SEP)
{
   /** \precond
    * The following has been set up in test group:
    * - Tracker calibrations have been initialized.
    * - A straight SEP has been created between x = (-10, 15), y = 5
    * - A object has been created and its vcs position has been set to (-5, 6) such that it is behind SEP
    * - Object on lsc id has been set to F360_INVALID_UNSIGNED_ID
    * Set object movable_prob to 0.0, such that it should not be considered for marking as behind SEP
    */
   obj.movable_prob = 0.0F;

    /** \action
     * Call Flag_Single_Object_Behind_SEP
     */
   Flag_Single_Object_Behind_SEP(sep, calibs, globals, obj);

   /** \result
    * Check that obj behind lsc id is F360_INVALID_UNSIGNED_ID
    * Check that object behind lsc ambiguous flag is false
    */
   CHECK_EQUAL_TEXT(F360_INVALID_UNSIGNED_ID, obj.behind_sep_id, "Object was marked as behind an lsc when it should not have been.");
   CHECK_FALSE_TEXT(obj.f_behind_sep_ambiguous, "Object was marked as behind lsc ambiguous when it should not have been.");

}

/** \purpose
 * Verify that function does not mark a moveable object as behind SEP when it is marked as on lsc.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_flag_single_object_behind_sep, Flag_Single_Object_Behind_SEP_Moveable_Obj_Flagged_As_On_SEP_Already)
{
   /** \precond
    * The following has been set up in test group:
    * - Tracker calibrations have been initialized.
    * - A straight SEP has been created between x = (-10, 15), y = 5
    * - A moveable object has been created and its vcs position has been set to (-5, 6) such that it is behind SEP
    * Set object on lsc id to 1
    */
   obj.on_sep_id = 1U;

    /** \action
     * Call Flag_Single_Object_Behind_SEP
     */
   Flag_Single_Object_Behind_SEP(sep, calibs, globals, obj);

   /** \result
    * Check that obj behind lsc id is F360_INVALID_UNSIGNED_ID
    * Check that object behind lsc ambiguous flag is false
    */
   CHECK_EQUAL_TEXT(F360_INVALID_UNSIGNED_ID, obj.behind_sep_id, "Object was marked as behind an lsc when it should not have been.");
   CHECK_FALSE_TEXT(obj.f_behind_sep_ambiguous, "Object was marked as behind lsc ambiguous when it should not have been.");
}

/** \purpose
 * Verify that function does not mark object as behind SEP when there is no valid SEP.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_flag_single_object_behind_sep, Flag_Single_Object_Behind_SEP_No_Valid_SEP)
{
   /** \precond
    * The following has been set up in test group:
    * - Tracker calibrations have been initialized.
    * - A straight SEP has been created between x = (-10, 15), y = 5
    * - A MOVEABLE object has been created and its vcs position has been set to (-5, 6) such that it is behind SEP
    * - Object on lsc id has been set to F360_INVALID_UNSIGNED_ID
    * Set valid flag for SEP to false
    */
   sep[0].status = F360_STATIC_ENV_POLY_STATUS_INVALID;

   /** \action
    * Call Flag_Single_Object_Behind_SEP
    */
   Flag_Single_Object_Behind_SEP(sep, calibs, globals, obj);

   /** \result
    * Check that obj behind lsc id is F360_INVALID_UNSIGNED_ID
    * Check that object behind lsc ambiguous flag is false
    */
   CHECK_EQUAL_TEXT(F360_INVALID_UNSIGNED_ID, obj.behind_sep_id, "Object was marked as behind an lsc when it should not have been.");
   CHECK_FALSE_TEXT(obj.f_behind_sep_ambiguous, "Object was marked as behind lsc ambiguous when it should not have been.");

}

/** \purpose
 * Verify that function correctly marks a moveable object as behind SEP when it is slightly below SEP max limit and that object behind lsc ambiguous flag is set to true.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_flag_single_object_behind_sep, Flag_Single_Object_Behind_SEP_Moveable_Obj_Is_Slightly_Below_SEP_Max_Limit)
{
   /** \precond
    * The following has been set up in test group:
    * - Tracker calibrations have been initialized.
    * - A straight SEP has been created between x = (-10, 8), y = 5
    * - A moveable object has been created.
    * - Object on lsc id has been set to F360_INVALID_UNSIGNED_ID
    * Set object vcs x position to slightly below SEP max.
    * Set object vcs y position to slightly above SEP lateral position
    * Set SEP upper longitudinal limit to 8
    */
   sep[0].upper_limit = 8.0F;
   obj.vcs_position.x = sep[0].upper_limit - 0.1F;
   obj.vcs_position.y = sep[0].p0 + 0.5F;
   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = obj.vcs_position;
   obj.bbox.Set_Center(center);

    /** \action
     * Call Flag_Single_Object_Behind_SEP
     */
   Flag_Single_Object_Behind_SEP(sep, calibs, globals, obj);

   /** \result
    * Check that object was marked as behind lsc with id 1
    * Check that object behind lsc ambiguous flag is true
    */
   CHECK_EQUAL_TEXT(1U, obj.behind_sep_id, "Object was not correctly marked as behind lsc with ID 1.");
   CHECK_TRUE_TEXT(obj.f_behind_sep_ambiguous, "Object was not marked as behind lsc ambiguous when it should have been.");

}

/** \purpose
 * Verify that function does not mark a moveable object as behind SEP when it is slightly above SEP max limit but that object behind lsc ambiguous flag is set to true
 *  when the rear end of the object is behind the SEP
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_flag_single_object_behind_sep, Flag_Single_Object_Behind_SEP_Moveable_Obj_Is_Slightly_Above_SEP_Max_Limit)
{
   /** \precond
    * The following has been set up in test group:
    * - Tracker calibrations have been initialized.
    * - A straight SEP has been created between x = (-10, 8), y = 5
    * - A moveable object has been created.
    * - Object on lsc id has been set to F360_INVALID_UNSIGNED_ID
    * Set object vcs x position to slightly above the SEP's upper limit.
    * Set object vcs y position to slightly above SEP lateral position
    * Set SEP upper longitudinal limit to 8
    */
   sep[0].upper_limit = 8.0F;
   obj.vcs_position.x = sep[0].upper_limit + 1.5F;
   obj.vcs_position.y = sep[0].p0 + 0.5F;
   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = obj.vcs_position;
   obj.bbox.Set_Center(center);

   /** \action
    * Call Flag_Single_Object_Behind_SEP
    */
   Flag_Single_Object_Behind_SEP(sep, calibs, globals, obj);

   /** \result
    * Check that obj behind lsc id is F360_INVALID_UNSIGNED_ID
    * Check that object behind lsc ambiguous flag is true
    */
   CHECK_EQUAL_TEXT(F360_INVALID_UNSIGNED_ID, obj.behind_sep_id, "Object was marked as behind an SEP when it should not have been");
   CHECK_TRUE_TEXT(obj.f_behind_sep_ambiguous, "Object was not marked as behind lsc ambiguous when it should have been.");

}

/** \purpose
 * Verify that function does not mark a moveable object as behind SEP when it is slightly below SEP min limit but that object behind lsc ambiguous flag is set to true
 * when object's front is behind the SEP
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_flag_single_object_behind_sep, Flag_Single_Object_Behind_SEP_Moveable_Obj_Is_Slightly_Below_SEP_Min_Limit)
{
   /** \precond
    * The following has been set up in test group:
    * - Tracker calibrations have been initialized.
    * - A straight SEP has been created between x = (-10, 15), y = 5
    * - A moveable object has been created.
    * - Object on lsc id has been set to F360_INVALID_UNSIGNED_ID
    * Set object vcs x position to slightly below SEP lower longitudinal limit.
    * Set object vcs y position to slightly above SEP lateral position
    */
   obj.vcs_position.x = sep[0].lower_limit - 1.5F;
   obj.vcs_position.y = sep[0].p0 + 0.1F;
   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = obj.vcs_position;
   obj.bbox.Set_Center(center);

   /** \action
    * Call Flag_Single_Object_Behind_SEP
    */
   Flag_Single_Object_Behind_SEP(sep, calibs, globals, obj);

   /** \result
    * Check that obj behind lsc id is F360_INVALID_UNSIGNED_ID
    * Check that object behind lsc ambiguous flag is true
    */
   CHECK_EQUAL_TEXT(F360_INVALID_UNSIGNED_ID, obj.behind_sep_id, "Object was marked as behind an SEP when it should not have been");
   CHECK_TRUE_TEXT(obj.f_behind_sep_ambiguous, "Object was not marked as behind lsc ambiguous when it should have been.");
}


/** \purpose
 * Verify that function does not mark a moveable object as behind SEP and that behind lsc ambiguous flag is false when object's front and rear are not behind the SEP.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_flag_single_object_behind_sep, Flag_Single_Object_Behind_SEP_Moveable_Obj_Is_Outside_Ambiguity_Distance)
{
   /** \precond
    * The following has been set up in test group:
    * - Tracker calibrations have been initialized.
    * - A straight SEP has been created between x = (-10, 15), y = 5
    * - A moveable object has been created.
    * - Object on lsc id has been set to F360_INVALID_UNSIGNED_ID
    * Set object vcs x position to above SEP's longitudinal upper limit.
    * Set object vcs y position to slightly above SEP lateral position
    * Set SEP upper longitudinal limit to 8
    */
   sep[0].upper_limit = 8.0F;
   obj.vcs_position.x = sep[0].upper_limit + 3.5F;
   obj.vcs_position.y = sep[0].p0 + 0.5F;
   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = obj.vcs_position;
   obj.bbox.Set_Center(center);

   /** \action
    * Call Flag_Single_Object_Behind_SEP
    */
   Flag_Single_Object_Behind_SEP(sep, calibs, globals, obj);

   /** \result
    * Check that obj behind lsc id is F360_INVALID_UNSIGNED_ID
    * Check that object behind lsc ambiguous flag is true
    */
   CHECK_EQUAL_TEXT(F360_INVALID_UNSIGNED_ID, obj.behind_sep_id, "Object was marked as behind an SEP when it should not have been");
   CHECK_FALSE_TEXT(obj.f_behind_sep_ambiguous, "Object was marked as behind lsc ambiguous when it not should have been.");

}

/** \purpose
 * Verify that function does not mark a moveable object as behind SEP when it is slightly below SEP min limit and that object behind lsc ambiguous flag is set to true, even if
 * there is a SEP on opposite side of host that vector from vcs origin to object centroid clearly intersects. This test is to verify that flag ambiguous flag is not cleared
 * when object vector intersection is with a SEP on opposite side of host.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_flag_single_object_behind_sep, Flag_Single_Object_Behind_SEP_Moveable_Obj_Is_Slightly_Below_SEP_Min_Limit_Unambiguous_Intersection_With_SEP_On_Other_Side_Object_Right_Side)
{
   /** \precond
    * The following has been set up in test group:
    * - Tracker calibrations have been initialized.
    * - A straight SEP has been created between x = (-10, 15), y = 5
    * - A moveable object has been created.
    * - Object on lsc id has been set to F360_INVALID_UNSIGNED_ID
    * Set object vcs x position to slightly below SEPs longitudinal lower limit.
    * Set object vcs y position to slightly above SEP lateral position
    * Create a second SEP on left side of host for which vector from vcs origin to object centroid intersects without ambiguity, to verify that objet is still marked as behind lsc ambiguous from first SEP.
    * Set SEP lower longitudinal limit to -5
    */
   sep[0].lower_limit = -5.0F;
   obj.vcs_position.x = -16.0F;
   obj.vcs_position.y = 15.0F;
   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = obj.vcs_position;
   obj.bbox.Set_Center(center);

   sep[1].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[1].p2 = 0.0F;
   sep[1].p1 = 0.0F;
   sep[1].p0 = -5.0F;
   sep[1].lower_limit = -50.0F;
   sep[1].upper_limit = 50.0F;

   /** \action
    * Call Flag_Single_Object_Behind_SEP
    */
   Flag_Single_Object_Behind_SEP(sep, calibs, globals, obj);

   /** \result
    * Check that obj behind lsc id is F360_INVALID_UNSIGNED_ID
    * Check that object behind lsc ambiguous flag is true
    */
   CHECK_EQUAL_TEXT(F360_INVALID_UNSIGNED_ID, obj.behind_sep_id, "Object was marked as behind an SEP when it should not have been");
   CHECK_TRUE_TEXT(obj.f_behind_sep_ambiguous, "Object was not marked as behind lsc ambiguous when it should have been.");
}

/** \purpose
 * Verify that function does not mark a moveable object as behind SEP and that behind SEP ambiguous flag is false when object and SEP is on opposite sides of host, even if the vector from vcs origin
 * to object centroid intersects the SEP in the ambiguous interval.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_flag_single_object_behind_sep, Flag_Single_Object_Behind_SEP_Moveable_Obj_Ambiguous_Intersection_With_SEP_On_Other_Side_Of_Host)
{
   /** \precond
    * The following has been set up in test group:
    * - Tracker calibrations have been initialized.
    * - A straight SEP has been created at lateral position y = 5
    * - A moveable object has been created
    * - Object on lsc id has been set to F360_INVALID_UNSIGNED_ID
    * Set SEP longitudinal limits to (-10, 0)
    * Set object vcs x position to slightly above SEP max longitudinal limit
    * Set object vcs y position to -1 such that it is on opposite side of host compared to SEP.
    */
   sep[0].lower_limit = -10.0F;
   sep[0].upper_limit = 0.0F;

   obj.vcs_position.x = 0.5F;
   obj.vcs_position.y = -1.0F;
   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = obj.vcs_position;
   obj.bbox.Set_Center(center);

   /** \action
    * Call Flag_Single_Object_Behind_SEP
    */
   Flag_Single_Object_Behind_SEP(sep, calibs, globals, obj);

   /** \result
    * Check that obj behind lsc id is F360_INVALID_UNSIGNED_ID
    * Check that object behind lsc ambiguous flag is false
    */
   CHECK_EQUAL_TEXT(F360_INVALID_UNSIGNED_ID, obj.behind_sep_id, "Object was marked as behind an SEP when it should not have been");
   CHECK_FALSE_TEXT(obj.f_behind_sep_ambiguous, "Object was not marked as behind lsc ambiguous when it should have been.");

}

/** \purpose
 * Verify that function correctly marks a moveable object as behind SEP and that behind SEP ambiguous flag is false when object is ambiguously behind the closest SEP and clearly behind the second closest SEP.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_flag_single_object_behind_sep, Flag_Single_Object_Behind_SEP_Moveable_Obj_Ambiguous_Intersection_With_SEP_First_SEP_Behind_Second)
{
   /** \precond
    * The following has been set up in test group:
    * - Tracker calibrations have been initialized.
    * - A straight SEP has been created at lateral position y = 5
    * - A moveable object has been created
    * - Object on lsc id has been set to F360_INVALID_UNSIGNED_ID
    * Set SEP longitudinal limits to (-10, 0)
    * Create a second SEP between x = (-10, 10), y = 8
    * Set object vcs x position to slightly above max longitudinal limit for first SEP
    * Set object vcs y position to 10 such that it is on opposite side of host compared to SEP.
    */
   sep[0].lower_limit = -10.0F;
   sep[0].upper_limit = 0.0F;

   sep[1].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[1].lower_limit = -10.0F;
   sep[1].upper_limit = 10.0F;
   sep[1].p2 = 0.0F;
   sep[1].p1 = 0.0F;
   sep[1].p0 = 8.0F;

   obj.vcs_position.x = 2.0F;
   obj.vcs_position.y = 10.0F;
   obj.reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = obj.vcs_position;
   obj.bbox.Set_Center(center);

   /** \action
    * Call Flag_Single_Object_Behind_SEP
    */
   Flag_Single_Object_Behind_SEP(sep, calibs, globals, obj);

   /** \result
    * Check that obj behind lsc id is 2
    * Check that object behind lsc ambiguous flag is false
    */
   CHECK_EQUAL_TEXT(2U, obj.behind_sep_id, "Object was not correctly marked as behind lsc with ID 2.");
   CHECK_FALSE_TEXT(obj.f_behind_sep_ambiguous, "Object was marked as behind lsc ambiguous when it should have been.");

}
 /** @}*/

/**\defgroup  f360_sep_support_functions_Flag_Objects_Behind_SEP
* @{
*/

/** \brief
   * This test group sets up 4 arbitrary SEPs, two on each side of host.
   * It also initializes 3 objects which changes position in the various
   * test cases.
   */
TEST_GROUP(f360_sep_support_functions_Flag_Objects_Behind_SEP)
{
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS];
   F360_Tracker_Info_T tracker_info;
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS];
   F360_Calibrations_T calibs;
   F360_Globals_T globals = {};

   /** \setup
      * Initialize calibrations and create the SEPs.
      * Also set up tracker info to contain 3 objects
      */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      Create_Four_SEP(sep);

      tracker_info.num_active_objs = 3;
      tracker_info.active_obj_ids[0] = 1;
      tracker_info.active_obj_ids[1] = 2;
      tracker_info.active_obj_ids[2] = 3;

      // Set properties of the object so they are valid to be flagged as behind curves, only moveable tracks are flagged as behind
      // and objects cannot be both flagged as on and behind a curve.
      for (int32_t i = 0; i < tracker_info.num_active_objs; i++)
      {
         objects[i].movable_prob = 1.0F;
         objects[i].on_sep_id = F360_INVALID_UNSIGNED_ID;
      }
   }
};

/** \purpose
   * Verify that objects are correctly flagged as behind curves.
   * In this test
   *    - Object 0 is behind curve 0, should be flagged
   *    - Object 1 is behind curve 0 but is not moveable, should not be flagged
   *    - Object 2 is behind curve 0 but is also flagged as on curve 0, should not be flagged
   * \req
   * NA.
   */
TEST(f360_sep_support_functions_Flag_Objects_Behind_SEP, Flag_Objects_Behind_SEP_One_Behind_One_Nonmoveable_One_On_Curve)
{
   /** \precond
      * Set object properties
      */
   objects[0].vcs_position.x = 0.0F;
   objects[0].vcs_position.y = -8.0F;

   objects[1].vcs_position.x = 0.0F;
   objects[1].vcs_position.y = -8.0F;
   objects[1].movable_prob = 0.0F;

   objects[2].vcs_position.x = 0.0F;
   objects[2].vcs_position.y = -8.0F;
   objects[2].on_sep_id = 1U;

   objects[0].reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = objects[0].vcs_position;
   objects[0].bbox.Set_Center(center);
   objects[1].reference_point = F360_REFERENCE_POINT_CENTER;
   center = objects[1].vcs_position;
   objects[1].bbox.Set_Center(center);
   objects[2].reference_point = F360_REFERENCE_POINT_CENTER;
   center = objects[2].vcs_position;
   objects[2].bbox.Set_Center(center);

   /** \action
      * Call function
      */
   Flag_Objects_Behind_SEP(tracker_info, sep, calibs, globals, objects);

   /** \result
      * Verify that objects are flagged as expected
      */
   CHECK_EQUAL(1U, objects[0].behind_sep_id);
   CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, objects[1].behind_sep_id);
   CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, objects[2].behind_sep_id);
}

/** \purpose
   * Verify that objects are correctly flagged as behind curves.
   * In this test
   *    - Object 0 is behind curve 0
   *    - Object 1 is behind curve 2
   *    - Object 2 is in front of host, should not be flagged
   *    - Curve 1 is invalid
   * \req
   * NA.
   */
TEST(f360_sep_support_functions_Flag_Objects_Behind_SEP, Flag_Objects_Behind_SEP_Two_Behind_One_Not_Behind)
{
   /** \precond
      * Set object properties
      * Set curve 1 as invalid
      */
   objects[0].vcs_position.x = 0.0F;
   objects[0].vcs_position.y = -8.0F;

   objects[1].vcs_position.x = 0.0F;
   objects[1].vcs_position.y = 8.0F;

   objects[2].vcs_position.x = 10.0F;
   objects[2].vcs_position.y = 0.0F;

   objects[0].reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = objects[0].vcs_position;
   objects[0].bbox.Set_Center(center);
   objects[1].reference_point = F360_REFERENCE_POINT_CENTER;
   center = objects[1].vcs_position;
   objects[1].bbox.Set_Center(center);
   objects[2].reference_point = F360_REFERENCE_POINT_CENTER;
   center = objects[2].vcs_position;
   objects[2].bbox.Set_Center(center);

   sep[1].status = F360_STATIC_ENV_POLY_STATUS_INVALID;

   /** \action
      * Call function
      */
   Flag_Objects_Behind_SEP(tracker_info, sep, calibs, globals, objects);

   /** \result
      * Verify that objects are flagged as expected
      */
   CHECK_EQUAL(1U, objects[0].behind_sep_id);
   CHECK_EQUAL(3U, objects[1].behind_sep_id);
   CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, objects[2].behind_sep_id);
}

/** \purpose
   * Verify that objects are correctly flagged as behind curves.
   * In this test
   *    - Curve 1 is invalid
   * \req
   * NA.
   */
TEST(f360_sep_support_functions_Flag_Objects_Behind_SEP, Flag_Objects_Behind_SEP_Interestion_Point_Below_SEP_And_Hysteris_Limit)
{
   /** \precond
    * Set object properties
    * Set curve 1 as invalid
    */
   objects[0].vcs_position.x = 0.0F;
   objects[0].vcs_position.y = -8.0F;

   objects[1].vcs_position.x = 0.0F;
   objects[1].vcs_position.y = 8.0F;

   objects[2].vcs_position.x = 10.0F;
   objects[2].vcs_position.y = 0.0F;

   objects[0].reference_point = F360_REFERENCE_POINT_CENTER;
   Point center = objects[0].vcs_position;
   objects[0].bbox.Set_Center(center);
   objects[1].reference_point = F360_REFERENCE_POINT_CENTER;
   center = objects[1].vcs_position;
   objects[1].bbox.Set_Center(center);
   objects[2].reference_point = F360_REFERENCE_POINT_CENTER;
   center = objects[2].vcs_position;
   objects[2].bbox.Set_Center(center);


   sep[1].status = F360_STATIC_ENV_POLY_STATUS_INVALID;

   /** \action
      * Call function
      */
   Flag_Objects_Behind_SEP(tracker_info, sep, calibs, globals, objects);

   /** \result
      * Verify that objects are flagged as expected
      */
   CHECK_EQUAL(1U, objects[0].behind_sep_id);
   CHECK_EQUAL(3U, objects[1].behind_sep_id);
   CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, objects[2].behind_sep_id);
}
/** @}*/

/** \defgroup  f360_sep_support_functions_Flag_Objects_On_And_Behind_SEP
 *  @{
 */

 /** \brief
  * This test group sets up 4 arbitrary SEP's, two on each side of host.
  * On both sides there are one straight line and one curved line.
  * Tracker info is set up to have one active object.
  * Tests using this test group will test the functionality of flagging objects on and behind SEP.
  */
TEST_GROUP(f360_sep_support_functions_Flag_Objects_On_And_Behind_SEP)
{
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS];
   F360_Tracker_Info_T tracker_info;
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS];
   F360_Calibrations_T calibs;
   F360_Globals_T globals = {};

   /** \setup
    * Initialize calibrations and create the SEPs and objects.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      Create_Four_SEP(sep);

      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 1;

      objects[0].movable_prob = 1.0F;
      objects[0].reference_point = F360_REFERENCE_POINT_CENTER;
   }
};

/** \purpose
 * Verify that function correctly flags object as on the second SEP on the left side of host and that object behind sep id is not set.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Flag_Objects_On_And_Behind_SEP, Flag_Objects_On_And_Behind_SEP_Object_On_Second_Curve_To_The_Left)
{
   /** \precond
    * Create object
    *    - Set position of object to be on the second curve to the left
    */
   objects[0].vcs_position.x = 0.0F;
   objects[0].vcs_position.y = sep[1].p0;

   Point center = objects[0].vcs_position;
   objects[0].bbox.Set_Center(center);

   /** \action
    * Call Flag_Objects_On_And_Behind_SEP
    */
   Flag_Objects_On_And_Behind_SEP(tracker_info, sep, calibs, globals, objects);

   /** \result
    * Check that object 0 is flagged as on SEP with id 2.
    * Check that object is not flagged as behind any SEP.
    */
   CHECK_EQUAL(2U, objects[0].on_sep_id);
   CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, objects[0].behind_sep_id);
}

/** \purpose
 * Verify that function correctly flags object as behind the first SEP on the left side of host when object is behind both first and second sep.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Flag_Objects_On_And_Behind_SEP, Flag_Objects_On_And_Behind_SEP_Object_Behind_Second_Curve_To_The_Left)
{
   /** \precond
    * Create object
    *    - Set position of object to be behind the second curve to the left
    */
   objects[0].vcs_position.x = 0.0F;
   objects[0].vcs_position.y = sep[1].p0 - 5.0F;

   Point center = objects[0].vcs_position;
   objects[0].bbox.Set_Center(center);

   /** \action
    * Call Flag_Objects_On_And_Behind_SEP
    */
   Flag_Objects_On_And_Behind_SEP(tracker_info, sep, calibs, globals, objects);

   /** \result
    * Check that object is not flagged as on any sep.
    * Check that object 0 is flagged behind SEP with id 2.
    */
   CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, objects[0].on_sep_id);
   CHECK_EQUAL(1U, objects[0].behind_sep_id);
}
/** @}*/

/** \defgroup  f360_sep_support_functions_Flag_Objects_On_And_Behind_SEP
 *  @{
 */

 /** \brief
  * This test group sets up 4 arbitrary SEP's, two on each side of host.
  * On both sides there are one straight line and one curved line.
  * Tests using this test group will test the functionality of flagging a single object on and behind SEP.
  */
TEST_GROUP(f360_sep_support_functions_Flag_Single_Object_On_And_Behind_SEP)
{
   F360_Object_Track_T obj;
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS];
   F360_Calibrations_T calibs;
   F360_Globals_T globals = {};

   /** \setup
    * Initialize calibrations and create the SEPs and objects.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      Create_Four_SEP(sep);

      obj.movable_prob = 1.0F;
      obj.reference_point = F360_REFERENCE_POINT_CENTER;
   }
};

/** \purpose
 * Verify that function correctly flags object as on the second SEP on the left side of host and that object behind sep id is not set.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Flag_Single_Object_On_And_Behind_SEP, Flag_Single_Object_On_And_Behind_SEP_Object_On_Second_Curve_To_The_Left)
{
   /** \precond
    * Create object
    *    - Set position of object to be on the second curve to the left
    */
   obj.vcs_position.x = 0.0F;
   obj.vcs_position.y = sep[1].p0;

   Point center = obj.vcs_position;
   obj.bbox.Set_Center(center);

   /** \action
    * Call Flag_Single_Objects_On_And_Behind_SEP
    */
   Flag_Single_Object_On_And_Behind_SEP(sep, calibs, globals, obj);

   /** \result
    * Check that object 0 is flagged as on SEP with id 2.
    * Check that object is not flagged as behind any SEP.
    */
   CHECK_EQUAL(2U, obj.on_sep_id);
   CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, obj.behind_sep_id);
}

/** \purpose
 * Verify that function correctly flags a object as behind the first SEP on the left side of host when object is behind both first and second sep.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Flag_Single_Object_On_And_Behind_SEP, Flag_Single_Object_On_And_Behind_SEP_Object_Behind_Second_Curve_To_The_Left)
{
   /** \precond
    * - Set position of object to be behind the second curve to the left
    */
   obj.vcs_position.x = 0.0F;
   obj.vcs_position.y = sep[1].p0 - 5.0F;

   Point center = obj.vcs_position;
   obj.bbox.Set_Center(center);

   /** \action
    * Call Flag_Single_Object_On_And_Behind_SEP
    */
   Flag_Single_Object_On_And_Behind_SEP(sep, calibs, globals, obj);

   /** \result
    * Check that object is not flagged as on any sep.
    * Check that object 0 is flagged behind SEP with id 2.
    */
   CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, obj.on_sep_id);
   CHECK_EQUAL(1U, obj.behind_sep_id);
}
/** @}*/

/** \defgroup  f360_sep_support_functions_Flag_Detections_On_SEP
 *  @{
 */

 /** \brief
  * This test group sets up 4 arbitrary SEP's, two on each side of host.
  * On both sides there are one straight line and one curved line.
  * It also initializes 3 detections with various position in the different test cases.
  */
TEST_GROUP(f360_sep_support_functions_Flag_Detections_On_SEP)
{
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   rspp_variant_A::RSPP_Detection_List_T raw_dets_list;
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS];
   F360_Calibrations_T calibs;

   /** \setup
    * Initialize calibrations and create the SEPs and set number of detections.
    * Also set all detections as ok to use
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      Create_Four_SEP(sep);

      raw_dets_list.number_of_valid_detections = 3U;

      for (uint32_t i = 0U; i < raw_dets_list.number_of_valid_detections; i++)
      {
         det_props[i].f_ok_to_use = true;
      }
   }
};

/** \purpose
 * Verify that function correctly flags detections as on the correct curve when all detections are on left side of host
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Flag_Detections_On_SEP, Flag_Dets_On_Longi_Stat_Curves_2_Dets_On_Curves_1_Not_On_Curve_Left_Side)
{
   /** \precond
    * Create detections
    *    - Detection 0 is on curve 0 (curve id 1)
    *    - Detection 1 is on curve 1 (curve id 2)
    *    - Detection 2 is in between curve 0 and 1 and should thus not be flagged as on
    */
   det_props[0].vcs_position.x = 10.0F;
   det_props[0].vcs_position.y = -5.0F;
   det_props[1].vcs_position.x = -0.0F;
   det_props[1].vcs_position.y = -10.0F;
   det_props[2].vcs_position.x = 0.0F;
   det_props[2].vcs_position.y = -7.5F;

   /** \action
    * Call function
    */
   for (unsigned int det_idx = 0; det_idx < raw_dets_list.number_of_valid_detections; det_idx++)
   {
      Mark_Detection_On_SEP(sep, calibs, det_props[det_idx]);
   }
   

   /** \result
    * Verify that detection 0 is flagged as on curve id 1
    * Verify that detection 1 is flagged as on curve id 2
    * Verify that detection 2 is not flagged as on any curve
    */
   CHECK_EQUAL(1U, det_props[0].on_sep_id);
   CHECK_EQUAL(2U, det_props[1].on_sep_id);
   CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, det_props[2].on_sep_id);
}

/** \purpose
 * Verify that function correctly flags detections as on the correct curve when 1 detection is on curve on right side,
 * 1 detection is in front of host and 1 detection is on curve on left side. One curve is invalid.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Flag_Detections_On_SEP, Flag_Dets_On_Longi_Stat_Curves_Dets_Left_Center_Right_One_Invalid_Curve)
{
   /** \precond
    * Create detections
    *    - Detection 0 is on curve 3 (curve id 4)
    *    - Detection 1 is in front of host and should thus not be flagged as on
    *    - Detection 2 is on curve 0 (curve id 1)
    * Set curve 2 (curve id 3) as invalid
    */
   det_props[0].vcs_position.x = 10.0F;
   det_props[0].vcs_position.y = 10.0F;
   det_props[1].vcs_position.x = 10.0F;
   det_props[1].vcs_position.y = 0.0F;
   det_props[2].vcs_position.x = 0.0F;
   det_props[2].vcs_position.y = -5.0F;
   sep[2].status = F360_STATIC_ENV_POLY_STATUS_INVALID;

   /** \action
    * Call function
    */
   for (unsigned int det_idx = 0; det_idx < raw_dets_list.number_of_valid_detections; det_idx++)
   {
      Mark_Detection_On_SEP(sep, calibs, det_props[det_idx]);
   }

   /** \result
    * Verify that detection 0 is flagged as on curve id 4
    * Verify that detection 1 is not flagged as on any curve
    * Verify that detection 2 is flagged as on curve id 1
    */
   CHECK_EQUAL(4U, det_props[0].on_sep_id);
   CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, det_props[1].on_sep_id);
   CHECK_EQUAL(1U, det_props[2].on_sep_id);
}

/** \purpose
 * Verify that function ignores detections that are flagged as not ok to use.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Flag_Detections_On_SEP, Flag_Dets_On_Longi_Stat_Curves_Det_Not_Ok_To_Use)
{
   /** \precond
    * Set one detections position as on curve
    * Set the detection as not ok to use
    */
   raw_dets_list.number_of_valid_detections = 1U;
   det_props[0].vcs_position.x = 10.0F;
   det_props[0].vcs_position.y = 10.0F;
   det_props[0].f_ok_to_use = false;


   /** \action
    * Call function
    */
   for (unsigned int det_idx = 0; det_idx < raw_dets_list.number_of_valid_detections; det_idx++)
   {
      Mark_Detection_On_SEP(sep, calibs, det_props[det_idx]);
   }

   /** \result
    * Verify that detection 0 is not flagged as on any curve
    */
   CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, det_props[0].on_sep_id);
}
/** @}*/


/** \defgroup  f360_sep_support_functions_Check_If_Point_Is_Behind_SEP
 *  @{
 */

 /** \brief
  * This test group sets up 4 arbitrary SEPs, two on each side of host.
  * It also initializes start and end points of a vector which end point shall
  * be tried as behind the curve or not. This vector is also expressed as a
  * linear equation on the form y = kx + m.
  */
TEST_GROUP(f360_sep_support_functions_Check_If_Point_Is_Behind_SEP)
{
   float32_t point_x_start;
   float32_t point_x_end;
   float32_t point_y_end;
   float32_t k;
   float32_t m;
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS];
   F360_Calibrations_T calibs;

   // Function support variable
   float32_t point_y_start;

   // Function output
   bool f_behind_curve;
   Point intersection_point = {};

   // Expected ouput
   Point exp_intersection_point = {};

   float32_t TEST_PASS_TH = 1.0e-4F;

   /** \setup
    * Initialize calibrations and create the SEPs.
    * Also set up start and end points of vector and express it
    * as a linear equation.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      Create_Four_SEP(sep);

   }
};

/** \purpose
 * Verify that a point is correctly flagged as behind the furthest SEP on host right side.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_Behind_SEP, Check_If_Point_Is_Behind_SEP_Point_Behind_Straight_Curve_Far_Right_Side)
{
   /** \precond
    * Create start and end points and express the vector as a linear equation.
    */
   point_x_start = 0.0F;
   point_y_start = 0.0F;
   point_x_end = 10.0F;
   point_y_end = 15.0F;

   Get_Vector_As_Linear_Equation(point_x_start, point_y_start, point_x_end, point_y_end, k, m);

   exp_intersection_point.x = 6.666666667F;
   exp_intersection_point.y = 10.0F;

   /** \action
    * Call function
    */
   f_behind_curve = Check_If_Point_Is_Behind_SEP(point_x_start, point_x_end, point_y_start, point_y_end, k, m, sep[3], calibs, intersection_point);

   /** \result
    * Verify that point is flagged as behind curve
    */
   CHECK_TRUE(f_behind_curve);
   DOUBLES_EQUAL(exp_intersection_point.x, intersection_point.x, TEST_PASS_TH);
   DOUBLES_EQUAL(exp_intersection_point.y, intersection_point.y, TEST_PASS_TH);
}

/** \purpose
 * Verify that a point is correctly not flagged as behind the furthest SEP on host right side when
 * point is in front of the curve
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_Behind_SEP, Check_If_Point_Is_Behind_SEP_Point_In_Front_Of_Straight_Curve_Far_Right_Side)
{
   /** \precond
    * Create start and end points and express the vector as a linear equation.
    */
   point_x_start = 0.0F;
   point_y_start = 0.0F;
   point_x_end = 10.0F;
   point_y_end = 5.0F;

   Get_Vector_As_Linear_Equation(point_x_start, point_y_start, point_x_end, point_y_end, k, m);

   exp_intersection_point.x = INFTY;
   exp_intersection_point.y = INFTY;
   /** \action
    * Call function
    */
   f_behind_curve = Check_If_Point_Is_Behind_SEP(point_x_start, point_x_end, point_y_start, point_y_end, k, m, sep[3], calibs, intersection_point);

   /** \result
    * Verify that point is not flagged as behind curve but have a valid intersection point
    */
   CHECK_FALSE(f_behind_curve);
   DOUBLES_EQUAL(exp_intersection_point.x, intersection_point.x, TEST_PASS_TH);
   DOUBLES_EQUAL(exp_intersection_point.y, intersection_point.y, TEST_PASS_TH);
}

/** \purpose
 * Verify that a point is correctly not flagged as behind the furthest SEP on host right side when
 * point vector is parallel to the curve
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_Behind_SEP, Check_If_Point_Is_Behind_SEP_Point_Parallel_Curve_Far_Right_Side)
{
   /** \precond
    * Create start and end points and express the vector as a linear equation.
    */
   point_x_start = 0.0F;
   point_y_start = 0.0F;
   point_x_end = 10.0F;
   point_y_end = 0.0F;

   Get_Vector_As_Linear_Equation(point_x_start, point_y_start, point_x_end, point_y_end, k, m);

   exp_intersection_point.x = INFTY;
   exp_intersection_point.y = INFTY;
   /** \action
    * Call function
    */
   f_behind_curve = Check_If_Point_Is_Behind_SEP(point_x_start, point_x_end, point_y_start, point_y_end, k, m, sep[3], calibs, intersection_point);

   /** \result
    * Verify that point is not flagged as behind curve
    */
   CHECK_FALSE(f_behind_curve);
   DOUBLES_EQUAL(exp_intersection_point.x, intersection_point.x, TEST_PASS_TH);
   DOUBLES_EQUAL(exp_intersection_point.y, intersection_point.y, TEST_PASS_TH);
}

/** \purpose
 * Verify that a point is correctly flagged as behind the closest SEP on host right side when
 * end point is in positive longitudinal plane. Meaning that the first intersection solution to
 * the polynomial should be chosen.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_Behind_SEP, Check_If_Point_Is_Behind_SEP_Point_Positive_Long_Plane_Closest_SEP_Right)
{
   /** \precond
    * Create start and end points and express the vector as a linear equation.
    */
   point_x_start = 0.0F;
   point_y_start = 0.0F;
   point_x_end = 10.0F;
   point_y_end = 10.0F;

   Get_Vector_As_Linear_Equation(point_x_start, point_y_start, point_x_end, point_y_end, k, m);

   exp_intersection_point.x = 4.926465286907814F;
   exp_intersection_point.y = 4.926465286907814F;
   /** \action
    * Call function
    */
   f_behind_curve = Check_If_Point_Is_Behind_SEP(point_x_start, point_x_end, point_y_start, point_y_end, k, m, sep[2], calibs, intersection_point);

   /** \result
    * Verify that point is flagged as behind curve
    */
   CHECK_TRUE(f_behind_curve);
   DOUBLES_EQUAL(exp_intersection_point.x, intersection_point.x, TEST_PASS_TH);
   DOUBLES_EQUAL(exp_intersection_point.y, intersection_point.y, TEST_PASS_TH);
}

/** \purpose
 * Verify that a point is correctly flagged as behind the closest SEP on host right side when
 * end point is in negative longitudinal plane. Meaning that the second intersection solution to
 * the polynomial should be chosen.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_Behind_SEP, Check_If_Point_Is_Behind_SEP_Point_Negative_Long_Plane_Closest_SEP_Right)
{
   /** \precond
    * Create start and end points and express the vector as a linear equation.
    */
   point_x_start = 0.0F;
   point_y_start = 0.0F;
   point_x_end = -10.0F;
   point_y_end = 10.0F;

   Get_Vector_As_Linear_Equation(point_x_start, point_y_start, point_x_end, point_y_end, k, m);

   exp_intersection_point.x = -5.024999375031248F;
   exp_intersection_point.y = 5.024999375031248F;
   /** \action
    * Call function
    */
   f_behind_curve = Check_If_Point_Is_Behind_SEP(point_x_start, point_x_end, point_y_start, point_y_end, k, m, sep[2], calibs, intersection_point);

   /** \result
    * Verify that point is flagged as behind curve
    */
   CHECK_TRUE(f_behind_curve);
   DOUBLES_EQUAL(exp_intersection_point.x, intersection_point.x, TEST_PASS_TH);
   DOUBLES_EQUAL(exp_intersection_point.y, intersection_point.y, TEST_PASS_TH);
}

/** \purpose
 * Verify that a point is correctly flagged as behind the closest SEP on host right side when
 * start and end points in x-direction is equal (yielding a very large k value)
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_Behind_SEP, Check_If_Point_Is_Behind_SEP_Point_Large_K_Right_Side_Behind)
{
   /** \precond
    * Create start and end points and express the vector as a linear equation.
    */
   point_x_start = 0.0F;
   point_y_start = 0.0F;
   point_x_end = 0.0F;
   point_y_end = 10.0F;

   Get_Vector_As_Linear_Equation(point_x_start, point_y_start, point_x_end, point_y_end, k, m);

   exp_intersection_point.x = point_x_end;
   exp_intersection_point.y = sep[2].p0;
   /** \action
    * Call function
    */
   f_behind_curve = Check_If_Point_Is_Behind_SEP(point_x_start, point_x_end, point_y_start, point_y_end, k, m, sep[2], calibs, intersection_point);


   /** \result
    * Verify that point is flagged as behind curve
    */
   CHECK_TRUE(f_behind_curve);
   DOUBLES_EQUAL(exp_intersection_point.x, intersection_point.x, TEST_PASS_TH);
   DOUBLES_EQUAL(exp_intersection_point.y, intersection_point.y, TEST_PASS_TH);
}

/** \purpose
 * Verify that a point is not flagged as behind the closest SEP on host right side when
 * start and end points in x-direction is equal (yielding a very large k value) and point is on left side.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_Behind_SEP, Check_If_Point_Is_Behind_SEP_Point_Large_K_Point_On_Opposite_Side)
{
   /** \precond
    * Create start and end points and express the vector as a linear equation.
    */
   point_x_start = 0.0F;
   point_y_start = 0.0F;
   point_x_end = 0.0F;
   point_y_end = -10.0F;

   Get_Vector_As_Linear_Equation(point_x_start, point_y_start, point_x_end, point_y_end, k, m);

   exp_intersection_point.x = INFTY;
   exp_intersection_point.y = INFTY;
   /** \action
    * Call function
    */
   f_behind_curve = Check_If_Point_Is_Behind_SEP(point_x_start, point_x_end, point_y_start, point_y_end, k, m, sep[2], calibs, intersection_point);

   /** \result
    * Verify that point is not flagged as behind curve
    */
   CHECK_FALSE(f_behind_curve);
   DOUBLES_EQUAL(exp_intersection_point.x, intersection_point.x, TEST_PASS_TH);
   DOUBLES_EQUAL(exp_intersection_point.y, intersection_point.y, TEST_PASS_TH);
}

/** \purpose
 * Verify that a point is correctly flagged as not behind the closest SEP on host right side when
 * start and end points in x-direction is almost equal (yielding a very large k value)
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_Behind_SEP, Check_If_Point_Is_Behind_SEP_Point_Large_K_Right_Side_Not_Behind)
{
   /** \precond
    * Create start and end points and express the vector as a linear equation.
    */
   point_x_start = 0.0F;
   point_y_start = 0.0F;
   point_x_end = 0.0F;
   point_y_end = 4.0F;

   Get_Vector_As_Linear_Equation(point_x_start, point_y_start, point_x_end, point_y_end, k, m);

   exp_intersection_point.x = INFTY;
   exp_intersection_point.y = INFTY;
   /** \action
    * Call function
    */
   f_behind_curve = Check_If_Point_Is_Behind_SEP(point_x_start, point_x_end, point_y_start, point_y_end, k, m, sep[2], calibs, intersection_point);

   /** \result
    * Verify that point is not flagged as behind curve
    */
   CHECK_FALSE(f_behind_curve);
   DOUBLES_EQUAL(exp_intersection_point.x, intersection_point.x, TEST_PASS_TH);
   DOUBLES_EQUAL(exp_intersection_point.y, intersection_point.y, TEST_PASS_TH);
}

/** \purpose
 * Verify that a point is correctly flagged as behind the closest SEP on host left side when
 * start and end points in x-direction is almost equal (yielding a very large k value)
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_Behind_SEP, Check_If_Point_Is_Behind_SEP_Point_Large_K_Left_Side_Behind)
{
   /** \precond
    * Create start and end points and express the vector as a linear equation.
    */
   point_x_start = 0.0F;
   point_y_start = 0.0F;
   point_x_end = 0.0F;
   point_y_end = -10.0F;

   Get_Vector_As_Linear_Equation(point_x_start, point_y_start, point_x_end, point_y_end, k, m);

   exp_intersection_point.x = point_x_end;
   exp_intersection_point.y = sep[0].p0;
   /** \action
    * Call function
    */
   f_behind_curve = Check_If_Point_Is_Behind_SEP(point_x_start, point_x_end, point_y_start, point_y_end, k, m, sep[0], calibs, intersection_point);

   /** \result
    * Verify that point is flagged as behind curve
    */
   CHECK_TRUE(f_behind_curve);
   DOUBLES_EQUAL(exp_intersection_point.x, intersection_point.x, TEST_PASS_TH);
   DOUBLES_EQUAL(exp_intersection_point.y, intersection_point.y, TEST_PASS_TH);
}

/** \purpose
 * Verify that a point is correctly flagged as not behind the closest SEP on host left side when
 * start and end points in x-direction is almost equal (yielding a very large k value)
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_Behind_SEP, Check_If_Point_Is_Behind_SEP_Point_Large_K_Left_Side_Not_Behind)
{
   /** \precond
    * Create start and end points and express the vector as a linear equation.
    */
   point_x_start = 0.0F;
   point_y_start = 0.0F;
   point_x_end = 0.0F;
   point_y_end = -2.0F;

   Get_Vector_As_Linear_Equation(point_x_start, point_y_start, point_x_end, point_y_end, k, m);

   /** \action
    * Call function
    */
   f_behind_curve = Check_If_Point_Is_Behind_SEP(point_x_start, point_x_end, point_y_start, point_y_end, k, m, sep[0], calibs, intersection_point);

   exp_intersection_point.x = INFTY;
   exp_intersection_point.y = INFTY;
   /** \result
    * Verify that point is not flagged as behind curve
    */
   CHECK_FALSE(f_behind_curve);
   DOUBLES_EQUAL(exp_intersection_point.x, intersection_point.x, TEST_PASS_TH);
   DOUBLES_EQUAL(exp_intersection_point.y, intersection_point.y, TEST_PASS_TH);
}

/** \purpose
 * Verify that a point is correctly flagged as not behind the closest SEP on host left side when
 * start and end points in x-direction is equal (yielding a very large k value) but SEP valid interval
 * is smaller than point.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_Behind_SEP, Check_If_Point_Is_Behind_SEP_Point_Large_K_Left_Side_Not_In_Valid_SEP_Interval_Low)
{
   /** \precond
    * Create start and end points and express the vector as a linear equation.
    */
   point_x_start = 0.0F;
   point_y_start = 0.0F;
   point_x_end = 0.0F;
   point_y_end = -10.0F;

   sep[0].lower_limit = -50.0F;
   sep[0].upper_limit = -25.0F;

   Get_Vector_As_Linear_Equation(point_x_start, point_y_start, point_x_end, point_y_end, k, m);

   /** \action
    * Call function
    */
   f_behind_curve = Check_If_Point_Is_Behind_SEP(point_x_start, point_x_end, point_y_start, point_y_end, k, m, sep[0], calibs, intersection_point);

   exp_intersection_point.x = INFTY;
   exp_intersection_point.y = INFTY;
   /** \result
    * Verify that point is not flagged as behind curve
    */
   CHECK_FALSE(f_behind_curve);
   DOUBLES_EQUAL(exp_intersection_point.x, intersection_point.x, TEST_PASS_TH);
   DOUBLES_EQUAL(exp_intersection_point.y, intersection_point.y, TEST_PASS_TH);
}

/** \purpose
 * Verify that a point is correctly flagged as not behind the closest SEP on host left side when
 * start and end points in x-direction is almost equal (yielding a very large k value) but SEP valid interval
 * is larger than point.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_Behind_SEP, Check_If_Point_Is_Behind_SEP_Point_Large_K_Left_Side_Not_In_Valid_SEP_Interval_High)
{
   /** \precond
    * Create start and end points and express the vector as a linear equation.
    */
   point_x_start = 0.0F;
   point_y_start = 0.0F;
   point_x_end = 0.0F;
   point_y_end = -10.0F;

   sep[0].lower_limit = 25.0F;
   sep[0].upper_limit = 50.0F;

   Get_Vector_As_Linear_Equation(point_x_start, point_y_start, point_x_end, point_y_end, k, m);

   /** \action
    * Call function
    */
   f_behind_curve = Check_If_Point_Is_Behind_SEP(point_x_start, point_x_end, point_y_start, point_y_end, k, m, sep[0], calibs, intersection_point);

   exp_intersection_point.x = INFTY;
   exp_intersection_point.y = INFTY;
   /** \result
    * Verify that point is not flagged as behind curve
    */
   CHECK_FALSE(f_behind_curve);
   DOUBLES_EQUAL(exp_intersection_point.x, intersection_point.x, TEST_PASS_TH);
   DOUBLES_EQUAL(exp_intersection_point.y, intersection_point.y, TEST_PASS_TH);
}

/** \purpose
 * Verify that a point is correctly flagged as behind the closest SEP on host right side when
 * the intersection lies on the valid interval of the curve is close to the curve's end point
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_Behind_SEP, Check_If_Point_Is_Behind_SEP_Point_Intersection_In_Max_Hyst_Right)
{
   /** \precond
    * Create start and end points and express the vector as a linear equation.
    * Decrease valid interval of curve 2
    */
   point_x_start = 0.0F;
   point_y_start = 0.0F;
   point_x_end = 10.0F;
   point_y_end = 8.0F;

   sep[2].upper_limit = 10.0F;

   Get_Vector_As_Linear_Equation(point_x_start, point_y_start, point_x_end, point_y_end, k, m);

   exp_intersection_point.x = 6.12649536F;
   exp_intersection_point.y = 4.90120125F;
   /** \action
    * Call function
    */
   f_behind_curve = Check_If_Point_Is_Behind_SEP(point_x_start, point_x_end, point_y_start, point_y_end, k, m, sep[2], calibs, intersection_point);

   /** \result
    * Verify that point is flagged as behind curve
    */
   CHECK_TRUE(f_behind_curve);
   DOUBLES_EQUAL(exp_intersection_point.x, intersection_point.x, TEST_PASS_TH);
   DOUBLES_EQUAL(exp_intersection_point.y, intersection_point.y, TEST_PASS_TH);
}

/** \purpose
 * Verify that a point slightly above valid SEP interval is not flagged as behind the SEP.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_Behind_SEP, Check_If_Point_Is_Behind_SEP_Point_Intersection_Above_Max_Right)
{
   /** \precond
    * Decrease x max valid interval of curve 2
    * Set start point in vcs origin
    * Set end point slightly above SEP max position
    * Find k and m to express start and end point as a linear equation.
    */
   sep[2].upper_limit = 10.0F;

   point_x_start = 0.0F;
   point_y_start = 0.0F;
   point_x_end = sep[2].upper_limit  + 1.0F;
   point_y_end = sep[2].p0 + 0.1F;


   Get_Vector_As_Linear_Equation(point_x_start, point_y_start, point_x_end, point_y_end, k, m);

   exp_intersection_point.x = INFTY;
   exp_intersection_point.y = INFTY;
   /** \action
    * Call function
    */
   f_behind_curve = Check_If_Point_Is_Behind_SEP(point_x_start, point_x_end, point_y_start, point_y_end, k, m, sep[2], calibs, intersection_point);

   /** \result
    * Verify that point is not flagged as behind curve
    */
   CHECK_FALSE(f_behind_curve);
   DOUBLES_EQUAL(exp_intersection_point.x, intersection_point.x, TEST_PASS_TH);
   DOUBLES_EQUAL(exp_intersection_point.y, intersection_point.y, TEST_PASS_TH);
}

/** \purpose
 * Verify that a point slightly below valid SEP interval is not flagged as behind the SEP.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_Behind_SEP, Check_If_Point_Is_Behind_SEP_Point_Intersection_Below_Max_Right)
{
   /** \precond
    * Decrease x min valid interval of curve 2
    * Set start point in vcs origin
    * Set end point slightly below SEP min position
    * Find k and m to express start and end point as a linear equation.
    */
   sep[2].lower_limit = -10.0F;

   point_x_start = 0.0F;
   point_y_start = 0.0F;
   point_x_end = sep[2].lower_limit - 1.0F;
   point_y_end = sep[2].p0 + 0.1F;


   Get_Vector_As_Linear_Equation(point_x_start, point_y_start, point_x_end, point_y_end, k, m);

   exp_intersection_point.x = INFTY;
   exp_intersection_point.y = INFTY;
   /** \action
    * Call function
    */
   f_behind_curve = Check_If_Point_Is_Behind_SEP(point_x_start, point_x_end, point_y_start, point_y_end, k, m, sep[2], calibs, intersection_point);

   /** \result
    * Verify that point is not flagged as behind curve
    */
   CHECK_FALSE(f_behind_curve);
   DOUBLES_EQUAL(exp_intersection_point.x, intersection_point.x, TEST_PASS_TH);
   DOUBLES_EQUAL(exp_intersection_point.y, intersection_point.y, TEST_PASS_TH);
}

/** \purpose
 * Verify that a point is correctly flagged as behind the closest SEP on host right side when
 * the intersection lies on the valid interval of the curve and is close to the curve's start point
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_Behind_SEP, Check_If_Point_Is_Behind_SEP_Intersection_In_Min_Hyst_Right)
{
   /** \precond
    * Create start and end points and express the vector as a linear equation.
    * Decrease valid interval of curve 2
    */
   point_x_start = 0.0F;
   point_y_start = 0.0F;
   point_x_end = -10.0F;
   point_y_end = 8.0F;

   sep[2].lower_limit = -10.0F;

   Get_Vector_As_Linear_Equation(point_x_start, point_y_start, point_x_end, point_y_end, k, m);

   exp_intersection_point.x = -6.27920532F;
   exp_intersection_point.y = 5.02336359F;
   /** \action
    * Call function
    */
   f_behind_curve = Check_If_Point_Is_Behind_SEP(point_x_start, point_x_end, point_y_start, point_y_end, k, m, sep[2], calibs, intersection_point);

   /** \result
    * Verify that point is not flagged as behind curve
    */
   CHECK_TRUE(f_behind_curve);
   DOUBLES_EQUAL(exp_intersection_point.x, intersection_point.x, TEST_PASS_TH);
   DOUBLES_EQUAL(exp_intersection_point.y, intersection_point.y, TEST_PASS_TH);
}

/** \purpose
 * Verify that a point is correctly flagged as behind a SEP when there are 2 valid intersections and verify that the intersection point closest to host is choosen.
 * This test is for the code branch where the first found intersection point is the closest one.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_Behind_SEP, Check_If_Point_Is_Behind_SEP_2_Intersections_Both_Valid_First_Is_Closest)
{
   /** \precond
    * Create start and end points and express the vector as a linear equation.
    * Create a SEP which has 2 valid intersection points to the line defined by the start and end point.
    * The test is setup such that the intersection points are [10, 0] and [40, 0].
    */
   point_x_start = 0.0F;
   point_y_start = 0.0F;
   point_x_end = 50.0F; // Set to be further away than the furthest intersection point but smaller than sep upper limit such that both intersection points are valid
   point_y_end = 0.0F;

   sep[2].p0 = 400.0F;
   sep[2].p1 = -50.0F;
   sep[2].p2 = 1.0F;
   sep[2].upper_limit = 60.0F; // Set to be further away than the end point such that the furthest intersection point is inside valid SEP limits
   sep[2].lower_limit = 5.0F; // Set to be closer than the closest intersection such that the closest intersection point is inside valid SEP limits
   

   Get_Vector_As_Linear_Equation(point_x_start, point_y_start, point_x_end, point_y_end, k, m);
   
   /** \action
    * Call function Check_If_Point_Is_Behind_SEP()
    */
   f_behind_curve = Check_If_Point_Is_Behind_SEP(point_x_start, point_x_end, point_y_start, point_y_end, k, m, sep[2], calibs, intersection_point);

   /** \result
    * Verify that point is flagged as behind curve and that the intersection point is the one with smallest x coordinate (closest to host), i.e. [10, 0]
    */
   exp_intersection_point.x = 10.0F;
   exp_intersection_point.y = 0.0F;
   CHECK_TRUE(f_behind_curve);
   DOUBLES_EQUAL(exp_intersection_point.x, intersection_point.x, TEST_PASS_TH);
   DOUBLES_EQUAL(exp_intersection_point.y, intersection_point.y, TEST_PASS_TH);
}

/** \purpose
 * Verify that a point is correctly flagged as behind a SEP when there are 2 valid intersections and verify that the intersection point closest to host is choosen.
 * this test is for the code branch where the second found intersection point is the closest one.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_Behind_SEP, Check_If_Point_Is_Behind_SEP_2_Intersections_Both_Valid_Second_Is_Closest)
{
   /** \precond
    * Create start and end points and express the vector as a linear equation.
    * Create a SEP which has 2 valid intersection points to the line defined by the start and end point.
    * The test is setup such that the intersection points are [-10, 0] and [-40, 0].
    */
   point_x_start = 0.0F;
   point_y_start = 0.0F;
   point_x_end = -50.0F; // Set to be further away than the furthest intersection point but smaller than sep upper limit such that both intersection points are valid
   point_y_end = 0.0F;

   sep[2].p0 = 400.0F;
   sep[2].p1 = 50.0F;
   sep[2].p2 = 1.0F;
   sep[2].upper_limit = -5.0F; // Set to be further away than the end point such that the furthest intersection point is inside valid SEP limits
   sep[2].lower_limit = -60.0F; // Set to be closer than the closest intersection such that the closest intersection point is inside valid SEP limits
   

   Get_Vector_As_Linear_Equation(point_x_start, point_y_start, point_x_end, point_y_end, k, m);
   
   /** \action
    * Call function Check_If_Point_Is_Behind_SEP()
    */
   f_behind_curve = Check_If_Point_Is_Behind_SEP(point_x_start, point_x_end, point_y_start, point_y_end, k, m, sep[2], calibs, intersection_point);

   /** \result
    * Verify that point is flagged as behind curve and that the intersection point is the one with smallest x coordinate (closest to host), i.e. [10, 0]
    */
   exp_intersection_point.x = -10.0F;
   exp_intersection_point.y = 0.0F;
   CHECK_TRUE(f_behind_curve);
   DOUBLES_EQUAL(exp_intersection_point.x, intersection_point.x, TEST_PASS_TH);
   DOUBLES_EQUAL(exp_intersection_point.y, intersection_point.y, TEST_PASS_TH);
}

/** \purpose
 * Verify that a point is correctly flagged as behind a SEP when there are 2 intersections but only the closest one is valid and
 * verify that the intersection point closest to host is choosen.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_Behind_SEP, Check_If_Point_Is_Behind_SEP_2_Intersections_Closest_Valid)
{
   /** \precond
    * Create start and end points and express the vector as a linear equation.
	* Create a SEP which has 2 intersection points to the line defined by the start and end point but only the closest one is a valid intersection point
	* The test is setup such that the intersection points are [10, 0] and [40, 0].
    */
   point_x_start = 0.0F;
   point_y_start = 0.0F;
   point_x_end = 30.0F; // Set to be closer than the furthest intersection point but further away than closest intersection point to invalidate the furthest away intersection
   point_y_end = 0.0F;

   sep[2].p0 = 400.0F;
   sep[2].p1 = -50.0F;
   sep[2].p2 = 1.0F;
   sep[2].upper_limit = 60.0F; // Set to be further away than the end point such that the furthest intersection point is inside valid SEP limits
   sep[2].lower_limit = 5.0F; // Set to be closer than the closest intersection such that the closest intersection point is inside valid SEP limits
   

   Get_Vector_As_Linear_Equation(point_x_start, point_y_start, point_x_end, point_y_end, k, m);
   
   /** \action
    * Call function
    */
   f_behind_curve = Check_If_Point_Is_Behind_SEP(point_x_start, point_x_end, point_y_start, point_y_end, k, m, sep[2], calibs, intersection_point);

   /** \result
    * Verify that point is flagged as behind curve and that the intersection point is the one with smallest x coordinate (closest to host), i.e. [10, 0]
    */
   exp_intersection_point.x = 10.0F;
   exp_intersection_point.y = 0.0F;
   CHECK_TRUE(f_behind_curve);
   DOUBLES_EQUAL(exp_intersection_point.x, intersection_point.x, TEST_PASS_TH);
   DOUBLES_EQUAL(exp_intersection_point.y, intersection_point.y, TEST_PASS_TH);
}

/** \purpose
 * Verify that a point is correctly flagged as behind a SEP when there are 2 intersections but only the furthest away one is valid and
 * verify that the intersection point furthest away from host is choosen.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_Behind_SEP, Check_If_Point_Is_Behind_SEP_2_Intersections_Furthest_Valid)
{
   /** \precond
    * Create start and end points and express the vector as a linear equation.
	* Create a SEP which has 2 intersection points to the line defined by the start and end point but only the furthest away one is a valid intersection point
	* The test is setup such that the intersection points are [10, 0] and [40, 0].
    */
   point_x_start = 0.0F;
   point_y_start = 0.0F;
   point_x_end = 50.0F; // Set to be further away than the furthest intersection point but smaller than sep upper limit such that furthest away intersection point is valid valid
   point_y_end = 0.0F;

   sep[2].p0 = 400.0F;
   sep[2].p1 = -50.0F;
   sep[2].p2 = 1.0F;
   sep[2].upper_limit = 60.0F; // Set to be further away than the end point such that the furthest intersection point is inside valid SEP limits
   sep[2].lower_limit = 15.0F; // Set to be larger than the closest intersection point such that the closest intersection point is outside of valid SEP limits
   

   Get_Vector_As_Linear_Equation(point_x_start, point_y_start, point_x_end, point_y_end, k, m);
   
   /** \action
    * Call function
    */
   f_behind_curve = Check_If_Point_Is_Behind_SEP(point_x_start, point_x_end, point_y_start, point_y_end, k, m, sep[2], calibs, intersection_point);

   /** \result
    * Verify that point is flagged as behind curve and that the intersection point is the one with largest x coordinate (furthest away from host), i.e. [40, 0]
    */
   exp_intersection_point.x = 40.0F;
   exp_intersection_point.y = 0.0F;
   CHECK_TRUE(f_behind_curve);
   DOUBLES_EQUAL(exp_intersection_point.x, intersection_point.x, TEST_PASS_TH);
   DOUBLES_EQUAL(exp_intersection_point.y, intersection_point.y, TEST_PASS_TH);
}

/** \purpose
 * Verify that a point is correctly flagged as not behind a SEP when there are 2 intersections but none is valid and
 * verify that the intersection point is set to INFTY.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Check_If_Point_Is_Behind_SEP, Check_If_Point_Is_Behind_SEP_2_Intersections_None_Valid)
{
   /** \precond
    * Create start and end points and express the vector as a linear equation.
	* Create a SEP which has 2 intersection points to the line defined by the start and end point but only the furthest away one is a valid intersection point
	* The test is setup such that the intersection points are [10, 0] and [40, 0].
    */
   point_x_start = 0.0F;
   point_y_start = 0.0F;
   point_x_end = 50.0F; // Set to be further away than the furthest intersection point
   point_y_end = 0.0F;

   sep[2].p0 = 400.0F;
   sep[2].p1 = -50.0F;
   sep[2].p2 = 1.0F;
   sep[2].upper_limit = 35.0F; // Set to be closer than the furthest away intersection point such that the furthest intersection point is outside valid SEP limits
   sep[2].lower_limit = 15.0F; // Set to be larger than the closest intersection point such that the closest intersection point is outside of valid SEP limits
   

   Get_Vector_As_Linear_Equation(point_x_start, point_y_start, point_x_end, point_y_end, k, m);
   
   /** \action
    * Call function
    */
   f_behind_curve = Check_If_Point_Is_Behind_SEP(point_x_start, point_x_end, point_y_start, point_y_end, k, m, sep[2], calibs, intersection_point);

   /** \result
    * Verify that point is flagged as not behind curve and that the intersection point is set to INFTY
    */
   exp_intersection_point.x = INFTY;
   exp_intersection_point.y = INFTY;
   CHECK_FALSE(f_behind_curve);
   DOUBLES_EQUAL(exp_intersection_point.x, intersection_point.x, TEST_PASS_TH);
   DOUBLES_EQUAL(exp_intersection_point.y, intersection_point.y, TEST_PASS_TH);
}

/** @}*/


/** \defgroup  f360_sep_support_functions_Flag_Detections_Behind_SEP
 *  @{
 */

/** \brief
  * This test group sets up 4 arbitrary SEPs, two on each side of host.
  * It also initializes 3 detections which changes position in the various
  * test cases.
  */
TEST_GROUP(f360_sep_support_functions_Flag_Detections_Behind_SEP)
{
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   rspp_variant_A::RSPP_Detection_List_T raw_dets_list;
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS];
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS];
   F360_Calibrations_T calibs;

   /** \setup
    * Initialize calibrations and create the SEPs.
    * Also set up 3 detections
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      Create_Four_SEP(sep);

      raw_dets_list.number_of_valid_detections = 3U;

      // Set properties of the detections so they are valid to be flagged as behind curves, only f_ok_to_use
      // detections are flagged as behind curve and detections cannot be both flagged as on and behind a curve.
      for (uint32_t i = 0; i < raw_dets_list.number_of_valid_detections; i++)
      {
         raw_dets_list.detections[i].raw.sensor_id = 1;
         det_props[i].f_ok_to_use = true;
         det_props[i].on_sep_id = F360_INVALID_UNSIGNED_ID;
      }

      // Set sensor position
      sensors[0].constant.mounting_position.vcs_position.longitudinal = -1.0F;
      sensors[0].constant.mounting_position.vcs_position.lateral = 1.0F;
   }
};

/** \purpose
 * Verify that flagging of detections as behind curves works as expected when
 *    - Detection 0 is flagged as not ok to use, should not be flagged as behind curve 2
 *    - Detection 1 is behind curve on right side, should be flagged
 *    - Detection 2 is already flagged as "on", should not be flagged
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Flag_Detections_Behind_SEP, Flag_Detections_Behind_SEP_One_Behind_One_Not_Ok_To_Use_One_Is_On)
{
   /** \precond
    * Set position and properties of detections
    */
   det_props[0].vcs_position.x = 0.0F;
   det_props[0].vcs_position.y = 8.0F;
   det_props[0].f_ok_to_use = false;

   det_props[1].vcs_position.x = 0.0F;
   det_props[1].vcs_position.y = 8.0F;

   det_props[2].vcs_position.x = 0.0F;
   det_props[2].vcs_position.y = 8.0F;
   det_props[2].on_sep_id = 1U;


   /** \action
    * Call function
    */
   for (unsigned int det_idx = 0; det_idx < raw_dets_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_detection = raw_dets_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = det_props[det_idx];
      const int32_t current_sensor_id = current_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      Mark_Detection_Behind_SEP(current_sensor, sep, calibs, current_detection_prop);
   }

   /** \result
    * Verify that detections are flagged as expected
    */
   CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, det_props[0].behind_sep_id);
   CHECK_EQUAL(3U, det_props[1].behind_sep_id);
   CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, det_props[2].behind_sep_id);
}

/** \purpose
 * Verify that flagging of detections as behind curves works as expected when
 *    - Detection 0 is in front of host, should not be flagged
 *    - Detection 1 is behind curve on right side, should be flagged
 *    - Detection 0 is behind curve on left side, should be flagged
 *    - Curve 1 is invalid
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Flag_Detections_Behind_SEP, Flag_Detections_Behind_SEP_One_Behind_Each_Side_One_In_Front_One_Invalid_Curve)
{
   /** \precond
    * Set position and properties of detections
    */
   det_props[0].vcs_position.x = 10.0F;
   det_props[0].vcs_position.y = 0.0F;

   det_props[1].vcs_position.x = 0.0F;
   det_props[1].vcs_position.y = 8.0F;

   det_props[2].vcs_position.x = 0.0F;
   det_props[2].vcs_position.y = -8.0F;

   sep[1].status = F360_STATIC_ENV_POLY_STATUS_INVALID;

   /** \action
    * Call function
    */
   for (unsigned int det_idx = 0; det_idx < raw_dets_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_detection = raw_dets_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = det_props[det_idx];
      const int32_t current_sensor_id = current_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      Mark_Detection_Behind_SEP(current_sensor, sep, calibs, current_detection_prop);
   }

   /** \result
    * Verify that detections are flagged as expected
    */
   CHECK_EQUAL(F360_INVALID_UNSIGNED_ID, det_props[0].behind_sep_id);
   CHECK_EQUAL(3U, det_props[1].behind_sep_id);
   CHECK_EQUAL(1U, det_props[2].behind_sep_id);
}

/** \purpose
 * Verify that a detection is correctly flagged as behind a SEP when the detection originates from forward right sensor and has a negative lateral position
 * and SEP is in front of host with a small positive lateral position.
 * \req
 * NA.
 */
TEST(f360_sep_support_functions_Flag_Detections_Behind_SEP, Flag_Detections_Behind_SEP_Detection_From_FR_Behind_SEP_In_Front_Of_Host)
{
   /** \precond
    * The following has been set up in test group:
    * - Three detections have been set up to be eligible for marking as behind SEP (ok to use, not on lsc).
    * - The three detections have been assigned sensor id 1
    * - Vcs position of sensor with ID 1 has been set to (-1, 1)
    * Create a SEP with ID 5 in front of host with a slight positive lateral position spanning longitudinally from 1m to 10m.
    * Set SEP with idx 1-3 to invalid
    * Set vcs position of first detection to (15, -2)
    */

   sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
   sep[0].lower_limit = 1.0F;
   sep[0].upper_limit = 10.0F;
   sep[0].p2 = 0.0F;
   sep[0].p1 = 0.0F;
   sep[0].p0 = 0.5F;

   sep[1].status = F360_STATIC_ENV_POLY_STATUS_INVALID;
   sep[2].status = F360_STATIC_ENV_POLY_STATUS_INVALID;
   sep[3].status = F360_STATIC_ENV_POLY_STATUS_INVALID;

   det_props[0].vcs_position.x = 15.0F;
   det_props[0].vcs_position.y = -2.0F;

   /** \action
    * Call function
    */
   for (unsigned int det_idx = 0; det_idx < raw_dets_list.number_of_valid_detections; det_idx++)
   {
      const rspp_variant_A::RSPP_Detection_T &current_detection = raw_dets_list.detections[det_idx];
      F360_Detection_Props_T &current_detection_prop = det_props[det_idx];
      const int32_t current_sensor_id = current_detection.raw.sensor_id;
      const F360_Radar_Sensor_T &current_sensor = sensors[current_sensor_id - 1];
      Mark_Detection_Behind_SEP(current_sensor, sep, calibs, current_detection_prop);
   }

   /** \result
    * Check that first detection was flagged as behind SEP with id 1
    */
   CHECK_EQUAL_TEXT(1U, det_props[0].behind_sep_id, "The detection from forward right sensor was not flagged as behind SEP when it should have been.");
}


/** @}*/
