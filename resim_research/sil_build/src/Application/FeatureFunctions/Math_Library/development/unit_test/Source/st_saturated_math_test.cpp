/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <gtest/gtest.h>
#include "ml_saturated_math.h"


/**
 * Sat_Inc_Uint8 Tests
 * \sdd{WI-13868}
 */
TEST(StSaturatedMathTest, WI_15190_Sat_Inc_Uint8_test__no_overflow_input_shall_be_increased)
{
   /** \arrange */
   uint8_t input = 100;

   /** \action call function under test */
   Sat_Inc_Uint8(&input);

   /** \assert */
   EXPECT_EQ(input, 101u);
}

/**
* Sat_Inc_Uint8 Tests
* \sdd{WI-13868}
*/
TEST(StSaturatedMathTest, WI_15191_Sat_Inc_Uint8_test__overflow_input_shall_be_saturated)
{
   /** \arrange */
   uint8_t input = UINT8_MAX;

   /** \action call function under test */
   Sat_Inc_Uint8(&input);

   /** \assert */
   EXPECT_EQ(input, UINT8_MAX);
}


/**
 * Sat_Inc_Int8 Tests
 * \sdd{WI-13877}
 */
TEST(StSaturatedMathTest, WI_15192_Sat_Inc_Int8_test__no_overflow_input_shall_be_increased)
{
   /** \arrange */
   int8_t input = 100;

   /** \action call function under test */
   Sat_Inc_Int8(&input);

   /** \assert */
   EXPECT_EQ(input, 101);
}

/**
* Sat_Inc_Int8 Tests
* \sdd{WI-13877}
*/
TEST(StSaturatedMathTest, WI_15193_Sat_Inc_Int8_test__overflow_input_shall_be_saturated)
{
   /** \arrange */
   int8_t input = INT8_MAX;

   /** \action call function under test */
   Sat_Inc_Int8(&input);

   /** \assert */
   EXPECT_EQ(input, INT8_MAX);
}


/**
 * Sat_Inc_Uint16 Tests
 * \sdd{WI-13867}
 */
TEST(StSaturatedMathTest, WI_15194_Sat_Inc_Uint16_test__no_overflow_input_shall_be_increased)
{
   /** \arrange */
   uint16_t input = 1000;

   /** \action call function under test */
   Sat_Inc_Uint16(&input);

   /** \assert */
   EXPECT_EQ(input, 1001u);
}

/**
* Sat_Inc_Uint16 Tests
* \sdd{WI-13867}
*/
TEST(StSaturatedMathTest, WI_15195_Sat_Inc_Uint16_test__overflow_input_shall_be_saturated)
{
   /** \arrange */
   uint16_t input = UINT16_MAX;

   /** \action call function under test */
   Sat_Inc_Uint16(&input);

   /** \assert */
   EXPECT_EQ(input, UINT16_MAX);
}


/**
 * Sat_Inc_Int16 Tests
 * \sdd{WI-13876}
 */
TEST(StSaturatedMathTest, WI_15196_Sat_Inc_Int16_test__no_overflow_input_shall_be_increased)
{
   /** \arrange */
   int16_t input = 10000;

   /** \action call function under test */
   Sat_Inc_Int16(&input);

   /** \assert */
   EXPECT_EQ(input, 10001);
}

/**
* Sat_Inc_Int16 Tests
* \sdd{WI-13876}
*/
TEST(StSaturatedMathTest, WI_15197_Sat_Inc_Int16_test__overflow_input_shall_be_saturated)
{
   /** \arrange */
   int16_t input = INT16_MAX;

   /** \action call function under test */
   Sat_Inc_Int16(&input);

   /** \assert */
   EXPECT_EQ(input, INT16_MAX);
}


/**
 * Sat_Inc_Uint32 Tests
 * \sdd{WI-13878}
 */
TEST(StSaturatedMathTest, WI_15198_Sat_Inc_Uint32_test__no_overflow_input_shall_be_increased)
{
   /** \arrange */
   uint32_t input = 100000;

   /** \action call function under test */
   Sat_Inc_Uint32(&input);

   /** \assert */
   EXPECT_EQ(input, 100001u);
}

