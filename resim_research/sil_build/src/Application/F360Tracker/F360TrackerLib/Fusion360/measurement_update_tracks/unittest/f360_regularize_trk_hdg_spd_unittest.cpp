/** \file
 * This file contains unit tests for content of f360_regularize_trk_hdg_spd.cpp file
 */

#include "f360_regularize_trk_hdg_spd.h"
#include <CppUTest/TestHarness.h>
#include "f360_constants.h"
#include <algorithm>
#include "f360_trk_fltr_ctca_states.h"
#include "f360_iterator.h"


using namespace f360_variant_A;

/** \defgroup  f360_regularize_trk_hdg_spd
 *  @{
 */

/** \brief
 * Test group for testing Regularize_Trk_Hdg_Spd()
 */
TEST_GROUP(f360_regularize_trk_hdg_spd__Regularize_Trk_Hdg_Spd)
{
   const float32_t initial_errcov[6][6] = { {0.1F, 0.1F, 0.1F, 0.1F, 0.1F, 0.1F},
                                             {0.1F, 0.1F, 0.1F, 0.1F, 0.1F, 0.1F},
                                             {0.1F, 0.1F, 0.1F, 0.1F, 0.1F, 0.1F},
                                             {0.1F, 0.1F, 0.1F, 0.1F, 0.1F, 0.1F},
                                             {0.1F, 0.1F, 0.1F, 0.1F, 0.1F, 0.1F},
                                             {0.1F, 0.1F, 0.1F, 0.1F, 0.1F, 0.1F} };

   F360_Calibrations_T calib = {};
   F360_Object_Track_T expected_object_track = {};
   F360_Object_Track_T test_object_track = {};

   /** \setup
    * Initialize calibrations.
    * Initialize two identical object_tracks with arbitrary, non-zero speed, heading, curvature, tang accel, error covariance matrix, and reference point
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      expected_object_track.vcs_heading = Angle{ F360_PI / 4.0F }; // pi/4
      expected_object_track.curvature = 1.0F;
      expected_object_track.tang_accel = 1.0F;
      expected_object_track.reference_point = F360_REFERENCE_POINT_CENTER;
      expected_object_track.min_projection_reference_point = F360_REFERENCE_POINT_CENTER;

      (void)std::copy(cmn::begin(initial_errcov), cmn::end(initial_errcov), cmn::begin(expected_object_track.errcov));


      test_object_track.speed = expected_object_track.speed;
      test_object_track.predicted_speed = expected_object_track.predicted_speed;
      test_object_track.vcs_heading = expected_object_track.vcs_heading;
      test_object_track.curvature = expected_object_track.curvature;
      test_object_track.tang_accel = expected_object_track.tang_accel;
      test_object_track.reference_point = F360_REFERENCE_POINT_CENTER;
      test_object_track.min_projection_reference_point = F360_REFERENCE_POINT_CENTER;

      (void)std::copy(cmn::begin(initial_errcov), cmn::end(initial_errcov), cmn::begin(test_object_track.errcov));

   }

};


/** \purpose  
 * 1. Test that Regularize_Trk_Hdg_Spd flips the sign of speed, heading, curvature, tang accel and related error covariances when obj.speed < -calib.max_reverse_abs_spd.
 * 2. Test that Normalize_Heading_Angle is called and works as intended.
 * \req
 * NA.
 */
TEST(f360_regularize_trk_hdg_spd__Regularize_Trk_Hdg_Spd, calculation_correctness_obj_speed_more_negative_than_max_reverse_speed)
{
   /** \precond
    * 1. Two identical object tracks, expected_object_track and test_object_track
    * 2. Set expected_object_track.speed and test_object_track.speed to a value < -calib.max_reverse_spd
    * 3. Flip the sign of expected_object_track speed, heading, curvature, tang accel and corresponding error cov states
    */
   test_object_track.speed = -calib.k_ctca_msmnt_update_max_reverse_abs_spd - 1.0F;
   expected_object_track.speed = -calib.k_ctca_msmnt_update_max_reverse_abs_spd - 1.0F;
   test_object_track.predicted_speed = -calib.k_ctca_msmnt_update_max_reverse_abs_spd - 1.0F;

   expected_object_track.predicted_speed = -test_object_track.predicted_speed;
   expected_object_track.speed = -expected_object_track.speed;
   expected_object_track.vcs_heading = Angle{ -F360_PI * 3.0F / 4.0F };
   expected_object_track.curvature = -expected_object_track.curvature;
   expected_object_track.tang_accel = -expected_object_track.tang_accel;

   expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_C] = -expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_C];
   expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_S] = -expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_S];
   expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_A] = -expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_A];
   expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_C] = -expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_C];
   expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_S] = -expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_S];
   expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_A] = -expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_A];
   expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_C] = -expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_C];
   expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_S] = -expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_S];
   expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_A] = -expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_A];
   expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_X] = -expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_X];
   expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_Y] = -expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_Y];
   expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_H] = -expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_H];
   expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_X] = -expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_X];
   expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_Y] = -expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_Y];
   expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_H] = -expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_H];
   expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_X] = -expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_X];
   expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_Y] = -expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_Y];
   expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_H] = -expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_H];

   /** \action
    * Call function.
    */
   Regularize_Trk_Hdg_Spd(calib,
                          test_object_track);

   /** \result
    * Expect test_object_track speed, curvature, and tang accel to flip sign.
    * Expect test_object_track_heading to reverse direction, i.e. test_object_track_heading = Norm_head_angle(test_object_track_heading + pi)
    * Expect test_object_track's corresponding errorcov states to flip sign
    */

   DOUBLES_EQUAL_TEXT(expected_object_track.speed, test_object_track.speed, F360_EPSILON, "Object speed was not computed as expected.");
   DOUBLES_EQUAL_TEXT(expected_object_track.predicted_speed, test_object_track.predicted_speed, F360_EPSILON, "Object predicted speed was not computed as expected.");
   DOUBLES_EQUAL_TEXT(expected_object_track.vcs_heading.Value(), test_object_track.vcs_heading.Value(), F360_EPSILON, "Object vcs heading was not computed as expected.");
   DOUBLES_EQUAL_TEXT(expected_object_track.curvature, test_object_track.curvature, F360_EPSILON, "Object curvature was not computed as expected.");
   DOUBLES_EQUAL_TEXT(expected_object_track.tang_accel, test_object_track.tang_accel, F360_EPSILON, "Object tangential acceleration was not computed as expected.");

   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_C], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_C], F360_EPSILON, "Cov between x-pos and curvature was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_S], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_S], F360_EPSILON, "Cov between x-pos and speed was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_A], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_A], F360_EPSILON, "Cov between x-pos and acceleration was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_C], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_C], F360_EPSILON, "Cov between y-pos and curvature was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_S], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_S], F360_EPSILON, "Cov between y-pos and speed was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_A], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_A], F360_EPSILON, "Cov between y-pos and acceleration was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_C], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_C], F360_EPSILON, "Cov between heading and curvature was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_S], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_S], F360_EPSILON, "Cov between heading and speed was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_A], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_A], F360_EPSILON, "Cov between heading and acceleration was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_X], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_X], F360_EPSILON, "Cov between curvature and x-pos was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_Y], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_Y], F360_EPSILON, "Cov between curvature and y-pos was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_H], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_H], F360_EPSILON, "Cov between curvature and heading was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_X], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_X], F360_EPSILON, "Cov between speed and x-pos was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_Y], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_Y], F360_EPSILON, "Cov between speed and y-pos was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_H], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_H], F360_EPSILON, "Cov between speed and heading was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_X], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_X], F360_EPSILON, "Cov between acceleration and x-pos was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_Y], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_Y], F360_EPSILON, "Cov between acceleration and y-pos was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_H], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_H], F360_EPSILON, "Cov between acceleration and heading was not computed as expected." );

}

