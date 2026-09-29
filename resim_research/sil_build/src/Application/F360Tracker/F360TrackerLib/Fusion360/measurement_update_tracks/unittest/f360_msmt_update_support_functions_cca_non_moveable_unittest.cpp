/** \file
 * This file contains unit tests for content of f360_msmt_update_support_functions_cca_non_moveable.cpp file
 */

#include "f360_msmt_update_support_functions_cca_non_moveable.h"
#include "f360_trk_fltr_cca_states.h"
#include "f360_clear_object_track.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;
constexpr uint8_t msmt_size = 3;

/** \defgroup  f360_msmt_update_support_functions_cca_non_moveable
 *  @{
 */

/** \brief
 * This test group sets up test data and expected output for Non Moveable CCA update in the general case.
 */
TEST_GROUP(f360_msmt_update_support_functions_cca_non_moveable)
{
   uint32_t nr_dets;
   uint32_t nr_total_msnmts;
   float32_t h_mat[msmt_size][STATE_DIMENSION] = {};
   float32_t p_mat[STATE_DIMENSION][STATE_DIMENSION] = {};
   float32_t r_mat[msmt_size][msmt_size] = {};
   float32_t z_mat[msmt_size] = {};
   float32_t zhat_mat[msmt_size] = {};
   float32_t state[STATE_DIMENSION];

   float32_t s_mat[msmt_size][msmt_size] = {};
   float32_t k_mat[STATE_DIMENSION][msmt_size] = {};

   float32_t exp_s_mat[msmt_size][msmt_size] = {};
   float32_t exp_k_mat[STATE_DIMENSION][msmt_size] = {};
   float32_t exp_p_mat[STATE_DIMENSION][STATE_DIMENSION] = {};
   float32_t exp_state[STATE_DIMENSION] = {};

   float32_t rdot_comp[3]; // Must be the size of number of detections
   float32_t azimuth[3]; // Must be the size of number of detections

   const float32_t threshold = 0.0001F; // Threshold for comparing calculated and expected data


   /** \setup
   * Create data for a CCA update
   */
   TEST_SETUP()
   {
      // Setup arbitrary reasonable state vector
      state[F360_TRK_FLTR_CCA_STATE_X] = 10.0F;
      state[F360_TRK_FLTR_CCA_STATE_VX] = 5.0F;
      state[F360_TRK_FLTR_CCA_STATE_AX] = 0.1F;
      state[F360_TRK_FLTR_CCA_STATE_Y] = 0.0F;
      state[F360_TRK_FLTR_CCA_STATE_VY] = 2.0F;
      state[F360_TRK_FLTR_CCA_STATE_AY] = 0.1F;

      // Three detections
      nr_dets = 1U;
      azimuth[0] = F360_DEG2RAD(0.0F);
      rdot_comp[0] = state[F360_TRK_FLTR_CCA_STATE_VX] * F360_Cosf(azimuth[0]) + state[F360_TRK_FLTR_CCA_STATE_VY] * F360_Sinf(azimuth[0]) + 0.1F; // Add some value to force state update


      // Set up H, Z and Zhat matrix
      //Two pseudo position measurements
      nr_total_msnmts = msmt_size;
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
      r_mat[1][0] = 0.3861097256837295F;
      r_mat[1][1] = 0.2461389364340652F;
      r_mat[1][2] = 0.0000000000000000F;
      r_mat[2][0] = 0.0000000000000000F;
      r_mat[2][1] = 0.0000000000000000F;
      r_mat[2][2] = 0.9029219420985610F;

   }

};

/** \purpose
 * Verify that acceleration driven by measurements is prevented when initialization
 *
 * \req
 * NA.
 */
TEST(f360_msmt_update_support_functions_cca_non_moveable, Test_Kalman_Gain_Update_NM_CCA_Large_Num_Times_Since_Init)
{
 /** \precond
    * Set required input
    */
   float32_t h_mat[3][6] = {
      {1.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F},
      {0.0000000F, 0.0000000F, 0.0000000F, 1.0000000F, 0.0000000F, 0.0000000F},
      {0.0000000F, 1.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F}
   };

   float32_t p_mat[6][6] = {
      {6.5169020F, 0.1945125F, -0.0384556F, -0.3850331F, 0.1285258F, 0.0938526F},
      {0.1945125F, 6.6638226F, 0.1043555F, 0.1661297F, 0.4279077F, -0.1840967F},
      {-0.0384556F, 0.1043555F, 6.5717516F, -0.0463125F, -0.0474637F, 0.4057486F},
      {-0.3850331F, 0.1661297F, -0.0463125F, 6.9483854F, -0.0724591F, -0.4093297F},
      {0.1285258F, 0.4279077F, -0.0474637F, -0.0724591F, 6.4492365F, -0.2768350F},
      {0.0938526F, -0.1840967F, 0.4057486F, -0.4093297F, -0.2768350F, 6.9573606F}
   };

   float32_t r_mat[3][3] =  {
      {0.4839733F, 0.0348570F, 0.0000000F},
      {0.0348570F, 0.1017386F, 0.0000000F},
      {0.0000000F, 0.0000000F, 0.9000000F}
   };


   float32_t exp_s_mat[3][3] = {
      {7.0008754F, -0.3501761F, 0.1945125F},
      {-0.3501761F, 7.0501240F, 0.1661297F},
      {0.1945125F, 0.1661297F, 7.5638226F}
   };

   float32_t exp_k_mat[6][3] = {
      {0.9303921F, -0.0084481F, 0.0019756F},
      {0.0034593F, 0.0029793F, 0.8808582F},
      {-0.0062459F, -0.0072119F, 0.0141157F},
      {-0.0057286F, 0.9852736F, 0.0004708F},
      {0.0162516F, -0.0107993F, 0.0563922F},
      {0.0112066F, -0.0569525F, -0.0233764F}
   };


   /** \action
    * Call function
    */
   Kalman_Gain_Update_CCA_Non_Moveable(
      h_mat,
      p_mat,
      r_mat,
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
         DOUBLES_EQUAL(k_mat[i][j], exp_k_mat[i][j], threshold);
      }
   }

}


