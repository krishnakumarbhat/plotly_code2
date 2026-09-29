/** \file
 * This file contains unit tests for content of f360_is_host_reflected_track_helpers.cpp file
 */

#include "f360_is_host_reflected_track_helpers.h"
#include <CppUTest/TestHarness.h>


using namespace f360_variant_A;

/** \defgroup  f360_Is_Predicted_Ghost_Position_In_Suspected_Object_Extended_Bbox
 *  @{
 */

/** \brief
 * Test group containing common data for tests related to function Is_Predicted_Ghost_Position_In_Suspected_Object_Extended_Bbox()
 */
TEST_GROUP(f360_Is_Predicted_Ghost_Position_In_Suspected_Object_Extended_Bbox)
{
   F360_Object_Track_T object = {};
   Point host_mirror_track_tcs_pos = {};
   F360_Calibrations_T calibs = {};
   F360_Host_T host = {};
   F360_Trailer_Estimator_Output_T trailer_data = {};


   /** \setup
    * Initialize tracker calibrations
    * Set up an object with posiiton in VCS origin
    * Set up arbitrary valid dimensions of object
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      object.reference_point = F360_REFERENCE_POINT_CENTER;
      object.vcs_position.x = 0.0F;
      object.vcs_position.y = 0.0F;
      object.bbox.Set_Center(object.vcs_position);
      object.vcs_heading = Angle{ 0.0F };
      object.hdg_ptng_disagmt = 0.0F;
      object.bbox.Set_Orientation(object.vcs_heading + object.hdg_ptng_disagmt);
      object.bbox.Set_Length(2.0F);
      object.bbox.Set_Width(2.0F);

      host.f_trailer_presence_hardware = false;
      trailer_data.trailer_length[0] = 0.0F;
      trailer_data.trailer_length[1] = 0.0F;
      trailer_data.trailer_width[0] = 0.0F;
      trailer_data.trailer_width[1] = 0.0F;
   }
};

/** \purpose
 * Verify that function returns true if host ghost predicted position is inside
 * zone to trigger countermeasure. Host has no trailer attached.
 * \req
 * NA
 */
TEST(f360_Is_Predicted_Ghost_Position_In_Suspected_Object_Extended_Bbox, Is_Predicted_Ghost_Position_In_Suspected_Object_Extended_Bbox__Ghost_Position_Inside_Zone_No_Trailer)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - Set object properties,
    *   - Position in VCS origin
    *   - VCS pointing set to 0, along with updated cos/sin values of pointing
    *   - All size dimensions set to 1
    * Set host_mirror_track_vcs_pos to be inside zone
    * Host has no trailer attached.
    */
   host_mirror_track_tcs_pos.x = 0.0F;
   host_mirror_track_tcs_pos.y = 0.0F;

   /** \action
    * Call Is_Predicted_Ghost_Position_In_Suspected_Object_Extended_Bbox
    */
   bool f_inside_susp_zone = Is_Predicted_Ghost_Position_In_Suspected_Object_Extended_Bbox(
      object,
      host_mirror_track_tcs_pos,
      calibs,
      host,
      trailer_data);

   /** \result
    * Verify that function have returned true
    */
   CHECK_TRUE(f_inside_susp_zone);
}

/** \purpose
 * Verify that function returns true if host ghost predicted position is outside
 * zone to trigger countermeasure. Host ghost predicted position is "above" the object.
 * Host has no trailer attached.
 * \req
 * NA
 */
TEST(f360_Is_Predicted_Ghost_Position_In_Suspected_Object_Extended_Bbox, Is_Predicted_Ghost_Position_In_Suspected_Object_Extended_Bbox__Ghost_Position_Outside_Zone_Above_No_Trailer)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - Set object properties,
    *   - Position in VCS origin
    *   - VCS pointing set to 0, along with updated cos/sin values of pointing
    *   - All size dimensions set to 1
    * Set host_mirror_track_vcs_pos to be outside zone (greater than len2 + calibration)
    * Host has no trailer attached.
    */
   host_mirror_track_tcs_pos.x = 0.5F * object.bbox.Get_Length() + calibs.k_host_refl_bbox_long_ext + 0.1F;
   host_mirror_track_tcs_pos.y = 0.0F;

   /** \action
    * Call Is_Predicted_Ghost_Position_In_Suspected_Object_Extended_Bbox
    */
   bool f_inside_susp_zone = Is_Predicted_Ghost_Position_In_Suspected_Object_Extended_Bbox(
      object,
      host_mirror_track_tcs_pos,
      calibs,
      host,
      trailer_data);

   /** \result
    * Verify that function have returned false
    */
   CHECK_FALSE(f_inside_susp_zone);
}

/** \purpose
 * Verify that function returns true if host ghost predicted position is inside
 * zone to trigger countermeasure. Host has two trailers attached.
 * \req
 * NA
 */
TEST(f360_Is_Predicted_Ghost_Position_In_Suspected_Object_Extended_Bbox, Is_Predicted_Ghost_Position_In_Suspected_Object_Extended_Bbox__Ghost_Position_Inside_Zone_Trailer_Attached)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - Set object properties,
    *   - VCS pointing set to 0, along with updated cos/sin values of pointing
    * Set object vcs_position to be inside zone
    * Host trailer attached.
    * Trailer dimensions set
    */

   object.vcs_position.x = -16.0F;
   object.vcs_position.y = -2.0F;

   host_mirror_track_tcs_pos.x = -2.0F;
   host_mirror_track_tcs_pos.y = -1.0F;

   host.f_trailer_presence_hardware = true;

   trailer_data.trailer_length[0] = 10.0F;
   trailer_data.trailer_length[1] = 4.0F;
   trailer_data.trailer_width[0] = 2.55F;
   trailer_data.trailer_width[1] = 3.0F;

   /** \action
    * Call Is_Predicted_Ghost_Position_In_Suspected_Object_Extended_Bbox
    */
   bool f_inside_susp_zone = Is_Predicted_Ghost_Position_In_Suspected_Object_Extended_Bbox(
      object,
      host_mirror_track_tcs_pos,
      calibs,
      host,
      trailer_data);

   /** \result
    * Verify that function have returned true
    */
  CHECK_TRUE(f_inside_susp_zone);
}

/** @}*/

/** \defgroup  f360_Determine_Heading_And_Speed_Threshold
 *  @{
 */

 /** \brief
  * Test group containing common data for tests related to function Determine_Heading_And_Speed_Threshold()
  */
TEST_GROUP(f360_Determine_Heading_And_Speed_Threshold)
{
   float32_t host_speed;
   F360_Calibrations_T calibs;
   float32_t max_heading;
   float32_t max_speed_diff;

   float32_t test_pass_thres = 0.0001F;
   float32_t rot_angle = 0.0F;

   /** \setup
    * Initialize tracker calibrations
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
   }
};

/** \purpose
 * Verify that function returns correct thresholds when host speed is low
 * \req
 * NA
 */
TEST(f360_Determine_Heading_And_Speed_Threshold, Determine_Heading_And_Speed_Threshold__Low_Host_Speed)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * Set host speed below calibration
    */
   host_speed = calibs.k_host_refl_lowspeed_host_speed_th - 0.1F;

   /** \action
    * Call Is_Predicted_Ghost_Position_In_Suspected_Object_Extended_Bbox
    */
   Determine_Heading_And_Speed_Threshold(
      host_speed,
      rot_angle,
      calibs,
      max_heading,
      max_speed_diff);

   /** \result
    * Verify that function have returned correct thresholds
    */
   DOUBLES_EQUAL(calibs.k_host_refl_lowspeed_speed_diff_th, max_speed_diff, test_pass_thres);
   DOUBLES_EQUAL(calibs.k_host_refl_lowspeed_heading_th, max_heading, test_pass_thres);

}

