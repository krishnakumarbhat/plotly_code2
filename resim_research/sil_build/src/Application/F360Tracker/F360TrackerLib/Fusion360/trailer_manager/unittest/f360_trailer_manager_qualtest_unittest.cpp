/** \file
 * This file contains qualtests for content of f360_trailer_manager.cpp file
 */

#include "f360_trailer_manager.h"
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
 * Set the host type to F360_HOST_TYPE_PASSENGER_VEHICLE and set the hardware presence to true
 * and check that the trailer output detects a trailer and default length of trailer for passenger
 * vehicle (which is 10)
 * \req   CPR-5450
 */
TEST(f360_trailer_manager, check_trailer_detector_triggered)
{
   /** \precond
    * set host type to passenger vehicle and trailer presence to true
    */
   float32_t expected_trailer_length = 10.0F;

   host.f_trailer_presence_hardware = true;
   host.host_type = F360_HOST_TYPE_PASSENGER_VEHICLE;

   /** \action
    * call Trailer_Manager().
    */
   Trailer_Manager(calibrations, host, raw_detect_list, sensors, tracker_info, detections, sensor_props, pvtrailer, cvt_state, trailer_output, timing_info);

   /** \result
    * Check that the cv trailer is called and the output is set as trailer present
    * and a default trailer length of 10 is set from the trailer detector
    */
   Get_Trailer_Manager_Output(host, cvt_state, pvtrailer, trailer_output);
   CHECK_EQUAL(f360_variant_A::TRAILER_PRESENCE_STATE_DETECTED, trailer_output.trailer_presence[0]);
   DOUBLES_EQUAL(expected_trailer_length, trailer_output.trailer_length[0], 0.1F);

}

/** \purpose
 * Ensure that all trailer detector estimation are cleared when trailer electronic signal is reset
 * \req   CPR-5446
 */
TEST(f360_trailer_manager, check_all_trailer_detector_estimation_cleared_upon_trailer_hw_reset)
{
   /** \precond
    * Set trailer estimation output to converged states
    * Set host type to a passenger vehicle
    * Set trailer electronic signal to true
    */
   trailer_output.trailer_angle[0] = 0.1F;
   trailer_output.trailer_length[0] = 6.0F;
   trailer_output.trailer_width[0] = 2.0F;
   trailer_output.trailer_presence[0] = f360_variant_A::TRAILER_PRESENCE_STATE_DETECTED;
   host.host_type = F360_HOST_TYPE_PASSENGER_VEHICLE;
   host.f_trailer_presence_hardware = true;

   /** \action
    * call Trailer_Manager() to make static bool f_internal_flag_for_trailer_presence become True.
    * Set trailer electronic signal to false
    * call Trailer_Manager() again
    */
   Trailer_Manager(calibrations, host, raw_detect_list, sensors, tracker_info, detections, sensor_props, pvtrailer, cvt_state, trailer_output, timing_info);
   host.f_trailer_presence_hardware = false;
   Trailer_Manager(calibrations, host, raw_detect_list, sensors, tracker_info, detections, sensor_props, pvtrailer, cvt_state, trailer_output, timing_info);


   /** \result
    * Check that all trailer output states including trailer length, width, angle and presence status are reset to the default states
    */
   Get_Trailer_Manager_Output(host, cvt_state, pvtrailer, trailer_output);
   CHECK_EQUAL(f360_variant_A::TRAILER_PRESENCE_STATE_NOT_DETECTED, trailer_output.trailer_presence[0]);
   CHECK_EQUAL(0.0F, trailer_output.trailer_angle[0]);
   CHECK_EQUAL(0.0F, trailer_output.trailer_length[0]);
   CHECK_EQUAL(0.0F, trailer_output.trailer_width[0]);

}

/** \purpose
 * Verify that trailer angle estimation shall start outputing after host speed is above 0.2 m/s and yaw rate above 0.02 rad/s consecutively for 1 second
 * \req   CPR-5448
 */
TEST(f360_trailer_manager, check_trailer_angle_estimation_successful_activation_conditions)
{
   /** \precond
    * Set host speed 0.3 m/s
    * Set yaw rate 0.01 m/s
    * Set host type to a passenger vehicle
    * Set trailer electronic signal to true
    * Set other host and tracker info parameters to ensure non-zero angle estimation
    */
   host.host_type = F360_HOST_TYPE_PASSENGER_VEHICLE;
   host.f_trailer_presence_hardware = true;
   host.speed = 0.3F;
   host.yaw_rate_rad = 0.01F;
   host.dist_rear_axle_to_vcs_m = 5.0F;
   host.vcs_sideslip = 0.01F;
   tracker_info.elapsed_time_s = 0.05F;

   /** \action
    * call Trailer_Manager() iteratively for 20 loops
    */
   for (int8_t loops=0; loops < 20; loops++)
   {
      Trailer_Manager(calibrations, host, raw_detect_list, sensors, tracker_info, detections, sensor_props, pvtrailer, cvt_state, trailer_output, timing_info);

      /** \result
       * Check that trailer angle estimation is still 0.0 at the 19th loop
       */
      if (18 == loops)
      {
         Get_Trailer_Manager_Output(host, cvt_state, pvtrailer, trailer_output);
         CHECK_EQUAL(trailer_output.trailer_angle[0], 0.0F);
      }
      /** \result
       * Check that trailer angle estimation is nonzero at the 20th loop
       */
      if (19 == loops)
      {
         Get_Trailer_Manager_Output(host, cvt_state, pvtrailer, trailer_output);
         CHECK_TRUE(trailer_output.trailer_angle[0] > 0.0F);
      }

   }
}

