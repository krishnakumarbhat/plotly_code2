/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/
#include <gtest/gtest.h>
#include "Basic_Vectors.hpp"

#include "Geometric_2d_Structs.h"
#include "Geometric_2d_Factory.h"
#include "Geometric_2d_Functions.h"

#include "line_hesse.h"
#include "st_vector_2d.h"
#include <math.h>
#include <array>

#define UT_HESSE_LINE_MARGIN (0.001f)

/** Create a line hesse from a line normal */
TEST(Line_Hesse_Test, Line_Hesse_Create_Using_Line_Normal)
{
   /** \arrange pick any distance to origin for the line */
   float distance_to_origin = 100.f;

   /** \arrange pick any orientation for the norm vector of the line */
   float norm_orientation = 0.8f;

   /**\arange Vector with an orientation perpendicular to the line */
   Vector_2d_T norm;
   norm.x = cos(norm_orientation);
   norm.y = sin(norm_orientation);

   /**\arrange Vector with the same orientation as the line */
   Vector_2d_T unit;
   unit.x = norm.y;
   unit.y = -norm.x;

   /**\arrange A point on the line */
   Vector_2d_T point0;
   point0.x = norm.x * distance_to_origin;
   point0.y = norm.y * distance_to_origin;

   /** \arrange Another point on the line */
   Vector_2d_T point1;
   point1.x = point0.x + unit.x;
   point1.y = point0.y + unit.y;

   /** \arrange create a norm line */
   Line_Normal_T line_norm = Create_Line_Normal(&point1, &point0);

   /** \action call function under test */
   Line_Hesse_Tag line_hesse_result = Line_Hesse_Create_Using_Line_Normal(&line_norm);

   /** \arrange A Hesse line we expect to get using the parameters */
   Line_Hesse_Tag line_hesse_expected{
      /* .norm_vector = */norm,
      /* .distance_to_origin = */distance_to_origin
   };

   /** \assert Resulting line hesse matches expectations */
   EXPECT_NEAR(line_hesse_result.distance_to_origin, line_hesse_expected.distance_to_origin, UT_HESSE_LINE_MARGIN);
   EXPECT_NEAR(line_hesse_result.norm_vector.x, line_hesse_expected.norm_vector.x, UT_HESSE_LINE_MARGIN);
   EXPECT_NEAR(line_hesse_result.norm_vector.y, line_hesse_expected.norm_vector.y, UT_HESSE_LINE_MARGIN);
}

class StLineNormalTest : public ::testing::Test
{
public:
   enum TetragonCornersT
   {
      TETRAGON_CORNER_FR,
      TETRAGON_CORNER_FL,
      TETRAGON_CORNER_RL,
      TETRAGON_CORNER_RR,
      TETRAGON_CORNER_NUM,
   };
   StLineNormalTest():
   m_tetragon_corners{
      Create_2d_Vector_Coordinates(1., 1.),
      Create_2d_Vector_Coordinates(-1., 1.),
      Create_2d_Vector_Coordinates(-1., -1.),
      Create_2d_Vector_Coordinates(1., -1.)
   }{ };

   std::array<Vector_2d_T, TETRAGON_CORNER_NUM> m_tetragon_corners;           // tetragon for testing
};

/** Test that a point on the line is considered as such */
TEST_F(StLineNormalTest, Get_Side_of_Point__Point_should_be_on_line)
{
   /** \action call function under test */
   Line_Normal_T normal = Create_Line_Normal(&m_tetragon_corners[TETRAGON_CORNER_FR], &m_tetragon_corners[TETRAGON_CORNER_FL]);
   Vector_2d_T   point = Vector_2d_Alg_Middle(&m_tetragon_corners[TETRAGON_CORNER_FR], &m_tetragon_corners[TETRAGON_CORNER_FL]);

   /** \action call function under test */
   Line_Side_T result = Get_Side_of_Point(&normal, &point);

   /** \assert */
   EXPECT_EQ(LINE_SIDE_ONLINE, result);
}

/** Test that a point on the right side of the line is considered as such */
TEST_F(StLineNormalTest, Get_Side_of_Point__Point_should_be_on_the_right_side_from_line)
{
   /** \action call function under test */
   Line_Normal_T normal = Create_Line_Normal(&m_tetragon_corners[TETRAGON_CORNER_FR], &m_tetragon_corners[TETRAGON_CORNER_RL]);

   /** \action call function under test */
   Line_Side_T result = Get_Side_of_Point(&normal, &m_tetragon_corners[TETRAGON_CORNER_FL]);

   /** \assert */
   EXPECT_EQ(LINE_SIDE_LEFT, result);
}

/** Test that a point on the left side of the line is considered as such */
TEST_F(StLineNormalTest, Get_Side_of_Point__Point_should_be_on_the_left_side_from_line)
{
   /** \action call function under test */
   Line_Normal_T normal = Create_Line_Normal(&m_tetragon_corners[TETRAGON_CORNER_FR], &m_tetragon_corners[TETRAGON_CORNER_RL]);

   /** \action call function under test */
   Line_Side_T result = Get_Side_of_Point(&normal, &m_tetragon_corners[TETRAGON_CORNER_RR]);

   /** \assert */
   EXPECT_EQ(LINE_SIDE_RIGHT, result);
}

/** Test that a norm line and a line in parameter form are perpendicular to each other */
TEST(StLineParameterTest, Create_Line_Normal__scalar_product_of_direction_vectors_from_line_and_its_normal_should_be_zero)
{
   /** \action Create a line in parameter form through two given points */
   Line_Parameter_T line = Create_Line_Parameter_Form(&Basic_Vectors::vec_arbitrary1, &Basic_Vectors::vec_arbitrary2);

   /** \action Create a line in normal form through two given points */
   Line_Normal_T normal = Create_Line_Normal(&Basic_Vectors::vec_arbitrary1, &Basic_Vectors::vec_arbitrary2);

   /** \assert the direction and the norm vector are perpendicular */
   float32_T scalar_product_result = Vector_2d_Alg_Scalar_Product(&line.direction, &normal.n);
   EXPECT_FLOAT_EQ(scalar_product_result, 0.0f);
}
