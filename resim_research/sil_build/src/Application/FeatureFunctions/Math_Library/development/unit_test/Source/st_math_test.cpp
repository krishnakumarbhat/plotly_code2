/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <gtest/gtest.h>
#include "ml_math.h"
#include "ml_math_infinity_silent.h"


/**
* \sdd{WI-13992}
*/
TEST(test_Sign_macro, WI_15003_macro_expansion_multiplication)
{
   /** \arrange */
   float num = 5;
   /** \action call function under test */
   float res = num * Sign(-10);
   /** \assert */
   EXPECT_EQ(res, -5);
}

/**
* \sdd{WI-13992}
*/
TEST(test_Sign_macro, WI_15004_macro_expansion_multiplication_reversed)
{
   /** \arrange */
   float num = 5;
   /** \action call function under test */
   float res = Sign(-10) * num;
   /** \assert */
   EXPECT_EQ(res, -5);
}

/**
* \sdd{WI-13992}
*/
TEST(test_Sign_macro, WI_15005_macro_expansion_addition)
{
   /** \arrange */
   float num = 5;
   /** \action call function under test */
   float res = num + Sign(-10);
   /** \assert */
   EXPECT_EQ(res, 4);
}

/**
* \sdd{WI-13992}
*/
TEST(test_Sign_macro, WI_15006_macro_expansion_addition_reversed)
{
   /** \arrange */
   float num = 5;
   /** \action call function under test */
   float res = Sign(-10) + num;
   /** \assert */
   EXPECT_EQ(res, 4);
}

/**
* \sdd{WI-13992}
*/
TEST(test_Sign_macro, WI_15007_positive)
{
   /** \arrange */
   /** \action call function under test */
   float res = Sign(10);
   EXPECT_EQ(res, 1);
}

/**
* \sdd{WI-13992}
*/
TEST(test_Sign_macro, WI_15008_negative)
{
   /** \arrange */
   /** \action call function under test */
   float res = Sign(-10);
   /** \assert */
   EXPECT_EQ(res, -1);
}

/**
* \sdd{WI-13992}
*/
TEST(test_Sign_macro, WI_15009_zero)
{
   /** \arrange */
   /** \action call function under test */
   float res = Sign(0);
   /** \assert */
   EXPECT_EQ(res, 0);
}


/**
* Test the function to process absolute value of a number
* \sdd{WI-13990}
*/
TEST(BasicMarcosTest, WI_15010_Abs__test_neg_int32)
{
   /** \arrange */
   const int   val_i = -2;

   /** \action call function under test */
   const int   test_result_i = Abs(val_i);

   /** \assert */
   EXPECT_NE(val_i, test_result_i);

   EXPECT_EQ(-val_i, test_result_i);
}

/**
*\sdd{ WI-13990}
*/
TEST(BasicMarcosTest, Abs__test_pos_int32)
{
   /** \arrange */
   const int   val_i = 4;

   /** \action call function under test */
   const int test_result_i = Abs(val_i);

   /** \assert */
   EXPECT_EQ(val_i, test_result_i);
}

/**
*\sdd{ WI-13990}
*/
TEST(BasicMarcosTest, Abs__test_neg_float)
{
   /** \arrange */
   const float val_f = -1.5f;

   /** \action call function under test */
   const float test_result_f = Abs(val_f);

   /** \assert */
   EXPECT_NE(val_f, test_result_f);

   EXPECT_FLOAT_EQ(-val_f, test_result_f);
}

/**
*\sdd{ WI-13990}
*/
TEST(BasicMarcosTest, Abs__test_pos_float)
{
   /** \arrange */
   const float val_f = 1.2f;

   /** \action call function under test */
   const float test_result_f = Abs(val_f);

   /** \assert */
   EXPECT_FLOAT_EQ(val_f, test_result_f);
}

/**
*\sdd{ WI-13990}
*/
TEST(BasicMarcosTest, Abs__test_neg_double)
{
   /** \arrange */
   const double    val_d = -PI;

   /** \action call function under test */
   const double    test_result_d = Abs(val_d);

   /** \assert */
   EXPECT_NE(val_d, test_result_d);
   EXPECT_DOUBLE_EQ(-val_d, test_result_d);
}

