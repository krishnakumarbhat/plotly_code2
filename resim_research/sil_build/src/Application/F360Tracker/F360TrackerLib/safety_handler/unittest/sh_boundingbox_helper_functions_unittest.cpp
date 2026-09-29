/** \file
 * This file contains unit tests for content of test_boundingbox_helper_functions.cpp file
 */

#include "sh_boundingbox_helper_functions.h"
#include <CppUTest/TestHarness.h>
#include <cmath>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \brief
 * Helper function to calculate expected bounding box from object parameters.
 * This function replicates the logic of Get_Object_Bounding_Box for test verification.
 *
 * \param test_object The object with position, dimensions, and orientation
 * \return SH_BoundingBox_T The calculated expected bounding box
 */
static SH_BoundingBox_T Calculate_Expected_BoundingBox(const ROT_Object_Output_T &test_object)
{
   SH_BoundingBox_T box_out{};

   // Reference point enum values
   enum
   {
      ROT_OBJECT_REF_POINT_CENTER = 0,
      ROT_OBJECT_REF_POINT_FRONT_LEFT = 1,
      ROT_OBJECT_REF_POINT_FRONT_MID = 2,
      ROT_OBJECT_REF_POINT_FRONT_RIGHT = 3,
      ROT_OBJECT_REF_POINT_RIGHT_MID = 4,
      ROT_OBJECT_REF_POINT_REAR_RIGHT = 5,
      ROT_OBJECT_REF_POINT_REAR_MID = 6,
      ROT_OBJECT_REF_POINT_REAR_LEFT = 7,
      ROT_OBJECT_REF_POINT_LEFT_MID = 8,
      ROT_OBJECT_REF_POINT_UNKNOWN = 255
   };

   const uint8_t reference_pt = test_object.reference_point;
   const float32_t point_angle = test_object.vcs_pointing;
   const float32_t half_length = test_object.length * 0.5F;
   const float32_t half_width = test_object.width * 0.5F;
   const float32_t cos_angle = cosf(point_angle);
   const float32_t sin_angle = sinf(point_angle);
   const float32_t x_pos = test_object.vcs_x_posn;
   const float32_t y_pos = test_object.vcs_y_posn;

   if (reference_pt == ROT_OBJECT_REF_POINT_CENTER)
   {
      box_out.corner_fl.x = x_pos + cos_angle * half_length + sin_angle * half_width;
      box_out.corner_fl.y = y_pos + sin_angle * half_length - cos_angle * half_width;
      box_out.corner_fr.x = x_pos + cos_angle * half_length - sin_angle * half_width;
      box_out.corner_fr.y = y_pos + sin_angle * half_length + cos_angle * half_width;
      box_out.corner_rl.x = x_pos - cos_angle * half_length + sin_angle * half_width;
      box_out.corner_rl.y = y_pos - sin_angle * half_length - cos_angle * half_width;
      box_out.corner_rr.x = x_pos - cos_angle * half_length - sin_angle * half_width;
      box_out.corner_rr.y = y_pos - sin_angle * half_length + cos_angle * half_width;
   }
   else if (reference_pt == ROT_OBJECT_REF_POINT_FRONT_LEFT)
   {
      box_out.corner_fl.x = x_pos;
      box_out.corner_fl.y = y_pos;
      box_out.corner_fr.x = x_pos - sin_angle * test_object.width;
      box_out.corner_fr.y = y_pos + cos_angle * test_object.width;
      box_out.corner_rl.x = x_pos - cos_angle * test_object.length;
      box_out.corner_rl.y = y_pos - sin_angle * test_object.length;
      box_out.corner_rr.x = x_pos - cos_angle * test_object.length - sin_angle * test_object.width;
      box_out.corner_rr.y = y_pos - sin_angle * test_object.length + cos_angle * test_object.width;
   }
   else if (reference_pt == ROT_OBJECT_REF_POINT_FRONT_MID)
   {
      box_out.corner_fl.x = x_pos + sin_angle * half_width;
      box_out.corner_fl.y = y_pos - cos_angle * half_width;
      box_out.corner_fr.x = x_pos - sin_angle * half_width;
      box_out.corner_fr.y = y_pos + cos_angle * half_width;
      box_out.corner_rl.x = x_pos - cos_angle * test_object.length + sin_angle * half_width;
      box_out.corner_rl.y = y_pos - sin_angle * test_object.length - cos_angle * half_width;
      box_out.corner_rr.x = x_pos - cos_angle * test_object.length - sin_angle * half_width;
      box_out.corner_rr.y = y_pos - sin_angle * test_object.length + cos_angle * half_width;
   }
   else if (reference_pt == ROT_OBJECT_REF_POINT_FRONT_RIGHT)
   {
      box_out.corner_fl.x = x_pos + sin_angle * test_object.width;
      box_out.corner_fl.y = y_pos - cos_angle * test_object.width;
      box_out.corner_fr.x = x_pos;
      box_out.corner_fr.y = y_pos;
      box_out.corner_rl.x = x_pos - cos_angle * test_object.length + sin_angle * test_object.width;
      box_out.corner_rl.y = y_pos - sin_angle * test_object.length - cos_angle * test_object.width;
      box_out.corner_rr.x = x_pos - cos_angle * test_object.length;
      box_out.corner_rr.y = y_pos - sin_angle * test_object.length;
   }
   else if (reference_pt == ROT_OBJECT_REF_POINT_RIGHT_MID)
   {
      box_out.corner_fl.x = x_pos + sin_angle * test_object.width + cos_angle * half_length;
      box_out.corner_fl.y = y_pos - cos_angle * test_object.width + sin_angle * half_length;
      box_out.corner_fr.x = x_pos + cos_angle * half_length;
      box_out.corner_fr.y = y_pos + sin_angle * half_length;
      box_out.corner_rl.x = x_pos + sin_angle * test_object.width - cos_angle * half_length;
      box_out.corner_rl.y = y_pos - cos_angle * test_object.width - sin_angle * half_length;
      box_out.corner_rr.x = x_pos - cos_angle * half_length;
      box_out.corner_rr.y = y_pos - sin_angle * half_length;
   }
   else if (reference_pt == ROT_OBJECT_REF_POINT_REAR_RIGHT)
   {
      box_out.corner_fl.x = x_pos + cos_angle * test_object.length + sin_angle * test_object.width;
      box_out.corner_fl.y = y_pos + sin_angle * test_object.length - cos_angle * test_object.width;
      box_out.corner_fr.x = x_pos + cos_angle * test_object.length;
      box_out.corner_fr.y = y_pos + sin_angle * test_object.length;
      box_out.corner_rl.x = x_pos + sin_angle * test_object.width;
      box_out.corner_rl.y = y_pos - cos_angle * test_object.width;
      box_out.corner_rr.x = x_pos;
      box_out.corner_rr.y = y_pos;
   }
   else if (reference_pt == ROT_OBJECT_REF_POINT_REAR_MID)
   {
      box_out.corner_fl.x = x_pos + cos_angle * test_object.length + sin_angle * half_width;
      box_out.corner_fl.y = y_pos + sin_angle * test_object.length - cos_angle * half_width;
      box_out.corner_fr.x = x_pos + cos_angle * test_object.length - sin_angle * half_width;
      box_out.corner_fr.y = y_pos + sin_angle * test_object.length + cos_angle * half_width;
      box_out.corner_rl.x = x_pos + sin_angle * half_width;
      box_out.corner_rl.y = y_pos - cos_angle * half_width;
      box_out.corner_rr.x = x_pos - sin_angle * half_width;
      box_out.corner_rr.y = y_pos + cos_angle * half_width;
   }
   else if (reference_pt == ROT_OBJECT_REF_POINT_REAR_LEFT)
   {
      box_out.corner_fl.x = x_pos + cos_angle * test_object.length;
      box_out.corner_fl.y = y_pos + sin_angle * test_object.length;
      box_out.corner_fr.x = x_pos + cos_angle * test_object.length - sin_angle * test_object.width;
      box_out.corner_fr.y = y_pos + sin_angle * test_object.length + cos_angle * test_object.width;
      box_out.corner_rl.x = x_pos;
      box_out.corner_rl.y = y_pos;
      box_out.corner_rr.x = x_pos - sin_angle * test_object.width;
      box_out.corner_rr.y = y_pos + cos_angle * test_object.width;
   }
   else if (reference_pt == ROT_OBJECT_REF_POINT_LEFT_MID)
   {
      box_out.corner_fl.x = x_pos + cos_angle * half_length;
      box_out.corner_fl.y = y_pos + sin_angle * half_length;
      box_out.corner_fr.x = x_pos - sin_angle * test_object.width + cos_angle * half_length;
      box_out.corner_fr.y = y_pos + cos_angle * test_object.width + sin_angle * half_length;
      box_out.corner_rl.x = x_pos - cos_angle * half_length;
      box_out.corner_rl.y = y_pos - sin_angle * half_length;
      box_out.corner_rr.x = x_pos - sin_angle * test_object.width - cos_angle * half_length;
      box_out.corner_rr.y = y_pos + cos_angle * test_object.width - sin_angle * half_length;
   }
   else
   {
      // Default to center if unknown reference point
      box_out.corner_fl.x = x_pos + cos_angle * half_length + sin_angle * half_width;
      box_out.corner_fl.y = y_pos + sin_angle * half_length - cos_angle * half_width;
      box_out.corner_fr.x = x_pos + cos_angle * half_length - sin_angle * half_width;
      box_out.corner_fr.y = y_pos + sin_angle * half_length + cos_angle * half_width;
      box_out.corner_rl.x = x_pos - cos_angle * half_length + sin_angle * half_width;
      box_out.corner_rl.y = y_pos - sin_angle * half_length - cos_angle * half_width;
      box_out.corner_rr.x = x_pos - cos_angle * half_length - sin_angle * half_width;
      box_out.corner_rr.y = y_pos - sin_angle * half_length + cos_angle * half_width;
   }

   return box_out;
}

