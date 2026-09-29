/** \file
 * This file contains unit tests for content of f360_trailer_manager.cpp file
 */

#include "f360_trailer_manager.h"
#include "f360_pvtrailer_main.h"
#include <CppUTest/TestHarness.h>


// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_trailer_manager
 *  @{
 */

/** \brief
 * Test group sets up input required to run Trailer manager, so we can trigger both
 * CV trailer and trailer detector
 */
TEST_GROUP(f360_trailer_manager)
{
   F360_Calibrations_T calibrations = {};
   F360_Host_T host = {};
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list = {};
   F360_Detection_Props_T detections[MAX_NUMBER_OF_DETECTIONS] = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Radar_Sensor_Props_T sensor_props[MAX_NUMBER_OF_SENSORS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_PVTrailer_Data_T pvtrailer = {};
   F360_CVT_State_T cvt_state = {};
   F360_Trailer_Estimator_Output_T trailer_output = {};
   F360_TRKR_TIMING_INFO_T timing_info = {};


   /** \setup
    * Set up host calibrations, radar calibrations and host data and detection data for one scan
    */
   TEST_SETUP()
   {
      // vehicle calibrations
      calibrations.host_vehicle_length = 5.0F;
      calibrations.host_vehicle_width = 2.0F;

      // radar calibrations
      sensors[0].constant.id = 1;
      sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;
      sensors[0].constant.mounting_position.vcs_boresight_azimuth_angle = 1.03F;
      sensors[0].constant.mounting_position.vcs_position.lateral = 0.8;
      sensors[0].constant.mounting_position.vcs_position.longitudinal = -0.37F;

      // vehicle dynamics
      host.vcs_speed = 3.0F;
      host.yaw_rate_rad = 0.0F;
      host.dist_rear_axle_to_vcs_m = 5.0F;
      host.vcs_sideslip = 0.01F;
      tracker_info.cnt_loops = 1;

      // detections info
      raw_detect_list.number_of_valid_detections = 0;
      for (uint16_t i = 0; i < 1; i++)
      {
         raw_detect_list.detections[i].raw.sensor_id = 1; // see if mapping is needed
         raw_detect_list.detections[i].processed.vcs_position_y = -5;
         raw_detect_list.detections[i].processed.vcs_position_x = -6;
         raw_detect_list.detections[i].raw.range_rate= 1.0F;
         raw_detect_list.detections[i].processed.motion_status =1;
         raw_detect_list.detections[i].processed.f_ok_to_use = 1;
         raw_detect_list.number_of_valid_detections++;
      }

      PVTrailer_Reset(pvtrailer);
      // set a default trailer length
      pvtrailer.pvtrailer_length.trailer_length = 10.0F;
   }

   /** \teardown
    * Clear the trailer output after each test
    */
   TEST_TEARDOWN()
   {
      trailer_output = {};
   }
};

/** \purpose
 * Set the host type to F360_HOST_TYPE_COMMERCIAL_VEHICLE and set the hardware presence to true
 * and check that the trailer output has a trailer present and has an initialized length of trailer
 * for commercial vehicle, later reset the hardware signal and see that the
 * trailer length is reset to 0
 * \req
 * NA
 */
TEST(f360_trailer_manager, check_cv_trailer_output_reset_when_no_hardware_signal_present)
{
   /** \precond
    * set host type to commercial vehicle and trailer presence to true
    */
   float32_t expected_trailer_length_after_reset = 0.0F;

   host.f_trailer_presence_hardware = true;
   host.host_type = F360_HOST_TYPE_COMMERCIAL_VEHICLE;
   host.vehicle_length = 5.0F;
   host.vehicle_width = 2.0F;

   /** \action
    * call Trailer_Manager() two times; one to trigger initialization, and one to execute.
    */
    Trailer_Manager(calibrations, host, raw_detect_list, sensors, tracker_info, detections, sensor_props, pvtrailer, cvt_state, trailer_output, timing_info);
    Trailer_Manager(calibrations, host, raw_detect_list, sensors, tracker_info, detections, sensor_props, pvtrailer, cvt_state, trailer_output, timing_info);
    // As long as hardware signal present length is estimated
    CHECK_EQUAL(f360_variant_A::TRAILER_PRESENCE_STATE_DETECTED, trailer_output.trailer_presence[0]);
    CHECK_TRUE(trailer_output.trailer_length[0] > 0.0F);
    
    // Turn off the hardware signal and see that the estimations are cleared
    host.f_trailer_presence_hardware = false;
    Trailer_Manager(calibrations, host, raw_detect_list, sensors, tracker_info, detections, sensor_props, pvtrailer, cvt_state, trailer_output, timing_info);


   /** \result
    * Check that the cv trailer is called and the output is set as 0
    */
    CHECK_EQUAL(f360_variant_A::TRAILER_PRESENCE_STATE_NOT_DETECTED, trailer_output.trailer_presence[0]);
    DOUBLES_EQUAL(expected_trailer_length_after_reset, trailer_output.trailer_length[0], 0.1F);

}

/** \purpose
 * Set the host type to F360_HOST_TYPE_PASSENGER_VEHICLE and set the hardware presence to true
 * and check that the trailer output has a trailer present and has an initialized length of trailer
 * for passenger vehicle (which is 10.0), later reset the hardware signal and see that the
 * trailer length is reset to 0
 * \req
 * NA
 */
TEST(f360_trailer_manager, check_passenger_trailer_output_reset_when_no_hardware_signal_present)
{
   /** \precond
    * set host type to passenger vehicle and trailer presence to true
    */
   float32_t expected_trailer_length = 10.0F;
   float32_t expected_trailer_length_after_reset = 0.0F;

   host.f_trailer_presence_hardware = true;
   host.host_type =  F360_HOST_TYPE_PASSENGER_VEHICLE;

   /** \action
    * call Trailer_Manager().
    */
    Trailer_Manager(calibrations, host, raw_detect_list, sensors, tracker_info, detections, sensor_props, pvtrailer, cvt_state, trailer_output, timing_info);
    // As long as hardware signal present length is estimated
    CHECK_EQUAL(f360_variant_A::TRAILER_PRESENCE_STATE_DETECTED, trailer_output.trailer_presence[0]);
    DOUBLES_EQUAL(expected_trailer_length, trailer_output.trailer_length[0], 0.1F);
    
    // Turn off the hardware signal and see that the estimations are cleared
    host.f_trailer_presence_hardware = false;
    Trailer_Manager(calibrations, host, raw_detect_list, sensors, tracker_info, detections, sensor_props, pvtrailer, cvt_state, trailer_output, timing_info);


   /** \result
    * Check that the passenger trailer is called and the output is set as 0
    */
    CHECK_EQUAL(f360_variant_A::TRAILER_PRESENCE_STATE_NOT_DETECTED, trailer_output.trailer_presence[0]);
    DOUBLES_EQUAL(expected_trailer_length_after_reset, trailer_output.trailer_length[0], 0.1F);

}

/** \purpose
 * Set the hardware presence to false and check there is no output from any of the trailer algos
 * NA
 */
TEST(f360_trailer_manager, check_no_trailer_output)
{
   /** \precond
    * Set trailer presence to false
    */

   host.f_trailer_presence_hardware = false;

   /** \action
    * call Trailer_Manager().
    */
    Trailer_Manager(calibrations, host, raw_detect_list, sensors, tracker_info, detections, sensor_props, pvtrailer, cvt_state, trailer_output, timing_info);


   /** \result
    * Check that the no trailer is called and the output is set as trailer not present
    */

   CHECK_EQUAL(f360_variant_A::TRAILER_PRESENCE_STATE_NOT_DETECTED, trailer_output.trailer_presence[0]);

}


/** \purpose
 * Check the ouput from cv trialer is parsed as zeros, when no fitting trailer model
 * is reported by the cv trailer estimator
 * \req
 * NA
 */
TEST(f360_trailer_manager, check_cv_trailer_parsed_output_no_best_trailer_model)
{
   /** \precond
    * Manually set output of one joint trailer
    */
   cvt_state.best_trailer_model = TRAILER_MODEL_NOT_AVAILABLE;
   cvt_state.one_link.trailer_length = 10.0F;
   cvt_state.one_link.trailer_width = 5.0F;
   cvt_state.one_link.ekf_state[0] = 0.02F;

   float32_t expected_trailer_length = 0.0F;
   float32_t expected_trailer_width = 0.0F;
   float32_t expected_trailer_angle = 0.0F;

   /** \action
    * call Parse_CV_Trailer_Output().
    */

   Parse_CV_Trailer_Output(cvt_state, trailer_output);

   /** \result
    * Check that the one joint trailer output is parsed to trailer_output
    */

   DOUBLES_EQUAL(expected_trailer_length, trailer_output.trailer_length[0], 0.1F);
   DOUBLES_EQUAL(expected_trailer_width, trailer_output.trailer_width[0], 0.1F);
   DOUBLES_EQUAL(expected_trailer_angle, trailer_output.trailer_angle[0], 0.001F);

}


/** \purpose
 * Check the ouput from cv trialer is parsed correctly from one joint trailer
 * to trailer_output struct
 * \req
 * NA
 */
TEST(f360_trailer_manager, check_cv_trailer_parsed_output_one_joint)
{
   /** \precond
    * Manually set output of one joint trailer
    */
   float32_t expected_trailer_length = 10.0F;
   float32_t expected_trailer_width = 2.0F;
   float32_t expected_trailer_angle = 0.02F;

   cvt_state.best_trailer_model = TRAILER_MODEL_ONE_LINK;
   cvt_state.one_link.trailer_length = expected_trailer_length;
   cvt_state.one_link.trailer_width = expected_trailer_width;
   cvt_state.one_link.ekf_state[0] = expected_trailer_angle;

   /** \action
    * call Parse_CV_Trailer_Output().
    */

   Parse_CV_Trailer_Output(cvt_state, trailer_output);

   /** \result
    * Check that the one joint trailer output is parsed to trailer_output
    */

   DOUBLES_EQUAL(expected_trailer_length, trailer_output.trailer_length[0], 0.1F);
   DOUBLES_EQUAL(expected_trailer_width, trailer_output.trailer_width[0], 0.1F);
   DOUBLES_EQUAL(expected_trailer_angle, trailer_output.trailer_angle[0], 0.001F);

}



/** \purpose
 * Check the ouput from cv trialer is parsed correctly from two joint trailer
 * to trailer_output struct when first trailer is higher angle
 * \req
 * NA
 */
TEST(f360_trailer_manager, check_cv_trailer_parsed_output_two_joint_with_more_angle_on_first_trailer)
{
   /** \precond
    * Manually set output of two joint trailer
    */
   
   cvt_state.best_trailer_model = TRAILER_MODEL_TWO_LINK;
   cvt_state.two_link.ekf_state[0] = 0.06F;
   cvt_state.two_link.trailer1_length = 10.0F;
   cvt_state.two_link.trailer1_width = 2.0F;
   cvt_state.two_link.joint1_dist_to_center = 5.0F;
   cvt_state.two_link.ekf_state[3] = 0.05F;
   cvt_state.two_link.trailer2_length = 5.0F;
   cvt_state.two_link.trailer2_width = 1.9F;
   cvt_state.two_link.joint2_dist_to_center = 5.0F;

   /** \action
    * call Parse_CV_Trailer_Output().
    */

   Parse_CV_Trailer_Output(cvt_state, trailer_output);

   /** \result
    * Check that the two joint trailer output is parsed to trailer_output
    */

   DOUBLES_EQUAL(cvt_state.two_link.trailer1_length, trailer_output.trailer_length[0], 0.1F);
   DOUBLES_EQUAL(cvt_state.two_link.trailer2_length, trailer_output.trailer_length[1], 0.1F);
   DOUBLES_EQUAL(cvt_state.two_link.trailer1_width, trailer_output.trailer_width[0], 0.1F);
   DOUBLES_EQUAL(cvt_state.two_link.trailer2_width, trailer_output.trailer_width[1], 0.1F);
   DOUBLES_EQUAL(cvt_state.two_link.ekf_state[0], trailer_output.trailer_angle[0], 0.001F);
   DOUBLES_EQUAL(cvt_state.two_link.ekf_state[3], trailer_output.trailer_angle[1], 0.001F);
}
/** @}*/