/**
*\sdd{ WI-13990}
*/
TEST(BasicMarcosTest, Abs__test_pos_double)
{
   /** \arrange */
   const double    val_d = PI;

   /** \action call function under test */
   const double    test_result_d = Abs(val_d);

   /** \assert */
   EXPECT_DOUBLE_EQ(val_d, test_result_d);
}


/**
* Test the function to return the max value between two numbers
* \sdd{WI-13986}
*/
TEST(BasicMarcosTest, WI_15011_MAX__test_int32_max_is_first_parameter)
{
   /** \arrange */
   const int   val_i1 = 2;
   const int   val_i2 = 9;

   /** \action call function under test */
   const int   test_result_i2 = Max(val_i2, val_i1);

   /** \assert */
   EXPECT_NE(val_i1, test_result_i2);
   EXPECT_EQ(val_i2, test_result_i2);
}

/**
* \sdd{WI-13986}
*/
TEST(BasicMarcosTest, WI_15012_MAX__test_int32_max_is_last_parameter)
{
   /** \arrange */
   const int   val_i1 = 2;
   const int   val_i2 = 9;

   /** \action call function under test */
   const int   test_result_i1 = Max(val_i1, val_i2);

   /** \assert */
   EXPECT_NE(val_i1, test_result_i1);
   EXPECT_EQ(val_i2, test_result_i1);
}

/**
* \sdd{WI-13986}
*/
TEST(BasicMarcosTest, WI_15013_MAX__test_int32_order_is_irrelevant)
{
   /** \arrange */
   const int   val_i1 = 2;
   const int   val_i2 = 9;

   /** \action call function under test */
   const int   test_result_i1 = Max(val_i1, val_i2);
   const int   test_result_i2 = Max(val_i2, val_i1);

   /** \assert */
   EXPECT_EQ(test_result_i1, test_result_i2);
}

/**
* \sdd{WI-13986}
*/
TEST(BasicMarcosTest, WI_15014_MAX__test_int32_against_infinity)
{
   /** \arrange */
   const int   val_i2 = 9;

   /** \action call function under test */
   const double    test_result_val_inf_f1 = Max(AS_TOOLBOX_INFINITY, val_i2);

   /** \assert */
   EXPECT_EQ(AS_TOOLBOX_INFINITY, test_result_val_inf_f1);
}

/**
* \sdd{WI-13986}
*/
TEST(BasicMarcosTest, WI_15015_MAX__test_int32_against_neg_infinity)
{
   /** \arrange */
   const int   val_i2 = 9;

   /** \action call function under test */
   const double    test_result_val_inf_f1 = Max(-AS_TOOLBOX_INFINITY, val_i2);

   /** \assert */
   EXPECT_EQ(val_i2, test_result_val_inf_f1);
}



/**
* \sdd{WI-13986}
*/
TEST(BasicMarcosTest, WI_15016_MAX__test_float_max_is_first_parameter)
{
   /** \arrange */
   const float val_f1 = -1.5f;
   const float val_f2 = 1.5f;

   /** \action call function under test */
   const float test_result_f2 = Max(val_f2, val_f1);

   /** \assert */
   EXPECT_NE(val_f1, test_result_f2);
   EXPECT_EQ(val_f2, test_result_f2);
}

/**
* \sdd{WI-13986}
*/
TEST(BasicMarcosTest, WI_15017_MAX__test_float_max_is_last_parameter)
{
   /** \arrange */
   const float val_f1 = -1.5f;
   const float val_f2 = 1.5f;

   /** \action call function under test */
   const float test_result_f1 = Max(val_f1, val_f2);

   /** \assert */
   EXPECT_NE(val_f1, test_result_f1);
   EXPECT_EQ(val_f2, test_result_f1);
}

/**
* \sdd{WI-13986}
*/
TEST(BasicMarcosTest, WI_15018_MAX__test_float_order_is_irrelevant)
{
   /** \arrange */
   const float val_f1 = -1.5f;
   const float val_f2 = 1.5f;

   /** \action call function under test */
   const float test_result_f1 = Max(val_f1, val_f2);
   const float test_result_f2 = Max(val_f2, val_f1);

   /** \assert */
   EXPECT_EQ(test_result_f1, test_result_f2);
}

