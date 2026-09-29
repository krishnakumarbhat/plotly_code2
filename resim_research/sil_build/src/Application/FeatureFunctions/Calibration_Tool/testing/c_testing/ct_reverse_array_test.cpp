/**
 * @file ct_reverse_array_test.cpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test implementation for array reversing unit tests
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include "ct_reverse_array_test.hpp"

extern "C"
{
#include "ct_endianness_switch.h"
#include "reuse.h"
}

/**
 * Test array reversing function, Here, a two-element uint8_t array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Two_Len_one_byte_ui8)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_one_byte_test_2_len_array_ui8[0],
                    sizeof(Cal_Test_Struct.ct_one_byte_test_2_len_array_ui8), CT_ONE_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t iter = 0; iter < CT_ARRAY_TWO_LEN; ++iter)
   {
      EXPECT_EQ(Cal_Test_Struct.ct_one_byte_test_2_len_array_ui8[iter],
                Cal_Test_Reference_Struct.ct_one_byte_test_2_len_array_ui8[CT_ARRAY_TWO_LEN_REVERSE_ITER - iter]);
   }
}

/**
 * Test array reversing function, Here, a two-element uint16_t array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Two_Len_two_byte_ui16)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_two_byte_test_2_len_array_ui16[0],
                    sizeof(Cal_Test_Struct.ct_two_byte_test_2_len_array_ui16), CT_TWO_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t iter = 0; iter < CT_ARRAY_TWO_LEN; ++iter)
   {
      EXPECT_EQ(Cal_Test_Struct.ct_two_byte_test_2_len_array_ui16[iter],
                Cal_Test_Reference_Struct.ct_two_byte_test_2_len_array_ui16[CT_ARRAY_TWO_LEN_REVERSE_ITER - iter]);
   }
}

/**
 * Test array reversing function, Here, a two-element uint32_t array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Two_Len_four_byte_ui32)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_four_byte_test_2_len_array_ui32[0],
                    sizeof(Cal_Test_Struct.ct_four_byte_test_2_len_array_ui32), CT_FOUR_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t iter = 0; iter < CT_ARRAY_TWO_LEN; ++iter)
   {
      EXPECT_EQ(Cal_Test_Struct.ct_four_byte_test_2_len_array_ui32[iter],
                Cal_Test_Reference_Struct.ct_four_byte_test_2_len_array_ui32[CT_ARRAY_TWO_LEN_REVERSE_ITER - iter]);
   }
}

/**
 * Test array reversing function, Here, a two-element int8_t array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Two_Len_one_byte_i8)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_one_byte_test_2_len_array_i8[0],
                    sizeof(Cal_Test_Struct.ct_one_byte_test_2_len_array_i8), CT_ONE_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t iter = 0; iter < CT_ARRAY_TWO_LEN; ++iter)
   {
      EXPECT_EQ(Cal_Test_Struct.ct_one_byte_test_2_len_array_i8[iter],
                Cal_Test_Reference_Struct.ct_one_byte_test_2_len_array_i8[CT_ARRAY_TWO_LEN_REVERSE_ITER - iter]);
   }
}

/**
 * Test array reversing function, Here, a two-element int16_t array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Two_Len_two_byte_i16)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_two_byte_test_2_len_array_i16[0],
                    sizeof(Cal_Test_Struct.ct_two_byte_test_2_len_array_i16), CT_TWO_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t iter = 0; iter < CT_ARRAY_TWO_LEN; ++iter)
   {
      EXPECT_EQ(Cal_Test_Struct.ct_two_byte_test_2_len_array_i16[iter],
                Cal_Test_Reference_Struct.ct_two_byte_test_2_len_array_i16[CT_ARRAY_TWO_LEN_REVERSE_ITER - iter]);
   }
}

/**
 * Test array reversing function, Here, a two-element int32_t array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Two_Len_four_byte_i32)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_four_byte_test_2_len_array_i32[0],
                    sizeof(Cal_Test_Struct.ct_four_byte_test_2_len_array_i32), CT_FOUR_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t iter = 0; iter < CT_ARRAY_TWO_LEN; ++iter)
   {
      EXPECT_EQ(Cal_Test_Struct.ct_four_byte_test_2_len_array_i32[iter],
                Cal_Test_Reference_Struct.ct_four_byte_test_2_len_array_i32[CT_ARRAY_TWO_LEN_REVERSE_ITER - iter]);
   }
}

/**
 * Test array reversing function, Here, a two-element float32_T array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Two_Len_four_byte_f)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_four_byte_test_2_len_array_f[0],
                    sizeof(Cal_Test_Struct.ct_four_byte_test_2_len_array_f), CT_FOUR_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t iter = 0; iter < CT_ARRAY_TWO_LEN; ++iter)
   {
      EXPECT_EQ(Cal_Test_Struct.ct_four_byte_test_2_len_array_f[iter],
                Cal_Test_Reference_Struct.ct_four_byte_test_2_len_array_f[CT_ARRAY_TWO_LEN_REVERSE_ITER - iter]);
   }
}

/**
 * Test array reversing function, Here, a three-element uint8_t array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Three_Len_one_byte_ui8)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_one_byte_test_3_len_array_ui8[0],
                    sizeof(Cal_Test_Struct.ct_one_byte_test_3_len_array_ui8), CT_ONE_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t iter = 0; iter < CT_ARRAY_THREE_LEN; ++iter)
   {
      EXPECT_EQ(Cal_Test_Struct.ct_one_byte_test_3_len_array_ui8[iter],
                Cal_Test_Reference_Struct.ct_one_byte_test_3_len_array_ui8[CT_ARRAY_THREE_LEN_REVERSE_ITER - iter]);
   }
}

/**
 * Test array reversing function, Here, a three-element uint16_t array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Three_Len_two_byte_ui16)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_two_byte_test_3_len_array_ui16[0],
                    sizeof(Cal_Test_Struct.ct_two_byte_test_3_len_array_ui16), CT_TWO_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t iter = 0; iter < CT_ARRAY_THREE_LEN; ++iter)
   {
      EXPECT_EQ(Cal_Test_Struct.ct_two_byte_test_3_len_array_ui16[iter],
                Cal_Test_Reference_Struct.ct_two_byte_test_3_len_array_ui16[CT_ARRAY_THREE_LEN_REVERSE_ITER - iter]);
   }
}

/**
 * Test array reversing function, Here, a three-element uint32_t array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Three_Len_four_byte_ui32)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_four_byte_test_3_len_array_ui32[0],
                    sizeof(Cal_Test_Struct.ct_four_byte_test_3_len_array_ui32), CT_FOUR_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t iter = 0; iter < CT_ARRAY_THREE_LEN; ++iter)
   {
      EXPECT_EQ(Cal_Test_Struct.ct_four_byte_test_3_len_array_ui32[iter],
                Cal_Test_Reference_Struct.ct_four_byte_test_3_len_array_ui32[CT_ARRAY_THREE_LEN_REVERSE_ITER - iter]);
   }
}

/**
 * Test array reversing function, Here, a three-element int8_t array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Three_Len_one_byte_i8)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_one_byte_test_3_len_array_i8[0],
                    sizeof(Cal_Test_Struct.ct_one_byte_test_3_len_array_i8), CT_ONE_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t iter = 0; iter < CT_ARRAY_THREE_LEN; ++iter)
   {
      EXPECT_EQ(Cal_Test_Struct.ct_one_byte_test_3_len_array_i8[iter],
                Cal_Test_Reference_Struct.ct_one_byte_test_3_len_array_i8[CT_ARRAY_THREE_LEN_REVERSE_ITER - iter]);
   }
}

/**
 * Test array reversing function, Here, a three-element int16_t array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Three_Len_two_byte_i16)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_two_byte_test_3_len_array_i16[0],
                    sizeof(Cal_Test_Struct.ct_two_byte_test_3_len_array_i16), CT_TWO_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t iter = 0; iter < CT_ARRAY_THREE_LEN; ++iter)
   {
      EXPECT_EQ(Cal_Test_Struct.ct_two_byte_test_3_len_array_i16[iter],
                Cal_Test_Reference_Struct.ct_two_byte_test_3_len_array_i16[CT_ARRAY_THREE_LEN_REVERSE_ITER - iter]);
   }
}

/**
 * Test array reversing function, Here, a three-element int32_t array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Three_Len_four_byte_i32)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_four_byte_test_3_len_array_i32[0],
                    sizeof(Cal_Test_Struct.ct_four_byte_test_3_len_array_i32), CT_FOUR_BYTE);
   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */

   for (uint32_t iter = 0; iter < CT_ARRAY_THREE_LEN; ++iter)
   {
      EXPECT_EQ(Cal_Test_Struct.ct_four_byte_test_3_len_array_i32[iter],
                Cal_Test_Reference_Struct.ct_four_byte_test_3_len_array_i32[CT_ARRAY_THREE_LEN_REVERSE_ITER - iter]);
   }
}

