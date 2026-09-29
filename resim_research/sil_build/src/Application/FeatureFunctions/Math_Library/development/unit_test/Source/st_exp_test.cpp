/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <gtest/gtest.h>
#include "fast_math_serialization_fixture.hpp"

#include "reuse.h"
#include "ml_exp.h"
#include "ml_macros.h"
#include "ml_math.h"

/**
* Fraction of the 'true' result to allow the FUT to differ.
* Since the exp function gets huge quite fast we cannot expect a constant accuracy, the
* higher the result the more off the Fast_Exp will be.
*/
#define EXPECTED_ACCURACY_FAST_MATH_EXP (0.05f)


/**
* tests for the exp function
*/
class FastMathParamExpFixture : public ::testing::TestWithParam<float>
{};

INSTANTIATE_TEST_SUITE_P(values_exp,
   FastMathParamExpFixture,
   testing::Range((LOWEST_DOMAIN_FLOAT - 10), HIGHEST_DOMAIN_FLOAT + 10, (1.0f / EXP_INV_PREC)) /* Input values to be tested  */
);

/**
* Checks the fast exp function from fastMath.
* Runs the FUT with values from below to above the numbers specified in the table
* \sdd{WI-14675}
*/
TEST_P(FastMathParamExpFixture, WI_14874_Fast_Exp_test)
{
   float32_T test_value = GetParam();
   /** \action call function under test */
   float32_T fast_result = Fast_Exp(test_value);
   /** \arrange calculate the allowed error marging */
   float32_T margin_allowed = expf(test_value) * EXPECTED_ACCURACY_FAST_MATH_EXP;
   margin_allowed = Max(margin_allowed, EXPECTED_ACCURACY_FAST_MATH_EXP);

   /** \assert Result is within allowed marging */
   float difference = fabsf(fast_result - expf(test_value));
   EXPECT_LE(difference, margin_allowed);
}

/**
* Checks the fast exp function from fastMath.
* Run the FUT with the parameter NaN
* \sdd{WI-14675}
*/
TEST(StExpTest, WI_14875_Fast_Exp_test_nan)
{
   /** \action call function under test */
   const float32_T invalid_val = Fast_Exp(std::numeric_limits<float32_T>::quiet_NaN());

   /** \assert the result is NaN */
   EXPECT_NE(invalid_val, invalid_val);
}


/** Serializing and deserializing work*/
TEST_F(FastMathSerializationFixture, deserialization_exp_round_trip)
{
   /** \action call function under test */
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Exp_Table(&buffer));
   /** \action call function under test */
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Exp_Table(&buffer));

   /** \assert calculating Fast Exp() leads to correct result */
   EXPECT_NEAR(Fast_Exp(10), expf(10), 0.005f);
}

/** Serializing and deserializing if endianness is swapped */
TEST_F(FastMathSerializationFixture, deserialization_exp_round_trip_swapped_endianness)
{
   /** \arrange Get a result from Fast_Exp() */
   float result_before = Fast_Exp(HIGHEST_DOMAIN_FLOAT - 1.f);

   /** \arrange serialize the exp table */
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Exp_Table(&buffer));

   /** \arrange swap the endianess of the serialized buffer */
   Swap_Endianness(&buffer);

   /** \action call function under test */
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Exp_Table(&buffer));

   /** \arrange Get a result from Fast_Exp() */
   float result_after = Fast_Exp(HIGHEST_DOMAIN_FLOAT - 1.f);

   /** \assert both Fast_Exp() results match */
   EXPECT_EQ(result_before, result_after);
}


/** Error code for unknown endianness */
TEST_F(FastMathSerializationFixture, deserialization_exp_round_trip_broken_endianness)
{
   /** \arrange serialize the exp table */
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Exp_Table(&buffer));
   /** \arrange randomize the magic bytes */
   buffer.p_data[0] = 0xb;
   buffer.p_data[1] = 0xa;
   buffer.p_data[2] = 0xc;
   buffer.p_data[3] = 0xd;
   /** \action call function under test */
#if ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
   EXPECT_EQ(SHARED_TOOLBOX_SRL_ERR_PARSE, Deserialize_Exp_Table(&buffer));
#else
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Exp_Table(&buffer));
#endif
}

/** Error code for wrong table size */
TEST_F(FastMathSerializationFixture, deserialization_exp_round_trip_wrong_table_size)
{
   /** \arrange serialize the exp table */
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Exp_Table(&buffer));
   /** \arange set the buffer size to a too small value **/
   buffer.p_data[4] = 0xb;
   /** \action call function under test */
   /** \assert Result is error code */
#if ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
   EXPECT_EQ(SHARED_TOOLBOX_SRL_ERR_PARSE, Deserialize_Exp_Table(&buffer));
#else
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Exp_Table(&buffer));
#endif
}

/** Serializing and deserializing if endianness is swapped */
TEST_F(FastMathSerializationFixture, deserialization_exp_round_trip_wrong_table_size_swapped_endianness)
{
   /** \arrange serialize the exp table */
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Exp_Table(&buffer));
   /** \arrange swap the endianess of the serialized buffer */
   Swap_Endianness(&buffer);
   /** \arange set the buffer size to a too small value **/
   buffer.p_data[4] = 0xb;
   /** \action call function under test */
   /** \assert Result is error code */
#if ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
   EXPECT_EQ(SHARED_TOOLBOX_SRL_ERR_PARSE, Deserialize_Exp_Table(&buffer));
#else
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Exp_Table(&buffer));
#endif
}

#if ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
/** Deserialization for a buffer filled with all zeros
* This test proofs that deserialization is actually done */
TEST_F(FastMathSerializationFixture, deserialization_exp_round_trip_broken_buffer)
{
   /** \arrange serialize the exp table */
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Exp_Table(&buffer));
   /** \arrange fill the data part of the buffer with 0x00 */
   memset(buffer.p_data + (2 * sizeof(uint32_t)), 0x00, buffer.length - (2 * sizeof(uint32_t)));
   /** \action call function under test */
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Exp_Table(&buffer));
   float32_T ret;
   /** Expect an assertion for unset EXP table to fire on a call to Fast_Exp
   * Since the EXP table now contains 0.0f only.
   * If deserialization would not be done this assertion would not fire
   * because the table still would contain correct values.*/
   EXPECT_DEBUG_DEATH(ret = Fast_Exp(5), "EXP_TABLE.0. > 0.f");
#ifdef NDEBUG
   EXPECT_EQ(ret, 0.f);
#endif
}
#endif

#if ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
/** Deserialization of a 'good' buffer into non initialized tables
* This test proves that serialization is actually done */
TEST_F(FastMathSerializationFixture, deserialization_exp_round_trip_broken_buffer_repaired)
{
   /** \arrange serialize the exp table */
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Exp_Table(&buffer));
   /** \arrange serialize the exp table into a second buffer*/
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Exp_Table(&buffer2));
   /** \arrange fill the data part of the first buffer with 0x00 */
   memset(buffer.p_data + (2 * sizeof(uint32_t)), 0x00, buffer.length - (2 * sizeof(uint32_t)));
   /** \action call function under test with first (broken) buffer */
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Exp_Table(&buffer));
   /** \action call function under test with second (healthy) buffer*/
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Exp_Table(&buffer2));
   /** \assert Fast_Exp() gives correct result */
   EXPECT_GE(Fast_Exp(5), 0.f);
}
#endif
