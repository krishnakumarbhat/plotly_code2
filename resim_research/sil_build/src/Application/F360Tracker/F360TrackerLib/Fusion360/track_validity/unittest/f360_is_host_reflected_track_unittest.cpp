/** \file
* This file contains unit tests for content of f360_is_host_reflected_track.cpp
*/

#include "f360_is_host_reflected_track.h"

#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include <cfloat>

#include "f360_math.h"

using namespace f360_variant_A;
/** \defgroup  f360_is_host_reflected_track
 *  @{
 */

/** \brief
* Test groups contains common data for test cases related to testing of function Is_Host_Reflected_Track()
* Data is set up in such a way that expectation is that object should be flagged as a host mirror.
* Data is tweaked in individual test cases.
**/

TEST_GROUP(f360_is_host_reflected_track)
{
   /** \setup
   * Initialize calibrations
   * Set up data so that object matches a host mirror object with SEP id 1.
   * Host speed is set to low to generate fixed thresholds equal to calibration parameters
   * Set SEP with id 1 to valid on right hand side of host
   **/

   F360_Host_T host = {};
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_Calibrations_T calibs = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   int32_t obj_idx;
   F360_Trailer_Estimator_Output_T trailer_data = {};


   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      host.speed = calibs.k_host_refl_lowspeed_host_speed_th - 0.1F;
      obj_idx = 1;

      object_tracks[obj_idx].speed = 0.0F;
      object_tracks[obj_idx].reference_point = F360_REFERENCE_POINT_CENTER;
      object_tracks[obj_idx].vcs_position.x = -2.0F;
      object_tracks[obj_idx].vcs_position.y = 6.0F;
      object_tracks[obj_idx].bbox.Set_Center(object_tracks[obj_idx].vcs_position);
      object_tracks[obj_idx].vcs_heading = Angle{ 0.0F };
      object_tracks[obj_idx].hdg_ptng_disagmt = 0.0F;
      object_tracks[obj_idx].bbox.Set_Orientation(Angle{object_tracks[obj_idx].vcs_heading + object_tracks[obj_idx].hdg_ptng_disagmt});
      object_tracks[obj_idx].bbox.Set_Length(5.0F);
      object_tracks[obj_idx].bbox.Set_Width(2.0F);

      object_tracks[obj_idx].speed = host.speed;
      object_tracks[obj_idx].behind_sep_id = 1U;

      sep[0].lower_limit = -25.0F;
      sep[0].upper_limit = 25.0F;
      sep[0].p0 = 0.5F * object_tracks[obj_idx].vcs_position.y;
      sep[0].p1 = 0.0F;
      sep[0].p2 = 0.0F;
      sep[0].status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      sep[0].poly_type = F360_STATIC_ENV_POLY_TYPE_LSC;

      sensors[0].variable.is_valid = true;
      sensors[0].constant.mounting_position.vcs_position.longitudinal = 0.0F;
      sensors[0].constant.mounting_position.vcs_position.lateral = 1.0F;
      sensors[1].variable.is_valid = true;
      sensors[1].constant.mounting_position.vcs_position.longitudinal = 0.0F;
      sensors[1].constant.mounting_position.vcs_position.lateral = -1.0F;
      sensors[2].variable.is_valid = true;
      sensors[2].constant.mounting_position.vcs_position.longitudinal = -4.0F;
      sensors[2].constant.mounting_position.vcs_position.lateral = 1.0F;
      sensors[3].variable.is_valid = true;
      sensors[3].constant.mounting_position.vcs_position.longitudinal = -4.0F;
      sensors[3].constant.mounting_position.vcs_position.lateral = -1.0F;

      host.f_trailer_presence_hardware = false;
      trailer_data.trailer_length[0] = 0.0F;
      trailer_data.trailer_length[1] = 0.0F;
   }
};

/**
*\purpose  Purpose of this test is to verify that object is not flagged as a host mirror
*          since object speed doesn't match host speed.
*          Host has no trailer attached.
*\req    NA
*/
TEST(f360_is_host_reflected_track, Is_Host_Reflected_Track__Object_Speed_Not_Matching_Host)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - Object heading and pointing set to 0 along with cos/sin
    * - Object position is adjacent to host behind SEP id 1
    * - Object is flagged as behind SEP id 1
    * - SEP with id 1 is valid adjacent to host on right side
    * Set object speed to zero
    * Host has no trailer attached.
    */
   object_tracks[obj_idx].speed = 0.0F;

   /** \action
   * Call Is_Host_Reflected_Track
   **/
   bool f_host_mirror =  Is_Host_Reflected_Track(
      tracker_info,
      host,
      sep,
      calibs,
      sensors,
      obj_idx,
      trailer_data,
      object_tracks);

   /** \result
   * Track should not be marked as mirror
   **/
   CHECK_FALSE(f_host_mirror);
}

