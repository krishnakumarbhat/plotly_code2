/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/


#include <gtest/gtest.h>
#include "ml_line.h"
#include "ml_line_parameter.h"
#include "ml_math.h"
#include "ml_vector_2d_t.h"



/**
* \sdd{WI-13983}
*/
TEST(StLine, WI_14973_Get_Slope_Of_Linear_Function_test__increasing_function)
{
   /** \arrange */
   const Vector_2d_T point1 = { -1.0, -1.0 };
   const Vector_2d_T point2 = { 1.0, 1.0 };
   float32_T slope_result;

   /** \action call function under test */
   slope_result = Get_Slope_Of_Line(&point1, &point2);

   /** \assert */
   EXPECT_FLOAT_EQ(slope_result, 1.0f);

}

/**
* \sdd{WI-13983}
*/
TEST(StLine, WI_14974_Get_Slope_Of_Linear_Function_test__decreasing_function)
{
   /** \arrange */
   const Vector_2d_T point1 = { -1.0, 1.0 };
   const Vector_2d_T point2 = { 1.0, -1.0 };
   float32_T slope_result;

   /** \action call function under test */
   slope_result = Get_Slope_Of_Line(&point1, &point2);

   /** \assert */
   EXPECT_FLOAT_EQ(slope_result, -1.0f);

}

/**
* \sdd{WI-13983}
*/
TEST(StLine, WI_14975_Get_Slope_Of_Linear_Function_test__almost_parallel_to_y_axis)
{

   /** \arrange */
   const Vector_2d_T point1 = { 1.0f          , 1.0f };
   const Vector_2d_T point2 = { 1.0f + EPSILON, 2.0f };
   float32_T slope_result;

   /** \action call function under test */
   slope_result = Get_Slope_Of_Line(&point1, &point2);

   /** \assert */
   EXPECT_NEAR(slope_result, (1.0f / EPSILON), 0.05f);

}


/**
* \sdd{WI-13983}
*/
TEST(StLine, WI_14976_Get_Slope_Of_Linear_Function_test__parallel_to_x_axis)
{

   /** \arrange */
   const Vector_2d_T point1 = { 1.0 , 1.0 };
   const Vector_2d_T point2 = { 2.0 , 1.0 };
   float32_T slope_result;

   /** \action call function under test */
   slope_result = Get_Slope_Of_Line(&point1, &point2);

   /** \assert */
   EXPECT_FLOAT_EQ(slope_result, 0.0f);

}

/**
* \sdd{WI-13982}
*/
TEST(StLine, WI_14977_Get_Y_Value_From_Line_Defined_By_2_Points_test__increasing_function)
{
   /** \arrange */
   const Vector_2d_T point1 = { -1.0, -1.0 };
   const Vector_2d_T point2 = { 1.0, 1.0 };
   const float32_T x_value = 0.5f;
   float32_T y_value;

   /** \action call function under test */
   y_value = Get_Y_Value_From_Line_Defined_By_2_Points(&point1, &point2, x_value);

   /** \assert */
   EXPECT_FLOAT_EQ(y_value, 0.5f);

}

/**
* \sdd{WI-13982}
*/
TEST(StLine, WI_14978_Get_Y_Value_From_Line_Defined_By_2_Points_test__decreasing_function)
{
   /** \arrange */
   const Vector_2d_T point1 = { -1.0, 1.0 };
   const Vector_2d_T point2 = { 1.0, -1.0 };
   const float32_T x_value = -0.5f;
   float32_T y_value;

   /** \action call function under test */
   y_value = Get_Y_Value_From_Line_Defined_By_2_Points(&point1, &point2, x_value);

   /** \assert */
   EXPECT_FLOAT_EQ(y_value, 0.5f);

}


/**
* \sdd{WI-13982}
*/
TEST(StLine, WI_14979_Get_Y_Value_From_Line_Defined_By_2_Points_test__function_parallel_to_x_axis)
{
   /** \arrange */
   const Vector_2d_T point1 = { 1.0, 1.0 };
   const Vector_2d_T point2 = { 2.0, 1.0 };
   const float32_T x_value = -0.5f;
   float32_T y_value;

   /** \action call function under test */
   y_value = Get_Y_Value_From_Line_Defined_By_2_Points(&point1, &point2, x_value);

   /** \assert */
   EXPECT_FLOAT_EQ(y_value, 1.0f);

}

/**
* \sdd{WI-13981}
*/
TEST(StLine, WI_14980_Get_Y_Value_From_Line_By_Coordinates__Y_Values_Of_pos_Bisection_Shall_Equal_x_value)
{
   /** \arrange */
   float32_T x_value = 10.f;
   float32_T x_1 = 0.f;
   float32_T x_2 = 1.f;
   float32_T y_1 = 0.f;
   float32_T y_2 = 0.f;

   /** \action call function under test */
   float32_T result = Get_Y_Value_From_Line_By_Coordinates(x_1, y_1, x_2, y_2, x_value);

   /** \assert */
   EXPECT_EQ(0, result);
}

/**
* \sdd{WI-13981}
*/
TEST(StLine, WI_14981_Get_Y_Value_From_Line_By_Coordinates__Y_Values_Of_Y_Axis_Shall_Cause_Assert)
{
   /** \arrange */
   float x_value = 10.;

   /** \action call function under test */
   float result = Get_Y_Value_From_Line_By_Coordinates(0, 0, 1, 1, x_value);

   /** \assert */
   EXPECT_EQ(result, x_value);
}

/**
* \sdd{WI-13981}
*/
TEST(StLine, WI_14982_Get_Y_Value_From_Line_By_Coordinates__x1_too_close_to_x2_returns_y2)
{
   /** \arrange */
   float x_value = 10.;

   /** \action call function under test */
   float result = Get_Y_Value_From_Line_By_Coordinates(0, 0, EPSILON, 1, x_value);

   /** \assert */
   EXPECT_EQ(result, 0);
}