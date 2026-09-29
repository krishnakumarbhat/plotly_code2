/*===========================================================================*\
* Copyright 2021 Aptiv Advanced Safety and User Experience. All rights reserved.
* Confidential - Restricted Aptiv information. Do not disclose.
\*===========================================================================*/

#include <gtest/gtest.h>
#include "ml_bool.h"
#include "ml_version_number_t.h"
#include "ml_version.h"


/**
* \sdd{WI-14026}
*/
TEST(Shared_Toolbox_Version_Check_Test, WI_15353_Version_High_Enough)
{
    /** \arrange */
    /** \action call function under test */
    int version_sufficient = Ml_Math_Library_Version_Insufficient(00, 01, 01, 0);

    /** \assert */
    EXPECT_FALSE(Is_True(version_sufficient));
}

/**
* \sdd{WI-14026}
*/
TEST(Shared_Toolbox_Version_Check_Test, WI_15354_Version_Too_Low)
{
    /** \arrange */
    /** \action call function under test */
    int version_insufficient = Ml_Math_Library_Version_Insufficient(99, 01, 01, 0);
    /** \assert */
    EXPECT_TRUE(Is_True(version_insufficient));
}

/**
* \sdd{WI-14027}
*/
TEST(Shared_Toolbox_Version_Check_Test, WI_15355_Version_Equals)
{
   /** \arrange */
   /** \action call function under test */
   int version_equals = Ml_Math_Library_Version_Equals(ML_MATH_LIBRARY_VERSION_YEAR, ML_MATH_LIBRARY_VERSION_MONTH, ML_MATH_LIBRARY_VERSION_DAY, ML_MATH_LIBRARY_VERSION_ITERATION);

   /** \assert */
   EXPECT_TRUE(Is_True(version_equals));
}

/**
* \sdd{WI-14027}
*/
TEST(Shared_Toolbox_Version_Check_Test, WI_15356_Version_Equals_Not)
{
   /** \arrange */
   /** \action call function under test */
   int version_equals = Ml_Math_Library_Version_Equals(ML_MATH_LIBRARY_VERSION_YEAR + 1, ML_MATH_LIBRARY_VERSION_MONTH, ML_MATH_LIBRARY_VERSION_DAY, ML_MATH_LIBRARY_VERSION_ITERATION);

   /** \assert */
   EXPECT_FALSE(Is_True(version_equals));
}

TEST(Version_Number_T, size_is_two_bytes)
{
   EXPECT_EQ(sizeof(Version_Number_T), 2u);
}
