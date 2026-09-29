/** \file
 * This file contains unit tests for content of f360_pvtrailer_angle_estimation.cpp file
 */

#include "CppUTest/TestHarness.h"
#include "f360_pvtrailer_angle_estimation.h"
#include "f360_constants.h"

using namespace f360_variant_A;

/** \defgroup  f360_pvtrailer_angle_estimation
 *  @{
 */

/** \brief
 * This test group checks that the PVTrailer_Estimate_Angle() function is working as expected.
 */
TEST_GROUP(f360_pvtrailer_angle_estimation)
{	
   F360_PVTrailer_Angle_Data_T pvtrailer_angle;
   F360_Host_T host;
   float32_t axle_length;
   const float32_t elasped_time_s = 0.05F;
   const float32_t test_pass_th = 1e-4F;

   /** \setup
    * Initialize PVTrailer angle data and host data with valid parameters to provide an estimate in the next iteration
    */
   TEST_SETUP()
   {
      pvtrailer_angle.HV_cnt = 19; // indicates that the next iteration will produce an estimate
      pvtrailer_angle.HV_start = false; // indicates that angle estimation is not current
      pvtrailer_angle.trailer_angle_rad = 0.087F; // approx. 5 deg
      pvtrailer_angle.trailer_angle_rate_rad = -0.040F; // some non-default reasonable value

      axle_length = 1.0F; // distance from hitch to trailer axle
      
      host.speed = 0.3; // note: threshold for estimation is set to 0.2 m/s
      host.yaw_rate_rad = -0.01F; // note: threshold for estimation is +- 0.02 rad/s
      host.vcs_sideslip = 0.026; // approx. 1.5 deg
      host.dist_rear_axle_to_vcs_m = 4.2F;
   }
};

/** \purpose  
 * Test that PVTrailer_Estimate_Angle() updates both HV_cnt and angle as expected when the trailer_axle_length is larger than 1m.
 * \req
 * NA
 */
TEST(f360_pvtrailer_angle_estimation, Test_HV_cnt_And_Angle_Updated_When_trailer_axle_length_Larger_Than_1m)
{
   /** \precond
    * Use the default test data from the test setup.
    * Set the trailer axle length to be slightly larger than 1m
    * Extract the trailer angle before function call (so that we can later check that is has increased/decreased with the correct value)
    */
	const float32_t trailer_angle_before = pvtrailer_angle.trailer_angle_rad;
   axle_length = 1.0F + F360_EPSILON;

   /** \action
    * Run PVTrailer_Estimate_Angle()
    */
   PVTrailer_Estimate_Angle(host, elasped_time_s, axle_length, pvtrailer_angle);

   /** \result
    * Check that the HV_cnt is increased to 20.
    * Check that HV_start has been set to true
    * Check that HV_angle is increased/decreased correctly
    * Check that trailer_angle_rate_rad has been correctly updated
    */
   
   CHECK_EQUAL(20, pvtrailer_angle.HV_cnt);
   CHECK_TRUE(pvtrailer_angle.HV_start);
   
   const float32_t exp_angle_increase = -0.001764F;
   const float32_t exp_angle = trailer_angle_before + exp_angle_increase;
   DOUBLES_EQUAL(exp_angle, pvtrailer_angle.trailer_angle_rad, test_pass_th);
   
   const float32_t exp_trailer_angle_rate_rad = (exp_angle - trailer_angle_before) / elasped_time_s;
   DOUBLES_EQUAL(exp_trailer_angle_rate_rad, pvtrailer_angle.trailer_angle_rate_rad, test_pass_th);
}


/** \purpose  
 * Test that PVTrailer_Estimate_Angle() updates both HV_cnt and angle as expected when the trailer_axle_length is larger than 1m.
 * \req
 * NA
 */
TEST(f360_pvtrailer_angle_estimation, Test_HV_cnt_And_Angle_Updated_When_trailer_axle_length_Smaller_Than_1m)
{
   /** \precond
    * Use the default test data from the test setup.
    * Set the trailer axle length to be slightly smaller than 1m
    * Extract the trailer angle before function call (so that we can later check that is has increased/decreased with the correct value)
    */
	const float32_t trailer_angle_before = pvtrailer_angle.trailer_angle_rad;
   axle_length = 1.0F - F360_EPSILON;

   /** \action
    * Run PVTrailer_Estimate_Angle()
    */
   PVTrailer_Estimate_Angle(host, elasped_time_s, axle_length, pvtrailer_angle);

   /** \result
    * Check that the HV_cnt is increased by 1.
    * Check that HV_start has been set to true
    * Check that trailer_angle_rad is increased/decreased correctly
    * Check that trailer_angle_rate_rad has been correctly updated
    */
   
   CHECK_EQUAL(20, pvtrailer_angle.HV_cnt);
   CHECK_TRUE(pvtrailer_angle.HV_start);
   
   const float32_t exp_angle_increase = -0.000816F;
   const float32_t exp_angle = trailer_angle_before + exp_angle_increase;
   DOUBLES_EQUAL(exp_angle, pvtrailer_angle.trailer_angle_rad, test_pass_th);
   
   const float32_t exp_trailer_angle_rate_rad = (exp_angle - trailer_angle_before) / elasped_time_s;
   DOUBLES_EQUAL(exp_trailer_angle_rate_rad, pvtrailer_angle.trailer_angle_rate_rad, test_pass_th);
}

