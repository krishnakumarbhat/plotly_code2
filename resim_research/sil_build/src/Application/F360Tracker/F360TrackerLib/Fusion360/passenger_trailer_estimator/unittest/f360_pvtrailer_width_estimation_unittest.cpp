/** \file
 * This file contains unit tests for content of f360_pvtrailer_width_estimation.cpp file
 */

#include <cstring>
#include "CppUTest/TestHarness.h"
#include "f360_pvtrailer_width_estimation.h"

using namespace f360_variant_A;


/** @}*/

/** \defgroup  f360_pvtrailer_width_estimation_main
 *  @{
 */

/** \brief
 * Check that the main trailer width estimation function is executing as expected
 */
TEST_GROUP(f360_pvtrailer_width_estimation_main)
{
   F360_PVTrailer_Width_Data_T pvtrailer_width;
   F360_Host_T vehicle_data;
   rspp_variant_A::RSPP_Detection_List_T raw_detect_list;
   F360_Detection_Props_T all_detections[MAX_NUMBER_OF_DETECTIONS];
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS];
   
   const float32_t test_pass_th = 1e-6F;
   const float32_t k_default_width = 2.5F;
   /** \setup
    * Clear all input structures
    */
   TEST_SETUP()
   {
      (void)memset(&pvtrailer_width, 0, sizeof(pvtrailer_width));
      (void)memset(&vehicle_data, 0, sizeof(vehicle_data));
      (void)memset(&raw_detect_list, 0, sizeof(raw_detect_list));
      (void)memset(&all_detections, 0, sizeof(all_detections));
      (void)memset(&sensors, 0, sizeof(sensors));
   }
};

/** \purpose
 * Test that when the ovly valid x interval has a flat distribution (5 in all bins), the trailer width correctly estimated to the maximum width (4.5).
 */
TEST(f360_pvtrailer_width_estimation_main, default_output)
{
   /** \precond
    * Use the default data.
    */
   
   /** \action
    * Call the PVTrailer_Estimate_Width() function
    */
   PVTrailer_Estimate_Width(vehicle_data, raw_detect_list, all_detections, sensors, pvtrailer_width);

   /** \result
    * Check that the expected width is estimated and confidence level is correct.
    */
   CHECK_FALSE(pvtrailer_width.f_estimation_done)
   DOUBLES_EQUAL(k_default_width, pvtrailer_width.trailer_width, test_pass_th)
}

/** \purpose
 * Test that when the ovly valid x interval has a flat distribution (5 in all bins), the trailer width correctly estimated to the maximum width (4.5).
 */
TEST(f360_pvtrailer_width_estimation_main, default_output_at_speed)
{
   /** \precond
    * Use the default data. 
    * Set the host speed to a value that allows input processing
    */
   vehicle_data.speed = 1.0F;
   
   /** \action
    * Call the PVTrailer_Estimate_Width() function
    */
   PVTrailer_Estimate_Width(vehicle_data, raw_detect_list, all_detections, sensors, pvtrailer_width);

   /** \result
    * Check that the expected width is estimated and confidence level is correct.
    */
   CHECK_FALSE(pvtrailer_width.f_estimation_done)
   DOUBLES_EQUAL(k_default_width, pvtrailer_width.trailer_width, test_pass_th)
}

/** \purpose
 * Test that when the ovly valid x interval has a flat distribution (5 in all bins), the trailer width correctly estimated to the maximum width (4.5).
 */
