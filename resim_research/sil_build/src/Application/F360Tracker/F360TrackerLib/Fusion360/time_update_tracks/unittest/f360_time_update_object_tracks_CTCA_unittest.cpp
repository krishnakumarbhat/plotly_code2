/** \file
 * This file contains unit tests for content of f360_time_update_object_tracks_CTCA.cpp file
 */

#include "f360_time_update_object_tracks_CTCA.h"
#include "f360_math.h"
#include "f360_iterator.h"
#include <algorithm>

#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_time_update_object_tracks_CTCA
 *  @{
 */

/** \brief
 * Define needed input parameters to function Time_Update_Object_Track_CTCA.
 * These parameters are tweaked in the individual test cases.
 */
TEST_GROUP(f360_time_update_object_tracks_CTCA)
{
   // Declare common variables used within all tests in this test group.
   float32_t elapsed_time = 0.05F;
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS];
   F360_Tracker_Info_T tracker_info;
   F360_Calibrations_T calibs;
   F360_TRKR_TIMING_INFO_T timing_info;

   int32_t obj_id;
   uint32_t obj_idx;

   float32_t test_pass_threshold = 1.0e-4F;

   /** \setup
    * Setup default parameters
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      obj_id = 10;
      obj_idx = obj_id - 1U;

      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = obj_id;

      object_tracks[obj_idx].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      object_tracks[obj_idx].vcs_heading = Angle{ F360_DEG2RAD(30.0F) };
      object_tracks[obj_idx].hdg_ptng_disagmt = 0.0F;
      object_tracks[obj_idx].speed = 10.0F;
      object_tracks[obj_idx].curvature = 0.05F;
      object_tracks[obj_idx].tang_accel = 0.7F;
      object_tracks[obj_idx].vcs_position.x = 10.0F;
      object_tracks[obj_idx].vcs_position.y = 1.0F;
      object_tracks[obj_idx].reference_point = F360_REFERENCE_POINT_REAR;
      object_tracks[obj_idx].bbox.Set_Orientation(object_tracks[obj_idx].vcs_heading);
      object_tracks[obj_idx].bbox.Set_Length(10.0F);
      object_tracks[obj_idx].bbox.Set_Width(3.0F);
      object_tracks[obj_idx].Update_Bbox_Center();
      object_tracks[obj_idx].f_moving = true;
      object_tracks[obj_idx].movable_prob = 1.0F;

      float32_t initial_state_error_cov_ctca_mat[STATE_DIMENSION][STATE_DIMENSION] = {
         {10.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F},
         {0.0F,10.0F, 0.0F, 0.0F, 0.0F, 0.0F},
         {0.0F, 0.0F, 0.01F, 0.0F, 0.0F, 0.0F},
         {0.0F,0.0F, 0.0F, 0.005F, 0.0F, 0.0F},
         {0.0F, 0.0F, 0.0F, 0.0F, 3.0F, 0.0F},
         {0.0F,0.0F, 0.0F, 0.0F, 0.0F, 3.0F} };

      std::copy(cmn::begin(initial_state_error_cov_ctca_mat), cmn::end(initial_state_error_cov_ctca_mat), cmn::begin(object_tracks[obj_idx].errcov));

   }
   
   void Check_Covariance_Matrix_Equal(
      float32_t(&exp_mat)[STATE_DIMENSION][STATE_DIMENSION],
      float32_t(&test_mat)[STATE_DIMENSION][STATE_DIMENSION])
   {
      for (uint8_t row = 0U; row < STATE_DIMENSION; row++)
      {
         for (uint8_t col = 0U; col < STATE_DIMENSION; col++)
         {
            DOUBLES_EQUAL(exp_mat[row][col], test_mat[row][col], test_pass_threshold);
         }
      }
   }
};

/** \purpose
 * Verify that object states are unchanged after call to function when the number of active objects is zero in
 * tracker_info structure
 * \req
 * NA
 */
TEST(f360_time_update_object_tracks_CTCA, Time_Update_Object_Track_CTCA__No_Active_Objects)
{
   /** \precond
    * Set number of active objects to 0
    * Copy object info before call to function
    */
   tracker_info.num_active_objs = 0;
   F360_Object_Track_T object_copy = object_tracks[obj_idx];

   /** \action
    * Call Time_Update_Object_Track_CTCA()
    */
   Time_Update_Object_Track_CTCA(elapsed_time, object_tracks, tracker_info, calibs, timing_info);

   /** \result
    * Verify object states haven't changed
    */
   DOUBLES_EQUAL(object_copy.vcs_heading.Value(), object_tracks[obj_idx].vcs_heading.Value(), test_pass_threshold);
   DOUBLES_EQUAL(object_copy.vcs_heading.Cos(), object_tracks[obj_idx].vcs_heading.Cos(), test_pass_threshold);
   DOUBLES_EQUAL(object_copy.vcs_heading.Sin(), object_tracks[obj_idx].vcs_heading.Sin(), test_pass_threshold);
   DOUBLES_EQUAL(object_copy.bbox.Get_Orientation().Value(), object_tracks[obj_idx].bbox.Get_Orientation().Value(), test_pass_threshold);
   DOUBLES_EQUAL(object_copy.bbox.Get_Orientation().Cos(), object_tracks[obj_idx].bbox.Get_Orientation().Cos(), test_pass_threshold);
   DOUBLES_EQUAL(object_copy.bbox.Get_Orientation().Sin(), object_tracks[obj_idx].bbox.Get_Orientation().Sin(), test_pass_threshold);
   DOUBLES_EQUAL(object_copy.speed, object_tracks[obj_idx].speed, test_pass_threshold);
   DOUBLES_EQUAL(object_copy.curvature, object_tracks[obj_idx].curvature, test_pass_threshold);
   DOUBLES_EQUAL(object_copy.tang_accel, object_tracks[obj_idx].tang_accel, test_pass_threshold);
   DOUBLES_EQUAL(object_copy.vcs_position.x, object_tracks[obj_idx].vcs_position.x, test_pass_threshold);
   DOUBLES_EQUAL(object_copy.vcs_position.y, object_tracks[obj_idx].vcs_position.y, test_pass_threshold);
   Check_Covariance_Matrix_Equal(object_copy.errcov, object_tracks[obj_idx].errcov);
}

/** \purpose
 * Verify that object states are unchanged after call to function when the object filter type is CCA type
 * \req
 * NA
 */
TEST(f360_time_update_object_tracks_CTCA, Time_Update_Object_Track_CTCA__Object_Is_CCA_Type)
{
   /** \precond
    * Set object to CCA type
    * Copy object info before call to function
    */
   object_tracks[obj_idx].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   F360_Object_Track_T object_copy = object_tracks[obj_idx];

   /** \action
    * Call Time_Update_Object_Track_CTCA()
    */
   Time_Update_Object_Track_CTCA(elapsed_time, object_tracks, tracker_info, calibs, timing_info);

   /** \result
    * Verify object states haven't changed
    */
   DOUBLES_EQUAL(object_copy.vcs_heading.Value(), object_tracks[obj_idx].vcs_heading.Value(), test_pass_threshold);
   DOUBLES_EQUAL(object_copy.vcs_heading.Cos(), object_tracks[obj_idx].vcs_heading.Cos(), test_pass_threshold);
   DOUBLES_EQUAL(object_copy.vcs_heading.Sin(), object_tracks[obj_idx].vcs_heading.Sin(), test_pass_threshold);
   DOUBLES_EQUAL(object_copy.bbox.Get_Orientation().Value(), object_tracks[obj_idx].bbox.Get_Orientation().Value(), test_pass_threshold);
   DOUBLES_EQUAL(object_copy.bbox.Get_Orientation().Cos(), object_tracks[obj_idx].bbox.Get_Orientation().Cos(), test_pass_threshold);
   DOUBLES_EQUAL(object_copy.bbox.Get_Orientation().Sin(), object_tracks[obj_idx].bbox.Get_Orientation().Sin(), test_pass_threshold);
   DOUBLES_EQUAL(object_copy.speed, object_tracks[obj_idx].speed, test_pass_threshold);
   DOUBLES_EQUAL(object_copy.curvature, object_tracks[obj_idx].curvature, test_pass_threshold);
   DOUBLES_EQUAL(object_copy.tang_accel, object_tracks[obj_idx].tang_accel, test_pass_threshold);
   DOUBLES_EQUAL(object_copy.vcs_position.x, object_tracks[obj_idx].vcs_position.x, test_pass_threshold);
   DOUBLES_EQUAL(object_copy.vcs_position.y, object_tracks[obj_idx].vcs_position.y, test_pass_threshold);
   Check_Covariance_Matrix_Equal(object_copy.errcov, object_tracks[obj_idx].errcov);
}

/** \purpose
 * Verify that object states have been updated as expected when:
 *    - the object is CTCA type
 *    - is flagged as moving
 *    - the object reference point is in rear center
 * \req
 * NA
 */
TEST(f360_time_update_object_tracks_CTCA, Time_Update_Object_Track_CTCA__Check_States_When_Obj_Is_CTCA_Moving_And_Ref_Pnt_REAR)
{
   /** \precond
    * Use default settings from TEST_GROUP
    */

   /** \action
    * Call Time_Update_Object_Track_CTCA()
    */
   Time_Update_Object_Track_CTCA(elapsed_time, object_tracks, tracker_info, calibs, timing_info);

   /** \result
    * Verify object states have changed as expected (all states updated)
    */
   float32_t exp_heading = 0.548642525598299F;
   float32_t exp_pointing = exp_heading;
   float32_t exp_speed = 10.035F;
   float32_t exp_curv = 0.05F;
   float32_t exp_tang_accel = 0.7F;
   float32_t exp_long_pos = 10.430634527050218F;
   float32_t exp_lat_pos = 1.255869119655628F;

   DOUBLES_EQUAL(exp_heading, object_tracks[obj_idx].vcs_heading.Value(), test_pass_threshold);
   DOUBLES_EQUAL(F360_Cosf(exp_heading), object_tracks[obj_idx].vcs_heading.Cos(), test_pass_threshold);
   DOUBLES_EQUAL(F360_Sinf(exp_heading), object_tracks[obj_idx].vcs_heading.Sin(), test_pass_threshold);
   DOUBLES_EQUAL(exp_pointing, object_tracks[obj_idx].bbox.Get_Orientation().Value(), test_pass_threshold);
   DOUBLES_EQUAL(F360_Cosf(exp_pointing), object_tracks[obj_idx].bbox.Get_Orientation().Cos(), test_pass_threshold);
   DOUBLES_EQUAL(F360_Sinf(exp_pointing), object_tracks[obj_idx].bbox.Get_Orientation().Sin(), test_pass_threshold);
   DOUBLES_EQUAL(exp_speed, object_tracks[obj_idx].speed, test_pass_threshold);
   DOUBLES_EQUAL(exp_curv, object_tracks[obj_idx].curvature, test_pass_threshold);
   DOUBLES_EQUAL(exp_tang_accel, object_tracks[obj_idx].tang_accel, test_pass_threshold);
   DOUBLES_EQUAL(exp_long_pos, object_tracks[obj_idx].vcs_position.x, test_pass_threshold);
   DOUBLES_EQUAL(exp_lat_pos, object_tracks[obj_idx].vcs_position.y, test_pass_threshold);
}

/** \purpose
 * Verify that object states have been updated as expected when:
 *    - the object is CTCA type
 *    - is flagged as non-moving
 *    - the object reference point is in rear center
 * \req
 * NA
 */
TEST(f360_time_update_object_tracks_CTCA, Time_Update_Object_Track_CTCA__Check_States_When_Obj_Is_CTCA_Stationary_And_Ref_Pnt_REAR)
{
   /** \precond
    * Use default settings from TEST_GROUP except for
    *    - change object f_moving to false
    */
   object_tracks[obj_idx].f_moving = false;

   /** \action
    * Call Time_Update_Object_Track_CTCA()
    */
   Time_Update_Object_Track_CTCA(elapsed_time, object_tracks, tracker_info, calibs, timing_info);

   /** \result
    * Verify object states have changed as expected (all states updated except for object heading and pointing which are unchanged)
    */
   float32_t exp_heading = 0.523598775598299F;
   float32_t exp_pointing = exp_heading;
   float32_t exp_speed = 10.035F;
   float32_t exp_curv = 0.05F;
   float32_t exp_tang_accel = 0.7F;
   float32_t exp_long_pos = 10.430634527050218F;
   float32_t exp_lat_pos = 1.255869119655628F;

   DOUBLES_EQUAL(exp_heading, object_tracks[obj_idx].vcs_heading.Value(), test_pass_threshold);
   DOUBLES_EQUAL(F360_Cosf(exp_heading), object_tracks[obj_idx].vcs_heading.Cos(), test_pass_threshold);
   DOUBLES_EQUAL(F360_Sinf(exp_heading), object_tracks[obj_idx].vcs_heading.Sin(), test_pass_threshold);
   DOUBLES_EQUAL(exp_pointing, object_tracks[obj_idx].bbox.Get_Orientation().Value(), test_pass_threshold);
   DOUBLES_EQUAL(F360_Cosf(exp_pointing), object_tracks[obj_idx].bbox.Get_Orientation().Cos(), test_pass_threshold);
   DOUBLES_EQUAL(F360_Sinf(exp_pointing), object_tracks[obj_idx].bbox.Get_Orientation().Sin(), test_pass_threshold);
   DOUBLES_EQUAL(exp_speed, object_tracks[obj_idx].speed, test_pass_threshold);
   DOUBLES_EQUAL(exp_curv, object_tracks[obj_idx].curvature, test_pass_threshold);
   DOUBLES_EQUAL(exp_tang_accel, object_tracks[obj_idx].tang_accel, test_pass_threshold);
   DOUBLES_EQUAL(exp_long_pos, object_tracks[obj_idx].vcs_position.x, test_pass_threshold);
   DOUBLES_EQUAL(exp_lat_pos, object_tracks[obj_idx].vcs_position.y, test_pass_threshold);
}

/** \purpose
 * Verify that object states have been updated as expected when:
 *    - the object is CTCA type
 *    - is flagged as moving
 *    - the object reference point is not in rear center (for this test front right corner is choosen)
 * \req
 * NA
 */
TEST(f360_time_update_object_tracks_CTCA, Time_Update_Object_Track_CTCA__Check_States_When_Obj_Is_CTCA_Moving_And_Ref_Pnt_FRONT_RIGHT)
{
   /** \precond
    * Use default settings from TEST_GROUP except for:
    *    - change object reference point to FRONT_RIGHT
    */
   object_tracks[obj_idx].reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;
   object_tracks[obj_idx].Update_Bbox_Center();

   /** \action
    * Call Time_Update_Object_Track_CTCA()
    */
   Time_Update_Object_Track_CTCA(elapsed_time, object_tracks, tracker_info, calibs, timing_info);

   /** \result
    * Verify object states have changed as expected (all states updated)
    */
   float32_t exp_heading = 0.548642525598299F;
   float32_t exp_pointing = exp_heading;
   float32_t exp_speed = 10.035F;
   float32_t exp_curv = 0.05F;
   float32_t exp_tang_accel = 0.7F;
   float32_t exp_long_pos = 10.270402377693637F;
   float32_t exp_lat_pos = 1.451996199206565F;

   DOUBLES_EQUAL(exp_heading, object_tracks[obj_idx].vcs_heading.Value(), test_pass_threshold);
   DOUBLES_EQUAL(F360_Cosf(exp_heading), object_tracks[obj_idx].vcs_heading.Cos(), test_pass_threshold);
   DOUBLES_EQUAL(F360_Sinf(exp_heading), object_tracks[obj_idx].vcs_heading.Sin(), test_pass_threshold);
   DOUBLES_EQUAL(exp_pointing, object_tracks[obj_idx].bbox.Get_Orientation().Value(), test_pass_threshold);
   DOUBLES_EQUAL(F360_Cosf(exp_pointing), object_tracks[obj_idx].bbox.Get_Orientation().Cos(), test_pass_threshold);
   DOUBLES_EQUAL(F360_Sinf(exp_pointing), object_tracks[obj_idx].bbox.Get_Orientation().Sin(), test_pass_threshold);
   DOUBLES_EQUAL(exp_speed, object_tracks[obj_idx].speed, test_pass_threshold);
   DOUBLES_EQUAL(exp_curv, object_tracks[obj_idx].curvature, test_pass_threshold);
   DOUBLES_EQUAL(exp_tang_accel, object_tracks[obj_idx].tang_accel, test_pass_threshold);
   DOUBLES_EQUAL(exp_long_pos, object_tracks[obj_idx].vcs_position.x, test_pass_threshold);
   DOUBLES_EQUAL(exp_lat_pos, object_tracks[obj_idx].vcs_position.y, test_pass_threshold);
}

/** \purpose
 * Verify that object states have been updated as expected when:
 *    - the object is CTCA type
 *    - is flagged as non-moving
 *    - the object reference point is not in rear center (for this test front right corner is choosen)
 * \req
 * NA
 */
TEST(f360_time_update_object_tracks_CTCA, Time_Update_Object_Track_CTCA__Check_States_When_Obj_Is_CTCA_Stationary_And_Ref_Pnt_FRONT_RIGHT)
{
   /** \precond
    * Use default settings from TEST_GROUP except for
    *    - change object reference point to front right corner
    *    - change object f_moving to false
    */
   object_tracks[obj_idx].reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;
   object_tracks[obj_idx].Update_Bbox_Center();
   object_tracks[obj_idx].f_moving = false;

   /** \action
    * Call Time_Update_Object_Track_CTCA()
    */
   Time_Update_Object_Track_CTCA(elapsed_time, object_tracks, tracker_info, calibs, timing_info);

   /** \result
    * Verify object states have changed as expected (all states updated except for object heading and pointing which are unchanged)
    */
   float32_t exp_heading = 0.523598775598299F;
   float32_t exp_pointing = exp_heading;
   float32_t exp_speed = 10.035F;
   float32_t exp_curv = 0.05F;
   float32_t exp_tang_accel = 0.7F;
   float32_t exp_long_pos = 10.270402377693637F;
   float32_t exp_lat_pos = 1.451996199206565F;

   DOUBLES_EQUAL(exp_heading, object_tracks[obj_idx].vcs_heading.Value(), test_pass_threshold);
   DOUBLES_EQUAL(F360_Cosf(exp_heading), object_tracks[obj_idx].vcs_heading.Cos(), test_pass_threshold);
   DOUBLES_EQUAL(F360_Sinf(exp_heading), object_tracks[obj_idx].vcs_heading.Sin(), test_pass_threshold);
   DOUBLES_EQUAL(exp_pointing, object_tracks[obj_idx].bbox.Get_Orientation().Value(), test_pass_threshold);
   DOUBLES_EQUAL(F360_Cosf(exp_pointing), object_tracks[obj_idx].bbox.Get_Orientation().Cos(), test_pass_threshold);
   DOUBLES_EQUAL(F360_Sinf(exp_pointing), object_tracks[obj_idx].bbox.Get_Orientation().Sin(), test_pass_threshold);
   DOUBLES_EQUAL(exp_speed, object_tracks[obj_idx].speed, test_pass_threshold);
   DOUBLES_EQUAL(exp_curv, object_tracks[obj_idx].curvature, test_pass_threshold);
   DOUBLES_EQUAL(exp_tang_accel, object_tracks[obj_idx].tang_accel, test_pass_threshold);
   DOUBLES_EQUAL(exp_long_pos, object_tracks[obj_idx].vcs_position.x, test_pass_threshold);
   DOUBLES_EQUAL(exp_lat_pos, object_tracks[obj_idx].vcs_position.y, test_pass_threshold);
}

/** \purpose
 * Verify that object pointing is correctly set when heading pointing disagreement is non-zero:
 * \req
 * NA
 */
TEST(f360_time_update_object_tracks_CTCA, Time_Update_Object_Track_CTCA__check_pnt_when_hdh_pnt_disagreement_non_zero)
{
   /** \precond
    * Use default settings from TEST_GROUP except for change the heading pointing disagreement to non-zero
    */
   object_tracks[obj_idx].hdg_ptng_disagmt = F360_DEG2RAD(3.0F);

   /** \action
    * Call Time_Update_Object_Track_CTCA()
    */
   Time_Update_Object_Track_CTCA(elapsed_time, object_tracks, tracker_info, calibs, timing_info);

   /** \result
    * Verify object states have changed as expected (all states updated)
    */
   float32_t exp_heading = 0.548642525598299F;
   float32_t exp_pointing = exp_heading + F360_DEG2RAD(2.25F);

   DOUBLES_EQUAL(exp_pointing, object_tracks[obj_idx].bbox.Get_Orientation().Value(), 1e-6F);
}

/** \purpose
 * Verify that object heading pointing disagreement is decreased as expected if it is being largely positive.
 * \req
 * NA
 */
TEST(f360_time_update_object_tracks_CTCA, Time_Update_Object_Track_CTCA__check_decrease_of_largely_positive_hdg_pnt_disagreement)
{
   /** \precond
    * Use default settings from TEST_GROUP except for change the heading pointing disagreement to non-zero and above 0.75degrees
    */
   object_tracks[obj_idx].hdg_ptng_disagmt = F360_DEG2RAD(3.0F);

   /** \action
    * Call Time_Update_Object_Track_CTCA()
    */
   Time_Update_Object_Track_CTCA(elapsed_time, object_tracks, tracker_info, calibs, timing_info);

   /** \result
    * Verify heading pointing disagreement has been decreased
    */
   DOUBLES_EQUAL(F360_DEG2RAD(2.25F), object_tracks[obj_idx].hdg_ptng_disagmt, 1e-6F);
}

/** \purpose
 * Verify that object heading pointing disagreement is set to zero if it is being small and positive.
 * \req
 * NA
 */
TEST(f360_time_update_object_tracks_CTCA, Time_Update_Object_Track_CTCA__check_decrease_of_small_positive_hdg_pnt_disagreement)
{
   /** \precond
    * Use default settings from TEST_GROUP except for change the heading pointing disagreement to positive and non-zero and below 0.75degrees
    */
   object_tracks[obj_idx].hdg_ptng_disagmt = F360_DEG2RAD(0.7F);

   /** \action
    * Call Time_Update_Object_Track_CTCA()
    */
   Time_Update_Object_Track_CTCA(elapsed_time, object_tracks, tracker_info, calibs, timing_info);

   /** \result
    * Verify heading pointing disagreement has been set to zero
    */
   DOUBLES_EQUAL(0.0F, object_tracks[obj_idx].hdg_ptng_disagmt, 1e-6F);
}

/** \purpose
 * Verify that object heading pointing disagreement is increased as expected if it is being largely negative.
 * \req
 * NA
 */
TEST(f360_time_update_object_tracks_CTCA, Time_Update_Object_Track_CTCA__check_increase_of_largely_negative_hdg_pnt_disagreement)
{
   /** \precond
    * Use default settings from TEST_GROUP except for change the heading pointing disagreement to non-zero and below -0.75degrees
    */
   object_tracks[obj_idx].hdg_ptng_disagmt = F360_DEG2RAD(-3.0F);

   /** \action
    * Call Time_Update_Object_Track_CTCA()
    */
   Time_Update_Object_Track_CTCA(elapsed_time, object_tracks, tracker_info, calibs, timing_info);

   /** \result
    * Verify heading pointing disagreement has been increased
    */
   DOUBLES_EQUAL(F360_DEG2RAD(-2.25F), object_tracks[obj_idx].hdg_ptng_disagmt, 1e-6F);
}

/** \purpose
 * Verify that object heading pointing disagreement is set to zero if it is being small in magnitude and negative.
 * \req
 * NA
 */
TEST(f360_time_update_object_tracks_CTCA, Time_Update_Object_Track_CTCA__check_increase_of_small_negative_hdg_pnt_disagreement)
{
   /** \precond
    * Use default settings from TEST_GROUP except for change the heading pointing disagreement to negative, non-zero and above -0.75degrees
    */
   object_tracks[obj_idx].hdg_ptng_disagmt = F360_DEG2RAD(-0.7F);

   /** \action
    * Call Time_Update_Object_Track_CTCA()
    */
   Time_Update_Object_Track_CTCA(elapsed_time, object_tracks, tracker_info, calibs, timing_info);

   /** \result
    * Verify heading pointing disagreement has been set to zero
    */
   DOUBLES_EQUAL(0.0F, object_tracks[obj_idx].hdg_ptng_disagmt, 1e-6F);
}

/** @}*/


/** \defgroup  Adaptive_Tuning_Of_Proc_Noise_Cov
 *  @{
 */

/** \brief
 * Define needed input parameters to function Adaptive_Tuning_Of_Proc_Noise_Cov.
 * These parameters are tweaked in the individual test cases.
 */
TEST_GROUP(Adaptive_Tuning_Of_Proc_Noise_Cov)
{
   // Declare common variables used within all tests in this test group.
   float32_t elapsed_time = 0.05F;
   F360_Object_Track_T obj;
   F360_Calibrations_T calibs;

   float32_t test_pass_threshold = F360_EPSILON;

   /** \setup
    * Setup default parameters such as
    *    - object tangential acceleration is small such that we use the slowest possible tuning in para direction
    *    - object speed is small such that we use the part of the tuning in orth direction where we have assumed curvature is limited by minimum turn radius
    *    - object vcs_heading is 0 such that TCS and VCS coordinates are aligned (i.e. such that rotation from TCS to VCS does not change  Q matrix)
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      obj.tang_accel = calibs.k_min_acc_for_increasing_acc_noise - 0.1F;
      obj.speed = 3.5F; // half way between 0 and the threshold for when we assume curvature is limited by minimum turning radius
      obj.vcs_heading.Value(0.0F);
   }
   
   void Check_Covariance_Matrix_Equal(
      float32_t(&exp_mat)[STATE_DIMENSION][STATE_DIMENSION],
      float32_t(&test_mat)[STATE_DIMENSION][STATE_DIMENSION])
   {
      for (uint8_t row = 0U; row < STATE_DIMENSION; row++)
      {
         for (uint8_t col = 0U; col < STATE_DIMENSION; col++)
         {
            DOUBLES_EQUAL(exp_mat[row][col], test_mat[row][col], test_pass_threshold);
         }
      }
   }
};

/** \purpose
 * Verify that the slowest tuning in para direction works as intended
 * tracker_info structure
 * \req
 * NA
 */
TEST(Adaptive_Tuning_Of_Proc_Noise_Cov, Test_Slow_Para_Tuning)
{
   /** \precond
    * Use default settings from test group
    */

   /** \action
    * Call Time_Update_Object_Track_CTCA()
    */
   float32_t q[STATE_DIMENSION][STATE_DIMENSION] = {};
   Adaptive_Tuning_Of_Proc_Noise_Cov(elapsed_time, obj, calibs, q);

   /** \result
    * Verify that outputted Q matrix is as expected in para direction
    */
   DOUBLES_EQUAL(0.125F, q[5][5], test_pass_threshold);
   DOUBLES_EQUAL(1.040625e-04F, q[4][4], test_pass_threshold);
   DOUBLES_EQUAL(0.0100000390625F, q[0][0], test_pass_threshold);
}

/** \purpose
 * Verify that the fastest tuning in para direction works as intended
 * tracker_info structure
 * \req
 * NA
 */
TEST(Adaptive_Tuning_Of_Proc_Noise_Cov, Test_Fast_Para_Tuning)
{
   /** \precond
    * Use default settings from test group except for
    *    - set object tangential acceleration to be large
    */
   obj.tang_accel = calibs.k_max_acc_for_increasing_acc_noise + 0.1F;

   /** \action
    * Call Time_Update_Object_Track_CTCA()
    */
   float32_t q[STATE_DIMENSION][STATE_DIMENSION] = {};
   Adaptive_Tuning_Of_Proc_Noise_Cov(elapsed_time, obj, calibs, q);

   /** \result
    * Verify that outputted Q matrix is as expected in para direction
    */
   DOUBLES_EQUAL(2.5F, q[5][5], test_pass_threshold);
   DOUBLES_EQUAL(0.00208125F, q[4][4], test_pass_threshold);
   DOUBLES_EQUAL(0.01000078125F, q[0][0], test_pass_threshold);
}

/** \purpose
 * Verify that the middle fast tuning in para direction works as intended
 * tracker_info structure
 * \req
 * NA
 */
TEST(Adaptive_Tuning_Of_Proc_Noise_Cov, Test_Mid_Para_Tuning)
{
   /** \precond
    * Use default settings from test group except for
    *    - set object tangential acceleration to be mid large
    */
   obj.tang_accel = 0.5F * (calibs.k_max_acc_for_increasing_acc_noise + calibs.k_min_acc_for_increasing_acc_noise);

   /** \action
    * Call Time_Update_Object_Track_CTCA()
    */
   float32_t q[STATE_DIMENSION][STATE_DIMENSION] = {};
   Adaptive_Tuning_Of_Proc_Noise_Cov(elapsed_time, obj, calibs, q);

   /** \result
    * Verify that outputted Q matrix is as expected in para direction
    */
   DOUBLES_EQUAL(1.3125F, q[5][5], test_pass_threshold);
   DOUBLES_EQUAL(0.00109265625F, q[4][4], test_pass_threshold);
   DOUBLES_EQUAL(0.01000041015625F, q[0][0], test_pass_threshold);
}


/** \purpose
 * Verify that the slow speed tuning in orth direction works as intended
 * tracker_info structure
 * \req
 * NA
 */
TEST(Adaptive_Tuning_Of_Proc_Noise_Cov, Test_Slow_Speed_Orth_Tuning)
{
   /** \precond
    * Use default settings from test group
    */

   /** \action
    * Call Time_Update_Object_Track_CTCA()
    */
   float32_t q[STATE_DIMENSION][STATE_DIMENSION] = {};
   Adaptive_Tuning_Of_Proc_Noise_Cov(elapsed_time, obj, calibs, q);

   /** \result
    * Verify that outputted Q matrix is as expected in orth direction
    */
   DOUBLES_EQUAL(2.082465639316951e-04F, q[3][3], test_pass_threshold);
   DOUBLES_EQUAL(2.123724489795919e-06F, q[2][2], test_pass_threshold);
   DOUBLES_EQUAL(0.002500009765625F, q[1][1], test_pass_threshold);
}

/** \purpose
 * Verify that the fast speed tuning in orth direction works as intended
 * tracker_info structure
 * \req
 * NA
 */
TEST(Adaptive_Tuning_Of_Proc_Noise_Cov, Test_Fast_Speed_Orth_Tuning)
{
   /** \precond
    * Use default settings from test group except for:
    *    - Change object speed to be large
    */
   obj.speed = 20.0F; // Should be larger than 7.0m/s which is the trhehold for "large speed"

   /** \action
    * Call Time_Update_Object_Track_CTCA()
    */
   float32_t q[STATE_DIMENSION][STATE_DIMENSION] = {};
   Adaptive_Tuning_Of_Proc_Noise_Cov(elapsed_time, obj, calibs, q);

   /** \result
    * Verify that outputted Q matrix is as expected in orth direction
    */
   DOUBLES_EQUAL(1.206597128125e-06F, q[3][3], test_pass_threshold);
   DOUBLES_EQUAL(4.017968436656252e-07F, q[2][2], test_pass_threshold);
   DOUBLES_EQUAL(0.002500060329856F, q[1][1], test_pass_threshold);
}

/** \purpose
 * Verify that rotation from TCS to VCS works as intended
 * tracker_info structure
 * \req
 * NA
 */
TEST(Adaptive_Tuning_Of_Proc_Noise_Cov, Test_TCS_To_VCS_Rotation)
{
   /** \precond
    * Use default settings from test group except for:
    *    - Change object heading to non-zero (in this test we use -88 degrees)
    */
   obj.vcs_heading.Value(F360_DEG2RAD(-88.0F));

   /** \action
    * Call Time_Update_Object_Track_CTCA()
    */
   float32_t q[STATE_DIMENSION][STATE_DIMENSION] = {};
   Adaptive_Tuning_Of_Proc_Noise_Cov(elapsed_time, obj, calibs, q);

   /** \result
    * Verify that outputted Q matrix is as expected after rotation to VCS
    */
   DOUBLES_EQUAL(0.002509144612834F, q[0][0], test_pass_threshold);
   DOUBLES_EQUAL(0.009990904215291F, q[1][1], test_pass_threshold);
   DOUBLES_EQUAL(-0.000261587798364F, q[0][1], test_pass_threshold);
   DOUBLES_EQUAL(-0.000261587798364F, q[1][0], test_pass_threshold);
   DOUBLES_EQUAL(2.123724489795919e-06F, q[2][2], test_pass_threshold);
   DOUBLES_EQUAL(2.082465639316951e-04F, q[3][3], test_pass_threshold);
   DOUBLES_EQUAL(1.040625e-04F, q[4][4], test_pass_threshold);
   DOUBLES_EQUAL(0.125F, q[5][5], test_pass_threshold);
}
/** @}*/


/** \defgroup  Saturate_Curvature_Variance
 *  @{
 */

/** \brief
 * Test group to test functionality of function Saturate_Curvature_Variance().
 */
TEST_GROUP(Saturate_Curvature_Variance)
{
   // Declare common variables used within all tests in this test group.
   const float32_t k_max_allowed_curvature_variance = 0.04F;
   float32_t obj_errcov[STATE_DIMENSION][STATE_DIMENSION];
   float32_t expected_correlations[STATE_DIMENSION][STATE_DIMENSION];
   float32_t obj_errcov_before_function_call[STATE_DIMENSION][STATE_DIMENSION];

   const float32_t test_pass_th = F360_EPSILON;

   /** \setup
    * Setup object errcov according to:
    *    - Diagonal elements:
    *       - Can be choosen randomly (but must be positive) except for the curvature elemnt which needs to be larger than k_max_allowed_curvature_variance.
    *       - In this test setup we choose [1, 2, 3, 0.05 (this is the curvature state), 4, 5]
    *    - Non-diagonal elements:
    *       - Can be choosen randomly as long as the errcov matrix is a valid covariance matrix.
    *       - In this test setup we set up an expected symmetric correlation matrix where the elements are choosen randomly in the interval [-1, 1].
    *         Expected correlations are in this test setup:
    *            - x position - curvature: 0.1
    *            - y position - curbature -0.2
    *            - heading - curvature: 0.3
    *            - speed - curvature: -0.4
    *            - acceleration - curvature: 0.5
    *            - remaining: 0.9
    * Copy errcov such that we can compare the, by the function call modified errcov, with the errcov before the function call
    */
   TEST_SETUP()
   {
      // Setup expected correlation
      expected_correlations[0][0] = 1.0F; // Diagonal element always have 1 correlation for a valid covariance matrix
      expected_correlations[0][1] = 0.9F;
      expected_correlations[0][2] = 0.9F;
      expected_correlations[0][3] = 0.1F; // Correlation with curvature state
      expected_correlations[0][4] = 0.9F;
      expected_correlations[0][5] = 0.9F;
      expected_correlations[1][0] = expected_correlations[0][1];
      expected_correlations[1][1] = 1.0F; // Diagonal element always have 1 correlation for a valid covariance matrix
      expected_correlations[1][2] = 0.9F;
      expected_correlations[1][3] = -0.2F; // Correlation with curvature state
      expected_correlations[1][4] = 0.9F;
      expected_correlations[1][5] = 0.9F;
      expected_correlations[2][0] = expected_correlations[0][2];
      expected_correlations[2][1] = expected_correlations[1][2];
      expected_correlations[2][2] = 1.0F; // Diagonal element always have 1 correlation for a valid covariance matrix
      expected_correlations[2][3] = 0.3F; // Correlation with curvature state
      expected_correlations[2][4] = 0.9F;
      expected_correlations[2][5] = 0.9F;
      expected_correlations[3][0] = expected_correlations[0][3];
      expected_correlations[3][1] = expected_correlations[1][3];
      expected_correlations[3][2] = expected_correlations[2][3];
      expected_correlations[3][3] = 1.0F; // Diagonal element always have 1 correlation for a valid covariance matrix
      expected_correlations[3][4] = -0.4F; // Correlation with curvature state
      expected_correlations[3][5] = 0.5F; // Correlation with curvature state
      expected_correlations[4][0] = expected_correlations[0][4];
      expected_correlations[4][1] = expected_correlations[1][4];
      expected_correlations[4][2] = expected_correlations[2][4];
      expected_correlations[4][3] = expected_correlations[3][4]; 
      expected_correlations[4][4] = 1.0F;// Diagonal element always have 1 correlation for a valid covariance matrix
      expected_correlations[4][5] = 0.9F;
      expected_correlations[5][0] = expected_correlations[0][5];
      expected_correlations[5][1] = expected_correlations[1][5];
      expected_correlations[5][2] = expected_correlations[2][5];
      expected_correlations[5][3] = expected_correlations[3][5]; 
      expected_correlations[5][4] = expected_correlations[4][5];
      expected_correlations[5][5] = 1.0F; // Diagonal element always have 1 correlation for a valid covariance matrix

      // Setup errcov diagonal elements
      obj_errcov[0][0] = 1.0F;
      obj_errcov[1][1] = 2.0F;
      obj_errcov[2][2] = 3.0F;
      obj_errcov[3][3] = 0.05F;
      obj_errcov[4][4] = 4.0F;
      obj_errcov[5][5] = 5.0F;

      // Setup errcov off-diagonal elements
      for(uint32_t row = 0U; row < STATE_DIMENSION; row++)
      {
         for(uint32_t col = 0U; col < STATE_DIMENSION; col++)
         {
            if(row == col)
            {
               continue;
            }
            else
            {
               obj_errcov[row][col] = expected_correlations[row][col] * F360_Sqrtf(obj_errcov[row][row]) * F360_Sqrtf(obj_errcov[col][col]);
            }
         }
      }

      std::copy(cmn::begin(obj_errcov), cmn::end(obj_errcov), cmn::begin(obj_errcov_before_function_call));
   }
};

/** \purpose
 * Verify that the saturation works as expected when the curvature state varaiance is too large
 *    - the curvature state varaiance is saturated as expected when too large
 *    - the corresponding curvature related cross-covariances are modified to kepp the same correlation
 *    - other elements of the errcov are unchanged
 * \req
 * NA
 */
TEST(Saturate_Curvature_Variance, Test_Saturation)
{
   /** \precond
    * Use default settings from test group
    */

   /** \action
    * Call Saturate_Curvature_Variance()
    */
   Saturate_Curvature_Variance(k_max_allowed_curvature_variance, obj_errcov);

   /** \result
    * Verify that outputted errcov is as expected. Expected is that:
    *    - the curvature state varaiance is set to 
    *    - the corresponding curvature related cross-covariances are modified to keep the same correlation
    *    - other elements of the errcov are unchanged
    */
   for (uint8_t row = 0U; row < STATE_DIMENSION; row++)
   {
      for (uint8_t col = 0U; col < STATE_DIMENSION; col++)
      {
         if((3U == row) && (3U == col)) // Curvature variance
         {
            DOUBLES_EQUAL_TEXT(k_max_allowed_curvature_variance, obj_errcov[3][3], test_pass_th, "Curvature variance is not saturated as expected.");
         }
         else if((3U == row) || (3U == col)) // Cross-varaince related to curvature state
         {
            const float32_t expected_cross_covariance = expected_correlations[row][col] * F360_Sqrtf(obj_errcov[row][row]) * F360_Sqrtf(obj_errcov[col][col]);
            DOUBLES_EQUAL_TEXT(expected_cross_covariance, obj_errcov[row][col], test_pass_th, "Correlation between curvature state and other states are not kept.");
         }
         else
         {
            DOUBLES_EQUAL_TEXT(obj_errcov_before_function_call[row][col], obj_errcov[row][col], test_pass_th, "Elements not related to curvature are unexpectedly changed.");
         }    
      }
   }
}

/** \purpose
 * Verify that the object errcov is unchanged when curvature variance is small
 * \req
 * NA
 */
TEST(Saturate_Curvature_Variance, Test_No_Saturation)
{
   /** \precond
    * Use default settings from test group except for modify the curvature variance to be smaller than k_max_allowed_curvature_variance
    */
   obj_errcov[3][3] = 0.04F - F360_EPSILON;
   obj_errcov_before_function_call[3][3] = obj_errcov[3][3];

   /** \action
    * Call Saturate_Curvature_Variance()
    */
   Saturate_Curvature_Variance(k_max_allowed_curvature_variance, obj_errcov);

   /** \result
    * Verify that outputted errcov is unchanged by the function call
    */
   for (uint8_t row = 0U; row < STATE_DIMENSION; row++)
   {
      for (uint8_t col = 0U; col < STATE_DIMENSION; col++)
      {
         DOUBLES_EQUAL_TEXT(obj_errcov_before_function_call[row][col], obj_errcov[row][col], test_pass_th, "Elements not related to curvature are unexpectedly changed.");  
      }
   }
}
/** @}*/
