/** \file
 * This file contains unit tests for content of f360_adjust_fltr_type_dependet_params.cpp file
*/

#include "f360_adjust_fltr_type_dependent_params.h"
#include "f360_trk_fltr_ccv_states.h"
#include "f360_trk_fltr_cca_states.h"
#include "f360_trk_fltr_ctca_states.h"
#include "f360_object_track_equal_operator.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <cfloat>

/** \defgroup  f360_adjust_fltr_type_dependet_params
 *  @{
 */

 /** \brief
  *  Set up one valid object that is to be used to test functionality of Adjust_Fltr_Type_Dependent_Params().
  */
using namespace f360_variant_A;

TEST_GROUP(f360_adjust_fltr_type_dependet_params)
{
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Calibrations_T calibrations = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};

   float32_t object_errorcov_ref[STATE_DIMENSION][STATE_DIMENSION] = {};
   float32_t test_epsilon = 0.001F;

   /** \setup
    * For object with idx 3, set:
    *   Filter type to CCA
    *   Moving flag to true
    *   Speed to be above fast moving calibration threshold
    *   Overall confidence to low
    *   Initialize errcov with some default values
    *   Initialize vca_heading to 0, heading pointing disagreement to just below the threshold for allowing CCA -> CTCA transition and vcs_poinitng to vcs_heading + heading pointing disagreement
    * Set number of active objects to 1
    * Set first slot in active object ids to 4 (corresponds to idx 3)
    *
   **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);

      calibrations.fast_moving_thresh = 5.0F;

      for (unsigned int idx = 0; idx < NUMBER_OF_OBJECT_TRACKS; idx++)
      {
         object_tracks[idx].trk_fltr_type = F360_TRACKER_TRKFLTR_INVALID;
         object_tracks[idx].f_moving = false;
         object_tracks[idx].speed = calibrations.fast_moving_thresh - test_epsilon;
      }

      object_tracks[3].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      object_tracks[3].f_moving = true;
      object_tracks[3].speed = calibrations.fast_moving_thresh + test_epsilon;
      object_tracks[3].vcs_heading.Value(0.0F);
      object_tracks[3].hdg_ptng_disagmt = F360_Asinf(0.25F / object_tracks[3].speed) - test_epsilon;
      object_tracks[3].bbox.Set_Orientation(object_tracks[3].vcs_heading + object_tracks[3].hdg_ptng_disagmt);

      object_tracks[3].time_since_initialization = 2.0F + test_epsilon;
      object_tracks[3].curvature = 0.018F - test_epsilon;
      object_tracks[3].heading_rate = object_tracks[3].curvature * object_tracks[3].speed;

      for (unsigned int row = 0; row < STATE_DIMENSION; row++)
      {
         for (unsigned int col = 0; col < STATE_DIMENSION; col++)
         {
            object_errorcov_ref[row][col] = (row + col) * 0.03F;
            object_tracks[3].errcov[row][col] = object_errorcov_ref[row][col];
         }
      }

      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 4;
   }
};

/**
*\purpose   Correct switching to CTCA (already CTCA)
*\req       N/A
*/
TEST(f360_adjust_fltr_type_dependet_params, Adjust_Fltr_Type_Dependent_Params__no_switch_due_to_already_CTCA)
{
   /** \precond
   **/
   object_tracks[3].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;

   /** \action
   **/
   Adjust_Fltr_Type_Dependent_Params(tracker_info, object_tracks, calibrations, timing_info);

   /** \result
   **/
   CHECK_EQUAL(object_tracks[3].trk_fltr_type, F360_TRACKER_TRKFLTR_CTCA);
}