/**
* \sdd{WI-13986}
*/
TEST(BasicMarcosTest, WI_15019_MAX__test_float_against_infinity)
{
   /** \arrange */
   const float    val_d1 = -1.5f;

   /** \action call function under test */
   const float    test_result_val_inf_f1 = Max(AS_TOOLBOX_INFINITY, val_d1);

   /** \assert */
   EXPECT_EQ(AS_TOOLBOX_INFINITY, test_result_val_inf_f1);
}

/**
* \sdd{WI-13986}
*/
TEST(BasicMarcosTest, WI_15020_MAX__test_float_against_neg_infinity)
{
   /** \arrange */
   const float    val_d1 = -1.5;

   /** \action call function under test */
   const float    test_result_val_inf_f1 = Max(-AS_TOOLBOX_INFINITY, val_d1);

   /** \assert */
   EXPECT_EQ(val_d1, test_result_val_inf_f1);
}



/**
* \sdd{WI-13986}
*/
TEST(BasicMarcosTest, WI_15021_MAX__test_double_max_is_first_parameter)
{
   /** \arrange */
   const double    val_d1 = -PI;
   const double    val_d2 = PI / 2.5;

   /** \action call function under test */
   const double    test_result_d2 = Max(val_d2, val_d1);

   /** \assert */
   EXPECT_NE(val_d1, test_result_d2);
   EXPECT_EQ(val_d2, test_result_d2);
}

/**
* \sdd{WI-13986}
*/
TEST(BasicMarcosTest, WI_15022_MAX__test_double_max_is_last_parameter)
{
   /** \arrange */
   const double    val_d1 = -PI;
   const double    val_d2 = PI / 2.5;

   /** \action call function under test */
   const double    test_result_d1 = Max(val_d1, val_d2);

   /** \assert */
   EXPECT_NE(val_d1, test_result_d1);
   EXPECT_EQ(val_d2, test_result_d1);
}

/**
* \sdd{WI-13986}
*/
TEST(BasicMarcosTest, WI_15023_MAX__test_double_order_is_irrelevant)
{
   /** \arrange */
   const double    val_d1 = -PI;
   const double    val_d2 = PI / 2.5;

   /** \action call function under test */
   const double    test_result_d1 = Max(val_d1, val_d2);
   const double    test_result_d2 = Max(val_d1, val_d2);

   /** \assert */
   EXPECT_EQ(test_result_d1, test_result_d2);
}

/**
* \sdd{WI-13986}
*/
TEST(BasicMarcosTest, WI_15024_MAX__test_double_against_infinity)
{
   /** \arrange */
   const double    val_d1 = -PI;

   /** \action call function under test */
   const double    test_result_val_inf_f1 = Max(AS_TOOLBOX_INFINITY, val_d1);

   /** \assert */
   EXPECT_EQ(AS_TOOLBOX_INFINITY, test_result_val_inf_f1);
}

/**
* \sdd{WI-13986}
*/
TEST(BasicMarcosTest, WI_15025_MAX__test_double_against_neg_infinity)
{
   /** \arrange */
   const double    val_d1 = -PI;

   /** \action call function under test */
   const double    test_result_val_inf_f1 = Max(-AS_TOOLBOX_INFINITY, val_d1);

   /** \assert */
   EXPECT_EQ(val_d1, test_result_val_inf_f1);
}


/**
* \sdd{WI-13986}
*/
TEST(BasicMarcosTest, WI_15026_MAX__test_double_against_int_result_is_int)
{
   /** \arrange */
   const double    val_d = -PI;
   const int		val_i = 1;
   /** \action call function under test */
   const double    result = Max(val_i, val_d);

   /** \assert */
   EXPECT_EQ(val_i, result);
}

/**
* \sdd{WI-13986}
*/
TEST(BasicMarcosTest, WI_15027_MAX__test_double_against_int_result_is_double)
{
   /** \arrange */
   const double    val_d = PI;
   const int		val_i = 1;
   /** \action call function under test */
   const double    result = Max(val_i, val_d);

   /** \assert */
   EXPECT_EQ(val_d, result);
}



