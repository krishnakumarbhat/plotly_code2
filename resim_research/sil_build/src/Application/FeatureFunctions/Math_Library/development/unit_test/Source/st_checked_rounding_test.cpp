/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/


#include <gtest/gtest.h>
#include "ml_checked_rounding.h"


/**
 * \sdd{WI-13859}
 */
TEST(StCheckedRoundingTest, WI_15634_Ml_Roundf_Test)
{
   float raw_pos = 10.1f;
   float rounded_pos;
   float raw_neg = -10.1f;
   float rounded_neg;

   rounded_pos = Ml_Roundf(raw_pos);
   rounded_neg = Ml_Roundf(raw_neg);

   EXPECT_EQ(rounded_pos, 10.0f);
   EXPECT_EQ(rounded_neg, -10.0f);
}

/**
 * \sdd{WI-13860}
 * \sdd{WI-13866}
 */
TEST(StCheckedRoundingTest, WI_14862_Roundf_Checked_Uint8)
{
   /** \arrange */
   float   round_to_uint8 = -0.1f;
   uint8_t round_result_uint8;

   /** \action call function under test */
   round_result_uint8 = Roundf_Checked_Uint8(round_to_uint8);

   /** \assert */
   EXPECT_EQ(round_result_uint8, 0u);
}

/**
 * \sdd{WI-13860}
 * \sdd{WI-13863}
 */
TEST(StCheckedRoundingTest, WI_14863_Roundf_Checked_Int8_pos)
{
   /** \arrange */
   float  round_to_int8_pos = (float)INT8_MAX + 0.1f;
   int8_t round_result_int8_pos;

   /** \action call function under test */
   round_result_int8_pos = Roundf_Checked_Int8(round_to_int8_pos);

   /** \assert */
   EXPECT_EQ(round_result_int8_pos, INT8_MAX);
}

/**
 * \sdd{WI-13860}
 * \sdd{WI-13863}
 */
TEST(StCheckedRoundingTest, WI_14864_Roundf_Checked_Int8_neg)
{
   /** \arrange */
   float  round_to_int8_neg = (float)INT8_MIN - 0.1f;
   int8_t round_result_int8_neg;

   /** \action call function under test */
   round_result_int8_neg = Roundf_Checked_Int8(round_to_int8_neg);

   /** \assert */
   EXPECT_EQ(round_result_int8_neg, INT8_MIN);
}

/**
 * \sdd{WI-13860}
 * \sdd{WI-13865}
 */
TEST(StCheckedRoundingTest, WI_14865_Roundf_Checked_Uint16)
{
   /** \arrange */

   float    round_to_uint16 = (float)UINT16_MAX + 0.1f;
   uint16_t round_result_uint16;

   /** \action call function under test */
   round_result_uint16 = Roundf_Checked_Uint16(round_to_uint16);

   /** \assert */

   EXPECT_EQ(round_result_uint16, UINT16_MAX);
}

/**
 * \sdd{WI-13860}
 * \sdd{WI-13861}
 */
TEST(StCheckedRoundingTest, WI_14866_Roundf_Checked_Int16_pos)
{
   /** \arrange */
   float   round_to_int16_pos = (float)INT16_MAX + 0.1f;
   int16_t round_result_int16_pos;

   /** \action call function under test */
   round_result_int16_pos = Roundf_Checked_Int16(round_to_int16_pos);
   /** \assert */
   EXPECT_EQ(round_result_int16_pos, INT16_MAX);
}

/**
 * \sdd{WI-13860}
 * \sdd{WI-13861}
 */
TEST(StCheckedRoundingTest, WI_14867_Roundf_Checked_Int16_neg)
{
   /** \arrange */
   float   round_to_int16_neg = (float)INT16_MIN - 0.1f;
   int16_t round_result_int16_neg;

   /** \action call function under test */
   round_result_int16_neg = Roundf_Checked_Int16(round_to_int16_neg);

   /** \assert */
   EXPECT_EQ(round_result_int16_neg, INT16_MIN);
}

/**
 * \sdd{WI-13860}
 * \sdd{WI-13864}
 */
TEST(StCheckedRoundingTest, WI_14868_Roundf_Checked_Uint32)
{
   /** \arrange */
   float    round_to_uint32 = 10000.1f;
   uint32_t round_result_uint32;

   /** \action call function under test */
   round_result_uint32 = Roundf_Checked_Uint32(round_to_uint32);

   /** \assert */
   EXPECT_EQ(round_result_uint32, 10000u);
}

/**
 * \sdd{WI-13860}
 * \sdd{WI-13862}
 */
TEST(StCheckedRoundingTest, WI_14869_Roundf_Checked_Int32_pos)
{
   /** \arrange */
   float   round_to_int32_pos = 10000.1f;
   int32_t round_result_int32_pos;

   /** \action call function under test */
   round_result_int32_pos = Roundf_Checked_Int32(round_to_int32_pos);

   /** \assert */
   EXPECT_EQ(round_result_int32_pos, 10000);
}

/**
 * \sdd{WI-13860}
 * \sdd{WI-13862}
 */
TEST(StCheckedRoundingTest, WI_14870_Roundf_Checked_Int32_neg)
{
   /** \arrange */
   float   round_to_int32_neg = (float)INT32_MIN - 0.1f;
   int32_t round_result_int32_neg;

   /** \action call function under test */
   round_result_int32_neg = Roundf_Checked_Int32(round_to_int32_neg);

   /** \assert */
   EXPECT_EQ(round_result_int32_neg, INT32_MIN);
}

