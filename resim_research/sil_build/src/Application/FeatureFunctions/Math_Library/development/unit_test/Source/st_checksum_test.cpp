/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <gtest/gtest.h>


#include <array>
#include "ml_checksum.h"

#include <array>

/**
 * Demo on how to use the checksum to secure a data structure
 */
TEST(ChecksumTestFixture, demo_u8)
{
   /** \arrange Define a data structure containing a checksum member that does not have padding bytes
     *          The easiest way to achieve this is to sort the members
     *          by memory size, biggest to smallest. */
   struct Test_T
   {
      float m_a;
      uint32_t m_b;
      uint16_t m_c;
      uint8_t m_d;
      uint8_t m_checksum;
   };
   /** \arrange Create an instance of the data structure, fill it with arbitrary values */
   Test_T test{
      /* .m_a = */ 1234.5678f,
      /* .m_b = */ 5678,
      /* .m_c = */ 9012,
      /* .m_d = */ 34,
      /** \arrange Set the checksum inside the data structure to zero */
      /* .m_checksum = */ 0
      };

   /** \arrange Set the checksum member to the twos complement of the calculated checksum + 1 */
   test.m_checksum = ~Calc_Checksum_U8(&test, sizeof(Test_T)) + 1;
   /** \assert The recalculated checksum of the structure now is zero */
   EXPECT_EQ(Calc_Checksum_U8(&test, sizeof(Test_T)), 0u);
   /** \arrange change some member value a bit */
   test.m_b += 1;
   /** \assert the checksum no longer is zero */
   EXPECT_NE(Calc_Checksum_U8(&test, sizeof(Test_T)), 0u);
}

/**
* Demo on how to use the checksum to secure a data structure
*/
TEST(ChecksumTestFixture, demo_u16)
{
   /** \arrange Define a data structure containing a checksum member that does not have padding bytes
     *          The easiest way to achieve this is to sort the members
     *          by memory size, biggest to smallest. */
     /**[data_structure]*/
   struct Test_T
   {
      float    m_a;
      uint32_t m_b;
      uint16_t m_c;
      uint16_t m_checksum;
      uint8_t  m_d;
      uint8_t  m_e;
   };
   /**[data_structure]*/
   /** \arrange Create an instance of the data structure, fill it with arbitrary values */
   /**[data_content]*/
   Test_T test{
      /* .m_a = */ 1234.5678f,
      /* .m_b = */ 5678,
      /* .m_c = */ 9012,
      /** \arrange Set the checksum inside the data structure to zero */
      /* .m_checksum = */ 0,
      /* .m_d = */ 12,
      /* .m_e = */ 34
   };
   /**[data_content]*/
   /** \arrange Set the checksum member to the twos complement of the calculated checksum + 1 */
   /**[set_cs]*/
   test.m_checksum = ~Calc_Checksum_U16(&test, sizeof(Test_T)) + 1;
   /**[set_cs]*/
   /** \assert The recalculated checksum of the structure now is zero */
   /**[check_cs1]*/
   EXPECT_EQ(Calc_Checksum_U16(&test, sizeof(Test_T)), 0u);
   /**[check_cs1]*/
   /** \arrange change some member value a bit */
   /**[change_data]*/
   test.m_b += 1;
   /**[change_data]*/
   /** \assert the checksum no longer is zero */
   /**[check_cs2]*/
   EXPECT_NE(Calc_Checksum_U16(&test, sizeof(Test_T)), 0u);
   /**[check_cs2]*/
}

/**
* Demo on how to use the checksum to secure a data structure
*/
TEST(ChecksumTestFixture, demo_u32)
{
   /** \arrange Define a data structure containing a checksum member that does not have padding bytes
   *          The easiest way to achieve this is to sort the members
   *          by memory size, biggest to smallest. */
   struct Test_T
   {
      float    m_a;
      uint32_t m_b;
      uint32_t m_c;
      uint32_t m_checksum;
   };
   Test_T test{
      /** \arrange Create an instance of the data structure, fill it with arbitrary values */
      /* .m_a = */ 1234.5678f,
      /* .m_b = */ 456,
      /* .m_c = */ 789,
      /** \arrange Set the checksum inside the data structure to zero */
      /* .m_checksum = */ 0
   };
   /** \arrange Set the checksum member to the twos complement of the calculated checksum + 1 */
   test.m_checksum = ~Calc_Checksum_U32(&test, sizeof(Test_T)) + 1;
   /** \assert The recalculated checksum of the structure now is zero */
   EXPECT_EQ(Calc_Checksum_U32(&test, sizeof(Test_T)), 0u);
   /** \arrange change some member value a bit */
   test.m_b += 1;
   /** \assert the checksum no longer is zero */
   EXPECT_NE(Calc_Checksum_U32(&test, sizeof(Test_T)), 0u);
}

