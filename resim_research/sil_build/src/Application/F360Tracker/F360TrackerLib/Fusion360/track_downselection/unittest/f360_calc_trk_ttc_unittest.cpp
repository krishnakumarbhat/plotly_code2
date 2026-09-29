/** \file
   These tests tests the function Calc_Trk_Ttc by placing a target vehicle in the vincinity of host and with a range of different velocities. 
   If the calculated TTCs (Time To Collision) are equal (within a threshold) to the expected value, the tests pass, otherwise fail.
*/

#include "f360_calc_trk_ttc.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>
#include <math.h>
#include "f360_math.h"

/** \defgroup  f360_calc_trk_ttc
 *  @{
 */
using namespace f360_variant_A;
/** \brief
*  Test group for Calc_Trk_Ttc
**/
TEST_GROUP(calc_trk_ttc)
{
   //setup
   F360_Calibrations_T calib = {};
   
   TEST_SETUP()
   {
      calib.k_calc_ttc_min_thresh_projected_velocity = 1.0F;
      calib.k_calc_ttc_max_thresh_projected_velocity = 1e+6F;
   }

   /** \teardown
   * Nothing to teardown in this test group
   **/
   TEST_TEARDOWN()
   {
      //mock.clear();
   }
};


/**
*\purpose  Test f360_calc_trk_ttc with target travelling towards host.
*\req    NA
*/
TEST(calc_trk_ttc, stopped_host_approaching_target)
{
   /** \step{1}
   * Set up target at VCS pos. long = -5, pos. lat = 5, vel. long = 5, vel. lat = -5
   * Set up host VCS speed = 0.
   **/

   // Inputs 
   F360_Object_Track_T object_track;
   object_track.vcs_position.x = -5.0F;
   object_track.vcs_position.y = 5.0F;
   object_track.vcs_velocity.longitudinal = 5.0F;
   object_track.vcs_velocity.lateral = -5.0F;

   F360_Host_T host;
   host.vcs_speed = 0.0F;
   host.vcs_sideslip = 0.0F;

   // Outputs
   float32_t ttc = 0.0F;
   
   // Expected
   float32_t ttc_exp = 1.0F;
   float32_t range = sqrtf(object_track.vcs_position.x*object_track.vcs_position.x + 
      object_track.vcs_position.y*object_track.vcs_position.y);

   float32_t threshold = 0.01F;

   /** \action
   *Call Calc_Trk_Ttc with specified input
   **/

   ttc = Calc_Trk_Ttc(object_track, host, F360_Cosf(host.vcs_sideslip), F360_Sinf(host.vcs_sideslip), calib, range);

   /** \result
   * Compare output to expected values
   **/

   DOUBLES_EQUAL_TEXT(ttc_exp, ttc, threshold, "TTC does not match expected values");
}


/**
*\purpose  Test f360_calc_trk_ttc with target travelling away from host.
*\req    NA
*/
TEST(calc_trk_ttc, stopped_host_target_goes_away)
{
   /** \step{1}
   * Set up target at VCS pos. long = -5, pos. lat = 5, vel. long = -5, vel. lat = 5
   * Set up host VCS speed = 0.
   **/

   // Inputs
   F360_Object_Track_T object_track;
   object_track.vcs_position.x = -5.0F;
   object_track.vcs_position.y = 5.0F;
   object_track.vcs_velocity.longitudinal = -5.0F;
   object_track.vcs_velocity.lateral = 5.0F;

   F360_Host_T host;
   host.vcs_speed = 0.0F;
   host.vcs_sideslip = 0.0F;

   // Outputs
   float32_t ttc = 0.0F;
   
   // Expected
   float32_t ttc_exp = -1.0F;
   float32_t range = sqrtf(object_track.vcs_position.x*object_track.vcs_position.x +
      object_track.vcs_position.y*object_track.vcs_position.y);

   float32_t threshold = 0.01F;

   /** \action
   *Call Calc_Trk_Ttc with specified input
   **/

   ttc = Calc_Trk_Ttc(object_track, host, F360_Cosf(host.vcs_sideslip), F360_Sinf(host.vcs_sideslip), calib, range);

   /** \result
   * Compare output to expected values
   **/

   DOUBLES_EQUAL_TEXT(ttc_exp, ttc, threshold, "TTC does not match expected values");
}


