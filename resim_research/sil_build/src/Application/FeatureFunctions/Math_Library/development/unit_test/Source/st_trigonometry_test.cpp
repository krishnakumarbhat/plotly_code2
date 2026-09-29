/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <gtest/gtest.h>
#include "fast_math_serialization_fixture.hpp"

#include "reuse.h"
#include "ml_bool.h"
#include "ml_exp.h"
#include "ml_macros.h"
#include "ml_math.h"
#include "ml_trigonometry.h"

/**
 * General setup for testing fast math functions:
 * - Provide a lazy access to NaN
 * - Initialize math tables if needed
 */
class FastMathFixture :
  public ::testing::Test
{
protected:
   FastMathFixture():
      m_invalid_val {std::numeric_limits<float32_T>::quiet_NaN()} /**< invalid value */
   {};
  float32_T m_invalid_val;   /**< invalid value */
};

/**
 * FastMathFixture tests that run from -4*pi till 4*pi
 */
class FastMathParam8piFixture : public FastMathFixture,
  public ::testing::WithParamInterface<float>
{};

#define EXPECTED_ACCURACY_FAST_MATH (0.005f)

/**
* Testing NaN as parameter
* \sdd{WI-14657}
*/
TEST_F(FastMathFixture, WI_15259_Fast_Cos_test_nan)
{
    // Action
    const float32_T invalid_val = Fast_Cos(m_invalid_val);
    EXPECT_NE(invalid_val, m_invalid_val);
}

/**
* FastMathFixture tests that run from -4*pi till 4*pi
*/
INSTANTIATE_TEST_SUITE_P(values8pi,
  FastMathParam8piFixture,
    testing::Range(-(4.0f * PI), (4.0f * PI), 0.001f*PI) /* Input values to be tested  */
);

/**
* Comparing the result of the fast FUT with the matching math.h function for a range of values
* \sdd{WI-14657}
*/
TEST_P(FastMathParam8piFixture, WI_15260_Fast_Cos_test)
{
  float32_T test_value = GetParam();
  float32_T fast_result = Fast_Cos(test_value);
  EXPECT_NEAR(fast_result, cosf(test_value), EXPECTED_ACCURACY_FAST_MATH);
}

/**
* Testing NaN as parameter
* \sdd{WI-14664}
*/
TEST_F(FastMathFixture, WI_15261_Fast_Sin_test_nan)
{
  // Action
  const float32_T invalid_val = Fast_Sin(m_invalid_val);
  // Run the evaluation
  EXPECT_NE(invalid_val, m_invalid_val);
}

/**
* Comparing the result of the fast FUT with the matching math.h function for a range of values
* \sdd{WI-14664}
*/
TEST_P(FastMathParam8piFixture, WI_15262_Fast_Sin_test)
{
  float32_T test_value = GetParam();
  float32_T fast_result = Fast_Sin(test_value);
  EXPECT_NEAR(fast_result, sinf(test_value), EXPECTED_ACCURACY_FAST_MATH);
}


/**
* Testing NaN as parameter
* \sdd{WI-14659}
*/
TEST_F(FastMathFixture, WI_15263_Fast_Atan2_test_nan0)
{
  // Action
  const float32_T invalid_val = Fast_Atan2(m_invalid_val, 1.0f);
  // Run the evaluation
  EXPECT_NE(invalid_val, m_invalid_val);
}

/**
* Testing NaN as parameter
* \sdd{WI-14659}
*/
TEST_F(FastMathFixture, WI_15264_Fast_Atan2_test_nan1)
{
  // Action
  const float32_T invalid_val = Fast_Atan2(1.0f, m_invalid_val);
  // Run the evaluation
  EXPECT_NE(invalid_val, m_invalid_val);
}

/**
* Testing NaN as parameter
* \sdd{WI-14659}
*/
TEST_F(FastMathFixture, WI_15265_Fast_Atan2_test_nan2)
{
  // Action
  const float32_T invalid_val = Fast_Atan2(m_invalid_val, m_invalid_val);
  // Run the evaluation
  EXPECT_NE(invalid_val, m_invalid_val);
}

/**
* FastMathFixture tests for the atan function
*/
class FastMathParamAtanFixture : public FastMathFixture,
   public ::testing::WithParamInterface<std::tuple<float, float, float>>
{};

/**
* FastMathFixture tests for the atan function
*/
INSTANTIATE_TEST_SUITE_P(values,
  FastMathParamAtanFixture,
   testing::Combine(
      testing::Range(-PI, PI, 0.1f * PI), /* x direction */
      testing::Range(-PI, PI, 0.1f * PI), /* y direction */
      testing::Values(1.0f, 10.0f)          /* scale       */
   )
);