/** \purpose
 * Verify that function returns correct thresholds when host speed is above calibration value
 * \req
 * NA
 */
TEST(f360_Determine_Heading_And_Speed_Threshold, Determine_Heading_And_Speed_Threshold__Host_Speed_Above_Cal)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * Set host speed above calibration
    */
   host_speed = calibs.k_host_refl_lowspeed_host_speed_th + 0.1F;

   /** \action
    * Call Is_Predicted_Ghost_Position_In_Suspected_Object_Extended_Bbox
    */
   Determine_Heading_And_Speed_Threshold(
      host_speed,
      rot_angle,
      calibs,
      max_heading,
      max_speed_diff);

   /** \result
    * Verify that function have returned correct thresholds
    */
   DOUBLES_EQUAL(3.04F, max_speed_diff, test_pass_thres);
   DOUBLES_EQUAL(calibs.k_host_refl_highspeed_heading_th, max_heading, test_pass_thres);
}

/** \purpose
 * Verify that function returns correct thresholds when host speed is by far above calibration value
 * and speed max_speed_diff should be saturated
 * \req
 * NA
 */
TEST(f360_Determine_Heading_And_Speed_Threshold, Determine_Heading_And_Speed_Threshold__Host_Speed_Above_Cal_Saturated_Threshold)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * Set host speed very high
    */
   host_speed = 150.0F;

   /** \action
    * Call Is_Predicted_Ghost_Position_In_Suspected_Object_Extended_Bbox
    */
   Determine_Heading_And_Speed_Threshold(
      host_speed,
      rot_angle,
      calibs,
      max_heading,
      max_speed_diff);

   /** \result
    * Verify that function have returned correct thresholds
    */
   DOUBLES_EQUAL(calibs.k_host_refl_highspeed_max_speed_diff_th, max_speed_diff, test_pass_thres);
   DOUBLES_EQUAL(calibs.k_host_refl_highspeed_heading_th, max_heading, test_pass_thres);
}

/** @}*/

/** \defgroup  f360_Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method_no_trailer_attached
 *  @{
 */

 /** \brief
  * Test group containing common data for tests related to function Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method() without trailer attached
  */
TEST_GROUP(f360_Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method_no_trailer_attached)
{
   F360_Object_Track_T object;
   float32_t host_speed;
   F360_Calibrations_T calibs;
   F360_Host_T host = {};
   F360_Trailer_Estimator_Output_T trailer_data = {};



   /** \setup
    * Initialize tracker calibrations
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);

      host_speed = calibs.k_host_refl_lowspeed_host_speed_th - 0.1F;

      object.vcs_heading = Angle{ 0.0F };
      object.vcs_position.x = calibs.k_host_refl_min_obj_long_pos - 1.0F;
      object.speed = 0.0F;

      host.f_trailer_presence_hardware = false;
      trailer_data.trailer_length[0] = 0.0F;
      trailer_data.trailer_length[1] = 0.0F;
   }
};

/** \purpose
 * Verify that function returns correct false when object is too far to the rear in longitudinal position. Host has no trailer attached.
 * \req
 * NA
 */
TEST(f360_Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method_no_trailer_attached, Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method__Object_Too_Far_To_The_Rear)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - Object heading set to 0
    * - Object position is outside suspected zone (too far to the rear in longitudinal position)
    * - Object speed set to 0
    * Host has no trailer attached.
    */

   /** \action
    * Call Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method
    */
   bool f_suspected = Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method(
      host,
      trailer_data,
      object,
      calibs);

   /** \result
    * Verify that function have returned false
    */
   CHECK_FALSE(f_suspected);
}

/** \purpose
 * Verify that function returns correct false when object is too far to the front in longitudinal position. Host has no trailer attached.
 * \req
 * NA
 */
TEST(f360_Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method_no_trailer_attached, Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method__Object_Too_Far_To_The_Front)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - Object heading set to 0
    * - Object speed set to 0
    * Set object position above calibration for max longitudinal position
    * Host has no trailer attached.
    */
   object.vcs_position.x = calibs.k_host_refl_max_obj_long_pos + 1.0F;

    /** \action
     * Call Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method
     */
   bool f_suspected = Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method(
      host,
      trailer_data,
      object,
      calibs);

   /** \result
    * Verify that function have returned false
    */
   CHECK_FALSE(f_suspected);
}

/** \purpose
 * Verify that function returns correct false when object heading is too large. Host has no trailer attached.
 * \req
 * NA
 */
TEST(f360_Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method_no_trailer_attached, Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method__Object_Heading_Too_Large)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - Object speed set to 0
    * - Object position is outside suspected zone (too far to the rear in longitudinal position)
    * Set object heading large so it's larger than dynamic threshold
    * Host has no trailer attached.
    */
   object.vcs_heading = Angle{ F360_DEG2RAD(90.0F) };

   /** \action
   *
    * Call Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method
    */
   bool f_suspected = Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method(
      host,
      trailer_data,
      object,
      calibs);

   /** \result
    * Verify that function have returned false
    */
   CHECK_FALSE(f_suspected);
}

/** \purpose
 * Verify that function returns correct false when object position is outside valid zone
 * but speed condition is fulfilled
 * Host has no trailer attached.
 * \req
 * NA
 */
TEST(f360_Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method_no_trailer_attached, Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method__Object_Speed_Same_As_Host)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - Object speed set to 0
    * - Object position is outside suspected zone (too far to the rear in longitudinal position)
    * Set object speed equal to host speed
    * Host has no trailer attached.
    */
   object.speed = host_speed;

   /** \action
   *
    * Call Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method
    */
   bool f_suspected = Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method(
      host,
      trailer_data,
      object,
      calibs);

   /** \result
    * Verify that function have returned false
    */
   CHECK_FALSE(f_suspected);
}

/** \purpose
 * Verify that function returns correct false when object position is inside valid zone
 * but object speed is too far off from host speed.
 * Host has no trailer attached.
 * \req
 * NA
 */
TEST(f360_Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method_no_trailer_attached, Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method__All_Conditions_Fulfilled_Except_Object_Speed)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - Object speed set to 0
    * - Object position is outside suspected zone (too far to the rear in longitudinal position)
    * Set object speed equal to host speed
    * Host has no trailer attached.
    */
   object.vcs_position.x = 0.0F;

   /** \action
   *
    * Call Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method
    */
   bool f_suspected = Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method(
      host,
      trailer_data,
      object,
      calibs);

   /** \result
    * Verify that function have returned false
    */
   CHECK_FALSE(f_suspected);
}

/** \purpose
 * Verify that function returns correct true when all conditions are met for the object to be a
 * host mirror suspect.
 * Host has no trailer attached.
 * \req
 * NA
 */