TEST(f360_pvtrailer_width_estimation_main, execute_estimation)
{
   /** \precond
    * Use the default data. 
    * Set up data to execute estimation
    */
   vehicle_data.speed = 1.0F;
   pvtrailer_width.window_timer = 1800U;   
   pvtrailer_width.total_number_of_dets = 30U;
   pvtrailer_width.n_dets_per_area[0] = 30U;

   const int32_t values_to_insert[DETECTION_COLS] = {2687, 45, 3689, 3766, 555, 904, 2443, 3856, 2744, 267, 3625, 3863, 3505, 2020, 3380, 2486, 1227, 2718, 2669,
                                                 1696, 3116, 452, 88, 3695, 2362, 2682, 347, 1506, 3776, 2418, 1583, 412, 3712, 2983, 365, 818, 3292, 2512, 689, 1128, 3768, 2735, 1822, 3326, 1305};
   for (uint32_t i = 0U; i < DETECTION_COLS; i++)
   {
      pvtrailer_width.dets_cnt_per_x_interval[0U][i] = values_to_insert[i];
   }
   
   for (uint32_t i = 0U; i < X_INTERVALS_NUMBER; i++)
   {
      pvtrailer_width.dets_in_left_side[i] = true;
      pvtrailer_width.dets_in_right_side[i] = true;
   }
   
   /** \action
    * Call the PVTrailer_Estimate_Width() function
    */
   PVTrailer_Estimate_Width(vehicle_data, raw_detect_list, all_detections, sensors, pvtrailer_width);

   /** \result
    * Check that the expected width is estimated and confidence level is correct.
    */
   const float32_t exp_trailer_width = 2.0F;
   CHECK_TRUE(pvtrailer_width.f_estimation_done)
   DOUBLES_EQUAL(exp_trailer_width, pvtrailer_width.trailer_width, test_pass_th)
}

/** @}*/

/** \defgroup  f360_pvtrailer_width_estimation_process_input
 *  @{
 */

/** \brief
 * Check that the trailer width estimation is processing input as expected
 */