/**
*\purpose   Correct covariance update
*\req       N/A
*/
TEST(f360_adjust_fltr_type_dependet_params, Adjust_Fltr_Type_Dependent_Params__low_overall_conf_update_error_cov_to_CTCA)
{
   /** \precond
   **/
   object_tracks[3].f_moving = true;
   object_tracks[3].speed = calibrations.fast_moving_thresh + test_epsilon;
   object_tracks[3].conf_overall = CONF3_LOW;

   float32_t exp_errcov[STATE_DIMENSION][STATE_DIMENSION] = {};
   exp_errcov[0][0] = object_tracks[3].errcov[0][0];
   exp_errcov[0][1] = object_tracks[3].errcov[0][3];
   exp_errcov[1][0] = object_tracks[3].errcov[3][0];
   exp_errcov[1][1] = object_tracks[3].errcov[3][3];
   exp_errcov[2][2] = 0.01F;
   exp_errcov[3][3] = 0.001F;
   exp_errcov[4][4] = object_tracks[3].errcov[4][4] * (object_tracks[3].vcs_heading.Sin() * object_tracks[3].vcs_heading.Sin()) + object_tracks[3].errcov[1][1] * (object_tracks[3].vcs_heading.Cos() * object_tracks[3].vcs_heading.Cos()) - 2.0F * object_tracks[3].errcov[1][4] * (object_tracks[3].vcs_heading.Sin() * object_tracks[3].vcs_heading.Cos());
   exp_errcov[5][5] = object_tracks[3].errcov[5][5] * (object_tracks[3].vcs_heading.Sin() * object_tracks[3].vcs_heading.Sin()) + object_tracks[3].errcov[2][2] * (object_tracks[3].vcs_heading.Cos() * object_tracks[3].vcs_heading.Cos()) - 2.0F * object_tracks[3].errcov[2][5] * (object_tracks[3].vcs_heading.Sin() * object_tracks[3].vcs_heading.Cos());

   /** \action
   **/
   Adjust_Fltr_Type_Dependent_Params(tracker_info, object_tracks, calibrations, timing_info);

   /** \result
   **/
   CHECK_EQUAL(object_tracks[3].trk_fltr_type, F360_TRACKER_TRKFLTR_CTCA);
   CHECK_EQUAL(object_tracks[3].conf_speed, CONF9_LOW4);

   for (int row = 0; row < STATE_DIMENSION; row++)
   {
      for (int col = 0; col < STATE_DIMENSION; col++)
      {
         DOUBLES_EQUAL(object_tracks[3].errcov[row][col], exp_errcov[row][col], 0.0001F);
      }
   }
}

/**
*\purpose   Verify that correct confidence level has been given to speed when
*           object overall confidence is high
*\req       N/A
*/
TEST(f360_adjust_fltr_type_dependet_params, Adjust_Fltr_Type_Dependent_Params__high_overall_conf_update_speed_accel_heading_conf)
{
   /** \precond
   * Set the objects overall confidence to high
   **/
   object_tracks[3].conf_overall = CONF3_HIGH;
   object_tracks[3].f_moving = true;
   object_tracks[3].speed = calibrations.fast_moving_thresh + test_epsilon;
   

   /** \action
   * Call Adjust_Fltr_Type_Dependent_Params
   **/
   Adjust_Fltr_Type_Dependent_Params(tracker_info, object_tracks, calibrations, timing_info);

   /** \result
   * Check that filter type is CTCA and that confidence level for speed, acceleration and heading is high.
   **/
   CHECK_EQUAL(object_tracks[3].trk_fltr_type, F360_TRACKER_TRKFLTR_CTCA);
   CHECK_EQUAL(object_tracks[3].conf_speed, CONF9_HIGH);
}

/**
*\purpose   Verify that correct confidence level has been given to speed when
*           object overall confidence is medium
*\req       N/A
*/
TEST(f360_adjust_fltr_type_dependet_params, Adjust_Fltr_Type_Dependent_Params__medium_overall_conf_update_speed_accel_heading_conf)
{
   /** \precond
   * Set the objects overall confidence to medium
   **/
   object_tracks[3].conf_overall = CONF3_MED;
   object_tracks[3].f_moving = true;
   object_tracks[3].speed = calibrations.fast_moving_thresh + test_epsilon;

   /** \action
   * Call Adjust_Fltr_Type_Dependent_Params
   **/
   Adjust_Fltr_Type_Dependent_Params(tracker_info, object_tracks, calibrations, timing_info);

   /** \result
   * Check that filter type is CCA and that confidence level for speed, acceleration and heading is medium.
   **/
   CHECK_EQUAL(object_tracks[3].trk_fltr_type, F360_TRACKER_TRKFLTR_CTCA);
   CHECK_EQUAL(object_tracks[3].conf_speed, CONF9_MED4);
}

