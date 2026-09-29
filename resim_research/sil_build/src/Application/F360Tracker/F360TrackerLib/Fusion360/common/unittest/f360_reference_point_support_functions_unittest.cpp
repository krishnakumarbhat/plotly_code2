/** \file
 * This file contains unit tests for content of f360_reference_point_support_functions.cpp file
 */

#include "f360_reference_point_support_functions.h"
#include <CppUTest/TestHarness.h>

//#include "headerfile_needed.h"

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  Is_Point_Inside_FOV
 *  @{
 */

/** \brief
  *  Testing function that checks if a given point is within the specified FOV
  * \req
  * NA
  **/
TEST_GROUP(Is_Point_Inside_FOV)
{
   Point position = {};
   F360_Radar_Sensor_T sensor = {};
   float32_t rotated_left_fov_normal[2] = {};
   float32_t rotated_right_fov_normal[2] = {};

   TEST_SETUP()
   {
      position.x = 0.0F;
      position.y = 0.0F;

      sensor.constant.mounting_position.vcs_position.longitudinal = 0.0F;
      sensor.constant.mounting_position.vcs_position.lateral = 0.0F;
      
      rotated_left_fov_normal[0] = 0.707106781186548F; // unit vector pointing 45 deg vcs
      rotated_left_fov_normal[1] = 0.707106781186548F;
      
      rotated_right_fov_normal[0] = 0.707106781186548F; // unit vector pointing -45 deg vcs
      rotated_right_fov_normal[1] = -0.707106781186548F;
   }
};

/**
  *\purpose
  * Verify function Is_Point_Inside_FOV returns true when a point is inside both the left and right FoV borders with the sensor at origin
  *\req    NA
  **/
TEST(Is_Point_Inside_FOV, Sensor_At_Origo_Point_Inside_FOV)
{
   /** \precond
    * Set position.x = 1
    **/
   position.x = 1.0F;

   /** \action
     * Call function Is_Point_Inside_FOV()
     **/
   const bool result = Is_Point_Inside_FOV(position, sensor, rotated_left_fov_normal, rotated_right_fov_normal);

  /** \result
    * Check that the function returned true
    **/
  CHECK_TRUE_TEXT(result,"Point is classified as outside field of view when it is expected to be on the inside")
}

/**
  *\purpose
  * Verify function Is_Point_Inside_FOV returns false when a point is inside the left but not the right FoV borders with the sensor at origin
  *\req    NA
  **/
TEST(Is_Point_Inside_FOV, Sensor_At_Origo_Point_Inside_Left_But_Not_Right_FOV)
{
   /** \precond
    * Set position.y = 1
    **/
   position.y = 1.0F;

   /** \action
     * Call function Is_Point_Inside_FOV()
     **/
   const bool result = Is_Point_Inside_FOV(position, sensor, rotated_left_fov_normal, rotated_right_fov_normal);

  /** \result
    * Check that the function returned false
    **/
  CHECK_FALSE_TEXT(result,"Point is classified as inside field of view when it is expected to be on the outside")
}

/**
  *\purpose
  * Verify function Is_Point_Inside_FOV returns false when a point is inside the right but not the left FoV borders with the sensor at origin
  *\req    NA
  **/
TEST(Is_Point_Inside_FOV, Sensor_At_Origo_Point_Inside_Right_But_Not_Left_FOV)
{
   /** \precond
    * Set position.y = -1
    **/
   position.y = -1.0F;

   /** \action
     * Call function Is_Point_Inside_FOV()
     **/
   const bool result = Is_Point_Inside_FOV(position, sensor, rotated_left_fov_normal, rotated_right_fov_normal);

  /** \result
    * Check that the function returned false
    **/
  CHECK_FALSE_TEXT(result,"Point is classified as inside field of view when it is expected to be on the outside")
}

/**
  *\purpose
  * Verify function Is_Point_Inside_FOV returns false when a point is outside both left and right FoV borders with the sensor at origin
  *\req    NA
  **/
TEST(Is_Point_Inside_FOV, Sensor_At_Origo_Point_Outside_FOV)
{
   /** \precond
    * Set position.x = -1
    **/
   position.x = -1.0F;

   /** \action
     * Call function Is_Point_Inside_FOV()
     **/
   const bool result = Is_Point_Inside_FOV(position, sensor, rotated_left_fov_normal, rotated_right_fov_normal);

  /** \result
    * Check that the function returned false
    **/
  CHECK_FALSE_TEXT(result,"Point is classified as inside field of view when it is expected to be on the outside")
}

/**
  *\purpose
  * Verify function Is_Point_Inside_FOV returns true when a point is inside both the left and right FoV borders with the point at origin
  *\req    NA
  **/
TEST(Is_Point_Inside_FOV, Point_At_Origo_Point_Inside_FOV)
{
   /** \precond
    * Set sensor.constant.mounting_position.vcs_position.longitudinal = -1
    **/
   sensor.constant.mounting_position.vcs_position.longitudinal = -1.0F;

   /** \action
     * Call function Is_Point_Inside_FOV()
     **/
   const bool result = Is_Point_Inside_FOV(position, sensor, rotated_left_fov_normal, rotated_right_fov_normal);

  /** \result
    * Check that the function returned true
    **/
  CHECK_TRUE_TEXT(result,"Point is classified as outside field of view when it is expected to be on the inside")
}

/**
  *\purpose
  * Verify function Is_Point_Inside_FOV returns false when a point is inside the left but not the right FoV borders with the point at origin
  *\req    NA
  **/
TEST(Is_Point_Inside_FOV, Point_At_Origo_Point_Inside_Left_But_Not_Right_FOV)
{
   /** \precond
    * Set sensor.constant.mounting_position.vcs_position.lateral = -1
    **/
   sensor.constant.mounting_position.vcs_position.lateral = -1.0F;

   /** \action
     * Call function Is_Point_Inside_FOV()
     **/
   const bool result = Is_Point_Inside_FOV(position, sensor, rotated_left_fov_normal, rotated_right_fov_normal);

  /** \result
    * Check that the function returned false
    **/
  CHECK_FALSE_TEXT(result,"Point is classified as inside field of view when it is expected to be on the outside")
}

/**
  *\purpose
  * Verify function Is_Point_Inside_FOV returns false when a point is inside the right but not the left FoV borders with the point at origin
  *\req    NA
  **/
TEST(Is_Point_Inside_FOV, Point_At_Origo_Point_Inside_Right_But_Not_Left_FOV)
{
   /** \precond
    * Set sensor.constant.mounting_position.vcs_position.lateral = 1
    **/
   sensor.constant.mounting_position.vcs_position.lateral = 1.0F;

   /** \action
     * Call function Is_Point_Inside_FOV()
     **/
   const bool result = Is_Point_Inside_FOV(position, sensor, rotated_left_fov_normal, rotated_right_fov_normal);

  /** \result
    * Check that the function returned false
    **/
  CHECK_FALSE_TEXT(result,"Point is classified as inside field of view when it is expected to be on the outside")
}

/**
  *\purpose
  * Verify function Is_Point_Inside_FOV returns false when a point is outside both left and right FoV borders with the point at origin
  *\req    NA
  **/
TEST(Is_Point_Inside_FOV, Point_At_Origo_Point_Outside_FOV)
{
   /** \precond
    * Set sensor.constant.mounting_position.vcs_position.longitudinal = 1
    **/
   sensor.constant.mounting_position.vcs_position.longitudinal = 1.0F;

   /** \action
     * Call function Is_Point_Inside_FOV()
     **/
   const bool result = Is_Point_Inside_FOV(position, sensor, rotated_left_fov_normal, rotated_right_fov_normal);

  /** \result
    * Check that the function returned false
    **/
  CHECK_FALSE_TEXT(result,"Point is classified as inside field of view when it is expected to be on the outside")
}
/** @}*/

/** \brief
  *  Group dedicated to unit testing of Is_Ref_Point_In_Sensors_FOV()
  * \req
  * NA
  **/
TEST_GROUP(Is_Ref_Point_In_Sensors_FOV)
{
   F360_Reference_Point_T ref_point = {};
   F360_Object_Track_T object_track = {};
   F360_Radar_Sensor_T sensors[MAX_NUMBER_OF_SENSORS] = {};
   F360_Globals_T globals = {};

   /** \setup
    * 
    **/
   TEST_SETUP()
   {
      object_track.bbox.Set_Length(4.0F);
      object_track.bbox.Set_Width(2.0F);
      object_track.bbox.Set_Center(Point{0.0F,0.0F});
      object_track.bbox.Set_Orientation(Angle{F360_DEG2RAD(90.0F)});
      
      ref_point = F360_REFERENCE_POINT_LEFT;

      globals.f_single_front_center_radar_only = true;

      sensors[0].variable.is_valid = true;
      sensors[0].constant.mounting_position.vcs_position.longitudinal = 2.0F;
      sensors[0].constant.mounting_position.vcs_position.lateral = 0.0F;

      globals.rotated_left_fov_normal[0][0] = 0.707106781186548F; // unit vector pointing 45 deg vcs
      globals.rotated_left_fov_normal[0][1] = 0.707106781186548F;
      
      globals.rotated_right_fov_normal[0][0] = 0.707106781186548F; // unit vector pointing -45 deg vcs
      globals.rotated_right_fov_normal[0][1] = -0.707106781186548F;

      sensors[2].variable.is_valid = true;
      sensors[2].constant.mounting_position.vcs_position.longitudinal = 0.0F;
      sensors[2].constant.mounting_position.vcs_position.lateral = 0.0F;

      globals.rotated_left_fov_normal[2][0] = 0.707106781186548F; // unit vector pointing 45 deg vcs
      globals.rotated_left_fov_normal[2][1] = 0.707106781186548F;
      
      globals.rotated_right_fov_normal[2][0] = 0.707106781186548F; // unit vector pointing -45 deg vcs
      globals.rotated_right_fov_normal[2][1] = -0.707106781186548F;
   }
};

/**
  *\purpose
  * Verify function Is_Ref_Point_In_Sensors_FOV returns true when it is visible by 1 sensor
  *\req    NA
  **/
TEST(Is_Ref_Point_In_Sensors_FOV, Visible_By_One_Sensor)
{
   /** \precond
    * None.
    **/

   /** \action
     * Call function Is_Ref_Point_In_Sensors_FOV()
     **/
   const bool result = Is_Ref_Point_In_Sensors_FOV(ref_point, object_track, sensors, globals);

  /** \result
    * Check that the function returned true
    **/
  CHECK_TRUE_TEXT(result,"Reference is classified as outside field of view when it is expected to be on the inside")
}

/**
  *\purpose
  * Verify function Is_Ref_Point_In_Sensors_FOV returns true when it is visible by 2 sensors
  *\req    NA
  **/
TEST(Is_Ref_Point_In_Sensors_FOV, Visible_By_Two_Sensors)
{
   /** \precond
    * Set sensors[0].constant.mounting_position.vcs_position.longitudinal = 0
    **/
   sensors[0].constant.mounting_position.vcs_position.longitudinal = 0.0F;

   /** \action
     * Call function Is_Ref_Point_In_Sensors_FOV()
     **/
   const bool result = Is_Ref_Point_In_Sensors_FOV(ref_point, object_track, sensors, globals);

  /** \result
    * Check that the function returned true
    **/
  CHECK_TRUE_TEXT(result,"Reference is classified as outside field of view when it is expected to be on the inside")
}

/**
  *\purpose
  * Verify function Is_Ref_Point_In_Sensors_FOV returns false when it is not visible by any sensor
  *\req    NA
  **/
TEST(Is_Ref_Point_In_Sensors_FOV, Not_Visible_In_Any_Sensor)
{
   /** \precond
    * Set sensors[2].constant.mounting_position.vcs_position.longitudinal = 2
    **/
   sensors[2].constant.mounting_position.vcs_position.longitudinal = 2.0F;

   /** \action
     * Call function Is_Ref_Point_In_Sensors_FOV()
     **/
   const bool result = Is_Ref_Point_In_Sensors_FOV(ref_point, object_track, sensors, globals);

  /** \result
    * Check that the function returned false
    **/
  CHECK_FALSE_TEXT(result,"Reference is classified as inside field of view when it is expected to be on the outside")
}

/**
  *\purpose
  * Verify function Is_Ref_Point_In_Sensors_FOV returns false when there are no valid sensor
  *\req    NA
  **/
TEST(Is_Ref_Point_In_Sensors_FOV, No_Valid_Sensor)
{
   /** \precond
    * Set sensors[0].variable.is_valid to false
    * Set sensors[2].variable.is_valid to false
    **/
   sensors[0].variable.is_valid = false;
   sensors[2].variable.is_valid = false;

   /** \action
     * Call function Is_Ref_Point_In_Sensors_FOV()
     **/
   const bool result = Is_Ref_Point_In_Sensors_FOV(ref_point, object_track, sensors, globals);

  /** \result
    * Check that the function returned false
    **/
  CHECK_FALSE_TEXT(result,"Reference is classified as inside field of view when it is expected to be on the outside")
}

/**
  *\purpose
  * Verify function Is_Ref_Point_In_Sensors_FOV returns true when there are no valid sensor
  * but globals.f_single_front_center_radar_only is set to false
  *\req    NA
  **/
TEST(Is_Ref_Point_In_Sensors_FOV, Not_Single_Forwardlooking_Sensor_Setup_Override)
{
   /** \precond
    * Set sensors[0].variable.is_valid to false
    * Set sensors[2].variable.is_valid to false
    * Set globals.f_single_front_center_radar_only to true
    **/
   sensors[0].variable.is_valid = false;
   sensors[2].variable.is_valid = false;
   globals.f_single_front_center_radar_only = false;

   /** \action
     * Call function Is_Ref_Point_In_Sensors_FOV()
     **/
   const bool result = Is_Ref_Point_In_Sensors_FOV(ref_point, object_track, sensors, globals);

  /** \result
    * Check that the function returned True
    **/
  CHECK_TRUE_TEXT(result,"Reference is classified as outside field of view when it is expected to be on the inside")
}
/** @}*/

/** \defgroup  Get_Reference_Point_Pos_In_TCS
 *  @{
 */

 /** \brief
  * Test group of Get_Reference_Point_Pos_In_TCS function. 
  */
TEST_GROUP(Get_Reference_Point_Pos_In_TCS)
{
   /* Common setup for test.
    Create an arbitrary object. */
   F360_Reference_Point_T reference_point = F360_REFERENCE_POINT_CENTER;
   float32_t bbox_length = 0.0F;
   float32_t bbox_width = 0.0F;

   /** \setup
    * Initialize an object with the bounding box of 4 meters length and 2 meters width
    **/
   TEST_SETUP()
   {
      bbox_length = 4.0F;
      bbox_width = 2.0F;
   }
};

/**
*\purpose  Verify that reference point position in TCS is precisely calculated given F360_REFERENCE_POINT_FRONT_LEFT
*\req    NA
*/
TEST(Get_Reference_Point_Pos_In_TCS, front_left_reference_point_seen)
{
   /** \precond
    * Suppose the reference_point varaible is set to F360_REFERENCE_POINT_FRONT_LEFT
    **/
   reference_point = F360_REFERENCE_POINT_FRONT_LEFT;
   float32_t expected_ref_post_x = 0.5F * bbox_length;
   float32_t expected_ref_post_y = -0.5F * bbox_width;

   /** \action
   * Call function Get_Reference_Point_Pos_In_TCS
   **/
   Point ref_pos_tcs = Get_Reference_Point_Pos_In_TCS(reference_point, bbox_length, bbox_width);

   /** \result
   * Expect the FRONT LEFT reference point
   **/
   CHECK_EQUAL(expected_ref_post_x, ref_pos_tcs.x);
   CHECK_EQUAL(expected_ref_post_y, ref_pos_tcs.y);
}

/**
*\purpose  Verify that reference point position in TCS is precisely calculated given F360_REFERENCE_POINT_FRONT
*\req    NA
*/
TEST(Get_Reference_Point_Pos_In_TCS, front_reference_point_seen)
{
   /** \precond
    * Suppose the reference_point varaible is set to F360_REFERENCE_POINT_FRONT
    **/
   reference_point = F360_REFERENCE_POINT_FRONT;
   float32_t expected_ref_post_x = 0.5F * bbox_length;
   float32_t expected_ref_post_y = 0.0F;

   /** \action
   * Call function Get_Reference_Point_Pos_In_TCS
   **/
   Point ref_pos_tcs = Get_Reference_Point_Pos_In_TCS(reference_point, bbox_length, bbox_width);

   /** \result
   * Expect the FRONT reference point
   **/
   CHECK_EQUAL(expected_ref_post_x, ref_pos_tcs.x);
   CHECK_EQUAL(expected_ref_post_y, ref_pos_tcs.y);
}

/**
*\purpose  Verify that reference point position in TCS is precisely calculated given F360_REFERENCE_POINT_FRONT_RIGHT
*\req    NA
*/
TEST(Get_Reference_Point_Pos_In_TCS, front_right_reference_point_seen)
{
   /** \precond
    * Suppose the reference_point varaible is set to F360_REFERENCE_POINT_FRONT_RIGHT
    **/
   reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;
   float32_t expected_ref_post_x = 0.5F * bbox_length;
   float32_t expected_ref_post_y = 0.5F * bbox_width;

   /** \action
   * Call function Get_Reference_Point_Pos_In_TCS
   **/
   Point ref_pos_tcs = Get_Reference_Point_Pos_In_TCS(reference_point, bbox_length, bbox_width);

   /** \result
   * Expect the FRONT RIGHT reference point
   **/
   CHECK_EQUAL(expected_ref_post_x, ref_pos_tcs.x);
   CHECK_EQUAL(expected_ref_post_y, ref_pos_tcs.y);
}

/**
*\purpose  Verify that reference point position in TCS is precisely calculated given F360_REFERENCE_POINT_RIGHT
*\req    NA
*/
TEST(Get_Reference_Point_Pos_In_TCS, right_reference_point_seen)
{
   /** \precond
    * Suppose the reference_point varaible is set to F360_REFERENCE_POINT_RIGHT
    **/
   reference_point = F360_REFERENCE_POINT_RIGHT;
   float32_t expected_ref_post_x = 0.0F;
   float32_t expected_ref_post_y = 0.5F * bbox_width;

   /** \action
   * Call function Get_Reference_Point_Pos_In_TCS
   **/
   Point ref_pos_tcs = Get_Reference_Point_Pos_In_TCS(reference_point, bbox_length, bbox_width);

   /** \result
   * Expect the RIGHT reference point
   **/
   CHECK_EQUAL(expected_ref_post_x, ref_pos_tcs.x);
   CHECK_EQUAL(expected_ref_post_y, ref_pos_tcs.y);
}

/**
*\purpose  Verify that reference point position in TCS is precisely calculated given F360_REFERENCE_POINT_REAR_RIGHT
*\req    NA
*/
TEST(Get_Reference_Point_Pos_In_TCS, rear_right_reference_point_seen)
{
   /** \precond
    * Suppose the reference_point varaible is set to F360_REFERENCE_POINT_REAR_RIGHT
    **/
   reference_point = F360_REFERENCE_POINT_REAR_RIGHT;
   float32_t expected_ref_post_x = - 0.5F * bbox_length;
   float32_t expected_ref_post_y = 0.5F * bbox_width;

   /** \action
   * Call function Get_Reference_Point_Pos_In_TCS
   **/
   Point ref_pos_tcs = Get_Reference_Point_Pos_In_TCS(reference_point, bbox_length, bbox_width);

   /** \result
   * Expect the REAR RIGHT reference point
   **/
   CHECK_EQUAL(expected_ref_post_x, ref_pos_tcs.x);
   CHECK_EQUAL(expected_ref_post_y, ref_pos_tcs.y);
}

/**
*\purpose  Verify that reference point position in TCS is precisely calculated given F360_REFERENCE_POINT_REAR
*\req    NA
*/
TEST(Get_Reference_Point_Pos_In_TCS, rear_reference_point_seen)
{
   /** \precond
    * Suppose the reference_point varaible is set to F360_REFERENCE_POINT_REAR
    **/
   reference_point = F360_REFERENCE_POINT_REAR;
   float32_t expected_ref_post_x = -0.5F * bbox_length;
   float32_t expected_ref_post_y = 0.0F;

   /** \action
   * Call function Get_Reference_Point_Pos_In_TCS
   **/
   Point ref_pos_tcs = Get_Reference_Point_Pos_In_TCS(reference_point, bbox_length, bbox_width);

   /** \result
   * Expect the REAR reference point
   **/
   CHECK_EQUAL(expected_ref_post_x, ref_pos_tcs.x);
   CHECK_EQUAL(expected_ref_post_y, ref_pos_tcs.y);
}

/**
*\purpose  Verify that reference point position in TCS is precisely calculated given F360_REFERENCE_POINT_REAR_LEFT
*\req    NA
*/
TEST(Get_Reference_Point_Pos_In_TCS, rear_left_reference_point_seen)
{
   /** \precond
    * Suppose the reference_point varaible is set to F360_REFERENCE_POINT_REAR_LEFT
    **/
   reference_point = F360_REFERENCE_POINT_REAR_LEFT;
   float32_t expected_ref_post_x = -0.5F * bbox_length;
   float32_t expected_ref_post_y = -0.5F * bbox_width;

   /** \action
   * Call function Get_Reference_Point_Pos_In_TCS
   **/
   Point ref_pos_tcs = Get_Reference_Point_Pos_In_TCS(reference_point, bbox_length, bbox_width);

   /** \result
   * Expect the REAR LEFT reference point
   **/
   CHECK_EQUAL(expected_ref_post_x, ref_pos_tcs.x);
   CHECK_EQUAL(expected_ref_post_y, ref_pos_tcs.y);
}

/**
*\purpose  Verify that reference point position in TCS is precisely calculated given F360_REFERENCE_POINT_LEFT
*\req    NA
*/
TEST(Get_Reference_Point_Pos_In_TCS, left_reference_point_seen)
{
   /** \precond
    * Suppose the reference_point varaible is set to F360_REFERENCE_POINT_FRONT_LEFT
    **/
   reference_point = F360_REFERENCE_POINT_LEFT;
   float32_t expected_ref_post_x = 0.0F;
   float32_t expected_ref_post_y = -0.5F * bbox_width;

   /** \action
   * Call function Get_Reference_Point_Pos_In_TCS
   **/
   Point ref_pos_tcs = Get_Reference_Point_Pos_In_TCS(reference_point, bbox_length, bbox_width);

   /** \result
   * Expect the LEFT reference point
   **/
   CHECK_EQUAL(expected_ref_post_x, ref_pos_tcs.x);
   CHECK_EQUAL(expected_ref_post_y, ref_pos_tcs.y);
}

/**
*\purpose  Verify that reference point position in TCS is precisely calculated given F360_REFERENCE_POINT_CENTER
*\req    NA
*/
TEST(Get_Reference_Point_Pos_In_TCS, center_reference_point_seen)
{
   /** \precond
    * Suppose the reference_point varaible is set to F360_REFERENCE_POINT_CENTER
    **/
   reference_point = F360_REFERENCE_POINT_CENTER;
   float32_t expected_ref_post_x = 0.0F;
   float32_t expected_ref_post_y = 0.0F;

   /** \action
   * Call function Get_Reference_Point_Pos_In_TCS
   **/
   Point ref_pos_tcs = Get_Reference_Point_Pos_In_TCS(reference_point, bbox_length, bbox_width);

   /** \result
   * Expect the CENTER reference point
   **/
   CHECK_EQUAL(expected_ref_post_x, ref_pos_tcs.x);
   CHECK_EQUAL(expected_ref_post_y, ref_pos_tcs.y);
}
/** @}*/

/** \defgroup  Get_Distances_From_Ref_Point_To_Obj_Sides
 *  @{
 */

 /** \brief
  * Test group of Get_Reference_Point_Pos_In_TCS function. 
  */
TEST_GROUP(Get_Distances_From_Ref_Point_To_Obj_Sides)
{
   /* Common setup for test.
    Create an arbitrary object. */
   F360_Object_Track_T obj;
   float32_t len1;
   float32_t len2;
   float32_t wid1;
   float32_t wid2;

   /** \setup
    * Initialize an object with the bounding box of 4 meters length and 2 meters width
    **/
   TEST_SETUP()
   {
      obj.reference_point = F360_REFERENCE_POINT_CENTER;
      obj.length_processed = 4.0F;
      obj.width_processed = 2.0F;
   }
};

/**
*\purpose  Verify that given the reference point as Front Left, the distance to object sides is calculated correctly
*\req    NA
*/
TEST(Get_Distances_From_Ref_Point_To_Obj_Sides, front_left_reference_point)
{
   /** \precond
    * Set the reference_point to F360_REFERENCE_POINT_FRONT_LEFT
    **/
   obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;

   /** \action
   * Call function Get_Distances_From_Ref_Point_To_Obj_Sides
   **/
   Get_Distances_From_Ref_Point_To_Obj_Sides(obj, len1, len2, wid1, wid2);

   /** \result
   * Expect the len1 to be equal to object length
   * Expect the len2 to be equal to zero
   * Expect the wid1 to be equal to zero
   * Expect the wid2 to be equal to object width
   **/
   CHECK_EQUAL(4.0F, len1);
   CHECK_EQUAL(0.0F, len2);
   CHECK_EQUAL(0.0F, wid1);
   CHECK_EQUAL(2.0F, wid2);
}

/**
*\purpose  Verify that given the reference point as Front, the distance to object sides is calculated correctly
*\req    NA
*/
TEST(Get_Distances_From_Ref_Point_To_Obj_Sides, front_reference_point)
{
   /** \precond
    * Set the reference_point to F360_REFERENCE_POINT_FRONT
    **/
   obj.reference_point = F360_REFERENCE_POINT_FRONT;

   /** \action
   * Call function Get_Distances_From_Ref_Point_To_Obj_Sides
   **/
   Get_Distances_From_Ref_Point_To_Obj_Sides(obj, len1, len2, wid1, wid2);

   /** \result
   * Expect the len1 to be equal to object length
   * Expect the len2 to be equal to zero
   * Expect the wid1 to be equal to half of object width
   * Expect the wid2 to be equal to half of object width
   **/
   CHECK_EQUAL(4.0F, len1);
   CHECK_EQUAL(0.0F, len2);
   CHECK_EQUAL(1.0F, wid1);
   CHECK_EQUAL(1.0F, wid2);
}

/**
*\purpose  Verify that given the reference point as Front Right, the distance to object sides is calculated correctly
*\req    NA
*/
TEST(Get_Distances_From_Ref_Point_To_Obj_Sides, front_right_reference_point)
{
   /** \precond
    * Set the reference_point to F360_REFERENCE_POINT_FRONT
    **/
   obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;

   /** \action
   * Call function Get_Distances_From_Ref_Point_To_Obj_Sides
   **/
   Get_Distances_From_Ref_Point_To_Obj_Sides(obj, len1, len2, wid1, wid2);

   /** \result
   * Expect the len1 to be equal to object length
   * Expect the len2 to be equal to zero
   * Expect the wid1 to be equal to object width
   * Expect the wid2 to be equal to zero
   **/
   CHECK_EQUAL(4.0F, len1);
   CHECK_EQUAL(0.0F, len2);
   CHECK_EQUAL(2.0F, wid1);
   CHECK_EQUAL(0.0F, wid2);
}

/**
*\purpose  Verify that given the reference point as Right, the distance to object sides is calculated correctly
*\req    NA
*/
TEST(Get_Distances_From_Ref_Point_To_Obj_Sides, right_reference_point)
{
   /** \precond
    * Set the reference_point to F360_REFERENCE_POINT_RIGHT
    **/
   obj.reference_point = F360_REFERENCE_POINT_RIGHT;

   /** \action
   * Call function Get_Distances_From_Ref_Point_To_Obj_Sides
   **/
   Get_Distances_From_Ref_Point_To_Obj_Sides(obj, len1, len2, wid1, wid2);

   /** \result
   * Expect the len1 to be equal to half of object length
   * Expect the len2 to be equal to half of object length
   * Expect the wid1 to be equal to object width
   * Expect the wid2 to be equal to zero
   **/
   CHECK_EQUAL(2.0F, len1);
   CHECK_EQUAL(2.0F, len2);
   CHECK_EQUAL(2.0F, wid1);
   CHECK_EQUAL(0.0F, wid2);
}

/**
*\purpose  Verify that given the reference point as Rear Right, the distance to object sides is calculated correctly
*\req    NA
*/
TEST(Get_Distances_From_Ref_Point_To_Obj_Sides, rear_right_reference_point)
{
   /** \precond
    * Set the reference_point to F360_REFERENCE_POINT_REAR_RIGHT
    **/
   obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;

   /** \action
   * Call function Get_Distances_From_Ref_Point_To_Obj_Sides
   **/
   Get_Distances_From_Ref_Point_To_Obj_Sides(obj, len1, len2, wid1, wid2);

   /** \result
   * Expect the len1 to be equal to zero
   * Expect the len2 to be equal to object length
   * Expect the wid1 to be equal to object width
   * Expect the wid2 to be equal to zero
   **/
   CHECK_EQUAL(0.0F, len1);
   CHECK_EQUAL(4.0F, len2);
   CHECK_EQUAL(2.0F, wid1);
   CHECK_EQUAL(0.0F, wid2);
}

/**
*\purpose  Verify that given the reference point as Rear, the distance to object sides is calculated correctly
*\req    NA
*/
TEST(Get_Distances_From_Ref_Point_To_Obj_Sides, rear_reference_point)
{
   /** \precond
    * Set the reference_point to F360_REFERENCE_POINT_REAR
    **/
   obj.reference_point = F360_REFERENCE_POINT_REAR;

   /** \action
   * Call function Get_Distances_From_Ref_Point_To_Obj_Sides
   **/
   Get_Distances_From_Ref_Point_To_Obj_Sides(obj, len1, len2, wid1, wid2);

   /** \result
   * Expect the len1 to be equal to zero
   * Expect the len2 to be equal to object length
   * Expect the wid1 to be equal to half of object width
   * Expect the wid2 to be equal to half of object width
   **/
   CHECK_EQUAL(0.0F, len1);
   CHECK_EQUAL(4.0F, len2);
   CHECK_EQUAL(1.0F, wid1);
   CHECK_EQUAL(1.0F, wid2);
}

/**
*\purpose  Verify that given the reference point as Rear Left, the distance to object sides is calculated correctly
*\req    NA
*/
TEST(Get_Distances_From_Ref_Point_To_Obj_Sides, rear_left_reference_point)
{
   /** \precond
    * Set the reference_point to F360_REFERENCE_POINT_REAR_LEFT
    **/
   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;

   /** \action
   * Call function Get_Distances_From_Ref_Point_To_Obj_Sides
   **/
   Get_Distances_From_Ref_Point_To_Obj_Sides(obj, len1, len2, wid1, wid2);

   /** \result
   * Expect the len1 to be equal to zero
   * Expect the len2 to be equal to object length
   * Expect the wid1 to be equal to zero
   * Expect the wid2 to be equal to object width
   **/
   CHECK_EQUAL(0.0F, len1);
   CHECK_EQUAL(4.0F, len2);
   CHECK_EQUAL(0.0F, wid1);
   CHECK_EQUAL(2.0F, wid2);
}

/**
*\purpose  Verify that given the reference point as Left, the distance to object sides is calculated correctly
*\req    NA
*/
TEST(Get_Distances_From_Ref_Point_To_Obj_Sides, left_reference_point)
{
   /** \precond
    * Set the reference_point to F360_REFERENCE_POINT_LEFT
    **/
   obj.reference_point = F360_REFERENCE_POINT_LEFT;

   /** \action
   * Call function Get_Distances_From_Ref_Point_To_Obj_Sides
   **/
   Get_Distances_From_Ref_Point_To_Obj_Sides(obj, len1, len2, wid1, wid2);

   /** \result
   * Expect the len1 to be equal to half of object length
   * Expect the len2 to be equal to half of object length
   * Expect the wid1 to be equal to zero
   * Expect the wid2 to be equal to object width
   **/
   CHECK_EQUAL(2.0F, len1);
   CHECK_EQUAL(2.0F, len2);
   CHECK_EQUAL(0.0F, wid1);
   CHECK_EQUAL(2.0F, wid2);
}

/**
*\purpose  Verify that given the reference point as center or default, the distance to object sides is calculated correctly
*\req    NA
*/
TEST(Get_Distances_From_Ref_Point_To_Obj_Sides, default_center_reference_point)
{
   /** \precond
    * Set the reference_point to F360_REFERENCE_POINT_CENTER
    **/
   obj.reference_point = F360_REFERENCE_POINT_CENTER;

   /** \action
   * Call function Get_Distances_From_Ref_Point_To_Obj_Sides
   **/
   Get_Distances_From_Ref_Point_To_Obj_Sides(obj, len1, len2, wid1, wid2);

   /** \result
   * Expect the len1 to be equal to half of object length
   * Expect the len2 to be equal to half of object length
   * Expect the wid1 to be equal to half of object width
   * Expect the wid2 to be equal to half of object width
   **/
   CHECK_EQUAL(2.0F, len1);
   CHECK_EQUAL(2.0F, len2);
   CHECK_EQUAL(1.0F, wid1);
   CHECK_EQUAL(1.0F, wid2);
}
/** @}*/


/** \defgroup  Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point
 *  @{
 */

 /** \brief
  * Test group for function Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point(). 
  */
TEST_GROUP(Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point)
{
  /* Common setup for test.
     Create an arbitrary object. */
  F360_Object_Track_T obj;

  const float32_t test_pass_th = F360_EPSILON;

   /** \setup
    * Initialize an object with:
    *    - a bounding box of 8 meters in length and 2 meters in width (exact numbers are not important for tests these numbers are choosen on random)
    *    - reference point in center of object
    **/
   TEST_SETUP()
   {
      obj.bbox.Set_Length(8.0F);
      obj.bbox.Set_Width(2.0F);
      obj.reference_point = F360_REFERENCE_POINT_CENTER;
   }
};

/**
*\purpose  Verify that Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point() is outputting the correct vector when reference point is in center of object
*\req    NA
*/
TEST(Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point, Test_Ref_Pnt_In_Center)
{
   /** \precond
    * Use default settings from TEST_GROUP (i.e. object bounding box dimensions are 8m x 2m and object reference point is in center)
    **/

   /** \action
   * Call function Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point()
   **/
   float32_t tcs_vec_from_center_rear_to_ref_pnt[2];
   Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point(obj, tcs_vec_from_center_rear_to_ref_pnt);

   /** \result
   * Check such that the outputted vector from object rear center to object reference point is [half bounding box length, 0], i.e. [4,0]
   **/
   DOUBLES_EQUAL_TEXT(4.0F, tcs_vec_from_center_rear_to_ref_pnt[0], test_pass_th, "Para distance from center rear to reference point is wrong.");
   DOUBLES_EQUAL_TEXT(0.0F, tcs_vec_from_center_rear_to_ref_pnt[1], test_pass_th, "Orth distance from center rear to reference point is wrong.");
}

/**
*\purpose  Verify that Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point() is outputting the correct vector when reference point is in front left corner of object.
*\req    NA
*/
TEST(Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point, Test_Ref_Pnt_In_Front_Left)
{
   /** \precond
    * Use default object bounding box settings from TEST_GROUP (i.e. object bounding box dimensions are 8m x 2m).
    * Set object reference point to be in front left corner.
    **/
   obj.reference_point = F360_REFERENCE_POINT_FRONT_LEFT;

   /** \action
   * Call function Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point()
   **/
   float32_t tcs_vec_from_center_rear_to_ref_pnt[2];
   Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point(obj, tcs_vec_from_center_rear_to_ref_pnt);

   /** \result
   * Check such that the outputted vector from object rear center to object reference point is [bounding box length, -half bounding box width], i.e. [8,-1]
   **/
   DOUBLES_EQUAL_TEXT(8.0F, tcs_vec_from_center_rear_to_ref_pnt[0], test_pass_th, "Para distance from center rear to reference point is wrong.");
   DOUBLES_EQUAL_TEXT(-1.0F, tcs_vec_from_center_rear_to_ref_pnt[1], test_pass_th, "Orth distance from center rear to reference point is wrong.");
}

/**
*\purpose  Verify that Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point() is outputting the correct vector when reference point is in front center of object.
*\req    NA
*/
TEST(Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point, Test_Ref_Pnt_In_Front_Center)
{
   /** \precond
    * Use default object bounding box settings from TEST_GROUP (i.e. object bounding box dimensions are 8m x 2m).
    * Set object reference point to be in front center.
    **/
   obj.reference_point = F360_REFERENCE_POINT_FRONT;

   /** \action
   * Call function Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point()
   **/
   float32_t tcs_vec_from_center_rear_to_ref_pnt[2];
   Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point(obj, tcs_vec_from_center_rear_to_ref_pnt);

   /** \result
   * Check such that the outputted vector from object rear center to object reference point is [bounding box length, 0], i.e. [8,0]
   **/
   DOUBLES_EQUAL_TEXT(8.0F, tcs_vec_from_center_rear_to_ref_pnt[0], test_pass_th, "Para distance from center rear to reference point is wrong.");
   DOUBLES_EQUAL_TEXT(0.0F, tcs_vec_from_center_rear_to_ref_pnt[1], test_pass_th, "Orth distance from center rear to reference point is wrong.");
}

/**
*\purpose  Verify that Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point() is outputting the correct vector when reference point is in front right corner of object.
*\req    NA
*/
TEST(Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point, Test_Ref_Pnt_In_Front_Right)
{
   /** \precond
    * Use default object bounding box settings from TEST_GROUP (i.e. object bounding box dimensions are 8m x 2m).
    * Set object reference point to be in front right corner.
    **/
   obj.reference_point = F360_REFERENCE_POINT_FRONT_RIGHT;

   /** \action
   * Call function Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point()
   **/
   float32_t tcs_vec_from_center_rear_to_ref_pnt[2];
   Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point(obj, tcs_vec_from_center_rear_to_ref_pnt);

   /** \result
   * Check such that the outputted vector from object rear center to object reference point is [bounding box length, half bounding box width], i.e. [8,1]
   **/
   DOUBLES_EQUAL_TEXT(8.0F, tcs_vec_from_center_rear_to_ref_pnt[0], test_pass_th, "Para distance from center rear to reference point is wrong.");
   DOUBLES_EQUAL_TEXT(1.0F, tcs_vec_from_center_rear_to_ref_pnt[1], test_pass_th, "Orth distance from center rear to reference point is wrong.");
}

/**
*\purpose  Verify that Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point() is outputting the correct vector when reference point is in center of right side of object.
*\req    NA
*/
TEST(Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point, Test_Ref_Pnt_In_Right_Center)
{
   /** \precond
    * Use default object bounding box settings from TEST_GROUP (i.e. object bounding box dimensions are 8m x 2m).
    * Set object reference point to be in center of right side.
    **/
   obj.reference_point = F360_REFERENCE_POINT_RIGHT;

   /** \action
   * Call function Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point()
   **/
   float32_t tcs_vec_from_center_rear_to_ref_pnt[2];
   Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point(obj, tcs_vec_from_center_rear_to_ref_pnt);

   /** \result
   * Check such that the outputted vector from object rear center to object reference point is [half bounding box length, half bounding box width], i.e. [4,1]
   **/
   DOUBLES_EQUAL_TEXT(4.0F, tcs_vec_from_center_rear_to_ref_pnt[0], test_pass_th, "Para distance from center rear to reference point is wrong.");
   DOUBLES_EQUAL_TEXT(1.0F, tcs_vec_from_center_rear_to_ref_pnt[1], test_pass_th, "Orth distance from center rear to reference point is wrong.");
}

/**
*\purpose  Verify that Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point() is outputting the correct vector when reference point is in rear right corner of object.
*\req    NA
*/
TEST(Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point, Test_Ref_Pnt_In_Rear_Right)
{
   /** \precond
    * Use default object bounding box settings from TEST_GROUP (i.e. object bounding box dimensions are 8m x 2m).
    * Set object reference point to be in rear right corner.
    **/
   obj.reference_point = F360_REFERENCE_POINT_REAR_RIGHT;

   /** \action
   * Call function Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point()
   **/
   float32_t tcs_vec_from_center_rear_to_ref_pnt[2];
   Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point(obj, tcs_vec_from_center_rear_to_ref_pnt);

   /** \result
   * Check such that the outputted vector from object rear center to object reference point is [0, half bounding box width], i.e. [0,1]
   **/
   DOUBLES_EQUAL_TEXT(0.0F, tcs_vec_from_center_rear_to_ref_pnt[0], test_pass_th, "Para distance from center rear to reference point is wrong.");
   DOUBLES_EQUAL_TEXT(1.0F, tcs_vec_from_center_rear_to_ref_pnt[1], test_pass_th, "Orth distance from center rear to reference point is wrong.");
}

/**
*\purpose  Verify that Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point() is outputting the correct vector when reference point is in rear center of object.
*\req    NA
*/
TEST(Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point, Test_Ref_Pnt_In_Rear_Center)
{
   /** \precond
    * Use default object bounding box settings from TEST_GROUP (i.e. object bounding box dimensions are 8m x 2m).
    * Set object reference point to be in rear center.
    **/
   obj.reference_point = F360_REFERENCE_POINT_REAR;

   /** \action
   * Call function Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point()
   **/
   float32_t tcs_vec_from_center_rear_to_ref_pnt[2];
   Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point(obj, tcs_vec_from_center_rear_to_ref_pnt);

   /** \result
   * Check such that the outputted vector from object rear center to object reference point is [0, 0]
   **/
   DOUBLES_EQUAL_TEXT(0.0F, tcs_vec_from_center_rear_to_ref_pnt[0], test_pass_th, "Para distance from center rear to reference point is wrong.");
   DOUBLES_EQUAL_TEXT(0.0F, tcs_vec_from_center_rear_to_ref_pnt[1], test_pass_th, "Orth distance from center rear to reference point is wrong.");
}

/**
*\purpose  Verify that Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point() is outputting the correct vector when reference point is in rear left corner of object.
*\req    NA
*/
TEST(Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point, Test_Ref_Pnt_In_Rear_Left)
{
   /** \precond
    * Use default object bounding box settings from TEST_GROUP (i.e. object bounding box dimensions are 8m x 2m).
    * Set object reference point to be in rear left corner.
    **/
   obj.reference_point = F360_REFERENCE_POINT_REAR_LEFT;

   /** \action
   * Call function Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point()
   **/
   float32_t tcs_vec_from_center_rear_to_ref_pnt[2];
   Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point(obj, tcs_vec_from_center_rear_to_ref_pnt);

   /** \result
   * Check such that the outputted vector from object rear center to object reference point is [0, -half bounding box width], i.e. [0,-1]
   **/
   DOUBLES_EQUAL_TEXT(0.0F, tcs_vec_from_center_rear_to_ref_pnt[0], test_pass_th, "Para distance from center rear to reference point is wrong.");
   DOUBLES_EQUAL_TEXT(-1.0F, tcs_vec_from_center_rear_to_ref_pnt[1], test_pass_th, "Orth distance from center rear to reference point is wrong.");
}

/**
*\purpose  Verify that Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point() is outputting the correct vector when reference point is in center of left side of object.
*\req    NA
*/
TEST(Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point, Test_Ref_Pnt_In_Left_Center)
{
   /** \precond
    * Use default object bounding box settings from TEST_GROUP (i.e. object bounding box dimensions are 8m x 2m).
    * Set object reference point to be in center of left side.
    **/
   obj.reference_point = F360_REFERENCE_POINT_LEFT;

   /** \action
   * Call function Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point()
   **/
   float32_t tcs_vec_from_center_rear_to_ref_pnt[2];
   Get_TCS_Vec_From_Obj_Center_Rear_to_Ref_Point(obj, tcs_vec_from_center_rear_to_ref_pnt);

   /** \result
   * Check such that the outputted vector from object rear center to object reference point is [half bounding box length, -half bounding box width], i.e. [4,-1]
   **/
   DOUBLES_EQUAL_TEXT(4.0F, tcs_vec_from_center_rear_to_ref_pnt[0], test_pass_th, "Para distance from center rear to reference point is wrong.");
   DOUBLES_EQUAL_TEXT(-1.0F, tcs_vec_from_center_rear_to_ref_pnt[1], test_pass_th, "Orth distance from center rear to reference point is wrong.");
}
/** @}*/