/** \purpose
 * Verify that trailer angle estimation shall not output a valid angle if the host speed is smaller than 0.2 m/s
 * \req   CPR-5448
 */
TEST(f360_trailer_manager, check_trailer_angle_estimation_activation_conditions_host_speed)
{
   /** \precond
    * Set host speed 0.19 m/s
    * Set yaw rate 0.01 m/s
    * Set host type to a passenger vehicle
    * Set trailer electronic signal to true
    * Set other host and tracker info parameters to ensure non-zero angle estimation
    */
   host.host_type = F360_HOST_TYPE_PASSENGER_VEHICLE;
   host.f_trailer_presence_hardware = true;
   host.speed = 0.19F;
   host.yaw_rate_rad = 0.01F;
   host.dist_rear_axle_to_vcs_m = 5.0F;
   host.vcs_sideslip = 0.01F;
   tracker_info.elapsed_time_s = 0.05F;

   /** \action
    * call Trailer_Manager() iteratively for 21 loops
    */
   for (int8_t loops=0; loops < 21; loops++)
   {
      Trailer_Manager(calibrations, host, raw_detect_list, sensors, tracker_info, detections, sensor_props, pvtrailer, cvt_state, trailer_output, timing_info);
   }

   /** \result
   * Check that trailer angle estimation is zero after 21 loops
   */
   Get_Trailer_Manager_Output(host, cvt_state, pvtrailer, trailer_output);
   CHECK_EQUAL(trailer_output.trailer_angle[0], 0.0F);
}

/** \purpose
 * Verify that trailer angle estimation shall not output a valid angle if the yaw rate is larger than 0.02 rad/s
 * \req   CPR-5448
 */
TEST(f360_trailer_manager, check_trailer_angle_estimation_activation_conditions_yaw_rate)
{
   /** \precond
    * Set host speed 0.3 m/s
    * Set yaw rate 0.03 m/s
    * Set host type to a passenger vehicle
    * Set trailer electronic signal to true
    * Set other host and tracker info parameters to ensure non-zero angle estimation
    */
   host.host_type = F360_HOST_TYPE_PASSENGER_VEHICLE;
   host.f_trailer_presence_hardware = true;
   host.speed = 0.3F;
   host.yaw_rate_rad = 0.03F;
   host.dist_rear_axle_to_vcs_m = 5.0F;
   host.vcs_sideslip = 0.01F;
   tracker_info.elapsed_time_s = 0.05F;

   /** \action
    * call Trailer_Manager() iteratively for 21 loops
    */
   for (int8_t loops=0; loops < 21; loops++)
   {
      Trailer_Manager(calibrations, host, raw_detect_list, sensors, tracker_info, detections, sensor_props, pvtrailer, cvt_state, trailer_output, timing_info);
   }

   /** \result
   * Check that trailer angle estimation is zero after 21 loops
   */
   Get_Trailer_Manager_Output(host, cvt_state, pvtrailer, trailer_output);
   CHECK_EQUAL(trailer_output.trailer_angle[0], 0.0F);
}

/** \purpose
 * Verify that trailer angle estimation shall be reset after the host speed is below 0 m/s
 * \req   CPR-5449
 */
TEST(f360_trailer_manager, check_trailer_angle_estimation_reset)
{
   /** \precond
    * Set host speed 0.5 m/s
    * Set yaw rate 0.01 m/s
    * Set host type to a passenger vehicle
    * Set trailer electronic signal to true
    * Set other host and tracker info parameters to ensure non-zero angle estimation
    */
   host.host_type = F360_HOST_TYPE_PASSENGER_VEHICLE;
   host.f_trailer_presence_hardware = true;
   host.speed = 0.5F;
   host.yaw_rate_rad = 0.01F;
   host.dist_rear_axle_to_vcs_m = 5.0F;
   host.vcs_sideslip = 0.01F;
   tracker_info.elapsed_time_s = 0.05F;

   /** \action
    * call Trailer_Manager() iteratively for 21 loops
    */
   for (int8_t loops=0; loops < 21; loops++)
   {
      Trailer_Manager(calibrations, host, raw_detect_list, sensors, tracker_info, detections, sensor_props, pvtrailer, cvt_state, trailer_output, timing_info);

      /** \result
       * Check that trailer angle estimation is nonzero at the 20th loop
       * Change the host speed to -0.1 m/s
       */
      if (19 == loops)
      {
         Get_Trailer_Manager_Output(host, cvt_state, pvtrailer, trailer_output);
         CHECK_TRUE(trailer_output.trailer_angle[0] > 0.0F);
         host.speed = -0.1F;
      }
   }

   /** \result
   * Check that trailer angle estimation is zero after 21 loops
   */
   Get_Trailer_Manager_Output(host, cvt_state, pvtrailer, trailer_output);
   CHECK_EQUAL(trailer_output.trailer_angle[0], 0.0F);
}
/** @}*/