/**
*\purpose   Verify that the filter type is not changed when CCA but not fast moving
*\req       N/A
*/
TEST(f360_adjust_fltr_type_dependet_params, Adjust_Fltr_Type_Dependent_Params__CCA_moving_but_not_fast_moving)
{
   /** \precond
   * Set the objects properties
   **/
   object_tracks[3].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[3].conf_overall = CONF3_MED;
   object_tracks[3].f_moving = true;
   object_tracks[3].speed = calibrations.fast_moving_thresh - F360_EPSILON;

   /** \action
   * Call Adjust_Fltr_Type_Dependent_Params
   **/
   Adjust_Fltr_Type_Dependent_Params(tracker_info, object_tracks, calibrations, timing_info);

   /** \result
   * Check that filter type is CCA
   **/
   CHECK_EQUAL(object_tracks[3].trk_fltr_type, F360_TRACKER_TRKFLTR_CCA);
}

/**
*\purpose   Verify that the filter type is not changed when object is newly initialized CCA object
*\req       N/A
*/
TEST(f360_adjust_fltr_type_dependet_params, Adjust_Fltr_Type_Dependent_Params__newly_initlialized_CCA_object)
{
   /** \precond
   * Set the objects properties
   **/
   object_tracks[3].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[3].conf_overall = CONF3_MED;
   object_tracks[3].f_moving = true;
   object_tracks[3].speed = calibrations.fast_moving_thresh + F360_EPSILON;
   object_tracks[3].time_since_initialization = 1.0F;

   /** \action
   * Call Adjust_Fltr_Type_Dependent_Params
   **/
   Adjust_Fltr_Type_Dependent_Params(tracker_info, object_tracks, calibrations, timing_info);

   /** \result
   * Check that filter type is CCA
   **/
   CHECK_EQUAL(object_tracks[3].trk_fltr_type, F360_TRACKER_TRKFLTR_CCA);
}

/**
*\purpose   Verify that the filter type is not changed when object is CCA and curvature is above threshold
*\req       N/A
*/
TEST(f360_adjust_fltr_type_dependet_params, Adjust_Fltr_Type_Dependent_Params__CCA_curvature_above_threshold)
{
   /** \precond
   * Set the objects properties
   **/
   object_tracks[3].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
   object_tracks[3].conf_overall = CONF3_MED;
   object_tracks[3].f_moving = true;
   object_tracks[3].speed = calibrations.fast_moving_thresh + F360_EPSILON;
   object_tracks[3].curvature = 0.018F + F360_EPSILON;

   /** \action
   * Call Adjust_Fltr_Type_Dependent_Params
   **/
   Adjust_Fltr_Type_Dependent_Params(tracker_info, object_tracks, calibrations, timing_info);

   /** \result
   * Check that filter type is CCA
   **/
   CHECK_EQUAL(object_tracks[3].trk_fltr_type, F360_TRACKER_TRKFLTR_CCA);
}

/**
*\purpose   Verify that the filter type is not changed when object is CCA and heading pointing disagreement is above threshold
*\req       N/A
*/
TEST(f360_adjust_fltr_type_dependet_params, Adjust_Fltr_Type_Dependent_Params__CCA_hdg_pnt_disag_above_threshold)
{
   /** \precond
   * use default settings from test group (which allow cca-> CTCA transition) but chnage the heading pointing disagreement to be above the threshold such that it prevents the transision
   **/
   object_tracks[3].hdg_ptng_disagmt = F360_Asinf(0.4F / object_tracks[3].speed) + test_epsilon;

   /** \action
   * Call Adjust_Fltr_Type_Dependent_Params
   **/
   Adjust_Fltr_Type_Dependent_Params(tracker_info, object_tracks, calibrations, timing_info);

   /** \result
   * Check that filter type is CCA
   **/
   CHECK_EQUAL(object_tracks[3].trk_fltr_type, F360_TRACKER_TRKFLTR_CCA);
}
/** @}*/


/** \defgroup  f360_adjust_fltr_type_dependet_params_CCA_to_CTCA
 *  @{
 */

 /** \brief
  *  This test group testst that switching from CCA to CTCA is correct
  */
