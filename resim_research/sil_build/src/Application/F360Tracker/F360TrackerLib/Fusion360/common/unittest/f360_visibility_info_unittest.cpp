/** \file
 * This file contains unit tests for content of f360_visibility_info.cpp file
 */

#include "f360_visibility_info.h"
#include "f360_internal_preprocessing.h"
#include "f360_math_func.h"
#include "f360_math.h"
#include <CppUTest/TestHarness.h>

using namespace f360_variant_A;

/** \defgroup  f360_visibility_info
 *  @{
 */

 /** \brief
  * Test group of Can_Object_Be_Detected_By_Sensors() function. Tests verify whether
  * function properly determines whether object can be seen by any of available sensors.
  */
TEST_GROUP(can_object_be_detected_by_sensors)
{
   /** \setup
    * Set up common variables to use in tests
    * Set selected sensor parameters to have:
    * - azimuth view range: -pi/+pi
    * - view range: 50 [m]
    */
   F360_Object_Track_T obj{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};

   TEST_SETUP()
   {
      sensors[0].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[0].constant.range_limits[F360_DET_LOOK_ID_0] = 100.0F;
      sensors[0].refined.left_fov_normal[F360_DET_LOOK_ID_0] = F360_PI;
      sensors[0].refined.right_fov_normal[F360_DET_LOOK_ID_0] = F360_PI;
   }

};

/** \purpose
 * Purpose of this test is to verify whether when there are no valid sensors object is not marked as visible by sensor.
 * \req
 * NA.
 */
TEST(can_object_be_detected_by_sensors, can_object_be_detected_by_sensors__no_valid_sensors)
{
   /** \precond
    * Set all sensors status to invalid
    */
   for (int32_t i = 0; i < MAX_NUMBER_OF_SENSORS; i++)
   {
      sensors[i].variable.is_valid = false;
   }

   /** \action
    * Call tested function
    */
   const bool f_visible = Can_Object_Be_Detected_By_Sensors(obj, sensors);

   /** \result
    * Check whether returned value is false
    */
   CHECK_FALSE(f_visible);
}

/** \purpose
* Purpose of this test is to verify whether when object is visible by at least one sensor
* function returns true
* \req
* NA.
*/
TEST(can_object_be_detected_by_sensors, can_object_be_detected_by_sensors__visible_by_one_sensor)
{
   /** \precond
   * Set selected sensor status to valid
   * Place object inside sensor FOV
   */
   sensors[0].variable.is_valid = true;

   obj.vcs_position.x = 20.0F;
   obj.vcs_position.y = 0.0F;

   /** \action
   * Call tested function
   */
   const bool f_visible = Can_Object_Be_Detected_By_Sensors(obj, sensors);

   /** \result
   * Check whether returned value is false
   */
   CHECK_TRUE(f_visible);
}
/** @}*/



/** \defgroup  is_rel_point_below_given_range_limit
 *  @{
 */

 /** \brief
  * Test group of Is_Rel_Point_Below_Given_Range_Limit() function. Tests verify whether
  * function properly determines whether object is within object view range.
  */
TEST_GROUP(is_rel_point_below_given_range_limit)
{
   /** \setup
   * Set up common vaiables
   */
   float32_t rel_posn_lon{};
   float32_t rel_posn_lat{};
   float32_t range_limit{};
};

/** \purpose
* Purpose of this test is to verify whether when object is above sensor range limit function returns false
* \req
* NA.
*/
TEST(is_rel_point_below_given_range_limit, is_rel_point_below_given_range_limit__object_out_of_sensor_view_range)
{
   /** \precond
   * Set up sensor range limit to 100.0F
   * Set up object vcs position at 120.0F [m] longitudinal
   */
   range_limit = 100.0F;
   rel_posn_lon = 120.0F;
   rel_posn_lat = 0.0F;

   /** \action
   * Call tested function
   */
   const bool f_below_range_limit = Is_Rel_Point_Below_Given_Range_Limit(rel_posn_lon, rel_posn_lat, range_limit);

   /** \result
   * Check whether returned value is false
   */
   CHECK_FALSE(f_below_range_limit)
}