/** \purpose
 * Test that Regularize_Trk_Hdg_Spd does nothing when obj.speed > -calib.max_reverse_abs_spd
 * \req
 * NA.
 */
TEST(f360_regularize_trk_hdg_spd__Regularize_Trk_Hdg_Spd, calculation_correctness_obj_speed_less_negative_than_max_reverse_speed)
{
   /** \precond
    * 1. Two identical object_track, expected_object_track and test_object_track
    * 2. Set expected_object_track.speed and test_object_track.speed to a value > -calib.max_reverse_spd
    */

   test_object_track.speed = -calib.k_ctca_msmnt_update_max_reverse_abs_spd + 1.0F;
   expected_object_track.speed = -calib.k_ctca_msmnt_update_max_reverse_abs_spd + 1.0F;
   test_object_track.predicted_speed = -test_object_track.speed;

   expected_object_track.predicted_speed = test_object_track.predicted_speed;

   /** \action
    * Call function.
    */
   Regularize_Trk_Hdg_Spd(calib,
                          test_object_track);

   /** \result
    * Expect no change in test_object_track.
    */

   DOUBLES_EQUAL_TEXT(expected_object_track.speed, test_object_track.speed, F360_EPSILON, "Object speed was not computed as expected.");
   DOUBLES_EQUAL_TEXT(expected_object_track.predicted_speed, test_object_track.predicted_speed, F360_EPSILON, "Object predicted speed was not computed as expected.");
   DOUBLES_EQUAL_TEXT(expected_object_track.vcs_heading.Value(), test_object_track.vcs_heading.Value(), F360_EPSILON, "Object vcs heading was not computed as expected.");
   DOUBLES_EQUAL_TEXT(expected_object_track.curvature, test_object_track.curvature, F360_EPSILON, "Object curvature was not computed as expected.");
   DOUBLES_EQUAL_TEXT(expected_object_track.tang_accel, test_object_track.tang_accel, F360_EPSILON, "Object tangential acceleration was not computed as expected.");

   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_C], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_C], F360_EPSILON, "Cov between x-pos and curvature was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_S], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_S], F360_EPSILON, "Cov between x-pos and speed was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_A], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_X][F360_TRK_FLTR_CTCA_STATE_A], F360_EPSILON, "Cov between x-pos and acceleration was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_C], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_C], F360_EPSILON, "Cov between y-pos and curvature was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_S], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_S], F360_EPSILON, "Cov between y-pos and speed was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_A], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_Y][F360_TRK_FLTR_CTCA_STATE_A], F360_EPSILON, "Cov between y-pos and acceleration was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_C], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_C], F360_EPSILON, "Cov between heading and curvature was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_S], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_S], F360_EPSILON, "Cov between heading and speed was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_A], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_H][F360_TRK_FLTR_CTCA_STATE_A], F360_EPSILON, "Cov between heading and acceleration was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_X], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_X], F360_EPSILON, "Cov between curvature and x-pos was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_Y], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_Y], F360_EPSILON, "Cov between curvature and y-pos was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_H], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_C][F360_TRK_FLTR_CTCA_STATE_H], F360_EPSILON, "Cov between curvature and heading was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_X], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_X], F360_EPSILON, "Cov between speed and x-pos was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_Y], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_Y], F360_EPSILON, "Cov between speed and y-pos was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_H], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_S][F360_TRK_FLTR_CTCA_STATE_H], F360_EPSILON, "Cov between speed and heading was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_X], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_X], F360_EPSILON, "Cov between acceleration and x-pos was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_Y], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_Y], F360_EPSILON, "Cov between acceleration and y-pos was not computed as expected." );
   DOUBLES_EQUAL_TEXT(expected_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_H], test_object_track.errcov[F360_TRK_FLTR_CTCA_STATE_A][F360_TRK_FLTR_CTCA_STATE_H], F360_EPSILON, "Cov between acceleration and heading was not computed as expected." );

}