/**
* \sdd{WI-13986}
*/
TEST(BasicMarcosTest, WI_15028_MAX__test_float_against_int_result_is_int)
{
   /** \arrange */
   const float    val_f = -1.5f;
   const int		val_i = 1;
   /** \action call function under test */
   const float    result = Max(val_i, val_f);

   /** \assert */
   EXPECT_EQ(val_i, result);
}

/**
* \sdd{WI-13986}
*/
TEST(BasicMarcosTest, WI_15029_MAX__test_float_against_int_result_is_float)
{
   /** \arrange */
   const float    val_f = 1.5f;
   const int		val_i = 1;
   /** \action call function under test */
   const float    result = Max(val_i, val_f);

   /** \assert */
   EXPECT_EQ(val_f, result);
}



/**
* \sdd{WI-13986}
*/
TEST(BasicMarcosTest, WI_15030_MAX__test_float_against_double_result_is_double)
{
   /** \arrange */
   const float    val_f = -1.5f;
   const double	val_d = PI;
   /** \action call function under test */
   const double    result = Max(val_d, val_f);

   /** \assert */
   EXPECT_EQ(val_d, result);
}

/**
* \sdd{WI-13986}
*/
TEST(BasicMarcosTest, WI_15031_MAX__test_float_against_double_result_is_float)
{
   /** \arrange */
   const float		val_f = 1.5f;
   const double	val_d = -PI;
   /** \action call function under test */
   const double    result = Max(val_d, val_f);

   /** \assert */
   EXPECT_EQ(val_f, result);
}



/**
* Test the function to return the min value between two numbers
* \sdd{WI-13988}
*/
TEST(BasicMarcosTest, WI_15032_MIN__test_int32_MIN_is_first_parameter)
{
   /** \arrange */
   const int   val_i1 = 2;
   const int   val_i2 = 9;

   /** \action call function under test */
   const int   test_result_i2 = Min(val_i1, val_i2);

   /** \assert */
   EXPECT_NE(val_i2, test_result_i2);
   EXPECT_EQ(val_i1, test_result_i2);
}

/**
* Test the function to return the min value between two numbers
* \sdd{WI-13988}
*/
TEST(BasicMarcosTest, WI_15033_MIN__test_int32_MIN_is_last_parameter)
{
   /** \arrange */
   const int   val_i1 = 2;
   const int   val_i2 = 9;

   /** \action call function under test */
   const int   test_result_i1 = Min(val_i2, val_i1);

   /** \assert */
   EXPECT_NE(val_i2, test_result_i1);
   EXPECT_EQ(val_i1, test_result_i1);
}

/**
* Test the function to return the min value between two numbers
* \sdd{WI-13988}
*/
TEST(BasicMarcosTest, WI_15034_MIN__test_int32_order_is_irrelevant)
{
   /** \arrange */
   const int   val_i1 = 2;
   const int   val_i2 = 9;

   /** \action call function under test */
   const int   test_result_i1 = Min(val_i1, val_i2);
   const int   test_result_i2 = Min(val_i2, val_i1);

   /** \assert */
   EXPECT_EQ(test_result_i1, test_result_i2);
}

/**
* Test the function to return the min value between two numbers
* \sdd{WI-13988}
*/
TEST(BasicMarcosTest, WI_15035_MIN__test_int32_against_infinity)
{
   /** \arrange */
   const int   val_i2 = 9;

   /** \action call function under test */
   const double    test_result_val_inf_f1 = Min(AS_TOOLBOX_INFINITY, val_i2);

   /** \assert */
   EXPECT_EQ(val_i2, test_result_val_inf_f1);
}

/**
* Test the function to return the min value between two numbers
* \sdd{WI-13988}
*/
TEST(BasicMarcosTest, WI_15036_MIN__test_int32_against_neg_infinity)
{
   /** \arrange */
   const int   val_i2 = 9;

   /** \action call function under test */
   const double    test_result_val_inf_f1 = Min(-AS_TOOLBOX_INFINITY, val_i2);

   /** \assert */
   EXPECT_EQ(-AS_TOOLBOX_INFINITY, test_result_val_inf_f1);
}


