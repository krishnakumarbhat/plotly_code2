/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

/**
 * As_Unit_Test does offer a reuse.h file that does offer a bunch of type definitions.
 * The tests below ensure that the size of these types matches the expectations.
 */

#include <gtest/gtest.h>
#include "reuse.h"

TEST(ReuseHeaderTest, size_of_float32_T)
{
   /** \arrange */
   /** \action call function under test */
   /** \assert */
   EXPECT_EQ(sizeof(float32_T), 4u);
}

TEST(ReuseHeaderTest, size_of_uint8_t)
{
   /** \arrange */
   /** \action call function under test */
   /** \assert */
   EXPECT_EQ(sizeof(uint8_t), 1u);
}
TEST(ReuseHeaderTest, size_of_uint16_t)
{
   /** \arrange */
   /** \action call function under test */
   /** \assert */
   EXPECT_EQ(sizeof(uint16_t), 2u);
}
TEST(ReuseHeaderTest, size_of_uint32_T)
{
   /** \arrange */
   /** \action call function under test */
   /** \assert */
   EXPECT_EQ(sizeof(uint32_t), 4u);
}

TEST(ReuseHeaderTest, size_of_uint64_T)
{
   /** \arrange */
   /** \action call function under test */
   /** \assert */
   EXPECT_EQ(sizeof(uint64_t), 8u);
}

TEST(ReuseHeaderTest, size_of_int8_t)
{
   /** \arrange */
   /** \action call function under test */
   /** \assert */
   EXPECT_EQ(sizeof(int8_t), 1u);
}
TEST(ReuseHeaderTest, size_of_int16_T)
{
   /** \arrange */
   /** \action call function under test */
   /** \assert */
   EXPECT_EQ(sizeof(int16_t), 2u);
}
TEST(ReuseHeaderTest, size_of_int32_T)
{
   /** \arrange */
   /** \action call function under test */
   /** \assert */
   EXPECT_EQ(sizeof(int32_t), 4u);
}

TEST(ReuseHeaderTest, size_of_int64_T)
{
   /** \arrange */
   /** \action call function under test */
   /** \assert */
   EXPECT_EQ(sizeof(int64_t), 8u);
}

TEST(ReuseHeaderTest, size_of_bitfield8_t)
{
   /** \arrange */
   /** \action call function under test */
   /** \assert */
   EXPECT_EQ(sizeof(bitfield8_t), 1u);
}
TEST(ReuseHeaderTest, size_of_bitfield16_t)
{
   /** \arrange */
   /** \action call function under test */
   /** \assert */
   EXPECT_EQ(sizeof(bitfield16_t), 2u);
}
TEST(ReuseHeaderTest, size_of_bitfield32_t)
{
   /** \arrange */
   /** \action call function under test */
   /** \assert */
   EXPECT_EQ(sizeof(bitfield32_t), 4u);
}