/** \purpose  
 * Test that PVTrailer_Estimate_Angle() increases HV_cnt by 1 but does not update angle estimate when HV_cnt is not yet saturated.
 * \req
 * NA.
 */
TEST(f360_pvtrailer_angle_estimation, Test_HV_cnt_Updated_But_Angle_Is_Not_When_HV_cnt_Not_Reached_Max)
{
   /** \precond
    * Use the default test data from the test setup except for:
    *    Change HV_cnt such that it will not saturate at its maximum value.
    * Extract the trailer angle before function call (so that we can later check that is has not been updated)
    */
   pvtrailer_angle.HV_cnt = 18;
   const float32_t axle_length = 1.0F;
	const float32_t trailer_angle_before = pvtrailer_angle.trailer_angle_rad;

   /** \action
    * Run PVTrailer_Estimate_Angle()
    */
   PVTrailer_Estimate_Angle(host, elasped_time_s, axle_length, pvtrailer_angle);

   /** \result
    * Check that the HV_cnt is increased by 1.
    * Check that HV_start is not set to true
    * Check that trailer_angle_rad is not updated
    * Check that trailer_angle_rate_rad has been correctly set to 0
    */

   CHECK_EQUAL(19, pvtrailer_angle.HV_cnt);
   CHECK_FALSE(pvtrailer_angle.HV_start);
   DOUBLES_EQUAL(trailer_angle_before, pvtrailer_angle.trailer_angle_rad, test_pass_th);
   DOUBLES_EQUAL(0.0F, pvtrailer_angle.trailer_angle_rate_rad, test_pass_th);
}

/** \purpose  
 * Test that PVTrailer_Estimate_Angle() is resetting HV_cnt to 0 and don't update the angles when host vehicle speed is smaller than 2.0.
 * \req
 * NA.
 */
TEST(f360_pvtrailer_angle_estimation, Test_Not_Updated_blocked_by_Small_Speed)
{
   /** \precond
    * Use the default test data from the test setup except for:
    *    Change host speed such that it is slightly below ta_calibs.speed_threshold (but positive)
    * Extract the trailer angle before function call (so that we can later check that is has not been updated)
    */
   host.speed = 0.19F;
	const float32_t trailer_angle_before = pvtrailer_angle.trailer_angle_rad;

   /** \action
    * Run PVTrailer_Estimate_Angle()
    */
   PVTrailer_Estimate_Angle(host, elasped_time_s, axle_length, pvtrailer_angle);

   /** \result
    * Check that the HV_cnt is 0.
    * Check that HV_start is not set to true
    * Check that trailer_angle_rad is not updated
    */

   CHECK_EQUAL(0, pvtrailer_angle.HV_cnt);
   CHECK_FALSE(pvtrailer_angle.HV_start);
   DOUBLES_EQUAL(trailer_angle_before, pvtrailer_angle.trailer_angle_rad, test_pass_th);
}

/** \purpose  
 * Test that PVTrailer_Estimate_Angle() is resetting HV_cnt to 0 and don't update the angles when host vehicle yawrate is larger than 0.02
 * \req
 * NA.
 */
TEST(f360_pvtrailer_angle_estimation, Test_Not_Updated_blocked_by_Large_Yaw_Rate)
{
   /** \precond
    * Use the default test data from the test setup except for:
    *    Change host yaw rate such that it is slightly above 0.02
    * Extract the value of trailer_angle_rad before function call (so that we can later check that is has not been updated)
    */
   host.yaw_rate_rad = 0.021F;
	const float32_t trailer_angle_before = pvtrailer_angle.trailer_angle_rad;

   /** \action
    * Run PVTrailer_Estimate_Angle()
    */
   PVTrailer_Estimate_Angle(host, elasped_time_s, axle_length, pvtrailer_angle);

   /** \result
    * Check that the HV_cnt is 0.
    * Check that HV_start is not set to true
    * Check that trailer_angle_rad is not updated
    */

   CHECK_EQUAL(0, pvtrailer_angle.HV_cnt);
   CHECK_FALSE(pvtrailer_angle.HV_start);
   DOUBLES_EQUAL(trailer_angle_before, pvtrailer_angle.trailer_angle_rad, test_pass_th);
}

/** \purpose  
 * Test that PVTrailer_Estimate_Angle() is clearing data when host speed is negative
 * \req
 * NA.
 */
TEST(f360_pvtrailer_angle_estimation, Test_Clear_When_Speed_Is_Negative)
{
   /** \precond
    * Use the default test data from the test setup except for:
    *    Change host speed such that it is negative
    */
   host.speed = -0.01F;
	
   /** \action
    * Run PVTrailer_Estimate_Angle()
    */
   PVTrailer_Estimate_Angle(host, elasped_time_s, axle_length, pvtrailer_angle);

   /** \result
    * Check that the HV_cnt is 0.
    * Check that HV_start is not set to true
    * Check that trailer_angle_rad is not updated
    */

   CHECK_EQUAL(0, pvtrailer_angle.HV_cnt);
   CHECK_FALSE(pvtrailer_angle.HV_start);
   DOUBLES_EQUAL(0, pvtrailer_angle.trailer_angle_rad, test_pass_th);
   DOUBLES_EQUAL(0, pvtrailer_angle.trailer_angle_rate_rad, test_pass_th);
}

/** @}*/