TEST_GROUP(f360_pvtrailer_width_estimation_process_input)
{	
   F360_PVTrailer_Width_Data_T pvtrailer_width;
   F360_Host_T vehicle_data;
   rspp_variant_A::RSPP_Detection_List_T raw_dets;
   F360_Detection_Props_T det_props[MAX_NUMBER_OF_DETECTIONS];
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS];

   /** \setup
    * Prepare data structs with some data that allows the algorithm to execute
    */
   TEST_SETUP()
   {
      // Set up internal parameters to represent a completed estimation
      (void)memset(&pvtrailer_width, 0, sizeof(pvtrailer_width));
      pvtrailer_width.f_estimation_done = true;
      pvtrailer_width.trailer_width = 3.0F;
      pvtrailer_width.window_timer = 10U;
      
      // Set up vehicle data
      vehicle_data.speed = 2.1F;
      const float vehicle_length = 5.5F;
      vehicle_data.dist_rear_axle_to_vcs_m = vehicle_length / 1.1F;

      // Set up sensor calibration
      sensors[0].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_REAR;       // Sensor with id 1 is valid for trailor detector
      sensors[1].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_REAR;        // Sensor with id 2 is valid for trailor detector
      sensors[2].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_REAR;      // Sensor with id 3 is valid for trailor detector
      sensors[3].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_FORWARD;     // Sensor with id 4 is not valid for trailor detector
      sensors[4].constant.mounting_location = F360_MOUNTING_LOCATION_CENTER_FORWARD;   // Sensor with id 5 is not valid for trailor detector
      sensors[5].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_FORWARD;    // Sensor with id 6 is not valid for trailor detector
      sensors[6].constant.mounting_location = F360_MOUNTING_LOCATION_LEFT_SIDE1;       // Sensor with id 7 is not valid for trailor detector
      sensors[7].constant.mounting_location = F360_MOUNTING_LOCATION_RIGHT_SIDE1;      // Sensor with id 8 is not valid for trailor detector
      
      // Set up detection data
      raw_dets.number_of_valid_detections = 9U;

      // Set up first detection such that it can be assigned a column/bin number
      raw_dets.detections[0].raw.sensor_id = 3;                                   // Left rear sensor
      raw_dets.detections[0].raw.range_rate = -(0.2F + F360_EPSILON);             // Any value such that |range_rate| < 0.3 
      det_props[0].vcs_position.x = -5.6F;                                   // -5.6 so it will be in the first x interval for easier testing. the limit are -(k_host_length + k_max_trailer_length) and -k_host_length 
      det_props[0].vcs_position.y = 1.5F;                                    // Left trailer edge
      det_props[0].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
      det_props[0].f_water_spray = false;

      // Set up second detection such that it can be assigned a column/bin number
      raw_dets.detections[1].raw.sensor_id = 2;                                   // Center rear sensor
      raw_dets.detections[1].raw.range_rate = (0.2F + F360_EPSILON);              // Any value such that |range_rate| < 0.3         
      det_props[1].vcs_position.x = -5.6F;                                    // -5.6 so it will be in the first x interval for easier testing. the limit are -(k_host_length + k_max_trailer_length) and -k_host_length
      det_props[1].vcs_position.y = -0.5F;                                     // Lateral position anywhere between -1.5 and 1.5 since k_max_trailer_width = 3.0
      det_props[1].f_double_bounce = false;
      det_props[1].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
      det_props[1].f_water_spray = false;

      // Set up third detection such that it can be assigned a column/bin number
      raw_dets.detections[2].raw.sensor_id = 1;                                   // Right rear sensor
      raw_dets.detections[2].raw.range_rate = 0.1F;                               // Any value such that |range_rate| < 0.3          
      det_props[2].vcs_position.x = -7.6F;                                   // -5.6 so it will be in the first x interval for easier testing. the limit are -(k_host_length + k_max_trailer_length) and -k_host_length 
      det_props[2].vcs_position.y = -1.5F;                                     // Right trailer edge 
      det_props[2].f_double_bounce = false;
      det_props[2].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
      det_props[2].f_water_spray = false;

      // Set up fourth detection such that it can be assigned a column/bin number
      raw_dets.detections[3].raw.sensor_id = 1;                                   // Right rear sensor
      raw_dets.detections[3].raw.range_rate = 0.3F - F360_EPSILON;                // Any value such that |range_rate| < 0.3 
      det_props[3].vcs_position.x = -7.6F;                                    // -5.6 so it will be in the first x interval for easier testing. the limit are -(k_host_length + k_max_trailer_length) and -k_host_length 
      det_props[3].vcs_position.y = 0.7F;                                     // Lateral position anywhere between -1.5 and 1.5 since k_max_trailer_width = 3.0
      det_props[3].f_double_bounce = false;
      det_props[3].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
      det_props[3].f_water_spray = false;

      // Set up fifth detection such that it can be assigned a column/bin number
      raw_dets.detections[4].raw.sensor_id = 2;                                   // Center rear sensor
      raw_dets.detections[4].raw.range_rate = 0.01F;                              // Any value such that |range_rate| < 0.3 
      det_props[4].vcs_position.x = -5.6F;                                   // -5.6 so it will be in the first x interval for easier testing. the limit are -(k_host_length + k_max_trailer_length) and -k_host_length
      det_props[4].vcs_position.y = 0.0;                                      // Lateral position anywhere between -1.5 and 1.5 since k_max_trailer_width = 3.0
      det_props[4].f_double_bounce = false;
      det_props[4].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
      det_props[4].f_water_spray = false;

      // Set up sixth detection such that it can be assigned a column/bin number
      raw_dets.detections[5].raw.sensor_id = 1;                                   // Right rear sensor
      raw_dets.detections[5].raw.range_rate = 0.3F - F360_EPSILON;                //  Any value such that |range_rate| < 0.3 
      det_props[5].vcs_position.x = -5.6F;                                   // Trailer rear 
      det_props[5].vcs_position.y = 1.5F;                                     // Right trailer edge 
      det_props[5].f_double_bounce = false;
      det_props[5].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
      det_props[5].f_water_spray = false;

      // Set up seventh detection such that it can be assigned a column/bin number
      raw_dets.detections[6].raw.sensor_id = 3;                                   // Left rear sensor
      raw_dets.detections[6].raw.range_rate = 0.3F - F360_EPSILON;                //  Any value such that |range_rate| < 0.3 
      det_props[6].vcs_position.x = -5.6F;                                    //-5.6 so it will be in the first x interval for easier testing. the limit are -(k_host_length + k_max_trailer_length) and -k_host_length 
      det_props[6].vcs_position.y = -1.5F;                                    // Left trailer edge 
      det_props[6].f_double_bounce = false;
      det_props[6].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
      det_props[6].f_water_spray = false;

      // Set up eight detection such that it can be assigned a column/bin number
      raw_dets.detections[7].raw.sensor_id = 2;                                   // Center rear sensor
      raw_dets.detections[7].raw.range_rate = 0.3F - F360_EPSILON;                //  Any value such that |range_rate| < 0.3 
      det_props[7].vcs_position.x = -5.6F;                                    // -5.6 so it will be in the first x interval for easier testing. the limit are -(k_host_length + k_max_trailer_length) and -k_host_length
      det_props[7].vcs_position.y = 0.92F;                                    // Lateral position anywhere between -1.5 and 1.5 since k_max_trailer_width = 3.0
      det_props[7].f_double_bounce = false;
      det_props[7].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
      det_props[7].f_water_spray = false;

      // Set up ninth detection such that it can be assigned a column/bin number
      raw_dets.detections[8].raw.sensor_id = 1;                                   // Left rear sensor
      raw_dets.detections[8].raw.range_rate = 0.3F - F360_EPSILON;                //  Any value such that |range_rate| < 0.3 
      det_props[8].vcs_position.x = -5.6F;                                    // -5.6 so it will be in the first x interval for easier testing. the limit are -(k_host_length + k_max_trailer_length) and -k_host_length
      det_props[8].vcs_position.y = -1.4F;                                    // Lateral position anywhere between -1.5 and 1.5 since k_max_trailer_width = 3.0
      det_props[8].f_double_bounce = false;
      det_props[8].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_INVALID;
      det_props[8].f_water_spray = false;
    }
};