TEST_GROUP(f360_adjust_fltr_type_dependet_params_CCA_to_CTCA)
{
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Calibrations_T calibrations = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};

   float32_t object_errorcov_ref[STATE_DIMENSION][STATE_DIMENSION] = {};
   float32_t test_pass_th = 1e-5F;

   /** \setup
    * Setup default tracker calibrations
    * Setup one active object with idx 16
    *    Filter type: CCA
    *    Time since initialization: larger than 2.0F (2.0F + 1e-3 is used in the test)
    *    Curvature: smaller than 0.018F (0.018F - 1e-3 is used in the test)
    *    VCS heading: 78deg
    *    Confidence overall: CONF3_MED
    *    Errcov: random covariance matrix ([10.1682   -5.4370    7.4322   -4.1438    0.2407  -10.3912
    *                                       -5.4370    9.5900    1.8433    3.1668   -2.5158    9.0877
    *                                       7.4322    1.8433   15.4084   -0.0900    3.5283   -0.3546
    *                                       -4.1438    3.1668   -0.0900    7.4868    2.1847    8.0054
    *                                       0.2407   -2.5158    3.5283    2.1847    8.6328    0.4575
    *                                       -10.3912    9.0877   -0.3546    8.0054    0.4575   22.5249] is used in the test
])
    *
   **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);

      tracker_info.num_active_objs = 1;
      tracker_info.active_obj_ids[0] = 17;

      object_tracks[tracker_info.active_obj_ids[0]-1].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      object_tracks[tracker_info.active_obj_ids[0]-1].time_since_initialization = 2.0F + 1e-3F;
      object_tracks[tracker_info.active_obj_ids[0]-1].curvature = 0.018F - 1e-3F;;
      object_tracks[tracker_info.active_obj_ids[0]-1].vcs_heading.Value(F360_DEG2RAD(78.0F));
      object_tracks[tracker_info.active_obj_ids[0]-1].conf_overall = CONF3_MED;
      object_tracks[tracker_info.active_obj_ids[0]-1].f_moving = true;
      object_tracks[tracker_info.active_obj_ids[0]-1].speed = calibrations.fast_moving_thresh + 1e-3F;

      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_X] = 10.1682F;
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_VX] = -5.4370F;
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_AX] = 7.4322F;
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_Y] = -4.1438F;
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_VY] = 0.2407F;
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_AY] = -10.3912F;

      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_X] = object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_VX];
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX] = 9.5900F;
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_AX] = 1.8433F;
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_Y] = 3.1668F;
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY] = -2.5158F;
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_AY] = 9.0877F;

      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_X] = object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_AX];
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_VX] = object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_AX];;
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AX] = 15.4084F;
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_Y] = -0.0900F;
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_VY] = 3.5283F;
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AY] = -0.3546F;

      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_X] = object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_Y];
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_VX] = object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_Y];;
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_AX] = object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_Y];
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_Y] = 7.4868F;
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_VY] = 2.1847F;
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_AY] = 8.0054F;

      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_X] = object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_VY];
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VX] = object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY];
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_AX] = object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_VY];
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_Y] = object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_VY];
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY] = 8.6328F;
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_AY] = 0.4575F;

      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_X] = object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_AY];
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_VX] = object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_AY];
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AX] = object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_AY];
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_Y] = object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_AY];
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_VY] = object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_AY];;
      object_tracks[tracker_info.active_obj_ids[0]-1].errcov[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AY] = 22.5249F;
   }
};

/**
*\purpose   Test correct switching to CTCA from CCA
*\req       N/A
*/
TEST(f360_adjust_fltr_type_dependet_params_CCA_to_CTCA, Test_Switching_from_CCA_To_CTCA)
{
   /** \precond
    * Use default test data from test group
   **/

   /** \action
    * Call Adjust_Fltr_Type_Dependent_Params()
   **/
   Adjust_Fltr_Type_Dependent_Params(tracker_info, object_tracks, calibrations, timing_info);

   /** \result
    * Test that:
    *    - object filter type is CTCA
    *    - object errcov is [10.1682, -4.1438, 0.0, 0.0, 0.0, 0.0
    *                        -4.1438, 7.4868,  0.0, 0.0, 0.0, 0.0]
    *                        0.0, 0.0, 0.01, 0.0, 0.0, 0.0
    *                        0.0, 0.0, 0.0, 0.001, 0.0, 0.0, 
    *                        0.0, 0.0, 0.0, 0.0, 7.650909097322152, 0.0
    *                        0.0, 0.0, 0.0, 0.0, 0.0, 22.073044311022105]
    *    - object speed confidence is CONF9_MED4;
    *                        
    *    - object speed confidence is
   **/
  float32_t exp_errcov[STATE_DIMENSION][STATE_DIMENSION] = {};
  exp_errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_X] = 10.1682F;
  exp_errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_Y] = -4.1438F;
  exp_errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_X] = exp_errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_Y];
  exp_errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_Y] = 7.4868F;
  exp_errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_H] = 0.01F;
  exp_errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_C] = 0.001F;
  exp_errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_S] = 7.650909097322152F;
  exp_errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_A] = 22.073044311022105F;

   CHECK_EQUAL_TEXT(object_tracks[tracker_info.active_obj_ids[0]-1].trk_fltr_type, F360_TRACKER_TRKFLTR_CTCA, "Filter type is wrong");
   for(uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx++)
   {
      for(uint32_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx++)
      {
         DOUBLES_EQUAL_TEXT(exp_errcov[row_idx][col_idx], object_tracks[tracker_info.active_obj_ids[0]-1].errcov[row_idx][col_idx], test_pass_th, "Errcov is wrong");
      }
   }

   CHECK_EQUAL_TEXT(object_tracks[tracker_info.active_obj_ids[0]-1].conf_speed, CONF9_MED4, "Speed confidence is wrong");
}