/**
 * Test array reversing function, Here, a three-element float32_T array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Three_Len_four_byte_f)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_four_byte_test_3_len_array_f[0],
                    sizeof(Cal_Test_Struct.ct_four_byte_test_3_len_array_f), CT_FOUR_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t iter = 0; iter < CT_ARRAY_THREE_LEN; ++iter)
   {
      EXPECT_EQ(Cal_Test_Struct.ct_four_byte_test_3_len_array_f[iter],
                Cal_Test_Reference_Struct.ct_four_byte_test_3_len_array_f[CT_ARRAY_THREE_LEN_REVERSE_ITER - iter]);
   }
}

/**
 * Test array reversing function, Here, a four-element uint8_t array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Four_Len_one_byte_ui8)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_one_byte_test_4_len_array_ui8[0],
                    sizeof(Cal_Test_Struct.ct_one_byte_test_4_len_array_ui8), CT_ONE_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t iter = 0; iter < CT_ARRAY_FOUR_LEN; ++iter)
   {
      EXPECT_EQ(Cal_Test_Struct.ct_one_byte_test_4_len_array_ui8[iter],
                Cal_Test_Reference_Struct.ct_one_byte_test_4_len_array_ui8[CT_ARRAY_FOUR_LEN_REVERSE_ITER - iter]);
   }
}

/**
 * Test array reversing function, Here, a four-element uint16_t array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Four_Len_two_byte_ui16)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_two_byte_test_4_len_array_ui16[0],
                    sizeof(Cal_Test_Struct.ct_two_byte_test_4_len_array_ui16), CT_TWO_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t iter = 0; iter < CT_ARRAY_FOUR_LEN; ++iter)
   {
      EXPECT_EQ(Cal_Test_Struct.ct_two_byte_test_4_len_array_ui16[iter],
                Cal_Test_Reference_Struct.ct_two_byte_test_4_len_array_ui16[CT_ARRAY_FOUR_LEN_REVERSE_ITER - iter]);
   }
}

/**
 * Test array reversing function, Here, a four-element uint32_t array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Four_Len_four_byte_ui32)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_four_byte_test_4_len_array_ui32[0],
                    sizeof(Cal_Test_Struct.ct_four_byte_test_4_len_array_ui32), CT_FOUR_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t iter = 0; iter < CT_ARRAY_FOUR_LEN; ++iter)
   {
      EXPECT_EQ(Cal_Test_Struct.ct_four_byte_test_4_len_array_ui32[iter],
                Cal_Test_Reference_Struct.ct_four_byte_test_4_len_array_ui32[CT_ARRAY_FOUR_LEN_REVERSE_ITER - iter]);
   }
}

/**
 * Test array reversing function, Here, a four-element int8_t array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Four_Len_one_byte_i8)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_one_byte_test_4_len_array_i8[0],
                    sizeof(Cal_Test_Struct.ct_one_byte_test_4_len_array_i8), CT_ONE_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t iter = 0; iter < CT_ARRAY_FOUR_LEN; ++iter)
   {
      EXPECT_EQ(Cal_Test_Struct.ct_one_byte_test_4_len_array_i8[iter],
                Cal_Test_Reference_Struct.ct_one_byte_test_4_len_array_i8[CT_ARRAY_FOUR_LEN_REVERSE_ITER - iter]);
   }
}

/**
 * Test array reversing function, Here, a four-element int16_t array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Four_Len_two_byte_i16)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_two_byte_test_4_len_array_i16[0],
                    sizeof(Cal_Test_Struct.ct_two_byte_test_4_len_array_i16), CT_TWO_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t iter = 0; iter < CT_ARRAY_FOUR_LEN; ++iter)
   {
      EXPECT_EQ(Cal_Test_Struct.ct_two_byte_test_4_len_array_i16[iter],
                Cal_Test_Reference_Struct.ct_two_byte_test_4_len_array_i16[CT_ARRAY_FOUR_LEN_REVERSE_ITER - iter]);
   }
}

/**
 * Test array reversing function, Here, a four-element int32_t array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Four_Len_four_byte_i32)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_four_byte_test_4_len_array_i32[0],
                    sizeof(Cal_Test_Struct.ct_four_byte_test_4_len_array_i32), CT_FOUR_BYTE);
   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */

   for (uint32_t iter = 0; iter < CT_ARRAY_FOUR_LEN; ++iter)
   {
      EXPECT_EQ(Cal_Test_Struct.ct_four_byte_test_4_len_array_i32[iter],
                Cal_Test_Reference_Struct.ct_four_byte_test_4_len_array_i32[CT_ARRAY_FOUR_LEN_REVERSE_ITER - iter]);
   }
}