/**
*\purpose  Purpose of this test is to verify that object is not flagged as a host mirror
*          since object isn't flagged as behind any SEP
*          Host has no trailer attached.
*\req    NA
*/
TEST(f360_is_host_reflected_track, Is_Host_Reflected_Track__Object_Not_Flagged_As_Behind_SEP)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - Object heading and pointing set to 0 along with cos/sin
    * - Object position is adjacent to host behind SEP id 1
    * - Object is flagged as behind SEP id 1
    * - SEP with id 1 is valid adjacent to host on right side
    * Set SEP as invalid
    * Host has no trailer attached.
    */
   sep[0].status = F360_STATIC_ENV_POLY_STATUS_INVALID;

   /** \action
   * Call Is_Host_Reflected_Track
   **/
   bool f_host_mirror =  Is_Host_Reflected_Track(
      tracker_info,
      host,
      sep,
      calibs,
      sensors,
      obj_idx,
      trailer_data,
      object_tracks);

   /** \result
   * Track should not be marked as mirror
   **/
   CHECK_FALSE(f_host_mirror);
}

/**
*\purpose  Purpose of this test is to verify that object is not flagged as a host mirror
*          since object lateral position is too far away
*          Host has no trailer attached.
*\req    NA
*/
TEST(f360_is_host_reflected_track, Is_Host_Reflected_Track__Object_Too_Far_In_Lateral_Direction)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - Object heading and pointing set to 0 along with cos/sin
    * - Object position is adjacent to host behind SEP id 1
    * - Object is flagged as behind SEP id 1
    * - SEP with id 1 is valid adjacent to host on right side
    * Set lateral position far away from host
    * Host has no trailer attached.
    */
   object_tracks[obj_idx].vcs_position.y = 20.0F;
   object_tracks[obj_idx].bbox.Set_Center(object_tracks[obj_idx].vcs_position);

   /** \action
   * Call Is_Host_Reflected_Track
   **/
   bool f_host_mirror =  Is_Host_Reflected_Track(
      tracker_info,
      host,
      sep,
      calibs,
      sensors,
      obj_idx,
      trailer_data,
      object_tracks);

   /** \result
   * Track should not be marked as mirror
   **/
   CHECK_FALSE(f_host_mirror);
}

/**
*\purpose  Purpose of this test is to verify that object is flagged as a host mirror
*          since all conditions are fulfilled
*          Host has no trailer attached.
*\req    NA
*/
TEST(f360_is_host_reflected_track, Is_Host_Reflected_Track__Object_Is_Host_Mirror)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - Object heading and pointing set to 0 along with cos/sin
    * - Object position is adjacent to host behind SEP id 1
    * - Object speed is equal to host speed
    * - Object is flaged as moving
    * - Object is flagged as behind SEP id 1
    * - SEP with id 1 is valid adjacent to host on right side
    * Host has no trailer attached.
    */
   host.speed = 3.0F;

   object_tracks[obj_idx].speed = 3.0F;
   object_tracks[obj_idx].f_moving = true;

   /** \action
   * Call Is_Host_Reflected_Track
   **/
   bool f_host_mirror =  Is_Host_Reflected_Track(
      tracker_info,
      host,
      sep,
      calibs,
      sensors,
      obj_idx,
      trailer_data,
      object_tracks);

   /** \result
   * Track should be marked as mirror
   **/
   CHECK_TRUE(f_host_mirror);
}