/** \brief
 * Helper function to calculate expected min and max points from a bounding box.
 * This function replicates the logic of Get_Axis_Aligned_Rect_MinMax_Pt for test verification.
 *
 * \param bbox The bounding box with four corner points
 * \param offset_x The x-axis offset to apply
 * \param offset_y The y-axis offset to apply
 * \param expected_max_point Output parameter for expected maximum point
 * \param expected_min_point Output parameter for expected minimum point
 */
static void Calculate_Expected_MinMax_Points(
    const SH_BoundingBox_T &bbox,
    const float32_t offset_x,
    const float32_t offset_y,
    SH_Point_T &expected_max_point,
    SH_Point_T &expected_min_point)
{
   // Find min and max X coordinates among all four corners
   const float32_t min_x = fminf(fminf(bbox.corner_fl.x, bbox.corner_fr.x),
                                 fminf(bbox.corner_rl.x, bbox.corner_rr.x));
   const float32_t max_x = fmaxf(fmaxf(bbox.corner_fl.x, bbox.corner_fr.x),
                                 fmaxf(bbox.corner_rl.x, bbox.corner_rr.x));

   // Find min and max Y coordinates among all four corners
   const float32_t min_y = fminf(fminf(bbox.corner_fl.y, bbox.corner_fr.y),
                                 fminf(bbox.corner_rl.y, bbox.corner_rr.y));
   const float32_t max_y = fmaxf(fmaxf(bbox.corner_fl.y, bbox.corner_fr.y),
                                 fmaxf(bbox.corner_rl.y, bbox.corner_rr.y));

   // Apply offsets to create axis-aligned rectangle
   expected_max_point.x = max_x + offset_x;
   expected_max_point.y = max_y + offset_y;
   expected_min_point.x = min_x - offset_x;
   expected_min_point.y = min_y - offset_y;
}