/**
 * Test array reversing function, Here, a four-element float32_T array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Four_Len_four_byte_f)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_four_byte_test_4_len_array_f[0],
                    sizeof(Cal_Test_Struct.ct_four_byte_test_4_len_array_f), CT_FOUR_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t iter = 0; iter < CT_ARRAY_FOUR_LEN; ++iter)
   {
      EXPECT_EQ(Cal_Test_Struct.ct_four_byte_test_4_len_array_f[iter],
                Cal_Test_Reference_Struct.ct_four_byte_test_4_len_array_f[CT_ARRAY_FOUR_LEN_REVERSE_ITER - iter]);
   }
}

/**
 * Test array reversing function, Here, a three-element uint8_t 2D array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Three_Len_one_byte_ui8_2d)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_one_byte_test_3_len_array_ui8_2d[0][0],
                    sizeof(Cal_Test_Struct.ct_one_byte_test_3_len_array_ui8_2d), CT_ONE_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t first_lvl_iter = 0; first_lvl_iter < CT_ARRAY_THREE_LEN; ++first_lvl_iter)
   {
      for (uint32_t second_lvl_iter = 0; second_lvl_iter < CT_ARRAY_THREE_LEN; ++second_lvl_iter)
      {
         EXPECT_EQ(Cal_Test_Struct.ct_one_byte_test_3_len_array_ui8_2d[first_lvl_iter][second_lvl_iter],
                   Cal_Test_Reference_Struct.ct_one_byte_test_3_len_array_ui8_2d[CT_ARRAY_THREE_LEN_REVERSE_ITER - first_lvl_iter]
                                                                                [CT_ARRAY_THREE_LEN_REVERSE_ITER - second_lvl_iter]);
      }
   }
}


/**
 * Test array reversing function, Here, a three-element uint16_t 2D array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Three_Len_two_byte_ui16_2d)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_two_byte_test_3_len_array_ui16_2d[0][0],
                    sizeof(Cal_Test_Struct.ct_two_byte_test_3_len_array_ui16_2d), CT_TWO_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t first_lvl_iter = 0; first_lvl_iter < CT_ARRAY_THREE_LEN; ++first_lvl_iter)
   {
      for (uint32_t second_lvl_iter = 0; second_lvl_iter < CT_ARRAY_THREE_LEN; ++second_lvl_iter)
      {
         EXPECT_EQ(Cal_Test_Struct.ct_two_byte_test_3_len_array_ui16_2d[first_lvl_iter][second_lvl_iter],
                   Cal_Test_Reference_Struct.ct_two_byte_test_3_len_array_ui16_2d[CT_ARRAY_THREE_LEN_REVERSE_ITER - first_lvl_iter]
                                                                                 [CT_ARRAY_THREE_LEN_REVERSE_ITER - second_lvl_iter]);
      }
   }
}

/**
 * Test array reversing function, Here, a three-element uint32_t 2D array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Three_Len_four_byte_ui32_2d)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_four_byte_test_3_len_array_ui32_2d[0][0],
                    sizeof(Cal_Test_Struct.ct_four_byte_test_3_len_array_ui32_2d), CT_FOUR_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t first_lvl_iter = 0; first_lvl_iter < CT_ARRAY_THREE_LEN; ++first_lvl_iter)
   {
      for (uint32_t second_lvl_iter = 0; second_lvl_iter < CT_ARRAY_THREE_LEN; ++second_lvl_iter)
      {
         EXPECT_EQ(Cal_Test_Struct.ct_four_byte_test_3_len_array_ui32_2d[first_lvl_iter][second_lvl_iter],
                   Cal_Test_Reference_Struct.ct_four_byte_test_3_len_array_ui32_2d[CT_ARRAY_THREE_LEN_REVERSE_ITER - first_lvl_iter]
                                                                                  [CT_ARRAY_THREE_LEN_REVERSE_ITER - second_lvl_iter]);
      }
   }
}

/**
 * Test array reversing function, Here, a three-element int8_t 2D array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Three_Len_one_byte_i8_2d)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_one_byte_test_3_len_array_i8_2d[0][0],
                    sizeof(Cal_Test_Struct.ct_one_byte_test_3_len_array_i8_2d), CT_ONE_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t first_lvl_iter = 0; first_lvl_iter < CT_ARRAY_THREE_LEN; ++first_lvl_iter)
   {
      for (uint32_t second_lvl_iter = 0; second_lvl_iter < CT_ARRAY_THREE_LEN; ++second_lvl_iter)
      {
         EXPECT_EQ(Cal_Test_Struct.ct_one_byte_test_3_len_array_i8_2d[first_lvl_iter][second_lvl_iter],
                   Cal_Test_Reference_Struct.ct_one_byte_test_3_len_array_i8_2d[CT_ARRAY_THREE_LEN_REVERSE_ITER - first_lvl_iter]
                                                                               [CT_ARRAY_THREE_LEN_REVERSE_ITER - second_lvl_iter]);
      }
   }
}

/**
 * Test array reversing function, Here, a three-element int16_t 2D array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Three_Len_two_byte_i16_2d)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_two_byte_test_3_len_array_i16_2d[0][0],
                    sizeof(Cal_Test_Struct.ct_two_byte_test_3_len_array_i16_2d), CT_TWO_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t first_lvl_iter = 0; first_lvl_iter < CT_ARRAY_THREE_LEN; ++first_lvl_iter)
   {
      for (uint32_t second_lvl_iter = 0; second_lvl_iter < CT_ARRAY_THREE_LEN; ++second_lvl_iter)
      {
         EXPECT_EQ(Cal_Test_Struct.ct_two_byte_test_3_len_array_i16_2d[first_lvl_iter][second_lvl_iter],
                   Cal_Test_Reference_Struct.ct_two_byte_test_3_len_array_i16_2d[CT_ARRAY_THREE_LEN_REVERSE_ITER - first_lvl_iter]
                                                                                [CT_ARRAY_THREE_LEN_REVERSE_ITER - second_lvl_iter]);
      }
   }
}

/**
 * Test array reversing function, Here, a three-element int32_t 2D array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Three_Len_four_byte_i32_2d)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_four_byte_test_3_len_array_i32_2d[0][0],
                    sizeof(Cal_Test_Struct.ct_four_byte_test_3_len_array_i32_2d), CT_FOUR_BYTE);
   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t first_lvl_iter = 0; first_lvl_iter < CT_ARRAY_THREE_LEN; ++first_lvl_iter)
   {
      for (uint32_t second_lvl_iter = 0; second_lvl_iter < CT_ARRAY_THREE_LEN; ++second_lvl_iter)
      {
         EXPECT_EQ(Cal_Test_Struct.ct_four_byte_test_3_len_array_i32_2d[first_lvl_iter][second_lvl_iter],
                   Cal_Test_Reference_Struct.ct_four_byte_test_3_len_array_i32_2d[CT_ARRAY_THREE_LEN_REVERSE_ITER - first_lvl_iter]
                                                                                 [CT_ARRAY_THREE_LEN_REVERSE_ITER - second_lvl_iter]);
      }
   }
}

/**
 * Test array reversing function, Here, a three-element float32_T 2D array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Three_Len_four_byte_f_2d)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_four_byte_test_3_len_array_f_2d[0][0],
                    sizeof(Cal_Test_Struct.ct_four_byte_test_3_len_array_f_2d), CT_FOUR_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t first_lvl_iter = 0; first_lvl_iter < CT_ARRAY_THREE_LEN; ++first_lvl_iter)
   {
      for (uint32_t second_lvl_iter = 0; second_lvl_iter < CT_ARRAY_THREE_LEN; ++second_lvl_iter)
      {
         EXPECT_EQ(Cal_Test_Struct.ct_four_byte_test_3_len_array_f_2d[first_lvl_iter][second_lvl_iter],
                   Cal_Test_Reference_Struct.ct_four_byte_test_3_len_array_f_2d[CT_ARRAY_THREE_LEN_REVERSE_ITER - first_lvl_iter]
                                                                               [CT_ARRAY_THREE_LEN_REVERSE_ITER - second_lvl_iter]);
      }
   }
}

/**
 * Test array reversing function, Here, a four-element uint8_t 2D array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Four_Len_one_byte_ui8_2d)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_one_byte_test_4_len_array_ui8_2d[0][0],
                    sizeof(Cal_Test_Struct.ct_one_byte_test_4_len_array_ui8_2d), CT_ONE_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t first_lvl_iter = 0; first_lvl_iter < CT_ARRAY_THREE_LEN; ++first_lvl_iter)
   {
      for (uint32_t second_lvl_iter = 0; second_lvl_iter < CT_ARRAY_FOUR_LEN; ++second_lvl_iter)
      {
         EXPECT_EQ(Cal_Test_Struct.ct_one_byte_test_4_len_array_ui8_2d[first_lvl_iter][second_lvl_iter],
                   Cal_Test_Reference_Struct.ct_one_byte_test_4_len_array_ui8_2d[CT_ARRAY_FOUR_LEN_REVERSE_ITER - first_lvl_iter]
                                                                                [CT_ARRAY_FOUR_LEN_REVERSE_ITER - second_lvl_iter]);
      }
   }
}

/**
 * Test array reversing function, Here, a four-element uint16_t 2D array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Four_Len_two_byte_ui16_2d)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_two_byte_test_4_len_array_ui16_2d[0][0],
                    sizeof(Cal_Test_Struct.ct_two_byte_test_4_len_array_ui16_2d), CT_TWO_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t first_lvl_iter = 0; first_lvl_iter < CT_ARRAY_THREE_LEN; ++first_lvl_iter)
   {
      for (uint32_t second_lvl_iter = 0; second_lvl_iter < CT_ARRAY_FOUR_LEN; ++second_lvl_iter)
      {
         EXPECT_EQ(Cal_Test_Struct.ct_two_byte_test_4_len_array_ui16_2d[first_lvl_iter][second_lvl_iter],
                   Cal_Test_Reference_Struct.ct_two_byte_test_4_len_array_ui16_2d[CT_ARRAY_FOUR_LEN_REVERSE_ITER - first_lvl_iter]
                                                                                 [CT_ARRAY_FOUR_LEN_REVERSE_ITER - second_lvl_iter]);
      }
   }
}

/**
 * Test array reversing function, Here, a four-element uint32_t 2D array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Four_Len_four_byte_ui32_2d)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_four_byte_test_4_len_array_ui32_2d[0][0],
                    sizeof(Cal_Test_Struct.ct_four_byte_test_4_len_array_ui32_2d), CT_FOUR_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t first_lvl_iter = 0; first_lvl_iter < CT_ARRAY_THREE_LEN; ++first_lvl_iter)
   {
      for (uint32_t second_lvl_iter = 0; second_lvl_iter < CT_ARRAY_FOUR_LEN; ++second_lvl_iter)
      {
         EXPECT_EQ(Cal_Test_Struct.ct_four_byte_test_4_len_array_ui32_2d[first_lvl_iter][second_lvl_iter],
                   Cal_Test_Reference_Struct.ct_four_byte_test_4_len_array_ui32_2d[CT_ARRAY_FOUR_LEN_REVERSE_ITER - first_lvl_iter]
                                                                                  [CT_ARRAY_FOUR_LEN_REVERSE_ITER - second_lvl_iter]);
      }
   }
}

/**
 * Test array reversing function, Here, a four-element int8_t 2D array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Four_Len_one_byte_i8_2d)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_one_byte_test_4_len_array_i8_2d[0][0],
                    sizeof(Cal_Test_Struct.ct_one_byte_test_4_len_array_i8_2d), CT_ONE_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t first_lvl_iter = 0; first_lvl_iter < CT_ARRAY_THREE_LEN; ++first_lvl_iter)
   {
      for (uint32_t second_lvl_iter = 0; second_lvl_iter < CT_ARRAY_FOUR_LEN; ++second_lvl_iter)
      {
         EXPECT_EQ(Cal_Test_Struct.ct_one_byte_test_4_len_array_i8_2d[first_lvl_iter][second_lvl_iter],
                   Cal_Test_Reference_Struct.ct_one_byte_test_4_len_array_i8_2d[CT_ARRAY_FOUR_LEN_REVERSE_ITER - first_lvl_iter]
                                                                               [CT_ARRAY_FOUR_LEN_REVERSE_ITER - second_lvl_iter]);
      }
   }
}

/**
 * Test array reversing function, Here, a four-element int16_t 2D array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Four_Len_two_byte_i16_2d)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_two_byte_test_4_len_array_i16_2d[0][0],
                    sizeof(Cal_Test_Struct.ct_two_byte_test_4_len_array_i16_2d), CT_TWO_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t first_lvl_iter = 0; first_lvl_iter < CT_ARRAY_THREE_LEN; ++first_lvl_iter)
   {
      for (uint32_t second_lvl_iter = 0; second_lvl_iter < CT_ARRAY_FOUR_LEN; ++second_lvl_iter)
      {
         EXPECT_EQ(Cal_Test_Struct.ct_two_byte_test_4_len_array_i16_2d[first_lvl_iter][second_lvl_iter],
                   Cal_Test_Reference_Struct.ct_two_byte_test_4_len_array_i16_2d[CT_ARRAY_FOUR_LEN_REVERSE_ITER - first_lvl_iter]
                                                                                [CT_ARRAY_FOUR_LEN_REVERSE_ITER - second_lvl_iter]);
      }
   }
}

/**
 * Test array reversing function, Here, a four-element int32_t 2D array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Four_Len_four_byte_i32_2d)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_four_byte_test_4_len_array_i32_2d[0][0],
                    sizeof(Cal_Test_Struct.ct_four_byte_test_4_len_array_i32_2d), CT_FOUR_BYTE);
   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t first_lvl_iter = 0; first_lvl_iter < CT_ARRAY_THREE_LEN; ++first_lvl_iter)
   {
      for (uint32_t second_lvl_iter = 0; second_lvl_iter < CT_ARRAY_FOUR_LEN; ++second_lvl_iter)
      {
         EXPECT_EQ(Cal_Test_Struct.ct_four_byte_test_4_len_array_i32_2d[first_lvl_iter][second_lvl_iter],
                   Cal_Test_Reference_Struct.ct_four_byte_test_4_len_array_i32_2d[CT_ARRAY_FOUR_LEN_REVERSE_ITER - first_lvl_iter]
                                                                                 [CT_ARRAY_FOUR_LEN_REVERSE_ITER - second_lvl_iter]);
      }
   }
}

/**
 * Test array reversing function, Here, a four-element float32_T 2D array is passed as an argument.
 * \uts{} \sdd{} \testtype{positive}
 */
