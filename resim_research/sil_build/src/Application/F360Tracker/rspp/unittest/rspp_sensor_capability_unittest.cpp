/** \file
    This file contains unit tests for content of f360_sensor_capability.cpp file
*/

#include "rspp_sensor_capability.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>
#include <cmath>
#include <cstdio>

using namespace rspp_variant_A;

/** \defgroup f360_sensor_capability_RSPP_Get_Uncertainty_Of_Compensated_Range_Rate
 *  @{
 */

/** \brief
 * Tests for RSPP_Get_Uncertainty_Of_Compensated_Range_Rate function with various
 * azimuth cos/sin input combinations.
 */
TEST_GROUP(f360_sensor_capability_RSPP_Get_Uncertainty_Of_Compensated_Range_Rate)
{
   float32_t threshold = 0.001F;
   float32_t sens_vel[2] = {18.0F, 0.5F};
   float32_t var_det_rng_rate = 0.8F;
   float32_t var_det_az = 1.0F;
   float32_t cov_sens_vel[2][2] = {{1.0005e-05F, 2.60488e-06F}, {2.60488e-06F, 0.00138266F}};
   float32_t var_comp_rng_rate;

   /** \setup
    * No setup required - using default initialized values.
    */
   TEST_SETUP()
   {
   }

   /** \teardown
    * No cleanup required for this test group.
    */
   TEST_TEARDOWN()
   {
   }
};
/** \purpose
 * Test RSPP_Get_Uncertainty_Of_Compensated_Range_Rate Function when both cos and sin of azimuth are inside allowed range
 * \req
 * NA
 */
TEST(f360_sensor_capability_RSPP_Get_Uncertainty_Of_Compensated_Range_Rate, Test_Cos_Sin_Az_Inside_Range)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Setting up cos_det_az to 0.5 and sin_det_az to 0.8
    **/
   float32_t cos_det_az = 0.5F;
   float32_t sin_det_az = 0.866025F;
   /** \action
    * Call RSPP_Get_Uncertainty_Of_Compensated_Range_Rate Function
    **/
   RSPP_Get_Uncertainty_Of_Compensated_Range_Rate(cos_det_az, sin_det_az, sens_vel, var_det_rng_rate, var_det_az, cov_sens_vel, var_comp_rng_rate);
   /** \result
    * Check the expected value for var_comp_rng_rate
    **/
   DOUBLES_EQUAL(var_comp_rng_rate, 236.0691F, threshold)
}

/** \purpose
 * Test RSPP_Get_Uncertainty_Of_Compensated_Range_Rate Function when cos or sin of azimuth is on boundary of allowed range
 * \req
 * NA
 */
TEST(f360_sensor_capability_RSPP_Get_Uncertainty_Of_Compensated_Range_Rate, Test_Cos_Sin_Az_On_Boundary_Of_Range)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Setting up cos_det_az to 1 and sin_det_az to 0
    **/
   float32_t cos_det_az = 1.0F;
   float32_t sin_det_az = 0.0F;
   /** \action
    * Call RSPP_Get_Uncertainty_Of_Compensated_Range_Rate Function
    **/
   RSPP_Get_Uncertainty_Of_Compensated_Range_Rate(cos_det_az, sin_det_az, sens_vel, var_det_rng_rate, var_det_az, cov_sens_vel, var_comp_rng_rate);
   /** \result
    * Check the expected value for var_comp_rng_rate
    **/
   DOUBLES_EQUAL(var_comp_rng_rate, 1.05001F, threshold)
}

/** \purpose
 * Test RSPP_Get_Uncertainty_Of_Compensated_Range_Rate Function when cos and sin of azimuth is beyond allowed range
 * \req
 * NA
 */
TEST(f360_sensor_capability_RSPP_Get_Uncertainty_Of_Compensated_Range_Rate, Test_Cos_Sin_Az_Out_Of_Range)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Setting up input to function and expected output
    **/
   /* Test with cos and sin out of range */
   float32_t cos_det_az = 2.0F;
   float32_t sin_det_az = 2.0F;

   /** \action
    * Call RSPP_Get_Uncertainty_Of_Compensated_Range_Rate Function
    **/
   RSPP_Get_Uncertainty_Of_Compensated_Range_Rate(cos_det_az, sin_det_az, sens_vel, var_det_rng_rate, var_det_az, cov_sens_vel, var_comp_rng_rate);
   /** \result
    * Check the expected value for var_comp_rng_rate
    **/
   DOUBLES_EQUAL(var_comp_rng_rate, 1225.806F, threshold)
}
/** @}*/

