/** \file
 * This file contains unit tests for content of f360_pvtrailer_main.cpp file
 */

#include <cstring>
#include "CppUTest/TestHarness.h"
#include "f360_pvtrailer_main.h"

using namespace f360_variant_A;

/** \defgroup  f360_pvtrailer_main_parse_output
 *  @{
 */

/** \brief
 * Test that the Parse_PV_Trailer_Output is populating the output structure as intended
 */
TEST_GROUP(f360_pvtrailer_main_parse_output)
{
   F360_PVTrailer_Data_T pvtrailer;

   // Setup the different types of output structures that the trailer detector are using
   F360_Trailer_Estimator_Output_T trailer_detector_output;
   
   const float32_t k_default_trailer_length = 10.0F;
   const float32_t k_default_trailer_width = 2.5F;
   
   // Set up threshold for floating number comparision
   float32_t test_pass_th = 1e-9F;

   /** \setup
    * Set up output relevant data in the pvtrailer struct
    */
   TEST_SETUP()
   {
      (void)memset(&pvtrailer, 0, sizeof(pvtrailer));
      (void)memset(&trailer_detector_output, 0, sizeof(trailer_detector_output));

      pvtrailer.f_trailer_connected = true;

      pvtrailer.pvtrailer_length.f_estimation_done = true;
      pvtrailer.pvtrailer_length.trailer_length = 4.5F;

      pvtrailer.pvtrailer_width.f_estimation_done = true;
      pvtrailer.pvtrailer_width.trailer_width = 2.8F;

      pvtrailer.pvtrailer_angle.trailer_angle_rad = 0.5F;
      pvtrailer.pvtrailer_angle.trailer_angle_rate_rad = 0.01F;
   }
};

/** \purpose
 * Check that Parse_PV_Trailer_Output() function works as intended when all three
 * estimator types have succeded in generating trailer estimates.
 * \req
 * NA.
 */
TEST(f360_pvtrailer_main_parse_output, Test_All_Estimators_Have_Valid_Estimates)
{
   /** \precond
    * Use default data from the test setup.
    */

   /** \action
    * Call Parse_PV_Trailer_Output().
    */
   Parse_PV_Trailer_Output(pvtrailer, trailer_detector_output);

   /** \result
    * Test that the trailer detector output is as expected
    */
   CHECK_EQUAL(TRAILER_PRESENCE_STATE_DETECTED, trailer_detector_output.trailer_presence[0]);
   
   DOUBLES_EQUAL(4.5F, trailer_detector_output.trailer_length[0], test_pass_th);
   DOUBLES_EQUAL(2.8F, trailer_detector_output.trailer_width[0], test_pass_th);
   DOUBLES_EQUAL(0.5F, trailer_detector_output.trailer_angle[0], test_pass_th);
}

/** \purpose
 * Check that Parse_PV_Trailer_Output() function works as intended when no trailer is connected.
 * \req
 * NA.
 */
TEST(f360_pvtrailer_main_parse_output, Test_No_Trailer_Connected)
{
   /** \precond
    * Use default data from the test setup.
    */
   pvtrailer.f_trailer_connected = false;

   /** \action
    * Call Parse_PV_Trailer_Output().
    */
   Parse_PV_Trailer_Output(pvtrailer, trailer_detector_output);

   /** \result
    * Test so that the trailer detector output is as expected
    */
   CHECK_EQUAL(TRAILER_PRESENCE_STATE_NOT_DETECTED, trailer_detector_output.trailer_presence[0]);
   
   DOUBLES_EQUAL(0.0F, trailer_detector_output.trailer_length[0], test_pass_th);
   DOUBLES_EQUAL(0.0F, trailer_detector_output.trailer_width[0], test_pass_th);
   DOUBLES_EQUAL(0.0F, trailer_detector_output.trailer_angle[0], test_pass_th);
}
/** @}*/


/** \defgroup  f360_pvtrailer_main_estimator
 *  @{
 */

/** \brief
 * Test that the Run_PV_Trailer_Estimator are updating pvtrailer data parameters as expected
 */
