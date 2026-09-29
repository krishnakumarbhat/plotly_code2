#include <gtest/gtest.h>
#include "ml_version_number_t.h"
#include "ml_version.h"


TEST(Get_Ml_Math_Library_Version, day_matches_expectation)
{
   Version_Number_T st_version = Get_Ml_Math_Library_Version();

   EXPECT_EQ(st_version.day, ML_MATH_LIBRARY_VERSION_DAY);
}

TEST(Get_Ml_Math_Library_Version, iteration_matches_expectation)
{
   Version_Number_T st_version = Get_Ml_Math_Library_Version();

   EXPECT_EQ(st_version.iteration, ML_MATH_LIBRARY_VERSION_ITERATION);
}

TEST(Get_Ml_Math_Library_Version, month_matches_expectation)
{
   Version_Number_T st_version = Get_Ml_Math_Library_Version();

   EXPECT_EQ(st_version.month, ML_MATH_LIBRARY_VERSION_MONTH);
}

TEST(Get_Ml_Math_Library_Version, year_matches_expectation)
{
   Version_Number_T st_version = Get_Ml_Math_Library_Version();

   EXPECT_EQ(st_version.year, ML_MATH_LIBRARY_VERSION_YEAR);
}