/** \purpose
* Purpose of this test is to verify whether when object is below sensor range limit function returns true
* \req
* NA.
*/
TEST(is_rel_point_below_given_range_limit, is_rel_point_below_given_range_limit__object_in_sensor_view_range)
{
   /** \precond
   * Set up sensor range limit to 100.0F
   * Set up object vcs position at 80.0F [m] longitudinal
   */
   range_limit = 100.0F;
   rel_posn_lon = 80.0F;
   rel_posn_lat = 0.0F;

   /** \action
   * Call tested function
   */
   const bool f_below_range_limit = Is_Rel_Point_Below_Given_Range_Limit(rel_posn_lon, rel_posn_lat, range_limit);

   /** \result
   * Check whether returned value is true
   */
   CHECK_TRUE(f_below_range_limit)
}
/** @}*/



/** \defgroup  is_rel_point_within_fov_azim
 *  @{
 */

 /** \brief
  * Test group of Is_Rel_Point_Within_FOV_Azim() function. Tests verify whether
  * function properly determines whether object is within object azimuth view range.
  */
TEST_GROUP(is_rel_point_within_fov_azim)
{
   /** \setup
   * Set up common vaiables
   */
   float32_t rel_posn_lon{};
   float32_t rel_posn_lat{};
   F360_Det_Range_Type_T range_type{};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
   F360_TRKR_TIMING_INFO_T timing_info{};
   const float32_t azim_th = F360_PI / 4;

   TEST_SETUP()
   {
      F360_Radar_Sensor_T &sensor_0 =  sensors[0];
      for (int i = 0; i < 4; i++)
      {
         sensor_0.constant.fov_min_az_rad[i] = -azim_th;
         sensor_0.constant.fov_max_az_rad[i] = azim_th;
      }

      const float min_fov_az_angle_lr = std::min(sensor_0.constant.fov_min_az_rad[F360_DET_LOOK_ID_0], sensor_0.constant.fov_min_az_rad[F360_DET_LOOK_ID_1]);
      const float min_fov_az_interior_angle_lr = min_fov_az_angle_lr;
      const float max_fov_az_angle_lr = std::max(sensor_0.constant.fov_max_az_rad[F360_DET_LOOK_ID_0], sensor_0.constant.fov_max_az_rad[F360_DET_LOOK_ID_1]);
      const float max_fov_az_interior_angle_lr = max_fov_az_angle_lr;
      const float min_fov_az_angle_mr = std::min(sensor_0.constant.fov_min_az_rad[F360_DET_LOOK_ID_2], sensor_0.constant.fov_min_az_rad[F360_DET_LOOK_ID_3]);
      const float min_fov_az_interior_angle_mr = min_fov_az_angle_mr;
      const float max_fov_az_angle_mr = std::max(sensor_0.constant.fov_max_az_rad[F360_DET_LOOK_ID_2], sensor_0.constant.fov_max_az_rad[F360_DET_LOOK_ID_3]);
      const float max_fov_az_interior_angle_mr = max_fov_az_angle_mr;

      sensor_0.refined.interior_fov[F360_DET_LOOK_ID_0] = min_fov_az_interior_angle_lr;
      sensor_0.refined.interior_fov[F360_DET_LOOK_ID_1] = max_fov_az_interior_angle_lr;
      sensor_0.refined.interior_fov[F360_DET_LOOK_ID_2] = min_fov_az_interior_angle_mr;
      sensor_0.refined.interior_fov[F360_DET_LOOK_ID_3] = max_fov_az_interior_angle_mr;

      const float min_fov_vcs_az_angle_lr = sensor_0.constant.mounting_position.vcs_boresight_azimuth_angle + min_fov_az_interior_angle_lr;
      const float max_fov_vcs_az_angle_lr = sensor_0.constant.mounting_position.vcs_boresight_azimuth_angle + max_fov_az_interior_angle_lr;
      const float min_fov_vcs_az_angle_mr = sensor_0.constant.mounting_position.vcs_boresight_azimuth_angle + min_fov_az_interior_angle_mr;
      const float max_fov_vcs_az_angle_mr = sensor_0.constant.mounting_position.vcs_boresight_azimuth_angle + max_fov_az_interior_angle_mr;

      sensor_0.refined.left_fov_normal[F360_DET_LOOK_ID_0] = -F360_Sinf(min_fov_vcs_az_angle_lr);
      sensor_0.refined.left_fov_normal[F360_DET_LOOK_ID_1] = F360_Cosf(min_fov_vcs_az_angle_lr);
      sensor_0.refined.right_fov_normal[F360_DET_LOOK_ID_0] = F360_Sinf(max_fov_vcs_az_angle_lr);
      sensor_0.refined.right_fov_normal[F360_DET_LOOK_ID_1] = -F360_Cosf(max_fov_vcs_az_angle_lr);
      sensor_0.refined.left_fov_normal[F360_DET_LOOK_ID_2] = -F360_Sinf(min_fov_vcs_az_angle_mr);
      sensor_0.refined.left_fov_normal[F360_DET_LOOK_ID_3] = F360_Cosf(min_fov_vcs_az_angle_mr);
      sensor_0.refined.right_fov_normal[F360_DET_LOOK_ID_2] = F360_Sinf(max_fov_vcs_az_angle_mr);
      sensor_0.refined.right_fov_normal[F360_DET_LOOK_ID_3] = -F360_Cosf(max_fov_vcs_az_angle_mr);

      rel_posn_lon = 10.0F;
      rel_posn_lat = 0.0F;
   }
};