/** \purpose
 * Test that Regularize_Trk_Hdg_Spd correctly changes the reference point, when the current reference point is FRONT_LEFT
 * \req
 * NA.
 */
TEST(f360_regularize_trk_hdg_spd__Regularize_Trk_Hdg_Spd, calculate_FRONT_LEFT_refPoint_reverse)
{
   /** \precond
    * 1. Set the test object's speed to be more negative than the max_reverse_abs_speed threshold
    * 2. Set the test object's reference point to be FRONT_LEFT
    * 3. Set the expected object's reference point to be REAR_RIGHT
    */

   test_object_track.speed = -calib.k_ctca_msmnt_update_max_reverse_abs_spd - 1.0F;
   test_object_track.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
   expected_object_track.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;

   /** \action
    * Call function.
    */
   Regularize_Trk_Hdg_Spd(calib,
                          test_object_track);

   /** \result
    * Expect the object's reference point to be REAR_RIGHT
    */
   CHECK_EQUAL_TEXT(expected_object_track.reference_point, test_object_track.reference_point, "The FRONT_LEFT reference point was not reversed as expected");
}

/** \purpose
 * Test that Regularize_Trk_Hdg_Spd correctly changes the reference point, when the current reference point is FRONT
 * \req
 * NA.
 */
TEST(f360_regularize_trk_hdg_spd__Regularize_Trk_Hdg_Spd, calculate_FRONT_refPoint_reverse)
{
   /** \precond
    * 1. Set the test object's speed to be more negative than the max_reverse_abs_speed threshold
    * 2. Set the test object's reference point to be FRONT
    * 3. Set the expected object's reference point to be REAR
    */

   test_object_track.speed = -calib.k_ctca_msmnt_update_max_reverse_abs_spd - 1.0F;
   test_object_track.reference_point = F360_REFERENCE_POINT_FRONT;
   expected_object_track.reference_point = F360_REFERENCE_POINT_REAR;

   /** \action
    * Call function.
    */
   Regularize_Trk_Hdg_Spd(calib,
                          test_object_track);

   /** \result
    * Expect the object's reference point to be REAR
    */
   CHECK_EQUAL_TEXT(expected_object_track.reference_point, test_object_track.reference_point, "The FRONT reference point was not reversed as expected");
}