/**
* Sat_Inc_Uint32 Tests
* \sdd{WI-13878}
*/
TEST(StSaturatedMathTest, WI_15199_Sat_Inc_Uint32_test__overflow_input_shall_be_saturated)
{
   /** \arrange */
   uint32_t input = UINT32_MAX;

   /** \action call function under test */
   Sat_Inc_Uint32(&input);

   /** \assert */
   EXPECT_EQ(input, UINT32_MAX);
}


/**
 * Sat_Inc_Int32 Tests
 * \sdd{WI-13874}
 */
TEST(StSaturatedMathTest, WI_15200_Sat_Inc_Int32_test__no_overflow_input_shall_be_increased)
{
   /** \arrange */
   int32_t input = 100000;

   /** \action call function under test */
   Sat_Inc_Int32(&input);

   /** \assert */
   EXPECT_EQ(input, 100001);
}

/**
* Sat_Inc_Int32 Tests
* \sdd{WI-13874}
*/
TEST(StSaturatedMathTest, WI_15201_Sat_Inc_Int32_test__overflow_input_shall_be_saturated)
{
   /** \arrange */
   int32_t input = INT32_MAX;

   /** \action call function under test */
   Sat_Inc_Int32(&input);

   /** \assert */
   EXPECT_EQ(input, INT32_MAX);
}




/**
 * Sat_Dec_Uint8 Tests
 * \sdd{WI-13872}
 */
TEST(StSaturatedMathTest, WI_15202_Sat_Dec_Uint8_test__no_overflow_input_shall_be_increased)
{
   /** \arrange */
   uint8_t input = 100;

   /** \action call function under test */
   Sat_Dec_Uint8(&input);

   /** \assert */
   EXPECT_EQ(input, 99u);
}

/**
* Sat_Dec_Uint8 Tests
* \sdd{WI-13872}
*/
TEST(StSaturatedMathTest, WI_15203_Sat_Dec_Uint8_test__overflow_input_shall_be_saturated)
{
   /** \arrange */
   uint8_t input = 0;

   /** \action call function under test */
   Sat_Dec_Uint8(&input);

   /** \assert */
   EXPECT_EQ(input, 0u);
}



/**
 * Sat_Dec_Int8 Tests
 * \sdd{WI-13882}
 */
TEST(StSaturatedMathTest, WI_15204_Sat_Dec_Int8_test__no_overflow_input_shall_be_increased)
{
   /** \arrange */
   int8_t input = 100;

   /** \action call function under test */
   Sat_Dec_Int8(&input);

   /** \assert */
   EXPECT_EQ(input, 99);
}

/**
* Sat_Dec_Int8 Tests
* \sdd{WI-13882}
*/
TEST(StSaturatedMathTest, WI_15205_Sat_Dec_Int8_test__overflow_input_shall_be_saturated)
{
   /** \arrange */
   int8_t input = INT8_MIN;

   /** \action call function under test */
   Sat_Dec_Int8(&input);

   /** \assert */
   EXPECT_EQ(input, INT8_MIN);
}



/**
 * Sat_Dec_Uint16 Tests
 * \sdd{WI-13870}
 */
TEST(StSaturatedMathTest, WI_15206_Sat_Dec_Uint16_test__no_overflow_input_shall_be_increased)
{
   /** \arrange */
   uint16_t input = 1000;

   /** \action call function under test */
   Sat_Dec_Uint16(&input);

   /** \assert */
   EXPECT_EQ(input, 999u);
}

/**
* Sat_Dec_Uint16 Tests
* \sdd{WI-13870}
*/
TEST(StSaturatedMathTest, WI_15207_Sat_Dec_Uint16_test__overflow_input_shall_be_saturated)
{
   /** \arrange */
   uint16_t input = 0;

   /** \action call function under test */
   Sat_Dec_Uint16(&input);

   /** \assert */
   EXPECT_EQ(input, 0u);
}



/**
 * Sat_Dec_Int16 Tests
 * \sdd{WI-13887}
 */
TEST(StSaturatedMathTest, WI_15208_Sat_Dec_Int16_test__no_overflow_input_shall_be_increased)
{
   /** \arrange */
   int16_t input = 1000;

   /** \action call function under test */
   Sat_Dec_Int16(&input);

   /** \assert */
   EXPECT_EQ(input, 999);
}