/**
* Comparing the result of the fast FUT with the matching math.h function for a range of values
* \sdd{WI-14659}
*/
TEST_P(FastMathParamAtanFixture, WI_15266_Fast_Atan2_test)
{
  float32_T param_scale = std::get<2>(GetParam());
  float32_T test_sin_value = sinf(std::get<0>(GetParam())) * param_scale;
  float32_T test_cos_value = cosf(std::get<1>(GetParam())) * param_scale;
  float32_T fast_result = Fast_Atan2(test_sin_value, test_cos_value);
  float32_T expected_result = atan2f(test_sin_value, test_cos_value);
  EXPECT_NEAR(fast_result, expected_result, EXPECTED_ACCURACY_FAST_MATH);
}

/**
* testing atan2 for x being exactly zero
* \sdd{WI-14659}
*/
TEST(FastMathParamAtanFixture, WI_15267_Fast_Atan2_test_x_zero_y_pos)
{
   float32_T test_sin_value = 12.0f;
   float32_T test_cos_value = 0.0f;
   float32_T fast_result = Fast_Atan2(test_sin_value, test_cos_value);
   float32_T expected_result = atan2f(test_sin_value, test_cos_value);
   EXPECT_NEAR(fast_result, expected_result, EXPECTED_ACCURACY_FAST_MATH);
}

/**
* testing atan2 for x being exactly zero
* \sdd{WI-14659}
*/
TEST(FastMathParamAtanFixture, WI_15268_Fast_Atan2_test_x_zero_y_neg)
{
   float32_T test_sin_value = -12.0f;
   float32_T test_cos_value = 0.0f;
   float32_T fast_result = Fast_Atan2(test_sin_value, test_cos_value);
   float32_T expected_result = atan2f(test_sin_value, test_cos_value);
   EXPECT_NEAR(fast_result, expected_result, EXPECTED_ACCURACY_FAST_MATH);
}

/**
* Comparing the result of the fast FUT with the matching math.h function for a range of values
* \sdd{WI-14659}
*/
TEST_P(FastMathParamAtanFixture, WI_15269_Fast_Atan_test)
{
   float32_T param_scale = std::get<2>(GetParam());
   float32_T test_sin_value = sinf(std::get<0>(GetParam())) * param_scale;
   float32_T test_cos_value = cosf(std::get<1>(GetParam())) * param_scale;
   float32_T test_value = test_sin_value / test_cos_value;
   float32_T fast_result = Fast_Atan(test_value);
   float32_T expected_result = atanf(test_value);
   EXPECT_NEAR(fast_result, expected_result, EXPECTED_ACCURACY_FAST_MATH);
}

/**
* Testing NaN as parameter
* \sdd{WI-14661}
*/
TEST_F(FastMathFixture, WI_15270_Fast_Atan_test_nan)
{
  // Action
  const float32_T invalid_val = Fast_Atan(m_invalid_val);
  // Run the evaluation
  EXPECT_NE(invalid_val, m_invalid_val);
}



/**
* Testing NaN as parameter
* \sdd{WI-14663}
*/
TEST_F(FastMathFixture, WI_15271_Fast_Asin_test_nan)
{
  // Action
  const float32_T invalid_val = Fast_Asin(m_invalid_val);
  // Run the evaluation
  EXPECT_NE(invalid_val, m_invalid_val);
}

/**
* FastMathFixture tests that run from -1 till 1
*/
class FastMathParamNegOneToOneFixture : public FastMathFixture,
  public ::testing::WithParamInterface<float>
{};

INSTANTIATE_TEST_SUITE_P(values_neg_1_to_1,
  FastMathParamNegOneToOneFixture,
  testing::Range(-1.0f, 1.0f, 0.001f) /* Input values to be tested  */
);

/**
* Comparing the result of the fast FUT with the matching math.h function for a range of values
* \sdd{WI-14663}
*/
TEST_P(FastMathParamNegOneToOneFixture, WI_15272_Fast_Asin_test)
{
  float32_T test_value = GetParam();
  float32_T fast_result = Fast_Asin(test_value);
  float32_T expected_result = asinf(test_value);
  EXPECT_NEAR(fast_result, expected_result, EXPECTED_ACCURACY_FAST_MATH);
}

/**
* Testing NaN as parameter
* \sdd{WI-14665}
*/
TEST_F(FastMathFixture, WI_15273_Fast_Acos_test_nan)
{
  // Action
  const float32_T invalid_val = Fast_Acos(m_invalid_val);
  // Run the evaluation
  EXPECT_NE(invalid_val, m_invalid_val);
}

