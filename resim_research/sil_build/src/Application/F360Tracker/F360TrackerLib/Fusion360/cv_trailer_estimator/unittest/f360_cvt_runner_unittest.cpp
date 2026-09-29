/** \file
 * This file contains unit tests for content of f360_cvt_runner.cpp file
 */

#include "f360_cvt_runner.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_cvt_runner
 *  @{
 */

/** \brief
 * Test group sets up input required to run CV trailer
 */
TEST_GROUP(f360_cvt_runner)
{
      F360_Host_T host = {};
      rspp_variant_A::RSPP_Detection_List_T raw_detect_list = {};
      F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS] = {};
      F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
      F360_Tracker_Info_T tracker_info = {};
      F360_CVT_State_T cvt_state = {};
      F360_TRKR_TIMING_INFO_T timing_info = {};
      F360_Calibrations_T calib{};
      float32_t epsilon = 1e-2F;
   /** \setup
    * Describe what is done in test setup. Remove test setup function and this tag if it is not used.
    */
   TEST_SETUP()
   {
      // Initialize calibrations
      Initialize_Tracker_Calibrations(calib);

      // radar calibrations
      sensors[0].constant.id = 1;
      sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;
      sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = 1.03F;
      sensors[0].constant.mounting_position.vcs_position.lateral = 0.8F;
      sensors[0].constant.mounting_position.vcs_position.longitudinal = -0.37F;

      // vehicle dynamics
      host.vcs_speed = 3.0F;
      host.yaw_rate_rad = 0.0F;
      host.dist_rear_axle_to_vcs_m = 5.0F;
      host.vcs_sideslip = 0.01F;
      host.vehicle_length = 5.0F;
      host.vehicle_width = 2.0F;
      tracker_info.cnt_loops = 1;

      // detections info
      raw_detect_list.number_of_valid_detections = 0;
      for (uint16_t i = 0; i < 2; i++)
      {
         raw_detect_list.detections[i].raw.sensor_id = 1;
         raw_detect_list.detections[i].raw.range_rate = 1.0F;
         raw_detect_list.detections[i].processed.f_ok_to_use = 1;
         raw_detect_list.number_of_valid_detections++;
         det_props[i].vcs_position.y = -5.0F;
         det_props[i].vcs_position.x = +10.0F;
         det_props[i].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
      }
   }

};

/** \purpose
 * Run CV trailer and check that the trailer output has an initialized length of trailer
 * when all the init conditions are met
 * * \req
 * NA
 *  */
TEST(f360_cvt_runner, F360_CV_Trailer_Estimator_initialized)
{
   /** \precond
    * All valid conditions for CV trailer have been setup in the test group.
    * Expect the trialer estimator is initialized and gives an output of the trailer length
    */
      
   /** \action
    * call Run_Trailer_Estimator().
    */
   Run_CV_Trailer_Estimator(calib, host, raw_detect_list, det_props, sensors, tracker_info, cvt_state, timing_info);

   /** \result
    * Check that the initialization is complete and trailer length is set
    */
   CHECK_TRUE(cvt_state.f_init_complete);
   CHECK_TRUE(cvt_state.one_link.trailer_length > 0.0F);
}

/** \purpose
 * Run CV trailer and check that the F360_CV_Trailer_Estimator is not initialized
 * when the required inputs are incorrect.
 * For eg. set a wrong mounting position
 * * \req
 * NA
 *  */
TEST(f360_cvt_runner, F360_CV_Trailer_Estimator_not_initialized)
{
   /** \precond
    * Set up a wrong mounting position and expect initialization not complete
    * and trailer length of 0
    */
   sensors[0].constant.mounting_position.vcs_position.longitudinal = 0.37F;

   /** \action
    * call Run_Trailer_Estimator().
    */
   Run_CV_Trailer_Estimator(calib, host, raw_detect_list, det_props, sensors, tracker_info, cvt_state, timing_info);

   /** \result
    * Check that the initialization is not complete and trailer lengths are the initial values
    */
   CHECK_FALSE(cvt_state.f_init_complete);
   DOUBLES_EQUAL_TEXT(5.0F, cvt_state.one_link.trailer_length, epsilon, "Trailer length of the 1-link model should be 5.0 when initialized from scratch and no measurement update has been done!")
   DOUBLES_EQUAL_TEXT(5.0F, cvt_state.two_link.trailer1_length, epsilon, "Trailer1 length of the 2-link model should be 5.0 when initialized from scratch and no measurement update has been done!")
   DOUBLES_EQUAL_TEXT(5.0F, cvt_state.two_link.trailer2_length, epsilon, "Trailer2 length of the 2-link model should be 5.0 when initialized from scratch and no measurement update has been done!")
}

/** \purpose
 * Run CV trailer and check that the F360_CV_Trailer_Estimator is initialized
 * when the required inputs are correct for left rear radar.
 * * \req
 * NA
 *  */