/**
*\purpose   Test that there is not filter switch from CCA to CTCA when time_since_init is small
*\req       N/A
*/
TEST(f360_adjust_fltr_type_dependet_params_CCA_to_CTCA, Test_No_Switching_From_CCA_To_CTCA_due_to_small_time_since_init)
{
   /** \precond
    * Use default test data from test group except for
    *    - set time_since_init to smaller than 2.0F (2.0F -1e-3 is used in the test)
   **/
  object_tracks[tracker_info.active_obj_ids[0]-1].time_since_initialization = 2.0F - 1e-3F;

   /** \action
    * Create a copy of the active object before calling the function so that comparisions can be made later
    * Call Adjust_Fltr_Type_Dependent_Params()
   **/
   const F360_Object_Track_T obj_before = object_tracks[tracker_info.active_obj_ids[0]-1];
   Adjust_Fltr_Type_Dependent_Params(tracker_info, object_tracks, calibrations, timing_info);

   /** \result
    * Test that:
    *    - the object has not been modified
   **/
   CHECK_TRUE(obj_before == object_tracks[tracker_info.active_obj_ids[0]-1]);
}

/**
*\purpose   Test that there is not filter switch from CCA to CTCA when curvature is large
*\req       N/A
*/
TEST(f360_adjust_fltr_type_dependet_params_CCA_to_CTCA, Test_No_Switching_From_CCA_To_CTCA_due_to_large_curvature)
{
   /** \precond
    * Use default test data from test group except for
    *    - set curvature to larget than 0.018F (0.018F + 1e-3 is used in the test)
   **/
  object_tracks[tracker_info.active_obj_ids[0]-1].time_since_initialization = 0.018F + 1e-3F;

   /** \action
    * Create a copy of the active object before calling the function so that comparisions can be made later
    * Call Adjust_Fltr_Type_Dependent_Params()
   **/
   const F360_Object_Track_T obj_before = object_tracks[tracker_info.active_obj_ids[0]-1];
   Adjust_Fltr_Type_Dependent_Params(tracker_info, object_tracks, calibrations, timing_info);

   /** \result
    * Test that:
    *    - the object has not been modified
   **/
   CHECK_TRUE(obj_before == object_tracks[tracker_info.active_obj_ids[0]-1]);
}
/** @}*/

/** \defgroup  Adjust_Fltr_Type_CTCA_To_CCA
 *  @{
 */

 /** \brief
  *  This test group verifies correct switching to CTCA from CCA 
  *  Note that it can only be called from object motion classification as this test case is being written
  */
