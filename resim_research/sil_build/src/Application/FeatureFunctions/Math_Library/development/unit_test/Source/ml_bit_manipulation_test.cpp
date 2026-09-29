/*================================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved. *
 * Confidential - Restricted Aptiv information. Do not disclose.                  *
\*================================================================================*/

#include "gtest/gtest.h"

class ml_bit_manipulation_Test : public ::testing::TestWithParam<int> {
   // You can implement all the usual fixture class members here.
   // To access the test parameter, call GetParam() from class
   // TestWithParam<T>.
};


extern "C" {
#include "ml_bit_manipulation.h"
}

INSTANTIATE_TEST_SUITE_P(ml_bit_manipulation_Tests,
                        ml_bit_manipulation_Test,
                        ::testing::Range(0, 32, 1));

/**
 * Try to get highest set bit
 */
TEST_P(ml_bit_manipulation_Test, Bitman_Get_Highest_Set_Bit_Index_BF32)
{
   /** \arrange Set one arbitrary bit */
   uint32_t param    = GetParam();
   uint32_t bit_test = 1 << param;
   /** \action call function under test */
   uint32_t index_result = Bitman_Get_Highest_Set_Bit_Index_BF32(bit_test);

   /** \assert Expect returned index to match set bit index*/
   EXPECT_EQ(param, index_result);
}

/**
 * Try to get highest set bit of a zero value
 */
TEST_F(ml_bit_manipulation_Test, Bitman_Get_Highest_Set_Bit_Index_BF32_no_bit_set)
{
   /** \action call function under test with 0 as parameter */
   uint32_t index_result = Bitman_Get_Highest_Set_Bit_Index_BF32(0);

   /** \assert Expect 0xffffffff to be returned */
   EXPECT_EQ(0xffffffff, index_result);
}

/**
 * Try to get lowest set bit
 */
TEST_P(ml_bit_manipulation_Test, Bitman_Get_Lowest_Set_Bit_Index_BF32)
{
   /** \arrange set one arbitrary bit */
   uint32_t param    = GetParam();
   uint32_t bit_test = 1 << param;
   /** \action call function under test */
   uint32_t index_result = Bitman_Get_Lowest_Set_Bit_Index_BF32(bit_test);

   /** \assert Expect returned index to match set bit index */
   EXPECT_EQ(param, index_result);
}

/**
 * Try to get lowest set bit of a zero value
 */
TEST_F(ml_bit_manipulation_Test, Bitman_Get_Lowest_Set_Bit_Index_BF32_no_bit_set)
{
   /** \action call function under test with a parameter of 0*/
   uint32_t index_result = Bitman_Get_Lowest_Set_Bit_Index_BF32(0);

   /** \assert Expect 0xffffffff to be returned */
   EXPECT_EQ(0xffffffff, index_result);
}

/**
 * Try to unset a bit thats set
 */
TEST_P(ml_bit_manipulation_Test, Bitman_Unset_Bit_BF32__unset_bit_which_is_set)
{
   /** \arrange set one arbitrary bit*/
   uint8_t  param    = (uint8_t)GetParam();
   uint32_t bit_test = 1 << param;
   /** \action call function under test */
   uint32_t unset_result = Bitman_Unset_Bit_BF32(bit_test, param);

   /** \assert Expect bit to be reset*/
   EXPECT_EQ((uint32_t)0, unset_result);
}

/**
 * Try to unset a bit thats set
 */
TEST_P(ml_bit_manipulation_Test, Bitman_Unset_Bit_BF32__unset_bit_when_all_bits_are_Set)
{
   /** \arrange Choose one arbitrary bit*/
   uint8_t  param    = (uint8_t)GetParam();
   uint32_t bit_test = 0xffffffff;
   /** \action call function under test */
   uint32_t unset_result = Bitman_Unset_Bit_BF32(bit_test, param);

   /** \assert the chosen bit is unset*/
   EXPECT_EQ((uint32_t)0, (unset_result >> param) & 1);
   /** \assert other bits are still set */
   EXPECT_EQ((uint32_t)0xffffffff, (1 << param) | unset_result);
}

/**
 * Try to unset a bit thats not set
 */
TEST_P(ml_bit_manipulation_Test, Bitman_Unset_Bit_BF32__unset_bit_which_is_not_set)
{
   /** \arrange Pick one arbitrary bit */
   uint8_t param = (uint8_t)GetParam();
   /** \action call function under test */
   uint32_t unset_result = Bitman_Unset_Bit_BF32(0, param);

   /** \assert the chosen bit is unset*/
   EXPECT_EQ((uint32_t)0, unset_result);
}

/**
 * Try to unset a bit thats not set
 */
TEST_P(ml_bit_manipulation_Test, Bitman_Unset_Bit_BF32__unset_bit_which_is_not_set_all_others_are_Set)
{
   /** \arrange Pick one arbitrary bit */
   uint8_t  param    = (uint8_t)GetParam();
   uint32_t bit_test = 1 << param;

   bit_test = ~bit_test;
   /** \action call function under test */
   uint32_t unset_result = Bitman_Unset_Bit_BF32(bit_test, param);
   /** \assert desired bit is unset*/
   EXPECT_EQ((uint32_t)0, (unset_result >> param) & 1);
   /** \assert other bits are still set */
   EXPECT_EQ((uint32_t)0xffffffff, (1 << param) | unset_result);
}

/**
 * Try to unset a bit
 */
TEST_P(ml_bit_manipulation_Test, Bitman_Unset_Bit_Pointer_BF32)
{
    /** \arrange Pick one arbitrary bit */
   uint8_t param = (uint8_t)GetParam();
   uint32_t bit_test = 1 << param;
   /** \action call function under test */
   Bitman_Unset_Bit_Pointer_BF32(&bit_test, param);
   /** \assert desired bit is unset*/
   EXPECT_EQ((uint32_t)0, bit_test);
}