/** \purpose
* Verify that error covariance matrix is calculated as expected.
* \req
* NA
*/
TEST(f360_msmt_update_support_functions_cca_non_moveable, Test_Error_Cov_Update_NM_CCA)
{
   /** \precond
    */
   float32_t k_mat[6][3] = {
      {0.9303921F, -0.0084481F, 0.0019756F},
      {0.0034593F, 0.0029793F, 0.8808582F},
      {-0.0062459F, -0.0072119F, 0.0141157F},
      {-0.0057286F, 0.9852736F, 0.0004708F},
      {0.0162516F, -0.0107993F, 0.0563922F},
      {0.0112066F, -0.0569525F, -0.0233764F}
   };

   float32_t r_mat[3][3] =  {
      {0.4839733F, 0.0348570F, 0.0000000F},
      {0.0348570F, 0.1017386F, 0.0000000F},
      {0.0000000F, 0.0000000F, 0.9000000F}
   };

   float32_t h_mat[3][6] = {
      {1.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F},
      {0.0000000F, 0.0000000F, 0.0000000F, 1.0000000F, 0.0000000F, 0.0000000F},
      {0.0000000F, 1.0000000F, 0.0000000F, 0.0000000F, 0.0000000F, 0.0000000F}
   };

   float32_t exp_p_mat[6][6] = {
      {0.4499905F, 0.0017780F, -0.0032742F, 0.0315711F, 0.0074889F, 0.0034385F},
      {0.0017780F, 0.7927723F, 0.0127041F, 0.0004237F, 0.0507530F, -0.0210388F},
      {-0.0032742F, 0.0127041F, 6.5697044F, -0.0009514F, -0.0532237F, 0.4059814F},
      {0.0315711F, 0.0004237F, -0.0009514F, 0.1000407F, -0.0005322F, -0.0054036F},
      {0.0074889F, 0.0507530F, -0.0532237F, -0.0005322F, 6.4222346F, -0.2723991F},
      {0.0034385F, -0.0210388F, 0.4059814F, -0.0054036F, -0.2723991F, 6.9286930F}
   };



   /** \action
   * Call function
   */
   Error_Cov_Update_CCA_Non_Moveable(
      k_mat,
      r_mat,
      h_mat,
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
TEST(f360_msmt_update_support_functions_cca_non_moveable, Test_State_Update_NM_CCA)
{

   /** \precond
    *
    */
   float32_t k_mat[6][3] = {
      {0.9303921F, -0.0084481F, 0.0019756F},
      {0.0034593F, 0.0029793F, 0.8808582F},
      {-0.0062459F, -0.0072119F, 0.0141157F},
      {-0.0057286F, 0.9852736F, 0.0004708F},
      {0.0162516F, -0.0107993F, 0.0563922F},
      {0.0112066F, -0.0569525F, -0.0233764F}
   };

   float32_t exp_state[6] = {
      10.0923920F,
      5.0887297F,
      0.1000658F,
      0.0980016F,
      2.0061845F,
      0.0930878F
   };

   /** \action
   * Call function
   */
   State_Update_CCA_Non_Moveable(
      z_mat,
      zhat_mat,
      k_mat,
      state);

   /** \result
   * Compare computed State vector against expected data
   */
   for (uint32_t i = 0U; i < STATE_DIMENSION; i++)
   {
      DOUBLES_EQUAL(exp_state[i], state[i], threshold);
   }
}
/** @}*/

/** \defgroup  f360_Check_For_Crossing_VRU_Type_Obj
 *  @{
 */
/** \brief
 * This test group sets up test data and expected output for Check_For_Crossing_VRU_Type_Obj
 */
TEST_GROUP(f360_Check_For_Crossing_VRU_Type_Obj)
{
   float32_t host_curvature_rear;
   F360_Calibrations_T calib;
   F360_Object_Track_T object_track;
   // Exp data variables
   bool exp_f_crossing_moveable;
   uint8_t exp_cca_cross_moving_buffer_index;
   int8_t exp_cca_cross_moving_buffer[F360_CCA_NON_MOVABLE_MAX_BUFFER_SIZE];

   /** \setup
   * Setup general input for Check_For_Crossing_VRU_Type_Obj()
   */
   TEST_SETUP()
   {
      // Initialize calibrations
      Initialize_Tracker_Calibrations(calib);

      // Reset Object Track Properties
      Clear_Object_Track(object_track);

      // Set host curvature
      host_curvature_rear = 0.0F; // Should be less than 0.007

      // Set over the ground height of the object
      object_track.otg_height = 1.0F;

      // Object_Track properties
      object_track.vcs_position.x = 5.0F; // Should be less than 90m
      object_track.vcs_position.y = 2.0; // should be less than 25m

      (void)object_track.vcs_heading.Value(F360_DEG2RAD(90)); // should be greater than 10 degrees

      object_track.cca_cross_moving_buffer_index = 4U;

      // Set  object_track.cca_cross_moving_buffer = [-1 0 -1 -1 0]
      // The above cca_cross_moving_buffer indicates that the object has been moving
      // towards the right for past 3 scans, while being perpendicular to host motion direction
      object_track.cca_cross_moving_buffer[0] = -1;
      object_track.cca_cross_moving_buffer[1] =  0;
      object_track.cca_cross_moving_buffer[2] = -1;
      object_track.cca_cross_moving_buffer[3] = -1;


      // prev_predicted_vcs_y_pos indicates the previos time updated vcs position
      // The prev_predicted_vcs_y_pos value is set such that the object has moved
      // 0.06 m since the last scan
      object_track.prev_predicted_vcs_y_pos = object_track.vcs_position.y + 0.06F;


      // Setup the default expected results
      exp_f_crossing_moveable = true;
      exp_cca_cross_moving_buffer_index = 0U;
      for(unsigned int ind = 0; ind < F360_CCA_NON_MOVABLE_MAX_BUFFER_SIZE; ind++)
      {
         exp_cca_cross_moving_buffer[ind] = object_track.cca_cross_moving_buffer[ind];
      }
   }

};

/** \purpose
* This test checks if the object is considered f_crossing_movable, when there is evidence (based on lateral position change)
* to suggest that it has been moving laterally for 4 scans and is closer than 50m to host
* \req
* NA
*/
TEST(f360_Check_For_Crossing_VRU_Type_Obj, Check_if_obj_is_f_crossing_moveable_when_seen_moving_laterally_for_4scans_and_closer_than_50m_to_host)
{
   /** \precond
    * Default test setup from TEST GROUP is used.
    * The test is setup for an object that is moving laterlly perpendicular to host
    * This object has been observed moving from right to left laterally for past 3 scans
    * For the current scan to be tested, the expectation is to continue the trend
    * exp_cca_cross_moving_buffer[4] = -1, meaning object is now seen moving for 4 scans
    * Since, previously object_track.cca_cross_moving_buffer_index = 4, after this function call, it should reset to 0
    */
   exp_cca_cross_moving_buffer[4] = -1;

   /** \action
    * Call Check_For_Crossing_VRU_Type_Obj()
    */
   const bool f_crossing_moveable = Check_For_Crossing_VRU_Type_Obj(host_curvature_rear, calib, object_track);

   /** \result
    * Check that the relevant properties are the same as the defined expected values in the setup
    */

   CHECK_EQUAL(exp_f_crossing_moveable, f_crossing_moveable);
   CHECK_EQUAL(exp_cca_cross_moving_buffer_index, object_track.cca_cross_moving_buffer_index);
   for(unsigned int ind = 0; ind < F360_CCA_NON_MOVABLE_MAX_BUFFER_SIZE; ind++)
   {
      CHECK_EQUAL(exp_cca_cross_moving_buffer[ind], object_track.cca_cross_moving_buffer[ind]);
   }
}

/** \purpose
* This test checks if the object is not considered to be cross movable when host curvature is high
* \req
* NA
*/
TEST(f360_Check_For_Crossing_VRU_Type_Obj, Check_if_obj_is_not_f_crossing_moveable_when_host_curvature_is_not_met)
{
   /** \precond
    * Set host_curvature to be greater than 0.007F
    * Set the expected f_crossing_moveable to false
    * object.cca_cross_moving_buffer_index is expected to not change
    * object.cca_cross_moving_buffer is expected to not change
    */
   host_curvature_rear = 0.07F;

   // Set Expected Data
   exp_f_crossing_moveable = false;
   exp_cca_cross_moving_buffer_index = 4U;

   /** \action
    * Call Check_For_Crossing_VRU_Type_Obj()
    */
   const bool f_crossing_moveable = Check_For_Crossing_VRU_Type_Obj(host_curvature_rear, calib, object_track);

   /** \result
    * Check that the relevant properties are the same as the defined expected values in the setup
    */

   CHECK_EQUAL(exp_f_crossing_moveable, f_crossing_moveable);
   CHECK_EQUAL(exp_cca_cross_moving_buffer_index, object_track.cca_cross_moving_buffer_index);
   for(unsigned int ind = 0; ind < F360_CCA_NON_MOVABLE_MAX_BUFFER_SIZE; ind++)
   {
      CHECK_EQUAL(exp_cca_cross_moving_buffer[ind], object_track.cca_cross_moving_buffer[ind]);
   }
}

/** \purpose
* This test checks if the object is not considered to be cross_movable when object position is beyond longitudinal position threshold
* \req
* NA
*/
TEST(f360_Check_For_Crossing_VRU_Type_Obj, Check_if_obj_is_not_f_crossing_moveable_when_object_long_pos_condition_is_not_met)
{
   /** \precond
    * Set object vcs position x to be greater than 90m
    * Set the expected f_crossing_moveable to false
    * object.cca_cross_moving_buffer_index is expected to not change
    * object.cca_cross_moving_buffer is expected to not change
    */
   object_track.vcs_position.x = 91.0F;

   // Set Expected Data
   exp_f_crossing_moveable = false;
   exp_cca_cross_moving_buffer_index = 4U;

   /** \action
    * Call Check_For_Crossing_VRU_Type_Obj()
    */
   const bool f_crossing_moveable = Check_For_Crossing_VRU_Type_Obj(host_curvature_rear, calib, object_track);

   /** \result
    * Check that the relevant properties are the same as the defined expected values in the setup
    */

   CHECK_EQUAL(exp_f_crossing_moveable, f_crossing_moveable);
   CHECK_EQUAL(exp_cca_cross_moving_buffer_index, object_track.cca_cross_moving_buffer_index);
   for(unsigned int ind = 0; ind < F360_CCA_NON_MOVABLE_MAX_BUFFER_SIZE; ind++)
   {
      CHECK_EQUAL(exp_cca_cross_moving_buffer[ind], object_track.cca_cross_moving_buffer[ind]);
   }
}

/** \purpose
* This test checks if the object is not considered to be cross_movable when object position is beyond lateral position threshold
* \req
* NA
*/
TEST(f360_Check_For_Crossing_VRU_Type_Obj, Check_if_obj_is_not_f_crossing_moveable_when_object_lat_pos_condition_is_not_met)
{
   /** \precond
    * Set object vcs position y to be greater than 25m
    * Set the expected f_crossing_moveable to false
    * object.cca_cross_moving_buffer_index is expected to not change
    * object.cca_cross_moving_buffer is expected to not change
    */
   object_track.vcs_position.y = 25.1F;

   // Set Expected Data
   exp_f_crossing_moveable = false;
   exp_cca_cross_moving_buffer_index = 4U;

   /** \action
    * Call Check_For_Crossing_VRU_Type_Obj()
    */
   const bool f_crossing_moveable = Check_For_Crossing_VRU_Type_Obj(host_curvature_rear, calib, object_track);

   /** \result
    * Check that the relevant properties are the same as the defined expected values in the setup
    */

   CHECK_EQUAL(exp_f_crossing_moveable, f_crossing_moveable);
   CHECK_EQUAL(exp_cca_cross_moving_buffer_index, object_track.cca_cross_moving_buffer_index);
   for(unsigned int ind = 0; ind < F360_CCA_NON_MOVABLE_MAX_BUFFER_SIZE; ind++)
   {
      CHECK_EQUAL(exp_cca_cross_moving_buffer[ind], object_track.cca_cross_moving_buffer[ind]);
   }
}

/** \purpose
* This test checks if the object is not considered to be cross_movable when object heading is beyond heading threshold
* \req
* NA
*/
TEST(f360_Check_For_Crossing_VRU_Type_Obj, Check_if_obj_is_not_f_crossing_moveable_when_object_heading_condition_is_not_met)
{
   /** \precond
    * Set object vcs heading to be lesser than 10 degrees
    * Set the expected f_crossing_moveable to false
    * object.cca_cross_moving_buffer_index is expected to not change
    * object.cca_cross_moving_buffer is expected to not change
    */
   (void)object_track.vcs_heading.Value(F360_DEG2RAD(1.0F));

   // Set Expected Data
   exp_f_crossing_moveable = false;
   exp_cca_cross_moving_buffer_index = 4U;

   /** \action
    * Call Check_For_Crossing_VRU_Type_Obj()
    */
   const bool f_crossing_moveable = Check_For_Crossing_VRU_Type_Obj(host_curvature_rear, calib, object_track);

   /** \result
    * Check that the relevant properties are the same as the defined expected values in the setup
    */

   CHECK_EQUAL(exp_f_crossing_moveable, f_crossing_moveable);
   CHECK_EQUAL(exp_cca_cross_moving_buffer_index, object_track.cca_cross_moving_buffer_index);
   for(unsigned int ind = 0; ind < F360_CCA_NON_MOVABLE_MAX_BUFFER_SIZE; ind++)
   {
      CHECK_EQUAL(exp_cca_cross_moving_buffer[ind], object_track.cca_cross_moving_buffer[ind]);
   }
}

/** \purpose
* This test checks if the object is not considered to be cross_movable when object height is beyond threshold
* \req
* NA
*/
TEST(f360_Check_For_Crossing_VRU_Type_Obj, Check_if_obj_is_not_f_crossing_moveable_when_object_height_condition_is_not_met)
{
   /** \precond
    * Set object height to be above 5 meters
    * Set the expected f_crossing_moveable to false
    * object.cca_cross_moving_buffer_index is expected to not change
    * object.cca_cross_moving_buffer is expected to not change
    */
   object_track.otg_height = 5.5F;

   // Set Expected Data
   exp_f_crossing_moveable = false;
   exp_cca_cross_moving_buffer_index = 4U;

   /** \action
    * Call Check_For_Crossing_VRU_Type_Obj()
    */
   const bool f_crossing_moveable = Check_For_Crossing_VRU_Type_Obj(host_curvature_rear, calib, object_track);

   /** \result
    * Check that the relevant properties are the same as the defined expected values in the setup
    */

   CHECK_EQUAL(exp_f_crossing_moveable, f_crossing_moveable);
   CHECK_EQUAL(exp_cca_cross_moving_buffer_index, object_track.cca_cross_moving_buffer_index);
   for(unsigned int ind = 0; ind < F360_CCA_NON_MOVABLE_MAX_BUFFER_SIZE; ind++)
   {
      CHECK_EQUAL(exp_cca_cross_moving_buffer[ind], object_track.cca_cross_moving_buffer[ind]);
   }
}

/** \purpose
* This test checks if the object is considered to be cross_movable when its lateral position is observed to change in one direction for 4 scans
* and in opposite direction 1 scan
* \req
* NA
*/
TEST(f360_Check_For_Crossing_VRU_Type_Obj, Check_if_obj_is_f_crossing_moveable_when_seen_moving_laterally_for_3scans_in_one_direction)
{
   /** \precond
    * The default setup ensures that cca_cross_moving_buffer[4] will be equal to -1
    * Set the cca_cross_moving_buffer such that it is -1 for 3 scans
    * The cca_cross_moving_buffer should not have 0 as an element
    * The expected cca_cross_moving_buffer = [-1 1 -1 -1 -1]
    */

   object_track.cca_cross_moving_buffer[0] = -1;
   object_track.cca_cross_moving_buffer[1] =  1;
   object_track.cca_cross_moving_buffer[2] = -1;
   object_track.cca_cross_moving_buffer[3] = -1;

   // Set exp data
   for(unsigned int ind = 0; ind < F360_CCA_NON_MOVABLE_MAX_BUFFER_SIZE; ind++)
   {
      exp_cca_cross_moving_buffer[ind] = object_track.cca_cross_moving_buffer[ind];
   }
   exp_cca_cross_moving_buffer[4] = -1;

   /** \action
    * Call Check_For_Crossing_VRU_Type_Obj()
    */
   const bool f_crossing_moveable = Check_For_Crossing_VRU_Type_Obj(host_curvature_rear, calib, object_track);

   /** \result
    * Check that the relevant properties are the same as the defined expected values in the setup
    */

   CHECK_EQUAL(exp_f_crossing_moveable, f_crossing_moveable);
   CHECK_EQUAL(exp_cca_cross_moving_buffer_index, object_track.cca_cross_moving_buffer_index);
   for(unsigned int ind = 0; ind < F360_CCA_NON_MOVABLE_MAX_BUFFER_SIZE; ind++)
   {
      CHECK_EQUAL(exp_cca_cross_moving_buffer[ind], object_track.cca_cross_moving_buffer[ind]);
   }
}

/** \purpose
* This test checks if the object is considered to be cross_movable when its lateral position is observed to change in one direction for 17 scans
* and in opposite direction 3 scans
* \req
* NA
*/
TEST(f360_Check_For_Crossing_VRU_Type_Obj, Check_if_obj_is_f_crossing_moveable_when_seen_moving_laterally_for_17scans_in_one_direction_and_is_further_than_50m_to_host)
{
   /** \precond
    * The default setup ensures that cca_cross_moving_buffer[19] will be equal to -1
    * Set the cca_cross_moving_buffer such that it is -1 for 16 scans
    * The cca_cross_moving_buffer should not have 0 as an element
    */

   object_track.vcs_position.x = 51.0F;
   object_track.vcs_position.y = 1.0;

   object_track.cca_cross_moving_buffer_index = 19U;
   object_track.cca_cross_moving_buffer[0] = -1;
   object_track.cca_cross_moving_buffer[1] = -1;
   object_track.cca_cross_moving_buffer[2] = -1;
   object_track.cca_cross_moving_buffer[3] = -1;
   object_track.cca_cross_moving_buffer[4] = -1;
   object_track.cca_cross_moving_buffer[5] = 1;
   object_track.cca_cross_moving_buffer[6] = -1;
   object_track.cca_cross_moving_buffer[7] = -1;
   object_track.cca_cross_moving_buffer[8] = -1;
   object_track.cca_cross_moving_buffer[9] = -1;
   object_track.cca_cross_moving_buffer[10] = -1;
   object_track.cca_cross_moving_buffer[11] = -1;
   object_track.cca_cross_moving_buffer[12] = -1;
   object_track.cca_cross_moving_buffer[13] = -1;
   object_track.cca_cross_moving_buffer[14] = -1;
   object_track.cca_cross_moving_buffer[15] = -1;
   object_track.cca_cross_moving_buffer[16] = -1;
   object_track.cca_cross_moving_buffer[17] =  1;
   object_track.cca_cross_moving_buffer[18] =  1;

   // Set exp data
   for(unsigned int ind = 0; ind < F360_CCA_NON_MOVABLE_MAX_BUFFER_SIZE; ind++)
   {
      exp_cca_cross_moving_buffer[ind] = object_track.cca_cross_moving_buffer[ind];
   }
   exp_cca_cross_moving_buffer[19] = -1;
   //expect buffer to reset index
   exp_cca_cross_moving_buffer_index = 0;

   /** \action
    * Call Check_For_Crossing_VRU_Type_Obj()
    */
   const bool f_crossing_moveable = Check_For_Crossing_VRU_Type_Obj(host_curvature_rear, calib, object_track);

   /** \result
    * The object is flagged as f_crossing_moveable
    * Buffer index is set to 0
    * Buffer contents as expected
    */
   CHECK_TRUE(f_crossing_moveable);
   CHECK_EQUAL(exp_cca_cross_moving_buffer_index, object_track.cca_cross_moving_buffer_index);
   for(unsigned int ind = 0; ind < F360_CCA_NON_MOVABLE_MAX_BUFFER_SIZE; ind++)
   {
     CHECK_EQUAL(exp_cca_cross_moving_buffer[ind], object_track.cca_cross_moving_buffer[ind]);
   }
}

/** \purpose
* This test checks if the object is not considered to be cross_movable when its lateral position is observed to change in one direction for 16 scans
* and in opposite direction 4 scans (buffer sum = 12), 0 is not present in buffer
* \req
* NA
*/
TEST(f360_Check_For_Crossing_VRU_Type_Obj, Check_if_obj_is_f_crossing_moveable_when_seen_moving_laterally_for_16scans_in_one_direction_4_opposite_0_not_present_and_is_further_than_50m_to_host)
{
   /** \precond
    * The default setup ensures that cca_cross_moving_buffer[19] will be equal to -1
    * Set the cca_cross_moving_buffer such that it is -1 for 16 scans, 1 for 4 scans
    * The cca_cross_moving_buffer should not have 0 as an element
    */

   object_track.vcs_position.x = 51.0F;
   object_track.vcs_position.y = 1.0;

   object_track.cca_cross_moving_buffer_index = 19U;
   object_track.cca_cross_moving_buffer[0] = -1;
   object_track.cca_cross_moving_buffer[1] = 1;
   object_track.cca_cross_moving_buffer[2] = -1;
   object_track.cca_cross_moving_buffer[3] = -1;
   object_track.cca_cross_moving_buffer[4] = -1;
   object_track.cca_cross_moving_buffer[5] = 1;
   object_track.cca_cross_moving_buffer[6] = -1;
   object_track.cca_cross_moving_buffer[7] = -1;
   object_track.cca_cross_moving_buffer[8] = -1;
   object_track.cca_cross_moving_buffer[9] = -1;
   object_track.cca_cross_moving_buffer[10] = -1;
   object_track.cca_cross_moving_buffer[11] = -1;
   object_track.cca_cross_moving_buffer[12] = -1;
   object_track.cca_cross_moving_buffer[13] = -1;
   object_track.cca_cross_moving_buffer[14] = -1;
   object_track.cca_cross_moving_buffer[15] = -1;
   object_track.cca_cross_moving_buffer[16] = -1;
   object_track.cca_cross_moving_buffer[17] =  1;
   object_track.cca_cross_moving_buffer[18] =  1;

   // Set exp data
   for(unsigned int ind = 0; ind < F360_CCA_NON_MOVABLE_MAX_BUFFER_SIZE; ind++)
   {
      exp_cca_cross_moving_buffer[ind] = object_track.cca_cross_moving_buffer[ind];
   }
   exp_cca_cross_moving_buffer[19] = -1;

   /** \action
    * Call Check_For_Crossing_VRU_Type_Obj()
    */
   const bool f_crossing_moveable = Check_For_Crossing_VRU_Type_Obj(host_curvature_rear, calib, object_track);

   /** \result
    * The object is not flagged as f_crossing_moveable
    */
   CHECK_FALSE(f_crossing_moveable);
}


/** \purpose
* This test checks if the object is considered to be cross_movable when its lateral position is observed to change in one direction for 18 scans
* and in opposite direction 1 scan (object thoughought scans in buffer was not moving (0 is present in buffer)- larger threshold applies)
* \req
* NA
*/
TEST(f360_Check_For_Crossing_VRU_Type_Obj, Check_if_obj_is_f_crossing_moveable_when_seen_moving_laterally_for_17scans_in_one_direction_0_is_in_buffer_and_is_further_than_50m_to_host)
{
   /** \precond
    * The default setup ensures that cca_cross_moving_buffer[19] will be equal to -1
    * Set the cca_cross_moving_buffer such that it is -1 for 18 scans, 1 for 1 scan and 0 for 1 scan
    */

   object_track.vcs_position.x = 51.0F;
   object_track.vcs_position.y = 1.0;

   object_track.cca_cross_moving_buffer_index = 19U;
   object_track.cca_cross_moving_buffer[0] = -1;
   object_track.cca_cross_moving_buffer[1] = -1;
   object_track.cca_cross_moving_buffer[2] = -1;
   object_track.cca_cross_moving_buffer[3] = -1;
   object_track.cca_cross_moving_buffer[4] = -1;
   object_track.cca_cross_moving_buffer[5] = 0;
   object_track.cca_cross_moving_buffer[6] = -1;
   object_track.cca_cross_moving_buffer[7] = -1;
   object_track.cca_cross_moving_buffer[8] = -1;
   object_track.cca_cross_moving_buffer[9] = -1;
   object_track.cca_cross_moving_buffer[10] = -1;
   object_track.cca_cross_moving_buffer[11] = -1;
   object_track.cca_cross_moving_buffer[12] = 1;
   object_track.cca_cross_moving_buffer[13] = -1;
   object_track.cca_cross_moving_buffer[14] = -1;
   object_track.cca_cross_moving_buffer[15] = -1;
   object_track.cca_cross_moving_buffer[16] = -1;
   object_track.cca_cross_moving_buffer[17] = -1;
   object_track.cca_cross_moving_buffer[18] = -1;

   // Set exp data
   for(unsigned int ind = 0; ind < F360_CCA_NON_MOVABLE_MAX_BUFFER_SIZE; ind++)
   {
      exp_cca_cross_moving_buffer[ind] = object_track.cca_cross_moving_buffer[ind];
   }
   exp_cca_cross_moving_buffer[19] = -1;

   /** \action
    * Call Check_For_Crossing_VRU_Type_Obj()
    */
   const bool f_crossing_moveable = Check_For_Crossing_VRU_Type_Obj(host_curvature_rear, calib, object_track);

   /** \result
    * The object is flagged as f_crossing_moveable
    */
   CHECK_TRUE(f_crossing_moveable);
}

/** \purpose
* This test checks if the object is considered not to be cross_movable when its longitudinal speed is over 1 m/s
* \req
* NA
*/
TEST(f360_Check_For_Crossing_VRU_Type_Obj, Check_if_obj_is_not_f_crossing_moveable_when_speed_below_threshold)
{
   /** \precond
    * longitudinal velocity of the object is above 1 m/s threshold
    */
   object_track.vcs_velocity.longitudinal = 1.1F;

   /** \action
    * Call Check_For_Crossing_VRU_Type_Obj()
    */
   const bool f_crossing_moveable = Check_For_Crossing_VRU_Type_Obj(host_curvature_rear, calib, object_track);

   /** \result
    * The object is not flagged as f_crossing_moveable
    */
   CHECK_FALSE(f_crossing_moveable);
}

/** @}*/

/** \defgroup  f360_compute_KF_gain_decrease_factor_CCA
*  @{
*/

/** \brief
*  This is a test group containing tests of the function Compute_KF_Gain_Decrease_Factor_CCA()
*  The purpose of this test group is to verify the decrease factor for reducing pseudo position impact on velocity and acceleration, for non movable objects, is calculated correctly
**/
TEST_GROUP(f360_compute_KF_gain_decrease_factor_CCA)
{
   /** \setup
   * Set up calibration data structure.
   * Set up object_track stucture
   * Set up host yaw rate
   * Set commom test variables (decrease_factor_x and decrease_factor_y) to store function output
   * Set commom test variables (exp_decrease_factor_x and exp_decrease_factor_y) to store expected output
   * Set up floating point accuracy for comparing if floats are equal
   **/
  F360_Calibrations_T calib = {};
  F360_Object_Track_T object_track = {};
  float32_t host_yaw_rate;
  float32_t decrease_factor_x;
  float32_t decrease_factor_y;
  float32_t exp_decrease_factor_x;
  float32_t exp_decrease_factor_y;
  const float32_t test_pass_th = 1e-5F;

   /** \setup
   * Initialize calibration structure to default tracker values
   * The host yaw rate is set up to be zero by default
   * The vcs longitudinal and lateral velocity is set up to be greater than 0.3 m/s. The absolute value of velcity needs to be greater than threshold
   * The vcs position and pseudo position are set up, such that the correction from pseudo position will be opposite to the direction of velocity, for both x and y direction
   * The otg_height is set such that the decrease factor will not be affected by the height factor.
   */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calib);
      host_yaw_rate = 0.0F;

      object_track.vcs_velocity.longitudinal = 0.4F;
      object_track.vcs_velocity.lateral = 0.4F;

      object_track.pseudo_vcs_position.x = 3.0F;
      object_track.vcs_position.x = 3.15F;

      object_track.pseudo_vcs_position.y = 3.0F;
      object_track.vcs_position.y = 3.15F;

      object_track.otg_height = 2.0F;
   }
};