TEST(f360_Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method_no_trailer_attached, Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method__Object_Is_Suspect)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - Object speed set to 0
    * Set object position adjacent to host
    * Set object speed equal to host speed
    * Host has no trailer attached.
    */
   object.vcs_position.x = 0.0F;
   object.speed = host_speed;
   object.f_moving = true;

   /** \action
   *
    * Call Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method
    */
   bool f_suspected = Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method(
      host,
      trailer_data,
      object,
      calibs);

   /** \result
    * Verify that function have returned true
    */
   CHECK_TRUE(f_suspected);
}
/** @}*/

/** @}*/

/** \defgroup  f360_Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method_with_trailer_attached
 *  @{
 */

 /** \brief
  * Test group containing common data for tests related to function Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method() with trailer attached
  */
TEST_GROUP(f360_Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method_with_trailer_attached)
{
   F360_Object_Track_T object;
   float32_t host_speed;
   F360_Calibrations_T calibs;
   F360_Host_T host = {};
   F360_Trailer_Estimator_Output_T trailer_data = {};



   /** \setup
    * Initialize tracker calibrations
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      object.f_moving = true;

      host.vehicle_length = 5.0F;
      host.f_trailer_presence_hardware = true;
      trailer_data.trailer_length[0] = 8.0F;
      trailer_data.trailer_length[1] = 5.0F;
   }
};

/** \purpose
 * Verify that function returns correct false when object is too far to the rear in longitudinal position. Host has a trailer attached.
 * \req
 * NA
 */
TEST(f360_Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method_with_trailer_attached, Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method__Trailer_Attached_Object_Too_Far_To_The_Rear)
{
   /** \precond
    * As in setup
    * Object position is too far rear in longitudinal position.
    * Host has trailer attached.
    */
   object.vcs_position.x = -18.1F;

   /** \action
    * Call Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method
    */
   bool f_suspected = Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method(
      host,
      trailer_data,
      object,
      calibs);

   /** \result
    * Verify that function have returned false
    */
   CHECK_FALSE(f_suspected);
}

/** \purpose
 * Verify that function returns correct true when object is at the very end of the rear longitudinal position limit with trailer. Host has a trailer attached.
 * \req
 * NA
 */
TEST(f360_Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method_with_trailer_attached, Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method__Trailer_Attached_Object_At_The_Very_End)
{
   /** \precond
    * As in setup
    * Object position is at the rear end of host with trailer in longitudinal position.
    * Host has trailer attached.
    */
   object.vcs_position.x = -17.9F;

   /** \action
    * Call Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method
    */
   bool f_suspected = Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method(
      host,
      trailer_data,
      object,
      calibs);

   /** \result
    * Verify that function have returned true
    */
   CHECK_TRUE(f_suspected);
}

/** \purpose
 * Verify that function returns correct true when object is at the very end of the rear longitudinal position limit with trailer. Host has a trailer attached and does not have the length set (should have default lenth)
 * \req
 * NA
 */
TEST(f360_Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method_with_trailer_attached, Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method__Trailer_Attached_Object_At_The_Very_End_Host_Length_Not_Set)
{
   /** \precond
    * As in setup
    * Object position is at the rear end of host with trailer in longitudinal position.
    * Host has no length set prior to the function call.
    * Host has trailer attached.
    */
   object.vcs_position.x = -17.9F;
   host.vehicle_length = 0.0F;

   /** \action
    * Call Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method
    */
   bool f_suspected = Is_Object_Suspected_Of_Being_Host_Reflection_For_SEP_Method(
      host,
      trailer_data,
      object,
      calibs);

   /** \result
    * Verify that function have returned true
    */
   CHECK_TRUE(f_suspected);
}

/** @}*/

/** \defgroup  f360_Calc_Predicted_Reflected_Track_TCS_Position
 *  @{
 */

 /** \brief
  * Test group containing common data for tests related to function Calc_Predicted_Reflected_Track_TCS_Position()
  */
TEST_GROUP(f360_Calc_Predicted_Reflected_Track_TCS_Position)
{
   F360_Object_Track_T object;
   float32_t sensor_long_pos = -5.0F;
   float32_t sensor_lat_pos = 1.0F;
   float32_t sep_lat_pos_sensor = 3.0F;
   float32_t test_pass_thres = 0.0001F;
   float32_t rot_angle = 0.0F;

   /** \setup
    * Initialize tracker calibrations
    */
   TEST_SETUP()
   {
      object.reference_point = F360_REFERENCE_POINT_CENTER;
      object.vcs_position.x = -2.0F;
      object.vcs_position.y = 4.0F;
      object.bbox.Set_Center(object.vcs_position);
      object.vcs_heading = Angle(0.0F);
      object.hdg_ptng_disagmt = 0.0F;
      object.bbox.Set_Orientation(object.vcs_heading + object.hdg_ptng_disagmt);
      object.bbox.Set_Length(5.0F);
      object.bbox.Set_Width(2.0F);
   }
};

/** \purpose
 * Verify that function returns correct predicted VCS position for an arbitrary object adjacent to host
 * \req
 * NA
 */
TEST(f360_Calc_Predicted_Reflected_Track_TCS_Position, Calc_Predicted_Reflected_Track_TCS_Position__Predicted_Position)
{
   /** \precond
    * In test group the following have been set up
    * - Object pointing set to 0, along with cos/sin
    * - Object position at VCS (-2, 4)
    */

    /** \action
     * Call Calc_Predicted_Reflected_Track_TCS_Position
     */
   Point tcs_pos = Calc_Predicted_Reflected_Track_TCS_Position(
      object,
      sensor_long_pos,
      sensor_lat_pos,
      sep_lat_pos_sensor,
	  rot_angle);
   /** \result
    * Verify that function have returned correct position in TCS frame
    */
   DOUBLES_EQUAL(-3.0F, tcs_pos.x, test_pass_thres);
   DOUBLES_EQUAL(1.0F, tcs_pos.y, test_pass_thres);
}
/** @}*/

/** \defgroup  f360_Is_SEP_Valid_For_Host_Mirror_Ghost
 *  @{
 */

 /** \brief
  * Test group containing common data for tests related to function Is_SEP_Valid_For_Host_Mirror_Ghost()
  */
TEST_GROUP(f360_Is_SEP_Valid_For_Host_Mirror_Ghost)
{
   Static_Env_Poly_T sep = {};
   F360_Calibrations_T calibs = {};

   /** \setup
    * Initialize tracker calibrations
    */
   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      sep.status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      sep.lower_limit = -25.0F;
      sep.upper_limit = 25.0F;
      sep.p0 = 2.0F;

   }
};

/** \purpose
 * Verify that function returns true when SEP is valid and its interval extends both behind and
 * in front of host.
 * \req
 * NA
 */
TEST(f360_Is_SEP_Valid_For_Host_Mirror_Ghost, Is_SEP_Valid_For_Host_Mirror_Ghost__SEP_Spanning_Over_Host_Length)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - SEP lower limit set to -25
    * - SEP upper limit set to 25
    */

    /** \action
     * Call Is_SEP_Valid_For_Host_Mirror_Ghost
     */
   bool f_valid_Sep = Is_SEP_Valid_For_Host_Mirror_Ghost(
      sep,
      calibs);

   /** \result
    * Verify that function have returned true
    */
   CHECK_TRUE(f_valid_Sep);
}