/** \purpose  
 * Verify that function Process_Input() works as intended if all conditions are meet
 * \req
 * NA
 */
TEST(f360_pvtrailer_width_estimation_process_input, Test_Assign_Dets_to_correct_bin)
{
   /** \precond
    * Use the first four detections with default set up. Host and detections has been initialized such that f_trailer_detection is true
    */
   raw_dets.number_of_valid_detections = 4U;

   /** \action
    * Run Process_Input() function
    */
   Process_Input(vehicle_data, raw_dets, det_props, sensors, pvtrailer_width);

   /** \result
    * Check that
    * - Four detections have updated their respective bins correctly
    * - No other bins have been updated (default test set up for detection_col[i] is one)
    */
   const uint16_t exp_first_dets_col_index = 7U;
   CHECK_EQUAL(0U, pvtrailer_width.dets_cnt_per_x_interval[0U][exp_first_dets_col_index]);

   const uint16_t exp_second_dets_col_index = 22U;
   CHECK_EQUAL(0U, pvtrailer_width.dets_cnt_per_x_interval[0U][exp_second_dets_col_index]);

   // The safe guard/ out of bounce scenario
   const uint16_t exp_third_dets_col_index = 37U;
   CHECK_EQUAL(0U, pvtrailer_width.dets_cnt_per_x_interval[0U][exp_third_dets_col_index]);

   const uint16_t exp_fourth_dets_col_index = 29U;
   CHECK_EQUAL(0U, pvtrailer_width.dets_cnt_per_x_interval[0U][exp_fourth_dets_col_index]);

   // No other bin should be updated
   for (uint16_t i = 0U; i < DETECTION_COLS; i++)
   {
      if (i != exp_first_dets_col_index &&
          i != exp_second_dets_col_index &&
          i != exp_third_dets_col_index &&
          i != exp_fourth_dets_col_index)
      {
         CHECK_EQUAL(pvtrailer_width.dets_cnt_per_x_interval[0][i], 0U);
      }
   }
}

/** \purpose
 * Check that detections with invalid properties do not update bins
 */