/**
* Sat_Dec_Int16 Tests
* \sdd{WI-13887}
*/
TEST(StSaturatedMathTest, WI_15209_Sat_Dec_Int16_test__overflow_input_shall_be_saturated)
{
   /** \arrange */
   int16_t input = INT16_MIN;

   /** \action call function under test */
   Sat_Dec_Int16(&input);

   /** \assert */
   EXPECT_EQ(input, INT16_MIN);
}


/**
 * Sat_Dec_Uint32 Tests
 * \sdd{WI-13880}
 */
TEST(StSaturatedMathTest, WI_15210_Sat_Dec_Uint32_test__no_overflow_input_shall_be_increased)
{
   /** \arrange */
   uint32_t input = 10000;

   /** \action call function under test */
   Sat_Dec_Uint32(&input);

   /** \assert */
   EXPECT_EQ(input, 9999u);
}

/**
* Sat_Dec_Uint32 Tests
* \sdd{WI-13880}
*/
TEST(StSaturatedMathTest, WI_15211_Sat_Dec_Uint32_test__overflow_input_shall_be_saturated)
{
   /** \arrange */
   uint32_t input = 0;

   /** \action call function under test */
   Sat_Dec_Uint32(&input);

   /** \assert */
   EXPECT_EQ(input, 0u);
}



/**
 * Sat_Dec_Int32 Tests
 * \sdd{WI-13889}
 */
TEST(StSaturatedMathTest, WI_15212_Sat_Dec_Int32_test__no_overflow_input_shall_be_increased)
{
   /** \arrange */
   int32_t input = 10000;

   /** \action call function under test */
   Sat_Dec_Int32(&input);

   /** \assert */
   EXPECT_EQ(input, 9999);
}

/**
* Sat_Dec_Int32 Tests
* \sdd{WI-13889}
*/
TEST(StSaturatedMathTest, WI_15213_Sat_Dec_Int32_test__overflow_input_shall_be_saturated)
{
   /** \arrange */
   int32_t input = INT32_MIN;

   /** \action call function under test */
   Sat_Dec_Int32(&input);

   /** \assert */
   EXPECT_EQ(input, INT32_MIN);
}



/**
 * Sat_Add_Uint8 Tests
 * \sdd{WI-13879}
 */
TEST(StSaturatedMathTest, WI_15214_Sat_Add_Uint8_test__no_overflow_result_shall_be_not_saturated)
{
   /** \arrange */
   uint8_t summand1 = UINT8_MAX - 50;
   uint8_t summand2 = 45;

   /** \action call function under test */
   uint8_t result = Sat_Add_Uint8(summand1, summand2);

   /** \assert */
   EXPECT_EQ(result, UINT8_MAX - 5u);
}

/**
* Sat_Add_Uint8 Tests
* \sdd{WI-13879}
*/
TEST(StSaturatedMathTest, WI_15215_Sat_Add_Uint8_test__overflow_result_shall_be_saturated)
{
   /** \arrange */
   uint8_t summand1 = UINT8_MAX - 5u;
   uint8_t summand2 = 200;

   /** \action call function under test */
   uint8_t result = Sat_Add_Uint8(summand1, summand2);

   /** \assert */
   EXPECT_EQ(result, UINT8_MAX);
}

/**
 * Sat_Add_Int8 Tests
 * \sdd{WI-13875}
 */
TEST(StSaturatedMathTest, WI_15216_Sat_Add_Int8_test__no_overflow_result_shall_be_not_saturated)
{
   /** \arrange */
   int8_t summand1 = INT8_MAX - 20;
   int8_t summand2 = 10;

   /** \action call function under test */
   int8_t result = Sat_Add_Int8(summand1, summand2);

   /** \assert */
   EXPECT_EQ(result, INT8_MAX - 10);
}

/**
* Sat_Add_Int8 Tests
* \sdd{WI-13875}
*/
TEST(StSaturatedMathTest, WI_15217_Sat_Add_Int8_test__overflow_result_shall_be_saturated)
{
   /** \arrange */
   int8_t summand1 = INT8_MAX - 10;
   int8_t summand2 = 50;

   /** \action call function under test */
   int8_t result = Sat_Add_Int8(summand1, summand2);

   /** \assert */
   EXPECT_EQ(result, INT8_MAX);
}

/**
 * Sat_Add_Uint16 Tests
 * \sdd{WI-13885}
 */
