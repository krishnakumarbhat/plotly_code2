/** \file
 * This file contains unit tests for testing requirements connected to f360_measurement_update_tracks.cpp file
 */

#include "f360_msmt_update_support_functions_common.h"
#include "f360_msmt_update_object_timestamp.h"
#include "f360_msmt_update_obj_trks_ctca.h"
#include "f360_msmt_update_obj_trks_cca_moveable.h"
#include "f360_msmt_update_obj_trks_cca_non_moveable.h"
#include "f360_pseudo_msmt.h"
#include "f360_math.h"
#include <string.h>
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_msmt_update_obj_trks_cca_non_moveable_qualtest
 *  @{
 */

/** \brief
 * Setup general values on object states and detection properties that is needed to perform
 * a measurement update.
 */
TEST_GROUP(f360_msmt_update_obj_trks_cca_non_moveable_qualtest)
{
   F360_Host_T host;
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Calibrations_T calib;
   F360_Object_Track_T object_track;
   F360_TRKR_TIMING_INFO_T timing_info;
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   uint32_t selected_dets_idx[MAX_DETS_IN_OBJ_TRK];
   uint32_t selected_dets_num;

   /** \setup
    * Setup general values for object and detection states.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      // Object states
      object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      object_track.vcs_position.x = 10.0F;
      object_track.vcs_velocity.longitudinal = 0.1F;
      object_track.vcs_position.y = 0.0F;
      object_track.vcs_velocity.lateral = -0.1F;

      object_track.pseudo_vcs_position.x = object_track.vcs_position.x - 0.1F;
      object_track.pseudo_vcs_position.y = object_track.vcs_position.y + 0.5F;

      object_track.ndets = 1;
      object_track.detids[0] = 1;

      object_track.movable_prob = 0.0F; // Msmt_Update_Obj_Trks_CCA_Non_Moveable() is only called for moveable objects. For non-moveable objects we call Msmt_Update_Obj_Trks_CCA_Moveable()

      object_track.vcs_heading.Value(0.5F); // Note heading and velocity values can be decoupled since object is non-moveable
      object_track.hdg_ptng_disagmt = 0.0F;

      object_track.reference_point = F360_REFERENCE_POINT_CENTER;
      object_track.bbox.Set_Center(object_track.vcs_position);
      object_track.bbox.Set_Orientation( object_track.vcs_heading.Value() + object_track.hdg_ptng_disagmt);

      // Arbitrary symmetric covariance matrix
      object_track.errcov[0][0] = 1.0F;
      object_track.errcov[0][1] = 0.5F;
      object_track.errcov[0][2] = -0.5F;
      object_track.errcov[0][3] = 0.1F;
      object_track.errcov[0][5] = 0.5F;

      object_track.errcov[1][0] = object_track.errcov[0][1];
      object_track.errcov[1][1] = 2.0F;
      object_track.errcov[1][2] = 0.3F;
      object_track.errcov[1][3] = -0.01F;

      object_track.errcov[2][0] = object_track.errcov[0][2];
      object_track.errcov[2][1] = object_track.errcov[1][2];
      object_track.errcov[2][2] = 1.3F;
      object_track.errcov[2][3] = -0.2F;

      object_track.errcov[3][0] = object_track.errcov[0][3];
      object_track.errcov[3][1] = object_track.errcov[1][3];
      object_track.errcov[3][2] = object_track.errcov[2][3];
      object_track.errcov[3][3] = 2.5F;

      object_track.errcov[4][0] = object_track.errcov[0][1];
      object_track.errcov[4][1] = 2.0F;
      object_track.errcov[4][2] = 0.3F;
      object_track.errcov[4][3] = -0.01F;
      object_track.errcov[4][4] = -0.01F;

      object_track.errcov[5][0] = object_track.errcov[0][5];
      object_track.errcov[5][3] = 2.0F;
      object_track.errcov[5][4] = 1.0F;
      object_track.errcov[5][5] = 0.01F;

      // Arbitrary symmetric pseudo measurement covariance matrix
      object_track.meascov[0][0] = 1.0F;
      object_track.meascov[0][1] = 0.23F;
      object_track.meascov[1][0] = object_track.meascov[0][1];
      object_track.meascov[1][1] = 1.2F;

      // Detections
      raw_detection_list.detections[0].processed.vcs_az = 0.0505F;
      raw_detection_list.detections[0].processed.cos_vcs_az = F360_Cosf(raw_detection_list.detections[0].processed.vcs_az);
      raw_detection_list.detections[0].processed.sin_vcs_az = F360_Sinf(raw_detection_list.detections[0].processed.vcs_az);

      det_props[0].vcs_position.x = 9.9F;
      det_props[0].vcs_position.y = 0.5F;

      det_props[0].range_rate_compensated = object_track.vcs_velocity.longitudinal + 0.1F;
   }

};

/** \purpose
 * Verify that object states are updated when 1 eligble detection is associated
 * \req
 * CPR-3738
 * CPR-3736
 * CPR-3739
 */
TEST(f360_msmt_update_obj_trks_cca_non_moveable_qualtest, Msmt_Update_Obj_Trks_CCA_Non_Moveable)
{
   /** \precond
    * Set number of selected detections for msmnt update to 1.
    * Set object to not moving
    * Copy states of object before function is called.
    */
   selected_dets_num = 1U;
   selected_dets_idx[0] = 0U;

   F360_Object_Track_T obj_before = object_track;

   /** \action
    * Call function
    */
   Msmt_Update_Obj_Trks_CCA_Non_Moveable(
      host,
      det_props,
      raw_detection_list,
      calib,
      selected_dets_idx,
      selected_dets_num,
      object_track,
      timing_info);

   /** \result
    * Verify that object heading and pointing haven't changed
    * Verify other states have changed.
    * Verify that object vcs_position and bbox center are still equal.
    */
   /** \result
    * Verify that object states have changed
    */
   float32_t delta_long_pos = fabsf(obj_before.vcs_position.x - object_track.vcs_position.x);
   float32_t delta_lat_pos = fabsf(obj_before.vcs_position.y - object_track.vcs_position.y);
   float32_t delta_long_vel = fabsf(obj_before.vcs_velocity.longitudinal - object_track.vcs_velocity.longitudinal);
   float32_t delta_lat_vel = fabsf(obj_before.vcs_velocity.lateral - object_track.vcs_velocity.lateral);
   float32_t delta_long_acc = fabsf(obj_before.vcs_accel.longitudinal - object_track.vcs_accel.longitudinal);
   float32_t delta_lat_acc = fabsf(obj_before.vcs_accel.lateral - object_track.vcs_accel.lateral);
   float32_t delta_head = fabsf(obj_before.vcs_heading.Value() - object_track.vcs_heading.Value());
   float32_t delta_cos_head = fabsf(obj_before.vcs_heading.Cos() - object_track.vcs_heading.Cos());
   float32_t delta_sin_head = fabsf(obj_before.vcs_heading.Sin() - object_track.vcs_heading.Sin());
   float32_t delta_point = fabsf(obj_before.bbox.Get_Orientation().Value() - object_track.bbox.Get_Orientation().Value());
   float32_t delta_cos_point = fabsf(obj_before.bbox.Get_Orientation().Cos() - object_track.bbox.Get_Orientation().Cos());
   float32_t delta_sin_point = fabsf(obj_before.bbox.Get_Orientation().Sin() - object_track.bbox.Get_Orientation().Sin());
   float32_t delta_heading_rate = fabsf(obj_before.heading_rate - object_track.heading_rate);

   CHECK_TRUE(delta_long_pos > F360_EPSILON);
   CHECK_TRUE(delta_lat_pos > F360_EPSILON);
   CHECK_TRUE(delta_long_vel > F360_EPSILON);
   CHECK_TRUE(delta_lat_vel > F360_EPSILON);
   CHECK_TRUE(delta_long_acc > F360_EPSILON);
   CHECK_TRUE(delta_lat_acc > F360_EPSILON);
   CHECK_TRUE(delta_head > F360_EPSILON);
   CHECK_TRUE(delta_cos_head > F360_EPSILON);
   CHECK_TRUE(delta_sin_head > F360_EPSILON);
   CHECK_TRUE(delta_point > F360_EPSILON);
   CHECK_TRUE(delta_cos_point > F360_EPSILON);
   CHECK_TRUE(delta_sin_point > F360_EPSILON);
   CHECK_TRUE(delta_heading_rate < F360_EPSILON);  //head rate not updated for NonMoveable CCA
   // Check that heading and pointing are equal
   const float32_t heading_pointing_diff = std::abs(object_track.vcs_heading.Value() - object_track.bbox.Get_Orientation().Value());
   CHECK_TRUE(heading_pointing_diff < F360_EPSILON);

   for (uint32_t row = 0U; row < STATE_DIMENSION; row++)
   {
      for (uint32_t col = 0U; col < STATE_DIMENSION; col++)
      {
         float32_t delta = fabsf(obj_before.errcov[row][col] - object_track.errcov[row][col]);
         CHECK_TRUE(delta > F360_EPSILON);
      }
   }
}
/** @}*/

/** \defgroup  f360_msmt_update_obj_trks_cca_moveable_qualtest
 *  @{
 */

/** \brief
 * Testing that a call to Msmt_Update_Obj_Trks_CCA() with one eligable detection is updating objects states
 */
TEST_GROUP(f360_msmt_update_obj_trks_cca_moveable_qualtest)
{
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Calibrations_T calib;
   F360_Object_Track_T object_track;
   F360_TRKR_TIMING_INFO_T timing_info;
   uint32_t selected_dets_idx[MAX_DETS_IN_OBJ_TRK];
   uint32_t selected_dets_num;
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};

   /** \setup
    * Setup general values for object and detection states.
    * Object:
    *    - filter type: CCA
    *    - vcs position: [10, 5] [m]
    *    - vcs velocity: [5, 0.1] [m/s]
    *    - vcs acceleration: [1.7, -0.6] [m/s^2
    *    - state error covariance: random covariance matrix, [6.516902025806293,   0.194512490773417,  -0.038455610730064,  -0.385033075453222,   0.128525832942101   0.093852564482656;
   *                                                          0.194512490773417,   6.663822593242870,   0.104355463057540,   0.166129709039925,   0.427907717373890,  -0.184096722846899;
   *                                                          -0.038455610730064,   0.104355463057540,   6.571751616753093,  -0.046312466759188,  -0.047463652995767,   0.405748620499433;
   *                                                          -0.385033075453222,   0.166129709039925,  -0.046312466759188,   6.948385410472763,  -0.072459134186868,  -0.409329684004222;
   *                                                          0.128525832942101,   0.427907717373890,  -0.047463652995767,  -0.072459134186868,   6.449236517036669,  -0.276834993063314;
   *                                                          0.093852564482656,  -0.184096722846899,   0.405748620499433,  -0.409329684004222,  -0.276834993063314,   6.957360618835150] is used in this test
    *    - pseudo vcs position: vcs_position + [0.1, -0.1] [m]
    *    - meascov: random covariance matrix, [1, 0.5; 0.5, 1] is used in the test
    *    - number of associated detections: 1
    *    - detection id of associated detection: 23
    *    - f_moving: true
    *    - object box orientation: 10deg
    *    - object heading rate: -0.1rad/s
    *    - object cca_pnt_filter_cov: random covariance matrix, [1, 0.5; 0.5, 2] is used in the test.
    * Setup selected detections for measurement update
    *    - the single detection with id 23 which is associated to the object
    * Setup one detection:
    *    - id 23
    *    - vcs azimuth: 5deg
    *    - cos vcs az: cos(5deg)
    *    - sin vcs az: sin(5deg)
    *    - vcs position: [10, 0.5] [m]
    *    - compensated range rate: 5.1m/s
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      // Object states
      object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      object_track.vcs_position.x = 10.0F;
      object_track.vcs_velocity.longitudinal = 5.0F;
      object_track.vcs_accel.longitudinal = 1.7F;
      object_track.vcs_position.y = 0.0F;
      object_track.vcs_velocity.lateral = 2.0F;
      object_track.vcs_accel.lateral = -0.6F;

      object_track.errcov[0][0] = 6.516902025806293F;
      object_track.errcov[0][1] = 0.194512490773417F;
      object_track.errcov[0][2] = -0.038455610730064F;
      object_track.errcov[0][3] = -0.385033075453222F;
      object_track.errcov[0][4] = 0.128525832942101F;
      object_track.errcov[0][5] = 0.093852564482656F;

      object_track.errcov[1][0] = object_track.errcov[0][1];
      object_track.errcov[1][1] = 6.663822593242870F;
      object_track.errcov[1][2] = 0.104355463057540F;
      object_track.errcov[1][3] = 0.166129709039925F;
      object_track.errcov[1][4] = 0.427907717373890F;
      object_track.errcov[1][5] = -0.184096722846899F;

      object_track.errcov[2][0] = object_track.errcov[0][2];
      object_track.errcov[2][1] = object_track.errcov[1][2];
      object_track.errcov[2][2] = 6.571751616753093F;
      object_track.errcov[2][3] = -0.046312466759188F;
      object_track.errcov[2][4] = -0.047463652995767F;
      object_track.errcov[2][5] = 0.405748620499433F;

      object_track.errcov[3][0] = object_track.errcov[0][3];
      object_track.errcov[3][1] = object_track.errcov[1][3];
      object_track.errcov[3][2] = object_track.errcov[2][3];
      object_track.errcov[3][3] = 6.948385410472763F;
      object_track.errcov[3][4] = -0.072459134186868F;
      object_track.errcov[3][5] = -0.409329684004222F;

      object_track.errcov[4][0] = object_track.errcov[0][4];
      object_track.errcov[4][1] = object_track.errcov[1][4];
      object_track.errcov[4][2] = object_track.errcov[2][4];
      object_track.errcov[4][3] = object_track.errcov[3][4];
      object_track.errcov[4][4] = 6.449236517036669F;
      object_track.errcov[4][5] = -0.276834993063314F;

      object_track.errcov[5][0] = object_track.errcov[0][5];
      object_track.errcov[5][1] = object_track.errcov[1][5];
      object_track.errcov[5][2] = object_track.errcov[2][5];
      object_track.errcov[5][3] = object_track.errcov[3][5];
      object_track.errcov[5][4] = object_track.errcov[4][5];
      object_track.errcov[5][5] = 6.957360618835150F;

      object_track.pseudo_vcs_position.x = object_track.vcs_position.x + 0.1F;
      object_track.pseudo_vcs_position.y = object_track.vcs_position.y - 0.1F;
      object_track.meascov[0][0] = 1.0F;
      object_track.meascov[0][1] = 0.5F;
      object_track.meascov[1][0] = object_track.meascov[0][1];
      object_track.meascov[1][1] = 1.0F;

      object_track.ndets = 1;
      object_track.detids[0] = 23;

      object_track.f_moving = true;
      object_track.num_updates_since_init = 11U;

      object_track.bbox.Set_Orientation(F360_DEG2RAD(10.0F));
      object_track.heading_rate = -0.1F;
      object_track.cca_pnt_filter_cov[0][0] = 1.0F;
      object_track.cca_pnt_filter_cov[0][1] = 0.5F;
      object_track.cca_pnt_filter_cov[1][0] = object_track.cca_pnt_filter_cov[0][1];
      object_track.cca_pnt_filter_cov[1][1] = 2.0F;

      // Selected detections for measurement update of object
      selected_dets_idx[0] = 22U;
      selected_dets_num = 1U;

      // The detection
      raw_detection_list.detections[22].processed.vcs_az = F360_DEG2RAD(5.0F);
      raw_detection_list.detections[22].processed.cos_vcs_az = F360_Cosf(raw_detection_list.detections[22].processed.vcs_az);
      raw_detection_list.detections[22].processed.sin_vcs_az = F360_Sinf(raw_detection_list.detections[22].processed.vcs_az);

      det_props[22].vcs_position.x = 10.0F;
      det_props[22].vcs_position.y = 0.5F;

      det_props[22].range_rate_compensated = 5.1F;
   }

};

/** \purpose
 * Verify that object states are updated when 1 eligble detection is associated to a CCA object and object is moving (i.e object bbox orientation should also be updated)
 * \req
 * CPR-3736
 */
TEST(f360_msmt_update_obj_trks_cca_moveable_qualtest, Msmt_Update_Obj_Trks_CCA_Moveable_Check_Update_One_Det_Obj_Moving)
{
   /** \precond
    * Use default test data from test group
    */

   /** \action
    * Copy the object before function call so that we can later make comparisions
    * Call function
    */
   F360_Object_Track_T obj_before = object_track;
   Msmt_Update_Obj_Trks_CCA_Moveable(
      det_props,
      raw_detection_list,
      calib,
      selected_dets_idx,
      selected_dets_num,
      object_track,
      timing_info);

   /** \result
    * Verify that object states have changed
    */
   float32_t delta_long_pos = fabsf(obj_before.vcs_position.x - object_track.vcs_position.x);
   float32_t delta_lat_pos = fabsf(obj_before.vcs_position.y - object_track.vcs_position.y);
   float32_t delta_long_vel = fabsf(obj_before.vcs_velocity.longitudinal - object_track.vcs_velocity.longitudinal);
   float32_t delta_lat_vel = fabsf(obj_before.vcs_velocity.lateral - object_track.vcs_velocity.lateral);
   float32_t delta_long_acc = fabsf(obj_before.vcs_accel.longitudinal - object_track.vcs_accel.longitudinal);
   float32_t delta_lat_acc = fabsf(obj_before.vcs_accel.lateral - object_track.vcs_accel.lateral);
   float32_t delta_head = fabsf(obj_before.vcs_heading.Value() - object_track.vcs_heading.Value());
   float32_t delta_cos_head = fabsf(obj_before.vcs_heading.Cos() - object_track.vcs_heading.Cos());
   float32_t delta_sin_head = fabsf(obj_before.vcs_heading.Sin() - object_track.vcs_heading.Sin());
   float32_t delta_point = fabsf(obj_before.bbox.Get_Orientation().Value() - object_track.bbox.Get_Orientation().Value());
   float32_t delta_cos_point = fabsf(obj_before.bbox.Get_Orientation().Cos() - object_track.bbox.Get_Orientation().Cos());
   float32_t delta_sin_point = fabsf(obj_before.bbox.Get_Orientation().Sin() - object_track.bbox.Get_Orientation().Sin());
   float32_t delta_heading_rate = fabsf(obj_before.heading_rate - object_track.heading_rate);

   CHECK_TRUE(delta_long_pos > F360_EPSILON);
   CHECK_TRUE(delta_lat_pos > F360_EPSILON);
   CHECK_TRUE(delta_long_vel > F360_EPSILON);
   CHECK_TRUE(delta_lat_vel > F360_EPSILON);
   CHECK_TRUE(delta_long_acc > F360_EPSILON);
   CHECK_TRUE(delta_lat_acc > F360_EPSILON);
   CHECK_TRUE(delta_head > F360_EPSILON);
   CHECK_TRUE(delta_cos_head > F360_EPSILON);
   CHECK_TRUE(delta_sin_head > F360_EPSILON);
   CHECK_TRUE(delta_point > F360_EPSILON);
   CHECK_TRUE(delta_cos_point > F360_EPSILON);
   CHECK_TRUE(delta_sin_point > F360_EPSILON);
   CHECK_TRUE(delta_heading_rate > F360_EPSILON);

   for (uint32_t row = 0U; row < STATE_DIMENSION; row++)
   {
      for (uint32_t col = 0U; col < STATE_DIMENSION; col++)
      {
         float32_t delta = fabsf(obj_before.errcov[row][col] - object_track.errcov[row][col]);
         CHECK_TRUE(delta > F360_EPSILON);
      }
   }
}

/** \purpose
 * Verify that object heading_rate and curvature is set to 0 when 1 eligble detection is associated to a CCA object and object is moving slow.
 * \req
 * CPR-3736
 */
TEST(f360_msmt_update_obj_trks_cca_moveable_qualtest, Msmt_Update_Obj_Trks_CCA_Moveable_Check_Update_One_Det_Obj_Moving_SpeedLow)
{
   /** \precond
    * calib.k_cca_min_speed_to_update_pnt = 10.0F is used this test
    */
   F360_Object_Track_T obj_before = object_track;
   calib.k_cca_min_speed_to_update_pnt = 10.0F;

   /** \action
    * Copy the object before function call so that we can later make comparisions
    * Call function
    */
   Msmt_Update_Obj_Trks_CCA_Moveable(
      det_props,
      raw_detection_list,
      calib,
      selected_dets_idx,
      selected_dets_num,
      object_track,
      timing_info);

   /** \result
    * Verify that object heading and pointing haven't changed and that heading rate has been set to 0
    * Verify other states have changed
    */
   DOUBLES_EQUAL(0.0F, object_track.heading_rate, F360_EPSILON);
   DOUBLES_EQUAL(0.0F, object_track.curvature, F360_EPSILON);
   DOUBLES_EQUAL(obj_before.bbox.Get_Orientation().Value(), object_track.bbox.Get_Orientation().Value(), F360_EPSILON);
   DOUBLES_EQUAL(obj_before.bbox.Get_Orientation().Cos(), object_track.bbox.Get_Orientation().Cos(), F360_EPSILON);
   DOUBLES_EQUAL(obj_before.bbox.Get_Orientation().Sin(), object_track.bbox.Get_Orientation().Sin(), F360_EPSILON);
   DOUBLES_EQUAL(0.0F, object_track.heading_rate, F360_EPSILON);

   float32_t delta_long_pos = fabsf(obj_before.vcs_position.x - object_track.vcs_position.x);
   float32_t delta_lat_pos = fabsf(obj_before.vcs_position.y - object_track.vcs_position.y);
   float32_t delta_long_vel = fabsf(obj_before.vcs_velocity.longitudinal - object_track.vcs_velocity.longitudinal);
   float32_t delta_lat_vel = fabsf(obj_before.vcs_velocity.lateral - object_track.vcs_velocity.lateral);
   float32_t delta_long_acc = fabsf(obj_before.vcs_accel.longitudinal - object_track.vcs_accel.longitudinal);
   float32_t delta_lat_acc = fabsf(obj_before.vcs_accel.lateral - object_track.vcs_accel.lateral);

   CHECK_TRUE(delta_long_pos > F360_EPSILON);
   CHECK_TRUE(delta_lat_pos > F360_EPSILON);
   CHECK_TRUE(delta_long_vel > F360_EPSILON);
   CHECK_TRUE(delta_lat_vel > F360_EPSILON);
   CHECK_TRUE(delta_long_acc > F360_EPSILON);
   CHECK_TRUE(delta_lat_acc > F360_EPSILON);
}

/** \purpose
 * Verify that object states are updated when 1 eligble detection is associated to a CCA object and object is not moving (i.e object bbox orientation should not also be updated)
 * \req
 * CPR-3738
 * CPR-3736
 */
TEST(f360_msmt_update_obj_trks_cca_moveable_qualtest, Msmt_Update_Obj_Trks_CCA_Moveable_Check_Head_Point_Not_Updated_Due_To_Not_Moving)
{
   /** \precond
    * Use default test data from test group except for:
    *    - set f_moving to false
    */
   object_track.f_moving = false;

   /** \action
    * Copy the object before function call so that we can later make comparisions
    * Call function
    */
   F360_Object_Track_T obj_before = object_track;
   Msmt_Update_Obj_Trks_CCA_Moveable(
      det_props,
      raw_detection_list,
      calib,
      selected_dets_idx,
      selected_dets_num,
      object_track,
      timing_info);

   /** \result
    * Verify that object heading and pointing haven't changed and that heading rate has been set to 0
    * Verify other states have changed
    */
   DOUBLES_EQUAL(obj_before.bbox.Get_Orientation().Value(), object_track.bbox.Get_Orientation().Value(), F360_EPSILON);
   DOUBLES_EQUAL(obj_before.bbox.Get_Orientation().Cos(), object_track.bbox.Get_Orientation().Cos(), F360_EPSILON);
   DOUBLES_EQUAL(obj_before.bbox.Get_Orientation().Sin(), object_track.bbox.Get_Orientation().Sin(), F360_EPSILON);
   DOUBLES_EQUAL(0.0F, object_track.heading_rate, F360_EPSILON);

   float32_t delta_long_pos = fabsf(obj_before.vcs_position.x - object_track.vcs_position.x);
   float32_t delta_lat_pos = fabsf(obj_before.vcs_position.y - object_track.vcs_position.y);
   float32_t delta_long_vel = fabsf(obj_before.vcs_velocity.longitudinal - object_track.vcs_velocity.longitudinal);
   float32_t delta_lat_vel = fabsf(obj_before.vcs_velocity.lateral - object_track.vcs_velocity.lateral);
   float32_t delta_long_acc = fabsf(obj_before.vcs_accel.longitudinal - object_track.vcs_accel.longitudinal);
   float32_t delta_lat_acc = fabsf(obj_before.vcs_accel.lateral - object_track.vcs_accel.lateral);

   CHECK_TRUE(delta_long_pos > F360_EPSILON);
   CHECK_TRUE(delta_lat_pos > F360_EPSILON);
   CHECK_TRUE(delta_long_vel > F360_EPSILON);
   CHECK_TRUE(delta_lat_vel > F360_EPSILON);
   CHECK_TRUE(delta_long_acc > F360_EPSILON);
   CHECK_TRUE(delta_lat_acc > F360_EPSILON);
}
/** @}*/


/** \defgroup  f360_msmt_update_obj_trks_ctca_qualtest
 *  @{
 */

/** \brief
  * Setup general values on object states and detection properties that is needed to perform
  * a measurement update.
  */
TEST_GROUP(f360_msmt_update_obj_trks_ctca_qualtest)
{
   F360_Host_T host;
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Calibrations_T calib;
   F360_Object_Track_T object_track;
   F360_TRKR_TIMING_INFO_T timing_info;
   uint32_t selected_dets_idx[MAX_DETS_IN_OBJ_TRK];
   uint32_t selected_dets_num;
   uint32_t selected_dets_score[MSMT_UPDATE_MAX_NUM_OF_MSMT]{};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};

   /** \setup
    * Setup general values for object and detection states.
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);


      // Object
      object_track.reference_point = F360_REFERENCE_POINT_LEFT;
      object_track.vcs_position.x = 10.0F;
      object_track.vcs_position.y = 0.0F;
      object_track.vcs_heading = Angle{ F360_DEG2RAD(2.0F) };
      object_track.hdg_ptng_disagmt = 0.0F;
      object_track.bbox.Set_Orientation(object_track.vcs_heading);
      object_track.bbox.Set_Length(6.0F);
      object_track.bbox.Set_Width(2.5F);
      object_track.Update_Bbox_Center();
      object_track.speed = 5.0F;
      object_track.curvature = 0.01F;
      object_track.tang_accel = 0.1F;

      object_track.pseudo_vcs_position.x = object_track.vcs_position.x + 1.0F;
      object_track.pseudo_vcs_position.y = object_track.vcs_position.y;

      object_track.f_moving = true;
      object_track.movable_prob = 1.0F;

      object_track.ndets = 1;
      object_track.detids[0] = 1U;

      // Arbitrary symmetric covariance matrix
      for (int row = 0; row < 6; row++)
      {
         for (int col = 0; col < 6; col++)
         {
            object_track.errcov[row][col] = 1.0F;
         }
      }

      // Arbitrary symmetric pseudo covariance matrix
      for (int row = 0; row < 3; row++)
      {
         object_track.meascov[row][row] = 1.0F;
      }

      // Detections
      raw_detection_list.detections[0].processed.vcs_az = F360_DEG2RAD(5.0F);
      raw_detection_list.detections[0].processed.cos_vcs_az = F360_Cosf(raw_detection_list.detections[0].processed.vcs_az);
      raw_detection_list.detections[0].processed.sin_vcs_az = F360_Sinf(raw_detection_list.detections[0].processed.vcs_az);

      det_props[0].vcs_position.x = 10.0F;
      det_props[0].vcs_position.y = 0.5F;

      det_props[0].range_rate_compensated = object_track.speed + 1.0F;

      std::fill(cmn::begin(selected_dets_score), cmn::end(selected_dets_score), 1U);
   }
};

/** \purpose
 * Verify object that states are updated when 1 eligble detection is associated
 * \req
 * CPR-3736, CPR-3740
 */
TEST(f360_msmt_update_obj_trks_ctca_qualtest, Msmt_Update_Obj_Trks_CTCA_Check_Update_One_Det)
{
   /** \precond
    * Set one detection as eligble for measurement update
    * Create copy of object before call to measurement update
    */
   selected_dets_num = 1U;
   selected_dets_idx[0] = 0U;
   F360_Object_Track_T copy_obj = object_track;

   /** \action
    * Call function
    */
   Msmt_Update_ObjTrks_CTCA(
      det_props,
      raw_detection_list,
      calib,
      selected_dets_idx,
      selected_dets_num,
      selected_dets_score,
      object_track,
      timing_info);

   /** \result
    * Verify that object was measurement updated, i.e. that object states and covariance matrix differ after update
    */
   float32_t delta_long_pos = fabsf(copy_obj.vcs_position.x - object_track.vcs_position.x);
   float32_t delta_lat_pos = fabsf(copy_obj.vcs_position.y - object_track.vcs_position.y);
   float32_t delta_head = fabsf(copy_obj.vcs_heading.Value() - object_track.vcs_heading.Value());
   float32_t delta_pnt = fabsf(copy_obj.bbox.Get_Orientation().Value() - object_track.bbox.Get_Orientation().Value());
   float32_t delta_curv = fabsf(copy_obj.curvature - object_track.curvature);
   float32_t delta_speed = fabsf(copy_obj.speed - object_track.speed);
   float32_t delta_acc = fabsf(copy_obj.tang_accel - object_track.tang_accel);

   CHECK_TRUE(delta_long_pos > F360_EPSILON);
   CHECK_TRUE(delta_lat_pos > F360_EPSILON);
   CHECK_TRUE(delta_head > F360_EPSILON);
   CHECK_TRUE(delta_pnt > F360_EPSILON);
   CHECK_TRUE(delta_curv > F360_EPSILON);
   CHECK_TRUE(delta_speed > F360_EPSILON);
   CHECK_TRUE(delta_acc > F360_EPSILON);

   for (uint32_t row = 0U; row < STATE_DIMENSION; row++)
   {
      for (uint32_t col = 0U; col < STATE_DIMENSION; col++)
      {
         float32_t delta = fabsf(copy_obj.errcov[row][col] - object_track.errcov[row][col]);
         CHECK_TRUE(delta > F360_EPSILON);
      }
   }
}

/** @}*/

/** \defgroup  f360_msmt_update_obj_trks_ctca_not_moving_qualtest
 *  @{
 */

 /** \brief
  * Test group for tests related to freezing of heading and curvature for CTCA tracks
  */
TEST_GROUP(f360_msmt_update_obj_trks_ctca_not_moving_qualtest)
{
   F360_Host_T host{};
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS]{};
   F360_Calibrations_T calib{};
   F360_Object_Track_T object_track{};
   F360_TRKR_TIMING_INFO_T timing_info{};
   uint32_t selected_dets_idx[MAX_DETS_IN_OBJ_TRK]{};
   uint32_t selected_dets_num{};
   uint32_t selected_dets_score[MSMT_UPDATE_MAX_NUM_OF_MSMT]{};
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};

   double threshold = 1e-4;

   /** \setup
    * Initialize tracker calibrations
    * Set up track parameters to be placed on left side of host, close to y vcs axis.
    * Set up parameters of two detections to make valid measurment
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);

      object_track.reference_point = F360_REFERENCE_POINT_LEFT;
      object_track.vcs_position.x = 0.0F;
      object_track.vcs_position.y = -2.0F;
      object_track.vcs_heading = Angle{ 1.57F };
      object_track.hdg_ptng_disagmt = 0.0F;
      object_track.bbox.Set_Orientation(object_track.vcs_heading);
      object_track.bbox.Set_Length(6.0F);
      object_track.bbox.Set_Width(2.5F);
      object_track.Update_Bbox_Center();
      object_track.curvature = 0.0F;

      object_track.ndets = 2;

      for (int i = 0; i < STATE_DIMENSION; i++)
      {
         for (int j = 0; j < STATE_DIMENSION; j++)
         {
            object_track.errcov[i][j] = 0.01F;
         }
      }

      object_track.meascov[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_X] = 0.5F;
      object_track.meascov[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_Y] = 0.5F;
      object_track.meascov[F360_PSEUDO_MSMT_POS_X][F360_PSEUDO_MSMT_POS_Y] = 0.0F;
      object_track.meascov[F360_PSEUDO_MSMT_POS_Y][F360_PSEUDO_MSMT_POS_X] = 0.0F;

      selected_dets_num = 2;
      selected_dets_idx[0] = 0;
      selected_dets_idx[1] = 1;

      raw_detection_list.detections[0].processed.cos_vcs_az = 0.0F;
      raw_detection_list.detections[0].processed.sin_vcs_az = -1.0F;
      det_props[0].vcs_position.x = 0.1F;
      det_props[0].vcs_position.y = -2.0F;
      det_props[0].range_rate_compensated = 5.0F;

      raw_detection_list.detections[1].processed.cos_vcs_az = 0.01F;
      raw_detection_list.detections[1].processed.sin_vcs_az = -0.99F;
      det_props[1].vcs_position.x = -0.1F;
      det_props[1].vcs_position.y = -1.9F;
      det_props[1].range_rate_compensated = 2.5F;

      std::fill(cmn::begin(selected_dets_score), cmn::end(selected_dets_score), 1U);
   }
};

/** \purpose
* Purpose of this test is to verify that when object is moveable but its speed is too low its
* curvature and heading is not updated
* \req
* CPR-3737
* CPR-3736
* CPR-3740
*/
TEST(f360_msmt_update_obj_trks_ctca_not_moving_qualtest, Msmt_Update_ObjTrks_CTCA__curvature_head_is_not_updated_if_movable_and_speed_too_low)
{
   /** \precond
   * Set track as movable
   * Set its speed below threshold
   * Initialize track to make measurement update step return valid numbers
   */
   object_track.f_moving = false;
   object_track.movable_prob = 1.0F;
   object_track.speed = 1.99F; // Slightly smaller than k_min_speed_for_updating_heading

   F360_Object_Track_T copy_obj = object_track;

   /** \action
   * Call tested function.
   */
   Msmt_Update_ObjTrks_CTCA(det_props, raw_detection_list, calib, selected_dets_idx, selected_dets_num, selected_dets_score, object_track, timing_info);

   /** \result
   * Check that curvature, heading and pointing of track was not changed.
   * Check that all other states were updated
   */
   DOUBLES_EQUAL(copy_obj.vcs_heading.Value(), object_track.vcs_heading.Value(), threshold);
   DOUBLES_EQUAL(copy_obj.bbox.Get_Orientation().Value(), object_track.bbox.Get_Orientation().Value(), threshold);
   DOUBLES_EQUAL(copy_obj.curvature, object_track.curvature, threshold);

   float32_t delta_long_pos = fabsf(copy_obj.vcs_position.x - object_track.vcs_position.x);
   float32_t delta_lat_pos = fabsf(copy_obj.vcs_position.y - object_track.vcs_position.y);
   float32_t delta_speed = fabsf(copy_obj.speed - object_track.speed);
   float32_t delta_acc = fabsf(copy_obj.tang_accel - object_track.tang_accel);

   CHECK_TRUE(delta_long_pos > F360_EPSILON);
   CHECK_TRUE(delta_lat_pos > F360_EPSILON);
   CHECK_TRUE(delta_speed > F360_EPSILON);
   CHECK_TRUE(delta_acc > F360_EPSILON);
}
/** @}*/

/** \defgroup  f360_msmt_update_object_timestamp_qualtest
 *  @{
 */

/** \brief
 * Create needed default variables needed for update of object measurement timestamp
 */
TEST_GROUP(f360_msmt_update_object_timestamp_qualtest)
{
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Calibrations_T calibs;
   F360_Object_Track_T obj;
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};

   /** \setup
    * Setup default values for easy testing between multiple test cases
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      raw_detection_list.detections[0].raw.sensor_id = 1;
      sensors[0].refined.time_since_measurement_s = 1.0F;

      det_props[0].f_rr_inlier = true;
      det_props[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;

      obj.ndets = 1;
      obj.detids[0] = 1U;
   }
};

/** \purpose
 * Verify that object measurement timestamp is equal to detection timestamp when
 * only one detection is used for update.
 * \req
 * CPR-3741
 */
TEST(f360_msmt_update_object_timestamp_qualtest, Msmt_Update_Object_Timestamp_One_Det)
{
   float32_t elapsed_time = 0.05F;

   /** \action
    * Call function
    */
   Msmt_Update_Object_Timestamp(elapsed_time, sensors, raw_detection_list, det_props, obj);


   /** \result
    * Check that object timestamp is equal to the only detection used in measurement update
    */
   DOUBLES_EQUAL(sensors[0].refined.time_since_measurement_s, obj.time_since_measurement, F360_EPSILON);
}
/** @}*/
