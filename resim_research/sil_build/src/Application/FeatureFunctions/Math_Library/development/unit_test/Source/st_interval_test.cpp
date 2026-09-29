/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <gtest/gtest.h>
#include "st_float_range_helper.hpp"
#include "st_int_range_helper.hpp"
#include "ml_bool.h"
#include "ml_interval.h"
#include "ml_math.h"
#include "ml_float_range_t.h"
#include "ml_int_range_t.h"
#include "ml_math_infinity_silent.h"


/**
 * The unit tests for the function to create float ranges
 * \sdd{WI-13856}
 */
TEST(StIntervalTest, WI_14876_Create_Float_Range__Min_shall_be_min_and_Max_shall_be_max)
{
   /** \arrange */
   const float val_1 = 10.f;
   const float val_2 = 13.f;

   /** \action call function under test */
   const Float_Range_T range = Create_Float_Range(val_1, val_2);

   /** \assert */
   EXPECT_FLOAT_EQ(range.max, Max(val_1, val_2));
   EXPECT_FLOAT_EQ(range.min, Min(val_1, val_2));
}

/**
 * The unit tests for the function to create float ranges
 * \sdd{WI-13856}
 */
TEST(StIntervalTest, WI_14877_Create_Float_Range__parameter_order_shall_be_irrelevant)
{
   /** \arrange */
   const float val_1 = 10.f;
   const float val_2 = 13.f;

   /** \action call function under test */
   const Float_Range_T range_1 = Create_Float_Range(val_1, val_2);
   const Float_Range_T range_2 = Create_Float_Range(val_2, val_1);

   /** \assert */
   EXPECT_FLOAT_RANGE_EQ(range_1, range_2);
}

/**
 * The unit tests for the function to create float ranges
 * \sdd{WI-13855}
 */
TEST(StIntervalTest, WI_14878_Create_Int_Range__Min_shall_be_min_and_Max_shall_be_max)
{
   /** \arrange */
   const int val_1 = 10;
   const int val_2 = 13;

   /** \action call function under test */
   const Int_Range_T range = Create_Int_Range(val_1, val_2);

   /** \assert */
   EXPECT_EQ(range.max, Max(val_1, val_2));
   EXPECT_EQ(range.min, Min(val_1, val_2));
}

/**
 * The unit tests for the function to create float ranges
 * \sdd{WI-13855}
 */
TEST(StIntervalTest, WI_14879_Create_Int_Range__parameter_order_shall_be_irrelevant)
{
   /** \arrange */
   const int val_1 = 10;
   const int val_2 = 13;

   /** \action call function under test */
   const Int_Range_T range_1 = Create_Int_Range(val_1, val_2);
   const Int_Range_T range_2 = Create_Int_Range(val_2, val_1);

   /** \assert */
   EXPECT_INT_RANGE_EQ(range_1, range_2);
}


/**
 * Initialize a float range with +/-infinity.
 * \sdd{WI-13858}
 */
TEST(StIntervalTest, WI_16186_Init_Float_Range__Test)
{
   /** \arrange initialize the float range with zero */
   Float_Range_T float_range = {};

   /** \action call function under test */
   float_range = Init_Float_Range();

   /** \assert float range is initialized with +/-infinity */
   EXPECT_FLOAT_EQ(float_range.min, AS_TOOLBOX_INFINITY);
   EXPECT_FLOAT_EQ(float_range.max, -AS_TOOLBOX_INFINITY);
}


/**
 * The unit test for the function to extend a float range.
 * \sdd{WI-13841}
 */
TEST(StIntervalTest, WI_15610_Extend_Float_Range__Test)
{
   /** \arrange */
   Float_Range_T float_range_changed = Init_Float_Range();

   /** \action call function under test */
   // Any Extension to this range shall change both min and max
   Extend_Float_Range(&float_range_changed, 0.0f);

   /** \assert */
   EXPECT_FLOAT_EQ(float_range_changed.max, 0.0f);
   EXPECT_FLOAT_EQ(float_range_changed.min, 0.0f);
}


/**
 * Initialize an integer range with int32 max/min values.
 * \sdd{WI-13857}
 */
TEST(StIntervalTest, WI_16187_Init_Int_Range__Test)
{
   /** \arrange initialize the integer range with zero */
   Int_Range_T int_range = {};

   /** \action call function under test */
   int_range = Init_Int_Range();

   /** \assert
    * integer range minimum is initialized with int32 max value and
    * integer range maximum is initialized with int32 min value
    */
   EXPECT_EQ(int_range.min, INT32_MAX);
   EXPECT_EQ(int_range.max, INT32_MIN);
}


/**
 * The unit test for the function to extend an integer range.
 * \sdd{WI-13840}
 */