/** \brief
 * Helper function to validate bounding box corners against expected values.
 *
 * \param output_box The actual bounding box output from the function under test
 * \param expected_output_box The expected bounding box values
 */
static void Validate_BoundingBox(
    const SH_BoundingBox_T &output_box,
    const SH_BoundingBox_T &expected_output_box)
{
   const float32_t test_pass_thres = 1.0e-7F;

   DOUBLES_EQUAL(expected_output_box.corner_fl.x, output_box.corner_fl.x, test_pass_thres);
   DOUBLES_EQUAL(expected_output_box.corner_fl.y, output_box.corner_fl.y, test_pass_thres);
   DOUBLES_EQUAL(expected_output_box.corner_fr.x, output_box.corner_fr.x, test_pass_thres);
   DOUBLES_EQUAL(expected_output_box.corner_fr.y, output_box.corner_fr.y, test_pass_thres);
   DOUBLES_EQUAL(expected_output_box.corner_rl.x, output_box.corner_rl.x, test_pass_thres);
   DOUBLES_EQUAL(expected_output_box.corner_rl.y, output_box.corner_rl.y, test_pass_thres);
   DOUBLES_EQUAL(expected_output_box.corner_rr.x, output_box.corner_rr.x, test_pass_thres);
   DOUBLES_EQUAL(expected_output_box.corner_rr.y, output_box.corner_rr.y, test_pass_thres);
}

/** \defgroup  test_Boundingbox_Helper_Functions
 *  @{
 */

/** \brief
 * Test the functionality of bounding box helper functions.
 */
TEST_GROUP(test_Boundingbox_Helper_Functions)
{
   // Declare common variables used within all tests in this test group.
   const float32_t PI = 3.1415927f;
   const float32_t deg_to_rad = PI / 180.0f;
   const float32_t test_pass_thres = 1.0e-7F;
   ROT_Object_Output_T test_object{};
   enum
   {
      ROT_OBJECT_REF_POINT_CENTER = 0,
      ROT_OBJECT_REF_POINT_FRONT_LEFT = 1,
      ROT_OBJECT_REF_POINT_FRONT_MID = 2,
      ROT_OBJECT_REF_POINT_FRONT_RIGHT = 3,
      ROT_OBJECT_REF_POINT_RIGHT_MID = 4,
      ROT_OBJECT_REF_POINT_REAR_RIGHT = 5,
      ROT_OBJECT_REF_POINT_REAR_MID = 6,
      ROT_OBJECT_REF_POINT_REAR_LEFT = 7,
      ROT_OBJECT_REF_POINT_LEFT_MID = 8,
      ROT_OBJECT_REF_POINT_UNKNOWN = 255
   };

   /** \teardown
    * reset the test_bbox and test_point after each test case.
    */
   TEST_TEARDOWN()
   {
      test_object.vcs_x_posn = 10.0f;
      test_object.vcs_y_posn = 0.0f;
      test_object.vcs_pointing = 0.0f;
      test_object.length = 4.0f;
      test_object.width = 2.0f;
      test_object.reference_point = ROT_OBJECT_REF_POINT_CENTER;
   }
};

/** \purpose
 * Test Get_Axis_Aligned_Rect_MinMax_Pt function with various object orientations.
 * Verifies that the function correctly calculates axis-aligned bounding rectangle
 * min/max points with offsets. Tests specific angles where each corner becomes
 * the extremal point (min/max) to ensure all corner cases are covered.
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Get_Axis_Aligned_Rect_MinMax_Pt_All_Corners)
{
   /** \precond
    * Set up object with standard dimensions and offsets for min/max point calculation
    */
   const float32_t offset_x = 3.0f;
   const float32_t offset_y = 2.0f;

   /** \action & \result
    * Test specific angles where different corners become the extremal points.
    * For a rectangle, each corner becomes min/max at different orientations:
    * - 0 deg: Front corners are max X, rear corners are min X
    * - 45 deg: Diagonal corners become extremal in both axes
    * - 90 deg: Left corners are max Y, right corners are min Y
    * - 135 deg: Opposite diagonal corners become extremal
    * - 180 deg: Rear corners are max X, front corners are min X
    * - 225 deg: Diagonal corners switch roles
    * - 270 deg: Right corners are max Y, left corners are min Y
    * - 315 deg: Final diagonal configuration
    *
    * Additionally test intermediate angles to ensure smooth transitions.
    */
   const int32_t test_angles[] = {
       // Cardinal angles - each side faces a primary axis
       0, 90, 180, 270,
       // Diagonal angles - corners align with axes
       45, 135, 225, 315,
       // Intermediate angles - ensure smooth transitions
       22, 67, 112, 157, 202, 247, 292, 337,
       // Edge cases
       -180, -90, -45, 360,
       // Fine-grained coverage near transitions
       43, 44, 46, 47, 88, 89, 91, 92};

   for (const int32_t angle_deg : test_angles)
   {
      test_object.vcs_pointing = static_cast<float32_t>(angle_deg) * deg_to_rad;

      SH_BoundingBox_T test_bbox{};
      Get_Object_Bounding_Box(test_object, test_bbox);

      SH_Point_T max_point_out{};
      SH_Point_T min_point_out{};
      Get_Axis_Aligned_Rect_MinMax_Pt(test_bbox, offset_x, offset_y, max_point_out, min_point_out);

      SH_Point_T expected_max_point{};
      SH_Point_T expected_min_point{};
      Calculate_Expected_MinMax_Points(test_bbox, offset_x, offset_y, expected_max_point, expected_min_point);

      DOUBLES_EQUAL(expected_max_point.x, max_point_out.x, F360_EPSILON);
      DOUBLES_EQUAL(expected_max_point.y, max_point_out.y, F360_EPSILON);
      DOUBLES_EQUAL(expected_min_point.x, min_point_out.x, F360_EPSILON);
      DOUBLES_EQUAL(expected_min_point.y, min_point_out.y, F360_EPSILON);
   }
}

