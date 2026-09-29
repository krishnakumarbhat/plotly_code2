/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/


#include <gtest/gtest.h>


#include <array>
#include "ml_bool.h"
#include "ml_polygon.h"
#include "ml_vector_2d.h"
#include "ml_vector_2d_t.h"

class StPolygonTestFixture : public ::testing::Test
{
public:
   StPolygonTestFixture():
      m_tetragon {
         Create_2d_Vector_Coordinates( 1.,  1.),
         Create_2d_Vector_Coordinates(-1.,  1.),
         Create_2d_Vector_Coordinates(-1., -1.),
         Create_2d_Vector_Coordinates( 1., -1.)
      },
      m_concave_tetragon {
         Create_2d_Vector_Coordinates(6.0f, 4.0f),
         Create_2d_Vector_Coordinates(1.0f, 7.0f),
         Create_2d_Vector_Coordinates(3.0f, 4.0f),
         Create_2d_Vector_Coordinates(1.0f, 2.0f)
      },
      m_convex_tetragon {
         Create_2d_Vector_Coordinates(6.0f, 4.0f),
         Create_2d_Vector_Coordinates(1.0f, 7.0f),
         Create_2d_Vector_Coordinates(0.2f, 4.0f),
         Create_2d_Vector_Coordinates(1.0f, 2.0f)
      },
      m_tetragon_reversed {
         m_tetragon[3],
         m_tetragon[2],
         m_tetragon[1],
         m_tetragon[0]
      },
      m_concave_tetragon_reversed {
         m_concave_tetragon[3],
         m_concave_tetragon[2],
         m_concave_tetragon[1],
         m_concave_tetragon[0]
      },
      m_convex_tetragon_reversed
      {
         m_convex_tetragon[3],
         m_convex_tetragon[2],
         m_convex_tetragon[1],
         m_convex_tetragon[0]
      }{};

protected:
   static const uint8_t num_corners_m_tetragon = 4;
   std::array<Vector_2d_T,num_corners_m_tetragon> m_tetragon;           // m_tetragon for testing
   std::array<Vector_2d_T,num_corners_m_tetragon> m_concave_tetragon;
   std::array<Vector_2d_T,num_corners_m_tetragon> m_convex_tetragon;
   std::array<Vector_2d_T,num_corners_m_tetragon> m_tetragon_reversed;  // m_tetragon with reversed order of points for testing
   std::array<Vector_2d_T,num_corners_m_tetragon> m_concave_tetragon_reversed;
   std::array<Vector_2d_T,num_corners_m_tetragon> m_convex_tetragon_reversed;
};

/**
* \sdd{WI-13985}
*/
TEST_F(StPolygonTestFixture, WI_15099_Is_Point_In_Polygon__Point_should_be_outside_of_Polygon)
{
   /** \arrange */
   const uint8_t     num_polygon_corners = 4;
   const Vector_2d_T offset = { 1.0f, 1.0f };
   const Vector_2d_T point = Vector_2d_Alg_Add(m_tetragon.data(), &offset);

   /** \action call function under test */
   boolean_T result = Is_Point_In_Polygon(m_tetragon.data(), num_polygon_corners, &point);

   /** \assert */
   EXPECT_FALSE(result);
}

/**
* \sdd{WI-13985}
*/
TEST_F(StPolygonTestFixture, WI_15100_Is_Point_In_Polygon__Point_should_be_outside_of_inverse_Polygon)
{
   /** \arrange */
   const uint8_t     num_polygon_corners = 4;
   const Vector_2d_T offset = { 1.0f, 1.0f };
   const Vector_2d_T point = Vector_2d_Alg_Add(m_tetragon.data(), &offset);

   /** \action call function under test */
   boolean_T result = Is_Point_In_Polygon(m_tetragon_reversed.data(), num_polygon_corners, &point);

   /** \assert */
   EXPECT_FALSE(result);
}