TEST(f360_cvt_runner, F360_CV_Trailer_Estimator_initialized_Radar_Det_match_at_Left_Rear)
{
   /** \precond
    * Mount the radar at left and expect initialization complete
    * and trailer length of not 0
    */
    // radar calibrations
    sensors[0].constant.id = 1;
    sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;
    sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = -1.03F;
    sensors[0].constant.mounting_position.vcs_position.lateral = -0.8F;
    sensors[0].constant.mounting_position.vcs_position.longitudinal = -0.37F;

   /** \action
    * call Run_Trailer_Estimator().
    */
   Run_CV_Trailer_Estimator(calib, host, raw_detect_list, det_props, sensors, tracker_info, cvt_state, timing_info);

   /** \result
    * Check that the initialization is completed and trailer length is larger than 0 after the measurement update
    */
   CHECK_TRUE(cvt_state.f_init_complete);
   CHECK_TRUE(cvt_state.one_link.trailer_length > 0.0F);
   CHECK_TRUE(cvt_state.two_link.trailer1_length > 0.0F);
   CHECK_TRUE(cvt_state.two_link.trailer2_length > 0.0F);
}

/** \purpose
 * Run CV trailer and check that the F360_CV_Trailer_Estimator is not initialized
 * when the radar position is not matched.
 * For eg. set a wrong radar location
 * * \req
 * NA
 *  */
TEST(f360_cvt_runner, F360_CV_Trailer_Estimator_not_initialized_Radar_Det_mismatch_at_Front)
{
   /** \precond
    * Set up a wrong mounting position and expect initialization not complete
    * and trailer length of 0
    */
    sensors[0].constant.id = 1;
    sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;  // without any rear radar, no initialization will be done
    sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = -1.03F;
    sensors[0].constant.mounting_position.vcs_position.lateral = -0.8F;

   /** \action
    * call Run_Trailer_Estimator().
    */
   Run_CV_Trailer_Estimator(calib, host, raw_detect_list, det_props, sensors, tracker_info, cvt_state, timing_info);

   /** \result
    * Check that the initialization is not complete and trailer length is 0
    */
   CHECK_FALSE(cvt_state.f_init_complete);
   CHECK_EQUAL_TEXT(0, cvt_state.one_link.trailer_length, "Trailer length of the 1-link model should be 0 when radar is not at rear!")
   CHECK_EQUAL_TEXT(0, cvt_state.two_link.trailer1_length, "Trailer1 length of the 2-link model should be 0 when radar is not at rear!")
   CHECK_EQUAL_TEXT(0, cvt_state.two_link.trailer2_length, "Trailer2 length of the 2-link model should be 0 when radar is not at rear!")
}

/** \purpose
 * Run CV trailer and check that the F360_CV_Trailer_Estimator is indeed initialized.
 * However, detection list should empty, as there are no detections from the rear radar.
 * * \req
 * NA
 *  */
TEST(f360_cvt_runner, F360_CV_Trailer_Estimator_Detection_Not_Parsed_if_from_Front_Radar)
{
   /** \precond
    * Set up a radar with id mismatching the detections
    * initialization should still be done, but no detection will be parsed
    */
    sensors[0].constant.id = 2;  // although the sensor id of dets are 1, here the only valid sensor has id = 2
    sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;  // with a rear radar, initialization will be done
    sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = -1.03F;
    sensors[0].constant.mounting_position.vcs_position.lateral = -0.8F;

   /** \action
    * call Run_Trailer_Estimator().
    */
   Run_CV_Trailer_Estimator(calib, host, raw_detect_list, det_props, sensors, tracker_info, cvt_state, timing_info);

   /** \result
    * Check that the initialization is complete but detectiions are not parsed
    */
   CHECK_TRUE(cvt_state.f_init_complete);
   CHECK_EQUAL_TEXT(0, cvt_state.n_valid_dets, "There should be no detection passed into cvt_execute!")
   CHECK_EQUAL_TEXT(0, cvt_state.primary_measurement.n_selected_dets, "There should be no detection passed into cvt_execute!")
   CHECK_EQUAL_TEXT(0, cvt_state.secondary_measurement.n_selected_dets, "There should be no detection passed into cvt_execute!")
}

/** \purpose
 * Run CV trailer and check that the F360_CV_Trailer_Estimator is not initialized again
 * if it has already been initialized.
 * * \req
 * NA
 *  */