/** \purpose
 * Test Is_Point_Inside_Bounding_Box function with points inside the rectangle.
 * Verifies that points at various locations within the axis-aligned bounding box
 * are correctly identified as being inside.
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Is_Point_Inside_Axis_Aligned_Rect_Inside)
{
   /** \precond
    * Define axis-aligned bounding box min/max points
    */
   const SH_Point_T max_point{8.0f, 1.0f};
   const SH_Point_T min_point{2.0f, -3.0f};

   /** \action & \result
    * Test various points inside the bounding box using a test vector:
    * - Center point
    * - Points on boundaries (edge cases - should be inside due to >= and <= checks)
    * - Corner points (all four corners)
    * - Points near boundaries but clearly inside
    * - Random interior points
    */

   const SH_Point_T test_points[] = {
       // Center of the box
       {5.0f, -1.0f},

       // Points on boundaries (edge cases - should be inside)
       {2.0f, -1.0f}, // on min X
       {8.0f, -1.0f}, // on max X
       {5.0f, -3.0f}, // on min Y
       {5.0f, 1.0f},  // on max Y

       // Corner points (should be inside)
       {2.0f, -3.0f}, // min X, min Y
       {8.0f, -3.0f}, // max X, min Y
       {2.0f, 1.0f},  // min X, max Y
       {8.0f, 1.0f},  // max X, max Y

       // Points near boundaries but clearly inside
       {2.5f, -1.0f}, // near min X
       {7.5f, -1.0f}, // near max X
       {5.0f, -2.5f}, // near min Y
       {5.0f, 0.5f},  // near max Y

       // Random interior points
       {3.0f, 0.0f},
       {7.0f, -2.0f},
       {4.0f, -0.5f}};

   for (const auto &test_point : test_points)
   {
      CHECK_TRUE(Is_Point_Inside_Bounding_Box(test_point, max_point, min_point));
   }
}

/** \purpose
 * Test Is_Point_Inside_Bounding_Box function with points outside the rectangle.
 * Verifies that points at various locations outside the axis-aligned bounding box
 * are correctly identified as being outside.
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Is_Point_Inside_Axis_Aligned_Rect_Outside)
{
   /** \precond
    * Define axis-aligned bounding box min/max points
    * Box boundaries: X[0.0, 5.0], Y[-3.0, 2.0]
    */
   const SH_Point_T max_point{5.0f, 2.0f};
   const SH_Point_T min_point{0.0f, -3.0f};

   /** \action & \result
    * Test various points outside the bounding box using a test vector:
    * - Points beyond each boundary (just outside)
    * - Points beyond each corner
    * - Points far outside the box
    * - Points outside in one dimension but inside in the other
    */

   const SH_Point_T test_points[] = {
       // Points just beyond each boundary
       {-0.1f, 0.0f}, // just beyond min X
       {5.1f, 0.0f},  // just beyond max X
       {2.5f, -3.1f}, // just beyond min Y
       {2.5f, 2.1f},  // just beyond max Y

       // Points beyond each corner
       {-0.5f, -3.5f}, // beyond min X, min Y corner
       {5.5f, -3.5f},  // beyond max X, min Y corner
       {-0.5f, 2.5f},  // beyond min X, max Y corner
       {5.5f, 2.5f},   // beyond max X, max Y corner

       // Points far outside the box
       {-10.0f, 0.0f}, // far left
       {15.0f, 0.0f},  // far right
       {2.5f, -10.0f}, // far below
       {2.5f, 10.0f},  // far above

       // Points outside in X, inside in Y
       {-1.0f, 0.0f},
       {6.0f, -1.0f},

       // Points outside in Y, inside in X
       {2.0f, -4.0f},
       {3.0f, 3.0f},

       // Points outside in both dimensions
       {-2.0f, -5.0f},
       {7.0f, 4.0f},
       {-1.0f, 5.0f},
       {8.0f, -6.0f}};

   for (const auto &test_point : test_points)
   {
      CHECK_FALSE(Is_Point_Inside_Bounding_Box(test_point, max_point, min_point));
   }
}

/** \purpose
 * Test Get_Object_Bounding_Box with various pointing angles from -360 to 360 degrees
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Get_Object_Bounding_Box_All_Angles)
{
   /** \precond
    * Test object at various angles from -360 to 360 degrees in 5 degree increments
    */

   /** \action & \result
    * Loop through all angles from -360 to 360 degrees in 5 degree intervals
    * For each angle, loop through all reference points and verify bounding box calculation
    */
   for (int32_t angle_deg = -360; angle_deg <= 360; angle_deg += 5)
   {
      test_object.vcs_pointing = static_cast<float32_t>(angle_deg) * deg_to_rad;

      for (uint8_t ref_pt = ROT_OBJECT_REF_POINT_CENTER; ref_pt <= ROT_OBJECT_REF_POINT_LEFT_MID; ++ref_pt)
      {
         test_object.reference_point = ref_pt;

         SH_BoundingBox_T output_box{};
         Get_Object_Bounding_Box(test_object, output_box);

         SH_BoundingBox_T expected_output_box = Calculate_Expected_BoundingBox(test_object);
         Validate_BoundingBox(output_box, expected_output_box);
      }
   }
}