/**
* Comparing the result of the fast FUT with the matching math.h function for a range of values
* \sdd{WI-14665}
*/
TEST_P(FastMathParamNegOneToOneFixture, WI_15274_Fast_Acos_test)
{
   float32_T test_value = GetParam();
   float32_T fast_result = Fast_Acos(test_value);
   float32_T expected_result = acosf(test_value);
   EXPECT_NEAR(fast_result, expected_result, EXPECTED_ACCURACY_FAST_MATH);
}

/**
* The fast acos function returns its parameter value if it is outside the defined range of acos.
* \sdd{WI-14665}
*/
TEST(FastMathParamNegOneToOneFixture, WI_15275_Fast_Acos_test_outside_defined_range_pos)
{
   float32_T fast_result = Fast_Acos(1.1f);
   /** \assert expect either NaN or the parameter being returned
     * The fast_math options return the parameter value, math.h returns NaN */
   EXPECT_TRUE((fast_result == 1.1f) || (fast_result != fast_result));
}

/**
* The fast acos function returns its parameter value if it is outside the defined range of acos.
* \sdd{WI-14665}
*/
TEST(FastMathParamNegOneToOneFixture, WI_15276_Fast_Acos_test_outside_defined_range_neg)
{
   float32_T fast_result = Fast_Acos(-1.1f);
   /** \assert expect either NaN or the parameter being returned
   * The fast_math options return the parameter value, math.h returns NaN */
   EXPECT_TRUE((fast_result == -1.1f) || (fast_result != fast_result));
}

/**
* FastMathFixture tests that run from -pi/2 till pi/2
* Actually we test only to 'close' to the maximum values. The error gets higher
* the closer to the max we test.
*/
class FastTanFixture : public FastMathFixture,
  public ::testing::WithParamInterface<float>
{};

INSTANTIATE_TEST_SUITE_P(plus_minus_pi_half,
  FastTanFixture,
  testing::Range((-PI / 2.0f)+ 0.001f*PI, (PI / 2.0f), 0.001f*PI) /* Input values to be tested. See UT Fast_Tan_test_minus_half_pi below. */
);

/**
* Testing NaN as parameter
* \sdd{WI-14662}
*/
TEST_F(FastMathFixture, WI_15277_Fast_Tan_test_nan)
{
  // Action
  const float32_T invalid_val = Fast_Tan(m_invalid_val);
  // Run the evaluation
  EXPECT_NE(invalid_val, m_invalid_val);
}

/**
* Comparing the result of the fast FUT with the matching math.h function for a range of values
* \sdd{WI-14662}
*/
TEST_P(FastTanFixture, WI_15278_Fast_Tan_test)
{
  float32_T test_value = GetParam();
  float32_T fast_result = Fast_Tan(test_value);
  float32_T expected_result = tanf(test_value);
  /* since the tangent gets quite huge a margin is calculated for the expected accuracy. */
  float32_T allowed_margin = fabsf(expected_result * EXPECTED_ACCURACY_FAST_MATH);
  /* Enure the allowed accuracy is never below EXPECTED_ACCURACY_FAST_MATH */
  allowed_margin = Max(allowed_margin, EXPECTED_ACCURACY_FAST_MATH);

  /* for high expected results allow a bigger margin */
  if (fabsf(expected_result) > 30.0f)
  {
     allowed_margin *= 2.0f;
  }
  if (fabsf(expected_result) > 100.0f)
  {
     allowed_margin *= 2.0f;
  }
  if (fabsf(expected_result) > 159.0f)
  {
     allowed_margin *= 2.0f;
  }
  EXPECT_NEAR(fast_result, expected_result, allowed_margin);
}

/**
* The tangens of +-PI/2 is on the singularity point. It can be positive or negative, depending
* on the method of calculation. Therefore we do not compare it to the outcome of a different
* implementation but expect the result to be far away from zero.
* \sdd{WI-14662}
*/
TEST(FastTanFixture, WI_15279_Fast_Tan_test_minus_half_pi)
{
   float32_T fast_result = Fast_Tan((-PI / 2.0f));
   EXPECT_GT(fabsf(fast_result), 1000000.0f);
}

/**
* The tangens of +-PI/2 is on the singularity point. It can be positive or negative, depending
* on the method of calculation. Therefore we do not compare it to the outcome of a different
* implementation but expect the result to be far away from zero.
* \sdd{WI-14662}
*/
TEST(FastTanFixture, WI_15280_Fast_Tan_test__half_pi)
{
   float32_T fast_result = Fast_Tan((PI / 2.0f));
   EXPECT_GT(fabsf(fast_result), 1000000.0f);
}