TEST(f360_cvt_runner, F360_CV_Trailer_Estimator_Not_initialize_again_if_already_initialized)
{
   /** \precond
    * The trialer estimator is already initialized and with some predefined states
    * Expect that the initialization is not done again
    */
   cvt_state.f_init_complete = true;
   cvt_state.one_link.ekf_state[0] = 0.2F; // set a non zero trailer angle
   cvt_state.one_link.ekf_state[1] = -2.5F; // set a non zero rear axle to joint distance
   cvt_state.one_link.ekf_state[2] = 2.0F; // set a non zero joint to wheels distance
   cvt_state.two_link.ekf_state[0] = 0.2F; // set a non zero trailer angle
   cvt_state.two_link.ekf_state[1] = -2.7F; // set a non zero rear axle to joint1 distance
   cvt_state.two_link.ekf_state[2] = 2.0F; // set a non zero joint1 to joint2 distance
   cvt_state.two_link.ekf_state[3] = 0.2F; // set a non zero trailer2
   cvt_state.two_link.ekf_state[4] = 8.0F; // set a non zero joint2 to wheels distance


   cvt_state.state_constraints.min_x0 = -0.5F * F360_PI;
   cvt_state.state_constraints.max_x0 = 0.5F * F360_PI;
   cvt_state.state_constraints.min_x1 = (0.2F * host.vehicle_length - host.dist_rear_axle_to_vcs_m);
   cvt_state.state_constraints.max_x1 = (1.5F * host.vehicle_length - host.dist_rear_axle_to_vcs_m);
   cvt_state.state_constraints.min_x2 = 3.0F;
   cvt_state.state_constraints.max_x2 = 30.0F;
   cvt_state.state_constraints.min_x3 = -0.75F * F360_PI;
   cvt_state.state_constraints.max_x3 = 0.75F * F360_PI;
   cvt_state.state_constraints.min_x4 = 5.0F;
   cvt_state.state_constraints.max_x4 = 30.0F;

   cvt_state.one_link.ekf_state_errcov[0][0] = 0.6F;
   cvt_state.one_link.ekf_state_errcov[1][1] = 1.1F;
   cvt_state.one_link.ekf_state_errcov[2][2] = 1.1F;
   cvt_state.two_link.ekf_state_errcov[0][0] = 0.6F;
   cvt_state.two_link.ekf_state_errcov[1][1] = 1.1F;
   cvt_state.two_link.ekf_state_errcov[2][2] = 1.1F;
   cvt_state.two_link.ekf_state_errcov[3][3] = 1.1F;
   cvt_state.two_link.ekf_state_errcov[4][4] = 1.1F;

   cvt_state.one_link.n_updates = 10;
   cvt_state.two_link.n_updates = 20;

   // Setup the detections in a way that they are aligned with the previous state
   raw_detect_list.number_of_valid_detections = 0;
   for (uint16_t i = 0; i < 15; i++)
   {
      raw_detect_list.detections[i].raw.sensor_id = 1;
      raw_detect_list.detections[i].raw.range_rate = 0.59F;
      raw_detect_list.detections[i].processed.f_ok_to_use = 1;
      raw_detect_list.number_of_valid_detections++;
      det_props[i].vcs_position.y = -static_cast<float32_t>(i) * F360_Sinf(0.22F);
      det_props[i].vcs_position.x = 1.0F - static_cast<float32_t>(i) * F360_Cosf(0.22F);
      det_props[i].motion_status = rspp_variant_A::RSPP_DETECTION_MOTION_STATUS_MOVING;
   }
   cvt_state.radar_id = 1;

   const float32_t epsilon = 1E-2F;

   /** \action
    * call Run_Trailer_Estimator().
    */
   Run_CV_Trailer_Estimator(calib, host, raw_detect_list, det_props, sensors, tracker_info, cvt_state, timing_info);

   /** \result
    * Check that the initialization is complete and 1-link model is updated while 2-link model is not updated
    */
   CHECK_TRUE(cvt_state.f_init_complete);
   DOUBLES_EQUAL_TEXT(cvt_state.one_link.ekf_state[0], 0.188F, epsilon, "State[0] of 1-link model is wrong, 1-link model should be updated")
   DOUBLES_EQUAL_TEXT(cvt_state.one_link.ekf_state[1], -2.43F, epsilon, "State[1] of 1-link model is wrong, 1-link model should be updated")
   DOUBLES_EQUAL_TEXT(cvt_state.one_link.ekf_state[2], 3.0F, epsilon, "State[2] of 1-link model is wrong, 1-link model should be updated")
   DOUBLES_EQUAL_TEXT(cvt_state.two_link.ekf_state[0], 0.185F, epsilon, "State[0] of 2-link model is wrong, 2-link model should not be updated!")
   DOUBLES_EQUAL_TEXT(cvt_state.two_link.ekf_state[1], -2.7F, epsilon, "State[1] of 2-link model is wrong, 2-link model should not be updated!")
   DOUBLES_EQUAL_TEXT(cvt_state.two_link.ekf_state[2], 3.0F, epsilon, "State[2] of 2-link model is wrong, 2-link model should not be updated!")
   DOUBLES_EQUAL_TEXT(cvt_state.two_link.ekf_state[3], 0.2F, epsilon, "State[3] of 2-link model is wrong, 2-link model should not be updated!")
   DOUBLES_EQUAL_TEXT(cvt_state.two_link.ekf_state[4], 8.0F, epsilon, "State[4] of 2-link model is wrong, 2-link model should not be updated!")
   CHECK_EQUAL_TEXT(11, cvt_state.one_link.n_updates, "One-link model should be updated!")
   CHECK_EQUAL_TEXT(20, cvt_state.two_link.n_updates, "Two-link model shall not be updated!")
}
/** @}*/