/*\purpose
* Verify that the decrease factor is calculated correctly, when the pseudo position correction is opposite to the direction of velocity (greater than 0.3 m/s) for both x and y direction
* In this case, the pseudo position impact should not be limited
*/
TEST(f360_compute_KF_gain_decrease_factor_CCA, Pseudo_Pos_Impact_Against_Velocity)
{
   /** \precond
   * Use default tracker calibrations from test group.
   * Set expected decrease factor to 1.0F, since the correction from pseudo position should not be limited
   **/
   exp_decrease_factor_x = 1.0F;
   exp_decrease_factor_y = 1.0F;
   /** \action
   * Call the function Compute_KF_Gain_Decrease_Factor_CCA()
   **/
   Compute_KF_Gain_Decrease_Factor_CCA(object_track, host_yaw_rate, calib, decrease_factor_x, decrease_factor_y);

   /** \result
   * Verify that the function output matches the expected data
   **/
   DOUBLES_EQUAL_TEXT(exp_decrease_factor_x, decrease_factor_x, test_pass_th, "Incorrect decrease_factor_x");
   DOUBLES_EQUAL_TEXT(exp_decrease_factor_y, decrease_factor_y, test_pass_th, "Incorrect decrease_factor_y");
}

/*\purpose
* Verify that the decrease factor is calculated correctly, when the pseudo position correction is opposite to the direction of velocity for both x and y direction, but the velocity is too low
* In this case, the pseudo position impact should be limited
*/
TEST(f360_compute_KF_gain_decrease_factor_CCA, Pseudo_Pos_Impact_Against_Velocity_But_Object_Velocity_is_Too_Low)
{
   /** \precond
   * Set the abs velocity components to be lesser than 0.3 m/s
   * Set the expected data
   **/
   object_track.vcs_velocity.longitudinal = 0.2F;
   object_track.vcs_velocity.lateral = 0.2F;

   exp_decrease_factor_x = 0.5F;
   exp_decrease_factor_y = 0.5F;
   /** \action
   * Call the function Compute_KF_Gain_Decrease_Factor_CCA()
   **/
   Compute_KF_Gain_Decrease_Factor_CCA(object_track, host_yaw_rate, calib, decrease_factor_x, decrease_factor_y);

   /** \result
   * Verify that the function output matches the expected data
   **/
   DOUBLES_EQUAL_TEXT(exp_decrease_factor_x, decrease_factor_x, test_pass_th, "Incorrect decrease_factor_x");
   DOUBLES_EQUAL_TEXT(exp_decrease_factor_y, decrease_factor_y, test_pass_th, "Incorrect decrease_factor_y");
}