/** \purpose
 * Test that Regularize_Trk_Hdg_Spd correctly changes the reference point, when the current reference point is FRONT_RIGHT
 * \req
 * NA.
 */
TEST(f360_regularize_trk_hdg_spd__Regularize_Trk_Hdg_Spd, calculate_FRONT_RIGHT_refPoint_reverse)
{
   /** \precond
    * 1. Set the test object's speed to be more negative than the max_reverse_abs_speed threshold
    * 2. Set the test object's reference point to be FRONT_RIGHT
    * 3. Set the expected object's reference point to be REAR_LEFT
    */

   test_object_track.speed = -calib.k_ctca_msmnt_update_max_reverse_abs_spd - 1.0F;
   test_object_track.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;
   expected_object_track.reference_point = F360_REFERENCE_POINT_REAR_LEFT;

   /** \action
    * Call function.
    */
   Regularize_Trk_Hdg_Spd(calib,
                          test_object_track);

   /** \result
    * Expect the object's reference point to be REAR_LEFT
    */
   CHECK_EQUAL_TEXT(expected_object_track.reference_point, test_object_track.reference_point, "The FRONT_RIGHT reference point was not reversed as expected");
}

/** \purpose
 * Test that Regularize_Trk_Hdg_Spd correctly changes the reference point, when the current reference point is RIGHT
 * \req
 * NA.
 */
TEST(f360_regularize_trk_hdg_spd__Regularize_Trk_Hdg_Spd, calculate_RIGHT_refPoint_reverse)
{
   /** \precond
    * 1. Set the test object's speed to be more negative than the max_reverse_abs_speed threshold
    * 2. Set the test object's reference point to be RIGHT
    * 3. Set the expected object's reference point to be LEFT
    */

   test_object_track.speed = -calib.k_ctca_msmnt_update_max_reverse_abs_spd - 1.0F;
   test_object_track.reference_point = F360_REFERENCE_POINT_RIGHT;
   expected_object_track.reference_point = F360_REFERENCE_POINT_LEFT;

   /** \action
    * Call function.
    */
   Regularize_Trk_Hdg_Spd(calib,
                          test_object_track);

   /** \result
    * Expect the object's reference point to be LEFT
    */
   CHECK_EQUAL_TEXT(expected_object_track.reference_point, test_object_track.reference_point, "The RIGHT reference point was not reversed as expected");
}

/** \purpose
 * Test that Regularize_Trk_Hdg_Spd correctly changes the reference point, when the current reference point is REAR_RIGHT
 * \req
 * NA.
 */
TEST(f360_regularize_trk_hdg_spd__Regularize_Trk_Hdg_Spd, calculate_REAR_RIGHT_refPoint_reverse)
{
   /** \precond
    * 1. Set the test object's speed to be more negative than the max_reverse_abs_speed threshold
    * 2. Set the test object's reference point to be REAR_RIGHT
    * 3. Set the expected object's reference point to be FRONT_LEFT
    */

   test_object_track.speed = -calib.k_ctca_msmnt_update_max_reverse_abs_spd - 1.0F;
   test_object_track.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
   expected_object_track.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;

   /** \action
    * Call function.
    */
   Regularize_Trk_Hdg_Spd(calib,
                          test_object_track);

   /** \result
    * Expect the object's reference point to be FRONT_LEFT
    */
   CHECK_EQUAL_TEXT(expected_object_track.reference_point, test_object_track.reference_point, "The REAR_RIGHT reference point was not reversed as expected");
}