/** \purpose
 * Verify that function returns false when SEP is valid but its valid interval is strictly behind host
 * \req
 * NA
 */
TEST(f360_Is_SEP_Valid_For_Host_Mirror_Ghost, Is_SEP_Valid_For_Host_Mirror_Ghost__SEP_Behind_Host)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - SEP lower limit set to -25
    * - SEP upper limit set to 25
    * Set SEP upper limit behind host
    */
   sep.upper_limit = -2.1F * calibs.k_host_refl_half_host_length;

    /** \action
     * Call Is_SEP_Valid_For_Host_Mirror_Ghost
     */
   bool f_valid_Sep = Is_SEP_Valid_For_Host_Mirror_Ghost(
      sep,
      calibs);

   /** \result
    * Verify that function have returned false
    */
   CHECK_FALSE(f_valid_Sep);
}

/** \purpose
 * Verify that function returns false when SEP is valid but its valid interval is strictly in front of host
 * \req
 * NA
 */
TEST(f360_Is_SEP_Valid_For_Host_Mirror_Ghost, Is_SEP_Valid_For_Host_Mirror_Ghost__SEP_In_Front_Of_Host)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - SEP lower limit set to -25
    * - SEP upper limit set to 25
    * Set SEP lower limit in front of host
    */
   sep.lower_limit = 1.0F;

   /** \action
    * Call Is_SEP_Valid_For_Host_Mirror_Ghost
    */
   bool f_valid_Sep = Is_SEP_Valid_For_Host_Mirror_Ghost(
      sep,
      calibs);

   /** \result
    * Verify that function have returned false
    */
   CHECK_FALSE(f_valid_Sep);
}

/** \purpose
 * Verify that function returns false when SEP is invalid
 * \req
 * NA
 */
TEST(f360_Is_SEP_Valid_For_Host_Mirror_Ghost, Is_SEP_Valid_For_Host_Mirror_Ghost__SEP_Invalid)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - SEP lower limit set to -25
    * - SEP upper limit set to 25
    * Set SEP as invalid
    */
   sep.status = F360_STATIC_ENV_POLY_STATUS_INVALID;

   /** \action
    * Call Is_SEP_Valid_For_Host_Mirror_Ghost
    */
   bool f_valid_Sep = Is_SEP_Valid_For_Host_Mirror_Ghost(
      sep,
      calibs);

   /** \result
    * Verify that function have returned false
    */
   CHECK_FALSE(f_valid_Sep);
}
/**
*\purpose  Purpose of this test is to verify that suspected SEP is crossing host's path.
*\req    NA
*/
TEST(f360_Is_SEP_Valid_For_Host_Mirror_Ghost, Is_SEP_Valid_For_Host_Mirror_Ghost__SEP_Cross_Path)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - SEP set up in such way that it is crossing host's path
    */

   sep.lower_limit = -7.2f;
   sep.upper_limit = -4.28f;
   sep.p0 = -11.86f;
   sep.p1 = -5.69f;
   sep.p2 = -0.52f;

   /** \action
   * Call Is_SEP_Valid_For_Host_Mirror_Ghost
   **/
   bool f_SEP_valid = Is_SEP_Valid_For_Host_Mirror_Ghost(
      sep,
      calibs);

   /** \result
   * SEP should not be valid
   **/
   CHECK_FALSE(f_SEP_valid);
}
/**
*\purpose  Purpose of this test is to verify that suspected SEP is crossing host's path but at greater distance.
*\req    NA
*/
TEST(f360_Is_SEP_Valid_For_Host_Mirror_Ghost, Is_SEP_Valid_For_Host_Mirror_Ghost__SEP_Cross_Path_Greater_Distance)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - SEP set up in such way that it is crossing host's path at a greater distance
    */

   sep.lower_limit = -25.0f;
   sep.upper_limit = 25.0f;
   sep.p0 = 320.0f;
   sep.p1 = -45.0f;
   sep.p2 = 1.0f;

   /** \action
   * Call Is_SEP_Valid_For_Host_Mirror_Ghost
   **/
   bool f_SEP_valid = Is_SEP_Valid_For_Host_Mirror_Ghost(
      sep,
      calibs);

   /** \result
   * SEP should be valid
   **/
   CHECK_TRUE(f_SEP_valid);
}
/** @}*/

/** \defgroup  f360_Is_Sensor_Adjacent_To_SEP
 *  @{
 */

 /** \brief
  * Test group containing common data for tests related to function Is_Sensor_Adjacent_To_SEP()
  */


TEST_GROUP(f360_Is_Sensor_Adjacent_To_SEP)
{
   /** \setup
    * Initialize tracker calibrations
    * Set up sensors
    */

   Static_Env_Poly_T sep = {};
   F360_Calibrations_T calibs = {};

   TEST_SETUP()
   {
      Initialize_Tracker_Calibrations(calibs);
      sep.status = F360_STATIC_ENV_POLY_STATUS_UPDATED;
      sep.p0 = 2.0F;
   }
};
/** \purpose
 * Verify that function returns true when sensor is adjacent
 * to the SEP.
 * \req
 * NA
 */
TEST(f360_Is_Sensor_Adjacent_To_SEP, Is_Sensor_Adjacent_To_SEP__SEP_Spanning_Over_Host_Length)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - SEP lower limit set to -25
    * - SEP upper limit set to 25
    * - Sensor position set to right upper corner of host
    */

   float32_t sensor_long_pos = 0.0F;
   float32_t sensor_lat_pos = 1.0F;
   sep.lower_limit = -25.0F;
   sep.upper_limit = 25.0F;

   /** \action
     * Call Is_Sensor_Adjacent_To_SEP
     */
   bool f_sensor_adjacent = Is_Sensor_Adjacent_To_SEP(
      sep,
      sensor_long_pos,
      sensor_lat_pos,
      calibs);

   /** \result
    * Verify that function have returned true
    */
   CHECK_TRUE(f_sensor_adjacent);
}

/** \purpose
 * Verify that function returns false when sensor is below
 * the SEP.
 * \req
 * NA
 */
TEST(f360_Is_Sensor_Adjacent_To_SEP, Is_Sensor_Adjacent_To_SEP__Below)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - SEP lower limit set to -2
    * - SEP upper limit set to 25
    * - Sensor position set to right lower corner of host
    */
   float32_t sensor_long_pos = -4.0F;
   float32_t sensor_lat_pos = 1.0F;
   sep.lower_limit = -2.0F;
   sep.upper_limit = 25.0F;

   /** \action
     * Call Is_Sensor_Adjacent_To_SEP
     */
   bool f_sensor_adjacent = Is_Sensor_Adjacent_To_SEP(
      sep,
      sensor_long_pos,
      sensor_lat_pos,
      calibs);

   /** \result
    * Verify that function have returned false
    */
   CHECK_FALSE(f_sensor_adjacent);
}
/** \purpose
 * Verify that function returns false when sensor is above
 * the SEP.
 * \req
 * NA
 */