/** \purpose
 * Test Get_Object_Bounding_Box with zero dimensions.
 * Edge case for point-like objects with no size.
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Get_Object_Bounding_Box_Zero_Dimensions)
{
   /** \precond
    * Object with zero length and width
    */
   test_object.length = 0.0f;
   test_object.width = 0.0f;
   test_object.vcs_pointing = 0.0f;

   /** \action
    * Get bounding box for zero-size object
    */
   SH_BoundingBox_T output_box{};
   Get_Object_Bounding_Box(test_object, output_box);

   /** \result
    * All corners should collapse to the object position (center)
    */
   DOUBLES_EQUAL(test_object.vcs_x_posn, output_box.corner_fl.x, test_pass_thres);
   DOUBLES_EQUAL(test_object.vcs_y_posn, output_box.corner_fl.y, test_pass_thres);
   DOUBLES_EQUAL(test_object.vcs_x_posn, output_box.corner_fr.x, test_pass_thres);
   DOUBLES_EQUAL(test_object.vcs_y_posn, output_box.corner_fr.y, test_pass_thres);
   DOUBLES_EQUAL(test_object.vcs_x_posn, output_box.corner_rl.x, test_pass_thres);
   DOUBLES_EQUAL(test_object.vcs_y_posn, output_box.corner_rl.y, test_pass_thres);
   DOUBLES_EQUAL(test_object.vcs_x_posn, output_box.corner_rr.x, test_pass_thres);
   DOUBLES_EQUAL(test_object.vcs_y_posn, output_box.corner_rr.y, test_pass_thres);
}

/** \purpose
 * Test Get_Object_Bounding_Box with very large dimensions.
 * Edge case for extremely large objects (e.g., building-sized).
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Get_Object_Bounding_Box_Very_Large_Dimensions)
{
   /** \precond
    * Object with very large dimensions (100m x 50m)
    */
   test_object.length = 100.0f;
   test_object.width = 50.0f;
   test_object.vcs_pointing = 0.0f;

   /** \action
    * Get bounding box and verify against expected values
    */
   SH_BoundingBox_T output_box{};
   Get_Object_Bounding_Box(test_object, output_box);

   SH_BoundingBox_T expected_output_box = Calculate_Expected_BoundingBox(test_object);

   /** \result
    * Bounding box should match expected calculation
    */
   Validate_BoundingBox(output_box, expected_output_box);
}

/** \purpose
 * Test Get_Object_Bounding_Box with very asymmetric dimensions.
 * Tests long narrow object (trailer-like) and wide short object.
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Get_Object_Bounding_Box_Asymmetric_Dimensions)
{
   /** \precond & \action & \result
    * Test both long-narrow and wide-short configurations
    */

   // Case 1: Very long, narrow object (20m x 1m - trailer)
   test_object.length = 20.0f;
   test_object.width = 1.0f;
   test_object.vcs_pointing = 0.0f;

   SH_BoundingBox_T output_box_long{};
   Get_Object_Bounding_Box(test_object, output_box_long);
   SH_BoundingBox_T expected_box_long = Calculate_Expected_BoundingBox(test_object);
   Validate_BoundingBox(output_box_long, expected_box_long);

   // Case 2: Very wide, short object (3m x 10m - wide platform)
   test_object.length = 3.0f;
   test_object.width = 10.0f;

   SH_BoundingBox_T output_box_wide{};
   Get_Object_Bounding_Box(test_object, output_box_wide);
   SH_BoundingBox_T expected_box_wide = Calculate_Expected_BoundingBox(test_object);
   Validate_BoundingBox(output_box_wide, expected_box_wide);
}

/** \purpose
 * Test Get_Object_Bounding_Box with object at origin.
 * Edge case for object centered at (0,0).
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Get_Object_Bounding_Box_At_Origin)
{
   /** \precond
    * Object at origin position
    */
   test_object.vcs_x_posn = 0.0f;
   test_object.vcs_y_posn = 0.0f;
   test_object.vcs_pointing = 0.0f;

   /** \action
    * Get bounding box for object at origin
    */
   SH_BoundingBox_T output_box{};
   Get_Object_Bounding_Box(test_object, output_box);

   SH_BoundingBox_T expected_output_box = Calculate_Expected_BoundingBox(test_object);

   /** \result
    * Bounding box should be correctly calculated around origin
    */
   Validate_BoundingBox(output_box, expected_output_box);
}

/** \purpose
 * Test Get_Object_Bounding_Box with negative object position.
 * Tests objects behind or to the left of the sensor.
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Get_Object_Bounding_Box_Negative_Position)
{
   /** \precond
    * Object at negative X and Y positions
    */
   test_object.vcs_x_posn = -15.0f;
   test_object.vcs_y_posn = -8.0f;
   test_object.vcs_pointing = 0.785f; // 45 degrees

   /** \action
    * Get bounding box for negatively positioned object
    */
   SH_BoundingBox_T output_box{};
   Get_Object_Bounding_Box(test_object, output_box);

   SH_BoundingBox_T expected_output_box = Calculate_Expected_BoundingBox(test_object);

   /** \result
    * Bounding box should be correctly calculated with negative coordinates
    */
   Validate_BoundingBox(output_box, expected_output_box);
}

/** \purpose
 * Test Get_Object_Bounding_Box with unknown/invalid reference point (255).
 * Should default to center reference point behavior.
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Get_Object_Bounding_Box_Unknown_Reference_Point)
{
   /** \precond
    * Object with unknown reference point value (255)
    */
   test_object.reference_point = ROT_OBJECT_REF_POINT_UNKNOWN;
   test_object.vcs_pointing = 0.0f;

   /** \action
    * Get bounding box with unknown reference point
    */
   SH_BoundingBox_T output_box{};
   Get_Object_Bounding_Box(test_object, output_box);

   // Expected behavior: should default to center reference point
   test_object.reference_point = ROT_OBJECT_REF_POINT_CENTER;
   SH_BoundingBox_T expected_output_box = Calculate_Expected_BoundingBox(test_object);

   /** \result
    * Should default to center reference point calculation
    */
   Validate_BoundingBox(output_box, expected_output_box);
}

