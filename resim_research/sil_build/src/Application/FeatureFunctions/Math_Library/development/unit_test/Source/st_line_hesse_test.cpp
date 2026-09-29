/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <gtest/gtest.h>

#include <math.h>
#include "ml_bool.h"
#include "ml_line_parameter.h"
#include "ml_line_segment.h"
#include "ml_trigonometry.h"
#include "ml_vector_2d.h"
#include "ml_line_hesse_t.h"
#include "ml_line_parameter_t.h"
#include "ml_line_segment_t.h"
#include "ml_vector_2d_t.h"
#include "ml_line_hesse.h"


class LineHesseTestFixture : public ::testing::TestWithParam<std::tuple < float, float, float>>
{
public:
   LineHesseTestFixture():
      m_sign { std::get<0>(GetParam()) },
      m_distance_to_origin { std::get<1>(GetParam()) * m_sign },
      m_norm_orientation { std::get<2>(GetParam()) },
      m_norm {
         /* .x = */ m_sign * cos(m_norm_orientation),
         /* .y= */  m_sign * sin(m_norm_orientation)
      },
      m_unit {
         /* .x = */ m_norm.y,
         /* .y= */ -m_norm.x
      },
      m_point0{
         /* .x = */ m_norm.x * m_distance_to_origin,
         /* .y= */  m_norm.y * m_distance_to_origin
      },
      m_point1{
         /* .x = */ m_point0.x + m_unit.x,
         /* .y= */  m_point0.y + m_unit.y
      },
      m_line_hesse_expected{
         /* .norm_vector = */ m_norm,
         /* .distance_to_origin = */ m_distance_to_origin
      }
   {};
protected:
   float m_sign;
   float m_distance_to_origin;
   float m_norm_orientation;
   Vector_2d_T m_norm; /* Vector with an orientation perpendicular to the line */
   Vector_2d_T m_unit; /* Vector with the same orientation as the line */
   Vector_2d_T m_point0; /* A point on the line */
   Vector_2d_T m_point1; /* Another point on the line */
   Line_Hesse_Tag m_line_hesse_expected; /* The Hesse line we expect to get using the parameters */
};

/* The Fast_Cos and Fast_Sin do cause some error. This margin allows the results to vary a bit from the expected values.*/
#define UT_HESSE_LINE_MARGIN (0.005f)

INSTANTIATE_TEST_SUITE_P(lines,
   LineHesseTestFixture,
   ::testing::Combine
   (
   testing::Values(-1.0f, 1.0f), /* Invert the Hesse line into its alternate form: Invert m_norm vector orientation and switch sign of distance to origin */
   testing::Range(0.0f, 10.0f, 5.0f), /* Distance to origin */
   testing::Range(0.0f, 2.0f * 3.14f, 0.8f) /* Orientation of m_norm vector */
   )
   );

/**
 * \sdd{WI-13951}
 */
TEST_P(LineHesseTestFixture, WI_14948_Line_Hesse_Create)
{
   Line_Hesse_Tag line_hesse_result = Line_Hesse_Create(m_distance_to_origin, &m_norm);
   EXPECT_FLOAT_EQ(line_hesse_result.distance_to_origin, m_line_hesse_expected.distance_to_origin);
   EXPECT_FLOAT_EQ(line_hesse_result.norm_vector.x, m_line_hesse_expected.norm_vector.x);
   EXPECT_FLOAT_EQ(line_hesse_result.norm_vector.y, m_line_hesse_expected.norm_vector.y);
}

/**
* \sdd{WI-13976}
*/
TEST_P(LineHesseTestFixture, WI_14949_Line_Hesse_Create_Using_Point_And_Unit_Vector)
{
   Line_Hesse_Tag line_hesse_result = Line_Hesse_Create_Using_Point_And_Unit_Vector(&m_point0, &m_unit);
   EXPECT_FLOAT_EQ(line_hesse_result.distance_to_origin, m_line_hesse_expected.distance_to_origin);
   EXPECT_FLOAT_EQ(line_hesse_result.norm_vector.x, m_line_hesse_expected.norm_vector.x);
   EXPECT_FLOAT_EQ(line_hesse_result.norm_vector.y, m_line_hesse_expected.norm_vector.y);
}

