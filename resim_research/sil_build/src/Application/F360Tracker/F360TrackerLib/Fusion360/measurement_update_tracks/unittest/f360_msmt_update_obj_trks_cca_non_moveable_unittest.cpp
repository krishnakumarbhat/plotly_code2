/** \file
 * This file contains unit tests for content of f360_msmt_update_obj_trks_cca_non_moveable.cpp file
 */

#include "f360_msmt_update_obj_trks_cca_non_moveable.h"
#include <CppUTest/TestHarness.h>

//#include "headerfile_needed.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_msmt_update_obj_trks_cca_non_moveable
 *  @{
 */

/** \brief
 * This test group is for testing the function Msmt_Update_Obj_Trks_CCA_Non_Moveable() which does a KF measurement updaate of a CCA object.
 */
TEST_GROUP(f360_msmt_update_obj_trks_cca_non_moveable)
{
   // Declare common variables used within all tests in this test group.
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list;
   F360_Calibrations_T calib;
   uint32_t selected_dets_idx[MAX_DETS_IN_OBJ_TRK];
   uint32_t selected_dets_num;
   F360_Object_Track_T object_track;
   F360_TRKR_TIMING_INFO_T timing_info;

   const float32_t test_pass_th = 1e-5F;
   F360_Host_T host;

/** \setup
    * Setup general values for object and detections that are used in the default test case.
   *    Calibrations
   *       - Default tracker calibrations
   *    Object:
   *       - trk_flitr_type: CCA
   *       - vcs_position: [10, 0] [m]
   *       - vcs_velocity: [5, 2] [m/s]
   *       - speed: length of the vcs_velocity vector
   *       - vcs_heading: direction of vcs_velocity
   *       - vcs_accel: [0.1, 0.1] [m/s^2]
   *       - tang_accel: scalar product of vcs_accel with unit vector in the same direction as vcs_velocity
   *       - bbox orientation: vcs_heading + 5deg
   *       - yaw_rate: -0.1F [rad/s]
   *       - curvature: yaw_rate / speed
   *       - errcov: random covariance matrix [6.516902025806293,   0.194512490773417,  -0.038455610730064,  -0.385033075453222,   0.128525832942101   0.093852564482656;
   *                                           0.194512490773417,   6.663822593242870,   0.104355463057540,   0.166129709039925,   0.427907717373890,  -0.184096722846899;
   *                                          -0.038455610730064,   0.104355463057540,   6.571751616753093,  -0.046312466759188,  -0.047463652995767,   0.405748620499433;
   *                                          -0.385033075453222,   0.166129709039925,  -0.046312466759188,   6.948385410472763,  -0.072459134186868,  -0.409329684004222;
   *                                           0.128525832942101,   0.427907717373890,  -0.047463652995767,  -0.072459134186868,   6.449236517036669,  -0.276834993063314;
   *                                           0.093852564482656,  -0.184096722846899,   0.405748620499433,  -0.409329684004222,  -0.276834993063314,   6.957360618835150]
   *       - cca_pnt_filter_cov: random covariance matrix [2.696766889080332, 1.305369243191352; 1.305369243191352 1.395963987598590]
   *       - pseudo_vcs_position: vcs_position + [0.1, 0.1] [m] (i.e object position + some noise)
   *       - meascov: random covariance matrix: [0.628373671485667, 0.386109725683730; 0.386109725683730,  0.246138936434065]
   *       - num_updates_since_init: a large value (such that the kalman gain is not modified), 255
   *       - f_moving: false
   *       - ndets: 3
   *    Detection 1:
   *       - idx: 3
   *       - vcs_az: 0deg
   *       - r_compensated: predicted range rate + 0.1
   *    Detection 2:
   *       - idx: 17
   *       - vcs_az: 10deg
   *       - r_compensated: predicted range rate
   *    Detection 3:
   *       - idx: 55
   *       - vcs_az: 5deg
   *       - r_compensated: predicted range rate - 0.1
   *    Selected detections
   *      - all three of the above measurements
   */
   TEST_SETUP()
   {
      // Calibrations
      Initialize_Tracker_Calibrations(calib);

      // Object
      object_track.trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;

      object_track.vcs_position.x = 10.0F;
      object_track.vcs_position.y = 0.0F;

      object_track.vcs_velocity.longitudinal = 0.9F;
      object_track.vcs_velocity.lateral = 2.0F;
      object_track.speed = F360_Get_Hypotenuse(object_track.vcs_velocity.longitudinal, object_track.vcs_velocity.lateral);
      (void)object_track.vcs_heading.Value(F360_Atan2f(object_track.vcs_velocity.lateral, object_track.vcs_velocity.longitudinal)).Normalize();

      object_track.vcs_accel.longitudinal = 0.1F;
      object_track.vcs_accel.lateral = 0.1F;
      object_track.tang_accel = object_track.vcs_accel.longitudinal * object_track.vcs_heading.Cos() + object_track.vcs_accel.lateral * object_track.vcs_heading.Sin();

      object_track.bbox.Set_Orientation(object_track.vcs_heading.Value() + F360_DEG2RAD(5.0F));
      object_track.heading_rate = -0.1F;
      object_track.curvature = object_track.heading_rate / object_track.speed;

      object_track.errcov[0][0] = 6.5169020F;
      object_track.errcov[0][1] = 0.1945125F;
      object_track.errcov[0][2] = -0.0384556F;
      object_track.errcov[0][3] = -0.3850331F;
      object_track.errcov[0][4] = 0.1285258F;
      object_track.errcov[0][5] = 0.0938526F;

      object_track.errcov[1][0] = 0.1945125F;
      object_track.errcov[1][1] = 6.6638226F;
      object_track.errcov[1][2] = 0.1043555F;
      object_track.errcov[1][3] = 0.1661297F;
      object_track.errcov[1][4] = 0.4279077F;
      object_track.errcov[1][5] = -0.1840967F;

      object_track.errcov[2][0] = -0.0384556F;
      object_track.errcov[2][1] = 0.1043555F;
      object_track.errcov[2][2] = 6.5717516F;
      object_track.errcov[2][3] = -0.0463125F;
      object_track.errcov[2][4] = -0.0474637F;
      object_track.errcov[2][5] = 0.4057486F;

      object_track.errcov[3][0] = -0.3850331F;
      object_track.errcov[3][1] = 0.1661297F;
      object_track.errcov[3][2] = -0.0463125F;
      object_track.errcov[3][3] = 6.9483854F;
      object_track.errcov[3][4] = -0.0724591F;
      object_track.errcov[3][5] = -0.4093297F;

      object_track.errcov[4][0] = 0.1285258F;
      object_track.errcov[4][1] = 0.4279077F;
      object_track.errcov[4][2] = -0.0474637F;
      object_track.errcov[4][3] = -0.0724591F;
      object_track.errcov[4][4] = 6.4492365F;
      object_track.errcov[4][5] = -0.2768350F;

      object_track.errcov[5][0] = 0.0938526F;
      object_track.errcov[5][1] = -0.1840967F;
      object_track.errcov[5][2] = 0.4057486F;
      object_track.errcov[5][3] = -0.4093297F;
      object_track.errcov[5][4] = -0.2768350F;
      object_track.errcov[5][5] = 6.9573606F;

      object_track.cca_pnt_filter_cov[0][0] = 2.6967668F;
      object_track.cca_pnt_filter_cov[0][1] = 1.3053692F;
      object_track.cca_pnt_filter_cov[1][0] = object_track.cca_pnt_filter_cov[0][1];
      object_track.cca_pnt_filter_cov[0][0] = 1.3959639F;

      object_track.pseudo_vcs_position.x = object_track.vcs_position.x + 0.1F;
      object_track.pseudo_vcs_position.y = object_track.vcs_position.y + 0.1F;

      object_track.meascov[0][0] = 0.4839733F;
      object_track.meascov[0][1] = 0.0348570F;
      object_track.meascov[1][0] = object_track.meascov[0][1];
      object_track.meascov[1][1] = 0.1017386F;

      object_track.num_updates_since_init = 255U;

      object_track.f_moving = false;

      object_track.ndets = 3U;

      // Detection 1
      uint32_t idx = 3;
      raw_detection_list.detections[idx].processed.vcs_az = F360_DEG2RAD(10.0F);
      raw_detection_list.detections[idx].processed.cos_vcs_az = F360_Cosf(raw_detection_list.detections[idx].processed.vcs_az);
      raw_detection_list.detections[idx].processed.sin_vcs_az = F360_Sinf(raw_detection_list.detections[idx].processed.vcs_az);
      det_props[idx].range_rate_compensated = object_track.vcs_velocity.longitudinal * raw_detection_list.detections[idx].processed.cos_vcs_az + object_track.vcs_velocity.lateral * raw_detection_list.detections[idx].processed.sin_vcs_az  + 0.1F;

      // Detection 2
      idx = 17;
      raw_detection_list.detections[idx].processed.vcs_az = F360_DEG2RAD(0.0F);
      raw_detection_list.detections[idx].processed.cos_vcs_az = F360_Cosf(raw_detection_list.detections[idx].processed.vcs_az);
      raw_detection_list.detections[idx].processed.sin_vcs_az = F360_Sinf(raw_detection_list.detections[idx].processed.vcs_az);
      det_props[idx].range_rate_compensated = object_track.vcs_velocity.longitudinal * raw_detection_list.detections[idx].processed.cos_vcs_az + object_track.vcs_velocity.lateral * raw_detection_list.detections[idx].processed.sin_vcs_az;

      // Detection 3
      idx = 55;
      raw_detection_list.detections[idx].processed.vcs_az = F360_DEG2RAD(5.0F);
      raw_detection_list.detections[idx].processed.cos_vcs_az = F360_Cosf(raw_detection_list.detections[idx].processed.vcs_az);
      raw_detection_list.detections[idx].processed.sin_vcs_az = F360_Sinf(raw_detection_list.detections[idx].processed.vcs_az);
      det_props[idx].range_rate_compensated = object_track.vcs_velocity.longitudinal * raw_detection_list.detections[idx].processed.cos_vcs_az + object_track.vcs_velocity.lateral * raw_detection_list.detections[idx].processed.sin_vcs_az - 0.1F;

      // Selected detections
      selected_dets_idx[0] = 3;
      selected_dets_idx[1] = 17;
      selected_dets_idx[2] = 55;
      selected_dets_num = 3U;

   }
};


/** \purpose
 * Verify that no measurement update is done when there are no associated eligable detections.
 * \req
 * NA
 */
TEST(f360_msmt_update_obj_trks_cca_non_moveable, Msmt_Update_Obj_Trks_CCA_No_Dets)
{
   /** \precond
    * Use default setup form test group except for:
    *    - Set number of selected detections for msmnt update to zero.
    * Copy object before call to function to be alble to compare afterwards that object is unchanged.
    */
   selected_dets_num = 0U;
   const F360_Object_Track_T copy_obj = object_track;

   /** \action
    * Call function Msmt_Update_Obj_Trks_CCA_Moveable()
    */
   Msmt_Update_Obj_Trks_CCA_Non_Moveable(host, det_props, raw_detection_list, calib, selected_dets_idx, selected_dets_num, object_track, timing_info);

   /** \result
    * Verify that object states haven't changed
    */
   DOUBLES_EQUAL(copy_obj.vcs_position.x, object_track.vcs_position.x, F360_EPSILON);
   DOUBLES_EQUAL(copy_obj.vcs_velocity.longitudinal, object_track.vcs_velocity.longitudinal, F360_EPSILON);
   DOUBLES_EQUAL(copy_obj.vcs_accel.longitudinal, object_track.vcs_accel.longitudinal, F360_EPSILON);
   DOUBLES_EQUAL(copy_obj.vcs_position.y, object_track.vcs_position.y, F360_EPSILON);
   DOUBLES_EQUAL(copy_obj.vcs_velocity.lateral, object_track.vcs_velocity.lateral, F360_EPSILON);
   DOUBLES_EQUAL(copy_obj.vcs_accel.lateral, object_track.vcs_accel.lateral, F360_EPSILON);
   for(uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx ++)
   {
      for(uint32_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx ++)
      {
         DOUBLES_EQUAL(copy_obj.errcov[row_idx][col_idx], object_track.errcov[row_idx][col_idx], F360_EPSILON);
      }
   }
}


/** \purpose
 * Verify that object states are updated correctly when there are associated eligable detections and object is not suspected moveable
 * \req
 * NA.
 */
TEST(f360_msmt_update_obj_trks_cca_non_moveable, Msmt_Update_Obj_Trks_CCA_Not_Suspected_Moveable)
{
   /** \precond
    * Use default setup form test group except for:
    *    - Set number of selected detections for msmnt update to 1.
    */
   selected_dets_num = 1U;

   float32_t exp_errcov[STATE_DIMENSION][STATE_DIMENSION] = {
      {0.4499861, 0.0010181, -0.0032667, 0.0315709, 0.005101325, 0.0035574},
      {0.0010181, 0.9458328, 0.02320338, 0.0005891, -0.8996135, 0.0152279},
      {-0.0032667, 0.02320338, 6.570058, -0.0009446, -0.0665535, 0.4065465},
      {0.0315709, 0.0005891, -0.0009446, 0.1000408, -0.0010207, -0.0053870},
      {0.0051013, -0.8996135, -0.0665535, -0.0010207, 6.138706, -0.2370066},
      {0.0035574, 0.0152279, 0.4065465, -0.0053870, -0.2370066, 6.929048}
   };

   float32_t exp_vcs_position_x = 10.0924034F;
   float32_t exp_vcs_vel_long = 0.9865988F;
   float32_t exp_vcs_acc_long =  0.1003662F;
   float32_t exp_vcs_position_y = 0.0979993F;
   float32_t exp_vcs_vel_lat = 2.01984F;
   float32_t exp_vcs_acc_lat = 0.09407402F;

   /** \action
    * Call function Msmt_Update_Obj_Trks_CCA_Moveable()
    */
   Msmt_Update_Obj_Trks_CCA_Non_Moveable(host, det_props, raw_detection_list, calib, selected_dets_idx, selected_dets_num, object_track, timing_info);

   /** \result
    * Verify that object states have changed
    */
   DOUBLES_EQUAL(exp_vcs_position_x, object_track.vcs_position.x, test_pass_th);
   DOUBLES_EQUAL(exp_vcs_vel_long, object_track.vcs_velocity.longitudinal, test_pass_th);
   DOUBLES_EQUAL(exp_vcs_acc_long, object_track.vcs_accel.longitudinal, test_pass_th);
   DOUBLES_EQUAL(exp_vcs_position_y, object_track.vcs_position.y, test_pass_th);
   DOUBLES_EQUAL(exp_vcs_vel_lat, object_track.vcs_velocity.lateral, test_pass_th);
   DOUBLES_EQUAL(exp_vcs_acc_lat, object_track.vcs_accel.lateral, test_pass_th);
   for(uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx ++)
   {
      for(uint32_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx ++)
      {
         DOUBLES_EQUAL(exp_errcov[row_idx][col_idx], object_track.errcov[row_idx][col_idx], test_pass_th);
      }
   }
}

/** \purpose
 * Verify that object states are updated correctly when there are associated eligable detections and object is suspected moveable due to moving detections
 * \req
 * NA.
 */
TEST(f360_msmt_update_obj_trks_cca_non_moveable, Msmt_Update_Obj_Trks_CCA_Suspected_Moveable)
{
   /** \precond
    * Use default setup form test group except for:
    *    - Set number of selected detections for msmnt update to 1.
    *    - Set det motion_status to moving
    */
   selected_dets_num = 1U;
   det_props[3].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   float32_t exp_errcov[STATE_DIMENSION][STATE_DIMENSION] = {
      {0.4499861, 0.0010181, -0.0032667, 0.0315709, 0.0051013, 0.0035574},
      {0.0010181, 0.9458101, 0.0232391, 0.0005891, -0.8995706, 0.0154426},
      {-0.0032667, 0.0232391, 6.5699913, -0.0009446, -0.0665756, 0.4062941},
      {0.0315709, 0.0005891, -0.0009446, 0.1000408, -0.0010207, -0.0053870},
      {0.0051013, -0.8995706, -0.0665756, -0.0010207, 6.1384363, -0.2377644},
      {0.0035574, 0.0154426, 0.4062941, -0.0053870, -0.2377644, 6.9263610}
   };

   float32_t exp_vcs_position_x = 10.0924034F;
   float32_t exp_vcs_vel_long = 0.9868297F;
   float32_t exp_vcs_acc_long =  0.0999200F;
   float32_t exp_vcs_position_y = 0.0979993F;
   float32_t exp_vcs_vel_lat = 2.0197585F;
   float32_t exp_vcs_acc_lat = 0.0925598F;

   /** \action
    * Call function Msmt_Update_Obj_Trks_CCA_Moveable()
    */
   Msmt_Update_Obj_Trks_CCA_Non_Moveable(host, det_props, raw_detection_list, calib, selected_dets_idx, selected_dets_num, object_track, timing_info);

   /** \result
    * Verify that object states have changed
    */
   DOUBLES_EQUAL(exp_vcs_position_x, object_track.vcs_position.x, test_pass_th);
   DOUBLES_EQUAL(exp_vcs_vel_long, object_track.vcs_velocity.longitudinal, test_pass_th);
   DOUBLES_EQUAL(exp_vcs_acc_long, object_track.vcs_accel.longitudinal, test_pass_th);
   DOUBLES_EQUAL(exp_vcs_position_y, object_track.vcs_position.y, test_pass_th);
   DOUBLES_EQUAL(exp_vcs_vel_lat, object_track.vcs_velocity.lateral, test_pass_th);
   DOUBLES_EQUAL(exp_vcs_acc_lat, object_track.vcs_accel.lateral, test_pass_th);
   for(uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx ++)
   {
      for(uint32_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx ++)
      {
         DOUBLES_EQUAL(exp_errcov[row_idx][col_idx], object_track.errcov[row_idx][col_idx], test_pass_th);
      }
   }
}

/** \purpose
 * Verify that object states are updated correctly when the object is suspected to be cross moveable
 * \req
 * NA.
 */
TEST(f360_msmt_update_obj_trks_cca_non_moveable, Msmt_Update_Obj_Trks_CCA_Suspected_VRU_Cross_Moveable)
{
   /** \precond
    * Use default setup form test group except for:
    *    - Set number of selected detections for msmnt update to 1.
    *    - The test is setup for an object that is moving laterlly perpendicular to host
    *    - This object has been observed moving from right to left laterally for past 3 scans
    *    - The expectation is to set f_crossing_moveable flag
    */
   selected_dets_num = 1U;

   (void)object_track.vcs_heading.Value(F360_DEG2RAD(90));
   object_track.cca_cross_moving_buffer_index = 4U;
   object_track.prev_predicted_vcs_y_pos = object_track.vcs_position.y + 0.06F;
   object_track.cca_cross_moving_buffer[0] = -1;
   object_track.cca_cross_moving_buffer[1] =  0;
   object_track.cca_cross_moving_buffer[2] = -1;
   object_track.cca_cross_moving_buffer[3] = -1;


   float32_t exp_errcov[STATE_DIMENSION][STATE_DIMENSION] = {
      {0.4499861, 0.0010181, -0.0032667, 0.0315709, 0.0051013, 0.0035574},
      {0.0010181, 0.9458101, 0.0232391, 0.0005891, -0.8995706, 0.0154426},
      {-0.0032667, 0.0232391, 6.5699913, -0.0009446, -0.0665756, 0.4062941},
      {0.0315709, 0.0005891, -0.0009446, 0.1000408, -0.0010207, -0.0053870},
      {0.0051013, -0.8995706, -0.0665756, -0.0010207, 6.1384363, -0.2377644},
      {0.0035574, 0.0154426, 0.4062941, -0.0053870, -0.2377644, 6.9263610}
   };

   float32_t exp_vcs_position_x = 10.0924034F;
   float32_t exp_vcs_vel_long = 0.9868297F;
   float32_t exp_vcs_acc_long =  0.0999200F;
   float32_t exp_vcs_position_y = 0.0979993F;
   float32_t exp_vcs_vel_lat = 2.0197585F;
   float32_t exp_vcs_acc_lat = 0.0925598F;

   /** \action
    * Call function Msmt_Update_Obj_Trks_CCA_Moveable()
    */
   Msmt_Update_Obj_Trks_CCA_Non_Moveable(host, det_props, raw_detection_list, calib, selected_dets_idx, selected_dets_num, object_track, timing_info);

   /** \result
    * Verify that object states have changed
    */
   DOUBLES_EQUAL(exp_vcs_position_x, object_track.vcs_position.x, test_pass_th);
   DOUBLES_EQUAL(exp_vcs_vel_long, object_track.vcs_velocity.longitudinal, test_pass_th);
   DOUBLES_EQUAL(exp_vcs_acc_long, object_track.vcs_accel.longitudinal, test_pass_th);
   DOUBLES_EQUAL(exp_vcs_position_y, object_track.vcs_position.y, test_pass_th);
   DOUBLES_EQUAL(exp_vcs_vel_lat, object_track.vcs_velocity.lateral, test_pass_th);
   DOUBLES_EQUAL(exp_vcs_acc_lat, object_track.vcs_accel.lateral, test_pass_th);
   for(uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx ++)
   {
      for(uint32_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx ++)
      {
         DOUBLES_EQUAL(exp_errcov[row_idx][col_idx], object_track.errcov[row_idx][col_idx], test_pass_th);
      }
   }
}