/** \purpose
* Purpose of this test is to verify whether when sensor is in long look and object is in sensor field of view
* function returns true
* \req
* NA.
*/
TEST(is_rel_point_within_fov_azim, is_rel_point_within_fov_azim__long_look_in_fov_returns_true)
{
   /** \precond
   * Set up object position to be placed in front of sensor.
   * Set range type as long
   */
   range_type = F360_DET_RANGE_TYPE_LONG;
   rel_posn_lat = 0.0F;

   /** \action
   * Call tested function
   */
   const bool f_is_in_fov_azim = Is_Rel_Point_Within_FOV_Azim(rel_posn_lon, rel_posn_lat, sensors[0], range_type);

   /** \result
   * Check whether returned value is true
   */
   CHECK_TRUE(f_is_in_fov_azim);
}

/** \purpose
* Purpose of this test is to verify whether when sensor is in long look and object is not in sensor
* right azimuth fov, function returns false.
* \req
* NA.
*/
TEST(is_rel_point_within_fov_azim, is_rel_point_within_fov_azim__long_look_out_of_right_fov_returns_false)
{
   /** \precond
   * Set up object longitudinal position to be placed in front of sensor.
   * Set up object lateral position to be placed above sensor right fov edge
   * Set range type as long
   */
   range_type = F360_DET_RANGE_TYPE_LONG;
   rel_posn_lat = rel_posn_lon * F360_Tanf(azim_th) + 0.1F;

   /** \action
   * Call tested function
   */
   const bool f_is_in_fov_azim = Is_Rel_Point_Within_FOV_Azim(rel_posn_lon, rel_posn_lat, sensors[0], range_type);

   /** \result
   * Check whether returned value is false
   */
   CHECK_FALSE(f_is_in_fov_azim);
}

/** \purpose
* Purpose of this test is to verify whether when sensor is in long look and object is not in sensor
* left azimuth fov, function returns false.
* \req
* NA.
*/
TEST(is_rel_point_within_fov_azim, is_rel_point_within_fov_azim__long_look_out_of_left_fov_returns_false)
{
   /** \precond
   * Set up object longitudinal position to be placed in front of sensor.
   * Set up object lateral position to be placed below sensor left fov edge
   * Set range type as long
   */
   range_type = F360_DET_RANGE_TYPE_LONG;
   rel_posn_lat = rel_posn_lon * F360_Tanf(-azim_th) - 0.1F;

   /** \action
   * Call tested function
   */
   const bool f_is_in_fov_azim = Is_Rel_Point_Within_FOV_Azim(rel_posn_lon, rel_posn_lat, sensors[0], range_type);

   /** \result
   * Check whether returned value is false
   */
   CHECK_FALSE(f_is_in_fov_azim);
}