/** \defgroup  f360_sensor_capability_RSPP_Compute_Raw_Detection_Uncertainty
 *  @{
 */

/** \brief
 *  This test group tests the functionality of the function RSPP_Compute_Raw_Detection_Uncertainty()
 */
TEST_GROUP(f360_sensor_capability_RSPP_Compute_Raw_Detection_Uncertainty)
{
   float32_t threshold = 0.001F;
   RSPP_Detection_T det = {};
   F360_Radar_Sensor_T sensor = {};

   /** \setup
    * Initialize calibrations, sensor FOV parameters, and detection azimuth.
    */
   TEST_SETUP()
   {
      sensor.constant.fov_min_az_rad[0] = -0.785398185F;
      sensor.constant.fov_max_az_rad[0] = 0.785398185F;
      sensor.refined.interior_fov[RSPP_DET_LOOK_ID_0] = -0.785398185F;
      sensor.refined.interior_fov[RSPP_DET_LOOK_ID_1] = 0.785398185F;
      sensor.refined.interior_fov[RSPP_DET_LOOK_ID_2] = -0.785398185F;
      sensor.refined.interior_fov[RSPP_DET_LOOK_ID_3] = 0.785398185F;
      det.raw.azimuth = -0.515454471F;
   }

   /** \teardown
    * No cleanup required for this test group.
    */
   TEST_TEARDOWN()
   {
   }
};

/** \purpose
 * Test RSPP_Compute_Raw_Detection_Uncertainty function with MRR360 radar type.
 * \req
 * NA
 * \req
 * NA
 */
TEST(f360_sensor_capability_RSPP_Compute_Raw_Detection_Uncertainty, Test_RSPP_Compute_Raw_Detection_Uncertainty)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Setting up input to function and expected output
    **/
   float32_t az_var;
   sensor.constant.sensor_type = RSPP_SENSOR_TYPE_MRR360_RADAR;
   sensor.variable.look_id = RSPP_DET_LOOK_ID_0;

   /** \action
    * Call RSPP_Compute_Raw_Detection_Uncertainty Function with correct parameters
    **/
   RSPP_Compute_Raw_Detection_Uncertainty(
       det.raw.azimuth,
       -0.785398185F, // fov_min_az_rad
       0.785398185F,  // fov_max_az_rad
       sensor.refined.interior_fov,
       sensor.variable.look_id,
       sensor.constant.sensor_type,
       az_var);
   /** \result
    * Check the expected value for rng_var and az_var
    **/
   DOUBLES_EQUAL_TEXT(az_var, 0.000304617F, threshold, "Azimuth differs from expected value")
}

/** \purpose
 * Test RSPP_Compute_Raw_Detection_Uncertainty Function with MRR3 radar type
 * \req
 * NA
 */
TEST(f360_sensor_capability_RSPP_Compute_Raw_Detection_Uncertainty, Test_RSPP_Compute_Raw_Detection_Uncertainty_2)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Setting up input to function and expected output
    **/
   float32_t az_var;
   sensor.constant.sensor_type = RSPP_SENSOR_TYPE_MRR3_RADAR;
   sensor.variable.look_id = RSPP_DET_LOOK_ID_2;

   /** \action
    * Call RSPP_Compute_Raw_Detection_Uncertainty Function with correct parameters
    **/
   RSPP_Compute_Raw_Detection_Uncertainty(
       det.raw.azimuth,
       -0.785398185F, // fov_min_az_rad
       0.785398185F,  // fov_max_az_rad
       sensor.refined.interior_fov,
       sensor.variable.look_id,
       sensor.constant.sensor_type,
       az_var);
   /** \result
    * Check the expected value for rng_var and az_var
    **/
   DOUBLES_EQUAL_TEXT(az_var, 7.61544e-05F, threshold, "Azimuth differs from expected value")
}
/** @}*/

/** \defgroup f360_sensor_capability_RSPP_Get_Host_Velocity_Uncertainty
 *  @{
 */

/** \brief
 *  This test group tests the functionality of the function RSPP_Get_Host_Velocity_Uncertainty()
 */
TEST_GROUP(f360_sensor_capability_RSPP_Get_Host_Velocity_Uncertainty)
{
   float32_t threshold = 0.001F;
   RSPP_Host_T host = {};
   float32_t host_speed_var = {};
   float32_t host_yaw_rate_var = 3.0F;
   float32_t translation_vec[2] = {20.0F, 1.0F};
   float32_t velocity_cov[2][2] = {};

   /** \setup
    * Initialize host yaw rate, rear axle distance, and cornering compliance.
    */
   TEST_SETUP()
   {
      host.yaw_rate_rad = -0.00629700907F;
      host.dist_rear_axle_to_vcs_m = 3.45000005F;
      host.rear_cornering_compliance = 0.00529999984F;
   }

   /** \teardown
    * No cleanup required for this test group.
    */
   TEST_TEARDOWN()
   {
   }
};
/** \purpose
 * Test RSPP_Get_Host_Velocity_Uncertainty function
 * \req
 * NA
 */