TEST_GROUP(Adjust_Fltr_Type_CTCA_To_CCA)
{
   F360_Object_Track_T object_track = {};
   F360_Calibrations_T calibrations = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};

   float32_t object_errorcov_ref[STATE_DIMENSION][STATE_DIMENSION] = {};
   float32_t test_pass_th = 1e-4F;

   /** \setup
    * Setup default tracker calibrations
    * Setup one active object with idx 16
    *    Filter type: CTCA
    *    Confidence overall: CONF3_MED
    *    Errcov: random covariance matrix ([10.1682   -5.4370    7.4322   -4.1438    0.2407  -10.3912
    *                                       -5.4370    9.5900    1.8433    3.1668   -2.5158    9.0877
    *                                       7.4322    1.8433   15.4084   -0.0900    3.5283   -0.3546
    *                                       -4.1438    3.1668   -0.0900    7.4868    2.1847    8.0054
    *                                       0.2407   -2.5158    3.5283    2.1847    8.6328    0.4575
    *                                       -10.3912    9.0877   -0.3546    8.0054    0.4575   22.5249] is used in the test
])
    *
   **/
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibrations);

      object_track.movable_prob = 0.0F;
      object_track.tang_accel = 2.0F;
      object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
      object_track.conf_overall = CONF3_MED;
      object_track.speed = calibrations.fast_moving_thresh + 1e-3F;

      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_X] = 10.1682F;
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_Y] = -5.4370F;
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_H] = 7.4322F;
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_C] = -4.1438F;
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_S] = 0.2407F;
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_A] = -10.3912F;

      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_X] = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_Y];
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_Y] = 9.5900F;
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_H] = 1.8433F;
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_C] = 3.1668F;
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_S] = -2.5158F;
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_A] = 9.0877F;

      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_X] = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_H];
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_Y] = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_H];;
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_H] = 15.4084F;
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_C] = -0.0900F;
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_S] = 3.5283F;
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_A] = -0.3546F;

      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_X] = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_C];
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_Y] = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_C];;
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_H] = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_C];
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_C] = 7.4868F;
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_S] = 2.1847F;
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_A] = 8.0054F;

      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_X] = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_S];
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_Y] = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_S];
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_H] = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_S];
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_C] = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_S];
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_S] = 8.6328F;
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_A] = 0.4575F;

      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_X] = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_A];
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_Y] = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_A];
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_H] = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_A];
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_C] = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_A];
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_S] = object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_A];;
      object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_A] = 22.5249F;
   }
};

/**
*\purpose   Test correct error covariance transformation from CTCA to CCA.
*\req       N/A
*/
TEST(Adjust_Fltr_Type_CTCA_To_CCA, Test_Switching_from_CTCA_CCA)
{
   /** \precond
    * Use default test data from test group
   **/

   /** \action
    * Call Adjust_Fltr_Type_CTCA_To_CCA()
   **/
   Adjust_Fltr_Type_CTCA_To_CCA(object_track);

   /** \result
    * Test that:
    *    - object filter type is CCA
    *    - object errcov is [10.1682, 0.0, 0.0, -5.4370F, 0.0, 0.0
    *                        0.0, 8.6328,  0.0, 0.0, 10.5884, 0.0]
    *                        0.0, 0.0, 22.5249, 0.0, 0.0, -20.7824
    *                        -5.4370, 0.0, 0.0, 9.5900, 0.0, 0.0, 
    *                        0.0, 10.5884, 0.0, 0.0, 138.7681, 0.0
    *                        0.0, -0.7092, 0.0, 0.0, 0.0, 61.6336]
    *    - object speed confidence is CONF9_MED4;
    *                        
    *    - object speed confidence is
   **/
   float32_t exp_errcov[STATE_DIMENSION][STATE_DIMENSION] = {};
   exp_errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_X] = 10.1682F;
   exp_errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_Y] = -5.4370F;
   exp_errcov[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_X] = exp_errcov[F360_TRK_FLTR_CCA_STATE_X][F360_TRK_FLTR_CCA_STATE_Y];
   exp_errcov[F360_TRK_FLTR_CCA_STATE_Y][F360_TRK_FLTR_CCA_STATE_Y] = 9.5900F;
   exp_errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VX] = 8.6328F;
   exp_errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VX] = 10.5884F;
   exp_errcov[F360_TRK_FLTR_CCA_STATE_VX][F360_TRK_FLTR_CCA_STATE_VY] = exp_errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VX];
   exp_errcov[F360_TRK_FLTR_CCA_STATE_VY][F360_TRK_FLTR_CCA_STATE_VY] = 138.7681F;
   exp_errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AX] = 22.5249F;
   exp_errcov[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AY] = 61.6336F;
   exp_errcov[F360_TRK_FLTR_CCA_STATE_AX][F360_TRK_FLTR_CCA_STATE_AY] = -20.7824F;
   exp_errcov[F360_TRK_FLTR_CCA_STATE_AY][F360_TRK_FLTR_CCA_STATE_AX] = -0.7092;

   CHECK_EQUAL_TEXT(object_track.trk_fltr_type, F360_TRACKER_TRKFLTR_CCA, "Filter type is wrong");
   for(uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx++)
   {
      for(uint32_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx++)
      {
         DOUBLES_EQUAL_TEXT(exp_errcov[row_idx][col_idx], object_track.errcov[row_idx][col_idx], test_pass_th, "Errcov is wrong");
      }
   }

   CHECK_EQUAL_TEXT(object_track.conf_speed, CONF9_MED4, "speed confidence is wrong");
}
/** @}*/
