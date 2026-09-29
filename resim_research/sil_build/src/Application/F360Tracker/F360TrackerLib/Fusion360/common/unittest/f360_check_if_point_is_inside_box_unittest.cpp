/** \file
 * This file contains unit tests for content of f360_check_if_point_is_inside_box.cpp file
 */

#include "f360_check_if_point_is_inside_box.h"
#include <CppUTest/TestHarness.h>

//#include "headerfile_needed.h"

// Unit testing guidelines: http://confluenceprod1.delphiauto.net:8090/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  check_if_point_is_inside_box_in_same_cs
 *  @{
 */

/** \brief
 * This test group includes test of the function Check_If_Point_Is_Inside_Box_In_Same_CS() defined in 
 * f360_check_if_point_is_inside_box.cpp.
 */
TEST_GROUP(check_if_point_is_inside_box_in_same_cs)
{	
   // Declare common variables used within all tests in this test group.
   float32_t x_point;
   float32_t y_point;
   float32_t box[2][2];
   /** \setup
    * Set up the dimensions of the default box used in this test group.
    */
   TEST_SETUP()
   {
      // Set up a default scenario for your tests. E.g. assign values to common variables declared above.
      box[0][0] = -1.0F;
      box[0][1] = 2.0F;
      box[1][0] = -1.5F;
      box[1][1] = 0.5F;
   }
};

/** \purpose  
 * Check that the function Check_If_Point_Is_Inside_Box_In_Same_CS works as intended when the point is inside
 * the specified box.
 * \req
 * NA
 */
TEST(check_if_point_is_inside_box_in_same_cs, Check_If_Point_Is_Inside_Box_In_Same_CS_Inside_Box)
{
   /** \precond
   * Prepare point so that it is expected to be inside the box defined in test setup.
   */
   x_point = 1.0F;
   y_point = -0.5F;
	
   /** \action
    * Call Check_If_Point_Is_Inside_Box_In_Same_CS().
    */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Box_In_Same_CS(x_point, y_point, box);

   /** \result
    * Check that Check_If_Point_Is_Inside_Box_In_Same_CS() returns the expected output. 
    */
   CHECK_TRUE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Box_In_Same_CS() did not return the expected output.")
}

/** \purpose
* Check that the function Check_If_Point_Is_Inside_Box_In_Same_CS works as intended when the point is outside
* and above the specified box.
* \req
* NA
*/
TEST(check_if_point_is_inside_box_in_same_cs, Check_If_Point_Is_Inside_Box_In_Same_CS_Outside_Box_Above)
{
   /** \precond
   * Prepare point so that it is expected to be outside and above the box defined in test setup.
   */
   x_point = 2.1F;
   y_point = 0.0F;

   /** \action
   * Call Check_If_Point_Is_Inside_Box_In_Same_CS().
   */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Box_In_Same_CS(x_point, y_point, box);

   /** \result
   * Check that Check_If_Point_Is_Inside_Box_In_Same_CS() returns the expected output.
   */
   CHECK_FALSE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Box_In_Same_CS() did not return the expected output.")
}

/** \purpose
* Check that the function Check_If_Point_Is_Inside_Box_In_Same_CS works as intended when the point is outside
* and below the specified box.
* \req
* NA
*/
TEST(check_if_point_is_inside_box_in_same_cs, Check_If_Point_Is_Inside_Box_In_Same_CS_Outside_Box_Below)
{
   /** \precond
   * Prepare point so that it is expected to be outside and below the box defined in test setup.
   */
   x_point = -1.1F;
   y_point = 0.0F;

   /** \action
   * Call Check_If_Point_Is_Inside_Box_In_Same_CS().
   */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Box_In_Same_CS(x_point, y_point, box);

   /** \result
   * Check that Check_If_Point_Is_Inside_Box_In_Same_CS() returns the expected output.
   */
   CHECK_FALSE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Box_In_Same_CS() did not return the expected output.")
}

/** \purpose
* Check that the function Check_If_Point_Is_Inside_Box_In_Same_CS works as intended when the point is outside
* and to the right of the specified box.
* \req
* NA
*/
TEST(check_if_point_is_inside_box_in_same_cs, Check_If_Point_Is_Inside_Box_In_Same_CS_Outside_Box_Right)
{
   /** \precond
   * Prepare point so that it is expected to be outside and to the right of the box defined in test setup.
   */
   x_point = 0.0F;
   y_point = 0.6F;

   /** \action
   * Call Check_If_Point_Is_Inside_Box_In_Same_CS().
   */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Box_In_Same_CS(x_point, y_point, box);

   /** \result
   * Check that Check_If_Point_Is_Inside_Box_In_Same_CS() returns the expected output.
   */
   CHECK_FALSE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Box_In_Same_CS() did not return the expected output.")
}

/** \purpose
* Check that the function Check_If_Point_Is_Inside_Box_In_Same_CS works as intended when the point is outside
* and to the left of the specified box.
* \req
* NA
*/
TEST(check_if_point_is_inside_box_in_same_cs, Check_If_Point_Is_Inside_Box_In_Same_CS_Outside_Box_Left)
{
   /** \precond
   * Prepare point so that it is expected to be outside and to the left of the box defined in test setup.
   */
   x_point = 0.0F;
   y_point = -1.6F;

   /** \action
   * Call Check_If_Point_Is_Inside_Box_In_Same_CS().
   */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Box_In_Same_CS(x_point, y_point, box);

   /** \result
   * Check that Check_If_Point_Is_Inside_Box_In_Same_CS() returns the expected output.
   */
   CHECK_FALSE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Box_In_Same_CS() did not return the expected output.")
}

/** \purpose
* Check that the function Check_If_Point_Is_Inside_Box_In_Same_CS works as intended when the point is on the corner of
* the specified box.
* \req
* NA
*/
TEST(check_if_point_is_inside_box_in_same_cs, Check_If_Point_Is_Inside_Box_In_Same_CS_On_Corner)
{
   /** \precond
   * Prepare point so that it is on the edge of the box defined in test setup, i.e. it should be set as inside.
   */
   x_point = -1.0F;
   y_point = -1.5F;

   /** \action
   * Call Check_If_Point_Is_Inside_Box_In_Same_CS().
   */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Box_In_Same_CS(x_point, y_point, box);

   /** \result
   * Check that Check_If_Point_Is_Inside_Box_In_Same_CS() returns the expected output.
   */
   CHECK_TRUE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Box_In_Same_CS() did not return the expected output.")
}
/** @}*/

/** \defgroup  check_if_point_is_inside_extended_bounding_box
 *  @{
 */

/** \brief
 * This test group includes test of the function Check_If_Point_Is_Inside_Extended_Bounding_Box() defined in 
 * f360_check_if_point_is_inside_box.cpp.
 */
TEST_GROUP(check_if_point_is_inside_extended_bounding_box)
{	
   // Declare common variables used within all tests in this test group.
   F360_Object_Track_T obj;
   /** \setup
    * Set up the dimensions of the default box used in this test group.
    */
   TEST_SETUP()
   {
      // Set up a default scenario for your tests. E.g. assign values to common variables declared above.
      obj.long_buffer_zone_len1 = 1.0F;
      obj.long_buffer_zone_len2 = 1.0F;
      obj.lat_buffer_zone_wid1 = 1.0F;
      obj.lat_buffer_zone_wid2 = 1.0F;
      obj.Update_Bbox_Size(3.0F, 2.0F);
   }
};

/** \purpose  
 * Check that the function Check_If_Point_Is_Inside_Extended_Bounding_Box works as intended when the point is inside
 * the specified box.
 * \req
 * NA
 */
TEST(check_if_point_is_inside_extended_bounding_box, Check_If_Point_Is_Inside_Extended_Bounding_Box_Inside_Box)
{
   /** \precond
   * Prepare point so that it is expected to be inside the box defined in test setup.
   */
   Point point{ 2.1F, -1.6F };
	
   /** \action
    * Call Check_If_Point_Is_Inside_Extended_Bounding_Box().
    */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Extended_Bounding_Box(point, obj);

   /** \result
    * Check that Check_If_Point_Is_Inside_Extended_Bounding_Box() returns the expected output. 
    */
   CHECK_TRUE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Extended_Bounding_Box() did not return the expected output.")
}

/** \purpose
* Check that the function Check_If_Point_Is_Inside_Extended_Bounding_Box works as intended when the point is outside
* and above the specified box.
* \req
* NA
*/
TEST(check_if_point_is_inside_extended_bounding_box, Check_If_Point_Is_Inside_Extended_Bounding_Box_Outside_Box_Above)
{
   /** \precond
   * Prepare point so that it is expected to be outside and above the box defined in test setup.
   */
   Point point{ 3.1F, 0.0F };

   /** \action
   * Call Check_If_Point_Is_Inside_Extended_Bounding_Box().
   */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Extended_Bounding_Box(point, obj);

   /** \result
   * Check that Check_If_Point_Is_Inside_Extended_Bounding_Box() returns the expected output.
   */
   CHECK_FALSE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Extended_Bounding_Box() did not return the expected output.")
}

/** \purpose
* Check that the function Check_If_Point_Is_Inside_Extended_Bounding_Box works as intended when the point is outside
* and below the specified box.
* \req
* NA
*/
TEST(check_if_point_is_inside_extended_bounding_box, Check_If_Point_Is_Inside_Extended_Bounding_Box_Outside_Box_Below)
{
   /** \precond
   * Prepare point so that it is expected to be outside and below the box defined in test setup.
   */
   Point point{ -2.6F, 0.5F };

   /** \action
   * Call Check_If_Point_Is_Inside_Extended_Bounding_Box().
   */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Extended_Bounding_Box(point, obj);

   /** \result
   * Check that Check_If_Point_Is_Inside_Extended_Bounding_Box() returns the expected output.
   */
   CHECK_FALSE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Extended_Bounding_Box() did not return the expected output.")
}

/** \purpose
* Check that the function Check_If_Point_Is_Inside_Extended_Bounding_Box works as intended when the point is outside
* and to the right of the specified box.
* \req
* NA
*/
TEST(check_if_point_is_inside_extended_bounding_box, Check_If_Point_Is_Inside_Extended_Bounding_Box_Outside_Box_Right)
{
   /** \precond
   * Prepare point so that it is expected to be outside and to the right of the box defined in test setup.
   */
   Point point{ -0.5F, 2.1F };

   /** \action
   * Call Check_If_Point_Is_Inside_Extended_Bounding_Box().
   */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Extended_Bounding_Box(point, obj);

   /** \result
   * Check that Check_If_Point_Is_Inside_Extended_Bounding_Box() returns the expected output.
   */
   CHECK_FALSE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Extended_Bounding_Box() did not return the expected output.")
}

/** \purpose
* Check that the function Check_If_Point_Is_Inside_Extended_Bounding_Box works as intended when the point is outside
* and to the left of the specified box.
* \req
* NA
*/
TEST(check_if_point_is_inside_extended_bounding_box, Check_If_Point_Is_Inside_Extended_Bounding_Box_Outside_Box_Left)
{
   /** \precond
   * Prepare point so that it is expected to be outside and to the left of the box defined in test setup.
   */
   Point point{ 0.0F, -2.6F };

   /** \action
   * Call Check_If_Point_Is_Inside_Extended_Bounding_Box().
   */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Extended_Bounding_Box(point, obj);

   /** \result
   * Check that Check_If_Point_Is_Inside_Extended_Bounding_Box() returns the expected output.
   */
   CHECK_FALSE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Extended_Bounding_Box() did not return the expected output.")
}

/** \purpose
* Check that the function Check_If_Point_Is_Inside_Extended_Bounding_Box works as intended when the point is on the corner of
* the specified box.
* \req
* NA
*/
TEST(check_if_point_is_inside_extended_bounding_box, Check_If_Point_Is_Inside_Extended_Bounding_Box_On_Corner)
{
   /** \precond
   * Prepare point so that it is on the edge of the box defined in test setup, i.e. it should be set as inside.
   */
   Point point{ -2.5F, -2.0F };

   /** \action
   * Call Check_If_Point_Is_Inside_Extended_Bounding_Box().
   */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Extended_Bounding_Box(point, obj);

   /** \result
   * Check that Check_If_Point_Is_Inside_Extended_Bounding_Box() returns the expected output.
   */
   CHECK_TRUE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Extended_Bounding_Box() did not return the expected output.")
}
/** @}*/

/** \defgroup  check_if_point_is_inside_extended_bounding_box
 *  @{
 */

/** \brief
 * This test group includes test of the function Determine_Extended_Bounding_Box() that extracts an objects extended
 * bounding box.
 */
TEST_GROUP(check_if_point_is_inside_box_extended_box)
{
   // Declare common variables used within all tests in this test group.
   F360_Object_Track_T obj = {};
   /** \setup
    * Set up the dimensions and extended buffer zones for the object
    */
   TEST_SETUP()
   {
      obj.long_buffer_zone_len1 = 1.0F;
      obj.long_buffer_zone_len2 = 1.0F;
      obj.lat_buffer_zone_wid1 = 1.0F;
      obj.lat_buffer_zone_wid2 = 1.0F;
      obj.Update_Bbox_Size(3.0F, 2.0F);
   }
};

/** \purpose
 * Check that the function Determine_Extended_Bounding_Box works as intended.
 * \req
 * NA
 */
TEST(check_if_point_is_inside_box_extended_box, Determine_Extended_Bounding_Box)
{
   /** \precond
   * An object has been created in test group with dimensions and extended buffer zones set.
   */
   const BoundingBox expected_box{ Point{-2.5F, -2.0F}, Point{2.5F, 2.0F} };
   BoundingBox box = obj.bbox;

   /** \action
    * Call Determine_Extended_Bounding_Box().
    */
   Determine_Extended_Bounding_Box(obj, box);

   /** \result
    * Check that bounding box match expected data
    */
   CHECK_TRUE_TEXT(expected_box.Get_Corners().Rear_Left() == box.Get_Corners().Rear_Left(), "Position of rear left corner did not match the expected data.");
   CHECK_TRUE_TEXT(expected_box.Get_Corners().Rear_Right() == box.Get_Corners().Rear_Right(), "Position of rear right corner did not match the expected data.");
   CHECK_TRUE_TEXT(expected_box.Get_Corners().Front_Left() == box.Get_Corners().Front_Left(), "Position of front left corner did not match the expected data.");
   CHECK_TRUE_TEXT(expected_box.Get_Corners().Front_Right() == box.Get_Corners().Front_Right(), "Position of front right corner did not match the expected data.");
}
/** @}*/

/** \defgroup  Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Water_Spray
 *  @{
 */

 /** \brief
  * This test group includes test of the function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() defined in
  * f360_check_if_point_is_inside_box.cpp.
  */
TEST_GROUP(Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Water_Spray)
{
   // Declare common variables used within all tests in this test group.
   float32_t x_point;
   float32_t y_point;
   F360_Object_Track_T obj;
   F360_Detection_Props_T detection_prop;
   F360_Calibrations_T calibs;
   /** \setup
    * Set up the dimensions of the default box used in this test group.
    */
   TEST_SETUP()
   {
      // Set up a default scenario for your tests. E.g. assign values to common variables declared above.
      obj.long_buffer_zone_len1 = 1.0F;
      obj.long_buffer_zone_len2 = 1.0F;
      obj.lat_buffer_zone_wid1 = 1.0F;
      obj.lat_buffer_zone_wid2 = 1.0F;
      detection_prop.f_water_spray = true;
      obj.Update_Bbox_Size(3.0F, 2.0F);

      Initialize_Tracker_Calibrations(calibs);
   }
};

/** \purpose
 * Check that the function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond works as intended when the point is inside
 * the specified box.
 * \req
 * NA
 */
TEST(Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Water_Spray, Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Inside_Box)
{
   /** \precond
   * Prepare point so that it is expected to be inside the box defined in test setup.
   */
   x_point = 1.5F + obj.long_buffer_zone_len2*calibs.k_ws_bbox_len_extension_factor - 0.0001F;
   y_point = 1.0F + obj.lat_buffer_zone_wid2*calibs.k_ws_bbox_wid_extension_factor - 0.0001F;
   Point point{ x_point , y_point };

   /** \action
    * Call Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond().
    */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond(point, obj, detection_prop.f_water_spray, calibs);

   /** \result
    * Check that Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() returns the expected output.
    */
   CHECK_TRUE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() did not return the expected output.")
}

/** \purpose
* Check that the function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond works as intended when the point is outside
* and above the specified box.
* \req
* NA
*/
TEST(Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Water_Spray, Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Outside_Box_Above)
{
   /** \precond
   * Prepare point so that it is expected to be outside and above the box defined in test setup.
   */
   x_point = 2.0F + obj.long_buffer_zone_len2*calibs.k_ws_bbox_len_extension_factor + 0.0001F;
   y_point = 0.5F + obj.lat_buffer_zone_wid2*calibs.k_ws_bbox_wid_extension_factor - 0.0001F;
   Point point{ x_point , y_point };

   /** \action
   * Call Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond().
   */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond(point, obj, detection_prop.f_water_spray, calibs);

   /** \result
   * Check that Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() returns the expected output.
   */
   CHECK_FALSE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() did not return the expected output.")
}

/** \purpose
* Check that the function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond works as intended when the point is outside
* and below the specified box.
* \req
* NA
*/
TEST(Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Water_Spray, Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Outside_Box_Below)
{
   /** \precond
   * Prepare point so that it is expected to be outside and below the box defined in test setup.
   */
   x_point = -(1.5F + obj.long_buffer_zone_len1*calibs.k_ws_bbox_len_extension_factor) - 0.0001F;
   y_point = 1.0F + obj.lat_buffer_zone_wid2*calibs.k_ws_bbox_wid_extension_factor - 0.0001F;
   Point point{ x_point , y_point };

   /** \action
   * Call Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond().
   */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond(point, obj, detection_prop.f_water_spray, calibs);

   /** \result
   * Check that Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() returns the expected output.
   */
   CHECK_FALSE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() did not return the expected output.")
}

/** \purpose
* Check that the function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond works as intended when the point is outside
* and to the right of the specified box.
* \req
* NA
*/
TEST(Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Water_Spray, Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Outside_Box_Right)
{
   /** \precond
   * Prepare point so that it is expected to be outside and to the right of the box defined in test setup.
   */
   x_point = 2.0F + obj.long_buffer_zone_len2*calibs.k_ws_bbox_len_extension_factor - 0.0001F;
   y_point = 0.5F + obj.lat_buffer_zone_wid2*calibs.k_ws_bbox_wid_extension_factor + 0.0001F;
   Point point{ x_point , y_point };

   /** \action
   * Call Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond().
   */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond(point, obj, detection_prop.f_water_spray, calibs);

   /** \result
   * Check that Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() returns the expected output.
   */
   CHECK_FALSE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() did not return the expected output.")
}

/** \purpose
* Check that the function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond works as intended when the point is outside
* and to the left of the specified box.
* \req
* NA
*/
TEST(Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Water_Spray, Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Outside_Box_Left)
{
   /** \precond
   * Prepare point so that it is expected to be outside and to the left of the box defined in test setup.
   */
   x_point = 2.0F + obj.long_buffer_zone_len2*calibs.k_ws_bbox_len_extension_factor - 0.0001F;
   y_point = -(1.5F + obj.lat_buffer_zone_wid1*calibs.k_ws_bbox_wid_extension_factor) - 0.0001F;
   Point point{ x_point , y_point };

   /** \action
   * Call Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond().
   */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond(point, obj, detection_prop.f_water_spray, calibs);

   /** \result
   * Check that Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() returns the expected output.
   */
   CHECK_FALSE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() did not return the expected output.")
}

/** \purpose
* Check that the function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond works as intended when the point is on the corner of
* the specified box.
* \req
* NA
*/
TEST(Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Water_Spray, Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_On_Corner)
{
   /** \precond
   * Prepare point so that it is on the edge of the box defined in test setup, i.e. it should be set as inside.
   * F360_EPSILON used to ensure numerical stability of the test
   */
   x_point = 1.5F + obj.long_buffer_zone_len2 * calibs.k_ws_bbox_len_extension_factor - F360_EPSILON;
   y_point = 1.0F + obj.lat_buffer_zone_wid2 * calibs.k_ws_bbox_wid_extension_factor - F360_EPSILON;
   Point point{ x_point , y_point };

   /** \action
   * Call Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond().
   */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond(point, obj, detection_prop.f_water_spray, calibs);

   /** \result
   * Check that Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() returns the expected output.
   */
   CHECK_TRUE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() did not return the expected output.")
}
/** @}*/

/** \defgroup  Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Default
 *  @{
 */

/** \brief
  * This test group includes test of the function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() defined in
  * f360_check_if_point_is_inside_box.cpp.
  */
TEST_GROUP(Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Default)
{
   // Declare common variables used within all tests in this test group.
   F360_Object_Track_T obj;
   F360_Detection_Props_T detection_prop;
   F360_Calibrations_T calibs;
   /** \setup
    * Set up the dimensions of the default box used in this test group.
    */
   TEST_SETUP()
   {
      // Set up a default scenario for your tests. E.g. assign values to common variables declared above.
      obj.long_buffer_zone_len1 = 1.0F;
      obj.long_buffer_zone_len2 = 1.0F;
      obj.lat_buffer_zone_wid1 = 1.0F;
      obj.lat_buffer_zone_wid2 = 1.0F;
      detection_prop.f_water_spray = false;
      obj.Update_Bbox_Size(3.0F, 2.0F);

      Initialize_Tracker_Calibrations(calibs);
   }
};

/** \purpose
 * Check that the function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond works as intended when the point is inside
 * the specified box.
 * \req
 * NA
 */
TEST(Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Default, Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Inside_Box)
{
   /** \precond
   * Prepare point so that it is expected to be inside the box defined in test setup.
   */
   Point point{ 2.1F, -1.6F };

   /** \action
    * Call Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond().
    */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond(point, obj, detection_prop.f_water_spray, calibs);

   /** \result
    * Check that Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() returns the expected output.
    */
   CHECK_TRUE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() did not return the expected output.")
}

/** \purpose
* Check that the function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond works as intended when the point is outside
* and above the specified box.
* \req
* NA
*/
TEST(Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Default, Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Outside_Box_Above)
{
   /** \precond
   * Prepare point so that it is expected to be outside and above the box defined in test setup.
   */
   Point point{ 3.1F, 0.0F };

   /** \action
   * Call Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond().
   */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond(point, obj, detection_prop.f_water_spray, calibs);

   /** \result
   * Check that Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() returns the expected output.
   */
   CHECK_FALSE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() did not return the expected output.")
}

/** \purpose
* Check that the function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond works as intended when the point is outside
* and below the specified box.
* \req
* NA
*/
TEST(Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Default, Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Outside_Box_Below)
{
   /** \precond
   * Prepare point so that it is expected to be outside and below the box defined in test setup.
   */
   Point point{ -2.6F, 0.5F };

   /** \action
   * Call Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond().
   */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond(point, obj, detection_prop.f_water_spray, calibs);

   /** \result
   * Check that Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() returns the expected output.
   */
   CHECK_FALSE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() did not return the expected output.")
}

/** \purpose
* Check that the function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond works as intended when the point is outside
* and to the right of the specified box.
* \req
* NA
*/
TEST(Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Default, Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Outside_Box_Right)
{
   /** \precond
   * Prepare point so that it is expected to be outside and to the right of the box defined in test setup.
   */
   Point point{ -0.5F, 2.1F };

   /** \action
   * Call Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond().
   */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond(point, obj, detection_prop.f_water_spray, calibs);

   /** \result
   * Check that Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() returns the expected output.
   */
   CHECK_FALSE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() did not return the expected output.")
}

/** \purpose
* Check that the function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond works as intended when the point is outside
* and to the left of the specified box.
* \req
* NA
*/
TEST(Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Default, Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Outside_Box_Left)
{
   /** \precond
   * Prepare point so that it is expected to be outside and to the left of the box defined in test setup.
   */
   Point point{ 0.0F, -2.6F };

   /** \action
   * Call Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond().
   */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond(point, obj, detection_prop.f_water_spray, calibs);

   /** \result
   * Check that Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() returns the expected output.
   */
   CHECK_FALSE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() did not return the expected output.")
}

/** \purpose
* Check that the function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond works as intended when the point is on the corner of
* the specified box.
* \req
* NA
*/
TEST(Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_Default, Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond_On_Corner)
{
   /** \precond
   * Prepare point so that it is on the edge of the box defined in test setup, i.e. it should be set as inside.
   */
   Point point{ -2.5F, -2.0F };

   /** \action
   * Call Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond().
   */
   bool f_is_inside_box = Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond(point, obj, detection_prop.f_water_spray, calibs);

   /** \result
   * Check that Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() returns the expected output.
   */
   CHECK_TRUE_TEXT(f_is_inside_box, "Function Check_If_Point_Is_Inside_Extended_Bounding_Box_Cond() did not return the expected output.")
}
/** @}*/

/** \defgroup  Check_If_Vcs_Point_Is_Inside_Bounding_Box_Group
 *  @{
 */

 /** \brief
   * This test group includes test of the function Check_If_Vcs_Point_Is_Inside_Extended_Bounding_Box_Cond() defined in
   * f360_check_if_point_is_inside_box.cpp.
   */
TEST_GROUP(Check_If_Vcs_Point_Is_Inside_Bounding_Box_Group)
{
   // Declare common variables used within all tests in this test group.
   F360_Object_Track_T obj;

   /** \setup
    * Set up object properties
    */
   TEST_SETUP()
   {
      // Set up a default scenario for your tests. E.g. assign values to common variables declared above.
      obj.Update_Bbox_Size(2.0F, 2.0F);
      obj.vcs_position.x = 10.0F;
      obj.vcs_position.y = 5.0F;
      obj.Set_Bbox_Orientation(Angle{ F360_DEG2RAD(45.0F) });

   }
};

/** \purpose
 * Check that the function Check_If_Vcs_Point_Is_Inside_Bounding_Box works as intended when the point is inside
 * the specified box.
 * \req
 * NA
 */
TEST(Check_If_Vcs_Point_Is_Inside_Bounding_Box_Group, Check_If_Vcs_Point_Is_Inside_Bounding_Box_Inside_Box)
{
   /** \precond
   * Prepare point so that it is expected to be inside the box defined in test setup.
   */
   float32_t x_point_vcs = 10.0F;
   float32_t y_point_vcs = 5.0F;

   /** \action
    * Call Check_If_Vcs_Point_Is_Inside_Bounding_Box().
    */
   bool f_is_inside_box = Check_If_Vcs_Point_Is_Inside_Bounding_Box(x_point_vcs, y_point_vcs, obj);

   /** \result
    * Check that Check_If_Vcs_Point_Is_Inside_Bounding_Box() returns the expected output.
    */
   CHECK_TRUE(f_is_inside_box);
}

/** \purpose
 * Check that the function Check_If_Vcs_Point_Is_Inside_Bounding_Box works as intended when the point is outside
 * the specified box.
 * \req
 * NA
 */
TEST(Check_If_Vcs_Point_Is_Inside_Bounding_Box_Group, Check_If_Vcs_Point_Is_Inside_Bounding_Box_Outside_Box)
{
   /** \precond
   * Prepare point so that it is expected to be outside the box defined in test setup.
   */
   float32_t x_point_vcs = 10.0F;
   float32_t y_point_vcs = -5.0F;

   /** \action
    * Call Check_If_Vcs_Point_Is_Inside_Bounding_Box().
    */
   bool f_is_inside_box = Check_If_Vcs_Point_Is_Inside_Bounding_Box(x_point_vcs, y_point_vcs, obj);

   /** \result
    * Check that Check_If_Vcs_Point_Is_Inside_Bounding_Box() returns the expected output.
    */
   CHECK_FALSE(f_is_inside_box);
}
/** @}*/