TEST(f360_sensor_capability_RSPP_Get_Host_Velocity_Uncertainty, Test_with_Forward_speed)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Setting up input to function and expected output
    **/
   host_speed_var = 40.0F;
   host.speed = 34.3173103F;
   /** \action
    * Call RSPP_Get_Host_Velocity_Uncertainty function
    **/
   RSPP_Get_Host_Velocity_Uncertainty(host, host_speed_var, host_yaw_rate_var, translation_vec, velocity_cov);

   /** \result
    * Check the expected value for velocity_cov
    **/
   DOUBLES_EQUAL(velocity_cov[0][0], 42.9571F, threshold)
   DOUBLES_EQUAL(velocity_cov[0][1], -51.16426F, threshold)
   DOUBLES_EQUAL(velocity_cov[1][0], -51.16426F, threshold)
   DOUBLES_EQUAL(velocity_cov[1][1], 888.3797F, threshold)
}

/** \purpose
 * Test RSPP_Get_Host_Velocity_Uncertainty function
 * \req
 * NA
 */
TEST(f360_sensor_capability_RSPP_Get_Host_Velocity_Uncertainty, Test_with_backward_speed)
{
   /** \step{1}
    * Execute test and verify result.
    */

   /** \precond
    * Setting up the Host speed to negative value
    **/
   host_speed_var = 10.0F;
   host.speed = -34.3173103F;
   /** \action
    * Call RSPP_Get_Host_Velocity_Uncertainty function
    **/
   RSPP_Get_Host_Velocity_Uncertainty(host, host_speed_var, host_yaw_rate_var, translation_vec, velocity_cov);

   /** \result
    * Check the expected value for velocity_cov
    **/
   DOUBLES_EQUAL(velocity_cov[0][0], 13.043F, threshold)
   DOUBLES_EQUAL(velocity_cov[0][1], -52.01689F, threshold)
   DOUBLES_EQUAL(velocity_cov[1][0], -52.01689F, threshold)
   DOUBLES_EQUAL(velocity_cov[1][1], 888.3796F, threshold)
}
/** @}*/

/** \defgroup  f360_sensor_capability
 *  @{
 */

/** \brief
  *  This test group tests sensor_capability file
  */
TEST_GROUP(f360_sensor_capability)
{
    /** \setup
     * No setup required for this test group.
     */
    TEST_SETUP()
    {
      // No specific setup required
    }

    /** \teardown
     * No cleanup required for this test group.
     */
    TEST_TEARDOWN()
    {
      // No specific cleanup required
    }
};

/** \purpose
 * Describe purpose of test
 * \req
 * NA
 */