TEST(f360_pvtrailer_width_estimation_process_input, Test_Dont_Update_blocked_by_detection_props)
{
   /** \precond
    * The default detection settings passes the variable f_trailer_detection as true. To verify if the Process_Input()
    * setup is correct, assign each of the nine detections with one property such that f_trailer_detection becomes false.
    */
   raw_dets.detections[0].raw.range_rate = 0.3F + F360_EPSILON;         // Choose any positive value such that |range_rate| > tw_calibs_set_val.k_ZRRateGate
   raw_dets.detections[1].raw.range_rate = -(0.3F + F360_EPSILON);      // Choose any negative value such that |range_rate| > tw_calibs_set_val.k_ZRRateGate
   det_props[2].vcs_position.x = -5.0F;                                 // Choose any value larger than -tw_calibs.k_host_length
   det_props[3].vcs_position.x = -18.5F;                                // Choose any value smaller than -(tw_calibs.k_host_length + tw_calibs.k_max_trailer_length)
   det_props[4].vcs_position.y = 2.3F;                                  // Choose any positive value failing |1.5F| <= half_width
   det_props[5].vcs_position.y = -2.3F;                                 // Choose any negative value failing |1.5F| <= half_width
   det_props[6].f_double_bounce = true;                                 // Default false
   det_props[7].wheel_spin_type = F360_DETECTION_WHEELSPIN_TYPE_NEARBY; // Choose any enum except F360_DETECTION_WHEELSPIN_TYPE_INVALID
   det_props[8].f_water_spray = true;                                   // Default false

   /** \action
    * Run Process_Input() function
    */
   Process_Input(vehicle_data, raw_dets, det_props, sensors, pvtrailer_width);

   /** \result
    * Check that no bin has been updated
    */

   for (uint32_t i = 0U; i < X_INTERVALS_NUMBER; i++)
   {
      for (uint32_t j = 0U; j < DETECTION_COLS; j++)
      {
         CHECK_EQUAL(pvtrailer_width.dets_cnt_per_x_interval[i][j], 0U);
      }
   }
}

/** \purpose
 * Verify that nothing happens if detections comes from sensors invalid for trailer detection
 */
TEST(f360_pvtrailer_width_estimation_process_input, Test_Incorrect_sensor_mounting)
{
   /** \precond
    * To verify if the Process_Input() setup is correct, assign three detections with a sensor mounting property such
    * the if-check for mounting location never enters.
    */
   raw_dets.number_of_valid_detections = 3U;

   raw_dets.detections[0].raw.sensor_id = 5; // Right forward
   raw_dets.detections[1].raw.sensor_id = 6; // Left side
   raw_dets.detections[2].raw.sensor_id = 7; // RIght side

   /** \action
    * Run Process_Input() function
    */
   Process_Input(vehicle_data, raw_dets, det_props, sensors, pvtrailer_width);

   /** \result
    * Check that no bin has been updated
    */
   for (uint32_t i = 0U; i < X_INTERVALS_NUMBER; i++)
   {
      for (uint32_t j = 0U; j < DETECTION_COLS; j++)
      {
         CHECK_EQUAL(0U, pvtrailer_width.dets_cnt_per_x_interval[i][j]);
      }
   }
}

/** \purpose
 *  Check that nothing happens if no detections are valid to use
 */
TEST(f360_pvtrailer_width_estimation_process_input, Test_No_detections_ok_to_use)
{
   /** \precond
    * Assign the number of valid detections to use to zero.
    * Check that no bins are updated if the number of detetions okey to use are zero.
    */
   raw_dets.number_of_valid_detections = 0U;

   /** \action
    * Run Process_Input() function
    */
   Process_Input(vehicle_data, raw_dets, det_props, sensors, pvtrailer_width);

   /** \result
    * Check that no bin has been updated
    */
   for (uint32_t i = 0U; i < X_INTERVALS_NUMBER; i++)
   {
      for (uint32_t j = 0U; j < DETECTION_COLS; j++)
      {
         CHECK_EQUAL(pvtrailer_width.dets_cnt_per_x_interval[i][j], 0U);
      }
   }
}

/** @}*/

/** \defgroup  f360_pvtrailer_width_estimation_execute_estimate
 *  @{
 */

/** \brief
 * Check that the trailer width estimation is executing the estimation as expected
 */
TEST_GROUP(f360_pvtrailer_width_estimation_execute_estimate)
{
   F360_PVTrailer_Width_Data_T pvtrailer_width;
   const float32_t test_pass_th = 1e-6F;

   /** \setup
    * Clear the width structure and set some default values.
    */
   TEST_SETUP()
   {
      (void)memset(&pvtrailer_width, 0, sizeof(pvtrailer_width));
      pvtrailer_width.window_timer = 1800U;     // set window timer to a value that will execute the estimation by default
      pvtrailer_width.n_dets_per_area[0] = 30U; // set 30 detections in the first row by default
      
      for (uint32_t i = 0U; i < X_INTERVALS_NUMBER; i++)
      {
         pvtrailer_width.dets_in_left_side[i] = true;
         pvtrailer_width.dets_in_right_side[i] = true;
      }
   }
};

