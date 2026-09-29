/** \file
 * This file contains unit tests for content of f360_time_update_object_tracks_CCA.cpp file
 */

#include "f360_time_update_object_tracks_CCA.h"
#include <CppUTest/TestHarness.h>
#include "f360_iterator.h"
#include "f360_object_track_equal_operator.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;


/** \defgroup  f360_time_update_object_tracks_CCA__Compute_Q_For_Slow_Moving_Old_Object
 *  @{
 */

/** \brief
 * This test group is testing the function Compute_Q_For_Slow_Moving_Old_Object().
 */
TEST_GROUP(f360_time_update_object_tracks_CCA__Compute_Q_For_Slow_Moving_Old_Object)
{   
   F360_Calibrations_T calibs;
   F360_Object_Track_T obj;

   const float32_t T = 0.05F;
   const float32_t T2 = T * T;
   const float32_t T3 = T2 * T;
   const float32_t T4 = T3 * T;
   const float32_t T5 = T4 * T;

   float32_t output_Q[STATE_DIMENSION][STATE_DIMENSION];

   const float32_t test_pass_th = 1e-6F;

   
   /** \setup
    * Set up common test data used in the test in the test group:
    *    - Setup tracker calibrations:
    *       - calibs.q_cca_pos_low_speed = 0.3
    *       - calibs.q_cca_vel = 0.0
    *       - calibs.q_cca_acc_low_speed = 0.05
    *    - Clear output_Q to use for old objects with low speed (i.e. set to 0)
    *    - elapsed time: 0.05s
    */
   TEST_SETUP()
   {
      calibs.q_cca_pos_low_speed = 0.3F;
      calibs.q_cca_vel_low_speed = 0.0F;
      calibs.q_cca_acc_low_speed = 0.05F;

      std::fill(cmn::begin(output_Q), cmn::end(output_Q), 0.0F);
   }
};


/** \purpose  
 * This test checks if the output of Pre_Compute_Q_For_Slow_Moving_Old_Object() function is the default Q for slow moving objects.
 * \req
 * NA.
 */
TEST(f360_time_update_object_tracks_CCA__Compute_Q_For_Slow_Moving_Old_Object, Test_Compute_Q_For_Slow_Moving_Old_Object)
{
   /** \precond
    * Default test setup can be used 
    */
   
   /** \action
    * Call Compute_Process_Noise_Covariance_CCA().
    */
   Compute_Q_For_Slow_Moving_Object(calibs, T, T2, T3, T4, T5, output_Q);

   /** \result
    * Expected output is default_low_speed_Q
    */
   const float32_t expected_Q[STATE_DIMENSION][STATE_DIMENSION] = {{0.015000000781250F, 3.90625e-08F, 1.04375e-06F, 0.0F, 0.0F, 0.0F},
   {3.90625e-08F, 2.08125e-06F, 6.25e-5F, 0.0F, 0.0F, 0.0F},
   {1.04375e-06F, 6.25e-5F, 0.0025F, 0.0F, 0.0F, 0.0F},
   {0.0F, 0.0F, 0.0F, 0.015000000781250F, 3.90625e-08F, 1.04375e-06F},
   {0.0F, 0.0F, 0.0F, 3.90625e-08F, 2.08125e-06F, 6.25e-5F},
   {0.0F, 0.0F, 0.0F, 1.043750000000000e-06F, 6.25e-5F, 0.0025F}};

   for(uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx++)
   {
      for(uint32_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx++)
      {
         DOUBLES_EQUAL(expected_Q[row_idx][col_idx], output_Q[row_idx][col_idx], test_pass_th);
      }
   }
}
/** @}*/

/** \defgroup  f360_time_update_object_tracks_CCA_Compute_Process_Noise_Covariance_CCA
 *  @{
 */

/** \brief
 * This test group is testing the function Compute_Process_Noise_Covariance_CCA().
 */