/*\purpose
* Verify that the decrease factor is calculated correctly, when the pseudo position correction is in the same direction of velocity, for both x and y direction
* In this case, the pseudo position impact should be limited
*/
TEST(f360_compute_KF_gain_decrease_factor_CCA, Pseudo_Pos_Impact_Towards_Velocity)
{
   /** \precond
   * Set vcs position and pseudo position (same as setup) such that the correction from pseudo postion will be in the same direction as velocity (for both x and y direction)
   * Set the expected data
   **/
   object_track.vcs_position.x = 2.85F;
   object_track.vcs_position.y = 2.85F;

   exp_decrease_factor_x = 0.5F;
   exp_decrease_factor_y = 0.5F;
   /** \action
   * Call the function Compute_KF_Gain_Decrease_Factor_CCA()
   **/
   Compute_KF_Gain_Decrease_Factor_CCA(object_track, host_yaw_rate, calib, decrease_factor_x, decrease_factor_y);

   /** \result
   * Verify that the function output matches the expected data
   **/
   DOUBLES_EQUAL_TEXT(exp_decrease_factor_x, decrease_factor_x, test_pass_th, "Incorrect decrease_factor_x");
   DOUBLES_EQUAL_TEXT(exp_decrease_factor_y, decrease_factor_y, test_pass_th, "Incorrect decrease_factor_y");
}