TEST_F(Ct_Reverse_Array_Test, Ct_Reverse_Array_Test_Four_Len_four_byte_f_2d)
{
   /** \arrange Data for Cal_Test_Struct and Cal_Test_Reference_Struct are prepared in Ct_Reverse_Array_Test fixture classes setup
    * phase, in file ct_reverse_array_test.hpp */

   /** \action Reverse Cal_Test_Struct array */
   Ct_Reverse_Array((uint8_t*) &Cal_Test_Struct.ct_four_byte_test_4_len_array_f_2d[0][0],
                    sizeof(Cal_Test_Struct.ct_four_byte_test_4_len_array_f_2d), CT_FOUR_BYTE);

   /** \assert The values in the arrays Cal Test Struct and Cal Test Reference Struct are the same in reverse order. */
   for (uint32_t first_lvl_iter = 0; first_lvl_iter < CT_ARRAY_THREE_LEN; ++first_lvl_iter)
   {
      for (uint32_t second_lvl_iter = 0; second_lvl_iter < CT_ARRAY_FOUR_LEN; ++second_lvl_iter)
      {
         EXPECT_EQ(Cal_Test_Struct.ct_four_byte_test_4_len_array_f_2d[first_lvl_iter][second_lvl_iter],
                   Cal_Test_Reference_Struct.ct_four_byte_test_4_len_array_f_2d[CT_ARRAY_FOUR_LEN_REVERSE_ITER - first_lvl_iter]
                                                                               [CT_ARRAY_FOUR_LEN_REVERSE_ITER - second_lvl_iter]);
      }
   }
}