/**
* \sdd{WI-13974}
*/
TEST_P(LineHesseTestFixture, WI_14950_Line_Hesse_Create_Using_Point_And_Normal_Vector)
{
   Line_Hesse_Tag line_hesse_result = Line_Hesse_Create_Using_Point_And_Normal_Vector(&m_point0, &m_norm);
   EXPECT_FLOAT_EQ(line_hesse_result.distance_to_origin, m_line_hesse_expected.distance_to_origin);
   EXPECT_FLOAT_EQ(line_hesse_result.norm_vector.x, m_line_hesse_expected.norm_vector.x);
   EXPECT_FLOAT_EQ(line_hesse_result.norm_vector.y, m_line_hesse_expected.norm_vector.y);
}

/**
* \sdd{WI-13952}
*/
TEST_P(LineHesseTestFixture, WI_14951_Line_Hesse_Create_Using_Two_Points)
{
   Line_Hesse_Tag line_hesse_result = Line_Hesse_Create_Using_Two_Points(&m_point0, &m_point1);
   EXPECT_NEAR(line_hesse_result.distance_to_origin, m_line_hesse_expected.distance_to_origin, UT_HESSE_LINE_MARGIN);
   EXPECT_NEAR(line_hesse_result.norm_vector.x, m_line_hesse_expected.norm_vector.x, UT_HESSE_LINE_MARGIN);
   EXPECT_NEAR(line_hesse_result.norm_vector.y, m_line_hesse_expected.norm_vector.y, UT_HESSE_LINE_MARGIN);
}

/**
* \sdd{WI-13952}
*/
TEST(LineHesseTestFixtures, WI_14952_Line_Hesse_Create_Using_Two_Points__points_too_close)
{
   Vector_2d_T point;
   point.x = 10;
   point.y = 10;
   Line_Hesse_T line_hesse_result;
   EXPECT_DEBUG_DEATH({
      line_hesse_result = Line_Hesse_Create_Using_Two_Points(&point, &point);
   }, "FALSE");
#ifdef NDEBUG
   EXPECT_EQ(point.x, 10.0f);
   EXPECT_EQ(point.y, 10.0f);
#endif
}

/**
* \sdd{WI-13978}
*/
TEST_P(LineHesseTestFixture, WI_14953_Line_Hesse_Create_Using_Line_Segment)
{
   Line_Segment_T line_segment = Create_Line_Segment_Fom_Points(&m_point0, &m_point1);
   Line_Hesse_Tag line_hesse_result = Line_Hesse_Create_Using_Line_Segment(&line_segment);
   EXPECT_NEAR(line_hesse_result.distance_to_origin, m_line_hesse_expected.distance_to_origin, UT_HESSE_LINE_MARGIN);
   EXPECT_NEAR(line_hesse_result.norm_vector.x, m_line_hesse_expected.norm_vector.x, UT_HESSE_LINE_MARGIN);
   EXPECT_NEAR(line_hesse_result.norm_vector.y, m_line_hesse_expected.norm_vector.y, UT_HESSE_LINE_MARGIN);
}

/**
* \sdd{WI-13970}
*/
TEST_P(LineHesseTestFixture, WI_14954_Line_Hesse_Create_Using_Line_Parameter)
{
   Line_Parameter_T line_param = Create_Line_Parameter_Form(&m_point1, &m_point0);
   Line_Hesse_Tag line_hesse_result = Line_Hesse_Create_Using_Line_Parameter(&line_param);
   EXPECT_NEAR(line_hesse_result.distance_to_origin, m_line_hesse_expected.distance_to_origin, UT_HESSE_LINE_MARGIN);
   EXPECT_NEAR(line_hesse_result.norm_vector.x, m_line_hesse_expected.norm_vector.x, UT_HESSE_LINE_MARGIN);
   EXPECT_NEAR(line_hesse_result.norm_vector.y, m_line_hesse_expected.norm_vector.y, UT_HESSE_LINE_MARGIN);

}


/**
* \sdd{WI-13965}
*/
TEST_P(LineHesseTestFixture, WI_14955_Line_Hesse_Get_Distance_Of_Point__on_line)
{
   Vector_2d_T my_point;
   my_point = m_point0;
   float dist = Line_Hesse_Get_Distance_Of_Point(&my_point, &m_line_hesse_expected);
   EXPECT_NEAR(dist, 0.0f, UT_HESSE_LINE_MARGIN);
}