/*\purpose
* Verify that the decrease factor is calculated correctly, when the pseudo position correction is in the same direction of velocity, for both x and y direction, but the velocity is too low
* In this case, the pseudo position impact should be limited
*/
TEST(f360_compute_KF_gain_decrease_factor_CCA, Pseudo_Pos_Impact_Towards_Velocity_And_Object_Velocity_is_Too_Low)
{
   /** \precond
   * Set vcs position and pseudo position (same as setup) such that the correction from pseudo postion will be in the same direction as velocity (for both x and y direction)
   * Set the abs velocity components to be lesser than 0.3 m/s
   * Set the expected data
   **/
   object_track.vcs_velocity.longitudinal = 0.2F;
   object_track.vcs_velocity.lateral = 0.2F;

   object_track.vcs_position.x = 2.85F;
   object_track.vcs_position.y = 2.85F;

   exp_decrease_factor_x = 0.5F;
   exp_decrease_factor_y = 0.5F;
   /** \action
   * Call the function Compute_KF_Gain_Decrease_Factor_CCA()
   **/
   Compute_KF_Gain_Decrease_Factor_CCA(object_track, host_yaw_rate, calib, decrease_factor_x, decrease_factor_y);

   /** \result
   * Verify that the function output matches the expected data
   **/
   DOUBLES_EQUAL_TEXT(exp_decrease_factor_x, decrease_factor_x, test_pass_th, "Incorrect decrease_factor_x");
   DOUBLES_EQUAL_TEXT(exp_decrease_factor_y, decrease_factor_y, test_pass_th, "Incorrect decrease_factor_y");
}