/** Return code when called with a NULL pointer */
TEST_F(FastMathSerializationFixture, serialization_exp_null)
{
#if ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
   EXPECT_EQ(SHARED_TOOLBOX_SRL_ERR_PTR, Serialize_Exp_Table(NULL));
#else
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Exp_Table(NULL));
#endif
}

/** Return code when called with buffer thats too small */
TEST_F(FastMathSerializationFixture, serialization_exp_buffer_too_small)
{
   buffer.length = 1;
#if ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
   EXPECT_EQ(SHARED_TOOLBOX_SRL_ERR_NOMEM, Serialize_Exp_Table(&buffer));
#else
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Exp_Table(&buffer));
#endif
}

/** Run successfully */
TEST_F(FastMathSerializationFixture, serialization_exp_success)
{
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Exp_Table(&buffer));
}

/** Return code when called with a NULL pointer */
TEST_F(FastMathSerializationFixture, deserialization_exp_null)
{
#if ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
   EXPECT_EQ(SHARED_TOOLBOX_SRL_ERR_PTR, Deserialize_Exp_Table(NULL));
#else
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Exp_Table(NULL));
#endif
}

/** Return code when called with buffer thats too small */
TEST_F(FastMathSerializationFixture, deserialization_exp_buffer_too_small)
{
   buffer.length = 1;
#if ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
   EXPECT_EQ(SHARED_TOOLBOX_SRL_ERR_NOMEM, Deserialize_Exp_Table(&buffer));
#else
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Exp_Table(&buffer));
#endif
}

/** Called with empty buffer */
TEST_F(FastMathSerializationFixture, deserialization_exp__err_parse)
{
#if ML_MATH_LIBRARY_FAST_MATH_EXP_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
   EXPECT_EQ(SHARED_TOOLBOX_SRL_ERR_PARSE, Deserialize_Exp_Table(&buffer));
#else
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Exp_Table(&buffer));
#endif
}


/** Return code when called with a NULL pointer */
TEST_F(FastMathSerializationFixture, serialization_trig_null)
{
#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
   EXPECT_EQ(SHARED_TOOLBOX_SRL_ERR_PTR, Serialize_Trig_Table(NULL));
#else
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Trig_Table(NULL));
#endif
}

/** Return code when called with buffer thats too small */
TEST_F(FastMathSerializationFixture, serialization_trig_buffer_too_small)
{
   buffer.length = 1;
#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
   EXPECT_EQ(SHARED_TOOLBOX_SRL_ERR_NOMEM, Serialize_Trig_Table(&buffer));
#else
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Trig_Table(&buffer));
#endif
}

/** Run successfully */
TEST_F(FastMathSerializationFixture, serialization_trig_success)
{
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Trig_Table(&buffer));
}

/** Return code when called with a NULL pointer */
TEST_F(FastMathSerializationFixture, deserialization_trig_null)
{
#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
   EXPECT_EQ(SHARED_TOOLBOX_SRL_ERR_PTR, Deserialize_Trig_Table(NULL));
#else
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Trig_Table(NULL));
#endif
}

/** Return code when called with buffer thats too small */
TEST_F(FastMathSerializationFixture, deserialization_trig_buffer_too_small)
{
   buffer.length = 1;
#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
   EXPECT_EQ(SHARED_TOOLBOX_SRL_ERR_NOMEM, Deserialize_Trig_Table(&buffer));
#else
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Trig_Table(&buffer));
#endif
}

/** Called with empty buffer */
TEST_F(FastMathSerializationFixture, deserialization_trig_err_parse)
{
#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
   EXPECT_EQ(SHARED_TOOLBOX_SRL_ERR_PARSE, Deserialize_Trig_Table(&buffer));
#else
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Trig_Table(&buffer));
#endif
}

/** Serializing and deserializing work*/
TEST_F(FastMathSerializationFixture, deserialization_trig_round_trip)
{
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Trig_Table(&buffer));

   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Trig_Table(&buffer));

   EXPECT_NEAR(Fast_Cos(PI/4.f), cosf(PI / 4.f), EXPECTED_ACCURACY_FAST_MATH);
}