TEST(f360_sensor_capability, Test_RSPP_Compute_Raw_Host_Speed_Uncertainty)
{
   /** \step{1}
    * Testing that RSPP_Compute_Raw_Host_Speed_Uncertainty() behaves as expected
    **/

   /** \precond
    * Setting up input to function and expected output
    **/
   RSPP_Host_T host = {};
   const uint32_t num_host_speeds = 22U;
   const uint32_t num_host_yaw_rates = 11U;
   float32_t host_speed_arr[num_host_speeds];
   float32_t host_yaw_rate_arr[num_host_yaw_rates];
   float32_t result_speed_var[num_host_speeds][num_host_yaw_rates];
   float32_t current_speed_var;
   uint32_t host_speed_ind, host_yaw_rate_ind, ind;
   uint32_t comp_host_speed_ind, comp_host_yaw_rate_ind;

   /* Defining host speed correction factor as it is used in RSPP_Compute_Raw_Host_Speed_Uncertainty  */
   host.speed_correction_factor = 1.0F;

   for (host_speed_ind = 0U; host_speed_ind < num_host_speeds; host_speed_ind++)
   {
      // Host speeds between -30kph to 400kph
      host_speed_arr[host_speed_ind] = (20.0F * static_cast<float>(host_speed_ind) - 30.0F) / 3.6F; // [m/s]
   }
   for (host_yaw_rate_ind = 0U; host_yaw_rate_ind < num_host_yaw_rates; host_yaw_rate_ind++)
   {
      // Host yaw rates between -25deg/s to 25deg/s
      host_yaw_rate_arr[host_yaw_rate_ind] = (5.0F * static_cast<float>(host_yaw_rate_ind) - 25.0F) * 3.14F / 180.0F; // [rad/s]
   }

   char textFailure[200];

   /** \action
    * Call the function
    **/
   for (host_speed_ind = 0U; host_speed_ind < num_host_speeds; host_speed_ind++)
   {
      host.speed = host_speed_arr[host_speed_ind];
      for (host_yaw_rate_ind = 0U; host_yaw_rate_ind < num_host_yaw_rates; host_yaw_rate_ind++)
      {
         host.yaw_rate_rad = host_yaw_rate_arr[host_yaw_rate_ind];

         current_speed_var = RSPP_Compute_Raw_Host_Speed_Uncertainty(host, 70.0F);
         result_speed_var[host_speed_ind][host_yaw_rate_ind] = current_speed_var;
      }
   }

   /** \result
    * Check that output corresponds to following two criteria:
    * 1) speed and yaw rate variance is always >= 0
    * 2) speed and yaw rate variance is increasing (or same but not decreasing)
    *    with abs(speed) and abs(yaw rate)
    **/
   for (host_speed_ind = 0U; host_speed_ind < num_host_speeds; host_speed_ind++)
   {
      for (host_yaw_rate_ind = 0U; host_yaw_rate_ind < num_host_yaw_rates; host_yaw_rate_ind++)
      {
         // Check that variance is >= 0
         (void)sprintf(textFailure, "Speed variance on index host_speed=%d, yaw_rate=%d is less than 0: \n", host_speed_ind, host_yaw_rate_ind);
         CHECK_TEXT(result_speed_var[host_speed_ind][host_yaw_rate_ind] >= 0.0F, textFailure);

         // Check variance is larger or the same if abs(speed) or abs(yaw_rate) is larger. Compare all outputs to each other
         ind = host_speed_ind * num_host_yaw_rates + host_yaw_rate_ind;
         for (uint32_t comp_ind = ind + 1U; comp_ind < num_host_yaw_rates * num_host_speeds; comp_ind++)
         {
            comp_host_speed_ind = static_cast<uint32_t>(static_cast<float>(comp_ind) / static_cast<float>(num_host_yaw_rates));
            comp_host_yaw_rate_ind = comp_ind - comp_host_speed_ind * num_host_yaw_rates;

            if ((fabsf(host_speed_arr[host_speed_ind]) >= fabsf(host_speed_arr[comp_host_speed_ind])) &&
                (fabsf(host_yaw_rate_arr[host_yaw_rate_ind]) >= fabsf(host_yaw_rate_arr[comp_host_yaw_rate_ind])))
            {
               // If abs(speed) and abs(yaw rate) are both larger for ind then we expect variance of ind to be larger or equal
               (void)sprintf(textFailure, "Speed variance on index host_speed=%d, yaw_rate=%d is not larger than variance on index host_speed=%d, yaw_rate=%d despite that abs(speed) and abs(yaw rate) are larger: \n", host_speed_ind, host_yaw_rate_ind, comp_host_speed_ind, comp_host_yaw_rate_ind);
               CHECK_TEXT(result_speed_var[host_speed_ind][host_yaw_rate_ind] >= result_speed_var[comp_host_speed_ind][comp_host_yaw_rate_ind], textFailure);
            }
            else if ((fabsf(host_speed_arr[comp_host_speed_ind]) >= fabsf(host_speed_arr[host_speed_ind])) &&
                     (fabsf(host_yaw_rate_arr[comp_host_yaw_rate_ind]) >= fabsf(host_yaw_rate_arr[host_yaw_rate_ind])))
            {
               // If abs(speed) and abs(yaw rate) are both larger for comp_ind then we expect variance of comp_ind to be larger or equal
               (void)sprintf(textFailure, "Speed variance on index host_speed=%d, yaw_rate=%d is not larger than variance on index host_speed=%d, yaw_rate=%d despite that abs(speed) and abs(yaw rate) are larger: \n", host_speed_ind, host_yaw_rate_ind, comp_host_speed_ind, comp_host_yaw_rate_ind);
               CHECK_TEXT(result_speed_var[comp_host_speed_ind][comp_host_yaw_rate_ind] >= result_speed_var[host_speed_ind][host_yaw_rate_ind], textFailure);
            }
            else
            {
               // Output is not easy to predict since it depends on the implementation and
               // tuning of function which could be changed. Therefore no test for this case
            }
         }
      }
   }
}

/** @}*/
