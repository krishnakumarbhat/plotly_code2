/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/


#include <gtest/gtest.h>
#include "Basic_Vectors.hpp"
#include "st_vector_2d_helper.hpp"
#include "ml_line_parameter.h"
#include "ml_vector_2d.h"
#include "ml_line_parameter_t.h"
#include "ml_vector_2d_t.h"


/**
* This function will try to generate a parameter form by using x and y normal. The result should be a negative bisectrix
* \sdd{WI-13968}
*/
TEST(StLineParameterTest, WI_14969_Create_Line_Parameter_Form__Parameter_Form_of_x_and_y_normal_should_be_a_bisectrix)
{
   /** \arrange */

   /** \action call function under test */
   Line_Parameter_T line = Create_Line_Parameter_Form(&Basic_Vectors::vec_x_normal, &Basic_Vectors::vec_y_normal);

   /** \assert */
   EXPECT_VECTOR_2D_EQ(line.p0, Basic_Vectors::vec_x_normal);
   EXPECT_VECTOR_2D_EQ(line.direction, Basic_Vectors::vec_fourth_bisectrix);
}

/**
* \sdd{WI-13971}
*/
TEST(StLineParameterTest, WI_14970_Get_Y_Value_From_Line__Y_Values_Of_pos_Bisection_Shall_Equal_x_value)
{
   /** \arrange */
   float x_value = 10.;
   Vector_2d_T vec_first_bisectrix = Create_2d_Vector_Coordinates(1., 1.);
   Vector_2d_T vec_third_bisectrix = Create_2d_Vector_Coordinates(-1., -1.);

   Line_Parameter_T line_p_bisection_pos = Create_Line_Parameter_Form(&vec_first_bisectrix, &vec_third_bisectrix);

   /** \action call function under test */
   float result = Get_Y_Value_From_Line(&line_p_bisection_pos, x_value);

   /** \assert */
   EXPECT_EQ(result, x_value);
}

/**
* \sdd{WI-13971}
*/
TEST(StLineParameterTest, WI_14971_Get_Y_Value_From_Line__Y_Values_Of_X_Axis_Shall_Be_Zero)
{
   /** \arrange */
   float x_value = 10.;
   Vector_2d_T vec_origin = Create_2d_Vector_Origin();
   Vector_2d_T vec_x_normal = Create_2d_Vector_X_Normal();

   Line_Parameter_T line_p_x_axis = Create_Line_Parameter_Form(&vec_origin, &vec_x_normal);

   /** \action call function under test */
   float result = Get_Y_Value_From_Line(&line_p_x_axis, x_value);

   /** \assert */
   EXPECT_EQ(0, result);
}