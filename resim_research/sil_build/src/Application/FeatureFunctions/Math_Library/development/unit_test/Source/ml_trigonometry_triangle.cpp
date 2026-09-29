/*===============================================================================*\
 * Copyright 2022 Aptiv Advanced Safety and User Experience. All rights reserved *
 * Confidential - Restricted Aptiv information. Do not disclose.                 *
\*===============================================================================*/
#include "gtest/gtest.h"

extern "C" {
#include "ml_trigonometry.h"
}

/**
 * Compute an angle of a right angled triangle
 */
TEST(MlTrigonometry, Triangle_Alpha_From_Abc_computes)
{
    /** \arrange prepare sides of a right angled triangle */
   float a = 5;
   float b = 5;
   float c = Fast_Sqrt(a*a + b*b);
   /** \action call function under test */
   float ret = Triangle_Alpha_From_Abc(b, c, a);
   /** \assert Expect pi/4 to be returned */
   EXPECT_LE(ret - PI / 4, 0.0002f); /* FAST_ACOS does have quite low accuracy */
}

/**
 * Compute an angle of a right angled triangle
 */
TEST(MlTrigonometry, Triangle_Beta_From_Abc_computes)
{
    /** \arrange prepare sides of a right angled triangle */
   float a = 5;
   float b = 5;
   float c = Fast_Sqrt(a*a + b*b);
   /** \action call function under test */
   float ret = Triangle_Beta_From_Abc(c, a, b);
   /** \assert Expect pi/4 to be returned */
   EXPECT_LE(ret - PI / 4, 0.0002f); /* FAST_ACOS does have quite low accuracy */
}

/**
 * Compute an angle of a right angled triangle
 */
TEST(MlTrigonometry, Triangle_Gamma_From_Abc_computes)
{
    /** \arrange prepare sides of a right angled triangle */
   float a = 5;
   float b = 5;
   float c = Fast_Sqrt(a*a + b*b);
   /** \action call function under test */
   float ret = Triangle_Gamma_From_Abc(a, b, c);
   /** \assert Expect pi/2 to be returned */
   EXPECT_FLOAT_EQ(ret, PI / 2);
}

/**
 * Compute an angle of a degenerated triangle
 */
TEST(MlTrigonometry, Triangle_Alpha_From_Abc_computes_degenerated)
{
    /** \prepare sides of a degenerated triangle */
   float a = 5;
   float b = 5;
   float c = a + b;
   /** \action call function under test */
   float ret = Triangle_Alpha_From_Abc(a,b,c);
   /** \assert Expect pi to be returned */
   EXPECT_FLOAT_EQ(ret, 0.0f);
}

/**
 * Compute an angle of a degenerated triangle
 */
TEST(MlTrigonometry, Triangle_Beta_From_Abc_computes_degenerated1)
{
    /** \prepare sides of a degenerated triangle */
   float a = 5;
   float b = 5;
   float c = a + b;
   /** \action call function under test */
   float ret = Triangle_Beta_From_Abc(a, b, c);
   /** \assert Expect pi to be returned */
   EXPECT_FLOAT_EQ(ret, 0.0f);
}

/**
 * Compute an angle of a degenerated triangle
 */
TEST(MlTrigonometry, Triangle_Gamma_From_Abc_computes_degenerated)
{
    /** \prepare sides of a degenerated triangle */
   float a = 5;
   float b = 5;
   float c = a + b;
   /** \action call function under test */
   float ret = Triangle_Gamma_From_Abc(a,b,c);
   /** \assert Expect pi to be returned */
   EXPECT_FLOAT_EQ(ret, PI);
}

/**
 * Compute an angle of a non-triangle
 */
TEST(MlTrigonometry, Triangle_Alpha_From_Abc_not_a_triangle)
{
    /** \prepare sides that do not form a triangle */
   float a = 5;
   float b = 5;
   float c = a + b + 2;
   float ret;
   EXPECT_DEBUG_DEATH({
      /** \action call function under test */
      ret = Triangle_Alpha_From_Abc(a, b, c);
      /** \assert helpful assertion is thrown */
   }, ">= adjacent_right");
#ifdef NDEBUG
   EXPECT_EQ(0.0f, ret);
#endif
}

/**
 * Compute an angle of a non-triangle
 */
TEST(MlTrigonometry, Triangle_Beta_From_Abc_not_a_triangle)
{
    /** \prepare sides that do not form a triangle */
   float a = 5;
   float b = 5;
   float c = a + b + 2;
   float ret;
   EXPECT_DEBUG_DEATH({
      /** \action call function under test */
      ret = Triangle_Beta_From_Abc(a, b, c);
      /** \assert helpful assertion is thrown */
   }, ">= adjacent_left");
#ifdef NDEBUG
   EXPECT_EQ(0.0f, ret);
#endif
}

/**
 * Compute an angle of a non-triangle
 */
TEST(MlTrigonometry, Triangle_Gamma_From_Abc_not_a_triangle)
{
    /** \prepare sides that do not form a triangle */
   float a = 5;
   float b = 5;
   float c = a + b + 2;
   float ret;
   EXPECT_DEBUG_DEATH({
      /** \action call function under test */
      ret = Triangle_Gamma_From_Abc(a, b, c);
      /** \assert helpful assertion is thrown */
   }, ">= opposite");
#ifdef NDEBUG
   EXPECT_EQ(0.0f, ret);
#endif
}

/**
 * Compute an angle of a non-triangle
 */
TEST(MlTrigonometry, Triangle_Alpha_From_Abc_not_a_triangle1)
{
    /** \prepare sides that contain a length of zero */
   float a = 0;
   float b = 5;
   float c = a + b + 2;
   float ret;
   EXPECT_DEBUG_DEATH({
      /** \action call function under test */
      ret = Triangle_Alpha_From_Abc(a, b, c);
      /** \assert helpful assertion is thrown */
   }, "THRESHOLD_IS_ZERO");
#ifdef NDEBUG
   EXPECT_EQ(0.0f, ret);
#endif
}

/**
 * Compute an angle of a non-triangle
 */
TEST(MlTrigonometry, Triangle_Beta_From_Abc_not_a_triangle1)
{
    /** \prepare sides that contain a length of zero */
   float a = 0;
   float b = 5;
   float c = a + b + 2;
   float ret;
   EXPECT_DEBUG_DEATH({
      /** \action call function under test */
      ret = Triangle_Beta_From_Abc(a, b, c);
      /** \assert helpful assertion is thrown */
   }, "THRESHOLD_IS_ZERO");
#ifdef NDEBUG
   EXPECT_EQ(0.0f, ret);
#endif
}

/**
 * Compute an angle of a non-triangle
 */
TEST(MlTrigonometry, Triangle_Gamma_From_Abc_not_a_triangle1)
{
    /** \prepare sides that contain a length of zero */
   float a = 0;
   float b = 5;
   float c = a + b + 2;
   float ret;
   EXPECT_DEBUG_DEATH({
      /** \action call function under test */
      ret = Triangle_Gamma_From_Abc(a, b, c);
      /** \assert helpful assertion is thrown */
   }, "THRESHOLD_IS_ZERO");
#ifdef NDEBUG
   EXPECT_EQ(0.0f, ret);
#endif
}