/**
* \sdd{WI-13965}
*/
TEST_P(LineHesseTestFixture, WI_14956_Line_Hesse_Get_Distance_Of_Point__in_m_normal_direction)
{
   Vector_2d_T my_point;
   my_point = Vector_2d_Alg_Add(&m_point0, &m_norm);
   float dist = Line_Hesse_Get_Distance_Of_Point(&my_point, &m_line_hesse_expected);
   EXPECT_NEAR(dist, 1.0, UT_HESSE_LINE_MARGIN);
}

/**
* \sdd{WI-13965}
*/
TEST_P(LineHesseTestFixture, WI_14957_Line_Hesse_Get_Distance_Of_Point__opposite_m_normal_direction)
{
   Vector_2d_T my_point;
   my_point = Vector_2d_Alg_Diff(&m_point0, &m_norm);
   float dist = Line_Hesse_Get_Distance_Of_Point(&my_point, &m_line_hesse_expected);
   EXPECT_NEAR(dist, -1.0f, UT_HESSE_LINE_MARGIN);
}

/**
* \sdd{WI-13975}
*/
TEST_P(LineHesseTestFixture, WI_14958_Line_Hesse_Update_Line_Normal_Direction__in_m_normal_direction_no_switch)
{
   Vector_2d_T my_point;
   my_point = Vector_2d_Alg_Add(&m_point0, &m_norm);
   Line_Hesse_Tag line_hesse_result = Line_Hesse_Update_Line_Normal_Direction(&my_point, &m_line_hesse_expected, LINE_HESSE_SIDE_POSITIVE);
   EXPECT_FLOAT_EQ(line_hesse_result.distance_to_origin, m_line_hesse_expected.distance_to_origin);
   EXPECT_FLOAT_EQ(line_hesse_result.norm_vector.x, m_line_hesse_expected.norm_vector.x);
   EXPECT_FLOAT_EQ(line_hesse_result.norm_vector.y, m_line_hesse_expected.norm_vector.y);
}

/**
* \sdd{WI-13975}
*/
TEST_P(LineHesseTestFixture, WI_14959_Line_Hesse_Update_Line_Normal_Direction__in_m_normal_direction_switch)
{
   Vector_2d_T my_point;
   my_point = Vector_2d_Alg_Add(&m_point0, &m_norm);
   Line_Hesse_Tag line_hesse_result = Line_Hesse_Update_Line_Normal_Direction(&my_point, &m_line_hesse_expected, LINE_HESSE_SIDE_NEGATIVE);
   EXPECT_FLOAT_EQ(line_hesse_result.distance_to_origin, -m_line_hesse_expected.distance_to_origin);
   EXPECT_FLOAT_EQ(line_hesse_result.norm_vector.x, -m_line_hesse_expected.norm_vector.x);
   EXPECT_FLOAT_EQ(line_hesse_result.norm_vector.y, -m_line_hesse_expected.norm_vector.y);
}

/**
* \sdd{WI-13975}
*/
TEST_P(LineHesseTestFixture, WI_14960_Line_Hesse_Update_Line_Normal_Direction__opposite_m_normal_direction_no_switch)
{
   Vector_2d_T my_point;
   my_point = Vector_2d_Alg_Diff(&m_point0, &m_norm);
   Line_Hesse_Tag line_hesse_result = Line_Hesse_Update_Line_Normal_Direction(&my_point, &m_line_hesse_expected, LINE_HESSE_SIDE_NEGATIVE);
   EXPECT_FLOAT_EQ(line_hesse_result.distance_to_origin, m_line_hesse_expected.distance_to_origin);
   EXPECT_FLOAT_EQ(line_hesse_result.norm_vector.x, m_line_hesse_expected.norm_vector.x);
   EXPECT_FLOAT_EQ(line_hesse_result.norm_vector.y, m_line_hesse_expected.norm_vector.y);
}

