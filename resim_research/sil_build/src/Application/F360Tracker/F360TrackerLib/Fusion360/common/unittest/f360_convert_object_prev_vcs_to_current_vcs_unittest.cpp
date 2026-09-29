/** \file
   Purpose of this file is to verify the functions defined in f360_convert_object_prev_vcs_to_current_vcs.cpp
*/

#include "f360_convert_object_prev_vcs_to_current_vcs.h"
#include "f360_math.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <cfloat>

using namespace f360_variant_A;

/** \defgroup  f360_test_group_convert_object_states
*  @{
*/
/** \brief
 *  Test group containing data needed for function and expected data when testing conversion of object states
 */
TEST_GROUP(f360_test_group_convert_object_states)
{
   F360_Object_Track_T obj;

   F360_Host_Props_T host_p;

   // Expected data
   float32_t exp_x_pos;
   float32_t exp_y_pos;
   float32_t exp_x_vel;
   float32_t exp_y_vel;
   float32_t exp_x_acc;
   float32_t exp_y_acc;
   float32_t exp_heading;
   float32_t exp_cos_head;
   float32_t exp_sin_head;
   float32_t exp_pointing;
   float32_t exp_cos_pointing;
   float32_t exp_sin_pointing;
   Point exp_prev_vcs_center_pos;
   float32_t exp_filtered_pos_heading;

   // Threshold for passing test
   float32_t thres = 0.0001F;

   /** \setup
   * Setup data of an arbitrary scenario. Assuming host delta pointing is 90 degrees and position delta 1 meter in both x and y creates easily derived expected data.
   */
   TEST_SETUP()
   {
      obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      obj.vcs_position.x = 10.0F;
      obj.vcs_position.y = 0.0F;
      obj.vcs_velocity.longitudinal = 10.0F;
      obj.vcs_velocity.lateral = 0.0F;
      obj.vcs_accel.longitudinal = 1.0F;
      obj.vcs_accel.lateral = 0.0F;
      obj.vcs_heading = Angle{ F360_DEG2RAD(0.0F) };
      obj.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(0.0F) });
      obj.prev_vcs_center_pos.x = 10.0F;
      obj.prev_vcs_center_pos.y = 10.0F;
      obj.filtered_pos_diff_heading = 0.0F;

      // Delta of VCS origin between two tracker iterations
      host_p.delta_pointing = F360_DEG2RAD(90.0F);
      host_p.cos_delta_pointing = F360_Cosf(host_p.delta_pointing);
      host_p.sin_delta_pointing = F360_Sinf(host_p.delta_pointing);
      host_p.delta_position_x = 1.0F;
      host_p.delta_position_y = 1.0F;

      // Expected data
      exp_x_pos = -1.0F;
      exp_y_pos = -9.0F;
      exp_x_vel = 0.0F;
      exp_y_vel = -10.0F;
      exp_x_acc = 0.0F;
      exp_y_acc = -1.0F;
      exp_heading = F360_DEG2RAD(-90.0F);
      exp_cos_head = F360_Cosf(exp_heading);
      exp_sin_head = F360_Sinf(exp_heading);
      exp_pointing = F360_DEG2RAD(-90.0F);
      exp_cos_pointing = F360_Cosf(exp_pointing);
      exp_sin_pointing = F360_Sinf(exp_pointing);
      exp_prev_vcs_center_pos.x = 9.0F;
      exp_prev_vcs_center_pos.y = -9.0F;
      exp_filtered_pos_heading = F360_DEG2RAD(-90.0F);
   }
};