/** \purpose
 * Test Get_Object_Bounding_Box with extreme angle (> 2*PI).
 * Tests angle normalization/wrapping behavior.
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Get_Object_Bounding_Box_Extreme_Angle)
{
   /** \precond
    * Object with angle > 2*PI (should behave like wrapped angle)
    */
   test_object.vcs_pointing = 3.0f * PI; // 540 degrees = 180 degrees wrapped

   /** \action
    * Get bounding box with extreme angle
    */
   SH_BoundingBox_T output_box{};
   Get_Object_Bounding_Box(test_object, output_box);

   SH_BoundingBox_T expected_output_box = Calculate_Expected_BoundingBox(test_object);

   /** \result
    * Should handle extreme angles correctly (via sin/cos wrapping)
    */
   Validate_BoundingBox(output_box, expected_output_box);
}

/** \purpose
 * Test Get_Object_Bounding_Box with very small dimensions.
 * Edge case for tiny objects like motorcycles or small debris.
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Get_Object_Bounding_Box_Very_Small_Dimensions)
{
   /** \precond
    * Object with very small dimensions (0.1m x 0.1m)
    */
   test_object.length = 0.1f;
   test_object.width = 0.1f;
   test_object.vcs_pointing = 1.57f; // 90 degrees

   /** \action
    * Get bounding box for very small object
    */
   SH_BoundingBox_T output_box{};
   Get_Object_Bounding_Box(test_object, output_box);

   SH_BoundingBox_T expected_output_box = Calculate_Expected_BoundingBox(test_object);

   /** \result
    * Should correctly calculate small bounding box
    */
   Validate_BoundingBox(output_box, expected_output_box);
}

/** \purpose
 * Test Get_Axis_Aligned_Rect_MinMax_Pt with zero offsets.
 * Edge case where extended rectangle equals original bounding box bounds.
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Get_Axis_Aligned_Rect_MinMax_Pt_Zero_Offsets)
{
   /** \precond
    * Standard object with zero offsets
    */
   test_object.vcs_pointing = 0.785f; // 45 degrees

   SH_BoundingBox_T test_bbox{};
   Get_Object_Bounding_Box(test_object, test_bbox);

   /** \action
    * Get min/max points with zero offsets
    */
   SH_Point_T max_point_out{};
   SH_Point_T min_point_out{};
   Get_Axis_Aligned_Rect_MinMax_Pt(test_bbox, 0.0f, 0.0f, max_point_out, min_point_out);

   SH_Point_T expected_max_point{};
   SH_Point_T expected_min_point{};
   Calculate_Expected_MinMax_Points(test_bbox, 0.0f, 0.0f, expected_max_point, expected_min_point);

   /** \result
    * Min/max should match bounding box extents exactly (no extension)
    */
   DOUBLES_EQUAL(expected_max_point.x, max_point_out.x, test_pass_thres);
   DOUBLES_EQUAL(expected_max_point.y, max_point_out.y, test_pass_thres);
   DOUBLES_EQUAL(expected_min_point.x, min_point_out.x, test_pass_thres);
   DOUBLES_EQUAL(expected_min_point.y, min_point_out.y, test_pass_thres);
}

/** \purpose
 * Test Get_Axis_Aligned_Rect_MinMax_Pt with negative offsets.
 * Negative offsets should shrink the bounding box instead of expanding it.
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Get_Axis_Aligned_Rect_MinMax_Pt_Negative_Offsets)
{
   /** \precond
    * Standard object with negative offsets (should shrink the box)
    */
   test_object.vcs_pointing = 0.0f;

   SH_BoundingBox_T test_bbox{};
   Get_Object_Bounding_Box(test_object, test_bbox);

   /** \action
    * Get min/max points with negative offsets
    */
   const float32_t offset_x = -1.0f;
   const float32_t offset_y = -0.5f;

   SH_Point_T max_point_out{};
   SH_Point_T min_point_out{};
   Get_Axis_Aligned_Rect_MinMax_Pt(test_bbox, offset_x, offset_y, max_point_out, min_point_out);

   SH_Point_T expected_max_point{};
   SH_Point_T expected_min_point{};
   Calculate_Expected_MinMax_Points(test_bbox, offset_x, offset_y, expected_max_point, expected_min_point);

   /** \result
    * Rectangle should be shrunk by negative offsets
    */
   DOUBLES_EQUAL(expected_max_point.x, max_point_out.x, test_pass_thres);
   DOUBLES_EQUAL(expected_max_point.y, max_point_out.y, test_pass_thres);
   DOUBLES_EQUAL(expected_min_point.x, min_point_out.x, test_pass_thres);
   DOUBLES_EQUAL(expected_min_point.y, min_point_out.y, test_pass_thres);
}

/** \purpose
 * Test Get_Axis_Aligned_Rect_MinMax_Pt with very large offsets.
 * Edge case for massive tolerance regions.
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Get_Axis_Aligned_Rect_MinMax_Pt_Very_Large_Offsets)
{
   /** \precond
    * Standard object with very large offsets
    */
   test_object.vcs_pointing = 0.0f;

   SH_BoundingBox_T test_bbox{};
   Get_Object_Bounding_Box(test_object, test_bbox);

   /** \action
    * Get min/max points with very large offsets (50m)
    */
   const float32_t offset_x = 50.0f;
   const float32_t offset_y = 50.0f;

   SH_Point_T max_point_out{};
   SH_Point_T min_point_out{};
   Get_Axis_Aligned_Rect_MinMax_Pt(test_bbox, offset_x, offset_y, max_point_out, min_point_out);

   SH_Point_T expected_max_point{};
   SH_Point_T expected_min_point{};
   Calculate_Expected_MinMax_Points(test_bbox, offset_x, offset_y, expected_max_point, expected_min_point);

   /** \result
    * Should handle large offsets correctly
    */
   DOUBLES_EQUAL(expected_max_point.x, max_point_out.x, test_pass_thres);
   DOUBLES_EQUAL(expected_max_point.y, max_point_out.y, test_pass_thres);
   DOUBLES_EQUAL(expected_min_point.x, min_point_out.x, test_pass_thres);
   DOUBLES_EQUAL(expected_min_point.y, min_point_out.y, test_pass_thres);
}