/** \purpose
 * Test that Regularize_Trk_Hdg_Spd correctly changes the reference point, when the current reference point is REAR
 * \req
 * NA.
 */
TEST(f360_regularize_trk_hdg_spd__Regularize_Trk_Hdg_Spd, calculate_REAR_refPoint_reverse)
{
   /** \precond
    * 1. Set the test object's speed to be more negative than the max_reverse_abs_speed threshold
    * 2. Set the test object's reference point to be REAR
    * 3. Set the expected object's reference point to be FRONT
    */

   test_object_track.speed = -calib.k_ctca_msmnt_update_max_reverse_abs_spd - 1.0F;
   test_object_track.reference_point = F360_REFERENCE_POINT_REAR;
   expected_object_track.reference_point = F360_REFERENCE_POINT_FRONT;

   /** \action
    * Call function.
    */
   Regularize_Trk_Hdg_Spd(calib,
                          test_object_track);

   /** \result
    * Expect the object's reference point to be FRONT
    */
   CHECK_EQUAL_TEXT(expected_object_track.reference_point, test_object_track.reference_point, "The REAR reference point was not reversed as expected");
}

/** \purpose
 * Test that Regularize_Trk_Hdg_Spd correctly changes the reference point, when the current reference point is REAR_LEFT
 * \req
 * NA.
 */
TEST(f360_regularize_trk_hdg_spd__Regularize_Trk_Hdg_Spd, calculate_REAR_LEFT_refPoint_reverse)
{
   /** \precond
    * 1. Set the test object's speed to be more negative than the max_reverse_abs_speed threshold
    * 2. Set the test object's reference point to be REAR_LEFT
    * 3. Set the expected object's reference point to be FRONT_RIGHT
    */

   test_object_track.speed = -calib.k_ctca_msmnt_update_max_reverse_abs_spd - 1.0F;
   test_object_track.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   expected_object_track.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;

   /** \action
    * Call function.
    */
   Regularize_Trk_Hdg_Spd(calib,
                          test_object_track);

   /** \result
    * Expect the object's reference point to be FRONT_RIGHT
    */
   CHECK_EQUAL_TEXT(expected_object_track.reference_point, test_object_track.reference_point, "The REAR_LEFT reference point was not reversed as expected");
}

/** \purpose
 * Test that Regularize_Trk_Hdg_Spd correctly changes the reference point, when the current reference point is LEFT
 * \req
 * NA.
 */
TEST(f360_regularize_trk_hdg_spd__Regularize_Trk_Hdg_Spd, calculate_LEFT_refPoint_reverse)
{
   /** \precond
    * 1. Set the test object's speed to be more negative than the max_reverse_abs_speed threshold
    * 2. Set the test object's reference point to be LEFT
    * 3. Set the expected object's reference point to be RIGHT
    */

   test_object_track.speed = -calib.k_ctca_msmnt_update_max_reverse_abs_spd - 1.0F;
   test_object_track.reference_point = F360_REFERENCE_POINT_LEFT;
   expected_object_track.reference_point = F360_REFERENCE_POINT_RIGHT;

   /** \action
    * Call function.
    */
   Regularize_Trk_Hdg_Spd(calib,
                          test_object_track);

   /** \result
    * Expect the object's reference point to be RIGHT
    */
   CHECK_EQUAL_TEXT(expected_object_track.reference_point, test_object_track.reference_point, "The LEFT reference point was not reversed as expected");
}

/** \purpose
 * Test that Regularize_Trk_Hdg_Spd correctly changes the min projection reference point, when the current reference point is LEFT
 * \req
 * NA.
 */