/** Serializing and deserializing if endianness is swapped */
TEST_F(FastMathSerializationFixture, deserialization_trig_round_trip_swapped_endianness)
{
   float test_val = PI / 4.f - 0.01f;
   float result_before = Fast_Cos(test_val);
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Trig_Table(&buffer));
   Swap_Endianness(&buffer);
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Trig_Table(&buffer));
   float result_after = Fast_Cos(test_val);

   EXPECT_EQ(result_before, result_after);
}

/** Error code for unknown endianness */
TEST_F(FastMathSerializationFixture, deserialization_trig_round_trip_broken_endianness)
{
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Trig_Table(&buffer));
   buffer.p_data[0] = 0xb;
   buffer.p_data[1] = 0xa;
   buffer.p_data[2] = 0xc;
   buffer.p_data[3] = 0xd;
#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
   EXPECT_EQ(SHARED_TOOLBOX_SRL_ERR_PARSE, Deserialize_Trig_Table(&buffer));
#else
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Trig_Table(&buffer));
#endif
}

#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
/** Deserialization for a buffer filled with all zeros
* This test proofs that deserialization is actually done */
TEST_F(FastMathSerializationFixture, deserialization_trig_round_trip_broken_buffer)
{
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Trig_Table(&buffer));

   memset(buffer.p_data + (3 * sizeof(uint32_t)), 0x00, buffer.length - (3 * sizeof(uint32_t)));

   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Trig_Table(&buffer));
   float32_T ret;
   /** Expect an assertion for unset CS table to fire on a call to Fast_Cos
   * Since the CS table now contains 0.0f only.
   * If deserialization would not be done this assertion would not fire
   * because the table still would contain correct values. */
   EXPECT_DEBUG_DEATH(ret = Fast_Cos(PI / 4.f), "CS_TABLE.0. == 1.0f");
#ifdef NDEBUG
   EXPECT_EQ(ret, 0.f);
#endif
}
#endif

#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
/** Deserialization with a buffer with broken cos table size */
TEST_F(FastMathSerializationFixture, deserialization_trig_wrong_cos_table_size)
{
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Trig_Table(&buffer));

   memset(buffer.p_data + sizeof(uint32_t), 0x00, sizeof(uint32_t));

   EXPECT_EQ(SHARED_TOOLBOX_SRL_ERR_PARSE, Deserialize_Trig_Table(&buffer));
}
#endif

#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
/** Deserialization with a buffer with broken cos table size */
TEST_F(FastMathSerializationFixture, deserialization_trig_wrong_cos_table_size_swapped_endianness)
{
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Trig_Table(&buffer));
   Swap_Endianness(&buffer);

   memset(buffer.p_data + sizeof(uint32_t), 0x00, sizeof(uint32_t));

   EXPECT_EQ(SHARED_TOOLBOX_SRL_ERR_PARSE, Deserialize_Trig_Table(&buffer));
}
#endif

#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
/** Deserialization with a buffer with broken atan table size */
TEST_F(FastMathSerializationFixture, deserialization_trig_wrong_atan_table_size)
{
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Trig_Table(&buffer));

   memset(buffer.p_data + (2 * sizeof(uint32_t)), 0x00, sizeof(uint32_t));

   EXPECT_EQ(SHARED_TOOLBOX_SRL_ERR_PARSE, Deserialize_Trig_Table(&buffer));
}
#endif

#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
/** Deserialization with a buffer with broken atan table size */
TEST_F(FastMathSerializationFixture, deserialization_trig_wrong_atan_table_size_swapped_endianness)
{
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Trig_Table(&buffer));
   Swap_Endianness(&buffer);
   memset(buffer.p_data + (2 * sizeof(uint32_t)), 0x00, sizeof(uint32_t));

   EXPECT_EQ(SHARED_TOOLBOX_SRL_ERR_PARSE, Deserialize_Trig_Table(&buffer));
}
#endif

#if ML_MATH_LIBRARY_FAST_MATH_TRIG_TABLE == ML_MATH_LIBRARY_FAST_MATH_USE_RUNTIME_TABLE
/** Deserialization of a 'good' buffer into non initialized tables
* This test proves that serialization is actually done */
TEST_F(FastMathSerializationFixture, deserialization_trig_round_trip_broken_buffer_repaired)
{
   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Trig_Table(&buffer));

   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Serialize_Trig_Table(&buffer2));

   memset(buffer.p_data + (3 * sizeof(uint32_t)), 0x00, buffer.length - (3 * sizeof(uint32_t)));

   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Trig_Table(&buffer));

   EXPECT_EQ(SHARED_TOOLBOX_SRL_SUCCESS, Deserialize_Trig_Table(&buffer2));

   EXPECT_GE(Fast_Cos(PI / 4.f), 0.f);
}
#endif