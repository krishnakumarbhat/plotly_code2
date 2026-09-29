/** \file
 * This file contains unit tests for content of f360_cvt_estimator.cpp file
 */

#include "f360_constants.h"
#include "f360_math.h"
#include "f360_cvt_estimator.h"
#include <CppUTest/TestHarness.h>
#include <vector>
#include <cstring>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_cvt_estimator_init_and_execute
 *  @{
 */

/** \brief
 * CV Trailer initialization function is tested
 */
TEST_GROUP(f360_cvt_estimator_init_and_execute)
{
   // Declare the variables and structs used in the tests
   F360_CVT_Initialization_Data_T init_data{};
   F360_CVT_State_T cvt_state{};
   F360_CVT_Input_Data_T cvt_input;
   float32_t epsilon;
   float32_t k_init_trailer_length;
   F360_Calibrations_T calib{};
   /** \setup
    * Setup the init values to make it eligible for completing the test
    */
   TEST_SETUP()
   {
      // Initialize calibrations
      Initialize_Tracker_Calibrations(calib);

      epsilon = 1E-3F;
      k_init_trailer_length = 5.0F;

      // The default values making the initialization successful
      init_data.radar_id = 2U;
      init_data.radar_vcs_longpos = -1.0F;
      init_data.radar_vcs_latpos = 1.0F;
      init_data.host_length = 5.0F;
      init_data.host_width = 2.0F;

      init_data.host_rear_axle_vcs_longpos = -3.9F;
      cvt_input.host_rear_axle_vcs_longpos = -3.9F;

      init_data.joint1_vcs_longpos_min = -1.0F * init_data.host_length - 3.5F; // 3.5m behind host
      init_data.joint1_vcs_longpos_max = -0.5F * init_data.host_length + 0.5F; // 0.5m ahead of mid host
      init_data.link1_wheelbase_min = 3.0F;
      init_data.link1_wheelbase_max = 30.0F;
      init_data.link2_wheelbase_min = 5.0F;
      init_data.link2_wheelbase_max = 30.0F;

      init_data.one_link_model.n_updates = 1U;
      init_data.one_link_model.link1_angle = 0.0F;
      init_data.one_link_model.joint1_vcs_longpos = init_data.host_rear_axle_vcs_longpos;
      init_data.one_link_model.link1_wheelbase = 10.0F;
      init_data.one_link_model.full_vehicle_length = init_data.host_length + init_data.one_link_model.link1_wheelbase + 2.0F;

      init_data.two_link_model.n_updates = 2U;
      init_data.two_link_model.full_vehicle_length = 12.0F;
      init_data.two_link_model.link1_angle = 0.0F;
      init_data.two_link_model.link2_angle = 0.0F;
      init_data.two_link_model.joint1_vcs_longpos = -init_data.host_length;
      init_data.two_link_model.link1_wheelbase = 5.0F;
      init_data.two_link_model.link2_wheelbase = 10.0F;
      init_data.two_link_model.full_vehicle_length = init_data.host_length + init_data.two_link_model.link1_wheelbase + init_data.two_link_model.link2_wheelbase + 2.0F;

      cvt_state.f_initiated_from_states = false;
      cvt_state.best_trailer_model = TRAILER_MODEL_TWO_LINK;
      cvt_state.one_link.joint_vcs_longpos = -1.1F;
      cvt_state.one_link.joint_dist_to_wheels = 2.1F;
      cvt_state.one_link.ekf_state_errcov[0][0] = 0.1F;
      cvt_state.one_link.ekf_state_errcov[1][1] = 0.1F;
      cvt_state.one_link.ekf_state_errcov[2][2] = 0.1F;
      cvt_state.one_link.trailer_length = 2.1F;
      cvt_state.one_link.n_updates = 100U;
      cvt_state.one_link.ekf_state[0] = init_data.one_link_model.link1_angle + 0.0001F;
      cvt_state.one_link.ekf_state[1] = -(init_data.one_link_model.joint1_vcs_longpos - init_data.host_rear_axle_vcs_longpos) + 0.1F;
      cvt_state.one_link.ekf_state[2] = init_data.one_link_model.link1_wheelbase + 0.1F;
      cvt_state.one_link.full_vehicle_length = 60.0F;
      cvt_state.two_link.full_vehicle_length = 60.0F;

      cvt_state.two_link.joint1_vcs_longpos = -1.1F;
      cvt_state.two_link.joint2_vcs_longpos = -1.1F;
      cvt_state.two_link.joint1_dist_to_wheels = 2.1F;
      cvt_state.two_link.joint2_dist_to_wheels = 2.1F;
      cvt_state.two_link.ekf_state_errcov[0][0] = 0.1F;
      cvt_state.two_link.ekf_state_errcov[1][1] = 0.1F;
      cvt_state.two_link.ekf_state_errcov[2][2] = 0.1F;
      cvt_state.two_link.ekf_state_errcov[3][3] = 0.1F;
      cvt_state.two_link.ekf_state_errcov[4][4] = 0.1F;
      cvt_state.two_link.trailer1_length = 2.1F;
      cvt_state.two_link.trailer2_length = 2.1F;
      cvt_state.two_link.n_updates = 200U;
      cvt_state.two_link.ekf_state[0] = init_data.two_link_model.link1_angle + 0.0001F;
      cvt_state.two_link.ekf_state[1] = -(init_data.two_link_model.joint1_vcs_longpos - init_data.host_rear_axle_vcs_longpos) + 0.1F;
      cvt_state.two_link.ekf_state[2] = init_data.two_link_model.link1_wheelbase + 0.1F;
      cvt_state.two_link.ekf_state[3] = init_data.two_link_model.link2_angle + 0.0001F;
      cvt_state.two_link.ekf_state[4] = init_data.two_link_model.link2_wheelbase + 0.1F;
   }

};

/** \purpose
 * Test relevant parameters of the CV Trailer has been initialized as intended.
 * Depending on the Flag "f_initiated_from_states", the initialization result should be different.
 * This test is for this flag to be positive, namely initializing from the state of the truck, which was saved before the engine stalled.
 * The test involves the radar properties, host properties, cv trailer state constraints, 
 * EKF states and matrices, model selection flag and initialization_complete flag.
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_data_are_loaded_correctly_when_initiated_from_states)
{
   /** \precond
    * Use the Setup values as in default
    */

   F360_CVT_State_T cvt_state_copy;
   // make the cvt_state valid for the initialization from state guard
   cvt_state.one_link.joint_vcs_longpos = -1.1F;
   cvt_state.one_link.joint_dist_to_wheels = 2.1F;
   cvt_state.one_link.ekf_state_errcov[0][0] = 0.1F;
   cvt_state.one_link.ekf_state_errcov[1][1] = 0.1F;
   cvt_state.one_link.ekf_state_errcov[2][2] = 0.1F;
   cvt_state.one_link.trailer_length = 2.1F;
   cvt_state.one_link.full_vehicle_length = init_data.host_length+0.1F;
   cvt_state.two_link.joint1_vcs_longpos = -1.1F;
   cvt_state.two_link.joint2_vcs_longpos = -1.1F;
   cvt_state.two_link.joint1_dist_to_wheels = 2.1F;
   cvt_state.two_link.joint2_dist_to_wheels = 2.1F;
   cvt_state.two_link.ekf_state_errcov[0][0] = 0.1F;
   cvt_state.two_link.ekf_state_errcov[1][1] = 0.1F;
   cvt_state.two_link.ekf_state_errcov[2][2] = 0.1F;
   cvt_state.two_link.ekf_state_errcov[3][3] = 0.1F;
   cvt_state.two_link.ekf_state_errcov[4][4] = 0.1F;
   cvt_state.two_link.trailer1_length = 2.1F;
   cvt_state.two_link.trailer2_length = 2.1F;
   cvt_state.two_link.full_vehicle_length = init_data.host_length+0.1F;
   cvt_state.best_trailer_model = TRAILER_MODEL_ONE_LINK;
   
   (void)std::memcpy(&cvt_state_copy, &cvt_state, sizeof(cvt_state_copy));

   // Reset the cvt_state
   (void)std::memcpy(&cvt_state, &cvt_state_copy, sizeof(cvt_state));
   const bool f_initiated_from_states = true;
   cvt_state.f_initiated_from_states = f_initiated_from_states;

   /** \action
    * Call CVT_Initialize to initialize the cvt_state
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * Expect the init_data to be parsed into cvt_state and init to be complete.
    * Check all internal states that are supposed to be arranged.
    */
   // check host properties
   DOUBLES_EQUAL_TEXT(init_data.radar_id, cvt_state.radar_id, epsilon, "Wrong initialization value for radar_id of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.radar_vcs_longpos, cvt_state.radar_vcs_longpos, epsilon, "Wrong initialization value for radar_vcs_longpos of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.radar_vcs_latpos, cvt_state.radar_vcs_latpos, epsilon, "Wrong initialization value for radar_vcs_latpos of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.host_length, cvt_state.host_length, epsilon, "Wrong initialization value for host_length of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.host_width, cvt_state.host_width, epsilon, "Wrong initialization value for host_width of CV Trailer State");

   // check constraints
   DOUBLES_EQUAL_TEXT(-0.5F * F360_PI, cvt_state.state_constraints.min_x0,  epsilon, "Wrong initialization value for state_constraints.min_x0 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.5F * F360_PI, cvt_state.state_constraints.max_x0,  epsilon, "Wrong initialization value for state_constraints.max_x0 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(-(init_data.joint1_vcs_longpos_max - init_data.host_rear_axle_vcs_longpos), cvt_state.state_constraints.min_x1,  epsilon, "Wrong initialization value for state_constraints.min_x1 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(-(init_data.joint1_vcs_longpos_min - init_data.host_rear_axle_vcs_longpos), cvt_state.state_constraints.max_x1,  epsilon, "Wrong initialization value for state_constraints.max_x1 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.link1_wheelbase_min, cvt_state.state_constraints.min_x2,  epsilon, "Wrong initialization value for state_constraints.min_x2 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.link1_wheelbase_max, cvt_state.state_constraints.max_x2,  epsilon, "Wrong initialization value for state_constraints.max_x2 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(-0.75F * F360_PI, cvt_state.state_constraints.min_x3,  epsilon, "Wrong initialization value for state_constraints.min_x3 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.75F * F360_PI, cvt_state.state_constraints.max_x3,  epsilon, "Wrong initialization value for state_constraints.max_x3 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.link2_wheelbase_min, cvt_state.state_constraints.min_x4,  epsilon, "Wrong initialization value for state_constraints.min_x4 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.link2_wheelbase_max, cvt_state.state_constraints.max_x4,  epsilon, "Wrong initialization value for state_constraints.max_x4 of CV Trailer State");

   DOUBLES_EQUAL_TEXT(cvt_state_copy.one_link.ekf_state_errcov[0][0], cvt_state.one_link.ekf_state_errcov[0][0], epsilon, "Initialization from states failed for cvt_state.one_link.ekf_state_errcov[0][0], value not loaded from memory correctly!");
   DOUBLES_EQUAL_TEXT(cvt_state_copy.one_link.ekf_state_errcov[1][1], cvt_state.one_link.ekf_state_errcov[1][1], epsilon, "Initialization from states failed for cvt_state.one_link.ekf_state_errcov[1][1], value not loaded from memory correctly!");
   DOUBLES_EQUAL_TEXT(cvt_state_copy.one_link.ekf_state_errcov[2][2], cvt_state.one_link.ekf_state_errcov[2][2], epsilon, "Initialization from states failed for cvt_state.one_link.ekf_state_errcov[2][2], value not loaded from memory correctly!");

   DOUBLES_EQUAL_TEXT(cvt_state_copy.one_link.n_updates, cvt_state.one_link.n_updates, epsilon, "Initialization from states failed for cvt_state.one_link.n_updates, value not loaded from memory correctly!");
   DOUBLES_EQUAL_TEXT(cvt_state_copy.one_link.full_vehicle_length, cvt_state.one_link.full_vehicle_length, epsilon, "Initialization from states failed for cvt_state.one_link.full_vehicle_length, value not loaded from memory correctly!");

   DOUBLES_EQUAL_TEXT(cvt_state_copy.two_link.ekf_state_errcov[0][0], cvt_state.two_link.ekf_state_errcov[0][0], epsilon, "Initialization from states failed for cvt_state.two_link.ekf_state_errcov[0][0], value not loaded from memory correctly!");
   DOUBLES_EQUAL_TEXT(cvt_state_copy.two_link.ekf_state_errcov[1][1], cvt_state.two_link.ekf_state_errcov[1][1], epsilon, "Initialization from states failed for cvt_state.two_link.ekf_state_errcov[1][1], value not loaded from memory correctly!");
   DOUBLES_EQUAL_TEXT(cvt_state_copy.two_link.ekf_state_errcov[2][2], cvt_state.two_link.ekf_state_errcov[2][2], epsilon, "Initialization from states failed for cvt_state.two_link.ekf_state_errcov[2][2], value not loaded from memory correctly!");
   DOUBLES_EQUAL_TEXT(cvt_state_copy.two_link.ekf_state_errcov[3][3], cvt_state.two_link.ekf_state_errcov[3][3], epsilon, "Initialization from states failed for cvt_state.two_link.ekf_state_errcov[3][3], value not loaded from memory correctly!");
   DOUBLES_EQUAL_TEXT(cvt_state_copy.two_link.ekf_state_errcov[4][4], cvt_state.two_link.ekf_state_errcov[4][4], epsilon, "Initialization from states failed for cvt_state.two_link.ekf_state_errcov[4][4], value not loaded from memory correctly!");

   DOUBLES_EQUAL_TEXT(cvt_state_copy.one_link.ekf_state[0], cvt_state.one_link.ekf_state[0], epsilon, "Initialization from states failed for cvt_state.one_link.ekf_state[0], value not loaded from memory correctly!");
   DOUBLES_EQUAL_TEXT(cvt_state_copy.one_link.ekf_state[1], cvt_state.one_link.ekf_state[1], epsilon, "Initialization from states failed for cvt_state.one_link.ekf_state[1], value not loaded from memory correctly!");
   DOUBLES_EQUAL_TEXT(cvt_state_copy.one_link.ekf_state[2], cvt_state.one_link.ekf_state[2], epsilon, "Initialization from states failed for cvt_state.one_link.ekf_state[2], value not loaded from memory correctly!");

   DOUBLES_EQUAL_TEXT(cvt_state_copy.two_link.ekf_state[0], cvt_state.two_link.ekf_state[0], epsilon, "Initialization from states failed for cvt_state.two_link.ekf_state[0], value not loaded from memory correctly!");   
   DOUBLES_EQUAL_TEXT(cvt_state_copy.two_link.ekf_state[1], cvt_state.two_link.ekf_state[1], epsilon, "Initialization from states failed for cvt_state.two_link.ekf_state[1], value not loaded from memory correctly!");
   DOUBLES_EQUAL_TEXT(cvt_state_copy.two_link.ekf_state[2], cvt_state.two_link.ekf_state[2], epsilon, "Initialization from states failed for cvt_state.two_link.ekf_state[2], value not loaded from memory correctly!");
   DOUBLES_EQUAL_TEXT(cvt_state_copy.two_link.ekf_state[3], cvt_state.two_link.ekf_state[3], epsilon, "Initialization from states failed for cvt_state.two_link.ekf_state[3], value not loaded from memory correctly!");
   DOUBLES_EQUAL_TEXT(cvt_state_copy.two_link.ekf_state[4], cvt_state.two_link.ekf_state[4], epsilon, "Initialization from states failed for cvt_state.two_link.ekf_state[4], value not loaded from memory correctly!");

   DOUBLES_EQUAL_TEXT(cvt_state_copy.two_link.n_updates, cvt_state.two_link.n_updates, epsilon, "Initialization from states failed for cvt_state.two_link.n_updates, value not loaded from memory correctly!");
   DOUBLES_EQUAL_TEXT(cvt_state_copy.two_link.full_vehicle_length, cvt_state.two_link.full_vehicle_length, epsilon, "Initialization from states failed for cvt_state.two_link.full_vehicle_length, value not loaded from memory correctly!");

   DOUBLES_EQUAL_TEXT(0U, cvt_state.best_model_change_counter, epsilon, "Wrong initialization value for best_model_change_counter of CV Trailer State");
   DOUBLES_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, epsilon, "Wrong initialization value for best_trailer_model of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.host_length, cvt_state.host_length, epsilon, "Wrong initialization value for host_length of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.host_width, cvt_state.host_width, epsilon, "Wrong initialization value for host_width of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.radar_vcs_longpos, cvt_state.radar_vcs_longpos, epsilon, "Wrong initialization value for radar_vcs_longpos of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.radar_vcs_latpos, cvt_state.radar_vcs_latpos, epsilon, "Wrong initialization value for radar_vcs_latpos of CV Trailer State");

   DOUBLES_EQUAL_TEXT(TRAILER_FILTER_STATE_INIT, cvt_state.one_link.filter_state, epsilon, "Wrong initialization value for one_link.filter_state of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.0F, cvt_state.one_link.angle_rate, epsilon, "Wrong initialization value for one_link.angle_rate of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.0F, cvt_state.one_link.prev_trailer_angle, epsilon, "Wrong initialization value for one_link.prev_trailer_angle of CV Trailer State");
   DOUBLES_EQUAL_TEXT(k_init_trailer_length, cvt_state.one_link.trailer_length, epsilon, "Wrong initialization value for one_link.trailer_length of CV Trailer State");
   DOUBLES_EQUAL_TEXT(2.55F, cvt_state.one_link.trailer_width, epsilon, "Wrong initialization value for one_link.trailer_width of CV Trailer State");

   DOUBLES_EQUAL_TEXT(TRAILER_FILTER_STATE_INIT, cvt_state.two_link.filter_state, epsilon, "Wrong initialization value for two_link.filter_state of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.0F, cvt_state.two_link.angle_rate1, epsilon, "Wrong initialization value for two_link.angle_rate1 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.0F, cvt_state.two_link.angle_rate2, epsilon, "Wrong initialization value for two_link.angle_rate2 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.0F, cvt_state.two_link.prev_trailer1_angle, epsilon, "Wrong initialization value for two_link.prev_trailer1_angle of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.0F, cvt_state.two_link.prev_trailer2_angle, epsilon, "Wrong initialization value for two_link.prev_trailer2_angle of CV Trailer State");
   DOUBLES_EQUAL_TEXT(k_init_trailer_length, cvt_state.two_link.trailer1_length, epsilon, "Wrong initialization value for two_link.trailer1_length of CV Trailer State");
   DOUBLES_EQUAL_TEXT(k_init_trailer_length, cvt_state.two_link.trailer2_length, epsilon, "Wrong initialization value for two_link.trailer2_length of CV Trailer State");
   DOUBLES_EQUAL_TEXT(2.55F, cvt_state.two_link.trailer1_width, epsilon, "Wrong initialization value for two_link.trailer1_width of CV Trailer State");
   DOUBLES_EQUAL_TEXT(2.55F, cvt_state.two_link.trailer2_width, epsilon, "Wrong initialization value for two_link.trailer2_width of CV Trailer State");

   // check flag for successful initialization
   CHECK_TRUE_TEXT(cvt_state.f_init_complete, "cvt_state.f_init_complete Flag not computed correctly");


}

/** \purpose
 * Test relevant parameters of the CV Trailer has been initialized as intended.
 * When Flag "f_initiated_from_states", the initialization result should use 
 * the previous states from NVM, i.e, cvt_state, ONLY IF the loaded data is valid.
 * This test is for this flag to be true, while the data is invalid.
 * The test involves the radar properties, host properties, cv trailer state constraints, 
 * EKF states and matrices, model selection flag and initialization_complete flag.
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_scratch_if_the_loaded_data_is_invalid)
{
   /** \precond
    * Use the Setup values as in default
    */

   F360_CVT_State_T cvt_state_copy;

   // make one cvt_state member invalid for the initialization from state guard
   cvt_state.one_link.full_vehicle_length = 5.0F;

   // make other cvt_state members valid for the initialization from state guard
   cvt_state.one_link.joint_vcs_longpos = -1.1F;
   cvt_state.one_link.joint_dist_to_wheels = 2.1F;
   cvt_state.one_link.ekf_state_errcov[0][0] = 0.1F;
   cvt_state.one_link.ekf_state_errcov[1][1] = 0.1F;
   cvt_state.one_link.ekf_state_errcov[2][2] = 0.1F;
   cvt_state.two_link.joint1_vcs_longpos = -1.1F;
   cvt_state.two_link.joint2_vcs_longpos = -1.1F;
   cvt_state.two_link.joint1_dist_to_wheels = 2.1F;
   cvt_state.two_link.joint2_dist_to_wheels = 2.1F;
   cvt_state.two_link.ekf_state_errcov[0][0] = 0.1F;
   cvt_state.two_link.ekf_state_errcov[1][1] = 0.1F;
   cvt_state.two_link.ekf_state_errcov[2][2] = 0.1F;
   cvt_state.two_link.ekf_state_errcov[3][3] = 0.1F;
   cvt_state.two_link.ekf_state_errcov[4][4] = 0.1F;
   cvt_state.two_link.full_vehicle_length = init_data.host_length+0.1F;
   
   (void)std::memcpy(&cvt_state_copy, &cvt_state, sizeof(cvt_state_copy));

   // Reset the cvt_state
   (void)std::memcpy(&cvt_state, &cvt_state_copy, sizeof(cvt_state));
   const bool f_initiated_from_states = true;
   cvt_state.f_initiated_from_states = f_initiated_from_states;

   /** \action
    * Call CVT_Initialize to initialize the cvt_state
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * Expect the init_data to be parsed into cvt_state and init to be complete.
    * Check all internal states that are supposed to be arranged.
    */
   // check host properties
   DOUBLES_EQUAL_TEXT(init_data.radar_id, cvt_state.radar_id, epsilon, "Wrong initialization value for radar_id of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.radar_vcs_longpos, cvt_state.radar_vcs_longpos, epsilon, "Wrong initialization value for radar_vcs_longpos of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.radar_vcs_latpos, cvt_state.radar_vcs_latpos, epsilon, "Wrong initialization value for radar_vcs_latpos of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.host_length, cvt_state.host_length, epsilon, "Wrong initialization value for host_length of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.host_width, cvt_state.host_width, epsilon, "Wrong initialization value for host_width of CV Trailer State");

   // check constraints
   DOUBLES_EQUAL_TEXT(-0.5F * F360_PI, cvt_state.state_constraints.min_x0,  epsilon, "Wrong initialization value for state_constraints.min_x0 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.5F * F360_PI, cvt_state.state_constraints.max_x0,  epsilon, "Wrong initialization value for state_constraints.max_x0 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(-(init_data.joint1_vcs_longpos_max - init_data.host_rear_axle_vcs_longpos), cvt_state.state_constraints.min_x1,  epsilon, "Wrong initialization value for state_constraints.min_x1 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(-(init_data.joint1_vcs_longpos_min - init_data.host_rear_axle_vcs_longpos), cvt_state.state_constraints.max_x1,  epsilon, "Wrong initialization value for state_constraints.max_x1 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.link1_wheelbase_min, cvt_state.state_constraints.min_x2,  epsilon, "Wrong initialization value for state_constraints.min_x2 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.link1_wheelbase_max, cvt_state.state_constraints.max_x2,  epsilon, "Wrong initialization value for state_constraints.max_x2 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(-0.75F * F360_PI, cvt_state.state_constraints.min_x3,  epsilon, "Wrong initialization value for state_constraints.min_x3 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.75F * F360_PI, cvt_state.state_constraints.max_x3,  epsilon, "Wrong initialization value for state_constraints.max_x3 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.link2_wheelbase_min, cvt_state.state_constraints.min_x4,  epsilon, "Wrong initialization value for state_constraints.min_x4 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.link2_wheelbase_max, cvt_state.state_constraints.max_x4,  epsilon, "Wrong initialization value for state_constraints.max_x4 of CV Trailer State");


   DOUBLES_EQUAL_TEXT(0.5F, cvt_state.one_link.ekf_state_errcov[0][0], epsilon, "Wrong initialization value for one_link.ekf_state_errcov[0][0] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(1.0F, cvt_state.one_link.ekf_state_errcov[1][1], epsilon, "Wrong initialization value for one_link.ekf_state_errcov[1][1] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(1.0F, cvt_state.one_link.ekf_state_errcov[2][2], epsilon, "Wrong initialization value for one_link.ekf_state_errcov[2][2] of CV Trailer State");

   DOUBLES_EQUAL_TEXT(init_data.one_link_model.n_updates, cvt_state.one_link.n_updates, epsilon, "Wrong initialization value for one_link.n_updates of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.one_link_model.full_vehicle_length, cvt_state.one_link.full_vehicle_length, epsilon, "Wrong initialization value for one_link.full_vehicle_length of CV Trailer State");

   DOUBLES_EQUAL_TEXT(0.05F, cvt_state.two_link.ekf_state_errcov[0][0], epsilon, "Wrong initialization value for two_link.ekf_state_errcov[0][0] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.2F, cvt_state.two_link.ekf_state_errcov[1][1], epsilon, "Wrong initialization value for two_link.ekf_state_errcov[1][1] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.5F, cvt_state.two_link.ekf_state_errcov[2][2], epsilon, "Wrong initialization value for two_link.ekf_state_errcov[2][2] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.05F, cvt_state.two_link.ekf_state_errcov[3][3], epsilon, "Wrong initialization value for two_link.ekf_state_errcov[3][3] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(1.0F, cvt_state.two_link.ekf_state_errcov[4][4], epsilon, "Wrong initialization value for two_link.ekf_state_errcov[4][4] of CV Trailer State");

   DOUBLES_EQUAL_TEXT(init_data.two_link_model.n_updates, cvt_state.two_link.n_updates, epsilon, "Wrong initialization value for two_link.n_updates of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.two_link_model.full_vehicle_length, cvt_state.two_link.full_vehicle_length, epsilon, "Wrong initialization value for two_link.full_vehicle_length of CV Trailer State");

   const float32_t one_link_x0 = init_data.one_link_model.link1_angle;
   const float32_t one_link_x1 = -(init_data.one_link_model.joint1_vcs_longpos - init_data.host_rear_axle_vcs_longpos);
   const float32_t one_link_x2 = init_data.one_link_model.link1_wheelbase;

   const float32_t two_link_x0 = init_data.two_link_model.link1_angle;
   const float32_t two_link_x1 = -(init_data.two_link_model.joint1_vcs_longpos - init_data.host_rear_axle_vcs_longpos);
   const float32_t two_link_x2 = init_data.two_link_model.link1_wheelbase;
   const float32_t two_link_x3 = init_data.two_link_model.link2_angle;
   const float32_t two_link_x4 = init_data.two_link_model.link2_wheelbase;

   DOUBLES_EQUAL_TEXT(fminf(cvt_state.state_constraints.max_x0, fmaxf(cvt_state.state_constraints.min_x0, one_link_x0)), cvt_state.one_link.ekf_state[0], epsilon, "Wrong initialization value for one_link.ekf_state[0] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(fminf(cvt_state.state_constraints.max_x1, fmaxf(cvt_state.state_constraints.min_x1, one_link_x1)), cvt_state.one_link.ekf_state[1], epsilon, "Wrong initialization value for one_link.ekf_state[1] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(fminf(cvt_state.state_constraints.max_x2, fmaxf(cvt_state.state_constraints.min_x2, one_link_x2)), cvt_state.one_link.ekf_state[2], epsilon, "Wrong initialization value for one_link.ekf_state[2] of CV Trailer State");

   DOUBLES_EQUAL_TEXT(fminf(cvt_state.state_constraints.max_x0, fmaxf(cvt_state.state_constraints.min_x0, two_link_x0)), cvt_state.two_link.ekf_state[0], epsilon, "Wrong initialization value for two_link.ekf_state[0] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(fminf(cvt_state.state_constraints.max_x1, fmaxf(cvt_state.state_constraints.min_x1, two_link_x1)), cvt_state.two_link.ekf_state[1], epsilon, "Wrong initialization value for two_link.ekf_state[1] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(fminf(cvt_state.state_constraints.max_x2, fmaxf(cvt_state.state_constraints.min_x2, two_link_x2)), cvt_state.two_link.ekf_state[2], epsilon, "Wrong initialization value for two_link.ekf_state[2] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(fminf(cvt_state.state_constraints.max_x3, fmaxf(cvt_state.state_constraints.min_x3, two_link_x3)), cvt_state.two_link.ekf_state[3], epsilon, "Wrong initialization value for two_link.ekf_state[3] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(fminf(cvt_state.state_constraints.max_x4, fmaxf(cvt_state.state_constraints.min_x4, two_link_x4)), cvt_state.two_link.ekf_state[4], epsilon, "Wrong initialization value for two_link.ekf_state[4] of CV Trailer State");


   DOUBLES_EQUAL_TEXT(0U, cvt_state.best_model_change_counter, epsilon, "Wrong initialization value for best_model_change_counter of CV Trailer State");
   DOUBLES_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, epsilon, "Wrong initialization value for best_trailer_model of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.host_length, cvt_state.host_length, epsilon, "Wrong initialization value for host_length of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.host_width, cvt_state.host_width, epsilon, "Wrong initialization value for host_width of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.radar_vcs_longpos, cvt_state.radar_vcs_longpos, epsilon, "Wrong initialization value for radar_vcs_longpos of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.radar_vcs_latpos, cvt_state.radar_vcs_latpos, epsilon, "Wrong initialization value for radar_vcs_latpos of CV Trailer State");

   DOUBLES_EQUAL_TEXT(TRAILER_FILTER_STATE_INIT, cvt_state.one_link.filter_state, epsilon, "Wrong initialization value for one_link.filter_state of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.0F, cvt_state.one_link.angle_rate, epsilon, "Wrong initialization value for one_link.angle_rate of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.0F, cvt_state.one_link.prev_trailer_angle, epsilon, "Wrong initialization value for one_link.prev_trailer_angle of CV Trailer State");
   DOUBLES_EQUAL_TEXT(k_init_trailer_length, cvt_state.one_link.trailer_length, epsilon, "Wrong initialization value for one_link.trailer_length of CV Trailer State");
   DOUBLES_EQUAL_TEXT(2.55F, cvt_state.one_link.trailer_width, epsilon, "Wrong initialization value for one_link.trailer_width of CV Trailer State");

   DOUBLES_EQUAL_TEXT(TRAILER_FILTER_STATE_INIT, cvt_state.two_link.filter_state, epsilon, "Wrong initialization value for two_link.filter_state of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.0F, cvt_state.two_link.angle_rate1, epsilon, "Wrong initialization value for two_link.angle_rate1 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.0F, cvt_state.two_link.angle_rate2, epsilon, "Wrong initialization value for two_link.angle_rate2 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.0F, cvt_state.two_link.prev_trailer1_angle, epsilon, "Wrong initialization value for two_link.prev_trailer1_angle of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.0F, cvt_state.two_link.prev_trailer2_angle, epsilon, "Wrong initialization value for two_link.prev_trailer2_angle of CV Trailer State");
   DOUBLES_EQUAL_TEXT(k_init_trailer_length, cvt_state.two_link.trailer1_length, epsilon, "Wrong initialization value for two_link.trailer1_length of CV Trailer State");
   DOUBLES_EQUAL_TEXT(k_init_trailer_length, cvt_state.two_link.trailer2_length, epsilon, "Wrong initialization value for two_link.trailer2_length of CV Trailer State");
   DOUBLES_EQUAL_TEXT(2.55F, cvt_state.two_link.trailer1_width, epsilon, "Wrong initialization value for two_link.trailer1_width of CV Trailer State");
   DOUBLES_EQUAL_TEXT(2.55F, cvt_state.two_link.trailer2_width, epsilon, "Wrong initialization value for two_link.trailer2_width of CV Trailer State");

   // check flag for successful initialization
   CHECK_TRUE_TEXT(cvt_state.f_init_complete, "cvt_state.f_init_complete Flag not computed correctly");


}


/** \purpose
 * Test relevant parameters of the CV Trailer has been initialized as intended.
 * Depending on the Flag "f_initiated_from_states", the initialization result should be different.
 * This test is for this flag to be negative, namely initializing from scratch.
 * The test involves the radar properties, host properties, cv trailer state constraints, 
 * EKF states and matrices, model selection flag and initialization_complete flag.
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_data_are_loaded_correctly_when_initialize_from_scratch)
{
   /** \precond
    * Use the Setup values as in default
    */

   F360_CVT_State_T cvt_state_copy;
   (void)std::memcpy(&cvt_state_copy, &cvt_state, sizeof(cvt_state_copy));

   // Reset the cvt_state
   (void)std::memcpy(&cvt_state, &cvt_state_copy, sizeof(cvt_state));
   const bool f_initiated_from_states = false;
   cvt_state.f_initiated_from_states = f_initiated_from_states;

   /** \action
    * Call CVT_Initialize to initialize the cvt_state
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * Expect the init_data to be parsed into cvt_state and init to be complete.
    * Check all internal states that are supposed to be arranged.
    */
   // check host properties
   DOUBLES_EQUAL_TEXT(init_data.radar_id, cvt_state.radar_id, epsilon, "Wrong initialization value for radar_id of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.radar_vcs_longpos, cvt_state.radar_vcs_longpos, epsilon, "Wrong initialization value for radar_vcs_longpos of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.radar_vcs_latpos, cvt_state.radar_vcs_latpos, epsilon, "Wrong initialization value for radar_vcs_latpos of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.host_length, cvt_state.host_length, epsilon, "Wrong initialization value for host_length of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.host_width, cvt_state.host_width, epsilon, "Wrong initialization value for host_width of CV Trailer State");

   // check constraints
   DOUBLES_EQUAL_TEXT(-0.5F * F360_PI, cvt_state.state_constraints.min_x0,  epsilon, "Wrong initialization value for state_constraints.min_x0 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.5F * F360_PI, cvt_state.state_constraints.max_x0,  epsilon, "Wrong initialization value for state_constraints.max_x0 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(-(init_data.joint1_vcs_longpos_max - init_data.host_rear_axle_vcs_longpos), cvt_state.state_constraints.min_x1,  epsilon, "Wrong initialization value for state_constraints.min_x1 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(-(init_data.joint1_vcs_longpos_min - init_data.host_rear_axle_vcs_longpos), cvt_state.state_constraints.max_x1,  epsilon, "Wrong initialization value for state_constraints.max_x1 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.link1_wheelbase_min, cvt_state.state_constraints.min_x2,  epsilon, "Wrong initialization value for state_constraints.min_x2 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.link1_wheelbase_max, cvt_state.state_constraints.max_x2,  epsilon, "Wrong initialization value for state_constraints.max_x2 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(-0.75F * F360_PI, cvt_state.state_constraints.min_x3,  epsilon, "Wrong initialization value for state_constraints.min_x3 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.75F * F360_PI, cvt_state.state_constraints.max_x3,  epsilon, "Wrong initialization value for state_constraints.max_x3 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.link2_wheelbase_min, cvt_state.state_constraints.min_x4,  epsilon, "Wrong initialization value for state_constraints.min_x4 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.link2_wheelbase_max, cvt_state.state_constraints.max_x4,  epsilon, "Wrong initialization value for state_constraints.max_x4 of CV Trailer State");


   DOUBLES_EQUAL_TEXT(0.5F, cvt_state.one_link.ekf_state_errcov[0][0], epsilon, "Wrong initialization value for one_link.ekf_state_errcov[0][0] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(1.0F, cvt_state.one_link.ekf_state_errcov[1][1], epsilon, "Wrong initialization value for one_link.ekf_state_errcov[1][1] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(1.0F, cvt_state.one_link.ekf_state_errcov[2][2], epsilon, "Wrong initialization value for one_link.ekf_state_errcov[2][2] of CV Trailer State");

   DOUBLES_EQUAL_TEXT(init_data.one_link_model.n_updates, cvt_state.one_link.n_updates, epsilon, "Wrong initialization value for one_link.n_updates of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.one_link_model.full_vehicle_length, cvt_state.one_link.full_vehicle_length, epsilon, "Wrong initialization value for one_link.full_vehicle_length of CV Trailer State");

   DOUBLES_EQUAL_TEXT(0.05F, cvt_state.two_link.ekf_state_errcov[0][0], epsilon, "Wrong initialization value for two_link.ekf_state_errcov[0][0] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.2F, cvt_state.two_link.ekf_state_errcov[1][1], epsilon, "Wrong initialization value for two_link.ekf_state_errcov[1][1] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.5F, cvt_state.two_link.ekf_state_errcov[2][2], epsilon, "Wrong initialization value for two_link.ekf_state_errcov[2][2] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.05F, cvt_state.two_link.ekf_state_errcov[3][3], epsilon, "Wrong initialization value for two_link.ekf_state_errcov[3][3] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(1.0F, cvt_state.two_link.ekf_state_errcov[4][4], epsilon, "Wrong initialization value for two_link.ekf_state_errcov[4][4] of CV Trailer State");

   DOUBLES_EQUAL_TEXT(init_data.two_link_model.n_updates, cvt_state.two_link.n_updates, epsilon, "Wrong initialization value for two_link.n_updates of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.two_link_model.full_vehicle_length, cvt_state.two_link.full_vehicle_length, epsilon, "Wrong initialization value for two_link.full_vehicle_length of CV Trailer State");

   const float32_t one_link_x0 = init_data.one_link_model.link1_angle;
   const float32_t one_link_x1 = -(init_data.one_link_model.joint1_vcs_longpos - init_data.host_rear_axle_vcs_longpos);
   const float32_t one_link_x2 = init_data.one_link_model.link1_wheelbase;

   const float32_t two_link_x0 = init_data.two_link_model.link1_angle;
   const float32_t two_link_x1 = -(init_data.two_link_model.joint1_vcs_longpos - init_data.host_rear_axle_vcs_longpos);
   const float32_t two_link_x2 = init_data.two_link_model.link1_wheelbase;
   const float32_t two_link_x3 = init_data.two_link_model.link2_angle;
   const float32_t two_link_x4 = init_data.two_link_model.link2_wheelbase;

   DOUBLES_EQUAL_TEXT(fminf(cvt_state.state_constraints.max_x0, fmaxf(cvt_state.state_constraints.min_x0, one_link_x0)), cvt_state.one_link.ekf_state[0], epsilon, "Wrong initialization value for one_link.ekf_state[0] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(fminf(cvt_state.state_constraints.max_x1, fmaxf(cvt_state.state_constraints.min_x1, one_link_x1)), cvt_state.one_link.ekf_state[1], epsilon, "Wrong initialization value for one_link.ekf_state[1] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(fminf(cvt_state.state_constraints.max_x2, fmaxf(cvt_state.state_constraints.min_x2, one_link_x2)), cvt_state.one_link.ekf_state[2], epsilon, "Wrong initialization value for one_link.ekf_state[2] of CV Trailer State");

   DOUBLES_EQUAL_TEXT(fminf(cvt_state.state_constraints.max_x0, fmaxf(cvt_state.state_constraints.min_x0, two_link_x0)), cvt_state.two_link.ekf_state[0], epsilon, "Wrong initialization value for two_link.ekf_state[0] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(fminf(cvt_state.state_constraints.max_x1, fmaxf(cvt_state.state_constraints.min_x1, two_link_x1)), cvt_state.two_link.ekf_state[1], epsilon, "Wrong initialization value for two_link.ekf_state[1] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(fminf(cvt_state.state_constraints.max_x2, fmaxf(cvt_state.state_constraints.min_x2, two_link_x2)), cvt_state.two_link.ekf_state[2], epsilon, "Wrong initialization value for two_link.ekf_state[2] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(fminf(cvt_state.state_constraints.max_x3, fmaxf(cvt_state.state_constraints.min_x3, two_link_x3)), cvt_state.two_link.ekf_state[3], epsilon, "Wrong initialization value for two_link.ekf_state[3] of CV Trailer State");
   DOUBLES_EQUAL_TEXT(fminf(cvt_state.state_constraints.max_x4, fmaxf(cvt_state.state_constraints.min_x4, two_link_x4)), cvt_state.two_link.ekf_state[4], epsilon, "Wrong initialization value for two_link.ekf_state[4] of CV Trailer State");


   DOUBLES_EQUAL_TEXT(0U, cvt_state.best_model_change_counter, epsilon, "Wrong initialization value for best_model_change_counter of CV Trailer State");
   DOUBLES_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, epsilon, "Wrong initialization value for best_trailer_model of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.host_length, cvt_state.host_length, epsilon, "Wrong initialization value for host_length of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.host_width, cvt_state.host_width, epsilon, "Wrong initialization value for host_width of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.radar_vcs_longpos, cvt_state.radar_vcs_longpos, epsilon, "Wrong initialization value for radar_vcs_longpos of CV Trailer State");
   DOUBLES_EQUAL_TEXT(init_data.radar_vcs_latpos, cvt_state.radar_vcs_latpos, epsilon, "Wrong initialization value for radar_vcs_latpos of CV Trailer State");

   DOUBLES_EQUAL_TEXT(TRAILER_FILTER_STATE_INIT, cvt_state.one_link.filter_state, epsilon, "Wrong initialization value for one_link.filter_state of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.0F, cvt_state.one_link.angle_rate, epsilon, "Wrong initialization value for one_link.angle_rate of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.0F, cvt_state.one_link.prev_trailer_angle, epsilon, "Wrong initialization value for one_link.prev_trailer_angle of CV Trailer State");
   DOUBLES_EQUAL_TEXT(k_init_trailer_length, cvt_state.one_link.trailer_length, epsilon, "Wrong initialization value for one_link.trailer_length of CV Trailer State");
   DOUBLES_EQUAL_TEXT(2.55F, cvt_state.one_link.trailer_width, epsilon, "Wrong initialization value for one_link.trailer_width of CV Trailer State");

   DOUBLES_EQUAL_TEXT(TRAILER_FILTER_STATE_INIT, cvt_state.two_link.filter_state, epsilon, "Wrong initialization value for two_link.filter_state of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.0F, cvt_state.two_link.angle_rate1, epsilon, "Wrong initialization value for two_link.angle_rate1 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.0F, cvt_state.two_link.angle_rate2, epsilon, "Wrong initialization value for two_link.angle_rate2 of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.0F, cvt_state.two_link.prev_trailer1_angle, epsilon, "Wrong initialization value for two_link.prev_trailer1_angle of CV Trailer State");
   DOUBLES_EQUAL_TEXT(0.0F, cvt_state.two_link.prev_trailer2_angle, epsilon, "Wrong initialization value for two_link.prev_trailer2_angle of CV Trailer State");
   DOUBLES_EQUAL_TEXT(k_init_trailer_length, cvt_state.two_link.trailer1_length, epsilon, "Wrong initialization value for two_link.trailer1_length of CV Trailer State");
   DOUBLES_EQUAL_TEXT(k_init_trailer_length, cvt_state.two_link.trailer2_length, epsilon, "Wrong initialization value for two_link.trailer2_length of CV Trailer State");
   DOUBLES_EQUAL_TEXT(2.55F, cvt_state.two_link.trailer1_width, epsilon, "Wrong initialization value for two_link.trailer1_width of CV Trailer State");
   DOUBLES_EQUAL_TEXT(2.55F, cvt_state.two_link.trailer2_width, epsilon, "Wrong initialization value for two_link.trailer2_width of CV Trailer State");

   // check flag for successful initialization
   CHECK_TRUE_TEXT(cvt_state.f_init_complete, "cvt_state.f_init_complete Flag not computed correctly");


}

/** \purpose
 * With different initialization setup and false f_initiated_from_states, test the flag f_init_complete is set correctly.
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_complete_flag_set_correctly_when_init_from_scratch)
{
   /** \precond
    * define several tests with different variables satisfying / violating the condition for a successful initialization
    */
   // Define the struct to save all parameters for one test
   struct cv_trailer_initialization_test_T
   {
      float32_t one_link_full_vehicle_length;   // Angle of the trailer, following Right hand rule, positive means trailer at rear left
      float32_t two_link_full_vehicle_length;    // Side slip angle of the host, Following Right Hand rule, positive means host turning right
      float32_t host_width;
      float32_t host_length;
      float32_t radar_vcs_longpos;
      float32_t radar_vcs_latpos;
      bool f_init_complete_expected;
      std::string test_description;  // Descriptor of the test
   };

   // Define different test cases
   std::vector<cv_trailer_initialization_test_T> all_test_cases = {
      //one_link_full_vehicle_length,  two_link_full_vehicle_length,  host_width,  host_length,  radar_vcs_longpos,  radar_vcs_latpos,  f_init_complete_expected,  test_description
       {7.0F,                          12.0F,                         2.0F,        5.0F,         -1.0F,              1.0F,              true,                       "Test1: all conditions are satisfied, expect init complete"},
       {60.1F,                         12.0F,                         2.0F,        5.0F,         -1.0F,              1.0F,              false,                      "Test2: one_link_full_vehicle_length not satisfied, expect initialization incomplete"},
       {7.0F,                          60.1F,                         2.0F,        5.0F,         -1.0F,              1.0F,              false,                      "Test3: two_link_full_vehicle_length not satisfied, expect initialization incomplete"},
       {-1.0F,                         12.0F,                         2.0F,        5.0F,         -1.0F,              1.0F,              false,                      "Test4: one_link_full_vehicle_length not satisfied, expect initialization incomplete"},
       {7.0F,                         -1.0F,                          2.0F,        5.0F,         -1.0F,              1.0F,              false,                      "Test5: two_link_full_vehicle_length not satisfied, expect initialization incomplete"},
       {7.0F,                          12.0F,                        -1.0F,        5.0F,         -1.0F,              1.0F,              false,                      "Test6: host_width not satisfied, expect initialization incomplete"},
       {7.0F,                          12.0F,                         2.0F,       -1.0F,         -1.0F,              1.0F,              false,                      "Test7: host_length not satisfied, expect initialization incomplete"},
       {7.0F,                          12.0F,                         2.0F,        5.0F,          1.0F,              1.0F,              false,                      "Test8: radar_vcs_longpos not satisfied, expect initialization incomplete"},
       {7.0F,                          12.0F,                         2.0F,        5.0F,         -6.0F,              1.0F,              false,                      "Test9: radar_vcs_longpos not satisfied, expect initialization incomplete"},
       {7.0F,                          12.0F,                         2.0F,        5.0F,         -1.0F,              2.0F,              false,                      "Test10:radar_vcs_latpos not satisfied, expect initialization incomplete"},
   };

   // Loop over all test cases
   for (const cv_trailer_initialization_test_T &test_case_i : all_test_cases)
   {
      init_data.one_link_model.full_vehicle_length = test_case_i.one_link_full_vehicle_length;
      init_data.two_link_model.full_vehicle_length = test_case_i.two_link_full_vehicle_length;
      init_data.host_width = test_case_i.host_width;
      init_data.host_length = test_case_i.host_length;
      init_data.radar_vcs_longpos = test_case_i.radar_vcs_longpos;
      init_data.radar_vcs_latpos = test_case_i.radar_vcs_latpos;
      const bool f_init_complete_expected = test_case_i.f_init_complete_expected;

      /** \action
       * initialize the cv trailer
       */
      CVT_Initialize(calib, init_data, cvt_state);

      /** \result
       * check the flag for successful initialization
       */
      if (f_init_complete_expected)
      {
         CHECK_TRUE_TEXT(cvt_state.f_init_complete, test_case_i.test_description.c_str());
      }
      else
      {
         CHECK_FALSE_TEXT(cvt_state.f_init_complete, test_case_i.test_description.c_str());
      }

   }
}

/** \purpose
 * Test that if the covariance of 2-link model in cvt_state violates the bound,
 * the init_from_state would fail, and init from scratch will run,
 * to avoid weird EKF behavior due to bad NVM data.
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_would_fail_if_2link_errcov_is_not_in_bound)
{

   /** \precond
   * switch on the flag to initialize the states from the cvt_state read in NVM
   */
   float32_t tmp_cpy_of_errcov[5][5];
   (void)memcpy(&tmp_cpy_of_errcov[0][0], &cvt_state.two_link.ekf_state_errcov[0][0], sizeof(tmp_cpy_of_errcov));

   // Loop over different cases of diagonal terms of the covariance matrix
   for (int32_t i = 0; i < 5; i++)
   {
      // recover the state errcov and the f_initiated_from_states flag at the beginning of each loop
      (void)memcpy(&cvt_state.two_link.ekf_state_errcov[0][0], &tmp_cpy_of_errcov[0][0], sizeof(cvt_state.two_link.ekf_state_errcov));
      cvt_state.f_initiated_from_states = true;
      cvt_state.one_link.joint_vcs_longpos = -1.1F;
      cvt_state.one_link.joint_dist_to_wheels = 2.1F;
      cvt_state.two_link.joint1_vcs_longpos = -1.1F;
      cvt_state.two_link.joint2_vcs_longpos = -1.1F;
      cvt_state.two_link.joint1_dist_to_wheels = 2.1F;
      cvt_state.two_link.joint2_dist_to_wheels = 2.1F;
      cvt_state.two_link.ekf_state_errcov[i][i] = 0.0F;

      /** \action
       * initialize the cv trailer with one of the 2-link error covariance out of bound
       */

      CVT_Initialize(calib, init_data, cvt_state);

      /** \result
       * check that init from state failed: the states are still initialized with init from scratch
       */
      CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
      CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "n_update of 1-link should be 1U since init from state should fail!");
      CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "n_update of 2-link should be 1U since init from state should fail!");

   }
}

/** \brief
 * Test that initialization succeeds when all external state init conditions are met
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_succeeds_when_all_conditions_met)
{
   /** \precond
    * All conditions for f_external_state_init are satisfied
    */
   cvt_state.f_initiated_from_states = true;

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from the provided cvt_state (n_updates preserved)
    */
   CHECK_EQUAL_TEXT(100U, cvt_state.one_link.n_updates, "One link n_updates should be preserved");
   CHECK_EQUAL_TEXT(200U, cvt_state.two_link.n_updates, "Two link n_updates should be preserved");
}

/** \brief
 * Test that initialization fails when f_initiated_from_states is false
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_f_initiated_from_states_false)
{
   /** \precond
    * f_initiated_from_states is false, all other conditions true
    */
   cvt_state.f_initiated_from_states = false;  // This violates the condition for init from states

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch (n_updates reset to 1)
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when one_link.n_updates < 0
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_one_link_n_updates_negative)
{
   /** \precond
    * one_link.n_updates < 0, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.one_link.n_updates = -1;  // This violates the condition for init from states

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}


/** \brief
 * Test that initialization fails when one_link.joint_dist_to_wheels <= 2.0F
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_one_link_joint_dist_to_wheels_not_greater_than_two)
{
   /** \precond
    * one_link.joint_dist_to_wheels <= 2.0F, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.one_link.full_vehicle_length = 5.0F;  // This violates the condition for init from states

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when one_link.ekf_state_errcov[0][0] <= 0.0F
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_one_link_errcov_not_positive)
{
   /** \precond
    * one_link.ekf_state_errcov[0][0] <= 0.0F, all other conditions true
    */
   float32_t tmp_cpy_of_errcov[3][3];
   (void)memcpy(&tmp_cpy_of_errcov[0][0], &cvt_state.one_link.ekf_state_errcov[0][0], sizeof(tmp_cpy_of_errcov));
   for (int32_t i = 0; i < 3; i++)
   {
      // recover the state errcov and init_from_state flag at the beginning of each loop
      cvt_state.f_initiated_from_states = true;
      cvt_state.one_link.joint_vcs_longpos = -1.1F;
      cvt_state.one_link.joint_dist_to_wheels = 2.1F;
      (void)memcpy(&cvt_state.one_link.ekf_state_errcov[0][0], &tmp_cpy_of_errcov[0][0], sizeof(cvt_state.one_link.ekf_state_errcov));
      cvt_state.one_link.ekf_state_errcov[i][i] = 0.0F;  // This violates the condition for init from states, should > 0

      /** \action
       * Call CVT_Initialize
       */
      CVT_Initialize(calib, init_data, cvt_state);

      /** \result
       * State should be initialized from scratch
       */
      CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
      CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
   }
}

/** \brief
 * Test that initialization fails when two_link.n_updates < 0
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_two_link_n_updates_negative)
{
   /** \precond
    * two_link.n_updates < 0, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.two_link.n_updates = -1;  // This violates the condition for init from states

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when one_link.full_vehicle_length <= init_data.host_length
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_one_link_full_vehicle_length_not_greater_than_host_length)
{
   /** \precond
    * one_link.full_vehicle_length <= init_data.host_length, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.one_link.full_vehicle_length = 5.0F;  // = host_length, violates the condition

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when one_link.full_vehicle_length > k_max_valid_vehicle_length
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_one_link_full_vehicle_length_greater_than_max_valid)
{
   /** \precond
    * one_link.full_vehicle_length > k_max_valid_vehicle_length, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.one_link.full_vehicle_length = 60.1F;  // > 60.0F, violates the condition

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when two_link.full_vehicle_length <= init_data.host_length
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_two_link_full_vehicle_length_not_greater_than_host_length)
{
   /** \precond
    * two_link.full_vehicle_length <= init_data.host_length, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.two_link.full_vehicle_length = 5.0F;  // = host_length, violates the condition

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when two_link.full_vehicle_length > k_max_valid_vehicle_length
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_two_link_full_vehicle_length_greater_than_max_valid)
{
   /** \precond
    * two_link.full_vehicle_length > k_max_valid_vehicle_length, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.two_link.full_vehicle_length = 60.1F;  // > 60.0F, violates the condition

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when one_link.ekf_state[0] is out of bounds (min_x0)
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_one_link_ekf_state_x0_below_min)
{
   /** \precond
    * one_link.ekf_state[0] < min_x0, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.one_link.ekf_state[0] = -0.5F * F360_PI - 0.1F;  // Below min_x0

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when one_link.ekf_state[0] is out of bounds (max_x0)
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_one_link_ekf_state_x0_above_max)
{
   /** \precond
    * one_link.ekf_state[0] > max_x0, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.one_link.ekf_state[0] = 0.5F * F360_PI + 0.1F;  // Above max_x0

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when one_link.ekf_state[1] is out of bounds (min_x1)
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_one_link_ekf_state_x1_below_min)
{
   /** \precond
    * one_link.ekf_state[1] < min_x1, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.one_link.ekf_state[1] = -(init_data.joint1_vcs_longpos_max - init_data.host_rear_axle_vcs_longpos) - 0.1F;  // Below min_x1

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when one_link.ekf_state[1] is out of bounds (max_x1)
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_one_link_ekf_state_x1_above_max)
{
   /** \precond
    * one_link.ekf_state[1] > max_x1, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.one_link.ekf_state[1] = -(init_data.joint1_vcs_longpos_min - init_data.host_rear_axle_vcs_longpos) + 0.1F;  // Above max_x1

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when one_link.ekf_state[2] is out of bounds (min_x2)
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_one_link_ekf_state_x2_below_min)
{
   /** \precond
    * one_link.ekf_state[2] < min_x2, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.one_link.ekf_state[2] = init_data.link1_wheelbase_min - 0.1F;  // Below min_x2

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when one_link.ekf_state[2] is out of bounds (max_x2)
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_one_link_ekf_state_x2_above_max)
{
   /** \precond
    * one_link.ekf_state[2] > max_x2, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.one_link.ekf_state[2] = init_data.link1_wheelbase_max + 0.1F;  // Above max_x2

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when two_link.ekf_state[0] is out of bounds (min_x0)
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_two_link_ekf_state_x0_below_min)
{
   /** \precond
    * two_link.ekf_state[0] < min_x0, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.two_link.ekf_state[0] = -0.5F * F360_PI - 0.1F;  // Below min_x0

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when two_link.ekf_state[0] is out of bounds (max_x0)
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_two_link_ekf_state_x0_above_max)
{
   /** \precond
    * two_link.ekf_state[0] > max_x0, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.two_link.ekf_state[0] = 0.5F * F360_PI + 0.1F;  // Above max_x0

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when two_link.ekf_state[1] is out of bounds (min_x1)
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_two_link_ekf_state_x1_below_min)
{
   /** \precond
    * two_link.ekf_state[1] < min_x1, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.two_link.ekf_state[1] = -(init_data.joint1_vcs_longpos_max - init_data.host_rear_axle_vcs_longpos) - 0.1F;  // Below min_x1

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when two_link.ekf_state[1] is out of bounds (max_x1)
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_two_link_ekf_state_x1_above_max)
{
   /** \precond
    * two_link.ekf_state[1] > max_x1, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.two_link.ekf_state[1] = -(init_data.joint1_vcs_longpos_min - init_data.host_rear_axle_vcs_longpos) + 0.1F;  // Above max_x1

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when two_link.ekf_state[2] is out of bounds (min_x2)
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_two_link_ekf_state_x2_below_min)
{
   /** \precond
    * two_link.ekf_state[2] < min_x2, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.two_link.ekf_state[2] = init_data.link1_wheelbase_min - 0.1F;  // Below min_x2

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when two_link.ekf_state[2] is out of bounds (max_x2)
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_two_link_ekf_state_x2_above_max)
{
   /** \precond
    * two_link.ekf_state[2] > max_x2, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.two_link.ekf_state[2] = init_data.link1_wheelbase_max + 0.1F;  // Above max_x2

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when two_link.ekf_state[3] is out of bounds (min_x3)
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_two_link_ekf_state_x3_below_min)
{
   /** \precond
    * two_link.ekf_state[3] < min_x3, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.two_link.ekf_state[3] = -0.75F * F360_PI - 0.1F;  // Below min_x3

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when two_link.ekf_state[3] is out of bounds (max_x3)
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_two_link_ekf_state_x3_above_max)
{
   /** \precond
    * two_link.ekf_state[3] > max_x3, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.two_link.ekf_state[3] = 0.75F * F360_PI + 0.1F;  // Above max_x3

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when two_link.ekf_state[4] is out of bounds (min_x4)
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_two_link_ekf_state_x4_below_min)
{
   /** \precond
    * two_link.ekf_state[4] < min_x4, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.two_link.ekf_state[4] = init_data.link2_wheelbase_min - 0.1F;  // Below min_x4

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when two_link.ekf_state[4] is out of bounds (max_x4)
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_two_link_ekf_state_x4_above_max)
{
   /** \precond
    * two_link.ekf_state[4] > max_x4, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.two_link.ekf_state[4] = init_data.link2_wheelbase_max + 0.1F;  // Above max_x4

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when two_link.ekf_state_errcov[0][0] is not positive
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_two_link_errcov_00_not_positive)
{
   /** \precond
    * two_link.ekf_state_errcov[0][0] <= 0.0F, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.two_link.ekf_state_errcov[0][0] = 0.0F;  // Not positive

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when two_link.ekf_state_errcov[1][1] is not positive
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_two_link_errcov_11_not_positive)
{
   /** \precond
    * two_link.ekf_state_errcov[1][1] <= 0.0F, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.two_link.ekf_state_errcov[1][1] = 0.0F;  // Not positive

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when two_link.ekf_state_errcov[2][2] is not positive
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_two_link_errcov_22_not_positive)
{
   /** \precond
    * two_link.ekf_state_errcov[2][2] <= 0.0F, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.two_link.ekf_state_errcov[2][2] = 0.0F;  // Not positive

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when two_link.ekf_state_errcov[3][3] is not positive
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_two_link_errcov_33_not_positive)
{
   /** \precond
    * two_link.ekf_state_errcov[3][3] <= 0.0F, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.two_link.ekf_state_errcov[3][3] = 0.0F;  // Not positive

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \brief
 * Test that initialization fails when two_link.ekf_state_errcov[4][4] is not positive
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, init_from_state_fails_when_two_link_errcov_44_not_positive)
{
   /** \precond
    * two_link.ekf_state_errcov[4][4] <= 0.0F, all other conditions true
    */
   cvt_state.f_initiated_from_states = true;
   cvt_state.two_link.ekf_state_errcov[4][4] = 0.0F;  // Not positive

   /** \action
    * Call CVT_Initialize
    */
   CVT_Initialize(calib, init_data, cvt_state);

   /** \result
    * State should be initialized from scratch
    */
   CHECK_EQUAL_TEXT(TRAILER_MODEL_ONE_LINK, cvt_state.best_trailer_model, "One link model should be selected since init from state should fail!");
   CHECK_EQUAL_TEXT(1U, cvt_state.one_link.n_updates, "One link n_updates should be initialized as 1, because init from state failed, and from scratch used instead");
   CHECK_EQUAL_TEXT(2U, cvt_state.two_link.n_updates, "Two link n_updates should be initialized as 2, because init from state failed, and from scratch used instead");
}

/** \purpose
 * In case of states Not initialized from states, Test the update of one link EKF and two link EKF. Given valid measurement, they should be updated, and vice versa
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, cvt_execute_update_states_correctly_init_from_scratch)
{

   /** \precond
    * define several tests with different variables satisfying / violating the condition for an update in EKFs
    */

   // Define the struct to save all parameters for one test
   struct cv_trailer_state_update_test_T
   {
      bool f_primary_measurement_valid;
      int32_t one_link_n_updates;
      int32_t two_link_n_updates;
      bool expect_one_link_updated;
      bool expect_two_link_updated;
      std::string test_description;  // Descriptor of the test
   };

   // Define different test cases
   std::vector<cv_trailer_state_update_test_T> all_test_cases = {
      //f_primary_measurement_valid,  one_link_n_updates,  two_link_n_updates,  expect_one_link_updated,  expect_two_link_updated,  test_description
       {true,                         50,                  379,                 true,                     true,                     "Test1: Both ekf models should be updated, as their angle gates are slightly larger than 25 degrees"},
       {true,                         51,                  379,                 false,                    true,                     "Test2: one_link should not be updated, two_link should be updated, as their angle gates are slightly smaller and larger than 25 degrees, respectively"},
       {true,                         50,                  381,                 true,                     false,                    "Test3: one_link should be updated, two_link should not be updated, as their angle gates are slightly larger and smaller than 25 degrees, respectively"},
       {true,                         51,                  381,                 false,                    false,                    "Test4: None ekf models should not be updated, as none of their angle gates are larger than 25 degrees"},
       {false,                        50,                  379,                 false,                    false,                    "Test5: None ekf models should not be updated, as measurement is not marked as valid"},
       {false,                        51,                  379,                 false,                    false,                    "Test6: None ekf models should not be updated, as measurement is not marked as valid"},
       {false,                        50,                  381,                 false,                    false,                    "Test7: None ekf models should not be updated, as measurement is not marked as valid"},
       {false,                        51,                  381,                 false,                    false,                    "Test8: None ekf models should not be updated, as measurement is not marked as valid"},
       {true,                       int32_t(1E6), int32_t(1E6),                 false,                    false,                    "Test9: None ekf models should not be updated, the counter should not grow in any models"},
       {true,                       int32_t(1E6), int32_t(1E6),                 false,                    false,                    "Test10: None ekf models should not be updated, the counter should not grow in any models"}
      };

   // Loop over all test cases
   for (const cv_trailer_state_update_test_T &test_case_i : all_test_cases)
   {
      init_data.one_link_model.n_updates = test_case_i.one_link_n_updates;
      init_data.two_link_model.n_updates = test_case_i.two_link_n_updates;
      CVT_Initialize(calib, init_data, cvt_state);

      cvt_state.radar_vcs_latpos = -1.0F;
      cvt_state.one_link.ekf_state[0] = F360_DEG2RAD(15.2F);
      cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(15.2F);
      cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(15.2F);
      const float32_t desired_trailer_angle_vcs = F360_DEG2RAD(15.2F+25.0F);

      const float32_t desired_trailer_intersect_vcs_long = -5.0F;
      if (test_case_i.f_primary_measurement_valid)
      {
         const float32_t k_trailer_orth_offset = 0.35F * 2.55F; // 35% of trailer width
         cvt_input.host_speed = 3.0F;
         const float32_t intersect_offset = fabsf((k_trailer_orth_offset) / F360_Sinf(desired_trailer_intersect_vcs_long));
         cvt_input.n_detections = 7;
         for (int32_t i = 0; i < 7; i++)
         {
            // Create 6 valid dets that form a line
            cvt_input.detections[i].range_rate = 0.3F;
            cvt_input.detections[i].vcs_longpos = - static_cast<float32_t>(i + 2) + desired_trailer_intersect_vcs_long + intersect_offset;
            cvt_input.detections[i].vcs_latpos = -static_cast<float32_t>(i + 2) * F360_Tanf(desired_trailer_angle_vcs);
         }
      }
      else
      {
         cvt_input.n_detections = 0;
      }

      const bool f_expect_one_link_updated = test_case_i.expect_one_link_updated;
      const bool f_expect_two_link_updated = test_case_i.expect_two_link_updated;

      /** \action
       * initialize the cv trailer
       */
      CVT_Execute(calib, cvt_input, cvt_state);

      /** \result
       * check the counter for EKF updates
       */
      if (f_expect_one_link_updated)
      {
         CHECK_EQUAL_TEXT(test_case_i.one_link_n_updates+1, cvt_state.one_link.n_updates, test_case_i.test_description.c_str());
      }
      else
      {
         CHECK_EQUAL_TEXT(test_case_i.one_link_n_updates, cvt_state.one_link.n_updates, test_case_i.test_description.c_str());
      }

      if (f_expect_two_link_updated)
      {
         CHECK_EQUAL_TEXT(test_case_i.two_link_n_updates+1, cvt_state.two_link.n_updates, test_case_i.test_description.c_str());
      }
      else
      {
         CHECK_EQUAL_TEXT(test_case_i.two_link_n_updates, cvt_state.two_link.n_updates, test_case_i.test_description.c_str());
      }
   }
}

/** \purpose
 * In case of states initialized from states, test the EKF are updated as expected
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, cvt_execute_update_states_correctly_init_from_states)
{

   /** \precond
    * define several tests with different variables satisfying / violating the condition for EKF updates
    */

   // Define the struct to save all parameters for one test
   struct cv_trailer_state_update_test_T
   {
      bool f_primary_measurement_valid;
      int32_t one_link_n_updates;
      int32_t two_link_n_updates;
      bool expect_one_link_updated;
      bool expect_two_link_updated;
      std::string test_description;  // Descriptor of the test
   };

   // Define different test cases
   std::vector<cv_trailer_state_update_test_T> all_test_cases = {
      //f_primary_measurement_valid,  one_link_n_updates,  two_link_n_updates,  expect_one_link_updated,  expect_two_link_updated,  test_description
       {true,                         50,                  379,                 true,                     true,                     "Test1: Both ekf models should be updated, as their angle gates are slightly larger than 25 degrees"},
       {true,                         51,                  379,                 false,                    true,                     "Test2: one_link should not be updated, two_link should be updated, as their angle gates are slightly smaller and larger than 25 degrees, respectively"},
       {true,                         50,                  381,                 true,                     false,                    "Test3: one_link should be updated, two_link should not be updated, as their angle gates are slightly larger and smaller than 25 degrees, respectively"},
       {true,                         51,                  381,                 false,                    false,                    "Test4: None ekf models should not be updated, as none of their angle gates are larger than 25 degrees"},
       {false,                        50,                  379,                 false,                    false,                    "Test5: None ekf models should not be updated, as measurement is not marked as valid"},
       {false,                        51,                  379,                 false,                    false,                    "Test6: None ekf models should not be updated, as measurement is not marked as valid"},
       {false,                        50,                  381,                 false,                    false,                    "Test7: None ekf models should not be updated, as measurement is not marked as valid"},
       {false,                        51,                  381,                 false,                    false,                    "Test8: None ekf models should not be updated, as measurement is not marked as valid"},
       {true,                       int32_t(1E6), int32_t(1E6),                 false,                    false,                    "Test9: None ekf models should not be updated, the counter should not grow in any models"},
       {true,                       int32_t(1E6), int32_t(1E6),                 false,                    false,                    "Test10: None ekf models should not be updated, the counter should not grow in any models"}
      };

   // Loop over all test cases
   for (const cv_trailer_state_update_test_T &test_case_i : all_test_cases)
   {
      cvt_state.f_initiated_from_states = true;
      CVT_Initialize(calib, init_data, cvt_state);

      cvt_state.radar_vcs_latpos = -1.0F;
      cvt_state.one_link.ekf_state[0] = F360_DEG2RAD(15.2F);
      cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(15.2F);
      cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(15.2F);
      const float32_t desired_trailer_angle_vcs = F360_DEG2RAD(15.2F+25.0F);

      const float32_t desired_trailer_intersect_vcs_long = -5.0F;
      if (test_case_i.f_primary_measurement_valid)
      {
         const float32_t k_trailer_orth_offset = 0.35F * 2.55F; // 35% of trailer width
         cvt_input.host_speed = 3.0F;
         const float32_t intersect_offset = fabsf((k_trailer_orth_offset) / F360_Sinf(desired_trailer_intersect_vcs_long));
         cvt_input.n_detections = 7;
         for (int32_t i = 0; i < 7; i++)
         {
            // Create 6 valid dets that form a line
            cvt_input.detections[i].range_rate = 0.3F;
            cvt_input.detections[i].vcs_longpos = - static_cast<float32_t>(i + 2) + desired_trailer_intersect_vcs_long + intersect_offset;
            cvt_input.detections[i].vcs_latpos = -static_cast<float32_t>(i + 2) * F360_Tanf(desired_trailer_angle_vcs);
         }
      }
      else
      {
         cvt_input.n_detections = 0;
      }
      
      cvt_state.one_link.n_updates = test_case_i.one_link_n_updates;
      cvt_state.two_link.n_updates = test_case_i.two_link_n_updates;

      const bool f_expect_one_link_updated = test_case_i.expect_one_link_updated;
      const bool f_expect_two_link_updated = test_case_i.expect_two_link_updated;

      /** \action
       * initialize the cv trailer
       */
      CVT_Execute(calib, cvt_input, cvt_state);

      /** \result
       * check the counter for EKF updates
       */
      if (f_expect_one_link_updated)
      {
         CHECK_EQUAL_TEXT(test_case_i.one_link_n_updates+1, cvt_state.one_link.n_updates, test_case_i.test_description.c_str());
      }
      else
      {
         CHECK_EQUAL_TEXT(test_case_i.one_link_n_updates, cvt_state.one_link.n_updates, test_case_i.test_description.c_str());
      }

      if (f_expect_two_link_updated)
      {
         CHECK_EQUAL_TEXT(test_case_i.two_link_n_updates+1, cvt_state.two_link.n_updates, test_case_i.test_description.c_str());
      }
      else
      {
         CHECK_EQUAL_TEXT(test_case_i.two_link_n_updates, cvt_state.two_link.n_updates, test_case_i.test_description.c_str());
      }
   }
}

/** \purpose
 * Test that after the initialzation from scratch, the EKF status flags and states are updated correctly, based on the measurements.
 * To test the EKFs are functioning as intended
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, cvt_execute_marks_filter_state_flags_correctly_when_init_from_scratch)
{

   /** \precond
    * define several tests with different variables satisfying / violating the condition for EKF updates
    */

   // Define the struct to save all parameters for one test
   struct cv_trailer_state_update_test_T
   {
      bool f_primary_measurement_valid;
      int32_t one_link_n_updates;
      int32_t two_link_n_updates;
      bool expect_one_link_state_active;
      bool expect_two_link_state_active;
      bool expect_one_link_updated;
      bool expect_two_link_updated;
      std::string test_description;  // Descriptor of the test
   };

   // Define different test cases
   std::vector<cv_trailer_state_update_test_T> all_test_cases = {
      //f_primary_measurement_valid,  one_link_n_updates,  two_link_n_updates,  expect_one_link_state_active,  expect_two_link_state_active,  expect_one_link_updated, expect_two_link_updated, test_description
       {true,                         99,                  99,                  false,                         false,                         true,                    true,                    "Test1: expect both filter state flag to be INIT"},
       {true,                         99,                 100,                  false,                         true,                          true,                    true,                    "Test2: ONLY expect two link filter to be ACTIVE"},
       {true,                         100,                 99,                  true,                          false,                         true,                    true,                    "Test3: ONLY expect one link filter to be ACTIVE"},
       {true,                         100,                100,                  true,                          true,                          true,                    true,                    "Test4: expect both filter state flag to be ACTIVE"},
       {true,                       int32_t(1E6), int32_t(1E6),                 true,                          true,                          false,                   false,                   "Test5: Both ekf models should be updated and ACTIVE, the counter should not grow in any models"},
       {true,                       int32_t(1E6), int32_t(1E6),                 true,                          true,                          false,                   false,                   "Test6: Both ekf models should be updated and ACTIVE, the counter should not grow in any models"}
      };

   // Loop over all test cases
   for (const cv_trailer_state_update_test_T &test_case_i : all_test_cases)
   {
      cvt_state.f_initiated_from_states = false;
      init_data.one_link_model.n_updates = test_case_i.one_link_n_updates;
      init_data.two_link_model.n_updates = test_case_i.two_link_n_updates;
      CVT_Initialize(calib, init_data, cvt_state);

      cvt_state.radar_vcs_latpos = -1.0F;
      cvt_state.one_link.ekf_state[0] = F360_DEG2RAD(15.2F);
      cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(15.2F);
      cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(15.2F);
      const float32_t desired_trailer_angle_vcs = F360_DEG2RAD(15.2F);  // No difference with state at all

      const float32_t desired_trailer_intersect_vcs_long = -5.0F;
      if (test_case_i.f_primary_measurement_valid)
      {
         const float32_t k_trailer_orth_offset = 0.15F * 2.55F; // 15% of trailer width
         cvt_input.host_speed = 3.0F;
         const float32_t intersect_offset = fabsf((k_trailer_orth_offset) / F360_Sinf(desired_trailer_intersect_vcs_long));
         cvt_input.n_detections = 7;
         for (int32_t i = 0; i < 7; i++)
         {
            // Create 6 valid dets that form a line
            cvt_input.detections[i].range_rate = 0.3F;
            cvt_input.detections[i].vcs_longpos = - static_cast<float32_t>(i + 2) + desired_trailer_intersect_vcs_long + intersect_offset;
            cvt_input.detections[i].vcs_latpos = -static_cast<float32_t>(i + 2) * F360_Tanf(desired_trailer_angle_vcs);
         }
      }
      else
      {
         cvt_input.n_detections = 0;
      }

      cvt_state.one_link.n_updates = test_case_i.one_link_n_updates;
      cvt_state.two_link.n_updates = test_case_i.two_link_n_updates;
      const bool f_expect_one_link_updated = test_case_i.expect_one_link_updated;
      const bool f_expect_two_link_updated = test_case_i.expect_two_link_updated;
      const bool f_expect_one_link_state_active = test_case_i.expect_one_link_state_active;
      const bool f_expect_two_link_state_active = test_case_i.expect_two_link_state_active;

      /** \action
       * initialize the cv trailer
       */
      CVT_Execute(calib, cvt_input, cvt_state);

      /** \result
       * check the flag for successful initialization and EKF updates
       */
      if (f_expect_one_link_updated)
      {
         CHECK_EQUAL_TEXT(test_case_i.one_link_n_updates+1, cvt_state.one_link.n_updates, test_case_i.test_description.c_str());
      }
      else
      {
         CHECK_EQUAL_TEXT(test_case_i.one_link_n_updates, cvt_state.one_link.n_updates, test_case_i.test_description.c_str());
      }

      if (f_expect_two_link_updated)
      {
         CHECK_EQUAL_TEXT(test_case_i.two_link_n_updates+1, cvt_state.two_link.n_updates, test_case_i.test_description.c_str());
      }
      else
      {
         CHECK_EQUAL_TEXT(test_case_i.two_link_n_updates, cvt_state.two_link.n_updates, test_case_i.test_description.c_str());
      }

      if (f_expect_one_link_state_active)
      {
         CHECK_EQUAL_TEXT(TRAILER_FILTER_STATE_ACTIVE, cvt_state.one_link.filter_state, test_case_i.test_description.c_str());
      }
      else
      {
         CHECK_EQUAL_TEXT(TRAILER_FILTER_STATE_INIT, cvt_state.one_link.filter_state, test_case_i.test_description.c_str());
      }

      if (f_expect_two_link_state_active)
      {
         CHECK_EQUAL_TEXT(TRAILER_FILTER_STATE_ACTIVE, cvt_state.two_link.filter_state, test_case_i.test_description.c_str());
      }
      else
      {
         CHECK_EQUAL_TEXT(TRAILER_FILTER_STATE_INIT, cvt_state.two_link.filter_state, test_case_i.test_description.c_str());
      }
   }
}

/** \purpose
 * Test that after initilization from states, the EKF status flags and states are updated correctly, based on the measurements.
 * To test the EKFs are functioning as intended
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, cvt_execute_marks_filter_state_flags_correctly_when_init_from_states)
{

   /** \precond
    * define several tests with different variables satisfying / violating the condition for EKF updates
    */

   // Define the struct to save all parameters for one test
   struct cv_trailer_state_update_test_T
   {
      bool f_primary_measurement_valid;
      int32_t one_link_n_updates;
      int32_t two_link_n_updates;
      bool expect_one_link_state_active;
      bool expect_two_link_state_active;
      bool expect_one_link_updated;
      bool expect_two_link_updated;
      std::string test_description;  // Descriptor of the test
   };

   // Define different test cases
   std::vector<cv_trailer_state_update_test_T> all_test_cases = {
      //f_primary_measurement_valid,  one_link_n_updates,  two_link_n_updates,  expect_one_link_state_active,  expect_two_link_state_active,  expect_one_link_updated, expect_two_link_updated, test_description
       {true,                         99,                  99,                  false,                         false,                         true,                    true,                    "Test1: expect both filter state flag to be INIT"},
       {true,                         99,                  100,                  false,                         true,                          true,                    true,                    "Test2: ONLY expect two link filter to be ACTIVE"},
       {true,                         100,                  99,                  true,                          false,                         true,                    true,                    "Test3: ONLY expect one link filter to be ACTIVE"},
       {true,                         100,                  100,                  true,                          true,                          true,                    true,                    "Test4: expect both filter state flag to be ACTIVE"},
       {true,                       int32_t(1E6), int32_t(1E6),                 true,                          true,                          false,                   false,                   "Test5: Both ekf models should be updated and ACTIVE, the counter should not grow in any models"},
       {true,                       int32_t(1E6), int32_t(1E6),                 true,                          true,                          false,                   false,                   "Test6: Both ekf models should be updated and ACTIVE, the counter should not grow in any models"}
      };

   // Loop over all test cases
   for (const cv_trailer_state_update_test_T &test_case_i : all_test_cases)
   {
      cvt_state.f_initiated_from_states = true;

      CVT_Initialize(calib, init_data, cvt_state);

      cvt_state.radar_vcs_latpos = -1.0F;
      cvt_state.one_link.ekf_state[0] = F360_DEG2RAD(15.2F);
      cvt_state.two_link.ekf_state[0] = F360_DEG2RAD(15.2F);
      cvt_state.two_link.ekf_state[3] = F360_DEG2RAD(15.2F);
      const float32_t desired_trailer_angle_vcs = F360_DEG2RAD(15.2F);  // No difference with state at all

      const float32_t desired_trailer_intersect_vcs_long = -5.0F;
      if (test_case_i.f_primary_measurement_valid)
      {
         const float32_t k_trailer_orth_offset = 0.15F * 2.55F; // 15% of trailer width
         cvt_input.host_speed = 3.0F;
         const float32_t intersect_offset = fabsf((k_trailer_orth_offset) / F360_Sinf(desired_trailer_intersect_vcs_long));
         cvt_input.n_detections = 7;
         for (int32_t i = 0; i < 7; i++)
         {
            // Create 6 valid dets that form a line
            cvt_input.detections[i].range_rate = 0.3F;
            cvt_input.detections[i].vcs_longpos = - static_cast<float32_t>(i + 2) + desired_trailer_intersect_vcs_long + intersect_offset;
            cvt_input.detections[i].vcs_latpos = -static_cast<float32_t>(i + 2) * F360_Tanf(desired_trailer_angle_vcs);
         }
      }
      else
      {
         cvt_input.n_detections = 0;
      }
      cvt_state.one_link.n_updates = test_case_i.one_link_n_updates;
      cvt_state.two_link.n_updates = test_case_i.two_link_n_updates;

      const bool f_expect_one_link_updated = test_case_i.expect_one_link_updated;
      const bool f_expect_two_link_updated = test_case_i.expect_two_link_updated;
      const bool f_expect_one_link_state_active = test_case_i.expect_one_link_state_active;
      const bool f_expect_two_link_state_active = test_case_i.expect_two_link_state_active;

      /** \action
       * initialize the cv trailer
       */
      CVT_Execute(calib, cvt_input, cvt_state);

      /** \result
       * check the flag for successful initialization and EKF updates
       */
      if (f_expect_one_link_updated)
      {
         CHECK_EQUAL_TEXT(test_case_i.one_link_n_updates+1, cvt_state.one_link.n_updates, test_case_i.test_description.c_str());
      }
      else
      {
         CHECK_EQUAL_TEXT(test_case_i.one_link_n_updates, cvt_state.one_link.n_updates, test_case_i.test_description.c_str());
      }

      if (f_expect_two_link_updated)
      {
         CHECK_EQUAL_TEXT(test_case_i.two_link_n_updates+1, cvt_state.two_link.n_updates, test_case_i.test_description.c_str());
      }
      else
      {
         CHECK_EQUAL_TEXT(test_case_i.two_link_n_updates, cvt_state.two_link.n_updates, test_case_i.test_description.c_str());
      }

      if (f_expect_one_link_state_active)
      {
         CHECK_EQUAL_TEXT(TRAILER_FILTER_STATE_ACTIVE, cvt_state.one_link.filter_state, test_case_i.test_description.c_str());
      }
      else
      {
         CHECK_EQUAL_TEXT(TRAILER_FILTER_STATE_INIT, cvt_state.one_link.filter_state, test_case_i.test_description.c_str());
      }

      if (f_expect_two_link_state_active)
      {
         CHECK_EQUAL_TEXT(TRAILER_FILTER_STATE_ACTIVE, cvt_state.two_link.filter_state, test_case_i.test_description.c_str());
      }
      else
      {
         CHECK_EQUAL_TEXT(TRAILER_FILTER_STATE_INIT, cvt_state.two_link.filter_state, test_case_i.test_description.c_str());
      }
   }
}

/** \purpose
 * In CVT_Execute(), the angle gate of 1-link model is not only determined by the number of measurement updates
 * When the measurement near the FoV border, the host is turning aggressively and the measurement angle is small
 * a larger angle gate will be used to help the trailer catch up the trailer measurement.
 * This test verifies that logic.
 * \req
 * NA
 */
TEST(f360_cvt_estimator_init_and_execute, cvt_execute_uses_larger_angle_gate_1link_when_measurement_near_fov_border)
{
   // Define the struct to save all parameters for one test
   struct cv_trailer_1link_angle_gate_test_T
   {
      float32_t host_yaw_rate;
      float32_t measured_angle;
      int32_t n_update_1link;
      bool msmt_update_expected;
      std::string test_description;  // Descriptor of the test
   };

   // Define different test cases
   std::vector<cv_trailer_1link_angle_gate_test_T> all_test_cases = {
      // host yaw rate,       primary measured angle,   n_update of 1 link model,     msmt_update_expected,  test_description
       { F360_DEG2RAD(15.1F), F360_DEG2RAD(19.9F),      100,                          true,                  "Test 1: when all 3 conditions are met, 1-link model should use larger angle gate, and update the measurement accordingly. n_update should increment."},
       { F360_DEG2RAD(15.0F), F360_DEG2RAD(19.9F),      100,                          false,                 "Test 2: when host yaw rate is not higher than 15 deg/s, use the normal n_update based angle gate.  n_update should not increment."},
       { F360_DEG2RAD(15.1F), F360_DEG2RAD(21.0F),      100,                          false,                 "Test 3: when primary measured angle is not smaller than 20 deg, use the normal n_update based angle gate. n_update should not increment."},
       { F360_DEG2RAD(15.1F), F360_DEG2RAD(-19.9F),       0,                          true,                  "Test 4: when the n_update confidence based angle gate is larger, use it. Despite of an angle difference of 39.9 deg,  n_update should still increment."}
      };

   // Loop over all test cases
   for (const cv_trailer_1link_angle_gate_test_T &test_case_i : all_test_cases)
   {
      /** \precond
       * cvt_state initialized from scratch,  the cvt_input and cvt_state are prepared based on test case setup
       */
      // Initialize the cvt_state and cvt_input based on test case setup, to prepare for different measurement updates
      init_data.radar_vcs_latpos = test_case_i.measured_angle > 0.0F? -1.0F : 1.0F;  // radar side
      CVT_Initialize(calib, init_data, cvt_state);
      // make the states and measurement match
      cvt_state.one_link.ekf_state[0] = F360_DEG2RAD(0.0F);
      cvt_state.primary_measurement.f_msmt_valid = true;
      cvt_state.primary_measurement.trailer_intersect_vcs_long = init_data.one_link_model.joint1_vcs_longpos - 0.2F;
      cvt_state.one_link.ekf_state[1] = -(init_data.one_link_model.joint1_vcs_longpos - init_data.host_rear_axle_vcs_longpos) + 0.1F;

      // parse the test case configuratiions into cvt_input
      cvt_input.host_yawrate = test_case_i.host_yaw_rate;
      cvt_state.one_link.n_updates = test_case_i.n_update_1link;
      cvt_input.host_speed = 2.1F;
      for (int32_t i = 0; i < 10; i++)
      {
         // Setup the only measured line based on test case
         cvt_input.detections[i].vcs_longpos = init_data.one_link_model.joint1_vcs_longpos - static_cast<float32_t>(i) * F360_Cosf(test_case_i.measured_angle);
         cvt_input.detections[i].vcs_latpos = -static_cast<float32_t>(i) * F360_Sinf(test_case_i.measured_angle);
         cvt_input.n_detections++;
      }

      /** \action
       * Execute the cv trailer
       */
      CVT_Execute(calib, cvt_input, cvt_state);

      /** \result
       * check the EKF has been updated as expected
       */
      const int32_t expected_n_update = test_case_i.msmt_update_expected ? test_case_i.n_update_1link+1 : test_case_i.n_update_1link;
      CHECK_EQUAL_TEXT(expected_n_update, cvt_state.one_link.n_updates, test_case_i.test_description.c_str());
   }

}
/** @}*/