/**
* Test the function to return the min value between two numbers
* \sdd{WI-13988}
*/
TEST(BasicMarcosTest, WI_15037_MIN__test_float_MIN_is_first_parameter)
{
   /** \arrange */
   const float val_f1 = -1.5f;
   const float val_f2 = 1.5f;

   /** \action call function under test */
   const float test_result_f2 = Min(val_f1, val_f2);

   /** \assert */
   EXPECT_NE(val_f2, test_result_f2);
   EXPECT_EQ(val_f1, test_result_f2);
}

/**
* Test the function to return the min value between two numbers
* \sdd{WI-13988}
*/
TEST(BasicMarcosTest, WI_15038_MIN__test_float_MIN_is_last_parameter)
{
   /** \arrange */
   const float val_f1 = -1.5f;
   const float val_f2 = 1.5f;

   /** \action call function under test */
   const float test_result_f1 = Min(val_f2, val_f1);

   /** \assert */
   EXPECT_NE(val_f2, test_result_f1);
   EXPECT_EQ(val_f1, test_result_f1);
}

/**
* Test the function to return the min value between two numbers
* \sdd{WI-13988}
*/
TEST(BasicMarcosTest, WI_15039_MIN__test_float_order_is_irrelevant)
{
   /** \arrange */
   const float val_f1 = -1.5f;
   const float val_f2 = 1.5f;

   /** \action call function under test */
   const float test_result_f1 = Min(val_f1, val_f2);
   const float test_result_f2 = Min(val_f2, val_f1);

   /** \assert */
   EXPECT_EQ(test_result_f1, test_result_f2);
}

/**
* Test the function to return the min value between two numbers
* \sdd{WI-13988}
*/
TEST(BasicMarcosTest, WI_15040_MIN__test_float_against_infinity)
{
   /** \arrange */
   const float    val_d1 = -1.5f;

   /** \action call function under test */
   const float    test_result_val_inf_f1 = Min(AS_TOOLBOX_INFINITY, val_d1);

   /** \assert */
   EXPECT_EQ(val_d1, test_result_val_inf_f1);
}

/**
* Test the function to return the min value between two numbers
* \sdd{WI-13988}
*/
TEST(BasicMarcosTest, WI_15041_MIN__test_float_against_neg_infinity)
{
   /** \arrange */
   const float    val_d1 = -1.5f;

   /** \action call function under test */
   const float    test_result_val_inf_f1 = Min(-AS_TOOLBOX_INFINITY, val_d1);

   /** \assert */
   EXPECT_EQ(-AS_TOOLBOX_INFINITY, test_result_val_inf_f1);
}



/**
* Test the function to return the min value between two numbers
* \sdd{WI-13988}
*/
TEST(BasicMarcosTest, WI_15042_MIN__test_double_MIN_is_first_parameter)
{
   /** \arrange */
   const double    val_d1 = -PI;
   const double    val_d2 = PI / 2.5;

   /** \action call function under test */
   const double    test_result_d2 = Min(val_d1, val_d2);

   /** \assert */
   EXPECT_NE(val_d2, test_result_d2);
   EXPECT_EQ(val_d1, test_result_d2);
}

/**
* Test the function to return the min value between two numbers
* \sdd{WI-13988}
*/
TEST(BasicMarcosTest, WI_15043_MIN__test_double_MIN_is_last_parameter)
{
   /** \arrange */
   const double    val_d1 = -PI;
   const double    val_d2 = PI / 2.5;

   /** \action call function under test */
   const double    test_result_d1 = Min(val_d2, val_d1);

   /** \assert */
   EXPECT_NE(val_d2, test_result_d1);
   EXPECT_EQ(val_d1, test_result_d1);
}

/**
* Test the function to return the min value between two numbers
* \sdd{WI-13988}
*/
TEST(BasicMarcosTest, WI_15044_MIN__test_double_order_is_irrelevant)
{
   /** \arrange */
   const double    val_d1 = -PI;
   const double    val_d2 = PI / 2.5;

   /** \action call function under test */
   const double    test_result_d1 = Min(val_d1, val_d2);
   const double    test_result_d2 = Min(val_d1, val_d2);

   /** \assert */
   EXPECT_EQ(test_result_d1, test_result_d2);
}