/** \purpose
* Purpose of this test is to verify whether when sensor is in medium look and object is in sensor field of view
* function returns true
* \req
* NA.
*/
TEST(is_rel_point_within_fov_azim, is_rel_point_within_fov_azim__medium_look_in_fov_returns_true)
{
   /** \precond
   * Set up object position to be placed in front of sensor.
   * Set range type as medium
   */
   range_type = F360_DET_RANGE_TYPE_MEDIUM;
   rel_posn_lon = 10.0F;
   rel_posn_lat = 0.0F;

   /** \action
   * Call tested function
   */
   const bool f_is_in_fov_azim = Is_Rel_Point_Within_FOV_Azim(rel_posn_lon, rel_posn_lat, sensors[0], range_type);

   /** \result
   * Check whether returned value is true
   */
   CHECK_TRUE(f_is_in_fov_azim);
}

/** \purpose
* Purpose of this test is to verify whether when sensor is in long look and object is not in sensor
* right azimuth fov, function returns false.
* \req
* NA.
*/
TEST(is_rel_point_within_fov_azim, is_rel_point_within_fov_azim__medium_look_out_of_right_fov_returns_false)
{
   /** \precond
   * Set up object longitudinal position to be placed in front of sensor.
   * Set up object lateral position to be placed above sensor right fov edge
   * Set range type as medium
   */
   range_type = F360_DET_RANGE_TYPE_MEDIUM;
   rel_posn_lat = rel_posn_lon * F360_Tanf(azim_th) + 0.1F;

   /** \action
   * Call tested function
   */
   const bool f_is_in_fov_azim = Is_Rel_Point_Within_FOV_Azim(rel_posn_lon, rel_posn_lat, sensors[0], range_type);

   /** \result
   * Check whether returned value is false
   */
   CHECK_FALSE(f_is_in_fov_azim);
}

/** \purpose
* Purpose of this test is to verify whether when sensor is in long look and object is not in sensor
* left azimuth fov, function returns false.
* \req
* NA.
*/
TEST(is_rel_point_within_fov_azim, is_rel_point_within_fov_azim__medium_look_out_of_left_fov_returns_false)
{
   /** \precond
   * Set up object longitudinal position to be placed in front of sensor.
   * Set up object lateral position to be placed below sensor left fov edge
   * Set range type as medium
   */
   range_type = F360_DET_RANGE_TYPE_MEDIUM;
   rel_posn_lat = rel_posn_lon * F360_Tanf(-azim_th) - 0.1F;

   /** \action
   * Call tested function
   */
   const bool f_is_in_fov_azim = Is_Rel_Point_Within_FOV_Azim(rel_posn_lon, rel_posn_lat, sensors[0], range_type);

   /** \result
   * Check whether returned value is false
   */
   CHECK_FALSE(f_is_in_fov_azim);
}
/** @}*/



/** \defgroup  is_vcs_point_within_current_fov_of_sensor
 *  @{
 */

 /** \brief
  * Test group of Is_VCS_Point_Within_Current_FOV_Of_Sensor() function. Tests verify whether
  * function properly determines whether object is within object fov.
  */
TEST_GROUP(is_vcs_point_within_current_fov_of_sensor)
{
   /** \setup
   * Set up common vaiables
   */
   Point point_pos;
   F360_Radar_Sensor_T sensor;

   TEST_SETUP()
   {
      sensor.variable.look_id = F360_DET_LOOK_ID_0;
      sensor.constant.range_limits[sensor.variable.look_id] = 50.0F;
      sensor.refined.left_fov_normal[F360_DET_LOOK_ID_0] = 0.5F;
      sensor.refined.right_fov_normal[F360_DET_LOOK_ID_0] = 0.5F;
   }
};

/** \purpose
* Purpose of this test is to verify whether when sensor is sensor field of fiew
* function returns true.
* \req
* NA.
*/
TEST(is_vcs_point_within_current_fov_of_sensor, is_vcs_point_within_current_fov_of_sensor__is_in_fov_returns_true)
{
   /** \precond
   * Set up object position to be placed in sensor field of view
   */
   point_pos.x = 10.0F;
   point_pos.y = 0.0F;

   /** \action
   * Call tested function
   */
   const bool f_visible = Is_VCS_Point_Within_Current_FOV_Of_Sensor(point_pos, sensor, sensor.variable.look_id);

   /** \result
   * Check whether returned value is true
   */
   CHECK_TRUE(f_visible);

}