/** \purpose
 * Test that when the ovly valid x interval has a flat distribution (5 in all bins), the trailer width correctly estimated to the maximum width (4.5).
 */
TEST(f360_pvtrailer_width_estimation_execute_estimate, Flat_Distribution)
{
   /** \precond
    * Set up a detections bins first index, which is the only valid, with 5 detections in 30 bins out of 45
    * Set expected width to 4.5m
    */
   pvtrailer_width.total_number_of_dets = 25U;
   for (uint32_t i = 0U; i < 5U; i++)
   {
      pvtrailer_width.dets_cnt_per_x_interval[0U][i] = 5U;
   }
   const float32_t exp_trailer_width = 4.5F;
   
   /** \action
    * Call the Estimate() function
    */
   Estimate(pvtrailer_width);

   /** \result
    * Check that the expected width is estimated and confidence level is correct.
    */
   CHECK_TRUE(pvtrailer_width.f_estimation_done)
   DOUBLES_EQUAL(exp_trailer_width, pvtrailer_width.trailer_width, test_pass_th)
}

/** \purpose
 * Test that when the ovly valid x interval has a flat distribution (5 in middle bins), the trailer width correctly estimated to the maximum width.
 */
TEST(f360_pvtrailer_width_estimation_execute_estimate, Flat_Distribution_Middle_Bins)
{
   /** \precond
    * Set up a detections bins first index, which is the only valid, with 5 detections in 30 bins out of 45
    * Set expected width to 3m
    */
   const int32_t values_to_insert[DETECTION_COLS] = {0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0};
   for (uint32_t i = 0U; i < DETECTION_COLS; i++)
   {
      pvtrailer_width.dets_cnt_per_x_interval[0U][i] = values_to_insert[i];
      pvtrailer_width.total_number_of_dets += values_to_insert[i];
   }

   float32_t exp_trailer_width = 3.0F;

   /** \action
    * Call the Estimate() function
    */
   Estimate(pvtrailer_width);

   /** \result
    * Check that the expected width is estimated and confidence level is correct.
    */
   CHECK_TRUE(pvtrailer_width.f_estimation_done)
   DOUBLES_EQUAL(exp_trailer_width, pvtrailer_width.trailer_width, test_pass_th)
}

/** \purpose
 * Test that when the ovly valid x interval has a normal distribution, the width is correctly estimated.
 */
TEST(f360_pvtrailer_width_estimation_execute_estimate, Estimate_Det_Col_Flat)
{
   /** \precond
    * Set up a detections bins first index, which is the only valid, to have a normal distribution
    * Set expected width to 1.2m
    */
   const int32_t values_to_insert[DETECTION_COLS] = {0, 0, 1, 1, 1, 1, 1, 1, 1, 2, 5, 3, 22, 27, 57, 72, 119, 211, 260, 379, 411, 449, 498, 504, 465, 401, 325, 280, 179, 127, 88, 62, 27, 18, 4, 2, 2, 0, 1, 1, 1, 1, 1, 1, 0};
   for (uint32_t i = 0U; i < DETECTION_COLS; i++)
   {
      pvtrailer_width.dets_cnt_per_x_interval[0U][i] = values_to_insert[i];
   }

   float32_t exp_trailer_width = 1.2F;

   /** \action
    * Call the Estimate() function
    */
   Estimate(pvtrailer_width);

   /** \result
    * Check that the expected width is estimated and confidence level is correct.
    */
   CHECK_TRUE(pvtrailer_width.f_estimation_done)
   DOUBLES_EQUAL(exp_trailer_width, pvtrailer_width.trailer_width, test_pass_th)
}

/** \purpose
 * Test that when detection col array is sampled from a uniform random distribution, the width is correctly estimated.
 */