TEST(StSaturatedMathTest, WI_15218_Sat_Add_Uint16_test__no_overflow_result_shall_be_not_saturated)
{
   /** \arrange */
   uint16_t summand1 = UINT16_MAX - 15u;
   uint16_t summand2 = 10;

   /** \action call function under test */
   uint16_t result = Sat_Add_Uint16(summand1, summand2);

   /** \assert */
   EXPECT_EQ(result, UINT16_MAX - 5u);
}

/**
* Sat_Add_Uint16 Tests
* \sdd{WI-13885}
*/
TEST(StSaturatedMathTest, WI_15219_Sat_Add_Uint16_test__overflow_result_shall_be_saturated)
{
   /** \arrange */
   uint16_t summand1 = UINT16_MAX - 100u;
   uint16_t summand2 = 2000u;

   /** \action call function under test */
   uint16_t result = Sat_Add_Uint16(summand1, summand2);

   /** \assert */
   EXPECT_EQ(result, UINT16_MAX);
}

/**
 * Sat_Add_Int16 Tests
 * \sdd{WI-13869}
 */
TEST(StSaturatedMathTest, WI_15220_Sat_Add_Int16_test__no_overflow_result_shall_be_not_saturated)
{
   /** \arrange */
   int16_t summand1 = INT16_MAX - 200;
   int16_t summand2 = 100;

   /** \action call function under test */
   int16_t result = Sat_Add_Int16(summand1, summand2);

   /** \assert */
   EXPECT_EQ(result, INT16_MAX - 100);
}

/**
* Sat_Add_Int16 Tests
* \sdd{WI-13869}
*/
TEST(StSaturatedMathTest, WI_15221_Sat_Add_Int16_test__overflow_result_shall_be_saturated)
{
   /** \arrange */
   int16_t summand1 = INT16_MAX - 100;
   int16_t summand2 = 2000;

   /** \action call function under test */
   int16_t result = Sat_Add_Int16(summand1, summand2);

   /** \assert */
   EXPECT_EQ(result, INT16_MAX);
}

/**
 * Sat_Add_Uint32 Tests
 * \sdd{WI-13873}
 */
TEST(StSaturatedMathTest, WI_15222_Sat_Add_Uint32_test__no_overflow_result_shall_be_not_saturated)
{
   /** \arrange */
   uint32_t summand1 = UINT32_MAX - 15u;
   uint32_t summand2 = 5u;

   /** \action call function under test */
   uint32_t result = Sat_Add_Uint32(summand1, summand2);

   /** \assert */
   EXPECT_EQ(result, UINT32_MAX - 10u);
}

/**
* Sat_Add_Uint32 Tests
* \sdd{WI-13873}
*/
TEST(StSaturatedMathTest, WI_15223_Sat_Add_Uint32_test__overflow_result_shall_be_saturated)
{
   /** \arrange */
   uint32_t summand1 = UINT32_MAX - 15u;
   uint32_t summand2 = 2000u;

   /** \action call function under test */
   uint32_t result = Sat_Add_Uint32(summand1, summand2);

   /** \assert */
   EXPECT_EQ(result, UINT32_MAX);
}

/**
* Sat_Add_Uint32 Tests
* \sdd{WI-13873}
*/
TEST(StSaturatedMathTest, WI_15224_Sat_Add_Uint32_test__parameter_one_max_range_saturate)
{
   /** \arrange */
   uint32_t summand1 = UINT32_MAX;
   uint32_t summand2 = 1u;

   /** \action call function under test */
   uint32_t result = Sat_Add_Uint32(summand1, summand2);

   /** \assert */
   EXPECT_EQ(result, UINT32_MAX);
}

/**
* Sat_Add_Uint32 Tests
* \sdd{WI-13873}
*/
TEST(StSaturatedMathTest, WI_15225_Sat_Add_Uint32_test__parameter_two_max_range_saturate)
{
   /** \arrange */
   uint32_t summand1 = 1u;
   uint32_t summand2 = UINT32_MAX;

   /** \action call function under test */
   uint32_t result = Sat_Add_Uint32(summand1, summand2);

   /** \assert */
   EXPECT_EQ(result, UINT32_MAX);
}

/**
* Sat_Add_Uint32 Tests
* \sdd{WI-13873}
*/
TEST(StSaturatedMathTest, WI_15226_Sat_Add_Uint32_test__parameter_min_nonsaturate)
{
   /** \arrange */
   uint32_t summand1 = 0u;
   uint32_t summand2 = 0u;

   /** \action call function under test */
   uint32_t result = Sat_Add_Uint32(summand1, summand2);

   /** \assert */
   EXPECT_EQ(result, 0u);
}