/**
 * Test fixture for testing the checksum functions.
 * It fills each entry of an array with the index plus 1.
 * This allows to calculate the resulting sum with the provided member
 * function CalcSum().
 * The test suit picks some starting indexes and ranges
 * within this array.
 * The expected checksum then is
 * \code
 * CalcSum(start_index, end_index);
 * \endcode
 */
class ChecksumTestFixture : public ::testing::TestWithParam<std::tuple <size_t, size_t>>
{
public:
   const static size_t buffer_size = 1096; /**< Size of the data buffer to calculate a checksum
                                             *  Needs to be public to be able to access it in
                                             *  INSTANTIATE_TEST_SUITE_P as last end index*/
protected:
   std::array<uint8_t, buffer_size>  m_data_u8 = {};   /**< A blob of uint8_t data to compute a checksum over*/
   std::array<uint16_t, buffer_size> m_data_u16 = {};  /**< A blob of uint16_t data to compute a checksum over*/
   std::array<uint32_t, buffer_size> m_data_u32 = {};  /**< A blob of uint32_t data to compute a checksum over*/
   size_t m_start_index;       /**< start index within the blob to start checksum computation on */
   size_t m_end_index;         /**< end index within the blob to stop checksum computation on */
   size_t m_index_range;       /**< the size of the checksum block (end_index - start_index)*/
   size_t m_expected_checksum; /**< The expected result of the checksum calculation */

   /**
    * \arrange Prepare a checksum calculation test
    * - Initializes the member data arrays with consecutive values starting from 1.
    * - Sets start_index according to test suit
    * - Sets end_index according to test suit
    * - Sets index_range according to test suit
    * - computes the expected chacksum
    */
   ChecksumTestFixture()
   {
      for (size_t i = 0; i < buffer_size; i++)
      {
         m_data_u8[i] = (char)i+1;
         m_data_u16[i] = (uint16_t)i+1;
         m_data_u32[i] = (uint32_t)i+1;
      }
      m_start_index = std::get<0>(GetParam());
      m_end_index = std::get<1>(GetParam());

      m_index_range = m_end_index - m_start_index;

      m_expected_checksum = CalcSum(m_start_index, m_end_index);
   }

   /**
   * Calculates the sum of consecutive values between given start and the given end.
   */
   static size_t CalcSum(
      size_t start,
      size_t end)
   {
      return CalcSum(end) - CalcSum(start);
   }
private:
   /**
   * Calculates the sum of consecutive values between 1 and the given value
   */
   static size_t CalcSum(size_t value)
   {
      return value*(value + 1) / 2;
   }
};

/** Pick arbitrary start and end indexes for the fixtures member data arrays to compute the checksum over. */
INSTANTIATE_TEST_SUITE_P(some_ranges,
   ChecksumTestFixture,
   ::testing::Combine(
      ::testing::Values(0,100,500), /* start index */
      ::testing::Values(600, 800, ChecksumTestFixture::buffer_size) /* end index */
   )
);

/**
 * Test the computed uint8_t checksum
 * \sdd{WI-14651}
 */
TEST_P(ChecksumTestFixture, WI_14871_compare_result_u8)
{
   /** \action run function under test */
   uint8_t calculated_checksum = Calc_Checksum_U8(&m_data_u8[m_start_index], m_index_range);

   /** \assert calculated checksum matches expectation */
   EXPECT_EQ((uint8_t)m_expected_checksum, calculated_checksum);
}

/**
 * Test the computed uint16_t checksum
 * \sdd{WI-14653}
 */
TEST_P(ChecksumTestFixture, WI_14872_compare_result_u16)
{
   /** \action run function under test */
   uint16_t calculated_checksum = Calc_Checksum_U16(&m_data_u16[m_start_index], sizeof(uint16_t)*m_index_range);

   /** \assert calculated checksum matches expectation */
   EXPECT_EQ((uint16_t)m_expected_checksum, calculated_checksum);
}

/**
 * Test the computed uint32_t checksum
 * \sdd{WI-14652}
 */
TEST_P(ChecksumTestFixture, WI_14873_compare_result_u32)
{
   /** \action run function under test */
   uint32_t calculated_checksum = Calc_Checksum_U32(&m_data_u32[m_start_index], sizeof(uint32_t)*m_index_range);

   /** \assert calculated checksum matches expectation */
   EXPECT_EQ((uint32_t)m_expected_checksum, calculated_checksum);
}