/*\purpose
* Verify that the decrease factor is calculated correctly, when the height is lower than 5.0m
* and height factor should not affect decrease factor
*/
TEST(f360_compute_KF_gain_decrease_factor_CCA, Height_Less_Than_Or_Equal_To_5m)
{
   /** \precond
   * Set vcs position and pseudo position such that the correction from pseudo
   * postion will be in the same direction as velocity (for both x and y direction)
   * Decrease factor should be 0.5
   * Set height to 5.0m such that height factor will not affect decrease factor
   * Set the expected data
   **/
   object_track.vcs_position.x = 2.85F;
   object_track.vcs_position.y = 2.85F;
   object_track.otg_height = 5.0F;
   exp_decrease_factor_x = 0.5F;
   exp_decrease_factor_y = 0.5F;
   /** \action
   * Call the function Compute_KF_Gain_Decrease_Factor_CCA()
   **/
   Compute_KF_Gain_Decrease_Factor_CCA(object_track, host_yaw_rate, calib, decrease_factor_x, decrease_factor_y);

   /** \result
   * Verify that the function output matches the expected data
   **/
   DOUBLES_EQUAL_TEXT(exp_decrease_factor_x, decrease_factor_x, test_pass_th, "Incorrect decrease_factor_x");
   DOUBLES_EQUAL_TEXT(exp_decrease_factor_y, decrease_factor_y, test_pass_th, "Incorrect decrease_factor_y");
}