TEST(f360_pvtrailer_width_estimation_execute_estimate, Estimate_Det_Col_Uniform_Random)
{
   /** \precond
    * A detection col is set up with random numbers in each bin
    * Set expected width to 2m.
    */
   pvtrailer_width.total_number_of_dets = 30U;
   const int32_t values_to_insert[DETECTION_COLS] = {2687, 45, 3689, 3766, 555, 904, 2443, 3856, 2744, 267, 3625, 3863, 3505, 2020, 3380, 2486, 1227, 2718, 2669,
                                                 1696, 3116, 452, 88, 3695, 2362, 2682, 347, 1506, 3776, 2418, 1583, 412, 3712, 2983, 365, 818, 3292, 2512, 689, 1128, 3768, 2735, 1822, 3326, 1305};
   for (uint32_t i = 0U; i < DETECTION_COLS; i++)
   {
      pvtrailer_width.dets_cnt_per_x_interval[0U][i] = values_to_insert[i];
   }

   float32_t exp_trailer_width = 2.0F;

   /** \action
    * Call the Estimate() function
    */
   Estimate(pvtrailer_width);

   /** \result
    * Check that the expected width is estimated and confidence level is correct.
    */
   CHECK_TRUE(pvtrailer_width.f_estimation_done)
   DOUBLES_EQUAL(exp_trailer_width, pvtrailer_width.trailer_width, test_pass_th)
}

/** \purpose
 * Test that trailer width is updated when we the first x interval is not valid.
 */
TEST(f360_pvtrailer_width_estimation_execute_estimate, Only_Second_X_Interval_Is_Valid)
{
   /** \precond
    * The second interval x, which is the only valid, is set up with in each bin for a width of 2.4m
    * Set expected width to same value as before estimate().
    */
   const float32_t exp_trailer_width = 2.4F;
   pvtrailer_width.n_dets_per_area[0] = 0U;
   pvtrailer_width.n_dets_per_area[1] = 30U;
   pvtrailer_width.total_number_of_dets = 30U;
   
   const int32_t values_to_insert[DETECTION_COLS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5,
      5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0};

   for (uint32_t i = 0U; i < DETECTION_COLS; i++) // Fill the second x interval with values
   {
      pvtrailer_width.dets_cnt_per_x_interval[1U][i] = values_to_insert[i];
   }

   /** \action
    * Call the Estimate() function
    */
   Estimate(pvtrailer_width);

   /** \result
    * Check that the expected width is estimated and confidence level is correct.
    */
   CHECK_TRUE(pvtrailer_width.f_estimation_done)
   DOUBLES_EQUAL(exp_trailer_width, pvtrailer_width.trailer_width, test_pass_th)
}

/** \purpose
 * Test that we select the maximum width among multiple x intervals when we have more than one valid x intervals.
 */
TEST(f360_pvtrailer_width_estimation_execute_estimate, Multiple_X_Interval_Is_Valid_Select_Max)
{
   /** \precond
    * Fill the bins for the first and second  x intervals where will have estimated widths of 2.4m and 2.8m, respectivelly
    * Set expected width to same value as before estimate().
    */
   pvtrailer_width.n_dets_per_area[0] = 30U;
   pvtrailer_width.n_dets_per_area[1] = 30U;
   pvtrailer_width.total_number_of_dets = 60U;
   
   const int32_t values_to_insert_first_interval[DETECTION_COLS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, // 2.4m Width
                                                                5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0};
   const int32_t values_to_insert_second_interval[DETECTION_COLS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, // 2.8m Width
                                                                 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0};

   for (uint32_t i = 0U; i < DETECTION_COLS; i++)
   {
      pvtrailer_width.dets_cnt_per_x_interval[0U][i] = values_to_insert_first_interval[i];
      pvtrailer_width.dets_cnt_per_x_interval[1U][i] = values_to_insert_second_interval[i];
   }

   const float32_t exp_trailer_width = 2.8F;

   /** \action
    * Call the Estimate() function
    */
   Estimate(pvtrailer_width);

   /** \result
    * Check that the expected width is estimated and confidence level is correct.
    */
   CHECK_TRUE(pvtrailer_width.f_estimation_done)
   DOUBLES_EQUAL(exp_trailer_width, pvtrailer_width.trailer_width, test_pass_th)
}

/** \purpose
 * Test that we select the minimum possible width when we receive 0 detections.
 */