/**
* \sdd{WI-13975}
*/
TEST_P(LineHesseTestFixture, WI_14961_Line_Hesse_Update_Line_Normal_Direction__opposite_m_normal_direction_switch)
{
   Vector_2d_T my_point;
   my_point = Vector_2d_Alg_Diff(&m_point0, &m_norm);
   Line_Hesse_Tag line_hesse_result = Line_Hesse_Update_Line_Normal_Direction(&my_point, &m_line_hesse_expected, LINE_HESSE_SIDE_POSITIVE);
   EXPECT_FLOAT_EQ(line_hesse_result.distance_to_origin, -m_line_hesse_expected.distance_to_origin);
   EXPECT_FLOAT_EQ(line_hesse_result.norm_vector.x, -m_line_hesse_expected.norm_vector.x);
   EXPECT_FLOAT_EQ(line_hesse_result.norm_vector.y, -m_line_hesse_expected.norm_vector.y);
}

/**
* \sdd{WI-13967}
*/
TEST_P(LineHesseTestFixture, WI_14962_Line_Hesse_Get_Side_of_Point__on_line)
{
   Side_Of_Line_Hesse_T side = Line_Hesse_Get_Side_of_Point(&m_line_hesse_expected, &m_point0, UT_HESSE_LINE_MARGIN);
   EXPECT_EQ(side, LINE_HESSE_SIDE_ON_LINE);
}

/**
* \sdd{WI-13967}
*/
TEST_P(LineHesseTestFixture, WI_14963_Line_Hesse_Get_Side_of_Point__m_normal_direction)
{
   Vector_2d_T my_point;
   my_point = Vector_2d_Alg_Add(&m_point0, &m_norm);
   Side_Of_Line_Hesse_T side = Line_Hesse_Get_Side_of_Point(&m_line_hesse_expected, &my_point, UT_HESSE_LINE_MARGIN);
   EXPECT_EQ(side, LINE_HESSE_SIDE_POSITIVE);
}

/**
* \sdd{WI-13967}
*/
TEST_P(LineHesseTestFixture, WI_14964_Line_Hesse_Get_Side_of_Point__opposite_of_m_normal_direction)
{
   Vector_2d_T my_point;
   my_point = Vector_2d_Alg_Diff(&m_point0, &m_norm);
   Side_Of_Line_Hesse_T side = Line_Hesse_Get_Side_of_Point(&m_line_hesse_expected, &my_point, UT_HESSE_LINE_MARGIN);
   EXPECT_EQ(side, LINE_HESSE_SIDE_NEGATIVE);
}

/**
* \sdd{WI-13959}
*/
TEST_P(LineHesseTestFixture, WI_14965_Line_Hesse_Create_Using_Azimuth_And_Point)
{
   float direction;
   direction = Fast_Atan2(m_unit.y, m_unit.x);
   Line_Hesse_Tag line_hesse_result = Line_Hesse_Create_Using_Azimuth_And_Point(&m_point0, direction);
   EXPECT_NEAR(line_hesse_result.distance_to_origin, m_line_hesse_expected.distance_to_origin, UT_HESSE_LINE_MARGIN);
   EXPECT_NEAR(line_hesse_result.norm_vector.x, m_line_hesse_expected.norm_vector.x, UT_HESSE_LINE_MARGIN);
   EXPECT_NEAR(line_hesse_result.norm_vector.y, m_line_hesse_expected.norm_vector.y, UT_HESSE_LINE_MARGIN);
}

/**
* \sdd{WI-13969}
*/
TEST_P(LineHesseTestFixture, WI_14966_Line_Hesse_Get_Norm_Vector)
{
   Vector_2d_T ret = Line_Hesse_Get_Norm_Vector(&m_line_hesse_expected);
   EXPECT_EQ(ret.x, m_line_hesse_expected.norm_vector.x);
   EXPECT_EQ(ret.y, m_line_hesse_expected.norm_vector.y);
}

/**
* \sdd{WI-13956}
*/
TEST_P(LineHesseTestFixture, WI_14967_Line_Hesse_Get_Unit_Vector)
{
   Vector_2d_T ret = Line_Hesse_Get_Unit_Vector(&m_line_hesse_expected);
   EXPECT_EQ(ret.x, m_unit.x);
   EXPECT_EQ(ret.y, m_unit.y);
}

/**
* \sdd{WI-13958}
*/
TEST_P(LineHesseTestFixture, WI_14968_Line_Hesse_Get_Distance_To_Origin)
{
   float ret = Line_Hesse_Get_Distance_To_Origin(&m_line_hesse_expected);
   EXPECT_EQ(ret, m_line_hesse_expected.distance_to_origin);
}