TEST(f360_regularize_trk_hdg_spd__Regularize_Trk_Hdg_Spd, calculate_LEFT_minProjRefPoint_reverse)
{
   /** \precond
    * 1. Set the test object's speed to be more negative than the max_reverse_abs_speed threshold
    * 2. Set the test object's reference point to be LEFT
    * 3. Set the expected object's reference point to be RIGHT
    */
   test_object_track.speed = -calib.k_ctca_msmnt_update_max_reverse_abs_spd - 1.0F;
   test_object_track.min_projection_reference_point = F360_REFERENCE_POINT_LEFT;
   expected_object_track.min_projection_reference_point = F360_REFERENCE_POINT_RIGHT;

   /** \action
    * Call function.
    */
   Regularize_Trk_Hdg_Spd(calib,
                          test_object_track);

   /** \result
    * Expect the object's min projection reference point to be RIGHT
    */
   CHECK_EQUAL_TEXT(expected_object_track.min_projection_reference_point, test_object_track.min_projection_reference_point, "The LEFT reference point was not reversed as expected");
}

/** \purpose
 * Test that Regularize_Trk_Hdg_Spd does not change the reference point, when the current reference point is CENTER
 * \req
 * NA.
 */
TEST(f360_regularize_trk_hdg_spd__Regularize_Trk_Hdg_Spd, calculate_CENTER_refPoint_reverse)
{
   /** \precond
    * 1. Set the test object's speed to be more negative than the max_reverse_abs_speed threshold
    * 2. Set the test object's reference point to be CENTER
    * 3. Set the expected object's reference point to be CENTER
    */

   test_object_track.speed = -calib.k_ctca_msmnt_update_max_reverse_abs_spd - 1.0F;
   test_object_track.reference_point = F360_REFERENCE_POINT_CENTER;
   expected_object_track.reference_point = F360_REFERENCE_POINT_CENTER;

   /** \action
    * Call function.
    */
   Regularize_Trk_Hdg_Spd(calib,
                          test_object_track);

   /** \result
    * Expect the object's reference point to be CENTER
    */
   CHECK_EQUAL_TEXT(expected_object_track.reference_point, test_object_track.reference_point, "The CENTER reference point was not reversed as expected");
}

/** \purpose
 * Test that Regularize_Trk_Hdg_Spd does not change the reference point, when the reverse condition is not applicable
 * \req
 * NA.
 */
TEST(f360_regularize_trk_hdg_spd__Regularize_Trk_Hdg_Spd, check_refPoint_kept_same)
{
   /** \precond
    * 1. Set the test object's speed to be greater than the negative of max_reverse_abs_speed threshold
    * 2. Set the test object's reference point to be REAR
    * 3. Set the expected object's reference point to be REAR
    */

   test_object_track.speed = -calib.k_ctca_msmnt_update_max_reverse_abs_spd + 1.0F;
   test_object_track.reference_point = F360_REFERENCE_POINT_REAR;
   expected_object_track.reference_point = F360_REFERENCE_POINT_REAR;

   /** \action
    * Call function.
    */
   Regularize_Trk_Hdg_Spd(calib,
                          test_object_track);

   /** \result
    * Expect the object's reference point to be REAR
    */
   CHECK_EQUAL_TEXT(expected_object_track.reference_point, test_object_track.reference_point, "The reference point was flipped, when not expected to do so");
}

/** \purpose
 * Test that Regularize_Trk_Hdg_Spd does not change the min projection reference point, when the reverse condition is not applicable
 * \req
 * NA.
 */
TEST(f360_regularize_trk_hdg_spd__Regularize_Trk_Hdg_Spd, check_minProjRefPoint_kept_same)
{
   /** \precond
    * 1. Set the test object's speed to be greater than the negative of max_reverse_abs_speed threshold
    * 2. Set the test object's reference point to be REAR
    * 3. Set the expected object's reference point to be REAR
    */

   test_object_track.speed = -calib.k_ctca_msmnt_update_max_reverse_abs_spd + 1.0F;
   test_object_track.min_projection_reference_point = F360_REFERENCE_POINT_REAR;
   expected_object_track.min_projection_reference_point = F360_REFERENCE_POINT_REAR;

   /** \action
    * Call function.
    */
   Regularize_Trk_Hdg_Spd(calib,
                          test_object_track);

   /** \result
    * Expect the object's reference point to be REAR
    */
   CHECK_EQUAL_TEXT(expected_object_track.min_projection_reference_point, test_object_track.min_projection_reference_point, "The reference point was flipped, when not expected to do so");
}
/** @}*/