/*\purpose
* Verify that the decrease factor is calculated correctly, when height factor should affect the decrease factor
*/
TEST(f360_compute_KF_gain_decrease_factor_CCA, Height_Between_5m_And_10m)
{
   /** \precond
   * Set vcs position and pseudo position such that the correction from pseudo
   * postion will be in the same direction as velocity (for both x and y direction)
   * Set height to 7.5m such that height factor will reduce decrease factor by 0.5.
   * Set the expected data
   **/
   object_track.vcs_position.x = 2.85F;
   object_track.vcs_position.y = 2.85F;
   object_track.otg_height = 7.5;
   exp_decrease_factor_x = 0.25025F; //0.5 * (1 - 0.2 * 2.5)
   exp_decrease_factor_y = 0.25025F;
   /** \action
   * Call the function Compute_KF_Gain_Decrease_Factor_CCA()
   **/
   Compute_KF_Gain_Decrease_Factor_CCA(object_track, host_yaw_rate, calib, decrease_factor_x, decrease_factor_y);

   /** \result
   * Verify that the function output matches the expected data
   **/
   DOUBLES_EQUAL_TEXT(exp_decrease_factor_x, decrease_factor_x, test_pass_th, "Incorrect decrease_factor_x");
   DOUBLES_EQUAL_TEXT(exp_decrease_factor_y, decrease_factor_y, test_pass_th, "Incorrect decrease_factor_y");
}

/*\purpose
* Verify that the decrease factor is calculated correctly, when height factor should affect the decrease factor
*/
TEST(f360_compute_KF_gain_decrease_factor_CCA, Height_Greater_than_10m)
{
   /** \precond
   * Set vcs position and pseudo position such that the correction from pseudo
   * postion will be in the same direction as velocity (for both x and y direction)
   * Set height to 10m such that height factor will reduce decrease factor by 0.001.
   * Set the expected data
   **/
   object_track.vcs_position.x = 2.85F;
   object_track.vcs_position.y = 2.85F;
   object_track.otg_height = 11.0F;
   exp_decrease_factor_x = 0.0F;
   exp_decrease_factor_y = 0.0F;
   /** \action
   * Call the function Compute_KF_Gain_Decrease_Factor_CCA()
   **/
   Compute_KF_Gain_Decrease_Factor_CCA(object_track, host_yaw_rate, calib, decrease_factor_x, decrease_factor_y);

   /** \result
   * Verify that the function output matches the expected data
   **/
   DOUBLES_EQUAL_TEXT(exp_decrease_factor_x, decrease_factor_x, test_pass_th, "Incorrect decrease_factor_x");
   DOUBLES_EQUAL_TEXT(exp_decrease_factor_y, decrease_factor_y, test_pass_th, "Incorrect decrease_factor_y");
}
/** @}*/