/**
 * Sat_Add_Int32 Tests
 * \sdd{WI-13871}
 */
TEST(StSaturatedMathTest, WI_15227_Sat_Add_Int32_test__positive_no_overflow_result_shall_be_not_saturated)
{
   /** \arrange */
   int32_t summand1 = INT32_MAX - 200;
   int32_t summand2 = 100;

   /** \action call function under test */
   int32_t result = Sat_Add_Int32(summand1, summand2);

   /** \assert */
   EXPECT_EQ(result, INT32_MAX - 100);
}

/**
* Sat_Add_Int32 Tests
* \sdd{WI-13871}
*/
TEST(StSaturatedMathTest, WI_15228_Sat_Add_Int32_test__negative_no_overflow_result_shall_be_not_saturated)
{
   /** \arrange */
   int32_t summand1 = INT32_MIN + 200;
   int32_t summand2 = -100;

   /** \action call function under test */
   int32_t result = Sat_Add_Int32(summand1, summand2);

   /** \assert */
   EXPECT_EQ(result, INT32_MIN + 100);
}


/**
* Sat_Add_Int32 Tests
* \sdd{WI-13871}
*/
TEST(StSaturatedMathTest, WI_15229_Sat_Add_Int32_test__positive_overflow_result_shall_be_saturated)
{
   /** \arrange */
   int32_t summand1 = INT32_MAX - 100;
   int32_t summand2 = 2000;

   /** \action call function under test */
   int32_t result = Sat_Add_Int32(summand1, summand2);

   /** \assert */
   EXPECT_EQ(result, INT32_MAX);
}

/**
* Sat_Add_Int32 Tests
* \sdd{WI-13871}
*/
TEST(StSaturatedMathTest, WI_15230_Sat_Add_Int32_test__negative_overflow_result_shall_be_saturated)
{
   /** \arrange */
   int32_t summand1 = INT32_MIN + 100;
   int32_t summand2 = -2000;

   /** \action call function under test */
   int32_t result = Sat_Add_Int32(summand1, summand2);

   /** \assert */
   EXPECT_EQ(result, INT32_MIN);
}


/**
 * Sat_Sub_Uint8 Tests
 * \sdd{WI-13884}
 */
TEST(StSaturatedMathTest, WI_15231_Sat_Sub_Uint8_test__no_underflow_result_shall_be_not_saturated)
{
   /** \arrange */
   uint8_t minuend    = 50u;
   uint8_t subtrahend = 25u;

   /** \action call function under test */
   uint8_t result = Sat_Sub_Uint8(minuend, subtrahend);

   /** \assert */
   EXPECT_EQ(result, 25u);
}


/**
* Sat_Sub_Uint8 Tests
* \sdd{WI-13884}
*/
TEST(StSaturatedMathTest, WI_15232_Sat_Sub_Uint8_test__underflow_result_shall_be_saturated)
{
   /** \arrange */
   uint8_t minuend    = 50u;
   uint8_t subtrahend = 123u;

   /** \action call function under test */
   uint8_t result = Sat_Sub_Uint8(minuend, subtrahend);

   /** \assert */
   EXPECT_EQ(result, 0u);
}

/**
 * Sat_Sub_Int8 Tests
 * \sdd{WI-13888}
 */
TEST(StSaturatedMathTest, WI_15233_Sat_Sub_Int8_test__no_underflow_result_shall_be_not_saturated)
{
   /** \arrange */
   int8_t minuend    = INT8_MIN + 50;
   int8_t subtrahend = 40;

   /** \action call function under test */
   int8_t result = Sat_Sub_Int8(minuend, subtrahend);

   /** \assert */
   EXPECT_EQ(result, INT8_MIN + 10);
}


/**
* Sat_Sub_Int8 Tests
* \sdd{WI-13888}
*/
TEST(StSaturatedMathTest, WI_15234_Sat_Sub_Int8_test__underflow_result_shall_be_saturated)
{
   /** \arrange */
   int8_t minuend    = INT8_MIN + 50;
   int8_t subtrahend = 100;

   /** \action call function under test */
   int8_t result = Sat_Sub_Int8(minuend, subtrahend);

   /** \assert */
   EXPECT_EQ(result, INT8_MIN);
}