/**
*\purpose  Test f360_calc_trk_ttc with target travelling perpendicular to host i.e. velocity towards host is 0.
*\req    NA
*/
TEST(calc_trk_ttc, stopped_host_target_velocity_towards_host_is_0)
{
   /** \step{1}
   * Set up target at VCS pos. long = -5, pos. lat = 5, vel. long = -5, vel. lat = -5
   * Set up host VCS speed = 0.
   **/

   // Inputs
   F360_Object_Track_T object_track;
   object_track.vcs_position.x = -5.0F;
   object_track.vcs_position.y = 5.0F;
   object_track.vcs_velocity.longitudinal = -5.0F;
   object_track.vcs_velocity.lateral = -5.0F;

   F360_Host_T host;
   host.vcs_speed = 0.0F;
   host.vcs_sideslip = 0.0F;

   // Outputs
   float32_t ttc = 0.0F;
   
   // Expected
   float32_t range = sqrtf(object_track.vcs_position.x*object_track.vcs_position.x +
      object_track.vcs_position.y*object_track.vcs_position.y);
   float32_t ttc_exp = -range;   // Special case for abs(rel vel) < 1

   float32_t threshold = 0.01F;

   /** \action
   *Call Calc_Trk_Ttc with specified input
   **/

   ttc = Calc_Trk_Ttc(object_track, host, F360_Cosf(host.vcs_sideslip), F360_Sinf(host.vcs_sideslip), calib, range);

   /** \result
   * Compare output to expected values
   **/
   DOUBLES_EQUAL_TEXT(ttc_exp, ttc, threshold, "TTC does not match expected values");
}


/**
*\purpose  Test f360_calc_trk_ttc with target travelling towards host with low speed.
*\req    NA
*/
TEST(calc_trk_ttc, host_stopped_target_travels_towards_host_with_low_speed)
{
   /** \step{1}
   * Set up target at VCS pos. long = -5, pos. lat = 5, vel. long = 0.5, vel. lat = -0.5
   * Set up host VCS speed = 0.
   **/

   // Inputs
   F360_Object_Track_T object_track;
   object_track.vcs_position.x = -5.0F;
   object_track.vcs_position.y = 5.0F;
   object_track.vcs_velocity.longitudinal = 0.5F;
   object_track.vcs_velocity.lateral = -0.5F;

   F360_Host_T host;
   host.vcs_speed = 0.0F;
   host.vcs_sideslip = 0.0F;

   // Outputs
   float32_t ttc = 0.0F;
   
   // Expected
   float32_t range = sqrtf(object_track.vcs_position.x*object_track.vcs_position.x +
      object_track.vcs_position.y*object_track.vcs_position.y);
   float32_t ttc_exp = range / 1.0F;   // Special case for abs(rel vel) < 1 
   
   float32_t threshold = 0.01F;

   /** \action
   *Call Calc_Trk_Ttc with specified input
   **/

   ttc = Calc_Trk_Ttc(object_track, host, F360_Cosf(host.vcs_sideslip), F360_Sinf(host.vcs_sideslip), calib, range);

   /** \result
   * Compare output to expected values
   **/
   DOUBLES_EQUAL_TEXT(ttc_exp, ttc, threshold, "TTC does not match expected values");
}


/**
*\purpose  Test f360_calc_trk_ttc with range to target < 1m.
*\req    NA
*/
TEST(calc_trk_ttc, host_stopped_target_near_host_below_1m)
{
   /** \step{1}
   * Set up target at VCS pos. long = -0.05, pos. lat = 0.05, vel. long = 5, vel. lat = -5
   * Set up host VCS speed = 0.
   **/

   // Inputs
   F360_Object_Track_T object_track;
   object_track.vcs_position.x = -0.05F;
   object_track.vcs_position.y = 0.05F;
   object_track.vcs_velocity.longitudinal = 5.0F;
   object_track.vcs_velocity.lateral = -5.0F;

   F360_Host_T host;
   host.vcs_speed = 0.0F;
   host.vcs_sideslip = 0.0F;

   // Outputs
   float32_t ttc = 0.0F;
      // Expected
   float32_t range = 0.1F;   // Special case for range < 1m
   float32_t ttc_exp = 0.02F;

   float32_t threshold = 0.01F;

   /** \action
   *Call Calc_Trk_Ttc with specified input
   **/

   ttc = Calc_Trk_Ttc(object_track, host, F360_Cosf(host.vcs_sideslip), F360_Sinf(host.vcs_sideslip), calib, range);

   /** \result
   * Compare output to expected values
   **/

   DOUBLES_EQUAL_TEXT(ttc_exp, ttc, threshold, "TTC does not match expected values");
}