TEST(f360_pvtrailer_width_estimation_execute_estimate, Total_Number_Of_Dets_Is_Zero)
{
   /** \precond
    * Fill the bins for the first and second  x intervals where will have estimated widths of 2.4m and 2.8m, respectivelly
    * Set expected width to same value as before estimate().
    */
   const float32_t exp_trailer_width = 1.2F;

   /** \action
    * Call the Estimate() function
    */
   Estimate(pvtrailer_width);

   /** \result
    * Check that the expected width is estimated and confidence level is correct.
    */
   CHECK_TRUE(pvtrailer_width.f_estimation_done)
   DOUBLES_EQUAL(exp_trailer_width, pvtrailer_width.trailer_width, test_pass_th)
}

/** \purpose
 * Test that we select the minimum possible width when the proportion for each x interval to
 * the total number of detections is < 10%.
 */
TEST(f360_pvtrailer_width_estimation_execute_estimate, X_Interval_Number_Of_Dets_Is_Smaller_Than_Required)
{
   /** \precond
    * Assign only 1 detection to detection_col_set_val.
    * Set total number of dets to 11
    * Set 0 to the detections of each area apart from the first to have only 1
    */
   pvtrailer_width.n_dets_per_area[0] = 1U;
   pvtrailer_width.dets_cnt_per_x_interval[0][0] = 1U;
   pvtrailer_width.total_number_of_dets = 11U;

   const float32_t exp_trailer_width = 1.2F;

   /** \action
    * Call the Estimate() function
    */
   Estimate(pvtrailer_width);

   /** \result
    * Check that the expected width is estimated and confidence level is correct.
    */
   CHECK_TRUE(pvtrailer_width.f_estimation_done)
   DOUBLES_EQUAL(exp_trailer_width, pvtrailer_width.trailer_width, test_pass_th)
}

TEST(f360_pvtrailer_width_estimation_execute_estimate, No_Left_Detections)
{
   /** \precond
    * Fill the bins for the first x interval only for indexes corredponding on the right side of the host
    * Set the total number of dets as 11
    * Set the flags for the x intervals false regarding if they have dets on the left of the host
    */
   const float32_t exp_trailer_width = 1.2F;
   pvtrailer_width.n_dets_per_area[0] = 11U;
   pvtrailer_width.total_number_of_dets = 11U;

   const int32_t values_to_insert_first_interval[DETECTION_COLS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5};

   for (uint32_t i = 0U; i < DETECTION_COLS; i++)
   {
      pvtrailer_width.dets_cnt_per_x_interval[0U][i] = values_to_insert_first_interval[i];
   }

   for (uint32_t i = 0U; i < X_INTERVALS_NUMBER; i++)
   {
      pvtrailer_width.dets_in_left_side[i] = false;
   }

   /** \action
    * Call the Estimate() function
    */
   Estimate(pvtrailer_width);

   /** \result
    * Check that the expected width is estimated and confidence level is correct.
    */
   CHECK_TRUE(pvtrailer_width.f_estimation_done)
   DOUBLES_EQUAL(exp_trailer_width, pvtrailer_width.trailer_width, test_pass_th)
}

TEST(f360_pvtrailer_width_estimation_execute_estimate, No_Right_Detections)
{
   /** \precond
    * Fill the bins for the first x interval only for indexes corredponding on the left side of the host
    * Set the total number of dets as 11
    * Set the flags for the x intervals false regarding if they have dets on the right of the host
    */
   const float32_t exp_trailer_width = 1.2F;
   pvtrailer_width.n_dets_per_area[0] = 11U;
   pvtrailer_width.total_number_of_dets = 11U;

   const int32_t values_to_insert_first_interval[DETECTION_COLS] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0,
                                                                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
   
   for (uint32_t i = 0U; i < DETECTION_COLS; i++)
   {
      pvtrailer_width.dets_cnt_per_x_interval[0U][i] = values_to_insert_first_interval[i];
   }
   
   for (uint32_t i = 0U; i < X_INTERVALS_NUMBER; i++)
   {
      pvtrailer_width.dets_in_right_side[i] = false;
   }

   /** \action
    * Call the Estimate() functiona
    */
   Estimate(pvtrailer_width);

   /** \result
    * Check that the expected width is estimated and confidence level is correct.
    */
   CHECK_TRUE(pvtrailer_width.f_estimation_done)
   DOUBLES_EQUAL(exp_trailer_width, pvtrailer_width.trailer_width, test_pass_th)
}
/** @}*/
