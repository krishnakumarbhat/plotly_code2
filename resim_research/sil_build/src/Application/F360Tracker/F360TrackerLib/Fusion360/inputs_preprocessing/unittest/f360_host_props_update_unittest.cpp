/** \file
 * This file contains unit tests for content of f360_host_props_update.cpp file
 */

#include "f360_host_props_update.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_host_props_update
 *  @{
 */

/** \brief
 * Test group for Host_Props_Update. Tests in this test group verify that the host properties
 * are time updated correctly.
 */
TEST_GROUP(f360_host_props_update)
{
   // Declare common variables used within all tests in this test group.
   float32_t elapsed_time = 0.05F;
   F360_Host_T host = {};
   F360_Host_Props_T host_props = {};

   const float32_t test_pass_th = 1e-6F;

   /** \setup
    * Set up a default scenario where host is driving straight along wcs coordinate system x axis.
    */
   TEST_SETUP()
   {
      host.yaw_rate_rad = 0.0F;
      host.vcs_sideslip = 0.0F;
      host.vcs_speed = 2.0F;

      host_props.heading_angle = 0.0F;
      host_props.position_x = 10.0F;
      host_props.position_y = 50.0F;

   }
};

/** \purpose  
 * Test that host heading and delta pointing is updated correctly when host is driving straight along wcs x axis.
 * \req NA.
 */
TEST(f360_host_props_update, Host_Props_Update_Driving_Straight_Along_X_Heading_Update)
{
   /** \precond
    * Host properties have been set up such that host is driving straight along wcs x axis.
    */
   const float32_t expected_hdg = 0.0F;
   const float32_t expected_cos_heading = 1.0F;
   const float32_t expected_sin_heading = 0.0F;
   const float32_t expected_delta_pointing = 0.0F;
   const float32_t expected_cos_delta_pointing = 1.0F;
   const float32_t expected_sin_delta_pointing = 0.0F;

   /** \action
    * Call Host_Props_Update
    */
   Host_Props_Update(elapsed_time, host, host_props);

   /** \result
    * Verify that host heading did not change and that host delta pointing is zero.
    */
   DOUBLES_EQUAL_TEXT(expected_hdg, host_props.heading_angle, test_pass_th, "Host heading angle was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_cos_heading, host_props.cos_heading, test_pass_th, "Cosine of host heading angle was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_sin_heading, host_props.sin_heading, test_pass_th, "Sine of host heading angle was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_delta_pointing, host_props.delta_pointing, test_pass_th, "Host delta pointing angle was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_cos_delta_pointing, host_props.cos_delta_pointing, test_pass_th, "Cosine of host delta pointing angle was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_sin_delta_pointing, host_props.sin_delta_pointing, test_pass_th, "Sine of host delta pointing angle was not updated correctly.");
}


/** \purpose
 * Test that host wcs position and delta vcs position are updated correctly when host is driving straight along wcs x axis.
 * \req NA.
 */
TEST(f360_host_props_update, Host_Props_Update_Driving_Straight_Along_X_Position_Update)
{
   /** \precond
    * Host properties have been set up such that host is driving straight along wcs x axis with a speed of 2 m/s.
    */
   const float32_t expected_x_pos = 10.1F;
   const float32_t expected_y_pos = 50.0F;
   const float32_t expected_delta_vcs_pos_x = 0.1F;
   const float32_t expected_delta_vcs_pos_y = 0.0F;

   /** \action
    * Call Host_Props_Update
    */
   Host_Props_Update(elapsed_time, host, host_props);

   /** \result
    * Verify that host wcs position as well as delta vcs position have been updated as expected.
    */
   DOUBLES_EQUAL_TEXT(expected_x_pos, host_props.position_x, test_pass_th, "Host x position was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_y_pos, host_props.position_y, test_pass_th, "Host y position was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_delta_vcs_pos_x, host_props.delta_position_x, test_pass_th, "Host delta vcs x position was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_delta_vcs_pos_y, host_props.delta_position_y, test_pass_th, "Host delta vcs y position was not updated correctly.");
}

/** \purpose
 * Test that host heading and delta pointing is updated correctly when host is driving straight along wcs y axis.
 * \req NA.
 */
TEST(f360_host_props_update, Host_Props_Update_Driving_Straight_Along_Y_Heading_Update)
{
   /** \precond
    * Host properties have been set up such that host is driving straight in test group.
    * Set host heading to pi / 2.
    */
   host_props.heading_angle = F360_PI_2; 

   const float32_t expected_hdg = F360_PI_2;
   const float32_t expected_cos_heading = 0.0F;
   const float32_t expected_sin_heading = 1.0F;
   const float32_t expected_delta_pointing = 0.0F;
   const float32_t expected_cos_delta_pointing = 1.0F;
   const float32_t expected_sin_delta_pointing = 0.0F;

   /** \action
    * Call Host_Props_Update
    */
   Host_Props_Update(elapsed_time, host, host_props);

   /** \result
    * Verify that host heading did not change and that host delta pointing is zero.
    */
   DOUBLES_EQUAL_TEXT(expected_hdg, host_props.heading_angle, test_pass_th, "Host heading angle was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_cos_heading, host_props.cos_heading, test_pass_th, "Cosine of host heading angle was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_sin_heading, host_props.sin_heading, test_pass_th, "Sine of host heading angle was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_delta_pointing, host_props.delta_pointing, test_pass_th, "Host delta pointing angle was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_cos_delta_pointing, host_props.cos_delta_pointing, test_pass_th, "Cosine of host delta pointing angle was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_sin_delta_pointing, host_props.sin_delta_pointing, test_pass_th, "Sine of host delta pointing angle was not updated correctly.");
}


/** \purpose
 * Test that host wcs position and delta vcs position are updated correctly when host is driving straight along wcs y axis.
 * \req NA.
 */
TEST(f360_host_props_update, Host_Props_Update_Driving_Straight_Along_Y_Position_Update)
{
   /** \precond
    * Host properties have been set up such that host is driving straight in test group.
    * Set host heading to pi / 2.
    */
   host_props.heading_angle = F360_PI_2;

   const float32_t expected_x_pos = 10.0F;
   const float32_t expected_y_pos = 50.1F;
   const float32_t expected_delta_vcs_pos_x = 0.1F;
   const float32_t expected_delta_vcs_pos_y = 0.0F;

   /** \action
    * Call Host_Props_Update
    */
   Host_Props_Update(elapsed_time, host, host_props);

   /** \result
    * Verify that host wcs position and delta vcs position have been updated as expected.
    */
   DOUBLES_EQUAL_TEXT(expected_x_pos, host_props.position_x, test_pass_th, "Host x position was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_y_pos, host_props.position_y, test_pass_th, "Host y position was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_delta_vcs_pos_x, host_props.delta_position_x, test_pass_th, "Host delta vcs x position was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_delta_vcs_pos_y, host_props.delta_position_y, test_pass_th, "Host delta vcs y position was not updated correctly.");
}


/** \purpose
 * Test that host wcs position and delta vcs position are updated correctly when host has a slight sideslip.
 * \req NA.
 */
TEST(f360_host_props_update, Host_Props_Update_Non_Zero_Sideslip_Position_Update)
{
   /** \precond
    * Host properties have been set up such that host is driving straight along wcs x axis in test group.
    * Set host vcs sideslip to 5 degrees
    */
   host.vcs_sideslip = F360_DEG2RAD(5.0F);

   const float32_t expected_x_pos = 10.09961947F;
   const float32_t expected_y_pos = 50.00871557F;
   const float32_t expected_delta_vcs_pos_x = 0.09961947F;
   const float32_t expected_delta_vcs_pos_y = 0.00871557F;

   /** \action
    * Call Host_Props_Update
    */
   Host_Props_Update(elapsed_time, host, host_props);

   /** \result
   * Verify that host wcs position as well as delta vcs position have been updated as expected.
    */
   DOUBLES_EQUAL_TEXT(expected_x_pos, host_props.position_x, test_pass_th, "Host x position was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_y_pos, host_props.position_y, test_pass_th, "Host y position was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_delta_vcs_pos_x, host_props.delta_position_x, test_pass_th, "Host delta vcs x position was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_delta_vcs_pos_y, host_props.delta_position_y, test_pass_th, "Host delta vcs y position was not updated correctly.");
}

/** \purpose
 * Test that host heading and delta pointing is updated correctly when host is yawing.
 * \req NA.
 */
TEST(f360_host_props_update, Host_Props_Update_Yawing_Heading_Update)
{
   /** \precond
    * Host properties have been set up such that host is driving straight along wcs x axis with a speed of 2 m/s.
    * Set host yaw rate to 0.1.
    */
   host.yaw_rate_rad = 0.1F;

   const float32_t expected_hdg = 0.005F;
   const float32_t expected_cos_heading = 0.99998750F;
   const float32_t expected_sin_heading = 0.00499998F;
   const float32_t expected_delta_pointing = 0.005F;
   const float32_t expected_cos_delta_pointing = 0.99998750F;
   const float32_t expected_sin_delta_pointing = 0.00499998F;

   /** \action
    * Call Host_Props_Update
    */
   Host_Props_Update(elapsed_time, host, host_props);

   /** \result
    * Verify that host heading and delta pointing have been updated as expected.
    */
   DOUBLES_EQUAL_TEXT(expected_hdg, host_props.heading_angle, test_pass_th, "Host heading angle was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_cos_heading, host_props.cos_heading, test_pass_th, "Cosine of host heading angle was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_sin_heading, host_props.sin_heading, test_pass_th, "Sine of host heading angle was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_delta_pointing, host_props.delta_pointing, test_pass_th, "Host delta pointing angle was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_cos_delta_pointing, host_props.cos_delta_pointing, test_pass_th, "Cosine of host delta pointing angle was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_sin_delta_pointing, host_props.sin_delta_pointing, test_pass_th, "Sine of host delta pointing angle was not updated correctly.");
}

/** \purpose
 * Test that host wcs position and delta vcs position are updated correctly when host is yawing.
 * \req NA.
 */
TEST(f360_host_props_update, Host_Props_Update_Yawing_Position_Update)
{
   /** \precond
    * Host properties have been set up such that host is driving straight along wcs x axis with a speed of 2 m/s.
    * Set host yaw rate to 0.1.
    */
   host.yaw_rate_rad = 0.1F;

   const float32_t expected_x_pos = 10.09999958F;
   const float32_t expected_y_pos = 50.00025F;
   const float32_t expected_delta_vcs_pos_x = 0.09999958F;
   const float32_t expected_delta_vcs_pos_y = 0.00025F;

   /** \action
    * Call Host_Props_Update
    */
   Host_Props_Update(elapsed_time, host, host_props);

   /** \result
    * Verify that host wcs position as well as delta vcs position have been updated as expected.
    */
   DOUBLES_EQUAL_TEXT(expected_x_pos, host_props.position_x, test_pass_th, "Host x position was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_y_pos, host_props.position_y, test_pass_th, "Host y position was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_delta_vcs_pos_x, host_props.delta_position_x, test_pass_th, "Host delta vcs x position was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_delta_vcs_pos_y, host_props.delta_position_y, test_pass_th, "Host delta vcs y position was not updated correctly.");
}

/** \purpose
 * Test that host heading is normalized correctly when heading exceeds 180 degrees.
 * \req NA.
 */
TEST(f360_host_props_update, Host_Props_Update_Yawing_Heading_Normalizated)
{
   /** \precond
    * Set host heading angle to 180 degrees.
    * Set host yaw rate such that host delta pointing is 5 degrees (yaw rate * elapsed time = delta pointing).
    */
   host_props.heading_angle = F360_DEG2RAD(180.0F);
   host.yaw_rate_rad = 1.745329251994329F;

   const float32_t expected_hdg = F360_DEG2RAD(-175.0F);
   const float32_t expected_cos_heading = -0.99619470F;
   const float32_t expected_sin_heading = -0.08715574F;
   const float32_t expected_delta_pointing = 0.08726646F;
   const float32_t expected_cos_delta_pointing = 0.99619470F;
   const float32_t expected_sin_delta_pointing = 0.08715574F;

   /** \action
    * Call Host_Props_Update
    */
   Host_Props_Update(elapsed_time, host, host_props);

   /** \result
    * Verify that host heading and delta pointing have been updated as expected.
    */
   DOUBLES_EQUAL_TEXT(expected_hdg, host_props.heading_angle, test_pass_th, "Host heading angle was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_cos_heading, host_props.cos_heading, test_pass_th, "Cosine of host heading angle was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_sin_heading, host_props.sin_heading, test_pass_th, "Sine of host heading angle was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_delta_pointing, host_props.delta_pointing, test_pass_th, "Host delta pointing angle was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_cos_delta_pointing, host_props.cos_delta_pointing, test_pass_th, "Cosine of host delta pointing angle was not updated correctly.");
   DOUBLES_EQUAL_TEXT(expected_sin_delta_pointing, host_props.sin_delta_pointing, test_pass_th, "Sine of host delta pointing angle was not updated correctly.");
}
/** @}*/