/** \purpose
* Purpose of this test is to verify whether when object has too high range
* function returns false.
* \req
* NA.
*/
TEST(is_vcs_point_within_current_fov_of_sensor, is_vcs_point_within_current_fov_of_sensor__too_high_range)
{
   /** \precond
   * Set up object position to be above sensor view range
   */
   point_pos.x = sensor.constant.range_limits[sensor.variable.look_id] + 10.0F;
   point_pos.y = 0.0F;

   /** \action
   * Call tested function
   */
   const bool f_visible = Is_VCS_Point_Within_Current_FOV_Of_Sensor(point_pos, sensor, sensor.variable.look_id);

   /** \result
   * Check whether returned value is false
   */
   CHECK_FALSE(f_visible);

}

/** \purpose
* Purpose of this test is to verify whether when object is not in sensor
* azimuth view function returns false.
* \req
* NA.
*/
TEST(is_vcs_point_within_current_fov_of_sensor, is_vcs_point_within_current_fov_of_sensor__out_of_sensor_azimuth_view_range)
{
   /** \precond
   * Set up object position to be above sensor azimuth view range
   */
   point_pos.x = 10.0F;
   point_pos.y = 100.0F;

   /** \action
   * Call tested function
   */
   const bool f_visible = Is_VCS_Point_Within_Current_FOV_Of_Sensor(point_pos, sensor, sensor.variable.look_id);

   /** \result
   * Check whether returned value is false
   */
   CHECK_FALSE(f_visible);

}
/** @}*/

/** \defgroup  Is_Object_In_Side_Blindzone
 *  @{
 */

 /** \brief
  * Test group of Is_Object_In_Side_Blindzone() function. Tests verify whether
  * function properly determines if the object is in a side blindzoe where sensors
  * can not detect it properly. It performs only for 4-corner sensors and the object
  * is between the front and rear sensor.
  */