/**
* Test the function to return the min value between two numbers
* \sdd{WI-13988}
*/
TEST(BasicMarcosTest, WI_15045_MIN__test_double_against_infinity)
{
   /** \arrange */
   const double    val_d1 = -PI;

   /** \action call function under test */
   const double    test_result_val_inf_f1 = Min(AS_TOOLBOX_INFINITY, val_d1);

   /** \assert */
   EXPECT_EQ(val_d1, test_result_val_inf_f1);
}

/**
* Test the function to return the min value between two numbers
* \sdd{WI-13988}
*/
TEST(BasicMarcosTest, WI_15046_MIN__test_double_against_neg_infinity)
{
   /** \arrange */
   const double    val_d1 = -PI;

   /** \action call function under test */
   const double    test_result_val_inf_f1 = Min(-AS_TOOLBOX_INFINITY, val_d1);

   /** \assert */
   EXPECT_EQ(-AS_TOOLBOX_INFINITY, test_result_val_inf_f1);
}



/**
* Test the function to return the min value between two numbers
* \sdd{WI-13988}
*/
TEST(BasicMarcosTest, WI_15047_MIN__test_double_against_int_result_is_int)
{
   /** \arrange */
   const double    val_d = PI;
   const int		val_i = 1;
   /** \action call function under test */
   const double    result = Min(val_d, val_i);

   /** \assert */
   EXPECT_EQ(val_i, result);
}

/**
* Test the function to return the min value between two numbers
* \sdd{WI-13988}
*/
TEST(BasicMarcosTest, WI_15048_MIN__test_double_against_int_result_is_double)
{
   /** \arrange */
   const double    val_d = -PI;
   const int		val_i = 1;
   /** \action call function under test */
   const double    result = Min(val_i, val_d);

   /** \assert */
   EXPECT_EQ(val_d, result);
}



/**
* Test the function to return the min value between two numbers
* \sdd{WI-13988}
*/
TEST(BasicMarcosTest, WI_15049_MIN__test_float_against_int_result_is_int)
{
   /** \arrange */
   const float    val_f = 1.5f;
   const int		val_i = 1;
   /** \action call function under test */
   const float    result = Min(val_i, val_f);

   /** \assert */
   EXPECT_EQ(val_i, result);
}

/**
* Test the function to return the min value between two numbers
* \sdd{WI-13988}
*/
TEST(BasicMarcosTest, WI_15050_MIN__test_float_against_int_result_is_float)
{
   /** \arrange */
   const float    val_f = -1.5f;
   const int		val_i = 1;
   /** \action call function under test */
   const float    result = Min(val_i, val_f);

   /** \assert */
   EXPECT_EQ(val_f, result);
}



/**
* Test the function to return the min value between two numbers
* \sdd{WI-13988}
*/
TEST(BasicMarcosTest, WI_15051_MIN__test_float_against_double_result_is_double)
{
   /** \arrange */
   const float    val_f = 1.5f;
   const double	val_d = -PI;
   /** \action call function under test */
   const double    result = Min(val_d, val_f);

   /** \assert */
   EXPECT_EQ(val_d, result);
}

/**
* Test the function to return the min value between two numbers
* \sdd{WI-13988}
*/
TEST(BasicMarcosTest, WI_15052_MIN__test_float_against_double_result_is_float)
{
   /** \arrange */
   const float		val_f = -5.5f;
   const double	val_d = -PI;
   /** \action call function under test */
   const double    result = Min(val_d, val_f);

   /** \assert */
   EXPECT_EQ(val_f, result);
}

/**
* \sdd{WI-13999}
*/
TEST(Math_Macros_Test, WI_15053_Quotient_Ceiled_CEILED_INPUT)
{
   /** \arrange */
   /** \action call function under test */
   int result = Quotient_Ceiled(10, 4);

   /** \assert */
   EXPECT_EQ(result, 3);
}

/**
* \sdd{WI-13999}
*/
TEST(Math_Macros_Test, WI_15054_Quotient_Ceiled_NON_CEILED_INPUT)
{
   /** \arrange */
   /** \action call function under test */
   int result = Quotient_Ceiled(10, 5);

   /** \assert */
   EXPECT_EQ(result, 2);
}