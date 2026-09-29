/** \file
 * This file contains unit tests for content of f360_msmt_update_obj_trks_cca_moveable.cpp file
 */

#include "f360_msmt_update_obj_trks_cca_moveable.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_msmt_update_obj_trks_cca_moveable
 *  @{
 */

/** \brief
 * This test group is for testing the function Msmt_Update_Obj_Trks_CCA_Moveable() which does a KF measurement updaate of a CCA object.
 */
TEST_GROUP(f360_msmt_update_obj_trks_cca_moveable)
{
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   rspp_variant_A::RSPP_Detection_List_T raw_detection_list;
   F360_Calibrations_T calib;
   uint32_t selected_dets_idx[MAX_DETS_IN_OBJ_TRK];
   uint32_t selected_dets_num;
   F360_Object_Track_T object_track;
   F360_TRKR_TIMING_INFO_T timing_info;

   const float32_t test_pass_th = 1e-6F;
   const float32_t test_pass_acc_th = 1e-3F;

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
   *       - f_moving: true
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

      object_track.vcs_velocity.longitudinal = 5.0F;
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

      object_track.meascov[0][0] = 0.6283736F;
      object_track.meascov[0][1] = 0.3861097F;
      object_track.meascov[1][0] = object_track.meascov[0][1];
      object_track.meascov[1][1] = 0.2461389F;

      object_track.num_updates_since_init = 255U;

      object_track.f_moving = true;

      object_track.ndets = 3U;
      object_track.detids[0] = 3;
      object_track.detids[1] = 17;
      object_track.detids[2] = 55;


      // Detection 1
      uint32_t idx = 2;
      raw_detection_list.detections[idx].processed.vcs_az = 0.0F;
      raw_detection_list.detections[idx].processed.cos_vcs_az = F360_Cosf(raw_detection_list.detections[idx].processed.vcs_az);
      raw_detection_list.detections[idx].processed.sin_vcs_az = F360_Sinf(raw_detection_list.detections[idx].processed.vcs_az);
      det_props[idx].range_rate_compensated = object_track.vcs_velocity.longitudinal * raw_detection_list.detections[idx].processed.cos_vcs_az + object_track.vcs_velocity.lateral * raw_detection_list.detections[idx].processed.sin_vcs_az  + 0.1F;

      // Detection 2
      idx = 16;
      raw_detection_list.detections[idx].processed.vcs_az = F360_DEG2RAD(10.0F);
      raw_detection_list.detections[idx].processed.cos_vcs_az = F360_Cosf(raw_detection_list.detections[idx].processed.vcs_az);
      raw_detection_list.detections[idx].processed.sin_vcs_az = F360_Sinf(raw_detection_list.detections[idx].processed.vcs_az);
      det_props[idx].range_rate_compensated = object_track.vcs_velocity.longitudinal * raw_detection_list.detections[idx].processed.cos_vcs_az + object_track.vcs_velocity.lateral * raw_detection_list.detections[idx].processed.sin_vcs_az;

      // Detection 3
      idx = 54;
      raw_detection_list.detections[idx].processed.vcs_az = F360_DEG2RAD(5.0F);
      raw_detection_list.detections[idx].processed.cos_vcs_az = F360_Cosf(raw_detection_list.detections[idx].processed.vcs_az);
      raw_detection_list.detections[idx].processed.sin_vcs_az = F360_Sinf(raw_detection_list.detections[idx].processed.vcs_az);
      det_props[idx].range_rate_compensated = object_track.vcs_velocity.longitudinal * raw_detection_list.detections[idx].processed.cos_vcs_az + object_track.vcs_velocity.lateral * raw_detection_list.detections[idx].processed.sin_vcs_az - 0.1F;
   
      // Selected detections
      selected_dets_idx[0] = 2;
      selected_dets_idx[1] = 16;
      selected_dets_idx[2] = 54;
      selected_dets_num = 3U;

   }
};

/** \purpose  
 * Verify that no measurement update is done when there are no associated eligable detections
 * \req
 * NA
 */
TEST(f360_msmt_update_obj_trks_cca_moveable, Msmt_Update_Obj_Trks_CCA_No_Dets)
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
   Msmt_Update_Obj_Trks_CCA_Moveable(det_props, raw_detection_list, calib, selected_dets_idx, selected_dets_num, object_track, timing_info);

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
 * Verify that object states are updated correctly when there are associated eligble detections
 * \req NA
 */
