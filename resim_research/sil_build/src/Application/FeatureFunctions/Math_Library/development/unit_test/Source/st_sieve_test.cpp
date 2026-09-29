/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <gtest/gtest.h>
#include "ml_bool.h"
#include "ml_math.h"
#include "ml_sieve.h"
#include "ml_max_sieve_t.h"
#include "ml_min_max_sieve_t.h"
#include "ml_min_sieve_t.h"
#include "ml_math_infinity_silent.h"


#define TEST_SIEVE_SET_SIZE                (3)
#define NUMBER_OF_SIEVES_TO_TEST_IN_SET    (TEST_SIEVE_SET_SIZE + 10)


/* MSVC warns whenever INFINITY is used. Disable it for the unit tests. */
#pragma warning (disable:4756)
#pragma warning (disable:4056)

/**
 * \sdd{WI-13577}
 */
TEST(SieveTest, WI_15243_Init_Max_Sieve_Test)
{
   /** \arrange */
   Max_Sieve_T sieve;

   /** \action call function under test */
   Init_Max_Sieve(&sieve);

   /** \assert */
   EXPECT_EQ(-AS_TOOLBOX_INFINITY,sieve.known_max);
}

/**
* \sdd{WI-13571}
 */
TEST(SieveTest, WI_15244_Init_Min_Sieve_Test)
{
   /** \arrange */
   Min_Sieve_T sieve;

   /** \action call function under test */
   Init_Min_Sieve(&sieve);

   /** \assert */
   EXPECT_EQ(AS_TOOLBOX_INFINITY,sieve.known_min);
}

/**
* \sdd{WI-13573}
 */
TEST(SieveTest, WI_15245_Init_Min_Max_Sieve_Test)
{
   /** \arrange */
   Min_Max_Sieve_T sieve;

   /** \action call function under test */
   Init_Min_Max_Sieve(&sieve);

   /** \assert */
   EXPECT_EQ(-AS_TOOLBOX_INFINITY,sieve.max_sieve.known_max);
   EXPECT_EQ(AS_TOOLBOX_INFINITY,sieve.min_sieve.known_min);
}

/**
* \sdd{WI-13571}
 */
TEST(SieveTest, WI_15247_Min_Sieve_sieving_test)
{
   /** \arrange */
   Min_Sieve_T sieve;
   float32_T value;

   Init_Min_Sieve(&sieve);

   /** \action call function under test */
   Min_Sieve(&sieve, 1);
   Min_Sieve(&sieve, 0);
   value = Min_Sieve(&sieve, -1);

   /** \assert */
   EXPECT_EQ(value, 0);
}

/**
* \sdd{WI-13577}
 */
TEST(SieveTest, WI_15248_Max_Sieve_sieving_test)
{
   /** \arrange */
   Max_Sieve_T sieve;
   float32_T value;

   Init_Max_Sieve(&sieve);

   /** \action call function under test */
   Max_Sieve(&sieve, -1);
   Max_Sieve(&sieve, 0);
   value = Max_Sieve(&sieve, 1);

   /** \assert */
   EXPECT_EQ(value, 0);
}

/**
* \sdd{WI-13573}
 */
TEST(SieveTest, WI_15249_Min_Max_Sieve_sieving_test)
{
   /** \arrange */
   Min_Max_Sieve_T sieve;
   float32_T     value;

   Init_Min_Max_Sieve(&sieve);

   /** \action call function under test */
   Min_Max_Sieve(&sieve, 1);
   Min_Max_Sieve(&sieve, 0);
   value = Min_Max_Sieve(&sieve, -1);

   /** \assert */
   EXPECT_EQ(value, 0);
}

/**
* \sdd{WI-13582}
 */
TEST(SieveTest, WI_15250_Max_Sieve_set_init_test)
{
   /** \arrange */
   Max_Sieve_T sieve[TEST_SIEVE_SET_SIZE];

   /** \action call function under test */
   Init_Max_Sieve_Set(sieve, TEST_SIEVE_SET_SIZE);

   /** \assert */
   for (auto entry: sieve)
   {
       EXPECT_EQ(-AS_TOOLBOX_INFINITY,entry.known_max);
   }
}

/**
* \sdd{WI-13582}
 */