/**
*\purpose  Verify that the objects properties are translated and rotated correctly for a CTCA object
*\req    NA
*/
TEST(f360_test_group_convert_object_states, Test_Convert_Object_States_CTCA)
{

   /** \action
   * Call function with data setup in test group
   */
   Convert_Object_Properties(
      host_p,
      obj);

   /** \result
   * Compare against expected data defined in the test group
   */
   DOUBLES_EQUAL_TEXT(exp_x_pos, obj.vcs_position.x, thres, "X-position does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_y_pos, obj.vcs_position.y, thres, "Y-position does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_x_vel, obj.vcs_velocity.longitudinal, thres, "X-velocity does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_y_vel, obj.vcs_velocity.lateral, thres, "Y-velocity does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_x_acc, obj.vcs_accel.longitudinal, thres, "X-acceleration does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_y_acc, obj.vcs_accel.lateral, thres, "Y-acceleration does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_heading, obj.vcs_heading.Value(), thres, "Heading does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_cos_head, obj.vcs_heading.Cos(), thres, "Cosine of heading does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_sin_head, obj.vcs_heading.Sin(), thres, "Sine of heading does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_pointing, obj.bbox.Get_Orientation().Value(), thres, "Pointing does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_cos_pointing, obj.bbox.Get_Orientation().Cos(), thres, "Cosine of pointing does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_sin_pointing, obj.bbox.Get_Orientation().Sin(), thres, "Sine of pointing does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_prev_vcs_center_pos.x, obj.prev_vcs_center_pos.x, thres, "Previous VCS position does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_prev_vcs_center_pos.y, obj.prev_vcs_center_pos.y, thres, "Previous VCS position does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_filtered_pos_heading, obj.filtered_pos_diff_heading, thres, "Filtered position diff heading does not match expected data");
}

/**
*\purpose  Verify that the objects properties are translated and rotated correctly for a NoneMovealbe CCA object
*\req    NA
*/
TEST(f360_test_group_convert_object_states, Test_Convert_Object_States_NoneMovealbe_CCA)
{

   /** \action
   * Call function with data setup in test group
   */

  obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;

   Convert_Object_Properties(
      host_p,
      obj);

   /** \result
   * Compare against expected data defined in the test group
   */
   DOUBLES_EQUAL_TEXT(exp_x_pos, obj.vcs_position.x, thres, "X-position does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_y_pos, obj.vcs_position.y, thres, "Y-position does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_x_vel, obj.vcs_velocity.longitudinal, thres, "X-velocity does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_y_vel, obj.vcs_velocity.lateral, thres, "Y-velocity does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_x_acc, obj.vcs_accel.longitudinal, thres, "X-acceleration does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_y_acc, obj.vcs_accel.lateral, thres, "Y-acceleration does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_heading, obj.vcs_heading.Value(), thres, "Heading does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_cos_head, obj.vcs_heading.Cos(), thres, "Cosine of heading does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_sin_head, obj.vcs_heading.Sin(), thres, "Sine of heading does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_pointing, obj.bbox.Get_Orientation().Value(), thres, "Pointing does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_cos_pointing, obj.bbox.Get_Orientation().Cos(), thres, "Cosine of pointing does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_sin_pointing, obj.bbox.Get_Orientation().Sin(), thres, "Sine of pointing does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_prev_vcs_center_pos.x, obj.prev_vcs_center_pos.x, thres, "Previous VCS position does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_prev_vcs_center_pos.y, obj.prev_vcs_center_pos.y, thres, "Previous VCS position does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_filtered_pos_heading, obj.filtered_pos_diff_heading, thres, "Filtered position diff heading does not match expected data");
}