/** \defgroup  f360_Force_Pseudo_Pos_Vel_Lim_Impact_Logic
 *  @{
 */

 /** \brief
  * This test group sets up test data and expected output for Force_Pseudo_Pos_Vel_Lim_Impact_Logic
  */
TEST_GROUP(f360_Force_Pseudo_Pos_Vel_Lim_Impact_Logic)
{
   float32_t host_speed;
   F360_Object_Track_T object_track;
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   uint32_t dets_idx[MAX_DETS_IN_OBJ_TRK];
   uint32_t num_dets;
   bool exp_result;

   /** \setup
   * Setup general input for Force_Pseudo_Pos_Vel_Lim_Impact_Logic()
   */
   TEST_SETUP()
   {
      // Set up detection properties
      memset(det_props, 0, sizeof(det_props));
      memset(dets_idx, 0, sizeof(dets_idx));
      num_dets = 0;

      // Set default values for object_track
      object_track.vcs_position.x = 5.0F;
      object_track.vcs_position.y = 2.0F;

      // Set default value for host speed
      host_speed = F360_KPH2MPS(15.0F);

      exp_result = false;
   }
};

/** \purpose
 * Verify that the function returns true when all conditions are met
 * \req
 * NA
 */
TEST(f360_Force_Pseudo_Pos_Vel_Lim_Impact_Logic, All_Conditions_Met)
{
   /** \precond
    * Set up conditions that should result in true
    */
   num_dets = 3;
   for (uint32_t i = 0; i < num_dets; i++)
   {
      dets_idx[i] = i;
      det_props[i].range_rate_compensated = 0.05F;
   }
   exp_result = true;

   /** \action
    * Call Force_Pseudo_Pos_Vel_Lim_Impact_Logic()
    */
   const bool result = Force_Pseudo_Pos_Vel_Lim_Impact_Logic(host_speed, object_track, det_props, dets_idx, num_dets);

   /** \result
    * Check that the function returns the expected result
    */
   CHECK_EQUAL(exp_result, result);
}

/** \purpose
 * Verify that the function returns false when object is too far
 * \req
 * NA
 */
TEST(f360_Force_Pseudo_Pos_Vel_Lim_Impact_Logic, Object_Too_Far_Long)
{
   /** \precond
    * Set object position beyond the distance threshold longitudinally
    */
   object_track.vcs_position.x = 15.0F;
   num_dets = 3;
   for (uint32_t i = 0; i < num_dets; i++)
   {
      dets_idx[i] = i;
      det_props[i].range_rate_compensated = 0.05F;
   }
   exp_result = false;

   /** \action
    * Call Force_Pseudo_Pos_Vel_Lim_Impact_Logic()
    */
   const bool result = Force_Pseudo_Pos_Vel_Lim_Impact_Logic(host_speed, object_track, det_props, dets_idx, num_dets);

   /** \result
    * Check that the function returns the expected result
    */
   CHECK_EQUAL(exp_result, result);
}

/** \purpose
 * Verify that the function returns false when object is too far
 * \req
 * NA
 */
TEST(f360_Force_Pseudo_Pos_Vel_Lim_Impact_Logic, Object_Too_Far_Lat)
{
   /** \precond
    * Set object position beyond the distance threshold laterally
    */
   object_track.vcs_position.y = -15.0F;
   num_dets = 3;
   for (uint32_t i = 0; i < num_dets; i++)
   {
      dets_idx[i] = i;
      det_props[i].range_rate_compensated = 0.05F;
   }
   exp_result = false;

   /** \action
    * Call Force_Pseudo_Pos_Vel_Lim_Impact_Logic()
    */
   const bool result = Force_Pseudo_Pos_Vel_Lim_Impact_Logic(host_speed, object_track, det_props, dets_idx, num_dets);

   /** \result
    * Check that the function returns the expected result
    */
   CHECK_EQUAL(exp_result, result);
}

/** \purpose
 * Verify that the function returns false when host speed is too high
 * \req
 * NA
 */
TEST(f360_Force_Pseudo_Pos_Vel_Lim_Impact_Logic, Host_Speed_Too_High)
{
   /** \precond
    * Set host speed above the threshold
    */
   host_speed = F360_KPH2MPS(25.0F);
   num_dets = 3;
   for (uint32_t i = 0; i < num_dets; i++)
   {
      dets_idx[i] = i;
      det_props[i].range_rate_compensated = 0.05F;
   }
   exp_result = false;

   /** \action
    * Call Force_Pseudo_Pos_Vel_Lim_Impact_Logic()
    */
   const bool result = Force_Pseudo_Pos_Vel_Lim_Impact_Logic(host_speed, object_track, det_props, dets_idx, num_dets);

   /** \result
    * Check that the function returns the expected result
    */
   CHECK_EQUAL(exp_result, result);
}

/** \purpose
 * Verify that the function returns false when one detection has high range rate
 * \req
 * NA
 */
TEST(f360_Force_Pseudo_Pos_Vel_Lim_Impact_Logic, One_Detection_High_Range_Rate)
{
   /** \precond
    * Set one detection with range rate above the threshold
    */
   num_dets = 3;
   for (uint32_t i = 0; i < num_dets; i++)
   {
      dets_idx[i] = i;
      det_props[i].range_rate_compensated = 0.05F;
   }
   det_props[1].range_rate_compensated = 0.2F;
   exp_result = false;

   /** \action
    * Call Force_Pseudo_Pos_Vel_Lim_Impact_Logic()
    */
   const bool result = Force_Pseudo_Pos_Vel_Lim_Impact_Logic(host_speed, object_track, det_props, dets_idx, num_dets);

   /** \result
    * Check that the function returns the expected result
    */
   CHECK_EQUAL(exp_result, result);
}

/** \purpose
 * Verify that the function returns false when there are no detections
 * \req
 * NA
 */
TEST(f360_Force_Pseudo_Pos_Vel_Lim_Impact_Logic, No_Detections)
{
   /** \precond
    * Set number of detections to zero
    */
   num_dets = 0;
   exp_result = false;

   /** \action
    * Call Force_Pseudo_Pos_Vel_Lim_Impact_Logic()
    */
   const bool result = Force_Pseudo_Pos_Vel_Lim_Impact_Logic(host_speed, object_track, det_props, dets_idx, num_dets);

   /** \result
    * Check that the function returns the expected result
    */
   CHECK_EQUAL(exp_result, result);
}

/** @}*/