TEST(f360_Is_Sensor_Adjacent_To_SEP, Is_Sensor_Adjacent_To_SEP__Above)
{
   /** \precond
    * In test group the following have been set up
    * - Initialize calibrations
    * - SEP lower limit set to -25
    * - SEP upper limit set to -2
    * - Sensor position set to right upper corner of host
    */

   float32_t sensor_long_pos = 0.0F;
   float32_t sensor_lat_pos = 1.0F;
   sep.lower_limit = -25.0F;
   sep.upper_limit = -2.0F;

   /** \action
     * Call Is_Sensor_Adjacent_To_SEP
     */
   bool f_sensor_adjacent = Is_Sensor_Adjacent_To_SEP(
      sep,
      sensor_long_pos,
      sensor_lat_pos,
      calibs);

   /** \result
    * Verify that function have returned false
    */
   CHECK_FALSE(f_sensor_adjacent);
}
/** @}*/

/** \defgroup  f360_determine_heading_and_speed_threshold
*  @{
*/

/** \brief
* Test groups contains common data for test cases related to testing of function Determine_Heading_And_Speed_Threshold()
* Data is set up in such a way that expectation to max_speading and max_speed change accordingly.
* Host is tweaked in an individual test case.
**/

TEST_GROUP(f360_determine_heading_and_speed_threshold)
{
    /** \setup
    * Initialize calibrations
    * Set up data so max_heading and max_speed are 0.
    * Host speed is set to low to generate fixed thresholds equal to calibration parameters.
	* Rotation angle set to 0 to simulate SEP being parallel to host
    **/

    float host_speed = {};
    F360_Calibrations_T calibs = {};
    float max_heading = 0.0F;
    float max_speed_diff = 0.0F;
   float rot_angle = 0.0F;

    TEST_SETUP()
    {
        Initialize_Tracker_Calibrations(calibs);
        host_speed = 1.0F;
    }
};

/**
*\purpose  Purpose of this test is to verify object's max speed and heading
* for slow host.
*\req    NA
*/
TEST(f360_determine_heading_and_speed_threshold, Host_Is_Slow)
{
    /** \precond
    * Init Calibrations: Host speed is 1.0
     */

     /** \action
     * Call Determine_Heading_And_Speed_Threshold
     **/
    Determine_Heading_And_Speed_Threshold(host_speed, rot_angle, calibs, max_heading, max_speed_diff);
    /** \result
    * max_speed and max_heading should be equal to the expected values
    **/
    float test_epsilon = 1e-4F;
    DOUBLES_EQUAL_TEXT(max_speed_diff, 1.0F, test_epsilon, "The maximum speed difference is wrongly determined");
    DOUBLES_EQUAL_TEXT(max_heading, 0.1F, test_epsilon, "The maximum heading difference is wrongly determined");
}

/**
*\purpose  Purpose of this test is to verify object's max speed and heading
* for fast host.
*\req    NA
*/
TEST(f360_determine_heading_and_speed_threshold, Host_Is_Fast)
{
    /** \precond
    * Change host speed to 50 m/s
     */
    host_speed = 50.0F;
    /** \action
    * Call Determine_Heading_And_Speed_Threshold
    **/
    Determine_Heading_And_Speed_Threshold(host_speed, rot_angle, calibs, max_heading, max_speed_diff);
    /** \result
    * max_speed and max_heading should be equal to the expected values
    **/
    float test_epsilon = 1e-4F;
    DOUBLES_EQUAL_TEXT(max_speed_diff, 7.0F, test_epsilon, "The maximum speed difference is wrongly determined");
    DOUBLES_EQUAL_TEXT(max_heading, 0.7F, test_epsilon, "The maximum heading difference is wrongly determined");
}

/**
*\purpose  Purpose of this test is to verify object's max speed and heading
* when the max speed difference is not saturated for fast host.
*\req    NA
*/
TEST(f360_determine_heading_and_speed_threshold, Host_Is_Fast_No_Max_Speed_Not_Saturated)
{
    /** \precond
    * Change host speed to 6 m/s
     */
    host_speed = 6.0F;
    /** \action
    * Call Determine_Heading_And_Speed_Threshold
    **/
    Determine_Heading_And_Speed_Threshold(host_speed, rot_angle, calibs, max_heading, max_speed_diff);
    /** \result
    * max_speed and max_heading should be equal to the expected values
    **/
    float test_epsilon = 1e-4F;
    DOUBLES_EQUAL_TEXT(max_speed_diff, 3.4F, test_epsilon, "The maximum speed difference is wrongly determined");
    DOUBLES_EQUAL_TEXT(max_heading, 0.7F, test_epsilon, "The maximum heading difference is wrongly determined");
}

/** \defgroup  f360_Is_Object_Suspected_Of_Being_Host_Reflection_For_None_SEP_Method
 *  @{
 */

 /** \brief
  * Test group containing common data for tests related to function Is_Object_Suspected_Of_Being_Host_Reflection_For_None_SEP_Method()
  */
TEST_GROUP(f360_Is_Object_Suspected_Of_Being_Host_Reflection_For_None_SEP_Method)
{
   F360_Host_T host;
   F360_Object_Track_T object;

   /** \setup
    * Set up default host and object values.
    */
   TEST_SETUP()
   {
      host.dist_rear_axle_to_vcs_m = 4.1667F; //corresponds to host center at 2.5m longitudinally
      host.speed = 10.0F;
      host.curvature_rear = 0.0F;

      object.f_moving = true;
      object.vcs_velocity.longitudinal = -5.0F; // Approaching host
      object.vcs_velocity.lateral = 0.0F;
      object.bbox.Set_Center(Point{ 10.0F, 0.0F });
   }
};

/** \purpose
 * Verify that function returns true when object is moving, approaching host, and within 55m.
 */