/**
 * Try to get a bit
 */
TEST_P(ml_bit_manipulation_Test, Bitman_Get_Bit_BF32)
{
    /** \arrange Pick one arbitrary bit */
   uint8_t param = (uint8_t)GetParam();
   uint32_t bit_test = 1 << param;
   /** \action call function under test */
   boolean_T ret = Bitman_Get_Bit_BF32(bit_test, param);
   /** \assert Correct bit state is returned */
   EXPECT_TRUE(ret);
}

/**
 * Try to set a bit
 */
TEST_P(ml_bit_manipulation_Test, Bitman_Set_Bit_BF32)
{
   /** \arrange Pick one arbitrary bit */
   uint8_t  param    = (uint8_t)GetParam();
   uint32_t bit_test = 1 << param;
   /** \action call function under test */
   uint32_t set_result = Bitman_Set_Bit_BF32(0xaa, param);

   /** \assert Correct bit state is returned */
   bit_test = bit_test | 0xaa;
   EXPECT_EQ(bit_test, set_result);
}

/**
 * Try to invert a bit
 */
TEST_P(ml_bit_manipulation_Test, Bitman_Invert_Bit_BF32)
{
   /** \arrange Pick one arbitrary bit */
   uint8_t  param    = (uint8_t)GetParam();
   uint32_t bit_test = 1 << param;
   /** \action call function under test */
   uint32_t set_result = Bitman_Invert_Bit_BF32(0xaa, param);

   /** \arrange read bit state at index param */
   bool original_state = (1 == (bit_test & 0xaa) >> param);
   bool new_state      = (1 == (bit_test & set_result) >> param);

   /** \assert Correct bit state is returned */
   EXPECT_EQ(!original_state, new_state);
}

/**
 * Try to set a bit
 */
TEST_P(ml_bit_manipulation_Test, Bitman_Set_Bit_Pointer_BF32)
{
    /** \arrange Pick one arbitrary bit */
   uint8_t param = (uint8_t)GetParam();
   uint32_t bit_test = 1 << param;
   uint32_t set_result = 0xaa;
   /** \action call function under test */
   Bitman_Set_Bit_Pointer_BF32(&set_result, param);

   /** \assert Correct bit state is returned */
   bit_test = bit_test | 0xaa;
   EXPECT_EQ(bit_test, set_result);
}

/**
 * Try to get the index of the nth least significant bit
 */
TEST_P(ml_bit_manipulation_Test, get_nth_least_significant_bit_index)
{
    /** \arrange Pick one arbitrary bit */
   uint32_t param = GetParam();
   uint32_t bit_test = 0xffffffff;
   uint32_t index = Bitman_Get_Nth_Lowest_Set_Bit_Index_BF32(bit_test, param);
   /** \assert returned Index matches expectation */
   EXPECT_EQ(index, param);
}

/**
 * Try to get the index of the nth least significant bit
 */
TEST_P(ml_bit_manipulation_Test, get_nth_least_significant_bit_index__bit_pattern_55)
{
    /** \arrange Pick one arbitrary bit */
   uint32_t param = GetParam();
   uint32_t bit_test = 0x55555555;
   uint32_t index = Bitman_Get_Nth_Lowest_Set_Bit_Index_BF32(bit_test, param);
   /** \assert returned Index matches expectation */
   if (param < 16)
   {
      EXPECT_EQ(index, param * 2);
   }
   else
   {
      EXPECT_EQ(index, (uint32_t)-1);
   }
}

/**
 * Try to get the index of the nth least significant bit
 */
TEST_P(ml_bit_manipulation_Test, get_nth_least_significant_bit_index__bit_pattern_aa)
{
    /** \arrange Pick one arbitrary bit */
   uint32_t param = GetParam();
   uint32_t bit_test = 0xaaaaaaaa;
   uint32_t index = Bitman_Get_Nth_Lowest_Set_Bit_Index_BF32(bit_test, param);
   /** \assert returned Index matches expectation */
   if (param < 16)
   {
      EXPECT_EQ(index, param * 2 + 1);
   }
   else
   {
      EXPECT_EQ(index, (uint32_t)-1);
   }
}

/**
 * Count number of set bits
 */
TEST_F(ml_bit_manipulation_Test, Bitman_Count_Sparse_Set_Bits_BF32__counts_16)
{
    /** \action call function under test with pattern 0xaaaaaaaa*/
   uint32_t bit_test = 0xaaaaaaaa;
   uint32_t count = Bitman_Count_Sparse_Set_Bits_BF32(bit_test);
   /** \assert 16 to be returned: One byte of 0xaa has 4 set bits => 4 set bits * 4 bytes = 16 set bits */
   EXPECT_EQ(count, (uint32_t)16);
}

/**
 * Count number of set bits
 */
TEST_F(ml_bit_manipulation_Test, Bitman_Count_Sparse_Set_Bits_BF32__counts0)
{
    /** \action call function under test with pattern 0x0*/
   uint32_t bit_test = 0;
   uint32_t count = Bitman_Count_Sparse_Set_Bits_BF32(bit_test);
   /** \assert Expect zero to be returned */
   EXPECT_EQ(count, (uint32_t)0);
}

/**
 * Count number of set bits
 */
TEST_F(ml_bit_manipulation_Test, Bitman_Count_Sparse_Set_Bits_BF32__counts32)
{
    /** \action call function under test with pattern 0xffffffff*/
   uint32_t bit_test = 0xffffffff;
   uint32_t count = Bitman_Count_Sparse_Set_Bits_BF32(bit_test);
   /** \assert expect 32 to be returned */
   EXPECT_EQ(count, (uint32_t)32);
}
