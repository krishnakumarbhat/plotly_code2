/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <gtest/gtest.h>


#include <array>
#include "ml_lookup_table_2d.h"
#include "ml_math.h"

#include <array>

class LookupTableTestFixture : public testing::Test
{
public:
   static const std::size_t m_lookup_table_size = 5;

   LookupTableTestFixture() : m_x_table{
                           0.f,
                           3.f,
                           4.f,
                           8.3f,
                           10.f},
                        m_y_table{0.f, 0.f, 1.f, 1.5f, 1.5f} {};

protected:
   std::array<float, m_lookup_table_size> m_x_table;
   float m_min_m_x_table = 0.f;
   float m_max_m_x_table = 10.f;

   std::array<float, m_lookup_table_size> m_y_table;
   float m_min_m_y_table = 0.f;
   float m_max_m_y_table = 1.5f;

   float m_x_threshhold = 1.0f;
   float m_y_threshhold = 2.0f;
};

/**
 * This function will test whether x below lowest value of the m_x_table will be defaulted to the lowest one on the m_y_table
 * \sdd{WI-13738}
 */
TEST_F(LookupTableTestFixture, WI_14983_Get_Value_From_2d_Lookup_Table__x_below_m_x_table_shall_be_m_min_m_y_table)
{
   /** \arrange */

   /** \action call function under test */
   float result = Get_Value_From_2d_Lookup_Table(
      m_x_table.data(),
      m_y_table.data(),
      m_lookup_table_size,
      m_min_m_x_table - 1);

   /** \assert */
   EXPECT_EQ(result, m_min_m_y_table);
}

/**
 * This function will test whether x above max value of the m_x_table will be defaulted to the highest one on the m_y_table
 * \sdd{WI-13738}
 */
TEST_F(LookupTableTestFixture, WI_14984_Get_Value_From_2d_Lookup_Table__x_above_m_x_table_shall_be_m_max_m_y_table)
{
   /** \arrange */

   /** \action call function under test */
   float result = Get_Value_From_2d_Lookup_Table(
      m_x_table.data(),
      m_y_table.data(),
      m_lookup_table_size,
      m_max_m_x_table + 1);

   /** \assert */
   EXPECT_EQ(result, m_max_m_y_table);
}

/**
 * \sdd{WI-13738}
 */
TEST_F(LookupTableTestFixture, WI_14985_Get_Value_From_2d_Lookup_Table__x_is_m_max_m_x_table)
{
   /** \arrange */

   /** \action call function under test */
   float result = Get_Value_From_2d_Lookup_Table(
      m_x_table.data(),
      m_y_table.data(),
      m_lookup_table_size,
      m_max_m_x_table);

   /** \assert */
   EXPECT_EQ(result, m_max_m_y_table);
}

/**
 * \sdd{WI-13738}
 */
TEST_F(LookupTableTestFixture, WI_14986_Get_Value_From_2d_Lookup_Table__x_is_m_min_m_x_table)
{
   /** \arrange */

   /** \action call function under test */
   float result = Get_Value_From_2d_Lookup_Table(
      m_x_table.data(),
      m_y_table.data(),
      m_lookup_table_size,
      m_min_m_x_table);

   /** \assert */
   EXPECT_EQ(result, m_min_m_y_table);
}

/**
 * This function will test a specific value from the lookuptable
 * \sdd{WI-13738}
 */
TEST_F(LookupTableTestFixture, WI_14987_Get_Value_From_2d_Lookup_Table__x_2_1_shall_be_zero)
{
   /** \arrange */
   float x_value_to_test = 2.1f;

   /** \action call function under test */
   float result = Get_Value_From_2d_Lookup_Table(
      m_x_table.data(),
      m_y_table.data(),
      m_lookup_table_size,
      x_value_to_test);

   /** \assert */
   EXPECT_EQ(result, 0);
}

/**
 * This function will test a specific value from the lookuptable
 * \sdd{WI-13738}
 */
TEST_F(LookupTableTestFixture, WI_14988_Get_Value_From_2d_Lookup_Table__x_9_5_shall_be_1_5)
{
   /** \arrange */
   float x_value_to_test = 9.5;

   /** \action call function under test */
   float result = Get_Value_From_2d_Lookup_Table(
      m_x_table.data(),
      m_y_table.data(),
      m_lookup_table_size,
      x_value_to_test);

   /** \assert */
   EXPECT_EQ(result, 1.5);
}