/**
* \sdd{WI-13984}
*/
TEST_F(StPolygonTestFixture, WI_15101_Is_Point_In_Convex_Polygon_Ray_Casting_Method__returns_TRUE_when_point_is_inside_concave_polygon)
{
   /** \arrange */
   const Vector_2d_T offset = { 1.0f, 1.0f };
   const Vector_2d_T point = Vector_2d_Alg_Add(&m_concave_tetragon[2], &offset);

   /** \action call function under test */
   boolean_T result = Is_Point_In_Convex_Polygon_Ray_Casting_Method(m_concave_tetragon.data(), num_corners_m_tetragon, &point);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
* \sdd{WI-13984}
*/
TEST_F(StPolygonTestFixture, WI_15102_Is_Point_In_Convex_Polygon_Ray_Casting_Method__returns_FALSE_when_point_is_outside_concave_polygon)
{
   /** \arrange */
   const Vector_2d_T offset = { -1.0f, 1.0f };
   const Vector_2d_T point = Vector_2d_Alg_Add(&m_concave_tetragon[2], &offset);

   /** \action call function under test */
   boolean_T result = Is_Point_In_Convex_Polygon_Ray_Casting_Method(m_concave_tetragon.data(), num_corners_m_tetragon, &point);

   /** \assert */
   EXPECT_FALSE(result);
}

/**
* \sdd{WI-13984}
*/
TEST_F(StPolygonTestFixture, WI_15103_Is_Point_In_Convex_Polygon_Ray_Casting_Method__returns_FALSE_when_point_is_outside_concave_reversed_polygon)
{
   /** \arrange */
   const Vector_2d_T offset = { -1.0f, 1.0f };
   const Vector_2d_T point = Vector_2d_Alg_Add(&m_concave_tetragon[2], &offset);

   /** \action call function under test */
   boolean_T result = Is_Point_In_Convex_Polygon_Ray_Casting_Method(m_concave_tetragon_reversed.data(), num_corners_m_tetragon, &point);

   /** \assert */
   EXPECT_FALSE(result);
}

/**
* \sdd{WI-13984}
*/
TEST_F(StPolygonTestFixture, WI_15104_Is_Point_In_Convex_Polygon_Ray_Casting_Method__returns_TRUE_when_point_is_inside_convex_polygon)
{
   /** \arrange */
   const Vector_2d_T offset = { 1.0f, 1.0f };
   const Vector_2d_T point = Vector_2d_Alg_Add(&m_convex_tetragon[2], &offset);

   /** \action call function under test */
   boolean_T result = Is_Point_In_Convex_Polygon_Ray_Casting_Method(m_convex_tetragon.data(), num_corners_m_tetragon, &point);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
* \sdd{WI-13984}
*/
TEST_F(StPolygonTestFixture, WI_15105_Is_Point_In_Convex_Polygon_Ray_Casting_Method__returns_TRUE_when_point_is_inside_reversed_convex_polygon)
{
   /** \arrange */
   const Vector_2d_T offset = { 1.0f, 1.0f };
   const Vector_2d_T point = Vector_2d_Alg_Add(&m_convex_tetragon[2], &offset);

   /** \action call function under test */
   boolean_T result = Is_Point_In_Convex_Polygon_Ray_Casting_Method(m_convex_tetragon_reversed.data(), num_corners_m_tetragon, &point);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
* \sdd{WI-13984}
*/
TEST_F(StPolygonTestFixture, WI_15106_Is_Point_In_Convex_Polygon_Ray_Casting_Method__returns_FALSE_when_point_is_outside_convex_polygon)
{
   /** \arrange */
   const Vector_2d_T offset = { -1.0f, 1.0f };
   const Vector_2d_T point = Vector_2d_Alg_Add(&m_convex_tetragon[2], &offset);

   /** \action call function under test */
   boolean_T result = Is_Point_In_Convex_Polygon_Ray_Casting_Method(m_convex_tetragon.data(), num_corners_m_tetragon, &point);

   /** \assert */
   EXPECT_FALSE(result);
}

/**
* \sdd{WI-13984}
*/
TEST_F(StPolygonTestFixture, WI_15107_Is_Point_In_Convex_Polygon_Ray_Casting_Method__returns_FALSE_when_point_is_outside_reversed_convex_polygon)
{
   /** \arrange */
   const Vector_2d_T offset = { -1.0f, 1.0f };
   const Vector_2d_T point = Vector_2d_Alg_Add(&m_convex_tetragon[2], &offset);

   /** \action call function under test */
   boolean_T result = Is_Point_In_Convex_Polygon_Ray_Casting_Method(m_convex_tetragon_reversed.data(), num_corners_m_tetragon, &point);

   /** \assert */
   EXPECT_FALSE(result);
}

/**
* \sdd{WI-13984}
*/
TEST_F(StPolygonTestFixture, WI_15108_Is_Point_In_Convex_Polygon_Ray_Casting_Method__returns_FALSE_when_m_tetragon_is_of_zero_area)
{
   /** \arrange */
   const Vector_2d_T point = Create_2d_Vector_Coordinates(1.0f, 1.0f);
   std::array<Vector_2d_T, num_corners_m_tetragon> zero_area_m_tetragon;


   zero_area_m_tetragon[0] = Create_2d_Vector_Origin();
   zero_area_m_tetragon[1] = Create_2d_Vector_Origin();
   zero_area_m_tetragon[2] = Create_2d_Vector_Origin();
   zero_area_m_tetragon[3] = Create_2d_Vector_Origin();

   /** \action call function under test */
   boolean_T result = Is_Point_In_Convex_Polygon_Ray_Casting_Method(&zero_area_m_tetragon[0], num_corners_m_tetragon, &point);

   /** \assert */
   EXPECT_FALSE(result);
}

/**
* \sdd{WI-13985}
*/
TEST_F(StPolygonTestFixture, WI_15109_Is_Point_In_Polygon__Edge_Point_First_should_be_in_Polygon)
{
   /** \arrange */
   const uint8_t     num_polygon_corners = 4;
   const Vector_2d_T point = Vector_2d_Alg_Middle(m_tetragon.data(), &m_tetragon[1]);

   /** \action call function under test */
   boolean_T result = Is_Point_In_Polygon(m_tetragon.data(), num_polygon_corners, &point);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
* \sdd{WI-13985}
*/
TEST_F(StPolygonTestFixture, WI_15110_Is_Point_In_Polygon__Edge_Point_Second_should_be_in_Polygon)
{
   /** \arrange */
   const uint8_t     num_polygon_corners = 4;
   const Vector_2d_T point = Vector_2d_Alg_Middle(&m_tetragon[1], &m_tetragon[2]);

   /** \action call function under test */
   boolean_T result = Is_Point_In_Polygon(m_tetragon.data(), num_polygon_corners, &point);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
* \sdd{WI-13985}
*/
TEST_F(StPolygonTestFixture, WI_15111_Is_Point_In_Polygon__Edge_Point_Second_should_be_in_Reversed_Polygon)
{
   /** \arrange */
   const uint8_t     num_polygon_corners = 4;
   const Vector_2d_T point = Vector_2d_Alg_Middle(&m_tetragon[1], &m_tetragon[2]);

   /** \action call function under test */
   boolean_T result = Is_Point_In_Polygon(m_tetragon_reversed.data(), num_polygon_corners, &point);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
* \sdd{WI-13985}
*/
TEST_F(StPolygonTestFixture, WI_15112_Is_Point_In_Polygon__Edge_Point_Last_should_be_in_Polygon)
{
   /** \arrange */
   const uint8_t     num_polygon_corners = 4;
   const Vector_2d_T point = Vector_2d_Alg_Middle(m_tetragon.data(), &m_tetragon[3]);

   /** \action call function under test */
   boolean_T result = Is_Point_In_Polygon(m_tetragon.data(), num_polygon_corners, &point);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
* \sdd{WI-13985}
*/
TEST_F(StPolygonTestFixture, WI_15113_Is_Point_In_Polygon__Edge_Point_Last_should_be_in_Reversed_Polygon)
{
   /** \arrange */
   const uint8_t     num_polygon_corners = 4;
   const Vector_2d_T point = Vector_2d_Alg_Middle(m_tetragon.data(), &m_tetragon[3]);

   /** \action call function under test */
   boolean_T result = Is_Point_In_Polygon(m_tetragon_reversed.data(), num_polygon_corners, &point);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
* \sdd{WI-13985}
*/
TEST_F(StPolygonTestFixture, WI_15114_Is_Point_In_Polygon__Edge_Point_should_be_in_Inverse_Polygon)
{
   /** \arrange */
   const uint8_t     num_polygon_corners = 4;
   const Vector_2d_T point = Vector_2d_Alg_Middle(m_tetragon.data(), &m_tetragon[1]);

   /** \action call function under test */
   boolean_T result = Is_Point_In_Polygon(m_tetragon_reversed.data(), num_polygon_corners, &point);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
* \sdd{WI-13985}
*/
TEST_F(StPolygonTestFixture, WI_15115_Is_Point_In_Polygon__Point_should_be_inside_Polygon)
{
   /** \arrange */
   const uint8_t     num_polygon_corners = 4;
   const Vector_2d_T point = Vector_2d_Alg_Middle(m_tetragon.data(), &m_tetragon[2]);

   /** \action call function under test */
   boolean_T result = Is_Point_In_Polygon(m_tetragon.data(), num_polygon_corners, &point);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
* \sdd{WI-13985}
*/
TEST_F(StPolygonTestFixture, WI_15116_Is_Point_In_Polygon__Point_should_be_inside__inverse_Polygon)
{
   /** \arrange */
   const uint8_t     num_polygon_corners = 4;
   const Vector_2d_T point = Vector_2d_Alg_Middle(m_tetragon.data(), &m_tetragon[2]);

   /** \action call function under test */
   boolean_T result = Is_Point_In_Polygon(m_tetragon_reversed.data(), num_polygon_corners, &point);

   /** \assert */
   EXPECT_TRUE(result);
}