/**
*\purpose  Purpose of this test is to verify that the object is flagged as a host mirror and have assigned correct mirror probability
*          given that it is newly initialized and of high heading angle.
*          Host has no trailer attached.
*\req    NA
*/
TEST(f360_is_host_reflected_track, Is_Host_Reflected_Track__New_Initialized_And_High_Heading_Object_Is_Host_Mirror)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - Object heading is higher than threshold
    * - Object speed is equal to host speed
    * - Object is flaged as moving
    * - Object is initialized 0.5 s ago
    * - Object position is adjacent to host behind SEP id 1
    * - Object is flagged as behind SEP id 1
    * - SEP with id 1 is valid adjacent to host on right side
    * Host has no trailer attached.
    */
   host.speed = 3.0F;

   object_tracks[obj_idx].speed = 3.0F;
   object_tracks[obj_idx].f_moving = true;
   object_tracks[obj_idx].vcs_heading = Angle{ -2.98F };
   object_tracks[obj_idx].time_since_initialization = 0.5F;

   /** \action
   * Call Is_Host_Reflected_Track
   **/

   bool f_host_mirror = Is_Host_Reflected_Track(
      tracker_info,
      host,
      sep,
      calibs,
      sensors,
      obj_idx,
      trailer_data,
      object_tracks);

   /** \result
   * Track should be marked as mirror and its mirror_prob should be set to 0.9
   **/
   CHECK_TRUE(f_host_mirror);
   CHECK_EQUAL(object_tracks[obj_idx].mirror_prob, 0.9F);
}

/**
*\purpose  Purpose of this test is to verify that the object is flagged as a host mirror
*          given that it have small heading, speed diff with host higher than threshold and is initialized 1 s ago
*          Host has no trailer attached.
*\req    NA
*/
TEST(f360_is_host_reflected_track, Is_Host_Reflected_Track__Small_Heading_And_Big_Speed_Diff_Long_Lived)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - Object speed is higher than threshold
    * - Object heading is 0
    * - Object is initialized 1 s ago
    * - Object is flaged as moving
    * - Object position is adjacent to host behind SEP id 1
    * - Object is flagged as behind SEP id 1
    * - SEP with id 1 is valid adjacent to host on right side
    * Host has no trailer attached.
    */

   host.speed = 3.0F;

   object_tracks[obj_idx].speed = 7.0F;
   object_tracks[obj_idx].f_moving = true;
   object_tracks[obj_idx].time_since_initialization = 1.0F;

   bool f_host_mirror = Is_Host_Reflected_Track(
      tracker_info,
      host,
      sep,
      calibs,
      sensors,
      obj_idx,
      trailer_data,
      object_tracks);
   /** \action
   * Call Is_Host_Reflected_Track
   **/


   /** \result
   * Track should not be marked as mirror
   **/
   CHECK_FALSE(f_host_mirror);
}

/**
*\purpose  Purpose of this test is to verify that object is not flagged as a host mirror
*          because of suspected SEP is crossing host's path at a great distance.
*          Host has no trailer attached.
*\req    NA
*/
TEST(f360_is_host_reflected_track, Is_Host_Reflected_Track__Reflection_Far_Ahead_SEP_Crossing_non_ghost)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - Object heading and pointing set to 0 along with cos/sin
    * - Object position is adjacent to host behind SEP id 1
    * - Object is flagged as behind SEP id 1
    * - Object is flagged as moving
    * - SEP with id 1 is valid adjacent to host on right side
    * Host has no trailer attached.
    */

   sep[0].lower_limit = -17.77F;
   sep[0].upper_limit = 40.47F;
   sep[0].p0 = -5.90F;
   sep[0].p1 = -0.11F;
   sep[0].p2 = 0.001142F;

   object_tracks[obj_idx].speed = 4.75F;
   object_tracks[obj_idx].time_since_initialization = 1.0F;
   object_tracks[obj_idx].f_moving = true;
   object_tracks[obj_idx].reference_point = F360_REFERENCE_POINT_LEFT;
   object_tracks[obj_idx].vcs_position.x = 3.05F;
   object_tracks[obj_idx].vcs_position.y = -11.52F;
   object_tracks[obj_idx].bbox.Set_Center(object_tracks[obj_idx].vcs_position);
   object_tracks[obj_idx].vcs_heading = Angle{ -2.98F };
   object_tracks[obj_idx].hdg_ptng_disagmt = 0.0F;
   object_tracks[obj_idx].bbox.Set_Orientation(Angle{object_tracks[obj_idx].vcs_heading + object_tracks[obj_idx].hdg_ptng_disagmt});
   object_tracks[obj_idx].bbox.Set_Length(3.0F);
   object_tracks[obj_idx].bbox.Set_Width(1.0F);

   host.speed = 6.08F;

   /** \action
   * Call Is_Host_Reflected_Track
   **/
   bool f_host_mirror = Is_Host_Reflected_Track(
      tracker_info,
      host,
      sep,
      calibs,
      sensors,
      obj_idx,
      trailer_data,
      object_tracks);

   /** \result
   * Track should be marked as mirror
   **/
   CHECK_FALSE(f_host_mirror);
}