TEST_GROUP(is_object_in_side_blindzone)
{
    F360_Object_Track_T obj{};
    F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS]{};
    F360_Globals_T globals{};

    /** \setup
    * Set up the sensors to represent 4-corner radar configuration
    * Set the object position and properties
    * Set up the required globals' properties 
    */
   TEST_SETUP()
   {
      sensors[0].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[1].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[2].variable.look_id = F360_DET_LOOK_ID_0;
      sensors[3].variable.look_id = F360_DET_LOOK_ID_0;

      // Set the normals for all 4 sensors
      sensors[0U].refined.left_fov_normal[0U] = 0.0524F;
      sensors[0U].refined.right_fov_normal[0U] = 0.7986F;
      sensors[0U].refined.left_fov_normal[1U] = 0.9986F;
      sensors[0U].refined.right_fov_normal[1U] = 0.6018F;
      sensors[0U].refined.left_fov_normal[2U] = -3.3173F;
      sensors[0U].refined.right_fov_normal[2U] = 1.9636F;
      sensors[0U].refined.left_fov_normal[3U] = 6.9483F;
      sensors[0U].refined.right_fov_normal[3U] = 3.2673F;

      sensors[1U].refined.left_fov_normal[0U] = 0.7986F;
      sensors[1U].refined.right_fov_normal[0U] = 0.0524F;
      sensors[1U].refined.left_fov_normal[1U] = -0.6018;
      sensors[1U].refined.right_fov_normal[1U] = -0.9986F;
      sensors[1U].refined.left_fov_normal[2U] =  0.0001F;
      sensors[1U].refined.right_fov_normal[2U] =  0.0001F;
      sensors[1U].refined.left_fov_normal[3U] =  0.0001F;
      sensors[1U].refined.right_fov_normal[3U] =  0.0001F;

      sensors[2U].refined.left_fov_normal[0U] = -0.9397F;
      sensors[2U].refined.right_fov_normal[0U] = -0.3421F;
      sensors[2U].refined.left_fov_normal[1U] = 0.3421F;
      sensors[2U].refined.right_fov_normal[1U] = 0.9397;
      sensors[2U].refined.left_fov_normal[2U] = -2.09997F;
      sensors[2U].refined.right_fov_normal[2U] = 3.0261F;
      sensors[2U].refined.left_fov_normal[3U] = 6.0261F;
      sensors[2U].refined.right_fov_normal[3U] = 3.9997F;

      sensors[3U].refined.left_fov_normal[0U] = -0.3421F;
      sensors[3U].refined.right_fov_normal[0U] = -0.9397F;
      sensors[3U].refined.left_fov_normal[1U] = -0.9397F;
      sensors[3U].refined.right_fov_normal[1U] = -0.3421F;
      sensors[3U].refined.left_fov_normal[2U] =  0.0001F;
      sensors[3U].refined.right_fov_normal[2U] =  0.0001F;
      sensors[3U].refined.left_fov_normal[3U] =  0.0001F;
      sensors[3U].refined.right_fov_normal[3U] =  0.0001F;

    // Make the first 4 sensors valid
    for (int32_t i = 0U; i < 4; i++)
    {
       sensors[i].variable.is_valid = true;
    }

    // Make all the rest sensors invalid
    for (int32_t i = 4; i < MAX_NUMBER_OF_SENSORS; i++)
    {
       sensors[i].variable.is_valid = false;
    }

    // Mounting positions for 4 sensors (VCS coordinates)
    // Front right
    sensors[0].constant.mounting_position.vcs_position.longitudinal = 0.0F;
    sensors[0].constant.mounting_position.vcs_position.lateral = 1.0F;

    // Front left
    sensors[1].constant.mounting_position.vcs_position.longitudinal = 0.0F;
    sensors[1].constant.mounting_position.vcs_position.lateral = -1.0F;

    // Rear right
    sensors[2].constant.mounting_position.vcs_position.longitudinal = -5.0F;
    sensors[2].constant.mounting_position.vcs_position.lateral = 1.0F;

    // Rear left
    sensors[3].constant.mounting_position.vcs_position.longitudinal = -5.0F;
    sensors[3].constant.mounting_position.vcs_position.lateral = -1.0F;
    // Set object properties so its reference point lies between sensors
    // Place at mid-longitudinal between x=0 and x=-5, and mid-lateral between y=10 and y=-10
    obj.vcs_position.x = -2.0F;
    obj.vcs_position.y = 0.5F;
    obj.vcs_heading = Angle{ 0.0F };
    obj.bbox.Set_Length(1.0F);
    obj.bbox.Set_Width(1.0F);
    obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
    obj.Set_Bbox_Orientation(obj.vcs_heading);

    // Set up the globals
    globals.four_corner_sensor_config_min_longitudinal_position = sensors[2].constant.mounting_position.vcs_position.longitudinal;
    globals.four_corner_sensor_config_max_longitudinal_position = sensors[0].constant.mounting_position.vcs_position.longitudinal;

    globals.f_four_corner_sensors_available = true;
}

};

/** \purpose
* Purpose of this test is to verify that object is correctly identified as being in side blindzone
* \req
* NA.
*/
TEST(is_object_in_side_blindzone, object_is_in_side_blindzone)
{
   /** \precond
   * The default set up from the test group is being used
   */

   /** \action
   * Call Is_Object_In_Side_Blindzone
   */
   const bool f_object_in_side_blindzone = Is_Object_In_Side_Blindzone(obj, sensors, globals);

   /** \result
   * Check whether returned value is true
   */
   CHECK_TRUE(f_object_in_side_blindzone);
}

/** \purpose
* Purpose of this test is to verify that object is correctly identified as not being in side blindzone
* eventhough is between the front and rear sensors
* \req
* NA.
*/
TEST(is_object_in_side_blindzone, object_is_not_in_side_blindzone)
{
   /** \precond
   * Set the y position of the object to be outside of the side blindzone
   */
   obj.vcs_position.y = 5.0F; // Move object out of side blindzone

   /** \action
   * Call Is_Object_In_Side_Blindzone
   */
   const bool f_object_in_side_blindzone = Is_Object_In_Side_Blindzone(obj, sensors, globals);

   /** \result
   * Check whether returned value is false
   */
   CHECK_FALSE(f_object_in_side_blindzone);
}