/**
 * Sat_Sub_Uint16 Tests
 * \sdd{WI-13881}
 */
TEST(StSaturatedMathTest, WI_15235_Sat_Sub_Uint16_test__no_underflow_result_shall_be_not_saturated)
{
   /** \arrange */
   uint16_t minuend    = 500u;
   uint16_t subtrahend = 250u;

   /** \action call function under test */
   uint16_t result = Sat_Sub_Uint16(minuend, subtrahend);

   /** \assert */
   EXPECT_EQ(result, 250u);
}

/**
* Sat_Sub_Uint16 Tests
* \sdd{WI-13881}
*/
TEST(StSaturatedMathTest, WI_15236_Sat_Sub_Uint16_test__underflow_result_shall_be_saturated)
{
   /** \arrange */
   uint16_t minuend    = 500u;
   uint16_t subtrahend = 1234u;

   /** \action call function under test */
   uint16_t result = Sat_Sub_Uint16(minuend, subtrahend);

   /** \assert */
   EXPECT_EQ(result, 0u);
}

/**
 * Sat_Sub_Int16 Tests
 * \sdd{WI-13890}
 */
TEST(StSaturatedMathTest, WI_15237_Sat_Sub_Int16_test__no_underflow_result_shall_be_not_saturated)
{
   /** \arrange */
   int16_t minuend    = INT16_MIN + 50;
   int16_t subtrahend = 40;

   /** \action call function under test */
   int16_t result = Sat_Sub_Int16(minuend, subtrahend);

   /** \assert */
   EXPECT_EQ(result, INT16_MIN + 10);
}

/**
* Sat_Sub_Int16 Tests
* \sdd{WI-13890}
*/
TEST(StSaturatedMathTest, WI_15238_Sat_Sub_Int16_test__underflow_result_shall_be_saturated)
{
   /** \arrange */
   int16_t minuend    = INT16_MIN + 50;
   int16_t subtrahend = 100;

   /** \action call function under test */
   int16_t result = Sat_Sub_Int16(minuend, subtrahend);

   /** \assert */
   EXPECT_EQ(result, INT16_MIN);
}


/**
 * Sat_Sub_Uint32 Tests
 * \sdd{WI-13883}
 */
TEST(StSaturatedMathTest, WI_15239_Sat_Sub_Uint32_test__no_underflow_result_shall_be_not_saturated)
{
   /** \arrange */
   uint32_t minuend    = 500u;
   uint32_t subtrahend = 250u;

   /** \action call function under test */
   uint32_t result = Sat_Sub_Uint32(minuend, subtrahend);

   /** \assert */
   EXPECT_EQ(result, 250u);
}

/**
* Sat_Sub_Uint32 Tests
* \sdd{WI-13883}
*/
TEST(StSaturatedMathTest, WI_15240_Sat_Sub_Uint32_test__underflow_result_shall_be_saturated)
{
   /** \arrange */
   uint32_t minuend    = 5000u;
   uint32_t subtrahend = 12345u;

   /** \action call function under test */
   uint32_t result = Sat_Sub_Uint32(minuend, subtrahend);

   /** \assert */
   EXPECT_EQ(result, 0u);
}

/**
 * Sat_Sub_Int32 Tests
 * \sdd{WI-13886}
 */
TEST(StSaturatedMathTest, WI_15241_Sat_Sub_Int32_test__no_underflow_result_shall_be_not_saturated)
{
   /** \arrange */
   int32_t minuend    = INT32_MIN + 50;
   int32_t subtrahend = 40;

   /** \action call function under test */
   int32_t result = Sat_Sub_Int32(minuend, subtrahend);

   /** \assert */
   EXPECT_EQ(result, INT32_MIN + 10);
}

/**
* Sat_Sub_Int32 Tests
* \sdd{WI-13886}
*/
TEST(StSaturatedMathTest, WI_15242_Sat_Sub_Int32_test__underflow_result_shall_be_saturated)
{
   /** \arrange */
   int32_t minuend    = INT32_MIN + 50;
   int32_t subtrahend = 100;

   /** \action call function under test */
   int32_t result = Sat_Sub_Int32(minuend, subtrahend);

   /** \assert */
   EXPECT_EQ(result, INT32_MIN);
}

