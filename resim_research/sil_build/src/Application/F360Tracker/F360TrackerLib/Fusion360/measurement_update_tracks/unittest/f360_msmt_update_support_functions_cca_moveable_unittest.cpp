/** \file
 * This file contains unit tests for content of f360_msmt_update_support_functions_cca_moveable.cpp file
 */

#include "f360_msmt_update_support_functions_cca_moveable.h"
#include <CppUTest/TestHarness.h>

#include "f360_math_func.h"
#include "f360_trk_fltr_cca_states.h"
#include "f360_pseudo_msmt.h"
#include <algorithm>

//#include "headerfile_needed.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_msmt_update_support_functions_cca_unittest_general
*  @{
*/

/** \brief
* This test group sets up test data and expected output for CCA update in the general case.
* One sensor is setup in VCS origin and oriented with bore sight aligned with VCS-long.
* Covariance matrix P and measurement noise variance matrix R are both symmetric and arbitrarily generated.
* Expected data was generated with help of the Matlab script Kalman_CCA_general_UT.m that can be found in Jira ticket DFD-1897.
*/
TEST_GROUP(f360_msmt_update_support_functions_cca_general)
{
   F360_Calibrations_T calib;
   const bool obj_ok_to_decay_state = false;
   uint32_t nr_dets;
   uint32_t nr_total_msnmts;
   float32_t h_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION] = {};
   float32_t p_mat[STATE_DIMENSION][STATE_DIMENSION] = {};
   float32_t r_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   float32_t z_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   float32_t zhat_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   float32_t state[STATE_DIMENSION];

   float32_t s_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   float32_t k_mat[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};

   float32_t exp_s_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   float32_t exp_k_mat[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   float32_t exp_p_mat[STATE_DIMENSION][STATE_DIMENSION] = {};
   float32_t exp_state[STATE_DIMENSION] = {};

   float32_t rdot_comp[3]; // Must be the size of number of detections
   float32_t azimuth[3]; // Must be the size of number of detections

   float32_t threshold = 0.0001F; // Threshold for comparing calculated and expected data

   /** \setup
   * Create data for a CCA update
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      // Setup arbitrary reasonable state vector
      state[F360_TRK_FLTR_CCA_STATE_X] = 10.0F;
      state[F360_TRK_FLTR_CCA_STATE_VX] = 5.0F;
      state[F360_TRK_FLTR_CCA_STATE_AX] = 0.1F;
      state[F360_TRK_FLTR_CCA_STATE_Y] = 0.0F;
      state[F360_TRK_FLTR_CCA_STATE_VY] = 2.0F;
      state[F360_TRK_FLTR_CCA_STATE_AY] = 0.1F;

      // Three detections
      nr_dets = 3U;
      azimuth[0] = F360_DEG2RAD(0.0F);
      azimuth[1] = F360_DEG2RAD(10.0F);
      azimuth[2] = F360_DEG2RAD(5.0F);
      rdot_comp[0] = state[F360_TRK_FLTR_CCA_STATE_VX] * F360_Cosf(azimuth[0]) + state[F360_TRK_FLTR_CCA_STATE_VY] * F360_Sinf(azimuth[0]) + 0.1F; // Add some value to force state update
      rdot_comp[1] = state[F360_TRK_FLTR_CCA_STATE_VX] * F360_Cosf(azimuth[1]) + state[F360_TRK_FLTR_CCA_STATE_VY] * F360_Sinf(azimuth[1]);
      rdot_comp[2] = state[F360_TRK_FLTR_CCA_STATE_VX] * F360_Cosf(azimuth[2]) + state[F360_TRK_FLTR_CCA_STATE_VY] * F360_Sinf(azimuth[2]) - 0.1F; // Add some value to force state update

      
                                                            
      // Set up H, Z and Zhat matrix
      //Two pseudo position measurements
      nr_total_msnmts = nr_dets + 2U;
      z_mat[0] = state[F360_TRK_FLTR_CCA_STATE_X] + 0.1F; // Pseudo position, add some value to force state update
      z_mat[1] = state[F360_TRK_FLTR_CCA_STATE_Y] + 0.1F; // Pseudo position (Yevhenii added to replicate the same thing in MATLAB file)

      h_mat[0][F360_TRK_FLTR_CCA_STATE_X] = 1.0F;
      h_mat[1][F360_TRK_FLTR_CCA_STATE_Y] = 1.0F;
      
      zhat_mat[0] = state[F360_TRK_FLTR_CCA_STATE_X];
      zhat_mat[1] = state[F360_TRK_FLTR_CCA_STATE_Y];
      for (uint32_t i = 2U; i < nr_total_msnmts; i++)
      {
         float32_t az = azimuth[i - 2];
         float32_t rdot = rdot_comp[i - 2];

         h_mat[i][F360_TRK_FLTR_CCA_STATE_VX] = F360_Cosf(az);
         h_mat[i][F360_TRK_FLTR_CCA_STATE_VY] = F360_Sinf(az);

         z_mat[i] = rdot;
         zhat_mat[i] = state[F360_TRK_FLTR_CCA_STATE_VX] * F360_Cosf(az) + state[F360_TRK_FLTR_CCA_STATE_VY] * F360_Sinf(az);
      }


      // Set up arbitrary symmetric P matrix
      // Fill upper triangle

      p_mat[0][0] = 6.5169020258062931F;
      p_mat[0][1] = 0.1945124907734175F;
      p_mat[0][2] = -0.0384556107300637F;
      p_mat[0][3] = -0.3850330754532219F;
      p_mat[0][4] = 0.1285258329421008F;
      p_mat[0][5] = 0.0938525644826565F;
      p_mat[1][0] = 0.1945124907734175F;
      p_mat[1][1] = 6.6638225932428696F;
      p_mat[1][2] = 0.1043554630575398F;
      p_mat[1][3] = 0.1661297090399247F;
      p_mat[1][4] = 0.4279077173738897F;
      p_mat[1][5] = -0.1840967228468995F;
      p_mat[2][0] = -0.0384556107300637F;
      p_mat[2][1] = 0.1043554630575398F;
      p_mat[2][2] = 6.5717516167530929F;
      p_mat[2][3] = -0.0463124667591884F;
      p_mat[2][4] = -0.0474636529957669F;
      p_mat[2][5] = 0.4057486204994333F;
      p_mat[3][0] = -0.3850330754532219F;
      p_mat[3][1] = 0.1661297090399247F;
      p_mat[3][2] = -0.0463124667591884F;
      p_mat[3][3] = 6.9483854104727634F;
      p_mat[3][4] = -0.0724591341868679F;
      p_mat[3][5] = -0.4093296840042222F;
      p_mat[4][0] = 0.1285258329421008F;
      p_mat[4][1] = 0.4279077173738897F;
      p_mat[4][2] = -0.0474636529957669F;
      p_mat[4][3] = -0.0724591341868679F;
      p_mat[4][4] = 6.4492365170366686F;
      p_mat[4][5] = -0.2768349930633138F;
      p_mat[5][0] = 0.0938525644826565F;
      p_mat[5][1] = -0.1840967228468995F;
      p_mat[5][2] = 0.4057486204994333F;
      p_mat[5][3] = -0.4093296840042222F;
      p_mat[5][4] = -0.2768349930633138F;
      p_mat[5][5] = 6.9573606188351498F;



      // Set up R matrix

      r_mat[0][0] = 0.6283736714856673F;
      r_mat[0][1] = 0.3861097256837295F;
      r_mat[0][2] = 0.0000000000000000F;
      r_mat[0][3] = 0.0000000000000000F;
      r_mat[0][4] = 0.0000000000000000F;
      r_mat[1][0] = 0.3861097256837295F;
      r_mat[1][1] = 0.2461389364340652F;
      r_mat[1][2] = 0.0000000000000000F;
      r_mat[1][3] = 0.0000000000000000F;
      r_mat[1][4] = 0.0000000000000000F;
      r_mat[2][0] = 0.0000000000000000F;
      r_mat[2][1] = 0.0000000000000000F;
      r_mat[2][2] = 0.9029219420985610F;
      r_mat[2][3] = 0.0000000000000000F;
      r_mat[2][4] = 0.0000000000000000F;
      r_mat[3][0] = 0.0000000000000000F;
      r_mat[3][1] = 0.0000000000000000F;
      r_mat[3][2] = 0.0000000000000000F;
      r_mat[3][3] = 0.0011865324620129F;
      r_mat[3][4] = 0.0000000000000000F;
      r_mat[4][0] = 0.0000000000000000F;
      r_mat[4][1] = 0.0000000000000000F;
      r_mat[4][2] = 0.0000000000000000F;
      r_mat[4][3] = 0.0000000000000000F;
      r_mat[4][4] = 0.1924966131303029F;


      // Setup expected data
      // Kalman gain

      exp_k_mat[0][0] = 0.9119462931857341F;
      exp_k_mat[0][1] = -0.0537361418264473F;
      exp_k_mat[0][2] = -0.0001736386005233F;
      exp_k_mat[0][3] = 0.0045208229573810F;
      exp_k_mat[0][4] = -0.0003948027679562F;
      exp_k_mat[1][0] = -0.0013500822776107F;
      exp_k_mat[1][1] = 0.0017911334762607F;
      exp_k_mat[1][2] = 0.1443275634214659F;
      exp_k_mat[1][3] = 0.4961869235258111F;
      exp_k_mat[1][4] = 0.3413184794961904F;
      exp_k_mat[2][0] = -0.0057670128460511F;
      exp_k_mat[2][1] = -0.0067876769795521F;
      exp_k_mat[2][2] = 0.0092483708625449F;
      exp_k_mat[2][3] = -0.0163313212850509F;
      exp_k_mat[2][4] = 0.0217224681339134F;
      exp_k_mat[3][0] = -0.0541059340591808F;
      exp_k_mat[3][1] = 0.9657449578008592F;
      exp_k_mat[3][2] = -0.0000890577637458F;
      exp_k_mat[3][3] = 0.0027470506493641F;
      exp_k_mat[3][4] = -0.0002011657605806F;
      exp_k_mat[4][0] = 0.0077204064371018F;
      exp_k_mat[4][1] = -0.0101817016007892F;
      exp_k_mat[4][2] = -0.8147673335300744F;
      exp_k_mat[4][3] = 2.9137564579077901F;
      exp_k_mat[4][4] = -1.9091530059665955F;
      exp_k_mat[5][0] = 0.0142609236438405F;
      exp_k_mat[5][1] = -0.0563825964678609F;
      exp_k_mat[5][2] = 0.0309843068228492F;
      exp_k_mat[5][3] = -0.1350222647695516F;
      exp_k_mat[5][4] = 0.0725271346601867F;

      // Innovation covariance

      exp_s_mat[0][0] = 7.1452756972919600F;
      exp_s_mat[0][1] = 0.0010766502305076F;
      exp_s_mat[0][2] = 0.1945124907734175F;
      exp_s_mat[0][3] = 0.2138756856448972F;
      exp_s_mat[0][4] = 0.2049740764534283F;
      exp_s_mat[1][0] = 0.0010766502305076F;
      exp_s_mat[1][1] = 7.1945243469068290F;
      exp_s_mat[1][2] = 0.1661297090399247F;
      exp_s_mat[1][3] = 0.1510234288613070F;
      exp_s_mat[1][4] = 0.1591823056821886F;
      exp_s_mat[2][0] = 0.1945124907734175F;
      exp_s_mat[2][1] = 0.1661297090399247F;
      exp_s_mat[2][2] = 7.5667445353414307F;
      exp_s_mat[2][3] = 6.6368895498550877F;
      exp_s_mat[2][4] = 6.6757593513477103F;
      exp_s_mat[3][0] = 0.2138756856448972F;
      exp_s_mat[3][1] = 0.1510234288613070F;
      exp_s_mat[3][2] = 6.6368895498550877F;
      exp_s_mat[3][3] = 6.8048916225953437F;
      exp_s_mat[3][4] = 6.7459677639995803F;
      exp_s_mat[4][0] = 0.2049740764534283F;
      exp_s_mat[4][1] = 0.1591823056821886F;
      exp_s_mat[4][2] = 6.6757593513477103F;
      exp_s_mat[4][3] = 6.7459677639995812F;
      exp_s_mat[4][4] = 6.9289945793698324F;


      // Updated error covariance

      exp_p_mat[0][0] = 0.5522949934669528F;
      exp_p_mat[0][1] = -0.0001567821024079F;
      exp_p_mat[0][2] = -0.0062446271321828F;
      exp_p_mat[0][3] = 0.3388847763030059F;
      exp_p_mat[0][4] = 0.0009200461261690F;
      exp_p_mat[0][5] = -0.0128086799066854F;
      exp_p_mat[1][0] = -0.0001567821024079F;
      exp_p_mat[1][1] = 0.1303165238628605F;
      exp_p_mat[1][2] = 0.0083505569804567F;
      exp_p_mat[1][3] = -0.0000804122090004F;
      exp_p_mat[1][4] = -0.7356713031494380F;
      exp_p_mat[1][5] = 0.0279764104910646F;
      exp_p_mat[2][0] = -0.0062446271321828F;
      exp_p_mat[2][1] = 0.0083505569804567F;
      exp_p_mat[2][2] = 6.5696257671516074F;
      exp_p_mat[2][3] = -0.0038974113406083F;
      exp_p_mat[2][4] = -0.0474699533846365F;
      exp_p_mat[2][5] = 0.4059760545809989F;
      exp_p_mat[3][0] = 0.3388847763030059F;
      exp_p_mat[3][1] = -0.0000804122090004F;
      exp_p_mat[3][2] = -0.0038974113406083F;
      exp_p_mat[3][3] = 0.2168166094222124F;
      exp_p_mat[3][4] = 0.0004748108084890F;
      exp_p_mat[3][5] = -0.0083716710118705F;
      exp_p_mat[4][0] = 0.0009200461261690F;
      exp_p_mat[4][1] = -0.7356713031494380F;
      exp_p_mat[4][2] = -0.0474699533846365F;
      exp_p_mat[4][3] = 0.0004748108084890F;
      exp_p_mat[4][4] = 4.1921088917508396F;
      exp_p_mat[4][5] = -0.1595847110267341F;
      exp_p_mat[5][0] = -0.0128086799066854F;
      exp_p_mat[5][1] = 0.0279764104910646F;
      exp_p_mat[5][2] = 0.4059760545809990F;
      exp_p_mat[5][3] = -0.0083716710118705F;
      exp_p_mat[5][4] = -0.1595847110267341F;
      exp_p_mat[5][5] = 6.9227280540421914F;


      // Updated state vector

      exp_state[0] = 10.0858431315526715F;
      exp_state[1] = 4.9803450135123919F;
      exp_state[2] = 0.0974971212903028F;
      exp_state[3] = 0.0911751131738513F;
      exp_state[4] = 2.1091924377272835F;
      exp_state[5] = 0.0916335499338642F;
   }
};



/** \purpose
* Verify that innovation covariance matrix S and Kalman gain matrix is calculated as expected
* \req
* NA
*/
TEST(f360_msmt_update_support_functions_cca_general, Test_Kalman_Gain_Update_CCA_large_num_times_since_init)
{

   /** \precond
    * Use default setup form test group
    */

   /** \action
    * Call function
    */
   Kalman_Gain_Update_CCA_Moveable(
      h_mat,
      p_mat,
      r_mat,
      nr_total_msnmts,
      obj_ok_to_decay_state,
      k_mat,
      s_mat);

   /** \result
   * Compare computed against expected data
   */

   // S-matrix
   for (uint32_t i = 0U; i < nr_total_msnmts; i++)
   {
      for (uint32_t j = 0U; j < nr_total_msnmts; j++)
      {
         DOUBLES_EQUAL(s_mat[i][j], exp_s_mat[i][j], threshold);
      }
   }

   // K matrix
   for (uint32_t i = 0U; i < STATE_DIMENSION; i++)
   {
      for (uint32_t j = 0U; j < nr_total_msnmts; j++)
      {
         DOUBLES_EQUAL(exp_k_mat[i][j], k_mat[i][j], threshold);
      }
   }
}


/** \purpose
* Verify that error covariance matrix is calculated as expected.
* \req
* NA
*/
TEST(f360_msmt_update_support_functions_cca_general, Test_Error_Cov_Update_CCA)
{

   /** \action
   * Call function
   */
   Error_Cov_Update_CCA_Moveable(
      exp_k_mat,
      r_mat,
      h_mat,
      nr_total_msnmts,
      p_mat);

   /** \result
   * Compare computed P matrix against expected data
   */
   for (uint32_t i = 0U; i < STATE_DIMENSION; i++)
   {
      for (uint32_t j = 0U; j < STATE_DIMENSION; j++)
      {
         DOUBLES_EQUAL(exp_p_mat[i][j], p_mat[i][j], threshold);
      }
   }
}

/** \purpose
* Verify that state vector is calculated as expected.
* \req
* NA
*/
TEST(f360_msmt_update_support_functions_cca_general, Test_State_Update_CCA)
{
   
   /** \precond
    * Nothing other than TEST_SETUP()
    */
   
   /** \action
   * Call function
   */
   State_Update_CCA_Moveable(
      z_mat,
      zhat_mat,
      nr_total_msnmts,
      exp_k_mat,
      state);

   /** \result
   * Compare computed State vector against expected data
   */
   for (uint32_t i = 0U; i < STATE_DIMENSION; i++)
   {
      DOUBLES_EQUAL(exp_state[i], state[i], threshold);
   }
}
/** \purpose
* Verify that K-matrix is decreased (or not) as expected in occluded scenario.
* \req
* NA
*/
TEST(f360_msmt_update_support_functions_cca_general, Decrease_Kmat_For_Occluded_cca)
{
   /** \precond
    * Set first track's properties such that it fits all conditions for limited heading and curvature update.
    * Set second track's properties the same, just OCCLUSN_STATUS_On_EDGE
    * Set third track properties such that it fits all conditions for limited heading and curvature update EXCEPT occlusion status
    * Set fourth track properties such that it fits all conditions for limited heading and curvature update EXCEPT speed
    * Set fifth track properties such that it fits all conditions for limited heading and curvature update EXCEPT longitudinal position
    * Set sixth track properties such that it fits all conditions for limited heading and curvature update EXCEPT it's behind SEP
    * Set sixth track properties such that it fits all conditions for limited heading and curvature update EXCEPT it's not oncoming
    */
   F360_Object_Track_T object_track_1 = {};
   object_track_1.vcs_heading = Angle{ F360_PI };
   object_track_1.speed = 16.0F;
   object_track_1.occlusion_status = OCCLUSION_STATUS_OCCLUDED;
   object_track_1.behind_sep_id = F360_INVALID_UNSIGNED_ID;
   object_track_1.vcs_position.x = 51.0F;
   object_track_1.f_moving = true;

   F360_Object_Track_T object_track_2 = object_track_1;
   F360_Object_Track_T object_track_3 = object_track_1;
   F360_Object_Track_T object_track_4 = object_track_1;
   F360_Object_Track_T object_track_5 = object_track_1;
   F360_Object_Track_T object_track_6 = object_track_1;
   F360_Object_Track_T object_track_7 = object_track_1;

   object_track_2.occlusion_status = OCCLUSION_STATUS_ON_EDGE;
   object_track_3.occlusion_status = OCCLUSION_STATUS_VISIBLE;
   object_track_4.speed = 14.0F;
   object_track_5.vcs_position.x = 49.0F;
   object_track_6.behind_sep_id = 1U;
   object_track_7.vcs_heading.Value(0.0F);

   // Compute Kalman Gain for H,P and R matrix as in Test Group setup
   Kalman_Gain_Update_CCA_Moveable(
      h_mat,
      p_mat,
      r_mat,
      nr_total_msnmts,
      obj_ok_to_decay_state,
      k_mat,
      s_mat);

   float32_t initial_K_vy = k_mat[F360_TRK_FLTR_CCA_STATE_VY][F360_PSEUDO_MSMT_POS_Y];
   float32_t initial_K_ay = k_mat[F360_TRK_FLTR_CCA_STATE_AY][F360_PSEUDO_MSMT_POS_Y];

   /** \action
    * Call tested function.
   */
   Decrease_Kmat_For_Occluded_CCA(calib, object_track_3, k_mat);

   /** \result
    * Check if K-matrix has expected value for each object.
    * For brevity, not to create too many k_mat arrays, make consecutive calls and check the result after every call (so that only one k_mat array is needed)
    */
   DOUBLES_EQUAL_TEXT(initial_K_vy, k_mat[F360_TRK_FLTR_CCA_STATE_VY][F360_PSEUDO_MSMT_POS_Y], 1e-5, "K-matrix has changed unexpectedly");
   DOUBLES_EQUAL_TEXT(initial_K_ay, k_mat[F360_TRK_FLTR_CCA_STATE_AY][F360_PSEUDO_MSMT_POS_Y], 1e-5, "K-matrix has changed unexpectedly");
   Decrease_Kmat_For_Occluded_CCA(calib, object_track_4, k_mat);
   DOUBLES_EQUAL_TEXT(initial_K_vy, k_mat[F360_TRK_FLTR_CCA_STATE_VY][F360_PSEUDO_MSMT_POS_Y], 1e-5, "K-matrix has changed unexpectedly");
   DOUBLES_EQUAL_TEXT(initial_K_ay, k_mat[F360_TRK_FLTR_CCA_STATE_AY][F360_PSEUDO_MSMT_POS_Y], 1e-5, "K-matrix has changed unexpectedly");
   Decrease_Kmat_For_Occluded_CCA(calib, object_track_5, k_mat);
   DOUBLES_EQUAL_TEXT(initial_K_vy, k_mat[F360_TRK_FLTR_CCA_STATE_VY][F360_PSEUDO_MSMT_POS_Y], 1e-5, "K-matrix has changed unexpectedly");
   DOUBLES_EQUAL_TEXT(initial_K_ay, k_mat[F360_TRK_FLTR_CCA_STATE_AY][F360_PSEUDO_MSMT_POS_Y], 1e-5, "K-matrix has changed unexpectedly");
   Decrease_Kmat_For_Occluded_CCA(calib, object_track_6, k_mat);
   DOUBLES_EQUAL_TEXT(initial_K_vy, k_mat[F360_TRK_FLTR_CCA_STATE_VY][F360_PSEUDO_MSMT_POS_Y], 1e-5, "K-matrix has changed unexpectedly");
   DOUBLES_EQUAL_TEXT(initial_K_ay, k_mat[F360_TRK_FLTR_CCA_STATE_AY][F360_PSEUDO_MSMT_POS_Y], 1e-5, "K-matrix has changed unexpectedly");
   Decrease_Kmat_For_Occluded_CCA(calib, object_track_7, k_mat);
   DOUBLES_EQUAL_TEXT(initial_K_vy, k_mat[F360_TRK_FLTR_CCA_STATE_VY][F360_PSEUDO_MSMT_POS_Y], 1e-5, "K-matrix has changed unexpectedly");
   DOUBLES_EQUAL_TEXT(initial_K_ay, k_mat[F360_TRK_FLTR_CCA_STATE_AY][F360_PSEUDO_MSMT_POS_Y], 1e-5, "K-matrix has changed unexpectedly");
   Decrease_Kmat_For_Occluded_CCA(calib, object_track_1, k_mat);
   DOUBLES_EQUAL_TEXT(0.1F * initial_K_vy, k_mat[F360_TRK_FLTR_CCA_STATE_VY][F360_PSEUDO_MSMT_POS_Y], 1e-5, "K-matrix did not decreased as expected");
   DOUBLES_EQUAL_TEXT(0.1F * initial_K_ay, k_mat[F360_TRK_FLTR_CCA_STATE_AY][F360_PSEUDO_MSMT_POS_Y], 1e-5, "K-matrix did not decreased as expected");
   Decrease_Kmat_For_Occluded_CCA(calib, object_track_2, k_mat);
   DOUBLES_EQUAL_TEXT(0.01F * initial_K_vy, k_mat[F360_TRK_FLTR_CCA_STATE_VY][F360_PSEUDO_MSMT_POS_Y], 1e-5, "K-matrix did not decreased as expected");
   DOUBLES_EQUAL_TEXT(0.01F * initial_K_ay, k_mat[F360_TRK_FLTR_CCA_STATE_AY][F360_PSEUDO_MSMT_POS_Y], 1e-5, "K-matrix did not decreased as expected");
}
/** @}*/


/** \defgroup  f360_msmt_update_support_functions_optimize_matrix_mulitplications
*  @{
*/

/** \brief
*  This is a test group containing testst of functions that are used to compute run time optimized matrix multiplications.
*  Expected data was generated with help of the Matlab script Kalman_CCA_general_UT.m that can be found in Jira ticket DFD-1897.
**/
TEST_GROUP(f360_msmt_update_support_functions_optimize_matrix_mulitplications)
{
   /** \setup
   * Declare matrices to operate on
   **/
   float32_t h_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION] = {};
   float32_t p_mat[STATE_DIMENSION][STATE_DIMENSION] = {};
   float32_t r_mat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   float32_t k_mat[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};

   float32_t HPmat[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION] = {};
   float32_t HPHTmat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   float32_t negKHmat[STATE_DIMENSION][STATE_DIMENSION] = {};
   float32_t KRmat[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};

   uint32_t num_measurements;

   const float32_t test_pass_threshold = 1e-6F;

   /** \setup
   * Initialize matrices to operate on
   */
   TEST_SETUP()
   {

      num_measurements = 5U;

      // H
      h_mat[0][0] = 1.0000000000000000F;
      h_mat[0][1] = 0.0000000000000000F;
      h_mat[0][2] = 0.0000000000000000F;
      h_mat[0][3] = 0.0000000000000000F;
      h_mat[0][4] = 0.0000000000000000F;
      h_mat[0][5] = 0.0000000000000000F;
      h_mat[1][0] = 0.0000000000000000F;
      h_mat[1][1] = 0.0000000000000000F;
      h_mat[1][2] = 0.0000000000000000F;
      h_mat[1][3] = 1.0000000000000000F;
      h_mat[1][4] = 0.0000000000000000F;
      h_mat[1][5] = 0.0000000000000000F;
      h_mat[2][0] = 0.0000000000000000F;
      h_mat[2][1] = 1.0000000000000000F;
      h_mat[2][2] = 0.0000000000000000F;
      h_mat[2][3] = 0.0000000000000000F;
      h_mat[2][4] = 0.0000000000000000F;
      h_mat[2][5] = 0.0000000000000000F;
      h_mat[3][0] = 0.0000000000000000F;
      h_mat[3][1] = 0.9848077530122080F;
      h_mat[3][2] = 0.0000000000000000F;
      h_mat[3][3] = 0.0000000000000000F;
      h_mat[3][4] = 0.1736481776669303F;
      h_mat[3][5] = 0.0000000000000000F;
      h_mat[4][0] = 0.0000000000000000F;
      h_mat[4][1] = 0.9961946980917455F;
      h_mat[4][2] = 0.0000000000000000F;
      h_mat[4][3] = 0.0000000000000000F;
      h_mat[4][4] = 0.0871557427476582F;
      h_mat[4][5] = 0.0000000000000000F;

      // Pmat
      p_mat[0][0] = 6.5169020258062931F;
      p_mat[0][1] = 0.1945124907734175F;
      p_mat[0][2] = -0.0384556107300637F;
      p_mat[0][3] = -0.3850330754532219F;
      p_mat[0][4] = 0.1285258329421008F;
      p_mat[0][5] = 0.0938525644826565F;
      p_mat[1][0] = 0.1945124907734175F;
      p_mat[1][1] = 6.6638225932428696F;
      p_mat[1][2] = 0.1043554630575398F;
      p_mat[1][3] = 0.1661297090399247F;
      p_mat[1][4] = 0.4279077173738897F;
      p_mat[1][5] = -0.1840967228468995F;
      p_mat[2][0] = -0.0384556107300637F;
      p_mat[2][1] = 0.1043554630575398F;
      p_mat[2][2] = 6.5717516167530929F;
      p_mat[2][3] = -0.0463124667591884F;
      p_mat[2][4] = -0.0474636529957669F;
      p_mat[2][5] = 0.4057486204994333F;
      p_mat[3][0] = -0.3850330754532219F;
      p_mat[3][1] = 0.1661297090399247F;
      p_mat[3][2] = -0.0463124667591884F;
      p_mat[3][3] = 6.9483854104727634F;
      p_mat[3][4] = -0.0724591341868679F;
      p_mat[3][5] = -0.4093296840042222F;
      p_mat[4][0] = 0.1285258329421008F;
      p_mat[4][1] = 0.4279077173738897F;
      p_mat[4][2] = -0.0474636529957669F;
      p_mat[4][3] = -0.0724591341868679F;
      p_mat[4][4] = 6.4492365170366686F;
      p_mat[4][5] = -0.2768349930633138F;
      p_mat[5][0] = 0.0938525644826565F;
      p_mat[5][1] = -0.1840967228468995F;
      p_mat[5][2] = 0.4057486204994333F;
      p_mat[5][3] = -0.4093296840042222F;
      p_mat[5][4] = -0.2768349930633138F;
      p_mat[5][5] = 6.9573606188351498F;

      // Rmat
      r_mat[0][0] = 0.6283736714856673F;
      r_mat[0][1] = 0.3861097256837295F;
      r_mat[0][2] = 0.0000000000000000F;
      r_mat[0][3] = 0.0000000000000000F;
      r_mat[0][4] = 0.0000000000000000F;
      r_mat[1][0] = 0.3861097256837295F;
      r_mat[1][1] = 0.2461389364340652F;
      r_mat[1][2] = 0.0000000000000000F;
      r_mat[1][3] = 0.0000000000000000F;
      r_mat[1][4] = 0.0000000000000000F;
      r_mat[2][0] = 0.0000000000000000F;
      r_mat[2][1] = 0.0000000000000000F;
      r_mat[2][2] = 0.9029219420985610F;
      r_mat[2][3] = 0.0000000000000000F;
      r_mat[2][4] = 0.0000000000000000F;
      r_mat[3][0] = 0.0000000000000000F;
      r_mat[3][1] = 0.0000000000000000F;
      r_mat[3][2] = 0.0000000000000000F;
      r_mat[3][3] = 0.0011865324620129F;
      r_mat[3][4] = 0.0000000000000000F;
      r_mat[4][0] = 0.0000000000000000F;
      r_mat[4][1] = 0.0000000000000000F;
      r_mat[4][2] = 0.0000000000000000F;
      r_mat[4][3] = 0.0000000000000000F;
      r_mat[4][4] = 0.1924966131303029F;

      // Kmat
      k_mat[0][0] = 0.9119462931857341F;
      k_mat[0][1] = -0.0537361418264473F;
      k_mat[0][2] = -0.0001736386005233F;
      k_mat[0][3] = 0.0045208229573810F;
      k_mat[0][4] = -0.0003948027679562F;
      k_mat[1][0] = -0.0013500822776107F;
      k_mat[1][1] = 0.0017911334762607F;
      k_mat[1][2] = 0.1443275634214659F;
      k_mat[1][3] = 0.4961869235258111F;
      k_mat[1][4] = 0.3413184794961904F;
      k_mat[2][0] = -0.0057670128460511F;
      k_mat[2][1] = -0.0067876769795521F;
      k_mat[2][2] = 0.0092483708625449F;
      k_mat[2][3] = -0.0163313212850509F;
      k_mat[2][4] = 0.0217224681339134F;
      k_mat[3][0] = -0.0541059340591808F;
      k_mat[3][1] = 0.9657449578008592F;
      k_mat[3][2] = -0.0000890577637458F;
      k_mat[3][3] = 0.0027470506493641F;
      k_mat[3][4] = -0.0002011657605806F;
      k_mat[4][0] = 0.0077204064371018F;
      k_mat[4][1] = -0.0101817016007892F;
      k_mat[4][2] = -0.8147673335300744F;
      k_mat[4][3] = 2.9137564579077901F;
      k_mat[4][4] = -1.9091530059665955F;
      k_mat[5][0] = 0.0142609236438405F;
      k_mat[5][1] = -0.0563825964678609F;
      k_mat[5][2] = 0.0309843068228492F;
      k_mat[5][3] = -0.1350222647695516F;
      k_mat[5][4] = 0.0725271346601867F;



      // H * P
      HPmat[0][0] = 6.5169020258062931F;
      HPmat[0][1] = 0.1945124907734175F;
      HPmat[0][2] = -0.0384556107300637F;
      HPmat[0][3] = -0.3850330754532219F;
      HPmat[0][4] = 0.1285258329421008F;
      HPmat[0][5] = 0.0938525644826565F;
      HPmat[1][0] = -0.3850330754532219F;
      HPmat[1][1] = 0.1661297090399247F;
      HPmat[1][2] = -0.0463124667591884F;
      HPmat[1][3] = 6.9483854104727634F;
      HPmat[1][4] = -0.0724591341868679F;
      HPmat[1][5] = -0.4093296840042222F;
      HPmat[2][0] = 0.1945124907734175F;
      HPmat[2][1] = 6.6638225932428696F;
      HPmat[2][2] = 0.1043554630575398F;
      HPmat[2][3] = 0.1661297090399247F;
      HPmat[2][4] = 0.4279077173738897F;
      HPmat[2][5] = -0.1840967228468995F;
      HPmat[3][0] = 0.2138756856448972F;
      HPmat[3][1] = 6.6368895498550877F;
      HPmat[3][2] = 0.0945280922401138F;
      HPmat[3][3] = 0.1510234288613070F;
      HPmat[3][4] = 1.5413050061700015F;
      HPmat[3][5] = -0.2293717720236480F;
      HPmat[4][0] = 0.2049740764534283F;
      HPmat[4][1] = 6.6757593513477103F;
      HPmat[4][2] = 0.0998216290844670F;
      HPmat[4][3] = 0.1591823056821886F;
      HPmat[4][4] = 0.9883673981180608F;
      HPmat[4][5] = -0.2075239386751227F;


      // H * P * H'  
      HPHTmat[0][0] = 6.5169020258062931F;
      HPHTmat[0][1] = -0.3850330754532219F;
      HPHTmat[0][2] = 0.1945124907734175F;
      HPHTmat[0][3] = 0.2138756856448972F;
      HPHTmat[0][4] = 0.2049740764534283F;
      HPHTmat[1][0] = -0.3850330754532219F;
      HPHTmat[1][1] = 6.9483854104727634F;
      HPHTmat[1][2] = 0.1661297090399247F;
      HPHTmat[1][3] = 0.1510234288613070F;
      HPHTmat[1][4] = 0.1591823056821886F;
      HPHTmat[2][0] = 0.1945124907734175F;
      HPHTmat[2][1] = 0.1661297090399247F;
      HPHTmat[2][2] = 6.6638225932428696F;
      HPHTmat[2][3] = 6.6368895498550877F;
      HPHTmat[2][4] = 6.6757593513477103F;
      HPHTmat[3][0] = 0.2138756856448972F;
      HPHTmat[3][1] = 0.1510234288613070F;
      HPHTmat[3][2] = 6.6368895498550877F;
      HPHTmat[3][3] = 6.8037050901333309F;
      HPHTmat[3][4] = 6.7459677639995803F;
      HPHTmat[4][0] = 0.2049740764534283F;
      HPHTmat[4][1] = 0.1591823056821886F;
      HPHTmat[4][2] = 6.6757593513477103F;
      HPHTmat[4][3] = 6.7459677639995812F;
      HPHTmat[4][4] = 6.7364979662395292F;


      // -K * H
      negKHmat[0][0] = -0.9119462931857341F;
      negKHmat[0][1] = -0.0038852024736713F;
      negKHmat[0][2] = 0.0000000000000000F;
      negKHmat[0][3] = 0.0537361418264473F;
      negKHmat[0][4] = -0.0007506233396240F;
      negKHmat[0][5] = 0.0000000000000000F;
      negKHmat[1][0] = 0.0013500822776107F;
      negKHmat[1][1] = -0.9729959522878012F;
      negKHmat[1][2] = 0.0000000000000000F;
      negKHmat[1][3] = -0.0017911334762607F;
      negKHmat[1][4] = -0.1159098206464094F;
      negKHmat[1][5] = 0.0000000000000000F;
      negKHmat[2][0] = 0.0057670128460511F;
      negKHmat[2][1] = -0.0148049666285649F;
      negKHmat[2][2] = 0.0000000000000000F;
      negKHmat[2][3] = 0.0067876769795521F;
      negKHmat[2][4] = 0.0009426663355187F;
      negKHmat[2][5] = 0.0000000000000000F;
      negKHmat[3][0] = 0.0541059340591808F;
      negKHmat[3][1] = -0.0024158587495372F;
      negKHmat[3][2] = 0.0000000000000000F;
      negKHmat[3][3] = -0.9657449578008592F;
      negKHmat[3][4] = -0.0004594875879420F;
      negKHmat[3][5] = 0.0000000000000000F;
      negKHmat[4][0] = -0.0077204064371018F;
      negKHmat[4][1] = -0.1528345142170657F;
      negKHmat[4][2] = 0.0000000000000000F;
      negKHmat[4][3] = 0.0101817016007892F;
      negKHmat[4][4] = -0.3395748508269947F;
      negKHmat[4][5] = 0.0000000000000000F;
      negKHmat[5][0] = -0.0142609236438405F;
      negKHmat[5][1] = 0.0297355193352083F;
      negKHmat[5][2] = 0.0000000000000000F;
      negKHmat[5][3] = 0.0563825964678609F;
      negKHmat[5][4] = 0.0171252139310264F;
      negKHmat[5][5] = 0.0000000000000000F;



      // K * R      
      KRmat[0][0] = 0.5522949934669530F;
      KRmat[0][1] = 0.3388847763030060F;
      KRmat[0][2] = -0.0001567821024078F;
      KRmat[0][3] = 0.0000053641031939F;
      KRmat[0][4] = -0.0000759981956860F;
      KRmat[1][0] = -0.0001567821024079F;
      KRmat[1][1] = -0.0000804122090004F;
      KRmat[1][2] = 0.1303165238628605F;
      KRmat[1][3] = 0.0005887418919897F;
      KRmat[1][4] = 0.0657026513018019F;
      KRmat[2][0] = -0.0062446271321828F;
      KRmat[2][1] = -0.0038974113406083F;
      KRmat[2][2] = 0.0083505569804568F;
      KRmat[2][3] = -0.0000193776428523F;
      KRmat[2][4] = 0.0041815015446093F;
      KRmat[3][0] = 0.3388847763030058F;
      KRmat[3][1] = 0.2168166094222124F;
      KRmat[3][2] = -0.0000804122090004F;
      KRmat[3][3] = 0.0000032594647703F;
      KRmat[3][4] = -0.0000387237275895F;
      KRmat[4][0] = 0.0009200461261690F;
      KRmat[4][1] = 0.0004748108084890F;
      KRmat[4][2] = -0.7356713031494436F;
      KRmat[4][3] = 0.0034572666237072F;
      KRmat[4][4] = -0.3675054875961081F;
      KRmat[5][0] = -0.0128086799066854F;
      KRmat[5][1] = -0.0083716710118705F;
      KRmat[5][2] = 0.0279764104910648F;
      KRmat[5][3] = -0.0001602083002436F;
      KRmat[5][4] = 0.0139612277821314F;

   }
};

/*\purpose
* Verify that function Hmat_Times_Pmat() works as intended.
*/
TEST(f360_msmt_update_support_functions_optimize_matrix_mulitplications, Hmat_Times_Pmat)
{
   /** \precond
   Use default setup
   **/

   /** \action
   * Call the function Hmat_Times_Pmat()
   **/
   float32_t output_HPmat[MSMT_UPDATE_MAX_NUM_OF_MSMT][STATE_DIMENSION] = {};
   Hmat_Times_Pmat_CCA_Moveable(h_mat, p_mat, num_measurements, output_HPmat);

   /** \result
   * Check that output is as expected
   **/

   for (uint32_t meas_idx = 0U; meas_idx < num_measurements; meas_idx++)
   {
      for (uint32_t state_idx = 0U; state_idx < STATE_DIMENSION; state_idx++)
      {
         DOUBLES_EQUAL_TEXT(HPmat[meas_idx][state_idx], output_HPmat[meas_idx][state_idx], test_pass_threshold, "Unexpected output after matrix multipliction");
      }
   }
}

/*\purpose
* Verify that function HPmat_Times_Hmat_Transpose() works as intended.
*/
TEST(f360_msmt_update_support_functions_optimize_matrix_mulitplications, HPmat_Times_Hmat_Transpose)
{
   /** \precond
   Use default setup
   **/

   /** \action
   * Call the function HPmat_Times_Hmat_Transpose()
   **/
   float32_t output_HPHTmat[MSMT_UPDATE_MAX_NUM_OF_MSMT][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   HPmat_Times_Hmat_Transpose_CCA_Moveable(HPmat, h_mat, num_measurements, output_HPHTmat);

   /** \result
   * Check that output is as expected
   **/

   for (uint32_t meas_idx1 = 0U; meas_idx1 < num_measurements; meas_idx1++)
   {
      for (uint32_t meas_idx2 = 0U; meas_idx2 < num_measurements; meas_idx2++)
      {
         DOUBLES_EQUAL_TEXT(HPHTmat[meas_idx1][meas_idx2], output_HPHTmat[meas_idx1][meas_idx2], test_pass_threshold, "Unexpected output after matrix multipliction");
      }
   }
}

/*\purpose
* Verify that function Negative_Kmat_Times_Hmat_CV() works as intended.
*/
TEST(f360_msmt_update_support_functions_optimize_matrix_mulitplications, Negative_Kmat_Times_Hmat)
{
   /** \precond
   Use default setup
   **/

   /** \action
   * Call the function Negative_Kmat_Times_Hmat()
   **/
   float32_t output_negKHmat[STATE_DIMENSION][STATE_DIMENSION] = {};
   Negative_Kmat_Times_Hmat_CCA_Moveable(k_mat, h_mat, num_measurements, output_negKHmat);

   /** \result
   * Check that output is as expected
   **/

   for (uint32_t state_idx1 = 0U; state_idx1 < STATE_DIMENSION; state_idx1++)
   {
      for (uint32_t state_idx2 = 0U; state_idx2 < STATE_DIMENSION; state_idx2++)
      {
         DOUBLES_EQUAL_TEXT(negKHmat[state_idx1][state_idx2], output_negKHmat[state_idx1][state_idx2], test_pass_threshold, "Unexpected output after matrix multipliction");
      }
   }
}

/*\purpose
* Verify that function Kmat_Times_Rmat() works as intended.
*/
TEST(f360_msmt_update_support_functions_optimize_matrix_mulitplications, Kmat_Times_Rmat)
{
   /** \precond
   Use default setup
   **/

   /** \action
   * Call the function Kmat_Times_Rmat()
   **/
   float32_t output_KRmat[STATE_DIMENSION][MSMT_UPDATE_MAX_NUM_OF_MSMT] = {};
   Kmat_Times_Rmat_CCA_Moveable(k_mat, r_mat, num_measurements, output_KRmat);

   /** \result
   * Check that output is as expected
   **/

   for (uint32_t state_idx = 0U; state_idx < STATE_DIMENSION; state_idx++)
   {
      for (uint32_t meas_idx = 0U; meas_idx < num_measurements; meas_idx++)
      {
         DOUBLES_EQUAL_TEXT(KRmat[state_idx][meas_idx], output_KRmat[state_idx][meas_idx], test_pass_threshold, "Unexpected output after matrix multipliction");
      }
   }
}
/** @}*/


/** \defgroup  f360_msmt_update_support_functions_saturate_heading_rate
*  @{
*/

/** \brief
*  This is a test group containing tests of the function Saturate_Heading_Rate()
**/
TEST_GROUP(f360_msmt_update_support_functions_saturate_heading_rate)
{
   /** \setup
   * Set up calibration data structure.
   * Set up floating point accuracy for comparing if floats are equal
   **/
  F360_Calibrations_T calibs = {};
  const float32_t test_pass_th = 1e-8F;
   

   /** \setup
   * Initialize calibration structure to default tracker values
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

   }
};

/*\purpose
* Verify that function Saturate_Heading_Rate() works as intended when object speed is large and heading rate needs to be saturated
*/
TEST(f360_msmt_update_support_functions_saturate_heading_rate, Saturate_Heading_Rate_Large_Speed)
{
   /** \precond
   * Use default tracker calibrations from test group.
   * Set abs_obj_speed to 8.0
   * Set heading rate to 7.0 / abs_obj_speed + epsilon
   **/
  const float32_t abs_obj_speed = 8.0F;
  const float32_t heading_rate =  7.0F / abs_obj_speed + 0.01F;

   /** \action
   * Call the function Saturate_Heading_Rate()
   **/
   const float32_t sat_heading_rate = Saturate_Heading_Rate(heading_rate, abs_obj_speed);

   /** \result
   * Check that output heading rate is as expected. Expected is that input heading rate has been saturated to 7.0 / abs_obj_speed
   **/
   const float32_t exp_sat_heading_rate = 7.0F / abs_obj_speed;
   DOUBLES_EQUAL(exp_sat_heading_rate, sat_heading_rate, test_pass_th);
}

/*\purpose
* Verify that function Saturate_Heading_Rate() works as intended when object speed is large and heading rate doesn't need to be saturated
*/
TEST(f360_msmt_update_support_functions_saturate_heading_rate, Dont_Saturate_Heading_Rate_Large_Speed)
{
   /** \precond
   * Use default tracker calibrations from test group.
   * Set abs_obj_speed to calibs.k_cca_heading_rate_high_speed_breakpoint + epsilon
   * Set heading rate to -(7.0 / abs_obj_speed - epsilon)
   **/
  const float32_t abs_obj_speed = 8.0F;
  const float32_t heading_rate =  -(7.0F / abs_obj_speed - 0.01F);

   /** \action
   * Call the function Saturate_Heading_Rate()
   **/
   const float32_t sat_heading_rate = Saturate_Heading_Rate(heading_rate, abs_obj_speed);

   /** \result
   * Check that outputed heading rate is as expected. Expected is that it is same as input heading rate.
   **/
   const float32_t exp_sat_heading_rate = heading_rate;
   DOUBLES_EQUAL(exp_sat_heading_rate, sat_heading_rate, test_pass_th);
}

/*\purpose
* Verify that function Saturate_Heading_Rate() works as intended when object speed is small and heading rate needs to be saturated
*/
TEST(f360_msmt_update_support_functions_saturate_heading_rate, Saturate_Heading_Rate_Small_Speed)
{
   /** \precond
   * Use default tracker calibrations from test group.
   * Set abs_obj_speed to 3.0
   * Set heading rate to -(4.0F * abs_obj_speed + epsilon)
   **/
  const float32_t abs_obj_speed = 3.0F;
  const float32_t heading_rate = -(4.0F * abs_obj_speed + 0.01F);

   /** \action
   * Call the function Saturate_Heading_Rate()
   **/
   const float32_t sat_heading_rate = Saturate_Heading_Rate(heading_rate, abs_obj_speed);

   /** \result
   * Check that outputed heading rate is as expected. Expected is that input heading rate has been saturated to - 1.0F/4.0F * abs_obj_speed
   **/
   const float32_t exp_sat_heading_rate = - 1.0F/4.0F  * abs_obj_speed;
   DOUBLES_EQUAL(exp_sat_heading_rate, sat_heading_rate, test_pass_th);
}

/*\purpose
* Verify that function Saturate_YHeading_Rate() works as intended when object speed is small and heading rate doesn't need to be saturated
*/
TEST(f360_msmt_update_support_functions_saturate_heading_rate, Dont_Saturate_Heading_Rate_Small_Speed)
{
   /** \precond
   * Use default tracker calibrations from test group.
   * Set abs_obj_speed to calibs.k_cca_heading_rate_low_speed_breakpoint - epsilon
   * Set heading rate to 4.0F * abs_obj_speed - epsilon
   **/
  const float32_t abs_obj_speed = 3.0F;
  const float32_t heading_rate = 4.0F * abs_obj_speed - 0.01F;

   /** \action
   * Call the function Saturate_Heading_Rate()
   **/
   const float32_t sat_heading_rate = Saturate_Heading_Rate(heading_rate, abs_obj_speed);

   /** \result
   * Check that outputed heading rate is as expected. Expected is that input heading rate not been modified.
   **/
   const float32_t exp_sat_heading_rate = 1.0F/4.0F  * abs_obj_speed;
   DOUBLES_EQUAL(exp_sat_heading_rate, sat_heading_rate, test_pass_th);
}

/*\purpose
* Verify that function Saturate_Heading_Rate() works as intended when object speed is medium and heading rate needs to be saturated
*/
TEST(f360_msmt_update_support_functions_saturate_heading_rate, Saturate_Heading_Rate_Medium_Speed)
{
   /** \precond
   * Use default tracker calibrations from test group.
   * Set abs_obj_speed to ( 11.0F) / 2
   * Set heading rate to -(1.0F + epsilon)
   **/
  const float32_t abs_obj_speed = (11.0F) / 2.0F;
  const float32_t heading_rate = -(1.0F + 0.01F);

   /** \action
   * Call the function Saturate_Heading_Rate()
   **/
   const float32_t sat_heading_rate = Saturate_Heading_Rate(heading_rate, abs_obj_speed);

   /** \result
   * Check that outputed heading rate is as expected. Expected is that input heading rate has been saturated to -1.0F
   **/
   const float32_t exp_sat_heading_rate = -1.0F;
   DOUBLES_EQUAL(exp_sat_heading_rate, sat_heading_rate, test_pass_th);
}

/*\purpose
* Verify that function Saturate_Heading_Rate() works as intended when object speed is medium and heading rate doesn't need to be saturated
*/
TEST(f360_msmt_update_support_functions_saturate_heading_rate, Dont_Saturate_Heading_Rate_Medium_Speed)
{
   /** \precond
   * Use default tracker calibrations from test group.
   * Set abs_obj_speed to ( 11.0F) / 2
   * Set heading rate to 1.0F - epsilon
   **/
  const float32_t abs_obj_speed = (11.0F) / 2.0F;
  const float32_t heading_rate = 1.0F - 0.01F;

   /** \action
   * Call the function Saturate_Heading_Rate()
   **/
   const float32_t sat_heading_rate = Saturate_Heading_Rate(heading_rate, abs_obj_speed);

   /** \result
   * Check that outputed heading rate is as expected. Expected is that input heading rate has not been modified.
   **/
   const float32_t exp_sat_heading_rate = heading_rate;
   DOUBLES_EQUAL(exp_sat_heading_rate, sat_heading_rate, test_pass_th);
}
/** @}*/

/** \defgroup  f360_msmt_update_support_functions_saturate_heading_rate
*  @{
*/

/** \brief
*  This is a test group containing tests of the function Measurement_Update_Pointing_Heading_Rate_CCA()
**/
TEST_GROUP(f360_msmt_update_support_functions_Measurement_Update_Pointing_Heading_Rate_CCA)
{
   /** \setup
   * Set up calibration data structure.
   * Set up object data structure.
   * Set up floating point accuracy for comparing if floats are equal
   **/
  F360_Calibrations_T calibs = {};
  F360_Object_Track_T obj = {};
  const float32_t test_pass_th = 1e-6F;
   

   /** \setup
   * Initialize calibration structure to default tracker values.
   * Initialize object bbox and position:
   *    - reference point: REAR_LEF
   *    - vcs_position: [50m, 0m]
   *    - length: 6m
   *    - width: 3m
   *    - orientation: -30deg.
   *    - bbox center: given by above parameters
   * Initialize object heading rate to 0.1
   * Initialize object cca_pnt_filter_cov to a random covariance matrix ([ 3.388311, 1.657181; 1.657181 1.394086])
   * Initialize object speed to be ((11.0F) / 2)
   * Initialize object vcs_heading to be 2 degrees larger than the bbox orientation (i.e -28deg)
   * Initialize object heading pointing disagreement to be the difference between object pointing and object heading 
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;
      obj.vcs_position.x = 50.0F;
      obj.vcs_position.y = 0.0F;
      obj.bbox.Set_Length(6.0F);
      obj.bbox.Set_Width(3.0F);
      obj.bbox.Set_Orientation(F360_DEG2RAD(-30.0F));
      obj.Update_Bbox_Center();
      obj.heading_rate = 0.1F;
      obj.cca_pnt_filter_cov[0][0] = 3.388311F;
      obj.cca_pnt_filter_cov[1][0] = 1.657181F;
      obj.cca_pnt_filter_cov[0][1] =  obj.cca_pnt_filter_cov[1][0];
      obj.cca_pnt_filter_cov[1][1] = 1.394086F;
      obj.speed = 2.5F;
      obj.curvature = obj.heading_rate / obj.speed;
      obj.vcs_heading.Value(F360_DEG2RAD(-28.0F));
      obj.hdg_ptng_disagmt = obj.bbox.Get_Orientation().Value() - obj.vcs_heading.Value();
   }
};

/*\purpose
* Verify that function Measurement_Update_Pointing_heading_Rate_CCA() works as for the deafult case where
*  - measurement noise (R) does not need to be saturated (i.e speed is large enough)
*  - heading rate does not need to be saturated (i.e. abs(heading rate) is small enough)
*/
TEST(f360_msmt_update_support_functions_Measurement_Update_Pointing_Heading_Rate_CCA, Measurement_Update_Pointing_Heading_Rate_CCA_default)
{
   /** \precond
   * Use default tracker calibrations from test group.
   **/

   /** \action
   * Extract object bbox center so that we can do before-after comparision after the function call.
   * Call the function Measurement_Update_Pointing_Heading_Rate_CCA().
   **/
   Point bbox_center_before = obj.bbox.Get_Center();
   Measurement_Update_Pointing_Heading_Rate_CCA(calibs, obj);

   /** \result
   * Check that output is as expected. Expected is that
   *    - object bbox oreintation is -0.494809726749981 rad
   *    - object yheading rate is 0.114080367699277 rad/s
   *    - object cca_pnt_filter_cov is [0.5938167582, 0.2904284315; 0.2904284315, 0.7256242241];
   *    - object hdg_ptng_disagmt is difference between updated ppointing and heading
   *    - object bbox center has been updated (i.e. it is not the same as before)
   * 
   **/
   const float32_t exp_pnt = -0.494809726749981F;
   const float32_t exp_heading_rate = 0.114080367699277F;
   const float32_t exp_cca_pnt_filter_cov[2][2] = {{0.5938167582F, 0.2904284315F},{0.2904284315F, 0.7256242241F}};
   const float32_t exp_hdg_ptng_disagmt = exp_pnt - obj.vcs_heading.Value();
   
   DOUBLES_EQUAL_TEXT(exp_pnt, obj.bbox.Get_Orientation().Value(), test_pass_th, "Object pointing is unexpected");
   DOUBLES_EQUAL_TEXT(exp_heading_rate, obj.heading_rate, test_pass_th, "Object heading_rate is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[0][0], obj.cca_pnt_filter_cov[0][0], test_pass_th, "Object cca_pnt_filter_cov[0][0] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[0][1], obj.cca_pnt_filter_cov[0][1], test_pass_th, "Object cca_pnt_filter_cov[0][1] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[1][0], obj.cca_pnt_filter_cov[1][0], test_pass_th, "Object cca_pnt_filter_cov[1][0] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[1][1], obj.cca_pnt_filter_cov[1][1], test_pass_th, "Object cca_pnt_filter_cov[1][1] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg_ptng_disagmt, obj.hdg_ptng_disagmt, test_pass_th, "Object pointing is unexpected");
   CHECK_TRUE_TEXT(std::abs(obj.bbox.Get_Center().x - bbox_center_before.x) > test_pass_th, "Object bbox center x coordinate is unexpected");
   CHECK_TRUE_TEXT(std::abs(obj.bbox.Get_Center().y - bbox_center_before.y) > test_pass_th, "Object bbox center y coordinate is unexpected");
}

/*\purpose
* Verify that function Measurement_Update_Pointing_heading_Rate_CCA() works as expected the reference point is not at the rear and has some curvature
* and there's no significant difference between actual heading pointing disagreement and the predicted heading pointing disagreement.
*/
TEST(f360_msmt_update_support_functions_Measurement_Update_Pointing_Heading_Rate_CCA, Measurement_Update_Pointing_Heading_Rate_CCA_Non_Rear_Ref_Point_Curvature)
{
   /** \precond
   * Use default tracker calibrations from test group.
   * Change object reference point to LEFT
   **/
   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   obj.Update_Bbox_Center();

   /** \action
   * Extract object bbox center so that we can do before-after comparision after the function call.
   * Call the function Measurement_Update_Pointing_Heading_Rate_CCA().
   **/
   Point bbox_center_before = obj.bbox.Get_Center();
   Measurement_Update_Pointing_Heading_Rate_CCA(calibs, obj);

   /** \result
   * Check that output is as expected. Expected is that
   *    - object bbox oreintation is -0.494809726749981 rad
   *    - object yheading rate is 0.114080367699277 rad/s
   *    - object cca_pnt_filter_cov is [0.5938167582, 0.2904284315; 0.2904284315, 0.7256242241];
   *    - object hdg_ptng_disagmt is difference between updated ppointing and heading
   *    - object bbox center has been updated (i.e. it is not the same as before)
   * 
   **/
   const float32_t exp_pnt = -0.568897963F;
   const float32_t exp_heading_rate = 0.0719980448F;
   const float32_t exp_cca_pnt_filter_cov[2][2] = {{0.560000002, -0.114826247F},{-0.114826247F, 0.298707008F}};
   const float32_t exp_hdg_ptng_disagmt = exp_pnt - obj.vcs_heading.Value();
   
   DOUBLES_EQUAL_TEXT(exp_pnt, obj.bbox.Get_Orientation().Value(), test_pass_th, "Object pointing is unexpected");
   DOUBLES_EQUAL_TEXT(exp_heading_rate, obj.heading_rate, test_pass_th, "Object heading_rate is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[0][0], obj.cca_pnt_filter_cov[0][0], test_pass_th, "Object cca_pnt_filter_cov[0][0] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[0][1], obj.cca_pnt_filter_cov[0][1], test_pass_th, "Object cca_pnt_filter_cov[0][1] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[1][0], obj.cca_pnt_filter_cov[1][0], test_pass_th, "Object cca_pnt_filter_cov[1][0] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[1][1], obj.cca_pnt_filter_cov[1][1], test_pass_th, "Object cca_pnt_filter_cov[1][1] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg_ptng_disagmt, obj.hdg_ptng_disagmt, test_pass_th, "Object pointing is unexpected");
   CHECK_TRUE_TEXT(std::abs(obj.bbox.Get_Center().x - bbox_center_before.x) > test_pass_th, "Object bbox center x coordinate is unexpected");
   CHECK_TRUE_TEXT(std::abs(obj.bbox.Get_Center().y - bbox_center_before.y) > test_pass_th, "Object bbox center y coordinate is unexpected");
}

/*\purpose
* Verify that function Measurement_Update_Pointing_heading_Rate_CCA() works as expected the reference point is not at the rear and has some curvature
* and there's is a significant difference between actual heading pointing disagreement and the predicted heading pointing disagreement.
*/
TEST(f360_msmt_update_support_functions_Measurement_Update_Pointing_Heading_Rate_CCA, Measurement_Update_Pointing_Heading_Rate_CCA_Non_Rear_Ref_Point_Curvature_Incorrect_Hdg_Ptn_Disagmt)
{
   /** \precond
   * Use default tracker calibrations from test group.
   * Change object reference point to LEFT
   * Introduce an error in hdg_ptng_disagmt such that there's a significant difference between actual heading pointing disagreement and the predicted heading pointing disagreement.
   **/
   obj.reference_point = F360_REFERENCE_POINT_LEFT;
   obj.Update_Bbox_Center();
   obj.hdg_ptng_disagmt = -0.0300F;

   /** \action
   * Extract object bbox center so that we can do before-after comparision after the function call.
   * Call the function Measurement_Update_Pointing_Heading_Rate_CCA().
   **/
   Point bbox_center_before = obj.bbox.Get_Center();
   Measurement_Update_Pointing_Heading_Rate_CCA(calibs, obj);

   /** \result
   * Check that output is as expected. Expected is that
   *    - object bbox oreintation is -0.494809726749981 rad
   *    - object yheading rate is 0.114080367699277 rad/s
   *    - object cca_pnt_filter_cov is [0.5938167582, 0.2904284315; 0.2904284315, 0.7256242241];
   *    - object hdg_ptng_disagmt is difference between updated ppointing and heading
   *    - object bbox center has been updated (i.e. it is not the same as before)
   * 
   **/
   const float32_t exp_pnt = -0.504890919F;
   const float32_t exp_heading_rate = 0.111564457F;
   const float32_t exp_cca_pnt_filter_cov[2][2] = {{0.560000002, -0.114826247F},{-0.114826247F, 0.298707008F}};
   const float32_t exp_hdg_ptng_disagmt = exp_pnt - obj.vcs_heading.Value();
   
   DOUBLES_EQUAL_TEXT(exp_pnt, obj.bbox.Get_Orientation().Value(), test_pass_th, "Object pointing is unexpected");
   DOUBLES_EQUAL_TEXT(exp_heading_rate, obj.heading_rate, test_pass_th, "Object heading_rate is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[0][0], obj.cca_pnt_filter_cov[0][0], test_pass_th, "Object cca_pnt_filter_cov[0][0] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[0][1], obj.cca_pnt_filter_cov[0][1], test_pass_th, "Object cca_pnt_filter_cov[0][1] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[1][0], obj.cca_pnt_filter_cov[1][0], test_pass_th, "Object cca_pnt_filter_cov[1][0] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[1][1], obj.cca_pnt_filter_cov[1][1], test_pass_th, "Object cca_pnt_filter_cov[1][1] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg_ptng_disagmt, obj.hdg_ptng_disagmt, test_pass_th, "Object pointing is unexpected");
   CHECK_TRUE_TEXT(std::abs(obj.bbox.Get_Center().x - bbox_center_before.x) > test_pass_th, "Object bbox center x coordinate is unexpected");
   CHECK_TRUE_TEXT(std::abs(obj.bbox.Get_Center().y - bbox_center_before.y) > test_pass_th, "Object bbox center y coordinate is unexpected");
}


/*\purpose
* Verify that function Measurement_Update_Pointing_heading_Rate_CCA() works as for the reversing case where
*  - measurement noise (R) does not need to be saturated (i.e speed is large enough)
*  - heading rate does not need to be saturated (i.e. abs(heading rate) is small enough)
*/
TEST(f360_msmt_update_support_functions_Measurement_Update_Pointing_Heading_Rate_CCA, Measurement_Update_Pointing_Heading_Rate_CCA_Reversing)
{
   /** \precond
   * Use default tracker calibrations from test group.
   * Change object's pointing to 150 degrees, such that it's very different from heading
   * Increase object speed to 3.5 m/s to indicate reversing along with the pointing
   **/
   obj.Set_Bbox_Orientation(obj.bbox.Get_Orientation() + F360_PI);
   obj.speed = 3.5F;

   /** \action
   * Extract object bbox center so that we can do before-after comparision after the function call.
   * Call the function Measurement_Update_Pointing_Heading_Rate_CCA().
   **/
   Point bbox_center_before = obj.bbox.Get_Center();
   Measurement_Update_Pointing_Heading_Rate_CCA(calibs, obj);

   /** \result
   * Check that output is as expected. Expected is that
   *    - object bbox oreintation is -0.4942499 rad
   *    - object yheading rate is 0.114354295 rad/s
   *    - object cca_pnt_filter_cov is [0.56, 0.263850200690759; 0.263850200690759F, 0.712625140199914];
   *        - Note that the first P matrix element will be saturated to the max value (r_init).
   *    - object hdg_ptng_disagmt is difference between updated ppointing and heading
   *    - object bbox center has been updated (i.e. it is not the same as before)
   * 
   **/
   const float32_t exp_pnt = -0.4942499F;
   const float32_t exp_heading_rate = 0.114354295F;
   const float32_t exp_cca_pnt_filter_cov[2][2] = {{0.56F, 0.263850200690759F},{0.263850200690759F, 0.712625140199914F}};

   const float32_t exp_hdg_ptng_disagmt = exp_pnt - obj.vcs_heading.Value();
   
   DOUBLES_EQUAL_TEXT(exp_pnt, obj.bbox.Get_Orientation().Value(), test_pass_th, "Object pointing is unexpected");
   DOUBLES_EQUAL_TEXT(exp_heading_rate, obj.heading_rate, test_pass_th, "Object heading_rate is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[0][0], obj.cca_pnt_filter_cov[0][0], test_pass_th, "Object cca_pnt_filter_cov[0][0] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[0][1], obj.cca_pnt_filter_cov[0][1], test_pass_th, "Object cca_pnt_filter_cov[0][1] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[1][0], obj.cca_pnt_filter_cov[1][0], test_pass_th, "Object cca_pnt_filter_cov[1][0] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[1][1], obj.cca_pnt_filter_cov[1][1], test_pass_th, "Object cca_pnt_filter_cov[1][1] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg_ptng_disagmt, obj.hdg_ptng_disagmt, test_pass_th, "Object pointing is unexpected");
   CHECK_TRUE_TEXT(std::abs(obj.bbox.Get_Center().x - bbox_center_before.x) > test_pass_th, "Object bbox center x coordinate is unexpected");
   CHECK_TRUE_TEXT(std::abs(obj.bbox.Get_Center().y - bbox_center_before.y) > test_pass_th, "Object bbox center y coordinate is unexpected");
}

/*\purpose
* Verify that function Measurement_Update_Pointing_heading_Rate_CCA() works as for the deafult case where heading rate needs to be saturated and 
*  - measurement noise (R) does not need to be saturated (i.e speed is large enough)
*/
TEST(f360_msmt_update_support_functions_Measurement_Update_Pointing_Heading_Rate_CCA, Measurement_Update_Pointing_Heading_Rate_CCA_saturate_heading_rate)
{
   /** \precond
   * Use default tracker calibrations from test group except for:
   *    - Set object heading rate to larger than 1.0F (int his test we use 1.0F + 0.1F)
   **/
  obj.heading_rate = 1.0F + 0.1F;
  obj.speed = (11.0F) / 2.0F;

   /** \action
   * Extract object bbox center so that we can do before-after comparision after the function call.
   * Call the function Measurement_Update_Pointing_Heading_Rate_CCA().
   **/
   Point bbox_center_before = obj.bbox.Get_Center();
   Measurement_Update_Pointing_Heading_Rate_CCA(calibs, obj);

   /** \result
   * Check that output is as expected. Expected is that
   *    - object bbox oreintation is -0.4938918153 rad
   *    - object heading rate is 1.0F
   *    - object cca_pnt_filter_cov is [0.56, 0.2468508293; 0.2468508293, 0.7043109579]; 
   *      first element is saturated
   *    - object hdg_ptng_disagmt is difference between updated ppointing and heading
   *    - object bbox center has been updated (i.e. it is not the same as before)
   * 
   **/
   const float32_t exp_pnt = -0.4938918153F;
   const float32_t exp_heading_rate = 1.0F;
   const float32_t exp_cca_pnt_filter_cov[2][2] = {{0.56F, 0.2468508293F},{0.2468508293F, 0.7043109579F}};
   const float32_t exp_hdg_ptng_disagmt = exp_pnt - obj.vcs_heading.Value();
   
   DOUBLES_EQUAL_TEXT(exp_pnt, obj.bbox.Get_Orientation().Value(), test_pass_th, "Object pointing is unexpected");
   DOUBLES_EQUAL_TEXT(exp_heading_rate, obj.heading_rate, test_pass_th, "Object heading_rate is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[0][0], obj.cca_pnt_filter_cov[0][0], test_pass_th, "Object cca_pnt_filter_cov[0][0] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[0][1], obj.cca_pnt_filter_cov[0][1], test_pass_th, "Object cca_pnt_filter_cov[0][1] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[1][0], obj.cca_pnt_filter_cov[1][0], test_pass_th, "Object cca_pnt_filter_cov[1][0] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[1][1], obj.cca_pnt_filter_cov[1][1], test_pass_th, "Object cca_pnt_filter_cov[1][1] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg_ptng_disagmt, obj.hdg_ptng_disagmt, test_pass_th, "Object pointing is unexpected");
   CHECK_TRUE_TEXT(std::abs(obj.bbox.Get_Center().x - bbox_center_before.x) > test_pass_th, "Object bbox center x coordinate is unexpected");
   CHECK_TRUE_TEXT(std::abs(obj.bbox.Get_Center().y - bbox_center_before.y) > test_pass_th, "Object bbox center y coordinate is unexpected");
}

/*\purpose
* Verify that function Measurement_Update_Pointing_Heading_Rate_CCA() works as for the case where object speed is
* small and measurement noise (R) needs to be saturated and
*  - heading rate also needs to saturated due to the very low speed
* Note: This test aims to test a "protection against zero division" branch of the function when speed is 0.
* This branch should not be reach becasue the function is not called for stationary objects. Becasue of this some of the
* the expected data might be a bit unintuative.
*/
TEST(f360_msmt_update_support_functions_Measurement_Update_Pointing_Heading_Rate_CCA, Measurement_Update_Pointing_YHeading_Rate_CCA_saturate_r)
{
   /** \precond
   * Use default tracker calibrations from test group except for
   *    - Change object speed to slightly smaller than threshold calibs.k_speed_th_for_saturating_r
   **/
  obj.speed = calibs.k_speed_th_for_saturating_r - 0.0000001F;

   /** \action
   * Extract object bbox center so that we can do before-after comparision after the function call.
   * Call the function Measurement_Update_Pointing_Heading_Rate_CCA().
   **/
   Point bbox_center_before = obj.bbox.Get_Center();
   Measurement_Update_Pointing_Heading_Rate_CCA(calibs, obj);

   /** \result
   * Check that output is as expected. Expected is that
   *    - object bbox oreintation is -0.523598774415555 rad
   *    - object heading rate is set to saturation value abs(speed) / min turn radius
   *    - object cca_pnt_filter_cov is [3.388310885193490, 1.657180943849556; 1.657180943849556, 1.394085972537512];
   *    - object hdg_ptng_disagmt is difference between updated ppointing and heading
   *    - object bbox center has been updated (i.e. it is not the same as before)
   * 
   **/
   const float32_t exp_pnt = -0.523598774415555F;
   const float32_t min_turn_radius = 4.0F;
   const float32_t max_heading_rate_for_speed = std::abs(obj.speed) / min_turn_radius;
   const float32_t exp_heading_rate = max_heading_rate_for_speed; // Saturated heading rate for low speed
   const float32_t exp_cca_pnt_filter_cov[2][2] = {{3.388310885193490F, 1.657180943849556F}, {1.657180943849556F, 1.394085972537512F}};
   const float32_t exp_hdg_ptng_disagmt = exp_pnt - obj.vcs_heading.Value();
   
   DOUBLES_EQUAL_TEXT(exp_pnt, obj.bbox.Get_Orientation().Value(), test_pass_th, "Object pointing is unexpected");
   DOUBLES_EQUAL_TEXT(exp_heading_rate, obj.heading_rate, test_pass_th, "Object heading_rate is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[0][0], obj.cca_pnt_filter_cov[0][0], test_pass_th, "Object cca_pnt_filter_cov[0][0] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[0][1], obj.cca_pnt_filter_cov[0][1], test_pass_th, "Object cca_pnt_filter_cov[0][1] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[1][0], obj.cca_pnt_filter_cov[1][0], test_pass_th, "Object cca_pnt_filter_cov[1][0] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[1][1], obj.cca_pnt_filter_cov[1][1], test_pass_th, "Object cca_pnt_filter_cov[1][1] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg_ptng_disagmt, obj.hdg_ptng_disagmt, test_pass_th, "Object pointing is unexpected");
   CHECK_TRUE_TEXT(std::abs(obj.bbox.Get_Center().y - bbox_center_before.y) > 1e-7F, "Object bbox center y coordinate is unexpected");
   // Note: Due to the filter being very slow for speed == 0m/s the change in pointing is very minor and therefore the change in object bbox center
   // is also very minor. Due to this we need a stricter test threshold for this zero speed case for the bbox center y coordinate test. For the bbox
   // center x coordinate the difference is so small that it is numerically rounded to zero so x coordinate can't be tested.
}

/*\purpose
* Verify that function Measurement_Update_Pointing_heading_Rate_CCA() works as for the deafult case where
*  - reference point is set to front center
*  - measurement noise (R) does not need to be saturated (i.e speed is large enough)
*  - heading rate does not need to be saturated (i.e. abs(heading rate) is small enough)
*/
TEST(f360_msmt_update_support_functions_Measurement_Update_Pointing_Heading_Rate_CCA, Measurement_Update_Pointing_Heading_Rate_CCA_default_ref_front)
{
   /** \precond
   * Use default tracker calibrations from test group except for
   *    - Change object reference point to front center
   *    - Change object curvature to be non-zero
   **/
   obj.reference_point = F360_REFERENCE_POINT_FRONT;
   obj.curvature = obj.heading_rate / obj.speed;

   /** \action
   * Extract object bbox center so that we can do before-after comparision after the function call.
   * Call the function Measurement_Update_Pointing_Heading_Rate_CCA().
   **/
   Point bbox_center_before = obj.bbox.Get_Center();
   Measurement_Update_Pointing_Heading_Rate_CCA(calibs, obj);

   /** \result
   * Check that output is as expected. Expected is that
   *    - object bbox oreintation is -0.568897918207857 rad
   *    - object yheading rate is 0.071998010525360 rad/s
   *    - object cca_pnt_filter_cov is [0.56, -0.1148262667; -0.1148262667, 0.2987070154];
   *    - object hdg_ptng_disagmt is difference between updated ppointing and heading
   *    - object bbox center has been updated (i.e. it is not the same as before)
   * 
   **/
   const float32_t exp_pnt = -0.568897918207857F;
   const float32_t exp_heading_rate = 0.071998010525360F;
   const float32_t exp_cca_pnt_filter_cov[2][2] = {{0.56F, -0.1148262667F},{-0.1148262667F, 0.2987070154F}};
   const float32_t exp_hdg_ptng_disagmt = Angle(exp_pnt - obj.vcs_heading.Value()).Normalize().Value();

   DOUBLES_EQUAL_TEXT(exp_pnt, obj.bbox.Get_Orientation().Value(), test_pass_th, "Object pointing is unexpected");
   DOUBLES_EQUAL_TEXT(exp_heading_rate, obj.heading_rate, test_pass_th, "Object heading_rate is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[0][0], obj.cca_pnt_filter_cov[0][0], test_pass_th, "Object cca_pnt_filter_cov[0][0] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[0][1], obj.cca_pnt_filter_cov[0][1], test_pass_th, "Object cca_pnt_filter_cov[0][1] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[1][0], obj.cca_pnt_filter_cov[1][0], test_pass_th, "Object cca_pnt_filter_cov[1][0] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[1][1], obj.cca_pnt_filter_cov[1][1], test_pass_th, "Object cca_pnt_filter_cov[1][1] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg_ptng_disagmt, obj.hdg_ptng_disagmt, test_pass_th, "Object pointing is unexpected");
   CHECK_TRUE_TEXT(std::abs(obj.bbox.Get_Center().x - bbox_center_before.x) > test_pass_th, "Object bbox center x coordinate is unexpected");
   CHECK_TRUE_TEXT(std::abs(obj.bbox.Get_Center().y - bbox_center_before.y) > test_pass_th, "Object bbox center y coordinate is unexpected");
}

/*\purpose
* Verify that function Measurement_Update_Pointing_heading_Rate_CCA() works as for the default case where
*  - The weight for the pseudo heading is 0 because the speed is too high and it has been more than 1.2 second since the object initialization
*/
TEST(f360_msmt_update_support_functions_Measurement_Update_Pointing_Heading_Rate_CCA, Measurement_Update_Pointing_Heading_Rate_CCA_Pseudo_Weight_Saturated_Due_To_High_Speed)
{
   /** \precond
   * Use default tracker calibrations from test group except for
   *    - Set object speed to 16 m/s
   *    - Set time since initializion equal to 1.0
   **/
   obj.speed = 16.0F;
   obj.time_since_initialization = 1.0F;

   /** \action
   * Extract object bbox center so that we can do before-after comparision after the function call.
   * Call the function Measurement_Update_Pointing_Heading_Rate_CCA().
   **/
   Point bbox_center_before = obj.bbox.Get_Center();
   Measurement_Update_Pointing_Heading_Rate_CCA(calibs, obj);

   /** \result
   * Check that output is as expected. Expected is that
   *    - object bbox oreintation is -0.4936728F rad
   *    - object yheading rate is 0.11463365 rad/s
   *    - object cca_pnt_filter_cov is [0.56, -0.1148262667; -0.1148262667, 0.2987070154];
   *    - object hdg_ptng_disagmt is difference between updated ppointing and heading
   *    - object bbox center has been updated (i.e. it is not the same as before)
   * 
   **/
   const float32_t exp_pnt = -0.4936728F;
   const float32_t exp_heading_rate = 0.1146365;
   const float32_t exp_cca_pnt_filter_cov[2][2] = {{0.56F, 0.2364482},{0.2364482, 0.6992232}};
   const float32_t exp_hdg_ptng_disagmt = Angle(exp_pnt - obj.vcs_heading.Value()).Normalize().Value();

   DOUBLES_EQUAL_TEXT(exp_pnt, obj.bbox.Get_Orientation().Value(), test_pass_th, "Object pointing is unexpected");
   DOUBLES_EQUAL_TEXT(exp_heading_rate, obj.heading_rate, test_pass_th, "Object heading_rate is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[0][0], obj.cca_pnt_filter_cov[0][0], test_pass_th, "Object cca_pnt_filter_cov[0][0] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[0][1], obj.cca_pnt_filter_cov[0][1], test_pass_th, "Object cca_pnt_filter_cov[0][1] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[1][0], obj.cca_pnt_filter_cov[1][0], test_pass_th, "Object cca_pnt_filter_cov[1][0] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[1][1], obj.cca_pnt_filter_cov[1][1], test_pass_th, "Object cca_pnt_filter_cov[1][1] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg_ptng_disagmt, obj.hdg_ptng_disagmt, test_pass_th, "Object pointing is unexpected");
   CHECK_TRUE_TEXT(std::abs(obj.bbox.Get_Center().x - bbox_center_before.x) > test_pass_th, "Object bbox center x coordinate is unexpected");
   CHECK_TRUE_TEXT(std::abs(obj.bbox.Get_Center().y - bbox_center_before.y) > test_pass_th, "Object bbox center y coordinate is unexpected");
}

/*\purpose
* Verify that function Measurement_Update_Pointing_heading_Rate_CCA() works as for the default case where
*  - The weight for the pseudo heading is 0 because the speed is higher than 7.5 but less than 12.0
*  - 
*/
TEST(f360_msmt_update_support_functions_Measurement_Update_Pointing_Heading_Rate_CCA, Measurement_Update_Pointing_Heading_Rate_CCA_Pseudo_Weight_Not_Saturated_Due_Time_Since_Init)
{
   /** \precond
   * Use default tracker calibrations from test group except for
   *    - Change object speed
   *    - Set time since initializion equal to 1.0
   **/
   obj.speed = 10.0F;
   obj.time_since_initialization = 1.0F;

   /** \action
   * Extract object bbox center so that we can do before-after comparision after the function call.
   * Call the function Measurement_Update_Pointing_Heading_Rate_CCA().
   **/
   Point bbox_center_before = obj.bbox.Get_Center();
   Measurement_Update_Pointing_Heading_Rate_CCA(calibs, obj);

   /** \result
   * Check that output is as expected. Expected is that
   *    - object bbox oreintation is -0.4937189 rad
   *    - object yheading rate is 0.114614 rad/s
   *    - object cca_pnt_filter_cov is [0.56, -0.1148262667; -0.1148262667, 0.2987070154];
   *    - object hdg_ptng_disagmt is difference between updated ppointing and heading
   *    - object bbox center has been updated (i.e. it is not the same as before)
   * 
   **/
   const float32_t exp_pnt = -0.4937189;
   const float32_t exp_heading_rate = 0.114614;
   const float32_t exp_cca_pnt_filter_cov[2][2] = {{0.56F, 0.2386354},{0.2386354, 0.7002929}};
   const float32_t exp_hdg_ptng_disagmt = Angle(exp_pnt - obj.vcs_heading.Value()).Normalize().Value();

   DOUBLES_EQUAL_TEXT(exp_pnt, obj.bbox.Get_Orientation().Value(), test_pass_th, "Object pointing is unexpected");
   DOUBLES_EQUAL_TEXT(exp_heading_rate, obj.heading_rate, test_pass_th, "Object heading_rate is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[0][0], obj.cca_pnt_filter_cov[0][0], test_pass_th, "Object cca_pnt_filter_cov[0][0] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[0][1], obj.cca_pnt_filter_cov[0][1], test_pass_th, "Object cca_pnt_filter_cov[0][1] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[1][0], obj.cca_pnt_filter_cov[1][0], test_pass_th, "Object cca_pnt_filter_cov[1][0] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_cca_pnt_filter_cov[1][1], obj.cca_pnt_filter_cov[1][1], test_pass_th, "Object cca_pnt_filter_cov[1][1] is unexpected");
   DOUBLES_EQUAL_TEXT(exp_hdg_ptng_disagmt, obj.hdg_ptng_disagmt, test_pass_th, "Object pointing is unexpected");
   CHECK_TRUE_TEXT(std::abs(obj.bbox.Get_Center().x - bbox_center_before.x) > test_pass_th, "Object bbox center x coordinate is unexpected");
   CHECK_TRUE_TEXT(std::abs(obj.bbox.Get_Center().y - bbox_center_before.y) > test_pass_th, "Object bbox center y coordinate is unexpected");
}

/** @}*/