TEST_GROUP(f360_time_update_object_tracks_CCA_Compute_Process_Noise_Covariance_CCA)
{   
   F360_Calibrations_T calibs;
   F360_Object_Track_T obj;

   float32_t T;
   float32_t T2;
   float32_t T3;
   float32_t T4;
   float32_t T5;

   float32_t default_low_speed_Q[STATE_DIMENSION][STATE_DIMENSION];

   float32_t Q[STATE_DIMENSION][STATE_DIMENSION];

   const float32_t test_pass_th = 1e-6F;

   
   /** \setup
    * Set up common test data used in the test in the test group:
    *    - Default tracker calibrations
    *    - Default Q to use for old objects with low speed
    *    - An object with:
    *       - speed just below calibs.k_cca_low_speed_th_to_ramp_down_proceess_noise
    *       - bbox orientation: 0deg
    *    - elapsed time: 0.05s
    *    - Clear process noise covariance matrix Q
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      for(uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx++)
      {
         for(uint32_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx++)
         {
            default_low_speed_Q[row_idx][col_idx] = static_cast<float32_t>(row_idx * col_idx);
         }
      }
 
      obj.speed = calibs.k_cca_low_speed_th_to_ramp_down_proceess_noise - 1e-3F;
      obj.bbox.Set_Orientation(0.0F);

      T = 0.05F;
      T2 = T * T;
      T3 = T2 * T;
      T4 = T3 * T;
      T5 = T4 * T;

      std::fill(cmn::begin(Q), cmn::end(Q), 0.0F);
   }
};


/** \purpose  
 * This test checks if the output of Compute_Process_Noise_Covariance_CCA() function the default Q when object.num_updates_since_init is large and object speed is small
 * and VCS and TCS have same orientation.
 * \req
 * NA.
 */
TEST(f360_time_update_object_tracks_CCA_Compute_Process_Noise_Covariance_CCA, Compute_Process_Noise_Covariance_CCA_slow_speed_no_rotation)
{
   /** \precond
    * Default test setup can be used 
    */
   
   /** \action
    * Call Compute_Process_Noise_Covariance_CCA().
    */
   Compute_Process_Noise_Covariance_CCA(obj, calibs, T, T2, T3, T4, T5, default_low_speed_Q, Q);

   /** \result
    * Expected output is default_low_speed_Q
    */

   float32_t expected_Q[STATE_DIMENSION][STATE_DIMENSION];
   (void)std::copy(cmn::begin(default_low_speed_Q), cmn::end(default_low_speed_Q), cmn::begin(expected_Q));

   for(uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx++)
   {
      for(uint32_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx++)
      {
         DOUBLES_EQUAL(expected_Q[row_idx][col_idx], Q[row_idx][col_idx], F360_EPSILON);
      }
   }
}


/** \purpose  
 * This test checks if the output of Compute_Process_Noise_Covariance_CCA() function the default Q when object.num_updates_since_init is large and object speed is small
 * and VCS and TCS have same different orientations.
 * \req
 * NA.
 */
TEST(f360_time_update_object_tracks_CCA_Compute_Process_Noise_Covariance_CCA, Compute_Process_Noise_Covariance_CCA_large_num_updates_since_init_slow_speed_some_rotation)
{
   /** \precond
    * Default test setup can be used except for change the object bbox orientation to something non-zero (-34 degrees is used in this test)
    */
      obj.bbox.Set_Orientation(F360_DEG2RAD(-34.0F));

   /** \action
    * Call Compute_Process_Noise_Covariance_CCA().
    */
   Compute_Process_Noise_Covariance_CCA(obj, calibs, T, T2, T3, T4, T5, default_low_speed_Q, Q);

   /** \result
    * Expected output is default_low_speed_Q
    */

   float32_t expected_Q[STATE_DIMENSION][STATE_DIMENSION];
   (void)std::copy(cmn::begin(default_low_speed_Q), cmn::end(default_low_speed_Q), cmn::begin(expected_Q));

   for(uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx++)
   {
      for(uint32_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx++)
      {
         DOUBLES_EQUAL(expected_Q[row_idx][col_idx], Q[row_idx][col_idx], F360_EPSILON);
      }
   }
}


/** \purpose  
 * This test checks so that the function Compute_Process_Noise_Covariance_CCA() is functioning as expected when VCS and TCS have same orientation.
 * \req
 * NA.
 */
TEST(f360_time_update_object_tracks_CCA_Compute_Process_Noise_Covariance_CCA, Compute_Process_Noise_Covariance_CCA)
{
   /** \precond
    * Use default test setup from test group but set object speed to large (> 5m/sm/s) such that the high speed calibration is used
    */
   obj.speed = 5.01F;
   
   /** \action
    * Call Compute_Process_Noise_Covariance_CCA().
    */
   Compute_Process_Noise_Covariance_CCA(obj, calibs, T, T2, T3, T4, T5, default_low_speed_Q, Q);

   /** \result
    * Expected output is
    * Q = [0.100083567      0.00251562521   0.000417500007        0                0                 0
    *      0.00251562521    0.1008325       0.0250000022          0                0                 0
    *      0.000417500007   0.0250000022    1                     0                0                 0
    *      0                   0                   0          0.100083262     0.00250046887    1.25250008e-05
    *      0                   0                   0          0.00250046887   0.100024976      0.000750000065
    *      0                   0                   0          1.25250008e-05  0.000750000065   0.0300000012]
    * 
    */

    const float32_t exp_Q[STATE_DIMENSION][STATE_DIMENSION] =
            {{0.100083567F,   0.00251562521F,   0.000417500007F,   0.0F,            0.0F,             0.0F},
            {0.00251562521F,  0.1008325F,       0.0250000022F,     0.0F,            0.0F,             0.0F},
            {0.000417500007F, 0.0250000022F,    1.0F,              0.0F,            0.0F,             0.00F},
            {0.0F,            0.0F,             0.0F,              0.100083262F,    0.00250046887F,   1.25250008e-05F},
            {0.0F,            0.0F,             0.0F,              0.00250046887F,  0.100024976F,     0.000750000065F},
            {0.0F,            0.0F,             0.0F,              1.25250008e-05F, 0.000750000065F,  0.0300000012F}};



   for(uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx++)
   {
      for(uint32_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx++)
      {
         DOUBLES_EQUAL(exp_Q[row_idx][col_idx], Q[row_idx][col_idx], test_pass_th);
      }
   }
}

/** \purpose  
 * This test checks so that the function Compute_Process_Noise_Covariance_CCA() is functioning as expected when
 * VCS and TCS have different orientation.
 * \req
 * NA.
 */
TEST(f360_time_update_object_tracks_CCA_Compute_Process_Noise_Covariance_CCA, Compute_Process_Noise_Covariance_CCA_some_rotation)
{
   /** \precond
    * Use default test setup from test group except
    *    - set object bbox orientation to -34deg
    *    - set object speed to large (> 1.5m/s such that we don't use the default Q matrix)
    */
   obj.bbox.Set_Orientation(F360_DEG2RAD(-34.0F));
   obj.speed = 1.51F;
   
   /** \action
    * Call Compute_Process_Noise_Covariance_CCA().
    */
   Compute_Process_Noise_Covariance_CCA(obj, calibs, T, T2, T3, T4, T5, default_low_speed_Q, Q);

   /** \result
    * Expected output is (q_cca_acc_para = 0 and no TCS to VCS rotation due to to coordinate systems having same orientation)
    * Q = [0.0350005329     5.955917e-8          1.04611593e-6    0.0              -5.73621151e-11  -1.153261226e-9
    *      5.955917e-8      2.90229286e-06       6.26416804e-05   -5.73621151e-11  -3.05612957e-09  -9.17698344e-08
    *      1.04611593e-6F   6.26416804e-05       0.00250566704    -1.53261226e-09  -9.17698344e-08  -3.67080793e-06
    *      0.0              -0.000017384697273   -1.53261226e-09  0.0350005329     5.95128249e-08    1.04487754e-06
    *     -5.73621151e-11F  -5.73621151e-11      -9.17698344e-08  5.95128249e-08   2.89982313e-06   6.25675239e-05
    *     -1.153261226e-9F  -9.17698344e-08      -3.67080793e-06  1.04487754e-06   6.25675239e-05    0.00250270078]
    *
    */

   const float32_t exp_Q[STATE_DIMENSION][STATE_DIMENSION] =
            {{0.0350005329F,   5.955917e-8F,     1.04611593e-6F,   0.0F,             -5.73621151e-11F, -1.153261226e-9F},
             {5.955917e-8F,    2.90229286e-06F,  6.26416804e-05F,  -5.73621151e-11F, -3.05612957e-09,  -9.17698344e-08F},
            {1.04611593e-6F,   6.26416804e-05F,  0.00250566704F,   -1.53261226e-09F, -9.17698344e-08F, -3.67080793e-06F},
            {0.0F,             -5.73621151e-11F, -1.53261226e-09F, 0.0350005329F,    5.95128249e-08F,  1.04487754e-06F},
            {-5.73621151e-11F, -3.05612957e-09F, -9.17698344e-08F, 5.95128249e-08F,  2.89982313e-06F,  6.25675239e-05F},
            {-1.153261226e-9F, -9.17698344e-08F, -3.67080793e-06F, 1.04487754e-06F,  6.25675239e-05F,  0.00250270078F}};



   for(uint32_t row_idx = 0U; row_idx < STATE_DIMENSION; row_idx++)
   {
      for(uint32_t col_idx = 0U; col_idx < STATE_DIMENSION; col_idx++)
      {
         DOUBLES_EQUAL(exp_Q[row_idx][col_idx], Q[row_idx][col_idx], test_pass_th);
      }
   }
}
/** @}*/


/** \defgroup  f360_time_update_object_tracks_CCA_Time_Update_Pointing_Heading_Rate_CCA
 *  @{
 */

/** \brief
 * This test group is testing the function Time_Update_Pointing_Heading_Rate_CCA().
 */
TEST_GROUP(f360_time_update_object_tracks_Time_Update_Pointing_Heading_Rate_CCA)
{   
   F360_Calibrations_T calibs;
   F360_Object_Track_T obj;

   float32_t T;
   float32_t T2;
   float32_t T3;


   const float32_t test_pass_th = 1e-6F;

   
   /** \setup
    * Set up common test data used in the test in the test group:
    *    - Default tracker calibrations
    *    - An object with:
    *        - reference point: REAR_RIGHT
    *        - vcs_position: [32m, -3m]
    *        - length: 6m
    *        - width: 3m
    *        - bbox orientation: -71deg.
    *        - bbox center: given by above parameters
    *        - heading rate: -0.13rad/s
    *        - cca_pnt_filter_cov to a random covariance matrix with the [0][0] element smaller than calibs.init_cca_pnt_filter_cov[0][0] ([0.020225751668102, 0.009790269323935; 0.009790269323935, 0.010469729906989] is used in this test)
    *        - vcs_heading: 7degrees smaller than bbbox orientation (i.e -78deg)
    *        - hdg_pnt_disagreement: the difference between object pointing and object heading
    *        - speed: large (i.e. larger than 4.0F. 4.0F + 1 is used in this test)
    *    - elapsed time: 0.05s
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
 

      obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
      obj.vcs_position.x = 32.0F;
      obj.vcs_position.y = -3.0F;
      obj.bbox.Set_Length(6.0F);
      obj.bbox.Set_Width(3.0F);
      obj.bbox.Set_Orientation(F360_DEG2RAD(-71.0F));
      obj.Update_Bbox_Center();
      obj.heading_rate = -0.13F;
      obj.cca_pnt_filter_cov[0][0] = 0.020225751668102F;
      obj.cca_pnt_filter_cov[1][0] = 0.009790269323935F;
      obj.cca_pnt_filter_cov[0][1] =  obj.cca_pnt_filter_cov[1][0];
      obj.cca_pnt_filter_cov[1][1] =  0.010469729906989F;
      obj.vcs_heading.Value(F360_DEG2RAD(-78.0F));
      obj.hdg_ptng_disagmt = obj.bbox.Get_Orientation().Value() - obj.vcs_heading.Value();
      obj.speed = 4.0F + 1.0F;

      T = 0.05F;
      T2 = T * T;
      T3 = T2 * T;
   }
};

/** \purpose  
 * This test checks so that the function Time_Update_Pointing_Heading_Rate_CCA() is functioning as expected for the default case 
 * where the error covariance for the pointing is small (i.e when obj.cca_pnt_filter_cov[0][0] < calibs.init_cca_pnt_filter_cov[0][0])
 * and the object speed is large (i.e larger than 4.0F)
 * \req
 * NA.
 */
TEST(f360_time_update_object_tracks_Time_Update_Pointing_Heading_Rate_CCA, Time_Update_Pointing_Heading_Rate_CCA_default)
{
   /** \precond
    * Use default test setup from test group.
    */
   
   /** \action
    * Extract object heading rate before function call so that we can compare after function call
    * Extract object bbox center before function call so that we can compare after function call
    * Call Time_Update_Pointing_Heading_Rate_CCA().
    */
   const float32_t heading_rate_before = obj.heading_rate;
   const Point bbox_center_before = obj.bbox.Get_Center();
   Time_Update_Pointing_Heading_Rate_CCA(T, T2, T3, calibs, obj);

   /** \result
    * Expected output is
    * object bbox orientation: -1.245683768915974
    * object heading rate: same as before function call
    * objet boox center: should have been changed compared to before function call
    * object heading pointing disagreement: new updated bbox orientation - vcs_heading
    * object cca_pnt_filter_cov: [0.021231369175263, 0.010326255819285; 0.010326255819285, 0.010969729906989]
    */

   const float32_t exp_pnt = -1.245683768915974F;
   const float32_t exp_hdg_pnt_disagreement = obj.bbox.Get_Orientation().Value() - obj.vcs_heading.Value();
   const float32_t exp_cca_pnt_filter_cov[2][2] =
            {{0.0212330341752630F, 0.0103762558192845F},
            {0.0103762558192845F, 0.0129697299069890F}};

   DOUBLES_EQUAL_TEXT(exp_pnt, obj.bbox.Get_Orientation().Value(), test_pass_th, "Object pointing is unexpected");
   CHECK_TRUE_TEXT(std::abs(heading_rate_before- obj.heading_rate) < F360_EPSILON, "heading rate has unexpectedly been modified");
   CHECK_TRUE_TEXT(std::abs(bbox_center_before.x -obj.bbox.Get_Center().x) > test_pass_th, " Boox center x coordinate has not been updated");
   CHECK_TRUE_TEXT(std::abs(bbox_center_before.y -obj.bbox.Get_Center().y) > test_pass_th, " Boox center y coordinate has not been updated");
   DOUBLES_EQUAL_TEXT(exp_hdg_pnt_disagreement, obj.hdg_ptng_disagmt, test_pass_th, "Object heading pointing disagreement is unexpected");
   for(uint32_t row_idx = 0U; row_idx < 2U; row_idx++)
   {
      for(uint32_t col_idx = 0U; col_idx < 2U; col_idx++)
      {
         DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[row_idx][col_idx], obj.cca_pnt_filter_cov[row_idx][col_idx], test_pass_th, "Object cca_pnt_filter_cov is unexpected");
      }
   }
}


/** \purpose  
 * This test checks so that the function Time_Update_Pointing_Heading_Rate_CCA() is functioning as expected for the case 
 * where the error covariance for the pointing is small (i.e when obj.cca_pnt_filter_cov[0][0] < calibs.init_cca_pnt_filter_cov[0][0])
 * and the object speed is small (i.e smaller than 4.0F)
 * \req
 * NA.
 */
TEST(f360_time_update_object_tracks_Time_Update_Pointing_Heading_Rate_CCA, Time_Update_Pointing_Heading_Rate_CCA_small_speed)
{
   /** \precond
    * Use default test setup from test group except for
    *    - change the object speed to be smaller than 4.0F. Set it to 4.0F - 1
    */
   obj.speed = 4.0F -1.0F;
   
   /** \action
    * Extract object heading rate before function call so that we can compare after function call
    * Extract object bbox center before function call so that we can compare after function call
    * Call Time_Update_Pointing_Yheading_Rate_CCA().
    */
   const float32_t heading_rate_before = obj.heading_rate;
   const Point bbox_center_before = obj.bbox.Get_Center();
   Time_Update_Pointing_Heading_Rate_CCA(T, T2, T3, calibs, obj);

   /** \result
    * Expected output is (Note: The expected output is the same as for the default test Time_Update_Pointing_Heading_Rate_CCA_default except for cca_pnt_filter_cov which is different)
    * object bbox orientation: -1.245683768915974
    * object heading rate: same as before function call
    * objet boox center: should have been changed compared to before function call
    * object heading pointing disagreement: new updated bbox orientation - vcs_heading
    * object cca_pnt_filter_cov: [0.021231187065888, 0.010320787069285; 0.010320787069285, 0.010750979906989]
    */

   const float32_t exp_pnt = -1.245683768915974F;
   const float32_t exp_hdg_pnt_disagreement = obj.bbox.Get_Orientation().Value() - obj.vcs_heading.Value();
   const float32_t exp_cca_pnt_filter_cov[2][2] =
            {{0.0212968049760442F, 0.0122912948817845F},
            {0.0122912948817845F, 0.0895712924069890F}};

   DOUBLES_EQUAL_TEXT(exp_pnt, obj.bbox.Get_Orientation().Value(), test_pass_th, "Object pointing is unexpected");
   CHECK_TRUE_TEXT(std::abs(heading_rate_before- obj.heading_rate) < F360_EPSILON, "Heading rate has unexpectedly been modified");
   CHECK_TRUE_TEXT(std::abs(bbox_center_before.x -obj.bbox.Get_Center().x) > test_pass_th, " Boox center x coordinate has not been updated");
   CHECK_TRUE_TEXT(std::abs(bbox_center_before.y -obj.bbox.Get_Center().y) > test_pass_th, " Boox center y coordinate has not been updated");
   DOUBLES_EQUAL_TEXT(exp_hdg_pnt_disagreement, obj.hdg_ptng_disagmt, test_pass_th, "Object heading pointing disagreement is unexpected");
   for(uint32_t row_idx = 0U; row_idx < 2U; row_idx++)
   {
      for(uint32_t col_idx = 0U; col_idx < 2U; col_idx++)
      {
         DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[row_idx][col_idx], obj.cca_pnt_filter_cov[row_idx][col_idx], test_pass_th, "Object cca_pnt_filter_cov is unexpected");
      }
   }
}