/** \purpose
 * Test Get_Axis_Aligned_Rect_MinMax_Pt with asymmetric offsets.
 * Tests different X and Y offsets.
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Get_Axis_Aligned_Rect_MinMax_Pt_Asymmetric_Offsets)
{
   /** \precond
    * Rotated object with very different X and Y offsets
    */
   test_object.vcs_pointing = 0.523f; // 30 degrees

   SH_BoundingBox_T test_bbox{};
   Get_Object_Bounding_Box(test_object, test_bbox);

   /** \action
    * Get min/max points with asymmetric offsets
    */
   const float32_t offset_x = 10.0f; // Large X offset
   const float32_t offset_y = 1.0f;  // Small Y offset

   SH_Point_T max_point_out{};
   SH_Point_T min_point_out{};
   Get_Axis_Aligned_Rect_MinMax_Pt(test_bbox, offset_x, offset_y, max_point_out, min_point_out);

   SH_Point_T expected_max_point{};
   SH_Point_T expected_min_point{};
   Calculate_Expected_MinMax_Points(test_bbox, offset_x, offset_y, expected_max_point, expected_min_point);

   /** \result
    * Should apply different offsets independently to each axis
    */
   DOUBLES_EQUAL(expected_max_point.x, max_point_out.x, test_pass_thres);
   DOUBLES_EQUAL(expected_max_point.y, max_point_out.y, test_pass_thres);
   DOUBLES_EQUAL(expected_min_point.x, min_point_out.x, test_pass_thres);
   DOUBLES_EQUAL(expected_min_point.y, min_point_out.y, test_pass_thres);
}

/** \purpose
 * Test Get_Axis_Aligned_Rect_MinMax_Pt with degenerate bounding box.
 * All corners at the same point (zero-size object).
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Get_Axis_Aligned_Rect_MinMax_Pt_Degenerate_Box)
{
   /** \precond
    * Zero-size object creates degenerate bounding box
    */
   test_object.length = 0.0f;
   test_object.width = 0.0f;
   test_object.vcs_x_posn = 5.0f;
   test_object.vcs_y_posn = 3.0f;

   SH_BoundingBox_T test_bbox{};
   Get_Object_Bounding_Box(test_object, test_bbox);

   /** \action
    * Get min/max points with standard offsets on degenerate box
    */
   const float32_t offset_x = 2.0f;
   const float32_t offset_y = 1.5f;

   SH_Point_T max_point_out{};
   SH_Point_T min_point_out{};
   Get_Axis_Aligned_Rect_MinMax_Pt(test_bbox, offset_x, offset_y, max_point_out, min_point_out);

   /** \result
    * Should create rectangle centered on object position with size = 2*offset
    */
   DOUBLES_EQUAL(test_object.vcs_x_posn + offset_x, max_point_out.x, test_pass_thres);
   DOUBLES_EQUAL(test_object.vcs_y_posn + offset_y, max_point_out.y, test_pass_thres);
   DOUBLES_EQUAL(test_object.vcs_x_posn - offset_x, min_point_out.x, test_pass_thres);
   DOUBLES_EQUAL(test_object.vcs_y_posn - offset_y, min_point_out.y, test_pass_thres);
}

/** \purpose
 * Test Is_Point_Inside_Bounding_Box with degenerate box (min == max).
 * Point-like bounding box where only exact match should return true.
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Is_Point_Inside_Degenerate_Box)
{
   /** \precond
    * Degenerate box where min equals max (single point)
    */
   const SH_Point_T max_point{5.0f, 3.0f};
   const SH_Point_T min_point{5.0f, 3.0f};

   /** \action & \result
    * Only point exactly at the box location should be inside
    */
   const SH_Point_T point_exact{5.0f, 3.0f};
   CHECK_TRUE(Is_Point_Inside_Bounding_Box(point_exact, max_point, min_point));

   const SH_Point_T point_near{5.001f, 3.0f};
   CHECK_FALSE(Is_Point_Inside_Bounding_Box(point_near, max_point, min_point));

   const SH_Point_T point_far{6.0f, 4.0f};
   CHECK_FALSE(Is_Point_Inside_Bounding_Box(point_far, max_point, min_point));
}

/** \purpose
 * Test Is_Point_Inside_Bounding_Box with inverted box (min > max).
 * Malformed box where min/max are swapped - should always return false.
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Is_Point_Inside_Inverted_Box)
{
   /** \precond
    * Inverted box where min > max (malformed)
    */
   const SH_Point_T max_point{2.0f, 1.0f}; // Actually smaller values
   const SH_Point_T min_point{8.0f, 5.0f}; // Actually larger values

   /** \action & \result
    * All points should be outside inverted box
    */
   const SH_Point_T test_points[] = {
       {5.0f, 3.0f},  // Center of what would be normal box
       {2.0f, 1.0f},  // At "max" point
       {8.0f, 5.0f},  // At "min" point
       {0.0f, 0.0f},  // Origin
       {10.0f, 10.0f} // Far away
   };

   for (const auto &test_point : test_points)
   {
      CHECK_FALSE(Is_Point_Inside_Bounding_Box(test_point, max_point, min_point));
   }
}

/** \purpose
 * Test Is_Point_Inside_Bounding_Box with very small box.
 * Tiny bounding box to test numerical precision.
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Is_Point_Inside_Very_Small_Box)
{
   /** \precond
    * Very small bounding box (0.01m x 0.01m)
    */
   const SH_Point_T max_point{10.005f, 5.005f};
   const SH_Point_T min_point{9.995f, 4.995f};

   /** \action & \result
    * Test points inside, on boundary, and outside small box
    */

   // Point at center (inside)
   const SH_Point_T point_center{10.0f, 5.0f};
   CHECK_TRUE(Is_Point_Inside_Bounding_Box(point_center, max_point, min_point));

   // Point on boundary (inside due to >= and <=)
   const SH_Point_T point_boundary{10.005f, 5.005f};
   CHECK_TRUE(Is_Point_Inside_Bounding_Box(point_boundary, max_point, min_point));

   // Point just outside
   const SH_Point_T point_outside{10.006f, 5.0f};
   CHECK_FALSE(Is_Point_Inside_Bounding_Box(point_outside, max_point, min_point));
}