/** \purpose
* Purpose of this test is to verify that object is correctly identified as not being in side blindzone
* because it is ahead of the longitudinal range of the sensors
* \req
* NA.
*/
TEST(is_object_in_side_blindzone, object_is_not_in_side_blindzone_because_is_ahead_of_longitudinal_range)
{
   /** \precond
   * Set the x position of the object to be ahead of the longitudinal range of the sensors
   */
   obj.vcs_position.x = globals.four_corner_sensor_config_max_longitudinal_position + 0.1F; // Move object out of side blindzone

   /** \action
   * Call Is_Object_In_Side_Blindzone
   */
   const bool f_object_in_side_blindzone = Is_Object_In_Side_Blindzone(obj, sensors, globals);

   /** \result
   * Check whether returned value is false
   */
   CHECK_FALSE(f_object_in_side_blindzone);
}

/** \purpose
* Purpose of this test is to verify that object is correctly identified as not being in side blindzone
* because it is behind of the longitudinal range of the sensors
* \req
* NA.
*/
TEST(is_object_in_side_blindzone, object_is_not_in_side_blindzone_because_is_behind_of_longitudinal_range)
{
   /** \precond
   * Set the x position of the object to be behind of the longitudinal range of the sensors
   */
   obj.vcs_position.x = globals.four_corner_sensor_config_min_longitudinal_position - 0.1F; // Move object out of side blindzone

   /** \action
   * Call Is_Object_In_Side_Blindzone
   */
   const bool f_object_in_side_blindzone = Is_Object_In_Side_Blindzone(obj, sensors, globals);

   /** \result
   * Check whether returned value is false
   */
   CHECK_FALSE(f_object_in_side_blindzone);
}


/** \purpose
* Purpose of this test is to verify that object is correctly identified not being in side blindzone
* while the sensors have different look IDs set. If the look IDs are set to 2, the object should
* not be in the blindzone.
* The FOVes for the sensors with look_id = 2 is much narrower and as a result the object with the same 
* position as in the test set up, is not in the blindzone anymore
* \req
* NA.
*/
TEST(is_object_in_side_blindzone, object_is_not_in_side_blindzone_with_different_look_ids)
{
   /** \precond
   * Set the look IDs of all sensors to 1 so it uses the other fov normals
   */
   sensors[0].variable.look_id = F360_DET_LOOK_ID_2;
   sensors[1].variable.look_id = F360_DET_LOOK_ID_2;
   sensors[2].variable.look_id = F360_DET_LOOK_ID_2;
   sensors[3].variable.look_id = F360_DET_LOOK_ID_2;

   /** \action
   * Call Is_Object_In_Side_Blindzone
   */
   const bool f_object_in_side_blindzone = Is_Object_In_Side_Blindzone(obj, sensors, globals);

   /** \result
   * Check whether returned value is false
   */
   CHECK_FALSE(f_object_in_side_blindzone);
}

/** \purpose
* Purpose of this test is to verify that object is correctly not identified as being in side blindzone
* because none of the four-corner sensors are valid
* \reqs
* NA.
*/
TEST(is_object_in_side_blindzone, object_is_not_in_side_blindzone_because_no_sensor_is_valid)
{
   /** \precond
   * Set all the sensors to be invalid
   */
   // Make all the sensors valid
   for (int32_t i = 0; i < 4; i++)
   {
      sensors[i].variable.is_valid = false;
   }

   /** \action
   * Call Is_Object_In_Side_Blindzone
   */
   const bool f_object_in_side_blindzone = Is_Object_In_Side_Blindzone(obj, sensors, globals);

   /** \result
   * Check whether returned value is false
   */
   CHECK_FALSE(f_object_in_side_blindzone);
}

/** \purpose
* Purpose of this test is to verify that object is correctly not identified as being in side blindzone
* because flag f_four_corner_sensors_available is false
* \reqs
* NA.
*/
TEST(is_object_in_side_blindzone, object_is_not_in_side_blindzone_because_f_four_corner_sensors_available_is_false)
{
    /** \precond
    * Set flag f_four_corner_sensors_available to false
    */
    globals.f_four_corner_sensors_available = false;

    /** \action
    * Call Is_Object_In_Side_Blindzone
    */
    const bool f_object_in_side_blindzone = Is_Object_In_Side_Blindzone(obj, sensors, globals);

    /** \result
    * Check whether returned value is false
    */
    CHECK_FALSE(f_object_in_side_blindzone);
}
/** @}*/