/** \purpose  
 * This test checks so that the function Time_Update_Pointing_Yheading_Rate_CCA() is functioning as expected for the case 
 * where the error covariance for the pointing is large (i.e when obj.cca_pnt_filter_cov[0][0] > calibs.init_cca_pnt_filter_cov[0][0])
 * \req
 * NA.
 */
TEST(f360_time_update_object_tracks_Time_Update_Pointing_Heading_Rate_CCA, Time_Update_Pointing_Heading_Rate_CCA_large_P_0_0)
{
   /** \precond
    * Use default test setup from test group except for
    *    - set object cca_pnt_filter_cov[0][0] to calibs.init_cca_pnt_filter_cov[0][0] + 0.01
    */
   obj.cca_pnt_filter_cov[0][0] = calibs.init_cca_pnt_filter_cov[0][0] + 0.01F;

   /** \action
    * Extract object heading rate before function call so that we can compare after function call
    * Extract object bbox center before function call so that we can compare after function call
    * Extract object cca_pnt_filter_cov before function call so that we can compare after function call
    * Call Time_Update_Pointing_Heading_Rate_CCA().
    */
   const float32_t heading_rate_before = obj.heading_rate;
   const Point bbox_center_before = obj.bbox.Get_Center();
   const float32_t cca_pnt_filter_cov_before[2][2] = {{obj.cca_pnt_filter_cov[0][0], obj.cca_pnt_filter_cov[0][1]},{obj.cca_pnt_filter_cov[1][0], obj.cca_pnt_filter_cov[1][1]}};
   Time_Update_Pointing_Heading_Rate_CCA(T, T2, T3, calibs, obj);

   /** \result
    * Expected output is
    * object bbox orientation: -1.245683768915974
    * object heading rate: same as before function call
    * objet boox center: should have been changed compared to before function call
    * object heading pointing disagreement: new updated bbox orientation - vcs_heading
    * object cca_pnt_filter_cov: is unchanged compared to before function call
    */

   const float32_t exp_pnt = -1.245683768915974F;
   const float32_t exp_hdg_pnt_disagreement = obj.bbox.Get_Orientation().Value() - obj.vcs_heading.Value();

   DOUBLES_EQUAL_TEXT(exp_pnt, obj.bbox.Get_Orientation().Value(), test_pass_th, "Object pointing is unexpected");
   CHECK_TRUE_TEXT(std::abs(heading_rate_before- obj.heading_rate) < F360_EPSILON, "heading rate has unexpectedly been modified");
   CHECK_TRUE_TEXT(std::abs(bbox_center_before.x -obj.bbox.Get_Center().x) > test_pass_th, " Boox center x coordinate has not been updated");
   CHECK_TRUE_TEXT(std::abs(bbox_center_before.y -obj.bbox.Get_Center().y) > test_pass_th, " Boox center y coordinate has not been updated");
   DOUBLES_EQUAL_TEXT(exp_hdg_pnt_disagreement, obj.hdg_ptng_disagmt, test_pass_th, "Object heading pointing disagreement is unexpected");
   for(uint32_t row_idx = 0U; row_idx < 2U; row_idx++)
   {
      for(uint32_t col_idx = 0U; col_idx < 2U; col_idx++)
      {
         DOUBLES_EQUAL_TEXT(cca_pnt_filter_cov_before[row_idx][col_idx], obj.cca_pnt_filter_cov[row_idx][col_idx], test_pass_th, "Object cca_pnt_filter_cov is unexpected");
      }
   }
}
/** @}*/


