/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <gtest/gtest.h>

#include "reuse.h"

#include <array>
#include "ml_macros.h"

#pragma warning(disable : 4756)
#pragma warning(disable : 4056)

/**
 * Test the swap macro
 * \sdd{WI-13996}
 */
TEST(BasicMarcosTest, WI_14996_Swap_float_Test)
{
   /** \arrange */
   const float a_orig = -1.0f;
   const float b_orig = 1.0f;
   float a = a_orig;
   float b = b_orig;

   /** \action call function under test */
   Swap(a, b, float);

   /** \assert */

   EXPECT_EQ(a, b_orig);
   EXPECT_EQ(b, a_orig);
}

/**
 * Test the swap macro
 * \sdd{WI-13996}
 */
TEST(BasicMarcosTest, WI_14997_Swap_float_pointer_Test)
{
   /** \arrange */
   float a_orig = -1.0f;
   float b_orig = 1.0f;
   float *a = &a_orig;
   float *b = &b_orig;

   /** \action call function under test */
   Swap(a, b, float *);

   /** \assert */

   EXPECT_EQ(*a, b_orig);
   EXPECT_EQ(*b, a_orig);
}
/**
 * Test the swap macro
 * \sdd{WI-13996}
 */
TEST(BasicMarcosTest, WI_14998_Swap_int_Test)
{
   /** \arrange */
   const int a_orig = -1;
   const int b_orig = 1;
   int a = a_orig;
   int b = b_orig;

   /** \action call function under test */
   Swap(a, b, int);

   /** \assert */

   EXPECT_EQ(a, b_orig);
   EXPECT_EQ(b, a_orig);
}

/**
 * Test the swap macro
 * \sdd{WI-13996}
 */
TEST(BasicMarcosTest, WI_14999_Swap_int_pointer_Test)
{
   /** \arrange */
   int a_orig = -1;
   int b_orig = 1;
   int *a = &a_orig;
   int *b = &b_orig;

   /** \action call function under test */
   Swap(a, b, int *);

   /** \assert */

   EXPECT_EQ(*a, b_orig);
   EXPECT_EQ(*b, a_orig);
}

/**
 * Test the swap macro
 * \sdd{WI-13996}
 */
TEST(BasicMarcosTest, WI_15000_Swap_int_array_Test)
{
   /** \arrange */
   std::array<int, 2> orig = {-1, 1};
   std::array<int, 2> swapped = {-1, 1};

   /** \action call function under test */
   Swap(swapped[0], swapped[1], int);

   /** \assert */

   EXPECT_EQ(swapped[1], orig[0]);
   EXPECT_EQ(swapped[0], orig[1]);
}
/**
 * Test the swap macro
 * \sdd{WI-13996}
 */
TEST(BasicMarcosTest, WI_15001_Swap_float_array_Test)
{
   /** \arrange */
   std::array<float, 2> orig = {-1, 1};
   std::array<float, 2> swapped = {-1, 1};

   /** \action call function under test */
   Swap(swapped[0], swapped[1], float);

   /** \assert */

   EXPECT_EQ(swapped[1], orig[0]);
   EXPECT_EQ(swapped[0], orig[1]);
}

/**
 * Test the swap macro
 * \sdd{WI-13996}
 */
TEST(BasicMarcosTest, WI_15002_Swap_structure_Test)
{
   /** \arrange */
   struct Test_Struct_T
   {
      float m_tst_flt;
      int m_tst_int;
   };
   Test_Struct_T a_orig{
      /*.m_tst_flt = */-1.0f,
      /*.m_tst_int = */-1};
   Test_Struct_T b_orig{
	   /*.m_tst_flt = */1.0f,
		/*.m_tst_int = */1
   };
   Test_Struct_T a{a_orig};
   Test_Struct_T b{b_orig};


   /** \action call function under test */
   Swap(a, b, Test_Struct_T);

   /** \assert */

   EXPECT_EQ(a.m_tst_flt, b_orig.m_tst_flt);
   EXPECT_EQ(a.m_tst_int, b_orig.m_tst_int);
   EXPECT_EQ(b.m_tst_flt, a_orig.m_tst_flt);
   EXPECT_EQ(b.m_tst_int, a_orig.m_tst_int);
}