TEST(StIntervalTest, WI_15611_Extend_Int_Range__Test)
{
   /** \arrange */
   Int_Range_T int_range_changed = Init_Int_Range();

   /** \action call function under test */
   // Any Extension to this range shall change both min and max
   Extend_Int_Range(&int_range_changed, 0);

   /** \assert */
   EXPECT_EQ(int_range_changed.max, 0);
   EXPECT_EQ(int_range_changed.min, 0);
}


/**
 * The unit tests for the function which checks wether a float number is contained in a float range or not
 * \sdd{WI-13847}
 */
TEST(StIntervalTest, WI_14882_Is_Float_Contained_In_Float_Range__test_in_range_value)
{
   /** \arrange */
   Float_Range_T float_range = Create_Float_Range(-25.3f, 4.f);
   float         test_value  = 3;

   /** \action call function under test */
   const boolean_T result = Is_Float_Contained_In_Float_Range(test_value, &float_range);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
 * The unit tests for the function which checks wether a float number is contained in a float range or not
 * \sdd{WI-13847}
 */
TEST(StIntervalTest, WI_14883_Is_Float_Contained_In_Float_Range__test_out_range_value_high)
{
   /** \arrange */
   Float_Range_T float_range = Create_Float_Range(-25.3f, 4.f);
   float         test_value  = 10;

   /** \action call function under test */
   const boolean_T result = Is_Float_Contained_In_Float_Range(test_value, &float_range);

   /** \assert */
   EXPECT_FALSE(result);
}

/**
 * The unit tests for the function which checks wether a float number is contained in a float range or not
 * \sdd{WI-13847}
 */
TEST(StIntervalTest, WI_14884_Is_Float_Contained_In_Float_Range__test_out_range_value_low)
{
   /** \arrange */
   Float_Range_T float_range = Create_Float_Range(-25.3f, 4.f);
   float         test_value  = -110;

   /** \action call function under test */
   const boolean_T result = Is_Float_Contained_In_Float_Range(test_value, &float_range);

   /** \assert */
   EXPECT_FALSE(result);
}


/**
 * The unit tests for the function which checks wether a int number is contained in a float range or not
 * \sdd{WI-13846}
 */
TEST(StIntervalTest, WI_14885_Is_Int_Contained_In_Float_Range__test_in_range_value)
{
   /** \arrange */
   Float_Range_T float_range = Create_Float_Range(-25.3f, 4.f);
   int           test_value  = 3;

   /** \action call function under test */
   const boolean_T result = Is_Int_Contained_In_Float_Range(test_value, &float_range);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
 * The unit tests for the function which checks wether a int number is contained in a float range or not
 * \sdd{WI-13846}
 */
TEST(StIntervalTest, WI_14886_Is_Int_Contained_In_Float_Range__test_out_range_value_high)
{
   /** \arrange */
   Float_Range_T float_range = Create_Float_Range(-25.3f, 4.f);
   int           test_value  = 10;

   /** \action call function under test */
   const boolean_T result = Is_Int_Contained_In_Float_Range(test_value, &float_range);

   /** \assert */
   EXPECT_FALSE(result);
}

/**
 * The unit tests for the function which checks wether a int number is contained in a float range or not
 * \sdd{WI-13846}
 */
TEST(StIntervalTest, WI_14887_Is_Int_Contained_In_Float_Range__test_out_range_value_low)
{
   /** \arrange */
   Float_Range_T float_range = Create_Float_Range(-25.3f, 4.f);
   int           test_value  = -110;

   /** \action call function under test */
   const boolean_T result = Is_Int_Contained_In_Float_Range(test_value, &float_range);

   /** \assert */
   EXPECT_FALSE(result);
}


/**
 * The unit tests for the function which checks wether a float number is contained in a int range or not
 * \sdd{WI-13845}
 */
TEST(StIntervalTest, WI_14888_Is_Float_Contained_In_Int_Range__test_in_range_value)
{
   /** \arrange */
   Int_Range_T in_range   = Create_Int_Range(-25, 4);
   float       test_value = 3;

   /** \action call function under test */
   const boolean_T result = Is_Float_Contained_In_Int_Range(test_value, &in_range);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
 * The unit tests for the function which checks wether a float number is contained in a int range or not
 * \sdd{WI-13845}
 */
TEST(StIntervalTest, WI_14889_Is_Float_Contained_In_Int_Range__test_out_range_value_high)
{
   /** \arrange */
   Int_Range_T in_range   = Create_Int_Range(-25, 4);
   float       test_value = 10;

   /** \action call function under test */
   const boolean_T result = Is_Float_Contained_In_Int_Range(test_value, &in_range);

   /** \assert */
   EXPECT_FALSE(result);
}

/**
 * The unit tests for the function which checks wether a float number is contained in a int range or not
 * \sdd{WI-13845}
 */
TEST(StIntervalTest, WI_14890_Is_Float_Contained_In_Int_Range__test_out_range_value_low)
{
   /** \arrange */
   Int_Range_T in_range   = Create_Int_Range(-25, 4);
   float       test_value = -110;

   /** \action call function under test */
   const boolean_T result = Is_Float_Contained_In_Int_Range(test_value, &in_range);

   /** \assert */
   EXPECT_FALSE(result);
}



/**
 * The unit tests for the function which checks wether a int number is contained in a int range or not
 * \sdd{WI-13844}
 */
TEST(StIntervalTest, WI_14891_Is_Int_Contained_In_Int_Range__test_in_range_value)
{
   /** \arrange */
   Int_Range_T in_range   = Create_Int_Range(-25, 4);
   int         test_value = 3;

   /** \action call function under test */
   const boolean_T result = Is_Int_Contained_In_Int_Range(test_value, &in_range);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
 * The unit tests for the function which checks wether a int number is contained in a int range or not
 * \sdd{WI-13844}
 */
TEST(StIntervalTest, WI_14892_Is_Int_Contained_In_Int_Range__test_out_range_value_high)
{
   /** \arrange */
   Int_Range_T in_range   = Create_Int_Range(-25, 4);
   int         test_value = 10;

   /** \action call function under test */
   const boolean_T result = Is_Int_Contained_In_Int_Range(test_value, &in_range);

   /** \assert */
   EXPECT_FALSE(result);
}

/**
 * The unit tests for the function which checks wether a int number is contained in a int range or not
 * \sdd{WI-13844}
 */
TEST(StIntervalTest, WI_14893_Is_Int_Contained_In_Int_Range__test_out_range_value_low)
{
   /** \arrange */
   Int_Range_T in_range   = Create_Int_Range(-25, 4);
   int         test_value = -110;

   /** \action call function under test */
   const boolean_T result = Is_Int_Contained_In_Int_Range(test_value, &in_range);

   /** \assert */
   EXPECT_FALSE(result);
}



/**
 * The unit tests for the function which checks if 2 int ranges overlap
 * \sdd{WI-13850}
 */
TEST(StIntervalTest, WI_14894_Does_Int_Range_Overlap_Int_Range__test_no_overlap)
{
   /** \arrange */
   Int_Range_T int_range_a = Create_Int_Range(-25, 4);
   Int_Range_T int_range_b = Create_Int_Range(5, 6);

   /** \action call function under test */
   const boolean_T result = Does_Int_Range_Overlap_Int_Range(&int_range_a, &int_range_b);

   /** \assert */
   EXPECT_FALSE(result);
}

/**
 * The unit tests for the function which checks if 2 int ranges overlap
 * \sdd{WI-13850}
 */
TEST(StIntervalTest, WI_14895_Does_Int_Range_Overlap_Int_Range____test_overlap_on_lower_border)
{
   /** \arrange */
   Int_Range_T int_range_a = Create_Int_Range(-25, 4);
   Int_Range_T int_range_b = Create_Int_Range(3, 6);

   /** \action call function under test */
   const boolean_T result = Does_Int_Range_Overlap_Int_Range(&int_range_a, &int_range_b);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
 * The unit tests for the function which checks if 2 int ranges overlap
 * \sdd{WI-13850}
 */
TEST(StIntervalTest, WI_14896_Does_Int_Range_Overlap_Int_Range__test_overlap_on_lower_border_reversed)
{
   /** \arrange */
   Int_Range_T int_range_b = Create_Int_Range(-25, 4);
   Int_Range_T int_range_a = Create_Int_Range(3, 6);

   /** \action call function under test */
   const boolean_T result = Does_Int_Range_Overlap_Int_Range(&int_range_a, &int_range_b);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
 * The unit tests for the function which checks if 2 int ranges overlap
 * \sdd{WI-13850}
 */
TEST(StIntervalTest, WI_14897_Does_Int_Range_Overlap_Int_Range__test_overlap_on_upper_border)
{
   /** \arrange */
   Int_Range_T int_range_a = Create_Int_Range(3, 6);
   Int_Range_T int_range_b = Create_Int_Range(-25, 4);

   /** \action call function under test */
   const boolean_T result = Does_Int_Range_Overlap_Int_Range(&int_range_a, &int_range_b);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
 * The unit tests for the function which checks if 2 int ranges overlap
 * \sdd{WI-13850}
 */
TEST(StIntervalTest, WI_14898_Does_Int_Range_Overlap_Int_Range__test_overlap_on_upper_border_reversed)
{
   /** \arrange */
   Int_Range_T int_range_b = Create_Int_Range(3, 6);
   Int_Range_T int_range_a = Create_Int_Range(-25, 4);

   /** \action call function under test */
   const boolean_T result = Does_Int_Range_Overlap_Int_Range(&int_range_a, &int_range_b);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
 * The unit tests for the function which checks if 2 int ranges overlap
 * \sdd{WI-13850}
 */
TEST(StIntervalTest, WI_14899_Does_Int_Range_Overlap_Int_Range__test_a_within_b)
{
   /** \arrange */
   Int_Range_T int_range_a = Create_Int_Range(3, 6);
   Int_Range_T int_range_b = Create_Int_Range(-25, 8);

   /** \action call function under test */
   const boolean_T result = Does_Int_Range_Overlap_Int_Range(&int_range_a, &int_range_b);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
 * The unit tests for the function which checks if 2 int ranges overlap
 * \sdd{WI-13850}
 */
TEST(StIntervalTest, WI_14900_Does_Int_Range_Overlap_Int_Range__test_b_completely_within_a)
{
   /** \arrange */
   Int_Range_T int_range_b = Create_Int_Range(3, 6);
   Int_Range_T int_range_a = Create_Int_Range(-25, 8);

   /** \action call function under test */
   const boolean_T result = Does_Int_Range_Overlap_Int_Range(&int_range_a, &int_range_b);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
 * \sdd{WI-13848}
 */
TEST(StIntervalTest, WI_14901_Does_Float_Range_Overlap_Float_Range__test_no_overlap_lower_border)
{
   /** \arrange */
   Float_Range_T float_range_a = Create_Float_Range(-25.0f, 4.0f);
   Float_Range_T float_range_b = Create_Float_Range(-30.0f, -26.0f);

   /** \action call function under test */
   const boolean_T result = Does_Float_Range_Overlap_Float_Range(&float_range_a, &float_range_b);

   /** \assert */
   EXPECT_FALSE(result);
}

/**
 * \sdd{WI-13848}
 */
TEST(StIntervalTest, WI_14902_Does_Float_Range_Overlap_Float_Range__test_no_overlap_upper_border)
{
   /** \arrange */
   Float_Range_T float_range_a = Create_Float_Range(-25.0f, 4.0f);
   Float_Range_T float_range_b = Create_Float_Range(5.0f, 6.0f);

   /** \action call function under test */
   const boolean_T result = Does_Float_Range_Overlap_Float_Range(&float_range_a, &float_range_b);

   /** \assert */
   EXPECT_FALSE(result);
}

/**
 * \sdd{WI-13848}
 */
TEST(StIntervalTest, WI_14903_Does_Float_Range_Overlap_Float_Range__test_partial_overlap_upper_border)
{
   /** \arrange */
   Float_Range_T float_range_a = Create_Float_Range(-25.0f, 4.0f);
   Float_Range_T float_range_b = Create_Float_Range(3.0f, 6.0f);

   /** \action call function under test */
   const boolean_T result = Does_Float_Range_Overlap_Float_Range(&float_range_a, &float_range_b);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
 * \sdd{WI-13848}
 */
TEST(StIntervalTest, WI_14904_Does_Float_Range_Overlap_Float_Range__test_a_within_b)
{
   /** \arrange */
   Float_Range_T float_range_a = Create_Float_Range(-25.0f, 4.0f);
   Float_Range_T float_range_b = Create_Float_Range(-30.0f, 6.0f);

   /** \action call function under test */
   const boolean_T result = Does_Float_Range_Overlap_Float_Range(&float_range_a, &float_range_b);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
 * \sdd{WI-13848}
 */
TEST(StIntervalTest, WI_14905_Does_Float_Range_Overlap_Float_Range__test_b_completely_within_a)
{
   /** \arrange */
   Float_Range_T float_range_a = Create_Float_Range(-30.0f, 6.0f);
   Float_Range_T float_range_b = Create_Float_Range(-25.0f, 4.0f);

   /** \action call function under test */
   const boolean_T result = Does_Float_Range_Overlap_Float_Range(&float_range_a, &float_range_b);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
 * \sdd{WI-13848}
 */
TEST(StIntervalTest, WI_14906_Does_Float_Range_Overlap_Float_Range__test_partial_overlap_lower_border)
{
   /** \arrange */
   Float_Range_T float_range_a = Create_Float_Range(-25.0f, 4.0f);
   Float_Range_T float_range_b = Create_Float_Range(-30.0f, -24.0f);

   /** \action call function under test */
   const boolean_T result = Does_Float_Range_Overlap_Float_Range(&float_range_a, &float_range_b);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
 * \sdd{WI-13848}
 */
TEST(StIntervalTest, WI_14907_Does_Float_Range_Overlap_Float_Range__test_overlap_complete_match)
{
   /** \arrange */
   Float_Range_T float_range = Create_Float_Range(-25.0f, 4.0f);

   /** \action call function under test */
   const boolean_T result = Does_Float_Range_Overlap_Float_Range(&float_range, &float_range);

   /** \assert */
   EXPECT_TRUE(result);
}



/**
 * The unit tests for the function which checks wether a int range and a float range overlap
 * \sdd{WI-13842}
 */
TEST(StIntervalTest, WI_14908_Does_Float_Range_Overlap_Int_Range__test_no_overlap)
{
   /** \arrange */
   Float_Range_T float_range_a = Create_Float_Range(-25.1f, 4.1f);
   Int_Range_T   int_range_b   = Create_Int_Range(5, 6);

   /** \action call function under test */
   const boolean_T result = Does_Float_Range_Overlap_Int_Range(&float_range_a, &int_range_b);

   /** \assert */
   EXPECT_FALSE(result);
}

/**
 * The unit tests for the function which checks wether a int range and a float range overlap
 * \sdd{WI-13842}
 */
TEST(StIntervalTest, WI_14909_Does_Float_Range_Overlap_Int_Range__test_overlap_lower_border)
{
   /** \arrange */
   Float_Range_T float_range_a = Create_Float_Range(-25.1f, 4.1f);
   Int_Range_T   int_range_b   = Create_Int_Range(3, 6);

   /** \action call function under test */
   const boolean_T result = Does_Float_Range_Overlap_Int_Range(&float_range_a, &int_range_b);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
 * The unit tests for the function which checks wether a int range and a float range overlap
 * \sdd{WI-13842}
 */
TEST(StIntervalTest, WI_14910_Does_Float_Range_Overlap_Int_Range__test_overlap_upper_border)
{
   /** \arrange */
   Float_Range_T float_range_a = Create_Float_Range(3.0f, 6.0f);
   Int_Range_T   int_range_b   = Create_Int_Range(-25, 4);

   /** \action call function under test */
   const boolean_T result = Does_Float_Range_Overlap_Int_Range(&float_range_a, &int_range_b);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
 * The unit tests for the function which checks wether a int range and a float range overlap
 * \sdd{WI-13842}
 */
TEST(StIntervalTest, WI_14911_Does_Float_Range_Overlap_Int_Range__test_a_in_b)
{
   /** \arrange */
   Float_Range_T float_range_a = Create_Float_Range(3.0f, 6.0f);
   Int_Range_T   int_range_b   = Create_Int_Range(-25, 8);

   /** \action call function under test */
   const boolean_T result = Does_Float_Range_Overlap_Int_Range(&float_range_a, &int_range_b);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
 * The unit tests for the function which checks wether a int range and a float range overlap
 * \sdd{WI-13842}
 */
TEST(StIntervalTest, WI_14912_Does_Float_Range_Overlap_Int_Range__test_b_in_a)
{
   /** \arrange */
   Float_Range_T float_range_a = Create_Float_Range(-25.0f, 8.0f);
   Int_Range_T   int_range_b   = Create_Int_Range(3, 6);

   /** \action call function under test */
   const boolean_T result = Does_Float_Range_Overlap_Int_Range(&float_range_a, &int_range_b);

   /** \assert */
   EXPECT_TRUE(result);
}


/**
 * The unit tests for the function which extends a given range
 * \sdd{WI-13841}
 */
TEST(StIntervalTest, WI_14913_Extend_Float_Range__Test_extension_with_new_min)
{
   /** \arrange */
   Float_Range_T float_range = Create_Float_Range(-25.1f, 4.1f);
   float32_T     new_min     = -50.f;

   /** \action call function under test */
   Extend_Float_Range(&float_range, new_min);

   /** \assert */
   ASSERT_EQ(new_min, float_range.min);
}

/**
 * The unit tests for the function to init ranges
 * \sdd{WI-13841}
 */
TEST(StIntervalTest, WI_14914_Extend_Float_Range__Test_extension_with_new_max)
{
   /** \arrange */
   Float_Range_T float_range = Create_Float_Range(-25.1f, 4.1f);
   float32_T     new_max     = 50.f;

   /** \action call function under test */
   Extend_Float_Range(&float_range, new_max);

   /** \assert */
   ASSERT_EQ(new_max, float_range.max);
}

/**
 * The unit tests for the function to init ranges
 * \sdd{WI-13841}
 */
TEST(StIntervalTest, WI_14915_Extend_Float_Range__Test_extension_with_new_min_using_zero_length_range)
{
   /** \arrange */
   Float_Range_T float_range = Create_Float_Range(-25.1f, -25.1f);
   float32_T     new_min     = -50.f;

   /** \action call function under test */
   Extend_Float_Range(&float_range, new_min);

   /** \assert */
   EXPECT_EQ(new_min, float_range.min);
}

/**
 * The unit tests for the function to init ranges
 * \sdd{WI-13841}
 */
TEST(StIntervalTest, WI_14916_Extend_Float_Range__Test_extension_with_new_max_using_zero_length_range)
{
   /** \arrange */
   Float_Range_T float_range = Create_Float_Range(-25.1f, -25.1f);
   float32_T     new_max     = 50.f;

   /** \action call function under test */
   Extend_Float_Range(&float_range, new_max);

   /** \assert */
   EXPECT_EQ(new_max, float_range.max);
}


/**
 * The unit tests for the function which extends a given range
 * \sdd{WI-13840}
 */
TEST(StIntervalTest, WI_14917_Extend_Int_Range__Test_extension_with_new_min)
{
   /** \arrange */
   Int_Range_T int_range = Create_Int_Range(-25, 4);
   int32_t     new_min   = -50;

   /** \action call function under test */
   Extend_Int_Range(&int_range, new_min);

   /** \assert */
   EXPECT_EQ(new_min, int_range.min);
}

/**
 * The unit tests for the function which extends a given range
 * \sdd{WI-13840}
 */
TEST(StIntervalTest, WI_14918_Extend_Int_Range__Test_extension_with_new_max)
{
   /** \arrange */
   Int_Range_T int_range = Create_Int_Range(-25, 4);
   int32_t     new_max   = 50;

   /** \action call function under test */
   Extend_Int_Range(&int_range, new_max);

   /** \assert */
   EXPECT_EQ(new_max, int_range.max);
}


/**
 * The unit tests for the function to get the nearest value in range
 * \sdd{WI-13852}
 */
TEST(StIntervalTest, WI_14919_Enforce_Range__test_value_in_range)
{
   /** \arrange */
   float32_T min_value = -1.1f;
   float32_T max_value = 5.75f;

   float32_T value_to_force = PI;

   /** \action call function under test */
   const float32_T result = Enforce_Range(value_to_force, min_value, max_value);

   /** \assert */
   EXPECT_EQ(value_to_force, result);
}

/**
 * The unit tests for the function to get the nearest value in range
 * \sdd{WI-13852}
 */
TEST(StIntervalTest, WI_14920_Enforce_Range__test_value_below_range)
{
   /** \arrange */
   float32_T min_value = -1.1f;
   float32_T max_value = 5.75f;

   float32_T value_to_force = -PI;

   /** \action call function under test */
   const float32_T result = Enforce_Range(value_to_force, min_value, max_value);

   /** \assert */
   EXPECT_EQ(min_value, result);
}

/**
 * The unit tests for the function to get the nearest value in range
 * \sdd{WI-13852}
 */
TEST(StIntervalTest, WI_14921_Enforce_Range__test_value_above_range)
{
   /** \arrange */
   float32_T min_value = -1.1f;
   float32_T max_value = 5.75f;

   float32_T value_to_force = 7.5f;

   /** \action call function under test */
   const float32_T result = Enforce_Range(value_to_force, min_value, max_value);

   /** \assert */
   EXPECT_EQ(max_value, result);
}


/**
 * \sdd{WI-13849}
 */
TEST(StIntervalTest, WI_14922_Is_Int_Interval_Subset_Of_Int_Interval_test__interval_a_should_be_a_part_of_interval_b)
{
   /** \arrange */
   Int_Range_T int_interval_a = Create_Int_Range(-8, 3);
   Int_Range_T int_interval_b = Create_Int_Range(-10, 10);

   /** \action call function under test */
   const boolean_T result = Is_Int_Interval_Subset_Of_Int_Interval(&int_interval_a, &int_interval_b);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
 * \sdd{WI-13849}
 */
TEST(StIntervalTest, WI_14923_Is_Int_Interval_Subset_Of_Int_Interval_test__interval_a_should_exceed_interval_b)
{
   /** \arrange */
   Int_Range_T int_interval_a = Create_Int_Range(-10, 10);
   Int_Range_T int_interval_b = Create_Int_Range(-8, 3);

   /** \action call function under test */
   const boolean_T result = Is_Int_Interval_Subset_Of_Int_Interval(&int_interval_a, &int_interval_b);

   /** \assert */
   EXPECT_FALSE(result);
}


/**
 * \sdd{WI-13849}
 */
TEST(StIntervalTest, WI_14924_Is_Int_Interval_Subset_Of_Int_Interval_test__interval_a_below_range_b_should_not_be_a_subset_of_interval_b)
{
   /** \arrange */
   Int_Range_T int_interval_a = Create_Int_Range(-10, 10);
   Int_Range_T int_interval_b = Create_Int_Range(20, 50);

   /** \action call function under test */
   const boolean_T result = Is_Int_Interval_Subset_Of_Int_Interval(&int_interval_a, &int_interval_b);

   /** \assert */
   EXPECT_FALSE(result);
}

/**
 * \sdd{WI-13849}
 */
TEST(StIntervalTest, WI_14925_Is_Int_Interval_Subset_Of_Int_Interval_test__interval_a_above_range_b_should_not_be_a_subset_of_interval_b)
{
   /** \arrange */
   Int_Range_T int_interval_a = Create_Int_Range(20, 50);
   Int_Range_T int_interval_b = Create_Int_Range(-10, 10);

   /** \action call function under test */
   const boolean_T result = Is_Int_Interval_Subset_Of_Int_Interval(&int_interval_a, &int_interval_b);

   /** \assert */
   EXPECT_FALSE(result);
}

/**
 * \sdd{WI-13849}
 */
TEST(StIntervalTest, WI_14926_Is_Int_Interval_Subset_Of_Int_Interval_test__zero_interval_a_should_be_a_subset_of_interval_b)
{
   /** \arrange */
   Int_Range_T int_interval_a = Create_Int_Range(0, 0);
   Int_Range_T int_interval_b = Create_Int_Range(-3, 3);

   /** \action call function under test */
   const boolean_T result = Is_Int_Interval_Subset_Of_Int_Interval(&int_interval_a, &int_interval_b);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
 * \sdd{WI-13849}
 */
TEST(StIntervalTest, WI_14927_Is_Int_Interval_Subset_Of_Int_Interval_test__zero_interval_a_should_not_be_a_subset_of_interval_b)
{
   /** \arrange */
   Int_Range_T int_interval_a = Create_Int_Range(0, 0);
   Int_Range_T int_interval_b = Create_Int_Range(3, 10);

   /** \action call function under test */
   const boolean_T result = Is_Int_Interval_Subset_Of_Int_Interval(&int_interval_a, &int_interval_b);

   /** \assert */
   EXPECT_FALSE(result);
}


/**
 * \sdd{WI-13843}
 */
TEST(StIntervalTest, WI_14928_Is_Float_Interval_Subset_Of_Float_Interval_test__interval_a_should_be_a_subset_of_interval_b)
{
   /** \arrange */
   Float_Range_T float_interval_a = Create_Float_Range(-8, 3);
   Float_Range_T float_interval_b = Create_Float_Range(-10, 10);

   /** \action call function under test */
   const boolean_T result = Is_Float_Interval_Subset_Of_Float_Interval(&float_interval_a, &float_interval_b);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
 * \sdd{WI-13843}
 */
TEST(StIntervalTest, WI_14929_Is_Float_Interval_Subset_Of_Float_Interval_test__interval_a_should_exceed_interval_b)
{
   /** \arrange */
   Float_Range_T float_interval_a = Create_Float_Range(-10, 10);
   Float_Range_T float_interval_b = Create_Float_Range(-8, 3);

   /** \action call function under test */
   const boolean_T result = Is_Float_Interval_Subset_Of_Float_Interval(&float_interval_a, &float_interval_b);

   /** \assert */
   EXPECT_FALSE(result);
}

/**
 * \sdd{WI-13843}
 */
TEST(StIntervalTest, WI_14930_Is_Float_Interval_Subset_Of_Float_Interval_test__interval_a_below_range_b_should_not_be_a_subset_of_interval_b)
{
   /** \arrange */
   Float_Range_T float_interval_a = Create_Float_Range(-10, 10);
   Float_Range_T float_interval_b = Create_Float_Range(20, 50);

   /** \action call function under test */
   const boolean_T result = Is_Float_Interval_Subset_Of_Float_Interval(&float_interval_a, &float_interval_b);

   /** \assert */
   EXPECT_FALSE(result);
}

/**
 * \sdd{WI-13843}
 */
TEST(StIntervalTest, WI_14931_Is_Float_Interval_Subset_Of_Float_Interval_test__interval_a_above_range_b_should_not_be_a_subset_of_interval_b)
{
   /** \arrange */
   Float_Range_T float_interval_a = Create_Float_Range(20, 50);
   Float_Range_T float_interval_b = Create_Float_Range(-10, 10);

   /** \action call function under test */
   const boolean_T result = Is_Float_Interval_Subset_Of_Float_Interval(&float_interval_a, &float_interval_b);

   /** \assert */
   EXPECT_FALSE(result);
}

/**
 * \sdd{WI-13843}
 */
TEST(StIntervalTest, WI_14932_Is_Float_Interval_Subset_Of_Float_Interval_test__zero_interval_a_should_be_a_subset_of_interval_b)
{
   /** \arrange */
   Float_Range_T float_interval_a = Create_Float_Range(0, 0);
   Float_Range_T float_interval_b = Create_Float_Range(-3, 3);

   /** \action call function under test */
   const boolean_T result = Is_Float_Interval_Subset_Of_Float_Interval(&float_interval_a, &float_interval_b);

   /** \assert */
   EXPECT_TRUE(result);
}

/**
 * \sdd{WI-13843}
 */
TEST(StIntervalTest, WI_14933_Is_Float_Interval_Subset_Of_Float_Interval_test__zero_interval_a_should_not_be_a_subset_of_interval_b)
{
   /** \arrange */
   Float_Range_T float_interval_a = Create_Float_Range(0, 0);
   Float_Range_T float_interval_b = Create_Float_Range(3, 10);

   /** \action call function under test */
   const boolean_T result = Is_Float_Interval_Subset_Of_Float_Interval(&float_interval_a, &float_interval_b);

   /** \assert */
   EXPECT_FALSE(result);
}


/**
 * A value below the threshold is not changed
 * \sdd{WI-13851}
 */
TEST(Force_None_Zero, WI_14934_outside_range_below)
{
   /** \arrange choose an arbitrary positive non zero threshold */
   float threshold = 1.0f;
   /** \arrange choose a value below the negative threshold */
   float value = -10.0f;
   /** \action call function under test */
   float ret = Enforce_Nonzero(value, threshold);

   /** \assert expect the value to not be changed */
   EXPECT_EQ(ret, value);
}

/**
 * A value above the threshold is not changed
 * \sdd{WI-13851}
 */
TEST(Force_None_Zero, WI_14935_outside_range_above)
{
   /** \arrange choose an arbitrary positive non zero threshold */
   float threshold = 1.0f;
   /** \arrange choose a value above the positive threshold */
   float value = -10.0f;
   /** \action call function under test */
   float ret = Enforce_Nonzero(value, threshold);

   /** \assert expect the value to not be changed */
   EXPECT_EQ(ret, value);
}

/**
 * A value between negative threshold and zero is clipped to negative threshold
 * \sdd{WI-13851}
 */
TEST(Force_None_Zero, WI_14936_inside_range_below_zero)
{
   /** \arrange choose an arbitrary positive non zero threshold */
   float threshold = 1.0f;
   /** \arrange choose a value between negative threshold and zero*/
   float value = -0.5f;
   /** \action call function under test */
   float ret = Enforce_Nonzero(value, threshold);

   /** \assert expect the result to be clipped to negative threshold */
   EXPECT_EQ(ret, -1.0f);
}

/**
 * A value between positive threshold and zero is clipped to positive threshold
 * \sdd{WI-13851}
 */
TEST(Force_None_Zero, WI_14937_inside_range_above_zero)
{
   /** \arrange choose an arbitrary positive non zero threshold */
   float threshold = 1.0f;
   /** \arrange choose a value between positive threshold and zero*/
   float value = 0.5f;
   /** \action call function under test */
   float ret = Enforce_Nonzero(value, threshold);

   /** \assert expect the result to be clipped to positive threshold */
   EXPECT_EQ(ret, 1.0f);
}

/**
 * A value of zero is clipped to negative threshold
 * \sdd{WI-13851}
 */
TEST(Force_None_Zero, WI_14938_inside_range_equal_zero)
{
   /** \arrange choose an arbitrary positive non zero threshold */
   float threshold = 1.0f;
   /** \arrange choose a value between positive threshold and zero*/
   float value = 0.0f;
   /** \action call function under test */
   float ret = Enforce_Nonzero(value, threshold);

   /** \assert expect the result to be clipped to negative threshold */
   EXPECT_EQ(ret, -1.0f);
}

/**
 * \sdd{WI-13853}
 */
TEST(Is_Float_Within_Tolerance, WI_14939_below_min)
{
   boolean_T ret = Is_Float_Within_Tolerance(-10.0f, -8.0f, 1.0f);

   EXPECT_FALSE(ret);
}

/**
 * \sdd{WI-13853}
 */
TEST(Is_Float_Within_Tolerance, WI_14940_above_max)
{
   boolean_T ret = Is_Float_Within_Tolerance(-6.0f, -8.0f, 1.0f);

   EXPECT_FALSE(ret);
}

/**
 * \sdd{WI-13853}
 */
TEST(Is_Float_Within_Tolerance, WI_14941_on_lower_border)
{
   boolean_T ret = Is_Float_Within_Tolerance(-9.0f, -8.0f, 1.0f);

   EXPECT_TRUE(ret);
}

/**
 * \sdd{WI-13853}
 */
TEST(Is_Float_Within_Tolerance, WI_14942_on_upper_border)
{
   boolean_T ret = Is_Float_Within_Tolerance(-7.0f, -8.0f, 1.0f);

   EXPECT_TRUE(ret);
}

/**
 * \sdd{WI-13853}
 */
TEST(Is_Float_Within_Tolerance, WI_14943_within)
{
   boolean_T ret = Is_Float_Within_Tolerance(-8.0f, -8.0f, 1.0f);

   EXPECT_TRUE(ret);
}

/**
 * \sdd{WI-13854}
 */
TEST(Is_Int_Within_Tolerance, WI_14939_below_min)
{
   boolean_T ret = Is_Int_Within_Tolerance(-10, -8, 1);

   EXPECT_FALSE(ret);
}

/**
 * \sdd{WI-13854}
 */
TEST(Is_Int_Within_Tolerance, WI_14940_above_max)
{
   boolean_T ret = Is_Int_Within_Tolerance(-6, -8, 1);

   EXPECT_FALSE(ret);
}

TEST(Is_Int_Within_Tolerance, WI_14941_on_lower_border)
{
   boolean_T ret = Is_Int_Within_Tolerance(-9, -8, 1);

   EXPECT_TRUE(ret);
}

/**
 * \sdd{WI-13854}
 */
TEST(Is_Int_Within_Tolerance, WI_14942_on_upper_border)
{
   boolean_T ret = Is_Int_Within_Tolerance(-7, -8, 1);

   EXPECT_TRUE(ret);
}

/**
 * \sdd{WI-13854}
 */
TEST(Is_Int_Within_Tolerance, WI_14943_within)
{
   boolean_T ret = Is_Int_Within_Tolerance(-8, -8, 1);

   EXPECT_TRUE(ret);
}

