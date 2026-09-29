/** \file
 * This file contains unit tests for content of f360_line.cpp file
 */

#include "f360_line.h"
#include <CppUTest/TestHarness.h>

// Unit testing guidelines: https://confluence.asux.aptiv.com/display/F360Core/Unit+testing+guidelines

using namespace f360_variant_A;

/** \defgroup  f360_line
 *  @{
 */

/** \brief
 * This test group checks function associated to the Line class are working as intended
 */
TEST_GROUP(f360_line)
{	
   // Declare Line object.
   Line default_line;
   
   /** \setup
    * Set up a simple line y = 2x
    */
   TEST_SETUP()
   {
      default_line = Line {2.0F, -1.0F, 0.0F};
   }

};

/** \purpose  
 * The purpose of this test is to check whether Get_Line_Perpendicular_At_Point() works as intended, i.e returns correct line at a given point.
 * \req
 * NA.
 */
TEST(f360_line, f360_line_get_perpendicular_line_at_point)
{
   /** \precond
    * Set up point (1,2)
    */
   Point default_point = Point{1.0F, 2.0F};
	
   /** \action
    * Call Get_Line_Perpendicular_At_Point(). Compute line equation at default_point and scalar product between lines.
    */
   Line perpendicular_line = default_line.Get_Line_Perpendicular_At_Point(default_point);
   float32_t line_eq_at_point = perpendicular_line.Get_a() * default_point.x + perpendicular_line.Get_b() * default_point.y + perpendicular_line.Get_c();
   float32_t scalar_product = perpendicular_line.Get_a() * default_line.Get_a() + perpendicular_line.Get_b() * default_line.Get_b();

   /** \result
    * Check if the returned line is correct. Line must be passing a default_point (line_eq = 0) and must be perpendicular to default_line (scalar product = 0)
    */
   DOUBLES_EQUAL(0.0F, line_eq_at_point, 1e-4);
   DOUBLES_EQUAL(0.0F, scalar_product, 1e-4);
}
/** \purpose  
 * The purpose of this test is to check whether Get_Lines_Equally_Distant_From_Original() works as intended, i.e returns lines that are equally distant from original.
 * \req
 * NA.
 */
TEST(f360_line, f360_get_lines_equally_distant)
{
   /** \precond
    * Set up two lines. Define distance to be 5.0
    */
   Line first_line;
   Line second_line;
   float32_t distance = 5.0F;
	
   /** \action
    * Call Get_Lines_Equally_Distant_From_Original(). Compute the difference in slope for each from default_line and distance between each to default_line
    */
   default_line.Get_Lines_Equally_Distant_From_Original(distance, first_line, second_line);
   float32_t a_diff_first = first_line.Get_a() - default_line.Get_a();
   float32_t b_diff_first = first_line.Get_b() - default_line.Get_b();
   float32_t c_diff_first = first_line.Get_c() - default_line.Get_c();
   float32_t a_diff_second = second_line.Get_a() - default_line.Get_a();
   float32_t b_diff_second = second_line.Get_b() - default_line.Get_b();
   float32_t c_diff_second = first_line.Get_c() - default_line.Get_c();
   

 
   /** \result
    * Check if the returned line is correct. Line must be passing a default_point (line_eq = 0) and must be perpendicular to default_line (scalar product = 0)
    */
   DOUBLES_EQUAL(0.0F, a_diff_first, 1e-4);
   DOUBLES_EQUAL(0.0F, b_diff_first, 1e-4);
   DOUBLES_EQUAL(distance, c_diff_first, 1e-4);
   DOUBLES_EQUAL(0.0F, a_diff_second, 1e-4);
   DOUBLES_EQUAL(0.0F, b_diff_second, 1e-4);
   DOUBLES_EQUAL(distance, c_diff_second, 1e-4);
}

/** \purpose  
 * The purpose of this test is to check whether Is_Point_Between_Parallel_Lines() returns true if point is between two lines.
 * \req
 * NA.
 */
TEST(f360_line, f360_is_point_between_parallel_lines)
{
   /** \precond
    * Set up two parallel lines and point between them
    */

   Point default_point = Point{-1.0F, 1.0F};
   Line first_line = Line {1.0F, 1.0F, 1.0F};
   Line second_line = Line {1.0F, 1.0F, -1.0F};

   /** \action
    * Call Is_Point_Between_Parallel_Lines().
    */
   float32_t f_between_lines = default_line.Is_Point_Between_Parallel_Lines(default_point, first_line, second_line);
 
   /** \result
    * Check if the f_beteen_lines is true
    */
   CHECK_TRUE(f_between_lines);
}
/** \purpose  
 * The purpose of this test is to check whether Is_Point_Between_Parallel_Lines() returns false if point is not between two lines.
 * \req
 * NA.
 */
TEST(f360_line, f360_is_point_not_between_parallel_lines)
{
   /** \precond
    * Set up two parallel lines and point that is not between them
    */

   Point default_point = Point{-1.0F, 1.0F};
   Line first_line = Line {1.0F, -1.0F, 1.0F};
   Line second_line = Line {-1.0F, 1.0F, -1.0F};

   /** \action
    * Call Is_Point_Between_Parallel_Lines().
    */
   float32_t f_between_lines = default_line.Is_Point_Between_Parallel_Lines(default_point, first_line, second_line);
 
   /** \result
    * Check if the f_between_lines is true
    */
   CHECK_FALSE(f_between_lines);
}

/** @}*/