TEST(f360_Is_Object_Suspected_Of_Being_Host_Reflection_For_None_SEP_Method, Approaching_And_Within_55m)
{
   /** \precond
    * In test group the following have been set up
    * - Host and object initialized with default values
    * - Object is moving (f_moving = true)
    * - Object is approaching host (vcs_velocity.longitudinal < 0)
    * - Object is within 55m of host in both x and y
    */
    /** \action
     * Call Is_Object_Suspected_Of_Being_Host_Reflection_For_None_SEP_Method
     */
   bool result = Is_Object_Suspected_Of_Being_Host_Reflection_For_None_SEP_Method(host, object);
   /** \result
    * Verify that function returns true
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Verify that function returns false when object is not moving.
 */
TEST(f360_Is_Object_Suspected_Of_Being_Host_Reflection_For_None_SEP_Method, Not_Moving)
{
   /** \precond
    * In test group the following have been set up
    * - Host and object initialized with default values
    * - Object is not moving (f_moving = false)
    */
   object.f_moving = false;
   /** \action
    * Call Is_Object_Suspected_Of_Being_Host_Reflection_For_None_SEP_Method
    */
   bool result = Is_Object_Suspected_Of_Being_Host_Reflection_For_None_SEP_Method(host, object);
   /** \result
    * Verify that function returns false
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Verify that function returns false when object is not approaching host (velocity away from host).
 */
TEST(f360_Is_Object_Suspected_Of_Being_Host_Reflection_For_None_SEP_Method, Not_Approaching)
{
   /** \precond
    * In test group the following have been set up
    * - Host and object initialized with default values
    * - Object is moving away from host (vcs_velocity.longitudinal > 0)
    */
   object.vcs_velocity.longitudinal = 5.0F; // Moving away from host
   /** \action
    * Call Is_Object_Suspected_Of_Being_Host_Reflection_For_None_SEP_Method
    */
   bool result = Is_Object_Suspected_Of_Being_Host_Reflection_For_None_SEP_Method(host, object);
   /** \result
    * Verify that function returns false
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Verify that function returns false when object is outside 55m in x.
 */
TEST(f360_Is_Object_Suspected_Of_Being_Host_Reflection_For_None_SEP_Method, Outside_55m_X)
{
   /** \precond
    * In test group the following have been set up
    * - Host and object initialized with default values
    * - Object is placed outside 55m in x direction
    */
   object.bbox.Set_Center(Point{ 56.0F, 0.0F });
   /** \action
    * Call Is_Object_Suspected_Of_Being_Host_Reflection_For_None_SEP_Method
    */
   bool result = Is_Object_Suspected_Of_Being_Host_Reflection_For_None_SEP_Method(host, object);
   /** \result
    * Verify that function returns false
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Verify that function returns false when object is outside 55m in y.
 */
TEST(f360_Is_Object_Suspected_Of_Being_Host_Reflection_For_None_SEP_Method, Outside_55m_Y)
{
   /** \precond
    * In test group the following have been set up
    * - Host and object initialized with default values
    * - Object is placed outside 55m in y direction
    */
   object.bbox.Set_Center(Point{ 10.0F, 56.0F });
   /** \action
    * Call Is_Object_Suspected_Of_Being_Host_Reflection_For_None_SEP_Method
    */
   bool result = Is_Object_Suspected_Of_Being_Host_Reflection_For_None_SEP_Method(host, object);
   /** \result
    * Verify that function returns false
    */
   CHECK_FALSE(result);
}

/** \defgroup  f360_Is_Ghost_Reflected_Host_Mirror_Without_SEP
 *  @{
 */

 /** \brief
  * Test group containing common data for tests related to function Is_Ghost_Reflected_Host_Mirror_Without_SEP()
  */
TEST_GROUP(f360_Is_Ghost_Reflected_Host_Mirror_Without_SEP)
{
   F360_Object_Track_T ghost_candidate = {};
   F360_Tracker_Info_T tracker_info = {};
   F360_Object_Track_T object_tracks[NUMBER_OF_OBJECT_TRACKS] = {};
   F360_Host_T host = {};

   /** \setup
    * Initialize ghost candidate, host, and tracker info with default values.
    */
   TEST_SETUP()
   {
      // Set up ghost candidate (object under test)
      ghost_candidate.speed = 1.6F;
      ghost_candidate.f_moving = true;
      ghost_candidate.reference_point = F360_REFERENCE_POINT_CENTER;
      ghost_candidate.vcs_velocity.longitudinal = -1.6F; // Approaching host
      ghost_candidate.vcs_velocity.lateral = 0.0F;
      ghost_candidate.bbox.Set_Center(Point{ 12.5F, 0.0F });
      ghost_candidate.bbox.Set_Orientation(Angle{ F360_PI });
      ghost_candidate.id = 1;

      // Set up host
      host.speed = 1.6F;
      host.dist_rear_axle_to_vcs_m = 4.1667F; //corresponds to host center at 2.5m longitudinally
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
   }
};

/** \purpose
 * Verify that function returns true when all conditions for a reflective guardrail are met.
 */
TEST(f360_Is_Ghost_Reflected_Host_Mirror_Without_SEP, Is_Ghost_Reflected_Host_Mirror_Without_SEP__Reflective_Guardrail_True)
{
   /** \precond
    * In test group the following have been set up
    * - Ghost candidate, host, tracker_info, and object_tracks initialized with default values
    * - >2 stationary objects near the predicted guardrail line
    */
    /** \action
     * Call Is_Ghost_Reflected_Host_Mirror_Without_SEP
     */
   bool result = Is_Ghost_Reflected_Host_Mirror_Without_SEP(
      ghost_candidate,
      tracker_info,
      object_tracks,
      host);

   /** \result
    * Should return true as there are >2 stationary objects near the predicted guardrail line.
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Verify that function returns false if not enough stationary objects are present.
 */
TEST(f360_Is_Ghost_Reflected_Host_Mirror_Without_SEP, Is_Ghost_Reflected_Host_Mirror_Without_SEP__Not_Enough_Objects)
{
   /** \precond
    * In test group the following have been set up
    * - Only one guardrail candidate near the predicted line
    */
   tracker_info.num_active_objs = 2;
   tracker_info.active_obj_ids[1] = 2;

   /** \action
    * Call Is_Ghost_Reflected_Host_Mirror_Without_SEP
    */
   bool result = Is_Ghost_Reflected_Host_Mirror_Without_SEP(
      ghost_candidate,
      tracker_info,
      object_tracks,
      host);

   /** \result
    * Should return false as there are not enough stationary objects near the predicted guardrail line.
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Verify that function returns false if ghost and host speeds are not similar.
 */
TEST(f360_Is_Ghost_Reflected_Host_Mirror_Without_SEP, Is_Ghost_Reflected_Host_Mirror_Without_SEP__Speed_Not_Similar)
{
   /** \precond
    * In test group the following have been set up
    * - Ghost candidate speed set to a value different from host speed
    */
   ghost_candidate.speed = 1.8F; // Speed difference just over 10%

   /** \action
    * Call Is_Ghost_Reflected_Host_Mirror_Without_SEP
    */
   bool result = Is_Ghost_Reflected_Host_Mirror_Without_SEP(
      ghost_candidate,
      tracker_info,
      object_tracks,
      host);

   /** \result
    * Should return false as ghost and host speeds are not similar.
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Verify that function returns true if host speed is near zero (and thus modifies value to 0.1 for safe division) and ghost speed is set to within 10% of modified host speed
 */
TEST(f360_Is_Ghost_Reflected_Host_Mirror_Without_SEP, Is_Ghost_Reflected_Host_Mirror_Without_SEP__Host_Speed_Zero)
{
   /** \precond
    * In test group the following have been set up
    * - Host speed set to zero
    */
   host.speed = 0.0F;
	ghost_candidate.speed = 0.1F; // Speed difference within 10% of modified host speed (0.1)

   /** \action
    * Call Is_Ghost_Reflected_Host_Mirror_Without_SEP
    */
   bool result = Is_Ghost_Reflected_Host_Mirror_Without_SEP(
      ghost_candidate,
      tracker_info,
      object_tracks,
      host);

   /** \result
    * Should return false as host speed is near zero.
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Verify that function returns false if guardrail candidates are not stationary or not CCA.
 */
TEST(f360_Is_Ghost_Reflected_Host_Mirror_Without_SEP, Is_Ghost_Reflected_Host_Mirror_Without_SEP__Candidates_Not_Stationary)
{
   /** \precond
    * In test group the following have been set up
    * - Guardrail candidates marked as moving (not stationary)
    */
   for (int i = 0; i < 5; ++i)
   {
      object_tracks[i + 1].movable_prob = 1.0F; // Mark as moving
   }

   /** \action
    * Call Is_Ghost_Reflected_Host_Mirror_Without_SEP
    */
   bool result = Is_Ghost_Reflected_Host_Mirror_Without_SEP(
      ghost_candidate,
      tracker_info,
      object_tracks,
      host);

   /** \result
    * Should return false as guardrail candidates are not stationary or not CCA.
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Verify that function returns false if ghost candidate is too far from the host.
 */
TEST(f360_Is_Ghost_Reflected_Host_Mirror_Without_SEP, Is_Ghost_Reflected_Host_Mirror_Without_SEP__Ghost_Too_Far)
{
   /** \precond
    * In test group the following have been set up
    * - Ghost candidate placed far from host
    */
   ghost_candidate.bbox.Set_Center(Point{ 100.0F, 0.0F }); // Far from host

   /** \action
    * Call Is_Ghost_Reflected_Host_Mirror_Without_SEP
    */
   bool result = Is_Ghost_Reflected_Host_Mirror_Without_SEP(
      ghost_candidate,
      tracker_info,
      object_tracks,
      host);

   /** \result
    * Should return false as ghost candidate is too far from the host.
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Verify that function returns false if objects are not updated (wrong status).
 */
TEST(f360_Is_Ghost_Reflected_Host_Mirror_Without_SEP, Is_Ghost_Reflected_Host_Mirror_Without_SEP__Objects_Not_Updated)
{
   /** \precond
    * In test group the following have been set up
    * - Guardrail candidates set to invalid status (not updated)
    */
   for (int i = 0; i < 5; ++i)
   {
      object_tracks[i + 1].status = F360_OBJECT_STATUS_INVALID; // Set to an invalid status
   }

   /** \action
    * Call Is_Ghost_Reflected_Host_Mirror_Without_SEP
    */
   bool result = Is_Ghost_Reflected_Host_Mirror_Without_SEP(
      ghost_candidate,
      tracker_info,
      object_tracks,
      host);

   /** \result
    * Should return false as objects are not updated (wrong status).
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Verify that function returns false if objects are not near the predicted guardrail line.
 */
TEST(f360_Is_Ghost_Reflected_Host_Mirror_Without_SEP, Is_Ghost_Reflected_Host_Mirror_Without_SEP__Objects_Not_Near_Guardrail)
{
   /** \precond
    * In test group the following have been set up
    * - Guardrail candidates placed far from the predicted guardrail line
    */
   for (int i = 0; i < 5; ++i)
   {
      object_tracks[i + 1].bbox.Set_Center(Point{ -1.0F, 6.0F }); // Far from the line
   }

   /** \action
    * Call Is_Ghost_Reflected_Host_Mirror_Without_SEP
    */
   bool result = Is_Ghost_Reflected_Host_Mirror_Without_SEP(
      ghost_candidate,
      tracker_info,
      object_tracks,
      host);

   /** \result
    * Should return false as objects are not near the predicted guardrail line.
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Verify that function returns false if guardrail candidate is outside the 5m X threshold.
 */
TEST(f360_Is_Ghost_Reflected_Host_Mirror_Without_SEP, Is_Ghost_Reflected_Host_Mirror_Without_SEP__Candidate_X_Outside_Threshold)
{
   /** \precond
    * In test group the following have been set up
    * - Guardrail candidates placed far from the predicted guardrail line  in x direction
    */
   for (int i = 0; i < 5; ++i)
   {
      object_tracks[i + 1].bbox.Set_Center(Point{ -1.0F, 0.0F }); // Far from the line
   }

   /** \action
    * Call Is_Ghost_Reflected_Host_Mirror_Without_SEP
    */
   bool result = Is_Ghost_Reflected_Host_Mirror_Without_SEP(
      ghost_candidate,
      tracker_info,
      object_tracks,
      host);

   /** \result
    * Should return false as the guardrail candidate is outside the X threshold.
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Verify that function returns false if guardrail candidate is outside the 5m Y threshold.
 */
TEST(f360_Is_Ghost_Reflected_Host_Mirror_Without_SEP, Is_Ghost_Reflected_Host_Mirror_Without_SEP__Candidate_Y_Outside_Threshold)
{
   /** \precond
    * In test group the following have been set up
    * - Guardrail candidates placed far from the predicted guardrail line  in y direction
    */
   for (int i = 0; i < 5; ++i)
   {
      object_tracks[i + 1].bbox.Set_Center(Point{ 5.0F, 6.0F }); // Far from the line
   }

   /** \action
    * Call Is_Ghost_Reflected_Host_Mirror_Without_SEP
    */
   bool result = Is_Ghost_Reflected_Host_Mirror_Without_SEP(
      ghost_candidate,
      tracker_info,
      object_tracks,
      host);

   /** \result
    * Should return false as the guardrail candidate is outside the Y threshold.
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Verify that function returns false if guardrail candidate is outside the 2m distance to suspected guardrail line.
 */
TEST(f360_Is_Ghost_Reflected_Host_Mirror_Without_SEP, Is_Ghost_Reflected_Host_Mirror_Without_SEP__Candidate_Dist_To_Guardrail_Outside_Threshold)
{
   /** \precond
    * In test group the following have been set up
    * - Guardrail candidates placed far from the predicted guardrail line even thouggh it is within the range of predicted reflection point
    */
   for (int i = 0; i < 5; ++i)
   {
      object_tracks[i + 1].bbox.Set_Center(Point{ 8.0F, 0.0F }); // Far from the line but within range of reflection point
   }

   /** \action
    * Call Is_Ghost_Reflected_Host_Mirror_Without_SEP
    */
   bool result = Is_Ghost_Reflected_Host_Mirror_Without_SEP(
      ghost_candidate,
      tracker_info,
      object_tracks,
      host);

   /** \result
    * Should return false as the guardrail candidate is outside the distance threshold to the guardrail line.
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Verify that function returns false if ghost candidate and host are at the same position (degenerate case).
 */
TEST(f360_Is_Ghost_Reflected_Host_Mirror_Without_SEP, Is_Ghost_Reflected_Host_Mirror_Without_SEP__Ghost_And_Host_Same_Position)
{
   /** \precond
    * In test group the following have been set up
    * - Ghost candidate and host set to the same position
    */
   ghost_candidate.bbox.Set_Center(Point{ -2.5F, 0.0F }); // Host center

   /** \action
    * Call Is_Ghost_Reflected_Host_Mirror_Without_SEP
    */
   bool result = Is_Ghost_Reflected_Host_Mirror_Without_SEP(
      ghost_candidate,
      tracker_info,
      object_tracks,
      host);

   /** \result
    * Should return false as ghost candidate and host are at the same position.
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Verify that function uses the correct speed difference threshold for high curvature (curvature >= 0.04).
 * Should return true if ghost and host speeds are similar within 25% threshold.
 */
TEST(f360_Is_Ghost_Reflected_Host_Mirror_Without_SEP, Is_Ghost_Reflected_Host_Mirror_Without_SEP__High_Curvature_Threshold)
{
   /** \precond
    * In test group the following have been set up
    * - Ghost candidate, host, tracker_info, and object_tracks initialized with default values
    * - Host curvature_rear set to high value (>= 0.04)
    * - Ghost candidate speed within 25% of host speed
    * - >2 stationary objects near the predicted guardrail line
    */
   host.curvature_rear = 0.05F; // High curvature
   host.speed = 2.0F;
   ghost_candidate.speed = 2.4F; // 20% difference, within 25% threshold

   /** \action
    * Call Is_Ghost_Reflected_Host_Mirror_Without_SEP
    */
   bool result = Is_Ghost_Reflected_Host_Mirror_Without_SEP(
      ghost_candidate,
      tracker_info,
      object_tracks,
      host);

   /** \result
    * Should return true as speeds are within 25% threshold for high curvature and >2 stationary objects are present.
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Verify that function uses interpolated speed difference threshold for curvature between 0 and 0.04.
 * Should return true if ghost and host speeds are similar within the calculated threshold.
 * Host curvature is set to positive value.
 */
TEST(f360_Is_Ghost_Reflected_Host_Mirror_Without_SEP, Is_Ghost_Reflected_Host_Mirror_Without_SEP__Pos_Curvature_Interpolated_Threshold)
{
   /** \precond
    * In test group the following have been set up
    * - Ghost candidate, host, tracker_info, and object_tracks initialized with default values
    * - Host curvature_rear set to a value between 0 and 0.04 (e.g. 0.02)
    * - Ghost candidate speed within the interpolated threshold
    * - >2 stationary objects near the predicted guardrail line
    */
   host.curvature_rear = 0.02F; // Midway between 0 and 0.04
   host.speed = 2.0F;
   // For curvature=0.02, threshold = 0.1 + (0.25-0.1)*0.5 = 0.175
   // So, max allowed speed difference = 0.175 * 2.0 = 0.35
   ghost_candidate.speed = 2.3F; // 0.3 difference, within threshold

   /** \action
    * Call Is_Ghost_Reflected_Host_Mirror_Without_SEP
    */
   bool result = Is_Ghost_Reflected_Host_Mirror_Without_SEP(
      ghost_candidate,
      tracker_info,
      object_tracks,
      host);

   /** \result
    * Should return true as speeds are within interpolated threshold for curvature between 0 and 0.04 and >2 stationary objects are present.
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Verify that function uses interpolated speed difference threshold for curvature between 0 and 0.04.
 * Should return true if ghost and host speeds are similar within the calculated threshold.
 * Host curvature is set to negative value.
 */
TEST(f360_Is_Ghost_Reflected_Host_Mirror_Without_SEP, Is_Ghost_Reflected_Host_Mirror_Without_SEP__Neg_Curvature_Interpolated_Threshold)
{
   /** \precond
    * In test group the following have been set up
    * - Ghost candidate, host, tracker_info, and object_tracks initialized with default values
    * - Host curvature_rear set to a value between 0 and -0.04 (e.g. -0.02) to cover negative curvature case
    * - Ghost candidate speed within the interpolated threshold
    * - >2 stationary objects near the predicted guardrail line
    */
	host.curvature_rear = -0.02F; // Midway between 0 and -0.04 covering negative curvature case
   host.speed = 2.0F;
	// For curvature = -0.02, threshold = 0.1 + (0.25-0.1)*0.5 = 0.175 (same as positive curvature case since abs curvature is used)
   // So, max allowed speed difference = 0.175 * 2.0 = 0.35
   ghost_candidate.speed = 2.3F; // 0.3 difference, within threshold

   /** \action
    * Call Is_Ghost_Reflected_Host_Mirror_Without_SEP
    */
   bool result = Is_Ghost_Reflected_Host_Mirror_Without_SEP(
      ghost_candidate,
      tracker_info,
      object_tracks,
      host);

   /** \result
    * Should return true as speeds are within interpolated threshold for curvature between 0 and 0.04 and >2 stationary objects are present.
    */
   CHECK_TRUE(result);
}

/** \purpose
 * Verify that function returns false if guardrail candidates have otg_height >= 5.0F (too high).
 */
TEST(f360_Is_Ghost_Reflected_Host_Mirror_Without_SEP, Is_Ghost_Reflected_Host_Mirror_Without_SEP__Candidates_Otg_Height_Too_High)
{
   /** \precond
    * In test group the following have been set up
    * - Ghost candidate, host, tracker_info, and object_tracks initialized with default values
    * - All guardrail candidates have otg_height set to 5.0F (threshold) or higher
    * - All other conditions for reflective guardrail are met
    */
   for (int i = 0; i < 5; ++i)
   {
      object_tracks[i + 1].otg_height = 5.0F; // Set to threshold value (should fail < 5.0F)
   }

   /** \action
    * Call Is_Ghost_Reflected_Host_Mirror_Without_SEP
    */
   bool result = Is_Ghost_Reflected_Host_Mirror_Without_SEP(
      ghost_candidate,
      tracker_info,
      object_tracks,
      host);

   /** \result
    * Should return false as all guardrail candidates have otg_height >= 5.0F.
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Verify that function returns false when all guardrail objects are on one side of the line joining host and ghost candidate.
 */
TEST(f360_Is_Ghost_Reflected_Host_Mirror_Without_SEP, Is_Ghost_Reflected_Host_Mirror_Without_SEP__Candidates_On_Same_Side)
{
   /** \precond
    * In test group the following have been set up
    * - Ghost candidate, host, tracker_info, and object_tracks initialized with default values
    * - >2 stationary objects near the predicted guardrail line
	 * - But all stationary objects are on one side of the line joining host and ghost candidate
    */
   for (int i = 0; i < 5; ++i)
   {
      int idx = i + 1; // Avoid obj_idx (0)
      object_tracks[idx].bbox.Set_Center(Point{ 5.0F, -2.5F + 0.3F * i });
      object_tracks[idx].bbox.Set_Orientation(Angle{ 0.0F });
   }
    /** \action
     * Call Is_Ghost_Reflected_Host_Mirror_Without_SEP
     */
   bool result = Is_Ghost_Reflected_Host_Mirror_Without_SEP(
      ghost_candidate,
      tracker_info,
      object_tracks,
      host);

   /** \result
	 * Should return false as there are >2 stationary objects near the predicted guardrail line but all on one side of the line joining host and ghost candidate.
    */
   CHECK_FALSE(result);
}

/** \purpose
 * Verify that function returns false when all guardrail objects are on too far from each other.
 */
TEST(f360_Is_Ghost_Reflected_Host_Mirror_Without_SEP, Is_Ghost_Reflected_Host_Mirror_Without_SEP__Candidates_Too_Far_From_Each_Other)
{
   /** \precond
    * In test group the following have been set up
    * - Ghost candidate, host, tracker_info, and object_tracks initialized with default values
    * - >2 stationary objects near the predicted guardrail line
	 * - But all stationary objects are too far from each other
    */
   for (int i = 0; i < 5; ++i)
   {
      int idx = i + 1; // Avoid obj_idx (0)
      object_tracks[idx].bbox.Set_Center(Point{ 5.0F, -3.5F + 3.5F * i });
      object_tracks[idx].bbox.Set_Orientation(Angle{ 0.0F });
   }
    /** \action
     * Call Is_Ghost_Reflected_Host_Mirror_Without_SEP
     */
   bool result = Is_Ghost_Reflected_Host_Mirror_Without_SEP(
      ghost_candidate,
      tracker_info,
      object_tracks,
      host);

   /** \result
	 * Should return false as there are >2 stationary objects near the predicted guardrail line but are too far from each other.
    */
   CHECK_FALSE(result);
}
/** @}*/