/**
*\purpose  Purpose of this test is to verify that object is flagged as a host mirror
*          because of suspected SEP has a local extreme value (minimum or maximum)
*          far away from host.
*          Host has no trailer attached.
*\req    NA
*/
TEST(f360_is_host_reflected_track, Is_Host_Reflected_Track__Reflection_Far_Ahead_SEP_Crossing_ghost)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - Object heading and pointing set to 0 along with cos/sin
    * - Object position is adjacent to host behind SEP id 1
    * - Object is flagged as behind SEP id 1
    * - Object is flagged as moving
    * - SEP with id 1 is valid adjacent to host on right side
    * Host has no trailer attached.
    */

   sep[0].lower_limit = -24.49F;
   sep[0].upper_limit = 43.89F;
   sep[0].p0 = 7.60F;
   sep[0].p1 = -0.0484F;
   sep[0].p2 = 0.0006845F;

   object_tracks[obj_idx].speed = 5.4F;
   object_tracks[obj_idx].f_moving = true;
   object_tracks[obj_idx].reference_point = F360_REFERENCE_POINT_LEFT;
   object_tracks[obj_idx].vcs_position.x = -2.12F;
   object_tracks[obj_idx].vcs_position.y = 14.53F;
   object_tracks[obj_idx].bbox.Set_Center(object_tracks[obj_idx].vcs_position);
   object_tracks[obj_idx].vcs_heading = Angle{ -0.072F };
   object_tracks[obj_idx].hdg_ptng_disagmt = -0.006F;
   object_tracks[obj_idx].bbox.Set_Orientation(Angle{object_tracks[obj_idx].vcs_heading + object_tracks[obj_idx].hdg_ptng_disagmt});
   object_tracks[obj_idx].bbox.Set_Length(5.62F);
   object_tracks[obj_idx].bbox.Set_Width(1.89F);

   host.speed = 6.08F;

   /** \action
   * Call Is_Host_Reflected_Track
   **/
   bool f_host_mirror = Is_Host_Reflected_Track(
      tracker_info,
      host,
      sep,
      calibs,
      sensors,
      obj_idx,
      trailer_data,
      object_tracks);

   /** \result
   * Track should be marked as mirror
   **/
   CHECK_TRUE(f_host_mirror);
}

/** \brief
* Test group contains common data for test cases related to testing of function Is_Host_Reflected_Track()
* using the non-SEP method (reflection without static environment polynomials).
* Data is set up so that the object matches a host mirror object by the non-SEP method,
* including stationary reflector objects required for the non-SEP logic.
* Data is tweaked in individual test cases to test different host speed conditions.
**/
TEST_GROUP(f360_is_host_reflected_track_non_sep)
{
   F360_Host_T host = {};
   Static_Env_Poly_T sep[F360_NUM_OF_STATIC_ENV_POLYS] = {};
   F360_Calibrations_T calibs = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   int32_t obj_idx;
   F360_Trailer_Estimator_Output_T trailer_data = {};

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      obj_idx = 0;

      // Set up ghost candidate (object under test)
      object_tracks[obj_idx].speed = 1.6F;
      object_tracks[obj_idx].f_moving = true;
      object_tracks[obj_idx].reference_point = F360_REFERENCE_POINT_CENTER;
      object_tracks[obj_idx].vcs_velocity.longitudinal = -1.6F; // Approaching host
      object_tracks[obj_idx].vcs_velocity.lateral = 0.0F;
      object_tracks[obj_idx].bbox.Set_Center(Point{ 12.5F, 0.0F });
      object_tracks[obj_idx].bbox.Set_Orientation(Angle{ F360_PI });
      object_tracks[obj_idx].id = 1;

      // Set up host
      host.vehicle_length = 5.0F;
      host.vehicle_width = 2.0F;
      host.f_trailer_presence_hardware = false;
      host.curvature_rear = 0.0F;

      // Set up 5 stationary objects as reflectors
      for (int i = 0; i < 5; ++i)
      {
         int idx = i + 1; // Avoid obj_idx (0)
         object_tracks[idx].id = idx + 1;
         object_tracks[idx].movable_prob = 0.0F;
         object_tracks[idx].status = F360_OBJECT_STATUS_UPDATED;
         object_tracks[idx].otg_height = 1.0F;
         // Place them near the midpoint between host and ghost candidate (host_center = (-2.5, 0), ghost_center = (12.5, 0) => midpoint = (5.0, 0))
         object_tracks[idx].bbox.Set_Center(Point{ 5.0F, -0.5F + 0.3F * i });
         object_tracks[idx].bbox.Set_Orientation(Angle{ 0.0F });
      }

      // Set up tracker_info to reference these objects as active
      tracker_info.num_active_objs = 6;
      tracker_info.active_obj_ids[0] = 1; // ghost candidate
      tracker_info.active_obj_ids[1] = 2;
      tracker_info.active_obj_ids[2] = 3;
      tracker_info.active_obj_ids[3] = 4;
      tracker_info.active_obj_ids[4] = 5;
      tracker_info.active_obj_ids[5] = 6;

      // No SEP is valid
      for (int i = 0; i < F360_NUM_OF_STATIC_ENV_POLYS; ++i) {
         sep[i].status = F360_STATIC_ENV_POLY_STATUS_INVALID;
      }
   }
};
/**
*\purpose  Purpose of this test is to verify that the non-SEP method branch is taken
*          when host speed is greater than 1.5 and the object is suspected as a host reflection
*          by the non-SEP method. The object should be flagged as a host mirror.
*\req    NA
*/
TEST(f360_is_host_reflected_track_non_sep, Is_Host_Reflected_Track__Non_SEP_Method_Object_Is_Host_Mirror)
{
   /** \precond
    * Host speed is set above 1.5
    * All other setup is done in TEST_SETUP
    */
   host.speed = 1.6F;

   /** \action */
   bool f_host_mirror = Is_Host_Reflected_Track(
      tracker_info,
      host,
      sep,
      calibs,
      sensors,
      obj_idx,
      trailer_data,
      object_tracks);

   /** \result */
   CHECK_TRUE(f_host_mirror);
   CHECK_EQUAL(object_tracks[obj_idx].mirror_prob, 1.0F);
}