/**
*\purpose  Verify that the objects properties are translated and rotated correctly for a CCA Moveable object
*\req    NA
*/
TEST(f360_test_group_convert_object_states, Test_Convert_Object_States_Movealbe_CCA)
{
   /** \precond
   * Set object to CCA type and movable probability equals 1.0
   **/
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   obj.movable_prob = 1.0F;

   /** \action
   * Call function with data setup in test group
   */



   Convert_Object_Properties(
      host_p,
      obj);

   /** \result
   * Compare against expected data defined in the test group
   */
   DOUBLES_EQUAL_TEXT(exp_x_pos, obj.vcs_position.x, thres, "X-position does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_y_pos, obj.vcs_position.y, thres, "Y-position does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_x_vel, obj.vcs_velocity.longitudinal, thres, "X-velocity does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_y_vel, obj.vcs_velocity.lateral, thres, "Y-velocity does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_x_acc, obj.vcs_accel.longitudinal, thres, "X-acceleration does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_y_acc, obj.vcs_accel.lateral, thres, "Y-acceleration does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_heading, obj.vcs_heading.Value(), thres, "Heading does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_cos_head, obj.vcs_heading.Cos(), thres, "Cosine of heading does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_sin_head, obj.vcs_heading.Sin(), thres, "Sine of heading does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_pointing, obj.bbox.Get_Orientation().Value(), thres, "Pointing does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_cos_pointing, obj.bbox.Get_Orientation().Cos(), thres, "Cosine of pointing does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_sin_pointing, obj.bbox.Get_Orientation().Sin(), thres, "Sine of pointing does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_prev_vcs_center_pos.x, obj.prev_vcs_center_pos.x, thres, "Previous VCS position does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_prev_vcs_center_pos.y, obj.prev_vcs_center_pos.y, thres, "Previous VCS position does not match expected data");
   DOUBLES_EQUAL_TEXT(exp_filtered_pos_heading, obj.filtered_pos_diff_heading, thres, "Filtered position diff heading does not match expected data");
}
/** @}*/

/** \defgroup  f360_test_group_convert_object_covariance
*  @{
*/
/** \brief
*  Test group containing data needed for function and expected data when rotating covriance matrix
*/
TEST_GROUP(f360_test_group_convert_object_covariance)
{
   F360_Object_Track_T obj;

   F360_Host_Props_T host_p;

   // Threshold for passing test
   float32_t thres = 0.001F;

   /** \setup
   * See Matlab scipt referenced in DFD-285
   */
   TEST_SETUP()
   {
      // Setup an arbitrary covariance matrix for the object
      obj.errcov[0][0] = 6.0F;
      obj.errcov[0][1] = 3.0F;
      obj.errcov[0][2] = 1.0F;
      obj.errcov[0][3] = 1.0F;
      obj.errcov[0][4] = -3.0F;
      obj.errcov[0][5] = -2.0F;

      obj.errcov[1][0] = obj.errcov[0][1];
      obj.errcov[1][1] = 17.0F;
      obj.errcov[1][2] = 0.0F;
      obj.errcov[1][3] = -3.0F;
      obj.errcov[1][4] = 0.0F;
      obj.errcov[1][5] = 5.0F;

      obj.errcov[2][0] = obj.errcov[0][2];
      obj.errcov[2][1] = obj.errcov[1][2];
      obj.errcov[2][2] = 10.0F;
      obj.errcov[2][3] = 11.0F;
      obj.errcov[2][4] = -4.0F;
      obj.errcov[2][5] = 0.0F;

      obj.errcov[3][0] = obj.errcov[0][3];
      obj.errcov[3][1] = obj.errcov[1][3];
      obj.errcov[3][2] = obj.errcov[2][3];
      obj.errcov[3][3] = 14.0F;
      obj.errcov[3][4] = -3.0F;
      obj.errcov[3][5] = 1.0F;

      obj.errcov[4][0] = obj.errcov[0][4];
      obj.errcov[4][1] = obj.errcov[1][4];
      obj.errcov[4][2] = obj.errcov[2][4];
      obj.errcov[4][3] = obj.errcov[3][4];;
      obj.errcov[4][4] = 10.0F;
      obj.errcov[4][5] = 2.0F;

      obj.errcov[5][0] = obj.errcov[0][5];
      obj.errcov[5][1] = obj.errcov[1][5];
      obj.errcov[5][2] = obj.errcov[2][5];
      obj.errcov[5][3] = obj.errcov[3][5];
      obj.errcov[5][4] = obj.errcov[4][5];
      obj.errcov[5][5] = 11.0F;


      // Delta of VCS origin between two tracker iterations
      host_p.delta_pointing = F360_DEG2RAD(90.0F);
      host_p.cos_delta_pointing = F360_Cosf(host_p.delta_pointing);
      host_p.sin_delta_pointing = F360_Sinf(host_p.delta_pointing);
   }

};

/**
*\purpose  Verify that the objects covariance matrix is rotated correctly when object is CTCA track
*\req    NA
*/
TEST(f360_test_group_convert_object_covariance, Test_Convert_Object_Covariance_CTCA)
{
   /** \precond
   * Set object to CTCA type and generate expected data
   **/
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

   float32_t exp_P[STATE_DIMENSION][STATE_DIMENSION] = {};
   // First copy P matrix as many elements are expected to be equal
   for (uint8_t i = 0; i < STATE_DIMENSION; i++)
   {
      for (uint8_t j = 0; j < STATE_DIMENSION; j++)
      {
         exp_P[i][j] = obj.errcov[i][j];
      }
   }

   // Now change elements that are expected to change after rotation, see Matlab scipt referenced in DFD-285
   exp_P[0][0] = 17.0F;
   exp_P[0][1] = -3.0F;
   exp_P[0][2] = 0.0F;
   exp_P[0][3] = -3.0F;
   exp_P[0][4] = 0.0F;
   exp_P[0][5] = 5.0F;
   exp_P[1][0] = -3.0F;
   exp_P[1][1] = 6.0F;
   exp_P[1][2] = -1.0F;
   exp_P[1][3] = -1.0F;
   exp_P[1][4] = 3.0F;
   exp_P[1][5] = 2.0F;
   exp_P[2][0] = 0.0F;
   exp_P[3][0] = -3.0F;
   exp_P[4][0] = 0.0F;
   exp_P[5][0] = 5.0F;
   exp_P[2][1] = -1.0F;
   exp_P[3][1] = -1.0F;
   exp_P[4][1] = 3.0F;
   exp_P[5][1] = 2.0F;

   /** \action
   * Call function with data setup in test group
   */
   Convert_Object_Covariance(
      host_p,
      obj);

   /** \result
   * Derive expected data and compare result
   */
   for (uint8_t i = 0; i < STATE_DIMENSION; i++)
   {
      for (uint8_t j = 0; j < STATE_DIMENSION; j++)
      {
         DOUBLES_EQUAL(exp_P[i][j], obj.errcov[i][j], thres)
      }
   }
}

/**
*\purpose  Verify that the objects covariance matrix is rotated correctly when object is CCA track
*\req    NA
*/
TEST(f360_test_group_convert_object_covariance, Test_Convert_Object_Covariance_CCA)
{
   /** \precond
   * Set object to CCA type and generate expected data
   **/
   obj.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;

   float32_t exp_P[STATE_DIMENSION][STATE_DIMENSION] = {};
   exp_P[0][0] = 14.0F;
   exp_P[0][1] = -3.0F;
   exp_P[0][2] = 1.0F;
   exp_P[0][3] = -1.0F;
   exp_P[0][4] = 3.0F;
   exp_P[0][5] = -11.0F;
   exp_P[1][0] = -3.0F;
   exp_P[1][1] = 10.0F;
   exp_P[1][2] = 2.0F;
   exp_P[1][3] = 3.0F;
   exp_P[1][4] = 0.0F;
   exp_P[1][5] = 4.0F;
   exp_P[2][0] = 1.0F;
   exp_P[2][1] = 2.0F;
   exp_P[2][2] = 11.0F;
   exp_P[2][3] = 2.0F;
   exp_P[2][4] = -5.0F;
   exp_P[2][5] = 0.0F;
   exp_P[3][0] = -1.0F;
   exp_P[3][1] = 3.0F;
   exp_P[3][2] = 2.0F;
   exp_P[3][3] = 6.0F;
   exp_P[3][4] = 3.0F;
   exp_P[3][5] = 1.0F;
   exp_P[4][0] = 3.0F;
   exp_P[4][1] = 0.0F;
   exp_P[4][2] = -5.0F;
   exp_P[4][3] = 3.0F;
   exp_P[4][4] = 17.0F;
   exp_P[4][5] = 0.0F;
   exp_P[5][0] = -11.0F;
   exp_P[5][1] = 4.0F;
   exp_P[5][2] = 0.0F;
   exp_P[5][3] = 1.0F;
   exp_P[5][4] = 0.0F;
   exp_P[5][5] = 10.0F;


   /** \action
   * Call function with data setup in test group
   */
   Convert_Object_Covariance(
      host_p,
      obj);

   /** \result
   * Derive expected data and compare result
   */
   for (uint8_t i = 0; i < STATE_DIMENSION; i++)
   {
      for (uint8_t j = 0; j < STATE_DIMENSION; j++)
      {
         DOUBLES_EQUAL(exp_P[i][j], obj.errcov[i][j], thres)
      }
   }
}
/** @}*/

/** \defgroup  f360_test_group_Transform_Point_From_Prev_To_Current_Vcs
*  @{
*/
/** \brief
*  Test group containing data needed for function and expected data when 
*  transforming a vcs point from previous to new vcs position due to
*  host motion between tracker iterations
*/
TEST_GROUP(f360_test_group_Transform_Point_From_Prev_To_Current_Vcs)
{
   Point prev_vcs_point = {};

   F360_Host_Props_T host_p = {};

   // Threshold for passing test
   float32_t thres = 0.001F;

   /** \setup
   * Set up host data with an arbitrary motion between iterations
   */
   TEST_SETUP()
   {
      // Delta of VCS origin between two tracker iterations
      host_p.delta_position_x = 1.0F;
      host_p.delta_position_y = 1.0F;
      host_p.delta_pointing = F360_DEG2RAD(90.0F);
      host_p.cos_delta_pointing = F360_Cosf(host_p.delta_pointing);
      host_p.sin_delta_pointing = F360_Sinf(host_p.delta_pointing);

      // Set point to something arbitrary
      prev_vcs_point.x = 10.0F;
      prev_vcs_point.y = 10.0F;
   }

};

/**
*\purpose  Verify that the objects previous position have been
*          transformed correctly when host have both translation
*          and rotation part that is positive.
*\req    NA
*/
TEST(f360_test_group_Transform_Point_From_Prev_To_Current_Vcs, Transform_Point_From_Prev_To_Current_Vcs__Host_Positive_Translated_And_Rotated)
{
   /** \precond
   * The following have been set up in test group
   *  host_p.delta_position_x = 1.0F;
   *  host_p.delta_position_y = 1.0F;
   *  host_p.delta_pointing = F360_DEG2RAD(90.0F);
   *  host_p.cos_delta_pointing = F360_Cosf(host_p.delta_pointing);
   *  host_p.sin_delta_pointing = F360_Sinf(host_p.delta_pointing);
   *  prev_vcs_point.longitudinal = 10.0F;
   *  prev_vcs_point.lateral = 10.0F;
   **/

   /** \action
   *  Call function Transform_Point_From_Prev_To_Current_Vcs
   */
   Point current_vcs_point = Transform_Point_From_Prev_To_Current_Vcs(
      prev_vcs_point,
      host_p);

   /** \result
   * Compare against expected data
   */
   DOUBLES_EQUAL(9.0F, current_vcs_point.x, thres);
   DOUBLES_EQUAL(-9.0F, current_vcs_point.y, thres);
}

/**
*\purpose  Verify that the previous vcs point have been
*          transformed correctly when host have both translation
*          and rotation part that is negative.
*\req    NA
*/
TEST(f360_test_group_Transform_Point_From_Prev_To_Current_Vcs, Transform_Point_From_Prev_To_Current_Vcs__Host_Negative_Translated_And_Rotated)
{
   /** \precond
   * The following have been set up in test group
   *  host_p.delta_position_x = 1.0F;
   *  host_p.delta_position_y = 1.0F;
   *  host_p.delta_pointing = F360_DEG2RAD(90.0F);
   *  host_p.cos_delta_pointing = F360_Cosf(host_p.delta_pointing);
   *  host_p.sin_delta_pointing = F360_Sinf(host_p.delta_pointing);
   *  prev_vcs_point.longitudinal = 10.0F;
   *  prev_vcs_point.lateral = 10.0F;
   * 
   * Modify host movement to negative direction
   **/
   host_p.delta_position_x = -1.0F;
   host_p.delta_position_y = -1.0F;
   host_p.delta_pointing = F360_DEG2RAD(-90.0F);
   host_p.cos_delta_pointing = F360_Cosf(host_p.delta_pointing);
   host_p.sin_delta_pointing = F360_Sinf(host_p.delta_pointing);

   /** \action
   * Call function Transform_Point_From_Prev_To_Current_Vcs
   */
   Point current_vcs_point = Transform_Point_From_Prev_To_Current_Vcs(
      prev_vcs_point,
      host_p);

   /** \result
   * Compare against expected data
   */
   DOUBLES_EQUAL(-11.0F, current_vcs_point.x, thres);
   DOUBLES_EQUAL(11.0F, current_vcs_point.y, thres);
}

/**
*\purpose  Verify that the previous vcs point have been
*          transformed correctly when host have positive translation
*          part and negative rotation part.
*\req    NA
*/
TEST(f360_test_group_Transform_Point_From_Prev_To_Current_Vcs, Transform_Point_From_Prev_To_Current_Vcs__Host_Positive_Translated_And_Negative_Rotated)
{
   /** \precond
   * The following have been set up in test group
   *  host_p.delta_position_x = 1.0F;
   *  host_p.delta_position_y = 1.0F;
   *  host_p.delta_pointing = F360_DEG2RAD(90.0F);
   *  host_p.cos_delta_pointing = F360_Cosf(host_p.delta_pointing);
   *  host_p.sin_delta_pointing = F360_Sinf(host_p.delta_pointing);
   *  prev_vcs_point.longitudinal = 10.0F;
   *  prev_vcs_point.lateral = 10.0F;
   *
   * Modify host rotation to negative direction
   **/
   host_p.delta_pointing = F360_DEG2RAD(-90.0F);
   host_p.cos_delta_pointing = F360_Cosf(host_p.delta_pointing);
   host_p.sin_delta_pointing = F360_Sinf(host_p.delta_pointing);

   /** \action
   * Call function Transform_Point_From_Prev_To_Current_Vcs
   */
   Point current_vcs_point = Transform_Point_From_Prev_To_Current_Vcs(
      prev_vcs_point,
      host_p);

   /** \result
   * Compare against expected data
   */
   DOUBLES_EQUAL(-9.0F, current_vcs_point.x, thres);
   DOUBLES_EQUAL(9.0F, current_vcs_point.y, thres);
}

/**
*\purpose  Verify that the previous vcs point have been
*          transformed correctly when host have both translation
*          and rotation part that is positive. Previous vcs point 
*          is in negative quadrant.
*\req    NA
*/
TEST(f360_test_group_Transform_Point_From_Prev_To_Current_Vcs, Transform_Point_From_Prev_To_Current_Vcs__Host_Positive_Translated_And_Rotated_Object_In_Negative_Quadrant)
{
   /** \precond
   * The following have been set up in test group
   *  host_p.delta_position_x = 1.0F;
   *  host_p.delta_position_y = 1.0F;
   *  host_p.delta_pointing = F360_DEG2RAD(90.0F);
   *  host_p.cos_delta_pointing = F360_Cosf(host_p.delta_pointing);
   *  host_p.sin_delta_pointing = F360_Sinf(host_p.delta_pointing);
   *  prev_vcs_point.longitudinal = 10.0F;
   *  prev_vcs_point.lateral = 10.0F;
   * 
   * Set previous vcs point in negative quadrant
   **/
   prev_vcs_point.x = -10.0F;
   prev_vcs_point.y = -10.0F;

   /** \action
   *  Call function Transform_Point_From_Prev_To_Current_Vcs
   */
   Point current_vcs_point = Transform_Point_From_Prev_To_Current_Vcs(
      prev_vcs_point,
      host_p);

   /** \result
   * Compare against expected data
   */
   DOUBLES_EQUAL(-11.0F, current_vcs_point.x, thres);
   DOUBLES_EQUAL(11.0F, current_vcs_point.y, thres);
}

/** @}*/