TEST(f360_msmt_update_obj_trks_cca_moveable, Msmt_Update_Obj_Trks_CCA_default)
{
   /** \precond
    * Use default setup from test group
    */

   /** \action
    * Call function Msmt_Update_Obj_Trks_CCA_Moveable()
    * Extract object pointing before function call so that it is possible to compare with afterwards.
    */
   float32_t pnt_before = object_track.bbox.Get_Orientation().Value();
   Msmt_Update_Obj_Trks_CCA_Moveable(det_props, raw_detection_list, calib, selected_dets_idx, selected_dets_num, object_track, timing_info);

   /** \result
    * Expected output is:
    *    - vcs_position: [10.085811428468116, 0.091158385978730]
    *    - vcs_velocity: [5.002059334855990, 1.980379592815318]
    *    - speed: length of updated vcs_velocity vector
    *    - vcs_heading: direction of updated vcs_velocity vector
    *    - vcs_accel: [0.098941016521831, 0.096558496272075]
    *    - tang_accel: scalar product of updated vcs_accel with unit vector in the same direction as updated vcs_velocity
    *    - errcov: [0.552309098094765, 0.002822719097295, -0.006220049282493, 0.338893491038036, 0.003852976625816, -0.013003917964762;
    *               0.002822719097295, 0.829903546572940, 0.017245996483160, 0.001767580812805, -0.418050182876877, -0.002183527111271;
    *              -0.006220049282493, 0.017245996483160, 6.569864014276908, -0.003881852642069, -0.058291180241595, 0.406220628539916;
    *               0.338893491038036, 0.001767580812805, -0.003881852642069, 0.216821994650829, 0.002256556717415, -0.008491185474361;
    *               0.003852976625816, -0.418050182876877, -0.058291180241595, 0.002256556717415, 6.100872837580059, -0.247858432974155;
    *              -0.013003917964762, -0.002183527111271, 0.406220628539916, -0.008491185474361, -0.247858432974155, 6.927180503823093F]
    * 
    *    - pointing yaw rate filter has run => pointing has changed
    *    - curvature: updated yaw_rate / updated speed
    */
   const float32_t exp_vcs_position_x = 10.0858114F;
   const float32_t exp_vcs_position_y = 0.0911583F;
   const float32_t exp_vcs_vel_x = 5.00205933F;
   const float32_t exp_vcs_vel_y = 1.9803795F;
   const float32_t exp_speed = F360_Get_Hypotenuse(exp_vcs_vel_x, exp_vcs_vel_y);
   const float32_t exp_vcs_hdg = F360_Atan2f(exp_vcs_vel_y, exp_vcs_vel_x);
   const float32_t exp_vcs_accel_x = 0.0989410F;
   const float32_t exp_vcs_accel_y = 0.0965584F;
   const float32_t exp_tang_accel = exp_vcs_accel_x * F360_Cosf(exp_vcs_hdg) + exp_vcs_accel_y * F360_Sinf(exp_vcs_hdg);
   const float32_t exp_curv = object_track.heading_rate / object_track.speed;
   const float32_t exp_errcov[STATE_DIMENSION][STATE_DIMENSION] = {{0.5523090F, 0.0028227F, -0.0062200F, 0.3388934F, 0.0038529F, -0.0130039F},
                                                                  {0.0028227F, 0.8299035F, 0.0172459F, 0.0017675F, -0.4180501F, -0.0021835F},
                                                                  {-0.0062200F, 0.0172459F, 6.5698640F, -0.0038818F, -0.0582911F, 0.4062206F},
                                                                  {0.3388934F, 0.0017675F, -0.0038818F, 0.2168219F, 0.0022565F, -0.0084911F},
                                                                  {0.0038529F, -0.4180501F, -0.0582911F, 0.0022565F, 6.1008728F, -0.2478584F},
                                                                  {-0.0130039F, -0.0021835F, 0.4062206F, -0.0084911F, -0.2478584F, 6.9271805F}};



   DOUBLES_EQUAL_TEXT(exp_vcs_position_x, object_track.vcs_position.x, test_pass_th, "VCS x postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_position_y, object_track.vcs_position.y, test_pass_th, "VCS y postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_x, object_track.vcs_velocity.longitudinal, test_pass_th, "VCS x velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_y, object_track.vcs_velocity.lateral, test_pass_th, "VCS y velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_speed, object_track.speed, test_pass_th, "Speed is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_hdg, object_track.vcs_heading.Value(), test_pass_th, "VCS heading is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_accel_x, object_track.vcs_accel.longitudinal, test_pass_th, "VCS x acceleration is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_accel_y, object_track.vcs_accel.lateral, test_pass_th, "VCS y acceleration is unexpected");
   DOUBLES_EQUAL_TEXT(exp_tang_accel, object_track.tang_accel, test_pass_acc_th, "Tangential acceleration is unexpected");
   CHECK_TRUE_TEXT(std::abs(pnt_before - object_track.bbox.Get_Orientation().Value()) > test_pass_th, "Object pointing has not been updated");
   DOUBLES_EQUAL_TEXT(exp_curv, object_track.curvature, test_pass_th, "Curvature is unexpected");
   for (uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx ++)
   {
      for (uint32_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx ++)
      {
         DOUBLES_EQUAL_TEXT(exp_errcov[row_idx][col_idx], object_track.errcov[row_idx][col_idx], test_pass_th, "Errcov is unexpected");
      }
   }
}


/** \purpose
 * Verify that object states are updated correctly when there are associated eligable detections but the object is stationary
 * \req NA
 */
TEST(f360_msmt_update_obj_trks_cca_moveable, Msmt_Update_Obj_Trks_CCA_stationary)
{
   /** \precond
    * Use default setup from test group except for
    *    - object f_moving is set to false
    */
   object_track.f_moving = false;

   /** \action
    * Call function Msmt_Update_Obj_Trks_CCA_Moveable()
    * Extract object pointing and curvature before function call so that it is possible to compare with afterwards.
    */
   float32_t pnt_before = object_track.bbox.Get_Orientation().Value();
   float32_t hdg_before = object_track.vcs_heading.Value();
   Msmt_Update_Obj_Trks_CCA_Moveable(det_props, raw_detection_list, calib, selected_dets_idx, selected_dets_num, object_track, timing_info);

   /** \result
    * Expected output is (note this is very similar to in Msmt_Update_Obj_Trks_CCA_default):
    *    - vcs_position: [10.085811428468116, 0.091158385978730]
    *    - vcs_velocity: [5.002059334855990, 1.980379592815318]
    *    - speed: length of updated vcs_velocity vector
    *    - vcs_heading: not updated
    *    - vcs_accel: [0.098941016521831, 0.096558496272075]
    *    - tang_accel: scalar product of updated vcs_accel with unit vector in the same direction as updated vcs_velocity
    *    - errcov: [0.552309098094765, 0.002822719097295, -0.006220049282493, 0.338893491038036, 0.003852976625816, -0.013003917964762;
    *               0.002822719097295, 0.829903546572940, 0.017245996483160, 0.001767580812805, -0.418050182876877, -0.002183527111271;
    *              -0.006220049282493, 0.017245996483160, 6.569864014276908, -0.003881852642069, -0.058291180241595, 0.406220628539916;
    *               0.338893491038036, 0.001767580812805, -0.003881852642069, 0.216821994650829, 0.002256556717415, -0.008491185474361;
    *               0.003852976625816, -0.418050182876877, -0.058291180241595, 0.002256556717415, 6.100872837580059, -0.247858432974155;
    *              -0.013003917964762, -0.002183527111271, 0.406220628539916, -0.008491185474361, -0.247858432974155, 6.927180503823093F]
    * 
    *    - pointing yaw rate filter has not run => pointing is unchanged
    *    - yaw_rate: 0
    *    - curvature: not updated
    */
   const float32_t exp_vcs_position_x = 10.085811428468116F;
   const float32_t exp_vcs_position_y = 0.091158385978730F;
   const float32_t exp_vcs_vel_x = 5.002059334855990F;
   const float32_t exp_vcs_vel_y = 1.980379592815318F;
   const float32_t exp_speed = F360_Get_Hypotenuse(exp_vcs_vel_x, exp_vcs_vel_y);
   const float32_t exp_vcs_accel_x = 0.0989410F;
   const float32_t exp_vcs_accel_y = 0.0965584F;
   const float32_t exp_tang_accel = exp_vcs_accel_x * F360_Cosf(hdg_before) + exp_vcs_accel_y * F360_Sinf(hdg_before);
   const float32_t exp_yaw_rate = 0.0F;
   const float32_t exp_curv = 0.0F;
   Angle exp_hdg;
   exp_hdg.Value(F360_Atan2f(exp_vcs_vel_y, exp_vcs_vel_x)).Normalize();
   const float32_t exp_errcov[STATE_DIMENSION][STATE_DIMENSION] = {{0.5523090F, 0.0028227F, -0.0062200F, 0.3388934F, 0.0038529F, -0.0130039F},
                                                                  {0.0028227F, 0.8299035F, 0.0172459F, 0.0017675F, -0.4180501F, -0.0021835F},
                                                                  {-0.0062200F, 0.0172459F, 6.5698640F, -0.0038818F, -0.0582911F, 0.4062206F},
                                                                  {0.3388934F, 0.0017675F, -0.0038818F, 0.2168219F, 0.0022565F, -0.0084911F},
                                                                  {0.0038529F, -0.4180501F, -0.0582911F, 0.0022565F, 6.1008728F, -0.2478584F},
                                                                  {-0.0130039F, -0.0021835F, 0.4062206F, -0.0084911F, -0.2478584F, 6.9271805F}};


   DOUBLES_EQUAL_TEXT(exp_vcs_position_x, object_track.vcs_position.x, test_pass_th, "VCS x postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_position_y, object_track.vcs_position.y, test_pass_th, "VCS y postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_x, object_track.vcs_velocity.longitudinal, test_pass_th, "VCS x velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_y, object_track.vcs_velocity.lateral, test_pass_th, "VCS y velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_speed, object_track.speed, test_pass_th, "Speed is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg.Value(), object_track.vcs_heading.Value(), test_pass_th, "VCS heading is unexpectedly updated");
   DOUBLES_EQUAL_TEXT(exp_vcs_accel_x, object_track.vcs_accel.longitudinal, test_pass_th, "VCS x acceleration is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_accel_y, object_track.vcs_accel.lateral, test_pass_th, "VCS y acceleration is unexpected");
   DOUBLES_EQUAL_TEXT(exp_tang_accel, object_track.tang_accel, test_pass_th, "Tangential acceleration is unexpected");
   DOUBLES_EQUAL_TEXT(pnt_before, object_track.bbox.Get_Orientation().Value(), F360_EPSILON, "Object pointing is unexpectedly updated");
   DOUBLES_EQUAL_TEXT(exp_yaw_rate, object_track.heading_rate, F360_EPSILON, "Object yaw_rate is not zero");
   DOUBLES_EQUAL_TEXT(exp_curv, object_track.curvature, test_pass_th, "Curvature is unexpectedly updated");
   for (uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx ++)
   {
      for (uint32_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx ++)
      {
         DOUBLES_EQUAL_TEXT(exp_errcov[row_idx][col_idx], object_track.errcov[row_idx][col_idx], test_pass_th, "Errcov is unexpected");
      }
   }

}



/** \purpose
 * Verify that object states are updated correctly when the object has high heading poiniting disagreement and is ok to decay states
 * \req NA
 */
TEST(f360_msmt_update_obj_trks_cca_moveable, Msmt_Update_Obj_Trks_CCA_obj_ok_to_decay_states)
{
   /** \precond
    * Use default setup from test group except for
    *    - object f_moving is set to true
    *             heading pointing disagreement is high
    *             object is placed mored than 100m and is long enough
    */
   object_track.f_moving = true;
   object_track.time_since_initialization = 1.4F;
   object_track.hdg_ptng_disagmt = 0.8F;
   object_track.vcs_position.x = 105.0F;
   object_track.bbox.Set_Length(4.0F);
   float32_t hdg_before = object_track.vcs_heading.Value();

   /** \action
    * Call function Msmt_Update_Obj_Trks_CCA_Moveable()
    * 
    */


   Msmt_Update_Obj_Trks_CCA_Moveable(det_props, raw_detection_list, calib, selected_dets_idx, selected_dets_num, object_track, timing_info);

   /** \result
    * Expected output is 
    *    - vcs_position: [10.1, 0.1]
    *    - vcs_velocity: [5.002059334855990, 1.980379592815318]
    *    - speed: length of updated vcs_velocity vector
    *    - vcs_heading: not updated
    *    - vcs_accel: [0.1, 0.0]
    *    - tang_accel: scalar product of updated vcs_accel with unit vector in the same direction as updated vcs_velocity
    * 
    *    - yaw_rate: -0.3224583
    *    - curvature:-0.06727862F
    */
   const float32_t exp_vcs_position_x = 10.1;
   const float32_t exp_vcs_position_y = 0.1F;
   const float32_t exp_vcs_vel_x = 4.792879;
   const float32_t exp_vcs_vel_y = 0.0F;
   const float32_t exp_speed = F360_Get_Hypotenuse(exp_vcs_vel_x, exp_vcs_vel_y);
   const float32_t exp_vcs_accel_x = 0.1F;
   const float32_t exp_vcs_accel_y = 0.0F;
   const float32_t exp_tang_accel = exp_vcs_accel_x * F360_Cosf(hdg_before) + exp_vcs_accel_y * F360_Sinf(hdg_before);
   const float32_t exp_yaw_rate = -0.3416343;
   const float32_t exp_curv = -0.07127956;
   Angle exp_hdg;
   exp_hdg.Value(F360_Atan2f(exp_vcs_vel_y, exp_vcs_vel_x)).Normalize();


   DOUBLES_EQUAL_TEXT(exp_vcs_position_x, object_track.vcs_position.x, test_pass_th, "VCS x postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_position_y, object_track.vcs_position.y, test_pass_th, "VCS y postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_x, object_track.vcs_velocity.longitudinal, test_pass_th, "VCS x velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_y, object_track.vcs_velocity.lateral, test_pass_th, "VCS y velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_speed, object_track.speed, test_pass_th, "Speed is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg.Value(), object_track.vcs_heading.Value(), test_pass_th, "VCS heading is unexpectedly updated");
   DOUBLES_EQUAL_TEXT(exp_vcs_accel_x, object_track.vcs_accel.longitudinal, test_pass_th, "VCS x acceleration is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_accel_y, object_track.vcs_accel.lateral, test_pass_th, "VCS y acceleration is unexpected");
   DOUBLES_EQUAL_TEXT(exp_tang_accel, object_track.tang_accel, test_pass_th, "Tangential acceleration is unexpected");
   DOUBLES_EQUAL_TEXT(exp_yaw_rate, object_track.heading_rate, F360_EPSILON, "Object yaw_rate is unexpected");
   DOUBLES_EQUAL_TEXT(exp_curv, object_track.curvature, test_pass_th, "Curvature is unexpectedly updated");


}


/** \purpose
 * Verify that object states are updated correctly when the object has high heading poiniting disagreement
 * but has a small length, therefore is not ok to decay states.
 * \req NA
 */
TEST(f360_msmt_update_obj_trks_cca_moveable, Msmt_Update_Obj_Trks_CCA_obj_not_ok_to_decay_states_for_small_objects)
{
   /** \precond
    * Use default setup from test group except for
    *    - object f_moving is set to true
    *             heading pointing disagreement is high
    *             object is placed mored than 100m 
    *             object is less than 3m long
    */
   object_track.f_moving = true;
   object_track.time_since_initialization = 1.4F;
   object_track.hdg_ptng_disagmt = 0.8F;
   object_track.vcs_position.x = 105.0F;
   object_track.bbox.Set_Length(2.0F);
   float32_t hdg_before = object_track.vcs_heading.Value();

   /** \action
    * Call function Msmt_Update_Obj_Trks_CCA_Moveable()
    * 
    */


   Msmt_Update_Obj_Trks_CCA_Moveable(det_props, raw_detection_list, calib, selected_dets_idx, selected_dets_num, object_track, timing_info);

   /** \result
    * Expected output is, this is an unrealistic output that shall not occur in the tracker, 
    * but is considered for the coverage perspective
    *    - vcs_position: [18.4491, 5.230103]
    *    - vcs_velocity: [4.792879, 0.6694605]
    *    - speed: length of updated vcs_velocity vector
    *    - vcs_heading: not updated
    *    - vcs_accel: [0.6460894, -1.22575]
    *    - tang_accel: scalar product of updated vcs_accel with unit vector in the same direction as updated vcs_velocity
    * 
    *    - yaw_rate: -0.3224583
    *    - curvature:-0.06727862F
    */
   const float32_t exp_vcs_position_x = 18.4491;
   const float32_t exp_vcs_position_y = 5.230103F;
   const float32_t exp_vcs_vel_x = 4.792879;
   const float32_t exp_vcs_vel_y = 0.6694605F;
   const float32_t exp_speed = F360_Get_Hypotenuse(exp_vcs_vel_x, exp_vcs_vel_y);
   const float32_t exp_vcs_accel_x = 0.6460894F;
   const float32_t exp_vcs_accel_y = -1.22575F;
   const float32_t exp_tang_accel = exp_vcs_accel_x * F360_Cosf(hdg_before) + exp_vcs_accel_y * F360_Sinf(hdg_before);
   const float32_t exp_yaw_rate = -0.3171268;
   const float32_t exp_curv = -0.06553008F;
   Angle exp_hdg;
   exp_hdg.Value(F360_Atan2f(exp_vcs_vel_y, exp_vcs_vel_x)).Normalize();
   const float32_t pos_threshold = 0.0001;


   DOUBLES_EQUAL_TEXT(exp_vcs_position_x, object_track.vcs_position.x, pos_threshold, "VCS x postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_position_y, object_track.vcs_position.y, test_pass_th, "VCS y postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_x, object_track.vcs_velocity.longitudinal, test_pass_th, "VCS x velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_y, object_track.vcs_velocity.lateral, test_pass_th, "VCS y velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_speed, object_track.speed, test_pass_th, "Speed is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg.Value(), object_track.vcs_heading.Value(), test_pass_th, "VCS heading is unexpectedly updated");
   DOUBLES_EQUAL_TEXT(exp_vcs_accel_x, object_track.vcs_accel.longitudinal, test_pass_th, "VCS x acceleration is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_accel_y, object_track.vcs_accel.lateral, test_pass_th, "VCS y acceleration is unexpected");
   DOUBLES_EQUAL_TEXT(exp_tang_accel, object_track.tang_accel, test_pass_th, "Tangential acceleration is unexpected");
   DOUBLES_EQUAL_TEXT(exp_yaw_rate, object_track.heading_rate, F360_EPSILON, "Object yaw_rate is unexpected");
   DOUBLES_EQUAL_TEXT(exp_curv, object_track.curvature, test_pass_th, "Curvature is unexpectedly updated");


}

/** \purpose
 * Verify that object states are updated correctly when the object has high heading poiniting disagreement
 * but is stationary, therefore is not ok to decay states.
 * \req NA
 */
TEST(f360_msmt_update_obj_trks_cca_moveable, Msmt_Update_Obj_Trks_CCA_obj_not_ok_to_decay_states_for_stationary_objects)
{
   /** \precond
    * Use default setup from test group except for
    *    - object f_moving is set to false
    *             heading pointing disagreement is high
    *             object is placed mored than 100m 
    *             object is more than 3m long
    */
   object_track.f_moving = false;
   object_track.time_since_initialization = 1.4F;
   object_track.hdg_ptng_disagmt = 0.8F;
   object_track.vcs_position.x = 105.0F;
   object_track.bbox.Set_Length(4.0F);
   float32_t hdg_before = object_track.vcs_heading.Value();

   /** \action
    * Call function Msmt_Update_Obj_Trks_CCA_Moveable()
    * 
    */


   Msmt_Update_Obj_Trks_CCA_Moveable(det_props, raw_detection_list, calib, selected_dets_idx, selected_dets_num, object_track, timing_info);

   /** \result
    * Expected output is, this is an unrealistic output that shall not occur in the tracker, 
    * but is considered for the coverage perspective
    *    - vcs_position: [18.4491, 5.230103]
    *    - vcs_velocity: [4.792879, 0.6694605]
    *    - speed: length of updated vcs_velocity vector
    *    - vcs_heading: not updated
    *    - vcs_accel: [0.6460894, -1.22575]
    *    - tang_accel: scalar product of updated vcs_accel with unit vector in the same direction as updated vcs_velocity
    * 
    *    - yaw_rate: 0.0
    *    - curvature:0.0
    */
   const float32_t exp_vcs_position_x = 18.4491;
   const float32_t exp_vcs_position_y = 5.230103F;
   const float32_t exp_vcs_vel_x = 4.792879;
   const float32_t exp_vcs_vel_y = 0.6694605F;
   const float32_t exp_speed = F360_Get_Hypotenuse(exp_vcs_vel_x, exp_vcs_vel_y);
   const float32_t exp_vcs_accel_x = 0.6460894F;
   const float32_t exp_vcs_accel_y = -1.22575F;
   const float32_t exp_tang_accel = exp_vcs_accel_x * F360_Cosf(hdg_before) + exp_vcs_accel_y * F360_Sinf(hdg_before);
   const float32_t exp_yaw_rate = 0.0F;
   const float32_t exp_curv = 0.0F;
   Angle exp_hdg;
   exp_hdg.Value(F360_Atan2f(exp_vcs_vel_y, exp_vcs_vel_x)).Normalize();
   const float32_t pos_threshold = 0.0001;


   DOUBLES_EQUAL_TEXT(exp_vcs_position_x, object_track.vcs_position.x, pos_threshold, "VCS x postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_position_y, object_track.vcs_position.y, test_pass_th, "VCS y postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_x, object_track.vcs_velocity.longitudinal, test_pass_th, "VCS x velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_y, object_track.vcs_velocity.lateral, test_pass_th, "VCS y velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_speed, object_track.speed, test_pass_th, "Speed is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg.Value(), object_track.vcs_heading.Value(), test_pass_th, "VCS heading is unexpectedly updated");
   DOUBLES_EQUAL_TEXT(exp_vcs_accel_x, object_track.vcs_accel.longitudinal, test_pass_th, "VCS x acceleration is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_accel_y, object_track.vcs_accel.lateral, test_pass_th, "VCS y acceleration is unexpected");
   DOUBLES_EQUAL_TEXT(exp_tang_accel, object_track.tang_accel, test_pass_th, "Tangential acceleration is unexpected");
   DOUBLES_EQUAL_TEXT(exp_yaw_rate, object_track.heading_rate, F360_EPSILON, "Object yaw_rate is unexpected");
   DOUBLES_EQUAL_TEXT(exp_curv, object_track.curvature, test_pass_th, "Curvature is unexpectedly updated");


}


/** \purpose
 * Verify that object states are updated correctly when the object has high heading poiniting disagreement 
 * but is less than 100m in vcs long
 * \req NA
 */
TEST(f360_msmt_update_obj_trks_cca_moveable, Msmt_Update_Obj_Trks_CCA_obj_not_ok_to_decay_for_close_objs_in_vcs)
{
   /** \precond
    * Use default setup from test group except for
    *    - object f_moving is set to true
    *             heading pointing disagreement is high
    *             object is placed less than 100m longitdinally
    */
   object_track.f_moving = true;
   object_track.time_since_initialization = 1.4F;
   object_track.hdg_ptng_disagmt = 0.8F;
   object_track.vcs_position.x = 10.0F;
   object_track.bbox.Set_Length(4.0F);


   /** \action
    * Call function Msmt_Update_Obj_Trks_CCA_Moveable()
    * 
    */


   Msmt_Update_Obj_Trks_CCA_Moveable(det_props, raw_detection_list, calib, selected_dets_idx, selected_dets_num, object_track, timing_info);

   /** \result
    * Expected output is (note this is very similar to in Msmt_Update_Obj_Trks_CCA_default):
    *    - vcs_position: [10.085811428468116, 0.091158385978730]
    *    - vcs_velocity: [5.002059334855990, 1.980379592815318]
    *    - speed: length of updated vcs_velocity vector
    *    - vcs_heading: not updated
    *    - vcs_accel: [0.098941016521831, 0.096558496272075]
    */
   const float32_t exp_vcs_position_x = 10.085811428468116F;
   const float32_t exp_vcs_position_y = 0.091158385978730F;
   const float32_t exp_vcs_vel_x = 5.002059334855990F;
   const float32_t exp_vcs_vel_y = 1.980379592815318F;
   const float32_t exp_speed = F360_Get_Hypotenuse(exp_vcs_vel_x, exp_vcs_vel_y);
   const float32_t exp_vcs_accel_x = 0.0989410F;
   const float32_t exp_vcs_accel_y = 0.0965584F;
   const float32_t exp_yaw_rate = -0.1493891F;
   const float32_t exp_curv = -0.0277684F;
   Angle exp_hdg;
   exp_hdg.Value(F360_Atan2f(exp_vcs_vel_y, exp_vcs_vel_x)).Normalize();


   DOUBLES_EQUAL_TEXT(exp_vcs_position_x, object_track.vcs_position.x, test_pass_th, "VCS x postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_position_y, object_track.vcs_position.y, test_pass_th, "VCS y postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_x, object_track.vcs_velocity.longitudinal, test_pass_th, "VCS x velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_y, object_track.vcs_velocity.lateral, test_pass_th, "VCS y velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_speed, object_track.speed, test_pass_th, "Speed is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg.Value(), object_track.vcs_heading.Value(), test_pass_th, "VCS heading is unexpectedly updated");
   DOUBLES_EQUAL_TEXT(exp_vcs_accel_x, object_track.vcs_accel.longitudinal, test_pass_th, "VCS x acceleration is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_accel_y, object_track.vcs_accel.lateral, test_pass_th, "VCS y acceleration is unexpected");
   DOUBLES_EQUAL_TEXT(exp_yaw_rate, object_track.heading_rate, F360_EPSILON, "Object yaw_rate is unexpected");
   DOUBLES_EQUAL_TEXT(exp_curv, object_track.curvature, test_pass_th, "Curvature is unexpectedly updated");


}


/** \purpose
 * Verify that object states are updated correctly when the object has low heading poiniting disagreement 
 * and is less than 100m in vcs long
 * \req NA
 */
TEST(f360_msmt_update_obj_trks_cca_moveable, Msmt_Update_Obj_Trks_CCA_obj_not_ok_to_decay_for_low_heading_pointing_disagmt)
{
   /** \precond
    * Use default setup from test group except for
    *    - object f_moving is set to true
    *             heading pointing disagreement is high
    *             object is placed less than 100m longitdinally
    */
   object_track.f_moving = true;
   object_track.time_since_initialization = 1.4F;
   object_track.hdg_ptng_disagmt = 0.4F;
   object_track.vcs_position.x = 10.0F;
   object_track.bbox.Set_Length(4.0F);


   /** \action
    * Call function Msmt_Update_Obj_Trks_CCA_Moveable()
    * 
    */


   Msmt_Update_Obj_Trks_CCA_Moveable(det_props, raw_detection_list, calib, selected_dets_idx, selected_dets_num, object_track, timing_info);

   /** \result
    * Expected output is
    *    - vcs_position: [10.085811428468116, 0.091158385978730]
    *    - vcs_velocity: [5.002059334855990, 1.980379592815318]
    *    - speed: length of updated vcs_velocity vector
    *    - vcs_heading: not updated
    *    - vcs_accel: [0.098941016521831, 0.096558496272075]
    */
   const float32_t exp_vcs_position_x = 10.085811428468116F;
   const float32_t exp_vcs_position_y = 0.091158385978730F;
   const float32_t exp_vcs_vel_x = 5.002059334855990F;
   const float32_t exp_vcs_vel_y = 1.980379592815318F;
   const float32_t exp_speed = F360_Get_Hypotenuse(exp_vcs_vel_x, exp_vcs_vel_y);
   const float32_t exp_vcs_accel_x = 0.0989410F;
   const float32_t exp_vcs_accel_y = 0.0965584F;
   const float32_t exp_yaw_rate = -0.1493891F;
   const float32_t exp_curv = -0.027768F;
   Angle exp_hdg;
   exp_hdg.Value(F360_Atan2f(exp_vcs_vel_y, exp_vcs_vel_x)).Normalize();


   DOUBLES_EQUAL_TEXT(exp_vcs_position_x, object_track.vcs_position.x, test_pass_th, "VCS x postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_position_y, object_track.vcs_position.y, test_pass_th, "VCS y postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_x, object_track.vcs_velocity.longitudinal, test_pass_th, "VCS x velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_y, object_track.vcs_velocity.lateral, test_pass_th, "VCS y velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_speed, object_track.speed, test_pass_th, "Speed is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg.Value(), object_track.vcs_heading.Value(), test_pass_th, "VCS heading is unexpectedly updated");
   DOUBLES_EQUAL_TEXT(exp_vcs_accel_x, object_track.vcs_accel.longitudinal, test_pass_th, "VCS x acceleration is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_accel_y, object_track.vcs_accel.lateral, test_pass_th, "VCS y acceleration is unexpected");
   DOUBLES_EQUAL_TEXT(exp_yaw_rate, object_track.heading_rate, F360_EPSILON, "Object yaw_rate is unexpected");
   DOUBLES_EQUAL_TEXT(exp_curv, object_track.curvature, test_pass_th, "Curvature is unexpectedly updated");


}

/** \purpose
 * Verify that object states are updated correctly when the object has no associated detections
 * \req NA
 */
TEST(f360_msmt_update_obj_trks_cca_moveable, Msmt_Update_Obj_Trks_CCA_n_dets_zero)
{
   /** \precond
    * Use default setup from test group except for
    *    - object n dets set to zero
    */
   object_track.ndets = 0;

   /** \action
    * Call function Msmt_Update_Obj_Trks_CCA_Moveable()
    * 
    */



   Msmt_Update_Obj_Trks_CCA_Moveable(det_props, raw_detection_list, calib, selected_dets_idx, selected_dets_num, object_track, timing_info);

   /** \result
    * Expected output is (note this is very similar to in Msmt_Update_Obj_Trks_CCA_default):
    *    - vcs_position: [10.085811428468116, 0.091158385978730]
    *    - vcs_velocity: [5.002059334855990, 1.980379592815318]
    *    - speed: length of updated vcs_velocity vector
    *    - vcs_heading: not updated
    *    - vcs_accel: [0.098941016521831, 0.096558496272075]
    *    - tang_accel: scalar product of updated vcs_accel with unit vector in the same direction as updated vcs_velocity
    *    - yaw_rate: -0.182852F
    *    - curvature: -0.03398848F
    */

   const float32_t exp_vcs_position_x = 10.085811428468116F;
   const float32_t exp_vcs_position_y = 0.091158385978730F;
   const float32_t exp_vcs_vel_x = 5.002059334855990F;
   const float32_t exp_vcs_vel_y = 1.980379592815318F;
   const float32_t exp_speed = F360_Get_Hypotenuse(exp_vcs_vel_x, exp_vcs_vel_y);
   const float32_t exp_vcs_accel_x = 0.0989410F;
   const float32_t exp_vcs_accel_y = 0.0965584F;
   const float32_t exp_yaw_rate = -0.182852F;
   const float32_t exp_curv = -0.03398848F;
   Angle exp_hdg;
   exp_hdg.Value(F360_Atan2f(exp_vcs_vel_y, exp_vcs_vel_x)).Normalize();


   DOUBLES_EQUAL_TEXT(exp_vcs_position_x, object_track.vcs_position.x, test_pass_th, "VCS x postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_position_y, object_track.vcs_position.y, test_pass_th, "VCS y postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_x, object_track.vcs_velocity.longitudinal, test_pass_th, "VCS x velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_y, object_track.vcs_velocity.lateral, test_pass_th, "VCS y velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_speed, object_track.speed, test_pass_th, "Speed is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg.Value(), object_track.vcs_heading.Value(), test_pass_th, "VCS heading is unexpectedly updated");
   DOUBLES_EQUAL_TEXT(exp_vcs_accel_x, object_track.vcs_accel.longitudinal, test_pass_th, "VCS x acceleration is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_accel_y, object_track.vcs_accel.lateral, test_pass_th, "VCS y acceleration is unexpected");
   DOUBLES_EQUAL_TEXT(exp_yaw_rate, object_track.heading_rate, F360_EPSILON, "Object yaw_rate is unexpected");
   DOUBLES_EQUAL_TEXT(exp_curv, object_track.curvature, test_pass_th, "Curvature is unexpectedly updated");


}


/** \purpose
 * Verify that object states is not updated when object is moving and has one moving detection
 * \req NA
 */
TEST(f360_msmt_update_obj_trks_cca_moveable, Msmt_Update_Obj_Trks_CCA_one_moving_detection)
{
      /** \precond
    * Use default setup from test group except for
    *    - object f_moving is set to true
    *      one moving detection
    */
   object_track.f_moving = true;
   det_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;

   /** \action
    * Call function Msmt_Update_Obj_Trks_CCA_Moveable()
    *
    */

   Msmt_Update_Obj_Trks_CCA_Moveable(det_props, raw_detection_list, calib, selected_dets_idx, selected_dets_num, object_track, timing_info);

   /** \result
    * Expected output is (note this is very similar to in Msmt_Update_Obj_Trks_CCA_default):
    *    - vcs_position: [10.085811428468116, 0.091158385978730]
    *    - vcs_velocity: [5.002059334855990, 1.980379592815318]
    *    - speed: length of updated vcs_velocity vector
    *    - vcs_heading: not updated
    *    - vcs_accel: [0.098941016521831, 0.096558496272075]
    *    - tang_accel: scalar product of updated vcs_accel with unit vector in the same direction as updated vcs_velocity
    */
   const float32_t exp_vcs_position_x = 10.085811428468116F;
   const float32_t exp_vcs_position_y = 0.091158385978730F;
   const float32_t exp_vcs_vel_x = 5.002059334855990F;
   const float32_t exp_vcs_vel_y = 1.980379592815318F;
   const float32_t exp_speed = F360_Get_Hypotenuse(exp_vcs_vel_x, exp_vcs_vel_y);
   const float32_t exp_vcs_accel_x = 0.0989410F;
   const float32_t exp_vcs_accel_y = 0.0965584F;
   const float32_t exp_yaw_rate = -0.182852F;
   const float32_t exp_curv = -0.03398848F;
   Angle exp_hdg;
   exp_hdg.Value(F360_Atan2f(exp_vcs_vel_y, exp_vcs_vel_x)).Normalize();


   DOUBLES_EQUAL_TEXT(exp_vcs_position_x, object_track.vcs_position.x, test_pass_th, "VCS x postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_position_y, object_track.vcs_position.y, test_pass_th, "VCS y postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_x, object_track.vcs_velocity.longitudinal, test_pass_th, "VCS x velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_y, object_track.vcs_velocity.lateral, test_pass_th, "VCS y velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_speed, object_track.speed, test_pass_th, "Speed is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg.Value(), object_track.vcs_heading.Value(), test_pass_th, "VCS heading is unexpectedly updated");
   DOUBLES_EQUAL_TEXT(exp_vcs_accel_x, object_track.vcs_accel.longitudinal, test_pass_th, "VCS x acceleration is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_accel_y, object_track.vcs_accel.lateral, test_pass_th, "VCS y acceleration is unexpected");
   DOUBLES_EQUAL_TEXT(exp_yaw_rate, object_track.heading_rate, F360_EPSILON, "Object yaw_rate is unexpected");
   DOUBLES_EQUAL_TEXT(exp_curv, object_track.curvature, test_pass_th, "Curvature is unexpectedly updated");


}


/** \purpose
 * Verify that object states are updated correctly when object acceleration is perpendicular to heading objs
 * \req NA
 */
TEST(f360_msmt_update_obj_trks_cca_moveable, Msmt_Update_Obj_Trks_CCA_acc_perpendicular_to_heading_objs)
{
      /** \precond
    * Use default setup from test group except for
    *    - object f_moving is set to true
    *      setting accelration perpendicular to obj heading
    */
   object_track.f_moving = true;
   det_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   object_track.vcs_accel.lateral = 50;
   object_track.vcs_accel.longitudinal = 2;
   object_track.speed = 3.0F;
   object_track.hdg_ptng_disagmt = 0.15;

   /** \action
    * Call function Msmt_Update_Obj_Trks_CCA_Moveable()
    */

   Msmt_Update_Obj_Trks_CCA_Moveable(det_props, raw_detection_list, calib, selected_dets_idx, selected_dets_num, object_track, timing_info);

   /** \result
    * Expected output is (note this is very similar to in Msmt_Update_Obj_Trks_CCA_default):
    *    - vcs_position: [10.085811428468116, 0.091158385978730]
    *    - vcs_velocity: [5.002059334855990, 1.980379592815318]
    *    - speed: length of updated vcs_velocity vector
    *    - tang_accel: scalar product of updated vcs_accel with unit vector in the same direction as updated vcs_velocity
    */
   const float32_t exp_vcs_position_x = 10.085811428468116F;
   const float32_t exp_vcs_position_y = 0.09115837;
   const float32_t exp_vcs_vel_x = 5.002059334855990F;
   const float32_t exp_vcs_vel_y = 1.980379592815318F;
   const float32_t exp_speed = F360_Get_Hypotenuse(exp_vcs_vel_x, exp_vcs_vel_y);

   Angle exp_hdg;
   exp_hdg.Value(F360_Atan2f(exp_vcs_vel_y, exp_vcs_vel_x)).Normalize();
   DOUBLES_EQUAL_TEXT(exp_vcs_position_x, object_track.vcs_position.x, test_pass_th, "VCS x postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_position_y, object_track.vcs_position.y, test_pass_th, "VCS y postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_x, object_track.vcs_velocity.longitudinal, test_pass_th, "VCS x velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_y, object_track.vcs_velocity.lateral, test_pass_th, "VCS y velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_speed, object_track.speed, test_pass_th, "Speed is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg.Value(), object_track.vcs_heading.Value(), test_pass_th, "VCS heading is unexpectedly updated");

}


/** \purpose
 * Verify that object states are updated correctly when object acceleration is perpendicular to heading objs for reversing objects
 * \req NA
 */
TEST(f360_msmt_update_obj_trks_cca_moveable, Msmt_Update_Obj_Trks_CCA_acc_perpendicular_to_heading_objs_for_reversing_object)
{
      /** \precond
    * Use default setup from test group except for
    *    - object f_moving is set to true
    *      setting accelration perpendicular to obj heading
    */
   object_track.f_moving = true;
   det_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   object_track.vcs_accel.lateral = 50;
   object_track.vcs_accel.longitudinal = 2;
   object_track.speed = -3.0F;
   object_track.hdg_ptng_disagmt = 0.15;

   /** \action
    * Call function Msmt_Update_Obj_Trks_CCA_Moveable()
    */

   Msmt_Update_Obj_Trks_CCA_Moveable(det_props, raw_detection_list, calib, selected_dets_idx, selected_dets_num, object_track, timing_info);

   /** \result
    * Expected output is (note this is very similar to in Msmt_Update_Obj_Trks_CCA_default):
    *    - vcs_position: [10.085811428468116, 0.091158385978730]
    *    - vcs_velocity: [5.002059334855990, 1.980379592815318]
    *    - speed: length of updated vcs_velocity vector
    *    - tang_accel: scalar product of updated vcs_accel with unit vector in the same direction as updated vcs_velocity
    */
   const float32_t exp_vcs_position_x = 10.085811428468116F;
   const float32_t exp_vcs_position_y = 0.09115837;
   const float32_t exp_vcs_vel_x = 5.002059334855990F;
   const float32_t exp_vcs_vel_y = 1.980379592815318F;
   const float32_t exp_speed = F360_Get_Hypotenuse(exp_vcs_vel_x, exp_vcs_vel_y);

   Angle exp_hdg;
   exp_hdg.Value(F360_Atan2f(exp_vcs_vel_y, exp_vcs_vel_x)).Normalize();
   DOUBLES_EQUAL_TEXT(exp_vcs_position_x, object_track.vcs_position.x, test_pass_th, "VCS x postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_position_y, object_track.vcs_position.y, test_pass_th, "VCS y postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_x, object_track.vcs_velocity.longitudinal, test_pass_th, "VCS x velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_y, object_track.vcs_velocity.lateral, test_pass_th, "VCS y velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_speed, object_track.speed, test_pass_th, "Speed is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg.Value(), object_track.vcs_heading.Value(), test_pass_th, "VCS heading is unexpectedly updated");

}


/** \purpose
 * Verify that object states are updated correctly when object acceleration is perpendicular to heading objs with high
 * reversing speed
 * \req NA
 */
TEST(f360_msmt_update_obj_trks_cca_moveable, Msmt_Update_Obj_Trks_CCA_acc_perpendicular_to_heading_objs_high_reversing_speed)
{
      /** \precond
    * Use default setup from test group except for
    *    - object f_moving is set to true
    *      setting accelration perpendicular to obj heading
    */
   object_track.f_moving = true;
   det_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   object_track.vcs_accel.lateral = 50;
   object_track.vcs_accel.longitudinal = 2;
   object_track.speed = -5.0F;
   object_track.hdg_ptng_disagmt = 0.15;

   /** \action
    * Call function Msmt_Update_Obj_Trks_CCA_Moveable()
    */

   Msmt_Update_Obj_Trks_CCA_Moveable(det_props, raw_detection_list, calib, selected_dets_idx, selected_dets_num, object_track, timing_info);

   /** \result
    * Expected output is (note this is very similar to in Msmt_Update_Obj_Trks_CCA_default):
    *    - vcs_position: [10.085811428468116, 0.091158385978730]
    *    - vcs_velocity: [5.002059334855990, 1.980379592815318]
    *    - speed: length of updated vcs_velocity vector
    *    - tang_accel: scalar product of updated vcs_accel with unit vector in the same direction as updated vcs_velocity
    */
   const float32_t exp_vcs_position_x = 10.085811428468116F;
   const float32_t exp_vcs_position_y = 0.09115837;
   const float32_t exp_vcs_vel_x = 5.002059334855990F;
   const float32_t exp_vcs_vel_y = 1.980379592815318F;
   const float32_t exp_speed = F360_Get_Hypotenuse(exp_vcs_vel_x, exp_vcs_vel_y);

   Angle exp_hdg;
   exp_hdg.Value(F360_Atan2f(exp_vcs_vel_y, exp_vcs_vel_x)).Normalize();
   DOUBLES_EQUAL_TEXT(exp_vcs_position_x, object_track.vcs_position.x, test_pass_th, "VCS x postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_position_y, object_track.vcs_position.y, test_pass_th, "VCS y postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_x, object_track.vcs_velocity.longitudinal, test_pass_th, "VCS x velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_y, object_track.vcs_velocity.lateral, test_pass_th, "VCS y velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_speed, object_track.speed, test_pass_th, "Speed is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg.Value(), object_track.vcs_heading.Value(), test_pass_th, "VCS heading is unexpectedly updated");

}


/** \purpose
 * Verify that object states are updated correctly when object acceleration is perpendicular to heading objs
 * but the longitudinal acceleration is not high enough
 * \req NA
 */
TEST(f360_msmt_update_obj_trks_cca_moveable, Msmt_Update_Obj_Trks_CCA_longi_acc_less_than_threshold)
{
      /** \precond
    * Use default setup from test group except for
    *    - object f_moving is set to true
    *      setting accelration perpendicular to obj heading
    *      longi accelaration is less than a threshold of 1.5
    */
   object_track.f_moving = true;
   det_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   object_track.vcs_accel.lateral = 30;
   object_track.vcs_accel.longitudinal = 0.5;
   object_track.speed = 3.0F;
   object_track.hdg_ptng_disagmt = 0.15;

   /** \action
    * Call function Msmt_Update_Obj_Trks_CCA_Moveable()
    */

   Msmt_Update_Obj_Trks_CCA_Moveable(det_props, raw_detection_list, calib, selected_dets_idx, selected_dets_num, object_track, timing_info);

   /** \result
    * Expected output is (note this is very similar to in Msmt_Update_Obj_Trks_CCA_default):
    *    - vcs_position: [10.085811428468116, 0.091158385978730]
    *    - vcs_velocity: [5.002059334855990, 1.980379592815318]
    *    - speed: length of updated vcs_velocity vector
    *    - tang_accel: scalar product of updated vcs_accel with unit vector in the same direction as updated vcs_velocity
    */
   const float32_t exp_vcs_position_x = 10.085811428468116F;
   const float32_t exp_vcs_position_y = 0.09115837;
   const float32_t exp_vcs_vel_x = 5.002059334855990F;
   const float32_t exp_vcs_vel_y = 1.980379592815318F;
   const float32_t exp_speed = F360_Get_Hypotenuse(exp_vcs_vel_x, exp_vcs_vel_y);

   Angle exp_hdg;
   exp_hdg.Value(F360_Atan2f(exp_vcs_vel_y, exp_vcs_vel_x)).Normalize();
   DOUBLES_EQUAL_TEXT(exp_vcs_position_x, object_track.vcs_position.x, test_pass_th, "VCS x postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_position_y, object_track.vcs_position.y, test_pass_th, "VCS y postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_x, object_track.vcs_velocity.longitudinal, test_pass_th, "VCS x velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_y, object_track.vcs_velocity.lateral, test_pass_th, "VCS y velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_speed, object_track.speed, test_pass_th, "Speed is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg.Value(), object_track.vcs_heading.Value(), test_pass_th, "VCS heading is unexpectedly updated");

}



/** \purpose
 * Verify that object states are updated correctly when object acceleration is no longer perpendicular to the heading
 * \req NA
 */
TEST(f360_msmt_update_obj_trks_cca_moveable, Msmt_Update_Obj_Trks_CCA_acc_angle_heading_diff_greater_than_max_thresh)
{
      /** \precond
    * Use default setup from test group except for
    *    - object f_moving is set to true
    *      setting accelration perpendicular to obj heading
    *      longi accelaration is less than a threshold of 1.5
    */
   object_track.f_moving = true;
   det_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   object_track.vcs_accel.lateral = 30;
   object_track.vcs_accel.longitudinal = -40;
   object_track.speed = 3.0F;
   object_track.hdg_ptng_disagmt = 0.15;

   /** \action
    * Call function Msmt_Update_Obj_Trks_CCA_Moveable()
    */

   Msmt_Update_Obj_Trks_CCA_Moveable(det_props, raw_detection_list, calib, selected_dets_idx, selected_dets_num, object_track, timing_info);

   /** \result
    * Expected output is (note this is very similar to in Msmt_Update_Obj_Trks_CCA_default):
    *    - vcs_position: [10.085811428468116, 0.091158385978730]
    *    - vcs_velocity: [5.002059334855990, 1.980379592815318]
    *    - speed: length of updated vcs_velocity vector
    *    - tang_accel: scalar product of updated vcs_accel with unit vector in the same direction as updated vcs_velocity
    */
   const float32_t exp_vcs_position_x = 10.085811428468116F;
   const float32_t exp_vcs_position_y = 0.09115837;
   const float32_t exp_vcs_vel_x = 5.002059334855990F;
   const float32_t exp_vcs_vel_y = 1.980379592815318F;
   const float32_t exp_speed = F360_Get_Hypotenuse(exp_vcs_vel_x, exp_vcs_vel_y);

   Angle exp_hdg;
   exp_hdg.Value(F360_Atan2f(exp_vcs_vel_y, exp_vcs_vel_x)).Normalize();
   DOUBLES_EQUAL_TEXT(exp_vcs_position_x, object_track.vcs_position.x, test_pass_th, "VCS x postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_position_y, object_track.vcs_position.y, test_pass_th, "VCS y postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_x, object_track.vcs_velocity.longitudinal, test_pass_th, "VCS x velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_y, object_track.vcs_velocity.lateral, test_pass_th, "VCS y velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_speed, object_track.speed, test_pass_th, "Speed is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg.Value(), object_track.vcs_heading.Value(), test_pass_th, "VCS heading is unexpectedly updated");

}



/** \purpose
 * Verify that object states are updated correctly when object acceleration is not perpendicular to heading objs
 * \req NA
 */
TEST(f360_msmt_update_obj_trks_cca_moveable, Msmt_Update_Obj_Trks_CCA_acc_not_perpendicular_to_heading_objs)
{
      /** \precond
    * Use default setup from test group except for
    *    - object f_moving is set to true
    */
   object_track.f_moving = true;
   det_props[2].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   object_track.vcs_accel.lateral = 30;
   object_track.vcs_accel.longitudinal = 1.0F;
   object_track.speed = 5.0F;
   object_track.hdg_ptng_disagmt = 0.20;

   /** \action
    * Call function Msmt_Update_Obj_Trks_CCA_Moveable()
    * Extract object pointing and curvature before function call so that it is possible to compare with afterwards.
    */

   Msmt_Update_Obj_Trks_CCA_Moveable(det_props, raw_detection_list, calib, selected_dets_idx, selected_dets_num, object_track, timing_info);

   /** \result
    * Expected output is (note this is very similar to in Msmt_Update_Obj_Trks_CCA_default):
    *    - vcs_position: [10.085811428468116, 0.091158385978730]
    *    - vcs_velocity: [5.002059334855990, 1.980379592815318]
    *    - speed: length of updated vcs_velocity vector
    *    - vcs_heading: not updated
    *    - vcs_accel: [0.098941016521831, 0.096558496272075]
    *    - tang_accel: scalar product of updated vcs_accel with unit vector in the same direction as updated vcs_velocity
    * 
    *    - pointing yaw rate filter has not run => pointing is unchanged
    *    - yaw_rate: 0
    *    - curvature: not updated
    */
   const float32_t exp_vcs_position_x = 10.085811428468116F;
   const float32_t exp_vcs_position_y = 0.09115837;
   const float32_t exp_vcs_vel_x = 5.002059334855990F;
   const float32_t exp_vcs_vel_y = 1.980379592815318F;
   const float32_t exp_speed = F360_Get_Hypotenuse(exp_vcs_vel_x, exp_vcs_vel_y);

   Angle exp_hdg;
   exp_hdg.Value(F360_Atan2f(exp_vcs_vel_y, exp_vcs_vel_x)).Normalize();
   DOUBLES_EQUAL_TEXT(exp_vcs_position_x, object_track.vcs_position.x, test_pass_th, "VCS x postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_position_y, object_track.vcs_position.y, test_pass_th, "VCS y postion is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_x, object_track.vcs_velocity.longitudinal, test_pass_th, "VCS x velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_vcs_vel_y, object_track.vcs_velocity.lateral, test_pass_th, "VCS y velocity is unexpected");
   DOUBLES_EQUAL_TEXT(exp_speed, object_track.speed, test_pass_th, "Speed is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg.Value(), object_track.vcs_heading.Value(), test_pass_th, "VCS heading is unexpectedly updated");

}

/** \defgroup  f360_update_measurement_covariances_for_long_range_crossing_object
 *  @{
 */

/** \brief
 * This test group is for testing the function Update_Measurement_Covariances_For_Long_Range_Crossing_Object()
 */
TEST_GROUP(f360_update_measurement_covariances_for_long_range_crossing_object)
{
   float32_t object_pos_x;
   float32_t object_longitudinal_velocity;
   float32_t object_lateral_velocity;

   /** \setup
    * Setup the object longitudinal, lateral, x position to be valid for being considered as long range crossing object
   */
   TEST_SETUP()
   {
      object_longitudinal_velocity = 2.0F;
      object_lateral_velocity = 13.0F;
      object_pos_x = 61.0F;
   }
};

/** \purpose
 *  Verify that when an object has properties related to a long range crossing object, then measurement covariance (for Pseudo pos Y and ranage rates) increase factors
 *  are calcuated as expected. In this test, the setup object is a valid long range crossing object, therefore,
 *  the resulting meascov will increase in measuremnt update after this function call
 * \req NA
 */
TEST(f360_update_measurement_covariances_for_long_range_crossing_object, Check_Meascov_Is_Increased_When_Object_Satisfies_Position_And_Velocity_Thresholds)
{
   /** \precond
   * Use default setup from test group
   * Setup expected data for meascov incerease factors
   */
  float32_t R_pseudo_pos_y_increase_factor;
  float32_t R_rr_addition_long_range_crossing;

  float32_t exp_R_pseudo_pos_y_increase_factor = 20.0F;
  float32_t exp_R_rr_addition_long_range_crossing = 4.0F;

   /** \action
    * Call function Update_Measurement_Covariances_For_Long_Range_Crossing_Object()
    */
   Update_Measurement_Covariances_For_Long_Range_Crossing_Object(
      object_longitudinal_velocity,
      object_lateral_velocity,
      object_pos_x,
      R_pseudo_pos_y_increase_factor,
      R_rr_addition_long_range_crossing);

   /** \result
    * Check that the function output is as expected
    */
   DOUBLES_EQUAL_TEXT(exp_R_pseudo_pos_y_increase_factor, R_pseudo_pos_y_increase_factor, F360_EPSILON, "Pseuodo Pos Y meascov increase factor is incorrect");
   DOUBLES_EQUAL_TEXT(exp_R_rr_addition_long_range_crossing, R_rr_addition_long_range_crossing, F360_EPSILON, "Range Rate meascov addition factor is incorrect");
}

/** \purpose
 *  Verify that when an object has properties are not valid to be a long range crossing object, then measurement covariance (for Pseudo pos Y and ranage rates) increase factors
 *  are calcuated as expected. In this test, the setup object is not valid to be a long range crossing object, based on too high longitudinal velocity, therefore,
 *  the resulting meascov will not increase in measuremnt update function after the tested function call
 * \req NA
 */
TEST(f360_update_measurement_covariances_for_long_range_crossing_object, Check_Meascov_Is_Increased_When_Object_Does_Not_Satisfy_Longitudinal_Velocity_Thresholds)
{
   /** \precond
   * Use default setup from test group except for
   * Set object longitudinal velocity to be greater than threshold (3m/s)
   * Setup expected data for meascov incerease factors
   */
  object_longitudinal_velocity = 4.0F;

  float32_t R_pseudo_pos_y_increase_factor;
  float32_t R_rr_addition_long_range_crossing;

  float32_t exp_R_pseudo_pos_y_increase_factor = 1.0F;
  float32_t exp_R_rr_addition_long_range_crossing = 0.0F;

   /** \action
    * Call function Update_Measurement_Covariances_For_Long_Range_Crossing_Object()
    */
   Update_Measurement_Covariances_For_Long_Range_Crossing_Object(
      object_longitudinal_velocity,
      object_lateral_velocity,
      object_pos_x,
      R_pseudo_pos_y_increase_factor,
      R_rr_addition_long_range_crossing);

   /** \result
    * Check that the function output is as expected 
    */
   DOUBLES_EQUAL_TEXT(exp_R_pseudo_pos_y_increase_factor, R_pseudo_pos_y_increase_factor, F360_EPSILON, "Pseuodo Pos Y meascov increase factor is incorrect");
   DOUBLES_EQUAL_TEXT(exp_R_rr_addition_long_range_crossing, R_rr_addition_long_range_crossing, F360_EPSILON, "Range Rate meascov addition factor is incorrect");
}

/** \purpose
 *  Verify that when an object has properties are not valid to be a long range crossing object, then measurement covariance (for Pseudo pos Y and ranage rates) increase factors
 *  are calcuated as expected. In this test, the setup object is not valid to be long range crossing object, based on too low lateral velocity, therefore,
 *  the resulting meascov will not increase in measuremnt update function after the tested function call
 * \req NA
 */
TEST(f360_update_measurement_covariances_for_long_range_crossing_object, Check_Meascov_Is_Increased_When_Object_Does_Not_Satisfy_Lateral_Velocity_Thresholds)
{
   /** \precond
   * Use default setup from test group except for
   * Set object lateral velocity to be lesser than threshold (12m/s)
   * Setup expected data for meascov incerease factors
   */
  object_lateral_velocity = 11.0F;

  float32_t R_pseudo_pos_y_increase_factor;
  float32_t R_rr_addition_long_range_crossing;

  float32_t exp_R_pseudo_pos_y_increase_factor = 1.0F;
  float32_t exp_R_rr_addition_long_range_crossing = 0.0F;

   /** \action
    * Call function Update_Measurement_Covariances_For_Long_Range_Crossing_Object()
    */
   Update_Measurement_Covariances_For_Long_Range_Crossing_Object(
      object_longitudinal_velocity,
      object_lateral_velocity,
      object_pos_x,
      R_pseudo_pos_y_increase_factor,
      R_rr_addition_long_range_crossing);

   /** \result
    * Check that the function output is as expected 
    */
   DOUBLES_EQUAL_TEXT(exp_R_pseudo_pos_y_increase_factor, R_pseudo_pos_y_increase_factor, F360_EPSILON, "Pseuodo Pos Y meascov increase factor is incorrect");
   DOUBLES_EQUAL_TEXT(exp_R_rr_addition_long_range_crossing, R_rr_addition_long_range_crossing, F360_EPSILON, "Range Rate meascov addition factor is incorrect");
}

/** \purpose
 *  Verify that when an object has properties are not valid to be a long range crossing object, then measurement covariance (for Pseudo pos Y and ranage rates) increase factors
 *  are calcuated as expected. In this test, the setup object is not valid to be a long range crossing object, based on closer longitudinal position, therefore,
 *  the resulting meascov will not increase in measuremnt update function after the tested function call
 * \req NA
 */
TEST(f360_update_measurement_covariances_for_long_range_crossing_object, Check_Meascov_Is_Increased_When_Object_Does_Not_Satisfy_Position_Thresholds)
{
   /** \precond
   * Use default setup from test group except for
   * Set object x position velocity to be lesser than threshold (60m)
   * Setup expected data for meascov incerease factors
   */
  object_pos_x = 59.0F;

  float32_t R_pseudo_pos_y_increase_factor;
  float32_t R_rr_addition_long_range_crossing;

  float32_t exp_R_pseudo_pos_y_increase_factor = 1.0F;
  float32_t exp_R_rr_addition_long_range_crossing = 0.0F;

   /** \action
    * Call function Update_Measurement_Covariances_For_Long_Range_Crossing_Object()
    */
   Update_Measurement_Covariances_For_Long_Range_Crossing_Object(
      object_longitudinal_velocity,
      object_lateral_velocity,
      object_pos_x,
      R_pseudo_pos_y_increase_factor,
      R_rr_addition_long_range_crossing);

   /** \result
    * Check that the function output is as expected
    */
   DOUBLES_EQUAL_TEXT(exp_R_pseudo_pos_y_increase_factor, R_pseudo_pos_y_increase_factor, F360_EPSILON, "Pseuodo Pos Y meascov increase factor is incorrect");
   DOUBLES_EQUAL_TEXT(exp_R_rr_addition_long_range_crossing, R_rr_addition_long_range_crossing, F360_EPSILON, "Range Rate meascov addition factor is incorrect");
}
/** @}*/