TEST(SieveTest, WI_15251_Min_Sieve_Set_init_test)
{
   /** \arrange */
   Min_Sieve_T sieve[TEST_SIEVE_SET_SIZE];

   /** \action call function under test */
   Init_Min_Sieve_Set(sieve, TEST_SIEVE_SET_SIZE);

   /** \assert */
   for (auto entry: sieve)
   {
       EXPECT_EQ(AS_TOOLBOX_INFINITY, entry.known_min);
   }
}


/**
* \sdd{WI-13582}
 */
TEST(SieveTest, WI_15252_Min_Max_Sieve_Set_init_test)
{
   /** \arrange */
   Min_Max_Sieve_T sieve[TEST_SIEVE_SET_SIZE];

   /** \action call function under test */
   Init_Min_Max_Sieve_Set(sieve, TEST_SIEVE_SET_SIZE);

   /** \assert */
   for (auto entry: sieve)
   {
       EXPECT_EQ(-AS_TOOLBOX_INFINITY,entry.max_sieve.known_max);
       EXPECT_EQ(AS_TOOLBOX_INFINITY,entry.min_sieve.known_min);
   }
}

/**
* \sdd{WI-13582}
*/
TEST(SieveTest, WI_15253_Max_Sieve_Set_test)
{
   float32_T value;
   /** \arrange */
   Max_Sieve_T sieve[TEST_SIEVE_SET_SIZE];

   Init_Max_Sieve_Set(sieve, TEST_SIEVE_SET_SIZE);

   /** \action call function under test */
   for (int i = 0; i < (NUMBER_OF_SIEVES_TO_TEST_IN_SET); i++)
   {
      Max_Sieve_Set(sieve, TEST_SIEVE_SET_SIZE, (-1.0f) * static_cast<float>(i));
      value = Max_Sieve_Set(sieve, TEST_SIEVE_SET_SIZE, static_cast<float>(i));
   }

   /** \assert */
   EXPECT_EQ(value, NUMBER_OF_SIEVES_TO_TEST_IN_SET - TEST_SIEVE_SET_SIZE - 1);
   for (int i = 0; i < (TEST_SIEVE_SET_SIZE); i++)
   {
      EXPECT_EQ(sieve[i].known_max, NUMBER_OF_SIEVES_TO_TEST_IN_SET - 1 - i);
   }
}

/**
* \sdd{WI-13582}
*/
TEST(SieveTest, WI_15254_Min_Sieve_Set_test)
{
   float32_T value;
   /** \arrange */
   Min_Sieve_T sieve[TEST_SIEVE_SET_SIZE];

   Init_Min_Sieve_Set(sieve, TEST_SIEVE_SET_SIZE);

   /** \action call function under test */
   for (int i = 0; i < (NUMBER_OF_SIEVES_TO_TEST_IN_SET); i++)
   {
      Min_Sieve_Set(sieve, TEST_SIEVE_SET_SIZE, (-1.0f) * static_cast<float>(i));
      value = Min_Sieve_Set(sieve, TEST_SIEVE_SET_SIZE, (float32_T)i);
   }

   /** \assert */
   EXPECT_EQ(value, NUMBER_OF_SIEVES_TO_TEST_IN_SET - 1);
   for (int i = 0; i < (TEST_SIEVE_SET_SIZE); i++)
   {
      EXPECT_EQ(sieve[i].known_min, -(NUMBER_OF_SIEVES_TO_TEST_IN_SET - 1 - i));
   }
}

/**
* \sdd{WI-13582}
*/
TEST(SieveTest, WI_15255_Min_Max_Sieve_Set_test)
{
   float32_T value;
   /** \arrange */
   Min_Max_Sieve_T sieve[TEST_SIEVE_SET_SIZE];

   Init_Min_Max_Sieve_Set(sieve, TEST_SIEVE_SET_SIZE);

   /** \action call function under test */
   for (int i = 0; i < (NUMBER_OF_SIEVES_TO_TEST_IN_SET); i++)
   {
      Min_Max_Sieve_Set(sieve, TEST_SIEVE_SET_SIZE, (-1.0f) * static_cast<float>(i));
      value = Min_Max_Sieve_Set(sieve, TEST_SIEVE_SET_SIZE, static_cast<float>(i));
   }

   /** \assert */
   EXPECT_EQ(value, NUMBER_OF_SIEVES_TO_TEST_IN_SET - TEST_SIEVE_SET_SIZE - 1);
   for (int i = 0; i < (TEST_SIEVE_SET_SIZE); i++)
   {
      EXPECT_EQ(sieve[i].max_sieve.known_max, NUMBER_OF_SIEVES_TO_TEST_IN_SET - 1 - i);
      EXPECT_EQ(sieve[i].min_sieve.known_min, -(NUMBER_OF_SIEVES_TO_TEST_IN_SET - 1 - i));
   }
}