TEST_GROUP(f360_pvtrailer_main_estimator)
{
   F360_Host_T vehicle_data;
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list;
   F360_Detection_Props_T all_detections[MAX_NUMBER_OF_DETECTIONS];
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS];
   float32_t elapsed_time_s;
   F360_PVTrailer_Data_T pvtrailer_data;
   F360_TRKR_TIMING_INFO_T timing_info;
   
   /** \setup
    * Clear the pvtrailer data struct
    */
   TEST_SETUP()
   {
      (void)memset(&pvtrailer_data, 0, sizeof(pvtrailer_data));
   }
};

/** \purpose
 * Check that internal pvtrailer data parameters are updated as expected when calling
 * the Run_PV_Trailer_Estimator() function.
 * \req
 * NA.
 */
TEST(f360_pvtrailer_main_estimator, Test_First_Estimator_Call)
{
   /** \precond
    * Use default data from the test setup.
    */

   /** \action
    * Call Run_PV_Trailer_Estimator()
    */
   Run_PV_Trailer_Estimator(vehicle_data, raw_detect_list, all_detections, sensors, elapsed_time_s, pvtrailer_data, timing_info);

   /** \result
    * Test that the trailer detector output is as expected
    */
   CHECK_EQUAL(1U, pvtrailer_data.radar_detection_timer);
   CHECK_EQUAL(TRAILER_DETECTOR_STATUS_RUNNING, pvtrailer_data.trailer_detection_status);
   CHECK_TRUE(pvtrailer_data.f_trailer_connected);
}

/** \purpose
 * Check that internal pvtrailer data parameters are updated as expected when calling
 * the Run_PV_Trailer_Estimator() function after the estimation has been completed.
 * \req
 * NA.
 */
TEST(f360_pvtrailer_main_estimator, Test_Completed_Estimator_Call)
{
   /** \precond
    * Update internal data structures to indicate a completed estimate
    */
   pvtrailer_data.pvtrailer_length.f_estimation_done = true;
   pvtrailer_data.pvtrailer_length.window_timer = 1801U;
   pvtrailer_data.pvtrailer_width.f_estimation_done = true;
   pvtrailer_data.pvtrailer_width.window_timer = 1801U;

   /** \action
    * Call Run_PV_Trailer_Estimator()
    */
   Run_PV_Trailer_Estimator(vehicle_data, raw_detect_list, all_detections, sensors, elapsed_time_s, pvtrailer_data, timing_info);

   /** \result
    * Test that the trailer detector output is as expected
    */
   CHECK_EQUAL(0U, pvtrailer_data.radar_detection_timer);
   CHECK_EQUAL(TRAILER_DETECTOR_STATUS_NOT_RUNNING, pvtrailer_data.trailer_detection_status);
   CHECK_TRUE(pvtrailer_data.f_trailer_connected);
}

/** \purpose
 * Check that internal pvtrailer data parameters are updated as expected when calling
 * the Run_PV_Trailer_Estimator() function after the estimation has been partially completed.
 * \req
 * NA.
 */
TEST(f360_pvtrailer_main_estimator, Test_Partial_Completed_Estimator_Call)
{
   /** \precond
    * Update internal data structures to indicate a completed estimate
    */
   pvtrailer_data.pvtrailer_length.f_estimation_done = true;
   pvtrailer_data.pvtrailer_length.window_timer = 1801U;
   pvtrailer_data.pvtrailer_width.f_estimation_done = false;
   pvtrailer_data.pvtrailer_width.window_timer = 1801U;

   /** \action
    * Call Run_PV_Trailer_Estimator()
    */
   Run_PV_Trailer_Estimator(vehicle_data, raw_detect_list, all_detections, sensors, elapsed_time_s, pvtrailer_data, timing_info);

   /** \result
    * Test that the trailer detector output is as expected
    */
   CHECK_EQUAL(1U, pvtrailer_data.radar_detection_timer);
   CHECK_EQUAL(TRAILER_DETECTOR_STATUS_RUNNING, pvtrailer_data.trailer_detection_status);
   CHECK_TRUE(pvtrailer_data.f_trailer_connected);
}
/** @}*/
