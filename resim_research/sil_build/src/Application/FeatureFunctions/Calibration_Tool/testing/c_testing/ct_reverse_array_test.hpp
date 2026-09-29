#ifndef CT_REVERSE_ARRAY_TEST_HPP
#define CT_REVERSE_ARRAY_TEST_HPP

/**
 * @file ct_reverse_array_test.hpp
 * @author SFL (Side Feature Logic) scrum team
 * @brief Test class Ct_Reverse_Array_Test
 *
 * @copyright Copyright (C) 2023 Aptiv. All rights reserved.
 *
 */

#include "gtest/gtest.h"
#include "gtest/gtest_pred_impl.h"

extern "C"
{
#include "reuse.h"
}

/* Macros for setting up different array sizes and iterating them in reverse order */
#define CT_ARRAY_TWO_LEN (2u)
#define CT_ARRAY_THREE_LEN (3u)
#define CT_ARRAY_FOUR_LEN (4u)
#define CT_ARRAY_TWO_LEN_REVERSE_ITER (1u)
#define CT_ARRAY_THREE_LEN_REVERSE_ITER (2u)
#define CT_ARRAY_FOUR_LEN_REVERSE_ITER (3u)


typedef struct
{
   uint8_t ct_one_byte_test_2_len_array_ui8[CT_ARRAY_TWO_LEN];
   uint16_t ct_two_byte_test_2_len_array_ui16[CT_ARRAY_TWO_LEN];
   uint32_t ct_four_byte_test_2_len_array_ui32[CT_ARRAY_TWO_LEN];
   int8_t ct_one_byte_test_2_len_array_i8[CT_ARRAY_TWO_LEN];
   int16_t ct_two_byte_test_2_len_array_i16[CT_ARRAY_TWO_LEN];
   int32_t ct_four_byte_test_2_len_array_i32[CT_ARRAY_TWO_LEN];
   float32_T ct_four_byte_test_2_len_array_f[CT_ARRAY_TWO_LEN];
   uint8_t ct_one_byte_test_3_len_array_ui8[CT_ARRAY_THREE_LEN];
   uint16_t ct_two_byte_test_3_len_array_ui16[CT_ARRAY_THREE_LEN];
   uint32_t ct_four_byte_test_3_len_array_ui32[CT_ARRAY_THREE_LEN];
   int8_t ct_one_byte_test_3_len_array_i8[CT_ARRAY_THREE_LEN];
   int16_t ct_two_byte_test_3_len_array_i16[CT_ARRAY_THREE_LEN];
   int32_t ct_four_byte_test_3_len_array_i32[CT_ARRAY_THREE_LEN];
   float32_T ct_four_byte_test_3_len_array_f[CT_ARRAY_THREE_LEN];
   uint8_t ct_one_byte_test_4_len_array_ui8[CT_ARRAY_FOUR_LEN];
   uint16_t ct_two_byte_test_4_len_array_ui16[CT_ARRAY_FOUR_LEN];
   uint32_t ct_four_byte_test_4_len_array_ui32[CT_ARRAY_FOUR_LEN];
   int8_t ct_one_byte_test_4_len_array_i8[CT_ARRAY_FOUR_LEN];
   int16_t ct_two_byte_test_4_len_array_i16[CT_ARRAY_FOUR_LEN];
   int32_t ct_four_byte_test_4_len_array_i32[CT_ARRAY_FOUR_LEN];
   float32_T ct_four_byte_test_4_len_array_f[CT_ARRAY_FOUR_LEN];

   uint8_t ct_one_byte_test_3_len_array_ui8_2d[CT_ARRAY_THREE_LEN][CT_ARRAY_THREE_LEN];
   uint16_t ct_two_byte_test_3_len_array_ui16_2d[CT_ARRAY_THREE_LEN][CT_ARRAY_THREE_LEN];
   uint32_t ct_four_byte_test_3_len_array_ui32_2d[CT_ARRAY_THREE_LEN][CT_ARRAY_THREE_LEN];
   int8_t ct_one_byte_test_3_len_array_i8_2d[CT_ARRAY_THREE_LEN][CT_ARRAY_THREE_LEN];
   int16_t ct_two_byte_test_3_len_array_i16_2d[CT_ARRAY_THREE_LEN][CT_ARRAY_THREE_LEN];
   int32_t ct_four_byte_test_3_len_array_i32_2d[CT_ARRAY_THREE_LEN][CT_ARRAY_THREE_LEN];
   float32_T ct_four_byte_test_3_len_array_f_2d[CT_ARRAY_THREE_LEN][CT_ARRAY_THREE_LEN];

   uint8_t ct_one_byte_test_4_len_array_ui8_2d[CT_ARRAY_FOUR_LEN][CT_ARRAY_FOUR_LEN];
   uint16_t ct_two_byte_test_4_len_array_ui16_2d[CT_ARRAY_FOUR_LEN][CT_ARRAY_FOUR_LEN];
   uint32_t ct_four_byte_test_4_len_array_ui32_2d[CT_ARRAY_FOUR_LEN][CT_ARRAY_FOUR_LEN];
   int8_t ct_one_byte_test_4_len_array_i8_2d[CT_ARRAY_FOUR_LEN][CT_ARRAY_FOUR_LEN];
   int16_t ct_two_byte_test_4_len_array_i16_2d[CT_ARRAY_FOUR_LEN][CT_ARRAY_FOUR_LEN];
   int32_t ct_four_byte_test_4_len_array_i32_2d[CT_ARRAY_FOUR_LEN][CT_ARRAY_FOUR_LEN];
   float32_T ct_four_byte_test_4_len_array_f_2d[CT_ARRAY_FOUR_LEN][CT_ARRAY_FOUR_LEN];

} Ct_Array_Testset_T;