/**
*\purpose Test calc_trk_ttc with range to target = 0m.
*\req    NA
*/
TEST(calc_trk_ttc, host_stopped_target_near_host_equal_0m)
{
   /** \step{1}
   * Set up target at VCS pos. long = -0.05, pos. lat = 0.05, vel. long = 5, vel. lat = -5
   * Set up host VCS speed = 0.
   **/

   // Inputs
   F360_Object_Track_T object_track;
   object_track.vcs_position.x = -0.05F;
   object_track.vcs_position.y = 0.05F;
   object_track.vcs_velocity.longitudinal = 5.0F;
   object_track.vcs_velocity.lateral = -5.0F;

   F360_Host_T host;
   host.vcs_speed = 0.0F;
   host.vcs_sideslip = 0.0F;

   // Outputs
   float32_t ttc = 0.0F;
   // Expected
   float32_t range = 0.0F;   // Special case for range = 0m
   float32_t ttc_exp = 0.0F;

   float32_t threshold = 0.000001F;

   /** \action
   *Call Calc_Trk_Ttc with specified input
   **/

   ttc = Calc_Trk_Ttc(object_track, host, F360_Cosf(host.vcs_sideslip), F360_Sinf(host.vcs_sideslip), calib, range);

   /** \result
   * Compare output to expected values
   **/

   DOUBLES_EQUAL_TEXT(ttc_exp, ttc, threshold, "TTC does not match expected values");
}


/**
*\purpose test calc_trk_ttc with proj_vel = 0 and calib.k_calc_ttc_min_thresh_projected_velocity = 0
*\req    NA
*/
TEST(calc_trk_ttc, result_from_F360_Matmul_MxN_NxP_is_zero_and_calib_param_is_zero)
{
   /** \step{1}
   * Set up target at VCS pos. long = -0.0, pos. lat = 0.0, vel. long = 0, vel. lat = 0
   * Set up host VCS speed = 0.
   **/

   // Inputs
   F360_Object_Track_T object_track;
   object_track.vcs_position.x = 0.0F;
   object_track.vcs_position.y = 0.0F;
   object_track.vcs_velocity.longitudinal = 0.0F;
   object_track.vcs_velocity.lateral = 0.0F;
   calib.k_calc_ttc_min_thresh_projected_velocity = 0.0F;
  
   F360_Host_T host;
   host.vcs_speed = 0.0F;
   host.vcs_sideslip = 0.0F;

   // Outputs
   float32_t ttc = 0.0F;
   // Expected
   float32_t range = 10.0F;
   float32_t ttc_exp = -84033613.0F;

   float32_t threshold = 0.000001F;

   /** \action
   *Call Calc_Trk_Ttc with specified input
   **/

   ttc = Calc_Trk_Ttc(object_track, host, F360_Cosf(host.vcs_sideslip), F360_Sinf(host.vcs_sideslip), calib, range);

   /** \result
   * Compare output to expected values
   **/

   DOUBLES_EQUAL_TEXT(ttc_exp, ttc, threshold, "TTC does not match expected values");
}


/**
*\purpose test calc_trk_ttc with proj_vel is higher than zero  and calib.k_calc_ttc_min_thresh_projected_velocity = 0
*\req    NA
*/
TEST(calc_trk_ttc, result_from_F360_Matmul_MxN_NxP_is_non_zero_but_calib_param_is_zero)
{
   /** \step{1}
   * Set up target at VCS pos. long = -0.0, pos. lat = 0.0, vel. long = 0, vel. lat = 0
   * Set up host VCS speed = 0.
   **/

   // Inputs
   F360_Object_Track_T object_track;
   object_track.vcs_position.x = 1.0F;
   object_track.vcs_position.y = 1.0F;
   object_track.vcs_velocity.longitudinal = 1.0F;
   object_track.vcs_velocity.lateral = 1.0F;
   calib.k_calc_ttc_min_thresh_projected_velocity = 0.0F;

   F360_Host_T host;
   host.vcs_speed = 0.0F;
   host.vcs_sideslip = 0.0F;

   // Outputs
   float32_t ttc = 0.0F;
   // Expected
   float32_t range = 10.0F;
   float32_t ttc_exp = -50.0F;

   float32_t threshold = 0.000001F;

   /** \action
   *Call Calc_Trk_Ttc with specified input
   **/

   ttc = Calc_Trk_Ttc(object_track, host, F360_Cosf(host.vcs_sideslip), F360_Sinf(host.vcs_sideslip), calib, range);

   /** \result
   * Compare output to expected values
   **/

   DOUBLES_EQUAL_TEXT(ttc_exp, ttc, threshold, "TTC does not match expected values");
}