/**
*\purpose  Purpose of this test is to verify that the non-SEP method branch is not taken
*          when host speed is less than or equal to 1.5, even if the object would otherwise be suspected.
*          The object should not be flagged as a host mirror.
*\req    NA
*/
TEST(f360_is_host_reflected_track_non_sep, Is_Host_Reflected_Track__Non_SEP_Method_Host_Speed_Too_Low)
{
   /** \precond
    * Host speed is set below or equal to 1.5
    * All other setup is done in TEST_SETUP
    */
   host.speed = 1.4F;
   object_tracks[obj_idx].speed = 1.4F;
   object_tracks[obj_idx].vcs_velocity.longitudinal = -1.4F; // Approaching host

   /** \action */
   bool f_host_mirror = Is_Host_Reflected_Track(
      tracker_info,
      host,
      sep,
      calibs,
      sensors,
      obj_idx,
      trailer_data,
      object_tracks);

   /** \result */
   CHECK_FALSE(f_host_mirror);
}

/**
*\purpose  Purpose of this test is to verify that the non-SEP method does NOT flag an object as host mirror
*          if it is an oncoming offset target (lateral offset in (2,20), heading in (165,-165) deg).
*\req    NA
*/
TEST(f360_is_host_reflected_track_non_sep, Is_Host_Reflected_Track__Non_SEP_Method_OncomingOffsetTarget_False)
{
   /** \precond
    * Host speed is set above 1.5
    * Object is moving
    * Object lateral position is within (2,20)
    * Object heading is within (165, -165) degrees
    * All other setup is done in TEST_SETUP
    */
   host.speed = 2.0F;
   object_tracks[obj_idx].f_moving = true;
   object_tracks[obj_idx].vcs_position.x = 10.0F;
   object_tracks[obj_idx].vcs_position.y = 10.0F; // within (2,20)
   object_tracks[obj_idx].bbox.Set_Center(object_tracks[obj_idx].vcs_position);
   object_tracks[obj_idx].vcs_heading = Angle(F360_DEG2RAD(170.0F)); // within (165, 180)
   object_tracks[obj_idx].speed = 2.0F;
   object_tracks[obj_idx].vcs_velocity.longitudinal = -2.0F;

   /** \action
    * Call Is_Host_Reflected_Track
    */
   bool f_host_mirror = Is_Host_Reflected_Track(
      tracker_info,
      host,
      sep,
      calibs,
      sensors,
      obj_idx,
      trailer_data,
      object_tracks);

   /** \result
    * Track should NOT be marked as mirror
    */
   CHECK_FALSE(f_host_mirror);
}
/** @}*/