/** \purpose
 * Test Is_Point_Inside_Bounding_Box with very large box.
 * Huge bounding box covering large area.
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Is_Point_Inside_Very_Large_Box)
{
   /** \precond
    * Very large bounding box (1000m x 1000m)
    */
   const SH_Point_T max_point{500.0f, 500.0f};
   const SH_Point_T min_point{-500.0f, -500.0f};

   /** \action & \result
    * Test various points relative to large box
    */

   // Points inside
   const SH_Point_T points_inside[] = {
       {0.0f, 0.0f},       // Center
       {250.0f, 250.0f},   // Quadrant 1
       {-250.0f, 250.0f},  // Quadrant 2
       {-250.0f, -250.0f}, // Quadrant 3
       {250.0f, -250.0f}   // Quadrant 4
   };

   for (const auto &test_point : points_inside)
   {
      CHECK_TRUE(Is_Point_Inside_Bounding_Box(test_point, max_point, min_point));
   }

   // Points outside
   const SH_Point_T points_outside[] = {
       {501.0f, 0.0f},    // Beyond max X
       {-501.0f, 0.0f},   // Beyond min X
       {0.0f, 501.0f},    // Beyond max Y
       {0.0f, -501.0f},   // Beyond min Y
       {1000.0f, 1000.0f} // Far outside
   };

   for (const auto &test_point : points_outside)
   {
      CHECK_FALSE(Is_Point_Inside_Bounding_Box(test_point, max_point, min_point));
   }
}

/** \purpose
 * Test Is_Point_Inside_Bounding_Box with points at exact corner positions.
 * Validates boundary inclusivity at all four corners.
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Is_Point_Inside_At_Exact_Corners)
{
   /** \precond
    * Standard bounding box
    */
   const SH_Point_T max_point{10.0f, 5.0f};
   const SH_Point_T min_point{0.0f, -5.0f};

   /** \action & \result
    * All four corner points should be inside (due to >= and <=)
    */
   const SH_Point_T corner_max_max{10.0f, 5.0f};  // Top right
   const SH_Point_T corner_max_min{10.0f, -5.0f}; // Bottom right
   const SH_Point_T corner_min_max{0.0f, 5.0f};   // Top left
   const SH_Point_T corner_min_min{0.0f, -5.0f};  // Bottom left

   CHECK_TRUE(Is_Point_Inside_Bounding_Box(corner_max_max, max_point, min_point));
   CHECK_TRUE(Is_Point_Inside_Bounding_Box(corner_max_min, max_point, min_point));
   CHECK_TRUE(Is_Point_Inside_Bounding_Box(corner_min_max, max_point, min_point));
   CHECK_TRUE(Is_Point_Inside_Bounding_Box(corner_min_min, max_point, min_point));
}

/** \purpose
 * Test Get_Object_Bounding_Box with all reference points at same angle.
 * Validates that different reference points produce correctly offset bounding boxes.
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Get_Object_Bounding_Box_All_Reference_Points_Same_Angle)
{
   /** \precond
    * Test all reference points at 0 degrees to see offset patterns clearly
    */
   test_object.vcs_pointing = 0.0f;

   /** \action & \result
    * For each reference point, verify bounding box is correctly positioned
    */
   for (uint8_t ref_pt = ROT_OBJECT_REF_POINT_CENTER; ref_pt <= ROT_OBJECT_REF_POINT_LEFT_MID; ++ref_pt)
   {
      test_object.reference_point = ref_pt;

      SH_BoundingBox_T output_box{};
      Get_Object_Bounding_Box(test_object, output_box);

      SH_BoundingBox_T expected_output_box = Calculate_Expected_BoundingBox(test_object);
      Validate_BoundingBox(output_box, expected_output_box);
   }
}

/** \purpose
 * Test Get_Axis_Aligned_Rect_MinMax_Pt with box at origin.
 * Edge case for bounding box centered at (0,0).
 * \req CPR-7532_Derived
 */
TEST(test_Boundingbox_Helper_Functions, Boundingbox_Helper_Functions_TC_Get_Axis_Aligned_Rect_MinMax_Pt_Box_At_Origin)
{
   /** \precond
    * Object at origin creates bounding box around (0,0)
    */
   test_object.vcs_x_posn = 0.0f;
   test_object.vcs_y_posn = 0.0f;
   test_object.vcs_pointing = 0.0f;

   SH_BoundingBox_T test_bbox{};
   Get_Object_Bounding_Box(test_object, test_bbox);

   /** \action
    * Get min/max points with standard offsets
    */
   const float32_t offset_x = 3.0f;
   const float32_t offset_y = 2.0f;

   SH_Point_T max_point_out{};
   SH_Point_T min_point_out{};
   Get_Axis_Aligned_Rect_MinMax_Pt(test_bbox, offset_x, offset_y, max_point_out, min_point_out);

   SH_Point_T expected_max_point{};
   SH_Point_T expected_min_point{};
   Calculate_Expected_MinMax_Points(test_bbox, offset_x, offset_y, expected_max_point, expected_min_point);

   /** \result
    * Should correctly calculate extended rectangle around origin
    */
   DOUBLES_EQUAL(expected_max_point.x, max_point_out.x, test_pass_thres);
   DOUBLES_EQUAL(expected_max_point.y, max_point_out.y, test_pass_thres);
   DOUBLES_EQUAL(expected_min_point.x, min_point_out.x, test_pass_thres);
   DOUBLES_EQUAL(expected_min_point.y, min_point_out.y, test_pass_thres);
}

/** @}*/