/**
 * This function will test a specific value from the lookuptable
 * \sdd{WI-13738}
 */
TEST_F(LookupTableTestFixture, WI_14989_Get_Value_From_2d_Lookup_Table__x_3_5_shall_be_0_5)
{
   /** \arrange */
   float x_value_to_test = 3.5;

   /** \action call function under test */
   float result = Get_Value_From_2d_Lookup_Table(
      m_x_table.data(),
      m_y_table.data(),
      m_lookup_table_size,
      x_value_to_test);

   /** \assert */
   EXPECT_EQ(result, 0.5);
}

/**
 * This function will test that when the x value is exactly equal to m_x_table[0] (first value), then it returns m_y_table[0]
 * \sdd{WI-13738}
 */
TEST_F(LookupTableTestFixture, WI_14990_Get_Value_From_2d_Lookup_Table__x_xtable0_shall_be_ytable0)
{
   /** \arrange */
   float x_value_to_test = 0.0;

   /** \action call function under test */
   float result = Get_Value_From_2d_Lookup_Table(
      m_x_table.data(),
      m_y_table.data(),
      m_lookup_table_size,
      x_value_to_test);

   /** \assert */
   EXPECT_EQ(result, 0.0);
}

/**
 * This function will test that when the x value is exactly equal to a m_x_table[2], then it returns m_y_table[2]
 * \sdd{WI-13738}
 */
TEST_F(LookupTableTestFixture, WI_14991_Get_Value_From_2d_Lookup_Table__m_x_table2_shall_be_ytable2)
{
   /** \arrange */
   float x_value_to_test = 4.0;

   /** \action call function under test */
   float result = Get_Value_From_2d_Lookup_Table(
      m_x_table.data(),
      m_y_table.data(),
      m_lookup_table_size,
      x_value_to_test);

   /** \assert */
   EXPECT_EQ(result, 1.0);
}

/**
 * This function will test that when the x value is exactly equal to a m_x_table[4] (last value), then it returns m_y_table[4]
 * \sdd{WI-13738}
 */
TEST_F(LookupTableTestFixture, WI_14992_Get_Value_From_2d_Lookup_Table__x_xtable4_shall_be_ytable4)
{
   /** \arrange */
   float x_value_to_test = 10.0;

   /** \action call function under test */
   float result = Get_Value_From_2d_Lookup_Table(
      m_x_table.data(),
      m_y_table.data(),
      m_lookup_table_size,
      x_value_to_test);

   /** \assert */
   EXPECT_EQ(result, 1.5);
}

/**
 * Test Function Interpolate_To_Zero
 * \sdd{WI-13740}
 */
TEST_F(LookupTableTestFixture, WI_14993_Interpolate_To_Zero__should_saturate_above_m_x_threshhold)
{
   /** \arrange */
   float x = m_x_threshhold + EPSILON /**< value to be interpolated to zero */;

   /** \action call function under test */
   float retvalue = Interpolate_To_Zero(x, m_x_threshhold, m_y_threshhold);

   /** \assert */
   EXPECT_EQ(retvalue, m_y_threshhold);
}

/**
 * Test Function Interpolate_To_Zero
 * \sdd{WI-13740}
 */
TEST_F(LookupTableTestFixture, WI_14994_Interpolate_To_Zero__should_saturate_below_zero)
{
   /** \arrange */
   float x = -EPSILON /**< value to be interpolated to zero */;

   /** \action call function under test */
   float retvalue = Interpolate_To_Zero(x, m_x_threshhold, m_y_threshhold);

   /** \assert */
   EXPECT_EQ(retvalue, 0);
}

/**
 * Test Function Interpolate_To_Zero
 * \sdd{WI-13740}
 */
TEST_F(LookupTableTestFixture, WI_14995_Interpolate_To_Zero__should_interpolate_linearly_between_the_origin_and_the_threshold_point)
{
   /** \arrange */
   float x = 0.5 /**< value to be interpolated to zero */;
   float expect = m_y_threshhold * (x / m_x_threshhold) /**< the expected value should be the result of a linear interpolation */;
   /** \action call function under test */
   float retvalue = Interpolate_To_Zero(x, m_x_threshhold, m_y_threshhold);

   /** \assert */
   EXPECT_EQ(retvalue, expect);
}