/** \defgroup  f360_time_update_object_tracks_CCA_Time_Update_Object_Tracks_CCA
 *  @{
 */

/** \brief
 * This test group is testing the function Time_Update_Object_Tracks_CCA().
 */
TEST_GROUP(f360_time_update_object_tracks_Time_Update_Object_Tracks_CCA)
{   
   F360_Calibrations_T calibs;
   F360_Object_Track_T objects[NUMBER_OF_OBJECT_TRACKS];
   F360_Tracker_Info_T tracker_info;
   F360_TRKR_TIMING_INFO_T timing_info;
   float32_t T;
   float32_t T2;
   float32_t T3;


   const float32_t test_pass_th = 1e-6F;
   const float32_t test_pass_th_reduced = 1e-2;

   
   /** \setup
    * Set up common test data used in the test in the test group:
    *    - Default tracker calibrations
    *    - Two active objects with same properties:
    *        - filter type CCA
    *        - vcs_position: [32, -3] [m]
    *        - vcs_velocity: [10.5, -6.06] [m/s]
    *        - speed: length of vcs_velocity vector
    *        - vcs_heading: direction of vcs_velocity vector (i.e ~-40deg)
    *        - bbox orientation: -34deg (i.e ~6 degrees larger than vcs_heading)
    *        - heading_rate: -0.13rad/s
    *        - curvature: heading_rate / speed
    *        - vcs_accel: [-0.23, 1.1] [m/s^2]
    *        - tang_accel: scalar product of vcs_accel and a unit vector in the direction of vcs_velocity
    *        - errcov: random covariance matrix ([2.460347738999359   0.435117407495604  -0.837776193601800   1.410379738048934  -0.261472923364090  -0.474531870128779
    *                                             0.435117407495604   1.957923613456870  -0.051647783989833  -0.760275343318476  -0.153150947536924   0.972480109948760
    *                                             -0.837776193601800  -0.051647783989833   2.544691648106593  -1.229003618363466  -0.804773651774034   1.765582859251130
    *                                             1.410379738048934  -0.760275343318476  -1.229003618363466   2.258567624860257  -0.774062272756955  -1.261605788558066
    *                                             -0.261472923364090  -0.153150947536924  -0.804773651774034  -0.774062272756955   2.221067920133194  -0.994649670494953
    *                                             -0.474531870128779   0.972480109948760   1.765582859251130  -1.261605788558066  -0.994649670494953   2.124040473688151] is uded in this test)
    *        - cca_pnt_filter_cov to a random covariance matrix ([0.020225751668102, 0.009790269323935; 0.009790269323935, 0.010469729906989] is used in this test)
    *        - num_updates_since_init: 255
    *        - f_moving: true
    *    - elapsed time: 0.05s
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      tracker_info.num_active_objs = 2;
      tracker_info.active_obj_ids[0] = 44;
      tracker_info.active_obj_ids[1] = 17;

      Fill_Default_Object_Data(tracker_info.active_obj_ids[0]-1, objects);
      Fill_Default_Object_Data(tracker_info.active_obj_ids[1]-1, objects);


      T = 0.05F;
      T2 = T * T;
      T3 = T2 * T;
   }

   void Fill_Default_Object_Data(const int32_t obj_idx, F360_Object_Track_T (&objects)[NUMBER_OF_OBJECT_TRACKS])
   {
      objects[obj_idx].trk_fltr_type = F360_TRACKER_TRKFLTR_CCA;
      objects[obj_idx].vcs_position.x = 32.0F;
      objects[obj_idx].vcs_position.y = -3.0F;
      objects[obj_idx].vcs_velocity.longitudinal = 10.5F;
      objects[obj_idx].vcs_velocity.lateral = -6.06F;
      objects[obj_idx].speed = F360_Get_Hypotenuse(objects[obj_idx].vcs_velocity.longitudinal, objects[obj_idx].vcs_velocity.lateral);
      (void) objects[obj_idx].vcs_heading.Value(F360_Atan2f(objects[obj_idx].vcs_velocity.lateral, objects[obj_idx].vcs_velocity.longitudinal)).Normalize();
      objects[obj_idx].bbox.Set_Orientation(F360_DEG2RAD(-34.0F));
      objects[obj_idx].heading_rate = -0.13F;
      objects[obj_idx].curvature = objects[obj_idx].heading_rate / objects[obj_idx].speed;
      objects[obj_idx].vcs_accel.longitudinal = -0.23F;
      objects[obj_idx].vcs_accel.lateral = 1.1F;
      objects[obj_idx].tang_accel = objects[obj_idx].vcs_accel.longitudinal * objects[obj_idx].vcs_heading.Cos() + objects[obj_idx].vcs_accel.lateral * objects[obj_idx].vcs_heading.Sin();
      
      objects[obj_idx].errcov[0][0] = 2.460347738999359F;
      objects[obj_idx].errcov[0][1] = 0.435117407495604F;
      objects[obj_idx].errcov[0][2] = -0.837776193601800F;
      objects[obj_idx].errcov[0][3] = 1.410379738048934F;
      objects[obj_idx].errcov[0][4] = -0.261472923364090F;
      objects[obj_idx].errcov[0][5] = -0.474531870128779F;

      objects[obj_idx].errcov[1][0] = objects[obj_idx].errcov[0][1];
      objects[obj_idx].errcov[1][1] = 1.957923613456870F;
      objects[obj_idx].errcov[1][2] = -0.051647783989833F;
      objects[obj_idx].errcov[1][3] = -0.760275343318476F;
      objects[obj_idx].errcov[1][4] = -0.153150947536924F;
      objects[obj_idx].errcov[1][5] = 0.972480109948760;

      objects[obj_idx].errcov[2][0] = objects[obj_idx].errcov[0][2];
      objects[obj_idx].errcov[2][1] = objects[obj_idx].errcov[1][2];
      objects[obj_idx].errcov[2][2] = 2.544691648106593F;
      objects[obj_idx].errcov[2][3] = -1.229003618363466F;
      objects[obj_idx].errcov[2][4] = -0.804773651774034F;
      objects[obj_idx].errcov[2][5] = 1.765582859251130F;

      objects[obj_idx].errcov[3][0] = objects[obj_idx].errcov[0][3];
      objects[obj_idx].errcov[3][1] = objects[obj_idx].errcov[1][3];;
      objects[obj_idx].errcov[3][2] = objects[obj_idx].errcov[2][3];;
      objects[obj_idx].errcov[3][3] = 2.258567624860257F;
      objects[obj_idx].errcov[3][4] = -0.774062272756955F;
      objects[obj_idx].errcov[3][5] = -1.261605788558066F;

      objects[obj_idx].errcov[4][0] = objects[obj_idx].errcov[0][4];
      objects[obj_idx].errcov[4][1] = objects[obj_idx].errcov[1][4];
      objects[obj_idx].errcov[4][2] = objects[obj_idx].errcov[2][4];
      objects[obj_idx].errcov[4][3] = objects[obj_idx].errcov[3][4];
      objects[obj_idx].errcov[4][4] = 2.221067920133194F;
      objects[obj_idx].errcov[4][5] = -0.994649670494953F;

      objects[obj_idx].errcov[5][0] = objects[obj_idx].errcov[0][5];
      objects[obj_idx].errcov[5][1] = objects[obj_idx].errcov[1][5];
      objects[obj_idx].errcov[5][2] = objects[obj_idx].errcov[2][5];
      objects[obj_idx].errcov[5][3] = objects[obj_idx].errcov[3][5];
      objects[obj_idx].errcov[5][4] = objects[obj_idx].errcov[4][5];
      objects[obj_idx].errcov[5][5] = 2.124040473688151F;
      
      objects[obj_idx].cca_pnt_filter_cov[0][0] = 0.020225751668102F;
      objects[obj_idx].cca_pnt_filter_cov[1][0] = 0.009790269323935F;
      objects[obj_idx].cca_pnt_filter_cov[0][1] =  objects[obj_idx].cca_pnt_filter_cov[1][0];
      objects[obj_idx].cca_pnt_filter_cov[1][1] =  0.010469729906989F;
      
      objects[obj_idx].num_updates_since_init = 255U;
      objects[obj_idx].f_moving = true;
      objects[obj_idx].movable_prob = 1.0F;
   }

   void Check_Default_Expected_Data(const F360_Object_Track_T &obj, const float32_t pnt_before)
   {
      const float32_t exp_vcs_pos_x = 32.5247125F;
      const float32_t exp_vcs_pos_y = -3.301625F;
      const float32_t exp_vcs_vel_x = 10.4885F;
      const float32_t exp_vcs_vel_y = -6.005F;
      const float32_t exp_vcs_acc_x = -0.23F;
      const float32_t exp_vcs_acc_y = 1.1F;
      const float32_t exp_errcov[STATE_DIMENSION][STATE_DIMENSION] = {{9.342608380002178F, 0.753412955683592F, -6.451159349691672F, 7.962695283895773F, -1.296294526168591F, -4.498342564199979F},
                                    {0.143241662199810F, 5.682984689774070F, 2.844053569539536F, -4.069396969926079F, -1.312586130799813F, 5.302714382762018},
                                    {-6.575605886546620F, 2.221284300263434F, 14.204913297621124F, -8.650528210759502F, -4.441018624452979F, 9.828723771088221F},
                                    {8.087602741135715F, -3.685464442929863F, -8.857511685704591F, 11.385370350783196F, -1.118849760082756F, -8.562705152128775F},
                                    {-1.473729659929264F, -0.852752841430484F, -3.893951404624561F, -1.892803967932365F, 7.022282387538024F, -4.200755668128804F},
                                    {-4.751192378624221F, 4.755647162933600F, 9.828723771088221F, -8.337378396163084F, -4.769790966103783F, 12.231178047400492F}};
      const float32_t exp_speed = F360_Get_Hypotenuse(exp_vcs_vel_x, exp_vcs_vel_y);
      const float32_t exp_vcs_heading = F360_Atan2f(exp_vcs_vel_y, exp_vcs_vel_x);
      const float32_t exp_tang_accel = exp_vcs_acc_x * F360_Cosf(exp_vcs_heading) +  exp_vcs_acc_y* F360_Sinf(exp_vcs_heading);
      const float32_t exp_curv = obj.heading_rate / obj.speed;

      DOUBLES_EQUAL_TEXT(exp_vcs_pos_x, obj.vcs_position.x, test_pass_th, "Object x position is unexpected");
      DOUBLES_EQUAL_TEXT(exp_vcs_pos_y, obj.vcs_position.y, test_pass_th, "Object y position is unexpected");
      DOUBLES_EQUAL_TEXT(exp_vcs_vel_x, obj.vcs_velocity.longitudinal, test_pass_th, "Object x velocity is unexpected");
      DOUBLES_EQUAL_TEXT(exp_vcs_vel_y, obj.vcs_velocity.lateral, test_pass_th, "Object y velocity is unexpected");
      DOUBLES_EQUAL_TEXT(exp_vcs_acc_x, obj.vcs_accel.longitudinal, test_pass_th, "Object x accel is unexpected");
      DOUBLES_EQUAL_TEXT(exp_vcs_acc_y, obj.vcs_accel.lateral, test_pass_th, "Object y accel is unexpected");
      for(uint32_t row_idx = 0U; row_idx > STATE_DIMENSION; row_idx++)
      {
         for(uint32_t col_idx = 0U; col_idx > STATE_DIMENSION; col_idx++)
         {
            DOUBLES_EQUAL_TEXT(exp_errcov[row_idx][col_idx], obj.errcov[row_idx][col_idx], test_pass_th, "Object errcov is unexpected");
         }
      }
      DOUBLES_EQUAL_TEXT(exp_speed, obj.speed, test_pass_th, "Object speed is unexpected");
      DOUBLES_EQUAL_TEXT(exp_vcs_heading, obj.vcs_heading.Value(), test_pass_th, "Object vcs heading is unexpected");
      DOUBLES_EQUAL_TEXT(exp_tang_accel, obj.tang_accel, test_pass_th_reduced, "Object tangential acceleration is unexpected");
      DOUBLES_EQUAL_TEXT(exp_curv, obj.curvature, test_pass_th, "Object curvature is unexpected");
      CHECK_TRUE_TEXT(std::abs(pnt_before - obj.bbox.Get_Orientation().Value()) > test_pass_th, "Object bbox orientation has not been updated");
   }
};

/** \purpose  
 * This test checks so that the function Time_Update_Object_Tracks_CCA() is functioning as expected for 2 active CCA objects when the objects are classified as moving
 * \req
 * NA.
 */
TEST(f360_time_update_object_tracks_Time_Update_Object_Tracks_CCA, Time_Update_Object_Tracks_CCA_default_2_objs_both_CCA)
{
   /** \precond
    * Use default test setup from test group.
    */
   
   /** \action
    * Extract objects pointing before function call so that it can be used for comparision later
    * Call Time_Update_Object_Tracks_CCA().
    */
   const float32_t pnt_before_first_obj = objects[tracker_info.active_obj_ids[0]-1].bbox.Get_Orientation().Value();
   const float32_t pnt_before_sec_obj = objects[tracker_info.active_obj_ids[1]-1].bbox.Get_Orientation().Value();
   Time_Update_Object_Tracks_CCA(T, tracker_info, calibs, objects, timing_info);

   /** \result
    * Check that both objects has been updated expectedly
    */
   Check_Default_Expected_Data(objects[tracker_info.active_obj_ids[0]-1], pnt_before_first_obj);
   Check_Default_Expected_Data(objects[tracker_info.active_obj_ids[1]-1], pnt_before_sec_obj);
}

/** \purpose  
 * This test checks so that the function Time_Update_Object_Tracks_CCA() is functioning as expected when one of the two active objects are not CCA and the other is CCA
 * \req
 * NA.
 */
TEST(f360_time_update_object_tracks_Time_Update_Object_Tracks_CCA, Time_Update_Object_Tracks_CCA_default_2_objs_one_not_CCA)
{
   /** \precond
    * Use default test setup from test group except for
    *    - change the filter type of the first object to CTCA
    */
   objects[tracker_info.active_obj_ids[0]-1].trk_fltr_type = F360_TRACKER_TRKFLTR_CTCA;
   
   /** \action
    * Create a copy of the first object so that it can be used for comparision later
    * Extract objects pointing of second object before function call so that it can be used for comparision later
    * Call Time_Update_Object_Tracks_CCA().
    */
   const F360_Object_Track_T first_obj_before =  objects[tracker_info.active_obj_ids[0]-1];
   const float32_t pnt_before_sec_obj = objects[tracker_info.active_obj_ids[1]-1].bbox.Get_Orientation().Value();
   Time_Update_Object_Tracks_CCA(T, tracker_info, calibs, objects, timing_info);

   /** \result
    * Check that only the second object has been updated
    */
   CHECK_TRUE_TEXT(first_obj_before == objects[tracker_info.active_obj_ids[0]-1], "First object has been unexpectedly updated");
   Check_Default_Expected_Data(objects[tracker_info.active_obj_ids[1]-1], pnt_before_sec_obj);
}


/** \purpose  
 * This test checks so that the function Time_Update_Object_Tracks_CCA() updates the object pointing 
 * to the same value as heading and that heading rate, curvature is zero when there is one single active
 * CCA object which is stationary.
 * \req
 * NA.
 */
TEST(f360_time_update_object_tracks_Time_Update_Object_Tracks_CCA, Time_Update_Object_Tracks_CCA_default_1_stationary_obj)
{
   /** \precond
    * Use default test setup from test group except for
    *    - change to one single active object
    *    - change active object's f_moving to false
    *    - change active object's movable_prob to 0.0
    */
   tracker_info.num_active_objs = 1;
   objects[tracker_info.active_obj_ids[0]-1].f_moving = false;
   objects[tracker_info.active_obj_ids[0]-1].movable_prob = 0.0F;
   
   /** \action
    * Call Time_Update_Object_Tracks_CCA().
    */
   Time_Update_Object_Tracks_CCA(T, tracker_info, calibs, objects, timing_info);

   /** \result
    * Check that the pointing has been updated to be same as heading and that the heading rate and curvature is set to 0
    */
   DOUBLES_EQUAL_TEXT(objects[tracker_info.active_obj_ids[0]-1].vcs_heading.Value(), objects[tracker_info.active_obj_ids[0]-1].bbox.Get_Orientation().Value(), F360_EPSILON, "Poitning has been unexpectedly updated for a stationary object");
   CHECK_TRUE_TEXT(std::abs(0.0F - objects[tracker_info.active_obj_ids[0]-1].heading_rate) < F360_EPSILON, "Heading rate is unexpectedly non-zero for a stationary object");
   CHECK_TRUE_TEXT(std::abs(0.0F - objects[tracker_info.active_obj_ids[0]-1].curvature) < F360_EPSILON, "Heading rate is unexpectedly non-zero for a stationary object");
}

/** \purpose  
 * This test checks so that the function Time_Update_Object_Tracks_CCA() does not update the object pointing but that it sets heading rate and curvature to zero
 * for a single active CCA object that is classified as moving but has close to zero speed.
 * \req
 * NA.
 */
TEST(f360_time_update_object_tracks_Time_Update_Object_Tracks_CCA, Time_Update_Object_Tracks_CCA_default_obj_obj_with_close_to_zero_speed_and_f_moving)
{
   /** \precond
    * Use default test setup from test group except for
    *    - change to one single active object
    *    - change active object's f_moving to true
    *    - change active object's movable_prob to 1.0
    *    - set vx, vy, ax, & ay, such that the updated speed is lesser than 0.001 m/s
    */
   tracker_info.num_active_objs = 1;
   objects[tracker_info.active_obj_ids[0]-1].f_moving = true;
   objects[tracker_info.active_obj_ids[0]-1].movable_prob = 1.0F;
   objects[tracker_info.active_obj_ids[0]-1].vcs_velocity.lateral = 0.0001F;
   objects[tracker_info.active_obj_ids[0]-1].vcs_velocity.longitudinal = 0.0001F;
   objects[tracker_info.active_obj_ids[0]-1].vcs_accel.lateral = 0.0F;
   objects[tracker_info.active_obj_ids[0]-1].vcs_accel.longitudinal = 0.0F;

   
   /** \action
    * Call Time_Update_Object_Tracks_CCA().
    */
   const float32_t pointing_before = objects[tracker_info.active_obj_ids[0]-1].bbox.Get_Orientation().Value();
   Time_Update_Object_Tracks_CCA(T, tracker_info, calibs, objects, timing_info);

   /** \result
    * Check that the pointing has been updated to be similar to heading and that the heading rate and curvature is set to 0
    */
   DOUBLES_EQUAL_TEXT(pointing_before, objects[tracker_info.active_obj_ids[0]-1].bbox.Get_Orientation().Value(), F360_EPSILON, "Poitning has been unexpectedly updated for a stationary object");
   CHECK_TRUE_TEXT(std::abs(0.0F - objects[tracker_info.active_obj_ids[0]-1].heading_rate) < F360_EPSILON, "Heading rate is unexpectedly non-zero for a stationary object");
   CHECK_TRUE_TEXT(std::abs(0.0F - objects[tracker_info.active_obj_ids[0]-1].curvature) < F360_EPSILON, "Curvature is unexpectedly non-zero for a stationary object");
}

/** \purpose  
 * This test checks so that the function Time_Update_Object_Tracks_CCA() does not update the object pointing but that it sets heading rate and curvature to zero
 * for a single active CCA object that is classified as stationary and has close to zero speed.
 * \req
 * NA.
 */
TEST(f360_time_update_object_tracks_Time_Update_Object_Tracks_CCA, Time_Update_Object_Tracks_CCA_default_obj_with_close_to_zero_speed_and_not_f_moving)
{
   /** \precond
    * Use default test setup from test group except for
    *    - change to one single active object
    *    - change active object's f_moving to false
    *    - change active object's movable_prob to 1.0
    *    - set vx, vy, ax, & ay, such that the updated speed is lesser than 0.001 m/s
    */
   tracker_info.num_active_objs = 1;
   objects[tracker_info.active_obj_ids[0]-1].f_moving = false;
   objects[tracker_info.active_obj_ids[0]-1].movable_prob = 1.0F;
   objects[tracker_info.active_obj_ids[0]-1].vcs_velocity.lateral = 0.0001F;
   objects[tracker_info.active_obj_ids[0]-1].vcs_velocity.longitudinal = 0.0001F;
   objects[tracker_info.active_obj_ids[0]-1].vcs_accel.lateral = 0.0F;
   objects[tracker_info.active_obj_ids[0]-1].vcs_accel.longitudinal = 0.0F;

   
   /** \action
    * Call Time_Update_Object_Tracks_CCA().
    */
   const float32_t pointing_before = objects[tracker_info.active_obj_ids[0]-1].bbox.Get_Orientation().Value();
   Time_Update_Object_Tracks_CCA(T, tracker_info, calibs, objects, timing_info);

   /** \result
    * Check that the pointing has not been updated and that the heading rate and curvature is set to 0
    */
   DOUBLES_EQUAL_TEXT(pointing_before, objects[tracker_info.active_obj_ids[0]-1].bbox.Get_Orientation().Value(), F360_EPSILON, "Poitning has been unexpectedly updated for a stationary object");
   CHECK_TRUE_TEXT(std::abs(0.0F - objects[tracker_info.active_obj_ids[0]-1].heading_rate) < F360_EPSILON, "Heading rate is unexpectedly non-zero for a stationary object");
   CHECK_TRUE_TEXT(std::abs(0.0F - objects[tracker_info.active_obj_ids[0]-1].curvature) < F360_EPSILON, "Curvature is unexpectedly non-zero for a stationary object");
}

/** @}*/