/**
 * Class used to create a fixture for Ct_Reverse_Array function
 * For writing two or more tests that operate on similar data, it can use a test fixture
 * It allows to reuse the same configuration of objects for several different tests
 */
class Ct_Reverse_Array_Test : public ::testing::Test
{
 protected:
   Ct_Array_Testset_T Cal_Test_Struct;
   Ct_Array_Testset_T Cal_Test_Reference_Struct;

   void CT_Test_SetUp(Ct_Array_Testset_T *Test_Struct)
   {
      Test_Struct->ct_one_byte_test_2_len_array_ui8[0]         = 0u;
      Test_Struct->ct_one_byte_test_2_len_array_ui8[1]         = 1u;
      Test_Struct->ct_two_byte_test_2_len_array_ui16[0]        = 0u;
      Test_Struct->ct_two_byte_test_2_len_array_ui16[1]        = 1u;
      Test_Struct->ct_four_byte_test_2_len_array_ui32[0]       = 0u;
      Test_Struct->ct_four_byte_test_2_len_array_ui32[1]       = 1u;
      Test_Struct->ct_one_byte_test_2_len_array_i8[0]          = 0u;
      Test_Struct->ct_one_byte_test_2_len_array_i8[1]          = 1u;
      Test_Struct->ct_two_byte_test_2_len_array_i16[0]         = 0u;
      Test_Struct->ct_two_byte_test_2_len_array_i16[1]         = 1u;
      Test_Struct->ct_four_byte_test_2_len_array_i32[0]        = 0u;
      Test_Struct->ct_four_byte_test_2_len_array_i32[1]        = 1u;
      Test_Struct->ct_four_byte_test_2_len_array_f[0]          = 0.0f;
      Test_Struct->ct_four_byte_test_2_len_array_f[1]          = 1.0f;
      Test_Struct->ct_one_byte_test_3_len_array_ui8[0]         = 0u;
      Test_Struct->ct_one_byte_test_3_len_array_ui8[1]         = 1u;
      Test_Struct->ct_one_byte_test_3_len_array_ui8[2]         = 2u;
      Test_Struct->ct_two_byte_test_3_len_array_ui16[0]        = 0u;
      Test_Struct->ct_two_byte_test_3_len_array_ui16[1]        = 1u;
      Test_Struct->ct_two_byte_test_3_len_array_ui16[2]        = 2u;
      Test_Struct->ct_four_byte_test_3_len_array_ui32[0]       = 0u;
      Test_Struct->ct_four_byte_test_3_len_array_ui32[1]       = 1u;
      Test_Struct->ct_four_byte_test_3_len_array_ui32[2]       = 2u;
      Test_Struct->ct_one_byte_test_3_len_array_i8[0]          = 0u;
      Test_Struct->ct_one_byte_test_3_len_array_i8[1]          = 1u;
      Test_Struct->ct_one_byte_test_3_len_array_i8[2]          = 2u;
      Test_Struct->ct_two_byte_test_3_len_array_i16[0]         = 0u;
      Test_Struct->ct_two_byte_test_3_len_array_i16[1]         = 1u;
      Test_Struct->ct_two_byte_test_3_len_array_i16[2]         = 2u;
      Test_Struct->ct_four_byte_test_3_len_array_i32[0]        = 0u;
      Test_Struct->ct_four_byte_test_3_len_array_i32[1]        = 1u;
      Test_Struct->ct_four_byte_test_3_len_array_i32[2]        = 2u;
      Test_Struct->ct_four_byte_test_3_len_array_f[0]          = 0.0f;
      Test_Struct->ct_four_byte_test_3_len_array_f[1]          = 1.0f;
      Test_Struct->ct_four_byte_test_3_len_array_f[2]          = 2.0f;
      Test_Struct->ct_one_byte_test_4_len_array_ui8[0]         = 0u;
      Test_Struct->ct_one_byte_test_4_len_array_ui8[1]         = 1u;
      Test_Struct->ct_one_byte_test_4_len_array_ui8[2]         = 2u;
      Test_Struct->ct_one_byte_test_4_len_array_ui8[3]         = 3u;
      Test_Struct->ct_two_byte_test_4_len_array_ui16[0]        = 0u;
      Test_Struct->ct_two_byte_test_4_len_array_ui16[1]        = 1u;
      Test_Struct->ct_two_byte_test_4_len_array_ui16[2]        = 2u;
      Test_Struct->ct_two_byte_test_4_len_array_ui16[3]        = 3u;
      Test_Struct->ct_four_byte_test_4_len_array_ui32[0]       = 0u;
      Test_Struct->ct_four_byte_test_4_len_array_ui32[1]       = 1u;
      Test_Struct->ct_four_byte_test_4_len_array_ui32[2]       = 2u;
      Test_Struct->ct_four_byte_test_4_len_array_ui32[3]       = 3u;
      Test_Struct->ct_one_byte_test_4_len_array_i8[0]          = 0u;
      Test_Struct->ct_one_byte_test_4_len_array_i8[1]          = 1u;
      Test_Struct->ct_one_byte_test_4_len_array_i8[2]          = 2u;
      Test_Struct->ct_one_byte_test_4_len_array_i8[3]          = 3u;
      Test_Struct->ct_two_byte_test_4_len_array_i16[0]         = 0u;
      Test_Struct->ct_two_byte_test_4_len_array_i16[1]         = 1u;
      Test_Struct->ct_two_byte_test_4_len_array_i16[2]         = 2u;
      Test_Struct->ct_two_byte_test_4_len_array_i16[3]         = 3u;
      Test_Struct->ct_four_byte_test_4_len_array_i32[0]        = 0u;
      Test_Struct->ct_four_byte_test_4_len_array_i32[1]        = 1u;
      Test_Struct->ct_four_byte_test_4_len_array_i32[2]        = 2u;
      Test_Struct->ct_four_byte_test_4_len_array_i32[3]        = 3u;
      Test_Struct->ct_four_byte_test_4_len_array_f[0]          = 0.0f;
      Test_Struct->ct_four_byte_test_4_len_array_f[1]          = 1.0f;
      Test_Struct->ct_four_byte_test_4_len_array_f[2]          = 2.0f;
      Test_Struct->ct_four_byte_test_4_len_array_f[3]          = 3.0f;
      Test_Struct->ct_one_byte_test_3_len_array_ui8_2d[0][0]   = 0u;
      Test_Struct->ct_one_byte_test_3_len_array_ui8_2d[0][1]   = 1u;
      Test_Struct->ct_one_byte_test_3_len_array_ui8_2d[0][2]   = 2u;
      Test_Struct->ct_one_byte_test_3_len_array_ui8_2d[1][0]   = 3u;
      Test_Struct->ct_one_byte_test_3_len_array_ui8_2d[1][1]   = 4u;
      Test_Struct->ct_one_byte_test_3_len_array_ui8_2d[1][2]   = 5u;
      Test_Struct->ct_one_byte_test_3_len_array_ui8_2d[2][0]   = 6u;
      Test_Struct->ct_one_byte_test_3_len_array_ui8_2d[2][1]   = 7u;
      Test_Struct->ct_one_byte_test_3_len_array_ui8_2d[2][2]   = 8u;
      Test_Struct->ct_two_byte_test_3_len_array_ui16_2d[0][0]  = 0u;
      Test_Struct->ct_two_byte_test_3_len_array_ui16_2d[0][1]  = 1u;
      Test_Struct->ct_two_byte_test_3_len_array_ui16_2d[0][2]  = 2u;
      Test_Struct->ct_two_byte_test_3_len_array_ui16_2d[1][0]  = 3u;
      Test_Struct->ct_two_byte_test_3_len_array_ui16_2d[1][1]  = 4u;
      Test_Struct->ct_two_byte_test_3_len_array_ui16_2d[1][2]  = 5u;
      Test_Struct->ct_two_byte_test_3_len_array_ui16_2d[2][0]  = 6u;
      Test_Struct->ct_two_byte_test_3_len_array_ui16_2d[2][1]  = 7u;
      Test_Struct->ct_two_byte_test_3_len_array_ui16_2d[2][2]  = 8u;
      Test_Struct->ct_four_byte_test_3_len_array_ui32_2d[0][0] = 0u;
      Test_Struct->ct_four_byte_test_3_len_array_ui32_2d[0][1] = 1u;
      Test_Struct->ct_four_byte_test_3_len_array_ui32_2d[0][2] = 2u;
      Test_Struct->ct_four_byte_test_3_len_array_ui32_2d[1][0] = 3u;
      Test_Struct->ct_four_byte_test_3_len_array_ui32_2d[1][1] = 4u;
      Test_Struct->ct_four_byte_test_3_len_array_ui32_2d[1][2] = 5u;
      Test_Struct->ct_four_byte_test_3_len_array_ui32_2d[2][0] = 6u;
      Test_Struct->ct_four_byte_test_3_len_array_ui32_2d[2][1] = 7u;
      Test_Struct->ct_four_byte_test_3_len_array_ui32_2d[2][2] = 8u;
      Test_Struct->ct_one_byte_test_3_len_array_i8_2d[0][0]    = 0u;
      Test_Struct->ct_one_byte_test_3_len_array_i8_2d[0][1]    = 1u;
      Test_Struct->ct_one_byte_test_3_len_array_i8_2d[0][2]    = 2u;
      Test_Struct->ct_one_byte_test_3_len_array_i8_2d[1][0]    = 3u;
      Test_Struct->ct_one_byte_test_3_len_array_i8_2d[1][1]    = 4u;
      Test_Struct->ct_one_byte_test_3_len_array_i8_2d[1][2]    = 5u;
      Test_Struct->ct_one_byte_test_3_len_array_i8_2d[2][0]    = 6u;
      Test_Struct->ct_one_byte_test_3_len_array_i8_2d[2][1]    = 7u;
      Test_Struct->ct_one_byte_test_3_len_array_i8_2d[2][2]    = 8u;
      Test_Struct->ct_two_byte_test_3_len_array_i16_2d[0][0]   = 0u;
      Test_Struct->ct_two_byte_test_3_len_array_i16_2d[0][1]   = 1u;
      Test_Struct->ct_two_byte_test_3_len_array_i16_2d[0][2]   = 2u;
      Test_Struct->ct_two_byte_test_3_len_array_i16_2d[1][0]   = 3u;
      Test_Struct->ct_two_byte_test_3_len_array_i16_2d[1][1]   = 4u;
      Test_Struct->ct_two_byte_test_3_len_array_i16_2d[1][2]   = 5u;
      Test_Struct->ct_two_byte_test_3_len_array_i16_2d[2][0]   = 6u;
      Test_Struct->ct_two_byte_test_3_len_array_i16_2d[2][1]   = 7u;
      Test_Struct->ct_two_byte_test_3_len_array_i16_2d[2][2]   = 8u;
      Test_Struct->ct_four_byte_test_3_len_array_i32_2d[0][0]  = 0u;
      Test_Struct->ct_four_byte_test_3_len_array_i32_2d[0][1]  = 1u;
      Test_Struct->ct_four_byte_test_3_len_array_i32_2d[0][2]  = 2u;
      Test_Struct->ct_four_byte_test_3_len_array_i32_2d[1][0]  = 3u;
      Test_Struct->ct_four_byte_test_3_len_array_i32_2d[1][1]  = 4u;
      Test_Struct->ct_four_byte_test_3_len_array_i32_2d[1][2]  = 5u;
      Test_Struct->ct_four_byte_test_3_len_array_i32_2d[2][0]  = 6u;
      Test_Struct->ct_four_byte_test_3_len_array_i32_2d[2][1]  = 7u;
      Test_Struct->ct_four_byte_test_3_len_array_i32_2d[2][2]  = 8u;
      Test_Struct->ct_four_byte_test_3_len_array_f_2d[0][0]    = 0u;
      Test_Struct->ct_four_byte_test_3_len_array_f_2d[0][1]    = 1u;
      Test_Struct->ct_four_byte_test_3_len_array_f_2d[0][2]    = 2u;
      Test_Struct->ct_four_byte_test_3_len_array_f_2d[1][0]    = 3u;
      Test_Struct->ct_four_byte_test_3_len_array_f_2d[1][1]    = 4u;
      Test_Struct->ct_four_byte_test_3_len_array_f_2d[1][2]    = 5u;
      Test_Struct->ct_four_byte_test_3_len_array_f_2d[2][0]    = 6u;
      Test_Struct->ct_four_byte_test_3_len_array_f_2d[2][1]    = 7u;
      Test_Struct->ct_four_byte_test_3_len_array_f_2d[2][2]    = 8u;
      Test_Struct->ct_one_byte_test_4_len_array_ui8_2d[0][0]   = 0u;
      Test_Struct->ct_one_byte_test_4_len_array_ui8_2d[0][1]   = 1u;
      Test_Struct->ct_one_byte_test_4_len_array_ui8_2d[0][2]   = 2u;
      Test_Struct->ct_one_byte_test_4_len_array_ui8_2d[0][3]   = 3u;
      Test_Struct->ct_one_byte_test_4_len_array_ui8_2d[1][0]   = 4u;
      Test_Struct->ct_one_byte_test_4_len_array_ui8_2d[1][1]   = 5u;
      Test_Struct->ct_one_byte_test_4_len_array_ui8_2d[1][2]   = 6u;
      Test_Struct->ct_one_byte_test_4_len_array_ui8_2d[1][3]   = 7u;
      Test_Struct->ct_one_byte_test_4_len_array_ui8_2d[2][0]   = 8u;
      Test_Struct->ct_one_byte_test_4_len_array_ui8_2d[2][1]   = 9u;
      Test_Struct->ct_one_byte_test_4_len_array_ui8_2d[2][2]   = 10u;
      Test_Struct->ct_one_byte_test_4_len_array_ui8_2d[2][3]   = 11u;
      Test_Struct->ct_one_byte_test_4_len_array_ui8_2d[3][0]   = 12u;
      Test_Struct->ct_one_byte_test_4_len_array_ui8_2d[3][1]   = 13u;
      Test_Struct->ct_one_byte_test_4_len_array_ui8_2d[3][2]   = 14u;
      Test_Struct->ct_one_byte_test_4_len_array_ui8_2d[3][3]   = 15u;
      Test_Struct->ct_two_byte_test_4_len_array_ui16_2d[0][0]  = 0u;
      Test_Struct->ct_two_byte_test_4_len_array_ui16_2d[0][1]  = 1u;
      Test_Struct->ct_two_byte_test_4_len_array_ui16_2d[0][2]  = 2u;
      Test_Struct->ct_two_byte_test_4_len_array_ui16_2d[0][3]  = 3u;
      Test_Struct->ct_two_byte_test_4_len_array_ui16_2d[1][0]  = 4u;
      Test_Struct->ct_two_byte_test_4_len_array_ui16_2d[1][1]  = 5u;
      Test_Struct->ct_two_byte_test_4_len_array_ui16_2d[1][2]  = 6u;
      Test_Struct->ct_two_byte_test_4_len_array_ui16_2d[1][3]  = 7u;
      Test_Struct->ct_two_byte_test_4_len_array_ui16_2d[2][0]  = 8u;
      Test_Struct->ct_two_byte_test_4_len_array_ui16_2d[2][1]  = 9u;
      Test_Struct->ct_two_byte_test_4_len_array_ui16_2d[2][2]  = 10u;
      Test_Struct->ct_two_byte_test_4_len_array_ui16_2d[2][3]  = 11u;
      Test_Struct->ct_two_byte_test_4_len_array_ui16_2d[3][0]  = 12u;
      Test_Struct->ct_two_byte_test_4_len_array_ui16_2d[3][1]  = 13u;
      Test_Struct->ct_two_byte_test_4_len_array_ui16_2d[3][2]  = 14u;
      Test_Struct->ct_two_byte_test_4_len_array_ui16_2d[3][3]  = 15u;
      Test_Struct->ct_four_byte_test_4_len_array_ui32_2d[0][0] = 0u;
      Test_Struct->ct_four_byte_test_4_len_array_ui32_2d[0][1] = 1u;
      Test_Struct->ct_four_byte_test_4_len_array_ui32_2d[0][2] = 2u;
      Test_Struct->ct_four_byte_test_4_len_array_ui32_2d[0][3] = 3u;
      Test_Struct->ct_four_byte_test_4_len_array_ui32_2d[1][0] = 4u;
      Test_Struct->ct_four_byte_test_4_len_array_ui32_2d[1][1] = 5u;
      Test_Struct->ct_four_byte_test_4_len_array_ui32_2d[1][2] = 6u;
      Test_Struct->ct_four_byte_test_4_len_array_ui32_2d[1][3] = 7u;
      Test_Struct->ct_four_byte_test_4_len_array_ui32_2d[2][0] = 8u;
      Test_Struct->ct_four_byte_test_4_len_array_ui32_2d[2][1] = 9u;
      Test_Struct->ct_four_byte_test_4_len_array_ui32_2d[2][2] = 10u;
      Test_Struct->ct_four_byte_test_4_len_array_ui32_2d[2][3] = 11u;
      Test_Struct->ct_four_byte_test_4_len_array_ui32_2d[3][0] = 12u;
      Test_Struct->ct_four_byte_test_4_len_array_ui32_2d[3][1] = 13u;
      Test_Struct->ct_four_byte_test_4_len_array_ui32_2d[3][2] = 14u;
      Test_Struct->ct_four_byte_test_4_len_array_ui32_2d[3][3] = 15u;
      Test_Struct->ct_one_byte_test_4_len_array_i8_2d[0][0]    = 0u;
      Test_Struct->ct_one_byte_test_4_len_array_i8_2d[0][1]    = 1u;
      Test_Struct->ct_one_byte_test_4_len_array_i8_2d[0][2]    = 2u;
      Test_Struct->ct_one_byte_test_4_len_array_i8_2d[0][3]    = 3u;
      Test_Struct->ct_one_byte_test_4_len_array_i8_2d[1][0]    = 4u;
      Test_Struct->ct_one_byte_test_4_len_array_i8_2d[1][1]    = 5u;
      Test_Struct->ct_one_byte_test_4_len_array_i8_2d[1][2]    = 6u;
      Test_Struct->ct_one_byte_test_4_len_array_i8_2d[1][3]    = 7u;
      Test_Struct->ct_one_byte_test_4_len_array_i8_2d[2][0]    = 8u;
      Test_Struct->ct_one_byte_test_4_len_array_i8_2d[2][1]    = 9u;
      Test_Struct->ct_one_byte_test_4_len_array_i8_2d[2][2]    = 10u;
      Test_Struct->ct_one_byte_test_4_len_array_i8_2d[2][3]    = 11u;
      Test_Struct->ct_one_byte_test_4_len_array_i8_2d[3][0]    = 12u;
      Test_Struct->ct_one_byte_test_4_len_array_i8_2d[3][1]    = 13u;
      Test_Struct->ct_one_byte_test_4_len_array_i8_2d[3][2]    = 14u;
      Test_Struct->ct_one_byte_test_4_len_array_i8_2d[3][3]    = 15u;
      Test_Struct->ct_two_byte_test_4_len_array_i16_2d[0][0]   = 0u;
      Test_Struct->ct_two_byte_test_4_len_array_i16_2d[0][1]   = 1u;
      Test_Struct->ct_two_byte_test_4_len_array_i16_2d[0][2]   = 2u;
      Test_Struct->ct_two_byte_test_4_len_array_i16_2d[0][3]   = 3u;
      Test_Struct->ct_two_byte_test_4_len_array_i16_2d[1][0]   = 4u;
      Test_Struct->ct_two_byte_test_4_len_array_i16_2d[1][1]   = 5u;
      Test_Struct->ct_two_byte_test_4_len_array_i16_2d[1][2]   = 6u;
      Test_Struct->ct_two_byte_test_4_len_array_i16_2d[1][3]   = 7u;
      Test_Struct->ct_two_byte_test_4_len_array_i16_2d[2][0]   = 8u;
      Test_Struct->ct_two_byte_test_4_len_array_i16_2d[2][1]   = 9u;
      Test_Struct->ct_two_byte_test_4_len_array_i16_2d[2][2]   = 10u;
      Test_Struct->ct_two_byte_test_4_len_array_i16_2d[2][3]   = 11u;
      Test_Struct->ct_two_byte_test_4_len_array_i16_2d[3][0]   = 12u;
      Test_Struct->ct_two_byte_test_4_len_array_i16_2d[3][1]   = 13u;
      Test_Struct->ct_two_byte_test_4_len_array_i16_2d[3][2]   = 14u;
      Test_Struct->ct_two_byte_test_4_len_array_i16_2d[3][3]   = 15u;
      Test_Struct->ct_four_byte_test_4_len_array_i32_2d[0][0]  = 0u;
      Test_Struct->ct_four_byte_test_4_len_array_i32_2d[0][1]  = 1u;
      Test_Struct->ct_four_byte_test_4_len_array_i32_2d[0][2]  = 2u;
      Test_Struct->ct_four_byte_test_4_len_array_i32_2d[0][3]  = 3u;
      Test_Struct->ct_four_byte_test_4_len_array_i32_2d[1][0]  = 4u;
      Test_Struct->ct_four_byte_test_4_len_array_i32_2d[1][1]  = 5u;
      Test_Struct->ct_four_byte_test_4_len_array_i32_2d[1][2]  = 6u;
      Test_Struct->ct_four_byte_test_4_len_array_i32_2d[1][3]  = 7u;
      Test_Struct->ct_four_byte_test_4_len_array_i32_2d[2][0]  = 8u;
      Test_Struct->ct_four_byte_test_4_len_array_i32_2d[2][1]  = 9u;
      Test_Struct->ct_four_byte_test_4_len_array_i32_2d[2][2]  = 10u;
      Test_Struct->ct_four_byte_test_4_len_array_i32_2d[2][3]  = 11u;
      Test_Struct->ct_four_byte_test_4_len_array_i32_2d[3][0]  = 12u;
      Test_Struct->ct_four_byte_test_4_len_array_i32_2d[3][1]  = 13u;
      Test_Struct->ct_four_byte_test_4_len_array_i32_2d[3][2]  = 14u;
      Test_Struct->ct_four_byte_test_4_len_array_i32_2d[3][3]  = 15u;
      Test_Struct->ct_four_byte_test_4_len_array_f_2d[0][0]    = 0u;
      Test_Struct->ct_four_byte_test_4_len_array_f_2d[0][1]    = 1u;
      Test_Struct->ct_four_byte_test_4_len_array_f_2d[0][2]    = 2u;
      Test_Struct->ct_four_byte_test_4_len_array_f_2d[0][3]    = 3u;
      Test_Struct->ct_four_byte_test_4_len_array_f_2d[1][0]    = 4u;
      Test_Struct->ct_four_byte_test_4_len_array_f_2d[1][1]    = 5u;
      Test_Struct->ct_four_byte_test_4_len_array_f_2d[1][2]    = 6u;
      Test_Struct->ct_four_byte_test_4_len_array_f_2d[1][3]    = 7u;
      Test_Struct->ct_four_byte_test_4_len_array_f_2d[2][0]    = 8u;
      Test_Struct->ct_four_byte_test_4_len_array_f_2d[2][1]    = 9u;
      Test_Struct->ct_four_byte_test_4_len_array_f_2d[2][2]    = 10u;
      Test_Struct->ct_four_byte_test_4_len_array_f_2d[2][3]    = 11u;
      Test_Struct->ct_four_byte_test_4_len_array_f_2d[3][0]    = 12u;
      Test_Struct->ct_four_byte_test_4_len_array_f_2d[3][1]    = 13u;
      Test_Struct->ct_four_byte_test_4_len_array_f_2d[3][2]    = 14u;
      Test_Struct->ct_four_byte_test_4_len_array_f_2d[3][3]    = 15u;
   }


   virtual void SetUp()
   {
      CT_Test_SetUp(&Cal_Test_Struct);
      CT_Test_SetUp(&Cal_Test_Reference_Struct);
   }

   /**
    * Function used to release any resources allocated in SetUp()
    */
   virtual void TearDown()
   {
   }

 public:
};

#endif /* CT_REVERSE_ARRAY_TEST_HPP */