/**
* \sdd{WI-13582}
*/
TEST(SieveTest, WI_15256_get_min_Max_Sieve_min_max_content_test)
{
   /** \arrange */
   Min_Max_Sieve_T sieve[TEST_SIEVE_SET_SIZE];

   Init_Min_Max_Sieve_Set(sieve, TEST_SIEVE_SET_SIZE);
   float32_T min_content;
   float32_T max_content;

   /** \action call function under test */
   for (int i = 0; i < (NUMBER_OF_SIEVES_TO_TEST_IN_SET); i++)
   {
      Min_Max_Sieve_Set(sieve, TEST_SIEVE_SET_SIZE, (-1.0f) * static_cast<float>(i));
      Min_Max_Sieve_Set(sieve, TEST_SIEVE_SET_SIZE, static_cast<float>(i));
   }

   min_content = Get_Min_Max_Sieve_Min_Content(sieve, 0);  /* Calls Get_Min_Sieve_Content() */
   max_content = Get_Min_Max_Sieve_Max_Content(sieve, 0);  /* Calls Get_Max_Sieve_Content() */

   /** \assert */
   EXPECT_EQ(min_content, -(NUMBER_OF_SIEVES_TO_TEST_IN_SET - 1));
   EXPECT_EQ(max_content, (NUMBER_OF_SIEVES_TO_TEST_IN_SET - 1));
}

/**
* \sdd{WI-13582}
*/
TEST(SieveTest, WI_15257_get_max_and_min_from_sieve_set_test)
{
    /** \arrange */
    Max_Sieve_T Max_Sieve[TEST_SIEVE_SET_SIZE];
    Min_Sieve_T Min_Sieve[TEST_SIEVE_SET_SIZE];

    float32_T min_content;
    float32_T max_content;

    Init_Min_Sieve_Set(Min_Sieve, TEST_SIEVE_SET_SIZE);
    Init_Max_Sieve_Set(Max_Sieve, TEST_SIEVE_SET_SIZE);

    /** \action call function under test */
    for (int i = 0; i < (NUMBER_OF_SIEVES_TO_TEST_IN_SET); i++)
    {
        Min_Sieve_Set(Min_Sieve, TEST_SIEVE_SET_SIZE, (-1.0f) * static_cast<float>(i));
        Max_Sieve_Set(Max_Sieve, TEST_SIEVE_SET_SIZE, static_cast<float>(i));
    }

    min_content = Get_Min_From_Min_Sieve_Set(Min_Sieve);  /* Calls Get_Min_Sieve_Content() */
    max_content = Get_Max_From_Max_Sieve_Set(Max_Sieve);  /* Calls Get_Max_Sieve_Content() */

    /** \assert */
    EXPECT_EQ(min_content, -(NUMBER_OF_SIEVES_TO_TEST_IN_SET - 1));
    EXPECT_EQ(max_content, (NUMBER_OF_SIEVES_TO_TEST_IN_SET - 1));
}

/**
* \sdd{WI-13577}
* \sdd{WI-13582}
* \sdd{WI-13573}
*/
TEST(SieveTest, WI_15258_Is_Sieved_Value_Valid_test)
{
   /** \arrange */
   float32_T value_invalid_plus;
   float32_T value_invalid_minus;
   float32_T value_valid         = 1.0f;
   boolean_T f_is_invalid_plus;
   boolean_T f_is_invalid_minus;
   boolean_T f_is_valid;
   Min_Sieve_T min_sieve;
   Max_Sieve_T max_sieve;
   Init_Min_Sieve(&min_sieve);
   Init_Max_Sieve(&max_sieve);
   value_invalid_minus = Get_Max_Sieve_Content(&max_sieve);
   value_invalid_plus = Get_Min_Sieve_Content(&min_sieve);
   /** \action call function under test */
   f_is_invalid_plus  = Is_Sieved_Value_Valid(value_invalid_plus);
   f_is_invalid_minus = Is_Sieved_Value_Valid(value_invalid_minus);
   f_is_valid         = Is_Sieved_Value_Valid(value_valid);

   /** \assert */
   EXPECT_EQ(f_is_invalid_plus, FALSE);
   EXPECT_EQ(f_is_invalid_minus, FALSE);
   EXPECT_EQ(f_is_valid, TRUE);
}

